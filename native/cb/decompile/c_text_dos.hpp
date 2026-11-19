// cb/decompile/c_text_dos.hpp -- a real-mode program's decompiled C made into C that OpenWatcom's 16-bit compiler accepts.
//
//     const c_text_dos::Result r = c_text_dos::compilable(text, c_text::header16());   wcc -ml r.text
//
// The decompiler writes each function of a DOS program in its own words: types it has no C name for (int2, xunknown2), far
// pointers as MK_FP(segment, (T *)offset), a piece of a variable as x._2_2_, memory outside the program as xRam000000fa,
// structures it recovered by name only (s_sub_06716_param_1), a declaration for every register it found read before it was
// written (most of them never used again), each function's parameters as its own instructions read them -- so one called with
// more arguments than it reads, or by callers that disagree, and one written `void` whose callers use what it leaves in AX.
// What no single function can settle is settled here, for the whole program at once:
//
//   the 16-bit header (header16: the types, CONCAT22, CARRY2, the conventions __cdecl16near ...) at the top
//   MK_FP(s, (T *)o)    ((T __far *)codebreaker_far(s, (uint2)(o)))    -- a register-only inline (DX:AX), no code of its own
//   x._N_M_             CODEBREAKER_PIECEM(x, N): the M bytes of x from byte N
//   xRam000000fa        a far reference to that linear address (0:FA, an interrupt vector), declared once
//   *(T *)0x5cd4        g_5cd4, the word of the data segment at 5CD4h, declared once with the type most of its uses give it
//   s_...               each structure declared: a word at every offset a field is read at, padding between
//   in_BX, unaff_SI     a declaration no statement uses is dropped
//   void f() used       returns a word
//   arguments           every caller passing N: f takes N; callers disagreeing: f is defined the old way (as C of 1986 was),
//                       and declared without a prototype
//   a prototype         for every function, ahead of all of them; an undeclared helper (int21, func_0x...) declared unprototyped
//
// Nothing here changes what a statement computes. The result is C that compiles function by function; what it does is the
// judge's to establish.
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::decompile::c_text_dos {

struct Report {
    std::size_t functions = 0;
    std::size_t registers_dropped = 0;   // declarations of registers no statement uses
    std::size_t structures = 0;          // structures declared
    std::size_t globals = 0;             // data-segment words named g_...
    std::size_t old_style = 0;           // functions defined the old way: their callers disagree on the arguments
    std::size_t widened = 0;             // functions given the arguments all their callers pass
    std::size_t strings = 0;             // pointers into the data segment shown with the string they point at
};

struct Result {
    std::string text;
    Report report;
    std::size_t placed = 0;   // functions placed in the source file they were compiled from (Layout)
};

// What the program says of its own source files. A program built from several C files and linked with Microsoft's LINK keeps
// each file's initialised data in the order the files were linked, and many such programs carry a version string of each
// file in its data (SCCS: `@(#)lex.c:1.9`). In the large and medium models each file's code is a code segment of its own.
// So a code segment whose functions read the data between one file's string and the next is that file's code.
struct Layout {
    std::vector<std::pair<std::uint32_t, std::string>> files;   // (offset in the data segment of its version string, the file), ascending
    std::map<std::string, std::uint32_t> segment_of;            // function name -> the code segment it is in
    std::string data_segment;                                   // its segment as the code writes it, "0x2a38" (empty: not known)
    std::string data;                                           // the data segment's bytes (up to 64 KB): a pointer into it that
                                                                // starts a string gets the string beside it, `/* "MSC_CMD_FLAGS" */`
};

// `text` is the program's C as the decompiler wrote it (Decompiler::as_c): its first lines (the title, the #includes of the
// library functions) and then the functions. `header` is the 16-bit header (c_text::header16()). With a layout, the functions
// are written file by file (`/* ---- lex.c ---- */`), each where the voting places its segment.
Result compilable(std::string_view text, std::string_view header, const Layout& layout = {});

}  // namespace cb::decompile::c_text_dos
