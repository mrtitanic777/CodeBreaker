// codebreaker/java/console/sdk.hpp -- SHIM over cb/java/console/sdk.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer (transpile, run, platform_config, platform_coverage).
//
// The settings key xbox360_sdk is read here (the new module takes the configured folder as a plain value).
#pragma once

#include <memory>
#include <optional>

#include "codebreaker/java/console/_shim.hpp"
#include "cb/java/console/sdk.hpp"
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

// Platforms that need NO SDK gate (portable / ubiquitous toolchain): None, 'none', 'pc_windows', 'pc', 'windows'.
inline bool is_ungated(const std::optional<py::str>& platform) {
    if (!platform) return true;
    const std::string p = platform->utf8();
    return p == "none" || p == "pc_windows" || p == "pc" || p == "windows";
}

inline std::string platform_text(const std::optional<py::str>& platform) { return platform ? platform->utf8() : std::string(); }

// A located SDK: the old record's fields, over the new SdkInfo that reads the headers.
class SdkInfo {
  public:
    std::optional<py::str> platform;
    py::str name;
    py::str root;
    py::list<py::str> include_dirs;   // list of existing include dirs to search for symbols
    py::str source;                   // 'env:XEDK' / 'path' / 'override' -- how we found it

    explicit SdkInfo(cb::java::console::sdk::SdkInfo found)
        : platform(cb::compat::to_py(found.platform())),
          name(cb::compat::to_py(found.name())),
          root(shim::native(found.root())),
          source(cb::compat::to_py(found.source_text())),
          impl_(std::make_shared<cb::java::console::sdk::SdkInfo>(std::move(found))) {
        for (const auto& d : impl_->include_dirs()) include_dirs.append(shim::native(d));
    }

    py::str repr() const { return py::str("SdkInfo(") + (platform ? *platform : py::str("None")) + py::str(" @ ") + root + py::str(" via ") + source + py::str(")"); }

    // True if `symbol` is declared anywhere in the located SDK's headers.
    bool has_symbol(const py::str& symbol) { return impl_->has_symbol(symbol.utf8()); }

  private:
    std::shared_ptr<cb::java::console::sdk::SdkInfo> impl_;
};

inline bool requires_sdk(const std::optional<py::str>& platform) {
    return cb::java::console::sdk::requires_sdk(platform_text(platform));
}

namespace detail_ {
inline cb::java::console::sdk::SdkHints hints_for(const std::optional<py::str>& platform, const std::optional<py::str>& override_) {
    cb::java::console::sdk::SdkHints hints;
    hints.override_root = shim::given(override_);
    if (const auto console = cb::java::console::sdk::console_of(platform_text(platform))) {
        const std::string& key = cb::java::console::sdk::spec_of(*console).setting_key;
        if (!key.empty()) {
            const py::str configured = py::settings::get_str(py::str(key));
            if (configured.size() > 0) hints.configured_root = shim::path_of(configured);
        }
    }
    return hints;
}
}  // namespace detail_

inline std::optional<SdkInfo> detect(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt) {
    auto found = cb::java::console::sdk::detect(platform_text(platform), detail_::hints_for(platform, override_));
    if (!found) return std::nullopt;
    return SdkInfo(std::move(*found));
}

inline SdkInfo require(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt) {
    try {
        return SdkInfo(cb::java::console::sdk::require(platform_text(platform), detail_::hints_for(platform, override_)));
    } catch (const cb::java::console::sdk::SdkNotFoundError& e) {
        throw SdkNotFound(std::string(e.what()));
    }
}

inline py::str describe(const std::optional<py::str>& platform, const std::optional<py::str>& override_ = std::nullopt) {
    return cb::compat::to_py(cb::java::console::sdk::describe(platform_text(platform), detail_::hints_for(platform, override_)));
}

}  // namespace codebreaker::java::console::sdk
