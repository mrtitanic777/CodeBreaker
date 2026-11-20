// source/pce_writer.cpp -- a PC Engine / TurboGrafx-16 HuCard as assembly source that builds back into it (pce_writer.hpp).

#include "source/pce_writer.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>
#include <unordered_map>

#include "build/m6502_asm.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/huc6280.hpp"
#include "source/isa_65xx.hpp"

namespace cb::source::pce_writer {

namespace hu = decompile::huc6280;
namespace iw = image_writer;
namespace pce = formats::pce;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
const iw::DataSyntax kData{".byte", ".res", ", "};
constexpr std::uint8_t kUnknown = 0xFF;                      // an MPR nobody has set (the hardware page's number too: both are "not ROM")
constexpr std::uint32_t kVectorsAt = pce::kBankSize - 10;    // the ten vector bytes at the end of bank 0
constexpr std::uint32_t kDefaultBase = 0x4000;               // where a bank the flow never found mapped is shown

std::uint32_t flow_of(std::size_t bank, std::uint32_t logical) { return static_cast<std::uint32_t>(bank) << 16 | logical; }

// What flow knows at an instruction: the physical bank each mapping register holds (kUnknown: not known) and the accumulator when it is a constant.
struct State {
    std::array<std::uint8_t, 8> mpr{kUnknown, kUnknown, kUnknown, kUnknown, kUnknown, kUnknown, kUnknown, kUnknown};
    int a = -1;
};

State initial_state() {
    State s;
    s.mpr[0] = pce::kHardwareBank;
    s.mpr[1] = pce::kRamBank;
    s.mpr[7] = 0;
    return s;
}

struct Insn {
    hu::Insn i;
    State before;    // the mapping the instruction runs under
    State after;     // and what it leaves
    std::uint32_t flow = 0;
};

bool writes_accumulator(const hu::Insn& i) {
    const std::string_view m = i.mnemonic;
    if (m == "TXA" || m == "TYA" || m == "PLA" || m == "ADC" || m == "SBC" || m == "AND" || m == "ORA" || m == "EOR" || m == "SAX" || m == "SAY" || m == "JSR" || m == "BSR") return true;
    return i.mode == hu::Mode::Acc && (m == "ASL" || m == "LSR" || m == "ROL" || m == "ROR" || m == "INC" || m == "DEC");
}

State advance(const hu::Insn& i, const State& before) {
    State after = before;
    const std::string_view m = i.mnemonic;
    if (m == "LDA") {
        after.a = i.mode == hu::Mode::Imm ? i.operand[0] : -1;
    } else if (m == "CLA") {
        after.a = 0;
    } else if (m == "TAM") {
        for (unsigned bit = 0; bit < 8; ++bit) {
            if (i.operand[0] >> bit & 1) after.mpr[bit] = before.a >= 0 ? static_cast<std::uint8_t>(before.a) : kUnknown;
        }
    } else if (m == "TMA") {
        after.a = -1;
        for (unsigned bit = 0; bit < 8; ++bit) {
            if (i.operand[0] >> bit & 1) {
                after.a = before.mpr[bit] == kUnknown ? -1 : before.mpr[bit];
                break;
            }
        }
    } else if (writes_accumulator(i)) {
        after.a = -1;
    }
    return after;
}

bool writes_memory(const hu::Insn& i) {
    const std::string_view m = i.mnemonic;
    if (m == "STA" || m == "STX" || m == "STY" || m == "STZ" || m == "TSB" || m == "TRB") return true;
    return i.mode != hu::Mode::Acc && (m == "INC" || m == "DEC" || m == "ASL" || m == "LSR" || m == "ROL" || m == "ROR");
}

bool zero_page_byte(const hu::Insn& i, std::uint16_t& out) {
    switch (i.mode) {
        case hu::Mode::Zp:
        case hu::Mode::Zpx:
        case hu::Mode::Zpy:
        case hu::Mode::Izp:
        case hu::Mode::Izx:
        case hu::Mode::Izy:
        case hu::Mode::ZpRel: out = i.operand[0]; return true;
        case hu::Mode::ImmZp:
        case hu::Mode::ImmZpx: out = i.operand[1]; return true;
        default: return false;
    }
}

// The 16-bit addresses an instruction's operands name that may stand for something: (value, is it written).
std::vector<std::pair<std::uint16_t, bool>> address_operands(const hu::Insn& i) {
    switch (i.mode) {
        case hu::Mode::Abs:
        case hu::Mode::Abx:
        case hu::Mode::Aby:
        case hu::Mode::Ind:
        case hu::Mode::Iax: return {{i.operand[0], writes_memory(i)}};
        case hu::Mode::ImmAbs:
        case hu::Mode::ImmAbx: return {{i.operand[1], false}};
        case hu::Mode::Block: return {{i.operand[0], false}, {i.operand[1], true}};
        default: return {};
    }
}

// Flow runs through the banks. The state at each address is the first one flow arrived there with; a bank has the one logical page it was first found
// mapped at (a jump to it through another page is the same code at that address).
class PceFlow final : public iw::CodeModel<Insn> {
public:
    explicit PceFlow(const pce::Rom& rom) : rom_(rom) { bases_[0] = pce::kResetBase; }

