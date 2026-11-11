// codebreaker/analyze/globals.cpp -- COMPAT SHIM over cb/analyze/globals.hpp (docs/CPP_DESIGN.md 6.3), see globals.hpp.
// The variables and tables still come from the DLL until analyze/image is refactored; the report is the new module's.

#include "codebreaker/analyze/globals.hpp"

#include <algorithm>
#include <map>

#include "cb/analyze/globals.hpp"

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

// The report: the engine's rows (the DLL's, until analyze/image is refactored) as the new module's variables and tables,
// its summary and counts, and the old record shapes back.
Analysis analyse(Image& img) {
    namespace ns = ::cb::analyze::globals;
    py::dict<int64_t, Variable> var = variables(img);
    Tables tables = pointer_tables(img);
    std::map<::cb::analyze::Va, ns::Variable> new_vars;
    for (const auto& kv : var.items()) {
        const Variable& g = std::get<1>(kv);
        ns::Variable v;
        v.at = g.at;
        v.size = g.size;
        for (int64_t x : g.readers) v.readers.insert(x);
        for (int64_t x : g.writers) v.writers.insert(x);
        v.taken = g.taken;
        for (const py::str& n : g.notes) v.notes.insert(n.utf8());
        for (int64_t x : g.values) v.values.insert(x);
        v.writable = g.writable;
        v.element = g.element;
        v.extent = g.extent;
        if (std::holds_alternative<int64_t>(g.initial)) {
            v.initial = {ns::Initial::Kind::Value, static_cast<uint64_t>(std::get<int64_t>(g.initial)), {}};
        } else if (std::get<py::str>(g.initial) == py::str("uninitialised")) {
            v.initial = {ns::Initial::Kind::Uninitialised, 0, {}};
        } else {
            v.initial = {ns::Initial::Kind::Address, 0, std::get<py::str>(g.initial).utf8()};
        }
        v.type = g.type.utf8();
        new_vars.emplace(g.at, std::move(v));
    }
    std::vector<ns::PointerTable> new_tables;
    for (const RawTable& t : tables) {
        ns::PointerTable n;
        n.at = t.at;
        n.count = t.count;
        n.stride = t.stride;
        n.shape = t.shape == py::str("run") ? ns::PointerTable::Shape::Run : ns::PointerTable::Shape::Records;
        for (int64_t x : t.targets) n.targets.push_back(x);
        for (const py::str& k : t.kinds) {
            const std::string w = k.utf8();
            n.kinds.push_back(w == "function" ? ns::SlotKind::Function : w == "import" ? ns::SlotKind::Import
                              : w == "string" ? ns::SlotKind::String : w == "code" ? ns::SlotKind::Code : ns::SlotKind::Data);
        }
        new_tables.push_back(std::move(n));
    }
    const ns::Report report = ns::make_report(new_vars, new_tables, [&](::cb::analyze::Va a) { return img.label(a).utf8(); });

    Analysis out;
    const ns::Counts& c = report.counts;
    out.counts.globals = c.globals;
    out.counts.typed_beyond_size = c.typed_beyond_size;
    out.counts.pointers = c.pointers;
    out.counts.function_pointers = c.function_pointers;
    out.counts.strings = c.strings;
    out.counts.arrays = c.arrays;
    out.counts.counters = c.counters;
    out.counts.flags = c.flags;
    out.counts.results_of_system_calls = c.results_of_system_calls;
    out.counts.floats = c.floats;
    out.counts.uninitialised = c.uninitialised;
    out.counts.written_never_read = c.written_never_read;
    out.counts.read_never_written = c.read_never_written;
    out.counts.pointer_tables = c.pointer_tables;
    out.counts.record_tables = c.record_tables;
    out.counts.table_entries = c.table_entries;
    out.counts.function_table_entries = c.function_table_entries;
    for (const ns::GlobalFact& g : report.globals) {
        GlobalFact f;
        f.addr = std::nullopt;
        f.kind = "global";
        f.at = g.at;
        f.size = g.size;
        f.readers = g.readers;
        f.writers = g.writers;
        f.taken = g.taken;
        for (const std::string& n : g.notes) f.notes.append(py::str(n));
        for (int64_t x : g.values) f.values.append(x);
        f.writable = g.writable;
        f.extent = g.extent;
        f.initial = g.initial.kind == ns::Initial::Kind::Value ? Initial(static_cast<int64_t>(g.initial.value))
                    : g.initial.kind == ns::Initial::Kind::Address ? Initial(py::str(g.initial.label)) : Initial(py::str("uninitialised"));
        f.type = py::str(g.type);
        out.facts.append(Fact(f));
    }
    for (const ns::TableFact& t : report.tables) {
        PointerTableFact f;
        f.addr = std::nullopt;
        f.kind = "pointer_table";
        f.at = t.at;
        f.count = t.count;
        f.stride = t.stride;
        f.shape = py::str(std::string(ns::name_of(t.shape)));
        for (const auto& [kind, n] : t.holds) f.holds.set(py::str(kind), n);
        for (const std::string& l : t.first) f.first.append(py::str(l));
        out.facts.append(Fact(f));
    }
    for (const std::string& line : report.text) out.text.append(py::str(line));
    return out;
}

}  // namespace codebreaker::analyze::globals
