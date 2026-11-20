// xna/port.hpp -- an XNA game's decompiled C# (Converted/<name>/src) built and run on a PC with MonoGame.
//
//     codebreaker xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]
//
//     port::BuildResult r = port::build(project, tools, hooks);       // r.ok, r.exe, r.errors {code: count}, r.first
//
// The project's src/ stays the decompiled source. The port is built from a copy in build/port/:
//
//   src/<assembly>/      each assembly's C#, rewritten for the PC where it must be:
//                          - the Game class's constructor first calls XnaPort.Runtime.Install(this) (the content manager
//                            below), and `new GraphicsDeviceManager(this)` is XnaPort.Runtime.Graphics(this) (HiDef, as the Xbox
//                            360 was);
//                          - a file using XACT (AudioEngine, WaveBank, SoundBank, Cue, AudioCategory) gets `using` aliases to
//                            XnaPort.Xact (xna/runtime/Xact.cs), which plays its cues from WAV;
//                          - ((GameComponent)this).Update(...) and the other lifecycle calls ILSpy writes as a cast to the base
//                            class are base.Update(...) -- as a cast they call the override again, forever;
//   XnaPort/             CodeBreaker's XNA runtime (xna/runtime/*.cs): the content manager, players and the Guide, storage on disk,
//                        XACT, Xbox-only calls;
//   Stubs/               the rest of XNA's Xbox-side API -- GamerServices, Net, Storage, Avatar -- generated from the XNA 4.0
//                        assemblies' metadata (xna/apistubs; the assemblies are XNA Game Studio's, found in the Windows GAC, read
//                        and never copied), members doing nothing;
//   <name>.Port.csproj   MonoGame DesktopGL 3.8.1 on .NET 8 (DOTNET_ROLL_FORWARD=LatestMajor), the game's own files copied beside
//                        the program, ContentConverted/ the converted content (textures, fonts and sounds the runtime loads in
//                        place of the Xbox's .xnb, the XACT banks as WAV, the songs' music -- WMA on the Xbox -- as Ogg Vorbis,
//                        which MonoGame plays, and the effects' recovered HLSL compiled by mgfxc to Content/shaders/<sha1>.mgfxo);
//                        an .xnb naming a type in an XNA assembly MonoGame does not have (Avatar, GamerServices, Net, Storage: the
//                        stubs, in this program) names this program instead, and one read by XNA's ReflectiveReader names the
//                        runtime's XnaReflectiveReader, which keeps XNA's rules;
//   overlay              a folder of the game's own hand work, copied over all of it last (setting `xna_overlays`, <dir>/<name>/; or
//                        --overlay): shaders the Xbox had only as microcode, fixes no rewrite makes. Its overlay.json may say more:
//                        "replace" [paths removed first], "sources" {path in the port: a folder elsewhere, copied there -- a hand
//                        port's own tree}, "drop" [paths removed after: the generic runtime's pieces a hand port replaces], "assets"
//                        (a folder copied to assets/ beside the build, where a hand port's runtime looks for open-format assets).
//
// The build runs until it compiles or stops changing: an error the compiler itself names the fix for (CS1620: an argument passed `ref`
// where the method wants `out`, or the other way) is fixed and the build run again; what is left is listed by error code in
// reports/port.json. `--run` starts the program for that many seconds and keeps its log (xnaport.log, and what it printed): the first
// missing piece is what it stops at -- a shader names itself. `--check-content` has the built program load every .xnb under Content/
// through the runtime (models, buffers and custom types read by MonoGame's readers and the game's own) and lists each one's result in
// reports/content_check.json.
//
// The tools and the folders of CodeBreaker's own runtime come in as `Tools`, the converters of formats not yet in the tree (XACT banks,
// the .xnb reader names) as `Hooks`; settings are read where the application is put together, not here.
#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/converted.hpp"
#include "core/bytes.hpp"
#include "core/ordered_map.hpp"

