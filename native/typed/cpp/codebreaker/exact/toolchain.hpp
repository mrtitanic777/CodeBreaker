// codebreaker/exact/toolchain.hpp -- typed C++ of codebreaker.exact.toolchain
//
// toolchain.py -- a compiler (or assembler) of the period, run in this process from its profile.
//
// A profile (toolchains/NAME.json) says what the tool is and how its driver ran it; nothing about one
// compiler is written here. Its parts:
//
//     kit         the settings.json `kits` entry holding the tool's files (they are not in the repository)
//     emulator    dos | xenix | isis
//     stage       {"files": {name in the stage: path in the kit}, "include": kit folder of headers}
//     psp         the PSP's segment (a pass that must be loaded low or high)
//     env         environment strings; "env_unless": {string: regex} -- left out when the switches match
//     switches    the default switches; "file_switches": NAME -- a source's first line /* NAME: ... */
//                 gives its own
//     derive      {name: {"regex", "then", "else"}} -- values made from the switches, for the command
//                 lines ({1} in "then" is the regex's first group)
//     passes      [{"run": program, "args": template}] -- {tag} the file's stem, {tag6} its first six
//                 letters, {name} any derived value; one pass's failure stops the compile
//     output      the object file's name, a template
//     patches     [{"change", "file", "at", "was", "now", "note"}] -- bytes changed in a copy of a pass
//                 (at: an offset in its load image; was: what must be there, else the kit is not this
//                 one); "variants": {name: [changes]} -- which of them a variant of the compiler made
//     hooks       {name: {"pass", "at": [addresses], "set": {register: value}, "header": WORD}} -- each
//                 time the pass reaches an address the count goes up; at the counts a source lists in a
//                 /* WORD: 1,2 */ line of its head, the registers are set (a pass's question answered)
//     rename_externs  {name: name} -- external names changed in the object (a compiler that calls its
//                 helpers by other names than the library the program was linked with)
//
//     tc = toolchain::load("msc300", "own");
//     auto [obj, console] = tc->compile(source_bytes, {{"tree.h", b"..."}}, {folder}, switches);   // obj: the object file's bytes, or None
//
// Typing notes (the Python's duck-typed objects):
//   * The profile is JSON; its parts are the structs below (Profile, Stage, Pass, Patch, Hook, Derive, Variant).  The
//     keys the Python reads with [] (name, passes[].run, patches[].change / file / at / was / now, hooks[].pass / at / set)
//     are required when the profile is read (the Python raises KeyError where it first uses one).  A value that is
//     an int or a string written as a number ("0x749D": psp, top, at, set) is a Num; a patch's "change" is an int or a
//     word (variants name them).  What the profile holds beyond the parts the Toolchain reads is kept only as the
//     text its key is made from (json.dumps(.., sort_keys=True)): the stage folder and cache entries of this class
//     are named exactly as the Python's.
//   * `p.get(key)` of a str entry (modules.py asks for "switches", "file_switches", "segment_switch") is p_get().
//   * Settings: the root of the checkout is CB_CHECKOUT_ROOT (py::settings); `kits` is read from its settings.json
//     (a dict is not a py::settings type); the state folder is the setting `state`.
//   * A compile's cache entry is cache kind "exact" (codebreaker::base::cache): the same sqlite, the same key.
//   * str.format(**values) in the profile's templates is `format_named`: {name}, {{ and }} -- the only forms profiles use.
//   * dos.run: dos::run of the C++ executive.  The Python executive's hooks (a pass instrumented: counted hooks,
//     "call" hooks, in-memory patches) need code hooks in the emulator that dos::run does not offer; _hook, _call_hooks
//     and _in_memory are ported over HookEngine (an Engine that can hook code) and the run goes through `Runner`,
//     which a program with an instrumenting executive sets (set_runner).  The built-in Runner runs a pass without
//     instrumentation and raises NotImplementedError for one with.  A "call" hook is a C++ function registered under
//     "module:function" (register_hook; hooks_lattice:cmp_test is registered).
//   * A profile with "plugin" (a toolchain whose running is code: asm86, plm86, mspascal, xenixc) is made by a
//     factory registered under "module:Class" (register_plugin); without one load() raises ModuleNotFoundError.
//   * Names of the process' own files: tag = "T%05d" % ((pid * 7 + counter) % 100000) as the Python's -- so a console
//     or an object module carries the tag of the process that made it.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/modules.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::toolchain {

