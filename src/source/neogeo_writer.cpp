// source/neogeo_writer.cpp -- a Neo Geo P-ROM as assembly source that builds back into it (neogeo_writer.hpp).

#include "source/neogeo_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m68k.hpp"
#include "source/isa.hpp"
#include "source/risc_source.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::neogeo_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;
namespace ng = formats::neogeo;

namespace {

constexpr std::uint32_t kHeaderFrom = 0x100;
constexpr std::uint32_t kWindow = static_cast<std::uint32_t>(ng::kRomWindow);
constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

// A field of the header at $100: where it is, how long, what it holds.
struct HeaderField {
    std::uint32_t at;
    std::uint32_t length;
    std::string_view what;
};
constexpr HeaderField kHeaderFields[] = {{0x100, 7, "magic"},          {0x107, 1, "system version"},  {0x108, 2, "NGH number"},
                                         {0x10A, 4, "program size"},   {0x10E, 4, "backup RAM"},      {0x112, 2, "save data size"},
                                         {0x114, 1, "eye-catcher"},    {0x115, 1, "eye-catcher sprite bank"},
                                         {0x116, 4, "Japan settings"}, {0x11A, 4, "US settings"},     {0x11E, 4, "Europe settings"},
                                         {0x182, 4, "security code"}};

const HeaderField* header_field(std::uint32_t offset) {
    for (const HeaderField& f : kHeaderFields) {
        if (f.at == offset) return &f;
    }
    return nullptr;
}

// Flow runs through the first megabyte of the P-ROM from the header's jump table on, at even addresses.
class NeoGeoFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit NeoGeoFlow(ByteView rom) : rom_(rom) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(rom_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= ng::kFirstCode && pc < rom_.size(); }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override { return pc + insn.length <= rom_.size(); }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target >= ng::kFirstCode && *insn.target < rom_.size() && *insn.target % 2 == 0 && insn.flow != m68k::Flow::None &&
               insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }

private:
    ByteView rom_;   // the first megabyte
};

class NeoGeoListing final : public iw::Listing<m68k::Insn> {
public:
    NeoGeoListing(translate::l2::Assembly& out, ByteView rom, const std::map<std::uint32_t, m68k::Insn>& code, const decompile::AddressLabels& names)
        : out_(out), rom_(rom), code_(code), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return ng::Rom::address_of(offset); }
    const m68k::Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return risc_source::m68k_code(insn, names_); }   // (the description's text, the names put in)
    std::vector<std::string> data_lines(ByteView data) const override { return risc_source::data_lines(data); }
    // The header's fields, each as data with its name: a text field as its text, a number as a word or a long.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        const HeaderField* field = header_field(offset);
        if (!field || offset + field->length > rom_.size()) return 0;
        flush();
        const ByteView bytes = rom_.subspan(offset, field->length);
        std::string line;
        if (offset == 0x100) {
            line = "        .ascii \"" + std::string(as_text(bytes)) + "\"";
        } else if (field->length == 2 || field->length == 4) {
            std::uint32_t v = 0;
            for (const std::uint8_t b : bytes) v = v << 8 | b;
            line = std::format("        {} 0x{:0{}x}", field->length == 2 ? ".half" : ".long", v, field->length * 2);
        } else {
            line = std::format("        .byte 0x{:02x}", bytes[0]);
        }
        out_.append(iw::commented({line}, field->what).front());
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
        // the program's windows
        {0x2FFFF0, "BANKSEL"},
        // controllers, DIP switches, the system ports
        {0x300000, "REG_P1CNT"},      {0x300001, "REG_DIPSW_WATCHDOG"}, {0x300081, "REG_SYSTYPE"},   {0x320000, "REG_SOUND"},
        {0x320001, "REG_STATUS_A"},   {0x340000, "REG_P2CNT"},          {0x380000, "REG_STATUS_B"},   {0x380001, "REG_POUTPUT"},
        {0x380011, "REG_CRDBANK"},    {0x380021, "REG_SLOT"},           {0x380031, "REG_LEDLATCHES"}, {0x380041, "REG_LEDDATA"},
        {0x380051, "REG_RTCCTRL"},    {0x380061, "REG_RESETCC1"},       {0x380063, "REG_RESETCL1"},   {0x380065, "REG_RESETCC2"},
        {0x380067, "REG_RESETCL2"},
        // the system latches (a write to the address sets the switch)
        {0x3A0001, "REG_NOSHADOW"},   {0x3A0011, "REG_SHADOW"},         {0x3A0003, "REG_SWPBIOS"},    {0x3A0013, "REG_SWPROM"},
        {0x3A000B, "REG_BRDFIX"},     {0x3A001B, "REG_CRTFIX"},         {0x3A000D, "REG_SRAMLOCK"},   {0x3A001D, "REG_SRAMUNLOCK"},
        {0x3A000F, "REG_PALBANK1"},   {0x3A001F, "REG_PALBANK0"},
        // the LSPC (video)
        {0x3C0000, "REG_VRAMADDR"},   {0x3C0002, "REG_VRAMRW"},        {0x3C0004, "REG_VRAMMOD"},     {0x3C0006, "REG_LSPCMODE"},
        {0x3C0008, "REG_TIMERHIGH"},  {0x3C000A, "REG_TIMERLOW"},      {0x3C000C, "REG_IRQACK"},      {0x3C000E, "REG_TIMERSTOP"},
        // the BIOS's calls
        {0xC00438, "SYSTEM_INT1"},    {0xC0043E, "SYSTEM_INT2"},        {0xC00444, "SYSTEM_RETURN"},   {0xC0044A, "SYSTEM_IO"},
        {0xC00450, "CREDIT_CHECK"},   {0xC00456, "CREDIT_DOWN"},        {0xC0045C, "READ_CALENDAR"},   {0xC00462, "SETUP_CALENDAR"},
        {0xC00468, "CARD"},           {0xC0046E, "CARD_ERROR"},         {0xC00474, "HOW_TO_PLAY"},     {0xC0047A, "CHECKSUM"},
        {0xC004C2, "FIX_CLEAR"},      {0xC004C8, "LSP_1ST"},            {0xC004CE, "MESS_OUT"},
        // the BIOS's variables in work RAM
        {0x10FD80, "BIOS_SYSTEM_MODE"}, {0x10FD82, "BIOS_MVS_FLAG"},    {0x10FD83, "BIOS_COUNTRY_CODE"}, {0x10FDAE, "BIOS_USER_REQUEST"},
        {0x10FDAF, "BIOS_USER_MODE"}};
    return table;
}