namespace cb::xna::port {

inline constexpr std::string_view kMonoGameVersion = "3.8.1.303";

struct Tools {
    std::filesystem::path runtime_dir;       // xna/runtime: CodeBreaker's runtime for the port (*.cs)
    std::filesystem::path apistubs_dir;      // xna/apistubs: the stub generator's C# project
    std::filesystem::path state;             // where the stubs, their generator and the compiled effects are kept (<state>/xna/...)
    std::optional<std::filesystem::path> overlays;   // setting `xna_overlays`: a folder of <game name>/ overlays (<state>/xna/overlays if unset)
    std::optional<std::filesystem::path> gac;        // the Windows GAC of .NET assemblies (%WINDIR%\Microsoft.NET\assembly if unset)
    std::optional<std::filesystem::path> mgfxc;      // MonoGame's effect compiler (PATH if unset)
    std::optional<std::filesystem::path> ffmpeg;     // the songs' converter (PATH if unset)
};

// What the port takes from formats that are not in the tree yet. An empty hook where its content is present throws
// UnsupportedError, naming what was needed.
struct Hooks {
    // An XACT wave bank (.xwb) as <out>/stream_NNN.wav; the bank's own name (written to <out>/bank.txt).
    std::function<void(const std::filesystem::path& bank, const std::filesystem::path& out_dir)> decode_wave_bank;
    std::function<std::string(const std::filesystem::path& bank)> wave_bank_name;
    // An XACT sound bank (.xsb): the JSON text of its cues.
    std::function<std::string(const std::filesystem::path& bank)> sound_bank_cues;
    // An .xnb's type readers renamed through `rename`; nullopt when the file names none to rename (formats/xnb).
    std::function<std::optional<Bytes>(ByteView xnb, const std::function<std::string(const std::string&)>& rename)> rename_readers;
    // The XNA API stubs written to a folder; by default generated by the stub tool (needs `dotnet` and XNA Game Studio's assemblies).
    std::function<void(const std::filesystem::path& out_dir)> stubs;
};

// ---- the source rewrites ------------------------------------------------------------------------------------------------------

// Names of the types the C# under `folder` declares (classes, structs, enums, interfaces).
std::set<std::string> declared_types(const std::filesystem::path& folder);

struct Rewritten {
    std::string text;
    std::vector<std::string> changes;   // what was changed, in words ("Install in Game1()", "XACT aliases" ...)
};

// The port's changes to one file of the game's C#. `own`: type names the game declares itself (never aliased away).
Rewritten rewrite(std::string_view text, const std::set<std::string>& own = {});

// The top-level arguments of the call whose '(' is at `open_paren`, each as [begin, end) in `text`, and the index after the ')'.
// nullopt when the call does not close.
struct Arguments {
    std::vector<std::pair<std::size_t, std::size_t>> spans;
    std::size_t after = 0;
};
std::optional<Arguments> call_arguments(std::string_view text, std::size_t open_paren);

// ---- the compiler's errors and what fixes them ---------------------------------------------------------------------------------

struct BuildError {
    std::string file;
    int line = 0, column = 0;
    std::string code, text;
};

// One line of `dotnet build` output, when it is a C# error.
std::optional<BuildError> parse_error(std::string_view line);
// The C# errors of a build's output, each once, in the order they came.
std::vector<BuildError> parse_errors(std::string_view output);

// Each fix edits the files the errors name and answers how many lines it changed.
//   access            CS0507: one project holds what were two assemblies, where a `protected` override of a `protected internal`
//                     member was right; here it must say `protected internal`
//   ref_out           CS1620: the keyword at the column the compiler names, flipped
//   color             CS0121 between Color(int, ...) and Color(byte, ...): the arguments cast to int, so the call is XNA's
//   vertex_binding    CS0029 VertexBuffer -> VertexBufferBinding: XNA converted it implicitly, MonoGame does not
//   named_arguments   CS1739: XNA's name for the argument -> MonoGame's
//   moved_members     CS1061 for an XNA member MonoGame keeps elsewhere: ReferenceStencil, CubeMapFace
//   shadowed_extensions  a call that means one of the game's extension methods but finds a MonoGame instance method of the same
//                     name: the extension called by its class
std::size_t fix_access(const std::vector<BuildError>& errors);
std::size_t fix_ref_out(const std::vector<BuildError>& errors);
std::size_t fix_color(const std::vector<BuildError>& errors);
std::size_t fix_vertex_binding(const std::vector<BuildError>& errors);
std::size_t fix_named_arguments(const std::vector<BuildError>& errors);
std::size_t fix_moved_members(const std::vector<BuildError>& errors);

// An extension method of the game's own: the class that holds it (global::Ns.Class) and the type of its first parameter.
struct Extension {
    std::string cls, receiver_type;
};
// {method name: where it is declared}, from the C# under `folder`.
std::map<std::string, std::vector<Extension>> extension_methods(const std::filesystem::path& folder);
std::size_t fix_shadowed_extensions(const std::vector<BuildError>& errors, const std::map<std::string, std::vector<Extension>>& extensions);

// ---- the project --------------------------------------------------------------------------------------------------------------

// A type reader's name in an .xnb as the port reads it: the stubs' types in the program's own assembly, XNA's ReflectiveReader as
// the runtime's.
std::string port_reader_name(std::string_view name, std::string_view program);
// Each .xnb under `game_dir` with its readers renamed (once: a stamp beside the folder records it). How many files were rewritten.
std::size_t port_reader_names(const std::filesystem::path& game_dir, std::string_view program, const Hooks& hooks);

struct EffectsReport {
    std::size_t compiled = 0;
    std::map<std::string, std::string> failed;   // file -> why (reported in sorted order)
};
struct ContentReport {
    std::size_t wave_banks = 0, sound_banks = 0, songs = 0;
    EffectsReport effects;
};

// Every .xwb as <bank file>/stream_NNN.wav (+ bank.txt, the bank's own name) and every .xsb as <file>.cues.json, under `converted` at
// the same relative folders: how many of each.
std::pair<std::size_t, std::size_t> convert_xact(const std::filesystem::path& game_dir, const std::filesystem::path& converted, const Hooks& hooks);
// Every recovered effect (.fx under `converted`, its first line "// sha1 <the Xbox effect's sha1>") compiled by mgfxc to
// `shaders`/<sha1>.mgfxo, which the runtime loads for that effect (cached by the source's contents under <state>/xna/mgfxo).
EffectsReport compile_effects(const std::filesystem::path& converted, const std::filesystem::path& shaders, const Tools& tools);
// Every song's music (.wma, beside its .xnb) as Ogg Vorbis under `converted`, at the same relative folder. How many.
std::size_t convert_songs(const std::filesystem::path& game_dir, const std::filesystem::path& converted, const Tools& tools);

struct PrepareReport {
    OrderedMap<std::string, std::vector<std::string>> rewrites;   // file (relative to the port) -> what was changed
    ContentReport content;
    std::size_t xnb_reader_names = 0;
    std::optional<std::string> overlay;
};

struct Prepared {
    std::filesystem::path dir;   // build/port
    std::string name;            // the program's
    PrepareReport report;
};

// build/port/ written from the project. UsageError when the game has no assemblies yet.
Prepared prepare(const base::converted::Project& project, const Tools& tools, const Hooks& hooks, const std::optional<std::filesystem::path>& overlay = {});

struct BuildRound {
    std::size_t errors = 0, fixed = 0;
};

struct BuildResult {
    bool ok = false;
    std::optional<std::filesystem::path> exe;
    std::map<std::string, std::size_t> errors;   // C# error code -> how many are left
    std::vector<std::string> first;              // the first 60, "file(line): CODE text"
    std::string build_output_tail;               // the last 2000 bytes of the output when there is no error line to show
    PrepareReport report;
    std::vector<BuildRound> rounds;
};

// prepare, then build until it compiles or no fix changes anything (at most `rounds` times). The result is also written to
// reports/port.json.
BuildResult build(const base::converted::Project& project, const Tools& tools, const Hooks& hooks,
                  const std::optional<std::filesystem::path>& overlay = {}, std::size_t rounds = 6);

struct RunResult {
    std::optional<int> exit;   // nullopt: it was still running when the time was up
    std::string output;        // the last 6000 bytes of what it printed
    std::string log;           // the last 6000 bytes of its own xnaport.log
};
// Start the built game for `seconds` (with `env` over the environment); the job is ended when the time is up.
RunResult run_for(const std::filesystem::path& exe, std::chrono::seconds seconds, const std::map<std::string, std::string>& env = {});

// Every .xnb under the built program's Content/ loaded by the program itself: {asset: type, or "error: why"}; nullopt when it wrote
// nothing.
std::optional<std::map<std::string, std::string>> check_content(const std::filesystem::path& exe, std::chrono::seconds seconds = std::chrono::seconds(600));

// `xna port <project> [--overlay DIR] [--run SECONDS] [--check-content]`: 0 when built, 1 when not, 2 for a bad argument.
int main(std::span<const std::string> args, const base::converted::Store& store, const Tools& tools, const Hooks& hooks);

}  // namespace cb::xna::port
