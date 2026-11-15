// cb-unity: alone
#include "cb/build/byte_asm.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "cb/build/asm_text.hpp"
#include "cb/core/text.hpp"

namespace cb::build::byte_asm {

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

bool is_directive(std::string_view k) { return k == "ORG" || k == "DB" || k == "DW" || k == "FILL" || k == "EQU" || k == "="; }

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
        std::string_view rest = text::trim(line);
        const std::size_t colon = rest.find(':');
        const std::size_t space = rest.find_first_of(" \t");
        if (colon != std::string_view::npos && (space == std::string_view::npos || colon < space)) {   // `name:`
            l.label = std::string(rest.substr(0, colon));
            rest = text::trim(rest.substr(colon + 1));
        } else if (!std::isspace(static_cast<unsigned char>(line.front()))) {   // a name in the first column
            l.label = std::string(rest.substr(0, space));
            rest = space == std::string_view::npos ? std::string_view() : text::trim(rest.substr(space));
        }
        if (!rest.empty()) {
            const std::size_t sp = rest.find_first_of(" \t");
            const std::string word = text::upper(sp == std::string_view::npos ? rest : rest.substr(0, sp));
            l.keyword = is_directive(word) ? word : std::string();
            l.instruction = !is_directive(word);
            l.operand = std::string(is_directive(word) ? (sp == std::string_view::npos ? std::string_view() : text::trim(rest.substr(sp + 1))) : rest);
        }
        out.push_back(std::move(l));
    }
    return out;
}

bool is_string(std::string_view item) { return item.size() >= 2 && item.front() == '"' && item.back() == '"'; }

Bytes directive_bytes(const Line& l, const NameLabels& labels, bool lenient, const Options& o) {
    Bytes out;
    const auto number = [&](std::string_view e) { return o.eval(e, labels, lenient); };
    if (l.keyword == "DB") {
        for (const std::string_view item : split_items(l.operand, false)) {
            if (is_string(item)) {
                for (const char c : item.substr(1, item.size() - 2)) out.push_back(static_cast<std::uint8_t>(c));
            } else {
                out.push_back(static_cast<std::uint8_t>(number(item)));
            }
        }
    } else if (l.keyword == "DW") {
        for (const std::string_view item : split_items(l.operand, false)) {
            const std::int64_t v = number(item);
            const std::uint8_t lo = static_cast<std::uint8_t>(v), hi = static_cast<std::uint8_t>(v >> 8);
            if (o.big_endian_words) { out.push_back(hi); out.push_back(lo); }
            else { out.push_back(lo); out.push_back(hi); }
        }
    } else if (l.keyword == "FILL") {
        const std::vector<std::string_view> parts = split_items(l.operand, false);
        if (parts.size() != 2) throw AsmError(l.number, l.text, "FILL count, value");
        out.assign(static_cast<std::size_t>(number(parts[0])), static_cast<std::uint8_t>(number(parts[1])));
    }
    return out;
}

}  // namespace

Image build(std::string_view source, const Options& o) {
    const std::vector<Line> lines = parse_lines(source);
    NameLabels labels;
    std::uint32_t pc = 0;
    try {
        for (const Line& l : lines) {
            if (l.keyword == "EQU" || l.keyword == "=") {
                if (l.label.empty()) throw AsmError(l.number, l.text, "EQU needs a name");
                labels[l.label] = o.eval(l.operand, labels, true);
                continue;
            }
            if (!l.label.empty()) labels[l.label] = pc;
            if (l.keyword.empty() && !l.instruction) continue;
            if (l.keyword == "ORG") pc = static_cast<std::uint32_t>(o.eval(l.operand, labels, true));
            else if (l.instruction) pc += static_cast<std::uint32_t>(o.assemble(l.operand, pc, labels, true).size());
            else pc += static_cast<std::uint32_t>(directive_bytes(l, labels, true, o).size());
        }
    } catch (const AsmError&) {
        throw;
    } catch (const Error& e) {
        throw AsmError(0, "", e.what());
    }
    Image image;
    bool started = false;
    pc = 0;
    for (const Line& l : lines) {
        try {
            if (l.keyword == "EQU" || l.keyword == "=" || (l.keyword.empty() && !l.instruction)) continue;
            if (l.keyword == "ORG") {
                pc = static_cast<std::uint32_t>(o.eval(l.operand, labels, false));
                if (!started) {
                    image.base = pc;
                    started = true;
                } else if (pc < image.base) {
                    throw AsmError(l.number, l.text, "an address before the start of the image");
                }
                continue;
            }
            if (!started) {
                image.base = pc;
                started = true;
            }
            const Bytes b = l.instruction ? o.assemble(l.operand, pc, labels, false) : directive_bytes(l, labels, false, o);
            const std::size_t at = pc - image.base;
            if (at + b.size() > (std::size_t{1} << 24)) throw AsmError(l.number, l.text, "past the end of the address space");
            if (image.data.size() < at + b.size()) image.data.resize(at + b.size(), 0);
            std::copy(b.begin(), b.end(), image.data.begin() + static_cast<std::ptrdiff_t>(at));
            pc += static_cast<std::uint32_t>(b.size());
        } catch (const AsmError&) {
            throw;
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    }
    return image;
}

}  // namespace cb::build::byte_asm
