// codebreaker/verify/lanes/driver.cpp -- typed C++ port of codebreaker.verify.lanes.driver (see driver.hpp).

#include "codebreaker/verify/lanes/driver.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/driver.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/shadow.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "codebreaker/platforms/vxd.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "codebreaker/verify/teeth.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::lanes::driver {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace ed = codebreaker::emulate::driver;

str S(const char* s) { return str(s); }

constexpr int64_t M32 = 0xFFFFFFFFLL;

std::map<std::u32string, std::shared_ptr<ed::Image>>& images() {
    static std::map<std::u32string, std::shared_ptr<ed::Image>> m;
    return m;
}

std::shared_ptr<ed::Image> image_of(const str& path) {
    auto& m = images();
    auto it = m.find(path.u32());
    if (it == m.end()) it = m.emplace(path.u32(), std::make_shared<ed::Image>(ed::load(path))).first;
    return it->second;
}

str held_repr(const Held& h) { return std::holds_alternative<int64_t>(h) ? py::to_str(std::get<int64_t>(h)) : py::repr(std::get<str>(h)); }

str held_text(const Held& h) { return std::holds_alternative<int64_t>(h) ? py::to_str(std::get<int64_t>(h)) : std::get<str>(h); }

// the 16 hex digits of sha1(text)
str digest16(const str& text) { return py::hashlib::sha1(text.encode("utf-8")).hexdigest().slice(0, 16); }

}  // namespace

const str KIND = S("le");

int64_t REG_CODE(const str& name) {
    static const char* names[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "cf", "zf", "ebp"};
    for (int64_t i = 0; i < 9; ++i)
        if (name == S(names[i])) return i + 1;
    return 0;
}

// ---- from a run to a fingerprint ---------------------------------------------------------------------------------------------------------
str _value(const ed::Image& image, int64_t v, bool aligned) {
    py::list<lane_common::Span> frame;
    frame.append(lane_common::Span(std::make_tuple(ed::STACK_LO, ed::STACK_HI)));
    lane_common::Image img;
    if (!aligned) img = std::function<bool(int64_t)>([&image](int64_t a) { return image.holds(a); });
    return lane_common::argument(v, py::list<int64_t>(), std::nullopt, false, frame, img);
}

str _service(const std::optional<int64_t>& device, int64_t service) {
    if (!device) return S("outside");
    const std::optional<str> name = symbols::driver_services::device_name(*device);
    const str dev = name && !name->empty() ? *name : py::percent(S("%04X"), *device);
    return py::percent(S("%s.%s%s"), dev, symbols::driver_services::label(*device, service & 0x7FFF), (service & 0x8000) ? S("!") : S(""));
}

std::optional<std::tuple<int64_t, str>> _effects(const ed::Image& image, const emulate::run::Run& r, bool aligned) {
    if (r.capped) return std::nullopt;
    std::map<int64_t, Held> state;
    std::map<int64_t, int64_t> before;
    std::vector<std::tuple<int64_t, Held>> own;
    for (const emulate::run::Write& w : r.writes) {
        const int64_t a = w.address, n = std::min<int64_t>(w.size, 8), v = w.value;
        if (ed::STACK_LO <= a && a < ed::STACK_HI) continue;
        const bool inside = image.holds(a);
        const uint64_t mask = n >= 8 ? ~0ULL : ((1ULL << (8 * n)) - 1);
        if (inside && !aligned) {
            own.emplace_back(n, n == 4 ? Held(_value(image, v, aligned)) : Held(static_cast<int64_t>(static_cast<uint64_t>(v) & mask)));
            continue;
        }
        const int64_t old = w.old.value_or(0);
        for (int64_t i = 0; i < n; ++i)
            if (!before.count(a + i)) before[a + i] = (old >> (8 * i)) & 0xFF;
        if (n == 4) {
            const str val = _value(image, v, aligned);
            if (val == S("S") || val == S("P")) {
                for (int64_t i = 0; i < 4; ++i) state[a + i] = val;
                continue;
            }
        }
        for (int64_t i = 0; i < n; ++i) state[a + i] = static_cast<int64_t>((v >> (8 * i)) & 0xFF);
    }
    std::vector<std::pair<int64_t, Held>> kept;
    for (const auto& kv : state) {
        auto it = before.find(kv.first);
        const bool same = it != before.end() && std::holds_alternative<int64_t>(kv.second) && std::get<int64_t>(kv.second) == it->second;
        if (!same) kept.emplace_back(kv.first, kv.second);
    }
    py::list<str> items;
    for (const auto& kv : kept) items.append(S("(") + py::to_str(kv.first) + S(", ") + held_repr(kv.second) + S(")"));
    py::list<str> owns;
    for (const auto& o : own) owns.append(S("(") + py::to_str(std::get<0>(o)) + S(", ") + held_repr(std::get<1>(o)) + S(")"));
    const str text = S("([") + py::join(S(", "), items) + S("], [") + py::join(S(", "), owns) + S("])");
    return std::make_tuple(static_cast<int64_t>(kept.size() + own.size()), digest16(text));
}

