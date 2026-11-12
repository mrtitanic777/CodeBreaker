// codebreaker/exact/project.cpp -- typed C++ of codebreaker.exact.project (see project.hpp).

#include "codebreaker/exact/project.hpp"

#include <algorithm>
#include <map>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/exact/project.hpp"
#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/mspascal.hpp"
#include "json.hpp"
#include "py_json.hpp"
#include "value.hpp"

namespace codebreaker::exact::project {

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

str S(const std::string& s) { return str(s); }

// ---------------------------------------------------------------------------------------------- json -> Config

str vstr(const py::V& v) { return str(v.str()); }

Num vnum(const py::V& v) {
    if (v.is_str()) return Num(vstr(v));
    return Num(v.as_int());
}

std::optional<str> opt_str(const py::V& d, const char* key) {
    const py::V& v = d.get(key);
    if (v.is_none()) return std::nullopt;
    if (!v.is_str()) throw py::TypeError(std::string("project.json: \"") + key + "\" is not a string");
    return vstr(v);
}

py::list<str> str_list(const py::V& d, const char* key) {
    py::list<str> out;
    const py::V& v = d.get(key);
    if (v.is_seq()) {
        for (const py::V& x : v.seq()) out.append(vstr(x));
    }
    return out;
}

py::list<Num> num_list(const py::V& v) {
    py::list<Num> out;
    if (v.is_seq()) {
        for (const py::V& x : v.seq()) out.append(vnum(x));
    }
    return out;
}

py::list<library::LibRef> lib_list(const py::V& v) {
    py::list<library::LibRef> out;
    if (v.is_seq()) {
        for (const py::V& x : v.seq()) {
            library::LibRef r;
            r.tag = x.has("tag") ? vstr(x.get("tag")) : str("");
            if (!x.has("path")) throw py::KeyError("'path'");
            r.path = vstr(x.get("path"));
            out.append(r);
        }
    }
    return out;
}

py::dict<str, int64_t> int_dict(const py::V& v) {
    py::dict<str, int64_t> out;
    if (v.is_dict()) {
        for (const auto& kv : v.items()) out.set(vstr(kv.first), kv.second.as_int());
    }
    return out;
}

bool vtruth(const py::V& v) { return py::truthy(v); }

pascal_join::Config pascal_config(const py::V& d) {
    pascal_join::Config c;
    if (d.has("root")) c.root = vstr(d.get("root"));
    c.over = str_list(d, "over");
    c.map = opt_str(d, "map");
    c.kinds = opt_str(d, "kinds");
    c.procedures = opt_str(d, "procedures");
    c.units = int_dict(d.get("units"));
    c.equivalent = opt_str(d, "equivalent");
    const py::V& cs = d.get("compilands");
    if (cs.is_dict()) {
        for (const auto& kv : cs.items()) {
            const py::V& u = kv.second;
            pascal_join::CompilandCfg x;
            x.dir = u.has("dir") ? vstr(u.get("dir")) : str("");
            x.kind = u.has("kind") ? vstr(u.get("kind")) : str("");
            if (u.has("main")) x.main = vnum(u.get("main"));
            if (u.has("parts")) x.parts = u.get("parts").as_int();
            if (u.has("over")) x.over = str_list(u, "over");
            if (u.has("dirs")) x.dirs = str_list(u, "dirs");
            if (!u.get("unit").is_none()) x.unit = u.get("unit").as_int();
            x.stackck = vtruth(u.get("stackck"));
            x.equivalent = opt_str(u, "equivalent");
            c.compilands.set(vstr(kv.first), x);
        }
    }
    return c;
}

Config from_value(const py::V& d) {
    Config c;
    if (!d.has("image")) throw py::KeyError("'image'");
    c.image = vstr(d.get("image"));
    if (d.has("src")) c.src = vstr(d.get("src"));
    c.include = str_list(d, "include");
    if (d.has("dgroup")) c.dgroup = d.get("dgroup").is_none() ? Num(int64_t(0)) : vnum(d.get("dgroup"));
    if (d.has("code_segments")) {
        const py::V& cs = d.get("code_segments");
        c.has_code_segments = true;
        if (cs.is_str()) c.code_segments_text = vstr(cs);
        else c.code_segments = num_list(cs);
    } else {
        c.code_segments.append(Num(int64_t(0)));
    }
    if (d.has("loadseg")) c.loadseg = vnum(d.get("loadseg"));
    if (d.has("code_end")) c.code_end = vnum(d.get("code_end"));
    if (d.has("code_start")) c.code_start = vnum(d.get("code_start"));
    if (d.has("data_end")) c.data_end = vnum(d.get("data_end"));
    if (d.has("code")) c.code = vstr(d.get("code"));
    c.switches = opt_str(d, "switches");
    c.starts = opt_str(d, "starts");
    c.starts_add = num_list(d.get("starts_add"));
    c.starts_drop = num_list(d.get("starts_drop"));
    c.packed = opt_str(d, "packed");
    if (d.has("sources")) c.sources = vstr(d.get("sources"));
    c.place_by = opt_str(d, "place_by");
    c.unplaced = opt_str(d, "unplaced");
    c.names = opt_str(d, "names");
    c.named_by = opt_str(d, "named_by");
    c.first_segment = opt_str(d, "first_segment");
    c.toolchain = opt_str(d, "toolchain");
    c.variant = opt_str(d, "variant");
    if (d.has("toolchains")) {
        c.has_toolchains = true;
        for (const py::V& x : d.get("toolchains").seq()) {
            ToolchainSpec t;
            t.label = opt_str(x, "label");
            if (!x.has("toolchain")) throw py::KeyError("'toolchain'");
            t.toolchain = vstr(x.get("toolchain"));
            t.variant = opt_str(x, "variant");
            c.toolchains.append(t);
        }
    }
    c.whole = vtruth(d.get("whole"));
    c.recut = vtruth(d.get("recut"));
    c.link_order = opt_str(d, "link_order");
    if (d.has("model")) c.model = vstr(d.get("model"));
    if (d.has("library_gap")) c.library_gap = d.get("library_gap").as_int();
    if (d.has("library_start") && !d.get("library_start").is_none()) c.library_start = vnum(d.get("library_start"));
    c.libraries = lib_list(d.get("libraries"));
    c.first_objects = str_list(d, "first_objects");
    c.after_library = str_list(d, "after_library");
    c.data_sizes = int_dict(d.get("data_sizes"));
    c.data_pads = int_dict(d.get("data_pads"));
    if (d.get("also").is_seq()) {
        for (const py::V& x : d.get("also").seq()) c.also.append(str(py::json_dump(x)));
    }
    if (d.get("link").is_dict()) c.link = linkwhole::link_config(str(py::json_dump(d.get("link"))));
    c.pascal_config = pascal_config(d);
    c.text = str(py::json_dump(d));
    return c;
}

// open(p, "r").read(), the locale's encoding as the Python's open()
str read_text(const str& p) {
    py::file f = py::open(p);
    str t = f.read_text();
    f.close();
    return t;
}

// a path as the old layer's strings had it (os.path: backslashes)
str native(const std::filesystem::path& p) {
    std::string t = cb::fs::to_text(p);
    std::replace(t.begin(), t.end(), '/', '\\');
    return S(t);
}

// the old layer's SYMBOLS (name -> (kind letter, address)) and the new layer's
Symbols symbols_to_py(const cb::exact::SymbolTable& t) {
    Symbols out;
    for (const auto& kv : t) out.set(S(kv.first), std::make_tuple(S(std::string(1, cb::exact::sym_kind_letter(kv.second.kind))), kv.second.address));
    return out;
}
cb::exact::SymbolTable symbols_from_py(const Symbols& s) {
    cb::exact::SymbolTable out;
    for (const auto& kv : s.items()) {
        const std::string k = std::get<0>(std::get<1>(kv)).utf8();
        cb::exact::SymKind kind = k == "C" ? cb::exact::SymKind::Code : k == "D" ? cb::exact::SymKind::Data : k == "S" ? cb::exact::SymKind::Segment : cb::exact::SymKind::Unknown;
        out.insert_or_assign(std::get<0>(kv).utf8(), cb::exact::SymPlace{kind, std::get<1>(std::get<1>(kv))});
    }
    return out;
}
py::bytes bytes_of(cb::ByteView v) { return py::bytes(std::string(cb::as_text(v))); }

compare::Decoder& decoder_slot() {
    static compare::Decoder d;
    return d;
}
ProveTexts& prove_slot() {
    static ProveTexts f;
    return f;
}

// ---------------------------------------------------------------------------------------------- the adapters

// what the adapters read of a project, and the sets other than the project itself
template <class Base>
struct ViewOf : Base {
    Project& p;
    std::shared_ptr<Project> own;      // (a set the adapter keeps alive)
    explicit ViewOf(Project& p_, std::shared_ptr<Project> own_ = nullptr) : p(p_), own(std::move(own_)) {}
};

// modules::Project's members, over any Base that derives from it
template <class Base>
struct ModulesImpl : ViewOf<Base> {
    using ViewOf<Base>::ViewOf;
    using ViewOf<Base>::p;
    str src() const override { return p.src; }
    str path(const str& name) const override { return p.path(name); }
    int64_t code_end() const override { return p.code_end; }
    int64_t own_end() const override { return p.own_end(); }
    std::optional<str> switches() const override { return p.switches; }
    std::optional<str> cfg_str(const str& key) const override {
        auto j = p.cfg_json(key);
        if (!j) return std::nullopt;
        py::V v = py::json_parse(j->utf8());
        if (!v.is_str()) return std::nullopt;
        return vstr(v);
    }
    py::list<int64_t> code_segments() const override { return p.code_segments; }
    std::shared_ptr<modules::Toolchain> toolchain() const override { return p.toolchain(); }
    py::dict<str, bytes> headers() const override { return p.headers(); }
    py::list<str> include() const override { return p.include; }
    py::list<std::shared_ptr<const modules::Project>> also_sets() const override;
    bytes image() const override { return p.image; }
    Symbols symbols() const override { return p.symbols(); }
    int64_t dgroup() const override { return p.dgroup; }
    int64_t loadseg() const override { return p.loadseg; }
    int64_t code_base(int64_t a) const override { return p.code_base(a); }
    const py::set<int64_t>& relocs() const override { return p.relocs(); }
};

struct ModulesView : ModulesImpl<modules::Project> {
    using ModulesImpl<modules::Project>::ModulesImpl;
};

template <class Base>
py::list<std::shared_ptr<const modules::Project>> ModulesImpl<Base>::also_sets() const {
    py::list<std::shared_ptr<const modules::Project>> out;
    py::list<std::shared_ptr<Project>> all = p.sets();
    for (int64_t k = 1; k < all.size(); ++k) out.append(std::make_shared<ModulesView>(*all[k], all[k]));
    return out;
}

struct DataView : ModulesImpl<data::Project> {
    using ModulesImpl<data::Project>::ModulesImpl;
    str data_order() const override { return profile_parts(*p.toolchain()).data_order; }
};

struct LibraryView : ModulesImpl<library::Project> {
    using ModulesImpl<library::Project>::ModulesImpl;
    str model() const override { return p.cfg.model; }
    py::list<library::LibRef> profile_libraries(const str& model) const override {
        const ProfileParts& pp = profile_parts(*p.toolchain());
        return pp.libraries.contains(model) ? pp.libraries.at(model) : py::list<library::LibRef>();
    }
    py::list<str> profile_objects(const str& model) const override {
        const ProfileParts& pp = profile_parts(*p.toolchain());
        return pp.objects.contains(model) ? pp.objects.at(model) : py::list<str>();
    }
    std::optional<str> profile_kit() const override { return profile_parts(*p.toolchain()).kit; }
    py::list<library::LibRef> project_libraries() const override { return p.cfg.libraries; }
    py::list<str> first_objects() const override { return p.cfg.first_objects; }
    int64_t library_gap() const override { return p.cfg.library_gap; }
    std::optional<std::variant<int64_t, str>> library_start() const override {
        if (!p.cfg.library_start) return std::nullopt;
        return *p.cfg.library_start;
    }
    py::dict<str, int64_t> data_sizes() const override { return p.cfg.data_sizes; }
    py::dict<str, int64_t> data_pads() const override { return p.cfg.data_pads; }
    str dir() const override { return p.dir; }
};

struct LedgerView : ViewOf<identify::Project> {
    using ViewOf<identify::Project>::ViewOf;
    str src() const override { return p.src; }
    str dir() const override { return p.dir; }
    str exe() const override { return p.exe; }
    str path(const str& name) const override { return p.path(name); }
    std::optional<str> cfg_str(const str& key) const override {
        auto j = p.cfg_json(key);
        if (!j) return std::nullopt;
        py::V v = py::json_parse(j->utf8());
        if (!v.is_str()) return std::nullopt;
        return vstr(v);
    }
    bool has_compilands() const override { return !p.cfg.pascal_config.compilands.empty(); }
    pascal_join::Project pascal_project() const override { return project::pascal_project(p); }
    py::dict<str, bytes> headers() const override { return p.headers(); }
    py::list<str> include() const override { return p.include; }
    std::optional<str> switches() const override { return p.switches; }
    py::list<int64_t> starts() const override {
        auto s = p.starts();
        return s ? *s : py::list<int64_t>();
    }
    bool whole(const str& path) const override { return p.whole(path); }
    bytes image() const override { return p.image; }
    int64_t code_end() const override { return p.code_end; }
    py::list<std::tuple<str, std::shared_ptr<ledger::Toolchain>>> toolchains() const override {
        py::list<std::tuple<str, std::shared_ptr<ledger::Toolchain>>> out;
        for (const auto& t : p.toolchains()) out.append(std::make_tuple(std::get<0>(t), std::shared_ptr<ledger::Toolchain>(std::get<1>(t))));
        return out;
    }
    Symbols symbols() const override { return p.symbols(); }
    void save_symbols(const Symbols& s) const override { p.save_symbols(s); }
    py::list<std::tuple<std::optional<int64_t>, str>> sources() const override {
        py::list<std::tuple<std::optional<int64_t>, str>> out;
        for (const auto& s : p.sources()) out.append(std::make_tuple(std::optional<int64_t>(std::get<0>(s)), std::get<1>(s)));
        return out;
    }
    py::list<std::shared_ptr<const ledger::Project>> sets() const override {
        py::list<std::shared_ptr<const ledger::Project>> out;
        py::list<std::shared_ptr<project::Project>> all = p.sets();
        for (int64_t k = 0; k < all.size(); ++k) out.append(std::make_shared<LedgerView>(*all[k], all[k]));
        return out;
    }
    int64_t dgroup() const override { return p.dgroup; }
    int64_t loadseg() const override { return p.loadseg; }
    int64_t code_base(int64_t a) const override { return p.code_base(a); }
    const py::set<int64_t>& relocs() const override { return p.relocs(); }
    compare::Decoder decoder() const override { return decoder16(); }
    void set_switches(const std::optional<str>& sw) override { p.switches = sw; }
};

struct StandinView : ViewOf<standin::Project> {
    using ViewOf<standin::Project>::ViewOf;
    bytes image() const override { return p.image; }
    Symbols symbols() const override { return p.symbols(); }
    int64_t loadseg() const override { return p.loadseg; }
    int64_t dgroup() const override { return p.dgroup; }
    int64_t code_base(int64_t a) const override { return p.code_base(a); }
    const py::set<int64_t>& relocs() const override { return p.relocs(); }
    std::optional<py::list<int64_t>> packed_relocs() const override {
        if (!p.packed) return std::nullopt;
        return p.packed->relocs;
    }
};

struct RecutView : ViewOf<recut::Project> {
    using ViewOf<recut::Project>::ViewOf;
    compare::Symbols symbols() const override { return p.symbols(); }
    std::optional<py::list<int64_t>> packed_relocs() const override {
        if (!p.packed) return std::nullopt;
        return p.packed->relocs;
    }
    py::list<int64_t> header() const override { return p.header; }
    bytes file() const override { return p.file; }
};

struct LinkwholeView : ViewOf<linkwhole::Project> {
    using ViewOf<linkwhole::Project>::ViewOf;
    str dir() const override { return p.dir; }
    str src() const override { return p.src; }
    py::list<std::tuple<int64_t, str>> sources() const override { return p.sources(); }
    std::shared_ptr<modules::Toolchain> toolchain() const override { return p.toolchain(); }
    py::dict<str, bytes> headers() const override { return p.headers(); }
    py::list<str> include() const override { return p.include; }
    std::optional<str> switches() const override { return p.switches; }
    py::list<std::shared_ptr<const linkwhole::Project>> sets() const override {
        py::list<std::shared_ptr<const linkwhole::Project>> out;
        py::list<std::shared_ptr<project::Project>> all = p.sets();
        for (int64_t k = 0; k < all.size(); ++k) out.append(std::make_shared<LinkwholeView>(*all[k], all[k]));
        return out;
    }
    linkwhole::LinkConfig link() const override { return p.cfg.link; }
    bytes image() const override { return p.image; }
    bytes file() const override { return p.file; }
    str exe() const override { return p.exe; }
    linkemu::Symbols symbols() const override { return p.symbols(); }
};

struct ReadableView : ViewOf<readable::Project> {
    using ViewOf<readable::Project>::ViewOf;
    str dir() const override { return p.dir; }
    str src() const override { return p.src; }
    readable::Symbols symbols() const override { return p.symbols(); }
    py::list<std::tuple<int64_t, str>> sources() const override { return p.sources(); }
    py::list<readable::Row> ledger_run() const override {
        py::list<readable::Row> out;
        auto lv = std::make_shared<LedgerView>(p);
        for (const auto& r : ledger::run(*lv)) {
            readable::Row x;
            x.file = std::get<0>(r);
            x.name = std::get<1>(r) ? *std::get<1>(r) : str("");
            x.address = std::get<2>(r);
            x.length = std::get<3>(r);
            x.verdict = std::get<4>(r);
            x.notes = std::get<5>(r);
            for (const auto& b : std::get<6>(r)) x.by.append(b ? *b : str(""));
            out.append(x);
        }
        return out;
    }
    std::unique_ptr<readable::Project> with_src(const str& src_) const override {
        std::string o;
        o += "{\"src\": ";
        py::json::put(o, src_);
        o += "}";
        auto np = std::make_shared<project::Project>(p.dir, std::optional<str>(str(o)));
        auto v = std::make_unique<ReadableView>(*np, np);
        return v;
    }
};

struct DraftView : ViewOf<draft::Project> {
    using ViewOf<draft::Project>::ViewOf;
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
    bytes image() override { return p.image; }
    py::set<int64_t> source_addresses() override {
        py::set<int64_t> s;
        for (const auto& x : p.sources()) s.add(std::get<0>(x));
        return s;
    }
    str path(const str& name) override { return p.path(name); }
    py::list<std::tuple<int64_t, int64_t, str>> functions() override { return carve::functions(p); }
    std::tuple<str, str> prove_texts(draft::InverseModel& inv, const py::list<std::tuple<str, py::dict<str, int64_t>>>& texts, int64_t a, int64_t ln,
                                     const py::dict<int64_t, str>& names) override {
        if (!prove_slot()) throw py::ModuleNotFoundError("No module named 'codebreaker.exact.draft.prove' (no typed port: set_prove_texts)");
        return prove_slot()(inv, texts, a, ln, names);
    }
};

}  // namespace

