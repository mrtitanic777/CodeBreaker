// codebreaker/analyze/observe.hpp -- COMPAT SHIM over cb/analyze/observe.hpp (docs/CPP_DESIGN.md 6.3), for the callers that still hold an old
// analyze::image::Image. Deleted with its last py:: consumer. Conversion and forwarding only: the runs and the analysis are
// native/cb/analyze/observe.hpp, over the cb Image and call graph the bridge (analyze/cb_image.hpp) makes of the old image, with
// the calling-convention facts and structures the cb passes give.
//
// observe.py -- what running a function shows that reading it does not.
//
//     auto img = image::load("comctl32.dll");
//     auto seen = observe::run(*img, 400);                  // {function: observation}
//
// Each function is run in the emulator with every argument pointing at a buffer of its own, filled so that any value read from
// it says which buffer and which offset it came from; the observation is what it touched through each pointer, where its
// indirect calls went, what it handed to the system, what it removed and returned, and -- for the small functions that loop and
// call nothing -- the C library function it behaves as on a battery of test inputs. 32-bit modules only.
//
// What differs from the port it replaces: the internals the port exposed (_fill, _where, _digest, _case, _idiom, _observe ...)
// are not kept, nothing but run() and analyse() was called from outside; the imports' argument counts come from the old
// api_arguments shim, and the emulator is given no headers, so an import gives nothing back through a pointer; the offsets of a
// structure_observed fact are sorted as the "+%x" texts, as before.
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "cb/analyze/observe.hpp"
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "py.hpp"

