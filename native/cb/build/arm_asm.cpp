#include "cb/build/arm_asm.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::build::arm_asm {

namespace {

constexpr std::int64_t kMaxImageBytes = 16 * 1024 * 1024;
constexpr std::int64_t kGuessDistance = 8;

class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};

[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

struct SourceLine {
    std::uint32_t number = 0;
    std::string_view text;   // comment removed, trimmed, never empty
};

std::vector<SourceLine> significant_lines(std::string_view source) {
    std::vector<SourceLine> out;
    std::uint32_t number = 0;
    for (std::string_view raw : text::lines(source)) {
        ++number;
        const std::string_view line = text::trim(strip(raw));
        if (!line.empty()) out.push_back({number, line});
    }
    return out;
}

struct Word {
    std::string_view head;
    std::string_view rest;   // trimmed
};
Word split_word(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && !text::is_space(line[i])) ++i;
    return {line.substr(0, i), text::ltrim(line.substr(i))};
}

bool is_name_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '.' || c == '$'; }
bool is_name_char(char c) { return is_name_start(c) || (c >= '0' && c <= '9'); }

// `label:` at the start of a line, with what follows it.
std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view line) {
    if (line.empty() || !is_name_start(line[0])) return std::nullopt;
    std::size_t i = 1;
    while (i < line.size() && is_name_char(line[i])) ++i;
    if (i >= line.size() || line[i] != ':') return std::nullopt;
    return std::pair{line.substr(0, i), text::trim(line.substr(i + 1))};
}

constexpr std::array<std::string_view, 17> kIgnored = {".globl", ".global", ".text", ".data", ".bss", ".section", ".type", ".size", ".thumb_func", ".syntax",
                                                       ".fpu", ".eabi_attribute", ".file", ".extern", ".local", ".comm", ".ltorg"};

bool ignored_directive(std::string_view head) {
    const std::string h = text::lower(head);
    return std::ranges::find(kIgnored, h) != kIgnored.end();
}

void append_little_endian(Bytes& out, std::int64_t value, std::int64_t width) {
    const auto bits = static_cast<std::uint64_t>(value);
    for (std::int64_t i = 0; i < width; ++i) out.push_back(static_cast<std::uint8_t>((bits >> (8 * i)) & 0xFF));
}

// The bytes of a quoted string with GNU as's escapes.
Bytes string_bytes(std::string_view quoted) {
    Bytes out;
    const std::string_view body = quoted.substr(1, quoted.size() - 2);
    for (std::size_t i = 0; i < body.size(); ++i) {
        if (body[i] != '\\') {
            out.push_back(static_cast<std::uint8_t>(body[i]));
            continue;
        }
        if (++i >= body.size()) fail("a string ends in a backslash");
        switch (body[i]) {
            case 'n': out.push_back('\n'); break;
            case 't': out.push_back('\t'); break;
            case 'r': out.push_back('\r'); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case '\\': out.push_back('\\'); break;
            case '"': out.push_back('"'); break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
                unsigned v = 0;
                std::size_t n = 0;
                while (n < 3 && i < body.size() && body[i] >= '0' && body[i] <= '7') {
                    v = v * 8 + static_cast<unsigned>(body[i++] - '0');
                    ++n;
                }
                --i;
                out.push_back(static_cast<std::uint8_t>(v));
                break;
            }
            default: fail(std::format("unknown escape \\{}", body[i]));
        }
    }
    return out;
}

class Pass {
public:
    Pass(const Encoder& isa, const Encoder* v5, Labels& labels, std::unordered_map<std::uint32_t, std::size_t>& sizes, bool final)
        : v4t_(isa), v5_(v5), active_(&isa), labels_(labels), sizes_(sizes), final_(final) {}

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
    std::int64_t need(std::string_view expr) const {
        Result<std::int64_t> r = active_->value(expr, labels_, pc_);
        if (!r.ok()) fail(r.error().what());
        return r.value();
    }
    std::optional<std::int64_t> soft(std::string_view expr, std::int64_t pc) const {
        Result<std::int64_t> r = active_->value(expr, labels_, pc);
        if (r.ok()) return r.value();
        if (is_undefined(r.error()) && !final_) return std::nullopt;
        fail(r.error().what());
    }

    void put(const Bytes& data) {
        if (static_cast<std::int64_t>(image_.size() + data.size()) > kMaxImageBytes) fail(std::format("the file is past {} bytes", kMaxImageBytes));
        image_.insert(image_.end(), data.begin(), data.end());
        pc_ += static_cast<std::int64_t>(data.size());
    }