// ---------------------------------------------------------------------------------------------- Num, Config

int64_t num(const Num& x) {
    if (std::holds_alternative<str>(x)) return py::int_parse(std::get<str>(x), 0);
    return std::get<int64_t>(x);
}

Config Config::from_json(const str& json_text) { return from_value(py::json_parse(json_text.utf8())); }

// ---------------------------------------------------------------------------------------------- the profile's parts

void set_decoder16(const compare::Decoder& d) { decoder_slot() = d; }
const compare::Decoder& decoder16() {
    if (!decoder_slot()) throw py::RuntimeError("no 16-bit decoder installed (project::set_decoder16)");
    return decoder_slot();
}
void set_prove_texts(const ProveTexts& f) { prove_slot() = f; }

const ProfileParts& profile_parts(const toolchain::Toolchain& tc) {
    static std::map<std::string, std::shared_ptr<ProfileParts>> cache;
    std::string key = tc.p.name.utf8();
    auto it = cache.find(key);
    if (it != cache.end()) return *it->second;
    str file = py::os::path::join(py::os::path::join(toolchain::HERE(), str("toolchains")), tc.p.name + str(".json"));
    py::V d = py::json_parse(read_text(file).utf8());
    auto pp = std::make_shared<ProfileParts>();
    pp->name = tc.p.name;
    pp->kit = opt_str(d, "kit");
    const py::V& cv = d.get("carve");
    if (cv.is_dict()) {
        pp->carve.prologues = str_list(cv, "prologues");
        pp->carve.prologue_call = opt_str(cv, "prologue_call");
        if (cv.has("frames")) pp->carve.frames = vtruth(cv.get("frames"));
        if (cv.has("calls")) pp->carve.calls = vtruth(cv.get("calls"));
        if (cv.has("emulator_ints")) pp->carve.emulator_ints = vtruth(cv.get("emulator_ints"));
    }
    const py::V& lv = d.get("link");
    if (vtruth(lv)) {
        LinkSpec l;
        l.kit = opt_str(lv, "kit");
        l.run = opt_str(lv, "run");
        l.dir = opt_str(lv, "dir");
        if (lv.get("libs").is_dict()) {
            for (const auto& kv : lv.get("libs").items()) {
                py::list<str> fs;
                for (const py::V& x : kv.second.seq()) fs.append(vstr(x));
                l.libs.set(vstr(kv.first), fs);
            }
        }
        l.copy = str_list(lv, "copy");
        l.exemod = opt_str(lv, "exemod");
        if (lv.has("psp")) l.psp = vnum(lv.get("psp"));
        if (lv.has("names")) l.names = str_list(lv, "names");
        pp->link = l;
    }
    const py::V& cm = d.get("communal");
    if (vtruth(cm)) {
        Communal c;
        if (!cm.has("segment")) throw py::KeyError("'segment'");
        c.segment = vstr(cm.get("segment"));
        c.para = cm.has("align") && cm.get("align") == "para";
        pp->communal = c;
    }
    if (d.has("headers")) pp->headers = str_list(d, "headers");
    if (d.get("libraries").is_dict()) {
        for (const auto& kv : d.get("libraries").items()) pp->libraries.set(vstr(kv.first), lib_list(kv.second));
    }
    if (d.get("objects").is_dict()) {
        for (const auto& kv : d.get("objects").items()) {
            py::list<str> fs;
            for (const py::V& x : kv.second.seq()) fs.append(vstr(x));
            pp->objects.set(vstr(kv.first), fs);
        }
    }
    pp->inverse = opt_str(d, "inverse");
    if (d.get("data").is_dict() && d.get("data").has("order")) pp->data_order = vstr(d.get("data").get("order"));
    cache[key] = pp;
    return *pp;
}

