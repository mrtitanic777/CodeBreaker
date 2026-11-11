// codebreaker/exact/draft/prove.cpp -- typed C++ from codebreaker.exact.draft.prove (see prove.hpp).
#include "codebreaker/exact/draft/prove.hpp"

#include <process.h>

#include <algorithm>
#include <cstdlib>
#include <map>
#include <set>
#include <variant>
#include <vector>

#include "codebreaker/exact/aim.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/draft/shapes.hpp"
#include "codebreaker/exact/draft/text.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/x86.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::exact::draft::prove {

namespace {

using py::str;
using Lines = py::list<str>;
using Match = py::re::Match;
using OMatch = std::optional<Match>;
namespace S = shapes;
namespace F = forms;
namespace C = candidates;
namespace T = text;
using Prj = forms::Prj;

str operator""_s(const char* p, size_t n) { return str(std::string(p, n)); }
template <class... A> str pf(const char* fmt, const A&... a) { return py::percent(str(std::string(fmt)), a...); }
bool nb(const str& x) { return !x.strip().empty(); }
str esc(const str& s) { return py::re::escape(s); }
int64_t gi(const Match& m, int64_t g) { return py::int_from_str(m.group(g)); }
str join_nl(const Lines& l) { return py::join("\n"_s, l); }
Lines split_nl(const str& t) { return py::split(t, "\n"_s); }

// prj.image[lo:hi] (a slice: clamped, as Python's)
py::bytes image_slice(const Prj& prj, int64_t lo, int64_t hi) {
    const std::string& all = prj.image.raw();
    int64_t n = static_cast<int64_t>(all.size());
    if (lo < 0) lo = std::max<int64_t>(0, n + lo);
    if (hi < 0) hi = std::max<int64_t>(0, n + hi);
    lo = std::min(lo, n);
    hi = std::min(hi, n);
    if (hi <= lo) return py::bytes();
    return py::bytes(all.substr(static_cast<size_t>(lo), static_cast<size_t>(hi - lo)));
}

const py::re::Pattern& errline() {
    static const py::re::Pattern p = py::re::compile(R"re(\((\d+)\) : (?:error \w+: )?(.*)$)re"_s, py::re::M);
    return p;
}

// the errors the compiler printed: [(line, message)], warnings left out
std::vector<std::pair<int64_t, str>> errors_of(const str& con) {
    std::vector<std::pair<int64_t, str>> errs;
    for (const Match& m : errline().finditer(con))
        if (!m.group(2).contains("warning"_s)) errs.push_back({gi(m, 1), m.group(2)});
    return errs;
}

bool byteptr(const str& p) { return T::BYTEPTR.contains(std::make_tuple(p, int64_t(0))); }

}  // namespace

const int64_t MAXCANDS = 64;
const int64_t FUNC_BUDGET = 150;   // (compiles a function's candidates may take in all)
const std::tuple<double, double> HOPELESS{0.25, 0.6};   // (a length off by more than this part of the function's,
const int64_t BEAM = 4;   // (the readings the search carries on with each round)
const int64_t REFINE_BUDGET = 240;   // (the compiles it may spend on one function)

namespace {
int64_t batch_size() {   // (candidates compiled in one run: batch.py; 0 none)
    static const int64_t b = [] {
        auto v = py::os::environ_::get("CB_BATCH"_s);
        return v ? py::int_from_str(*v) : int64_t(16);
    }();
    return b;
}
bool aim_on() {   // (the beam aimed by the differences: aim.py)
    static const bool a = [] {
        auto v = py::os::environ_::get("CB_AIM"_s);
        return !v || *v != "0";
    }();
    return a;
}
}  // namespace

// mendings of the line the compiler stopped at, as the error says ([] when none):
//     left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
//     illegal indirection                             *X     -> X an int * (or a char *); else *(char *)X
//     subscript on non-array                          X[I]   -> X an int *; else ((int *)X)[I]
// (the model's expressions carry no types: what the code shows is given them - each way, the bytes decide)
Lines compile_fixes(const str& src, const str& con) {
    Lines out;
    auto errs = errors_of(con);
    if (errs.empty()) return out;
    int64_t n = errs[0].first;
    str msg = errs[0].second;
    Lines lines = split_nl(src);
    if (!(0 < n && n <= lines.size())) return out;
    str l = lines[n - 1];
    OMatch m;
    if (msg.contains("illegal indirection"_s)) m = py::re::search(R"re((?<![\w)\]])\*(?!\()\s*(?:\+\+|--)?([A-Za-z_]\w*))re"_s, l);
    else if (msg.contains("subscript on non-array"_s)) m = py::re::search(R"re(\b([A-Za-z_]\w*)\[)re"_s, l);
    if (m) {
        str g1 = m->group(1);
        for (const char* ty : byteptr(g1) ? std::vector<const char*>{"char", "int"} : std::vector<const char*>{"int", "char"}) {   // (read a byte at a time: char)
            std::optional<str> d = T::_declare_pointer(src, g1, str(std::string(ty)));
            if (d && !d->empty()) out.append(*d);
        }
    }
    if (m && msg.contains("illegal indirection"_s) && !byteptr(m->group(1))) {   // (or a word through a cast in place: the variable stays an int)
        str nw = py::re::sub(pf(R"re((?<![\w)\]])\*(?!\()(\s*%s)\b)re", esc(m->group(1))), R"re(*(int *)\1)re"_s, l, 1);
        if (nw != l) {
            Lines lines2 = lines.copy();
            lines2[n - 1] = nw;
            out.append(join_nl(lines2));
        }
    }
    auto names_of = [&](const str& line) {
        Lines xs;
        for (const str& x : py::re::findall(R"re(\b([A-Za-z_]\w*)\b)re"_s, line))
            if (!xs.contains(x)) xs.append(x);
        return xs;
    };
    if (msg.contains("bad left operand"_s) || msg.contains("bad right operand"_s)) {   // (a pointer in arithmetic only ints take: an int)
        for (const str& x : names_of(l)) {
            str d = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?(?:unsigned )?char \*%s;$)re", x), pf(R"re(\1\2int %s;)re", x), src);
            if (d != src) out.append(d);
        }
    }
    if (msg.contains("2 pointers"_s)) {   // (two pointers added: one of them an int - each way)
        for (const str& x : names_of(l)) {
            str d = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?char \*%s;$)re", x), pf(R"re(\1\2int %s;)re", x), src);
            if (d != src) out.append(d);
        }
    }
    std::optional<str> one = compile_fix(src, con);
    if (one && !one->empty()) out.append(*one);
    return out;
}