namespace {

str vector_repr(const Vector& v) {
    py::list<str> regs;
    std::vector<std::pair<str, int64_t>> rv;
    for (const auto& kv : v.regs.items()) rv.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(rv.begin(), rv.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    for (const auto& p : rv) regs.append(S("(") + py::repr(p.first) + S(", ") + py::to_str(p.second) + S(")"));
    py::list<str> st;
    for (int64_t x : v.stack) st.append(py::to_str(x));
    py::list<str> mem;
    for (const auto& m : v.memory) mem.append(S("(") + py::to_str(std::get<0>(m)) + S(", ") + py::repr(std::get<1>(m)) + S(")"));
    return S("([") + py::join(S(", "), regs) + S("], (") + py::join(S(", "), st) + (st.size() == 1 ? S(",") : S("")) + S("), ") + py::to_str(v.flags) + S(", [") +
           py::join(S(", "), mem) + S("], ") + py::to_str(v.stub_ret.value_or(0)) + S(", ") + py::to_str(v.stub_carry) + S(")");
}

}  // namespace

std::tuple<compare::Fingerprint, Detail> _run(const ed::Image& image, int64_t entry, const Vector& v, bool aligned) {
    Detail detail;
    emulate::run::Run r(std::nullopt);
    try {
        r = ed::recorded(image, entry, v.regs, v.stack, v.memory, v.stub_ret.value_or(0), v.stub_carry, ed::INSN_CAP, v.flags);
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {
        compare::Fingerprint fp;
        fp.err = py::percent(S("%s: %s"), str(e.type()), str(e.what()));
        return {fp, detail};
    }
    py::list<lane_common::Called> called;
    for (const auto& c : r.calls) {
        lane_common::Called k;
        const auto& d = c.detail;
        std::optional<str> dev;
        int64_t second = 0;
        if (std::holds_alternative<std::tuple<std::optional<str>, emulate::run::Word>>(d)) {
            const auto& t = std::get<std::tuple<std::optional<str>, emulate::run::Word>>(d);
            dev = std::get<0>(t);
            if (std::holds_alternative<int64_t>(std::get<1>(t))) second = std::get<int64_t>(std::get<1>(t));
        }
        if (dev) k.name = _service(py::int_parse(*dev), second);
        else k.name = S("outside:") + _value(image, second, aligned);
        k.args = c.args;
        called.append(k);
    }
    py::list<str> deps = lane_common::calls(called, [&](int64_t x) { return _value(image, x, aligned); }, nullptr, nullptr, std::optional<int64_t>(ed::MAX_CALLS));
    {
        int64_t n = 0;
        for (const auto& p : r.io) {
            if (n++ >= ed::MAX_CALLS) break;
            deps.append(py::percent(S("%s(%x,%d,%x)"), p.kind, p.port, p.size.value_or(0), p.value));
        }
    }
    const bool done = r.ended() == S("ret");
    auto memory = _effects(image, r, aligned);
    py::list<Held> result;
    if (done) {
        result.append(Held(std::get<int64_t>(r.results.at(S("flags")))));
        for (const str& n : ed::RESULT_REGISTERS) result.append(Held(_value(image, std::get<int64_t>(r.results.at(n)), aligned)));
    }
    py::list<str> rr;
    for (const Held& h : result) rr.append(held_repr(h));
    const str result_repr = S("(") + py::join(S(", "), rr) + (rr.size() == 1 ? S(",") : S("")) + S(")");
    std::optional<str> writes;
    if (memory) {
        // "%d:%s" % (memory[0], sha1(repr((memory[1], result)).encode()).hexdigest()[:16])
        writes = py::percent(S("%d:%s"), std::get<0>(*memory), digest16(S("(") + py::repr(std::get<1>(*memory)) + S(", ") + result_repr + S(")")));
    }
    std::vector<int64_t> cover;
    for (int64_t p : r.path) cover.push_back(p);
    std::sort(cover.begin(), cover.end());
    py::list<int64_t> cov(cover);
    compare::Fingerprint fp = lane_common::fingerprint(r, deps, std::get<int64_t>(r.results.at(S("eax"))), writes, std::nullopt, cov);
    detail.memory = memory;
    detail.result = result;
    return {fp, detail};
}

compare::Fingerprint fingerprint(const ed::Image& image, int64_t entry, const Vector& v, bool aligned) {
    const str key = compare::cache_key_repr(S("(") + py::repr(KIND) + S(", ") + py::to_str(REV) + S(", ") + py::repr(image.digest) + S(", ") + py::to_str(entry) + S(", ") +
                                            (aligned ? S("True") : S("False")) + S(", ") + vector_repr(v) + S(")"));
    return lane_common::remembered(key, [&] { return std::get<0>(_run(image, entry, v, aligned)); });
}

std::optional<str> _mode() { return emulate::shadow::mode(platforms::vxd::PLATFORM().cpu.language); }

bool concolic_ready() {
    if (!(lane_common::guided_on() && lane_common::concolic_on())) return false;
    const std::optional<str> mode = _mode();
    return mode && emulate::pcode::lifter(*mode) != nullptr;
}

str _paths_profile() {
    const bool ready = concolic_ready();
    str stamp = S("''");
    if (ready) {
        auto s = emulate::pcode::stamp(*_mode());
        stamp = S("['") + std::get<0>(s) + S("', '") + std::get<1>(s) + S("']");
    }
    return S("[") + (ready ? S("True") : S("False")) + S(", ") + py::to_str(lane_common::PATH_BUDGET) + S(", ") + py::to_str(lane_common::MAX_PATHS) + S(", ") + stamp + S("]");
}

std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names) {
    Vector out = v;
    py::dict<str, int64_t> regs = v.regs.copy();
    py::list<int64_t> stack = v.stack.copy();
    int64_t flags = v.flags;
    py::list<std::tuple<int64_t, py::bytes>> memory = v.memory.copy();
    bool changed = false;
    static const char* reg_names[] = {"eax", "ebx", "ecx", "edx", "esi", "edi"};
    for (const auto& kv : values.items()) {
        const str& name = std::get<0>(kv);
        const int64_t value = std::get<1>(kv);
        if (!names.contains(name)) continue;
        const auto& var = names.at(name);
        const str& kind = std::get<0>(var);
        const int64_t where = std::get<1>(var), was = std::get<3>(var);
        if (kind == S("arg") && where < static_cast<int64_t>(stack.size())) {
            changed |= stack[where] != (value & M32);
            stack[where] = value & M32;
        } else if (kind == S("reg") && ((where >= 1 && where <= 6) || where == 9) && regs.contains(where == 9 ? S("ebp") : S(reg_names[where - 1]))) {
            const str reg = where == 9 ? S("ebp") : S(reg_names[where - 1]);
            changed |= regs.at(reg) != (value & M32);
            regs.set(reg, value & M32);
        } else if (kind == S("reg") && (where == 7 || where == 8)) {
            const int64_t bit = where == 7 ? 1 : 0x40;
            const int64_t nw = value ? (flags | bit) : (flags & ~bit);
            changed |= nw != flags;
            flags = nw;
        } else if (kind == S("ret")) {
            changed |= v.stub_ret.value_or(0) != (value & M32);
            out.stub_ret = value & M32;
        } else if (kind == S("carry")) {
            changed |= v.stub_carry != (value ? 1 : 0);
            out.stub_carry = value ? 1 : 0;
        } else if (kind == S("mem")) {
            changed |= was != (value & 0xFF);
            memory.append(std::make_tuple(where, py::bytes(std::string(1, static_cast<char>(value & 0xFF)))));
        }
    }
    if (!changed) return std::nullopt;
    out.regs = regs;
    out.stack = stack;
    out.flags = flags;
    out.memory = memory;
    py::list<int64_t> args;
    for (int64_t i = 0; i < 6; ++i) args.append(regs.at(ed::REGISTERS[i]));
    for (int64_t s : stack) args.append(s);
    out.args = args;
    return out;
}

std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(const ed::Image& image, int64_t entry, const py::list<Vector>& seeds) {
    const std::shared_ptr<emulate::pcode::Lifter> lifter = emulate::pcode::lifter(*_mode());
    emulate::shadow::Inputs scratch;
    scratch.append(std::make_tuple(ed::SCRATCH, ed::SCRATCH + emulate::emulator::_SCRATCH_SIZE));

    std::function<std::optional<concolic::Observation>(Vector&)> run = [&](Vector& v) -> std::optional<concolic::Observation> {
        std::shared_ptr<emulate::native_shadow::Shadow> sh = emulate::shadow::ShadowLE(lifter, scratch);
        const int64_t esp = emulate::emulator::_STACK_TOP - 4 * static_cast<int64_t>(v.stack.size()) - 4;
        py::list<emulate::native_shadow::Place> places;
        int64_t k = 0;
        for (int64_t w : v.stack) {
            emulate::native_shadow::Place p;
            p.where = esp + 4 + 4 * k;
            p.value = w & M32;
            p.k = std::variant<int64_t, str>(k);
            places.append(p);
            ++k;
        }
        for (int64_t i = 0; i < 6; ++i) {
            const str& n = ed::REGISTERS[i];
            if (!v.regs.contains(n)) continue;
            emulate::native_shadow::Place p;
            p.where = n.upper();
            p.value = v.regs.at(n) & M32;
            p.k = std::variant<int64_t, str>(n);
            places.append(p);
        }
        sh->enter(places, 32);
        py::list<emulate::native_shadow::Place> flag_places;
        {
            emulate::native_shadow::Place cf, zf;
            cf.where = S("CF");
            cf.value = v.flags & 1;
            cf.k = std::variant<int64_t, str>(S("cf"));
            zf.where = S("ZF");
            zf.value = (v.flags >> 6) & 1;
            zf.k = std::variant<int64_t, str>(S("zf"));
            flag_places.append(cf);
            flag_places.append(zf);
        }
        sh->enter(flag_places, 8);
        emulate::run::Run r(std::nullopt);
        try {
            r = ed::recorded(image, entry, v.regs, v.stack, v.memory, v.stub_ret.value_or(0), v.stub_carry, ed::INSN_CAP, v.flags, sh.get());
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception&) {
            return std::nullopt;
        }
        concolic::Observation ob;
        ob.trail = sh->trail();
        for (const auto& e : sh->path()) {
            concolic::PathEntry p;
            p.site = e.site;
            p.taken = e.taken;
            p.cond = e.cond;
            p.kind = e.kind;
            ob.path.append(p);
        }
        for (const auto& kv : sh->vars().items()) {
            const emulate::native_shadow::Var& var = std::get<1>(kv);
            int64_t where = 0;
            if (std::holds_alternative<int64_t>(var.where)) where = std::get<int64_t>(var.where);
            else if (std::holds_alternative<str>(var.where)) where = REG_CODE(std::get<str>(var.where));
            ob.vars.set(std::get<0>(kv), std::make_tuple(var.kind, where, var.bits, var.value));
        }
        for (const str& s : sh->lost()) ob.lost.add(s);
        const bool cut = r.truncated;
        if (cut) ob.lost.add(S("truncated"));
        ob.cut = cut;
        return ob;
    };
    std::function<std::optional<Vector>(const Vector&, const concolic::Values&, const concolic::Vars&)> derive =
        [](const Vector& v, const concolic::Values& values, const concolic::Vars& names) { return _derive(v, values, names); };
    auto got = std::make_shared<concolic::Exploration<Vector>>(lane_common::explored<Vector>(
        run, seeds, derive, [&image](int64_t a) { return image.holds(a); }, py::dict<int64_t, concolic::Trail>(), std::nullopt, static_cast<int64_t>(seeds.size())));
    return {got->kept.copy(), got};
}

str explain(const compare::Fingerprint& a, const Detail& da, const compare::Fingerprint& b, const Detail& db) {
    const str d = compare::difference_detail(a, b);
    if (d.startswith(S("writes"))) {
        auto same_memory = [&] {
            if (!da.memory && !db.memory) return true;
            if (!da.memory || !db.memory) return false;
            return std::get<0>(*da.memory) == std::get<0>(*db.memory) && std::get<1>(*da.memory) == std::get<1>(*db.memory);
        };
        if (!same_memory()) {
            const str ma = da.memory ? py::to_str(std::get<0>(*da.memory)) : S("?"), mb = db.memory ? py::to_str(std::get<0>(*db.memory)) : S("?");
            return py::percent(S("memory left behind differs: %s bytes changed vs %s"), ma, mb);
        }
        const py::list<Held>& ra = da.result;
        const py::list<Held>& rb = db.result;
        auto first = [](const py::list<Held>& r) { return r.empty() ? std::optional<Held>() : std::optional<Held>(r[0]); };
        auto eq = [](const std::optional<Held>& x, const std::optional<Held>& y) {
            if (!x && !y) return true;
            if (!x || !y) return false;
            return *x == *y;
        };
        if (!eq(first(ra), first(rb))) {
            if (ra.empty() || rb.empty()) throw py::IndexError("tuple index out of range");
            return py::percent(S("flags handed back differ: carry/zero %02x vs %02x"), std::get<int64_t>(ra[0]), std::get<int64_t>(rb[0]));
        }
        py::list<str> parts;
        const int64_t n = static_cast<int64_t>(ed::RESULT_REGISTERS.size());
        for (int64_t i = 0; i < n; ++i) {
            std::optional<Held> x = static_cast<int64_t>(ra.size()) > 1 + i ? std::optional<Held>(ra[1 + i]) : std::nullopt;
            std::optional<Held> y = static_cast<int64_t>(rb.size()) > 1 + i ? std::optional<Held>(rb[1 + i]) : std::nullopt;
            if (!eq(x, y))
                parts.append(py::percent(S("%s %s vs %s"), ed::RESULT_REGISTERS[i], x ? held_text(*x) : S("None"), y ? held_text(*y) : S("None")));
        }
        return S("registers handed back differ: ") + py::join(S(", "), parts);
    }
    return d;
}

// ---- inputs ------------------------------------------------------------------------------------------------------------------------------
py::list<std::tuple<int64_t, py::bytes>> _memory(const std::optional<int64_t>& fill) {
    const uint32_t f = static_cast<uint32_t>(fill ? *fill : ed::SAFE);
    std::string word(4, '\0');
    for (int i = 0; i < 4; ++i) word[static_cast<size_t>(i)] = static_cast<char>((f >> (8 * i)) & 0xFF);
    auto repeat = [&](size_t n) {
        std::string s;
        for (size_t i = 0; i < n; ++i) s += word;
        return py::bytes(s);
    };
    std::string seq;
    for (int i = 0; i < 256; ++i) seq.push_back(static_cast<char>(i));
    py::list<std::tuple<int64_t, py::bytes>> out;
    out.append(std::make_tuple(ed::VM, repeat(0x100)));
    out.append(std::make_tuple(ed::CLIENT, py::bytes(std::string(0x100, '\0'))));
    out.append(std::make_tuple(ed::BUFFER, py::bytes(seq + seq)));
    out.append(std::make_tuple(ed::OBJECT, repeat(0x400)));
    out.append(std::make_tuple(ed::SAFE, py::bytes(std::string(16, '\xc3'))));
    return out;
}

Vector _vector(const VectorArgs& a) {
    Vector v;
    v.regs.set(S("eax"), a.eax);
    v.regs.set(S("ebx"), a.ebx);
    v.regs.set(S("ecx"), a.ecx);
    v.regs.set(S("edx"), a.edx);
    v.regs.set(S("esi"), a.esi);
    v.regs.set(S("edi"), a.edi);
    v.regs.set(S("ebp"), ed::CLIENT);
    v.stack = a.stack;
    v.memory = _memory(a.fill);
    v.stub_ret = a.ret;
    v.stub_carry = a.carry;
    v.flags = a.flags;
    py::list<int64_t> args;
    for (int64_t i = 0; i < 6; ++i) args.append(v.regs.at(ed::REGISTERS[i]));
    for (int64_t s : a.stack) args.append(s);
    v.args = args;   // what the comparer prints
    return v;
}

namespace {

VectorArgs va(int64_t eax, int64_t ebx, int64_t ecx, int64_t edx, int64_t esi, int64_t edi, const std::vector<int64_t>& stack, std::optional<int64_t> fill = std::nullopt,
              int64_t ret = 0, int64_t carry = 0, int64_t flags = 1) {
    VectorArgs a;
    a.eax = eax, a.ebx = ebx, a.ecx = ecx, a.edx = edx, a.esi = esi, a.edi = edi;
    a.stack = py::list<int64_t>(stack);
    a.fill = fill;
    a.ret = ret, a.carry = carry, a.flags = flags;
    return a;
}

}  // namespace

std::tuple<py::list<Vector>, py::list<Vector>> inputs(const str& label) {
    using ed::BUFFER;
    using ed::OBJECT;
    using ed::VM;
    py::list<Vector> first, more;
    if (label == S("control")) {
        py::list<int64_t> msgs;
        {
            std::vector<int64_t> keys = symbols::driver_services::messages().keys();
            std::sort(keys.begin(), keys.end());
            for (int64_t m : keys) msgs.append(m);
        }
        if (msgs.empty())
            for (int64_t m = 0; m < 0x33; ++m) msgs.append(m);
        for (int64_t m : msgs) {
            VectorArgs a;
            a.eax = m;
            first.append(_vector(a));
        }
        auto ctl = [&](int64_t m, int64_t ret, int64_t carry, int64_t flags) {
            VectorArgs a;
            a.eax = m, a.ret = ret, a.carry = carry, a.flags = flags;
            more.append(_vector(a));
        };
        for (int64_t m : {0, 1, 2, 0x1B, 0x1C, 0x23}) ctl(m, 1, 0, 1);
        for (int64_t m : {0, 1, 2, 0x1B}) ctl(m, 0, 1, 1);
        ctl(0x70000000, 0, 0, 1);
        ctl(0xFFFFFFFF, 0, 0, 1);
        for (int64_t m : {0, 1, 2, 3, 0x1B, 0x23}) ctl(m, 0, 0, 0x40);
        return {first, more};
    }
    std::vector<int64_t> objs, plain, objs_stack, plain_stack;
    for (int64_t i = 0; i < 6; ++i) {
        objs.push_back(OBJECT + 0x40 * i);
        plain.push_back(BUFFER + 0x20 * i);
        plain_stack.push_back(BUFFER + 0x100 + 0x20 * i);
        objs_stack.push_back(OBJECT + 0x200 + 0x40 * i);
    }
    first.append(_vector(va(0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0})));
    first.append(_vector(va(1, 2, 3, 4, 5, 6, {7, 8, 9, 10, 11, 12})));
    first.append(_vector(va(plain[0], plain[1], plain[2], plain[3], plain[4], plain[5], plain_stack)));
    first.append(_vector(va(objs[0], objs[1], objs[2], objs[3], objs[4], objs[5], objs_stack)));
    first.append(_vector(va(0, VM, 0, 0, BUFFER, OBJECT, {VM, 0, 1, BUFFER, 0, 0})));
    auto o6 = [&](const std::vector<int64_t>& stack, std::optional<int64_t> fill, int64_t ret, int64_t carry, int64_t flags) {
        more.append(_vector(va(objs[0], objs[1], objs[2], objs[3], objs[4], objs[5], stack, fill, ret, carry, flags)));
    };
    o6(objs, std::nullopt, 1, 0, 1);
    o6(objs, std::nullopt, OBJECT + 0x800, 0, 1);
    o6(objs, std::nullopt, 0, 1, 1);
    more.append(_vector(va(1, 2, 3, 4, 5, 6, {7, 8, 9, 10, 11, 12}, std::nullopt, 1)));
    more.append(_vector(va(0xFFFFFFFF, 0xFFFFFFFE, 0x80000000, 0x7FFFFFFF, 0xFFFF, 0x100, {0xFFFFFFFF, 0x8000, 0xFF, 1, 0, 2})));
    o6(objs, int64_t(0xFFFFFFFF), 1, 0, 1);
    o6(objs, int64_t(3), 0, 0, 1);
    more.append(_vector(va(6, 5, 4, 3, 2, 1, {6, 5, 4, 3, 2, 1})));
    more.append(_vector(va(objs[0], 2, 3, objs[1], 5, 6, {objs[2], 1, 2, 3, 4, 5})));
    o6(objs, std::nullopt, 0, 0, 0x40);
    more.append(_vector(va(1, 2, 3, 4, 5, 6, {7, 8, 9, 10, 11, 12}, std::nullopt, 0, 0, 0)));
    return {first, more};
}

