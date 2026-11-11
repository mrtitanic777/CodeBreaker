// cb/emulate/powerpc.hpp -- run a PowerPC function and record what it did.
//
//     powerpc::RunResult r = powerpc::run(regions, entry, args, options);
//     r.exit_text()   "ret" | "exception:<why>" | "none" (the step limit was reached)
//     r.ret           r3 on return
//     r.calls         the calls that left the code given, in order, each with r3..r10
//     r.writes        the bytes written outside the stack, with the byte before and after
//     r.path          the addresses inside `options.span` that ran
//     r.saved         whether r14-r31 and the stack pointer came back as they went in
//     cb::emulate::Run record = powerpc::record(r);       // the one run record the judge compares
//
// The emulator library used for x86 cannot run 64-bit PowerPC code (its one selectable 64-bit processor model faults on
// the first instruction fetch), so this is an interpreter of our own: the 64-bit PowerPC user instruction set with
// integer, branch, condition-register and floating-point instructions, as compilers for 32-bit-address PowerPC targets
// use it -- 64-bit registers and arithmetic, effective addresses cut to 32 bits, big-endian memory.
//
// What it does not do, and says so: vector instructions (the run ends "exception:unsupported"), overflow recording
// (XER[OV] is never set), floating-point status and rounding modes other than round-to-nearest, and anything privileged.
//
// Memory is pages created on first touch (zero-filled), with a limit on how many, so code that reaches into memory it was
// not given keeps running the same way on both sides of a comparison.
//
// A Machine and a run belong to one thread. A PageImage is immutable and may be shared by the runs of every thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/emulate/run.hpp"

namespace cb::emulate::powerpc {

inline constexpr std::uint32_t kReturnAddress = 0x7FFF0000;   // where the function returns to
inline constexpr std::uint32_t kStackTop = 0x70100000;        // stack: the megabyte below this
inline constexpr std::uint32_t kStackLow = 0x70000000;
inline constexpr std::uint32_t kArgsBase = 0x60000000;        // memory pointer-shaped inputs can point into
inline constexpr std::uint32_t kPageSize = 0x1000;
inline constexpr std::size_t kMaxPages = 2048;
inline constexpr std::uint64_t kMaxSteps = 20000;
inline constexpr std::size_t kMaxCalls = 400;
inline constexpr std::uint32_t kEntryStackPointer = kStackTop - 0x4000;   // r1 at entry
inline constexpr std::uint64_t kSavedRegisterBase = 0xA5A50000;           // r14..r31 hold kSavedRegisterBase + n at entry

// Why the guest cannot go on.
enum class StopReason : std::uint8_t {
    Alignment,     // an instruction fetch at an address that is not a multiple of 4
    Invalid,       // an instruction word of zeros (nothing there)
    Memory,        // more than kMaxPages pages touched
    Syscall,       // sc
    Trap,          // a tw/td/twi/tdi whose test holds
    Unsupported,   // an instruction this interpreter does not run (vector, privileged, reserved)
};
std::string_view to_text(StopReason reason);   // "alignment", "invalid", "memory", "syscall", "trap", "unsupported"

// The guest stopped: thrown by Machine and step(); run() catches it and ends the run with the reason.
class Fault : public EmulationError {
public:
    explicit Fault(StopReason reason);
    StopReason reason() const { return reason_; }
    CB_ERROR_BOILERPLATE(Fault)

private:
    StopReason reason_;
};

// ---- arithmetic helpers, as the instruction semantics name them --------------------------------------------------

// Bits mb..me set, bit 0 the most significant of 64 (wrapping when mb > me); mb, me in 0..63.
std::uint64_t mask(unsigned mb, unsigned me);
std::uint64_t rotl64(std::uint64_t v, unsigned n);   // n taken mod 64
std::uint64_t rotl32(std::uint64_t v, unsigned n);   // the low word rotated, n mod 32, the result in both halves (as the hardware does)
std::uint64_t sign_extend(std::uint64_t v, unsigned bits);   // from `bits` wide to 64
double to_double(std::uint64_t bits);
std::uint64_t to_bits(double x);
double round_to_single(double x);                    // rounded to single precision (still held as a double); overflow gives infinity

// ---- memory ------------------------------------------------------------------------------------------------------

using Page = std::array<std::uint8_t, kPageSize>;

// (address, bytes) to place in memory.
struct Region {
    std::uint32_t address = 0;
    Bytes data;
};

// An image prepared once as pages and shared by every run of it (a run copies a page only when it writes to it).
class PageImage {
public:
    PageImage() = default;
    // The pages of the regions (a region may start anywhere; addresses wrap at 4 GB).
    static PageImage of(std::span<const Region> regions);

