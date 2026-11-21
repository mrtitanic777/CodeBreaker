// source/nes_writer.cpp -- a NES program as assembly source that builds back into it (nes_writer.hpp).

#include "source/nes_writer.hpp"

#include <format>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/isa.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::nes_writer {

namespace m6502 = decompile::m6502;
namespace iw = image_writer;

namespace {

constexpr std::uint32_t kPrgBase = 0x8000;
constexpr std::uint32_t kVectorsAt = 0xFFFA;
constexpr std::uint32_t kBankSize = 0x4000;
constexpr iw::Spelling kSub{"sub_", 4};
// The NES's source is ca65's (config/dialects/ca65.json).
const dialect::Dialect& ca65() { static const dialect::Dialect& d = dialect::find("ca65"); return d; }
constexpr iw::Spelling kLocal{"L_", 4};

// Flow runs through the CPU's address space: $8000-$FFFF is the program, and the PRG offset an address is shown at files the
// instruction (a bank's mirror shows the same bytes).
class NesFlow final : public iw::CodeModel<m6502::Insn> {
public:
    explicit NesFlow(const formats::nes::Rom& rom) : rom_(rom), memory_(rom.memory()) {}

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override { return m6502::decode(memory_, pc, pc); }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc >= kPrgBase && pc <= 0xFFFF; }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override {
        const auto first = rom_.mapped(pc);
        const auto last = rom_.mapped(pc + insn.size - 1);
        return first && last && *last == *first + insn.size - 1;
    }
    bool follows(const m6502::Insn& insn) const override { return insn.target && *insn.target >= kPrgBase; }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return *rom_.mapped(pc); }   // (fits() has seen it mapped)

private:
    const formats::nes::Rom& rom_;
    Bytes memory_;
};

// One bank of the PRG written as lines.
class NesListing final : public iw::Listing<m6502::Insn> {
public:
    NesListing(translate::l2::Assembly& out, const formats::nes::Rom& rom, const Bank& bank, const std::map<std::uint32_t, m6502::Insn>& code,
               const iw::Names& labels, const decompile::AddressLabels& names)
        : out_(out), rom_(rom), bank_(bank), code_(code), labels_(labels), names_(names),
          vectors_here_(bank.base + bank.size == 0x10000) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.prg[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return bank_.base + offset - bank_.offset; }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        const m6502::Insn& insn = it->second;
        if (offset + insn.size > bank_.offset + bank_.size || (vectors_here_ && address + insn.size > kVectorsAt)) return nullptr;
        at_offset_ = offset, at_address_ = address;   // (text() of this instruction follows)
        return &insn;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {
        return ca65().line(ca65().fallback(m6502_source::code(m6502_source::Cpu::M6502, ByteView(rom_.prg).subspan(at_offset_, insn.size), at_address_, names_)));   // (the description's text, the names put in, said in ca65's)
    }
    std::vector<std::string> data_lines(ByteView data) const override { return ca65().data_lines(data); }
    // The last six bytes of the bank that holds them are the vectors.
    std::uint32_t special(std::uint32_t, std::uint32_t address, const iw::Flush& flush) override {
        if (!vectors_here_ || address != kVectorsAt) return 0;
        flush();
        const formats::nes::Vectors& v = rom_.vectors;
        std::vector<std::string> words;
        for (const std::uint32_t at : {std::uint32_t{v.nmi}, std::uint32_t{v.reset}, std::uint32_t{v.irq}}) {
            const auto named = labels_.find(at);
            words.push_back(named != labels_.end() ? named->second : std::format("0x{:04x}", at));
        }
        out_.append(ca65().line("        .half " + text::join(words, ",")));
        return 6;
    }

    static std::string byte_list(ByteView data) {
        std::vector<std::string> items;
        for (const std::uint8_t b : data) items.push_back(ca65().number(b, 2));
        return text::join(items, ", ");
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the instruction code_at() last gave: its PRG offset, the address it is shown at)
    translate::l2::Assembly& out_;
    const formats::nes::Rom& rom_;
    const Bank& bank_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& names_;
    bool vectors_here_;
};

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

// The ld65 configuration: where each segment of the source loads (config/dialects/ca65.json reads it back).
std::string link_config(const formats::nes::Rom& rom, const std::vector<Bank>& banks) {
    std::vector<std::string> cfg = {"MEMORY {", "    HEADER: start = $0000, size = $0010, fill = yes, file = %O;"};
    if (!rom.trainer.empty()) cfg.push_back("    TRAINER: start = $7000, size = $0200, fill = yes, file = %O;");
    for (const Bank& b : banks) cfg.push_back(std::format("    PRG{}: start = ${:04X}, size = ${:04X}, fill = yes, file = %O;", b.segment.substr(4), b.base, b.size));
    if (!rom.chr.empty()) cfg.push_back(std::format("    CHR: start = $0000, size = ${:04X}, fill = yes, file = %O;", rom.chr.size()));
    cfg.insert(cfg.end(), {"}", "SEGMENTS {", "    HEADER: load = HEADER, type = ro;"});
    if (!rom.trainer.empty()) cfg.push_back("    TRAINER: load = TRAINER, type = ro;");
    for (const Bank& b : banks) cfg.push_back(std::format("    {}: load = PRG{}, type = ro;", b.segment, b.segment.substr(4)));
    if (!rom.chr.empty()) cfg.push_back("    CHARS: load = CHR, type = ro;");
    cfg.push_back("}");
    return text::join(cfg, "\n") + "\n";
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/nes/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("nes", "hardware");
    return table;
}

