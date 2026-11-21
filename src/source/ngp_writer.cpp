// cb-unity: alone
// source/ngp_writer.cpp -- a Neo Geo Pocket cartridge as TLCS-900/H source that builds back into it (ngp_writer.hpp).

#include "source/ngp_writer.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/tlcs900.hpp"
#include "source/described_source.hpp"
#include "source/processors.hpp"

namespace cb::source::ngp_writer {

namespace dt = decompile::tlcs900;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
const processors::Processor& tlcs900_cpu() {
    static const processors::Processor& cpu = processors::find("tlcs900");
    return cpu;
}
constexpr std::uint32_t kFirstVector = 0x6FB8, kLastVector = 0x6FFF;   // the BIOS's table of interrupt handlers the program fills in

bool printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"' && c != '\\' && c != ';'; });
}

class NgpFlow final : public iw::CodeModel<dt::Insn> {
public:
    explicit NgpFlow(const formats::ngp::Rom& rom) : rom_(rom) {}

    std::optional<dt::Insn> decode(std::uint32_t pc) const override { return inside(pc) ? dt::decode(rom_.data, pc - formats::ngp::kRomBase, pc) : std::nullopt; }
    std::uint32_t size(const dt::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const dt::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= formats::ngp::kRomBase && pc - formats::ngp::kRomBase < rom_.data.size(); }
    bool fits(std::uint32_t pc, const dt::Insn& insn) const override { return pc - formats::ngp::kRomBase + insn.size <= rom_.data.size(); }
    bool follows(const dt::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == dt::Flow::Branch || insn.flow == dt::Flow::Jump || insn.flow == dt::Flow::Call);
    }
    bool stops(const dt::Insn& insn) const override { return insn.flow == dt::Flow::Jump || insn.flow == dt::Flow::Return || insn.flow == dt::Flow::Indirect; }
    bool finds_functions() const override { return true; }
    bool starts_function(const dt::Insn& insn) const override { return insn.flow == dt::Flow::Call; }

private:
    const formats::ngp::Rom& rom_;
};

class NgpListing final : public iw::Listing<dt::Insn> {
public:
    NgpListing(translate::l2::Assembly& out, const formats::ngp::Rom& rom, const std::map<std::uint32_t, dt::Insn>& code, const std::map<std::uint32_t, std::string>& text,
               bool header_is_data, bool start_is_labelled)
        : out_(out), rom_(rom), code_(code), text_(text), header_is_data_(header_is_data), start_is_labelled_(start_is_labelled) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address(offset); }
    const dt::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const dt::Insn& insn) const override { return insn.size; }
    std::string text(const dt::Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return described_source::data_lines(data); }

    // The header: licence, start address, catalogue, sub-catalogue, colour, title, reserved bytes.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (!header_is_data_ || offset != 0) return 0;
        flush();
        const ByteView d = ByteView(rom_.data).first(formats::ngp::kHeaderSize);
        const auto field = [&](std::size_t at, std::size_t n, const char* what) {
            const ByteView f = d.subspan(at, n);
            if (printable(f)) out_.append(std::format("        .ascii \"{}\"  ; {}", std::string(reinterpret_cast<const char*>(f.data()), f.size()), what));
            else out_.extend(iw::commented(described_source::data_lines(f), what));
        };
        field(0, 28, "licence");
        if (start_is_labelled_) out_.append("        .long start  ; start address");
        else out_.extend(iw::commented(described_source::data_lines(d.subspan(0x1C, 4)), "start address"));
        out_.extend(iw::commented(described_source::data_lines(d.subspan(0x20, 2)), "catalogue number"));
        out_.extend(iw::commented(described_source::data_lines(d.subspan(0x22, 1)), "sub-catalogue"));
        out_.extend(iw::commented(described_source::data_lines(d.subspan(0x23, 1)), d[0x23] == 0x10 ? "colour" : "monochrome"));
        field(0x24, 12, "title");
        out_.extend(iw::commented(described_source::data_lines(d.subspan(0x30, 16)), "reserved"));
        return static_cast<std::uint32_t>(formats::ngp::kHeaderSize);
    }

private:
    translate::l2::Assembly& out_;
    const formats::ngp::Rom& rom_;
    const std::map<std::uint32_t, dt::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    bool header_is_data_;
    bool start_is_labelled_;
};

// The addresses of the cartridge an instruction names: a 32-bit constant, an absolute memory operand, a PC-relative one.
void rom_references(const dt::Insn& insn, const formats::ngp::Rom& rom, std::set<std::uint32_t>& out) {
    const dt::Row& r = *insn.row;
    const auto inside = [&](std::uint32_t a) { return a >= formats::ngp::kRomBase + formats::ngp::kHeaderSize && a - formats::ngp::kRomBase < rom.data.size(); };
    for (unsigned k = 0; k < 2; ++k) {
        const dt::Tok t = k == 0 ? r.a : r.b;
        if ((t == dt::Tok::Il || (t == dt::Tok::Iz && insn.z == 4)) && inside(insn.v[k])) out.insert(insn.v[k]);
    }
    if (r.ctx == dt::Ctx::Src || r.ctx == dt::Ctx::Dst) {
        const dt::MemKind k = insn.m.kind;
        if ((k == dt::MemKind::Abs16 || k == dt::MemKind::Abs24 || k == dt::MemKind::PcRel) && inside(insn.m.addr)) out.insert(insn.m.addr);
    }
}

