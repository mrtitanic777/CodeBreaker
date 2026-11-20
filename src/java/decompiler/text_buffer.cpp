// java/decompiler/text_buffer.cpp -- see text_buffer.hpp. Ported from Fernflower (main.TextBuffer, util.InterpreterUtil), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0.
#include "java/decompiler/text_buffer.hpp"

#include <algorithm>
#include <cstdint>
#include <format>

#include "java/decompiler/context.hpp"

namespace cb::java::decompiler {

namespace {

#include "java/decompiler/printable_ranges.inc"

// Java's String.split(separator): the pieces between separators, trailing empty ones dropped.
std::vector<std::string> tb_split(const std::string& s, const std::string& sep) {
    std::vector<std::string> out;
    std::size_t from = 0;
    for (std::size_t at; (at = s.find(sep, from)) != std::string::npos; from = at + sep.size()) out.push_back(s.substr(from, at - from));
    out.push_back(s.substr(from));
    while (!out.empty() && out.back().empty()) out.pop_back();
    if (out.empty()) out.push_back(std::string());   // (an empty string splits into one empty piece)
    return out;
}

std::string tb_trim(const std::string& s) {   // Java's String.trim: every char <= ' ' off both ends
    std::size_t b = 0, e = s.size();
    while (b < e && static_cast<unsigned char>(s[b]) <= ' ') ++b;
    while (e > b && static_cast<unsigned char>(s[e - 1]) <= ' ') --e;
    return s.substr(b, e - b);
}

std::vector<std::string> tb_compact_lines(std::vector<std::string> res, int required) {
    if (res.size() < 2 || static_cast<int>(res.size()) <= required) return res;
    for (std::size_t i = res.size() - 1; i > 0; --i) {   // lines holding only { or } join the line before
        const std::string t = tb_trim(res[i]);
        if (t == "{" || t == "}") {
            res[i - 1] += res[i];
            res.erase(res.begin() + static_cast<std::ptrdiff_t>(i));
        }
        if (static_cast<int>(res.size()) <= required) return res;
    }
    for (std::size_t i = res.size() - 1; i > 0; --i) {   // then the empty lines
        if (tb_trim(res[i]).empty()) {
            res[i - 1] += res[i];
            res.erase(res.begin() + static_cast<std::ptrdiff_t>(i));
        }
        if (static_cast<int>(res.size()) <= required) return res;
    }
    return res;
}

}  // namespace

TextBuffer::TextBuffer() : separator_(DecompilerContext::new_line_separator()), indent_(DecompilerContext::property(pref::INDENT_STRING)) {}

TextBuffer::TextBuffer(std::string text) : TextBuffer() { text_ = std::move(text); }

TextBuffer& TextBuffer::append(std::string_view s) {
    text_ += s;
    return *this;
}

TextBuffer& TextBuffer::append(char c) {
    text_ += c;
    return *this;
}

TextBuffer& TextBuffer::append(int i) {
    text_ += std::to_string(i);
    return *this;
}

TextBuffer& TextBuffer::append(const TextBuffer& other) {
    if (other.line_to_offset_ && !other.line_to_offset_->empty()) {
        if (!line_to_offset_) line_to_offset_.emplace();
        for (const auto& [line, offset] : *other.line_to_offset_) (*line_to_offset_)[line] = offset + static_cast<int>(text_.size());
    }
    text_ += other.text_;
    return *this;
}

TextBuffer& TextBuffer::append_line_separator() { return append(separator_); }

TextBuffer& TextBuffer::append_indent(int length) {
    while (length-- > 0) text_ += indent_;
    return *this;
}

TextBuffer& TextBuffer::enclose(std::string_view left, std::string_view right) {
    prepend(left);
    return append(right);
}

TextBuffer& TextBuffer::insert(std::size_t offset, std::string_view s) {
    text_.insert(offset, s);
    shift_mapping(static_cast<int>(offset), static_cast<int>(s.size()));
    return *this;
}

bool TextBuffer::contains_only_whitespaces() const {
    return std::all_of(text_.begin(), text_.end(), [](char c) { return c == ' '; });
}

TextBuffer& TextBuffer::set_start(std::size_t position) {
    text_.erase(0, position);
    shift_mapping(0, -static_cast<int>(position));
    return *this;
}

void TextBuffer::set_length(std::size_t position) {
    text_.resize(position, '\0');
    if (line_to_offset_) std::erase_if(*line_to_offset_, [position](const auto& e) { return e.second > static_cast<int>(position); });
}

void TextBuffer::set_line_mapping(int line, int offset) {
    if (line < 0) return;
    if (!line_to_offset_) line_to_offset_.emplace();
    (*line_to_offset_)[line] = offset;
}

void TextBuffer::shift_mapping(int start_offset, int shift_offset) {
    if (!line_to_offset_) return;
    std::map<int, int> m;
    for (const auto& [line, value] : *line_to_offset_) {
        int v = value;
        if (v >= start_offset) v += shift_offset;
        if (v >= 0) m[line] = v;
    }
    *line_to_offset_ = std::move(m);
}

int TextBuffer::count(std::string_view substring, std::size_t from) const {
    int n = 0;
    std::size_t p = from;
    while ((p = text_.find(substring, p)) != std::string::npos && p > 0) {   // (the original's loop stops at a match at 0)
        ++n;
        p += substring.size();
    }
    return n;
}

void TextBuffer::dump_original_line_numbers(const std::vector<int>& mapping) {
    if (mapping.empty()) return;
    line_mapping_.emplace();
    for (std::size_t i = 0; i + 1 < mapping.size(); i += 2) {
        const int key = mapping[i + 1], value = mapping[i];
        const auto it = line_mapping_->find(key);
        if (it == line_mapping_->end() || value < it->second) (*line_mapping_)[key] = value;
    }
}

std::string TextBuffer::add_original_line_numbers() const {
    std::string sb;
    std::size_t line_start = 0, line_end;
    int count = 0;
    while ((line_end = text_.find(separator_, line_start)) != std::string::npos && line_end > 0) {
        ++count;
        sb += text_.substr(line_start, line_end - line_start);
        if (const auto it = line_mapping_->find(count); it != line_mapping_->end()) sb += "// " + std::to_string(it->second);
        sb += separator_;
        line_start = line_end + separator_.size();
    }
    if (line_start < text_.size()) sb += text_.substr(line_start);
    return sb;
}

void TextBuffer::append_lines(std::string& res, const std::vector<std::string>& src, int from, int to, int required) const {
    if (to - from > required) {
        const std::vector<std::string> lines = tb_compact_lines(std::vector<std::string>(src.begin() + from, src.begin() + to), required);
        int separators = required - 1;
        for (const std::string& s : lines) {
            res += s;
            if (separators-- > 0) res += separator_;
        }
        res += separator_;
    } else {
        for (int i = from; i < to; ++i) res += src[static_cast<std::size_t>(i)] + separator_;
        for (int i = 0; i < required - to + from; ++i) res += separator_;
    }
}

std::string TextBuffer::to_string() const {
    if (!line_to_offset_ || line_to_offset_->empty()) {
        if (line_mapping_) return add_original_line_numbers();
        return text_;
    }
    std::string res;
    const std::vector<std::string> src = tb_split(text_, separator_);
    int current_start = 0, current_line = 0, previous_mark = 0, dumped = 0;
    for (const auto& [mark_line, mark_offset] : *line_to_offset_) {   // (sorted by line, as the original sorts them)
        while (current_line < static_cast<int>(src.size())) {
            const int line_end = current_start + static_cast<int>(src[static_cast<std::size_t>(current_line)].size() + separator_.size());
            if (mark_offset <= line_end) {
                const int required = mark_line - 1;
                const int to_add = required - dumped;
                dumped = required;
                append_lines(res, src, previous_mark, current_line, to_add);
                previous_mark = current_line;
                break;
            }
            current_start = line_end;
            ++current_line;
        }
    }
    if (previous_mark < static_cast<int>(src.size()))
        append_lines(res, src, previous_mark, static_cast<int>(src.size()), static_cast<int>(src.size()) - previous_mark);
    return res;
}

std::string indent_string(int length) {
    if (length == 0) return {};
    const std::string indent = DecompilerContext::property(pref::INDENT_STRING);
    std::string buf;
    while (length-- > 0) buf += indent;
    return buf;
}

std::string char_to_unicode_literal(int value) {
    std::string hex = std::format("{:x}", static_cast<unsigned>(value));
    hex = ("0000" + hex).substr(hex.size());
    return "\\u" + hex;
}

bool is_printable_unicode(std::uint32_t c) {
    if (c > 0xFFFF) return false;
    const auto* it = std::upper_bound(std::begin(kPrintable), std::end(kPrintable), c, [](std::uint32_t v, const std::uint16_t (&r)[2]) { return v < r[0]; });
    if (it == std::begin(kPrintable)) return false;
    --it;
    return c >= (*it)[0] && c <= (*it)[1];
}

}  // namespace cb::java::decompiler
