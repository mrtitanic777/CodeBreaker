// cb/source/genesis_writer.cpp -- a Genesis / Mega Drive program as assembly source that builds back into it (genesis_writer.hpp).

#include "cb/source/genesis_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/build/m68k_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/m68k.hpp"
#include "cb/source/isa.hpp"

namespace cb::source::genesis_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;

namespace {

constexpr std::uint32_t kFirstCode = 0x200;   // the vectors and the header come before
constexpr std::uint32_t kHeaderFrom = 0x100;
constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
const iw::DataSyntax kSyntax{.bytes = "dc.b", .fill = "dcb.b", .fill_separator = ","};

// A field of the header at $100: where it is, how long, what it holds.
struct HeaderField {
    std::uint32_t at;
    std::uint32_t length;
    std::string_view what;
};
constexpr HeaderField kHeaderFields[] = {{0x100, 16, "system"},     {0x110, 16, "copyright"},         {0x120, 48, "domestic name"},
                                         {0x150, 48, "overseas name"}, {0x180, 14, "serial and version"}, {0x18E, 2, "checksum"},
                                         {0x190, 16, "devices"},    {0x1A0, 8, "ROM start and end"},  {0x1A8, 8, "RAM start and end"},
                                         {0x1B0, 12, "backup RAM"}, {0x1BC, 12, "modem"},             {0x1C8, 40, "notes"},
                                         {0x1F0, 16, "regions"}};

const HeaderField* header_field(std::uint32_t offset) {
    for (const HeaderField& f : kHeaderFields) {
        if (f.at == offset) return &f;
    }
    return nullptr;
}

// Flow runs through the ROM from $200 on, at even addresses.
class GenesisFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit GenesisFlow(ByteView rom) : rom_(rom) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(rom_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= kFirstCode && pc < rom_.size(); }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override { return pc + insn.length <= rom_.size(); }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target >= kFirstCode && *insn.target < rom_.size() && *insn.target % 2 == 0 &&
               insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }

private:
    ByteView rom_;
};

class GenesisListing final : public iw::Listing<m68k::Insn> {
public:
    GenesisListing(translate::l2::Assembly& out, ByteView rom, const std::map<std::uint32_t, m68k::Insn>& code, const decompile::AddressLabels& names)
        : out_(out), rom_(rom), code_(code), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const m68k::Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return m68k::text(insn, names_); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }
    // The header's fields, each as data with its name; a text field as its text.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        const HeaderField* field = header_field(offset);
        if (!field) return 0;
        flush();
        const ByteView bytes = rom_.subspan(offset, field->length);
        const bool plain = std::ranges::all_of(bytes, [](std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22; });
        const std::vector<std::string> lines = plain ? std::vector<std::string>{"        dc.b \"" + std::string(as_text(bytes)) + "\""}
                                                     : iw::data_lines(bytes, kSyntax);
        out_.extend(iw::commented(lines, field->what));
        return field->length;
    }

private:
    translate::l2::Assembly& out_;
    ByteView rom_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const decompile::AddressLabels& names_;
};

// The addresses an instruction's operands name (absolute, PC-relative, long immediates), and how each is named.
struct Reference {
    std::uint32_t value;
    bool immediate;
};
std::vector<Reference> references(const m68k::Insn& insn) {
    std::vector<Reference> out;
    for (const m68k::Op& o : insn.operands()) {
        switch (o.kind) {
            case m68k::OpKind::AbsW:
            case m68k::OpKind::AbsL:
            case m68k::OpKind::PcDisp:
            case m68k::OpKind::PcIndex: out.push_back({static_cast<std::uint32_t>(o.value & 0xFFFFFFFF), false}); break;
            case m68k::OpKind::Imm:
                if (o.size == m68k::Size::Long && o.value >= 0 && o.value <= 0xFFFFFFFF) out.push_back({static_cast<std::uint32_t>(o.value), true});
                break;
            default: break;
        }
    }
    return out;
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xC00000, "VDP_DATA"},   {0xC00004, "VDP_CTRL"},   {0xC00008, "VDP_HVCOUNTER"}, {0xC00011, "PSG"},        {0xA00000, "Z80_RAM"},
        {0xA04000, "YM2612"},     {0xA10001, "IO_VERSION"}, {0xA10003, "IO_DATA1"},      {0xA10005, "IO_DATA2"},   {0xA10007, "IO_DATA3"},
        {0xA10009, "IO_CTRL1"},   {0xA1000B, "IO_CTRL2"},   {0xA1000D, "IO_CTRL3"},      {0xA11100, "Z80_BUSREQ"}, {0xA11200, "Z80_RESET"},
        {0xA14000, "TMSS"},       {0xA130F1, "SRAM_CTRL"}};
    return table;
}

