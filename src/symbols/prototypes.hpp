// symbols/prototypes.hpp -- how the Windows headers declare a function: result, convention, parameters.
//
//     PrototypeTable table(read_headers(HeaderFolders::standard(openwatcom, reference)));
//     table.of("GetDlgItem")             {returns "HWND", convention "stdcall", marker "WINAPI",
//                                         parameters [{"HWND","hDlg"}, {"int","nIDDlgItem"}], header "winuser.h"}
//     table.of("GETDLGITEM", Bits::W16)  the 16-bit declaration (found whatever the capitals)
//     table.declaration("LZRead", Bits::W16)   "int WINAPI LZRead(HFILE, void FAR *, int)"
//     table.spelling("GETDLGITEM")       "GetDlgItem"    (16-bit name tables are all capitals)
//
// symbols/api_arguments answers one question about an import -- how many bytes of arguments it removes -- from the
// exporting module's code. This is the rest of the declaration, for reading: what the function returns and what each
// parameter is.
//
// Read from the headers reconstructions are compiled against (OpenWatcom's h/nt and h/win), so a declaration given
// here is one the compiler accepts. Those headers leave parameters unnamed; where Microsoft's own headers of the time
// are at hand (the corpus's reference folder) and declare the same number of parameters, their parameter names are
// added, and functions only they declare are taken from them.
//
// Reading the headers (`read_headers`) is the slow part; the caller keeps what it makes in the knowledge base (platforms
// win32 and win16, kind "prototype"), made again when `stamp()` of the folders changes. A PrototypeTable answers from
// what it was given and says to the knowledge reads what it looked up (cb/knowledge/reads.hpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/ordered_map.hpp"

namespace cb::symbols::prototypes {

// How many bits the headers are for: Win32 (h/nt, MSTOOLS/INCLUDE, INC32), Win16 (h/win, INC16) or Win64 (the Windows SDK of
// this computer: its um and shared folders, where the x64 declarations are -- the period headers declare some functions as
// the Alpha and MIPS had them, RtlVirtualUnwind with six arguments where x64's has eight).
enum class Bits { W16 = 16, W32 = 32, W64 = 64 };

struct Parameter {
    std::string type;
    std::string name;   // "" where the header leaves the parameter unnamed
    friend bool operator==(const Parameter&, const Parameter&) = default;
};

// One declaration: how a header declares a function.
struct Prototype {
    std::string returns;
    std::string convention;   // "stdcall" or "cdecl"
    std::string marker;       // as the header wrote it: WINAPI, WINAPIV, STDAPI, ...
    std::vector<Parameter> parameters;
    std::string header;       // the header's file name, lower case
    friend bool operator==(const Prototype&, const Prototype&) = default;
};

// {name: prototype} in the order the headers declare them (the first declaration of a name is kept).
using PrototypeMap = OrderedMap<std::string, Prototype>;

// The header folders, the compiler's first (it is the one whose declarations are kept), then Microsoft's.
struct HeaderFolders {
    std::vector<std::filesystem::path> w32;
    std::vector<std::filesystem::path> w16;
    std::vector<std::filesystem::path> w64;

    // The folders of OpenWatcom (`openwatcom_root`, may be empty) and of the period SDK and DDK (`reference_root`)
    // that exist, and the newest Windows SDK's um and shared folders when one is installed (w64).
    static HeaderFolders standard(const std::filesystem::path& openwatcom_root, const std::filesystem::path& reference_root);
};

// What the headers declare.
struct Headers {
    PrototypeMap w32;
    PrototypeMap w16;
    PrototypeMap w64;   // only what read_headers64 reads (read_headers leaves it empty: the SDK is large and only the 64-bit lanes ask)
};

// A change of the reader, to be part of every stamp.
inline constexpr int kReaderRevision = 7;

// Text that differs whenever the reader or a header folder (its .h count and its modification time) does.
std::string stamp(const HeaderFolders& folders);
// The same for the Win64 folders alone.
std::string stamp64(const HeaderFolders& folders);

// Every header of every folder: `<result> <CONVENTION> name(...);` and COM's `STDAPI_(type) name(...);`. The first
// folder's declaration is kept; a later folder's declaration names the parameters a first one left unnamed (when it
// declares as many), and adds the functions only it declares. Unreadable folders and files are skipped.
Headers read_headers(const HeaderFolders& folders);
// The Win64 folders' declarations.
PrototypeMap read_headers64(const HeaderFolders& folders);

// The newest Windows SDK's include folders on this computer (`<kit>/Include/<version>/um` and `shared`): the WindowsSdkDir
// environment's kit, else the standard place; empty when none is installed.
std::vector<std::filesystem::path> windows_sdk_include();

// The declarations, found by name.
class PrototypeTable {
public:
    explicit PrototypeTable(Headers headers);

    const Headers& headers() const { return headers_; }

    // The prototype of a Windows function, or nullptr. Capitals do not matter (the headers' spelling is tried).
    const Prototype* of(std::string_view name, Bits bits = Bits::W32) const;

    // The name as the headers write it (`GETDLGITEM` -> `GetDlgItem`, `_WSPRINTF` -> `wsprintf`); unchanged when they
    // do not.
    std::string spelling(std::string_view name) const;

    // The declaration as one line of C, or nullopt.
    std::optional<std::string> declaration(std::string_view name, Bits bits = Bits::W32) const;

private:
    const PrototypeMap& map(Bits bits) const { return bits == Bits::W16 ? headers_.w16 : bits == Bits::W64 ? headers_.w64 : headers_.w32; }

    Headers headers_;
    std::unordered_map<std::string, std::string> spelling_;   // lookup only: {NAME: name}, the 16-bit spelling wins
};

}  // namespace cb::symbols::prototypes
