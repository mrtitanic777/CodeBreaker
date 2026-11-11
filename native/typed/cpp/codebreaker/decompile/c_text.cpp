// codebreaker/decompile/c_text.cpp -- typed C++ of codebreaker.decompile.c_text (see c_text.hpp).
//
// c_text.py -- make the decompiler's output into C a compiler accepts and that means what the original meant.
//
// The C back end (translate/backends/c, native_l2) is reached with the library's values (native_l2::Wire): the facts a
// function is written with go in as a Wire dict (ModuleFacts -> facts_wire) and what the back end reports comes back out
// of it (read_back). Everything else here is typed.
//
// Not carried over: compilable16's result is not kept in the build cache (Python kept it pickled; a pickle is Python's own
// format and nothing in C++ reads one). The decompiler's own output, the expensive step, is still kept (decompiler.cpp).

#include "codebreaker/decompile/c_text.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/build/data.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/translate/backends/c.hpp"

namespace codebreaker::decompile::c_text {

namespace cback = codebreaker::translate::backends::c;
namespace native_l2 = codebreaker::translate::native_l2;
using native_l2::Wire;

namespace {
#include "codebreaker/decompile/c_text_consts.inc"

py::str S(const char* s) { return py::str(s); }
py::str S(const std::string& s) { return py::str(s); }

template <class... A>
py::str fmt(const char* f, A... a) {
    char buf[1024];
    std::snprintf(buf, sizeof buf, f, a...);
    return py::str(std::string(buf));
}

py::list<py::str> L(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) out.append(S(x));
    return out;
}

// py "%d" of a number
py::str D(int64_t v) { return py::str(std::to_string(v)); }
// py "%x"
py::str X(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}
py::str X4(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%04llx", static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}

std::string join(const std::vector<py::str>& v, const char* sep) {
    std::string out;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) out += sep;
        out += v[i].utf8();
    }
    return out;
}
py::str joins(const std::vector<py::str>& v, const char* sep) { return py::str(join(v, sep)); }

py::str lines_text(const std::vector<py::str>& v) { return joins(v, "\n"); }

std::vector<py::str> vec(const py::list<py::str>& l) { return l.vec(); }

// V helpers
Wire Wi(int64_t v) { return Wire(static_cast<long long>(v)); }
Wire Ws(const py::str& s) { return Wire(s.utf8()); }
Wire Wstrs(const py::list<py::str>& l) {
    std::vector<Wire> items;
    for (const py::str& s : l.vec()) items.push_back(Ws(s));
    return Wire::list(items);
}
Wire Wstr_set(const py::set<py::str>& l) {
    std::vector<Wire> items;
    for (const py::str& s : l) items.push_back(Ws(s));
    return Wire::set(items);
}
Wire Wint_set(const py::set<int64_t>& l) {
    std::vector<Wire> items;
    for (int64_t s : l) items.push_back(Wi(s));
    return Wire::set(items);
}
py::str as_str(const Wire& v) { return py::str(v.str()); }
std::optional<py::str> opt_str(const Wire& v) {
    if (v.is_none()) return std::nullopt;
    return as_str(v);
}
Wire Wopt(const std::optional<py::str>& s) { return s ? Ws(*s) : Wire(); }
Wire Wopt_i(const std::optional<int64_t>& s) { return s ? Wi(*s) : Wire(); }

py::str group(const std::optional<py::re::Match>& m, int64_t g) { return m->group(g); }

// a function's L2 document as the C back end takes it: exactly as written (see FunctionNotes::l2_exact)
Wire doc_wire(const core16::FunctionNotes& f) { return f.l2_exact ? *f.l2_exact : native_l2::to_wire(translate::ir::Val(*f.l2)); }

}  // namespace

// ---- constants ------------------------------------------------------------------------------------

const py::list<py::str> CFLAGS = L({"-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecd"});
const py::dict<py::str, int64_t>& _SIZEOF() {
    static const py::dict<py::str, int64_t> d = cback::SIZEOF;
    return d;
}
const py::str HEADER = py::str(HEADER_TEXT);
const py::list<py::str> INTERRUPT_REGISTERS = L({"ax", "bx", "cx", "dx", "si", "di", "ds", "es"});
const py::str SEG_PTR = py::str(SEG_PTR_TEXT);

const py::dict<py::str, py::str> INTRINSICS16 = [] {
    py::dict<py::str, py::str> d;
    d.set(S("in"), S("uint1 in(uint2 port);\n#pragma aux in = 0xEC parm [dx] value [al];"));
    d.set(S("out"), S("void out(uint2 port, uint1 value);\n#pragma aux out = 0xEE parm [dx] [al];"));
    d.set(S("LOCK"), S("#define LOCK()    /* the next instruction carries a LOCK prefix */"));
    d.set(S("UNLOCK"), S("#define UNLOCK()  /* (the end of that instruction) */"));
    // LSL: the limit of a selector's segment, 32 bits (lsl eax,eax; edx = its high half)
    d.set(S("SegmentLimit"),
          S("uint4 SegmentLimit(uint2 selector);\n#pragma aux SegmentLimit = 0x66 0x0f 0x03 0xc0 "
            "0x66 0x8b 0xd0 0x66 0xc1 0xea 0x10 parm [ax] value [dx ax] modify exact [ax dx];"));
    d.set(S("POPCOUNT"), S("static uint2 POPCOUNT(uint4 x) { uint2 n = 0; while (x) { n += (uint2)(x & 1); x >>= 1; } return n; }"));
    return d;
}();

namespace {

const py::str _TYPES = py::str(TYPES_TEXT);
const py::str _HELPERS = py::str(HELPERS_TEXT);
const py::str _POWERPC = py::str(POWERPC_TEXT);


// The joins, pieces and widenings the core writes for every pair of sizes up to 8 bytes (CONCAT35, SUB86, ZEXT38 ...),
// beyond the ones spelled out above; and the processor's floating-point operations it writes by name.
py::str _more_helpers(const py::str& have) {
    auto kind = [](int64_t n, bool is_signed) {
        int64_t w = n == 1 ? 1 : n == 2 ? 2 : n <= 4 ? 4 : 8;
        return fmt(is_signed ? "int%d" : "uint%d", static_cast<int>(w));
    };
    auto mask = [](int64_t n) {
        std::string s = "0x";
        for (int64_t i = 0; i < n; ++i) s += "FF";
        return py::str(s + "ULL");
    };
    std::vector<py::str> out;
    for (int a = 1; a < 8; ++a) {
        for (int b = 1; b < 9 - a; ++b) {
            if (!have.contains(fmt("CONCAT%d%d(", a, b)))
                out.push_back(fmt("#define CONCAT%d%d(a,b) ((", a, b) + kind(a + b, false) + S(")((((uint8)(a) & ") + mask(a) +
                              fmt(") << %d) | ((uint8)(b) & ", 8 * b) + mask(b) + S(")))"));
        }
    }
    for (int a = 2; a < 9; ++a) {
        for (int b = 1; b < a; ++b) {
            if (!have.contains(fmt("SUB%d%d(", a, b)))
                out.push_back(fmt("#define SUB%d%d(x,n) ((", a, b) + kind(b, false) + S(")((((uint8)(x) & ") + mask(a) +
                              S(") >> (8 * (n))) & ") + mask(b) + S("))"));
            if (!have.contains(fmt("ZEXT%d%d(", b, a)))
                out.push_back(fmt("#define ZEXT%d%d(x) ((", b, a) + kind(a, false) + S(")((uint8)(x) & ") + mask(b) + S("))"));
            if (!have.contains(fmt("SEXT%d%d(", b, a)) && (b == 1 || b == 2 || b == 4))
                out.push_back(fmt("#define SEXT%d%d(x) ((", b, a) + kind(a, true) + S(")(") + kind(b, true) + S(")(x))"));
        }
    }
    for (const char* x : {
             "double sqrt(double); double sin(double); double cos(double); double tan(double);",
             "double atan2(double, double); double pow(double, double); double ldexp(double, int);",
             "double log(double); double floor(double); double fmod(double, double);",
             // (to the nearest whole number, halves to the even one -- written out, so that a
             // module that never rounds does not have the mathematics library linked into it)
             "#define ROUND(x) (((double)(x) - floor((double)(x)) > 0.5 || ((double)(x) - floor((double)(x)) == 0.5 && "
             "fmod(floor((double)(x)), 2.0) != 0)) ? floor((double)(x)) + 1 : floor((double)(x)))",
             "#define SQRT(x) sqrt((double)(x))", "#define ABS(x) ((x) < 0 ? -(x) : (x))", "#define NAN(x) ((x) != (x))",
             "#define fsin(x) sin((double)(x))", "#define fcos(x) cos((double)(x))", "#define fptan(x) tan((double)(x))",
             "#define fpatan(a,b) atan2((double)(a), (double)(b))", "#define f2xm1(x) (pow(2.0, (double)(x)) - 1.0)",
             "#define fscale(a,b) ldexp((double)(a), (int)(b))", "#define log2(x) (log((double)(x)) / 0.6931471805599453)"})
        out.push_back(S(x));
    return joins(out, "\n") + S("\n");
}

const py::str& X86_HELPERS() {
    static const py::str h = _more_helpers(_HELPERS);
    return h;
}

const py::list<py::str> LIBM = L({"sqrt", "sin", "cos", "tan", "atan2", "pow", "ldexp", "log", "floor", "fmod"});

py::str by_size(int64_t size, const char* dflt = "uint4") {
    if (size == 1) return S("uint1");
    if (size == 2) return S("uint2");
    if (size == 4) return S("uint4");
    if (size == 8) return S("uint8");
    return S(dflt);
}
bool is_by_size_value(const py::str& t) { return t == "uint1" || t == "uint2" || t == "uint4" || t == "uint8"; }

const py::set<py::str>& C_WORDS() {
    static const py::set<py::str> w = [] {
        py::set<py::str> s;
        for (const char* x : {"char", "short", "int", "long", "float", "double", "void", "code"}) s.add(S(x));
        return s;
    }();
    return w;
}

// {structure name: its bytes} of the structures recovered
py::dict<py::str, int64_t> _struct_sizes(const py::list<core16::StructNote>& structs) {
    py::dict<py::str, int64_t> out;
    for (const core16::StructNote& s : structs.vec()) out.setdefault(s.name, s.bytes);
    return out;
}

// The C declaring the structures recovered -- every name first (`typedef struct s_f_p s_f_p;`, so one may point at another),
// then each at its exact layout: its fields at their offsets with filler between, the whole its full size, packed (the 32-bit
// header packs everything; a 16-bit structure says so itself). A field type the header does not know is the bytes it takes.
std::vector<py::str> _struct_decls(const py::list<core16::StructNote>& structs, const py::str& header, bool far16 = false) {
    py::dict<py::str, std::tuple<int64_t, std::vector<std::tuple<int64_t, int64_t, py::str>>>> found;
    for (const core16::StructNote& s : structs.vec()) {
        if (found.contains(s.name)) continue;
        std::vector<std::tuple<int64_t, int64_t, py::str>> fs;
        for (const core16::StructField& f : s.fields.vec()) fs.emplace_back(f.offset, f.bytes, f.type);
        found.set(s.name, std::make_tuple(s.bytes, fs));
    }
    std::vector<py::str> out;
    if (found.empty()) return out;
    py::set<py::str> known;
    for (const py::str& w : py::re::findall("typedef [^;()]*?\\b(\\w+);", header).vec()) known.add(w);
    for (const py::str& w : C_WORDS()) known.add(w);
    for (const py::str& n : found.keys()) known.add(n);
    for (const py::str& n : found.keys()) out.push_back(S("typedef struct ") + n + S(" ") + n + S(";"));
    for (const auto& kv : found.items()) {
        const py::str& name = std::get<0>(kv);
        int64_t size = std::get<0>(std::get<1>(kv));
        auto fields = std::get<1>(std::get<1>(kv));
        std::sort(fields.begin(), fields.end());
        std::vector<py::str> body;
        int64_t at = 0;
        for (const auto& [off, width, tok] : fields) {
            if (off < at || off + width > size) continue;   // (overlapping or outside: left as bytes)
            if (off > at) body.push_back(S("uint1 _pad_") + X(at) + S("[") + D(off - at) + S("];"));
            py::str base = tok.rstrip(S("*^"));
            py::str stars = tok.slice(base.size(), tok.size());
            if (!known.contains(base))
                base = (stars.empty() && (width == 1 || width == 2 || width == 4 || width == 8)) ? S("xunknown") + D(width)
                                                                                                : S("xunknown1");
            if (stars.empty() && found.contains(base))   // (a structure held whole: its bytes)
                base = (width != 1 && width != 2 && width != 4 && width != 8) ? S("xunknown1") : S("xunknown") + D(width);
            std::string ptr;
            for (char32_t c : stars.u32()) ptr += (c == U'^') ? " __far *" : " *";
            body.push_back(base + S(ptr) + S(" f_") + X(off) + S(";"));
            at = off + width;
        }
        if (size > at) body.push_back(S("uint1 _pad_") + X(at) + S("[") + D(size - at) + S("];"));
        out.push_back(S(far16 ? "_Packed " : "") + S("struct ") + name + S(" { ") + joins(body, " ") + S(" };"));
    }
    return out;
}

py::str lines_of_structs(const std::vector<py::str>& v) {
    std::string out;
    for (const py::str& t : v) out += t.utf8() + "\n";
    return py::str(out);
}

const py::re::Pattern& RAM() {
    static const py::re::Pattern p = py::re::compile("([a-z]+)Ram([0-9a-f]{8})");
    return p;
}

// The C type of a global from the decompiler's name prefix (p = pointer to what follows).
py::str _ram_type(const py::str& prefix, int64_t size, bool called) {
    int64_t stars = prefix.size() - prefix.lstrip(S("p")).size();
    py::str base = prefix.slice(stars, prefix.size());
    if (base.empty()) base = S("x");
    std::string star_text(static_cast<size_t>(stars), '*');
    if (stars) {
        if (base[0] == "c") return called ? S("code ") + S(star_text) : S("char ") + S(star_text);
        py::str ty = base[0] == "i" ? S("int4") : base[0] == "u" ? S("uint4") : base[0] == "b" ? S("bool")
                   : base[0] == "f" ? S("float4") : S("xunknown4");
        return ty + S(" ") + S(star_text);
    }
    if (called) return S("code *");
    if (base[0] == "i") return S("int") + D(size);
    if (base[0] == "c") return S("char");
    return by_size(size);
}

// A global type for a 16-bit module (c_text._global_type)
py::str _global_type(const py::str& prefix, int64_t size, const std::optional<int64_t>& elem) {
    if (prefix[0] == "p" && prefix.size() > 1) {   // a pointer, by the decompiler's own naming (piRam: int *)
        py::str body = prefix.lstrip(S("p"));
        py::str first = body.empty() ? S("") : body.slice(0, 1);
        py::str to;
        if (first == "i") to = S("int2");
        else if (first == "u") to = S("uint2");
        else if (first == "c") to = S("char");
        else if (first == "b") to = S("bool");
        else to = S("xunknown2");
        // (px..: a pointer to an unknown of 1, 2 or 4 bytes -- the core says which; `p + 2` counts
        // in those: USER's DisableOEMLayer read [[12Eh]+4] for the original's [[12Eh]+2])
        if (elem && *elem && prefix.size() - body.size() == 1) {
            py::str letter = first;
            py::str t2 = letter == "x" ? S("xunknown") : letter == "i" ? S("int") : letter == "u" ? S("uint") : S("");
            if (!t2.empty()) to = t2 + D(*elem);
            else if (letter == "c") to = S("char");
            else if (letter == "b") to = S("bool");
            else to = S("xunknown") + D(*elem);
        }
        std::string stars(static_cast<size_t>(prefix.size() - body.size()), '*');
        return to + S(" ") + S(size == 4 ? "__far " : "") + S(stars);
    }
    bool is_signed = prefix.startswith(S("i")) || prefix.startswith(S("c"));
    if (size == 1) return S(is_signed ? "int1" : "uint1");
    if (size == 2) return S(is_signed ? "int2" : "uint2");
    if (size == 4) return S(is_signed ? "int4" : "uint4");
    return S("uint2");
}

// ---- the C back end's values -------------------------------------------------------------------

Wire wire_of(const py::set<py::str>& s) { return Wstr_set(s); }

Wire wire_str_int_dict(const py::dict<py::str, int64_t>& d) {
    Wire out = Wire::dict();
    for (const auto& kv : d.items()) out.set(Ws(std::get<0>(kv)), Wi(std::get<1>(kv)));
    return out;
}

}  // namespace

