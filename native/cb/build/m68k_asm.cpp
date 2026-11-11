#include "cb/build/m68k_asm.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::build::m68k_asm {

namespace {

// The largest image the assembler will size to: an `org` or a `dcb` with a wrong number must not allocate gigabytes.
constexpr std::int64_t kMaxImageBytes = 64 * 1024 * 1024;

// What an unknown name stands for while an instruction is only being sized.
constexpr std::int64_t kGuessDistance = 4;

// One reason a single line did not assemble; the pass loop turns it into an AsmError that names the line.
class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};

[[noreturn]] void fail(const std::string& reason) {
    throw LineFault(reason);
}

struct SourceLine {
    std::uint32_t number = 0; // 1-based, in the source
    std::string_view text;    // comment removed, trimmed, never empty
};

std::vector<SourceLine> significant_lines(std::string_view source) {
    std::vector<SourceLine> out;
    std::uint32_t number = 0;
    for (std::string_view raw : text::lines(source)) {
        ++number;
        const std::string_view line = text::trim(strip(raw));
        if (!line.empty())
            out.push_back({number, line});
    }
    return out;
}

struct Word {
    std::string_view head;
    std::string_view rest; // trimmed
};
Word split_word(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && !text::is_space(line[i]))
        ++i;
    return {line.substr(0, i), text::ltrim(line.substr(i))};
}

bool is_name_start(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}
bool is_name_char(char c) {
    return is_name_start(c) || (c >= '0' && c <= '9');
}

// `label:` at the start of a line, with what follows it. A label starts with a letter, `_`, `.` or `@` and goes on with
// those and digits (local labels).
std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view line) {
    auto start = [](char c) { return is_name_start(c) || c == '.' || c == '@'; };
    if (line.empty() || !start(line[0]))
        return std::nullopt;
    std::size_t i = 1;
    while (i < line.size() && (start(line[i]) || (line[i] >= '0' && line[i] <= '9')))
        ++i;
    if (i >= line.size() || line[i] != ':')
        return std::nullopt;
    return std::pair{line.substr(0, i), text::trim(line.substr(i + 1))};
}

// The operand size a `dc.x` / `dcb.x` directive's suffix names: 1, 2 or 4 bytes; 0 for anything else.
std::int64_t width_of(char suffix) {
    switch (suffix) {
    case 'b':
    case 'B':
        return 1;
    case 'w':
    case 'W':
        return 2;
    case 'l':
    case 'L':
        return 4;
    default:
        return 0;
    }
}

// `dc.w` -> 2: the width of a directive word that is `<stem>.<b|w|l>`, 0 when it is not one.
std::int64_t directive_width(std::string_view word, std::string_view stem) {
    if (word.size() != stem.size() + 2 || !text::istarts_with(word, stem) || word[stem.size()] != '.')
        return 0;
    return width_of(word.back());
}

// `value` as `width` big-endian bytes (the high bits that do not fit are dropped, as an assembler does).
void append_big_endian(Bytes& out, std::int64_t value, std::int64_t width) {
    const auto bits = static_cast<std::uint64_t>(value);
    for (std::int64_t i = width - 1; i >= 0; --i)
        out.push_back(static_cast<std::uint8_t>((bits >> (8 * i)) & 0xFF));
}

// One assembly pass: the image it writes and the labels it learns.
class Pass {
public:
    Pass(const Encoder& isa, Labels& labels, std::unordered_map<std::uint32_t, std::size_t>& sizes, bool final)
        : isa_(isa), labels_(labels), sizes_(sizes), final_(final) {}

    Bytes run(const std::vector<SourceLine>& lines) {
        for (const SourceLine& l : lines) {
            try {
                line(l);
            } catch (const LineFault& e) {
                throw AsmError(l.number, current_, e.what());
            }
        }
        return std::move(image_);
    }

private:
    // An expression that must have a value now (an address, a count).
    std::int64_t need(std::string_view expr, std::int64_t pc) const {
        Result<std::int64_t> r = isa_.value(expr, labels_, pc);
        if (!r.ok())
            fail(r.error().what());
        return r.value();
    }
    // An expression whose label may be defined further on: nullopt on the first pass, a fault on the last.
    std::optional<std::int64_t> soft(std::string_view expr, std::int64_t pc) const {
        Result<std::int64_t> r = isa_.value(expr, labels_, pc);
        if (r.ok())
            return r.value();
        if (is_undefined(r.error()) && !final_)
            return std::nullopt;
        fail(r.error().what());
    }

    void put(const Bytes& data) {
        if (pc_ < 0 || pc_ + static_cast<std::int64_t>(data.size()) > kMaxImageBytes) {
            fail(std::format("bytes outside the image (${:X}, {} bytes)", pc_, data.size()));
        }
        const auto at = static_cast<std::size_t>(pc_);
        if (at + data.size() > image_.size())
            image_.resize(at + data.size(), 0);
        std::copy(data.begin(), data.end(), image_.begin() + static_cast<std::ptrdiff_t>(at));
        pc_ += static_cast<std::int64_t>(data.size());
    }

