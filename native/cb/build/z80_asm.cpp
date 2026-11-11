#include "cb/build/z80_asm.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <string>
#include <utility>

#include "cb/core/fs.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/text.hpp"

namespace cb::build::z80_asm {

namespace {

using Path = std::filesystem::path;

// The largest ROM an image may be sized to by BANKSTOTAL * BANKSIZE: a wrong number must not allocate gigabytes.
constexpr std::int64_t kMaxRomBytes = 64 * 1024 * 1024;

// One reason a single line did not assemble. The pass loop turns it into an AsmError that names the line.
class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};

[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

// Run `step` for one source line; a fault in it is reported with the line's number and text.
template <class Step>
void on_line(std::uint32_t number, std::string_view text, Step&& step) {
    try {
        step();
    } catch (const LineFault& e) {
        throw AsmError(number, text, e.what());
    }
}

struct SourceLine {
    std::uint32_t number = 0;   // 1-based, in the source
    std::string_view text;      // comment removed, trimmed, never empty
};

std::vector<SourceLine> significant_lines(std::string_view source) {
    std::vector<SourceLine> out;
    std::uint32_t number = 0;
    for (std::string_view raw : text::lines(source)) {
        ++number;
        const std::string_view line = text::trim(strip_comment(raw));
        if (!line.empty()) out.push_back({number, line});
    }
    return out;
}

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

bool is_name_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
bool is_name_char(char c) { return is_name_start(c) || (c >= '0' && c <= '9'); }

// `label:` at the start of a line, with what follows it. Label names may start with `.` or `@` and contain them
// (local labels).
std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view line) {
    auto label_start = [](char c) { return is_name_start(c) || c == '.' || c == '@'; };
    auto label_char = [&](char c) { return label_start(c) || (c >= '0' && c <= '9'); };
    if (line.empty() || !label_start(line[0])) return std::nullopt;
    std::size_t i = 1;
    while (i < line.size() && label_char(line[i])) ++i;
    if (i >= line.size() || line[i] != ':') return std::nullopt;
    return std::pair{line.substr(0, i), text::trim(line.substr(i + 1))};
}

// `NAME = value` and `NAME EQU value`: a constant. `keyword` is "=" or "EQU" (matched without case).
struct Constant {
    std::string_view name;
    std::string_view expr;
};
std::optional<Constant> split_assignment(std::string_view line) {
    if (line.empty() || !is_name_start(line[0])) return std::nullopt;
    std::size_t i = 1;
    while (i < line.size() && is_name_char(line[i])) ++i;
    const std::string_view name = line.substr(0, i);
    const std::string_view rest = text::ltrim(line.substr(i));
    if (rest.empty() || rest[0] != '=') return std::nullopt;
    const std::string_view expr = text::ltrim(rest.substr(1));
    if (expr.empty()) return std::nullopt;
    return Constant{name, expr};
}
std::optional<Constant> split_equ(std::string_view line) {
    if (line.empty() || !is_name_start(line[0])) return std::nullopt;
    const auto [name, rest] = split_word(line);
    const auto [keyword, expr] = split_word(rest);
    if (!text::iequals(keyword, "EQU") || expr.empty()) return std::nullopt;
    return Constant{name, expr};
}

// `"name"` at the start of `rest`: the text between the quotes (not empty).
std::optional<std::string_view> quoted_name(std::string_view rest) {
    if (rest.empty() || rest[0] != '"') return std::nullopt;
    const std::size_t close = rest.find('"', 1);
    if (close == std::string_view::npos || close == 1) return std::nullopt;
    return rest.substr(1, close - 1);
}

// `first , second`: split at the first comma; both parts must exist. (`.DSB n,value`.) WLA-DX also takes the two
// separated by white space only (`.DSB 4 $AB`), as the header comment of z80_asm.hpp writes it.
struct Pair {
    std::string_view first;
    std::string_view second;
};
std::optional<Pair> split_pair(std::string_view rest) {
    const std::size_t comma = rest.find(',');
    if (comma == std::string_view::npos) {
        const std::vector<std::string_view> t = text::split_ws(rest);
        if (t.size() != 2) return std::nullopt;
        return Pair{t[0], t[1]};
    }
    if (comma == 0) return std::nullopt;
    const std::string_view second = text::ltrim(rest.substr(comma + 1));
    if (second.empty()) return std::nullopt;
    return Pair{rest.substr(0, comma), second};
}

bool all_digits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

Bytes repeated_byte(std::int64_t v, std::int64_t count) {
    return count > 0 ? Bytes(static_cast<std::size_t>(count), static_cast<std::uint8_t>(v & 0xFF)) : Bytes{};
}

// One assembly pass's evaluation of expressions and instructions against the labels so far.
class Evaluator {
public:
    Evaluator(const Encoder& isa, Labels& labels, bool final) : isa_(isa), labels_(labels), final_(final) {}

