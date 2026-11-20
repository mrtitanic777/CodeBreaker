// source/vxd/corpus.hpp -- the whole VxD corpus through object-level assemble-and-compare.
//
// `masm::compare_module` on every driver of a folder (and every module of a W3 bundle), which proves -- or measures -- byte-exact CODE
// recovery across the ~215 drivers: each executable object is written as MASM, assembled with the period assembler and compared with the
// original's bytes. The drivers also ship as IOS port and mini-port drivers (.pdr, .mpd) and under a few other names; all of them are LE
// ring-0 modules the recovery reads the same, so the corpus is the complete set.
//
// The results go to a JSON file after every module, so a run is RESUMABLE: asked again, it skips the modules the file has (a full pass is
// many hours of assembling). Several modules are assembled at once.
//
//     codebreaker vxd corpus FOLDER RESULTS [--redo vkd,vmm] [--workers 4]
#pragma once

#include <cstddef>
#include <filesystem>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "base/corpus.hpp"
#include "source/vxd/disassembly.hpp"
#include "source/vxd/masm.hpp"

namespace cb::source::vxd::corpus {

// The drivers of a folder: its *.vxd, *.386, *.pdr and *.mpd files, and csmapper.sys, bigmem.drv and drvspace.bin where they are, that hold
// LE ring-0 modules (a stray real-mode .sys or .drv is left out), sorted by path.
std::vector<std::filesystem::path> driver_files(const std::filesystem::path& folder);

// The command line's `--redo a,b,c`: the names to do again.
std::set<std::string> redo_set(const std::string& text);

struct Environment {
    const DdkNames& names;
    const masm::Toolchain& tools;
    const base::Corpus& corpus;               // the folder taken when none is given: its root
    std::filesystem::path results_file;       // the file taken when none is given (the state folder's vxd/byte_exact.json)
};

// `[FOLDER [RESULTS]] [--redo NAMES] [--workers N]`. Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::corpus
