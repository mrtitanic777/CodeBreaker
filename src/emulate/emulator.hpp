// emulate/emulator.hpp -- run one 32-bit function of a PE module in Unicorn and record what it did: the behavioral engine's
// observation primitive.
//
//     Tracer tracer(Environment{.resolve = [&](std::string_view name) { return corpus.resolve(name); }});
//     Trace t = tracer.trace("lz32.dll", FuncSelector{std::uint64_t{0x1234}}, options);
//     t.record.path, t.record.calls, t.record.mem_writes ...  what the hooks saw (cb/emulate/native_trace)
//     t.exit, t.ret, t.esp_delta, t.saved_regs                  how it ended and what it left
//     Run run = record(std::make_shared<const Trace>(std::move(t)));   // the one run record the judge compares
//
// What a run is: the module's code sections are laid out at their real addresses in one buffer (the gaps zero-filled), with a scratch
// window for pointer arguments and a stack of its own; stdcall arguments are pushed above a return-address sentinel and the run goes
// from the function's first instruction to that sentinel (or to the instruction cap, or to a fault). Unlike a plain run, an access to
// memory nobody mapped maps a zeroed page and goes on, the access recorded as evidence. With imports stubbed, a landing address is
// planted in every import slot so a call through the table reaches a named import, answered with a stand-in.
//
// No fabrication: everything in a Trace was observed during the run. A fault, a truncation or a non-return exit is recorded as itself.
//
// A Tracer keeps what is costly to make between runs: the carved modules (carving a PE is the dominant per-trace cost), the decode
// caches of the recorder, and -- for runs that ask to `reuse` -- machines with the module laid out, put back exactly as they were after
// each run. Its governor is a deterministic ceiling on the total instructions run for one function (the caller resets it); one Tracer is
// for one thread's sequence of runs, and its caches are guarded so that other threads may ask it for modules and run beside it on their own
// Tracers.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "core16_types.hpp"
#include "emulate/native_shadow.hpp"
#include "emulate/native_trace.hpp"
#include "emulate/run.hpp"
#include "formats/pe_functions.hpp"
#include "symbols/prototypes.hpp"

namespace cb::knowledge {
class Knowledge;
}

