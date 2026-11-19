// Tests of the window's layout (window_layout.hpp): the metrics of the approved design, and what a point is over.
#include "cb/app/window_layout.hpp"

#include <algorithm>
#include <cmath>

#include "cb/core/testing.hpp"

namespace cb::app::window {
namespace {

bool wl_near(float a, float b) { return std::fabs(a - b) < 0.01f; }

CB_TEST("app.window_layout.two_equal_cards_with_the_design_s_padding_and_gap", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(wl_near(l.title_bar.h, 48));
    CB_CHECK(wl_near(l.left_card.x, 24));
    CB_CHECK(wl_near(l.left_card.w, l.right_card.w));
    CB_CHECK(wl_near(l.right_card.x - l.left_card.right(), 16));
    CB_CHECK(wl_near(1180 - l.right_card.right(), 24));
    CB_CHECK(wl_near(l.status_bar.h, 32));
    CB_CHECK(wl_near(l.status_bar.y - l.left_card.bottom(), 16));
    CB_CHECK(wl_near(l.left_card.y, 56));
    // the caption buttons are 46 wide, close at the right edge
    CB_CHECK(wl_near(l.caption_close.right(), 1180));
    CB_CHECK(wl_near(l.caption_close.w, 46));
    CB_CHECK(wl_near(l.caption_maximize.right(), l.caption_close.x));
});

CB_TEST("app.window_layout.the_plus_button_is_a_72_circle_in_the_middle_of_the_drop_area", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(wl_near(l.plus.w, 72));
    CB_CHECK(wl_near(l.plus.x + l.plus.w / 2, l.drop.x + l.drop.w / 2));
    CB_CHECK(l.plus.y > l.drop.y);
    CB_CHECK(l.browse.bottom() < l.drop.bottom());
    // a corner of the "+" button's rectangle is not the button, its centre is
    CB_CHECK(hit_test(l, View::Idle, l.plus.x + 1, l.plus.y + 1) == Control::None);
    CB_CHECK(hit_test(l, View::Idle, l.plus.x + 36, l.plus.y + 36) == Control::Plus);
    CB_CHECK(hit_test(l, View::Idle, l.browse.x + 5, l.browse.y + 5) == Control::Browse);
});

CB_TEST("app.window_layout.the_output_buttons_exist_always_and_act_only_when_done", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(hit_test(l, View::Idle, l.open_folder.x + 5, l.open_folder.y + 5) == Control::OpenFolder);
    CB_CHECK(!control_enabled(Control::OpenFolder, View::Idle));
    CB_CHECK(!control_enabled(Control::CopyPath, View::Working));
    CB_CHECK(control_enabled(Control::OpenFolder, View::Done));
    CB_CHECK(control_enabled(Control::CopyPath, View::Done));
    CB_CHECK(wl_near(l.output_label.x, l.right_card.x + 24));
    CB_CHECK(wl_near(l.field.h, 34));
});

CB_TEST("app.window_layout.an_info_bar_pushes_the_drop_area_down_and_Cancel_is_only_while_working", [] {
    const Layout idle = compute_layout(View::Idle, 1180, 700);
    const Layout bad = compute_layout(View::Unsupported, 1180, 700);
    CB_CHECK(bad.drop.y > idle.drop.y + 100);
    CB_CHECK(bad.info_bar.bottom() < bad.drop.y);
    CB_CHECK(hit_test(bad, View::Unsupported, bad.try_another.x + 4, bad.try_another.y + 4) == Control::TryAnother);
    CB_CHECK(!control_exists(Control::Cancel, View::Idle));
    CB_CHECK(control_exists(Control::Cancel, View::Working));
    CB_CHECK(control_exists(Control::TryAnother, View::Failed));
    CB_CHECK(!control_exists(Control::TryAnother, View::Idle));
    CB_CHECK(hit_test(compute_layout(View::Working, 1180, 700), View::Working, 1, 100) == Control::None);
});

CB_TEST("app.window_layout.the_done_view_offers_another_file_where_browse_was", [] {
    const Layout l = compute_layout(View::Done, 1180, 700);
    CB_CHECK(hit_test(l, View::Done, l.browse.x + 3, l.browse.y + 3) == Control::Browse);
    const std::vector<Control> order = focus_order(View::Done);
    CB_CHECK(order.size() == 4);   // (Browse, Language, Open folder, Copy path)
    CB_CHECK(order.front() == Control::Browse);
});

