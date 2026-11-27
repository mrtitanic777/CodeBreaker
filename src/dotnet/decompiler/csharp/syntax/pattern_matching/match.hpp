// dotnet/decompiler/csharp/syntax/pattern_matching/match.hpp -- the result of matching a pattern against a tree: success or failure, and
// the nodes the pattern named (NamedNode / AnyNode groups), in the order they were matched.
//
//     Match m = match(pattern, node);                      // pattern_matching/inode.hpp
//     if (m.success()) {
//         Expression* x = m.get<Expression>("x").front();  // every node the group "x" captured
//     }
//
// A Match is a handle on a shared list of results: copies of one Match see each other's additions (upstream's struct holds a reference
// to a List), so the match functions take `const Match&` and add through it. A default-constructed Match is the failure (success() is
// false and it holds nothing); Match::create_new() starts a match that is under way. Group names are std::string, the empty string
// being upstream's null group (not recorded).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: Match, from Match.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class INode;

class Match {
public:
    // The failure: no results, success() false.
    Match() = default;

    // Whether the match succeeded (upstream: the results list exists).
    bool success() const { return results_ != nullptr; }

    // A match that is under way, with no results yet.
    static Match create_new();

    // The number of results so far, to go back to with restore_check_point when an alternative fails (backtracking).
    std::size_t check_point() const;
    void restore_check_point(std::size_t point) const;

    // The nodes recorded under a group name, in the order they were added (null for a group that matched nothing: AnyNodeOrNull).
    std::vector<INode*> get(std::string_view group_name) const;

    // The same, each as a T (a node class); throws std::bad_cast for a node of another type.
    template <class T>
    std::vector<T*> get(std::string_view group_name) const {
        std::vector<T*> result;
        for (INode* node : get(group_name)) {
            T* typed = dynamic_cast<T*>(node);
            if (typed == nullptr && node != nullptr) throw std::bad_cast();
            result.push_back(typed);
        }
        return result;
    }

    bool has(std::string_view group_name) const;

    // Records `node` under the group; nothing is recorded for an empty group name or a null node.
    void add(const std::string& group_name, INode* node) const;
    // Records that the group matched nothing (null), unless the group name is empty.
    void add_null(const std::string& group_name) const;

private:
    using Results = std::vector<std::pair<std::string, INode*>>;
    std::shared_ptr<Results> results_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
