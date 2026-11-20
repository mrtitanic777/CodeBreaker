// cb/formats/powerpc_image.hpp -- what a PowerPC PE image states about its own code.
//
//     const powerpc_image::Image image = powerpc_image::read("default.exe");     // an .xex's basefile, or any PowerPC PE
//     image.base()                      the address the image is built for
//     image.spans()                     where the file's bytes sit in memory (address, file offset, size)
//     image.sections()                  the section table, at the addresses the image loads at
//     image.functions()                 the function starts the image states or its calls show, ascending
//     image.imports()                   {address: name}: the stubs calls to other modules go through
//     image.inlined()                   {address: name}: register save/restore runs entered by `bl`
//     image.code_begin() / code_end()   the code: every executable section
//     image.extents()                   {start: size, prologue size, handler}: the exception table (.pdata)
//     image.entry()                     the entry point's address (0 when there is none)
//     image.word(address)               one big-endian instruction word
//     image.bytes_at(address, size)     the bytes at an address
//     virtual_methods(image)            the virtual functions the run-time type information names
//
// Big-endian PowerPC code in a PE container (machine type 0x1F2), as Microsoft's PowerPC compilers produce it (an Xbox 360
// executable is one inside its .xex: `xex::basefile` writes the PE image and, beside it, <image>.json with the load address the
// header states). Three things about such an image are stated by the image and need no guessing:
//
// Functions. The exception table (.pdata) has one record per function that has unwind information: its start and its length.
// Small leaf functions have no record, so every `bl` target inside the code is a function start as well.
//
// Imports. A call to another module goes through a 16-byte stub in the code: two record words naming the library (by index) and
// the ordinal, then `mtctr r11; bctr`. The loader rewrites the two words into a load of the function's address. A stub is named
// `import<library>_ord_<n>`.
//
// Prologue and epilogue helpers. A function that saves many registers does not do so inline: it calls into a run of
// `std rN,-X(r1)` instructions that ends `stw r12,-8(r1); blr` (entering at the instruction for the first register it needs), and
// leaves by jumping into the matching run of `ld rN,-X(r1)` that ends `lwz r12,-8(r1); mtlr r12; blr`. The same exists for the
// floating-point registers, relative to r12. These runs are part of every caller's own prologue and epilogue, not functions of
// their own: the ones entered by `bl` are listed in inlined() so their instructions are read as the caller's. (The restore run that is reached by a plain jump is named in run_names() and not listed in inlined().)
//
// Classes. Microsoft's C++ compilers leave run-time type information beside each virtual table of a class that has any: the word
// before the table points at a locator (signature 0) whose fourth word points at the class's type descriptor, which holds the
// class's decorated name (`.?AVCFoo@@`). Where that chain is intact, every function the table lists is a virtual function of that
// class, named for certain; the standard library's own classes are left out.
//
// Some files hold the image as it sits in memory (every section at its own address) rather than packed as a file on disk; their
// section headers still give file positions that are no longer true. Which of the two a file is is decided by where the exception
// table's entries point at code.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/xex.hpp"

namespace cb::formats::powerpc_image {

inline constexpr std::uint16_t kMachine = 0x01F2;

using Address = std::uint32_t;

// `size` bytes of the file at `file_offset` live at `va`.
struct Span {
    Address va = 0;
    std::uint64_t file_offset = 0;
    std::uint64_t size = 0;
    friend bool operator==(const Span&, const Span&) = default;
};

struct Section {
    std::string name;
    Address va = 0;
    std::uint32_t size = 0;
    std::uint32_t characteristics = 0;
    bool executable() const { return (characteristics & 0x20000000u) != 0 || name == ".text"; }
    friend bool operator==(const Section&, const Section&) = default;
};

// A function the exception table (.pdata) states.
struct Extent {
    std::uint32_t size = 0;            // bytes
    std::uint32_t prologue_size = 0;   // bytes
    bool has_handler = false;          // it has an exception handler
    friend bool operator==(const Extent&, const Extent&) = default;
};

// A virtual function the run-time type information names: slot `slot` of the table of class `class_name` (`Outer::Inner`).
struct VirtualMethod {
    Address address = 0;
    std::string class_name;
    std::uint32_t slot = 0;
    friend bool operator==(const VirtualMethod&, const VirtualMethod&) = default;
};

class Image {
public:
    // A PowerPC PE file. NotFoundError / IoError for a file that cannot be read; FormatError when it is not a PE image or its machine
    // is not PowerPC; the load address is the one in <file>.json (`"base"`, written by `xex::basefile`) when there is one, else the
    // header's.
    static Image read(const std::filesystem::path& file);
    // The image inside an .xex: its basefile (`xex::basefile`: unpacked once per distinct file, its load address beside it), read.
    // ToolError if the xex engine cannot unpack it.
    static Image read_xex(const xex::Tools& tools, const std::filesystem::path& xex);
    // The same from the file's bytes (`name` only goes into messages). `base` overrides the header's load address.
    static Image parse(Bytes file, std::optional<Address> base = std::nullopt, std::string_view name = "image");

    ByteView file() const { return file_; }
    Address base() const { return base_; }
    // The file is the image as laid out in memory (its section headers' file positions are not true).
    bool memory_layout() const { return memory_layout_; }
    std::span<const Span> spans() const { return spans_; }
    std::span<const Section> sections() const { return sections_; }
    // Function starts, ascending: the exception table's, and the targets of `bl` in the code, less the register runs and the import
    // stubs. Empty for an image with no code.
    std::span<const Address> functions() const { return functions_; }
    const std::map<Address, std::string>& imports() const { return imports_; }
    const std::map<Address, std::string>& inlined() const { return inlined_; }
    // Every register save/restore run in the code, by the address of each instruction of it: `__savegprlr_14` ... The save runs are
    // inlined() too.
    const std::map<Address, std::string>& run_names() const { return run_names_; }
    // The code: from the first executable section's address to the end of the last (0, 0 for an image with no executable section).
    Address code_begin() const { return code_begin_; }
    Address code_end() const { return code_end_; }
    bool has_code() const { return has_code_; }
    // The exception table's records by function start.
    const std::map<Address, Extent>& extents() const { return extents_; }
    // The entry point's address; 0 for an image with none.
    Address entry() const { return entry_; }

    // The file offset of an address, or nullopt when no span holds it.
    std::optional<std::uint64_t> offset(Address va) const;
    // One big-endian instruction word; nullopt where no span holds the address or the file ends.
    std::optional<std::uint32_t> word(Address va) const;
    // Up to `size` bytes at an address (fewer where the file ends; none where no span holds the address).
    ByteView bytes_at(Address va, std::size_t size) const;
    // The image as it sits in memory, for running it: each span's bytes at its address.
    struct Region {
        Address va = 0;
        ByteView bytes;
    };
    std::vector<Region> regions() const;

private:
    Image() = default;

    Bytes file_;
    Address base_ = 0;
    bool memory_layout_ = false;
    std::vector<Span> spans_;
    std::vector<Section> sections_;
    std::vector<Address> functions_;
    std::map<Address, std::string> imports_, inlined_, run_names_;
    Address code_begin_ = 0, code_end_ = 0;
    bool has_code_ = false;
    std::map<Address, Extent> extents_;
    Address entry_ = 0;
};

// Each virtual function whose table still links, through its locator, to the class's type descriptor (see the notes at the top):
// by address, a function once (the first table that lists it). The classes of the standard library are left out.
std::vector<VirtualMethod> virtual_methods(const Image& image);

}  // namespace cb::formats::powerpc_image