    // An expression that must have a value now (an address, a count): any failure is a fault.
    std::int64_t need(std::string_view expr, std::int64_t pc) const {
        Result<std::int64_t> r = isa_.value(expr, labels_, pc);
        if (!r.ok()) fail(r.error().what());
        return r.value();
    }
    // An expression whose label may be defined further on: nullopt on the first pass, a fault on the last.
    std::optional<std::int64_t> soft(std::string_view expr, std::int64_t pc) const {
        Result<std::int64_t> r = isa_.value(expr, labels_, pc);
        if (r.ok()) return r.value();
        if (is_undefined(r.error()) && !final_) return std::nullopt;
        fail(r.error().what());
    }
    // The bytes of an instruction; on the first pass a not-yet-defined label only needs the size right.
    Bytes instruction(std::string_view line, std::int64_t pc) const {
        Result<Bytes> r = isa_.assemble(line, pc, labels_);
        if (!r.ok() && is_undefined(r.error()) && !final_) r = size_guess(isa_, line, pc);
        if (!r.ok()) fail(r.error().what());
        return std::move(r).value();
    }
    // The bytes of a data directive's items: strings as they are (bytes only), numbers and labels as bytes or
    // little-endian words. A label not defined yet is 0 on the first pass. `base` is the address of the first byte.
    Bytes data(std::string_view items_text, bool word, std::int64_t base) const {
        Bytes out;
        for (std::string_view item : split_items(items_text, true)) {
            if (!word && item.size() >= 2 && item.front() == '"' && item.back() == '"') {
                const auto text = as_bytes(item.substr(1, item.size() - 2));
                out.insert(out.end(), text.begin(), text.end());
                continue;
            }
            const std::int64_t v = soft(item, base + static_cast<std::int64_t>(out.size())).value_or(0);
            out.push_back(static_cast<std::uint8_t>(v & 0xFF));
            if (word) out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        }
        return out;
    }
    Labels& labels() const { return labels_; }

private:
    const Encoder& isa_;
    Labels& labels_;
    bool final_;
};

Bytes read_include(const Path& folder, std::string_view name) { return fs::read_file(folder / std::string(name)); }

// ---- the WLA-DX dialect ---------------------------------------------------------------------------------------

// One pass over the lines, writing into the ROM image.
class RomPass {
public:
    RomPass(const Evaluator& ev, std::int64_t bank_size, const std::unordered_map<std::int64_t, std::int64_t>& slots,
            std::int64_t total, const Path& folder)
        : ev_(ev), bank_size_(bank_size), slots_(slots), folder_(folder), image_(static_cast<std::size_t>(total), 0xFF) {
        slot_base_ = slot_address(0);
    }

    Bytes run(const std::vector<SourceLine>& lines) {
        bool in_map = false;
        for (const SourceLine& l : lines) {
            const std::string up = text::upper(l.text);
            if (up.starts_with(".MEMORYMAP") || up.starts_with(".ROMBANKMAP")) {
                in_map = true;
            } else if (up.starts_with(".ENDME") || up.starts_with(".ENDRO")) {
                in_map = false;
            } else if (!in_map) {
                on_line(l.number, l.text, [&] { line(l.text); });
            }
        }
        return std::move(image_);
    }

private:
    std::int64_t slot_address(std::int64_t slot) const {
        const auto it = slots_.find(slot);
        return it == slots_.end() ? 0 : it->second;
    }
    std::int64_t here() const { return slot_base_ + pos_; }

    void put(const Bytes& data) {
        const std::int64_t at = bank_ * bank_size_ + pos_;
        if (at < 0 || at + static_cast<std::int64_t>(data.size()) > static_cast<std::int64_t>(image_.size())) {
            fail(std::format("bytes outside the ROM (bank {}, offset ${:X})", bank_, pos_));
        }
        std::copy(data.begin(), data.end(), image_.begin() + at);
        pos_ += static_cast<std::int64_t>(data.size());
    }

