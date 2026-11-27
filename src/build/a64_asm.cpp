// cb-unity: alone
#include "build/a64_asm.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"
#include "decompile/a64.hpp"
#include "decompile/z80.hpp"
#include "decompile/labels.hpp"
#include "source/dialect.hpp"
#include "source/processors.hpp"

namespace cb::build::a64_asm {

using NameLabels = decompile::NameLabels;

namespace {

// The instruction syntax: the AArch64 description (the one assembler), its encoder.
const Encoder& isa() { return source::processors::described_encoder(source::processors::find("a64")); }

// The failures an encoder returns, as the layer reports them: a name with no value is the retryable one.
struct da {
    static std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient, std::int64_t pc = 0) {
        const Result<std::int64_t> r = isa().value(expr, labels, pc);
        if (r.ok()) return r.value();
        if (lenient && is_undefined(r.error())) return 0;
        throw ParseError("", 0, 0, r.error().what());
    }
    // The table of decompile/a64 first: it gives the encoding the original has where the description's assembler finds two that read the same
    // (the alias rows: asr, lsr, ubfx, the bitmask immediates ...; ftpd, NX-Shell and the like rebuild byte for byte). What the table does not
    // know the description assembles.
    static Bytes assemble(const std::string& line, std::uint64_t pc, const NameLabels& labels, bool lenient) {
        if (!lenient) {
            try {
                return decompile::a64::assemble(line, pc, labels, false);
            } catch (const decompile::z80::UnknownLabel& e) {
                throw ParseError("", 0, 0, std::string("undefined symbol '") + e.name() + "'");
            } catch (const Error&) {
            }
        }
        NameLabels local = lenient ? labels : NameLabels{};
        const NameLabels& use = lenient ? local : labels;
        const std::string described = std::string(text::trim(source::dialect::find("gas-a64").parse(line)));
        Result<Bytes> r = isa().assemble(described, static_cast<std::int64_t>(pc), use);
        while (lenient && !r.ok() && is_undefined(r.error())) {
            const std::string name = static_cast<const UndefinedSymbolError&>(r.error()).name();
            if (local.contains(name)) break;
            local[name] = static_cast<std::int64_t>(pc);
            r = isa().assemble(described, static_cast<std::int64_t>(pc), local);
        }
        if (!r.ok()) throw ParseError("", 0, 0, r.error().what());
        return r.value();
    }
};

}  // namespace

namespace {

struct Line {
    std::uint32_t number = 0;
    std::string text;      // the original line, for messages
    std::vector<std::string> labels;
    std::string keyword;   // lower case: a directive or a mnemonic; empty for labels alone
    std::string operand;
    bool instruction = false;
};

// The part of a line before `//` that is not inside a string.
std::string_view strip_line_comment(std::string_view s) {
    bool quoted = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '"' && (i == 0 || s[i - 1] != '\\')) quoted = !quoted;
        else if (!quoted && s[i] == '/' && i + 1 < s.size() && s[i + 1] == '/') return s.substr(0, i);
    }
    return s;
}

bool is_label_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$'; }

std::vector<Line> parse_lines(std::string_view source) {
    std::vector<Line> out;
    std::uint32_t number = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++number;
        std::string_view line = raw;
        if (!line.empty() && line.front() == '#') continue;
        line = text::trim(strip_line_comment(line));
        if (line.empty()) continue;
        Line l;
        l.number = number;
        l.text = std::string(raw);
        // labels: `name:` at the start, any number of them
        while (true) {
            std::size_t i = 0;
            while (i < line.size() && is_label_char(line[i])) ++i;
            if (i > 0 && i < line.size() && line[i] == ':') {
                l.labels.emplace_back(line.substr(0, i));
                line = text::trim(line.substr(i + 1));
            } else {
                break;
            }
        }
        if (!line.empty()) {
            const std::size_t sp = line.find_first_of(" \t");
            l.keyword = text::lower(sp == std::string_view::npos ? line : line.substr(0, sp));
            l.operand = std::string(sp == std::string_view::npos ? std::string_view() : text::trim(line.substr(sp + 1)));
            l.instruction = !l.keyword.starts_with('.');
        }
        out.push_back(std::move(l));
    }
    return out;
}

bool is_string(std::string_view item) { return item.size() >= 2 && item.front() == '"' && item.back() == '"'; }