// the facts a function is written with (backends/c.function's `facts`): the Python's dict, key by key
Wire facts_wire(const ModuleFacts& m) {
    Wire w = Wire::dict();
    if (m.win16) {
        w.set("distance", Wopt(m.distance));
        w.set("model", Wopt(m.model));
        w.set("far_return", Wire(m.far_return));
        w.set("selector", Wopt_i(m.selector));
        if (m.have_code) {
            Wire widths = Wire::dict();
            widths.set(Wire(true), wire_str_int_dict(m.widths_far));
            widths.set(Wire(false), wire_str_int_dict(m.widths_near));
            w.set("widths", widths);
        } else {
            w.set("widths", Wire::dict());
        }
        w.set("loads", Wint_set(m.loads));
        w.set("export", Wire(m.export_));
        w.set("loadds", Wire(m.loadds));
        std::vector<Wire> ss;
        for (int64_t v : m.ss_stores) ss.push_back(Wi(v));
        w.set("ss_stores", Wire::list(ss));
        w.set("selector_structs", Wire(m.selector_structs));
        w.set("callee_pops", Wire(m.callee_pops));
        w.set("without_notes", Wire(m.without_notes));
        Wire sv = Wire::dict();
        for (const auto& kv : m.selector_values.items())
            sv.set(Wi(std::get<0>(kv)), Wire::tuple({Ws(std::get<0>(std::get<1>(kv))), Ws(std::get<1>(std::get<1>(kv)))}));
        w.set("selector_values", sv);
        for (const auto& kv : m.more.items()) w.set(kv.first, kv.second);   // (the module's own, added since)
        return w;
    }
    if (m.lane) {
        w.set("lane", Ws(*m.lane));
        w.set("functions", Wstr_set(m.functions));
        return w;
    }
    w.set("functions", Wstr_set(m.functions));
    w.set("struct_sizes", wire_str_int_dict(m.struct_sizes));
    w.set("called_through", Wstr_set(m.called_through));
    w.set("without_notes", Wire(m.without_notes));
    Wire al = Wire::dict();
    for (const auto& kv : m.aliases.items()) al.set(Ws(std::get<0>(kv)), Ws(std::get<1>(kv)));
    w.set("aliases", al);
    w.set("pointer_globals", Wstr_set(m.pointer_globals));
    w.set("takes", wire_str_int_dict(m.takes));
    if (m.struct_fields) {
        Wire sf = Wire::dict();
        for (const auto& kv : m.struct_fields->items()) {
            const struct_text::Planned& p = std::get<1>(kv);
            Wire fields = Wire::dict();
            for (const auto& f : std::get<1>(p).items())
                fields.set(Wi(std::get<0>(f)), Wire::tuple({Ws(std::get<0>(std::get<1>(f))), Wi(std::get<1>(std::get<1>(f)))}));
            sf.set(Ws(std::get<0>(kv)), Wire::tuple({Ws(std::get<0>(p)), fields}));
        }
        w.set("struct_fields", sf);
    }
    for (const auto& kv : m.more.items()) w.set(kv.first, kv.second);
    return w;
}

py::set<py::str> str_set_of_(const Wire& v) {
    py::set<py::str> out;
    if (v.is_none()) return out;
    for (const Wire& x : v.seq()) out.add(as_str(x));
    return out;
}

// what the back end left in the facts, read back into the module facts
void read_back(ModuleFacts& m, const Wire& w) {
    if (w.has("declared") && !w.get("declared").is_none()) m.declared = as_str(w.get("declared"));
    if (w.has("registers")) m.registers = opt_str(w.get("registers"));
    if (w.has("structs_used")) m.structs_used = str_set_of_(w.get("structs_used"));
    if (w.has("novalue")) m.novalue = ::py::truthy(w.get("novalue"));
    if (w.has("interrupts")) {
        for (const Wire& t : w.get("interrupts").seq()) {
            Interrupt i;
            i.vector = t[0].as_int();
            for (const Wire& r : t[1].seq()) i.registers.append(as_str(r));
            m.interrupts.insert(i);
        }
    }
    if (w.has("selector_decls")) {
        m.selector_decls = py::list<py::str>();
        for (const Wire& x : w.get("selector_decls").seq()) m.selector_decls.append(as_str(x));
    }
    if (w.has("selector_shared")) {
        m.selector_shared = py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>>();
        for (const auto& kv : w.get("selector_shared").items()) {
            py::dict<py::str, py::dict<int64_t, py::str>> per;
            for (const auto& fk : kv.second.items()) {
                py::dict<int64_t, py::str> fields;
                for (const auto& f : fk.second.items()) fields.set(f.first.as_int(), as_str(f.second));
                per.set(as_str(fk.first), fields);
            }
            m.selector_shared.set(as_str(kv.first), per);
        }
    }
    if (w.has("selector_imports")) {
        m.selector_imports = py::dict<py::str, py::str>();
        for (const auto& kv : w.get("selector_imports").items()) m.selector_imports.set(as_str(kv.first), as_str(kv.second));
    }
    if (w.has("names")) {
        NamesUsed n;
        const Wire& u = w.get("names");
        n.called = str_set_of_(u.get("called"));
        n.called_through = str_set_of_(u.get("called_through"));
        if (!u.get("globals").is_none())
            for (const Wire& g : u.get("globals").seq()) n.globals.add(std::make_tuple(as_str(g[0]), as_str(g[1])));
        if (!u.get("segments").is_none())
            for (const Wire& g : u.get("segments").seq()) n.segments.add(g.as_int());
        n.mk_fp = ::py::truthy(u.get("mk_fp"));
        m.names = n;
    }
    if (w.has("heading")) {
        Heading h;
        const Wire& x = w.get("heading");
        const Wire& params = x.get("params");
        py::list<std::tuple<py::str, py::str>> plist;
        for (const Wire& p : params[1].seq()) plist.append(std::make_tuple(as_str(p[0]), as_str(p[1])));
        h.params = std::make_tuple(as_str(params[0]), plist);
        for (const auto& kv : x.get("types").items()) h.types.set(as_str(kv.first), as_str(kv.second));
        for (const Wire& c : x.get("calls").seq()) {
            py::list<py::str> args;
            for (const Wire& a : c[1].seq()) args.append(as_str(a));
            h.calls.append(std::make_tuple(as_str(c[0]), args));
        }
        h.declaration = as_str(x.get("declaration"));
        h.fastcall = ::py::truthy(x.get("fastcall"));
        m.heading = h;
    }
    if (w.has("frame")) m.more.set("frame", w.get("frame"));
    if (w.has("code_reads")) m.more.set("code_reads", w.get("code_reads"));
}

namespace {

Wire heading_wire(const Heading& h) {
    Wire w = Wire::dict();
    std::vector<Wire> plist;
    for (const auto& p : std::get<1>(h.params).vec()) plist.push_back(Wire::tuple({Ws(std::get<0>(p)), Ws(std::get<1>(p))}));
    w.set("params", Wire::tuple({Ws(std::get<0>(h.params)), Wire::list(plist)}));
    Wire types = Wire::dict();
    for (const auto& kv : h.types.items()) types.set(Ws(std::get<0>(kv)), Ws(std::get<1>(kv)));
    w.set("types", types);
    {
        std::vector<Wire> cs;
        for (const auto& c : h.calls.vec()) cs.push_back(Wire::tuple({Ws(std::get<0>(c)), Wstrs(std::get<1>(c))}));
        w.set("calls", Wire::list(cs));
    }
    w.set("declaration", Ws(h.declaration));
    w.set("fastcall", Wire(h.fastcall));
    return w;
}

}  // namespace

bool Interrupt::operator==(const Interrupt& o) const {
    if (vector != o.vector || registers.size() != o.registers.size()) return false;
    for (int64_t i = 0; i < registers.size(); ++i)
        if (registers[i] != o.registers[i]) return false;
    return true;
}

// (vector, registers) compare as Python's tuples do: the vector, then the names one by one
bool Interrupt::operator<(const Interrupt& o) const {
    if (vector != o.vector) return vector < o.vector;
    int64_t n = std::min(registers.size(), o.registers.size());
    for (int64_t i = 0; i < n; ++i)
        if (registers[i] != o.registers[i]) return registers[i] < o.registers[i];
    return registers.size() < o.registers.size();
}

// ---- 32-bit ---------------------------------------------------------------------------------------

