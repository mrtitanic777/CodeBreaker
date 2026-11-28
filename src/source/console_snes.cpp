// source/console_snes.cpp -- the console writer's Super Nintendo (config/platforms/snes.json): a cartridge on the `segmented` shape, with the
// flow the shape cannot do alone (console_writer.hpp: SegmentCode). The register widths change an instruction's length, so flow tracks them:
// from the reset vector (emulation mode, 8 bits) through `REP`, `SEP`, and `CLC; XCE` (which sets or clears emulation mode when the carry is
// known there), each instruction is decoded under the widths the path that reached it left, and one that takes an immediate of the
// register's width gets a `.a8` / `.a16` / `.i8` / `.i16` before it when the assembler's last setting differs. The first path to arrive at an
// address decides the widths there. Entries are the vectors (RESET in emulation mode; NMI, IRQ, COP, BRK, ABORT in native mode, 8 bits
// assumed); calls are followed (JSL, JSR, the branches and JMP and JML to a constant), and a function is every call target. A function's
// address carries the state it starts in (console_snes.hpp).
//
// Banks are linked at the address the code runs at: LoROM at $nn8000 (the mirror, $00 or $80, that more of the program's long addresses use),
// HiROM at $C0nn0000. The 64-byte internal header is written as its fields. Function starts are `reset`, `nmi`, `irq`, `cop`, `brk`, `abort`,
// `emu_...` or `sub_<address>`; jump targets `L_<address>`; a data address an instruction names `D_<address>`; the chips' registers have their
// hardware names, the direct page `dp_20`. A 16-bit operand that names a label of another bank is written `.loword(label)`. An instruction
// whose text does not assemble back into its own bytes is written as `.byte` data; the SPC700 sound program is data.

#include "source/console_snes.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/w65816.hpp"
#include "formats/snes.hpp"
#include "source/console_shapes.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer::snes_code {

namespace w = decompile::w65816;
namespace iw = image_writer;
namespace snes = formats::snes;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};

// What flow knows at an instruction: the register widths, whether the CPU is in emulation mode, and the carry when a CLC / SEC / REP / SEP made it known
// (which is how XCE is followed).
struct State {
    bool m16 = false;
    bool x16 = false;
    bool e = true;
    bool carry_known = false;
    bool carry = false;
};

struct Insn {
    w::Insn i;
    State before;
    State after;
};

State advance(const w::Insn& i, State s) {
    const std::string_view m = i.mnemonic;
    const auto carry_is = [&](bool value) {
        s.carry_known = true;
        s.carry = value;
    };
    if (m == "REP") {
        if (!s.e) {
            if (i.operand & 0x20) s.m16 = true;
            if (i.operand & 0x10) s.x16 = true;
        }
        if (i.operand & 1) carry_is(false);
    } else if (m == "SEP") {
        if (!s.e) {
            if (i.operand & 0x20) s.m16 = false;
            if (i.operand & 0x10) s.x16 = false;
        }
        if (i.operand & 1) carry_is(true);
    } else if (m == "CLC") {
        carry_is(false);
    } else if (m == "SEC") {
        carry_is(true);
    } else if (m == "XCE") {
        if (s.carry_known) {
            const bool was = s.e;
            s.e = s.carry;
            s.carry = was;
            if (s.e) s.m16 = s.x16 = false;
        }
    } else if (m == "ADC" || m == "SBC" || m == "CMP" || m == "CPX" || m == "CPY" || m == "ASL" || m == "LSR" || m == "ROL" || m == "ROR" || m == "PLP" || m == "RTI" ||
               m == "JSR" || m == "JSL") {
        s.carry_known = false;
    }
    return s;
}

bool is_hardware(std::uint32_t v) { return !snes::register_name(v).empty(); }

// Flow runs through the ROM by canonical address (the mirror `high` picks); the first path to reach an address decides the state there.
class SnesFlow final : public iw::CodeModel<Insn> {
public:
    SnesFlow(const snes::Rom& rom, bool high) : rom_(rom), high_(high) {}

