// cb-unity: alone
// source/o2_writer.cpp -- an Odyssey 2 cartridge as 8048 source that builds back into it (o2_writer.hpp).

#include "source/o2_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "build/i8048_asm.hpp"
#include "core/fs.hpp"
#include "decompile/i8048.hpp"

namespace cb::source::o2_writer {

namespace di = decompile::i8048;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
const iw::DataSyntax kSyntax{.bytes = "DB", .fill = "FILL", .fill_separator = ","};

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
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }

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
    decompile::NameLabels name_values;
    for (const auto& [a, n] : address_labels) name_values[n] = a;

    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        std::string t = di::text(insn, address_labels);
        bool same = false;
        try {
            const Bytes built = di::assemble(t, at, name_values);
            same = built.size() == insn.size && std::equal(built.begin(), built.end(), insn.raw.begin());
        } catch (const Error&) {
        }
        if (same) {
            instruction_text[at] = std::move(t);
        } else {
            std::string row = "DB ";
            for (std::size_t k = 0; k < insn.size; ++k) row += (k ? "," : "") + std::format("${:02X}", insn.raw[k]);
            instruction_text[at] = row + "  ; " + t;
        }
    }

    translate::l2::Assembly out({std::format("; {}.asm -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Odyssey 2 cartridge, {} bytes at $0400 (the 8048's program memory)", rom.data.size()), ""},
                                "i8048", std::string(file_name));
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        ORG ${:04X}", o2::kBase));
    O2Listing listing(rom, code, instruction_text);
    iw::emit<di::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            build::byte_asm::Image image = build::i8048_asm::build(source);
            if (image.base != o2::kBase) throw build::AsmError("the image starts at the wrong address");
            return std::move(image.data);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) { return write(formats::o2::read(rom_file), rom_file.filename().string(), folder); }

}  // namespace cb::source::o2_writer
