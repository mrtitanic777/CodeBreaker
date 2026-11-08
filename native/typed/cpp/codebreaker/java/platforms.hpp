// codebreaker/java/platforms.hpp -- typed C++ port of codebreaker.java.platforms.
//
// platforms.py -- target platform definitions for stub annotation.
//
// Each platform maps the converter's subsystem IDs (from stubs.py) to the
// platform-specific SDK, API, and library names that should fill that stub.
// The converter emits these as comments inside the stub body so the developer
// knows exactly what to reach for.
//
// Notes on the port:
//   * PLATFORMS is an OrderedDict of dicts with fixed keys: each platform is a `Platform` record and each
//     subsystem entry a `Subsystem` record ('sdk_dir' is absent on some platforms, so it is optional);
//   * the _STDAFX table is {platform: dict with fixed keys}: a `StdafxConfig` record, where the keys the
//     Python reads with .get() ('globals', 'extra', 'host_minmax', 'timer_impl') are an empty list / empty
//     string / false / nullopt when absent;
//   * get_platform returns the Python dict or None: std::optional<Platform>.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"

namespace codebreaker::java::platforms {

// One subsystem entry of a platform: the SDK, libraries and APIs that fill the stub.
struct Subsystem {
    py::str sdk;
    py::str libs;
    py::str apis;
};

struct Platform {
    py::str label;
    py::str description;
    std::optional<py::str> sdk_dir;
    bool big_endian = false;
    bool null_page_protected = false;
    py::str null_deref_msg;
    py::dict<py::str, Subsystem> subsystems;
};

extern const py::dict<py::str, Platform> PLATFORMS;

// Return platform info or None.
std::optional<Platform> get_platform(const py::str& platform_id);

// Return a comment string for a stub body, given a platform and subsystem.
// e.g. '// TODO: Implement with D3D9 (d3d9.lib) -- IDirect3DDevice9, ...'
py::str get_stub_comment(const py::str& platform_id, const py::str& subsystem_id);

// ---------------------------------------------------------------------------
// Platform-specific stdafx.h generation
// ---------------------------------------------------------------------------

extern const py::str _STDAFX_COMMON_CRT;
extern const py::str _STDAFX_HOST_WINDOWS;
extern const py::str _STDAFX_HOST_MINMAX;
extern const py::str _STDAFX_MSVC_WARNINGS;
extern const py::str _STDAFX_COMMON_TYPEDEFS;

// One entry of the _STDAFX table.
struct StdafxConfig {
    py::list<py::str> system_headers;
    py::list<py::str> sdk_headers;
    py::list<py::str> globals;
    py::str warnings;
    py::str extra;
    bool host_minmax = false;
    std::optional<py::str> timer_impl;
};

extern const py::dict<py::str, StdafxConfig> _STDAFX;

// Generate a platform-specific stdafx.h precompiled header.
py::str generate_stdafx(const py::str& platform_id, const py::str& cpp_std_label = py::str("C++03"));

}  // namespace codebreaker::java::platforms
