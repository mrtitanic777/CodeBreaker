// cb/source/snes_writer.cpp -- a Super Nintendo cartridge as assembly source that builds back into it (snes_writer.hpp).

#include "cb/source/snes_writer.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>
#include <unordered_map>

#include "cb/build/m6502_asm.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/w65816.hpp"
#include "cb/source/isa_65xx.hpp"

namespace cb::source::snes_writer {

namespace w = decompile::w65816;
namespace iw = image_writer;
namespace snes = formats::snes;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
const iw::DataSyntax kData{".byte", ".res", ", "};

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

std::string insn_text(const Naming& n, const Insn& insn, bool directives, State* assembler = nullptr) {
    const w::Insn& i = insn.i;
    decompile::AddressLabels names;
    // the name of the label at the place `full` shows in the ROM, when its address is `full` itself (`must_equal`) or has the same low 16 bits
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
            if (i.target) {
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
    std::string out = w::text(i, names, n.direct_page);
    if (directives && assembler) {   // the register width an immediate operand has, when the assembler is not at it
        std::string prefix;
        if (i.mode == w::Mode::ImmM && i.m16 != (assembler->m16)) {
            prefix += i.m16 ? ".a16\n        " : ".a8\n        ";
            assembler->m16 = i.m16;
        }
        if (i.mode == w::Mode::ImmX && i.x16 != (assembler->x16)) {
            prefix += i.x16 ? ".i16\n        " : ".i8\n        ";
            assembler->x16 = i.x16;
        }
        out = prefix + out;
    }
    return out;
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

std::string word_list(const std::vector<std::string>& words) { return "        .word " + text::join(words, ", "); }

class SnesListing final : public iw::Listing<Insn> {
public:
    SnesListing(const snes::Rom& rom, bool high, std::size_t first_offset, const std::map<std::uint32_t, Insn>& code, const std::set<std::uint32_t>& demoted,
                const Naming& naming, State& assembler, translate::l2::Assembly& out)
        : rom_(rom), high_(high), first_(first_offset), code_(code), demoted_(demoted), naming_(naming), assembler_(assembler), out_(out) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address_of(offset, high_); }
    const Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        if (it == code_.end() || demoted_.contains(offset)) return nullptr;
        if (offset + it->second.i.size > first_ + rom_.bank_size()) return nullptr;
        return &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    std::string text(const Insn& insn) const override { return insn_text(naming_, insn, true, &assembler_); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kData); }

    // The internal header, field by field; a vector that is a label of bank 0 is the label.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != rom_.header_offset) return 0;
        flush();
        const ByteView h = ByteView(rom_.data).subspan(offset, snes::kInternalHeaderSize);
        const auto hex = [&](std::size_t from, std::size_t count) {
            std::vector<std::string> v;
            for (std::size_t k = 0; k < count; ++k) v.push_back(std::format("${:02X}", h[from + k]));
            return "        .byte " + text::join(v, ", ");
        };
        const auto vector_words = [&](std::size_t from) {
            std::vector<std::string> v;
            for (std::size_t k = 0; k < 6; ++k) {
                const std::uint16_t at = static_cast<std::uint16_t>(h[from + 2 * k] | h[from + 2 * k + 1] << 8);
                std::string text = std::format("${:04X}", at);
                if (const auto o = rom_.offset(at); o && at >= 0x8000) {
                    const auto named = naming_.labels.find(rom_.address_of(*o, high_));
                    if (named != naming_.labels.end() && (rom_.address_of(*o, high_) & 0xFFFF0000) == 0) text = named->second;
                }
                v.push_back(text);
            }
            return word_list(v);
        };
        out_.append("        ; the internal header");
        for (const std::string& line : iw::data_lines(h.subspan(0, 21), kData)) out_.append(line);
        out_.append(hex(0x15, 7) + "   ; map mode, chipset, ROM size, RAM size, country, developer, version");
        out_.append(std::format("        .word ${:04X}, ${:04X}   ; checksum complement, checksum", rom_.complement, rom_.checksum));
        out_.append(hex(0x20, 4));
        out_.append(vector_words(0x24) + "   ; native mode: COP, BRK, ABORT, NMI, -, IRQ");
        out_.append(hex(0x30, 4));
        out_.append(vector_words(0x34) + "   ; emulation mode: COP, -, ABORT, NMI, RESET, IRQ");
        return static_cast<std::uint32_t>(snes::kInternalHeaderSize);
    }

private:
    const snes::Rom& rom_;
    bool high_;
    std::size_t first_;
    const std::map<std::uint32_t, Insn>& code_;
    const std::set<std::uint32_t>& demoted_;
    const Naming& naming_;
    State& assembler_;
    translate::l2::Assembly& out_;
};

