// source/console_pce.cpp -- the console writer's PC Engine (config/platforms/pce.json): a HuCard on the `segmented` shape, with the flow the
// shape cannot do alone (console_writer.hpp: SegmentCode). The HuC6280 maps 8 KB banks into eight pages through its MPRs, so flow tracks
// `LDA #n` and `TAM #mask` through the code: an address is `bank << 16 | logical address`, and each bank is linked at the page flow first
// found it mapped at (bank 0 at $E000; another bank never found mapped at $4000). Code is what flow reaches from the vectors (RESET, NMI,
// IRQ1, IRQ2, TIMER); a jump into a page whose mapping is not known from the code that reaches it is not followed. Function starts are
// `reset`, `nmi`, `irq1`, `irq2`, `timer` or `sub_<bank><address>`; jump targets `L_<bank><address>`; a data address an instruction names
// `D_<bank><address>`. The chips' registers carry the hardware names, the zero page (the RAM at $2000) `zp_20`, the ten vector bytes are
// `.word irq2, irq1, timer, nmi, reset`. An instruction whose text does not assemble back into its own bytes is written as `.byte` data.

#include "source/console_shapes.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/huc6280.hpp"
#include "formats/pce.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer::pce_code {

namespace hu = decompile::huc6280;
namespace iw = image_writer;
namespace pce = formats::pce;

namespace {

// The PC Engine's source is ca65's, with .res fills and string rows (config/dialects/ca65-res.json).
const dialect::Dialect* g_said = nullptr;   // (the description's dialect, set by follow(): config/dialects/ca65-res.json)
const dialect::Dialect& ca65() { return *g_said; }

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
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
    std::set<std::pair<std::uint32_t, std::string>>* equates = nullptr;   // the hardware names used (collected when set; a register read and written has a name each)
};

// The names of the numbers an instruction has: its labels, the hardware registers and the zero-page bytes it names.
decompile::AddressLabels insn_names(const Naming& n, const Insn& insn) {
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
            if (n.equates) n.equates->insert({value, it->second});
        }
    };
    for (const auto& [value, written] : address_operands(i)) name(value, written);
    if (i.target) name(*i.target, false);
    if (std::uint16_t byte = 0; zero_page_byte(i, byte)) {
        if (const auto it = n.zero_page.find(byte); it != n.zero_page.end()) names[byte] = it->second;
    }
    return names;
}

// The instruction as a line: the description's reading `described` with its names put in.
std::string insn_text(const Naming& n, const Insn& insn, const std::string& described) {
    const hu::Insn& i = insn.i;
    std::string out = described_source::name_numbers(described, insn_names(n, insn));
    if (i.mnemonic == "ST0") out += "  ; VDC " + pce::vdc_register_name(i.operand[0] & 0x1F);
    return out;
}

class PceListing final : public iw::Listing<Insn> {
public:
    PceListing(const pce::Rom& rom, std::size_t bank, std::uint32_t base, const std::map<std::uint32_t, Insn>& code, const std::set<std::uint32_t>& demoted,
               const std::map<std::uint32_t, std::string>& described, const Naming& naming, translate::l2::Assembly& out)
        : rom_(rom), bank_(bank), base_(base), code_(code), demoted_(demoted), described_(described), naming_(naming), out_(out) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return flow_of(bank_, base_ + static_cast<std::uint32_t>(offset - bank_ * pce::kBankSize)); }
    const Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        if (offset + it->second.i.size > (bank_ + 1) * pce::kBankSize) return nullptr;
        at_offset_ = offset;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.i.size; }
    std::string text(const Insn& insn) const override {
        if (const auto it = described_.find(at_offset_); it != described_.end()) return insn_text(naming_, insn, it->second);
        // (a text the description cannot say back: the bytes, with the description's reading in a comment)
        const decompile::AddressLabels names = insn_names(naming_, insn);
        return m6502_source::in_dialect_absolute(ca65(), insn.i.mnemonic, insn.i.mode == hu::Mode::Abs, insn.i.operand[0], names,
                                                 m6502_source::code(m6502_source::Cpu::Huc6280, ByteView(rom_.data).subspan(at_offset_, insn.i.size), base_ + at_offset_ % pce::kBankSize, names));
    }
    std::vector<std::string> data_lines(ByteView data) const override { return ca65().data_lines(data); }

    // The ten vector bytes: IRQ2 (and BRK), IRQ1, TIMER, NMI, RESET.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (bank_ != 0 || offset != kVectorsAt) return 0;
        flush();
        const pce::Vectors& v = rom_.vectors;
        std::vector<std::string> words;
        for (const std::uint16_t at : {v.irq2, v.irq1, v.timer, v.nmi, v.reset}) {
            const auto named = naming_.labels.find(flow_of(0, at));
            words.push_back(named != naming_.labels.end() && at >= pce::kResetBase ? named->second : std::format("0x{:04x}", at));
        }
        out_.append("        .half " + text::join(words, ","));
        return 10;
    }

