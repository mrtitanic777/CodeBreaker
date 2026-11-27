// dotnet/decompiler/csharp/syntax/comment.cpp -- see comment.hpp.
#include "dotnet/decompiler/csharp/syntax/comment.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Comment::Comment(const std::string& content_value, CommentType type_value) {
    set_comment_type(type_value);
    set_content(content_value);
}

Comment::Comment(CommentType comment_type_value, TextLocation start_location, TextLocation end_location) : Trivia(start_location, end_location) {
    set_comment_type(comment_type_value);
}

bool Comment::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<Comment>(other);
    return o != nullptr
           && comment_type() == o->comment_type()
           && match_string(content(), o->content());
}

int Comment::get_child_count() const {
    return 0;
}

AstNode* Comment::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void Comment::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& Comment::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void Comment::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