// the source with the line the compiler stopped at mended, as the error says, or None:
//     left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
//     illegal indirection                             *X     -> *(char *)X
//     subscript on non-array                          X[I]   -> ((int *)X)[I]
// (the model's expressions carry no types; a cast gives the compiler the one the code shows)
std::optional<str> compile_fix(const str& src, const str& con) {
    auto errs = errors_of(con);
    if (errs.empty()) return std::nullopt;
    int64_t n = errs[0].first;
    str msg = errs[0].second;
    Lines lines = split_nl(src);
    if (!(0 < n && n <= lines.size())) return std::nullopt;
    str l = lines[n - 1];
    str nw;
    OMatch m = py::re::search(R"re(left of .->f(\d+). must have a struct)re"_s, msg);
    if (m) {
        str f = m->group(1);
        nw = py::re::sub(
            pf(R"re(((?:\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)|[A-Za-z_]\w*(?:\[[^\]]*\])?)(?:(?:\.|->)f\d+)*)->f%s\b)re", f),
            [&](const Match& x) { return pf("*(int *)((char *)%s + %s)", x.group(1), f); }, l, 1);
    } else if (msg.contains("illegal indirection"_s)) {
        nw = py::re::sub(R"re((?<![\w)\]])\*(?!\()(\s*[A-Za-z_]\w*))re"_s, R"re(*(char *)\1)re"_s, l, 1);
    } else if (msg.contains("subscript on non-array"_s)) {
        nw = py::re::sub(R"re(\b([A-Za-z_]\w*)\[)re"_s, R"re(((int *)\1)[)re"_s, l, 1);
    } else {
        return std::nullopt;
    }
    if (nw == l) return std::nullopt;
    lines[n - 1] = nw;
    return join_nl(lines);
}

namespace {
int64_t g_spent = 0;
}
int64_t& spent() { return g_spent; }

// how near a verdict is: one that differs (the right length first, then by fewer bytes), before one that
// does not compile, before one with registers left
Rank _rank(const str& v) {
    OMatch m = py::re::match(R"re(differs: (\d+) bytes(?:, length (\d+) not (\d+))?)re"_s, v);
    if (m) {
        int64_t gap = m->group_opt(2) ? std::abs(gi(*m, 2) - gi(*m, 3)) : 0;
        return {0, gap ? 1 : 0, gi(*m, 1) + gap};   // (what is missing counts as differing)
    }
    return v.startswith("does not"_s) ? Rank{1, 0, 0} : Rank{2, 0, 0};
}

// the verdict on the k-th function of the object (a batch's: batch.compile_many)
str _verdict(const Prj& prj, const py::bytes& raw, int64_t a, int64_t ln, int64_t k) {
    formats::omf::Obj o = formats::omf::read(raw);
    py::list<str> segs = formats::omf::code_segments(o);
    py::list<std::tuple<str, int64_t, int64_t>> fns;
    if (!segs.empty()) {
        auto fs = formats::omf::functions(o, segs[0]);
        std::vector<std::tuple<str, int64_t, int64_t>> v;
        for (const auto& f : fs) v.push_back(f);
        std::stable_sort(v.begin(), v.end(), [](const auto& x, const auto& y) { return std::get<1>(x) < std::get<1>(y); });
        fns = py::list<std::tuple<str, int64_t, int64_t>>(v);
    }
    if (fns.size() <= k) return "no function compiled"_s;
    auto [name, off, l2] = fns[k];
    compare::Obj co;   // (the object as compare.function reads it: a group's index, or nothing, becomes text)
    co.seg = o.seg;
    for (const auto& f : o.fix) {
        const auto& tg = std::get<3>(f);
        str second;
        if (std::holds_alternative<str>(std::get<1>(tg))) second = std::get<str>(std::get<1>(tg));
        else if (std::holds_alternative<int64_t>(std::get<1>(tg))) second = py::str(std::to_string(std::get<int64_t>(std::get<1>(tg))));
        co.fix.append(std::make_tuple(std::get<0>(f), std::get<1>(f), std::get<2>(f), compare::Target(std::get<0>(tg), second), std::get<4>(f)));
    }
    for (const auto& p : o.pubs) co.pubs.append(std::make_tuple(std::get<0>(p), std::get<1>(p).value_or(str("")), std::get<2>(p)));
    auto [bad, notes] = compare::function(co, prj.image, name, off, l2, a, prj.cmp, prj.symbols, compare::Symbols(), prj.decode, segs[0]);
    if (compare::identical(bad, notes) && l2 == ln) return "identical"_s;
    return pf("differs: %d bytes%s", bad.size(), l2 != ln ? pf(", length %d not %d", l2, ln) : ""_s);
}

namespace {
// tc.compile of a candidate (the source as latin-1 bytes, replacing what it cannot hold)
std::tuple<std::optional<py::bytes>, str> comp(const Prj& prj, batch::Toolchain& tc, const str& s) {
    return tc.compile(s.encode("latin1", "replace"), prj.headers(), prj.include, prj.switches);
}
}  // namespace

// the verdict on src - mended where the compiler says it cannot be compiled, each way the error offers, and
// again (a few rounds, a few compiles): the best of what compiles. The second member is the source the verdict
// is of
std::tuple<str, str> _prove(const Prj& prj, batch::Toolchain& tc, const str& src, int64_t a, int64_t ln, int64_t tries, int64_t budget) {
    Lines frontier{src};
    std::optional<std::pair<Rank, std::pair<str, str>>> best;   // (rank, (source, verdict))
    std::optional<std::pair<str, str>> first_con;
    for (int64_t depth = 0; depth <= tries; ++depth) {
        Lines nxt;
        for (const str& s : frontier) {
            if (budget <= 0) break;
            budget -= 1;
            g_spent += 1;
            auto [raw, con] = comp(prj, tc, s);
            if (!raw) {
                if (!first_con) first_con = std::make_pair(s, con);
                if (depth < tries) {
                    Lines fx = compile_fixes(s, con);
                    nxt.extend(fx.slice(0, 3));
                }
                continue;
            }
            str v = _verdict(prj, *raw, a, ln);
            if (v == "identical") return {v, s};
            Rank r = _rank(v);
            if (!best || r < best->first) best = std::make_pair(r, std::make_pair(s, v));
        }
        if (nxt.empty() || budget <= 0) break;
        frontier = nxt;
    }
    if (best) return {best->second.second, best->second.first};
    str s = first_con ? first_con->first : src;
    str con = first_con ? first_con->second : ""_s;
    Lines last, errs_;
    for (const str& l : py::splitlines(con.strip()))
        if (!l.contains("(the pass"_s)) last.append(l);
    for (const str& l : last)   // (the error, not a warning before it)
        if (!l.contains("warning"_s)) errs_.append(l);
    if (!errs_.empty()) last = errs_.slice(0, 1);
    return {pf("does not compile: %s", !last.empty() ? last[-1].slice(0, 100) : ""_s), s};
}

