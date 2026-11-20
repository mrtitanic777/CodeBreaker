// source/described_source.cpp -- see described_source.hpp.

#include "source/described_source.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "core/error.hpp"
#include "core/text.hpp"

namespace cb::source::described_source {
namespace {

bool is_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }
bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

std::vector<processors::Line> read(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, const Context& context) {
    return context.empty() ? processors::read_described(cpu, bytes, pc, 1) : processors::read_described(cpu, bytes, pc, 1, context);
}

}  // namespace

std::optional<std::string> checked_text(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, std::size_t size, const Context& context) {
    const std::vector<processors::Line> lines = read(cpu, bytes, pc, context);
    if (lines.empty() || lines[0].bytes.size() != size || lines[0].text.starts_with(".d")) return std::nullopt;
    const std::string& text = lines[0].text;
    try {
        const std::string source = std::format("\t.org 0x{:x}\n{}\t{}\n", pc, context.empty() ? "" : "\t" + context_line(context) + "\n", text);
        const processors::Image again = processors::assemble_described(cpu, source, {});
        if (again.data.size() != size || !std::equal(again.data.begin(), again.data.end(), bytes.begin())) return std::nullopt;
    } catch (const ParseError&) {
        return std::nullopt;
    }
    return text;
}

std::string byte_line(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, const Context& context, std::string_view what) {
    std::string out;
    for (const std::uint8_t b : bytes) out += std::format("{}0x{:02x}", out.empty() ? "" : ",", b);
    const std::vector<processors::Line> lines = read(cpu, bytes, pc, context);
    const std::string said = !lines.empty() && !lines[0].text.starts_with(".d") ? lines[0].text : std::string(what);
    return ".byte " + out + "  ; " + said;
}

std::string name_numbers(const std::string& text, const decompile::AddressLabels& names) {
    std::string out;
    for (std::size_t i = 0; i < text.size();) {
        if (text[i] == '0' && i + 2 < text.size() && text[i + 1] == 'x' && (i == 0 || (!is_word(text[i - 1]) && text[i - 1] != '#'))) {
            std::size_t e = i + 2;
            while (e < text.size() && is_hex(text[e])) ++e;
            if (e > i + 2 && !(e < text.size() && is_word(text[e]))) {
                const std::optional<std::uint32_t> n = text::parse_int<std::uint32_t>(std::string_view(text).substr(i + 2, e - i - 2), 16);
                if (n) {
                    if (const auto named = names.find(*n); named != names.end()) {
                        out += named->second;
                        i = e;
                        continue;
                    }
                }
            }
        }
        out += text[i++];
    }
    return out;
}

std::string name_number(const std::string& text, std::uint32_t value, const std::string& name, bool in_parentheses) {
    std::size_t found = std::string::npos, found_end = 0;
    for (std::size_t i = 0; i + 2 < text.size(); ++i) {
        if (text[i] != '0' || text[i + 1] != 'x' || (i > 0 && is_word(text[i - 1]))) continue;
        std::size_t e = i + 2;
        while (e < text.size() && is_hex(text[e])) ++e;
        if (e == i + 2 || (e < text.size() && is_word(text[e]))) continue;
        if (in_parentheses && !(i > 0 && text[i - 1] == '(' && e < text.size() && text[e] == ')')) continue;
        const std::optional<std::uint32_t> n = text::parse_int<std::uint32_t>(std::string_view(text).substr(i + 2, e - i - 2), 16);
        if (n && *n == value) found = i, found_end = e;
        i = e - 1;
    }
    if (found == std::string::npos) return text;
    return text.substr(0, found) + name + text.substr(found_end);
}

std::string context_line(const Context& context) {
    std::string out = ".context";
    for (const auto& [name, value] : context) out += std::format(" {}={}", name, value);
    return out;
}

std::vector<std::string> data_lines(ByteView data) {
    const auto printable = [](std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22 && b != 0x5C && b != 0x3B; };
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < data.size()) {
        std::size_t run = 1;
        while (i + run < data.size() && data[i + run] == data[i]) ++run;
        if (run >= 16) {
            out.push_back(std::format("        .space {}, 0x{:02x}", run, data[i]));
            i += run;
            continue;
        }
        std::size_t text_end = i;
        while (text_end < data.size() && printable(data[text_end])) ++text_end;
        if (text_end - i >= 4) {
            out.push_back("        .ascii \"" + std::string(as_text(data.subspan(i, text_end - i))) + "\"");
            i = text_end;
            continue;
        }
        std::size_t row_end = i;
        std::string row;
        while (row_end < data.size() && row_end - i < 16) {
            std::size_t same = 1;
            while (row_end + same < data.size() && data[row_end + same] == data[row_end]) ++same;
            if (same >= 16) break;   // (the run after the row is a space line of its own)
            std::size_t p = row_end;
            while (p < data.size() && printable(data[p])) ++p;
            if (row_end > i && p - row_end >= 4) break;   // (the text after the row is a line of its own)
            row += std::format("{}0x{:02x}", row.empty() ? "" : ",", data[row_end]);
            ++row_end;
        }
        out.push_back("        .byte " + row);
        i = row_end;
    }
    return out;
}

std::string equ(const std::string& name, std::uint32_t value, unsigned digits) { return std::format(".equ {}, 0x{:0{}x}", name, value, digits); }

Bytes assemble(const processors::Processor& cpu, std::string_view source, const std::filesystem::path& folder) {
    return processors::assemble_described(cpu, source, folder).data;
}

}  // namespace cb::source::described_source