    std::optional<std::uint32_t> canonical(std::uint32_t address) const {
        const auto offset = rom_.offset(address);
        if (!offset) return std::nullopt;
        return rom_.address_of(*offset, high_);
    }
    void seed(std::uint32_t pc, const State& state) const { pending_.try_emplace(pc, state); }
    std::optional<State> state_at(std::uint32_t pc) const {
        const auto it = pending_.find(pc);
        if (it == pending_.end()) return std::nullopt;
        return it->second;
    }

    std::optional<Insn> decode(std::uint32_t pc) const override {
        const auto offset = code_offset(pc);
        if (!offset) return std::nullopt;
        Bytes bytes;
        for (std::uint32_t i = 0; i < 4; ++i) {   // the bytes after it in the bank, as far as the file has them in a row
            const auto at = rom_.offset((pc & 0xFF0000) | ((pc + i) & 0xFFFF));
            if (!at || *at != *offset + i) break;
            bytes.push_back(rom_.data[*at]);
        }
        State state;
        if (const auto it = pending_.find(pc); it != pending_.end()) state = it->second;
        const std::optional<w::Insn> insn = w::decode(bytes, 0, pc, state.m16 && !state.e, state.x16 && !state.e);
        if (!insn) return std::nullopt;
        if (*offset < rom_.header_offset && *offset + insn->size > rom_.header_offset) return std::nullopt;   // (the header is data)
        Insn out{*insn, state, advance(*insn, state)};
        if (!w::ends_flow(insn->mnemonic)) pending_.try_emplace(pc + insn->size, out.after);
        return out;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    std::optional<std::uint32_t> target(const Insn& insn) const override {
        if (!insn.i.target || insn.i.mnemonic == "PER") return std::nullopt;   // (PER pushes an address: it is not a jump)
        const auto to = canonical(*insn.i.target);
        if (!to || !code_offset(*to)) return std::nullopt;
        pending_.try_emplace(*to, insn.after);
        return to;
    }
    bool inside(std::uint32_t pc) const override { return code_offset(pc).has_value(); }
    bool fits(std::uint32_t, const Insn&) const override { return true; }   // (decode only gives an instruction whose bytes are all in the ROM, in a row)
    bool follows(const Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const Insn& insn) const override { return w::ends_flow(insn.i.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return *rom_.offset(pc); }
    bool finds_functions() const override { return true; }
    bool starts_function(const Insn& insn) const override { return insn.i.mnemonic == "JSR" || insn.i.mnemonic == "JSL"; }

private:
    // The ROM offset of a place code may stand: the ROM outside the internal header.
    std::optional<std::uint32_t> code_offset(std::uint32_t pc) const {
        const auto offset = rom_.offset(pc);
        if (!offset || (*offset >= rom_.header_offset && *offset < rom_.header_offset + snes::kInternalHeaderSize)) return std::nullopt;
        return offset;
    }

    const snes::Rom& rom_;
    bool high_;
    mutable std::unordered_map<std::uint32_t, State> pending_;
};

// What text() of an instruction needs to name its operands.
struct Naming {
    const snes::Rom& rom;
    bool high;
    const iw::Names& labels;                     // by canonical address
    const decompile::AddressLabels& direct_page;   // by byte
    std::map<std::uint32_t, std::string>* equates = nullptr;   // the hardware names used (collected when set)
};

bool direct_page_byte(const w::Insn& i, std::uint32_t& out);

// The names of the numbers an instruction has: its labels, the hardware registers and the direct-page bytes it names. A label stands for the number it
// is (a branch's or a long operand's 24 bits, a 16-bit operand of a label in bank 0); a 16-bit operand of a label past bank 0 is `.loword(label)`.
decompile::AddressLabels insn_names(const Naming& n, const Insn& insn) {
    const w::Insn& i = insn.i;
    decompile::AddressLabels names;
    // the name of the label at the place `full` shows in the ROM, when its address is `operand` itself (`sixteen_bit`: when it has the same low 16 bits)
    const auto label_for = [&](std::uint32_t full, std::uint32_t operand, bool sixteen_bit) -> std::optional<std::string> {
        const auto offset = n.rom.offset(full);
        if (!offset) return std::nullopt;
        const std::uint32_t canonical = n.rom.address_of(*offset, n.high);
        const auto it = n.labels.find(canonical);
        if (it == n.labels.end()) return std::nullopt;
        if (sixteen_bit ? (canonical & 0xFFFF) != operand : canonical != operand) return std::nullopt;
        return sixteen_bit && canonical > 0xFFFF ? ".loword(" + it->second + ")" : it->second;
    };
    const auto hardware = [&](std::uint32_t v) {
        const std::string name = snes::register_name(v);
        if (name.empty()) return false;
        names[v] = name;
        if (n.equates) (*n.equates)[v & 0xFFFF] = name;
        return true;
    };
    const std::uint32_t bank = i.at & 0xFF0000;
    switch (i.mode) {
        case w::Mode::Rel:
        case w::Mode::RelLong:
            if (const auto name = label_for(*i.target, *i.target, false)) names[*i.target] = *name;
            break;
        case w::Mode::Abs:
            if (i.target) {   // (JMP and JSR: the description writes the 24 bits, the program bank's with the operand; the source says the 16 bits)
                if (const auto name = label_for(*i.target, i.operand, true)) names[*i.target] = *name;
            } else if (i.mnemonic != "PEA" && !hardware(i.operand)) {
                if (const auto name = label_for(bank | i.operand, i.operand, true)) names[i.operand] = *name;
            }
            break;
        case w::Mode::AbsX:
        case w::Mode::AbsY:
        case w::Mode::AbsIndX:
        case w::Mode::AbsIndL:
            if (!hardware(i.operand)) {
                if (const auto name = label_for(bank | i.operand, i.operand, true)) names[i.operand] = *name;
            }
            break;
        case w::Mode::Long:
        case w::Mode::LongX:
            if ((i.operand >> 16) == 0 && is_hardware(i.operand & 0xFFFF)) hardware(i.operand);
            else if (const auto name = label_for(i.operand, i.operand, false)) names[i.operand] = *name;
            break;
        default: break;
    }
    if (std::uint32_t byte = 0; direct_page_byte(i, byte)) {
        if (const auto it = n.direct_page.find(byte); it != n.direct_page.end()) names[byte] = it->second;
    }
    return names;
}

bool direct_page_byte(const w::Insn& i, std::uint32_t& out) {
    switch (i.mode) {
        case w::Mode::Dp:
        case w::Mode::DpX:
        case w::Mode::DpY:
        case w::Mode::DpInd:
        case w::Mode::DpIndX:
        case w::Mode::DpIndY:
        case w::Mode::DpIndL:
        case w::Mode::DpIndLY: out = i.operand; return true;
        default: return false;
    }
}

std::string word_list(const std::vector<std::string>& words) { return "        .half " + text::join(words, ","); }

// The Super Nintendo's source is ca65's: the description's dialect (config/dialects/ca65-65816.json), set by follow().
const dialect::Dialect* g_said = nullptr;
const dialect::Dialect& ca65() { return *g_said; }

// An instruction the description reads in a way its bytes do not come back from, written as ca65 writes it: a long operand in bank 0 (or a named one)
// as `f:`, an absolute operand below $100 as `a:`. Empty when it is not one of these.
std::string forced_text(const Insn& insn, const decompile::AddressLabels& names) {
    const w::Insn& i = insn.i;
    if ((i.mode == w::Mode::Long || i.mode == w::Mode::LongX) && !i.target) {
        const auto named = names.find(i.operand);
        if (named == names.end() && i.operand >= 0x10000) return {};
        return ca65().forced_long(i.mnemonic, named != names.end() ? named->second : ca65().number(i.operand, 6), i.mode == w::Mode::LongX);
    }
    if (i.mode == w::Mode::Abs && !i.target && i.mnemonic != "PEA" && i.operand < 0x100) {
        const auto named = names.find(i.operand);
        return ca65().forced_absolute(i.mnemonic, named != names.end() ? named->second : ca65().number(i.operand, 4));
    }
    return {};
}

// The number of hex digits an operand of this instruction is written with.
unsigned digits_of(const w::Insn& i) {
    switch (i.mode) {
        case w::Mode::ImmM: return i.m16 ? 4 : 2;
        case w::Mode::ImmX: return i.x16 ? 4 : 2;
        case w::Mode::Long:
        case w::Mode::LongX:
        case w::Mode::Rel:
        case w::Mode::RelLong: return 6;
        case w::Mode::Abs:
        case w::Mode::AbsX:
        case w::Mode::AbsY:
        case w::Mode::AbsInd:
        case w::Mode::AbsIndX:
        case w::Mode::AbsIndL: return 4;
        default: return 2;
    }
}

// `text` with each number written 0x<hex> that is not part of a name written with at least `digits` digits (`LDX #0x10` is `LDX #0x0010` when X is 16 bits wide).
std::string padded(const std::string& text, unsigned digits) {
    std::string out;
    for (std::size_t k = 0; k < text.size();) {
        const bool name_before = k > 0 && (std::isalnum(static_cast<unsigned char>(text[k - 1])) || text[k - 1] == '_');
        if (!name_before && text[k] == '0' && k + 2 < text.size() && text[k + 1] == 'x' && std::isxdigit(static_cast<unsigned char>(text[k + 2]))) {
            std::size_t end = k + 2;
            while (end < text.size() && std::isxdigit(static_cast<unsigned char>(text[end]))) ++end;
            std::string hex = text.substr(k + 2, end - k - 2);
            if (hex.size() < digits) hex.insert(0, digits - hex.size(), '0');
            out += "0x" + hex;
            k = end;
        } else {
            out += text[k++];
        }
    }
    return out;
}

class SnesListing final : public iw::Listing<Insn> {
public:
    SnesListing(const snes::Rom& rom, bool high, std::size_t first_offset, const std::map<std::uint32_t, Insn>& code, const std::set<std::uint32_t>& demoted,
                const std::map<std::uint32_t, std::string>& described, const Naming& naming, State& assembler, translate::l2::Assembly& out)
        : rom_(rom), high_(high), first_(first_offset), code_(code), demoted_(demoted), described_(described), naming_(naming), assembler_(assembler), out_(out) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address_of(offset, high_); }
    const Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        if (offset + it->second.i.size > first_ + rom_.bank_size()) return nullptr;
        at_offset_ = offset;   // (prelude() and text() of this instruction follow)
        return &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    // The register widths an immediate operand has, when the assembler is not at them: `.context` sets both of the description's variables.
    std::string prelude(const Insn& insn) override {
        const w::Insn& i = insn.i;
        if (!((i.mode == w::Mode::ImmM && i.m16 != assembler_.m16) || (i.mode == w::Mode::ImmX && i.x16 != assembler_.x16))) return {};
        // (one variable of the description's context at a time: ca65's `.a16` / `.i16`)
        if (i.mode == w::Mode::ImmM) {
            assembler_.m16 = i.m16;
            return std::format(".context m16={}", i.m16 ? 1 : 0);
        }
        assembler_.x16 = i.x16;
        return std::format(".context x16={}", i.x16 ? 1 : 0);
    }
    std::string text(const Insn& insn) const override {
        const decompile::AddressLabels names = insn_names(naming_, insn);
        if (std::string forced = forced_text(insn, names); !forced.empty()) return forced;
        if (const auto it = described_.find(at_offset_); it != described_.end()) return padded(described_source::name_numbers(it->second, names), digits_of(insn.i));
        // (a text the description cannot say back: the bytes, with the description's reading in a comment)
        const w::Insn& i = insn.i;
        return m6502_source::code(m6502_source::Cpu::W65816, ByteView(rom_.data).subspan(at_offset_, i.size), i.at, {}, m6502_source::widths(i.m16, i.x16));
    }
    std::vector<std::string> data_lines(ByteView data) const override { return m6502_source::data_lines(data); }

    // The internal header, field by field; a vector that is a label of bank 0 is the label.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != rom_.header_offset) return 0;
        flush();
        const ByteView h = ByteView(rom_.data).subspan(offset, snes::kInternalHeaderSize);
        const auto hex = [&](std::size_t from, std::size_t count) {
            std::vector<std::string> v;
            for (std::size_t k = 0; k < count; ++k) v.push_back(std::format("0x{:02x}", h[from + k]));
            return "        .byte " + text::join(v, ",");
        };
        const auto vector_words = [&](std::size_t from) {
            std::vector<std::string> v;
            for (std::size_t k = 0; k < 6; ++k) {
                const std::uint16_t at = static_cast<std::uint16_t>(h[from + 2 * k] | h[from + 2 * k + 1] << 8);
                std::string text = std::format("0x{:04x}", at);
                if (const auto o = rom_.offset(at); o && at >= 0x8000) {
                    const auto named = naming_.labels.find(rom_.address_of(*o, high_));
                    if (named != naming_.labels.end() && (rom_.address_of(*o, high_) & 0xFFFF0000) == 0) text = named->second;
                }
                v.push_back(text);
            }
            return word_list(v);
        };
        out_.append("        ; the internal header");
        for (const std::string& line : m6502_source::data_lines(h.subspan(0, 21))) out_.append(line);
        out_.append(hex(0x15, 7) + "   ; map mode, chipset, ROM size, RAM size, country, developer, version");
        out_.append(std::format("        .half 0x{:04x},0x{:04x}   ; checksum complement, checksum", rom_.complement, rom_.checksum));
        out_.append(hex(0x20, 4));
        out_.append(vector_words(0x24) + "   ; native mode: COP, BRK, ABORT, NMI, -, IRQ");
        out_.append(hex(0x30, 4));
        out_.append(vector_words(0x34) + "   ; emulation mode: COP, -, ABORT, NMI, RESET, IRQ");
        return static_cast<std::uint32_t>(snes::kInternalHeaderSize);
    }

private:
    mutable std::uint32_t at_offset_ = 0;   // (the ROM offset of the instruction code_at() last gave)
    const snes::Rom& rom_;
    bool high_;
    std::size_t first_;
    const std::map<std::uint32_t, Insn>& code_;
    const std::set<std::uint32_t>& demoted_;
    const std::map<std::uint32_t, std::string>& described_;   // by ROM offset: the description's text of each instruction written as one
    const Naming& naming_;
    State& assembler_;
    translate::l2::Assembly& out_;
};

