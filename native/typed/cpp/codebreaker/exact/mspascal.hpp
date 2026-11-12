// codebreaker/exact/mspascal.hpp -- COMPAT SHIM over cb/exact/mspascal.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/exact/mspascal.{hpp,cpp}: the Microsoft Pascal compiler (PAS1/PAS2) as a toolchain run under the DOS emulator, its
// procedures named in the object. This header keeps the old class over the old toolchain port, header-only: `MSPascal` is still a
// toolchain::Toolchain (so that toolchain::load() hands it out as the plugin "codebreaker.exact.mspascal:MSPascal"), and its compile is the
// new module's, the answer kept in the cache the old callers shared. The pre-release 1.10 kit's zips are extracted with the java area's reader.
//
// DIVERGES: not kept, internals of the module that only its own harness called: `_segnames`, `_compile` (compile() runs the passes), `HELPERS`
// and `INLINE` as py::bytes (functions in the new module); the passes run `fast` (no block hook, which this module never reads); a heading that
// never ends stops emit_order (the Python raised IndexError).
#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "cb/exact/mspascal.hpp"
#include "cb/java/console/zip_archive.hpp"
#include "codebreaker/exact/plugin_shim.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::mspascal {

namespace nw = ::cb::exact::mspascal;

inline const py::str CACHE_VERSION = py::str(std::string(nw::kCacheVersion));

// the procedures of a source in the order their code is written: each at the end of its body, after the procedures declared inside it -> [name]
inline py::list<py::str> emit_order(const py::str& text) {
    py::list<py::str> out;
    for (const std::string& n : nw::emit_order(text.utf8())) out.append(::cb::compat::to_py(n));
    return out;
}

// the object with a PUBDEF of the code segment naming procs {name: offset} (before its MODEND)
inline py::bytes publish(const py::bytes& obj, const py::dict<py::str, int64_t>& procs, const py::str& code_seg) {
    ::cb::OrderedMap<std::string, std::uint32_t> p;
    for (const auto& kv : procs.items()) p.insert_or_assign(std::get<0>(kv).utf8(), static_cast<std::uint32_t>(std::get<1>(kv)));
    return ::cb::compat::to_py(::cb::ByteView(nw::publish(::cb::compat::from_py(obj), p, code_seg.utf8())));
}

class MSPascal : public toolchain::Toolchain {
  public:
    MSPascal(const toolchain::Profile& profile, const std::optional<py::str>& variant_in = std::nullopt)
        : toolchain::Toolchain(profile, std::nullopt),
          impl_(std::make_shared<nw::MsPascal>(plugin_shim::new_profile(profile), variant_in ? std::optional<std::string>(variant_in->utf8()) : std::nullopt,
                                               plugin_shim::context(), hooks())) {
        name = profile.name;
        if (variant_in && !variant_in->empty()) variant = *variant_in;
        else if (profile.default_variant && !profile.default_variant->empty()) variant = *profile.default_variant;
        else variant = py::str("");
        version = ::cb::compat::to_py(impl_->version());
        helper_data = static_cast<int64_t>(impl_->helper_data());
        key = ::cb::compat::to_py(impl_->plugin_key());
    }

    py::str version;
    int64_t helper_data;
    py::str key;   // sha1(name + version + CACHE_VERSION)[:12]

    // the folder of the compiler's files (the 1.10 kit's zips are extracted to the state folder once)
    py::str kit() const { return ::cb::compat::to_py(::cb::fs::to_text(impl_->kit())); }

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override {
        (void)headers;
        (void)include;
        (void)switches;
        return compile(src, std::nullopt, py::list<py::str>(), std::nullopt, true, py::str("T.PAS"));
    }

    // compile(src, headers=None, include=(), switches=None, cache=True, name="T.PAS") -> (the object module or None, the console text)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt, bool use_cache = true,
                                                          const py::str& name_ = py::str("T.PAS")) const {
        (void)headers;
        (void)include;
        (void)switches;   // (the headers, include folders and switches mean nothing to this compiler)
        const ::cb::Bytes bytes = ::cb::compat::from_py(src);
        const std::string file = name_.utf8();
        return plugin_shim::kept(::cb::compat::to_py(impl_->cache_key(bytes, file)), use_cache, [&] {
            const auto r = impl_->build(bytes, file);
            return plugin_shim::result_of({r.object, r.console});
        });
    }

  private:
    static nw::Hooks hooks() {
        nw::Hooks h;
        h.extract_zip = [](const std::filesystem::path& zip, const std::filesystem::path& dest) {
            const auto archive = ::cb::java::console::ZipArchive::open(zip);
            for (const auto& e : archive.entries()) {
                if (e.is_directory()) continue;
                ::cb::fs::write_file(dest / ::cb::fs::from_text(e.name), archive.read(e));
            }
        };
        return h;
    }

    std::shared_ptr<nw::MsPascal> impl_;
};

// puts MSPascal where toolchain::load() finds it: the plugin "codebreaker.exact.mspascal:MSPascal"
inline void register_plugin() {
    toolchain::register_plugin(py::str(std::string(nw::kPluginName)),
                               [](const toolchain::Profile& p, const std::optional<py::str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                                   return std::make_shared<MSPascal>(p, variant);
                               });
}

}  // namespace codebreaker::exact::mspascal
