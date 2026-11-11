// codebreaker/formats/genesis.hpp -- compat shim over cb/formats/genesis.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/genesis.{hpp,cpp}; this header keeps the old py:: surface for the ports
// that have not been refactored yet and is deleted with the last of them. Conversion and forwarding only; the 68000
// decoder `functions()` follows the code with is the decompiler's (still an old port) and is adapted here.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/formats/probe.hpp"
#include "codebreaker/decompile/m68k.hpp"

namespace codebreaker::formats::genesis {

inline const std::tuple<py::str, py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str, py::str>{py::str(".md"), py::str(".bin"), py::str(".gen"), py::str(".smd")};
// {vector number: name}
inline const py::dict<int64_t, py::str> VECTOR_NAMES = [] {
    py::dict<int64_t, py::str> names;
    for (unsigned n = 0; n < 64; ++n) {
        const std::string name = cb::formats::genesis::vector_name(n);
        if (!name.empty()) names.set(n, cb::compat::to_py(name));
    }
    return names;
}();

// ROM.header: a dict with ten fixed keys (a record); the keys "domestic name" and "overseas name" have a space.
struct Header {
    py::str system;
    py::str copyright;
    py::str domestic_name;
    py::str overseas_name;
    py::str serial;
    int64_t checksum = 0;
    py::str devices;
    std::tuple<int64_t, int64_t> rom;
    std::tuple<int64_t, int64_t> ram;
    py::str region;
};
template <class F> void for_each_field(const Header& r, F&& f) {
    f("system", r.system);
    f("copyright", r.copyright);
    f("domestic name", r.domestic_name);
    f("overseas name", r.overseas_name);
    f("serial", r.serial);
    f("checksum", r.checksum);
    f("devices", r.devices);
    f("rom", r.rom);
    f("ram", r.ram);
    f("region", r.region);
}
template <class F> void for_each_field_mut(Header& r, F&& f) {
    f("system", r.system, true);
    f("copyright", r.copyright, true);
    f("domestic name", r.domestic_name, true);
    f("overseas name", r.overseas_name, true);
    f("serial", r.serial, true);
    f("checksum", r.checksum, true);
    f("devices", r.devices, true);
    f("rom", r.rom, true);
    f("ram", r.ram, true);
    f("region", r.region, true);
}

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    bool smd = false;
    py::bytes data;
    Header header;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // the checksum the header should hold: the sum of the words from $200, mod 2**16
    int64_t checksum() const;
};

bool is_genesis(const py::str& path);
py::ref<ROM> read(const py::str& path);

// An .smd image: a 512-byte header, then 16 KB blocks holding each block's odd bytes first.
py::bytes _desmd(const py::bytes& data);

// ValueError if the image is too short for a cartridge.
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));

// Function starts: the reset vector and the handlers the vectors name, then every JSR and BSR
// target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

// The part of the old ROM that memory(), checksum() and functions() need.
inline cb::formats::genesis::Rom to_cb(const ROM& r) {
    cb::formats::genesis::Rom out;
    out.data = cb::compat::from_py(r.data);
    out.reset = cb::compat::int_from_py<std::uint32_t>(r.vectors.at(py::str("reset")));
    for (const auto& kv : r.vectors.items()) {
        const py::str& name = std::get<0>(kv);
        if (name == py::str("ssp") || name == py::str("reset")) continue;
        out.vectors.push_back({cb::compat::from_py(name), cb::compat::int_from_py<std::uint32_t>(std::get<1>(kv))});
    }
    return out;
}

inline py::ref<ROM> from_cb(const cb::formats::genesis::Rom& r, const py::str& path) {
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->smd = r.smd;
    rom->data = cb::compat::to_py(cb::ByteView(r.data));
    const cb::formats::genesis::Header& h = r.header;
    rom->header.system = cb::compat::to_py(h.system);
    rom->header.copyright = cb::compat::to_py(h.copyright);
    rom->header.domestic_name = cb::compat::to_py(h.domestic_name);
    rom->header.overseas_name = cb::compat::to_py(h.overseas_name);
    rom->header.serial = cb::compat::to_py(h.serial);
    rom->header.checksum = h.checksum;
    rom->header.devices = cb::compat::to_py(h.devices);
    rom->header.rom = {h.rom_start, h.rom_end};
    rom->header.ram = {h.ram_start, h.ram_end};
    rom->header.region = cb::compat::to_py(h.region);
    rom->vectors.set(py::str("ssp"), r.initial_ssp);
    rom->vectors.set(py::str("reset"), r.reset);
    for (const cb::formats::genesis::Vector& v : r.vectors) rom->vectors.set(cb::compat::to_py(v.name), v.address);
    return rom;
}

// The old 68000 decoder in the form cb::formats::code_scan wants (`data`: the ROM, converted once).
inline cb::formats::Decoder decoder(const py::bytes& data) {
    return [&data](cb::ByteView, std::uint32_t pc) -> std::optional<cb::formats::DecodedInsn> {
        const std::optional<decompile::m68k::Insn> insn = decompile::m68k::decode(data, pc, pc);
        if (!insn) return std::nullopt;
        cb::formats::DecodedInsn out;
        out.size = static_cast<std::uint32_t>(insn->length);
        if (insn->target) out.target = static_cast<std::uint32_t>(*insn->target);
        if (insn->flow == py::str("call")) {
            // a TRAP's handler is followed but is not a function start
            out.flow = insn->mnemonic == py::str("TRAP") ? cb::formats::Flow::Branch : cb::formats::Flow::Call;
        } else if (insn->flow == py::str("jump")) {
            out.flow = cb::formats::Flow::Jump;
        } else if (insn->flow == py::str("branch")) {
            out.flow = cb::formats::Flow::Branch;
        } else if (insn->flow == py::str("ret")) {
            out.flow = cb::formats::Flow::Return;
        } else if (insn->flow == py::str("indirect")) {
            out.flow = cb::formats::Flow::Indirect;
        } else if (insn->flow == py::str("halt")) {
            out.flow = cb::formats::Flow::Halt;
        }
        return out;
    };
}

}  // namespace shim_

inline py::bytearray ROM::memory() const {
    return py::bytearray(cb::compat::to_py(cb::ByteView(shim_::to_cb(*this).memory())));
}
inline py::list<int64_t> ROM::functions() const { return genesis::functions(*this); }
inline int64_t ROM::checksum() const { return shim_::to_cb(*this).computed_checksum(); }

inline bool is_genesis(const py::str& path) {
    return cb::formats::genesis::probe(cb::fs::from_text(cb::compat::from_py(path)));
}

inline py::ref<ROM> read(const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(cb::formats::genesis::read(cb::fs::from_text(cb::compat::from_py(path))), path);
    });
}

inline py::bytes _desmd(const py::bytes& data) {
    return cb::compat::guard(
        [&] { return cb::compat::to_py(cb::ByteView(cb::formats::genesis::deinterleave_smd(cb::compat::from_py(data)))); });
}

inline py::ref<ROM> parse(const py::bytes& data, const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(
            cb::formats::genesis::parse(cb::compat::from_py(data),
                                        cb::formats::extension_of(cb::fs::from_text(cb::compat::from_py(path)))),
            path);
    });
}

inline py::list<int64_t> functions(const ROM& rom) {
    const cb::formats::genesis::Rom cb_rom = shim_::to_cb(rom);
    const py::bytes data = cb::compat::to_py(cb::ByteView(cb_rom.data));
    return cb::compat::to_py(cb_rom.functions(shim_::decoder(data)));
}

}  // namespace codebreaker::formats::genesis
