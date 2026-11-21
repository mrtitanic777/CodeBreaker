// cb-unity: alone
// source/vectrex_writer.cpp -- a Vectrex cartridge as 6809 source that builds back into it (vectrex_writer.hpp).

#include "source/vectrex_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "decompile/m6809.hpp"
#include "source/described_source.hpp"
#include "source/processors.hpp"

namespace cb::source::vectrex_writer {

namespace dm = decompile::m6809;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
const processors::Processor& m6809_cpu() {
    static const processors::Processor& cpu = processors::find("m6809");
    return cpu;
}

bool printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"'; });
}

std::string hex8(std::uint8_t v) { return std::format("0x{:02x}", v); }

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
    std::vector<std::string> data_lines(ByteView data) const override { return described_source::data_lines(data); }

    // The header: copyright text, music pointer, title lines, the closing zero.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != 0) return 0;
        flush();
        const ByteView d = rom_.data;
        const std::size_t copyright = rom_.copyright.size();
        out_.append(printable(d.first(copyright)) ? std::format("        .ascii \"{}\"", rom_.copyright) : "        .byte " + hex8(d[0]));
        if (!printable(d.first(copyright))) out_.extend(described_source::data_lines(d.subspan(1, copyright - 1)));
        out_.append("        .byte 0x80");
        out_.append(std::format("        .half 0x{:04x}  ; music", rom_.music));
        std::size_t p = copyright + 3;
        for (const formats::vectrex::TitleLine& t : rom_.title) {
            out_.append(std::format("        .byte {},{},{},{}  ; height, width, y, x", hex8(d[p]), hex8(d[p + 1]), hex8(d[p + 2]), hex8(d[p + 3])));
            const ByteView text = d.subspan(p + 4, t.text.size());
            if (printable(text)) out_.append(std::format("        .ascii \"{}\"", t.text));
            else out_.extend(described_source::data_lines(text));
            out_.append("        .byte 0x80");
            p += 4 + t.text.size() + 1;
        }
        out_.append("        .byte 0x0");
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
    // config/knowledge/vectrex/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().table_names("vectrex", "bios");
    return table;
}

const std::map<std::uint32_t, std::string>& via() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("vectrex", "via");
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
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        // (the description's text, the label or name of the address an operand or a target is written in its place, kept when it builds back into the bytes)
        std::optional<std::string> t = described_source::checked_text(m6809_cpu(), insn.bytes(), at, insn.size);
        if (t) {
            const bool names_address = insn.mode == dm::Mode::Ext || insn.mode == dm::Mode::Imm16;
            const std::optional<std::uint32_t> named = names_address ? std::optional<std::uint32_t>(insn.value) : insn.target;
            if (named && address_labels.contains(*named)) *t = described_source::name_number(*t, *named, address_labels.at(*named), false);
            instruction_text[at] = *t;
        } else {
            instruction_text[at] = described_source::byte_line(m6809_cpu(), insn.bytes(), at, {}, "");
        }
    }

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Vectrex cartridge, {} bytes, \"{}\", program at ${:04X}", rom.data.size(), rom.copyright, rom.entry), ""},
                                "sleigh-m6809", std::string(file_name));
    for (const std::uint32_t a : used) out.append(described_source::equ(*hardware_name(a), a, 4));
    if (!used.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append("        .org 0x0000");
    VectrexListing listing(out, rom, code, instruction_text);
    iw::emit<dm::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(r, [&] { return described_source::assemble(m6809_cpu(), source, folder); }, rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::vectrex::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::vectrex_writer
