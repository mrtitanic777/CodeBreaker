// cb-unity: alone
// cb/source/x64_writer.cpp -- a 64-bit x86 program file as NASM source that builds back into it (x64_writer.hpp).

#include "cb/source/x64_writer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <unordered_map>

#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/flat_rounds.hpp"
#include "cb/source/x86_flat.hpp"

namespace cb::source::x64_writer {

namespace iw = image_writer;
namespace xf = x86_flat;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

const platforms::Decoder& capstone64() {
    thread_local const platforms::Decoder decoder = platforms::Decoder::open(platforms::Arch::X86, 64, true);
    return decoder;
}

// The last word of a mnemonic Capstone gives with its prefixes ("notrack jmp", "bnd ret", "rep stosq").
std::string_view last_word(std::string_view m) {
    const std::size_t space = m.rfind(' ');
    return space == std::string_view::npos ? m : m.substr(space + 1);
}

bool stops_flow(std::string_view mnemonic) {
    const std::string_view m = last_word(mnemonic);
    return m == "jmp" || m == "ret" || m == "retf" || m == "retq" || m == "hlt" || m == "ud2" || m == "int3" || m == "iret" || m == "iretq" || m == "sysret" || m == "sysretq" || m == "ud1" || m == "ud0";
}

// The registers a register-to-register instruction names, as NASM spells them, by width: 3 is 64 bits, 1 is 32, 2 is 16, 0 is a byte.
constexpr std::array<std::string_view, 16> kRegs64 = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
constexpr std::array<std::string_view, 16> kRegs32 = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
constexpr std::array<std::string_view, 16> kRegs16 = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
constexpr std::array<std::string_view, 16> kRegs8 = {"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"};

bool is_general_register(std::string_view name) {
    for (const auto* set : {&kRegs64, &kRegs32, &kRegs16, &kRegs8}) {
        if (std::ranges::find(*set, name) != set->end()) return true;
    }
    return false;
}

// The opcode byte of the instruction: the first byte past the legacy prefixes and the REX prefix.
std::uint8_t opcode_of(const xf::Insn& i) {
    std::size_t k = 0;
    while (k < i.size && (i.bytes[k] == 0x66 || i.bytes[k] == 0x67 || i.bytes[k] == 0xF0 || i.bytes[k] == 0xF2 || i.bytes[k] == 0xF3 || xf::is_segment_prefix(i.bytes[k]))) ++k;
    if (k < i.size && (i.bytes[k] & 0xF0) == 0x40) ++k;
    return k < i.size ? i.bytes[k] : 0;
}

// The lines that define the register-to-register macros: `mov_ a, b` writes the form with the direction bit set (opcode reg, r/m), which NASM never chooses.
std::vector<std::string> prelude() {
    std::vector<std::string> p = {"", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m) that compilers write and NASM does not.",
                                  "; _x_<register> holds the register's number, its width (3: 64 bits, 1: 32, 2: 16, 0: 8) and, for spl bpl sil dil, a flag that wants a REX prefix."};
    const auto define = [&](const std::array<std::string_view, 16>& names, int width) {
        for (std::size_t k = 0; k < names.size(); ++k) {
            const bool byte_with_rex = width == 0 && k >= 4 && k < 8;
            p.push_back(std::format("%assign _x_{} 0x{:X}", names[k], (byte_with_rex ? 0x1000 : 0) | width << 8 | static_cast<int>(k)));
        }
    };
    define(kRegs64, 3);
    define(kRegs32, 1);
    define(kRegs16, 2);
    define(kRegs8, 0);
    for (const char* line : {"%macro _rr 3", "  %assign _a _x_%2", "  %assign _b _x_%3", "  %assign _w (_a >> 8) & 7", "  %assign _rex 0",
                             "  %if _w == 2", "    db 0x66", "  %endif", "  %if _w == 3", "    %assign _rex _rex | 8", "  %endif",
                             "  %if (_a & 8) != 0", "    %assign _rex _rex | 4", "  %endif", "  %if (_b & 8) != 0", "    %assign _rex _rex | 1", "  %endif",
                             "  %if _rex != 0 || ((_a | _b) & 0x1000) != 0", "    db 0x40 | _rex", "  %endif",
                             "  db (%1) | 2 | (_w > 0), 0xC0 | ((_a & 7) << 3) | (_b & 7)", "%endmacro"}) {
        p.emplace_back(line);
    }
    for (const xf::RegisterPairOp& op : xf::register_pair_ops()) p.push_back(std::format("%define {}_ _rr 0x{:02x},", op.mnemonic, op.opcode));
    return p;
}

