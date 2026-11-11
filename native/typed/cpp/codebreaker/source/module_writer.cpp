// codebreaker/source/module_writer.cpp -- typed C++ port of codebreaker.source.module_writer (see module_writer.hpp).

#include "codebreaker/source/module_writer.hpp"

#include <algorithm>
#include <ctime>
#include <set>

#include "codebreaker/analyze/globals.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/strings.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/analyze/image_bridge.hpp"
#include "codebreaker/source/classes_writer.hpp"
#include "codebreaker/source/java_writer.hpp"
#include "codebreaker/source/placement.hpp"
#include "codebreaker/source/readable.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/demangle.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/translate/backends/module_file.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::module_writer {

namespace V = verify::verdicts;
namespace F = store::functions;
namespace mf = translate::backends::module_file;
using engine::native_engine::Candidate;
using engine::native_engine::Cell;

const py::dict<py::str, py::str> WHY = [] {
    py::dict<py::str, py::str> d;
    d.set(py::str("ret-differs"), py::str("returns a different value"));
    d.set(py::str("deps-differ"), py::str("makes different API calls, or passes them different arguments"));
    d.set(py::str("exit-differs"), py::str("ends differently (one side faults or runs away)"));
    d.set(py::str("writes-differ"), py::str("leaves different values in memory"));
    d.set(py::str("stack-differs"), py::str("removes a different number of bytes from the stack (convention or argument count)"));
    d.set(py::str("registers-differ"), py::str("changes a register it must preserve"));
    d.set(py::str("retail: call-into-nothing"), py::str("the original calls through a pointer the loader fills in (a thunk entry)"));
    return d;
}();
const py::list<py::str> ORDER = V::SCORED();
const py::dict<py::str, py::str> TITLE = V::TITLE();

namespace {

py::str cs(const Cell& c) { return engine::native_engine::cell_str(c); }
bool cell_none(const Cell& c) { return std::holds_alternative<std::monostate>(c); }

// settings.state(*parts): a path inside the state folder; the folder of the last part is created if it is not there
py::str state_path(const py::list<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

py::str strftime(const char* fmt) {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof buf, fmt, std::localtime(&t));
    return py::str(buf);
}

// The index of a module's rows by L2 provenance (verify.verdicts.Index over the store's candidates)
class Judged {
  public:
    Judged(const py::str& module, const py::dict<py::str, Candidate>& rows) : module_(module) {
        for (const auto& kv : rows.items()) rows_.set(std::make_tuple(module, std::get<0>(kv)), std::get<1>(kv));
    }
    std::optional<Candidate> at(const V::Origin& origin) const {
        if (!origin.module && !origin.va) return std::nullopt;
        if (!origin.module || !origin.va) return std::nullopt;
        return rows_.get(std::make_tuple(*origin.module, *origin.va));
    }
    V::Origin origin(const py::str& addr) const { return V::Origin{module_, addr}; }

  private:
    py::str module_;
    py::dict<std::tuple<py::str, py::str>, Candidate> rows_;
};

translate::l2::Dict origin_dict(const V::Origin& o) {
    translate::l2::Dict d;
    if (o.module) d.set(py::str("module"), translate::ir::Val(*o.module));
    if (o.va) d.set(py::str("va"), translate::ir::Val(*o.va));
    return d;
}

}  // namespace

py::str describe(const Candidate& row) {
    // The words after the status for one function's best candidate.
    py::list<py::str> limits;
    if (!cell_none(row.limits))
        for (const py::str& c : py::split(cs(row.limits), py::str(","))) if (!c.empty()) limits.append(c);
    std::optional<int64_t> cov;
    for (const py::str& c : limits)
        if (c.startswith(py::str("coverage="))) {
            cov = py::int_parse(c.slice(9, c.size()), 10);
            break;
        }
    py::str cls = cs(row.klass);
    py::str reason = cs(row.reason);
    if (cls == V::VERIFIED) return cov ? py::percent(py::str("%d%% of it exercised"), *cov) : py::str("");
    if (cls == V::WEAK) {
        py::list<py::str> why;
        for (const py::str& c : row.weak) why.append(V::WEAK_LIMITS().at(c));
        return py::join(py::str("; "), why) + (cov ? py::percent(py::str(" (%d%% ran)"), *cov) : py::str(""));
    }
    if (cls == V::UNVERIFIED || cls == V::NOT_JUDGED) return WHY.get(reason, reason);
    if (cls == V::NO_COMPILE) return reason.slice(0, 90);
    return reason.slice(0, 80);
}

