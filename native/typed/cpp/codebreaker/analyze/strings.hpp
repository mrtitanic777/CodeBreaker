// codebreaker/analyze/strings.hpp -- COMPAT SHIM over cb/analyze/strings.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/analyze/strings.py port. Deleted with its last py:: consumer (analyze/globals, image_bridge,
// source/module_writer).
//
// A module's strings: where they are, who uses them, and what they say. The engine -- the table, the uses, the strings
// built on the stack, the format strings and the report -- is native/cb/analyze/strings.hpp. Here:
//   * the readers and the pure functions forward to it (_ascii, _wide, at, conversions, _assemble, analyse);
//   * the old callers hold the module as an `analyze::strings::Image` the DLL bridge answers questions about
//     (analyze/native.hpp), so _table/_uses/stack_strings/_formats still turn the DLL's rows into the old shapes.
//     They go when analyze/image is refactored and the engine is called on a cb::analyze::Image directly.
//
//     strings.table(img)            {address: (text, "A" | "W", referenced)}
//     strings.at(img, va)           (text, "A" | "W") of the string at an address, or None
//     strings.uses(img)             {function: [(site, address, text)]}
//     strings.stack_strings(img)    [(function, where, text)]  strings assembled by immediate stores
//     strings.formats(img)          [call sites of printf-like functions with their format string
//                                    and the argument types it implies]
//
// Typing notes (unchanged from the port):
//   * The class Image below is the part of the old image this module uses: bits, data_segment, span(), label(), and
//     the four answers of the C++ analysis engine, already read into typed rows. img.memo(key, make) is the three memo_*.
//   * A call argument is a tuple ("i" | "r", int | str) in the Python; Arg holds a std::variant<int64_t, py::str>.
//   * The dicts of analyse() are records (for_each_field gives their keys in order); the facts are a std::variant.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/analyze/strings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "py.hpp"

namespace codebreaker::analyze::strings {

namespace ns = ::cb::analyze::strings;

// _SPEC: the pattern of a printf conversion -- the text of the Python's re.compile(...) (the engine scans for it by hand)
inline const py::str _SPEC = py::str(R"(%([-+ 0#]*)(\*|\d+)?(?:\.(\*|\d+))?(hh|h|ll|l|L|w|I64|I32)?([diouxXeEfgGcCsSpn%]))");

// (va, data, executable, writable, name, initialised length) -- an image span
struct Span {
    int64_t va = 0;
    py::bytes data;
    bool executable = false;
    bool writable = false;
    py::str name;
    int64_t initialised = 0;
};

// {address: (text, "A" | "W", referenced)}
using TableEntry = std::tuple<py::str, py::str, bool>;
using Table = py::dict<int64_t, TableEntry>;
// (site, address, text)
using Use = std::tuple<int64_t, int64_t, py::str>;
using Uses = py::dict<int64_t, py::list<Use>>;
// (function, "ebp-0x20", text, wide)
using StackString = std::tuple<int64_t, py::str, py::str, bool>;
// the argument of a call: ("i", address or immediate) | ("r", register name)
struct Arg {
    py::str kind;
    std::variant<int64_t, py::str> value;
};

// ---- what the engine answers (the rows of `_ask`) ---------------------------------------------------
// ["table"]: [address, text, kind, referenced]
using RawTableRow = std::tuple<int64_t, py::str, py::str, bool>;
// ["uses"]: [function, [[site, address, text]]]
using RawUsesRow = std::tuple<int64_t, py::list<Use>>;
// ["formats"]: rows [function, site, api, text, wide, listed], the number of sites, own [function, argument]
using RawFormatRow = std::tuple<int64_t, int64_t, py::str, py::str, bool, bool>;
struct RawFormats {
    py::list<RawFormatRow> rows;
    int64_t sites = 0;
    py::list<std::tuple<int64_t, int64_t>> own;
};

// A format call site: {addr, kind, site, function, format, arguments}
struct FormatFact {
    int64_t addr = 0;
    py::str kind = "format";
    int64_t site = 0;
    py::str function;
    py::str format;
    std::optional<py::list<py::str>> arguments;   // None: the argument list is elsewhere
};
// (the list of facts, the number of call sites, {a printf-like function: the position of its format})
using Formats = std::tuple<py::list<FormatFact>, int64_t, py::dict<int64_t, int64_t>>;

// What this module asks of the image.
class Image {
public:
    int64_t bits = 32;           // img.bits
    int64_t data_segment = 0;    // img.data_segment (16-bit: the automatic data segment's base)
    virtual ~Image() = default;

