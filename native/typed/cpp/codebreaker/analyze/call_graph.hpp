// codebreaker/analyze/call_graph.hpp -- COMPAT SHIM over cb/analyze/call_graph.hpp (docs/CPP_DESIGN.md 6.3), for the
// old codebreaker/analyze/call_graph.py port. Deleted with its last py:: consumer (analyze/image, structures, mcp_server).
//
// Who calls whom, what refers to what, and what the exports reach: the engine (the graph worked out from a decoded
// module, and the report of it) is native/cb/analyze/call_graph.hpp. The old callers hold the module as an
// `analyze::call_graph::Image` that the DLL bridge (analyze/native.hpp) answers questions about, so the graph still
// arrives as the DLL's answer (RawGraph) and is made into the old `Graph` here; the report is made by the new code
// from that graph (analyse converts to cb types, calls cb::analyze::call_graph::make_report, converts back).
//
//     g = call_graph::build(image)      g.calls[f]  g.callers[f]  g.refs  g.taken[f]  g.users[name]  g.order
//                                       g.reachable  g.reachable_with_tables   (see cb/analyze/call_graph.hpp)
//
// The image the graph is built over (codebreaker.analyze.image) has no typed port yet, so this header names the part
// of it call_graph uses (Image, Function, Ins, Span below) and the shape of the answer the C++ engine gives to
// ask("call_graph.build") (RawGraph).
#pragma once

#include <algorithm>
#include <memory>

#include "cb/analyze/call_graph.hpp"
#include "cb/compat/py_bridge.hpp"
#include "py.hpp"

namespace codebreaker::analyze::call_graph {

// (tuples of operation names in the Python)
inline const py::list<py::str> READS = {py::str("cmp"), py::str("test"), py::str("push"), py::str("bt"), py::str("call"), py::str("jmp"),
                                        py::str("lcall"), py::str("ljmp"), py::str("fld"), py::str("fild"), py::str("fcomp"), py::str("fcom"),
                                        py::str("fadd"), py::str("fsub"), py::str("fmul"), py::str("fdiv"), py::str("ficom"), py::str("ficomp"),
                                        py::str("bound"), py::str("div"), py::str("idiv"), py::str("mul")};
inline const py::list<py::str> PURE_WRITES = {py::str("mov"), py::str("pop"), py::str("fstp"), py::str("fst"), py::str("fistp"), py::str("fist"),
                                              py::str("lea"), py::str("movzx"), py::str("movsx"), py::str("sidt"), py::str("sgdt"),
                                              py::str("fstcw"), py::str("fnstcw"), py::str("fnstsw"), py::str("fstsw")};

// ---- the parts of codebreaker.analyze.image that call_graph reads ---------------------------------

// One instruction (the real class also has mn, ops, text, rd, wr, flow and target).
struct Ins {
    int64_t addr = 0;
    int64_t size = 0;
};
using InsRef = std::shared_ptr<Ins>;

// One decoded function: `insns` in address order; `index` maps an instruction's address to its position.
struct Function {
    py::list<InsRef> insns;
    py::dict<int64_t, int64_t> index;
};

// (va, bytes, executable, writable, name, initialised length)
struct Span {
    int64_t va = 0;
    py::bytes data;
    bool executable = false;
    bool writable = false;
    py::str name;
    int64_t initialised = 0;
};

// What ask("call_graph.build") answers, as the engine writes it.
struct RawCall {            // [site, kind, destination or null]
    int64_t site = 0;
    py::str kind;
    std::optional<int64_t> destination;
};
struct RawTaken {           // ["code", function, site] or ["data", null, address]
    py::str kind;
    std::optional<int64_t> function;
    int64_t at = 0;
};
struct RawRef {             // [function, instruction address, address, how]
    int64_t function = 0;
    int64_t at = 0;
    int64_t va = 0;
    py::str how;
};
struct RawGraph {
    py::list<std::tuple<int64_t, py::list<RawCall>>> calls;
    py::list<std::tuple<int64_t, py::list<int64_t>>> callers;
    py::list<std::tuple<int64_t, py::list<int64_t>>> users;     // by the address standing for the import
    py::list<RawRef> refs;
    py::list<std::tuple<int64_t, py::list<RawTaken>>> taken;
    py::list<int64_t> order;
    py::list<py::list<int64_t>> cycles;
    py::list<int64_t> reachable;
    py::list<int64_t> reachable_with_tables;
    py::list<int64_t> unreferenced;
};

struct Graph;

class Image {
  public:
    virtual ~Image() = default;

    py::list<int64_t> functions;
    py::dict<int64_t, py::str> imports;     // address standing for an import -> "MODULE!name"
    py::list<int64_t> roots;
    py::list<Span> spans;
    py::set<int64_t> discovered;            // function starts found here that the description did not have