py::dict<py::str, py::str> real_import_names(const py::str& path, const py::set<py::str>& used) {
    // {symbol: name} for a 16-bit module's imports by number that can be written by their real name: the name is a C
    // name, no function of the module has it (`used`), and no two imports share it. (A 32-bit module's are already
    // named in the decompiled text.)
    std::optional<py::str> kind = verify::reconstruct::kind_name(path);
    if (kind && *kind == py::str("powerpc")) return console_import_names(path, used);
    std::optional<py::str> lane = base::corpus::lane(path);
    if (!lane || *lane != py::str("ne")) return {};
    py::dict<py::str, py::str> found = decompile::describe::import_names16(formats::ne::NEFile(path));
    py::dict<py::str, int64_t> times;
    for (const py::str& name : found.values()) times.set(name, times.get(name, 0) + 1);
    py::dict<py::str, py::str> out;
    for (const auto& kv : found.items()) {
        const py::str& n = std::get<1>(kv);
        bool is_a_key = found.contains(n);       // `n not in found`: a name that is another import's symbol
        if (times.at(n) == 1 && !used.contains(n) && !is_a_key) out.set(std::get<0>(kv), n);
    }
    return out;
}

py::dict<py::str, py::str> console_import_names(const py::str& path, const py::set<py::str>& used) {
    // {symbol: name} for a PowerPC image's imports. An image names what it imports by library and number only
    // (`import0_ord_16`); the numbers of the console's system libraries are public knowledge
    // (symbols/console_ordinals.json). The libraries' names are in the .xex header, not in the image: for an image
    // read from a .xex they are taken from there (formats/xex.py). An image extracted by other means is taken to have
    // the kernel first and the system library second, the order every image seen has them in -- a call's arguments
    // bear it out (number 16 of the kernel takes a category, a setting, a buffer, its size and a length).
    static const py::re::Pattern import_sym("import(\\d+)_ord_(\\d+)", 0);
    static const py::re::Pattern c_name("[A-Za-z_]\\w*", 0);
    namespace sources = knowledge::sources;
    py::dict<py::str, sources::Json> table = symbols::api_arguments::_knowledge_table(py::str("xbox360"), py::str("ordinal_names"));    // (console_ordinals.json, as the knowledge base has it)
    py::list<py::str> libs;
    auto from_xex = formats::xex::import_libraries(path);
    if (from_xex && std::holds_alternative<std::vector<std::pair<py::str, symbols::native::Json>>>(from_xex->value)) {
        // (a list of names: the header's)
    }
    if (from_xex && std::holds_alternative<symbols::native::Json::Array>(from_xex->value)) {
        for (const auto& item : std::get<symbols::native::Json::Array>(from_xex->value))
            if (std::holds_alternative<py::str>(item.value)) libs.append(std::get<py::str>(item.value));
    }
    if (libs.empty()) {
        libs.append(py::str("xboxkrnl.exe"));
        libs.append(py::str("xam.xex"));
    }
    py::dict<int64_t, sources::Json> libraries;
    for (int64_t i = 0; i < libs.size(); i++) {
        sources::Json none;
        libraries.set(i, table.get(libs[i].lower()).value_or(none));
    }
    py::dict<py::str, py::str> found;
    py::set<py::str> symbols_seen;
    for (const auto& kv : formats::powerpc_image::read(path)->imports.items()) symbols_seen.add(std::get<1>(kv));
    for (const py::str& symbol : symbols_seen) {
        auto m = import_sym.fullmatch(symbol);
        std::optional<py::str> name;
        if (m) {
            auto lib = libraries.get(py::int_parse(m->group(1), 10));
            if (lib && lib->kind == sources::Json::Dict) {
                auto entry = lib->dict.get(m->group(2));
                if (entry && entry->kind == sources::Json::Str) name = entry->s;
            }
        }
        if (name && !name->empty() && c_name.fullmatch(*name)) found.set(symbol, *name);
    }
    py::dict<py::str, int64_t> times;
    for (const py::str& name : found.values()) times.set(name, times.get(name, 0) + 1);
    py::dict<py::str, py::str> out;
    for (const auto& kv : found.items())
        if (times.at(std::get<1>(kv)) == 1 && !used.contains(std::get<1>(kv))) out.set(std::get<0>(kv), std::get<1>(kv));
    return out;
}

