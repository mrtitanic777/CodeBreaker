// cb/app/window_convert.cpp -- see window_convert.hpp.
#include "cb/app/window_convert.hpp"

#include <algorithm>

#include "cb/app/window_functions.hpp"

namespace cb::app::window {

QueueLayout compute_queue_layout(const Layout& l, const Model& m) {
    QueueLayout q;
    const float x = l.left_card.x + 24, w = l.left_card.w - 48;
    const float top = l.left_desc.bottom() + 16;
    // above "Cancel all" (the card's foot) and a 12 gap
    const float bottom = l.left_card.bottom() - 20 - 32 - 12;
    q.area = {x, top, w, std::max(0.0f, bottom - top)};
    const float bar_w = std::min(260.0f, w * 0.3f);
    for (std::size_t i = 0; i < m.queue.size(); ++i) {
        const float y = top + static_cast<float>(i) * (kQueueRowHeight + kQueueRowGap) - m.queue_scroll;
        QueueRowRects r;
        r.row = {x, y, w, kQueueRowHeight};
        r.icon = {x + 14, y + (kQueueRowHeight - 16) / 2, 16, 16};
        r.status = {r.row.right() - 12 - 90, y, 90, kQueueRowHeight};
        r.bar = {r.status.x - 12 - bar_w, y + (kQueueRowHeight - 4) / 2, bar_w, 4};
        r.text = {x + 44, y, std::max(0.0f, r.bar.x - 12 - (x + 44)), kQueueRowHeight};
        q.rows.push_back(r);
    }
    q.content_height = m.queue.empty() ? 0.0f : static_cast<float>(m.queue.size()) * (kQueueRowHeight + kQueueRowGap) - kQueueRowGap;
    return q;
}

float queue_max_scroll(const QueueLayout& q) { return std::max(0.0f, q.content_height - q.area.h); }

PreviewLayout compute_preview_layout(const Layout& l, const Model& m) {
    PreviewLayout p;
    if (l.preview_list.w <= 0) return p;   // (the Files tab only)
    p.list_area = {l.preview_list.x + 8, l.preview_list.y + 8, std::max(0.0f, l.preview_list.w - 16), std::max(0.0f, l.preview_list.h - 16)};
    for (std::size_t i = 0; i < m.preview_files.size(); ++i)
        p.file_rows.push_back({p.list_area.x, p.list_area.y + static_cast<float>(i) * kFileRowHeight - m.preview_list_scroll, p.list_area.w, kFileRowHeight});
    p.list_content_height = static_cast<float>(m.preview_files.size()) * kFileRowHeight;
    const float note_h = m.preview_note.empty() ? 0.0f : 26.0f;
    p.code_area = {l.preview_code.x + kCodePad, l.preview_code.y + 12, std::max(0.0f, l.preview_code.w - 2 * kCodePad), std::max(0.0f, l.preview_code.h - 24 - note_h)};
    if (note_h > 0) p.note_area = {p.code_area.x, p.code_area.bottom(), p.code_area.w, note_h};
    p.visible_lines = std::max(1, static_cast<int>(p.code_area.h / kCodeLineHeight));
    p.visible_columns = std::max(1, static_cast<int>(p.code_area.w / kCodeCharWidth));
    return p;
}

int preview_max_first_line(const Model& m, const PreviewLayout& p) { return std::max(0, static_cast<int>(m.preview_lines.size()) - p.visible_lines); }

int preview_max_hscroll(const Model& m, const PreviewLayout& p) {
    std::size_t longest = 0;
    for (const std::string& line : m.preview_lines) longest = std::max(longest, line.size());
    return std::max(0, static_cast<int>(longest) - p.visible_columns);
}

float preview_list_max_scroll(const PreviewLayout& p) { return std::max(0.0f, p.list_content_height - p.list_area.h); }

PageHit hit_test_convert(const Layout& l, const Model& m, float x, float y) {
    const View view = m.view;
    if (const Control c = hit_test(l, view, x, y); c != Control::None) return {c, -1};
    if (view == View::Idle) {
        for (std::size_t i = 0; i < l.recent_rows.size() && i < m.recent.size(); ++i) {
            if (m.recent[i].missing && l.recent_remove[i].contains(x, y)) return {Control::RecentRemove, static_cast<int>(i)};
            if (l.recent_rows[i].contains(x, y)) return {Control::RecentRow, static_cast<int>(i)};
        }
    }
    if (view == View::Done) {
        if (l.tabs.w > 0) {
            if (const PageHit h = hit_test_source_tabs(l, m, x, y); h.control != Control::None) return h;
        }
        if (l.field.contains(x, y)) return {Control::OutputBox, -1};
        if (l.preview_list.w > 0) {
            const PreviewLayout p = compute_preview_layout(l, m);
            if (p.list_area.contains(x, y)) {
                for (std::size_t i = 0; i < p.file_rows.size(); ++i)
                    if (p.file_rows[i].contains(x, y)) return {Control::PreviewFile, static_cast<int>(i)};
                return {};
            }
            if (l.preview_code.contains(x, y)) return {Control::CodeView, -1};
        }
    }
    return {};
}

std::vector<PageHit> focus_order_convert(const Layout& l, const Model& m) {
    std::vector<PageHit> order;
    for (const Control c : focus_order(m.view)) {
        if (c == Control::CopyPath && l.copy_path.w <= 0) continue;   // (the Source card's other tabs have no path row)
        order.push_back({c, -1});
    }
    if (m.view == View::Idle) {
        const std::size_t shown = std::min(m.recent.size(), l.recent_rows.size());
        for (std::size_t i = 0; i < shown; ++i) order.push_back({m.recent[i].missing ? Control::RecentRemove : Control::RecentRow, static_cast<int>(i)});
    } else if (m.view == View::Done && !m.preview_files.empty()) {
        for (const PageHit& h : focus_order_source_tabs(l, m)) order.push_back(h);
        if (l.preview_list.w > 0) {
            order.push_back({Control::PreviewFile, m.preview_selected});
            order.push_back({Control::CodeView, -1});
        }
    }
    return order;
}

}  // namespace cb::app::window
