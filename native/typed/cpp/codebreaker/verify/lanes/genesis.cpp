// codebreaker/verify/lanes/genesis.cpp -- typed C++ port of codebreaker.verify.lanes.genesis (see genesis.hpp).

#include "codebreaker/verify/lanes/genesis.hpp"

#include <algorithm>
#include <memory>

#include "codebreaker/emulate/m68k.hpp"
#include "codebreaker/formats/genesis.hpp"
#include "codebreaker/verify/verdicts.hpp"

namespace codebreaker::verify::lanes::genesis {

using image_lane::Effect;
namespace m68k = codebreaker::emulate::m68k;
namespace V = codebreaker::verify::verdicts;

const py::str KIND_("genesis");

namespace {

State state(std::initializer_list<int64_t> d, std::initializer_list<int64_t> a, int64_t fill) {
    State s;
    for (int64_t v : d) s.d.append(v);
    for (int64_t v : a) s.a.append(v);
    s.fill = fill;
    return s;
}

int64_t reg(const emulate::run::Run& run, const py::str& name) {
    const emulate::run::Register& r = run.results.at(name);
    if (const int64_t* v = std::get_if<int64_t>(&r)) return *v;
    throw py::TypeError("register " + name.utf8() + " is not an integer");
}

// repr of a list of integers: [1, 2]
py::str repr_ints(const py::list<int64_t>& xs) {
    py::list<py::str> parts;
    for (int64_t v : xs) parts.append(py::to_str(v));
    return py::str("[") + py::join(py::str(", "), parts) + py::str("]");
}

// repr of a state as Python writes the dict: {'d': [...], 'a': [...], 'fill': 0}
py::str repr_state(const State& s) {
    return py::str("{'d': ") + repr_ints(s.d) + py::str(", 'a': ") + repr_ints(s.a) + py::str(", 'fill': ") + py::to_str(s.fill) + py::str("}");
}

}  // namespace

const py::str KIND = KIND_;

const py::list<State>& STATES() {
    static const py::list<State> s = {
        state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0),
        state({1, 2, 3, 4, 5, 6, 7, 8}, {0xFF0100, 0xFF0200, 0xFF0300, 0xFF0400, 0xFF0500, 0xFF0600, 0xFF0700}, 1),
        state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40},
              {0xFF8000, 0xFF9000, 0xFFA000, 0xFFB000, 0xFFC000, 0xFFD000, 0xFFE000}, 2),
    };
    return s;
}

const py::list<py::str>& WHAT() {
    static const py::list<py::str> w = {py::str("end"), py::str("D registers"), py::str("A registers"),
                                        py::str("condition codes"), py::str("RAM"), py::str("hardware accesses")};
    return w;
}

Effects effects(const py::str& path, int64_t entry, const State& st) {
    py::bytearray mem = formats::genesis::read(path)->memory();
    const int64_t seed = st.fill;
    for (int64_t i = 0; i < (seed ? 0x10000 : 0); i++) {
        mem[0xFF0000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & ((seed % 2) ? 0x0F : 0xFF);
    }
    auto bus = std::make_shared<m68k::Bus>(mem);
    // (VDP status: in vblank)
    const m68k::IoRead vdp = [](int64_t a, int64_t) -> int64_t { return (a & ~int64_t(1)) == 0xC00004 ? 0x0008 : 0; };
    bus->map(HARDWARE_1_LO, HARDWARE_1_HI, vdp);
    bus->map(HARDWARE_2_LO, HARDWARE_2_HI, vdp);
    m68k::RunOptions kw;
    kw.steps = 20000;
    kw.d = st.d;
    py::list<int64_t> a = st.a;   // state["a"] + [0]
    a.append(0);
    kw.a = a;
    const emulate::run::Run run = m68k::recorded(bus, entry, kw);
    py::list<int64_t> d, ar;
    Effects out{Effect(run.exit ? *run.exit : py::str("None"))};
    for (int64_t i = 0; i < 8; i++) d.append(reg(run, py::percent(py::str("D%d"), i)));
    for (int64_t i = 0; i < 7; i++) ar.append(reg(run, py::percent(py::str("A%d"), i)));
    py::list<py::str> dtext, atext;
    for (int64_t v : d) dtext.append(py::to_str(v));
    for (int64_t v : ar) atext.append(py::to_str(v));
    out.push_back(Effect(dtext));
    out.push_back(Effect(atext));
    out.push_back(Effect(reg(run, py::str("SR")) & 0x1F));
    out.push_back(Effect(py::bytes(mem.slice(0xFF0000, 0x1000000).view())));
    py::list<py::str> io;
    const int64_t n = std::min<int64_t>(run.io.size(), 4000);
    for (int64_t i = 0; i < n; i++) {
        const emulate::run::IO& x = run.io[i];
        io.append(py::percent(py::str("%s %d %d"), x.kind, x.port, x.value));
    }
    out.push_back(Effect(io));
    return out;
}

py::str start(const State& st) {
    return py::percent(py::str("from D0=%08X A0=%06X"), st.d[0], st.a[0]);
}

Lane::Lane(const std::function<image_lane::Writer()>& platform)
    : KIND(KIND_), REV(genesis::REV), SAME(image_lane::SAME),
      inner_(KIND_, genesis::REV, py::list<image_lane::State>(),
             [](const py::str&, int64_t, const image_lane::State&) { return Effects(); }, WHAT(),
             [](const image_lane::State&) { return py::str(""); }, platform, nullptr, py::str("%06x")) {}

std::tuple<py::str, py::str> Lane::profile(const std::optional<py::str>& module_path) const {
    (void)module_path;
    // repr((self.KIND, self.REV, self._states))  (the states are part of what a verdict depends on)
    py::list<py::str> shown;
    for (const State& s : STATES()) shown.append(repr_state(s));
    const py::str settings = py::str("(") + py::repr(KIND) + py::str(", ") + py::to_str(REV) + py::str(", [") +
                             py::join(py::str(", "), shown) + py::str("])");
    return {py::hashlib::sha1(settings.encode()).hexdigest().slice(0, 12), settings};
}

image_lane::Prepared Lane::prepare(const py::str& module_path, const py::str& work) const {
    return inner_.prepare(module_path, work);
}

image_lane::Built Lane::build(const image_lane::Prepared& m, const py::str& module_path, const py::str& work) const {
    return inner_.build(m, module_path, work);
}

image_lane::Verdict Lane::judge(const std::optional<image_lane::Context>& context, const image_lane::Item& item) const {
    if (!context) throw py::TypeError("cannot unpack non-iterable NoneType object");
    if (context->same) return {V::EQUIVALENT, py::str(""), py::list<py::str>(), SAME};
    const int64_t entry = std::get<1>(item);
    for (const State& st : STATES()) {
        const Effects a = effects(context->original, entry, st);
        const Effects b = effects(context->rebuilt, entry, st);
        if (!(a == b)) {
            // next(w for w, x, y in zip(self._what, a, b) if x != y)
            const py::list<py::str>& what = WHAT();
            const size_t n = std::min<size_t>({static_cast<size_t>(what.size()), a.size(), b.size()});
            for (size_t i = 0; i < n; ++i) {
                if (!(a[i] == b[i])) return {V::DIVERGENT, what[static_cast<int64_t>(i)] + py::str("-differs"), py::list<py::str>(), start(st)};
            }
            throw py::StopIteration();
        }
    }
    return {V::EQUIVALENT, py::str(""), py::list<py::str>(),
            py::percent(py::str("the same effects from %d register states"), static_cast<int64_t>(STATES().size()))};
}

}  // namespace codebreaker::verify::lanes::genesis
