// cb-unity: alone
// source/console_cp1610.cpp -- the flat shape's CP1610 processor (console_executable.cpp): an Intellivision cartridge as CP1610 source that builds back into it
// (config/platforms/intv.json).
//
//     r.source           game.asm (as1600's syntax; source/intv_build reads it, with the SEGMENT / ROMTAIL / FORMAT directives it adds)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {word address: name}
//
// Every segment is `SEGMENT first,last` and `ORG`, its words code or `DECLE` data. Code is what flow reaches from the program's start (the third pointer of the header at $5000):
// branches, `J`, `JSR`, and an `MVII #address,R7`. An SDBD and the instruction it prefixes are two lines. The start is `start`, other functions `sub_<address>`, branch and jump
// targets `L_<address>`. An immediate that is the address of a line of a segment (`MVII #D_5200,R4`) is written as that label. A direct address that is a register of the STIC
// ($0000-$003F: the MOB registers, display enable, the colour stack, the border) or of the PSG ($01F0-$01FF, with the two controller ports) is written by its name, defined by an
// `EQU`. The words that are not code are `DECLE` rows and `FILL count,value` runs; the Intellicart's tables after the last segment are `ROMTAIL` bytes.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/intv.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "source/intv_build.hpp"
#include "core/fs.hpp"
#include "decompile/cp1610.hpp"

namespace cb::source::console_writer::cp1610_code {

namespace dc = decompile::cp1610;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};

class IntvFlow final : public iw::CodeModel<dc::Insn> {
public:
    explicit IntvFlow(const formats::intv::Rom& rom) : rom_(rom) {}

    std::optional<dc::Insn> decode(std::uint32_t pc) const override {
        const formats::intv::Segment* s = rom_.segment_at(pc);
        if (s == nullptr) return std::nullopt;
        std::optional<dc::Insn> insn = dc::decode(s->words, pc - s->start, pc, after_sdbd_.contains(pc));
        if (insn && insn->op == dc::Op::Sdbd) after_sdbd_.insert(pc + 1);
        return insn;
    }
    std::uint32_t size(const dc::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const dc::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return rom_.segment_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const dc::Insn& insn) const override {
        const formats::intv::Segment* s = rom_.segment_at(pc);
        return s != nullptr && pc + insn.size <= s->end();
    }
    bool follows(const dc::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == dc::Flow::Branch || insn.flow == dc::Flow::Jump || insn.flow == dc::Flow::Call);
    }
    bool stops(const dc::Insn& insn) const override {
        return insn.flow == dc::Flow::Jump || insn.flow == dc::Flow::Return || insn.flow == dc::Flow::Indirect || insn.flow == dc::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const dc::Insn& insn) const override { return insn.flow == dc::Flow::Call; }

private:
    const formats::intv::Rom& rom_;
    mutable std::set<std::uint32_t> after_sdbd_;   // the addresses that follow an SDBD the walk has decoded
};

std::string word_text(std::uint16_t v) { return std::format("${:04X}", v); }

}  // namespace

