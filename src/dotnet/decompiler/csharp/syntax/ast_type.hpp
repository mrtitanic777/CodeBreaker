// dotnet/decompiler/csharp/syntax/ast_type.hpp -- AstType, the base of every C# type node (SimpleType, MemberType, PrimitiveType,
// ComposedType, TupleAstType, FunctionPointerAstType, InvocationAstType). It also builds the composed forms (pointer, array, nullable,
// ref) around a type and the type of a dotted name.
//
//     AstType* t = AstType::create("System.Collections.Generic.List");     // a MemberType chain over a SimpleType
//     AstType* pointer = t->make_pointer_type();                           // `T*`: a ComposedType around t
//     bool is_var = t->is_var();                                           // a plain `var`
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AstType, from AstType.cs and the pattern placeholder generated into it),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

#include "dotnet/decompiler/csharp/resolver/name_lookup_mode.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class AstType : public AstNode {
public:
    // A deep copy of this subtree, unattached (AstNode::clone, typed).
    AstType* clone() const { return static_cast<AstType*>(AstNode::clone()); }

    // The node that stands for `pattern` where a type goes (upstream's implicit conversion Pattern -> AstType).
    static AstType* from_pattern(pattern_matching::Pattern* pattern);

    // Whether this is a plain `var` (a SimpleType named var without type arguments).
    bool is_var() const;

    // How a name in this type is looked up: in a using, as the base type of a type declaration (or of a constraint on one), or as a
    // plain type.
    resolver::NameLookupMode get_name_lookup_mode() const;

    // This type as a pointer / array / nullable / ref type: a ComposedType around it (ComposedType itself changes in place where it can).
    virtual AstType* make_pointer_type();
    virtual AstType* make_array_type(int rank = 1);
    AstType* make_nullable_type();
    virtual AstType* make_ref_type();

    // `this.member_name<type_arguments>`: a MemberType with this type as its target.
    MemberType* member_type(const std::string& member_name, const std::vector<AstType*>& type_arguments = {});

    // The type of a dotted name: `A.B.C` is MemberType(MemberType(SimpleType(A), B), C).
    static AstType* create(std::string_view dotted_name);

protected:
    AstType() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
