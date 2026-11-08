// codebreaker/verify/compare.cpp -- C++17 port of codebreaker.verify.compare (see compare.hpp).

#include "codebreaker/verify/compare.hpp"

#include <variant>

#include "py_os.hpp"

namespace codebreaker::verify::compare {

namespace cache = codebreaker::base::cache;

const int64_t REV = 25;

py::str first_difference(const Fingerprint& a, const Fingerprint& b, int64_t ret_mask) {
    // The original (a) cut off at the step limit did only part of its work, and where it stopped
    // is the harness's doing: the rebuild must have made every call it made by then, in order,
    // but how far either got is not comparable (a C loop takes more steps than `rep stosd`).
    // A rebuild that ENDED where the original was still going ended early, unless it had run more
    // steps than the original was given (`steps`, where the lane records it).
    if (a.exit == py::str("truncated") && b.exit != py::str("wild-call")) {
        const int64_t n = a.deps.size();   // (a list cut at its cap says so, "...", in both runs alike)
        bool same_start = b.deps.size() >= n;
        for (int64_t i = 0; same_start && i < n; ++i) {
            same_start = a.deps[i] == b.deps[i];
        }
        if (!same_start) {
            return py::str("deps-differ");
        }
        if (b.exit != py::str("truncated") && !(a.steps && *a.steps != 0 && b.steps.value_or(0) > *a.steps)) {
            return py::str("exit-differs");
        }
        return py::str("");
    }
    if (a.exit != b.exit) {
        return py::str("exit-differs");
    }
    if (!(a.deps == b.deps)) {
        return py::str("deps-differ");
    }
    // EAX is a return value only when the function returned: at a fault it holds whatever the
    // compiler happened to keep there, which says nothing about the function.
    if (a.exit == py::str("ret") && ((a.ret.value_or(0) ^ b.ret.value_or(0)) & ~ret_mask & 0xFFFFFFFF)) {
        return py::str("ret-differs");
    }
    // A run cut off at the step limit stopped in the middle of something: the two builds spend
    // different numbers of instructions per loop pass, so memory at that instant is not comparable.
    if (a.truncated || b.truncated) {
        return py::str("");
    }
    if (a.writes && b.writes && *a.writes != *b.writes) {
        return py::str("writes-differ");
    }
    if (a.pop && b.pop && *a.pop != *b.pop) {
        return py::str("stack-differs");   // a different calling convention or argument count
    }
    if (a.saved.value_or(false) && b.saved && !*b.saved) {
        return py::str("registers-differ");   // the original preserves them; this does not
    }
    return py::str("");
}

py::str difference_detail(const Fingerprint& a, const Fingerprint& b, const std::optional<Input>& v, int64_t ret_mask) {
    py::str d = first_difference(a, b, ret_mask);
    py::str out;
    auto before_colon = [](const std::optional<py::str>& w) {
        py::str t = (w && !w->empty()) ? *w : py::str("none");
        return py::split(t, py::str(":"))[0];
    };
    if (d == py::str("exit-differs")) {
        out = py::percent("exit %s vs %s", a.exit, b.exit);
    } else if (d == py::str("ret-differs")) {
        out = py::percent("ret %x vs %x", a.ret.value_or(0), b.ret.value_or(0));
    } else if (d == py::str("writes-differ")) {
        out = py::percent("writes %s vs %s", before_colon(a.writes), before_colon(b.writes));
    } else if (d == py::str("stack-differs")) {
        out = py::percent("removes %s bytes on return vs %s", *a.pop, *b.pop);
    } else if (d == py::str("registers-differ")) {
        out = py::str("saved registers preserved vs changed");
    } else if (d == py::str("deps-differ")) {
        const py::list<py::str>& da = a.deps;
        const py::list<py::str>& db = b.deps;
        int64_t i = 0;
        while (i < da.size() && i < db.size() && da[i] == db[i]) {
            ++i;
        }
        py::str ra = i < da.size() ? da[i] : py::str("(nothing)");
        py::str rb = i < db.size() ? db[i] : py::str("(nothing)");
        py::str na = py::split(ra, py::str("("))[0], nb = py::split(rb, py::str("("))[0];
        py::str kind;
        if (rb == py::str("(nothing)")) {
            kind = py::str("rebuilt stops calling");
        } else if (ra == py::str("(nothing)")) {
            kind = py::str("rebuilt calls extra");
        } else if (na != nb) {
            kind = py::str("different call");
        } else {
            py::list<py::str> aa = py::split(ra.slice_from(na.size()).strip(py::str("()")), py::str(","));
            py::list<py::str> ab = py::split(rb.slice_from(nb.size()).strip(py::str("()")), py::str(","));
            py::list<py::str> bad;
            for (int64_t k = 0; k < std::min(aa.size(), ab.size()); ++k) {
                if (aa[k] != ab[k]) {
                    bad.append(py::to_str(k));
                }
            }
            kind = py::percent("same call, argument %s differs", py::join(py::str("+"), bad));
        }
        out = py::percent("call #%d %s: %s vs %s", i, kind, ra, rb);
    } else {
        return py::str("");
    }
    if (v) {
        py::list<py::str> hexes;
        for (int64_t x : v->args) {
            hexes.append(py::percent("%x", x));
        }
        out = out + py::percent("  [input %s%s]", py::join(py::str(","), hexes),
                                (v->stub_ret && *v->stub_ret != 0) ? py::percent(" ret=%x", *v->stub_ret) : py::str(""));
    }
    return out.slice(0, 300);
}

// ---- the fingerprint in the cache: the Python dict {"ret", "exit", "truncated", "deps", "writes", "pop", "saved",
//      "cov", "reach"} (+ "steps", "err" where set), its sequences pickled as tuples as lane_common.fingerprint makes them ----
}  // namespace codebreaker::verify::compare

