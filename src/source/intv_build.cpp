// cb-unity: alone
#include "source/intv_build.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "build/asm_text.hpp"
#include "core/text.hpp"
#include "source/dialect.hpp"
#include "source/processors.hpp"
#include "formats/intv.hpp"

namespace cb::source::intv_build {

using build::AsmError;
using build::Labels;
using build::split_items;
using build::strip_comment;

namespace {

struct Line {
    std::uint32_t number = 0;
    std::string text;
    std::string label;
    std::string keyword;   // upper case: a directive or a mnemonic; empty for a label alone
    std::string operand;
    bool instruction = false;
};

bool is_directive(std::string_view k) { return k == "SEGMENT" || k == "ORG" || k == "DECLE" || k == "FILL" || k == "ROMTAIL" || k == "FORMAT" || k == "EQU" || k == "="; }

std::vector<Line> parse_lines(std::string_view source) {
    std::vector<Line> out;
    std::uint32_t number = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++number;
        std::string_view line = strip_comment(raw);
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) line.remove_suffix(1);
        if (line.empty()) continue;
        Line l;
        l.number = number;
        l.text = std::string(raw);
        std::string_view rest = line;
        if (!std::isspace(static_cast<unsigned char>(line.front()))) {
            std::size_t end = 0;
            while (end < line.size() && !std::isspace(static_cast<unsigned char>(line[end])) && line[end] != ':') ++end;
            l.label = std::string(line.substr(0, end));
            rest = line.substr(end);
            if (!rest.empty() && rest.front() == ':') rest.remove_prefix(1);
        }
        rest = text::trim(rest);
        if (l.label.empty() && !rest.empty()) {   // (the dialect indents every line: `NAME EQU value` and `NAME = value` are the constants of a line that began in the first column)
            const std::size_t w_end = rest.find_first_of(" 	");
            if (w_end != std::string_view::npos) {
                const std::string_view after = text::ltrim(rest.substr(w_end));
                if (text::istarts_with(after, "EQU ") || after.starts_with("= ")) {
                    l.label = std::string(rest.substr(0, w_end));
                    rest = after;
                }
            }
        }
        if (l.label.empty() && !rest.empty()) {   // an indented label: `name:`
            const std::size_t word_end = rest.find_first_of(" \t");
            const std::string_view word = rest.substr(0, word_end);
            if (word.size() > 1 && word.back() == ':') {
                l.label = std::string(word.substr(0, word.size() - 1));
                rest = word_end == std::string_view::npos ? std::string_view() : text::trim(rest.substr(word_end));
            }
        }
        if (!rest.empty()) {
            const std::size_t space = rest.find_first_of(" \t");
            l.keyword = text::upper(space == std::string_view::npos ? rest : rest.substr(0, space));
            l.operand = std::string(space == std::string_view::npos ? std::string_view() : text::trim(rest.substr(space + 1)));
            l.instruction = !is_directive(l.keyword);
        }
        out.push_back(std::move(l));
    }
    return out;
}

const build::Encoder& encoder() { return processors::described_encoder(processors::find("cp1610")); }

// A number or label expression; a label not defined yet is 0 on the first pass (`lenient`).
std::int64_t value(std::string_view expr, const Labels& labels, std::int64_t pc, bool lenient) {
    const Result<std::int64_t> r = encoder().value(expr, labels, pc);
    if (r.ok()) return r.value();
    if (lenient && build::is_undefined(r.error())) return 0;
    throw Error(r.error().what());
}

// The decles of one instruction line (an instruction after SDBD is double-byte). On the first pass a label not defined yet stands for a place
// that is near and a place that is small: whichever the instruction takes.
std::vector<std::uint16_t> assemble(const std::string& line, std::uint32_t pc, const Labels& labels, bool lenient, bool sdbd) {
    const std::string context = sdbd ? "doublebyte=1" : "";
    Labels local = labels;
    Result<Bytes> r = encoder().assemble_in(context, line, pc, local);
    for (const std::int64_t guess : {static_cast<std::int64_t>(pc) + 8, std::int64_t{0x10}, std::int64_t{0}}) {
        if (!lenient) break;
        while (!r.ok() && build::is_undefined(r.error())) {
            const std::string name = static_cast<const build::UndefinedSymbolError&>(r.error()).name();
            if (local.contains(name)) break;
            local[name] = guess;
            r = encoder().assemble_in(context, line, pc, local);
        }
        if (r.ok()) break;
        local = labels;
        r = encoder().assemble_in(context, line, pc, local);
    }
    if (!r.ok()) throw Error(r.error().what());
    const Bytes& b = r.value();
    if (b.size() % 2 != 0) throw Error("an instruction of an odd number of bytes");
    std::vector<std::uint16_t> out;
    for (std::size_t i = 0; i + 1 < b.size(); i += 2) out.push_back(static_cast<std::uint16_t>(((b[i] << 8) | b[i + 1]) & 0x3FF));
    return out;
}

// The words a directive writes (DECLE, FILL), given the labels known.
std::vector<std::uint16_t> directive_words(const Line& l, const Labels& labels, bool lenient) {
    std::vector<std::uint16_t> out;
    const auto number = [&](std::string_view e) { return value(e, labels, 0, lenient); };
    if (l.keyword == "DECLE") {
        for (const std::string_view item : split_items(l.operand, false)) out.push_back(static_cast<std::uint16_t>(number(item)));
    } else if (l.keyword == "FILL") {
        const std::vector<std::string_view> parts = split_items(l.operand, false);
        if (parts.size() != 2) throw AsmError(l.number, l.text, "FILL count, value");
        out.assign(static_cast<std::size_t>(number(parts[0])), static_cast<std::uint16_t>(number(parts[1])));
    }
    return out;
}

}  // namespace

