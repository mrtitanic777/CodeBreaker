// dotnet/decompiler/csharp/syntax/primitive_type.hpp -- PrimitiveType: a type written as a C# keyword (`int`, `string`, `void` ...).
//
//     auto* t = make<PrimitiveType>("int");
//     t->known_type_code();                                   // KnownTypeCode::Int32
//     PrimitiveType::get_type_code_for_primitive_type("long");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: PrimitiveType, from PrimitiveType.cs and the members generated for it),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <string_view>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class PrimitiveType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::PrimitiveType;

    PrimitiveType() = default;
    explicit PrimitiveType(std::string keyword_text) : keyword_(std::move(keyword_text)) {}
    PrimitiveType(std::string keyword_text, TextLocation location) : keyword_(std::move(keyword_text)) { store_print_start(location); }

    // The keyword ("int").
    const std::string& keyword() const { return keyword_; }
    void set_keyword(std::string value) { keyword_ = std::move(value); }

    // The type the keyword stands for (None for a word that is not a type keyword); derived from the keyword.
    typesystem::KnownTypeCode known_type_code() const { return get_type_code_for_primitive_type(keyword_); }

    // The start is the printed start (from the base); the end follows from the keyword's length.
    TextLocation end_location() const override;

    // A primitive type writes as its keyword.
    std::string to_string(const output_visitor::CSharpFormattingOptions* formatting_options = nullptr) const override;

    // The known type a type keyword stands for ("long": Int64), or None.
    static typesystem::KnownTypeCode get_type_code_for_primitive_type(std::string_view keyword_text);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_primitive_type(*this); }
    AstNode* shallow_clone() const override { return make<PrimitiveType>(*this); }

private:
    std::string keyword_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