Compilable32 compilable32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    namespace B = build::watcom;
    decompiler::Decompiled got = decompiler::raw(module_path, funcs, outdir, 3600, true, false);
    py::dict<py::str, py::str> out = got.texts;
    py::list<py::str> order = got.order;
    py::dict<int64_t, py::str> by_va = got.by_va;
    py::dict<py::str, py::str> imports = B::import_map(module_path).copy();
    // imports by number, under the names the description gave them: declared and linked too
    B::Numbered numbered = describe::BY_NUMBER.contains(module_path) ? describe::BY_NUMBER.at(module_path) : B::Numbered();
    for (const auto& kv : numbered.items())
        if (!imports.contains(std::get<0>(kv))) imports.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
    py::dict<int64_t, py::set<int64_t>> sizes;
    py::set<py::str> in_ecx, in_edx;
    py::dict<py::str, py::dict<int64_t, py::set<int64_t>>> own;   // function -> address of a global -> bytes of its uses there
    py::list<core16::StructNote> struct_notes;                    // the structures the decompiler recovered (its facts)
    py::dict<py::str, py::dict<py::str, int64_t>> struct_sizes;   // function -> the sizes of those recovered up to it
    py::dict<py::str, native_l2::ModuleNames> named;              // function -> what its tree names (backends/c.module_names32)
    auto notes_of = [&](const py::str& n) -> const core16::FunctionNotes* {
        return got.notes.contains(n) ? &got.notes.at(n) : nullptr;
    };
    for (const py::str& n : order.vec()) {
        const core16::FunctionNotes* facts = notes_of(n);
        if (facts)
            for (const core16::StructNote& s : facts->structs.vec()) struct_notes.append(s);
        struct_sizes.set(n, _struct_sizes(struct_notes).copy());
        if (facts) {
            for (const core16::GlobalNote& g : facts->globals.vec()) {
                if (!sizes.contains(g.address)) sizes.set(g.address, py::set<int64_t>());
                sizes.at(g.address).add(g.bytes);
                if (!own.contains(n)) own.set(n, py::dict<int64_t, py::set<int64_t>>());
                py::dict<int64_t, py::set<int64_t>> mine = own.at(n);
                if (!mine.contains(g.address)) mine.set(g.address, py::set<int64_t>());
                mine.at(g.address).add(g.bytes);
            }
            if (facts->l2) named.set(n, cback::module_names32(doc_wire(*facts)));
        }
    }
    // What the module decides over all of its functions, from their trees before any is written:
    // a global the code calls through holds a function pointer, whatever else it is compared with
    py::set<py::str> called_through, names, called;
    for (const auto& kv : named.items()) {
        const native_l2::ModuleNames& x = std::get<1>(kv);
        for (const py::str& s : x.called_through) called_through.add(s);
        for (const py::str& s : x.names) names.add(s);
        for (const py::str& s : x.calls) called.add(s);
    }
    py::set<py::str> underscored;
    for (const py::str& n2 : names)
        if (py::re::fullmatch("_([a-z]+)Ram([0-9a-f]{8})", n2)) underscored.add(n2);
    std::vector<py::str> macros;
    py::dict<py::str, py::dict<py::str, py::str>> aliases;
    std::set<std::tuple<std::string, std::string>> ramnames;
    for (const py::str& n2 : names) {
        std::optional<py::re::Match> m2 = RAM().fullmatch(n2);
        if (m2) ramnames.emplace(m2->group(1).utf8(), m2->group(2).utf8());
    }
    for (const auto& [pre_s, addr_s] : ramnames) {   // (sorted(set of pairs))
        py::str pre = S(pre_s), addr = S(addr_s);
        int64_t a = py::int_parse(addr, 16);
        py::set<int64_t> szs = sizes.contains(a) && !sizes.at(a).empty() ? sizes.at(a) : py::set<int64_t>();
        if (szs.empty()) szs.add(4);
        int64_t size;
        bool any_le8 = false;
        for (int64_t s : szs) any_le8 = any_le8 || s <= 8;
        int64_t mn = INT64_MAX, mx = INT64_MIN;
        for (int64_t s : szs) {
            mn = std::min(mn, s);
            if (s <= 8) mx = std::max(mx, s);
        }
        if (szs.size() > 1 && (pre[0] == "c" || pre[0] == "b")) size = mn;
        else size = any_le8 ? mx : 4;
        // `pc` is the decompiler's prefix for a pointer to char; a global is a code pointer
        // when something calls through it
        py::str key = pre + S("Ram") + addr;
        py::str ty = called_through.contains(key) ? S("code *") : pre == "pc" ? S("char *")
                    : (pre[0] == "i" ? S("int") + D(size) : by_size(size));
        if (pre[0] == "p" && pre.size() > 1 && ty != "code *") ty = _ram_type(pre, size, false);   // a pointer, by the decompiler's own naming (piRam: int *)
        macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" *)0x") + addr + S(")"));
        if (underscored.contains(S("_") + key))
            // (the core's name for the same place read at another width: `_uRam0040c000`)
            macros.push_back(S("#define _") + key + S(" (*(") + ty + S(" *)0x") + addr + S(")"));
        // A function that uses the place at one width other than the declared one (a byte
        // stored where other functions read a word) uses it at its own width: the declared
        // width would store, or read, the bytes beside it too (backends/c._global_now).
        if (is_by_size_value(ty) || py::re::fullmatch("int\\d", ty)) {
            py::str name = key;
            for (const py::str& n : order.vec()) {
                if (!own.contains(n) || !own.at(n).contains(a)) continue;
                py::set<int64_t> mine = own.at(n).at(a);
                if (mine.size() == 1) {
                    int64_t w = *mine.begin();
                    if (w != size && (w == 1 || w == 2 || w == 4 || w == 8)) {
                        py::str alias = name + S("_") + D(w);
                        if (!aliases.contains(n)) aliases.set(n, py::dict<py::str, py::str>());
                        aliases.at(n).setdefault(name, alias);
                        py::str m = S("#define ") + alias + S(" (*(") + (pre[0] == "i" ? S("int") + D(w) : by_size(w)) + S(" *)0x") + addr + S(")");
                        bool have = false;
                        for (const py::str& x : macros) have = have || x == m;
                        if (!have) macros.push_back(m);
                    }
                }
            }
        }
    }
    // (a pointer-typed global compared with a number the decompiler cast to an integer is compared
    // as the same integer: backends/c._same_cast)
    py::set<py::str> pointers;
    for (const py::re::Match& m : py::re::finditer("#define (\\w+Ram[0-9a-f]{8}) \\(\\*\\([^()]*\\* \\*\\)", joins(macros, "\n")).vec())
        pointers.add(m.group(1));
    // (how many arguments each function's definition takes, of those not __cdecl: a call that
    // passes another number is made through a pointer -- backends/c._fit_calls)
    py::dict<py::str, int64_t> takes;
    for (const py::str& n : order.vec()) {
        const core16::FunctionNotes* f = notes_of(n);
        if (f && f->l2) {
            std::optional<int64_t> k = cback::takes32(cback::signature(doc_wire(*f)));
            if (k) takes.set(n, *k);
        }
    }
    py::dict<py::str, py::str> declared;
    py::dict<py::str, ModuleFacts> written_of;
    py::set<py::str> order_set;
    for (const py::str& n : order.vec()) order_set.add(n);
    for (const py::str& n : order.vec()) {
        core16::FunctionNotes empty;
        core16::FunctionNotes& facts = got.notes.contains(n) ? const_cast<core16::FunctionNotes&>(got.notes.at(n)) : empty;
        // (each function's C written from its L2 document here, with the module's facts: the back
        // end's passes of the 32-bit path -- backends/c.FLAT -- and its part of the module's
        // decisions)
        ModuleFacts written;
        written.functions = order_set;
        written.struct_sizes = struct_sizes.at(n);
        written.called_through = called_through;
        written.without_notes = true;
        written.aliases = aliases.contains(n) ? aliases.at(n) : py::dict<py::str, py::str>();
        written.pointer_globals = pointers;
        written.takes = takes;
        written_of.set(n, written);
        if (facts.l2) out.set(n, *decompiler::c_from_l2(out.at(n), facts, std::nullopt, &written));
        if (facts.l2 && !facts.from_l2) {
            // (a function the back end declines -- a node it does not write -- is one the
            // decompiler could not make, as one whose decompile failed: no C, so no declaration,
            // and the build stands something in for it)
            out.set(n, S("/* ") + n + S(": its C could not be written from L2 */"));
        }
        if (facts.from_l2) declared.set(n, *written.declared);
        // (a comment the core put before the function -- "WARNING: Removing unreachable block" --
        // is not part of it: left in, it became the function's declaration and broke the header.
        // The back end leaves them out, the facts' without_notes; one with no C has no text.)
        out.set(n, facts.from_l2 ? out.at(n).strip() : S(""));
        // every function returns a value in EAX whether or not the core saw it used; the compiler
        // used here has no word for "first argument in ECX": the function is declared plainly, and
        // a #pragma in the header says where its first argument is (both the back end's:
        // backends/c._result_shown, _register_arguments)
        if (written.registers && *written.registers == "ecx") in_ecx.add(n);
        else if (written.registers && *written.registers == "edx") in_edx.add(n);
    }
    std::vector<py::str> used_v;
    for (const py::str& n : py::sorted(called).vec())
        if (imports.contains(n) && !out.contains(n)) used_v.push_back(n);
    py::list<py::str> used(used_v);
    std::vector<py::str> decls;
    for (const py::str& n : used_v) {
        std::optional<int64_t> pop = numbered.contains(n) ? std::get<2>(numbered.at(n)) : symbols::api_arguments::stack_bytes(imports.at(n), n);
        decls.push_back(S("int ") + S(pop && *pop == 0 ? "__cdecl" : "__stdcall") + S(" ") + n + S("();"));
    }
    for (const py::str& n : order.vec()) {   // each function, declared before any use
        py::str head = declared.get(n, S(""));
        if (!head.endswith(S(")"))) continue;
        if (head.contains(S("__cdecl"))) {
            // The caller removes the arguments, so nothing in the binary fixes how many there
            // are: the decompiler recovers each call on its own evidence and callers need not
            // agree with the definition. Declared with the list left open, each call compiles as
            // it was recovered. (A function that removes its own arguments -- the default here --
            // states their size in its return, and keeps its full declaration.)
            head = head.slice(0, head.index(S("("))) + S("()");
        }
        decls.push_back(head + S(";"));
        if (in_ecx.contains(n))
            decls.push_back(S("#pragma aux ") + n + S(" \"_*\" parm routine [ecx] value [eax] modify [eax ecx edx];"));
        else if (in_edx.contains(n))
            decls.push_back(S("#pragma aux ") + n + S(" \"_*\" parm routine [ecx] [edx] value [eax] modify [eax ecx edx];"));
    }
    // the core's names for values of sizes C has no type for (5 bytes of a 64-bit product, 10 of a
    // floating-point register): each is given the next type that holds it. One left undeclared
    // in a function's heading broke the header, and with it every function of the module. (The
    // types each written function's tree uses: backends/c.type_names.)
    std::vector<py::str> odd;
    py::set<py::str> types;
    for (const py::str& n : declared.keys())
        for (const py::str& t : cback::type_names(doc_wire(got.notes.at(n)))) types.add(t);
    for (const py::str& name : py::sorted(types).vec()) {
        if (!py::re::fullmatch("unk(?:byte|int|uint)\\d+|u?int[3567]|xunknown[3567]", name)) continue;
        if (py::re::search("\\b" + name.utf8() + ";", _TYPES)) continue;
        int64_t size = py::int_parse(py::re::search("\\d+$", name)->group(0), 10);
        bool is_signed = name.startswith(S("unkint")) || name.startswith(S("int"));
        odd.push_back(S("typedef ") + (size > 8 ? S("long double") : is_signed ? S(size > 4 ? "long long" : "int")
                                                                      : S(size > 4 ? "unsigned long long" : "unsigned int")) +
                      S(" ") + name + S(";"));
    }
    // A module that is itself a C runtime (crtdll, msvcrt, mfc40, opengl32 ...) has functions of
    // its own called sqrt, ldexp, floor: its declarations are the ones that hold, and the
    // helpers' library prototypes of the same names step aside (a clash broke the whole header).
    py::set<py::str> own_names;
    for (const py::str& d : decls) {
        if (!d.contains(S("("))) continue;
        py::list<py::str> w = py::split(py::split(d, S("("))[0]);
        own_names.add(w[-1].lstrip(S("*")));
    }
    bool clash = false;
    for (const py::str& l : LIBM.vec()) clash = clash || own_names.contains(l);
    py::str helpers = X86_HELPERS();
    if (clash) {
        helpers = py::re::sub("double (" + join(LIBM.vec(), "|") + ")\\([^)]*\\); ?",
                              [&](const py::re::Match& m) { return own_names.contains(m.group(1)) ? S("") : m.group(0); }, helpers);
    }
    std::vector<py::str> structs = _struct_decls(struct_notes, _TYPES + joins(odd, "\n"));
    py::str preamble = _TYPES + joins(odd, "\n") + S("\n") + lines_of_structs(structs) + _HELPERS + helpers + joins(decls, "\n") +
                       S("\n") + joins(macros, "\n") + S("\n");
    // `p->field` for `*(T *)(p + N)`: which accesses through a parameter are a structure's fields
    // is the module's decision (struct_text.plan, from the trees and the headings as written), and
    // the functions it names are written again with theirs (backends/c._struct_fields)
    py::dict<py::str, L2Doc> docs_of;
    py::set<py::str> declared_names;
    for (const py::str& n : declared.keys()) {
        docs_of.set(n, *got.notes.at(n).l2);
        declared_names.add(n);
    }
    struct_text::Analysis analysis;
    analysis.recover = [](const py::str& path) {
        py::list<struct_text::Structure> out_s;
        std::shared_ptr<py::list<analyze::structures::Structure>> found = analyze::structures::_recover(*analyze::image::load(path));
        for (const analyze::structures::Structure& s : found->vec()) {
            struct_text::Structure t;
            t.name = s.name;
            for (const analyze::structures::Root& r : s.roots.vec()) {
                if (r.kind == "arg") t.roots.append(struct_text::Root{r.kind, r.addr, r.index});
                else if (r.kind == "this") t.roots.append(struct_text::Root{r.kind, r.addr, std::nullopt});
            }
            out_s.append(t);
        }
        return out_s;
    };
    analysis.parameter_names = [&](const py::str& fn) { return cback::parameter_names(docs_of.at(fn)); };
    analysis.struct_accesses = [&](const py::str& fn, const py::str& param) {
        py::list<struct_text::Access> acc;
        for (const auto& a : cback::struct_accesses(docs_of.at(fn), param).vec())
            acc.append(struct_text::Access{std::get<0>(a), std::get<1>(a), std::get<2>(a)});
        return acc;
    };
    struct_text::Plan planned = struct_text::plan(module_path, declared_names, by_va, declared, analysis);
    py::set<py::str> used_structs;
    py::dict<py::str, py::str> final_ = declared.copy();   // (each heading as last written: with its fields)
    for (const py::str& n : order.vec()) {
        if (!planned.contains(n)) continue;
        ModuleFacts written = written_of.at(n);
        written.struct_fields = planned.at(n);
        core16::FunctionNotes& facts = const_cast<core16::FunctionNotes&>(got.notes.at(n));
        out.set(n, decompiler::c_from_l2(out.at(n), facts, std::nullopt, &written)->strip());
        for (const py::str& s : written.structs_used) used_structs.add(s);
        if (written.declared && !written.declared->empty()) final_.set(n, *written.declared);
    }
    if (!planned.empty()) preamble += struct_text::declarations(planned, used_structs);
    py::dict<py::str, L2Doc> docs;
    for (const py::str& n : order.vec())
        if (got.notes.contains(n) && got.notes.at(n).l2) docs.set(n, *got.notes.at(n).l2);
    py::dict<py::str, py::str> decl_out;
    for (const auto& kv : final_.items())
        if (std::get<1>(kv).endswith(S(")"))) decl_out.set(std::get<0>(kv), std::get<1>(kv) + S(";"));
    Compilable32 r;
    r.preamble = preamble;
    r.texts = out;
    r.order = order;
    r.by_va = by_va;
    r.imports = used;
    r.in_ecx = in_ecx;
    r.in_edx = in_edx;
    r.docs = docs;
    r.declarations = decl_out;
    return r;
}

namespace {

// ---- Data kept in a code segment, read through CS -------------------------------------------------
// KEYBOARD's GetKeyNameText looks a key up in tables beside its own code (9:0261, 9:02E9 ...)
// and reads them through CS: `MK_FP(unaff_CS, pcVar11)` with pcVar11 set to 0x261. The rebuilt
// code segment is laid out by the compiler -- functions rebuilt from C are other sizes, some are
// moved -- so CS:0261 is not the table any more, and in a function judged on its own it is
// whatever the throw-away build has there. The source such a module was written from named the
// table, in the code segment (`static char _based(_segname("_CODE")) ...`): so does this. The
// bytes a function's C reads through CS are the original's, in a structure in its own code
// segment, codebreaker_code_<n> (one member at each place the code starts reading from), and
// each number that is such a place is written as the member's offset.

bool is_cs_return(const py::str& mn) { return mn == "ret" || mn == "retf" || mn == "iret" || mn == "iretd" || mn == "hlt"; }

// Which bytes of a code segment are its functions' instructions: each function followed from its start, every branch it can
// take. -> 1 at a byte of code. (A place only a jump table reaches is taken for data -- its bytes copied as data is all
// that comes of it.)
std::vector<uint8_t> _code_bytes(const py::bytes& image, const std::vector<int64_t>& starts) {
    std::shared_ptr<const describe::Decoder> md = describe::decoder16();
    int64_t n = image.size();
    std::vector<uint8_t> code(static_cast<size_t>(n), 0);
    std::set<int64_t> seen;
    std::vector<int64_t> todo;
    for (int64_t s : starts)
        if (0 <= s && s < n) todo.push_back(s);
    static const py::re::Pattern target_rx = py::re::compile("0x([0-9a-f]+)|(\\d+)");
    while (!todo.empty()) {
        int64_t a = todo.back();
        todo.pop_back();
        while (0 <= a && a < n && !seen.count(a)) {
            py::list<describe::Insn> ins = md->disasm(image.slice(a, a + 16), a);
            if (ins.empty()) break;
            const describe::Insn& one = ins[0];
            int64_t size = one.size;
            seen.insert(a);
            for (int64_t k = a; k < std::min<int64_t>(a + size, n); ++k) code[static_cast<size_t>(k)] = 1;
            if (is_cs_return(one.mnemonic)) break;
            std::optional<py::re::Match> target = target_rx.fullmatch(one.op_str.strip());
            if ((one.mnemonic.startswith(S("j")) || one.mnemonic.startswith(S("loop")) || one.mnemonic == "call") && target) {
                todo.push_back(!target->group(1).empty() ? py::int_parse(target->group(1), 16) : py::int_parse(target->group(2), 10));
            }
            if (one.mnemonic == "jmp" || one.mnemonic == "ljmp") break;
            a += size;
        }
    }
    return code;
}

// [(start, end)] of the bytes of a code segment that are not instructions
std::vector<std::pair<int64_t, int64_t>> _data_runs(const std::vector<uint8_t>& code) {
    std::vector<std::pair<int64_t, int64_t>> runs;
    size_t k = 0;
    while (k < code.size()) {
        if (code[k]) {
            ++k;
            continue;
        }
        size_t s = k;
        while (k < code.size() && !code[k]) ++k;
        runs.emplace_back(static_cast<int64_t>(s), static_cast<int64_t>(k));
    }
    return runs;
}

}  // namespace