// ---------------------------------------------------------------------------------------------- Project

Project::Project(const str& folder, const std::optional<str>& override_json) {
    dir = py::os::path::abspath(folder);
    py::V d = py::json_parse(read_text(py::os::path::join(dir, str("project.json"))).utf8());
    if (override_json) {                                    // (a set of the project's sources: its own folder, pattern, toolchain)
        py::V nd = py::copy_dict(d);
        nd.erase(py::V("also"));
        py::V o = py::json_parse(override_json->utf8());
        for (const auto& kv : o.items()) nd.set(kv.first, kv.second);
        d = nd;
    }
    cfg = from_value(d);
    const Config& c = cfg;
    // the logic: the new layer's project (the program's header, relocations and image, the code segments, DGROUP)
    impl_ = cb::exact::project::Project::open(std::filesystem::path(dir.utf8()), nullptr, override_json ? std::optional<std::string>(override_json->utf8()) : std::nullopt);
    const cb::exact::project::Project& ip = *impl_;
    exe = native(ip.exe());
    src = native(ip.src());
    for (const auto& x : ip.include()) include.append(native(x));
    dgroup = ip.dgroup();
    code_segments = py::list<int64_t>();
    for (uint32_t x : ip.code_segments()) code_segments.append(x);
    loadseg = ip.loadseg();
    code_end = ip.code_end();
    switches = c.switches;
    file = bytes_of(ip.file());
    image = bytes_of(ip.image());
    if (ip.header()) {
        for (uint16_t w : *ip.header()) header.append(w);
    }
    {
        py::set<int64_t> r;
        for (uint32_t a : ip.relocs()) r.add(a);
        relocs_ = r;
    }
    if (c.packed && !c.packed->empty()) {                   // (the original as EXEPACK left it)
        py::file f = py::open_rb(py::os::path::normpath(py::os::path::join(dir, *c.packed)));
        packed_file = f.read();
        f.close();
        packed = exepack::read(packed_file);
    }
}

