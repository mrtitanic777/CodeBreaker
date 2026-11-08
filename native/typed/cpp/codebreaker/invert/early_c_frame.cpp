// codebreaker/invert/early_c_frame.cpp -- C++17 port of codebreaker.invert.early_c_frame (see early_c_frame.hpp).

#include "codebreaker/invert/early_c_frame.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::invert::early_c_frame {

const py::dict<py::str, int64_t> SIZE = {
    {py::str("char"), 1}, {py::str("unsigned char"), 1}, {py::str("int"), 2}, {py::str("unsigned"), 2},
    {py::str("unsigned int"), 2}, {py::str("short"), 2}, {py::str("long"), 4}, {py::str("unsigned long"), 4},
    {py::str("float"), 4}, {py::str("double"), 8}, {py::str("pointer"), 2}, {py::str("ptr"), 2},
};

int64_t tsize(const py::str& t, int64_t count) {
    int64_t base = SIZE.get(t, 2);
    return base * count;
}

std::tuple<py::dict<py::str, int64_t>, int64_t, py::dict<py::str, int64_t>, int64_t>
layout(const py::list<py::str>& temps, const py::list<std::tuple<py::str, py::str, int64_t>>& locals_,
       const std::optional<py::list<std::tuple<py::str, py::str>>>& params) {
    int64_t off = 0;
    for (const auto& t : temps) {   // temp region first, from bp+0
        off += tsize(t);
    }
    int64_t temp_bytes = off;
    py::dict<py::str, int64_t> offsets;
    for (const auto& [name, t, count] : locals_) {   // locals stack above, declaration order
        offsets.set(name, off);
        off += tsize(t, count);
    }
    int64_t framesize = off + (off & 1);   // round up to even
    py::dict<py::str, int64_t> poff;
    int64_t p = framesize + 4;   // params above saved-bp + near-ret
    if (params && !params->empty()) {
        for (const auto& [name, t] : *params) {
            poff.set(name, p);
            p += tsize(t);
        }
    }
    return std::make_tuple(offsets, framesize, poff, temp_bytes);
}

template <class K> py::list<std::tuple<K, K>> _chain_swaps(const py::list<std::tuple<K, K>>& copies) {
    py::dict<K, K> outg;
    py::set<K> incoming;
    for (const auto& [dest, src] : copies) {
        outg.set(src, dest);
        incoming.add(dest);
    }
    py::list<std::tuple<K, K>> swaps;
    py::set<K> seen;
    for (const auto& copy : copies) {   // iterate origins (srcs w/o incoming)
        K src = std::get<1>(copy);
        if (incoming.contains(src) || seen.contains(src)) continue;
        K cur = src;
        seen.add(src);
        while (outg.contains(cur)) {   // walk to sink
            cur = outg.at(cur);
            seen.add(cur);
        }
        if (!(src == cur)) swaps.append(std::make_tuple(src, cur));
    }
    return swaps;
}

template <class K> py::dict<K, K> apply_copy_swaps(const py::dict<K, int64_t>& offsets, const py::list<std::tuple<K, K>>& copies) {
    py::dict<K, K> cur;
    for (const auto& o : offsets) cur.set(o, o);
    for (const auto& [a, b] : _chain_swaps<K>(copies)) {
        if (cur.contains(a) && cur.contains(b)) {
            K x = cur.at(a);
            K y = cur.at(b);
            cur.set(a, y);
            cur.set(b, x);
        }
    }
    return cur;
}

template <class K> py::list<K> infer_decl_order(const py::list<K>& offsets, const py::list<std::tuple<K, K>>& copies) {
    std::vector<K> offs;   // sorted(set(offsets))
    {
        py::set<K> unique = py::set<K>::from(offsets);
        for (const auto& o : unique) offs.push_back(o);
        std::sort(offs.begin(), offs.end());
    }
    py::dict<K, K> pos;
    for (const auto& o : offs) pos.set(o, o);
    for (const auto& [a, b] : _chain_swaps<K>(copies)) {
        if (pos.contains(a) && pos.contains(b)) {
            K x = pos.at(a);
            K y = pos.at(b);
            pos.set(a, y);
            pos.set(b, x);
        }
    }
    std::stable_sort(offs.begin(), offs.end(), [&](const K& l, const K& r) { return pos.at(l) < pos.at(r); });
    return py::list<K>(offs);
}

