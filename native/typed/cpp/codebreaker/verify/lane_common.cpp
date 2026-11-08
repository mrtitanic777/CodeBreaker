// codebreaker/verify/lane_common.cpp -- C++17 port of codebreaker.verify.lane_common (see lane_common.hpp).

#include "codebreaker/verify/lane_common.hpp"

#include <algorithm>

#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::lane_common {

namespace {

// os.environ.get(name): set and not empty
bool env_set_(const char* name) {
    std::optional<py::str> v = py::os::environ_::get(py::str(name));
    return v && !v->empty();
}

}  // namespace

// --- the switches -----------------------------------------------------------------------------
bool fp_cache_on() {
    return !env_set_("CB_NO_FP_CACHE");
}

bool guided_on() {
    return !env_set_("CB_NO_GUIDED");
}

bool concolic_on() {
    return !env_set_("CB_NO_CONCOLIC");
}

const bool PLAIN = env_set_("CB_PLAIN");

// --- fingerprints kept across runs --------------------------------------------------------------
compare::Fingerprint remembered(const std::optional<py::str>& key, const std::function<compare::Fingerprint()>& run,
                                const std::function<bool(const compare::Fingerprint&)>& keep) {
    if (!key || !fp_cache_on()) {
        return run();
    }
    std::optional<compare::Fingerprint> fp = compare::cache_get(*key);
    if (!fp) {
        fp = run();
        if (!fp->err && (!keep || keep(*fp))) {
            compare::cache_put(*key, *fp);
        }
    }
    return *fp;
}

// --- calls, as two builds agree on them ---------------------------------------------------------
py::str call(const py::str& name, const std::optional<py::list<py::str>>& args) {
    return args ? py::percent("%s(%s)", name, py::join(py::str(","), *args)) : name;
}

py::list<Called> called(const py::list<emulate::run::Call>& seq) {
    py::list<Called> out;
    for (const emulate::run::Call& c : seq) {
        out.append(Called{c.name, c.args});
    }
    return out;
}

py::list<py::str> calls(const py::list<Called>& seq, const std::function<py::str(int64_t)>& argument,
                        const std::function<py::str(const std::optional<py::str>&)>& name,
                        const std::function<bool(const std::optional<py::str>&)>& skip, const std::optional<int64_t>& cap) {
    py::list<py::str> out;
    for (const Called& c : seq) {
        if (skip && skip(c.name)) {
            continue;
        }
        if (cap && out.size() >= *cap) {
            out.append(py::str("..."));
            break;
        }
        py::str shown = name ? name(c.name) : (c.name ? *c.name : py::str("None"));
        std::optional<py::list<py::str>> args;
        if (c.args) {
            py::list<py::str> rendered;
            for (int64_t a : *c.args) {
                rendered.append(argument(a));
            }
            args = rendered;
        }
        out.append(call(shown, args));
    }
    return out;
}

py::str argument(int64_t v, const py::list<int64_t>& inputs, const std::optional<int64_t>& above, bool small, const py::list<Span>& frame,
                 const Image& image) {
    if (inputs.contains(v) && (!above || v > *above)) {
        return py::percent("a%d", inputs.index(v));
    }
    if (small) {
        int64_t sv = (v & 0x80000000) ? v - (int64_t(1) << 32) : v;
        if (-0x10000 < sv && sv < 0x10000) {
            return py::percent("%d", sv);
        }
    }
    for (const Span& span : frame) {
        if (span && std::get<0>(*span) <= v && v < std::get<1>(*span)) {
            return py::str("S");
        }
    }
    if (const auto* range = std::get_if<std::tuple<int64_t, int64_t>>(&image)) {
        if (std::get<0>(*range) <= v && v < std::get<1>(*range)) {
            return py::str("P");
        }
    } else if (const auto* test = std::get_if<std::function<bool(int64_t)>>(&image)) {
        if ((*test)(v)) {
            return py::str("P");
        }
    }
    return py::percent("%x", v);
}