namespace {

// A comment block describing the module's global variables, from the globals analysis (codebreaker.analyze.globals):
// for each global the functions refer to by its address, the type recovered from how it is used, its initial value,
// and how many functions read and write it. Nothing is renamed -- the globals keep the macros the decompiler bound to
// their original addresses -- so this only adds information. '' when there is nothing to say.
py::str _globals_note(const py::str& path, const py::str& header) {
    static const py::re::Pattern ram("\\b[a-z]+Ram([0-9a-f]{8})\\b", 0);
    std::set<int64_t> addrs;
    for (const auto& m : ram.finditer(header)) addrs.insert(py::int_parse(m.group(1), 16));
    if (addrs.empty()) return py::str("");
    py::dict<int64_t, analyze::globals::Variable> var;
    try {
        auto img = analyze::image::load(path);
        var = analyze::globals::variables(analyze::bridge::globals_of(*img));
    } catch (const std::exception&) {
        return py::str("");
    }
    py::list<py::str> lines;
    for (int64_t a : addrs) {
        auto g = var.get(a);
        if (!g) continue;
        py::str init;
        if (std::holds_alternative<py::str>(g->initial)) init = std::get<py::str>(g->initial);
        else init = py::percent(py::str("%#x"), std::get<int64_t>(g->initial));
        py::str tail;
        if (init != py::str("") && init != py::str("uninitialised") && init != py::str("?")) tail = py::str("  initially ") + init.slice(0, 24);
        else if (init == py::str("uninitialised")) tail = py::str("  uninitialised");
        lines.append(py::percent(py::str(" *   %08x  %-34s read by %d, written by %d%s"), a, g->type, static_cast<int64_t>(g->readers.size()),
                                 static_cast<int64_t>(g->writers.size()), tail));
    }
    if (lines.empty()) return py::str("");
    return py::percent(py::str("\n/* globals (type recovered from use; still at the original's address):\n%s */\n"), py::join(py::str("\n"), lines));
}

// address -> the module's string there, or None (for naming what GetProcAddress asks for).
source::readable::Readable::StringAt _string_reader(const py::str& path, const std::optional<py::str>& kind) {
    if (!kind || (*kind != py::str("pe") && *kind != py::str("ne"))) return nullptr;
    std::shared_ptr<analyze::image::Image> img;
    try {
        img = analyze::image::load(path);
    } catch (const std::exception&) {
        return nullptr;
    }
    return [img](int64_t address) -> std::optional<py::str> {
        try {
            auto found = analyze::bridge::string_at(*img, address);
            if (found) return std::get<0>(*found);
            return std::nullopt;
        } catch (const std::exception&) {
            return std::nullopt;
        }
    };
}

}  // namespace

ModuleWritten write_assembly(const py::str& path, const FunctionStorePtr& fs, const py::str& sha, const py::str& digest,
                             const py::str& out_dir, const Log& log0) {
    // A driver's or a real-mode program's source: its one assembly file, the status at its head.
    Log log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    py::str name = py::os::path::basename(path).lower();
    py::str stem = std::get<0>(py::os::path::splitext(name));
    Counts counts;
    int64_t total = 0;
    for (const auto& kv : fs->scores(digest, sha).items()) {
        int64_t n = engine::native_engine::cell_int(std::get<1>(kv));
        counts.set(std::get<0>(kv), n);
        total += n;
    }
    py::str text = cs(fs->get_text(sha, verify::reconstruct::SOURCE, py::str("header")));
    auto best = fs->best(sha, digest);
    py::str how;
    for (const auto& kv : best.items()) {
        py::str d = cs(std::get<1>(kv).detail);
        if (!d.empty()) {
            how = d;
            break;
        }
    }
    mf::Facts facts;
    for (const py::str& va : fs->addresses(sha)) {
        auto row = best.get(va);
        if (row && cs(row->klass) != py::str("verified"))
            facts.rows.append(mf::Row{va, cs(row->name), TITLE.at(cs(row->klass)), describe(*row)});
    }
    // the module as one L2 tree (its source, kept as its instructions), the status the store's facts
    mf::backend_asm::Document doc;
    doc.node = py::str("Module");
    py::list<mf::backend_asm::Unit> units;
    if (!text.empty()) {
        mf::backend_asm::Unit u;
        u.node = py::str("MachineFragment");
        u.asm_ = text;
        u.name = stem;
        u.syntax = py::str("masm");
        units.append(u);
    }
    doc.units = units;
    facts.name = name;
    facts.date = strftime("%Y-%m-%d");
    facts.digest = digest;
    facts.order = ORDER;
    facts.titles = TITLE;
    facts.counts = counts;
    facts.total = total;
    facts.how = how;
    py::str out = py::os::path::join(out_dir, stem + py::str(".asm"));
    {
        py::file f = py::open(out, py::str("w"), py::str("latin1"), py::str("replace"), py::str("\r\n"));
        f.write(mf::asm_(doc, facts));
        f.close();
    }
    py::list<py::str> shown;
    for (const py::str& c : ORDER)
        if (counts.get(c, 0)) shown.append(py::percent(py::str("%s %d"), TITLE.at(c), counts.at(c)));
    log(py::percent(py::str("%s: %s"), out, py::join(py::str(", "), shown)));
    return ModuleWritten(out, counts, total);
}

