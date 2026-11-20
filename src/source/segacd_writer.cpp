// source/segacd_writer.cpp -- a Sega CD / Mega-CD disc's boot sectors as assembly source that builds back into them (segacd_writer.hpp).

#include "source/segacd_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/m68k_asm.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m68k.hpp"
#include "source/genesis_writer.hpp"
#include "source/isa.hpp"

namespace cb::source::segacd_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;
namespace sc = formats::segacd;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
const iw::DataSyntax kSyntax{.bytes = "dc.b", .fill = "dcb.b", .fill_separator = ","};
constexpr std::uint32_t kPaddingAt = 0x100000;   // where the assembler is told to put the bytes between the programs (not an address either CPU uses)
constexpr std::uint32_t kPaddingStep = 0x10000;

// A field of the boot header.
enum class Kind { Text, Word, Long };
struct HeaderField {
    std::uint32_t at;
    std::uint32_t length;
    Kind kind;
    std::string_view what;
};
constexpr HeaderField kHeaderFields[] = {
    {0x00, 16, Kind::Text, "disc ID"},        {0x10, 11, Kind::Text, "volume name"},       {0x1C, 2, Kind::Word, "volume version"},
    {0x1E, 2, Kind::Word, "volume type"},     {0x20, 11, Kind::Text, "system name"},       {0x30, 4, Kind::Long, "IP: start on the disc"},
    {0x34, 4, Kind::Long, "IP: size"},        {0x38, 4, Kind::Long, "IP: entry offset"},   {0x3C, 4, Kind::Long, "IP: work RAM size"},
    {0x40, 4, Kind::Long, "SP: start on the disc"}, {0x44, 4, Kind::Long, "SP: size"},     {0x48, 4, Kind::Long, "SP: entry offset"},
    {0x4C, 4, Kind::Long, "SP: work RAM size"}, {0x100, 16, Kind::Text, "system type"},     {0x110, 16, Kind::Text, "copyright"},
    {0x120, 48, Kind::Text, "domestic name"}, {0x150, 48, Kind::Text, "overseas name"},     {0x180, 14, Kind::Text, "serial and version"},
    {0x190, 16, Kind::Text, "devices"},       {0x1F0, 16, Kind::Text, "regions"}};

const HeaderField* header_field(std::uint32_t offset) {
    for (const HeaderField& f : kHeaderFields) {
        if (f.at == offset) return &f;
    }
    return nullptr;
}

// Where a stretch of the boot area is written: its offsets [begin, end) at `base` on the CPU that runs it (or at an address the assembler is told to use).
struct Block {
    std::uint32_t begin, end, base;
    std::string_view title;
};

// Flow runs through one program, at even addresses; its bytes are boot[begin, end) at `base`.
class ProgramFlow final : public iw::CodeModel<m68k::Insn> {
public:
    ProgramFlow(ByteView bytes, std::uint32_t base) : bytes_(bytes), base_(base) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(bytes_, pc - base_, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= base_ && pc - base_ < bytes_.size(); }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override { return pc - base_ + insn.length <= bytes_.size(); }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && inside(*insn.target) && *insn.target % 2 == 0 && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }

private:
    ByteView bytes_;
    std::uint32_t base_;
};

class DiscListing final : public iw::Listing<m68k::Insn> {
public:
    DiscListing(translate::l2::Assembly& out, ByteView boot, std::vector<Block> blocks, const std::map<std::uint32_t, m68k::Insn>& code,
                const decompile::AddressLabels& names)
        : out_(out), boot_(boot), blocks_(std::move(blocks)), code_(code), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return boot_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override {
        for (const Block& b : blocks_) {
            if (offset >= b.begin && offset < b.end) return b.base + (offset - b.begin);
        }
        return offset;
    }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return m68k::text(insn, names_); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }
    // The boot header's fields, each as data with its name.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset >= sc::kSecurityAt) return 0;
        const HeaderField* field = header_field(offset);
        if (!field) return 0;
        flush();
        const ByteView bytes = boot_.subspan(offset, field->length);
        std::string line;
        const bool plain = std::ranges::all_of(bytes, [](std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22; });
        if (field->kind == Kind::Text && plain) {
            line = "        dc.b \"" + std::string(as_text(bytes)) + "\"";
        } else if (field->kind == Kind::Word || field->kind == Kind::Long) {
            std::uint32_t v = 0;
            for (const std::uint8_t b : bytes) v = v << 8 | b;
            line = std::format("        {} ${:0{}X}", field->kind == Kind::Word ? "dc.w" : "dc.l", v, field->length * 2);
        } else {
            out_.extend(iw::commented(iw::data_lines(bytes, kSyntax), field->what));
            return field->length;
        }
        out_.append(iw::commented({line}, field->what).front());
        return field->length;
    }

