// cb-unity: alone
// source/n3ds_writer.cpp -- a Nintendo 3DS program as assembly source that builds back into it (n3ds_writer.hpp).

#include "source/n3ds_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <unordered_map>

#include "build/arm_asm.hpp"
#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "decompile/arm.hpp"
#include "source/isa.hpp"
#include "verify/stage_timer.hpp"

namespace cb::source::n3ds_writer {

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

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x1FF80000, "KERNEL_CONFIG_MEM"}, {0x1FF81000, "KERNEL_SHARED_PAGE"}, {0x1FF82000, "THREAD_LOCAL_STORAGE"}, {0x1EC00000, "IO_MEM_ARM11"},
        {0x1EF00000, "GPU_REGS"},          {0x1F000000, "VRAM"},               {0x14000000, "LINEAR_HEAP"},          {0x08000000, "APP_HEAP"}};
    return table;
}

std::string_view svc_name(std::uint32_t number) {
    static const char* const names[0x4C] = {
        "",                    "ControlMemory",      "QueryMemory",       "ExitProcess",        "GetProcessAffinityMask", "SetProcessAffinityMask", "GetProcessIdealProcessor",
        "SetProcessIdealProcessor", "CreateThread",  "ExitThread",        "SleepThread",        "GetThreadPriority",   "SetThreadPriority",    "GetThreadAffinityMask",
        "SetThreadAffinityMask",    "GetThreadIdealProcessor", "SetThreadIdealProcessor", "GetCurrentProcessorNumber", "Run", "CreateMutex", "ReleaseMutex",
        "CreateSemaphore",     "ReleaseSemaphore",   "CreateEvent",       "SignalEvent",        "ClearEvent",          "CreateTimer",          "SetTimer",
        "CancelTimer",         "ClearTimer",         "CreateMemoryBlock", "MapMemoryBlock",     "UnmapMemoryBlock",    "CreateAddressArbiter", "ArbitrateAddress",
        "CloseHandle",         "WaitSynchronization1", "WaitSynchronizationN", "SignalAndWait", "DuplicateHandle",     "GetSystemTick",        "GetHandleInfo",
        "GetSystemInfo",       "GetProcessInfo",     "GetThreadInfo",     "ConnectToPort",      "SendSyncRequest1",    "SendSyncRequest2",     "SendSyncRequest3",
        "SendSyncRequest4",    "SendSyncRequest",    "OpenProcess",       "OpenThread",         "GetProcessId",        "GetProcessIdOfThread", "GetThreadId",
        "GetResourceLimit",    "GetResourceLimitLimitValues", "GetResourceLimitCurrentValues", "GetThreadContext", "Break", "OutputDebugString", "ControlProcessMemory",
        "MapProcessMemory",    "UnmapProcessMemory", "CreateCodeSet",     "RandomStub",         "CreateProcess",       "TerminateProcess",     "SetProcessResourceLimits",
        "CreateResourceLimit", "SetResourceLimitValues", "AddCodeSegment", "Backdoor",          "KernelSetState"};
    return number < 0x4C ? std::string_view(names[number]) : std::string_view();
}

iw::Written write(const formats::n3ds::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
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
    decompile::AddressLabels address_labels;
    decompile::NameLabels name_values;
    for (const auto& [a, n] : labels) {
        address_labels[a] = n;
        name_values[n] = a;
    }
    for (const auto& [n, v] : equates) name_values[n] = v;
    // (each instruction on its own, from tables nothing changes: the lines are made side by side and filed in address order)
    std::map<std::uint32_t, std::string> instruction_text;
    std::vector<const std::pair<const std::uint32_t, da::Insn>*> to_write;
    to_write.reserve(code.size());
    for (const auto& entry : code) to_write.push_back(&entry);
    std::vector<std::string> written_lines(to_write.size());
    parallel::parallel_for(to_write.size(), [&](std::size_t item) {
        const std::uint32_t at = to_write[item]->first;
        const da::Insn& insn = to_write[item]->second;
        std::string t = da::text(insn, address_labels);
        bool same = false;
        try {
            const Bytes built = da::assemble(t, at, name_values, kArch);
            same = built.size() == 4 && std::equal(built.begin(), built.end(), insn.raw.begin());
        } catch (const Error&) {
        }
        std::string line = same ? std::move(t) : std::format(".word 0x{:08X}  @ {}", insn.word, t);
        if (insn.op == da::Op::Swi) {
            const std::string_view svc = svc_name(insn.imm);
            if (!svc.empty()) line += std::format("  @ svc{}", svc);
        }
        written_lines[item] = std::move(line);
    });
    for (std::size_t item = 0; item < to_write.size(); ++item) instruction_text.emplace_hint(instruction_text.end(), to_write[item]->first, std::move(written_lines[item]));

    std::vector<std::string> head = {std::format("@ {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("@ Nintendo 3DS {}, {} bytes, entry 0x{:06X}; the processor is the ARM11 (ARMv6K, VFP), loaded at 0x00100000",
                                                 program.kind == formats::n3ds::Kind::X3dsx ? "3DSX" : "ELF", file.size(), program.entry),
                                     "@ addresses in this file are file offsets; a pointer word is its target's label plus the difference the loader's layout makes",
                                     "",
                                     "        .syntax unified",
                                     "        .arm",
                                     ""};
    for (const auto& [name, value] : equates) head.push_back(std::format("        .equ {}, 0x{:08X}", name, value));
    if (!equates.empty()) head.push_back("");
    stage.emplace("n3ds.emit");
    translate::l2::Assembly out(head, "gnu-as", std::string(file_name));
    out.part("image", "segment", std::nullopt, file.size());

    N3dsListing listing(out, file, code, instruction_text, labels, words);
    iw::emit<da::Insn>(out, 0, static_cast<std::uint32_t>(file.size()), labels, functions, listing);
    out.append("");

    stage.emplace("n3ds.join");
    const std::string source = iw::text(out) + "\n";
    stage.emplace("n3ds.assemble");
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), file.size());
    const build::arm_asm::Assembler assembler(isa::arm11_encoder());
    const char* extension = program.kind == formats::n3ds::Kind::X3dsx ? ".built.3dsx" : ".built.elf";
    iw::assembled(r, [&] { return assembler.build(source); }, program.file, folder / (stem + extension));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::n3ds::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::n3ds_writer
