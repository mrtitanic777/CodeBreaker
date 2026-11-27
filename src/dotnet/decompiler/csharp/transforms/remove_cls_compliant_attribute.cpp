// dotnet/decompiler/csharp/transforms/remove_cls_compliant_attribute.cpp -- see remove_cls_compliant_attribute.hpp.
#include "dotnet/decompiler/csharp/transforms/remove_cls_compliant_attribute.hpp"

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

void RemoveCLSCompliantAttribute::run(AstNode* root_node, TransformContext&) {
    for (AstNode* child : root_node->children()) {
        auto* section = as<AttributeSection>(child);
        if (section == nullptr) continue;
        if (section->attribute_target() == "assembly") continue;
        for (Attribute* attribute : section->attributes().to_vector()) {
            auto* trr = attribute->type()->annotation<semantics::TypeResolveResult>();
            if (trr != nullptr && trr->type()->full_name() == "System.CLSCompliantAttribute") attribute->remove();
        }
        if (section->attributes().size() == 0) section->remove();
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
