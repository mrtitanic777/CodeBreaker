// cb-unity: alone
// source/console_n3ds.cpp -- the executable shape's container `n3ds` (console_executable.cpp): a Nintendo 3DS program (a `.3dsx`, or the ARM ELF devkitARM links it from) as assembly source that builds back into it
// (config/platforms/n3ds.json).
//
//     r.source           app.s (GNU as syntax, `.syntax unified`, ARM state: what devkitARM's gcc makes of homebrew)
//     r.same             the source, built, is the file byte for byte
//     r.functions        {file offset: name}
//
// The file is written whole, in file order, an address being the byte's offset in the file (a 3DSX: its header, relocation headers, code, read-only data,
// data, the relocation tables and the icon and RomFS after them; an ELF: its headers, segments, symbols and debug information). Code is what flow reaches
// (decompile/arm, `Arch::V6K`: the ARMv6K integer set and the VFP) from the entry point, the symbols of an ELF, and the code a 3DSX's relocation table
// says is pointed at (a jump table's cases, a callback, a virtual function); the rest is data: strings as `.ascii`, runs as `.fill`, bytes. A word the
// relocation table patches (a literal-pool address, a vtable entry) is written as its target's label (`D_<offset>`, `sub_<offset>`) plus the difference
// between the loaded address and the file offset, so the source says what the pointer points at; an instruction whose text would not assemble back to its
// bits stays a `.word` with the text as a comment. A software interrupt is followed by a comment naming the Horizon kernel's call (`@ svcSendSyncRequest`).
//
// It builds with CodeBreaker's own assembler (the one assembler, ARMv6K). The rebuilt file is kept beside the source as <stem>.built.3dsx or .built.elf.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/n3ds.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "decompile/arm.hpp"
#include "source/isa.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"
#include "verify/stage_timer.hpp"

namespace cb::source::console_writer::n3ds_code {

namespace da = decompile::arm;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
constexpr da::Arch kArch = da::Arch::V6K;

bool ends_flow(const da::Insn& insn) { return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect; }

std::uint32_t word_at(ByteView b, std::size_t off) {
    return static_cast<std::uint32_t>(b[off]) | static_cast<std::uint32_t>(b[off + 1]) << 8 | static_cast<std::uint32_t>(b[off + 2]) << 16 |
           static_cast<std::uint32_t>(b[off + 3]) << 24;
}

// The address a PC-relative word load reads.
std::optional<std::uint32_t> literal_address(const da::Insn& i) {
    if (i.op != da::Op::Ldr || i.rn != 15 || !i.imm_form || !i.pre || i.wb || i.user || i.size != 4 || i.sign) return std::nullopt;
    const std::uint32_t base = i.pc + 8;
    return i.up ? base + i.imm : base - i.imm;
}

// Flow runs through the code segments in ARM state, an address being a file offset.
class N3dsFlow final : public iw::CodeModel<da::Insn> {
public:
    N3dsFlow(ByteView file, const formats::n3ds::Program& program) : file_(file), program_(program) {}

    std::optional<da::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        return da::decode(file_, pc, pc, kArch);
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::optional<std::uint32_t> target(const da::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 4 == 0 && pc + 4 <= file_.size() && program_.in_code(pc); }
    bool fits(std::uint32_t pc, const da::Insn&) const override { return pc + 4 <= file_.size(); }
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

    ByteView file_;
    const formats::n3ds::Program& program_;
};

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

class N3dsListing final : public iw::Listing<da::Insn> {
public:
    N3dsListing(translate::l2::Assembly& assembly, ByteView file, const std::map<std::uint32_t, da::Insn>& code, const std::map<std::uint32_t, std::string>& text,
                const iw::Names& labels, const std::map<std::uint32_t, std::string>& words)
        : assembly_(assembly), file_(file), code_(code), text_(text), labels_(labels), words_(words) {}

