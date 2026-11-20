// source/sms_writer.cpp -- a Master System / Game Gear program as assembly source that builds back into it (sms_writer.hpp).

#include "source/sms_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "decompile/z80.hpp"
#include "knowledge/package.hpp"
#include "source/isa.hpp"
#include "source/z80_source.hpp"

namespace cb::source::sms_writer {

namespace z80 = decompile::z80;
namespace iw = image_writer;

namespace {

constexpr std::uint32_t kPowerOnEnd = 0xC000;   // the three slots
constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};

// Flow runs through the three slots as they are at power-on.
class SmsFlow final : public iw::CodeModel<z80::Insn> {
public:
    explicit SmsFlow(const formats::sms::Rom& rom)
        : memory_(rom.memory()), limit_(static_cast<std::uint32_t>(std::min<std::size_t>(rom.data.size(), kPowerOnEnd))) {}

    std::uint32_t limit() const { return limit_; }
    std::optional<z80::Insn> decode(std::uint32_t pc) const override { return z80::decode(memory_, pc, pc); }
    std::uint32_t size(const z80::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const z80::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc < limit_; }
    bool fits(std::uint32_t pc, const z80::Insn& insn) const override { return pc + insn.size <= limit_; }
    bool follows(const z80::Insn& insn) const override { return insn.target && *insn.target < limit_ && insn.flow != z80::Flow::None; }
    bool stops(const z80::Insn& insn) const override {   // (HALT resumes after an interrupt)
        return insn.flow == z80::Flow::Jump || insn.flow == z80::Flow::Return || insn.flow == z80::Flow::Indirect;
    }

private:
    Bytes memory_;
    std::uint32_t limit_;
};

bool port_access(const z80::Insn& insn) { return insn.pattern == "OUT (n),A" || insn.pattern == "IN A,(n)"; }

// One 16 KB page of the ROM written as lines.
class SmsListing final : public iw::Listing<z80::Insn> {
public:
    SmsListing(const formats::sms::Rom& rom, std::uint32_t base, std::uint32_t first, std::uint32_t end, bool mapped,
               const std::map<std::uint32_t, z80::Insn>& code, const decompile::AddressLabels& names, const iw::Names& port_names)
        : rom_(rom), base_(base), first_(first), end_(end), mapped_(mapped), code_(code), names_(names), port_names_(port_names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return base_ + offset - first_; }
    const z80::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {   // (flow was followed in the mapped pages only)
        if (!mapped_) return nullptr;
        const auto it = code_.find(address);
        return it != code_.end() && offset + it->second.size <= end_ ? &it->second : nullptr;
    }
    std::uint32_t size(const z80::Insn& insn) const override { return insn.size; }
    std::string text(const z80::Insn& insn) const override { return z80_source::code(insn, names_, port_names_); }   // (the description's text, the names put in)
    std::vector<std::string> data_lines(ByteView data) const override { return z80_source::data_lines(data); }

private:
    const formats::sms::Rom& rom_;
    std::uint32_t base_, first_, end_;
    bool mapped_;
    const std::map<std::uint32_t, z80::Insn>& code_;
    const decompile::AddressLabels& names_;
    const iw::Names& port_names_;
};

}  // namespace

const std::map<std::uint32_t, std::string>& ports() {
    // config/knowledge/sms/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("sms", "ports");
    return table;
}

const std::map<std::uint32_t, std::string>& game_gear_ports() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("sms", "game_gear_ports");
    return table;
}

const std::map<std::uint32_t, std::string>& mapper_registers() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("sms", "mapper");
    return table;
}

std::uint32_t slot_of(std::size_t page) { return page == 0 ? 0 : page == 1 ? 1 : 2; }