private:
    mutable std::uint32_t at_offset_ = 0;   // (the file offset of the instruction code_at() last gave)
    const pce::Rom& rom_;
    std::size_t bank_;
    std::uint32_t base_;
    const std::map<std::uint32_t, Insn>& code_;
    const std::set<std::uint32_t>& demoted_;
    const std::map<std::uint32_t, std::string>& described_;   // by file offset: the description's text of each instruction written as one
    const Naming& naming_;
    translate::l2::Assembly& out_;
};



// The PC Engine's code: flow through the banks as the code maps them (the bank's page decided where flow first finds it).
class PceCode final : public SegmentCode {
public:
    explicit PceCode(std::shared_ptr<const pce::Rom> rom) : rom_(std::move(rom)), flow_(*rom_) {}

    void follow(const Description& d) override {
        g_said = &dialect::find(d.dialect);
        const pce::Rom& rom = *rom_;
        const PceFlow& flow = flow_;
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
        functions_ = iw::name_functions(walked.starts, names, kSub);

        // a bank an instruction names data in is shown at the page it was named through
        for (const auto& entry : walked.code) {
            for (const auto& [value, written] : address_operands(entry.second.i)) {
                (void)written;
                flow.locate(entry.second.before, value, true);
            }
        }

        // where a line can start: any byte of a bank not inside an instruction
        code_ = walked.code;
        std::set<std::uint32_t> inside;
        for (const auto& [at, insn] : code_) {
            for (std::uint32_t a = at + 1; a < at + insn.i.size; ++a) inside.insert(a);
        }
        std::set<std::uint32_t> line_at;
        for (std::size_t b = 0; b < rom.bank_count(); ++b) {
            const std::uint32_t base = flow.base_or_default(b);
            for (std::uint32_t off = 0; off < pce::kBankSize; ++off) {
                if (!inside.contains(static_cast<std::uint32_t>(b * pce::kBankSize + off))) line_at.insert(flow_of(b, base + off));
            }
        }
        labels_ = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kLocal);
        for (const auto& entry : code_) {   // data an instruction names, where a line starts
            for (const auto& [value, written] : address_operands(entry.second.i)) {
                (void)written;
                if (const auto at = flow.exact(entry.second.before, value); at && line_at.contains(*at)) labels_.try_emplace(*at, std::format("D_{:06X}", *at));
            }
        }
        for (const auto& entry : code_) {
            if (std::uint16_t byte = 0; zero_page_byte(entry.second.i, byte)) zero_page_[byte] = std::format("zp_{:02X}", byte);
        }

