#include "cb/source/ne_asm/run.hpp"

#include <array>

namespace cb::source::ne_asm::run {

std::string_view usage() {
    return R"DOC(the hand-written assembly of a 16-bit module as the source its authors wrote.

    codebreaker ne-asm <tool> [arguments]

    idiom MODULE SEG LO HI          a range of a code segment as Microsoft SDK cMacros source (cProc,
                                    cBegin, cEnd), assembled and checked against the original;
                                    HI may be "whole". NE_SAVE=FILE keeps the source
    sweep [--limit N] [--emit DIR]  every code segment of the 16-bit corpus through `idiom`
    recon MODULE [--seg N]          a segment as assembly that reassembles to the same bytes
    segdiff C --module M --seg N --off X --size N --func F
                                    compile a function group and compare it with the original
    reachability MODULE [--seg N]   which bytes of a segment are code
    datacode MODULE [--seg N]       which bytes of a segment are data
    segment ...  /  fixups ...      the segment and fix-up readers the others stand on
    debug-names                     real internal names of KRNL386 from Microsoft's debug build
                                    of it (the symbol map of WIN32SDK\SCT\DEBUG16), checked against
                                    its exports; written where `idiom` picks them up
    debug-names-estimate            names for what the exact match leaves anonymous, as estimates
    msvc152 [C] [--flags F]         compile with Microsoft C/C++ 8 (Visual C++ 1.52c) and show
                                    its listing

The 16-bit kernel, USER and GDI were written largely in assembly with the SDK's cMacros; this is
that form, not a disassembly listing: every place a loader patches is a label, and each range
is reassembled (MASM, or a MASM-compatible NASM -- settings: masm, nasm) and compared.)DOC";
}

std::span<const std::string_view> tool_names() {
    static constexpr std::array<std::string_view, 11> names = {"idiom", "sweep", "recon", "segdiff", "reachability", "datacode",
                                                               "segment", "fixups", "debug-names", "debug-names-estimate", "msvc152"};
    return names;
}

int main(const base::commands::ToolTable& tools, std::span<const std::string> args) {
    return base::commands::dispatch(tools, args, usage());
}

}  // namespace cb::source::ne_asm::run
