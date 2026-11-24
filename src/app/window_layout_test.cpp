// Tests of the window's layout (window_layout.hpp): the metrics of the approved design, and what a point is over.
#include "app/window_layout.hpp"

#include <algorithm>
#include <cmath>

#include "core/testing.hpp"

namespace cb::app::window {
namespace {

bool wl_near(float a, float b) { return std::fabs(a - b) < 0.01f; }

CB_TEST("app.window_layout.the_running_clock_and_the_time_taken", [] {
    using std::chrono::seconds;
    CB_CHECK_EQ(elapsed_clock(seconds{0}), "0:00");
    CB_CHECK_EQ(elapsed_clock(seconds{7}), "0:07");
    CB_CHECK_EQ(elapsed_clock(seconds{12 * 60 + 4}), "12:04");
    CB_CHECK_EQ(elapsed_clock(seconds{3600 + 2 * 60 + 3}), "1:02:03");
    CB_CHECK_EQ(elapsed_clock(seconds{-5}), "0:00");
    CB_CHECK_EQ(took_words(seconds{7}), "7 s");
    CB_CHECK_EQ(took_words(seconds{12 * 60 + 4}), "12 min 4 s");
    CB_CHECK_EQ(took_words(seconds{3600 + 2 * 60 + 3}), "1 h 2 min");
    // Working: the clock sits at the right end of the detail line, beside it and not over it
    const Layout l = compute_layout(View::Working, 1180, 700);
    CB_CHECK(l.elapsed.w > 0);
    CB_CHECK(wl_near(l.elapsed.y, l.detail.y));
    CB_CHECK(l.detail.right() <= l.elapsed.x + 0.01f);
    CB_CHECK(wl_near(l.elapsed.right(), l.bar.right()));
});

CB_TEST("app.window_layout.one_card_the_page_s_whole_width_program_then_source_in_its_place", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(wl_near(l.title_bar.h, 48));
    CB_CHECK(wl_near(l.left_card.x, 24));
    CB_CHECK(wl_near(1180 - l.left_card.right(), 24));
    CB_CHECK(wl_near(l.right_card.x, l.left_card.x) && wl_near(l.right_card.w, l.left_card.w));   // the Source card takes the Program card's place
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
    CB_CHECK(l.drop_line2.bottom() < l.drop.bottom());
    // a corner of the "+" button's rectangle is not the button, its centre is
    CB_CHECK(hit_test(l, View::Idle, l.plus.x + 1, l.plus.y + 1) == Control::None);
    CB_CHECK(hit_test(l, View::Idle, l.plus.x + 36, l.plus.y + 36) == Control::Plus);
    // no Browse beside it: the "+" is the one way to the file picker (and dragging a file, and Ctrl+O)
    CB_CHECK(!control_exists(Control::Browse, View::Idle));
    CB_CHECK(!control_exists(Control::Browse, View::Unsupported));
    const std::vector<Control> order = focus_order(View::Idle);
    CB_CHECK(std::find(order.begin(), order.end(), Control::Browse) == order.end());
});

CB_TEST("app.window_layout.the_output_and_its_buttons_are_the_source_card_s_shown_when_done", [] {
    const Layout l = compute_layout(View::Done, 1180, 700);
    CB_CHECK(!control_exists(Control::OpenFolder, View::Idle));
    CB_CHECK(!control_exists(Control::CopyPath, View::Working));
    CB_CHECK(control_enabled(Control::OpenFolder, View::Done));
    CB_CHECK(control_enabled(Control::CopyPath, View::Done));
    CB_CHECK(hit_test(l, View::Done, l.open_folder.x + 5, l.open_folder.y + 5) == Control::OpenFolder);
    CB_CHECK(hit_test(l, View::Done, l.copy_path.x + 5, l.copy_path.y + 5) == Control::CopyPath);
    // one column in the middle of the card: the check, the name, Output, and the three buttons in a row under it
    CB_CHECK(wl_near(l.field.x + l.field.w / 2, l.right_card.x + l.right_card.w / 2));
    CB_CHECK(l.file_name.bottom() < l.output_label.y && l.field.bottom() < l.open_folder.y);
    CB_CHECK(l.open_folder.right() < l.copy_path.x && l.copy_path.right() < l.browse.x);
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
    CB_CHECK(order.size() == 3);   // (Open folder, Copy path, Convert another file)
    CB_CHECK(order.front() == Control::OpenFolder);
});

CB_TEST("app.window_layout.the_language_drop_down_is_in_the_program_card_s_header_and_reachable", [] {
    const Layout l = compute_layout(View::Idle, 1180, 700);
    CB_CHECK(wl_near(l.language_combo.right(), l.left_card.right() - 24));   // at the right of the header
    CB_CHECK(l.language_combo.bottom() < l.drop.y);                           // above the drop area
    CB_CHECK(l.left_title.right() < l.language_label.x);                       // the title does not run under it
    CB_CHECK(wl_near(l.language_combo.h, 34));
    CB_CHECK(hit_test(l, View::Idle, l.language_combo.x + 5, l.language_combo.y + 5) == Control::Language);
    CB_CHECK(control_enabled(Control::Language, View::Idle));
    CB_CHECK(!control_exists(Control::Language, View::Done));             // (the Source card has no language)
    CB_CHECK(!control_enabled(Control::Language, View::Working));   // (the language of a run under way cannot change)
    for (const View v : {View::Idle, View::Unsupported, View::Failed}) {
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
        CB_CHECK(l.drop.right() <= l.left_card.right());
        CB_CHECK(l.language_combo.x > l.left_card.x);
        if (v == View::Working) {
            CB_CHECK(l.file_icon.y >= l.drop.y);
            CB_CHECK(l.cancel.bottom() <= l.drop.bottom());
        } else if (v == View::Done) {
            CB_CHECK(l.file_icon.y >= l.right_desc.bottom());
            CB_CHECK(l.browse.bottom() <= l.right_card.bottom());
            CB_CHECK(l.browse.right() <= l.right_card.right());
        } else {
            CB_CHECK(l.plus.y >= l.drop.y);
            CB_CHECK(l.drop_line2.bottom() <= l.drop.bottom());
        }
    }
});

CB_TEST("app.window_stream.characters_appear_one_at_a_time_at_a_steady_pace", [] {
    CharStream s;
    s.push({"ABCDEFGHIJ"});
    CB_CHECK(s.typing());
    s.advance(0.04);   // 10 waiting: about 126 a second, so 5 in 40 ms
    CB_CHECK_EQ(s.visible(3).size(), 1u);
    CB_CHECK_EQ(s.visible(3).back(), std::string("ABCDE"));
    s.advance(1.0);
    CB_CHECK(!s.typing());
    CB_CHECK(s.visible(3) == std::vector<std::string>{"ABCDEFGHIJ"});
});

CB_TEST("app.window_stream.lines_scroll_and_only_the_last_rows_are_drawn", [] {
    CharStream s;
    s.push({"one", "two", "three", "four"});
    s.advance(10.0);
    CB_CHECK(s.visible(2) == (std::vector<std::string>{"three", "four"}));
    s.push({"five"});
    s.advance(0.01);   // "five" begun: the last row is the line being typed
    const std::vector<std::string> rows = s.visible(3);
    CB_CHECK_EQ(rows.size(), 3u);
    CB_CHECK_EQ(rows[0], std::string("three"));
    CB_CHECK(std::string("five").starts_with(rows[2]));
});

CB_TEST("app.window_stream.a_backlog_types_faster_so_it_keeps_up_with_the_work", [] {
    CharStream slow, busy;
    slow.push({std::string(40, 'x')});
    busy.push(std::vector<std::string>(200, std::string(40, 'x')));
    slow.advance(0.1);
    busy.advance(0.1);
    // 0.1 s at the steady pace is about 11 characters; with 8,000 waiting the stream types whole lines in the same time
    CB_CHECK(slow.visible(1).back().size() < 20);
    CB_CHECK(busy.visible(50).size() > 10);
});

CB_TEST("app.window_stream.without_animation_everything_shows_at_once", [] {
    CharStream s;
    s.push({"a", "b", "c"});
    s.show_all();
    CB_CHECK(!s.typing());
    CB_CHECK(s.visible(5) == (std::vector<std::string>{"a", "b", "c"}));
    s.clear();
    CB_CHECK(s.empty());
    CB_CHECK(s.visible(5).empty());
});

CB_TEST("app.window_stream.a_huge_backlog_drops_its_oldest_lines", [] {
    CharStream s;
    std::vector<std::string> many;
    for (int i = 0; i < 1000; ++i) many.push_back("line " + std::to_string(i));
    s.push(many);
    s.show_all();
    const std::vector<std::string> rows = s.visible(CharStream::kStreamKeptLines);
    CB_CHECK_EQ(rows.back(), std::string("line 999"));
    CB_CHECK(rows.size() <= CharStream::kStreamKeptLines);
});

}  // namespace
}  // namespace cb::app::window