// Where a [rip + disp] operand goes: the address after the instruction plus the displacement.
struct RipOperand {
    std::size_t begin = 0, end = 0;   // the bracket in the operand text
    std::uint32_t target = 0;
};

std::optional<RipOperand> rip_operand(const xf::Insn& i) {
    const std::string_view ops = i.op_str;
    const std::size_t begin = ops.find("[rip");
    if (begin == std::string_view::npos) return std::nullopt;
    const std::size_t close = ops.find(']', begin);
    if (close == std::string_view::npos) return std::nullopt;
    std::int64_t d = 0;
    if (close != begin + 4) {   // "[rip + 0x1234]" or "[rip - 0x10]"; "[rip]" is a displacement of nothing
        if (close < begin + 9 || ops[begin + 4] != ' ' || (ops[begin + 5] != '+' && ops[begin + 5] != '-') || ops[begin + 6] != ' ' || ops[begin + 7] != '0' || ops[begin + 8] != 'x') return std::nullopt;
        const auto disp = text::parse_int<std::uint64_t>(ops.substr(begin + 9, close - begin - 9), 16);
        if (!disp || *disp > 0x7FFFFFFFull) return std::nullopt;
        d = ops[begin + 5] == '-' ? -static_cast<std::int64_t>(*disp) : static_cast<std::int64_t>(*disp);
    }
    const std::int64_t target = static_cast<std::int64_t>(i.address) + i.size + d;
    if (target < 0 || target > 0xFFFFFFFFll) return std::nullopt;
    return RipOperand{begin, close + 1, static_cast<std::uint32_t>(target)};
}

// The operand text in NASM's spelling: Capstone's size words and prefixes, and the wide vector sizes.
std::string spell_operands(std::string_view operands) {
    std::string ops = xf::nasm_operands(operands);
    ops = text::replace_all(ops, "ymmword ", "yword ");
    ops = text::replace_all(ops, "zmmword ", "zword ");
    return ops;
}

// ---- how flow runs through the file ----------------------------------------------------------------------------------

class Flow final : public iw::CodeModel<xf::Insn> {
public:
    explicit Flow(const Image& image) : image_(image) {
        for (std::size_t k = 0; k < image.runs.size(); ++k) {
            if (image.runs[k].mapped) by_address_.push_back(k);
        }
        std::ranges::sort(by_address_, [&](std::size_t a, std::size_t b) { return image.runs[a].address < image.runs[b].address; });
    }

    // The run holding the address, mapped.
    const Run* run_at(std::uint32_t address) const {
        const auto it = std::ranges::upper_bound(by_address_, address, {}, [&](std::size_t k) { return image_.runs[k].address; });
        if (it == by_address_.begin()) return nullptr;
        const Run& r = image_.runs[*(it - 1)];
        return address - r.address < r.size ? &r : nullptr;
    }
    const Run* code_run_at(std::uint32_t address) const {
        const Run* r = run_at(address);
        return r && r->code ? r : nullptr;
    }

    std::optional<xf::Insn> decode(std::uint32_t pc) const override {
        const Run* r = code_run_at(pc);
        if (!r) return std::nullopt;
        const std::uint32_t at = r->offset + (pc - r->address);
        const std::size_t n = std::min<std::size_t>(15, r->offset + r->size - at);
        return xf::decode(capstone64(), ByteView(image_.file).subspan(at, n), pc);
    }
    std::uint32_t size(const xf::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const xf::Insn& insn) const override { return xf::branch_target(insn); }
    bool inside(std::uint32_t pc) const override { return code_run_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const xf::Insn& insn) const override {
        const Run* r = code_run_at(pc);
        return r && pc + insn.size <= r->address + r->size;
    }
    bool follows(const xf::Insn& insn) const override {
        const auto t = target(insn);
        return t && inside(*t);
    }
    bool stops(const xf::Insn& insn) const override { return stops_flow(insn.mnemonic); }
    bool finds_functions() const override { return true; }
    bool starts_function(const xf::Insn& insn) const override { return insn.mnemonic == "call"; }

private:
    const Image& image_;
    std::vector<std::size_t> by_address_;
};