    void line(const SourceLine& l) {
        std::string_view text = l.text;
        current_ = text;
        line_number_ = l.number;
        while (const auto label = split_label(text)) {
            labels_[std::string(label->first)] = pc_;
            text = label->second;
            if (text.empty()) return;
            current_ = text;
        }
        const auto [head, rest] = split_word(text);

        if (ignored_directive(head)) return;
        if (text::iequals(head, ".set")) {
            const std::vector<std::string_view> parts = split_items(rest, true);
            if (parts.size() == 2) {
                if (const auto v = soft(parts[1], pc_)) labels_[std::string(parts[0])] = *v;
                return;
            }
            fail("`.set` takes a name and a value");
        }
        if (text::iequals(head, ".equ")) {
            const std::vector<std::string_view> parts = split_items(rest, true);
            if (parts.size() != 2) fail("`.equ` takes a name and a value");
            if (const auto v = soft(parts[1], pc_)) labels_[std::string(parts[0])] = *v;
            return;
        }
        if (text::is_identifier(head) && rest.starts_with('=')) {   // name = value
            if (const auto v = soft(text::ltrim(rest.substr(1)), pc_)) labels_[std::string(head)] = *v;
            return;
        }
        if (text::iequals(head, ".cpu") || text::iequals(head, ".arch")) {   // the processor: the ARM946E-S and ARMv5 name the ARMv5TE set (when an encoder for it was given)
            const std::string name = text::lower(rest);
            active_ = (v5_ != nullptr && (name.find("946") != std::string::npos || name.find("armv5") != std::string::npos || name.find("arm9e") != std::string::npos)) ? v5_ : &v4t_;
            return;
        }
        if (text::iequals(head, ".arm")) {
            thumb_ = false;
            return;
        }
        if (text::iequals(head, ".thumb")) {
            thumb_ = true;
            return;
        }
        if (text::iequals(head, ".code")) {
            const std::int64_t bits = need(rest);
            if (bits != 16 && bits != 32) fail("`.code` takes 16 (Thumb) or 32 (ARM)");
            thumb_ = bits == 16;
            return;
        }
        if (text::iequals(head, ".org")) {
            pc_ = need(rest);
            return;
        }
        if (text::iequals(head, ".word") || text::iequals(head, ".int") || text::iequals(head, ".long") || text::iequals(head, ".4byte")) return put(items(rest, 4));
        if (text::iequals(head, ".half") || text::iequals(head, ".hword") || text::iequals(head, ".short") || text::iequals(head, ".2byte")) return put(items(rest, 2));
        if (text::iequals(head, ".byte")) return put(items(rest, 1));
        if (text::iequals(head, ".ascii")) return put(strings(rest, false));
        if (text::iequals(head, ".asciz") || text::iequals(head, ".asciiz")) return put(strings(rest, true));
        if (text::iequals(head, ".space") || text::iequals(head, ".skip")) {
            const std::vector<std::string_view> parts = split_items(rest, true);
            if (parts.empty() || parts.size() > 2) fail("`.space` takes a count and a fill value");
            const std::int64_t count = need(parts[0]);
            const std::int64_t value = parts.size() == 2 ? need(parts[1]) : 0;
            if (count < 0 || count > kMaxImageBytes) fail(std::format(".space of {} bytes", count));
            return put(Bytes(static_cast<std::size_t>(count), static_cast<std::uint8_t>(value)));
        }
        if (text::iequals(head, ".fill")) {
            const std::vector<std::string_view> parts = split_items(rest, true);
            if (parts.empty() || parts.size() > 3) fail("`.fill` takes a count, a size and a value");
            const std::int64_t count = need(parts[0]);
            const std::int64_t size = parts.size() > 1 ? need(parts[1]) : 1;
            const std::int64_t value = parts.size() > 2 ? need(parts[2]) : 0;
            if (count < 0 || size < 1 || size > 8 || count * size > kMaxImageBytes) fail(std::format(".fill of {} items of {} bytes", count, size));
            Bytes out;
            for (std::int64_t i = 0; i < count; ++i) append_little_endian(out, value, size);
            return put(out);
        }
        if (text::iequals(head, ".align") || text::iequals(head, ".p2align")) {
            const std::int64_t power = need(rest);
            if (power < 0 || power > 16) fail("an alignment of 2^" + std::to_string(power));
            const std::int64_t unit = std::int64_t{1} << power;
            const std::int64_t pad = (unit - (pc_ % unit)) % unit;
            return put(Bytes(static_cast<std::size_t>(pad), 0));
        }
        if (text::iequals(head, ".balign")) {
            const std::int64_t unit = need(rest);
            if (unit < 1 || unit > 65536) fail("an alignment of " + std::to_string(unit) + " bytes");
            return put(Bytes(static_cast<std::size_t>((unit - (pc_ % unit)) % unit), 0));
        }
        if (head.starts_with('.')) fail("unknown directive " + std::string(head));
        instruction(text);
    }

