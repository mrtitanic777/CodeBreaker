// dotnet/decompiler/csharp/transforms/custom_patterns.cpp -- see custom_patterns.hpp.
#include "dotnet/decompiler/csharp/transforms/custom_patterns.hpp"

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern_extensions.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using syntax::pattern_matching::AnyNode;
using syntax::pattern_matching::INode;
using syntax::pattern_matching::Match;

bool TypePattern::match_one(INode* other, const Match&) const {
    ComposedType* ct = as<ComposedType>(other);
    AstType* o;
    if (ct != nullptr && !ct->has_ref_specifier() && !ct->has_nullable_specifier() && ct->pointer_rank() == 0 &&
        ct->array_specifiers().empty()) {
        // Special case: ILSpy sometimes produces a ComposedType but then removed all array specifiers
        // from it. In that case, we need to look at the base type for the annotations.
        o = ct->base_type();
    } else {
        o = as<AstType>(other);
        if (o == nullptr) return false;
    }
    auto* trr = dynamic_cast<semantics::TypeResolveResult*>(get_resolve_result(o));
    return trr != nullptr && trr->type()->namespace_() == ns_ && trr->type()->name() == name_;
}

LdTokenPattern::LdTokenPattern(const std::string& group_name) : child_node_(make<AnyNode>(group_name)) {}

bool LdTokenPattern::match_one(INode* other, const Match& match) const {
    InvocationExpression* ie = as<InvocationExpression>(other);
    if (ie != nullptr && ie->annotation<LdTokenAnnotation>() != nullptr && ie->arguments().count() == 1) {
        return child_node_->match_one(ie->arguments()[0], match);
    }
    return false;
}

TypeOfPattern::TypeOfPattern(const std::string& group_name) {
    using syntax::pattern_matching::to_type;
    auto* type_reference = make<TypeReferenceExpression>();
    type_reference->set_type(to_type(make<TypePattern>("System", "Type")));
    auto* get_type_from_handle = make<MemberReferenceExpression>(type_reference, "GetTypeFromHandle");
    auto* type_of = make<TypeOfExpression>(to_type(make<AnyNode>(group_name)));
    auto* invocation = make<InvocationExpression>(get_type_from_handle, std::initializer_list<Expression*>{type_of});
    child_node_ = make<MemberReferenceExpression>(invocation, "TypeHandle");
}

bool TypeOfPattern::match_one(INode* other, const Match& match) const { return child_node_->match_one(other, match); }

}  // namespace cb::dotnet::decompiler::csharp::transforms
