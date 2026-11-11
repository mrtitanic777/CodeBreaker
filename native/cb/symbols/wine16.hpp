// cb/symbols/wine16.hpp -- the 16-bit Windows modules' exports as Wine's .spec files type them.
//
//     const wine16::Specs wine("C:/wine/dlls");                  // the folder the `wine16` setting names
//     const wine16::Export& e = wine.exports("SETUPX").at(7);    // IpFindFirstLine: pascal, (word str str ptr), a stub
//
// Wine (LGPL) reimplements the 16-bit system modules -- KERNEL, USER, GDI, SETUPX, the drivers -- and declares every
// export in a .spec file: its ordinal, convention, the kind of each argument and whether the result is 16 bits (-ret16)
// or 32. An export Wine leaves unimplemented (`stub`) often carries its arguments in a comment
// (`stub IpFindFirstLine #(word str str ptr)`). Argument kinds, as pushed:
//
//     Word, SWord           2 bytes (unsigned, signed)
//     Long, SegPtr, SegStr  4 bytes (a number; a far pointer passed as such)
//     Ptr, Str              4 bytes, a far pointer (Wine converts it to a flat one)
//
// The order is the declaration's: for pascal the first is pushed first. The folders read are the `*.dll16`, `*.exe16`
// and `*.drv16` ones of Wine's `dlls` folder, each file `*.spec` in them; modules are named in capitals, KRNL386 being
// KERNEL.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::symbols::wine16 {

enum class Convention { Pascal, Cdecl, Varargs, Stdcall };

enum class ArgKind { Word, SWord, Long, SegPtr, SegStr, Ptr, Str };

// Bytes an argument of this kind takes on the stack.
std::uint32_t width(ArgKind kind);
// Whether it is a far pointer.
bool is_pointer(ArgKind kind);
// The type the reconstruction gives an argument of this kind.
std::string_view c_type(ArgKind kind);
// The kind as a .spec file spells it ("s_word", "segstr" ...).
std::string_view spelling(ArgKind kind);

struct Export {
    std::string name;
    Convention convention = Convention::Pascal;   // a stub is declared pascal
    std::optional<std::vector<ArgKind>> args;     // nullopt: nothing is said of the arguments, or a kind this reader does not know
    bool ret16 = false;                           // the result is 16 bits, not 32
    bool stub = false;                            // Wine leaves it unimplemented
};

using Exports = std::map<std::uint32_t, Export>;   // by ordinal, which is the order they are read in

// The exports one .spec file's text declares; a line that says none is passed over, a later line for an ordinal
// replaces an earlier one.
Exports read_spec(std::string_view text);

// The exports of every module of a Wine `dlls` folder, read on first use.
class Specs {
public:
    // `wine_dlls`: Wine's `dlls` folder; empty, or a folder that is not there: every module has no exports.
    explicit Specs(std::filesystem::path wine_dlls) : folder_(std::move(wine_dlls)) {}

    // The exports of a 16-bit module (KERNEL, USER, SETUPX ... in any case), or none when Wine has none. IoError when a
    // .spec file of the folder cannot be read. Thread-safe.
    const Exports& exports(std::string_view module) const;

private:
    void load() const;

    std::filesystem::path folder_;
    mutable std::once_flag loaded_;
    mutable std::map<std::string, Exports> modules_;   // by module name in capitals
};

}  // namespace cb::symbols::wine16
