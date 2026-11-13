// cb/emulate/windows16.hpp -- run one function of a 16-bit Windows module (NE) in protected mode and record what it did.
//
//     Runner runner(Environment{.governor = &governor, .models = models, .argument_bytes = sizes});
//     ProbeOptions options;  options.function = Ordinal{7};  options.args = {1, 2};
//     Trace16 t = runner.probe("lzexpand.dll", options);
//     t.ending, t.ax, t.dx_ax, t.imports_hit, t.observed.path, t.observed.mem_writes ...
//     emulate::Run run = record(t);                      // the one run record the judge compares (cb/emulate/run.hpp)
//
// The module's segments are laid out behind selectors of their own, each in a 64K window of one Unicorn engine, with their
// relocations applied; every import is a far address in a segment of landing places, so a call to Windows is seen, named, and
// returned from with the arguments it was given. The function is entered the way it is really called (far, or near with a
// return offset only), with its arguments pushed left to right (Pascal). Memory and the instruction stream are kept by the
// 16-bit recorder (cb/emulate/native_win16); the events it hands up -- an import entered, a callee that is not run, an interrupt
// -- are this module's. What a run did is judged in verify/lanes/win16.
//
// `trace` runs the function under an environment the caller models call by call (the `import_stub`); `probe` runs it under the
// controlled Win16 environment (`default_stub`: the knowledge base's stand-ins, a far heap whose blocks are real selectors).
//
// What is kept between runs: the module, parsed and fixed up once (each run after the first writes the fixed-up segments as they
// were; walking every fix-up chain again was most of a run's setup), the pattern a data segment's tail is filled with, and where
// the recorded system's `retf n` removes its arguments. All of it is guarded: one Runner may be asked by several threads; the
// engine of each run is its own. The governor in the Environment is the caller's and is not: one thread per governor.
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

#include "cb/core/bytes.hpp"
#include "cb/core16_types.hpp"
#include "cb/emulate/emulator.hpp"
#include "cb/emulate/native_shadow.hpp"
#include "cb/emulate/native_trace.hpp"
#include "cb/emulate/native_win16.hpp"
#include "cb/emulate/ports16.hpp"
#include "cb/emulate/run.hpp"

namespace cb::knowledge {
class Knowledge;
}

