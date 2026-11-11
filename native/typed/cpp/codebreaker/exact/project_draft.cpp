// codebreaker/exact/project_draft.cpp -- the draft stage's view of a project (exact.project.Project, as exact.draft asks for it).
//
// project::as_draft(p) is the draft::Project of a real project: the library's places, the carve, the toolchain, and the
// two stages the draft stage hands a function to -- the inverse model (invert::early_c, the one a toolchain profile names)
// and the compiler's proof (draft::prove::prove_texts, over forms::Prj below). project::draft_prj(p) is that forms::Prj:
// what forms, candidates and prove read of the project and its toolchain's profile.
//
// It is a file of its own so that the harnesses that do not draft do not link the draft package and the model.
#include <map>

#include "codebreaker/exact/batch.hpp"
#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/draft/package.hpp"
#include "codebreaker/exact/draft/prove.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/project.hpp"
#include "codebreaker/invert/early_c.hpp"
#include "json.hpp"
#include "py_json.hpp"
#include "value.hpp"

namespace codebreaker::exact::project {

using py::str;

namespace {

str vstr(const py::V& v) { return str(v.str()); }

std::optional<str> opt_str(const py::V& d, const char* key) {
    const py::V& v = d.get(key);
    if (v.is_none() || !v.is_str()) return std::nullopt;
    return vstr(v);
}

py::list<str> str_list(const py::V& v) {
    py::list<str> out;
    if (v.is_seq()) {
        for (const py::V& x : v.seq()) out.append(vstr(x));
    }
    return out;
}

// the profile of the project's toolchain, as the file has it (toolchains/NAME.json)
py::V profile_json(const toolchain::Toolchain& tc) {
    str file = py::os::path::join(py::os::path::join(toolchain::HERE(), str("toolchains")), tc.p.name + str(".json"));
    py::file f = py::open(file);
    str text = f.read_text();
    f.close();
    return py::json_parse(text.utf8());
}

// the compiler as draft.prove asks for it: tc.compile(src, headers=, include=, switches=)
class BatchToolchain : public batch::Toolchain {
  public:
    explicit BatchToolchain(const std::shared_ptr<toolchain::Toolchain>& t) : tc(t) {}
    std::tuple<std::optional<py::bytes>, str> compile(const py::bytes& src, const py::dict<str, py::bytes>& headers, const py::list<str>& include,
                                                      const std::optional<str>& switches) override {
        return tc->compile(src, headers, include, switches);
    }

  private:
    std::shared_ptr<toolchain::Toolchain> tc;
};

// codebreaker.invert.early_c as the draft stage's inverse model. The Python's module is its own state (the settings the
// stage writes, LAST, the flags); the C++ module's are namespace variables, so each call puts this object's settings into
// them first and reads LAST back after.
class EarlyCModel : public draft::InverseModel {
  public:
    EarlyCModel() {
        has_recover_cfg = true;
        has_fold_ladders = true;
        has_mark = true;
        has_switch = true;
        FRAME_AFTER_BP = invert::early_c::FRAME_AFTER_BP;
        REGVARS_EAGER = invert::early_c::REGVARS_EAGER;
        FPCASTS = invert::early_c::FPCASTS;
        UNSIGNED_CAST = invert::early_c::UNSIGNED_CAST;
        SWITCH = invert::early_c::SWITCH;
    }

    str module_name() const override { return str("codebreaker.invert.early_c"); }

    draft::Recovery recover(const py::bytes& code) override {
        push();
        return recovered(invert::early_c::recover(code));
    }
    draft::Recovery recover_cfg(const py::bytes& code) override {
        push();
        return recovered(invert::early_c::recover_cfg(code));
    }
    str to_c(const str& name, int64_t fs, const py::list<str>& params, const py::list<str>& stmts, const std::shared_ptr<void>& pt,
             const std::shared_ptr<void>& loc) override {
        push();
        return invert::early_c::to_c(name, fs, params, stmts, *std::static_pointer_cast<str>(pt),
                                     std::optional<py::list<int64_t>>(*std::static_pointer_cast<py::list<int64_t>>(loc)));
    }
    py::list<str> _fold_ladders(const py::list<str>& stmts) override {
        push();
        return invert::early_c::_fold_ladders(stmts);
    }

    // inv.MARK = True; recover; to_c("f", ..); inv.MARK = old
    std::optional<str> checked_c(const py::bytes& code) override {
        bool old = invert::early_c::MARK;
        invert::early_c::MARK = true;
        std::optional<str> out;
        try {
            draft::Recovery r = recover(code);
            out = to_c(str("f"), r.fs, r.params, r.stmts, r.pt, r.loc);
        } catch (const std::exception&) {
            out = std::nullopt;
        }
        invert::early_c::MARK = old;
        return out;
    }