// The code flow finds from the vectors with the long addresses written as the mirror `high` says.
struct Found {
    iw::Walked<Insn> walked;
    iw::Names names;
    std::vector<std::pair<std::uint32_t, State>> entries;
};

Found find_code(const snes::Rom& rom, const SnesFlow& flow) {
    Found f;
    const auto add = [&](std::uint16_t vector, std::string_view label, bool native) {
        const auto at = flow.canonical(vector);
        if (!at) return;
        State s;
        s.e = !native;
        flow.seed(*at, s);
        if (std::none_of(f.entries.begin(), f.entries.end(), [&](const auto& e) { return e.first == *at; })) f.entries.emplace_back(*at, s);
        f.names.try_emplace(*at, std::string(label));
    };
    add(rom.emulation.reset, "reset", false);
    add(rom.native.nmi, "nmi", true);
    add(rom.native.irq, "irq", true);
    add(rom.native.cop, "cop", true);
    add(rom.native.brk, "brk", true);
    add(rom.native.abort, "abort", true);
    add(rom.emulation.nmi, "emu_nmi", false);
    add(rom.emulation.irq, "emu_irq", false);
    add(rom.emulation.cop, "emu_cop", false);
    add(rom.emulation.abort, "emu_abort", false);
    std::vector<std::uint32_t> at;
    for (const auto& e : f.entries) at.push_back(e.first);
    f.walked = iw::walk<Insn>(flow, at);
    return f;
}