std::optional<py::str> _string_literal(const py::bytes& chunk) {
    const std::string& c = chunk.raw();
    auto u = [&](size_t i) { return static_cast<unsigned char>(c[i]); };
    if (c.size() < 2 || u(c.size() - 1) != 0) return std::nullopt;
    size_t printable = 0;
    for (size_t i = 0; i < c.size(); ++i)
        if (32 <= u(i) && u(i) < 127) ++printable;
    if (printable * 2 < c.size()) return std::nullopt;
    for (size_t i = 0; i < c.size(); ++i)
        if (u(i) && !(32 <= u(i) && u(i) < 127) && u(i) != 9 && u(i) != 10 && u(i) != 13) return std::nullopt;
    std::string out = "\"";
    for (size_t i = 0; i + 1 < c.size(); ++i) {
        switch (u(i)) {
            case 0: out += "\\000"; break;
            case 9: out += "\\t"; break;
            case 10: out += "\\n"; break;
            case 13: out += "\\r"; break;
            case 34: out += "\\\""; break;
            case 92: out += "\\\\"; break;
            case 63: out += "\\?"; break;
            default: out.push_back(static_cast<char>(u(i)));
        }
    }
    return py::str(out + "\"");
}

namespace {

// `items` joined by commas, `per` to a line (an initialiser's elements)
py::str _rows(const std::vector<py::str>& items, size_t per) {
    if (items.size() <= per) return joins(items, ", ");
    std::vector<py::str> rows;
    for (size_t k = 0; k < items.size(); k += per) {
        std::vector<py::str> part(items.begin() + static_cast<long>(k), items.begin() + static_cast<long>(std::min(items.size(), k + per)));
        rows.push_back(joins(part, ", "));
    }
    return S("\n    ") + joins(rows, ",\n    ") + S("\n  ");
}

// what writing a function found of its reads through CS (backends/c._code_segment_reads), typed
struct CodeReads {
    int64_t reads = 0;
    std::vector<std::pair<int64_t, int64_t>> places;                                      // {k: v}
    std::vector<std::pair<py::str, std::pair<int64_t, int64_t>>> events;                  // [(kind, (k, v))]
    std::vector<py::str> unknown;
    std::vector<std::vector<std::pair<int64_t, int64_t>>> each;
    std::vector<py::str> unsafe;
};

CodeReads parse_reads(const Wire& rep) {
    CodeReads r;
    r.reads = rep.get("reads").as_int();
    for (const auto& kv : rep.get("places").items()) r.places.emplace_back(kv.first.as_int(), kv.second.as_int());
    for (const Wire& e : rep.get("events").seq()) r.events.emplace_back(as_str(e[0]), std::make_pair(e[1][0].as_int(), e[1][1].as_int()));
    for (const Wire& u : rep.get("unknown").seq()) r.unknown.push_back(as_str(u));
    for (const Wire& g : rep.get("each").seq()) {
        std::vector<std::pair<int64_t, int64_t>> one;
        for (const Wire& p : g.seq()) one.emplace_back(p[0].as_int(), p[1].as_int());
        r.each.push_back(one);
    }
    for (const Wire& u : rep.get("unsafe").seq()) r.unsafe.push_back(::py::str_of(u).empty() ? S("") : S(::py::str_of(u)));
    return r;
}

struct CodeData {
    std::vector<py::str> decls;
    std::map<int64_t, py::str> defs;
    std::map<int64_t, CodeSegmentReport> report;
    std::vector<std::pair<py::str, Wire>> plan;   // {function: {"code_places": {node: "(uint2)member"}}}
};

// Each number in the C that is a place in a code segment's data, read through CS, written as that place in the segment's
// codebreaker_code_<n> (above): from what writing each function found (`reports` {function: backends/c._code_segment_reads'
// facts["code_reads"]}, in address order; `where` {function: (segment, offset)}).
CodeData _code_segment_data(const std::vector<std::pair<py::str, std::optional<Wire>>>& reports,
                            const std::map<std::string, std::optional<core16::SegOff>>& where, const describe::Description& d,
                            const py::str& module_path) {
    CodeData out;
    struct Found {
        py::str name;
        CodeReads rep;
        int64_t seg;
    };
    std::vector<Found> found;
    for (const auto& [name, r] : reports) {
        auto w = where.find(name.utf8());
        if (!r || !::py::truthy(*r) || w == where.end() || !w->second) continue;
        found.push_back({name, parse_reads(*r), std::get<0>(*w->second)});
    }
    if (found.empty()) return out;
    formats::ne::NEFile nef = formats::ne::NEFile(module_path);
    std::map<int64_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : nef.segments().vec()) segs[s.idx] = s;
    std::map<int64_t, std::vector<int64_t>> starts;
    for (const auto& kv : d.where.items()) starts[std::get<0>(std::get<1>(kv))].push_back(std::get<1>(std::get<1>(kv)));
    using Run = std::pair<int64_t, int64_t>;
    std::map<int64_t, std::vector<Run>> runs;
    std::map<int64_t, std::vector<int64_t>> relocated;
    std::set<int64_t> found_segs;
    for (const Found& f : found) found_segs.insert(f.seg);
    for (int64_t sg : found_segs) {
        py::bytes image = d.images.get(sg, py::bytes());
        runs[sg] = _data_runs(_code_bytes(image, starts[sg]));
        auto s = segs.find(sg);
        std::vector<int64_t> rel;
        if (s != segs.end() && s->second.file_off) {
            py::bytes blob = nef.data.slice(s->second.file_off, s->second.file_off + s->second.length);
            for (const auto& p : build::data::_places(nef, s->second, blob).vec()) rel.push_back(std::get<0>(p));
            std::sort(rel.begin(), rel.end());
        }
        relocated[sg] = rel;
    }
    auto run_of = [&](int64_t sg, int64_t v) -> std::optional<Run> {
        auto it = runs.find(sg);
        if (it == runs.end()) return std::nullopt;
        const std::vector<Run>& rs = it->second;
        // bisect_right(rs, (v, 0x10000)) - 1
        size_t lo = 0, hi = rs.size();
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (Run(v, 0x10000) < rs[mid]) hi = mid;
            else lo = mid + 1;
        }
        if (lo == 0) return std::nullopt;
        const Run& r = rs[lo - 1];
        return (r.first <= v && v < r.second) ? std::optional<Run>(r) : std::nullopt;
    };
    struct Per {
        py::str name;
        std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> places;   // k -> (k, v)
        std::vector<std::pair<int64_t, py::str>> kinds;                          // v -> kind (insertion order)
    };
    std::map<int64_t, std::vector<Per>> per;
    std::map<int64_t, std::vector<std::set<int64_t>>> each_read;
    std::map<int64_t, CodeSegmentReport> report;
    for (const Found& f : found) {
        int64_t sg = f.seg;
        auto inside = [&](int64_t v) { return run_of(sg, v).has_value(); };
        Per p;
        p.name = f.name;
        for (const auto& kv : f.rep.places)
            if (inside(kv.second)) p.places.emplace_back(kv.first, std::make_pair(kv.first, kv.second));
        auto set_kind = [&](int64_t v, const py::str& kind, bool overwrite) {
            for (auto& kk : p.kinds)
                if (kk.first == v) {
                    if (overwrite) kk.second = kind;
                    return;
                }
            p.kinds.emplace_back(v, kind);
        };
        for (const auto& e : f.rep.events) {   // (in the order the reads found them)
            int64_t v = e.second.second;
            if (!inside(v)) continue;
            if (e.first == "call") set_kind(v, e.first, true);
            else set_kind(v, e.first, false);
        }
        std::vector<std::set<int64_t>> each;
        for (const auto& got : f.rep.each) {
            std::set<int64_t> s;
            for (const auto& kv : got)
                if (inside(kv.second)) s.insert(kv.second);
            each.push_back(s);
        }
        CodeSegmentReport& r = report[sg];
        r.read += f.rep.reads;
        if (!f.rep.unsafe.empty()) {   // (a table of places it cannot write as places)
            r.left.append(f.name + S(": an offset it reads through CS is a ") + f.rep.unsafe[0]);
            continue;
        }
        per[sg].push_back(p);
        for (const auto& s : each) each_read[sg].push_back(s);
        if (p.places.empty()) {
            std::set<std::string> uniq;
            for (const py::str& u : f.rep.unknown) uniq.insert(u.utf8());
            std::vector<py::str> names;
            for (const std::string& u : uniq) names.push_back(S(u));
            r.left.append(f.name + S(": its offsets through CS come from ") +
                          (names.empty() ? S("nothing it sets to a place in the data") : joins(names, ", ")));
        }
    }
    for (auto& [sg, pers] : per) {
        py::bytes image = d.images.get(sg, py::bytes());
        std::set<int64_t> bases;
        std::map<int64_t, py::str> kinds;
        for (const Per& p : pers) {
            for (const auto& kv : p.places) bases.insert(kv.second.second);
            for (const auto& kv : p.kinds)
                if (!kinds.count(kv.first) || kinds[kv.first] != "call") kinds[kv.first] = kv.second;
        }
        std::set<int64_t> calls;
        for (const auto& kv : kinds)
            if (kv.second == "call") calls.insert(kv.first);
        if (!calls.empty()) {   // (its addresses would be the original's)
            std::vector<py::str> cs;
            for (int64_t v : calls) cs.push_back(X4(v));
            report[sg].left.append(S("tables of the code's own addresses, called: ") + joins(cs, ", "));
        }
        for (int64_t c : calls) bases.erase(c);
        // what each table of places holds: words, each a place in the data, up to the first of
        // those places that is after it (the strings it points at, after the table)
        std::map<int64_t, std::vector<int64_t>> pointers;
        for (int64_t b : bases) {
            auto kk = kinds.find(b);
            if (kk == kinds.end() || kk->second != "pointers") continue;
            Run run = *run_of(sg, b);
            int64_t limit = run.second;
            for (int64_t x : bases)
                if (x > b) limit = std::min(limit, x);
            int64_t at = b;
            std::vector<int64_t> got;
            for (;;) {
                int64_t first_after = 0x10000;
                for (int64_t t : got)
                    if (t > b) first_after = std::min(first_after, t);
                if (!(at + 2 <= limit && at < first_after)) break;
                const std::string& im = image.raw();
                int64_t v = static_cast<unsigned char>(im[static_cast<size_t>(at)]) | (static_cast<unsigned char>(im[static_cast<size_t>(at) + 1]) << 8);
                if (!run_of(sg, v) || calls.count(v)) break;
                got.push_back(v);
                at += 2;
            }
            if (!got.empty()) pointers[b] = got;
        }
        // the structure: each run of data a place is in, whole (a read may go back from the place:
        // VGA's sub_5_027E reads from 40h down), a member at each place
        std::map<Run, int64_t> spans;
        {
            std::set<int64_t> all = bases;
            for (const auto& kv : pointers)
                for (int64_t t : kv.second) all.insert(t);
            for (int64_t v : all) {
                Run r = *run_of(sg, v);
                spans[r] = r.first;
            }
        }
        if (spans.empty()) continue;
        std::set<int64_t> cuts;
        for (;;) {
            cuts.clear();
            std::set<int64_t> targets;
            for (const auto& kv : pointers)
                for (int64_t t : kv.second) targets.insert(t);
            for (const auto& [run, lo] : spans) {
                cuts.insert(lo);
                cuts.insert(run.second);
                for (int64_t v : bases)
                    if (lo <= v && v < run.second) cuts.insert(v);
                for (int64_t v : targets)
                    if (lo <= v && v < run.second) cuts.insert(v);
                for (const auto& kv : pointers)
                    if (lo <= kv.first && kv.first < run.second) cuts.insert(kv.first + 2 * static_cast<int64_t>(kv.second.size()));
            }
            // (a table some place points into the middle of is bytes after all)
            std::vector<int64_t> torn;
            for (const auto& kv : pointers)
                for (int64_t c : cuts)
                    if (kv.first < c && c < kv.first + 2 * static_cast<int64_t>(kv.second.size())) {
                        torn.push_back(kv.first);
                        break;
                    }
            if (torn.empty()) break;
            for (int64_t b : torn) pointers.erase(b);
        }
        std::map<int64_t, py::str> member_at;
        std::vector<std::pair<int64_t, int64_t>> members;
        for (const auto& [run, lo] : spans) {
            std::vector<int64_t> edges;
            for (int64_t c : cuts)
                if (lo <= c && c <= run.second) edges.push_back(c);
            for (size_t i = 0; i + 1 < edges.size(); ++i) {
                members.emplace_back(edges[i], edges[i + 1]);
                member_at[edges[i]] = S("at_") + X4(edges[i]);
            }
        }
        py::str struct_name = S("codebreaker_code_") + D(sg);
        auto place = [&](int64_t v) {
            int64_t k = INT64_MIN;
            for (const auto& m : members)
                if (m.first <= v) k = std::max(k, m.first);
            if (k == INT64_MIN) throw py::ValueError("max() arg is an empty sequence");
            return k == v ? struct_name + S(".") + member_at[k] : S("&") + struct_name + S(".") + member_at[k] + S("[") + D(v - k) + S("]");
        };
        // (a place in a member the loader writes into -- a selector, a far pointer kept there -- is
        // left as it was: the bytes copied would be the made-up values the description gave them)
        std::set<int64_t> fixed;
        for (const auto& m : members)
            for (int64_t r : relocated[sg])
                if (m.first <= r && r < m.second) {
                    fixed.insert(m.first);
                    break;
                }
        if (!fixed.empty()) {
            std::vector<py::str> fs;
            for (int64_t a : fixed) fs.push_back(X4(a));
            report[sg].left.append(S("data the loader fixes up: ") + joins(fs, ", "));
        }
        std::set<int64_t> ok_place;
        for (int64_t v : bases)
            for (const auto& m : members)
                if (m.first <= v && v < m.second && !fixed.count(m.first)) {
                    ok_place.insert(v);
                    break;
                }
        std::vector<py::str> fields, inits;
        for (const auto& [a, b] : members) {
            if (pointers.count(a)) {
                fields.push_back(S("  uint1 __based(__segname(\"_CODE\")) *") + member_at[a] + S("[") + D(static_cast<int64_t>(pointers[a].size())) + S("];"));
                std::vector<py::str> ps;
                for (int64_t v : pointers[a]) ps.push_back(place(v));
                inits.push_back(S("  {") + _rows(ps, 3) + S("}"));
            } else {
                fields.push_back(S("  uint1 ") + member_at[a] + S("[") + D(b - a) + S("];"));
                py::bytes chunk = image.slice(a, b);
                std::optional<py::str> lit = _string_literal(chunk);
                if (lit) {
                    inits.push_back(S("  ") + *lit);
                } else {
                    std::vector<py::str> hx;
                    for (unsigned char c : chunk.raw()) hx.push_back(fmt("0x%02x", static_cast<int>(c)));
                    inits.push_back(S("  {") + _rows(hx, 16) + S("}"));
                }
            }
        }
        std::vector<py::str> wh;
        for (const auto& [run, lo] : spans) wh.push_back(X4(lo) + S("-") + X4(run.second));
        out.decls.push_back(S("/* the original's bytes of code segment ") + D(sg) + S(" (") + joins(wh, ", ") +
                            S(") that its C reads through CS */"));
        out.decls.push_back(S("_Packed struct ") + struct_name + S(" {\n") + joins(fields, "\n") + S("\n};"));
        out.decls.push_back(S("extern _Packed struct ") + struct_name + S(" __based(__segname(\"_CODE\")) ") + struct_name + S(";"));
        out.defs[sg] = S("_Packed struct ") + struct_name + S(" __based(__segname(\"_CODE\")) ") + struct_name + S(" = {\n") +
                       joins(inits, ",\n") + S("\n};\n");
        for (const Per& p : pers) {
            Wire written = Wire::dict();
            bool any = false;
            for (const auto& kv : p.places) {
                int64_t k = kv.second.first, v = kv.second.second;
                if (ok_place.count(v)) {
                    written.set(Wi(k), Ws(S("(uint2)") + place(v)));
                    any = true;
                }
            }
            if (any) {
                Wire w = Wire::dict();
                w.set("code_places", written);
                bool replaced = false;
                for (auto& pl : out.plan)
                    if (pl.first == p.name) {
                        pl.second = w;
                        replaced = true;
                    }
                if (!replaced) out.plan.emplace_back(p.name, w);
            }
        }
        std::vector<int64_t> okv(ok_place.begin(), ok_place.end());
        report[sg].placed = py::list<int64_t>(okv);
        int64_t named = 0;
        for (const auto& got : each_read[sg]) {
            bool sub = !got.empty();
            for (int64_t v : got) sub = sub && ok_place.count(v);
            if (sub) ++named;
        }
        report[sg].named = named;
    }
    out.report = report;
    return out;
}

