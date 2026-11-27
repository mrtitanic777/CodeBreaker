// dotnet/decompiler/csharp/syntax/identifier.hpp -- Identifier, the node of one name token (a variable, a member, a type name ...). The
// nodes that have a name keep an Identifier child (`name_token`) and offer the name as a string.
//
//     Identifier* id = Identifier::create("value");             // "@class" makes the name "class", written with the @
//     id->name();   id->set_name("other");
//     auto* var = Identifier::create_if_not_empty(name);        // nullptr for no name: for the optional name slots
//
// Where upstream tells an absent optional name (null) from an empty one, a std::string here does not: nodes read an absent name token
// as the empty string and an empty name set as no token.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Identifier, from Identifier.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class Identifier final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Identifier;

    // An identifier with an empty name and no location. (Make named ones with create.)
    Identifier() = default;
    // An identifier of exactly this name (a leading @ is part of the name; create strips it and marks the identifier verbatim).
    Identifier(std::string name_text, TextLocation location) : name_(std::move(name_text)), token_start_(location) {}

    // The name as written in the C#, without a verbatim @.
    const std::string& name() const { return name_; }
    void set_name(std::string value) { name_ = std::move(value); }

    // Whether the name is written with a leading @ (a lexical detail: not part of what a pattern compares).
    bool is_verbatim() const { return is_verbatim_; }
    void set_is_verbatim(bool value) { is_verbatim_ = value; }

    // The start is the one given at creation; the end follows from the name's length (in UTF-16 units, as .NET counts it).
    TextLocation start_location() const override { return token_start_; }
    TextLocation end_location() const override;
    void set_start_location(TextLocation value) { token_start_ = value; }

    // An identifier of `name_text` at `location`. A leading @ is dropped from the name, the location moves one column right to the
    // name, and the identifier is verbatim; an empty name gives an empty identifier.
    static Identifier* create(const std::string& name_text, TextLocation location = TextLocation());
    // The same, saying whether the name is verbatim (the name is taken as it is).
    static Identifier* create(const std::string& name_text, TextLocation location, bool is_verbatim);
    // An identifier for an optional name: nullptr when the name is empty.
    static Identifier* create_if_not_empty(const std::string& name_text);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_identifier(*this); }
    AstNode* shallow_clone() const override { return make<Identifier>(*this); }

private:
    std::string name_;
    TextLocation token_start_;
    bool is_verbatim_ = false;
};

// The length of UTF-8 text in UTF-16 code units, which is what .NET's string.Length counts (end locations are computed from it).
int utf16_length(const std::string& utf8);

}  // namespace cb::dotnet::decompiler::csharp::syntax