// The SNES's code: flow with the register widths and the emulation flag it follows, the long addresses in the mirror more of
// the program's own long operands use.
class SnesCode final : public SegmentCode {
public:
    explicit SnesCode(std::shared_ptr<const snes::Rom> rom) : rom_(std::move(rom)) {}

    void follow(const Description& d) override {
        g_said = &dialect::find(d.dialect);
        const snes::Rom& rom = *rom_;
        // LoROM's long addresses are written in one of two mirrors, $00 or $80: the one more of the program's own long operands use
        if (rom.mapping == snes::Mapping::LoRom) {
            const SnesFlow probe(rom, false);
            const Found first = find_code(rom, probe);
            std::size_t high_votes = 0, low_votes = 0;
            for (const auto& entry : first.walked.code) {
                const w::Insn& i = entry.second.i;
                if (i.mode != w::Mode::Long && i.mode != w::Mode::LongX) continue;
                const std::uint32_t bank = i.operand >> 16;
                if (bank >= 0x7E && bank <= 0x7F) continue;
                (bank >= 0x80 ? high_votes : low_votes) += 1;
            }
            high_ = high_votes > low_votes;
        }
        flow_ = std::make_unique<SnesFlow>(rom, high_);
        const SnesFlow& flow = *flow_;
        const bool high = high_;
        Found found = find_code(rom, flow);
        iw::Names names = found.names;
        functions_ = iw::name_functions(found.walked.starts, names, kSub);

        // where a line can start: any byte not inside an instruction
        std::set<std::uint32_t> inside;
        for (const auto& [offset, insn] : found.walked.code) {
            for (std::uint32_t k = 1; k < insn.i.size; ++k) inside.insert(offset + k);
        }
        std::set<std::uint32_t> line_at;
        for (std::uint32_t offset = 0; offset < rom.data.size(); ++offset) {
            if (!inside.contains(offset)) line_at.insert(rom.address_of(offset, high));
        }
        labels_ = iw::make_labels(found.walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
        code_ = found.walked.code;
        // data an instruction names where a line starts (a 16-bit operand is in the instruction's own bank)
        for (const auto& entry : code_) {
            const w::Insn& i = entry.second.i;
            std::uint32_t full = 0;
            switch (i.mode) {
                case w::Mode::Abs: if (i.target || i.mnemonic == "PEA") continue; full = (i.at & 0xFF0000) | i.operand; break;
                case w::Mode::AbsX:
                case w::Mode::AbsY:
                case w::Mode::AbsIndX:
                case w::Mode::AbsIndL: full = (i.at & 0xFF0000) | i.operand; break;
                case w::Mode::Long:
                case w::Mode::LongX: full = i.operand; break;
                default: continue;
            }
            if (is_hardware(full & 0xFFFF) && (full >> 16 == 0 || i.mode == w::Mode::Abs || i.mode == w::Mode::AbsX || i.mode == w::Mode::AbsY)) continue;
            if (const auto offset = rom.offset(full)) {
                const std::uint32_t canonical = rom.address_of(*offset, high);
                const bool in_header = *offset >= rom.header_offset && *offset < rom.header_offset + snes::kInternalHeaderSize;   // (the header is written as its fields)
                if (line_at.contains(canonical) && !in_header) labels_.try_emplace(canonical, std::format("D_{:06X}", canonical));
            }
        }
        for (const auto& entry : code_) {
            if (std::uint32_t byte = 0; direct_page_byte(entry.second.i, byte)) direct_page_[byte] = std::format("dp_{:02X}", byte);
        }

        // the text of each instruction; one that does not assemble back into its bytes is data
        naming_ = std::make_unique<Naming>(Naming{rom, high, labels_, direct_page_, &hardware_});
        for (const auto& entry : code_) (void)insn_names(*naming_, entry.second);   // (collects the hardware names used)
        naming_->equates = nullptr;
        for (const auto& [offset, insn] : code_) {
            const w::Insn& i = insn.i;
            if (!forced_text(insn, insn_names(*naming_, insn)).empty()) continue;   // (ca65's `f:` and `a:` say what the description's text does not)
            if (const auto text = m6502_source::described(m6502_source::Cpu::W65816, ByteView(rom.data).subspan(offset, i.size), i.at, m6502_source::widths(i.m16, i.x16))) described_[offset] = *text;
            else demoted_.insert(offset);
        }
        for (const auto& entry : code_) {
            if (!demoted_.contains(entry.first)) code_total_ += entry.second.i.size;
        }
        // the functions as the lane runs them: with the state each starts in
        for (const auto& [at, label] : functions_) {
            const State s = flow.state_at(at).value_or(State{});
            std::uint32_t key = at & kAddressMask;
            if (!s.e && s.m16) key |= kStartsWith16BitAccumulator;
            if (!s.e && s.x16) key |= kStartsWith16BitIndex;
            if (!s.e) key |= kStartsNative;
            with_state_[key] = label;
        }
        assembler_.m16 = assembler_.x16 = false;   // the widths the `.context` the assembler is at gives: 8 bits until said
    }
    iw::Functions functions() const override { return with_state_; }
    std::size_t code_bytes() const override { return code_total_; }
    std::vector<std::string> names() const override {
        std::vector<std::string> lines;
        for (const auto& [value, label] : hardware_) lines.push_back(m6502_source::equ(label, value, 4));
        if (!hardware_.empty()) lines.push_back("");
        for (const auto& [byte, label] : direct_page_) lines.push_back(m6502_source::equ(label, byte, 2));
        if (!direct_page_.empty()) lines.push_back("");
        return lines;
    }
    std::uint32_t linked_at(const Piece& piece) const override { return rom_->address_of(static_cast<std::uint32_t>(first_of(piece)), high_); }
    void write(translate::l2::Assembly& out, const Piece& piece) override {
        const std::size_t first = first_of(piece);
        const std::size_t last = std::min(rom_->data.size(), first + rom_->bank_size());
        SnesListing listing(*rom_, high_, first, code_, demoted_, described_, *naming_, assembler_, out);
        iw::emit<Insn>(out, static_cast<std::uint32_t>(first), static_cast<std::uint32_t>(last), labels_, functions_, listing);
    }

private:
    std::size_t first_of(const Piece& piece) const { return piece.offset - rom_->copier_header.size(); }   // (the bank's first ROM offset)

    std::shared_ptr<const snes::Rom> rom_;
    bool high_ = false;
    std::unique_ptr<SnesFlow> flow_;
    iw::Functions functions_, with_state_;
    std::map<std::uint32_t, Insn> code_;
    iw::Names labels_;
    decompile::AddressLabels direct_page_;
    std::map<std::uint32_t, std::string> hardware_;
    std::unique_ptr<Naming> naming_;
    std::set<std::uint32_t> demoted_;
    std::map<std::uint32_t, std::string> described_;
    std::size_t code_total_ = 0;
    State assembler_;
};

}  // namespace

