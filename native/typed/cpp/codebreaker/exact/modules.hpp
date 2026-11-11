// codebreaker/exact/modules.hpp -- compat shim for cb/exact/modules.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding (and the one cache the Python had, of the functions' return types per source folder). Deleted with
// its last old-layer consumer (exact/data, exact/project, exact/library, exact/ledger, exact/link ...).
//
//     modules::rows(project) -> [(name, code start, code end, data start, switches)]
//     modules::source(project, row) -> (text, [function addresses], flags) or None
//     modules::check(project, row, decode) -> CheckResult(res=[(name, address, length, verdict)], dlen, dbad, raw)
//
// `Project` and `Toolchain` are the old layer's own interfaces (the members of exact.project.Project and
// exact.toolchain.Toolchain that modules.py reads); the real ones implement them. Source text crosses as Python text
// whose characters are the file's bytes (Latin-1), as the Python read them.
#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/exact/modules.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/standin.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::modules {

using Symbols = compare::Symbols;

// exact.toolchain.Toolchain, as modules.py uses it
class Toolchain {
  public:
    virtual ~Toolchain() = default;
    // tc.p.get(key): the profile's entry (a string), or nullopt when it has none
    virtual std::optional<py::str> p_get(const py::str& key) const = 0;
    // tc.compile(src, headers=, include=, switches=) -> (the object module or None, the console text)
    virtual std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                                  const py::list<py::str>& include,
                                                                  const std::optional<py::str>& switches) const = 0;
};

// exact.project.Project, as modules.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::str src() const = 0;                                    // prj.src
    virtual py::str path(const py::str& name) const = 0;                // prj.path(name)
    virtual int64_t code_end() const = 0;                               // prj.code_end (0 when none)
    virtual int64_t own_end() const = 0;                                // prj.own_end()
    virtual std::optional<py::str> switches() const = 0;                // prj.switches
    virtual std::optional<py::str> cfg_str(const py::str& key) const = 0;      // prj.cfg.get(key) (a string entry)
    virtual py::list<int64_t> code_segments() const = 0;                // prj.code_segments (empty when it has none)
    virtual std::shared_ptr<Toolchain> toolchain() const = 0;           // prj.toolchain()
    virtual py::dict<py::str, py::bytes> headers() const = 0;           // prj.headers()
    virtual py::list<py::str> include() const = 0;                      // prj.include
    virtual py::list<std::shared_ptr<const Project>> also_sets() const = 0;    // prj.sets()[1:]
    // used by check()
    virtual py::bytes image() const = 0;                                // prj.image
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual int64_t dgroup() const = 0;                                 // prj.dgroup
    virtual int64_t loadseg() const = 0;                                // prj.loadseg
    virtual int64_t code_base(int64_t a) const = 0;                     // prj.code_base(a)
    virtual const py::set<int64_t>& relocs() const = 0;                 // prj.relocs()
};

// (name, code start, code end, data start, switches)
using Row = std::tuple<py::str, std::optional<int64_t>, std::optional<int64_t>, std::optional<int64_t>, std::optional<py::str>>;

namespace detail {

namespace nw = cb::exact::modules;

inline py::str latin1_text(const std::string& bytes) { return py::bytes(bytes).decode("latin1"); }
inline std::string latin1_bytes(const py::str& s) { return s.encode("latin1").raw(); }
inline std::string utf8(const py::str& s) { return cb::compat::from_py(s); }
inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(utf8(s)); }
inline py::str text_of(const std::filesystem::path& p) { return cb::compat::to_py(cb::fs::to_text(p)); }

class ToolchainAdapter final : public nw::Toolchain {
  public:
    explicit ToolchainAdapter(std::shared_ptr<codebreaker::exact::modules::Toolchain> t) : old(std::move(t)) {}
    std::shared_ptr<codebreaker::exact::modules::Toolchain> old;
    std::optional<std::string> profile(std::string_view key) const override {
        const auto v = old->p_get(cb::compat::to_py(key));
        if (!v) return std::nullopt;
        return cb::compat::from_py(*v);
    }
    nw::CompileResult compile(cb::ByteView source, const nw::CompileOptions& options) const override {
        py::dict<py::str, py::bytes> headers;
        for (const auto& [name, bytes] : options.headers) headers.set(cb::compat::to_py(name), cb::compat::to_py(cb::ByteView(bytes)));
        py::list<py::str> include;
        for (const std::string& i : options.include) include.append(cb::compat::to_py(i));
        std::optional<py::str> switches;
        if (options.switches) switches = cb::compat::to_py(*options.switches);
        auto [object, console] = old->compile(cb::compat::to_py(source), headers, include, switches);
        nw::CompileResult r;
        if (object) r.object = cb::compat::from_py(*object);
        r.console = cb::compat::from_py(console);
        return r;
    }
};