// How wide the original reads through the far pointer it takes as argument n: `les reg, [bp+N]`, then the first
// `es:[reg...]` through it. -> {a<n>: bytes}
py::dict<py::str, int64_t> _far_argument_widths(const py::bytes& code, int64_t start, int64_t end, bool far_fn) {
    std::shared_ptr<const describe::Decoder> md = describe::decoder16();
    int64_t first = far_fn ? 6 : 4;   // (the first argument above the return address)
    py::dict<py::str, int64_t> out;
    py::dict<py::str, py::str> held;
    static const py::re::Pattern les_rx = py::re::compile("(\\w+), ptr \\[bp \\+ (0x[0-9a-f]+|\\d+)\\]");
    for (const describe::Insn& ins : md->disasm(code.slice(start, end), start).vec()) {
        std::optional<py::re::Match> mm;
        if (ins.mnemonic == "les") mm = les_rx.fullmatch(ins.op_str);
        if (mm) {
            int64_t n = py::floordiv(py::int_parse(mm->group(2), 0) - first, 2) + 1;
            if (n >= 1) held.set(mm->group(1), S("a") + D(n));
            continue;
        }
        for (const auto& kv : held.items()) {
            const py::str& reg = std::get<0>(kv);
            const py::str& arg = std::get<1>(kv);
            std::optional<py::re::Match> ref = py::re::search("(byte|word|dword) ptr es:\\[" + reg.utf8() + "\\b", ins.op_str);
            if (ref) {
                py::str w = ref->group(1);
                out.setdefault(arg, w == "byte" ? 1 : w == "word" ? 2 : 4);
                held.pop(reg);
            } else if (py::re::match("\\s*" + reg.utf8() + "\\s*,", ins.op_str) || ins.mnemonic == "call" || ins.mnemonic == "lcall" ||
                       ins.mnemonic == "ret" || ins.mnemonic == "retf") {
                held.pop(reg);   // (the register set again, or gone)
            }
        }
    }
    return out;
}

// The offsets from BP of the argument words a function loads as far pointers: `les bx, [bp+6]` takes the word at +6 as an
// offset and the one above it as its segment.
py::set<int64_t> _far_loads(const py::bytes& code, int64_t start, int64_t end) {
    std::shared_ptr<const describe::Decoder> md = describe::decoder16();
    py::set<int64_t> found;
    static const py::re::Pattern rx = py::re::compile("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]");
    for (const describe::Insn& ins : md->disasm(code.slice(start, end), start).vec()) {
        if (ins.mnemonic == "les" || ins.mnemonic == "lds") {
            std::optional<py::re::Match> m = rx.search(ins.op_str);
            if (m) found.add(py::int_parse(m->group(1), 0));
        }
    }
    return found;
}

// Whether a function's prologue loads DS with its own data segment: `mov ax, DGROUP` then `mov ds, ax`, among its first
// instructions (before or after `enter`/`push bp`, saves).
bool _loads_dgroup(const py::bytes& code, int64_t off, int64_t dgroup) {
    std::shared_ptr<const describe::Decoder> md = describe::decoder16();
    bool loaded = false;
    for (const describe::Insn& i : md->disasm(code.slice(off, off + 24), off).vec()) {
        if (i.mnemonic == "mov" && i.op_str == "ax, 0x" + X(dgroup).utf8()) {
            loaded = true;
        } else if (i.mnemonic == "mov" && i.op_str == "ds, ax") {
            return loaded;
        } else if (!(i.mnemonic == "push" || i.mnemonic == "enter" || i.mnemonic == "inc" || i.mnemonic == "nop" || i.mnemonic == "sub" ||
                     (i.mnemonic == "mov" && i.op_str == "bp, sp"))) {
            return false;
        }
    }
    return false;
}

// {segment: [(offset, segment whose selector the loader puts there)]} -- the module's own selector fix-ups (a selector, or
// the selector half of a far pointer), chains followed.
std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> _selector_sites(const py::str& module_path) {
    formats::ne::NEFile nef(module_path);
    py::list<formats::ne::Entry> entries = nef.entries();
    std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> out;
    for (const formats::ne::Segment& s : nef.segments().vec()) {
        if (!s.file_off) continue;
        std::string raw = nef.data.slice(s.file_off, s.file_off + s.length).raw();
        for (const formats::ne::Relocation& r : nef.relocations(s).vec()) {
            if (r.kind != "INTERNAL" || (r.src != 2 && r.src != 3)) continue;
            std::optional<int64_t> target = r.seg;
            if (target && *target == 0xFF) {   // a movable segment, through its entry
                std::optional<int64_t> seg;
                int64_t e = r.entry.value_or(0);
                if (0 < e && e <= entries.size()) seg = entries[e - 1].seg;
                target = seg;
            }
            if (!target || !*target) continue;
            int64_t at = r.offset;
            std::set<int64_t> seen;
            while (at < static_cast<int64_t>(raw.size()) - 1 && !seen.count(at)) {
                seen.insert(at);
                // (the target of a far call or jump into the segment is not its selector as a
                // value: with one in the function, a plain `push 1000h` became segment 1's)
                int64_t opcode = at - (r.src == 3 ? 1 : 3);
                if (!(opcode >= 0 && (static_cast<unsigned char>(raw[static_cast<size_t>(opcode)]) == 0x9A ||
                                      static_cast<unsigned char>(raw[static_cast<size_t>(opcode)]) == 0xEA)))
                    out[s.idx].emplace_back(at + (r.src == 3 ? 2 : 0), *target);
                if (r.additive) break;
                int64_t nxt = static_cast<unsigned char>(raw[static_cast<size_t>(at)]) | (static_cast<unsigned char>(raw[static_cast<size_t>(at) + 1]) << 8);
                if (nxt == 0xFFFF) break;
                at = nxt;
            }
        }
    }
    return out;
}

}  // namespace

py::list<int64_t> _ss_store_places(const py::bytes& code, int64_t start, int64_t end) {
    py::list<int64_t> places;
    static const py::re::Pattern rx = py::re::compile("word ptr \\[bp - (0x[0-9a-f]+|\\d+)\\], ss");
    for (const describe::Insn& ins : describe::decoder16()->disasm(code.slice(start, end), start).vec()) {
        std::optional<py::re::Match> mm;
        if (ins.mnemonic == "mov") mm = rx.fullmatch(ins.op_str);
        if (mm) places.append(py::int_parse(mm->group(1), 0) + 2);
    }
    return places;
}

ModuleFacts module_facts16(const std::optional<py::bytes>& code, int64_t off, int64_t end, const std::optional<py::str>& distance,
                           const std::optional<py::str>& model, const std::optional<int64_t>& dgroup, bool far_return,
                           const std::optional<int64_t>& seg) {
    ModuleFacts f;
    f.win16 = true;
    bool have = code.has_value();
    f.have_code = have;
    f.distance = distance;
    f.model = model;
    f.far_return = far_return;
    if (seg) f.selector = describe::selector(*seg);
    if (have) {
        f.widths_far = _far_argument_widths(*code, off, end, true);
        f.widths_near = _far_argument_widths(*code, off, end, false);
        f.loads = _far_loads(*code, off, end);
        py::bytes head = code->slice(off, off + 3);
        f.export_ = head == py::bytes("\x8c\xd8\x90", 3) || head == py::bytes("\x1e\x58\x90", 3);   // mov ax, ds / nop (either encoding)
        f.loadds = dgroup && _loads_dgroup(*code, off, *dgroup);
        f.ss_stores = _ss_store_places(*code, off, end);
    }
    return f;
}

namespace {

// Does the function at `off` set up BP as its frame -- Windows' far prologue, `push bp / mov bp, sp`, or `enter`?
bool _keeps_frame(const py::bytes& code, int64_t off) {
    py::bytes head = code.slice(off, off + 3);
    return describe::windows_far_prologue(code, off) || head == py::bytes("\x55\x8b\xec", 3) || head == py::bytes("\x55\x89\xe5", 3) ||
           (head.size() >= 1 && static_cast<unsigned char>(head.raw()[0]) == 0xC8);
}

// _IMAGES16: segment -> its bytes, for _write16
py::dict<int64_t, py::bytes>& IMAGES16() {
    static py::dict<int64_t, py::bytes> d;
    return d;
}

// one function to be written: the Python's tuple (va, distance, model, seg, off, end, dgroup, known, by_selector, callee_pops,
// shared)
struct Item16 {
    int64_t va = 0;
    std::optional<py::str> distance, model;
    int64_t seg = 0, off = 0, end = 0;
    std::optional<int64_t> dgroup;
    std::optional<core16::FunctionNotes> known;
    bool by_selector = false;
    bool callee_pops = false;
    SelectorImports selector_values;     // shared["selector_values"]
    Wire extra = Wire::dict();           // what the module decided of it since ("frame_slots" ...)
};

// _write16's answer: (address, text or None where the back end declines, its flags in the module, its facts as the back end
// may have changed them, what its passes found for the module)
struct Written {
    bool ok = false;
    int64_t va = 0;
    std::optional<py::str> text;
    py::set<py::str> flags;
    core16::FunctionNotes known;
    ModuleFacts found;
};

// One function's C written from its L2 document by the C back end (backends/c), with what the module says of it
// (module_facts16).
Written _write16(const std::optional<py::dict<int64_t, py::bytes>>& images, const Item16& item) {
    Written w;
    w.ok = true;
    w.va = item.va;
    core16::FunctionNotes known = item.known ? *item.known : core16::FunctionNotes();   // dict(known or {})
    bool far_return = known.pointers.contains(S("return")) && known.pointers.at(S("return")) == 4;
    std::optional<py::bytes> code;
    if (IMAGES16().contains(item.seg)) code = IMAGES16().at(item.seg);
    ModuleFacts facts = module_facts16(code, item.off, item.end, item.distance, item.model, item.dgroup, far_return, item.seg);
    facts.selector_structs = item.by_selector;
    facts.callee_pops = item.callee_pops;
    facts.without_notes = true;   // (the module's C: no notes above a function)
    facts.selector_values = item.selector_values;
    facts.more = ::py::copy_dict(item.extra);
    w.text = decompiler::c_from_l2(std::nullopt, known, images, &facts);
    w.known = known;
    if (!known.from_l2) return w;   // (va, None, set(), known, {})
    w.found = facts;
    w.found.win16 = true;
    w.flags = cback::flags(doc_wire(known), *facts.wire);
    return w;
}

// the keys of an L2 `found` the second round keeps: the found ModuleFacts without what was only given
const py::str& note_failed_nodoc() {
    static const py::str s = S("it has no L2");
    return s;
}

}  // namespace

// ---- 16-bit ---------------------------------------------------------------------------------------

std::tuple<py::str, std::tuple<int64_t, py::str, py::str>> generation() {
    // (Python: a digest of the source of what makes the result; C++: the version of this port)
    return {base::cache::key({base::cache::Part(S("c_text.cpp 1"))}), decompiler::generation()};
}

py::str interrupt_helper_name(int64_t vector, const py::list<py::str>& regs) {
    return fmt("int%02x_", static_cast<int>(vector)) + (regs.empty() ? S("none") : py::join(S("_"), regs));
}

