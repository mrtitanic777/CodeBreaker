// cb/translate/native_l2_cases_test.cpp -- what the C back end writes (the C back end proper, the survival rules of L2, and the
// 16-bit frame rules), asked of the library through its wire.
//
// testdata/c_backend_cases.json holds, for each case, every call made of cbl2.dll -- the entry point, the arguments as the library
// reads them (documents whole, nothing held from an earlier call), and the answer. The checked-in answers were reviewed against the
// rules they state (far/near passes, frames through SS, pieces, buffers, selector structures, interrupts, heading rules,
// prototypes, struct fields ...). Each test here asks the library the same questions again and holds its answers to those. They need
// cbl2.dll (cmake --build native/l2/build --target cbl2, or CB_L2_DLL naming it); without it they say so and pass.
#include <cstdlib>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/native_l2.hpp"

namespace {

struct L2caseRecord {
    std::string fn, args, result;
    int status = 0;
};

const cb::translate::native_l2::Library* l2cases_library() {
    static const std::optional<cb::translate::native_l2::Library> lib = []() -> std::optional<cb::translate::native_l2::Library> {
        std::filesystem::path dll = cb::translate::native_l2::Library::default_dll(cb::testing::data_path("../.."));
        if (const auto env = cb::fs::get_env("CB_L2_DLL")) dll = cb::fs::from_text(*env);
        std::error_code ec;
        if (!std::filesystem::is_regular_file(dll, ec)) {
            std::cerr << "native_l2 case tests: " << dll.string() << " is not built; skipped\n";
            return std::nullopt;
        }
        return cb::translate::native_l2::Library::load(dll);
    }();
    return lib ? &*lib : nullptr;
}

const std::map<std::string, std::vector<L2caseRecord>>& l2cases_recorded() {
    static const std::map<std::string, std::vector<L2caseRecord>> all = [] {
        std::map<std::string, std::vector<L2caseRecord>> out;
        const cb::Json doc = cb::json::parse(cb::fs::read_text(cb::testing::data_path("translate/testdata/c_backend_cases.json")));
        for (const cb::Json& c : doc) {
            out[c.at("test").get<std::string>()].push_back(
                {c.at("fn").get<std::string>(), c.at("args").get<std::string>(), c.at("result").get<std::string>(), c.at("status").get<int>()});
        }
        return out;
    }();
    return all;
}

// The library asked what the case asked of it; its answers are the recorded ones.
void l2cases_replay(const std::string& case_name) {
    const auto* lib = l2cases_library();
    if (lib == nullptr) return;
    const auto found = l2cases_recorded().find(case_name);
    CB_CHECK(found != l2cases_recorded().end());
    if (found == l2cases_recorded().end()) return;
    int n = 0;
    for (const L2caseRecord& c : found->second) {
        const auto [status, text] = lib->call_raw(c.fn, c.args);
        CB_CHECK_EQ(status, c.status);
        CB_CHECK(text == c.result);
        if (text != c.result) {
            std::cerr << case_name << " call " << n << " (" << c.fn << "): answered\n" << text.substr(0, 600) << "\nrecorded\n"
                      << c.result.substr(0, 600) << "\n";
        }
        ++n;
    }
}

}  // namespace