iw::Written write(const formats::genesis::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView data = rom.data;
    const std::uint32_t limit = static_cast<std::uint32_t>(data.size());
    const GenesisFlow flow(data);
    const std::vector<std::uint32_t> entries = rom.functions(isa::m68k_decoder());
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;
    const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());

    // a handler is named for its vector when one vector names it (or it is the reset, vblank or hblank handler); one many vectors
    // share is the program's catch-all
    std::vector<std::pair<std::string, std::uint32_t>> vector_list = {{"ssp", rom.initial_ssp}, {"reset", rom.reset}};
    for (const formats::genesis::Vector& v : rom.vectors) vector_list.emplace_back(v.name, v.address);
    std::map<std::uint32_t, std::vector<std::string>> sharing;
    for (const auto& [name, at] : vector_list) sharing[at].push_back(name);
    iw::Names names;
    for (const auto& [at, vecs] : sharing) {
        if (!starts.contains(at) || (vecs.size() == 1 && vecs[0] == "ssp")) continue;
        std::string main_name;
        for (const std::string& v : vecs) {
            if (v == "reset" || v == "vblank" || v == "hblank" || v == "external") {
                main_name = v;
                break;
            }
        }
        names[at] = vecs.size() > 2 ? "exception" : !main_name.empty() ? main_name : vecs[0];
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a >= kFirstCode && a < limit && !inside_insn.contains(a) && a % 2 == 0; };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const Reference& ref : references(entry.second)) {
            if (hardware().contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value) || ref.value >= 0x400)) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kLocal : kData)(ref.value));
            }
        }
    }

    const formats::genesis::Header& h = rom.header;
    std::vector<std::string> head = {
        std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
        std::format("; \"{}\" / \"{}\", {}, {} KB ROM{}", h.domestic_name, h.overseas_name, h.serial, data.size() / 1024,
                    rom.smd ? ", from an .smd dump" : ""),
        ""};
    translate::l2::Assembly out(head, "asm68k", std::string(file_name));
    for (const std::uint32_t a : used) out.append(std::format("{:<16} equ ${:06X}", hardware().at(a), a));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    // the vector table: 64 longs
    out.part("vectors", "vector table", std::nullopt, 0x100);
    out.append("        org $0");
    for (unsigned i = 0; i < 64; ++i) {
        const std::uint32_t v = static_cast<std::uint32_t>(data[i * 4]) << 24 | static_cast<std::uint32_t>(data[i * 4 + 1]) << 16 |
                                static_cast<std::uint32_t>(data[i * 4 + 2]) << 8 | data[i * 4 + 3];
        std::string target = std::format("${:08X}", v);
        if (i != 0) {
            if (const auto named = labels.find(v); named != labels.end()) target = named->second;
        }
        const std::string what = i == 0 ? "initial stack pointer" : i == 1 ? "reset" : formats::genesis::vector_name(i);
        out.append("        dc.l " + target + (what.empty() ? std::string() : "  ; " + what));
    }
    GenesisListing listing(out, data, code, names_for_text);
    iw::emit<m68k::Insn>(out, kHeaderFrom, limit, labels, functions, listing);

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), data.size());
    const build::m68k_asm::Assembler assembler(isa::m68k_encoder());
    iw::assembled(r, [&] { return assembler.build(source); }, data, folder / (stem + ".built.md"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::genesis::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::genesis_writer
