#include "cb/base/ctext.hpp"

#include <algorithm>

namespace cb::base::ctext {

namespace {

// Where a string or character constant that opens at `open` ends: one past its closing quote when it closes, with
// `closed` set; else the end of what it covers (the end of its line, unless `across_lines`, or of the text; a
// backslash that is the last byte is not part of it). A backslash escapes the byte after it, a newline included.
struct Literal {
    std::size_t end;
    bool closed;
};

Literal scan_literal(std::string_view text, std::size_t open, bool across_lines) {
    const char quote = text[open];
    std::size_t j = open + 1;
    while (j < text.size()) {
        const char c = text[j];
        if (c == quote) return {j + 1, true};
        if (c == '\n' && !across_lines) break;
        if (c == '\\') {
            if (j + 1 >= text.size()) break;
            j += 2;
            continue;
        }
        ++j;
    }
    return {j, false};
}

bool opens_comment(std::string_view text, std::size_t i) {
    return text[i] == '/' && i + 1 < text.size() && text[i + 1] == '*';
}

void blank_range(std::string& s, std::size_t from, std::size_t to, bool keep_newlines) {
    for (std::size_t k = from; k < to; ++k)
        if (!(keep_newlines && s[k] == '\n')) s[k] = ' ';
}

std::string blank_inside(std::string_view text) {
    std::string out(text);
    std::size_t i = 0;
    while (i < text.size()) {
        if (opens_comment(text, i)) {
            const std::size_t close = text.find("*/", i + 2);
            const std::size_t end = close == std::string_view::npos ? text.size() : close + 2;
            blank_range(out, i, end, true);
            i = end;
        } else if (text[i] == '"' || text[i] == '\'') {
            const Literal lit = scan_literal(text, i, false);
            blank_range(out, i + 1, lit.closed ? lit.end - 1 : lit.end, false);
            i = lit.end;
        } else {
            ++i;
        }
    }
    return out;
}

std::string blank_whole(std::string_view text, bool across_lines) {
    std::string out(text);
    std::size_t i = 0;
    while (i < text.size()) {
        std::size_t end = i;   // i itself: nothing matched here
        if (opens_comment(text, i)) {
            const std::size_t close = text.find("*/", i + 2);
            if (close != std::string_view::npos) end = close + 2;
        } else if (text[i] == '"' || text[i] == '\'') {
            const Literal lit = scan_literal(text, i, across_lines);
            if (lit.closed) end = lit.end;
        }
        if (end == i) {
            ++i;
            continue;
        }
        blank_range(out, i, end, false);
        i = end;
    }
    return out;
}

}  // namespace

std::string blank(std::string_view text, Mode mode) {
    switch (mode) {
        case Mode::InsideLiterals: return blank_inside(text);
        case Mode::WholeClosed: return blank_whole(text, false);
        case Mode::WholeClosedAcrossLines: return blank_whole(text, true);
    }
    return std::string(text);
}

std::string sub_code(std::string_view text, const Regex& rx, const std::function<std::string(const RegexMatch&)>& fn) {
    const std::string code = blank(text);
    std::string out;
    out.reserve(text.size());
    std::size_t last = 0;
    rx.for_each_match(code, [&](const RegexMatch& m) {
        out.append(text.substr(last, m.start() - last));
        out.append(fn(m));
        last = m.end();
        return true;
    });
    out.append(text.substr(last));
    return out;
}

}  // namespace cb::base::ctext
