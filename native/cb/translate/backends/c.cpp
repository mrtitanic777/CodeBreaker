// cb/translate/backends/c.cpp -- the C back end's pass sets, scalar sizes and parameter names (see the header).
#include "cb/translate/backends/c.hpp"

#include <array>
#include <unordered_map>

namespace cb::translate::backends::c {

namespace {

// The passes of c_text done on the tree, in order. (The Python listed open_arguments twice in the module set; once is the set.)
constexpr std::array<std::string_view, 52> kMovedPasses = {
    "far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers", "null_is_zero",
    "global_pointer_steps", "one_name_for_globals", "far_result", "model_word", "joined_arguments", "far_arguments",
    "void_result", "byte_result", "export", "loadds", "selector_structs", "struct_gaps", "own_segment_calls",
    "open_arguments", "interrupts", "far_addresses", "far_casts", "entry_sp", "stack_arguments", "far_locals",
    "far_concat_casts", "buffer_fields", "far_reads", "near_to_far", "stack_segment_stores", "callee_pops",
    "caller_frame_slots", "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
    "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame", "globals_through",
    "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result"};

constexpr std::array<std::string_view, 42> kModulePasses = {
    "model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
    "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses", "far_casts",
    "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields", "far_reads", "near_to_far",
    "stack_segment_stores", "callee_pops", "caller_frame_slots", "selector_imports", "segment_values",
    "code_segment_reads", "names_used", "heading_facts", "int_result", "register_arguments", "code_pointers",
    "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields",
    "wide_address", "homed_arguments", "uint8_result"};

constexpr std::array<std::string_view, 13> kFlatPasses = {
    "pieces", "null_is_zero", "struct_gaps", "int_result", "register_arguments", "code_pointers", "compare_as_numbers",
    "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields"};

constexpr std::array<std::string_view, 8> kPowerpcPasses = {
    "wide_address", "struct_gaps", "pieces", "compare_as_numbers", "code_pointers", "homed_arguments", "uint8_result",
    "open_arguments"};

constexpr std::array<ScalarSize, 22> kScalarSizes = {{
    {"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"char", 1}, {"bool", 1}, {"undefined", 1}, {"undefined1", 1},
    {"int2", 2}, {"uint2", 2}, {"xunknown2", 2}, {"undefined2", 2},
    {"int4", 4}, {"uint4", 4}, {"xunknown4", 4}, {"undefined4", 4}, {"float4", 4},
    {"int8", 8}, {"uint8", 8}, {"xunknown8", 8}, {"undefined8", 8}, {"float8", 8},
    {"float10", 10}}};

}  // namespace

std::span<const std::string_view> moved_passes() { return kMovedPasses; }
std::span<const std::string_view> module_passes() { return kModulePasses; }
std::span<const std::string_view> flat_passes() { return kFlatPasses; }
std::span<const std::string_view> powerpc_passes() { return kPowerpcPasses; }

std::span<const ScalarSize> scalar_sizes() { return kScalarSizes; }

std::optional<std::size_t> scalar_size(std::string_view type_name) {
    static const std::unordered_map<std::string_view, std::size_t> table = [] {   // lookup only
        std::unordered_map<std::string_view, std::size_t> t;
        for (const ScalarSize& s : kScalarSizes) t.emplace(s.name, s.bytes);
        return t;
    }();
    const auto it = table.find(type_name);
    if (it == table.end()) return std::nullopt;
    return it->second;
}

std::string param_name(const ir::Param& param) { return param.sym != nullptr ? param.sym->name : std::string(); }

}  // namespace cb::translate::backends::c