std::vector<Bank> banks(const formats::nes::Rom& rom) {
    if (rom.prg.size() == kBankSize) return {{"BANK0", 0xC000, 0, kBankSize}};
    if (rom.mapper == 0 || rom.prg.size() <= 0x8000) return {{"BANK0", kPrgBase, 0, static_cast<std::uint32_t>(rom.prg.size())}};
    std::vector<Bank> out;
    const std::size_t count = rom.bank_count();
    for (std::size_t i = 0; i < count; ++i) {
        out.push_back({std::format("BANK{}", i), i == count - 1 ? 0xC000u : kPrgBase, static_cast<std::uint32_t>(i * kBankSize), kBankSize});
    }
    return out;
}

iw::Written write(const formats::nes::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const NesFlow flow(rom);
    const std::vector<std::uint32_t> entries = rom.functions(isa::m6502_decoder());
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, entries);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;
    const std::vector<Bank> bank_list = banks(rom);

    iw::Names names = {{rom.vectors.reset, "reset"}};
    names.try_emplace(rom.vectors.nmi, "nmi");
    names.try_emplace(rom.vectors.irq, "irq");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);

    // a label can only stand where a line starts, in the bank shown at that address
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // PRG offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (const Bank& b : bank_list) {
        for (std::uint32_t off = b.offset; off < b.offset + b.size; ++off) {
            if (!inside_insn.contains(off)) line_at.insert(b.base + off - b.offset);
        }
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
    for (const auto& entry : code) {   // absolute operands that name a line's start
        const m6502::Insn& insn = entry.second;
        if ((absolute_mode(insn.mode) || insn.mode == m6502::Mode::Ind) && insn.mnemonic != "JMP" && insn.mnemonic != "JSR" && insn.operand &&
            line_at.contains(*insn.operand) && *insn.operand >= kPrgBase) {
            labels.try_emplace(*insn.operand, std::format("D_{:04X}", *insn.operand));
        }
    }
    std::set<std::uint32_t> used_registers;
    for (const auto& entry : code) {
        const m6502::Insn& insn = entry.second;
        if (absolute_mode(insn.mode) && insn.operand && hardware().contains(*insn.operand)) used_registers.insert(*insn.operand);
    }

    std::vector<std::string> head = {std::format("; {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("; mapper {}, {} KB PRG, {} KB CHR, {} mirroring{}", rom.mapper, rom.prg.size() / 1024,
                                                 rom.chr.size() / 1024, formats::nes::to_string(rom.mirroring),
                                                 rom.battery ? ", battery-backed RAM" : ""),
                                     ""};
    for (const std::uint32_t a : used_registers) head.push_back(ca65().line(m6502_source::equ(hardware().at(a), a, 4)));
    if (!used_registers.empty()) head.push_back("");
    for (const std::string& l : ca65().block("segment", {{"name", "HEADER"}})) head.push_back(l);
    head.push_back("        .byte " + NesListing::byte_list(ByteView(rom.header).first(std::min<std::size_t>(16, rom.header.size()))));
    head.push_back("");
    translate::l2::Assembly out(head, "ca65", std::string(file_name));
    if (!rom.trainer.empty()) {
        out.extend(ca65().block("segment", {{"name", "TRAINER"}}));
        out.append("        .byte " + NesListing::byte_list(rom.trainer));
        out.append("");
    }

    decompile::AddressLabels names_for_text;   // the registers and the labels: a label wins
    for (const std::uint32_t a : used_registers) names_for_text[a] = hardware().at(a);
    for (const auto& [a, label] : labels) names_for_text[a] = label;

    for (const Bank& b : bank_list) {
        out.part(b.segment, "segment", std::nullopt, b.size);
        out.extend(ca65().block("segment", {{"name", b.segment}}));
        NesListing listing(out, rom, b, code, labels, names_for_text);
        iw::emit<m6502::Insn>(out, b.offset, b.offset + b.size, labels, functions, listing);
        out.append("");
    }
    if (!rom.chr.empty()) {
        out.part("CHARS", "segment", std::nullopt, rom.chr.size());
        fs::write_file(folder / (stem + ".chr"), rom.chr);   // (the CHR beside the source, which names it)
        out.extend(ca65().block("segment", {{"name", "CHARS"}}));
        out.append(std::format("        .incbin \"{}.chr\"", stem));
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    const std::string config = link_config(rom, bank_list);
    const std::filesystem::path config_path = folder / (stem + ".cfg");
    fs::write_file(config_path, as_bytes(config));

    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), rom.prg.size());
    r.config = config_path;
    Bytes original = rom.header;
    original.insert(original.end(), rom.trainer.begin(), rom.trainer.end());
    original.insert(original.end(), rom.prg.begin(), rom.prg.end());
    original.insert(original.end(), rom.chr.begin(), rom.chr.end());
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M6502, ca65().parse(source, {config, folder}), folder); }, original, folder / (stem + ".built.nes"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::nes::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::nes_writer
