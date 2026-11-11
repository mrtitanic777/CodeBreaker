// codebreaker/formats/sms.hpp -- compat shim over cb/formats/sms.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/sms.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet and is deleted with the last of them. Conversion and forwarding only; the Z80
// decoder `functions()` follows the code with is the decompiler's (still an old port) and is adapted here.
#pragma once

#include <array>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"
#include "cb/formats/sms.hpp"
#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::formats::sms {

constexpr int64_t PAGE = 0x4000;
inline const std::array<int64_t, 3> _HEADER_AT = {0x7FF0, 0x3FF0, 0x1FF0};
// {region nibble: (system, region name)}
inline const py::dict<int64_t, std::tuple<py::str, py::str>> REGIONS = {
    {3, {py::str("Master System"), py::str("Japan")}},
    {4, {py::str("Master System"), py::str("export")}},
    {5, {py::str("Game Gear"), py::str("Japan")}},
    {6, {py::str("Game Gear"), py::str("export")}},
    {7, {py::str("Game Gear"), py::str("international")}},
};
// {size nibble: ROM size in bytes}
inline const py::dict<int64_t, int64_t> _SIZES = {
    {0xA, 0x2000}, {0xB, 0x4000}, {0xC, 0x8000},   {0xD, 0xC000},  {0xE, 0x10000},
    {0xF, 0x20000}, {0x0, 0x40000}, {0x1, 0x80000}, {0x2, 0x100000},
};
inline const std::tuple<py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str>{py::str(".sms"), py::str(".gg"), py::str(".sg")};

// ROM.header: a dict with six fixed keys (a record)
struct Header {
    int64_t at = 0;
    int64_t checksum = 0;
    py::str product;
    int64_t version = 0;
    py::str region;
    std::optional<int64_t> size;   // _SIZES.get(size): None for a size code that is not listed
};
template <class F> void for_each_field(const Header& r, F&& f) {
    f("at", r.at);
    f("checksum", r.checksum);
    f("product", r.product);
    f("version", r.version);
    f("region", r.region);
    f("size", r.size);
}
template <class F> void for_each_field_mut(Header& r, F&& f) {
    f("at", r.at, true);
    f("checksum", r.checksum, true);
    f("product", r.product, true);
    f("version", r.version, true);
    f("region", r.region, true);
    f("size", r.size, true);
}

// the plain attribute holder the module's `ROM` is
class ROM : public py::object {
public:
    py::str path;
    py::bytes data;
    std::optional<Header> header;
    py::str system;
    bool mapper = false;
    py::list<py::bytes> pages;
    py::dict<py::str, int64_t> vectors;

    py::bytearray memory() const;
    py::list<int64_t> functions() const;
    // Where in the ROM the byte the Z80 reads at `address` at power-on is: an offset, or nullopt.
    std::optional<int64_t> mapped(int64_t address) const;
};

bool is_sms(const py::str& path);
py::ref<ROM> read(const py::str& path);
py::ref<ROM> parse(const py::bytes& data, const py::str& path = py::str(""));
py::bytearray _memory(const ROM& rom);

// Function starts in the power-on address space ($0000-$BFFF): the reset, interrupt and NMI
// vectors, then every CALL and RST target reached by following the code from them.
py::list<int64_t> functions(const ROM& rom);

// An interrupt vector the program does not use is often filled with $FF (RST $38) or zero.
bool _looks_like_code(const py::bytearray& mem, int64_t at);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

// The part of the old ROM that memory(), mapped() and functions() need.
inline cb::formats::sms::Rom to_cb(const ROM& r) {
    cb::formats::sms::Rom out;
    out.data = cb::compat::from_py(r.data);
    out.system = r.system == py::str("Game Gear") ? cb::formats::sms::System::GameGear
                 : r.system == py::str("SG-1000")  ? cb::formats::sms::System::Sg1000
                                                   : cb::formats::sms::System::MasterSystem;
    out.has_mapper = r.mapper;
    return out;
}

