// recompile/xenon_engine.cpp -- see xenon_engine.hpp.
#include "recompile/xenon_engine.hpp"

#include <mutex>

#include "recompile/xenonrecomp/XenonAnalyse/xenon_analyse_main.h"   // recompile/xenonrecomp/XenonAnalyse: the program's main as a function
#include "recompile/xenonrecomp/XenonUtils/xenon_log.h"            // recompile/xenonrecomp/XenonUtils: where its messages go
#include "recompile/xenonrecomp/XenonRecomp/xenon_recomp_main.h"    // recompile/xenonrecomp/XenonRecomp

namespace cb::recompile::xenon_engine {
namespace {

std::mutex& engine_lock() {
    static std::mutex lock;
    return lock;
}

// The program's main over `args`, with `name` as argv[0], its messages collected.
template <typename Main>
Run run(Main main, const char* name, const std::vector<std::string>& args) {
    std::vector<std::string> owned;
    owned.reserve(args.size() + 1);
    owned.emplace_back(name);
    owned.insert(owned.end(), args.begin(), args.end());
    std::vector<char*> argv;
    for (std::string& a : owned) argv.push_back(a.data());
    argv.push_back(nullptr);
    Run out;
    std::lock_guard hold(engine_lock());
    const xenon_log::Capture capture(out.log);
    out.code = main(static_cast<int>(owned.size()), argv.data());
    return out;
}

}  // namespace

Run recomp(const std::vector<std::string>& args) { return run(xenon_recomp_main, "XenonRecomp", args); }

Run analyse(const std::vector<std::string>& args) { return run(xenon_analyse_main, "XenonAnalyse", args); }

}  // namespace cb::recompile::xenon_engine
