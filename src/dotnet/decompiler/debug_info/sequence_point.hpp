// dotnet/decompiler/debug_info/sequence_point.hpp -- a sequence point: a range of IL offsets and the source text (lines and columns) it
// came from, as read from a PDB or produced by the decompiler.
//
//     debug_info::SequencePoint sp;
//     sp.offset = 0; sp.end_offset = 12; sp.start_line = 3; sp.start_column = 5; sp.end_line = 3; sp.end_column = 20;
//     sp.is_hidden();                      // false; set_hidden() makes the line 0xfeefee
//
// Ported from ILSpy (ICSharpCode.Decompiler.DebugInfo: SequencePoint, from SequencePoint.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Upstream's SequencePoint is a class; here it is a value, and code that changed a
// point held in a list changes it in the list.
#pragma once

#include <string>

namespace cb::dotnet::decompiler::debug_info {

struct SequencePoint {
    // IL start offset.
    int offset = 0;
    // IL end offset. This does not get stored in debug information; it is used internally to create hidden sequence points for the IL
    // fragments not covered by any sequence point.
    int end_offset = 0;

    int start_line = 0;
    int start_column = 0;
    int end_line = 0;
    int end_column = 0;

    std::string document_url;   // empty: none

    bool is_hidden() const { return start_line == 0xfeefee && start_line == end_line; }
    void set_hidden() { start_line = end_line = 0xfeefee; }
};

}  // namespace cb::dotnet::decompiler::debug_info
