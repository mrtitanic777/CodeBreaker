// cb-unity: alone
// source/threedo_writer.cpp -- a 3DO program as assembly source that builds back into it (threedo_writer.hpp).

#include "source/threedo_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "build/arm_asm.hpp"
#include "core/fs.hpp"
#include "decompile/arm.hpp"
#include "source/isa.hpp"

namespace cb::source::threedo_writer {

namespace da = decompile::arm;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

bool ends_flow(const da::Insn& insn) { return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect; }

// The address a PC-relative word load reads.
std::optional<std::uint32_t> literal_address(const da::Insn& i) {
    if (i.op != da::Op::Ldr || i.rn != 15 || !i.imm_form || !i.pre || i.wb || i.user || i.size != 4 || i.sign) return std::nullopt;
    const std::uint32_t base = i.pc + 8;
    return i.up ? base + i.imm : base - i.imm;
}

std::uint32_t word_at(ByteView b, std::size_t off) {
    return static_cast<std::uint32_t>(b[off]) | static_cast<std::uint32_t>(b[off + 1]) << 8 | static_cast<std::uint32_t>(b[off + 2]) << 16 |
           static_cast<std::uint32_t>(b[off + 3]) << 24;
}

// Flow runs through the image in ARM state.
class ThreeDoFlow final : public iw::CodeModel<da::Insn> {
public:
    explicit ThreeDoFlow(ByteView native) : native_(native) {}

    std::optional<da::Insn> decode(std::uint32_t pc) const override {
        if (pc % 4 != 0 || pc + 4 > native_.size()) return std::nullopt;
        return da::decode(native_, pc, pc);
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::optional<std::uint32_t> target(const da::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 4 == 0 && pc + 4 <= native_.size(); }
    bool fits(std::uint32_t pc, const da::Insn&) const override { return pc + 4 <= native_.size(); }
    bool follows(const da::Insn& insn) const override {
        return insn.target && (*insn.target & 1) == 0 && inside(*insn.target) && insn.flow != da::Flow::None && insn.op != da::Op::Swi;
    }
    // `mov lr, pc` before `mov pc, rN`: a call through a register, which comes back to the next instruction.
    bool stops(const da::Insn& insn) const override { return ends_flow(insn) && !indirect_call(insn); }
    bool finds_functions() const override { return true; }
    bool starts_function(const da::Insn& insn) const override { return insn.op == da::Op::Bl; }

private:
    bool indirect_call(const da::Insn& i) const {
        if (i.flow != da::Flow::Indirect || i.pc < 4) return false;
        const std::optional<da::Insn> before = decode(i.pc - 4);
        return before && before->op == da::Op::Mov && before->rd == 14 && !before->imm_form && before->rm == 15 && before->cond == i.cond && !before->shift_by_reg &&
               before->shift == da::Shift::Lsl && before->amount == 0 && !before->rrx;
    }

    ByteView native_;
};

class ThreeDoListing final : public iw::Listing<da::Insn> {
public:
    ThreeDoListing(translate::l2::Assembly& assembly, ByteView native, const std::map<std::uint32_t, da::Insn>& code, const std::map<std::uint32_t, std::string>& text,
                   const iw::Names& labels, const std::map<std::uint32_t, std::string>& register_words)
        : assembly_(assembly), native_(native), code_(code), text_(text), labels_(labels), register_words_(register_words) {}

