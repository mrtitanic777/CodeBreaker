// cb/app/window_layout.cpp -- see window_layout.hpp.
#include "cb/app/window_layout.hpp"
#include "cb/app/window_pages.hpp"

#include <algorithm>

namespace cb::app::window {

namespace {

// Metrics of the approved design (win11.html), in DIPs.
constexpr float kCardPadX = 24;
constexpr float kCardPadTop = 20;
constexpr float kCardTitleHeight = 27;      // 20 px semibold, Segoe UI Variable Display
constexpr float kCardDescGap = 4;
constexpr float kCardDescHeight = 19;       // 14 px
constexpr float kDropTop = 20;              // below the description
constexpr float kCardPadBottom = 20;
constexpr float kPlusSize = 72;
constexpr float kButtonHeight = 32;
constexpr float kInfoBarHeight = 132;

Rect centered(float cx, float y, float w, float h) { return Rect{cx - w / 2, y, w, h}; }

}  // namespace

Layout compute_layout(View view, float width, float height, float rail) {
    Layout l;
    l.width = width;
    l.height = height;

    // ---- title bar
    l.title_bar = {0, 0, width, kTitleBarHeight};
    l.app_icon = {16, (kTitleBarHeight - 16) / 2, 16, 16};
    l.caption_close = {width - kCaptionButtonWidth, 0, kCaptionButtonWidth, kTitleBarHeight};
    l.caption_maximize = {width - 2 * kCaptionButtonWidth, 0, kCaptionButtonWidth, kTitleBarHeight};
    l.caption_minimize = {width - 3 * kCaptionButtonWidth, 0, kCaptionButtonWidth, kTitleBarHeight};
    l.title_text = {44, 0, std::max(0.0f, l.caption_minimize.x - 44), kTitleBarHeight};

    // ---- status bar
    l.status_bar = {0, height - kStatusBarHeight, width, kStatusBarHeight};
    l.status_dot = {kOuterPadding, l.status_bar.y + (kStatusBarHeight - 8) / 2, 8, 8};
    const float half = (width - 2 * kOuterPadding) / 2;
    l.status_text = {kOuterPadding + 14, l.status_bar.y, half - 14, kStatusBarHeight};
    l.platforms = {width - kOuterPadding - half, l.status_bar.y, half, kStatusBarHeight};

    // ---- the cards: 8 below the title bar, a 16 gap, 24 on the sides, 16 above the status bar
    const float top = kTitleBarHeight + 8;
    const float card_h = std::max(0.0f, height - kStatusBarHeight - 16 - top);
    const float left_edge = rail > 0 ? rail + 4 : kOuterPadding;   // (the design's page starts 4 after the rail)
    const float card_w = std::max(0.0f, (width - left_edge - kOuterPadding - kCardGap) / 2);
    l.left_card = {left_edge, top, card_w, card_h};
    l.right_card = {left_edge + card_w + kCardGap, top, card_w, card_h};
    if (rail > 0) {   // the rail: three items of 48 x 56, Convert and Tools at the top, Settings at the bottom
        l.rail = {0, kTitleBarHeight, rail, std::max(0.0f, l.status_bar.y - kTitleBarHeight)};
        l.rail_convert = {8, kTitleBarHeight + 4, 48, 56};
        l.rail_tools = {8, kTitleBarHeight + 4 + 60, 48, 56};
        l.rail_settings = {8, l.status_bar.y - 56 - 8, 48, 56};
    }
    for (const Rect* card : {&l.left_card, &l.right_card}) {
        Rect& title = card == &l.left_card ? l.left_title : l.right_title;
        Rect& desc = card == &l.left_card ? l.left_desc : l.right_desc;
        title = {card->x + kCardPadX, card->y + kCardPadTop, card->w - 2 * kCardPadX, kCardTitleHeight};
        desc = {card->x + kCardPadX, title.bottom() + kCardDescGap, card->w - 2 * kCardPadX, kCardDescHeight};
    }

    // ---- the left card: the InfoBar (when there is one), then the drop area to the bottom
    const bool has_info = view == View::Unsupported || view == View::Failed;
    float drop_top = l.left_desc.bottom() + kDropTop;
    if (has_info) {
        l.info_bar = {l.left_card.x + kCardPadX, drop_top, l.left_card.w - 2 * kCardPadX, kInfoBarHeight};
        l.info_icon = {l.info_bar.x + 16, l.info_bar.y + 14, 16, 16};
        l.info_title = {l.info_bar.x + 44, l.info_bar.y + 12, l.info_bar.w - 44 - 16, 20};
        l.info_message = {l.info_bar.x + 44, l.info_bar.y + 36, l.info_bar.w - 44 - 16, 57};
        l.try_another = {l.info_bar.right() - 16 - 140, l.info_bar.bottom() - 12 - kButtonHeight, 140, kButtonHeight};
        drop_top = l.info_bar.bottom() + 16;
    }
    l.drop = {l.left_card.x + kCardPadX, drop_top, l.left_card.w - 2 * kCardPadX, std::max(0.0f, l.left_card.bottom() - kCardPadBottom - drop_top)};
    const float cx = l.drop.x + l.drop.w / 2;

    // idle: the "+" button, two lines, Browse (centred as a group)
    {
        const float group = kPlusSize + 18 + 20 + 4 + 17 + 18 + kButtonHeight;
        float y = l.drop.y + (l.drop.h - group) / 2;
        l.plus = centered(cx, y, kPlusSize, kPlusSize);
        y += kPlusSize + 18;
        l.drop_line1 = {l.drop.x + 16, y, l.drop.w - 32, 20};
        y += 20 + 4;
        l.drop_line2 = {l.drop.x + 16, y, l.drop.w - 32, 17};
        y += 17 + 18;
        l.browse = centered(cx, y, 112, kButtonHeight);
    }
    // working / done: the file's icon and name, the bar, its label and percentage, the detail, Cancel (centred as a group)
    {
        const bool done = view == View::Done;
        const float bar_w = std::min(380.0f, std::max(120.0f, l.drop.w - 64));
        const float group = 56 + 14 + 22 + (done ? 16 : 20 + 4 + 8 + 19 + 6 + 16) + 24 + kButtonHeight;
        float y = l.drop.y + (l.drop.h - group) / 2;
        l.file_icon = centered(cx, y, 56, 56);
        y += 56 + 14;
        l.file_name = {l.drop.x + 16, y, l.drop.w - 32, 22};
        y += 22;
        if (!done) {
            y += 20;
            l.bar = centered(cx, y, bar_w, 4);
            y += 4 + 8;
            l.bar_label = {cx - bar_w / 2, y, bar_w - 56, 19};
            l.bar_percent = {cx + bar_w / 2 - 56, y, 56, 19};
            y += 19 + 6;
            l.detail = {cx - bar_w / 2, y, bar_w, 16};
            y += 16;
        } else {
            l.detail = {l.drop.x + 16, y, l.drop.w - 32, 16};
            y += 16;
        }
        y += 24;
        l.cancel = centered(cx, y, done ? 180.0f : 96.0f, kButtonHeight);
        if (done) l.browse = l.cancel;   // the Done view's "Convert another file" button takes Browse's place
    }

    // ---- the right card: Output label, the field, two buttons, the hint -- centred as a group in what is below the description
    {
        // Language (label, drop-down), then Output (label, field, two buttons, the hint of up to two lines)
        const float group = 19 + 8 + 34 + 20 + 19 + 8 + 34 + 12 + kButtonHeight + 14 + 34;
        const float area_top = l.right_desc.bottom();
        const float area_h = l.right_card.bottom() - kCardPadBottom - area_top;
        float y = area_top + std::max(0.0f, (area_h - group) / 2);
        const float x = l.right_card.x + kCardPadX;
        const float w = l.right_card.w - 2 * kCardPadX;
        l.language_label = {x, y, w, 19};
        y += 19 + 8;
        l.language_combo = {x, y, w, 34};
        y += 34 + 20;
        l.output_label = {x, y, w, 19};
        y += 19 + 8;
        l.field = {x, y, w, 34};
        y += 34 + 12;
        l.open_folder = {x, y, 124, kButtonHeight};
        l.copy_path = {x + 124 + 8, y, 112, kButtonHeight};
        y += kButtonHeight + 14;
        l.hint = {x, y, w, 34};
    }
    return l;
}

bool control_exists(Control control, View view) {
    switch (control) {
        case Control::None: return false;
        case Control::Plus: return view == View::Idle || view == View::Unsupported || view == View::Failed;
        case Control::Browse: return view != View::Working;   // (in the Done view it is "Convert another file")
        case Control::Cancel: return view == View::Working;
        case Control::TryAnother: return view == View::Unsupported || view == View::Failed;
        case Control::OpenFolder:
        case Control::CopyPath:
        case Control::Language: return true;
        case Control::CaptionMinimize:
        case Control::CaptionMaximize:
        case Control::CaptionClose: return true;
        // the rail and the pages after the first: the window knows which page it is on (window_pages.hpp)
        case Control::NavConvert:
        case Control::NavTools:
        case Control::NavSettings: return true;
        case Control::ToolSearch:
        case Control::ToolRow:
        case Control::Field:
        case Control::FieldBrowse:
        case Control::Run:
        case Control::OutputPane: return false;
    }
    return false;
}

bool control_enabled(Control control, View view) {
    if (!control_exists(control, view)) return false;
    if (control == Control::OpenFolder || control == Control::CopyPath) return view == View::Done;
    if (control == Control::Language) return view != View::Working;   // (the language of a run under way cannot change)
    return true;
}

Rect control_rect(const Layout& l, Control control) {
    switch (control) {
        case Control::Plus: return l.plus;
        case Control::Browse: return l.browse;
        case Control::Cancel: return l.cancel;   // (in the Done view it is the "Convert another file" button)
        case Control::TryAnother: return l.try_another;
        case Control::OpenFolder: return l.open_folder;
        case Control::CopyPath: return l.copy_path;
        case Control::Language: return l.language_combo;
        case Control::CaptionMinimize: return l.caption_minimize;
        case Control::CaptionMaximize: return l.caption_maximize;
        case Control::CaptionClose: return l.caption_close;
        case Control::NavConvert: return l.rail_convert;
        case Control::NavTools: return l.rail_tools;
        case Control::NavSettings: return l.rail_settings;
        case Control::ToolSearch:
        case Control::ToolRow:
        case Control::Field:
        case Control::FieldBrowse:
        case Control::Run:
        case Control::OutputPane:
        case Control::None: break;
    }
    return {};
}

Control hit_test(const Layout& l, View view, float x, float y) {
    for (const Control c : {Control::CaptionClose, Control::CaptionMaximize, Control::CaptionMinimize, Control::TryAnother, Control::Cancel,
                            Control::Browse, Control::OpenFolder, Control::CopyPath, Control::Language, Control::NavConvert, Control::NavTools, Control::NavSettings}) {
        if (control_exists(c, view) && control_rect(l, c).contains(x, y)) return c;
    }
    // the round "+" is a button
    if (control_exists(Control::Plus, view)) {
        const float r = l.plus.w / 2, dx = x - (l.plus.x + r), dy = y - (l.plus.y + r);
        if (dx * dx + dy * dy <= r * r) return Control::Plus;
    }
    return Control::None;
}

std::vector<Control> focus_order(View view) {
    switch (view) {
        case View::Idle: return {Control::Plus, Control::Browse, Control::Language};
        case View::Working: return {Control::Cancel};
        case View::Done: return {Control::Browse, Control::Language, Control::OpenFolder, Control::CopyPath};
        case View::Unsupported:
        case View::Failed: return {Control::TryAnother, Control::Plus, Control::Browse, Control::Language};
    }
    return {};
}

LanguageList language_list_layout(const Layout& l, const std::vector<LanguageItem>& items) {
    return popup_list_layout(l.language_combo, l.status_bar.y - 8, kTitleBarHeight + 8, items);
}

int language_item_at(const LanguageList& list, float x, float y) {
    for (std::size_t i = 0; i < list.rows.size(); ++i)
        if (list.rows[i].contains(x, y)) return static_cast<int>(i);
    return -1;
}

int language_next_enabled(const std::vector<LanguageItem>& items, int from, int direction) {
    const int count = static_cast<int>(items.size());
    for (int i = from + direction; i >= 0 && i < count; i += direction)
        if (items[static_cast<std::size_t>(i)].supported) return i;
    if (from >= 0 && from < count && items[static_cast<std::size_t>(from)].supported) return from;
    for (int i = 0; i < count; ++i)
        if (items[static_cast<std::size_t>(i)].supported) return i;
    return -1;
}

}  // namespace cb::app::window
