// app/window_functions.cpp -- see window_functions.hpp.
#include "app/window_functions.hpp"

#include <algorithm>

#include "app/window_convert.hpp"

namespace cb::app::window {
namespace {

constexpr float kBarWidth = 360;
constexpr float kTopRow = 32;
constexpr float kPickRowHeight = 32;
constexpr float kButtonHeight = 32;

float wf_clamp_pos(float v) { return std::max(0.0f, v); }

}  // namespace

// ---- Functions

FunctionsLayout compute_functions_layout(const Layout& l, const Model& m) {
    FunctionsLayout f;
    if (l.preview.w <= 0) return f;
    const Rect& p = l.preview;
    const float top = p.y + 4;
    f.filter = {p.right() - 132, top, 132, kTopRow};
    f.bar = {p.x, top + (kTopRow - 6) / 2, std::min(kBarWidth, p.w * 0.3f), 6};
    f.summary = {f.bar.right() + 14, top, wf_clamp_pos(f.filter.x - 12 - (f.bar.right() + 14)), kTopRow};
    const float list_top = top + kTopRow + 10;
    f.list_area = {p.x, list_top, p.w, wf_clamp_pos(p.bottom() - list_top)};
    f.shown = fnsrc::filtered(m.functions, m.fn_filter);
    for (std::size_t i = 0; i < f.shown.size(); ++i)
        f.rows.push_back({f.list_area.x, f.list_area.y + static_cast<float>(i) * kFnRowHeight - m.fn_scroll, f.list_area.w, kFnRowHeight});
    f.content_height = static_cast<float>(f.shown.size()) * kFnRowHeight;
    return f;
}

float functions_max_scroll(const FunctionsLayout& f) { return std::max(0.0f, f.content_height - f.list_area.h); }

FnRowParts fn_row_parts(const Rect& row) {
    FnRowParts p;
    const float chip_w = 176;
    p.icon = {row.x + 8, row.y, 20, row.h};
    p.chip = {row.right() - 12 - chip_w, row.y + 3, chip_w, row.h - 6};
    p.address = {p.chip.x - 24 - 90, row.y, 90, row.h};
    p.name = {row.x + 44, row.y, wf_clamp_pos(p.address.x - 12 - (row.x + 44)), row.h};
    return p;
}

std::string functions_summary(const fnsrc::Counts& c) {
    if (c.total == 0) return "No functions were found in the output folder.";
    if (c.classed() > 0) {
        const auto of = [&](verify::Class k) {
            const auto it = c.classes.find(k);
            return it == c.classes.end() ? 0 : it->second;
        };
        const std::string dot = " \xC2\xB7 ";
        const int no_compile = of(verify::Class::NoCompile);
        std::string out = std::to_string(of(verify::Class::Verified)) + " verified" + dot + std::to_string(of(verify::Class::Weak)) + " weak" + dot +
                          std::to_string(of(verify::Class::Unverified)) + " unverified" + dot + std::to_string(no_compile) + (no_compile == 1 ? " does" : " do") +
                          " not compile" + dot + std::to_string(of(verify::Class::NoSource)) + " no source";
        if (const int n = of(verify::Class::NotJudged); n > 0) out += dot + std::to_string(n) + " not run";
        if (const int n = of(verify::Class::Identified); n > 0) out += dot + std::to_string(n) + " runtime library";
        if (const int n = c.total - c.classed(); n > 0) out += dot + std::to_string(n) + " not checked";
        return out;
    }
    if (c.judged() == 0) return std::to_string(c.total) + (c.total == 1 ? " function" : " functions") + " \xC2\xB7 none checked by running them";
    return std::to_string(c.matches) + " of " + std::to_string(c.total) + " behave like the original \xC2\xB7 " + std::to_string(c.differs) + (c.differs == 1 ? " differs" : " differ") +
           " \xC2\xB7 " + std::to_string(c.unchecked) + " not checked";
}

std::string filter_name(int filter) { return filter == 1 ? "Differs" : filter == 2 ? "Not checked" : "All"; }

std::string verdict_name(convert::FunctionVerdict v) {
    switch (v) {
        case convert::FunctionVerdict::Matches: return "Behaves like the original";
        case convert::FunctionVerdict::Differs: return "Differs";
        case convert::FunctionVerdict::NotChecked: break;
    }
    return "Not checked";
}

std::string verdict_name(const convert::FunctionRecord& f) {
    if (!f.judged) return verdict_name(f.verdict);
    switch (*f.judged) {
        case verify::Class::Verified: return "Verified";
        case verify::Class::Weak: return "Weak match";
        case verify::Class::Unverified: return "Differs";
        case verify::Class::NotJudged: return "Not run";
        case verify::Class::NoCompile: return "Does not compile";
        case verify::Class::NoSource: return "No source";
        case verify::Class::Identified: return "Runtime library";
    }
    return verdict_name(f.verdict);
}

std::string empty_filter_text(int filter) {
    if (filter == 1) return "No function differs.";
    if (filter == 2) return "Every function was checked.";
    return "No functions were found in the output folder.";
}

// ---- Check

CheckLayout compute_check_layout(const Layout& l, const Model& m) {
    CheckLayout c;
    if (l.preview.w <= 0) return c;
    const Rect& p = l.preview;
    const fnsrc::Counts counts = fnsrc::count(m.functions);
    c.judged = counts.judged() > 0;
    const float cx = p.x + p.w / 2;
    if (c.judged) {
        const float ring = 132, gap = 56, col = 430, group_w = ring + gap + col;
        const float gx = p.x + std::max(0.0f, (p.w - group_w) / 2);
        const float text_h = 22 + 4 + 52 + 14 + 3 * 29 + 14 + kButtonHeight;
        const float top = p.y + std::max(0.0f, (p.h - text_h) / 2);
        c.ring = {gx, top + (text_h - ring) / 2, ring, ring};
        const float tx = gx + ring + gap;
        float y = top;
        c.title = {tx, y, col, 22};
        y += 22 + 4;
        c.body = {tx, y, std::min(col, 400.0f), 52};
        y += 52 + 14;
        for (int i = 0; i < 3; ++i) {
            c.dot[i] = {tx, y + 10.5f, 8, 8};
            c.label[i] = {tx + 26, y, col - 26 - 60, 29};
            c.count[i] = {tx + col - 60, y, 60, 29};
            y += 29;
        }
        y += 14;
        float bx = tx;
        if (counts.differs > 0) {
            c.show = {bx, y, 158, kButtonHeight};
            bx = c.show.right() + 8;
        }
        c.again = {bx, y, 123, kButtonHeight};
        if (!m.can_recheck) c.note = {tx, y + kButtonHeight + 8, col, 18};
    } else {
        const float text_h = 56 + 16 + 24 + 6 + 40 + 20 + kButtonHeight + 8 + 18;
        float y = p.y + std::max(0.0f, (p.h - text_h) / 2);
        c.mark = {cx - 28, y, 56, 56};
        y += 56 + 16;
        c.title = {cx - 260, y, 520, 24};
        y += 24 + 6;
        c.body = {cx - 230, y, 460, 40};
        y += 40 + 20;
        c.again = {cx - 61.5f, y, 123, kButtonHeight};
        if (!m.can_recheck) c.note = {cx - 260, y + kButtonHeight + 8, 520, 18};
    }
    return c;
}

// ---- Compare

CompareLayout compute_compare_layout(const Layout& l, const Model&) {
    CompareLayout c;
    if (l.preview.w <= 0) return c;
    const Rect& p = l.preview;
    const float top = p.y + 4;
    c.all = {p.x, top, 128, kTopRow};
    c.picker = {c.all.right() + 12, top, 260, kTopRow};
    c.chip = {c.picker.right() + 12, top + 5, 180, 22};
    const float hint_x = std::max(c.chip.right() + 12, p.right() - 360);
    c.hint = {hint_x, top, wf_clamp_pos(p.right() - hint_x), kTopRow};
    const float py = top + kTopRow + 14;
    const float ph = wf_clamp_pos(p.bottom() - py);
    const float pw = (p.w - 12) / 2;
    c.asm_pane = {p.x, py, pw, ph};
    c.c_pane = {p.x + pw + 12, py, pw, ph};
    for (Rect* pane : {&c.asm_pane, &c.c_pane}) {
        const bool left = pane == &c.asm_pane;
        Rect& head = left ? c.asm_head : c.c_head;
        Rect& code = left ? c.asm_code : c.c_code;
        head = Rect{pane->x, pane->y, pane->w, 34};
        code = Rect{pane->x + 14, pane->y + 34 + 10, wf_clamp_pos(pane->w - 28), wf_clamp_pos(pane->h - 34 - 18)};
    }
    c.visible_lines = std::max(1, static_cast<int>(c.asm_code.h / kCodeLineHeight));
    return c;
}

int compare_max_first_line(const Model& m, const CompareLayout& c, int side) {
    const std::size_t lines = side == 0 ? m.cmp_asm.size() : m.cmp_c.size();
    return std::max(0, static_cast<int>(lines) - c.visible_lines);
}

int compare_line_at(const Model& m, const CompareLayout& c, int side, float y) {
    const Rect& code = side == 0 ? c.asm_code : c.c_code;
    const float first = side == 0 ? m.cmp_scroll_asm : m.cmp_scroll_c;
    if (y < code.y || y >= code.bottom()) return -1;
    const int line = static_cast<int>(first) + static_cast<int>((y - code.y) / kCodeLineHeight);
    const std::size_t lines = side == 0 ? m.cmp_asm.size() : m.cmp_c.size();
    return line >= 0 && static_cast<std::size_t>(line) < lines ? line : -1;
}

bool compare_line_lit(const Model& m, int side, int line) {
    if (m.cmp_side < 0 || m.cmp_line < 0) return false;
    if (side == m.cmp_side && line == m.cmp_line) return true;
    const fnsrc::Link* link = fnsrc::link_at(m.cmp_links, m.cmp_side == 0 ? fnsrc::Side::Asm : fnsrc::Side::C, m.cmp_line);
    if (link == nullptr) return false;
    return side == 0 ? (line >= link->asm_first && line <= link->asm_last) : (line >= link->c_first && line <= link->c_last);
}

PickerLayout compute_picker_layout(const Layout& l, const Model& m) {
    PickerLayout p;
    const CompareLayout c = compute_compare_layout(l, m);
    if (c.picker.w <= 0) return p;
    const int count = static_cast<int>(m.functions.size());
    p.first = std::clamp(m.cmp_picker_top, 0, std::max(0, count - 1));
    const int rows = std::min(count - p.first, static_cast<int>(kPickerRows));
    const float h = static_cast<float>(std::max(0, rows)) * kPickRowHeight + 8;
    p.outline = {c.picker.x, c.picker.bottom() + 2, c.picker.w, h};
    for (int i = 0; i < rows; ++i) p.rows.push_back({p.outline.x + 4, p.outline.y + 4 + static_cast<float>(i) * kPickRowHeight, p.outline.w - 8, kPickRowHeight});
    return p;
}

int picker_item_at(const PickerLayout& p, float x, float y) {
    for (std::size_t i = 0; i < p.rows.size(); ++i)
        if (p.rows[i].contains(x, y)) return p.first + static_cast<int>(i);
    return -1;
}

// ---- the tabs

PageHit hit_test_source_tabs(const Layout& l, const Model& m, float x, float y) {
    if (l.tabs.w <= 0) return {};
    for (const Control c : {Control::TabFiles, Control::TabFunctions, Control::TabCheck}) {
        Rect r = control_rect(l, c);
        r.y = l.tabs.y;
        r.h = l.tabs.h;
        if (r.contains(x, y)) return {c, -1};
    }
    if (m.source_tab == SourceTab::Functions) {
        if (m.cmp_index < 0) {
            const FunctionsLayout f = compute_functions_layout(l, m);
            if (f.filter.contains(x, y)) return {Control::FnFilter, -1};
            if (f.list_area.contains(x, y))
                for (std::size_t i = 0; i < f.rows.size(); ++i)
                    if (f.rows[i].contains(x, y)) return {Control::FnRow, f.shown[i]};
            return {};
        }
        const CompareLayout c = compute_compare_layout(l, m);
        if (c.all.contains(x, y)) return {Control::FnAll, -1};
        if (c.picker.contains(x, y)) return {Control::FnPicker, -1};
        if (c.asm_code.contains(x, y)) {
            const int line = compare_line_at(m, c, 0, y);
            return line >= 0 ? PageHit{Control::CmpAsmLine, line} : PageHit{};
        }
        if (c.c_code.contains(x, y)) {
            const int line = compare_line_at(m, c, 1, y);
            return line >= 0 ? PageHit{Control::CmpCLine, line} : PageHit{};
        }
        return {};
    }
    if (m.source_tab == SourceTab::Check) {
        const CheckLayout c = compute_check_layout(l, m);
        if (c.show.w > 0 && c.show.contains(x, y)) return {Control::CheckShow, -1};
        if (c.again.contains(x, y)) return {Control::CheckAgain, -1};
    }
    return {};
}

std::vector<PageHit> focus_order_source_tabs(const Layout& l, const Model& m) {
    std::vector<PageHit> order;
    if (l.tabs.w <= 0) return order;
    order = {{Control::TabFiles, -1}, {Control::TabFunctions, -1}, {Control::TabCheck, -1}};
    if (m.source_tab == SourceTab::Functions) {
        if (m.cmp_index < 0) {
            order.push_back({Control::FnFilter, -1});
            const FunctionsLayout f = compute_functions_layout(l, m);
            const int at = std::clamp(m.focus == Control::FnRow ? m.focus_index : 0, 0, std::max(0, static_cast<int>(m.functions.size()) - 1));
            if (!f.shown.empty()) order.push_back({Control::FnRow, std::find(f.shown.begin(), f.shown.end(), at) != f.shown.end() ? at : f.shown.front()});
        } else {
            order.push_back({Control::FnAll, -1});
            order.push_back({Control::FnPicker, -1});
        }
    } else if (m.source_tab == SourceTab::Check) {
        const CheckLayout c = compute_check_layout(l, m);
        if (c.show.w > 0) order.push_back({Control::CheckShow, -1});
        if (m.can_recheck) order.push_back({Control::CheckAgain, -1});
    }
    return order;
}

// ---- the command palette

PaletteLayout compute_palette_layout(const Layout& l, const Model& m) {
    PaletteLayout p;
    const float w = std::min(kPaletteWidth, std::max(0.0f, l.width - 2 * kOuterPadding));
    const float x = (l.width - w) / 2;
    const float y = kTitleBarHeight + 48;
    const float limit = l.status_bar.y - 16;
    p.input = {x, y, w, kPaletteInputHeight};
    p.close_hint = {x + w - 120, y, 104, kPaletteInputHeight};
    float cy = p.input.bottom() + 1 + 6;
    for (const palette::Row& row : m.palette_result.rows) {
        const float h = row.heading ? kPaletteHeadingHeight : kPaletteRowHeight;
        if (cy + h > limit) {
            p.rows.push_back({});   // (no room: not shown)
            continue;
        }
        p.rows.push_back({x + 6, cy, w - 12, h});
        cy += h;
    }
    const bool any = !m.palette_result.rows.empty();
    float bottom = any ? cy + 6 : p.input.bottom() + 1 + 40;
    p.box = {x, y, w, bottom - y};
    return p;
}

int palette_pick_at(const PaletteLayout& p, const Model& m, float x, float y) {
    int pick = 0;
    for (std::size_t i = 0; i < m.palette_result.rows.size() && i < p.rows.size(); ++i) {
        if (m.palette_result.rows[i].heading) continue;
        if (p.rows[i].w > 0 && p.rows[i].contains(x, y)) return pick;
        ++pick;
    }
    return -1;
}

}  // namespace cb::app::window
