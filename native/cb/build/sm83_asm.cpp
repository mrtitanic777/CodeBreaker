// cb-unity: alone  (it names file-local helpers after the line forms; z80_asm.cpp's are of the same kind)
#include "cb/build/sm83_asm.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/sm83.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::build::sm83_asm {

namespace {

namespace z80 = cb::decompile::z80;
namespace sm83 = cb::decompile::sm83;
using decompile::NameLabels;

// One reason a single line did not assemble; the pass turns it into an AsmError that names the line.
class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};

[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

constexpr char LF = 10, CR = 13;

struct SourceLine {
    std::uint32_t number = 0;   // 1-based, in the source
    std::string_view text;      // comment removed, trimmed, never empty
};

// The lines of the source: a line feed, a carriage return and line feed, or a lone carriage return ends one.
std::vector<std::string_view> split_source_lines(std::string_view source) {
    std::vector<std::string_view> out;
    std::size_t start = 0;
    for (std::size_t i = 0; i < source.size(); ++i) {
        if (source[i] != LF && source[i] != CR) continue;
        out.push_back(source.substr(start, i - start));
        if (source[i] == CR && i + 1 < source.size() && source[i + 1] == LF) ++i;
        start = i + 1;
    }
    if (start < source.size()) out.push_back(source.substr(start));
    return out;
}

std::vector<SourceLine> significant_lines(std::string_view source) {
    std::vector<SourceLine> out;
    std::uint32_t number = 0;
    for (std::string_view raw : split_source_lines(source)) {
        ++number;
        const std::string_view line = text::trim(strip_comment(raw));
        if (!line.empty()) out.push_back({number, line});
    }
    return out;
}

// ---- the forms of a line ------------------------------------------------------------------------------------------

bool is_letter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// The word before the first whitespace and the trimmed remainder ("" when there is none).
struct Word {
    std::string_view head;
    std::string_view rest;
};
Word split_word(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && !text::is_space(line[i])) ++i;
    return {line.substr(0, i), text::ltrim(line.substr(i))};
}

// `label:` or `label::` at the start of a line, with what follows it. A label starts with a letter, `_` or `.` and goes on
// with letters, digits, `_` and `.`.
std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view line) {
    if (line.empty() || !(is_letter(line[0]) || line[0] == '_' || line[0] == '.')) return std::nullopt;
    std::size_t i = 1;
    while (i < line.size() && (is_letter(line[i]) || is_digit(line[i]) || line[i] == '_' || line[i] == '.')) ++i;
    if (i >= line.size() || line[i] != ':') return std::nullopt;
    std::size_t colons = 1;
    if (i + 1 < line.size() && line[i + 1] == ':') colons = 2;
    return std::pair{line.substr(0, i), text::trim(line.substr(i + colons))};
}

// `NAME EQU value` and `DEF NAME EQU value`: a constant.
struct Constant {
    std::string_view name;
    std::string_view expr;
};
std::optional<Constant> equ_without_def(std::string_view line) {
    const Word first = split_word(line);
    const bool name = !first.head.empty() && (is_letter(first.head[0]) || first.head[0] == '_') &&
                      std::all_of(first.head.begin(), first.head.end(), [](char c) { return is_letter(c) || is_digit(c) || c == '_'; });
    if (!name) return std::nullopt;
    const Word second = split_word(first.rest);
    if (!text::iequals(second.head, "EQU") || second.rest.empty()) return std::nullopt;
    return Constant{first.head, second.rest};
}
std::optional<Constant> split_equ(std::string_view line) {
    const Word def = split_word(line);
    if (text::iequals(def.head, "DEF") && !def.rest.empty())
        if (auto c = equ_without_def(def.rest)) return c;
    return equ_without_def(line);   // (`DEF EQU 5` is the constant called DEF)
}

// `SECTION "name", ROM0[addr]` and `SECTION "name", ROMX[addr], BANK[n]`.
struct Section {
    bool rom0 = true;
    std::string_view address;
    std::optional<std::string_view> bank;
};
// `[ expr ]` at the start of `s`: the text inside, and what follows the `]`.
std::optional<std::pair<std::string_view, std::string_view>> bracketed(std::string_view s) {
    s = text::ltrim(s);
    if (s.empty() || s[0] != '[') return std::nullopt;
    const std::size_t close = s.find(']');
    if (close == std::string_view::npos || close == 1) return std::nullopt;
    return std::pair{text::trim(s.substr(1, close - 1)), text::ltrim(s.substr(close + 1))};
}
std::optional<Section> split_section(std::string_view line) {
    const Word word = split_word(line);
    if (!text::iequals(word.head, "SECTION") || word.rest.empty() || word.rest[0] != '"') return std::nullopt;
    const std::size_t close = word.rest.find('"', 1);
    if (close == std::string_view::npos) return std::nullopt;
    std::string_view rest = text::ltrim(word.rest.substr(close + 1));
    if (rest.empty() || rest[0] != ',') return std::nullopt;
    rest = text::ltrim(rest.substr(1));
    Section out;
    if (rest.size() < 4 || !(text::iequals(rest.substr(0, 4), "ROM0") || text::iequals(rest.substr(0, 4), "ROMX"))) return std::nullopt;
    out.rom0 = text::iequals(rest.substr(0, 4), "ROM0");
    const auto address = bracketed(rest.substr(4));
    if (!address) return std::nullopt;
    out.address = address->first;
    rest = address->second;
    if (rest.empty()) return out;
    if (rest[0] != ',') return std::nullopt;
    rest = text::ltrim(rest.substr(1));
    if (rest.size() < 4 || !text::iequals(rest.substr(0, 4), "BANK")) return std::nullopt;
    const auto bank = bracketed(rest.substr(4));
    if (!bank || !bank->second.empty()) return std::nullopt;
    out.bank = bank->first;
    return out;
}

