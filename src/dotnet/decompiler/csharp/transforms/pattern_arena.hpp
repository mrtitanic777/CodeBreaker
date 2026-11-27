// dotnet/decompiler/csharp/transforms/pattern_arena.hpp -- where the static patterns of the AST transforms live. Upstream's transforms keep
// their patterns in `static readonly` fields; syntax nodes here belong to the thread's current ILArena (which is freed with the method
// being decompiled), so a pattern that outlives one decompilation is made in an arena of its own that is never freed.
//
//     static AstNode* const pattern = build_pattern([] { return make<ExpressionStatement>(...); });     // made once, on first use
//     Match m = match(pattern, node);
//     IdentifierExpression* id = single<IdentifierExpression>(m, "variable");                           // upstream's m.Get<T>("x").Single()
//
// build_pattern makes the arena current for the call (under a lock: arenas are not thread safe and two threads may build different
// patterns at once). The patterns are only matched against, never attached to a tree.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: the static readonly patterns, Match.Get<T>(...).Single()), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/match.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

namespace detail {
inline il::ILArena& pattern_arena() {
    static il::ILArena arena;
    return arena;
}
inline std::mutex& pattern_arena_mutex() {
    static std::mutex mutex;
    return mutex;
}
}  // namespace detail

// Calls `build` (which makes nodes with make<T>) with the process-wide pattern arena current, and returns what it returns.
template <class F>
auto build_pattern(F&& build) {
    std::lock_guard<std::mutex> lock(detail::pattern_arena_mutex());
    il::ILArenaScope scope(detail::pattern_arena());
    return build();
}

// The one node of group `name` that the match found (upstream's `m.Get<T>(name).Single()`).
// std::logic_error when the group does not have exactly one node.
template <class T>
T* single(const syntax::pattern_matching::Match& m, const std::string& name) {
    std::vector<T*> nodes = m.get<T>(name);
    if (nodes.size() != 1) throw std::logic_error("Sequence does not contain exactly one element");
    return nodes[0];
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
