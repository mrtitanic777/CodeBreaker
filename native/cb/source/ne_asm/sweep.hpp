// cb/source/ne_asm/sweep.hpp -- the idiomatic NE re-emitter across the whole 16-bit corpus, unattended.
//
// The KERNEL, USER and GDI analog of the PE sweep: every code segment of every NE module is reconciled to byte-exact cMacros source (idiom.hpp, by
// default through the nasm backend, which needs no DOS: only what nasm encodes another way is pinned as db), and a coverage ledger is kept. One pass
// reconstructs the roughly three hundred NE modules the one-segment-at-a-time command had not.
//
//     codebreaker ne-asm sweep [--limit N] [--start K] [--emit DIR] [--list] [--backend nasm|ml]
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/source/ne_asm/idiom.hpp"

namespace cb::source::ne_asm::sweep {

// One CODE segment of a module.
struct SegmentRecord {
    std::size_t segment = 0, bytes = 0;
    bool ok = false;
    std::size_t code_instructions = 0, pinned = 0, live = 0;
};

// One module: its code segments, and the totals.
struct ModuleRecord {
    std::string module;
    std::vector<SegmentRecord> segments;
    bool ok = false;          // every segment exact (and there is at least one)
    std::size_t exact_segments = 0, code_instructions = 0, live = 0;
};

// The NE modules of the corpus's system tree, by file name, each once: *.exe, *.dll, *.drv, *.386, *.cpl, *.scr and *.fon of the root and its
// WINDOWS folder (the corpus as Setup installs it), in SYSTEM, SYSTEM32, the folder itself and COMMAND, that hold an NE module. (name in lower case, path)
std::vector<std::pair<std::string, std::filesystem::path>> ne_modules(const base::Corpus& corpus);

// Every code segment of one module reconciled; with `emit`, each exact one written to <emit>/<module>/<module>_segNN.asm. `state`: where recovered
// names are looked for (<state>/idiomatic/<module>/names.json).
ModuleRecord sweep_module(const std::string& module, const std::filesystem::path& path, const idiom::Assemblers& assemblers, idiom::Backend backend,
                          const std::filesystem::path& state, const segment::SharedOrdinals& ordinals, const std::optional<std::filesystem::path>& emit,
                          const fixups::Objdump& objdump = fixups::run_objdump);

struct Environment {
    const base::Corpus& corpus;
    idiom::Assemblers assemblers;
    std::filesystem::path state;
    fixups::Objdump objdump = fixups::run_objdump;
};
// `[--limit N] [--start K] [--emit DIR] [--backend nasm|ml] [--list] [--ords FILE]`: each module's line, then the totals; with --emit also
// _ne_idiom_summary.json in DIR. Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::ne_asm::sweep