// ---- the kind interface ---------------------------------------------------------------------------------------------------------------------
std::tuple<str, str> profile(const std::optional<str>&) {
    py::list<str> tools;
    for (const auto& kv : platforms::vxd::tools().items()) {
        const str& exe = std::get<1>(kv);
        try {
            tools.append(py::percent(S("%s:%d:%d"), py::os::path::basename(exe), py::os::path::getsize(exe), static_cast<int64_t>(py::os::path::getmtime(exe))));
        } catch (const py::OSError&) {
            tools.append(S(""));
        }
    }
    auto [first, more] = inputs(S("control"));
    auto [sfirst, smore] = inputs(S("service_0"));
    std::string o = "{\"cap\": " + std::to_string(ed::INSN_CAP) + ", \"comparer\": " + std::to_string(compare::REV) + ", \"inputs\": [" + std::to_string(first.size()) + ", " +
                    std::to_string(more.size()) + ", " + std::to_string(sfirst.size()) + ", " + std::to_string(smore.size()) + "], \"judge\": " + std::to_string(REV) + ", \"kind\": ";
    py::json::put(o, KIND);
    o += ", \"paths\": ";
    {
        const bool ready = concolic_ready();
        o += "[" + std::string(ready ? "true" : "false") + ", " + std::to_string(lane_common::PATH_BUDGET) + ", " + std::to_string(lane_common::MAX_PATHS) + ", ";
        if (ready) {
            auto s = emulate::pcode::stamp(*_mode());
            o += "[";
            py::json::put(o, std::get<0>(s));
            o += ", ";
            py::json::put(o, std::get<1>(s));
            o += "]";
        } else {
            o += "\"\"";
        }
        o += "]";
    }
    o += ", \"segments\": [";
    bool firstseg = true;
    for (const auto& seg : build::driver::SEGMENT_CLASSES) {
        o += firstseg ? "" : ", ";
        firstseg = false;
        o += "[";
        py::json::put(o, std::get<0>(seg));
        o += ", ";
        py::json::put(o, std::get<1>(seg));
        o += "]";
    }
    auto sizes = symbols::driver_services::size();
    o += "], \"services\": [" + std::to_string(std::get<0>(sizes)) + ", " + std::to_string(std::get<1>(sizes)) + ", " + std::to_string(std::get<2>(sizes)) + "], \"tools\": [";
    bool firsttool = true;
    for (const str& t : tools) {
        o += firsttool ? "" : ", ";
        firsttool = false;
        py::json::put(o, t);
    }
    o += "]}";
    const str text(o);
    return {py::hashlib::sha1(text.encode("utf-8")).hexdigest().slice(0, 12), text};
}