    void line(const SourceLine& l) {
        line_number_ = l.number;
        std::string_view text = l.text;
        current_ = text;
        if (const auto label = split_label(text)) {
            labels_[std::string(label->first)] = pc_;
            text = label->second;
            if (text.empty())
                return;
            current_ = text;
        }
        const auto [head, rest] = split_word(text);

        if (text::is_identifier(head)) {
            const Word keyword = split_word(rest);
            if (text::iequals(keyword.head, "equ") && !keyword.rest.empty()) {
                if (const auto v = soft(keyword.rest, pc_))
                    labels_[std::string(head)] = *v;
                return;
            }
        }
        if (text::iequals(head, "org") && !rest.empty()) {
            pc_ = need(rest, pc_);
            return;
        }
        if (const std::int64_t width = directive_width(head, "dc"); width != 0 && !rest.empty()) {
            put(data(rest, width));
            return;
        }
        if (const std::int64_t width = directive_width(head, "dcb"); width != 0 && !rest.empty()) {
            const std::size_t comma = rest.find(',');
            if (comma != std::string_view::npos && comma > 0) {
                const std::string_view value_expr = text::ltrim(rest.substr(comma + 1));
                if (!value_expr.empty())
                    return block(rest.substr(0, comma), value_expr, width);
            }
        }
        if (text::iequals(text, "even")) {
            pc_ += pc_ & 1;
            return;
        }
        instruction(text);
    }

    // The bytes of a dc.x directive's items. A "string" is its own bytes (dc.b only); a number or a label is `width`
    // big-endian bytes; a label not defined yet is 0 on the first pass.
    Bytes data(std::string_view items_text, std::int64_t width) const {
        Bytes out;
        for (std::string_view item : split_items(items_text, true)) {
            if (width == 1 && item.size() >= 2 && item.front() == '"' && item.back() == '"') {
                const auto text = as_bytes(item.substr(1, item.size() - 2));
                out.insert(out.end(), text.begin(), text.end());
                continue;
            }
            const std::int64_t v = soft(item, pc_ + static_cast<std::int64_t>(out.size())).value_or(0);
            append_big_endian(out, v, width);
        }
        return out;
    }

    // dcb.x count,value
    void block(std::string_view count_expr, std::string_view value_expr, std::int64_t width) {
        const std::int64_t count = need(count_expr, pc_);
        const std::int64_t v = need(value_expr, pc_);
        if (count < 0 || count > kMaxImageBytes / width)
            fail(std::format("dcb of {} items", count));
        Bytes one;
        append_big_endian(one, v, width);
        Bytes out;
        out.reserve(one.size() * static_cast<std::size_t>(count));
        for (std::int64_t i = 0; i < count; ++i)
            out.insert(out.end(), one.begin(), one.end());
        put(out);
    }

    // One instruction. On the first pass a label it names that is not defined yet only needs the size right; the sizes
    // are remembered, and the last pass must find each the same (a short branch settles between the passes).
    void instruction(std::string_view text) {
        Result<Bytes> r = isa_.assemble(text, pc_, labels_);
        if (!r.ok() && is_undefined(r.error()) && !final_)
            r = size_guess(isa_, text, pc_, labels_);
        if (!r.ok())
            fail(r.error().what());
        const Bytes& code = r.value();
        if (!final_) {
            sizes_[line_number_] = code.size();
        } else if (const auto it = sizes_.find(line_number_); it != sizes_.end() && it->second != code.size()) {
            fail("the instruction's size changed between passes");
        }
        put(code);
    }

    const Encoder& isa_;
    Labels& labels_;
    std::unordered_map<std::uint32_t, std::size_t>& sizes_; // line number -> bytes on the first pass; looked up only
    bool final_;
    Bytes image_;
    std::int64_t pc_ = 0;
    std::string_view current_; // the line being assembled, for the error message
    std::uint32_t line_number_ = 0;
};

} // namespace

std::string_view strip(std::string_view line) {
    if (text::ltrim(line).starts_with('*'))
        return {};
    return text::rtrim(strip_comment(line));
}

Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc, const Labels& labels) {
    // The labels so far, plus each name the encoder asks for that is not among them, worth pc + 4.
    Labels any = labels;
    for (;;) {
        Result<Bytes> r = encoder.assemble(line, pc, any);
        if (r.ok() || !is_undefined(r.error()))
            return r;
        const std::string& name = static_cast<const UndefinedSymbolError&>(r.error()).name();
        if (any.contains(name))
            return r; // it asked again for a name it was given
        any[name] = pc + kGuessDistance;
    }
}

Bytes Assembler::build(std::string_view source) const {
    const std::vector<SourceLine> lines = significant_lines(source);
    Labels labels; // kept across the passes
    std::unordered_map<std::uint32_t, std::size_t> sizes;
    Bytes image;
    for (const bool final : {false, true}) {
        Pass pass(*isa_, labels, sizes, final);
        image = pass.run(lines);
    }
    return image;
}

} // namespace cb::build::m68k_asm
