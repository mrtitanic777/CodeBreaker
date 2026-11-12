// codebreaker/exact/plm86.hpp -- COMPAT SHIM over cb/exact/plm86.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/exact/plm86.{hpp,cpp}: Intel's ISIS-II tools for the 8086 (PL/M-86, ASM86) as a toolchain run under the ISIS-II
// emulator. This header keeps the old class over the old toolchain port, header-only: `PLM86` is still a toolchain::Toolchain (so that
// toolchain::load() hands it out as the plugin "codebreaker.exact.plm86:PLM86"), and its compile is the new module's, the answer kept in the
// cache the old callers shared.
//
// DIVERGES: not kept, internals of the module that only its own harness called: `_compile` (it takes the headers sorted and upper-cased:
// compile() does that), `CACHE_VERSION` as a py::str (a constant in the new module); the cache keeps what it kept under another key.
#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "cb/exact/plm86.hpp"
#include "codebreaker/exact/plugin_shim.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::plm86 {

namespace nw = ::cb::exact::plm86;

inline const py::str CACHE_VERSION = py::str(std::string(nw::kCacheVersion));

class PLM86 : public toolchain::Toolchain {
  public:
    PLM86(const toolchain::Profile& profile, const std::optional<py::str>& variant_in = std::nullopt)
        : toolchain::Toolchain(profile, std::nullopt),
          impl_(std::make_shared<nw::Plm86>(plugin_shim::new_profile(profile), variant_in ? std::optional<std::string>(variant_in->utf8()) : std::nullopt,
                                            plugin_shim::context())) {
        name = profile.name;
        if (variant_in && !variant_in->empty()) variant = *variant_in;
        else if (profile.default_variant && !profile.default_variant->empty()) variant = *profile.default_variant;
        else variant = py::str("");
        version = ::cb::compat::to_py(impl_->version());
        controls = ::cb::compat::to_py(impl_->controls());
        key = ::cb::compat::to_py(impl_->plugin_key());
    }

    py::str version;
    py::str controls;
    py::str key;   // sha1(name + version + controls + CACHE_VERSION)[:12]

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override {
        return compile(src, std::optional<py::dict<py::str, py::bytes>>(headers), include, switches, true);
    }

    // compile(src, headers=None, include=(), switches=None, cache=True) -> (the object module or None, the console text)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt, bool use_cache = true) const {
        const ::cb::Bytes bytes = ::cb::compat::from_py(src);
        const auto options = plugin_shim::options_of(headers ? &*headers : nullptr, py::list<py::str>(), switches);   // (the include folders are not used)
        (void)include;
        return plugin_shim::kept(::cb::compat::to_py(impl_->cache_key(bytes, options)), use_cache,
                                 [&] { return plugin_shim::result_of(impl_->build(bytes, options)); });
    }

  private:
    std::shared_ptr<nw::Plm86> impl_;
};

// puts PLM86 where toolchain::load() finds it: the plugin "codebreaker.exact.plm86:PLM86"
inline void register_plugin() {
    toolchain::register_plugin(py::str(std::string(nw::kPluginName)),
                               [](const toolchain::Profile& p, const std::optional<py::str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                                   return std::make_shared<PLM86>(p, variant);
                               });
}

}  // namespace codebreaker::exact::plm86