str key_of(const formats::le::Place& where) { return py::percent(S("%d:%x"), std::get<0>(where), std::get<1>(where)); }

std::tuple<bool, bool, int64_t, str> _entry_order(const str& label) {
    return {label != S("control"), !label.startswith(S("service")), label.size(), label};
}

str source_folder(const str& module_path) {
    const str name = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).lower();
    const std::optional<str> env = py::os::environ_::get(S("CB_DRIVER_SOURCES"));
    const std::vector<str> roots{env && !env->empty() ? *env : S(""), py::os::path::join(py::os::path::dirname(base::corpus::design_dir()), S("vmm32"))};
    for (const str& root : roots)
        if (!root.empty() && py::os::path::isfile(py::os::path::join(root, name, name + S(".asm")))) return py::os::path::join(root, name);
    return S("");
}

reconstruct::Prepared prepare(const str& module_path, const str& work) {
    auto m = std::make_shared<Module>();
    const std::optional<str> env = py::os::environ_::get(S("CB_DRIVER_SOURCES"));
    m->source = env && !env->empty() ? source_folder(module_path) : S("");
    str text;
    auto read = [](const str& path) {
        py::file f = py::open(path, S("r"), std::optional<str>(S("latin1")));
        str t = f.read_text();
        f.close();
        return t;
    };
    if (!m->source.empty()) {
        text = read(py::os::path::join(m->source, py::os::path::basename(m->source) + S(".asm")));
    } else {
        m->written = source::driver_writer::reconstruct(module_path, py::os::path::join(work, S("source")));
        if (m->written->linked) text = read(m->written->source);
    }
    const py::dict<str, formats::le::Place> points = platforms::vxd::entries(module_path);
    py::set<std::tuple<int64_t, int64_t>> taken;
    std::vector<str> names = points.keys();
    std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return _entry_order(a) < _entry_order(b); });
    reconstruct::Prepared out;
    out.header = text;
    for (const str& n : names) {   // two labels, one procedure: judged once
        const auto where = std::make_tuple(std::get<0>(points.at(n)), std::get<1>(points.at(n)));
        if (!taken.contains(where)) {
            taken.add(where);
            out.order.append(n);
        }
    }
    // the unit judged is an entry point; its text is the whole assembly file (in the header)
    const str source_name = py::os::path::basename(!m->source.empty() ? m->source : m->written->source);
    for (const str& n : out.order) out.texts.set(n, !text.empty() ? py::percent(S("; entry point %s of %s"), n, source_name) : S(""));
    if (!text.empty())
        for (const str& n : out.order) out.key.set(n, key_of(points.at(n)));
    for (const str& n : out.order) out.all_keys.append(key_of(points.at(n)));
    {
        // sha1(("%d\0%s" % (REV, text)).encode("latin1", "replace"))
        std::string raw = std::to_string(REV);
        raw.push_back('\0');
        raw += text.encode("latin1", "replace").raw();
        out.sig = py::hashlib::sha1(py::bytes(raw)).hexdigest();
    }
    out.lane = m;
    return out;
}