    // img.span(va): the span holding va, or None
    virtual std::optional<Span> span(int64_t va) = 0;
    // img.label(va): the function, the import, or the address itself
    virtual py::str label(int64_t va) = 0;
    // _ask(img, "table") etc.: h = img._handle(); h.names(img); h.ask("strings." + what)
    virtual py::list<RawTableRow> ask_table() = 0;
    virtual py::list<RawUsesRow> ask_uses() = 0;
    virtual py::list<StackString> ask_stack() = 0;
    virtual RawFormats ask_formats() = 0;

    // img.memo(key, make): what is worked out once per image
    std::optional<Table> memo_strings;
    std::optional<Uses> memo_uses;
    std::optional<Formats> memo_formats;
};

// ---- analyse() -----------------------------------------------------------------------------------
struct Counts {
    int64_t strings = 0;
    int64_t referenced = 0;
    int64_t wide = 0;
    int64_t functions_using_strings = 0;
    int64_t string_uses = 0;
    int64_t stack_strings = 0;
    int64_t formatter_call_sites = 0;
    int64_t format_strings_read = 0;
    int64_t own_printf_like_functions = 0;
    int64_t format_strings_at_own_functions = 0;
    int64_t arguments_typed_by_formats = 0;
};
struct StringFact {            // kind "string"; addr is None
    std::optional<int64_t> addr;
    py::str kind = "string";
    int64_t at = 0;
    py::str text;
    bool wide = false;
    bool referenced = false;
};
struct StringUsesFact {        // kind "string_uses"
    int64_t addr = 0;
    py::str kind = "string_uses";
    py::list<py::str> strings;   // sorted, distinct
};
struct StackStringFact {       // kind "stack_string"
    int64_t addr = 0;
    py::str kind = "stack_string";
    py::str where;
    py::str text;
    bool wide = false;
};
struct PrintfLikeFact {        // kind "printf_like"
    int64_t addr = 0;
    py::str kind = "printf_like";
    int64_t format_argument = 0;
};
using Fact = std::variant<StringFact, StringUsesFact, StackStringFact, FormatFact, PrintfLikeFact>;
struct Analysis {
    Counts counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

template <class F> void for_each_field(const FormatFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("site", r.site); f("function", r.function); f("format", r.format);
    f("arguments", r.arguments);
}
template <class F> void for_each_field(const StringFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("at", r.at); f("text", r.text); f("wide", r.wide); f("referenced", r.referenced);
}
template <class F> void for_each_field(const StringUsesFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("strings", r.strings);
}
template <class F> void for_each_field(const StackStringFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("where", r.where); f("text", r.text); f("wide", r.wide);
}
template <class F> void for_each_field(const PrintfLikeFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("format_argument", r.format_argument);
}
template <class F> void for_each_field(const Counts& r, F&& f) {
    f("strings", r.strings); f("referenced", r.referenced); f("wide", r.wide);
    f("functions_using_strings", r.functions_using_strings); f("string_uses", r.string_uses);
    f("stack_strings", r.stack_strings); f("formatter_call_sites", r.formatter_call_sites);
    f("format_strings_read", r.format_strings_read); f("own_printf_like_functions", r.own_printf_like_functions);
    f("format_strings_at_own_functions", r.format_strings_at_own_functions);
    f("arguments_typed_by_formats", r.arguments_typed_by_formats);
}
template <class F> void for_each_field(const Analysis& r, F&& f) {
    f("counts", r.counts); f("facts", r.facts); f("text", r.text);
}

// ---- the functions --------------------------------------------------------------------------------
// The ASCII string at data[start:], or None (a run of printable characters, at least `least` long, then 0).
inline std::optional<py::str> _ascii(const py::bytes& data, int64_t start, int64_t least) {
    std::optional<std::string> s = ns::read_ascii(::cb::as_bytes(data.raw()), start, least);
    if (!s) return std::nullopt;
    return py::str(*s);
}
// The 16-bit-character string at data[start:], or None.
inline std::optional<py::str> _wide(const py::bytes& data, int64_t start, int64_t least) {
    std::optional<std::string> s = ns::read_wide(::cb::as_bytes(data.raw()), start, least);
    if (!s) return std::nullopt;
    return py::str(*s);
}

// The string at exactly `va`: (text, "A" | "W"), or None.
inline std::optional<std::tuple<py::str, py::str>> at(Image& img, int64_t va, int64_t least = 2) {
    std::optional<Span> s = img.span(va);
    if (!s) return std::nullopt;
    std::optional<ns::Found> f = ns::string_in(::cb::as_bytes(s->data.raw()), va - s->va, s->initialised, static_cast<int>(img.bits),
                                               static_cast<int>(least));
    if (!f) return std::nullopt;
    return std::make_tuple(py::str(f->text), py::str(f->encoding == ns::Encoding::Wide ? "W" : "A"));
}

inline Table _table(Image& img) {
    Table out;
    for (const RawTableRow& r : img.ask_table()) out.set(std::get<0>(r), TableEntry(std::get<1>(r), std::get<2>(r), std::get<3>(r)));
    return out;
}

inline Table table(Image& img) {
    if (!img.memo_strings) img.memo_strings = _table(img);
    return *img.memo_strings;
}

inline Uses _uses(Image& img) {
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

inline Uses uses(Image& img) {
    if (!img.memo_uses) img.memo_uses = _uses(img);
    return *img.memo_uses;
}

// Printable runs in {(base, offset): (byte, store number)} -> [(base, offset, text, wide)].
inline py::list<std::tuple<py::str, int64_t, py::str, bool>> _assemble(
    const py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>>& stores, int64_t least = 4) {
    ns::Stores in;
    for (const auto& kv : stores.items()) {
        const auto& bo = std::get<0>(kv);
        const auto& vn = std::get<1>(kv);
        in[{std::to_string(std::get<0>(bo)), std::get<1>(bo)}] = ns::Store{static_cast<std::uint8_t>(std::get<0>(vn)), static_cast<int>(std::get<1>(vn))};
    }
    py::list<std::tuple<py::str, int64_t, py::str, bool>> out;
    for (const ns::Piece& p : ns::assemble(in, static_cast<std::size_t>(least)))
        out.append(std::make_tuple(py::str(p.base), p.offset, py::str(p.text), p.wide));
    return out;
}

// [(function, "ebp-0x20", text, wide)] -- strings built by storing immediates into the frame.
inline py::list<StackString> stack_strings(Image& img) {
    py::list<StackString> out;
    for (const StackString& r : img.ask_stack()) out.append(r);
    return out;
}

// The C types of the arguments a format string consumes, in order.
inline py::list<py::str> conversions(const py::str& fmt, bool wide = false) {
    py::list<py::str> out;
    for (const std::string& t : ns::conversions(fmt.utf8(), wide)) out.append(py::str(t));
    return out;
}

// The string a call's k-th argument points at: (address, text, kind), or None.
inline std::optional<std::tuple<int64_t, py::str, py::str>> literal(Image& img, const py::list<Arg>& args, int64_t k) {
    Table tab = table(img);
    auto entry = [&](int64_t va) {   // (va,) + tab[va][:2]
        const TableEntry& e = tab.at(va);
        return std::make_tuple(va, std::get<0>(e), std::get<1>(e));
    };
    if (img.bits == 32) {
        if (k < py::len(args) && args[k].kind == py::str("i")) {
            const int64_t* v = std::get_if<int64_t>(&args[k].value);   // (a register name is never an address in the table)
            if (v && tab.contains(*v)) return entry(*v);
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
            if (name) throw py::TypeError("unsupported operand type(s) for <<: 'str' and 'int'");
            int64_t shifted = std::get<int64_t>(seg.value) * 65536;   // seg[1] << 16
            if (img.span(shifted)) base = shifted;
        }
        if (base) {
            const int64_t* off = std::get_if<int64_t>(&args[lo].value);
            if (!off) throw py::TypeError("unsupported operand type(s) for +: 'int' and 'str'");
            int64_t va = *base + *off;
            if (tab.contains(va)) return entry(va);
        }
    }
    return std::nullopt;
}

inline Formats _formats(Image& img) {
    table(img);
    RawFormats got = img.ask_formats();
    py::list<FormatFact> out;
    for (const RawFormatRow& r : got.rows) {
        FormatFact fact;
        fact.addr = std::get<0>(r);
        fact.site = std::get<1>(r);
        fact.function = std::get<2>(r);
        fact.format = std::get<3>(r);
        if (std::get<5>(r)) fact.arguments = conversions(std::get<3>(r), std::get<4>(r));
        out.append(fact);
    }
    py::dict<int64_t, int64_t> own;
    for (const auto& kv : got.own) own.set(std::get<0>(kv), std::get<1>(kv));
    return Formats(out, got.sites, own);
}

inline Formats formats(Image& img) {
    if (!img.memo_formats) img.memo_formats = _formats(img);
    return *img.memo_formats;
}

// The report: the old rows as cb's parts, the new code's make_report, and its answer as the old Analysis.
inline Analysis analyse(Image& img) {
    Table tab = table(img);
    Uses use = uses(img);
    py::list<StackString> stack = stack_strings(img);
    Formats fm = formats(img);

    ns::Table table_in;
    for (const auto& kv : tab.items()) {
        const TableEntry& e = std::get<1>(kv);
        table_in[std::get<0>(kv)] = ns::Entry{std::get<0>(e).utf8(), std::get<1>(e) == py::str("W") ? ns::Encoding::Wide : ns::Encoding::Ascii, std::get<2>(e)};
    }
    std::vector<ns::FunctionUses> uses_in;
    for (const auto& kv : use.items()) {
        ns::FunctionUses f;
        f.function = std::get<0>(kv);
        for (const Use& u : std::get<1>(kv)) f.uses.push_back({std::get<0>(u), std::get<1>(u), std::get<2>(u).utf8()});
        uses_in.push_back(std::move(f));
    }
    std::vector<ns::StackString> stack_in;
    for (const StackString& r : stack)
        stack_in.push_back({std::get<0>(r), std::get<1>(r).utf8(), std::get<2>(r).utf8(), std::get<3>(r)});
    std::vector<ns::FormatFact> formats_in;
    for (const FormatFact& f : std::get<0>(fm)) {
        ns::FormatFact g{f.addr, f.site, f.function.utf8(), f.function.u32().find(U'!') != std::u32string::npos, f.format.utf8(), std::nullopt};
        if (f.arguments) {
            std::vector<std::string> args;
            for (const py::str& a : f.arguments->vec()) args.push_back(a.utf8());
            g.arguments = std::move(args);
        }
        formats_in.push_back(std::move(g));
    }
    std::map<::cb::analyze::Va, int> own_in;
    for (const auto& kv : std::get<2>(fm).items()) own_in[std::get<0>(kv)] = static_cast<int>(std::get<1>(kv));
    const ns::Report r = ns::make_report(table_in, uses_in, stack_in, formats_in, own_in, std::get<1>(fm),
                                         [&img](::cb::analyze::Va va) { return img.label(va).utf8(); });

    Analysis out;
    const ns::Counts& c = r.counts;
    out.counts = Counts{c.strings, c.referenced, c.wide, c.functions_using_strings, c.string_uses, c.stack_strings,
                        c.formatter_call_sites, c.format_strings_read, c.own_printf_like_functions,
                        c.format_strings_at_own_functions, c.arguments_typed_by_formats};
    for (const ns::StringFact& f : r.strings) {
        StringFact s;
        s.at = f.at;
        s.text = py::str(f.text);
        s.wide = f.wide;
        s.referenced = f.referenced;
        out.facts.append(s);
    }
    for (const ns::StringUsesFact& f : r.string_uses) {
        StringUsesFact s;
        s.addr = f.function;
        for (const std::string& t : f.strings) s.strings.append(py::str(t));
        out.facts.append(s);
    }
    for (const ns::StackString& f : r.stack_strings) {
        StackStringFact s;
        s.addr = f.function;
        s.where = py::str(f.where);
        s.text = py::str(f.text);
        s.wide = f.wide;
        out.facts.append(s);
    }
    for (const ns::FormatFact& f : r.formats) {
        FormatFact s;
        s.addr = f.function;
        s.site = f.site;
        s.function = py::str(f.api);
        s.format = py::str(f.format);
        if (f.arguments) {
            py::list<py::str> args;
            for (const std::string& a : *f.arguments) args.append(py::str(a));
            s.arguments = args;
        }
        out.facts.append(s);
    }
    for (const ns::PrintfLikeFact& f : r.printf_like) {
        PrintfLikeFact s;
        s.addr = f.function;
        s.format_argument = f.format_argument;
        out.facts.append(s);
    }
    out.text = ::cb::compat::to_py(r.text);
    return out;
}

}  // namespace codebreaker::analyze::strings
