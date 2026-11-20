// cb-unity: alone
// source/ps3_writer.cpp -- a PlayStation 3 program as assembly source that builds back into it (ps3_writer.hpp).

#include "source/ps3_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "build/mips_asm.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/ppc.hpp"
#include "platforms/decoder.hpp"
#include "source/isa_ppc.hpp"

namespace cb::source::ps3_writer {

namespace dp = decompile::ppc;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
constexpr std::uint16_t kVector = 0xFFFF;   // `Insn::row` of a word the table does not say that is still an instruction (VMX): its text is the disassembler's

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}
std::uint64_t be64(ByteView b, std::size_t at) { return static_cast<std::uint64_t>(be32(b, at)) << 32 | be32(b, at + 4); }

bool ends_flow(const dp::Insn& insn) { return insn.flow == dp::Flow::Jump || insn.flow == dp::Flow::Return || insn.flow == dp::Flow::Indirect; }

// Flow runs through the code segments, four-byte instructions, an address being an image offset.
class Ps3Flow final : public iw::CodeModel<dp::Insn> {
public:
    Ps3Flow(ByteView image, const formats::ps3::Program& program) : image_(image), program_(program) {}

    std::optional<dp::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        if (auto insn = dp::decode(image_, pc, pc, true)) return insn;
        const std::uint32_t word = be32(image_, pc);
        if ((word >> 26) == 4) {   // the vector unit's instructions are decoded for their length and text only
            dp::Insn v;
            v.word = word;
            v.pc = pc;
            v.row = kVector;
            v.mnemonic = "vmx";
            return v;
        }
        return std::nullopt;
    }
    std::uint32_t size(const dp::Insn&) const override { return 4; }
    std::optional<std::uint32_t> target(const dp::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 4 == 0 && pc + 4 <= image_.size() && program_.in_code(pc); }
    bool fits(std::uint32_t pc, const dp::Insn&) const override { return pc + 4 <= image_.size(); }
    bool follows(const dp::Insn& insn) const override { return insn.target && inside(*insn.target) && insn.flow != dp::Flow::None; }
    bool stops(const dp::Insn& insn) const override { return ends_flow(insn); }
    bool finds_functions() const override { return true; }
    bool starts_function(const dp::Insn& insn) const override { return insn.flow == dp::Flow::Call && insn.target; }

private:
    ByteView image_;
    const formats::ps3::Program& program_;
};

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

class Ps3Listing final : public iw::Listing<dp::Insn> {
public:
    Ps3Listing(translate::l2::Assembly& assembly, ByteView image, const std::map<std::uint32_t, dp::Insn>& code, const std::map<std::uint32_t, std::string>& text,
               const iw::Names& labels, const std::map<std::uint32_t, std::string>& words)
        : assembly_(assembly), image_(image), code_(code), text_(text), labels_(labels), words_(words) {}

    std::uint8_t byte(std::uint32_t offset) const override { return image_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const dp::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const dp::Insn&) const override { return 4; }
    std::string text(const dp::Insn& insn) const override { return text_.at(insn.pc); }
    std::vector<std::string> data_lines(ByteView data) const override {
        std::vector<std::string> lines;
        std::size_t i = 0;
        while (i < data.size()) {
            std::size_t run = 1;
            while (i + run < data.size() && data[i + run] == data[i]) ++run;
            if (run >= 16) {
                lines.push_back(std::format("        .fill {}, 1, 0x{:02X}", run, data[i]));
                i += run;
                continue;
            }
            std::size_t text_end = i;
            while (text_end < data.size() && printable(data[text_end])) ++text_end;
            if (text_end - i >= 6) {
                const bool zero = text_end < data.size() && data[text_end] == 0;
                lines.push_back(std::format("        {} \"{}\"", zero ? ".asciz" : ".ascii", std::string(reinterpret_cast<const char*>(data.data()) + i, text_end - i)));
                i = text_end + (zero ? 1 : 0);
                continue;
            }
            std::string row = "        .byte ";
            std::size_t n = 0;
            while (i < data.size() && n < 16) {
                std::size_t same = 1;
                while (i + same < data.size() && data[i + same] == data[i]) ++same;
                if (same >= 16 && n > 0) break;
                if (n != 0) row += ", ";
                row += std::format("0x{:02X}", data[i]);
                ++i;
                ++n;
            }
            lines.push_back(std::move(row));
        }
        return lines;
    }
    // A doubleword that is a pointer of the program (a function descriptor's code and TOC, a vtable entry) is its label plus a difference.
    std::uint32_t special(std::uint32_t offset, std::uint32_t address, const iw::Flush& flush) override {
        const auto it = words_.find(offset);
        if (it == words_.end() || code_.contains(address)) return 0;
        flush();
        if (const auto label = labels_.find(address); label != labels_.end()) assembly_.append(label->second + ":");
        assembly_.append("        .quad " + it->second);
        return 8;
    }

private:
    translate::l2::Assembly& assembly_;
    ByteView image_;
    const std::map<std::uint32_t, dp::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    const iw::Names& labels_;
    const std::map<std::uint32_t, std::string>& words_;
};

std::string signed_hex(std::int64_t v) { return v < 0 ? std::format("- 0x{:X}", -v) : std::format("+ 0x{:X}", v); }

std::string clean(std::string_view s) {
    std::string out;
    for (const char c : s) out += (std::isalnum(static_cast<unsigned char>(c)) || c == '_') ? c : '_';
    return out;
}

