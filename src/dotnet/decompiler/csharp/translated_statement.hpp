// dotnet/decompiler/csharp/translated_statement.hpp -- the output of the C# StatementBuilder: a C# statement with the ILInstructions it
// came from.
//
//     TranslatedStatement s = with_il_instruction(make<ReturnStatement>(value), inst);   // see annotations.hpp
//     Statement* statement = s;
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: TranslatedStatement, from TranslatedStatement.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cassert>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/statement.hpp"
#include "dotnet/decompiler/csharp/translated_expression.hpp"

namespace cb::dotnet::decompiler::csharp {

using syntax::Statement;

struct TranslatedStatement {
    Statement* statement = nullptr;

    TranslatedStatement() = default;
    explicit TranslatedStatement(Statement* stmt) : statement(stmt) { assert(stmt != nullptr); }

    std::vector<il::ILInstruction*> il_instructions() const { return il_instructions_of(statement); }

    operator Statement*() const { return statement; }
};

}  // namespace cb::dotnet::decompiler::csharp
