#include "cb/exact/habits/cmpp.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::habits::cmpp {

namespace {

constexpr std::uint8_t kCallNear = 0xE8;
constexpr std::uint8_t kCallFar = 0x9A;
constexpr std::uint8_t kJumpFar = 0xEA;
constexpr std::uint32_t kEntryDataAt = 3;      // the entry helper's call is three bytes; its data follow it
constexpr std::size_t kLongestInstruction = 16;
constexpr std::size_t kMostDifferencesShown = 12;
constexpr std::size_t kOutOfStepRows = 14;
constexpr std::size_t kConsoleTail = 600;
constexpr std::size_t kMostDiagnostics = 30;

const x86::Decoder16& decoder_of(const Options& o) { return o.decoder ? *o.decoder : x86::capstone_decoder(); }

std::size_t entry_data_size(EntryData e) { return e == EntryData::ByteAndWord ? 3 : 2; }

std::uint16_t word_at(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }

// The text of a listing with its numbers taken out, so two instructions that differ only in a number compare equal:
// a hexadecimal literal (0x followed by lower-case hex digits) or a run of decimal digits standing alone as a word.
std::string without_numbers(std::string_view t) {
    const auto word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };
    const auto hex = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); };
    std::string out;
    std::size_t i = 0;
    while (i < t.size()) {
        if (t[i] == '0' && i + 2 < t.size() && t[i + 1] == 'x' && hex(t[i + 2])) {
            i += 2;
            while (i < t.size() && hex(t[i])) ++i;
            out += 'N';
        } else if (std::isdigit(static_cast<unsigned char>(t[i])) && (i == 0 || !word(t[i - 1]))) {
            std::size_t j = i;
            while (j < t.size() && std::isdigit(static_cast<unsigned char>(t[j]))) ++j;
            if (j < t.size() && word(t[j])) {   // not a number standing alone
                out += t[i++];
            } else {
                out += 'N';
                i = j;
            }
        } else {
            out += t[i++];
        }
    }
    return out;
}

std::string instruction_text(const x86::Insn& i) {
    return i.op_str.empty() ? i.mnemonic : i.mnemonic + " " + i.op_str;
}

}  // namespace

std::vector<Row> walk(ByteView code, std::uint32_t base, bool skip_header, const Options& options) {
    const x86::Decoder16& decoder = decoder_of(options);
    const std::size_t n = code.size();
    const std::size_t data = entry_data_size(options.entry_data);
    std::vector<Row> out;
    std::size_t p = 0;
    while (p < n) {
        if (skip_header && p == kEntryDataAt && code[0] == kCallNear && n >= kEntryDataAt + data) {
            Row row;
            row.offset = kEntryDataAt;
            row.size = static_cast<std::uint32_t>(data);
            row.mnemonic = "hdr";
            row.text = options.entry_data == EntryData::Word
                           ? std::format("dw 0x{:04x}", word_at(code, 3))
                           : std::format("db 0x{:02x} ; dw 0x{:04x}", code[3], word_at(code, 4));
            out.push_back(std::move(row));
            p += data;
            continue;
        }
        const std::optional<x86::Insn> insn = decoder.first(code.subspan(p, std::min(kLongestInstruction, n - p)), base + static_cast<std::uint32_t>(p));
        if (!insn) {
            out.push_back({static_cast<std::uint32_t>(p), 1, std::format("db 0x{:02x}", code[p]), "db"});
            ++p;
        } else {
            out.push_back({static_cast<std::uint32_t>(p), static_cast<std::uint32_t>(insn->size()), instruction_text(*insn), insn->mnemonic});
            p += insn->size();
        }
    }
    return out;
}

std::set<std::uint32_t> loose_bytes(ByteView code, const std::set<std::uint32_t>& fixups, const Options& options) {
    std::set<std::uint32_t> loose = fixups;
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    for (const Row& row : walk(code, 0, true, options)) {
        if (row.mnemonic == "hdr") continue;   // (its first byte is frame data, whatever its value)
        const std::uint8_t op = code[row.offset];
        if (op == kCallNear && row.size == 3) {
            const std::int64_t target = row.offset + 3 + static_cast<std::int16_t>(word_at(code, row.offset + 1));
            if (!(0 <= target && target < n)) {   // to another procedure: where it lies is the linker's
                loose.insert(row.offset + 1);
                loose.insert(row.offset + 2);
            }
        } else if ((op == kCallFar || op == kJumpFar) && row.size == 5) {
            for (std::uint32_t k = 1; k <= 4; ++k) loose.insert(row.offset + k);
        }
    }
    return loose;
}

