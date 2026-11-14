// cb/translate/frontends/csharp/frontend.hpp -- the C# front end, step one and the whole of it: Roslyn's bound tree of a set of
// files, run through the installed .NET SDK, and lowered into the translator's IR (lower.hpp is step two).
//
// CsTreeDump (CsTreeDump.cs) runs the C# compiler of the SDK over the files -- parse and bind, no assembly written -- and writes
// each file's declarations, and every body and initialiser as Roslyn's IOperation tree, with what the compiler resolved. The tool
// is compiled once with the SDK's own csc.dll against the SDK's Microsoft.CodeAnalysis assemblies, by path -- no project, no NuGet
// restore, nothing downloaded -- and kept under the state folder, keyed by its source and the SDK.
//
//     const auto sdk = dotnet::Sdk::find();
//     const Frontend frontend{*sdk, "CsTreeDump.cs", state_folder};
//     Arena arena;
//     Program program = frontend.lower(arena, {"Prog.cs"}, work);   // CompileError if the C# does not compile
//     for (ir::Node* unit : program.units) ...                       // one ir::Unit per file, in order of the file's name
//     for (const Refused& r : program.refused) ...                   // what became ir::Unsupported
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/json.hpp"
#include "cb/translate/dotnet/package.hpp"
#include "cb/translate/frontends/csharp/lower.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::frontends::csharp {

// How long CsTreeDump may take over a program.
inline constexpr std::chrono::minutes kDumpTimeout{15};

// What the C# compiler said of the files, as CsTreeDump writes it.
struct Diagnostic {
    std::string kind;       // "ERROR" or "WARNING"
    std::string file;
    std::uint32_t line = 0;
    std::string message;
};

// The C# did not compile: the front end needs bound code. The first error is the message.
class CompileError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(CompileError)
};

// One dumped file: its name without `.cs` and its tree.
struct DumpedFile {
    std::string name;
    Json tree;
};

struct Dumped {
    std::vector<DumpedFile> files;           // in order of the name
    std::vector<Diagnostic> diagnostics;
    std::vector<Diagnostic> errors() const;
};

// What a program lowers to.
struct Refused {
    std::string where;      // "<file name>:<line>"
    std::string kind;       // "await", "iterator", ... ("" when the dump named none)
    std::string why;
};

struct Program {
    std::vector<ir::Node*> units;            // ir::Unit nodes, in the arena the caller gave
    std::vector<Refused> refused;            // in the order met, file by file
};

class Frontend {
public:
    // `sdk` must outlive the Frontend. `tool_source` is CsTreeDump.cs; `state` the folder the compiled tool is kept in (as
    // <state>/translate/csdump-<key>).
    Frontend(const dotnet::Sdk& sdk, std::filesystem::path tool_source, std::filesystem::path state)
        : sdk_(sdk), source_(std::move(tool_source)), state_(std::move(state)) {}

    // CsTreeDump.dll, compiled when its source or the SDK changed (ToolError when it does not compile or there is no SDK).
    std::filesystem::path tool() const;

    // Dump the files into `out_dir` (made new). -> how many were dumped.
    std::size_t run(const std::vector<std::filesystem::path>& files, const std::filesystem::path& out_dir,
                    const std::optional<std::string>& langversion = std::nullopt) const;

    // The dumped trees and diagnostics of out_dir.
    static Dumped load(const std::filesystem::path& out_dir);

    // run + load.
    Dumped dump(const std::vector<std::filesystem::path>& files, const std::filesystem::path& out_dir,
                const std::optional<std::string>& langversion = std::nullopt) const;

    // Dump (under `work`/dump) and lower. CompileError when the C# has errors.
    Program lower(Arena& arena, const std::vector<std::filesystem::path>& files, const std::filesystem::path& work) const;

private:
    const dotnet::Sdk& sdk_;
    std::filesystem::path source_, state_;
};

// Every .cs file under a folder (or the one file), sorted.
std::vector<std::filesystem::path> sources(const std::filesystem::path& src);

}  // namespace cb::translate::frontends::csharp
