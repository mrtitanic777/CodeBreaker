// decompiler -- CodeBreaker's decompiler: machine code in, C out, for any processor it has a
// language description for and any file format someone can describe to it.
//
// It does not read executables. Whoever calls it has already taken the file apart and says, in a
// plain description, where the bytes go in memory and what is known for certain about the code:
//
//   arch   <language id>                      e.g. x86:LE:32:default:windows
//                                                  PowerPC:BE:64:Xenon:homed
//   bytes  <hexVA> <file> <hexOffset> <hexSize>    these bytes of that file are at this address
//   F      <hexVA> <name> <pop>               a function; pop = bytes of arguments it removes on
//                                             return (x86 `ret N`), -1 when unknown/not applicable
//   I      <hexVA> <name> <pop>               an import slot (a pointer the loader fills in) and
//                                             the function it will hold
//   X      <hexVA> <name> <pop> [<model>]     a function outside the image, at an address the
//                                             describer chose (16-bit imports are patched in as
//                                             far pointers to such addresses)
//   reg    <register> <hexValue>              a register's value everywhere, e.g. DS = the data
//                                             segment of a 16-bit module
//
// F may carry a fifth field, the calling-convention model by name (e.g. __stdcall16far), or
// the word `inline`: a compiler helper that is part of every caller's prologue or epilogue
// (PowerPC register save/restore runs), whose instructions are read as the caller's own.
//
// (tab separated). The analysis itself -- exact instruction semantics, data flow, stack and type
// recovery, control-flow structuring, C printing -- is the Ghidra decompiler core in core/,
// linked in (Apache-2.0; see core/LICENSE and core/NOTICE). What is added here is that nothing
// is guessed that the binary states: function starts and names, calling convention and argument
// count from each function's own return, and imports resolved to named functions.
//
//   decompiler <description> [--at <hexVA>...]      every function listed, or only those
//
// CB_STRUCTS=1 recovers structures from field accesses (the section "Structures" below).
// CB_THREADS=N analyses N functions at once (each thread its own analysis of the whole
// description); the output is the same, in the same order.
//
// For processors other than 32-bit x86, each pointer-typed local or parameter is reported as
// `/*@P <name> <bytes> */` (16-bit code has near and far pointers; the C text does not say which).
// Each function is preceded by `/*@@ <name> <hexVA> */`; each global it touches is reported as
// `/*@G <hexVA> <bytes> <bytes pointed at, or 0> */` (the printed name of an unnamed global
// carries neither its size nor, for a pointer, the size of what it points at).
// CB_LANGUAGES names the folder of language files (*.sla, *.ldefs, *.pspec, *.cspec).
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "libdecomp.hh"
#include "sleigh_arch.hh"
#include "loadimage.hh"
#include "printc.hh"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <new>
#include <thread>
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <io.h>
#pragma comment(lib, "psapi.lib")
#undef LoadImage                // (a Windows macro: the decompiler core has a class by that name)
#else
#include <unistd.h>
#endif

// ---- The governor: one function may not take the machine down -------------------------------
// A wrong function start (the middle of an instruction, data that decodes) sends the analysis
// through "flow" that never ends, and its data structures grow with the square of what it saw:
// once, one run grew to 27 GB and the machine went down with it. So the program keeps its own
// account of the memory it holds (every new and delete), and a function that takes more than its
// budget (CB_MEMORY_MB, 1536 by default) or runs longer than CB_FUNCTION_SECONDS (120) is ended
// where it is: its failure is written after its marker, and the program exits. Whoever runs it
// starts it again from the next function (codebreaker/decompile/decompiler.py: _run_resuming).
// And the operating system holds it to a hard ceiling besides (a Windows job), for what the
// account does not see.
//
// With CB_THREADS=N (N > 1) N functions are analysed at once, each thread with its own analysis
// (main, below). CB_MEMORY_MB is then the whole process's: a function over its share stops
// cleanly at the analysis's next check (the one holding the most, once the process is past three
// quarters of the budget), and so does one over its time; only what the checks cannot reach
// ends the process -- with nothing written to the output, so the caller does that function
// again as it would any cut-off one.
namespace governor {
std::atomic<long long> held{0};          // bytes held now
long long budget = 0;                    // 0: not yet set up (before main) -- no limit
std::atomic<bool> tripping{false};
int threads = 1;                         // CB_THREADS

// What each analysing thread is doing, for the watchdog. (The analysis's own record of where it
// is -- cb_action, cb_rule, cb_steps -- is per thread: core/cpp/action.hh. The watchdog reads it
// only to say where a function was when it was stopped.)
struct Slot {
  const char* const* action = nullptr;
  const char* const* rule = nullptr;
  const unsigned long long* steps = nullptr;
  std::atomic<long long> held{0};        // what this thread's allocations hold
  std::atomic<long long> base_held{0};   // ...when its function began
  std::atomic<long long> started_ms{0};  // when its function began (0: none)
  std::atomic<long long> expired{0};     // past the time limit: the start (started_ms) of the function
                                         // to stop at its next check -- that function's, not the next one's
};
const int kMaxSlots = 64;
Slot slots[kMaxSlots];
std::atomic<int> nslots{0};
thread_local Slot* my_slot = nullptr;

void fail_and_exit(const char* why, int code, const Slot* s) {
  if (tripping.exchange(true)) return;
  // (no allocation here: the stream library would come back into operator new)
  const char* act = s && s->action ? *s->action : "";
  const char* rule = s && s->rule ? *s->rule : "";
  char buf[400];
  int n;
  std::fflush(stdout);
  if (threads == 1) {
    n = std::snprintf(buf, sizeof(buf), "/* decompile failed: %s, in %s / %s */\n\n", why, act, rule);
#ifdef _WIN32
    _write(1, buf, n);
#else
    ::write(1, buf, n);
#endif
  } else {                               // (the output holds only whole functions)
    n = std::snprintf(buf, sizeof(buf), "[decompiler stopped: %s, in %s / %s]\n", why, act, rule);
#ifdef _WIN32
    _write(2, buf, n);
#else
    ::write(2, buf, n);
#endif
  }
  std::_Exit(code);
}
}  // namespace governor

void* operator new(std::size_t n) {
  if (governor::budget && governor::held.load(std::memory_order_relaxed) + (long long)n > governor::budget)
    governor::fail_and_exit("over the memory budget (CB_MEMORY_MB)", 3, governor::my_slot);
  void* p = std::malloc(n + 16);
  if (!p) throw std::bad_alloc();
  *(std::size_t*)p = n;
  governor::held.fetch_add((long long)n, std::memory_order_relaxed);
  if (governor::my_slot) governor::my_slot->held.fetch_add((long long)n, std::memory_order_relaxed);
  return (char*)p + 16;
}
void operator delete(void* p) noexcept {
  if (!p) return;
  char* q = (char*)p - 16;
  const long long n = (long long)*(std::size_t*)q;
  governor::held.fetch_sub(n, std::memory_order_relaxed);
  if (governor::my_slot) governor::my_slot->held.fetch_sub(n, std::memory_order_relaxed);
  std::free(q);
}
void operator delete(void* p, std::size_t) noexcept { operator delete(p); }
void* operator new[](std::size_t n) { return operator new(n); }
void operator delete[](void* p) noexcept { operator delete(p); }
void operator delete[](void* p, std::size_t) noexcept { operator delete(p); }

namespace governor {
long long now_ms();

long long process_bytes() {
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX pm = {};
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pm, sizeof(pm)))
    return (long long)pm.PrivateUsage;
#endif
  return held.load();
}

// Bounded analysis (docs/CORE_ROADMAP.md, phase 1): the analysis asks, at the places it can stop
// cleanly, whether it must. A function past three quarters of the memory budget -- what the
// process held when it began, and then some -- or past CB_MAX_STEPS rule tests fails there, like
// any analysis error: its memory freed, and the next function goes on in the same process.
long long base_bytes = 0;                // what the process held when the function began (one thread)
long long soft_bytes = 0;
unsigned long long max_steps = 0;
thread_local char soft_why[200];