    void line(std::string_view text) {
        if (const auto label = split_label(text)) {
            ev_.labels()[std::string(label->first)] = here();
            text = label->second;
            if (text.empty()) return;
        }
        const auto [directive, rest] = split_word(text);
        const std::string name = text::upper(directive);

        if (name == ".DEFINE") {
            const Word w = split_word(rest);
            if (!w.rest.empty()) return constant(w.head, w.rest);
        } else if (const auto assign = split_assignment(text)) {
            return constant(assign->name, assign->expr);
        }
        if (name == ".BANK" && bank(rest)) return;
        if (name == ".ORG" && !rest.empty()) {
            pos_ = ev_.need(rest, 0);
            return;
        }
        if (name == ".ORGA" && !rest.empty()) {
            pos_ = ev_.need(rest, 0) - slot_base_;
            return;
        }
        if ((name == ".DB" || name == ".BYTE" || name == ".DW" || name == ".WORD") && !rest.empty()) {
            put(ev_.data(rest, name == ".DW" || name == ".WORD", here()));
            return;
        }
        if (name == ".DSB") {
            if (const auto p = split_pair(rest)) {
                const std::int64_t count = ev_.need(p->first, 0);
                const std::int64_t v = ev_.need(p->second, 0);
                if (count < 0 || count > static_cast<std::int64_t>(image_.size())) {
                    fail(std::format("bytes outside the ROM (.DSB of {} bytes)", count));
                }
                put(repeated_byte(v, count));
                return;
            }
        }
        if (name == ".INCBIN") {
            if (const auto file = quoted_name(rest)) {
                put(read_include(folder_, *file));
                return;
            }
        }
        const std::string up = text::upper(text);
        if (up.starts_with(".EMPTYFILL") || up.starts_with(".SDSCTAG") || up.starts_with(".COMPUTESMSCHECKSUM")) return;
        if (text.starts_with('.')) fail("directive not understood");
        put(ev_.instruction(text, here()));
    }

    void constant(std::string_view name, std::string_view expr) {
        if (const auto v = ev_.soft(expr, here())) ev_.labels()[std::string(name)] = *v;
    }

    // `.BANK n [SLOT s]`: false when the line is not of that form.
    bool bank(std::string_view rest) {
        const std::vector<std::string_view> t = text::split_ws(rest);
        const bool plain = t.size() == 1;
        if (!plain && !(t.size() == 3 && text::iequals(t[1], "SLOT"))) return false;
        bank_ = ev_.need(t[0], 0);
        slot_base_ = slot_address(plain ? 0 : ev_.need(t[2], 0));
        pos_ = 0;
        return true;
    }

    const Evaluator& ev_;
    std::int64_t bank_size_;
    const std::unordered_map<std::int64_t, std::int64_t>& slots_;
    Path folder_;
    Bytes image_;
    std::int64_t bank_ = 0;        // signed: a bad .BANK is reported by put(), not wrapped
    std::int64_t slot_base_ = 0;
    std::int64_t pos_ = 0;
};

}  // namespace

Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc) {
    // A table that has every name, each worth `pc`, grown by the names the encoder asks for.
    Labels any;
    for (;;) {
        Result<Bytes> r = encoder.assemble(line, pc, any);
        if (r.ok() || !is_undefined(r.error())) return r;
        const std::string& name = static_cast<const UndefinedSymbolError&>(r.error()).name();
        if (any.contains(name)) return r;   // it asked again for a name it was given
        any[name] = pc;
    }
}

Bytes Assembler::build_file(const Path& path) const {
    const std::string text = fs::read_text(path);
    return build(text, std::filesystem::absolute(path).parent_path());
}

