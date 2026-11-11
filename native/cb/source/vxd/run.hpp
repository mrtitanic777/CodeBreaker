// cb/source/vxd/run.hpp -- the `vxd` command: a virtual device driver as the source its authors wrote.
//
// The command is a set of tools, each with its own command line (idiom, tree, masm, ...). The tools are registered by
// the modules that implement them, as a base::commands::ToolTable; this module owns the command's help text and the
// list of tool names, and hands the arguments on.
#pragma once

#include <array>
#include <span>
#include <string>
#include <string_view>

#include "cb/base/commands.hpp"

namespace cb::source::vxd {

// What the command says when its tool is missing or unknown.
inline constexpr std::string_view kVxdUsage = R"DOC(vxd -- a virtual device driver as the source its authors wrote: the driver kit's macros.

    codebreaker vxd <tool> [arguments]

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
writes it in the kit's macro form, which is how the Windows 95 tree's drivers are kept.)DOC";

// The tools the command offers.
inline constexpr std::array<std::string_view, 11> kVxdTools = {
    "idiom", "tree", "masm", "link", "from-source", "owbuild", "finalize", "disassembly", "corpus", "loadable-corpus", "boot"};

// args[0] names a tool of `tools`; the rest are its arguments, and its exit code is returned. With no tool or an
// unknown one, the usage and the registered tool names are written to stdout and cb::kExitUsage is returned.
int run(std::span<const std::string> args, const base::commands::ToolTable& tools);

}  // namespace cb::source::vxd