namespace codebreaker::base::cache::pickle {

template <> struct Codec<verify::compare::Fingerprint> {
    using F = verify::compare::Fingerprint;
    static void put_tuple(std::string& out, size_t n, const std::function<void(size_t)>& item) {
        if (n == 0) {
            put_op(out, ')');
            return;
        }
        put_mark(out);
        for (size_t i = 0; i < n; ++i) {
            item(i);
        }
        put_op(out, 't');
    }
    static void dump(std::string& out, const F& v) {
        put_op(out, '}');
        put_mark(out);
        auto key = [&](const char* k) { put_str(out, py::str(k)); };
        key("ret");
        Codec<std::optional<int64_t>>::dump(out, v.ret);
        key("exit");
        put_str(out, v.exit);
        key("truncated");
        Codec<bool>::dump(out, v.truncated);
        key("deps");
        put_tuple(out, v.deps.vec().size(), [&](size_t i) { put_str(out, v.deps.vec()[i]); });
        key("writes");
        Codec<std::optional<py::str>>::dump(out, v.writes);
        key("pop");
        Codec<std::optional<int64_t>>::dump(out, v.pop);
        key("saved");
        Codec<std::optional<bool>>::dump(out, v.saved);
        key("cov");
        put_tuple(out, v.cov.vec().size(), [&](size_t i) { put_int(out, v.cov.vec()[i]); });
        key("reach");
        put_tuple(out, v.reach.vec().size(), [&](size_t i) { put_int(out, v.reach.vec()[i]); });
        if (v.steps) {
            key("steps");
            put_int(out, *v.steps);
        }
        if (v.err) {
            key("err");
            put_str(out, *v.err);
        }
        put_op(out, 'u');
    }
    static bool strings(const Node& n, py::list<py::str>& out) {
        if (n.kind != Node::Tuple && n.kind != Node::List) return false;
        py::list<py::str> r;
        for (const Node& c : n.items) {
            if (c.kind != Node::Str) return false;
            r.append(c.s);
        }
        out = r;
        return true;
    }
    static bool ints(const Node& n, py::list<int64_t>& out) {
        if (n.kind != Node::Tuple && n.kind != Node::List) return false;
        py::list<int64_t> r;
        for (const Node& c : n.items) {
            if (c.kind != Node::Int) return false;
            r.append(c.i);
        }
        out = r;
        return true;
    }
    static bool load(const Node& n, F& v) {
        if (n.kind != Node::Dict || n.items.size() % 2 != 0) return false;
        F out;
        for (size_t i = 0; i + 1 < n.items.size(); i += 2) {
            const Node& k = n.items[i];
            const Node& x = n.items[i + 1];
            if (k.kind != Node::Str) return false;
            const std::string name = k.s.utf8();
            bool ok = false;
            if (name == "ret") ok = Codec<std::optional<int64_t>>::load(x, out.ret);
            else if (name == "exit") ok = Codec<py::str>::load(x, out.exit);
            else if (name == "truncated") ok = Codec<bool>::load(x, out.truncated);
            else if (name == "deps") ok = strings(x, out.deps);
            else if (name == "writes") ok = Codec<std::optional<py::str>>::load(x, out.writes);
            else if (name == "pop") ok = Codec<std::optional<int64_t>>::load(x, out.pop);
            else if (name == "saved") ok = Codec<std::optional<bool>>::load(x, out.saved);
            else if (name == "cov") ok = ints(x, out.cov);
            else if (name == "reach") ok = ints(x, out.reach);
            else if (name == "steps") ok = Codec<std::optional<int64_t>>::load(x, out.steps);
            else if (name == "err") ok = Codec<std::optional<py::str>>::load(x, out.err);
            if (!ok) return false;   // a field this port does not know (a lane's own): the run is made again
        }
        v = std::move(out);
        return true;
    }
};

}  // namespace codebreaker::base::cache::pickle

