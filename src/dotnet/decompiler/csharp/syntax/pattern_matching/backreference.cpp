// dotnet/decompiler/csharp/syntax/pattern_matching/backreference.cpp -- see backreference.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/backreference.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

bool Backreference::match_one(INode* other, const Match& match) const {
    std::vector<INode*> found = match.get(referenced_group_name_);
    INode* last = found.empty() ? nullptr : found.back();
    if (last == nullptr) return other == nullptr;
    return is_match(last, other);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
