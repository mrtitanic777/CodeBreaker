// dotnet/decompiler/decompile_run.hpp -- DecompileRun: the state of one decompilation, shared by everything that takes part in it (the
// statement and expression builders, the AST transforms, member hiding): the settings and cancellation token, the namespaces in use and the
// using scope made from them, the documentation provider, the preprocessor symbols defined so far, and the caches that make the several
// consumers of an analysis decide from the same result.
//
//     DecompileRun run(settings, using_scope);
//     run.namespaces = {"System", "System.Linq"};
//     RecordDecompiler* record = run.record_decompilers.at(type_definition).get();
//     bool automatic = run.automatic_events.contains(ev);
//
// Ported from ILSpy (ICSharpCode.Decompiler: DecompileRun, EnumValueDisplayMode, from DecompileRun.cs), Copyright Siegfried Pammer for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Differences: the run neither owns the settings (held by reference: they must outlive it) nor the documentation provider (a pointer: the
// CSharpDecompiler that made the run owns it); the using scope is made in the thread's ILArena, which keeps it. A map keyed by a type
// definition or an event is keyed by the pointer (definitions are one object each).
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}
namespace cb::dotnet::decompiler::csharp {
class RecordDecompiler;
}
namespace cb::dotnet::decompiler::csharp::type_system {
class UsingScope;
}
namespace cb::dotnet::decompiler::documentation {
class IDocumentationProvider;
}

namespace cb::dotnet::decompiler {

class DecompileRun {
public:
    // The settings must outlive the run (std::invalid_argument for a null using scope).
    DecompileRun(const DecompilerSettings& settings, const csharp::type_system::UsingScope* using_scope);
    ~DecompileRun();
    DecompileRun(const DecompileRun&) = delete;
    DecompileRun& operator=(const DecompileRun&) = delete;

    // The preprocessor symbols defined so far (#define lines the decompiler has added for [Conditional] calls).
    std::unordered_set<std::string> defined_symbols;
    // Every namespace that may be used by the code being decompiled.
    std::unordered_set<std::string> namespaces;
    util::CancellationToken cancellation_token;
    const DecompilerSettings& settings;
    // Where XML documentation comes from; nullptr for none. Not owned.
    documentation::IDocumentationProvider* documentation_provider = nullptr;
    // The record decompiler of each record type being decompiled.
    std::unordered_map<const typesystem::ITypeDefinition*, std::unique_ptr<csharp::RecordDecompiler>> record_decompilers;

    // Memoized AutoEventDecompiler verdicts (nullptr: the event is not automatic). Shared so that every consumer of the verdict (member
    // hiding, the event declaration, reference translation) decides from the same analysis.
    std::unordered_map<const typesystem::IEvent*, const typesystem::IField*> automatic_events;

    std::unordered_map<const typesystem::ITypeDefinition*, bool> type_hierarchy_is_known;

    const csharp::type_system::UsingScope* using_scope;
};

enum class EnumValueDisplayMode { None, All, AllHex, FirstOnly };

}  // namespace cb::dotnet::decompiler