py::list<py::str> interrupt_helpers(const Interrupts& used) {
    py::list<py::str> out;
    if (used.empty()) return out;
    out.append(S("struct codebreaker_registers { uint2 ax, bx, cx, dx, si, di, ds, es; uint1 cf, zf; uint2 set; };"));
    out.append(S("extern struct codebreaker_registers codebreaker_int;   /* as the last interrupt left them */"));
    std::vector<py::str> regs = INTERRUPT_REGISTERS.vec();
    regs.push_back(S("cf"));
    regs.push_back(S("zf"));
    for (const py::str& r : regs) out.append(S("#define int_") + r.upper() + S(" codebreaker_int.") + r);
    for (const char* r : {"ax", "bx", "cx", "dx"}) {   // and their bytes: AL AH, BL BH ...
        py::str rs = S(r);
        py::str up = rs.slice(0, 1).upper();
        out.append(S("#define int_") + up + S("L (*(uint1 *)&codebreaker_int.") + rs + S(")"));
        out.append(S("#define int_") + up + S("H (*((uint1 *)&codebreaker_int.") + rs + S(" + 1))"));
    }
    std::set<int64_t> vectors;
    for (const Interrupt& i : used) vectors.insert(i.vector);
    for (int64_t v : vectors) out.append(fmt("void __far codebreaker_int%02x(void);              /* INT %02Xh */", static_cast<int>(v), static_cast<int>(v)));
    for (const Interrupt& i : used) {
        std::vector<py::str> ps, sets;
        int64_t mask = 0;
        for (const py::str& r : i.registers.vec()) {
            ps.push_back(S("uint2 ") + r);
            sets.push_back(S("codebreaker_int.") + r + S(" = ") + r + S(";"));
            mask += int64_t(1) << INTERRUPT_REGISTERS.index(r);
        }
        py::str params = ps.empty() ? S("void") : joins(ps, ", ");
        out.append(S("static uint4 ") + interrupt_helper_name(i.vector, i.registers) + S("(") + params + S(") { ") + joins(sets, " ") +
                   fmt(" codebreaker_int.set = 0x%02x; codebreaker_int%02x(); ", static_cast<int>(mask), static_cast<int>(i.vector)) +
                   S("return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }"));
    }
    return out;
}

// The structures of global selectors (backends/c._selector_structs' `selector_shared`): one per global for the whole module --
// the fields every function saw, together -- where they agree (one type per offset, none overlapping); where they do not, each
// function's own, as for a local selector. -> (declarations, {function: {the module's name: its own}})
std::tuple<py::list<py::str>, py::dict<py::str, py::dict<py::str, py::str>>> _shared_selector_structs(
    const py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>>& shared) {
    py::list<py::str> out;
    py::dict<py::str, py::dict<py::str, py::str>> renames;
    for (const py::str& name : py::sorted(py::list<py::str>(shared.keys())).vec()) {
        const py::dict<py::str, py::dict<int64_t, py::str>>& per = shared.at(name);
        py::dict<int64_t, py::str> every;
        bool agree = true;
        for (const auto& pv : per.items()) {
            for (const auto& fv : std::get<1>(pv).items()) {
                if (!every.contains(std::get<0>(fv))) every.set(std::get<0>(fv), std::get<1>(fv));
                else if (every.at(std::get<0>(fv)) != std::get<1>(fv)) {
                    agree = false;
                    break;
                }
            }
            if (!agree) break;
        }
        std::optional<py::list<py::str>> body;
        if (agree) body = cback::seg_layout(every);
        if (body) {
            out.append(S("typedef _Packed struct ") + name + S(" { ") + py::join(S(" "), *body) + S(" } ") + name + S(";"));
            continue;
        }
        for (const py::str& fname : py::sorted(py::list<py::str>(per.keys())).vec()) {
            py::str own = S("seg_") + py::re::sub("\\W", S("_"), fname) + S("_") + name.slice(4, name.size());
            std::optional<py::list<py::str>> lay = cback::seg_layout(per.at(fname));
            out.append(S("typedef _Packed struct ") + own + S(" { ") + py::join(S(" "), *lay) + S(" } ") + own + S(";"));
            if (!renames.contains(fname)) renames.set(fname, py::dict<py::str, py::str>());
            renames.at(fname).set(name, own);
        }
    }
    return {out, renames};
}

