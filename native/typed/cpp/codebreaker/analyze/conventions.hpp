// codebreaker/analyze/conventions.hpp -- COMPAT SHIM over cb/analyze/conventions.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/analyze/conventions.py port (classes, observe, decompile/describe and the cb_image bridge). Deleted with its last
// py:: consumer.
//
// The pass itself -- how each function is called and what kind of function it is -- is native/cb/analyze/conventions.{hpp,cpp}
// (it replaces cbanalyze.dll's conventions.cc). The old callers hold the module as an old `analyze::image::Image`; the new pass
// runs on the cb Image and call graph that analyze/cb_image.hpp makes of it (once per image) and answers in the old records
// (`Fact` over py::str / py::list), memoised in the old image under "conventions" as before.
//
// Not carried over (nothing outside the old conventions.cpp called them): `_signature`, `_functions` and `stack_delta` (the new
// module has its own, over the cb Image).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <variant>

#include "cb/analyze/conventions.hpp"
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::conventions {

// The 16/32-bit bridge APIs (THUNKCONNECT32 ...), upper-cased.
inline const py::list<py::str> _THUNK_APIS = {py::str("THUNKCONNECT32"), py::str("THUNKCONNECT16"), py::str("C16THKSL01"),
                                              py::str("C32THKSL01"), py::str("THUNKINITLS"), py::str("QT_THUNK"), py::str("FT_THUNK"),
                                              py::str("FT_PROLOG"), py::str("FT_EXIT"), py::str("SSONBIGSTACK"), py::str("SSCALL"),
                                              py::str("MAPLS"), py::str("UNMAPLS"), py::str("MAPSL"), py::str("GETVDMPOINTER32W"),
                                              py::str("ALLOCSLCALLBACK"), py::str("GETPROCADDRESS32W"), py::str("K32THK1632PROLOG"),
                                              py::str("FT_EXIT0")};

// {"callers_use", "callers_ignore"}: how many callers use the result / ignore it
struct ResultEvidence {
    int64_t callers_use = 0;
    int64_t callers_ignore = 0;
};

// One function's facts (a row of the DLL's conventions.functions).
struct Fact {
    int64_t addr = 0;
    py::str kind = py::str("convention");
    py::str name;
    py::str shape;                                         // "import thunk" | "jump thunk" | "empty" | "constant" | "leaf" | "calls" | "not code"
    std::optional<py::str> thunk_of;                       // an import thunk's import ("USER32!X"), a jump thunk's target
    std::optional<int64_t> constant;                       // shape "constant": what it returns
    std::optional<py::list<py::str>> tail_calls;           // names of the functions it jumps to
    std::optional<py::str> convention;
    py::list<py::str> reg_args;                            // registers read before written
    std::optional<int64_t> removes;                        // bytes its return removes (0: caller cleans), None: no return seen
    std::optional<int64_t> args;
    std::optional<py::str> arg_source;
    std::optional<py::dict<py::str, int64_t>> arg_slots;   // {"+8": width read ...}: argument slots read
    bool frame = false;
    int64_t locals = 0;
    py::list<py::str> saved;                               // callee-saved registers it pushes
    std::optional<py::str> result;                         // "none" | "value" | "flag" | "float" | "wide" | None
    std::optional<ResultEvidence> result_evidence;
    std::optional<int64_t> loops;
    std::optional<py::list<py::str>> string_ops;
    std::optional<int64_t> float_instructions;
    std::optional<py::list<int64_t>> float_args;
    bool variadic = false;
    std::optional<py::list<py::str>> entry;                // 16-bit: notes on the entry sequence
    std::optional<py::str> procedure;                      // "window procedure" | "dialog procedure" | "hook" | ...
};
using Facts = py::dict<int64_t, Fact>;

namespace shim_ {

namespace nc = ::cb::analyze::conventions;

inline py::list<py::str> to_py_list(const std::vector<std::string>& items) {
    py::list<py::str> out;
    for (const std::string& s : items) out.append(py::str(s));
    return out;
}

inline Fact to_old(const nc::Fact& f) {
    Fact o;
    o.addr = f.addr;
    o.name = py::str(f.name);
    o.shape = py::str(std::string(nc::name_of(f.shape)));
    if (f.shape == nc::Shape::NotCode) {   // what is not code has no more than this (its convention, args and arg_source are None)
        return o;
    }
    if (f.thunk_of) o.thunk_of = py::str(*f.thunk_of);
    o.constant = f.constant;
    if (!f.tail_calls.empty()) o.tail_calls = to_py_list(f.tail_calls);
    if (f.convention) o.convention = py::str(*f.convention);
    o.reg_args = to_py_list(f.reg_args);
    o.removes = f.removes;
    o.args = f.args;
    if (f.arg_source) o.arg_source = py::str(*f.arg_source);
    if (!f.arg_slots.empty()) {
        py::dict<py::str, int64_t> slots;
        for (const auto& [offset, width] : f.arg_slots) slots.set(py::str("+" + std::to_string(offset)), width);
        o.arg_slots = slots;
    }
    o.frame = f.frame;
    o.locals = f.locals;
    o.saved = to_py_list(f.saved);
    if (f.result) o.result = py::str(std::string(nc::name_of(*f.result)));
    o.result_evidence = ResultEvidence{f.callers_use, f.callers_ignore};
    if (f.loops != 0) o.loops = f.loops;
    if (!f.string_ops.empty()) o.string_ops = to_py_list(f.string_ops);
    if (f.float_instructions != 0) {
        o.float_instructions = f.float_instructions;
        if (!f.float_args.empty()) {
            py::list<int64_t> slots;
            for (int64_t s : f.float_args) slots.append(s);
            o.float_args = slots;
        }
    }
    o.variadic = f.variadic;
    if (!f.entry.empty()) o.entry = to_py_list(f.entry);
    if (f.procedure) o.procedure = py::str(*f.procedure);
    return o;
}

}  // namespace shim_

// {start: fact} for every function of the image (memo "conventions").
inline std::shared_ptr<Facts> functions(image::Image& img) {
    return img.memo<Facts>(py::str("conventions"), [](image::Image& i) {
        auto out = std::make_shared<Facts>();
        for (const auto& [start, fact] : ::cb::analyze::conventions::functions(bridge::cb_image_of(i), bridge::cb_graph_of(i)))
            out->set(start, shim_::to_old(fact));
        return out;
    });
}

// Groups of functions with the same code (the same source compiled more than once), sorted by first address.
inline py::list<py::list<int64_t>> duplicates(image::Image& img, int64_t least = 6) {
    py::list<py::list<int64_t>> out;
    for (const std::vector<::cb::analyze::Va>& group : ::cb::analyze::conventions::duplicates(bridge::cb_image_of(img), static_cast<std::size_t>(least))) {
        py::list<int64_t> g;
        for (const auto va : group) g.append(va);
        out.append(g);
    }
    return out;
}

// The 16/32-bit bridge functions a module imports ([] when it is not such a layer).
inline py::list<py::str> thunk_layer(image::Image& img) {
    return shim_::to_py_list(::cb::analyze::conventions::thunk_layer(bridge::cb_image_of(img)));
}

// ---- analyse() ----
struct DuplicatesRow {                   // {"addr": g[0], "kind": "duplicates", "functions": [label ...]}
    int64_t addr = 0;
    py::str kind = py::str("duplicates");
    py::list<py::str> functions;
};
struct ThunkLayerRow {                   // {"addr": None, "kind": "thunk_layer", "imports": [...]}
    py::str kind = py::str("thunk_layer");
    py::list<py::str> imports;
};
using AnyFact = std::variant<Fact, DuplicatesRow, ThunkLayerRow>;
struct Analysis {
    py::dict<py::str, int64_t> counts;   // functions, convention_known, callee_removes_arguments, ... bridge_imports
    py::list<AnyFact> facts;
    py::list<py::str> text;
};

inline Analysis analyse(image::Image& img) {
    namespace nc = ::cb::analyze::conventions;
    const nc::Report report = nc::analyse(bridge::cb_image_of(img), bridge::cb_graph_of(img));
    Analysis out;
    const nc::Counts& c = report.counts;
    const std::pair<const char*, int64_t> counts[] = {
        {"functions", c.functions}, {"convention_known", c.convention_known}, {"callee_removes_arguments", c.callee_removes_arguments},
        {"caller_removes_arguments", c.caller_removes_arguments}, {"argument_count_known", c.argument_count_known},
        {"argument_count_from_return", c.argument_count_from_return}, {"argument_count_from_callers", c.argument_count_from_callers},
        {"argument_count_from_frame", c.argument_count_from_frame}, {"register_arguments", c.register_arguments}, {"this_in_ecx", c.this_in_ecx},
        {"result_known", c.result_known}, {"result_none", c.result_none}, {"import_thunks", c.import_thunks}, {"jump_thunks", c.jump_thunks},
        {"tail_calling", c.tail_calling}, {"empty_or_constant", c.empty_or_constant}, {"leaf", c.leaf}, {"with_loops", c.with_loops},
        {"inline_string_operations", c.inline_string_operations}, {"floating_point", c.floating_point}, {"variadic", c.variadic},
        {"system_called_procedures", c.system_called_procedures}, {"duplicate_groups", c.duplicate_groups},
        {"duplicate_functions", c.duplicate_functions}, {"bridge_imports", c.bridge_imports}};
    for (const auto& [name, value] : counts) out.counts.set(py::str(name), value);
    for (const auto& [start, fact] : report.facts) out.facts.append(shim_::to_old(fact));
    for (const nc::DuplicatesFact& d : report.duplicates) {
        DuplicatesRow row;
        row.addr = d.addr;
        row.functions = shim_::to_py_list(d.functions);
        out.facts.append(row);
    }
    if (!report.bridge_imports.empty()) {
        ThunkLayerRow row;
        row.imports = shim_::to_py_list(report.bridge_imports);
        out.facts.append(row);
    }
    out.text = shim_::to_py_list(report.text);
    return out;
}

}  // namespace codebreaker::analyze::conventions
