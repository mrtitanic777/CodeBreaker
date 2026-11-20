// cb-unity: alone
// source/atari5200_writer.cpp -- an Atari 5200 cartridge as assembly source that builds back into it (atari5200_writer.hpp).

#include "source/atari5200_writer.hpp"

#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "source/isa.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari5200_writer {

namespace m6502 = decompile::m6502;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr std::size_t kTail = 4;   // flags, start address

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
    const bool store = writes(insn.mnemonic);
    for (const auto* map : {store ? &gtia_write() : &gtia_read(), store ? &antic_write() : &antic_read(), store ? &pokey_write() : &pokey_read()}) {
        const auto it = map->find(*insn.operand);
        if (it != map->end()) return &it->second;
    }
    return nullptr;
}

class A5200Flow final : public iw::CodeModel<m6502::Insn> {
public:
    explicit A5200Flow(const formats::atari5200::Rom& rom) : rom_(rom), memory_(rom.memory()) {}

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override {
        if (pc > 0xFFFF) return std::nullopt;
        return m6502::decode(memory_, pc, pc);
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target || !rom_.offset(*insn.target)) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return rom_.offset(pc).has_value(); }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override { return rom_.offset(pc) && rom_.offset(pc + insn.size - 1); }
    bool follows(const m6502::Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return *rom_.offset(pc); }
    bool finds_functions() const override { return true; }
    bool starts_function(const m6502::Insn& insn) const override { return insn.mnemonic == "JSR"; }

private:
    const formats::atari5200::Rom& rom_;
    Bytes memory_;
};

class A5200Listing final : public iw::Listing<m6502::Insn> {
public:
    A5200Listing(translate::l2::Assembly& out, const formats::atari5200::Rom& rom, const std::map<std::uint32_t, m6502::Insn>& code, const iw::Names& labels,
                 const decompile::AddressLabels& read_names, const decompile::AddressLabels& write_names)
        : out_(out), rom_(rom), code_(code), labels_(labels), read_names_(read_names), write_names_(write_names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.base + offset; }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        if (offset + it->second.size > rom_.data.size() - kTail) return nullptr;   // (the header is data)
        at_offset_ = offset, at_address_ = address;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the description's text, the names put in)
        return m6502_source::code(m6502_source::Cpu::M6502, ByteView(rom_.data).subspan(at_offset_, insn.size), at_address_,
                                  writes(insn.mnemonic) ? write_names_ : read_names_);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return m6502_source::data_lines(data); }

    // The last four bytes are the cartridge header: two flag bytes and the start address.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != rom_.data.size() - kTail) return 0;
        flush();
        out_.append(std::format("        .byte 0x{:02x},0x{:02x}", rom_.flag_a, rom_.flag_b));
        const auto named = labels_.find(rom_.start);
        out_.append("        .half " + (named != labels_.end() ? named->second : std::format("0x{:04x}", rom_.start)));
        return kTail;
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and the address it is shown at)
    translate::l2::Assembly& out_;
    const formats::atari5200::Rom& rom_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& read_names_;
    const decompile::AddressLabels& write_names_;
};

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

bool names_the(const std::map<std::uint32_t, std::string>& table, std::uint32_t value, const std::string& name) {
    const auto it = table.find(value);
    return it != table.end() && it->second == name;
}

}  // namespace

const std::map<std::uint32_t, std::string>& gtia_write() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xC000, "HPOSP0"}, {0xC001, "HPOSP1"}, {0xC002, "HPOSP2"}, {0xC003, "HPOSP3"}, {0xC004, "HPOSM0"}, {0xC005, "HPOSM1"}, {0xC006, "HPOSM2"},
        {0xC007, "HPOSM3"}, {0xC008, "SIZEP0"}, {0xC009, "SIZEP1"}, {0xC00A, "SIZEP2"}, {0xC00B, "SIZEP3"}, {0xC00C, "SIZEM"},  {0xC00D, "GRAFP0"},
        {0xC00E, "GRAFP1"}, {0xC00F, "GRAFP2"}, {0xC010, "GRAFP3"}, {0xC011, "GRAFM"},  {0xC012, "COLPM0"}, {0xC013, "COLPM1"}, {0xC014, "COLPM2"},
        {0xC015, "COLPM3"}, {0xC016, "COLPF0"}, {0xC017, "COLPF1"}, {0xC018, "COLPF2"}, {0xC019, "COLPF3"}, {0xC01A, "COLBK"},  {0xC01B, "PRIOR"},
        {0xC01C, "VDELAY"}, {0xC01D, "GRACTL"}, {0xC01E, "HITCLR"}, {0xC01F, "CONSOL"}};
    return table;
}