namespace codebreaker::analyze::observe {

namespace ob = ::cb::analyze::observe;

inline constexpr int64_t BUFFERS = ob::kBuffers, BUFFER_SIZE = ob::kBufferSize;   // argument k points at BUFFERS + k * 0x10000
inline constexpr int64_t MARK = ob::kMark;                                        // the dword at offset o of buffer k is MARK + (k << 24) + (o << 12)
inline constexpr int64_t TEXT_A = ob::kTextA, TEXT_B = ob::kTextB;                // the idiom battery's two buffers
inline constexpr int64_t MAX_ARGS = ob::kMaxArgs, INSNS = ob::kInstructions;

using Cell = std::array<int64_t, 3>;                                              // [size, reads, writes]
using Cells = py::dict<int64_t, Cell>;                                            // offset -> cell

// ("argument", k) or ("field", k, offset): the result is one of those
struct Returns {
    py::str kind;
    int64_t k = 0;
    std::optional<int64_t> offset;
};

struct Observation {
    py::dict<int64_t, Cells> fields;                                              // argument -> its cells
    py::dict<std::tuple<int64_t, int64_t>, Cells> nested;                         // (argument, offset) -> cells
    py::list<std::tuple<int64_t, int64_t>> indirect;                              // (call site, function reached)
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t, py::str>> passes;     // (import, parameter, argument, offset, "address" | "value")
    std::optional<py::str> exit;                                                  // "ret" | "cut" | "fault"
    std::optional<int64_t> removed;
    std::optional<Returns> returns;
    std::optional<py::str> idiom;
};

// {function: observation} for up to `limit` functions (None: all of them); a function the pool lost is None
using Seen = py::dict<int64_t, std::optional<Observation>>;

namespace shim_detail {

inline Cell cell_to_old(const ob::Cell& c) { return Cell{c.size, c.reads, c.writes}; }

inline Cells cells_to_old(const ob::Cells& cells) {
    Cells out;
    for (const auto& [offset, cell] : cells) out.set(offset, cell_to_old(cell));
    return out;
}

inline Returns returns_to_old(const ob::Returns& r) {
    return Returns{py::str(r.kind == ob::Returns::Kind::Argument ? "argument" : "field"), r.k, r.offset};
}

inline Observation observation_to_old(const ob::Observation& o) {
    Observation out;
    for (const auto& [k, cells] : o.fields) out.fields.set(k, cells_to_old(cells));
    for (const auto& [key, cells] : o.nested) out.nested.set(std::make_tuple<int64_t, int64_t>(key.first, key.second), cells_to_old(cells));
    for (const ob::Indirect& i : o.indirect) out.indirect.append(std::make_tuple(int64_t(i.site), int64_t(i.target)));
    for (const ob::Pass& p : o.passes)
        out.passes.append(std::make_tuple(py::str(p.import), int64_t(p.parameter), int64_t(p.argument), int64_t(p.offset),
                                          py::str(p.how == ob::Pass::How::Address ? "address" : "value")));
    if (o.exit) out.exit = py::str(std::string(ob::name_of(*o.exit)));
    out.removed = o.removed;
    if (o.returns) out.returns = returns_to_old(*o.returns);
    if (o.idiom) out.idiom = py::str(std::string(ob::name_of(*o.idiom)));
    return out;
}

// the cb pass results the runs are set against (made over the cb image the bridge keeps for the old one)
struct Static {
    ::cb::analyze::conventions::Facts facts;
    std::vector<::cb::analyze::structures::Structure> found;
};
inline Static static_of(image::Image& img) {
    ::cb::analyze::Image& image = bridge::cb_image_of(img);
    const auto& graph = bridge::cb_graph_of(img);
    Static s;
    s.facts = ::cb::analyze::conventions::functions(image, graph);
    s.found = ::cb::analyze::structures::recover(image, graph, ::cb::analyze::conventions::summary(s.facts));
    return s;
}

inline ob::Setting setting_of(const image::Image& img, const std::optional<int64_t>& limit, int64_t jobs) {
    ob::Setting s;
    s.module = img.path.utf8();
    if (limit) s.limit = static_cast<std::size_t>(*limit);
    else s.limit = std::nullopt;
    s.jobs = static_cast<int>(jobs);
    s.argc = [](const ::cb::emulate::native_trace::ImportName& i) { return symbols::api_arguments::argc(py::str(i.dll), py::str(i.name)); };
    return s;
}

inline ob::Seen run_new(image::Image& img, const std::optional<int64_t>& limit, int64_t jobs) {
    const Static s = static_of(img);
    return ob::run(bridge::cb_image_of(img), s.facts, s.found, setting_of(img, limit, jobs));
}

}  // namespace shim_detail

// {function: observation} for up to `limit` functions (None: all of them); a function the pool lost is None (memo "observe:<limit>")
inline std::shared_ptr<Seen> run(image::Image& img, const std::optional<int64_t>& limit = 400, int64_t jobs = 4) {
    const std::string key = limit ? std::format("observe:{}", *limit) : std::string("observe:None");
    return img.memo<Seen>(py::str(key), [limit, jobs](image::Image& i) {
        auto out = std::make_shared<Seen>();
        for (const auto& [f, o] : shim_detail::run_new(i, limit, jobs)) {
            if (o) out->set(f, shim_detail::observation_to_old(*o));
            else out->set(f, std::nullopt);
        }
        return out;
    });
}

// ---- the analysis
struct ObservedFact {                                                    // kind "observed"
    int64_t addr = 0;
    py::str kind = py::str("observed");
    std::optional<py::str> exit;
    std::optional<py::dict<py::str, py::dict<py::str, Cell>>> fields;    // "argument 1" -> {"+8": cell}
    std::optional<py::dict<py::str, py::dict<py::str, Cell>>> nested;    // "argument 1 +8" -> {"+4": cell}
    std::optional<py::list<py::str>> passes;
    std::optional<py::list<py::str>> indirect_calls;
    std::optional<int64_t> removed;
    std::optional<Returns> returns;
    std::optional<py::str> behaves_like;
};
struct StructureObservedFact {                                           // kind "structure_observed"; addr None
    py::str kind = py::str("structure_observed");
    py::str name;
    py::list<py::str> fields_confirmed;
    py::list<py::str> fields_added;
    int64_t fields_not_reached = 0;
};
using Fact = std::variant<ObservedFact, StructureObservedFact>;

struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

inline Analysis analyse(image::Image& img, const std::optional<int64_t>& limit = 400, int64_t jobs = 4) {
    ::cb::analyze::Image& image = bridge::cb_image_of(img);
    const shim_detail::Static s = shim_detail::static_of(img);
    const ob::Report r = ob::analyse(image, s.facts, s.found, shim_detail::run_new(img, limit, jobs));
    Analysis out;
    const ob::Counts& c = r.counts;
    for (const auto& [k, v] : std::initializer_list<std::pair<const char*, int64_t>>{
             {"functions_run", c.functions_run}, {"returned", c.returned}, {"faulted", c.faulted}, {"cut_off", c.cut_off},
             {"with_pointer_arguments", c.with_pointer_arguments}, {"fields_observed", c.fields_observed},
             {"nested_pointers_followed", c.nested_pointers_followed}, {"static_fields_confirmed", c.static_fields_confirmed},
             {"fields_only_seen_running", c.fields_only_seen_running},
             {"structures_with_a_field_confirmed", c.structures_with_a_field_confirmed},
             {"indirect_calls_resolved", c.indirect_calls_resolved}, {"function_starts_found", c.function_starts_found},
             {"fields_passed_to_system_calls", c.fields_passed_to_system_calls},
             {"removed_bytes_agree_with_static", c.removed_bytes_agree_with_static}, {"removed_bytes_differ", c.removed_bytes_differ},
             {"library_idioms", c.library_idioms}})
        out.counts.set(py::str(k), v);
    for (const ob::ObservedFact& f : r.observed) {
        ObservedFact old;
        old.addr = f.addr;
        old.exit = py::str(std::string(ob::name_of(f.exit)));
        auto text_cells = [](const ob::Cells& cells) {
            py::dict<py::str, Cell> m;
            for (const auto& [offset, cell] : cells) m.set(py::str(std::format("+{:x}", offset)), shim_detail::cell_to_old(cell));
            return m;
        };
        if (!f.fields.empty()) {
            py::dict<py::str, py::dict<py::str, Cell>> m;
            for (const auto& [k, cells] : f.fields) m.set(py::str(std::format("argument {}", k)), text_cells(cells));
            old.fields = m;
        }
        if (!f.nested.empty()) {
            py::dict<py::str, py::dict<py::str, Cell>> m;
            for (const auto& [k, cells] : f.nested) m.set(py::str(std::format("argument {} +{:x}", k.first, k.second)), text_cells(cells));
            old.nested = m;
        }
        if (!f.passes.empty()) {
            py::list<py::str> l;
            for (const ob::Pass& p : f.passes) l.append(py::str(ob::describe(p)));
            old.passes = l;
        }
        if (!f.indirect_calls.empty()) {
            py::list<py::str> l;
            for (const std::string& t : f.indirect_calls) l.append(py::str(t));
            old.indirect_calls = l;
        }
        old.removed = f.removed;
        if (f.returns) old.returns = shim_detail::returns_to_old(*f.returns);
        if (f.behaves_like) old.behaves_like = py::str(std::string(ob::name_of(*f.behaves_like)));
        out.facts.append(Fact(old));
    }
    for (const ob::StructureObservedFact& f : r.structures) {
        StructureObservedFact old;
        old.name = py::str(f.name);
        auto texts = [](const std::vector<::cb::analyze::Va>& offsets) {
            std::vector<std::string> sorted;
            for (auto o : offsets) sorted.push_back(std::format("+{:x}", o));
            std::sort(sorted.begin(), sorted.end());   // as the "+%x" texts (the port sorted them so)
            py::list<py::str> l;
            for (const std::string& t : sorted) l.append(py::str(t));
            return l;
        };
        old.fields_confirmed = texts(f.fields_confirmed);
        old.fields_added = texts(f.fields_added);
        old.fields_not_reached = f.fields_not_reached;
        out.facts.append(Fact(old));
    }
    for (const std::string& t : r.text) out.text.append(py::str(t));
    return out;
}

}  // namespace codebreaker::analyze::observe
