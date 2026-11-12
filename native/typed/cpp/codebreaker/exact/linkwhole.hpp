// codebreaker/exact/linkwhole.hpp -- compat shim for cb/exact/linkwhole.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (exact/link, exact/project).
//
//     python -m codebreaker exact PROJECT link [--keep DIR] [-v]       (a project whose "link" names a "linker")
//
//     link_config(json text) -> LinkConfig       wanted(prj)      listed(prj, lk) -> [(k, name, object, odd)]
//     modules(prj) -> [(place, file name, object)] in link order      run(prj, objs, keep) -> (file or None, words, map)
//     compare(prj, exe) -> (identical, [lines])       main(prj, argv, out) -> exit code
//
// `Project` is the old layer's own interface (what linkwhole.py reads of exact.project.Project); the real one
// implements it. A LinkConfig holds the new layer's typed entry. What main prints goes to stdout (cb::out), not to
// `out`, as the other refactored commands' shims do. The functions the Python kept for its own use (_found, _own, _name,
// _index, _file) are not shimmed: nothing outside the module called them.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/exact/linkwhole.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/linkemu.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/standin.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::linkwhole {

namespace nw = cb::exact::linkwhole;

struct LinkConfig {   // prj.cfg["link"]
    nw::LinkConfig value;
};

// the dict prj.cfg["link"] (JSON text) as a LinkConfig
inline LinkConfig link_config(const py::str& json_text) {
    return cb::compat::guard([&] { return LinkConfig{nw::parse_link_config(cb::compat::from_py(json_text))}; });
}

class Project {
  public:
    virtual ~Project() = default;
    virtual py::str dir() const = 0;                                                  // prj.dir
    virtual py::str src() const = 0;                                                  // prj.src
    virtual py::list<std::tuple<int64_t, py::str>> sources() const = 0;               // prj.sources() -> [(address, path)]
    virtual std::shared_ptr<modules::Toolchain> toolchain() const = 0;                // prj.toolchain()
    virtual py::dict<py::str, py::bytes> headers() const = 0;                         // prj.headers()
    virtual py::list<py::str> include() const = 0;                                    // prj.include
    virtual std::optional<py::str> switches() const = 0;                              // prj.switches
    virtual py::list<std::shared_ptr<const Project>> sets() const = 0;                // prj.sets(): [prj] + the other sets
    virtual LinkConfig link() const = 0;                                              // prj.cfg.get("link") or {}
    virtual py::bytes image() const = 0;                                              // prj.image
    virtual py::bytes file() const = 0;                                               // prj.file
    virtual py::str exe() const = 0;                                                  // prj.exe
    virtual linkemu::Symbols symbols() const = 0;                                     // prj.symbols()
};

namespace detail {

namespace nm = cb::exact::modules;

inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }

using OldProject = codebreaker::exact::linkwhole::Project;   // (inside NewView, `Project` is the new layer's base class)

// The old project as the new layer's interface.
class NewView final : public nw::Project {
  public:
    explicit NewView(std::shared_ptr<const OldProject> old) : old_(std::move(old)), config_(old_->link().value) {}
    explicit NewView(const OldProject& old) : NewView(std::shared_ptr<const OldProject>(std::shared_ptr<void>(), &old)) {}

    std::filesystem::path dir() const override { return path_of(old_->dir()); }
    std::filesystem::path src() const override { return path_of(old_->src()); }
    std::vector<std::pair<std::uint32_t, std::filesystem::path>> sources() const override {
        std::vector<std::pair<std::uint32_t, std::filesystem::path>> out;
        for (const auto& [at, path] : old_->sources()) out.emplace_back(cb::compat::int_from_py<std::uint32_t>(at), path_of(path));
        return out;
    }
    std::shared_ptr<const nm::Toolchain> toolchain() const override { return std::make_shared<modules::detail::ToolchainAdapter>(old_->toolchain()); }
    std::map<std::string, cb::Bytes> headers() const override {
        std::map<std::string, cb::Bytes> out;
        for (const auto& kv : old_->headers().items()) out[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
        return out;
    }
    std::vector<std::string> include() const override {
        std::vector<std::string> out;
        for (const py::str& i : old_->include()) out.push_back(cb::compat::from_py(i));
        return out;
    }
    std::optional<std::string> switches() const override {
        const auto s = old_->switches();
        return s ? std::optional<std::string>(cb::compat::from_py(*s)) : std::nullopt;
    }
    std::vector<std::shared_ptr<const nw::Project>> sets() const override {
        std::vector<std::shared_ptr<const nw::Project>> out;
        for (const auto& s : old_->sets()) out.push_back(std::make_shared<NewView>(s));
        return out;
    }
    const nw::LinkConfig& link() const override { return config_; }
    cb::ByteView image() const override {
        if (!image_) image_ = cb::compat::from_py(old_->image());
        return *image_;
    }
    cb::ByteView file() const override {
        if (!file_) file_ = cb::compat::from_py(old_->file());
        return *file_;
    }
    std::string exe() const override { return cb::compat::from_py(old_->exe()); }
    cb::exact::SymbolTable symbols() const override { return standin::detail::symbols_to_new(old_->symbols()); }
    std::filesystem::path resolve(std::string_view spec) const override {
        return path_of(toolchain::kit_path(std::nullopt, cb::compat::to_py(spec), old_->dir()));
    }
    std::shared_ptr<const nm::Toolchain> load_toolchain(std::string_view name, const std::optional<std::string>& variant) const override {
        std::optional<py::str> v;
        if (variant) v = cb::compat::to_py(*variant);
        return std::make_shared<modules::detail::ToolchainAdapter>(toolchain::load(cb::compat::to_py(name), v));
    }

  private:
    std::shared_ptr<const OldProject> old_;
    nw::LinkConfig config_;
    mutable std::optional<cb::Bytes> image_, file_;
};

// The old DOS emulator running the linker.
class OldDos final : public nw::DosHost {
  public:
    std::string run(const std::filesystem::path& exe, const std::filesystem::path& workdir, std::string_view command_line,
                    std::optional<std::uint32_t> psp) const override {
        std::optional<int64_t> segment;
        if (psp) segment = *psp;
        const auto r = emu::dos::run(cb::compat::to_py(cb::fs::to_text(exe)), cb::compat::to_py(cb::fs::to_text(workdir)), cb::compat::to_py(command_line),
                                     py::bytes(std::string()), emu::dos::DEFAULT_MAX_BLOCKS, false, py::bytes(std::string("PATH=\0", 6)), segment);
        return r.console.raw();
    }
};

}  // namespace detail