namespace cb::emulate::windows16 {

// ---- the layout of the machine (linear addresses of a 32-bit protected-mode engine) ------------------------------------------------
inline constexpr std::uint32_t kGdtAddress = 0x1000;
inline constexpr std::uint32_t kSegmentOrigin = 0x100000;   // the first NE segment's base; each gets a 64K window of its own
inline constexpr std::uint32_t kSegmentStride = 0x10000;
inline constexpr std::uint32_t kStackBase = 0x900000;       // the stack segment
inline constexpr std::uint32_t kSentinelBase = 0xA00000;    // the import-sentinel segment: a far call here is a call to Windows
inline constexpr std::uint32_t kScratchBase = 0xB00000;     // the segment every selector the run does not name points at
inline constexpr unsigned kGdtSlots = 512;                  // a dense GDT: every selector index is present, so loads and far returns never #GP
inline constexpr unsigned kStackSlot = 101;                 // the stack's GDT index, the sentinel's the next: the same whatever the module
inline constexpr unsigned kModuleSlot = 300;                // the module's segment n is at GDT index kModuleSlot + n - 1
inline constexpr unsigned kHeapSlots = 24;                  // GDT indices kept at the top for far-heap blocks
inline constexpr std::uint32_t kImportStride = 0x10;        // the size of an import's place in the sentinel segment
inline constexpr std::uint16_t kEntryAx = 0x7E57;           // AX as a function is entered: a function that sets no result hands it back
inline constexpr std::uint16_t kNearReturn = 0xFFEE;        // where a near function entered by the tracer returns to
inline constexpr std::uint16_t kFarReturn = 0xFFF0;         // the offset in the sentinel segment a far function returns to
inline constexpr std::uint32_t kLdtAddress = 0x1F00000;     // a recorded call's LDT
inline constexpr std::uint32_t kForeignBase = 0x2000000;    // the recorded segments' copies, a window each
inline constexpr std::uint32_t kSnapshotBase = 0x10000000;  // a recorded call's whole memory
inline constexpr std::uint32_t kOwnBase = 0x8000000;        // the module's own code as the recorded memory's selectors name it
inline constexpr std::uint32_t kHeapBase = 0xB00000;        // the first far-heap block (the scratch window's own address: block 0 is the scratch)
inline constexpr std::uint32_t kHeapStride = 0x10000;

// The selector for GDT index `index` at privilege `rpl`.
constexpr std::uint16_t selector(unsigned index, unsigned rpl = 0) { return static_cast<std::uint16_t>((index << 3) | (rpl & 3)); }
inline constexpr std::uint16_t kSafeSelector = 200 << 3;                // names a real, writable scratch segment
inline constexpr std::uint16_t kStackSelector = kStackSlot << 3;        // the stack's, whatever the module

// One 8-byte GDT descriptor: a present, byte-granular segment of `limit`+1 bytes at `base`, 16-bit unless `big`.
std::array<std::uint8_t, 8> descriptor(std::uint32_t base, std::uint32_t limit, bool code, unsigned dpl = 0, bool big = false);

// Where the module's segment `index` (1-based) is laid out: its own 64K window.
constexpr std::uint32_t segment_base(unsigned index) { return kSegmentOrigin + index * kSegmentStride; }

// The base the descriptor for `selector` gives, read from the GDT as it is now (0 where the table cannot be read).
std::uint32_t selector_base(core16::Uc& machine, std::uint32_t selector);

// ---- what a run is asked --------------------------------------------------------------------------------------------------------

// An exported function by its entry-table ordinal, or by its place: an NE segment number (1-based) and an offset in it.
struct Ordinal {
    std::uint32_t number = 0;
};
struct CodePoint {
    std::uint32_t segment = 0;
    std::uint32_t offset = 0;
};
using Function = std::variant<Ordinal, CodePoint>;

// What an import's stand-in answers: DX:AX the result and the Pascal argument bytes the callee removes. Absent: not given (an
// unmodelled call answers 0, and probe takes the argument bytes from the import's own export).
struct StubResult {
    std::optional<std::uint32_t> ax, dx, pop;
};
// A caller that models Windows calls itself: told each call (the hit may be annotated: `unmodelled`, `arg_bytes`), answers it, or
// answers nothing (AX = 0, no arguments removed).
using ImportStub = std::function<std::optional<StubResult>(ImportHit&, core16::Uc&)>;

// A far-heap allocator that installs real GDT descriptors, so GlobalAlloc gives genuinely distinct far selectors.
class HeapContext {
public:
    virtual ~HeapContext() = default;
    virtual std::uint16_t alloc_heap(std::uint32_t size = 0x10000) = 0;   // a new far selector
};
// probe's `stub`: as ImportStub, with the run's heap.
using HeapStub = std::function<StubResult(ImportHit&, core16::Uc&, HeapContext&)>;

// The heap of one run: each block is a window and the top GDT index not yet taken.
class Heaps : public HeapContext {
public:
    explicit Heaps(core16::Uc& machine) : machine_(machine) {}
    std::uint16_t alloc_heap(std::uint32_t size = 0x10000) override;
    struct Block {
        std::uint32_t base = 0, size = 0;
    };
    const std::map<std::uint16_t, Block>& blocks() const { return blocks_; }   // by selector

private:
    core16::Uc& machine_;
    unsigned made_ = 0;
    std::map<std::uint16_t, Block> blocks_;
};

// What a stand-in does for a system function, as the knowledge base's win16 model says (knowledge/models/win16.json): each of DX,
// AX and the bytes removed is a number, a new heap selector, or the Nth word of the arguments as pushed.
struct FromHeap {};
struct FromArgument {
    std::size_t index = 0;
};
using ModelWord = std::variant<std::int64_t, FromHeap, FromArgument>;
struct StubModel {
    std::optional<ModelWord> ax, dx, pop;
};
// The model of a function by its name as the hit gives it ("KERNEL.GLOBALALLOC"; a KERNEL ordinal "KERNEL.@15" is looked up by its
// number too); absent when it is not modelled.
using ModelLookup = std::function<std::optional<StubModel>(std::string_view api)>;
// The lookup over the knowledge base (`api_model("win16", ...)` and the `kernel_ordinal` table).
ModelLookup models_of(knowledge::Knowledge& knowledge);

// The module's other functions, not run: reaching one is a call recorded and answered at once with a result made from its words,
// so a function is judged on what it does itself whatever its callees' state.
struct Callee {
    std::string name;
    std::optional<std::uint32_t> removes;   // bytes of arguments it removes; absent: not known (the stack after it is not either)
    bool far = false;
};
using Callees = std::map<core16::SegOff, Callee>;

// A setup call run before the function, sharing its machine and import stand-ins; its trace is then discarded.
struct PreludeCall {
    Function function;
    std::vector<std::uint16_t> args;
};

// Memory written before the run: at a linear address, or at an offset of the module's data segment wherever this build has it.
struct DataOffset {
    std::uint32_t offset = 0;
};
struct MemoryInit {
    std::variant<std::uint32_t, DataOffset> where;
    Bytes data;
};

// A call recorded in the running system (a breakpoint at the function's entry), replayed. The recorded stack is the run's stack at
// the same SP (its return address replaced by the tracer's); the selectors the system had name the same bytes here (an LDT of
// them): SS the stack, the module's data selector its data segment, the rest the recorded segments' copies, any other one scratch.
struct RecordedSegment {
    Bytes bytes;
    bool code = false;
};
struct RecordedRegisters {
    std::uint16_t ax = 0, bx = 0, cx = 0, dx = 0, si = 0, di = 0, bp = 0;
};
struct RecordedMemory {            // the whole of memory at the call, and where the LDT was in it
    Bytes memory;
    std::uint32_t ldt_at = 0, ldt_limit = 0;
};
struct Redirect {                  // that function of this side's build, by name
    std::uint32_t segment = 0, offset = 0;
    std::string name;
};
struct RecordedCall {
    Bytes stack;                   // its stack segment's bytes
    std::uint16_t sp = 0, ss = 0;
    std::optional<std::uint16_t> ds, es;
    RecordedRegisters regs;
    std::map<std::uint16_t, RecordedSegment> ldt;           // by selector
    std::optional<std::uint16_t> data_selector;             // the module's own data selector there
    std::optional<RecordedMemory> snapshot;                 // with it every selector the system had names its bytes
    std::map<std::uint16_t, std::uint32_t> own;             // selector -> the module's own code segment: far pointers sent on by name
    std::map<core16::SegOff, Redirect> redirect;            // (recorded segment, offset) -> this side's function of the same name
    std::string key;                                        // the recording's name: where its `retf n` are remembered; empty: not remembered
};

struct OffsetRange {
    std::uint32_t lo = 0, hi = 0;
};
// Past the original's data, the same on both sides: zeros from `image_end` (or the data image's end) to `segment_end` -- what the
// loader gives -- then a pattern to 64K. A build's own data segment is laid out longer, and a read past the end found the build's
// bytes on one side and nothing on the other.
struct DataEnd {
    std::uint32_t image_end = 0, segment_end = 0;
};

// What every run asks.
struct Setup {
    Function function;
    std::vector<std::uint16_t> args;               // Pascal words pushed left to right; empty: `arity` zeros
    std::size_t arity = 0;
    std::int64_t insn_cap = emulator::kMaxInsns;
    std::vector<PreludeCall> prelude;
    bool near_entry = false;                       // a NEAR function: only a return offset is pushed, and returning to it ends the run
    Bytes data_image;                              // written over the start of the data segment before the run (a rebuild gets the original's data)
    Callees callees;
    std::optional<DataEnd> data_end;
    const RecordedCall* context = nullptr;         // a call to replay (the arguments are then not pushed); not owned
    std::vector<MemoryInit> mem_init;
    std::optional<OffsetRange> branches;           // offsets in the function's segment: also log each conditional jump there
    std::optional<std::uint16_t> bp;               // BP as the function is entered (else 0): a helper that works in its caller's frame
    bool ports = false;                            // `in` and `out` reach the UARTs and interrupt controllers (ports16); else they read 0
    native_shadow::Shadow* shadow = nullptr;       // the path explorer's, kept up beside the run; not owned
    bool fill_pages = true;                        // pages mapped on demand are filled with the pattern (false: zeros)
};

struct TraceOptions : Setup {
    ImportStub import_stub;                        // empty: every import answers AX = 0
};
struct ProbeOptions : Setup {
    bool cdecl_args = false;                       // reverse `args` before pushing: INTERNAL functions are often __cdecl16far
    HeapStub stub;                                 // empty: the Runner's default_stub
    std::optional<std::uint32_t> stub_ret;         // what a Windows function the stub does not model returns in DX:AX (else 0)
};

// ---- what a run recorded -----------------------------------------------------------------------------------------------------------

enum class Ending : std::uint8_t {
    Returned,       // reached the return address
    Truncated,      // the instruction cap or the governor stopped it
    BadOrdinal,     // the ordinal is not in the entry table: nothing ran (the record has the module and the ordinal only)
    Faulted,        // Unicorn stopped it, a limit was passed, or an interrupt ended it: `fault_text` says why
};

struct Layout {
    std::vector<std::uint32_t> segment_bases;     // by NE segment number - 1
    std::optional<std::uint32_t> data_base;       // the module's data segment (DGROUP)
    std::uint32_t autodata_segment = 0;
};
// A call into another of the module's functions, recorded and answered.
struct CalleeCall {
    std::string name;
    std::vector<std::uint16_t> args;
    std::uint16_t sp = 0;
    std::int64_t n = 0;
    std::uint16_t ret_ip = 0;
};
// The instruction at the faulting CS:EIP.
struct Fault {
    std::uint32_t cs = 0, eip = 0;
    std::optional<std::string> insn;
    std::string bytes;                            // the first 8 bytes, as hex
};
struct EntryRegisters {                           // as the function is entered
    std::uint16_t ax = 0, bx = 0, cx = 0, dx = 0, si = 0, di = 0, bp = 0, cs = 0, ds = 0, es = 0, ss = 0;
};
struct ExitRegisters {                            // the ones a function must hand back, as it left them
    std::uint16_t si = 0, di = 0, ds = 0;
};

struct Trace16 {
    std::string module;                           // the file's name
    std::optional<std::uint32_t> ordinal;
    std::uint32_t segment = 0, offset = 0;
    std::vector<std::uint16_t> args;              // the words pushed
    Ending ending = Ending::Returned;
    std::string fault_text;                       // Faulted: Unicorn's text, or why
    native_win16::Trace observed;                 // what the hooks saw: path, memory, pages mapped, counters
    std::vector<ImportHit> imports_hit;
    Layout layout;
    std::vector<CalleeCall> calls_out;
    bool unsure = false;                          // how many bytes a callee removes was not known
    std::optional<std::uint8_t> interrupt;        // an interrupt (other than int 21h) that ended the run
    std::optional<std::vector<native_trace::BranchEntry>> branches;   // asked for with Setup::branches
    std::size_t prelude_ran = 0;
    EntryRegisters entry_regs;
    ExitRegisters exit_regs;
    std::uint16_t ax = 0;                         // AX at the end
    std::uint32_t dx_ax = 0;                      // DX:AX
    std::uint16_t sp_delta = 0;                   // how far SP moved: the return address, plus any arguments removed
    std::uint32_t code_base = 0;
    std::size_t n_imports = 0;
    std::optional<Fault> fault;
    std::optional<std::vector<ports16::LogEntry>> io;                  // asked for with Setup::ports
    std::map<std::uint16_t, std::uint32_t> heap_sels;                  // probe: selector -> base of the stand-in heaps made

