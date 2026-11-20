// app/window_convert.hpp -- where the lists of the Convert page are: the rows of the batch queue, the file list and code view of the Source card's
// preview, what a point is over on the page, and what Tab visits. (The Program card's Recent rows and the preview's frame are in window_layout.hpp's
// Layout, because they depend only on the window's size.) No operating-system API: tested on every system (window_convert_test.cpp); the drawing is
// window_render_win.cpp and the events are window_win.cpp.
#pragma once

#include <vector>

#include "app/window_layout.hpp"
#include "app/window_pages.hpp"

namespace cb::app::window {

inline constexpr float kQueueRowHeight = 44;
inline constexpr float kQueueRowGap = 6;
inline constexpr float kFileRowHeight = 30;
inline constexpr float kCodeLineHeight = 20;
inline constexpr float kCodeFontSize = 12.5f;
inline constexpr float kCodeCharWidth = 7.5f;     // a monospace face advances 0.6 em
inline constexpr float kCodePad = 16;

// ---- the batch queue (Working, several files)

struct QueueRowRects {
    Rect row, icon, text, bar, status;
};

struct QueueLayout {
    Rect area;                         // where the rows show (they are clipped to it)
    std::vector<QueueRowRects> rows;   // one per Model::queue, moved up by Model::queue_scroll
    float content_height = 0;
};
QueueLayout compute_queue_layout(const Layout& shell, const Model& model);
// The most Model::queue_scroll can be.
float queue_max_scroll(const QueueLayout& layout);

// ---- the preview (Done)

struct PreviewLayout {
    Rect list_area;                    // the file list, inside the frame
    std::vector<Rect> file_rows;       // one per Model::preview_files, moved up by Model::preview_list_scroll
    float list_content_height = 0;
    Rect code_area;                    // where the lines are drawn (clipped to it)
    Rect note_area;                    // the note under the code ("Showing the first 128 KB ..."); empty when there is none
    int visible_lines = 0;
    int visible_columns = 0;
};
PreviewLayout compute_preview_layout(const Layout& shell, const Model& model);
// The most the code can be scrolled: the first visible line, and columns to the left.
int preview_max_first_line(const Model& model, const PreviewLayout& layout);
int preview_max_hscroll(const Model& model, const PreviewLayout& layout);
float preview_list_max_scroll(const PreviewLayout& layout);

// ---- the page

// What a point is over on the Convert page: everything hit_test finds, and also a Recent row (index) or its Remove, a file of the preview (index), the code
// view and the output box.
PageHit hit_test_convert(const Layout& shell, const Model& model, float x, float y);
// What Tab visits on the Convert page, in order (the rail's items come after).
std::vector<PageHit> focus_order_convert(const Layout& shell, const Model& model);

}  // namespace cb::app::window