private:
    translate::l2::Assembly& out_;
    ByteView boot_;
    std::vector<Block> blocks_;
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
            case m68k::OpKind::PcIndex: out.push_back({static_cast<std::uint32_t>(o.value & 0xFFFFFF), false}); break;   // (24 bits: the CPUs' address lines)
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
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t = genesis_writer::hardware();
        const std::map<std::uint32_t, std::string> more = {
            // the gate array, as the main CPU sees it
            {0xA12000, "GA_RESET"},      {0xA12002, "GA_MEMMODE"},     {0xA12003, "GA_MEMMODE_LO"},   {0xA12004, "GA_CDCMODE"},
            {0xA12006, "GA_HINTVEC"},    {0xA12008, "GA_CDCHOST"},     {0xA1200C, "GA_STOPWATCH"},    {0xA1200E, "GA_COMFLAG_MAIN"},
            {0xA1200F, "GA_COMFLAG_SUB"}, {0xA12010, "GA_COMCMD0"},    {0xA12020, "GA_COMSTAT0"},
            // ... and as the sub CPU sees it
            {0xFF8000, "SGA_LED"},       {0xFF8002, "SGA_MEMMODE"},    {0xFF8004, "SGA_CDCMODE"},     {0xFF800E, "SGA_COMFLAGS"},
            {0xFF800F, "SGA_COMFLAG_SUB"}, {0xFF8010, "SGA_COMCMD0"},  {0xFF8020, "SGA_COMSTAT0"},
            // the CD BIOS's calls (the sub CPU's jump table)
            {0x5F0A, "SETJMPTBL"},       {0x5F10, "WAITVSYNC"},        {0x5F16, "ADRERR"},            {0x5F1C, "CODERR"},
            {0x5F22, "CDBIOS"},          {0x5F28, "USERCALL0"},        {0x5F2E, "USERCALL1"},         {0x5F34, "USERCALL2"},
            {0x5F3A, "USERCALL3"}};
        t.insert(more.begin(), more.end());
        return t;
    }();
    return table;
}