// ---- the file as items ----------------------------------------------------------------------------------------------

enum class Kind { Code, Row, String, Zeros };

struct Item {
    std::uint32_t offset = 0, size = 0;
    Kind kind = Kind::Row;
    std::size_t run = 0;
    std::optional<xf::Insn> insn;
};

bool printable(std::uint8_t b) { return b >= 0x20 && b < 0x7F && b != '"'; }

class X64Image final : public flat_rounds::FlatImage<std::uint32_t> {
public:
    X64Image(const Image& image, const Flow& flow, const std::map<std::uint32_t, xf::Insn>& code, const iw::Names& labels, const iw::Functions& functions)
        : image_(image), flow_(flow), labels_(labels), functions_(functions) {
        // where a line may not continue: a label, an instruction, a comment, the end of a run
        std::set<std::uint32_t> stops;
        for (const Run& r : image.runs) stops.insert(r.offset);
        for (const auto& [a, name] : labels) {
            if (const Run* r = flow.run_at(a)) stops.insert(r->offset + (a - r->address));
        }
        for (const auto& [at, insn] : code) {
            if (const Run* r = flow.run_at(at)) stops.insert(r->offset + (at - r->address));
        }
        for (const auto& [off, text] : image.comments) stops.insert(off);
        const ByteView file(image.file);
        for (std::size_t k = 0; k < image.runs.size(); ++k) {
            const Run& r = image.runs[k];
            const std::uint32_t end = r.offset + r.size;
            std::uint32_t offset = r.offset;
            while (offset < end) {
                Item item;
                item.offset = offset;
                item.run = k;
                const auto here = r.mapped ? code.find(r.address + (offset - r.offset)) : code.end();
                if (here != code.end() && offset + here->second.size <= end) {
                    item.kind = Kind::Code;
                    item.insn = here->second;
                    item.size = here->second.size;
                } else {
                    const auto next_stop = stops.upper_bound(offset);
                    const std::uint32_t limit = next_stop == stops.end() ? end : std::min(*next_stop, end);
                    // zeros, then a string, then a row of at most 16 bytes that ends where either begins
                    std::uint32_t zeros = 0;
                    while (offset + zeros < limit && file[offset + zeros] == 0) ++zeros;
                    std::uint32_t chars = 0;
                    while (offset + chars < limit && printable(file[offset + chars])) ++chars;
                    if (zeros >= 16) {
                        item.kind = Kind::Zeros;
                        item.size = zeros;
                    } else if (chars >= 4) {
                        item.kind = Kind::String;
                        item.size = chars + (offset + chars < limit && file[offset + chars] == 0 ? 1 : 0);
                    } else {
                        item.kind = Kind::Row;
                        std::uint32_t n = 1;
                        while (n < 16 && offset + n < limit) {
                            std::uint32_t z = 0, c = 0;
                            while (z < 16 && offset + n + z < limit && file[offset + n + z] == 0) ++z;
                            while (c < 4 && offset + n + c < limit && printable(file[offset + n + c])) ++c;
                            if (z >= 16 || c >= 4) break;
                            ++n;
                        }
                        item.size = n;
                    }
                }
                items_.push_back(std::move(item));
                offset += items_.back().size;
            }
        }
    }

    std::string mark(std::size_t index) const { return std::format("m_{}", index); }

