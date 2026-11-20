// app/window_pages.hpp -- where everything of the Tools and Settings pages is (the design is work/ui-mockup/win11-tools.html): the layout of the two
// pages in device-independent pixels, the form layout they share, and which part a point is over. No operating-system API here (tested on every system:
// window_pages_test.cpp); the drawing is window_render_win.cpp and the events are window_win.cpp.
//
// The Tools page is two cards: the list (a title, a search box, every tool in groups, one plain-word row each) and the selected tool (its title, one
// sentence, a drop box per input (or a module's search), the choice as a row of buttons, Run and Open folder, a thin progress bar and the
// output pane: nothing on it is typed but a search). The Settings page is one card holding a form. A form field is a
// FieldItem of the Model: a label and a text box (with Browse for a file or folder), a drop-down for a choice, a switch for a toggle.
#pragma once

#include <vector>

#include "app/window_layout.hpp"

namespace cb::app::window {

// One field's parts. A toggle has its switch in `input` and its label beside it; Browse is empty unless the field is a file or a folder.
struct FieldRects {
    Rect label, note, input, browse;   // note: under the label, empty when the field has none
    Rect browse_folder;                // a drop box that takes a file or a folder: its second button (Folder...)
};

struct FormLayout {
    std::vector<FieldRects> fields;
    float content_height = 0;          // all the fields, for scrolling
};

inline constexpr float kFieldLabelHeight = 19;
inline constexpr float kFieldInputHeight = 32;
inline constexpr float kFieldGap = 14;
inline constexpr float kFieldNoteHeight = 16;
inline constexpr float kToggleRowHeight = 28;
inline constexpr float kToolRowHeight = 34;
inline constexpr float kToolHeadingHeight = 32;
inline constexpr float kToolListWidth = 340;
inline constexpr float kDropBoxHeight = 84;
inline constexpr float kChoiceButtonHeight = 32;

// The fields stacked in `area`, the first at the top moved up by `scroll`.
FormLayout layout_form(const Rect& area, const std::vector<FieldItem>& fields, float scroll);

struct ToolsLayout {
    Rect list_card, list_title, search, list_area;
    std::vector<Rect> rows;            // one per Model::tool_rows (the ones scrolled out of list_area are outside it)
    float list_content_height = 0;
    Rect form_card, form_title, form_desc, fields_area;
    FormLayout form;
    Rect choice_label;                 // the choice's label, then one button per choice (empty when the tool has none)
    std::vector<Rect> choice_buttons;
    Rect run, open_folder, run_note, bar, output;   // open_folder: empty until a run has left a result folder
};
// A choice button's width for its words (the layout knows no fonts: about 7.5 DIPs a character at 14 px, and room around them).
float choice_button_width(const std::string& label);
ToolsLayout compute_tools_layout(const Layout& shell, const Model& model);

// The Settings page has two views. The main one is rows (work/ui-mockup/feat-settings-*.png): "General" (Theme, where converted source goes,
// Keep the computer responsive: each a row with its label on the left and its control on the right), "What this computer can convert" (one row
// per Model::capabilities, Locate... on a missing one) and the Advanced row. Advanced is Back, a title, and every other setting as a form.
struct SettingsLayout {
    Rect card, title, desc, fields_area;
    FormLayout form;                   // the main view: a row per field (field_rows), the control on its right; Advanced: the stacked form
    Rect back;                         // Advanced only
    Rect general_heading, can_heading; // the main view only
    std::vector<Rect> field_rows;      // the main view: the row behind each field
    std::vector<Rect> capability_rows, locate;   // one per Model::capabilities; locate is empty for a ready row
    Rect advanced_row;
};
inline constexpr float kSettingRowHeight = 60;
inline constexpr float kSettingsMaxWidth = 760;
SettingsLayout compute_settings_layout(const Layout& shell, const Model& model);

// A part of a page under a point: a control and which one (tool row, field).
struct PageHit {
    Control control = Control::None;
    int index = -1;
    friend bool operator==(const PageHit&, const PageHit&) = default;
};
PageHit hit_test_tools(const ToolsLayout& layout, const Model& model, float x, float y);
PageHit hit_test_settings(const SettingsLayout& layout, const Model& model, float x, float y);
// The rail (any page).
PageHit hit_test_rail(const Layout& layout, float x, float y);

// What Tab visits, in order, on each page (the rail's three items come last).
std::vector<PageHit> focus_order_tools(const Model& model);
std::vector<PageHit> focus_order_settings(const Model& model);

// A drop-down list under (or over) any anchor: the same shape as the Language list (window_layout.hpp), for a form's choices.
LanguageList popup_list_layout(const Rect& anchor, float bottom_limit, float top_limit, const std::vector<LanguageItem>& items);

}  // namespace cb::app::window
