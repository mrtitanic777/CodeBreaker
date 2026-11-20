// exact/command.hpp -- `codebreaker exact`, complete: the tool table of run.hpp wired from the settings.
//
//     const int code = exact::command::run(args, {.config = config});          // args: what follows `exact`
//     table.set("exact", exact::command::runner({.config = config}));          // as the command table's raw runner
//
// exact takes its own words (`PROJECT STAGE [options]`, `--toolchains`, `--compile PROFILE FILE.c [switches]`), which each stage reads, so it
// is a raw runner and not a declared cli::Command. Everything it runs on is made when it runs, from the configuration alone:
//
//   * the toolchain profiles and kits of the checkout, the plugins (plm86, mspascal, xenix_c) and the hooks the profiles name;
//   * the DOS executive (exact/emu/dosexec, linked in, over unicorn.dll) that runs the period compilers and linkers; without it a stage
//     that has to run one says why it cannot;
//   * the stages: ledger, library, carve, modules, bss, data, repair, names, identify, bench, seed, readable, link and draft.
//
// What is not wired is said by the stage that needs it, not left out of the table: see `draft` in command.cpp.
#pragma once

#include <functional>
#include <span>
#include <string>

#include "base/config.hpp"

namespace cb::exact::command {

struct Environment {
    config::Config config;   // the checkout (profiles, kits), the state folder, where the DOS executive is
};

// The command. 0 when the stage says so; 2 for no stage or none given (the usage is printed).
int run(std::span<const std::string> args, const Environment& environment);

// The same as the command table's raw runner takes it.
std::function<int(std::span<const std::string>)> runner(const Environment& environment);

}  // namespace cb::exact::command