    // "ret", "truncated", "bad-ordinal" or "exception: <fault_text>": the words the run record carries.
    std::string exit_text() const;
};

// The record as the one run record (emulate/run.hpp): AX and DX:AX the results, every Windows call made, the devices' log, SI DI DS
// as they went in and came out, the linear addresses run in order. The Trace16 is the run's `raw` (a std::shared_ptr<const Trace16>).
Run record(const std::shared_ptr<const Trace16>& trace);

// ---- the runner -----------------------------------------------------------------------------------------------------------------------

struct Environment {
    // The instruction ceiling for one function (the caller resets `n` and sets `ceil`); required, and the caller's alone.
    native_trace::Governor* governor = nullptr;
    // The knowledge base's win16 stand-ins (`models_of`); empty: no call is modelled.
    ModelLookup models;
    // The bytes of arguments a Windows function removes, by the name a hit gives ("USER.@171"); empty: not known.
    std::function<std::optional<std::uint32_t>(std::string_view api)> argument_bytes;
    // A new 32-bit x86 engine; empty: cb/core's Unicorn. A host whose recorder was pointed at another copy of the library makes its
    // engines from that copy.
    std::function<std::unique_ptr<core16::Uc>()> open_engine;
};

class Runner {
public:
    // UsageError when `environment.governor` is missing.
    explicit Runner(Environment environment);
    Runner(const Runner&) = delete;
    Runner& operator=(const Runner&) = delete;
    ~Runner();

