// app/window_board.cpp -- see window_board.hpp.
#include "app/window_board.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <limits>
#include <string_view>
#include <unordered_map>

#include "app/window_convert.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::app::window {

namespace {
constexpr float kPad = 16;
constexpr float kHeaderHeight = 40;
constexpr float kButtonH = 32;
}  // namespace

bool board_column_shown(const Model& m, std::size_t column) {
    if (m.board_c_view == board::CView::All) return true;
    if (m.board_stacks[column].total() > 0) return true;
    return std::none_of(m.board_stacks.begin(), m.board_stacks.end(), [](const board::Stack& s) { return s.total() > 0; });
}

std::size_t board_columns_shown(const Model& m) {
    std::size_t shown = 0;
    for (std::size_t i = 0; i < board::kPileCount; ++i) shown += board_column_shown(m, i) ? 1 : 0;
    return shown;
}

BoardLayout compute_board_layout(const Layout& shell, const Model& model) {
    BoardLayout b;
    b.card = shell.left_card;
    const float right = b.card.right() - kPad;
    b.open = {right - 112, b.card.y + 12, 112, kButtonH};
    b.search = {b.open.x - 8 - 200, b.card.y + 12, 200, kButtonH};
    b.title = {b.card.x + kPad, b.card.y + 8, 120, 24};
    b.summary = {b.card.x + kPad, b.title.bottom(), std::max(0.0f, b.search.x - 8 - b.card.x - kPad), 16};
    if (model.board) {
        constexpr float kTabW = 62;
        for (std::size_t i = 0; i < b.tabs.size(); ++i) b.tabs[i] = {b.search.x - 8 - kTabW * static_cast<float>(b.tabs.size() - i), b.card.y + 12, kTabW, kButtonH};
        b.summary.w = std::max(0.0f, b.tabs[0].x - 8 - b.summary.x);
    }
    float top = b.card.y + 8 + kHeaderHeight + 8;
    if (model.board_busy) {
        b.cancel = {right - 88, top, 88, kButtonH};
        b.bar_text = {b.card.x + kPad, top, std::max(0.0f, b.cancel.x - 8 - b.card.x - kPad), 18};
        b.bar = {b.card.x + kPad, top + 22, b.bar_text.w, 4};
        top += kButtonH + 8;
    }
    if (model.board && model.board_view == 0) {   // the row of signal kinds, and the filters in force
        constexpr float kPillH = 26, kGroupW = 106, kCSummaryH = 40, kCPillW = 124;
        if (model.board_scores) {
            b.c_summary = {b.card.x + kPad, top, b.card.w - 2 * kPad, kCSummaryH};
            b.c_leaders = {b.c_summary.right() - 6 - 128, b.c_summary.y + 3, 128, 18};
            top += kCSummaryH + 6;
        }
        b.group_copies = {right - kGroupW, top, kGroupW, kPillH};
        if (model.board_signals) {
            const float room = b.group_copies.x - 8 - (b.card.x + kPad);
            const float width = std::min(92.0f, (room + 4) / static_cast<float>(board::kSignalTypes));
            for (std::size_t i = 0; i < b.signal_pills.size(); ++i)
                b.signal_pills[i] = {b.card.x + kPad + static_cast<float>(i) * width, top, width - 4, kPillH};
        }
        top += kPillH + 6;
        b.batch_button = {right - 126, top, 126, kPillH};
        b.export_button = {b.batch_button.x - 6 - 66, top, 66, kPillH};
        b.decks_button = {b.export_button.x - 6 - 66, top, 66, kPillH};
        b.orphans_button = {b.decks_button.x - 6 - 92, top, 92, kPillH};
        float filters_x = b.card.x + kPad;
        if (model.board_scores) {
            const float width = std::min(kCPillW, std::max(0.0f, b.orphans_button.x - 8 - filters_x + 4) / static_cast<float>(board::kCViews));
            for (std::size_t i = 0; i < b.c_pills.size(); ++i) b.c_pills[i] = {filters_x + static_cast<float>(i) * width, top, width - 4, kPillH};
            filters_x += static_cast<float>(b.c_pills.size()) * width + 4;
        }
        b.filter_row = {filters_x, top, std::max(0.0f, b.orphans_button.x - 8 - filters_x), kPillH};
        top += kPillH + 6;
    }
    float bottom = b.card.bottom() - 12;
    if (model.board && model.board_selected >= 0 && static_cast<std::size_t>(model.board_selected) < model.board->cards.size()) {
        b.detail = {b.card.x + kPad, bottom - kBoardDetailHeight, b.card.w - 2 * kPad, kBoardDetailHeight};
        b.convert = {b.detail.right() - 10 - 96, b.detail.y + 10, 96, kButtonH};
        b.open_card = {b.convert.x - 8 - 84, b.detail.y + 10, 84, kButtonH};
        b.map = {b.open_card.x - 8 - 72, b.detail.y + 10, 72, kButtonH};
        b.inspect = {b.map.x - 8 - 84, b.detail.y + 10, 84, kButtonH};
        b.detail_text = {b.detail.x + 12, b.detail.y + 6, std::max(0.0f, b.inspect.x - 12 - b.detail.x - 12), kBoardDetailHeight - 12};
        bottom = b.detail.y - 8;
    }
    b.tableau = {b.card.x + kPad, top, b.card.w - 2 * kPad, std::max(0.0f, bottom - top)};
    if (!model.board) {
        b.empty = b.tableau;
        return b;
    }
    if (model.board_inspecting && model.board_inspector) {
        const float width = std::min(420.0f, b.card.w * 0.42f);
        b.drawer = {b.card.right() - kPad - width, b.tableau.y, width, b.tableau.h};
        b.drawer_close = {b.drawer.right() - 32, b.drawer.y + 4, 28, 26};
    }
    const float n = static_cast<float>(board_columns_shown(model));
    b.column_width = std::max(kBoardColumnMin, (b.tableau.w - (n - 1) * kBoardColumnGap) / n);
    if (model.board_c_view != board::CView::All) b.column_width = std::min(b.column_width, std::max(kBoardColumnMin, kBoardColumnWide));
    b.content_width = n * b.column_width + (n - 1) * kBoardColumnGap;
    if (model.board_view != 0) {
        constexpr float kChipW = 88, kChipH = 26;
        for (std::size_t i = 0; i < b.chips.size(); ++i) b.chips[i] = {b.tableau.x + static_cast<float>(i) * (kChipW + 6), b.tableau.y, kChipW, kChipH};
        constexpr float kWidthW = 58;
        const float start = b.chips.back().right() + 14;
        for (std::size_t i = 0; i < b.widths.size(); ++i) b.widths[i] = {start + static_cast<float>(i) * kWidthW, b.tableau.y, kWidthW, kChipH};
        const float after = b.widths.back().right() + 12;
        b.export_button = {b.tableau.right() - 66, b.tableau.y, 66, kChipH};
        b.note = {after, b.tableau.y, std::max(0.0f, b.export_button.x - 8 - after), kChipH};
        constexpr float kCrumbH = 24;
        b.crumbs = {b.tableau.x, b.tableau.y + kChipH + 4, b.tableau.w, kCrumbH};
        if (b.tableau.w >= 900) {
            constexpr float kHotW = 470;
            b.hot_path = {b.tableau.right() - kHotW, b.crumbs.y, kHotW, kCrumbH};
            b.crumbs.w -= kHotW + 8;
        }
        b.canvas = {b.tableau.x, b.crumbs.bottom() + 4, b.tableau.w, std::max(0.0f, b.tableau.h - kChipH - kCrumbH - 8)};
    }
    return b;
}

float board_max_hscroll(const BoardLayout& layout) { return std::max(0.0f, layout.content_width - layout.tableau.w); }

