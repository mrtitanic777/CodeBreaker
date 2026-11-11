// codebreaker/build/flat_thunk.cpp -- typed C++ of codebreaker.build.flat_thunk (see flat_thunk.hpp).

#include "codebreaker/build/flat_thunk.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_glob.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::flat_thunk {

namespace corpus = codebreaker::base::corpus;
namespace detect = codebreaker::formats::detect;
namespace omf = codebreaker::formats::omf;
namespace watcom = codebreaker::build::watcom;
namespace prototypes = codebreaker::symbols::prototypes;

namespace {

const char* _DOC = R"DOC(flat_thunk.py -- the authentic-rebuild + binary-verify engine, aimable at any Win95 module.

Automates the workflow proven by hand on LZ32.DLL: CLASSIFY a binary by how Microsoft actually built
it, RECOVER authentic source for that class, BUILD it with the period toolchain, DIFF the result
against the shipped binary, and DOCUMENT the verification level. This file is the FOUNDATION --
the binary model (PE/NE parse), the classifier, and the differ; the per-class recoverers and the
build drivers plug into it.

The guiding standard (design/RECONSTRUCTION_STANDARD.md): authentic-method + binary-verified, not
source-text-exact. The differ reports what is actually verifiable -- entry point, section layout,
exports, imports, size -- never "we guessed the names."

    python -m codebreaker flat-thunk <binary> [--classify] [--diff <built.dll>]
)DOC";

// (the items of a list that is a temporary: a copy of them, which a range-for keeps alive)
template <class L>
std::vector<typename L::value_type> V(const L& l) {
    return l.vec();
}

py::str D(int64_t v) { return py::str(std::to_string(v)); }

// subprocess.run(args, cwd=, env=, capture_output=True, text=True) (no timeout); `strict` is text=True's own decoding
// errors, else errors="replace"
py::subprocess::CompletedProcess run_tool(const py::list<py::str>& args, const std::optional<py::str>& cwd,
                                          const py::dict<py::str, py::str>& env) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.env = env;   // (the whole environment of the tool)
    return py::subprocess::run(args, o);
}

py::str out_of(const py::subprocess::CompletedProcess& r) {
    return (r.stdout_ ? *r.stdout_ : py::str("")) + (r.stderr_ ? *r.stderr_ : py::str(""));
}

// glob.glob(os.path.join(dir, "*" + ext))
py::list<py::str> glob_ext(const py::str& dir, const std::string& ext) {
    return py::glob::glob(py::os::path::join(dir, py::str("*") + py::str(ext)));
}

py::str lower_ext(const py::str& f) {
    // f.rsplit(".", 1)[-1].lower()
    int64_t p = f.rfind(py::str("."));
    return (p < 0 ? f : f.slice(p + 1, f.size())).lower();
}

std::optional<py::list<py::str>> k32_of(const py::dict<py::str, py::list<py::str>>& imps) {
    py::list<py::str> a = imps.get(py::str("KERNEL32.dll"), py::list<py::str>());
    if (!a.empty()) return a;
    py::list<py::str> b = imps.get(py::str("KERNEL32.DLL"), py::list<py::str>());
    if (!b.empty()) return b;
    return py::list<py::str>();
}

}  // namespace

// ------------------------------------------------------------------ binary model (PE + NE)
PE::PE(const py::str& path_) : path(path_), f(pe_format::open_pe(path_)), d(f.data), entry(f.entry), nsec(f.n_sections) {
    for (const pe_format::Section& s : f.sections.vec()) secs.append(Section(s.name, s.vaddr, s.vsize, s.rawsize, s.rawptr));
}

std::optional<int64_t> PE::_file2rva(int64_t o) const {
    for (const Section& s : secs.vec()) {
        int64_t va = std::get<1>(s), rs = std::get<3>(s), pr = std::get<4>(s);
        if (rs && pr <= o && o < pr + rs) return va + (o - pr);
    }
    return std::nullopt;
}

py::list<std::tuple<int64_t, py::str>> PE::exports() const {
    std::vector<std::tuple<int64_t, py::str>> v;
    for (const pe_format::ExportSymbol& x : V(f.export_table().symbols))
        if (!x.name.empty()) v.emplace_back(x.ordinal, x.name);
    std::sort(v.begin(), v.end());
    return py::list<std::tuple<int64_t, py::str>>(v);
}

py::dict<py::str, py::list<py::str>> PE::imports() const {
    py::dict<py::str, py::list<py::str>> out;
    for (const pe_format::ImportModule& m : V(f.import_table())) {
        py::list<py::str> names;
        for (const pe_format::ImportSymbol& x : m.symbols.vec())
            names.append(!x.name.empty() ? x.name : py::percent(py::str("@%d"), x.ordinal));
        out.set(m.module, names);
    }
    return out;
}

py::dict<py::str, int64_t> PE::export_addrs() const {
    std::vector<std::tuple<int64_t, py::str, int64_t>> named;
    for (const pe_format::ExportSymbol& x : V(f.export_table().symbols))
        if (!x.name.empty()) named.emplace_back(x.hint, x.name, x.rva);
    std::sort(named.begin(), named.end());
    py::dict<py::str, int64_t> out;
    for (const auto& t : named) out.set(std::get<1>(t), std::get<2>(t));   // (in the name table's order)
    return out;
}

py::bytes PE::read_at(int64_t rva, int64_t n) const {
    std::optional<int64_t> o = f.file_offset(rva);
    return o ? d.slice(*o, *o + n) : py::bytes();
}

NE::NE(const py::str& path_) : path(path_), f(path_), d(f.data), nseg(f.h.n_segs) {
    for (const ne_format::Segment& s : V(f.segments())) segs.append(std::make_tuple(s.kind, s.file_off, s.length, s.flags));
}

py::list<std::tuple<int64_t, py::str>> NE::resident_names() const {
    py::list<std::tuple<int64_t, py::str>> out;
    for (const auto& t : V(f.resident_names())) out.append(std::make_tuple(std::get<1>(t), std::get<0>(t)));
    return out;
}

Binary load(const py::str& path) {
    std::optional<py::str> fmt = detect::format_of(path);
    if (fmt && *fmt == "PE") return PE(path);
    if (fmt && *fmt == "NE") return NE(path);
    return std::monostate{};
}

// ------------------------------------------------------------------ classifier
const py::list<py::str> _THUNK_MARKERS{py::str("SMapLS"), py::str("SUnMapLS"), py::str("ThunkConnect32"),
                                       py::str("ThunkInitLSF"), py::str("FT_Prolog"), py::str("QT_Thunk")};

py::list<std::tuple<py::str, py::str>> Classification::items() const {
    py::list<std::tuple<py::str, py::str>> out;
    out.append(std::make_tuple(py::str("format"), format));
    out.append(std::make_tuple(py::str("subtype"), subtype));
    out.append(std::make_tuple(py::str("recipe"), recipe ? *recipe : py::str("None")));
    if (n_exports) out.append(std::make_tuple(py::str("n_exports"), D(*n_exports)));
    if (n_imports) out.append(std::make_tuple(py::str("n_imports"), D(*n_imports)));
    out.append(std::make_tuple(py::str("why"), why));
    return out;
}

Classification classify(const py::str& path) {
    Binary b = load(path);
    Classification c;
    if (std::holds_alternative<std::monostate>(b)) {
        c.format = py::str("?");
        c.subtype = py::str("unknown");
        c.why = py::str("not MZ/PE/NE");
        return c;
    }
    if (const NE* ne = std::get_if<NE>(&b)) {
        c.format = py::str("NE");
        c.subtype = py::str("win16-c");
        c.recipe = py::str("msc6-dosbox");
        c.why = py::percent(py::str("16-bit NE; %d segments"), ne->nseg);
        return c;
    }
    // PE: flat-thunk veneer vs 32-bit C logic
    const PE& pe = std::get<PE>(b);
    py::dict<py::str, py::list<py::str>> imps = pe.imports();
    py::list<py::str> k32 = *k32_of(imps);
    bool has_thunk = false;
    for (const py::str& n : k32.vec())
        for (const py::str& m : _THUNK_MARKERS.vec())
            if (n.contains(m)) has_thunk = true;
    py::list<std::tuple<int64_t, py::str>> exp = pe.exports();
    int64_t textsz = 0;
    for (const Section& s : pe.secs.vec())
        if (std::get<0>(s) == ".text") {
            textsz = std::get<2>(s);
            break;
        }
    c.format = py::str("PE");
    if (has_thunk) {
        c.subtype = py::str("flat-thunk");
        c.recipe = py::str("thunk-ml-cl-link");
        c.n_exports = exp.size();
        int64_t n = 0;
        for (const auto& kv : imps.items()) n += std::get<1>(kv).size();
        c.n_imports = n;
        py::set<py::str> found;
        for (const py::str& n2 : k32.vec())
            for (const py::str& m : _THUNK_MARKERS.vec())
                if (n2.contains(m)) found.add(m);
        c.why = py::str("imports flat-thunk helpers (") + py::join(py::str(","), py::sorted(found)) + ")";
        return c;
    }
    c.subtype = py::str("win32-c");
    c.recipe = py::str("msvc-native");
    c.n_exports = exp.size();
    c.why = py::percent(py::str("32-bit PE, no flat-thunk markers; .text=%d B"), textsz);
    return c;
}

