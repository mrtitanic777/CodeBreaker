// cb/formats/xex_imports.hpp -- an Xbox 360 module's imports: library, ordinal, name, function or variable, slot, thunk.
//
//     for (const xex_imports::Import& i : xex_imports::imports(xex_bytes, image, names))     // image: the module's basefile
//
// Works for any module, a title or a system one (any load address). Names come from the knowledge base's console ordinals (the
// caller hands them in as a function); an ordinal missing there is named <library>_<ordinal>.
//
// Each imported function has two records: a 4-byte slot (filled by the loader) and a 16-byte code thunk (which the recompiler turns
// into its `__imp__Name` function). An imported variable has only the slot. The import header (0x000103FF) is: the total size, the
// size of the string table, the number of strings, the library names (each padded to 4 bytes), then import blocks until the total
// is used -- and a library can have several blocks (dashboard 17559's dash.xex has two for xam). A block names its library by index
// in its byte at 0x25 (early devkit dashboards -- 2255, 2258, 2832 -- set other bits in the byte before it), then lists the
// addresses of its records; a record's word in the image has the ordinal in its low 16 bits and, for a thunk, a non-zero top byte.
//
// `xex-imports <module.xex>` prints a header line and one tab-separated line per import.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/powerpc_image.hpp"
#include "cb/formats/xex.hpp"

namespace cb::formats::xex_imports {

inline constexpr std::uint32_t kImportHeader = 0x000103FF;

struct Import {
    std::string library;                    // "xam.xex"
    std::uint32_t ordinal = 0;
    std::string name;                       // "NetDll_WSAStartup", or "xam_1" when the ordinal has no known name
    bool is_function = false;               // it has a thunk (else it is a variable)
    std::optional<std::uint32_t> slot;      // the 4-byte record the loader fills
    std::optional<std::uint32_t> thunk;     // the 16-byte code thunk of a function
    friend bool operator==(const Import&, const Import&) = default;
};

// The name of an ordinal of a library ("xam.xex", 1), when it has one.
using OrdinalNames = std::function<std::optional<std::string>(std::string_view library, std::uint32_t ordinal)>;

// The imports of the XEX whose bytes are `xex_file`, by library and ordinal; `image` is the module's PowerPC image (its basefile),
// which says what each record holds. None for a module with no import header. FormatError / TruncatedError for a damaged header.
std::vector<Import> imports(ByteView xex_file, const powerpc_image::Image& image, const OrdinalNames& names);

struct Environment {
    xex::Tools tools;            // where the xex program is and where unpacked images are kept (the basefile is read through them)
    OrdinalNames ordinal_name;   // the knowledge base's console ordinals; without it every import is named <library>_<ordinal>
};

// `xex-imports <module.xex>`.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xex_imports