const py::set<int64_t>& Project::relocs() const { return *relocs_; }

py::list<std::shared_ptr<Project>> Project::sets() {
    py::list<std::shared_ptr<Project>> out;
    out.append(std::shared_ptr<Project>(this, [](Project*) {}));
    for (const str& x : cfg.also) out.append(std::make_shared<Project>(dir, std::optional<str>(x)));
    return out;
}

int64_t Project::code_base(int64_t a) const {
    int64_t best = 0;
    bool any = false;
    for (int64_t b : code_segments) {
        if (b <= a && (!any || b > best)) {
            best = b;
            any = true;
        }
    }
    return any ? best : 0;
}

int64_t Project::own_end() {
    if (!own_end_) {
        auto fs = carve::functions(*this);
        int64_t m = 0;
        bool any = false;
        for (const auto& f : fs) {
            int64_t e = std::get<0>(f) + std::get<1>(f);
            if (!any || e > m) m = e;
            any = true;
        }
        own_end_ = any ? m : 0;
    }
    return *own_end_;
}

std::optional<py::list<int64_t>> Project::starts() const {
    auto v = impl_->starts();
    if (!v) return std::nullopt;
    py::list<int64_t> out;
    for (uint32_t x : *v) out.append(x);
    return out;
}

py::list<std::tuple<str, std::shared_ptr<toolchain::Toolchain>>> Project::toolchains() const {
    py::list<std::tuple<str, std::shared_ptr<toolchain::Toolchain>>> out;
    if (cfg.has_toolchains) {
        for (const ToolchainSpec& x : cfg.toolchains) {
            str label = x.label && !x.label->empty() ? *x.label : x.toolchain;
            out.append(std::make_tuple(label, toolchain::load(x.toolchain, x.variant)));
        }
        return out;
    }
    if (!cfg.toolchain) throw py::KeyError("'toolchain'");
    out.append(std::make_tuple(*cfg.toolchain, toolchain::load(*cfg.toolchain, cfg.variant)));
    return out;
}

