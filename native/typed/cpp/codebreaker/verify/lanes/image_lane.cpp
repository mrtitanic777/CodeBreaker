// codebreaker/verify/lanes/image_lane.cpp -- typed C++ port of codebreaker.verify.lanes.image_lane.

#include "codebreaker/verify/lanes/image_lane.hpp"

#include <algorithm>

#include "codebreaker/verify/verdicts.hpp"

namespace codebreaker::verify::lanes::image_lane {

namespace V = codebreaker::verify::verdicts;

const py::str SAME("the cartridge built from this source is the original, byte for byte");

namespace {

// sorted(...) of strings as Python orders them (by code point)
bool str_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

// repr of a state: {'a': 0, 'x': 2}
py::str repr_state(const State& s) {
    py::list<py::str> parts;
    for (const auto& kv : s.items()) parts.append(py::repr(std::get<0>(kv)) + py::str(": ") + py::to_str(std::get<1>(kv)));
    return py::str("{") + py::join(py::str(", "), parts) + py::str("}");
}

}  // namespace

Lane::Lane(const py::str& kind, int64_t rev, const py::list<State>& states, const EffectsFn& effects,
           const py::list<py::str>& what, const StartFn& start, const std::function<Writer()>& platform,
           const SubjectsFn& subjects, const py::str& key, const py::str& same)
    : KIND(kind), REV(rev), SAME(same), states_(states), effects_(effects), what_(what), start_(start), platform_(platform),
      subjects_(subjects), key_(key) {}

Writer Lane::writer() const {
    if (!platform_) throw py::RuntimeError("the lane's platform is not set");
    return platform_();
}

std::tuple<py::str, py::str> Lane::profile(const std::optional<py::str>& module_path) const {
    (void)module_path;
    // repr((self.KIND, self.REV, self._states))  (the states are part of what a verdict depends on)
    py::list<py::str> shown;
    for (const State& s : states_) shown.append(repr_state(s));
    const py::str settings = py::str("(") + py::repr(KIND) + py::str(", ") + py::to_str(REV) + py::str(", [") +
                             py::join(py::str(", "), shown) + py::str("])");
    return {py::hashlib::sha1(settings.encode()).hexdigest().slice(0, 12), settings};
}

Prepared Lane::prepare(const py::str& module_path, const py::str& work) const {
    Prepared m;
    m.written = writer()(module_path, py::os::path::join(work, py::str("source")));
    const Written& r = m.written;
    if (!r.source) throw py::KeyError("'source'");
    const py::str text = py::file::open(*r.source, py::str("r"), py::str("latin1")).read_text();
    if (!r.functions) throw py::KeyError("'functions'");
    const py::dict<int64_t, Function>& functions = *r.functions;
    m.header = text;
    std::vector<int64_t> addrs;
    for (const auto& kv : functions.items()) addrs.push_back(std::get<0>(kv));
    std::vector<int64_t> by_addr = addrs;
    std::sort(by_addr.begin(), by_addr.end());
    for (int64_t a : by_addr) m.order.append(functions.at(a).name);
    for (int64_t a : addrs) m.key.set(functions.at(a).name, py::percent(key_, a));
    std::vector<py::str> keys;
    for (int64_t a : addrs) keys.push_back(py::percent(key_, a));
    std::sort(keys.begin(), keys.end(), str_less);
    for (const py::str& k : keys) m.all_keys.append(k);
    const py::str base = py::os::path::basename(*r.source);
    for (const py::str& n : m.order) m.texts.set(n, py::percent(py::str("; %s, in %s"), n, base));
    for (int64_t a : addrs) m.by_name.set(functions.at(a).name, a);
    // hashlib.sha1(("%d\0%s" % (self.REV, text)).encode("latin1", "replace")).hexdigest()
    const py::str nul(std::u32string(1, U'\0'));
    m.sig = py::hashlib::sha1((py::percent(py::str("%d"), REV) + nul + text).encode("latin1", "replace")).hexdigest();
    return m;
}

Built Lane::build(const Prepared& m, const py::str& module_path, const py::str& work) const {
    (void)work;
    const Written& r = m.written;
    Built out;
    if (!(r.linked && *r.linked)) {
        py::str why = (r.notes && !r.notes->empty()) ? (*r.notes)[-1] : py::str("did not build");
        why = why.slice(0, 200);
        for (const py::str& n : m.order) out.missing.set(n, why);
        return out;
    }
    py::str original, rebuilt;
    if (subjects_) {
        std::tie(original, rebuilt) = subjects_(module_path, r);
    } else {
        if (!r.built) throw py::KeyError("'built'");
        original = module_path;
        rebuilt = *r.built;
    }
    if (!r.same) throw py::KeyError("'same'");
    out.context = Context{*r.same, original, rebuilt};
    for (const py::str& n : m.order) out.items.set(n, Item(m.key.at(n), m.by_name.at(n)));
    return out;
}

Verdict Lane::judge(const std::optional<Context>& context, const Item& item) const {
    if (!context) throw py::TypeError("cannot unpack non-iterable NoneType object");
    if (context->same) return {V::EQUIVALENT, py::str(""), py::list<py::str>(), SAME};
    const int64_t entry = std::get<1>(item);
    for (const State& state : states_) {
        const Effects a = effects_(context->original, entry, state);
        const Effects b = effects_(context->rebuilt, entry, state);
        if (!(a == b)) {
            // next(w for w, x, y in zip(self._what, a, b) if x != y)
            const size_t n = std::min<size_t>({static_cast<size_t>(what_.size()), a.size(), b.size()});
            for (size_t i = 0; i < n; ++i) {
                if (!(a[i] == b[i])) return {V::DIVERGENT, what_[static_cast<int64_t>(i)] + py::str("-differs"), py::list<py::str>(), start_(state)};
            }
            throw py::StopIteration();
        }
    }
    return {V::EQUIVALENT, py::str(""), py::list<py::str>(),
            py::percent(py::str("the same effects from %d register states"), static_cast<int64_t>(states_.size()))};
}

}  // namespace codebreaker::verify::lanes::image_lane