inline py::ref<ROM> from_cb(const cb::formats::sms::Rom& r, const py::str& path) {
    py::ref<ROM> rom = std::make_shared<ROM>();
    rom->path = path;
    rom->data = cb::compat::to_py(cb::ByteView(r.data));
    if (r.header) {
        Header h;
        h.at = r.header->at;
        h.checksum = r.header->checksum;
        h.product = cb::compat::to_py(r.header->product);
        h.version = r.header->version;
        h.region = cb::compat::to_py(r.header->region);
        h.size = cb::compat::to_py(r.header->size);
        rom->header = h;
    }
    rom->system = cb::compat::to_py(cb::formats::sms::to_string(r.system));
    rom->mapper = r.has_mapper;
    for (std::size_t i = 0; i < r.page_count(); ++i) rom->pages.append(cb::compat::to_py(r.page(i)));
    rom->vectors.set(py::str("reset"), cb::formats::sms::kResetVector);
    rom->vectors.set(py::str("irq"), cb::formats::sms::kIrqVector);
    rom->vectors.set(py::str("nmi"), cb::formats::sms::kNmiVector);
    return rom;
}

// The old Z80 decoder in the form cb::formats::code_scan wants (`memory`: the power-on image, converted once).
inline cb::formats::Decoder decoder(const py::bytes& memory) {
    return [&memory](cb::ByteView, std::uint32_t pc) -> std::optional<cb::formats::DecodedInsn> {
        const std::optional<decompile::z80::Insn> insn = decompile::z80::decode(memory, pc, pc);
        if (!insn) return std::nullopt;
        cb::formats::DecodedInsn out;
        out.size = static_cast<std::uint32_t>(insn->size);
        if (insn->target) out.target = static_cast<std::uint32_t>(*insn->target);
        if (insn->flow == py::str("call") || insn->flow == py::str("ccall")) out.flow = cb::formats::Flow::Call;
        else if (insn->flow == py::str("jump")) out.flow = cb::formats::Flow::Jump;
        else if (insn->flow == py::str("branch")) out.flow = cb::formats::Flow::Branch;
        else if (insn->flow == py::str("ret")) out.flow = cb::formats::Flow::Return;
        else if (insn->flow == py::str("indirect")) out.flow = cb::formats::Flow::Indirect;
        return out;
    };
}

}  // namespace shim_

inline py::bytearray ROM::memory() const { return _memory(*this); }
inline py::list<int64_t> ROM::functions() const { return sms::functions(*this); }
inline std::optional<int64_t> ROM::mapped(int64_t address) const {
    if (address < 0) return std::nullopt;
    return cb::compat::guard([&]() -> std::optional<int64_t> {
        const auto at = shim_::to_cb(*this).mapped(cb::compat::int_from_py<std::uint32_t>(address));
        if (!at) return std::nullopt;
        return static_cast<int64_t>(*at);
    });
}

inline bool is_sms(const py::str& path) { return cb::formats::sms::probe(cb::fs::from_text(cb::compat::from_py(path))); }

inline py::ref<ROM> read(const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(cb::formats::sms::read(cb::fs::from_text(cb::compat::from_py(path))), path);
    });
}

inline py::ref<ROM> parse(const py::bytes& data, const py::str& path) {
    return cb::compat::guard([&] {
        return shim_::from_cb(
            cb::formats::sms::parse(cb::compat::from_py(data), cb::formats::extension_of(cb::fs::from_text(cb::compat::from_py(path)))),
            path);
    });
}

inline py::bytearray _memory(const ROM& rom) {
    return py::bytearray(cb::compat::to_py(cb::ByteView(shim_::to_cb(rom).memory())));
}

inline py::list<int64_t> functions(const ROM& rom) {
    const cb::formats::sms::Rom cb_rom = shim_::to_cb(rom);
    const py::bytes memory = cb::compat::to_py(cb::ByteView(cb_rom.memory()));
    return cb::compat::to_py(cb_rom.functions(shim_::decoder(memory)));
}

inline bool _looks_like_code(const py::bytearray& mem, int64_t at) { return static_cast<int64_t>(mem[at]) != 0xFF; }

}  // namespace codebreaker::formats::sms