CB_TEST("translate.native_l2.cases.c_backend.numbers_in_their_natural_base", [] { l2cases_replay("c_backend/numbers_in_their_natural_base"); });
CB_TEST("translate.native_l2.cases.c_backend.brackets_as_printc_places_them", [] { l2cases_replay("c_backend/brackets_as_printc_places_them"); });
CB_TEST("translate.native_l2.cases.c_backend.joined_conditions", [] { l2cases_replay("c_backend/joined_conditions"); });
CB_TEST("translate.native_l2.cases.c_backend.statements", [] { l2cases_replay("c_backend/statements"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_passes_far_and_near_calls", [] { l2cases_replay("c_backend/moved_passes_far_and_near_calls"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_from_stack", [] { l2cases_replay("c_backend/moved_far_from_stack"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_frame_through_ss", [] { l2cases_replay("c_backend/moved_frame_through_ss"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_pieces", [] { l2cases_replay("c_backend/moved_pieces"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_registers_null_and_globals", [] { l2cases_replay("c_backend/moved_registers_null_and_globals"); });
CB_TEST("translate.native_l2.cases.c_backend.a_module_as_one_tree_with_what_stayed_assembly", [] { l2cases_replay("c_backend/a_module_as_one_tree_with_what_stayed_assembly"); });
CB_TEST("translate.native_l2.cases.c_backend.a_32_bit_module_as_one_tree", [] { l2cases_replay("c_backend/a_32_bit_module_as_one_tree"); });
CB_TEST("translate.native_l2.cases.c_backend.comments_where_printc_writes_them", [] { l2cases_replay("c_backend/comments_where_printc_writes_them"); });
CB_TEST("translate.native_l2.cases.c_backend.a_long_line_broken_as_printc_breaks_it", [] { l2cases_replay("c_backend/a_long_line_broken_as_printc_breaks_it"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_passes_that_read_the_module", [] { l2cases_replay("c_backend/moved_passes_that_read_the_module"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_own_segment_calls", [] { l2cases_replay("c_backend/moved_own_segment_calls"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_interrupts", [] { l2cases_replay("c_backend/moved_interrupts"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_selector_structs", [] { l2cases_replay("c_backend/moved_selector_structs"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_old_style_definition", [] { l2cases_replay("c_backend/moved_old_style_definition"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_entry_sp_and_stack_arguments", [] { l2cases_replay("c_backend/moved_entry_sp_and_stack_arguments"); });
CB_TEST("translate.native_l2.cases.c_backend.decompile16_counts_calls_in_l2", [] { l2cases_replay("c_backend/decompile16_counts_calls_in_l2"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_addresses", [] { l2cases_replay("c_backend/moved_far_addresses"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_casts", [] { l2cases_replay("c_backend/moved_far_casts"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_locals", [] { l2cases_replay("c_backend/moved_far_locals"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_concat_casts", [] { l2cases_replay("c_backend/moved_far_concat_casts"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_buffer_fields", [] { l2cases_replay("c_backend/moved_buffer_fields"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_far_reads", [] { l2cases_replay("c_backend/moved_far_reads"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_near_to_far", [] { l2cases_replay("c_backend/moved_near_to_far"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_stack_segment_stores", [] { l2cases_replay("c_backend/moved_stack_segment_stores"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_callee_pops", [] { l2cases_replay("c_backend/moved_callee_pops"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_caller_frame_slots", [] { l2cases_replay("c_backend/moved_caller_frame_slots"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_selector_imports", [] { l2cases_replay("c_backend/moved_selector_imports"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_segment_values", [] { l2cases_replay("c_backend/moved_segment_values"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_code_segment_reads", [] { l2cases_replay("c_backend/moved_code_segment_reads"); });
CB_TEST("translate.native_l2.cases.c_backend.names_used_for_the_header", [] { l2cases_replay("c_backend/names_used_for_the_header"); });
CB_TEST("translate.native_l2.cases.c_backend.prototypes_where_the_calls_agree", [] { l2cases_replay("c_backend/prototypes_where_the_calls_agree"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_32bit_heading", [] { l2cases_replay("c_backend/moved_32bit_heading"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_keep_frame", [] { l2cases_replay("c_backend/moved_keep_frame"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_32bit_globals", [] { l2cases_replay("c_backend/moved_32bit_globals"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_fit_calls", [] { l2cases_replay("c_backend/moved_fit_calls"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_powerpc_passes", [] { l2cases_replay("c_backend/moved_powerpc_passes"); });
CB_TEST("translate.native_l2.cases.c_backend.moved_struct_fields", [] { l2cases_replay("c_backend/moved_struct_fields"); });
CB_TEST("translate.native_l2.cases.c_backend.inline_asm_block_and_a_label_ending_a_block", [] { l2cases_replay("c_backend/inline_asm_block_and_a_label_ending_a_block"); });
CB_TEST("translate.native_l2.cases.c_backend.heading_declaration_from_l2", [] { l2cases_replay("c_backend/heading_declaration_from_l2"); });
CB_TEST("translate.native_l2.cases.l2_survives.switch_on_a_pointer_keeps_its_case_labels_cast", [] { l2cases_replay("l2_survives/switch_on_a_pointer_keeps_its_case_labels_cast"); });
CB_TEST("translate.native_l2.cases.l2_survives.switch_on_a_far_pointer_is_written_on_its_four_bytes", [] { l2cases_replay("l2_survives/switch_on_a_far_pointer_is_written_on_its_four_bytes"); });
CB_TEST("translate.native_l2.cases.l2_survives.an_array_index_past_9_is_a_literal", [] { l2cases_replay("l2_survives/an_array_index_past_9_is_a_literal"); });
CB_TEST("translate.native_l2.cases.l2_survives.a_supplied_jump_table_flow_never_reaches_is_dropped", [] { l2cases_replay("l2_survives/a_supplied_jump_table_flow_never_reaches_is_dropped"); });
CB_TEST("translate.native_l2.cases.frame16.inc_is_written_as_plus_plus", [] { l2cases_replay("frame16/inc_is_written_as_plus_plus"); });
CB_TEST("translate.native_l2.cases.frame16.the_pieces_of_a_buffer_are_one_array", [] { l2cases_replay("frame16/the_pieces_of_a_buffer_are_one_array"); });
CB_TEST("translate.native_l2.cases.frame16.the_back_end_writes_postfix_increments", [] { l2cases_replay("frame16/the_back_end_writes_postfix_increments"); });
CB_TEST("translate.native_l2.cases.frame16.a_piece_of_a_buffer_piece_is_read_at_its_place", [] { l2cases_replay("frame16/a_piece_of_a_buffer_piece_is_read_at_its_place"); });
CB_TEST("translate.native_l2.cases.frame16.a_buffer_with_an_array_among_its_pieces_is_left", [] { l2cases_replay("frame16/a_buffer_with_an_array_among_its_pieces_is_left"); });
