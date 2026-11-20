// source/atari7800_writer.cpp -- an Atari 7800 cartridge as assembly source that builds back into it (atari7800_writer.hpp).

#include "source/atari7800_writer.hpp"

#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "source/isa.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari7800_writer {

namespace m6502 = decompile::m6502;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub1{"sub_", 4};
constexpr iw::Spelling kSubN{"sub_", 6};
constexpr iw::Spelling kLocal1{"L_", 4};
constexpr iw::Spelling kLocalN{"L_", 6};

std::uint32_t flow_address(std::size_t bank, std::uint32_t address) { return static_cast<std::uint32_t>(bank) << 16 | address; }

bool writes(std::string_view mnemonic) {
    return mnemonic == "STA" || mnemonic == "STX" || mnemonic == "STY" || mnemonic == "STZ" || mnemonic == "INC" || mnemonic == "DEC" ||
           mnemonic == "ASL" || mnemonic == "LSR" || mnemonic == "ROL" || mnemonic == "ROR";
}

bool names_address(m6502::Mode mode) {
    switch (mode) {
        case m6502::Mode::Zp:
        case m6502::Mode::Zpx:
        case m6502::Mode::Zpy:
        case m6502::Mode::Abs:
        case m6502::Mode::Abx:
        case m6502::Mode::Aby: return true;
        default: return false;
    }
}

const std::string* hardware_name(const m6502::Insn& insn) {
    if (!names_address(insn.mode) || !insn.operand) return nullptr;
    const auto& tia = writes(insn.mnemonic) ? tia_write() : tia_read();
    for (const auto* map : {&tia, &maria(), &riot()}) {
        const auto it = map->find(*insn.operand);
        if (it != map->end()) return &it->second;
    }
    return nullptr;
}

// Flow runs through a bank: a linear ROM is bank 0; a SuperGame's code at $C000 and up is in its fixed bank, and below it in the bank the flow is in.
class A7800Flow final : public iw::CodeModel<m6502::Insn> {
public:
    explicit A7800Flow(const formats::atari7800::Rom& rom) : rom_(rom) {
        for (std::size_t b = 0; b < rom.bank_count(); ++b) memory_.push_back(rom.memory(b));
    }

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override {
        const std::size_t bank = pc >> 16;
        if (bank >= memory_.size()) return std::nullopt;
        std::optional<m6502::Insn> insn = m6502::decode(memory_[bank], pc & 0xFFFF, pc & 0xFFFF);
        if (insn) insn->at = pc;
        return insn;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        const std::size_t bank = insn.at >> 16;
        if (!rom_.supergame) return rom_.offset(0, *insn.target) ? std::optional<std::uint32_t>(*insn.target) : std::nullopt;
        if (*insn.target >= 0xC000) return flow_address(rom_.fixed_bank(), *insn.target);
        if (*insn.target < 0x8000 || bank == rom_.fixed_bank()) return std::nullopt;   // (which bank the fixed bank's code selected is a run-time fact)
        return flow_address(bank, *insn.target);
    }
    bool inside(std::uint32_t pc) const override { return (pc >> 16) < rom_.bank_count() && rom_.offset(pc >> 16, pc & 0xFFFF).has_value(); }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override {
        const auto first = rom_.offset(pc >> 16, pc & 0xFFFF);
        const auto last = rom_.offset(pc >> 16, (pc & 0xFFFF) + insn.size - 1);
        return first && last && *last == *first + insn.size - 1;
    }
    bool follows(const m6502::Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return *rom_.offset(pc >> 16, pc & 0xFFFF); }
    bool finds_functions() const override { return true; }
    bool starts_function(const m6502::Insn& insn) const override { return insn.mnemonic == "JSR"; }

private:
    const formats::atari7800::Rom& rom_;
    std::vector<Bytes> memory_;
};

// One stretch of the ROM: the bytes [offset, offset + size) of it, shown at `base` in `bank`.
struct Stretch {
    std::size_t bank = 0;
    std::uint32_t base = 0;
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
};

std::vector<Stretch> stretches(const formats::atari7800::Rom& rom) {
    if (!rom.supergame) return {{0, rom.base, 0, static_cast<std::uint32_t>(rom.data.size())}};
    std::vector<Stretch> out;
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        out.push_back({b, b == rom.fixed_bank() ? 0xC000u : 0x8000u, static_cast<std::uint32_t>(b * formats::atari7800::kBankSize), static_cast<std::uint32_t>(formats::atari7800::kBankSize)});
    }
    return out;
}