// The disassembler's text of a word the table does not say.
std::string vector_text(std::uint32_t word, std::uint32_t pc) {
    thread_local std::optional<platforms::Decoder> decoder;
    try {
        if (!decoder) decoder.emplace(platforms::Decoder::open(platforms::Arch::Ppc, 64, false));
        const Bytes bytes = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
        const auto insns = decoder->disasm(bytes, pc, 1);
        if (!insns.empty()) return insns.front().mnemonic + (insns.front().op_str.empty() ? std::string() : " " + insns.front().op_str);
    } catch (const Error&) {
    }
    return "vmx";
}

}  // namespace

iw::Written write(const formats::ps3::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const ByteView image = program.image;
    const Ps3Flow flow(image, program);

    // function descriptors: a doubleword that is the address of code followed by the program's TOC; the entry's descriptor and the imports' are among them
    std::set<std::uint32_t> entry_set;
    std::map<std::uint32_t, std::string> words;   // doubleword offset -> the operand of its `.quad`
    iw::Names names;
    std::map<std::uint32_t, std::uint64_t> pointer_value;
    if (const auto code = program.offset_of(program.entry_code); code && flow.inside(*code)) {
        entry_set.insert(*code);
        names.try_emplace(*code, "entry");
    }
    for (const formats::ps3::Segment& seg : program.segments) {
        if (seg.code()) continue;
        for (std::uint32_t at = seg.image_offset; at + 24 <= seg.image_offset + seg.size; at += 8) {
            const std::uint64_t code_address = be64(image, at), toc = be64(image, at + 8);
            if (toc != program.toc || program.toc == 0) continue;
            const auto code = program.offset_of(code_address);
            if (!code || !flow.inside(*code)) continue;
            entry_set.insert(*code);
            pointer_value[at] = code_address;
        }
    }
    const std::vector<std::uint32_t> entries(entry_set.begin(), entry_set.end());
    const iw::Walked<dp::Insn> walked = iw::walk<dp::Insn>(flow, entries);
    const std::map<std::uint32_t, dp::Insn>& code = walked.code;

    // the imports' descriptors are named by their function
    {
        std::set<std::string> used;
        const auto unique = [&](std::string n, std::uint32_t at) {
            if (!used.insert(n).second) n = std::format("{}_{:06X}", n, at);
            used.insert(n);
            return n;
        };
        for (const formats::ps3::Import& i : program.imports) {
            const auto at = program.offset_of(i.descriptor);
            if (at) names.try_emplace(*at, unique(i.name.empty() ? std::format("{}_{:08X}", clean(i.library), i.nid) : i.name, *at));
        }
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a < image.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    for (const auto& [at, name] : names)
        if (line_start(at)) labels.try_emplace(at, name);
    for (const auto& [at, value] : pointer_value) {
        const auto target = program.offset_of(value);
        if (!target || !line_start(*target)) continue;
        if (!labels.contains(*target)) labels.emplace(*target, kData(*target));
        const std::int64_t delta = static_cast<std::int64_t>(value) - static_cast<std::int64_t>(*target);
        words[at] = labels.at(*target) + (delta != 0 ? " " + signed_hex(delta) : std::string());
    }

    decompile::AddressLabels address_labels;
    decompile::NameLabels name_values;
    for (const auto& [a, n] : labels) {
        address_labels[a] = n;
        name_values[n] = a;
    }
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        if (insn.row == kVector) {
            instruction_text[at] = std::format(".long 0x{:08X}  # {}", insn.word, vector_text(insn.word, at));
            continue;
        }
        std::string t = dp::text(insn, address_labels);
        bool same = false;
        try {
            const Bytes built = dp::assemble(t, at, name_values, true);
            same = built.size() == 4 && be32(built, 0) == insn.word;
        } catch (const Error&) {
        }
        std::string line = same ? std::move(t) : std::format(".long 0x{:08X}  # {}", insn.word, t);
        if (insn.mnemonic == "sc") {   // the lv2 system call: the number is in r11
            for (std::uint32_t back = 4; back <= 16 && back <= at; back += 4) {
                const auto prior = code.find(at - back);
                if (prior == code.end() || prior->second.mnemonic != "li" || prior->second.rt() != 11) continue;
                const std::string name = formats::ps3::syscall_name(static_cast<std::uint32_t>(prior->second.simm()));
                if (!name.empty()) line += "  # lv2 " + name;
                break;
            }
        }
        instruction_text[at] = std::move(line);
    }

    std::vector<std::string> head = {std::format("# {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("# PlayStation 3 {}, image {} bytes in {} segments; the processor is the Cell's PPU (64-bit PowerPC, big-endian, VMX), entry at 0x{:X}, TOC 0x{:X}",
                                                 program.self ? "SELF" : "ELF", image.size(), program.segments.size(), program.entry_code, program.toc),
                                     "# addresses in this file are image offsets; the segments are laid end to end, each 16-byte aligned; a pointer is its target's label",
                                     ""};
    translate::l2::Assembly out(head, "gnu-as", std::string(file_name));
    out.part("image", "segment", std::nullopt, image.size());

    Ps3Listing listing(out, image, code, instruction_text, labels, words);
    iw::emit<dp::Insn>(out, 0, static_cast<std::uint32_t>(image.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), image.size());
    const build::mips_asm::Assembler assembler(isa::ppc64_encoder(), build::mips_asm::ByteOrder::Big);
    const Bytes original = program.to_elf(image);
    iw::assembled(r, [&] { return program.to_elf(assembler.build(source)); }, original, folder / (stem + ".built.elf"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::ps3::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::ps3_writer
