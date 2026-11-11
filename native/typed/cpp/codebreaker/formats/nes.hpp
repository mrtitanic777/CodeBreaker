// codebreaker/formats/nes.hpp -- compat shim over cb/formats/nes.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/nes.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet (decompile, platforms, source, verify/lanes) and is deleted with the last of them.
// Conversion and forwarding only. The one thing it supplies that the new module does not know is the 6502 decoder
// that `functions()` follows the code with (the decompiler's, still an old port).
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/nes.hpp"
#include "codebreaker/decompile/m6502.hpp"

namespace codebreaker::formats::nes {

inline const py::bytes MAGIC = py::bytes("NES\x1a");
inline const py::dict<int64_t, py::str> MIRRORING = {{0, py::str("horizontal")}, {1, py::str("vertical")}};

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes header;
    bool nes2 = false;
    int64_t mapper = 0;
    int64_t submapper = 0;
    bool four_screen = false;
    py::str mirroring;
    bool battery = false;
    py::bytes trainer;
    py::bytes prg;
    py::bytes chr;
    py::bytes extra;                 // (anything after: some dumps carry a title)
    py::list<py::bytes> banks;
    int64_t code_at = 0;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // Where in the PRG the byte the CPU reads at `address` is: an offset, or nullopt.
    std::optional<int64_t> mapped(int64_t address) const;
};

bool is_nes(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// The CPU's 64 KB at power-on: PRG mapped (see the module's notes), the rest zero.
py::bytearray _memory(const ROM& rom);

// Where in the PRG the byte the CPU reads at `address` is: an offset, or None.
std::optional<int64_t> _mapped(const ROM& rom, int64_t address);

// Function starts in the fixed address space: the three vectors, then every JSR target
// reached by following the code from them (fall through, branches, JMP, JSR).
py::list<int64_t> functions(const ROM& rom);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

// The new Rom the old ROM holds (the old class is the record the translated Python sees; the logic lives in cb).
inline cb::formats::nes::Rom to_cb(const ROM& r) {
    cb::formats::nes::Rom out;
    out.header = cb::compat::from_py(r.header);
    out.nes2 = r.nes2;
    out.mapper = cb::compat::int_from_py<std::uint16_t>(r.mapper);
    out.submapper = cb::compat::int_from_py<std::uint8_t>(r.submapper);
    out.mirroring = r.four_screen ? cb::formats::nes::Mirroring::FourScreen
                    : r.mirroring == py::str("vertical") ? cb::formats::nes::Mirroring::Vertical
                                                         : cb::formats::nes::Mirroring::Horizontal;
    out.battery = r.battery;
    out.trainer = cb::compat::from_py(r.trainer);
    out.prg = cb::compat::from_py(r.prg);
    out.chr = cb::compat::from_py(r.chr);
    out.extra = cb::compat::from_py(r.extra);
    out.code_at = cb::compat::int_from_py<std::uint32_t>(r.code_at);
    out.vectors = {cb::compat::int_from_py<std::uint16_t>(r.vectors.at(py::str("nmi"))),
                   cb::compat::int_from_py<std::uint16_t>(r.vectors.at(py::str("reset"))),
                   cb::compat::int_from_py<std::uint16_t>(r.vectors.at(py::str("irq")))};
    return out;
}

inline py::ref<ROM> from_cb(const cb::formats::nes::Rom& r, const py::str& path) {
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->header = cb::compat::to_py(cb::ByteView(r.header));
    rom->nes2 = r.nes2;
    rom->mapper = r.mapper;
    rom->submapper = r.submapper;
    rom->four_screen = r.mirroring == cb::formats::nes::Mirroring::FourScreen;
    rom->mirroring = cb::compat::to_py(cb::formats::nes::to_string(r.mirroring));
    rom->battery = r.battery;
    rom->trainer = cb::compat::to_py(cb::ByteView(r.trainer));
    rom->prg = cb::compat::to_py(cb::ByteView(r.prg));
    rom->chr = cb::compat::to_py(cb::ByteView(r.chr));
    rom->extra = cb::compat::to_py(cb::ByteView(r.extra));
    for (std::size_t i = 0; i < r.bank_count(); ++i) rom->banks.append(cb::compat::to_py(r.bank(i)));
    rom->code_at = r.code_at;
    rom->vectors.set(py::str("nmi"), r.vectors.nmi);
    rom->vectors.set(py::str("reset"), r.vectors.reset);
    rom->vectors.set(py::str("irq"), r.vectors.irq);
    return rom;
}

// The old 6502 decoder, in the form cb::formats::code_scan wants. `memory` is the power-on image the old decoder
// reads (converted once per call of functions()).
inline cb::formats::Decoder decoder(const py::bytes& memory) {
    return [&memory](cb::ByteView, std::uint32_t pc) -> std::optional<cb::formats::DecodedInsn> {
        const std::optional<decompile::m6502::Insn> insn = decompile::m6502::decode(memory, pc, pc);
        if (!insn) return std::nullopt;
        cb::formats::DecodedInsn out;
        out.size = static_cast<std::uint32_t>(insn->size);
        if (insn->target) out.target = static_cast<std::uint32_t>(*insn->target);
        if (insn->mnemonic == py::str("JSR")) out.flow = cb::formats::Flow::Call;
        else if (insn->mnemonic == py::str("JMP")) out.flow = out.target ? cb::formats::Flow::Jump : cb::formats::Flow::Indirect;
        else if (insn->mnemonic == py::str("RTS") || insn->mnemonic == py::str("RTI")) out.flow = cb::formats::Flow::Return;
        else if (insn->mnemonic == py::str("BRK")) out.flow = cb::formats::Flow::Halt;
        else out.flow = out.target ? cb::formats::Flow::Branch : cb::formats::Flow::Next;
        return out;
    };
}

}  // namespace shim_

inline py::bytearray ROM::memory() const { return _memory(*this); }
inline py::list<int64_t> ROM::functions() const { return nes::functions(*this); }
inline std::optional<int64_t> ROM::mapped(int64_t address) const { return _mapped(*this, address); }

inline bool is_nes(const py::str& path) { return cb::formats::nes::probe(cb::fs::from_text(cb::compat::from_py(path))); }

inline py::ref<ROM> read(const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(cb::formats::nes::read(cb::fs::from_text(cb::compat::from_py(path))), path);
    });
}

inline py::ref<ROM> parse(const py::bytes& data, const py::str& path) {
    return cb::compat::guard(
        [&] { return shim_::from_cb(cb::formats::nes::parse(cb::compat::from_py(data)), path); });
}

inline py::bytearray _memory(const ROM& rom) {
    return py::bytearray(cb::compat::to_py(cb::ByteView(shim_::to_cb(rom).memory())));
}

inline std::optional<int64_t> _mapped(const ROM& rom, int64_t address) {
    if (address < 0) return std::nullopt;   // (below $8000, as the old test said)
    return cb::compat::guard([&]() -> std::optional<int64_t> {
        const auto at = shim_::to_cb(rom).mapped(cb::compat::int_from_py<std::uint32_t>(address));
        if (!at) return std::nullopt;
        return static_cast<int64_t>(*at);
    });
}

inline py::list<int64_t> functions(const ROM& rom) {
    const py::bytes memory = cb::compat::to_py(cb::ByteView(shim_::to_cb(rom).memory()));
    return cb::compat::to_py(shim_::to_cb(rom).functions(shim_::decoder(memory)));
}

}  // namespace codebreaker::formats::nes
