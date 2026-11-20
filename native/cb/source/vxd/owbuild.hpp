// cb/source/vxd/owbuild.hpp -- a ring-0 VxD built from the recovered source with OpenWatcom's tools and no Microsoft ones: wasm or nasm
// (assemble) and wlink `system win_vxd` (link).
//
// The per-segment source of tree.hpp is written UNSPLIT (one file per LE object, so every branch target is inside its file), each file is
// assembled to an object, and the objects are linked into an LE VxD with the DDB as its single required export (`export VXD_DDB.1`).
// A byte-exact build writes raw the instructions the assembler re-encodes differently from the original (the force-raw set of the
// object-level test) and assembles with nasm in MASM mode, the assembler that set was verified against; a buildable one converges each
// file against wasm and may drift.
//
//     const owbuild::Result r = owbuild::build_vxd(env, driver, "VKD", out_folder);       // -> <out>/vkd/vkd.vxd
//
//     codebreaker vxd owbuild VXD [--out DIR]
//
// 211 of the 215 standalone ring-0 drivers of the Windows 95 corpus build into valid LE binaries this way.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/bytes.hpp"
#include "cb/formats/le.hpp"
#include "cb/source/vxd/disassembly.hpp"
#include "cb/source/vxd/link.hpp"

namespace cb::source::vxd::owbuild {

// Where the tools are. The OpenWatcom tree is the folder `wasm` and `wlink` sit in, as the tree is laid out: <tree>/bld/wasm/nt386/wasm.exe
// and <tree>/rel/binnt64/wlink.exe.
struct Tools {
    std::filesystem::path wasm, wlink;
    std::filesystem::path watcom;   // the tree's rel folder: the WATCOM variable, and its binnt64 first on PATH
};

// The tools of an OpenWatcom tree given by the `openwatcom` setting (its rel folder; empty: experiments/premise-proof/open-watcom-v2 of
// the checkout). The byte-exact build assembles with x86asm, in this process.
Tools tools_from_settings(const std::filesystem::path& checkout_root, std::string_view openwatcom_setting);

struct Environment {
    const link::Environment& link;   // the object-level test that finds the force-raw set of a byte-exact build
    Tools tools;
};

struct Result {
    bool ok = false;
    std::filesystem::path output;    // the .vxd
    std::size_t bytes = 0, objects = 0;
    std::string error;               // why not, when not
};

// Assemble and link one driver's recovered source. `name` is the driver's name; the folder is <out>/<name in lower case>.
Result build_vxd(const Environment& env, const formats::le::Module& driver, std::string_view name, const std::filesystem::path& out,
                 bool byte_exact = true);

// The components of a W3 bundle header at the LE offset of its DOS header: (name, offset, size) each. Empty for a file that is not a bundle.
struct Component {
    std::string name;
    std::uint32_t offset = 0, size = 0;
};
std::vector<Component> w3_components(ByteView data);

// A W3 bundle (vmm32.vxd) with each component rebuilt from its recovered source and the bundle packed again: the real-mode front is
// kept (it is a loader, not driver source), the header and records are written again, each module page-aligned.
struct BundleResult {
    bool ok = false;
    std::filesystem::path output;
    std::size_t bytes = 0;
    std::vector<std::pair<std::string, bool>> components;   // each component and whether it built
    std::string error;
};
BundleResult build_w3(const Environment& env, const std::filesystem::path& bundle, const std::filesystem::path& out);

struct CommandEnvironment {
    const Environment& build;
    const base::Corpus& corpus;
    std::filesystem::path output_folder;   // the state folder's vxd_ow
};
// `VXD [--out DIR]`: the first driver of the file (or a corpus name) built, and a line about it. Returns 0.
int main(const CommandEnvironment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::owbuild
