// codebreaker/port/asmdata.cpp -- typed C++ port of codebreaker.port.asmdata (see asmdata.hpp).
//
// The data of an assembler module, as C (part of the port; see port/asm16 for its code).
//
// A module of data alone is the old linker's business: its bytes were placed, and the names in them were offsets.
// Here they are C - each table given the C program's types where it declares them (and where a pointer of a
// declared one points: that type too), its words that held an offset a pointer now, initialized as the bytes were:
//
//   image(text)        the module's segments: their bytes, which words were the offset of a name (relocations), the
//                      labels' places, each segment's class
//   messages(text)     a table of numbered messages for the C library's __NMSG_TEXT / __FMSG_TEXT (segments of class
//                      MSG / FAR_MSG: a word, the number; the text; a 0), as C: the table and the two lookups
//   typed(text, ct)    the tables of a data module with pointers in them, as C definitions with initializers

#include "codebreaker/port/asmdata.hpp"

#include <algorithm>
#include <functional>
#include <vector>

namespace codebreaker::port::asmdata {

using py::dict;
using py::list;
using py::set;
using py::str;

namespace {

str S(const char* s) { return str(s); }
// the Python's "%..." % args
template <class... A> str F(const char* f, const A&... a) { return py::percent(str(f), a...); }
// re.sub(r"\s+", " ", x)
str ws1(const str& x) { return py::re::sub(S(R"(\s+)"), S(" "), x); }
// x.replace(a, b, 1)
str replace1(const str& x, const str& a, const str& b) {
    int64_t at = x.find(a);
    if (at < 0) return x;
    return x.slice(0, at) + b + x.slice(at + a.size(), x.size());
}
// the bytes of b[at:at+n] (clamped, as a slice), read little-endian as a number: int.from_bytes(.., "little")
int64_t word_of(const py::bytearray& data, int64_t at, int64_t n) {
    const std::string& raw = data.raw();
    int64_t size = static_cast<int64_t>(raw.size());
    int64_t a = std::min(at, size), b = std::min(at + n, size);
    uint64_t v = 0;
    for (int64_t i = b - 1; i >= a; i--) {
        if (i - a >= 8 && raw[static_cast<size_t>(i)] != 0) throw py::OverflowError("Python int too large to convert to C long");
        if (i - a < 8) v = (v << 8) | static_cast<unsigned char>(raw[static_cast<size_t>(i)]);
    }
    return static_cast<int64_t>(v);
}
// a number cut to its unit's bytes, little-endian
std::string le_bytes(int64_t v, int64_t unit) {
    std::string out;
    uint64_t u = static_cast<uint64_t>(v);
    for (int64_t i = 0; i < unit; i++) out.push_back(static_cast<char>((u >> (8 * i)) & 0xFF));
    return out;
}

}  // namespace

// ---------------------------------------------------------------- the image

// a DW/DD item: (number, None) or (displacement, symbol)
std::tuple<std::optional<int64_t>, std::optional<str>> _value(const str& v0, const dict<str, str>& equ) {
    str v = py::re::sub(S(R"((?i)^offset\s+(?:\w+:)?)"), S(""), v0.strip());
    std::optional<int64_t> n = asm16::number(v);
    if (n) return {*n, std::nullopt};
    if (equ.contains(v) && asm16::number(equ.at(v))) return {*asm16::number(equ.at(v)), std::nullopt};
    auto m = py::re::fullmatch(S(R"(([A-Za-z_$@?][\w$@?]*)\s*(?:([+-])\s*(\w+))?)"), v);
    if (m) {
        std::optional<str> sign = m->group_opt(2), num = m->group_opt(3);
        std::optional<int64_t> d = 0;
        if (num) d = asm16::number(*num);
        if (sign && *sign == S("-")) {
            if (!d) throw py::TypeError("bad operand type for unary -: 'NoneType'");   // (-None)
            d = -*d;
        }
        return {d, m->group(1)};
    }
    throw asm16::Asm16Error(F("a data item not understood: %r", v));
}

// a DB item: its bytes
py::bytes _bytes(const str& item0) {
    str item = item0.strip();
    if (item.size() >= 2 && item[0] == item[-1] && S("'\"").contains(item[0])) {
        str q = item[0];
        return item.slice(1, item.size() - 1).replace(q + q, q).encode("latin1");
    }
    std::optional<int64_t> n = asm16::number(item);
    if (!n) throw asm16::Asm16Error(F("a byte not understood: %r", item));
    return py::bytes(std::string(1, static_cast<char>(*n & 0xFF)));
}

// {segment name: Seg} of a module's data
dict<str, SegP> image(const str& text) {
    dict<str, SegP> segs;
    SegP cur;
    dict<str, str> equ;
    for (const asm16::LineP& l : asm16::parse(text)) {
        const std::optional<asm16::Directive>& d = l->directive;
        if (l->label && cur) cur->labels[*l->label] = cur->data.size();
        if (!d) continue;
        const str& kind = d->kind;
        str rest = asm16::strip_comment(d->rest).strip();
        if (kind == "SEGMENT") {
            const str& name = *d->name;
            auto m = py::re::search(S(R"('([^']*)')"), rest);
            if (!segs.contains(name)) segs.set(name, std::make_shared<Seg>(name, m ? std::optional<str>(m->group(1)) : std::nullopt));
            cur = segs.at(name);
            if (m && !(cur->cls && !cur->cls->empty())) cur->cls = m->group(1);
            continue;
        }
        if (kind == "ENDS") {
            cur = nullptr;
            continue;
        }
        if (kind == "EQU" || kind == "=") {
            equ[*d->name] = rest;
            continue;
        }
        if (!cur) continue;
        if (kind == "LABEL") {
            cur->labels[*d->name] = cur->data.size();
            continue;
        }
        if (kind == "DB" || kind == "DW" || kind == "DD") {
            if (d->name && !d->name->empty()) cur->labels[*d->name] = cur->data.size();
            int64_t unit = kind == "DB" ? 1 : kind == "DW" ? 2 : 4;
            auto m = py::re::match(S(R"((?i)\s*(\w+)\s+DUP\s*\((.*)\)\s*$)"), rest);
            if (m) {
                std::optional<int64_t> n = asm16::number(m->group(1));
                if (!n) throw py::TypeError("can't multiply sequence by non-int of type 'NoneType'");
                str v = m->group(2).strip();
                std::optional<int64_t> fn = v == "?" ? std::optional<int64_t>(0) : asm16::number(v);
                int64_t fill = fn ? *fn : 0;
                int64_t at = cur->data.size();
                std::string unitb = le_bytes(fill, unit), rep;
                for (int64_t k = 0; k < *n; k++) rep += unitb;
                cur->data.raw_mut() += rep;
                cur->items.append({at, unit * (*n > 0 ? *n : 0)});
                continue;
            }
            for (const str& it : asm16::split_ops(rest)) {
                int64_t at = cur->data.size();
                if (unit == 1) {
                    cur->data += _bytes(it);
                } else {
                    auto [v, sym] = _value(it, equ);
                    if (sym) {
                        cur->reloc[at] = {*sym, v};
                        v = 0;
                    }
                    if (!v) throw py::TypeError("unsupported operand type(s) for &: 'NoneType' and 'int'");
                    cur->data.raw_mut() += le_bytes(*v, unit);
                }
                cur->items.append({at, static_cast<int64_t>(cur->data.size()) - at});
            }
        }
    }
    return segs;
}

// bytes as a C string literal (its 0 left to C)
str c_string(const py::bytes& b) {
    std::string out;
    for (unsigned char c : b.raw()) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else if (32 <= c && c < 127) out.push_back(static_cast<char>(c));
        else {
            char buf[8];
            snprintf(buf, sizeof buf, "\\%03o", c);
            out += buf;
        }
    }
    str s = py::re::sub(S(R"((\\[0-7]{3})(?=[0-7]))"), S(R"(\1"")"), str(out));
    return S("\"") + s + S("\"");
}