reconstruct::Built build(const reconstruct::Prepared& p, const str& module_path, const str& work) {
    const auto m = std::static_pointer_cast<Module>(p.lane);
    reconstruct::Built out;
    auto ctx = std::make_shared<Context>();
    ctx->original = module_path;
    out.context = ctx;
    auto fail = [&](const str& why) {
        for (const str& n : p.order) out.dropped.set(n, why);
    };
    auto items = [&] {
        for (const str& n : p.order) out.items.set(n, std::make_shared<Item>(Item{p.key.at(n), n}));
    };
    if (m->written) {
        if (!m->written->linked) {
            fail((!m->written->notes.empty() ? m->written->notes[static_cast<int64_t>(m->written->notes.size()) - 1] : S("did not build")).slice(0, 200));
            return out;
        }
        ctx->rebuilt = m->written->image;
        items();
        return out;
    }
    build::driver::BuildResult r = build::driver::build(m->source, std::optional<str>(module_path), std::optional<str>(py::os::path::join(work, S("build"))));
    if (!r.linked) {
        py::list<str> lines = py::splitlines(r.log);
        fail((!lines.empty() ? lines[static_cast<int64_t>(lines.size()) - 1] : S("did not build")).slice(0, 200));
        return out;
    }
    ctx->rebuilt = r.path;
    items();
    return out;
}