namespace {
// {source: verdict} of those of srcs a batch compiled (one run of the compiler, counted as two compiles); the rest are
// left to _prove
std::map<std::string, str> batched(const Prj& prj, batch::Toolchain& tc, const Lines& srcs, int64_t a, int64_t ln) {
    std::map<std::string, str> out;
    if (srcs.size() < 2) return out;
    py::dict<int64_t, std::tuple<py::bytes, int64_t>> got;
    try {
        batch::Project bp;
        bp.headers = prj.headers;
        bp.include = prj.include;
        bp.switches = prj.switches;
        got = batch::compile_many(bp, tc, srcs);
    } catch (const std::exception&) {
        return out;
    }
    g_spent += 2;
    for (const auto& kv : got.items()) {
        try {
            out[srcs[std::get<0>(kv)].utf8()] = _verdict(prj, std::get<0>(std::get<1>(kv)), a, ln, std::get<1>(std::get<1>(kv)));
        } catch (const std::exception&) {
        }
    }
    return out;
}
}  // namespace

// -> (source, verdict): of each of the model's texts [(text, register parameters)] and their variants, the
// first the compiler proves identical; else the best of them (one that differs before one that does not
// compile, before one with registers left)
std::tuple<str, str> prove_texts(const Prj& prj, batch::Toolchain& tc, const Inv& inv, const Texts& texts0, int64_t a, int64_t ln,
                                 const py::dict<int64_t, str>& names) {
    std::optional<std::tuple<Rank, str, str>> best;
    Texts texts = texts0.copy();
    for (const auto& tr : texts0) {   // (a register loaded from a parameter and then given a new value: maybe a local the parameter started - the compiler says)
        const str& text = std::get<0>(tr);
        py::dict<str, int64_t> regparam = std::get<1>(tr) ? *std::get<1>(tr) : py::dict<str, int64_t>();
        std::vector<str> given;
        for (const auto& kv : regparam.items())
            if (py::re::search(pf(R"re((?m)^[ \t]*%s = )re", std::get<0>(kv)), text)) given.push_back(std::get<0>(kv));
        std::sort(given.begin(), given.end());
        for (const str& g_ : given) {
            py::dict<str, int64_t> d;
            for (const auto& kv : regparam.items())
                if (std::get<0>(kv) != g_) d.set(std::get<0>(kv), std::get<1>(kv));
            texts.append(std::make_tuple(text, std::optional<py::dict<str, int64_t>>(d)));
        }
        if (given.size() > 1) {
            py::dict<str, int64_t> d;
            for (const auto& kv : regparam.items())
                if (std::find(given.begin(), given.end(), std::get<0>(kv)) == given.end()) d.set(std::get<0>(kv), std::get<1>(kv));
            texts.append(std::make_tuple(text, std::optional<py::dict<str, int64_t>>(d)));
        }
    }
    for (const auto& tr : texts) {
        auto [src, v] = prove_best(prj, tc, inv, std::get<0>(tr), a, ln, names, std::get<1>(tr));
        if (v == "identical") return {src, v};
        Rank rank = _rank(v);
        if (!best || rank < std::get<0>(*best)) best = std::make_tuple(rank, src, v);
    }
    if (!best) throw py::TypeError("'NoneType' object is not subscriptable");
    return {std::get<1>(*best), std::get<2>(*best)};
}

// True: the prologue calls the stack probe (`mov ax,N` or `xor ax,ax`, then a call, after `mov bp,sp`);
// False: a frame without one (`sub sp,N`, or none and no call there); None: not a frame the rule knows
std::optional<bool> _probes(const py::bytes& code) {
    py::list<x86::Insn> ins = x86::disasm(code, 0, std::min<int64_t>(code.size(), 16), false);
    if (ins.size() < 2 || ins[0].mnemonic != "push" || ins[0].op_str != "bp" || ins[1].op_str != "bp, sp") return std::nullopt;
    if (ins.size() > 3 && (ins[2].op_str.startswith("ax, "_s) && (ins[2].mnemonic == "mov" || ins[2].mnemonic == "xor")) && ins[3].mnemonic == "call") return true;
    return false;
}

// a verdict too far from the program's code for the beam's rewrites to close (a statement missing, a loop
// misread): the model's reading is wrong, not its spelling
bool _hopeless(const str& v, int64_t ln) {
    OMatch m = py::re::match(R"re(differs: (\d+) bytes(?:, length (\d+) not (\d+))?)re"_s, v);
    if (!m || ln == 0) return false;
    int64_t gap = m->group_opt(2) ? std::abs(gi(*m, 2) - gi(*m, 3)) : 0;
    // (by the length alone: a byte too many early shifts every byte after it, so the bytes that differ say
    //  little - a function 36 bytes off by one instruction was closed by the beam)
    return static_cast<double>(gap) > std::max(16.0, std::get<0>(HOPELESS) * static_cast<double>(ln));
}

// a candidate as the compiler sees it, for telling two apart: a doubled pair of parentheses one, the
// spacing one (neither changes a byte of the code) - not what is compiled, only what is compared
str _canon(const str& src) {
    str t = py::re::sub(R"re([ \t]+)re"_s, " "_s, src);
    for (int i = 0; i < 6; ++i) {
        str t2 = py::re::sub(R"re(\(\(([^()]*(?:\([^()]*\)[^()]*)*)\)\))re"_s, R"re((\1))re"_s, t);
        if (t2 == t) break;
        t = t2;
    }
    return t;
}