CB_TEST("app.window_layout.the_language_drop_down_is_above_the_output_and_reachable", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(l.language_label.bottom() <= l.language_combo.y);
    CB_CHECK(l.language_combo.bottom() < l.output_label.y);   // above the Output box
    CB_CHECK(wl_near(l.language_combo.w, l.field.w));
    CB_CHECK(wl_near(l.language_combo.h, 34));
    CB_CHECK(l.language_combo.x >= l.right_card.x && l.language_combo.right() <= l.right_card.right());
    CB_CHECK(hit_test(l, View::Idle, l.language_combo.x + 5, l.language_combo.y + 5) == Control::Language);
    CB_CHECK(control_enabled(Control::Language, View::Idle));
    CB_CHECK(control_enabled(Control::Language, View::Done));
    CB_CHECK(!control_enabled(Control::Language, View::Working));   // (the language of a run under way cannot change)
    for (const View v : {View::Idle, View::Done, View::Unsupported, View::Failed}) {
        const std::vector<Control> order = focus_order(v);
        CB_CHECK(std::find(order.begin(), order.end(), Control::Language) != order.end());   // Tab reaches it
    }
});

CB_TEST("app.window_layout.the_open_list_has_a_row_per_language_and_opens_where_there_is_room", [] {
    std::vector<LanguageItem> items = {{"original", "Original", true, ""}, {"c", "C", false, "C# to C is not supported yet"}, {"cpp17", "C++17", true, ""},
                                       {"python", "Python", false, "Python output is not supported yet"}};
    const Layout l = compute_layout(View::Idle, 1180, 700);
    const LanguageList list = language_list_layout(l, items);
    CB_CHECK(list.rows.size() == items.size());
    CB_CHECK(list.outline.y >= l.language_combo.bottom());   // below the control
    CB_CHECK(list.outline.bottom() < l.status_bar.y);
    CB_CHECK(wl_near(list.rows[0].h, kLanguageRowHeight));
    CB_CHECK(wl_near(list.rows[1].h, kLanguageRowWithReasonHeight));   // a reason sits under the name
    CB_CHECK(language_item_at(list, list.rows[2].x + 3, list.rows[2].y + 3) == 2);
    CB_CHECK(language_item_at(list, 1, 1) == -1);
    // the arrows skip what cannot be chosen, and stay at the ends
    CB_CHECK(language_next_enabled(items, 0, 1) == 2);
    CB_CHECK(language_next_enabled(items, 2, 1) == 2);
    CB_CHECK(language_next_enabled(items, 2, -1) == 0);
    CB_CHECK(language_next_enabled(items, 0, -1) == 0);
    CB_CHECK(language_next_enabled(items, -1, 1) == 0);
    CB_CHECK(language_next_enabled(items, 4, -1) == 2);
    // with many items the list is cut to the room there is
    for (int i = 0; i < 20; ++i) items.push_back({"x", "X", false, "no"});
    const LanguageList tall = language_list_layout(compute_layout(View::Idle, kMinWidth, kMinHeight), items);
    CB_CHECK(tall.outline.y >= 56 && tall.outline.bottom() <= kMinHeight - kStatusBarHeight);
});

CB_TEST("app.window_layout.the_smallest_window_still_fits_its_parts", [] {
    for (const View v : {View::Idle, View::Working, View::Done, View::Unsupported, View::Failed}) {
        const Layout l = compute_layout(v, kMinWidth, kMinHeight);
        CB_CHECK(l.drop.h > 150);
        CB_CHECK(l.left_card.right() <= l.right_card.x);
        CB_CHECK(l.drop.right() <= l.left_card.right());
        if (v == View::Working || v == View::Done) {
            CB_CHECK(l.file_icon.y >= l.drop.y);
            CB_CHECK(l.cancel.bottom() <= l.drop.bottom());
        } else {
            CB_CHECK(l.plus.y >= l.drop.y);
            CB_CHECK(l.browse.bottom() <= l.drop.bottom());
        }
    }
});

}  // namespace
}  // namespace cb::app::window
