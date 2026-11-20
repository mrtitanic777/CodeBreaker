// Tests of the Convert page's lists (window_convert.hpp and window_layout.hpp's Extras): the Recent rows, the batch queue, the Source card's preview and
// the output box, what a point is over, and what Tab visits.
#include "app/window_convert.hpp"

#include <algorithm>
#include <cmath>

#include "core/testing.hpp"

namespace cb::app::window {
namespace {

bool wc_near(float a, float b) { return std::fabs(a - b) < 0.01f; }

Model wc_recent_model(std::size_t rows) {
    Model m;
    m.view = View::Idle;
    for (std::size_t i = 0; i < rows; ++i) m.recent.push_back({"file" + std::to_string(i) + ".dll", "C", "3 files", false, false});
    return m;
}

Model wc_preview_model() {
    Model m;
    m.view = View::Done;
    m.output_path = "C:\\Converted\\lz32";
    m.preview_files = {{"lz32.c", false}, {"lz32.asm", false}, {"lz32.inc", false}, {"build.cmd", true}};
    for (int i = 0; i < 100; ++i) m.preview_lines.push_back(std::string(i == 3 ? 400 : 20, 'x'));
    return m;
}

Model wc_queue_model(std::size_t rows) {
    Model m;
    m.view = View::Working;
    for (std::size_t i = 0; i < rows; ++i) m.queue.push_back({"f" + std::to_string(i), "", "Waiting", 0, 0});
    return m;
}

CB_TEST("app.window_convert.extras_follow_what_the_card_of_a_view_holds", [] {
    Model m = wc_recent_model(7);
    m.queue = wc_queue_model(2).queue;
    m.preview_files = wc_preview_model().preview_files;
    CB_CHECK(extras_for(m, View::Idle).recent_rows == 5);   // (at most five)
    CB_CHECK(extras_for(m, View::Working).recent_rows == 0 && extras_for(m, View::Working).queue);
    CB_CHECK(!extras_for(m, View::Idle).queue && !extras_for(m, View::Idle).preview);
    CB_CHECK(extras_for(m, View::Done).preview && !extras_for(m, View::Done).queue);
    CB_CHECK(!extras_for(Model{}, View::Done).preview);
});

CB_TEST("app.window_convert.recent_rows_sit_under_the_drop_area_which_gives_way_to_them", [] {
    const Layout plain = compute_layout(View::Idle, 1180, 700, kRailWidth);
    CB_CHECK(plain.recent_rows.empty());
    const Layout l = compute_layout(View::Idle, 1180, 700, kRailWidth, Extras{3, false, false});
    CB_CHECK(l.recent_rows.size() == 3 && l.recent_remove.size() == 3);
    CB_CHECK(l.drop.h < plain.drop.h && l.drop.h >= 180);
    CB_CHECK(l.recent_heading.y >= l.drop.bottom() && l.recent_rows[0].y >= l.recent_heading.bottom());
    CB_CHECK(l.recent_rows[1].y >= l.recent_rows[0].bottom() && l.recent_rows[2].bottom() <= l.left_card.bottom());
    CB_CHECK(wc_near(l.recent_rows[0].x, l.drop.x) && wc_near(l.recent_rows[0].w, l.drop.w));   // as wide as the drop area
    CB_CHECK(l.recent_remove[0].right() <= l.recent_rows[0].right() && l.recent_remove[0].y >= l.recent_rows[0].y);
    // the "+" and its two lines stay in the middle of the smaller drop area
    CB_CHECK(l.plus.y > l.drop.y && l.plus.bottom() < l.drop.bottom());
    CB_CHECK(l.drop_line2.bottom() < l.drop.bottom());
    // the other views have no Recent list
    CB_CHECK(compute_layout(View::Working, 1180, 700, kRailWidth, Extras{3, false, false}).recent_rows.empty());
    // a short window shows fewer rows rather than a drop area under 180
    const Layout small = compute_layout(View::Idle, 800, 600, kRailWidth, Extras{5, false, false});
    CB_CHECK(small.recent_rows.size() < 5 && small.drop.h >= 180);
});

CB_TEST("app.window_convert.a_point_is_over_a_recent_row_its_remove_or_the_old_controls", [] {
    Model m = wc_recent_model(3);
    m.recent[1].missing = true;
    const Layout l = compute_layout(View::Idle, 1180, 700, kRailWidth, extras_for(m, View::Idle));
    const auto mid = [](const Rect& r) { return std::pair<float, float>{r.x + r.w / 2, r.y + r.h / 2}; };
    auto at = mid(l.recent_rows[0]);
    CB_CHECK(hit_test_convert(l, m, at.first, at.second) == (PageHit{Control::RecentRow, 0}));
    at = mid(l.recent_rows[2]);
    CB_CHECK(hit_test_convert(l, m, at.first, at.second) == (PageHit{Control::RecentRow, 2}));
    at = mid(l.recent_remove[1]);   // a row whose folder is gone offers Remove
    CB_CHECK(hit_test_convert(l, m, at.first, at.second) == (PageHit{Control::RecentRemove, 1}));
    at = mid(l.recent_remove[0]);    // the others have none: the click is on the row
    CB_CHECK(hit_test_convert(l, m, at.first, at.second) == (PageHit{Control::RecentRow, 0}));
    CB_CHECK(hit_test_convert(l, m, l.plus.x + 36, l.plus.y + 36) == (PageHit{Control::Plus, -1}));
    CB_CHECK(hit_test_convert(l, m, l.language_combo.x + 5, l.language_combo.y + 5) == (PageHit{Control::Language, -1}));
    CB_CHECK(hit_test_convert(l, m, 1, 100) == PageHit{});
    // Tab: the "+", Language, then every row (Remove for the one whose folder is gone)
    const std::vector<PageHit> order = focus_order_convert(l, m);
    CB_CHECK(order.size() == 5);
    CB_CHECK(order[2] == (PageHit{Control::RecentRow, 0}) && order[3] == (PageHit{Control::RecentRemove, 1}) && order[4] == (PageHit{Control::RecentRow, 2}));
});

CB_TEST("app.window_convert.the_queue_lists_a_row_per_file_with_cancel_all_at_the_foot", [] {
    const Model m = wc_queue_model(6);
    const Layout l = compute_layout(View::Working, 1180, 700, kRailWidth, extras_for(m, View::Working));
    CB_CHECK(wc_near(l.cancel.x, l.left_card.x + 24) && l.cancel.bottom() <= l.left_card.bottom() - 19);
    CB_CHECK(l.hint.x >= l.cancel.right() && wc_near(l.hint.y, l.cancel.y));
    CB_CHECK(hit_test_convert(l, m, l.cancel.x + 5, l.cancel.y + 5) == (PageHit{Control::Cancel, -1}));
    const QueueLayout q = compute_queue_layout(l, m);
    CB_CHECK(q.rows.size() == 6);
    CB_CHECK(q.area.bottom() < l.cancel.y);
    CB_CHECK(q.rows[0].row.y >= l.left_desc.bottom() && wc_near(q.rows[1].row.y - q.rows[0].row.bottom(), kQueueRowGap));
    // each row: icon, name, bar, status from the left to the right, inside the row
    const QueueRowRects& r = q.rows[0];
    CB_CHECK(r.icon.x > r.row.x && r.icon.right() <= r.text.x && r.text.right() <= r.bar.x && r.bar.right() <= r.status.x && r.status.right() <= r.row.right());
    // six rows fit; many do not, and scroll
    CB_CHECK(wc_near(queue_max_scroll(q), 0));
    Model many = wc_queue_model(40);
    const QueueLayout big = compute_queue_layout(l, many);
    CB_CHECK(queue_max_scroll(big) > 100);
    many.queue_scroll = 100;
    CB_CHECK(wc_near(compute_queue_layout(l, many).rows[0].row.y, big.rows[0].row.y - 100));
    // without the queue the Working view keeps its one-file layout: Cancel in the middle
    const Layout single = compute_layout(View::Working, 1180, 700, kRailWidth);
    CB_CHECK(wc_near(single.cancel.x + single.cancel.w / 2, single.drop.x + single.drop.w / 2));
});

CB_TEST("app.window_convert.the_source_card_with_a_preview_has_the_path_row_above_the_files_and_the_code", [] {
    const Model m = wc_preview_model();
    const Layout l = compute_layout(View::Done, 1180, 700, kRailWidth, extras_for(m, View::Done));
    CB_CHECK(l.preview.w > 0 && wc_near(l.preview.x, l.right_card.x + 24) && wc_near(l.preview.right(), l.right_card.right() - 24));
    CB_CHECK(l.file_icon.bottom() < l.field.y && l.field.bottom() < l.preview.y);
    CB_CHECK(wc_near(l.preview.bottom(), l.right_card.bottom() - 20));
    // the box and Copy path share the path row; Open folder and Convert another file are at the header's right, above the tabs
    CB_CHECK(wc_near(l.field.y, l.copy_path.y));
    CB_CHECK(l.field.right() < l.copy_path.x && wc_near(l.copy_path.right(), l.preview.right()));
    CB_CHECK(l.open_folder.right() < l.browse.x && wc_near(l.browse.right(), l.preview.right()) && l.browse.bottom() < l.tabs.y);
    // the list on the left, the code on the right
    CB_CHECK(wc_near(l.preview_list.w, 220) && wc_near(l.preview_code.x, l.preview_list.right()) && wc_near(l.preview_code.right(), l.preview.right()));
    const PreviewLayout p = compute_preview_layout(l, m);
    CB_CHECK(p.file_rows.size() == 4 && wc_near(p.file_rows[1].y - p.file_rows[0].y, kFileRowHeight));
    CB_CHECK(p.list_area.x > l.preview_list.x && p.code_area.x > l.preview_code.x && p.code_area.right() < l.preview_code.right());
    CB_CHECK(p.visible_lines >= 15 && static_cast<float>(p.visible_lines) * kCodeLineHeight <= p.code_area.h + 0.01f);
    CB_CHECK(p.visible_columns > 60);
    // a window too narrow for 220 gives the list a third
    const Layout narrow = compute_layout(View::Done, 800, 600, kRailWidth, extras_for(m, View::Done));
    CB_CHECK(narrow.preview_list.w <= narrow.preview.w / 3 + 0.01f && narrow.field.w >= 120);
});

CB_TEST("app.window_convert.the_preview_scrolls_by_line_and_sideways_and_the_note_takes_a_row", [] {
    Model m = wc_preview_model();
    const Layout l = compute_layout(View::Done, 1180, 700, kRailWidth, extras_for(m, View::Done));
    const PreviewLayout p = compute_preview_layout(l, m);
    CB_CHECK(preview_max_first_line(m, p) == 100 - p.visible_lines);
    CB_CHECK(preview_max_hscroll(m, p) == 400 - p.visible_columns);   // (the longest line is 400 characters)
    Model short_file = m;
    short_file.preview_lines.assign(5, std::string(20, 'x'));
    CB_CHECK(preview_max_first_line(short_file, p) == 0);
    CB_CHECK(preview_max_hscroll(short_file, p) == 0);
    // the list scrolls when it has more files than fit
    Model files = m;
    files.preview_files.assign(60, PreviewFileItem{"f.c", false});
    const PreviewLayout many = compute_preview_layout(l, files);
    CB_CHECK(preview_list_max_scroll(many) > 100 && wc_near(preview_list_max_scroll(p), 0));
    files.preview_list_scroll = 90;
    CB_CHECK(wc_near(compute_preview_layout(l, files).file_rows[0].y, many.file_rows[0].y - 90));
    // a note ("Showing the first 128 KB of 3.4 MB.") takes a row under the code
    Model noted = m;
    noted.preview_note = "Showing the first 128 KB of 3.4 MB.";
    const PreviewLayout n = compute_preview_layout(l, noted);
    CB_CHECK(p.note_area.h == 0 && n.note_area.h > 0 && n.code_area.h < p.code_area.h && wc_near(n.note_area.y, n.code_area.bottom()));
    // no preview, no preview layout
    CB_CHECK(compute_preview_layout(compute_layout(View::Done, 1180, 700, kRailWidth), m).file_rows.empty());
});

CB_TEST("app.window_convert.a_point_is_over_a_file_the_code_or_the_output_box_of_the_source_card", [] {
    const Model m = wc_preview_model();
    const Layout l = compute_layout(View::Done, 1180, 700, kRailWidth, extras_for(m, View::Done));
    const PreviewLayout p = compute_preview_layout(l, m);
    CB_CHECK(hit_test_convert(l, m, p.file_rows[2].x + 5, p.file_rows[2].y + 5) == (PageHit{Control::PreviewFile, 2}));
    CB_CHECK(hit_test_convert(l, m, p.list_area.x + 5, p.file_rows[3].bottom() + 20) == PageHit{});   // (the empty part of the list)
    CB_CHECK(hit_test_convert(l, m, p.code_area.x + 30, p.code_area.y + 30) == (PageHit{Control::CodeView, -1}));
    CB_CHECK(hit_test_convert(l, m, l.field.x + 20, l.field.y + 10) == (PageHit{Control::OutputBox, -1}));
    CB_CHECK(hit_test_convert(l, m, l.open_folder.x + 5, l.open_folder.y + 5) == (PageHit{Control::OpenFolder, -1}));
    CB_CHECK(hit_test_convert(l, m, l.browse.x + 5, l.browse.y + 5) == (PageHit{Control::Browse, -1}));
    // Tab: the three buttons, the three tabs, the file list (at the selected file), the code
    Model chosen = m;
    chosen.preview_selected = 2;
    const std::vector<PageHit> order = focus_order_convert(l, chosen);
    CB_CHECK(order.size() == 8);
    CB_CHECK(order[0].control == Control::OpenFolder && order[3].control == Control::TabFiles && order[5].control == Control::TabCheck &&
             order[6] == (PageHit{Control::PreviewFile, 2}) && order[7].control == Control::CodeView);
});

CB_TEST("app.window_convert.without_a_preview_the_output_box_is_still_a_drag_source_with_its_hint_under_it", [] {
    const Model m = [] {
        Model x;
        x.view = View::Done;
        return x;
    }();
    const Layout l = compute_layout(View::Done, 1180, 700, kRailWidth, extras_for(m, View::Done));
    CB_CHECK(l.preview.w == 0);
    CB_CHECK(l.hint.y >= l.field.bottom() && l.hint.bottom() <= l.open_folder.y && wc_near(l.hint.x, l.field.x));
    CB_CHECK(l.field.w <= 560.01f);
    CB_CHECK(hit_test_convert(l, m, l.field.x + 20, l.field.y + 10) == (PageHit{Control::OutputBox, -1}));
    // the group is still centred in the card, and the three buttons are below the hint
    const float top = l.file_icon.y - l.right_desc.bottom(), foot = l.right_card.bottom() - 20 - l.open_folder.bottom();
    CB_CHECK(std::fabs(top - foot) < 1.5f);
    CB_CHECK(std::string(kDragHint).find("Explorer") != std::string::npos);
});

}  // namespace
}  // namespace cb::app::window
