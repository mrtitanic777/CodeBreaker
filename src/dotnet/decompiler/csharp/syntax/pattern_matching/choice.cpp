// dotnet/decompiler/csharp/syntax/pattern_matching/choice.cpp -- see choice.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/choice.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

void Choice::add(const std::string& name, INode* alternative) {
    if (alternative == nullptr) throw std::invalid_argument("alternative");
    alternatives_.push_back(il::make<NamedNode>(name, alternative));
}

void Choice::add(INode* alternative) {
    if (alternative == nullptr) throw std::invalid_argument("alternative");
    alternatives_.push_back(alternative);
}

bool Choice::match_one(INode* other, const Match& match) const {
    std::size_t check_point = match.check_point();
    for (INode* alt : alternatives_) {
        if (alt->match_one(other, match)) return true;
        match.restore_check_point(check_point);
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
