// codebreaker/decompile/decompiler.hpp -- interface of codebreaker.decompile.decompiler (typed C++; implementation pending).
//
// decompiler.py -- CodeBreaker's decompiler: a module's functions as C.
//
//     auto module = decompiler::decompile(module_path);   // Module32: header, functions {address: (name, C text)}, order
//
// One decompiler for everything. The native program (native/decompiler) knows processors, not file formats: this module
// takes the file apart with the readers in codebreaker.formats and describes it (decompile/describe.hpp), runs the
// native program on the description, takes the notes apart from the C (split_notes) and, for the lanes that ask
// (l2=true), has the hub's C back end write each function's C from its L2 document (c_from_l2).
//
// Typing notes:
//   * run()/raw() answer `Decompiled`, the Python's tuple subclass: (texts, order, by_va, stderr) plus `notes`.
//   * A description is handed to run() as the path of its file (a str), as in the Python.
//   * decompile16() answers a record (Decompiled16) in place of (texts, order, by_va, Description); decompile_le() a
//     list of LeMember; decompile_dos() a DosDecompiled.  Description is shared (std::shared_ptr): the Python hands
//     the one object about and mutates it (`d.notes`, `d.names`, `d.lines`).
//   * A function's facts are core16::FunctionNotes; the "module" argument of c_from_l2 is c_text::ModuleFacts.
//   * decompile() answers c_text::Module32 (the l2.Made the Python builds with imports/in_ecx/in_edx/l2/declarations).
//   * The regular expressions that tests reach into (_BLOCK) are py::re::Pattern.
//   * Constants read from the settings/environment (_exe, _languages ...) are functions, as in the Python.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::describe {
class Description;   // codebreaker/decompile/describe.hpp
}

