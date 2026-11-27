// app/window_pages.cpp -- see window_pages.hpp.
#include "app/window_pages.hpp"

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
        if (f.kind == 5) {   // a drop box: the label, then the box with its Browse button (two, File... and Folder..., when it takes either)
            r.label = {area.x, y, area.w, kFieldLabelHeight};
            r.input = {area.x, y + kFieldLabelHeight + 6, area.w, kDropBoxHeight};
            const float bw = f.accept == 2 ? 96.0f : 112.0f;
            const float by = r.input.y + (kDropBoxHeight - kFieldInputHeight) / 2;
            r.browse = {r.input.right() - 16 - bw, by, bw, kFieldInputHeight};
            if (f.accept == 2) {
                r.browse_folder = r.browse;
                r.browse = {r.browse_folder.x - 8 - bw, by, bw, kFieldInputHeight};
            }
            y += kFieldLabelHeight + 6 + kDropBoxHeight + kFieldGap;
            out.fields.push_back(r);
            continue;
        }
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

float choice_button_width(const std::string& label) { return std::max(96.0f, 7.5f * static_cast<float>(label.size()) + 40.0f); }

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
    float below = t.fields_area.bottom();
    if (!m.tool_choice_label.empty() && !m.tool_choices.empty()) {   // the choice: its label, then a row of buttons
        t.choice_label = {t.fields_area.x, below, t.fields_area.w, kFieldLabelHeight};
        float x = t.fields_area.x;
        for (const std::string& c : m.tool_choices) {
            const float w = choice_button_width(c);
            t.choice_buttons.push_back({x, t.choice_label.bottom() + 6, w, kChoiceButtonHeight});
            x += w + 8;
        }
        below = t.choice_label.bottom() + 6 + kChoiceButtonHeight + kFieldGap;
    }
    t.run = {t.fields_area.x, below + 2, 96, 32};
    float note_x = t.run.right() + 12;
    if (!m.tool_result.empty() && !m.tool_running) {
        t.open_folder = {t.run.right() + 8, t.run.y, 136, 32};
        note_x = t.open_folder.right() + 12;
    }
    t.output_toggle = {t.form_title.right() - kOutputToggleWidth, t.run.y, kOutputToggleWidth, 32};
    t.elapsed = {t.output_toggle.x - 12 - kElapsedWidth, t.run.y, kElapsedWidth, 32};
    t.run_note = {note_x, t.run.y, std::max(0.0f, t.elapsed.x - 8 - note_x), 32};
    t.bar = {t.fields_area.x, t.run.bottom() + 16, t.form_title.w, 3};
    if (m.tool_output_shown)
        t.output = {t.fields_area.x, t.bar.bottom() + 10, t.form_title.w, std::max(0.0f, t.form_card.bottom() - 20 - (t.bar.bottom() + 10))};
    return t;
}