extern const py::str CACHE_VERSION;

// the folder codebreaker/exact of the checkout (the Python's HERE)
py::str HERE();

// the folder of the kit `name`: the environment CB_KIT_NAME, else settings.json `kits`
py::str kit_dir(const py::str& name);

// a file named in a profile or project: "KIT:path" in that kit, else in `kit`, else from `base`
py::str kit_path(const std::optional<py::str>& kit, const py::str& path, const std::optional<py::str>& base = std::nullopt);

// the names of the profiles, sorted
py::list<py::str> profiles();

using Num = std::variant<int64_t, py::str>;          // an int, or a number written as text
int64_t _int(const Num& x);                           // int(x, 0) if it is text
using Change = std::variant<int64_t, py::str>;        // a patch's number or name

struct Derive {
    py::str regex;
    py::str then_;                                    // "then" (default "")
    py::str else_;                                    // "else" (default "")
};

struct Pass {
    py::str run;
    py::str args;                                     // default ""
    py::list<py::str> env;
    std::optional<py::str> listing;
};

struct Patch {
    Change change;
    py::str file;
    Num at;
    py::str was;                                      // hex
    py::str now;                                      // hex
    bool in_memory = false;
};

struct Hook {
    py::str pass_;                                    // "pass"
    py::list<Num> at;
    py::dict<py::str, Num> set;
    std::optional<py::str> header;
    std::optional<py::str> call;                      // "module:function"
};

struct Stage {
    py::dict<py::str, py::str> files;
    std::optional<py::str> include;
    bool all = false;
};

struct Variant {                                      // a list of changes, or {"patches": [..], "hooks": [..]}
    py::list<Change> patches;
    py::list<py::str> hooks;
};

struct Profile {
    py::str name;
    std::optional<py::str> kit;
    std::optional<py::str> default_variant;
    py::str emulator = py::str("dos");
    Stage stage;
    std::optional<Num> psp;                           // p.get("psp", 0x100): absent is 0x100, a null is None
    bool psp_set = false;                             // the key is there
    std::optional<Num> top;
    py::list<py::str> env;
    py::dict<py::str, py::str> env_unless;
    std::optional<py::str> switches;
    std::optional<py::str> file_switches;
    std::optional<py::str> file_switches_comment;
    py::dict<py::str, Derive> derive;
    py::list<Pass> passes;
    bool has_passes = false;
    std::optional<py::str> output;
    py::str source_ext = py::str(".C");
    bool source_eof = false;
    py::list<Patch> patches;
    py::dict<py::str, Variant> variants;
    py::dict<py::str, Hook> hooks;
    py::dict<py::str, py::str> rename_externs;
    bool has_rename_externs = false;
    std::optional<py::str> plugin;
    py::dict<py::str, py::str> strings;               // every top-level entry that is a str (p.get(key) of a str)
    py::str keyed;                                    // json.dumps(profile with its passes' "listing" left out, sort_keys=True)
};

// json.load of toolchains/NAME.json (or of the text given)
Profile profile_from_json(const py::str& text);

// ---- an emulator whose code can be hooked (unicorn's `mu` / `uc` of an instrument)
class HookEngine : public emu::dpmi::Engine {
  public:
    using CodeHook = std::function<void(HookEngine& uc, int64_t address, int64_t size)>;
    // mu.hook_add(UC_HOOK_CODE, fn, begin=, end=)
    virtual void hook_code(int64_t begin, int64_t end, const CodeHook& fn) = 0;
};
using Instrument = std::function<void(HookEngine& mu, int64_t load)>;           // given the engine and the load segment
using When = std::function<bool(HookEngine& uc, int64_t addr, int64_t load)>;   // may the instrument be put in yet?

// unicorn's UC_X86_REG_<NAME> (NAME in upper case: AX, BP, IP ...); KeyError-free: AttributeError for another
int64_t x86_reg(const py::str& name);