iw::Written write(const formats::neogeo::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView data = rom.data;
    const std::uint32_t first = static_cast<std::uint32_t>(std::min(data.size(), ng::kRomWindow));   // the bytes the flow runs through
    const std::uint32_t second = static_cast<std::uint32_t>(std::min(data.size(), ng::kRomWindow * 2));   // ... and those written as the bank
    const ByteView code_space = data.first(first);
    const NeoGeoFlow flow(code_space);
    const std::vector<std::uint32_t> function_starts = rom.functions(isa::m68k_decoder());
    std::vector<std::uint32_t> entries = function_starts;   // flow also starts from the header's JMPs, which are not functions of their own
    for (const std::uint32_t jump : rom.callback_jumps()) entries.push_back(jump);
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;
    const std::set<std::uint32_t> starts(function_starts.begin(), function_starts.end());

    // a handler is named for the callback or the vector that names it; one many vectors share is the program's catch-all
    std::vector<std::pair<std::string, std::uint32_t>> vector_list = {{"ssp", rom.initial_ssp}, {"reset", rom.reset}};
    for (const formats::neogeo::Vector& v : rom.vectors) vector_list.emplace_back(v.name, v.address);
    std::map<std::uint32_t, std::vector<std::string>> sharing;
    for (const auto& [name, at] : vector_list) sharing[at].push_back(name);
    iw::Names names;
    for (std::size_t i = 0; i < rom.header.callbacks.size(); ++i) {   // (where a callback jumps to is the function; two callbacks to one place take the first's name)
        const std::uint32_t target = rom.header.callbacks[i];
        if (target != 0 && starts.contains(target)) names.try_emplace(target, std::string(ng::kCallbackNames[i]));
    }
    if (rom.header.security != 0 && starts.contains(rom.header.security)) {
        names.try_emplace(rom.header.security, "security");
    }
    for (const auto& [at, vecs] : sharing) {
        if (!starts.contains(at) || names.contains(at) || (vecs.size() == 1 && vecs[0] == "ssp")) continue;
        std::string main_name;
        for (const std::string& v : vecs) {
            if (v == "reset" || v == "vblank" || v == "timer") {
                main_name = v;
                break;
            }
        }
        names[at] = vecs.size() > 2 ? "exception" : !main_name.empty() ? main_name : vecs[0];
    }
    const iw::Functions functions = iw::name_functions(function_starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a >= ng::kFirstCode && a < first && !inside_insn.contains(a) && a % 2 == 0; };
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

    std::vector<std::string> head = {
        std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
        std::format("; Neo Geo, NGH {:04X}, {} KB P-ROM ({})", rom.header.ngh, data.size() / 1024, formats::neogeo::layout_text(rom.layout)), ""};
    head.insert(head.end() - 1, "; (68000 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu m68k --sleigh`)");
    translate::l2::Assembly out(head, "sleigh-m68k", std::string(file_name));
    for (const std::uint32_t a : used) out.append(risc_source::equ(hardware().at(a), a, 6));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    // the vector table: 64 longs
    out.part("vectors", "vector table", std::nullopt, 0x100);
    out.append("        .org 0x0");
    for (unsigned i = 0; i < 64; ++i) {
        const std::uint32_t v = static_cast<std::uint32_t>(data[i * 4]) << 24 | static_cast<std::uint32_t>(data[i * 4 + 1]) << 16 |
                                static_cast<std::uint32_t>(data[i * 4 + 2]) << 8 | data[i * 4 + 3];
        std::string target = std::format("0x{:08x}", v);
        if (i != 0) {
            if (const auto named = labels.find(v); named != labels.end()) target = named->second;
        }
        const std::string what = i == 0 ? "initial stack pointer" : i == 1 ? "reset" : formats::neogeo::vector_name(i);
        out.append("        .long " + target + (what.empty() ? std::string() : "  ; " + what));
    }
    NeoGeoListing listing(out, data, code, names_for_text);
    iw::emit<m68k::Insn>(out, kHeaderFrom, first, labels, functions, listing);
    if (second > kWindow) {
        out.append("");
        out.append("        .org 0x200000   ; the bank window: the P-ROM's second megabyte");
        iw::emit<m68k::Insn>(out, kWindow, second, {}, {}, listing);
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), data.size());
    iw::assembled(
        r,
        [&] {   // the first megabyte from address 0, the second from the window: the P-ROM's order
            return risc_source::assemble("m68k", source, folder);   // (the window's bytes follow the first megabyte's)
        },
        data.first(second), folder / (stem + ".built.p1"));
    if (data.size() > second) {
        r.same = false;
        r.notes.push_back(std::format("the P-ROM holds {} bytes: the banks after the second megabyte are not written", data.size()));
    }
    return r;
}

