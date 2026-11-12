// cb/verify/lanes/java.hpp -- what is particular to reconstructing a Java program as C++03.
//
// The module is a folder of Java source (a decompiled game's src/, say); its "functions" are its classes. The source is transpiled
// (java/transpile, for no particular platform) and each class's C++ is compiled on its own with the host compiler.
//
// A class that does not compile is NoCompile, with the compiler's first complaint. One that compiles is then run beside the original
// where it can be (java/probe): its methods with primitive parameters and results (static ones directly, instance ones on an object
// made with a constructor that takes primitives) are called on the same inputs in the JVM -- the Java source compiled by javac -- and
// natively, and the results compared exactly. All alike: Equivalent, or Equivalent with the limit `methods-partial` when only some of
// the class's methods could be run. Any different: Divergent, with the method, the input and both results. Nothing it could run (no
// such method, no JDK): Inconclusive, saying why. A jar is not a module of this kind: decompile it first (`codebreaker java decompile`).
//
// What the lane needs of its surroundings is given to it, not looked up: where the host compiler and the JDK are (`Tools`, from the
// settings), and the transpiler (`Convert`: the one conversion this kind asks for -- no platform, C++03 -- over the services the
// application built). A `Kind` (kind.hpp); `traits()` is what reconstruct::Kinds is told of it.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/java/probe.hpp"
#include "cb/java/transpile.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/reconstruct.hpp"

namespace cb::verify::lanes {

class JavaLane final : public Kind {
public:
    // Bump when what a verdict means changes (the flags, the judge, the transpile call).
    static constexpr int kRev = 3;
    // The dialect the program is compiled as, and the label the transpiler is asked for.
    static constexpr std::string_view kStandard = "c++03";
    static constexpr std::string_view kCppStandard = "C++03";
    // The compiler's switches for a syntax check: the dialect, no code, no warnings.
    static const std::vector<std::string>& flags();

    // Where the tools are. An empty `cxx` is no compiler (the lane has no profile); an empty `javac` no JDK (nothing is run).
    struct Tools {
        std::filesystem::path cxx;
        std::filesystem::path javac, java;
        std::vector<std::filesystem::path> libraries;        // the jars the game is compiled and run against
        std::filesystem::path shim_sources;                  // stand-ins for classes no library download has; empty for none
    };
    using ToolsFn = std::function<Tools()>;
    // Transpile the Java source in the first folder into the second, as this kind asks (C++03, no platform).
    using Convert = std::function<java::transpile::Summary(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir)>;

    JavaLane(ToolsFn tools, Convert convert) : tools_(std::move(tools)), convert_(std::move(convert)) {}

    const std::string& name() const override;
    // The digest covers the revision, the compiler, the flags, the probe's revision, the JDK and the game's libraries.
    // NotFoundError when there is no C++ compiler.
    Profile profile(const std::filesystem::path& module_path) const override;
    // Transpile the module into <work>/cpp. Classes are named by their .java file. UsageError for a jar.
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    // One translation unit per class: its .cpp, or for a header-only class a unit that includes it.
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    // What prepare() carries to build().
    struct PreparedJava final : Prepared {
        std::filesystem::path out;                           // the emitted C++
        java::transpile::Summary summary;
        std::filesystem::path src;                           // the Java source folder
        std::map<std::string, std::filesystem::path> sources;   // class name -> its .java file
    };

    // What the reconstruction path is told of this kind: a verdict is per class, there is no runtime library, the candidates are the
    // transpiler's, and a module's results are filed under its .java files' content.
    static reconstruct::KindTraits traits();

    // ---- the module ------------------------------------------------------------------------------------------------------

    // A folder holding .java files (at any depth), or a .java file.
    static bool is_module(const std::filesystem::path& path);
    // The folder the source is in: the path, or for a file its folder.
    static std::filesystem::path source_dir(const std::filesystem::path& path);
    // What results are filed under: the SHA-256 of the .java files' names and contents, in a fixed order.
    static std::string module_sha(const std::filesystem::path& path);

    // ---- the host compiler ---------------------------------------------------------------------------------------------------

    struct Check {
        bool ok = false;
        std::vector<std::string> errors;                     // the compiler's lines that say `error`
    };
    // Compile one translation unit for syntax and types. `prefix` is a header precompiled by `precompile` -- the support headers every
    // unit starts with -- included first. ToolError when the compiler cannot be started.
    static Check check(const std::filesystem::path& cxx, const std::filesystem::path& include_dir, const std::filesystem::path& unit,
                       const std::optional<std::filesystem::path>& prefix = std::nullopt);
    // stdafx.h and the Java runtime (JavaRT.h and what it includes) precompiled once for the module: every class unit starts with them,
    // and parsing them was four fifths of each class's compile. The header to pass as `prefix`, or nothing (a compiler that is not
    // GCC's, or the header did not precompile).
    static std::optional<std::filesystem::path> precompile(const std::filesystem::path& cxx, const std::filesystem::path& include_dir,
                                                           const std::filesystem::path& work);

    // ---- the judge ---------------------------------------------------------------------------------------------------------

    // One class to judge.
    struct Item {
        std::string name;
        std::filesystem::path unit;                          // the translation unit to compile
        std::filesystem::path source;                        // its .java file, or empty
    };
    // What judging a class needs of the whole build.
    struct Context {
        Tools tools;
        java::probe::Toolchain toolchain;
        std::filesystem::path include_dir;                   // the emitted C++
        std::filesystem::path work;
        std::shared_ptr<java::probe::SourceTree> tree;       // the staged Java source (none: no JDK)
        java::probe::Compiled compiled;                      // the Java harnesses, compiled in batches beforehand
    };
    // The verdict of one class: it compiles, and where it can be run beside the original, it does the same. Safe to call from several
    // threads at once.
    static Judgement judge_class(const Context& context, const Item& item);

private:
    ToolsFn tools_;
    Convert convert_;
};

}  // namespace cb::verify::lanes
