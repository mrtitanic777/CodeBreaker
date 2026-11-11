// codebreaker/verify/lanes/gb.cpp -- typed C++ port of codebreaker.verify.lanes.gb (see gb.hpp).

#include "codebreaker/verify/lanes/gb.hpp"

#include <algorithm>
#include <memory>

#include "codebreaker/emulate/sm83.hpp"
#include "codebreaker/formats/gb.hpp"

namespace codebreaker::verify::lanes::gb {

using image_lane::Effect;
namespace sm83 = codebreaker::emulate::sm83;

const py::str KIND("gb");

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
                                        py::str("HL"), py::str("RAM"), py::str("I/O and bank accesses")};
    return w;
}

Effects effects(const py::str& path, int64_t entry, const State& st) {
    py::bytearray mem = formats::gb::read(path)->memory();
    const int64_t seed = st.at(py::str("fill"));
    for (int64_t i = 0; i < (seed ? 0x2000 : 0); i++) {
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & ((seed % 2) ? 0x0F : 0xFF);
    }
    auto bus = std::make_shared<sm83::Bus>(mem);
    bus->map(IO_LO, IO_HI, [](int64_t a) -> int64_t { return a == 0xFF44 ? 0x90 : 0; });   // (LY in vblank)
    bus->map(0xFFFF, 0xFFFF);
    bus->map(0x2000, 0x3FFF, [mem](int64_t a) -> int64_t { return mem[a]; });               // bank select writes, logged
    sm83::RunOptions kw;
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
    const emulate::run::Run run = sm83::recorded(sm83::Memory(bus), entry, kw);
    const py::bytes ram = py::bytes(bus->mem.slice(0xC000, 0xE000).view()) + py::bytes(bus->mem.slice(0xFF80, 0xFFFF).view());
    py::list<py::str> io;
    int64_t kept = 0;
    for (const emulate::run::IO& a : run.io) {
        if (a.kind == py::str("r") && 0x2000 <= a.port && a.port <= 0x3FFF) continue;
        if (kept++ >= 4000) break;
        io.append(py::percent(py::str("%s %d %d"), a.kind, a.port, a.value));
    }
    Effects out{Effect(run.exit ? *run.exit : py::str("None"))};
    for (const char* k : {"A", "F", "BC", "DE", "HL"}) out.push_back(Effect(reg(run, k)));
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

}  // namespace codebreaker::verify::lanes::gb
