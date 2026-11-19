// Tests of the layout of the rail, the Tools page and the Settings page (window_pages.hpp).
#include "cb/app/window_pages.hpp"

#include <cmath>

#include "cb/core/testing.hpp"

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
    const Model m = wp_tools_model();
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
    CB_CHECK(order[order.size() - 4].control == Control::Run);
    CB_CHECK(order.back().control == Control::NavSettings);
    std::size_t browse = 0;
    for (const PageHit& h : order) browse += h.control == Control::FieldBrowse;
    CB_CHECK(browse == 2);   // (the two file fields)
    m.tool_selected = -1;
    CB_CHECK(focus_order_tools(m).size() == 4);   // the search box and the rail
    m.setting_fields = {{"Theme", 3, "System", {"System", "Light", "Dark"}, true, ""}, {"nasm", 0, "", {}, false, ""}};
    CB_CHECK(focus_order_settings(m).size() == 2 + 1 + 3);
});

CB_TEST("app.window_pages.the_settings_page_scrolls_a_long_form", [] {
    const Layout shell = compute_layout(View::Idle, 1180, 700, kRailWidth);
    Model m;
    for (int i = 0; i < 40; ++i) m.setting_fields.push_back({"key" + std::to_string(i), i % 3 == 0 ? 0 : 2, "", {}, false, ""});
    const SettingsLayout s = compute_settings_layout(shell, m);
    CB_CHECK(s.form.content_height > s.fields_area.h);
    CB_CHECK(hit_test_settings(s, m, s.form.fields[0].input.x + 3, s.form.fields[0].input.y + 3) == PageHit({Control::Field, 0}));
    CB_CHECK(hit_test_settings(s, m, s.form.fields[39].input.x + 3, s.form.fields[39].input.y + 3).control == Control::None);   // below the card: scrolled out
    m.setting_scroll = s.form.fields[39].input.y - s.fields_area.y - 20;
    const SettingsLayout moved = compute_settings_layout(shell, m);
    CB_CHECK(moved.fields_area.contains(moved.form.fields[39].input.x + 3, moved.form.fields[39].input.y + 3));
});

}  // namespace
}  // namespace cb::app::window