const char* over_budget() {
  if (max_steps && ghidra::cb_steps > max_steps) {
    std::snprintf(soft_why, sizeof(soft_why), "over the analysis budget (%llu rule tests) in %s / %s",
                  ghidra::cb_steps, ghidra::cb_action, ghidra::cb_rule);
    return soft_why;
  }
  if (threads == 1) {
    long long now = process_bytes();
    if (soft_bytes && now - base_bytes > soft_bytes) {
      std::snprintf(soft_why, sizeof(soft_why), "over the analysis budget (%lld MB) in %s / %s",
                    (now - base_bytes) >> 20, ghidra::cb_action, ghidra::cb_rule);
      return soft_why;
    }
    return nullptr;
  }
  Slot* s = my_slot;
  if (s == nullptr) return nullptr;
  if (s->expired.load() != 0 && s->expired.load() == s->started_ms.load()) {
    std::snprintf(soft_why, sizeof(soft_why), "over the time limit (CB_FUNCTION_SECONDS) in %s / %s",
                  ghidra::cb_action, ghidra::cb_rule);
    return soft_why;
  }
  long long own = s->held.load() - s->base_held.load();
  bool stop = soft_bytes && own > soft_bytes;
  if (!stop && soft_bytes && process_bytes() > soft_bytes) {
    // the process is past three quarters of its budget: the function holding the most stops
    stop = true;
    for (int i = 0, n = nslots.load(); i < n; ++i)
      if (&slots[i] != s && slots[i].started_ms.load() && slots[i].held.load() - slots[i].base_held.load() > own)
        stop = false;
  }
  if (stop) {
    std::snprintf(soft_why, sizeof(soft_why), "over the analysis budget (%lld MB) in %s / %s",
                  own >> 20, ghidra::cb_action, ghidra::cb_rule);
    return soft_why;
  }
  return nullptr;
}

// This thread analyses functions: the watchdog is to look at it
void claim_slot() {
  int i = nslots.fetch_add(1);
  if (i >= kMaxSlots) { nslots = kMaxSlots; return; }
  slots[i].action = &ghidra::cb_action;
  slots[i].rule = &ghidra::cb_rule;
  slots[i].steps = &ghidra::cb_steps;
  my_slot = &slots[i];
}

void begin_function() {
  ghidra::cb_steps = 0;
  if (threads == 1) base_bytes = process_bytes();
  if (Slot* s = my_slot) {
    s->base_held = s->held.load();
    s->started_ms = now_ms();
  }
}

void end_function() {
  if (Slot* s = my_slot) s->started_ms = 0;
}

long long now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now().time_since_epoch()).count();
}

void set_up() {
  const char* th = std::getenv("CB_THREADS");
  threads = th ? std::atoi(th) : 1;
  if (threads < 1) threads = 1;
  if (threads > kMaxSlots) threads = kMaxSlots;
  const char* mb = std::getenv("CB_MEMORY_MB");
  long long limit = (mb ? std::atoll(mb) : 1536) * 1024 * 1024;
  budget = limit;
#ifdef _WIN32
  // the operating system's ceiling: the budget and room for what is not counted (the
  // runtime's own, the stream buffers)
  HANDLE job = CreateJobObjectA(nullptr, nullptr);
  if (job) {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION li = {};
    li.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;
    li.ProcessMemoryLimit = (SIZE_T)(limit + 512LL * 1024 * 1024);
    if (SetInformationJobObject(job, JobObjectExtendedLimitInformation, &li, sizeof(li)))
      AssignProcessToJobObject(job, GetCurrentProcess());
  }
#endif
  soft_bytes = limit / 4 * 3;
  const char* st = std::getenv("CB_MAX_STEPS");
  max_steps = st ? std::strtoull(st, nullptr, 10) : 400000000ULL;
  ghidra::cb_over_budget = over_budget;
  const char* sec = std::getenv("CB_FUNCTION_SECONDS");
  const long long max_ms = (sec ? std::atoll(sec) : 120) * 1000;
  // (the count above is what was asked for; what the process really holds is more -- the
  // allocator's own overhead, memory freed but kept by the heap: one function reached the
  // operating system's ceiling with its count still under budget. So what Windows says the
  // process holds is looked at too, ten times a second.)
  // CB_TRACE=1: what the analysis is doing, five times a second, on stderr -- where a function
  // that runs long or grows spends it (the action, the rule last applied, the steps, the memory)
  const bool trace = std::getenv("CB_TRACE") != nullptr;
  std::thread([max_ms, limit, trace] {
    int tick = 0;
    for (;;) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      const bool show = trace && (++tick & 1) == 0;
      for (int i = 0, n = nslots.load(); i < n; ++i) {
        Slot& sl = slots[i];
        long long s = sl.started_ms.load();
        if (!s) continue;
        if (show)
          std::fprintf(stderr, "[trace %d] %6lld ms  %-24s %-24s steps %llu  %lld MB\n", i, now_ms() - s,
                       *sl.action, *sl.rule, *sl.steps, process_bytes() >> 20);
        if (now_ms() - s > max_ms) {
          // one thread: the process ends here, as it always has. Several: the function stops at
          // its next check; one the checks do not reach (twice the limit) ends the process
          if (threads == 1) fail_and_exit("over the time limit (CB_FUNCTION_SECONDS)", 4, &sl);
          sl.expired = s;
          if (now_ms() - s > 2 * max_ms) fail_and_exit("over twice the time limit (CB_FUNCTION_SECONDS)", 4, &sl);
        }
      }
#ifdef _WIN32
      PROCESS_MEMORY_COUNTERS_EX pm = {};
      if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pm, sizeof(pm)) &&
          (long long)pm.PrivateUsage > limit)
        fail_and_exit("over the memory budget (CB_MEMORY_MB)", 3, nullptr);
#endif
    }
  }).detach();
}
}  // namespace governor

using namespace ghidra;

namespace {

struct Fact { uint64_t va; std::string name; int pop; std::string model; int result = -1; std::vector<std::string> args; };   // result: bytes it returns in AX (DX:AX), when known
struct Span { uint64_t va; std::vector<uint8_t> data; };
// A call through a pointer (a driver's dispatch table, a callback): what it is passed, as the
// code pushed it -- the callee is not known, so the analysis cannot tell what it takes.
struct CallSite { uint64_t site; uint64_t func; int pop; std::string model; };
// A switch jump through a table, and the places its table sends to (`J <site> <t>,<t>,...`):
// the describer reads the table as the code bounds it; the analysis would take the range it can
// prove for the index, which can be wrong (USER 1:03A5: odd offsets of an even index, garbage
// words read as places, half a segment followed).
struct JumpOverride { uint64_t site; std::vector<uint64_t> targets; };

// Imports live at made-up addresses outside the image: an import slot reads as the address of
// "its" function, so a call through the slot resolves to a named function with a real prototype.
const uint64_t kExternBase = 0xE0000000u;
// The software interrupts' made-up functions: vector n at F000:n*16 (languages/x86-16.cspec).
// What one hands back besides AX is named int_<register> (core/cpp/database.cc, buildDefaultName).
const uint64_t kInterruptBase = 0xF0000000u;

class Image : public LoadImage {
  std::shared_ptr<const std::vector<Span>> spans_;  // (read only: shared by every thread's analysis)
  std::map<uint64_t, uint64_t> slot_value_;        // import slot VA -> made-up function address
  std::string arch_;
  bool big_ = false;
  int slot_size_ = 4;
 public:
  Image(const std::string& arch, std::shared_ptr<const std::vector<Span>> spans)
      : LoadImage("codebreaker"), spans_(std::move(spans)), arch_(arch) {
    big_ = arch.find(":BE:") != std::string::npos;
  }
  void setSlot(uint64_t slot_va, uint64_t value) { slot_value_[slot_va] = value; }
  const std::map<uint64_t, uint64_t>& slots() const { return slot_value_; }
  int slotSize() const { return slot_size_; }
  bool holds(uint64_t va) const {
    for (const auto& s : *spans_)
      if (va >= s.va && va - s.va < s.data.size()) return true;
    return false;
  }
  void loadFill(uint1* ptr, int4 size, const Address& addr) override {
    uint64_t a = addr.getOffset();
    for (int4 i = 0; i < size; ++i) {
      uint64_t va = a + (uint64_t)i;
      auto it = slot_value_.upper_bound(va);
      if (it != slot_value_.begin()) {
        --it;
        uint64_t k = va - it->first;
        if (k < (uint64_t)slot_size_) {
          int shift = 8 * (int)(big_ ? (slot_size_ - 1 - k) : k);
          ptr[i] = (uint1)(it->second >> shift);
          continue;
        }
      }
      ptr[i] = 0;
      for (const auto& s : *spans_)
        if (va >= s.va && va - s.va < s.data.size()) { ptr[i] = s.data[(size_t)(va - s.va)]; break; }
    }
  }
  string getArchType(void) const override { return arch_; }
  void adjustVma(long) override {}
};

class Machine : public SleighArchitecture {
  Image* image_;
  void buildLoader(DocumentStorage&) override {
    collectSpecFiles(*errorstream);
    loader = image_;
  }
  void resolveArchitecture(void) override {
    archid = getTarget();
    SleighArchitecture::resolveArchitecture();
  }
 public:
  Machine(const string& fname, const string& target, ostream* estream, Image* image)
      : SleighArchitecture(fname, target, estream), image_(image) {}
};

// x86, 32-bit: a function that returns with `ret N` (N > 0) removes its own N/4 arguments
// (stdcall); with a plain `ret` the caller cleans up (cdecl) and the argument count is left to
// the analysis.
//
// `self`: the function takes its first argument in ECX (a C++ member function's object) and
// removes the rest itself -- said by the describer, which saw ECX read before it is written.
void applyX86Proto(Machine& m, Funcdata* fd, const std::string& name, int pop, bool self = false,
                   bool second = false) {
  if (pop < 0) return;
  FuncProto& fp = fd->getFuncProto();
  PrototypePieces pieces;
  // (`second`: and its second in EDX -- the two-register convention)
  pieces.model = m.getModel(second ? "__fastcall" : self ? "__thiscall" : pop == 0 ? "__cdecl" : "__stdcall");
  pieces.name = name;
  // Every x86 function leaves a value in EAX and callers are free to use it; declaring the
  // result keeps it. Left to itself the analysis calls a function `void` whenever it does not
  // see the value used, and the function then returns nothing.
  pieces.outtype = m.types->getBase(4, TYPE_INT);
  pieces.firstVarArgSlot = -1;
  if (self) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("self");
  }
  if (second) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("second");
  }
  for (int i = 0; i < pop / 4; ++i) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("a" + std::to_string(i + 1));
  }
  fp.setPieces(pieces);
  // The stdcall model says only "the callee removes some bytes"; left at that, the analysis has
  // to solve for how many, and when it solves wrong every stack reference after the call is off
  // by four. We know the number: the return address plus the function's own `ret N`.
  fp.setExtraPop(4 + pop);
  // With `ret N` the argument count is known; with a plain `ret` it is left to the analysis.
  // The argument TYPES are placeholders either way (four bytes each).
  fp.setInputLock(pop > 0 || self);
  fp.setModelLock(true);
  fp.setOutputLock(true);
}

