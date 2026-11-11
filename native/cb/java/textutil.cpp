#include "cb/java/textutil.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::java::textutil {

std::optional<Span> literal_at(std::string_view s, std::size_t pos) {
    if (pos >= s.size() || (s[pos] != '"' && s[pos] != '\'')) return std::nullopt;
    const char quote = s[pos];
    std::size_t i = pos + 1;
    while (i < s.size()) {
        const char c = s[i];
        if (c == quote) return Span{pos, i + 1};
        if (c == '\\') {
            if (i + 1 >= s.size() || s[i + 1] == '\n') return std::nullopt;
            i += 2;
        } else {
            ++i;
        }
    }
    return std::nullopt;
}

std::vector<Span> literal_spans(std::string_view s) {
    std::vector<Span> out;
    std::size_t pos = 0;
    while (pos < s.size()) {
        if (auto span = literal_at(s, pos)) {
            out.push_back(*span);
            pos = span->end;
        } else {
            ++pos;
        }
    }
    return out;
}

namespace {

// "\r\n" and a lone "\r" both become "\n" (what a text-mode read gives).
std::string normalise_newlines(std::string text) {
    if (text.find('\r') == std::string::npos) return text;
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r') {
            out += '\n';
            if (i + 1 < text.size() && text[i + 1] == '\n') ++i;
        } else {
            out += text[i];
        }
    }
    return out;
}

std::string decode(const std::filesystem::path& path, std::string raw, Decode mode) {
    if (!text::is_valid_utf8(raw)) {
        if (mode == Decode::Strict) throw FormatError(fs::to_text(path), 0, "not valid UTF-8");
        raw = text::sanitize_utf8(raw);
    }
    return normalise_newlines(std::move(raw));
}

}  // namespace

std::string read_text(const std::filesystem::path& path, Decode mode) {
    return decode(path, fs::read_text(path), mode);
}

std::optional<std::string> try_read_text(const std::filesystem::path& path, Decode mode) {
    std::string raw;
    try {
        raw = fs::read_text(path);
    } catch (const IoError&) {   // the one place an unreadable file is a normal answer, not a fault
        return std::nullopt;
    }
    return decode(path, std::move(raw), mode);
}

void write_text(const std::filesystem::path& path, std::string_view text) { fs::write_text(path, text); }

}  // namespace cb::java::textutil
