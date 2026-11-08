// codebreaker/verify/lanes/sms.cpp -- typed C++ port of codebreaker.verify.lanes.sms (see sms.hpp).

#include "codebreaker/verify/lanes/sms.hpp"

#include <algorithm>
#include <memory>

#include "codebreaker/emulate/z80.hpp"
#include "codebreaker/formats/sms.hpp"

namespace codebreaker::verify::lanes::sms {

using image_lane::Effect;
namespace z80 = codebreaker::emulate::z80;

const py::str KIND("sms");

namespace {

State state(std::initializer_list<std::pair<const char*, int64_t>> items) {
    State s;
    for (const auto& kv : items) s.set(py::str(kv.first), kv.second);
    return s;
}

int64_t reg(const emulate::run::Run& run, const char* name) {
    const emulate::run::Register& r = run.results.at(py::str(name));
    if (const int64_t* v = std::get_if<int64_t>(&r)) return *v;
    throw py::TypeError(std::string("register ") + name + " is not an integer");
}

}  // namespace

const py::list<State>& STATES() {
    static const py::list<State> s = {
        state({{"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}, {"e", 0}, {"h", 0}, {"l", 0}, {"fill", 0}}),
        state({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 0xC1}, {"e", 0x00}, {"h", 0xC2}, {"l", 0x10}, {"fill", 1}}),
        state({{"a", 0x80}, {"b", 0xFF}, {"c", 0x7F}, {"d", 0xC0}, {"e", 0x80}, {"h", 0xC1}, {"l", 0x00}, {"fill", 2}}),
        state({{"a", 0x55}, {"b", 0x10}, {"c", 0xAA}, {"d", 0xD0}, {"e", 0x00}, {"h", 0xC8}, {"l", 0x40}, {"fill", 3}}),
    };
    return s;
}

const py::list<py::str>& WHAT() {
    static const py::list<py::str> w = {py::str("end"), py::str("A"), py::str("flags"), py::str("BC"), py::str("DE"),
                                        py::str("HL"), py::str("IX"), py::str("IY"), py::str("RAM"),
                                        py::str("port and mapper accesses")};
    return w;
}

Effects effects(const py::str& path, int64_t entry, const State& st) {
    py::bytearray mem = formats::sms::read(path)->memory();
    const int64_t seed = st.at(py::str("fill"));
    for (int64_t i = 0; i < (seed ? 0x2000 : 0); i++) {   // small numbers, as counters and indexes hold
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & ((seed % 2) ? 0x0F : 0xFF);
    }
    auto bus = std::make_shared<z80::Bus>(mem, [](int64_t port) -> int64_t { return (port & 0xFF) == 0xBF ? 0x80 : 0xFF; });   // (the VDP always in vblank)
    bus->map(MAPPER_LO, MAPPER_HI);
    z80::RunOptions kw;
    kw.steps = 20000;
    for (const auto& item : st.items()) {
        const py::str& k = std::get<0>(item);
        const int64_t v = std::get<1>(item);
        if (k == py::str("a")) kw.regs.a = v;
        else if (k == py::str("b")) kw.regs.b = v;
        else if (k == py::str("c")) kw.regs.c = v;
        else if (k == py::str("d")) kw.regs.d = v;
        else if (k == py::str("e")) kw.regs.e = v;
        else if (k == py::str("h")) kw.regs.h = v;
        else if (k == py::str("l")) kw.regs.l = v;
        else if (!(k == py::str("fill"))) throw py::TypeError("run() got an unexpected keyword argument '" + k.utf8() + "'");
    }
    const emulate::run::Run run = z80::recorded(z80::Memory(bus), entry, kw);
    const py::bytes ram(bus->mem.slice(0xC000, 0xE000).view());
    py::list<py::str> io;
    const int64_t n = std::min<int64_t>(run.io.size(), 4000);
    for (int64_t i = 0; i < n; i++) {
        const emulate::run::IO& a = run.io[i];
        io.append(py::percent(py::str("%s %d %d"), a.kind, a.port, a.value));
    }
    Effects out{Effect(run.exit ? *run.exit : py::str("None"))};
    for (const char* k : {"A", "F", "BC", "DE", "HL", "IX", "IY"}) out.push_back(Effect(reg(run, k)));
    out.push_back(Effect(ram));
    out.push_back(Effect(io));
    return out;
}

py::str start(const State& st) {
    return py::percent(py::str("from A=%02X BC=%02X%02X HL=%02X%02X"), st.at(py::str("a")), st.at(py::str("b")),
                       st.at(py::str("c")), st.at(py::str("h")), st.at(py::str("l")));
}

image_lane::Lane lane(const std::function<image_lane::Writer()>& platform) {
    return image_lane::Lane(KIND, REV, STATES(), effects, WHAT(), start, platform);
}

}  // namespace codebreaker::verify::lanes::sms
