// cb-unity: alone
// source/vectrex_writer.cpp -- a Vectrex cartridge as 6809 source that builds back into it (vectrex_writer.hpp).

#include "source/vectrex_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/m6809_asm.hpp"
#include "core/fs.hpp"
#include "decompile/m6809.hpp"

namespace cb::source::vectrex_writer {

namespace dm = decompile::m6809;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
const iw::DataSyntax kSyntax{.bytes = "FCB", .fill = "FILL", .fill_separator = ","};

bool printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"'; });
}

std::string hex8(std::uint8_t v) { return std::format("${:02X}", v); }

class VectrexFlow final : public iw::CodeModel<dm::Insn> {
public:
    explicit VectrexFlow(const formats::vectrex::Rom& rom) : rom_(rom), memory_(rom.memory()) {}

    std::optional<dm::Insn> decode(std::uint32_t pc) const override { return dm::decode(memory_, pc, pc); }
    std::uint32_t size(const dm::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const dm::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= rom_.entry && pc < rom_.data.size(); }
    bool fits(std::uint32_t pc, const dm::Insn& insn) const override { return pc + insn.size <= rom_.data.size(); }
    bool follows(const dm::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == dm::Flow::Branch || insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Call);
    }
    bool stops(const dm::Insn& insn) const override { return insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Return || insn.flow == dm::Flow::Indirect; }
    bool finds_functions() const override { return true; }
    bool starts_function(const dm::Insn& insn) const override { return insn.flow == dm::Flow::Call; }

private:
    const formats::vectrex::Rom& rom_;
    Bytes memory_;
};

class VectrexListing final : public iw::Listing<dm::Insn> {
public:
    VectrexListing(translate::l2::Assembly& out, const formats::vectrex::Rom& rom, const std::map<std::uint32_t, dm::Insn>& code,
                   const std::map<std::uint32_t, std::string>& text)
        : out_(out), rom_(rom), code_(code), text_(text) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const dm::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const dm::Insn& insn) const override { return insn.size; }
    std::string text(const dm::Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }
    std::string label_line(const std::string& name) const override { return name; }

    // The header: copyright text, music pointer, title lines, the closing zero.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != 0) return 0;
        flush();
        const ByteView d = rom_.data;
        const std::size_t copyright = rom_.copyright.size();
        out_.append(printable(d.first(copyright)) ? std::format("        FCC \"{}\"", rom_.copyright) : "        FCB " + hex8(d[0]));
        if (!printable(d.first(copyright))) out_.extend(iw::data_lines(d.subspan(1, copyright - 1), kSyntax));
        out_.append("        FCB $80");
        out_.append(std::format("        FDB ${:04X}  ; music", rom_.music));
        std::size_t p = copyright + 3;
        for (const formats::vectrex::TitleLine& t : rom_.title) {
            out_.append(std::format("        FCB {},{},{},{}  ; height, width, y, x", hex8(d[p]), hex8(d[p + 1]), hex8(d[p + 2]), hex8(d[p + 3])));
            const ByteView text = d.subspan(p + 4, t.text.size());
            if (printable(text)) out_.append(std::format("        FCC \"{}\"", t.text));
            else out_.extend(iw::data_lines(text, kSyntax));
            out_.append("        FCB $80");
            p += 4 + t.text.size() + 1;
        }
        out_.append("        FCB 0");
        return rom_.entry;
    }

private:
    translate::l2::Assembly& out_;
    const formats::vectrex::Rom& rom_;
    const std::map<std::uint32_t, dm::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
};

// The name an operand's address has, when it has one: a BIOS routine or a VIA register.
const std::string* hardware_name(std::uint32_t address) {
    if (const auto it = bios().find(address); it != bios().end()) return &it->second;
    if (const auto it = via().find(address); it != via().end()) return &it->second;
    return nullptr;
}

}  // namespace

const std::map<std::uint32_t, std::string>& bios() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xF000, "Cold_Start"},   {0xF06C, "Warm_Start"},   {0xF14C, "Init_VIA"},     {0xF164, "Init_OS_RAM"},  {0xF192, "Wait_Recal"},   {0xF1A2, "Set_Refresh"},
        {0xF1AA, "DP_to_D0"},     {0xF1AF, "DP_to_C8"},     {0xF1B4, "Read_Btns_Mask"}, {0xF1BA, "Read_Btns"},  {0xF1F5, "Joy_Analog"},   {0xF1F8, "Joy_Digital"},
        {0xF256, "Sound_Byte"},   {0xF272, "Clear_Sound"},  {0xF289, "Do_Sound"},     {0xF29D, "Intensity_1F"}, {0xF2A1, "Intensity_3F"}, {0xF2A5, "Intensity_5F"},
        {0xF2A9, "Intensity_7F"}, {0xF2AB, "Intensity_a"},  {0xF2BE, "Dot_ix_b"},     {0xF2C3, "Dot_ix"},       {0xF2C5, "Dot_d"},       {0xF2D5, "Dot_here"},
        {0xF2DE, "Dot_List"},     {0xF2FC, "Dot_List_Reset"}, {0xF312, "Moveto_d"},   {0xF30C, "Moveto_x_7F"},  {0xF34A, "Reset0Int"},   {0xF34F, "Check0Ref"},
        {0xF354, "Reset0Ref"},    {0xF35B, "Reset_Pen"},    {0xF373, "Print_Str_hwyx"}, {0xF378, "Print_Str_yx"}, {0xF37A, "Print_Str_d"},  {0xF38A, "Print_List"},
        {0xF38C, "Print_List_hw"}, {0xF3B7, "Mov_Draw_VL"}, {0xF3CE, "Draw_VLc"},     {0xF3D4, "Draw_VL_b"},    {0xF3D6, "Draw_VLcs"},   {0xF3DA, "Draw_VL"},
        {0xF3DD, "Draw_VL_mode"}, {0xF3DF, "Draw_Line_d"},  {0xF40C, "Draw_Grid_VL"}, {0xF511, "Random_3"},    {0xF517, "Random"},      {0xF53F, "Clear_x_b"},
        {0xF548, "Clear_C8_RAM"}, {0xF55A, "Clear_x_256"},  {0xF55E, "Dec_6_Counters"}, {0xF563, "Dec_3_Counters"}, {0xF56D, "Dec_Counters"}, {0xF57D, "Delay_3"},
        {0xF57E, "Delay_2"},      {0xF57F, "Delay_1"},      {0xF579, "Delay_0"},      {0xF593, "Rise_Run_Angle"}, {0xF683, "Move_Mem_a"}, {0xF67F, "Move_Mem_a_1"}};
    return table;
}