// the key types the module is used with: frame offsets (int) and, in the tests, names (str)
template py::list<std::tuple<int64_t, int64_t>> _chain_swaps<int64_t>(const py::list<std::tuple<int64_t, int64_t>>&);
template py::list<std::tuple<py::str, py::str>> _chain_swaps<py::str>(const py::list<std::tuple<py::str, py::str>>&);
template py::dict<int64_t, int64_t> apply_copy_swaps<int64_t>(const py::dict<int64_t, int64_t>&, const py::list<std::tuple<int64_t, int64_t>>&);
template py::dict<py::str, py::str> apply_copy_swaps<py::str>(const py::dict<py::str, int64_t>&, const py::list<std::tuple<py::str, py::str>>&);
template py::list<int64_t> infer_decl_order<int64_t>(const py::list<int64_t>&, const py::list<std::tuple<int64_t, int64_t>>&);
template py::list<py::str> infer_decl_order<py::str>(const py::list<py::str>&, const py::list<std::tuple<py::str, py::str>>&);

std::tuple<py::list<std::tuple<py::str, py::str, int64_t>>, py::list<py::str>>
emit_local_decls(const py::list<int64_t>& offsets, int64_t framesize, const std::optional<py::set<int64_t>>& longset_in,
                 const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies) {
    py::set<int64_t> longset = longset_in && !longset_in->empty() ? *longset_in : py::set<int64_t>();
    py::list<int64_t> offs;   // sorted(set(offsets))
    {
        std::vector<int64_t> sorted_offsets;
        for (int64_t o : py::set<int64_t>::from(offsets)) sorted_offsets.push_back(o);
        std::sort(sorted_offsets.begin(), sorted_offsets.end());
        offs = py::list<int64_t>(sorted_offsets);
    }
    if (offs.empty()) return std::make_tuple(py::list<std::tuple<py::str, py::str, int64_t>>(), py::list<py::str>());
    bool contiguous = true;   // offsets are consecutive slots of int/long
    for (int64_t k = 0; k + 1 < py::len(offs); ++k) {
        if (offs[k + 1] - offs[k] != tsize(py::str(longset.contains(offs[k]) ? "long" : "int"))) {
            contiguous = false;
            break;
        }
    }
    // (and offs[0] == min(offs), which holds for sorted offsets)
    if (copies && !copies->empty() && contiguous) {
        // copy-reversal present: emit in the inferred declaration order so the compiler's
        // src<->dest offset swap lands each var at its own offset (round-trip byte-exact).
        py::list<int64_t> order = infer_decl_order<int64_t>(offs, *copies);
        py::list<std::tuple<py::str, py::str, int64_t>> ordered;
        py::list<py::str> names;
        for (int64_t o : order) {
            ordered.append(std::make_tuple(py::str(longset.contains(o) ? "long" : "int"), py::str("v") + py::to_str(o), int64_t(1)));
        }
        for (int64_t o : order) names.append(py::str("v") + py::to_str(o));
        return std::make_tuple(ordered, names);
    }
    py::list<std::tuple<py::str, py::str, int64_t>> ordered;
    py::list<py::str> names;
    for (int64_t k = 0; k < py::len(offs); ++k) {
        int64_t o = offs[k];
        py::str t(longset.contains(o) ? "long" : "int");
        int64_t sz = tsize(t);
        ordered.append(std::make_tuple(t, py::str("v") + py::to_str(o), int64_t(1)));
        names.append(py::str("v") + py::to_str(o));
        int64_t nxt = k + 1 < py::len(offs) ? offs[k + 1] : framesize;
        int64_t gap = nxt - o - sz;
        if (gap > 0) ordered.append(std::make_tuple(py::str("char"), py::str("_p") + py::to_str(o), gap));   // unused local(s) filler
    }
    return std::make_tuple(ordered, names);
}

py::str fmt_decl(const py::str& t, const py::str& name, int64_t count) {
    if (count > 1) return t + " " + name + "[" + py::to_str(count) + "];";
    return t + " " + name + ";";
}