SettingsLayout compute_settings_layout(const Layout& shell, const Model& m) {
    SettingsLayout s;
    const float top = kTitleBarHeight + 8;
    const float left = shell.rail.w > 0 ? shell.rail.w + 4 : kOuterPadding;
    s.card = {left, top, std::max(0.0f, shell.width - kOuterPadding - left), std::max(0.0f, shell.status_bar.y - 16 - top)};
    s.title = {s.card.x + kPgCardPadX, s.card.y + kPgCardPadTop, s.card.w - 2 * kPgCardPadX, kPgTitleHeight};
    if (m.settings_advanced) {   // Back, then the title beside it, and Scan this computer at the right
        s.back = {s.title.x, s.title.y - 2, 32, 32};
        s.title.x += 40;
        s.title.w = std::max(0.0f, s.title.w - 40 - 196);
        s.scan = {s.card.right() - kPgCardPadX - 184, s.card.y + kPgCardPadTop - 2, 184, 32};
    }
    s.desc = {s.title.x, s.title.bottom() + 4, s.title.w, 19};
    const float fields_top = s.desc.bottom() + (m.settings_advanced ? 20.0f : 8.0f);
    s.fields_area = {s.card.x + kPgCardPadX, fields_top, std::min(s.card.w - 2 * kPgCardPadX, kSettingsMaxWidth),
                     std::max(0.0f, s.card.bottom() - 12 - fields_top)};
    const float pad = 16;
    // a setting row: its label and note on the left, its control on the right (the main view's rows; Advanced's too)
    const auto setting_row = [&](const FieldItem& f, const Rect& row) {
        FieldRects r;
        float control_w = 0;
        if (f.kind == 3) {
            control_w = 180;
            r.input = {row.right() - pad - control_w, row.y + 14, control_w, 32};
        } else if (f.kind == 4) {
            control_w = 40;
            r.input = {row.right() - pad - control_w, row.y + 20, control_w, 20};
        } else if (f.kind == 0 || f.kind == 1) {   // Change... (the path is the note under the label)
            control_w = 112;
            r.browse = {row.right() - pad - control_w, row.y + 14, control_w, 32};
        } else {
            control_w = 200;
            r.input = {row.right() - pad - control_w, row.y + 14, control_w, 32};
        }
        const float text_w = std::max(0.0f, row.w - 2 * pad - control_w - 16);
        r.label = {row.x + pad, row.y + 10, text_w, 20};
        r.note = {row.x + pad, row.y + 31, text_w, 17};
        s.form.fields.push_back(r);
        s.field_rows.push_back(row);
    };
    if (m.settings_advanced) {
        const Rect& a = s.fields_area;
        // the menu: the search box, then its entries
        s.nav_search = {a.x, a.y, kAdvancedNavWidth, 32};
        float ny = s.nav_search.bottom() + 8;
        for (const SettingsNavItem& item : m.settings_nav) {
            const float h = item.heading ? 28.0f : 36.0f;
            s.nav_rows.push_back({a.x, ny, kAdvancedNavWidth, h});
            ny += h;
        }
        // the chosen group, at the right of the menu: it scrolls
        s.pane = {a.x + kAdvancedNavWidth + 20, a.y, std::max(0.0f, a.w - kAdvancedNavWidth - 20 - 14), a.h};
        s.fields_area = s.pane;
        const Rect& p = s.pane;
        float y = p.y - m.setting_scroll;
        s.pane_title = {p.x, y, p.w, 22};
        s.pane_note = {p.x, y + 24, p.w, 18};
        y += 24 + 18 + 12;
        const int columns = p.w >= 560 ? 2 : 1;
        const float tile_w = (p.w - 8.0f * static_cast<float>(columns - 1)) / static_cast<float>(columns);
        for (std::size_t i = 0; i < m.tool_tiles.size(); ++i) {
            const int col = static_cast<int>(i) % columns;
            const Rect tile{p.x + static_cast<float>(col) * (tile_w + 8), y, tile_w, kToolTileHeight};
            s.tiles.push_back(tile);
            const ToolTileItem& t = m.tool_tiles[i];
            s.tile_button.push_back(t.state == 0 ? Rect{} : Rect{tile.right() - 14 - 104, tile.bottom() - 12 - 28, 104, 28});
            s.tile_get.push_back(t.state == 3 && !t.download.empty() ? Rect{tile.right() - 14 - 104 - 12 - 92, tile.bottom() - 12 - 28, 92, 28} : Rect{});
            if (col == columns - 1 || i + 1 == m.tool_tiles.size()) y += kToolTileHeight + 8;
        }
        if (!m.tool_attachable.empty()) {
            s.attach_heading = {p.x, y + 10, p.w, 20};
            y += 10 + 20 + 8;
            for (std::size_t i = 0; i < m.tool_attachable.size(); ++i) {
                const Rect row{p.x, y, p.w, kAttachRowHeight};
                s.attach_rows.push_back(row);
                s.attach_buttons.push_back({row.right() - pad - 104, row.y + 12, 104, 28});
                y += kAttachRowHeight + 4;
            }
        }
        if (!m.setting_fields.empty() && (!m.tool_tiles.empty() || !m.tool_attachable.empty())) y += 12;
        for (const FieldItem& f : m.setting_fields) {
            setting_row(f, Rect{p.x, y, p.w, kSettingRowHeight});
            y += kSettingRowHeight + 4;
        }
        if (m.tool_tiles.empty() && m.tool_attachable.empty() && m.setting_fields.empty()) {
            s.pane_empty = {p.x, y, p.w, 20};
            y += 20;
        }
        s.form.content_height = y + m.setting_scroll - p.y;
        return s;
    }

    // the main view: rows
    const Rect& a = s.fields_area;
    float y = a.y - m.setting_scroll;
    const auto heading = [&](Rect& r) {
        r = {a.x, y + 14, a.w, 20};
        y += 14 + 20 + 8;
    };
    heading(s.general_heading);
    for (const FieldItem& f : m.setting_fields) {
        const Rect row{a.x, y, a.w, kSettingRowHeight};
        setting_row(f, row);
        y += kSettingRowHeight + 4;
    }
    heading(s.can_heading);
    for (const CapabilityItem& c : m.capabilities) {
        const Rect row{a.x, y, a.w, kSettingRowHeight};
        s.capability_rows.push_back(row);
        s.locate.push_back(!c.ready && !c.key.empty() ? Rect{row.right() - pad - 112, row.y + 14, 112, 32} : Rect{});
        y += kSettingRowHeight + 4;
    }
    s.advanced_row = {a.x, y, a.w, kSettingRowHeight};
    y += kSettingRowHeight;
    s.form.content_height = y + m.setting_scroll - a.y;
    return s;
}

