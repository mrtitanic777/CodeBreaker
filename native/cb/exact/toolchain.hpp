// cb/exact/toolchain.hpp -- a compiler (or assembler) of the period, run in this process from its profile.
//
// A profile (config/exact/toolchains/NAME.json) says what the tool is and how its driver ran it; nothing about one
// compiler is written in the code. Its parts:
//
//     kit         the settings.json `kits` entry holding the tool's files (they are not in the repository)
//     emulator    dos | xenix | isis
//     stage       {"files": {name in the stage: path in the kit}, "include": kit folder of headers}
//     psp         the PSP's segment (a pass that must be loaded low or high)
//     env         environment strings; "env_unless": {string: regex} -- left out when the switches match
//     switches    the default switches; "file_switches": NAME -- a source's first line /* NAME: ... */ gives its own
//     derive      {name: {"regex", "then", "else"}} -- values made from the switches, for the command lines ({1} in "then"
//                 is the regex's first group)
//     passes      [{"run": program, "args": template}] -- {tag} the file's stem, {tag6} its first six letters, {name} any
//                 derived value; one pass's failure stops the compile
//     output      the object file's name, a template
//     patches     [{"change", "file", "at", "was", "now", "note"}] -- bytes changed in a copy of a pass (at: an offset in
//                 its load image; was: what must be there, else the kit is not this one); "variants": {name: [changes]} --
//                 which of them a variant of the compiler made
//     hooks       {name: {"pass", "at": [addresses], "set": {register: value}, "header": WORD}} -- each time the pass reaches
//                 an address the count goes up; at the counts a source lists in a /* WORD: 1,2 */ line of its head, the
//                 registers are set (a pass's question answered)
//     rename_externs  {name: name} -- external names changed in the object (a compiler that calls its helpers by other
//                 names than the library the program was linked with)
//
//     Toolchains toolchains(locations, &runner, &cache);
//     auto tc = toolchains.load("msc300", "own");
//     CompileResult r = tc->compile(source_bytes, {.headers = {{"tree.h", bytes}}, .include = {folder}});   // r.object: the object file's bytes, or none
//
// A compile is cached (kind "exact", under the source, the headers and include files, the switches, the sources' answers and
// the profile's own digest), so the same source compiled again is not run again.
//
// A pass is run by a PassRunner -- the DOS executive of exact/emu/dos for a plain pass. A pass that the profile instruments (a
// counted hook, a call hook, a memory patch) is described as data (Instrument); a runner that cannot carry it out says so
// (UnsupportedError), and nothing is silently run without it.
//
// A profile with "plugin" (a toolchain whose running is code: asm86, plm86, mspascal, xenixc) is made by a factory registered
// by name on the Toolchains; without one, loading it is an UnsupportedError.
//
// Nothing here is global: the locations, the runner, the cache, the registered hooks and plugins, the stages in use and the
// toolchains loaded belong to the Toolchains object.
#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/regex.hpp"
#include "cb/exact/emu/dos.hpp"
#include "cb/exact/modules.hpp"

namespace cb::exact::toolchain {

inline constexpr std::string_view kCacheVersion = "1";
inline constexpr std::string_view kCacheKind = "exact";   // the cache kind compiles are kept under (the owner of the cache declares it)

// ---- where things are ----------------------------------------------------------------------------------------------
struct Locations {
    std::filesystem::path profiles;        // toolchains/NAME.json: config/exact/toolchains of the checkout
    std::filesystem::path state;           // the state folder: stages are kept in <state>/exact/stage
    std::filesystem::path checkout_root;   // a relative kit folder is taken against it
    std::map<std::string, std::filesystem::path> kits;   // settings.json "kits": {name: folder}
    std::string stage_tag;                 // CB_STAGE_TAG: a worker of a parallel run has a stage of its own (the cache is shared)

    // The locations of the checkout at `checkout_root`: its settings.json (kits, state) and the environment (CB_STAGE_TAG).
    static Locations of_checkout(const std::filesystem::path& checkout_root);

