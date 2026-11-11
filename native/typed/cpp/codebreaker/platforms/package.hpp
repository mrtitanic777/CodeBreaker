// codebreaker/platforms/package.hpp -- typed C++ port of codebreaker/platforms/__init__.py.
//
// platforms -- one description per platform, which every stage asks.
//
//     platforms::identify(path)        // the Platform a file belongs to, or nullptr
//     p.name, p.title, p.cpu, p.source // "pe", "32-bit Windows", "x86:LE:32:default:windows", "c"
//     p.lane_module()                  // its kind module's name (verify/<kind>.py)
//     p.run_function()                 // what runs its code: the emulator's run Ref, or nullopt
//     p.readable(path)                 // the file the stages read (a .xex: its basefile)
//     platforms::named("powerpc")      // by the name results are filed under (or its file's)
//     platforms::for_machine(0x1F2)    // a PE machine type's platform
//
// A platform is what a program was made for: the processor its code is for, the container the code
// comes in, the language its functions come back as, the module that reconstructs and judges it,
// and what runs its code beside the original's. Six slots (loader, cpu, convention, emulator,
// compiler, packager), each a record here.
//
// Differences from the Python (a typed port cannot import a module by name):
//   * Ref is data -- (module, name, leading args, keyword args). `Ref.target()` (import and look up)
//     and `Ref.__call__` do not exist; the C++ callers call the function they mean directly. What a
//     platform records as `Ref(...)` is still named here, and repr / pair / == / hash are the same.
//   * A slot's field is std::optional (None = not given). A field that is "a value, or a Ref"
//     (Compiler.flags) is a std::variant; Cpu.decoder is capstone-tuple | module name | none; a
//     Convention's `arguments` is a count | the word "registers".
//   * The Python Slot "given()" dict (field -> value) is given() here: (field, repr of value)
//     pairs in field order; repr(slot) is the Python repr text.
//   * Platform.readable() runs `readable_fn` (set for the XBE and XEX platforms) when the loader names
//     one; lane_module()/steps_module() give the module's NAME (no import).
//   * The decoder is capstone's, loaded from the capstone.dll the Python package carries
//     (emulate/capstone_x86.hpp load_capstone); `Decoder::disasm` returns the plain `Insn` below.
//   * all() builds the thirteen descriptors from the typed ports of each descriptor file
//     (platforms/win16, dos, vxd, nes, sms, gb, genesis, msx) and, for the five that have no port
//     of their own (java, win32, xbox360, xbox, zx), from the same values inline in package.cpp.
//     zx's recogniser needs formats.zx, which has no typed port: set_zx_reader() installs one
//     (without it a .sna/.z80/.tap file is judged by its extension and a 16-byte minimum).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::platforms {

extern const py::list<py::str> ORDER;
extern const py::list<py::str> SLOTS;

// A function or value of another module, named here (imported when first used, in the Python).
// Leading arguments (str) may be given, as functools.partial does: Ref("m", "f", "wcc").
struct Ref {
    py::str module;
    py::str name;
    py::list<py::str> args;
    py::list<std::tuple<py::str, py::str>> kw;   // keyword arguments (name, value)

    Ref() = default;
    Ref(const py::str& module_, const py::str& name_, const py::list<py::str>& args_ = {},
        const py::list<std::tuple<py::str, py::str>>& kw_ = {})
        : module(module_), name(name_), args(args_), kw(kw_) {}

    // (module, name), as the older tables wrote what runs a platform's code
    std::tuple<py::str, py::str> pair() const;
    py::str repr() const;
    friend bool operator==(const Ref& a, const Ref& b);
};
bool operator==(const Ref& a, const Ref& b);

// Loader: open the file.
struct Loader {
    std::optional<py::str> container;            // what the code comes in
    std::optional<Ref> open, readable, parts, entries, exports, imports;
    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;   // (field, repr of value)
    bool given_any() const;
    py::str repr() const;
};

// Cpu: the SLEIGH language id plus its fields.
struct Cpu {
    py::str language;                              // the value of the str subclass
    std::optional<int64_t> bits;
    std::optional<py::str> endian;
    struct Capstone { py::str arch; int64_t bits; };            // ("capstone", arch, bits)
    struct Module { py::str name; };                            // a module of CodeBreaker's own
    std::variant<std::monostate, Capstone, Module> decoder;
    std::optional<py::list<py::str>> registers;
    std::optional<py::str> stack;

    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;       // "language" first when non-empty
    py::str repr() const;                                       // the str's repr, as `str(p.cpu)` shows it
    bool truthy() const { return !language.empty(); }           // bool(str)
};

struct Convention {
    std::optional<py::list<py::str>> names;
    std::variant<std::monostate, int64_t, py::str> arguments;    // a count of words, or "registers"
    std::optional<int64_t> word;
    std::optional<py::str> order;
    std::optional<py::list<py::str>> results;
    std::optional<py::str> pops;
    std::optional<py::list<py::str>> preserved;
    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;
    bool given_any() const;
    py::str repr() const;
};

struct Emulator {
    std::optional<Ref> run, call;
    std::optional<py::str> engine;
    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;
    bool given_any() const;
    py::str repr() const;
};

struct Compiler {
    std::optional<py::str> language, toolchain;
    std::optional<Ref> tool, tools, env, compile, library;
    std::variant<std::monostate, py::list<py::str>, Ref> flags;  // a value (a list of options), or a Ref to one
    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;
    bool given_any() const;
    py::str repr() const;
    // The flags as a list (a Ref is not read here: nullopt for one -- its target lives in another module).
    std::optional<py::list<py::str>> options() const;
};

