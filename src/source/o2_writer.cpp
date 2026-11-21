// cb-unity: alone
// source/o2_writer.cpp -- an Odyssey 2 cartridge as 8048 source that builds back into it (o2_writer.hpp).

#include "source/o2_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "decompile/i8048.hpp"
#include "source/described_source.hpp"
#include "source/processors.hpp"

namespace cb::source::o2_writer {

namespace di = decompile::i8048;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
const processors::Processor& i8048_cpu() {
    static const processors::Processor& cpu = processors::find("i8048");
    return cpu;
}

class O2Flow final : public iw::CodeModel<di::Insn> {
public:
    explicit O2Flow(const formats::o2::Rom& rom) : rom_(rom) {}

    std::optional<di::Insn> decode(std::uint32_t pc) const override { return inside(pc) ? di::decode(rom_.data, pc - formats::o2::kBase, pc) : std::nullopt; }
    std::uint32_t size(const di::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const di::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= formats::o2::kBase && pc - formats::o2::kBase < rom_.data.size(); }
    bool fits(std::uint32_t pc, const di::Insn& insn) const override { return pc - formats::o2::kBase + insn.size <= rom_.data.size(); }
    bool follows(const di::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == di::Flow::Branch || insn.flow == di::Flow::Jump || insn.flow == di::Flow::Call);
    }
    bool stops(const di::Insn& insn) const override { return insn.flow == di::Flow::Jump || insn.flow == di::Flow::Return || insn.flow == di::Flow::Indirect; }
    bool finds_functions() const override { return true; }
    bool starts_function(const di::Insn& insn) const override { return insn.flow == di::Flow::Call; }

private:
    const formats::o2::Rom& rom_;
};

class O2Listing final : public iw::Listing<di::Insn> {
public:
    O2Listing(const formats::o2::Rom& rom, const std::map<std::uint32_t, di::Insn>& code, const std::map<std::uint32_t, std::string>& text) : rom_(rom), code_(code), text_(text) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address(offset); }
    const di::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const di::Insn& insn) const override { return insn.size; }
    std::string text(const di::Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return described_source::data_lines(data); }

private:
    const formats::o2::Rom& rom_;
    const std::map<std::uint32_t, di::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
};

}  // namespace

iw::Written write(const formats::o2::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    namespace o2 = formats::o2;
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const O2Flow flow(rom);
    const std::vector<std::uint32_t> entries = {o2::kBase};
    const iw::Walked<di::Insn> walked = iw::walk<di::Insn>(flow, entries);
    const std::map<std::uint32_t, di::Insn>& code = walked.code;
    iw::Names names;
    names.try_emplace(o2::kBase, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return flow.inside(a) && !inside_insn.contains(a); };
    const iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;

    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        // (the description's text, a jump or call target written as its label, kept when it builds back into the instruction's bytes)
        std::optional<std::string> t = described_source::checked_text(i8048_cpu(), insn.bytes(), at, insn.size);
        if (t) {
            if (insn.target && address_labels.contains(*insn.target)) *t = described_source::name_number(*t, *insn.target, address_labels.at(*insn.target), false);
            instruction_text[at] = *t;
        } else {
            instruction_text[at] = described_source::byte_line(i8048_cpu(), insn.bytes(), at, {}, "");
        }
    }

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Odyssey 2 cartridge, {} bytes at $0400 (the 8048's program memory)", rom.data.size()), ""},
                                "sleigh-i8048", std::string(file_name));
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:04x}", o2::kBase));
    O2Listing listing(rom, code, instruction_text);
    iw::emit<di::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            return described_source::assemble(i8048_cpu(), source, folder);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) { return write(formats::o2::read(rom_file), rom_file.filename().string(), folder); }

}  // namespace cb::source::o2_writer
