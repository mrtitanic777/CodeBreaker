// cb/translate/judge_c_java_command.hpp -- `codebreaker judge-c-java`: the C/C++ <-> Java judge as a command.
//
//     codebreaker judge-c-java roundtrip [--keep] [--work DIR] [--main CLASS] [FILE.java ...]
//
// `roundtrip` judges Java -> C++ -> Java (judge_native_jvm::roundtrip): the Java lane writes the classes as C++, clang reads that back, j2x maps
// the Java runtime in it onto Java's library, the Java back end writes Java again, and the original and the round-tripped program run on the JVM.
// With no FILE it judges the Java lane's fixture classes (Vec3D, MathHelper, AxisAlignedBB, Base, Sub) with the round-trip driver. It prints `Round trip: <verdict>` and the one-row table (program, verdict, tier, reason), and exits 0 for an equivalent verdict, 1 for any
// other.
//
// The Host the judge is given is made from the production pieces: the JDK of the settings (java, JAVA_HOME, PATH), the Java lane's converter
// (java/production's assembly, C++03, no platform), clang as the settings or Visual Studio's LLVM or PATH have it, the Java runtime's sources and API
// maps under data/, and j2x as the lift.
//
//     table.set(translate::judge_c_java::command({.config = config}));
#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/cli.hpp"
#include "cb/translate/judge_native_jvm.hpp"

namespace cb::translate::judge_c_java {

struct Environment {
    cb::config::Config config;
};

// The judge's Host from the settings. A part the machine lacks (no JDK, no clang) is left to fail where it is used, with its own message.
// The result refers to `environment`'s data and must not outlive the returned owner.
struct ProductionHost {
    judge_native_jvm::Host host;
    // (what the host's functions point at)
    std::shared_ptr<void> keep;
};
ProductionHost make_host(const Environment& environment);

// The fixture classes of the Java lane and the driver, the files (staged under `staging`), and the qualified main class.
struct RoundTripInputs {
    std::vector<std::filesystem::path> files;
    std::string main_class;
};
RoundTripInputs default_inputs(const Environment& environment, const std::filesystem::path& staging);

// The `judge-c-java` command.
cli::Command command(const Environment& environment);
int main(const std::vector<std::string>& args, const Environment& environment);

}  // namespace cb::translate::judge_c_java