BoardColumn compute_board_column(const BoardLayout& layout, const Model& model, std::size_t column) {
    BoardColumn c;
    if (!board_column_shown(model, column)) {
        c.cards = &model.board_stacks[column].todo;
        return c;
    }
    std::size_t slot = 0;
    for (std::size_t i = 0; i < column; ++i) slot += board_column_shown(model, i) ? 1 : 0;
    const float x = layout.tableau.x + static_cast<float>(slot) * (layout.column_width + kBoardColumnGap) - model.board_hscroll;
    c.foundation = {x, layout.tableau.y, layout.column_width, kBoardFoundationHeight};
    c.head = {x, c.foundation.bottom() + 4, layout.column_width, kBoardHeadHeight};
    const float area_top = c.head.bottom() + 4;
    c.area = {x, area_top, layout.column_width, std::max(0.0f, layout.tableau.bottom() - area_top)};
    c.expanded = model.board_expanded[column];
    c.done_view = model.board_show_done[column];
    const board::Stack& stack = model.board_stacks[column];
    c.cards = c.done_view ? &stack.done : &stack.todo;
    c.count = static_cast<int>(c.cards->size());
    if (c.count == 0 || c.area.h <= 0) {
        c.top = c.area.y;
        return c;
    }
    if (c.expanded) {
        c.pitch = kBoardRowPitch;
        c.card_height = kBoardRowHeight;
        c.content_height = static_cast<float>(c.count) * c.pitch;
        const float scroll = std::clamp(model.board_scroll[column], 0.0f, std::max(0.0f, c.content_height - c.area.h));
        c.top = c.area.y - scroll;
        c.first = std::clamp(static_cast<int>(std::floor(scroll / c.pitch)), 0, c.count);
        c.last = std::clamp(static_cast<int>(std::ceil((scroll + c.area.h) / c.pitch)), c.first, c.count);
        return c;
    }
    const auto fit_in = [&](float room) { return std::max(1, static_cast<int>(std::floor((room - kBoardCardHeight) / kBoardFanPitch)) + 1); };
    int shown = fit_in(c.area.h);
    c.top = c.area.y;
    if (c.count > shown) {   // the rest wait in a deck above the fan
        c.deck = {x, c.area.y, layout.column_width, kBoardDeckHeight};
        c.top = c.deck.bottom() + 2;
        shown = fit_in(c.area.h - kBoardDeckHeight - 2);
    }
    shown = std::min(shown, c.count);
    c.first = 0;
    c.last = shown;
    c.hidden = c.count - shown;
    return c;
}

Rect board_card_rect(const BoardColumn& c, int position) {
    return {c.area.x, c.top + static_cast<float>(position) * c.pitch, c.area.w, c.card_height};
}

float board_max_scroll(const BoardColumn& c) { return std::max(0.0f, c.content_height - c.area.h); }

PageHit hit_test_board(const BoardLayout& b, const Model& m, float x, float y) {
    if (m.board_view == 0 && m.board_compare) {   // the compare panel is over the columns and the drawer
        const BoardCompareLayout panel = layout_board_compare(b);
        if (panel.close.contains(x, y)) return {Control::BoardCompareClose, -1};
        if (m.board_compare_list.size() > 1) {
            if (panel.prev.contains(x, y)) return {Control::BoardComparePrev, -1};
            if (panel.next.contains(x, y)) return {Control::BoardCompareNext, -1};
        }
        if (panel.outline.contains(x, y)) return {};
    }
    if (m.board_view == 0 && m.board_leaders_open) {   // the list of closest functions is over the columns and the drawer
        const BoardLeadersLayout list = layout_board_leaders(b, m);
        if (list.close.contains(x, y)) return {Control::BoardLeadersClose, -1};
        if (list.search.contains(x, y)) return {Control::BoardLeadersSearch, -1};
        if (list.export_button.contains(x, y)) return {Control::BoardLeadersExport, -1};
        for (std::size_t i = 0; i < list.rows.size(); ++i)
            if (list.rows[i].contains(x, y)) return {Control::BoardLeaderRow, list.first + static_cast<int>(i)};
        if (list.outline.contains(x, y)) return {};
    }
    if (m.board_view == 0 && m.board_decks_open) {   // the open list of decks is over everything under it
        const DeckPanel panel = layout_deck_panel(b, m);
        if (panel.outline.contains(x, y)) {
            if (panel.save.contains(x, y)) return {Control::BoardDeckRow, -1};
            for (std::size_t i = 0; i < panel.rows.size(); ++i) {
                if (panel.removes[i].contains(x, y)) return {Control::BoardDeckRemove, static_cast<int>(i)};
                if (panel.rows[i].contains(x, y)) return {Control::BoardDeckRow, static_cast<int>(i)};
            }
            return {};
        }
    }
    if (m.board && b.export_button.contains(x, y)) return {Control::BoardExport, -1};
    if (m.board_view == 0 && m.board) {
        if (b.orphans_button.contains(x, y)) return {Control::BoardOrphans, -1};
        if (b.decks_button.contains(x, y)) return {Control::BoardDecks, -1};
        if (b.batch_button.contains(x, y)) return {Control::BoardBatch, -1};
    }
    if (m.board_view == 0 && m.board_signal_type >= 0) {   // the open list is over everything under it
        const SignalPanel panel = layout_signal_panel(b, m);
        if (panel.outline.contains(x, y)) {
            for (std::size_t i = 0; i < panel.rows.size(); ++i)
                if (panel.rows[i].contains(x, y)) return {Control::BoardSignalRow, static_cast<int>(m.board_signal_rows[static_cast<std::size_t>(panel.first) + i].first)};
            return {};
        }
    }
    if (b.drawer.w > 0 && b.drawer.contains(x, y)) {   // the drawer is over the board
        if (b.drawer_close.contains(x, y)) return {Control::BoardInspectClose, -1};
        const InspectorLayout inspector = layout_inspector(b, m);
        for (std::size_t i = 0; i < inspector.rows.size(); ++i) {
            if (!inspector.rows[i].contains(x, y)) continue;
            const auto at = static_cast<std::size_t>(inspector.first) + i;
            const board::InspectorRow& row = m.board_inspector->rows[at];
            if (row.card >= 0 || row.signal >= 0 || row.kind == board::InspectorRow::Kind::Function) return {Control::BoardInspectRow, static_cast<int>(at)};
        }
        return {};
    }
    if (m.board_view == 0) {
        for (std::size_t i = 0; i < b.signal_pills.size(); ++i)
            if (b.signal_pills[i].contains(x, y)) return {Control::BoardSignalType, static_cast<int>(i)};
        if (b.group_copies.contains(x, y)) return {Control::BoardGroupCopies, -1};
        if (b.c_leaders.contains(x, y)) return {Control::BoardLeaders, -1};
        for (std::size_t i = 0; i < b.c_pills.size(); ++i)
            if (b.c_pills[i].contains(x, y)) return {Control::BoardCView, static_cast<int>(i) + 1};
        if (b.filter_row.contains(x, y)) {
            const auto at = static_cast<std::size_t>((x - b.filter_row.x) / (kFilterChipWidth + 6));
            if (at < m.board_filters.size() && x - b.filter_row.x - static_cast<float>(at) * (kFilterChipWidth + 6) < kFilterChipWidth)
                return {Control::BoardFilterChip, static_cast<int>(m.board_filters[at])};
            return {};
        }
    }
    if (b.open.contains(x, y)) return {Control::BoardOpen, -1};
    if (m.board_busy && b.cancel.contains(x, y)) return {Control::BoardCancel, -1};
    if (b.search.contains(x, y)) return {Control::BoardSearch, -1};
    for (std::size_t i = 0; i < b.tabs.size(); ++i)
        if (b.tabs[i].contains(x, y)) return {Control::BoardViewTab, static_cast<int>(i)};
    if (b.detail.w > 0) {
        if (b.map.contains(x, y)) return {Control::BoardMap, m.board_selected};
        if (b.inspect.contains(x, y)) return {Control::BoardInspect, m.board_selected};
        if (b.convert.contains(x, y)) return {Control::BoardCardConvert, m.board_selected};
        if (b.open_card.contains(x, y)) return {Control::BoardCardOpen, m.board_selected};
    }
    if (m.board && m.board_view != 0) {
        for (std::size_t i = 0; i < b.chips.size(); ++i)
            if (b.chips[i].contains(x, y)) return {Control::BoardLinkChip, static_cast<int>(i)};
        for (std::size_t i = 0; i < b.widths.size(); ++i)
            if (b.widths[i].contains(x, y)) return {Control::BoardWidth, static_cast<int>(i)};
        if (b.crumbs.contains(x, y)) {
            const std::vector<std::string> words = board_crumb_words(m);
            const CrumbLayout crumbs = layout_crumbs(b.crumbs, words);
            for (std::size_t i = 0; i + 1 < crumbs.crumbs.size(); ++i)   // (the last is where you are)
                if (crumbs.crumbs[i].contains(x, y)) return {Control::BoardCrumb, static_cast<int>(i) - 1};
            return {};
        }
        if (!b.canvas.contains(x, y)) return {};
        if (m.board_view == 3 && m.board_library) {
            const LibraryLayout l = layout_library(b.canvas, *m.board_library, m.board_left_scroll, m.board_right_scroll, m.board_hover_export);
            for (std::size_t i = 0; i < l.left_rows.size(); ++i)
                if (l.left_rows[i].contains(x, y)) return {Control::BoardLeft, l.left_first + static_cast<int>(i)};
            for (std::size_t i = 0; i < l.right_rows.size(); ++i)
                if (l.right_rows[i].contains(x, y)) return {Control::BoardRight, l.right_first + static_cast<int>(i)};
            if (m.board_hover_export >= 0 && static_cast<std::size_t>(m.board_hover_export) < m.board_library->right.size()) {
                const std::vector<board::LibraryUser>& users = m.board_library->right[static_cast<std::size_t>(m.board_hover_export)].users;
                for (std::size_t i = 0; i < l.user_rows.size() && i < users.size(); ++i)
                    if (l.user_rows[i].contains(x, y)) return {Control::BoardUser, static_cast<int>(users[i].module)};
            }
            return {};
        }
        if (m.board_view == 1 && m.board_flow) {
            const FlowLayout f = layout_flow(b.canvas, *m.board_flow);
            for (std::size_t i = 0; i < f.in.size(); ++i)
                if (f.in[i].bar.contains(x, y) || f.in[i].label.contains(x, y)) return {Control::BoardFlowBand, static_cast<int>(i)};
            for (std::size_t i = 0; i < f.out.size(); ++i)
                if (f.out[i].bar.contains(x, y) || f.out[i].label.contains(x, y)) return {Control::BoardFlowBand, 100 + static_cast<int>(i)};
        } else if (m.board_view == 2 && m.board_trace) {
            const TraceLayout t = layout_trace(b.canvas, *m.board_trace);
            for (std::size_t c = 0; c < board::kTraceColumns; ++c)
                for (std::size_t p = 0; p < t.nodes[c].size(); ++p)
                    if (t.nodes[c][p].contains(x, y)) return {Control::BoardNode, static_cast<int>(m.board_trace->columns[c][p])};
        }
        return {};
    }
    if (!m.board || !b.tableau.contains(x, y)) return {};
    const float pitch = b.column_width + kBoardColumnGap;
    const float along = x - b.tableau.x + m.board_hscroll;
    if (along < 0) return {};
    const auto slot = static_cast<std::size_t>(along / pitch);
    if (slot >= board_columns_shown(m) || along - static_cast<float>(slot) * pitch >= b.column_width) return {};
    std::size_t column = 0;
    for (std::size_t seen = 0; column < board::kPileCount; ++column) {
        if (!board_column_shown(m, column)) continue;
        if (seen++ == slot) break;
    }
    const BoardColumn c = compute_board_column(b, m, column);
    if (c.foundation.contains(x, y)) return {Control::BoardFoundation, static_cast<int>(column)};
    if (c.head.contains(x, y) || c.deck.contains(x, y)) return {Control::BoardExpand, static_cast<int>(column)};
    if (!c.area.contains(x, y) || c.last <= c.first) return {};
    int position = static_cast<int>(std::floor((y - c.top) / c.pitch));
    if (position < c.first) return {};
    if (!c.expanded) position = std::min(position, c.last - 1);   // the last card of a fan is whole
    if (position >= c.last) return {};
    if (!board_card_rect(c, position).contains(x, y)) return {};
    return {Control::BoardCard, static_cast<int>((*c.cards)[static_cast<std::size_t>(position)])};
}

