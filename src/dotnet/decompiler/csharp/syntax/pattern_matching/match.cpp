// dotnet/decompiler/csharp/syntax/pattern_matching/match.cpp -- see match.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/match.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

Match Match::create_new() {
    Match m;
    m.results_ = std::make_shared<Results>();
    return m;
}

std::size_t Match::check_point() const { return results_->size(); }

void Match::restore_check_point(std::size_t point) const {
    results_->erase(results_->begin() + static_cast<std::ptrdiff_t>(point), results_->end());
}

std::vector<INode*> Match::get(std::string_view group_name) const {
    std::vector<INode*> result;
    if (results_ == nullptr) return result;
    for (const auto& pair : *results_)
        if (pair.first == group_name) result.push_back(pair.second);
    return result;
}

bool Match::has(std::string_view group_name) const {
    if (results_ == nullptr) return false;
    for (const auto& pair : *results_)
        if (pair.first == group_name) return true;
    return false;
}

void Match::add(const std::string& group_name, INode* node) const {
    if (!group_name.empty() && node != nullptr) results_->emplace_back(group_name, node);
}

void Match::add_null(const std::string& group_name) const {
    if (!group_name.empty()) results_->emplace_back(group_name, nullptr);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
