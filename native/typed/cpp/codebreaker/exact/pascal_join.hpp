// codebreaker/exact/pascal_join.hpp -- compat shim for cb/exact/pascal_join.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (exact/ledger, exact/repair, exact/project).
//
//     pascal_join::run(project) -> rows as ledger.run's
//
// `Project` is the members of exact.project.Project that pascal_join reads, `Config`/`CompilandCfg` the keys of
// project.json it reads. "equivalent" names a function "module:name"; it is looked up in `equivalents` (a registry of
// the project) and then in the habits comparers that have C++ ports (codebreaker.exact.habits.newcomp:equivalent /
// equivalent3, ...oldcomp:equivalent).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <map>
#include <set>
#include <unordered_set>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/pascal_join.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/habits/cmpp.hpp"
#include "codebreaker/exact/habits/newcomp.hpp"
#include "codebreaker/exact/habits/oldcomp.hpp"
#include "codebreaker/exact/standin.hpp"
#include "py.hpp"

namespace codebreaker::exact::pascal_join {

inline const py::str NL("\n");

// what a compiland's entry of project.json's "compilands" holds
struct CompilandCfg {
    py::str dir;
    py::str kind;                                   // "PROGRAM" or "MODULE"
    std::variant<int64_t, py::str> main = int64_t(0);   // its own block's place
    int64_t parts = 1;
    std::optional<py::list<py::str>> over;          // "over": overlay folders (else the project's)
    std::optional<py::list<py::str>> dirs;          // "dirs": else [dir]
    std::optional<int64_t> unit;
    bool stackck = false;
    std::optional<py::str> equivalent;
};

struct Config {
    py::str root = py::str(".");
    py::list<py::str> over;
    std::optional<py::str> map;
    std::optional<py::str> kinds;
    std::optional<py::str> procedures;              // required to run (KeyError when absent)
    py::dict<py::str, int64_t> units;
    std::optional<py::str> equivalent;
    py::dict<py::str, CompilandCfg> compilands;     // in project.json's order
};

struct Toolchain {
    // compile(source, name) -> (the object's bytes or None, the console text)
    std::function<std::tuple<std::optional<py::bytes>, py::str>(const py::bytes&, const py::str&)> compile;
    int64_t helper_data = 3;                        // the bytes after the entry helper's call
};

// an equivalence comparer: (want, got, offsets the linker fills) -> (same, why)
using Equivalent = std::function<std::tuple<bool, py::list<py::str>>(const py::bytes&, const py::bytes&, const py::set<int64_t>&)>;

struct Project {
    py::str dir;
    Config cfg;
    py::bytes image;
    std::function<Toolchain()> toolchain;
    std::function<codebreaker::exact::compare::Symbols()> symbols;
    codebreaker::exact::compare::Project compare;
    codebreaker::exact::compare::Decoder decode;
    py::dict<py::str, Equivalent> equivalents;      // "module:name" -> comparer, tried before the ported habits
};

namespace detail {

namespace nw = cb::exact::pascal_join;

inline std::string utf8(const py::str& s) { return cb::compat::from_py(s); }

inline nw::CompilandConfig compiland_to_new(const CompilandCfg& u) {
    nw::CompilandConfig n;
    n.dir = utf8(u.dir);
    n.kind = utf8(u.kind) == "PROGRAM" ? nw::CompilandKind::Program : nw::CompilandKind::Module;
    if (const int64_t* i = std::get_if<int64_t>(&u.main)) n.main_at = cb::compat::int_from_py<std::uint32_t>(*i);
    else n.main_at = cb::compat::int_from_py<std::uint32_t>(cb::text::parse_int<int64_t>(utf8(std::get<py::str>(u.main)), 0).value());
    n.parts = static_cast<int>(u.parts);
    const auto list = [](const std::optional<py::list<py::str>>& l) -> std::optional<std::vector<std::string>> {
        if (!l) return std::nullopt;
        std::vector<std::string> out;
        for (const py::str& s : *l) out.push_back(utf8(s));
        return out;
    };
    n.over = list(u.over);
    n.dirs = list(u.dirs);
    n.unit = u.unit;
    n.stackck = u.stackck;
    if (u.equivalent) n.equivalent = utf8(*u.equivalent);
    return n;
}

inline nw::Config config_to_new(const Config& c) {
    nw::Config n;
    n.root = utf8(c.root);
    for (const py::str& o : c.over) n.over.push_back(utf8(o));
    if (c.map) n.map = utf8(*c.map);
    if (c.kinds) n.kinds = utf8(*c.kinds);
    n.procedures = c.procedures ? utf8(*c.procedures) : std::string();
    for (const auto& kv : c.units.items()) n.units[utf8(std::get<0>(kv))] = std::get<1>(kv);
    if (c.equivalent) n.equivalent = utf8(*c.equivalent);
    for (const auto& kv : c.compilands.items()) n.compilands.insert_or_assign(utf8(std::get<0>(kv)), compiland_to_new(std::get<1>(kv)));
    return n;
}

// The new toolchain over the old one's compile function.
class ToolchainAdapter final : public nw::Toolchain {
  public:
    explicit ToolchainAdapter(codebreaker::exact::pascal_join::Toolchain t) : old_(std::move(t)) {}
    nw::CompileResult compile(cb::ByteView source, std::string_view file_name) const override {
        auto [object, console] = old_.compile(cb::compat::to_py(source), cb::compat::to_py(file_name));
        nw::CompileResult r;
        if (object) r.object = cb::compat::from_py(*object);
        r.console = cb::compat::from_py(console);
        return r;
    }
    std::uint32_t helper_data() const override { return cb::compat::int_from_py<std::uint32_t>(old_.helper_data); }

