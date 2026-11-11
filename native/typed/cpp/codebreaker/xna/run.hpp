// codebreaker/xna/run.hpp -- typed C++ of codebreaker.xna.run.
//
// run.py -- `codebreaker xna <package, folder or game .exe> [--name N] [--no-content]`: an XNA game as
// a Converted project; `codebreaker xna port <project>` then builds its C# for the PC (xna/port.py).
//
//     Converted/<name>/
//       original/   what came in (the package, or the game's .exe)
//       assets/     the game's files as it ships them (from a package: unpacked), and content/: every
//                   .xnb converted (formats/xnb.py) -- textures and fonts as PNG (+ JSON), at their paths
//       src/        C#: one project per assembly of the game's own (ILSpy, then fixups.py)
//       reports/    content.json (each .xnb: kind, size, readers, or why it did not convert),
//                   decompile.json (each assembly, and what the fix-ups left)
//
// Typing notes:
//   * formats/xnb.py and formats/media.py have no typed port: what run.py asks of them is `Hooks` (media.extract,
//     xnb.convert, xnb.convert_effect_file), passed to convert() and main().  An xnb record is a JSON object
//     (the Python's dict with the keys "kind" / "error" / "folder" and the rest the converter wrote).
//   * subprocess.run(env=...) and timeouts are not in the runtime's py::subprocess::run: the environment the
//     Python builds with dict(os.environ, ...) is set on the process for the length of the call (detail::EnvGuard);
//     no call of this module passes a timeout.
//   * The helpers in `detail` (state paths, text files as Python's open(..., encoding="utf-8-sig") reads them, JSON
//     as json.dump(indent=1) writes it, copytree) are shared with port.cpp.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/base/converted.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::xna::run {

using Json = knowledge::sources::Json;

// The assemblies the game's own are not: Microsoft.*, System.*, mscorlib.
extern const py::list<py::str> FRAMEWORK;

// What run.py takes from formats/media.py and formats/xnb.py.
struct Hooks {
    // media.extract(source, game_dir): falsy when nothing was in it to read
    std::function<bool(const py::str& source, const py::str& game_dir)> media_extract;
    // xnb.convert_effect_file(path, out_dir): the record of a non-.xnb file that is an effect, or None
    std::function<std::optional<Json>(const py::str& path, const py::str& out_dir)> convert_effect_file;
    // xnb.convert(path, out_dir): the record of an .xnb (a JSON object)
    std::function<Json(const py::str& path, const py::str& out_dir)> xnb_convert;
};

// ILSpy's command line: the `ilspycmd` setting, PATH, or ~/.dotnet/tools; RuntimeError when none.
py::str ilspycmd();

// The game's own assemblies under `folder` ([path]), the .exe first.
py::list<py::str> assemblies(const py::str& folder);

// de4dot's program, or None: the `de4dot` setting, PATH, or a build beside the checkout (../_tools/de4dot).
std::optional<py::str> de4dot();

// The obfuscator de4dot finds in `assembly`, or None (none, or de4dot not installed).
std::optional<py::str> obfuscator(const py::str& assembly);

// `assembly` cleaned by de4dot (cached under state/xna/deobfuscated by its contents), with what it was
// obfuscated with -- or (assembly, None) when nothing was.
std::tuple<py::str, std::optional<py::str>> deobfuscated(const py::str& assembly);

void decompile(const py::str& assembly, const py::str& out_dir);

// One assembly of convert()'s report: the keys of the Python dict, in order.
struct AssemblyReport {
    py::str assembly;                                   // relative to the game's folder
    py::str project;
    std::optional<py::str> deobfuscated;
    int64_t ref_casts = 0, constructors = 0;
    py::list<std::tuple<py::str, int64_t, py::str>> left;   // (file, line, why)
};
struct Report {
    py::list<AssemblyReport> assemblies;
};

// -> (the project, the report, {content kind: count})
std::tuple<base::converted::Project, Report, py::dict<py::str, int64_t>> convert(
    const py::str& source, const std::optional<py::str>& name, bool content, const Hooks& hooks);

// The module's usage text (its docstring).
extern const py::str DOC;

// `port_main` is xna/port.py's main (the Python imports it when the first word is "port": the two modules import
// each other, so here it is handed in).
int64_t main(const py::list<py::str>& argv, const Hooks& hooks,
             const std::function<int64_t(const py::list<py::str>&)>& port_main);

// ---- helpers shared with port.cpp -------------------------------------------------------------------------------
namespace detail {

// dict(os.environ, **overrides) for the length of one call: the variables are set on the process, then put back.
class EnvGuard {
  public:
    EnvGuard(const std::vector<std::pair<std::string, std::string>>& set);
    ~EnvGuard();
    EnvGuard(const EnvGuard&) = delete;
    EnvGuard& operator=(const EnvGuard&) = delete;

  private:
    std::vector<std::pair<std::string, std::optional<std::string>>> saved_;
};

// settings.state(*parts): a path inside the state folder; its folder (or itself, with no parts) is created.
py::str state(const std::vector<py::str>& parts);

// open(path, encoding="utf-8-sig"[, errors="replace"]).read(): the BOM dropped, newlines translated to "\n".
py::str read_text_sig(const py::str& path, bool replace_errors = false, bool strip_bom = true);
// open(path, "w", encoding="utf-8").write(text): "\n" written as "\r\n" (Windows).
void write_text(const py::str& path, const py::str& text);

// json.dump(value, f, indent=1)
py::str json_dumps_indent1(const Json& value);
void write_json(const py::str& path, const Json& value);

Json jstr(const py::str& s);
Json jint(int64_t i);
Json jbool(bool b);
Json jnone();
Json jlist(const std::vector<Json>& items);
Json jdict(const std::vector<std::pair<py::str, Json>>& items);

// shutil.copytree(src, dst): dst must not exist.
void copytree(const py::str& src, const py::str& dst);

}  // namespace detail

}  // namespace codebreaker::xna::run