namespace cb::emulate::emulator {

// ---- the machine a run starts on -------------------------------------------------------------------------------------------------
// (the entry state difftest proved correct, so traces are comparable to the oracle it trusts)
inline constexpr std::uint32_t kScratchBase = 0x200000, kScratchSize = 0x10000;   // where pointer arguments are aimed
inline constexpr std::uint32_t kStackBase = 0x300000, kStackSize = 0x40000;       // the stack has a region of its own
inline constexpr std::uint32_t kStackTop = kStackBase + kStackSize - 0x1000;      // esp starts below this
inline constexpr std::uint32_t kReturnAddress = 0xDEAD0000;                       // the run halts when it gets here
inline constexpr std::uint32_t kPage = 0x1000;
// EBX, ESI, EDI, EBP at entry: a function that hands one back changed is caught though its caller here holds nothing of value there.
inline constexpr std::array<std::uint32_t, 4> kSavedSentinels = {0xB0B0B0B0, 0x51515151, 0xD1D1D1D1, 0xBDBDBDBD};
// One landing address per import slot, kSentinelStride apart from kSentinelBase.
inline constexpr std::uint32_t kSentinelBase = 0xE1000000, kSentinelStride = 0x10;

// Recording caps: a trace is evidence, not a full replay log, so it is bounded and says that it was.
inline constexpr std::int64_t kMaxInsns = 20000;   // instructions per run
inline constexpr std::int64_t kMaxMem = 40000;     // memory operations kept
inline constexpr std::size_t kMaxMachines = 6;     // laid-out machines kept between reuse runs

// ---- what the Tracer needs from outside ------------------------------------------------------------------------------------------

// How many argument dwords an import's stand-in pops (absent: not known). Asked once per import entered in a run. A call back into the
// harness ("CALLBACK") takes none, and is not asked.
using ImportArgc = std::function<std::optional<std::int64_t>(const native_trace::ImportName&)>;

struct Environment {
    // The file a module name (or a path) names: the corpus's retail copy. Throws NotFoundError when there is none. Required.
    std::function<std::filesystem::path(std::string_view)> resolve;
    // How the headers declare a Windows function, by its own name without its library; none when the headers have none. Optional: without
    // it no import gives anything back through a pointer.
    std::function<std::optional<symbols::prototypes::Prototype>(std::string_view)> prototype;
    // A new 32-bit x86 engine. Optional: the default is cb/core's Unicorn. A host whose recorder was pointed at another copy of the library
    // (emulate/tracer's use_libraries) makes its engines from that copy.
    std::function<std::unique_ptr<core16::Uc>()> open_engine;
};

// ---- the module as loaded ----------------------------------------------------------------------------------------------------------

struct CodeRange {
    std::uint32_t lo = 0, hi = 0;   // [lo, hi) virtual addresses
};

// A PE module's executable sections, carved once. Offsets are from `va0`, the lowest section's address.
struct LoadedModule {
    std::filesystem::path path;
    std::uint32_t va0 = 0;
    Bytes code;                                          // every executable section laid out at its address from va0 (gaps zeros)
    std::string section;                                 // the sections' names joined with '+'
    std::vector<CodeRange> ranges;                       // the sections as loaded
    std::map<std::uint32_t, std::uint32_t> functions;    // recovered function: offset -> end offset (ordered: the next start is asked for)
    std::string sha256;                                  // of the file, lowercase hex
    std::map<std::uint32_t, formats::pe_functions::ImportName> imports;   // import address table slot (a virtual address) -> (library, name)
};

// Which function: the first one recovered, a section offset or a virtual address (a value at or above the module's va0 is an address).
using FuncSelector = std::variant<std::monostate, std::uint64_t, std::string>;   // (a text is "0x..." or decimal)

struct ResolvedFunction {
    std::shared_ptr<const LoadedModule> module;
    std::uint32_t off = 0, end = 0;                      // offsets from module->va0
    std::uint32_t va() const { return module->va0 + off; }
    std::uint32_t end_va() const { return module->va0 + end; }
};

// ---- a run -----------------------------------------------------------------------------------------------------------------------

struct MemoryInit {   // memory written before the run (pages are mapped as needed): a controlled buffer a pointer argument points at
    std::uint32_t address = 0;
    Bytes data;
};

struct TraceOptions {
    std::optional<std::vector<std::uint32_t>> args;      // the stdcall dwords; absent: `arity` zeros
    std::optional<int> arity;                            // absent: 4
    std::int64_t insn_cap = kMaxInsns;
    std::vector<MemoryInit> mem_init;
    // A call that leaves the module's code (an import) returns to the caller with EAX = stub_ret and pops stub_cleanup extra bytes; the
    // call is recorded as a dependency. Absent: the Tracer's default.
    std::optional<bool> stub_externals;
    std::int64_t stub_ret = 0;
    std::int64_t stub_cleanup = 0;
    ImportArgc argc;                                     // empty: the Tracer's default
    bool stub_ret_unique = false;                        // each stand-in returns its own marker
    // Another module whose code is mapped alongside at its own addresses: a rebuilt body refers to the ORIGINAL's code and data by literal
    // address (a function pointer, a thunk target), and without the original there those addresses read as zeros in the rebuilt run only.
    std::optional<std::string> overlay;
    std::optional<std::array<std::uint32_t, 4>> sentinels;   // EBX ESI EDI EBP at entry; absent: kSavedSentinels
    bool reads = true;                                   // record memory reads
    bool stop_wild = false;                              // stop at the second 00 00 instruction
    bool quiet_stack = false;                            // writes into the stack count but are not recorded
    bool reuse = false;                                  // keep the laid-out machine between runs
    bool fast = false;                                   // told of whole straight runs, not of each instruction (not with a shadow or a governor ceiling)
    bool lean = false;                                   // writes as (address, size, value, pc) with the pages as first written
    std::optional<std::uint32_t> ecx, edx;               // an argument in ECX / EDX: a member function's object
    std::optional<native_trace::AddressRange> pads;      // padding bytes: a call into it is a callback
    bool branches = false;                               // log each conditional jump of the function
    std::vector<native_trace::Sentinel> stub_at;         // functions entered as if they were imports
    native_shadow::Shadow* shadow = nullptr;             // a Flat32 shadow kept up beside the run (one instruction at a time)
};

// How a run ended.
enum class Exit : std::uint8_t {
    Returned,    // reached the return sentinel
    Truncated,   // the instruction cap or the governor stopped it
    Faulted,     // the engine stopped it, or it stopped somewhere that is not the sentinel
};

struct Trace {
    std::string module;                                  // the label (a file name when the module was given as a path)
    std::filesystem::path module_path;                   // the file actually traced
    std::string target_sha256;
    std::string section;
    std::uint32_t func_off = 0, func_va = 0, func_end_va = 0;
    std::vector<std::uint32_t> args;                     // the dwords pushed
    std::uint32_t entry_esp = 0;
    Exit exit = Exit::Returned;
    std::string fault;                                   // Faulted: why (the engine's text, or where it stopped)
    std::optional<std::uint32_t> ret;                    // EAX
    std::optional<std::array<std::uint32_t, 4>> saved_regs;   // EBX ESI EDI EBP at the end
    std::optional<std::uint32_t> esp_delta;              // 4 = cdecl, 4 + N = stdcall with N bytes
    std::optional<double> st0;                           // an x87 return
    native_trace::TraceRecord record;                    // what the hooks recorded

