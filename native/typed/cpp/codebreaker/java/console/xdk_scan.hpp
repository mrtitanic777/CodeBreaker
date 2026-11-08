// codebreaker/java/console/xdk_scan.hpp -- C++ port of codebreaker.java.console.xdk_scan.
//
// xdk_scan.py -- AUTO-DETECT platform capabilities & requirements from the INSTALLED Xbox 360 SDK.
//
// The converter knows the target platform, and the user supplies the SDK -- so what the platform offers
// (and mandates) must be READ FROM THE SDK'S OWN HEADERS, never hand-typed from memory. This walks the SDK
// include tree, enumerates the exported function families, and maps the ones that are PRESENT to the
// subsystems the port needs.
//
// The honest split:
//   * DETECTION  (is this API family in THIS SDK? which exact functions?) -> 100% from the scanned headers.
//   * INTERPRETATION (which family serves which subsystem, and why it matters) -> the converter's platform
//     knowledge, encoded as PATTERNS below -- but every pattern is GATED on its family being detected, and
//     the functions it reports are the ones actually found. Against an SDK that lacks XContent*, the
//     "saves need an STFS package" requirement is simply NOT emitted.
//
// Consumed by platform_coverage.py to render SDK-derived requirements/opportunities/equivalents.
#pragma once

#include "py.hpp"

namespace codebreaker::java::console::xdk_scan {

// Exported-looking XDK entry points: XFoo(, XamFoo(, D3DXFoo(, MmFoo(, KeFoo(, NtFoo(, VdFoo(.
extern const py::re::Pattern _FUNC_RE;

// One entry of PATTERNS (a dict in the Python). 'sev' is present on requirements only and 'effort' on
// opportunities only, so each is an optional.
struct Pattern {
    py::str needs;
    py::str family;
    py::str kind;
    py::str sub;
    std::optional<py::str> sev;
    std::optional<py::str> effort;
    py::str title;
    py::str why;
};

// A PATTERNS entry as detect() reports it: the pattern plus what the scan found (dict(p) + 5 keys).
struct Detection : Pattern {
    bool present = false;
    py::list<py::str> functions;
    int64_t func_count = 0;
    py::str header;
};

// detect()'s result: {'include': dir, 'funcs': {name: header}, 'found': [...], 'missing': [...]}.
struct ScanResult {
    std::optional<py::str> include;        // None when no include dir was found
    py::dict<py::str, py::str> funcs;
    py::list<Detection> found;
    py::list<Detection> missing;
};

// {function_name: header_basename} for every exported-looking function in the SDK headers.
// (include_dir: a str or None)
py::dict<py::str, py::str> scan_functions(const std::optional<py::str>& include_dir);

// The 360 header dir inside an SDK root (include/xbox), else the root itself if headers live there.
std::optional<py::str> sdk_include_dir(const std::optional<py::str>& sdk_root);

// PATTERNS: platform knowledge, each GATED on an SDK family being present. `needs` = the function (or
// family prefix) that MUST exist in the scanned SDK for this to be claimed; `kind` = requirement (the
// platform mandates it) / equivalent (the API to resolve a stub to) / opportunity (a native win). The
// reported functions come from the scan; only `why`/`title` are interpretation.
extern const py::list<Pattern> PATTERNS;

// Scan the installed SDK and return the capabilities/requirements that are ACTUALLY present in it.
ScanResult detect(const std::optional<py::str>& sdk_root);

}  // namespace codebreaker::java::console::xdk_scan
