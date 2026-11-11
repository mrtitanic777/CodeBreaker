// cb/emulate/native_trace.hpp -- the 32-bit run recorder: trace_function's hooks, in C++.
//
// A run of one 32-bit function calls back into the recorder on every instruction, every block and every memory
// write: ping.exe's judge made 2,459 runs and 8.5 million callbacks, most of its time. The recorder (emulate/tracer)
// is the hooks, attached to the Unicorn engine the emulator laid out; the emulator still writes the frame, starts
// each run and puts memory back. This header is the typed face of it:
//
//     TraceCache cache;                                         // what runs of one program share
//     StaticId code = cache.add_code(image_base + text_rva, text_bytes);
//     Options opt;  opt.cache = &cache;  opt.statics = {code};  opt.func = {lo, hi};  ...
//     TraceRun run(engine, opt);                                // the hooks attached
//     int uc_status = run.run(begin, until);                    // emu_start (0: it ran to the end)
//     TraceRecord rec = run.collect();                          // what the hooks recorded
//     run.detach(); run.restore();                              // (a kept machine: hooks off, pages back)
//
// During a run the caller is asked one thing: an import's argument count and what it gives back through a pointer,
// once per import entered (the ImportResolver). The engine must outlive the TraceRun.
//
// One TraceRun belongs to one thread, as the engine it hooks does. A TraceCache is shared by the runs of one program
// and may be used from several threads (its decoded-instruction caches are guarded).
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

struct uc_struct;   // Unicorn's engine (uc_engine)

namespace cb::emulate::tracer {
struct Shadow;      // the path explorer's symbolic shadow (emulate/tracer/shadow)
}

namespace cb::emulate::native_trace {

inline constexpr std::uint64_t kPage = 0x1000;

// ---- what the caller sets up ---------------------------------------------------------------------------------

using StaticId = std::int32_t;

// What runs of one program share: its code ranges as they are in the file, with what was decoded of them, and the
// decoded conditional jumps. Make one per program (module) and keep it for as long as runs of it are made.
class TraceCache {
public:
    TraceCache();
    TraceCache(const TraceCache&) = delete;
    TraceCache& operator=(const TraceCache&) = delete;
    ~TraceCache();

    // A code range as it is in the file: `image` is the bytes of [lo, lo + image.size()). Its decoded instructions
    // are kept for the life of the cache. -> its id, for Options::statics.
    StaticId add_code(std::uint64_t lo, ByteView image);

    struct Impl;
    Impl& impl() { return *impl_; }

private:
    std::unique_ptr<Impl> impl_;
};

struct AddressRange {
    std::uint64_t lo = 0;
    std::uint64_t hi = 0;                    // exclusive
};

// An import's landing place and who it is: a call (or jump) that reaches `address` entered `import`.
struct ImportName {
    std::string dll;                         // "CALLBACK" for a call back into the harness
    std::string name;
};
struct Sentinel {
    std::uint64_t address = 0;
    ImportName import;
};

// A parameter through which an import gives something back: its place, and the number it gives (absent: a handle).
struct OutParam {
    std::int64_t index = 0;
    std::optional<std::int64_t> value;
};
// What the caller knows of an import: how many arguments it takes (absent: not known), and which parameters give
// something back. Asked once per import entered in a run.
struct ImportAnswer {
    std::optional<std::int64_t> argc;
    std::vector<OutParam> outs;
};
using ImportResolver = std::function<ImportAnswer(const ImportName&)>;

// emulator._GOV, carried through the runs of one machine: instructions made so far, and the ceiling.
struct Governor {
    std::int64_t n = 0;
    std::int64_t ceil = 0;
};

struct Options {
    TraceCache* cache = nullptr;             // required
    std::vector<StaticId> statics;           // code ranges, in trace_function's order
    std::vector<std::uint64_t> init_pages;   // page numbers (address >> 12) set before the run
    std::vector<std::uint64_t> mapped;       // page bases already mapped
    std::optional<std::vector<std::uint64_t>> saved;   // page bases the caller already saved (reuse); absent: not reusing
    std::vector<Sentinel> sentinels;         // import landing places (empty when not stubbing)
    std::string module_hash;                 // the traced module's hash: the branch decode cache's key
    Governor* governor = nullptr;            // required

    bool fast = false;                       // told of whole straight runs, not of each instruction
    bool lean = false;                       // writes as (address, size, value, pc); pages as first written
    bool reads = false;                      // record memory reads too
    bool stop_wild = false;                  // stop at the second 00 00 instruction
    bool stub = false;                       // answer imports (sentinels) with a stand-in
    bool unique = false;                     // each stand-in returns its own marker
    bool branches = false;                   // log the function's conditional jumps
    std::optional<AddressRange> quiet;       // writes into it count but are not recorded
    std::optional<AddressRange> pads;        // calls into it are callbacks

    std::int64_t insn_cap = 0;
    std::int64_t stub_ret = 0;               // what a stand-in returns
    std::int64_t stub_cleanup = 0;           // bytes a stand-in pops
    AddressRange func;                       // the function: calls into it, and the branch log's range
    AddressRange image;                      // the module image: a call landing outside it is external
    std::uint64_t lean_low_end = 0;          // lean writes are followed in [0, lean_low_end] ...
    std::uint64_t lean_high_begin = 0;       // ... and [lean_high_begin, 0xFFFFFFFF]

