// exact/run.hpp -- exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).
//
//     codebreaker exact PROJECT STAGE [options]
//     codebreaker exact --toolchains             the toolchain profiles there are
//     codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]
//                                                one file compiled, its functions listed
//
// PROJECT is a folder with a project.json (exact/project.hpp). The stages:
//
//     ledger      every one-function source compiled and put against the program (exact/ledger.hpp)
//     carve       where the program's own functions begin and end (exact/carve.hpp)
//     modules     each source file put together, compiled whole, its functions and data checked (exact/modules.hpp)
//     draft       functions written by the compiler's inverse model, kept when the compiler proves them (exact/draft/)
//     data        a module's initialized data: where it stands, definitions drafted (exact/data.hpp)
//     bss         the variables without a value at their places: src/BSS (exact/bss.hpp)
//     link        the program linked from its sources by the period linker, compared, diagnosed (exact/link.hpp)
//     library     the library modules found in the program, in the linker's order; stand-ins (exact/library.hpp)
//
// and repair, names, readable, seed, identify, bench. A stage is a function of the project and the rest of the command line; the table
// of them is the caller's (`Stages`), so that a program that has another stage to run -- or a different DOS emulator under the linker --
// says so there. `standard_stages` is the table of the stages that are driven from the project alone.
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "exact/linkwhole.hpp"
#include "exact/project.hpp"
#include "exact/toolchain.hpp"

namespace cb::exact::run {

using Stage = std::function<int(project::Project& project, const std::vector<std::string>& args)>;

class Stages {
public:
    // Replaces the stage of the same name (keeping its place in the list).
    void add(std::string name, Stage stage);
    const Stage* find(std::string_view name) const;
    // The stage names, in the order added.
    std::vector<std::string> names() const;

private:
    std::vector<std::pair<std::string, Stage>> stages_;
};

// ledger, library, carve, modules, data, bss, names, identify, repair and bench -- and link, with the DOS emulator under it (`dos` outlives
// the table; none: no link stage). The stages that take their own options (draft, readable) and seed are the caller's.
Stages standard_stages(const linkwhole::DosHost* dos = nullptr);

// The text `-h` prints.
std::string usage();

// The command: `args` is what follows `exact`. 0 when the stage says so, 2 for a command no stage answers (or none given: with the usage).
// UsageError is not thrown for these: they are the command's own answers.
int main(const std::vector<std::string>& args, const Stages& stages, toolchain::Toolchains& toolchains, const std::shared_ptr<const project::Services>& services);

}  // namespace cb::exact::run
