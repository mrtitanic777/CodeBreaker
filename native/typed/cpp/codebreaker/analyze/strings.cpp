// codebreaker/analyze/strings.cpp -- typed C++ port of codebreaker.analyze.strings.

#include "codebreaker/analyze/strings.hpp"

#include <algorithm>

#include "codebreaker/analyze/native.hpp"
#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::strings {

const py::str _SPEC = py::str(R"(%([-+ 0#]*)(\*|\d+)?(?:\.(\*|\d+))?(hh|h|ll|l|L|w|I64|I32)?([diouxXeEfgGcCsSpn%]))");

namespace {

// 0x20 <= c < 0x7F or c in (9, 10, 13)
bool printable(unsigned char c) {
    return (0x20 <= c && c < 0x7F) || c == 9 || c == 10 || c == 13;
}

// bytes.decode("latin1") of the bytes raw[from], raw[from + step], ... below `to`
py::str latin1(const std::string& raw, int64_t from, int64_t to, int64_t step) {
    std::u32string out;
    for (int64_t i = from; i < to; i += step) {
        out.push_back(static_cast<char32_t>(static_cast<unsigned char>(raw[static_cast<size_t>(i)])));
    }
    return py::str(out);
}

// the text of s after its last "!" (s.split("!")[-1])
py::str after_bang(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t at = u.rfind(U'!');
    return py::str(at == std::u32string::npos ? u : u.substr(at + 1));
}

bool has_bang(const py::str& s) {
    return s.u32().find(U'!') != std::u32string::npos;
}

// s[:n]
py::str head(const py::str& s, size_t n) {
    return py::str(s.u32().substr(0, n));
}

}  // namespace

// _ascii: precondition start >= 0 (a negative start would index from the end in the Python)
std::optional<py::str> _ascii(const py::bytes& data, int64_t start, int64_t least) {
    const std::string& raw = data.raw();
    int64_t end = start;
    int64_t n = static_cast<int64_t>(raw.size());
    while (end < n && printable(static_cast<unsigned char>(raw[static_cast<size_t>(end)]))) {
        end += 1;
    }
    if (end - start >= least && end < n && raw[static_cast<size_t>(end)] == 0) {
        return latin1(raw, start, end, 1);
    }
    return std::nullopt;
}

// _wide: precondition start >= 0
std::optional<py::str> _wide(const py::bytes& data, int64_t start, int64_t least) {
    const std::string& raw = data.raw();
    int64_t end = start;
    int64_t n = static_cast<int64_t>(raw.size());
    while (end + 1 < n && raw[static_cast<size_t>(end + 1)] == 0 &&
           printable(static_cast<unsigned char>(raw[static_cast<size_t>(end)]))) {
        end += 2;
    }
    if ((end - start) / 2 >= least && end + 1 < n && raw[static_cast<size_t>(end)] == 0 &&
        raw[static_cast<size_t>(end + 1)] == 0) {
        return latin1(raw, start, end, 2);
    }
    return std::nullopt;
}

// The string at exactly `va`: (text, "A" | "W"), or None.
std::optional<std::tuple<py::str, py::str>> at(Image& img, int64_t va, int64_t least) {
    std::optional<Span> s = img.span(va);
    if (!s || va - s->va >= s->initialised) {
        return std::nullopt;
    }
    int64_t off = va - s->va;
    const py::bytes& data = s->data;
    if (img.bits == 32) {
        std::optional<py::str> w = _wide(data, off, least);
        if (w) {
            return std::make_tuple(*w, py::str("W"));
        }
    }
    std::optional<py::str> a = _ascii(data, off, least);
    if (a && !(py::len(*a) < 4 && off != 0 && 0x20 <= static_cast<unsigned char>(data.raw()[static_cast<size_t>(off - 1)]) &&
               static_cast<unsigned char>(data.raw()[static_cast<size_t>(off - 1)]) < 0x7F)) {   // not the tail of a longer one
        return std::make_tuple(*a, py::str("A"));
    }
    return std::nullopt;
}

Table _table(Image& img) {
    Table out;
    for (const RawTableRow& r : img.ask_table()) {
        out.set(std::get<0>(r), TableEntry(std::get<1>(r), std::get<2>(r), std::get<3>(r)));
    }
    return out;
}

Table table(Image& img) {
    if (!img.memo_strings) {
        img.memo_strings = _table(img);
    }
    return *img.memo_strings;
}

Uses _uses(Image& img) {
    Table tab = table(img);
    Uses out;
    for (const RawUsesRow& row : img.ask_uses()) {
        int64_t f = std::get<0>(row);
        const py::list<Use>& rows = std::get<1>(row);
        out.set(f, rows.copy());
        if (img.bits == 16) {   // a use from 16-bit code makes the string referenced
            for (const Use& u : rows) {
                int64_t va = std::get<1>(u);
                const TableEntry& e = tab.at(va);
                tab.set(va, TableEntry(std::get<0>(e), std::get<1>(e), true));
            }
        }
    }
    return out;
}