    const Page* find(std::uint32_t page_number) const;   // page_number = address >> 12
    std::size_t size() const { return pages_ ? pages_->size() : 0; }
    bool empty() const { return size() == 0; }
    template <class F>
    void for_each(F&& f) const {   // (page number, page), in no particular order
        if (pages_) {
            for (const auto& [key, page] : *pages_) f(key, page);
        }
    }

private:
    std::shared_ptr<const std::unordered_map<std::uint32_t, Page>> pages_;
};

// What the memory behind pointer-shaped inputs holds (ARGS_BASE upward), so that what a function reads through a pointer
// is not always zero:
//   Pointers   every word holds the address 0x100 past itself: any chain of pointers leads on
//   Counts     words 1, 2, 3, ...
//   Text       a run of letters ending in a zero every 24 bytes
enum class Fill : std::uint8_t { None, Pointers, Counts, Text };
std::optional<Fill> fill_from_text(std::string_view name);   // "pointers" | "counts" | "text"
Bytes filled(Fill kind, std::size_t size = 0x10000);          // None: nothing
const PageImage& fill_image(Fill kind);                       // filled(kind) as pages at kArgsBase, built once

// One byte of a function's output memory: the byte before the run and after it.
struct ByteWrite {
    std::uint32_t address = 0;
    std::uint8_t before = 0, after = 0;
};

class Machine {
public:
    // The regions are loaded over `shared`: a region's page is made from the shared one.
    explicit Machine(std::span<const Region> regions = {}, PageImage shared = {});

    // Memory read but not written falls back to `under` after `shared` (the fill: a page of the regions was made without it).
    void set_under(PageImage under);

    std::array<std::uint64_t, 32> r{};
    std::array<std::uint64_t, 32> f{};   // floating registers, as 64 raw bits
    std::uint32_t lr = 0;
    std::uint64_t ctr = 0;
    std::uint32_t cr = 0;
    bool ca = false;                     // XER[CA]
    std::uint32_t pc = 0;
    std::optional<std::uint32_t> reserve;   // the address lwarx/ldarx reserved

    void load(std::uint32_t va, ByteView data);
    // Big-endian, 1..8 bytes. A page that was never touched is created (zero-filled): Fault{Memory} past kMaxPages.
    std::uint64_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint64_t value);

    // The bytes written outside the stack, by first write: (address -> byte before the first write, byte after the last).
    const OrderedMap<std::uint32_t, std::pair<std::uint8_t, std::uint8_t>>& writes() const { return writes_; }
    // The pages this run has written or created (not the shared ones it only read).
    template <class F>
    void for_each_page(F&& f) const {   // (page number, page), in no particular order
        for (const auto& [key, page] : pages_) f(key, *page);
    }
    std::size_t page_count() const { return pages_.size(); }
    // The page holding `address` as the run sees it (written, shared or the fill), or nullptr: nothing is created.
    const Page* peek(std::uint32_t address) const;
    const Page* find_written(std::uint32_t page_number) const;     // a page this run has written or created
    const Page* find_unwritten(std::uint32_t page_number) const;   // a shared or fill page it has only read so far

    // -- condition register
    void set_cr(unsigned field, unsigned bits);
    bool cr_bit(unsigned i) const { return (cr >> (31 - i)) & 1; }

private:
    Page& page_for_write(std::uint32_t key);
    const Page& page_for_read(std::uint32_t key);

