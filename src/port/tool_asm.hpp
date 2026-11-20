// port/tool_asm.hpp -- the assembler modules of the program the port reads, and what is made of each.
//
// A program of the DOS C compilers of the 1980s carries assembler modules beside its C. The port sorts them by what they are:
//
//   runtime   the old runtime's: it makes public what the runtime defines (or every name it makes public is the C library's or
//             the platform layer's), or it is the start-up code (`END start`) -- the C library of today stands in
//   data      data alone (no instruction in a code segment): its variables are defined in C, with the types the C program
//             declares them, at the places the old linker gave them
//   code      the program's own assembly language: ported as assembly language (port/asm16) and assembled with ml64
//
//     const Role role = asm_role(text, profile);
//     AsmPorting made = port_assembler_modules(sink, c_files, originals, modules, roles, profile, options);
//
// What the modules leave for the rest of the port goes in an `AsmSink` (the C definitions of the data, the names that were
// one place, the sizes the assembler placed), so that this module knows nothing of the port beside it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/ordered_map.hpp"
#include "port/asm16.hpp"
#include "port/bss.hpp"
#include "port/pre_ansi_types.hpp"

namespace cb::port::tool {

enum class AsmKind { Runtime, Data, Code };

struct Role {
    AsmKind kind = AsmKind::Runtime;
    std::vector<std::string> publics;   // sorted
};

std::string_view asm_kind_name(AsmKind kind);

// What an assembler module of the program is (see the top of this file).
Role asm_role(std::string_view text, const pre_ansi::Profile& profile);

// {name: the type it returns, as today's C spells it} of the functions the C program declares and does not define
// (`extern T NEAR f();`): what it expects of the assembly's.
std::map<std::string, std::string> c_declared(std::span<const std::string> texts);

// {routine: its lines} of the old runtime's routines that are straight lines to a RET (cret: the shared end of the routines
// of Lattice-era C): what a jump into one does, there.
std::map<std::string, std::vector<std::string>> runtime_tails(const OrderedMap<std::string, std::string>& modules);

// A JMP to one of those routines becomes its instructions. Adds a note to `notes` when there were any.
std::string inline_tails(std::string_view text, const std::map<std::string, std::vector<std::string>>& tails,
                         std::vector<std::string>& notes, std::string_view module_name);

// The variables of an assembler module of data alone: C definitions, with the C program's types where it declares them.
struct DataDefinitions {
    std::vector<std::string> definitions;
    std::map<std::string, std::string> one_place;   // a scalar under two names: the name to keep (each other name -> it)
    std::map<std::string, std::string> aliases;     // a name that is a place in another variable: the expression
    std::map<std::string, std::uint64_t> sizes;     // each variable's own bytes
};
DataDefinitions asm_data_defs(std::string_view text, const asm16::CTypes& ctypes);

// {alias: (owner, byte offset now)} of the names the old linker gave places inside other variables.
std::map<std::string, asm16::Alias> alias_places(const pre_ansi::Aliases& aliases, const asm16::CTypes& ctypes);

// What the C program calls assembler routine `fn` with, argument by argument: the old size and what it points to (a pointer to a
// variable it declares, a pointer variable; far ones are 4 bytes, as they were pushed). The call with the most known pointers
// wins.
class CallArgTypes {
public:
    // `originals`: the sources as they were (where `far` is said).
    CallArgTypes(std::span<const std::string> originals, const asm16::CTypes& ctypes);
    std::vector<asm16::ParamType> of(const std::string& fn, std::span<const std::string> texts) const;

private:
    const asm16::CTypes& ctypes_;
    std::set<std::string> far_;
};

// What the assembler modules leave for the rest of the port.
struct AsmSink {
    std::vector<std::pair<std::string, std::string>> asm_data;   // (module, a C definition) of data alone, in order
    std::map<std::string, std::string> renames;                   // a name that is another: the name kept
    std::vector<pre_ansi::DeclRule> decl_rules;
    std::map<std::string, std::uint64_t> asm_sizes;               // each variable the assembler placed: its bytes
    pre_ansi::Aliases aliases;                                    // a name that is a place in another variable
};

struct AsmOptions {
    bool c_underscore = true;                                     // a routine without a leading underscore is pascal (the profile's)
    std::optional<std::filesystem::path> old_assembler;           // the period assembler whose listing places the tables
    asm16::AssemblerRunner run_old_assembler;                     // runs it (the DOS emulator's business)
    std::optional<std::filesystem::path> ml64;                    // assembles what is ported
    std::filesystem::path out;                                    // where the ported modules and their objects go
};

struct AsmPorting {
    std::vector<std::string> objects;                             // the objects to link
    std::map<std::string, std::string> prototypes;                // {C name: its declaration} of the routines ported
    std::vector<std::string> notes;
};

// The program's own assembly language ported as assembly language and assembled; its data alone as C definitions.
// `c_files`: the port's C (new layouts); `originals`: the sources as they were; `modules`/`paths`: the assembler modules and
// where each was read; `roles`: asm_role of each; `libc`: the names of the C library (a routine of one of them is kept under
// a name of its own).
AsmPorting port_assembler_modules(AsmSink& sink, std::span<const std::string> c_files, std::span<const std::string> originals,
                                  const OrderedMap<std::string, std::string>& modules,
                                  const std::map<std::string, std::filesystem::path>& paths,
                                  const std::map<std::string, Role>& roles, const pre_ansi::Profile& profile,
                                  const AsmOptions& options);

}  // namespace cb::port::tool