namespace {

py::str repr_place_(const Place& p) {
    if (const auto* a = std::get_if<int64_t>(&p)) {
        return py::repr(*a);
    }
    const auto& t = std::get<std::tuple<py::str, int64_t>>(p);
    return py::str("(") + py::repr(std::get<0>(t)) + py::str(", ") + py::repr(std::get<1>(t)) + py::str(")");
}

py::str repr_held_(const Held& h) {
    return std::visit([](const auto& x) { return py::repr(x); }, h);
}

// the order sorted() puts places in: ints by value, (tag, offset) by tag then offset; the two kinds do not mix
bool place_less_(const Place& a, const Place& b) {
    if (a.index() != b.index()) {
        throw py::TypeError(a.index() == 1 ? "'<' not supported between instances of 'tuple' and 'int'"
                                           : "'<' not supported between instances of 'int' and 'tuple'");
    }
    if (a.index() == 0) {
        return std::get<0>(a) < std::get<0>(b);
    }
    const auto& x = std::get<1>(a);
    const auto& y = std::get<1>(b);
    if (std::get<0>(x).u32() != std::get<0>(y).u32()) {
        return std::get<0>(x).u32() < std::get<0>(y).u32();
    }
    return std::get<1>(x) < std::get<1>(y);
}

}  // namespace

py::str writes(const State& state) {
    if (state.empty()) {
        return py::str("");
    }
    std::vector<std::tuple<Place, Held>> items = state.items();
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return place_less_(std::get<0>(a), std::get<0>(b)); });
    py::str text = py::str("[");
    for (size_t i = 0; i < items.size(); ++i) {
        text = text + (i ? py::str(", ") : py::str("")) + py::str("(") + repr_place_(std::get<0>(items[i])) + py::str(", ") +
               repr_held_(std::get<1>(items[i])) + py::str(")");
    }
    text = text + py::str("]");
    return py::percent("%d:%s", state.size(), py::hashlib::sha1(text.encode("utf-8")).hexdigest().slice(0, 16));
}

// --- from a run record (emulate/run.py) to a fingerprint ----------------------------------------
State changed(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place,
              const std::function<std::optional<py::str>(int64_t)>& word, int64_t most, bool everything) {
    State state;
    py::dict<int64_t, std::optional<int64_t>> before;
    py::dict<Place, int64_t> at;
    const bool little = run.endian == py::str("little");
    for (const emulate::run::Write& w : run.writes) {
        const int64_t a = w.address, n = std::min(w.size, most), v = w.value;
        auto byte_of = [&](int64_t i) -> std::optional<int64_t> {
            if (!w.old) {
                return std::nullopt;
            }
            return (*w.old >> (8 * (little ? i : n - 1 - i))) & 0xFF;
        };
        if (everything) {
            for (int64_t i = 0; i < n; ++i) {
                before.setdefault(a + i, byte_of(i));
            }
        }
        if (w.harness) {
            continue;
        }
        std::optional<Where> where;
        if (place) {
            where = place(a);
        } else {
            where = Where(a);
        }
        if (!where) {
            continue;
        }
        std::optional<py::str> token = (word && n == 4) ? word(v) : std::nullopt;
        for (int64_t i = 0; i < n; ++i) {
            Place k;
            if (const auto* namer = std::get_if<ByteNamer>(&*where)) {
                std::optional<Place> named = (*namer)(i);
                if (!named) {
                    continue;
                }
                k = *named;
            } else if (const auto* base = std::get_if<int64_t>(&*where)) {
                k = Place(*base + i);
            } else {
                const auto& t = std::get<std::tuple<py::str, int64_t>>(*where);
                k = Place(std::make_tuple(std::get<0>(t), std::get<1>(t) + i));
            }
            if (!everything) {
                before.setdefault(a + i, byte_of(i));
            }
            state.set(k, token ? Held(*token) : Held((v >> (8 * (little ? i : n - 1 - i))) & 0xFF));
            at.setdefault(k, a + i);
        }
    }
    State out;
    for (const auto& [k, x] : state.items()) {
        const auto* num = std::get_if<int64_t>(&x);
        const std::optional<int64_t>& was = before.at(at.at(k));
        if (!num || !was || *num != *was) {   // (a token, or a byte that is not what it was)
            out.set(k, x);
        }
    }
    return out;
}

