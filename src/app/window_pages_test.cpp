// Tests of the layout of the rail, the Tools page and the Settings page (window_pages.hpp).
#include "app/window_pages.hpp"

#include <algorithm>
#include <cmath>

#include "core/testing.hpp"

namespace cb::app::window {
namespace {

bool wp_near(float a, float b) { return std::fabs(a - b) < 0.01f; }

Model wp_tools_model() {
    Model m;
    m.page = Page::Tools;
    m.tool_rows = {{true, "Rebuild source", -1}, {false, "Windows program", 0}, {false, "DOS program", 1}, {true, "Inspect", -1}, {false, "What is this file?", 2}};
    m.tool_selected = 0;
    m.tool_fields = {{"Program", 0, "", {}, true, ""}, {"Stub", 0, "", {}, false, ""}, {"Check", 4, "", {}, false, ""}, {"Action", 3, "list", {"list", "extract"}, true, ""}};
    return m;
}

CB_TEST("app.window_pages.the_rail_is_64_wide_with_three_items_and_the_cards_start_after_it", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700, kRailWidth);
    CB_CHECK(wp_near(l.rail.w, 64));
    CB_CHECK(wp_near(l.rail_convert.w, 48) && wp_near(l.rail_convert.h, 56));
    CB_CHECK(l.rail_tools.y > l.rail_convert.bottom());
    CB_CHECK(l.rail_settings.y > l.rail_tools.bottom() && l.rail_settings.bottom() < l.status_bar.y);   // Settings at the bottom
    CB_CHECK(l.left_card.x >= l.rail.right());
    CB_CHECK(wp_near(1180 - l.right_card.right(), 24));
    CB_CHECK(wp_near(l.left_card.w, l.right_card.w));
    CB_CHECK(hit_test_rail(l, l.rail_tools.x + 5, l.rail_tools.y + 5).control == Control::NavTools);
    CB_CHECK(hit_test_rail(l, l.rail_settings.x + 5, l.rail_settings.y + 5).control == Control::NavSettings);
    CB_CHECK(hit_test_rail(l, 500, 300).control == Control::None);
    // without a rail the Convert page is as it always was
    CB_CHECK(wp_near(compute_layout(View::Idle, 1180, 700).left_card.x, 24));
});

CB_TEST("app.window_pages.the_tools_page_is_a_list_card_and_a_tool_card", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m = wp_tools_model();
    m.tool_output_shown = true;   // (the pane shown: its place under the bar)
    const ToolsLayout t = compute_tools_layout(shell, m);
    CB_CHECK(wp_near(t.list_card.w, kToolListWidth));
    CB_CHECK(t.form_card.x > t.list_card.right() && wp_near(1180 - t.form_card.right(), 24));
    CB_CHECK(t.search.bottom() < t.list_area.y && t.list_title.bottom() < t.search.y);
    CB_CHECK(t.rows.size() == m.tool_rows.size());
    CB_CHECK(wp_near(t.rows[0].h, kToolHeadingHeight) && wp_near(t.rows[1].h, kToolRowHeight));
    CB_CHECK(t.form_title.y < t.form_desc.y && t.form_desc.bottom() <= t.fields_area.y);
    CB_CHECK(t.run.y >= t.fields_area.bottom() && t.bar.y > t.run.bottom() && t.output.y > t.bar.bottom());
    CB_CHECK(t.output.bottom() <= t.form_card.bottom() && t.output.h > 80);
    // what a point is over
    CB_CHECK(hit_test_tools(t, m, t.search.x + 4, t.search.y + 4) == PageHit({Control::ToolSearch, -1}));
    CB_CHECK(hit_test_tools(t, m, t.rows[2].x + 4, t.rows[2].y + 4) == PageHit({Control::ToolRow, 1}));   // (the tool's index, not the row's)
    CB_CHECK(hit_test_tools(t, m, t.rows[0].x + 4, t.rows[0].y + 4).control == Control::None);           // a heading is not a button
    CB_CHECK(hit_test_tools(t, m, t.run.x + 4, t.run.y + 4).control == Control::Run);
    CB_CHECK(hit_test_tools(t, m, t.form.fields[0].input.x + 4, t.form.fields[0].input.y + 4) == PageHit({Control::Field, 0}));
    CB_CHECK(hit_test_tools(t, m, t.form.fields[0].browse.x + 4, t.form.fields[0].browse.y + 4) == PageHit({Control::FieldBrowse, 0}));
    CB_CHECK(t.form.fields[3].browse.w == 0);   // a choice has no Browse
});

