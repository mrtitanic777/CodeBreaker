// cb/exact/x86.hpp -- 16-bit code disassembled, as the period's compilers wrote it.
//
// Microsoft's languages (C 3.00-5.x, Pascal, FORTRAN) compiled floating point for the 8087 emulator (-FPi): each ESC
// instruction an interrupt the emulator takes - INT 34h-3Bh stand for the ESC opcodes D8h-DFh (the next byte is the
// instruction's modrm), INT 3Ch for an ESC with a segment override (the next byte the segment and the opcode), INT 3Dh
// for FWAIT. A plain disassembler reads the bytes after the interrupt as instructions of their own and loses its way;
// here they are read as what they stand for.
//
// The decoder underneath is Capstone (16-bit x86, Intel syntax). `Decoder16` is the seam: the default is the
// per-thread Capstone one; a test or another front end can supply its own.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::x86 {

struct Insn {
    std::uint32_t address = 0;
    Bytes bytes;
    std::string mnemonic;
    std::string op_str;
    std::size_t size() const { return bytes.size(); }
    friend bool operator==(const Insn&, const Insn&) = default;
};

// A decoder of 16-bit x86 instructions.
class Decoder16 {
public:
    virtual ~Decoder16() = default;
    // The first instruction `code` decodes to (as if it were at `address`), or nullopt when it decodes to none.
    virtual std::optional<Insn> first(ByteView code, std::uint32_t address) const = 0;
    // Every instruction `code` decodes to, in order, up to the first byte that decodes to none. The default
    // repeats `first`; the Capstone decoder does it in one call.
    virtual std::vector<Insn> all(ByteView code, std::uint32_t address) const;
};

// The 16-bit Capstone decoder of the calling thread (making one costs a thousand decodes, so each thread keeps its
// own). Throws NotFoundError if capstone.dll cannot be loaded.
const Decoder16& capstone_decoder();

// The instructions of img[at, end), the 8087 emulator's interrupts read as the instructions they stand for
// (`emulator`: false reads them as the interrupts they are). A byte that decodes to nothing is `db 0xNN`. `end` is
// clamped to the image. The emulator's instructions are named f<mnemonic>; an ESC with a segment override is
// printed `seg: operand`.
std::vector<Insn> disasm(ByteView img, std::uint32_t at, std::uint32_t end, bool emulator = true,
                         const Decoder16& decoder = capstone_decoder());

// {table address: its length in bytes} - the jump tables of a switch kept in the code (jmp cs:[bx+T]): the words at T,
// as long as each is a place in [lo, hi). `base` is the image offset the code's own address 0 stands at.
std::map<std::uint32_t, std::uint32_t> tables(std::span<const Insn> insns, ByteView img, std::uint32_t lo,
                                              std::uint32_t hi, std::uint32_t base = 0);

// The instructions of a function's code [lo, hi), its switch tables stepped over (they are data).
std::vector<Insn> body(ByteView img, std::uint32_t lo, std::uint32_t hi, bool emulator = true, std::uint32_t base = 0,
                       const Decoder16& decoder = capstone_decoder());

}  // namespace cb::exact::x86
