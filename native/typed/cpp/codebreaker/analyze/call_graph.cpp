// codebreaker/analyze/call_graph.cpp -- C++ port of codebreaker.analyze.call_graph (see call_graph.hpp).

#include "codebreaker/analyze/call_graph.hpp"

namespace codebreaker::analyze::call_graph {

const py::list<py::str> READS = {py::str("cmp"), py::str("test"), py::str("push"), py::str("bt"), py::str("call"), py::str("jmp"),
                                 py::str("lcall"), py::str("ljmp"), py::str("fld"), py::str("fild"), py::str("fcomp"), py::str("fcom"),
                                 py::str("fadd"), py::str("fsub"), py::str("fmul"), py::str("fdiv"), py::str("ficom"), py::str("ficomp"),
                                 py::str("bound"), py::str("div"), py::str("idiv"), py::str("mul")};
const py::list<py::str> PURE_WRITES = {py::str("mov"), py::str("pop"), py::str("fstp"), py::str("fst"), py::str("fistp"), py::str("fist"),
                                       py::str("lea"), py::str("movzx"), py::str("movsx"), py::str("sidt"), py::str("sgdt"),
                                       py::str("fstcw"), py::str("fnstcw"), py::str("fnstsw"), py::str("fstsw")};

py::list<std::shared_ptr<Function>> Image::each() {
    py::list<std::shared_ptr<Function>> out;
    py::list<int64_t> starts = functions.copy();   // list(self.functions): the loop may not see later changes
    for (int64_t start : starts) {
        out.append(function(start));
    }
    return out;
}

namespace {
// sorted(xs) of the ints in a set
std::vector<int64_t> sorted_ints(const py::set<int64_t>& xs) {
    std::vector<int64_t> out;
    for (int64_t x : xs) out.push_back(x);
    std::sort(out.begin(), out.end());
    return out;
}
}  // namespace

// The graph, worked out in C++ (native/analyze, call_graph.cc) over the image the DLL holds.
std::shared_ptr<Graph> _build(Image& img) {
    RawGraph got = img.ask_call_graph();
    std::shared_ptr<Graph> g = std::make_shared<Graph>();
    for (const auto& entry : got.calls) {
        py::list<Call> calls;
        for (const RawCall& c : std::get<1>(entry)) {
            calls.append(Call{c.site, c.kind, c.destination});
        }
        g->calls.set(std::get<0>(entry), calls);
    }
    for (const auto& entry : got.callers) {
        g->callers.set(std::get<0>(entry), std::get<1>(entry));
    }
    // by name: two addresses may stand for one import
    py::dict<py::str, py::set<int64_t>> users;
    for (const auto& entry : got.users) {
        const py::str& name = img.imports.at(std::get<0>(entry));
        py::set<int64_t> fs = users.setdefault(name, py::set<int64_t>());
        fs.update(std::get<1>(entry));
    }
    for (const auto& kv : users.items()) {
        std::vector<int64_t> sorted_fs = sorted_ints(std::get<1>(kv));
        g->users.set(std::get<0>(kv), py::list<int64_t>(sorted_fs));
    }
    for (const RawRef& r : got.refs) {
        std::shared_ptr<Function> fn = img.function(r.function);
        g->refs.append(Reference{r.function, fn->insns[fn->index.at(r.at)], r.va, r.how});
    }
    for (const auto& entry : got.taken) {
        py::list<Taken> sites;
        for (const RawTaken& t : std::get<1>(entry)) {
            sites.append(Taken{t.kind, t.function, t.at});
        }
        g->taken.set(std::get<0>(entry), sites);
    }
    g->order = got.order;
    g->cycles = got.cycles;
    g->reachable = py::set<int64_t>::from(got.reachable);
    g->reachable_with_tables = py::set<int64_t>::from(got.reachable_with_tables);
    g->unreferenced = got.unreferenced;
    return g;
}

std::shared_ptr<Graph> build(Image& img) {
    if (!img.memo_call_graph) {
        img.memo_call_graph = _build(img);
    }
    return img.memo_call_graph;
}

Analysis analyse(Image& img) {
    std::shared_ptr<Graph> g = build(img);
    py::dict<py::str, int64_t> kinds = {{py::str("call"), 0}, {py::str("import"), 0}, {py::str("tail"), 0}, {py::str("indirect"), 0}};
    int64_t edges = 0;
    py::list<Fact> facts;
    for (int64_t f : img.functions) {
        py::set<int64_t> callees;
        py::set<py::str> imports;
        int64_t indirect = 0;
        for (const Call& c : g->calls.at(f)) {
            kinds.set(c.kind, py::checked_add(kinds.at(c.kind), 1));   // (KeyError for a kind that is none of the four)
            if (!c.destination.has_value()) {
                indirect += 1;
            } else if (img.imports.contains(*c.destination)) {
                imports.add(img.imports.at(*c.destination));
            } else {
                callees.add(*c.destination);
            }
        }
        edges += callees.size();
        Fact fact;
        fact.addr = f;
        fact.kind = "calls";
        for (int64_t c : sorted_ints(callees)) {
            fact.callees.append(img.label(c));
        }
        std::vector<py::str> import_names(imports.begin(), imports.end());
        std::sort(import_names.begin(), import_names.end());
        for (const py::str& name : import_names) {
            fact.imports.append(name);
        }
        fact.indirect = indirect;
        if (g->callers.contains(f)) {
            for (int64_t c : g->callers.at(f)) {
                fact.callers.append(img.label(c));
            }
        }
        fact.address_taken = g->taken.contains(f) ? py::len(g->taken.at(f)) : 0;
        fact.reached = g->reachable.contains(f);
        facts.append(fact);
    }
    int64_t leaves = 0;
    for (int64_t f : img.functions) {
        if (g->calls.at(f).empty()) {
            leaves += 1;
        }
    }
    int64_t reached = 0;
    for (const std::shared_ptr<Function>& fn : img.each()) {
        for (const InsRef& i : fn->insns) {
            reached += i->size;
        }
    }
    int64_t held = 0;
    for (const Span& s : img.spans) {
        if (s.executable) {
            held += s.initialised;
        }
    }
    Counts counts;
    counts.functions = py::len(img.functions);
    counts.functions_found_from_calls = py::len(img.discovered);
    counts.edges = edges;
    counts.call_sites = kinds.at(py::str("call"));
    counts.import_call_sites = kinds.at(py::str("import"));
    counts.tail_jumps = kinds.at(py::str("tail"));
    counts.indirect_call_sites = kinds.at(py::str("indirect"));
    counts.imports_used = py::len(g->users);
    counts.leaf_functions = leaves;
    counts.recursive_groups = py::len(g->cycles);
    counts.address_taken_functions = py::len(g->taken);
    counts.data_references = py::len(g->refs);
    counts.roots = py::len(img.roots);
    counts.reached_from_roots = py::len(g->reachable);
    counts.reached_with_data_tables = py::len(g->reachable_with_tables);
    counts.unreferenced_functions = py::len(g->unreferenced);
    counts.code_bytes_decoded = reached;
    counts.code_section_bytes_not_reached = std::max<int64_t>(held - reached, 0);

    // top = sorted(g.callers.items(), key=lambda kv: -len(kv[1]))[:15]
    std::vector<std::tuple<int64_t, py::list<int64_t>>> top = g->callers.items();
    std::stable_sort(top.begin(), top.end(), [](const auto& a, const auto& b) { return -py::len(std::get<1>(a)) < -py::len(std::get<1>(b)); });
    if (top.size() > 15) top.resize(15);
    std::vector<std::tuple<py::str, py::list<int64_t>>> used = g->users.items();
    std::stable_sort(used.begin(), used.end(), [](const auto& a, const auto& b) { return -py::len(std::get<1>(a)) < -py::len(std::get<1>(b)); });
    if (used.size() > 15) used.resize(15);

    py::list<py::str> text = {py::str("most called:")};
    for (const auto& kv : top) {
        text.append(py::percent("  %-28s %d callers", img.label(std::get<0>(kv)), py::len(std::get<1>(kv))));
    }
    text.append(py::str("most used imports:"));
    for (const auto& kv : used) {
        text.append(py::percent("  %-36s %d functions", std::get<0>(kv), py::len(std::get<1>(kv))));
    }
    if (!g->cycles.empty()) {
        py::list<py::str> groups;
        py::list<py::list<int64_t>> first_groups = g->cycles.slice(0, 8);
        for (const py::list<int64_t>& c : first_groups) {
            py::list<py::str> names;
            for (int64_t f : c.slice(0, 4)) {
                names.append(img.label(f));
            }
            groups.append(py::join(py::str(","), names) + (py::len(c) > 4 ? py::str("...") : py::str("")));
        }
        text.append(py::str("recursive groups: ") + py::join(py::str("; "), groups));
    }
    if (!img.discovered.empty()) {
        py::list<py::str> names;
        std::vector<int64_t> found = sorted_ints(img.discovered);
        if (found.size() > 6) found.resize(6);
        for (int64_t f : found) {
            names.append(img.label(f));
        }
        text.append(py::percent("function starts found from calls (not in the decompiler's description): %d, e.g. %s",
                                py::len(img.discovered), py::join(py::str(", "), names)));
    }
    Analysis result;
    result.counts = counts;
    result.facts = facts;
    result.text = text;
    return result;
}

}  // namespace codebreaker::analyze::call_graph