CB_TEST("app.window_pages.a_tool_is_a_drop_box_a_row_of_choice_buttons_run_and_open_folder", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m = wp_tools_model();
    FieldItem drop;
    drop.label = "Java program";
    drop.kind = 5;
    drop.accept = 2;   // a file or a folder: two buttons, File... and Folder...
    m.tool_fields = {drop};
    m.tool_choice_label = "Platform";
    m.tool_choices = {"PC", "Xbox 360"};
    ToolsLayout t = compute_tools_layout(shell, m);
    const FieldRects& r = t.form.fields[0];
    CB_CHECK(wp_near(r.input.h, kDropBoxHeight) && wp_near(r.input.w, t.fields_area.w));   // the box is the form's width
    CB_CHECK(r.browse.w > 0 && r.browse_folder.w > 0 && r.browse.right() < r.browse_folder.x && r.browse_folder.right() < r.input.right());
    CB_CHECK(hit_test_tools(t, m, r.input.x + 10, r.input.y + 10) == PageHit({Control::FieldBrowse, 0}));          // the whole box browses
    CB_CHECK(hit_test_tools(t, m, r.browse_folder.x + 4, r.browse_folder.y + 4) == PageHit({Control::FieldBrowseFolder, 0}));
    CB_CHECK(t.choice_buttons.size() == 2 && t.choice_label.y >= r.input.bottom() && t.choice_buttons[1].x > t.choice_buttons[0].right());
    CB_CHECK(t.run.y > t.choice_buttons[0].bottom());
    CB_CHECK(hit_test_tools(t, m, t.choice_buttons[1].x + 4, t.choice_buttons[1].y + 4) == PageHit({Control::ToolChoice, 1}));
    CB_CHECK(t.open_folder.w == 0);   // no result yet
    m.tool_result = "C:\\Games\\a1.2.6-converted";
    t = compute_tools_layout(shell, m);
    CB_CHECK(t.open_folder.w > 0 && t.open_folder.x > t.run.right() && t.run_note.x > t.open_folder.right());
    CB_CHECK(hit_test_tools(t, m, t.open_folder.x + 4, t.open_folder.y + 4).control == Control::OpenResult);
    // Tab: the box's two buttons, the choices, Run, Open folder (a drop box itself is no text field)
    const std::vector<PageHit> order = focus_order_tools(m);
    const std::vector<PageHit> want = {{Control::ToolSearch, -1}, {Control::ToolRow, 0}, {Control::FieldBrowse, 0}, {Control::FieldBrowseFolder, 0},
                                       {Control::ToolChoice, 0}, {Control::ToolChoice, 1}, {Control::Run, -1}, {Control::OpenResult, -1}};
    CB_CHECK(order.size() >= want.size() && std::equal(want.begin(), want.end(), order.begin()));
    // a drop box for a file alone has one Browse
    m.tool_fields[0].accept = 0;
    CB_CHECK(compute_tools_layout(shell, m).form.fields[0].browse_folder.w == 0);
});

CB_TEST("app.window_pages.the_output_is_hidden_until_its_button_shows_it_and_the_clock_sits_beside_it", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m = wp_tools_model();
    m.tool_result = "C:\\Games\\a1.2.6-converted";
    ToolsLayout t = compute_tools_layout(shell, m);
    CB_CHECK(!m.tool_output_shown && t.output.w == 0 && t.output.h == 0);   // hidden at first
    CB_CHECK(wp_near(t.output_toggle.right(), t.form_title.right()) && wp_near(t.output_toggle.y, t.run.y));
    CB_CHECK(t.elapsed.w > 0 && t.elapsed.right() < t.output_toggle.x && t.run_note.right() <= t.elapsed.x);
    CB_CHECK(t.run_note.x > t.open_folder.right());
    CB_CHECK(hit_test_tools(t, m, t.output_toggle.x + 4, t.output_toggle.y + 4).control == Control::ToolOutputToggle);
    CB_CHECK(hit_test_tools(t, m, t.form_title.x + 20, t.bar.bottom() + 40).control != Control::OutputPane);   // no pane to scroll
    const std::vector<PageHit> order = focus_order_tools(m);
    CB_CHECK(std::find(order.begin(), order.end(), PageHit({Control::ToolOutputToggle, -1})) != order.end());
    m.tool_output_shown = true;
    t = compute_tools_layout(shell, m);
    CB_CHECK(t.output.h > 100 && t.output.y > t.bar.bottom() && t.output.bottom() <= t.form_card.bottom());
    CB_CHECK(hit_test_tools(t, m, t.output.x + 20, t.output.y + 20).control == Control::OutputPane);
});