iw::Written write(const formats::sms::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::filesystem::path file(file_name);
    const std::string stem = file.stem().string();
    const SmsFlow flow(rom);
    const std::uint32_t limit = flow.limit();
    const std::vector<std::uint32_t> entries = rom.functions(isa::z80_decoder());
    const iw::Walked<z80::Insn> walked = iw::walk<z80::Insn>(flow, entries);
    const std::map<std::uint32_t, z80::Insn>& code = walked.code;
    const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());

    iw::Names names = {{formats::sms::kResetVector, "reset"}};
    if (starts.contains(formats::sms::kIrqVector)) names.try_emplace(formats::sms::kIrqVector, "irq");
    if (starts.contains(formats::sms::kNmiVector)) names.try_emplace(formats::sms::kNmiVector, "nmi");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a < limit && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    // pointers into the program: a word loaded into a pointer register (HL DE IX IY, not BC, which mostly holds counts), or read and
    // written through (nn), that is the start of a line past the vectors
    for (const auto& entry : code) {
        const z80::Insn& insn = entry.second;
        if (insn.value_count == 0 || insn.values[insn.value_count - 1] < 0) continue;
        const std::uint32_t v = static_cast<std::uint32_t>(insn.values[insn.value_count - 1]);
        if (!line_start(v) || v < 0x0100) continue;
        const bool pointer = insn.pattern == "LD HL,nn" || insn.pattern == "LD DE,nn" || insn.pattern == "LD IX,nn" || insn.pattern == "LD IY,nn" ||
                             insn.pattern == "LD SP,nn";
        if (pointer || (insn.flow == z80::Flow::None && isa::pattern_has(insn.pattern, "(nn)"))) {
            labels.try_emplace(v, code.contains(v) ? (names.contains(v) ? names.at(v) : kLocal(v)) : kData(v));
        }
    }
    // the hardware the code names
    std::map<std::uint32_t, std::string> known_ports = ports();
    if (rom.system == formats::sms::System::GameGear) known_ports.insert(game_gear_ports().begin(), game_gear_ports().end());
    if (rom.system == formats::sms::System::Sg1000) {   // (no memory control, I/O control or V counter; the SC-3000's 8255 is at $DC-$DF)
        for (const std::uint32_t p : {0x3Eu, 0x3Fu, 0x7Eu}) known_ports.erase(p);
        known_ports[0xDE] = "PPI_C";
        known_ports[0xDF] = "PPI_CONTROL";
    }
    std::set<std::uint32_t> used_ports, used_registers;
    for (const auto& entry : code) {
        const z80::Insn& insn = entry.second;
        if (port_access(insn) && insn.value_count > 0 && insn.values[0] >= 0 && known_ports.contains(static_cast<std::uint32_t>(insn.values[0]))) {
            used_ports.insert(static_cast<std::uint32_t>(insn.values[0]));
        }
        if (rom.system != formats::sms::System::Sg1000 && isa::pattern_has(insn.pattern, "(nn)") && insn.value_count > 0 && insn.values[insn.value_count - 1] >= 0 &&
            mapper_registers().contains(static_cast<std::uint32_t>(insn.values[insn.value_count - 1]))) {
            used_registers.insert(static_cast<std::uint32_t>(insn.values[insn.value_count - 1]));
        }
    }
    iw::Names port_names, constants;
    for (const std::uint32_t a : used_ports) port_names[a] = known_ports.at(a);
    for (const std::uint32_t a : used_registers) constants[a] = mapper_registers().at(a);

    const std::size_t pages = rom.page_count();
    translate::l2::Assembly out(
        {std::format("; {}.asm -- written by CodeBreaker from {}", stem, file.filename().string()),
         std::format("; {}, {} KB ROM{}", formats::sms::to_string(rom.system), rom.data.size() / 1024,
                     rom.header ? std::format(", header product {} version {} ({})", rom.header->product, rom.header->version, rom.header->region)
                                : std::string()),
         std::format("; {} pages of 16 KB: page 0 is seen at $0000, page 1 at $4000, every later page at $8000", pages),
         "; (Z80 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu z80 --sleigh`)", ""},
        "sleigh-z80", std::string(file_name));
    for (const auto& [a, name] : port_names) out.append(z80_source::equ(name, a, 2));
    for (const auto& [a, name] : constants) out.append(z80_source::equ(name, a, 4));
    if (!port_names.empty() || !constants.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const auto& [a, name] : constants) names_for_text[a] = name;
    std::set<std::uint32_t> fixed;
    for (const auto& entry : constants) fixed.insert(entry.first);
    iw::Names early;   // (labels the assembler's first pass meets before their lines: defined once here, see z80_source::forward_reference)
    for (const auto& entry : code) {
        if (const auto a = z80_source::forward_reference(entry.second, names_for_text, fixed)) early[*a] = names_for_text.at(*a);
    }
    if (!early.empty()) {
        out.append("; read ahead of their lines (behind a register the assembler's first pass takes them for unknown names)");
        for (const auto& [a, name] : early) out.append(z80_source::equ(name, a, 4));
        out.append("");
    }

    const iw::Names no_labels;
    for (std::size_t page = 0; page < pages; ++page) {
        const std::uint32_t slot = slot_of(page);
        const std::uint32_t base = slot * static_cast<std::uint32_t>(formats::sms::kPage);
        const std::uint32_t first = static_cast<std::uint32_t>(page * formats::sms::kPage);
        const std::uint32_t end = first + static_cast<std::uint32_t>(rom.page(page).size());
        const bool mapped = page <= 2 && first < limit;   // (the pages seen at power-on: where flow was followed)
        out.part(std::format("bank{}", page), "bank", std::nullopt, rom.page(page).size());
        out.append(std::format("; page {} in slot {}", page, slot));
        out.append(std::format("        .org 0x{:04x}", base));
        SmsListing listing(rom, base, first, end, mapped, code, names_for_text, port_names);
        iw::emit<z80::Insn>(out, first, end, mapped ? labels : no_labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return z80_source::assemble(source, folder); }, rom.data, folder / (stem + ".built" + file.extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::sms::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::sms_writer
