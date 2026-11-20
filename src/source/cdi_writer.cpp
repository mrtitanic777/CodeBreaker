// cb-unity: alone
// source/cdi_writer.cpp -- the modules of a CD-i disc as 68000 source that builds back into them (cdi_writer.hpp).

#include "source/cdi_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "build/m68k_asm.hpp"
#include "core/fs.hpp"
#include "decompile/m68k.hpp"
#include "source/isa.hpp"

namespace cb::source::cdi_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;
namespace cd = formats::cdi;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
const iw::DataSyntax kSyntax{.bytes = "dc.b", .fill = "dcb.b", .fill_separator = ","};

// `TRAP #0`: the call is the word after it.
bool os9_call(const m68k::Insn& insn) {
    if (insn.mnemonic != "TRAP") return false;
    const auto ops = insn.operands();
    return !ops.empty() && ops[0].value == 0;
}

std::uint32_t be16(ByteView d, std::size_t at) { return static_cast<std::uint32_t>(d[at] << 8 | d[at + 1]); }
std::uint32_t be32(ByteView d, std::size_t at) { return be16(d, at) << 16 | be16(d, at + 2); }

class CdiFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit CdiFlow(const cd::Program& program) : program_(program), memory_(program.memory()) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(memory_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return os9_call(insn) ? 4 : insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && program_.module_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override {
        const cd::Module* m = program_.module_at(pc);
        return m && pc + size(insn) <= m->address + m->size;
    }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target % 2 == 0 && program_.module_at(*insn.target) != nullptr && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const m68k::Insn& insn) const override { return insn.mnemonic == "JSR" || insn.mnemonic == "BSR"; }

private:
    const cd::Program& program_;
    Bytes memory_;
};

class CdiListing final : public iw::Listing<m68k::Insn> {
public:
    CdiListing(translate::l2::Assembly& out, const cd::Program& program, const cd::Module& module, const std::map<std::uint32_t, m68k::Insn>& code,
               const decompile::AddressLabels& names, std::set<std::string>& used_calls)
        : out_(out), program_(program), module_(module), code_(code), names_(names), used_calls_(used_calls) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.file[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return module_.address + (offset - module_.file_offset); }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return os9_call(insn) ? 4 : insn.length; }
    std::string text(const m68k::Insn& insn) const override {
        std::string line = m68k::text(insn, names_);
        if (!os9_call(insn)) return line;
        const std::uint32_t code = be16(program_.file, program_.offset(insn.pc).value() + 2);
        const auto named = system_calls().find(code);
        if (named == system_calls().end()) return line + std::format("\n        dc.w ${:04X}", code);
        used_calls_.insert(named->second);
        return line + "\n        dc.w " + named->second;
    }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }

    // The module's header, field by field.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != module_.file_offset) return 0;
        flush();
        const ByteView f = program_.file;
        const std::size_t h = offset;
        out_.append(std::format("        dc.w ${:04X}  ; module sync, {}", be16(f, h), module_.name));
        out_.append(std::format("        dc.w ${:04X}  ; system revision", be16(f, h + 2)));
        out_.append(std::format("        dc.l ${:08X}  ; module size", be32(f, h + 4)));
        out_.append(std::format("        dc.l ${:08X}  ; owner", be32(f, h + 8)));
        out_.append(std::format("        dc.l ${:08X}  ; name offset", be32(f, h + 12)));
        out_.append(std::format("        dc.w ${:04X}  ; access", be16(f, h + 16)));
        out_.append(std::format("        dc.b ${:02X},${:02X}  ; type, language", f[h + 18], f[h + 19]));
        out_.append(std::format("        dc.b ${:02X},${:02X}  ; attributes, revision", f[h + 20], f[h + 21]));
        out_.append(std::format("        dc.w ${:04X}  ; edition", be16(f, h + 22)));
        out_.append(std::format("        dc.l ${:08X}  ; usage", be32(f, h + 24)));
        out_.append(std::format("        dc.l ${:08X}  ; symbol table", be32(f, h + 28)));
        out_.extend(iw::commented(iw::data_lines(f.subspan(h + 32, 14), kSyntax), "reserved"));
        out_.append(std::format("        dc.w ${:04X}  ; header parity", be16(f, h + 46)));
        return static_cast<std::uint32_t>(cd::kHeader);
    }

private:
    translate::l2::Assembly& out_;
    const cd::Program& program_;
    const cd::Module& module_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const decompile::AddressLabels& names_;
    std::set<std::string>& used_calls_;
};

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