iw::Written write(const formats::segacd::Disc& disc, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView boot = disc.boot;
    const std::uint32_t boot_size = static_cast<std::uint32_t>(boot.size());

    // The blocks of the system area, in disc order, each at the address its CPU runs it at.
    std::vector<Block> blocks = {{0, sc::kSecurityAt, 0, "the boot header"}};
    blocks.push_back({disc.main_begin, disc.main_end, sc::kMainBase, "the main CPU's program: the security code and the IP, loaded at $FF0000"});
    std::uint32_t padding = kPaddingAt;
    const auto pad = [&](std::uint32_t begin, std::uint32_t end) {
        if (end > begin) {
            blocks.push_back({begin, end, padding, "bytes of the system area no program runs"});
            padding += kPaddingStep;
        }
    };
    if (disc.has_sub()) {
        pad(disc.main_end, disc.sub_begin);
        blocks.push_back({disc.sub_begin, disc.sub_end, sc::kSubBase, "the sub CPU's program (the SP), loaded at $6000 of PRG-RAM"});
        pad(disc.sub_end, boot_size);
    } else {
        pad(disc.main_end, boot_size);
    }

    const ProgramFlow main_flow(boot.subspan(disc.main_begin, disc.main_end - disc.main_begin), sc::kMainBase);
    const ProgramFlow sub_flow(disc.has_sub() ? boot.subspan(disc.sub_begin, disc.sub_end - disc.sub_begin) : ByteView(), sc::kSubBase);
    const std::vector<std::uint32_t> entries = disc.functions(isa::m68k_decoder());
    std::vector<std::uint32_t> main_entries, sub_entries;
    for (const std::uint32_t a : entries) (sc::Disc::is_sub(a) ? sub_entries : main_entries).push_back(a);
    const iw::Walked<m68k::Insn> main_walk = iw::walk<m68k::Insn>(main_flow, main_entries);
    const iw::Walked<m68k::Insn> sub_walk = iw::walk<m68k::Insn>(sub_flow, sub_entries);
    std::map<std::uint32_t, m68k::Insn> code = main_walk.code;
    code.insert(sub_walk.code.begin(), sub_walk.code.end());
    std::set<std::uint32_t> targets = main_walk.targets;
    targets.insert(sub_walk.targets.begin(), sub_walk.targets.end());
    std::vector<std::uint32_t> starts = main_walk.starts;
    starts.insert(starts.end(), sub_walk.starts.begin(), sub_walk.starts.end());
    std::ranges::sort(starts);

    iw::Names names;
    names[sc::kMainBase] = "security";
    if (const std::uint32_t ip = disc.header.ip.start + disc.header.ip.entry; ip >= disc.main_begin && ip < disc.main_end && ip != disc.main_begin)
        names.try_emplace(disc.main_address(ip), "ip_start");
    const std::array<std::uint32_t, 4> table = disc.sub_entries();
    for (std::size_t i = 0; i < table.size(); ++i) {
        if (table[i] != 0) names.try_emplace(table[i], std::string(sc::kSubEntryNames[i]));
    }
    const iw::Functions functions = iw::name_functions(starts, names, kSub);
    const auto in_program = [&](std::uint32_t a) { return main_flow.inside(a) || sub_flow.inside(a); };
    std::set<std::uint32_t> inside_insn = iw::covered(code, main_flow);
    for (const std::uint32_t a : iw::covered(code, sub_flow)) inside_insn.insert(a);
    const auto line_start = [&](std::uint32_t a) { return in_program(a) && !inside_insn.contains(a) && a % 2 == 0; };
    iw::Names labels = iw::make_labels(targets, names, line_start, kLocal);
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

    const formats::segacd::Header& h = disc.header;
    std::vector<std::string> head = {
        std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
        std::format("; Sega CD \"{}\" / \"{}\", {}: the system area ({} KB) of a {}-byte-sector image", h.domestic_name, h.overseas_name, h.serial, boot.size() / 1024,
                    disc.sector_size),
        ""};
    translate::l2::Assembly out(head, "asm68k", std::string(file_name));
    for (const std::uint32_t a : used) out.append(std::format("{:<18} equ ${:06X}", hardware().at(a), a));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    DiscListing listing(out, boot, blocks, code, names_for_text);
    for (const Block& b : blocks) {
        out.part(std::format("block_{:04X}", b.begin), std::string(b.title), std::nullopt, std::uint64_t{b.end - b.begin});
        out.append(std::format("        org ${:X}   ; {}", b.base, b.title));
        iw::emit<m68k::Insn>(out, b.begin, b.end, labels, functions, listing);
        out.append("");
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, main_flow), boot.size());
    const build::m68k_asm::Assembler assembler(isa::m68k_encoder());
    iw::assembled(
        r,
        [&] {   // each block back at its disc offset
            const Bytes image = assembler.build(source);
            Bytes joined(boot.size(), 0);
            for (const Block& b : blocks) {
                for (std::uint32_t i = b.begin; i < b.end; ++i) {
                    const std::size_t at = std::size_t{b.base} + (i - b.begin);
                    if (at < image.size()) joined[i] = image[at];
                }
            }
            return joined;
        },
        boot, folder / (stem + ".built.iso"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder) {
    return write(formats::segacd::read(image_file), image_file.filename().string(), folder);
}

}  // namespace cb::source::segacd_writer