namespace {

// The part of a form under a point (inside `clip`).
PageHit pg_hit_form(const FormLayout& form, const std::vector<FieldItem>& fields, const Rect& clip, float x, float y) {
    if (!clip.contains(x, y)) return {};
    for (std::size_t i = 0; i < form.fields.size() && i < fields.size(); ++i) {
        const FieldRects& r = form.fields[i];
        if (r.browse.w > 0 && r.browse.contains(x, y)) return {Control::FieldBrowse, static_cast<int>(i)};
        if (r.browse_folder.w > 0 && r.browse_folder.contains(x, y)) return {Control::FieldBrowseFolder, static_cast<int>(i)};
        if (fields[i].kind == 5 && r.input.contains(x, y)) return {Control::FieldBrowse, static_cast<int>(i)};   // (the whole box opens Browse)
        if (r.input.contains(x, y)) return {Control::Field, static_cast<int>(i)};
        if (fields[i].kind == 4 && r.label.contains(x, y)) return {Control::Field, static_cast<int>(i)};   // (the label of a switch is part of it)
    }
    return {};
}

}  // namespace

PageHit hit_test_rail(const Layout& l, float x, float y) {
    if (l.rail_convert.contains(x, y)) return {Control::NavConvert, -1};
    if (l.rail_tools.contains(x, y)) return {Control::NavTools, -1};
    if (l.rail_board.contains(x, y)) return {Control::NavBoard, -1};
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
    if (t.open_folder.w > 0 && t.open_folder.contains(x, y)) return {Control::OpenResult, -1};
    if (t.output_toggle.contains(x, y)) return {Control::ToolOutputToggle, -1};
    for (std::size_t i = 0; i < t.choice_buttons.size(); ++i)
        if (t.choice_buttons[i].contains(x, y)) return {Control::ToolChoice, static_cast<int>(i)};
    if (t.output.w > 0 && t.output.contains(x, y)) return {Control::OutputPane, -1};
    return pg_hit_form(t.form, m.tool_fields, t.form_card, x, y);
}

PageHit hit_test_settings(const SettingsLayout& s, const Model& m, float x, float y) {
    if (m.settings_advanced) {
        if (s.back.contains(x, y)) return {Control::SettingsBack, -1};
        if (s.scan.contains(x, y)) return {Control::SettingsScan, -1};
        if (s.nav_search.contains(x, y)) return {Control::SettingsSearch, -1};
        for (std::size_t i = 0; i < s.nav_rows.size() && i < m.settings_nav.size(); ++i)
            if (!m.settings_nav[i].heading && s.nav_rows[i].contains(x, y)) return {Control::SettingsNav, static_cast<int>(i)};
        if (!s.pane.contains(x, y)) return {};
        for (std::size_t i = 0; i < s.tiles.size(); ++i) {
            if (s.tile_button[i].w > 0 && s.tile_button[i].contains(x, y)) return {Control::ToolTileButton, static_cast<int>(i)};
            if (s.tile_get[i].w > 0 && s.tile_get[i].contains(x, y)) return {Control::ToolTileGet, static_cast<int>(i)};
        }
        for (std::size_t i = 0; i < s.attach_buttons.size(); ++i)
            if (s.attach_buttons[i].contains(x, y)) return {Control::ToolAttach, static_cast<int>(i)};
        return pg_hit_form(s.form, m.setting_fields, s.pane, x, y);
    }
    if (!s.fields_area.contains(x, y)) return {};
    if (const PageHit hit = pg_hit_form(s.form, m.setting_fields, s.fields_area, x, y); hit.control != Control::None) return hit;
    for (std::size_t i = 0; i < s.locate.size(); ++i)
        if (s.locate[i].w > 0 && s.locate[i].contains(x, y)) return {Control::Locate, static_cast<int>(i)};
    if (s.advanced_row.contains(x, y)) return {Control::SettingsAdvanced, -1};
    return {};
}