class ProjectAdapter final : public nw::Project {
  public:
    explicit ProjectAdapter(std::shared_ptr<const codebreaker::exact::modules::Project> p) : old_(std::move(p)) {}
    // (a project that is not owned: the adapter is for the length of one call)
    explicit ProjectAdapter(const codebreaker::exact::modules::Project& p)
        : old_(std::shared_ptr<const codebreaker::exact::modules::Project>(std::shared_ptr<void>(), &p)) {}
    std::shared_ptr<const codebreaker::exact::modules::Project> old() const { return old_; }

    std::filesystem::path src() const override { return path_of(old_->src()); }
    std::filesystem::path path(std::string_view name) const override { return path_of(old_->path(cb::compat::to_py(name))); }
    std::uint32_t code_end() const override { return cb::compat::int_from_py<std::uint32_t>(old_->code_end()); }
    std::uint32_t own_end() const override { return cb::compat::int_from_py<std::uint32_t>(old_->own_end()); }
    std::optional<std::string> switches() const override {
        const auto s = old_->switches();
        return s ? std::optional<std::string>(utf8(*s)) : std::nullopt;
    }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto s = old_->cfg_str(cb::compat::to_py(key));
        return s ? std::optional<std::string>(utf8(*s)) : std::nullopt;
    }
    std::vector<std::uint32_t> code_segments() const override {
        std::vector<std::uint32_t> out;
        for (int64_t s : old_->code_segments()) out.push_back(cb::compat::int_from_py<std::uint32_t>(s));
        return out;
    }
    std::shared_ptr<const nw::Toolchain> toolchain() const override { return std::make_shared<ToolchainAdapter>(old_->toolchain()); }
    std::map<std::string, cb::Bytes> headers() const override {
        std::map<std::string, cb::Bytes> out;
        for (const auto& kv : old_->headers().items()) out[utf8(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
        return out;
    }
    std::vector<std::string> include() const override {
        std::vector<std::string> out;
        for (const py::str& i : old_->include()) out.push_back(utf8(i));
        return out;
    }
    std::vector<std::shared_ptr<const nw::Project>> also_sets() const override {
        std::vector<std::shared_ptr<const nw::Project>> out;
        for (const auto& s : old_->also_sets()) out.push_back(std::make_shared<ProjectAdapter>(s));
        return out;
    }
    cb::ByteView image() const override {
        if (!image_) image_ = cb::compat::from_py(old_->image());
        return *image_;
    }
    cb::exact::SymbolTable symbols() const override { return codebreaker::exact::standin::detail::symbols_to_new(old_->symbols()); }
    cb::exact::compare::Program program() const override {
        using cb::compat::int_from_py;
        if (!relocs_) {
            relocs_ = std::unordered_set<std::uint32_t>();
            for (int64_t r : old_->relocs()) relocs_->insert(int_from_py<std::uint32_t>(r));
        }
        cb::exact::compare::Program p;
        p.dgroup = old_->dgroup();
        p.loadseg = int_from_py<std::uint32_t>(old_->loadseg());
        p.code_base = [o = old_](std::uint32_t a) { return int_from_py<std::uint32_t>(o->code_base(a)); };
        p.relocs = &*relocs_;
        return p;
    }

  private:
    std::shared_ptr<const codebreaker::exact::modules::Project> old_;
    mutable std::optional<cb::Bytes> image_;
    mutable std::optional<std::unordered_set<std::uint32_t>> relocs_;
};

inline nw::Row row_to_new(const Row& r) {
    using cb::compat::int_from_py;
    nw::Row n;
    n.name = utf8(std::get<0>(r));
    if (std::get<1>(r)) n.code_start = int_from_py<std::uint32_t>(*std::get<1>(r));
    if (std::get<2>(r)) n.code_end = int_from_py<std::uint32_t>(*std::get<2>(r));
    if (std::get<3>(r)) n.data_start = int_from_py<std::uint32_t>(*std::get<3>(r));
    if (std::get<4>(r)) n.switches = utf8(*std::get<4>(r));
    return n;
}

inline Row row_to_old(const nw::Row& n) {
    std::optional<int64_t> a, b, c;
    if (n.code_start) a = *n.code_start;
    if (n.code_end) b = *n.code_end;
    if (n.data_start) c = *n.data_start;
    std::optional<py::str> sw;
    if (n.switches) sw = cb::compat::to_py(*n.switches);
    return Row(cb::compat::to_py(n.name), a, b, c, sw);
}

// The Python kept the return types per source folder for the life of the process.
inline const nw::FunctionTypes& types_for(const std::filesystem::path& src) {
    static std::mutex mutex;
    static std::map<std::string, std::shared_ptr<nw::FunctionTypes>> cache;
    const std::lock_guard<std::mutex> g(mutex);
    auto& slot = cache[cb::fs::to_text(src)];
    if (!slot) slot = std::make_shared<nw::FunctionTypes>(nw::FunctionTypes::read(src));
    return *slot;
}

}  // namespace detail

