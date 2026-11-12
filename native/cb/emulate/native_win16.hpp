// cb/emulate/native_win16.hpp -- the 16-bit run recorder: trace_function16's per-instruction and memory hooks, in C++.
//
// A run of one NE function: the emulator lays the module out, enters the function and starts the run (emu_start to the
// sentinel base) and reads the result; this attaches the recorder's 16-bit hooks to the same Unicorn engine and turns
// what its plain code path and memory hooks recorded into a Trace.
//
//     native_win16::Options opt;  opt.eip_id = UC_X86_REG_EIP;  opt.cap = 20000;  opt.governor = &gov;  ...
//     opt.handlers.on_sentinel = [&](std::int64_t addr, std::int64_t n) { /* an import landing: the stand-in */ };
//     Run run(engine, opt);                          // hooks attached
//     ... emu_start(...) ...
//     if (std::exception_ptr e = run.raised()) std::rethrow_exception(e);
//     Trace trace = run.collect();
//
// The heavy, rare events stay with the caller, called from the hooks: the import stand-in (a landing in the sentinel
// range), a call into another of the module's functions (an intercept), interrupts (int 21h as DOS3Call), and -- for a
// recorded-system context -- the module's own code reached through a recorded far pointer. A handler may throw: the
// run stops, the first exception is kept, and `raised()` hands it back for the caller to rethrow after emu_start. The
// branch log and the ports' device model are the caller's own hooks; the ports' clock reads the live instruction
// count (`count()`).
//
// The recorder's limits are its own (what a run may record: 40000 memory records, 512 pages mapped on demand, 3
// million memory operations, 20000 path entries); a run that goes past one is truncated. A page the run touches that
// nothing mapped is mapped on demand, filled with the pattern when `fill` is set (what a read of it then sees).
//
// The engine must outlive the Run. One Run belongs to one thread, as the engine it hooks does.
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "cb/emulate/native_trace.hpp"

namespace cb::emulate::native_win16 {

// The events the hooks hand to the caller: (address or interrupt number, instructions run so far).
using Event = std::function<void(std::int64_t what, std::int64_t n)>;

struct Handlers {
    Event on_sentinel;                                           // execution reached an import sentinel
    Event on_intercept;                                          // reached a callee the caller intercepts
    Event on_interrupt;                                          // `int n`
    std::function<bool(std::int64_t, std::int64_t)> on_own;      // the module's own code in a recorded context: true if redirected
};

struct Options {
    int eip_id = 0;                                              // Unicorn's id of EIP (the record's pc, the limit check)
    bool near_entry = false;                                     // the function is entered by a near call
    bool fill = false;                                           // fill pages mapped on demand with the pattern
    std::int64_t cap = 0;                                        // instructions at most
    std::uint64_t cs_base = 0;
    std::uint64_t near_ret = 0, far_ret = 0;                     // where a return lands: the run has returned
    std::uint64_t sent_lo = 0, sent_hi = 0;                      // the import sentinels' range
    std::uint64_t snap_base = 0;                                 // addresses from here are not segment-limited
    native_trace::Governor* governor = nullptr;                  // required: carried through the runs of one machine
    std::vector<std::uint64_t> intercept;                        // callee-intercept landing addresses
    tracer::Shadow* shadow = nullptr;                            // the path explorer's symbolic shadow, stepped on ordinary instructions
    std::optional<native_trace::AddressRange> own;               // a recorded-system context: the module's own code
    Handlers handlers;
};

// What a run recorded.
struct Trace {
    std::int64_t insn_count = 0;
    bool truncated = false;
    bool returned = false;                                       // reached the return address
    bool limit_fault = false;                                    // an access ran past the end of a segment
    std::vector<std::uint64_t> path;                             // addresses, in order
    std::vector<native_trace::MemRead> mem_reads;
    std::vector<native_trace::MemWrite> mem_writes;
    std::vector<native_trace::LazyMap> lazy_maps;                // pages mapped on demand
    std::optional<std::int64_t> memops;                          // memory operations seen (absent: none)
    std::optional<std::int64_t> writes_seen;
    std::optional<std::int64_t> stuck;                           // accesses that mapped nothing new, in a row
};

class Run {
public:
    // Attaches the hooks to `engine`. Throws EmulationError when Unicorn refuses a hook.
    Run(uc_struct* engine, Options options);
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
    ~Run();

    // A prelude ran: its path, instruction count, reads and writes are dropped (the other counters carry on).
    void reset();
    // Instructions run so far (the ports' clock reads it while the run goes).
    std::int64_t count() const;
    // What a handler threw, or null; the first is kept and cleared by this call. Also brings the governor's count
    // back from the recorder.
    std::exception_ptr raised();
    Trace collect();
    // Hooks off. Throws EmulationError if one cannot be taken off.
    void detach();

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::native_win16
