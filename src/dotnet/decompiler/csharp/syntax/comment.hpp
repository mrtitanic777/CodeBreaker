// dotnet/decompiler/csharp/syntax/comment.hpp -- Comment: a comment attached as trivia to a node.
//
//     auto* c = make<Comment>(" the value", CommentType::SingleLine);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Comment, from GeneralScope/Comment.cs with the members ILSpy 11's source
// generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/trivia.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// What kind of comment a Comment is.
enum class CommentType {
    SingleLine,
    MultiLine,
    Documentation,
    InactiveCode,
    MultiLineDocumentation,
};

class Comment final : public Trivia {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Comment;
    Comment(const std::string& content_value, CommentType type_value = CommentType::SingleLine);
    Comment(CommentType comment_type_value, TextLocation start_location, TextLocation end_location);

    CommentType comment_type() const { return comment_type_; }
    void set_comment_type(CommentType value) { comment_type_ = value; }
    const std::string& content() const { return content_; }
    void set_content(std::string value) { content_ = std::move(value); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_comment(*this); }
    AstNode* shallow_clone() const override { return make<Comment>(*this); }

private:
    CommentType comment_type_{};
    std::string content_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
