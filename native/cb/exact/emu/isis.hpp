// cb/exact/emu/isis.hpp -- run an Intel ISIS-II program (8080 code): enough of the 8080 and of ISIS-II for the Intel
// development tools of 1978-82 - PL/M-86, LINK86, LOC86, LIB86, OH86.
//
// The tools are data here: 8080 object files interpreted by this module, nothing of them runs on the host. Drives
// :F0: .. :F9: are host directories (file names upper case, as ISIS has them).
//
// ISIS calls (CALL 40h, C = function, DE -> parameter block): OPEN CLOSE DELETE READ WRITE SEEK LOAD RENAME CONSOL
// EXIT ATTRIB RESCAN ERROR WHOCON SPATH. Monitor entries: CI CO MEMCK; the rest of the monitor's range returns at
// once. The 8080 is the whole instruction set including the undocumented duplicates of NOP, JMP, CALL and RET.
//
// A program's memory is the 8080's 64 KiB, and its address arithmetic wraps there: a parameter block at the very top
// reads on from address 0, and a write that would go past the top is cut off at it.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::exact::emu::isis {

// The host directory of each drive :F0: .. :F9:. Drive 0 is required: it is where a name without a drive goes, and
// where a drive that is not given falls back to.
using Drives = std::map<int, std::filesystem::path>;

struct Options {
    std::uint64_t limit = 2'000'000'000;           // instructions after which the run stops (an ISIS call and a monitor entry count one)
    std::function<void(std::string_view)> echo;    // sees what the program writes to the console, as it is written
};

struct Result {
    std::string console;         // what the program wrote to the console (the bytes as Latin-1 text)
    bool finished = false;       // it ended (the EXIT call or HLT) within the limit
    std::uint64_t steps = 0;     // instructions run
};

// The 8080's registers and flags.
struct Cpu {
    std::uint8_t b = 0, c = 0, d = 0, e = 0, h = 0, l = 0, a = 0;
    std::uint16_t sp = 0, pc = 0;
    bool zero = false, sign = false, parity = false, carry = false, aux_carry = false;
    friend bool operator==(const Cpu&, const Cpu&) = default;
};

// One program's machine: memory, the 8080, the console and the drives. Use `run` for the common case; the machine is
// public for tests and tools that load more than one object file or look at the memory afterwards.
class Machine {
  public:
    // `tail` is the command line after the program's name (Latin-1 text; what a program reads with CONSOL/RESCAN is
    // "PROG" + tail).
    Machine(Drives drives, std::string_view tail, Options options = {});
    Machine(const Machine&) = delete;
    Machine& operator=(const Machine&) = delete;

    // Load an ISIS object file into memory at `bias`; the entry point it names (0 if it names none). FormatError if a
    // record is cut short.
    std::uint32_t load(const std::filesystem::path& object, std::uint32_t bias = 0);
    // Load a program and set the program counter to its entry point.
    void start(const std::filesystem::path& object);

    // Run until the program ends or `limit` instructions have been run. Returns the instructions run.
    std::uint64_t run(std::uint64_t limit);
    bool finished() const { return finished_; }

    const Cpu& cpu() const { return cpu_; }
    ByteView memory() const { return memory_; }
    // What the program wrote to the console, as bytes.
    ByteView console() const { return console_; }

  private:
    // A file a program has open (or a device: its writes are dropped).
    struct OpenFile {
        std::optional<std::filesystem::path> host;   // absent for a device
        std::uint64_t position = 0;
        std::optional<std::string> device;
        Bytes data;                                  // the file's whole contents, edited in memory
        bool dirty = false;                          // to be written back when closed
    };

    // The instruction at the program counter. HLT ends the program.
    void step();
    // CALL 40h: the ISIS call the program has asked for. True when control has gone to a program just loaded.
    bool isis_call();
    void monitor_entry();

    Drives drives_;
    Options options_;
    std::array<std::uint8_t, 65536> memory_{};
    Cpu cpu_;
    Bytes console_;
    Bytes console_input_;
    std::size_t input_position_ = 4;   // RESCAN goes back to the program's name
    std::map<int, OpenFile> files_;    // by the number ISIS gave (2..98); iteration order is the number
    bool finished_ = false;
};

// Run the object file `program` with `tail` as the rest of its command line.
Result run(const std::filesystem::path& program, std::string_view tail, const Drives& drives, const Options& options = {});

}  // namespace cb::exact::emu::isis
