// cb-unity: alone
// source/console_v30.cpp -- the flat shape's V30MZ processor (console_executable.cpp): a WonderSwan cartridge as NASM source for its V30MZ that builds back into it
// (config/platforms/wonderswan.json).
//
//     r.source           game.asm (NASM, `bits 16`)
//     r.same             the source, built with `nasm -f bin`, is the cartridge file byte for byte
//     r.functions        {linear address: name}
//
// The program is the V30MZ's, in the code segment the footer's reset jump names: code is what flow reaches from the start address (a footer's
// `jmpf cs:ip`) -- jumps, branches and near calls inside that segment's 64 KB -- and every other byte is data (`db` rows). The ROM is written as up to three NASM
// sections, the bytes before the code segment, the segment itself with `vstart` so that a label is the offset the CPU uses, and the bytes after it, the
// last 16 (the footer, with the reset jump) among them. Each instruction is written as text the way Capstone reads it, in NASM's spelling, and what NASM
// does not give back byte for byte (the other encoding of a register move, a long form where a short one fits ...) is written as `db` with the
// instruction beside it, found out in rounds against the assembler (source/flat_rounds). Functions are `start` and `sub_<address>` (the linear address,
// five digits), jump targets `L_<address>`; the I/O ports an `in` or `out` names have the hardware's names (DISPLAY_CTRL, SPR_BASE, KEY_SCAN, INT_ENABLE,
// BANK_ROM0 ...), written as equates. Far jumps and calls into other segments are not followed.
//
// x86asm, CodeBreaker's own assembler, builds the source, in this process.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/wonderswan.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "platforms/decoder.hpp"
#include "source/flat_rounds.hpp"
#include "source/x86_flat.hpp"

namespace cb::source::console_writer::v30_code {

namespace iw = image_writer;
namespace xf = x86_flat;
namespace ws = formats::wonderswan;

const std::map<std::uint32_t, std::string>& ports();

namespace {

constexpr iw::Spelling kSub{"sub_", 5};
constexpr iw::Spelling kLocal{"L_", 5};

const platforms::Decoder& capstone16() {
    thread_local const platforms::Decoder decoder = platforms::Decoder::open(platforms::Arch::X86, 16, true);
    return decoder;
}

bool stops_flow(std::string_view m) { return m == "jmp" || m == "ljmp" || m.starts_with("ret") || m.starts_with("iret") || m == "hlt"; }

// How an instruction leaves, for the format's code scan.
std::optional<formats::DecodedInsn> flow_of(const xf::Insn& i, std::uint32_t code_base) {
    formats::DecodedInsn d;
    d.size = i.size;
    if (const std::optional<std::uint32_t> t = xf::branch_target(i)) d.target = code_base + (*t & 0xFFFF);
    const std::string& m = i.mnemonic;
    if (m == "call") d.flow = d.target ? formats::Flow::Call : formats::Flow::Next;
    else if (m == "jmp") d.flow = d.target ? formats::Flow::Jump : formats::Flow::Indirect;
    else if (m.starts_with("ret") || m.starts_with("iret")) d.flow = formats::Flow::Return;
    else if (m == "ljmp") d.flow = formats::Flow::Indirect;
    else if (m == "hlt") d.flow = formats::Flow::Halt;
    else if (xf::is_branch(m) && d.target) d.flow = formats::Flow::Branch;
    return d;
}

// Flow runs through the code segment's window of the ROM.
class WsFlow final : public iw::CodeModel<xf::Insn> {
public:
    explicit WsFlow(const ws::Rom& rom) : rom_(rom), memory_(rom.memory()), lo_(std::max(rom.code_base(), rom.base())),
                                          hi_(static_cast<std::uint32_t>(std::min<std::uint64_t>(static_cast<std::uint64_t>(rom.code_base()) + 0x10000, ws::kEnd))) {}

    std::uint32_t low() const { return lo_; }
    std::uint32_t high() const { return hi_; }

