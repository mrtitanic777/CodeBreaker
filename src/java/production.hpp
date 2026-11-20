// java/production.hpp -- the transpiler made from the settings: the production assembly of assembly.hpp.
//
//     const JavaData data(config.checkout_root() / "config" / "java");
//     auto assembly = production::make_assembly(config, data);                       // javac's tree of the sources as the front end
//     JavaLane lane(tools, assembly->converter("C++03"));
//
// The front end is TreeDump run by the JDK the settings name (java, else JAVA_HOME, else the PATH), against the jars of setting java_libs and the
// stand-ins for the classes no library download has (probe_shims/, compiled against those jars once).
#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "base/config.hpp"
#include "java/assembly.hpp"
#include "java/frontend.hpp"
#include "java/paths.hpp"

namespace cb::java::production {

// The JDK's java, as the settings and the environment give it.
std::optional<std::filesystem::path> find_java(const cb::config::Config& config);
// The tools the front end runs javac with. `javac` is empty when no JDK was found.
frontend::DumpTools dump_tools(const cb::config::Config& config, const JavaData& data);
// The transpiler over `data`, with that front end (the tools are looked for when a conversion asks for them).
std::unique_ptr<assembly::Assembly> make_assembly(const cb::config::Config& config, const JavaData& data);

}  // namespace cb::java::production