// The handlers a program installs: `ld XWA,<address of the cartridge>` then `ld (<a slot of the BIOS's vector table>),XWA` in a straight line of code.
std::map<std::uint32_t, std::string> installed_handlers(const std::map<std::uint32_t, dt::Insn>& code) {
    std::map<std::uint32_t, std::string> found;
    std::array<std::optional<std::uint32_t>, 8> known;
    std::uint32_t expected = 0;
    for (const auto& [at, insn] : code) {
        if (at != expected) known = {};
        expected = at + insn.size;
        const dt::Row& r = *insn.row;
        if (r.id == dt::Op::Ld && r.ctx == dt::Ctx::Fixed && r.a == dt::Tok::R32) {
            known[insn.op & 7u] = insn.v[1];
        } else if (r.id == dt::Op::Ld && r.ctx == dt::Ctx::Dst && r.a == dt::Tok::M && r.b == dt::Tok::R32 && insn.m.kind == dt::MemKind::Abs16 && insn.m.addr >= kFirstVector &&
                   insn.m.addr <= kLastVector && known[insn.op & 7u]) {
            const auto name = formats::ngp::hardware().find(insn.m.addr);
            std::string label = name == formats::ngp::hardware().end() ? std::format("isr_{:04X}", insn.m.addr) : "isr_" + text::lower(std::string_view(name->second).substr(std::string_view("BIOS_INT_").size()));
            found.try_emplace(*known[insn.op & 7u], std::move(label));
        }
        if (insn.flow != dt::Flow::None) known = {};
    }
    return found;
}

}  // namespace

iw::Written write(const formats::ngp::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    namespace ngp = formats::ngp;
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const NgpFlow flow(rom);
    iw::Names names;
    names.try_emplace(rom.header.start, "start");
    std::vector<std::uint32_t> entries = {rom.header.start};
    iw::Walked<dt::Insn> walked = iw::walk<dt::Insn>(flow, entries);
    for (unsigned round = 0; round < 6; ++round) {   // (code found may install more handlers)
        bool grew = false;
        for (const auto& [address, name] : installed_handlers(walked.code)) {
            if (!flow.inside(address) || !dt::decode(rom.data, address - ngp::kRomBase, address)) continue;
            if (std::find(entries.begin(), entries.end(), address) == entries.end()) {
                entries.push_back(address);
                names.try_emplace(address, name);
                grew = true;
            }
        }
        if (!grew) break;
        walked = iw::walk<dt::Insn>(flow, entries);
    }
    const std::map<std::uint32_t, dt::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return flow.inside(a) && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> referenced;
    for (const auto& [at, insn] : code) rom_references(insn, rom, referenced);
    for (const std::uint32_t a : referenced) {
        if (line_start(a)) labels.try_emplace(a, kData(a));
    }
    decompile::AddressLabels code_labels, all_labels;
    for (const auto& [a, n] : labels) code_labels[a] = n;
    all_labels = code_labels;
    for (const auto& [a, n] : ngp::hardware()) all_labels.try_emplace(a, n);

    // each instruction as text (with the names of hardware registers, else with numbers only, else with no names at all), or as bytes when no text assembles back to them
    std::map<std::uint32_t, std::string> instruction_text;
    std::set<std::string> used;   // the names the instruction texts use
    for (const auto& [at, insn] : code) {
        // (the description's text, kept when it builds back into the instruction's bytes; the program's names where its numbers are)
        std::optional<std::string> t = described_source::checked_text(tlcs900_cpu(), insn.bytes(), at, insn.size);
        if (t) *t = described_source::name_numbers(*t, all_labels);
        if (t) {
            std::string word;
            for (const char c : *t + " ") {
                if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                    word += c;
                } else {
                    if (!word.empty()) used.insert(word);
                    word.clear();
                }
            }
            instruction_text[at] = std::move(*t);
        } else {
            instruction_text[at] = described_source::byte_line(tlcs900_cpu(), insn.bytes(), at, {}, "");
        }
    }
    std::set<std::uint32_t> equates;
    for (const auto& [a, n] : ngp::hardware()) {
        if (used.contains(n)) equates.insert(a);
    }

    bool header_is_data = true;
    for (std::size_t k = 0; k < ngp::kHeaderSize; ++k) {
        const std::uint32_t a = rom.address(k);
        if (code.contains(a) || inside_insn.contains(a)) header_is_data = false;
    }
    // the header's start address is the `start` label when its four bytes are that label's address (written as one .long)
    const bool start_labelled = labels.contains(rom.header.start) && labels.at(rom.header.start) == "start" && rom.data[0x1E] == ((rom.header.start >> 16) & 0xFF) &&
                                rom.data[0x1F] == ((rom.header.start >> 24) & 0xFF);

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Neo Geo Pocket {} cartridge, {} bytes, \"{}\", start at 0x{:06X}", rom.header.colour ? "Color" : "", rom.data.size(), rom.header.title, rom.header.start), ""},
                                "sleigh-tlcs900", std::string(file_name));
    for (const std::uint32_t a : equates) out.append(described_source::equ(ngp::hardware().at(a), a, 4));
    if (!equates.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:06x}", ngp::kRomBase));
    NgpListing listing(out, rom, code, instruction_text, header_is_data, start_labelled);
    iw::emit<dt::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            return described_source::assemble(tlcs900_cpu(), source, folder);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) { return write(formats::ngp::read(rom_file), rom_file.filename().string(), folder); }

}  // namespace cb::source::ngp_writer