    std::unordered_map<std::uint32_t, std::unique_ptr<Page>> pages_;
    PageImage shared_, under_;
    OrderedMap<std::uint32_t, std::pair<std::uint8_t, std::uint8_t>> writes_;
    // the last page found, to skip the lookup (written pages: rw; shared or fill: ro)
    std::uint32_t rw_key_ = ~0u, ro_key_ = ~0u;
    Page* rw_page_ = nullptr;
    const Page* ro_page_ = nullptr;
};

// Execute one instruction word at m.pc and set m.pc for the next. Throws Fault.
void step(Machine& m, std::uint32_t word);

// ---- running a function ------------------------------------------------------------------------------------------

// What the last compare into a condition field compared.
struct Compare {
    unsigned size = 0;                   // 4 or 8 bytes
    std::uint64_t a = 0, b = 0;
    std::optional<std::uint32_t> a_from, b_from;   // where a / b was loaded from
};
struct BranchEvent {
    std::uint32_t address = 0;
    bool taken = false;
    std::optional<Compare> compare;      // none: the field the branch tests was last set some other way
};

// emulate/shadow.py's ShadowPPC, kept up beside the run (the path explorer's): stepped before each instruction, told of
// each call out. The run is the same run with it or without.
class Shadow {
public:
    virtual ~Shadow() = default;
    virtual void step(Machine& m, std::uint32_t pc) = 0;
    virtual void after_call(Machine& m, bool known, std::uint64_t ret) = 0;
};

using ExternTable = std::unordered_map<std::uint32_t, std::string>;
struct AddressRange {
    std::uint32_t lo = 0, hi = 0;        // [lo, hi)
    bool contains(std::uint32_t a) const { return lo <= a && a < hi; }
};

struct RunOptions {
    // Functions outside what was given (address -> name): a call to one is recorded with its eight argument registers and
    // returns `stub_ret`. Must outlive the call.
    const ExternTable* externs = nullptr;
    std::optional<AddressRange> span;    // the addresses whose execution is recorded (the path, the branch log)
    std::uint64_t stub_ret = 0;
    std::uint64_t max_steps = kMaxSteps;
    std::vector<double> fargs;           // floats, in f1.. (at most 13)
    PageImage shared;                    // memory prepared with PageImage::of
    Fill fill = Fill::None;
    bool branches = false;               // log each conditional branch inside `span`
    // Where code the image calls may be missing: a call that lands on nothing there is a call out, recorded as
    // "@<address>" (a partial image calls into the rest of its title).
    std::optional<AddressRange> absent;
    Shadow* shadow = nullptr;
};

struct Call {
    std::string name;
    std::array<std::uint64_t, 8> args{};   // r3..r10
};

enum class Exit : std::uint8_t { None, Ret, Exception };

struct RunResult {
    Exit exit = Exit::None;
    std::optional<StopReason> stopped;     // the reason, when exit is Exception
    bool truncated = false;                // the step or call limit was reached
    std::vector<Call> calls;
    std::uint64_t ret = 0;                 // r3
    std::uint64_t fret = 0;                // f1, its raw bits
    std::uint64_t steps = 0;
    std::vector<std::uint32_t> path;       // addresses of `span` that ran, in order of first arrival
    std::uint32_t pc = 0;
    std::vector<ByteWrite> writes;         // outside the stack, only where the byte changed
    bool saved = false;
    std::shared_ptr<Machine> machine;
    std::optional<std::vector<BranchEvent>> branches;   // present when options.branches

    std::string exit_text() const;         // "ret" | "exception:<why>" | "none"
};

// Run the function at `entry`. `regions` are placed in memory; `args` go in r3..r10 (at most 8). With `options.branches`
// the conditional branches inside the span are logged (at most 4096). The step limit counts instructions run, not calls
// answered by `externs`.
RunResult run(std::span<const Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args = {},
              const RunOptions& options = {});

// The run as the one run record: r3 and f1 (raw bits) the results, each call out with its eight argument registers, the
// bytes it left changed (outside the stack), r14-r31 and the stack pointer as they went in and came out, the addresses of
// `span` that ran. The RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner (platforms/xbox360).
cb::emulate::Run recorded(std::span<const Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args = {},
                          const RunOptions& options = {});

}  // namespace cb::emulate::powerpc