bool readFile(const std::string& path, uint64_t off, uint64_t size, std::vector<uint8_t>& out) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return false;
  in.seekg((std::streamoff)off);
  out.resize((size_t)size);
  in.read((char*)out.data(), (std::streamsize)size);
  out.resize((size_t)in.gcount());
  return true;
}

std::vector<std::string> split(const std::string& line) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : line) {
    if (c == '\t') { out.push_back(cur); cur.clear(); }
    else if (c != '\r') cur += c;
  }
  out.push_back(cur);
  return out;
}

// What the description says, read once; every analysis (one per thread) is built from it
struct Description {
  std::string path, arch;
  std::vector<Fact> funcs, imports, externs;
  std::vector<std::pair<std::string, uint64_t>> regs;
  std::shared_ptr<const std::vector<Span>> spans;
  std::vector<CallSite> calls;
  std::vector<JumpOverride> jumps;
  std::set<uint64_t> only;
  bool x86_32 = false;
};

// One analysis: a Machine over the description, every function, import and interrupt registered,
// and the functions to write, in the description's order. Its analysis state is its own, so
// several can run at once, one per thread.
struct Analysis {
  std::ostringstream errs;
  DocumentStorage store;
  Image* image = nullptr;                          // (the machine owns it)
  std::unique_ptr<Machine> m;
  std::vector<std::pair<Funcdata*, Fact>> todo;
  std::set<const Symbol*> registered;              // the global symbols the description made
};

void setUp(Analysis& a, const Description& d) {
  const std::vector<Fact>& imports = d.imports;
  const bool x86_32 = d.x86_32;
  a.image = new Image(d.arch, d.spans);
  for (size_t i = 0; i < imports.size(); ++i)
    a.image->setSlot(imports[i].va, kExternBase + (uint64_t)i * 0x10);
  a.m.reset(new Machine(d.path, d.arch, &a.errs, a.image));
  Machine& m = *a.m;
  m.init(a.store);
  m.readonlypropagate = true;
  // Nothing stops an escaped local's aliasing: a buffer whose address is passed on is as long as
  // the callee writes, not as the array the analysis guessed (KRNL386 fills a struct and reads
  // a field past what it sized as the array -- read as a variable never set)
  m.alias_block_level = 0;
  m.max_instructions = 20000;                    // a wrong function start must fail quickly, not walk memory
  if (const char* mx = std::getenv("CB_MAX_INSTRUCTIONS"))  // (a large function is tried again with more)
    m.max_instructions = (uint4)std::atoi(mx);
  AddrSpace* code = m.getDefaultCodeSpace();
  Scope* glb = m.symboltab->getGlobalScope();
  // An import slot never changes while the module runs: reading one gives a constant, so a
  // call through it -- directly or after the slot was copied to a register -- is a call to
  // that import.
  for (const auto& kv : a.image->slots())
    m.symboltab->setPropertyRange(Varnode::readonly, Range(code, kv.first, kv.first + a.image->slotSize() - 1));

  for (size_t i = 0; i < imports.size(); ++i) {
    Address ea(code, (uintb)kExternBase + (uintb)i * 0x10);
    Funcdata* fd = glb->addFunction(ea, imports[i].name)->getFunction();
    if (x86_32) applyX86Proto(m, fd, imports[i].name, imports[i].pop);
  }
  for (const auto& r : d.regs) {                   // a register known to hold one value throughout
    const VarnodeData& vd = m.translate->getRegister(r.first);
    TrackedSet& ts = m.context->getTrackedDefault();
    ts.emplace_back();
    ts.back().loc = vd;
    ts.back().val = (uintb)r.second;
    if (r.first == "DS") {                         // (the data segment: a constant segment that is
      PrintC::dataSegmentKnown = true;             // another one is printed whole -- PrintC::nearSegment)
      PrintC::dataSegment = (uintb)r.second;
    }
  }
  auto applyModel = [&](Funcdata* fd, const Fact& f) {
    if (f.model == "inline") { fd->getFuncProto().setInline(true); return; }
    if (x86_32) { applyX86Proto(m, fd, f.name, f.pop, f.model == "__thiscall" || f.model == "__fastcall",
                                f.model == "__fastcall"); return; }
    if (!f.model.empty() && m.hasModel(f.model)) {
      FuncProto& fp = fd->getFuncProto();
      const int ret_addr = f.model.find("far") != std::string::npos ? 4 : 2;
      // (and a function the caller cleans up after, with its arguments' sizes given: those --
      // left to the analysis, each caller guessed for itself, SETUPX 24:0F1E's size lost)
      std::vector<std::string> sizes(f.pop > 0 ? f.pop / 2 : 0, "2");
      if (f.pop == 0) sizes = f.args;
      if ((f.pop > 0 && f.model.find("stdcall") != std::string::npos) ||
          (f.pop == 0 && !f.args.empty() && f.model.find("cdecl") != std::string::npos)) {
        // 16-bit, the function removes its own arguments: `ret N` / `retf N` states how many
        // bytes there are, so the arguments are N/2 words. (Their types are placeholders; a
        // far pointer is two of them.) Without this the analysis decides per function how
        // many it sees used, and a function that ignores an argument is rebuilt removing
        // fewer bytes than its callers pushed.
        PrototypePieces pieces;
        pieces.model = m.getModel(f.model);
        pieces.name = f.name;
        pieces.outtype = m.types->getBase(2, TYPE_INT);
        pieces.firstVarArgSlot = -1;
        for (size_t i = 0; i < sizes.size(); ++i) {
          // (`4p`: a far pointer -- as a 4-byte number it was dereferenced as a near one)
          if (sizes[i] == "4p")
            pieces.intypes.push_back(m.types->getTypePointer(4, m.types->getBase(1, TYPE_UNKNOWN),
                                                             m.getDefaultDataSpace()->getWordSize()));
          else
            pieces.intypes.push_back(m.types->getBase(atoi(sizes[i].c_str()), TYPE_INT));
          pieces.innames.push_back("a" + std::to_string(i + 1));
        }
        if (f.result == 2 || f.result == 4)
          pieces.outtype = m.types->getBase(f.result, TYPE_INT);
        fp.setPieces(pieces);
        fp.setInputLock(true);
        // the result: what its instructions leave in AX (DX:AX) when it returns, where the
        // description says (an export's caller is Windows: nothing in the module shows it read);
        // otherwise two bytes or four, left to the analysis
        fp.setOutputLock(f.result == 2 || f.result == 4);
      } else {
        fp.setModel(m.getModel(f.model));
        if (f.result == 2 || f.result == 4) {
          // the arguments left to the analysis, the result as the description says
          PrototypePieces pieces;
          pieces.model = m.getModel(f.model);
          pieces.name = f.name;
          pieces.outtype = m.types->getBase(f.result, TYPE_INT);
          pieces.firstVarArgSlot = -1;
          fp.setPieces(pieces);
          fp.setInputLock(false);
          fp.setOutputLock(true);
        }
      }
      fp.setModelLock(true);
      // the bytes a function removes: its own `ret N` plus the return address
      if (f.pop >= 0) fp.setExtraPop(f.pop + ret_addr);
    }
  };
  for (const auto& x : d.externs) {
    Address ea(code, (uintb)x.va);
    applyModel(glb->addFunction(ea, x.name)->getFunction(), x);
  }
  // 16-bit: the software interrupts. The compiler spec turns `INT n` into a call to F000:n*16
  // (its swi fixup); each of those is a function int<n> taking its arguments and giving its
  // result in registers (__int16), so what the code puts in them before the interrupt is seen.
  if (!x86_32 && m.hasModel("__int16")) {
    for (const char* nm : {"__int16", "__int16keepds"})  // its registers are its arguments, by position
      if (m.hasModel(nm)) m.getModel(nm)->keepArgumentHoles();
    for (int v = 0; v < 256; ++v) {
      char nm[16];
      std::snprintf(nm, sizeof(nm), "int%02x", v);
      Fact x{kInterruptBase + (uint64_t)v * 16, nm, -1,
             (v == 0x21 || v == 0x2f || !m.hasModel("__int16keepds")) ? "__int16" : "__int16keepds"};
      Address ea(code, (uintb)x.va);
      applyModel(glb->addFunction(ea, x.name)->getFunction(), x);
    }
  }
  for (const auto& f : d.funcs) {
    Address addr(code, (uintb)f.va);
    Funcdata* fd = glb->addFunction(addr, f.name)->getFunction();
    applyModel(fd, f);
    if (f.model != "inline" && (d.only.empty() || d.only.count(f.va))) a.todo.push_back({fd, f});
  }
  for (MapIterator it = glb->begin(); it != glb->end(); ++it) a.registered.insert((*it)->getSymbol());
}