const std::map<std::uint32_t, std::string>& via() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xD000, "VIA_port_b"},    {0xD001, "VIA_port_a"},   {0xD002, "VIA_DDR_b"},    {0xD003, "VIA_DDR_a"},      {0xD004, "VIA_t1_cnt_lo"}, {0xD005, "VIA_t1_cnt_hi"},
        {0xD006, "VIA_t1_lch_lo"}, {0xD007, "VIA_t1_lch_hi"}, {0xD008, "VIA_t2_lo"},   {0xD009, "VIA_t2_hi"},      {0xD00A, "VIA_shift_reg"}, {0xD00B, "VIA_aux_cntl"},
        {0xD00C, "VIA_cntl"},      {0xD00D, "VIA_int_flags"}, {0xD00E, "VIA_int_enable"}, {0xD00F, "VIA_port_a_nohs"}};
    return table;
}

formats::Decoder decoder() {
    return [](ByteView mem, std::uint32_t pc) -> std::optional<formats::DecodedInsn> {
        const std::optional<dm::Insn> insn = dm::decode(mem, pc, pc);
        if (!insn) return std::nullopt;
        formats::DecodedInsn d;
        d.size = insn->size;
        d.target = insn->target;
        switch (insn->flow) {
            case dm::Flow::None: d.flow = formats::Flow::Next; break;
            case dm::Flow::Branch: d.flow = formats::Flow::Branch; break;
            case dm::Flow::Jump: d.flow = insn->target ? formats::Flow::Jump : formats::Flow::Indirect; break;
            case dm::Flow::Call: d.flow = insn->target ? formats::Flow::Call : formats::Flow::Next; break;
            case dm::Flow::Return: d.flow = formats::Flow::Return; break;
            case dm::Flow::Indirect: d.flow = formats::Flow::Indirect; break;
            case dm::Flow::Halt: d.flow = formats::Flow::Halt; break;
        }
        return d;
    };
}

iw::Written write(const formats::vectrex::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const VectrexFlow flow(rom);
    const std::vector<std::uint32_t> entries = {rom.entry};
    const iw::Walked<dm::Insn> walked = iw::walk<dm::Insn>(flow, entries);
    const std::map<std::uint32_t, dm::Insn>& code = walked.code;
    iw::Names names;
    names.try_emplace(rom.entry, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return a >= rom.entry && a < rom.data.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    // a 16-bit immediate or an extended address that is the start of a line of ROM gets that line's label
    for (const auto& [at, insn] : code) {
        if ((insn.mode == dm::Mode::Ext || insn.mode == dm::Mode::Imm16) && line_start(insn.value)) labels.try_emplace(insn.value, std::format("D_{:04X}", insn.value));
    }
    std::set<std::uint32_t> used;   // equates: BIOS routines and VIA registers the code names
    for (const auto& [at, insn] : code) {
        const bool names_address = insn.mode == dm::Mode::Ext || insn.mode == dm::Mode::Imm16 || (insn.target && insn.flow != dm::Flow::Branch);
        const std::uint32_t value = insn.mode == dm::Mode::Ext || insn.mode == dm::Mode::Imm16 ? insn.value : insn.target.value_or(0);
        if (names_address && hardware_name(value)) used.insert(value);
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = *hardware_name(a);

    // each instruction as text, or as bytes when the text would not assemble back to them
    decompile::NameLabels name_values;
    for (const auto& [a, n] : address_labels) name_values[n] = a;
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        std::string t = dm::text(insn, address_labels);
        bool same = false;
        try {
            const Bytes built = dm::assemble(t, at, name_values);
            same = built.size() == insn.size && std::equal(built.begin(), built.end(), insn.raw.begin());
        } catch (const Error&) {
        }
        if (same) {
            instruction_text[at] = std::move(t);
        } else {
            std::string row = "FCB ";
            for (std::size_t k = 0; k < insn.size; ++k) row += (k ? "," : "") + hex8(insn.raw[k]);
            instruction_text[at] = row + "  ; " + t;
        }
    }

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Vectrex cartridge, {} bytes, \"{}\", program at ${:04X}", rom.data.size(), rom.copyright, rom.entry), ""},
                                "6809", std::string(file_name));
    for (const std::uint32_t a : used) out.append(std::format("{:<16} EQU ${:04X}", *hardware_name(a), a));
    if (!used.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append("        ORG $0000");
    VectrexListing listing(out, rom, code, instruction_text);
    iw::emit<dm::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return build::m6809_asm::build(source); }, rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::vectrex::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::vectrex_writer
