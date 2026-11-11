// codebreaker/verify/lanes/z80_flat.cpp -- typed C++ port of codebreaker.verify.lanes.z80_flat (see z80_flat.hpp).

#include "codebreaker/verify/lanes/z80_flat.hpp"

#include <algorithm>
#include <memory>

#include "codebreaker/emulate/z80.hpp"
#include "codebreaker/verify/verdicts.hpp"

namespace codebreaker::verify::lanes::z80_flat {

using image_lane::Effect;
namespace z80 = codebreaker::emulate::z80;
namespace V = codebreaker::verify::verdicts;

const py::str SAME("the program built from this source is the original, byte for byte");

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

// "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
py::str start(const State& st) {
    return py::percent(py::str("from A=%02X BC=%02X%02X HL=%02X%02X"), st.at(py::str("a")), st.at(py::str("b")),
                       st.at(py::str("c")), st.at(py::str("h")), st.at(py::str("l")));
}

}  // namespace

const py::list<State>& STATES() {
    static const py::list<State> s = {
        state({{"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}, {"e", 0}, {"h", 0}, {"l", 0}, {"fill", 0}}),
        state({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 0xC1}, {"e", 0x00}, {"h", 0xC2}, {"l", 0x10}, {"fill", 1}}),
        state({{"a", 0x80}, {"b", 0xFF}, {"c", 0x7F}, {"d", 0xD0}, {"e", 0x80}, {"h", 0xE1}, {"l", 0x00}, {"fill", 2}}),
    };
    return s;
}

const py::list<py::str>& WHAT() {
    static const py::list<py::str> w = {py::str("end"), py::str("A"), py::str("flags"), py::str("BC"), py::str("DE"),
                                        py::str("HL"), py::str("IX"), py::str("IY"), py::str("memory"), py::str("port accesses")};
    return w;
}

Effects effects(const py::str& kind, const std::tuple<int64_t, int64_t>& rom, const Subject& subject, int64_t entry, const State& st) {
    (void)kind;   // (the emulator slot of every machine of this lane is the Z80 of emulate/z80.py)
    const Program& program = subject.program;
    const py::bytes& image = subject.image;
    py::bytearray mem = program.memory ? py::bytearray(program.memory->view()) : py::bytearray::zeros(0x10000);
    const int64_t lo = std::get<0>(rom), hi = std::get<1>(rom);
    mem.set_slice(lo, hi, py::bytes(std::string(static_cast<size_t>(std::max<int64_t>(hi - lo, 0)), '\xC9')));   // the ROM: every byte RET
    mem.set_slice(program.origin, program.origin + image.size(), image);
    const int64_t seed = st.at(py::str("fill"));
    const int64_t end = program.origin + image.size();
    const int64_t stop = seed ? 0x10000 : end;
    for (int64_t i = end; i < stop; i++) {   // memory past the program
        mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & ((seed % 2) ? 0x0F : 0xFF);
    }
    auto bus = std::make_shared<z80::Bus>(mem, [](int64_t) -> int64_t { return 0xFF; });
    z80::RunOptions kw;
    kw.steps = 20000;
    kw.sp = 0xFFF0;
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
    const py::bytes before(mem.view());
    const emulate::run::Run run = z80::recorded(z80::Memory(bus), entry, kw);
    // what the run changed (the program lives in this memory: an edited byte the function does not
    // write is the source's difference, not the function's)
    py::list<py::str> changed;
    const py::bytes& after = bus->mem.view();
    const int64_t n = std::min(before.size(), after.size());
    for (int64_t i = 0; i < n; i++) {
        if (before[i] != after[i]) changed.append(py::percent(py::str("%d %d"), i, static_cast<int64_t>(after[i])));
    }
    py::list<py::str> io;
    const int64_t m = std::min<int64_t>(run.io.size(), 4000);
    for (int64_t i = 0; i < m; i++) {
        const emulate::run::IO& x = run.io[i];
        io.append(py::percent(py::str("%s %d %d"), x.kind, x.port, x.value));
    }
    Effects out{Effect(run.exit ? *run.exit : py::str("None"))};
    for (const char* k : {"A", "F", "BC", "DE", "HL", "IX", "IY"}) out.push_back(Effect(reg(run, k)));
    out.push_back(Effect(changed));
    out.push_back(Effect(io));
    return out;
}

Lane::Lane(const py::str& kind, int64_t rev, const std::tuple<int64_t, int64_t>& rom, const FlatWriter& writer,
           const std::function<Program(const py::str& path)>& open)
    : KIND(kind), REV(rev), SAME(z80_flat::SAME), rom_(rom), writer_(writer), open_(open),
      inner_(kind, rev, STATES(), nullptr, WHAT(), start, nullptr) {}

std::tuple<py::str, py::str> Lane::profile(const std::optional<py::str>& module_path) const {
    return inner_.profile(module_path);
}

Prepared Lane::prepare(const py::str& module_path, const py::str& work) const {
    Prepared out;
    // image_lane's prepare() writes the source through the platform's writer; it is given this lane's, and
    // what that returned is kept (the built block is in it)
    FlatWritten written;
    const FlatWriter writer = writer_;
    const image_lane::Lane base(KIND, REV, STATES(), nullptr, WHAT(), start,
                                [&written, writer]() -> image_lane::Writer {
                                    return [&written, writer](const py::str& p, const py::str& w) -> image_lane::Written {
                                        written = writer(p, w);
                                        return written;
                                    };
                                });
    out.base = base.prepare(module_path, work);
    out.written = written;
    return out;
}

Built Lane::build(const Prepared& m, const py::str& module_path, const py::str& work) const {
    (void)work;
    const FlatWritten& r = m.written;
    Built out;
    if (!(r.linked && *r.linked)) {
        py::str why = (r.notes && !r.notes->empty()) ? (*r.notes)[-1] : py::str("did not build");
        why = why.slice(0, 200);
        for (const py::str& n : m.base.order) out.missing.set(n, why);
        return out;
    }
    // subjects(): (p, p.image), (p, written["built image"])
    const Program p = open_(module_path);
    if (!r.built_image) throw py::KeyError("'built image'");
    Subject original{p, p.image};
    Subject rebuilt{p, *r.built_image};
    if (!r.same) throw py::KeyError("'same'");
    out.context = Context{*r.same, original, rebuilt};
    for (const py::str& n : m.base.order) out.items.set(n, image_lane::Item(m.base.key.at(n), m.base.by_name.at(n)));
    return out;
}

image_lane::Verdict Lane::judge(const std::optional<Context>& context, const image_lane::Item& item) const {
    if (!context) throw py::TypeError("cannot unpack non-iterable NoneType object");
    if (context->same) return {V::EQUIVALENT, py::str(""), py::list<py::str>(), SAME};
    const int64_t entry = std::get<1>(item);
    for (const State& st : STATES()) {
        const Effects a = effects(KIND, rom_, context->original, entry, st);
        const Effects b = effects(KIND, rom_, context->rebuilt, entry, st);
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

Lane lane(const py::str& kind, int64_t rev, const std::tuple<int64_t, int64_t>& rom, const FlatWriter& writer,
          const std::function<Program(const py::str& path)>& open) {
    return Lane(kind, rev, rom, writer, open);
}

}  // namespace codebreaker::verify::lanes::z80_flat