namespace {
using Sig = std::pair<std::vector<std::string>, bool>;

// a candidate's declarations - its parameters' and locals' types, names aside - and whether it returns a
// value: the kind of reading it is
Sig _decl_sig(const str& src) {
    auto [head, sep, body] = T::split_body(src);
    str hb = py::split(head, "\n"_s, 1)[-1] + py::split(body, "\n\n"_s, 1)[0];
    std::vector<std::string> norm;
    for (const str& d : py::re::findall(R"re((?m)^[ \t]*((?:register )?(?:unsigned )?(?:char|int|long|short|struct \w+|double|float)\b[^;(]*);)re"_s, hb))
        norm.push_back(py::re::sub(R"re(\b(?:[a-z]|v\d+\w*|r_\w+|_p\w*)\b(?=[\[;, ]|$))re"_s, "X"_s, d).utf8());
    std::sort(norm.begin(), norm.end());
    return {norm, py::re::search(R"re(\breturn [^;])re"_s, body).has_value()};
}

// the string of the rewrites the beam tries: (name, function)
Lines opt1(const std::optional<str>& o) {
    Lines l;
    if (o) l.append(*o);
    return l;
}

str place_decl(const str& name, const str& t) {
    return !py::re::search(pf(R"re((?m)^extern .*\b%s\b)re", name), t) ? pf("extern char %s[];\n", name) + t : t;
}
}  // namespace

const std::vector<Refiner>& REFINERS() {
    static const std::vector<Refiner> r = [] {
        std::vector<Refiner> v;
        auto add = [&](const char* name, std::function<Lines(const Prj&, const str&)> fn) { v.push_back(Refiner{str(std::string(name)), std::move(fn)}); };
        add("assign_in_args", [](const Prj&, const str& s) { return opt1(F::assign_in_args(s)); });
        add("return_sources", [](const Prj&, const str& s) { return opt1(F::return_sources(s)); });
        add("compound_assigns", [](const Prj&, const str& s) { return opt1(F::compound_assigns(s)); });
        add("chain_assigns", [](const Prj&, const str& s) { return opt1(F::chain_assigns(s)); });
        add("for_loops", [](const Prj&, const str& s) { return opt1(S::for_loops(s)); });
        add("store_increments", [](const Prj&, const str& s) { return opt1(F::store_increments(s)); });
        add("pre_steps", [](const Prj&, const str& s) { return opt1(F::pre_steps(s)); });
        add("pre_increments", [](const Prj&, const str& s) { return opt1(F::pre_increments(s)); });
        add("assign_in_tests", [](const Prj&, const str& s) { return opt1(F::assign_in_tests(s)); });
        add("and_returns", [](const Prj&, const str& s) { return opt1(F::and_returns(s)); });
        add("register_temps", [](const Prj&, const str& s) { return opt1(F::register_temps(s)); });
        add("void_variant", [](const Prj&, const str& s) { return opt1(C::void_variant(s)); });
        add("pointer_widths", [](const Prj&, const str& s) { return F::pointer_widths(s); });
        add("address_variants", [](const Prj&, const str& s) {   // _place_variants: the numbers a pointer is given or compared with in a localized source as the places in the data they are
            Lines out;
            for (const auto& nt : F::address_variants(s, 0x10000, 3)) out.append(place_decl(std::get<0>(nt), std::get<1>(nt)));
            return out;
        });
        add("string_variants", [](const Prj& prj, const str& s) { return C::string_variants(prj, s); });
        add("split_ternaries", [](const Prj&, const str& s) { return opt1(F::split_ternaries(s)); });
        add("default_last", [](const Prj&, const str& s) { return opt1(F::default_last(s)); });
        add("push_tails", [](const Prj&, const str& s) { return opt1(F::push_tails(s)); });
        add("unsigned_registers", [](const Prj&, const str& s) { return opt1(F::unsigned_registers(s)); });
        add("unmasked", [](const Prj&, const str& s) { return opt1(F::unmasked(s)); });
        add("no_default", [](const Prj&, const str& s) { return opt1(F::no_default(s)); });
        add("empty_cases_first", [](const Prj&, const str& s) { return opt1(F::empty_cases_first(s)); });
        add("register_slots", [](const Prj&, const str& s) { return opt1(C::register_slots(s)); });
        add("register_copies", [](const Prj&, const str& s) { return opt1(F::register_copies(s)); });
        add("lead_filler", [](const Prj&, const str& s) { return opt1(F::lead_filler(s)); });
        add("scaled_places", [](const Prj&, const str& s) { return opt1(F::scaled_places(s)); });
        add("shared_tails", [](const Prj&, const str& s) { return opt1(F::shared_tails(s)); });
        add("guard_returns", [](const Prj&, const str& s) { return opt1(F::guard_returns(s)); });
        add("default_after", [](const Prj&, const str& s) { return opt1(F::default_after(s)); });
        add("frame_more", [](const Prj&, const str& s) { return opt1(F::frame_more(s)); });
        add("loop_tests", [](const Prj&, const str& s) { return opt1(S::loop_tests(s)); });
        add("switch_breaks", [](const Prj&, const str& s) { return opt1(F::switch_breaks(s)); });
        add("register_reads", [](const Prj&, const str& s) { return opt1(F::register_reads(s)); });
        add("compare_results", [](const Prj&, const str& s) { return opt1(F::compare_results(s)); });
        add("or_loops", [](const Prj&, const str& s) { return opt1(F::or_loops(s)); });
        add("loop_guards", [](const Prj&, const str& s) { return opt1(S::loop_guards(s)); });
        add("const_tests", [](const Prj&, const str& s) { return opt1(S::const_tests(s)); });
        add("flat_returns", [](const Prj&, const str& s) { return opt1(S::flat_returns(s)); });
        add("return_cases_first", [](const Prj&, const str& s) { return opt1(S::return_cases_first(s)); });
        add("registers_last", [](const Prj&, const str& s) { return opt1(C::registers_last(s)); });
        add("unsigned_locals", [](const Prj&, const str& s) { return C::unsigned_locals(s); });
        add("guard_shared", [](const Prj&, const str& s) { return opt1(F::guard_shared(s)); });
        add("guard_flat", [](const Prj&, const str& s) { return opt1(F::guard_flat(s)); });
        add("common_tails", [](const Prj&, const str& s) { return opt1(T::_changed([](const str& x) { return S::common_tails(x); }, s)); });
        add("byte_heads", [](const Prj&, const str& s) { return opt1(T::_changed([](const str& x) { return S::byte_heads(x); }, s)); });
        add("word_offsets", [](const Prj&, const str& s) { return opt1(T::_changed([](const str& x) { return S::word_offsets(x); }, s)); });
        add("chain_tails", [](const Prj&, const str& s) { return opt1(S::chain_tails(s)); });
        add("heads_variant", [](const Prj&, const str& s) { return opt1(S::heads_variant(s)); });
        add("case_guards", [](const Prj&, const str& s) { return S::case_guards(s); });
        add("shared_exit", [](const Prj&, const str& s) { return opt1(S::shared_exit(s)); });
        add("label_sinks", [](const Prj&, const str& s) { return opt1(S::label_sinks(s)); });
        add("call_pairs", [](const Prj&, const str& s) { return opt1(S::call_pairs(s)); });
        add("assign_loops", [](const Prj&, const str& s) { return opt1(S::assign_loops(s)); });
        add("char_widens", [](const Prj&, const str& s) { return opt1(S::char_widens(s)); });
        add("tail_continues", [](const Prj&, const str& s) { return opt1(T::_changed([](const str& x) { return S::tail_continues(x); }, s)); });
        add("adjacent_swaps", [](const Prj&, const str& s) { return S::adjacent_swaps(s); });
        add("unsigned_fields", [](const Prj&, const str& s) { return S::unsigned_fields(s); });
        add("frame_names", [](const Prj& prj, const str& s) {
            Lines out;
            if (prj.frame_order) {
                auto x = F::frame_names(s, *prj.frame_order);
                if (x) out.append(*x);
                auto y = F::frame_names(s, *prj.frame_order, true);
                if (y) out.append(*y);
            }
            return out;
        });
        // (a value and a zero word pushed: it widened)
        add("other", [](const Prj&, const str& s) { return Lines{py::re::sub(R"re(\b([A-Za-z_]\w*), 0\))re"_s, R"re((long)\1))re"_s, s, 1)}; });
        // (two zero words pushed: a long 0)
        add("other", [](const Prj&, const str& s) { return Lines{py::re::sub(R"re(\b0, 0\))re"_s, "0L)"_s, s)}; });
        // (the same first of more arguments)
        add("other", [](const Prj&, const str& s) { return Lines{py::re::sub(R"re((?<=\()0, 0(?=, ))re"_s, "0L"_s, s, 1)}; });
        return v;
    }();
    return r;
}

