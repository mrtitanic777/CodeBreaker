// cb-unity: alone
#include "build/v810_asm.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "build/asm_text.hpp"
#include "core/text.hpp"
#include "decompile/v810.hpp"

namespace cb::build::v810_asm {

namespace dv = decompile::v810;
using NameLabels = decompile::NameLabels;

namespace {

struct Line {
    std::uint32_t number = 0;
    std::string text;      // the original line, for messages
    std::string label;
    std::string keyword;   // lower case: a directive or a mnemonic; empty for a label alone
    std::string operand;
    bool instruction = false;
};

bool is_directive(std::string_view k) { return k.starts_with('.'); }

std::vector<Line> parse_lines(std::string_view source) {
    std::vector<Line> out;
    std::uint32_t number = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++number;
        std::string_view line = strip_comment(raw);
        if (!raw.empty() && raw.front() == '#') line = {};
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) line.remove_suffix(1);
        if (line.empty()) continue;
        Line l;
        l.number = number;
        l.text = std::string(raw);
        std::string_view rest = text::trim(line);
        // a label: a first word ending in `:`, or a word in the first column that is not a directive
        const std::size_t colon = rest.find(':');
        const std::size_t space = rest.find_first_of(" \t");
        if (colon != std::string_view::npos && (space == std::string_view::npos || colon < space)) {
            l.label = std::string(rest.substr(0, colon));
            rest = text::trim(rest.substr(colon + 1));
        } else if (!std::isspace(static_cast<unsigned char>(line.front())) && !is_directive(rest)) {
            l.label = std::string(rest.substr(0, space));
            rest = space == std::string_view::npos ? std::string_view() : text::trim(rest.substr(space));
        }
        if (!rest.empty()) {
            const std::size_t sp = rest.find_first_of(" \t");
            l.keyword = text::lower(sp == std::string_view::npos ? rest : rest.substr(0, sp));
            l.operand = std::string(sp == std::string_view::npos ? std::string_view() : text::trim(rest.substr(sp + 1)));
            l.instruction = !is_directive(l.keyword);
        }
        out.push_back(std::move(l));
    }
    return out;
}

void push_string(Bytes& out, std::string_view item) {
    for (const char c : item.substr(1, item.size() - 2)) out.push_back(static_cast<std::uint8_t>(c));
}

bool is_string(std::string_view item) { return item.size() >= 2 && item.front() == '"' && item.back() == '"'; }

// The bytes a directive writes, given the labels known.
Bytes directive_bytes(const Line& l, const NameLabels& labels, bool lenient) {
    Bytes out;
    const auto number = [&](std::string_view e) { return dv::value(e, labels, lenient); };
    if (l.keyword == ".byte") {
        for (const std::string_view item : split_items(l.operand, false)) {
            if (is_string(item)) push_string(out, item);
            else out.push_back(static_cast<std::uint8_t>(number(item)));
        }
    } else if (l.keyword == ".hword") {
        for (const std::string_view item : split_items(l.operand, false)) {
            const std::int64_t v = number(item);
            out.push_back(static_cast<std::uint8_t>(v));
            out.push_back(static_cast<std::uint8_t>(v >> 8));
        }
    } else if (l.keyword == ".word") {
        for (const std::string_view item : split_items(l.operand, false)) {
            const std::int64_t v = number(item);
            for (unsigned k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
        }
    } else if (l.keyword == ".ascii") {
        if (!is_string(l.operand)) throw AsmError(l.number, l.text, ".ascii needs a string");
        push_string(out, l.operand);
    } else if (l.keyword == ".fill") {
        const std::vector<std::string_view> parts = split_items(l.operand, false);
        if (parts.size() != 2) throw AsmError(l.number, l.text, ".fill count, value");
        out.assign(static_cast<std::size_t>(number(parts[0])), static_cast<std::uint8_t>(number(parts[1])));
    } else if (l.keyword != ".org" && l.keyword != ".equ" && l.keyword != ".set") {
        throw AsmError(l.number, l.text, "unknown directive");
    }
    return out;
}

// `.equ NAME, value`
std::pair<std::string, std::string> equate(const Line& l) {
    const std::vector<std::string_view> parts = split_items(l.operand, false);
    if (parts.size() != 2) throw AsmError(l.number, l.text, ".equ NAME, value");
    return {std::string(parts[0]), std::string(parts[1])};
}

}  // namespace

Image build(std::string_view source) {
    const std::vector<Line> lines = parse_lines(source);
    NameLabels labels;
    // pass 1: addresses and constants
    std::uint32_t pc = 0;
    try {
        for (const Line& l : lines) {
            if (!l.label.empty()) labels[l.label] = pc;
            if (l.keyword.empty()) continue;
            if (l.keyword == ".equ" || l.keyword == ".set") {
                const auto [name, expr] = equate(l);
                labels[name] = dv::value(expr, labels, true);
            } else if (l.keyword == ".org") {
                pc = static_cast<std::uint32_t>(dv::value(l.operand, labels, true));
            } else if (l.instruction) {
                pc += static_cast<std::uint32_t>(dv::assemble(l.keyword + " " + l.operand, pc, labels, true).size());
            } else {
                pc += static_cast<std::uint32_t>(directive_bytes(l, labels, true).size());
            }
        }
    } catch (const AsmError&) {
        throw;
    } catch (const Error& e) {
        throw AsmError(0, "", e.what());
    }
    // pass 2: the bytes
    Image image;
    bool started = false;
    pc = 0;
    for (const Line& l : lines) {
        try {
            if (l.keyword.empty() || l.keyword == ".equ" || l.keyword == ".set") continue;
            if (l.keyword == ".org") {
                pc = static_cast<std::uint32_t>(dv::value(l.operand, labels, false));
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
            const Bytes b = l.instruction ? dv::assemble(l.keyword + " " + l.operand, pc, labels, false) : directive_bytes(l, labels, false);
            const std::size_t at = pc - image.base;
            if (at + b.size() > (std::size_t{1} << 26)) throw AsmError(l.number, l.text, "past the end of the address space (64 MB image)");
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

}  // namespace cb::build::v810_asm