    // "ret", "truncated" or "exception: <fault>": the words the run record carries.
    std::string exit_text() const;
};

// The one run record: EAX (and x87 ST0) the results, the imports and callbacks entered with their arguments, EBX ESI EDI EBP against the
// sentinels they were given (kSavedSentinels unless `sentinels` says), every instruction run (first seen first). A write made on the way out
// of zeros is the harness's. A lean run's writes carry what their bytes held before the run (its pages as first written). The Trace is the
// run's `raw` (a std::shared_ptr<const Trace>).
Run record(const std::shared_ptr<const Trace>& trace, const std::optional<std::array<std::uint32_t, 4>>& sentinels = std::nullopt);

// The parameters through which an API gives something back, by its declaration: a handle (no value: the stand-in hands out one by the
// parameter's place) or a number (0).
std::vector<native_trace::OutParam> out_params(const symbols::prototypes::Prototype& prototype);

// Argument count from the knowledge base's win32 model (documented counts for common APIs; an A/W variant shares the count of its base),
// or none.
std::optional<std::int64_t> builtin_argc(knowledge::Knowledge& knowledge, std::string_view name);

class Tracer {
public:
    // UsageError when `environment.resolve` is missing.
    explicit Tracer(Environment environment);
    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;
    ~Tracer();

    // The ceiling on instructions run for one function: the caller sets `ceil` (0: none) and resets `n` for each function; a run stops once it
    // is past it (the cascade runs dry fast and the function is honestly skipped).
    native_trace::Governor& governor();

    // The default for runs that do not say: whether to stub imports, and how many arguments each takes.
    void set_default_stub(bool enabled, ImportArgc argc = {});

    // The module, carved once. FormatError for a file that is not a PE32 with code, NotFoundError from the environment's resolver. A name
    // with a "#member" suffix (a module inside a bundle) is resolved by the name before it.
    std::shared_ptr<const LoadedModule> load(std::string_view module);

    // The function the selector names. A virtual address or offset inside a recovered function snaps to its start; one outside any (an export
    // the recovery did not reach) is accepted as a start that ends at the next recovered function's, else at the section's end. UsageError for a
    // text that is no number, FormatError for a module with no function of 8 bytes or more or an address outside the code.
    ResolvedFunction resolve(std::string_view module, const FuncSelector& function = {});

    // Run one function with full hooks. A fault of the guest is the Trace's `exit`, not an exception; an exception a native hook raised, a
    // module that cannot be loaded or a function that cannot be found is one.
    Trace trace(std::string_view module, const FuncSelector& function = {}, const TraceOptions& options = {});

    // trace() as the one run record (the platform's runner).
    Run recorded(std::string_view module, const FuncSelector& function = {}, const TraceOptions& options = {});

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::emulator
