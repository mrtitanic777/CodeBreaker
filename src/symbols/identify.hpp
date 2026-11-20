// symbols/identify.hpp -- `identify <module>`: which of a file's functions came with the compiler, and which compiler.
//
//     codebreaker identify comctl32.dll                                       its runtime-library functions and its compiler
//     codebreaker identify --learn "Visual C++ 2.0 C runtime=C:\msvc20\lib\libc.lib" ...    make the signatures from libraries
//
//     Identifier identifier({corpus, knowledge, ordinals});
//     identifier.identify(path)            {address: {name, library}}   (a 16-bit module's address as decompile::describe::address lays it out)
//     identifier.compiler(path, found)     {linker, compiler, evidence}
//
// runtime_library.hpp does the work over a module as plain data (what the format readers found); this is the part that reads the
// file: the code sections and the function starts of a 32-bit module (the carve), the segments and function starts of a 16-bit one,
// whether the thunk compiler made it (build/flat_thunk), the signatures the knowledge base holds, and the names the ordinal tables
// give the numbered exports of a module that is itself a runtime library.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "knowledge/package.hpp"
#include "symbols/environment.hpp"
#include "symbols/ordinals.hpp"
#include "symbols/runtime_library.hpp"

namespace cb::symbols::identify {

// A function's address: a 32-bit module's as it is, a 16-bit one's as decompile::describe::address lays it out.
using Address = std::uint64_t;
using Found = std::map<Address, runtime_library::Identified>;

class Identifier {
public:
    struct Sources {
        const base::Corpus& corpus;
        knowledge::Knowledge& knowledge;        // the signatures
        const ordinals::Ordinals& ordinals;     // the names of a runtime-library module's numbered exports
    };

    explicit Identifier(Sources sources);
    Identifier(const Identifier&) = delete;
    Identifier& operator=(const Identifier&) = delete;
    ~Identifier();

    // The functions of a module that belong to a runtime library; empty for a file that is neither a 32-bit x86 PE nor a 16-bit NE.
    // NotFoundError / FormatError for a file that cannot be read.
    Found identify(const std::filesystem::path& module);

    // What built a module, from what the file itself says (see runtime_library::describe_compiler); `found` is identify()'s answer for it.
    runtime_library::CompilerInfo compiler(const std::filesystem::path& module, const Found& found);
    runtime_library::CompilerInfo compiler(const std::filesystem::path& module) { return compiler(module, identify(module)); }

    // How many functions the module has (the starts the decompiler would describe); 0 for a file of another kind.
    std::size_t function_count(const std::filesystem::path& module);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

using Environment = symbols::Environment;

// `identify <module>` and `identify --learn WHAT=FILE...`. Everything the command opens (the knowledge base, the corpus's modules) is opened
// when it runs.
cli::Command command(const Environment& environment);

}  // namespace cb::symbols::identify
