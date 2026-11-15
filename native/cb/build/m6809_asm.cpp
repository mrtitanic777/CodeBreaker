// cb-unity: alone
#include "cb/build/m6809_asm.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "cb/build/asm_text.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/m6809.hpp"

namespace cb::build::m6809_asm {

namespace dm = decompile::m6809;
using NameLabels = decompile::NameLabels;

namespace {

struct Line {
    std::uint32_t number = 0;
    std::string text;      // the original line, for messages
    std::string label;
    std::string keyword;   // upper case: a directive or a mnemonic; empty for a label alone
    std::string operand;
    bool instruction = false;
};

bool is_directive(std::string_view k) { return k == "ORG" || k == "FCB" || k == "FDB" || k == "FCC" || k == "FILL" || k == "RMB" || k == "EQU" || k == "="; }

std::vector<Line> parse_lines(std::string_view source) {
    std::vector<Line> out;
    std::uint32_t number = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++number;
        std::string_view line = strip_comment(raw);
        if (!raw.empty() && raw.front() == '*') line = {};
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

// The bytes a directive writes, given the labels known.
Bytes directive_bytes(const Line& l, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    Bytes out;
    const auto number = [&](std::string_view e) { return dm::value(e, labels, pc, lenient); };
    if (l.keyword == "FCB") {
        for (const std::string_view item : split_items(l.operand, false)) {
            if (item.size() >= 3 && item.front() == '"' && item.back() == '"') {   // a string's bytes
                for (const char c : item.substr(1, item.size() - 2)) out.push_back(static_cast<std::uint8_t>(c));
            } else if (item.size() > 4 && item.front() == '"' && item[2] == '"') {   // "A"+$80
                out.push_back(static_cast<std::uint8_t>(item[1] + number(item.substr(4))));
            } else {
                out.push_back(static_cast<std::uint8_t>(number(item)));
            }
        }
    } else if (l.keyword == "FDB") {
        for (const std::string_view item : split_items(l.operand, false)) {
            const std::int64_t v = number(item);
            out.push_back(static_cast<std::uint8_t>(v >> 8));
            out.push_back(static_cast<std::uint8_t>(v));
        }
    } else if (l.keyword == "FCC") {
        const std::string_view o = l.operand;
        if (o.size() < 2 || o.front() != o.back()) throw AsmError(l.number, l.text, "FCC needs a delimited string");
        for (const char c : o.substr(1, o.size() - 2)) out.push_back(static_cast<std::uint8_t>(c));
    } else if (l.keyword == "FILL") {
        const std::vector<std::string_view> parts = split_items(l.operand, false);
        if (parts.size() != 2) throw AsmError(l.number, l.text, "FILL count, value");
        out.assign(static_cast<std::size_t>(number(parts[0])), static_cast<std::uint8_t>(number(parts[1])));
    } else if (l.keyword == "RMB") {
        out.assign(static_cast<std::size_t>(number(l.operand)), 0);
    }
    return out;
}

}  // namespace

Bytes build(std::string_view source) {
    const std::vector<Line> lines = parse_lines(source);
    NameLabels labels;
    // pass 1: addresses and constants
    std::uint32_t pc = 0;
    try {
        for (const Line& l : lines) {
            if (l.keyword == "EQU" || l.keyword == "=") {
                if (l.label.empty()) throw AsmError(l.number, l.text, "EQU needs a name");
                labels[l.label] = dm::value(l.operand, labels, pc, true);
                continue;
            }
            if (!l.label.empty()) labels[l.label] = pc;
            if (l.keyword.empty()) continue;
            if (l.keyword == "ORG") pc = static_cast<std::uint32_t>(dm::value(l.operand, labels, pc, true));
            else if (l.instruction) pc += static_cast<std::uint32_t>(dm::assemble(l.keyword + " " + l.operand, pc, labels, true).size());
            else pc += static_cast<std::uint32_t>(directive_bytes(l, pc, labels, true).size());
        }
    } catch (const AsmError&) {
        throw;
    } catch (const Error& e) {
        throw AsmError(0, "", e.what());
    }
    // pass 2: the bytes
    Bytes image(0x10000, 0);
    std::size_t end = 0;
    pc = 0;
    for (const Line& l : lines) {
        try {
            if (l.keyword.empty() || l.keyword == "EQU" || l.keyword == "=") continue;
            if (l.keyword == "ORG") {
                pc = static_cast<std::uint32_t>(dm::value(l.operand, labels, pc, false));
                continue;
            }
            const Bytes b = l.instruction ? dm::assemble(l.keyword + " " + l.operand, pc, labels, false) : directive_bytes(l, pc, labels, false);
            if (pc + b.size() > image.size()) throw AsmError(l.number, l.text, "past the end of the 64 KB address space");
            std::copy(b.begin(), b.end(), image.begin() + pc);
            pc += static_cast<std::uint32_t>(b.size());
            end = std::max<std::size_t>(end, pc);
        } catch (const AsmError&) {
            throw;
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    }
    image.resize(end);
    return image;
}

}  // namespace cb::build::m6809_asm