  private:
    // the settings of this object, in the module
    void push() const {
        if (FRAME_AFTER_BP) invert::early_c::FRAME_AFTER_BP = *FRAME_AFTER_BP;
        if (REGVARS_EAGER) invert::early_c::REGVARS_EAGER = *REGVARS_EAGER;
        if (FPCASTS) invert::early_c::FPCASTS = *FPCASTS;
        if (UNSIGNED_CAST) invert::early_c::UNSIGNED_CAST = *UNSIGNED_CAST;
        invert::early_c::SWITCH = SWITCH;
        invert::early_c::flow16_CMPSWITCH = draft::flow16_cmpswitch;   // (flow16.CMPSWITCH, which the stage sets)
    }
    // the module's LAST, read back; its answer as the stage's Recovery
    draft::Recovery recovered(const invert::early_c::Recovered& r) {
        const invert::early_c::Last& l = invert::early_c::LAST;
        LAST = draft::Last();
        if (l.regparam) LAST.regparam = l.regparam->copy();
        if (l.fptypes) LAST.fptypes = l.fptypes->copy();
        LAST.fpret = l.fpret;
        if (l.unsigned_longs) LAST.unsigned_longs = py::set<str>::from(*l.unsigned_longs);
        draft::Recovery out;
        out.fs = std::get<0>(r);
        out.params = std::get<1>(r);
        out.stmts = std::get<2>(r);
        out.pt = std::make_shared<str>(std::get<3>(r));
        out.loc = std::make_shared<py::list<int64_t>>(std::get<4>(r));
        return out;
    }
};

struct DraftView : draft::Project {
    project::Project& p;                                     // (not `Project`: that is draft::Project here, the base)
    explicit DraftView(project::Project& p_) : p(p_) {}

    str toolchain_name() override { return p.toolchain()->name; }
    std::optional<str> inverse_model() override { return profile_parts(*p.toolchain()).inverse; }
    draft::Located locate() override {
        draft::Located out;
        auto lib = as_library(p);
        library::MapP mp = library::locate(*lib);
        auto conv = [](const library::MemberP& m) {
            draft::LibModule x;
            py::dict<str, int64_t> ids;                          // (a segment's number: its place among the member's segments)
            for (int64_t i = 0; i < m->segments.size(); ++i) {
                if (m->segments[i]) ids.setdefault(*m->segments[i], i);
            }
            auto id = [&](const std::optional<str>& s) { return s && ids.contains(*s) ? ids.at(*s) : int64_t(-1); };
            for (const auto& kv : m->pubs.items()) x.pubs.set(std::get<0>(kv), std::make_tuple(id(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))));
            x.first_code_segment = m->code.empty() ? int64_t(-1) : id(std::optional<str>(std::get<0>(m->code[0])));
            return x;
        };
        auto place = [&](const library::PlacedP& pl) {
            draft::LibPlace lp;
            lp.code = pl->code ? *pl->code : 0;
            lp.m = conv(pl->m);
            for (const auto& a : pl->also) lp.also.append(conv(a));
            return lp;
        };
        for (const auto& pl : mp->coded) out.coded.append(place(pl));
        for (const auto& pl : mp->lone) out.lone.append(place(pl));
        return out;
    }
    py::dict<str, std::tuple<str, int64_t>> symbols() override { return p.symbols(); }
    py::bytes image() override { return p.image; }
    py::set<int64_t> source_addresses() override {
        py::set<int64_t> s;
        for (const auto& x : p.sources()) s.add(std::get<0>(x));
        return s;
    }
    str path(const str& name) override { return p.path(name); }
    py::list<std::tuple<int64_t, int64_t, str>> functions() override { return carve::functions(p); }

    std::shared_ptr<draft::InverseModel> load_inverse_model(const str& model) override {
        if (model == str("early_c")) return std::make_shared<EarlyCModel>();
        throw py::ModuleNotFoundError("No module named 'codebreaker.invert." + model.utf8() + "'");
    }

    // draft.prove.prove_texts(prj, tc, inv, texts, a, ln, names)
    std::tuple<str, str> prove_texts(draft::InverseModel& inv, const py::list<std::tuple<str, py::dict<str, int64_t>>>& texts, int64_t a, int64_t ln,
                                     const py::dict<int64_t, str>& names) override {
        BatchToolchain tc(p.toolchain());
        draft::prove::Inv checker;                                // (the model's checker, and `getattr(inv, "LAST", {}).get("regparam")`)
        checker.has_mark = inv.has_mark;
        checker.checked_c = [&inv](const py::bytes& code) { return inv.checked_c(code); };
        checker.last_regparam = inv.LAST.regparam.copy();
        draft::prove::Texts ts;
        for (const auto& t : texts) ts.append(std::make_tuple(std::get<0>(t), std::optional<py::dict<str, int64_t>>(std::get<1>(t))));
        return draft::prove::prove_texts(draft_prj(p), tc, checker, ts, a, ln, names);
    }
};

}  // namespace