Request parse_request(std::span<const std::string> args) {
    Request r;
    std::vector<std::string> positional;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];
        if (a == "--asm") {
            r.original_only = true;
        } else if (a == "--cod") {
            r.show_listing = true;
        } else if (a == "--proc") {
            if (i + 1 >= args.size()) throw UsageError("--proc needs a procedure name");
            r.procedure = args[++i];
        } else if (a.starts_with("--")) {
            throw UsageError(std::format("unknown option {}", a));
        } else {
            positional.push_back(a);
        }
    }
    if (positional.size() < 3) throw UsageError("usage: cmpp M.EXE 0x00001 m100/p00001.pas [--proc NAME] [--asm] [--cod]");
    r.exe = positional[0];
    const std::optional<std::uint32_t> address = text::parse_int<std::uint32_t>(positional[1], 16);
    if (!address) throw UsageError(std::format("{} is not an address", positional[1]));
    r.address = *address;
    r.source = positional[2];
    return r;
}

namespace {

std::uint32_t ledger_size(const std::filesystem::path& ledger_dir, const std::string& exe, std::uint32_t address) {
    const std::filesystem::path file = ledger_dir / (text::upper(exe) + ".json");
    const Json ledger = json::load(file);
    const std::string key = std::format("0x{:05x}", address);
    if (!ledger.is_object() || !ledger.contains(key)) {
        throw NotFoundError(file, 0, std::format("the ledger lists no procedure at {}", key));
    }
    const Json& entry = ledger.at(key);
    if (!entry.is_object() || !entry.contains("size") || !entry.at("size").is_number_unsigned()) {
        throw FormatError("exact.habits.cmpp", 0, std::format("{}: the entry for {} has no size", path_text(file), key));
    }
    return entry.at("size").get<std::uint32_t>();
}

// The lines of the compiler's code listing that belong to `name`: from its "Procedure/Function : NAME" line up to the
// next procedure's.
std::string procedure_listing(std::string_view cod, const std::string& name) {
    std::string out;
    bool on = false;
    for (const std::string_view line : text::lines(cod)) {
        const std::string_view body = text::ltrim(line);
        constexpr std::string_view kHeading = "Procedure/Function : ";
        if (body.starts_with(kHeading)) {
            const std::string_view rest = body.substr(kHeading.size());
            const std::string_view word = rest.substr(0, rest.find_first_of(" \t\r\n\v\f"));
            if (!word.empty()) on = text::upper(word) == name;
        }
        if (on) {
            out += line;
            out += '\n';
        }
    }
    return out;
}

}  // namespace

