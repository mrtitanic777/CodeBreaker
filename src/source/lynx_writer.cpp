// source/lynx_writer.cpp -- an Atari Lynx program as assembly source that builds back into it (lynx_writer.hpp).

#include "source/lynx_writer.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "source/dialect.hpp"
#include "source/dialect.hpp"
#include "source/isa.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::lynx_writer {

namespace m6502 = decompile::m6502;
namespace iw = image_writer;
namespace lynx = formats::lynx;

namespace {

// The Lynx's source is ca65's, with .res fills and string rows (config/dialects/ca65-res.json).
const dialect::Dialect& ca65() { static const dialect::Dialect& d = dialect::find("ca65-res"); return d; }
constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

// One stretch of the file as it is written: the header, bytes that are not loaded, or a segment linked at its RAM address.
struct Piece {
    enum class Kind { Header, Gap, Code, Trailer } kind = Kind::Gap;
    std::string area;            // the ca65 segment and ld65 area: "HEADER", "GAP0", "SEG0"
    std::uint32_t offset = 0;    // in the file
    std::uint32_t size = 0;
    std::uint32_t address = 0;   // Code: where it is linked
};

// The ld65 configuration: where each segment of the source loads (config/dialects/ca65-res.json reads it back).
std::string link_config(const std::vector<Piece>& pieces) {
    std::vector<std::string> cfg = {"MEMORY {"};
    for (const Piece& p : pieces) {
        cfg.push_back(std::format("    {}: start = ${:04X}, size = ${:04X}, fill = yes, file = %O;", p.area, p.kind == Piece::Kind::Code ? p.address : 0, p.size));
    }
    cfg.insert(cfg.end(), {"}", "SEGMENTS {"});
    for (const Piece& p : pieces) cfg.push_back(std::format("    {}: load = {}, type = ro;", p.area, p.area));
    cfg.push_back("}");
    return text::join(cfg, "\n") + "\n";
}

std::vector<Piece> pieces_of(const lynx::Program& p) {
    std::vector<Piece> out;
    const std::uint32_t header = static_cast<std::uint32_t>(p.kind == lynx::Kind::Cartridge ? lynx::kHeaderSize : lynx::kBs93Header);
    out.push_back({Piece::Kind::Header, "HEADER", 0, header, 0});
    std::uint32_t at = header;
    int gaps = 0, codes = 0;
    const auto gap_to = [&](std::uint32_t end, Piece::Kind kind) {
        if (end > at) out.push_back({kind, kind == Piece::Kind::Trailer ? "TRAILER" : std::format("GAP{}", gaps++), at, end - at, 0});
        at = std::max(at, end);
    };
    for (const lynx::Segment& s : p.segments) {
        if (s.offset < at || s.offset + s.size > p.file.size()) continue;   // (set_loaded sorts them; one that overlaps another or runs off is data)
        gap_to(s.offset, Piece::Kind::Gap);
        out.push_back({Piece::Kind::Code, std::format("SEG{}", codes++), s.offset, s.size, s.address});
        at = s.offset + s.size;
    }
    gap_to(static_cast<std::uint32_t>(p.file.size()), p.kind == lynx::Kind::Bs93 ? Piece::Kind::Trailer : Piece::Kind::Gap);
    return out;
}

// Flow runs through RAM; the file offset an address is shown at files the instruction (an address two segments load to shows the later one).
class LynxFlow final : public iw::CodeModel<m6502::Insn> {
public:
    LynxFlow(const lynx::Program& program, const std::vector<Piece>& pieces) : memory_(program.memory()), file_offset_(0x10000, -1) {
        for (const Piece& piece : pieces) {
            if (piece.kind != Piece::Kind::Code) continue;
            for (std::uint32_t i = 0; i < piece.size && piece.address + i < lynx::kRamEnd; ++i) file_offset_[piece.address + i] = static_cast<std::int64_t>(piece.offset + i);
        }
    }

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override { return m6502::decode(memory_, pc, pc, m6502::Variant::Cmos); }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc < lynx::kRamEnd && file_offset_[pc] >= 0; }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override {
        if (!inside(pc)) return false;
        for (std::uint32_t i = 1; i < insn.size; ++i) {
            if (!inside(pc + i) || file_offset_[pc + i] != file_offset_[pc] + i) return false;
        }
        return true;
    }
    bool follows(const m6502::Insn& insn) const override { return insn.target && inside(*insn.target); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return static_cast<std::uint32_t>(file_offset_[pc]); }

    // The address a file offset is linked at when it is the byte its address shows, else nullopt.
    std::optional<std::uint32_t> shown_at(const Piece& piece, std::uint32_t offset) const {
        const std::uint32_t address = piece.address + (offset - piece.offset);
        if (address >= lynx::kRamEnd || file_offset_[address] != static_cast<std::int64_t>(offset)) return std::nullopt;
        return address;
    }

private:
    Bytes memory_;
    std::vector<std::int64_t> file_offset_;
};

class LynxListing final : public iw::Listing<m6502::Insn> {
public:
    LynxListing(const lynx::Program& program, const Piece& piece, const LynxFlow& flow, const std::map<std::uint32_t, m6502::Insn>& code,
                const decompile::AddressLabels& names)
        : program_(program), piece_(piece), flow_(flow), code_(code), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.file[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return piece_.address + offset - piece_.offset; }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        if (!flow_.shown_at(piece_, offset)) return nullptr;
        const auto it = code_.find(offset);
        if (it == code_.end() || offset + it->second.size > piece_.offset + piece_.size) return nullptr;
        at_offset_ = offset, at_address_ = address;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the 65C02 description's text, the names put in)
        return ca65().fallback(m6502_source::code(m6502_source::Cpu::M65C02, ByteView(program_.file).subspan(at_offset_, insn.size), at_address_, names_));
    }
    std::vector<std::string> data_lines(ByteView data) const override { return ca65().data_lines(data); }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and the address it is shown at)
    const lynx::Program& program_;
    const Piece& piece_;
    const LynxFlow& flow_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const decompile::AddressLabels& names_;
};

