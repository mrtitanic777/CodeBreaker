// cb-unity: alone
// source/chf_writer.cpp -- a Channel F cartridge as F8 source that builds back into it (chf_writer.hpp).

#include "source/chf_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "decompile/f8.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/processors.hpp"

namespace cb::source::chf_writer {

namespace df = decompile::f8;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};
const processors::Processor& f8_cpu() {
    static const processors::Processor& cpu = processors::find("f8");
    return cpu;
}

class ChfFlow final : public iw::CodeModel<df::Insn> {
public:
    explicit ChfFlow(const formats::chf::Rom& rom) : rom_(rom) {}

    std::optional<df::Insn> decode(std::uint32_t pc) const override { return inside(pc) ? df::decode(rom_.data, pc - formats::chf::kBase, pc) : std::nullopt; }
    std::uint32_t size(const df::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const df::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= formats::chf::kBase && pc - formats::chf::kBase < rom_.data.size(); }
    bool fits(std::uint32_t pc, const df::Insn& insn) const override { return pc - formats::chf::kBase + insn.size <= rom_.data.size(); }
    bool follows(const df::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == df::Flow::Branch || insn.flow == df::Flow::Jump || insn.flow == df::Flow::Call);
    }
    bool stops(const df::Insn& insn) const override { return insn.flow == df::Flow::Jump || insn.flow == df::Flow::Return || insn.flow == df::Flow::Indirect; }
    bool finds_functions() const override { return true; }
    bool starts_function(const df::Insn& insn) const override { return insn.flow == df::Flow::Call; }

private:
    const formats::chf::Rom& rom_;
};

class ChfListing final : public iw::Listing<df::Insn> {
public:
    ChfListing(const formats::chf::Rom& rom, const std::map<std::uint32_t, df::Insn>& code, const std::map<std::uint32_t, std::string>& text) : rom_(rom), code_(code), text_(text) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address(offset); }
    const df::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const df::Insn& insn) const override { return insn.size; }
    std::string text(const df::Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return dialect::find("asmf8").data_lines(data); }

private:
    const formats::chf::Rom& rom_;
    const std::map<std::uint32_t, df::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
};

}  // namespace

iw::Written write(const formats::chf::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    namespace chf = formats::chf;
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const ChfFlow flow(rom);
    const std::vector<std::uint32_t> entries = {chf::kBase};
    const iw::Walked<df::Insn> walked = iw::walk<df::Insn>(flow, entries);
    const std::map<std::uint32_t, df::Insn>& code = walked.code;
    iw::Names names;
    names.try_emplace(chf::kBase, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return flow.inside(a) && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    for (const auto& [at, insn] : code) {   // a DCI's constant that is a line of the cartridge
        if (insn.op == 0x2A && line_start(insn.value)) labels.try_emplace(insn.value, kData(insn.value));
    }
    std::set<std::uint32_t> used;   // BIOS routines the code names
    for (const auto& [at, insn] : code) {
        if (insn.target && chf::bios().contains(*insn.target)) used.insert(*insn.target);
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = chf::bios().at(a);

    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        // (the description's text, a branch target or a DCI's constant written as its label, kept when it builds back into the instruction's bytes)
        std::optional<std::string> t = described_source::checked_text(f8_cpu(), insn.bytes(), at, insn.size);
        if (t) {
            const std::optional<std::uint32_t> named = insn.target ? insn.target : insn.op == 0x2A ? std::optional<std::uint32_t>(insn.value) : std::nullopt;
            if (named && address_labels.contains(*named)) *t = described_source::name_number(*t, *named, address_labels.at(*named), false);
            instruction_text[at] = *t;
        } else {
            instruction_text[at] = dialect::find("asmf8").fallback(described_source::byte_line(f8_cpu(), insn.bytes(), at, {}, ""));
        }
    }

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Channel F cartridge, {} bytes at $0800 (the F8's memory)", rom.data.size()), ""},
                                "asmf8", std::string(file_name));
    for (const std::uint32_t a : used) out.append(described_source::equ(chf::bios().at(a), a, 4));
    if (!used.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:04x}", chf::kBase));
    ChfListing listing(rom, code, instruction_text);
    iw::emit<df::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = dialect::find("asmf8").print(iw::text(out) + "\n");   // (said in the F8 assembler's, config/dialects/asmf8.json)
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            return described_source::assemble(f8_cpu(), dialect::find("asmf8").parse(source), folder);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) { return write(formats::chf::read(rom_file), rom_file.filename().string(), folder); }

}  // namespace cb::source::chf_writer
