// The calling-convention service: the SLEIGH compiler specs (decompile/engine/languages/*.cspec, named by the *.ldefs) are the one
// source of truth for what a calling convention is, and this reads them (a small XML reader of its own, no dependency on the
// decompiler core) so that every lane can ask. It describes; it never judges a particular function (that is analyze/conventions).
//
// The spelling of a model's name is here too: name_of/parse are the one place that knows "__stdcall16far" is a callee-pops convention
// at a far distance, so a lane that needs the convention, the distance or the other spelling does not test the text itself.
// to_l2/to_export map a model onto the enums of the L2 schema and of the Win16 export tables.
//
// The core throws on an element it does not know inside a <prototype> ("Unknown element in prototype", fspec.cc), so a fact the
// specs lack (a Win16 entry prologue, a symbol decoration, a compiler keyword) is kept in config/abi/*.json beside them, keyed by
// the model's name, not as a new element in a cspec.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "symbols/wine16.hpp"
#include "translate/ir.hpp"

namespace cb::decompile::conventions {

// What a model is called, apart from where it is (near or far, 16 or 32 bits).
enum class Convention : std::uint8_t { Cdecl, Stdcall, Fastcall, Thiscall, Regcall, Interrupt, InterruptKeepDs };
// Flat: no suffix (32-bit and other machines); Near and Far: the 16-bit models ("__cdecl16near", "__stdcall16far").
enum class Distance : std::uint8_t { Flat, Near, Far };

struct Parsed {
    Convention convention = Convention::Cdecl;
    Distance distance = Distance::Flat;
    friend bool operator==(const Parsed&, const Parsed&) = default;
};

// "__stdcall16far", "__cdecl", "__fastcall16near", "__regcall", "__int16", "__int16keepds". A convention that has no
// near/far form (regcall, interrupts, thiscall) has no suffix whatever the distance asked for.
std::string name_of(Convention convention, Distance distance = Distance::Flat);
// The inverse: nullopt for a name that is not one of these (the PowerPC "__stdcall" is, the z80's "__asmA" is not).
std::optional<Parsed> parse(std::string_view model);

// Does this text (a heading, a line of C) name a 16-bit model of the convention ("__cdecl16" in "int2 __cdecl16far f()")? Any
// distance; a model that is the prefix of a longer word counts, as the substring tests this replaces did.
bool mentions(std::string_view text, Convention convention);
// ... of any convention that has a 16-bit form (cdecl, stdcall, fastcall).
bool mentions_16bit(std::string_view text);

// The convention in the vocabulary of the L2 schema (callee pops, 16-bit: Pascal) and of the Win16 export tables.
translate::ir::CallingConvention to_l2(Parsed parsed);
translate::ir::CallingConvention to_l2(std::string_view model);   // an unknown model is Cdecl, as the L2 reader's default
symbols::wine16::Convention to_export(Parsed parsed);

enum class Order : std::uint8_t { LeftToRight, RightToLeft };   // Pascal pushes left to right

struct RegisterArg {
    std::string reg;           // "AX", "r3", "f1"
    unsigned min_size = 1, max_size = 8;
    bool is_float = false;
};

struct ResultEntry {
    std::string reg;           // "AX", "DX:AX" (a join, high first), "r3", "ST0"
    unsigned min_size = 1, max_size = 8;
    bool is_float = false;
};

struct Model {                                   // one <prototype>
    std::string name;                            // "__stdcall16far"
    std::string spec;                            // the file's stem: "x86-16"
    bool is_default = false;                     // inside <default_proto>
    int stack_shift = 0;                         // bytes the call leaves on the stack (the return address)
    std::optional<int> extra_pop;                // bytes the callee pops; 0: the caller does; nullopt: "unknown" (the callee's `ret N`)
    std::vector<RegisterArg> reg_args;           // in order
    std::optional<int> stack_args_offset;        // first stack argument from the entry sp (4 far, 2 near); nullopt: none on the stack
    unsigned stack_align = 0;                    // 0: the model gives none
    Order stack_order = Order::RightToLeft;      // the Pascal models push left to right
    std::vector<ResultEntry> results;            // by size
    std::vector<std::string> unaffected;         // registers (a memory range among them is left out)
    std::vector<std::string> killed;
    bool has_this = false;                       // the "__thiscall" model

    bool far_call() const { return stack_shift == 4 && spec == "x86-16"; }
    // The register that holds a result of this many bytes (a join is "DX:AX"); empty when the model names none.
    std::string result_register(unsigned bytes, bool is_float = false) const;
};

// One <compiler> of a language in an ldefs: the spec it reads.
struct Compiler {
    std::string name, id, spec;                  // "Visual Studio", "windows", "x86win.cspec"
};

class Service {
public:
    // Loaded once, from CB_LANGUAGES_DIR or the checkout's own folder; throws UsageError if there is none.
    static const Service& get();
    // A service over another folder (tests; a run that compares another set of specs).
    explicit Service(const std::filesystem::path& languages);

    const std::filesystem::path& folder() const { return folder_; }
    // The language ids of the ldefs ("x86:LE:16:Real Mode") and their compilers; empty for an unknown id.
    std::vector<std::string> languages() const;
    std::span<const Compiler> compilers(std::string_view language) const;
    // A model of a language: by name, in the compiler with this id, or (compiler empty) in the first that has one.
    const Model* model(std::string_view language, std::string_view name, std::string_view compiler = {}) const;
    // The default prototype of a language's compiler (the first when compiler is empty).
    const Model* default_model(std::string_view language, std::string_view compiler = {}) const;
    // A cspec's models by its file stem ("x86-16", "ppc_32"); empty for one that is not there.
    std::span<const Model> models_of(std::string_view spec) const;
    const Model* model_of(std::string_view spec, std::string_view name) const;
    std::vector<std::string> specs() const;      // every cspec read, by stem

    // The facts the specs lack (config/abi/*.json), by model name: nullptr when there are none.
    struct Extra {
        std::string decoration;                  // "_%s@%N", "_%s", "%s_", "%s"
        std::map<std::string, std::string> compiler_keyword;   // by compiler: {"msc6": "_pascal"}
        std::string notes;
    };
    const Extra* extra(std::string_view model) const;

private:
    struct Spec { std::vector<Model> models; };
    struct Language { std::vector<Compiler> compilers; };
    std::filesystem::path folder_;
    std::map<std::string, Spec, std::less<>> specs_;
    std::map<std::string, Language, std::less<>> languages_;
    std::map<std::string, Extra, std::less<>> extras_;
};

}  // namespace cb::decompile::conventions