CB_TEST("app.window_pages.a_form_stacks_its_fields_and_a_switch_has_its_label_beside_it", [] {
    const Model m = wp_tools_model();
    const FormLayout f = layout_form({100, 100, 400, 0}, m.tool_fields, 0);
    CB_CHECK(f.fields.size() == 4);
    CB_CHECK(wp_near(f.fields[0].input.h, kFieldInputHeight));
    CB_CHECK(f.fields[1].label.y > f.fields[0].input.bottom());
    CB_CHECK(wp_near(f.fields[0].input.w + 8 + f.fields[0].browse.w, 400));   // the text box and Browse fill the width
    CB_CHECK(wp_near(f.fields[2].input.w, 40) && wp_near(f.fields[2].input.h, 20));   // the switch
    CB_CHECK(f.fields[2].label.x > f.fields[2].input.right());
    CB_CHECK(f.content_height > 0);
    // scrolling moves everything up
    const FormLayout g = layout_form({100, 100, 400, 0}, m.tool_fields, 50);
    CB_CHECK(wp_near(g.fields[0].input.y, f.fields[0].input.y - 50));
    CB_CHECK(wp_near(g.content_height, f.content_height));
});

CB_TEST("app.window_pages.tab_visits_the_page_and_ends_at_the_rail", [] {
    Model m = wp_tools_model();
    const std::vector<PageHit> order = focus_order_tools(m);
    CB_CHECK(order.front().control == Control::ToolSearch);
    CB_CHECK(order[1] == PageHit({Control::ToolRow, 0}));
    CB_CHECK(order[order.size() - 6].control == Control::Run && order[order.size() - 5].control == Control::ToolOutputToggle);
    CB_CHECK(order.back().control == Control::NavSettings);
    std::size_t browse = 0;
    for (const PageHit& h : order) browse += h.control == Control::FieldBrowse;
    CB_CHECK(browse == 2);   // (the two file fields)
    m.tool_selected = -1;
    CB_CHECK(focus_order_tools(m).size() == 5);   // the search box and the rail
    m.setting_fields = {{"Theme", 3, "System", {"System", "Light", "Dark"}, true, ""}, {"masm", 0, "", {}, false, ""}};
    CB_CHECK(focus_order_settings(m).size() == 2 + 1 + 4);
});

CB_TEST("app.window_pages.advanced_settings_scroll_a_long_form", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m;
    m.settings_advanced = true;
    for (int i = 0; i < 40; ++i) m.setting_fields.push_back({"key" + std::to_string(i), i % 3 == 0 ? 0 : 2, "", {}, false, ""});
    const SettingsLayout s = compute_settings_layout(shell, m);
    CB_CHECK(s.form.content_height > s.fields_area.h);
    CB_CHECK(hit_test_settings(s, m, s.form.fields[0].input.x + 3, s.form.fields[0].input.y + 3) == PageHit({Control::Field, 0}));
    CB_CHECK(hit_test_settings(s, m, s.form.fields[39].input.x + 3, s.form.fields[39].input.y + 3).control == Control::None);   // below the card: scrolled out
    m.setting_scroll = s.form.fields[39].input.y - s.fields_area.y - 20;
    const SettingsLayout moved = compute_settings_layout(shell, m);
    CB_CHECK(moved.fields_area.contains(moved.form.fields[39].input.x + 3, moved.form.fields[39].input.y + 3));
});