namespace {

const char* const OURS[] = {"reconstructed source", "behavioral reconstruction", "authored-source reconstruction",
                            "(functional group, reconstructed)", "decompiled by CodeBreaker", "by CodeBreaker"};

}  // namespace

bool made_by_us(const py::str& path) {
    // Is the file at `path` one CodeBreaker wrote (this tool or the earlier one) -- and so may be written again? A
    // file anyone wrote by hand is never replaced. A file that is not there is ours to write.
    py::str head;
    try {
        head = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace")).read_text(600);
    } catch (const py::OSError&) {
        return true;
    }
    for (const char* mark : OURS)
        if (head.contains(py::str(mark))) return true;
    return false;
}

bool in_corpus(const py::str& path) {
    // Is the file one of the selected product's originals?
    std::optional<py::str> setting = py::settings::get_opt_str(py::str("corpus"));
    py::str root = py::os::path::normcase(py::os::path::abspath(setting ? *setting : py::str("")));
    return !root.empty() && py::os::path::normcase(py::os::path::abspath(path)).startswith(root + py::str("\\"));
}

Written write_module(const py::str& module, bool verify_, const std::optional<py::str>& out_dir0, const Log& log0, int64_t split, bool build_whole) {
    Log log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    std::optional<py::str> found_path = py::os::path::isdir(module) ? std::optional<py::str>(module) : base::corpus::retail(module);
    if (!found_path || found_path->empty()) throw py::SystemExit(py::percent(py::str("no module %r in the corpus"), module));
    py::str path = *found_path;
    if (py::os::path::isdir(path) || path.lower().endswith(py::str(".java")) || path.lower().endswith(py::str(".jar"))) {   // a Java program: its own writer
        auto fs = F::open();
        py::str sha, digest;
        std::tie(sha, digest) = verify_ ? verify::reconstruct::run(path, fs, log) : verify::reconstruct::identity(path);
        py::str name = py::os::path::basename(py::os::path::normpath(path));
        py::str where = out_dir0 ? *out_dir0 : state_path(py::list<py::str>{py::str("source"), name});
        return java_writer::write_java(path, fs, sha, digest, where, log);
    }
    py::str name = py::os::path::basename(path).lower();
    py::str stem = std::get<0>(py::os::path::splitext(name));
    // A product's source tree is for that product's modules. A file from anywhere else (given by path: another
    // product's, a PowerPC image) is written under state/source/ here.
    py::str out_dir = out_dir0 ? *out_dir0 : (in_corpus(path) ? placement::folder(name, path) : state_path(py::list<py::str>{py::str("source"), stem}));
    py::list<py::str> held;
    for (const py::str& f : {stem + py::str(".c"), stem + py::str(".h")})
        if (!made_by_us(py::os::path::join(out_dir, f))) held.append(f);
    if (!held.empty()) {                                       // someone wrote these by hand: not replaced
        log(py::percent(py::str("%s: %s written by hand -- the reconstruction goes to the state folder instead"), out_dir,
                        py::join(py::str(", "), held)));
        out_dir = state_path(py::list<py::str>{py::str("source"), stem});
    }
    py::os::makedirs(out_dir, 0777, true);
    auto fs = F::open();
    py::str sha, digest;
    if (verify_) std::tie(sha, digest) = verify::reconstruct::run(path, fs, log);
    else std::tie(sha, digest) = verify::reconstruct::identity(path);
    std::optional<py::str> kind = verify::reconstruct::kind_name(path);
    if (kind && (*kind == py::str("le") || *kind == py::str("dos"))) return write_assembly(path, fs, sha, digest, out_dir, log);
    auto best = fs->best(sha, digest);
    Judged judged(sha, best);          // each function's verdict, by its L2 provenance (module + address)
    py::list<py::str> addrs = fs->addresses(sha);                 // every function the module is known to have
    auto known = fs->identified(sha);                              // the runtime library's among them: {address: (name, library)}
    py::dict<py::str, std::tuple<py::str, py::str>> known_s;
    for (const auto& kv : known.items()) known_s.set(std::get<0>(kv), std::make_tuple(cs(std::get<0>(std::get<1>(kv))), cs(std::get<1>(std::get<1>(kv)))));
    py::dict<py::str, py::str> exported;
    for (const auto& kv : decompile::describe::decorated_names(path).items())
        exported.set(symbols::names::key(path, std::get<0>(kv)), std::get<1>(kv));
    py::set<py::str> used;
    for (const auto& kv : best.items())
        if (!cell_none(std::get<1>(kv).name)) used.add(cs(std::get<1>(kv).name));
    py::dict<py::str, py::str> real = real_import_names(path, used);
    // a recovered name for an internal function (a name it shares with a named function in another module, a
    // runtime-library name, or one from a debug symbol file) replaces the decompiler's sub_... placeholder
    // everywhere the module uses it
    py::dict<py::str, py::str> recovered;
    try {
        for (const auto& f : symbols::names::of_module(path, fs).functions)
            if (f.source != py::str("(none)") && !f.name.startswith(py::str("sub_"))) recovered.set(f.key, f.name);
    } catch (const std::exception&) {
    }
    py::dict<py::str, py::str> rename;
    py::set<py::str> taken;
    for (const py::str& v : real.values()) taken.add(v);
    for (const py::str& v : exported.values()) taken.add(v);
    for (const py::str& va : addrs) {
        auto row = best.get(va);
        py::str cur = (row && !cs(row->name).empty()) ? cs(row->name) : py::str("sub_") + va;
        std::optional<py::str> nm = recovered.get(va);
        bool in_rename = false;
        for (const py::str& v : rename.values())
            if (nm && v == *nm) in_rename = true;
        if (nm && !nm->empty() && cur.startswith(py::str("sub_")) && !taken.contains(*nm) && !in_rename) {
            rename.set(cur, *nm);
            taken.add(*nm);
        }
    }
    py::dict<py::str, py::str> sub = real.copy();
    sub.update(rename);
    std::optional<py::re::Pattern> spell;
    if (!sub.empty()) {
        std::vector<py::str> keys;
        for (const py::str& k : sub.keys()) keys.push_back(k);
        std::stable_sort(keys.begin(), keys.end(), [](const py::str& a, const py::str& b) { return a.size() > b.size(); });
        py::list<py::str> parts;
        for (const py::str& k : keys) parts.append(k);
        spell = py::re::Pattern(py::percent(py::str("\\b(%s)\\b"), py::join(py::str("|"), parts)), 0);
    }
    auto respell = [&](const py::str& text) -> py::str {
        if (!spell) return text;
        return spell->sub([&](const py::re::Match& m) { return sub.at(m.group(1)); }, text);
    };
    // names in place of numbers and placeholders (source/readable.py): SDK constants, handles and parameters named for
    // what Windows takes them as, function pointers for what they point at
    py::dict<py::str, py::str> texts;
    for (const auto& kv : best.items()) {
        const Candidate& r = std::get<1>(kv);
        if (!cs(r.source).empty()) texts.set(std::get<0>(kv), respell(cs(r.source)));
    }
    // the module as the store holds it, one L2 tree (each function's C as text, the header the module's), through the
    // readable pass; then the module as it is written, each function under its status line
    translate::ir::Val::List stored_units;
    for (const auto& kv : texts.items())
        stored_units.append(translate::ir::Val(translate::l2::function(std::get<0>(kv), std::get<1>(kv), origin_dict(judged.origin(std::get<0>(kv))))));
    py::str stored_header = cs(fs->get_text(sha, verify::reconstruct::SOURCE, py::str("header")));
    source::readable::Readable words(translate::l2::module(stored_units, stored_header), _string_reader(path, kind));
    Counts counts;
    for (const py::str& c : ORDER) counts.set(c, 0);
    mf::Document unit_doc;
    unit_doc.node = py::str("Module");
    for (const py::str& va : addrs) {
        V::Origin origin = judged.origin(va);
        std::optional<Candidate> row = judged.at(origin);       // (the verdict of the function this node stands for)
        py::list<py::str> said;
        if (exported.contains(va)) said.append(py::percent(py::str("exported as %s: %s"), exported.at(va), symbols::demangle::readable(exported.at(va))));
        bool is_known = known_s.contains(va);
        if (!row) {
            py::str shown_c = is_known ? V::IDENTIFIED : V::NO_SOURCE;
            counts.set(shown_c, counts.at(shown_c) + 1);
            py::list<py::str> why;
            if (is_known) why.append(std::get<1>(known_s.at(va)));
            for (const py::str& s : said) why.append(s);
            why.append(py::str("not measured on the current verifier"));
            mf::Unit u;
            u.node = py::str("Comment");
            u.text = py::percent(py::str("[%s] %s@ %s -- %s"), TITLE.at(shown_c), is_known ? std::get<0>(known_s.at(va)) + py::str("  ") : py::str(""), va,
                                 py::join(py::str("; "), why));
            unit_doc.units.append(u);
            continue;
        }
        py::str cls = cs(row->klass);
        py::str detail = describe(*row);
        py::str shown = (is_known && cls != V::VERIFIED && cls != V::WEAK) ? V::IDENTIFIED : cls;
        counts.set(shown, counts.at(shown) + 1);
        if (shown == V::IDENTIFIED) {
            said.insert(0, py::percent(py::str("%s of %s; reconstructed below: %s%s"), std::get<0>(known_s.at(va)), std::get<1>(known_s.at(va)),
                                       TITLE.at(cls), !detail.empty() ? py::str(", ") + detail : py::str("")));
        } else {
            py::list<py::str> front;
            if (!detail.empty()) front.append(detail);
            for (const py::str& s : said) front.append(s);
            said = front;
            if (is_known) said.append(py::percent(py::str("this is %s of %s"), std::get<0>(known_s.at(va)), std::get<1>(known_s.at(va))));
        }
        py::str source = (texts.contains(va) ? words.function(va) : py::str("/* (no reconstruction) */")).strip();
        py::str row_name = cs(row->name).empty() ? py::str("sub_") + va : cs(row->name);
        py::str shown_name = rename.get(row_name, row_name);
        if (shown_name.startswith(py::str("sub_")) && texts.contains(va)) {
            py::str hint = source::readable::calls_hint(texts.at(va));
            if (!hint.empty()) said.append(hint);
        }
        if (!exported.contains(va) && words.results_unused().contains(cs(row->name))) said.append(py::str("no caller here uses what it returns"));
        mf::Unit u;
        u.node = py::str("MethodDecl");
        u.text = source;
        u.comments = py::list<py::str>{py::percent(py::str("[%s] %s  @ %s%s"), TITLE.at(shown), shown_name, va,
                                                   !said.empty() ? py::str(" -- ") + py::join(py::str("; "), said) : py::str(""))};
        unit_doc.units.append(u);
    }
    int64_t total = addrs.size();
    mf::Facts facts;
    if (build_whole) {
        build::module::Built whole = build::module::build(path, py::os::path::join(out_dir, py::str("build")), false, [](const py::str&) {});
        mf::Whole w;
        w.linked = whole.linked();
        w.error = whole.error;
        w.exports_same = whole.exports_same;
        w.exports_total = whole.exports_total;
        w.rejected = whole.rejected.size();
        w.absent = whole.absent.size();
        w.built = py::os::path::join(out_dir, py::str("build"), stem + std::get<1>(py::os::path::splitext(name)));
        facts.whole = w;
    }
    facts.name = name;
    facts.stem = stem;
    facts.date = strftime("%Y-%m-%d");
    facts.digest = digest;
    facts.order = ORDER;
    facts.titles = TITLE;
    facts.counts = counts;
    facts.total = total;
    facts.kind = kind ? *kind : py::str("");
    facts.real = real.size();
    std::optional<py::str> header_cell;
    {
        Cell h = fs->get_text(sha, verify::reconstruct::SOURCE, py::str("header"));
        if (!cell_none(h)) header_cell = cs(h);
    }
    py::str header = header_cell ? *header_cell : py::str("");
    if (!header.empty()) {
        header = respell(header);
        if (!real.empty()) {                                   // which module and number each import is
            std::vector<std::pair<py::str, py::str>> items;
            for (const auto& kv : real.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
            std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.second.u32() < b.second.u32(); });
            py::str body;
            for (const auto& it : items) {
                auto [head, sep, tail] = it.first.rpartition(py::str("_"));      // s.rsplit("_", 1)
                py::str a = sep.empty() ? it.first : head, b = sep.empty() ? py::str("") : tail;
                body += py::percent(py::str(" *   %-28s %s.%s\n"), it.second, a, b);
            }
            header += py::percent(py::str("\n/* imports by number:\n%s */\n"), body);
        }
        header += _globals_note(path, header);
        header = source::readable::_rename(header, words.globals);       // the globals as the functions now call them
        if (!words.constants_used.empty())
            header += py::percent(py::str("\n/* SDK constants written into %s.c, with the values the SDK headers give them */\n%s"), stem, words.definitions());
        header += words.struct_definitions();
        py::file f = py::open(py::os::path::join(out_dir, stem + py::str(".h")), py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(py::percent(py::str("/* %s -- types, declarations and globals used by the functions in %s.c.\n"
                                    " * Globals are bound to their addresses in the original until the module's data is\n"
                                    " * reconstructed. */\n\n"),
                            name, stem) + header);
        f.close();
    }
    facts.header = !header.empty();
    // the C, written by its back end from the module's tree and the store's facts
    auto files = mf::c(unit_doc, facts, split);
    for (const auto& tf : files) {
        py::file f = py::open(py::os::path::join(out_dir, std::get<0>(tf)), py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(std::get<1>(tf));
        f.close();
    }
    py::str out = py::os::path::join(out_dir, std::get<0>(files[0]));
    // the module's C++ classes, each virtual function tied to the function above that implements it
    auto name_at = [&](int64_t address) -> py::str {
        py::str k = symbols::names::key(path, address);
        if (!best.contains(k) && !known_s.contains(k) && formats::detect::flat(kind)) {
            std::optional<py::str> behind = classes_writer::import_behind(path, address);
            if (behind) return *behind;
        }
        auto row = best.get(k);
        py::str cur = (row && !cs(row->name).empty()) ? cs(row->name) : (known_s.contains(k) ? std::get<0>(known_s.at(k)) : py::str("sub_") + k);
        return sub.get(cur, cur);
    };
    auto status_at = [&](int64_t address) -> py::str {
        auto row = best.get(symbols::names::key(path, address));
        return row ? TITLE.at(cs(row->klass)) : py::str("");
    };
    std::optional<py::str> written = classes_writer::write(path, kind ? *kind : py::str(""), out_dir, stem, name_at, status_at);
    if (written) log(py::percent(py::str("%s: the module's C++ classes"), *written));
    py::list<py::str> shown;
    for (const py::str& c : ORDER)
        if (counts.at(c)) shown.append(py::percent(py::str("%s %d"), TITLE.at(c), counts.at(c)));
    log(py::percent(py::str("%s: %s"), out, py::join(py::str(", "), shown)));
    return ModuleWritten(out, counts, total);
}

py::str scoreboard(const std::optional<py::str>& out_dir, const Log& log0) {
    // Rewrite SCOREBOARD.md from the store: modules judged under the CURRENT verifier settings of their kind (32-bit,
    // 16-bit, PowerPC, driver).
    Log log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    auto fs = F::open();
    std::vector<std::pair<py::str, py::str>> current;       // (digest, kind)
    for (const auto& kv : verify::reconstruct::digests().items())
        if (std::get<1>(kv) && !std::get<1>(kv)->empty()) current.emplace_back(*std::get<1>(kv), std::get<0>(kv));
    {   // dict: a digest shared by two kinds keeps the later kind, in the first one's place
        std::vector<std::pair<py::str, py::str>> uniq;
        for (const auto& c : current) {
            bool done = false;
            for (auto& u : uniq)
                if (u.first == c.first) { u.second = c.second; done = true; }
            if (!done) uniq.push_back(c);
        }
        current = uniq;
    }
    std::vector<std::pair<py::str, py::str>> by_kind = current;
    std::stable_sort(by_kind.begin(), by_kind.end(), [](const auto& a, const auto& b) { return a.second.u32() < b.second.u32(); });
    py::list<py::str> dparts;
    for (const auto& dk : by_kind) dparts.append(py::percent(py::str("%s %s"), dk.second, dk.first));
    py::str digest = py::join(py::str(", "), dparts);
    py::list<py::str> lines;
    lines.append(py::str("# Reconstruction scoreboard"));
    lines.append(py::str(""));
    lines.append(py::percent(py::str("Generated %s by `python -m codebreaker scoreboard` (verifier settings `%s`)."), strftime("%Y-%m-%d %H:%M"), digest));
    lines.append(py::str(""));
    lines.append(py::str("Only modules measured on the current verifier are listed. A function is VERIFIED when "
                         "its reconstructed C, rebuilt and run beside the original, returns the same value, makes "
                         "the same API calls with the same arguments, changes the same memory and leaves the stack "
                         "and saved registers alike on every input tried, with at least half of it exercised. "
                         "Everything else is not progress. A function IDENTIFIED as a runtime library's (the C "
                         "runtime, a compiler helper) is not the module's own code and is counted apart, unless it "
                         "was verified as well."));
    lines.append(py::str(""));
    lines.append(py::str("| Module | Kind | Functions | Verified | Verified (weak) | Identified (runtime library) | Unverified | Not judged | Does not compile | No source | Builds whole | Runs in place |"));
    lines.append(py::str("|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|"));
    py::dict<py::str, int64_t> tot;
    for (const py::str& k : ORDER) tot.set(k, 0);
    py::dict<py::str, py::str> where;
    for (const auto& row : fs->module_rows()) where.set(cs(row.at(0)), cs(row.at(2)));
    struct Mod {
        py::str name, kind, sha, d;
    };
    std::vector<Mod> mods;
    for (const auto& dk : current)
        for (const auto& row : fs->modules(dk.first))
            if (in_corpus(where.get(cs(row.at(0))).value_or(py::str("")))) mods.push_back({cs(row.at(1)), dk.second, cs(row.at(0)), dk.first});
    std::sort(mods.begin(), mods.end(), [](const Mod& a, const Mod& b) {
        if (a.name.u32() != b.name.u32()) return a.name.u32() < b.name.u32();
        if (a.kind.u32() != b.kind.u32()) return a.kind.u32() < b.kind.u32();
        if (a.sha.u32() != b.sha.u32()) return a.sha.u32() < b.sha.u32();
        return a.d.u32() < b.d.u32();
    });
    for (const Mod& m : mods) {
        py::dict<py::str, int64_t> c;
        for (const auto& kv : fs->scores(m.d, m.sha).items()) c.set(std::get<0>(kv), engine::native_engine::cell_int(std::get<1>(kv)));
        int64_t n = 0;
        for (int64_t v : c.values()) n += v;
        for (const py::str& k : ORDER) tot.set(k, tot.at(k) + c.at(k));
        lines.append(py::percent(py::str("| %s | %s | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | not tested | not tested |"), m.name, m.kind, n,
                                 c.at(py::str("verified")), 100.0 * static_cast<double>(c.at(py::str("verified"))) / static_cast<double>(std::max<int64_t>(1, n)),
                                 c.at(py::str("weak")), c.at(py::str("identified")), c.at(py::str("unverified")), c.at(py::str("not-judged")),
                                 c.at(py::str("no-compile")), c.at(py::str("no-source"))));
    }
    int64_t n = 0;
    for (int64_t v : tot.values()) n += v;
    if (n) {
        lines.append(py::percent(py::str("| **all measured** | | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | | |"), n, tot.at(py::str("verified")),
                                 100.0 * static_cast<double>(tot.at(py::str("verified"))) / static_cast<double>(n), tot.at(py::str("weak")),
                                 tot.at(py::str("identified")), tot.at(py::str("unverified")), tot.at(py::str("not-judged")),
                                 tot.at(py::str("no-compile")), tot.at(py::str("no-source"))));
    }
    lines.append(py::str(""));
    lines.append(py::percent(py::str("%d modules measured so far (the corpus has %d 32-bit, %d 16-bit)."), static_cast<int64_t>(mods.size()),
                             base::corpus::modules(py::str("pe")).size(), base::corpus::modules(py::str("ne")).size()));
    lines.append(py::str(""));
    py::str out = py::os::path::join(out_dir ? *out_dir : py::os::path::dirname(base::corpus::design_dir()), py::str("SCOREBOARD.md"));
    {
        py::file f = py::open(out, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(py::join(py::str("\n"), lines));
        f.close();
    }
    log(py::percent(py::str("wrote %s (%d modules)"), out, static_cast<int64_t>(mods.size())));
    return out;
}

int64_t main(const py::list<py::str>& argv) {
    std::optional<py::str> out_dir;
    if (argv.contains(py::str("--out"))) out_dir = argv[argv.index(py::str("--out")) + 1];
    int64_t split = argv.contains(py::str("--split")) ? py::int_parse(argv[argv.index(py::str("--split")) + 1], 10) : 0;
    py::list<py::str> mods;
    for (const py::str& a : argv) {
        bool skipped = (out_dir && a == *out_dir) || (split && a == py::to_str(split));
        if (!a.startswith(py::str("--")) && !skipped) mods.append(a);
    }
    if (mods.empty() && !argv.contains(py::str("--scoreboard"))) {
        py::print(py::str("module_writer.py -- one source file per module, every function labelled with what is known.\n\n"
                          "    python -m codebreaker module comctl32.dll               reconstruct, judge, then write the file\n"
                          "    python -m codebreaker module comctl32.dll --no-verify   write from what the store already holds\n"
                          "    python -m codebreaker module comctl32.dll --out DIR     write there instead of the source tree\n"
                          "    python -m codebreaker module comctl32.dll --build      also build the whole module and report it\n"
                          "    python -m codebreaker module comctl32.dll --split 200   at most 200 functions to a file\n"
                          "    python -m codebreaker scoreboard                        rewrite the scoreboard only\n"
                          "    python -m codebreaker module path/to/java/src           a Java program: its C++, class by class"));
        return 2;
    }
    for (const py::str& m : mods)
        write_module(m, !argv.contains(py::str("--no-verify")), out_dir, nullptr, split, argv.contains(py::str("--build")));
    bool any = false;
    for (const py::str& m : mods)
        if (!py::os::path::exists(m) || in_corpus(m)) any = true;
    if (argv.contains(py::str("--scoreboard")) || any) scoreboard(out_dir);      // (not for a file that is not the product's)
    return 0;
}

}  // namespace codebreaker::source::module_writer
