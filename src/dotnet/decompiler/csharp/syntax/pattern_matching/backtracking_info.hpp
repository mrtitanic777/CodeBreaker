// dotnet/decompiler/csharp/syntax/pattern_matching/backtracking_info.hpp -- the state of matching one list of children: the stack of
// places a list match can resume from when what follows fails (Repeat and OptionalNode push to it, Pattern::match_collections pops).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: BacktrackingInfo and Pattern.PossibleMatch, from
// BacktrackingInfo.cs and Pattern.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstddef>
#include <vector>

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

// One place to resume from: matching continues at the pattern child after the one that pushed it.
struct PossibleMatch {
    std::size_t next_other_index;  // index of the next element of the matched list after the last one matched
    std::size_t checkpoint;        // the Match's check point at that place
};

class BacktrackingInfo {
public:
    std::vector<PossibleMatch> backtracking_stack;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
