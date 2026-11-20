// analyze/origin.hpp -- was a 16-bit function written in assembly, or made by a C compiler?
//
//     NoKnowledge knowledge;
//     origin::Result r = origin::module16(ne::Module::open_file(path), workdir, knowledge);
//     r.functions   {address: Origin{name, language, evidence, compiler}}
//     r.summary()   how many of each language, and why the assembly ones are
//     std::cout << origin::report("lzexpand.dll", r, /*list=*/true);
//
// A function is reconstructed in the language it was written in: what a C compiler made, as C; what a person wrote in assembly, as
// assembly (Windows' 16-bit core and its drivers are largely the second). Microsoft's C compilers of the period (C 5.1 to 8.0) have a
// small, regular repertoire (no LOOP/JCXZ, string instructions only with REP, no arguments in registers, no INT n, no 32-bit registers,
// BP never a plain register ...); one piece of assembly evidence makes a function assembly, and the evidence is kept with the verdict. A
// C compiler leaves fingerprints of its own (__chkstk, its arithmetic helpers, the frame it builds and takes down).
//
// The verdict is worked out every time it is asked (it is cheap beside
// describe16, which decodes every segment of the module).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "decompile/describe.hpp"

namespace cb::formats::ne {
class Module;
}

namespace cb::analyze::origin {

enum class Language : std::uint8_t { C, Assembly };
std::string_view to_text(Language language);   // "c", "assembly"

// The verdict on one function.
struct Origin {
    std::int64_t va = 0;
    std::string name;
    Language language = Language::C;
    std::vector<std::string> evidence;   // why assembly (one piece makes it so), each "<kind> (<detail>)"
    std::vector<std::string> compiler;   // the C compiler's fingerprints

    // "<name language: evidence; ...>" -- the evidence, else the compiler's fingerprints, else "nothing either way".
    std::string describe() const;
};

// What a module's verdicts add up to.
struct Summary {
    std::size_t c = 0, assembly = 0;
    // The kind of each function's first piece of evidence (the text before " ("), commonest first, ties in the order first met.
    std::vector<std::pair<std::string, std::size_t>> reasons;
};

struct Result {
    std::map<std::int64_t, Origin> functions;   // by address
    Summary summary() const;
};

// Every function of a 16-bit module, judged. `workdir` is where describe16 writes its files; `policy` is describe16's. `more`: {address:
// name} of functions found besides the ones the module's own tables give (the decompiler finds the ones only called). Throws what
// describe16 throws.
Result module16(const formats::ne::Module& module, const std::filesystem::path& workdir, decompile::describe::Knowledge& knowledge,
                const std::map<std::int64_t, std::string>* more = nullptr, const decompile::describe::Policy& policy = {});

// The function at `start`..`end` of a segment judged by what its code does. `starts`: the offsets of the segment's functions;
// `names`: {address: name} of the module's functions, to name what it calls; `base`: the address of the segment's offset 0; `g3`: the
// module's linker is C 7/8's, whose /G3 code uses 32-bit registers. Fills o.language, o.evidence and o.compiler.
void judge(Origin& o, const decompile::describe::Code16& code, std::int64_t start, std::int64_t end, const std::set<std::int64_t>& starts, bool g3,
           const std::map<std::int64_t, std::string>& names, std::int64_t base);

// The text `codebreaker origin <module>` prints: "<name>: N functions -- C C, A assembly", the twelve commonest reasons, and with `list` a
// line for each function.
std::string report(std::string_view module_name, const Result& result, bool list);

// What the command needs of the rest of the program.
struct Environment {
    // A corpus name -> the binary (corpus.retail); nullopt when there is none. A path that is a file is taken as it is.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    // Where describe16 writes its files: <scratch>/origin/<module name>.
    std::filesystem::path scratch;
    // What the names of describe16 are asked of; none given, it knows nothing (a module's own tables name the functions).
    decompile::describe::Knowledge* knowledge = nullptr;
};

// `origin <module> [--list]`: prints `report` of the module (exit 0); a module that is not found says so (exit 2).
cli::Command command(const Environment& environment);

}  // namespace cb::analyze::origin