std::shared_ptr<draft::Project> as_draft(Project& p) { return std::make_shared<DraftView>(p); }

draft::forms::Prj draft_prj(Project& p) {
    namespace forms = draft::forms;
    forms::Prj j;
    std::shared_ptr<toolchain::Toolchain> tc = p.toolchain();
    const ProfileParts& pp = profile_parts(*tc);
    py::V d = profile_json(*tc);
    Project* pr = &p;

    j.dir = p.dir;
    j.toolchain_name = tc->name;
    j.kit = pp.kit;
    j.kit_dir = [](const str& name) { return toolchain::kit_dir(name); };
    // library.locate(prj), read as forms.data_macros reads it: the library's data places, and the placed modules' own
    // data names (a table the macros read in line, which no code calls)
    j.library_places = [pr]() {
        py::dict<str, int64_t> places;
        library::MapP mp = library::locate(*as_library(*pr));
        for (const auto& kv : mp->allrefs.items()) {
            if (std::get<0>(std::get<1>(kv)) == str("D")) places.set(std::get<0>(kv), std::get<1>(std::get<1>(kv)));
        }
        for (const library::PlacedP& pl : mp->coded + mp->donly) {
            if (!pl->data) continue;
            for (const auto& pub : pl->m->o.pubs) {
                const std::optional<str>& s = std::get<1>(pub);
                if (s) {
                    str up = s->upper();
                    if (up == str("_DATA") || up == str("DATA") || up == str("CONST")) places.setdefault(std::get<0>(pub), *pl->data + std::get<2>(pub));
                }
            }
        }
        return places;
    };

    // the profile's entries
    const py::V& fo = d.get("frame_order");
    if (py::truthy(fo)) {
        forms::FrameRule r;
        if (fo.has("mod")) r.mod = fo.get("mod").as_int();
        j.frame_order = r;
    }
    if (d.get("table_macros").is_seq()) {
        for (const py::V& m : d.get("table_macros").seq()) {
            forms::TableMacro t;
            t.header = vstr(m.get("header"));
            j.table_macros.append(t);
        }
    }
    if (d.get("data_macros").is_seq()) {
        for (const py::V& m : d.get("data_macros").seq()) {
            forms::DataMacro x;
            x.base = vstr(m.get("base"));
            x.size = m.get("size").as_int();
            x.names = str_list(m.get("names"));
            if (m.get("fields").is_dict()) {
                for (const auto& kv : m.get("fields").items()) x.fields.set(vstr(kv.first), vstr(kv.second));
            }
            x.include = opt_str(m, "include");
            j.data_macros.append(x);
        }
    }
    if (d.get("macros").is_seq()) {
        for (const py::V& m : d.get("macros").seq()) {
            forms::LibMacro x;
            x.call = vstr(m.get("call"));
            x.as = vstr(m.get("as"));
            x.type = opt_str(m, "type");
            x.include = opt_str(m, "include");
            if (m.get("byte_args").is_seq()) {
                for (const py::V& b : m.get("byte_args").seq()) x.byte_args.append(b.as_int());
            }
            j.macros.append(x);
        }
    }
    j.fp_conversions = str_list(d.get("fp_conversions"));
    if (d.get("long_helpers").is_dict()) {
        for (const auto& kv : d.get("long_helpers").items()) j.long_helpers.set(vstr(kv.first), vstr(kv.second));
    }
    j.carve_register_variables = py::list<str>{str("si"), str("di")};
    if (d.get("carve").is_dict() && d.get("carve").has("register_variables")) j.carve_register_variables = str_list(d.get("carve").get("register_variables"));
    j.fp_switch = opt_str(d, "fp_switch");
    j.file_switches = opt_str(d, "file_switches");
    j.probe_switch = opt_str(d, "probe_switch");
    j.tc_switches = opt_str(d, "switches");

    // the project
    j.switches = p.switches;
    j.include = p.include;
    j.headers = [pr]() { return pr->headers(); };
    j.image = p.image;
    j.dgroup = p.dgroup;
    j.symbols = p.symbols();
    j.cmp = compare_project(p);
    j.decode = [](const py::bytes& code, int64_t address) { return decoder16()(code, address); };
    // tc.compile(source, headers=, include=, switches=, listing=True) -> (object, console, listing text)
    j.compile_listing = [pr, tc](const py::bytes& src) { return tc->compile_listing(src, pr->headers(), pr->include, pr->switches); };
    return j;
}

}  // namespace codebreaker::exact::project
