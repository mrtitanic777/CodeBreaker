// cb/java/projects/minecraft/converted_tree.hpp -- reading and rewriting the .cpp files of a converted tree, for the
// text form of the Minecraft fix passes (`codebreaker java <fix> <dir>`: applet_ping, sound_fix, render_fix).
//
// A fix works on text with "\n" line ends. A file that uses CRLF is read with the ends made "\n" and written back
// with CRLF, so a fix never changes a file's line-end style. Bytes that are not valid UTF-8 are kept as they are.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace cb::java::projects::minecraft::converted_tree {

// The names of the *.cpp files directly in `dir`, sorted. A directory that is missing or unreadable has none listed.
std::vector<std::string> cpp_files(const std::filesystem::path& dir);

struct CppText {
    std::string text;   // line ends are "\n"
    bool crlf = false;  // the file had CRLF line ends
};

// The file's text, or nullopt when it cannot be read (a file another process holds open is skipped, not an error).
std::optional<CppText> read_cpp(const std::filesystem::path& file);

// Write the text back in the style it was read in.
void write_cpp(const std::filesystem::path& file, const std::string& text, bool crlf);

}  // namespace cb::java::projects::minecraft::converted_tree