// ---- the passes -------------------------------------------------------------------------------------------------------

// The size an instruction will have when a label it names is not defined yet: every name counts as an address (`ldh` takes
// one in the high page).
Result<Bytes> size_guess(std::string_view line, std::int64_t pc) {
    const std::int64_t guess = text::upper(line).find("LDH") != std::string::npos ? 0xFF80 : pc;
    NameLabels any;
    for (std::size_t i = 0; i < line.size();) {
        const char c = line[i];
        if (is_letter(c) || c == '_' || c == '.' || c == '@') {
            std::size_t j = i + 1;
            while (j < line.size() && (is_letter(line[j]) || is_digit(line[j]) || line[j] == '_' || line[j] == '.' || line[j] == '@')) ++j;
            any[std::string(line.substr(i, j - i))] = guess;
            i = j;
        } else {
            ++i;
        }
    }
    try {
        return sm83::assemble(line, static_cast<std::uint32_t>(pc), any);
    } catch (const ParseError& e) {   // (a line that does not assemble even with every name known)
        return Failure(e.clone());
    }
}

// One instruction line at `pc`, or why not (UnknownLabel when it names a label that is not defined).
Result<Bytes> try_assemble(std::string_view line, std::int64_t pc, const NameLabels& labels) {
    try {
        return sm83::assemble(line, static_cast<std::uint32_t>(pc), labels);
    } catch (const ParseError& e) {
        return Failure(e.clone());   // (the dynamic type: an unknown label is routine in the first pass)
    }
}

bool is_unknown_label(const Error& e) { return e.kind() == "UnknownLabel"; }

// Why, as a sentence: the message of a ParseError begins with its (here empty) place.
std::string reason_of(const Error& e) {
    std::string reason = e.what();
    if (reason.starts_with(": ")) reason.erase(0, 2);
    return reason;
}

// An offset as the message writes it: $1F, or $-5 for one before the bank.
std::string offset_text(std::int64_t pos) { return pos < 0 ? std::format("$-{:X}", static_cast<std::uint64_t>(-pos)) : std::format("${:X}", pos); }

class Pass {
public:
    Pass(NameLabels& labels, bool final, std::size_t size, const std::filesystem::path& folder)
        : labels_(labels), final_(final), folder_(folder), image_(size, 0xFF) {}

    // The line, or a LineFault for why it cannot be taken.
    void line(std::string_view text);
    // The part of the line the fault is about: what follows a label on the line.
    std::string_view shown() const { return shown_; }
    Bytes take_image() { return std::move(image_); }

private:
    std::int64_t pc(std::size_t extra = 0) const { return base_ + pos_ + static_cast<std::int64_t>(extra); }
    // An expression that must have a value now.
    std::int64_t need(std::string_view expr, std::int64_t at = 0) const {
        Result<std::int64_t> r = z80::try_value(expr, labels_, static_cast<std::uint32_t>(at));
        if (!r.ok()) fail(reason_of(r.error()));
        return r.value();
    }
    // An expression whose label may be defined further on: nullopt in the first pass, a fault in the last.
    std::optional<std::int64_t> soft(std::string_view expr, std::int64_t at) const {
        Result<std::int64_t> r = z80::try_value(expr, labels_, static_cast<std::uint32_t>(at));
        if (r.ok()) return r.value();
        if (is_unknown_label(r.error()) && !final_) return std::nullopt;
        fail(reason_of(r.error()));
    }
    void put(ByteView data);
    void data_directive(std::string_view items, bool word);
    void reserve(std::string_view args);
    void include_binary(std::string_view name);
    void instruction(std::string_view text);

    NameLabels& labels_;
    bool final_;
    std::filesystem::path folder_;
    Bytes image_;
    std::int64_t bank_ = 0, base_ = 0, pos_ = 0;
    std::string_view shown_;
};

// Write `data` at the current place of the current bank, and move on.
void Pass::put(ByteView data) {
    const std::int64_t at = bank_ * kBank + pos_;
    if (at < 0 || at + static_cast<std::int64_t>(data.size()) > static_cast<std::int64_t>(image_.size()))
        fail(std::format("bytes outside the ROM (bank {}, offset {})", bank_, offset_text(pos_)));
    std::copy(data.begin(), data.end(), image_.begin() + static_cast<std::ptrdiff_t>(at));
    pos_ += static_cast<std::int64_t>(data.size());
}