namespace codebreaker::decompile::decompiler {

using core16::FunctionNotes;
using core16::L2Doc;

// bump when what is fed to / taken from the native program changes
inline constexpr int64_t REV = 16;
// instructions a function may have, tried again when the first limit stops it
inline constexpr int64_t LARGE = 200000;

// `<settings decompiler_bin>/decompiler.exe`
py::str _exe();
// CB_LANGUAGES_DIR, else native/decompiler/languages under the project root
py::str _languages();
// "1" when the native program is to recover structures for this description (its CB_STRUCTS), else "0"; the setting
// `struct_recovery`: "off", "32" (32-bit x86 only) or "all". `description` is the description file's path.
py::str _structs(const py::str& description);
// The native program and its languages are there
bool available();
// CB_FRAME16=1: a 16-bit function's locals laid out as the original's frame (decompile/frame16.py, shape16.py)
bool frame16_on();
// Whether a 16-bit function's L2 tree is given passes that read its code (CB_ASM_BLOCKS, CB_FRAME16)
bool tree_images();
// {segment: bytes} of a 16-bit module's code, from its description file's `bytes` lines
py::dict<int64_t, py::bytes> _segment_images(const py::str& description);

// A block of the native program's output: `/*@@ <name> <hex address> */` and the text after it, up to the next
// (tests/test_verification reaches in)
const py::re::Pattern& _BLOCK();   // (compiled on first use)

// ---- run() -----------------------------------------------------------------------------------------
// (the C, its facts) of one function as the native program wrote it (the /*@G ... */ notes taken off its front)
std::tuple<py::str, FunctionNotes> split_notes(const py::str& text);

// run()'s (texts, order, by_va, stderr) -- the texts C alone -- and `notes`: {name: its facts}.
struct Decompiled {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    py::str stderr_;
    py::dict<py::str, FunctionNotes> notes;
};

// A function's C written by the hub's C back end from its L2 document (facts.l2) in place of the C the decompiler
// printed (`text`). Without a document, or one the back end declines, the printed C stays. Sets facts.from_l2 (and
// facts.l2/placed when `images` gives the passes that read the code). `images` {segment: code}; `module`: what the
// module says of the function, for the back end's passes that need it (it also receives what the back end reports).
// `text` None is c_text._write16's: the answer is then the C only when written (from_l2), else None.
std::optional<py::str> c_from_l2(const std::optional<py::str>& text, FunctionNotes& facts,
                                 const std::optional<py::dict<int64_t, py::bytes>>& images = std::nullopt,
                                 c_text::ModuleFacts* module = nullptr);

// As _run, the notes taken apart from the C (split_notes). `description` is the description file's path; `only` limits
// it to those addresses. With `l2` (None: the CB_C_FROM_L2 environment variable) each function's C is the hub's C back
// end's, from its L2 (c_from_l2). `write`=false: the L2 documents kept and the printed C handed on.
Decompiled run(const py::str& description, const py::list<int64_t>& only = py::list<int64_t>(), int64_t timeout = 3600,
               const std::optional<int64_t>& max_instructions = std::nullopt, const std::optional<bool>& l2 = std::nullopt,
               bool write = true);

// The language files' folder digest (cache.folder(_languages()))
py::str _language_stamp();
// The version of what makes decompiled C, as the cache keeps its results apart (base/cache.py GENERATIONS):
// (REV, the native program's digest, its languages' digest)
std::tuple<int64_t, py::str, py::str> generation();

// The native program's own output for a PE module, before it is made compilable
Decompiled raw(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir, int64_t timeout = 3600,
               const std::optional<bool>& l2 = std::nullopt, bool write = true);

// Decompile a module (every function, or the addresses in `funcs`). -> the Module (an l2.Made with imports ...)
c_text::Module32 decompile(const py::str& module_path, const std::optional<py::list<int64_t>>& funcs = std::nullopt,
                           const std::optional<py::str>& outdir = std::nullopt);

// ---- 16-bit modules ---------------------------------------------------------------------------------
// The addresses of places a function calls that are not known as functions yet (func_0x...): from its L2 document's
// calls, or without one (None) from its text
py::set<int64_t> _placeholders(const py::str& text, const std::optional<L2Doc>& doc);
// The number of arguments each call to `name` passes: in its L2 document's calls, or without one in `text`
py::list<int64_t> _call_arguments(const py::str& text, const py::str& name, const std::optional<L2Doc>& doc = std::nullopt);
// Does the function call `name`?
bool _calls(const py::str& text, const std::optional<L2Doc>& doc, const py::str& name);
// (count, [bytes of each, "2" | "4" | "4p"]) of the arguments a function declares -- in its L2 document (facts.l2), or
// without one in its own text -- or None
std::optional<std::tuple<int64_t, py::list<py::str>>> _own_arguments(const py::str& text, const py::str& name,
                                                                     const std::optional<FunctionNotes>& facts = std::nullopt);

// decompile16()'s answer: ({name: text}, [names], {address: name}, Description)
struct Decompiled16 {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    std::shared_ptr<describe::Description> description;
};
// Decompile a 16-bit Windows module. Function starts come first from what the file states; decompiling then shows
// calls to places not yet known as functions (`func_0x...`); those are added and the module is done again.
// Sets description->notes ({name: its facts}).
Decompiled16 decompile16(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt, int64_t rounds = 4);

// One virtual device driver member: (member name, {name: text}, [names], Description)
struct LeMember {
    py::str member;
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    std::shared_ptr<describe::Description> description;
};
// Decompile a virtual device driver file: one LE image, or every member of a container.
py::list<LeMember> decompile_le(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt);

// decompile_dos()'s answer: ({name: text}, [names], Description)
struct DosDecompiled {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    std::shared_ptr<describe::Description> description;
};
// Decompile a DOS program.
DosDecompiled decompile_dos(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt);

// A PE module is x86 (machine 0x014C)
bool _is_x86(const py::str& module_path);

// Any executable as C: (text, functions decompiled, functions found). Raises py::ValueError for a file that is not an
// executable this can read.
std::tuple<py::str, int64_t, int64_t> as_c(const py::str& path);

// `python -m codebreaker decompile <module> [--out FILE]`: the exit code
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::decompile::decompiler