int board_foundation_at(const BoardLayout& b, const Model& m, float x, float y) {
    if (!m.board) return -1;
    for (std::size_t i = 0; i < board::kPileCount; ++i) {
        const BoardColumn c = compute_board_column(b, m, i);
        if (c.foundation.contains(x, y) && b.tableau.contains(x, y)) return static_cast<int>(i);
    }
    return -1;
}

std::vector<PageHit> focus_order_board(const Model& m) {
    std::vector<PageHit> order{{Control::BoardSearch, -1}, {Control::BoardOpen, -1}};
    if (m.board)
        for (int i = 0; i < 4; ++i) order.push_back({Control::BoardViewTab, i});
    if (m.board_busy) order.push_back({Control::BoardCancel, -1});
    if (m.board) {
        if (m.board_scores && m.board_view == 0) order.push_back({Control::BoardLeaders, -1});
        if (m.board_leaders_open && m.board_view == 0) order.push_back({Control::BoardLeadersSearch, -1});
        if (m.board_scores && m.board_view == 0)
            for (int i = 1; i <= static_cast<int>(board::kCViews); ++i) order.push_back({Control::BoardCView, i});
        for (int i = 0; i < static_cast<int>(board::kPileCount); ++i)
            if (board_column_shown(m, static_cast<std::size_t>(i))) order.push_back({Control::BoardFoundation, i});
        for (int i = 0; i < static_cast<int>(board::kPileCount); ++i)
            if (board_column_shown(m, static_cast<std::size_t>(i))) order.push_back({Control::BoardExpand, i});
        if (m.board_selected >= 0) {
            order.push_back({Control::BoardCardOpen, m.board_selected});
            order.push_back({Control::BoardCardConvert, m.board_selected});
        }
    }
    order.push_back({Control::NavConvert, -1});
    order.push_back({Control::NavTools, -1});
    order.push_back({Control::NavBoard, -1});
    order.push_back({Control::NavSettings, -1});
    return order;
}

InspectorLayout layout_inspector(const BoardLayout& b, const Model& m) {
    InspectorLayout l;
    if (b.drawer.w <= 0 || !m.board_inspector) return l;
    l.list = {b.drawer.x + 6, b.drawer.y + 34, b.drawer.w - 12, std::max(0.0f, b.drawer.h - 40)};
    const float content = static_cast<float>(m.board_inspector->rows.size()) * kInspectorRowHeight;
    l.max_scroll = std::max(0.0f, content - l.list.h);
    const float scroll = std::clamp(m.board_inspect_scroll, 0.0f, l.max_scroll);
    l.first = static_cast<int>(scroll / kInspectorRowHeight);
    const float offset = l.list.y - (scroll - static_cast<float>(l.first) * kInspectorRowHeight);
    for (std::size_t i = static_cast<std::size_t>(l.first); i < m.board_inspector->rows.size(); ++i) {
        const float y = offset + static_cast<float>(i - static_cast<std::size_t>(l.first)) * kInspectorRowHeight;
        if (y >= l.list.bottom()) break;
        l.rows.push_back({l.list.x, y, l.list.w, kInspectorRowHeight - 1});
    }
    return l;
}

void board_open_compare(Model& m, int row) {
    if (!m.board || !m.board_index || !m.board_inspector || row < 0 || static_cast<std::size_t>(row) >= m.board_inspector->rows.size()) return;
    const board::InspectorRow& r = m.board_inspector->rows[static_cast<std::size_t>(row)];
    if (r.kind != board::InspectorRow::Kind::Function) return;
    m.board_compare_list.clear();
    m.board_compare_at = -1;
    for (std::size_t i = 0; i < m.board_inspector->rows.size(); ++i) {
        const board::InspectorRow& other = m.board_inspector->rows[i];
        if (other.kind != board::InspectorRow::Kind::Function) continue;
        if (static_cast<int>(i) == row) m.board_compare_at = static_cast<int>(m.board_compare_list.size());
        m.board_compare_list.push_back({m.board_inspector->card, other.text, other.detail});
    }
    board_compare_step(m, 0);
}

void board_close_compare(Model& m) {
    m.board_compare.reset();
    m.board_compare_scroll = {};
    m.board_compare_list.clear();
    m.board_compare_at = -1;
}

