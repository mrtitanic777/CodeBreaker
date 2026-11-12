// codebreaker/translate/backends/c.cpp -- typed C++ port of codebreaker.translate.backends.c (see the header).
#include "codebreaker/translate/backends/c.hpp"

namespace codebreaker::translate::backends::c {

namespace {
py::str K(const char* s) { return py::str(s); }
py::list<py::str> names(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) out.append(K(x));
    return out;
}
bool truthy(const ir::Val& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, bool>) return x;
            else if constexpr (std::is_same_v<T, int64_t>) return x != 0;
            else if constexpr (std::is_same_v<T, double>) return x != 0.0;
            else if constexpr (std::is_same_v<T, py::str>) return py::len(x) != 0;
            else if constexpr (std::is_same_v<T, ir::TypeP>) return true;
            else return x.size() != 0;                    // a list or a dict
        },
        v.v);
}
}  // namespace

const py::list<py::str> MOVED = native_l2::default_moved();   // (the one list: the native_l2 shim holds it)

const py::list<py::str> IN_MODULE = names(
    {"model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
     "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
     "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields",
     "far_reads", "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots",
     "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
     "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame",
     "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address",
     "homed_arguments", "uint8_result", "open_arguments"});

const py::list<py::str> FLAT = names(
    {"pieces", "null_is_zero", "struct_gaps", "int_result", "register_arguments", "code_pointers",
     "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls",
     "struct_fields"});

const py::list<py::str> POWERPC = names(
    {"wide_address", "struct_gaps", "pieces", "compare_as_numbers", "code_pointers",
     "homed_arguments", "uint8_result", "open_arguments"});

const py::dict<py::str, int64_t> SIZEOF = {
    {K("int1"), 1}, {K("uint1"), 1}, {K("xunknown1"), 1}, {K("char"), 1}, {K("bool"), 1}, {K("undefined"), 1}, {K("undefined1"), 1},
    {K("int2"), 2}, {K("uint2"), 2}, {K("xunknown2"), 2}, {K("undefined2"), 2},
    {K("int4"), 4}, {K("uint4"), 4}, {K("xunknown4"), 4}, {K("undefined4"), 4}, {K("float4"), 4},
    {K("int8"), 8}, {K("uint8"), 8}, {K("xunknown8"), 8}, {K("undefined8"), 8}, {K("float8"), 8}, {K("float10"), 10}};

// (p.get('sym') or {}).get('name') or p.get('name', '')
ir::Val _param_name(const ir::Dict& p) {
    ir::Val sym = p.get(K("sym"), ir::Val());
    if (truthy(sym)) {
        if (!std::holds_alternative<ir::Dict>(sym.v)) throw py::AttributeError("object has no attribute 'get'");
        ir::Val n = std::get<ir::Dict>(sym.v).get(K("name"), ir::Val());
        if (truthy(n)) return n;
    }
    return p.get(K("name"), ir::Val(""));
}

}  // namespace codebreaker::translate::backends::c
