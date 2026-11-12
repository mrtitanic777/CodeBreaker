// codebreaker/exact/ledger.hpp -- compat shim for cb/exact/ledger.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer (exact/project, exact/identify, exact/repair).
//
// ledger.py -- every one-function source of a project compiled by its toolchain and put against the program.
//
//     python -m codebreaker exact PROJECT ledger [--learn] [-v] [--write]
//
//     ledger::run(project, learn=false) -> [(file, function, address, length, verdict, notes, by)]
//         verdict: identical | differs | no place | does not compile
//         by:      the labels of the toolchains that give the function's bytes
//
// `Project` is the old layer's own interface (the members of exact.project.Project that ledger.py reads, and what
// compare.function takes of a project); the real one implements it. A row is (file, function or None, address or
// None, length, verdict, notes); a finished row has `by` after the notes, as a 7-tuple (Row7). What main prints goes
// to stdout (cb::out), not to a stream. Not shimmed: the `__name__` block (there is none).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/exact/ledger.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/pascal_join.hpp"
#include "codebreaker/exact/standin.hpp"
#include "py.hpp"

namespace codebreaker::exact::ledger {

using Symbols = compare::Symbols;
using Toolchain = modules::Toolchain;

// exact.project.Project, as ledger.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::str src() const = 0;                                    // prj.src
    virtual py::str dir() const = 0;                                    // prj.dir
    virtual py::str exe() const = 0;                                    // prj.exe: the program's file
    virtual py::str path(const py::str& name) const = 0;                // prj.path(name)
    virtual std::optional<py::str> cfg_str(const py::str& key) const = 0;   // prj.cfg.get(key) (a string entry): "named_by", "toolchain"
    virtual bool has_compilands() const = 0;                            // bool(prj.cfg.get("compilands"))
    virtual pascal_join::Project pascal_project() const = 0;            // the project as pascal_join reads it
    virtual py::dict<py::str, py::bytes> headers() const = 0;           // prj.headers()
    virtual py::list<py::str> include() const = 0;                      // prj.include
    virtual std::optional<py::str> switches() const = 0;                // prj.switches
    virtual py::list<int64_t> starts() const = 0;                       // prj.starts() (ascending)
    virtual bool whole(const py::str& path) const = 0;                  // prj.whole(path)
    virtual py::bytes image() const = 0;                                // prj.image
    virtual int64_t code_end() const = 0;                               // prj.code_end (0: none)
    virtual py::list<std::tuple<py::str, std::shared_ptr<Toolchain>>> toolchains() const = 0;   // prj.toolchains(): (label, toolchain)
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual void save_symbols(const Symbols& syms) const = 0;           // prj.save_symbols(s)
    // prj.sources(): (address or None, source file)
    virtual py::list<std::tuple<std::optional<int64_t>, py::str>> sources() const = 0;
    // prj.sets(): the project's source sets (the project itself, then those of "also"); a set is a Project
    virtual py::list<std::shared_ptr<const Project>> sets() const = 0;
    // for compare.function
    virtual int64_t dgroup() const = 0;
    virtual int64_t loadseg() const = 0;
    virtual int64_t code_base(int64_t a) const = 0;
    virtual const py::set<int64_t>& relocs() const = 0;
    virtual compare::Decoder decoder() const = 0;
};

using By = py::list<std::optional<py::str>>;
// (file, function, address, length, verdict, notes)
using Row = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>>;
// ... and by
using Row7 = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>, By>;

namespace detail {

namespace nl = cb::exact::ledger;
using OldProject = codebreaker::exact::ledger::Project;   // (inside NewView, `Project` is the new layer's base class)

inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }
inline py::str text_of(const std::filesystem::path& p) { return cb::compat::to_py(cb::fs::to_text(p)); }

inline nl::Verdict verdict_of(const py::str& v) {
    namespace pj = cb::exact::pascal_join;
    const std::string t = cb::compat::from_py(v);
    for (const auto k : {pj::Verdict::DoesNotCompile, pj::Verdict::NoPlace, pj::Verdict::Identical, pj::Verdict::Equivalent, pj::Verdict::Differs})
        if (pj::verdict_text(k) == t) return k;
    return pj::Verdict::Differs;
}

inline Symbols symbols_to_old(const cb::exact::SymbolTable& table) {
    Symbols out;
    for (const auto& [name, place] : table)
        out.set(cb::compat::to_py(name), std::make_tuple(cb::compat::to_py(std::string(1, cb::exact::sym_kind_letter(place.kind))), place.address));
    return out;
}

