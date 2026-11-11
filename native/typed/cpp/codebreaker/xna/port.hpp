// codebreaker/xna/port.hpp -- typed C++ of codebreaker.xna.port.
//
// port.py -- an XNA game's decompiled C# (Converted/<name>/src) built and run on a PC with MonoGame.
//
//     codebreaker xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]
//     result = port::build(project, hooks)        {"exe", "errors": {code: count}, "first": [error lines]}
//
// The project's src/ stays the decompiled source. The port is built from a copy in build/port/:
//
//   src/<assembly>/      each assembly's C#, rewritten for the PC where it must be (the Game class's constructor
//                        first calls XnaPort.Runtime.Install(this); `new GraphicsDeviceManager(this)` is
//                        XnaPort.Runtime.Graphics(this); XACT types alias XnaPort.Xact; the base-class cast calls
//                        ILSpy writes are base.Update(...) ...);
//   XnaPort/             CodeBreaker's XNA runtime (xna/runtime/*.cs);
//   Stubs/               the rest of XNA's Xbox-side API, generated from the XNA 4.0 assemblies' metadata;
//   <name>.Port.csproj   MonoGame DesktopGL 3.8.1 on .NET 8, the game's own files beside the program,
//                        ContentConverted/ the converted content;
//   overlay              a folder of the game's own hand work, copied over all of it last.
//
// The build runs until it compiles or stops changing: an error the compiler itself names the fix for is fixed and the
// build run again; what is left is listed by error code in reports/port.json. `--run` starts the program for that many
// seconds and keeps its log. `--check-content` has the built program load every .xnb under Content/ through the runtime.
//
// Typing notes:
//   * formats/xnb.py (rename_readers) and formats/xbox/xsb.py (read) have no typed port: `Hooks` carries them.
//     An xsb bank is a JSON object (what xsb.read returns, written to <file>.cues.json by convert_xact).
//   * A compiler error is the Python's dict {file, line, col, code, text} (the regex's groupdict): `BuildError`.
//   * Python's dicts with fixed keys are structs (BuildResult, RunResult, EffectsReport); the build report, whose keys
//     depend on what happened (rewrites, content, overlay, rounds ...), is a Json object, kept in the Python's key order.
//   * `own` of rewrite() (a frozenset of type names) is a py::set<py::str>.
//   * subprocess.Popen(timeout=) / env= are done with the Windows API and detail::EnvGuard (run.hpp).
//   * Paths the Python takes from __file__ (xna/runtime, xna/apistubs) are <checkout root>/codebreaker/xna/...
//   * The overlay manifest (overlay.json) is read as JSON; a manifest of the wrong shape (a "replace" that is not a list
//     of strings ...) is outside this port (the Python would fail with whichever error its operation gives).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/base/converted.hpp"
#include "codebreaker/xna/run.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::xna::port {

using Json = knowledge::sources::Json;

extern const py::list<py::str> STUBBED;
extern const py::list<py::str> HAND_WRITTEN;
extern const py::str MONOGAME;
extern const py::list<py::str> XACT_TYPES;
extern const py::list<py::str> LIFECYCLE;
extern const py::dict<py::str, py::str> RENAMED_PARAMETERS;

// What port.py takes from modules without a typed port.
struct Hooks {
    // xsb.read(path): the bank as a JSON object
    std::function<Json(const py::str& path)> xsb_read;
    // xnb.rename_readers(raw, rename): the .xnb with its type readers renamed, or None when nothing changed
    std::function<std::optional<py::bytes>(const py::bytes& raw, const std::function<py::str(const py::str&)>& rename)> xnb_rename_readers;
};

// A compiler error line: the regex's groups.
struct BuildError {
    py::str file, line, col, code, text;
};

// codebreaker/xna (where runtime/ and apistubs/ are)
py::str HERE();
py::str RUNTIME();
py::str APISTUBS();

// ---- the generated XNA API stubs --------------------------------------------------------------------------------
std::optional<py::str> _xna_assembly(const py::str& name);
py::str _stub_tool();
// The XNA Xbox-side API stubs, generated once (state/xna/stubs) and copied to `out_dir`.
void stubs(const py::str& out_dir);

// ---- source rewrites --------------------------------------------------------------------------------------------
// Names of the types the game's C# under `folder` declares (classes, structs, enums, interfaces, delegates).
py::set<py::str> declared_types(const py::str& folder);

// The port's changes to one file of the game's C#: (text, [what changed]). `own`: type names the game declares itself.
std::tuple<py::str, py::list<py::str>> rewrite(const py::str& text, const py::set<py::str>& own = py::set<py::str>());

// `new Effect(device, bytes)`, and `: base(device, bytes)` in a class deriving from Effect, with the bytes passed through
// XnaPort.Effects.ForPC. -> (text, wrapped)
std::tuple<py::str, int64_t> _effect_bytes(const py::str& text);