    // The folder of the kit `name`: the environment CB_KIT_<NAME>, else the kits setting. UsageError when it is not set up.
    std::filesystem::path kit(std::string_view name) const;
    // A file named in a profile or project: "KIT:path" in that kit, else in `kit`, else from `base`.
    std::filesystem::path kit_path(const std::optional<std::string>& kit_name, std::string_view path,
                                   const std::filesystem::path& base = ".") const;
    // The names of the profiles, sorted.
    std::vector<std::string> profile_names() const;
};

// ---- the profile ---------------------------------------------------------------------------------------------------
struct Derive {
    std::string regex, then, otherwise;   // `otherwise` is the profile's "else"
};
struct Pass {
    std::string run, args;
    std::vector<std::string> env;
    std::optional<std::string> listing;   // the switches that make the pass write a listing
};
using Change = std::variant<std::int64_t, std::string>;   // a patch's number or name
struct Patch {
    Change change;
    std::string file;
    std::int64_t at = 0;
    Bytes was, now;
    bool in_memory = false;   // changed once the pass has unpacked itself, not in the stage's copy
};
struct Hook {
    std::string pass;
    std::vector<std::int64_t> at;
    OrderedMap<std::string, std::int64_t> set;   // register -> value
    std::optional<std::string> header;           // the word a source's head answers the question under
    std::optional<std::string> call;             // the registered name of a function that decides where the pass goes on
};
struct Stage {
    OrderedMap<std::string, std::string> files;   // name in the stage -> path in the kit
    std::optional<std::string> include;
    bool all = false;                             // every file of the kit, its folders flattened
};
struct Variant {
    std::vector<Change> patches;
    std::vector<std::string> hooks;
};

struct Profile {
    std::string name;
    std::optional<std::string> kit, default_variant;
    std::string emulator = "dos";
    Stage stage;
    std::optional<std::int64_t> psp = 0x100;   // absent: 0x100; a null: none
    std::optional<std::int64_t> top;           // a zero is none
    std::vector<std::string> env;
    OrderedMap<std::string, std::string> env_unless;
    std::optional<std::string> switches, file_switches, file_switches_comment;
    OrderedMap<std::string, Derive> derive;
    std::vector<Pass> passes;
    std::optional<std::string> output;
    std::string source_ext = ".C";
    bool source_eof = false;
    std::vector<Patch> patches;
    OrderedMap<std::string, Variant> variants;
    OrderedMap<std::string, Hook> hooks;
    OrderedMap<std::string, std::string> rename_externs;
    std::vector<std::string> try_switches;     // switches that change the code, for `identify` to try
    std::optional<std::string> plugin;
    std::map<std::string, std::string> strings;   // every top-level entry that is a string (the rest of the code asks by key)
    std::string digest;                        // of the profile without its passes' listing switches: names a stage, keys the cache
};

// The profile written in `text` (a JSON document). ParseError for text that is not JSON, FormatError for a profile that lacks
// what it must have (a name, a pass's program, a patch's file and bytes, a hook's pass and addresses).
Profile parse_profile(std::string_view text);
// toolchains/NAME.json.
Profile read_profile(const Locations& locations, std::string_view name);

// ---- running a pass ------------------------------------------------------------------------------------------------
// What is done to a pass as it runs.
struct CountedHook {                             // each time the pass reaches an address the count goes up (shared by the
    std::vector<std::int64_t> at;                // addresses); at the counts, the registers are set
    std::set<std::int64_t> counts;
    OrderedMap<std::string, std::int64_t> registers;
};
// Given a reader of words (segment, offset), BP, SS and DS: where the pass goes on (an IP), or nothing to go on as it was.
using WordReader = std::function<std::int64_t(std::int64_t segment, std::int64_t offset)>;
using HookFn = std::function<std::optional<std::int64_t>(const WordReader& read_word, std::int64_t bp, std::int64_t ss, std::int64_t ds)>;
struct CallHook {
    std::vector<std::int64_t> at;
    HookFn fn;
};
struct MemoryPatch {                             // written at the load segment's offset once `was` is there
    std::int64_t at = 0;
    Bytes was, now;
};
struct Instrument {
    std::vector<CountedHook> counted;
    std::vector<CallHook> calls;
    std::vector<MemoryPatch> memory;
    bool empty() const { return counted.empty() && calls.empty() && memory.empty(); }
};

struct PassRun {
    std::filesystem::path exe, workdir;
    std::string command_tail;
    Bytes environment;                           // NUL-terminated strings
    Instrument instrument;
    std::optional<std::int64_t> psp, top;
    bool fast = false;                           // no hook of ours in the pass
    emu::dos::MemFS* files = nullptr;            // the run's files in memory (none: the disk)
};

class PassRunner {
public:
    virtual ~PassRunner() = default;
    virtual emu::dos::Result run(const PassRun& pass) const = 0;
};

// The DOS executive as a runner. A pass with an instrument is an UnsupportedError: the executive has no code hooks.
class ExecutiveRunner final : public PassRunner {
public:
    explicit ExecutiveRunner(const emu::dos::Executive& executive) : executive_(executive) {}
    emu::dos::Result run(const PassRun& pass) const override;

private:
    const emu::dos::Executive& executive_;
};

// ---- the toolchain -------------------------------------------------------------------------------------------------
struct Compiled {
    std::optional<Bytes> object;
    std::string console;               // what the passes printed
    std::optional<std::string> listing;   // the pass the profile's "listing" names writing its listing of the code (the source line each comes from)
};

// What a toolchain is run with.
struct Context {
    const Locations* locations = nullptr;
    const PassRunner* runner = nullptr;
    base::cache::Cache* cache = nullptr;                      // none: nothing is kept
    const std::map<std::string, HookFn>* hooks = nullptr;     // by the name a profile's "call" gives
    std::atomic<std::uint64_t>* tags = nullptr;               // the counter the names of a compile's files are made from
    bool memfs = true;                                        // a compile's files kept in memory, not written to the stage
};

class Toolchain : public modules::Toolchain {
public:
    // UsageError for a variant the profile does not have.
    Toolchain(Profile profile, std::optional<std::string> variant, const Context& context);
    ~Toolchain() override = default;