// _hook: at each time the pass reaches one of h.at the count goes up; at the counts given the registers h.set are set
Instrument _hook(const Hook& h, const py::set<int64_t>& counts);
// _call_hooks: at each address of each hook the function it names is given (read word(seg, off), bp, ss, ds) and says
// where execution goes on (an IP), or nothing to go on as it was; `then` is run after
using Reader = std::function<int64_t(int64_t, int64_t)>;
using HookFn = std::function<std::optional<int64_t>(const Reader& rd, int64_t bp, int64_t ss, int64_t ds)>;
void register_hook(const py::str& module_function, const HookFn& fn);
Instrument _call_hooks(const py::list<Hook>& hooks, const std::optional<Instrument>& then = std::nullopt);
// _in_memory: the bytes changed once the pass has unpacked itself -> (instrument, when)
std::tuple<Instrument, When> _in_memory(const py::list<Patch>& mem, const std::optional<Instrument>& then = std::nullopt);

// ---- running a pass
struct RunArgs {
    py::str exe;
    py::str workdir;
    py::str cmdline;
    py::bytes env;
    std::optional<Instrument> instrument;
    std::optional<When> instrument_when;
    std::optional<int64_t> psp = 0x100;
    std::optional<int64_t> top;
    bool fast = false;
    const emu::dos::MemFS* fs = nullptr;
};
using Runner = std::function<emu::dos::Result(const RunArgs&)>;
void set_runner(const Runner& r);                     // the executive that runs passes (an empty one: the built-in dos::run, no instruments)
void set_memfs(bool on);                              // the module's MEMFS: a compile's files kept in memory, or written to the stage (CB_MEMFS=0 at the start turns it off)
void set_tag_counter(int64_t n);                  // the counter the names of a compile's files are made from (the next compile uses n + 1): a test makes two processes name them alike

// ---- the toolchain
class Toolchain : public modules::Toolchain {
  public:
    Profile p;
    py::str name;
    py::str variant;
    py::list<py::str> variant_hooks;
    py::list<Patch> patches;
    py::str key;

    Toolchain(const Profile& profile, const std::optional<py::str>& variant = std::nullopt);
    virtual ~Toolchain() = default;

    // tc.p.get(key) of a str entry
    std::optional<py::str> p_get(const py::str& key) const override;

    // the stage: the tool's files, patched as the variant has them
    py::str stage() const;

    // one source
    py::str switches_of(const py::bytes& src, const std::optional<py::str>& switches = std::nullopt) const;
    py::dict<py::str, py::list<int64_t>> answers_of(const py::bytes& src) const;
    py::dict<py::str, py::str> _values(const py::str& sw, const py::str& tag) const;
    py::bytes _env(const py::str& sw) const;

    // -> (object bytes or None, what the tool printed)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override;
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include, const std::optional<py::str>& switches,
                                                          bool cache) const;
    // (object, printed, the listing's text): the pass the profile's "listing" names writing its listing of the code
    // (the source line each comes from); a profile without one gives ""
    std::tuple<std::optional<py::bytes>, py::str, py::str> compile_listing(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                                           const py::list<py::str>& include,
                                                                           const std::optional<py::str>& switches, bool cache = true) const;

    // the cache key of a compile of `src` with these headers and include files (read from the folders), as the Python makes it
    py::str compile_key(const py::bytes& src, const py::dict<py::str, py::bytes>& headers, const py::list<py::str>& include,
                        const std::optional<py::str>& switches) const;

    // -> (exit code, what the passes printed); `listing_args`: the pass (index) whose args are given instead
    std::tuple<std::optional<int64_t>, py::bytes> _run(const py::str& w, const py::str& sw, const py::str& tag,
                                                       const py::dict<py::str, py::list<int64_t>>& ans) const;

  private:
    std::tuple<std::optional<py::bytes>, py::str> compile_(const py::bytes& src0, const py::dict<py::str, py::bytes>& headers0,
                                                           const py::list<py::str>& include, const std::optional<py::str>& switches,
                                                           bool cache) const;
    mutable const emu::dos::MemFS* fs_ = nullptr;                 // self._fs
    mutable std::optional<py::str> listing_;                      // self._listing
    mutable std::optional<std::tuple<int64_t, py::str>> args_override_;     // (pass index, args) while a listing is asked for
};

using Plugin = std::function<std::shared_ptr<Toolchain>(const Profile&, const std::optional<py::str>& variant)>;
void register_plugin(const py::str& module_class, const Plugin& make);

// toolchain.load(name, variant): kept, one for each (name, variant)
std::shared_ptr<Toolchain> load(const py::str& name, const std::optional<py::str>& variant = std::nullopt);

// str.format(**values) of a profile's template
py::str format_named(const py::str& tmpl, const py::dict<py::str, py::str>& values);

}  // namespace codebreaker::exact::toolchain
