// cb/translate/dotnet/package.hpp -- the installed .NET SDK as a toolchain.
//
// Compile C# with the SDK's own Roslyn (csc.dll), run the result on the installed runtime. No project file, no
// NuGet restore, nothing downloaded: the compiler is given the reference assemblies of the SDK's targeting pack by
// path, and the program gets a runtimeconfig.json naming the installed Microsoft.NETCore.App.
//
//     if (auto sdk = dotnet::Sdk::find(); sdk && sdk->available()) {
//         Compiled c = sdk->compile({.sources = {"Prog.cs"}, .out_dll = "out/Prog.dll"});   // an exe (Main)
//         RunResult r = sdk->run("out/Prog.dll");
//     }
//
// The SDK is found from CB_DOTNET (the path of dotnet.exe), then PATH, then the standard install folder.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::translate::dotnet {

// How long csc may run before it is given up on (TimeoutError).
inline constexpr std::chrono::seconds kCompileTimeout{300};

// The installed Microsoft.NETCore.App that the targeting pack matches.
struct Runtime {
    std::string major;     // "9"
    std::string version;   // "9.0.5": the newest installed of that major, else the targeting pack's own version
};

// What csc said and whether it produced the program.
struct Compiled {
    bool ok = false;
    std::string log;       // the compiler's output
};

enum class Target { Exe, Library };

struct CompileOptions {
    std::vector<std::filesystem::path> sources;
    std::filesystem::path out_dll;
    std::vector<std::filesystem::path> references;      // assemblies beyond the targeting pack's
    Target target = Target::Exe;
    bool nullable = true;                               // -nullable:enable
    bool allow_unsafe = false;                          // -unsafe+
    bool warn = false;                                  // false: the warnings the translators' output always has are muted
    std::optional<std::filesystem::path> tmp;           // TEMP and TMP for the compiler (made if absent)
    std::optional<std::string> main;                    // the class whose Main is the entry point, where several have one
};

struct RunOptions {
    std::vector<std::string> args;
    std::chrono::seconds timeout{60};
    Bytes stdin_data;                                   // empty: the program's stdin is empty
    std::optional<std::filesystem::path> tmp;
};

struct RunResult {
    std::optional<std::uint32_t> exit_code;             // nullopt: it did not finish within the timeout and was ended
    std::string out, err;                               // decoded as UTF-8 (a bad byte becomes U+FFFD)
};

// A .NET installation: dotnet.exe and what it holds. Discovery looks at the disk once, when the object is made.
class Sdk {
public:
    // CB_DOTNET (when it names a file), then dotnet on PATH, then the standard install folders; nullopt if none.
    static std::optional<Sdk> find();
    // The installation of this dotnet.exe.
    static Sdk at(const std::filesystem::path& dotnet_exe);

    const std::filesystem::path& exe() const { return exe_; }
    // The folder dotnet.exe is in (symbolic links resolved): sdk/, packs/ and shared/ are in it.
    const std::filesystem::path& home() const { return home_; }
    // The newest SDK folder (it holds Roslyn).
    const std::optional<std::filesystem::path>& sdk_dir() const { return sdk_dir_; }
    // The folder of the SDK's compiler: csc.dll and Microsoft.CodeAnalysis*.dll.
    const std::optional<std::filesystem::path>& roslyn_dir() const { return roslyn_dir_; }
    // The targeting pack's reference assemblies folder (System.Runtime.dll, System.Console.dll, ...).
    const std::optional<std::filesystem::path>& reference_dir() const { return reference_dir_; }
    const std::optional<Runtime>& runtime() const { return runtime_; }

    // Compiler, targeting pack and runtime are all there.
    bool available() const { return roslyn_dir_ && reference_dir_ && runtime_; }

    // Compile C# sources with the SDK's csc (ToolError if it is not available; TimeoutError past kCompileTimeout). An
    // exe gets its runtimeconfig.json beside it.
    Compiled compile(const CompileOptions& options) const;

    // Run a compiled program on the installed runtime.
    RunResult run(const std::filesystem::path& dll, const RunOptions& options = {}) const;

    // Write <dll>.runtimeconfig.json beside a compiled program: run on the installed runtime. Returns its path.
    std::filesystem::path write_runtimeconfig(const std::filesystem::path& dll) const;

private:
    Sdk() = default;
    std::filesystem::path exe_, home_;
    std::optional<std::filesystem::path> sdk_dir_, roslyn_dir_, reference_dir_;
    std::optional<Runtime> runtime_;
};

// The newest of the folders (or files) in `folder` whose name starts with a digit and with `prefix`, by the numbers
// of its first three dot- or dash-separated parts (a part that is not a number counts as 0; the first of equals
// stays); nullopt when there is none or the folder cannot be read.
std::optional<std::string> newest_version_dir(const std::filesystem::path& folder, std::string_view prefix = "");

// The up to three numbers a version name stands for ("9.0.100-preview.1" is 9, 0, 100).
std::vector<std::uint64_t> version_key(std::string_view name);

}  // namespace cb::translate::dotnet
