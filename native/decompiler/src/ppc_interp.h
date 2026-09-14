// XEXMagic - PowerPC interpreter for validating analysis against real behaviour
//
// Runs a function out of the loaded image so a conclusion can be executed
// instead of argued: feed a buffer in, call the parser/decoder/checksum that
// consumes it, read the result back out. Covers the integer/branch/memory
// subset that parsers and codecs use; it is not a console emulator.
//
// External calls are stubbed by address, which is what makes real code
// runnable - most functions bottom out in an allocator or a kernel import.

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct InterpMemWrite {
  uint32_t addr = 0;
  std::vector<uint8_t> data;
};

struct InterpDump {
  uint32_t addr = 0;
  uint32_t len = 0;
};

// One xboxkrnl import trampolined during a run.
struct InterpKernelCall {
  uint32_t thunk = 0;      // address of the import thunk
  uint32_t ordinal = 0;
  std::string name;        // "" when the ordinal is not in the table
  bool implemented = false;
  uint32_t r3 = 0;         // value handed back to the guest
  uint32_t arg[4] = {};    // r3..r6 at the call
};

struct InterpRequest {
  uint32_t entry = 0;
  std::vector<uint32_t> args;                 // r3, r4, ...
  std::vector<InterpMemWrite> writes;         // input data placed before the run
  std::map<uint32_t, uint32_t> stubs;         // call target -> value returned in r3
  // Functions executed natively instead of instruction-by-instruction, keyed by
  // entry address. The XDK memory routines are VMX (lvsl/vperm/stvx), and the
  // VMX ALU is not modelled -- stepping them would quietly move no bytes at
  // all, which is worse than refusing to run. Implementing them directly keeps
  // any harness that touches memset/memcpy honest, and is far faster.
  enum Intrinsic { kMemset = 1, kMemcpy = 2, kMemmove = 3 };
  std::map<uint32_t, int> intrinsics;         // entry address -> Intrinsic
  std::vector<InterpDump> dumps;              // memory to read back afterwards
  uint32_t stack_top = 0x70000000;
  uint64_t max_steps = 20000000;
  bool trace_calls = true;
  uint32_t trace_steps = 0;   // record N instructions...
  // ...starting from this step, not from entry. A fault deep in a run is
  // unreachable otherwise: tracing from step 0 fills the budget on setup code
  // and stops long before the interesting part.
  uint64_t trace_from = 0;
  // Restrict tracing to a PC window. Combined with trace_from this isolates
  // one region of one function out of a multi-thousand-step run.
  uint32_t trace_pc_lo = 0;
  uint32_t trace_pc_hi = 0;   // 0 = no upper bound
  // Watch one GPR and record every write that changes its value. A counter
  // incremented from dozens of scattered sites is impossible to attribute from
  // a call list alone -- this says WHICH site fired, and in what order.
  int watch_gpr = -1;         // -1 = off
  // Watch an address range for writes. Answers "who clobbered this?", which a
  // register watch cannot -- the writer is often a function you never suspected.
  uint32_t watch_mem_lo = 0;
  uint32_t watch_mem_hi = 0;  // lo==hi==0 -> off
};

struct InterpTrace {
  uint32_t pc = 0;
  uint32_t word = 0;
  uint32_t gpr[32] = {};
};

struct InterpDumpResult {
  uint32_t addr = 0;
  std::vector<uint8_t> data;
};

// One observed call, with the argument registers captured AT THE CALL. Knowing
// a function was reached is often not enough -- what it was called WITH is the
// actual answer (an error count, a failing string, a size). r3..r10 is the
// PPC parameter set, and lr says which call site it came from.
struct InterpCall {
  uint32_t target = 0;
  uint32_t lr = 0;      // return address = the call site
  uint32_t arg[8] = {}; // r3..r10
};

// One observed write into the watched address range.
struct InterpMemHit {
  uint32_t pc = 0;
  uint32_t addr = 0;
  uint8_t old_value = 0;
  uint8_t new_value = 0;
  uint64_t step = 0;
};

// One observed change to the watched register: the instruction that did it,
// and the values either side.
struct InterpWatchHit {
  uint32_t pc = 0;
  uint32_t old_value = 0;
  uint32_t new_value = 0;
  uint64_t step = 0;
};

struct InterpResult {
  // Kernel imports trampolined during the run, in order.
  std::vector<InterpKernelCall> kernel_calls;
  bool ok = false;
  std::string error;                          // why it stopped, when not ok
  uint32_t r3 = 0;
  uint64_t steps = 0;
  uint32_t last_pc = 0;
  // Vector (VMX/VMX128) instructions are not modelled. They are skipped so
  // integer code can run, and counted so a result is never trusted blindly:
  // non-zero means the run may be wrong if any of them fed the integer path.
  uint64_t vmx_skipped = 0;
  std::vector<InterpCall> calls;              // call targets, in order
  std::vector<InterpDumpResult> dumps;
  std::vector<InterpTrace> trace;             // when trace_steps > 0
  std::vector<InterpWatchHit> watch;          // when watch_gpr >= 0
  std::vector<InterpMemHit> mem_watch;        // when watch_mem_lo/hi set
};

// Reads the initial byte at a virtual address, or -1 when unmapped. Pages not
// backed by the image start as zero, matching BSS.
using InterpImageReader = int (*)(void* ctx, uint32_t va);

InterpResult PPCInterpRun(const InterpRequest& req, InterpImageReader reader,
                          void* reader_ctx);
