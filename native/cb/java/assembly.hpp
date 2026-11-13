// cb/java/assembly.hpp -- the transpiler put together for use: the services a conversion works with (transpile.hpp's `Services`),
// made once from the lane's data and the Java front end, and the conversions asked of it.
//
// A conversion is handed its runtime, its target profiles, its projects and its translator; nothing is global. This is where they
// are made: the platform layer and API maps read from the lane's folder (`JavaData`), the runtime's headers, the profiles of the
// projects this build carries (Minecraft's seven eras), the projects themselves, and the C++03 / C++17 back end as the translator. The
// one thing the assembly does not carry is the Java front end -- javac's typed tree of the sources lowered into the translator's IR --
// because that is a stage of its own that whoever owns it hands in (`FrontEnd`); the project's IR fixes are applied to what it gives.
//
//     assembly::Assembly assembly(JavaData::locate(), front_end);
//     JavaLane lane(tools, assembly.converter("C++17"), {.standard = "c++17"});
//     Summary done = assembly.convert(java_dir, out_dir, {.cpp_std = "C++03", .platform = "xbox360"});
//
// An assembly converts one program at a time (the profile sets and project hooks are shared between its conversions); make one per
// thread to convert side by side.
#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/java/paths.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/projects/package.hpp"
#include "cb/java/runtime.hpp"
#include "cb/java/transpile.hpp"
#include "cb/translate/backends/cpp03.hpp"
#include "cb/translate/ir.hpp"

namespace cb::java::assembly {

// The Java sources under `java_dir` as the translator's IR, made in `arena`: javac's typed tree of every compilation unit, lowered.
// May throw (ToolError when javac reports an error, with its first message).
using FrontEnd = std::function<std::vector<translate::ir::Node*>(Arena& arena, const std::filesystem::path& java_dir)>;

// A conversion of a program to C++ in some dialect: the Java source folder in, the C++ folder out.
using Converter = std::function<transpile::Summary(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir)>;

class Assembly {
public:
    // `data`: the lane's folder (platform layer, API maps, the projects' manifests). NotFoundError when the runtime's canonical
    // headers are not in it.
    Assembly(JavaData data, FrontEnd front_end);
    Assembly(const Assembly&) = delete;
    Assembly& operator=(const Assembly&) = delete;

    // The services of a conversion; they live as long as the assembly.
    transpile::Services services() { return {data_, runtime_, profiles_, projects_, translator_}; }

    transpile::Summary convert(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir, const transpile::Options& options);

    // The conversion to `cpp_std` ("C++03", "C++17" ...) for no particular platform, as the Java lane asks for: the report is not
    // written. The assembly must outlive the converter.
    Converter converter(std::string cpp_std);

private:
    JavaData data_;
    runtime::Runtime runtime_;
    profiles::ProfileSet profiles_;
    projects::ProjectSet projects_;
    translate::backends::cpp03::Translator translator_;
};

}  // namespace cb::java::assembly