        // the text of each instruction; one that does not assemble back into its bytes is data
        naming_ = std::make_unique<Naming>(Naming{flow_, labels_, zero_page_, &hardware_});
        for (const auto& entry : code_) (void)insn_names(*naming_, entry.second);   // (collects the hardware names used)
        naming_->equates = nullptr;
        for (const auto& [offset, insn] : code_) {
            const std::uint32_t bank = offset / pce::kBankSize;
            const std::uint32_t logical = flow.base_or_default(bank) + offset % pce::kBankSize;
            if (const auto text = m6502_source::described(m6502_source::Cpu::Huc6280, ByteView(rom.data).subspan(offset, insn.i.size), logical)) described_[offset] = *text;
            else if (const std::string forced = insn.i.mode == hu::Mode::Abs && insn.i.operand[0] < 0x100
                                                    ? ca65().forced_absolute(insn.i.mnemonic, std::format("0x{:x}", insn.i.operand[0]))
                                                    : std::string();
                     !forced.empty())
                described_[offset] = forced;   // (the absolute form of a zero-page address: `STA a:x` in ca65's, which the description's text cannot say)
            else demoted_.insert(offset);
        }
        for (const auto& entry : code_) {
            if (!demoted_.contains(entry.first)) code_total_ += entry.second.i.size;
        }
    }
    iw::Functions functions() const override { return functions_; }
    std::size_t code_bytes() const override { return code_total_; }
    std::vector<std::string> names() const override {
        std::vector<std::string> lines;
        for (const auto& [value, label] : hardware_) lines.push_back(ca65().line(m6502_source::equ(label, value, 4), 14));
        if (!hardware_.empty()) lines.push_back("");
        for (const auto& [byte, label] : zero_page_) lines.push_back(ca65().line(m6502_source::equ(label, byte, 2), 14));
        if (!zero_page_.empty()) lines.push_back("");
        return lines;
    }
    std::uint32_t linked_at(const Piece& piece) const override { return flow_.base_or_default(bank_of(piece)); }
    void write(translate::l2::Assembly& out, const Piece& piece) override {
        const std::size_t b = bank_of(piece);
        PceListing listing(*rom_, b, flow_.base_or_default(b), code_, demoted_, described_, *naming_, out);
        iw::emit<Insn>(out, static_cast<std::uint32_t>(b * pce::kBankSize), static_cast<std::uint32_t>((b + 1) * pce::kBankSize), labels_, functions_, listing);
    }

private:
    std::size_t bank_of(const Piece& piece) const { return (piece.offset - rom_->header.size()) / pce::kBankSize; }

    std::shared_ptr<const pce::Rom> rom_;
    PceFlow flow_;
    iw::Functions functions_;
    std::map<std::uint32_t, Insn> code_;
    iw::Names labels_;
    decompile::AddressLabels zero_page_;
    std::set<std::pair<std::uint32_t, std::string>> hardware_;
    std::unique_ptr<Naming> naming_;
    std::set<std::uint32_t> demoted_;
    std::map<std::uint32_t, std::string> described_;
    std::size_t code_total_ = 0;
};

}  // namespace

Loaded load_pce_file(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const pce::Rom>(pce::read(file));
    Loaded l;
    l.image = rom->header;
    l.image.insert(l.image.end(), rom->data.begin(), rom->data.end());
    Segments& s = l.segments;
    const std::uint32_t header = static_cast<std::uint32_t>(rom->header.size());
    if (header) s.pieces.push_back({"HEADER", "HEADER", "copier", 0, header, 0, false});
    for (std::size_t b = 0; b < rom->bank_count(); ++b) {
        const std::string name = std::format("BANK{}", b);
        s.pieces.push_back({name, name, "", header + static_cast<std::uint32_t>(b * pce::kBankSize), static_cast<std::uint32_t>(pce::kBankSize), 0, true});
    }
    s.code = std::make_shared<PceCode>(rom);
    s.measured = rom->data.size();
    l.values = {{"banks", static_cast<std::int64_t>(rom->bank_count())}, {"copier", static_cast<std::int64_t>(header != 0)}};
    return l;
}

}  // namespace cb::source::console_writer::pce_code
