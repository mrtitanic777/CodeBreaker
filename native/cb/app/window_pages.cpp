// cb/app/window_pages.cpp -- see window_pages.hpp.
#include "cb/app/window_pages.hpp"

#include <algorithm>

namespace cb::app::window {

namespace {

constexpr float kPgCardPadX = 20;
constexpr float kPgCardPadTop = 16;
constexpr float kPgTitleHeight = 27;

bool pg_has_browse(const FieldItem& f) { return f.kind == 0 || f.kind == 1; }

}  // namespace

LanguageList popup_list_layout(const Rect& anchor, float bottom_limit, float top_limit, const std::vector<LanguageItem>& items) {
    LanguageList list;
    float height = 8;   // 4 above and below the rows
    for (const LanguageItem& item : items) height += item.supported || item.reason.empty() ? kLanguageRowHeight : kLanguageRowWithReasonHeight;
    const float room_below = bottom_limit - anchor.bottom() - 4;
    const float room_above = anchor.y - 4 - top_limit;
    float y = anchor.bottom() + 4;
    if (height > room_below && room_above > room_below) {   // no room below: the list opens upwards
        height = std::min(height, room_above);
        y = anchor.y - 4 - height;
    } else {
        height = std::min(height, std::max(0.0f, room_below));
    }
    list.outline = {anchor.x, y, anchor.w, height};
    float row_y = y + 4;
    for (const LanguageItem& item : items) {
        const float h = item.supported || item.reason.empty() ? kLanguageRowHeight : kLanguageRowWithReasonHeight;
        list.rows.push_back({anchor.x + 4, row_y, anchor.w - 8, h});
        row_y += h;
    }
    return list;
}

FormLayout layout_form(const Rect& area, const std::vector<FieldItem>& fields, float scroll) {
    FormLayout out;
    float y = area.y - scroll;
    for (const FieldItem& f : fields) {
        FieldRects r;
        const float note_h = f.note.empty() ? 0.0f : kFieldNoteHeight;
        if (f.kind == 4) {   // a toggle: the switch, then its label (and its note under that)
            r.input = {area.x, y + 4, 40, 20};
            r.label = {area.x + 52, y, std::max(0.0f, area.w - 52), kToggleRowHeight - 4};
            if (note_h > 0) r.note = {area.x + 52, y + kToggleRowHeight - 6, std::max(0.0f, area.w - 52), note_h};
            y += kToggleRowHeight + note_h + kFieldGap;
        } else {
            r.label = {area.x, y, area.w, kFieldLabelHeight};
            if (note_h > 0) r.note = {area.x, y + kFieldLabelHeight, area.w, note_h};
            const float browse_w = pg_has_browse(f) ? 112.0f : 0.0f;
            const float input_w = std::max(0.0f, area.w - (browse_w > 0 ? browse_w + 8 : 0.0f));
            r.input = {area.x, y + kFieldLabelHeight + note_h + 6, input_w, kFieldInputHeight};
            if (browse_w > 0) r.browse = {area.x + input_w + 8, r.input.y, browse_w, kFieldInputHeight};
            y += kFieldLabelHeight + note_h + 6 + kFieldInputHeight + kFieldGap;
        }
        out.fields.push_back(r);
    }
    out.content_height = y + scroll - area.y;
    return out;
}

ToolsLayout compute_tools_layout(const Layout& shell, const Model& m) {
    ToolsLayout t;
    const float top = kTitleBarHeight + 8;
    const float card_h = std::max(0.0f, shell.status_bar.y - 16 - top);
    const float left = shell.rail.w > 0 ? shell.rail.w + 4 : kOuterPadding;
    t.list_card = {left, top, kToolListWidth, card_h};
    t.form_card = {t.list_card.right() + kCardGap, top, std::max(0.0f, shell.width - kOuterPadding - (t.list_card.right() + kCardGap)), card_h};

    // the list
    t.list_title = {t.list_card.x + kPgCardPadX, t.list_card.y + kPgCardPadTop, t.list_card.w - 2 * kPgCardPadX, kPgTitleHeight};
    t.search = {t.list_card.x + kPgCardPadX, t.list_title.bottom() + 12, t.list_card.w - 2 * kPgCardPadX, 32};
    t.list_area = {t.list_card.x + 8, t.search.bottom() + 8, t.list_card.w - 16, std::max(0.0f, t.list_card.bottom() - 12 - (t.search.bottom() + 8))};
    float y = t.list_area.y - m.tool_scroll;
    for (const ToolRowItem& row : m.tool_rows) {
        const float h = row.heading ? kToolHeadingHeight : kToolRowHeight;
        t.rows.push_back({t.list_area.x, y, t.list_area.w, h});
        y += h;
    }
    t.list_content_height = y + m.tool_scroll - t.list_area.y;

    // the selected tool
    t.form_title = {t.form_card.x + kPgCardPadX, t.form_card.y + kPgCardPadTop, t.form_card.w - 2 * kPgCardPadX, kPgTitleHeight};
    t.form_desc = {t.form_title.x, t.form_title.bottom() + 4, t.form_title.w, 40};
    t.fields_area = {t.form_title.x, t.form_desc.bottom() + 8, t.form_title.w, 0};
    t.form = layout_form(t.fields_area, m.tool_fields, 0);
    t.fields_area.h = t.form.content_height;
    t.run = {t.fields_area.x, t.fields_area.bottom() + 2, 96, 32};
    t.run_note = {t.run.right() + 12, t.run.y, std::max(0.0f, t.form_title.right() - t.run.right() - 12), 32};
    t.bar = {t.fields_area.x, t.run.bottom() + 16, t.form_title.w, 3};
    t.output = {t.fields_area.x, t.bar.bottom() + 10, t.form_title.w, std::max(0.0f, t.form_card.bottom() - 20 - (t.bar.bottom() + 10))};
    return t;
}

SettingsLayout compute_settings_layout(const Layout& shell, const Model& m) {
    SettingsLayout s;
    const float top = kTitleBarHeight + 8;
    const float left = shell.rail.w > 0 ? shell.rail.w + 4 : kOuterPadding;
    s.card = {left, top, std::max(0.0f, shell.width - kOuterPadding - left), std::max(0.0f, shell.status_bar.y - 16 - top)};
    s.title = {s.card.x + kPgCardPadX, s.card.y + kPgCardPadTop, s.card.w - 2 * kPgCardPadX, kPgTitleHeight};
    s.desc = {s.title.x, s.title.bottom() + 4, s.title.w, 19};
    const float fields_top = s.desc.bottom() + 16;
    s.fields_area = {s.title.x, fields_top, std::min(s.title.w, 720.0f), std::max(0.0f, s.card.bottom() - 12 - fields_top)};
    s.form = layout_form(s.fields_area, m.setting_fields, m.setting_scroll);
    return s;
}

namespace {

// The part of a form under a point (inside `clip`).
PageHit pg_hit_form(const FormLayout& form, const std::vector<FieldItem>& fields, const Rect& clip, float x, float y) {
    if (!clip.contains(x, y)) return {};
    for (std::size_t i = 0; i < form.fields.size() && i < fields.size(); ++i) {
        const FieldRects& r = form.fields[i];
        if (r.browse.w > 0 && r.browse.contains(x, y)) return {Control::FieldBrowse, static_cast<int>(i)};
        if (r.input.contains(x, y)) return {Control::Field, static_cast<int>(i)};
        if (fields[i].kind == 4 && r.label.contains(x, y)) return {Control::Field, static_cast<int>(i)};   // (the label of a switch is part of it)
    }
    return {};
}

}  // namespace

PageHit hit_test_rail(const Layout& l, float x, float y) {
    if (l.rail_convert.contains(x, y)) return {Control::NavConvert, -1};
    if (l.rail_tools.contains(x, y)) return {Control::NavTools, -1};
    if (l.rail_settings.contains(x, y)) return {Control::NavSettings, -1};
    return {};
}

PageHit hit_test_tools(const ToolsLayout& t, const Model& m, float x, float y) {
    if (t.search.contains(x, y)) return {Control::ToolSearch, -1};
    if (t.list_area.contains(x, y)) {
        for (std::size_t i = 0; i < t.rows.size() && i < m.tool_rows.size(); ++i)
            if (!m.tool_rows[i].heading && t.rows[i].contains(x, y)) return {Control::ToolRow, m.tool_rows[i].tool};
        return {};
    }
    if (m.tool_selected < 0) return {};
    if (t.run.contains(x, y)) return {Control::Run, -1};
    if (t.output.contains(x, y)) return {Control::OutputPane, -1};
    return pg_hit_form(t.form, m.tool_fields, t.form_card, x, y);
}

PageHit hit_test_settings(const SettingsLayout& s, const Model& m, float x, float y) { return pg_hit_form(s.form, m.setting_fields, s.fields_area, x, y); }

namespace {

void pg_add_fields(std::vector<PageHit>& order, const std::vector<FieldItem>& fields) {
    for (std::size_t i = 0; i < fields.size(); ++i) {
        order.push_back({Control::Field, static_cast<int>(i)});
        if (fields[i].kind == 0 || fields[i].kind == 1) order.push_back({Control::FieldBrowse, static_cast<int>(i)});
    }
}

void pg_add_rail(std::vector<PageHit>& order) {
    for (const Control c : {Control::NavConvert, Control::NavTools, Control::NavSettings}) order.push_back({c, -1});
}

}  // namespace

std::vector<PageHit> focus_order_tools(const Model& m) {
    std::vector<PageHit> order = {{Control::ToolSearch, -1}};
    if (m.tool_selected >= 0) {
        order.push_back({Control::ToolRow, m.tool_selected});
        pg_add_fields(order, m.tool_fields);
        order.push_back({Control::Run, -1});
    }
    pg_add_rail(order);
    return order;
}

std::vector<PageHit> focus_order_settings(const Model& m) {
    std::vector<PageHit> order;
    pg_add_fields(order, m.setting_fields);
    pg_add_rail(order);
    return order;
}

}  // namespace cb::app::window