    flat_rounds::Source<std::uint32_t> write(const std::set<std::uint32_t>& raw, bool final) override {
        flat_rounds::Source<std::uint32_t> out;
        std::vector<std::pair<std::string, std::optional<std::uint32_t>>> lines;
        std::size_t run = SIZE_MAX;
        bool pairs = false;
        const ByteView file(image_.file);
        for (std::size_t k = 0; k < items_.size(); ++k) {
            const Item& item = items_[k];
            const Run& r = image_.runs[item.run];
            if (item.run != run) {
                run = item.run;
                lines.emplace_back("", std::nullopt);
                std::string header = std::format("section .s{} progbits align=1", run);
                if (r.mapped) header += std::format(" vstart=0x{:x}", r.address);
                lines.emplace_back(header, std::nullopt);
                if (r.mapped) lines.emplace_back(std::format("{}:", segment_label(r.address)), std::nullopt);
            }
            const std::uint32_t address = r.address + (item.offset - r.offset);
            if (const auto c = image_.comments.find(item.offset); c != image_.comments.end()) lines.emplace_back("; " + c->second, std::nullopt);
            if (r.mapped) {
                if (const auto fn = functions_.find(address); fn != functions_.end() && item.kind == Kind::Code) {
                    lines.emplace_back("", std::nullopt);
                    lines.emplace_back(fn->second + ":", std::nullopt);
                } else if (const auto l = labels_.find(address); l != labels_.end()) {
                    lines.emplace_back(l->second + ":", std::nullopt);
                }
            }
            if (!final && (item.kind == Kind::Code || (k > 0 && items_[k - 1].kind == Kind::Code))) lines.emplace_back(mark(k) + ":", std::nullopt);
            switch (item.kind) {
                case Kind::Code:
                    if (raw.contains(address)) {
                        lines.emplace_back("        db " + xf::db_bytes(file, item.offset, item.offset + item.size) + "  ; " + item.insn->mnemonic + " " + item.insn->op_str, std::nullopt);
                    } else {
                        lines.emplace_back("        " + text_of(*item.insn, pairs), address);
                    }
                    break;
                case Kind::Zeros: lines.emplace_back(std::format("        times {} db 0", item.size), std::nullopt); break;
                case Kind::String: {
                    const bool terminated = file[item.offset + item.size - 1] == 0;
                    const std::size_t chars = item.size - (terminated ? 1 : 0);
                    std::string s(reinterpret_cast<const char*>(file.data() + item.offset), chars);
                    lines.emplace_back("        db \"" + s + "\"" + (terminated ? ", 0" : ""), std::nullopt);
                    break;
                }
                case Kind::Row: lines.emplace_back("        db " + xf::db_bytes(file, item.offset, item.offset + item.size), std::nullopt); break;
            }
        }
        std::size_t line = 0;
        const auto add = [&](const std::string& text) {
            out.text += text + "\n";
            return ++line;
        };
        for (const std::string_view h : text::split(image_.head, '\n')) add(std::string(h));
        add("bits 64");
        add("default rel");
        if (pairs) {
            for (const std::string& p : prelude()) add(p);
        }
        for (const auto& [text, key] : lines) {
            const std::size_t n = add(text);
            if (key) out.instruction_at_line[n] = *key;
        }
        return out;
    }

    // An instruction came back different when the bytes at its mark are not its own, or the next mark is not where they end. One of another length moves what
    // follows it, so while any is of another length those alone are given and what is left is judged in the next round.
    std::set<std::uint32_t> differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<std::uint32_t>& raw) override {
        std::set<std::uint32_t> other_length, other_bytes, other_plain;
        const ByteView file(image_.file);
        std::vector<std::size_t> code;
        for (std::size_t k = 0; k < items_.size(); ++k) {
            if (items_[k].kind == Kind::Code) code.push_back(k);
        }
        for (std::size_t n = 0; n < code.size(); ++n) {
            const std::size_t k = code[n];
            const Item& item = items_[k];
            const Run& r = image_.runs[item.run];
            const std::uint32_t address = r.address + (item.offset - r.offset);
            if (raw.contains(address)) continue;
            const auto here = labels.find(mark(k));
            if (here == labels.end()) {
                other_length.insert(address);
                continue;
            }
            const std::uint64_t at = here->second.place;
            // the next mark, or the next item of any kind: an instruction ends where the following item begins
            std::uint64_t end = built.size();
            if (k + 1 < items_.size()) {
                const auto next = labels.find(mark(k + 1));   // (the item after an instruction is always marked)
                end = next != labels.end() ? next->second.place : at + item.size + 1;   // (no mark: the build lost it, so the length is wrong)
            }
            if (end - at != item.size || at + item.size > built.size()) {
                other_length.insert(address);
            } else if (!std::equal(file.begin() + item.offset, file.begin() + item.offset + item.size, built.begin() + static_cast<std::ptrdiff_t>(at))) {
                other_bytes.insert(address);
                // (one that names no address differs in its own bytes, whatever moved: it can be given now, with the ones of another length)
                if (!xf::branch_target(*item.insn) && !rip_operand(*item.insn)) other_plain.insert(address);
            }
        }
        if (other_length.empty()) return other_bytes;
        other_length.insert(other_plain.begin(), other_plain.end());
        return other_length;
    }