class A7800Listing final : public iw::Listing<m6502::Insn> {
public:
    A7800Listing(translate::l2::Assembly& out, const formats::atari7800::Rom& rom, const Stretch& stretch, const std::map<std::uint32_t, m6502::Insn>& code,
                 const iw::Names& labels, const decompile::AddressLabels& read_names, const decompile::AddressLabels& write_names)
        : out_(out), rom_(rom), stretch_(stretch), code_(code), labels_(labels), read_names_(read_names), write_names_(write_names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return flow_address(stretch_.bank, stretch_.base + offset - stretch_.offset); }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        if (offset + it->second.size > stretch_.offset + stretch_.size) return nullptr;
        if (offset + it->second.size > rom_.data.size() - 6 && offset < rom_.data.size()) return nullptr;   // (the vectors are data)
        at_offset_ = offset, at_address_ = address & 0xFFFF;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the description's text, the names put in)
        return m6502_source::code(m6502_source::Cpu::M6502, ByteView(rom_.data).subspan(at_offset_, insn.size), at_address_,
                                  writes(insn.mnemonic) ? write_names_ : read_names_);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return m6502_source::data_lines(data); }

    // The last six bytes of the ROM are the vectors.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != rom_.data.size() - 6) return 0;
        flush();
        const formats::atari7800::Vectors& v = rom_.vectors;
        std::vector<std::string> words;
        for (const std::uint32_t at : {std::uint32_t{v.nmi}, std::uint32_t{v.reset}, std::uint32_t{v.irq}}) {
            const auto named = labels_.find(flow_address(at >= 0xC000 ? rom_.fixed_bank() : stretch_.bank, at));
            words.push_back(named != labels_.end() ? named->second : std::format("0x{:04x}", at));
        }
        out_.append("        .half " + text::join(words, ","));
        return 6;
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and the address it is shown at)
    translate::l2::Assembly& out_;
    const formats::atari7800::Rom& rom_;
    const Stretch& stretch_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& read_names_;
    const decompile::AddressLabels& write_names_;
};

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

std::string header_note(const formats::atari7800::Rom& rom) {
    return std::format("; header: version {}, \"{}\", {} bytes of ROM, cartridge type ${:04X}, controllers {}/{}, TV {}, save device {}", rom.version, rom.title,
                       rom.declared_size, rom.cart_type, rom.controller1, rom.controller2, rom.tv, rom.save);
}

}  // namespace

const std::map<std::uint32_t, std::string>& tia_write() {
    static const std::map<std::uint32_t, std::string> table = {{0x01, "INPTCTRL"}, {0x15, "AUDC0"}, {0x16, "AUDC1"}, {0x17, "AUDF0"},
                                                               {0x18, "AUDF1"},    {0x19, "AUDV0"}, {0x1A, "AUDV1"}};
    return table;
}

const std::map<std::uint32_t, std::string>& tia_read() {
    static const std::map<std::uint32_t, std::string> table = {{0x08, "INPT0"}, {0x09, "INPT1"}, {0x0A, "INPT2"},
                                                               {0x0B, "INPT3"}, {0x0C, "INPT4"}, {0x0D, "INPT5"}};
    return table;
}

const std::map<std::uint32_t, std::string>& maria() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x20, "BACKGRND"}, {0x21, "P0C1"}, {0x22, "P0C2"}, {0x23, "P0C3"}, {0x24, "WSYNC"}, {0x25, "P1C1"}, {0x26, "P1C2"}, {0x27, "P1C3"},
        {0x28, "MSTAT"},    {0x29, "P2C1"}, {0x2A, "P2C2"}, {0x2B, "P2C3"}, {0x2C, "DPPH"},  {0x2D, "P3C1"}, {0x2E, "P3C2"}, {0x2F, "P3C3"},
        {0x30, "DPPL"},     {0x31, "P4C1"}, {0x32, "P4C2"}, {0x33, "P4C3"}, {0x34, "CHBASE"}, {0x35, "P5C1"}, {0x36, "P5C2"}, {0x37, "P5C3"},
        {0x38, "OFFSET"},   {0x39, "P6C1"}, {0x3A, "P6C2"}, {0x3B, "P6C3"}, {0x3C, "CTRL"},  {0x3D, "P7C1"}, {0x3E, "P7C2"}, {0x3F, "P7C3"}};
    return table;
}

