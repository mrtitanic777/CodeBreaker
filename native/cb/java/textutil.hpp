// cb/java/textutil.hpp -- small text/IO helpers shared across the transpiler.
//
// Several transpiler passes keep their own `split_top_commas`, `balanced_paren`, `sub_outside_literals`; they look
// like duplicates but are deliberately context-tuned (one tracks `<>`, another must not because it would
// mis-balance `->`, a third tracks `{}` and backslash escapes) and are NOT interchangeable. Only the genuinely
// identical utilities are centralised here.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::textutil {

// A string or character literal in source text, with backslash escapes ("..." or '...'). Used to mask literals
// before applying substitutions to code. A backslash may not be followed by a newline, and a literal that is
// not closed is not a literal.
struct Span {
    std::size_t begin = 0;
    std::size_t end = 0;   // one past the closing quote
};

// The literal that starts exactly at `pos`, or nullopt when the text there does not start one.
std::optional<Span> literal_at(std::string_view s, std::size_t pos);

// Every literal in the text, scanning left to right; after a literal the scan resumes where it ended.
std::vector<Span> literal_spans(std::string_view s);

// How undecodable bytes are treated when a file is read as UTF-8.
enum class Decode {
    Replace,   // each bad byte becomes U+FFFD
    Strict,    // FormatError
};

// A UTF-8 text file with line endings normalised to "\n". IoError (NotFoundError for a missing file),
// or FormatError under Decode::Strict.
std::string read_text(const std::filesystem::path& path, Decode decode = Decode::Replace);

// The same, but nullopt when the file cannot be read at all (missing, a directory, no access).
std::optional<std::string> try_read_text(const std::filesystem::path& path, Decode decode = Decode::Replace);

// Write a UTF-8 text file (parent folders are created), overwriting. The text is written as given.
void write_text(const std::filesystem::path& path, std::string_view text);

}  // namespace cb::java::textutil