// the project says it is linked by a period linker
inline bool wanted(const Project& prj) { return cb::compat::guard([&] { return nw::wanted(prj.link().value); }); }

// the modules a file lists in the order they were linked -> [(k, name, object bytes, odd)]
inline py::list<std::tuple<int64_t, py::str, py::bytes, bool>> listed(const Project& prj, const LinkConfig& lk) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, py::str, py::bytes, bool>> out;
        for (const nw::ListedModule& m : nw::listed(detail::NewView(prj), lk.value))
            out.append(std::make_tuple(int64_t(m.index), cb::compat::to_py(m.name), cb::compat::to_py(cb::ByteView(m.object)), m.odd));
        return out;
    });
}

// [(place, file name, object bytes)] in link order
inline py::list<std::tuple<int64_t, py::str, py::bytes>> modules(const Project& prj) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, py::str, py::bytes>> out;
        for (const nw::PlacedObject& o : nw::placed_objects(detail::NewView(prj)))
            out.append(std::make_tuple(int64_t(o.place), cb::compat::to_py(o.name), cb::compat::to_py(cb::ByteView(o.object))));
        return out;
    });
}

// the object with its segments of those names in the class cls
inline py::bytes reclass(const py::bytes& obj, const py::str& cls, const py::list<py::str>& segments) {
    return cb::compat::guard([&] {
        std::vector<std::string> names;
        for (const py::str& s : segments) names.push_back(cb::compat::from_py(s));
        const cb::Bytes raw = cb::compat::from_py(obj);
        return cb::compat::to_py(cb::ByteView(nw::reclass(raw, cb::compat::from_py(cls), names)));
    });
}

// -> (the file written or None, the linker's words, its map)
inline std::tuple<std::optional<py::bytes>, py::str, py::str> run(const Project& prj, const py::list<std::tuple<int64_t, py::str, py::bytes>>& objs,
                                                                  const std::optional<py::str>& keep = std::nullopt) {
    return cb::compat::guard([&] {
        std::vector<nw::PlacedObject> placed;
        for (const auto& [at, name, raw] : objs) placed.push_back({cb::compat::int_from_py<std::uint32_t>(at), cb::compat::from_py(name), cb::compat::from_py(raw)});
        std::optional<std::filesystem::path> folder;
        if (keep) folder = detail::path_of(*keep);
        const nw::LinkOutput r = nw::link(detail::NewView(prj), placed, detail::OldDos(), folder);
        std::optional<py::bytes> exe;
        if (r.exe) exe = cb::compat::to_py(cb::ByteView(*r.exe));
        return std::make_tuple(exe, cb::compat::to_py(r.console), cb::compat::to_py(r.map));
    });
}

// -> (identical, [lines]): the file against the original, but for what the project accepts
inline std::tuple<bool, py::list<py::str>> compare(const Project& prj, const py::bytes& exe) {
    return cb::compat::guard([&] {
        const cb::Bytes original = cb::compat::from_py(prj.file());
        const nw::Comparison c = nw::compare(prj.link().value, original, cb::compat::from_py(exe));
        py::list<py::str> lines;
        for (const std::string& l : c.notes) lines.append(cb::compat::to_py(l));
        return std::make_tuple(c.identical, lines);
    });
}

// `python -m codebreaker exact PROJECT link ...`: the exit code (what it prints goes to stdout)
inline int64_t main(const Project& prj, const py::list<py::str>& argv, std::ostream&) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return int64_t(nw::run(detail::NewView(prj), args, detail::OldDos()));
    });
}

}  // namespace codebreaker::exact::linkwhole