void push_string(Bytes& out, std::string_view item) {
    item = item.substr(1, item.size() - 2);
    for (std::size_t i = 0; i < item.size(); ++i) {
        char c = item[i];
        if (c == '\\' && i + 1 < item.size()) {
            const char e = item[++i];
            switch (e) {
                case 'n': c = '\n'; break;
                case 't': c = '\t'; break;
                case 'r': c = '\r'; break;
                case '0': c = '\0'; break;
                default: c = e; break;
            }
        }
        out.push_back(static_cast<std::uint8_t>(c));
    }
}

void push_unit(Bytes& out, std::uint64_t v, unsigned size) {
    for (unsigned k = 0; k < size; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}

bool ignored_directive(const std::string& k) {
    return k == ".text" || k == ".section" || k == ".global" || k == ".globl" || k == ".type" || k == ".size" || k == ".arch" || k == ".file";
}

// The bytes a directive writes at pc, given the labels known.
Bytes directive_bytes(const Line& l, std::uint64_t pc, const NameLabels& labels, bool lenient, const std::filesystem::path& dir) {
    Bytes out;
    const auto number = [&](std::string_view e) { return da::value(e, labels, lenient); };
    const std::string& k = l.keyword;
    const auto items = [&] { return split_items(l.operand, false); };
    if (k == ".byte") {
        for (const std::string_view item : items()) {
            if (is_string(item)) push_string(out, item);
            else out.push_back(static_cast<std::uint8_t>(number(item)));
        }
    } else if (k == ".hword" || k == ".short") {
        for (const std::string_view item : items()) push_unit(out, static_cast<std::uint64_t>(number(item)), 2);
    } else if (k == ".word" || k == ".long" || k == ".int") {
        for (const std::string_view item : items()) push_unit(out, static_cast<std::uint64_t>(number(item)), 4);
    } else if (k == ".quad" || k == ".xword") {
        for (const std::string_view item : items()) push_unit(out, static_cast<std::uint64_t>(number(item)), 8);
    } else if (k == ".ascii" || k == ".asciz" || k == ".string") {
        const std::vector<std::string_view> parts = items();
        if (parts.empty()) throw AsmError(l.number, l.text, k + " needs a string");
        for (const std::string_view item : parts) {
            if (!is_string(item)) throw AsmError(l.number, l.text, k + " needs a string");
            push_string(out, item);
            if (k != ".ascii") out.push_back(0);
        }
    } else if (k == ".incbin") {
        const std::vector<std::string_view> parts = items();
        if (parts.size() != 1 || !is_string(parts[0])) throw AsmError(l.number, l.text, ".incbin \"file\"");
        const std::string name(parts[0].substr(1, parts[0].size() - 2));
        const std::filesystem::path file = std::filesystem::path(name).is_absolute() ? std::filesystem::path(name) : dir / name;
        try {
            out = fs::read_file(file);
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    } else if (k == ".fill") {
        const std::vector<std::string_view> parts = items();
        if (parts.empty() || parts.size() > 3) throw AsmError(l.number, l.text, ".fill count[, size[, value]]");
        const std::int64_t count = number(parts[0]);
        const std::int64_t size = parts.size() > 1 ? number(parts[1]) : 1;
        const std::int64_t value = parts.size() > 2 ? number(parts[2]) : 0;
        if (count < 0 || (size != 1 && size != 2 && size != 4 && size != 8)) throw AsmError(l.number, l.text, "bad .fill");
        if (count > (1 << 26)) throw AsmError(l.number, l.text, ".fill too large");
        for (std::int64_t i = 0; i < count; ++i) push_unit(out, static_cast<std::uint64_t>(value), static_cast<unsigned>(size));
    } else if (k == ".space" || k == ".zero") {
        const std::vector<std::string_view> parts = items();
        if (parts.empty() || parts.size() > 2) throw AsmError(l.number, l.text, k + " count[, value]");
        const std::int64_t count = number(parts[0]);
        if (count < 0 || count > (1 << 26)) throw AsmError(l.number, l.text, "bad size");
        out.assign(static_cast<std::size_t>(count), parts.size() > 1 ? static_cast<std::uint8_t>(number(parts[1])) : std::uint8_t{0});
    } else if (k == ".balign" || k == ".p2align" || k == ".align") {
        const std::vector<std::string_view> parts = items();
        if (parts.empty()) throw AsmError(l.number, l.text, k + " needs an alignment");
        std::int64_t a = number(parts[0]);
        if (k != ".balign") a = a < 0 || a > 24 ? 0 : (std::int64_t{1} << a);
        if (a <= 0 || (a & (a - 1)) != 0) throw AsmError(l.number, l.text, "the alignment is a power of two");
        const std::uint64_t pad = (static_cast<std::uint64_t>(a) - (pc % static_cast<std::uint64_t>(a))) % static_cast<std::uint64_t>(a);
        out.assign(static_cast<std::size_t>(pad), 0);
    } else if (k != ".org" && k != ".equ" && k != ".set" && !ignored_directive(k)) {
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

Image build(std::string_view source, const std::filesystem::path& include_dir) {
    const std::vector<Line> lines = parse_lines(source);
    NameLabels labels;
    std::uint64_t pc = 0;
    // pass 1: addresses and constants
    for (const Line& l : lines) {
        try {
            for (const std::string& name : l.labels) labels[name] = static_cast<std::int64_t>(pc);
            if (l.keyword.empty()) continue;
            if (l.keyword == ".equ" || l.keyword == ".set") {
                const auto [name, expr] = equate(l);
                labels[name] = da::value(expr, labels, true);
            } else if (l.keyword == ".org") {
                pc = static_cast<std::uint64_t>(da::value(l.operand, labels, true));
            } else if (l.instruction) {
                pc += 4;
            } else {
                pc += directive_bytes(l, pc, labels, true, include_dir).size();
            }
        } catch (const AsmError&) {
            throw;
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    }
    // pass 2: the bytes. An instruction is four bytes and its place is known from the lines before it, so the instructions are assembled side by side:
    // a serial walk finds each line's address (and writes the directives, which are few and cheap), the instructions are assembled in parallel, and the
    // bytes are put in place in line order, where the first fault in line order is the one reported, as when the lines were done one at a time.
    struct Placed {
        const Line* line;
        std::uint64_t pc;
        Bytes bytes;
        std::exception_ptr error;   // the fault of assembling this instruction
    };
    const auto as_asm_error = [](const std::exception_ptr& error, const Line& l) {
        try {
            std::rethrow_exception(error);
        } catch (const AsmError&) {
            throw;
        } catch (const Error& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    };
    Image image;
    bool started = false;
    pc = 0;
    std::vector<Placed> placed;
    std::vector<std::size_t> instructions;   // the entries of `placed` still to assemble
    std::exception_ptr walk_error;           // the fault that ended the walk, and its line
    const Line* walk_line = nullptr;
    for (const Line& l : lines) {
        try {
            if (l.keyword.empty() || l.keyword == ".equ" || l.keyword == ".set") continue;
            if (l.keyword == ".org") {
                pc = static_cast<std::uint64_t>(da::value(l.operand, labels, false));
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
            if (l.instruction) {
                instructions.push_back(placed.size());
                placed.push_back({&l, pc, {}, nullptr});
                pc += 4;
            } else {
                Bytes b = directive_bytes(l, pc, labels, false, include_dir);
                const std::size_t size = b.size();
                placed.push_back({&l, pc, std::move(b), nullptr});
                pc += size;
            }
        } catch (...) {
            walk_error = std::current_exception();
            walk_line = &l;
            break;
        }
    }
    parallel::parallel_for(instructions.size(), [&](std::size_t item) {
        Placed& p = placed[instructions[item]];
        try {
            p.bytes = da::assemble(p.line->keyword + " " + p.line->operand, p.pc, labels, false);
        } catch (...) {
            p.error = std::current_exception();
        }
    });
    for (Placed& p : placed) {
        if (p.error) as_asm_error(p.error, *p.line);
        const Line& l = *p.line;
        const std::size_t at = static_cast<std::size_t>(p.pc - image.base);
        if (at + p.bytes.size() > (std::size_t{1} << 28)) throw AsmError(l.number, l.text, "past the end of the image (256 MB)");
        if (image.data.size() < at + p.bytes.size()) image.data.resize(at + p.bytes.size(), 0);
        std::copy(p.bytes.begin(), p.bytes.end(), image.data.begin() + static_cast<std::ptrdiff_t>(at));
    }
    if (walk_error) as_asm_error(walk_error, *walk_line);
    return image;
}

}  // namespace cb::build::a64_asm