  private:
    codebreaker::exact::pascal_join::Toolchain old_;
};

// The comparers the old layer has: the project's registry, then the ported habits.
inline std::map<std::string, nw::Equivalent> equivalents_of(const Project& prj) {
    namespace habits = codebreaker::exact::habits;
    std::map<std::string, nw::Equivalent> out;
    const auto wrap = [](Equivalent f) {
        return nw::Equivalent([f](cb::ByteView want, cb::ByteView got, const std::set<std::uint32_t>& holes, std::uint32_t helper) {
            habits::cmpp::HDRLEN = helper;   // (the bytes after the entry helper's call)
            py::set<int64_t> fixed;
            for (const auto h : holes) fixed.add(h);
            auto [same, why] = f(cb::compat::to_py(want), cb::compat::to_py(got), fixed);
            nw::Equivalence r;
            r.same = same;
            for (const py::str& w : why) r.why.push_back(cb::compat::from_py(w));
            return r;
        });
    };
    out["codebreaker.exact.habits.newcomp:equivalent"] = wrap(habits::newcomp::equivalent);
    out["codebreaker.exact.habits.newcomp:equivalent3"] = wrap(habits::newcomp::equivalent3);
    out["codebreaker.exact.habits.oldcomp:equivalent"] = wrap(habits::oldcomp::equivalent);
    for (const auto& kv : prj.equivalents.items()) out[utf8(std::get<0>(kv))] = wrap(std::get<1>(kv));
    return out;
}

}  // namespace detail

// A compiland: its pieces joined (the old layer's view of cb::exact::pascal_join::Compiland).
class Compiland {
  public:
    Compiland(const Project& prj, const py::str& name_, const CompilandCfg& u_)
        : project_(std::make_unique<cb::exact::pascal_join::Project>()), name(name_) {
        project_->dir = cb::fs::from_text(detail::utf8(prj.dir));
        project_->config = detail::config_to_new(prj.cfg);
        impl_ = std::make_unique<cb::exact::pascal_join::Compiland>(*project_, detail::utf8(name_), detail::compiland_to_new(u_));
        for (const auto& d : impl_->directories()) dirs.append(cb::compat::to_py(cb::fs::to_text(d)));
    }
    py::str name;
    py::list<py::str> dirs;