    const Profile& profile_data() const { return profile_; }
    const std::string& name() const { return profile_.name; }
    const std::string& variant() const { return variant_; }
    // The profile's digest and variant: names the stage, keys the cache.
    const std::string& key() const { return key_; }
    const std::vector<Patch>& patches() const { return patches_; }

    // A string entry of the profile ("switches", "file_switches", "segment_switch"), or none.
    std::optional<std::string> profile(std::string_view key) const override;

    // The stage: the tool's files, patched as the variant has them. A kit that is not this compiler's (a patch's `was` is
    // not there) is a FormatError naming the change.
    std::filesystem::path stage() const;

    // The switches of one source: its own first line's, else `switches`, else the profile's.
    std::string switches_of(ByteView source, const std::optional<std::string>& switches = std::nullopt) const;
    // What the head of a source answers to the profile's hooks: {hook name: sorted counts}.
    std::map<std::string, std::vector<std::int64_t>> answers_of(ByteView source) const;
    // The values the command lines are made from: tag, tag6, switches and each derived value.
    std::map<std::string, std::string> values(const std::string& switches, const std::string& tag) const;
    // The environment of the passes for these switches.
    Bytes environment(const std::string& switches) const;

    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override;
    // The same with the listing, and with the cache not used when `cache` is false.
    Compiled compile_full(ByteView source, const modules::CompileOptions& options, bool want_listing = false, bool cache = true) const;

    // The cache key of a compile of `source` with these headers and include files (the folders are read).
    std::string compile_key(ByteView source, const modules::CompileOptions& options) const;

private:
    Compiled compile_uncached(ByteView source, const modules::CompileOptions& options, bool want_listing, bool cache) const;
    // -> (the exit code of the pass that stopped, or 0; what the passes printed)
    std::pair<std::uint32_t, Bytes> run_passes(const std::filesystem::path& work, const std::string& switches,
                                                              const std::string& tag,
                                                              const std::map<std::string, std::vector<std::int64_t>>& answers,
                                                              emu::dos::MemFS* files, const std::optional<std::size_t>& listing_pass) const;
    Instrument instrument_for(const Pass& pass, const std::map<std::string, std::vector<std::int64_t>>& answers) const;

    Profile profile_;
    std::string variant_, key_;
    std::vector<std::string> variant_hooks_;
    std::vector<Patch> patches_;
    Context context_;
    std::vector<std::pair<std::string, std::shared_ptr<Regex>>> unless_, derive_;   // compiled once: env_unless and derive
    std::shared_ptr<Regex> file_switches_regex_;
    std::vector<std::pair<std::string, std::shared_ptr<Regex>>> answer_regexes_;   // hook name -> the pattern of its header line
    mutable std::atomic<std::uint64_t> local_tags_{0};
    mutable std::mutex stage_mutex_;
};

// ---- the toolchains in use -----------------------------------------------------------------------------------------
class Toolchains {
public:
    using Plugin = std::function<std::shared_ptr<Toolchain>(const Profile&, const std::optional<std::string>& variant, const Context&)>;

    // `runner` and `cache` are not owned and must outlive this; the cache may be null.
    Toolchains(Locations locations, const PassRunner* runner, base::cache::Cache* cache = nullptr);

    const Locations& locations() const { return locations_; }
    // Register a function a profile's hook names in "call".
    void register_hook(std::string name, HookFn fn);
    // Register the factory of a toolchain whose running is code, under the name its profile gives in "plugin".
    void register_plugin(std::string name, Plugin make);
    // Make compiles keep their files on disk instead of in memory (CB_MEMFS=0 does so at the start).
    void set_memfs(bool on) { context_.memfs = on; }

    // The toolchain `name` with `variant`: kept, one for each (name, variant). NotFoundError for a profile that is not there.
    std::shared_ptr<Toolchain> load(const std::string& name, const std::optional<std::string>& variant = std::nullopt);
    std::vector<std::string> profiles() const { return locations_.profile_names(); }

private:
    Locations locations_;
    const PassRunner* runner_;
    base::cache::Cache* cache_;
    std::map<std::string, HookFn> hooks_;
    std::map<std::string, Plugin> plugins_;
    std::atomic<std::uint64_t> tags_{0};
    Context context_;
    std::mutex mutex_;
    std::map<std::pair<std::string, std::string>, std::shared_ptr<Toolchain>> loaded_;   // by (name, variant): looked up
};

// `{name}` filled from `values`, `{{` and `}}` for the braces: the only forms profiles use. A name not in `values` is a
// FormatError.
std::string format_named(std::string_view pattern, const std::map<std::string, std::string>& values);

}  // namespace cb::exact::toolchain
