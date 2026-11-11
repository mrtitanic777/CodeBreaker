// codebreaker/exact/draft/package.hpp -- compat shim for cb/exact/draft/package.hpp (docs/CPP_DESIGN.md 6.3), from
// codebreaker/exact/draft/__init__.py.
//
// draft -- functions written by an inverse model of the compiler, kept only when the compiler proves them.
//
//     python -m codebreaker exact PROJECT draft [--all] [--limit N] [-v]
//
// The new module is typed (a model reads with explicit `Reading`/`Structuring` arguments, the stage returns what the
// later passes need instead of setting module tables). This header keeps the old layer's shape: the model with its
// attributes the stage sets and resets (`FRAME_AFTER_BP`, `FPCASTS`, `UNSIGNED_CAST`, `SWITCH`, `LAST`), the
// project interface, and the tables FPINFO / FRAME / flow16_cmpswitch; the adapters below turn one into the other.
// The only logic is that adaptation: setting an attribute for the length of one call, as the Python stage did.
// Deleted with its last old-layer consumer.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/log.hpp"
#include "cb/exact/draft/package.hpp"
#include "codebreaker/exact/draft/text.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::exact::draft {

// what the model read of the function besides its text (inv.LAST)
struct Last {
    py::dict<py::str, int64_t> regparam;        // register -> the index of the parameter it carries
    py::dict<py::str, py::str> fptypes;         // the 8087's places -> their types
    std::optional<py::str> fpret;
    py::set<py::str> unsigned_longs;
};

// (fs, params, stmts, pt, loc) of inv.recover / inv.recover_cfg
struct Recovery {
    int64_t fs = 0;
    py::list<py::str> params, stmts;
    std::shared_ptr<void> pt, loc;
};

// where a switch's jump table sends each value, or None (inv.SWITCH)
using SwitchHook = std::function<std::optional<py::list<int64_t>>(int64_t)>;

// a module of codebreaker.invert: the inverse model of a compiler
class InverseModel {
  public:
    virtual ~InverseModel() = default;
    virtual py::str module_name() const = 0;        // (for the AttributeError of an absent attribute)

    virtual Recovery recover(const py::bytes& code) = 0;
    bool has_recover_cfg = false;
    virtual Recovery recover_cfg(const py::bytes& code) = 0;
    virtual py::str to_c(const py::str& name, int64_t fs, const py::list<py::str>& params, const py::list<py::str>& stmts,
                         const std::shared_ptr<void>& pt, const std::shared_ptr<void>& loc) = 0;
    bool has_fold_ladders = false;
    virtual py::list<py::str> _fold_ladders(const py::list<py::str>& stmts) = 0;

    // the module's switches, written by the stage (absent: None of the optional)
    std::optional<bool> FRAME_AFTER_BP, REGVARS_EAGER, FPCASTS, UNSIGNED_CAST;
    bool has_switch = false;
    SwitchHook SWITCH;                                // (empty: None)
    Last LAST;                                        // (absent: the empty one)
};

// table_hook(code): the function SWITCH takes; `base` is set by the caller
class TableHook {
  public:
    int64_t base = 0;
    explicit TableHook(const py::bytes& code) : code_(code) {}
    std::optional<py::list<int64_t>> operator()(int64_t addr) const {
        if (!hook_ || hook_base_ != base) {
            hook_ = std::make_shared<cb::exact::draft::TableHook>(cb::compat::from_py(code_), cb::compat::int_from_py<std::uint32_t>(base));
            hook_base_ = base;
        }
        const auto places = (*hook_)(cb::compat::int_from_py<std::uint32_t>(addr));
        if (!places) return std::nullopt;
        return cb::compat::to_py(*places);
    }

  private:
    py::bytes code_;
    mutable std::shared_ptr<cb::exact::draft::TableHook> hook_;
    mutable int64_t hook_base_ = 0;
};

// SWITCH(address) for the model: the places a near jump through a table goes (`jmp cs:[bx + T]`, after `cmp R, N`: N + 1
// words at T) - addresses in the function's own code, from 0. T is the program's offset of the table, so the function's
// own offset is found where its code begins being the program's
inline std::shared_ptr<TableHook> table_hook(const py::bytes& code) { return std::make_shared<TableHook>(code); }

