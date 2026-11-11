// codebreaker/port/pre_ansi_p4.cpp -- typed C++ port of codebreaker.port.pre_ansi, PART 4 (Python lines 3077-4261):
// the Port methods bss_views .. _same_def, and the free functions split_view_lists .. said.

#include <algorithm>
#include <map>
#include <set>
#include <functional>

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::pre_ansi {

using asm16::Layout;
using asm16::Layouts;
using base::ctext::blank;
using base::ctext::sub_code;
using py::list;
using py::str;

namespace {

str S(const char* s) { return str(s); }
str E(const str& s) { return py::re::escape(s); }
list<str> lines_of(const str& t) { return py::split(t, S("\n")); }
str join_nl(const list<str>& l) { return py::join(S("\n"), l); }

// "".join-free helpers of the Python's str methods that py::str has with other meaning
// s.replace(a, b, 1)
str rep1(const str& s, const str& a, const str& b) {
    int64_t i = s.find(a);
    if (i < 0) return s;
    return s.slice(0, i) + b + s.slice(i + a.size(), s.size());
}
// s[len(s) - len(s.lstrip()):] gives the indentation
str lead_of(const str& s) { return s.slice(0, s.size() - s.lstrip().size()); }
// the pattern of "struct foo" as the Python builds it: re.escape(t).replace(r"\ ", r"\s+")
str tagrx(const str& t) {
    list<str> parts = py::split(t, S(" "));
    list<str> esc;
    for (const str& p : parts) esc.append(E(p));
    return py::join(S("\\s+"), esc);
}
// the loop that finds the '}' of the brace at k (d from 0, +1 for '{', -1 for '}'; stops when d == 0)
int64_t match_brace(const str& code, int64_t k) {
    int64_t d = 0;
    while (k < code.size()) {
        char32_t c = code.at_(k);
        if (c == '{') d += 1;
        else if (c == '}') d -= 1;
        if (d == 0) break;
        k += 1;
    }
    return k;
}
bool in_chars(char32_t c, const char* set) {
    for (const char* p = set; *p; ++p) if (static_cast<char32_t>(*p) == c) return true;
    return false;
}
list<str> sorted_strs(const py::set<str>& s) {
    std::vector<str> v;
    for (const str& x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    return list<str>(v);
}
int64_t to_int(const str& s) { return py::int_parse(s, 10); }
// int(s, 0)
int64_t to_int0(const str& s) { return py::int_parse(s, 0); }
int64_t floordiv(int64_t a, int64_t b) { return static_cast<int64_t>(py::floordiv(static_cast<double>(a), static_cast<double>(b))); }
// bool(re.match(...)) helpers
bool has_match(const char* pat, const str& s) { return py::re::match(S(pat), s).has_value(); }
bool has_search(const char* pat, const str& s) { return py::re::search(S(pat), s).has_value(); }
bool has_full(const char* pat, const str& s) { return py::re::fullmatch(S(pat), s).has_value(); }

}  // namespace

// ---------------------------------------------------------------- constants

const py::dict<str, int64_t> SIZES = {
    {S("char"), 1}, {S("signed char"), 1}, {S("unsigned char"), 1}, {S("int16_t"), 2}, {S("uint16_t"), 2},
    {S("int32_t"), 4}, {S("uint32_t"), 4}, {S("int"), 2}, {S("unsigned"), 2}, {S("short"), 2}, {S("long"), 4},
    {S("float"), 4}, {S("double"), 8}, {S("unsigned int"), 2}, {S("unsigned short"), 2}, {S("short int"), 2},
    {S("unsigned long"), 4}, {S("long int"), 4}, {S("unsigned long int"), 4}, {S("signed int"), 2},
    {S("signed long"), 4}, {S("int8_t"), 1}, {S("uint8_t"), 1}};

const py::dict<str, int64_t> OLD_SCALARS = {
    {S("char"), 1}, {S("signed char"), 1}, {S("unsigned char"), 1}, {S("short"), 2}, {S("short int"), 2},
    {S("unsigned short"), 2}, {S("int"), 2}, {S("unsigned"), 2}, {S("unsigned int"), 2}, {S("signed"), 2},
    {S("long"), 4}, {S("long int"), 4}, {S("unsigned long"), 4}, {S("unsigned long int"), 4}, {S("float"), 4},
    {S("double"), 8}};

const str CAST = str(
    R"(\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)|\(\s*[\w\s]+\**\s*\(\s*\*\s*\)\s*\([^()]*\)\s*\))");

// what the compilers say, by the kind of complaint: gcc's words, clang's, MSVC's
const list<std::tuple<str, list<str>>> SAYS = {
    {S("libname"), list<str>{str(R"(conflicting types for '(?P<fn>\w+)')"),
                             str(R"('(?P<fn>\w+)': redefinition; different)")}},
    {S("argtype"),
     list<str>{str(R"(passing argument (?P<n>\d+) of '(?P<fn>\w+)' (?P<how>from incompatible pointer type|makes integer )"
                   R"(from pointer without a cast|makes pointer from integer without a cast))"),
               str(R"(incompatible (?P<how>pointer types|pointer to integer conversion|integer to pointer conversion) )"
                   R"(passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of type '(?P<to>[^']*)')"),
               str(R"('function': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*)')"),
               str(R"('function': '(?P<to>[^']*)' differs in levels of indirection from '(?P<frm>[^']*)')")}},
    {S("fnptr"),
     list<str>{str(R"((?:assignment to|initialization of) '(?P<to>[^']*\(\*\)\([^']*\))'(?: \{aka '[^']*'\})? from )"
                   R"(incompatible pointer type )"
                   R"('(?P<frm>[^']*\(\*\)\([^']*\))')"),
               str(R"(incompatible function pointer types assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from )"
                   R"('(?P<frm>[^']*)')"),
               str(R"('=': incompatible types - from '(?P<frm>[^']*\*\)\([^']*\))' to '(?P<to>[^']*)')")}},
    {S("intptr"),
     list<str>{str(R"(assignment to '(?P<to>[^']*)'(?: \{aka '[^']*'\})? from '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? makes )"
                   R"(integer from pointer)"),
               str(R"(incompatible pointer to integer conversion assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from )"
                   R"('(?P<frm>[^']*\*)')"),
               str(R"('=': '(?P<to>[^'*]*)' differs in levels of indirection from '(?P<frm>[^']*\*)')")}},
    {S("assign"),
     list<str>{str(R"((?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from incompatible pointer )"
                   R"(type '(?P<frm>[^']*)')"),
               str(R"(incompatible pointer types (?:assigning to|initializing) '(?P<to>[^']*\*)'(?: \(aka '[^']*'\))? )"
                   R"((?:from|with an expression of type) '(?P<frm>[^']*)')"),
               str(R"('(?:=|initializing)': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*\*)')")}},
    {S("retptr"),
     list<str>{str(R"(returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with return type )"
                   R"('(?P<to>[^']*)'(?: \{aka '[^']*'\})? makes integer from pointer)"),
               str(R"(incompatible pointer to integer conversion returning '(?P<frm>[^']*\*)')")}},
    {S("retint"),
     list<str>{str(R"(returning '(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? from a function with return type )"
                   R"('(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? makes pointer from integer)"),
               str(R"(incompatible integer to pointer conversion returning '(?P<frm>[^'*]*)')")}},
    {S("retmix"),
     list<str>{str(R"(returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with incompatible return type )"
                   R"('(?P<to>[^']*\*)')"),
               str(R"(incompatible pointer types returning '(?P<frm>[^']*\*)')")}},
    {S("callptr"),
     list<str>{str(R"((?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from )"
                   R"('(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? makes pointer from integer)"),
               str(R"(incompatible integer to pointer conversion (?:assigning to|initializing) '(?P<to>[^']*\*)')")}},
    {S("neg"),
     list<str>{str(R"(comparison is always (?:false|true) due to limited range of data type)"),
               str(R"(result of comparison of constant -\d+ with expression of type '(?P<t>[^']*)'.* is always)"),
               str(R"('(?:==|!=)': signed/unsigned mismatch)")}},
    {S("member"),
     list<str>{str(R"(request for member '(?P<m>\w+)' in something not a structure or union)"),
               str(R"('(?:struct|union) \w+' has no member named '(?P<m>\w+)')"),
               str(R"(no member named '(?P<m>\w+)' in)"),
               str(R"(member reference base type '[^']*' is not a structure or union)"),
               str(R"(left of '->(?P<m>\w+)' must point to struct)"), str(R"(invalid type argument of '->')"),
               str(R"('(?P<m>\w+)' : is not a member of)")}},
    {S("void"),
     list<str>{str(R"(void value not ignored)"), str(R"(requires expression of scalar type \('void' invalid\))"),
               str(R"(incompatible type 'void')"), str(R"(illegal use of type 'void')"), str(R"('void' illegal)"),
               str(R"(void expression)"), str(R"(controlling expression has type 'void')")}},
    {S("ptrcast"),
     list<str>{str(R"(cast from pointer to integer of different size)"),
               str(R"(cast to smaller integer type '[^']*' from '[^']*\*')")}},
    {S("incomplete"),
     list<str>{str(R"(type of formal parameter (?P<n>\d+) is incomplete)"),
               str(R"(argument type '(?P<to>(?:union|struct) \w+)' is incomplete)")}},
    {S("unionset"),
     list<str>{str(R"(incompatible types when assigning to type '(?P<to>union \w+)' from type '(?P<frm>[^']*)')"),
               str(R"(assigning to '(?P<to>union \w+)' from incompatible type '(?P<frm>[^']*)')")}},
    {S("unionarg"),
     list<str>{str(R"(incompatible type for argument (?P<n>\d+) of '(?P<fn>\w+)')"),
               str(R"(passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of incompatible type '(?P<to>union \w+)')")}},
    {S("seqpt"),
     list<str>{str(R"(operation on '(?P<v>\w+)' may be undefined)"),
               str(R"(unsequenced modification and access to '(?P<v>\w+)')")}},
    {S("intcast"),
     list<str>{str(R"(cast to pointer from integer of different size)"),
               str(R"(cast to '[^']*' from smaller integer type)"),
               str(R"(conversion from '[^']*' to '[^']*' of greater size)")}},
    {S("arity"),
     list<str>{str(R"(too (?:many|few) arguments to function (?:'(?P<fn>\w+)'|call))"),
               str(R"(too (?:many|few) actual parameters)")}},
};

// (the kind of a complaint, the match) - or (None, None)
std::tuple<std::optional<str>, std::optional<py::re::Match>> said(const str& text) {
    for (const auto& kv : SAYS) {
        for (const str& p : std::get<1>(kv)) {
            auto m = py::re::search(p, text);
            if (m) return {std::get<0>(kv), m};
        }
    }
    return {std::nullopt, std::nullopt};
}

// ---------------------------------------------------------------- the free functions

// in a union's structures: char kind, flags; - one member a line (each can be given its own new place)
str split_view_lists(str text) {
    str code = blank(text);
    list<std::tuple<int64_t, int64_t, str>> out;
    for (const auto& um : py::re::finditer(S(R"(\bunion\s*\w*\s*\{)"), code)) {
        int64_t k = match_brace(code, um.end() - 1);
        for (const auto& vm : py::re::finditer(S(R"(\bstruct\s*\{([^{}]*)\})"), code.slice(um.end(), k))) {
            int64_t a0 = um.end() + vm.start(1);
            for (const auto& st : py::re::finditer(S(R"([^;]*;)"), code.slice(a0, um.end() + vm.end(1)))) {
                auto r = declarators(st.group(0).strip().rstrip(S(";")));
                if (!r || std::get<1>(*r).size() < 2 || st.group(0).contains(S("("))) continue;
                int64_t a = a0 + st.start(), b = a0 + st.end();
                str seg = text.slice(a, b);
                str lead = lead_of(seg);
                str ind = lead.contains(S("\n")) ? py::split(lead, S("\n"))[-1] : S("        ");
                list<str> parts;
                for (const auto& nd : std::get<1>(*r)) parts.append(std::get<0>(*r).strip() + S(" ") + std::get<1>(nd) + S(";"));
                out.append({a, b, lead + py::join(S("\n") + ind, parts)});
            }
        }
    }
    auto v = out.vec();
    std::sort(v.begin(), v.end(), [](const auto& x, const auto& y) { return y < x; });
    for (const auto& e : v) text = text.slice(0, std::get<0>(e)) + std::get<2>(e) + text.slice(std::get<1>(e), text.size());
    return text;
}

namespace {
// one member of a view: [decl start, decl end, old off, old size, new size, name, is a byte array]
struct VMember {
    std::optional<int64_t> s, e;
    int64_t off = 0;
    int64_t old_sz = 0;
    int64_t new_sz = 0;
    str name;
    bool bytes = false;
};
struct View {
    std::optional<int64_t> vs, ve;
    std::vector<VMember> members;
};
}  // namespace

// a union whose views (its structures) meet at the same old places - prc.len where lab.scope was, the bytes
// of a char f1c[6] over another's members: the views laid out again, each old place one new place in all of
// them (padding where a view has nothing there), so that what one view writes another reads where it did; its
// plain members (a pointer, a long) views of their own - they say where the next old place may be now
str union_views(str text, Notes& notes, const str& fname, const py::dict<str, std::tuple<int64_t, int64_t>>& sizes) {
    text = split_view_lists(text);
    str code = blank(text);
    std::vector<std::tuple<int64_t, int64_t, str>> edits;
    const Layouts no_structs;
    for (const auto& um : py::re::finditer(S(R"(\bunion\s*\w*\s*\{)"), code)) {
        int64_t k = match_brace(code, um.end() - 1);
        int64_t body_at = um.end();
        std::optional<std::vector<View>> views = std::vector<View>();   // (None when a view could not be read)
        for (const auto& vm : py::re::finditer(S(R"(\bstruct\s*\{)"), code.slice(body_at, k))) {
            int64_t vs = body_at + vm.end();
            int64_t d2 = 1, e = vs;
            while (e < k && d2) {
                char32_t c = code.at_(e);
                if (c == '{') d2 += 1;
                else if (c == '}') d2 -= 1;
                e += 1;
            }
            std::optional<std::vector<VMember>> members = std::vector<VMember>();
            int64_t off = 0;
            for (const auto& stmt : py::re::finditer(S(R"([^;{}]*;)"), code.slice(vs, e - 1))) {
                str s = stmt.group(0);
                if (!s.strip(S(";")).strip().size()) continue;
                bool isfar = has_search(R"(\b(far|huge)\b)", s);
                auto r = declarators(py::re::sub(S(R"(\b(far|near|huge)\b)"), S(""), s).strip());
                if (!r || std::get<1>(*r).size() != 1) {
                    members = std::nullopt;
                    break;
                }
                str n = std::get<0>(std::get<1>(*r)[0]), dcl = std::get<1>(std::get<1>(*r)[0]);
                std::optional<int64_t> old_sz = size_of(std::get<0>(*r), dcl, n, isfar ? 4 : 2, no_structs);
                if (!old_sz) {
                    auto unit = OLD_SCALARS.get(py::re::sub(S(R"(\s+)"), S(" "),
                                                            py::re::sub(S(R"(\b(register|const|volatile)\b)"), S(""),
                                                                        std::get<0>(*r))).strip());
                    int64_t cnt_ = 1;
                    for (const auto& x : py::re::finditer(S(R"(\[\s*(\d+)\s*\])"), dcl)) cnt_ *= to_int(x.group(1));
                    if (unit && *unit) old_sz = *unit * cnt_;
                    else old_sz = std::nullopt;
                }
                if (!old_sz) {
                    members = std::nullopt;
                    break;
                }
                bool ptr = rep1(dcl, n, S("")).contains(S("*"));
                int64_t cnt = 1;
                for (const auto& x : py::re::finditer(S(R"(\[\s*(\d+)\s*\])"), dcl)) cnt *= to_int(x.group(1));
                int64_t new_sz = ptr ? 8 * cnt : *old_sz;
                if (*old_sz > 1 && off % 2) off += 1;
                bool bytes_ = py::re::fullmatch(S(R"((?:unsigned\s+|signed\s+)?char)"), std::get<0>(*r).strip()).has_value() &&
                              dcl.contains(S("[")) && !ptr;
                VMember vmm;
                vmm.s = vs + stmt.start();
                vmm.e = vs + stmt.end();
                vmm.off = off;
                vmm.old_sz = *old_sz;
                vmm.new_sz = new_sz;
                vmm.name = n;
                vmm.bytes = bytes_;
                members->push_back(vmm);
                off += *old_sz;
            }
            if (!members) {
                views = std::nullopt;
                break;
            }
            View v;
            v.vs = vs;
            v.ve = e - 1;
            v.members = *members;
            views->push_back(v);
        }
        if (views && !views->empty()) {   // (its plain members: one-member views at its start)
            str inner = code.slice(body_at, k);
            str flat = py::re::sub(S(R"(\bstruct\s*\w*\s*\{[^{}]*\}[^;]*;)"),
                                   [](const py::re::Match& x) { return py::repeat(S(" "), x.group(0).size()); }, inner);
            for (const auto& st : py::re::finditer(S(R"([^;{}]*;)"), flat)) {
                auto r = declarators(st.group(0).strip().rstrip(S(";")));
                if (!r || std::get<1>(*r).size() != 1 || !st.group(0).strip().rstrip(S(";")).strip().size()) continue;
                str n = std::get<0>(std::get<1>(*r)[0]), dcl = std::get<1>(std::get<1>(*r)[0]);
                str base = py::re::sub(S(R"(\s+)"), S(" "), std::get<0>(*r)).strip();
                bool ptr = rep1(dcl, n, S("")).contains(S("*"));
                std::optional<int64_t> old_sz, new_sz;
                if (ptr) {
                    old_sz = has_search(R"(\b(far|huge)\b)", st.group(0)) ? 4 : 2;
                    new_sz = 8;
                } else if (sizes.contains(base)) {
                    old_sz = std::get<0>(sizes.at(base));
                    new_sz = std::get<1>(sizes.at(base));
                } else {
                    old_sz = OLD_SCALARS.get(base);
                    new_sz = old_sz;
                }
                if (!old_sz || !*old_sz || dcl.contains(S("["))) continue;
                VMember pm;
                pm.off = 0;
                pm.old_sz = *old_sz;
                pm.new_sz = new_sz ? *new_sz : 0;
                pm.name = n;
                pm.bytes = false;
                View pv;
                pv.members.push_back(pm);
                views->push_back(pv);
            }
        }
        if (!views || views->size() < 2) continue;
        {
            bool any_diff = false;
            for (const auto& v : *views) for (const auto& m : v.members) if (m.new_sz != m.old_sz) any_diff = true;
            if (!any_diff) continue;
        }
        std::set<int64_t> bset;
        bset.insert(0);
        for (const auto& v : *views) for (const auto& m : v.members) { bset.insert(m.off); bset.insert(m.off + m.old_sz); }
        std::map<int64_t, int64_t> nw;
        auto nwat = [&](int64_t key) -> int64_t {
            auto it = nw.find(key);
            if (it == nw.end()) throw py::KeyError("key not found");
            return it->second;
        };
        std::optional<int64_t> prev;
        for (int64_t b : bset) {
            int64_t n_ = !prev ? 0 : nwat(*prev) + (b - *prev);
            for (const auto& v : *views)
                for (const auto& m : v.members)
                    if (m.off + m.old_sz == b && !m.bytes) n_ = std::max<int64_t>(n_, nwat(m.off) + m.new_sz);
            nw[b] = n_ + ((b % 2 == 0) ? n_ % 2 : 0);
            prev = b;
        }
        bool changed = false;
        for (const auto& v : *views) {
            if (!v.vs) continue;   // (a plain member: at the start, as it is)
            int64_t cur = 0, pad = 0;
            for (const auto& m : v.members) {
                int64_t at = nwat(m.off);
                str seg = code.slice(*m.s, *m.e);
                int64_t ds = *m.s + (seg.size() - seg.lstrip().size());   // (the declaration itself)
                int64_t ls = text.rfind(S("\n"), 0, ds) + 1;
                str pre = text.slice(ls, ds);
                str indent = !pre.strip().size() ? pre : S("            ");
                if (at > cur) {   // (a line of its own before it: the room)
                    edits.emplace_back(ls, ls,
                                       py::percent(S("%schar _old%d[%d];\t\t/* (where the other views have members) */\n"),
                                                   indent, pad, at - cur));
                    pad += 1;
                    changed = true;
                }
                if (m.bytes) {   // (a byte array over other views' members: as long)
                    int64_t size = nwat(m.off + m.old_sz) - at;
                    str stmt = text.slice(ds, *m.e);
                    str new_stmt = py::re::sub(S(R"(\[\s*\d+\s*\])"), py::percent(S("[%d]"), size), stmt, 1);
                    if (new_stmt != stmt) {
                        edits.emplace_back(ds, *m.e, new_stmt);
                        changed = true;
                    }
                    cur = at + size;
                } else {
                    cur = at + m.new_sz;
                }
            }
        }
        if (changed) {
            notes[S("a union whose views meet at the same old places: laid out again, each old place one new place "
                    "in all of them")].append(fname);
        }
    }
    std::sort(edits.begin(), edits.end(), [](const auto& x, const auto& y) { return y < x; });
    for (const auto& e : edits) text = text.slice(0, std::get<0>(e)) + std::get<2>(e) + text.slice(std::get<1>(e), text.size());
    return text;
}

// the bytes of a C string literal ("a\0b" "c"): as the compiler made them, the 0 after left out
py::bytes c_string_bytes(const str& lit) {
    std::string out;
    for (const str& part : py::re::findall(S(R"x("((?:[^"\\]|\\.)*)")x"), lit)) {
        // part.encode("latin1").decode("unicode_escape").encode("latin1")
        const std::u32string& u = part.u32();
        std::u32string dec;
        for (size_t i = 0; i < u.size(); ++i) {
            char32_t c = u[i];
            if (c != '\\') { dec.push_back(c); continue; }
            if (i + 1 >= u.size()) throw py::ValueError("\\ at end of string");
            char32_t n = u[++i];
            auto hex = [&](size_t count) {
                char32_t v = 0;
                for (size_t q = 0; q < count; ++q) {
                    if (i + 1 >= u.size()) throw py::ValueError("truncated escape");
                    char32_t h = u[++i];
                    int dgt = (h >= '0' && h <= '9') ? h - '0' : (h >= 'a' && h <= 'f') ? h - 'a' + 10
                              : (h >= 'A' && h <= 'F') ? h - 'A' + 10 : -1;
                    if (dgt < 0) throw py::ValueError("truncated escape");
                    v = v * 16 + dgt;
                }
                return v;
            };
            switch (n) {
                case '\n': break;
                case '\\': dec.push_back('\\'); break;
                case '\'': dec.push_back('\''); break;
                case '"': dec.push_back('"'); break;
                case 'a': dec.push_back(7); break;
                case 'b': dec.push_back(8); break;
                case 'f': dec.push_back(12); break;
                case 'n': dec.push_back(10); break;
                case 'r': dec.push_back(13); break;
                case 't': dec.push_back(9); break;
                case 'v': dec.push_back(11); break;
                case 'x': dec.push_back(hex(2)); break;
                case 'u': dec.push_back(hex(4)); break;
                case 'U': dec.push_back(hex(8)); break;
                default:
                    if (n >= '0' && n <= '7') {
                        char32_t v = n - '0';
                        for (int q = 0; q < 2 && i + 1 < u.size() && u[i + 1] >= '0' && u[i + 1] <= '7'; ++q) v = v * 8 + (u[++i] - '0');
                        dec.push_back(v);
                    } else {
                        dec.push_back('\\');
                        dec.push_back(n);
                    }
            }
        }
        for (char32_t c : dec) {
            if (c > 255) throw py::ValueError("'latin-1' codec can't encode character");
            out.push_back(static_cast<char>(c));
        }
    }
    return py::bytes(out);
}

// the size a declaration had in the original (its pointers ptr bytes) - or None
std::optional<int64_t> size_of(const str& base, const str& decl, const str& name, int64_t ptr, const Layouts& structs) {
    str d = rep1(decl, name, S(""));
    int64_t n = 1;
    for (const auto& x : py::re::finditer(S(R"(\[\s*(\d+)\s*\])"), d)) n *= to_int(x.group(1));
    if (d.contains(S("*")) || d.contains(S("("))) return ptr * n;
    str b = py::re::sub(S(R"(\s+)"), S(" "),
                        py::re::sub(S(R"(\b(?:register|static|extern|auto|const|volatile)\s+)"), S(""), base)).strip();
    if (SIZES.contains(b)) return SIZES.at(b) * n;
    if (structs.contains(b)) return structs.at(b).size * n;
    return std::nullopt;
}

// {"struct S": {member: (offset, size), "@size": n}} of every structure, laid out as the 8086 compilers did
// (a word on an even offset; a nested struct or union one member)
py::dict<str, SimpleLayout> layouts(const list<str>& texts, int64_t ptr) {
    Layouts full = asm16::CTypes::_layouts(texts, ptr, ptr);
    py::dict<str, SimpleLayout> out;
    for (const auto& kv : full.map.items()) {
        SimpleLayout sl;
        for (const auto& m : std::get<1>(kv).members) sl.members.set(m.first, {m.second.off, m.second.size});
        sl.size = std::get<1>(kv).size;
        out.set(std::get<0>(kv), sl);
    }
    return out;
}

py::list<py::list<Span>> _calls(const str& body, const str& g) {
    list<list<Span>> out;
    for (const auto& m : py::re::finditer(py::percent(S(R"(\b%s\s*\()"), E(g)), body)) {
        int64_t i = m.end(), d = 1, start = m.end();
        list<Span> spans;
        while (i < body.size() && d) {
            char32_t c = body.at_(i);
            d += in_chars(c, "([") - in_chars(c, ")]");
            if ((c == ',' && d == 1) || d == 0) {
                spans.append({start, i});
                start = i + 1;
            }
            i += 1;
        }
        out.append(spans);
    }
    return out;
}

// the function definitions of a file, old style or new: (name, first line, '{' line, last line)
list<DefinitionAny> definitions_any(const list<str>& lines, const list<str>& code) {
    list<DefinitionAny> out;
    list<int64_t> ds = depths(code);
    int64_t i = 0;
    while (i < lines.size()) {
        std::optional<py::re::Match> m;
        if (ds[i] == 0) m = py::re::match(S(R"(^[\w\s*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$)"), code[i]);
        if (m && !KEYWORDS.contains(m->group(1)) && i + 1 < lines.size() && code[i + 1].strip().startswith(S("{"))) {
            int64_t j = i + 1, d = 0, k = i + 1;
            while (k < lines.size()) {
                d += code[k].count(S("{")) - code[k].count(S("}"));
                if (d == 0) break;
                k += 1;
            }
            out.append({m->group(1), i, j, k});
            i = k + 1;
        } else {
            i += 1;
        }
    }
    return out;
}

// {name: (base, declarator, size, line, register)} of the declarations heading a body, in their order
py::dict<str, LocalDecl> _locals(const list<str>& lines, const list<str>& code, int64_t j, int64_t k, int64_t ptr) {
    (void)lines;
    py::dict<str, LocalDecl> out;
    const Layouts none;
    for (int64_t x = j + 1; x < k; ++x) {
        str s = code[x].strip();
        if (!s.size()) continue;
        std::optional<std::tuple<str, list<std::tuple<str, str>>>> r;
        if (s.endswith(S(";")) && !s.contains(S("=")) && !s.replace(S("(*"), S("")).contains(S("(")))
            r = declarators(s);
        if (!r || !std::get<0>(*r).size()) break;
        list<str> words = py::split(py::re::sub(S(R"(\b(?:struct|union)\s+\w+)"), S(""), std::get<0>(*r)));
        bool sub = true;
        for (const str& w : words) if (!TYPEWORDS.contains(w)) sub = false;
        if (!sub) break;
        for (const auto& nd : std::get<1>(*r)) {
            LocalDecl ld;
            ld.base = std::get<0>(*r);
            ld.decl = std::get<1>(nd);
            ld.size = size_of(std::get<0>(*r), std::get<1>(nd), std::get<0>(nd), ptr, none);
            ld.line = x;
            ld.reg = std::get<0>(*r).contains(S("register"));
            out.set(std::get<0>(nd), ld);
        }
    }
    return out;
}

void _overlay(Port& port, const str& fname, const list<str>& lines_in, const list<str>& code, int64_t j, int64_t k,
              const py::dict<str, LocalDecl>& locs, const str& a, int64_t e, const str& g) {
    (void)j;
    std::vector<str> names = locs.keys();
    int64_t i = std::find(names.begin(), names.end(), a) - names.begin();
    const LocalDecl& la = locs.at(a);
    std::vector<std::tuple<str, str, str>> members;
    members.emplace_back(a, la.base, la.decl);
    int64_t off = la.size ? *la.size : 0;
    for (int64_t q = i - 1; q >= 0; --q) {   // (the frame grows down: those declared before it lie above, the nearest first)
        const str& n = names[q];
        const LocalDecl& l2 = locs.at(n);
        if (l2.reg) continue;
        if (!l2.size) return;
        if (*l2.size > 1 && off % 2) off += 1;
        members.emplace_back(n, l2.base, l2.decl);
        off += *l2.size;
        if (off >= e) break;
    }
    if (off < e) return;
    py::set<str> gone;
    for (const auto& m : members) gone.add(std::get<0>(m));
    std::vector<std::optional<str>> lines;
    for (const str& l : lines_in) lines.push_back(l);
    std::set<int64_t> gone_lines;
    for (const str& n : gone) gone_lines.insert(locs.at(n).line);
    for (auto it = gone_lines.rbegin(); it != gone_lines.rend(); ++it) {   // (the declarations: the rest of each line kept)
        int64_t x = *it;
        auto r = declarators(code[x].strip());
        list<str> keep;
        for (const auto& nd : std::get<1>(*r)) if (!gone.contains(std::get<0>(nd))) keep.append(std::get<1>(nd));
        str ln = *lines[x];
        str ind = ln.slice(0, ln.size() - ln.lstrip().size());
        if (keep.size()) lines[x] = ind + std::get<0>(*r) + S(" ") + py::join(S(", "), keep) + S(";");
        else lines[x] = std::nullopt;
    }
    str ind = S("\t");
    list<str> decl;
    decl.append(ind + py::percent(S("struct {\t\t\t/* side by side, as the original's frame had them: %s reads past %s */"), g, a));
    for (const auto& m : members) {
        decl.append(ind + S("\t") + py::re::sub(S(R"(\bregister\s+)"), S(""), std::get<1>(m)) + S(" ") + std::get<2>(m) + S(";"));
    }
    decl.append(ind + S("} frame;"));
    int64_t first = locs.at(a).line;   // (where the array was declared)
    {
        list<str> parts;
        if (lines[first]) parts.append(*lines[first]);
        parts.extend(decl);
        lines[first] = join_nl(parts);
    }
    list<str> esc;
    for (const str& n : gone) esc.append(E(n));
    py::re::Pattern rx(py::percent(S(R"((?<![.>\w])(%s)\b)"), py::join(S("|"), esc)), 0);
    for (int64_t x = first + 1; x <= k; ++x) {
        if (lines[x]) lines[x] = sub_code(*lines[x], rx, [](const py::re::Match& m) { return S("frame.") + m.group(1); });
    }
    list<str> kept;
    for (const auto& l : lines) if (l) kept.append(*l);
    list<str> flat = lines_of(join_nl(kept));
    lines_in.clear();
    lines_in.extend(flat);
    list<str> names_out;
    for (const auto& m : members) names_out.append(std::get<0>(m));
    port.notes[S("locals the program reaches past an array (as the original's frame had them): kept side by side "
                 "in a structure")]
        .append(py::percent(S("%s (%s: %s)"), fname, g, py::join(S(", "), names_out)));
}

// a local array the program reads past the end of - given to a function that takes its pointer for a
// structure's (a name buffer read as a struct whose next member is the frame's next variable): the locals it
// reaches, in a structure of their own, side by side as the original's frame had them
void frames(Port& port, py::dict<str, str>& out) {
    int64_t wp = port.profile.widths_pointer ? *port.profile.widths_pointer : 2;
    int64_t ptr = floordiv(wp, 8);
    if (!ptr) ptr = 2;
    list<str> out_values;
    for (const str& v : out.values()) out_values.append(v);
    py::dict<str, SimpleLayout> lay = layouts(out_values, ptr);
    py::dict<str, list<str>> owner;
    for (const auto& kv : lay.items()) {
        for (const auto& m : std::get<1>(kv).members.keys()) {
            if (m != S("@size")) owner[m].append(std::get<0>(kv));
        }
    }
    py::dict<std::tuple<str, int64_t>, int64_t> need;   // ((function, parameter index): bytes it reaches)
    for (const auto& f : port.fns.values()) {
        const str& body = f->body;
        int64_t i = 0;
        for (const auto& pd : f->params) {
            str p = std::get<0>(pd), decl = std::get<1>(pd);
            int64_t idx = i++;
            if (!decl.contains(S("*"))) continue;
            auto own = py::re::search(S(R"(\b(struct \w+)\s*\*)"), decl);
            int64_t e = 0;
            for (const auto& m : py::re::finditer(
                     py::percent(S(R"((?:\(\s*\(\s*(struct \w+)\s*\*\s*\)\s*%s\s*\)|\b%s)\s*->\s*(\w+))"), p, p), body)) {
                std::optional<str> s = m.group_opt(1);
                if (!s || !s->size()) {
                    s = std::nullopt;
                    if (own && lay.contains(own->group(1))) s = own->group(1);
                }
                if (!s) {
                    auto ow = owner.get(m.group(2));
                    if (ow && ow->size()) s = (*ow)[0];
                }
                if (s && lay.contains(*s) && lay.at(*s).members.contains(m.group(2))) {
                    auto [o, sz] = lay.at(*s).members.at(m.group(2));
                    e = std::max<int64_t>(e, o + sz);
                }
            }
            if (e) need.set({f->name, idx}, e);
        }
    }
    for (int q = 0; q < 8; ++q) {   // (through the functions that hand the pointer on)
        bool grew = false;
        for (const auto& f : port.fns.values()) {
            const str& body = f->body;
            int64_t i = 0;
            for (const auto& pd : f->params) {
                str p = std::get<0>(pd);
                int64_t idx = i++;
                auto items = need.items();
                for (const auto& it : items) {
                    str g = std::get<0>(std::get<0>(it));
                    int64_t k = std::get<1>(std::get<0>(it));
                    int64_t e = std::get<1>(it);
                    for (const auto& spans : _calls(body, g)) {
                        if (k < spans.size() && body.slice(std::get<0>(spans[k]), std::get<1>(spans[k])).strip() == p &&
                            need.get({f->name, idx}, 0) < e) {
                            need.set({f->name, idx}, e);
                            grew = true;
                        }
                    }
                }
            }
        }
        if (!grew) break;
    }
    if (!need.size()) return;
    for (const auto& kv : out.items()) {
        str fname = std::get<0>(kv), text = std::get<1>(kv);
        if (!fname.endswith(S(".c"))) continue;
        list<str> lines = lines_of(text);
        list<str> code = lines_of(blank(text));
        list<DefinitionAny> defl = definitions_any(lines, code);
        auto& defs = defl.vec();
        for (auto d = defs.rbegin(); d != defs.rend(); ++d) {
            int64_t j = std::get<2>(*d), k = std::get<3>(*d);
            str body = join_nl(code.slice(j, k + 1));
            auto locs = _locals(lines, code, j, k);
            auto arrays_of = [&](const py::dict<str, LocalDecl>& l) {
                py::dict<str, int64_t> arrays;
                for (const auto& lk : l.items()) {
                    const LocalDecl& ld = std::get<1>(lk);
                    if (ld.decl.contains(S("[")) && ld.size && *ld.size) arrays.set(std::get<0>(lk), *ld.size);
                }
                return arrays;
            };
            auto arrays = arrays_of(locs);
            for (const auto& it : need.items()) {
                str g = std::get<0>(std::get<0>(it));
                int64_t idx = std::get<1>(std::get<0>(it));
                int64_t e = std::get<1>(it);
                for (const auto& spans : _calls(body, g)) {
                    if (idx >= spans.size()) continue;
                    str a = body.slice(std::get<0>(spans[idx]), std::get<1>(spans[idx])).strip();
                    if (!arrays.contains(a) || arrays.at(a) >= e) continue;
                    _overlay(port, fname, lines, code, j, k, locs, a, e, g);
                    code = lines_of(blank(join_nl(lines)));
                    body = join_nl(code.slice(j, k + 1));
                    locs = _locals(lines, code, j, k);
                    arrays = arrays_of(locs);
                }
            }
        }
        out.set(fname, join_nl(lines));
    }
}

// {member: [the structures that have it]} of every structure the program defines
py::dict<str, list<str>> members(const list<str>& texts) {
    py::dict<str, list<str>> out;
    for (const str& t : texts) {
        str code = blank(t);
        for (const auto& m : py::re::finditer(S(R"(\b(struct|union)\s+(\w+)\s*\{)"), code)) {
            int64_t k = match_brace(code, m.end() - 1);
            for (const str& s : py::split(code.slice(m.end(), k), S(";"))) {
                std::optional<std::tuple<str, list<std::tuple<str, str>>>> r;
                if (s.strip().size()) r = declarators(s);
                if (r) {
                    for (const auto& nd : std::get<1>(*r)) out[std::get<0>(nd)].append(m.group(1) + S(" ") + m.group(2));
                }
            }
        }
    }
    return out;
}

// the start of the postfix expression that ends just before `at` (the -> of a member)
int64_t object_before(const str& line, int64_t at) {
    int64_t i = at;
    while (true) {
        while (i && in_chars(line.at_(py::index_(i - 1, line.size(), "string")), " \t")) i -= 1;
        if (i && in_chars(line.at_(py::index_(i - 1, line.size(), "string")), ")]")) {
            char32_t close = line.at_(py::index_(i - 1, line.size(), "string"));
            char32_t open_ = close == ')' ? '(' : '[';
            int64_t d = 0;
            i = i - 1;
            while (i >= 0) {
                char32_t c = line.at_(i);
                if (c == close) d += 1;
                else if (c == open_) d -= 1;
                if (d == 0) break;
                i -= 1;
            }
            if (open_ == '(' && !(i && py::re::match(S(R"(\w)"), line[i - 1]).has_value())) return i;   // (a parenthesized object: the whole)
            continue;
        }
        auto m = py::re::search(S(R"([A-Za-z_]\w*$)"), line.slice(0, i));
        if (!m) return i;
        i = m->start();
        if (line.slice(0, i).endswith(S("->")) || line.slice(0, i).endswith(S("."))) {
            i -= line.slice(0, i).endswith(S("->")) ? 2 : 1;
            continue;
        }
        return i;
    }
}

// 'register char *name' -> 'char *' (the type alone, for a cast)
str abstract(const str& decl, const str& name) {
    str t = py::re::sub(py::percent(S(R"((?<!struct )(?<!union )(?<!enum )\b%s\b)"), E(name)), S(""),
                        py::re::sub(S(R"(\bregister\s+)"), S(""), decl), 1)
                .strip();   // (struct bcd *bcd: the name, not the tag)
    t = py::re::sub(S(R"(\s*\[\s*\d*\s*\]$)"), S(" *"), t);
    return py::re::sub(S(R"(\s+\*)"), S(" *"), t);
}

// [(start, end) of each argument] of the first call of fn on the line, or None (partial: of a call that goes
// on to the next line, the arguments whole on this one)
std::optional<list<Span>> call_args(const str& line, const str& fn, bool partial) {
    auto m = py::re::search(py::percent(S(R"(\b%s\s*\()"), E(fn)), line);
    if (!m) return std::nullopt;
    int64_t i = m->end(), d = 1, start = m->end();
    list<Span> spans;
    while (i < line.size() && d) {
        char32_t c = line.at_(i);
        if (in_chars(c, "([{")) d += 1;
        else if (in_chars(c, ")]}")) d -= 1;
        if ((c == ',' && d == 1) || d == 0) {
            spans.append({start, i});
            start = i + 1;
        }
        i += 1;
    }
    if (d == 0 || (partial && d >= 1 && spans.size())) return spans;
    return std::nullopt;
}

// whether a parameter is only ever tested (if (p), !p, p && q): a pointer given for it may be p != 0
bool truthy(const str& body, const str& name) {
    list<py::re::Match> uses = py::re::finditer(py::percent(S(R"(\b%s\b)"), E(name)), body);
    if (!uses.size()) return false;
    for (const auto& u : uses) {
        if (!(py::re::search(S(R"((?:(?:^|[^\w\s\])]|\b(?:if|while|for|return))\s*\(|!|&&|\|\|)\s*$)"), body.slice(0, u.start())).has_value() &&   // (a call's argument is no test)
              py::re::match(S(R"(\s*(?:\)|&&|\|\|))"), body.slice(u.end(), body.size())).has_value()))
            return false;
    }
    return true;
}

// ---------------------------------------------------------------- Port methods

// f(&g_4e52) where f takes a struct type *, g_4e52 a variable of the BSS list and those after it at the
// structure's members' places (g_4e56 its t_code, g_4e58 its t_kind): the old linker's order made them one
// structure - one variable of it, their names its members'
void Port::bss_views() {
    if (!bss.size() || _bss_views_done) return;
    _bss_views_done = true;
    const Layouts& old = old_layouts();
    std::vector<str> names = bss.keys();
    py::dict<str, int64_t> offs;   // (each one's place from the first: the sizes in order)
    int64_t o = 0;
    for (const str& nm : names) {
        offs.set(nm, o);
        o += bss.at(nm);
    }
    py::dict<str, str> found;
    for (const FnP& fn : fns.values()) {
        py::dict<int64_t, str> ks;
        int64_t k = 0;
        for (const auto& pd : fn->params) {
            auto x = py::re::match(S(R"(\s*(?:register\s+)?((?:struct|union)\s+\w+)\s*\*\s*\w+\s*$)"), std::get<1>(pd));
            if (x) ks.set(k, py::re::sub(S(R"(\s+)"), S(" "), x->group(1)));
            ++k;
        }
        if (!ks.size()) continue;
        for (const str& t : files.values()) {
            str code = blank(t);
            for (const auto& m : py::re::finditer(py::percent(S(R"(\b%s\s*\()"), E(fn->name)), code)) {
                auto sp = call_args(code.slice(m.start(), code.size()), fn->name);
                list<Span> spans = sp ? *sp : list<Span>();
                for (const auto& kv : ks.items()) {
                    int64_t kk = std::get<0>(kv);
                    const str& st_ = std::get<1>(kv);
                    if (kk < spans.size()) {
                        str a = code.slice(m.start() + std::get<0>(spans[kk]), m.start() + std::get<1>(spans[kk])).strip();
                        auto x = py::re::fullmatch(S(R"(&\s*(\w+))"), a);
                        if (x && bss.contains(x->group(1)) && old.contains(st_) && bss.at(x->group(1)) < old.at(st_).size)
                            found.set(x->group(1), st_);
                    }
                }
            }
        }
    }
    std::vector<std::tuple<str, str>> fitems = found.items();
    std::sort(fitems.begin(), fitems.end());
    for (const auto& fv : fitems) {
        const str& v = std::get<0>(fv);
        const str& st_ = std::get<1>(fv);
        const Layout& lay = old.at(st_);
        py::dict<int64_t, str> mem;
        for (const auto& kv : lay.members) mem.set(kv.second.off, kv.first);
        int64_t k0 = std::find(names.begin(), names.end(), v) - names.begin();
        std::optional<std::vector<std::tuple<str, str>>> run = std::vector<std::tuple<str, str>>();
        for (size_t q = k0; q < names.size(); ++q) {
            const str& nm = names[q];
            int64_t rel = offs.at(nm) - offs.at(v);
            if (rel >= lay.size) break;
            if (!mem.contains(rel)) {
                run = std::nullopt;
                break;
            }
            run->emplace_back(nm, mem.at(rel));
        }
        if (!run || run->empty()) continue;
        bool aliased = false;
        for (const auto& r : *run) if (aliases.contains(std::get<0>(r))) aliased = true;
        if (aliased) continue;
        str var = v + S("_s");
        for (const auto& r : *run) {
            aliases.set(std::get<0>(r), {py::percent(S("(%s.%s)"), var, std::get<1>(r)), py::percent(S("%s %s;"), st_, var)});
        }
        list<str> nms;
        for (const auto& r : *run) nms.append(std::get<0>(r));
        bss_struct.set(var, {st_, nms});
        for (const auto& hv : files.items()) {
            const str& h = std::get<0>(hv);
            if (h.endswith(S(".h")) && py::re::search(py::percent(S(R"(\b%s\s*\{)"), tagrx(st_)), blank(std::get<1>(hv))))
                view_headers.add(h);
        }
        list<str> parts;
        for (const auto& r : *run) parts.append(py::percent(S("%s=.%s"), std::get<0>(r), std::get<1>(r)));
        notes[S("variables of the BSS list one after another that a routine takes as a structure (its "
                "members' places): one variable of it")]
            .append(py::percent(S("%s (%s: %s)"), var, st_, py::join(S(", "), parts)));
    }
}

// the program's functions that give blocks of memory: a call of one given a structure's whole old size
// somewhere ((struct inode *)f0a42b(2, 32)) - the evidence that a number given it is a size
const py::dict<str, py::set<int64_t>>& Port::allocators() {
    if (!_allocs) {
        const Layouts& old = old_layouts();
        py::dict<str, py::set<int64_t>> out;   // (name: the places of its arguments the size was at)
        for (const str& t : originals.values()) {
            str code = blank(t);
            py::dict<str, str> ptr_of;
            for (const auto& m : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])"), code))
                ptr_of.set(m.group(2), py::re::sub(S(R"(\s+)"), S(" "), m.group(1)));
            for (const auto& m : py::re::finditer(
                     S(R"((?:\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)|\b(\w+)\s*=)\s*)"
                       R"((?:\w+\s*\(\s*)?([A-Za-z_]\w*)\s*\(([^()]*)\))"),
                     code)) {
                std::optional<str> st_;
                if (m.group_opt(1) && m.group_opt(1)->size()) st_ = py::re::sub(S(R"(\s+)"), S(" "), m.group(1));
                else if (m.group_opt(2)) st_ = ptr_of.get(*m.group_opt(2));
                if (!st_ || !old.contains(*st_) || KEYWORDS.contains(m.group(3))) continue;
                py::set<int64_t> at_;
                int64_t k = 0;
                for (const str& a_ : py::split(m.group(4), S(","))) {
                    if (py::re::fullmatch(S(R"(\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*)"), a_) &&
                        to_int0(a_.strip()) == old.at(*st_).size)
                        at_.add(k);
                    ++k;
                }
                if (at_.size()) out.setdefault(m.group(3), py::set<int64_t>()).update(at_);
            }
        }
        if (!rule(S("alloc_places"))) {   // (the names alone: no place of the size known)
            py::dict<str, py::set<int64_t>> names_only;
            for (const str& n : out.keys()) names_only.set(n, py::set<int64_t>());
            _allocs = names_only;
            return *_allocs;
        }
        py::dict<str, FnP> defs_;   // (and what one gives on to another: f03f02(n, v, pool)
        py::dict<str, str> bodies;  //  { f03e86(pool, n); } - f03e86's size its second)
        for (const auto& nt : files.items()) {
            const str& n = std::get<0>(nt);
            if (n.endswith(S(".c"))) {
                list<str> cl_ = lines_of(blank(std::get<1>(nt)));
                for (const auto& d : definitions(lines_of(std::get<1>(nt)), cl_)) {
                    const FnP& f = std::get<0>(d);
                    defs_.setdefault(f->name, f);
                    bodies.setdefault(f->name, join_nl(cl_.slice(std::get<2>(d), std::get<3>(d) + 1)));
                }
            }
        }
        for (int q = 0; q < 3; ++q) {
            for (const auto& ak : out.items()) {
                const str& a = std::get<0>(ak);
                const py::set<int64_t>& ks = std::get<1>(ak);
                if (ks.size() != 1 || !defs_.contains(a) ||
                    *std::min_element(ks.begin(), ks.end()) >= defs_.at(a)->params.size())
                    continue;
                str pa = std::get<0>(defs_.at(a)->params[*std::min_element(ks.begin(), ks.end())]);
                str body = bodies.get(a, S(""));
                for (const auto& m : py::re::finditer(S(R"(\b(\w+)\s*\()"), body)) {
                    if (KEYWORDS.contains(m.group(1)) || m.group(1) == a || !defs_.contains(m.group(1))) continue;
                    auto sp = call_args(body.slice(m.start(), m.start() + 400), m.group(1));
                    list<Span> spans = sp ? *sp : list<Span>();
                    std::vector<int64_t> js;
                    int64_t jj = 0;
                    for (const auto& s : spans) {
                        if (body.slice(m.start() + std::get<0>(s), m.start() + std::get<1>(s)).strip() == pa) js.push_back(jj);
                        ++jj;
                    }
                    if (js.size() == 1) out.set(m.group(1), py::set<int64_t>{js[0]});
                }
            }
        }
        _allocs = out;
    }
    return *_allocs;
}

// {BSS name: (base type, declarator)} - what the program's externs say of each of its arrays
py::dict<str, std::tuple<str, str>> Port::bss_decls() {
    py::dict<str, std::tuple<str, str>> decl;
    for (const auto& nt : originals.items()) {
        if (!(std::get<0>(nt).endswith(S(".c")) || std::get<0>(nt).endswith(S(".h")))) continue;
        str t = blank(std::get<1>(nt));
        list<str> tl = lines_of(t);
        list<int64_t> ds = depths(tl);
        for (int64_t i = 0; i < tl.size(); ++i) {
            str st_ = tl[i].strip();
            if (ds[i] || !st_.startswith(S("extern")) || !st_.endswith(S(";")) || st_.replace(S("(*"), S("")).contains(S("(")))
                continue;
            auto r = declarators(py::re::sub(S(R"(^extern\s+)"), S(""), st_).rstrip(S(";")));
            if (r) {
                for (const auto& nd : std::get<1>(*r)) {
                    if (bss.contains(std::get<0>(nd)) && std::get<1>(nd).contains(S("[")))
                        decl.setdefault(std::get<0>(nd), {py::re::sub(S(R"(\s+)"), S(" "), std::get<0>(*r)).strip(), std::get<1>(nd)});
                }
            }
        }
    }
    return decl;
}

// a BSS array the program went past the end of into the variable after it (in the old layout: the
// BSS list's order) - the two one array, the second's name a place in it, when they are arrays of one
// element type; -> whether they were
bool Port::join_bss(const str& var) {
    std::vector<str> names = bss.keys();
    str head = var;
    for (const auto& hr : bss_joined.items()) {
        if (std::get<1>(hr).contains(var)) {
            head = std::get<0>(hr);
            break;
        }
    }
    list<str> chain;
    chain.append(head);
    chain.extend(bss_joined.get(head, list<str>()));
    auto pos = std::find(names.begin(), names.end(), chain[-1]);
    if (pos == names.end() || (pos - names.begin()) + 1 >= static_cast<int64_t>(names.size())) return false;
    str nxt = *(pos + 1);
    auto decls = bss_decls();
    auto a = decls.get(head);
    auto b = decls.get(nxt);
    auto shape = [](const std::tuple<str, str>& d) {   // (*v[64] and *w[64]: one shape)
        return py::re::sub(S(R"(\[[^\]]*\])"), S("[]"), py::re::sub(S(R"([A-Za-z_]\w*)"), S("@"), std::get<1>(d), 1));
    };
    if (!a || !b || std::get<0>(*a) != std::get<0>(*b) || shape(*a) != shape(*b) || bss_struct.contains(nxt)) return false;
    {
        list<str> joined_head = bss_joined.get(head, list<str>()).copy();
        list<str> add;
        add.append(nxt);
        list<str> popped = bss_joined.pop(nxt, list<str>());
        joined_head.append(nxt);
        joined_head.extend(popped);
        bss_joined.set(head, joined_head);
    }
    bss_definitions();   // (the places' names now: their externs dropped)
    auto n_ = py::re::search(S(R"(\[(\d+)\])"), std::get<1>(aliases.at(nxt)));
    if (n_) {   // (and the array's own externs of its new size)
        decl_rules.append(DeclRule(py::percent(S(R"((\bextern\b[^;\n]*?\b%s\s*\[)\s*\d*\s*\])"), E(head)),
                                   py::percent(S(R"(\g<1>%s])"), n_->group(1))));
    }
    return true;
}

// the definitions of the BSS list's variables no file of the program defines: the type its extern says,
// an array of as many elements as its old bytes hold
list<str> Port::bss_definitions() {
    if (!bss.size()) return list<str>();
    const Layouts& old = old_layouts();
    list<str> texts;
    for (const auto& nt : originals.items())
        if (std::get<0>(nt).endswith(S(".c")) || std::get<0>(nt).endswith(S(".h"))) texts.append(blank(std::get<1>(nt)));
    py::set<str> defined;
    py::dict<str, std::tuple<str, str>> decl;
    for (const str& t : texts) {
        list<str> tl = lines_of(t);
        list<int64_t> ds = depths(tl);
        for (int64_t i = 0; i < tl.size(); ++i) {
            str st_ = tl[i].strip();
            if (ds[i] || !st_.endswith(S(";")) || st_.replace(S("(*"), S("")).contains(S("("))) continue;
            bool ext = st_.startswith(S("extern"));
            auto r = declarators(
                py::re::sub(S(R"(=.*;$)"), S(";"), py::re::sub(S(R"(^(?:extern|static)\s+)"), S(""), st_)).rstrip(S(";")));
            if (!r) continue;
            for (const auto& nd : std::get<1>(*r)) {
                const str& nm = std::get<0>(nd);
                const str& dcl = std::get<1>(nd);
                if (bss.contains(nm)) {
                    if (ext) {   // (extern int g in one file, char *g in another: the pointer)
                        auto was = decl.get(nm);
                        if (!was ||
                            (py::re::fullmatch(S(R"((?:unsigned\s+|signed\s+)?(?:int|short|unsigned))"), std::get<0>(*was)) &&
                             ((std::get<1>(*was) == nm && dcl != nm) ||   // (and
                              (dcl.contains(S("*")) && !std::get<1>(*was).contains(S("*")) && rule(S("bss_ptr_tables")))))) {
                            // (and int g[]; struct ins *g[];)
                            decl.set(nm, {std::get<0>(*r).strip(), dcl});
                        }
                    } else {
                        defined.add(nm);
                    }
                }
            }
        }
    }
    list<str> out;
    py::set<str> members_;
    for (const auto& vs : bss_struct.items()) {
        const str& var = std::get<0>(vs);
        const str& st_ = std::get<0>(std::get<1>(vs));
        const list<str>& nms = std::get<1>(std::get<1>(vs));
        out.append(py::percent(S("%s %s;\t/* (%s: one place, as the old linker kept them) */"), st_, var, py::join(S(", "), nms)));
        for (const str& n : nms) members_.add(n);
    }
    for (const auto& bv : bss.items()) {
        const str& nm = std::get<0>(bv);
        int64_t size = std::get<1>(bv);
        if (defined.contains(nm) || profile.runtime_internals.contains(nm) || nm.startswith(S("_")) || members_.contains(nm))
            continue;
        bool in_rest = false;
        for (const auto& jr : bss_joined.items()) if (std::get<1>(jr).contains(nm)) in_rest = true;
        if (in_rest) continue;   // (a place in the array before it: joined)
        list<str> parts;
        parts.append(nm);
        parts.extend(bss_joined.get(nm, list<str>()));
        size = 0;
        for (const str& p_ : parts) size += bss.get(p_, 0);
        auto dd = decl.get(nm);
        str base = dd ? std::get<0>(*dd) : S("char");
        str dcl = dd ? std::get<1>(*dd) : nm + S("[]");
        if (!dcl.contains(S("["))) {
            out.append(py::percent(S("%s %s;"), base, dcl));
            continue;
        }
        str el = py::re::sub(S(R"(\[[^\]]*\])"), S(""), dcl, 1).replace(nm, S(""));
        list<py::re::Match> inner_all = py::re::finditer(S(R"(\[\s*(\d+)\s*\])"), dcl);
        int64_t esz;
        if (el.contains(S("*"))) esz = 2;
        else if (old.contains(base)) esz = old.at(base).size;
        else esz = OLD_SCALARS.get(base, 2);
        for (int64_t q = 1; q < inner_all.size(); ++q) esz *= to_int(inner_all[q].group(1));
        int64_t room = (base == S("char") && esz == 1) ? 16 : 0;   // (bytes: a block's head may have grown)
        if (parts.size() > 1) {   // (the names after it: places in it)
            int64_t at_ = 0;
            int64_t cnt = std::max<int64_t>(1, floordiv(size, std::max<int64_t>(1, esz))) + (rule(S("bss_room")) ? room : 0);
            str full = py::percent(S("%s %s;"), base,
                                   py::re::sub(S(R"(\[\s*\]|\[[^\]]*\])"), py::percent(S("[%d]"), cnt), dcl, 1));
            for (const str& p_ : parts) {
                if (p_ != nm)
                    aliases.set(p_, {py::percent(S("(&%s[%d])"), nm, floordiv(at_, std::max<int64_t>(1, esz))), full});
                at_ += bss.get(p_, 0);
            }
        }
        int64_t cnt2 = std::max<int64_t>(1, floordiv(size, std::max<int64_t>(1, esz))) + room;
        out.append(py::percent(S("%s %s;\t/* (%d bytes then%s) */"), base,
                               py::re::sub(S(R"(\[\s*\]|\[[^\]]*\])"), py::percent(S("[%d]"), cnt2), dcl, 1), size,
                               room ? S(", and room") : S("")));
    }
    return out;
}

// {global array: the bytes of one of its elements in the original} (its far pointers 4, near ones 2)
const py::dict<str, int64_t>& Port::old_elem_sizes() {
    if (!_old_elems) {
        const Layouts& old = old_layouts();
        py::dict<str, int64_t> out;
        for (const str& t : originals.values()) {
            str code = blank(t);
            list<str> cl = lines_of(code);
            list<int64_t> ds = depths(cl);
            for (int64_t i = 0; i < cl.size(); ++i) {
                str st_ = cl[i].strip();
                if (ds[i] || !st_.contains(S("[")) || !st_.endswith(S(";")) || st_.replace(S("(*"), S("")).contains(S("(")))
                    continue;
                bool isfar = has_search(R"(\b(far|huge)\b)", st_);
                auto r = declarators(
                    py::re::sub(S(R"(\b(far|near|huge)\b)"), S(""), py::re::sub(S(R"(=.*;$)"), S(";"), st_)).rstrip(S(";")));
                if (!r) continue;
                str base = py::re::sub(S(R"(\s+)"), S(" "),
                                       py::re::sub(S(R"(\b(extern|static|register|const)\b)"), S(""), std::get<0>(*r)))
                               .strip();
                for (const auto& nd : std::get<1>(*r)) {
                    const str& nm = std::get<0>(nd);
                    str el = py::re::sub(S(R"(\[[^\]]*\])"), S(""), std::get<1>(nd), 1);
                    if (el.replace(nm, S("")).contains(S("*"))) {
                        out.setdefault(nm, isfar ? 4 : 2);
                    } else if ((base.startswith(S("struct ")) || base.startswith(S("union "))) && old.contains(base)) {
                        out.setdefault(nm, old.at(base).size);
                    } else if (OLD_SCALARS.contains(base)) {
                        out.setdefault(nm, OLD_SCALARS.at(base));
                    }
                }
            }
        }
        _old_elems = out;
    }
    return *_old_elems;
}

// {global: its bytes in the original} of the program's arrays (far pointers 4 bytes, structures as laid out)
py::dict<str, int64_t> Port::old_var_sizes() {
    if (!_old_vars) {
        const Layouts& old = old_layouts();
        py::dict<str, int64_t> out;
        for (const str& t : originals.values()) {
            str code = blank(t);
            list<str> cl = lines_of(code);
            list<int64_t> ds = depths(cl);
            for (int64_t i = 0; i < cl.size(); ++i) {
                str st_ = cl[i].strip();
                if (ds[i] || !st_.contains(S("[")) || st_.contains(S("(")) || !st_.endswith(S(";"))) continue;
                bool isfar = has_search(R"(\b(far|huge)\b)", st_);
                auto r = declarators(
                    py::re::sub(S(R"(\b(far|near|huge)\b)"), S(""), py::re::sub(S(R"(=.*;$)"), S(";"), st_)).rstrip(S(";")));
                if (!r) continue;
                for (const auto& nd : std::get<1>(*r)) {
                    const str& nm = std::get<0>(nd);
                    const str& dcl = std::get<1>(nd);
                    if (!py::re::search(S(R"(\[\s*\d+\s*\])"), dcl)) continue;
                    std::optional<int64_t> sz =
                        size_of(py::re::sub(S(R"(\b(extern|static)\b)"), S(""), std::get<0>(*r)).strip(), dcl, nm, isfar ? 4 : 2, old);
                    if (!sz) {
                        str base = py::re::sub(S(R"(\s+)"), S(" "),
                                               py::re::sub(S(R"(\b(extern|static|register|const)\b)"), S(""), std::get<0>(*r)))
                                       .strip();
                        auto unit = OLD_SCALARS.get(base);
                        int64_t cnt = 1;
                        for (const auto& x : py::re::finditer(S(R"(\[\s*(\d+)\s*\])"), dcl)) cnt *= to_int(x.group(1));
                        if (unit && *unit && !dcl.contains(S("*"))) sz = *unit * cnt;
                        else sz = std::nullopt;
                    }
                    if (sz && *sz) out.setdefault(nm, *sz);
                }
            }
        }
        _old_vars = out;
    }
    py::dict<str, int64_t> res = _old_vars->copy();   // (and what the assembler's data placed)
    res.update(asm_sizes);
    return res;
}

// the structures as the old compiler laid them out (16-bit pointers, far ones 4 bytes)
const Layouts& Port::old_layouts() {
    if (!_old) {
        list<str> texts;
        for (const str& t : originals.values()) texts.append(t);
        int64_t wp = profile.widths_pointer ? *profile.widths_pointer : 16;
        _old = asm16::CTypes::_layouts(texts, floordiv(wp, 8), 4);
    }
    return *_old;
}

// the old layouts as a file sees them: the definitions of the headers it includes (in their order) before
// any other file's - two headers that define one tag two ways: the one it includes
const Layouts& Port::old_layouts_for(const str& fname) {
    if (!_twice_tags) {
        py::dict<str, str> seen;
        py::set<str> twice;
        for (const auto& nt : originals.items()) {
            for (const auto& m : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s*\{)"), blank(std::get<1>(nt)))) {
                str k_ = py::re::sub(S(R"(\s+)"), S(" "), m.group(1));
                if (seen.setdefault(k_, std::get<0>(nt)) != std::get<0>(nt)) twice.add(k_);
            }
        }
        _twice_tags = twice;
        _lay_cache = py::dict<str, Layouts>();
    }
    if (!_twice_tags->size()) return old_layouts();
    list<str> order;   // (its headers, depth first, in the order included)
    list<str> todo;
    todo.append(fname);
    while (todo.size()) {
        str n = todo.pop(0);
        if (order.contains(n) || !originals.contains(n)) continue;
        order.append(n);
        list<str> inc;
        for (const auto& m : py::re::finditer(S(R"x(#\s*include\s*"([^"]+)")x"), originals.at(n))) inc.append(m.group(1));
        todo = inc + todo;
    }
    str key = py::join(S("\n"), order);
    if (!_lay_cache.contains(key)) {
        list<str> texts;
        for (int64_t i = order.size() - 1; i >= 0; --i) texts.append(originals.at(order[i]));
        for (const auto& nt : originals.items())
            if (!order.contains(std::get<0>(nt))) texts.append(std::get<1>(nt));
        int64_t wp = profile.widths_pointer ? *profile.widths_pointer : 16;
        _lay_cache.set(key, asm16::CTypes::_layouts(texts, floordiv(wp, 8), 4));
    }
    return _lay_cache.at(key);
}

// the loop's rules: names that meet the C library's renamed, declarations given their real types, lines mended
str Port::finish(const str& fname, str text) {
    if (renames.size()) {
        std::vector<str> rn = renames.keys();
        std::sort(rn.begin(), rn.end());
        list<str> esc;
        for (const str& r : rn) esc.append(E(r));
        py::re::Pattern rx(py::percent(S(R"(\b(%s)\b)"), py::join(S("|"), esc)), 0);
        const str orig = text;
        text = sub_code(orig, rx, [&](const py::re::Match& m) {
            str pre = orig.slice(orig.rfind(S("\n"), 0, m.start()) + 1, m.start());
            if (!py::re::match(S(R"([ \t]*#\s*include\b)"), pre)) return renames.at(m.group(1));
            return m.group(0);   // (#include <setjmp.h>: the header's own name)
        });
    }
    for (const DeclRule& rn : decl_rules) text = apply_decl_rule(rn, text);
    text = apply_local_retypes(text, fname, local_retypes);
    if (array_sizes.size()) {   // (extern T a[]; - its size, where it is defined)
        std::vector<str> ks = array_sizes.keys();
        std::sort(ks.begin(), ks.end());
        list<str> esc;
        for (const str& r : ks) esc.append(E(r));
        text = py::re::sub(py::percent(S(R"((?m)^(\s*extern\b[^;(){}]*?\b(%s))\s*\[\s*\])"), py::join(S("|"), esc)),
                           [&](const py::re::Match& m) {
                               return py::percent(S("%s[%d]"), m.group(1), array_sizes.at(m.group(2)));
                           },
                           text);
    }
    list<str> out;
    py::dict<str, int64_t> seen;   // (a rule for the n-th line of that text: two alike lines in one file are two places)
    py::dict<int64_t, std::tuple<str, int64_t>> origin;
    int64_t i = 0;
    for (const str& l : lines_of(text)) {
        int64_t k = seen.get(l, 0);
        seen.set(l, k + 1);
        str fixed = line_fixes.get({fname, l, -1}, l);
        out.append(line_fixes.get({fname, l, k}, fixed));
        origin.set(i + 1, {l, k});
        ++i;
    }
    line_origin.set(fname, origin);
    text = join_nl(out);
    for (const NpRule& rule_ : np_rules) {   // (p[0]->op, p[0] a small number at times, or l->op, l a NULL: read through
        if (rule_.file != fname) continue;   //  the null segment there, as on DOS - on the lines as mended: their rules
        const str& var_ = rule_.var;         //  are by the text before)
        const str& typ_ = rule_.typ;
        auto wrap = [&](const py::re::Match& m) { return py::percent(S("((%s)dos_np(%s))"), typ_, m.group(1)); };
        if (rule_.kind && *rule_.kind == S("var")) {
            text = sub_code(text, py::re::Pattern(py::percent(S(R"((?<![\w.>])(%s)(?=\s*->))"), E(var_)), 0), wrap);
        } else if (rule_.kind && *rule_.kind == S("assign")) {   // ((q = f06df2(p))->kind, NULL at the list's end)
            text = sub_code(text, py::re::Pattern(py::percent(S(R"(\(\s*(%s\s*=\s*\w+\s*\([^()]*\))\s*\)(?=\s*->))"), E(var_)), 0), wrap);
        } else if (rule_.kind && *rule_.kind == S("mem")) {   // (p->next->prev = ..., the last's next NULL: its link read through the null segment)
            text = sub_code(text,
                            py::re::Pattern(py::percent(S(R"((?<![\w.>])((?:\(\s*\([^()]*\)\s*dos_np\(\s*\w+\s*\)\s*\)|\w+)\s*->\s*%s)(?=\s*->))"),
                                                        E(var_)), 0),
                            wrap);
        } else {
            text = sub_code(text, py::re::Pattern(py::percent(S(R"((?<![\w.>])(%s\s*\[[^\[\]]*\])(?=\s*->))"), E(var_)), 0), wrap);
        }
    }
    return text;
}

// #include of the old library's headers -> port.h; the program's own macros for what the C library has
// (putc...) dropped
str Port::_preprocessor(const str& text, const str& fname) {
    py::set<str> lib = py::set<str>::from(profile.libc);
    list<str> out;
    int64_t i = 0;
    for (const str& l : lines_of(text)) {
        ++i;
        auto m = py::re::match(S(R"(\s*#\s*include\s*["<]([^">]+)[">])"), l);
        if (m && (left_out.contains(m->group(1)) || profile.runtime_headers.contains(m->group(1).lower()))) {
            // (port.h first of all, below: the program's own headers, before the library's, need its types)
            notes[S("#include of the old library's header -> port.h")].append(py::percent(S("%s:%d"), fname, i));
            continue;
        }
        m = py::re::match(S(R"(\s*#\s*(?:undef|define)\s+(\w+))"), l);
        if (m && (lib.contains(m->group(1)) || m->group(1) == S("FILE"))) {
            notes[S("macro of the old library's dropped (the C library of today's stands in)")].append(
                py::percent(S("%s:%d"), fname, i));
            continue;
        }
        out.append(l);
    }
    if (fname.endswith(S(".c")) && !out.contains(S("#include \"port.h\""))) out.insert(0, S("#include \"port.h\""));
    return join_nl(out);
}

str Port::_definitions(const str& text, const str& fname) {
    list<str> lines = lines_of(text);
    list<str> code = lines_of(blank(text));
    list<Definition> defs = definitions(lines, code);
    const str ARGC = ARGCAST;
    for (int64_t di = defs.size() - 1; di >= 0; --di) {
        const FnP& f = std::get<0>(defs[di]);
        int64_t s = std::get<1>(defs[di]), j = std::get<2>(defs[di]), k = std::get<3>(defs[di]);
        FnP g = fns.contains(f->name) ? fns.at(f->name) : f;
        g->params = f->params.copy();
        g->static_ = f->static_;
        g->implicit = f->implicit;
        g->ret = ((g->keep_int || g->ret_set) && g->ret.size()) ? g->ret : f->ret;
        for (const auto& pt : param_types.items()) {   // (what the loop found the parameters to be)
            const str& fn_ = std::get<0>(std::get<0>(pt));
            int64_t pk = std::get<1>(std::get<0>(pt));
            if (fn_ == f->name && pk < g->params.size()) g->params[pk] = {std::get<0>(g->params[pk]), std::get<1>(pt)};
        }
        str body = join_nl(code.slice(j, k + 1));
        std::optional<std::tuple<str, str>> last;
        if (g->params.size() >= 2) last = g->params[-1];
        std::optional<py::re::Match> walk;
        if (last)
            walk = py::re::search(py::percent(S(R"(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?&\s*%s\s*;)"), E(std::get<0>(*last))), body);
        if (walk && py::re::search(S(R"(\bchar\s*\*)"), std::get<1>(g->params[-2])) &&
            py::re::search(py::percent(S(R"(\b(?:int|short|int16_t|unsigned)\s*\*\s*%s\s*;)"), E(walk->group(1))), body) &&
            !py::re::search(py::percent(S(R"([(,]\s*%s&\s*%s\s*[,)])"), ARGC, E(std::get<0>(*last))), body)) {
            // (a function that walks its own arguments from the place of the first, by a format of its own -
            //  "PNTPN": ap = &args; *ap; ap++ - each argument a word (two a long) then: here a slot each, read
            //  from the variable arguments; its callers give each as an intptr_t)
            str ap_ = walk->group(1);
            str second = std::get<0>(g->params[-2]);
            str lastname = std::get<0>(*last);
            g->varargs = true;
            g->va_slots = true;
            g->loose = false;
            g->params = g->params.slice(0, g->params.size() - 1);
            for (int64_t x = j; x <= k; ++x) {
                str l_ = lines[x];
                l_ = py::re::sub(py::percent(S(R"(\b%s\s*=\s*(?:\([^()]*\)\s*)?&\s*%s\s*;)"), E(ap_), E(lastname)),
                                 py::percent(S("%s = dos_args_;"), ap_), l_);
                l_ = py::re::sub(py::percent(S(R"(\b(?:int|short|int16_t|unsigned)(\s*\*\s*%s\s*;))"), E(ap_)), S(R"(intptr_t\1)"), l_);
                l_ = py::re::sub(py::percent(S(R"(\*\s*\(\s*long\s*\*\s*\)\s*%s\b)"), E(ap_)), py::percent(S("((int32_t)*%s)"), ap_), l_);
                l_ = py::re::sub(py::percent(S(R"(\b%s\s*\+=\s*2\s*;)"), E(ap_)), py::percent(S("%s += 1;"), ap_), l_);
                lines[x] = l_;
            }
            lines[j] = lines[j].rstrip() + py::percent(S(" DOS_VA_ARGS(dos_args_, %s);"), second);
            notes[S("function walking its own arguments from the place of the first (by a format of its own): the "
                    "variable arguments, a slot each (DOS_VA_ARGS)")]
                .append(py::percent(S("%s:%d (%s)"), fname, s + 1, f->name));
            body = join_nl(lines_of(blank(join_nl(lines))).slice(j, k + 1));
            last = std::nullopt;
        }
        if (last && py::re::search(py::percent(S(R"([(,]\s*%s&\s*%s\s*[,)])"), ARGC, E(std::get<0>(*last))), body) &&
            py::re::fullmatch(py::percent(S(R"((?:register\s+)?(?:int|short|unsigned|int16_t|intptr_t)\s+%s)"), E(std::get<0>(*last))),
                              std::get<1>(*last).strip()) &&
            py::re::search(S(R"(\bchar\s*\*)"), std::get<1>(g->params[-2]))) {
            // (the old way of a function of any number of arguments: the place of the first of them given on
            //  - they followed it on the stack, a word each; here they are put in words of their own)
            str second = std::get<0>(g->params[-2]);
            str lastname = std::get<0>(*last);
            g->varargs = true;
            g->loose = false;
            g->params = g->params.slice(0, g->params.size() - 1);
            for (int64_t x = j; x <= k; ++x) {
                str rx_ = py::percent(S(R"(([(,]\s*%s)&\s*%s\b(?=\s*[,)]))"), ARGC, E(lastname));   // (an argument only)
                if (py::re::search(rx_, code[x]))
                    lines[x] = py::re::sub(rx_, [&](const py::re::Match& mm) { return mm.group(1) + lastname; }, lines[x]);
            }
            lines[j] = lines[j].rstrip() + py::percent(S(" DOS_VA_WORDS(%s, %s);"), lastname, second);
            notes[S("function of any number of arguments the old way (the place of the first given on): ..., its "
                    "arguments put in words (DOS_VA_WORDS, by its format)")]
                .append(py::percent(S("%s:%d (%s)"), fname, s + 1, f->name));
            body = join_nl(lines_of(blank(join_nl(lines))).slice(j, k + 1));
        }
        g->body = body;
        bool valued = py::re::search(S(R"(\breturn\b\s*[^\s;])"), body).has_value();
        list<str> bare = py::re::findall(S(R"(\breturn\s*;)"), body);
        if (f->name == S("main")) g->ret = g->ret.size() ? g->ret : S("int");
        g->void_ = !valued && !g->ret.size() && f->name != S("main") && !g->keep_int;
        auto ret_or_int = [&]() { return (g->ret.size() ? g->ret : S("int")).strip(); };
        if (!valued && !bare.size() && (g->ret.size() || g->keep_int) && ret_or_int() != S("void") &&
            ret_or_int() != S("static void") && f->name != S("main")) {
            // (a function that gives a value, falling off its end: what the register held - the value of its
            // last assignment of a call's, when nothing after it gives the register another)
            std::vector<int64_t> stmts;
            for (int64_t x = j + 1; x < k; ++x) if (code[x].strip().size()) stmts.push_back(x);
            std::optional<std::tuple<int64_t, str>> last2;
            std::optional<py::re::Match> tail;
            if (!stmts.empty()) tail = py::re::fullmatch(S(R"((\w+)\s*\([^;]*\)\s*;)"), code[stmts.back()].strip());
            FnP callee;
            if (tail && fns.contains(tail->group(1))) callee = fns.at(tail->group(1));
            if (callee && (callee->ret.size() ? callee->ret : S("int")).strip() != S("") &&
                (callee->ret.size() ? callee->ret : S("int")).strip() != S("void") && !callee->void_ &&
                depths(code)[stmts.back()] == depths(code)[j] + 1) {
                int64_t x = stmts.back();   // (its last statement a call of one that gives a value: what that gave was in the register - so given)
                str ind = py::re::match(S(R"(\s*)"), lines[x])->group(0);
                lines[x] = ind + S("return ") + lines[x].lstrip();
                notes[S("function giving a value that fell off its end after a call (the value in the register): the "
                        "call's value returned")]
                    .append(py::percent(S("%s:%d (%s)"), fname, x + 1, f->name));
                stmts.clear();
            }
            for (auto it = stmts.rbegin(); it != stmts.rend(); ++it) {
                int64_t x = *it;
                str s_ = code[x].strip();
                auto a_ = py::re::fullmatch(S(R"((\w+)\s*=\s*\w+\s*\([^;]*\)\s*;)"), s_);
                if (a_) {
                    last2 = std::make_tuple(x, a_->group(1));
                    break;
                }
                if (!py::re::fullmatch(S(R"((?:\+\+|--)?\s*[\w.\->\[\]]+\s*(?:\+\+|--)?\s*;|[\w.\->\[\]]+\s*=\s*)"
                                         R"((?:\d+|0x[0-9a-fA-F]+)\s*;)"),
                                       s_))
                    break;
            }
            if (last2 && py::re::search(py::percent(S(R"(\b%s\b)"), std::get<1>(*last2)), join_nl(code.slice(j, std::get<0>(*last2))))) {
                str ind = py::re::match(S(R"(\s*)"), lines[std::get<0>(*last2)])->group(0);
                lines[k] = ind + py::percent(S("return %s;\t\t/* (what the register held: the old function fell off its end) */\n"),
                                             std::get<1>(*last2)) +
                           lines[k];
                notes[S("function giving a value that fell off its end (the value in the register): its last value returned")]
                    .append(py::percent(S("%s:%d (%s)"), fname, k + 1, f->name));
            }
        }
        if (valued && ret_or_int() != S("void") && ret_or_int() != S("static void") && f->name != S("main")) {
            // (a function that gives a value but whose last statement, an if-else, has a branch falling off
            // its end after x = f(...): what the register held then was that value - given)
            int64_t dep = 0;
            std::vector<int64_t> at_;
            for (int64_t x = j; x <= k; ++x) {
                at_.push_back(dep);
                dep += code[x].count(S("{")) - code[x].count(S("}"));
            }
            int64_t D = at_.size() > 1 ? at_[1] : 1;   // (the body's depth)
            std::vector<int64_t> body_;
            for (int64_t x = j + 1; x < k; ++x) if (code[x].strip().size()) body_.push_back(x);
            std::vector<int64_t> closes;
            for (int64_t x : body_) {
                if (code[x].strip().startswith(S("}")) && at_[x - j] == D + 1 &&
                    py::re::fullmatch(S(R"(\}\s*(?:else\b.*)?)"), code[x].strip()))
                    closes.push_back(x);
            }
            str last_at_d;
            bool any_at_d = false;
            for (int64_t x : body_) if (at_[x - j] == D) { last_at_d = code[x]; any_at_d = true; }
            bool any_else = false;
            for (int64_t c_ : closes) if (py::re::search(S(R"(\belse\b)"), code[c_])) any_else = true;
            if (!closes.empty() && closes.back() == body_.back() &&
                py::re::match(S(R"(\s*(?:if|else)\b)"), any_at_d ? last_at_d : S("")) && any_else) {
                for (int64_t c_ : closes) {
                    std::optional<int64_t> prev;
                    for (int64_t x : body_) if (x < c_ && code[x].strip().size()) prev = prev ? std::max(*prev, x) : x;
                    if (!prev || at_[*prev - j] != D + 1) continue;
                    str s_ = code[*prev].strip();
                    if (py::re::fullmatch(S(R"([\w.\->\[\]]+\s*=\s*\w+\s*\([^;]*\)\s*;)"), s_) && !s_.startswith(S("return"))) {
                        str ind = py::re::match(S(R"(\s*)"), lines[*prev])->group(0);
                        lines[*prev] = ind + S("return ") + lines[*prev].lstrip();
                        notes[S("function giving a value whose branch fell off its end after an assignment of a call "
                                "(the value in the register): that value returned")]
                            .append(py::percent(S("%s:%d (%s)"), fname, *prev + 1, f->name));
                    }
                }
            }
        }
        if (bare.size() && (valued || !g->void_) && ret_or_int() != S("void")) {   // (or an int to its callers)
            g->bare = bare.size();
            for (int64_t x = j; x <= k; ++x) {
                if (py::re::search(S(R"(\breturn\s*;)"), code[x])) {
                    lines[x] = py::re::sub(S(R"(\breturn\s*;)"), S("return 0;"), lines[x]);
                    notes[S("bare return in a function that gives a value elsewhere: return 0 (what the register held "
                            "is not known)")]
                        .append(py::percent(S("%s:%d"), fname, x + 1));
                }
            }
        }
        if (g->loose) {   // (the old-style definition kept - its calls unchecked: what it gives as its callers say - an int
            if (!g->ret.size()) g->ret = g->void_ ? S("void") : S("int");   //  when it never said - and its integers)
            if (g->ret.size()) {
                str nmrx = py::percent(S(R"(\b%s\s*\()"), E(g->name));
                auto nm = py::re::search(nmrx, lines[s]);
                if (nm) {
                    lines[s] = (g->static_ ? S("static ") : S("")) + g->ret + (g->ret.endswith(S("*")) ? S("") : S(" ")) +
                               lines[s].slice(nm->start(), lines[s].size());
                } else if (s + 1 < lines.size() && py::re::search(nmrx, lines[s + 1])) {
                    auto nm2 = py::re::search(nmrx, lines[s + 1]);
                    lines[s] = S("");   // (the type on the line before: the head on one line)
                    lines[s + 1] = (g->static_ ? S("static ") : S("")) + g->ret + (g->ret.endswith(S("*")) ? S("") : S(" ")) +
                                   lines[s + 1].slice(nm2->start(), lines[s + 1].size());
                }
            }
            for (int64_t x = s + 1; x < j; ++x) {
                if (lines[x].strip().endswith(S(";")))
                    lines[x] = py::re::sub(S(R"(\b(?:unsigned\s+int|unsigned(?!\s+(?:char|long|short))|short|int)\b)"
                                             R"((?!\s*\*))"),
                                           S("intptr_t"), lines[x]);
            }
            list<str> undecl;
            for (const auto& pd : g->params) if (g->implicit.contains(std::get<0>(pd))) undecl.append(std::get<0>(pd));
            if (undecl.size() && !lines[j].lstrip().startswith(S("intptr_t")))   // (never declared: old C's int - here intptr_t)
                lines[j] = py::percent(S("intptr_t %s; %s"), py::join(S(", "), undecl), lines[j].lstrip());
            notes[S("old-style definition kept: its integer parameters intptr_t (the callers may give pointers)")]
                .append(py::percent(S("%s:%d (%s)"), fname, s + 1, g->name));
            continue;
        }
        notes[S("old-style definition -> prototype")].append(py::percent(S("%s:%d"), fname, s + 1));
        list<str> one;
        one.append(g->proto());
        lines.set_slice(s, j, one);
    }
    list<FnP> statics;
    for (const auto& d : defs) if (std::get<0>(d)->static_) statics.append(fns.at(std::get<0>(d)->name));
    if (statics.size()) {   // (the file's own functions: declared at its head)
        int64_t mx = -1, i = 0;
        for (const str& l : lines) {
            if (l.startswith(S("#include"))) mx = std::max(mx, i);
            ++i;
        }
        int64_t at = mx + 1;
        list<str> protos;
        for (const FnP& g : statics) protos.append(g->proto() + S(";"));
        lines.set_slice(at, at, protos);
    }
    return join_nl(lines);
}

// port.h, proto.h and data.c
py::dict<str, std::optional<str>> Port::headers() {
    py::set<str> tagset;
    for (const FnP& f : fns.values()) {
        for (const auto& pd : f->params)
            for (const str& t : py::re::findall(S(R"(\b(?:struct|union)\s+\w+)"), std::get<1>(pd))) tagset.add(t);
        for (const str& t : py::re::findall(S(R"(\b(?:struct|union)\s+\w+)"), f->ret)) tagset.add(t);
    }
    list<str> tags = sorted_strs(tagset);
    py::set<str> whole;   // (T (*a)[3]: an array of T - T must be complete)
    for (const FnP& f : fns.values()) {
        for (const auto& pd : f->params) {
            for (const auto& x : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s*\(\s*\*\s*\w+\s*\)\s*\[)"),
                                                  array_param(std::get<1>(pd), std::get<0>(pd))))
                whole.add(py::re::sub(S(R"(\s+)"), S(" "), x.group(1)));
        }
    }
    list<str> incs;
    for (const str& t : sorted_strs(whole)) {
        std::optional<str> h;
        for (const auto& hv : files.items()) {
            if (std::get<0>(hv).endswith(S(".h")) && py::re::search(py::percent(S(R"(\b%s\s*\{)"), tagrx(t)), blank(std::get<1>(hv)))) {
                h = std::get<0>(hv);
                break;
            }
        }
        if (h && !incs.contains(*h)) incs.append(*h);
    }
    list<str> proto;
    proto.append(S("/* proto.h -- every function of the program, declared (made by CodeBreaker's port) */"));
    proto.append(S("#ifndef PROTO_H"));
    proto.append(S("#define PROTO_H"));
    for (const str& h : incs) proto.append(py::percent(S("#include \"%s\""), h));
    for (const str& t : tags) proto.append(t + S(";"));
    proto.append(S(""));
    {
        std::vector<FnP> fv = fns.values();
        std::stable_sort(fv.begin(), fv.end(), [](const FnP& a, const FnP& b) {
            str fa = a->file ? *a->file : S(""), fb = b->file ? *b->file : S("");
            if (fa != fb) return fa < fb;
            return a->line < b->line;
        });
        for (const FnP& f : fv)
            if (!f->static_) proto.append(py::re::sub(S(R"(\bregister\s+)"), S(""), f->proto()) + S(";"));
    }
    if (asm_protos.size()) {   // (the program's own assembly routines, called old-style: their arguments pushed as they were)
        proto.append(S(""));
        proto.append(S("/* the program's routines in assembly language (their *64.asm) */"));
        std::vector<std::tuple<str, str>> ap = asm_protos.items();
        std::sort(ap.begin(), ap.end());
        for (const auto& kv : ap) proto.append(std::get<1>(kv));
    }
    proto.append(S(""));
    proto.append(S("#endif"));
    str ptext = join_nl(proto);
    {
        Notes tmp;
        ptext = std::get<0>(widths(ptext, blank(ptext), tmp, S("proto.h"), int16));
    }
    list<str> port;
    for (const char* l : {"/* port.h -- what every file of the port includes first (made by CodeBreaker's port) */",
                          "#ifndef PORT_H", "#define PORT_H", "#include <stdio.h>", "#include <stdlib.h>",
                          "#include <string.h>", "#include <stdint.h>", "#include <ctype.h>", "#include <signal.h>",
                          "#include <errno.h>", "#include <time.h>"})
        port.append(S(l));
    port.append(py::percent(S("#include \"%s\""), profile.platform_header));
    if (profile.struct_align && *profile.struct_align) {   // (the program's structures laid out as its compiler did - the system's headers above)
        port.append(py::percent(S("/* the program's structures laid out as its compiler did: members on %d-byte boundaries at most */"),
                                *profile.struct_align));
        port.append(py::percent(S("#pragma pack(push, %d)"), *profile.struct_align));
    }
    for (const str& h : sorted_strs(view_headers)) port.append(py::percent(S("#include \"%s\""), h));   // (a structure some names are members of: known here)
    if (aliases.size()) {   // (variables the old linker gave one place: the names of its elements are the elements)
        py::dict<str, str> owners;
        std::vector<std::tuple<str, std::tuple<str, str>>> al_items = aliases.items();
        std::sort(al_items.begin(), al_items.end());
        for (const auto& kv : al_items) {
            const str& where = std::get<0>(std::get<1>(kv));
            owners.setdefault(py::re::search(S(R"([A-Za-z_]\w*)"), where)->group(0), std::get<1>(std::get<1>(kv)));
        }
        port.append(S(""));
        port.append(S("/* the names the old linker gave the places of an array's elements */"));
        std::vector<std::tuple<str, str>> ow = owners.items();
        std::sort(ow.begin(), ow.end());
        for (const auto& o : ow) {
            const str& od = std::get<1>(o);
            if (!od.size()) continue;
            Notes tmp;
            str ext = S("extern ") + od;
            port.append(std::get<0>(widths(ext, blank(ext), tmp, S("port.h"), int16)));
        }
        std::vector<std::tuple<str, std::tuple<str, str>>> byw = aliases.items();   // (sorted by where, in the dict's order for ties)
        std::stable_sort(byw.begin(), byw.end(),
                         [](const auto& a, const auto& b) { return std::get<0>(std::get<1>(a)) < std::get<0>(std::get<1>(b)); });
        for (const auto& kv : byw)
            port.append(py::percent(S("#define %s\t%s"), std::get<0>(kv), std::get<0>(std::get<1>(kv))));
    }
    port.append(S("#include \"proto.h\""));
    port.append(S("#endif"));

    // (the tags a header defines, itself and through its includes)
    std::function<py::set<str>(const str&, py::set<str>&)> tags_of = [&](const str& h, py::set<str>& seen) -> py::set<str> {
        if (seen.contains(h) || !files.contains(h)) return py::set<str>();
        seen.add(h);
        const str& t = files.at(h);
        py::set<str> out_;
        for (const auto& m : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s*\{)"), blank(t))) out_.add(m.group(1));
        for (const auto& m : py::re::finditer(S(R"x(#\s*include\s*"([^"]+)")x"), t)) out_ = out_ | tags_of(m.group(1), seen);
        py::set<str> norm;
        for (const str& x : out_) norm.add(py::re::sub(S(R"(\s+)"), S(" "), x));
        return norm;
    };
    auto tags_of1 = [&](const str& h) {
        py::set<str> seen;
        return tags_of(h, seen);
    };
    list<list<str>> groups;   // (headers that define one tag two ways: not in one file - a file each such way)
    {
        py::set<str> hs;
        for (const auto& hd : data) if (std::get<0>(hd).endswith(S(".h"))) hs.add(std::get<0>(hd));
        for (const str& h : sorted_strs(hs)) {
            std::optional<int64_t> found_g;
            for (int64_t gi = 0; gi < groups.size(); ++gi) {
                py::set<str> un, v2;
                for (const str& x : groups[gi]) un = un | tags_of1(x);
                for (const str& x : groups[gi]) {
                    py::set<str> part;
                    py::set<str> th = tags_of1(h);
                    for (const str& t_ : tags_of1(x)) if (th.contains(t_) && _same_def(t_, x, h)) part.add(t_);
                    v2 = v2 | part;
                }
                py::set<str> clash = (tags_of1(h) & un) - v2;
                if (!clash.size()) {
                    found_g = gi;
                    break;
                }
            }
            if (!found_g) {
                list<str> ng;
                ng.append(h);
                groups.append(ng);
            } else {
                groups[*found_g].append(h);
            }
        }
    }
    if (asm_data.size() || !groups.size()) {
        py::set<str> hs;
        for (const auto& hd : data) if (std::get<0>(hd).endswith(S(".h"))) hs.add(std::get<0>(hd));
        if (asm_data.size()) for (const str& h : files.keys()) if (h.endswith(S(".h"))) hs.add(h);
        groups = list<list<str>>();
        groups.append(sorted_strs(hs));
    }
    list<std::tuple<str, list<str>>> datas;
    py::set<str> all_group;
    for (const auto& g_ : groups) for (const str& h : g_) all_group.add(h);
    for (int64_t k_ = 0; k_ < groups.size(); ++k_) {
        const list<str>& g_ = groups[k_];
        str name = k_ == 0 ? S("data.c") : py::percent(S("data%d.c"), k_ + 1);
        list<str> ls;
        ls.append(py::percent(S("/* %s -- the variables the headers share, defined once (made by CodeBreaker's port) */"), name));
        ls.append(S("#include \"port.h\""));
        for (const str& h : g_) ls.append(py::percent(S("#include \"%s\""), h));
        ls.append(S(""));
        for (const auto& hd : data) {
            if (g_.contains(std::get<0>(hd)) || (k_ == 0 && !all_group.contains(std::get<0>(hd)))) ls.append(std::get<1>(hd));
        }
        if (asm_data.size() && k_ == 0) {
            ls.append(S(""));
            ls.append(S("/* the variables an assembler module of data alone placed */"));
            for (const auto& hd : asm_data) ls.append(std::get<1>(hd));
        }
        datas.append({name, ls});
    }
    list<str> bss_ = bss_definitions();

    // (the header that defines a structure - of two that do, the one of its name)
    auto defining = [&](const str& u) -> std::optional<str> {
        list<str> hs_;
        for (const auto& hv : files.items())
            if (std::get<0>(hv).endswith(S(".h")) && py::re::search(py::percent(S(R"(\b%s\s*\{)"), tagrx(u)), blank(std::get<1>(hv))))
                hs_.append(std::get<0>(hv));
        list<str> uw = py::split(u);
        for (const str& h : hs_) {
            // os.path.splitext(h)[0]
            int64_t dot = h.rfind(S("."));
            str stem = h;
            if (dot > 0 && h.slice(0, dot).lstrip(S(".")).size()) stem = h.slice(0, dot);
            if (stem == uw[-1]) return h;
        }
        if (hs_.size()) return hs_[0];
        return std::nullopt;
    };
    list<str> bdata;
    py::set<str> seen_;
    std::function<void(const str&, py::set<str>&)> add = [&](const str& h, py::set<str>& seen) {   // (and first what its members' structures need: by value - a complete type)
        if (seen.contains(h)) return;
        seen.add(h);
        str body = blank(files.at(h));
        for (const auto& um : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s+\w+\s*(?:\[[^\]]*\])?\s*;)"), body)) {
            auto d_ = defining(py::re::sub(S(R"(\s+)"), S(" "), um.group(1)));
            if (d_ && *d_ != h) add(*d_, seen);
        }
        bdata.append(py::percent(S("#include \"%s\""), h));
    };
    bdata.append(S("/* bss.c -- the variables the old build defined beginning without a value (its BSS list), as the "
                   "program declares them (made by CodeBreaker's port) */"));
    bdata.append(S("#include \"port.h\""));
    {
        py::set<str> us;
        for (const auto& um : py::re::finditer(S(R"(\b((?:struct|union)\s+\w+)\s+\w+\s*[\[;])"), join_nl(bss_))) us.add(um.group(1));
        for (const str& u : sorted_strs(us)) {
            auto h = defining(py::re::sub(S(R"(\s+)"), S(" "), u));   // (a structure of its own, not a pointer: its header)
            if (h) add(*h, seen_);
        }
    }
    list<str> bl = bdata;
    bl.append(S(""));
    bl.extend(bss_);
    str btext = join_nl(bl) + S("\n");
    {
        Notes tmp;
        btext = std::get<0>(widths(btext, blank(btext), tmp, S("bss.c"), int16));
    }
    py::dict<str, std::optional<str>> res;
    res.set(S("port.h"), finish(S("port.h"), join_nl(port) + S("\n")));
    res.set(S("proto.h"), finish(S("proto.h"), ptext + S("\n")));
    if (bss_.size()) res.set(S("bss.c"), finish(S("bss.c"), btext));
    else res.set(S("bss.c"), std::nullopt);
    for (const auto& nd : datas) {
        str dtext = join_nl(std::get<1>(nd)) + S("\n");
        Notes tmp;
        dtext = std::get<0>(widths(dtext, blank(dtext), tmp, std::get<0>(nd), int16));
        if (data.size() || asm_data.size()) res.set(std::get<0>(nd), finish(std::get<0>(nd), dtext));
        else res.set(std::get<0>(nd), std::nullopt);
    }
    return res;
}

// whether two headers define a tag alike (one header including the other: the same definition)
bool Port::_same_def(const str& tag, const str& h1, const str& h2) {
    std::function<std::optional<str>(const str&, py::set<str>&)> where = [&](const str& h, py::set<str>& seen) -> std::optional<str> {
        if (seen.contains(h) || !files.contains(h)) return std::nullopt;
        seen.add(h);
        if (py::re::search(py::percent(S(R"(\b%s\s*\{)"), tagrx(tag)), blank(files.at(h)))) return h;
        for (const auto& m : py::re::finditer(S(R"x(#\s*include\s*"([^"]+)")x"), files.at(h))) {
            auto w_ = where(m.group(1), seen);
            if (w_) return w_;
        }
        return std::nullopt;
    };
    py::set<str> s1, s2;
    auto w1 = where(h1, s1);
    auto w2 = where(h2, s2);
    return w1 == w2;
}

}  // namespace codebreaker::port::pre_ansi