struct Packager {
    std::optional<Ref> link, functions, writer, tools, linker, assembler, resources;
    static const py::list<py::str>& FIELDS();
    py::list<std::tuple<py::str, py::str>> given() const;
    bool given_any() const;
    py::str repr() const;
};

using Recogniser = std::function<bool(const py::str& path)>;
using Reader = std::function<py::str(const py::str& path)>;

struct Platform {
    py::str name, title, family;
    std::optional<py::str> source;
    std::optional<py::str> lane;
    std::optional<int64_t> machine;
    std::optional<py::str> steps;
    std::optional<py::str> id;                    // (set by all(): the descriptor's file)
    Cpu cpu;
    Loader loader;
    Convention convention;
    Emulator emulator;
    Compiler compiler;
    Packager packager;
    Recogniser recognise_fn;
    Reader readable_fn;                           // the loader's `readable` function, when it has one

    // The older keywords of Platform(...): `runner` (module, function) fills emulator.run; `open` is
    // loader.readable; `container` loader.container. Call finish() after setting fields: it applies
    // the defaults the Python constructor does (steps).
    void finish(const std::optional<std::tuple<py::str, py::str>>& runner = std::nullopt,
                const std::optional<Ref>& open = std::nullopt, const py::str& container = py::str());

    py::str container() const;                    // loader.container or ""
    std::optional<std::tuple<py::str, py::str>> runner() const;   // (module, function) of its emulator's run
    // {slot: the slot's repr}, the six in order (SLOTS)
    py::list<std::tuple<py::str, py::str>> slots() const;
    bool recognises(const py::str& path) const;   // OSError and ValueError mean "no"
    py::str readable(const py::str& path) const;  // the file the stages read
    std::optional<py::str> lane_module() const;   // the module's name (the Python imports it)
    std::optional<py::str> steps_module() const;
    std::optional<Ref> run_function() const;      // emulator.run
    py::str repr() const;                         // <platform name: title>
};

// A machine whose programs were written in assembly and come back as assembly (the console and
// home-computer lanes): its reader, its processor, registers in and out, CodeBreaker's own
// interpreter, the assembler that rebuilds it and the writer of its source.
// `reader`, `runner`, `assembler`, `writer`: (module, function).
Platform machine(const py::str& name, const py::str& title, const py::str& family, const py::str& container,
                 const Cpu& cpu, const Recogniser& recognise, const std::tuple<py::str, py::str>& reader,
                 const std::tuple<py::str, py::str>& runner, const std::tuple<py::str, py::str>& assembler,
                 const std::tuple<py::str, py::str>& writer, const py::list<py::str>& results,
                 const py::str& toolchain = py::str("CodeBreaker's own assembler"));

// ---- the decoder (capstone) ------------------------------------------------------------------
// One decoded instruction: what capstone's CsInsn says that every caller reads.
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
    py::bytes bytes;
};

// capstone's error (ValueError in the Python for an unknown arch; ImportError when capstone cannot be loaded)
class Decoder {
  public:
    // md.disasm(code, address): the instructions from the start of `code`, up to the first byte
    // capstone cannot decode (the Python's generator, run to the end)
    py::list<Insn> disasm(const py::bytes& code, int64_t address) const;
    py::str arch;
    int64_t bits = 0;
    bool detail = false;
    uintptr_t handle = 0;                          // capstone's csh
};

// A capstone decoder for `arch` ("x86": 16, 32 or 64 bits; "ppc": 32 or 64 bits, big-endian, as the
// Xbox 360's), kept per thread and option set. ValueError for any other arch (or a bits capstone has
// no mode for: KeyError, as the Python's table raises it).
std::shared_ptr<Decoder> decoder(int64_t bits = 16, const py::str& arch = py::str("x86"), bool detail = false);
// decoder(cpu): the decoder its Cpu declares (ValueError when it declares none or a module of its own)
std::shared_ptr<Decoder> decoder(const Cpu& cpu, bool detail = false);

// ---- the registry ----------------------------------------------------------------------------
// Every platform, in the order they are tried.
const py::list<Platform>& all();
// The platform `path` belongs to, or nullptr when no platform recognises it.
const Platform* identify(const py::str& path);
// The platform results are filed under `name` (or whose descriptor is `name`.py), or nullptr.
const Platform* named(const py::str& name);
// named(kind), which must exist (ValueError "no platform %r").
const Platform& of(const py::str& kind);
// The platform of a PE image with this machine type, or nullptr.
const Platform* for_machine(int64_t machine);
// {name: kind module} for every platform that has a lane.
py::dict<py::str, py::str> lanes();
// {name: (module, function)} for every platform whose code CodeBreaker can run.
py::dict<py::str, std::tuple<py::str, py::str>> runners();
// {id: the module of its steps} for every platform on the engine.
py::dict<py::str, py::str> step_modules();
// The executable class of a file by its header (base/corpus.lane), cached there.
std::optional<py::str> lane_of(const py::str& path);
int64_t pe_machine(const py::str& path);
bool is_folder_or_java(const py::str& path);

// formats.zx has no typed port: install its reader (a .sna/.z80/.tap file's `read(path) is not None`;
// it may throw OSError/ValueError/IndexError, which mean "no"). Without one, zx judges by extension.
void set_zx_reader(const std::function<bool(const py::str& path)>& reader);
// formats.xbox.xbe.to_pe has no typed port either: install it (the XBE platform's `readable` calls it for a
// file that starts with XBEH; without one that call raises RuntimeError).
void set_xbe_to_pe(const std::function<py::str(const py::str& path)>& to_pe);

}  // namespace codebreaker::platforms
