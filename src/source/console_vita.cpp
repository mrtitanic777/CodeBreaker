// cb-unity: alone
// source/console_vita.cpp -- the executable shape's container `vita` (console_executable.cpp): a PlayStation Vita homebrew program (an unencrypted SELF such as a `.vpk`'s eboot.bin, or the ELF inside it) as assembly source that builds back into it
// (config/platforms/vita.json).
//
//     r.source           eboot.s (GNU as syntax, `.syntax unified`, `.thumb`: what VitaSDK's gcc makes of homebrew)
//     r.same             the source, built, is the program's image byte for byte (the loadable segments inflated and laid end to end; compared as the plain ELF
//                        formats::vita::Program::to_elf makes of it, which is also what the lane keeps as `<stem>.built.velf` and runs beside the original)
//     r.functions        {image offset: name}
//
// The image is written whole, an address being the byte's offset in the image. Code is what flow reaches in Thumb state (decompile/arm, `Arch::V7A`) from the module's
// start and stop functions and from what they call; an instruction inside an IT block is written as it decodes, with a comment naming the condition that applies;
// the instructions the tables do not say (most of Thumb-2's 32-bit integer set and NEON) are written as `.word` with the disassembler's text as a comment, so the
// source is the program whatever the analysis guessed. An imported function's stub is named from its NID (`sceCtrlPeekBufferPositive`, or `<library>_<NID>`), so a
// call reads `bl sceCtrlPeekBufferPositive`; a pointer-sized literal that holds an address of the program is written as its label.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/vita.hpp"

#include <algorithm>
#include <bit>
#include <cctype>
#include <format>
#include <set>
#include <unordered_set>


#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/arm.hpp"
#include "decompile/arm_ext.hpp"
#include "source/isa.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer::vita_code {

namespace da = decompile::arm;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
constexpr da::Arch kArch = da::Arch::V7A;
constexpr unsigned kRounds = 0;   // how many times the code found may add functions through pushes in the gaps (off: the scan found too much data)

std::uint32_t word_at(ByteView b, std::size_t off) {
    return static_cast<std::uint32_t>(b[off]) | static_cast<std::uint32_t>(b[off + 1]) << 8 | static_cast<std::uint32_t>(b[off + 2]) << 16 |
           static_cast<std::uint32_t>(b[off + 3]) << 24;
}

bool ends_flow(const da::Insn& insn) { return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect; }

// The address a PC-relative word load reads (Thumb: the word-aligned pc plus 4, plus the offset).
std::optional<std::uint32_t> literal_address(const da::Insn& i) {
    if (i.op != da::Op::Ldr || i.rn != 15 || !i.imm_form || !i.pre || i.wb || i.user || i.size != 4 || i.sign || !i.up) return std::nullopt;
    return ((i.pc + 4) & ~3u) + i.imm;
}

struct It {
    unsigned first, mask, index;
};

// Flow runs through the code segments in Thumb state, an address being an image offset (even).
class VitaFlow final : public iw::CodeModel<da::Insn> {
public:
    VitaFlow(ByteView image, const formats::vita::Program& program) : image_(image), program_(program) {}

    std::optional<da::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        return da::decode(image_, pc, pc | 1, kArch);
    }
    std::uint32_t size(const da::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const da::Insn& insn) const override {
        if (!insn.target || (*insn.target & 1) == 0) return std::nullopt;   // (a call into ARM state is not followed)
        return *insn.target & ~1u;
    }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && pc + 2 <= image_.size() && program_.in_code(pc); }
    bool fits(std::uint32_t pc, const da::Insn& insn) const override { return pc + insn.length <= image_.size(); }
    bool follows(const da::Insn& insn) const override {
        const auto to = target(insn);
        return to && inside(*to) && insn.flow != da::Flow::None && insn.op != da::Op::Swi;
    }
    // A jump, return or indirect branch ends the flow unless an IT block makes it conditional.
    bool stops(const da::Insn& insn) const override { return ends_flow(insn) && !it_at(insn.pc); }
    bool finds_functions() const override { return true; }
    bool starts_function(const da::Insn& insn) const override { return insn.op == da::Op::Bl; }

    // The IT block that covers the instruction at `pc`, looking back over the instructions before it.
    std::optional<It> it_at(std::uint32_t pc) const {
        for (const std::uint32_t back : {2u, 4u, 6u, 8u}) {
            if (pc < back) break;
            const std::uint32_t q = pc - back;
            if (q + 2 > image_.size()) continue;
            const unsigned a = image_[q] | image_[q + 1] << 8;
            if ((a & 0xFF00) != 0xBF00 || (a & 15) == 0) continue;
            const unsigned mask = a & 15, first = (a >> 4) & 15, count = 4 - static_cast<unsigned>(std::countr_zero(mask));
            std::uint32_t at = q + 2;
            for (unsigned k = 0; k < count; ++k) {
                if (at == pc) return It{first, mask, k};
                if (at > pc) break;
                const auto in = decode(at);
                if (!in) break;
                at += in->length;
            }
        }
        return std::nullopt;
    }