inline py::list<Row> rows(const Project& prj) {
    return cb::compat::guard([&] {
        py::list<Row> out;
        for (const auto& r : cb::exact::modules::rows(detail::ProjectAdapter(prj))) out.append(detail::row_to_old(r));
        return out;
    });
}

inline std::optional<int64_t> lib_start(const Project& prj) {
    return cb::compat::guard([&]() -> std::optional<int64_t> {
        const auto s = cb::exact::modules::lib_start(detail::ProjectAdapter(prj));
        if (!s) return std::nullopt;
        return static_cast<int64_t>(*s);
    });
}

inline py::list<std::tuple<int64_t, int64_t, py::str>> parts(const Project& prj, const py::str& name) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t, py::str>> out;
        for (const auto& p : cb::exact::modules::parts(detail::ProjectAdapter(prj), detail::utf8(name)))
            out.append(std::make_tuple(int64_t(p.start), int64_t(p.end), cb::compat::to_py(p.segment)));
        return out;
    });
}

inline py::list<int64_t> starts(const Project& prj) {
    return cb::compat::guard([&] {
        py::list<int64_t> out;
        for (const auto s : cb::exact::modules::starts(detail::ProjectAdapter(prj))) out.append(int64_t(s));
        return out;
    });
}

inline py::list<py::str> files_of(const py::str& src, int64_t start, int64_t end) {
    return cb::compat::guard([&] {
        py::list<py::str> out;
        const auto lo = cb::compat::int_from_py<std::uint32_t>(std::max<int64_t>(start, 0));
        const auto hi = cb::compat::int_from_py<std::uint32_t>(std::clamp<int64_t>(end, 0, 0xFFFFFFFFll));
        for (const auto& p : cb::exact::modules::files_of(detail::path_of(src), lo, hi)) out.append(detail::text_of(p));
        return out;
    });
}

// {function: the type it returns} - the program's functions that return something else than an int
inline py::dict<py::str, py::str> deftypes(const py::str& srcdir) {
    return cb::compat::guard([&] {
        py::dict<py::str, py::str> out;
        for (const auto& [fn, ty] : detail::types_for(detail::path_of(srcdir)).all()) out.set(cb::compat::to_py(fn), cb::compat::to_py(ty));
        return out;
    });
}

// the files' texts as one source; the switches the functions' files ask for. before: {path: text put
// before its body} (a pragma).  header_tag: the profile's "file_switches" (None/empty: no header comment)
inline std::tuple<py::str, py::set<py::str>> combine(const py::list<py::str>& paths, const py::str& default_,
                                                     const std::optional<py::str>& header_tag,
                                                     const py::dict<py::str, py::str>& before = py::dict<py::str, py::str>()) {
    return cb::compat::guard([&] {
        std::vector<std::filesystem::path> files;
        for (const py::str& p : paths) files.push_back(detail::path_of(p));
        std::map<std::filesystem::path, std::string> pragmas;
        for (const auto& kv : before.items()) pragmas[detail::path_of(std::get<0>(kv))] = detail::latin1_bytes(std::get<1>(kv));
        const std::filesystem::path folder = files.empty() ? std::filesystem::path(".") : files.front().parent_path();
        const std::string tag = header_tag ? detail::utf8(*header_tag) : std::string();
        const auto c = cb::exact::modules::combine(files, detail::utf8(default_), tag.empty() ? std::nullopt : std::optional<std::string_view>(tag), pragmas,
                                                   detail::types_for(folder));
        py::set<py::str> flags;
        for (const std::string& s : c.switches) flags.add(cb::compat::to_py(s));
        return std::make_tuple(detail::latin1_text(c.text), flags);
    });
}

// (the module's whole text, the addresses of its functions in order, the switches) or None (an object of the kit)
inline std::optional<std::tuple<py::str, py::list<int64_t>, py::set<py::str>>> source(const Project& prj, const Row& row) {
    return cb::compat::guard([&]() -> std::optional<std::tuple<py::str, py::list<int64_t>, py::set<py::str>>> {
        const detail::ProjectAdapter project(prj);
        const auto s = cb::exact::modules::source(project, detail::row_to_new(row), detail::types_for(project.src()));
        if (!s) return std::nullopt;
        py::list<int64_t> addrs;
        for (const auto a : s->addresses) addrs.append(int64_t(a));
        py::set<py::str> flags;
        for (const std::string& f : s->switches) flags.add(cb::compat::to_py(f));
        return std::make_tuple(detail::latin1_text(s->text), addrs, flags);
    });
}

