// source/risc_source.cpp -- see risc_source.hpp.

#include "source/risc_source.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "core/error.hpp"
#include "core/text.hpp"
#include "source/processors.hpp"

namespace cb::source::risc_source {
namespace {

bool rs_is_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }
bool rs_is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

// The text of one instruction as the description reads it, or nullopt for bytes that are no instruction.
// `following` is what comes after the instruction (a delay slot's instruction is read with its branch).
std::optional<std::string> rs_read(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, ByteView following = {}) {
    Bytes joined(bytes.begin(), bytes.end());
    joined.insert(joined.end(), following.begin(), following.end());
    const std::vector<processors::Line> lines = processors::read_described(cpu, joined, pc, 1);
    if (lines.empty() || lines[0].bytes.size() != bytes.size() || lines[0].text.starts_with(".d")) return std::nullopt;
    return lines[0].text;
}

// `text` with its number literals 0x<hex> that `pick` gives a replacement for written as the replacement. A literal is a whole word (not part of a name).
template <class Pick>
std::string rs_replace_literals(const std::string& text, Pick&& pick) {
    std::string out;
    for (std::size_t i = 0; i < text.size();) {
        if (text[i] == '0' && i + 2 < text.size() && text[i + 1] == 'x' && (i == 0 || !rs_is_word(text[i - 1]))) {
            std::size_t e = i + 2;
            while (e < text.size() && rs_is_hex(text[e])) ++e;
            if (e > i + 2 && (e >= text.size() || !rs_is_word(text[e]))) {
                const std::optional<std::uint64_t> n = text::parse_int<std::uint64_t>(std::string_view(text).substr(i + 2, e - i - 2), 16);
                const std::optional<std::string> r = n ? pick(*n, out) : std::nullopt;
                if (r) {
                    out += *r;
                    i = e;
                    continue;
                }
                out.append(text, i, e - i);
                i = e;
                continue;
            }
        }
        out += text[i++];
    }
    return out;
}

}  // namespace

std::optional<std::string> described(std::string_view cpu_name, ByteView bytes, std::uint64_t pc, ByteView following) {
    const processors::Processor& cpu = processors::find(cpu_name);
    const std::optional<std::string> text = rs_read(cpu, bytes, pc, following);
    if (!text) return std::nullopt;
    try {
        const processors::Image again = processors::assemble_described(cpu, std::format("\t.org 0x{:x}\n\t{}\n", pc, *text), {});
        if (again.data.size() != bytes.size() || !std::equal(again.data.begin(), again.data.end(), bytes.begin())) return std::nullopt;
    } catch (const ParseError&) {
        return std::nullopt;
    }
    return text;
}

std::optional<std::string> read_text(std::string_view cpu_name, ByteView bytes, std::uint64_t pc) { return rs_read(processors::find(cpu_name), bytes, pc); }

std::string code(std::string_view cpu_name, ByteView bytes, std::uint64_t pc, const std::map<std::uint32_t, std::string>& names, std::span<const std::uint32_t> operands,
                 const std::string& immediate, ByteView following, const decompile::NameLabels* symbols) {
    std::optional<std::string> text = described(cpu_name, bytes, pc, following);
    if (!text) {
        std::string raw;
        for (const std::uint8_t b : bytes) raw += std::format("{}0x{:02x}", raw.empty() ? "" : ",", b);
        const std::optional<std::string> said = rs_read(processors::find(cpu_name), bytes, pc, following);
        return ".byte " + raw + (said ? "  ; " + *said : std::string());
    }
    const std::string plain = std::move(*text);
    const auto with_names = [&] {
        return rs_replace_literals(plain, [&](std::uint64_t n, const std::string&) -> std::optional<std::string> {
            if (n > 0xFFFFFFFFull) return std::nullopt;
            const std::uint32_t v = static_cast<std::uint32_t>(n);
            if (std::find(operands.begin(), operands.end(), v) == operands.end()) return std::nullopt;
            const auto named = names.find(v);
            if (named == names.end()) return std::nullopt;
            return named->second;
        });
    };
    const auto with_immediate = [&]() -> std::optional<std::string> {   // the last number literal, with its sign
        std::size_t last = std::string::npos, last_end = 0;
        for (std::size_t i = 0; i + 2 < plain.size(); ++i) {
            if (plain[i] != '0' || plain[i + 1] != 'x' || (i > 0 && rs_is_word(plain[i - 1]))) continue;
            std::size_t e = i + 2;
            while (e < plain.size() && rs_is_hex(plain[e])) ++e;
            if (e == i + 2 || (e < plain.size() && rs_is_word(plain[e]))) continue;
            last = i, last_end = e;
            i = e - 1;
        }
        if (last == std::string::npos) return std::nullopt;
        const std::size_t from = last > 0 && plain[last - 1] == '-' ? last - 1 : last;
        std::string line = plain.substr(0, from) + immediate + plain.substr(last_end);
        // (PowerPC: the description writes an add of a negative number as `subi` of its magnitude; an address half is the add's operand, not its negation)
        if (cpu_name == "ppc")
            for (const std::string_view pseudo : {"subi ", "subis "})
                if (line.starts_with(pseudo)) line = std::string("add") + line.substr(3);
        return line;
    };
    // (with the program's symbols to check against: the line is kept only when the names' values, put in the instruction, are its bytes)
    const auto holds = [&](const std::string& line) {
        if (!symbols) return true;
        const std::optional<Bytes> built = processors::assemble_line(processors::find(cpu_name), line, pc, *symbols);
        return built && built->size() == bytes.size() && std::equal(built->begin(), built->end(), bytes.begin());
    };
    if (!immediate.empty()) {
        if (const std::optional<std::string> line = with_immediate()) {
            if (holds(*line)) return *line;
        } else {
            return plain;
        }
    }
    std::string named = with_names();
    return named != plain && holds(named) ? named : plain;
}

std::string m68k_code(const decompile::m68k::Insn& insn, const decompile::AddressLabels& labels) {
    namespace m68k = decompile::m68k;
    std::map<std::uint32_t, std::string> names;
    std::vector<std::uint32_t> operands;
    const auto add = [&](std::int64_t value) {
        const std::uint32_t v = static_cast<std::uint32_t>(value & 0xFFFFFFFF);
        if (const auto named = labels.find(v); named != labels.end() && !named->second.empty()) names[v] = named->second, operands.push_back(v);
    };
    for (const m68k::Op& o : insn.operands()) {
        switch (o.kind) {
            case m68k::OpKind::AbsW:
            case m68k::OpKind::AbsL:
            case m68k::OpKind::PcDisp:
            case m68k::OpKind::PcIndex:
            case m68k::OpKind::Expr: add(o.value); break;
            case m68k::OpKind::Imm:
                if (o.size == m68k::Size::Long && o.value >= 0 && o.value <= 0xFFFFFFFF) add(o.value);
                break;
            case m68k::OpKind::Label8: add(o.target); break;
            default: break;
        }
    }
    if (insn.target) add(*insn.target);
    return code("m68k", insn.bytes(), insn.pc, names, operands);
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

Bytes assemble(std::string_view cpu_name, std::string_view source, const std::filesystem::path& folder) {
    return processors::assemble_described(processors::find(cpu_name), source, folder).data;
}

}  // namespace cb::source::risc_source
