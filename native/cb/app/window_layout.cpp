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
// the Recent list
constexpr float kRecentGap = 16;
constexpr float kRecentHeadingHeight = 24;
constexpr float kRecentRowHeight = 40;
constexpr float kRecentMinDrop = 180;
constexpr float kRecentRemoveWidth = 76;
// the preview
constexpr float kPreviewListWidth = 220;
constexpr float kPreviewIcon = 44;

Rect centered(float cx, float y, float w, float h) { return Rect{cx - w / 2, y, w, h}; }

}  // namespace

Extras extras_for(const Model& model, View view) {
    Extras e;
    if (view == View::Idle) e.recent_rows = static_cast<int>(std::min<std::size_t>(model.recent.size(), 5));
    e.preview = view == View::Done && !model.preview_files.empty();
    e.queue = view == View::Working && !model.queue.empty();
    return e;
}

Layout compute_layout(View view, float width, float height, float rail, const Extras& extras) {
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
    // One card, the page's whole width: Program until a conversion is done, then Source in its place (the window cross-fades them).
    const float card_w = std::max(0.0f, width - left_edge - kOuterPadding);
    l.left_card = {left_edge, top, card_w, card_h};
    l.right_card = l.left_card;
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
    // the Language drop-down sits in the Program card's header, on the right, level with the title and description
    {
        const float combo_w = std::min(220.0f, std::max(140.0f, l.left_card.w * 0.25f));
        l.language_combo = {l.left_card.right() - kCardPadX - combo_w, l.left_card.y + kCardPadTop + 8, combo_w, 34};
        l.language_label = {l.language_combo.x - 80, l.language_combo.y, 72, l.language_combo.h};
        const float text_w = std::max(0.0f, l.language_label.x - 16 - l.left_title.x);
        l.left_title.w = text_w;
        l.left_desc.w = text_w;
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
    float drop_h = std::max(0.0f, l.left_card.bottom() - kCardPadBottom - drop_top);
    // the Recent list under the drop area (idle): the drop area gives it room, and the list shows fewer rows rather than squeeze the drop area below 180
    if (view == View::Idle && extras.recent_rows > 0) {
        int rows = extras.recent_rows;
        while (rows > 0 && drop_h - (kRecentGap + kRecentHeadingHeight + static_cast<float>(rows) * kRecentRowHeight) < kRecentMinDrop) --rows;
        if (rows > 0) {
            drop_h -= kRecentGap + kRecentHeadingHeight + static_cast<float>(rows) * kRecentRowHeight;
            const float x = l.left_card.x + kCardPadX, w = l.left_card.w - 2 * kCardPadX;
            float y = drop_top + drop_h + kRecentGap;
            l.recent_heading = {x, y, w, kRecentHeadingHeight};
            y += kRecentHeadingHeight;
            for (int i = 0; i < rows; ++i, y += kRecentRowHeight) {
                l.recent_rows.push_back({x, y, w, kRecentRowHeight});
                l.recent_remove.push_back({x + w - 12 - kRecentRemoveWidth, y + (kRecentRowHeight - 28) / 2, kRecentRemoveWidth, 28});
            }
        }
    }
    l.drop = {l.left_card.x + kCardPadX, drop_top, l.left_card.w - 2 * kCardPadX, drop_h};
    const float cx = l.drop.x + l.drop.w / 2;

    // idle: the "+" button and two lines (centred as a group); the "+" itself opens the file picker
    {
        const float group = kPlusSize + 18 + 20 + 4 + 17;
        float y = l.drop.y + (l.drop.h - group) / 2;
        l.plus = centered(cx, y, kPlusSize, kPlusSize);
        y += kPlusSize + 18;
        l.drop_line1 = {l.drop.x + 16, y, l.drop.w - 32, 20};
        y += 20 + 4;
        l.drop_line2 = {l.drop.x + 16, y, l.drop.w - 32, 17};
    }
    // working: the file's icon and name, the bar, its label and percentage, the detail, Cancel (centred as a group in the drop area)
    {
        const float bar_w = std::min(380.0f, std::max(120.0f, l.drop.w - 64));
        const float group = 56 + 14 + 22 + 20 + 4 + 8 + 19 + 6 + 16 + 24 + kButtonHeight;
        float y = l.drop.y + (l.drop.h - group) / 2;
        l.file_icon = centered(cx, y, 56, 56);
        y += 56 + 14;
        l.file_name = {l.drop.x + 16, y, l.drop.w - 32, 22};
        y += 22 + 20;
        l.bar = centered(cx, y, bar_w, 4);
        y += 4 + 8;
        l.bar_label = {cx - bar_w / 2, y, bar_w - 56, 19};
        l.bar_percent = {cx + bar_w / 2 - 56, y, 56, 19};
        y += 19 + 6;
        l.detail = {cx - bar_w / 2, y, bar_w, 16};
        y += 16 + 24;
        l.cancel = centered(cx, y, 96.0f, kButtonHeight);
    }

    // ---- the Source card (Done): the finished file (a check, its name, what was written), then Output: the folder, and Open folder, Copy path,
    // Convert another file -- one column, centred in the card
    // several files: "Cancel all" at the foot of the Program card, the hint beside it
    if (extras.queue) {
        l.cancel = {l.left_card.x + kCardPadX, l.left_card.bottom() - kCardPadBottom - kButtonHeight, 116, kButtonHeight};
        l.hint = {l.cancel.right() + 16, l.cancel.y, std::max(0.0f, l.left_card.right() - kCardPadX - l.cancel.right() - 16), kButtonHeight};
    }
    // the Source card with the preview: the finished file at the top left, the path row (the output box and the three buttons), then the files and the code
    if (view == View::Done && extras.preview) {
        const float x = l.right_card.x + kCardPadX, w = l.right_card.w - 2 * kCardPadX;
        float y = l.right_desc.bottom() + 16;
        l.file_icon = {x, y, kPreviewIcon, kPreviewIcon};
        l.file_name = {x + kPreviewIcon + 12, y + 2, std::max(0.0f, w - kPreviewIcon - 12), 22};
        l.detail = {l.file_name.x, y + 26, l.file_name.w, 18};
        y += kPreviewIcon + 16;
        const float buttons = 124 + 8 + 112 + 8 + 180;
        const float bx = x + std::max(0.0f, w - buttons);
        l.field = {x, y, std::max(120.0f, bx - 12 - x), 34};
        l.open_folder = {bx, y, 124, kButtonHeight};
        l.copy_path = {bx + 124 + 8, y, 112, kButtonHeight};
        l.browse = {bx + 124 + 8 + 112 + 8, y, 180, kButtonHeight};
        y += 34 + 16;
        const float h = std::max(0.0f, l.right_card.bottom() - kCardPadBottom - y);
        l.preview = {x, y, w, h};
        const float list_w = std::min(kPreviewListWidth, w / 3);
        l.preview_list = {x, y, list_w, h};
        l.preview_code = {x + list_w, y, w - list_w, h};
        l.cancel = {};
    } else if (view == View::Done) {
        const float w = std::min(560.0f, std::max(0.0f, l.right_card.w - 2 * kCardPadX));
        const float x = l.right_card.x + (l.right_card.w - w) / 2;
        const float mid = x + w / 2;
        const float group = 56 + 14 + 22 + 4 + 16 + 32 + 19 + 8 + 34 + 6 + 18 + 16 + kButtonHeight;
        const float area_top = l.right_desc.bottom();
        const float area_h = l.right_card.bottom() - kCardPadBottom - area_top;
        float y = area_top + std::max(0.0f, (area_h - group) / 2);
        l.file_icon = centered(mid, y, 56, 56);
        y += 56 + 14;
        l.file_name = {x, y, w, 22};
        y += 22 + 4;
        l.detail = {x, y, w, 16};       // "Conversion finished. 12 files written."
        y += 16 + 32;
        l.output_label = {x, y, w, 19};
        y += 19 + 8;
        l.field = {x, y, w, 34};
        l.hint = {x, y + 34 + 6, w, 18};   // "Drag the folder anywhere ..." under the output box, which is a drag source
        y += 34 + 6 + 18 + 16;
        const float row = 124 + 8 + 112 + 8 + 180;
        const float bx = mid - row / 2;
        l.open_folder = {bx, y, 124, kButtonHeight};
        l.copy_path = {bx + 124 + 8, y, 112, kButtonHeight};
        l.browse = {bx + 124 + 8 + 112 + 8, y, 180, kButtonHeight};   // "Convert another file": back to the Program card
        l.cancel = {};
    }
    return l;
}

bool control_exists(Control control, View view) {
    switch (control) {
        case Control::None: return false;
        case Control::Plus: return view == View::Idle || view == View::Unsupported || view == View::Failed;
        case Control::Browse: return view == View::Done;      // "Convert another file" on the Source card (the Program card's "+" opens the picker)
        case Control::Cancel: return view == View::Working;
        case Control::TryAnother: return view == View::Unsupported || view == View::Failed;
        case Control::OpenFolder:
        case Control::CopyPath: return view == View::Done;          // (the Source card)
        case Control::Language: return view != View::Done;          // (the Program card)
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
        case Control::OutputPane:
        // the Convert page's lists: the window knows which exist (window_convert.hpp)
        case Control::RecentRow:
        case Control::RecentRemove:
        case Control::PreviewFile:
        case Control::CodeView:
        case Control::OutputBox:
        case Control::SettingsAdvanced:   // (the Settings page: window_pages.hpp)
        case Control::SettingsBack:
        case Control::Locate: return false;
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
        case Control::RecentRow:
        case Control::RecentRemove:
        case Control::PreviewFile:
        case Control::CodeView:
        case Control::OutputBox:
        case Control::SettingsAdvanced:
        case Control::SettingsBack:
        case Control::Locate:
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
        case View::Idle: return {Control::Plus, Control::Language};
        case View::Working: return {Control::Cancel};
        case View::Done: return {Control::OpenFolder, Control::CopyPath, Control::Browse};
        case View::Unsupported:
        case View::Failed: return {Control::TryAnother, Control::Plus, Control::Language};
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

// ---- the character stream

void CharStream::push(const std::vector<std::string>& lines) {
    for (const std::string& line : lines) pending_.push_back(line);
    while (pending_.size() > kStreamPendingLines) pending_.pop_front();
    if (!has_current_) start_next();
}

std::size_t CharStream::backlog() const {
    std::size_t chars = has_current_ ? current_.size() - typed_ : 0;
    for (const std::string& line : pending_) chars += line.size() + 1;
    return chars;
}

void CharStream::start_next() {
    has_current_ = !pending_.empty();
    if (!has_current_) return;
    current_ = std::move(pending_.front());
    pending_.pop_front();
    typed_ = 0;
}

void CharStream::advance(double seconds) {
    if (!has_current_ || seconds <= 0) return;
    // a backlog of 1000 characters types about 15 times faster than the steady pace: the stream keeps up with the work
    const double rate = kStreamCharsPerSecond * (1.0 + static_cast<double>(backlog()) / 70.0);
    carry_ += seconds * rate;
    while (has_current_ && carry_ >= 1.0) {
        if (typed_ < current_.size()) {
            const std::size_t step = std::min(current_.size() - typed_, static_cast<std::size_t>(carry_));
            typed_ += step;
            carry_ -= static_cast<double>(step);
        } else {   // the end of a line costs one character, then the next begins
            carry_ -= 1.0;
            shown_.push_back(std::move(current_));
            while (shown_.size() > kStreamKeptLines) shown_.pop_front();
            current_.clear();
            start_next();
        }
    }
    if (!has_current_) carry_ = 0;
}

void CharStream::show_all() {
    while (has_current_) {
        shown_.push_back(std::move(current_));
        current_.clear();
        start_next();
    }
    while (shown_.size() > kStreamKeptLines) shown_.pop_front();
    carry_ = 0;
}

void CharStream::clear() {
    pending_.clear();
    shown_.clear();
    current_.clear();
    typed_ = 0;
    has_current_ = false;
    carry_ = 0;
}

std::vector<std::string> CharStream::visible(std::size_t rows) const {
    std::vector<std::string> out;
    if (rows == 0) return out;
    const std::size_t from_shown = std::min(shown_.size(), has_current_ ? rows - 1 : rows);
    for (std::size_t i = shown_.size() - from_shown; i < shown_.size(); ++i) out.push_back(shown_[i]);
    if (has_current_) out.push_back(current_.substr(0, typed_));
    return out;
}

}  // namespace cb::app::window
