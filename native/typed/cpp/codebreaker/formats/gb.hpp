// codebreaker/formats/gb.hpp -- compat shim over cb/formats/gb.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/gb.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet and is deleted with the last of them. Conversion and forwarding only; the SM83
// decoder `functions()` follows the code with is the decompiler's (still an old port) and is adapted here.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/gb.hpp"
#include "codebreaker/decompile/sm83.hpp"

namespace codebreaker::formats::gb {

constexpr int64_t BANK = 0x4000;
inline const std::tuple<py::str, py::str> EXTENSIONS = std::tuple<py::str, py::str>{py::str(".gb"), py::str(".gbc")};
inline const py::bytes LOGO = cb::compat::to_py(cb::ByteView(cb::formats::gb::kLogo));
// {cartridge type: memory bank controller name}, for the types the table lists
inline const py::dict<int64_t, py::str> MBC = [] {
    py::dict<int64_t, py::str> names;
    for (int64_t type = 0; type < 256; ++type) {
        cb::formats::gb::Rom probe;
        probe.cartridge_type = static_cast<std::uint8_t>(type);
        const std::string name = probe.mbc();
        if (name.rfind("type $", 0) != 0) names.set(type, cb::compat::to_py(name));
    }
    return names;
}();

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes data;
    bool logo_ok = false;
    py::str cgb;                       // "", "also" or "only"
    py::str title;
    bool sgb = false;
    py::str mbc;
    int64_t header_checksum = 0;
    bool header_checksum_ok = false;
    py::list<py::bytes> banks;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
};

bool is_gb(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// Function starts in the power-on address space ($0000-$7FFF): the entry at $0100, each
// interrupt vector that holds code (unused ones are commonly $FF or a lone RETI), then every
// CALL and RST target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

// The part of the old ROM that memory() and functions() need.
inline cb::formats::gb::Rom to_cb(const ROM& r) {
    cb::formats::gb::Rom out;
    out.data = cb::compat::from_py(r.data);
    return out;
}

inline py::ref<ROM> from_cb(const cb::formats::gb::Rom& r, const py::str& path) {
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->data = cb::compat::to_py(cb::ByteView(r.data));
    rom->logo_ok = r.logo_ok;
    rom->cgb = r.color == cb::formats::gb::ColorSupport::Also   ? py::str("also")
               : r.color == cb::formats::gb::ColorSupport::Only ? py::str("only")
                                                                : py::str("");
    rom->title = cb::compat::to_py(r.title);
    rom->sgb = r.sgb;
    rom->mbc = cb::compat::to_py(r.mbc());
    rom->header_checksum = r.header_checksum;
    rom->header_checksum_ok = r.header_checksum_ok;
    for (std::size_t i = 0; i < r.bank_count(); ++i) rom->banks.append(cb::compat::to_py(r.bank(i)));
    rom->vectors.set(py::str("entry"), cb::formats::gb::kEntry);
    for (const cb::formats::gb::InterruptVector& v : cb::formats::gb::kInterruptVectors) {
        rom->vectors.set(cb::compat::to_py(v.name), v.address);
    }
    return rom;
}

// The old SM83 decoder in the form cb::formats::code_scan wants (`memory`: the power-on image, converted once).
inline cb::formats::Decoder decoder(const py::bytes& memory) {
    return [&memory](cb::ByteView, std::uint32_t pc) -> std::optional<cb::formats::DecodedInsn> {
        const std::optional<decompile::sm83::Insn> insn = decompile::sm83::decode(memory, pc, pc);
        if (!insn) return std::nullopt;
        cb::formats::DecodedInsn out;
        out.size = static_cast<std::uint32_t>(insn->size);
        if (insn->target) out.target = static_cast<std::uint32_t>(*insn->target);
        if (insn->pattern == py::str("STOP")) out.flow = cb::formats::Flow::Halt;
        else if (insn->flow == py::str("call") || insn->flow == py::str("ccall")) out.flow = cb::formats::Flow::Call;
        else if (insn->flow == py::str("jump")) out.flow = cb::formats::Flow::Jump;
        else if (insn->flow == py::str("branch")) out.flow = cb::formats::Flow::Branch;
        else if (insn->flow == py::str("ret")) out.flow = cb::formats::Flow::Return;
        else if (insn->flow == py::str("indirect")) out.flow = cb::formats::Flow::Indirect;
        return out;
    };
}

}  // namespace shim_

inline py::bytearray ROM::memory() const {
    return py::bytearray(cb::compat::to_py(cb::ByteView(shim_::to_cb(*this).memory())));
}
inline py::list<int64_t> ROM::functions() const { return gb::functions(*this); }

inline bool is_gb(const py::str& path) { return cb::formats::gb::probe(cb::fs::from_text(cb::compat::from_py(path))); }

inline py::ref<ROM> read(const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(cb::formats::gb::read(cb::fs::from_text(cb::compat::from_py(path))), path);
    });
}

inline py::ref<ROM> parse(const py::bytes& data, const py::str& path) {
    return cb::compat::guard([&] { return shim_::from_cb(cb::formats::gb::parse(cb::compat::from_py(data)), path); });
}

inline py::list<int64_t> functions(const ROM& rom) {
    const cb::formats::gb::Rom cb_rom = shim_::to_cb(rom);
    const py::bytes memory = cb::compat::to_py(cb::ByteView(cb_rom.memory()));
    return cb::compat::to_py(cb_rom.functions(shim_::decoder(memory)));
}

}  // namespace codebreaker::formats::gb