private:
    std::string segment_label(std::uint32_t address) const { return std::format("seg_{:x}", address); }

    // The operand that names `target`: a label, or the nearest run's label and the distance to it.
    std::string place_of(std::uint32_t target) const {
        if (const auto l = labels_.find(target); l != labels_.end()) return l->second;
        if (const Run* r = flow_.run_at(target)) {
            const std::int64_t d = static_cast<std::int64_t>(target) - r->address;
            return d == 0 ? segment_label(r->address) : std::format("{} + 0x{:x}", segment_label(r->address), d);
        }
        // past the file (memory the loader zeroes): relative to the last mapped run that starts below it
        const Run* below = nullptr;
        for (const Run& r : image_.runs) {
            if (r.mapped && r.address <= target && (!below || r.address > below->address)) below = &r;
        }
        if (!below) {
            for (const Run& r : image_.runs) {
                if (r.mapped) {
                    below = &r;
                    break;
                }
            }
        }
        if (!below) return std::format("0x{:x}", target);
        const std::int64_t d = static_cast<std::int64_t>(target) - below->address;
        return d >= 0 ? std::format("{} + 0x{:x}", segment_label(below->address), d) : std::format("{} - 0x{:x}", segment_label(below->address), -d);
    }

    // An instruction in NASM's spelling.
    std::string text_of(const xf::Insn& i, bool& pairs) const {
        std::string m = i.mnemonic;
        if (m == "movabs") m = "mov";
        if (xf::is_string_op(m, true)) return m;
        std::string ops = spell_operands(i.op_str);
        if (const std::optional<std::uint32_t> t = xf::branch_target(i)) {
            ops = place_of(*t);
            const std::string_view core = last_word(m);
            if (core == "jmp") m += i.size == 2 ? " short" : " near";
            else if (core.starts_with('j') && core != "jmp") m += i.size == 2 ? " short" : " near";
        } else if (const std::optional<RipOperand> rip = rip_operand(i)) {
            const std::string original = i.op_str;
            std::string spelled = original.substr(0, rip->begin) + "[rel " + place_of(rip->target) + "]" + original.substr(rip->end);
            ops = spell_operands(spelled);
        } else if (xf::has_register_pair_form(m)) {
            const std::size_t comma = i.op_str.find(", ");
            if (comma != std::string::npos) {
                const std::string_view a = std::string_view(i.op_str).substr(0, comma), b = std::string_view(i.op_str).substr(comma + 2);
                if (is_general_register(a) && is_general_register(b) && (opcode_of(i) & 2) != 0) {
                    pairs = true;
                    return std::format("{}_ {}", m, i.op_str);
                }
            }
        }
        return ops.empty() ? m : m + " " + ops;
    }

    const Image& image_;
    const Flow& flow_;
    const iw::Names& labels_;
    const iw::Functions& functions_;
    std::vector<Item> items_;
};

