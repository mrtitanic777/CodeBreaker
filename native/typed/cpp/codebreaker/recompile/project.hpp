// codebreaker/recompile/project.hpp -- SHIM over cb/recompile/project.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// A recompiled Xbox 360 title as a PC project that builds (the rexglue layout), and the `recompile-project` command. The
// module is the new one's; this shim reads the image, the .xex header and the import names through the old ports (the new
// module takes them as an Environment) and hands the old title locator and projects folder to the command.
//
//     python -m codebreaker recompile-project <image or .xex> -o <folder>
//     auto agg = project::scaffold(path, folder);
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/project.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/formats/xex_imports.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/powerpc.hpp"
#include "codebreaker/recompile/title.hpp"
#include "codebreaker/recompile/x360project.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::project {

namespace shim_ {
inline py::str default_root() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}
inline py::str& root_() {
    static py::str root = default_root();
    return root;
}
}  // namespace shim_

// The checkout root (a plain variable a test may point elsewhere) and the rexglue folder below it.
inline py::str ROOT() { return shim_::root_(); }
inline py::str _REXGLUE() { return py::os::path::join(shim_::root_(), py::str("native"), py::str("rexglue")); }
inline void set_root(const py::str& root) { shim_::root_() = root; }

inline py::str _clean_name(const py::str& module) { return cb::compat::to_py(cb::recompile::project::clean_name(cb::compat::from_py(module))); }

// scaffold's result: powerpc.recompile_image's agg, with the project's name, folder, translation units and imports
struct Scaffolded {
    powerpc::Agg agg;
    py::str name;
    py::str dir;
    int64_t shards = 0;
    int64_t imports = 0;
};

namespace shim_ {

// The environment of the old ports: the image through discover/disassemble/xex, the import names of xex_imports.
inline cb::recompile::project::Environment environment() {
    cb::recompile::project::Environment env;
    env.read_title = [](const std::filesystem::path& file) {
        return cb::compat::guard([&] {
            namespace xex = formats::xex;
            const py::str path = cb::compat::to_py(cb::fs::to_text(file));
            cb::recompile::project::TitleImage t;
            t.loaded = cb::recompile::powerpc::LoadedImage{discover::shim_::image_of(*discover::_image(path)), disassemble::shim_::facts_of(path)};
            t.is_xex = xex::is_xex(path);
            if (t.is_xex) {
                const xex::Header h = xex::header(path);
                if (h.name) t.image_name = cb::compat::from_py(*h.name);
                for (const py::str& l : h.imports) t.libraries.push_back(cb::compat::from_py(l));
            }
            const py::ref<formats::powerpc_image::Image> image = formats::powerpc_image::read(t.is_xex ? xex::basefile(path) : path);
            for (const auto& kv : image->imports.items()) {
                const py::list<py::str> bits = py::split(std::get<1>(kv).slice_from(6), py::str("_ord_"));   // "import<lib>_ord_<ordinal>"
                if (bits.size() != 2) throw py::ValueError("not an import symbol: " + cb::compat::from_py(std::get<1>(kv)));
                t.imports.push_back({static_cast<std::uint32_t>(std::get<0>(kv)), static_cast<std::size_t>(py::int_parse(bits[0], 10)),
                                     static_cast<std::uint32_t>(py::int_parse(bits[1], 10))});
            }
            return t;
        });
    };
    env.vector = disassemble::shim_::vector_decoder();
    env.import_names = [](std::string_view library, std::uint32_t ordinal) -> std::string {
        static const py::dict<py::str, py::dict<int64_t, py::str>> names = formats::xex_imports::_names();
        const auto table = names.get(cb::compat::to_py(library));
        if (!table || !table->contains(static_cast<int64_t>(ordinal))) return {};
        return cb::compat::from_py(table->at(static_cast<int64_t>(ordinal)));
    };
    env.rexglue = cb::fs::from_text(cb::compat::from_py(_REXGLUE()));
    return env;
}

}  // namespace shim_

// Write the project (see the new module's notes).
inline Scaffolded scaffold(const py::str& xex_path, const py::str& out_dir, const std::optional<int64_t>& jobs = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<unsigned> threads;
        if (jobs) threads = static_cast<unsigned>(std::max<int64_t>(*jobs, 1));
        const cb::recompile::project::Scaffolded made = cb::recompile::project::scaffold(
            shim_::environment(), cb::fs::from_text(cb::compat::from_py(xex_path)), cb::fs::from_text(cb::compat::from_py(out_dir)), threads);
        Scaffolded out;
        const auto& g = made.agg;
        out.agg.funcs = static_cast<int64_t>(g.funcs);
        out.agg.total = static_cast<int64_t>(g.total);
        out.agg.translated = static_cast<int64_t>(g.translated);
        out.agg.unimpl = static_cast<int64_t>(g.unimplemented);
        out.agg.discovered = static_cast<int64_t>(g.discovered);
        out.agg.tail_entries = static_cast<int64_t>(g.tail_entries);
        out.agg.lo = g.lo;
        out.agg.hi = g.hi;
        for (const std::string& p : g.parts) out.agg.parts.append(cb::compat::to_py(p));
        for (const std::uint32_t a : g.decls) out.agg.decls.append(a);
        for (const std::uint32_t a : g.chosen) out.agg.chosen.append(a);
        for (const std::uint32_t a : g.skipped) out.agg.skipped.append(a);
        out.name = cb::compat::to_py(made.name);
        out.dir = out_dir;
        out.shards = static_cast<int64_t>(made.shards);
        out.imports = static_cast<int64_t>(made.imports);
        return out;
    });
}

// The command line without the program name (sys.argv[1:]); prints to stdout. -> the exit code
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        cb::recompile::project::CommandHooks hooks{
            .env = shim_::environment(),
            .recompiler = x360project::shim_::recompiler(),
            .locate_title = [](const std::filesystem::path& path) -> std::optional<cb::recompile::project::TitleFiles> {
                return cb::compat::guard([&]() -> std::optional<cb::recompile::project::TitleFiles> {
                    auto found = title::locate(cb::compat::to_py(cb::fs::to_text(path)));
                    if (!found) return std::nullopt;
                    cb::recompile::project::TitleFiles t;
                    t.name = found->name ? cb::compat::from_py(*found->name) : std::string();
                    t.source = cb::compat::from_py(found->source);
                    t.xex = cb::compat::from_py(found->read_xex());
                    auto shared = std::make_shared<title::Title>(std::move(*found));
                    t.unpack = [shared](const std::filesystem::path& folder) {
                        return cb::compat::guard([&] { return static_cast<std::size_t>(shared->unpack(cb::compat::to_py(cb::fs::to_text(folder)))); });
                    };
                    return t;
                });
            },
            .store = cb::base::converted::Store(cb::fs::from_text(cb::compat::from_py(base::converted::root())))};
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return static_cast<int64_t>(cb::recompile::project::run_command(hooks, args, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::recompile::project