// Each function is decompiled from the description alone, not from what the functions before it
// left: the global variables (and functions called by address) an analysis names are its own,
// and go with it. Kept, the next function to touch the same global took over the type the
// earlier one had given it -- a function's C depended on which ran before it in the same process
// (`uRam10500071` alone, `xRam10500071` after another), and the per-function cache
// (codebreaker/decompile/decompiler.py) would have handed back either.
void forgetGlobals(Analysis& a) {
  Scope* glb = a.m->symboltab->getGlobalScope();
  std::set<Symbol*> made;
  for (MapIterator it = glb->begin(); it != glb->end(); ++it) {
    Symbol* sym = (*it)->getSymbol();
    if (!a.registered.count(sym)) made.insert(sym);
  }
  for (Symbol* sym : made) glb->removeSymbol(sym);
}

// ---- Structures (docs/CORE_ROADMAP.md, Track 1 item C) ---------------------------------------
// CB_STRUCTS=1. The analysis types memory by what each single access reads, so a structure shows
// as pointer arithmetic: `*(int4 *)(p + 8)`, `*(int2 *)(p + 0x10)`. Here every load and store is
// a constraint on the variable it goes through: "points at something with a field of this size at
// this offset". The address is followed back through constant additions (`q = p + 8; *(q + 4)` is
// p's field at 12) and, in 16-bit code, through a near segment (the DS or SS the function was
// entered with) or a far pointer -- a four-byte variable whose high word is the segment and whose
// low word, plus constants, the offset (`les bx, p; mov ax, es:[bx+8]`). A variable's constraints are solved
// together: it is a structure when it has fields at two or more offsets, none overlapping, none
// negative, and it is never indexed by a value only known when the program runs (that is an
// array). Such a variable is given a structure with exactly those fields -- their offsets and
// sizes, the rest of the layout unknown and left as padding -- and the function is analysed again
// with it, so its C reads `p->f_8`. Each structure is the function's own, named after it and the
// variable, and reported before the function as
//   /*@S <name> <bytes> <offset>:<bytes>:<type> ... */
// (each type one word: see typeToken)
// The function's prototype is put back afterwards: what one function learned about its own
// parameters does not change how the next one sees its calls (each function from the description
// alone, as with the globals).
bool g_structs = false;

// ----------------------------------------------------------------------------------------------- L2
// PrintIR, first stage (docs/IR_UNIFICATION.md, docs/L2_BINARY_TYPES.md): with CB_L2=1, each
// function is preceded by `/*@L2 {...} */`, its type as L2 JSON -- the hub IR's "fn": convention,
// near or far, the bytes it removes, each parameter's type and where it is (stack offset or
// register), the result's type and where it is. Pointers carry their address space. Off by
// default: the C is not changed. (Bodies come later; codebreaker.translate.l2.validate judges it.)
bool g_l2 = false;

std::string jsonText(const std::string& s) {
  std::string out = "\"";
  for (char c : s) {
    if (c == '"' || c == '\\') { out += '\\'; out += c; }
    else if ((unsigned char)c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); out += b; }
    else out += c;
  }
  return out + "\"";
}

std::string l2Type(Datatype* t, bool flat, int depth = 0) {
  if (t == nullptr || depth > 6) return "{\"kind\":\"class\",\"name\":\"undefined\"}";
  int n = (int)t->getSize();
  switch (t->getMetatype()) {
  case TYPE_VOID: return "{\"kind\":\"void\"}";
  case TYPE_BOOL: return "{\"kind\":\"bool\"}";
  case TYPE_FLOAT: return n == 4 ? "{\"kind\":\"f32\"}" : n == 8 ? "{\"kind\":\"f64\"}" : "{\"kind\":\"f80\"}";
  case TYPE_INT:
  case TYPE_UINT: {
    bool u = t->getMetatype() == TYPE_UINT;
    if (n == 1 && t->getName().find("char") != std::string::npos)
      return std::string("{\"kind\":\"char\",\"signed\":") + (u ? "false" : "true") + "}";
    if (n == 1 || n == 2 || n == 4 || n == 8)
      return "{\"kind\":\"" + std::string(u ? "u" : "i") + std::to_string(n * 8) + "\"}";
    break;
  }
  case TYPE_PTR: {
    Datatype* to = ((TypePointer*)t)->getPtrTo();
    std::string space = flat ? "flat" : n == 4 ? "far" : "near";
    if (to != nullptr && to->getMetatype() == TYPE_CODE)
      return "{\"kind\":\"fnptr\",\"space\":\"" + space + "\",\"fn\":{\"kind\":\"fn\",\"conv\":\"cdecl\","
             "\"params\":[],\"result\":{\"at\":\"none\"}}}";
    std::string seg = (!flat && n == 2) ? ",\"seg\":\"ds\"" : "";
    return "{\"kind\":\"ptr\",\"space\":\"" + space + "\"" + seg + ",\"to\":" + l2Type(to, flat, depth + 1) + "}";
  }
  case TYPE_ARRAY: {
    TypeArray* a = (TypeArray*)t;
    return "{\"kind\":\"array\",\"elem\":" + l2Type(a->getBase(), flat, depth + 1) +
           ",\"count\":" + std::to_string(a->numElements()) + "}";
  }
  case TYPE_STRUCT: {
    TypeStruct* s = (TypeStruct*)t;
    std::string f;
    for (auto it = s->beginField(); it != s->endField(); ++it) {
      if (!f.empty()) f += ",";
      f += "{\"name\":" + jsonText(it->name) + ",\"offset\":" + std::to_string(it->offset) +
           ",\"type\":" + l2Type(it->type, flat, depth + 1) + "}";
    }
    return "{\"kind\":\"record\",\"name\":" + jsonText(t->getName()) + ",\"role\":\"struct\",\"size\":" +
           std::to_string(n) + ",\"packed\":true,\"fields\":[" + f + "]}";
  }
  default: break;
  }
  // (what has no L2 kind of its own: a nominal type by the core's name for it)
  return "{\"kind\":\"class\",\"name\":" + jsonText(t->getName()) + "}";
}