const std::map<std::uint32_t, std::string>& gtia_read() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xC000, "M0PF"}, {0xC001, "M1PF"}, {0xC002, "M2PF"}, {0xC003, "M3PF"}, {0xC004, "P0PF"}, {0xC005, "P1PF"}, {0xC006, "P2PF"}, {0xC007, "P3PF"},
        {0xC008, "M0PL"}, {0xC009, "M1PL"}, {0xC00A, "M2PL"}, {0xC00B, "M3PL"}, {0xC00C, "P0PL"}, {0xC00D, "P1PL"}, {0xC00E, "P2PL"}, {0xC00F, "P3PL"},
        {0xC010, "TRIG0"}, {0xC011, "TRIG1"}, {0xC012, "TRIG2"}, {0xC013, "TRIG3"}, {0xC014, "PAL"}, {0xC01F, "CONSOL"}};
    return table;
}

const std::map<std::uint32_t, std::string>& antic_write() {
    static const std::map<std::uint32_t, std::string> table = {{0xD400, "DMACTL"}, {0xD401, "CHACTL"}, {0xD402, "DLISTL"}, {0xD403, "DLISTH"},
                                                               {0xD404, "HSCROL"}, {0xD405, "VSCROL"}, {0xD407, "PMBASE"}, {0xD409, "CHBASE"},
                                                               {0xD40A, "WSYNC"},  {0xD40E, "NMIEN"},  {0xD40F, "NMIRES"}};
    return table;
}

const std::map<std::uint32_t, std::string>& antic_read() {
    static const std::map<std::uint32_t, std::string> table = {{0xD40B, "VCOUNT"}, {0xD40C, "PENH"}, {0xD40D, "PENV"}, {0xD40F, "NMIST"}};
    return table;
}

const std::map<std::uint32_t, std::string>& pokey_write() {
    static const std::map<std::uint32_t, std::string> table = {{0xE800, "AUDF1"},  {0xE801, "AUDC1"}, {0xE802, "AUDF2"},  {0xE803, "AUDC2"},
                                                               {0xE804, "AUDF3"},  {0xE805, "AUDC3"}, {0xE806, "AUDF4"},  {0xE807, "AUDC4"},
                                                               {0xE808, "AUDCTL"}, {0xE809, "STIMER"}, {0xE80A, "SKREST"}, {0xE80B, "POTGO"},
                                                               {0xE80D, "SEROUT"}, {0xE80E, "IRQEN"}, {0xE80F, "SKCTL"}};
    return table;
}

const std::map<std::uint32_t, std::string>& pokey_read() {
    static const std::map<std::uint32_t, std::string> table = {{0xE800, "POT0"},   {0xE801, "POT1"},   {0xE802, "POT2"},   {0xE803, "POT3"},
                                                               {0xE804, "POT4"},   {0xE805, "POT5"},   {0xE806, "POT6"},   {0xE807, "POT7"},
                                                               {0xE808, "ALLPOT"}, {0xE809, "KBCODE"}, {0xE80A, "RANDOM"}, {0xE80D, "SERIN"},
                                                               {0xE80E, "IRQST"},  {0xE80F, "SKSTAT"}};
    return table;
}

iw::Written write(const formats::atari5200::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const A5200Flow flow(rom);

    std::vector<std::uint32_t> entries = {rom.start};
    iw::Names names;
    names.try_emplace(rom.start, "start");
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, entries);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);

    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // ROM offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (std::uint32_t off = 0; off < rom.data.size(); ++off) {
        if (!inside_insn.contains(off)) line_at.insert(rom.base + off);
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
    for (const auto& entry : code) {   // absolute operands that name a line's start
        const m6502::Insn& insn = entry.second;
        if (!absolute_mode(insn.mode) || !insn.operand || *insn.operand < 0x100) continue;
        if (line_at.contains(*insn.operand)) labels.try_emplace(*insn.operand, std::format("D_{:04X}", *insn.operand));
    }
    std::map<std::uint32_t, std::set<std::string>> equates;
    for (const auto& entry : code) {
        if (const std::string* name = hardware_name(entry.second)) equates[*entry.second.operand].insert(*name);
    }

    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("; cartridge: {} bytes at ${:04X}, flags ${:02X} ${:02X}, start ${:04X}", rom.data.size(), rom.base, rom.flag_a, rom.flag_b, rom.start),
                                     "; (6502 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu 6502 --sleigh`)",
                                     ""};
    decompile::AddressLabels read_names, write_names;
    for (const auto& [value, set] : equates) {
        for (const std::string& name : set) {
            head.push_back(m6502_source::equ(name, value, 4));
            if (names_the(gtia_read(), value, name) || names_the(antic_read(), value, name) || names_the(pokey_read(), value, name)) read_names[value] = name;
            if (names_the(gtia_write(), value, name) || names_the(antic_write(), value, name) || names_the(pokey_write(), value, name)) write_names[value] = name;
        }
    }
    if (!equates.empty()) head.push_back("");
    for (const auto& [at, label] : labels) {
        read_names[at] = label;
        write_names[at] = label;
    }
    translate::l2::Assembly out(head, "sleigh-6502", std::string(file_name));
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:04x}", rom.base));
    A5200Listing listing(out, rom, code, labels, read_names, write_names);
    iw::emit<m6502::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M6502, source, folder); }, rom.data, folder / (stem + ".built.a52"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::atari5200::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::atari5200_writer