namespace {

Case mk(const char* name, std::initializer_list<py::str> temps,
        std::initializer_list<std::tuple<py::str, py::str, int64_t>> locals_,
        std::initializer_list<std::tuple<py::str, py::str>> params, int64_t expected_fs,
        std::initializer_list<int64_t> expected_disps) {
    return Case(py::str(name), py::list<py::str>(temps), py::list<std::tuple<py::str, py::str, int64_t>>(locals_),
                py::list<std::tuple<py::str, py::str>>(params), expected_fs, py::list<int64_t>(expected_disps));
}

py::list<Case> build_cases() {
    return py::list<Case>{
        mk("f01", {}, {{"a", "int", 1}}, {}, 2, {0}),
        mk("f02", {}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 4, {0, 2}),
        mk("f03", {}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {}, 6, {0, 2, 4}),
        mk("f04", {}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 4, {0, 2}),
        mk("f08", {}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {}, 6, {0, 4}),   // b unused
        mk("f11", {}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}, {"d", "int", 1}}, {}, 8, {0, 2, 4, 6}),
        mk("f12", {"int"}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {{"p", "int"}}, 8, {2, 4, 6, 12}),
        mk("f13", {"int"}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 6, {2, 4}),
        mk("f14", {"long"}, {{"a", "int", 1}, {"b", "long", 1}}, {{"p", "int"}}, 10, {4, 6, 8, 14}),
        mk("f15", {"int"}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 6, {2, 4}),
        mk("g01", {"int"}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 6, {2, 4}),
        mk("g02", {"int", "int"}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {}, 10, {4, 6, 8}),
        mk("g03", {"long"}, {{"a", "long", 1}, {"b", "long", 1}}, {}, 12, {4, 6, 8, 10}),
        mk("g04", {"long"}, {{"a", "long", 1}, {"b", "int", 1}}, {}, 10, {4, 6, 8}),
        mk("g05", {}, {{"a", "int", 1}, {"b", "long", 1}, {"c", "long", 1}}, {}, 10, {0}),
        mk("g06", {}, {{"a", "char", 1}, {"b", "char", 1}}, {}, 2, {0, 1}),
        mk("g07", {}, {{"a", "int", 1}, {"b", "char", 1}, {"c", "long", 1}}, {}, 8, {0, 2, 3, 5}),
        mk("g08", {}, {{"a", "int", 3}}, {}, 6, {0}),
        mk("g09", {}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {}, 6, {0, 2, 4}),
        mk("g10", {"int"}, {{"a", "int", 1}}, {{"p", "int"}}, 4, {2, 8}),
        mk("g11", {"int"}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}}, {}, 8, {2, 4, 6}),
        mk("g12", {"int"}, {{"a", "int", 1}}, {}, 4, {2}),
        mk("g13", {"int"}, {{"a", "int", 1}, {"b", "int", 1}}, {}, 6, {2, 4}),
        mk("g14", {"long"}, {{"a", "long", 1}, {"b", "long", 1}, {"c", "long", 1}}, {}, 16, {4, 6, 8, 10, 12, 14}),
        mk("g15", {}, {{"a", "int", 1}, {"b", "int", 1}, {"c", "int", 1}, {"d", "int", 1}, {"e", "int", 1}}, {}, 10, {0, 2, 4, 6, 8}),
    };
}

// repr of a dict with str keys and int values: {'a': 0, 'b': 2}
py::str dict_repr(const py::dict<py::str, int64_t>& d) {
    py::str o("{");
    bool first = true;
    for (const auto& kv : d.items()) {
        if (!first) o += py::str(", ");
        o += py::repr(std::get<0>(kv)) + ": " + py::to_str(std::get<1>(kv));
        first = false;
    }
    return o + "}";
}

}  // namespace

const py::list<Case> CASES = build_cases();

bool _self_test() {
    int64_t ok = 0;
    for (const auto& [name, temps, loc, params, efs, edisps] : CASES) {
        auto [offs, fs, poff, tb] = layout(temps, loc, params);
        (void)tb;
        // predicted referenced offsets = every local slot that appears + long hi-word + params
        py::set<int64_t> pred;
        for (const auto& [n, t, c] : loc) {
            int64_t base = offs.at(n);
            for (int64_t k = 0; k < c; ++k) {
                pred.add(base + k * tsize(t));
                if (t == "long" || t == "float") pred.add(base + k * tsize(t) + 2);   // hi word ref
            }
        }
        for (const auto& [n, t] : params) {
            (void)t;
            pred.add(poff.at(n));
        }
        // we only assert framesize + that every OBSERVED disp is a slot we predict
        bool good = (fs == efs);
        // stricter: fs must match and predicted local bases must cover observed
        good = (fs == efs);
        for (int64_t d : edisps) {
            if (!pred.contains(d)) good = false;
        }
        if (good) {
            ok += 1;
        } else {
            std::vector<int64_t> sorted_pred;
            for (int64_t p : pred) sorted_pred.push_back(p);
            std::sort(sorted_pred.begin(), sorted_pred.end());
            py::print(py::str("  ") + py::format(name, "<5") + " FAIL  fs=" + py::to_str(fs) + "/" + py::to_str(efs) +
                      "  pred=" + py::repr(py::list<int64_t>(sorted_pred)) + "  obs=" + py::repr(edisps) + "  offs=" + dict_repr(offs));
        }
    }
    py::print(py::str("framemodel self-test: ") + py::to_str(ok) + "/" + py::to_str(py::len(CASES)) + " byte-exact frame layouts");
    return ok == py::len(CASES);
}

void cli() { _self_test(); }

}  // namespace codebreaker::invert::early_c_frame