// CS0507 (an override changes the access it overrides): `protected internal` here. -> fixes.
int64_t fix_access(const py::list<BuildError>& errors);

// The top-level arguments of the call whose '(' is at open_paren: ([(start, end)], index after ')'); (None, -1) when
// the parentheses never close.
std::tuple<std::optional<py::list<std::tuple<int64_t, int64_t>>>, int64_t> _args(const py::str& text, int64_t open_paren);

// Each error line of `code` whose text has `wanted`: edit(line) -> new line. -> fixes.
int64_t _edit_lines(const py::list<BuildError>& errors, const py::str& code, const py::str& wanted,
                    const std::function<py::str(const py::str&)>& edit);

int64_t fix_color(const py::list<BuildError>& errors);
int64_t fix_vertex_binding(const py::list<BuildError>& errors);

// {method name: [(full class name, first parameter's type)]}: the game's own extension methods.
py::dict<py::str, py::list<std::tuple<py::str, py::str>>> extension_methods(const py::str& folder);
int64_t fix_shadowed_extensions(const py::list<BuildError>& errors, const py::dict<py::str, py::list<std::tuple<py::str, py::str>>>& extensions);

int64_t fix_named_arguments(const py::list<BuildError>& errors);
int64_t fix_moved_members(const py::list<BuildError>& errors);
int64_t fix_ref_out(const py::list<BuildError>& errors);

// ---- content ----------------------------------------------------------------------------------------------------
// Every .xwb as <bank file>/stream_NNN.wav (+ bank.txt) and every .xsb as <file>.cues.json, under `converted`.
// -> {"wave banks": n, "sound banks": n}
py::dict<py::str, int64_t> convert_xact(const py::str& game_dir, const py::str& converted, const Hooks& hooks);

struct EffectsReport {
    int64_t compiled = 0;
    py::dict<py::str, py::str> failed;
};
// Every recovered effect (.fx under `converted`, first line "// sha1 <the Xbox effect's sha1>") compiled by MonoGame's
// mgfxc to `shaders`/<sha1>.mgfxo.
EffectsReport compile_effects(const py::str& converted, const py::str& shaders);

// Every song's music (.wma, beside its .xnb) as Ogg Vorbis under `converted`. -> songs
int64_t convert_songs(const py::str& game_dir, const py::str& converted);

// ---- the project ------------------------------------------------------------------------------------------------
py::str _csproj(const py::str& name, const py::str& exe_rel, const py::list<py::str>& assemblies, const py::list<py::str>& defines);
std::optional<py::str> _overlay_dir(const base::converted::Project& project, const std::optional<py::str>& overlay);
// src over dst; a file already there with the same size and time is left as it is.
void _copy_tree(const py::str& src, const py::str& dst);

extern const py::re::Pattern STUB_ASSEMBLIES;
extern const py::str _REFLECTIVE;
// A type reader's name in an .xnb as the port reads it.
py::str port_reader_name(const py::str& name, const py::str& program);
// Each .xnb under game_dir rewritten to port_reader_name's names (once: a stamp records it). -> files rewritten
int64_t port_reader_names(const py::str& game_dir, const py::str& program, const Hooks& hooks);

// build/port/ written from the project. -> (port dir, project name, report: a JSON object)
std::tuple<py::str, py::str, Json> prepare(const base::converted::Project& project, const std::optional<py::str>& overlay, const Hooks& hooks);

// -> (built, the errors, the whole output)
std::tuple<bool, py::list<BuildError>, py::str> _build_once(const py::str& port, const py::str& name);

struct BuildResult {
    bool ok = false;
    std::optional<py::str> exe;
    py::dict<py::str, int64_t> errors;     // error code -> count
    py::list<py::str> first;
    py::str build_output_tail;
    Json report;                           // rewrites, content, [xnb reader names, overlay,] rounds: as the Python's dict
    Json to_json() const;                  // the dict build() returns (and writes to reports/port.json)
};
BuildResult build(const base::converted::Project& project, const std::optional<py::str>& overlay, int64_t rounds, const Hooks& hooks);

struct RunResult {
    std::optional<int64_t> exit;           // None: still running when the time was up
    py::str output;                        // the last 6000 characters
    py::str log;
};
// Start the built game for `seconds`; xnaport.log and what it printed.
RunResult run_for(const py::str& exe, int64_t seconds, const std::vector<std::pair<std::string, std::string>>& env = {});

// Every .xnb under the built program's Content/ loaded by the program itself -> {asset: type or "error: why"}, or None
// when it wrote nothing.
std::optional<Json> check_content(const py::str& exe, int64_t seconds = 600);

extern const py::str DOC;
int64_t main(const py::list<py::str>& argv, const Hooks& hooks);

}  // namespace codebreaker::xna::port
