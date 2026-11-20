// decompile/engine/sleigh_reader.hh -- the decompiler core's SLEIGH translator as CodeBreaker's one instruction reader.
//
// A processor is a SLEIGH description (languages/<cpu>.sla, compiled from languages/src/<cpu>/*.slaspec by sleigh.exe); the same
// description the decompiler lifts to p-code reads the instructions here. Nothing of the core shows through this header.
//
//     sleigh::Reader z80(languages / "z80.sla", {});
//     for (const sleigh::Instruction& i : z80.read(bytes.data(), bytes.size(), 0x0000, 16)) ...    // "LD A,0x12"
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace cb::decompile::sleigh {

// How control leaves an instruction, as its p-code says: the last branch, call or return it holds. A branch inside the instruction
// (to a p-code label: a conditional RET's test, a REP's loop) is not a way out of it, so a conditional return falls through (Next).
enum class Flow {
    Next,      // falls through
    Branch,    // may go to `target` or fall through
    Jump,      // goes to `target` only
    Call,      // calls `target` (or through a register: CallIndirect), then falls through
    CallIndirect,
    Return,
    Indirect,  // goes through a register or memory cell
};

struct Instruction {
    std::uint64_t address = 0;
    std::size_t length = 0;     // 0: the bytes there are no instruction of the processor's
    std::string mnemonic;
    std::string operands;
    Flow flow = Flow::Next;
    std::optional<std::uint64_t> target;   // where a branch, jump or call goes, when it is a constant address
};

class Reader {
public:
    // `sla`: a compiled description; `context`: the description's context variables to start from (x86's "addrsize", ARM's
    // "TMode" ...). Error (std::runtime_error) for a file that is not a description.
    Reader(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context);
    ~Reader();
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;

    // `count` instructions (0: all) of the `size` bytes at `code`, the first at `base`. A place that does not decode is one
    // Instruction of length 0; the caller decides how far to step over it.
    std::vector<Instruction> read(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::size_t count) const;
    // The one instruction at `address` of the `size` bytes at `code` (the first at `base`), as read() reads it. Reading again in
    // the same bytes keeps the translator's parses (a decoder following code calls this for every instruction of one image).
    Instruction at(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::uint64_t address) const;
    // The smallest step the processor's instructions take (its alignment), in bytes.
    std::size_t alignment() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::decompile::sleigh
