// codebreaker/build/functions.hpp -- interface of codebreaker.build.functions (typed C++; implementation pending).
//
// functions.py -- compile a module's reconstructed functions and link a DLL from them.
//
// One builder for every kind of module:
//
//     auto [dll, where, dropped] = functions::build16(header, texts, order, imports, outdir, segments);
//     auto [dll, where, dropped] = functions::build32(module_path, header, texts, order, imports, outdir, cflags);
//     auto [dll, where, dropped] = functions::build_powerpc(header, texts, order, externs, outdir, base);
//         where    {function name: where it is in the DLL} -- (segment, offset) for 16-bit, an address for 32-bit
//         dropped  {function name: why the compiler rejected it}
//
// Each function is compiled on its own, against a header that declares every function's convention and result. A function
// the compiler rejects cannot take others with it; it is replaced by a stand-in with the same signature, so its callers
// still link.
//
// Typing notes:
//   * `env` is the compiler's environment: {name: value} (build/watcom.hpp's _ow_env answers py::dict<py::str, py::str>).
//   * The three build functions answer a record each (Built16, Built32, BuiltPowerPC): the DLL path or None, where, dropped.
//   * `more` ({function: [extra switches]}) and `groups` ([(cflags, order, outdir)]) are as the Python has them.
//   * compile_groups answers [(stems, dropped)], one Compiled per group.
//   * The module-level dicts _HEADER and OBJECTS (build folder -> ...) are `extern` (tests read OBJECTS).
//   * Compile outcomes a caller keeps: _compile/_run_compile answer "" or the first error; compile_kept/_run_compile answer
//     (why, keepable).
//   * never_set answers the variables the compiler says are read but never assigned.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::functions {

// ---- constants ------------------------------------------------------------------------------------
// Medium model (far code, near data), as the module build has it; -ecd: a function declared without a convention is __stdcall
extern const py::list<py::str> CFLAGS16;
// The PowerPC compiler's options
extern const py::list<py::str> CFLAGS_POWERPC;

// build folder -> the hash of the header in it (used by build/module, engine/win16)
extern py::dict<py::str, py::str> _HEADER;
// _HEADER read and written under one lock (compiles run on threads): the header key of a build folder / set it
std::optional<py::str> header_key(const py::str& outdir);
void set_header_key(const py::str& outdir, const py::str& key);
// build folder -> {function: digest of its object code}, of the last build there
extern py::dict<py::str, py::dict<py::str, py::str>> OBJECTS;

// ---- texts ----------------------------------------------------------------------------------------
// The declaration of the function a text defines (everything before its body).
py::str prototype(const py::str& text);
// A definition with the same signature that does nothing.
py::str stand_in(const py::str& text);
// The C name behind a linker symbol (`_NAME@8`, `NAME_`, `_NAME`).
py::str _plain(const py::str& symbol);
// The options one function's text asks for on top of the module's -- the Windows prologue for an export, the frame kept
// for a function that reads where SP stood on entry.
py::list<py::str> _adjust_flags(const py::list<py::str>& cflags, const py::str& text);

// ---- compiling one function -----------------------------------------------------------------------
// Compile one function, running the compiler (no cache). `cflags` are already adjusted. -> (why, keepable): why is "" on
// success, else the first error; keepable says whether the outcome may be remembered. On success the object is at
// outdir/stem.obj.
std::tuple<py::str, bool> _run_compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text);

// An OpenWatcom object kept from a run in another folder, as the tool writes it run in `folder` -> bytes, or None when it
// cannot be said (then the tool is run).
std::optional<py::bytes> omf_here(const py::bytes& obj, const py::str& folder);
// A tool's message as the shared build cache keeps it: the folder it ran in as a mark, put back by message_here -- bytes.
py::bytes message_kept(const py::str& msg, const py::str& folder);
// message_kept's message, naming `folder` as the tool run there names it.
py::str message_here(const py::bytes& kept, const py::str& folder);

// What a compiler reads besides its command line and the source, by content: the program's bytes, the headers it may
// include and the environment variable of options it reads too (OpenWatcom's WCC, WASM). A cache key.
py::str tool_inputs(const py::str& cc, const std::optional<py::dict<py::str, py::str>>& env, const py::str& variable);

// What the engine already made (engine/win16: function.object, function.never_set): while the link runs, `_compile` and `never_set` of a
// function with the header, options and text such a fact was made from answer it, and the compiler is not started (the Python patches
// functions._compile for the length of the link). Keys: served_key(header key, options, text).
py::str served_key(const py::str& header_key, const py::list<py::str>& cflags, const py::str& text);
struct Served {
    py::str error;                  // the compiler's first error ("" when it made the object)
    py::bytes obj;                  // the object (when there is no error)
};
// Offer objects / flow checks to the next compiles; `unserve` takes them back. Counted: {"served", "compiled"} as the Python's _PATCHED.
void serve(const py::dict<py::str, Served>& objects, const py::dict<py::str, py::list<py::str>>& never_sets);
void unserve();
std::tuple<int64_t, int64_t> served_counts();

// _run_compile, kept in the shared build cache (kind "objects"). `cflags` are already adjusted. -> (why, keepable)
std::tuple<py::str, bool> compile_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text,
                                       const std::optional<py::str>& header_key);