std::shared_ptr<toolchain::Toolchain> Project::toolchain() const { return std::get<1>(toolchains()[0]); }

str Project::path(const str& name) const { return py::os::path::join(src, name); }

Symbols Project::symbols() const { return symbols_to_py(impl_->symbols()); }

void Project::save_symbols(const Symbols& s) const { impl_->save_symbols(symbols_from_py(s)); }

py::list<std::tuple<int64_t, str>> Project::sources() const {
    py::list<std::tuple<int64_t, str>> out;
    for (const auto& x : impl_->sources()) out.append(std::make_tuple(x.at, native(x.path)));
    return out;
}

bool Project::whole(const str& path_) const { return impl_->whole(std::filesystem::path(path_.utf8())); }

py::dict<str, bytes> Project::headers() const {
    const ProfileParts& pp = profile_parts(*toolchain());
    std::vector<std::string> exts;
    for (const str& e : pp.headers) exts.push_back(e.utf8());
    py::dict<str, bytes> out;
    for (const auto& kv : impl_->headers_with(exts)) out.set(S(kv.first), bytes_of(kv.second));
    return out;
}

std::optional<str> Project::cfg_json(const str& key) const {
    py::V d = py::json_parse(cfg.text.utf8());
    if (!d.has(key.utf8().c_str())) return std::nullopt;
    return str(py::json_dump(d.get(key.utf8().c_str())));
}