inline Row7 row_to_old(const nl::Row& r) {
    std::optional<py::str> function;
    if (r.function) function = cb::compat::to_py(*r.function);
    std::optional<int64_t> address;
    if (r.address) address = *r.address;
    By by;
    for (const std::string& b : r.by) by.append(cb::compat::to_py(b));
    return Row7(cb::compat::to_py(r.file), function, address, int64_t(r.length), cb::compat::to_py(cb::exact::pascal_join::verdict_text(r.verdict)),
                cb::compat::to_py(r.notes), by);
}

inline nl::Row row_to_new(const Row7& r) {
    nl::Row n;
    n.file = cb::compat::from_py(std::get<0>(r));
    if (std::get<1>(r)) n.function = cb::compat::from_py(*std::get<1>(r));
    if (std::get<2>(r)) n.address = cb::compat::int_from_py<std::uint32_t>(*std::get<2>(r));
    n.length = cb::compat::int_from_py<std::uint32_t>(std::get<3>(r));
    n.verdict = verdict_of(std::get<4>(r));
    for (const py::str& note : std::get<5>(r)) n.notes.push_back(cb::compat::from_py(note));
    for (const auto& b : std::get<6>(r))
        if (b) n.by.push_back(cb::compat::from_py(*b));
    return n;
}

// The old project as the new layer's interface.
class NewView final : public nl::Project {
  public:
    explicit NewView(std::shared_ptr<const OldProject> old) : old_(std::move(old)), decode_(old_->decoder()), decoder_(decode_) {}
    explicit NewView(const OldProject& old) : NewView(std::shared_ptr<const OldProject>(std::shared_ptr<void>(), &old)) {}

    std::filesystem::path dir() const override { return path_of(old_->dir()); }
    std::filesystem::path src() const override { return path_of(old_->src()); }
    std::string exe() const override { return cb::compat::from_py(old_->exe()); }
    std::filesystem::path path(std::string_view name) const override { return path_of(old_->path(cb::compat::to_py(name))); }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto v = old_->cfg_str(cb::compat::to_py(key));
        return v ? std::optional<std::string>(cb::compat::from_py(*v)) : std::nullopt;
    }
    const cb::exact::pascal_join::Project* compilands() const override {
        if (!old_->has_compilands()) return nullptr;
        if (!pascal_) {
            pascal_.emplace(old_->pascal_project());
            pascal_view_ = std::make_unique<pascal_join::detail::NewProject>(*pascal_);
        }
        return &pascal_view_->get();
    }
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
    std::vector<std::uint32_t> starts() const override {
        std::vector<std::uint32_t> out;
        for (const int64_t s : old_->starts()) out.push_back(cb::compat::int_from_py<std::uint32_t>(s));
        return out;
    }
    bool whole(const std::filesystem::path& file) const override { return old_->whole(text_of(file)); }
    cb::ByteView image() const override {
        if (!image_) image_ = cb::compat::from_py(old_->image());
        return *image_;
    }
    std::uint32_t code_end() const override { return cb::compat::int_from_py<std::uint32_t>(old_->code_end()); }
    std::vector<nl::LabelledToolchain> toolchains() const override {
        std::vector<nl::LabelledToolchain> out;
        for (const auto& [label, tc] : old_->toolchains())
            out.push_back({cb::compat::from_py(label), std::make_shared<modules::detail::ToolchainAdapter>(tc)});
        return out;
    }
    cb::exact::SymbolTable symbols() const override { return standin::detail::symbols_to_new(old_->symbols()); }
    void save_symbols(const cb::exact::SymbolTable& table) const override { old_->save_symbols(symbols_to_old(table)); }
    std::vector<nl::SourceFile> sources() const override {
        std::vector<nl::SourceFile> out;
        for (const auto& [at, path] : old_->sources()) out.push_back({at, path_of(path)});
        return out;
    }
    std::vector<std::shared_ptr<const nl::Project>> sets() const override {
        std::vector<std::shared_ptr<const nl::Project>> out;
        for (const auto& s : old_->sets()) out.push_back(std::make_shared<NewView>(s));
        return out;
    }
    cb::exact::compare::Program program() const override {
        if (!relocs_) {
            relocs_.emplace();
            for (const int64_t r : old_->relocs()) relocs_->insert(cb::compat::int_from_py<std::uint32_t>(r));
        }
        cb::exact::compare::Program p;
        p.dgroup = old_->dgroup();
        p.loadseg = cb::compat::int_from_py<std::uint32_t>(old_->loadseg());
        p.code_base = [o = old_](std::uint32_t a) { return cb::compat::int_from_py<std::uint32_t>(o->code_base(a)); };
        p.relocs = &*relocs_;
        return p;
    }
    const cb::exact::x86::Decoder16& decoder() const override { return decoder_; }

  private:
    std::shared_ptr<const OldProject> old_;
    compare::Decoder decode_;
    compare::detail::OldDecoder decoder_;
    mutable std::optional<cb::Bytes> image_;
    mutable std::optional<std::unordered_set<std::uint32_t>> relocs_;
    mutable std::optional<pascal_join::Project> pascal_;
    mutable std::unique_ptr<pascal_join::detail::NewProject> pascal_view_;
};

}  // namespace detail

