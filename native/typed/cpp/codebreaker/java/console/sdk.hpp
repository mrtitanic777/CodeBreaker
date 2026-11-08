// codebreaker/java/console/sdk.hpp -- C++17 port of codebreaker.java.console.sdk.
//
// sdk.py -- platform SDK detection & gatekeeping.
//
// A console platform's SDK (Xbox 360 XDK, PS3 SDK, Wii RVL SDK) is proprietary. The converter must NOT
// ship or redistribute those headers/signatures, and must NOT emit platform-specific code for a target
// the user has no SDK for. So this module:
//
//   * DETECTS the SDK on the user's own machine (env var first, then the standard install paths), or
//     accepts an explicit `--sdk <path>` override.
//   * GATEKEEPS: converting/emitting for a console platform is refused unless its SDK is located. PC is
//     ungated; `--platform none` (portable only) is always allowed.
//   * VERIFIES symbols against the located SDK headers, so the code we emit calls only APIs that actually
//     exist in THAT installed SDK -- we pull the truth from the user's SDK instead of trusting a baked-in
//     signature list. (The real declarations come from the user's own `<xtl.h>` at compile time; the tool
//     ships none of them.)
//
// `require(platform)` is the one call the pipeline/config use to gate. It returns an SdkInfo or raises
// SdkNotFound with actionable instructions.
//
// Typing notes: a platform is `str or None` (std::optional<py::str>); PLATFORM_SDKS' dict-with-fixed-keys
// values are the SdkSpec record. settings.get(key) is py::settings::get_str (a str setting).
#pragma once

#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::console::sdk {

struct SdkNotFound : py::Exception {
    SdkNotFound() : py::Exception() {}
    explicit SdkNotFound(const std::string& m) : py::Exception(m) {}
    explicit SdkNotFound(const py::str& m) : py::Exception(m) {}
    const char* type() const override { return "SdkNotFound"; }
};

// One PLATFORM_SDKS entry: the environment variable(s) that point at the SDK root, the standard install
// paths to probe, the include subdir, and header files that must exist for the root to count as a real SDK.
struct SdkSpec {
    py::str name;
    py::list<py::str> env;
    py::list<py::str> default_paths;
    py::list<py::str> include_subdirs;
    py::list<py::str> probe_headers;
    py::str get_env;
    std::optional<py::str> setting;   // the settings.json key (spec.get('setting')): only xbox360 has one
};

extern const py::dict<py::str, SdkSpec> PLATFORM_SDKS;

// Platforms that need NO SDK gate (portable / ubiquitous toolchain): None, 'none', 'pc_windows', 'pc', 'windows'.
bool is_ungated(const std::optional<py::str>& platform);

class SdkInfo {
  public:
    std::optional<py::str> platform;
    py::str name;
    py::str root;
    py::list<py::str> include_dirs;   // list of existing include dirs to search for symbols
    py::str source;                   // 'env:XEDK' / 'path' / 'override' -- how we found it

    SdkInfo(const std::optional<py::str>& platform, const py::str& name, const py::str& root,
            const py::list<py::str>& include_dirs, const py::str& source);

    py::str repr() const;   // __repr__
    // True if `symbol` is declared anywhere in the located SDK's headers. Lets the emitter verify
    // an API exists in THIS install before emitting a call to it (pull-from-SDK, not baked-in).
    bool has_symbol(const py::str& symbol);

  private:
    const py::str& _all_header_text();
    std::optional<py::str> _symbol_cache;
};

bool requires_sdk(const std::optional<py::str>& platform);

// Does `root` look like a real SDK (has an include dir with a probe header)?  Returns the list of
// existing include dirs if valid, else None.
std::optional<py::list<py::str>> _valid_root(const SdkSpec& spec, const std::optional<py::str>& root);

// Locate the SDK for `platform`. Order: explicit override -> the platform's settings.json key
// (xbox360_sdk) -> env var(s) -> default install paths. Returns SdkInfo or None. Ungated platforms
// return a trivial SdkInfo. (`override_`: the Python's `override`.)
std::optional<SdkInfo> detect(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt);

// Return the SdkInfo for `platform`, or raise SdkNotFound with actionable guidance. This is the
// gate: no console SDK -> no platform-specific conversion/emission.
SdkInfo require(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt);

// Human-readable one-liner about SDK status for `platform` (for CLI `codebreaker java sdk detect`).
py::str describe(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt);

}  // namespace codebreaker::java::console::sdk
