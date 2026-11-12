// cb/source/gb_writer.cpp -- a Game Boy program as assembly source that builds back into it (gb_writer.hpp).

#include "cb/source/gb_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/build/sm83_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/sm83.hpp"
#include "cb/source/isa.hpp"

namespace cb::source::gb_writer {

namespace sm83 = decompile::sm83;
namespace iw = image_writer;

namespace {

constexpr std::uint32_t kHeaderFrom = 0x104;     // the cartridge header is data
constexpr std::uint32_t kHeaderTo = 0x150;
constexpr std::uint32_t kPowerOnEnd = 0x8000;    // the two banks mapped at power-on
constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};
const iw::DataSyntax kSyntax{.bytes = "db", .fill = "ds", .fill_separator = ", "};

struct HeaderField {
    std::uint32_t at;
    std::uint32_t size;
    std::string_view what;
};
constexpr HeaderField kHeaderFields[] = {{0x0104, 0x30, "Nintendo's logo (the boot ROM checks it)"},
                                         {0x0134, 0x0F, "title"},
                                         {0x0143, 1, "Color support"},
                                         {0x0144, 2, "licensee"},
                                         {0x0146, 1, "Super Game Boy support"},
                                         {0x0147, 1, "cartridge type"},
                                         {0x0148, 1, "ROM size"},
                                         {0x0149, 1, "RAM size"},
                                         {0x014A, 1, "destination"},
                                         {0x014B, 1, "old licensee"},
                                         {0x014C, 1, "version"},
                                         {0x014D, 1, "header checksum"},
                                         {0x014E, 2, "global checksum"}};

const HeaderField* header_field(std::uint32_t address) {
    for (const HeaderField& f : kHeaderFields) {
        if (f.at == address) return &f;
    }
    return nullptr;
}

// Flow runs through the two banks mapped at power-on, around the header.
class GbFlow final : public iw::CodeModel<sm83::Insn> {
public:
    explicit GbFlow(const formats::gb::Rom& rom)
        : memory_(rom.memory()), limit_(static_cast<std::uint32_t>(std::min<std::size_t>(rom.data.size(), kPowerOnEnd))) {}

    std::uint32_t limit() const { return limit_; }
    std::optional<sm83::Insn> decode(std::uint32_t pc) const override { return sm83::decode(memory_, pc, pc); }
    std::uint32_t size(const sm83::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const sm83::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc < limit_ && !(pc >= kHeaderFrom && pc < kHeaderTo); }
    bool fits(std::uint32_t pc, const sm83::Insn& insn) const override { return pc + insn.size <= limit_ && !(pc < kHeaderFrom && kHeaderFrom < pc + insn.size); }
    bool follows(const sm83::Insn& insn) const override { return insn.target && *insn.target < limit_ && insn.flow != sm83::Flow::None; }
    bool stops(const sm83::Insn& insn) const override {
        return insn.flow == sm83::Flow::Jump || insn.flow == sm83::Flow::Return || insn.flow == sm83::Flow::Indirect || insn.pattern == "STOP";
    }

private:
    Bytes memory_;
    std::uint32_t limit_;
};

// One 16 KB bank of the ROM written as lines.
class GbListing final : public iw::Listing<sm83::Insn> {
public:
    GbListing(translate::l2::Assembly& out, const formats::gb::Rom& rom, std::size_t bank, std::uint32_t first, std::uint32_t end, bool mapped,
              const std::map<std::uint32_t, sm83::Insn>& code, const decompile::AddressLabels& names)
        : out_(out), rom_(rom), bank_(bank), first_(first), end_(end), mapped_(mapped), code_(code), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return (bank_ == 0 ? 0 : static_cast<std::uint32_t>(formats::gb::kBank)) + offset - first_; }
    const sm83::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {   // (flow was followed in the mapped banks only)
        if (!mapped_) return nullptr;
        const auto it = code_.find(address);
        return it != code_.end() && offset + it->second.size <= end_ ? &it->second : nullptr;
    }
    std::uint32_t size(const sm83::Insn& insn) const override { return insn.size; }
    std::string text(const sm83::Insn& insn) const override { return sm83::text(insn, names_); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }
    // The cartridge header, each field with its name.
    std::uint32_t special(std::uint32_t, std::uint32_t address, const iw::Flush& flush) override {
        const HeaderField* field = bank_ == 0 ? header_field(address) : nullptr;
        if (!field) return 0;
        flush();
        out_.extend(iw::commented(iw::data_lines(ByteView(rom_.data).subspan(address, field->size), kSyntax), field->what));
        return field->size;
    }

private:
    translate::l2::Assembly& out_;
    const formats::gb::Rom& rom_;
    std::size_t bank_;
    std::uint32_t first_;
    std::uint32_t end_;
    bool mapped_;
    const std::map<std::uint32_t, sm83::Insn>& code_;
    const decompile::AddressLabels& names_;
};

}  // namespace