iw::Written write_sound(ByteView sound, std::string_view file_name, const std::filesystem::path& folder) {
    constexpr std::size_t kFixed = 0x8000;   // the Z80's fixed window; the rest of an M-ROM is banked into $8000-$FFFF
    const std::string stem = std::filesystem::path(file_name).stem().string() + ".m1";
    static const z80_writer::Machine machine{
        .ports = {{0x00, "SOUND_COMMAND"}, {0x04, "YM2610_ADDR_A"}, {0x05, "YM2610_DATA_A"}, {0x06, "YM2610_ADDR_B"}, {0x07, "YM2610_DATA_B"}, {0x0C, "SOUND_REPLY"}},
        .names = {}};
    z80_writer::Block block{.image = sound.first(std::min(sound.size(), kFixed)), .origin = 0, .entries = {{0x0000, "reset"}, {0x0038, "irq"}, {0x0066, "nmi"}}, .header = {}};
    block.header.push_back(std::format("Neo Geo sound program (Z80) from {}: {} bytes{}", std::filesystem::path(file_name).filename().string(), sound.size(),
                                       sound.size() > kFixed ? "; the first 32 KB, the rest is banked data" : ""));
    iw::Written r = z80_writer::write(block, machine, folder, stem);
    if (sound.size() > kFixed) {
        r.same = false;
        r.notes.push_back(std::format("the M-ROM holds {} bytes: the banks after the first 32 KB are not written", sound.size()));
    }
    return r;
}

iw::Written reconstruct_sound(const std::filesystem::path& sound_file, const std::filesystem::path& folder) {
    const fs::MappedFile file(sound_file);
    return write_sound(file.view(), sound_file.filename().string(), folder);
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    const formats::neogeo::Rom rom = formats::neogeo::read(rom_file);
    iw::Written r = write(rom, rom_file.filename().string(), folder);
    if (!rom.sound.empty()) {   // a .neo container: its Z80 program is written beside the 68000's
        const iw::Written sound = write_sound(rom.sound, rom_file.filename().string(), folder);
        if (!sound.same) {
            r.same = false;
            r.notes.push_back("the Z80 sound program (M-ROM) did not rebuild" + (sound.notes.empty() ? std::string() : ": " + sound.notes.front()));
        }
    }
    return r;
}

}  // namespace cb::source::neogeo_writer