void board_compare_step(Model& m, int delta) {
    if (!m.board || !m.board_index || m.board_compare_list.empty()) return;
    const int at = std::clamp(m.board_compare_at + delta, 0, static_cast<int>(m.board_compare_list.size()) - 1);
    if (m.board_compare && at == m.board_compare_at) return;
    m.board_compare_at = at;
    const board::CCompareItem& item = m.board_compare_list[static_cast<std::size_t>(at)];
    m.board_compare = std::make_shared<const board::CFunctionCompare>(board::load_c_compare(*m.board, *m.board_index, item.card, m.board_scores.get(), item.function, item.measure));
    m.board_compare_scroll = {};
}

bool board_toggle_leaders(Model& m) {
    if (!m.board || !m.board_scores) return false;
    m.board_leaders_open = !m.board_leaders_open;
    m.board_leaders_scroll = 0;
    return m.board_leaders_open && !m.board_leaders_busy && (!m.board_leaders || m.board_leaders->scored != m.board_scores->scored);
}

void board_close_leaders(Model& m) {
    m.board_leaders_open = false;
    m.board_leaders_scroll = 0;
    if (m.focus == Control::BoardLeadersSearch) m.focus = Control::None;
}

const std::vector<board::CLeader>& board_leader_rows(const Model& m) {
    static const std::vector<board::CLeader> none;
    if (!m.board_leaders) return none;
    return m.board_leaders_query.find_first_not_of(' ') == std::string::npos ? m.board_leaders->top : m.board_leaders_found;
}

void board_search_leaders(Model& m) {
    m.board_leaders_scroll = 0;
    m.board_leaders_found.clear();
    m.board_leaders_matches = 0;
    if (!m.board_leaders) return;
    board::CSearch found = board::search_c_catalog(m.board_leaders->catalog, m.board_leaders_query);
    m.board_leaders_found = std::move(found.rows);
    m.board_leaders_matches = found.matches;
}

std::filesystem::path board_export_leaders(const Model& m) {
    const bool searching = m.board_leaders_query.find_first_not_of(' ') != std::string::npos;
    const std::vector<board::CLeader> rows = searching && m.board_leaders ? board::search_c_catalog(m.board_leaders->catalog, m.board_leaders_query, std::numeric_limits<std::size_t>::max()).rows : board_leader_rows(m);
    const std::filesystem::path folder = m.board->root / "exports";
    fs::make_dirs(folder);
    const std::filesystem::path file = folder / board::c_leaders_file_name(searching ? std::string_view(m.board_leaders_query) : std::string_view());
    fs::write_file_atomic(file, as_bytes(board::c_leaders_tsv(rows)));
    return file;
}

void board_open_leader(Model& m, int rank) {
    const std::vector<board::CLeader>& rows = board_leader_rows(m);
    if (!m.board || !m.board_index || !m.board_leaders || rank < 0 || static_cast<std::size_t>(rank) >= rows.size()) return;
    std::unordered_map<std::string_view, std::uint32_t> cards;
    cards.reserve(m.board->cards.size());
    for (std::uint32_t i = 0; i < m.board->cards.size(); ++i) cards.emplace(m.board->cards[i].rel, i);
    m.board_compare_list.clear();
    m.board_compare_at = -1;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const board::CLeader& leader = rows[i];
        const auto card = cards.find(leader.rel);
        if (card == cards.end()) continue;
        if (static_cast<int>(i) == rank) m.board_compare_at = static_cast<int>(m.board_compare_list.size());
        m.board_compare_list.push_back({card->second, leader.function, leader.words});
    }
    if (m.board_compare_at < 0) {
        m.board_compare_list.clear();
        return;
    }
    m.board_compare.reset();
    board_compare_step(m, 0);
}

BoardLeadersLayout layout_board_leaders(const BoardLayout& b, const Model& m) {
    BoardLeadersLayout l;
    l.outline = b.tableau;
    l.close = {l.outline.right() - 34, l.outline.y + 4, 28, 26};
    l.export_button = {l.close.x - 6 - 62, l.outline.y + 5, 62, 24};
    l.search = {l.export_button.x - 6 - 230, l.outline.y + 3, 230, 28};
    l.title = {l.outline.x + 12, l.outline.y + 4, std::max(0.0f, l.search.x - 8 - l.outline.x - 12), 26};
    l.head = {l.outline.x + 8, l.outline.y + 32, std::max(0.0f, l.outline.w - 16), 20};
    l.list = {l.head.x, l.head.bottom(), l.head.w, std::max(0.0f, l.outline.bottom() - 6 - l.head.bottom())};
    if (!m.board_leaders) return l;
    const std::size_t count = board_leader_rows(m).size();
    l.max_scroll = std::max(0.0f, static_cast<float>(count) * kLeaderRowHeight - l.list.h);
    const float scroll = std::clamp(m.board_leaders_scroll, 0.0f, l.max_scroll);
    l.first = static_cast<int>(scroll / kLeaderRowHeight);
    const float offset = l.list.y - (scroll - static_cast<float>(l.first) * kLeaderRowHeight);
    for (std::size_t i = static_cast<std::size_t>(l.first); i < count; ++i) {
        const float y = offset + static_cast<float>(i - static_cast<std::size_t>(l.first)) * kLeaderRowHeight;
        if (y >= l.list.bottom()) break;
        l.rows.push_back({l.list.x, y, l.list.w, kLeaderRowHeight - 1});
    }
    return l;
}

BoardCompareLayout layout_board_compare(const BoardLayout& b) {
    constexpr float kInset = 12, kTitleH = 30, kHeadH = 22, kFooterH = 22, kGap = 8;
    BoardCompareLayout p;
    p.outline = b.tableau;
    p.close = {p.outline.right() - 34, p.outline.y + 4, 28, 26};
    p.next = {p.close.x - 4 - 28, p.outline.y + 4, 28, 26};
    p.prev = {p.next.x - 2 - 28, p.outline.y + 4, 28, 26};
    p.position = {p.prev.x - 6 - 84, p.outline.y + 4, 84, 26};
    p.title = {p.outline.x + kInset, p.outline.y + 4, std::max(0.0f, p.position.x - 6 - p.outline.x - kInset), kTitleH - 4};
    p.footer = {p.outline.x + kInset, p.outline.bottom() - kFooterH, std::max(0.0f, p.outline.w - 2 * kInset), kFooterH - 4};
    const float width = std::max(0.0f, (p.outline.w - 2 * kInset - kGap) / 2);
    const float head_y = p.outline.y + kTitleH + 2, code_y = head_y + kHeadH;
    const float code_h = std::max(0.0f, p.footer.y - 4 - code_y);
    p.source_head = {p.outline.x + kInset, head_y, width, kHeadH};
    p.assembly_head = {p.source_head.right() + kGap, head_y, width, kHeadH};
    p.source = {p.source_head.x, code_y, width, code_h};
    p.assembly = {p.assembly_head.x, code_y, width, code_h};
    return p;
}

float board_compare_max_scroll(const BoardCompareLayout& panel, const Model& m, int side) {
    if (!m.board_compare) return 0;
    const std::size_t lines = side == 0 ? m.board_compare->source.size() : m.board_compare->assembly.size();
    return std::max(0.0f, static_cast<float>(lines) * kCodeLineHeight + 8 - (side == 0 ? panel.source.h : panel.assembly.h));
}

void board_inspect_card(Model& m, int card) {
    board_close_compare(m);
    if (card < 0 || !m.board || !m.board_index || static_cast<std::size_t>(card) >= m.board->cards.size()) {
        m.board_inspecting = false;
        m.board_inspector.reset();
        return;
    }
    m.board_inspecting = true;
    m.board_selected = card;
    m.board_inspect_scroll = 0;
    std::optional<board::CNearMisses> near_misses;
    if (m.board_scores) {
        const std::string rel = board::c_scored_rel(*m.board, *m.board_index, static_cast<std::uint32_t>(card), *m.board_scores);
        if (!rel.empty()) near_misses = board::load_c_near_misses(m.board->root, rel);
    }
    m.board_inspector = std::make_shared<const board::Inspector>(board::build_inspector(*m.board, *m.board_index, m.board_signals.get(), static_cast<std::uint32_t>(card), m.board_scores.get(), near_misses ? &*near_misses : nullptr));
}

