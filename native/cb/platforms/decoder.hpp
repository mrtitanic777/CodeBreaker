// cb/platforms/decoder.hpp -- the one disassembler: Capstone, for the processors that have one.
//
// A `Decoder` is one Capstone engine for an architecture and word size, with or without the per-instruction detail
// (operands, registers read and written). Making one is a thousand times dearer than decoding with it, and an engine is
// not shared between threads: a `DecoderSet` owns the ones one thread has made and hands each out by kind.
//
//     platforms::DecoderSet decoders;                                    // one per thread (an emulator, a worker)
//     const platforms::Decoder& cs = decoders.get(Arch::X86, 16);
//     for (const platforms::Insn& i : cs.disasm(code, 0x1000)) { ... i.mnemonic, i.op_str ... }
//     cs.each(code, 0x1000, 0, [&](const cs_insn& c) { ...; return true; });   // Capstone's own records, for detail
//
// `disasm` stops at the first byte Capstone cannot decode, as the old generator did. Capstone's records (`cs_insn`) are
// only named here, not defined: a caller that reads them includes cb/core/capstone_api.hpp.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"

struct cs_insn;   // Capstone's instruction record

namespace cb::platforms {

// The architectures Capstone decodes for CodeBreaker.
enum class Arch { X86, Ppc };

// One decoded instruction: what every caller reads.
struct Insn {
    std::uint64_t address = 0;
    std::uint8_t size = 0;
    std::string mnemonic;
    std::string op_str;
    std::array<std::uint8_t, 16> bytes{};   // the first `size` bytes are the instruction's
    ByteView raw() const { return ByteView(bytes.data(), size); }
};

// Registers an instruction reads and writes, as Capstone numbers them.
struct RegisterAccess {
    std::vector<std::uint16_t> read;
    std::vector<std::uint16_t> written;
};

class Decoder {
public:
    // x86: 16, 32 or 64 bits; ppc: 32 or 64 bits, big-endian (the Xbox 360's). Throws UsageError for any other
    // width, NotFoundError when capstone.dll cannot be loaded, Error for a refusal of Capstone's own.
    static Decoder open(Arch arch, unsigned bits, bool detail);

    Decoder(Decoder&&) noexcept;
    Decoder& operator=(Decoder&&) noexcept;
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;
    ~Decoder();

    Arch arch() const { return arch_; }
    unsigned bits() const { return bits_; }
    bool detail() const { return detail_; }

    // The instructions from the start of `code` up to the first byte that does not decode; `count` 0: all of them.
    std::vector<Insn> disasm(ByteView code, std::uint64_t address, std::size_t count = 0) const;

    // The same, as Capstone's own records, for a caller that reads the operands (needs a decoder opened with detail).
    // `visit` returns false to stop. The records live until each() returns.
    void each(ByteView code, std::uint64_t address, std::size_t count, const std::function<bool(const cs_insn&)>& visit) const;

    // Capstone's own handle (csh), for a caller that has to call Capstone itself (cb/core/capstone_api.hpp) on this engine.
    std::uintptr_t native_handle() const { return handle_; }
    // Capstone's name of a register, or nullptr for a number it does not know.
    const char* register_name(std::uint32_t reg) const;
    // The registers an instruction reads and writes; nullopt where Capstone cannot say.
    std::optional<RegisterAccess> register_access(const cs_insn& insn) const;

private:
    Decoder(Arch arch, unsigned bits, bool detail, std::uintptr_t handle) : arch_(arch), bits_(bits), detail_(detail), handle_(handle) {}
    Arch arch_;
    unsigned bits_;
    bool detail_;
    std::uintptr_t handle_ = 0;   // Capstone's csh
};

// The decoders one thread has made. Not thread-safe: each thread (each emulator, each worker) has its own.
class DecoderSet {
public:
    // A decoder of this kind, made on the first ask. The reference stays valid for the life of the set.
    const Decoder& get(Arch arch, unsigned bits, bool detail = false);

private:
    std::map<std::tuple<Arch, unsigned, bool>, std::unique_ptr<Decoder>> held_;   // lookup only; a handful
};

}  // namespace cb::platforms