// one source file compiled by one toolchain and held to the program; `learnt` gets the places of the functions
// found identical (when all of the file's placed functions are)
inline py::list<Row> check_file(const Project& prj, const Toolchain& tc, const py::str& path, std::optional<int64_t> at, const Symbols& syms,
                                Symbols& learnt) {
    return cb::compat::guard([&] {
        namespace nl = cb::exact::ledger;
        cb::exact::SymbolTable learned;
        const detail::NewView view(prj);
        const modules::detail::ToolchainAdapter adapter(std::shared_ptr<modules::Toolchain>(std::shared_ptr<void>(), const_cast<Toolchain*>(&tc)));
        py::list<Row> out;
        for (const nl::Row& r : nl::check_file(view, adapter, detail::path_of(path), at, standin::detail::symbols_to_new(syms), learned)) {
            const Row7 full = detail::row_to_old(r);
            out.append(Row(std::get<0>(full), std::get<1>(full), std::get<2>(full), std::get<3>(full), std::get<4>(full), std::get<5>(full)));
        }
        for (const auto& [name, place] : learned) {   // (what the old port put in `learnt`)
            const Symbols one = detail::symbols_to_old(cb::exact::SymbolTable{{name, place}});
            for (const auto& kv : one.items()) learnt.set(std::get<0>(kv), std::get<1>(kv));
        }
        return out;
    });
}

// after a line /* SAMEFILE */: empty stand-ins of functions of the program's same file, which a far function
// calls another way than one of another file (push cs, call near) - they are compiled and not compared
inline py::set<py::str> _stubs(const py::bytes& src) {
    return cb::compat::guard([&] {
        py::set<py::str> out;
        const cb::Bytes raw = cb::compat::from_py(src);
        for (const std::string& s : cb::exact::ledger::stubs(cb::as_text(raw))) out.add(cb::compat::to_py(s));
        return out;
    });
}

// the function goes on to the next start (or to a pad of zeros before it: a segment's end)
inline bool _reaches(const Project& prj, const py::list<int64_t>& starts, int64_t a, int64_t ln) {
    return cb::compat::guard([&] {
        std::vector<std::uint32_t> s;
        for (const int64_t x : starts) s.push_back(cb::compat::int_from_py<std::uint32_t>(x));
        const cb::Bytes image = cb::compat::from_py(prj.image());
        return cb::exact::ledger::reaches(s, cb::compat::int_from_py<std::uint32_t>(prj.code_end()), image, cb::compat::int_from_py<std::uint32_t>(a),
                                          cb::compat::int_from_py<std::uint32_t>(ln));
    });
}

// one file's rows under each toolchain [(label, rows)] -> its rows, a function identical when one gave it
inline py::list<Row7> _merge(const py::list<std::tuple<py::str, py::list<Row>>>& per) {
    return cb::compat::guard([&] {
        namespace nl = cb::exact::ledger;
        std::vector<nl::Attempt> attempts;
        for (const auto& [label, rows] : per) {
            nl::Attempt a{cb::compat::from_py(label), {}};
            for (const Row& r : rows)
                a.rows.push_back(detail::row_to_new(Row7(std::get<0>(r), std::get<1>(r), std::get<2>(r), std::get<3>(r), std::get<4>(r), std::get<5>(r), By())));
            attempts.push_back(std::move(a));
        }
        py::list<Row7> out;
        for (const nl::Row& r : nl::merge(attempts)) out.append(detail::row_to_old(r));
        return out;
    });
}

inline py::list<Row7> run(const Project& prj, bool learn = false, bool verbose = false) {
    return cb::compat::guard([&] {
        py::list<Row7> out;
        for (const cb::exact::ledger::Row& r : cb::exact::ledger::run(detail::NewView(prj), learn, verbose)) out.append(detail::row_to_old(r));
        return out;
    });
}

inline void write(const Project& prj, const py::list<Row7>& rows) {
    cb::compat::guard([&] {
        std::vector<cb::exact::ledger::Row> rs;
        for (const Row7& r : rows) rs.push_back(detail::row_to_new(r));
        cb::exact::ledger::write(detail::NewView(prj), rs);
    });
}

inline int64_t main(const Project& prj, const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return int64_t(cb::exact::ledger::command(detail::NewView(prj), args));
    });
}

}  // namespace codebreaker::exact::ledger