void board_sync_inspector(Model& m) {
    if (m.board_inspecting && m.board_selected >= 0 && (!m.board_inspector || static_cast<int>(m.board_inspector->card) != m.board_selected)) board_inspect_card(m, m.board_selected);
}

void board_hover_card(Model& m, int card) {
    if (card < 0 || !m.board || !m.board_index || static_cast<std::size_t>(card) >= m.board_index->nodes.size()) {
        m.board_hover.reset();
        m.board_hover_linked = {};
        return;
    }
    if (m.board_hover && static_cast<int>(m.board_hover->card) == card) return;
    auto hover = std::make_shared<board::HoverLinks>(board::hover_links(*m.board_index, static_cast<std::uint32_t>(card)));
    for (std::size_t column = 0; column < board::kPileCount; ++column) {
        const std::vector<std::uint32_t>& listed = m.board_show_done[column] ? m.board_stacks[column].done : m.board_stacks[column].todo;
        std::uint32_t lit = 0;
        for (const std::uint32_t i : listed) lit += hover->role[i] != 0 ? 1 : 0;
        m.board_hover_linked[column] = lit;
    }
    m.board_hover = std::move(hover);
}

void board_rebuild_stacks(Model& m) {
    if (!m.board) return;
    board::StackOptions options;
    std::vector<std::uint32_t> group;
    if (m.board_group_copies && m.board_index && m.board_index->nodes.size() == m.board->cards.size()) {
        group.resize(m.board->cards.size());
        for (std::size_t i = 0; i < group.size(); ++i) group[i] = m.board_index->nodes[i].group;
        options.group = &group;
    }
    std::vector<char> allowed;
    if (!m.board_filters.empty() && m.board_signals) {
        allowed.assign(m.board->cards.size(), 1);
        std::vector<char> here(m.board->cards.size());
        for (const std::uint32_t signal : m.board_filters) {
            std::fill(here.begin(), here.end(), char{0});
            for (const std::uint32_t at : m.board_signals->of_signal(signal)) here[m.board_signals->links[at].node] = 1;
            for (std::size_t i = 0; i < allowed.size(); ++i) allowed[i] = static_cast<char>(allowed[i] && here[i]);
        }
        options.allowed = &allowed;
    }
    const auto only = [&](const std::vector<char>& these) {
        if (allowed.empty()) allowed = these;
        else
            for (std::size_t i = 0; i < allowed.size(); ++i) allowed[i] = static_cast<char>(allowed[i] && these[i]);
        options.allowed = &allowed;
    };
    if (m.board_orphans_only && m.board_orphans) only(m.board_orphans->card);
    if (!m.board_deck_names.empty()) only(board::cards_named(*m.board, m.board_deck_names));
    m.board_stacks = board::build_stacks(*m.board, m.board_query, options);
    m.board_c_marks.reset();
    if (m.board_scores) {   // (each time: a card that has just been converted changes its mark)
        const bool linked = m.board_index && m.board_index->nodes.size() == m.board->cards.size();
        m.board_c_marks = std::make_shared<const board::CMarks>(board::build_c_marks(*m.board, linked ? m.board_index.get() : nullptr, *m.board_scores));
        board::apply_c_view(m.board_stacks, *m.board_c_marks, m.board_c_view);
    }
    if (m.board_measured && m.board_index && !m.board_projection && m.board_index->nodes.size() == m.board->cards.size())
        m.board_projection = std::make_shared<const board::Projection>(board::build_projection(*m.board, *m.board_index, m.board_signals.get(), m.board_measured.get()));
    m.board_batch = m.board_index ? board::batch_order(*m.board, *m.board_index, m.board_stacks, m.board_measured ? m.board_projection.get() : nullptr) : std::vector<std::uint32_t>{};
    m.board_batch_armed = 0;
}

namespace {

// The filter by C result off, the columns as they were before it. The stacks are the caller's to rebuild.
void board_clear_c_view(Model& m) {
    if (m.board_c_view == board::CView::All) return;
    m.board_c_view = board::CView::All;
    m.board_show_done = m.board_c_kept_done;
    m.board_hscroll = 0;
}

}  // namespace

void board_toggle_c_view(Model& m, board::CView view) {
    if (!m.board || !m.board_scores) return;
    m.board_scroll = {};
    if (view == board::CView::All || view == m.board_c_view) {
        board_clear_c_view(m);
        board_rebuild_stacks(m);
        return;
    }
    if (m.board_c_view == board::CView::All) m.board_c_kept_done = m.board_show_done;
    m.board_c_view = view;
    m.board_hscroll = 0;
    board_rebuild_stacks(m);
    for (std::size_t i = 0; i < board::kPileCount; ++i) m.board_show_done[i] = !m.board_stacks[i].done.empty();
}

namespace {

std::pair<std::uint64_t, std::int64_t> board_scores_stamp_of(const std::filesystem::path& root) {
    std::error_code ec;
    const std::filesystem::path table = root / "c_scores.tsv";
    const std::uintmax_t size = std::filesystem::file_size(table, ec);
    if (ec) return {};
    const auto when = std::filesystem::last_write_time(table, ec);
    if (ec) return {};
    return {static_cast<std::uint64_t>(size), static_cast<std::int64_t>(when.time_since_epoch().count())};
}

}  // namespace

bool board_refresh_scores(Model& m) {
    if (!m.board || m.board_busy || m.board_drag >= 0 || m.board_batch_armed > 0) return false;
    const auto stamp = board_scores_stamp_of(m.board->root);   // (taken before reading: a row added meanwhile is read next time)
    if (stamp.first == 0 || stamp == m.board_scores_stamp) return false;
    std::optional<board::CScores> fresh;
    try {
        fresh = board::load_c_scores(m.board->root);
    } catch (const std::exception&) {
        return false;   // (the scorer has it open: next time)
    }
    if (!fresh) return false;
    const std::string before = m.board_scores ? board::c_score_words(*m.board_scores) : std::string();
    m.board_scores = std::make_shared<const board::CScores>(std::move(*fresh));
    m.board_scores_stamp = stamp;
    board_rebuild_stacks(m);
    if (m.board_c_view != board::CView::All)
        for (std::size_t i = 0; i < board::kPileCount; ++i)   // (a column a filter has just reached; one the person turned stays turned)
            if (!m.board_stacks[i].done.empty() && m.board_stacks[i].todo.empty()) m.board_show_done[i] = true;
    if (!before.empty() && m.status_text.ends_with(before)) m.status_text.replace(m.status_text.size() - before.size(), before.size(), board::c_score_words(*m.board_scores));
    if (m.board_inspecting && m.board_selected >= 0) {
        const float scroll = m.board_inspect_scroll;
        const auto compare = m.board_compare;                  // (new numbers do not close what is being read)
        const auto compare_scroll = m.board_compare_scroll;
        auto compare_list = std::move(m.board_compare_list);
        const int compare_at = m.board_compare_at;
        board_inspect_card(m, m.board_selected);
        m.board_inspect_scroll = scroll;
        m.board_compare = compare;
        m.board_compare_scroll = compare_scroll;
        m.board_compare_list = std::move(compare_list);
        m.board_compare_at = compare_at;
    }
    return true;
}

void board_toggle_orphans(Model& m) {
    if (!m.board || !m.board_index) return;
    if (!m.board_orphans) m.board_orphans = std::make_shared<const board::Orphans>(board::find_orphans(*m.board, *m.board_index, m.board_signals.get(), m.board_measured.get()));
    m.board_orphans_only = !m.board_orphans_only;
    m.board_active_deck = -1;
    m.board_scroll = {};
    board_rebuild_stacks(m);
}

void board_load_decks(Model& m) {
    m.board_decks = board::builtin_decks();
    m.board_active_deck = -1;
    m.board_deck_names.clear();
    board_clear_c_view(m);
    board_close_compare(m);
    board_close_leaders(m);
    m.board_leaders.reset();
    m.board_leaders_query.clear();
    m.board_leaders_found.clear();
    m.board_leaders_matches = 0;
    m.board_c_marks.reset();
    if (!m.board) return;
    for (board::Deck& d : board::load_decks(m.board->root)) m.board_decks.push_back(std::move(d));
    m.board_projection.reset();
    std::optional<board::Measured> kept = board::load_measured(m.board->root);
    m.board_measured = kept ? std::make_shared<const board::Measured>(std::move(*kept)) : nullptr;
    m.board_scores_stamp = board_scores_stamp_of(m.board->root);
    std::optional<board::CScores> scores = board::load_c_scores(m.board->root);
    m.board_scores = scores ? std::make_shared<const board::CScores>(std::move(*scores)) : nullptr;
}

