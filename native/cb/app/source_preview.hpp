// cb/app/source_preview.hpp -- what the Source card's preview shows: the files an output folder holds, in the order a person wants them (the main
// source first), one file's first kilobytes as lines, and simple syntax colouring of those lines. No operating-system API: tested on every system.
//
//     std::vector<FileEntry> files = list_files(folder, "lz32");          // lz32.c, lz32.asm, lz32.inc, ... build.cmd
//     Preview text = read_preview(folder / files[0].path);                // the first kPreviewBytes as lines; `truncated` says there is more
//     auto spans = highlight(lang_for(files[0].path), text.lines);        // one list of coloured spans per line
//
// Colouring is deliberately small: keywords, comments, numbers and strings for C, C++, C# and assembly; any other file is plain.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace cb::app::preview {

// How much of a file the preview reads, and the most files it lists.
inline constexpr std::size_t kPreviewBytes = 128 * 1024;
inline constexpr std::size_t kMaxListedFiles = 400;

struct FileEntry {
    std::string path;                  // relative to the folder, '/' separators
    std::uint64_t size = 0;
};

// The files of `folder` (subfolders included), the main source first: the files named `main_stem` with a source extension, in the order .c, .cpp,
// .cs, .java, .py, .asm; then every other file by name (sources and headers before build scripts and notes). `main_stem` may be empty.
std::vector<FileEntry> list_files(const std::filesystem::path& folder, std::string_view main_stem);

// What the output folder's main source is called for a person: "C", "C++", "C#", "Java", "Python" or "Assembly" from the first file's extension; "Source"
// when it is none of them or the list is empty.
std::string chip_for_files(const std::vector<FileEntry>& files);

// A build script or note rather than source (.cmd, .bat, .sh, .txt, .md, .json, Makefile ...): the preview gives it another icon.
bool is_script_file(std::string_view path);

enum class Lang { Plain, C, Cpp, CSharp, Asm };
// By file extension (".c", ".h" C; ".cpp" ".hpp" ".cc" C++; ".cs" C#; ".asm" ".s" ".inc" assembly); anything else Plain.
Lang lang_for(std::string_view file_name);

enum class Tok : std::uint8_t { Plain, Keyword, Comment, Number, String };
struct Span {
    std::uint32_t start = 0;           // byte offset in the line
    std::uint32_t length = 0;
    Tok kind = Tok::Plain;
    friend bool operator==(const Span&, const Span&) = default;
};
// The coloured spans of each line (spans cover only the coloured runs; the gaps are Plain). A block comment that is open at the end of a line
// carries on in the next.
std::vector<std::vector<Span>> highlight(Lang lang, const std::vector<std::string>& lines);

struct Preview {
    std::vector<std::string> lines;    // tabs become four spaces; no line ends
    bool truncated = false;            // the file is longer than kPreviewBytes
    bool binary = false;               // a NUL byte in the first 4 KB: nothing is shown but the note
    std::uint64_t total_bytes = 0;
    std::string note;                  // "Showing the first 128 KB of 3.4 MB." / "Binary file, 12 KB."; empty when the whole file is shown
};
// The first kPreviewBytes of a file as lines. An unreadable file gives an empty Preview whose note says so.
Preview read_preview(const std::filesystem::path& file);
// The same from bytes already in memory (what read_preview uses; for tests).
Preview make_preview(std::string_view bytes, std::uint64_t total_bytes);

// "12 KB", "3.4 MB", "812 bytes".
std::string size_words(std::uint64_t bytes);

}  // namespace cb::app::preview
