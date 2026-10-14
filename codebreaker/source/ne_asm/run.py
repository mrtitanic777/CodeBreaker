"""run.py -- the hand-written assembly of a 16-bit module as the source its authors wrote.

    python -m codebreaker ne-asm <tool> [arguments]

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
                                    of it (the symbol map of WIN32SDK\\SCT\\DEBUG16), checked against
                                    its exports; written where `idiom` picks them up
    debug-names-estimate            names for what the exact match leaves anonymous, as estimates
    msvc152 [C] [--flags F]         compile with Microsoft C/C++ 8 (Visual C++ 1.52c) and show
                                    its listing

The 16-bit kernel, USER and GDI were written largely in assembly with the SDK's cMacros; this is
that form, not a disassembly listing: every place a loader patches is a label, and each range
is reassembled (MASM, or a MASM-compatible NASM -- settings: masm, nasm) and compared.
"""
from codebreaker.base import commands

TOOLS = {
    "idiom": "codebreaker.source.ne_asm.idiom",
    "sweep": "codebreaker.source.ne_asm.sweep",
    "recon": "codebreaker.source.ne_asm.recon",
    "segdiff": "codebreaker.source.ne_asm.segdiff",
    "reachability": "codebreaker.source.ne_asm.reachability",
    "datacode": "codebreaker.source.ne_asm.datacode",
    "segment": "codebreaker.source.ne_asm.segment",
    "fixups": "codebreaker.source.ne_asm.fixups",
    "debug-names": "codebreaker.symbols.debug_build",
    "debug-names-estimate": "codebreaker.symbols.debug_build_estimate",
    "msvc152": "codebreaker.build.msvc152",
}


def main(argv):
    return commands.dispatch(TOOLS, argv, __doc__)