// the code with the floating-point emulator's calls (int 34h..3Bh: an 8087 instruction, its first byte D8h + n; int 3Ch:
// one with a segment; int 3Dh: fwait) written as the 8087's own - each as long as it was (a wait before it), so the model
// reads them as the instructions they stand for
inline py::bytes fp_native(const py::bytes& code) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::exact::draft::fp_native(cb::compat::from_py(code)))); });
}

// an arm that is only `goto L;`, L the start of statements to the end of their block: those statements in its place (the
// compiler's one copy of a tail two arms share); L dropped when nothing goes to it then
inline py::list<py::str> inline_goto_arms(const py::list<py::str>& lines) {
    return cb::compat::to_py(cb::exact::draft::inline_goto_arms(cb::compat::from_py(lines)));
}

// candidates.FPINFO: what the model read of the 8087's values (types by place, the return, whether the code has any)
struct FpInfo {
    py::dict<py::str, py::str> types;
    std::optional<py::str> ret;
    bool fp = false;
};
inline FpInfo FPINFO;
// candidates.FRAME: the one-element list [the frame's size]
inline py::list<int64_t> FRAME{0};
// invert.flow16.CMPSWITCH: switches by compares read as switches
inline bool flow16_cmpswitch = false;

namespace detail {

// A new-tree model over an old one: each call sets the old module's attributes for its length, as the stage did.
class NewModel final : public cb::exact::draft::InverseModel {
public:
    explicit NewModel(draft::InverseModel& old) : old_(old) {}
    cb::exact::draft::ModelCapabilities capabilities() const override {
        cb::exact::draft::ModelCapabilities c;
        c.frame_after_bp = old_.FRAME_AFTER_BP.has_value();
        c.regvars_eager = old_.REGVARS_EAGER.has_value();
        c.structured = old_.has_recover_cfg;
        c.table_switch = old_.has_switch;
        c.fp_casts = old_.FPCASTS.has_value();
        c.unsigned_compares = old_.UNSIGNED_CAST.has_value();
        c.fold_ladders = old_.has_fold_ladders;
        return c;
    }
    cb::exact::draft::Recovery recover(cb::ByteView code, const cb::exact::draft::Reading& reading) override {
        set_reading(reading);
        return convert(old_.recover(cb::compat::to_py(code)));
    }
    cb::exact::draft::Recovery recover_structured(cb::ByteView code, const cb::exact::draft::Reading& reading,
                                                  const cb::exact::draft::Structuring& how) override {
        set_reading(reading);
        struct Restore {   // (the module's switches back as they were, whatever the call does)
            draft::InverseModel& m;
            SwitchHook switch_;
            std::optional<bool> unsigned_cast;
            bool fp_casts;
            ~Restore() {
                flow16_cmpswitch = false;
                if (m.has_switch) m.SWITCH = switch_;
                if (fp_casts) m.FPCASTS = false;
                m.UNSIGNED_CAST = unsigned_cast;
            }
        } restore{old_, old_.SWITCH, old_.UNSIGNED_CAST, how.fp_casts};
        flow16_cmpswitch = how.compare_switches;
        if (how.table_switch && old_.has_switch) {
            const auto hook = how.table_switch;
            old_.SWITCH = [hook](int64_t address) -> std::optional<py::list<int64_t>> {
                const auto places = hook(cb::compat::int_from_py<std::uint32_t>(address));
                if (!places) return std::nullopt;
                return cb::compat::to_py(*places);
            };
        }
        if (how.fp_casts) old_.FPCASTS = true;
        if (how.unsigned_compares) old_.UNSIGNED_CAST = true;
        return convert(old_.recover_cfg(cb::compat::to_py(code)));
    }
    std::string to_c(std::string_view name, const cb::exact::draft::Recovery& r) override {
        const auto state = std::any_cast<std::pair<std::shared_ptr<void>, std::shared_ptr<void>>>(&r.model_state);
        return old_.to_c(py::str(std::string(name)), r.frame_size, cb::compat::to_py(r.params), cb::compat::to_py(r.stmts),
                         state != nullptr ? state->first : nullptr, state != nullptr ? state->second : nullptr)
            .utf8();
    }
    std::vector<std::string> fold_ladders(const std::vector<std::string>& stmts) override {
        return cb::compat::from_py(old_._fold_ladders(cb::compat::to_py(stmts)));
    }
    cb::exact::draft::ModelLast last() const override {
        cb::exact::draft::ModelLast l;
        for (const auto& kv : old_.LAST.regparam.items()) l.regparam.insert_or_assign(std::get<0>(kv).utf8(), static_cast<int>(std::get<1>(kv)));
        for (const auto& kv : old_.LAST.fptypes.items()) l.fptypes[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
        if (old_.LAST.fpret) l.fpret = old_.LAST.fpret->utf8();
        for (const py::str& s : old_.LAST.unsigned_longs) l.unsigned_longs.insert(s.utf8());
        return l;
    }

private:
    void set_reading(const cb::exact::draft::Reading& r) {   // (a draft is of 3.00 or later code: its -Gs frames, its register variables)
        if (old_.FRAME_AFTER_BP) old_.FRAME_AFTER_BP = r.frame_after_bp;
        if (old_.REGVARS_EAGER) old_.REGVARS_EAGER = r.regvars_eager;
    }
    static cb::exact::draft::Recovery convert(const Recovery& r) {
        cb::exact::draft::Recovery out;
        out.frame_size = cb::compat::int_from_py<std::uint32_t>(r.fs);
        out.params = cb::compat::from_py(r.params);
        out.stmts = cb::compat::from_py(r.stmts);
        out.model_state = std::make_pair(r.pt, r.loc);
        return out;
    }
    draft::InverseModel& old_;
};

}  // namespace detail

// [(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk; a
// structured control-flow graph, where the model has one): the compiler decides between them. Sets FRAME, FPINFO and
// text::LONGINFO, which the later passes read.
inline py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> model_texts(InverseModel& inv, const py::bytes& code, const py::str& name,
                                                                             std::optional<int64_t> base = std::nullopt) {
    detail::NewModel model(inv);
    const cb::exact::draft::ModelTexts t = cb::compat::guard([&] {
        return cb::exact::draft::model_texts(model, cb::compat::from_py(code), name.utf8(),
                                             base ? std::optional<std::uint32_t>(cb::compat::int_from_py<std::uint32_t>(*base)) : std::nullopt);
    });
    FRAME[0] = t.frame_size;
    FPINFO = FpInfo();
    for (const auto& [k, v] : t.fp.types) FPINFO.types.set(py::str(k), py::str(v));
    if (t.fp.ret) FPINFO.ret = py::str(*t.fp.ret);
    FPINFO.fp = t.fp.fp;
    text::LONGINFO.clear();
    text::LONGINFO.unsigned_ = cb::compat::to_py(t.longs.unsigned_names);
    if (t.longs.ret) text::LONGINFO.ret = *t.longs.ret;
    py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> out;
    for (const auto& d : t.texts) {
        py::dict<py::str, int64_t> regparam;
        for (const auto& [reg, ix] : d.regparam) regparam.set(py::str(reg), ix);
        out.append(std::make_tuple(py::str(d.text), regparam));
    }
    return out;
}

// ---- the project and the stages draft asks of it ------------------------------------------------------------------------------
struct LibModule {
    py::dict<py::str, std::tuple<int64_t, int64_t>> pubs;     // name -> (segment, offset)
    int64_t first_code_segment = 0;                           // m.code[0][0]
};
struct LibPlace {
    int64_t code = 0;
    LibModule m;
    py::list<LibModule> also;
};
struct Located {
    py::list<LibPlace> coded, lone;
};

class Project {
  public:
    virtual ~Project() = default;
    virtual py::str toolchain_name() = 0;                     // prj.toolchain().name
    virtual std::optional<py::str> inverse_model() = 0;       // prj.toolchain().p.get("inverse")
    virtual Located locate() = 0;                             // exact.library.locate(prj): .coded, .lone
    virtual py::dict<py::str, std::tuple<py::str, int64_t>> symbols() = 0;     // {name: (type, address)}
    virtual py::bytes image() = 0;                            // prj.image
    virtual py::set<int64_t> source_addresses() = 0;          // {a for a, p in prj.sources()}
    virtual py::str path(const py::str& name) = 0;            // prj.path(name)
    virtual py::list<std::tuple<int64_t, int64_t, py::str>> functions() = 0;   // exact.carve.functions(prj)
    // draft.prove.prove_texts(prj, tc, inv, texts, a, ln, names) -> (source, verdict)
    virtual std::tuple<py::str, py::str> prove_texts(InverseModel& inv, const py::list<std::tuple<py::str, py::dict<py::str, int64_t>>>& texts,
                                                     int64_t a, int64_t ln, const py::dict<int64_t, py::str>& names) = 0;
};

// importlib.import_module("codebreaker.invert." + model)
inline std::function<std::shared_ptr<InverseModel>(const py::str&)> inverse_model_loader;

namespace detail {

// The new-tree project over an old one.
class NewProject final : public cb::exact::draft::Project {
public:
    explicit NewProject(draft::Project& p) : p_(p) {
        symbols_ = [&] {
            cb::exact::SymbolTable t;
            for (const auto& kv : p.symbols().items()) {
                const std::string kind = std::get<0>(std::get<1>(kv)).utf8();
                t.insert_or_assign(std::get<0>(kv).utf8(),
                                   cb::exact::SymPlace{kind == "C" ? cb::exact::SymKind::Code : kind == "D" ? cb::exact::SymKind::Data
                                                       : kind == "S" ? cb::exact::SymKind::Segment : cb::exact::SymKind::Unknown,
                                                       std::get<1>(std::get<1>(kv))});
            }
            return t;
        }();
        image_ = cb::compat::from_py(p.image());
    }
    std::string toolchain_name() const override { return p_.toolchain_name().utf8(); }
    std::optional<std::string> inverse_model_name() const override {
        const auto m = p_.inverse_model();
        return m ? std::optional<std::string>(m->utf8()) : std::nullopt;
    }
    std::shared_ptr<cb::exact::draft::InverseModel> load_inverse_model(std::string_view name) override {
        if (!inverse_model_loader) throw cb::NotFoundError(std::string("codebreaker.invert.") + std::string(name), 0, "no such inverse model");
        old_model_ = inverse_model_loader(py::str(std::string(name)));
        return std::make_shared<NewModel>(*old_model_);
    }
    std::vector<cb::exact::draft::LibraryPlace> library_places() const override {
        const auto module = [](const LibModule& m) {
            cb::exact::draft::LibraryModule out;
            for (const auto& kv : m.pubs.items())
                out.publics.insert_or_assign(std::get<0>(kv).utf8(), std::make_pair(cb::compat::int_from_py<std::uint32_t>(std::get<0>(std::get<1>(kv))),
                                                                                  cb::compat::int_from_py<std::uint32_t>(std::get<1>(std::get<1>(kv)))));
            out.first_code_segment = cb::compat::int_from_py<std::uint32_t>(m.first_code_segment);
            return out;
        };
        std::vector<cb::exact::draft::LibraryPlace> places;
        const Located located = p_.locate();
        for (const py::list<LibPlace>* group : {&located.coded, &located.lone})
            for (const LibPlace& lp : *group) {
                cb::exact::draft::LibraryPlace place;
                place.code = cb::compat::int_from_py<std::uint32_t>(lp.code);
                place.module = module(lp.m);
                for (const LibModule& m : lp.also) place.also.push_back(module(m));
                places.push_back(std::move(place));
            }
        return places;
    }
    const cb::exact::SymbolTable& symbols() const override { return symbols_; }
    cb::ByteView image() const override { return image_; }
    std::set<std::uint32_t> source_addresses() const override {
        std::set<std::uint32_t> out;
        for (int64_t a : p_.source_addresses()) out.insert(cb::compat::int_from_py<std::uint32_t>(a));
        return out;
    }
    std::filesystem::path path(std::string_view name) const override { return std::filesystem::path(p_.path(py::str(std::string(name))).utf8()); }
    std::vector<std::pair<std::uint32_t, std::uint32_t>> functions() const override {
        std::vector<std::pair<std::uint32_t, std::uint32_t>> out;
        for (const auto& f : p_.functions())
            out.emplace_back(cb::compat::int_from_py<std::uint32_t>(std::get<0>(f)), cb::compat::int_from_py<std::uint32_t>(std::get<1>(f)));
        return out;
    }
    cb::exact::draft::ProveOutcome prove_texts(cb::exact::draft::InverseModel&, const cb::exact::draft::ModelTexts& texts, std::uint32_t address,
                                               std::uint32_t length, const std::map<std::uint32_t, std::string>& names) override {
        // (the old prover reads FRAME, FPINFO and LONGINFO, which the old model_texts set; set them from these texts)
        FRAME[0] = texts.frame_size;
        FPINFO = FpInfo();
        for (const auto& [k, v] : texts.fp.types) FPINFO.types.set(py::str(k), py::str(v));
        if (texts.fp.ret) FPINFO.ret = py::str(*texts.fp.ret);
        FPINFO.fp = texts.fp.fp;
        text::LONGINFO.clear();
        text::LONGINFO.unsigned_ = cb::compat::to_py(texts.longs.unsigned_names);
        if (texts.longs.ret) text::LONGINFO.ret = *texts.longs.ret;
        py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> list;
        for (const auto& d : texts.texts) {
            py::dict<py::str, int64_t> regparam;
            for (const auto& [reg, ix] : d.regparam) regparam.set(py::str(reg), ix);
            list.append(std::make_tuple(py::str(d.text), regparam));
        }
        py::dict<int64_t, py::str> old_names;
        for (const auto& [a, n] : names) old_names.set(a, py::str(n));
        const auto [source, verdict] = p_.prove_texts(*old_model_, list, address, length, old_names);
        return {source.utf8(), verdict.utf8()};
    }

private:
    draft::Project& p_;
    std::shared_ptr<draft::InverseModel> old_model_;
    cb::exact::SymbolTable symbols_;
    cb::Bytes image_;
};

}  // namespace detail

// {image offset: name} of the code the program has names for (SYMBOLS, the library's modules)
inline py::dict<int64_t, py::str> _names(Project& prj) {
    detail::NewProject p(prj);
    py::dict<int64_t, py::str> out;
    for (const auto& [a, n] : cb::exact::draft::code_names(p)) out.set(a, py::str(n));
    return out;
}

// draft.run(project) -> {address: verdict}
inline py::dict<int64_t, py::str> run(Project& prj, bool every = false, std::optional<int64_t> limit = std::nullopt, bool verbose = false) {
    detail::NewProject p(prj);
    cb::exact::draft::Options options;
    options.every = every;
    if (limit) options.limit = cb::compat::int_from_py<std::size_t>(*limit);
    options.verbose = verbose;
    py::dict<int64_t, py::str> out;
    try {
        for (const auto& [a, v] : cb::exact::draft::run(p, options)) out.set(a, py::str(v));
    } catch (const cb::UsageError& e) {   // (the toolchain names no inverse model: SystemExit(text), as it was)
        throw py::SystemExit(py::str(e.what()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
    cb::flush_out();
    return out;
}

inline int64_t main(Project& prj, const py::list<py::str>& argv) {
    cb::exact::draft::Options options;
    for (std::size_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == py::str("--all")) options.every = true;
        else if (argv[i] == py::str("-v")) options.verbose = true;
        else if (argv[i] == py::str("--limit") && i + 1 < argv.size()) options.limit = static_cast<std::size_t>(py::int_from_str(argv[i + 1]));
    }
    detail::NewProject p(prj);
    int64_t code = 0;
    try {
        code = cb::exact::draft::run_command(p, options);
    } catch (const cb::UsageError& e) {
        throw py::SystemExit(py::str(e.what()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
    cb::flush_out();
    return code;
}

}  // namespace codebreaker::exact::draft