const std::map<std::uint32_t, std::string>& riot() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x280, "SWCHA"}, {0x281, "SWACNT"}, {0x282, "SWCHB"}, {0x283, "CTLSWB"}, {0x284, "INTIM"},
        {0x285, "TIMINT"}, {0x294, "TIM1T"}, {0x295, "TIM8T"}, {0x296, "TIM64T"}, {0x297, "T1024T"}};
    return table;
}

iw::Written write(const formats::atari7800::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const bool banked = rom.supergame;
    const iw::Spelling& sub = banked ? kSubN : kSub1;
    const iw::Spelling& local = banked ? kLocalN : kLocal1;
    const A7800Flow flow(rom);
    const std::vector<Stretch> parts = stretches(rom);

    // the vectors: where each stands, and the bank its code is in
    const auto vector_address = [&](std::uint16_t v) { return flow_address(rom.supergame && v >= 0xC000 ? rom.fixed_bank() : 0, v); };
    std::vector<std::uint32_t> entries;
    iw::Names names;
    for (const auto& [vector, label] : {std::pair{rom.vectors.reset, "reset"}, std::pair{rom.vectors.nmi, "nmi"}, std::pair{rom.vectors.irq, "irq"}}) {
        const std::uint32_t at = vector_address(vector);
        if (!rom.offset(at >> 16, vector)) continue;
        entries.push_back(at);
        names.try_emplace(at, label);
    }
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, entries);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, sub);

    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // ROM offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (const Stretch& s : parts) {
        for (std::uint32_t off = s.offset; off < s.offset + s.size; ++off) {
            if (!inside_insn.contains(off)) line_at.insert(flow_address(s.bank, s.base + off - s.offset));
        }
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, local);
    for (const auto& entry : code) {   // absolute operands that name a line's start
        const m6502::Insn& insn = entry.second;
        if (!absolute_mode(insn.mode) || !insn.operand || *insn.operand < 0x100) continue;
        const std::uint32_t at = flow_address(rom.supergame && *insn.operand >= 0xC000 ? rom.fixed_bank() : insn.at >> 16, *insn.operand);
        if (line_at.contains(at)) labels.try_emplace(at, std::format("D_{:0{}X}", banked ? at : *insn.operand, banked ? 6 : 4));
    }
    std::map<std::uint32_t, std::set<std::string>> equates;
    for (const auto& entry : code) {
        if (const std::string* name = hardware_name(entry.second)) equates[*entry.second.operand].insert(*name);
    }

    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     header_note(rom),
                                     "; (6502 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu 6502 --sleigh`; the file is the header, then each stretch in order)",
                                     ""};
    for (const auto& [value, set] : equates) {
        for (const std::string& name : set) head.push_back(m6502_source::equ(name, value, value > 0xFF ? 4 : 2));
    }
    if (!equates.empty()) head.push_back("");
    translate::l2::Assembly out(head, "sleigh-6502", std::string(file_name));
    out.part("HEADER", "segment", std::nullopt, formats::atari7800::kHeaderSize);
    out.append("        .org 0x0000");
    out.extend(iw::commented(m6502_source::data_lines(rom.header), "the A78 header"));
    out.append("");

    for (const Stretch& s : parts) {
        decompile::AddressLabels read_names, write_names;
        for (const auto& [value, set] : equates) {
            for (const std::string& name : set) {
                if ((tia_read().contains(value) && tia_read().at(value) == name) || (riot().contains(value) && riot().at(value) == name) ||
                    (maria().contains(value) && maria().at(value) == name)) {
                    read_names[value] = name;
                }
                if ((tia_write().contains(value) && tia_write().at(value) == name) || (riot().contains(value) && riot().at(value) == name) ||
                    (maria().contains(value) && maria().at(value) == name)) {
                    write_names[value] = name;
                }
            }
        }
        for (const auto& [at, label] : labels) {
            if ((at >> 16) != s.bank) continue;
            read_names[at & 0xFFFF] = label;
            write_names[at & 0xFFFF] = label;
        }
        out.part(banked ? std::format("BANK{}", s.bank) : std::string("ROM"), "segment", std::nullopt, s.size);
        out.append(std::format("; file offset 0x{:04x}", formats::atari7800::kHeaderSize + s.offset));
        out.append(std::format("        .org 0x{:04x}", s.base));
        A7800Listing listing(out, rom, s, code, labels, read_names, write_names);
        iw::emit<m6502::Insn>(out, s.offset, s.offset + s.size, labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    Bytes original = rom.header;
    original.insert(original.end(), rom.data.begin(), rom.data.end());
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M6502, source, folder); }, original, folder / (stem + ".built.a78"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::atari7800::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::atari7800_writer