// What a module's name makes of a label: letters and digits, lower case.
std::string label_of(const std::string& name, std::size_t index) {
    std::string s;
    for (const char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c))) s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    if (s.empty() || std::isdigit(static_cast<unsigned char>(s[0]))) s = "module" + std::to_string(index) + s;
    return "main_" + s;
}

Bytes rebuilt_file(const cd::Program& program, const Bytes& image) {
    Bytes out = program.file;
    for (const cd::Module& m : program.modules) {
        if (!m.runs()) continue;
        for (std::uint32_t i = 0; i < m.size; ++i) out[m.file_offset + i] = m.address + i < image.size() ? image[m.address + i] : 0;
    }
    return out;
}

}  // namespace

const std::map<std::uint32_t, std::string>& system_calls() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x00, "F_Link"},  {0x01, "F_Load"},   {0x02, "F_UnLink"}, {0x03, "F_Fork"},   {0x04, "F_Wait"},   {0x05, "F_Chain"}, {0x06, "F_Exit"},
        {0x07, "F_Mem"},   {0x08, "F_Send"},   {0x09, "F_Icpt"},   {0x0A, "F_Sleep"},  {0x0C, "F_SPrior"}, {0x0D, "F_ID"},    {0x0E, "F_SSWI"},
        {0x15, "F_STime"}, {0x80, "I_Attach"}, {0x81, "I_Detach"}, {0x82, "I_Dup"},    {0x83, "I_Create"}, {0x84, "I_Open"},  {0x85, "I_MakDir"},
        {0x86, "I_ChgDir"}, {0x87, "I_Delete"}, {0x88, "I_Seek"},  {0x89, "I_Read"},   {0x8A, "I_Write"},  {0x8B, "I_ReadLn"}, {0x8C, "I_WritLn"},
        {0x8D, "I_GetStt"}, {0x8E, "I_SetStt"}, {0x8F, "I_Close"}};
    return table;
}

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = {{0x200000, "MCD212_CSR1"}, {0x200010, "MCD212_DCR1"}, {0x300000, "SLAVE_BASE"},
                                                               {0x310000, "SLAVE_CMD"},   {0x320000, "NVRAM"},       {0x340000, "CLOCK"}};
    return table;
}

iw::Written write(const cd::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const CdiFlow flow(program);
    const std::vector<std::uint32_t> entries = program.entries();
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;

    iw::Names names;
    std::size_t index = 0;
    for (const cd::Module& m : program.modules) {
        if (m.runs()) names.try_emplace(m.entry(), label_of(m.name, index));
        ++index;
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + flow.size(insn); ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return a % 2 == 0 && program.module_at(a) != nullptr && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const Reference& ref : references(entry.second)) {
            if (hardware().contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value))) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kLocal : kData)(ref.value));
            }
        }
    }
    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    std::set<std::string> used_calls;
    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
                                 std::format("; Philips CD-i {}: {} module(s), {} run by the model", program.disc ? "disc" : "module file", program.modules.size(), program.placed().size()), ""},
                                "asm68k", std::string(file_name));
    std::vector<cd::Module> order = program.placed();
    std::sort(order.begin(), order.end(), [](const cd::Module& a, const cd::Module& b) { return a.address < b.address; });
    // the segments are written into a second assembly first, so the equates for the calls and chips used can come before them
    translate::l2::Assembly body({}, "asm68k", std::string(file_name));
    for (const cd::Module& m : order) {
        body.part(m.name.empty() ? "module" : m.name, "segment", std::nullopt, m.size);
        body.append(std::format("        org ${:06X}", m.address));
        CdiListing listing(body, program, m, code, names_for_text, used_calls);
        iw::emit<m68k::Insn>(body, m.file_offset, m.file_offset + m.size, labels, functions, listing);
        body.append("");
    }
    for (const std::uint32_t a : used) out.append(std::format("{:<16} equ ${:06X}", hardware().at(a), a));
    for (const std::string& name : used_calls) {
        for (const auto& [code_number, n] : system_calls()) {
            if (n == name) out.append(std::format("{:<16} equ ${:02X}", name, code_number));
        }
    }
    out.append("");
    const std::string source = iw::text(out) + "\n" + iw::text(body) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.file.size());
    const build::m68k_asm::Assembler assembler(isa::m68k_encoder());
    iw::assembled(r, [&] { return rebuilt_file(program, assembler.build(source)); }, program.file, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(cd::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::cdi_writer