// ------------------------------------------------------------------ differ
BinDiff bindiff(const py::str& built, const py::str& shipped) {
    Binary bo = load(built), sh = load(shipped);
    BinDiff r;
    if (std::holds_alternative<std::monostate>(bo) || std::holds_alternative<std::monostate>(sh)) {
        r.error = py::str("could not parse one of the inputs");
        return r;
    }
    r.named = true;
    r.built = py::os::path::basename(built);
    r.shipped = py::os::path::basename(shipped);
    r.size_built = std::holds_alternative<PE>(bo) ? std::get<PE>(bo).d.size() : std::get<NE>(bo).d.size();
    r.size_shipped = std::holds_alternative<PE>(sh) ? std::get<PE>(sh).d.size() : std::get<NE>(sh).d.size();
    if (std::holds_alternative<PE>(bo) && std::holds_alternative<PE>(sh)) {
        const PE& b = std::get<PE>(bo);
        const PE& s = std::get<PE>(sh);
        r.kind = py::str("PE");
        r.entry_match = b.entry == s.entry;
        r.entry = std::make_tuple(py::hex(b.entry), py::hex(s.entry));
        for (const Section& x : b.secs.vec()) r.sections_built.append(std::get<0>(x));
        for (const Section& x : s.secs.vec()) r.sections_shipped.append(std::get<0>(x));
        r.sections_match = r.sections_built == r.sections_shipped;
        py::dict<int64_t, py::str> eb, es;
        for (const auto& t : V(b.exports())) eb.set(std::get<0>(t), std::get<1>(t));
        for (const auto& t : V(s.exports())) es.set(std::get<0>(t), std::get<1>(t));
        r.exports_match = py::sorted(b.exports()) == py::sorted(s.exports());
        r.n_exports = std::make_tuple(eb.size(), es.size());
        py::dict<py::str, py::list<py::str>> bi = b.imports(), si = s.imports();
        py::set<py::str> allsyms_b, allsyms_s;
        py::list<py::str> ib_names, ii_names;
        for (const auto& kv : bi.items()) {
            ib_names.append(std::get<0>(kv));
            for (const py::str& n : std::get<1>(kv).vec()) allsyms_b.add(std::get<0>(kv) + "!" + n);
        }
        for (const auto& kv : si.items()) {
            ii_names.append(std::get<0>(kv));
            for (const py::str& n : std::get<1>(kv).vec()) allsyms_s.add(std::get<0>(kv) + "!" + n);
        }
        r.import_dlls_built = py::sorted(ib_names);
        r.import_dlls_shipped = py::sorted(ii_names);
        r.imports_only_in_shipped = py::sorted(allsyms_s - allsyms_b);
        r.imports_only_in_built = py::sorted(allsyms_b - allsyms_s);
        r.n_imports = std::make_tuple(allsyms_b.size(), allsyms_s.size());
    } else if (std::holds_alternative<NE>(bo) && std::holds_alternative<NE>(sh)) {
        const NE& b = std::get<NE>(bo);
        const NE& s = std::get<NE>(sh);
        r.kind = py::str("NE");
        r.nseg = std::make_tuple(b.nseg, s.nseg);
        for (const auto& x : b.segs.vec()) r.seg_kinds_built.append(std::get<0>(x));
        for (const auto& x : s.segs.vec()) r.seg_kinds_shipped.append(std::get<0>(x));
        r.seg_kinds_match = r.seg_kinds_built == r.seg_kinds_shipped;
        r.exports_match = py::sorted(b.resident_names()) == py::sorted(s.resident_names());
    } else {
        r.error = py::str("format mismatch: built=") + (std::holds_alternative<PE>(bo) ? "PE" : "NE") +
                  " shipped=" + (std::holds_alternative<PE>(sh) ? "PE" : "NE");
        return r;
    }
    // verdict
    if (r.kind && *r.kind == "PE") {
        bool exact = r.entry_match && r.sections_match && r.exports_match && r.imports_only_in_shipped.empty() &&
                     r.imports_only_in_built.empty();
        bool struct_ok = r.entry_match && r.sections_match && r.exports_match;
        r.verdict = py::str(exact ? "byte-surface-identical" : (struct_ok ? "structural-parity" : "partial"));
    } else {
        r.verdict = py::str((r.seg_kinds_match && r.exports_match) ? "structural-parity" : "partial");
    }
    return r;
}

namespace {
py::str pair_repr(const py::str& a, const py::str& b) { return py::str("(") + py::repr(a) + ", " + py::repr(b) + ")"; }
py::str pair_repr(int64_t a, int64_t b) { return py::str("(") + D(a) + ", " + D(b) + ")"; }
}  // namespace

py::str _fmt_diff(const BinDiff& r) {
    py::list<py::str> L;
    L.append(py::percent(py::str("  kind=%s  size built=%d shipped=%d  verdict=%s"), r.kind ? *r.kind : py::str("?"), r.size_built,
                         r.size_shipped, r.verdict ? *r.verdict : py::str("?")));
    if (r.kind && *r.kind == "PE") {
        L.append(py::percent(py::str("  entry %s %s"), pair_repr(std::get<0>(r.entry), std::get<1>(r.entry)),
                             py::str(r.entry_match ? "MATCH" : "differ")));
        py::str detail = r.sections_match ? py::str("")
                                          : py::percent(py::str(" (%s vs %s)"), py::repr(r.sections_built), py::repr(r.sections_shipped));
        L.append(py::percent(py::str("  sections %s%s"), py::str(r.sections_match ? "MATCH" : "differ"), detail));
        L.append(py::percent(py::str("  exports %s %s"), pair_repr(std::get<0>(r.n_exports), std::get<1>(r.n_exports)),
                             py::str(r.exports_match ? "MATCH" : "differ")));
        L.append(py::percent(py::str("  imports %s  only-in-shipped=%s  only-in-built=%s"),
                             pair_repr(std::get<0>(r.n_imports), std::get<1>(r.n_imports)), py::repr(r.imports_only_in_shipped),
                             py::repr(r.imports_only_in_built)));
    } else if (r.kind && *r.kind == "NE") {
        L.append(py::percent(py::str("  segments %s  kinds %s  exports %s"), pair_repr(std::get<0>(r.nseg), std::get<1>(r.nseg)),
                             py::str(r.seg_kinds_match ? "MATCH" : "differ"), py::str(r.exports_match ? "MATCH" : "differ")));
    }
    if (r.error && !r.error->empty()) L.append(py::str("  ERROR: ") + *r.error);
    return py::join(py::str("\n"), L);
}

// ------------------------------------------------------------------ toolchain resolver
std::optional<py::str> _first(const py::list<std::optional<py::str>>& cands) {
    for (const std::optional<py::str>& c : cands.vec())
        if (c && !c->empty() && py::os::path::exists(*c)) return c;
    return std::nullopt;
}

py::list<py::str> _reference_roots() {
    std::optional<py::str> ref = corpus::reference();
    py::list<py::str> cands{corpus::reference()};
    py::list<py::str> out;
    py::set<py::str> seen;
    for (const py::str& c : cands.vec())
        if (!c.empty() && !seen.contains(c) && py::os::path::isdir(c)) {
            seen.add(c);
            out.append(c);
        }
    return out;
}

Toolchain toolchain() {
    // from codebreaker.base import settings
    py::list<py::str> refs = _reference_roots();
    auto ref_find = [&](const std::vector<py::str>& rel) -> std::optional<py::str> {
        py::list<std::optional<py::str>> cands;
        for (const py::str& r : refs.vec()) {
            py::str p = r;
            for (const py::str& part : rel) p = py::os::path::join(p, part);
            cands.append(p);
        }
        return _first(cands);
    };
    std::optional<py::str> cl_opt = py::settings::get_opt_str(py::str("period_compiler"));   // the BIN folder of Visual C++ 2.0
    py::str cl = cl_opt ? *cl_opt : py::str("");
    py::str stripped = cl.rstrip(py::str("\\/"));
    py::str msvc2 = py::os::path::basename(stripped).upper() == "BIN" ? py::os::path::dirname(stripped) : cl;
    std::optional<py::str> win32lib = ref_find({py::str("WIN32SDK"), py::str("MSTOOLS"), py::str("LIB"), py::str("I386"), py::str("THUNK32.LIB")});
    Toolchain tc;
    tc.thunk = ref_find({py::str("thunk-1.01"), py::str("THUNK.EXE")});
    if (!tc.thunk || tc.thunk->empty())
        tc.thunk = ref_find({py::str("WIN32SDK"), py::str("MSTOOLS"), py::str("BIN"), py::str("I386"), py::str("THUNK.EXE")});
    tc.ml = ref_find({py::str("Microsoft Windows 95 DDK"), py::str("MASM611C"), py::str("ML.EXE")});
    tc.cl = py::os::path::join(msvc2, py::str("BIN"), py::str("CL.EXE"));
    tc.link = py::os::path::join(msvc2, py::str("BIN"), py::str("LINK.EXE"));
    tc.lib = py::os::path::join(msvc2, py::str("BIN"), py::str("LIB.EXE"));
    tc.rc = py::os::path::join(msvc2, py::str("BIN"), py::str("RC.EXE"));
    tc.include = py::os::path::join(msvc2, py::str("INCLUDE"));
    tc.lib_dirs.append(py::os::path::join(msvc2, py::str("LIB")));
    if (win32lib && !win32lib->empty()) tc.lib_dirs.append(py::os::path::dirname(*win32lib));
    return tc;
}

py::str _winpath(const py::str& p) { return p.replace(py::str("/"), py::str("\\")); }

Discovery _discover_flat_thunk(const py::str& srcdir) {
    Discovery out;
    py::list<py::str> thk = glob_ext(srcdir, ".thk");
    py::list<py::str> defs = glob_ext(srcdir, ".def");
    py::list<py::str> k32, moddefs;
    for (const py::str& d : defs.vec())
        if (py::os::path::basename(d).lower().endswith(py::str("k32.def"))) k32.append(d);
    for (const py::str& d : defs.vec())
        if (!k32.contains(d)) moddefs.append(d);
    py::list<py::str> cfiles = glob_ext(srcdir, ".c");
    py::list<py::str> rc = glob_ext(srcdir, ".rc");
    if (thk.empty()) {
        out.error = py::str("no .thk in ") + srcdir;
        return out;
    }
    if (moddefs.empty()) {
        out.error = py::str("no module .def in ") + srcdir;
        return out;
    }
    if (k32.empty()) {
        out.error = py::str("no *k32.def (KERNEL32 ordinal import lib) in ") + srcdir;
        return out;
    }
    if (cfiles.empty()) {
        out.error = py::str("no veneer .c in ") + srcdir;
        return out;
    }
    py::str text;
    {
        py::file f = py::open(moddefs[0], py::str("r"), py::str("latin1"));
        text = f.read_text();
    }
    std::optional<py::re::Match> m = py::re::search(py::str("^\\s*LIBRARY\\s+(\\w+)"), text, py::re::M | py::re::I);
    out.module = (m ? m->group(1) : std::get<0>(py::os::path::splitext(py::os::path::basename(moddefs[0])))).lower();
    out.thk = thk[0];
    out.cfiles = cfiles;
    out.moddef = moddefs[0];
    out.k32def = k32[0];
    if (!rc.empty()) out.rc = rc[0];
    return out;
}

