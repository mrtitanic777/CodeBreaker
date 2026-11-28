// naming/names_command.cpp -- the `names` command as the program registers it; see names.hpp.
#include "naming/workbench.hpp"

namespace cb::naming::names {

cli::Command command(const symbols::Environment& environment) {
    // (the bench -- the knowledge base, the corpus's modules, the Namer over them -- lives for the one run)
    return declare_command([environment](const cli::Args& args) -> int {
        Workbench bench(environment);
        return bench.namer().execute(args);
    });
}

}  // namespace cb::naming::names