std::string link_config(const snes::Rom& rom, bool high) {
    std::vector<std::string> cfg = {"MEMORY {"};
    if (!rom.copier_header.empty()) cfg.push_back(std::format("    HEADER: start = $000000, size = ${:04X}, fill = yes, file = %O;", snes::kCopierHeaderSize));
    const std::size_t banks = rom.bank_count();
    for (std::size_t b = 0; b < banks; ++b) {
        const std::uint32_t start = rom.address_of(static_cast<std::uint32_t>(b * rom.bank_size()), high);
        const std::size_t size = std::min(rom.bank_size(), rom.data.size() - b * rom.bank_size());
        cfg.push_back(std::format("    BANK{}: start = ${:06X}, size = ${:04X}, fill = yes, file = %O;", b, start, size));
    }
    cfg.insert(cfg.end(), {"}", "SEGMENTS {"});
    if (!rom.copier_header.empty()) cfg.push_back("    HEADER: load = HEADER, type = ro;");
    for (std::size_t b = 0; b < banks; ++b) cfg.push_back(std::format("    BANK{}: load = BANK{}, type = ro;", b, b));
    cfg.push_back("}");
    return text::join(cfg, "\n") + "\n";
}

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

}  // namespace

iw::Written write(const snes::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();

    // LoROM's long addresses are written in one of two mirrors, $00 or $80: the one more of the program's own long operands use
    bool high = false;
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
        high = high_votes > low_votes;
    }
    const SnesFlow flow(rom, high);
    Found found = find_code(rom, flow);
    const std::map<std::uint32_t, Insn>& walked_code = found.walked.code;
    iw::Names names = found.names;
    const iw::Functions functions = iw::name_functions(found.walked.starts, names, kSub);

    // where a line can start: any byte not inside an instruction
    std::set<std::uint32_t> inside;
    for (const auto& [offset, insn] : walked_code) {
        for (std::uint32_t k = 1; k < insn.i.size; ++k) inside.insert(offset + k);
    }
    std::set<std::uint32_t> line_at;
    for (std::uint32_t offset = 0; offset < rom.data.size(); ++offset) {
        if (!inside.contains(offset)) line_at.insert(rom.address_of(offset, high));
    }
    iw::Names labels = iw::make_labels(found.walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
    std::map<std::uint32_t, Insn> code = walked_code;
    // data an instruction names where a line starts (a 16-bit operand is in the instruction's own bank)
    for (const auto& entry : code) {
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
            if (line_at.contains(canonical) && !in_header) labels.try_emplace(canonical, std::format("D_{:06X}", canonical));
        }
    }
    decompile::AddressLabels direct_page;
    for (const auto& entry : code) {
        if (std::uint32_t byte = 0; direct_page_byte(entry.second.i, byte)) direct_page[byte] = std::format("dp_{:02X}", byte);
    }

    // the text of each instruction; one that does not assemble back into its bytes is data
    std::map<std::uint32_t, std::string> hardware;
    Naming full_naming{rom, high, labels, direct_page, &hardware};
    for (const auto& entry : code) (void)insn_text(full_naming, entry.second, false);   // (collects the hardware names used)
    decompile::NameLabels values;
    for (const auto& [at, label] : labels) values[label] = at;
    for (const auto& [value, label] : hardware) values[label] = value;
    for (const auto& [byte, label] : direct_page) values[label] = byte;
    full_naming.equates = nullptr;
    std::set<std::uint32_t> demoted;
    for (const auto& [offset, insn] : code) {
        bool ok = false;
        try {
            const Bytes built = w::assemble(insn_text(full_naming, insn, false), insn.i.at, values, insn.i.m16, insn.i.x16);
            ok = built.size() == insn.i.size && std::equal(built.begin(), built.end(), rom.data.begin() + offset);
        } catch (const ParseError&) {
        }
        if (!ok) demoted.insert(offset);
    }
    std::size_t code_total = 0;
    for (const auto& entry : code) {
        if (!demoted.contains(entry.first)) code_total += entry.second.i.size;
    }

    std::vector<std::string> head = {std::format("; {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("; Super Nintendo cartridge \"{}\": {}, {} bytes of ROM in {} banks{}", rom.title, snes::to_string(rom.mapping), rom.data.size(), rom.bank_count(),
                                                 rom.copier_header.empty() ? "" : ", with a 512-byte copier header"),
                                     "; each BANKn segment is one bank, linked at the address the code runs at; the SPC700 sound program is data", "",
                                     "        .p816", ""};
    for (const auto& [value, label] : hardware) head.push_back(std::format("{:<14} = ${:04X}", label, value));
    if (!hardware.empty()) head.push_back("");
    for (const auto& [byte, label] : direct_page) head.push_back(std::format("{:<14} = ${:02X}", label, byte));
    if (!direct_page.empty()) head.push_back("");
    translate::l2::Assembly out(head, "ca65", std::string(file_name));

    if (!rom.copier_header.empty()) {
        out.part("HEADER", "segment", std::nullopt, snes::kCopierHeaderSize);
        out.append(".segment \"HEADER\"");
        out.extend(iw::commented(iw::data_lines(rom.copier_header, kData), "the copier header"));
        out.append("");
    }
    State assembler;   // what `.a8` .. `.i16` the assembler is at: 8 bits until said
    assembler.m16 = assembler.x16 = false;
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        const std::size_t first = b * rom.bank_size();
        const std::size_t last = std::min(rom.data.size(), first + rom.bank_size());
        out.part(std::format("BANK{}", b), "segment", std::nullopt, last - first);
        out.append(std::format(".segment \"BANK{}\"", b));
        SnesListing listing(rom, high, first, code, demoted, full_naming, assembler, out);
        iw::emit<Insn>(out, static_cast<std::uint32_t>(first), static_cast<std::uint32_t>(last), labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    const std::string config = link_config(rom, high);
    const std::filesystem::path config_path = folder / (stem + ".cfg");
    fs::write_file(config_path, as_bytes(config));

    // the functions as the lane runs them: with the state each starts in
    iw::Functions with_state;
    for (const auto& [at, label] : functions) {
        const State s = flow.state_at(at).value_or(State{});
        std::uint32_t key = at & kAddressMask;
        if (!s.e && s.m16) key |= kStartsWith16BitAccumulator;
        if (!s.e && s.x16) key |= kStartsWith16BitIndex;
        if (!s.e) key |= kStartsNative;
        with_state[key] = label;
    }
    iw::Written r = iw::finish(folder, stem + ".s", source, with_state, code_total, rom.data.size());
    r.config = config_path;
    Bytes original = rom.copier_header;
    original.insert(original.end(), rom.data.begin(), rom.data.end());
    const build::m6502_asm::Assembler assembler_of_the_source(isa::w65816_encoder());
    iw::assembled(r, [&] { return assembler_of_the_source.build(source, config, folder); }, original, folder / (stem + ".built.sfc"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(snes::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::snes_writer
