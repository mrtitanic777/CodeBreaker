// java/decompiler/text_buffer.hpp -- the text the decompiler writes Java into: appended to, indented with the run's indent string, lines ended with the
// run's separator, and (when the original's line numbers are asked for) each written line tied to the original line it came from.
//
//     TextBuffer buf;
//     buf.append_indent(1).append("return x;").append_line_separator();
//
// Ported from Fernflower (de.fernflower.main.TextBuffer, main.collectors.CounterContainer, util.InterpreterUtil's text helpers), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The text is UTF-8 (the original's is UTF-16): offsets into it are the original's
// only within one buffer, which is all the line mapping uses.
#pragma once

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::decompiler {

class TextBuffer {
public:
    TextBuffer();
    explicit TextBuffer(std::string text);

    TextBuffer& append(std::string_view s);
    TextBuffer& append(char c);
    TextBuffer& append(int i);
    TextBuffer& append(const TextBuffer& other);
    TextBuffer& append_line_separator();
    TextBuffer& append_indent(int length);
    TextBuffer& prepend(std::string_view s) { return insert(0, s); }
    TextBuffer& enclose(std::string_view left, std::string_view right);
    TextBuffer& insert(std::size_t offset, std::string_view s);
    bool contains_only_whitespaces() const;
    std::size_t length() const { return text_.size(); }
    std::string substring(std::size_t start) const { return text_.substr(start); }
    TextBuffer& set_start(std::size_t position);
    void set_length(std::size_t position);
    void set_current_line(int line) { set_line_mapping(line, static_cast<int>(text_.size()) + 1); }
    void set_line_mapping(int line, int offset);
    int count_lines(std::size_t from = 0) const { return count(separator_, from); }
    int count(std::string_view substring, std::size_t from) const;
    const std::string& original_text() const { return text_; }
    void dump_original_line_numbers(const std::vector<int>& line_mapping);
    // The text, with the lines moved to the original's line numbers when a mapping was set.
    std::string to_string() const;

private:
    void shift_mapping(int start_offset, int shift_offset);
    std::string add_original_line_numbers() const;
    void append_lines(std::string& res, const std::vector<std::string>& src, int from, int to, int required) const;

    std::string separator_, indent_;
    std::string text_;
    std::optional<std::map<int, int>> line_to_offset_;
    std::optional<std::map<int, int>> line_mapping_;   // new line -> original
};

// `length` copies of the run's indent string.
std::string indent_string(int length);
// "\\u00e9": a character as a Java unicode escape (four lowercase hex digits).
std::string char_to_unicode_literal(int value);
// Whether Java would print the character as itself (not unassigned, a line or paragraph separator, a control, a format, private use or a surrogate).
bool is_printable_unicode(std::uint32_t c);

}  // namespace cb::java::decompiler