Uses uses(Image& img) {
    if (!img.memo_uses) {
        img.memo_uses = _uses(img);
    }
    return *img.memo_uses;
}

py::list<std::tuple<py::str, int64_t, py::str, bool>> _assemble(
    const py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>>& stores, int64_t least) {
    py::list<std::tuple<py::str, int64_t, py::str, bool>> out;
    py::V rows = py::json_parse(native::assemble(stores, least).utf8());
    for (const py::V& r : rows.seq()) {
        const std::vector<py::V>& c = r.seq();
        out.append(std::make_tuple(py::str(c[0].str()), c[1].as_int(), py::str(c[2].str()),
                                   c[3].is_bool() ? c[3].b : c[3].as_int() != 0));
    }
    return out;
}

py::list<StackString> stack_strings(Image& img) {
    py::list<StackString> out;
    for (const StackString& r : img.ask_stack()) {
        out.append(r);
    }
    return out;
}

py::list<py::str> conversions(const py::str& fmt, bool wide) {
    static const py::re::Pattern spec = py::re::compile(_SPEC);
    py::list<py::str> types;
    for (const auto& m : spec.findall_groups<py::str, py::str, py::str, py::str, py::str>(fmt)) {
        const py::str& width = std::get<1>(m);
        const py::str& prec = std::get<2>(m);
        const py::str& length = std::get<3>(m);
        const py::str& conv = std::get<4>(m);
        if (conv == py::str("%")) {
            continue;
        }
        int64_t stars = (width == py::str("*") ? 1 : 0) + (prec == py::str("*") ? 1 : 0);
        for (int64_t i = 0; i < stars; i++) {
            types.append(py::str("int"));
        }
        if (conv == py::str("s") || conv == py::str("S")) {
            types.append(py::str((length == py::str("l") || length == py::str("w")) || ((conv == py::str("S")) != wide)
                                     ? "wchar_t *" : "char *"));
        } else if (conv == py::str("c") || conv == py::str("C")) {
            types.append(py::str((conv == py::str("C")) != wide ? "wchar_t" : "char"));
        } else if (conv == py::str("p")) {
            types.append(py::str("void *"));
        } else if (conv == py::str("n")) {
            types.append(py::str("int *"));
        } else if (conv == py::str("e") || conv == py::str("E") || conv == py::str("f") || conv == py::str("g") ||
                   conv == py::str("G")) {
            types.append(py::str("double"));
        } else {
            bool is_signed = conv == py::str("d") || conv == py::str("i");
            if (length == py::str("ll") || length == py::str("I64")) {
                types.append(py::str(is_signed ? "__int64" : "unsigned __int64"));
            } else if (length == py::str("l")) {
                types.append(py::str(is_signed ? "long" : "unsigned long"));
            } else if (length == py::str("h")) {
                types.append(py::str(is_signed ? "short" : "unsigned short"));
            } else {
                types.append(py::str(is_signed ? "int" : "unsigned"));
            }
        }
    }
    return types;
}

std::optional<std::tuple<int64_t, py::str, py::str>> literal(Image& img, const py::list<Arg>& args, int64_t k) {
    Table tab = table(img);
    auto entry = [&](int64_t va) {   // (va,) + tab[va][:2]
        const TableEntry& e = tab.at(va);
        return std::make_tuple(va, std::get<0>(e), std::get<1>(e));
    };
    if (img.bits == 32) {
        if (k < py::len(args) && args[k].kind == py::str("i")) {
            // (args[k][1] in tab: a register name is never an address in the table)
            const int64_t* v = std::get_if<int64_t>(&args[k].value);
            if (v && tab.contains(*v)) {
                return entry(*v);
            }
        }
        return std::nullopt;
    }
    // 16-bit: a far pointer is two words, offset pushed last; the format is the k-th far pointer
    int64_t lo = 2 * k, hi = 2 * k + 1;
    if (hi < py::len(args) && args[lo].kind == py::str("i")) {
        const Arg& seg = args[hi];
        std::optional<int64_t> base;
        const py::str* name = std::get_if<py::str>(&seg.value);
        if (seg.kind == py::str("r") && name && *name == py::str("ds")) {
            base = img.data_segment;
        } else if (seg.kind == py::str("i")) {
            if (name) {
                throw py::TypeError("unsupported operand type(s) for <<: 'str' and 'int'");
            }
            int64_t shifted = std::get<int64_t>(seg.value) * 65536;   // seg[1] << 16
            if (img.span(shifted)) {
                base = shifted;
            }
        }
        if (base) {
            const int64_t* off = std::get_if<int64_t>(&args[lo].value);
            if (!off) {
                throw py::TypeError("unsupported operand type(s) for +: 'int' and 'str'");
            }
            int64_t va = *base + *off;
            if (tab.contains(va)) {
                return entry(va);
            }
        }
    }
    return std::nullopt;
}