    std::optional<xf::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        return xf::decode(capstone16(), ByteView(memory_).subspan(pc, std::min<std::size_t>(15, hi_ - pc)), pc - rom_.code_base());
    }
    std::uint32_t size(const xf::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const xf::Insn& insn) const override {
        if (const std::optional<std::uint32_t> t = xf::branch_target(insn)) return rom_.code_base() + (*t & 0xFFFF);
        return std::nullopt;
    }
    bool inside(std::uint32_t pc) const override { return pc >= lo_ && pc < hi_; }
    bool fits(std::uint32_t pc, const xf::Insn& insn) const override { return pc + insn.size <= hi_; }
    bool follows(const xf::Insn& insn) const override { const auto t = target(insn); return t && inside(*t); }
    bool stops(const xf::Insn& insn) const override { return stops_flow(insn.mnemonic); }
    bool finds_functions() const override { return true; }
    bool starts_function(const xf::Insn& insn) const override { return insn.mnemonic == "call"; }

private:
    const ws::Rom& rom_;
    Bytes memory_;
    std::uint32_t lo_, hi_;
};

// An I/O port's operand in an `in` or `out`: the port number as the equate's name.
bool is_port_op(std::string_view m) { return m == "in" || m == "out"; }

// The pieces of the file in order: an instruction or a row of data, each at a file offset.
struct Item {
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    bool code = false;
    std::optional<xf::Insn> insn;
};

class WsImage final : public flat_rounds::FlatImage<std::uint32_t> {
public:
    WsImage(const ws::Rom& rom, const std::map<std::uint32_t, xf::Insn>& code, const iw::Names& labels, const iw::Functions& functions, const WsFlow& flow,
            std::string head)
        : rom_(rom), labels_(labels), functions_(functions), flow_(flow), head_(std::move(head)) {
        const std::uint32_t unmapped = static_cast<std::uint32_t>(rom.data.size() - rom.mapped());
        const auto linear = [&](std::uint32_t offset) { return offset < unmapped ? std::optional<std::uint32_t>() : std::optional<std::uint32_t>(rom.base() + offset - unmapped); };
        std::uint32_t offset = 0;
        while (offset < rom.data.size()) {
            Item item;
            item.offset = offset;
            const std::optional<std::uint32_t> at = linear(offset);
            const auto hit = at ? code.find(*at) : code.end();
            if (hit != code.end()) {
                item.code = true;
                item.insn = hit->second;
                item.size = hit->second.size;
            } else {   // data up to 16 bytes, not across a label, an instruction, a section edge or the end of the unmapped part
                std::uint32_t n = 1;
                while (n < 16 && offset + n < rom.data.size()) {
                    const std::optional<std::uint32_t> a = linear(offset + n);
                    if (a && (!at || code.contains(*a) || labels.contains(*a) || *a == flow.low() || *a == flow.high())) break;
                    ++n;
                }
                item.size = n;
            }
            items_.push_back(std::move(item));
            offset += items_.back().size;
        }
        unmapped_ = unmapped;
    }

    std::uint32_t linear_of(const Item& item) const { return rom_.base() + item.offset - unmapped_; }
    bool mapped(const Item& item) const { return item.offset >= unmapped_; }
    std::string mark(std::size_t index) const { return std::format("m_{}", index); }

