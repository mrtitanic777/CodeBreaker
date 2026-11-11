// codebreaker/verify/lanes/nes.cpp -- typed C++ port of codebreaker.verify.lanes.nes (see nes.hpp).

#include "codebreaker/verify/lanes/nes.hpp"

#include <algorithm>
#include <memory>

#include "codebreaker/emulate/m6502.hpp"
#include "codebreaker/formats/nes.hpp"

namespace codebreaker::verify::lanes::nes {

using image_lane::Effect;
namespace m6502 = codebreaker::emulate::m6502;

const py::str KIND("nes");

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
        state({{"a", 0}, {"x", 0}, {"y", 0}, {"fill", 0}}),
        state({{"a", 1}, {"x", 2}, {"y", 3}, {"fill", 1}}),
        state({{"a", 0x80}, {"x", 0xFF}, {"y", 0x7F}, {"fill", 2}}),
        state({{"a", 0x55}, {"x", 0x10}, {"y", 0xAA}, {"fill", 3}}),
        state({{"a", 0}, {"x", 0}, {"y", 0}, {"fill", 4}}),
        state({{"a", 0xFF}, {"x", 1}, {"y", 0}, {"fill", 5}}),
    };
    return s;
}

const py::list<py::str>& WHAT() {
    static const py::list<py::str> w = {py::str("end"), py::str("A"), py::str("X"), py::str("Y"),
                                        py::str("flags"), py::str("RAM"), py::str("hardware accesses")};
    return w;
}

Effects effects(const py::str& path, int64_t entry, const State& st) {
    py::bytearray mem = formats::nes::read(path)->memory();
    const int64_t seed = st.at(py::str("fill"));
    for (int64_t i = 0; i < (seed ? 0x800 : 0); i++) {   // small numbers, as counters and indexes hold
        mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & ((seed % 2) ? 0x0F : 0xFF);
    }
    auto bus = std::make_shared<m6502::Bus>(mem);
    bus->map(HARDWARE_LO, HARDWARE_HI, [](int64_t a) -> int64_t { return a == 0x2002 ? 0x80 : 0; });   // (the picture unit always in vblank)
    m6502::RunOptions kw;
    kw.steps = 20000;
    kw.a = st.at(py::str("a"));
    kw.x = st.at(py::str("x"));
    kw.y = st.at(py::str("y"));
    const emulate::run::Run run = m6502::recorded(m6502::Memory(bus), entry, kw);
    const py::bytes ram(bus->mem.slice(0, 0x800).view());
    py::list<py::str> io;
    const int64_t n = std::min<int64_t>(run.io.size(), 4000);
    for (int64_t i = 0; i < n; i++) {
        const emulate::run::IO& a = run.io[i];
        io.append(py::percent(py::str("%s %d %d"), a.kind, a.port, a.value));
    }
    return Effects{Effect(run.exit ? *run.exit : py::str("None")), Effect(reg(run, "A")), Effect(reg(run, "X")),
                   Effect(reg(run, "Y")), Effect(reg(run, "P")), Effect(ram), Effect(io)};
}

py::str start(const State& st) {
    return py::percent(py::str("from A=%02X X=%02X Y=%02X"), st.at(py::str("a")), st.at(py::str("x")), st.at(py::str("y")));
}

image_lane::Lane lane(const std::function<image_lane::Writer()>& platform) {
    return image_lane::Lane(KIND, REV, STATES(), effects, WHAT(), start, platform);
}

}  // namespace codebreaker::verify::lanes::nes
