// codebreaker/analyze/strings.hpp -- typed C++ port of codebreaker.analyze.strings.
//
// strings.py -- a module's strings: where they are, who uses them, and what they say.
//
//     from codebreaker.analyze import image, strings
//     img = image.load("comctl32.dll")
//     strings.table(img)            {address: (text, "A" | "W", referenced)}
//     strings.at(img, va)           (text, "A" | "W") of the string at an address, or None
//     strings.uses(img)             {function: [(site, address, text)]}
//     strings.stack_strings(img)    [(function, where, text)]  strings assembled by immediate stores
//     strings.formats(img)          [call sites of printf-like functions with their format string
//                                    and the argument types it implies]
//
// A string is a run of printable characters ending in a zero byte (or of 16-bit characters ending
// in a zero word). One the code refers to is believed from two characters up; one nothing refers
// to is listed only from five, and only outside the code (early linkers put constants in the code
// section, where an unreferenced "string" is usually instructions).
//
// A format string says what follows it in the argument list: `"%s = %ld"` is a string pointer and
// a long. That types the unnamed arguments of every call to wsprintf and its relatives -- and
// finds the module's own printf-like functions: a function that is handed a literal with
// conversions in it, at the same position, from several places.
//
// Typing notes:
//   * The image (codebreaker.analyze.image) has no typed port. The class Image below is the part of it
//     this module uses: bits, data_segment, span(), label(), and the four answers of the C++ analysis
//     engine ("strings.table", ".uses", ".stack", ".formats": the Python's `_ask`, which hands the DLL
//     the image's names first), already read into typed rows. img.memo(key, make) is the three
//     memo_* members.
//   * A call argument (an element of `args` in literal) is a tuple ("i" | "r", int | str) in the Python:
//     the second element is an int for "i" and a register name for "r". That mixes types in one place, so
//     Arg holds a std::variant<int64_t, py::str>.
//   * The dicts of analyse() (counts, and the facts of five kinds) are records below (for_each_field gives
//     their keys in order); the facts together are a std::variant.
//   * _assemble goes through native::assemble, which answers JSON text; the rows are read from it here
//     (the Python's json.loads). The base of a row is a str in the engine's answer.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::analyze::strings {

// _SPEC: the pattern of a printf conversion -- the text of the Python's re.compile(...) (the compiled form is made in conversions)
extern const py::str _SPEC;

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
std::optional<py::str> _ascii(const py::bytes& data, int64_t start, int64_t least);
// The 16-bit-character string at data[start:], or None.
std::optional<py::str> _wide(const py::bytes& data, int64_t start, int64_t least);

// The string at exactly `va`: (text, "A" | "W"), or None.
std::optional<std::tuple<py::str, py::str>> at(Image& img, int64_t va, int64_t least = 2);

Table _table(Image& img);
Table table(Image& img);
Uses _uses(Image& img);
Uses uses(Image& img);

// Printable runs in {(base, offset): (byte, store number)} -> [(base, offset, text, wide)] (in C++).
py::list<std::tuple<py::str, int64_t, py::str, bool>> _assemble(
    const py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>>& stores, int64_t least = 4);

// [(function, "ebp-0x20", text, wide)] -- strings built by storing immediates into the frame.
py::list<StackString> stack_strings(Image& img);

// The C types of the arguments a format string consumes, in order.
py::list<py::str> conversions(const py::str& fmt, bool wide = false);

// The string a call's k-th argument points at: (address, text, kind), or None.
std::optional<std::tuple<int64_t, py::str, py::str>> literal(Image& img, const py::list<Arg>& args, int64_t k);

Formats _formats(Image& img);
Formats formats(Image& img);

Analysis analyse(Image& img);

}  // namespace codebreaker::analyze::strings