    flat_rounds::Source<std::uint32_t> write(const std::set<std::uint32_t>& raw, bool final) override {
        flat_rounds::Source<std::uint32_t> out;
        std::size_t line = 0;
        const auto add = [&](const std::string& text) {
            out.text += text + "\n";
            return ++line;
        };
        std::set<std::string> used_ports;
        std::vector<std::pair<std::string, std::optional<std::uint32_t>>> lines;
        int section = -1;   // 0 before the code segment, 1 the segment, 2 after
        const auto section_of = [&](const Item& item) {
            if (!mapped(item)) return 0;
            const std::uint32_t a = linear_of(item);
            return a < flow_.low() ? 0 : a < flow_.high() ? 1 : 2;
        };
        for (std::size_t k = 0; k < items_.size(); ++k) {
            const Item& item = items_[k];
            const int s = section_of(item);
            if (s != section) {
                section = s;
                const char* names[] = {"pre", "code", "post"};
                std::string header = std::format("section .{} align=1", names[s]);
                if (s == 1) header += std::format(" vstart=0x{:04x}", flow_.low() - rom_.code_base());
                lines.emplace_back("", std::nullopt);
                lines.emplace_back(header, std::nullopt);
            }
            if (mapped(item)) {
                const std::uint32_t a = linear_of(item);
                if (const auto fn = functions_.find(a); fn != functions_.end() && item.code) {
                    lines.emplace_back("", std::nullopt);
                    lines.emplace_back(fn->second + ":", std::nullopt);
                } else if (const auto l = labels_.find(a); l != labels_.end()) {
                    lines.emplace_back(l->second + ":", std::nullopt);
                }
            }
            if (!final) lines.emplace_back(mark(k) + ":", std::nullopt);
            if (item.code && !raw.contains(linear_of(item))) {
                lines.emplace_back("        " + text_of(*item.insn, used_ports), linear_of(item));
            } else if (item.code) {
                lines.emplace_back("        db " + xf::db_bytes(ByteView(rom_.data).subspan(item.offset, item.size), 0, item.size) + "  ; " + item.insn->mnemonic + " " + item.insn->op_str, std::nullopt);
            } else {
                lines.emplace_back("        db " + xf::db_bytes(ByteView(rom_.data).subspan(item.offset, item.size), 0, item.size), std::nullopt);
            }
        }
        for (const std::string_view h : text::split(head_, '\n')) add(std::string(h));
        add("bits 16");
        for (const std::string& p : used_ports) {
            for (const auto& [number, name] : ports()) {
                if (name == p) add(std::format("{:<14} equ 0x{:02X}", name, number));
            }
        }
        for (const auto& [text, key] : lines) {
            const std::size_t n = add(text);
            if (key) out.instruction_at_line[n] = *key;
        }
        return out;
    }

    // An instruction came back different when the bytes at its mark are not its own, or the next mark is not where they end. An instruction of another
    // length moves what follows it (a branch over it then differs only in its offset), so while any instruction is of another length those alone are
    // given; what is left is judged in the next round.
    std::set<std::uint32_t> differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<std::uint32_t>& raw) override {
        std::set<std::uint32_t> other_length, other_bytes;
        for (std::size_t k = 0; k < items_.size(); ++k) {
            const Item& item = items_[k];
            if (!item.code || raw.contains(linear_of(item))) continue;
            const auto here = labels.find(mark(k));
            if (here == labels.end()) {
                other_length.insert(linear_of(item));
                continue;
            }
            const std::uint64_t at = here->second.place;
            const auto next = k + 1 < items_.size() ? labels.find(mark(k + 1)) : labels.end();
            const std::uint64_t end = next != labels.end() ? next->second.place : built.size();
            if (end - at != item.size || at + item.size > built.size()) {
                other_length.insert(linear_of(item));
            } else if (!std::equal(rom_.data.begin() + item.offset, rom_.data.begin() + item.offset + item.size, built.begin() + static_cast<std::ptrdiff_t>(at))) {
                other_bytes.insert(linear_of(item));
            }
        }
        return other_length.empty() ? other_bytes : other_length;
    }

private:
    // An instruction in NASM's spelling: a direct branch names its label, a port its equate, a string instruction has no operands.
    std::string text_of(const xf::Insn& i, std::set<std::string>& used_ports) const {
        std::string m = i.mnemonic;
        if (m == "ljmp") m = "jmp";
        if (m == "lcall") m = "call";
        std::string ops = xf::nasm_operands(i.op_str);
        if (xf::is_string_op(m)) return m;
        if (const std::optional<std::uint32_t> t = xf::branch_target(i)) {
            const std::uint32_t linear = rom_.code_base() + (*t & 0xFFFF);
            const auto fn = functions_.find(linear);
            const auto lab = labels_.find(linear);
            if (fn != functions_.end()) ops = fn->second;
            else if (lab != labels_.end()) ops = lab->second;
            if (m == "jmp") m += i.size == 2 ? " short" : " near";   // (the form the original linked, not the one NASM would choose)
        } else if (is_port_op(m)) {
            const std::size_t comma = ops.find(", ");
            const bool in = m == "in";
            std::string port = in ? ops.substr(comma == std::string::npos ? 0 : comma + 2) : ops.substr(0, comma);
            const bool hex = port.starts_with("0x") && port.size() <= 4 && port.size() > 2;
            const bool decimal = !port.empty() && port.size() <= 3 && std::ranges::all_of(port, [](char c) { return c >= '0' && c <= '9'; });   // (Capstone writes a number below 10 in decimal)
            if (hex || decimal) {
                const std::uint32_t number = static_cast<std::uint32_t>(std::stoul(port, nullptr, hex ? 16 : 10));
                if (const auto it = ports().find(number); it != ports().end()) {
                    used_ports.insert(it->second);
                    ops = in ? ops.substr(0, comma + 2) + it->second : it->second + ops.substr(comma);
                }
            }
        }
        return ops.empty() ? m : m + " " + ops;
    }

