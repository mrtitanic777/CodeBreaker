// source/readable.hpp -- names in place of numbers and placeholders, in a function's written source.
//
//     const readable::Sdk sdk = readable::Sdk::read(folders, prototypes);        // the SDK's constants, prototypes and structures
//     std::unique_ptr<module_writer::ReadablePass> pass = readable::make_pass(sdk, stored, string_at);
//     std::string text = pass->function(address);                                  // the text with the names below written in
//     readable::calls_hint(sdk, text)                                              // "registry: RegOpenKeyExA, RegQueryValueExA" or ""
//
// What is changed, and only where the evidence says so:
//
//     constants   a number passed to a Windows function where the SDK names that parameter's values: LocalAlloc(0x40, n) ->
//                 LocalAlloc(LMEM_ZEROINIT, n); MessageBoxA(..., 0x24) -> MB_YESNO | MB_ICONQUESTION. Only when the value is exactly one
//                 constant of the parameter's family, or (for flags) exactly an OR of them; else the number stays.
//     locals and parameters
//                 a placeholder (iVar1, uVar2, local_8, a1) passed to Windows functions always as the same parameter takes that parameter's
//                 name (hWnd, lpText); one that holds only the handle a Windows function returns is named for the handle (hwnd, hdc, hMem);
//                 one counted up in a loop and compared is i, j, k.
//     globals     a placeholder global assigned only GetProcAddress(h, "Name") is g_pfnName; one assigned only a handle a Windows function
//                 returns is g_<handle name>.
//     comparisons a variable passed to Windows always as the same parameter (a window procedure's message, passed on to DefWindowProc as
//                 Msg) has the numbers it is compared with and switched on written as that parameter's constants: case 0x10 -> case WM_CLOSE.
//     strings     the address of a string passed to Windows as a string has the string beside it in a comment, and a printf-style format's
//                 arguments the type each conversion takes: wsprintfA(buf, 0x40a1b0 /* "%s = %ld" */, name /* %s */, n /* %ld */).
//     fields      a pointer passed to Windows functions as one SDK structure (LPRECT, LPMSG ...), every one of whose dereferences lands on a
//                 field of that structure, has them written as fields: *(int4 *)(lprc + 8) -> ((RECT *)lprc)->right. One that is also read
//                 past the structure, or moved, or passed as two kinds, keeps its numbers.
//
// Renaming changes no code and an SDK constant is its value, so what was compiled and judged is what these texts compile to. Nothing is
// named from a guess about intent: a function's own name is never invented -- `calls_hint` only says which Windows functions it calls, by
// area.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "source/module_writer.hpp"
#include "symbols/prototypes.hpp"
#include "symbols/sdk_structs.hpp"
#include "translate/l2.hpp"

namespace cb::source::readable {

// The values of the SDK's #defines, as the headers give them: the name, the value (32 bits), the header (its file name, lower case).
struct Define {
    std::string name;
    std::uint32_t value = 0;
    std::string header;
};

// What the pass reads of the SDK: the constants of the 32-bit headers (then the 16-bit), the prototypes of the Windows functions and the layouts
// of its structures.
class Sdk {
public:
    // Reads every header of the folders. `prototypes` outlives the Sdk.
    static Sdk read(const symbols::prototypes::HeaderFolders& folders, const symbols::prototypes::PrototypeTable& prototypes);
    Sdk(std::vector<Define> defines, const symbols::prototypes::PrototypeTable& prototypes, symbols::sdk_structs::StructTable structures);

    // The `#define NAME value` constants in the order the headers give them, then the names defined as an OR of others (a name defined as an
    // OR (WS_OVERLAPPEDWINDOW) gets the value of the OR).
    const std::vector<Define>& defines() const { return defines_; }
    std::optional<std::uint32_t> value(std::string_view name) const;

    // The prototype of a Windows function as its headers give it: the name, else the name without its A/W ending.
    const symbols::prototypes::Prototype* prototype(std::string_view name) const;
    const symbols::sdk_structs::StructTable& structures() const { return structures_; }

    // The SDK's name for `value` passed as `parameter` of `function`, or nullopt: one name, or names joined by " | ".
    std::optional<std::string> constant(std::string_view function, std::string_view parameter, std::int64_t value) const;
    // The prefixes of the constants a parameter takes (none: the parameter is not one whose values are named).
    static const std::vector<std::string>& family_of(std::string_view function, std::string_view parameter);

private:
    std::vector<Define> defines_;
    std::map<std::string, std::uint32_t, std::less<>> values_;
    const symbols::prototypes::PrototypeTable* prototypes_;
    symbols::sdk_structs::StructTable structures_;
};

// The module's string at an address, or nothing (for naming what GetProcAddress asks for, and a string passed to Windows).
using StringAt = std::function<std::optional<std::string>(std::uint32_t address)>;

// The pass over one module: `stored` is the module as the store holds it, each function's C by its address and its header. The pass reads
// `sdk` for as long as it lives.
std::unique_ptr<module_writer::ReadablePass> make_pass(const Sdk& sdk, const translate::l2::Made& stored, StringAt string_at);

// Which areas of the Windows API a function works with, and which functions: "registry: RegOpenKeyExA, ..." or "".
std::string calls_hint(const Sdk& sdk, std::string_view text);

// ---- reading calls in C text (shared with the tests) ----------------------------------------------------------------------------

// A call in a text: the function, where its argument list starts and ends (the offsets just after the '(' and of the ')'), and each argument as
// written.
struct Call {
    std::string function;
    std::size_t start = 0, end = 0;
    std::vector<std::string> args;
};
std::vector<Call> calls(std::string_view text);

}  // namespace cb::source::readable