    // Run one exported function with `options.import_stub` answering Windows. A fault of the guest is the Trace's `ending`, not an
    // exception; an exception a stub or hook raised, a module that cannot be read (IoError, FormatError) and a function that cannot be
    // found (UsageError) are. A function by an ordinal the entry table does not have is a Trace16 ending BadOrdinal.
    Trace16 trace(const std::filesystem::path& module, const TraceOptions& options = {});

    // Run one function under the controlled Win16 environment: Windows calls answered by `options.stub` (default_stub), the arguments
    // of one the stub does not model removed as the real function would.
    Trace16 probe(const std::filesystem::path& module, const ProbeOptions& options = {});

    // probe() as the one run record (the platform's runner).
    Run recorded(const std::filesystem::path& module, const ProbeOptions& options = {});

    // The stand-in for the Win16 APIs an LZ-class module calls, as the model lookup says; an unmodelled call answers AX = 0.
    StubResult default_stub(const ImportHit& hit, HeapContext& heap) const;

    // The module's data segment as the loader leaves it -- its fix-ups applied (the selectors of far pointers kept in it) -- the bytes
    // a rebuilt module is given in place of its own, so both sides start from the same globals. Empty when it has no data segment.
    Bytes relocated_data(const std::filesystem::path& module);

    struct Impl;   // (the run's own helpers reach into it)

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::windows16
