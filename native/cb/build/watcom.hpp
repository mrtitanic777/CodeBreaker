// cb/build/watcom.hpp -- compiling and linking reconstructed C with OpenWatcom.
//
// The pieces a whole-module build needs: where the tools are and the environment they run in, the flags a
// reconstruction is compiled with, and how a module's imports are declared and linked so calls reach the same
// DLL functions the original calls.
//
//     OpenWatcom ow = OpenWatcom::from_settings(settings_root);   // CB_OPENWATCOM overrides the setting
//     ProcessSpec spec{.argv = {ow.cc(Target::Win32).string(), ...}, .env = ow.environment(Target::Win32)};
//
//     ImportMaps imports(read_imports);                           // each retail module's imports, read once
//     auto map = imports.of("retail.dll");                        // {C name -> the DLL that exports it}
//     std::string lines = import_lines(externs_of_object, wanted_names, *map, imports);
//
// "ne" modules (16-bit Windows) compile with wcc and link against lib286/win; "pe" modules (32-bit) with
// wcc386 against lib386/nt. Nothing here is global: an OpenWatcom is a root path, ImportMaps owns its cache.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cb::build::watcom {

// The kind of module a build targets.
enum class Target {
    Win16,   // NE: wcc, headers h/win, libraries lib286/win
    Win32,   // PE: wcc386, headers h/nt, libraries lib386/nt
};

// The compiler emits calls to its own runtime helpers for some constructs: __TryInit/__TryUnwind/__Except for
// __try/__except, and 64-bit arithmetic and floating-point helpers. They live in the C runtime library; without
// it every body using such a construct failed to link. Local stubs for a body's callees are defined in the
// object and still take precedence.
inline constexpr std::string_view kRuntimeLibrary = "library clib3r.lib\n";

// Where a rebuilt DLL is based. The linker's default (0x400000) is exactly where ordinary computed values fall --
// the sum of two of the oracle's scratch pointers, say -- and a value inside the rebuilt image is taken for "an
// address of this build's own data", which the original cannot have: a correct body was then reported as passing
// a different argument. Up here nothing collides.
inline constexpr std::uint32_t kImageBase = 0x6E000000;
inline constexpr std::string_view kBaseOption = "option offset=0x6E000000\n";

// Build scratch lives in the temp dir, not beside the source (it grew to many thousands of files under
// src/targets/win95 and slowed every search of the repo): <temp>/codebreaker/bodyverify/<parts...>.
std::filesystem::path scratch_dir(std::span<const std::string_view> parts = {});

// The from-source OpenWatcom release tree.
class OpenWatcom {
public:
    explicit OpenWatcom(std::filesystem::path root) : root_(std::move(root)) {}
    // The environment variable CB_OPENWATCOM when it is set and not empty, else `settings_root` (settings.json's
    // "openwatcom").
    static OpenWatcom from_settings(std::string_view settings_root);

    const std::filesystem::path& root() const { return root_; }

    // The absolute path of a tool ('wcc', 'wcc386', 'wlink', 'wasm', 'wrc'; `.exe` is added), or empty when
    // OpenWatcom is not built. Absolute because Windows CreateProcess searches the CALLER's PATH, not the
    // environment passed in.
    std::filesystem::path tool(std::string_view name) const;
    // The C compiler for a target.
    std::filesystem::path cc(Target target) const { return tool(target == Target::Win16 ? "wcc" : "wcc386"); }
    // Header search directories: Win16 uses h/win, Win32 uses h/nt.
    std::vector<std::filesystem::path> include_dirs(Target target) const;
    // The runtime library directory (the Win16 one holds libentry.obj + clib*.lib).
    std::filesystem::path lib_dir(Target target) const;
    // What to set in the environment of OpenWatcom's tools, over the inherited one: WATCOM, INCLUDE,
    // WLINK_LNK, a PATH that begins with the tools' folder, and the switch that lets them run from the current
    // directory.
    std::map<std::string, std::string> environment(Target target) const;

private:
    std::filesystem::path root_;
};

// ---- imports --------------------------------------------------------------------------------------------------
// A rebuilt body must call the retail module's imported functions THROUGH AN IMPORT, not through a local
// stand-in. The oracle records an external call when execution lands on an import; a local
// `int HeapAlloc(){return 0;}` is invisible to it. With stand-ins the rebuilt side never called anything, so any
// body whose retail path reached an API could not match -- most of the code that matters.

// One entry of a PE import table: the DLL's file name and the symbol ("#12" for an import by ordinal).
struct ImportedName {
    std::string dll;
    std::string name;
};

// {C name -> the DLL (upper case, no extension) that exports it}: lookup only.
using ImportMap = std::unordered_map<std::string, std::string>;

// A function that reads a PE module's import table; it may throw (the module is not a readable PE), which
// gives an empty map.
using ImportReader = std::function<std::vector<ImportedName>(const std::filesystem::path& module)>;

// The import maps of the retail modules a process builds against, each read once. Safe to use from the threads
// that compile in parallel.
class ImportMaps {
public:
    explicit ImportMaps(ImportReader reader) : reader_(std::move(reader)) {}

    // {C name: DLL} for the functions a retail PE imports by name (the first DLL wins). An export with a stdcall
    // decoration (`_AtqReadFile@16`, INFOCOMM's) or a C++ one (`?QueryStrA@STR@@QBEPADXZ`) is under its C
    // spelling, the one the decompiler writes (`_AtqReadFile_16`, `_QueryStrA_STR__QBEPADXZ`), and links to the
    // decorated name (export_name()): left out, the call was linked to nothing and went to the next instruction.
    std::shared_ptr<const ImportMap> of(const std::filesystem::path& module);

    // The name the DLL exports `c_name` by, where it differs from the C name; `c_name` itself otherwise.
    std::string export_name(const std::string& c_name) const;

private:
    ImportReader reader_;
    mutable std::mutex lock_;   // compiles run on threads: the two tables below are shared
    std::map<std::filesystem::path, std::shared_ptr<const ImportMap>> maps_;
    std::unordered_map<std::string, std::string> export_names_;
};

// An import the original makes by number, as the decompiler's tables hold it.
struct NumberedImport {
    std::string dll;
    std::uint32_t number = 0;
    std::optional<std::uint32_t> argument_bytes;   // bytes of arguments the callee removes
};

// wlink `import` directives binding each wanted name's decorated symbol to DLL.name -- or to DLL.number for one
// the original imports by number (`numbered`). `externs` are the external symbols of the compiled object (the
// OMF reader's answer), `wanted` the names to bind, in any order: the lines come out in name order.
std::string import_lines(std::span<const std::string> externs, std::span<const std::string> wanted, const ImportMap& imports,
                         const ImportMaps& names, const std::unordered_map<std::string, NumberedImport>* numbered = nullptr);

}  // namespace cb::build::watcom