std::optional<py::str> left(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place,
                            const std::function<std::optional<py::str>(int64_t)>& word, int64_t most, bool everything) {
    if (run.capped) {
        return std::nullopt;
    }
    return writes(changed(run, place, word, most, everything));
}

compare::Fingerprint fingerprint(const emulate::run::Run& run, const py::list<py::str>& deps, const std::optional<int64_t>& ret,
                                 const std::optional<py::str>& writes_, const std::optional<bool>& saved, const py::list<int64_t>& cov,
                                 const py::list<int64_t>& reach, const std::optional<int64_t>& steps) {
    const bool done = run.ended() == py::str("ret");
    compare::Fingerprint fp;
    fp.ret = ret;
    fp.exit = run.ended();
    fp.truncated = run.truncated;
    fp.deps = deps;
    fp.writes = writes_;
    fp.pop = done ? run.stack : std::nullopt;
    fp.saved = done ? saved : std::nullopt;
    fp.cov = cov;
    fp.reach = reach;
    fp.steps = steps;
    return fp;
}

// --- inputs -------------------------------------------------------------------------------------
py::list<int64_t> counting(int64_t n, int64_t first, int64_t step) {
    py::list<int64_t> out;
    for (int64_t i = 0; i < n; ++i) {
        out.append(first + step * i);
    }
    return out;
}

py::list<int64_t> descending(int64_t n, int64_t top) {
    py::list<int64_t> out;
    for (int64_t i = 0; i < n; ++i) {
        out.append(top - i);
    }
    return out;
}

py::list<int64_t> alternating(int64_t n, const Pattern& even, const Pattern& odd) {
    auto at = [](const Pattern& f, int64_t i) { return std::holds_alternative<int64_t>(f) ? std::get<int64_t>(f) : std::get<1>(f)(i); };
    py::list<int64_t> out;
    for (int64_t i = 0; i < n; ++i) {
        out.append(i % 2 == 0 ? at(even, i) : at(odd, i));
    }
    return out;
}

py::list<guided::Input> guided(const guided::Run& run, const py::list<guided::Input>& seeds, int64_t seed, const py::str& byteorder,
                               int64_t word) {
    if (!guided_on()) {
        return py::list<guided::Input>();
    }
    return codebreaker::verify::guided::guided_inputs(run, seeds, GUIDED_BUDGET, MAX_GUIDED, seed, byteorder, word);
}

py::list<py::str> path_limits(const concolic::ExplorationStats* got) {
    py::list<py::str> out;
    if (got == nullptr) {
        return out;
    }
    out.append(py::percent("paths=%d", got->paths));
    if (got->complete) {
        out.append(py::str("paths-all"));
    }
    return out;
}

// --- the verdict --------------------------------------------------------------------------------
std::tuple<py::str, py::str, py::list<py::str>, py::str> judged(
    const std::function<std::tuple<py::str, py::str, py::list<py::str>>(compare::Info&)>& compare_one) {
    compare::Info info;
    py::str verdict, reason;
    py::list<py::str> limits;
    try {
        std::tie(verdict, reason, limits) = compare_one(info);
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {
        verdict = py::str(verdicts::INCONCLUSIVE);
        reason = py::percent("judge: %s", py::str(e.type()));
        limits = py::list<py::str>();
    } catch (const std::exception&) {
        verdict = py::str(verdicts::INCONCLUSIVE);
        reason = py::str("judge: Exception");
        limits = py::list<py::str>();
    }
    return {verdict, reason, limits.copy(), info.detail ? *info.detail : py::str("")};
}

}  // namespace codebreaker::verify::lane_common