    const ws::Rom& rom_;
    const iw::Names& labels_;
    const iw::Functions& functions_;
    const WsFlow& flow_;
    std::string head_;
    std::vector<Item> items_;
    std::uint32_t unmapped_ = 0;
};

}  // namespace

// The I/O ports by number (config/knowledge/wonderswan/registers.json).
const std::map<std::uint32_t, std::string>& ports() {
    // config/knowledge/wonderswan/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("wonderswan", "ports");
    return table;
}

formats::Decoder decoder(std::uint32_t code_base) {
    return [code_base](ByteView mem, std::uint32_t pc) -> std::optional<formats::DecodedInsn> {
        if (pc < code_base || pc >= mem.size()) return std::nullopt;
        const std::optional<xf::Insn> insn = xf::decode(capstone16(), mem.subspan(pc, std::min<std::size_t>(15, mem.size() - pc)), pc - code_base);
        if (!insn) return std::nullopt;
        return flow_of(*insn, code_base);
    };
}

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    const ws::Rom& rom = contained_as<ws::Rom>(loaded);
    fs::make_dirs(folder);
    const std::string stem = std::get<std::string>(values.at("stem"));
    const WsFlow flow(rom);
    const std::vector<std::uint32_t> entries = {rom.entry()};
    const iw::Walked<xf::Insn> walked = iw::walk<xf::Insn>(flow, entries);
    const std::map<std::uint32_t, xf::Insn>& code = walked.code;
    iw::Names names;
    names.try_emplace(rom.entry(), "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return flow.inside(a) && !inside_insn.contains(a); }, kLocal);
    for (const auto& [a, n] : functions) labels.try_emplace(a, n);

    std::string head;   // (the description's lines, one text)
    for (const std::string& line : head_comment(description, values)) head += (head.empty() ? "" : "\n") + line;
    WsImage image(rom, code, labels, functions, flow, head);

    iw::Written r;
    r.functions = functions;
    r.image_bytes = rom.data.size();
    for (const auto& [at, insn] : code) r.code_bytes += insn.size;
    flat_rounds::FlatOptions options;
    flat_rounds::FlatResult<std::uint32_t> flat = flat_rounds::flat(rom.data, image, folder, stem, options);
    r.source = flat.source;
    r.linked = flat.image.has_value();
    r.same = flat.same;
    r.built = flat.image;
    if (!flat.same) r.notes.push_back(flat.log);
    return r;
}


}  // namespace cb::source::console_writer::v30_code
