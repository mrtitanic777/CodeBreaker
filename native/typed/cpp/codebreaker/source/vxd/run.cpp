// codebreaker/source/vxd/run.cpp -- C++ from codebreaker.source.vxd.run.

#include "codebreaker/source/vxd/run.hpp"

namespace codebreaker::source::vxd::run {

const py::str DOC = py::str(R"DOC(run.py -- a virtual device driver as the source its authors wrote: the driver kit's macros.

    python -m codebreaker vxd <tool> [arguments]

    idiom FILE                    the driver as kit-macro source (Declare_Virtual_Device,
                                  Begin_Control_Dispatch, VMMcall, BeginProc ...), to the screen
    idiom FOLDER [OUT]            every driver of a folder, one source tree each
    idiom --verify [FOLDER]       does every one assemble?
    tree FILE [OUT]               the driver as one source file per segment, built back object by object
    masm FILE                     assemble a driver's objects and compare them with the original
    link FILE [NAME]              the whole driver rebuilt with ML and LINK, and compared
    from-source FILE [OUT]        a loadable driver from its kit-macro source
    owbuild ...                   the same with OpenWatcom's assembler and linker only
    finalize BUILT ORIGINAL OUT   what the loader of Windows 95 needs of a linked driver that the
                                  linkers leave out: fix-ups grouped as Microsoft's LINK groups them,
                                  the real-mode entry point, the module's flags and version resource
    disassembly FILE              the driver's code as the reader sees it
    corpus FOLDER RESULTS         every driver of a folder through `masm`
    loadable-corpus FOLDER RESULTS   every driver of a folder through `from-source`
    boot [--drivers N,...] [--vmm32] [--retail] [--bisect]
                                  the drivers rebuilt and put into a copy of an installed
                                  Windows 95 disk, booted in QEMU: does it reach the desktop?

V2's own driver writer (`driver-source`) writes a driver as plain labelled assembly; this chain
writes it in the kit's macro form, which is how the Windows 95 tree's drivers are kept.
)DOC");

const py::dict<py::str, py::str> TOOLS = {
    {py::str("idiom"), py::str("codebreaker.source.vxd.idiom")},
    {py::str("tree"), py::str("codebreaker.source.vxd.tree")},
    {py::str("masm"), py::str("codebreaker.source.vxd.masm")},
    {py::str("link"), py::str("codebreaker.source.vxd.link")},
    {py::str("from-source"), py::str("codebreaker.source.vxd.from_source")},
    {py::str("owbuild"), py::str("codebreaker.source.vxd.owbuild")},
    {py::str("finalize"), py::str("codebreaker.source.vxd.coalesce")},
    {py::str("disassembly"), py::str("codebreaker.source.vxd.disassembly")},
    {py::str("corpus"), py::str("codebreaker.source.vxd.corpus")},
    {py::str("loadable-corpus"), py::str("codebreaker.source.vxd.loadable_corpus")},
    {py::str("boot"), py::str("codebreaker.source.vxd.boot")},
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

}  // namespace codebreaker::source::vxd::run
