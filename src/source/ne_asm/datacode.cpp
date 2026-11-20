#include "source/ne_asm/datacode.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::source::ne_asm::datacode {

namespace {

constexpr std::size_t kMinGarbageRun = 3;     // consecutive rows that make a garbage cluster
constexpr std::size_t kReportRegions = 40;    // regions the report lists
constexpr std::size_t kReportPreview = 24;    // bytes shown of each

bool is_printable(std::uint8_t b) { return (b >= 0x20 && b < 0x7F) || b == 0x09; }

bool has_lowercase(ByteView run) {
    return std::any_of(run.begin(), run.end(), [](std::uint8_t b) { return b >= 'a' && b <= 'z'; });
}

bool word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// Mnemonics objdump tends to emit only when it decodes data as code.
bool is_data_mnemonic(std::string_view mnemonic) {
    static constexpr std::array<std::string_view, 15> names = {"outs", "ins", "arpl", "bound", "aaa", "aas", "daa", "das",
                                                               "lds",  "les", "sahf", "lahf",  "xlat", "hlt", "into"};
    return std::find(names.begin(), names.end(), mnemonic) != names.end();
}

bool is_segment_prefix(std::string_view mnemonic) {
    static constexpr std::array<std::string_view, 9> names = {"fs", "gs", "es", "ds", "cs", "ss", "lock", "repz", "repnz"};
    return std::find(names.begin(), names.end(), mnemonic) != names.end();
}

// A 32-bit general register named as a whole word anywhere in the instruction.
bool names_32bit_register(std::string_view instruction) {
    static constexpr std::array<std::string_view, 8> registers = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"};
    std::size_t i = 0;
    while (i < instruction.size()) {
        if (!word_char(instruction[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < instruction.size() && word_char(instruction[j])) ++j;
        if (std::find(registers.begin(), registers.end(), instruction.substr(i, j - i)) != registers.end()) return true;
        i = j;
    }
    return false;
}

bool is_garbage(std::string_view instruction) {
    const std::vector<std::string_view> words = text::split_ws(instruction);
    const std::string_view mnemonic = words.empty() ? std::string_view() : words.front();
    return is_data_mnemonic(mnemonic) || (is_segment_prefix(mnemonic) && words.size() > 1) ||
           instruction.find("(bad)") != std::string_view::npos || names_32bit_register(instruction);
}

}  // namespace

std::string_view kind_name(Kind kind) { return kind == Kind::String ? "str" : "data"; }

std::vector<TextRun> find_strings(ByteView code, std::size_t min_length) {
    std::vector<TextRun> out;
    std::size_t i = 0;
    const std::size_t n = code.size();
    while (i < n) {
        if (is_printable(code[i])) {
            std::size_t j = i;
            while (j < n && is_printable(code[j])) ++j;
            const ByteView run = code.subspan(i, j - i);
            if (j - i >= min_length && has_lowercase(run)) {
                const std::size_t end = (j < n && code[j] == 0) ? j + 1 : j;   // eat a NUL
                out.push_back({static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(end), std::string(run.begin(), run.end())});
                i = end;
                continue;
            }
        }
        ++i;
    }
    return out;
}

std::vector<Region> garbage_spans(std::span<const segment::Row> rows) {
    // by offset: a row met twice keeps the later text
    std::map<std::uint32_t, std::string_view> by_offset;
    for (const segment::Row& row : rows) by_offset[row.offset] = row.text;

    std::vector<Region> spans;
    std::vector<std::uint32_t> run;
    // a run ends where the row after it starts; the last row's run ends at its own offset
    const auto close = [&](std::map<std::uint32_t, std::string_view>::const_iterator after) {
        if (run.size() >= kMinGarbageRun) {
            spans.push_back({run.front(), after != by_offset.end() ? after->first : run.back(), Kind::Data});
        }
        run.clear();
    };
    for (auto it = by_offset.begin(); it != by_offset.end(); ++it) {
        if (is_garbage(it->second)) {
            run.push_back(it->first);
        } else {
            close(it);
        }
    }
    close(by_offset.end());
    return spans;
}

std::vector<Region> data_regions(ByteView code, std::span<const segment::Row> rows) {
    std::vector<Region> spans;
    for (const TextRun& s : find_strings(code)) spans.push_back({s.start, s.end, Kind::String});
    for (const Region& g : garbage_spans(rows)) spans.push_back(g);
    std::sort(spans.begin(), spans.end(), [](const Region& a, const Region& b) {
        if (a.start != b.start) return a.start < b.start;
        if (a.end != b.end) return a.end < b.end;
        return a.kind < b.kind;
    });
    std::vector<Region> merged;
    for (const Region& r : spans) {
        if (!merged.empty() && r.start <= merged.back().end) {
            merged.back().end = std::max(merged.back().end, r.end);
        } else {
            merged.push_back(r);
        }
    }
    return merged;
}

std::string report(std::string_view module_name, std::size_t segment, ByteView code, std::span<const Region> regions) {
    std::uint64_t total = 0;
    for (const Region& r : regions) total += r.end - r.start;
    std::string out = std::format("{} seg{}: {} data region(s), {} bytes ({:.1f}% of {})\n", module_name, segment, regions.size(), total,
                                  100.0 * static_cast<double>(total) / static_cast<double>(std::max<std::size_t>(1, code.size())), code.size());
    for (std::size_t k = 0; k < regions.size() && k < kReportRegions; ++k) {
        const Region& r = regions[k];
        const std::size_t from = std::min<std::size_t>(r.start, code.size());
        const std::size_t to = std::min<std::size_t>({static_cast<std::size_t>(r.end), static_cast<std::size_t>(r.start) + kReportPreview, code.size()});
        std::string preview;
        for (std::size_t i = from; i < to; ++i) preview.push_back(code[i] >= 0x20 && code[i] < 0x7F ? static_cast<char>(code[i]) : '.');
        out += std::format("  {:04x}-{:04x}  {:<4}  {:3}B  |{}|\n", r.start, r.end, kind_name(r.kind), r.end - r.start, preview);
    }
    return out;
}

int main(const base::Corpus& corpus, std::span<const std::string> args, const fixups::Objdump& objdump) {
    const cli::Command command{
        "datacode",
        "which bytes of a segment are data: strings and the clusters objdump decodes into nonsense",
        {cli::Positional{"module", "a corpus name or a path"},
         cli::Option{"--seg", "", "the segment (from 1)", cli::Arg::Value, "N", "1"},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const std::string name = a.required("module");
            const auto seg_number = a.integer<std::size_t>("--seg");
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            const std::filesystem::path path = corpus.retail(name).value_or(fs::from_text(name));
            const formats::ne::Module module = fixups::open_module(path);
            const segment::Seg seg(module, seg_number.value_or(1), ordinals, {}, objdump);
            const std::vector<Region> regions = data_regions(seg.code(), seg.rows());
            out_text(report(name, seg_number.value_or(1), seg.code(), regions));
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("datacode", {command});
    std::vector<std::string> line = {"datacode"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::datacode