    py::list<py::str> pieces() const { return cb::compat::to_py(impl_->pieces()); }
    py::list<py::list<py::str>> parts() const { return cb::compat::guard([&] { return cb::compat::to_py(impl_->parts()); }); }
    py::str source(const py::list<py::str>& part) const {
        return cb::compat::guard([&] { return py::bytes(impl_->source(strings(part))).decode("latin1"); });
    }
    py::list<py::str> names_of(const py::list<py::str>& part) const {
        return cb::compat::guard([&] { return cb::compat::to_py(impl_->names_of(strings(part))); });
    }
    int64_t addr_of(const py::str& h) const { return cb::compat::guard([&] { return int64_t(impl_->address_of(detail::utf8(h))); }); }

  private:
    static std::vector<std::string> strings(const py::list<py::str>& l) {
        std::vector<std::string> out;
        for (const py::str& s : l) out.push_back(cb::compat::from_py(s));
        return out;
    }
    std::unique_ptr<cb::exact::pascal_join::Project> project_;
    std::unique_ptr<cb::exact::pascal_join::Compiland> impl_;
};

using Row = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>>;

// {address: (size, unit)}
inline py::dict<int64_t, std::tuple<std::optional<int64_t>, std::optional<int64_t>>> _procedures(const Project& prj) {
    return cb::compat::guard([&] {
        cb::exact::pascal_join::Project p;
        p.dir = cb::fs::from_text(detail::utf8(prj.dir));
        p.config = detail::config_to_new(prj.cfg);
        py::dict<int64_t, std::tuple<std::optional<int64_t>, std::optional<int64_t>>> out;
        for (const auto& [a, v] : cb::exact::pascal_join::procedures(p)) {
            std::optional<int64_t> size;
            if (v.size) size = *v.size;
            out.set(int64_t(a), std::make_tuple(size, v.unit));
        }
        return out;
    });
}

inline py::list<Row> run(const Project& prj, bool verbose = false) {
    return cb::compat::guard([&] {
        namespace nw = cb::exact::pascal_join;
        const detail::ToolchainAdapter tc(prj.toolchain());
        const cb::Bytes image = cb::compat::from_py(prj.image);
        const compare::Symbols syms = prj.symbols();
        std::unordered_set<std::uint32_t> relocs;
        for (int64_t r : prj.compare.relocs()) relocs.insert(cb::compat::int_from_py<std::uint32_t>(r));
        const compare::detail::OldDecoder decoder(prj.decode);
        nw::Project p;
        p.dir = cb::fs::from_text(detail::utf8(prj.dir));
        p.config = detail::config_to_new(prj.cfg);
        p.image = image;
        p.toolchain = &tc;
        p.symbols = standin::detail::symbols_to_new(syms);
        p.program.dgroup = prj.compare.dgroup;
        p.program.loadseg = cb::compat::int_from_py<std::uint32_t>(prj.compare.loadseg);
        p.program.code_base = [&](std::uint32_t a) { return cb::compat::int_from_py<std::uint32_t>(prj.compare.code_base(a)); };
        p.program.relocs = &relocs;
        p.equivalents = detail::equivalents_of(prj);
        p.decoder = &decoder;
        py::list<Row> out;
        for (const nw::Row& r : nw::run(p, verbose)) {
            std::optional<py::str> procedure;
            if (r.procedure) procedure = cb::compat::to_py(*r.procedure);
            std::optional<int64_t> address;
            if (r.address) address = *r.address;
            out.append(Row(cb::compat::to_py(r.label), procedure, address, int64_t(r.length), cb::compat::to_py(nw::verdict_text(r.verdict)),
                           cb::compat::to_py(r.notes)));
        }
        return out;
    });
}

}  // namespace codebreaker::exact::pascal_join