namespace {

void pg_add_fields(std::vector<PageHit>& order, const std::vector<FieldItem>& fields) {
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].kind != 5) order.push_back({Control::Field, static_cast<int>(i)});   // (a drop box is reached by its buttons)
        if (fields[i].kind == 0 || fields[i].kind == 1 || fields[i].kind == 5) order.push_back({Control::FieldBrowse, static_cast<int>(i)});
        if (fields[i].kind == 5 && fields[i].accept == 2) order.push_back({Control::FieldBrowseFolder, static_cast<int>(i)});
    }
}

void pg_add_rail(std::vector<PageHit>& order) {
    for (const Control c : {Control::NavConvert, Control::NavTools, Control::NavBoard, Control::NavSettings}) order.push_back({c, -1});
}

}  // namespace

std::vector<PageHit> focus_order_tools(const Model& m) {
    std::vector<PageHit> order = {{Control::ToolSearch, -1}};
    if (m.tool_selected >= 0) {
        order.push_back({Control::ToolRow, m.tool_selected});
        pg_add_fields(order, m.tool_fields);
        if (!m.tool_choice_label.empty())
            for (std::size_t i = 0; i < m.tool_choices.size(); ++i) order.push_back({Control::ToolChoice, static_cast<int>(i)});
        order.push_back({Control::Run, -1});
        if (!m.tool_result.empty() && !m.tool_running) order.push_back({Control::OpenResult, -1});
        order.push_back({Control::ToolOutputToggle, -1});
    }
    pg_add_rail(order);
    return order;
}

std::vector<PageHit> focus_order_settings(const Model& m) {
    std::vector<PageHit> order;
    if (m.settings_advanced) {
        order.push_back({Control::SettingsBack, -1});
        order.push_back({Control::SettingsScan, -1});
        order.push_back({Control::SettingsSearch, -1});
        for (std::size_t i = 0; i < m.settings_nav.size(); ++i)
            if (!m.settings_nav[i].heading) order.push_back({Control::SettingsNav, static_cast<int>(i)});
        for (std::size_t i = 0; i < m.tool_tiles.size(); ++i) {
            if (m.tool_tiles[i].state == 3 && !m.tool_tiles[i].download.empty()) order.push_back({Control::ToolTileGet, static_cast<int>(i)});
            if (m.tool_tiles[i].state != 0) order.push_back({Control::ToolTileButton, static_cast<int>(i)});
        }
        for (std::size_t i = 0; i < m.tool_attachable.size(); ++i) order.push_back({Control::ToolAttach, static_cast<int>(i)});
        for (std::size_t i = 0; i < m.setting_fields.size(); ++i) {   // (a path is a row with Change..., as in the main view)
            const bool path = m.setting_fields[i].kind == 0 || m.setting_fields[i].kind == 1;
            order.push_back({path ? Control::FieldBrowse : Control::Field, static_cast<int>(i)});
        }
    } else {   // a path is a row with Change..., not a text box
        for (std::size_t i = 0; i < m.setting_fields.size(); ++i) {
            const bool path = m.setting_fields[i].kind == 0 || m.setting_fields[i].kind == 1;
            order.push_back({path ? Control::FieldBrowse : Control::Field, static_cast<int>(i)});
        }
        for (std::size_t i = 0; i < m.capabilities.size(); ++i)
            if (!m.capabilities[i].ready && !m.capabilities[i].key.empty()) order.push_back({Control::Locate, static_cast<int>(i)});
        order.push_back({Control::SettingsAdvanced, -1});
    }
    pg_add_rail(order);
    return order;
}

}  // namespace cb::app::window
