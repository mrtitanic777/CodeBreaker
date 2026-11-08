// codebreaker/source/readable.hpp -- typed C++ port of codebreaker.source.readable.
//
// readable.py -- names in place of numbers and placeholders, in a function's written source.
//
//     readable::Readable r(doc, string_at);      // doc: the whole module, as its L2 Module (l2.module: each function
//                                                //  a MethodDecl whose `text` is its C; the header the Module's);
//                                                //  string_at(address) -> the module's string there, or None
//     auto text = r.function(name);              // the text with the names below written in
//     r.constants_used;                          // {NAME: value} of the SDK constants written in
//     readable::calls_hint(text);                // "registry: RegOpenKeyExA, RegQueryValueExA" or ""
//
// What is changed, and only where the evidence says so:
//
//     constants   a number passed to a Windows function where the SDK names that parameter's values:
//                 LocalAlloc(0x40, n) -> LocalAlloc(LMEM_ZEROINIT, n); MessageBoxA(..., 0x24) -> MB_YESNO |
//                 MB_ICONQUESTION. Only when the value is exactly one constant of the parameter's family, or
//                 (for flags) exactly an OR of them; else the number stays.
//     locals and parameters
//                 a placeholder (iVar1, uVar2, local_8, a1) passed to Windows functions always as the same
//                 parameter takes that parameter's name (hWnd, lpText); one that holds only the handle a
//                 Windows function returns is named for the handle (hwnd, hdc, hMem); one counted up in a
//                 loop and compared is i, j, k.
//     globals     a placeholder global assigned only GetProcAddress(h, "Name") is g_pfnName; one assigned only
//                 a handle a Windows function returns is g_<handle name>.
//     comparisons a variable passed to Windows always as the same parameter (a window procedure's message,
//                 passed on to DefWindowProc as Msg) has the numbers it is compared with and switched on
//                 written as that parameter's constants: case 0x10 -> case WM_CLOSE.
//     strings     the address of a string passed to Windows as a string has the string beside it in a
//                 comment, and a printf-style format's arguments the type each conversion takes.
//     fields      a pointer passed to Windows functions as one SDK structure (LPRECT, LPMSG ...), every one of
//                 whose dereferences lands on a field of that structure, has them written as fields:
//                 *(int4 *)(lprc + 8) -> ((RECT *)lprc)->right. One that is also read past the structure, or
//                 moved, or passed as two kinds, keeps its numbers.
//
// Renaming changes no code and an SDK constant is its value, so what was compiled and judged is what these
// texts compile to. Nothing is named from a guess about intent: a function's own name is never invented --
// `calls_hint` only says which Windows functions it calls, by area.
//
// Notes on the port:
//   * The module `doc` is a translate::l2::Dict; `doc.get("header") or ""` is read from its Val.
//   * `getattr(self, "_unused", None)` caching of results_unused() is a std::optional member.
//   * The Python's hash-ordered sets that fix an iteration order nowhere (the set of names built from a
//     match) are sorted wherever the order could show. One place the Python's own choice is hash-ordered:
//     _compared() takes `next(iter(uses))` from the set of (function, parameter) pairs a variable is passed
//     as; the pairs all name one family (that was checked), and here the first one passed is taken.
//   * int(text, 0) is unbounded in Python and a define of 0xFFFFFFFFFFFFFFFF is past int64: int0() keeps the
//     low 64 bits (every use masks to 32 bits, or reads a small number), and still refuses what int() refuses.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/symbols/prototypes.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::readable {

// (function without its A/W ending, lower case; parameter name) -> (prefixes, "flags" | "enum")
using Family = std::tuple<py::list<py::str>, py::str>;
const py::dict<std::tuple<py::str, py::str>, Family>& FAMILIES();

// The SDK constants: {NAME: value}, and [(NAME, value, header)] in the order the headers give them.
struct Constants {
    py::dict<py::str, int64_t> values;
    py::list<std::tuple<py::str, int64_t, py::str>> order;
};

// {NAME: value} from the SDK headers, and the order the headers give them. A name defined as an OR of others
// (WS_OVERLAPPEDWINDOW) gets the value of the OR.
const Constants& _constants();

// [(NAME, value)] of the constants with these prefixes -- from `header` (the function's own header) when it
// defines any, so MB_ for MessageBox is winuser.h's, not winnls.h's.
py::list<std::tuple<py::str, int64_t>> _family(const py::list<py::str>& prefixes,
                                               const std::optional<py::str>& header = std::nullopt);

py::list<py::str> prefixes_of(const py::str& base, const py::str& parameter);

// The SDK's name for `value` passed as `parameter` of `function`, or None.
std::optional<py::str> constant(const py::str& function, const py::str& parameter, int64_t value);

// ---- reading calls in C text ----
// (function, start of its argument list, end, [argument texts]) for every call in text.
using Call = std::tuple<py::str, int64_t, int64_t, py::list<py::str>>;
py::list<Call> calls(const py::str& text);

std::optional<symbols::prototypes::Prototype> _prototype(const py::str& name);

// ---- names for placeholders ----
py::list<py::str> _assignments(const py::str& text, const py::str& var);

// {placeholder: name} for one function's locals and parameters.
py::dict<py::str, py::str> _local_names(const py::str& text);

py::str _rename(const py::str& text, const py::dict<py::str, py::str>& names);

// *(T *)(var + K) and *(T *)var, with T and K.
py::re::Pattern _deref(const py::str& var);

// {variable: structure} for the pointers passed to Windows functions as one SDK structure.
py::dict<py::str, py::str> _struct_vars(const py::str& text);

// The readable pass over one module. The module is its L2 Module (the one module model, l2.module): its
// functions' C is read from their MethodDecls' `text` (the passes here are still text passes) and its header
// from the Module's.
class Readable {
  public:
    using StringAt = std::function<std::optional<py::str>(int64_t)>;

    explicit Readable(const translate::l2::Dict& doc, StringAt string_at = nullptr);

    translate::l2::Dict doc;
    py::dict<py::str, py::str> texts;
    StringAt string_at;
    py::set<py::str> structs_used;
    py::dict<py::str, int64_t> constants_used;
    py::dict<py::str, py::str> globals;

    py::str function(const py::str& name);

    // Names of the module's functions that are called and whose value no caller uses: what they return is left
    // over in a register, not a result (their declared type stays, as the compiled code depends on it).
    const py::set<py::str>& results_unused();

    // The module's header with the globals renamed as in the functions, and the SDK constants and structures
    // written into them defined.
    py::str header(const py::str& text);

    // The SDK structures the functions now use, laid out at the SDK's offsets.
    py::str struct_definitions();

    // #define lines for the SDK constants written in, with the values the SDK gives them.
    py::str definitions();

    py::str _constants_pass(const py::str& text);    // Readable._constants
    py::str _compared(const py::str& text);
    py::str _strings(const py::str& text);
    py::str _fields(const py::str& text);

  private:
    py::dict<py::str, py::str> _global_names();
    std::optional<py::str> _proc_name(const py::str& value);

    py::str header_text_;
    std::optional<py::set<py::str>> unused_;
};

// Which areas of the Windows API a function works with, and the functions: or "".
py::str calls_hint(const py::str& text);

}  // namespace codebreaker::source::readable