void Project::set_project_entry(const str& key, const str& sub, int64_t value) {
    str pj = py::os::path::join(dir, str("project.json"));
    py::V d = py::json_parse(read_text(pj).utf8());
    std::string k = key.utf8();
    if (!d.has(k.c_str())) d.set(k.c_str(), py::V::dict());
    d.get(k.c_str()).set(sub.utf8().c_str(), py::V((long long)value));
    py::DumpOpts o;
    o.indent = 1;
    py::file f = py::open(pj, str("w"));
    f.write(str(py::json_dump(d, o)));
    f.close();
    cfg = from_value(d);
}

void Project::forget_library() {
    if (library_view_) library_view_->_library = nullptr;   // prj._library = None
}

std::shared_ptr<Project> load(const str& folder) { return std::make_shared<Project>(folder); }

// ---------------------------------------------------------------------------------------------- the adapters

std::shared_ptr<modules::Project> as_modules(Project& p) { return std::make_shared<ModulesView>(p); }
std::shared_ptr<library::Project> as_library(Project& p) {
    if (!p.library_view_) p.library_view_ = std::make_shared<LibraryView>(p);
    return p.library_view_;
}
std::shared_ptr<data::Project> as_data(Project& p) { return std::make_shared<DataView>(p); }
std::shared_ptr<ledger::Project> as_ledger(Project& p) { return std::make_shared<LedgerView>(p); }
std::shared_ptr<identify::Project> as_identify(Project& p) { return std::make_shared<LedgerView>(p); }
std::shared_ptr<standin::Project> as_standin(Project& p) { return std::make_shared<StandinView>(p); }
std::shared_ptr<recut::Project> as_recut(Project& p) { return std::make_shared<RecutView>(p); }
std::shared_ptr<linkwhole::Project> as_linkwhole(Project& p) { return std::make_shared<LinkwholeView>(p); }
std::shared_ptr<readable::Project> as_readable(Project& p) { return std::make_shared<ReadableView>(p); }
std::shared_ptr<draft::Project> as_draft(Project& p) { return std::make_shared<DraftView>(p); }