// (the source set, its toolchain) whose files' pattern names this file - an "also" set's, else the project's.
// The set is null when it is the project itself.
inline std::tuple<std::shared_ptr<const Project>, std::shared_ptr<Toolchain>> set_of(const Project& prj, const py::str& mod) {
    return cb::compat::guard([&] {
        const auto found = cb::exact::modules::set_of(detail::ProjectAdapter(prj), detail::utf8(mod));
        std::shared_ptr<const Project> own;
        if (found.own) own = static_cast<const detail::ProjectAdapter&>(*found.own).old();
        return std::make_tuple(own, static_cast<const detail::ToolchainAdapter&>(*found.toolchain).old);
    });
}

// the name the project gives the first code segment (project "first_segment"), for a module that lies in it
inline std::optional<py::str> first_segment(const Project& prj, std::optional<int64_t> start) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        std::optional<std::uint32_t> s;
        if (start) s = cb::compat::int_from_py<std::uint32_t>(*start);
        const auto name = cb::exact::modules::first_segment(detail::ProjectAdapter(prj), s);
        if (!name) return std::nullopt;
        return cb::compat::to_py(*name);
    });
}

// -> (the object module or None, the console text, the addresses of the module's functions)
inline std::tuple<std::optional<py::bytes>, py::str, py::list<int64_t>> compile_module(const Project& prj, const Row& row,
                                                                                       const std::shared_ptr<Toolchain>& tc = nullptr) {
    return cb::compat::guard([&] {
        const detail::ProjectAdapter project(prj);
        const detail::ToolchainAdapter given(tc);
        auto c = cb::exact::modules::compile_module(project, detail::row_to_new(row), detail::types_for(project.src()), tc ? &given : nullptr);
        std::optional<py::bytes> object;
        if (c.object) object = cb::compat::to_py(cb::ByteView(*c.object));
        py::list<int64_t> addrs;
        for (const auto a : c.addresses) addrs.append(int64_t(a));
        return std::make_tuple(object, cb::compat::to_py(c.console), addrs);
    });
}

// check()'s answer
struct CheckResult {
    bool obj = false;                       // an object of the kit (res, dlen, dbad empty)
    std::optional<py::str> error;           // the compiler's console text when it did not compile
    py::list<std::tuple<py::str, std::optional<int64_t>, int64_t, py::str>> res;    // (name, address, length, verdict)
    int64_t dlen = 0;
    py::list<py::str> dbad;
    std::optional<py::bytes> raw;
};

inline CheckResult check(const Project& prj, const Row& row, const compare::Decoder& decode, const std::optional<py::bytes>& raw = std::nullopt) {
    return cb::compat::guard([&] {
        const detail::ProjectAdapter project(prj);
        const compare::detail::OldDecoder decoder(decode);
        std::optional<cb::Bytes> given;
        if (raw) given = cb::compat::from_py(*raw);
        const auto r = cb::exact::modules::check(project, detail::row_to_new(row), detail::types_for(project.src()), given, decoder);
        CheckResult out;
        out.obj = r.kit_object;
        if (r.error) out.error = cb::compat::to_py(*r.error);
        for (const auto& f : r.functions) {
            std::optional<int64_t> a;
            if (f.address) a = *f.address;
            const std::string verdict = f.verdict == cb::exact::modules::Verdict::Same ? "same"
                                        : f.verdict == cb::exact::modules::Verdict::NoPlace ? "no place"
                                                                                           : "differs " + std::to_string(f.differing_bytes);
            out.res.append(std::make_tuple(cb::compat::to_py(f.name), a, int64_t(f.length), cb::compat::to_py(verdict)));
        }
        out.dlen = r.data_length;
        for (const std::string& d : r.data_differences) out.dbad.append(cb::compat::to_py(d));
        if (r.object) out.raw = cb::compat::to_py(cb::ByteView(*r.object));
        return out;
    });
}

inline int64_t main(const Project& prj, const py::list<py::str>& argv, const compare::Decoder& decode) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(detail::utf8(a));
        const detail::ProjectAdapter project(prj);
        const compare::detail::OldDecoder decoder(decode);
        return int64_t(cb::exact::modules::run(project, args, decoder));
    });
}

}  // namespace codebreaker::exact::modules