Comparison compare(const Request& request, PascalCompiler& compiler, const std::filesystem::path& ledger_dir, const Options& options) {
    Comparison c;
    c.exe = request.exe;
    c.address = request.address;
    const Bytes image = compiler.image(request.exe);
    c.original_size = ledger_size(ledger_dir, request.exe, request.address);
    const ByteView want = io::checked_range(image, request.address, c.original_size);
    if (request.original_only) {
        c.verdict = Comparison::Verdict::OriginalOnly;
        c.original_rows = walk(want, request.address, true, options);
        c.original_code.assign(want.begin(), want.end());
        return c;
    }
    c.procedure = text::upper(request.procedure.value_or(std::format("P{:05X}", request.address)));
    const Compiled compiled = compiler.compile(request.source);
    const auto found = compiled.procedures.find(c.procedure);
    if (!compiled.code || found == compiled.procedures.end()) {
        c.verdict = Comparison::Verdict::NoCode;
        std::vector<std::string> bad;
        for (const std::string_view line : text::lines(compiled.listing)) {
            if (line.find('^') != std::string_view::npos || line.find("Error") != std::string_view::npos ||
                line.find("Warning") != std::string_view::npos) {
                bad.emplace_back(line);
            }
        }
        if (bad.size() > kMostDiagnostics) bad.resize(kMostDiagnostics);
        if (bad.empty()) {
            const std::string_view tail = compiled.console;
            bad.emplace_back(tail.substr(tail.size() > kConsoleTail ? tail.size() - kConsoleTail : 0));
        }
        c.diagnostics = std::move(bad);
        return c;
    }
    const std::uint32_t offset = found->second.offset;
    c.compiled_size = found->second.size;
    const ByteView got = io::checked_range(*compiled.code, offset, c.compiled_size);
    std::set<std::uint32_t> fixups;
    for (const std::uint32_t k : compiled.fixups) {
        if (k >= offset && k - offset < c.compiled_size) fixups.insert(k - offset);
    }
    if (request.show_listing) c.compiler_listing = procedure_listing(compiled.cod, c.procedure);
    const std::set<std::uint32_t> loose_got = loose_bytes(got, fixups, options);
    c.not_compared = loose_got.size();
    bool identical = c.compiled_size == c.original_size;
    for (std::uint32_t k = 0; identical && k < c.compiled_size; ++k) {
        if (!loose_got.contains(k) && got[k] != want[k]) identical = false;
    }
    if (identical) {
        c.verdict = Comparison::Verdict::Identical;
        return c;
    }
    c.verdict = Comparison::Verdict::Differs;
    // Walk both listings together: the same instruction (the numbers set aside where a byte is the linker's) moves both
    // on; one that differs in a number but not in kind or size is shown; anything else is where they fall out of step.
    const std::vector<Row> original = walk(want, request.address, true, options);
    const std::vector<Row> compiled_rows = walk(got, request.address, true, options);
    std::size_t i = 0, j = 0;
    while (i < original.size() && j < compiled_rows.size()) {
        const Row& a = original[i];
        const Row& b = compiled_rows[j];
        bool loose_here = false;
        for (std::uint32_t k = 0; k < b.size; ++k) loose_here = loose_here || loose_got.contains(b.offset + k);
        const bool same = loose_here ? without_numbers(a.text) == without_numbers(b.text)
                                     : std::ranges::equal(want.subspan(a.offset, a.size), got.subspan(b.offset, b.size));
        if (same) {
            ++i;
            ++j;
            continue;
        }
        if (a.mnemonic == b.mnemonic && a.size == b.size && c.differences.size() < kMostDifferencesShown) {
            c.differences.push_back({request.address + a.offset, a.text, b.text});
            ++i;
            ++j;
            continue;
        }
        OutOfStep step;
        step.original_address = request.address + a.offset;
        step.compiled_offset = b.offset;
        for (std::size_t k = 0; k < kOutOfStepRows; ++k) {
            StepRow row;
            if (i + k < original.size()) row.original = std::format("{:05x} {}", request.address + original[i + k].offset, original[i + k].text);
            if (j + k < compiled_rows.size()) {
                const Row& r = compiled_rows[j + k];
                bool any_loose = false;
                for (std::uint32_t q = 0; q < r.size; ++q) any_loose = any_loose || loose_got.contains(r.offset + q);
                row.compiled = std::string(any_loose ? "*" : " ") + r.text;
            }
            step.rows.push_back(std::move(row));
        }
        c.out_of_step = std::move(step);
        break;
    }
    return c;
}

std::string render(const Comparison& c) {
    std::string out;
    switch (c.verdict) {
        case Comparison::Verdict::OriginalOnly: {
            out += std::format("== {} 0x{:05x}  {} bytes\n", c.exe, c.address, c.original_size);
            // (the bytes of each instruction, as hex text cut at 14 characters)
            for (const Row& r : c.original_rows) {
                std::string hex;   // the instruction's bytes as hex text, cut at 14 characters
                for (std::uint32_t k = 0; k < r.size && hex.size() < 14; ++k) hex += std::format("{:02x}", c.original_code[r.offset + k]);
                if (hex.size() > 14) hex.resize(14);
                out += std::format("{:05x} {:<14} {}\n", c.address + r.offset, hex, r.text);
            }
            return out;
        }
        case Comparison::Verdict::NoCode:
            out += std::format("NO CODE for {}\n", c.procedure);
            for (const std::string& line : c.diagnostics) out += line + "\n";
            return out;
        default: break;
    }
    out += c.compiler_listing;
    if (c.verdict == Comparison::Verdict::Identical) {
        out += std::format("IDENTICAL  {} 0x{:05x}  {} bytes ({} not compared: link-time addresses, calls to other procedures)\n", c.exe,
                           c.address, c.compiled_size, c.not_compared);
        return out;
    }
    out += std::format("DIFFERS  compiled {} bytes, original {}\n", c.compiled_size, c.original_size);
    for (const Difference& d : c.differences) out += std::format("  {:05x}  {:<34} | {}\n", d.address, d.original, d.compiled);
    if (c.out_of_step) {
        out += std::format("  out of step at original {:05x} / compiled +{}:\n", c.out_of_step->original_address, c.out_of_step->compiled_offset);
        for (const StepRow& r : c.out_of_step->rows) out += std::format("    {:<40} | {}\n", r.original, r.compiled);
    }
    return out;
}

int exit_status(const Comparison& c) {
    switch (c.verdict) {
        case Comparison::Verdict::Identical:
        case Comparison::Verdict::OriginalOnly: return 0;
        case Comparison::Verdict::NoCode: return 2;
        case Comparison::Verdict::Differs: break;
    }
    return 1;
}

}  // namespace cb::exact::habits::cmpp