    std::uint8_t byte(std::uint32_t offset) const override { return file_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const da::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::string text(const da::Insn& insn) const override { return text_.at(insn.pc); }
    // Strings, runs of one byte as a fill, the rest as rows of bytes.
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
    // A word that is a pointer (or a system address) is written as the name and its offset.
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
    ByteView file_;
    const std::map<std::uint32_t, da::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    const iw::Names& labels_;
    const std::map<std::uint32_t, std::string>& words_;
};

std::string signed_hex(std::int64_t v) { return v < 0 ? std::format("- 0x{:X}", -v) : std::format("+ 0x{:X}", v); }

// The system addresses programs load into registers (the kernel's configuration page, the thread-local storage, the I/O and GPU register windows).
const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/n3ds/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("n3ds", "hardware");
    return table;
}

}  // namespace

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const formats::n3ds::Program& program = *loaded.contained.n3ds;
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));
    const ByteView file = program.file;
    const N3dsFlow flow(file, program);
    std::optional<verify::StageTimer> stage;   // (CB_PROFILE: where the writer's time goes)
    stage.emplace("n3ds.entries");

    // where flow starts: the entry, the symbols, and (a 3DSX) the code the relocation table says is pointed at
    std::set<std::uint32_t> entry_set = {program.entry};
    for (const auto& [at, name] : program.names)
        if (flow.inside(at)) entry_set.insert(at);
    std::map<std::uint32_t, std::uint32_t> pointer_value;   // word offset -> the value stored
    std::map<std::uint32_t, std::uint32_t> pointer_target;  // word offset -> file offset it points at
    for (const std::uint32_t w : program.pointers) {
        if (w + 4 > file.size()) continue;
        const std::uint32_t v = word_at(file, w);
        const std::optional<std::uint32_t> t = program.offset_of(v + formats::n3ds::kBase);
        if (!t) continue;
        pointer_value[w] = v;
        pointer_target[w] = *t;
        if (program.in_code(*t) && (v & 3) == 0) entry_set.insert(*t);
    }
    const std::vector<std::uint32_t> entries(entry_set.begin(), entry_set.end());
    stage.emplace("n3ds.walk");
    const iw::Walked<da::Insn> walked = iw::walk<da::Insn>(flow, entries);
    stage.emplace("n3ds.names");
    const std::map<std::uint32_t, da::Insn>& code = walked.code;

    iw::Names names;
    names.try_emplace(program.entry, "entry");
    for (const auto& [at, name] : program.names) {
        std::string clean;
        for (const char c : name) clean += (std::isalnum(static_cast<unsigned char>(c)) || c == '_') ? c : '_';
        if (!clean.empty() && !std::isdigit(static_cast<unsigned char>(clean[0]))) names.try_emplace(at, clean);
    }
    // two symbols with one name get their address
    {
        std::set<std::string> seen;
        for (auto& [at, name] : names)
            if (!seen.insert(name).second) name = std::format("{}_{:06X}", name, at);
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    stage.emplace("n3ds.labels");
    const iw::ByteMarks inside_insn = iw::covered_marks(code, flow, file.size());
    const auto line_start = [&](std::uint32_t a) { return a < file.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // the words the code loads: pointers of the program, or the system's addresses
    std::map<std::uint32_t, std::string> words;      // word offset -> the operand of its `.word`
    std::map<std::string, std::uint32_t> equates;
    const auto label_for = [&](std::uint32_t target) -> std::string {
        if (!line_start(target)) return {};
        const auto it = labels.find(target);
        if (it != labels.end()) return it->second;
        labels.emplace(target, kData(target));
        return kData(target);
    };
    for (const auto& [w, t] : pointer_target) {
        const std::string name = label_for(t);
        if (name.empty() || (w % 4) != 0) continue;
        const std::int64_t delta = static_cast<std::int64_t>(pointer_value[w]) - static_cast<std::int64_t>(t);
        words[w] = name + (delta != 0 ? " " + signed_hex(delta) : std::string());
    }
    for (const auto& [at, insn] : code) {
        const std::optional<std::uint32_t> lit = literal_address(insn);
        if (!lit || *lit + 4 > file.size() || code.contains(*lit) || (*lit % 4) != 0 || words.contains(*lit)) continue;
        labels.try_emplace(*lit, kData(*lit));
        const std::uint32_t value = word_at(file, *lit);
        if (const auto reg = hardware().find(value); reg != hardware().end()) {
            equates[reg->second] = value;
            words[*lit] = reg->second;
        } else if (program.kind == formats::n3ds::Kind::Elf) {
            if (const auto t = program.offset_of(value)) {
                const std::string name = label_for(*t);
                if (!name.empty()) {
                    const std::int64_t delta = static_cast<std::int64_t>(value) - static_cast<std::int64_t>(*t);
                    words[*lit] = name + (delta != 0 ? " " + signed_hex(delta) : std::string());
                }
            }
        }
    }

    stage.emplace("n3ds.text");
    // (each instruction on its own, from tables nothing changes: the lines are made side by side and filed in address order)
    std::map<std::uint32_t, std::string> instruction_text;
    std::vector<const std::pair<const std::uint32_t, da::Insn>*> to_write;
    to_write.reserve(code.size());
    for (const auto& entry : code) to_write.push_back(&entry);
    std::vector<std::string> written_lines(to_write.size());
    parallel::parallel_for(to_write.size(), [&](std::size_t item) {
        const std::uint32_t at = to_write[item]->first;
        const da::Insn& insn = to_write[item]->second;
        // the description's text with the labels of a branch and of the word a load reads put in, kept only when it assembles back to the instruction's bytes
        std::map<std::uint32_t, std::string> by_value;
        std::vector<std::uint32_t> operands;
        const auto name = [&](std::uint32_t a) {
            if (const auto named = labels.find(a); named != labels.end()) by_value[a] = named->second, operands.push_back(a);
        };
        if (insn.target) name(*insn.target);
        if (const auto word = literal_address(insn)) name(*word);
        std::string line = risc_source::code("arm11", insn.bytes(), at, by_value, operands);
        if (line.starts_with(".byte")) line = std::format(".word 0x{:08x}  ; {}", insn.word, dialect::find("gas-arm").instruction(risc_source::read_text("arm11", insn.bytes(), at).value_or("?")));
        if (insn.op == da::Op::Swi) {
            const std::string_view svc = formats::n3ds::svc_name(insn.imm);
            if (!svc.empty()) line += std::format("  ; svc{}", svc);
        }
        written_lines[item] = std::move(line);
    });
    for (std::size_t item = 0; item < to_write.size(); ++item) instruction_text.emplace_hint(instruction_text.end(), to_write[item]->first, std::move(written_lines[item]));

    std::vector<std::string> head = head_comment(description, values);
    head.insert(head.end(), description.executable.prologue.begin(), description.executable.prologue.end());
    for (const auto& [name, value] : equates) head.push_back(std::format("        .equ {}, 0x{:08X}", name, value));
    if (!equates.empty()) head.push_back("");
    stage.emplace("n3ds.emit");
    translate::l2::Assembly out(head, "gas", file_name);
    out.part("image", "segment", std::nullopt, file.size());

    N3dsListing listing(out, file, code, instruction_text, labels, words);
    iw::emit<da::Insn>(out, 0, static_cast<std::uint32_t>(file.size()), labels, functions, listing);
    out.append("");

    stage.emplace("n3ds.join");
    const std::string source = dialect::find("gas-arm").print(iw::text(out) + "\n");   // (said in GNU as's: config/dialects/gas-arm.json)
    stage.emplace("n3ds.assemble");
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), file.size());
    iw::assembled(r, [&] { return risc_source::assemble("arm11", dialect::find("gas-arm").parse(source), folder); }, program.file, folder / fill(description.executable.built, values));
    return r;
}

}  // namespace cb::source::console_writer::n3ds_code
