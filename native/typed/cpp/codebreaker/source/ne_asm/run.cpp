// codebreaker/source/ne_asm/run.cpp -- C++ from codebreaker.source.ne_asm.run.

#include "codebreaker/source/ne_asm/run.hpp"

namespace codebreaker::source::ne_asm::run {

const py::str DOC = py::str(R"DOC(run.py -- the hand-written assembly of a 16-bit module as the source its authors wrote.

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
                                    of it (the symbol map of WIN32SDK\SCT\DEBUG16), checked against
                                    its exports; written where `idiom` picks them up
    debug-names-estimate            names for what the exact match leaves anonymous, as estimates
    msvc152 [C] [--flags F]         compile with Microsoft C/C++ 8 (Visual C++ 1.52c) and show
                                    its listing

The 16-bit kernel, USER and GDI were written largely in assembly with the SDK's cMacros; this is
that form, not a disassembly listing: every place a loader patches is a label, and each range
is reassembled (MASM, or a MASM-compatible NASM -- settings: masm, nasm) and compared.
)DOC");

const py::dict<py::str, py::str> TOOLS = {
    {py::str("idiom"), py::str("codebreaker.source.ne_asm.idiom")},
    {py::str("sweep"), py::str("codebreaker.source.ne_asm.sweep")},
    {py::str("recon"), py::str("codebreaker.source.ne_asm.recon")},
    {py::str("segdiff"), py::str("codebreaker.source.ne_asm.segdiff")},
    {py::str("reachability"), py::str("codebreaker.source.ne_asm.reachability")},
    {py::str("datacode"), py::str("codebreaker.source.ne_asm.datacode")},
    {py::str("segment"), py::str("codebreaker.source.ne_asm.segment")},
    {py::str("fixups"), py::str("codebreaker.source.ne_asm.fixups")},
    {py::str("debug-names"), py::str("codebreaker.symbols.debug_build")},
    {py::str("debug-names-estimate"), py::str("codebreaker.symbols.debug_build_estimate")},
    {py::str("msvc152"), py::str("codebreaker.build.msvc152")},
};

namespace {

// codebreaker.base.commands.dispatch, with run_module handed in (see RunModule in the header).
int64_t dispatch(const py::dict<py::str, py::str>& tools, const py::list<py::str>& argv, const py::str& doc,
                 const RunModule& run_module) {
    if (py::len(argv) == 0 || !tools.contains(argv[0])) {
        std::cout << doc.utf8() << "\n";
        py::list<py::str> names = py::sorted(tools.keys());
        std::cout << (py::str("tools: ") + py::join(py::str(", "), names)).utf8() << "\n";
        return 2;
    }
    return run_module(tools.at(argv[0]), argv.slice_from(1));
}

}  // namespace

int64_t main(const py::list<py::str>& argv, const RunModule& run_module) {
    return dispatch(TOOLS, argv, DOC, run_module);
}

}  // namespace codebreaker::source::ne_asm::run
