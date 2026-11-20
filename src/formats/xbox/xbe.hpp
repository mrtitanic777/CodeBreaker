// formats/xbox/xbe.hpp -- original Xbox XBE executables: header, certificate, sections, and the program as a 32-bit PE image.
//
//     xbe::Xbe x = xbe::Xbe::parse(bytes);
//     x.title_name, x.title_id_text(), x.region_text(), x.sections(), x.entry_point()
//     Bytes pe = x.pe_image();                              the same bytes at the same addresses, for the 32-bit Windows lane
//     std::filesystem::path p = xbe::to_pe(file, folder);   that image written once per distinct file, named by its contents
//
// XBE header (little-endian): 0x104 base address, 0x10C size of image, 0x118 certificate address (a virtual address), 0x11C section count,
// 0x120 section headers (VA), 0x128 entry point and 0x148 kernel thunk (both XORed with a key). The certificate holds the title id (0x08), the
// title name (0x0C, 40 UTF-16LE units), the allowed media (0x9C), region (0xA0), ratings (0xA4), disc number (0xA8) and version (0xAC). A
// section header is 0x38 bytes: flags, virtual address, virtual size, file offset, file size, the VA of its name.
//
// The entry point and kernel thunk address are XOR-obfuscated with one key per console kind (retail, debug, Chihiro); the key that lands the
// address inside the image is the one used.
//
// The original Xbox runs x86 code, so its programs go down the 32-bit Windows lane once they are in the shape that lane reads. `pe_image` makes
// that: each section at its address, the XBE's own header at the base replaced by PE headers, the entry point, and the kernel's functions as
// imports. The kernel thunk table already has the shape of a PE import address table -- one word per import, the kernel's ordinal with the top
// bit set, a zero at the end -- so it becomes the import table of `xboxkrnl.exe`, whose ordinals symbols/ordinal_names.json names.
//
// Errors: FormatError (TruncatedError) for bytes that are not an XBE or whose tables lie outside the file or the image; ToolError-free.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::xbe {

inline constexpr std::string_view kMagic = "XBEH";

// Which console kind's key unscrambled an address.
enum class KeySet { Retail, Debug, Chihiro, Unknown };
std::string_view key_set_name(KeySet keys);   // "retail" "debug" "chihiro" "unknown"

struct Unscrambled {
    std::uint32_t address = 0;
    KeySet keys = KeySet::Unknown;   // Unknown: no key landed it in the image; the retail key was applied
};

struct Section {
    std::string name;   // Latin-1 decoded; empty when the header names none
    std::uint32_t flags = 0, vaddr = 0, vsize = 0, raw_addr = 0, raw_size = 0;
};

// Section flags the PE image reads.
inline constexpr std::uint32_t kSectionWritable = 0x1;
inline constexpr std::uint32_t kSectionExecutable = 0x4;

class Xbe {
public:
    static Xbe parse(ByteView file);

    std::uint32_t base = 0, size_of_image = 0, size_of_headers = 0, cert_addr = 0;
    std::uint32_t section_count = 0, section_header_addr = 0, init_flags = 0;
    std::uint32_t entry_raw = 0, thunk_raw = 0;   // as stored, still scrambled
    std::uint32_t cert_size = 0, title_id = 0, allowed_media = 0, region = 0, ratings = 0, disk_number = 0, version = 0;
    std::string title_name;

    Unscrambled entry_point() const;
    Unscrambled kernel_thunk() const;
    // The section table.
    std::vector<Section> sections() const;

    // "AA-nnn": two publisher letters and the three-digit game number (the publisher as four hex digits when it is not two printable letters).
    std::string title_id_text() const;
    // "North America|Japan"; the number as 0x.. when no known bit is set.
    std::string region_text() const;
    std::string media_text() const;

    // The program as a PE image (see the header). FormatError for a section the PE headers cannot precede, or one outside the image.
    Bytes pe_image() const;

private:
    Bytes file_;
};

// The XBE at `path` as a PE image, written under `folder` as <name>-<first 12 hex of the file's SHA-1>.exe unless that file is already there.
std::filesystem::path to_pe(const std::filesystem::path& path, const std::filesystem::path& folder);

// `xbe info|sections <default.xbe>`: the module's command line.
cli::Command command();

}  // namespace cb::formats::xbox::xbe