    std::uint8_t byte(std::uint32_t offset) const override { return native_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const da::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::string text(const da::Insn& insn) const override { return text_.at(insn.pc); }
    // Words, with runs of one word as a fill.
    std::vector<std::string> data_lines(ByteView data) const override {
        std::vector<std::string> lines;
        std::size_t i = 0;
        while (i + 4 <= data.size()) {
            std::size_t run = 1;
            const std::uint32_t w = word_at(data, i);
            while (i + 4 * (run + 1) <= data.size() && word_at(data, i + 4 * run) == w) ++run;
            if (run >= 8) {
                lines.push_back(std::format("        .fill {}, 4, 0x{:08X}", run, w));
                i += 4 * run;
                continue;
            }
            std::string row = "        .word ";
            for (std::size_t n = 0; n < 4 && i + 4 <= data.size(); ++n, i += 4) {
                if (n != 0) row += ", ";
                row += std::format("0x{:08X}", word_at(data, i));
            }
            lines.push_back(std::move(row));
        }
        return lines;
    }
    // A literal word that is a register's address is written as the register (with its own label line, which this takes the place of).
    std::uint32_t special(std::uint32_t, std::uint32_t address, const iw::Flush& flush) override {
        const auto it = register_words_.find(address);
        if (it == register_words_.end()) return 0;
        flush();
        if (const auto label = labels_.find(address); label != labels_.end()) assembly_.append(label->second + ":");
        assembly_.append("        .word " + it->second);
        return 4;
    }

private:
    translate::l2::Assembly& assembly_;
    ByteView native_;
    const std::map<std::uint32_t, da::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    const iw::Names& labels_;
    const std::map<std::uint32_t, std::string>& register_words_;
};

// The disc (or bare AIF) with the image's words put back from what was built (the words reversed again).
Bytes rebuilt_file(const formats::threedo::Program& program, const Bytes& built) {
    Bytes out = program.file;
    for (std::size_t i = 0; i < program.image_size; ++i) {
        const std::size_t word = i & ~std::size_t{3};
        const std::size_t from = word + 3 - (i & 3);
        out[program.image_offset + i] = from < built.size() ? built[from] : 0;
    }
    return out;
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/threedo/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("threedo", "hardware");
    return table;
}

iw::Written write(const formats::threedo::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const bool disc = program.kind == formats::threedo::Kind::Disc;
    const Bytes native = program.native();
    const ThreeDoFlow flow(native);
    const std::vector<std::uint32_t> entries = {program.aif.entry};
    const iw::Walked<da::Insn> walked = iw::walk<da::Insn>(flow, entries);
    const std::map<std::uint32_t, da::Insn>& code = walked.code;

    iw::Names names;
    names.try_emplace(program.aif.entry, "entry");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a % 4 == 0 && a < native.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::map<std::string, std::uint32_t> equates;   // register names used by a literal word
    std::map<std::uint32_t, std::string> literal_name;
    for (const auto& [at, insn] : code) {
        const std::optional<std::uint32_t> lit = literal_address(insn);
        if (!lit || !line_start(*lit) || code.contains(*lit)) continue;
        labels.try_emplace(*lit, kData(*lit));
        const std::uint32_t value = word_at(native, *lit);
        if (const auto reg = hardware().find(value); reg != hardware().end()) {
            equates[reg->second] = value;
            literal_name[*lit] = reg->second;
        }
    }

    decompile::AddressLabels address_labels;
    decompile::NameLabels name_values;
    for (const auto& [a, n] : labels) {
        address_labels[a] = n;
        name_values[n] = a;
    }
    for (const auto& [n, v] : equates) name_values[n] = v;
    std::map<std::uint32_t, std::string> instruction_text;   // each instruction as text, or its word with the text as a comment when it would not assemble back
    for (const auto& [at, insn] : code) {
        std::string t = da::text(insn, address_labels);
        bool same = false;
        try {
            const Bytes built = da::assemble(t, at, name_values);
            same = built.size() == 4 && std::equal(built.begin(), built.end(), insn.raw.begin());
        } catch (const Error&) {
        }
        instruction_text[at] = same ? std::move(t) : std::format(".word 0x{:08X}  @ {}", insn.word, t);
    }

    std::vector<std::string> head = {std::format("@ {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("@ 3DO {} {}, {} bytes, entry 0x{:06X}; every word is its value (the processor is big-endian)", disc ? "disc, program" : "AIF",
                                                 program.name, program.image_size, program.aif.entry),
                                     "",
                                     "        .syntax unified",
                                     "        .arm",
                                     ""};
    for (const auto& [name, value] : equates) head.push_back(std::format("        .equ {}, 0x{:08X}", name, value));
    if (!equates.empty()) head.push_back("");
    translate::l2::Assembly out(head, "gnu-as", std::string(file_name));
    out.part("image", "segment", std::nullopt, native.size());

    ThreeDoListing listing(out, native, code, instruction_text, labels, literal_name);
    iw::emit<da::Insn>(out, 0, static_cast<std::uint32_t>(native.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), program.image_size);
    const build::arm_asm::Assembler assembler(isa::arm_encoder());
    iw::assembled(r, [&] { return rebuilt_file(program, assembler.build(source)); }, program.file, folder / (stem + (disc ? ".built.iso" : ".built.aif")));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::threedo::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::threedo_writer
