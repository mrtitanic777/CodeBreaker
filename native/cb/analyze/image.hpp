// cb/analyze/image.hpp -- a module as the analyses see it, made from a PE, NE or LE file.
//
//     Image pe = image::load_pe(formats::pe::Image::open(path));      // a 32-bit module: its own addresses
//     Image ne = image::load(image::ne_spec(layout));                // a 16-bit one, as the decompiler lays it out
//     Image le = image::load(image::le_spec(layout));                // a driver
//
// One address space per module, whatever its format (see analyze/native.hpp for the Image itself). A 32-bit module's
// addresses are its own. A 16-bit module is laid out as the decompiler lays it out (each segment at selector * 0x10000,
// relocations applied, each imported function at an address standing for it), and so is a driver, so an address an
// analysis reports is the address the decompiler and the function store use.
//
// What each format gives the Image:
//   PE   sections as spans, the import address table, the base relocations as the places the loader writes an address,
//        exports and the entry point as the roots; function starts from the exports, the entry point and the standard
//        prologues (`push ebp; mov ebp,esp`, with or without the hot-patch `mov edi,edi`), then discover() adds the
//        starts the code gives away.
//   NE   a NeLayout: what the decompiler's describe step makes of the module (segment bytes with relocations applied,
//        function starts, imports, the places relocations write). The step is decompile/describe's; the layout is its
//        result in typed form.
//   LE   an LeLayout, likewise, plus the device descriptor block: the control procedure, the API procedures and the
//        service table are entered from outside and are function starts.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/analyze/native.hpp"
#include "cb/formats/pe.hpp"

namespace cb::analyze::image {

// ---- the address spaces of the 16-bit and driver layouts -----------------------------------------------------------

inline constexpr std::int64_t kFirstSelector = 0x1000;      // segment n's selector is kFirstSelector + 8 * (n - 1)
inline constexpr std::int64_t kExternSelector = 0xE000;     // imported function k stands at kExternSelector:16*k
inline constexpr Va kLeBase = 0x01000000, kLeStride = 0x01000000;   // object n sits at kLeBase + (n-1) * kLeStride

constexpr Va ne_address(int segment, std::uint32_t offset) { return ((kFirstSelector + 8 * (segment - 1)) << 16) + offset; }
constexpr Va le_address(int object, std::uint32_t offset) { return kLeBase + (object - 1) * kLeStride + offset; }
constexpr Va extern_address(std::size_t index) { return (kExternSelector << 16) + 16 * static_cast<Va>(index); }

// The generated name of a function nothing names: "sub_%06X" (PE: the offset from the image base), "sub_%d_%04X" (NE:
// segment and offset), "sub_%d_%05X" (LE: object and offset).
std::string auto_name(ImageKind kind, Va image_base, Va va);

// ---- PE ---------------------------------------------------------------------------------------------------------------

// The recipe of a 32-bit module's Image (without the function starts discovery adds). UnsupportedError for a module of
// another machine.
ImageSpec pe_spec(const formats::pe::Image& pe);
// The Image of a module, with its functions discovered and named.
Image load_pe(const formats::pe::Image& pe);

// ---- NE ---------------------------------------------------------------------------------------------------------------

struct NeSegment {
    int index = 0;                    // 1-based
    Bytes bytes;                      // as laid out, relocations applied; empty: the segment holds nothing
    bool code = false;
    std::int64_t file_length = 0;     // the bytes the file holds (0: the segment has no file data, it is only reserved)
};

struct NeFunction {
    int segment = 0;
    std::uint32_t offset = 0;
    std::string name;
};

struct NeLayout {
    std::vector<NeSegment> segments;
    std::vector<NeFunction> functions;                       // every function start the description found
    std::map<int, std::uint32_t> code_length;                // per code segment: where its code ends
    std::vector<std::string> imports;                        // "MODULE!name" by index: the k-th stands at extern_address(k)
    std::vector<std::pair<int, std::uint32_t>> entry_points; // (segment, offset) of the exported entries and the program's entry
    int data_segment = 0;                                    // the automatic data segment (0: none)
    std::vector<std::pair<Va, Pointer>> pointers;            // where relocation writes an address, in the file's order
};
ImageSpec ne_spec(const NeLayout& layout);

// ---- LE ---------------------------------------------------------------------------------------------------------------

struct LeObject {
    int index = 0;                    // 1-based
    Bytes bytes;
    bool code = false;
    bool writable = false;
};

struct LeLayout {
    std::vector<LeObject> objects;
    std::map<Va, std::string> functions;                     // start -> name (the description's)
    std::map<Va, std::string> imports;                       // service address -> "VMM!name" / "VXD!name"
    std::vector<std::pair<Va, Pointer>> pointers;            // internal 32-bit fixups: where an address is written
    bool has_pointers = false;
    std::optional<Va> device_descriptor;                     // the DDB's address
};
ImageSpec le_spec(const LeLayout& layout);

// ---- all formats ------------------------------------------------------------------------------------------------------

// The Image of a recipe with its functions discovered; the starts discovery finds are named by auto_name.
// `image_base` is the PE's preferred base (the name of a PE function is its offset from it); 0 for the others.
Image load(ImageSpec spec, Va image_base = 0);

}  // namespace cb::analyze::image