void Pass::data_directive(std::string_view items, bool word) {
    Bytes data;
    for (std::string_view item : split_items(items, true)) {
        if (!word && !item.empty() && item.front() == '"' && item.back() == '"') {
            const std::string_view inside = item.size() >= 2 ? item.substr(1, item.size() - 2) : std::string_view();
            const Bytes latin1 = text::to_latin1(inside);   // (what the string says, a character a byte)
            data.insert(data.end(), latin1.begin(), latin1.end());
            continue;
        }
        const std::int64_t v = soft(item, pc(data.size())).value_or(0);
        data.push_back(static_cast<std::uint8_t>(v & 0xFF));
        if (word) data.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
    }
    put(data);
}

// `ds count[, value]`: count bytes of value (0 when there is none); a count below zero writes nothing and moves back.
void Pass::reserve(std::string_view args) {
    const std::size_t comma = args.find(',');
    const std::string_view count_text = comma == std::string_view::npos ? args : args.substr(0, comma);
    const std::int64_t count = need(count_text);
    const std::int64_t v = comma == std::string_view::npos ? 0 : need(text::ltrim(args.substr(comma + 1)));
    // (checked before the bytes are made: a count past the ROM is refused, not allocated)
    const std::int64_t at = bank_ * kBank + pos_;
    const std::int64_t room = static_cast<std::int64_t>(image_.size());
    if (at < 0 || at > room || std::max<std::int64_t>(count, 0) > room - at)
        fail(std::format("bytes outside the ROM (bank {}, offset {})", bank_, offset_text(pos_)));
    put(Bytes(static_cast<std::size_t>(std::max<std::int64_t>(count, 0)), static_cast<std::uint8_t>(v & 0xFF)));
    if (count < 0) pos_ += count;   // (a negative count moves back: what follows overwrites what was written)
}

void Pass::include_binary(std::string_view name) {
    const Bytes data = fs::read_file(folder_ / std::string(name));   // IoError: not a fault of the line
    put(data);
}

void Pass::instruction(std::string_view line_text) {
    Result<Bytes> r = try_assemble(line_text, pc(), labels_);
    if (!r.ok() && is_unknown_label(r.error()) && !final_) r = size_guess(line_text, pc());
    if (!r.ok()) fail(reason_of(r.error()));
    put(r.value());
}

void Pass::line(std::string_view text) {
    shown_ = text;
    if (const auto label = split_label(text)) {
        labels_[std::string(label->first)] = pc();
        text = label->second;
        shown_ = text;
        if (text.empty()) return;
    }
    if (const auto equ = split_equ(text)) {
        Result<std::int64_t> r = z80::try_value(equ->expr, labels_, static_cast<std::uint32_t>(pc()));
        if (r.ok()) labels_[std::string(equ->name)] = r.value();
        else if (!(is_unknown_label(r.error()) && !final_)) fail(reason_of(r.error()));
        return;
    }
    if (const auto section = split_section(text)) {
        const std::int64_t address = need(section->address);
        bank_ = section->bank ? need(*section->bank) : (section->rom0 ? 0 : 1);
        base_ = bank_ == 0 ? 0 : kBank;
        pos_ = address - base_;
        return;
    }
    const Word word = split_word(text);
    if (!word.rest.empty() && (text::iequals(word.head, "DB") || text::iequals(word.head, "DW"))) {
        data_directive(word.rest, text::iequals(word.head, "DW"));
        return;
    }
    if (text::iequals(word.head, "DS") && !word.rest.empty()) {
        const std::size_t comma = word.rest.find(',');
        const bool form = comma == std::string_view::npos ? true : (comma > 0 && !text::ltrim(word.rest.substr(comma + 1)).empty());
        if (form) {
            reserve(word.rest);
            return;
        }
    }
    if (text::iequals(word.head, "INCBIN") && !word.rest.empty() && word.rest[0] == '"') {
        const std::size_t close = word.rest.find('"', 1);
        if (close != std::string_view::npos && close > 1) {   // (what follows the closing quote is not read)
            include_binary(word.rest.substr(1, close - 1));
            return;
        }
    }
    instruction(text);
}

}  // namespace

Bytes build(std::string_view source, std::size_t size, const std::filesystem::path& folder) {
    const std::vector<SourceLine> lines = significant_lines(source);
    NameLabels labels;
    Bytes image;
    for (const bool final : {false, true}) {
        Pass pass(labels, final, size, folder);
        for (const SourceLine& line : lines) {
            try {
                pass.line(line.text);
            } catch (const LineFault& e) {
                throw AsmError(line.number, pass.shown(), e.what());
            }
        }
        image = pass.take_image();
    }
    return image;
}

}  // namespace cb::build::sm83_asm