std::string l2Location(Architecture& m, const Address& a, int size) {
  if (a.isInvalid()) return "\"none\"";
  AddrSpace* sp = a.getSpace();
  if (sp->getType() == IPTR_SPACEBASE) {                 // the stack: the offset from the entry SP, signed
    int bits = 8 * (int)sp->getAddrSize();
    long long off = (long long)a.getOffset();
    if (bits < 64 && ((off >> (bits - 1)) & 1)) off -= 1ll << bits;
    return "{\"stack\":" + std::to_string(off) + "}";
  }
  if (sp->getType() == IPTR_PROCESSOR) {
    std::string r = m.translate->getRegisterName(sp, a.getOffset(), size);
    if (!r.empty()) return "{\"reg\":" + jsonText(r) + "}";
    if (size == 4 && !m.translate->getRegisterName(sp, a.getOffset() + 2, 2).empty())
      return "{\"regs\":[" + jsonText(m.translate->getRegisterName(sp, a.getOffset() + 2, 2)) + "," +
             jsonText(m.translate->getRegisterName(sp, a.getOffset(), 2)) + "]}";
  }
  return "\"none\"";
}

std::string l2Function(Architecture& m, Funcdata* fd, bool flat) {
  const FuncProto& p = fd->getFuncProto();
  std::string model = p.getModelName();
  std::string conv = model.find("cdecl") != std::string::npos ? "cdecl"
                   : model.find("pascal") != std::string::npos ? "pascal"
                   : model.find("stdcall") != std::string::npos ? "stdcall"
                   : model.find("fastcall") != std::string::npos ? "fastcall"
                   : model.find("thiscall") != std::string::npos ? "thiscall" : "register";
  std::string distance = flat ? "flat" : model.find("far") != std::string::npos ? "far" : "near";
  int ret = flat ? 4 : distance == "far" ? 4 : 2;
  int xp = p.getExtraPop();
  std::string pops = xp == ProtoModel::extrapop_unknown ? "null" : std::to_string(xp - ret);
  std::string params;
  for (int i = 0; i < p.numParams(); ++i) {
    ProtoParameter* q = p.getParam(i);
    if (!params.empty()) params += ",";
    params += "{\"name\":" + jsonText(q->getName()) + ",\"type\":" + l2Type(q->getType(), flat) +
              ",\"at\":" + l2Location(m, q->getAddress(), q->getType() ? (int)q->getType()->getSize() : 0) + "}";
  }
  ProtoParameter* o = p.getOutput();
  Datatype* ot = o ? o->getType() : nullptr;
  std::string result = (ot == nullptr || ot->getMetatype() == TYPE_VOID)
      ? "{\"at\":\"none\"}"
      : "{\"type\":" + l2Type(ot, flat) + ",\"at\":" + l2Location(m, o->getAddress(), (int)ot->getSize()) + "}";
  std::string fn = "{\"kind\":\"fn\",\"conv\":\"" + conv + "\",\"distance\":\"" + distance + "\",\"pops\":" + pops +
                   ",\"params\":[" + params + "],\"result\":" + result + ",\"variadic\":" +
                   (p.isDotdotdot() ? "true" : "false") + "}";
  // (where it is in the original: a 16-bit function by segment and offset -- the description gives
  // segment n the made-up selector 1000h + 8(n-1) -- a 32-bit one by its address)
  uint64_t va = fd->getAddress().getOffset();
  std::string origin = flat ? "{\"module\":\"\",\"va\":" + std::to_string(va) + "}"
      : "{\"module\":\"\",\"segment\":" + std::to_string((((va >> 16) & 0xFFFF) - 0x1000) / 8 + 1) +
        ",\"offset\":" + std::to_string(va & 0xFFFF) + "}";
  return "{\"node\":\"MethodDecl\",\"name\":" + jsonText(fd->getName()) + ",\"kind\":\"function\"," +
         "\"sym\":{\"kind\":\"function\",\"name\":" + jsonText(fd->getName()) + ",\"static\":true,\"type\":" + fn + "}," +
         "\"ret\":" + (ot == nullptr ? std::string("{\"kind\":\"void\"}") : l2Type(ot, flat)) +
         ",\"origin\":" + origin + "}";
}

struct FieldUse { int size; Datatype* type; };

// A function's prototype as the description set it, to put back once the function is written
struct ProtoSnap {
  PrototypePieces pieces;
  bool in_lock = false, out_lock = false, model_lock = false;
  int4 pop = 0;
};

ProtoSnap snapProto(Machine& m, FuncProto& fp, const std::string& name) {
  ProtoSnap s;
  s.pieces.model = m.getModel(fp.getModelName());
  s.pieces.name = name;
  s.pieces.outtype = fp.getOutputType();
  s.pieces.firstVarArgSlot = fp.isDotdotdot() ? fp.numParams() : -1;
  for (int4 i = 0; i < fp.numParams(); ++i) {
    s.pieces.intypes.push_back(fp.getParam(i)->getType());
    s.pieces.innames.push_back(fp.getParam(i)->getName());
  }
  s.in_lock = fp.isInputLocked();
  s.out_lock = fp.isOutputLocked();
  s.model_lock = fp.isModelLocked();
  s.pop = fp.getExtraPop();
  return s;
}

void restoreProto(FuncProto& fp, const ProtoSnap& s) {
  if (s.in_lock) fp.setPieces(s.pieces);
  fp.setExtraPop(s.pop);
  fp.setInputLock(s.in_lock);                    // (unlocked: the analysis's parameters go with it)
  fp.setOutputLock(s.out_lock);
  fp.setModelLock(s.model_lock);
}
struct Sketch {
  std::map<int64_t, FieldUse> fields;
  bool bad = false;
  bool isFar = false;                                // (16-bit: reached as a far pointer)
};

// The high-level variable a varnode belongs to, or null (an annotation has none)
HighVariable* highOf(Varnode* vn) {
  try { return vn->getHigh(); } catch (LowlevelError&) { return nullptr; }
}

// The variable an address is `base + offset` of, through constant additions
bool baseOf(Varnode* vn, Varnode*& base, int64_t& off) {
  off = 0;
  for (int depth = 0; depth < 16; ++depth) {
    if (vn->isConstant() || vn->isSpacebase() || vn->isAnnotation()) return false;
    PcodeOp* def = vn->isWritten() ? vn->getDef() : nullptr;
    if (def != nullptr) {
      const OpCode oc = def->code();
      if ((oc == CPUI_INT_ADD || oc == CPUI_PTRSUB) && def->getIn(1)->isConstant()) {
        const Varnode* c = def->getIn(1);
        off += sign_extend(c->getOffset(), c->getSize() * 8 - 1);
        vn = def->getIn(0);
        continue;
      }
      // a copy is the same pointer: the constraint belongs to what it was copied from (a
      // parameter moved to a register keeps its fields)
      if ((oc == CPUI_COPY || oc == CPUI_CAST) && !def->getIn(0)->isConstant() &&
          def->getIn(0)->getSize() == vn->getSize()) {
        vn = def->getIn(0);
        continue;
      }
      if (oc == CPUI_PTRADD && def->getIn(1)->isConstant() && def->getIn(2)->isConstant()) {
        const Varnode* c = def->getIn(1);
        off += sign_extend(c->getOffset(), c->getSize() * 8 - 1) * (int64_t)def->getIn(2)->getOffset();
        vn = def->getIn(0);
        continue;
      }
    }
    base = vn;
    return true;
  }
  return false;
}

// The near offset a 16-bit address is made from, or null when its segment is not the one the
// function was entered with (a far pointer)
Varnode* nearOffset(Varnode* addr) {
  if (!addr->isWritten() || addr->getDef()->code() != CPUI_SEGMENTOP) return addr;
  PcodeOp* seg = addr->getDef();
  const Varnode* s = seg->getIn(1);
  // (the segment the function was entered with is a register -- DS, SS; a parameter on the
  // stack is an input too, but the high word of a far pointer passed as two words: USER
  // sub_7_3B0C's `a2:a1`, its offset half typed a near structure, read a byte at a time)
  if (s->isConstant() || (s->isInput() && s->getSpace()->getType() == IPTR_PROCESSOR)) return seg->getIn(2);
  return nullptr;
}

