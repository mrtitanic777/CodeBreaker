// dotnet/decompiler/csharp/csharp_language_version.hpp -- the C# language versions, under the name the C# stages use.
//
//     csharp::LanguageVersion v = csharp::LanguageVersion::CSharp7_3;
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: LanguageVersion, from CSharpLanguageVersion.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). The enum itself was ported with the settings it gates (decompiler_settings.hpp,
// namespace cb::dotnet::decompiler, same values as upstream including Preview and Latest); this header makes it visible as
// csharp::LanguageVersion.
#pragma once

#include "dotnet/decompiler/decompiler_settings.hpp"

namespace cb::dotnet::decompiler::csharp {

using decompiler::LanguageVersion;

}  // namespace cb::dotnet::decompiler::csharp