void forget_image(const str& path) { images().erase(path.u32()); }

reconstruct::Verdict judge(const Context& context, const Item& item) {
    const str& original = context.original;
    const str& label = item.label;
    std::shared_ptr<ed::Image> a, b;
    try {
        a = image_of(original);
        if (!context.rebuilt) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
        b = image_of(*context.rebuilt);
    } catch (const py::Exception& e) {
        return {V::INCONCLUSIVE, py::percent(S("judge: %s: %s"), str(e.type()), str(e.what())), py::list<str>(), S("")};
    }
    if (!a->entries.contains(label)) return {V::INCONCLUSIVE, S("the original has no such entry point"), py::list<str>(), S("")};
    if (!b->entries.contains(label)) return {V::DIVERGENT, S("entry-missing"), py::list<str>(), py::percent(S("the rebuilt driver has no %s"), label)};
    const bool aligned = a->shape == b->shape;
    auto [first, more] = inputs(label);
    py::set<int64_t> ran;
    std::optional<Vector> last_a, last_b;
    auto side = [&](const std::shared_ptr<ed::Image>& image, int64_t entry, bool is_a) {
        return compare::FingerprintOf<Vector>([&, image, entry, is_a](const Vector& v) {
            compare::Fingerprint f = fingerprint(*image, entry, v, aligned);
            (is_a ? last_a : last_b) = v;
            if (is_a && !f.err)
                for (int64_t c : f.cov) ran.add(c);
            return f;
        });
    };
    std::shared_ptr<concolic::Exploration<Vector>> found;
    std::function<py::list<Vector>(const py::set<int64_t>&, const py::set<int64_t>&)> aimed = [&](const py::set<int64_t>&, const py::set<int64_t>&) {
        if (!concolic_ready()) return py::list<Vector>();
        py::list<Vector> seeds;
        for (const Vector& v : first) seeds.append(v);
        for (const Vector& v : more) seeds.append(v);
        auto [kept, got] = explore_vectors(*a, a->entries.at(label), seeds);
        found = got;
        return kept;
    };
    compare::Info info;
    str verdict, reason;
    py::list<str> limits;
    try {
        auto r = compare::compare<Vector>(side(a, a->entries.at(label), true), side(b, b->entries.at(label), false), first, more, 0, aimed, &info);
        verdict = std::get<0>(r), reason = std::get<1>(r), limits = std::get<2>(r);
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {   // one entry point must not sink the rest
        return {V::INCONCLUSIVE, py::percent(S("judge: %s: %s"), str(e.type()), str(e.what())), py::list<str>(), S("")};
    }
    py::list<str> lim = limits.copy();
    if (verdict != S("divergent"))
        for (const str& s : lane_common::path_limits(found.get())) lim.append(s);
    if (!aligned) lim.append(S("layout-differs"));
    str detail = info.detail.value_or(S(""));
    if (verdict == V::DIVERGENT && reason == S("writes-differ") && last_a && last_b) {
        // (the two fingerprints of the last input, made again with what explain() reads)
        auto [fa, da] = _run(*a, a->entries.at(label), *last_a, aligned);
        auto [fb, db] = _run(*b, b->entries.at(label), *last_b, aligned);
        const str ex = explain(fa, da, fb, db);
        detail = detail.contains(S("  [input")) ? ex + detail.slice_from(detail.find(S("  [input"))) : ex;
    }
    if (verdict != S("divergent")) detail = py::percent(S("%d instructions of the original ran"), static_cast<int64_t>(ran.size()));
    return {verdict, reason, lim, detail};
}

py::list<std::tuple<str, reconstruct::Verdict>> judge_files(const str& rebuilt, const str& original, const std::optional<py::list<str>>& labels) {
    auto a = image_of(original);
    std::vector<str> order = a->entries.keys();
    std::sort(order.begin(), order.end(), [](const str& x, const str& y) { return _entry_order(x) < _entry_order(y); });
    py::list<std::tuple<str, reconstruct::Verdict>> out;
    Context ctx{original, std::optional<str>(rebuilt)};
    for (const str& n : order) {
        if (labels && !labels->contains(n)) continue;
        reconstruct::Verdict v;
        try {
            v = judge(ctx, Item{n, n});
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception&) {
            v = {V::INCONCLUSIVE, S("judge: the emulator crashed on this entry point"), py::list<str>(), S("")};
        }
        out.append(std::make_tuple(n, v));
    }
    return out;
}

// ---- the judge judged ----------------------------------------------------------------------------------------------------------------------
std::optional<std::tuple<int64_t, str>> break_instruction(const str& original, const str& out, const str& label, int64_t nth) {
    auto members = formats::le::members(original);
    const formats::le::LEFile lf = std::get<1>(members[0]);
    auto image = image_of(original);
    if (!image->entries.contains(label) || formats::le::is_member(lf)) return std::nullopt;
    std::set<int64_t> cover;
    const auto first_inputs = inputs(label);   // (a name of its own: a range-for over std::get of a temporary would dangle)
    for (const Vector& v : std::get<0>(first_inputs)) {
        emulate::run::Run r = ed::recorded(*image, image->entries.at(label), v.regs, v.stack, v.memory, 0, 0, ed::INSN_CAP, v.flags);
        for (int64_t p : r.path) cover.insert(p);
    }
    const auto& md = authored::match::decoder(32);
    std::set<int64_t> patched;
    for (const auto& ref : formats::le::references(lf)) {
        const int64_t base = decompile::describe::le_address(std::get<0>(ref), std::get<1>(ref));
        for (int64_t i = 0; i < 4; ++i) patched.insert(base + i);
    }
    struct Picked {
        int64_t obj, off, size;
        str mnemonic, op_str;
    };
    std::vector<Picked> picked;
    for (int64_t address : cover) {
        std::optional<std::tuple<int64_t, int64_t, int64_t>> span;
        for (const auto& s : image->spans)
            if (std::get<0>(s) <= address && address < std::get<1>(s)) {
                span = s;
                break;
            }
        if (!span) throw py::RuntimeError("generator raised StopIteration");
        const int64_t lo = std::get<0>(*span), obj = std::get<2>(*span);
        std::optional<py::bytes> data;
        for (const auto& rg : image->regions)
            if (std::get<0>(rg) == lo) {
                data = std::get<1>(rg);
                break;
            }
        if (!data) throw py::RuntimeError("generator raised StopIteration");
        auto insns = md(data->slice(address - lo, address - lo + 15), address);
        if (insns.empty()) continue;
        const authored::match::Insn& ins = insns[0];
        if (ins.mnemonic == S("nop") || ins.mnemonic == S("int3") || patched.count(ins.address + ins.size - 1)) continue;
        picked.push_back({obj, address - lo, ins.size, ins.mnemonic, ins.op_str});
    }
    if (nth >= static_cast<int64_t>(picked.size())) return std::nullopt;
    const Picked& p = picked[static_cast<size_t>(nth)];
    auto at = formats::le::file_offset(lf, p.obj, p.off + p.size - 1);
    if (!at) return std::nullopt;
    std::string data = lf.d().raw();
    data[static_cast<size_t>(*at)] = static_cast<char>(static_cast<uint8_t>(data[static_cast<size_t>(*at)]) ^ (p.size > 1 ? 0x01 : 0x08));
    {
        py::file f = py::open(out, S("wb"));
        f.write(py::bytes(data));
        f.close();
    }
    return std::make_tuple(*at, (p.mnemonic + S(" ") + p.op_str).strip());
}

SelfCheck self_check(const str& original, const std::optional<str>& work_in, int64_t per_entry, int64_t entries) {
    str work = work_in && !work_in->empty() ? *work_in
                                            : py::os::path::join(py::settings::get_str(S("state")), S("driver_judge"),
                                                                 std::get<0>(py::os::path::splitext(py::os::path::basename(original))).lower());
    if (!work_in || work_in->empty()) py::os::makedirs(py::os::path::dirname(work), 0777, true);
    py::os::makedirs(work, 0777, true);
    auto own = judge_files(original, original);
    SelfCheck out;
    for (const auto& kv : own) {
        const str& verdict = std::get<0>(std::get<1>(kv));
        bool found = false;
        for (auto& p : out.self)
            if (std::get<0>(p) == verdict) {
                std::get<1>(p) += 1;
                found = true;
            }
        if (!found) out.self.emplace_back(verdict, int64_t(1));
    }
    std::vector<str> good;
    for (const auto& kv : own)
        if (std::get<0>(std::get<1>(kv)) == V::EQUIVALENT && static_cast<int64_t>(good.size()) < entries) good.push_back(std::get<0>(kv));
    const str ext = std::get<1>(py::os::path::splitext(original));
    size_t gi = 0;
    int64_t nth = 0;
    std::function<std::optional<teeth::Round>()> broken = [&]() -> std::optional<teeth::Round> {   // (a round each: one entry point, one break)
        while (gi < good.size()) {
            const str& label = good[gi];
            if (nth >= per_entry) {
                ++gi;
                nth = 0;
                continue;
            }
            const str copy = py::os::path::join(work, py::percent(S("broken_%s_%d%s"), label, nth, ext));
            auto changed = break_instruction(original, copy, label, nth);
            if (!changed) {
                ++gi;
                nth = 0;
                continue;
            }
            ++nth;
            forget_image(copy);
            return teeth::Round{std::make_tuple(label, std::get<1>(*changed), copy)};
        }
        return std::nullopt;
    };
    std::function<py::dict<str, str>(const teeth::Round&)> judged = [&](const teeth::Round& batch) {
        const auto& b = batch[0];
        py::dict<str, str> r;
        r.set(std::get<0>(b), std::get<0>(judge(Context{original, std::optional<str>(std::get<2>(b))}, Item{std::get<0>(b), std::get<0>(b)})));
        return r;
    };
    std::function<str(const str&)> outcome = [](const str& v) { return v == V::DIVERGENT ? S("caught") : S("missed"); };
    std::function<void(const std::vector<teeth::Planted<str>>&)> each = [&](const std::vector<teeth::Planted<str>>& round) {
        for (const auto& p : round) {
            out.broken += 1;
            if (p.outcome == S("caught")) out.caught += 1;
            else out.missed.append(std::make_tuple(p.name, p.what, p.said));
        }
    };
    teeth::plant<str>(broken, judged, outcome, each);
    return out;
}

const str DOC = S("python -m codebreaker driver-judge <rebuilt.vxd> <original.vxd> | --self-check <original.vxd>");

int64_t main(const py::list<str>& argv) {
    if (argv.size() == 2 && argv[0] == S("--self-check")) {
        const std::optional<str> path = base::corpus::retail(argv[1]);
        if (!path || path->empty()) {
            py::print(py::percent(S("%s: not found"), argv[1]));
            return 2;
        }
        SelfCheck r = self_check(*path);
        py::list<str> parts;
        std::vector<std::tuple<str, int64_t>> sorted = r.self;
        std::sort(sorted.begin(), sorted.end(), [](const auto& x, const auto& y) { return std::get<0>(x).u32() < std::get<0>(y).u32(); });
        for (const auto& kv : sorted) parts.append(py::percent(S("%s %d"), std::get<0>(kv), std::get<1>(kv)));
        py::print(py::percent(S("%s against itself: %s"), py::os::path::basename(*path), py::join(S(", "), parts)));
        py::print(py::percent(S("one instruction broken: %d copies, %d reported divergent"), r.broken, r.caught));
        for (const auto& m : r.missed) py::print(py::percent(S("    not caught: %s with `%s` changed -> %s"), std::get<0>(m), std::get<1>(m), std::get<2>(m)));
        auto count = [&](const str& v) {
            for (const auto& p : r.self)
                if (std::get<0>(p) == v) return std::get<1>(p);
            return int64_t(0);
        };
        return !count(V::DIVERGENT) && !count(V::INCONCLUSIVE) ? 0 : 1;
    }
    if (argv.size() != 2) {
        py::print(DOC);
        return 2;
    }
    const str rebuilt = argv[0];
    const std::optional<str> original = base::corpus::retail(argv[1]);
    if (!py::os::path::isfile(rebuilt) || !original || original->empty()) {
        py::print(py::percent(S("%s: not found"), py::os::path::isfile(rebuilt) ? argv[1] : rebuilt));
        return 2;
    }
    std::vector<std::tuple<str, int64_t>> counts;
    auto add = [&](const str& v) {
        for (auto& p : counts)
            if (std::get<0>(p) == v) {
                std::get<1>(p) += 1;
                return;
            }
        counts.emplace_back(v, 1);
    };
    for (const auto& kv : judge_files(rebuilt, *original)) {
        const auto& [verdict, reason, limits, detail] = std::get<1>(kv);
        add(verdict);
        py::print(py::percent(S("%-12s %-12s %s%s%s"), std::get<0>(kv), verdict, reason, !limits.empty() ? S("  [") + py::join(S(","), limits) + S("]") : S(""),
                              !detail.empty() ? S("  ") + detail : S("")));
    }
    std::sort(counts.begin(), counts.end(), [](const auto& x, const auto& y) { return std::get<0>(x).u32() < std::get<0>(y).u32(); });
    py::list<str> parts;
    for (const auto& kv : counts) parts.append(py::percent(S("%s %d"), std::get<0>(kv), std::get<1>(kv)));
    py::print(py::percent(S("%s vs %s: %s"), py::os::path::basename(rebuilt), py::os::path::basename(*original), parts.empty() ? S("no entry points") : py::join(S(", "), parts)));
    for (const auto& kv : counts)
        if (std::get<0>(kv) == V::DIVERGENT) return 1;
    return 0;
}

namespace {

namespace W = jobs::workers;

class DriverKind : public reconstruct::Kind {
  public:
    str KIND() const override { return driver::KIND; }
    std::tuple<str, str> profile(const str& module_path) const override { return driver::profile(module_path); }
    reconstruct::Prepared prepare(const str& module_path, const str& work) const override { return driver::prepare(module_path, work); }
    reconstruct::Built build(const reconstruct::Prepared& m, const str& module_path, const str& work) const override { return driver::build(m, module_path, work); }
    reconstruct::Verdict judge(const reconstruct::Context& context, const reconstruct::Item& item) const override {
        return driver::judge(*std::static_pointer_cast<Context>(context), *std::static_pointer_cast<Item>(item));
    }
    std::optional<std::string> encode_context(const reconstruct::Context& context) const override {
        const Context& c = *std::static_pointer_cast<Context>(context);
        return W::Enc().s(c.original.utf8()).i(c.rebuilt ? 1 : 0).s(c.rebuilt ? c.rebuilt->utf8() : "").text();
    }
    reconstruct::Context decode_context(const std::string& text) const override {
        W::Dec d(text);
        auto c = std::make_shared<Context>();
        c->original = str(d.s());
        const bool has = d.i() != 0;
        const str r(d.s());
        if (has) c->rebuilt = r;
        return c;
    }
    std::string encode_item(const reconstruct::Item& item) const override {
        const Item& it = *std::static_pointer_cast<Item>(item);
        return W::Enc().s(it.key.utf8()).s(it.label.utf8()).text();
    }
    reconstruct::Item decode_item(const std::string& text) const override {
        W::Dec d(text);
        auto it = std::make_shared<Item>();
        it->key = str(d.s());
        it->label = str(d.s());
        return it;
    }
};

}  // namespace

const std::shared_ptr<reconstruct::Kind>& kind() {
    static std::shared_ptr<reconstruct::Kind> k = std::make_shared<DriverKind>();
    return k;
}

void register_lane() { reconstruct::register_kind(KIND, kind()); }

}  // namespace codebreaker::verify::lanes::driver
