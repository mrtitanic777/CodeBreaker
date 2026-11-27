// dotnet/decompiler/csharp/resolver/log.hpp -- the resolver's logging helper. Upstream wraps System.Diagnostics.Debug behind a compile-time
// switch that is off (the logging is a huge amount of output and slows the resolver down), so every call compiles away; here the functions
// are empty to keep the call sites of the port the same as upstream's.
//
//     log::write_line("Performing overload resolution");
//     log::indent();
//     log::unindent();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: Log, from Log.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
#pragma once

#include <string_view>

namespace cb::dotnet::decompiler::csharp::resolver::log {

// Whether logging is enabled (upstream's logEnabled constant).
inline constexpr bool log_enabled = false;

inline void write_line(std::string_view) {}
inline void indent() {}
inline void unindent() {}

}  // namespace cb::dotnet::decompiler::csharp::resolver::log