    void seed(std::uint32_t pc, const State& state) const { pending_.try_emplace(pc, state); }

    std::optional<std::uint32_t> base_of(std::size_t bank) const {
        const auto it = bases_.find(bank);
        if (it == bases_.end()) return std::nullopt;
        return it->second;
    }
    // The address bank `bank` is shown at: the first page it was found at, or the page of `logical` when it is found now.
    std::uint32_t ensure_base(std::size_t bank, std::uint32_t logical) const { return bases_.try_emplace(bank, logical & 0xE000).first->second; }
    std::uint32_t base_or_default(std::size_t bank) const { return base_of(bank).value_or(kDefaultBase); }

    // The flow address of logical address `logical` as `state` maps it, when it is a place in the ROM.
    std::optional<std::uint32_t> locate(const State& state, std::uint32_t logical, bool assign) const {
        const std::optional<std::size_t> bank = rom_.rom_bank(state.mpr[logical >> 13 & 7]);
        if (!bank) return std::nullopt;
        std::uint32_t base = 0;
        if (assign) {
            base = ensure_base(*bank, logical);
        } else if (const auto known = base_of(*bank)) {
            base = *known;
        } else {
            return std::nullopt;
        }
        return flow_of(*bank, base + (logical & 0x1FFF));
    }
    // The same, when the operand written as `logical` and the address shown for it are one value (so a label's name stands for it).
    std::optional<std::uint32_t> exact(const State& state, std::uint32_t logical) const {
        const auto at = locate(state, logical, false);
        if (!at || (*at & 0xFFFF) != logical) return std::nullopt;
        return at;
    }

