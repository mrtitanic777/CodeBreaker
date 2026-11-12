// cb/emulate/m68k.hpp -- a 68000, to run functions on (Unicorn's 68000 is not what a judge can compare: this one is
// small, deterministic, and logs what a function does to the machine's devices).
//
//     m68k::Bus bus;                                   // 16 MB, big-endian; map(lo, hi, read, write) hands ranges to callbacks
//     m68k::Cpu cpu(bus);
//     cpu.step();                                      // one instruction
//     m68k::RunResult r = m68k::run(bus, entry, {.d = {...}});   // call a function as JSR would: run to its RTS
//     cb::emulate::Run record = m68k::record(r);       // the one run record the judge compares
//
// Every instruction cb/decompile/m68k decodes, read from it (each decoded instruction is cached by address, and is not
// decoded again if the code under it changes: a function is run as it was loaded). Flags as the 68000 sets them -- X N Z V
// C, with X following C where the manual says, ADDX/SUBX/NEGX/ABCD/SBCD/NBCD leaving Z set only if it was and the
// result is zero, DIVU/DIVS overflow leaving the destination alone and setting V -- and addresses 24 bits wide. A byte
// pushed or popped through A7 moves it by two. Exceptions (TRAP, CHK, a division by zero, ILLEGAL, line A/F) end the run
// with the reason: a function is run on its own, without the program's handlers.
//
// What a function touches outside its own memory is the machine's business: a Bus hands chosen ranges (the Genesis's
// VDP, its I/O and Z80 areas) to callbacks and logs every access to them in order -- the effects the judge compares.
// A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/decompile/m68k.hpp"
#include "cb/emulate/run.hpp"

namespace cb::emulate::m68k {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kMemorySize = 0x1000000;       // the whole 24-bit address space
inline constexpr std::uint32_t kAddressMask = 0x00FFFFFF;
// The return address run() pushes (even, outside a cartridge's code): reaching it ends the run.
inline constexpr std::uint32_t kReturnAddress = 0x00FFFFF0;

// The bits of the status register that run() and the flag instructions use.
namespace flag {
inline constexpr std::uint16_t X = 0x10, N = 0x08, Z = 0x04, V = 0x02, C = 0x01;
}

// One access to a device range: a read or a write of 1, 2 or 4 bytes at a 24-bit address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 1;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string text() const;   // the kind as a run record names it: "r1" "r2" "r4" "w1" "w2" "w4"
};

// Memory, big-endian, 24-bit addresses, with ranges handed to callbacks. The first range that holds an address answers
// it; every access to a range is logged. A read past the end of the memory gives 0 and a write is dropped (a Bus smaller
// than 16 MB is a machine with less RAM). Memory the Bus owns starts as zeros (or the image given); Bus::over() uses the
// caller's memory in place, which must outlive the Bus.
class Bus {
public:
    // A device read answers with a value of the access's size (the Bus keeps the low `size` bytes of what it returns).
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    explicit Bus(std::size_t size = kMemorySize);
    // A copy of `image` at address 0 (UsageError when it does not fit in `size` bytes), the rest zeros.
    explicit Bus(ByteView image, std::size_t size = kMemorySize);
    static Bus over(ByteSpan ram);
    Bus(Bus&&) noexcept = default;
    Bus& operator=(Bus&&) noexcept = default;

    // Addresses lo..hi (inclusive) go to the callbacks: a range with no read callback reads 0, with no write callback
    // ignores the write.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});

    // `size` is 1, 2 or 4.
    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);

    ByteSpan memory() { return ram_; }
    ByteView memory() const { return ram_; }
    const std::vector<BusAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    Bytes owned_;
    ByteSpan ram_;
    std::vector<Range> ranges_;
    std::vector<BusAccess> log_;
};

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,    // the bytes at pc are not an instruction
        NotRun,           // an instruction this interpreter does not run (`mnemonic`)
        DivideByZero,
        Check,            // CHK out of bounds
        TrapOverflow,     // TRAPV with V set
        Trap,             // TRAP #n
        Illegal,          // ILLEGAL
        Stop,             // STOP
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::string mnemonic;        // NotRun: which
    // "no instruction at 0014FE" | "not run: MOVEC" | "division by zero" | "CHK exception" | "TRAPV exception" | "TRAP" |
    // "ILLEGAL" | "STOP"
    std::string text() const;
};

struct DecodedCache;   // the instructions decoded so far, with what the interpreter works out from each once (m68k.cpp)

class Cpu {
public:
    explicit Cpu(Bus& bus);
    ~Cpu();
    Cpu(const Cpu&) = delete;
    Cpu& operator=(const Cpu&) = delete;

    std::array<std::uint32_t, 8> d{}, a{};   // D0-D7, A0-A7 (A7 is the stack pointer)
    std::uint32_t pc = 0;
    std::uint16_t sr = 0x2700;               // T S . . . I2 I1 I0 . . . X N Z V C
    std::uint32_t usp = 0;                   // the user stack pointer, MOVE USP's
    std::uint64_t steps = 0;                 // instructions executed

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not finish (pc is already past it, as the CPU
    // leaves it, and the step is not counted).
    std::optional<Fault> step();

    Bus& bus() { return *bus_; }

private:
    Bus* bus_;
    std::unique_ptr<DecodedCache> cache_;
};

// The registers run() starts from: D0-D7 and A0-A7 (A7 is then replaced by `sp`). Absent: all zero.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::optional<std::array<std::uint32_t, 8>> d, a;
    std::uint32_t sp = 0x00FFFF00;
    std::uint16_t sr = 0x2700;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                  // why it stopped
    std::array<std::uint32_t, 8> d{}, a{};       // as the run left them
    std::uint16_t sr = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                   // the device accesses of this run, in order
    std::string exit() const;                    // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JSR would and run it until it returns (RTS to kReturnAddress), stops, or takes
// `max_steps` instructions. What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

// The run as the one run record: D0-D7 A0-A7 SR as results, the bus's accesses as io, big-endian, truncated when it hit
// the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner (platforms/genesis).
cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::m68k
