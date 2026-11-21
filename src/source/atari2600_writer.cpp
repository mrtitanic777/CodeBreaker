// source/atari2600_writer.cpp -- an Atari 2600 cartridge as assembly source that builds back into it (atari2600_writer.hpp).

#include "source/atari2600_writer.hpp"

#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/isa.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari2600_writer {

namespace m6502 = decompile::m6502;
namespace iw = image_writer;

namespace {

// The Atari 2600's source is DASM's (config/dialects/dasm.json).
const dialect::Dialect& dasm() { static const dialect::Dialect& d = dialect::find("dasm"); return d; }
constexpr iw::Spelling kSub1{"sub_", 4};
constexpr iw::Spelling kSubN{"sub_", 6};
constexpr iw::Spelling kLocal1{"L_", 4};
constexpr iw::Spelling kLocalN{"L_", 6};

// A flow address is `bank << 16 | address in the window`; the instruction keeps it in `at`.
std::uint32_t flow_address(std::size_t bank, std::uint32_t folded) { return static_cast<std::uint32_t>(bank) << 16 | folded; }

// Does the instruction write the address it names (so it is the TIA's write name that stands)?
bool writes(std::string_view mnemonic) {
    return mnemonic == "STA" || mnemonic == "STX" || mnemonic == "STY" || mnemonic == "STZ" || mnemonic == "INC" || mnemonic == "DEC" ||
           mnemonic == "ASL" || mnemonic == "LSR" || mnemonic == "ROL" || mnemonic == "ROR";
}

// Does the instruction's operand name a memory address (and not a value, a branch target or a pointer)?
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
    const auto& table = writes(insn.mnemonic) ? tia_write() : tia_read();
    for (const auto* map : {&table, &riot()}) {
        const auto it = map->find(*insn.operand);
        if (it != map->end()) return &it->second;
    }
    return nullptr;
}

class A2600Flow final : public iw::CodeModel<m6502::Insn> {
public:
    explicit A2600Flow(const formats::atari2600::Rom& rom) : rom_(rom) {
        for (std::size_t b = 0; b < rom.bank_count(); ++b) memory_.push_back(rom.memory(b));
    }

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override {
        const std::size_t bank = pc >> 16;
        if (bank >= memory_.size()) return std::nullopt;
        const std::uint32_t at = pc & 0xFFFF;
        std::optional<m6502::Insn> insn = m6502::decode(memory_[bank], at, at);
        if (insn) insn->at = pc;
        return insn;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        const auto folded = rom_.fold(*insn.target);
        if (!folded) return std::nullopt;
        return flow_address(insn.at >> 16, *folded);
    }
    bool inside(std::uint32_t pc) const override {
        const std::uint32_t at = pc & 0xFFFF;
        return (pc >> 16) < rom_.bank_count() && at >= rom_.window && at < rom_.window + rom_.bank_size;
    }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override { return (pc & 0xFFFF) + insn.size <= rom_.window + rom_.bank_size; }
    bool follows(const m6502::Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return rom_.offset(pc >> 16, pc & 0xFFFF); }
    bool finds_functions() const override { return true; }
    bool starts_function(const m6502::Insn& insn) const override { return insn.mnemonic == "JSR"; }

private:
    const formats::atari2600::Rom& rom_;
    std::vector<Bytes> memory_;
};

// One bank of the cartridge written as lines.
class A2600Listing final : public iw::Listing<m6502::Insn> {
public:
    A2600Listing(translate::l2::Assembly& out, const formats::atari2600::Rom& rom, std::size_t bank, const std::map<std::uint32_t, m6502::Insn>& code,
                 const iw::Names& labels, const decompile::AddressLabels& read_names, const decompile::AddressLabels& write_names)
        : out_(out), rom_(rom), bank_(bank), code_(code), labels_(labels), read_names_(read_names), write_names_(write_names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override {
        return flow_address(bank_, rom_.window + static_cast<std::uint32_t>(offset - bank_ * rom_.bank_size));
    }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        const std::uint32_t end = static_cast<std::uint32_t>((bank_ + 1) * rom_.bank_size);
        if (offset + it->second.size > end - 4) return nullptr;   // (the vectors are data)
        at_offset_ = offset, at_address_ = address & 0xFFFF;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the description's text, the names put in)
        const decompile::AddressLabels& names = writes(insn.mnemonic) ? write_names_ : read_names_;
        return dasm().line(m6502_source::in_dialect(dasm(), insn, names, m6502_source::code(m6502_source::Cpu::M6502, ByteView(rom_.data).subspan(at_offset_, insn.size), at_address_, names)));   // (said in DASM's)
    }
    std::vector<std::string> data_lines(ByteView data) const override { return dasm().data_lines(data); }
    std::string label_line(const std::string& name) const override { return dasm().label_line(name); }

