// codebreaker/analyze/globals.cpp -- typed C++ port of codebreaker.analyze.globals (see globals.hpp).

#include "codebreaker/analyze/globals.hpp"

#include <algorithm>

namespace codebreaker::analyze::globals {

namespace {

// s.split(sep)[0]: the text before the first sep
py::str before(const py::str& s, const char32_t* sep) {
    const std::u32string& u = s.u32();
    size_t at = u.find(sep);
    return py::str(at == std::u32string::npos ? u : u.substr(0, at));
}

// "word" in text
bool contains(const py::str& text, const char32_t* word) {
    return text.u32().find(word) != std::u32string::npos;
}

// _ask(img, "variables") and _ask(img, "tables") first make sure the strings table is read
// (strings.table(img)); the answers themselves come from the image.
py::list<RawVariable> ask_variables(Image& img) {
    strings::table(img);
    return img.ask_variables();
}

py::list<RawTable> ask_tables(Image& img) {
    strings::table(img);
    return img.ask_tables();
}

py::set<int64_t> int_set(const py::list<int64_t>& xs) {
    py::set<int64_t> out;
    for (int64_t x : xs) out.add(x);
    return out;
}

py::set<py::str> str_set(const py::list<py::str>& xs) {
    py::set<py::str> out;
    for (const py::str& x : xs) out.add(x);
    return out;
}

// sorted(xs)
py::list<int64_t> sorted_ints(const py::set<int64_t>& xs) {
    return py::sorted(xs);
}

}  // namespace

py::dict<int64_t, Variable> _variables(Image& img) {
    py::dict<int64_t, Variable> out;
    for (const RawVariable& g : ask_variables(img)) {
        Variable v;
        v.addr = std::nullopt;
        v.kind = "global";
        v.at = g.at;
        v.size = g.size;
        v.readers = int_set(g.readers);
        v.writers = int_set(g.writers);
        v.taken = g.taken;
        v.notes = str_set(g.notes);
        v.values = int_set(g.values);
        v.writable = g.writable;
        v.element = g.element;
        v.extent = g.extent;
        v.initial = g.initial;
        v.type = g.type;
        out.set(g.at, v);
    }
    return out;
}

py::dict<int64_t, Variable> variables(Image& img) {
    if (!img.memo_globals) {
        img.memo_globals = _variables(img);
    }
    return *img.memo_globals;
}

Tables pointer_tables(Image& img) {
    if (!img.memo_tables) {
        img.memo_tables = ask_tables(img);
    }
    return *img.memo_tables;
}

Analysis analyse(Image& img) {
    py::dict<int64_t, Variable> var = variables(img);
    Tables tables = pointer_tables(img);
    py::list<Fact> facts;
    py::dict<py::str, int64_t> by_type;
    for (int64_t va : sorted_ints(py::set<int64_t>::from(var.keys()))) {
        const Variable& g = var.get(va, Variable());
        GlobalFact f;
        f.addr = std::nullopt;
        f.kind = g.kind;
        f.at = g.at;
        f.size = g.size;
        f.readers = py::len(g.readers);
        f.writers = py::len(g.writers);
        f.taken = g.taken;
        py::list<py::str> notes = py::sorted(g.notes);
        f.notes = notes;
        py::list<int64_t> values = sorted_ints(g.values);
        py::list<int64_t> shown;
        for (int64_t i = 0; i < py::len(values) && i < 24; ++i) shown.append(values[i]);
        f.values = shown;
        f.writable = g.writable;
        f.extent = g.extent;
        f.initial = g.initial;
        f.type = g.type;
        facts.append(Fact(f));
        py::str key;
        if (!g.type.startswith(py::str("result of"))) {
            key = before(before(g.type, U"["), U" (");
        } else {
            key = py::str("result of a system call");
        }
        // g["type"][0] is looked at only when the type ends in "]", so it is not empty then
        if (g.type.endswith(py::str("]")) && (g.type.u32()[0] == U'c' || g.type.u32()[0] == U'w')) {
            key = py::str("string");
        }
        by_type.set(key, by_type.get(key, 0) + 1);
    }
    for (const RawTable& t : tables) {
        py::dict<py::str, int64_t> kinds;
        for (const py::str& k : t.kinds) {
            kinds.set(k, kinds.get(k, 0) + 1);
        }
        PointerTableFact f;
        f.addr = std::nullopt;
        f.kind = "pointer_table";
        f.at = t.at;
        f.count = t.count;
        f.stride = t.stride;
        f.shape = t.shape;
        f.holds = kinds;
        for (int64_t i = 0; i < py::len(t.targets) && i < 8; ++i) f.first.append(img.label(t.targets[i]));
        facts.append(Fact(f));
    }

    const py::list<py::str> plain = {py::str("unsigned char"), py::str("unsigned short"), py::str("unsigned long")};
    auto is_plain = [&](const py::str& k) {
        for (const py::str& p : plain) if (p == k) return true;
        return false;
    };
    Counts counts;
    counts.globals = py::len(var);
    for (const auto& kv : by_type.items()) {
        const py::str& k = std::get<0>(kv);
        int64_t n = std::get<1>(kv);
        if (!is_plain(k) && !contains(k, U"bytes")) counts.typed_beyond_size += n;
        if (k.endswith(py::str("]")) || contains(k, U"[]")) counts.arrays += n;
    }
    counts.pointers = by_type.get(py::str("pointer"), 0) + by_type.get(py::str("char *"), 0) +
                      by_type.get(py::str("far pointer"), 0);
    counts.function_pointers = by_type.get(py::str("function pointer"), 0);
    counts.strings = by_type.get(py::str("string"), 0);
    counts.counters = by_type.get(py::str("counter"), 0);
    counts.flags = by_type.get(py::str("flags"), 0);
    counts.results_of_system_calls = by_type.get(py::str("result of a system call"), 0);
    counts.floats = by_type.get(py::str("float"), 0) + by_type.get(py::str("double"), 0);
    for (const Variable& g : var.values()) {
        if (std::holds_alternative<py::str>(g.initial) && std::get<py::str>(g.initial) == py::str("uninitialised")) {
            counts.uninitialised += 1;
        }
        if (py::truthy(g.writers) && !py::truthy(g.readers) && !py::truthy(g.taken)) counts.written_never_read += 1;
        if (py::truthy(g.readers) && !py::truthy(g.writers) && !py::truthy(g.taken)) counts.read_never_written += 1;
    }
    for (const RawTable& t : tables) {
        if (t.shape == py::str("run")) counts.pointer_tables += 1;
        if (t.shape == py::str("records")) counts.record_tables += 1;
        counts.table_entries += t.count;
        for (const py::str& k : t.kinds) {
            if (k == py::str("function")) counts.function_table_entries += 1;
        }
    }

    py::list<py::str> text;
    {
        // sorted(by_type.items(), key=lambda kv: -kv[1]): stable, so equal counts keep their order of arrival
        auto items = by_type.items();
        std::stable_sort(items.begin(), items.end(),
                         [](const auto& a, const auto& b) { return -std::get<1>(a) < -std::get<1>(b); });
        py::str joined;
        bool first = true;
        for (const auto& kv : items) {
            joined = joined + py::str(first ? "" : ", ") + py::percent(py::str("%s %d"), std::get<0>(kv), std::get<1>(kv));
            first = false;
        }
        text.append(py::str("types: ") + joined);
    }
    {
        // sorted(var.values(), key=lambda g: -(len(readers) + len(writers)))[:12]
        auto all = var.values();
        std::stable_sort(all.begin(), all.end(), [](const Variable& a, const Variable& b) {
            return -(py::len(a.readers) + py::len(a.writers)) < -(py::len(b.readers) + py::len(b.writers));
        });
        int64_t shown = 0;
        for (const Variable& g : all) {
            if (shown++ >= 12) break;
            py::str tail;
            if (py::truthy(g.values)) {
                py::list<int64_t> vals = sorted_ints(g.values);
                py::str joined;
                for (int64_t i = 0; i < py::len(vals) && i < 8; ++i) {
                    joined = joined + py::str(i ? "," : "") + py::percent(py::str("%x"), vals[i]);
                }
                tail = py::str("  values ") + joined;
            }
            text.append(py::percent(py::str("  %x  %-28s read by %d, written by %d%s"), g.at, g.type,
                                    py::len(g.readers), py::len(g.writers), tail));
        }
    }
    {
        int64_t shown = 0;
        for (const RawTable& t : tables) {
            if (shown++ >= 8) break;
            py::list<py::str> kinds_seen = py::sorted(py::set<py::str>::from(t.kinds));
            py::str kinds_joined;
            for (int64_t i = 0; i < py::len(kinds_seen); ++i) kinds_joined = kinds_joined + py::str(i ? "/" : "") + kinds_seen[i];
            py::str labels;
            for (int64_t i = 0; i < py::len(t.targets) && i < 4; ++i) {
                labels = labels + py::str(i ? ", " : "") + img.label(t.targets[i]);
            }
            text.append(py::percent(py::str("  table %x: %d %s, e.g. %s"), t.at, t.count, kinds_joined, labels));
        }
    }

    Analysis a;
    a.counts = counts;
    a.facts = facts;
    a.text = text;
    return a;
}

}  // namespace codebreaker::analyze::globals
