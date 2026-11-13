// cb/source/commands.hpp -- the source writers' commands, assembled from the settings and the corpus.
//
//     commands::Production production(config::Config::load(checkout), corpus);
//     table.set("vxd", production.vxd());                    // vxd <tool> ...
//     table.set("ne-asm", production.ne_asm());              // ne-asm <tool> ...
//     table.set("driver-source", production.driver_source());
//     table.set("dos-source", production.dos_source());
//     table.set(production.dos_build());                     // dos-build <folder> [--against F]
//     table.set(production.boot_floppy());                   // boot-floppy [--image F] [--out F] [--extract DIR]
//     table.set(production.driver_judge());                  // driver-judge <rebuilt> <original>
//
// Each module of this area takes what it needs from the world as an `Environment` (the DDK's names, the assemblers, the linkers, the corpus, the state
// folder); this is where those are made from the settings, as the application starts them. Nothing is made until a command runs: the DDK's include
// files, the tool folders, the DOS emulator and the lane kit (for the driver judge and the Windows half of a combined DOS file) are found when first
// asked for, and shared by the commands asked after it. A command it returns is independent of this object: it keeps what it needs alive.
//
// The tools a command takes are named in the tables (`vxd_tools`, `ne_asm_tools`), for the tests and for whoever lists them; `vxd::run` and
// `ne_asm::run::main` dispatch on them.
#pragma once

#include <functional>
#include <memory>
#include <span>
#include <string>

#include "cb/base/commands.hpp"
#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"

namespace cb::source::commands {

// A command that parses its own command line: the words after its name in, its exit code out.
using Runner = std::function<int(std::span<const std::string> args)>;

class Production {
public:
    Production(config::Config config, std::shared_ptr<base::Corpus> corpus);
    ~Production();
    Production(const Production&) = delete;
    Production& operator=(const Production&) = delete;

    // `vxd <tool> ...`: idiom, tree, masm, link, from-source, owbuild, finalize, disassembly, corpus, loadable-corpus, boot.
    Runner vxd() const;
    base::commands::ToolTable vxd_tools() const;
    // `ne-asm <tool> ...`: idiom, sweep, recon, segdiff, reachability, datacode, segment, fixups, debug-names, debug-names-estimate, msvc152.
    Runner ne_asm() const;
    base::commands::ToolTable ne_asm_tools() const;
    // `driver-source <driver> [--out DIR]`, `dos-source <program> [--out DIR]`.
    Runner driver_source() const;
    Runner dos_source() const;
    // `dos-build <folder> [--against ORIGINAL]`.
    cli::Command dos_build() const;
    // `boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]`: the Windows 95 Setup boot floppy (build/floppy.hpp), its programs
    // written and built by the DOS writer with the same surroundings `dos-source` has.
    cli::Command boot_floppy() const;
    // `driver-judge <rebuilt> <original> | --self-check <original>`: every entry point of both drivers run and compared.
    cli::Command driver_judge() const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

}  // namespace cb::source::commands
