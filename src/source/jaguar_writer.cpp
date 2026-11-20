// cb-unity: alone
// source/jaguar_writer.cpp -- an Atari Jaguar program as assembly source that builds back into it (jaguar_writer.hpp).

#include "source/jaguar_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m68k.hpp"
#include "source/isa.hpp"
#include "source/risc_source.hpp"

namespace cb::source::jaguar_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;
namespace jag = formats::jaguar;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

// Flow runs through the segments at even addresses.
class JaguarFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit JaguarFlow(const jag::Program& program) : program_(program), memory_(program.memory()) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(memory_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && program_.segment_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override {
        const jag::Segment* s = program_.segment_at(pc);
        return s && pc + insn.length <= s->address + s->size;
    }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target % 2 == 0 && program_.segment_at(*insn.target) != nullptr && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const m68k::Insn& insn) const override { return insn.mnemonic == "JSR" || insn.mnemonic == "BSR"; }

private:
    const jag::Program& program_;
    Bytes memory_;
};

class JaguarListing final : public iw::Listing<m68k::Insn> {
public:
    JaguarListing(translate::l2::Assembly& out, const jag::Program& program, const jag::Segment& segment, const std::map<std::uint32_t, m68k::Insn>& code,
                  const iw::Names& labels, const decompile::AddressLabels& names)
        : out_(out), program_(program), segment_(segment), code_(code), labels_(labels), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.file[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return segment_.address + (offset - segment_.offset); }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return risc_source::m68k_code(insn, names_); }   // (the description's text, the names put in)
    std::vector<std::string> data_lines(ByteView data) const override { return risc_source::data_lines(data); }

    // A cartridge's boot signature and its header: the flags long and the start address.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (program_.kind != jag::Kind::Cartridge) return 0;
        if (offset == 0) {
            flush();
            out_.extend(iw::commented(risc_source::data_lines(ByteView(program_.file).first(jag::kHeaderAt)), "the boot signature"));
            return static_cast<std::uint32_t>(jag::kHeaderAt);
        }
        if (offset == jag::kHeaderAt) {
            flush();
            out_.append(std::format("        .long 0x{:08x}  ; header flags", program_.header_flags));
            const auto named = labels_.find(program_.entry);
            out_.append("        .long " + (named != labels_.end() ? named->second : std::format("0x{:08x}", program_.entry)) + "  ; start address");
            return 8;
        }
        return 0;
    }

private:
    translate::l2::Assembly& out_;
    const jag::Program& program_;
    const jag::Segment& segment_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& names_;
};

// The addresses an instruction's operands name, and whether it is a long immediate.
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

