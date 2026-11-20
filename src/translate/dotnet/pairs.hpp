// translate/dotnet/pairs.hpp -- the C# kit's language pairs, each judged: the source run in its own runtime beside the
// translation in the target's (judge.hpp).
//
//     Pairs pairs{.sdk = *sdk, .csharp = frontend, .arena = arena};
//     pairs.csharp_to_csharp(files, work);        // Roslyn front end -> L2 -> the C# back end
//     pairs.java_to_csharp(src_dir, "pkg.Main", work, java_side);
//     pairs.csharp_to_cpp(files, work, cpp_side);  // Roslyn front end -> L2 -> the C++03 back end
//
// Each gives a Verdict: the judge's verdict and detail, what a front or back end declined (where, kind, why), the translation
// written, and the two runs. The Java side and the C++ side belong to the Java lane (a JDK, its front end and the API maps; the
// converter and the host compiler): the caller hands them in, so a host without them still judges C# to C#.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "core/arena.hpp"
#include "java/apimap.hpp"
#include "java/frontend.hpp"
#include "translate/dotnet/cs2cpp.hpp"
#include "translate/dotnet/judge.hpp"
#include "translate/dotnet/package.hpp"
#include "translate/frontends/csharp/frontend.hpp"

namespace cb::translate::dotnet::pairs {

// What a front or back end declined: where, which kind of construct, why. Written "<where> <kind>: <why>" in the judge's detail.
struct Refusal {
    std::string where, kind, why;
    std::string text() const { return where + " " + kind + ": " + why; }
};

struct Verdict {
    judge::Verdict verdict = judge::Verdict::NoOriginal;
    std::string detail;
    std::vector<Refusal> refused;
    std::vector<std::filesystem::path> files;   // the translation written (a folder, for C++)
    std::optional<judge::Run> original, translation;
};

// A Java program's side of a pair: javac's front end, the API maps and runtime of the C# back end's Java library, a JDK to run it.
struct JavaSide {
    cb::java::frontend::DumpTools tools;
    const cb::java::apimap::Registry* registry = nullptr;
    std::filesystem::path runtime_dir;          // JavaRt.cs and the other runtime files
    std::optional<judge::Jdk> jdk;
};

// The C++03 path's side: the converter that runs the Java transpiler over the skeleton with the C# units as its front end, and the
// host compiler that builds what it writes.
struct CppSide {
    cs2cpp::Converter converter;
    std::optional<judge::HostBuild> host;
};

struct Pairs {
    const Sdk& sdk;
    const frontends::csharp::Frontend& csharp;
    Arena& arena;       // the units are made in it

    // C# files -> units and what the front end refused.
    frontends::csharp::Program units(const std::vector<std::filesystem::path>& files, const std::filesystem::path& work) const;

    Verdict csharp_to_csharp(const std::vector<std::filesystem::path>& files, const std::filesystem::path& work,
                             const std::optional<std::string>& main = std::nullopt) const;
    Verdict java_to_csharp(const std::filesystem::path& src_dir, const std::string& main, const std::filesystem::path& work,
                           const JavaSide& java) const;
    Verdict csharp_to_cpp(const std::vector<std::filesystem::path>& files, const std::filesystem::path& work, const CppSide& cpp,
                          const std::string& main_class = "Program") const;
};

}  // namespace cb::translate::dotnet::pairs