Formats _formats(Image& img) {
    table(img);
    RawFormats got = img.ask_formats();
    py::list<FormatFact> out;
    for (const RawFormatRow& r : got.rows) {
        FormatFact fact;
        fact.addr = std::get<0>(r);
        fact.site = std::get<1>(r);
        fact.function = std::get<2>(r);
        fact.format = std::get<3>(r);
        if (std::get<5>(r)) {
            fact.arguments = conversions(std::get<3>(r), std::get<4>(r));
        }
        out.append(fact);
    }
    py::dict<int64_t, int64_t> own;
    for (const auto& kv : got.own) {
        own.set(std::get<0>(kv), std::get<1>(kv));
    }
    return Formats(out, got.sites, own);
}

Formats formats(Image& img) {
    if (!img.memo_formats) {
        img.memo_formats = _formats(img);
    }
    return *img.memo_formats;
}

Analysis analyse(Image& img) {
    Table tab = table(img);
    Uses use = uses(img);
    py::list<StackString> stack = stack_strings(img);
    Formats fm = formats(img);
    const py::list<FormatFact>& fmts = std::get<0>(fm);
    int64_t sites = std::get<1>(fm);
    const py::dict<int64_t, int64_t>& own = std::get<2>(fm);

    py::list<Fact> facts;
    auto tab_items = tab.items();
    std::sort(tab_items.begin(), tab_items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto& kv : tab_items) {
        const TableEntry& e = std::get<1>(kv);
        StringFact s;
        s.at = std::get<0>(kv);
        s.text = std::get<0>(e);
        s.wide = std::get<1>(e) == py::str("W");
        s.referenced = std::get<2>(e);
        facts.append(s);
    }
    auto use_items = use.items();
    std::sort(use_items.begin(), use_items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto& kv : use_items) {
        std::vector<py::str> texts;
        for (const Use& u : std::get<1>(kv)) {
            texts.push_back(std::get<2>(u));
        }
        std::sort(texts.begin(), texts.end());
        texts.erase(std::unique(texts.begin(), texts.end()), texts.end());   // sorted({...})
        StringUsesFact s;
        s.addr = std::get<0>(kv);
        for (const py::str& t : texts) {
            s.strings.append(t);
        }
        facts.append(s);
    }
    for (const StackString& r : stack) {
        StackStringFact s;
        s.addr = std::get<0>(r);
        s.where = std::get<1>(r);
        s.text = std::get<2>(r);
        s.wide = std::get<3>(r);
        facts.append(s);
    }
    for (const FormatFact& f : fmts) {
        facts.append(f);
    }
    auto own_items = own.items();
    std::sort(own_items.begin(), own_items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto& kv : own_items) {
        PrintfLikeFact p;
        p.addr = std::get<0>(kv);
        p.format_argument = std::get<1>(kv);
        facts.append(p);
    }

    Counts counts;
    counts.strings = tab.size();
    for (const auto& kv : tab.items()) {
        if (std::get<2>(std::get<1>(kv))) counts.referenced += 1;
        if (std::get<1>(std::get<1>(kv)) == py::str("W")) counts.wide += 1;
    }
    counts.functions_using_strings = use.size();
    for (const auto& kv : use.items()) {
        counts.string_uses += py::len(std::get<1>(kv));
    }
    counts.stack_strings = py::len(stack);
    counts.formatter_call_sites = sites;
    for (const FormatFact& f : fmts) {
        if (has_bang(f.function)) counts.format_strings_read += 1;
        else counts.format_strings_at_own_functions += 1;
        if (f.arguments) counts.arguments_typed_by_formats += py::len(*f.arguments);
    }
    counts.own_printf_like_functions = own.size();

    py::list<py::str> text;
    int64_t shown = 0;
    for (const StackString& r : stack) {
        if (shown++ >= 8) break;
        text.append(py::str("stack string in ") + img.label(std::get<0>(r)) + py::str(" at ") + std::get<1>(r) +
                    py::str(": ") + py::repr(std::get<2>(r)));
    }
    shown = 0;
    for (const FormatFact& f : fmts) {
        if (shown++ >= 10) break;
        py::str args;
        if (f.arguments && !f.arguments->empty()) {
            bool first = true;
            for (const py::str& a : *f.arguments) {
                args = args + py::str(first ? "" : ", ") + a;
                first = false;
            }
        } else {
            args = py::str("(argument list elsewhere)");   // f["arguments"] or ["(argument list elsewhere)"]
        }
        text.append(after_bang(f.function) + py::str("(") + py::repr(head(f.format, 50)) + py::str(") in ") +
                    img.label(f.addr) + py::str(" -> ") + args);
    }
    shown = 0;
    for (const auto& kv : own_items) {
        if (shown++ >= 8) break;
        text.append(py::str("printf-like: ") + img.label(std::get<0>(kv)) + py::str(", format is argument ") +
                    py::to_str(std::get<1>(kv)));
    }

    Analysis a;
    a.counts = counts;
    a.facts = facts;
    a.text = text;
    return a;
}

}  // namespace codebreaker::analyze::strings