void board_apply_deck(Model& m, int deck) {
    if (deck < 0 || static_cast<std::size_t>(deck) >= m.board_decks.size()) return;
    m.board_filters.clear();
    board_clear_c_view(m);
    m.board_scroll = {};
    if (m.board_active_deck == deck) {   // the same deck again: everything
        m.board_active_deck = -1;
        m.board_deck_names.clear();
        m.board_query.clear();
        m.board_orphans_only = false;
        board_rebuild_stacks(m);
        return;
    }
    const board::Deck& d = m.board_decks[static_cast<std::size_t>(deck)];
    m.board_active_deck = deck;
    m.board_deck_names = d.names;
    m.board_query = d.query;
    m.board_orphans_only = d.orphans;
    if (d.orphans && !m.board_orphans && m.board && m.board_index) m.board_orphans = std::make_shared<const board::Orphans>(board::find_orphans(*m.board, *m.board_index, m.board_signals.get(), m.board_measured.get()));
    if (m.board_signals) {
        for (const board::DeckFilter& f : d.filters)
            for (std::uint32_t id = 0; id < m.board_signals->signals.size(); ++id)
                if (m.board_signals->signals[id].type == f.type && m.board_signals->signals[id].value == f.value) m.board_filters.push_back(id);
    }
    board_rebuild_stacks(m);
    if (d.c_view != board::CView::All) board_toggle_c_view(m, d.c_view);
}

void board_save_deck(Model& m) {
    if (!m.board) return;
    board::Deck d;
    d.query = m.board_query;
    d.orphans = m.board_orphans_only;
    d.names = m.board_deck_names;
    d.c_view = m.board_c_view;
    std::string name;
    const auto part = [&](const std::string& words) { name += (name.empty() ? "" : " + ") + words; };
    if (m.board_active_deck >= 0 && static_cast<std::size_t>(m.board_active_deck) < m.board_decks.size()) part(m.board_decks[static_cast<std::size_t>(m.board_active_deck)].name);
    if (m.board_orphans_only) part("Orphans");
    if (m.board_c_view != board::CView::All) part(std::string(board::c_view_name(m.board_c_view)));
    if (!m.board_query.empty()) part("\"" + m.board_query + "\"");
    if (m.board_signals) {
        for (const std::uint32_t id : m.board_filters) {
            d.filters.push_back({m.board_signals->signals[id].type, m.board_signals->signals[id].value});
            part(board_filter_words(m, id, 28));
        }
    }
    d.name = name.empty() ? "All files" : name.substr(0, 60);
    m.board_decks.push_back(std::move(d));
    m.board_active_deck = static_cast<int>(m.board_decks.size()) - 1;
    board::save_decks(m.board->root, m.board_decks);
}

void board_remove_deck(Model& m, int deck) {
    if (!m.board || deck < 0 || static_cast<std::size_t>(deck) >= m.board_decks.size() || m.board_decks[static_cast<std::size_t>(deck)].builtin) return;
    m.board_decks.erase(m.board_decks.begin() + deck);
    if (m.board_active_deck == deck) {
        m.board_active_deck = -1;
        m.board_deck_names.clear();
        board_rebuild_stacks(m);
    } else if (m.board_active_deck > deck) {
        --m.board_active_deck;
    }
    board::save_decks(m.board->root, m.board_decks);
}

std::filesystem::path board_export(Model& m) {
    const board::Graph graph = board::build_graph(*m.board, *m.board_index, board::kLinkImports | board::kLinkInstalls | board::kLinkReferences);
    if (!m.board_projection) m.board_projection = std::make_shared<const board::Projection>(board::build_projection(*m.board, *m.board_index, m.board_signals.get(), m.board_measured.get()));
    std::vector<std::uint32_t> modules;
    std::string slug = "board";
    if (m.board_view != 0 && m.board_trace) {
        for (const auto& column : m.board_trace->columns) modules.insert(modules.end(), column.begin(), column.end());
        slug = graph.modules[m.board_trace->focus].name;
    } else {
        for (const board::Stack& stack : m.board_stacks) {
            for (const std::uint32_t card : stack.todo) modules.push_back(graph.module_of_card[card]);
            for (const std::uint32_t card : stack.done) modules.push_back(graph.module_of_card[card]);
        }
        if (m.board_active_deck >= 0 && static_cast<std::size_t>(m.board_active_deck) < m.board_decks.size()) slug = m.board_decks[static_cast<std::size_t>(m.board_active_deck)].name;
        else if (m.board_orphans_only) slug = "orphans";
    }
    for (char& c : slug)
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '.' || c == '-')) c = '_';
    const board::Cluster cluster = board::build_cluster(graph, *m.board_index, m.board_projection.get(), std::move(modules));
    const std::filesystem::path folder = m.board->root / "exports";
    fs::make_dirs(folder);
    fs::write_file_atomic(folder / (slug + ".json"), as_bytes(board::cluster_json(*m.board, *m.board_index, m.board_signals.get(), graph, cluster)));
    fs::write_file_atomic(folder / (slug + ".graphml"), as_bytes(board::cluster_graphml(*m.board, *m.board_index, graph, cluster)));
    fs::write_file_atomic(folder / (slug + "-maltego.csv"), as_bytes(board::cluster_table(*m.board, graph, cluster)));
    return folder / slug;
}

DeckPanel layout_deck_panel(const BoardLayout& b, const Model& m) {
    constexpr float kWidth = 340;
    DeckPanel p;
    const float x = std::clamp(b.decks_button.right() - kWidth, b.card.x + 8, std::max(b.card.x + 8, b.card.right() - kWidth - 8));
    const float height = std::min((static_cast<float>(m.board_decks.size()) + 1) * kDeckRowHeight + 8, std::max(40.0f, b.card.bottom() - b.decks_button.bottom() - 12));
    p.outline = {x, b.decks_button.bottom() + 4, kWidth, height};
    p.save = {x + 4, p.outline.y + 4, kWidth - 8, kDeckRowHeight - 2};
    for (std::size_t i = 0; i < m.board_decks.size(); ++i) {
        const float y = p.save.y + static_cast<float>(i + 1) * kDeckRowHeight;
        if (y + kDeckRowHeight > p.outline.bottom()) break;
        p.rows.push_back({x + 4, y, kWidth - 8, kDeckRowHeight - 2});
        p.removes.push_back(m.board_decks[i].builtin ? Rect{} : Rect{x + kWidth - 32, y + 2, 24, kDeckRowHeight - 6});
    }
    return p;
}