    std::optional<Insn> decode(std::uint32_t pc) const override {
        const std::size_t bank = pc >> 16;
        const std::optional<std::uint32_t> base = base_of(bank);
        const std::uint32_t logical = pc & 0xFFFF;
        if (!base || logical < *base || logical >= *base + pce::kBankSize || bank >= rom_.bank_count()) return std::nullopt;
        const std::optional<hu::Insn> insn = hu::decode(rom_.bank(bank), logical - *base, logical);
        if (!insn) return std::nullopt;
        State before = initial_state();
        if (const auto it = pending_.find(pc); it != pending_.end()) before = it->second;
        before.mpr[logical >> 13 & 7] = static_cast<std::uint8_t>(bank);
        Insn out{*insn, before, advance(*insn, before), pc};
        if (!hu::ends_flow(insn->mnemonic)) pending_.try_emplace(pc + insn->size, out.after);
        return out;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    std::optional<std::uint32_t> target(const Insn& insn) const override {
        if (!insn.i.target) return std::nullopt;
        const auto to = locate(insn.after, *insn.i.target, true);
        if (!to || ((*to >> 16) == 0 && (*to & 0xFFFF) - pce::kResetBase >= kVectorsAt)) return std::nullopt;   // (the vectors are data)
        pending_.try_emplace(*to, insn.after);
        return to;
    }
    bool inside(std::uint32_t pc) const override {
        const std::size_t bank = pc >> 16;
        const std::optional<std::uint32_t> base = base_of(bank);
        const std::uint32_t logical = pc & 0xFFFF;
        return base && bank < rom_.bank_count() && logical >= *base && logical < *base + pce::kBankSize;
    }
    bool fits(std::uint32_t pc, const Insn& insn) const override {
        const std::uint32_t offset = (pc & 0xFFFF) - base_of(pc >> 16).value_or(0);
        const std::uint32_t limit = (pc >> 16) == 0 ? kVectorsAt : static_cast<std::uint32_t>(pce::kBankSize);   // (the vectors are data)
        return offset + insn.i.size <= limit;
    }
    bool follows(const Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const Insn& insn) const override { return hu::ends_flow(insn.i.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return static_cast<std::uint32_t>((pc >> 16) * pce::kBankSize + ((pc & 0xFFFF) - base_of(pc >> 16).value_or(0))); }
    bool finds_functions() const override { return true; }
    bool starts_function(const Insn& insn) const override { return insn.i.mnemonic == "JSR" || insn.i.mnemonic == "BSR"; }

private:
    const pce::Rom& rom_;
    mutable std::map<std::size_t, std::uint32_t> bases_;
    mutable std::unordered_map<std::uint32_t, State> pending_;
};

// What text() of an instruction needs to name its operands.
struct Naming {
    const PceFlow& flow;
    const iw::Names& labels;                     // by flow address
    const decompile::AddressLabels& zero_page;   // by byte
    std::map<std::uint32_t, std::string>* equates = nullptr;   // the hardware names used (collected when set)
};

std::string insn_text(const Naming& n, const Insn& insn) {
    const hu::Insn& i = insn.i;
    decompile::AddressLabels names;
    const auto name = [&](std::uint16_t value, bool written) {
        if (const auto at = n.flow.exact(insn.before, value)) {
            if (const auto it = n.labels.find(*at); it != n.labels.end()) {
                names[value] = it->second;
                return;
            }
        }
        if (value >= 0x1800 || insn.before.mpr[0] != pce::kHardwareBank) return;
        const auto& table = written ? pce::registers_write() : pce::registers_read();
        if (const auto it = table.find(value); it != table.end()) {
            names[value] = it->second;
            if (n.equates) (*n.equates)[value] = it->second;
        }
    };
    for (const auto& [value, written] : address_operands(i)) name(value, written);
    if (i.target) name(*i.target, false);
    std::string out = hu::text(i, names, n.zero_page);
    if (i.mnemonic == "ST0") out += "  ; VDC " + pce::vdc_register_name(i.operand[0] & 0x1F);
    return out;
}

class PceListing final : public iw::Listing<Insn> {
public:
    PceListing(const pce::Rom& rom, std::size_t bank, std::uint32_t base, const std::map<std::uint32_t, Insn>& code, const std::set<std::uint32_t>& demoted,
               const Naming& naming, translate::l2::Assembly& out)
        : rom_(rom), bank_(bank), base_(base), code_(code), demoted_(demoted), naming_(naming), out_(out) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return flow_of(bank_, base_ + static_cast<std::uint32_t>(offset - bank_ * pce::kBankSize)); }
    const Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        if (it == code_.end() || demoted_.contains(offset)) return nullptr;
        if (offset + it->second.i.size > (bank_ + 1) * pce::kBankSize) return nullptr;
        return &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    std::string text(const Insn& insn) const override { return insn_text(naming_, insn); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kData); }