    ImportResolver resolver;                 // empty: no import is answered
    tracer::Shadow* shadow = nullptr;        // the path explorer's, stepped before each instruction
};

// ---- what the run recorded ------------------------------------------------------------------------------------

enum class Access { Read, Write, Fetch, Other };
std::string_view to_text(Access a);                     // "read", "write", "fetch", "?"

// The instruction a conditional jump decided on (emulator._COMPARES).
enum class CompareOp : std::uint8_t { Cmp, Test, Sub, And, Or, Dec, Inc, Add, Xor };
std::string_view to_text(CompareOp op);

// A call answered by a stand-in.
struct StubbedCall {
    std::string api;                                     // "DLL!name" or "CALLBACK!k"
    std::optional<std::int64_t> ret_sentinel;            // the marker it returned (with `unique`)
};
struct CallArgs {
    std::uint64_t stack[6] = {0, 0, 0, 0, 0, 0};         // the words above the return address
    std::uint64_t ecx = 0;
    std::uint64_t edx = 0;
};

struct CallRecord {
    std::uint64_t pc = 0;
    std::optional<std::uint64_t> target;                 // absent: an indirect call
    bool into_func = false;                              // lands in the function itself
    std::optional<std::uint64_t> resolved_target;        // where it went, once the next instruction ran
    bool external = false;                               // landed outside the module image
    std::optional<StubbedCall> stub;
    std::optional<CallArgs> args;
    std::optional<std::int64_t> argc;                    // the import's argument count, when known
    bool indirect() const { return !target; }
};

struct MemRead {
    std::uint64_t pc = 0, addr = 0, size = 0;
    std::optional<std::uint64_t> value;                  // absent: not readable
    std::uint64_t n = 0;                                 // instruction count at the access
};
struct MemWrite {
    std::uint64_t pc = 0, addr = 0, size = 0, value = 0, n = 0, old = 0;
};
struct LeanWrite {
    std::uint64_t addr = 0, size = 0, value = 0, pc = 0;
};
struct LazyMap {                                         // a page mapped on demand
    std::uint64_t pc = 0, addr = 0, page = 0;
    Access access = Access::Other;
    std::uint64_t size = 0;
};
struct BranchCompare {
    CompareOp op = CompareOp::Cmp;
    std::uint64_t size = 0, a = 0, b = 0;
    std::optional<std::uint64_t> at_a, at_b;             // where the operands were loaded from
};
struct BranchEntry {
    std::uint64_t address = 0;
    bool taken = false;
    std::optional<BranchCompare> compare;
};
struct ApiCall {
    std::string api;
    std::optional<std::vector<std::int64_t>> args;       // absent: their number is not known
};

struct TraceRecord {
    std::int64_t insn_count = 0;
    bool truncated = false;
    bool governed = false;
    std::vector<std::uint64_t> path;                     // addresses, in order
    OrderedMap<std::uint64_t, std::string> mnemonics;    // address -> the instruction's text, first seen first
    std::vector<CallRecord> calls;
    std::vector<MemRead> mem_reads;
    std::vector<MemWrite> mem_writes;                    // not lean
    std::vector<LeanWrite> lean_writes;                  // lean
    OrderedMap<std::uint64_t, Bytes> pages0;             // lean: each page as it was when first written
    std::vector<LazyMap> lazy_maps;
    std::optional<std::int64_t> memops;
    std::optional<std::int64_t> writes_seen;
    bool dirty = false;
    std::optional<std::int64_t> stuck;
    std::optional<std::int64_t> bogus_stubs;
    bool ran_into_zeros = false;
    std::vector<std::string> api_deps;                   // imports reached
    std::optional<std::vector<ApiCall>> api_calls;       // every import entered, in order (absent: none was stubbed)
    std::optional<std::int64_t> stubbed_externals;
    std::vector<std::uint64_t> vxd_services;
    std::vector<BranchEntry> branches;
};

// ---- one run ----------------------------------------------------------------------------------------------------

class TraceRun {
public:
    // Attaches the hooks to `engine`. Throws EmulationError when Unicorn refuses a hook.
    TraceRun(uc_struct* engine, Options options);
    TraceRun(const TraceRun&) = delete;
    TraceRun& operator=(const TraceRun&) = delete;
    ~TraceRun();                                         // takes the hooks off, if they are still on

    // A whole run as trace_function makes it: start at `begin` and go to `until`, instruction by instruction (with
    // insn_cap + 1 as the count), or -- with Options::fast -- a block at a time until the recorder meets code it
    // cannot take as a block, then on from there instruction by instruction; then settle(). Returns Unicorn's status.
    int run(std::uint64_t begin, std::uint64_t until);

    // uc_emu_start(begin, until, no time limit, `count` instructions at most: 0 = unlimited). Returns Unicorn's own
    // status (0 = ran to `until`; a fault is its error number: the emulator says what it ended as). Throws what the
    // resolver threw, and EmulationError when a hook could not read the guest.
    int start(std::uint64_t begin, std::uint64_t until, std::size_t count = 0);

    // For a host that starts the engine itself (its wrapper has its own error model): around its emu_start. The first
    // clears what an earlier run carried; the second brings the governor's count back and throws what the resolver threw,
    // or EmulationError when a hook could not read the guest.
    void before_host_start();
    void after_host_start();

    // A fast run gone one instruction at a time (the recorder switched when it met code it could not take as a block).
    bool switched() const;
    void attach_code();

    // A fast run stopped inside a straight run: bring the record up to the instruction it stopped at.
    void settle();

    // What the hooks recorded. Also carries the governor's count back to the Options::governor.
    TraceRecord collect();
    bool truncated() const;
    std::int64_t lean_seen() const;

    void detach();                                       // hooks off (a kept machine)
    void restore();                                      // the pages the hooks saved, put back

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::native_trace
