// app/notify.hpp -- what the window says when a conversion (or a batch) finishes while it is not the window the person is looking at: the words of the
// notification and the toast's XML. No operating-system API here (tested on every system); showing it is window_shell_win.cpp.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cb::app::notify {

enum class Outcome { Done, CannotConvert, Failed };

// How one file ended.
struct Result {
    std::string name;                  // "setup.exe"
    Outcome outcome = Outcome::Done;
    std::uint64_t files = 0;           // Done: files written
    std::string folder;                // Done: the output folder's name ("setup")
    std::string detail;                // otherwise: what the person is told ("This looks like a PNG image.")
};

struct Message {
    std::string title;                 // "setup.exe is converted"
    std::string body;                  // "12 files written to Converted\setup"
    bool open_folder = false;          // there is a folder worth opening: the toast has an "Open folder" button
};

// The words for the files of one run (one file, or a batch). Empty results give an empty message.
Message compose(const std::vector<Result>& results);

// "&", "<", ">", quotes escaped for XML text and attributes.
std::string xml_escape(const std::string& text);

// The toast: a title and a body, buttons "Open folder" (when the message has one) and "Show". A button's `arguments` are "open" and "show" (a click on
// the toast itself is "show").
std::string toast_xml(const Message& message);

}  // namespace cb::app::notify