const dict<str, int64_t> BASE_SIZES = {
    {S("char"), 1}, {S("signed char"), 1}, {S("unsigned char"), 1}, {S("short"), 2}, {S("int"), 2}, {S("unsigned"), 2},
    {S("unsigned int"), 2}, {S("unsigned short"), 2}, {S("int16_t"), 2}, {S("uint16_t"), 2}, {S("long"), 4},
    {S("unsigned long"), 4}, {S("int32_t"), 4}, {S("uint32_t"), 4}, {S("int8_t"), 1}, {S("uint8_t"), 1}};

set<str> publics(const str& text) {
    set<str> out;
    for (const asm16::LineP& l : asm16::parse(text)) {
        if (l->directive && l->directive->kind == "PUBLIC") {
            for (const str& x : py::split(l->directive->rest, S(","))) {
                str t = x.strip();
                if (!t.empty()) out.add(t);
            }
        }
    }
    return out;
}

list<str> typed_notes;

namespace {

// the work of typed(): the Python's nested functions share its variables, here its members do
struct Typed {
    const asm16::CTypes& ct;
    set<str> pubs;
    dict<str, std::tuple<SegP, int64_t>> where;
    dict<str, str> types;                                   // (label: the type of its elements)
    std::vector<str> queue;
    list<str>& notes;

