#include "cb/recompile/disassemble.hpp"

#include <algorithm>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::recompile::disassemble {
namespace {

// Flow does not go on past these; calls (bl bla bcl bctrl bclrl) come back.
bool ends_flow(std::string_view mnemonic) {
    return mnemonic == "b" || mnemonic == "ba" || mnemonic == "blr" || mnemonic == "bctr" || mnemonic == "bctrl" || mnemonic == "rfi" ||
           mnemonic == "rfid";
}

bool has_group(const cs_detail& detail, std::uint8_t group) {
    return std::find(detail.groups, detail.groups + detail.groups_count, group) != detail.groups + detail.groups_count;
}

std::uint32_t word_at(ByteView code, std::size_t off) {
    return (static_cast<std::uint32_t>(code[off]) << 24) | (static_cast<std::uint32_t>(code[off + 1]) << 16) |
           (static_cast<std::uint32_t>(code[off + 2]) << 8) | code[off + 3];
}

}  // namespace

bool is_vmx128(std::uint32_t word) {
    const std::uint32_t primary = word >> 26;
    return primary == 4 || primary == 5 || primary == 6;
}

Disassembler::Disassembler(VectorDecoder vector) : vector_(std::move(vector)) {
    (void)decoders_.get(platforms::Arch::Ppc, 32, true);   // fail here, not on the first word, if there is no Capstone
}

std::optional<Disassembler::Decoded> Disassembler::decode_one(ByteView code, std::size_t off, std::uint32_t base_va) {
    if (off + kWord > code.size()) return std::nullopt;
    const platforms::Decoder& cs = decoders_.get(platforms::Arch::Ppc, 32, true);
    std::optional<Decoded> out;
    cs.each(code.subspan(off, kWord), static_cast<std::uint64_t>(base_va) + off, 1, [&](const cs_insn& insn) {
        Decoded d;
        d.mnemonic = insn.mnemonic;
        d.operands = insn.op_str;
        if (insn.detail != nullptr) {
            d.jump = has_group(*insn.detail, CS_GRP_JUMP);
            d.call = has_group(*insn.detail, CS_GRP_CALL);
            const cs_ppc& ppc = insn.detail->ppc;
            for (int i = 0; i < ppc.op_count; ++i) {
                if (ppc.operands[i].type == PPC_OP_IMM) d.immediates.push_back(ppc.operands[i].imm);
            }
        }
        out = std::move(d);
        return false;
    });
    return out;
}

Coverage Disassembler::classify(ByteView code, std::uint32_t base_va, std::span<const std::uint32_t> entries) {
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    Coverage covered(code.size(), false);
    std::vector<bool> seen(code.size(), false);
    std::vector<std::int64_t> work;
    for (const std::uint32_t e : entries) work.push_back(static_cast<std::int64_t>(e) - base_va);
    const auto cover = [&](std::int64_t off) {
        for (std::int64_t k = off; k < std::min<std::int64_t>(off + kWord, n); ++k) covered[static_cast<std::size_t>(k)] = true;
    };
    while (!work.empty()) {
        const std::int64_t off = work.back();
        work.pop_back();
        if (off < 0 || off >= n || off % kWord != 0 || seen[static_cast<std::size_t>(off)]) continue;
        seen[static_cast<std::size_t>(off)] = true;
        const std::optional<Decoded> ins = decode_one(code, static_cast<std::size_t>(off), base_va);
        if (!ins) {
            if ((code[static_cast<std::size_t>(off)] >> 2) >= 4 && (code[static_cast<std::size_t>(off)] >> 2) <= 6) {   // vector code: falls through, never branches
                cover(off);
                work.push_back(off + kWord);
            }
            continue;
        }
        cover(off);
        if (ins->jump || ins->call) {
            for (const std::int64_t target : ins->immediates) {
                const std::int64_t at = target - base_va;
                if (at >= 0 && at < n) work.push_back(at);
            }
        }
        if (!ends_flow(ins->mnemonic)) work.push_back(off + kWord);
    }
    return covered;
}

std::vector<Span> Disassembler::data_spans(ByteView code, std::uint32_t base_va, std::span<const std::uint32_t> entries, std::size_t min_run) {
    const Coverage covered = classify(code, base_va, entries);
    std::vector<Span> out;
    std::size_t i = 0;
    while (i < code.size()) {
        if (covered[i]) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < code.size() && !covered[j]) ++j;
        if (j - i >= min_run) out.push_back({i, j});
        i = j;
    }
    return out;
}

