// cb/symbols/demangle.hpp -- a decorated linker name as the declaration it was made from.
//
//     demangle::declaration("?AddRef@CFoo@@UAGKXZ")  "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
//     demangle::short_name("?AddRef@CFoo@@UAGKXZ")   "CFoo::AddRef"
//     demangle::parse("?AddRef@CFoo@@UAGKXZ")        a Declaration: name, kind, access, static, virtual, far,
//                                                    convention, result, parameters, const
//     demangle::plain("_LZRead@12")                  {"LZRead", "__stdcall", 12}     (also "LZRead@12")
//
// A C++ compiler writes a function's whole declaration into its linker name: the classes it is nested in, whether it
// is public, static or virtual, its calling convention, its result and every parameter type. Microsoft's scheme is the
// one the corpus uses (every C++ export and every MFC import, 32-bit and 16-bit); the 16-bit compilers use the same
// scheme with near and far added, and those are read too. A C name with its convention's decoration (`_name@12`,
// `@name@8`, `_name`) is taken apart by plain().
//
// The text produced is the one Microsoft's own tools print, so it can be compared with them. Anything that cannot be
// read gives nothing: a name is never guessed at. Names are read as bytes; a name with a non-ASCII byte is read as
// the bytes it is.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::symbols::demangle {

enum class Kind {
    Function,   // a function or method
    Data,       // a variable, a vtable, a string literal
    Name,       // a name and nothing else
};

// What a decorated C++ name says. A data name carries only name, kind and text.
struct Declaration {
    std::string name;                     // qualified: "CFoo::AddRef"
    std::string base;                     // the last part: "AddRef"
    std::string owner;                    // the classes it is nested in: "Outer::Inner" ("" for none)
    Kind kind = Kind::Function;
    std::string access;                   // "public", "protected", "private"; "" for a free function
    bool is_static = false;
    bool is_virtual = false;
    bool far = false;                     // a 16-bit far function
    std::string convention;               // "__stdcall", "__thiscall", ...; "" when the name has none
    std::string returns;                  // the result type
    std::vector<std::string> parameters;  // empty for (void)
    std::string const_qualifier;          // "const" and so on, after the parameter list of a method
    std::string text;                     // the whole declaration
};

// A type's name as run-time type information keeps it -- `.?AVCFoo@@` (a class), `.?AU...` (a struct) -- as C++
// writes it: "CFoo", "Outer::Inner", "std::vector<int>".
std::optional<std::string> type_name(std::string_view decorated);

// What a decorated C++ name says; nullopt when it is not one or is not understood.
std::optional<Declaration> parse(std::string_view name);

// The declaration a decorated C++ name was made from.
std::optional<std::string> declaration(std::string_view name);

// Just the qualified name (`CFoo::AddRef`).
std::optional<std::string> short_name(std::string_view name);

// A C linker name without its convention's decoration.
struct PlainName {
    std::string name;
    std::string convention;                        // "__stdcall", "__fastcall", "__cdecl", or "" when undecorated
    std::optional<std::uint32_t> argument_bytes;   // what `@N` says the function removes from the stack
};
PlainName plain(std::string_view name);

// The best readable form of any linker name: a C++ declaration, a C name without its decoration, or the name as it
// stands.
std::string readable(std::string_view name);

// Bytes of arguments the function removes on return, as far as the name alone settles it (nullopt when it does not:
// a caller-cleans function, a class passed by value, a 16-bit function whose sizes depend on the memory model).
std::optional<std::uint32_t> stack_bytes(std::string_view name);

// `codebreaker demangle <name> [<name> ...]`: prints each name readable; returns the exit code.
int run(std::span<const std::string> args);

}  // namespace cb::symbols::demangle