const py::dict<py::str, int64_t> _BYVAL{{py::str("POINT"), 8},          {py::str("POINTL"), 8},         {py::str("SIZE"), 8},
                                        {py::str("SIZEL"), 8},          {py::str("RECT"), 16},          {py::str("RECTL"), 16},
                                        {py::str("double"), 8},         {py::str("__int64"), 8},        {py::str("LARGE_INTEGER"), 8},
                                        {py::str("ULARGE_INTEGER"), 8}, {py::str("DWORDLONG"), 8},      {py::str("LONGLONG"), 8}};

int64_t _param_bytes(const py::str& part_in) {
    py::str part = part_in.strip();
    if (part.empty() || part == "void" || part == "...") return 0;
    if (part.contains(py::str("*"))) return 4;
    py::list<py::str> toks;
    static const py::set<py::str> skip{py::str("const"),  py::str("far"),      py::str("near"),    py::str("FAR"),
                                       py::str("NEAR"),   py::str("unsigned"), py::str("signed")};
    for (const py::str& t : V(py::split(part)))
        if (!skip.contains(t)) toks.append(t);
    return _BYVAL.get(!toks.empty() ? toks[0] : py::str(""), 4);
}

std::optional<OwPaths> _ow_paths() {
    // P = platforms.of("pe") -- 32-bit Windows: its compiler and linker
    py::str ow = watcom::openwatcom_root();
    py::str wcc = watcom::ow_tool(py::str("wcc386"));
    py::str wlink = watcom::ow_tool(py::str("wlink"));
    py::list<py::str> incs{py::os::path::join(ow, py::str("h"), py::str("nt")), py::os::path::join(ow, py::str("h"))};
    if (wcc.empty()) return std::nullopt;
    OwPaths p;
    p.root = ow;
    p.wcc386 = wcc;
    p.wlink = wlink;
    p.include = incs;
    return p;
}

CompileOw compile_ow(const py::str& srcdir) {
    CompileOw out;
    std::optional<OwPaths> ow = _ow_paths();
    if (!ow) {
        out.error = py::str("OpenWatcom not built (experiments/premise-proof/open-watcom-v2)");
        return out;
    }
    py::dict<py::str, py::str> env = py::os::environ_::copy();   // os.environ, with the compiler's additions
    env.set(py::str("WATCOM"), ow->root);
    env.set(py::str("INCLUDE"), py::join(py::str(";"), ow->include));
    env.set(py::str("PATH"), py::os::path::dirname(ow->wcc386) + py::os::pathsep + py::os::getenv(py::str("PATH"), py::str("")));
    py::list<py::str> cs = py::sorted(glob_ext(srcdir, ".c"));
    for (const py::str& c : cs.vec()) {
        py::subprocess::CompletedProcess r =
            run_tool(py::list<py::str>{ow->wcc386, py::str("-bt=nt"), py::str("-zq"), py::str("-w0"), py::os::path::basename(c)}, srcdir, env);
        py::str first;
        for (const py::str& l : V(py::splitlines(out_of(r))))
            if (l.contains(py::str("Error"))) {
                first = l;
                break;
            }
        out.results.append(std::make_tuple(py::os::path::basename(c), r.returncode, first.strip().slice(0, 100)));
    }
    bool ok = true;
    for (const auto& t : out.results.vec())
        if (std::get<1>(t) != 0) ok = false;
    out.ok = ok;
    return out;
}

