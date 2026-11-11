// cb/java/console/xdk_scan.hpp -- read what the installed Xbox 360 SDK offers from the SDK's own headers.
//
// The converter knows the target platform, and the user supplies the SDK -- so what the platform offers (and
// mandates) must be READ FROM THE SDK'S OWN HEADERS, never hand-typed from memory. This walks the SDK include
// tree, enumerates the exported function families, and maps the ones that are PRESENT to the subsystems the port
// needs.
//
// The honest split:
//   * DETECTION (is this API family in THIS SDK? which exact functions?) comes 100% from the scanned headers.
//   * INTERPRETATION (which family serves which subsystem, and why it matters) is the converter's platform
//     knowledge, encoded as the patterns below -- but every pattern is GATED on its family being detected, and the
//     functions it reports are the ones actually found. Against an SDK that lacks XContent*, the "saves need an
//     STFS package" requirement is simply NOT emitted.
#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::console::xdk_scan {

// What a pattern says about the platform: the platform mandates it, it is the API a stub resolves to, or it is
// a native feature the port could add.
enum class Kind { Requirement, Equivalent, Opportunity };

// How bad it is to ignore a requirement.
enum class Severity { Correctness, Cert, Compat };

// How big an opportunity is to take (small, medium).
enum class Effort { Small, Medium };

// One piece of platform knowledge, gated on an SDK family being present: `needs` is the function that MUST exist
// in the scanned SDK for the pattern to be claimed, `family` the name prefix of the functions reported for it.
// Only `title` and `why` are interpretation; the reported functions come from the scan.
struct Pattern {
    std::string needs;
    std::string family;
    Kind kind = Kind::Requirement;
    std::string sub;                     // the subsystem: save, system, network, video, input, audio, io, i18n
    std::optional<Severity> sev;         // requirements only
    std::optional<Effort> effort;        // opportunities only
    std::string title;
    std::string why;
};

// Every pattern, requirements first, then equivalents, then opportunities.
const std::vector<Pattern>& patterns();

// A pattern and what the scan found for it.
struct Detection : Pattern {
    bool present = false;                // `needs` was found in the headers
    std::vector<std::string> functions;  // the functions of the family actually found (sorted, at most kMaxListed)
    std::size_t func_count = 0;          // how many there are in all
    std::string header;                  // the header that declares `needs` ("" when absent)
};

// At most this many of a family's functions are listed (the rest are counted).
inline constexpr std::size_t kMaxListed = 12;

struct ScanResult {
    std::optional<std::filesystem::path> include;    // the header folder scanned; empty when there was none
    std::map<std::string, std::string> funcs;        // function name -> header file name, for every function found
    std::vector<Detection> found;                    // patterns whose `needs` is in the SDK
    std::vector<Detection> missing;                  // patterns the SDK does not support
};

// Every exported-looking XDK entry point (XFoo(, XamFoo(, D3DXFoo(, MmFoo(, KeFoo(, NtFoo(, VdFoo() that a header
// of `include_dir` mentions, with the file name of the first header (in walk order) that does. Empty when the
// folder does not exist.
std::map<std::string, std::string> scan_functions(const std::filesystem::path& include_dir);

// The same over one header's text: each entry point once, in order of appearance.
std::vector<std::string_view> entry_points_in(std::string_view header_text);

// The 360 header folder inside an SDK root (include/xbox), else include/, else the root itself; empty when none
// of them is a folder.
std::optional<std::filesystem::path> sdk_include_dir(const std::filesystem::path& sdk_root);

// Scan the installed SDK and report the capabilities and requirements that are actually present in it. An empty
// `sdk_root` (no SDK) finds nothing: every pattern is `missing`.
ScanResult detect(const std::optional<std::filesystem::path>& sdk_root);

}  // namespace cb::java::console::xdk_scan
