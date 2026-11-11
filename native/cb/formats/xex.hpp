// cb/formats/xex.hpp -- an Xbox 360 executable (.xex): the PowerPC image inside it, and what its header says.
//
// A .xex wraps a PE image in its own header: the image is compressed (LZX or by blocks), usually encrypted with a
// per-title key, and the libraries its imports come from are named in the header, not in the image. Reading the
// header is done here, over CodeBreaker's one XEX parser (native/xex/src/xex_file.h, which the xex program and the
// recompiled-game runtime compile too). Decrypting and decompressing is the xex program's (native/xex, built with
// CMake): `basefile` runs it once per distinct file and keeps the image, so the PowerPC lane reads a .xex like any
// PowerPC image.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::xex {

inline constexpr std::array<std::uint8_t, 4> kMagic = {'X', 'E', 'X', '2'};
inline constexpr std::uint32_t kFileFormatInfo = 0x000003FF;
inline constexpr std::uint32_t kImportLibraries = 0x000103FF;
inline constexpr std::uint32_t kExecutionInfo = 0x00040006;

// Does the file start with XEX2? (Reads four bytes.)
bool probe(const std::filesystem::path& path);

// One entry of the header's optional-header table. `value` is the data itself for a key whose low byte is 0 or 1,
// else the offset of the data in the header.
struct OptionalHeader {
    std::uint64_t offset = 0;   // where the entry sits in the file
    std::uint32_t key = 0;
    std::uint32_t value = 0;
};

// The one reader of the optional-header table (xex_edit, xex_imports, god, the FATX carver read through it): a
// cursor that reads an entry per call, so a reader that stops at the key it wants reads no further and a table that
// is damaged past that key does not matter. The view must outlive the cursor.
class OptionalHeaders {
public:
    // TruncatedError if the table's count is not there.
    explicit OptionalHeaders(ByteView xex);
    // The next entry; nullopt at the end of the table. TruncatedError if the table is cut short.
    std::optional<OptionalHeader> next();
    std::uint32_t count() const { return count_; }

private:
    ByteView data_;
    std::uint32_t count_ = 0;
    std::uint32_t index_ = 0;
};

// The first entry with `key`, reading the table in order and no further than that entry.
std::optional<OptionalHeader> find_optional_header(ByteView xex, std::uint32_t key);

// The title the image belongs to, from the header's execution info.
struct TitleIds {
    std::uint32_t media_id = 0;
    std::uint32_t title_id = 0;
};
// {0, 0} for data that is not an XEX2 header or has no execution info; TruncatedError if the table is cut short
// before it says so.
TitleIds ids(ByteView xex);

// A static library the image was built with.
struct Library {
    std::string name;
    std::string version;   // "2.0.1861.0"
};

// What the header states.
struct Header {
    std::optional<std::string> name;          // the image's own name (bootanim.dll, default.exe)
    std::optional<std::uint32_t> base;        // its load address
    std::optional<std::uint32_t> entry;       // its entry point
    std::vector<std::string> imports;         // library names in the order the image's import stubs number them
    std::vector<Library> libraries;           // what it was built with
};

// FormatError if the data is not a parseable XEX2 file.
Header parse_header(ByteView xex);
Header read_header(const std::filesystem::path& path);

// Where the xex program is and where unpacked images are kept (cb::paths will own these; callers pass them).
struct Tools {
    std::filesystem::path xex_program;    // xex.exe
    std::filesystem::path unpacked_dir;   // state/unpacked/xex
};

// The PowerPC image inside `xex`, extracted once per distinct file (named by its contents, so a changed .xex is a
// new basefile and nothing is stale); the header's facts are written beside it as <image>.json. NotFoundError if the
// xex program is not built; ToolError if it fails.
std::filesystem::path basefile(const Tools& tools, const std::filesystem::path& xex);

// Write `xex` as an unencrypted XEX with basic compression: every header byte kept (optional headers, security
// info, imports, exports, resources), the file format info replaced by one block holding the whole image.
// XenonRecomp and the Xbox 360 runtime read only this form. The security info's hashes no longer match; nothing that
// reads it checks them. FormatError if the file is not an XEX2 module with a file format info header.
void write_basic(const Tools& tools, const std::filesystem::path& xex, const std::filesystem::path& out);

// The library names for a basefile written by basefile() -- the .xex header named them -- or nullopt for an image
// that did not come from a .xex.
std::optional<std::vector<std::string>> import_libraries(const std::filesystem::path& image);

// Run `xex <args>` with the program's own stdout and stderr (info, list, extract, resources, xml, idc, compress,
// decompress, binary, encrypt, decrypt, machine, remove-limits, bounding-path, patch, selftest); returns its exit code.
int run(const Tools& tools, const std::vector<std::string>& args);

}  // namespace cb::formats::xex