BuildResult build_flat_thunk_ow(const py::str& srcdir_in, const std::optional<py::str>& outdir_in) {
    BuildResult res;
    std::optional<OwPaths> ow = _ow_paths();
    if (!ow) {
        res.error = py::str("OpenWatcom not built");
        return res;
    }
    py::str srcdir = py::os::path::abspath(srcdir_in);
    py::str outdir = py::os::path::abspath(outdir_in && !outdir_in->empty() ? *outdir_in : py::os::path::join(srcdir, py::str("_ow")));
    py::os::makedirs(outdir, 0777, true);
    py::list<py::str> cfiles = glob_ext(srcdir, ".c");
    if (cfiles.empty()) {
        res.error = py::str("no veneer .c in ") + srcdir;
        return res;
    }
    py::str cpath = cfiles[0];
    py::str module = std::get<0>(py::os::path::splitext(py::os::path::basename(cpath)));
    py::dict<py::str, py::str> env = py::os::environ_::copy();   // os.environ, with the compiler's additions
    env.set(py::str("WATCOM"), ow->root);
    env.set(py::str("INCLUDE"), py::join(py::str(";"), ow->include));
    env.set(py::str("PATH"), py::os::path::dirname(ow->wcc386) + py::os::pathsep + py::os::getenv(py::str("PATH"), py::str("")));
    py::str libdir = py::os::path::join(ow->root, py::str("lib386"), py::str("nt"));

    // copy the veneer verbatim (authentic source; exports come from directives, not __declspec).
    py::str text;
    {
        py::file f = py::open(cpath, py::str("r"), py::str("utf-8"), py::str("replace"));
        text = f.read_text();
    }
    {
        py::file f = py::open(py::os::path::join(outdir, module + ".c"), py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
        f.write(text);
    }
    // the thunk connection routine is emitted by the thunk compiler; stub it for the scaffold link.
    {
        py::file f = py::open(py::os::path::join(outdir, py::str("thkstub.c")), py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
        f.write(py::percent(py::str("#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n"
                                    "/* scaffold stub for the thunk compiler's connection routine (real one at body curation) */\n"
                                    "BOOL __stdcall %s_ThunkConnect32(LPSTR a,LPSTR b,HINSTANCE h,DWORD r){return TRUE;}\n"),
                            module));
    }
    py::list<py::str> log;
    for (const py::str& c : {module + ".c", py::str("thkstub.c")}) {
        py::subprocess::CompletedProcess r =
            run_tool(py::list<py::str>{ow->wcc386, py::str("-bt=nt"), py::str("-zq"), py::str("-w0"), c}, outdir, env);
        log.append(py::percent(py::str("$ wcc386 %s -> %d\n%s"), c, r.returncode, out_of(r).strip()));
        if (r.returncode != 0) {
            res.error = py::str("compile ") + c + " failed";
            res.log = py::join(py::str("\n"), log);
            return res;
        }
    }

    // export directives: OpenWatcom decorates a public symbol by convention -- stdcall `_name@N`
    // (N = total parameter BYTES), cdecl `_name`. The authoritative N is in the object's PUBDEF
    // records; parse them so by-value structs (POINT=8, RECT=16) get the right N, not a guessed
    // nargs*4. Alias each clean export name to its decorated public symbol.
    py::bytes obj_bytes;
    {
        py::file f = py::open_rb(py::os::path::join(outdir, module + ".obj"));
        obj_bytes = f.read();
    }
    py::set<py::str> publics = omf::publics(obj_bytes);          // set of exact public symbol strings
    py::dict<py::str, py::str> by_clean;                         // clean name -> decorated symbol
    for (const py::str& sym : publics) {
        py::str base = sym.startswith("_") ? sym.slice(1, sym.size()) : sym;   // drop leading underscore
        base = py::split(base, py::str("@"), 1)[0];                          // drop @N
        by_clean.setdefault(base, sym);
    }
    py::list<py::str> exp_lines;
    for (const py::re::Match& mo : V(py::re::finditer(py::str("(?m)^([A-Za-z_][\\w \\*]*?)\\s+(\\w+)\\s+(\\w+)\\s*\\(([^;{]*)\\)\\s*\\{"), text))) {
        py::str conv = mo.group(2), name = mo.group(3), params = mo.group(4);
        if (name == "DllMain") continue;
        std::optional<py::str> internal = by_clean.get(name);    // exact symbol from PUBDEF (best)
        if (!internal || internal->empty()) {                    // fallback: compute the stdcall decoration
            py::str pl = params.strip();
            int64_t nbytes = 0;
            if (!(pl.empty() || pl == "void"))
                for (const py::str& p : V(_split_params(pl))) nbytes += _param_bytes(p);
            internal = (conv.endswith(py::str("APIV")) || conv == "__cdecl" || conv == "cdecl") ? py::str("_") + name
                                                                                                  : py::percent(py::str("_%s@%d"), name, nbytes);
        }
        exp_lines.append(py::percent(py::str("export %s='%s'"), name, *internal));
    }
    py::str lnk = py::os::path::join(outdir, py::str("link.lnk"));
    {
        py::file f = py::open(lnk, py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
        f.write(py::percent(py::str("system nt_dll\nname %s.dll\noption quiet\n"), module));
        f.write(py::percent(py::str("libpath %s\nlibrary kernel32,user32,gdi32\n"), libdir.replace(py::str("\\"), py::str("/"))));
        f.write(py::percent(py::str("file %s.obj\nfile thkstub.obj\n"), module));
        for (const py::str& l : exp_lines.vec()) f.write(l + "\n");
    }
    py::subprocess::CompletedProcess r = run_tool(py::list<py::str>{ow->wlink, py::str("@link.lnk")}, outdir, env);
    log.append(py::percent(py::str("$ wlink -> %d\n%s"), r.returncode, out_of(r).strip()));
    py::str dll = py::os::path::join(outdir, module + ".dll");
    if (py::os::path::exists(dll)) {
        int64_t nexp = 0;
        try {
            nexp = PE(dll).exports().size();
        } catch (const py::Exception&) {
            nexp = 0;
        } catch (const std::exception&) {
            nexp = 0;
        }
        res.ok = true;
        res.dll = dll;
        res.module = module;
        res.n_exports = nexp;
        res.bytes = py::os::path::getsize(dll);
        res.log = py::join(py::str("\n"), log);
        return res;
    }
    res.error = py::str("link produced no DLL");
    res.log = py::join(py::str("\n"), log);
    return res;
}

BuildResult build_flat_thunk(const py::str& srcdir_in, const std::optional<py::str>& outdir_in) {
    BuildResult res;
    py::str srcdir = py::os::path::abspath(srcdir_in);
    py::str outdir = py::os::path::abspath(outdir_in && !outdir_in->empty() ? *outdir_in : py::os::path::join(srcdir, py::str("_build")));
    py::os::makedirs(outdir, 0777, true);
    Toolchain tc = toolchain();
    struct Tool {
        const char* key;
        std::optional<py::str> path;
    };
    for (const Tool& t : {Tool{"thunk", tc.thunk}, Tool{"ml", tc.ml}, Tool{"cl", std::optional<py::str>(tc.cl)},
                          Tool{"link", std::optional<py::str>(tc.link)}, Tool{"lib", std::optional<py::str>(tc.lib)}}) {
        if (!t.path || t.path->empty() || !py::os::path::exists(*t.path)) {
            res.log = py::str("missing tool: ") + t.key + " (" + (t.path ? *t.path : py::str("None")) + ")";
            return res;
        }
    }
    Discovery disc = _discover_flat_thunk(srcdir);
    if (disc.error) {
        res.log = *disc.error;
        return res;
    }
    py::str module = disc.module;
    // stage sources into outdir (keep the tracked tree clean)
    for (const py::str& f : V(py::os::listdir(srcdir))) {
        py::str e = lower_ext(f);
        if (e == "thk" || e == "c" || e == "h" || e == "def" || e == "rc" || e == "ico" || e == "res")
            py::shutil::copy2(py::os::path::join(srcdir, f), outdir);
    }
    py::str thk = py::os::path::basename(disc.thk);
    py::str thkstem = std::get<0>(py::os::path::splitext(thk));
    py::list<py::str> cobjs;
    for (const py::str& c : disc.cfiles.vec()) cobjs.append(std::get<0>(py::os::path::splitext(py::os::path::basename(c))) + ".obj");
    py::str k32 = py::os::path::basename(disc.k32def);
    py::str k32stem = std::get<0>(py::os::path::splitext(k32));
    py::str moddef = py::os::path::basename(disc.moddef);
    std::optional<py::str> rc;
    if (disc.rc) rc = py::os::path::basename(*disc.rc);
    py::str resf = rc ? module + ".res" : py::str("");
    // INCLUDE: the compiler's own headers PLUS the Win32 SDK headers (msacm.h/vfw.h/commctrl.h...),
    // so a veneer's real SDK-typed signatures resolve -- the authentic build config.
    std::optional<py::str> win32inc;
    for (const py::str& r : V(_reference_roots())) {
        py::str p = py::os::path::join(r, py::str("WIN32SDK"), py::str("MSTOOLS"), py::str("INCLUDE"));
        if (py::os::path::isdir(p)) {
            win32inc = p;
            break;
        }
    }
    py::list<py::str> incdirs;
    if (win32inc && !win32inc->empty()) incdirs.append(*win32inc);
    incdirs.append(tc.include);
    py::list<py::str> inc_w, lib_w;
    for (const py::str& x : incdirs.vec()) inc_w.append(_winpath(x));
    for (const py::str& x : tc.lib_dirs.vec()) lib_w.append(_winpath(x));
    py::list<py::str> L{py::str("@echo off"), py::str("cd /d ") + _winpath(outdir),
                        py::str("set INCLUDE=") + py::join(py::str(";"), inc_w),
                        py::str("set LIB=") + py::join(py::str(";"), lib_w),
                        py::str("set PATH=") + _winpath(py::os::path::dirname(tc.cl)) + ";%PATH%",
                        py::percent(py::str("del %s.dll *.obj %s.asm %s 2>nul"), module, thkstem, resf),
                        py::percent(py::str("\"%s\" -o %s.asm %s > _t.log 2>&1"), _winpath(*tc.thunk), thkstem, thk),
                        py::percent(py::str("\"%s\" /c /W0 /DIS_32 /Fo%s_thk.obj %s.asm >> _t.log 2>&1"), _winpath(*tc.ml), thkstem, thkstem)};
    for (const py::str& c : disc.cfiles.vec()) {
        // /DWIN32 /D_WIN32: the SDK headers gate their 32-bit types on _WIN32 (msacm.h:288 etc.)
        L.append(py::percent(py::str("\"%s\" /c /W3 /DWIN32 /D_WIN32 %s >> _t.log 2>&1"), _winpath(tc.cl), py::os::path::basename(c)));
    }
    if (rc) L.append(py::percent(py::str("\"%s\" /r /fo %s %s >> _t.log 2>&1"), _winpath(tc.rc), resf, *rc));
    L.append(py::percent(py::str("\"%s\" /NOLOGO /DEF:%s /OUT:%s.lib /MACHINE:IX86 >> _t.log 2>&1"), _winpath(tc.lib), k32, k32stem));
    L.append(py::percent(py::str("\"%s\" /DLL /NOLOGO /NODEFAULTLIB /ENTRY:DllMain@12 /DEF:%s /OUT:%s.dll %s %s_thk.obj %s %s.lib kernel32.lib thunk32.lib >> _t.log 2>&1"),
                         _winpath(tc.link), moddef, module, py::join(py::str(" "), cobjs), thkstem, resf, k32stem));
    L.append(py::str("echo RC=%errorlevel% >> _t.log"));
    py::str bat = py::os::path::join(outdir, py::str("_build.bat"));
    {
        py::file f = py::open(bat, py::str("w"));
        f.write(py::join(py::str("\r\n"), L) + "\r\n");
    }
    try {
        py::subprocess::RunOptions o;
        py::subprocess::run(py::list<py::str>{py::str("cmd"), py::str("/c"), _winpath(bat)}, o);
    } catch (const py::Exception& e) {
        res.log = py::str("build run failed: ") + py::str(e.what());
        return res;
    }
    py::str tlog = py::os::path::join(outdir, py::str("_t.log"));
    py::str log;
    if (py::os::path::exists(tlog)) {
        py::file f = py::open(tlog, py::str("r"), py::str("latin1"));
        log = f.read_text();
    }
    py::str dll = py::os::path::join(outdir, module + ".dll");
    bool exists = py::os::path::exists(dll);
    res.ok = exists;
    if (exists) res.dll = dll;
    res.module = module;
    res.log = log;
    return res;
}

// ------------------------------------------------------------------ RECOVER (flat-thunk)
const py::dict<int64_t, std::tuple<py::str, std::optional<int64_t>>> _KERNEL32_ORDINALS{
    {17, std::make_tuple(py::str("CommonUnimpStub"), std::optional<int64_t>(0))},
    {20, std::make_tuple(py::str("DosFileHandleToWin32Handle"), std::optional<int64_t>(4))},
    {21, std::make_tuple(py::str("Win32HandleToDosFileHandle"), std::optional<int64_t>(4))},
    {22, std::make_tuple(py::str("DisposeLZ32Handle"), std::optional<int64_t>(4))},
    {41, std::make_tuple(py::str("ThunkInitLSF"), std::optional<int64_t>())},
    {89, std::make_tuple(py::str("FT_PrologPrime"), std::optional<int64_t>())},
    {90, std::make_tuple(py::str("QT_ThunkPrime"), std::optional<int64_t>())}};

// ------------------------------------------------------------------ stub analysis (x86-32)
namespace {
py::set<int64_t> range_set(int64_t lo, int64_t hi) {
    py::set<int64_t> s;
    for (int64_t i = lo; i < hi; ++i) s.add(i);
    return s;
}
py::set<int64_t> union_of(std::initializer_list<py::set<int64_t>> sets) {
    py::set<int64_t> s;
    for (const auto& x : sets) s.update(x);
    return s;
}
}  // namespace

const py::set<int64_t> _ARITH_RM = union_of({range_set(0x00, 0x04), range_set(0x08, 0x0C), range_set(0x10, 0x14), range_set(0x18, 0x1C),
                                             range_set(0x20, 0x24), range_set(0x28, 0x2C), range_set(0x30, 0x34), range_set(0x38, 0x3C)});
const py::set<int64_t> _ARITH_AX{0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D};
const py::set<int64_t> _MODRM_ONLY = union_of({_ARITH_RM, py::set<int64_t>{0x62, 0x63, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
                                                                          0x8C, 0x8D, 0x8E, 0x8F, 0xD0, 0xD1, 0xD2, 0xD3, 0xFE,
                                                                          0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF}});
const py::set<int64_t> _MODRM_IMM8{0x6B, 0x80, 0x82, 0x83, 0xC0, 0xC1, 0xC6};
const py::set<int64_t> _MODRM_IMMW{0x69, 0x81, 0xC7};

int64_t _modrm_len(const py::bytes& d, int64_t i) {
    if (i >= d.size()) return 0;
    int64_t modrm = d.raw()[static_cast<size_t>(i)] & 0xFF;
    int64_t mod = modrm >> 6, rm = modrm & 7;
    int64_t n = 1;
    if (mod == 3) return n;
    if (rm == 4) {                                           // SIB
        n += 1;
        int64_t base = i + 1 < d.size() ? (d[i + 1] & 7) : 0;
        if (mod == 0 && base == 5) n += 4;
    }
    if (mod == 1) n += 1;
    else if (mod == 2) n += 4;
    else if (mod == 0 && rm == 5) n += 4;
    return n;
}

int64_t _ilen(const py::bytes& d, int64_t i) {
    int64_t start = i;
    bool opsize = false;
    auto prefix = [](int64_t b) {
        return b == 0xF0 || b == 0xF2 || b == 0xF3 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x26 || b == 0x64 || b == 0x65 ||
               b == 0x66 || b == 0x67;
    };
    while (i < d.size() && prefix(d[i])) {
        if (d[i] == 0x66) opsize = true;
        i += 1;
    }
    if (i >= d.size()) return 0;
    int64_t immw = opsize ? 2 : 4;
    int64_t op = d[i];
    i += 1;
    if (op == 0x0F) {                                        // two-byte
        if (i >= d.size()) return 0;
        int64_t op2 = d[i];
        i += 1;
        if (0x80 <= op2 && op2 <= 0x8F) return i - start + immw;      // jcc rel32
        if (op2 == 0x1F || (0x90 <= op2 && op2 <= 0x9F) || op2 == 0xA3 || op2 == 0xA5 || op2 == 0xAB || op2 == 0xAD || op2 == 0xAF ||
            op2 == 0xB6 || op2 == 0xB7 || op2 == 0xBE || op2 == 0xBF || op2 == 0xB0 || op2 == 0xB1 || (0x40 <= op2 && op2 <= 0x4F))
            return i - start + _modrm_len(d, i);             // setcc/movzx/movsx/imul/cmov/bt/nop r/m
        if (op2 == 0xA2 || op2 == 0x31 || op2 == 0x77 || op2 == 0x05 || op2 == 0x34 || op2 == 0x0B)
            return i - start;                                // cpuid/rdtsc/emms/sysenter/ud2 -- no operand
        return 0;
    }
    if (_MODRM_ONLY.contains(op)) return i - start + _modrm_len(d, i);
    if (_MODRM_IMM8.contains(op)) return i - start + _modrm_len(d, i) + 1;
    if (_MODRM_IMMW.contains(op)) return i - start + _modrm_len(d, i) + immw;
    if (op == 0xF6 || op == 0xF7) {                          // grp3: test(/0,/1) has imm; rest don't
        int64_t ml = _modrm_len(d, i);
        int64_t reg = i < d.size() ? ((d[i] >> 3) & 7) : 7;
        return i - start + ml + ((reg == 0 || reg == 1) ? (op == 0xF6 ? 1 : immw) : 0);
    }
    if (op == 0xFF) return i - start + _modrm_len(d, i);    // grp5 inc/dec/call/jmp/push r/m
    if ((0x50 <= op && op <= 0x5F) || (0x90 <= op && op <= 0x9D) || (0x40 <= op && op <= 0x4F) || op == 0xC3 || op == 0xC9 ||
        op == 0xCB || op == 0xCC || op == 0xF4 || op == 0xF5 || op == 0xF8 || op == 0xF9 || op == 0xFA || op == 0xFB || op == 0xFC ||
        op == 0xFD || op == 0x98 || op == 0x99)
        return i - start;                                    // push/pop/inc/dec/nop/ret/leave/flags/one-byte
    if (0xB8 <= op && op <= 0xBF) return i - start + immw;  // mov reg,imm32
    if (0xB0 <= op && op <= 0xB7) return i - start + 1;    // mov reg8,imm8
    if (op == 0x68) return i - start + immw;                // push imm32
    if (op == 0x6A || op == 0xEB || (0x70 <= op && op <= 0x7F) || op == 0xA8 || op == 0x04 || op == 0x0C || op == 0x14 || op == 0x1C ||
        op == 0x24 || op == 0x2C || op == 0x34 || op == 0x3C)
        return i - start + 1;                                // push imm8 / jmp rel8 / jcc rel8 / test al / op al,imm8
    if (op == 0xE8 || op == 0xE9) return i - start + immw;  // call/jmp rel32
    if (op == 0xA9 || _ARITH_AX.contains(op)) return i - start + immw;    // test eax,imm / op eax,imm
    if (op == 0xA0 || op == 0xA1 || op == 0xA2 || op == 0xA3) return i - start + 4;   // mov moffs (address-sized)
    if (op == 0xC2) return i - start + 2;                   // ret imm16
    return 0;
}

int64_t _imagebase(const PE& pe) { return static_cast<int64_t>(pe.f.image_base); }

py::set<int64_t> _smapls_iat_rvas(const PE& pe) {
    uint64_t base = pe.f.image_base;
    const py::bytes& d = pe.d;
    py::set<int64_t> slot_vas;
    for (const pe_format::ImportModule& m : V(pe.f.import_table()))
        if (m.module.upper().contains(py::str("KERNEL32")))
            for (const pe_format::ImportSymbol& x : m.symbols.vec())
                if (x.name.startswith("SMapLS") || x.name.startswith("SUnMapLS"))
                    slot_vas.add(static_cast<int64_t>((base + static_cast<uint64_t>(x.slot)) & 0xFFFFFFFFull));
    py::set<int64_t> trig = py::set<int64_t>::from(slot_vas);
    // scan for `jmp dword ptr [slot]` (FF 25 abs32) thunks that forward to those IAT slots
    const std::string& raw = d.raw();
    for (size_t o = raw.find("\xff\x25", 0, 2); o != std::string::npos; o = raw.find("\xff\x25", o + 1, 2)) {
        if (o + 6 > raw.size()) continue;                    // (struct.error: a short operand is passed over)
        uint32_t operand = static_cast<uint32_t>(static_cast<unsigned char>(raw[o + 2])) |
                           (static_cast<uint32_t>(static_cast<unsigned char>(raw[o + 3])) << 8) |
                           (static_cast<uint32_t>(static_cast<unsigned char>(raw[o + 4])) << 16) |
                           (static_cast<uint32_t>(static_cast<unsigned char>(raw[o + 5])) << 24);
        if (slot_vas.contains(operand)) {
            std::optional<int64_t> thunk_rva = pe._file2rva(static_cast<int64_t>(o));
            if (thunk_rva) trig.add(static_cast<int64_t>((base + static_cast<uint64_t>(*thunk_rva)) & 0xFFFFFFFFull));
        }
    }
    return trig;
}

StubSignature _stub_signature(const PE& pe, int64_t frva, int64_t maxlen) {
    py::bytes d = pe.read_at(frva, maxlen);
    if (d.empty()) return StubSignature(std::nullopt, 0);
    const py::set<int64_t>& trig = pe.smapls_rvas;
    uint64_t base = pe.f.image_base;
    const std::string& s = d.raw();
    const int64_t len = d.size();
    auto u8 = [&](int64_t k) { return static_cast<int64_t>(static_cast<unsigned char>(s[static_cast<size_t>(k)])); };
    auto need = [&](int64_t at, int64_t n) {                 // struct.unpack of d[at:at+n] needs exactly n bytes
        if (at + n > len) throw py::StructError("unpack requires a buffer of " + std::to_string(n) + " bytes");
    };
    int64_t i = 0, nptr = 0;
    while (i < len) {
        int64_t b = u8(i);
        if (b == 0xC2) {                                     // ret imm16 -> stdcall epilogue
            need(i + 1, 2);
            int64_t n = u8(i + 1) | (u8(i + 2) << 8);
            if (n % 4 == 0 && n <= 256) return StubSignature(n, nptr);
            return StubSignature(std::nullopt, nptr);
        }
        if (b == 0xC3) return StubSignature(0, nptr);        // ret -> no stack cleanup (0 args, or cdecl)
        if (b == 0xCC) return StubSignature(std::nullopt, nptr);   // int3 padding before a ret -> unknown
        if (b == 0xE8 && i + 5 <= len) {                     // call rel32 -> SMapLS jump-thunk?
            int32_t rel = static_cast<int32_t>(static_cast<uint32_t>(u8(i + 1)) | (static_cast<uint32_t>(u8(i + 2)) << 8) |
                                               (static_cast<uint32_t>(u8(i + 3)) << 16) | (static_cast<uint32_t>(u8(i + 4)) << 24));
            uint64_t tgt = (base + static_cast<uint64_t>(frva) + static_cast<uint64_t>(i) + 5 + static_cast<uint64_t>(static_cast<int64_t>(rel))) &
                           0xFFFFFFFFull;
            if (trig.contains(static_cast<int64_t>(tgt))) nptr += 1;
        } else if (b == 0xFF && i + 6 <= len && u8(i + 1) == 0x15) {   // call dword ptr [abs] -> IAT SMapLS?
            int64_t operand = static_cast<int64_t>(static_cast<uint32_t>(u8(i + 2)) | (static_cast<uint32_t>(u8(i + 3)) << 8) |
                                                   (static_cast<uint32_t>(u8(i + 4)) << 16) | (static_cast<uint32_t>(u8(i + 5)) << 24));
            if (trig.contains(operand)) nptr += 1;
        }
        int64_t ln = _ilen(d, i);
        if (ln <= 0) break;                                  // opcode not modelled (e.g. inline jump table) -> try the epilogue mode-scan
        i += ln;
    }
    // Fallback: a function with an inline switch jump table defeats linear sweep. Its stdcall
    // epilogue `ret N` still appears at every return site with the SAME N, so pattern-scan for
    // `C2 NN 00` (N a plausible stdcall count) and accept N only if it repeats -- a lone data
    // coincidence will not, but a real multi-return epilogue does.
    std::optional<int64_t> n = _argbytes_by_epilogue(d);
    return StubSignature(n, nptr);
}

std::optional<int64_t> _argbytes_by_epilogue(const py::bytes& d) {
    const std::string& s = d.raw();
    std::vector<std::pair<int64_t, int64_t>> c;              // Counter: value -> count, in first-seen order
    std::vector<std::pair<int64_t, int64_t>> termd;          // value -> count of the ones after a terminator
    auto bump = [](std::vector<std::pair<int64_t, int64_t>>& v, int64_t k) {
        for (auto& e : v)
            if (e.first == k) {
                e.second += 1;
                return;
            }
        v.emplace_back(k, 1);
    };
    auto u8 = [&](size_t k) { return static_cast<int64_t>(static_cast<unsigned char>(s[k])); };
    for (size_t i = 0; i + 2 < s.size(); ++i) {
        if (u8(i) == 0xC2 && u8(i + 2) == 0x00) {
            int64_t n = u8(i + 1);
            if (n % 4 == 0 && 0 < n && n <= 256) {
                bump(c, n);
                if (i > 0) {
                    int64_t prev = u8(i - 1);
                    if ((0x58 <= prev && prev < 0x60) || prev == 0xC9) bump(termd, n);     // pop eax..edi / leave
                }
            }
        }
    }
    if (c.empty()) return std::nullopt;
    std::pair<int64_t, int64_t> best = c[0];
    for (const auto& e : c)
        if (e.second > best.second) best = e;                // most_common(1): the first of the largest counts
    if (best.second >= 2) return best.first;
    if (termd.size() == 1) return termd[0].first;            // exactly one terminator-preceded candidate
    return std::nullopt;
}

std::tuple<std::optional<py::str>, py::list<py::str>> _thunk_dll_targets(const PE& pe, const py::str& module) {
    const std::string& d = pe.d.raw();
    py::str up = module.upper();
    py::list<py::str> cands;
    py::set<py::str> seen;
    // rb"[A-Za-z_][A-Za-z0-9_]{1,10}\.[Dd][Ll][Ll]": a name of 2 to 11 characters (not starting with a digit), then ".dll"
    auto word = [&](size_t k) {
        unsigned char c = static_cast<unsigned char>(d[k]);
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
    };
    auto first = [&](size_t k) {
        unsigned char c = static_cast<unsigned char>(d[k]);
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
    };
    size_t pos = 0;
    while (pos < d.size()) {
        bool matched = false;
        if (first(pos)) {
            size_t k = pos + 1;
            while (k < d.size() && word(k) && k - (pos + 1) < 11) ++k;
            size_t run = k - (pos + 1);                      // the word characters after the first (greedy, at most 11 here)
            if (run >= 1 && run <= 10 && k + 4 <= d.size() && d[k] == '.' && (d[k + 1] | 0x20) == 'd' && (d[k + 2] | 0x20) == 'l' &&
                (d[k + 3] | 0x20) == 'l') {
                py::str nm = py::bytes(d.substr(pos, k + 4 - pos)).decode("latin1").upper();
                if (!seen.contains(nm)) {
                    seen.add(nm);
                    cands.append(nm);
                }
                pos = k + 4;
                matched = true;
            }
        }
        if (!matched) ++pos;
    }
    // a flat-thunk's 16-bit partner is almost always the 32-bit name with a trailing "32" dropped
    // (AVICAP32->AVICAP, MSACM32->MSACM); a few well-known ones rename (COMCTL32->COMMCTRL,
    // MSVFW32->MSVIDEO). Take the partner name only if it is actually present in the image --
    // authoritative -- else fall back to the lone non-system .DLL string; else leave it ambiguous.
    static const py::dict<py::str, py::str> _ALIAS{
        {py::str("COMCTL32"), py::str("COMMCTRL.DLL")}, {py::str("MSVFW32"), py::str("MSVIDEO.DLL")},
        {py::str("WINMM"), py::str("MMSYSTEM.DLL")},    {py::str("SHELL32"), py::str("SHELL.DLL")},
        {py::str("COMDLG32"), py::str("COMMDLG.DLL")},  {py::str("MPR"), py::str("PMSPL.DLL")},
        {py::str("VERSION"), py::str("VER.DLL")},       {py::str("WSOCK32"), py::str("WINSOCK.DLL")},
        {py::str("OLE32"), py::str("OLE2.DLL")}};
    py::list<py::str> candidates_16;
    if (_ALIAS.contains(up)) candidates_16.append(_ALIAS.at(up));
    candidates_16.append((up.endswith(py::str("32")) ? up.slice(0, up.size() - 2) : up) + ".DLL");
    for (const py::str& c16 : candidates_16.vec())
        if (cands.contains(c16)) return std::make_tuple(std::optional<py::str>(c16), cands);
    py::set<py::str> sysdlls{py::str("KERNEL32.DLL"), py::str("USER32.DLL"),   py::str("GDI32.DLL"),    py::str("ADVAPI32.DLL"),
                             py::str("WINMM.DLL"),    py::str("SHELL32.DLL"),  py::str("COMDLG32.DLL"), py::str("COMCTL32.DLL"),
                             py::str("OLE32.DLL"),    py::str("OLEAUT32.DLL"), py::str("RPCRT4.DLL"),   py::str("VERSION.DLL"),
                             py::str("LZ32.DLL"),     py::str("MPR.DLL"),      py::str("WSOCK32.DLL"),  py::str("IMM32.DLL"),
                             py::str("WINSPOOL.DRV"), py::str("DDEML.DLL"),    py::str("MSVCRT.DLL"),   py::str("MSVCRT20.DLL"),
                             py::str("CRTDLL.DLL"),   py::str("WOW32.DLL"),    py::str("DCIMAN32.DLL"), py::str("MMSYSTEM.DLL"),
                             up + ".DLL"};
    py::list<py::str> others;
    for (const py::str& c : cands.vec())
        if (!sysdlls.contains(c)) others.append(c);
    std::optional<py::str> target;
    if (others.size() == 1) target = others[0];
    return std::make_tuple(target, cands);
}

py::dict<py::str, SdkProto> _load_sdk_protos(bool win16) {
    py::dict<py::str, SdkProto> out;
    const prototypes::Table& table = prototypes::table();
    const py::dict<py::str, prototypes::Prototype>& which = win16 ? table.w16 : table.w32;
    static const py::str void_ = py::str("void"), winapi = py::str("WINAPI"), documented = py::str("documented");
    for (const py::str& name : which) {
        const prototypes::Prototype& p = which.at(name);
        // ", ".join((t + " " + n).strip() for t, n in p["parameters"]) or "void", built in one string
        std::u32string params;
        for (const auto& tn : p.parameters.vec()) {
            std::u32string one = std::get<0>(tn).u32() + U' ' + std::get<1>(tn).u32();
            size_t lo = 0, hi = one.size();
            while (lo < hi && py::str::space_(one[lo])) ++lo;
            while (hi > lo && py::str::space_(one[hi - 1])) --hi;
            if (!params.empty() || &tn != &p.parameters.vec().front()) params += U", ";
            params.append(one, lo, hi - lo);
        }
        SdkProto sp;
        sp.ret = p.returns;
        sp.params = params.empty() ? void_ : py::str(std::move(params));
        sp.conv = !p.marker.empty() ? p.marker : winapi;
        sp.header = p.header.upper();
        sp.doc = documented;
        out.set(name, sp);
    }
    return out;
}

std::tuple<std::optional<py::str>, bool> _sig_params(const StubSignature& sig, const std::optional<SdkProto>& proto) {
    if (proto) return std::make_tuple(std::optional<py::str>(proto->params), false);
    std::optional<int64_t> argbytes = std::get<0>(sig);
    int64_t nptr = std::get<1>(sig);
    if (!argbytes) return std::make_tuple(std::optional<py::str>(), false);
    int64_t nargs = py::floordiv(*argbytes, 4);
    if (nargs == 0) return std::make_tuple(std::optional<py::str>(py::str("void")), nptr == 0);
    py::list<py::str> ps;
    for (int64_t i = 0; i < nargs; ++i)
        ps.append(i < nptr ? py::percent(py::str("LPVOID p%d"), i + 1) : py::percent(py::str("DWORD a%d"), i + 1));
    return std::make_tuple(std::optional<py::str>(py::join(py::str(", "), ps)), true);
}

py::str _sig_ret(const std::optional<SdkProto>& proto) { return (proto && !proto->ret.empty()) ? proto->ret : py::str("DWORD"); }

py::str _sig_conv(const std::optional<SdkProto>& proto) { return (proto && !proto->conv.empty()) ? proto->conv : py::str("WINAPI"); }

const py::set<py::str> _IN_WINDOWS_H{py::str("WINUSER.H"), py::str("WINGDI.H"), py::str("WINBASE.H"), py::str("WINREG.H"),
                                     py::str("WINNLS.H"),  py::str("WINNT.H"),  py::str("WINDOWS.H"), py::str("WINDEF.H"),
                                     py::str("WINCON.H"),  py::str("WINSVC.H"), py::str("WINERROR.H")};

const py::dict<py::str, py::list<py::str>> _HEADER_PREREQ{
    {py::str("msacm.h"), py::list<py::str>{py::str("mmsystem.h"), py::str("mmreg.h")}},
    {py::str("vfw.h"), py::list<py::str>{py::str("mmsystem.h"), py::str("commdlg.h")}},
    {py::str("digitalv.h"), py::list<py::str>{py::str("mmsystem.h")}},
    {py::str("mmreg.h"), py::list<py::str>{py::str("mmsystem.h")}},
    {py::str("mciavi.h"), py::list<py::str>{py::str("mmsystem.h"), py::str("vfw.h")}},
    {py::str("prsht.h"), py::list<py::str>{py::str("commctrl.h")}},
    {py::str("commctrl.h"), py::list<py::str>{py::str("objbase.h")}}};      // IStream in ImageList_Read/Write, prop sheets

const py::set<py::str> _CKW{py::str("void"),   py::str("char"),  py::str("int"),    py::str("long"),  py::str("short"),
                            py::str("unsigned"), py::str("signed"), py::str("double"), py::str("float"), py::str("const"),
                            py::str("struct"), py::str("union"), py::str("enum"),   py::str("far"),   py::str("near"),
                            py::str("huge"),   py::str("volatile")};

py::list<py::str> _split_params(const py::str& s) {
    py::list<py::str> out;
    int64_t depth = 0;
    std::u32string cur;
    for (char32_t ch : s.u32()) {
        if (ch == U'(') depth += 1;
        else if (ch == U')') depth -= 1;
        if (ch == U',' && depth == 0) {
            out.append(py::str(cur));
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    if (!py::str(cur).strip().empty()) out.append(py::str(cur));
    return out;
}

py::str _def_params(const py::str& params) {
    py::str p = params.strip();
    if (p.empty() || p.lower() == "void") return py::str("void");
    py::list<py::str> out;
    int64_t i = 0;
    for (const py::str& raw_part : V(_split_params(p))) {
        i += 1;
        py::str part = raw_part.strip();
        if (part == "..." || part.empty()) {
            out.append(part.empty() ? py::str("void") : part);
            continue;
        }
        // the parameter is named iff its trailing identifier (ignoring a glued `*`, e.g. `int*pn`
        // or `UINT FAR *puMxId`) is a mixed/lower-case name -- SDK types are ALL-CAPS or keywords.
        bool is_type;
        if (py::split(part).size() == 1) {
            // a single token is always a bare type (HWND, LPCSTR, int, va_list) -- no name present
            is_type = true;
        } else {
            std::optional<py::re::Match> m = py::re::search(py::str("([A-Za-z_]\\w*)\\s*$"), part);
            py::str last = m ? m->group(1) : py::str("");
            // a type token is a keyword or a multi-char ALL-CAPS name (HWND, LPSTR); a single upper
            // letter (X, Y, W, H) is a legitimate parameter name, so require len>=2 for the type test.
            is_type = last.empty() || _CKW.contains(last.lower()) || (last.isupper() && last.size() >= 2);
        }
        out.append(!is_type ? part : part + py::percent(py::str(" a%d"), i));
    }
    return py::join(py::str(", "), out);
}

py::list<py::str> _needed_includes(const py::list<SdkProto>& protos_used) {
    py::list<py::str> hdrs;
    py::set<py::str> seen;
    std::function<void(const py::str&)> add = [&](const py::str& h) {
        py::str hl = h.lower();
        if (seen.contains(hl) || _IN_WINDOWS_H.contains(h.upper())) return;
        for (const py::str& pre : V(_HEADER_PREREQ.get(hl, py::list<py::str>()))) add(pre);
        seen.add(hl);
        hdrs.append(hl);
    };
    for (const SdkProto& p : protos_used.vec())
        if (!p.header.empty()) add(p.header);
    return hdrs;
}

bool _is_thunk_data(const py::str& nm) {
    return nm.endswith(py::str("ThunkData32")) || nm.endswith(py::str("ThunkData")) || nm.contains(py::str("_ThunkData")) ||
           nm.endswith(py::str("ThkSL")) || py::re::match(py::str("^[A-Za-z0-9]+Thk(SL)?_?ThunkData"), nm).has_value();
}

// ------------------------------------------------------------------ RECOVER (flat-thunk)
Recovered recover_flat_thunk(const py::str& pe_path, const py::str& outdir) {
    Recovered rec;
    Binary b = load(pe_path);
    if (!std::holds_alternative<PE>(b)) {
        rec.error = py::str("not a PE: ") + pe_path;
        return rec;
    }
    PE& pe = std::get<PE>(b);
    py::str module = std::get<0>(py::os::path::splitext(py::os::path::basename(pe_path))).lower();
    py::os::makedirs(outdir, 0777, true);
    py::list<std::tuple<int64_t, py::str>> exps = pe.exports();
    py::dict<py::str, py::list<py::str>> imps = pe.imports();
    py::list<py::str> k32 = *k32_of(imps);

    // locate SMapLS in the IAT so a `call SMapLS` in a stub is recognisable (marks a pointer arg).
    py::dict<py::str, int64_t> addrs = pe.export_addrs();
    pe.smapls_rvas = _smapls_iat_rvas(pe);
    std::vector<int64_t> sa = addrs.values();
    std::sort(sa.begin(), sa.end());

    auto extent = [&](int64_t rva) -> int64_t {
        auto it = std::upper_bound(sa.begin(), sa.end(), rva);
        int64_t v = it != sa.end() ? (*it - rva) : 8192;
        return std::min<int64_t>(v, 8192);                   // bound the walk to this function
    };

    py::dict<py::str, StubSignature> sigs;
    for (const auto& kv : addrs.items()) sigs.set(std::get<0>(kv), _stub_signature(pe, std::get<1>(kv), extent(std::get<1>(kv))));
    auto target = _thunk_dll_targets(pe, module);
    std::optional<py::str> target16 = std::get<0>(target);
    py::list<py::str> dll_cands = std::get<1>(target);

    // 1) module .def -- exports, verbatim
    {
        py::file f = py::open(py::os::path::join(outdir, module + ".def"), py::str("w"));
        f.write(py::percent(py::str("LIBRARY   %s\nEXPORTS\n"), module.upper()));
        for (const auto& t : exps.vec()) f.write(py::percent(py::str("    %-18s @%d\n"), std::get<1>(t), std::get<0>(t)));
    }

    // 2) <mod>k32.def -- the KERNEL32 by-ordinal helpers this binary imports, named via the oracle
    py::list<int64_t> ords;
    {
        std::vector<int64_t> v;
        for (const py::str& n : k32.vec())
            if (n.startswith("@")) v.push_back(py::int_parse(n.slice(1, n.size())));
        std::sort(v.begin(), v.end());
        ords = py::list<int64_t>(v);
    }
    py::list<int64_t> unknown;
    for (int64_t o : ords.vec())
        if (!_KERNEL32_ORDINALS.contains(o)) unknown.append(o);
    {
        py::file f = py::open(py::os::path::join(outdir, module + "k32.def"), py::str("w"));
        f.write(py::percent(py::str("; KERNEL32 by-ordinal flat-thunk / handle helpers imported by %s.\n"), module.upper()));
        f.write(py::str("; Names from the Windows Me symbol file (Geoff Chappell). NONAME = by-ordinal import.\n"));
        f.write(py::str("; NOTE: ThunkInitLSF@41 is the Microsoft-INTERNAL connect (never in a public SDK); we\n"));
        f.write(py::str(";       connect via the public ThunkConnect32 instead -- see the engine docs.\n"));
        f.write(py::str("LIBRARY KERNEL32\nEXPORTS\n"));
        for (int64_t o : ords.vec()) {
            if (o == 41) continue;      // internal ThunkInitLSF -- deliberately not imported (use ThunkConnect32)
            std::tuple<py::str, std::optional<int64_t>> na =
                _KERNEL32_ORDINALS.get(o, std::make_tuple(py::percent(py::str("Kernel32_Ord%d"), o), std::optional<int64_t>()));
            py::str dec = std::get<1>(na) ? py::percent(py::str("%s@%d"), std::get<0>(na), *std::get<1>(na)) : std::get<0>(na);
            f.write(py::percent(py::str("    %-28s @%d NONAME\n"), dec, o));
        }
    }

    py::dict<py::str, SdkProto> protos = _load_sdk_protos();
    auto proto_of = [&](const py::str& n) -> std::optional<SdkProto> {
        if (!protos.contains(n)) return std::nullopt;
        return protos.at(n);
    };
    py::str dll16 = target16 && !target16->empty() ? *target16 : py::str("?16.DLL");   // real 16-bit target when unambiguous, else placeholder
    int64_t n_arity = 0, n_ptr = 0, n_proto = 0;

    // 3) .thk -- one thunked function per export; arity recovered from the stub's stdcall epilogue,
    //    pointer args (from SMapLS) flagged with input/output/inout markers for the thunk compiler.
    {
        py::file f = py::open(py::os::path::join(outdir, module + ".thk"), py::str("w"));
        f.write(py::percent(py::str("/* %s.thk -- flat-thunk script. Function list from the export table; per-function\n"
                                    " * ARITY recovered from the stub's `ret N` (stdcall) and pointer args from the SMapLS\n"
                                    " * marshalling. Pointer input/output DIRECTION is defaulted to input -- curate where a\n"
                                    " * function writes back through a pointer (mark output/inout). 16-bit target: %s. */\n"
                                    "enablemapdirect3216 = true;\n\n"
                                    "typedef unsigned short WORD;  typedef unsigned long DWORD;  typedef int INT;\n"
                                    "typedef long LONG;  typedef char *LPSTR;  typedef void *LPVOID;\n"
                                    "typedef void VOID;  typedef int HFILE;\n\n"),
                            module, dll16));
        // 16-bit Windows has no wide APIs: A/W exports share ONE 16-bit target (the ANSI form), so
        // emit each base once, preferring the A variant's (LPSTR) prototype. Thunk-data exports are
        // not functions -- note them and skip.
        py::dict<py::str, std::tuple<int64_t, py::str>> by_base;
        for (const auto& t : exps.vec()) {
            const py::str& nm = std::get<1>(t);
            if (_is_thunk_data(nm)) continue;
            py::str bs = (nm.endswith(py::str("A")) || nm.endswith(py::str("W"))) ? nm.slice(0, nm.size() - 1) : nm;
            // prefer an A export (or a suffix-less one) as the canonical 16-bit target
            if (!by_base.contains(bs) || (nm.endswith(py::str("A")) && !std::get<1>(by_base.at(bs)).endswith(py::str("A"))))
                by_base.set(bs, std::make_tuple(std::get<0>(t), nm));
        }
        py::list<py::str> td;
        for (const auto& t : exps.vec())
            if (_is_thunk_data(std::get<1>(t))) td.append(std::get<1>(t));
        if (!td.empty())
            f.write(py::percent(py::str("/* thunk-data exports (emitted by the thunk compiler, not APIs): %s */\n\n"), py::join(py::str(", "), td)));
        auto items = by_base.items();
        std::stable_sort(items.begin(), items.end(),
                         [](const auto& a, const auto& b2) { return std::get<0>(std::get<1>(a)) < std::get<0>(std::get<1>(b2)); });
        for (const auto& kv : items) {
            const py::str& bs = std::get<0>(kv);
            int64_t ordv = std::get<0>(std::get<1>(kv));
            const py::str& nm = std::get<1>(std::get<1>(kv));
            std::optional<SdkProto> proto = proto_of(nm);
            if (!proto) proto = proto_of(bs + "A");
            if (!proto) proto = proto_of(bs);
            StubSignature sg = sigs.get(nm, StubSignature(std::nullopt, 0));
            auto pk = _sig_params(sg, proto);
            py::str ret = _sig_ret(proto);
            if (!std::get<0>(pk)) {
                f.write(py::percent(py::str("/* @%d %s -- stub did not resolve; TODO: signature + pointer directions */\n%s %s16(void)\n{\n}\n\n"),
                                    ordv, nm, ret, bs));
                continue;
            }
            f.write(py::percent(py::str("/* @%d %s -- arity recovered from stub%s */\n%s %s16(%s)\n{\n}\n\n"), ordv, nm,
                                py::str(proto ? " (SDK prototype)" : ""), ret, bs, *std::get<0>(pk)));
        }
    }

    // 4) veneer .c -- DllMain (real 16-bit target) + a thunk-through per export with its recovered
    //    signature. Bodies remain scaffolds (the marshal/thunk/return is filled during curation).
    {
        py::list<SdkProto> used;
        for (const auto& t : exps.vec())
            if (protos.contains(std::get<1>(t))) used.append(protos.at(std::get<1>(t)));
        py::list<py::str> inc = _needed_includes(used);
        std::string inc_block;
        for (const py::str& h : inc.vec()) inc_block += "#include <" + h.utf8() + ">\n";
        py::file f = py::open(py::os::path::join(outdir, module + ".c"), py::str("w"));
        f.write(py::percent(
            py::str("/* %s.c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.\n"
                    " * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args\n"
                    " * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral\n"
                    " * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not\n"
                    " * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */\n"
                    "#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n%s\n"
                    "extern BOOL __stdcall %s_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);\n\n"
                    "/* DLL entry -- connects the 32:16 thunk to the 16-bit target %s */\n"
                    "BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)\n{\n"
                    "    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);\n"
                    "        if (!%s_ThunkConnect32(\"%s\", \"%s.DLL\", h, r)) return FALSE; }\n"
                    "    else if (r == DLL_PROCESS_DETACH) %s_ThunkConnect32(\"%s\", \"%s.DLL\", h, r);\n"
                    "    return TRUE;\n}\n\n"),
            module, py::str(inc_block), module, dll16, module, dll16, module.upper(), module, dll16, module.upper()));
        for (const auto& t : exps.vec()) {
            int64_t ordv = std::get<0>(t);
            const py::str& nm = std::get<1>(t);
            if (_is_thunk_data(nm)) {                        // data export, emitted by the thunk compiler
                f.write(py::percent(py::str("/* @%d %s -- thunk-data table (generated by the thunk compiler from %s.thk;\n"
                                            " * exported by name in the .def, no C definition here) */\n\n"),
                                    ordv, nm, module));
                continue;
            }
            if (!py::re::match(py::str("^[A-Za-z_]\\w*$"), nm)) {   // decorated/non-identifier export (e.g. _f@4)
                f.write(py::percent(py::str("/* @%d %s -- decorated/non-identifier export name; kept in the .def, no C\n"
                                            " * definition here (the name is not a legal C identifier) */\n\n"),
                                    ordv, nm));
                continue;
            }
            // #undef the name before defining it: many SDK names are macros in the headers -- A/W
            // aliases (`#define PlaySound PlaySoundA`) and function-like macros (`ICCompress` ->
            // ICSendMessage in vfw.h) -- which would corrupt our real definition. A no-op when the
            // name is not a macro.
            py::str undef = py::str("#undef ") + nm + "\n";
            std::optional<SdkProto> proto = proto_of(nm);
            StubSignature sig = sigs.get(nm, StubSignature(std::nullopt, 0));
            auto pk = _sig_params(sig, proto);
            py::str ret = _sig_ret(proto);
            if (proto) n_proto += 1;
            if (!std::get<0>(pk)) {                          // stub unresolved -> honest scaffold
                f.write(py::percent(py::str("/* @%d %s -- API (mark documented/undocumented); scaffold. "
                                            "TODO: signature + marshal + thunk + return */\n"
                                            "%sDWORD WINAPI %s(void) { return 0; }\n\n"),
                                    ordv, nm, undef, nm));
                continue;
            }
            n_arity += 1;
            std::optional<int64_t> argbytes = std::get<0>(sig);
            int64_t nptr = std::get<1>(sig);
            n_ptr += nptr;
            py::str tag;
            if (proto && proto->doc == "documented") {
                tag = py::str("documented");
            } else {
                if (!argbytes) throw py::TypeError("unsupported operand type(s) for //: 'NoneType' and 'int'");
                tag = py::percent(py::str("%d-arg"), py::floordiv(*argbytes, 4)) + (nptr ? py::percent(py::str(", %d ptr"), nptr) : py::str(""));
            }
            py::str body = ret.strip().lower() == "void" ? py::str("{ }") : py::str("{ return 0; }");   // void: no return value
            f.write(py::percent(py::str("/* @%d %s -- %s; scaffold. TODO: marshal args, thunk to %s, return */\n%s%s %s %s(%s) %s\n\n"), ordv,
                                nm, tag, dll16, undef, ret, _sig_conv(proto), nm, _def_params(*std::get<0>(pk)), body));
        }
    }

    rec.module = module;
    rec.n_exports = exps.size();
    rec.k32_ordinals = ords;
    rec.unknown_ordinals = unknown;
    rec.outdir = outdir;
    rec.target16 = target16;
    rec.dll_candidates = dll_cands;
    rec.n_signatures_recovered = n_arity;
    rec.n_pointer_args = n_ptr;
    rec.n_sdk_prototypes = n_proto;
    rec.recovered = py::list<py::str>{module + ".def", module + "k32.def",
                                      py::percent(py::str("%d/%d signatures (stub arity)"), n_arity, exps.size())};
    rec.scaffolded = py::list<py::str>{module + ".thk", module + ".c"};
    return rec;
}

// ------------------------------------------------------------------ RECOVER from a decompile (logic modules)
std::optional<py::str> _opt(const py::list<py::str>& argv, const py::str& name) {
    if (argv.contains(name) && argv.index(name) + 1 < argv.size()) return argv[argv.index(name) + 1];
    return std::nullopt;
}

std::optional<int64_t> main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
        py::print(py::str(_DOC));
        return int64_t(0);
    }
    py::str mod = argv[0];
    std::optional<py::str> found = !py::os::path::exists(mod) ? corpus::retail(mod) : std::optional<py::str>(mod);
    if (!found) throw py::TypeError("stat: path should be string, bytes, os.PathLike or integer, not NoneType");
    py::str path = *found;
    Classification c = classify(path);
    py::print(py::percent(py::str("=== classify %s ==="), py::os::path::basename(path)));
    for (const auto& kv : V(c.items())) py::print(py::percent(py::str("  %-12s %s"), std::get<0>(kv), std::get<1>(kv)));
    std::optional<py::str> recdir = _opt(argv, py::str("--recover"));
    if (recdir) {
        py::print(py::percent(py::str("=== recover (flat-thunk) -> %s ==="), *recdir));
        Recovered rr = recover_flat_thunk(path, *recdir);
        if (rr.error) {
            py::print(py::str("  ") + *rr.error);
            return std::nullopt;
        }
        py::str k32s = py::str("[") + py::join(py::str(", "), py::list<py::str>::from(([&] {
                           std::vector<py::str> v;
                           for (int64_t o : rr.k32_ordinals.vec()) v.push_back(D(o));
                           return v;
                       })())) + "]";
        py::str unk = py::str("");
        if (!rr.unknown_ordinals.empty()) {
            std::vector<py::str> v;
            for (int64_t o : rr.unknown_ordinals.vec()) v.push_back(D(o));
            unk = py::str("  UNKNOWN=[") + py::join(py::str(", "), py::list<py::str>::from(v)) + "]";
        }
        py::print(py::percent(py::str("  module=%s  exports=%d  KERNEL32 ordinals=%s%s"), rr.module, rr.n_exports, k32s, unk));
        py::str cand = py::str("");
        if (!(rr.target16 && !rr.target16->empty())) cand = py::str("   candidates=") + py::repr(rr.dll_candidates);
        py::print(py::percent(py::str("  16-bit target: %s%s"), rr.target16 && !rr.target16->empty() ? *rr.target16 : py::str("?16.DLL (ambiguous)"), cand));
        py::print(py::percent(py::str("  signatures recovered from stubs: %d/%d  (%d pointer args, %d SDK prototypes)"), rr.n_signatures_recovered,
                              rr.n_exports, rr.n_pointer_args, rr.n_sdk_prototypes));
        py::print(py::str("  recovered (from binary): ") + py::join(py::str(", "), rr.recovered));
        py::print(py::str("  scaffolded (need curation): ") + py::join(py::str(", "), rr.scaffolded));
        if (argv.contains(py::str("--ow-compile"))) {         // verify with the from-source OpenWatcom cc
            CompileOw cr = compile_ow(*recdir);
            if (cr.error) {
                py::print(py::str("  ow-compile: ") + *cr.error);
            } else {
                for (const auto& t : cr.results.vec())
                    py::print(py::percent(py::str("  ow-compile %-16s %s%s"), std::get<0>(t), py::str(std::get<1>(t) == 0 ? "OK" : "FAIL"),
                                          !std::get<2>(t).empty() ? py::str("  ") + std::get<2>(t) : py::str("")));
            }
        }
    }
    std::optional<py::str> built = _opt(argv, py::str("--diff"));
    std::optional<py::str> srcdir = _opt(argv, py::str("--build"));
    std::optional<py::str> owdir = _opt(argv, py::str("--ow-build"));
    auto tail12 = [](const py::str& log) {
        py::list<py::str> lines = py::splitlines(log);
        py::list<py::str> out;
        for (const py::str& l : V(lines.slice(-12, lines.size()))) out.append(py::str("    ") + l);
        return py::join(py::str("\n"), out);
    };
    if (owdir) {
        py::print(py::percent(py::str("=== build (flat-thunk, from-source OpenWatcom) from %s ==="), *owdir));
        BuildResult res = build_flat_thunk_ow(*owdir);
        if (!res.ok) {
            py::print(py::str("  BUILD FAILED: ") + (res.error ? *res.error : py::str("")) + "\n" + tail12(res.log ? *res.log : py::str("")));
            return std::nullopt;
        }
        built = res.dll;
        py::print(py::percent(py::str("  built %s (%d bytes, %d exports) -- no Microsoft tools"), py::os::path::basename(*built), res.bytes,
                              res.n_exports));
    }
    if (srcdir) {
        py::print(py::percent(py::str("=== build (flat-thunk) from %s ==="), *srcdir));
        BuildResult res = build_flat_thunk(*srcdir);
        if (!res.ok) {
            py::print(py::str("  BUILD FAILED:\n") + tail12(res.log ? *res.log : py::str("")));
            return std::nullopt;
        }
        built = res.dll;
        py::print(py::percent(py::str("  built %s (%d bytes)"), py::os::path::basename(*built), py::os::path::getsize(*built)));
    }
    if (built) {
        py::print(py::percent(py::str("=== diff %s vs %s ==="), py::os::path::basename(*built), py::os::path::basename(path)));
        py::print(_fmt_diff(bindiff(*built, path)));
    }
    return std::nullopt;
}

}  // namespace codebreaker::build::flat_thunk
