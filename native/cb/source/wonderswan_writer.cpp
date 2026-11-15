// cb-unity: alone
// cb/source/wonderswan_writer.cpp -- a WonderSwan cartridge as NASM source that builds back into it (wonderswan_writer.hpp).

#include "cb/source/wonderswan_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/flat_rounds.hpp"
#include "cb/source/x86_flat.hpp"

namespace cb::source::wonderswan_writer {

namespace iw = image_writer;
namespace xf = x86_flat;
namespace ws = formats::wonderswan;

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

const std::map<std::uint32_t, std::string>& ports() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x00, "DISPLAY_CTRL"}, {0x01, "BACK_COLOR"},   {0x02, "LINE_CURRENT"}, {0x03, "LINE_COMPARE"}, {0x04, "SPR_BASE"},     {0x05, "SPR_FIRST"},
        {0x06, "SPR_COUNT"},    {0x07, "MAP_BASE"},     {0x08, "SCR2_WIN_X0"},  {0x09, "SCR2_WIN_Y0"},  {0x0A, "SCR2_WIN_X1"},  {0x0B, "SCR2_WIN_Y1"},
        {0x0C, "SPR_WIN_X0"},   {0x0D, "SPR_WIN_Y0"},   {0x0E, "SPR_WIN_X1"},   {0x0F, "SPR_WIN_Y1"},   {0x10, "SCR1_X"},       {0x11, "SCR1_Y"},
        {0x12, "SCR2_X"},       {0x13, "SCR2_Y"},       {0x14, "LCD_CTRL"},     {0x15, "LCD_ICON"},     {0x16, "LCD_VTOTAL"},   {0x17, "LCD_VSYNC"},
        {0x1C, "PALMONO_POOL0"}, {0x1D, "PALMONO_POOL1"}, {0x1E, "PALMONO_POOL2"}, {0x1F, "PALMONO_POOL3"}, {0x40, "DMA_SOURCE"}, {0x44, "DMA_DEST"},
        {0x46, "DMA_LENGTH"},   {0x48, "DMA_CTRL"},     {0x4A, "SDMA_SOURCE"},  {0x4E, "SDMA_LENGTH"},  {0x52, "SDMA_CTRL"},    {0x60, "SYSTEM_CTRL2"},
        {0x62, "SYSTEM_CTRL3"}, {0x80, "SND_CH1_PITCH"}, {0x82, "SND_CH2_PITCH"}, {0x84, "SND_CH3_PITCH"}, {0x86, "SND_CH4_PITCH"}, {0x88, "SND_CH1_VOL"},
        {0x89, "SND_CH2_VOL"},  {0x8A, "SND_CH3_VOL"},  {0x8B, "SND_CH4_VOL"},  {0x8C, "SND_SWEEP"},    {0x8D, "SND_SWEEP_TIME"}, {0x8E, "SND_NOISE"},
        {0x8F, "SND_WAVE_BASE"}, {0x90, "SND_CTRL"},    {0x91, "SND_OUTPUT"},   {0x92, "SND_RANDOM"},   {0x94, "SND_VOL_CH2"},  {0xA0, "HW_FLAGS"},
        {0xA2, "TIMER_CTRL"},   {0xA4, "HTIMER_FREQ"},  {0xA6, "VTIMER_FREQ"},  {0xA8, "HTIMER_COUNT"}, {0xAA, "VTIMER_COUNT"}, {0xB0, "INT_BASE"},
        {0xB1, "SER_DATA"},     {0xB2, "INT_ENABLE"},   {0xB3, "SER_STATUS"},   {0xB4, "INT_STATUS"},    {0xB5, "KEY_SCAN"},     {0xB6, "INT_ACK"},
        {0xBA, "IEEP_DATA"},    {0xBC, "IEEP_CMD"},     {0xBE, "IEEP_CTRL"},    {0xC0, "BANK_ROM_LINEAR"}, {0xC1, "BANK_SRAM"},  {0xC2, "BANK_ROM0"},
        {0xC3, "BANK_ROM1"},    {0xC4, "EEP_DATA"},     {0xC6, "EEP_CMD"},      {0xC8, "EEP_CTRL"},     {0xCA, "RTC_CTRL"},     {0xCB, "RTC_DATA"}};
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

std::optional<std::filesystem::path> find_nasm() {
    if (const auto env = fs::get_env("CB_NASM"); env && !env->empty()) return fs::from_text(*env);
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        if (const auto configured = main.path("nasm"); configured && std::filesystem::is_regular_file(*configured)) return configured;
        const std::filesystem::path built = main.checkout_root() / "native" / "nasm" / "build" / "bin" / "nasm.exe";
        if (std::filesystem::is_regular_file(built)) return built;
    } catch (const Error&) {   // (no configuration: no assembler)
    }
    return std::nullopt;
}

iw::Written write(const ws::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
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

    const std::string head = std::format("; {}.asm -- written by CodeBreaker from {}\n; WonderSwan{} cartridge, {} bytes, start {:04X}:{:04X}, developer {:02X}, game {:02X}, version {}, save type {:02X}, checksum {:04X} ({})",
                                         stem, std::filesystem::path(file_name).filename().string(), rom.is_color() ? " Color" : "", rom.data.size(), rom.reset_segment,
                                         rom.reset_offset, rom.developer, rom.game_id, rom.version, rom.save, rom.checksum, rom.checksum_ok ? "right" : "does not match the file");
    WsImage image(rom, code, labels, functions, flow, head);

    iw::Written r;
    r.functions = functions;
    r.image_bytes = rom.data.size();
    for (const auto& [at, insn] : code) r.code_bytes += insn.size;
    const std::optional<std::filesystem::path> nasm = find_nasm();
    if (!nasm) {
        r.source = folder / (stem + ".asm");
        fs::write_text(r.source, image.write({}, true).text);
        r.notes.push_back("NASM was not found (settings: nasm, or CB_NASM): the source is written and not built");
        return r;
    }
    flat_rounds::FlatOptions options;
    options.nasm = *nasm;
    flat_rounds::FlatResult<std::uint32_t> flat = flat_rounds::flat(rom.data, image, folder, stem, options);
    r.source = flat.source;
    r.linked = flat.image.has_value();
    r.same = flat.same;
    r.built = flat.image;
    if (!flat.same) r.notes.push_back(flat.log);
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(ws::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::wonderswan_writer