std::string header_comment(const lynx::Program& p) {
    if (p.kind == lynx::Kind::Bs93) {
        return std::format("; BS93 object: $80 $08, load address ${:04X} and length ${:04X} (header and body) both big-endian, \"BS93\"", p.load_address, p.block_length);
    }
    return std::format("; LYNX header: bank 0 block {} bytes, bank 1 block {}, version {}, rotation {}", p.bank0_block, p.bank1_block, p.version, p.rotation);
}

}  // namespace

iw::Written write(const lynx::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::vector<Piece> pieces = pieces_of(program);
    const LynxFlow flow(program, pieces);
    const std::vector<std::uint32_t> entries = program.functions(isa::m65sc02_decoder());
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, entries);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;

    iw::Names names;
    if (!program.entries.empty() && flow.inside(program.entries.front())) names[program.entries.front()] = "start";
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);

    // a label can only stand where a line starts, in the segment that shows the address
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // file offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (const Piece& piece : pieces) {
        if (piece.kind != Piece::Kind::Code) continue;
        for (std::uint32_t off = piece.offset; off < piece.offset + piece.size; ++off) {
            if (const auto address = flow.shown_at(piece, off); address && !inside_insn.contains(off)) line_at.insert(*address);
        }
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
    for (const auto& entry : code) {   // absolute operands that name a line's start
        const m6502::Insn& insn = entry.second;
        if (absolute_mode(insn.mode) && insn.mnemonic != "JMP" && insn.mnemonic != "JSR" && insn.operand && line_at.contains(*insn.operand)) {
            labels.try_emplace(*insn.operand, std::format("D_{:04X}", *insn.operand));
        }
    }
    std::set<std::uint32_t> used_registers;
    for (const auto& entry : code) {
        const m6502::Insn& insn = entry.second;
        if (absolute_mode(insn.mode) && insn.operand && lynx::registers().contains(*insn.operand)) used_registers.insert(*insn.operand);
    }

    std::vector<std::string> head = {std::format("; {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     program.kind == lynx::Kind::Cartridge
                                         ? std::format("; Atari Lynx cartridge \"{}\" ({}): {} encrypted first-stage block(s), {} loaded segment(s)", program.name, program.manufacturer,
                                                       program.encrypted_blocks, program.segments.size())
                                         : std::format("; Atari Lynx BS93 object: {} bytes at ${:04X}", program.block_length - lynx::kBs93Header, program.load_address),
                                     "; the file in order: each segment below is a stretch of it; code is linked at the RAM address its loader copies it to", "",
                                     "        .setcpu \"65SC02\"", ""};
    for (const std::uint32_t a : used_registers) head.push_back(m6502_source::equ(lynx::registers().at(a), a, 4));
    if (!used_registers.empty()) head.push_back("");
    translate::l2::Assembly out(head, "ca65", std::string(file_name));

    decompile::AddressLabels names_for_text;   // the registers and the labels: a label wins
    for (const std::uint32_t a : used_registers) names_for_text[a] = lynx::registers().at(a);
    for (const auto& [a, label] : labels) names_for_text[a] = label;

    for (const Piece& piece : pieces) {
        out.part(piece.area, "segment", std::nullopt, piece.size);
        out.extend(ca65().block("segment", {{"name", piece.area}}));
        if (piece.kind == Piece::Kind::Header) {
            out.append(header_comment(program));
            out.extend(ca65().data_lines(ByteView(program.file).subspan(piece.offset, piece.size)));
        } else if (piece.kind == Piece::Kind::Code) {
            iw::Names mine;   // the labels of the addresses this segment shows
            for (const auto& [a, label] : labels) {
                if (a < lynx::kRamEnd && a >= piece.address && a < piece.address + piece.size && flow.shown_at(piece, piece.offset + (a - piece.address))) mine[a] = label;
            }
            LynxListing listing(program, piece, flow, code, names_for_text);
            iw::emit<m6502::Insn>(out, piece.offset, piece.offset + piece.size, mine, functions, listing);
        } else {
            if (piece.offset == lynx::kHeaderSize && program.encrypted_blocks > 0) {
                out.append(std::format("; the boot ROM's first stage: {} count byte and {} encrypted block(s) of {} bytes, decrypted to ${:04X}", 1, program.encrypted_blocks,
                                       lynx::kEncryptedBlock, lynx::kStage1Address));
            }
            out.extend(ca65().data_lines(ByteView(program.file).subspan(piece.offset, piece.size)));
        }
        out.append("");
    }
    const std::string source = ca65().print(iw::text(out) + "\n");   // (said in ca65's)
    const std::string config = link_config(pieces);
    const std::filesystem::path config_path = folder / (stem + ".cfg");
    fs::write_file(config_path, as_bytes(config));

    std::size_t code_total = 0;
    for (const auto& entry : code) code_total += entry.second.size;
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, code_total, program.file.size());
    r.config = config_path;
    const std::string extension = program.kind == lynx::Kind::Cartridge ? ".built.lnx" : ".built.o";
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M65C02, ca65().parse(source, {config, folder}), folder); }, program.file, folder / (stem + extension));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const Booter& booter) {
    lynx::Program program = lynx::read(file);
    if (program.kind == lynx::Kind::Cartridge && booter) booter(program);
    return write(program, file.filename().string(), folder);
}

}  // namespace cb::source::lynx_writer
