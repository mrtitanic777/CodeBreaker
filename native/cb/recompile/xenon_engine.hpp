// cb/recompile/xenon_engine.hpp -- XenonRecomp and XenonAnalyse in-process: the fork in cb/recompile/xenonrecomp, built as the
// libraries LibXenonRecomp and LibXenonAnalyseMain and linked into the program. No XenonRecomp.exe is built or started.
//
//     const Run r = recomp({config_toml, context_header});     // a title (or the instruction tests: {objs_folder, out_folder})
//     const Run a = analyse({title_xex, switch_tables_toml});  // the switch tables XenonAnalyse finds
//
// Each is the program's own command line with its exit code and everything it printed (warnings, the instructions it does
// not implement). Paths are given absolute: the program resolves a configuration's paths against the configuration's own
// folder, and nothing changes the process's working folder. The programs keep state of their own between functions (a
// shared scratch buffer), so one runs at a time.
#pragma once

#include <functional>
#include <string>
#include <vector>

namespace cb::recompile::xenon_engine {

struct Run {
    int code = 0;
    std::string log;   // what the program printed
};

// A program's arguments (without its name) -> what it did. The production tools are recomp and analyse; a test hands its own.
using Tool = std::function<Run(const std::vector<std::string>& args)>;

Run recomp(const std::vector<std::string>& args);
Run analyse(const std::vector<std::string>& args);

}  // namespace cb::recompile::xenon_engine