    Typed(const asm16::CTypes& ct_, list<str>& notes_) : ct(ct_), notes(notes_) {}

    str cname(const str& n) const { return (n.startswith(S("_")) && pubs.contains(n)) ? n.slice(1, n.size()) : n; }

    // the size of a type as the old compiler had it, or None
    std::optional<int64_t> osize(const str& t0) const {
        str t = ws1(t0.strip());
        if (t.endswith(S("*"))) return 2;
        if (t.startswith(S("struct ")) || t.startswith(S("union "))) {
            const asm16::Layout* lay = ct.old.get(t);
            if (lay) return lay->size;
            return std::nullopt;
        }
        str k = py::re::sub(S(R"(^(signed|const) )"), S(""), t);
        if (BASE_SIZES.contains(k)) return BASE_SIZES.at(k);
        return std::nullopt;
    }

    void assign(const str& sym, const str& t0, const str& why) {
        str t = ws1(t0.strip());
        if (!where.contains(sym)) {
            notes.append(F("%s: not in the module (%s)", sym, why));
            return;
        }
        if (types.contains(sym)) {
            if (types.at(sym) != t) notes.append(F("%s: %s and %s (%s)", sym, types.at(sym), t, why));
            return;
        }
        types[sym] = t;
        queue.push_back(sym);
    }

    // (segment, offset, the end of what the label holds)
    std::tuple<SegP, int64_t, int64_t> extent(const str& sym) const {
        auto [s, o] = where.at(sym);
        int64_t nxt = s->data.size();
        for (const auto& kv : s->labels.items()) {
            int64_t x = std::get<1>(kv);
            if (x > o) nxt = std::min(nxt, x);
        }
        return {s, o, nxt};
    }

    // what the element of type t at s+at points to: typed
    void walk(const str& t, const SegP& s, int64_t at) {
        if (t.endswith(S("*"))) {
            auto r = s->reloc.get(at);
            if (r) assign(std::get<0>(*r), t.slice(0, t.size() - 1), S("pointed to"));
            return;
        }
        const asm16::Layout* lay = ct.old.get(t);
        if (!lay) return;
        for (const auto& kv : lay->members) {
            const str& name = kv.first;
            const asm16::Member& v = kv.second;
            if (v.isptr) {
                int64_t stars = replace1(v.decl, name, S("")).count(S("*"));
                auto r = s->reloc.get(at + v.off);
                if (r) assign(std::get<0>(*r), v.type + (stars > 1 ? S(" ") + str(std::string(static_cast<size_t>(stars - 1), '*')) : S("")), F("%s.%s", t, name));
            }
        }
    }

    static int64_t sign(int64_t v, int64_t bytes) {         // (v - (1 << 8n) when v has the top bit of n bytes)
        if (bytes >= 1 && bytes <= 4 && v >= (int64_t(1) << (8 * bytes - 1))) return v - (int64_t(1) << (8 * bytes));
        return v;
    }

    str ref(const std::tuple<str, std::optional<int64_t>>& r, const str& t) const {
        const str& sym = std::get<0>(r);
        if (!types.contains(sym)) return S("");             // (None: the caller tells by an empty text)
        if (!std::get<1>(r)) throw py::TypeError("unsupported operand type(s) for %: 'NoneType' and 'int'");
        int64_t disp = *std::get<1>(r);
        const str& u = types.at(sym);
        std::optional<int64_t> us0 = osize(u);
        int64_t us = (us0 && *us0) ? *us0 : 1;
        str name = cname(sym);
        if (py::mod(disp, us) == 0) return disp ? F("&%s[%d]", name, py::floordiv(disp, us)) : F("&%s[0]", name);
        return F("(%s)((char *)%s + %d)", t, name, disp);    // (into the middle of an element)
    }

