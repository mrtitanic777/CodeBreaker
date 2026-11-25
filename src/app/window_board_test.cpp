// Tests of window_board.hpp: columns, the fan and its deck, virtualisation, and what a point is over.
#include "app/window_board.hpp"

#include <cmath>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::window {
namespace {

Model wb_model(std::size_t per_pile) {
    Model m;
    m.page = Page::Board;
    m.board = std::make_shared<board::Board>();
    m.board->complete = true;
    for (std::size_t p = 0; p < board::kPileCount; ++p) {
        for (std::size_t i = 0; i < per_pile; ++i) {
            board::Card c;
            c.name = "f" + std::to_string(p) + "_" + std::to_string(i);
            c.rel = c.name;
            c.pile = static_cast<board::Pile>(p);
            m.board->cards.push_back(std::move(c));
        }
    }
    m.board_stacks = board::build_stacks(*m.board);
    return m;
}

Layout wb_shell(const Model& m, float w = 1180, float h = 700) { return compute_layout(m.view, w, h, kRailWidth, extras_for(m, m.view)); }

CB_TEST("app.window_board.the_rail_has_a_board_item_between_tools_and_settings", [] {
    const Model m = wb_model(1);
    const Layout l = wb_shell(m);
    CB_CHECK(l.rail_board.y >= l.rail_tools.bottom() && l.rail_board.bottom() <= l.rail_settings.y);
    CB_CHECK(hit_test_rail(l, l.rail_board.x + 5, l.rail_board.y + 5).control == Control::NavBoard);
});

CB_TEST("app.window_board.a_narrow_window_scrolls_the_columns_sideways_and_a_wide_one_fills", [] {
    Model m = wb_model(3);
    const BoardLayout narrow = compute_board_layout(wb_shell(m), m);
    CB_CHECK(narrow.column_width == kBoardColumnMin);
    CB_CHECK(board_max_hscroll(narrow) > 0);
    const BoardLayout wide = compute_board_layout(wb_shell(m, 2400, 1200), m);
    CB_CHECK(wide.column_width > kBoardColumnMin);
    CB_CHECK(board_max_hscroll(wide) == 0);
});

CB_TEST("app.window_board.a_long_column_fans_a_few_cards_and_keeps_the_rest_in_a_deck", [] {
    const Model m = wb_model(5000);
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    const BoardColumn c = compute_board_column(b, m, 0);
    CB_CHECK(c.deck.h > 0);
    CB_CHECK(c.last - c.first < 30);
    CB_CHECK_EQ(c.hidden + (c.last - c.first), 5000);
    CB_CHECK(board_card_rect(c, c.last - 1).bottom() <= c.area.bottom() + 0.5f);
});

CB_TEST("app.window_board.an_expanded_column_lays_out_only_what_is_in_view", [] {
    Model m = wb_model(20000);
    m.board_expanded[2] = true;
    m.board_scroll[2] = kBoardRowPitch * 15000;
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    const BoardColumn c = compute_board_column(b, m, 2);
    CB_CHECK_EQ(c.first, 15000);
    CB_CHECK(c.last - c.first <= static_cast<int>(c.area.h / kBoardRowPitch) + 2);
    const Rect r = board_card_rect(c, c.first);
    CB_CHECK(r.bottom() > c.area.y && r.y <= c.area.y + 0.5f);
    m.board_scroll[2] = 1e9f;   // beyond the end: the last card is in view
    const BoardColumn end = compute_board_column(compute_board_layout(wb_shell(m), m), m, 2);
    CB_CHECK_EQ(end.last, 20000);
});

CB_TEST("app.window_board.a_point_finds_the_card_the_foundation_and_the_head", [] {
    Model m = wb_model(4);
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    const BoardColumn c = compute_board_column(b, m, 1);
    CB_CHECK(hit_test_board(b, m, c.foundation.x + 4, c.foundation.y + 4) == (PageHit{Control::BoardFoundation, 1}));
    CB_CHECK(hit_test_board(b, m, c.head.x + 4, c.head.y + 4) == (PageHit{Control::BoardExpand, 1}));
    const Rect first = board_card_rect(c, 0);
    const PageHit top = hit_test_board(b, m, first.x + 4, first.y + 4);
    CB_CHECK(top.control == Control::BoardCard);
    CB_CHECK_EQ(top.index, static_cast<int>((*c.cards)[0]));
    const Rect last = board_card_rect(c, c.last - 1);
    const PageHit bottom = hit_test_board(b, m, last.x + 4, last.bottom() - 3);   // the last of a fan is whole
    CB_CHECK_EQ(bottom.index, static_cast<int>((*c.cards)[static_cast<std::size_t>(c.last - 1)]));
    CB_CHECK_EQ(board_foundation_at(b, m, c.foundation.x + 4, c.foundation.y + 4), 1);
    CB_CHECK_EQ(board_foundation_at(b, m, first.x + 4, first.y + 4), -1);
});

CB_TEST("app.window_board.a_column_scrolled_out_of_the_tableau_cannot_be_hit", [] {
    Model m = wb_model(2);
    m.board_hscroll = 400;
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    const BoardColumn c = compute_board_column(b, m, 0);
    CB_CHECK(c.foundation.right() < b.tableau.x);
    CB_CHECK(hit_test_board(b, m, c.foundation.x + 4, c.foundation.y + 4).control != Control::BoardFoundation);
});

CB_TEST("app.window_board.a_selected_card_opens_the_detail_strip_with_its_two_buttons", [] {
    Model m = wb_model(2);
    CB_CHECK(compute_board_layout(wb_shell(m), m).detail.w == 0);
    m.board_selected = 3;
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(b.detail.w > 0 && b.tableau.bottom() <= b.detail.y);
    CB_CHECK(hit_test_board(b, m, b.convert.x + 2, b.convert.y + 2) == (PageHit{Control::BoardCardConvert, 3}));
    CB_CHECK(hit_test_board(b, m, b.open_card.x + 2, b.open_card.y + 2) == (PageHit{Control::BoardCardOpen, 3}));
});

CB_TEST("app.window_board.with_no_board_the_page_is_a_drop_area", [] {
    Model m;
    m.page = Page::Board;
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(b.empty.h > 0);
    CB_CHECK(hit_test_board(b, m, b.open.x + 2, b.open.y + 2).control == Control::BoardOpen);
});

Model wb_linked() {
    Model m = wb_model(3);
    auto index = std::make_shared<board::Index>();
    index->nodes.resize(m.board->cards.size());
    for (std::uint32_t i = 0; i < index->nodes.size(); ++i) index->nodes[i].group = i;
    index->edges = {{0, 3, board::Link::Imports, 100}, {1, 3, board::Link::Imports, 100}, {3, 6, board::Link::Imports, 100}, {9, 0, board::Link::Installs, 100}};
    index->finish();
    m.board_index = index;
    return m;
}

CB_TEST("app.window_board.flow_opens_on_the_most_linked_file_and_map_follows_the_selected_card", [] {
    Model m = wb_linked();
    board_show_view(m, 1);
    CB_CHECK(m.board_flow != nullptr);
    CB_CHECK_EQ(m.board_selected, 3);
    CB_CHECK_EQ(m.board_flow->in_total, std::uint64_t{2});
    m.board_selected = 6;
    board_show_view(m, 2);
    CB_CHECK_EQ(m.board_trace->focus, m.board_graph->module_of_card[6]);
});

CB_TEST("app.window_board.a_kind_of_link_changes_the_map_and_keeps_the_focus", [] {
    Model m = wb_linked();
    board_show_view(m, 1);
    const int focus = m.board_focus;
    m.board_links = board::kLinkInstalls;
    board_refresh_maps(m);
    CB_CHECK_EQ(m.board_focus, focus);
    CB_CHECK_EQ(m.board_flow->in_total, std::uint64_t{0});
});

CB_TEST("app.window_board.flow_bands_fit_and_their_ends_fill_the_center", [] {
    board::Flow f;
    f.in = {{board::Pile::Programs, 900, 3, 0}, {board::Pile::Libraries, 90, 2, 0}, {board::Pile::Drivers, 10, 1, 0}};
    f.in_total = 1000;
    const Rect area{100, 100, 1000, 500};
    const FlowLayout l = layout_flow(area, f);
    CB_CHECK(l.in[0].bar.y >= area.y && l.in[2].bar.bottom() <= area.bottom() + 0.5f);
    CB_CHECK(l.in[0].bar.h > l.in[1].bar.h && l.in[2].bar.h >= 3);
    CB_CHECK(std::abs(l.in[2].slice.bottom() - l.center.bottom()) < 0.5f);
    CB_CHECK(l.in[2].label.bottom() <= area.bottom());   // the last band's words are not cut off
});

CB_TEST("app.window_board.both_sides_of_a_flow_share_one_scale", [] {
    board::Flow f;
    f.in = {{board::Pile::Libraries, 800, 3, 0}};
    f.in_total = 800;
    f.out = {{board::Pile::Libraries, 2, 2, 0}};
    f.out_total = 2;
    const FlowLayout l = layout_flow(Rect{100, 100, 1000, 500}, f);
    CB_CHECK(l.out[0].bar.h < l.in[0].bar.h / 20);
    CB_CHECK(l.out[0].slice.h < l.in[0].slice.h / 20);
    CB_CHECK(l.out[0].slice.y > l.center.y && l.out[0].slice.bottom() < l.center.bottom());
});

CB_TEST("app.window_board.a_trace_shows_what_fits_and_counts_the_rest", [] {
    board::Trace t;
    t.columns[board::kTraceCenter] = {0};
    for (std::uint32_t i = 1; i <= 64; ++i) t.columns[2].push_back(i);
    t.hidden[2] = 100;
    const TraceLayout l = layout_trace(Rect{0, 0, 1400, 300}, t);
    CB_CHECK(l.nodes[2].size() < 64);
    CB_CHECK_EQ(l.more_count[2], 100u + 64u - static_cast<std::uint32_t>(l.nodes[2].size()));
    CB_CHECK(l.more[2].bottom() <= 300.5f);
});

Model wb_library() {
    Model m = wb_linked();
    auto index = std::make_shared<board::Index>(*m.board_index);
    index->exports = {{3, 1, "Create"}, {3, 2, "Destroy"}};
    index->finish();
    index->export_uses = {{0, 0, 12}, {1, 0, 4}};
    index->finish();
    m.board_index = index;
    m.board_selected = 3;
    board_show_view(m, 3);
    return m;
}

CB_TEST("app.window_board.a_library_view_has_channels_each_side_and_shows_users_on_hover", [] {
    Model m = wb_library();
    CB_CHECK(m.board_library != nullptr);
    CB_CHECK_EQ(m.board_library->right.size(), std::size_t{2});
    BoardLayout b = compute_board_layout(wb_shell(m), m);
    LibraryLayout l = layout_library(b.canvas, *m.board_library, 0, 0, -1);
    CB_CHECK(l.left_area.right() <= l.center.x && l.center.right() <= l.right_area.x);
    CB_CHECK(l.users_area.x - l.right_area.right() >= 100);   // room for the lines from an export to its files
    CB_CHECK(l.user_rows.empty());
    const Rect row = l.right_rows[0];
    CB_CHECK(hit_test_board(b, m, row.x + 2, row.y + 2) == (PageHit{Control::BoardRight, 0}));
    m.board_hover_export = 0;
    l = layout_library(b.canvas, *m.board_library, 0, 0, 0);
    CB_CHECK_EQ(l.user_rows.size(), std::size_t{2});
    const PageHit user = hit_test_board(b, m, l.user_rows[0].x + 2, l.user_rows[0].y + 2);
    CB_CHECK(user.control == Control::BoardUser);
    CB_CHECK_EQ(user.index, static_cast<int>(m.board_library->right[0].users[0].module));
});

CB_TEST("app.window_board.a_library_with_thousands_of_exports_lays_out_only_what_is_in_view", [] {
    board::LibraryView v;
    for (int i = 0; i < 5000; ++i) {
        board::LibraryChannel c;
        c.label = "f" + std::to_string(i);
        c.weight = 5000u - static_cast<unsigned>(i);
        v.right_total += c.weight;
        v.right.push_back(std::move(c));
    }
    const LibraryLayout l = layout_library(Rect{0, 0, 1400, 600}, v, 0, kLibraryRowPitch * 3000, -1);
    CB_CHECK_EQ(l.right_first, 3000);
    CB_CHECK(l.right_rows.size() <= 30);
    CB_CHECK(l.right_slices[0].y > l.center.y);
    CB_CHECK(library_thickness(5000, 5000) > library_thickness(1, 5000));
    CB_CHECK(library_thickness(1, 5000) >= 1);
});

CB_TEST("app.window_board.breadcrumbs_step_back_through_the_way_here_to_the_board", [] {
    Model m = wb_library();
    const int start = m.board_focus;
    board_go_to(m, static_cast<int>(m.board_graph->module_of_card[6]));
    board_go_to(m, start, "create");
    CB_CHECK_EQ(m.board_crumbs.size(), std::size_t{3});
    CB_CHECK_EQ(m.board_function, std::string("create"));
    CB_CHECK_EQ(m.board_library->left.size(), std::size_t{2});   // centred on the function: the files that use it
    const std::vector<std::string> words = board_crumb_words(m);
    CB_CHECK_EQ(words.size(), std::size_t{4});
    CB_CHECK_EQ(words[0], std::string("Board"));
    CB_CHECK(words[3].ends_with("! Create"));                    // as the library spells it
    board_go_to(m, start, "create");
    CB_CHECK_EQ(m.board_crumbs.size(), std::size_t{3});          // the same place twice is one step
    board_step_back(m);
    CB_CHECK_EQ(m.board_focus, static_cast<int>(m.board_graph->module_of_card[6]));
    CB_CHECK(m.board_function.empty());
    board_crumb_back(m, 0);
    CB_CHECK_EQ(m.board_focus, start);
    CB_CHECK_EQ(m.board_crumbs.size(), std::size_t{1});
    board_step_back(m);
    CB_CHECK_EQ(m.board_view, 0);
    CB_CHECK(m.board_crumbs.empty());
});

CB_TEST("app.window_board.a_long_way_keeps_the_first_and_last_crumbs", [] {
    std::vector<std::string> words{"Board"};
    for (int i = 0; i < 40; ++i) words.push_back("some_long_file_name_" + std::to_string(i) + ".dll");
    const Rect area{10, 10, 700, 24};
    const CrumbLayout c = layout_crumbs(area, words);
    CB_CHECK(c.crumbs.front().w > 0 && c.crumbs.back().w > 0 && c.more.w > 0);
    CB_CHECK(c.crumbs[1].w == 0);
    CB_CHECK(c.crumbs.back().right() <= area.right() + 0.5f);
});

Model wb_signals() {
    Model m = wb_linked();
    auto index = std::make_shared<board::Index>(*m.board_index);
    index->nodes[3].group = 0;   // cards 0 and 3 hold the same bytes
    index->nodes[0].copies = index->nodes[3].copies = 2;
    m.board_index = index;
    auto signals = std::make_shared<board::Signals>();
    signals->signals = {{board::SignalType::Mutex, "shared"}, {board::SignalType::Mutex, "single"}, {board::SignalType::IoPort, "0x3f8"}};
    signals->links = {{0, 0, board::Direction::Writes, 70}, {3, 0, board::Direction::Reads, 70}, {6, 0, board::Direction::Reads, 70}, {6, 1, board::Direction::Writes, 70}, {9, 2, board::Direction::Reads, 50}};
    signals->finish(m.board->cards.size());
    m.board_signals = signals;
    board_rebuild_stacks(m);
    return m;
}

CB_TEST("app.window_board.identical_files_are_one_master_card_until_grouping_is_off", [] {
    Model m = wb_signals();
    std::size_t cards = 0;
    for (const board::Stack& s : m.board_stacks) cards += s.total();
    CB_CHECK_EQ(cards, m.board->cards.size() - 1);
    m.board_group_copies = false;
    board_rebuild_stacks(m);
    cards = 0;
    for (const board::Stack& s : m.board_stacks) cards += s.total();
    CB_CHECK_EQ(cards, m.board->cards.size());
});

CB_TEST("app.window_board.a_signal_list_is_busiest_first_and_filters_add_up", [] {
    Model m = wb_signals();
    board_open_signals(m, static_cast<int>(board::SignalType::Mutex));
    CB_CHECK_EQ(m.board_signal_rows.size(), std::size_t{2});
    CB_CHECK_EQ(m.board_signal_rows[0].first, 0u);
    CB_CHECK_EQ(m.board_signal_rows[0].second, 2u);   // cards 0 and 3 are one file
    board_toggle_filter(m, 0);
    std::size_t cards = 0;
    for (const board::Stack& s : m.board_stacks) cards += s.total();
    CB_CHECK_EQ(cards, std::size_t{2});
    board_toggle_filter(m, 1);
    cards = 0;
    for (const board::Stack& s : m.board_stacks) cards += s.total();
    CB_CHECK_EQ(cards, std::size_t{1});               // wired to both
    board_toggle_filter(m, 0);
    board_toggle_filter(m, 1);
    CB_CHECK(m.board_filters.empty());
    board_open_signals(m, static_cast<int>(board::SignalType::Mutex));
    CB_CHECK_EQ(m.board_signal_type, -1);             // the same kind again closes its list
});

CB_TEST("app.window_board.the_signal_row_and_its_list_are_hit", [] {
    Model m = wb_signals();
    BoardLayout b = compute_board_layout(wb_shell(m), m);
    const Rect& pill = b.signal_pills[static_cast<std::size_t>(board::SignalType::Mutex)];
    CB_CHECK(hit_test_board(b, m, pill.x + 2, pill.y + 2) == (PageHit{Control::BoardSignalType, static_cast<int>(board::SignalType::Mutex)}));
    CB_CHECK(b.signal_pills.back().right() <= b.group_copies.x);
    board_open_signals(m, static_cast<int>(board::SignalType::Mutex));
    SignalPanel p = layout_signal_panel(b, m);
    CB_CHECK_EQ(p.rows.size(), std::size_t{2});
    CB_CHECK(hit_test_board(b, m, p.rows[1].x + 2, p.rows[1].y + 2) == (PageHit{Control::BoardSignalRow, 1}));
    board_open_signals(m, -1);
    board_toggle_filter(m, 0);
    b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(b.filter_row.h > 0 && b.tableau.y >= b.filter_row.bottom());
    CB_CHECK(hit_test_board(b, m, b.filter_row.x + 3, b.filter_row.y + 3) == (PageHit{Control::BoardFilterChip, 0}));
});

CB_TEST("app.window_board.the_maps_project_their_flow_and_name_the_hot_path", [] {
    Model m = wb_library();
    CB_CHECK(m.board_projection != nullptr);
    CB_CHECK(m.board_projection->max_channel > 0);
    const std::string words = board_hot_path_words(m);
    CB_CHECK(words.find("/h") != std::string::npos);
    const BoardLayout wide = compute_board_layout(wb_shell(m, 1600, 900), m);
    CB_CHECK(wide.hot_path.w > 0 && wide.crumbs.right() <= wide.hot_path.x);
});

CB_TEST("app.window_board.the_inspector_opens_on_a_card_scrolls_and_follows_the_selection", [] {
    Model m = wb_signals();
    board_inspect_card(m, 3);
    CB_CHECK(m.board_inspector != nullptr);
    CB_CHECK_EQ(m.board_selected, 3);
    BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(b.drawer.w > 0 && b.drawer.right() <= b.card.right());
    CB_CHECK(hit_test_board(b, m, b.drawer_close.x + 2, b.drawer_close.y + 2).control == Control::BoardInspectClose);
    CB_CHECK(hit_test_board(b, m, b.inspect.x + 2, b.inspect.y + 2) == (PageHit{Control::BoardInspect, 3}));
    const InspectorLayout l = layout_inspector(b, m);
    CB_CHECK(!l.rows.empty() && l.rows.back().y < l.list.bottom());
    m.board_selected = 6;
    board_sync_inspector(m);
    CB_CHECK_EQ(m.board_inspector->card, 6u);
    board_inspect_card(m, -1);
    CB_CHECK(!m.board_inspecting && m.board_inspector == nullptr);
    CB_CHECK(compute_board_layout(wb_shell(m), m).drawer.w == 0);
});

CB_TEST("app.window_board.a_hover_counts_what_it_lights_in_each_column", [] {
    Model m = wb_signals();
    board_hover_card(m, 6);   // card 6 is imported by card 3, whose bytes card 0 shares
    CB_CHECK(m.board_hover != nullptr);
    CB_CHECK_EQ(m.board_hover->used_by, 1u);
    std::uint32_t lit = 0;
    for (const std::uint32_t n : m.board_hover_linked) lit += n;
    CB_CHECK_EQ(lit, 1u);     // copies are one master card
    board_hover_card(m, -1);
    CB_CHECK(m.board_hover == nullptr);
    lit = 0;
    for (const std::uint32_t n : m.board_hover_linked) lit += n;
    CB_CHECK_EQ(lit, 0u);
});

CB_TEST("app.window_board.orphans_decks_and_the_batch_follow_what_is_shown", [] {
    Model m = wb_signals();
    m.board->root = std::filesystem::temp_directory_path() / "cb_window_board_test" / "decks";
    fs::remove_tree(m.board->root);
    fs::make_dirs(m.board->root);
    for (board::Card& c : m.board->cards) c.format = "PE";
    board_rebuild_stacks(m);
    const std::size_t all = m.board_batch.size();
    CB_CHECK(all > 0);
    board_toggle_orphans(m);
    CB_CHECK(m.board_orphans_only && m.board_batch.size() < all);
    board_toggle_orphans(m);
    CB_CHECK_EQ(m.board_batch.size(), all);
    board_load_decks(m);
    const std::size_t builtin = m.board_decks.size();
    CB_CHECK_EQ(builtin, std::size_t{3});
    m.board_query = "f1_";
    board_rebuild_stacks(m);
    board_save_deck(m);
    CB_CHECK_EQ(m.board_decks.size(), builtin + 1);
    CB_CHECK(m.board_decks.back().name.find("f1_") != std::string::npos);
    const int mine = static_cast<int>(builtin);
    board_apply_deck(m, mine);   // it is the active one: again means everything
    CB_CHECK(m.board_query.empty());
    board_apply_deck(m, mine);
    CB_CHECK_EQ(m.board_query, std::string("f1_"));
    board_load_decks(m);
    CB_CHECK_EQ(m.board_decks.size(), builtin + 1);   // kept with the board
    board_remove_deck(m, mine);
    board_remove_deck(m, 0);                          // a built-in deck stays
    CB_CHECK_EQ(m.board_decks.size(), builtin);
});

CB_TEST("app.window_board.with_no_signal_kinds_group_copies_shares_the_automation_row", [] {
    Model m = wb_signals();
    const BoardLayout with = compute_board_layout(wb_shell(m), m);
    m.board_signals = nullptr;   // nothing found yet: no row of signal kinds
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(std::abs(b.group_copies.y - b.orphans_button.y) < 0.5f && b.group_copies.right() < b.orphans_button.x);
    CB_CHECK(b.filter_row.right() <= b.group_copies.x);
    CB_CHECK(b.tableau.y < with.tableau.y);   // the row it had alone goes to the cards
    CB_CHECK(hit_test_board(b, m, b.group_copies.x + 2, b.group_copies.y + 2).control == Control::BoardGroupCopies);
});

CB_TEST("app.window_board.the_automation_buttons_are_hit_and_leave_the_filters_room", [] {
    Model m = wb_signals();
    board_load_decks(m);
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(hit_test_board(b, m, b.orphans_button.x + 2, b.orphans_button.y + 2).control == Control::BoardOrphans);
    CB_CHECK(hit_test_board(b, m, b.decks_button.x + 2, b.decks_button.y + 2).control == Control::BoardDecks);
    CB_CHECK(hit_test_board(b, m, b.export_button.x + 2, b.export_button.y + 2).control == Control::BoardExport);
    CB_CHECK(hit_test_board(b, m, b.batch_button.x + 2, b.batch_button.y + 2).control == Control::BoardBatch);
    CB_CHECK(b.filter_row.right() <= b.orphans_button.x && b.tableau.y >= b.batch_button.bottom());
    m.board_decks_open = true;
    const DeckPanel p = layout_deck_panel(b, m);
    CB_CHECK_EQ(p.rows.size(), std::size_t{3});
    CB_CHECK(hit_test_board(b, m, p.save.x + 2, p.save.y + 2) == (PageHit{Control::BoardDeckRow, -1}));
    CB_CHECK(hit_test_board(b, m, p.rows[1].x + 2, p.rows[1].y + 2) == (PageHit{Control::BoardDeckRow, 1}));
});

CB_TEST("app.window_board.the_map_views_hit_their_tabs_chips_and_nodes", [] {
    Model m = wb_linked();
    board_show_view(m, 2);
    const BoardLayout b = compute_board_layout(wb_shell(m), m);
    CB_CHECK(hit_test_board(b, m, b.tabs[1].x + 3, b.tabs[1].y + 3) == (PageHit{Control::BoardViewTab, 1}));
    CB_CHECK(hit_test_board(b, m, b.chips[2].x + 3, b.chips[2].y + 3) == (PageHit{Control::BoardLinkChip, 2}));
    const TraceLayout t = layout_trace(b.canvas, *m.board_trace);
    const Rect& center = t.nodes[board::kTraceCenter][0];
    CB_CHECK(hit_test_board(b, m, center.x + 3, center.y + 3) == (PageHit{Control::BoardNode, m.board_focus}));
});

}  // namespace
}  // namespace cb::app::window