Loaded load_snes_file(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const snes::Rom>(snes::read(file));
    Loaded l;
    l.image = rom->copier_header;
    l.image.insert(l.image.end(), rom->data.begin(), rom->data.end());
    Segments& s = l.segments;
    const std::uint32_t header = static_cast<std::uint32_t>(rom->copier_header.size());
    if (header) s.pieces.push_back({"HEADER", "HEADER", "copier", 0, header, 0, false});
    for (std::size_t b = 0; b < rom->bank_count(); ++b) {
        const std::size_t first = b * rom->bank_size();
        const std::string name = std::format("BANK{}", b);
        s.pieces.push_back({name, name, "", header + static_cast<std::uint32_t>(first), static_cast<std::uint32_t>(std::min(rom->bank_size(), rom->data.size() - first)), 0, true});
    }
    s.code = std::make_shared<SnesCode>(rom);
    s.measured = rom->data.size();
    l.values = {{"title", rom->title}, {"mapping", std::string(snes::to_string(rom->mapping))}, {"rom_bytes", static_cast<std::int64_t>(rom->data.size())},
                {"banks", static_cast<std::int64_t>(rom->bank_count())}, {"copier", static_cast<std::int64_t>(header != 0)}};
    return l;
}

}  // namespace cb::source::console_writer::snes_code