// What the assembled image makes of the file: the segments' bytes follow one another in the order of their addresses (the order they are written in); a
// cartridge is those bytes, a COFF is the original with each section's bytes replaced.
Bytes rebuilt_file(const jag::Program& program, const Bytes& image) {
    Bytes out = program.file;
    std::vector<jag::Segment> order = program.segments;
    std::sort(order.begin(), order.end(), [](const jag::Segment& a, const jag::Segment& b) { return a.address < b.address; });
    std::size_t at = 0;
    for (const jag::Segment& s : order) {
        for (std::uint32_t i = 0; i < s.size; ++i, ++at) out[s.offset + i] = at < image.size() ? image[at] : 0;
    }
    return out;
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xF00000, "MEMCON1"}, {0xF00002, "MEMCON2"}, {0xF00004, "HC"},      {0xF00006, "VC"},      {0xF00008, "LPH"},     {0xF0000A, "LPV"},
        {0xF00010, "OB0"},     {0xF00012, "OB1"},     {0xF00014, "OB2"},    {0xF00016, "OB3"},     {0xF00020, "OLP"},     {0xF00026, "OBF"},
        {0xF00028, "VMODE"},   {0xF0002A, "BORD1"},   {0xF0002C, "BORD2"},  {0xF0002E, "HP"},      {0xF00030, "HBB"},     {0xF00032, "HBE"},
        {0xF00034, "HS"},      {0xF00036, "HVS"},     {0xF00038, "HDB1"},   {0xF0003A, "HDB2"},    {0xF0003C, "HDE"},     {0xF0003E, "VP"},
        {0xF00040, "VBB"},     {0xF00042, "VBE"},     {0xF00044, "VS"},     {0xF00046, "VDB"},     {0xF00048, "VDE"},     {0xF0004A, "VEB"},
        {0xF0004C, "VEE"},     {0xF0004E, "VI"},      {0xF00050, "PIT0"},   {0xF00052, "PIT1"},    {0xF00054, "HEQ"},     {0xF00058, "BG"},
        {0xF000E0, "INT1"},    {0xF000E2, "INT2"},    {0xF00400, "CLUT"},   {0xF00800, "LBUFA"},   {0xF01000, "LBUFB"},   {0xF01800, "LBUFC"},
        {0xF02100, "G_FLAGS"}, {0xF02104, "G_MTXC"},  {0xF02108, "G_MTXA"}, {0xF0210C, "G_END"},   {0xF02110, "G_PC"},    {0xF02114, "G_CTRL"},
        {0xF02118, "G_HIDATA"}, {0xF0211C, "G_DIVCTRL"}, {0xF02200, "A1_BASE"}, {0xF02204, "A1_FLAGS"}, {0xF02208, "A1_CLIP"}, {0xF0220C, "A1_PIXEL"},
        {0xF02210, "A1_STEP"}, {0xF02214, "A1_FSTEP"}, {0xF02218, "A1_FPIXEL"}, {0xF0221C, "A1_INC"}, {0xF02220, "A1_FINC"}, {0xF02224, "A2_BASE"},
        {0xF02228, "A2_FLAGS"}, {0xF0222C, "A2_MASK"}, {0xF02230, "A2_PIXEL"}, {0xF02234, "A2_STEP"}, {0xF02238, "B_CMD"},  {0xF0223C, "B_COUNT"},
        {0xF02240, "B_SRCD"},  {0xF02248, "B_DSTD"},  {0xF02250, "B_DSTZ"}, {0xF02258, "B_SRCZ1"}, {0xF02260, "B_SRCZ2"}, {0xF02268, "B_PATD"},
        {0xF02270, "B_IINC"},  {0xF02274, "B_ZINC"},  {0xF02278, "B_STOP"}, {0xF0227C, "B_I3"},    {0xF02280, "B_I2"},    {0xF02284, "B_I1"},
        {0xF02288, "B_I0"},    {0xF0228C, "B_Z3"},    {0xF02290, "B_Z2"},   {0xF02294, "B_Z1"},    {0xF02298, "B_Z0"},    {0xF03000, "G_RAM"},
        {0xF10000, "JPIT1"},   {0xF10002, "JPIT2"},   {0xF10004, "JPIT3"}, {0xF10006, "JPIT4"},   {0xF10010, "CLK1"},    {0xF10012, "CLK2"},
        {0xF10020, "J_INT"},   {0xF14000, "JOYSTICK"}, {0xF14002, "JOYBUTS"}, {0xF1A100, "D_FLAGS"}, {0xF1A104, "D_MTXC"}, {0xF1A108, "D_MTXA"},
        {0xF1A10C, "D_END"},   {0xF1A110, "D_PC"},    {0xF1A114, "D_CTRL"}, {0xF1A118, "D_MOD"},   {0xF1A11C, "D_DIVCTRL"}, {0xF1A120, "D_MACHI"},
        {0xF1A148, "L_I2S"},   {0xF1A14C, "R_I2S"},   {0xF1A150, "SCLK"},  {0xF1A154, "SMODE"},   {0xF1B000, "D_RAM"}};
    return table;
}

iw::Written write(const jag::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const bool cartridge = program.kind == jag::Kind::Cartridge;
    const JaguarFlow flow(program);
    const std::vector<std::uint32_t> entries = {program.entry};
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;

    iw::Names names;
    names.try_emplace(program.entry, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a % 2 == 0 && program.segment_at(a) != nullptr && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const Reference& ref : references(entry.second)) {
            if (hardware().contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value) || ref.value >= 0x4000)) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kLocal : kData)(ref.value));
            }
        }
    }

    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
                                     std::format("; Atari Jaguar {}, {} bytes, start ${:06X}", cartridge ? "cartridge" : "COFF executable", program.file.size(), program.entry),
                                     ""};
    head.insert(head.end() - 1, "; (68000 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu m68k --sleigh`)");
    translate::l2::Assembly out(head, "sleigh-m68k", std::string(file_name));
    for (const std::uint32_t a : used) out.append(risc_source::equ(hardware().at(a), a, 6));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    std::vector<jag::Segment> order = program.segments;
    std::sort(order.begin(), order.end(), [](const jag::Segment& a, const jag::Segment& b) { return a.address < b.address; });
    for (const jag::Segment& s : order) {
        out.part(s.name.empty() ? "segment" : s.name, "segment", std::nullopt, s.size);
        out.append(std::format("        .org 0x{:06x}", s.address));
        JaguarListing listing(out, program, s, code, labels, names_for_text);
        iw::emit<m68k::Insn>(out, s.offset, s.offset + s.size, labels, functions, listing);
        out.append("");
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.file.size());
    const std::string extension = cartridge ? ".j64" : ".cof";
    iw::assembled(r, [&] { return rebuilt_file(program, risc_source::assemble("m68k", source, folder)); }, program.file, folder / (stem + ".built" + extension));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(jag::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::jaguar_writer