void board_open_signals(Model& m, int type) {
    if (m.board_signal_type == type || !m.board_signals || type < 0 || static_cast<std::size_t>(type) >= board::kSignalTypes) {
        m.board_signal_type = -1;
        m.board_signal_rows.clear();
        return;
    }
    m.board_signal_type = type;
    m.board_signal_scroll = 0;
    m.board_signal_rows.clear();
    std::vector<std::uint32_t> files;
    for (std::uint32_t id = 0; id < m.board_signals->signals.size(); ++id) {
        if (static_cast<int>(m.board_signals->signals[id].type) != type) continue;
        files.clear();
        for (const std::uint32_t at : m.board_signals->of_signal(id)) {
            const std::uint32_t node = m.board_signals->links[at].node;
            files.push_back(m.board_index && node < m.board_index->nodes.size() ? m.board_index->nodes[node].group : node);
        }
        std::sort(files.begin(), files.end());
        const auto distinct = static_cast<std::uint32_t>(std::unique(files.begin(), files.end()) - files.begin());
        m.board_signal_rows.emplace_back(id, distinct);
    }
    std::stable_sort(m.board_signal_rows.begin(), m.board_signal_rows.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
}

void board_toggle_filter(Model& m, std::uint32_t signal) {
    const auto it = std::find(m.board_filters.begin(), m.board_filters.end(), signal);
    if (it != m.board_filters.end()) m.board_filters.erase(it);
    else m.board_filters.push_back(signal);
    m.board_scroll = {};
    board_rebuild_stacks(m);
}

SignalPanel layout_signal_panel(const BoardLayout& b, const Model& m) {
    constexpr float kWidth = 460, kMaxHeight = 420;
    SignalPanel p;
    if (m.board_signal_type < 0 || static_cast<std::size_t>(m.board_signal_type) >= b.signal_pills.size()) return p;
    const Rect& pill = b.signal_pills[static_cast<std::size_t>(m.board_signal_type)];
    const float x = std::clamp(pill.x, b.card.x + 8, std::max(b.card.x + 8, b.card.right() - kWidth - 8));
    const float content = static_cast<float>(m.board_signal_rows.size()) * kSignalRowHeight;
    const float height = std::min({kMaxHeight, content + 8, std::max(40.0f, b.card.bottom() - pill.bottom() - 12)});
    p.outline = {x, pill.bottom() + 4, kWidth, height};
    p.list = {x + 4, p.outline.y + 4, kWidth - 8, std::max(0.0f, height - 8)};
    p.max_scroll = std::max(0.0f, content - p.list.h);
    const float scroll = std::clamp(m.board_signal_scroll, 0.0f, p.max_scroll);
    p.first = static_cast<int>(scroll / kSignalRowHeight);
    const float offset = p.list.y - (scroll - static_cast<float>(p.first) * kSignalRowHeight);
    for (std::size_t i = static_cast<std::size_t>(p.first); i < m.board_signal_rows.size(); ++i) {
        const float y = offset + static_cast<float>(i - static_cast<std::size_t>(p.first)) * kSignalRowHeight;
        if (y >= p.list.bottom()) break;
        p.rows.push_back({p.list.x, y, p.list.w, kSignalRowHeight - 2});
    }
    return p;
}

std::string board_filter_words(const Model& m, std::uint32_t signal, std::size_t room) {
    if (!m.board_signals || signal >= m.board_signals->signals.size()) return {};
    const board::Signal& s = m.board_signals->signals[signal];
    std::string words = std::string(board::signal_type_name(s.type)) + ": " + s.value;
    if (words.size() > room) words = words.substr(0, room - 1) + "\xE2\x80\xA6";
    return words;
}

namespace {

// The bands of one side, spread down `area`, each as tall as its share of `total` (never under 3), with their ends on the centre bar. `total` is the larger
// side's, so both sides share one scale: two links never look as thick as eight hundred.
std::vector<FlowRow> flow_rows(const std::vector<board::FlowBand>& bands, std::uint64_t total, const Rect& area, const Rect& center, float bar_x, float label_x, float label_w) {
    constexpr float kGap = 6, kMin = 3, kBar = 10;
    std::vector<FlowRow> rows(bands.size());
    if (bands.empty() || total == 0) return rows;
    const float room = std::max(0.0f, area.h - kGap * static_cast<float>(bands.size() - 1) - kMin * static_cast<float>(bands.size()));
    float used = kGap * static_cast<float>(bands.size() - 1);
    std::vector<float> heights(bands.size());
    for (std::size_t i = 0; i < bands.size(); ++i) {
        heights[i] = kMin + room * static_cast<float>(bands[i].weight) / static_cast<float>(total);
        used += heights[i];
    }
    std::uint64_t side = 0;
    for (const board::FlowBand& band : bands) side += band.weight;
    float y = area.y + std::max(0.0f, (area.h - used) / 2);
    float at = center.y + (center.h - center.h * static_cast<float>(side) / static_cast<float>(total)) / 2;
    for (std::size_t i = 0; i < bands.size(); ++i) {
        const float slice = center.h * static_cast<float>(bands[i].weight) / static_cast<float>(total);
        rows[i].bar = {bar_x, y, kBar, heights[i]};
        rows[i].label = {label_x, y + heights[i] / 2 - 9, label_w, 18};
        rows[i].slice = {center.x, at, center.w, slice};
        at += slice;
        y += heights[i] + kGap;
    }
    return rows;
}

}  // namespace

FlowLayout layout_flow(const Rect& area, const board::Flow& flow) {
    constexpr float kLabelW = 176, kTitleH = 40;
    FlowLayout f;
    f.title = {area.x, area.y, area.w, kTitleH};
    const Rect body{area.x, area.y + kTitleH, area.w, std::max(0.0f, area.h - kTitleH - 12)};   // (room under the last band for its words)
    f.center = {area.x + area.w / 2 - 6, body.y + body.h * 0.14f, 12, body.h * 0.72f};
    const std::uint64_t scale = std::max(flow.in_total, flow.out_total);
    f.in = flow_rows(flow.in, scale, body, f.center, area.x + kLabelW + 6, area.x, kLabelW);
    f.out = flow_rows(flow.out, scale, body, f.center, area.right() - kLabelW - 16, area.right() - kLabelW, kLabelW);
    return f;
}

TraceLayout layout_trace(const Rect& area, const board::Trace& trace) {
    constexpr float kHeader = 22;
    TraceLayout t;
    const float width = area.w / static_cast<float>(board::kTraceColumns);
    const float top = area.y + kHeader;
    const auto fit = static_cast<std::size_t>(std::max(0.0f, (area.bottom() - top - kTraceRowPitch) / kTraceRowPitch));
    for (std::size_t c = 0; c < board::kTraceColumns; ++c) {
        const float x = area.x + static_cast<float>(c) * width;
        t.headers[c] = {x, area.y, width, kHeader - 2};
        const std::size_t shown = std::min(trace.columns[c].size(), fit);
        const float first = c == board::kTraceCenter ? top + (area.bottom() - top) / 2 - kTraceRowPitch / 2 : top;
        for (std::size_t p = 0; p < shown; ++p) t.nodes[c].push_back({x + 5, first + static_cast<float>(p) * kTraceRowPitch, width - 10, kTraceRowPitch - 2});
        t.more_count[c] = trace.hidden[c] + static_cast<std::uint32_t>(trace.columns[c].size() - shown);
        if (t.more_count[c] > 0) t.more[c] = {x + 5, top + static_cast<float>(shown) * kTraceRowPitch, width - 10, kTraceRowPitch - 2};
    }
    return t;
}

void board_focus_module(Model& m, int module) {
    if (!m.board_graph || module < 0 || static_cast<std::size_t>(module) >= m.board_graph->modules.size()) {
        m.board_flow.reset();
        m.board_trace.reset();
        return;
    }
    m.board_focus = module;
    const auto id = static_cast<std::uint32_t>(module);
    m.board_selected = static_cast<int>(m.board_graph->modules[id].card);
    m.board_flow = std::make_shared<const board::Flow>(board::compute_flow(*m.board_graph, id));
    m.board_trace = std::make_shared<const board::Trace>(board::compute_trace(*m.board_graph, id, kTraceKept));
    m.board_library = std::make_shared<const board::LibraryView>(board::compute_library(*m.board_graph, *m.board_index, id, m.board_width, m.board_function));
    m.board_hover_export = -1;
    m.board_left_scroll = m.board_right_scroll = 0;
}

void board_go_to(Model& m, int module, const std::string& function) {
    if (!m.board_graph || module < 0 || static_cast<std::size_t>(module) >= m.board_graph->modules.size()) return;
    const std::pair<int, std::string> here{module, function};
    if (m.board_crumbs.empty() || m.board_crumbs.back() != here) m.board_crumbs.push_back(here);
    m.board_function = function;
    board_focus_module(m, module);
}

void board_crumb_back(Model& m, int index) {
    if (index < 0 || static_cast<std::size_t>(index) >= m.board_crumbs.size()) {
        m.board_crumbs.clear();
        m.board_function.clear();
        m.board_view = 0;
        return;
    }
    m.board_crumbs.resize(static_cast<std::size_t>(index) + 1);
    m.board_function = m.board_crumbs.back().second;
    board_focus_module(m, m.board_crumbs.back().first);
}

void board_step_back(Model& m) { board_crumb_back(m, static_cast<int>(m.board_crumbs.size()) - 2); }

std::vector<std::string> board_crumb_words(const Model& m) {
    std::vector<std::string> words{"Board"};
    if (!m.board_graph || !m.board) return words;
    for (const auto& [module, function] : m.board_crumbs) {
        if (module < 0 || static_cast<std::size_t>(module) >= m.board_graph->modules.size()) continue;
        std::string word = m.board->cards[m.board_graph->modules[static_cast<std::size_t>(module)].card].name;
        if (!function.empty()) {   // the function as the library spells it
            std::string shown = function;
            if (m.board_index)
                for (const board::ExportedSymbol& e : m.board_index->exports_of(m.board_graph->modules[static_cast<std::size_t>(module)].card))
                    if (!e.name.empty() && text::lower(e.name) == function) shown = e.name;
            word += " ! " + shown;
        }
        words.push_back(std::move(word));
    }
    return words;
}

std::string board_hot_path_words(const Model& m) {
    if (!m.board_projection || !m.board_graph || !m.board || m.board_projection->hot_path.empty()) return {};
    std::string words;
    for (const std::uint32_t module : m.board_projection->hot_path) {
        if (!words.empty()) words += " \xE2\x80\xBA ";
        words += m.board_graph->modules[module].name;
    }
    return words + "  \xC2\xB7  peak " + board::rate_words(m.board_projection->max_channel) + "/h";
}

CrumbLayout layout_crumbs(const Rect& area, const std::vector<std::string>& words) {
    constexpr float kCharW = 6.6f, kCrumbPad = 12, kSeparator = 16, kMoreW = 26;
    CrumbLayout out;
    out.crumbs.resize(words.size());
    if (words.empty()) return out;
    const auto width = [&](std::size_t i) { return std::min(220.0f, static_cast<float>(words[i].size()) * kCharW) + kCrumbPad; };
    std::vector<bool> shown(words.size(), true);
    float used = 0;
    for (std::size_t i = 0; i < words.size(); ++i) used += width(i) + (i > 0 ? kSeparator : 0);
    bool cut = false;
    for (std::size_t i = 1; i + 1 < words.size() && used + (cut ? 0 : kMoreW + kSeparator) > area.w; ++i) {   // the oldest in the middle give way first
        shown[i] = false;
        used -= width(i) + kSeparator;
        if (!cut) used += kMoreW + kSeparator;
        cut = true;
    }
    float x = area.x;
    for (std::size_t i = 0; i < words.size(); ++i) {
        if (!shown[i]) {
            if (out.more.w == 0) {
                x += kSeparator;
                out.more = {x, area.y, kMoreW, area.h};
                x += kMoreW;
            }
            continue;
        }
        if (i > 0) x += kSeparator;
        out.crumbs[i] = {x, area.y, width(i), area.h};
        x += width(i);
    }
    return out;
}

float library_thickness(std::uint64_t weight, std::uint64_t heaviest) {
    if (heaviest == 0) return 1;
    return std::clamp(static_cast<float>(weight) / static_cast<float>(heaviest) * (kLibraryRowPitch - 4), 1.0f, kLibraryRowPitch - 4);
}

LibraryLayout layout_library(const Rect& area, const board::LibraryView& view, float left_scroll, float right_scroll, int hover) {
    constexpr float kHead = 20, kTitle = 38, kLeftW = 190, kRightW = 230, kUsersW = 190, kLinesW = 110;   // (kLinesW: the room an export's lines cross to its files)
    LibraryLayout l;
    l.title = {area.x + kLeftW, area.y, std::max(0.0f, area.w - kLeftW - kRightW - kUsersW), kTitle};
    const float top = area.y + kHead, list_h = std::max(0.0f, area.bottom() - top);
    l.left_area = {area.x, top, kLeftW, list_h};
    l.users_area = {area.right() - kUsersW, top, kUsersW, list_h};
    const float gap = std::max(40.0f, (area.w - kLeftW - kRightW - kUsersW - kLinesW - 12) / 2);
    const float center_x = l.left_area.right() + gap;
    l.center = {center_x, top + kTitle, 12, std::max(0.0f, list_h - kTitle - 8)};
    l.right_area = {center_x + 12 + gap, top, kRightW, list_h};
    l.left_head = {l.left_area.x, area.y, kLeftW, kHead - 2};
    l.right_head = {l.right_area.x, area.y, kRightW, kHead - 2};
    l.users_head = {l.users_area.x, area.y, kUsersW, kHead - 2};
    const auto side = [&](const std::vector<board::LibraryChannel>& channels, std::uint64_t total, const Rect& list, float scroll, std::vector<Rect>& rows, std::vector<Rect>& slices, int& first, float& max_scroll) {
        const float content = static_cast<float>(channels.size()) * kLibraryRowPitch;
        max_scroll = std::max(0.0f, content - list.h);
        scroll = std::clamp(scroll, 0.0f, max_scroll);
        first = static_cast<int>(scroll / kLibraryRowPitch);
        const float offset = list.y - (scroll - static_cast<float>(first) * kLibraryRowPitch);
        std::uint64_t before = 0;
        for (std::size_t i = 0; i < channels.size(); ++i) {
            const float y = offset + (static_cast<float>(i) - static_cast<float>(first)) * kLibraryRowPitch;
            if (static_cast<int>(i) >= first && y < list.bottom()) {
                rows.push_back({list.x, y, list.w, kLibraryRowPitch - 2});
                const float share = total == 0 ? 0 : static_cast<float>(before) / static_cast<float>(total);
                const float size = total == 0 ? 0 : static_cast<float>(channels[i].weight) / static_cast<float>(total);
                slices.push_back({l.center.x, l.center.y + l.center.h * share, l.center.w, std::max(0.5f, l.center.h * size)});
            }
            before += channels[i].weight;
        }
    };
    side(view.left, view.left_total, l.left_area, left_scroll, l.left_rows, l.left_slices, l.left_first, l.left_max_scroll);
    side(view.right, view.right_total, l.right_area, right_scroll, l.right_rows, l.right_slices, l.right_first, l.right_max_scroll);
    if (hover >= 0 && static_cast<std::size_t>(hover) < view.right.size()) {
        const std::vector<board::LibraryUser>& users = view.right[static_cast<std::size_t>(hover)].users;
        const auto fit = static_cast<std::size_t>(std::max(0.0f, (list_h - kLibraryRowPitch) / kLibraryRowPitch));
        const std::size_t shown = std::min(fit, users.size());
        for (std::size_t i = 0; i < shown; ++i) l.user_rows.push_back({l.users_area.x, top + static_cast<float>(i) * kLibraryRowPitch, kUsersW, kLibraryRowPitch - 2});
        l.users_more = users.size() - shown;
    }
    return l;
}

void board_refresh_maps(Model& m) {
    m.board_graph.reset();
    m.board_flow.reset();
    m.board_trace.reset();
    m.board_library.reset();
    if (!m.board || !m.board_index || m.board_index->nodes.size() != m.board->cards.size()) return;
    m.board_graph = std::make_shared<const board::Graph>(board::build_graph(*m.board, *m.board_index, m.board_links, m.board_width));
    if (!m.board_projection) m.board_projection = std::make_shared<const board::Projection>(board::build_projection(*m.board, *m.board_index, m.board_signals.get(), m.board_measured.get()));
    int focus = m.board_focus;
    if (focus < 0 && m.board_selected >= 0 && static_cast<std::size_t>(m.board_selected) < m.board_graph->module_of_card.size())
        focus = static_cast<int>(m.board_graph->module_of_card[static_cast<std::size_t>(m.board_selected)]);
    if (focus < 0)
        if (const auto most = m.board_graph->most_linked()) focus = static_cast<int>(*most);
    if (m.board_crumbs.empty()) board_go_to(m, focus);
    else board_focus_module(m, focus);
}

void board_show_view(Model& m, int view) {
    m.board_view = std::clamp(view, 0, 3);
    if (m.board_view == 0) return;
    if (!m.board_graph) board_refresh_maps(m);
    if (m.board_graph && m.board_selected >= 0 && static_cast<std::size_t>(m.board_selected) < m.board_graph->module_of_card.size()) {
        const int module = static_cast<int>(m.board_graph->module_of_card[static_cast<std::size_t>(m.board_selected)]);
        if (module != m.board_focus || m.board_crumbs.empty()) {   // (another card was chosen on the board: the way goes on from there)
            m.board_function.clear();
            board_go_to(m, module);
        }
    }
}

}  // namespace cb::app::window