    // The ten vector bytes: IRQ2 (and BRK), IRQ1, TIMER, NMI, RESET.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (bank_ != 0 || offset != kVectorsAt) return 0;
        flush();
        const pce::Vectors& v = rom_.vectors;
        std::vector<std::string> words;
        for (const std::uint16_t at : {v.irq2, v.irq1, v.timer, v.nmi, v.reset}) {
            const auto named = naming_.labels.find(flow_of(0, at));
            words.push_back(named != naming_.labels.end() && at >= pce::kResetBase ? named->second : std::format("${:04X}", at));
        }
        out_.append("        .word " + text::join(words, ", "));
        return 10;
    }

private:
    const pce::Rom& rom_;
    std::size_t bank_;
    std::uint32_t base_;
    const std::map<std::uint32_t, Insn>& code_;
    const std::set<std::uint32_t>& demoted_;
    const Naming& naming_;
    translate::l2::Assembly& out_;
};

std::string link_config(const pce::Rom& rom, const PceFlow& flow) {
    std::vector<std::string> cfg = {"MEMORY {"};
    if (!rom.header.empty()) cfg.push_back(std::format("    HEADER: start = $0000, size = ${:04X}, fill = yes, file = %O;", pce::kHeaderSize));
    for (std::size_t b = 0; b < rom.bank_count(); ++b) cfg.push_back(std::format("    BANK{}: start = ${:04X}, size = ${:04X}, fill = yes, file = %O;", b, flow.base_or_default(b), pce::kBankSize));
    cfg.insert(cfg.end(), {"}", "SEGMENTS {"});
    if (!rom.header.empty()) cfg.push_back("    HEADER: load = HEADER, type = ro;");
    for (std::size_t b = 0; b < rom.bank_count(); ++b) cfg.push_back(std::format("    BANK{}: load = BANK{}, type = ro;", b, b));
    cfg.push_back("}");
    return text::join(cfg, "\n") + "\n";
}

}  // namespace

