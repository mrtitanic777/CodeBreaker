// cb/app/window_functions.hpp -- where the parts of the Source card's Functions, Check and Compare views are, what a point is over in them, and the
// words they show; and where the command palette's rows are. The data behind them is Model::functions (function_source.hpp) and palette.hpp. No
// operating-system API: tested on every system (window_functions_test.cpp); the drawing is window_render_win.cpp, the events window_win.cpp.
#pragma once

#include <string>
#include <vector>

#include "cb/app/window_layout.hpp"
#include "cb/app/window_pages.hpp"

namespace cb::app::window {

inline constexpr float kFnRowHeight = 30;
inline constexpr float kPickerRows = 9;     // the function drop-down shows this many items at most

// ---- Functions

struct FunctionsLayout {
    Rect bar;                          // the segmented summary bar
    Rect summary;                      // "38 of 42 behave like the original · 1 differs · 3 not checked"
    Rect filter;                       // "Show: All"
    Rect list_area;                    // the rows are clipped to it
    std::vector<int> shown;            // the functions (indexes of Model::functions) the filter lets through
    std::vector<Rect> rows;            // one per `shown`, moved up by Model::fn_scroll
    float content_height = 0;
};
FunctionsLayout compute_functions_layout(const Layout& shell, const Model& model);
float functions_max_scroll(const FunctionsLayout& layout);

// The parts of one row: the code glyph, the name, the address (monospace) and the verdict chip.
struct FnRowParts {
    Rect icon, name, address, chip;
};
FnRowParts fn_row_parts(const Rect& row);

// "38 of 42 behave like the original · 1 differs · 3 not checked"; "42 functions · none checked by running them" when nothing was judged; "No functions
// were found in the output folder." when there are none.
std::string functions_summary(const fnsrc::Counts& counts);
// "All", "Differs", "Not checked".
std::string filter_name(int filter);
// "Behaves like the original", "Differs", "Not checked".
std::string verdict_name(convert::FunctionVerdict verdict);
// The sentence under the list when the filter hides everything ("No function differs." ...).
std::string empty_filter_text(int filter);

// ---- Check

struct CheckLayout {
    bool judged = false;               // some function was judged: the ring; else the plain explanation
    Rect ring;
    Rect title, body;
    Rect dot[3], label[3], count[3];   // Behaves like the original, Differs, Not checked
    Rect show;                         // "Show what differs" (empty when nothing differs)
    Rect again;                        // "Check again"
    Rect note;                         // under the buttons, when Check again is disabled: why
    Rect mark;                         // the grey mark above the explanation (not judged)
};
CheckLayout compute_check_layout(const Layout& shell, const Model& model);
// The sentence about Check again when it cannot run.
inline constexpr const char* kRecheckUnavailable = "This kind of file cannot be checked again here yet.";

// ---- Compare

struct CompareLayout {
    Rect all, picker, chip, hint;
    Rect asm_pane, asm_head, asm_code, c_pane, c_head, c_code;
    int visible_lines = 0;
};
CompareLayout compute_compare_layout(const Layout& shell, const Model& model);
// The first visible line's largest value for a side (0 asm, 1 C).
int compare_max_first_line(const Model& model, const CompareLayout& layout, int side);
// The line of a side at a y inside its code area; -1 outside the lines it has.
int compare_line_at(const Model& model, const CompareLayout& layout, int side, float y);
// Whether the line is lit: it is in the link of the clicked line, or it is the clicked line.
bool compare_line_lit(const Model& model, int side, int line);

// The function drop-down of Compare: the outline and the rows of the items from Model::cmp_picker_top.
struct PickerLayout {
    Rect outline;
    std::vector<Rect> rows;
    int first = 0;                     // the function (index of Model::functions) of the first row
};
PickerLayout compute_picker_layout(const Layout& shell, const Model& model);
int picker_item_at(const PickerLayout& layout, float x, float y);   // an index of Model::functions; -1 none

// ---- the Source card's tabs

// What a point is over among the tabs and the view of the shown tab (not the Files tab's own list: window_convert.hpp).
PageHit hit_test_source_tabs(const Layout& shell, const Model& model, float x, float y);
// The tabs, then the controls of the view, in the order Tab visits.
std::vector<PageHit> focus_order_source_tabs(const Layout& shell, const Model& model);

// ---- the command palette

inline constexpr float kPaletteWidth = 620;
inline constexpr float kPaletteInputHeight = 48;
inline constexpr float kPaletteHeadingHeight = 28;
inline constexpr float kPaletteRowHeight = 44;

struct PaletteLayout {
    Rect box, input, close_hint;
    std::vector<Rect> rows;            // one per Model::palette_result.rows
};
PaletteLayout compute_palette_layout(const Layout& shell, const Model& model);
// The choosable position (palette_result.picks) under a point; -1 none.
int palette_pick_at(const PaletteLayout& layout, const Model& model, float x, float y);

}  // namespace cb::app::window