Bytes Assembler::build(std::string_view source, const Path& folder) const {
    const std::vector<SourceLine> lines = significant_lines(source);

    // The memory map and ROM size come first: they are needed before the first byte is placed.
    std::unordered_map<std::int64_t, std::int64_t> slots;   // slot number -> CPU address; looked up only
    std::optional<std::int64_t> banks_total;
    std::int64_t bank_size = 0x4000;
    {
        Labels none;
        const Evaluator ev(*isa_, none, true);
        for (const SourceLine& l : lines) {
            on_line(l.number, l.text, [&] {
                const std::vector<std::string_view> t = text::split_ws(l.text);
                if (t.size() >= 3 && text::iequals(t[0], "SLOT") && all_digits(t[1])) {
                    slots[text::parse_int<std::int64_t>(t[1]).value_or(-1)] = ev.need(t[2], 0);
                }
                if (t.size() >= 2 && text::iequals(t[0], "BANKSTOTAL")) banks_total = ev.need(t[1], 0);
                if (t.size() >= 2 && text::iequals(t[0], "BANKSIZE")) bank_size = ev.need(t[1], 0);
            });
        }
    }
    const std::int64_t banks = banks_total && *banks_total != 0 ? *banks_total : 1;
    if (bank_size <= 0 || banks < 0 || banks > kMaxRomBytes / bank_size) {
        throw AsmError(std::format("ROM of {} banks of {} bytes: not a size this assembler builds", banks, bank_size));
    }

    // Two passes: the first learns the labels (their values only settle when every instruction has its size), the
    // second writes the final bytes.
    Labels labels;
    Bytes image;
    for (const bool final : {false, true}) {
        const Evaluator ev(*isa_, labels, final);
        RomPass pass(ev, bank_size, slots, banks * bank_size, folder);
        image = pass.run(lines);
    }
    return image;
}

Assembler::Flat Assembler::build_flat(std::string_view source, const Path& folder) const {
    const std::vector<SourceLine> lines = significant_lines(source);
    Labels labels;
    // Insertion order is the meaning: where chunks overlap, the one written last wins.
    OrderedMap<std::int64_t, Bytes> chunks;
    for (const bool final : {false, true}) {
        chunks.clear();
        const Evaluator ev(*isa_, labels, final);
        std::int64_t pc = 0;
        auto emit = [&](Bytes data) {
            const auto size = static_cast<std::int64_t>(data.size());
            chunks.insert_or_assign(pc, std::move(data));
            pc += size;
        };
        for (const SourceLine& l : lines) {
            on_line(l.number, l.text, [&] {
                std::string_view text = l.text;
                if (const auto label = split_label(text)) {
                    labels[std::string(label->first)] = pc;
                    text = label->second;
                    if (text.empty()) return;
                }
                const auto [directive, rest] = split_word(text);
                const std::string name = text::upper(directive);
                if (const auto equ = split_equ(text)) {
                    if (const auto v = ev.soft(equ->expr, pc)) labels[std::string(equ->name)] = *v;
                } else if (name == "ORG" && !rest.empty()) {
                    pc = ev.need(rest, pc);
                } else if ((name == "DEFB" || name == "DB" || name == "DEFM" || name == "DEFW" || name == "DW") &&
                           !rest.empty()) {
                    emit(ev.data(rest, name == "DEFW" || name == "DW", pc));
                } else if (name == "DEFS" || name == "DS") {
                    // DEFS n[,value]: an absent value is 0
                    const std::size_t comma = rest.find(',');
                    const std::string_view count_expr = comma == std::string_view::npos ? rest : rest.substr(0, comma);
                    const std::string_view value_expr =
                        comma == std::string_view::npos ? std::string_view() : text::ltrim(rest.substr(comma + 1));
                    if (count_expr.empty() || (comma != std::string_view::npos && value_expr.empty())) {
                        emit(ev.instruction(text, pc));
                        return;
                    }
                    const std::int64_t count = ev.need(count_expr, pc);
                    const std::int64_t v = value_expr.empty() ? 0 : ev.need(value_expr, pc);
                    if (count < 0 || count > kMaxRomBytes) fail(std::format("DEFS of {} bytes", count));
                    emit(repeated_byte(v, count));
                } else if (name == "INCBIN" && quoted_name(rest)) {
                    emit(read_include(folder, *quoted_name(rest)));
                } else {
                    emit(ev.instruction(text, pc));
                }
            });
        }
    }
    Flat flat;
    if (chunks.empty()) return flat;
    std::int64_t start = chunks.begin()->first;
    std::int64_t end = 0;
    for (const auto& [at, data] : chunks) {
        start = std::min(start, at);
        end = std::max(end, at + static_cast<std::int64_t>(data.size()));
    }
    if (start < 0 || start > 0xFFFFFFFF || end - start > kMaxRomBytes) {
        throw AsmError(std::format("program at ${:X}..${:X}: not an address range this assembler builds", start, end));
    }
    flat.start = static_cast<std::uint32_t>(start);
    flat.bytes.assign(static_cast<std::size_t>(end - start), 0);
    for (const auto& [at, data] : chunks) std::copy(data.begin(), data.end(), flat.bytes.begin() + (at - start));
    return flat;
}

}  // namespace cb::build::z80_asm