    // The last four bytes of a bank are the vectors.
    std::uint32_t special(std::uint32_t offset, std::uint32_t address, const iw::Flush& flush) override {
        if (offset != (bank_ + 1) * rom_.bank_size - 4) return 0;
        flush();
        const formats::atari2600::Vectors& v = rom_.vectors[bank_];
        std::vector<std::string> words;
        for (const std::uint32_t at : {std::uint32_t{v.reset}, std::uint32_t{v.irq}}) {
            const auto named = labels_.find(flow_address(bank_, at));
            words.push_back(named != labels_.end() ? named->second : std::format("0x{:04x}", at));
        }
        out_.append(dasm().line("        .half " + text::join(words, ",")));
        (void)address;
        return 4;
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and the window address it is shown at)
    translate::l2::Assembly& out_;
    const formats::atari2600::Rom& rom_;
    std::size_t bank_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& read_names_;
    const decompile::AddressLabels& write_names_;
};

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

}  // namespace

const std::map<std::uint32_t, std::string>& tia_write() {
    // config/knowledge/atari2600/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("atari2600", "tia_write");
    return table;
}

const std::map<std::uint32_t, std::string>& tia_read() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("atari2600", "tia_read");
    return table;
}

const std::map<std::uint32_t, std::string>& riot() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("atari2600", "riot");
    return table;
}

iw::Written write(const formats::atari2600::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const bool banked = rom.bank_count() > 1;
    const iw::Spelling& sub = banked ? kSubN : kSub1;
    const iw::Spelling& local = banked ? kLocalN : kLocal1;
    const A2600Flow flow(rom);

    std::vector<std::uint32_t> entries;
    iw::Names names;
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        const std::string prefix = banked ? std::format("bank{}_", b) : "";
        const bool start = b == rom.start_bank();
        for (const auto& [vector, label] : {std::pair{rom.vectors[b].reset, "reset"}, std::pair{rom.vectors[b].irq, "irq"}}) {
            if (const auto folded = rom.fold(vector)) {
                const std::uint32_t at = flow_address(b, *folded);
                entries.push_back(at);
                names.try_emplace(at, (start ? "" : prefix) + label);
            }
        }
    }
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, entries);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, sub);

    // a label can only stand where a line starts
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // file offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        for (std::uint32_t off = static_cast<std::uint32_t>(b * rom.bank_size); off < (b + 1) * rom.bank_size; ++off) {
            if (!inside_insn.contains(off)) line_at.insert(flow_address(b, rom.window + off - static_cast<std::uint32_t>(b * rom.bank_size)));
        }
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, local);
    for (const auto& entry : code) {   // absolute operands that name a line's start (exactly: a mirror's address is left as it is)
        const m6502::Insn& insn = entry.second;
        const std::uint32_t at = flow_address(insn.at >> 16, insn.operand.value_or(0));
        if (absolute_mode(insn.mode) && insn.operand && rom.fold(*insn.operand) == insn.operand && line_at.contains(at)) {
            labels.try_emplace(at, std::format("D_{:0{}X}", banked ? at : *insn.operand, banked ? 6 : 4));
        }
    }
    std::map<std::uint32_t, std::set<std::string>> equates;   // the register names the code uses, by address
    for (const auto& entry : code) {
        if (const std::string* name = hardware_name(entry.second)) equates[*entry.second.operand].insert(*name);
    }

    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("; Atari 2600, {} bytes, bank switching {}, {} bank{}", rom.data.size(), formats::atari2600::to_string(rom.scheme),
                                                 rom.bank_count(), banked ? "s" : "")};
    for (const std::string& l : dasm().block("prologue", {{"stem", stem}, {"ext", "a26"}})) head.push_back(l);
    for (const auto& [value, set] : equates) {
        for (const std::string& name : set) head.push_back(dasm().line(m6502_source::equ(name, value, value > 0xFF ? 4 : 2)));
    }
    if (!equates.empty()) head.push_back("");
    translate::l2::Assembly out(head, "dasm", std::string(file_name));

    // the names of the registers and the labels by address, per bank, for the instruction text (a label wins)
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        decompile::AddressLabels read_names, write_names;
        for (const auto& [value, set] : equates) {
            for (const std::string& name : set) {
                if (tia_read().contains(value) && tia_read().at(value) == name) read_names[value] = name;
                if (riot().contains(value) && riot().at(value) == name) {
                    read_names[value] = name;
                    write_names[value] = name;
                }
                if (tia_write().contains(value) && tia_write().at(value) == name) write_names[value] = name;
            }
        }
        for (const auto& [at, label] : labels) {
            if ((at >> 16) != b) continue;
            read_names[static_cast<std::uint16_t>(at & 0xFFFF)] = label;
            write_names[static_cast<std::uint16_t>(at & 0xFFFF)] = label;
        }
        out.part(std::format("BANK{}", b), "segment", std::nullopt, rom.bank_size);
        out.extend(dasm().block("bank", {{"offset", dasm().number(b * rom.bank_size, 4)}, {"window", dasm().number(rom.window, 4)}}));
        A2600Listing listing(out, rom, b, code, labels, read_names, write_names);
        iw::emit<m6502::Insn>(out, static_cast<std::uint32_t>(b * rom.bank_size), static_cast<std::uint32_t>((b + 1) * rom.bank_size), labels, functions,
                              listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M6502, dasm().parse(source), folder); }, rom.data, folder / (stem + ".built.a26"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::atari2600::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::atari2600_writer
