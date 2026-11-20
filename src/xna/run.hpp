// xna/run.hpp -- `codebreaker xna <package, folder or game .exe> [--name N] [--no-content]`: an XNA game as a Converted project.
// (`codebreaker xna port <project>` then builds its C# for the PC: xna/port.)
//
//     Converted/<name>/
//       original/   what came in (the package, or the game's .exe)
//       assets/     the game's files as it ships them (from a package: unpacked) in game/, and content/: every .xnb converted --
//                   textures and fonts as PNG (+ JSON), at their paths
//       src/        C#: one project per assembly of the game's own (ILSpy, then the fix-ups of xna/fixups)
//       reports/    content.json (each .xnb: kind, size, readers, or why it did not convert), decompile.json (each assembly, and
//                   what the fix-ups left)
//
// An assembly is the game's own unless it is the framework's: Microsoft.*, System.*, mscorlib, netstandard. Other libraries a game
// ships (a middleware .dll) are decompiled too, into their own project, since they are what the game was built with. An assembly an
// obfuscator has been over (de4dot names one: SunBurn's Spices.Net) is cleaned by de4dot first -- names restored, strings decrypted,
// control flow and field data put back -- and the cleaned copy is decompiled; de4dot's "Unknown Obfuscator" is its word for none
// found, and such an assembly is decompiled as it is. The decompiler is ILSpy's command line (`ilspycmd`), run with
// DOTNET_ROLL_FORWARD=LatestMajor so a newer .NET runs it.
//
//     run::Environment env{.tools = {.ilspycmd = settings_path, .checkout_root = root, .state = state},
//                          .store = converted::Store(converted_root)};
//     run::Converted done = run::convert(game_exe, env, {.name = "block-world"}, {.convert_xnb = ..., .convert_effect_file = ...});
//     done.project.root();  done.report.assemblies;  done.kinds;
//
// The tools and folders come in as values (settings are read where the application is put together, not here). The converters of
// .xnb content are not in the tree yet (formats/xnb): they come in as `Hooks`, JSON text of the record each file made.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/converted.hpp"
#include "xna/fixups.hpp"

namespace cb::xna::run {

// The tools run.cpp starts, and where it keeps what it makes: the values of the `ilspycmd` and `de4dot` settings (each empty when
// not set), the checkout (de4dot is looked for in a build beside it, `../_tools/de4dot`), and the state folder.
struct Tools {
    std::optional<std::filesystem::path> ilspycmd;
    std::optional<std::filesystem::path> de4dot;
    std::filesystem::path checkout_root;
    std::filesystem::path state;
    // The user's home folder, where `dotnet tool install -g` puts ilspycmd (`.dotnet/tools`); the environment's by default.
    std::optional<std::filesystem::path> home;
};

// What run.cpp takes from formats/xnb: each is given a file and the folder its conversion goes in and answers the JSON text of
// the record it made (an object: "kind", or "error", and the rest the converter wrote; run adds "folder").
struct Hooks {
    // A file that is not an .xnb but may be an effect: the record, or nothing when it is not one.
    std::function<std::optional<std::string>(const std::filesystem::path& file, const std::filesystem::path& out_dir)> convert_effect_file;
    // An .xnb file.
    std::function<std::string(const std::filesystem::path& file, const std::filesystem::path& out_dir)> convert_xnb;
};

// The assemblies whose names say they are the framework's (a lower-case file name starting with one of these).
std::span<const std::string_view> framework_prefixes();

// ILSpy's command line: the setting, else PATH, else ~/.dotnet/tools. NotFoundError when it is none of those.
std::filesystem::path find_ilspycmd(const Tools& tools);

// The game's own assemblies under `folder`, the .exe first (the others by path): .NET files that are not the framework's nor a
// satellite resource assembly.
std::vector<std::filesystem::path> assemblies(const std::filesystem::path& folder);

// de4dot's program: the setting, else PATH, else a build beside the checkout; nothing when none.
std::optional<std::filesystem::path> find_de4dot(const Tools& tools);

// The obfuscator de4dot finds in `assembly`; nothing when none is found or de4dot is not installed.
std::optional<std::string> obfuscator(const std::filesystem::path& assembly, const Tools& tools);

// An assembly as it will be decompiled, and what it was obfuscated with.
struct Cleaned {
    std::filesystem::path path;
    std::optional<std::string> obfuscator;
};
// `assembly` cleaned by de4dot (kept under <state>/xna/deobfuscated by its contents' digest), or itself when nothing obfuscated it.
// ToolError when de4dot fails.
Cleaned deobfuscated(const std::filesystem::path& assembly, const Tools& tools);

// ILSpy over one assembly: a project of C# (C# 11: newer syntax puts a constructor's assignments into field initialisers, where one
// field may not set another) in `out_dir`. ToolError when it fails.
void decompile(const std::filesystem::path& assembly, const std::filesystem::path& out_dir, const Tools& tools);

// What was made of one assembly of the game's own.
struct AssemblyReport {
    std::string assembly;                   // its path in the game's folder
    std::string project;                    // its project's folder name in src/
    std::optional<std::string> deobfuscated;   // what de4dot cleaned it of
    std::size_t ref_casts = 0;
    std::size_t constructors = 0;
    std::vector<fixups::LeftInFile> left;   // what the fix-ups could not place
};

struct Report {
    std::vector<AssemblyReport> assemblies;
};

struct Environment {
    Tools tools;
    base::converted::Store store;
};

struct Options {
    std::optional<std::string> name;   // the project's name (the file's by default)
    bool content = true;               // convert the .xnb files too
};

struct Converted {
    base::converted::Project project;
    Report report;
    std::map<std::string, std::size_t> kinds;   // content records by kind ("error" for a failed one)
};

// An XNA game -- a folder, a game's .exe, or a package (a disc image, an archive) -- as a Converted project. UsageError when a folder
// holds no .NET assembly; FormatError when a package has nothing in it to read; UnsupportedError when content is wanted and the
// hooks do not give the converters. The decompiled C# and its report stay where an earlier run left them (an assembly's project is
// not made again when its folder is there).
Converted convert(const std::filesystem::path& source, const Environment& env, const Options& options = {}, const Hooks& hooks = {});

// `xna <source> [--name N] [--no-content]` (and `xna port ...`, handed to `port`): the report on stdout, 0 on success, 1 when the
// conversion failed, 2 for a missing source. Without arguments (or with -h) the usage, as a failure (2) only when nothing was asked.
int main(std::span<const std::string> args, const Environment& env, const Hooks& hooks,
         const std::function<int(std::span<const std::string>)>& port = {});

}  // namespace cb::xna::run
