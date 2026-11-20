// symbols/names_command.cpp -- the `names` command as the program registers it; see names.hpp.
#include "symbols/workbench.hpp"

namespace cb::symbols::names {

cli::Command command(const Environment& environment) {
    // (the bench -- the knowledge base, the corpus's modules, the Namer over them -- lives for the one run)
    return declare_command([environment](const cli::Args& args) -> int {
        Workbench bench(environment);
        return bench.namer().execute(args);
    });
}

}  // namespace cb::symbols::names
