// dotnet/decompiler/csharp/syntax/tokens.hpp -- the text of the punctuation and keywords the syntax tree and its writer use most.
//
//     output.write(Tokens::l_par);        // "("
//
// Names are upstream's in snake_case (`Tokens.LPar` is `Tokens::l_par`).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Tokens, from Tokens.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#pragma once

#include <string_view>

namespace cb::dotnet::decompiler::csharp::syntax {

struct Tokens {
    // some predefined constants for the most used punctuation
    static constexpr std::string_view l_par = "(";
    static constexpr std::string_view r_par = ")";
    static constexpr std::string_view l_bracket = "[";
    static constexpr std::string_view r_bracket = "]";
    static constexpr std::string_view l_brace = "{";
    static constexpr std::string_view r_brace = "}";
    static constexpr std::string_view l_chevron = "<";
    static constexpr std::string_view r_chevron = ">";
    static constexpr std::string_view comma = ",";
    static constexpr std::string_view dot = ".";
    static constexpr std::string_view semicolon = ";";
    static constexpr std::string_view assign = "=";
    static constexpr std::string_view colon = ":";
    static constexpr std::string_view double_colon = "::";
    static constexpr std::string_view arrow = "=>";

    static constexpr std::string_view where_keyword = "where";
    static constexpr std::string_view delegate_keyword = "delegate";
    static constexpr std::string_view extern_keyword = "extern";
    static constexpr std::string_view alias_keyword = "alias";
    static constexpr std::string_view namespace_keyword = "namespace";

    static constexpr std::string_view enum_keyword = "enum";
    static constexpr std::string_view interface_keyword = "interface";
    static constexpr std::string_view struct_keyword = "struct";
    static constexpr std::string_view class_keyword = "class";
    static constexpr std::string_view record_keyword = "record";
    static constexpr std::string_view record_struct_keyword = "record";
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