private:
    ByteView image_;
    const formats::vita::Program& program_;
};

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

class VitaListing final : public iw::Listing<da::Insn> {
public:
    VitaListing(translate::l2::Assembly& assembly, ByteView image, const std::map<std::uint32_t, da::Insn>& code, const std::map<std::uint32_t, std::string>& text,
                const iw::Names& labels, const std::map<std::uint32_t, std::string>& words)
        : assembly_(assembly), image_(image), code_(code), text_(text), labels_(labels), words_(words) {}

    std::uint8_t byte(std::uint32_t offset) const override { return image_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const da::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const da::Insn& insn) const override { return insn.length; }
    std::string text(const da::Insn& insn) const override { return text_.at(insn.pc); }
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
    std::uint32_t special(std::uint32_t offset, std::uint32_t address, const iw::Flush& flush) override {
        const auto it = words_.find(offset);
        if (it == words_.end() || code_.contains(address)) return 0;
        flush();
        if (const auto label = labels_.find(address); label != labels_.end()) assembly_.append(label->second + ":");
        assembly_.append("        .word " + it->second);
        return 4;
    }

private:
    translate::l2::Assembly& assembly_;
    ByteView image_;
    const std::map<std::uint32_t, da::Insn>& code_;
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

}  // namespace

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const formats::vita::Program& program = *loaded.contained.vita;
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));
    const ByteView image = program.image;
    const VitaFlow flow(image, program);

    // where flow starts: the module's start and stop functions (Thumb)
    std::set<std::uint32_t> entry_set;
    for (const std::uint32_t e : program.entries) {
        if ((e & 1) == 0) continue;
        if (const auto at = program.offset_of(e & ~1u); at && flow.inside(*at)) entry_set.insert(*at);
    }
    if (entry_set.empty())
        for (const formats::vita::Segment& s : program.segments)
            if (s.code()) {
                entry_set.insert(s.image_offset);
                break;
            }
    std::vector<std::uint32_t> entries(entry_set.begin(), entry_set.end());
    iw::Walked<da::Insn> walked = iw::walk<da::Insn>(flow, entries);
    // functions only called through a pointer: a word-aligned `push {..., lr}` that the flow did not reach, in the gaps between the code found (a few rounds, each
    // round's functions calling more)
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::set<std::uint32_t> inside_now = iw::covered(walked.code, flow);
        std::size_t before = entries.size();
        for (const formats::vita::Segment& seg : program.segments) {
            if (!seg.code()) continue;
            for (std::uint32_t at = seg.image_offset; at + 4 <= seg.image_offset + seg.size; at += 4) {
                if (walked.code.contains(at) || inside_now.contains(at)) continue;
                const unsigned first = image[at] | image[at + 1] << 8;
                const bool push = (first & 0xFF00) == 0xB500 || (first == 0xE92D && (image[at + 3] & 0x40) != 0);
                if (!push) continue;
                // it must read as code for a few instructions
                std::uint32_t pc = at;
                unsigned good = 0;
                while (good < 6) {
                    const auto in = flow.decode(pc);
                    if (!in || (in->op == da::Op::Ext && in->ext == da::t2::kOpaque)) break;
                    pc += in->length;
                    ++good;
                }
                if (good >= 3) entries.push_back(at);
            }
        }
        if (entries.size() == before) break;
        walked = iw::walk<da::Insn>(flow, entries);
    }
    const std::map<std::uint32_t, da::Insn>& code = walked.code;

    iw::Names names;
    {
        std::set<std::string> used;
        const auto unique = [&](std::string n, std::uint32_t at) {
            if (!used.insert(n).second) n = std::format("{}_{:06X}", n, at);
            used.insert(n);
            return n;
        };
        if (!program.entries.empty())
            if (const auto at = program.offset_of(program.entries.front() & ~1u)) names.try_emplace(*at, unique("module_start", *at));
        for (const formats::vita::Import& i : program.imports) {
            const auto at = program.offset_of(i.stub);
            if (!at) continue;
            names.try_emplace(*at, unique(i.name.empty() ? std::format("{}_{:08X}", clean(i.library), i.nid) : i.name, *at));
        }
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a < image.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    for (const auto& [at, name] : names) {
        if (line_start(at)) labels.try_emplace(at, name);
    }

    // the words the code loads: a pointer of the program is its label plus the difference between the loaded address and the offset
    std::map<std::uint32_t, std::string> words;
    for (const auto& [at, insn] : code) {
        const std::optional<std::uint32_t> lit = literal_address(insn);
        if (!lit || *lit + 4 > image.size() || (*lit % 2) != 0 || code.contains(*lit) || words.contains(*lit)) continue;
        if (!labels.contains(*lit)) labels.emplace(*lit, kData(*lit));
        const std::uint32_t value = word_at(image, *lit);
        if (const auto target = program.offset_of(value & ~1u); target && line_start(*target)) {
            if (!labels.contains(*target)) labels.emplace(*target, kData(*target));
            const std::int64_t delta = static_cast<std::int64_t>(value) - static_cast<std::int64_t>(*target);
            words[*lit] = labels.at(*target) + (delta != 0 ? " " + signed_hex(delta) : std::string());
        }
    }

    // the description's text (Thumb-2: the ARMv7 description in Thumb state), a branch by its label and a PC-relative load by the label of the word it reads,
    // kept only when it assembles back to the instruction's bytes
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        std::map<std::uint32_t, std::string> by_value;
        std::vector<std::uint32_t> operands;
        const auto name = [&](std::uint32_t a) {
            if (const auto named = labels.find(a); named != labels.end()) by_value[a] = named->second, operands.push_back(a);
        };
        if (insn.target) name(*insn.target & ~1u);
        if (const auto word = literal_address(insn)) name(*word);
        std::string line = risc_source::code("armv7t", insn.bytes(), insn.pc, by_value, operands);
        if (line.starts_with(".byte"))   // (an instruction the description does not write as it assembles: its bits, as halves, with its text)
            line = insn.length == 2 ? std::format(".half 0x{:04X}  ; {}", insn.word & 0xFFFF, dialect::find("gas-arm").instruction(risc_source::read_text("armv7t", insn.bytes(), insn.pc).value_or("?")))
                                    : std::format(".half 0x{:04X}, 0x{:04X}  ; {}", insn.raw[0] | insn.raw[1] << 8, insn.raw[2] | insn.raw[3] << 8,
                                                  dialect::find("gas-arm").instruction(risc_source::read_text("armv7t", insn.bytes(), insn.pc).value_or("?")));
        if (const auto it = flow.it_at(at)) line += std::format("  ; if {}", da::condition_suffix(da::t2::it_condition(static_cast<std::uint8_t>(it->first), static_cast<std::uint8_t>(it->mask), it->index)));
        instruction_text[at] = std::move(line);
    }

    std::vector<std::string> head = head_comment(description, values);
    head.insert(head.end(), description.executable.prologue.begin(), description.executable.prologue.end());
    translate::l2::Assembly out(head, "gas", file_name);
    out.part("image", "segment", std::nullopt, image.size());

    VitaListing listing(out, image, code, instruction_text, labels, words);
    iw::emit<da::Insn>(out, 0, static_cast<std::uint32_t>(image.size()), labels, functions, listing);
    out.append("");

    const std::string source = dialect::find("gas-arm").print(iw::text(out) + "\n");   // (said in GNU as's: config/dialects/gas-arm.json)
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), image.size());
    const Bytes original = program.to_elf(image);
    iw::assembled(r, [&] { return program.to_elf(risc_source::assemble("armv7", dialect::find("gas-arm").parse(source), folder)); }, original, folder / fill(description.executable.built, values));
    return r;
}

}  // namespace cb::source::console_writer::vita_code
