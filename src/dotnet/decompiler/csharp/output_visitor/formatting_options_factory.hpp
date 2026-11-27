// dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp -- the predefined formatting styles (CSharpFormattingOptions).
//
//     CSharpFormattingOptions options = FormattingOptionsFactory::create_allman();   // what ILSpy writes with
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: FormattingOptionsFactory), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/output_visitor/csharp_formatting_options.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

struct FormattingOptionsFactory {
    // Empty options (everything off).
    static CSharpFormattingOptions create_empty();
    // Mono indent style.
    static CSharpFormattingOptions create_mono();
    static CSharpFormattingOptions create_sharp_develop();
    // K&R style: braces at the end of the line.
    static CSharpFormattingOptions create_kr_style();
    // Allman style: braces on their own lines, at the indentation of the construct.
    static CSharpFormattingOptions create_allman();
    // Whitesmiths style: braces on their own lines, indented with the body.
    static CSharpFormattingOptions create_whitesmiths();
    // GNU style.
    static CSharpFormattingOptions create_gnu();
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
