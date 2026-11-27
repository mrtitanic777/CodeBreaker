// dotnet/decompiler/instrumentation/decompiler_event_source.hpp -- performance tracing of the decompilation pipeline: events at the start and
// end of a type, a member, a project, a file, a transform. Nobody listens in this port (upstream's listeners are ETW and EventPipe), so every
// event is a call that does nothing; the callers stay as upstream has them.
//
//     DecompilerEventSource::log().project_decompilation_start(module_name);
//     if (DecompilerEventSource::log().is_enabled()) ...      // false
//
// Ported from ILSpy (ICSharpCode.Decompiler.Instrumentation: DecompilerEventSource, DecompiledMemberKind, from DecompilerEventSource.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Only the events that take plain values are here; the overloads that take
// an entity or a transform only compute these values for a listener, so a caller that wants them calls the plain event with the values (names,
// tokens). If a listener is ever wanted, it goes behind set_listener() (not present: nothing needs it).
#pragma once

#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::instrumentation {

// Member kind reported in the DecompileMemberStart/Stop events.
enum class DecompiledMemberKind { Method = 1, Field = 2, Property = 3, Event = 4 };

class DecompilerEventSource {
public:
    static DecompilerEventSource& log() {
        static DecompilerEventSource source;
        return source;
    }
    // Whether anyone listens (nobody does).
    bool is_enabled() const { return false; }
    bool is_transform_tracing_enabled() const { return false; }

    void decompile_type_start(std::string_view) {}
    void decompile_type_stop(std::string_view) {}
    void decompile_member_start(std::string_view, int, int, int) {}
    void decompile_member_stop(std::string_view, int, int) {}
    void type_system_init_start(std::string_view) {}
    void type_system_init_stop(std::string_view, int) {}
    void assembly_resolve_start(std::string_view) {}
    void assembly_resolve_stop(std::string_view, std::string_view, bool) {}
    void project_decompilation_start(std::string_view) {}
    void project_decompilation_stop(std::string_view, int, int) {}
    void project_file_start(std::string_view, int) {}
    void project_file_stop(std::string_view) {}
    void il_transform_executed(std::string_view, int, double) {}
    void ast_transform_executed(std::string_view, double) {}

private:
    DecompilerEventSource() = default;
};

}  // namespace cb::dotnet::decompiler::instrumentation