    str value(const str& t, const SegP& s, int64_t at) {
        if (t.endswith(S("*"))) {
            auto r = s->reloc.get(at);
            if (r) {
                str v = ref(*r, t);
                if (!v.empty()) return v;
                notes.append(F("%s+%d: %s, not typed", s->name, at, std::get<0>(*r)));
                return S("0");
            }
            int64_t n = word_of(s->data, at, 2);
            return n == 0 ? S("0") : F("(%s)%d", t, n);
        }
        const asm16::Layout* lay = ct.old.get(t);
        if (lay) {
            list<str> vals;
            for (const auto& kv : lay->members) {
                const str& name = kv.first;
                const asm16::Member& v = kv.second;
                int64_t fo = v.off, fs = v.size;
                if (v.isptr) {
                    int64_t stars = replace1(v.decl, name, S("")).count(S("*"));
                    vals.append(value(v.type + S(" ") + str(std::string(static_cast<size_t>(stars), '*')), s, at + fo));
                } else if (v.decl.contains(S("["))) {
                    std::optional<int64_t> n0 = osize(v.type);
                    int64_t n = (n0 && *n0) ? *n0 : 1;
                    list<str> ws;
                    for (int64_t j = 0; j < py::floordiv(fs, n); j++) ws.append(str(std::to_string(word_of(s->data, at + fo + j * n, n))));
                    vals.append(F("{ %s }", py::join(S(", "), ws)));
                } else {
                    int64_t n = word_of(s->data, at + fo, fs);
                    if ((fs == 1 || fs == 2 || fs == 4) && !v.type.startswith(S("unsigned"))) n = sign(n, fs);
                    vals.append(str(std::to_string(n)));
                }
            }
            return F("{ %s }", py::join(S(", "), vals));
        }
        std::optional<int64_t> n0 = osize(t);
        int64_t n = (n0 && *n0) ? *n0 : 1;
        int64_t v = word_of(s->data, at, n);
        if ((n == 1 || n == 2 || n == 4) && !t.startswith(S("unsigned"))) v = sign(v, n);
        return str(std::to_string(v));
    }
};

}  // namespace

// the C of a data module whose words hold addresses (a table of chains, of names): each table typed - by the
// C program's declaration of its name, or by what points to it - and initialized as its bytes were. None when
// the module has no addresses in it
std::optional<str> typed(const str& text, const asm16::CTypes& ct) {
    dict<str, SegP> segs = image(text);
    bool any = false;
    for (const auto& kv : segs.items())
        if (!std::get<1>(kv)->reloc.empty()) any = true;
    if (!any) return std::nullopt;
    list<str> notes;
    Typed T(ct, notes);
    for (const auto& kv : segs.items()) {
        const SegP& s = std::get<1>(kv);
        for (const auto& lk : s->labels.items()) T.where[std::get<0>(lk)] = {s, std::get<1>(lk)};
    }
    T.pubs = publics(text);
    std::vector<str> sorted_pubs;
    for (const str& p : T.pubs) sorted_pubs.push_back(p);
    std::sort(sorted_pubs.begin(), sorted_pubs.end());
    for (const str& p : sorted_pubs) {
        auto v = ct.var(T.cname(p));
        if (v && T.where.contains(p)) {
            int64_t stars = replace1(py::re::sub(S(R"(\[[^\]]*\])"), S(""), std::get<1>(*v)), T.cname(p), S("")).count(S("*"));
            T.assign(p, std::get<0>(*v).strip() + (stars ? S(" ") + str(std::string(static_cast<size_t>(stars), '*')) : S("")), S("declared"));
        }
    }

    while (!T.queue.empty()) {
        str sym = T.queue.back();
        T.queue.pop_back();
        str t = T.types.at(sym);
        std::optional<int64_t> size = T.osize(t);
        if (!size || *size == 0) {
            notes.append(F("%s: no size for %s", sym, t));
            continue;
        }
        auto [s, o, end] = T.extent(sym);
        for (int64_t k = 0; k < std::max<int64_t>(1, py::floordiv(end - o, *size)); k++) T.walk(t, s, o + k * *size);
    }

    std::vector<str> order;
    for (const auto& kv : T.types.items()) order.push_back(std::get<0>(kv));
    std::stable_sort(order.begin(), order.end(), [&](const str& a, const str& b) {
        const auto& wa = T.where.at(a);
        const auto& wb = T.where.at(b);
        if (std::get<0>(wa)->name != std::get<0>(wb)->name) return std::get<0>(wa)->name < std::get<0>(wb)->name;
        return std::get<1>(wa) < std::get<1>(wb);
    });
    list<str> decls, defs;
    for (const str& sym : order) {
        str t = T.types.at(sym);
        auto [s, o, end] = T.extent(sym);
        std::optional<int64_t> size = T.osize(t);
        if (!size) throw py::TypeError("unsupported operand type(s) for //: 'int' and 'NoneType'");
        int64_t count = std::max<int64_t>(1, py::floordiv(end - o, *size));
        str name = T.cname(sym);
        std::optional<std::tuple<str, str>> v;
        if (T.pubs.contains(sym)) v = ct.var(name);
        bool array = !v || std::get<1>(*v).contains(S("["));
        str static_ = T.pubs.contains(sym) ? S("") : S("static ");
        str sep = t.endswith(S("*")) ? S("") : S(" ");
        str dim = array ? F("[%d]", count) : S("");
        decls.append(F("%s%s%s%s%s;", static_, t, sep, name, dim));
        if (t == "char" && array) {
            const std::string& raw = s->data.raw();
            std::string b = raw.substr(static_cast<size_t>(std::min<int64_t>(o, raw.size())),
                                       static_cast<size_t>(std::max<int64_t>(0, std::min<int64_t>(o + count, raw.size()) - std::min<int64_t>(o, raw.size()))));
            std::string text_ = (!b.empty() && b.back() == '\0') ? b.substr(0, b.size() - 1) : b;
            if (text_.find('\0') == std::string::npos) {
                defs.append(F("%s%s %s%s = %s;", static_, t, name, dim, c_string(py::bytes(text_))));
                continue;
            }
        }
        if (array) {
            list<str> items;
            for (int64_t k = 0; k < count; k++) items.append(T.value(t, s, o + k * *size));
            defs.append(F("%s%s%s%s%s = {\n\t%s\n};", static_, t, sep, name, dim, py::join(S(",\n\t"), items)));
        } else {
            defs.append(F("%s%s%s%s = %s;", static_, t, sep, name, T.value(t, s, o)));
        }
    }
    list<str> head;
    head.append(S("/* the tables of an assembler module, with the addresses in them (made by CodeBreaker's port: typed "
                  "by the C program's declarations and what points where) */"));
    for (int64_t i = 0; i < std::min<int64_t>(20, notes.size()); i++) head.append(F("/* (not typed: %s) */", notes[i]));
    typed_notes = notes;
    list<str> all = head;
    all.extend(decls);
    all.append(S(""));
    all.extend(defs);
    return py::join(S("\n"), all);
}

