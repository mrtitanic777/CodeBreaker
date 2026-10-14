"""run.py -- a virtual device driver as the source its authors wrote: the driver kit's macros.

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

V2's own driver writer (`driver-source`) writes a driver as plain labelled assembly; this chain
writes it in the kit's macro form, which is how the Windows 95 tree's drivers are kept.
"""
from codebreaker.base import commands

TOOLS = {
    "idiom": "codebreaker.source.vxd.idiom",
    "tree": "codebreaker.source.vxd.tree",
    "masm": "codebreaker.source.vxd.masm",
    "link": "codebreaker.source.vxd.link",
    "from-source": "codebreaker.source.vxd.from_source",
    "owbuild": "codebreaker.source.vxd.owbuild",
    "finalize": "codebreaker.source.vxd.coalesce",
    "disassembly": "codebreaker.source.vxd.disassembly",
    "corpus": "codebreaker.source.vxd.corpus",
    "loadable-corpus": "codebreaker.source.vxd.loadable_corpus",
}


def main(argv):
    return commands.dispatch(TOOLS, argv, __doc__)