// The hardware's registers by address (config/knowledge/intv/registers.json).
const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/intv/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("intv", "hardware");
    return table;
}

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    const formats::intv::Rom& rom = contained_as<formats::intv::Rom>(loaded);
    fs::make_dirs(folder);
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));
    const IntvFlow flow(rom);
    std::vector<std::uint32_t> entries;
    iw::Names names;
    if (const auto e = rom.entry()) {
        entries.push_back(*e);
        names.try_emplace(*e, "start");
    }
    const iw::Walked<dc::Insn> walked = iw::walk<dc::Insn>(flow, entries);
    const std::map<std::uint32_t, dc::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return rom.segment_at(a) != nullptr && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    // an immediate that is the address of a line of the ROM is that line's label
    for (const auto& [at, insn] : code) {
        if (insn.access == dc::Access::Immediate && insn.value >= 0x4800 && line_start(insn.value)) labels.try_emplace(insn.value, kData(insn.value));
    }
    std::set<std::uint32_t> used;   // equates: the hardware registers the code names
    for (const auto& [at, insn] : code) {
        if (insn.access == dc::Access::Direct && hardware().contains(insn.value)) used.insert(insn.value);
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = hardware().at(a);
    decompile::NameLabels name_values;
    for (const auto& [a, n] : address_labels) name_values[n] = a;

    // each instruction as text, or as decles when the text would not assemble back to them
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        const bool label_immediate = insn.access == dc::Access::Immediate && insn.value >= 0x4800 && labels.contains(insn.value);
        std::string t = dc::text(insn, address_labels, label_immediate);
        bool same = false;
        try {
            const std::vector<std::uint16_t> built = intv_build::assemble_line(t, at, std::unordered_map<std::string, std::int64_t>(name_values.begin(), name_values.end()), insn.sdbd);
            same = built.size() == insn.size && std::equal(built.begin(), built.end(), insn.words.begin());
        } catch (const Error&) {
        }
        if (same) {
            instruction_text[at] = std::move(t);
        } else {
            std::string row = "DECLE ";
            for (std::size_t k = 0; k < insn.size; ++k) row += (k ? "," : "") + word_text(insn.words[k]);
            instruction_text[at] = row + "  ; " + dc::text(insn, address_labels);
        }
    }

    translate::l2::Assembly out(head_comment(description, values),
                                "cp1610", std::string(file_name));
    if (rom.raw) {
        out.append("        FORMAT RAW");
        out.append("");
    }
    for (const std::uint32_t a : used) out.append(std::format("{:<26} EQU ${:04X}", hardware().at(a), a));
    if (!used.empty()) out.append("");

    for (const formats::intv::Segment& seg : rom.segments) {
        out.part(std::format("segment_{:04X}", seg.start), "segment", std::nullopt, seg.words.size());
        out.append(std::format("        SEGMENT ${:02X},${:02X}", seg.start >> 8, (seg.end() - 1) >> 8));
        out.append(std::format("        ORG ${:04X}", seg.start));
        std::vector<std::uint16_t> data;
        const auto flush = [&] {
            std::size_t i = 0;
            while (i < data.size()) {
                std::size_t run = 1;
                while (i + run < data.size() && data[i + run] == data[i]) ++run;
                if (run >= 8) {
                    out.append(std::format("        FILL {},{}", run, word_text(data[i])));
                    i += run;
                    continue;
                }
                std::string row = "        DECLE ";
                std::size_t n = 0;
                while (i < data.size() && n < 8) {
                    std::size_t same = 1;
                    while (i + same < data.size() && data[i + same] == data[i]) ++same;
                    if (same >= 8 && n > 0) break;
                    row += (n ? "," : "") + word_text(data[i]);
                    ++i;
                    ++n;
                }
                out.append(row);
            }
            data.clear();
        };
        for (std::uint32_t a = seg.start; a < seg.end();) {
            if (const auto label = labels.find(a); label != labels.end()) {
                flush();
                if (const auto fn = functions.find(a); fn != functions.end()) {
                    out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), a});
                    out.append("");
                }
                out.append(label->second + ":");
            }
            if (const auto it = code.find(a); it != code.end() && a + it->second.size <= seg.end()) {
                flush();
                out.append("        " + instruction_text.at(a));
                a += it->second.size;
            } else {
                data.push_back(seg.words[a - seg.start]);
                if (labels.contains(a + 1)) flush();
                ++a;
            }
        }
        flush();
        out.append("");
    }
    if (!rom.tail.empty()) {
        out.append("; the Intellicart's tables (memory attributes) that follow the segments");
        for (std::size_t i = 0; i < rom.tail.size(); i += 16) {
            std::string row = "        ROMTAIL ";
            for (std::size_t k = i; k < std::min(rom.tail.size(), i + 16); ++k) row += (k > i ? "," : "") + std::format("${:02X}", rom.tail[k]);
            out.append(row);
        }
        out.append("");
    }

    const std::string source = iw::text(out) + "\n";
    std::size_t code_words = 0;
    for (const auto& [at, insn] : code) code_words += insn.size;
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, code_words, rom.word_count());
    iw::assembled(r, [&] { return intv_build::build(source); }, rom.serialize(false), folder / fill(description.executable.built, values));
    return r;
}


}  // namespace cb::source::console_writer::cp1610_code