namespace {
// (the refiners each kind of difference asks for first - aim.py's kinds; the rest after them)
const std::map<std::string, std::set<std::string>>& aim_kind() {
    static const std::map<std::string, std::set<std::string>> m = {
        {"frame", {"frame_names", "registers_last", "frame_more", "lead_filler", "register_slots"}},
        {"width", {"pointer_widths", "char_widens", "unsigned_registers", "unmasked", "byte_heads", "unsigned_fields", "high_words", "word_offsets", "scaled_places"}},
        {"sign", {"unsigned_locals", "unsigned_fields", "unsigned_registers"}},
        {"order", {"adjacent_swaps", "return_cases_first", "empty_cases_first", "default_after", "default_last", "switch_breaks", "flat_returns", "guard_returns", "guard_shared",
                   "guard_flat", "shared_tails", "push_tails", "loop_tests", "or_loops", "loop_guards", "for_loops", "label_sinks", "shared_exit", "case_guards", "chain_tails",
                   "common_tails", "heads_variant", "no_default", "assign_loops", "tail_continues"}},
        {"shape", {"assign_in_args", "return_sources", "compound_assigns", "chain_assigns", "store_increments", "pre_steps", "pre_increments", "assign_in_tests", "and_returns",
                   "register_temps", "split_ternaries", "compare_results", "const_tests", "register_reads", "register_copies", "word_offsets", "call_pairs", "string_variants",
                   "address_variants"}}};
    return m;
}
bool in_line_kinds(const std::string& n) { return aim_kind().at("shape").count(n) > 0; }   // (rewrites of a statement: kept only where the code differs)

using Priors = py::dict<str, py::dict<str, int64_t>>;
Priors& PRIORS() {   // ({kind: {refiner: wins}} - the beam's history, learned)
    static Priors p;
    return p;
}

str priors_path() {
    str path = py::os::path::join(py::settings::get_str(str("state")), "exact"_s);
    path = py::os::path::join(path, "priors.json"_s);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// json.load of the priors file: {"kind": {"refiner": n, ..}, ..} (anything else is the Python's later failure; here, the load's)
Priors parse_priors(const std::string& s) {
    size_t i = 0;
    auto ws = [&] { while (i < s.size() && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t')) ++i; };
    auto expect = [&](char c) {
        ws();
        if (i >= s.size() || s[i] != c) throw py::ValueError("bad priors");
        ++i;
    };
    auto string_ = [&]() {
        ws();
        if (i >= s.size() || s[i] != '"') throw py::ValueError("bad priors");
        ++i;
        std::string o;
        while (i < s.size() && s[i] != '"') {
            if (s[i] == '\\') {
                ++i;
                if (i >= s.size()) throw py::ValueError("bad priors");
                char c = s[i];
                if (c == 'u') {
                    if (i + 4 >= s.size()) throw py::ValueError("bad priors");
                    unsigned cp = static_cast<unsigned>(std::stoul(s.substr(i + 1, 4), nullptr, 16));
                    i += 4;
                    if (cp < 0x80) o += static_cast<char>(cp);
                    else if (cp < 0x800) { o += static_cast<char>(0xC0 | (cp >> 6)); o += static_cast<char>(0x80 | (cp & 0x3F)); }
                    else { o += static_cast<char>(0xE0 | (cp >> 12)); o += static_cast<char>(0x80 | ((cp >> 6) & 0x3F)); o += static_cast<char>(0x80 | (cp & 0x3F)); }
                } else o += c == 'n' ? '\n' : c == 't' ? '\t' : c == 'r' ? '\r' : c;
            } else o += s[i];
            ++i;
        }
        if (i >= s.size()) throw py::ValueError("bad priors");
        ++i;   // (the closing quote)
        return o;
    };
    Priors p;
    expect('{');
    ws();
    if (i < s.size() && s[i] == '}') return p;
    while (true) {
        std::string k = string_();
        expect(':');
        expect('{');
        py::dict<str, int64_t> d;
        ws();
        if (i < s.size() && s[i] != '}') {
            while (true) {
                std::string n = string_();
                expect(':');
                ws();
                size_t j = i;
                if (j < s.size() && s[j] == '-') ++j;
                while (j < s.size() && s[j] >= '0' && s[j] <= '9') ++j;
                if (j == i) throw py::ValueError("bad priors");
                d.set(str(n), std::stoll(s.substr(i, j - i)));
                i = j;
                ws();
                if (i < s.size() && s[i] == ',') { ++i; continue; }
                break;
            }
        }
        expect('}');
        p.set(str(k), d);
        ws();
        if (i < s.size() && s[i] == ',') { ++i; continue; }
        break;
    }
    expect('}');
    return p;
}

Priors& _priors() {
    Priors& p = PRIORS();
    if (p.empty()) {
        try {
            py::file f = py::open(priors_path(), "r"_s);
            Priors d = parse_priors(f.read_text().utf8());
            p.update(d);
        } catch (const std::exception&) {
            p.set("_"_s, py::dict<str, int64_t>());
        }
    }
    return p;
}

// json.dump(p, sort_keys=True)
std::string dump_priors(const Priors& p) {
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, int64_t>>>> items;
    for (const auto& kv : p.items()) {
        std::vector<std::pair<std::string, int64_t>> in;
        for (const auto& nv : std::get<1>(kv).items()) in.push_back({std::get<0>(nv).utf8(), std::get<1>(nv)});
        std::sort(in.begin(), in.end());
        items.push_back({std::get<0>(kv).utf8(), in});
    }
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    auto js = [](const std::string& s) { std::string o; py::json::put(o, str(s)); return o; };
    std::string out = "{";
    for (size_t i = 0; i < items.size(); ++i) {
        out += (i ? ", " : "") + js(items[i].first) + ": {";
        for (size_t j = 0; j < items[i].second.size(); ++j) out += (j ? ", " : "") + js(items[i].second[j].first) + ": " + std::to_string(items[i].second[j].second);
        out += "}";
    }
    return out + "}";
}

// a refiner that brought a reading nearer, for a difference of these kinds: counted (state/exact/priors.json)
void _learn(const Lines& kinds, const str& name) {
    Priors& p = _priors();
    Lines ks = kinds.empty() ? Lines{"shape"_s} : kinds;
    for (const str& k : ks) {
        if (!p.contains(k)) p.set(k, py::dict<str, int64_t>());
        py::dict<str, int64_t> d = p.at(k);
        d.set(name, d.get(name, 0) + 1);
    }
    str f = priors_path();
    try {   // (merged with what other processes wrote meanwhile)
        py::file fh = py::open(f, "r"_s);
        Priors disk = parse_priors(fh.read_text().utf8());
        for (const auto& kd : disk.items()) {
            for (const auto& nc : std::get<1>(kd).items()) {
                const str& k = std::get<0>(kd);
                if (!p.contains(k)) p.set(k, py::dict<str, int64_t>());
                py::dict<str, int64_t> d = p.at(k);
                d.set(std::get<0>(nc), std::max(d.get(std::get<0>(nc), 0), std::get<1>(nc)));
            }
        }
    } catch (const std::exception&) {
    }
    str tmp = pf("%s.%d.tmp", f, static_cast<int64_t>(_getpid()));
    try {
        {
            py::file fh = py::open(tmp, "w"_s);
            fh.write(str(dump_priors(p)));
        }
        py::os::replace(tmp, f);
    } catch (const py::OSError&) {
    }
}

std::map<std::string, std::optional<aim::Diagnosis>>& AIMS() {
    static std::map<std::string, std::optional<aim::Diagnosis>> m;
    return m;
}

// aim.diagnose of a candidate (one compile more, with the listing; kept per text)
const std::optional<aim::Diagnosis>& _aim(const Prj& prj, const str& src, int64_t a, int64_t ln) {
    auto it = AIMS().find(src.utf8());
    if (it != AIMS().end()) return it->second;
    std::optional<aim::Diagnosis> d;
    try {
        auto [obj, con, lst] = prj.compile_listing(src.encode("latin1", "replace"));
        py::list<aim::ListingEntry> code = aim::listing_code(lst);
        if (obj && !code.empty()) d = aim::diagnose(image_slice(prj, a, a + ln), aim::code_of(code), code);
    } catch (const std::exception&) {
        d = std::nullopt;
    }
    return AIMS().emplace(src.utf8(), d).first->second;
}

// does alt change s at one of these source lines (or within one of them)
bool _touches(const str& s, const str& alt, const std::vector<int64_t>& lines) {
    if (lines.empty()) return true;
    Lines sl = split_nl(s), al = split_nl(alt);
    for (const auto& op : aim::_get_opcodes(sl, al)) {
        if (std::get<0>(op) == "equal") continue;
        int64_t i1 = std::get<1>(op), i2 = std::get<2>(op);
        for (int64_t l : lines)
            if (i1 - 1 <= l - 1 && l - 1 <= std::max(i2, i1 + 1)) return true;
    }
    return false;
}

struct Ordered {
    const Refiner* r;
    std::optional<std::vector<int64_t>> lines;   // the lines-filter (None: none)
};

// the refiners in the order a reading's difference asks for: [(refiner, lines-filter or None)] - the kinds
// diagnosed first (by their wins so far), the rest after
std::pair<std::vector<Ordered>, Lines> _ordered(const Prj& prj, const str& s_, int64_t a, int64_t ln) {
    const std::optional<aim::Diagnosis>& d = _aim(prj, s_, a, ln);
    std::vector<std::pair<str, int64_t>> ks;
    if (d)
        for (const auto& kv : d->kinds.items()) ks.push_back({std::get<0>(kv), std::get<1>(kv)});
    std::stable_sort(ks.begin(), ks.end(), [](const auto& x, const auto& y) { return -x.second < -y.second; });
    Lines kinds;
    for (const auto& k : ks) kinds.append(k.first);
    Priors& pri = _priors();
    std::vector<const Refiner*> first, rest;
    for (const Refiner& r : REFINERS()) {
        bool hit = false;
        for (const str& k : kinds) {
            auto it = aim_kind().find(k.utf8());
            if (it != aim_kind().end() && it->second.count(r.name.utf8())) hit = true;
        }
        (hit ? first : rest).push_back(&r);
    }
    auto score = [&](const Refiner* r) {
        int64_t sum = 0;
        Lines kk = kinds.empty() ? Lines{"shape"_s} : kinds;
        for (const str& k : kk)
            if (pri.contains(k)) sum += pri.at(k).get(r->name, 0);
        return -sum;
    };
    std::stable_sort(first.begin(), first.end(), [&](const Refiner* x, const Refiner* y) { return score(x) < score(y); });
    std::stable_sort(rest.begin(), rest.end(), [&](const Refiner* x, const Refiner* y) { return score(x) < score(y); });
    std::optional<std::vector<int64_t>> lines;
    if (!kinds.empty()) {
        bool only_shape = true;
        for (const str& k : kinds)
            if (k != "shape") only_shape = false;
        if (only_shape) {
            std::vector<int64_t> ls;
            if (d)
                for (int64_t i = 0; i < std::min<int64_t>(6, d->lines.size()); ++i) ls.push_back(d->lines[i]);
            lines = ls;
        }
    }
    std::vector<Ordered> out;
    for (const Refiner* r : first) out.push_back({r, in_line_kinds(r->name.utf8()) ? lines : std::nullopt});
    for (const Refiner* r : rest) out.push_back({r, std::nullopt});
    return {out, kinds};
}

}  // namespace

// -> (source, verdict): of the model's text and its variants, the first the compiler proves identical; else
// the first that compiles (or the first)
std::tuple<str, str> prove_best(const Prj& prj, batch::Toolchain& tc, const Inv& inv, const str& text, int64_t a, int64_t ln,
                                const py::dict<int64_t, str>& names, const std::optional<py::dict<str, int64_t>>& regparam_in) {
    Lines regs = prj.carve_register_variables;
    py::dict<str, int64_t> regparam = regparam_in ? *regparam_in : (inv.last_regparam ? *inv.last_regparam : py::dict<str, int64_t>());
    C::ByteFields widths = C::byte_fields(inv, image_slice(prj, a, a + ln));
    {
        std::vector<std::tuple<str, int64_t>> old(T::BYTEPTR.begin(), T::BYTEPTR.end());
        for (const auto& x : old) T::BYTEPTR.discard(x);
        for (const auto& x : widths.fields) T::BYTEPTR.add(x);
    }
    auto cands0 = C::candidates(prj, text, a, names, regs, regparam, widths);
    std::set<std::string> seen;
    std::vector<std::pair<str, Lines>> cands;
    for (const auto& sl : cands0) {   // (each text once - spacing and doubled parentheses aside; the first readings first, and at most MAXCANDS of them: a bench must end)
        std::string c_ = _canon(std::get<0>(sl)).utf8();
        if (!seen.count(c_)) {
            seen.insert(c_);
            cands.push_back({std::get<0>(sl), std::get<1>(sl)});
        }
    }
    if (static_cast<int64_t>(cands.size()) > MAXCANDS) cands.resize(static_cast<size_t>(MAXCANDS));
    const std::optional<str>& fpsw = prj.fp_switch;
    const std::optional<str>& fsw = prj.file_switches;
    const std::optional<str>& gs = prj.probe_switch;
    str sw = prj.switches ? *prj.switches : prj.tc_switches.value_or(""_s);
    Lines want = py::split(sw);
    if (C::FPINFO.fp && fpsw && !fpsw->empty() && !want.contains(*fpsw)) want.append(*fpsw);   // (code of the 8087's: compiled for it - the profile's "fp_switch")
    if (gs && !gs->empty()) {   // (a stack probe in the prologue, or none: the switch that leaves it out - "probe_switch" - as it shows)
        std::optional<bool> probe = _probes(image_slice(prj, a, a + ln));
        if (probe && *probe && want.contains(*gs)) want.remove(*gs);
        else if (probe && !*probe && !want.contains(*gs)) want.append(*gs);
    }
    if (fsw && !fsw->empty() && want != py::split(sw)) {
        for (auto& c : cands) c.first = pf("/* %s: %s */\n", *fsw, py::join(" "_s, want)) + c.first;
    }
    std::optional<Reading> best;
    std::vector<Reading> tops;   // (every reading that compiled, near or far: the beam's seeds)
    g_spent = 0;
    std::map<std::string, str> known;   // (verdicts a batch gave: batch.compile_many)
    for (size_t ci = 0; ci < cands.size(); ++ci) {
        str src = cands[ci].first;
        const Lines& left = cands[ci].second;
        str v;
        if (!left.empty()) {
            v = pf("registers left: %s", py::join(" "_s, left));
        } else if (g_spent >= FUNC_BUDGET && best) {
            break;   // (compiled enough for one function: the best so far)
        } else {
            if (batch_size() && !known.count(src.utf8())) {   // (the next ones compiled together, one run of the compiler)
                Lines ahead;
                for (size_t j = ci; j < cands.size() && ahead.size() < batch_size(); ++j)
                    if (cands[j].second.empty() && !known.count(cands[j].first.utf8())) ahead.append(cands[j].first);
                for (const auto& kv : batched(prj, tc, ahead, a, ln)) known[kv.first] = kv.second;
            }
            auto it = known.find(src.utf8());
            if (it != known.end()) {
                v = it->second;
            } else {
                auto [v1, s1] = _prove(prj, tc, src, a, ln);
                v = v1;
                src = s1;   // (as the compiler's errors mended it)
            }
            if (v != "identical") {
                for (const str& alt : C::string_variants(prj, src)) {   // (a number that is a string's place: the string)
                    auto [v2, s2] = _prove(prj, tc, alt, a, ln);
                    if (v2 == "identical") {
                        src = s2;
                        v = "identical"_s;
                        break;
                    }
                }
            }
        }
        if (v == "identical") return {src, v};
        Rank rank = _rank(v);
        if (v.startswith("differs"_s)) tops.push_back(Reading{rank, src, v});
        if (!best || rank < best->rank) best = Reading{rank, src, v};
    }
    if (best && best->verdict.startswith("differs"_s) && _hopeless(best->verdict, ln)) return {best->src, best->verdict};   // (too far for rewrites to close: the reading's own fault)
    if (best && best->verdict.startswith("differs"_s)) {   // (the nearest that compile, rewritten - a beam of them: combinations the variants alone miss)
        std::vector<Reading> ranked(tops);
        std::stable_sort(ranked.begin(), ranked.end(), [](const Reading& x, const Reading& y) { return x.rank < y.rank; });
        py::list<Reading> seeds;
        for (size_t i = 0; i < ranked.size() && static_cast<int64_t>(i) < BEAM; ++i) seeds.append(ranked[i]);
        std::vector<std::pair<Sig, Reading>> kinds;   // (and the nearest of each other way of declaring the locals - a buffer, a pointer's type - the beam's start from each reading, not only the nearest)
        for (const Reading& r_ : ranked) {
            Sig sig = _decl_sig(r_.src);
            bool have = false;
            for (const auto& k : kinds)
                if (k.first == sig) have = true;
            if (!have) kinds.push_back({sig, r_});
        }
        std::vector<Reading> extra;
        for (const auto& k : kinds) {
            bool in = false;
            for (const Reading& s : seeds)
                if (s == k.second) in = true;
            if (!in) extra.push_back(k.second);
        }
        for (size_t i = 0; i < extra.size() && static_cast<int64_t>(i) < BEAM; ++i) seeds.append(extra[i]);
        return refine(prj, tc, a, ln, *best, 6, std::nullopt, seeds);
    }
    if (!best) throw py::TypeError("'NoneType' object is not subscriptable");
    return {best->src, best->verdict};
}

// -> (source, verdict): a beam search over the REFINERS' rewritings - the BEAM nearest readings (the best and
// the other seeds) each rewritten every way, the nearest BEAM of all kept, again, until one is identical, no round
// brings any nearer, or the budget of compiles is spent. Two rewritings neither of which helps alone are found
// together: a reading as near as the worst kept goes on (sideways moves) when the beam has room
std::tuple<str, str> refine(const Prj& prj, batch::Toolchain& tc, int64_t a, int64_t ln, const Reading& best, int64_t rounds,
                            std::optional<int64_t> budget_in, const std::optional<py::list<Reading>>& seeds) {
    int64_t budget = budget_in ? *budget_in : REFINE_BUDGET;
    std::vector<Reading> beam;
    std::set<std::string> seen;
    {
        std::vector<Reading> init{best};
        if (seeds)
            for (const Reading& s : *seeds) init.push_back(s);
        for (const Reading& r : init) {
            std::string c = _canon(r.src).utf8();
            if (!seen.count(c)) {
                seen.insert(c);
                beam.push_back(r);
            }
        }
    }
    auto keep = [&](std::vector<Reading> xs) {   // (the nearest BEAM, and the nearest of each other kind of reading - its declarations - up to BEAM more)
        std::stable_sort(xs.begin(), xs.end(), [](const Reading& x, const Reading& y) { return x.rank < y.rank; });
        std::vector<Reading> out(xs.begin(), xs.begin() + std::min<size_t>(xs.size(), static_cast<size_t>(BEAM)));
        std::vector<Sig> kinds;
        for (const Reading& x : out) {
            Sig s = _decl_sig(x.src);
            if (std::find(kinds.begin(), kinds.end(), s) == kinds.end()) kinds.push_back(s);
        }
        for (size_t i = static_cast<size_t>(BEAM); i < xs.size(); ++i) {
            Sig k = _decl_sig(xs[i].src);
            if (std::find(kinds.begin(), kinds.end(), k) == kinds.end() && static_cast<int64_t>(out.size()) < 2 * BEAM) {
                kinds.push_back(k);
                out.push_back(xs[i]);
            }
        }
        return out;
    };
    beam = keep(beam);
    int64_t spent_ = 0;
    Reading top = beam[0];
    for (int64_t round = 0; round < rounds; ++round) {
        std::vector<Reading> pool;
        for (const Reading& cur : beam) {
            const Rank r_ = cur.rank;
            const str& s_ = cur.src;
            std::vector<Ordered> order;
            Lines kinds;
            if (aim_on()) {
                auto oo = _ordered(prj, s_, a, ln);
                order = oo.first;
                kinds = oo.second;
            } else {
                for (const Refiner& r : REFINERS()) order.push_back({&r, std::nullopt});
            }
            std::vector<std::pair<str, str>> work, deferred;   // (the rewrites of this reading, in the aim's order: those at the lines that differ, then the rest)
            for (const Ordered& o : order) {
                Lines alts;
                try {
                    for (const str& x : o.r->fn(prj, s_))
                        if (!x.empty() && x != s_) alts.append(x);
                } catch (const std::exception&) {
                    continue;
                }
                for (const str& alt : alts) {
                    std::string c_ = _canon(alt).utf8();
                    if (seen.count(c_)) continue;
                    seen.insert(c_);
                    (o.lines && !_touches(s_, alt, *o.lines) ? deferred : work).push_back({o.r->name, alt});
                }
            }
            work.insert(work.end(), deferred.begin(), deferred.end());
            const int64_t step = std::max<int64_t>(1, batch_size());
            for (int64_t w0 = 0; w0 < static_cast<int64_t>(work.size()); w0 += step) {
                if (spent_ >= budget) break;
                std::vector<std::pair<str, str>> chunk(work.begin() + w0, work.begin() + std::min<int64_t>(w0 + step, static_cast<int64_t>(work.size())));
                std::map<std::string, str> known;
                if (batch_size()) {
                    Lines xs;
                    for (const auto& c : chunk) xs.append(c.second);
                    known = batched(prj, tc, xs, a, ln);
                }
                spent_ += known.empty() ? 0 : 2;
                for (const auto& nm : chunk) {
                    const str& name = nm.first;
                    const str& alt = nm.second;
                    str v2, got;
                    auto it = known.find(alt.utf8());
                    if (it != known.end()) {
                        v2 = it->second;
                        got = alt;
                    } else {
                        if (spent_ >= budget) break;
                        spent_ += 1;
                        auto pr = _prove(prj, tc, alt, a, ln, 2, 3);
                        v2 = std::get<0>(pr);
                        got = std::get<1>(pr);
                    }
                    if (v2 == "identical") {
                        if (aim_on()) _learn(kinds, name);
                        return {got, v2};
                    }
                    if (v2.startswith("differs"_s)) {
                        if (aim_on() && _rank(v2) < r_) _learn(kinds, name);
                        pool.push_back(Reading{_rank(v2), got, v2});
                    }
                }
            }
        }
        if (pool.empty()) break;
        std::vector<Reading> both(beam);
        both.insert(both.end(), pool.begin(), pool.end());
        std::vector<Reading> merged = keep(both);
        bool all_in = true;
        for (const Reading& x : merged)
            if (std::find(beam.begin(), beam.end(), x) == beam.end()) all_in = false;
        if (merged[0].rank >= top.rank && all_in) break;   // (nothing nearer, nothing new: the search is done)
        beam = merged;
        if (beam[0].rank < top.rank) top = beam[0];
        if (spent_ >= budget) break;
    }
    return {top.src, top.verdict};
}

}  // namespace codebreaker::exact::draft::prove
