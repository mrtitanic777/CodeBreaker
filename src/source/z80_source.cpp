// source/z80_source.cpp -- see z80_source.hpp.

#include "source/z80_source.hpp"

#include <cctype>
#include <format>
#include <optional>

#include "core/error.hpp"
#include "core/text.hpp"
#include "source/described_source.hpp"
#include "source/processors.hpp"

namespace cb::source::z80_source {
namespace {

namespace z80 = decompile::z80;
namespace iw = image_writer;

const processors::Processor& z80_cpu() {
    static const processors::Processor& cpu = processors::find("z80");
    return cpu;
}

bool z80s_is_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }
bool z80s_is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

// `text` with its last number 0x<value> written as `name`. (A number that is the operand of the instruction: the description writes each as 0x<hex>.)
std::string z80s_name_number(const std::string& text, std::uint32_t value, const std::string& name, bool in_parentheses) {
    std::size_t found = std::string::npos, found_end = 0;
    for (std::size_t i = 0; i + 2 < text.size(); ++i) {
        if (text[i] != '0' || text[i + 1] != 'x' || (i > 0 && z80s_is_word(text[i - 1]))) continue;
        std::size_t e = i + 2;
        while (e < text.size() && z80s_is_hex(text[e])) ++e;
        if (e == i + 2 || (e < text.size() && z80s_is_word(text[e]))) continue;
        if (in_parentheses && !(i > 0 && text[i - 1] == '(' && e < text.size() && text[e] == ')')) continue;
        const std::optional<std::uint32_t> n = text::parse_int<std::uint32_t>(std::string_view(text).substr(i + 2, e - i - 2), 16);
        if (n && *n == value) found = i, found_end = e;
        i = e - 1;
    }
    if (found == std::string::npos) return text;
    return text.substr(0, found) + name + text.substr(found_end);
}

// The instruction's bytes as the description reads and assembles them again, when they come out the same.
std::optional<std::string> z80s_described(const z80::Insn& insn) {
    const std::vector<processors::Line> lines = processors::read_described(z80_cpu(), insn.bytes(), insn.pc, 1);
    if (lines.empty() || lines[0].bytes.size() != insn.size || lines[0].text.starts_with(".d")) return std::nullopt;
    const std::string& text = lines[0].text;
    try {
        const processors::Image again = processors::assemble_described(z80_cpu(), std::format("\t.org 0x{:x}\n\t{}\n", insn.pc, text), {});
        if (again.data.size() != insn.size || !std::equal(again.data.begin(), again.data.end(), insn.bytes().begin())) return std::nullopt;
    } catch (const ParseError&) {
        return std::nullopt;
    }
    return text;
}

}  // namespace

std::string code(const z80::Insn& insn, const decompile::AddressLabels& names, const iw::Names& ports) {
    return code(insn, names, ports, dialect::find("sleigh"));
}

std::string code(const z80::Insn& insn, const decompile::AddressLabels& names, const iw::Names& ports, const dialect::Dialect& dialect) {
    std::optional<std::string> described = z80s_described(insn);
    if (!described) {
        std::string bytes;
        for (const std::uint8_t b : insn.bytes()) bytes += std::format("{}0x{:02x}", bytes.empty() ? "" : ",", b);
        const std::vector<processors::Line> lines = processors::read_described(z80_cpu(), insn.bytes(), insn.pc, 1);
        const std::string said = !lines.empty() && !lines[0].text.starts_with(".d") ? lines[0].text : z80::plain_text(insn);
        return dialect.line(".byte " + bytes) + "  ; " + dialect.instruction(said);
    }
    std::string out = std::move(*described);
    // the address operand (an nn or an e of the pattern) by its name, the port of IN A,(n) and OUT (n),A by its
    std::size_t k = 0;
    const std::string& pattern = insn.pattern;
    for (std::size_t i = 0; i < pattern.size();) {
        if (!z80s_is_word(pattern[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < pattern.size() && z80s_is_word(pattern[end])) ++end;
        const std::string_view w = std::string_view(pattern).substr(i, end - i);
        i = end;
        if (w != "nn" && w != "n" && w != "d" && w != "e") continue;
        if (k >= insn.value_count) break;
        const std::uint32_t v = static_cast<std::uint32_t>(insn.values[k++]);
        if (w == "nn" || w == "e") {
            if (const auto named = names.find(v); named != names.end()) out = z80s_name_number(out, v, named->second, false);
        } else if (w == "n" && (pattern == "OUT (n),A" || pattern == "IN A,(n)")) {
            if (const auto named = ports.find(v); named != ports.end()) out = z80s_name_number(out, v, named->second, true);
        }
    }
    return dialect.instruction(out);
}

std::optional<std::uint32_t> forward_reference(const z80::Insn& insn, const decompile::AddressLabels& names, const std::set<std::uint32_t>& constants) {
    const std::string& pattern = insn.pattern;
    const std::size_t space = pattern.find(' ');
    if (space == std::string::npos) return std::nullopt;
    std::size_t k = 0;
    for (std::size_t i = space; i < pattern.size();) {
        if (!z80s_is_word(pattern[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < pattern.size() && z80s_is_word(pattern[end])) ++end;
        const std::string_view w = std::string_view(pattern).substr(i, end - i);
        if (w == "nn" || w == "e") {
            if (k >= insn.value_count) return std::nullopt;
            const std::uint32_t v = static_cast<std::uint32_t>(insn.values[k]);
            const bool behind_a_word = pattern.substr(space, i - space).find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos;
            if (behind_a_word && v > insn.pc && names.contains(v) && !constants.contains(v)) return v;
            return std::nullopt;
        }
        if (w == "n" || w == "d") ++k;
        i = end;
    }
    return std::nullopt;
}

std::vector<std::string> data_lines(ByteView data) {
    return described_source::data_lines(data);
}

std::vector<std::string> data_lines(ByteView data, const dialect::Dialect& dialect) { return dialect.data_lines(data); }

std::string equ(const std::string& name, std::uint32_t value, unsigned digits) { return described_source::equ(name, value, digits); }

std::string equ(const std::string& name, std::uint32_t value, unsigned digits, const dialect::Dialect& dialect) {
    return dialect.line(described_source::equ(name, value, digits));
}

Bytes assemble(std::string_view source, const std::filesystem::path& folder) { return processors::assemble_described(z80_cpu(), source, folder).data; }

Bytes assemble(std::string_view source, const std::filesystem::path& folder, const dialect::Dialect& dialect) {
    return processors::assemble_described(z80_cpu(), dialect.parse(source), folder).data;
}

}  // namespace cb::source::z80_source
