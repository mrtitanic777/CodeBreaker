// cb-unity: alone
#include "build/cp1610_asm.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "build/asm_text.hpp"
#include "core/text.hpp"
#include "decompile/cp1610.hpp"
#include "formats/intv.hpp"

namespace cb::build::cp1610_asm {

namespace dc = decompile::cp1610;
using NameLabels = decompile::NameLabels;

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

// The words a directive writes (DECLE, FILL), given the labels known.
std::vector<std::uint16_t> directive_words(const Line& l, const NameLabels& labels, bool lenient) {
    std::vector<std::uint16_t> out;
    const auto number = [&](std::string_view e) { return dc::value(e, labels, lenient); };
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

Bytes build(std::string_view source) {
    const std::vector<Line> lines = parse_lines(source);
    NameLabels labels;
    // pass 1: addresses and constants
    std::uint32_t pc = 0;
    bool sdbd = false;
    try {
        for (const Line& l : lines) {
            if (l.keyword == "EQU" || l.keyword == "=") {
                if (l.label.empty()) throw AsmError(l.number, l.text, "EQU needs a name");
                labels[l.label] = dc::value(l.operand, labels, true);
                continue;
            }
            if (!l.label.empty()) labels[l.label] = pc;
            if (l.keyword.empty()) continue;
            if (l.keyword == "SEGMENT") {
                const auto parts = split_items(l.operand, false);
                if (parts.size() != 2) throw AsmError(l.number, l.text, "SEGMENT first, last");
                pc = static_cast<std::uint32_t>(dc::value(parts[0], labels, true)) << 8;
            } else if (l.keyword == "ORG") {
                pc = static_cast<std::uint32_t>(dc::value(l.operand, labels, true));
            } else if (l.keyword == "ROMTAIL" || l.keyword == "FORMAT") {
            } else if (l.instruction) {
                const std::vector<std::uint16_t> w = dc::assemble(l.keyword + " " + l.operand, pc, labels, true, sdbd);
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
                for (const std::string_view item : split_items(l.operand, false)) rom.tail.push_back(static_cast<std::uint8_t>(dc::value(item, labels, false)));
                continue;
            }
            if (l.keyword == "SEGMENT") {
                const auto parts = split_items(l.operand, false);
                const std::int64_t first = dc::value(parts[0], labels, false), last = dc::value(parts[1], labels, false);
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
                pc = static_cast<std::uint32_t>(dc::value(l.operand, labels, false));
                continue;
            }
            const std::vector<std::uint16_t> w = l.instruction ? dc::assemble(l.keyword + " " + l.operand, pc, labels, false, sdbd) : directive_words(l, labels, false);
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

}  // namespace cb::build::cp1610_asm