    Bytes items(std::string_view items_text, std::int64_t width) const {
        Bytes out;
        for (std::string_view item : split_items(items_text, true)) {
            const std::int64_t v = soft(item, pc_ + static_cast<std::int64_t>(out.size())).value_or(0);
            append_little_endian(out, v, width);
        }
        return out;
    }

    Bytes strings(std::string_view items_text, bool terminate) const {
        Bytes out;
        for (std::string_view item : split_items(items_text, false)) {
            if (item.size() < 2 || item.front() != '"' || item.back() != '"') fail("a string is written in double quotes");
            const Bytes b = string_bytes(item);
            out.insert(out.end(), b.begin(), b.end());
            if (terminate) out.push_back(0);
        }
        return out;
    }

    void instruction(std::string_view text) {
        const std::int64_t unit = thumb_ ? 2 : 4;
        if (pc_ % unit != 0) fail(std::format("an instruction at ${:X}, which is not a multiple of {}", pc_, unit));
        const std::int64_t at = thumb_ ? pc_ | 1 : pc_;   // (the lowest bit of the address is the state: build/arm_asm.hpp)
        Result<Bytes> r = active_->assemble(text, at, labels_);
        if (!r.ok() && is_undefined(r.error()) && !final_) {
            r = size_guess(*active_, text, at, labels_);
            if (!r.ok() && !is_undefined(r.error())) r = Bytes(nominal_size(text), 0);   // (the first pass only needs how long it is)
        }
        if (!r.ok()) fail(r.error().what());
        const Bytes& code = r.value();
        if (!final_) {
            sizes_[line_number_] = code.size();
        } else if (const auto it = sizes_.find(line_number_); it != sizes_.end() && it->second != code.size()) {
            fail("the instruction's size changed between passes");
        }
        put(code);
    }

    // How long an instruction is when its operands cannot be worked out yet: a Thumb BL is two halfwords, every other instruction one unit.
    std::size_t nominal_size(std::string_view text) const {
        if (!thumb_) return 4;
        const std::string mnemonic = text::lower(split_word(text).head);
        return mnemonic == "bl" ? 4 : 2;
    }


    const Encoder& v4t_;
    const Encoder* v5_;
    const Encoder* active_;   // the encoder the instructions are assembled with: `.cpu` picks it
    Labels& labels_;
    std::unordered_map<std::uint32_t, std::size_t>& sizes_;
    bool final_;
    Bytes image_;
    std::int64_t pc_ = 0;
    bool thumb_ = false;   // the state instructions are assembled in
    std::string_view current_;
    std::uint32_t line_number_ = 0;
};

}  // namespace

std::string_view strip(std::string_view line) {
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '\\' && quoted) {
            ++i;
        } else if (line[i] == '"') {
            quoted = !quoted;
        } else if ((line[i] == '@' || (line[i] == '#' && text::trim(line.substr(0, i)).empty())) && !quoted) {   // (`#` begins a comment only as the first character of a line: it is an immediate's mark elsewhere)
            return text::rtrim(line.substr(0, i));
        }
    }
    return text::rtrim(line);
}

Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc, const Labels& labels) {
    Labels any = labels;
    for (;;) {
        Result<Bytes> r = encoder.assemble(line, pc, any);
        if (r.ok() || !is_undefined(r.error())) return r;
        const std::string& name = static_cast<const UndefinedSymbolError&>(r.error()).name();
        if (any.contains(name)) return r;
        any[name] = ((pc & ~std::int64_t{1}) & ~std::int64_t{3}) + kGuessDistance;   // (a word-aligned place just ahead: in reach of every branch and PC-relative load)
    }
}

Bytes Assembler::build(std::string_view source) const {
    const std::vector<SourceLine> lines = significant_lines(source);
    Labels labels;
    std::unordered_map<std::uint32_t, std::size_t> sizes;
    Bytes image;
    for (const bool final : {false, true}) {
        Pass pass(*isa_, v5_, labels, sizes, final);
        image = pass.run(lines);
    }
    return image;
}

}  // namespace cb::build::arm_asm
