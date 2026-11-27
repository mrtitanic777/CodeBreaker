// Tests of window_functions.hpp: the Source card's tabs, the Functions list, Check, Compare, the function drop-down and the command palette's rows.
#include "app/window_functions.hpp"

#include <cmath>

#include "app/window_convert.hpp"
#include "core/testing.hpp"

namespace cb::app::window {
namespace {

Model wf_model(SourceTab tab, int functions = 42) {
    Model m;
    m.view = View::Done;
    m.output_path = "C:\\Converted\\lz32";
    m.preview_files = {{"lz32.c", false}, {"lz32.asm", false}};
    m.source_tab = tab;
    for (int i = 0; i < functions; ++i) {
        convert::FunctionRecord f;
        f.name = "fn" + std::to_string(i);
        f.address = "0x" + std::to_string(1000 + i);
        f.verdict = i == 4 ? convert::FunctionVerdict::Differs : i >= functions - 3 ? convert::FunctionVerdict::NotChecked : convert::FunctionVerdict::Matches;
        m.functions.push_back(f);
    }
    return m;
}

Layout wf_layout(const Model& m, float w = 1180, float h = 700) { return compute_layout(View::Done, w, h, kRailWidth, extras_for(m, View::Done)); }

CB_TEST("app.window_functions.the_tabs_sit_under_the_finished_file_and_the_files_tab_keeps_the_path_row", [] {
    const Model m = wf_model(SourceTab::Files);
    const Layout l = wf_layout(m);
    CB_CHECK(l.tabs.w > 0 && l.tabs.y >= l.file_icon.bottom() && l.tabs.bottom() <= l.field.y);
    CB_CHECK(l.tab_files.right() < l.tab_functions.x && l.tab_functions.right() < l.tab_check.x);
    CB_CHECK(l.open_folder.right() < l.browse.x && std::fabs(l.browse.right() - l.preview.right()) < 0.01f);   // the two buttons are at the header's right
    CB_CHECK(l.open_folder.bottom() <= l.tabs.y && l.browse.y < l.tabs.y);
    CB_CHECK(l.field.w > 0 && l.copy_path.w > 0 && l.preview_list.w > 0 && l.preview_code.w > 0);
    CB_CHECK(l.preview.y >= l.field.bottom());
});

CB_TEST("app.window_functions.the_other_tabs_give_the_path_row_room_to_their_view", [] {
    const Layout files = wf_layout(wf_model(SourceTab::Files));
    for (const SourceTab tab : {SourceTab::Functions, SourceTab::Check}) {
        const Layout l = wf_layout(wf_model(tab));
        CB_CHECK(l.field.w <= 0 && l.copy_path.w <= 0 && l.preview_list.w <= 0 && l.preview_code.w <= 0);
        CB_CHECK(l.preview.w > 0 && l.preview.y < files.preview.y);
        CB_CHECK(std::fabs(l.preview.bottom() - files.preview.bottom()) < 0.01f);
        CB_CHECK(std::fabs(l.tabs.y - files.tabs.y) < 0.01f);   // the tabs do not move
    }
});

CB_TEST("app.window_functions.tabs_are_found_and_switch_nothing_by_themselves", [] {
    const Model m = wf_model(SourceTab::Functions);
    const Layout l = wf_layout(m);
    CB_CHECK(hit_test_source_tabs(l, m, l.tab_files.x + 5, l.tabs.y + 5).control == Control::TabFiles);
    CB_CHECK(hit_test_source_tabs(l, m, l.tab_functions.x + 5, l.tabs.bottom() - 2).control == Control::TabFunctions);
    CB_CHECK(hit_test_source_tabs(l, m, l.tab_check.x + 5, l.tabs.y + 5).control == Control::TabCheck);
    CB_CHECK(hit_test_convert(l, m, l.tab_check.x + 5, l.tabs.y + 5).control == Control::TabCheck);
    CB_CHECK(hit_test_source_tabs(l, m, l.tabs.right() - 3, l.tabs.y + 5).control == Control::None);
});

CB_TEST("app.window_functions.the_summary_says_how_many_behave_alike_and_never_invents_a_check", [] {
    CB_CHECK_EQ(functions_summary(fnsrc::count(wf_model(SourceTab::Functions).functions)), "38 of 42 behave like the original \xC2\xB7 1 differs \xC2\xB7 3 not checked");
    std::vector<convert::FunctionRecord> none(7);
    CB_CHECK_EQ(functions_summary(fnsrc::count(none)), "7 functions \xC2\xB7 none checked by running them");
    CB_CHECK_EQ(functions_summary(fnsrc::count({})), "No functions were found in the output folder.");
    CB_CHECK_EQ(filter_name(0), "All");
    CB_CHECK_EQ(filter_name(1), "Differs");
    CB_CHECK_EQ(filter_name(2), "Not checked");
    CB_CHECK_EQ(verdict_name(convert::FunctionVerdict::Matches), "Behaves like the original");
    CB_CHECK_EQ(verdict_name(convert::FunctionVerdict::NotChecked), "Not checked");
});

CB_TEST("app.window_functions.after_a_judge_the_summary_and_chips_say_each_class", [] {
    std::vector<convert::FunctionRecord> list(7);
    const store::Class classes[] = {store::Class::Verified, store::Class::Verified, store::Class::Weak, store::Class::Unverified, store::Class::NoCompile,
                                     store::Class::NoSource};
    for (std::size_t i = 0; i < 6; ++i) {
        list[i].judged = classes[i];
        list[i].verdict = convert::verdict_of(classes[i]);
    }
    CB_CHECK_EQ(functions_summary(fnsrc::count(list)),
                "2 verified \xC2\xB7 1 weak \xC2\xB7 1 unverified \xC2\xB7 1 does not compile \xC2\xB7 1 no source \xC2\xB7 1 not checked");
    CB_CHECK_EQ(verdict_name(list[0]), "Verified");
    CB_CHECK_EQ(verdict_name(list[2]), "Weak match");
    CB_CHECK_EQ(verdict_name(list[3]), "Differs");
    CB_CHECK_EQ(verdict_name(list[4]), "Does not compile");
    CB_CHECK_EQ(verdict_name(list[5]), "No source");
    CB_CHECK_EQ(verdict_name(list[6]), "Not checked");   // (no judge ran it: the verdict's words)
    convert::FunctionRecord not_run;
    not_run.judged = store::Class::NotJudged;
    CB_CHECK_EQ(verdict_name(not_run), "Not run");
});

CB_TEST("app.window_functions.the_list_has_a_row_per_function_the_filter_lets_through_and_scrolls", [] {
    Model m = wf_model(SourceTab::Functions);
    Layout l = wf_layout(m);
    FunctionsLayout f = compute_functions_layout(l, m);
    CB_CHECK_EQ(f.shown.size(), 42u);
    CB_CHECK_EQ(f.rows.size(), 42u);
    CB_CHECK(f.bar.right() < f.filter.x && f.summary.x >= f.bar.right() && f.filter.right() <= l.preview.right() + 0.01f);
    CB_CHECK(f.list_area.y >= f.filter.bottom() && f.rows[1].y - f.rows[0].y == kFnRowHeight);
    CB_CHECK(functions_max_scroll(f) > 0);
    m.fn_scroll = 90;
    f = compute_functions_layout(l, m);
    CB_CHECK(std::fabs(f.rows[0].y - (f.list_area.y - 90)) < 0.01f);
    m.fn_filter = 1;
    f = compute_functions_layout(l, m);
    CB_CHECK(f.shown == std::vector<int>{4});
    m.fn_filter = 2;
    f = compute_functions_layout(l, m);
    CB_CHECK_EQ(f.shown.size(), 3u);
    CB_CHECK_EQ(f.shown[0], 39);
});

CB_TEST("app.window_functions.a_row_is_found_by_the_function_not_by_its_place_in_the_filtered_list", [] {
    Model m = wf_model(SourceTab::Functions);
    m.fn_filter = 2;   // only the last three
    const Layout l = wf_layout(m);
    const FunctionsLayout f = compute_functions_layout(l, m);
    const PageHit h = hit_test_source_tabs(l, m, f.rows[1].x + 40, f.rows[1].y + 5);
    CB_CHECK(h.control == Control::FnRow && h.index == 40);
    CB_CHECK(hit_test_source_tabs(l, m, f.filter.x + 5, f.filter.y + 5).control == Control::FnFilter);
    CB_CHECK(hit_test_source_tabs(l, m, f.rows[2].x + 40, f.rows[2].bottom() + 80).control == Control::None);
    const FnRowParts p = fn_row_parts(f.rows[0]);
    CB_CHECK(p.name.right() <= p.address.x && p.address.right() <= p.chip.x && p.chip.right() <= f.rows[0].right());
});

CB_TEST("app.window_functions.check_shows_the_ring_only_when_something_was_judged", [] {
    Model m = wf_model(SourceTab::Check);
    Layout l = wf_layout(m);
    CheckLayout c = compute_check_layout(l, m);
    CB_CHECK(c.judged && c.ring.w == 132 && c.show.w > 0);
    CB_CHECK(c.ring.right() < c.title.x && c.dot[0].y < c.dot[1].y && c.dot[1].y < c.dot[2].y);
    CB_CHECK(c.show.bottom() <= l.preview.bottom() && c.again.x > c.show.right());
    CB_CHECK(c.note.h > 0);   // Check again is disabled (no route judges again): the page says why
    CB_CHECK(hit_test_source_tabs(l, m, c.show.x + 3, c.show.y + 3).control == Control::CheckShow);
    CB_CHECK(hit_test_source_tabs(l, m, c.again.x + 3, c.again.y + 3).control == Control::CheckAgain);
    m.can_recheck = true;
    CB_CHECK_EQ(compute_check_layout(l, m).note.h, 0.0f);
    // nothing judged: the plain explanation, no ring, no Show what differs
    for (convert::FunctionRecord& f : m.functions) f.verdict = convert::FunctionVerdict::NotChecked;
    l = wf_layout(m);
    c = compute_check_layout(l, m);
    CB_CHECK(!c.judged && c.ring.w == 0 && c.show.w == 0 && c.mark.w > 0 && c.again.w > 0);
});

CB_TEST("app.window_functions.show_what_differs_is_offered_only_when_something_differs", [] {
    Model m = wf_model(SourceTab::Check);
    m.functions[4].verdict = convert::FunctionVerdict::Matches;
    const CheckLayout c = compute_check_layout(wf_layout(m), m);
    CB_CHECK(c.judged && c.show.w == 0);
    for (const PageHit& h : focus_order_source_tabs(wf_layout(m), m)) CB_CHECK(h.control != Control::CheckShow);
});

CB_TEST("app.window_functions.compare_has_two_panes_whose_lines_are_found_and_lit_together", [] {
    Model m = wf_model(SourceTab::Functions);
    m.cmp_index = 4;
    m.cmp_asm = {"LZRead:", "push ebp", "call lz_handle_of", "test eax, eax", "jz .bad", "ret"};
    m.cmp_c = {"INT LZRead()", "{", "lz = lz_handle_of(h);", "if (!lz) return;", "}"};
    m.cmp_links = {{2, 4, 2, 3}};
    const Layout l = wf_layout(m);
    const CompareLayout c = compute_compare_layout(l, m);
    CB_CHECK(c.all.right() < c.picker.x && c.picker.right() < c.chip.x);
    CB_CHECK(c.asm_pane.right() < c.c_pane.x && c.asm_pane.y == c.c_pane.y && c.visible_lines > 5);
    CB_CHECK(c.asm_code.x > c.asm_pane.x && c.asm_code.y >= c.asm_head.bottom());
    CB_CHECK(hit_test_source_tabs(l, m, c.all.x + 3, c.all.y + 3).control == Control::FnAll);
    CB_CHECK(hit_test_source_tabs(l, m, c.picker.x + 3, c.picker.y + 3).control == Control::FnPicker);
    const PageHit asm_hit = hit_test_source_tabs(l, m, c.asm_code.x + 10, c.asm_code.y + 3 * kCodeLineHeight + 2);
    CB_CHECK(asm_hit.control == Control::CmpAsmLine && asm_hit.index == 3);
    const PageHit c_hit = hit_test_source_tabs(l, m, c.c_code.x + 10, c.c_code.y + 2);
    CB_CHECK(c_hit.control == Control::CmpCLine && c_hit.index == 0);
    CB_CHECK(hit_test_source_tabs(l, m, c.c_code.x + 10, c.c_code.y + 9 * kCodeLineHeight).control == Control::None);   // below its last line
    // nothing clicked: nothing is lit; clicking a line with a link lights its range on both sides; one without lights only itself
    CB_CHECK(!compare_line_lit(m, 0, 2));
    m.cmp_side = 1;
    m.cmp_line = 3;
    CB_CHECK(compare_line_lit(m, 1, 2) && compare_line_lit(m, 1, 3) && !compare_line_lit(m, 1, 4));
    CB_CHECK(compare_line_lit(m, 0, 2) && compare_line_lit(m, 0, 3) && compare_line_lit(m, 0, 4) && !compare_line_lit(m, 0, 1) && !compare_line_lit(m, 0, 5));
    m.cmp_side = 0;
    m.cmp_line = 5;   // `ret`: no link
    CB_CHECK(compare_line_lit(m, 0, 5) && !compare_line_lit(m, 0, 4) && !compare_line_lit(m, 1, 0));
    // scrolling moves what is under a point
    m.cmp_asm.resize(100, "nop");
    m.cmp_scroll_asm = 10;
    CB_CHECK_EQ(compare_line_at(m, c, 0, c.asm_code.y + 1), 10);
    CB_CHECK_EQ(compare_max_first_line(m, c, 0), 100 - c.visible_lines);
    CB_CHECK_EQ(compare_max_first_line(m, c, 1), 0);
});

CB_TEST("app.window_functions.the_function_drop_down_lists_nine_at_most_from_its_top", [] {
    Model m = wf_model(SourceTab::Functions);
    m.cmp_index = 4;
    m.cmp_picker_open = true;
    m.cmp_picker_top = 10;
    const Layout l = wf_layout(m);
    PickerLayout p = compute_picker_layout(l, m);
    CB_CHECK_EQ(p.rows.size(), 9u);
    CB_CHECK_EQ(p.first, 10);
    const CompareLayout c = compute_compare_layout(l, m);
    CB_CHECK(p.outline.y >= c.picker.bottom() && std::fabs(p.outline.x - c.picker.x) < 0.01f);
    CB_CHECK_EQ(picker_item_at(p, p.rows[2].x + 5, p.rows[2].y + 5), 12);
    CB_CHECK_EQ(picker_item_at(p, 1, 1), -1);
    m.cmp_picker_top = 40;   // near the end: fewer rows
    p = compute_picker_layout(l, m);
    CB_CHECK_EQ(p.rows.size(), 2u);
    m.cmp_picker_top = 999;   // never past the last function
    CB_CHECK_EQ(compute_picker_layout(l, m).first, 41);
});

CB_TEST("app.window_functions.tab_visits_the_tabs_then_the_view", [] {
    Model m = wf_model(SourceTab::Functions);
    Layout l = wf_layout(m);
    auto order = focus_order_source_tabs(l, m);
    CB_CHECK(order.size() >= 5 && order[0].control == Control::TabFiles && order[2].control == Control::TabCheck && order[3].control == Control::FnFilter &&
             order[4].control == Control::FnRow);
    m.cmp_index = 4;
    order = focus_order_source_tabs(l, m);
    CB_CHECK(order[3].control == Control::FnAll && order[4].control == Control::FnPicker);
    // Convert's order: the buttons, the tabs, the view; Copy path only where it exists
    m = wf_model(SourceTab::Functions);
    l = wf_layout(m);
    bool copy = false;
    for (const PageHit& h : focus_order_convert(l, m)) copy = copy || h.control == Control::CopyPath;
    CB_CHECK(!copy);
    m = wf_model(SourceTab::Files);
    l = wf_layout(m);
    copy = false;
    for (const PageHit& h : focus_order_convert(l, m)) copy = copy || h.control == Control::CopyPath;
    CB_CHECK(copy);
});

CB_TEST("app.window_functions.the_palette_box_is_centred_near_the_top_with_a_row_per_result_row", [] {
    Model m;
    m.palette_open = true;
    m.palette_items = palette::make_items(tools::all(), {{"lz32.dll", "converted 2 minutes ago", false}});
    m.palette_result = palette::search(m.palette_items, "s");
    const Layout l = compute_layout(View::Idle, 1180, 700, kRailWidth);
    const PaletteLayout p = compute_palette_layout(l, m);
    CB_CHECK(std::fabs(p.box.w - kPaletteWidth) < 0.01f && std::fabs(p.box.x + p.box.w / 2 - l.width / 2) < 0.01f);
    CB_CHECK(p.box.y >= kTitleBarHeight && p.input.h == kPaletteInputHeight && p.box.bottom() <= l.status_bar.y);
    CB_CHECK_EQ(p.rows.size(), m.palette_result.rows.size());
    for (std::size_t i = 0; i < p.rows.size(); ++i) {
        CB_CHECK(p.rows[i].w == 0 || (p.rows[i].y >= p.input.bottom() && p.rows[i].bottom() <= p.box.bottom()));
        if (i > 0 && p.rows[i].w > 0) CB_CHECK(p.rows[i].y >= p.rows[i - 1].bottom());
        if (p.rows[i].w > 0) CB_CHECK(p.rows[i].h == (m.palette_result.rows[i].heading ? kPaletteHeadingHeight : kPaletteRowHeight));
    }
    // a click is on a choosable row (headings are not choosable)
    int first_item = -1;
    for (std::size_t i = 0; i < m.palette_result.rows.size(); ++i)
        if (!m.palette_result.rows[i].heading && first_item < 0) first_item = static_cast<int>(i);
    CB_CHECK(first_item > 0);
    const Rect& r = p.rows[static_cast<std::size_t>(first_item)];
    CB_CHECK_EQ(palette_pick_at(p, m, r.x + 20, r.y + 5), 0);
    CB_CHECK_EQ(palette_pick_at(p, m, p.rows[0].x + 20, p.rows[0].y + 5), -1);   // a heading
    CB_CHECK_EQ(palette_pick_at(p, m, 1, 1), -1);
});

CB_TEST("app.window_functions.an_empty_palette_result_is_a_short_box", [] {
    Model m;
    m.palette_open = true;
    m.palette_items = palette::make_items(tools::all(), {});
    m.palette_query = "qqqqqq";
    m.palette_result = palette::search(m.palette_items, m.palette_query);
    const PaletteLayout p = compute_palette_layout(compute_layout(View::Idle, 1180, 700, kRailWidth), m);
    CB_CHECK(p.rows.empty() && p.box.h < kPaletteInputHeight + 60);
});

CB_TEST("app.window_functions.a_small_window_hides_the_rows_that_do_not_fit_instead_of_drawing_over_the_status_bar", [] {
    Model m;
    m.palette_open = true;
    m.palette_items = palette::make_items(tools::all(), {{"a", "", false}, {"b", "", false}, {"c", "", false}});
    m.palette_result = palette::search(m.palette_items, "");
    const Layout l = compute_layout(View::Idle, kMinWidth, kMinHeight, kRailWidth);
    const PaletteLayout p = compute_palette_layout(l, m);
    for (const Rect& r : p.rows) CB_CHECK(r.w == 0 || r.bottom() <= l.status_bar.y);
    CB_CHECK(p.box.bottom() <= l.status_bar.y);
});

}  // namespace
}  // namespace cb::app::window