// Compile one function. -> "" or the first error. (What the compiler made of this text is kept: compile_kept.)
py::str _compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env, const py::str& outdir,
                 const py::str& stem, const py::str& text);

// never_set, running the compiler (no cache). -> [name, ...]
py::list<py::str> _run_never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text);
// The variables a function reads that nothing in it ever sets, as the compiler sees them (W200).
py::list<py::str> never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                            const py::str& outdir, const py::str& stem, const py::str& text);
// _run_never_set, kept in the shared build cache (kind "never_set").
py::list<py::str> never_set_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text,
                                 const std::optional<py::str>& header_key);

// A digest of what a compiled object file IS -- its code, data, the places to patch and the names it defines and refers
// to -- and of nothing else. None when there is no file.
std::optional<py::str> object_digest(const py::str& path);

// ---- compiling every function ---------------------------------------------------------------------
// One folder's outcome: ({name: object file stem}, {name: why it was rejected})
struct Compiled {
    py::dict<py::str, py::str> stems;
    py::dict<py::str, py::str> dropped;
};
// Compile every function. `more` gives switches of a function's own: {name: [switch, ...]}.
Compiled compile_all(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                     const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                     const py::str& outdir, const std::optional<int64_t>& jobs = std::nullopt,
                     const std::optional<py::dict<py::str, py::list<py::str>>>& more = std::nullopt);
// One folder of compile_groups: (cflags, order, outdir)
struct Group {
    py::list<py::str> cflags;
    py::list<py::str> order;
    py::str outdir;
};
// compile_all for several folders at once, every function of every folder in the one pool. One Compiled per group.
py::list<Compiled> compile_groups(const py::str& cc, const py::dict<py::str, py::str>& env, const py::str& header,
                                  const py::dict<py::str, py::str>& texts, const py::list<Group>& groups,
                                  const std::optional<int64_t>& jobs = std::nullopt,
                                  const std::optional<py::dict<py::str, py::list<py::str>>>& more = std::nullopt);

// ---- linking --------------------------------------------------------------------------------------
// What a build answers: the DLL (None: nothing was linked), where each function is, and why each rejected one was.
// `where` is (segment, offset) for 16-bit (SegOffAt), an address for 32-bit / PowerPC (build32 / build_powerpc: AddressAt).
struct Built16 {
    std::optional<py::str> dll;
    py::dict<py::str, std::tuple<int64_t, int64_t>> where;
    py::dict<py::str, py::str> dropped;
};
struct Built32 {
    std::optional<py::str> dll;
    py::dict<py::str, int64_t> where;
    py::dict<py::str, py::str> dropped;
};
using BuiltPowerPC = Built32;   // (image path, {name: address} for functions and externs, {name: why it was rejected})

// `code_segment` {name: the number of the original's segment the function is in}: the functions of one segment are put in
// one segment again. `interrupts` {vector}: the software interrupts the texts take (c_text: m.interrupts). `code_data`
// {segment: C}: the original's data its functions read through CS, compiled into that code segment. `code_image`
// {segment: bytes}: the original's whole segment, put at the start of the rebuilt one. `imports` {symbol: (module, ordinal)}.
// `segments` {segment: bytes} other segments the code reads from.
Built16 build16(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                const py::dict<py::str, std::tuple<py::str, int64_t>>& imports, const py::str& outdir,
                const std::optional<py::dict<int64_t, py::bytes>>& segments = std::nullopt,
                const std::optional<int64_t>& jobs = std::nullopt,
                const std::optional<py::dict<py::str, int64_t>>& code_segment = std::nullopt,
                const std::optional<py::set<int64_t>>& interrupts = std::nullopt,
                const std::optional<py::dict<int64_t, py::str>>& code_data = std::nullopt,
                const std::optional<py::dict<int64_t, py::bytes>>& code_image = std::nullopt);

// `originals` {name: address}: where a function the build lacks is jumped to in the original. `cflags`: the module's.
Built32 build32(const py::str& module_path, const py::str& header, const py::dict<py::str, py::str>& texts,
                const py::list<py::str>& order, const py::list<py::str>& used_imports, const py::str& outdir,
                const py::list<py::str>& cflags, const std::optional<int64_t>& jobs = std::nullopt,
                const std::optional<py::dict<py::str, int64_t>>& originals = std::nullopt);

// ---- PowerPC --------------------------------------------------------------------------------------
// (compiler, linker, runtime library), or None when settings.json does not name them.
std::optional<std::tuple<py::str, py::str, py::str>> powerpc_tools();

// Compile each function with the PowerPC compiler and link one image at `base`. `externs` are the names of functions
// outside the module (imports): each gets a place of its own in the image.
BuiltPowerPC build_powerpc(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                           const py::dict<py::str, int64_t>& externs, const py::str& outdir, int64_t base,
                           const std::optional<int64_t>& jobs = std::nullopt);

// ---- running a program ----------------------------------------------------------------------------
// subprocess.run(args, cwd=, env=os.environ + `env`, capture_output=True, text=True, errors="replace") -> (returncode, stdout,
// stderr). The variables of `env` are in the program's environment while it runs (threads that run at once with the same
// variables share them); the Python's `timeout=` is not kept (the runtime's subprocess has none).
std::tuple<int64_t, py::str, py::str> run_process(const py::list<py::str>& args, const std::optional<py::str>& cwd,
                                                  const py::dict<py::str, py::str>& env);

}  // namespace codebreaker::build::functions