// The four-byte variable a 16-bit far address `segment(s, o)` is made from: s its high word and o
// its low word (plus constants, added to `off`), or null
Varnode* farBase(Varnode* addr, int64_t& off) {
  if (!addr->isWritten() || addr->getDef()->code() != CPUI_SEGMENTOP) return nullptr;
  PcodeOp* seg = addr->getDef();
  Varnode* s = seg->getIn(1);
  Varnode* lo;
  if (!baseOf(seg->getIn(2), lo, off)) return nullptr;
  auto piece = [](Varnode* v, int at) -> Varnode* {   // v = SUBPIECE(w, at) of a 4-byte w, through copies
    for (int depth = 0; depth < 8 && v->isWritten(); ++depth) {
      PcodeOp* d = v->getDef();
      if (d->code() == CPUI_COPY) { v = d->getIn(0); continue; }
      if (d->code() == CPUI_SUBPIECE && d->getIn(0)->getSize() == 4 && (int)d->getIn(1)->getOffset() == at)
        return d->getIn(0);
      return nullptr;
    }
    return nullptr;
  };
  Varnode* hi = piece(s, 2);
  Varnode* low = piece(lo, 0);
  if (hi == nullptr || low == nullptr) return nullptr;
  HighVariable* a = highOf(hi);
  if (hi != low && (a == nullptr || a != highOf(low))) return nullptr;
  return low;
}

// Is any member of h added to a value not known until run time (an index: an array, not a
// structure)?
bool indexed(HighVariable* h) {
  std::vector<const Varnode*> work;
  std::set<const Varnode*> seen;
  for (int4 i = 0; i < h->numInstances(); ++i) work.push_back(h->getInstance(i));
  while (!work.empty()) {                          // (and its copies)
    const Varnode* vn = work.back();
    work.pop_back();
    if (!seen.insert(vn).second || seen.size() > 256) continue;
    for (auto it = vn->beginDescend(); it != vn->endDescend(); ++it) {
      const PcodeOp* op = *it;
      const OpCode oc = op->code();
      if (oc == CPUI_INT_ADD || oc == CPUI_INT_SUB || oc == CPUI_PTRADD) {
        for (int4 k = 0; k < op->numInput(); ++k)
          if (op->getIn(k) != vn && !op->getIn(k)->isConstant()) return true;
      }
      if ((oc == CPUI_COPY || oc == CPUI_CAST) && op->getOut() != nullptr) work.push_back(op->getOut());
    }
  }
  return false;
}

// A field's type as one word for the `/*@S` note: its name (`int2`, `xunknown4`, `code`, another
// structure's), and for a pointer the pointed-at type's word followed by `*` -- or, a 16-bit far
// pointer (four bytes in a two-byte program), by `^`
std::string typeToken(Datatype* t, bool seg16, int depth = 0) {
  if (t == nullptr) return "xunknown1";
  if (t->getMetatype() == TYPE_PTR && depth < 4) {
    const TypePointer* p = (const TypePointer*)t;
    return typeToken(p->getPtrTo(), seg16, depth + 1) + ((seg16 && p->getSize() == 4) ? "^" : "*");
  }
  std::string n = t->getName();
  bool plain = !n.empty();
  for (char c : n)
    if (!(std::isalnum((unsigned char)c) || c == '_')) plain = false;
  if (!plain) return "xunknown" + std::to_string(t->getSize() > 0 ? t->getSize() : 1);
  return n;
}

std::string hexOf(int64_t v) {
  char b[24];
  std::snprintf(b, sizeof(b), "%llx", (unsigned long long)v);
  return b;
}

// A byte read and written straight back to where it came from (`p->f_0 = p->f_0`): Windows
// probes that a caller's buffer is writable this way, under an exception handler
bool probeLoad(const PcodeOp* load) {
  const Varnode* v = load->getOut();
  if (v == nullptr || v->hasNoDescend()) return false;
  for (auto it = v->beginDescend(); it != v->endDescend(); ++it) {
    const PcodeOp* use = *it;
    if (use->code() != CPUI_STORE || use->getIn(2) != v || use->getIn(1) != load->getIn(1)) return false;
  }
  return true;
}

// Gather each variable's field constraints, solve them, and give the variables that are
// structures their structure. -> the notes to write before the function (empty: nothing changed)
std::string recoverStructs(Machine& m, Funcdata* fd, const Fact& f, bool seg16, bool& proto_changed, bool& changed) {
  changed = false;
  std::map<HighVariable*, Sketch> sketches;
  for (auto it = fd->beginOpAlive(); it != fd->endOpAlive(); ++it) {
    PcodeOp* op = *it;
    Varnode* addr;
    Varnode* val;
    if (op->code() == CPUI_LOAD) { addr = op->getIn(1); val = op->getOut(); }
    else if (op->code() == CPUI_STORE) { addr = op->getIn(1); val = op->getIn(2); }
    else continue;
    if (val == nullptr) continue;
    // a value read and never used, or only written back, says only that the memory is there --
    // Windows probes a caller's buffer by touching its first and last byte -- not what is in it
    if (op->code() == CPUI_LOAD && val->hasNoDescend()) continue;
    if (op->code() == CPUI_LOAD ? probeLoad(op)
                                : val->isWritten() && val->getDef()->code() == CPUI_LOAD && probeLoad(val->getDef()))
      continue;
    Varnode* base;
    int64_t off;
    HighVariable* bh;
    bool isFar = false;
    if (seg16) {
      Varnode* nearAddr = nearOffset(addr);
      if (nearAddr != nullptr) addr = nearAddr;
      else if ((base = farBase(addr, off)) != nullptr) isFar = true;
      else continue;
    }
    if (isFar ? (bh = highOf(base)) == nullptr : !baseOf(addr, base, off) || (bh = highOf(base)) == nullptr) continue;
    Sketch& s = sketches[bh];
    if (!s.fields.empty() && s.isFar != isFar) { s.bad = true; continue; }   // (near and far at once: neither)
    s.isFar = isFar;
    if (off < 0 || off > 0xffff) { s.bad = true; continue; }
    auto at = s.fields.find(off);
    if (at != s.fields.end() && at->second.size != val->getSize()) { s.bad = true; continue; }
    if (at == s.fields.end()) s.fields[off] = {val->getSize(), highOf(val) ? highOf(val)->getType() : nullptr};
  }
  std::string notes;
  const int ptr_size = seg16 ? 2 : (int)m.getDefaultDataSpace()->getAddrSize();
  ScopeLocal* loc = fd->getScopeLocal();
  FuncProto& fp = fd->getFuncProto();
  // the variables that are structures, in an order that does not depend on where the analysis's
  // objects happen to sit in memory: parameters by position, then locals by name
  struct Candidate { Symbol* sym; bool param; int index; std::string name; const Sketch* sketch; int64_t end; };
  std::vector<Candidate> found;
  for (auto& kv : sketches) {
    HighVariable* h = kv.first;
    const Sketch& s = kv.second;
    if (s.bad || s.fields.size() < 2 || h->getInstance(0)->getSize() != (s.isFar ? 4 : ptr_size)) continue;
    Symbol* sym = h->getSymbol();
    // (the function's own: a global is every function's, and not this one's to type)
    if (sym == nullptr || sym->getScope() != loc || indexed(h)) continue;
    Datatype* now = h->getType();
    if (now->getMetatype() == TYPE_PTR && ((TypePointer*)now)->getPtrTo()->getMetatype() == TYPE_STRUCT) continue;
    int64_t end = 0;
    bool overlap = false;
    for (const auto& fu : s.fields) {
      if (fu.first < end) overlap = true;
      end = fu.first + fu.second.size;
    }
    if (overlap) continue;
    const bool param = sym->getCategory() == Symbol::function_parameter;
    const int index = param ? (int)sym->getCategoryIndex() : -1;
    if (param && index >= fp.numParams()) continue;
    found.push_back({sym, param, index, sym->getName(), &s, end});
  }
  std::sort(found.begin(), found.end(), [](const Candidate& x, const Candidate& y) {
    if (x.param != y.param) return x.param;
    if (x.index != y.index) return x.index < y.index;
    return x.name < y.name;
  });
  std::map<int, Datatype*> param_types;            // parameter position -> its new type
  std::set<const Symbol*> done;
  for (const Candidate& c : found) {
    if (!done.insert(c.sym).second) continue;      // (two variables of one symbol: typed once)
    std::string name = "s_" + f.name + "_" +
                       (!c.name.empty() ? c.name : c.param ? "p" + std::to_string(c.index + 1) : std::string("v"));
    for (char& ch : name)                          // (a C name: a decorated function's `?`, `@` ...)
      if (!(std::isalnum((unsigned char)ch) || ch == '_')) ch = '_';
    if (m.types->findByName(name) != nullptr) continue;
    TypeStruct* st = m.types->getTypeStruct(name);
    std::vector<TypeField> fields;
    std::string line = "/*@S " + name + " " + std::to_string(c.end);
    int4 id = 0;
    for (const auto& fu : c.sketch->fields) {
      Datatype* ft = fu.second.type;
      if (ft == nullptr || ft->getSize() != fu.second.size || ft->getMetatype() == TYPE_STRUCT ||
          ft->getMetatype() == TYPE_ARRAY || ft->getMetatype() == TYPE_UNION || ft->getMetatype() == TYPE_PARTIALSTRUCT)
        ft = m.types->getBase(fu.second.size, TYPE_UNKNOWN);
      fields.emplace_back(id++, (int4)fu.first, "f_" + hexOf(fu.first), ft);
      line += " " + hexOf(fu.first) + ":" + std::to_string(fu.second.size) + ":" +
              typeToken(ft, seg16);
    }
    m.types->assignFixedFields(st, fields, (int4)c.end);
    Datatype* ptr = m.types->getTypePointer(c.sketch->isFar ? 4 : ptr_size, st, m.getDefaultDataSpace()->getWordSize());
    if (c.param) {
      param_types[c.index] = ptr;
    } else {
      loc->retypeSymbol(c.sym, ptr);
      loc->setAttribute(c.sym, Varnode::typelock);
    }
    changed = true;
    notes += line + " */\n";
  }
  if (!param_types.empty()) {
    // a parameter's type is part of the prototype: lock the prototype as it now stands, with
    // those parameters pointers to their structures -- all at once (setting the prototype makes
    // its parameters' symbols anew)
    PrototypePieces pieces;
    pieces.model = m.getModel(fp.getModelName());
    pieces.name = f.name;
    pieces.outtype = fp.getOutputType();
    pieces.firstVarArgSlot = fp.isDotdotdot() ? fp.numParams() : -1;
    for (int4 i = 0; i < fp.numParams(); ++i) {
      ProtoParameter* p = fp.getParam(i);
      auto nt = param_types.find(i);
      pieces.intypes.push_back(nt != param_types.end() ? nt->second : p->getType());
      pieces.innames.push_back(p->getName());
    }
    const int4 pop = fp.getExtraPop();
    const bool out_locked = fp.isOutputLocked();
    fp.setPieces(pieces);
    fp.setExtraPop(pop);
    fp.setInputLock(true);
    fp.setOutputLock(out_locked);
    fp.setModelLock(true);
    proto_changed = true;
  }
  return notes;
}