// A name NASM takes as a label: letters, digits and `_ . $ # @ ~ ?`, not starting with a digit, not an instruction's or a register's word.
bool nasm_label(const std::string& name) {
    if (name.empty() || !((name[0] >= 'a' && name[0] <= 'z') || (name[0] >= 'A' && name[0] <= 'Z') || name[0] == '_' || name[0] == '.' || name[0] == '?')) return false;   // (a label starts with a letter, `_`, `.` or `?`)
    for (const char c : name) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '$' || c == '#' || c == '@' || c == '~' || c == '?')) return false;
    }
    static const std::set<std::string> reserved = {"abs", "rel", "seg", "wrt", "near", "short", "far", "byte", "word", "dword", "qword", "tword", "oword", "yword", "zword", "ptr",
                                                   "db", "dw", "dd", "dq", "dt", "do", "dy", "dz", "equ", "times", "section", "segment", "global", "extern", "default", "bits", "org",
                                                   "mov", "add", "sub", "and", "or", "xor", "not", "neg", "mul", "div", "imul", "idiv", "inc", "dec", "push", "pop", "call", "ret", "jmp",
                                                   "lea", "cmp", "test", "nop", "int", "loop", "lock", "rep", "hlt", "in", "out", "bt", "bts", "btr", "shl", "shr", "sar", "rol", "ror",
                                                   "cmc", "clc", "stc", "cld", "std", "cli", "sti", "xchg", "cpuid", "syscall", "sysret", "leave", "enter", "bswap", "cwd", "cdq", "cqo"};
    std::string lower = name;
    std::ranges::transform(lower, lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return !reserved.contains(lower) && !is_general_register(lower) && !lower.starts_with("xmm") && !lower.starts_with("ymm") && !lower.starts_with("st");
}

}  // namespace

iw::Written write(const Image& image, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const Flow flow(image);
    std::vector<std::uint32_t> entries = image.entries;
    std::erase_if(entries, [&](std::uint32_t a) { return !flow.inside(a); });
    const iw::Walked<xf::Insn> walked = iw::walk<xf::Insn>(flow, entries);
    const std::map<std::uint32_t, xf::Insn>& code = walked.code;

    // names: known ones that NASM takes (a name seen twice keeps its first address), the first entry, then sub_<address>
    iw::Names names;
    std::set<std::string> taken;
    const auto give = [&](std::uint32_t address, const std::string& name) {
        if (names.contains(address)) return;
        std::string unique = name;
        if (!nasm_label(unique)) return;
        if (!taken.insert(unique).second) {
            unique = std::format("{}_{:x}", name, address);
            if (!taken.insert(unique).second) return;
        }
        names.emplace(address, unique);
    };
    for (const auto& [address, name] : image.functions) give(address, name);
    if (!entries.empty() && !names.contains(entries.front())) give(entries.front(), image.first_entry_name);
    for (const auto& [address, name] : image.data) give(address, name);
    // a function that was named but is not code stays a plain label; those that are called or entered are the functions
    std::vector<std::uint32_t> starts = walked.starts;
    iw::Names function_names = names;
    const iw::Functions functions = iw::name_functions(starts, function_names, kSub);
    names = function_names;

    // the places a [rip + disp] operand names, which want a label of their own
    const auto inside_instruction = [&](std::uint32_t a) {
        auto it = code.upper_bound(a);
        if (it == code.begin()) return false;
        --it;
        return a != it->first && a < it->first + it->second.size;
    };
    std::set<std::uint32_t> referenced;
    for (const auto& [at, insn] : code) {
        if (const auto rip = rip_operand(insn); rip && flow.run_at(rip->target) && !inside_instruction(rip->target)) referenced.insert(rip->target);
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return flow.run_at(a) != nullptr && !inside_instruction(a); }, kLocal);
    for (const std::uint32_t a : referenced) {
        if (labels.contains(a) || names.contains(a)) continue;
        labels.emplace(a, code.contains(a) ? kLocal(a) : kData(a));
    }
    for (const auto& [a, n] : functions) labels.try_emplace(a, n);
    for (const auto& [a, n] : names) {
        if (flow.run_at(a) && !inside_instruction(a)) labels.try_emplace(a, n);
    }

    X64Image source(image, flow, code, labels, functions);
    iw::Written r;
    r.functions = functions;
    r.image_bytes = image.file.size();
    for (const auto& [at, insn] : code) r.code_bytes += insn.size;
    flat_rounds::FlatOptions options;
    flat_rounds::FlatResult<std::uint32_t> flat = flat_rounds::flat(image.file, source, folder, image.name, options);
    r.source = flat.source;
    r.linked = flat.image.has_value();
    r.same = flat.same;
    r.built = flat.image;
    if (!flat.same) r.notes.push_back(flat.log);
    return r;
}

}  // namespace cb::source::x64_writer