// the C of a module of numbered messages (segments of class MSG and FAR_MSG), or None: a table of each - the
// near ones __NMSG_TEXT's, the far ones __FMSG_TEXT's (their numbers are their own: the same number in both is
// two messages)
std::optional<str> messages(const str& text) {
    dict<str, SegP> segs = image(text);
    dict<str, list<SegP>> tables;
    tables[S("MSG")] = list<SegP>();
    tables[S("FAR_MSG")] = list<SegP>();
    for (const auto& kv : segs.items()) {
        const SegP& s = std::get<1>(kv);
        if (s->cls && tables.contains(*s->cls) && s->data.size() > 0) tables.at(*s->cls).append(s);
    }
    if (tables.at(S("MSG")).empty() && tables.at(S("FAR_MSG")).empty()) return std::nullopt;
    list<str> out;
    out.append(S("/* the numbered messages of the C library's __NMSG_TEXT (near) and __FMSG_TEXT (far) */"));
    const char* names[2][2] = {{"MSG", "dos_nmsgs"}, {"FAR_MSG", "dos_fmsgs"}};
    for (auto& cn : names) {
        out.append(F("static const struct { int16_t num; const char *text; } %s[] = {", cn[1]));
        for (const SegP& sg : tables.at(S(cn[0]))) {
            const std::string b = sg->data.raw();
            int64_t i = 0, n = static_cast<int64_t>(b.size());
            while (i + 2 <= n) {
                int64_t num = static_cast<unsigned char>(b[i]) | (static_cast<unsigned char>(b[i + 1]) << 8);
                if (num == 0xFFFF) break;
                size_t z = b.find('\0', static_cast<size_t>(i + 2));
                int64_t j = z == std::string::npos ? n : static_cast<int64_t>(z);
                out.append(F("\t{ %d, %s },", num, c_string(py::bytes(b.substr(static_cast<size_t>(i + 2), static_cast<size_t>(j - (i + 2)))))));
                i = j + 1;
            }
        }
        out.append(S("\t{ -1, 0 }"));
        out.append(S("};"));
        out.append(S(""));
    }
    const char* fns[2][2] = {{"__NMSG_TEXT", "dos_nmsgs"}, {"__FMSG_TEXT", "dos_fmsgs"}};
    for (auto& fn : fns) {
        out.append(F("char *%s(int32_t n)\t\t/* (an int: as the callers give it) */", fn[0]));
        out.append(S("{"));
        out.append(S("\tint32_t i;"));
        out.append(F("\tfor (i = 0; %s[i].text; i++)", fn[1]));
        out.append(F("\t\tif (%s[i].num == n) return (char *)%s[i].text;", fn[1], fn[1]));
        out.append(S("\treturn (char *)\"\";"));
        out.append(S("}"));
        out.append(S(""));
    }
    return py::join(S("\n"), out);
}

}  // namespace codebreaker::port::asmdata