Module16 _compilable16(const py::str& module_path, const std::optional<py::str>& outdir) {
    decompiler::Decompiled16 dec = decompiler::decompile16(module_path, outdir);
    py::dict<py::str, py::str> texts = dec.texts;
    py::dict<int64_t, py::str> by_va = dec.by_va;
    std::shared_ptr<describe::Description> dp = dec.description;
    describe::Description& d = *dp;
    // the module made: one L2 Module (translate/l2.Made), each function a MethodDecl with its C, and
    // what the 16-bit build needs said of the module beside it
    Module16 m(py::os::path::basename(module_path).lower());
    m.notes = d.notes;   // what the decompiler said of each besides its C (the one dict: later changes show in both)
    py::dict<int64_t, int64_t> sizes;
    py::list<core16::StructNote> struct_notes;   // the structures the decompiler recovered
    std::vector<py::str> selector_structs;       // ... and those a segment holds (_selector_structs)
    py::str env_sel = py::os::getenv(S("CB_SELECTOR_STRUCTS"), S(""));
    bool by_selector = decompiler::_structs(d.path) == "1" && env_sel != "0";   // (with structures)
    py::dict<int64_t, int64_t> elems;                                           // ... a pointer's -> bytes of what it points at
    std::map<int64_t, int64_t> ends;
    std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> starts;         // where each function's code stops: at the next one
    for (int64_t va : py::sorted(py::list<int64_t>(d.where.keys())).vec())
        starts[std::get<0>(d.where.at(va))].emplace_back(std::get<1>(d.where.at(va)), va);
    for (auto& [seg, lst] : starts) {
        std::sort(lst.begin(), lst.end());
        for (size_t k = 0; k < lst.size(); ++k)
            ends[lst[k].second] = k + 1 < lst.size() ? lst[k + 1].first : d.images.get(seg, py::bytes()).size();
    }
    // each function's model as the description gives it: near or far is how it returns, and
    // the C must say it (the decompiler leaves the convention out of a head whose result is a
    // pointer -- and in the medium model an unmarked function is far)
    std::map<std::string, std::optional<py::str>> distance;
    std::map<std::string, py::str> model_of;
    for (const py::list<py::str>& parts : d.records(L({"F"})).vec()) {
        if (parts.size() > 4) {
            distance[parts[2].utf8()] = parts[4].endswith(S("far")) ? std::optional<py::str>(S("far"))
                                       : parts[4].endswith(S("near")) ? std::optional<py::str>(S("near")) : std::nullopt;
            if (parts[4].startswith(S("__stdcall16")) || parts[4].startswith(S("__cdecl16"))) model_of[parts[2].utf8()] = parts[4];
        }
    }
    std::optional<int64_t> dgroup;
    if (d.data_segment) dgroup = describe::selector(d.data_segment);
    py::dict<int64_t, py::bytes> images = d.images.copy();
    // (the segments given each worker once, as it starts; here they are shared by the threads)
    IMAGES16() = images;
    // Each function's C written from its L2 document by the C back end, with what the module says
    // of it (_write16, among workers: the back end's passes on the tree, its heading as the description and its
    // instructions give it)
    std::optional<py::dict<int64_t, py::bytes>> asm_images;
    if (decompiler::tree_images()) asm_images = decompiler::_segment_images(d.path);
    // the functions whose calls through pointers remove nothing themselves: the callee does (the
    // arguments are listed in stack order, so __stdcall lays them out as the original pushed
    // them; backends/c._callee_pops). Each call is kept by its own place -- the function the
    // description named for it may since have been found to be two: the call belongs to the one
    // that holds it, of every function the module will have (those the decompiler could not make
    // among them)
    std::vector<int64_t> sites;
    for (const py::list<py::str>& parts : d.records(L({"O"})).vec())
        if (parts[4].startswith(S("__stdcall"))) sites.push_back(py::int_parse(parts[1], 16));
    std::set<int64_t> starts_set;
    for (int64_t va : by_va.keys()) starts_set.insert(va);
    for (const py::list<py::str>& parts : d.records(L({"F"})).vec()) {
        int64_t va = py::int_parse(parts[1], 16);
        if (!by_va.contains(va) && !texts.contains(parts[2]) && d.where.contains(va)) starts_set.insert(va);
    }
    std::vector<int64_t> starts_all(starts_set.begin(), starts_set.end());
    std::set<int64_t> callee_pops;
    for (int64_t site : sites) {
        auto it = std::upper_bound(starts_all.begin(), starts_all.end(), site);   // bisect_right
        int64_t k = static_cast<int64_t>(it - starts_all.begin()) - 1;
        if (k >= 0 && (starts_all[static_cast<size_t>(k)] >> 16) == (site >> 16)) callee_pops.insert(starts_all[static_cast<size_t>(k)]);
    }
    // what the module says that every function is written with: its imports used for their value
    // (describe's selector_imports, the made-up number each is shown as)
    SelectorImports shared_selector_values = d.selector_imports;
    std::vector<Item16> items;
    for (int64_t va : py::sorted(py::list<int64_t>(by_va.keys())).vec()) {
        py::str name = by_va.at(va);
        core16::SegOff so = d.where.at(va);
        Item16 it;
        it.va = va;
        auto dd = distance.find(name.utf8());
        if (dd != distance.end()) it.distance = dd->second;
        auto mm = model_of.find(name.utf8());
        if (mm != model_of.end()) it.model = mm->second;
        it.seg = std::get<0>(so);
        it.off = std::get<1>(so);
        it.end = ends.count(va) ? ends[va] : it.off;
        it.dgroup = dgroup;
        if (d.notes.contains(name)) it.known = d.notes.at(name);
        it.by_selector = by_selector;
        it.callee_pops = callee_pops.count(va) > 0;
        it.selector_values = shared_selector_values;
        items.push_back(it);
    }
    py::dict<int64_t, Written> written;
    {
        jobs::pool::Options<Item16, Written> o;
        o.below_normal = false;
        written = jobs::pool::run(
            [](const std::optional<py::dict<int64_t, py::bytes>>& ctx, const Item16& it) { return _write16(ctx, it); },
            py::list<Item16>(items), asm_images, [](const Item16& it) { return it.va; }, o);
    }
    for (const Item16& item : items)   // (a worker that could not start or died is done here, as before)
        if (!written.contains(item.va) || !written.at(item.va).ok) written.set(item.va, _write16(asm_images, item));
    // A function the decompiler could not make stays a function of the module: a body that says
    // so, which the build replaces by the function's own instructions where it was written in
    // assembly, and otherwise reports as not rebuilt (m.failed). So is one whose C the back end
    // could not write from its L2 -- an Unsupported node, where the core's PrintL2 has no node
    // yet (GDI's sub_22_0B96): the passes the C is made with are the back end's, on the tree.
    std::map<std::string, std::set<std::string>> flags_of;
    std::map<std::string, ModuleFacts> found_of;
    std::vector<std::pair<int64_t, py::str>> by_va_items;
    for (const auto& kv : by_va.items()) by_va_items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    for (const auto& [va, nm] : by_va_items) {
        Written& wr = const_cast<Written&>(written.at(va));
        for (const Interrupt& i : wr.found.interrupts) m.interrupts.insert(i);
        found_of[nm.utf8()] = wr.found;
        d.notes.set(nm, wr.known);
        if (!wr.text) {
            m.failed.set(nm, wr.known.l2 ? S("its C could not be written from L2") : S("it has no L2"));
            texts.pop(nm);
            by_va.pop(va);
        } else {
            texts.set(nm, *wr.text);
            std::set<std::string> fl;
            for (const py::str& f : wr.flags) fl.insert(f.utf8());
            flags_of[nm.utf8()] = fl;
        }
    }
    for (const py::list<py::str>& parts : d.records(L({"F"})).vec()) {
        int64_t va = py::int_parse(parts[1], 16);
        py::str nm = parts[2];
        if (!by_va.contains(va) && !texts.contains(nm) && d.where.contains(va)) {
            py::str model = (parts.size() > 4 && parts[4] != "inline") ? parts[4] : S("__cdecl16near");
            core16::SegOff so = d.where.at(va);
            ModuleFacts facts = module_facts16(images.contains(std::get<0>(so)) ? std::optional<py::bytes>(images.at(std::get<0>(so))) : std::nullopt,
                                               std::get<1>(so), ends.count(va) ? ends[va] : std::get<1>(so), std::nullopt, std::nullopt, dgroup,
                                               false, std::get<0>(so));
            bool far = model.contains(S("__stdcall16far")) || model.contains(S("__cdecl16far"));
            std::vector<py::str> words{model};
            if (far && facts.export_) words.push_back(S("__export"));
            if (facts.loadds) words.push_back(S("__loadds"));
            texts.set(nm, S("xunknown2 ") + joins(words, " ") + S(" ") + nm +
                              S("()\n{\n  /* the decompiler could not make this function */\n  return 0;\n}"));
            py::str head = S("xunknown2 ") + joins(words, " ") + S(" ") + nm;   // (its heading, as the back end reports one)
            ModuleFacts fo;
            Heading h;
            h.params = std::make_tuple(head, py::list<std::tuple<py::str, py::str>>());
            h.declaration = head + S("();");
            h.fastcall = head.contains(S("__fastcall16"));
            fo.heading = h;
            found_of[nm.utf8()] = fo;
            std::set<std::string> fl;
            if (far && facts.export_) fl.insert("exported_prologue");
            if (!far) fl.insert("near");
            flags_of[nm.utf8()] = fl;
            by_va.set(va, nm);
            if (!m.failed.contains(nm)) m.failed.set(nm, S("the decompiler could not make it"));
        }
    }
    // helpers that work in their caller's frame: its places passed to them -- what the module
    // decides from what writing each function found (backends/c.caller_frame_plan), and the
    // functions it names written again with it
    Wire frames = Wire::dict();
    for (const auto& kv : by_va.items()) {
        int64_t va = std::get<0>(kv);
        const py::str& nm = std::get<1>(kv);
        bool keeps = false;
        if (d.where.contains(va)) {
            int64_t seg = std::get<0>(d.where.at(va)), off = std::get<1>(d.where.at(va));
            keeps = d.images.contains(seg) && _keeps_frame(d.images.at(seg), off);
        }
        frames.set(Ws(nm), Wire(keeps));
    }
    Wire reports = Wire::dict();
    for (const py::str& nm : texts.keys()) {
        auto f = found_of.find(nm.utf8());
        Wire fr;
        if (f != found_of.end() && f->second.more.has("frame")) fr = f->second.more.get("frame");
        reports.set(Ws(nm), fr);
    }
    Wire plan = cback::caller_frame_plan(reports, frames);
    auto plan_update = [&](const py::str& nm, const Wire& extra) {
        Wire cur = plan.has(Ws(nm)) ? plan.get(Ws(nm)) : Wire::dict();
        if (!plan.has(Ws(nm))) plan.set(Ws(nm), cur);
        for (const auto& kv : extra.items()) cur.set(kv.first, kv.second);
    };
    // ... and the module's own segments as values, where the function's code holds a loader fix-up
    // for one (backends/c._segment_values: which function is far is known once each is written)
    std::set<std::string> near_;
    for (const auto& kv : flags_of)
        if (kv.second.count("near")) near_.insert(kv.first);

    {
        std::vector<std::tuple<int64_t, py::str>> sorted_items(by_va_items.begin(), by_va_items.end());
        std::vector<std::pair<int64_t, py::str>> cur;
        for (const auto& kv : by_va.items()) cur.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(cur.begin(), cur.end());
        std::vector<Wire> fns, nearw;
        for (const auto& [va, nm] : cur) fns.push_back(Wire::tuple({Wi(va), Ws(nm)}));
        for (const std::string& n : near_) nearw.push_back(Ws(S(n)));
        Wire where_w = Wire::dict();
        for (const auto& kv : d.where.items())
            where_w.set(Wi(std::get<0>(kv)), Wire::tuple({Wi(std::get<0>(std::get<1>(kv))), Wi(std::get<1>(std::get<1>(kv)))}));
        Wire sites_w = Wire::dict();
        for (const auto& [seg, lst] : _selector_sites(module_path)) {
            std::vector<Wire> l;
            for (const auto& p : lst) l.push_back(Wire::tuple({Wi(p.first), Wi(p.second)}));
            sites_w.set(Wi(seg), Wire::list(l));
        }
        Wire code_w = Wire::dict();
        for (const auto& kv : d.code.items()) code_w.set(Wi(std::get<0>(kv)), Wi(std::get<1>(kv)));
        std::vector<Wire> other;
        for (int64_t sg : d.images.keys())
            if (!d.code.contains(sg) && sg != d.data_segment) other.push_back(Wi(sg));
        Wire extras = cback::segment_plan(Wire::list(fns), Wire::set(nearw), where_w, sites_w, code_w, Wi(d.data_segment),
                                          Wire::set(other), Wi(describe::FIRST_SELECTOR));
        for (const auto& kv : extras.items()) plan_update(as_str(kv.first), kv.second);
    }
    // ... and the data a function reads through CS from beside its code: by name, in its own code
    // segment (backends/c._code_segment_reads finds the numbers, _code_segment_data lays it out)
    CodeData cd;
    if (py::os::getenv(S("CB_CODE_DATA"), S("")) != "0") {
        std::vector<std::pair<py::str, std::optional<Wire>>> reports16;
        std::vector<std::pair<int64_t, py::str>> cur;
        for (const auto& kv : by_va.items()) cur.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(cur.begin(), cur.end());
        std::map<std::string, std::optional<core16::SegOff>> where16;
        for (const auto& [va, nm] : cur) {
            auto f = found_of.find(nm.utf8());
            std::optional<Wire> cr;
            if (f != found_of.end() && f->second.more.has("code_reads")) cr = f->second.more.get("code_reads");
            reports16.emplace_back(nm, cr);
            where16[nm.utf8()] = d.where.contains(va) ? std::optional<core16::SegOff>(d.where.at(va)) : std::nullopt;
        }
        cd = _code_segment_data(reports16, where16, d, module_path);
    }
    for (const auto& [nm, extra] : cd.plan) plan_update(nm, extra);
    // ... and the structures of global selectors: one for the module where every function's fields
    // agree, else each function's own -- its uses then written by that name (backends/c
    // ._selector_structs, given "selector_renames")
    py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>> shared_selectors;
    {
        std::vector<std::pair<int64_t, py::str>> cur;
        for (const auto& kv : by_va.items()) cur.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(cur.begin(), cur.end());
        for (const auto& [va, nm] : cur) {
            auto f = found_of.find(nm.utf8());
            if (f == found_of.end()) continue;
            for (const auto& kv : f->second.selector_shared.items()) {
                if (!shared_selectors.contains(std::get<0>(kv))) shared_selectors.set(std::get<0>(kv), py::dict<py::str, py::dict<int64_t, py::str>>());
                for (const auto& fk : std::get<1>(kv).items()) shared_selectors.at(std::get<0>(kv)).set(std::get<0>(fk), std::get<1>(fk));   // (.update)
            }
        }
    }
    auto [shared_decls, renames] = _shared_selector_structs(shared_selectors);
    for (const auto& kv : renames.items()) {
        Wire names = Wire::dict();
        for (const auto& p : std::get<1>(kv).items()) names.set(Ws(std::get<0>(p)), Ws(std::get<1>(p)));
        Wire one = Wire::dict();
        one.set("selector_renames", names);
        plan_update(std::get<0>(kv), one);
    }
    std::map<std::string, const Item16*> item_of;
    for (const Item16& item : items)
        if (by_va.contains(item.va)) item_of[by_va.at(item.va).utf8()] = &item;
    for (const auto& kv : plan.items()) {
        py::str nm = as_str(kv.first);
        auto it = item_of.find(nm.utf8());
        if (it == item_of.end()) continue;
        Item16 again = *it->second;
        again.extra = kv.second;   // dict(shared, **extra)
        Written wr = _write16(asm_images, again);
        if (wr.text) {
            texts.set(nm, *wr.text);
            found_of[nm.utf8()] = wr.found;
        }
    }
    for (const auto& kv : d.notes.items())
        if (std::get<1>(kv).l2) m.l2.set(std::get<0>(kv), *std::get<1>(kv).l2);   // each one's L2 document
    std::vector<int64_t> final_vas = py::sorted(py::list<int64_t>(by_va.keys())).vec();
    for (int64_t va : final_vas) {
        py::str name = by_va.at(va);
        py::str text = texts.at(name).strip();
        ModuleFacts found = found_of.count(name.utf8()) ? found_of[name.utf8()] : ModuleFacts();
        core16::FunctionNotes known = d.notes.contains(name) ? d.notes.at(name) : core16::FunctionNotes();
        for (const core16::StructNote& s : known.structs.vec()) struct_notes.append(s);
        for (const core16::GlobalNote& g : known.globals.vec()) {
            sizes.set(g.address, g.bytes <= 4 ? std::max<int64_t>(sizes.get(g.address, 0), g.bytes) : sizes.get(g.address, 2));
            if (g.elem == 1 || g.elem == 2 || g.elem == 4) elems.set(g.address, g.elem);
        }
        if (flags_of.count(name.utf8())) {
            for (const std::string& flag : flags_of[name.utf8()]) {
                if (flag == "near") m.near.add(name);
                else if (flag == "wide") m.wide.add(name);
                else if (flag == "novalue") m.novalue.add(name);
                else if (flag == "exported_prologue") m.exported_prologue.add(name);
                else throw py::AttributeError("'Made' object has no attribute '" + flag + "'");
            }
        }
        for (const py::str& sd : found.selector_decls.vec()) selector_structs.push_back(sd);
        m.add(name, text, va);
        m.where.set(va, d.where.contains(va) ? std::optional<core16::SegOff>(d.where.at(va)) : std::nullopt);
        // (an import's value used bare is the import: backends/c._selector_imports)
        for (const auto& kv : found.selector_imports.items()) m.selector_imports.set(std::get<0>(kv), std::get<1>(kv));
    }
    std::set<int64_t> other_data;
    for (int64_t sg : d.images.keys())
        if (!d.code.contains(sg) && sg != d.data_segment) other_data.insert(sg);
    for (const auto& kv : cd.defs) m.code_data.set(kv.first, kv.second);
    for (const auto& kv : cd.report) m.code_reads.set(kv.first, kv.second);
    // what the functions name and call, as each was written (backends/c._names_used)
    std::vector<NamesUsed> used;
    for (const py::str& n : m.order().vec()) {
        auto f = found_of.find(n.utf8());
        used.push_back(f != found_of.end() && f->second.names ? *f->second.names : NamesUsed());
    }
    std::set<std::string> called;
    for (const NamesUsed& u : used)
        for (const py::str& c : u.called) called.insert(c.utf8());
    m.imports = py::dict<py::str, std::tuple<py::str, int64_t>>();
    py::dict<py::str, py::str> mtexts = m.texts();
    for (const py::str& nm : d.imports.vec()) {
        if (called.count(nm.utf8()) && !mtexts.contains(nm)) {
            std::optional<py::re::Match> mm = py::re::fullmatch("(.+)_(\\d+)", nm);
            if (mm) m.imports.set(nm, std::make_tuple(mm->group(1), py::int_parse(mm->group(2), 10)));
        }
    }
    // Globals the decompiler names by address. In the data segment: an offset through DS, as in
    // the original. In another segment (constants kept beside the code, a second data segment):
    // the module carries that segment's bytes, and the global is a place in them.
    std::vector<py::str> macros;
    m.segments = py::dict<int64_t, py::bytes>();
    for (int64_t sg : other_data) {   // (a segment's selector used as a value)
        bool any = false;
        for (const NamesUsed& u : used) any = any || u.segments.contains(sg);
        if (any) m.segments.set(sg, d.images.at(sg));
    }
    // a global the code calls through holds a function pointer -- near or far by its size
    std::set<std::string> called_through;
    for (const NamesUsed& u : used)
        for (const py::str& c : u.called_through) called_through.insert(c.utf8());
    std::set<std::tuple<std::string, std::string>> globals_used;
    for (const NamesUsed& u : used)
        for (const auto& g : u.globals) globals_used.emplace(std::get<0>(g).utf8(), std::get<1>(g).utf8());
    py::dict<int64_t, py::bytes> mby_images = d.images;
    for (const auto& [prefix_s, addr_s] : globals_used) {
        py::str prefix = S(prefix_s), addr = S(addr_s);
        int64_t va = py::int_parse(addr, 16);
        core16::SegOff so = describe::where(va);
        int64_t seg = std::get<0>(so), off = std::get<1>(so);
        py::str ty = _global_type(prefix, sizes.get(va, 2), elems.contains(va) ? std::optional<int64_t>(elems.at(va)) : std::nullopt);
        py::str key = prefix + S("Ram") + addr;
        if (called_through.count(key.utf8())) ty = sizes.get(va, 2) == 4 ? S("code __far *") : S("code __near *");
        auto selimp = d.selector_imports.contains(va >> 16);
        if (seg == d.data_segment || (va >> 16) == 0) {
            // (segment 0: an address the core folded to a constant -- a table index after it
            // copied a shared tail, LZEXPAND's sub_11_00AB `cRam00000095` -- an offset through
            // DS like the rest; it was declared nowhere and the C did not compile)
            macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" *)0x") + X((va >> 16) == 0 ? (va & 0xFFFF) : off) + S("))"));
        } else if ((va >> 16) == 0x40) {
            // through the selector 0040h itself: the BIOS's data, which Windows keeps at that selector
            macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" __far *)MK_FP(0x40, 0x") + X(va & 0xFFFF) + S("))"));
            m.bios_data = true;
        } else if (selimp) {
            // a place in a segment the module gets as an import (KERNEL's __0040H: the BIOS data):
            // the import's selector, which the loader fills in, and the offset
            const auto& [imp, how] = d.selector_imports.at(va >> 16);
            m.selector_imports.set(imp, how);
            macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" __far *)MK_FP(") + S(how == "segment" ? "FP_SEG" : "FP_OFF") + S("(&") + imp +
                             S("), 0x") + X(va & 0xFFFF) + S("))"));
        } else {
            bool far_in_seg = false;
            std::optional<py::str> far_name;
            if (d.code.contains(seg)) {
                for (const auto& kv : m.by_va().items()) {
                    int64_t v = std::get<0>(kv);
                    const py::str& n = std::get<1>(kv);
                    if (m.where.contains(v) && m.where.at(v) && std::get<0>(*m.where.at(v)) == seg && !m.near.contains(n)) {
                        far_in_seg = true;
                        if (!far_name || n < *far_name) far_name = n;
                    }
                }
            }
            if (far_in_seg) {
                // in one of the module's code segments (a variable kept beside the code): that
                // segment itself, by the selector of a far function in it -- not a copy of its bytes,
                // which the code there would never see written
                macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" __far *)(((uint4)(uint2)((uint4)(void __far *)&") + *far_name +
                                 S(" >> 16) << 16) | 0x") + X(off) + S("))"));
            } else if (d.images.contains(seg) && off < d.images.at(seg).size()) {
                m.segments.set(seg, d.images.at(seg));
                macros.push_back(S("#define ") + key + S(" (*(") + ty + S(" __far *)(codebreaker_segment_") + D(seg) + S(" + 0x") + X(off) + S("))"));
            }
        }
    }
    // (data.c, compiled with CODEBREAKER_DATA, defines these segments with their pointers in
    // place: the declaration as bytes is for the code)
    std::vector<py::str> decls;
    for (int64_t n : py::sorted(py::list<int64_t>(m.segments.keys())).vec())
        decls.push_back(S("#ifndef CODEBREAKER_DATA\nextern uint1 __based(__segname(\"CODEBREAKER_SEGMENT_") + D(n) +
                        S("\")) codebreaker_segment_") + D(n) + S("[];\n#endif"));
    if (!m.selector_imports.empty()) {   // imports used for their value
        decls.push_back(S("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))"));
        decls.push_back(S("#define FP_SEG(p) ((uint2)((uint4)(void __far *)(p) >> 16))"));
        decls.push_back(S("#define FP_OFF(p) ((uint2)(uint4)(void __far *)(p))"));
        for (const py::str& n : py::sorted(py::list<py::str>(m.selector_imports.keys())).vec())
            decls.push_back(S("extern uint1 __far ") + n + S(";  /* an import: its ") + m.selector_imports.at(n) + S(" is the value */"));
        for (const py::str& n : m.selector_imports.keys()) {
            std::optional<py::re::Match> mm = py::re::fullmatch("(.+)_(\\d+)", n);
            if (mm) m.imports.set(n, std::make_tuple(mm->group(1), py::int_parse(mm->group(2), 10)));
        }
    }
    for (const py::str& nm : py::sorted(py::list<py::str>(m.imports.keys())).vec())
        if (!m.selector_imports.contains(nm)) decls.push_back(S("extern uint4 __far __stdcall ") + nm + S("();"));
    // each function declared: with its parameters where every call agrees with its definition
    // (backends/c.prototypes, from each one's heading and calls as the back end wrote them)
    py::list<py::str> order_now = m.order();
    Wire headings_w = Wire::dict();
    std::map<std::string, Heading> headings;
    for (const py::str& n : order_now.vec()) {
        auto f = found_of.find(n.utf8());
        if (f != found_of.end() && f->second.heading) {
            headings[n.utf8()] = *f->second.heading;
            headings_w.set(Ws(n), heading_wire(*f->second.heading));
        } else {
            headings_w.set(Ws(n), Wire::dict());
        }
    }
    py::dict<py::str, py::str> agreed;
    if (py::os::getenv(S("CB_PROTOTYPES"), S("1")) != "0") agreed = cback::prototypes(headings_w, order_now);
    for (const py::str& a : agreed.keys()) m.prototyped.add(a);
    // (each function's declaration with its argument list left open, from its heading as the back
    // end wrote it from L2: the module's public header declares its exports with them -- build/module)
    for (const py::str& n : order_now.vec()) {
        if (!headings.count(n.utf8())) throw py::KeyError("declaration");
        m.declarations.set(n, headings[n.utf8()].declaration);
    }
    for (const py::str& n : order_now.vec()) {
        py::str a = agreed.get(n, S(""));
        decls.push_back(!a.empty() ? a : headings[n.utf8()].declaration);
    }
    for (const py::str& n : order_now.vec())
        if (headings[n.utf8()].fastcall) decls.push_back(S("#ifdef __WATCOMC__\n#pragma aux (codebreaker_fastcall) ") + n + S(";\n#endif"));
    std::vector<py::str> intrinsics;
    for (const py::str& n : py::sorted(py::list<py::str>(INTRINSICS16.keys())).vec())
        if (called.count(n.utf8())) intrinsics.push_back(INTRINSICS16.at(n));
    for (const py::str& h : interrupt_helpers(m.interrupts).vec()) intrinsics.push_back(h);
    bool any_mk_fp = false;
    for (const NamesUsed& u : used) any_mk_fp = any_mk_fp || u.mk_fp;
    if ((any_mk_fp || m.bios_data) && m.selector_imports.empty())
        intrinsics.push_back(S("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))"));
    for (const py::str& s : shared_decls.vec()) selector_structs.push_back(s);
    if (!selector_structs.empty()) intrinsics.push_back(SEG_PTR);
    std::vector<py::str> all = intrinsics;
    for (const py::str& x : macros) all.push_back(x);
    for (const py::str& x : _struct_decls(struct_notes, HEADER, true)) all.push_back(x);
    for (const py::str& x : selector_structs) all.push_back(x);
    for (const py::str& x : cd.decls) all.push_back(x);
    for (const py::str& x : decls) all.push_back(x);
    m.set_header(HEADER + S("\n") + joins(all, "\n") + S("\n"));
    m.description = dp;
    return m;
}