namespace codebreaker::verify::compare {

namespace {

// The cache names a kind's generation by the value of the module that owns it (cache.GENERATIONS: compare.REV).
void register_generation_() {
    if (!cache::GENERATION_REPRS.contains(py::str("fingerprints"))) {
        cache::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return py::repr(REV); });
    }
}

// repr() of one part of a key, as Python writes it
struct PartRepr {
    py::str operator()(const py::str& v) const { return py::repr(v); }
    py::str operator()(const py::bytes& v) const { return py::repr(v); }
    py::str operator()(int64_t v) const { return py::repr(v); }
    py::str operator()(double v) const { return py::repr(v); }
    py::str operator()(bool v) const { return py::repr(v); }
    py::str operator()(py::none_t) const { return py::str("None"); }
    py::str operator()(const py::list<py::str>& v) const { return py::repr(v); }
    py::str operator()(const py::list<int64_t>& v) const { return py::repr(v); }
};

}  // namespace

std::optional<Fingerprint> cache_get(const py::str& key) {
    register_generation_();
    return cache::value<Fingerprint>(py::str("fingerprints"), key);
}

void cache_put(const py::str& key, const Fingerprint& fp) {
    register_generation_();
    cache::keep<Fingerprint>(py::str("fingerprints"), key, fp);
}

py::str cache_key_repr(const py::str& parts_repr) {
    return py::percent("r%d:%s", REV, py::hashlib::sha1(parts_repr.encode("utf-8", "replace")).hexdigest());
}

py::str cache_key(const std::vector<cache::Part>& parts) {
    // repr() of the tuple of the arguments: (a, b), and (a,) for one
    py::str text = py::str("(");
    for (size_t i = 0; i < parts.size(); ++i) {
        text = text + (i ? py::str(", ") : py::str("")) + std::visit(PartRepr{}, parts[i]);
    }
    text = text + (parts.size() == 1 ? py::str(",)") : py::str(")"));
    return cache_key_repr(text);
}

}  // namespace codebreaker::verify::compare