compare::Project compare_project(Project& p) {
    compare::Project c;
    c.dgroup = p.dgroup;
    c.loadseg = p.loadseg;
    Project* pp = &p;
    c.code_base = [pp](int64_t a) { return pp->code_base(a); };
    c.relocs = [pp]() -> const py::set<int64_t>& { return pp->relocs(); };
    return c;
}

batch::Project batch_project(Project& p) {
    batch::Project b;
    Project* pp = &p;
    b.headers = [pp]() { return pp->headers(); };
    b.include = p.include;
    b.switches = p.switches;
    return b;
}

pascal_join::Project pascal_project(Project& p) {
    pascal_join::Project q;
    q.dir = p.dir;
    q.cfg = p.cfg.pascal_config;
    q.image = p.image;
    Project* pp = &p;
    q.toolchain = [pp]() {
        pascal_join::Toolchain t;
        std::shared_ptr<toolchain::Toolchain> tc = pp->toolchain();
        auto ms = std::dynamic_pointer_cast<mspascal::MSPascal>(tc);
        if (!ms) throw py::AttributeError("the toolchain " + tc->name.utf8() + " has no helper_data (it is not MSPascal)");
        t.helper_data = ms->helper_data;
        t.compile = [ms](const bytes& src_, const str& name) {
            return ms->compile(src_, std::nullopt, py::list<str>(), std::nullopt, true, name);
        };
        return t;
    };
    q.symbols = [pp]() { return pp->symbols(); };
    q.compare = compare_project(p);
    q.decode = decoder_slot();
    return q;
}

}  // namespace codebreaker::exact::project