// One instruction line's decles. NOP and SIN leave the low bit of their word free (the description says neither has an operand): as1600's
// `NOP 1` and `SIN 1` are that bit.
std::vector<std::uint16_t> instruction_words(const Line& l, std::uint32_t pc, const Labels& labels, bool lenient, bool sdbd) {
    if ((l.keyword == "NOP" || l.keyword == "SIN") && !l.operand.empty()) {
        const std::int64_t bit = value(l.operand, labels, 0, lenient);
        if (bit != 0 && bit != 1) throw Error(l.keyword + " takes 0 or 1");
        std::vector<std::uint16_t> w = assemble(l.keyword, pc, labels, lenient, sdbd);
        w.at(0) |= static_cast<std::uint16_t>(bit);
        return w;
    }
    return assemble(l.keyword + " " + l.operand, pc, labels, lenient, sdbd);
}

Bytes build(std::string_view source) {
    const std::vector<Line> lines = parse_lines(dialect::find("as1600").parse(source));
    Labels labels;
    // pass 1: addresses and constants
    std::uint32_t pc = 0;
    bool sdbd = false;
    try {
        for (const Line& l : lines) {
            if (l.keyword == "EQU" || l.keyword == "=") {
                if (l.label.empty()) throw AsmError(l.number, l.text, "EQU needs a name");
                labels[l.label] = value(l.operand, labels, 0, true);
                continue;
            }
            if (!l.label.empty()) labels[l.label] = pc;
            if (l.keyword.empty()) continue;
            if (l.keyword == "SEGMENT") {
                const auto parts = split_items(l.operand, false);
                if (parts.size() != 2) throw AsmError(l.number, l.text, "SEGMENT first, last");
                pc = static_cast<std::uint32_t>(value(parts[0], labels, 0, true)) << 8;
            } else if (l.keyword == "ORG") {
                pc = static_cast<std::uint32_t>(value(l.operand, labels, 0, true));
            } else if (l.keyword == "ROMTAIL" || l.keyword == "FORMAT") {
            } else if (l.instruction) {
                const std::vector<std::uint16_t> w = instruction_words(l, pc, labels, true, sdbd);
                pc += static_cast<std::uint32_t>(w.size());
                sdbd = l.keyword == "SDBD";
            } else {
                pc += static_cast<std::uint32_t>(directive_words(l, labels, true).size());
            }
        }
    } catch (const AsmError&) {
        throw;
    } catch (const Error& e) {
        throw AsmError(0, "", e.what());
    }
    // pass 2: the words
    formats::intv::Rom rom;
    formats::intv::Segment* segment = nullptr;
    pc = 0;
    sdbd = false;
    for (const Line& l : lines) {
        try {
            if (l.keyword.empty() || l.keyword == "EQU" || l.keyword == "=") continue;
            if (l.keyword == "FORMAT") {
                if (text::upper(l.operand) != "RAW" && text::upper(l.operand) != "ROM") throw AsmError(l.number, l.text, "FORMAT RAW or ROM");
                rom.raw = text::upper(l.operand) == "RAW";
                continue;
            }
            if (l.keyword == "ROMTAIL") {
                for (const std::string_view item : split_items(l.operand, false)) rom.tail.push_back(static_cast<std::uint8_t>(value(item, labels, 0, false)));
                continue;
            }
            if (l.keyword == "SEGMENT") {
                const auto parts = split_items(l.operand, false);
                const std::int64_t first = value(parts[0], labels, 0, false), last = value(parts[1], labels, 0, false);
                if (first < 0 || last > 0xFF || last < first) throw AsmError(l.number, l.text, "a segment's pages are $00 to $FF, first before last");
                formats::intv::Segment s;
                s.start = static_cast<std::uint32_t>(first) << 8;
                s.words.assign(static_cast<std::size_t>(last - first + 1) * 256, 0);
                rom.segments.push_back(std::move(s));
                segment = &rom.segments.back();
                pc = segment->start;
                continue;
            }
            if (l.keyword == "ORG") {
                pc = static_cast<std::uint32_t>(value(l.operand, labels, 0, false));
                continue;
            }
            const std::vector<std::uint16_t> w = l.instruction ? instruction_words(l, pc, labels, false, sdbd) : directive_words(l, labels, false);
            if (l.instruction) sdbd = l.keyword == "SDBD";
            if (segment == nullptr || pc < segment->start || pc + w.size() > segment->end()) throw AsmError(l.number, l.text, "outside the segment");
            // (a vector's data may move when a later SEGMENT is added: `segment` points into the last element, which is stable until then)
            std::copy(w.begin(), w.end(), segment->words.begin() + (pc - segment->start));
            pc += static_cast<std::uint32_t>(w.size());
        } catch (const AsmError&) {
            throw;
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    }
    return rom.serialize(true);
}

std::vector<std::uint16_t> assemble_line(std::string_view line, std::uint32_t pc, const std::unordered_map<std::string, std::int64_t>& labels, bool sdbd) {
    const std::vector<Line> lines = parse_lines(dialect::find("as1600").parse(line));
    if (lines.size() != 1 || !lines.front().instruction) throw AsmError(0, line, "one instruction");
    Labels known;
    for (const auto& [name, v] : labels) known[name] = v;
    try {
        return instruction_words(lines.front(), pc, known, false, sdbd);
    } catch (const AsmError&) {
        throw;
    } catch (const Error& e) {
        throw AsmError(0, line, e.what());
    }
}

}  // namespace cb::source::intv_build