const std::map<std::uint32_t, std::string>& registers() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xFF00, "rP1"},   {0xFF01, "rSB"},   {0xFF02, "rSC"},   {0xFF04, "rDIV"},  {0xFF05, "rTIMA"}, {0xFF06, "rTMA"},  {0xFF07, "rTAC"},
        {0xFF0F, "rIF"},   {0xFF10, "rNR10"}, {0xFF11, "rNR11"}, {0xFF12, "rNR12"}, {0xFF13, "rNR13"}, {0xFF14, "rNR14"}, {0xFF16, "rNR21"},
        {0xFF17, "rNR22"}, {0xFF18, "rNR23"}, {0xFF19, "rNR24"}, {0xFF1A, "rNR30"}, {0xFF1B, "rNR31"}, {0xFF1C, "rNR32"}, {0xFF1D, "rNR33"},
        {0xFF1E, "rNR34"}, {0xFF20, "rNR41"}, {0xFF21, "rNR42"}, {0xFF22, "rNR43"}, {0xFF23, "rNR44"}, {0xFF24, "rNR50"}, {0xFF25, "rNR51"},
        {0xFF26, "rNR52"}, {0xFF40, "rLCDC"}, {0xFF41, "rSTAT"}, {0xFF42, "rSCY"},  {0xFF43, "rSCX"},  {0xFF44, "rLY"},   {0xFF45, "rLYC"},
        {0xFF46, "rDMA"},  {0xFF47, "rBGP"},  {0xFF48, "rOBP0"}, {0xFF49, "rOBP1"}, {0xFF4A, "rWY"},   {0xFF4B, "rWX"},   {0xFF4D, "rKEY1"},
        {0xFF4F, "rVBK"},  {0xFF51, "rHDMA1"}, {0xFF52, "rHDMA2"}, {0xFF53, "rHDMA3"}, {0xFF54, "rHDMA4"}, {0xFF55, "rHDMA5"}, {0xFF56, "rRP"},
        {0xFF68, "rBCPS"}, {0xFF69, "rBCPD"}, {0xFF6A, "rOCPS"}, {0xFF6B, "rOCPD"}, {0xFF70, "rSVBK"}, {0xFFFF, "rIE"}};
    return table;
}

const std::map<std::uint32_t, std::string>& mbc_registers() {
    static const std::map<std::uint32_t, std::string> table = {{0x0000, "rRAMG"}, {0x2000, "rROMB0"}, {0x3000, "rROMB1"}, {0x4000, "rRAMB"}};
    return table;
}

iw::Written write(const formats::gb::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::filesystem::path file(file_name);
    const std::string stem = file.stem().string();
    const GbFlow flow(rom);
    const std::uint32_t limit = flow.limit();
    const std::vector<std::uint32_t> entries = rom.functions(isa::sm83_decoder());
    const iw::Walked<sm83::Insn> walked = iw::walk<sm83::Insn>(flow, entries);
    const std::map<std::uint32_t, sm83::Insn>& code = walked.code;
    const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());

    iw::Names names;   // a function is named for the vector it is at
    if (starts.contains(formats::gb::kEntry)) names[formats::gb::kEntry] = "entry";
    for (const formats::gb::InterruptVector& v : formats::gb::kInterruptVectors) {
        if (starts.contains(v.address)) names[v.address] = std::string(v.name);
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a < limit && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    for (const auto& entry : code) {
        const sm83::Insn& insn = entry.second;
        if (!insn.value || *insn.value < 0) continue;
        const std::uint32_t v = static_cast<std::uint32_t>(*insn.value);
        if (!line_start(v) || v < 0x0150) continue;
        if (insn.pattern.starts_with("LD (nn)")) continue;   // a write into ROM space goes to the bank controller, not to data
        const bool pointer = insn.pattern == "LD HL,nn" || insn.pattern == "LD DE,nn" || insn.pattern == "LD SP,nn";
        if (pointer || (insn.flow == sm83::Flow::None && isa::pattern_has(insn.pattern, "(nn)"))) {
            labels.try_emplace(v, code.contains(v) ? (names.contains(v) ? names.at(v) : kLocal(v)) : kData(v));
        }
    }
    std::map<std::uint32_t, std::string> known = registers();
    known.insert(mbc_registers().begin(), mbc_registers().end());
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        const sm83::Insn& insn = entry.second;
        if (!insn.value || *insn.value < 0) continue;
        const std::uint32_t v = static_cast<std::uint32_t>(*insn.value);
        if (!known.contains(v)) continue;
        if (insn.pattern.starts_with("LDH") || isa::pattern_has(insn.pattern, "(nn)")) used.insert(v);
    }

    const std::string base_name = file.filename().string();
    translate::l2::Assembly out(
        {std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
         std::format("; \"{}\", {}, {} KB ROM{}{}", rom.title, rom.mbc(), rom.data.size() / 1024,
                     rom.color == formats::gb::ColorSupport::None ? std::string()
                                                                  : std::format(", Game Boy Color ({})", rom.color == formats::gb::ColorSupport::Also ? "also" : "only"),
                     rom.sgb ? ", Super Game Boy" : ""),
         ""},
        "rgbds", std::string(file_name));
    for (const std::uint32_t a : used) out.append(std::format("DEF {:<8} EQU ${:04X}", known.at(a), a));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = known.at(a);

    const iw::Names no_labels;
    for (std::size_t bank = 0; bank < rom.bank_count(); ++bank) {
        const std::uint32_t first = static_cast<std::uint32_t>(bank * formats::gb::kBank);
        const std::uint32_t end = first + static_cast<std::uint32_t>(rom.bank(bank).size());
        const bool mapped = bank <= 1;   // (the banks seen at power-on: where flow was followed)
        out.part(std::format("bank{}", bank), "bank", std::nullopt, rom.bank(bank).size());
        out.append(std::format("SECTION \"bank{}\", {}", bank, bank == 0 ? std::string("ROM0[$0000]") : std::format("ROMX[$4000], BANK[{}]", bank)));
        GbListing listing(out, rom, bank, first, end, mapped, code, names_for_text);
        iw::emit<sm83::Insn>(out, first, end, mapped ? labels : no_labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return build::sm83_asm::build(source, rom.data.size(), folder); }, rom.data, folder / (stem + ".built" + file.extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::gb::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::gb_writer