// The module made compilable (see _compilable16). (Python kept the result in the build cache, pickled -- see the top of
// this file: here it is made again; the decompiler's own output, the expensive part, is kept.)
Module16 compilable16(const py::str& module_path, const std::optional<py::str>& outdir) {
    Module16 m = _compilable16(module_path, outdir);
    // (its description is written where it was asked for, as when it is made)
    return m;
}

// ---- PowerPC --------------------------------------------------------------------------------------

py::dict<py::str, int64_t> call_counts(const py::dict<py::str, py::set<int64_t>>& calls, const py::set<py::str>& names) {
    py::dict<py::str, int64_t> out;
    for (const auto& kv : calls.items())
        if (names.contains(std::get<0>(kv)) && std::get<1>(kv).size() == 1) out.set(std::get<0>(kv), *std::get<1>(kv).begin());
    return out;
}

ModulePPC compilable_powerpc(const py::str& module_path, const py::str& outdir) {
    py::ref<formats::powerpc_image::Image> img = formats::powerpc_image::read(module_path);
    // (the L2 documents kept, and each function's C written from its own here, by the C back end
    // with the PowerPC path's passes -- backends/c.POWERPC; the facts' lane says which)
    decompiler::Decompiled got = decompiler::raw(module_path, describe::function_starts(module_path), outdir, 3600, true, false);
    py::dict<py::str, py::str> out = got.texts;
    py::list<py::str> order = got.order;
    py::dict<int64_t, py::str> by_va = got.by_va;
    py::dict<int64_t, py::set<int64_t>> sizes;
    py::set<py::str> novalue;
    py::dict<py::str, native_l2::ModuleNames> named;
    py::dict<py::str, py::str> declared;
    py::set<py::str> order_set;
    for (const py::str& n : order.vec()) order_set.add(n);
    for (const py::str& n : order.vec()) {
        core16::FunctionNotes empty;
        core16::FunctionNotes& facts = got.notes.contains(n) ? const_cast<core16::FunctionNotes&>(got.notes.at(n)) : empty;
        for (const core16::GlobalNote& g : facts.globals.vec()) {
            if (!sizes.contains(g.address)) sizes.set(g.address, py::set<int64_t>());
            sizes.at(g.address).add(g.bytes);
        }
        ModuleFacts written;
        written.lane = S("powerpc");
        written.functions = order_set;
        if (facts.l2) out.set(n, *decompiler::c_from_l2(out.at(n), facts, std::nullopt, &written));
        if (!facts.from_l2) {
            // (a function the back end declines -- a node it does not write -- is one the
            // decompiler could not make, as one whose decompile failed: no C, so no declaration,
            // and the build leaves it to the original)
            if (facts.l2) out.set(n, S("/* ") + n + S(": its C could not be written from L2 */"));
            out.set(n, out.at(n).strip());
            continue;
        }
        named.set(n, cback::module_names32(doc_wire(facts)));   // (what its tree names, as its text does)
        declared.set(n, *written.declared);
        out.set(n, out.at(n).strip());
        if (written.novalue) {
            // (every function leaves something in r3 and callers are free to use it: a `void`
            // result is uint8 -- the back end's, backends/c._result_shown -- and the module notes it)
            novalue.add(n);
        }
    }
    py::set<py::str> names, called_through, called;
    for (const auto& kv : named.items()) {
        const native_l2::ModuleNames& x = std::get<1>(kv);
        for (const py::str& s : x.names) names.add(s);
        for (const py::str& s : x.called_through) called_through.add(s);
        for (const py::str& s : x.calls) called.add(s);
    }
    std::vector<py::str> macros;
    std::set<std::tuple<std::string, std::string>> ramnames;
    for (const py::str& n2 : names) {
        std::optional<py::re::Match> m2 = RAM().fullmatch(n2);
        if (m2) ramnames.emplace(m2->group(1).utf8(), m2->group(2).utf8());
    }
    for (const auto& [pre_s, addr_s] : ramnames) {
        py::str pre = S(pre_s), addr = S(addr_s);
        int64_t a = py::int_parse(addr, 16);
        int64_t size = 4;
        bool seen = false;
        if (sizes.contains(a) && !sizes.at(a).empty()) {
            for (int64_t x : sizes.at(a))
                if (x <= 8 && (!seen || x > size)) {
                    size = x;
                    seen = true;
                }
        }
        macros.push_back(S("#define ") + pre + S("Ram") + addr + S(" (*(") + _ram_type(pre, size, called_through.contains(pre + S("Ram") + addr)) + S(" *)0x") +
                         addr + S(")"));
    }
    // a call to a place in the original the decompiler was not told is a function: called there,
    // in the original, which is in memory beside the rebuilt image when it is judged
    int64_t lo_image = INT64_MAX, hi_image = INT64_MIN;
    for (const auto& s : img->spans.vec()) {
        lo_image = std::min(lo_image, std::get<0>(s));
        hi_image = std::max(hi_image, std::get<0>(s) + std::get<2>(s));
    }
    std::set<std::string> func_addrs;
    for (const py::str& n2 : names)
        if (py::re::fullmatch("func_0x[0-9a-f]{8}", n2)) func_addrs.insert(n2.slice(7, n2.size()).utf8());
    for (const std::string& a : func_addrs) {
        py::str addr = S(a);
        int64_t v = py::int_parse(addr, 16);
        if (lo_image <= v && v < hi_image && !out.contains(S("func_0x") + addr))
            macros.push_back(S("#define func_0x") + addr + S(" ((code *)0x") + addr + S(")"));
    }
    std::vector<py::str> used_v, decls;
    for (const py::str& n : py::sorted(called).vec())
        if (py::re::fullmatch("import\\d+_ord_\\d+", n)) used_v.push_back(n);
    for (const py::str& n : used_v) decls.push_back(S("uint8 ") + n + S("();"));
    for (const py::str& n : order.vec()) {
        py::str head = declared.get(n, S(""));
        if (!head.contains(S("("))) continue;
        decls.push_back(head.slice(0, head.index(S("("))) + S("();"));
    }
    // The structures recovered for each function (its facts), declared before the prototypes:
    // a function returning a pointer to one names it in its prototype, which every body includes.
    py::str types = _TYPES.replace(S("typedef int code();"), S("typedef uint8 code();")) + _HELPERS + _POWERPC;
    py::list<core16::StructNote> all_structs;
    for (const py::str& n : order.vec())
        if (got.notes.contains(n))
            for (const core16::StructNote& st : got.notes.at(n).structs.vec()) all_structs.append(st);
    std::vector<py::str> structs = _struct_decls(all_structs, types);
    ModulePPC m(py::os::path::basename(module_path).lower(),
                types + lines_of_structs(structs) + joins(decls, "\n") + S("\n") + joins(macros, "\n") + S("\n"), out, order, by_va);
    m.novalue = novalue;
    py::dict<py::str, int64_t> by_name;
    for (const auto& kv : img->imports.items()) by_name.set(std::get<1>(kv), std::get<0>(kv));
    for (const py::str& n : used_v)
        if (by_name.contains(n)) m.externs.set(n, by_name.at(n));
    for (const auto& kv : named.items()) {
        for (const auto& ar : std::get<1>(kv).arity.vec()) {
            if (!m.calls.contains(std::get<0>(ar))) m.calls.set(std::get<0>(ar), py::set<int64_t>());
            m.calls.at(std::get<0>(ar)).add(std::get<1>(ar));
        }
    }
    py::set<py::str> extern_names;
    for (const py::str& k : m.externs.keys()) extern_names.add(k);
    m.argc = call_counts(m.calls, extern_names);
    m.image = img;
    return m;
}

// Assembly for the routines the interrupt helpers call: codebreaker_int<n> loads the registers the helper set
// (codebreaker_int.set, a bit each in INTERRUPT_REGISTERS' order) from codebreaker_int -- the others keep what they hold, DS
// and ES the caller's -- takes INT n, and stores every register and CF and ZF back.
// -> (assembly text, C text that defines codebreaker_int)
std::tuple<py::str, py::str> interrupt_routines(const py::set<int64_t>& vectors) {
    std::map<std::string, int> off, bit;
    for (int i = 0; i < INTERRUPT_REGISTERS.size(); ++i) {
        off[INTERRUPT_REGISTERS[i].utf8()] = 2 * i;
        bit[INTERRUPT_REGISTERS[i].utf8()] = 1 << i;
    }
    std::vector<py::str> out{S("; the software interrupts the module takes, each as the original takes it: the registers"),
                             S("; from codebreaker_int, INT n, the registers and the carry and zero flags back into it"),
                             S(".286"), S("DGROUP group _DATA"), S("_DATA segment word public 'DATA'"), S("        extrn _codebreaker_int:word"),
                             S("_DATA ends"), S("_TEXT segment word public 'XCODE'"), S("        assume cs:_TEXT, ds:DGROUP")};
    std::vector<int64_t> vs(vectors.begin(), vectors.end());
    std::sort(vs.begin(), vs.end());
    for (int64_t v : vs) {
        py::str name = fmt("_codebreaker_int%02x", static_cast<int>(v));
        for (const py::str& l : {S("        public ") + name, name + S(" proc far"), S("        push bp"), S("        push si"), S("        push di"),
                                 S("        push ds"), S("        push es"),
                                 S("        mov  bp, ds                                   ; DGROUP, to come back to"),
                                 S("        push ds                                       ; DS as the caller has it")})
            out.push_back(l);
        const char* order8[] = {"es", "ax", "bx", "cx", "dx", "si", "di", "ds"};
        for (int k = 0; k < 8; ++k) {
            std::string r = order8[k];
            py::str skip = fmt("L_%02x_%d", static_cast<int>(v), k);
            out.push_back(S("        test word ptr _codebreaker_int+18, ") + D(bit[r]));
            out.push_back(S("        jz   short ") + skip);
            if (r == "ds") {   // (DS last: it is what the others are read through)
                out.push_back(S("        pop  ax"));
                out.push_back(S("        push word ptr _codebreaker_int+") + D(off["ds"]));
            } else {
                out.push_back(S("        mov  ") + S(r) + S(", word ptr _codebreaker_int+") + D(off[r]));
            }
            out.push_back(skip + S(":"));
        }
        py::str lax = fmt("L_%02x_ax", static_cast<int>(v));
        for (const py::str& l : {S("        test word ptr _codebreaker_int+18, ") + D(bit["ax"]), S("        jz   short ") + lax,
                                 S("        mov  ax, word ptr _codebreaker_int+") + D(off["ax"]), lax + S(":"), S("        pop  ds"),
                                 fmt("        int  %03Xh", static_cast<int>(v)), S("        pushf"), S("        push ds"), S("        mov  ds, bp")})
            out.push_back(l);
        for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "es"})
            out.push_back(S("        mov  word ptr _codebreaker_int+") + D(off[r]) + S(", ") + S(r));
        for (const py::str& l : {S("        pop  word ptr _codebreaker_int+") + D(off["ds"]), S("        pop  ax"),
                                 S("        mov  byte ptr _codebreaker_int+16, al"), S("        and  byte ptr _codebreaker_int+16, 1"),
                                 S("        shr  ax, 6"), S("        and  al, 1"), S("        mov  byte ptr _codebreaker_int+17, al"),
                                 S("        pop  es"), S("        pop  ds"), S("        pop  di"), S("        pop  si"), S("        pop  bp"),
                                 S("        retf"), name + S(" endp")})
            out.push_back(l);
    }
    out.push_back(S("_TEXT ends"));
    out.push_back(S("end"));
    out.push_back(S(""));
    py::str c = S("#include \"m.h\"\nstruct codebreaker_registers codebreaker_int;\n");
    return {joins(out, "\n"), c};
}

}  // namespace codebreaker::decompile::c_text