iw::Written write(const pce::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const PceFlow flow(rom);

    // the vectors are the entries, each with the mapping a reset leaves
    std::vector<std::uint32_t> entries;
    iw::Names names;
    const pce::Vectors& v = rom.vectors;
    for (const auto& [vector, label] : {std::pair{v.reset, "reset"}, std::pair{v.nmi, "nmi"}, std::pair{v.irq1, "irq1"}, std::pair{v.irq2, "irq2"}, std::pair{v.timer, "timer"}}) {
        if (vector < pce::kResetBase) continue;
        const std::uint32_t at = flow_of(0, vector);
        flow.seed(at, initial_state());
        if (std::find(entries.begin(), entries.end(), at) == entries.end()) entries.push_back(at);
        names.try_emplace(at, label);
    }
    const iw::Walked<Insn> walked = iw::walk<Insn>(flow, entries);
    const std::map<std::uint32_t, Insn>& walked_code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);

    // a bank an instruction names data in is shown at the page it was named through
    for (const auto& entry : walked_code) {
        for (const auto& [value, written] : address_operands(entry.second.i)) {
            (void)written;
            flow.locate(entry.second.before, value, true);
        }
    }

    // where a line can start: any byte of a bank not inside an instruction
    std::map<std::uint32_t, Insn> code = walked_code;
    std::set<std::uint32_t> demoted;
    const auto line_starts = [&](const std::map<std::uint32_t, Insn>& c, const std::set<std::uint32_t>& gone) {
        std::set<std::uint32_t> inside;
        for (const auto& [at, insn] : c) {
            if (gone.contains(at)) continue;
            for (std::uint32_t a = at + 1; a < at + insn.i.size; ++a) inside.insert(a);
        }
        std::set<std::uint32_t> lines;
        for (std::size_t b = 0; b < rom.bank_count(); ++b) {
            const std::uint32_t base = flow.base_or_default(b);
            for (std::uint32_t off = 0; off < pce::kBankSize; ++off) {
                if (!inside.contains(static_cast<std::uint32_t>(b * pce::kBankSize + off))) lines.insert(flow_of(b, base + off));
            }
        }
        return lines;
    };
    const std::set<std::uint32_t> line_at = line_starts(code, demoted);
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
    for (const auto& entry : code) {   // data an instruction names, where a line starts
        for (const auto& [value, written] : address_operands(entry.second.i)) {
            (void)written;
            if (const auto at = flow.exact(entry.second.before, value); at && line_at.contains(*at)) labels.try_emplace(*at, std::format("D_{:06X}", *at));
        }
    }
    decompile::AddressLabels zero_page;
    for (const auto& entry : code) {
        if (std::uint16_t byte = 0; zero_page_byte(entry.second.i, byte)) zero_page[byte] = std::format("zp_{:02X}", byte);
    }

    // the text of each instruction; one that does not assemble back into its bytes is data
    std::map<std::uint32_t, std::string> hardware;
    Naming naming{flow, labels, zero_page, &hardware};
    for (const auto& entry : code) (void)insn_text(naming, entry.second);   // (collects the hardware names used)
    decompile::NameLabels values;
    for (const auto& [at, label] : labels) values[label] = at & 0xFFFF;
    for (const auto& [value, label] : hardware) values[label] = value;
    for (const auto& [byte, label] : zero_page) values[label] = byte;
    naming.equates = nullptr;
    for (const auto& [offset, insn] : code) {
        const std::uint32_t bank = offset / pce::kBankSize;
        const std::uint32_t logical = flow.base_or_default(bank) + offset % pce::kBankSize;
        bool ok = false;
        try {
            const Bytes built = hu::assemble(insn_text(naming, insn), logical, values);
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
                                     std::format("; PC Engine / TurboGrafx-16 HuCard: {} banks of 8 KB{}", rom.bank_count(), rom.header.empty() ? "" : ", with a 512-byte copier header"),
                                     "; each BANKn segment is one 8 KB bank, linked at the address the code that maps it expects", "",
                                     "        .setcpu \"HuC6280\"", ""};
    for (const auto& [value, label] : hardware) head.push_back(std::format("{:<14} = ${:04X}", label, value));
    if (!hardware.empty()) head.push_back("");
    for (const auto& [byte, label] : zero_page) head.push_back(std::format("{:<14} = ${:02X}", label, byte));
    if (!zero_page.empty()) head.push_back("");
    translate::l2::Assembly out(head, "ca65", std::string(file_name));

    if (!rom.header.empty()) {
        out.part("HEADER", "segment", std::nullopt, pce::kHeaderSize);
        out.append(".segment \"HEADER\"");
        out.extend(iw::commented(iw::data_lines(rom.header, kData), "the copier header"));
        out.append("");
    }
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        out.part(std::format("BANK{}", b), "segment", std::nullopt, pce::kBankSize);
        out.append(std::format(".segment \"BANK{}\"", b));
        PceListing listing(rom, b, flow.base_or_default(b), code, demoted, naming, out);
        iw::emit<Insn>(out, static_cast<std::uint32_t>(b * pce::kBankSize), static_cast<std::uint32_t>((b + 1) * pce::kBankSize), labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    const std::string config = link_config(rom, flow);
    const std::filesystem::path config_path = folder / (stem + ".cfg");
    fs::write_file(config_path, as_bytes(config));

    iw::Written r = iw::finish(folder, stem + ".s", source, functions, code_total, rom.data.size());
    r.config = config_path;
    Bytes original = rom.header;
    original.insert(original.end(), rom.data.begin(), rom.data.end());
    const build::m6502_asm::Assembler assembler(isa::huc6280_encoder());
    iw::assembled(r, [&] { return assembler.build(source, config, folder); }, original, folder / (stem + ".built.pce"));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(pce::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::pce_writer
