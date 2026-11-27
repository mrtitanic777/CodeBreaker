// dotnet/decompiler/csharp/auto_event_decompiler.hpp -- recognizes automatic events (`public event EventHandler E;`) by structurally
// matching the ILAst of the compiler-made add and remove accessors with the backing field.
//
//     const IField* backing_field = nullptr;
//     if (auto_event_decompiler::is_automatic_event(type_system, ev, decompile_run, token, &backing_field)) ...   // one field-like event
//
// The accessor bodies are decompiled with a fixed set of decompiler settings (CSharpDecompiler::decompile_body_for_analysis), so the
// recognition does not depend on the settings that shape the final C# output. The verdict is kept in the DecompileRun, so that every
// consumer of it (member hiding, the event declaration, reference translation) decides from the same analysis.
//
// The shapes matched: the thread-safe compare-exchange loop of csc 4 and Roslyn (C# and VB), the one of mcs (which passes the combined
// delegate straight to Interlocked.CompareExchange), and the plain `field = (T)Delegate.Combine(field, value)` that csc before 4.0 wrote
// (with [MethodImpl(Synchronized)] on the accessors).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: AutoEventDecompiler, from AutoEventDecompiler.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's static class is the namespace auto_event_decompiler. It needs
// DecompileRun (automatic_events: a map from event to its backing field, nullptr for an event that is not automatic) and
// CSharpDecompiler::decompile_body_for_analysis, which belong to the C# decompiler's own port.
#pragma once

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler {
class DecompileRun;
}
namespace cb::dotnet::decompiler::typesystem {
class DecompilerTypeSystem;
}
namespace cb::dotnet::decompiler::csharp::syntax {
class EventDeclaration;
class TypeSystemAstBuilder;
}

namespace cb::dotnet::decompiler::csharp::auto_event_decompiler {

// Whether the event is an automatic event, memoizing the verdict in the DecompileRun. The backing field is set for an automatic event
// (nullptr otherwise).
bool is_automatic_event(const typesystem::DecompilerTypeSystem& type_system, const typesystem::IEvent* ev, DecompileRun& decompile_run,
                        const util::CancellationToken& cancellation_token, const typesystem::IField** backing_field);

// Adds the add-accessor and backing-field attributes of an automatic event to its field-like declaration (as "method:" and "field:"
// sections), dropping the attributes the compiler puts on automatic events.
void add_field_like_event_attributes(syntax::EventDeclaration* event_decl, syntax::TypeSystemAstBuilder& ast_builder,
                                     const typesystem::IEvent* ev, const typesystem::IField* backing_field);

}  // namespace cb::dotnet::decompiler::csharp::auto_event_decompiler