// Decompile the i-th function to write, onto `out`: its marker, what it touches, its C
void decompileOne(Analysis& a, const Description& d, size_t i, std::ostream& out) {
  Machine& m = *a.m;
  auto& t = a.todo[i];
  AddrSpace* code = m.getDefaultCodeSpace();
  m.print->setOutputStream(&out);
  out << "/*@@ " << t.second.name << " " << std::hex << t.second.va << std::dec << " */" << std::endl;
  governor::begin_function();
  ProtoSnap snap;
  bool proto_changed = false;
  if (g_structs) snap = snapProto(m, t.first->getFuncProto(), t.second.name);
  try {
    // its calls through pointers: each passed the words the code pushed before it, removed
    // by the callee (Windows' drivers and callbacks are Pascal). Every call's override is
    // offered, by the call's own address: the function the description named for a call may
    // since have been found to be two, and the call belongs to the one that holds it (an
    // override at an address the function has no call at is never used).
    for (const auto& c : d.calls) {
      if (!m.hasModel(c.model)) continue;
      FuncProto* p = new FuncProto();
      p->setInternal(m.getModel(c.model), m.types->getTypeVoid());
      PrototypePieces pieces;
      pieces.model = m.getModel(c.model);
      pieces.name = "";
      pieces.outtype = m.types->getBase(2, TYPE_UNKNOWN);
      pieces.firstVarArgSlot = -1;
      for (int k = 0; k < c.pop / 2; ++k) {
        pieces.intypes.push_back(m.types->getBase(2, TYPE_INT));
        pieces.innames.push_back("p" + std::to_string(k + 1));
      }
      p->setPieces(pieces);
      p->setInputLock(true);
      p->setOutputLock(false);
      // (the callee removes them -- or, a C function's, its caller does: only the return address)
      p->setExtraPop((c.model.find("cdecl") != std::string::npos ? 0 : c.pop) +
                     (c.model.find("far") != std::string::npos ? 4 : 2));
      t.first->getOverride().insertProtoOverride(Address(code, (uintb)c.site), p);
    }
    // its switch jumps: each jump belongs to the function that starts last before it in its
    // segment (the 16-bit segment is the address's upper half)
    for (const auto& j : d.jumps) {
      uint64_t owner = 0;
      for (const auto& f : d.funcs)
        if (f.va <= j.site && f.va > owner && (f.va >> 16) == (j.site >> 16)) owner = f.va;
      if (owner != t.second.va) continue;
      std::vector<Address> addrs;
      for (uint64_t ta : j.targets) addrs.push_back(Address(code, (uintb)ta));
      JumpTable* jt = t.first->installJumpTable(Address(code, (uintb)j.site));
      jt->setOverride(addrs, Address(), 0, 0);
    }
    Action* act = m.allacts.getCurrent();
    act->reset(*t.first);
    act->perform(*t.first);
    std::string struct_notes;
    if (g_structs) {
      bool changed = false;
      struct_notes = recoverStructs(m, t.first, t.second, d.arch.find(":16:") != std::string::npos,
                                    proto_changed, changed);
      if (changed) {                             // analysed again, with its structures --
        governor::begin_function();              // a new analysis, with its own time and steps
        // -- its globals kept as the first analysis typed them: a structure is the function's
        // own, and a global copied to or from a variable given one took it over (GDI32
        // `piRambff4b328->f_10`, a global printed as a structure nothing declares it as)
        Scope* glb = m.symboltab->getGlobalScope();
        std::vector<Symbol*> made;
        for (MapIterator it = glb->begin(); it != glb->end(); ++it) {
          Symbol* sym = (*it)->getSymbol();
          if (!a.registered.count(sym) && !sym->isTypeLocked()) made.push_back(sym);
        }
        for (Symbol* sym : made) glb->setAttribute(sym, Varnode::typelock);
        m.clearAnalysis(t.first);
        act->reset(*t.first);
        act->perform(*t.first);
      }
    }
    // every global the function touches, with its size and -- for a pointer -- the size of
    // what it points at, from whichever of its varnodes carries the type it is printed with
    std::map<std::pair<uint64_t, int>, int> globals;
    AddrSpace* ram = m.getDefaultDataSpace();
    for (VarnodeLocSet::const_iterator it = t.first->beginLoc(ram); it != t.first->endLoc(ram); ++it) {
      const Varnode* vn = *it;
      if (vn->isAnnotation()) continue;
      uint64_t va = vn->getOffset();
      if (!a.image->holds(va)) continue;
      int& elem = globals[{va, (int)vn->getSize()}];
      Datatype* ct = vn->getType();
      try {                                              // (the type it is printed with: its variable's)
        if (vn->getHigh() != nullptr) ct = vn->getHigh()->getType();
      } catch (...) {
      }
      if ((ct == nullptr || ct->getMetatype() != TYPE_PTR)) {
        // (an unnamed global is printed from the symbol the core made for it: its type)
        const SymbolEntry* e = m.symboltab->getGlobalScope()->queryContainer(vn->getAddr(), vn->getSize(), Address());
        if (e != nullptr && e->getSymbol() != nullptr) ct = e->getSymbol()->getType();
      }
      if (ct != nullptr && ct->getMetatype() == TYPE_PTR && elem == 0)
        elem = (int)((TypePointer*)ct)->getPtrTo()->getSize();
    }
    for (const auto& g : globals)
      out << "/*@G " << std::hex << g.first.first << std::dec << " " << g.first.second << " "
          << g.second << " */" << std::endl;
    // 16-bit code has two sizes of pointer and C text does not show which a variable is:
    // report every pointer-typed local and parameter with its size, `/*@P <name> <bytes> */`.
    if (!d.x86_32) {
      ScopeLocal* loc = t.first->getScopeLocal();
      std::set<std::string> said;
      for (MapIterator it = loc->begin(); it != loc->end(); ++it) {
        const Symbol* sym = (*it)->getSymbol();
        if (sym == nullptr || sym->getType() == nullptr) continue;
        if (sym->getType()->getMetatype() != TYPE_PTR) continue;
        if (said.insert(sym->getName()).second)
          out << "/*@P " << sym->getName() << " " << sym->getType()->getSize() << " */" << std::endl;
      }
      // ...and the function's own result, under the name `return`
      Datatype* rt = t.first->getFuncProto().getOutputType();
      if (rt != nullptr && rt->getMetatype() == TYPE_PTR)
        out << "/*@P return " << rt->getSize() << " */" << std::endl;
    }
    out << struct_notes;
    if (g_l2) out << "/*@L2 " << l2Function(m, t.first, d.x86_32) << " */" << std::endl;
    m.print->docFunction(t.first);
  } catch (LowlevelError& e) {
    out << "/* decompile failed: " << e.explain << " */" << std::endl;
  }
  if (proto_changed) restoreProto(t.first->getFuncProto(), snap);
  out << std::endl;
  governor::end_function();
  m.clearAnalysis(t.first);                      // free this function's analysis; keep its symbol
  t.first->getOverride().clear();                // ...and the call overrides it was given (they were
                                                 // kept for the whole run: every function, every call)
  forgetGlobals(a);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    std::fprintf(stderr, "usage: %s <description> [--at <hexVA>...]\n", argv[0]);
    return 2;
  }
  const char* specs = std::getenv("CB_LANGUAGES");
  if (!specs) { std::fprintf(stderr, "FAIL: CB_LANGUAGES is not set\n"); return 1; }
  governor::set_up();

  Description d;
  d.path = argv[1];
  {
    auto spans = std::make_shared<std::vector<Span>>();
    std::ifstream in(argv[1]);
    if (!in) { std::fprintf(stderr, "FAIL: cannot read %s\n", argv[1]); return 1; }
    std::string line;
    while (std::getline(in, line)) {
      std::vector<std::string> f = split(line);
      if (f.size() >= 2 && f[0] == "arch") d.arch = f[1];
      else if (f.size() >= 5 && f[0] == "bytes") {
        Span s;
        s.va = strtoull(f[1].c_str(), nullptr, 16);
        if (!readFile(f[2], strtoull(f[3].c_str(), nullptr, 16), strtoull(f[4].c_str(), nullptr, 16), s.data)) {
          std::fprintf(stderr, "FAIL: cannot read %s\n", f[2].c_str());
          return 1;
        }
        spans->push_back(std::move(s));
      } else if (f.size() >= 3 && (f[0] == "F" || f[0] == "I" || f[0] == "X")) {
        Fact x{strtoull(f[1].c_str(), nullptr, 16), f[2], f.size() > 3 ? atoi(f[3].c_str()) : -1,
               f.size() > 4 ? f[4] : std::string()};
        if (f.size() > 5 && !f[5].empty()) x.result = atoi(f[5].c_str());
        if (f.size() > 6 && !f[6].empty()) {        // (a cdecl function: its arguments' sizes, `4,2,2`)
          std::stringstream as(f[6]);
          std::string a;
          while (std::getline(as, a, ',')) if (!a.empty()) x.args.push_back(a);
        }
        (f[0] == "I" ? d.imports : f[0] == "X" ? d.externs : d.funcs).push_back(x);
      } else if (f.size() >= 5 && f[0] == "O") {
        d.calls.push_back({strtoull(f[1].c_str(), nullptr, 16), strtoull(f[2].c_str(), nullptr, 16),
                           atoi(f[3].c_str()), f[4]});
      } else if (f.size() >= 3 && f[0] == "J") {
        JumpOverride j{strtoull(f[1].c_str(), nullptr, 16), {}};
        std::stringstream ts(f[2]);
        std::string t;
        while (std::getline(ts, t, ',')) if (!t.empty()) j.targets.push_back(strtoull(t.c_str(), nullptr, 16));
        if (!j.targets.empty()) d.jumps.push_back(j);
      } else if (f.size() >= 3 && f[0] == "reg") {
        d.regs.push_back({f[1], strtoull(f[2].c_str(), nullptr, 16)});
      }
    }
    d.spans = spans;
  }
  if (d.arch.empty() || d.spans->empty()) { std::fprintf(stderr, "FAIL: the description names no arch or no bytes\n"); return 1; }
  for (int i = 2; i < argc; ++i)
    if (std::string(argv[i]) == "--at")
      for (++i; i < argc && argv[i][0] != '-'; ++i) d.only.insert(strtoull(argv[i], nullptr, 16));
  d.x86_32 = d.arch.rfind("x86:LE:32", 0) == 0;
  if (const char* sv = std::getenv("CB_STRUCTS")) g_structs = std::atoi(sv) != 0;
  if (const char* lv = std::getenv("CB_L2")) g_l2 = std::atoi(lv) != 0;

  try {
    startDecompilerLibrary(std::vector<std::string>{});
    SleighArchitecture::specpaths.addDir2Path(specs);
  } catch (LowlevelError& e) {
    std::fprintf(stderr, "FAIL: %s\n", e.explain.c_str());
    return 1;
  }

  if (governor::threads == 1) {
    // One thread: each function's marker is written before it is analysed, so a process that
    // dies on a function shows which (codebreaker/decompile/decompiler.py: _run_resuming)
    governor::claim_slot();
    try {
      Analysis a;
      setUp(a, d);
      for (size_t i = 0; i < a.todo.size(); ++i) decompileOne(a, d, i, std::cout);
      if (!a.errs.str().empty()) std::fprintf(stderr, "%s\n", a.errs.str().c_str());
    } catch (LowlevelError& e) {
      std::fprintf(stderr, "FAIL: %s\n", e.explain.c_str());
      return 1;
    } catch (DecoderError& e) {
      std::fprintf(stderr, "FAIL: %s\n", e.explain.c_str());
      return 1;
    }
    return 0;
  }

  // CB_THREADS=N: N threads, each with its own analysis of the whole description, take the
  // functions in turn. Each function's text is written whole, in the description's order, as soon
  // as it and every one before it are done -- the output is what one thread writes.
  size_t total = 0;
  for (const auto& f : d.funcs)
    if (f.model != "inline" && (d.only.empty() || d.only.count(f.va))) ++total;
  std::mutex set_up_mu, out_mu;                    // (setting up runs the core's p-code compiler,
  std::atomic<size_t> next{0};                     //  whose parser state is global: one at a time)
  std::vector<std::string> text(total);
  std::vector<char> ready(total, 0);
  size_t written = 0;
  std::string failed, warnings;
  auto work = [&] {
    governor::claim_slot();
    try {
      Analysis a;
      {
        std::lock_guard<std::mutex> lock(set_up_mu);
        setUp(a, d);
      }
      for (size_t i; (i = next.fetch_add(1)) < total;) {
        std::ostringstream out;
        decompileOne(a, d, i, out);
        std::lock_guard<std::mutex> lock(out_mu);
        text[i] = out.str();
        ready[i] = 1;
        for (; written < total && ready[written]; ++written) {
          std::fwrite(text[written].data(), 1, text[written].size(), stdout);
          std::string().swap(text[written]);
        }
        std::fflush(stdout);
      }
      std::lock_guard<std::mutex> lock(out_mu);
      if (warnings.empty()) warnings = a.errs.str();  // (every analysis reads the same files)
    } catch (LowlevelError& e) {
      std::lock_guard<std::mutex> lock(out_mu);
      failed = e.explain;
      next = total;
    } catch (DecoderError& e) {
      std::lock_guard<std::mutex> lock(out_mu);
      failed = e.explain;
      next = total;
    }
  };
  const int n = (int)std::min<size_t>((size_t)governor::threads, std::max<size_t>(total, 1));
  std::vector<std::thread> pool;
  for (int k = 0; k < n; ++k) pool.emplace_back(work);
  for (auto& th : pool) th.join();
  if (!warnings.empty()) std::fprintf(stderr, "%s\n", warnings.c_str());
  if (!failed.empty()) {
    std::fprintf(stderr, "FAIL: %s\n", failed.c_str());
    return 1;
  }
  return 0;
}