void Disassembler::each_row(ByteView code, std::uint32_t base_va, std::size_t start, std::optional<std::size_t> count,
                            const std::function<void(const Row&)>& visit) {
    std::size_t emitted = 0;
    for (std::size_t off = start - start % kWord; off + kWord <= code.size() && (!count || emitted < *count); off += kWord, ++emitted) {
        Row row;
        row.address = base_va + static_cast<std::uint32_t>(off);
        row.word = word_at(code, off);
        // the vector decoder first: Capstone reads these as other instructions (ISA 2.07/3.0)
        if (const auto vec = vector_ ? vector_(row.word) : std::nullopt) {
            row.mnemonic = vec->first;
            row.operands = vec->second;
        } else if (const std::optional<Decoded> ins = decode_one(code, off, base_va)) {
            row.mnemonic = ins->mnemonic;
            row.operands = ins->operands;
        } else {
            row.mnemonic = ".long";
            row.operands = std::format("0x{:08X}", row.word);
        }
        visit(row);
    }
}

std::vector<Row> Disassembler::listing(ByteView code, std::uint32_t base_va, std::size_t start, std::optional<std::size_t> count) {
    std::vector<Row> rows;
    each_row(code, base_va, start, count, [&](const Row& row) { rows.push_back(row); });
    return rows;
}

// ---- an image's code ---------------------------------------------------------------------------------------------------

CodeSection code_section(const ImageFacts& image) {
    CodeSection out;
    out.address = image.code_begin;
    out.entry = image.entry != 0 ? image.entry : image.code_begin;
    // the bytes at the start of the code, as the file holds them from the chunk the address falls in
    for (const ImageFacts::Chunk& c : image.chunks) {
        if (c.va <= image.code_begin && image.code_begin - c.va < c.size) {
            const std::uint64_t at = static_cast<std::uint64_t>(c.file_offset) + (image.code_begin - c.va);
            const std::uint64_t want = image.code_end > image.code_begin ? image.code_end - image.code_begin : 0;
            const ByteView file(image.data);
            const ByteView view = at <= file.size() ? file.subspan(static_cast<std::size_t>(at), std::min<std::uint64_t>(want, file.size() - at)) : ByteView();
            out.code.assign(view.begin(), view.end());
            break;
        }
    }
    return out;
}

FlatImage flatten(const ImageFacts& image) {
    if (image.chunks.empty()) throw FormatError("powerpc image", 0, "no chunk of the image is loaded");
    std::uint64_t top = 0;
    for (const ImageFacts::Chunk& c : image.chunks) top = std::max<std::uint64_t>(top, static_cast<std::uint64_t>(c.va) + c.size);
    if (top < image.base) throw FormatError("powerpc image", 0, "a chunk lies below the image base");
    FlatImage out;
    out.base = image.base;
    out.entry = image.entry != 0 ? image.entry : image.code_begin;
    out.bytes.assign(static_cast<std::size_t>(top - image.base), 0);
    for (const ImageFacts::Chunk& c : image.chunks) {
        const ByteView piece = io::checked_range(image.data, c.file_offset, c.size);
        if (c.va < image.base) throw FormatError("powerpc image", c.file_offset, "a chunk lies below the image base");
        std::copy(piece.begin(), piece.end(), out.bytes.begin() + static_cast<std::ptrdiff_t>(c.va - image.base));
    }
    for (const ImageFacts::Section& s : image.sections) {
        out.sections.push_back({s.name, s.va, s.size, (s.characteristics & 0x20000000) != 0 || s.name == ".text"});
    }
    return out;
}

// ---- the command ---------------------------------------------------------------------------------------------------------

int main(const ImageReader& read_image, const VectorDecoder& vector, std::span<const std::string> args) {
    const cli::Command command{
        "disassemble",
        "Xbox 360 PowerPC as (mnemonic, operands) text",
        {cli::Positional{"image", "a PowerPC image or an .xex"}, cli::Option{"--limit", "", "how many words to list", cli::Arg::Value, "N", "60"}},
        [&](const cli::Args& a) -> int {
            const std::filesystem::path path = fs::from_text(a.required("image"));
            const std::size_t limit = a.integer<std::size_t>("--limit").value_or(60);
            const CodeSection section = code_section(read_image(path));
            Disassembler dis(vector);
            const std::uint32_t entries[] = {section.entry, section.address};
            cb::out("{}  code at 0x{:08X}, entry 0x{:08X}, {} bytes\n", fs::to_text(path.filename()), section.address, section.entry, section.code.size());
            const Coverage covered = dis.classify(section.code, section.address, entries);
            dis.each_row(section.code, section.address, 0, limit, [&](const Row& row) {
                const std::array<std::uint8_t, 4> raw = row.raw();
                std::string hex;
                for (const std::uint8_t b : raw) hex += std::format("{:02x}", b);
                cb::out("  {} {:08X}  {}  {:<10} {}\n", covered[row.address - section.address] ? " " : "d", row.address, hex, row.mnemonic, row.operands);
            });
            std::size_t data = 0;
            for (const Span& s : dis.data_spans(section.code, section.address, entries)) data += s.end - s.begin;
            cb::out("  --- {} bytes, {} read as data ({:.1f}%)\n", section.code.size(), data,
                    100.0 * static_cast<double>(data) / static_cast<double>(std::max<std::size_t>(1, section.code.size())));
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("codebreaker", {command});
    std::vector<std::string> line = {"disassemble"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::recompile::disassemble