    virtual RawGraph ask_call_graph() = 0;                       // img._handle().ask("call_graph.build")
    virtual std::shared_ptr<Function> function(int64_t start) = 0;  // the decoded function starting at `start`
    virtual py::str label(int64_t va) = 0;                       // a name for an address

    // every function, decoded (a generator in the Python)
    py::list<std::shared_ptr<Function>> each() {
        py::list<std::shared_ptr<Function>> out;
        py::list<int64_t> starts = functions.copy();   // list(self.functions): the loop may not see later changes
        for (int64_t start : starts) out.append(function(start));
        return out;
    }
    // img.memo("call_graph", build): worked out once per image
    std::shared_ptr<Graph> memo_call_graph;
};

// ---- the graph -----------------------------------------------------------------------------------

struct Call {               // (site, kind, destination)
    int64_t site = 0;
    py::str kind;
    std::optional<int64_t> destination;
};
struct Taken {              // ("code", function, site) | ("data", None, address)
    py::str kind;
    std::optional<int64_t> function;
    int64_t at = 0;
};
struct Reference {          // (function, instruction, address, how)
    int64_t function = 0;
    InsRef instruction;
    int64_t address = 0;
    py::str how;
};

struct Graph {
    py::dict<int64_t, py::list<Call>> calls;
    py::dict<int64_t, py::list<int64_t>> callers;
    py::list<Reference> refs;
    py::dict<int64_t, py::list<Taken>> taken;
    py::dict<py::str, py::list<int64_t>> users;
    py::list<int64_t> order;
    py::list<py::list<int64_t>> cycles;
    py::set<int64_t> reachable;
    py::list<int64_t> unreferenced;
    py::set<int64_t> reachable_with_tables;
};

// ---- what analyse reports --------------------------------------------------------------------------

struct Counts {
    int64_t functions = 0;
    int64_t functions_found_from_calls = 0;
    int64_t edges = 0;
    int64_t call_sites = 0;
    int64_t import_call_sites = 0;
    int64_t tail_jumps = 0;
    int64_t indirect_call_sites = 0;
    int64_t imports_used = 0;
    int64_t leaf_functions = 0;
    int64_t recursive_groups = 0;
    int64_t address_taken_functions = 0;
    int64_t data_references = 0;
    int64_t roots = 0;
    int64_t reached_from_roots = 0;
    int64_t reached_with_data_tables = 0;
    int64_t unreferenced_functions = 0;
    int64_t code_bytes_decoded = 0;
    int64_t code_section_bytes_not_reached = 0;
};
struct Fact {               // {"addr", "kind": "calls", "callees", "imports", "indirect", "callers", "address_taken", "reached"}
    int64_t addr = 0;
    py::str kind;
    py::list<py::str> callees;
    py::list<py::str> imports;
    int64_t indirect = 0;
    py::list<py::str> callers;
    int64_t address_taken = 0;
    bool reached = false;
};
struct Analysis {           // {"counts", "facts", "text"}
    Counts counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

// The graph, made from the answer of the engine's call_graph.build.
inline std::shared_ptr<Graph> _build(Image& img) {
    RawGraph got = img.ask_call_graph();
    std::shared_ptr<Graph> g = std::make_shared<Graph>();
    for (const auto& entry : got.calls) {
        py::list<Call> calls;
        for (const RawCall& c : std::get<1>(entry)) calls.append(Call{c.site, c.kind, c.destination});
        g->calls.set(std::get<0>(entry), calls);
    }
    for (const auto& entry : got.callers) g->callers.set(std::get<0>(entry), std::get<1>(entry));
    // by name: two addresses may stand for one import
    py::dict<py::str, py::set<int64_t>> users;
    for (const auto& entry : got.users) {
        const py::str& name = img.imports.at(std::get<0>(entry));
        py::set<int64_t> fs = users.setdefault(name, py::set<int64_t>());
        fs.update(std::get<1>(entry));
    }
    for (const auto& kv : users.items()) {
        std::vector<int64_t> sorted_fs(std::get<1>(kv).begin(), std::get<1>(kv).end());
        std::sort(sorted_fs.begin(), sorted_fs.end());
        g->users.set(std::get<0>(kv), py::list<int64_t>(sorted_fs));
    }
    for (const RawRef& r : got.refs) {
        std::shared_ptr<Function> fn = img.function(r.function);
        g->refs.append(Reference{r.function, fn->insns[fn->index.at(r.at)], r.va, r.how});
    }
    for (const auto& entry : got.taken) {
        py::list<Taken> sites;
        for (const RawTaken& t : std::get<1>(entry)) sites.append(Taken{t.kind, t.function, t.at});
        g->taken.set(std::get<0>(entry), sites);
    }
    g->order = got.order;
    g->cycles = got.cycles;
    g->reachable = py::set<int64_t>::from(got.reachable);
    g->reachable_with_tables = py::set<int64_t>::from(got.reachable_with_tables);
    g->unreferenced = got.unreferenced;
    return g;
}

inline std::shared_ptr<Graph> build(Image& img) {
    if (!img.memo_call_graph) img.memo_call_graph = _build(img);
    return img.memo_call_graph;
}

// The report: the old graph as cb's, the new code's make_report, and its answer as the old Analysis.
inline Analysis analyse(Image& img) {
    namespace ncg = ::cb::analyze::call_graph;
    std::shared_ptr<Graph> g = build(img);
    for (int64_t f : img.functions)   // (what the Python raised for a function the graph does not know)
        if (!g->calls.contains(f)) throw py::KeyError(std::to_string(f));
    ncg::Graph graph;
    for (const auto& kv : g->calls.items()) {
        std::vector<ncg::Call>& calls = graph.calls[std::get<0>(kv)];
        for (const Call& c : std::get<1>(kv)) {
            if (!(c.kind == py::str("call") || c.kind == py::str("import") || c.kind == py::str("tail") || c.kind == py::str("indirect")))
                throw py::KeyError(c.kind.utf8());   // (the Python's dict of counts has no such kind)
            ncg::CallKind kind = c.kind == py::str("call") ? ncg::CallKind::Call
                                 : c.kind == py::str("import") ? ncg::CallKind::Import
                                 : c.kind == py::str("tail") ? ncg::CallKind::Tail
                                                             : ncg::CallKind::Indirect;
            calls.push_back({c.site, kind, c.destination});
        }
    }
    for (const auto& kv : g->callers.items()) graph.callers[std::get<0>(kv)] = ::cb::compat::from_py(std::get<1>(kv));
    for (const auto& kv : g->taken.items()) graph.taken[std::get<0>(kv)].resize(static_cast<std::size_t>(py::len(std::get<1>(kv))));
    for (const auto& kv : g->users.items()) graph.users[std::get<0>(kv).utf8()] = ::cb::compat::from_py(std::get<1>(kv));
    graph.cycles = ::cb::compat::from_py(g->cycles);
    for (int64_t f : g->reachable) graph.reachable.insert(f);
    for (int64_t f : g->reachable_with_tables) graph.reachable_with_tables.insert(f);
    graph.refs.resize(static_cast<std::size_t>(py::len(g->refs)));
    graph.unreferenced = ::cb::compat::from_py(g->unreferenced);

    ncg::ReportInput in;
    in.functions = ::cb::compat::from_py(img.functions);
    in.roots = static_cast<std::size_t>(py::len(img.roots));
    for (int64_t f : img.discovered) in.discovered.push_back(f);
    for (const std::shared_ptr<Function>& fn : img.each())
        for (const InsRef& i : fn->insns) in.decoded_bytes += i->size;
    for (const Span& s : img.spans)
        if (s.executable) in.code_bytes_held += s.initialised;
    in.label = [&img](::cb::analyze::Va va) { return img.label(va).utf8(); };
    in.import_name = [&img](::cb::analyze::Va va) -> std::optional<std::string> {
        if (!img.imports.contains(va)) return std::nullopt;
        return img.imports.at(va).utf8();
    };
    const ncg::Report report = ncg::make_report(in, graph);

    Analysis out;
    const ncg::Counts& c = report.counts;
    out.counts = Counts{c.functions, c.functions_found_from_calls, c.edges, c.call_sites, c.import_call_sites, c.tail_jumps,
                        c.indirect_call_sites, c.imports_used, c.leaf_functions, c.recursive_groups, c.address_taken_functions,
                        c.data_references, c.roots, c.reached_from_roots, c.reached_with_data_tables, c.unreferenced_functions,
                        c.code_bytes_decoded, c.code_section_bytes_not_reached};
    for (const ncg::CallsFact& f : report.facts) {
        Fact fact;
        fact.addr = f.addr;
        fact.kind = py::str("calls");
        fact.callees = ::cb::compat::to_py(f.callees);
        fact.imports = ::cb::compat::to_py(f.imports);
        fact.indirect = f.indirect;
        fact.callers = ::cb::compat::to_py(f.callers);
        fact.address_taken = f.address_taken;
        fact.reached = f.reached;
        out.facts.append(fact);
    }
    out.text = ::cb::compat::to_py(report.text);
    return out;
}

}  // namespace codebreaker::analyze::call_graph
