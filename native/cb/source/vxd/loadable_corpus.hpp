// cb/source/vxd/loadable_corpus.hpp -- the whole VxD corpus through the from-source loadable build.
//
// The object-level test (corpus.hpp) proves byte-exact CODE recovery. This proves the next thing: that the recovered source rebuilds, through
// the period toolchain (ML, LINK 2.60 /VXD), into a container the Windows 95 loader accepts -- the device id stamped from the DDB, an
// identity page map, the version resource embedded. It runs `from_source::build` on every module and records what came out.
//
// The results go to a JSON file after every module (resumable). A boot is a separate step (`vxd boot`); this measures BUILDABILITY and
// container validity at corpus scale, the static precondition for one.
//
//     codebreaker vxd loadable-corpus FOLDER RESULTS [--redo vkd,vmm] [--workers 4]
#pragma once

#include <filesystem>
#include <span>
#include <string>

#include "cb/base/corpus.hpp"
#include "cb/source/vxd/from_source.hpp"

namespace cb::source::vxd::loadable_corpus {

struct Environment {
    const from_source::Environment& build;
    const base::Corpus& corpus;               // the folder taken when none is given: its root
    std::filesystem::path results_file;       // the file taken when none is given (the state folder's vxd/loadable.json)
};

// `[FOLDER [RESULTS]] [--redo NAMES] [--workers N]`. Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::loadable_corpus
