// emulate/v810.hpp -- the NEC V810 as an interpreter, to run functions on (small, deterministic, and it logs what a function does to the devices).
//
//     v810::Bus bus(0x07FFFFFF);                                  // the address lines the machine has (the Virtual Boy: 27)
//     bus.memory(0x05000000, 0x05FFFFFF, {}, true, false, 0x10000);   // 64 KB of RAM seen again and again over 16 MB
//     bus.device(0x02000000, 0x02FFFFFF, read, write);            // a device range: every access logged
//     v810::RunResult r = v810::run(bus, entry, {.r6 = 1});       // call the function as `jal` would, run to its `jmp [lp]`
//
// Every instruction of decompile/v810: the flags Z S OV CY of the arithmetic, the shifts, the 64-bit result of mul / mulu and the remainder of div / divu in r30, the loads
// and stores of 8, 16 and 32 bits (a halfword or word access ignores the low address bits), caxi, the branches with all sixteen conditions, jr / jal / jmp, ldsr / stsr on the
// system registers, setf, the single-precision floating-point instructions (a NaN or denormal operand sets the reserved-operand flag and leaves the result alone, a division by
// zero sets the zero-divide flag; the result's sign goes to S and CY), xb xh rev mpyhw, and the ten bit-string instructions on r26-r30 (as the V810 manual describes: r30 and
// r29 are the source and destination word addresses, r28 the length in bits, r27 and r26 the bit offsets in those words; a search leaves Z set when nothing matched).
// Timing is not modelled. trap, reti and halt, a divide by zero and an undefined bit pattern stop a run with that reason. A run starts with the registers it is given and
// lp pointing at an address nothing is at; reaching it is the end.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/v810.hpp"

namespace cb::emulate::v810 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kReturnAddress = 0x04FFFFF0;   // where a called function's `jmp [lp]` lands

// The program status word's bits.
namespace psw {
inline constexpr std::uint32_t Z = 1u << 0, S = 1u << 1, OV = 1u << 2, CY = 1u << 3, FPR = 1u << 4, FUD = 1u << 5, FOV = 1u << 6, FZD = 1u << 7, FIV = 1u << 8, FRO = 1u << 9,
                               ID = 1u << 12, AE = 1u << 13, EP = 1u << 14, NP = 1u << 15;
inline constexpr std::uint32_t kWritable = 0x000FF3FF;
}  // namespace psw

// One access to a device range of the bus.
struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint32_t address = 0;
    std::uint8_t size = 1;   // bytes: 1, 2 or 4
    std::uint32_t value = 0;
};

// An address space: memory ranges (read-only or not, mirrored over their span) and device ranges handed to callbacks. The first range that holds an address answers it; an
// address outside every range reads 0 and ignores writes. Addresses are masked to the machine's address lines first. Reads and writes are little-endian.
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    explicit Bus(std::uint32_t address_mask = 0xFFFFFFFFu) : mask_(address_mask) {}

    // Addresses lo..hi (inclusive, already masked) are memory: `initial` (zero padded to `size`, which is `hi - lo + 1` unless given) repeated over the range. With `logged`,
    // every access is logged; without `writable`, a write is dropped.
    void memory(std::uint32_t lo, std::uint32_t hi, ByteView initial = {}, bool writable = true, bool logged = false, std::size_t size = 0);
    // Addresses lo..hi go to the callbacks, every access logged unless `logged` is false.
    void device(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {}, bool logged = true);

    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);

    // The bytes of the memory range that starts at `lo`.
    std::span<std::uint8_t> bytes(std::uint32_t lo);
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        Bytes data;          // memory; empty for a device
        bool writable;
        bool logged;
        ReadFn read;
        WriteFn write;
    };
    Range* find(std::uint32_t address);
    std::uint32_t mask_;
    std::vector<Range> ranges_;
    std::vector<DeviceAccess> log_;
};

struct Registers {
    std::array<std::uint32_t, 32> r{};
    std::uint32_t pc = 0;
    std::uint32_t psw = 0;
    std::uint32_t eipc = 0, eipsw = 0, fepc = 0, fepsw = 0, ecr = 0, chcw = 0, adtre = 0;
};

// Why the CPU cannot go on.
struct Fault {
    std::uint32_t pc = 0;
    std::string reason;   // "undefined instruction at 07001234" | "trap 5 at ..." | "divide by zero at ..."
    std::string text() const { return reason; }
};

class Cpu {
public:
    explicit Cpu(Bus& bus) : bus_(&bus) {}

    Registers regs;
    std::uint64_t steps = 0;

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not (nothing changed).
    std::optional<Fault> step();

private:
    Bus* bus_;
};

struct RunOptions {
    std::array<std::uint32_t, 32> r{};     // the starting registers (r0 is ignored, lp is set to the return address)
    std::uint32_t sp = 0x0500FFF0;
    std::uint64_t max_steps = kMaxSteps;
    // Called with the registers before each instruction runs.
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;                        // as the run left them (pc: where it stopped)
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;          // the device accesses of this run, in order
    std::string exit() const;              // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as jal would and run it until it returns (jmp [lp] to kReturnAddress), faults or takes `max_steps` instructions.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::v810