Model wp_settings_model() {
    Model m;
    m.page = Page::Settings;
    m.setting_fields = {{"Theme", 3, "System", {"System", "Light", "Dark"}, false, ""}, {"Save converted source to", 1, "C:\\Converted", {}, false, ""},
                        {"Keep the computer responsive", 4, "1", {}, false, ""}};
    m.setting_keys = {"", "converted", "low_load"};
    m.capabilities = {{"Ready", "Windows and DOS programs", true, "", true}, {"Xbox 360 games", "Needs the Xbox 360 SDK", false, "xbox360_sdk", true}};
    return m;
}

CB_TEST("app.window_pages.settings_are_rows_with_the_control_on_the_right", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    const Model m = wp_settings_model();
    const SettingsLayout s = compute_settings_layout(shell, m);
    CB_CHECK(s.field_rows.size() == 3 && s.capability_rows.size() == 2 && s.locate.size() == 2);
    CB_CHECK(s.fields_area.w <= kSettingsMaxWidth);
    CB_CHECK(s.general_heading.bottom() <= s.field_rows[0].y);
    CB_CHECK(s.field_rows[2].bottom() <= s.can_heading.y && s.can_heading.bottom() <= s.capability_rows[0].y);
    CB_CHECK(s.capability_rows[1].bottom() <= s.advanced_row.y);
    CB_CHECK(s.advanced_row.bottom() <= s.card.bottom());   // everything fits the default window: no scrolling
    for (std::size_t i = 0; i < 3; ++i) {   // each control inside its row, at the right, beside the words
        const FieldRects& r = s.form.fields[i];
        const Rect& control = m.setting_fields[i].kind == 1 ? r.browse : r.input;
        CB_CHECK(control.w > 0 && control.x >= s.field_rows[i].x && control.right() <= s.field_rows[i].right());
        CB_CHECK(control.y >= s.field_rows[i].y && control.bottom() <= s.field_rows[i].bottom());
        CB_CHECK(r.label.right() < control.x);
    }
    CB_CHECK(s.form.fields[1].input.w == 0);   // a folder is Change..., not a text box
    CB_CHECK(s.locate[0].w == 0 && s.locate[1].w > 0);   // Locate... only on what is missing
    CB_CHECK(hit_test_settings(s, m, s.form.fields[0].input.x + 3, s.form.fields[0].input.y + 3) == PageHit({Control::Field, 0}));
    CB_CHECK(hit_test_settings(s, m, s.form.fields[1].browse.x + 3, s.form.fields[1].browse.y + 3) == PageHit({Control::FieldBrowse, 1}));
    CB_CHECK(hit_test_settings(s, m, s.locate[1].x + 3, s.locate[1].y + 3) == PageHit({Control::Locate, 1}));
    CB_CHECK(hit_test_settings(s, m, s.advanced_row.x + 30, s.advanced_row.y + 20).control == Control::SettingsAdvanced);
    CB_CHECK(hit_test_settings(s, m, s.capability_rows[0].x + 30, s.capability_rows[0].y + 20).control == Control::None);
    const std::vector<PageHit> order = focus_order_settings(m);
    CB_CHECK(order.size() == 3 + 1 + 1 + 4);   // the three settings, Locate..., Advanced, the rail
    CB_CHECK(order[1] == PageHit({Control::FieldBrowse, 1}));
    CB_CHECK(order[3] == PageHit({Control::Locate, 1}));
    CB_CHECK(order[4].control == Control::SettingsAdvanced);
});

CB_TEST("app.window_pages.advanced_has_back_beside_its_title", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m = wp_settings_model();
    m.settings_advanced = true;
    m.setting_fields = {{"MASM assembler", 0, "", {}, false, ""}, {"Jobs at once", 2, "4", {}, false, ""}};
    m.setting_keys = {"masm", "workers"};
    const SettingsLayout s = compute_settings_layout(shell, m);
    CB_CHECK(s.back.w > 0 && s.back.right() < s.title.x);
    CB_CHECK(hit_test_settings(s, m, s.back.x + 5, s.back.y + 5).control == Control::SettingsBack);
    CB_CHECK(s.form.fields.size() == 2 && s.form.fields[0].input.w > 0 && s.form.fields[0].browse.w > 0);
    const std::vector<PageHit> order = focus_order_settings(m);
    CB_CHECK(order.front().control == Control::SettingsBack);
    CB_CHECK(order.size() == 1 + 3 + 4);   // Back, the file field and its Browse, the text field, the rail
});

}  // namespace
}  // namespace cb::app::window
