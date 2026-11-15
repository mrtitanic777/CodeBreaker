// cb/source/sh2_writer.cpp -- SH-2 programs as assembly source that builds back into them (sh2_writer.hpp).

#include "cb/source/sh2_writer.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <optional>
#include <set>
#include <unordered_map>

#include "cb/build/sh_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/sh2.hpp"
#include "cb/source/isa_sh2.hpp"

namespace cb::source::sh2_writer {

namespace dm = decompile::sh2;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr iw::Spelling kConstant{"K_", 8};
constexpr unsigned kStringMin = 4;           // a run of printable bytes this long is a string
constexpr unsigned kStringMax = 96;
constexpr std::size_t kFillMin = 16;         // a run of one value this long is `.space` / `.fill`
constexpr unsigned kRounds = 4;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 400;     // how many instructions a function is looked at for before it is called data
constexpr std::uint32_t kGapReach = 0x200;   // how far past a function's end the next one is looked for (constants stand between)

// An instruction, and whether it is the delay slot of a branch that ends the flow (so the flow ends after it).
struct ShInsn {
    dm::Insn insn;
    bool ends = false;
};

bool ends_flow(const dm::Insn& insn) {
    return insn.delay_slot && (insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Return || insn.flow == dm::Flow::Indirect);
}

std::uint16_t half_at(ByteView buf, std::size_t at, bool little) {
    return little ? static_cast<std::uint16_t>(buf[at + 1] << 8 | buf[at]) : static_cast<std::uint16_t>(buf[at] << 8 | buf[at + 1]);
}

std::uint32_t word_at(ByteView buf, std::size_t at, bool little) {
    if (little) return static_cast<std::uint32_t>(buf[at + 3]) << 24 | static_cast<std::uint32_t>(buf[at + 2]) << 16 | static_cast<std::uint32_t>(buf[at + 1]) << 8 | buf[at];
    return static_cast<std::uint32_t>(buf[at]) << 24 | static_cast<std::uint32_t>(buf[at + 1]) << 16 | static_cast<std::uint32_t>(buf[at + 2]) << 8 | buf[at + 3];
}

// Where the image lies in memory.
class Layout {
public:
    explicit Layout(const Image& image) : image_(image), variant_(image.sh4 ? dm::Variant::Sh4 : dm::Variant::Sh2) {}
    dm::Variant variant() const { return variant_; }

    const Part* part_at(std::uint32_t address) const {
        for (const Part& p : image_.parts)
            if (p.mapped && address >= p.base && address - p.base < p.length) return &p;
        return nullptr;
    }
    bool in_image(std::uint32_t address) const { return part_at(address) != nullptr; }
    bool in_code(std::uint32_t address) const {
        const Part* p = part_at(address);
        return p != nullptr && p->code;
    }
    // The image offset of an address (the address must be in a part).
    std::uint32_t offset_of(std::uint32_t address) const {
        const Part* p = part_at(address);
        return p->offset + (address - p->base);
    }
    std::uint32_t address_of(const Part& p, std::uint32_t offset) const { return p.base + (offset - p.offset); }
    ByteView bytes() const { return image_.bytes; }
    // The 16 and 32-bit values at an address (big-endian for the SH-2, little-endian for the SH-4).
    std::uint16_t half(std::uint32_t address) const { return half_at(image_.bytes, offset_of(address), dm::little_endian(variant_)); }
    std::uint32_t word(std::uint32_t address) const { return word_at(image_.bytes, offset_of(address), dm::little_endian(variant_)); }
    // Do `length` bytes from `address` lie in one part?
    bool holds(std::uint32_t address, std::uint32_t length) const {
        const Part* p = part_at(address);
        return p != nullptr && address + length <= p->base + p->length && address + length > address;
    }

private:
    const Image& image_;
    dm::Variant variant_;
};

// Flow runs through the code parts at even addresses.
class ShFlow final : public iw::CodeModel<ShInsn> {
public:
    explicit ShFlow(const Layout& layout) : layout_(layout) {}

    std::optional<ShInsn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        const Part* part = layout_.part_at(pc);
        std::optional<dm::Insn> insn = dm::decode(layout_.bytes(), layout_.offset_of(pc), pc, layout_.variant());
        if (!insn) return std::nullopt;
        ShInsn out{*insn, false};
        if (pc - part->base >= 2) {
            const std::optional<dm::Insn> before = dm::decode(layout_.bytes(), layout_.offset_of(pc) - 2, pc - 2, layout_.variant());
            out.ends = before && ends_flow(*before);
        }
        return out;
    }
    std::uint32_t size(const ShInsn&) const override { return 2; }
    std::optional<std::uint32_t> target(const ShInsn& p) const override { return p.insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && layout_.in_code(pc) && layout_.holds(pc, 2); }
    bool fits(std::uint32_t pc, const ShInsn&) const override { return inside(pc); }
    bool follows(const ShInsn& p) const override { return p.insn.target && inside(*p.insn.target) && p.insn.flow != dm::Flow::None; }
    bool stops(const ShInsn& p) const override { return p.ends; }
    bool finds_functions() const override { return true; }
    bool starts_function(const ShInsn& p) const override { return p.insn.mnemonic == "bsr"; }

private:
    const Layout& layout_;
};

// How GCC opens a function that calls others or keeps a frame: it saves registers (`mov.l r8..r14,@-r15`, `sts.l pr,@-r15`) or opens the frame.
bool is_prologue(std::uint16_t w) {
    if ((w & 0xFF0F) == 0x2F06) return ((w >> 4) & 15) >= 8;   // mov.l rm,@-r15 for rm = r8 .. r14
    if (w == 0x4F22) return true;                               // sts.l pr,@-r15
    return (w & 0xFF80) == 0x7F80;                              // add #-n,r15
}

// Do the bytes at `at` read as a function: instructions that all decode, whose branches stay in the program, that end in a branch that
// leaves the flow (and its delay slot) within a few hundred words? Data does not: random words stop decoding, and zeros never end.
bool reads_as_function(const Layout& layout, std::uint32_t at) {
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 2 * count;
        if (!layout.in_code(pc) || !layout.holds(pc, 2)) return false;
        const std::optional<dm::Insn> insn = dm::decode(layout.bytes(), layout.offset_of(pc), pc, layout.variant());
        if (!insn) return false;
        if (insn->target && (!layout.in_code(*insn->target) || *insn->target % 2 != 0) && insn->mnemonic != "bsr") return false;
        if (!ends_flow(*insn)) continue;
        if (!layout.in_code(pc + 2) || !layout.holds(pc + 2, 2) || !dm::decode(layout.bytes(), layout.offset_of(pc + 2), pc + 2, layout.variant())) return false;
        return count >= 1;
    }
    return false;
}

// The address a program's pointer means in the image's terms: the cache and mirror bits dropped (the SH-2's top three bits; the SH-4's P0-P3 views), but not the chip registers'.
std::uint32_t physical_of(std::uint32_t a, bool sh4) { return a >= (sh4 ? 0xE0000000u : 0xFFFFFE00u) ? a : a & 0x1FFFFFFFu; }

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

std::string byte_row(ByteView bytes) {
    std::vector<std::string> items;
    for (const std::uint8_t b : bytes) items.push_back(std::format("0x{:02X}", b));
    return "        .byte " + text::join(items, ", ");
}

std::size_t printable_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 0;
    while (off + run < limit && run < kStringMax && printable(buf[off + run])) ++run;
    return run;
}

std::size_t same_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 1;
    while (off + run < limit && buf[off + run] == buf[off]) ++run;
    return run;
}

std::uint32_t be32(ByteView buf, std::size_t at) {
    return static_cast<std::uint32_t>(buf[at]) << 24 | static_cast<std::uint32_t>(buf[at + 1]) << 16 | static_cast<std::uint32_t>(buf[at + 2]) << 8 | buf[at + 3];
}

// Where a string starts inside the next longword (1 to 3 bytes in), or 0.
std::size_t string_inside_word(ByteView buf, std::size_t at, std::size_t limit) {
    for (std::size_t k = 1; k < 4 && at + k < limit; ++k)
        if (printable_run(buf, at + k, limit) >= kStringMin) return k;
    return 0;
}

// One line of data from buf[off], which is at `addr`, to before `limit`: a string, a run of one value, up to four longwords (one that
// `pointer_at` names is written as the name), up to eight words, or a few bytes.
struct DataItem {
    std::string line;
    std::size_t took = 0;
};
DataItem data_item(ByteView buf, std::size_t off, std::size_t limit, std::uint32_t addr, bool little,
                   const std::function<std::optional<std::string>(std::uint32_t)>& pointer_at) {
    if (const std::size_t run = printable_run(buf, off, limit); run >= kStringMin) {
        const bool terminated = off + run < limit && buf[off + run] == 0;
        return {std::format("        {} \"{}\"", terminated ? ".asciz" : ".ascii", as_text(buf.subspan(off, run))), run + (terminated ? 1u : 0u)};
    }
    if (const std::size_t same = same_run(buf, off, limit); same >= kFillMin)
        return {buf[off] == 0 ? std::format("        .space {}", same) : std::format("        .fill {}, 1, 0x{:02X}", same, buf[off]), same};
    if (addr % 4 == 0 && off + 4 <= limit) {
        if (const std::size_t lead = string_inside_word(buf, off, limit); lead != 0) return {byte_row(buf.subspan(off, lead)), lead};
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 4 && off + took + 4 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0)) break;
            const std::optional<std::string> name = pointer_at(addr + static_cast<std::uint32_t>(took));
            items.push_back(name ? *name : hex8(word_at(buf, at, little)));
            took += 4;
        }
        return {"        .long " + text::join(items, ", "), took};
    }
    if (addr % 2 == 0 && off + 2 <= limit) {
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 8 && off + took + 2 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin)) break;
            if (took != 0 && (addr + took) % 4 == 0 && at + 4 <= limit) break;   // (the longwords that follow are written as longwords)
            items.push_back(std::format("0x{:04X}", half_at(buf, at, little)));
            took += 2;
        }
        return {"        .word " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 2 != 0 ? 1 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

}  // namespace

const std::map<std::uint32_t, std::string>& chip_registers() {
    static const std::map<std::uint32_t, std::string> table = {
        {0xFFFFFE00, "SCI_SMR"},   {0xFFFFFE01, "SCI_BRR"},   {0xFFFFFE02, "SCI_SCR"},   {0xFFFFFE03, "SCI_TDR"},   {0xFFFFFE04, "SCI_SSR"},
        {0xFFFFFE05, "SCI_RDR"},   {0xFFFFFE10, "FRT_TIER"},  {0xFFFFFE11, "FRT_FTCSR"}, {0xFFFFFE12, "FRT_FRC"},   {0xFFFFFE14, "FRT_OCRA"},
        {0xFFFFFE16, "FRT_TCR"},   {0xFFFFFE17, "FRT_TOCR"},  {0xFFFFFE18, "FRT_ICR"},   {0xFFFFFE60, "INTC_IPRB"}, {0xFFFFFE62, "INTC_VCRA"},
        {0xFFFFFE64, "INTC_VCRB"}, {0xFFFFFE66, "INTC_VCRC"}, {0xFFFFFE68, "INTC_VCRD"}, {0xFFFFFE71, "DMAC_DRCR0"}, {0xFFFFFE72, "DMAC_DRCR1"},
        {0xFFFFFE80, "WDT_WTCSR"}, {0xFFFFFE81, "WDT_WTCNT"}, {0xFFFFFE83, "WDT_RSTCSR"}, {0xFFFFFE91, "SBYCR"},      {0xFFFFFE92, "CCR"},
        {0xFFFFFEE0, "INTC_ICR"},  {0xFFFFFEE2, "INTC_IPRA"}, {0xFFFFFEE4, "INTC_VCRWDT"},
        {0xFFFFFF00, "DVSR"},      {0xFFFFFF04, "DVDNT"},     {0xFFFFFF08, "DVCR"},      {0xFFFFFF0C, "VCRDIV"},    {0xFFFFFF10, "DVDNTH"},
        {0xFFFFFF14, "DVDNTL"},    {0xFFFFFF80, "DMAC_SAR0"}, {0xFFFFFF84, "DMAC_DAR0"}, {0xFFFFFF88, "DMAC_TCR0"}, {0xFFFFFF8C, "DMAC_CHCR0"},
        {0xFFFFFF90, "DMAC_SAR1"}, {0xFFFFFF94, "DMAC_DAR1"}, {0xFFFFFF98, "DMAC_TCR1"}, {0xFFFFFF9C, "DMAC_CHCR1"}, {0xFFFFFFA0, "DMAC_VCRDMA0"},
        {0xFFFFFFA8, "DMAC_VCRDMA1"}, {0xFFFFFFB0, "DMAC_DMAOR"}, {0xFFFFFFE0, "BSC_BCR1"}, {0xFFFFFFE4, "BSC_BCR2"}, {0xFFFFFFE8, "BSC_WCR"},
        {0xFFFFFFEC, "BSC_MCR"},   {0xFFFFFFF0, "BSC_RTCSR"}, {0xFFFFFFF4, "BSC_RTCNT"}, {0xFFFFFFF8, "BSC_RTCOR"}};
    return table;
}

iw::Written write(const Image& image, std::string_view stem_in, const std::filesystem::path& folder, std::string_view built_name) {
    fs::make_dirs(folder);
    const std::string stem(stem_in);
    const Layout layout(image);
    const ShFlow flow(layout);
    const bool little = image.sh4;
    const std::uint32_t chip_base = image.sh4 ? 0xE0000000u : 0xFFFFFE00u;   // addresses from here are the chip's registers, not a view of memory
    const std::uint32_t uncached_view = image.sh4 ? 0xA0000000u : 0x20000000u;
    const auto physical = [sh4 = image.sh4](std::uint32_t a) { return physical_of(a, sh4); };
    const std::size_t image_size = image.bytes.size();

    // function starts: the entry points, then what the code points at and where functions begin after another one's end, round by round
    std::set<std::uint32_t> entries;
    for (const std::uint32_t a : image.entries)
        if (flow.inside(a)) entries.insert(a);
    iw::Walked<ShInsn> walked;
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<ShInsn>(flow, from);
        const auto& code = walked.code;
        std::set<std::uint32_t> pointed;
        for (const auto& [pc, p] : code) {   // the constants the code loads, and the longwords in the data
            if (p.insn.literal && !p.insn.makes_address() && p.insn.size == 4 && layout.holds(*p.insn.literal, 4)) pointed.insert(layout.word(*p.insn.literal));
        }
        for (const Part& part : image.parts) {
            if (!part.code) continue;
            for (std::uint32_t a = part.base; a + 4 <= part.base + part.length; a += 4)
                if (!code.contains(a) && !code.contains(a + 2) && layout.holds(a, 4)) pointed.insert(layout.word(a));
        }
        bool added = false;
        for (const std::uint32_t a : pointed) {
            const std::uint32_t p = physical(a);
            if (p % 2 == 0 && flow.inside(p) && !code.contains(p) && (is_prologue(layout.half(p)) || reads_as_function(layout, p))) added |= entries.insert(p).second;
        }
        for (const auto& [pc, p] : code) {   // after a function's end, past its constants, the next function
            if (!p.ends) continue;
            for (std::uint32_t a = pc + 2; a < pc + 2 + kGapReach && flow.inside(a) && !code.contains(a); a += 2) {
                if (is_prologue(layout.half(a)) || reads_as_function(layout, a)) {
                    added |= entries.insert(a).second;
                    break;
                }
            }
        }
        if (!added) break;
    }
    const std::map<std::uint32_t, ShInsn>& code = walked.code;
    std::vector<std::uint32_t> function_starts;   // (an entry point whose bytes are not an instruction is not a function)
    for (const std::uint32_t a : walked.starts)
        if (code.contains(a)) function_starts.push_back(a);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);

    // the constants PC-relative loads read: which bytes they are (a constant is a longword or a word the code does not run, that no other constant
    // overlaps); no label can stand inside one
    std::map<std::uint32_t, unsigned> slot_sizes;     // by the constant's address: 4 (.long) or 2 (.word)
    std::set<std::uint32_t> slot_interior;            // the bytes of a constant after its first
    {
        std::set<std::uint32_t> taken;
        for (const auto& [pc, p] : code) {
            if (!p.insn.literal || p.insn.makes_address()) continue;
            const std::uint32_t at = *p.insn.literal;
            const unsigned size = p.insn.size;
            if (slot_sizes.contains(at) || !layout.holds(at, size) || (size == 4 && at % 4 != 0) || (size == 2 && at % 2 != 0)) continue;
            bool free_of_others = true;
            for (unsigned k = 0; k < size; ++k) free_of_others = free_of_others && !code.contains(at + k) && !inside_insn.contains(at + k) && !taken.contains(at + k);
            if (!free_of_others) continue;
            slot_sizes[at] = size;
            for (unsigned k = 0; k < size; ++k) taken.insert(at + k);
            for (unsigned k = 1; k < size; ++k) slot_interior.insert(at + k);
        }
    }
    const auto line_start = [&](std::uint32_t a) {
        const Part* part = layout.part_at(a);
        return part != nullptr && (part->labels || a == part->base) && !inside_insn.contains(a) && !slot_interior.contains(a);
    };

    // names: an entry has the name it was given, a function `sub_<address>`
    iw::Names names = image.names;
    const iw::Functions functions = iw::name_functions(function_starts, names, kSub);
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;    // the names an address outside the program is written with
    const auto program_label = [&](std::uint32_t a) -> std::optional<std::string> {
        if (!line_start(a)) return std::nullopt;
        const auto it = labels.find(a);
        if (it != labels.end()) return it->second;
        const std::string made = (code.contains(a) ? kLocal : kData)(a);
        labels[a] = made;
        return made;
    };
    // what an address or a constant is written as in the source: a label (in the view it is given: `label + $20000000` for the uncached one),
    // a register of the machine, a variable in RAM, or nothing (a number)
    const auto name_of = [&](std::uint32_t v) -> std::optional<std::string> {
        const std::uint32_t phys = physical(v);
        if (v < chip_base && layout.in_image(phys)) {
            if (const auto label = program_label(phys)) return (v & 0xE0000000u) == 0 ? *label : std::format("{} + 0x{:X}", *label, v & 0xE0000000u);
        }
        if (image.hardware) {
            const auto it = image.hardware->find(phys);
            if (it != image.hardware->end()) {
                const std::uint32_t view = v & 0xE0000000u;
                if ((v >= chip_base || view == uncached_view || view == 0 || (image.sh4 && view == 0x80000000u)) && !(image.sh4 && v == 0)) {   // (a null pointer is not the BIOS ROM)
                    std::string n = it->second + (v >= chip_base || view == uncached_view ? "" : image.sh4 && view == 0x80000000u ? "_P1" : "_C");   // (each view of a register is a name of its own)
                    equates[n] = v;
                    return n;
                }
            }
        }
        if (v < chip_base && image.is_variable && image.is_variable(phys)) {
            std::string n = std::format("var_{:08X}", v);
            equates[n] = v;
            return n;
        }
        return std::nullopt;
    };

    // the constants PC-relative loads read: each is a label, written by what it holds; `mova` makes an address of a label too
    std::map<std::uint32_t, std::string> slot_lines;   // by the constant's address: its source line
    for (const auto& [pc, p] : code)
        if (p.insn.literal && p.insn.makes_address() && line_start(*p.insn.literal)) program_label(*p.insn.literal);
    for (const auto& [at, size] : slot_sizes) {
        if (!labels.contains(at)) labels[at] = kConstant(at);
        if (size == 4) {
            const std::uint32_t v = layout.word(at);
            const std::optional<std::string> n = name_of(v);
            slot_lines[at] = "        .long " + (n ? *n : hex8(v));
        } else {
            slot_lines[at] = std::format("        .word 0x{:04X}", layout.half(at));
        }
    }

    // longwords in the data that hold an address in the program (a jump table, a table of pointers)
    std::set<std::uint32_t> pointer_words;
    for (const Part& part : image.parts) {
        if (!part.mapped) continue;
        for (std::uint32_t a = part.base; a + 4 <= part.base + part.length; a += 4) {
            if (a % 4 != 0 || code.contains(a) || slot_lines.contains(a)) continue;
            bool clear = true;
            for (unsigned k = 0; k < 4; ++k) clear = clear && !inside_insn.contains(a + k) && !code.contains(a + k) && (k == 0 || !slot_lines.contains(a + k));
            if (!clear) continue;
            const std::uint32_t v = layout.word(a);
            if (v >= 0x01000000u && layout.in_image(physical(v)) && line_start(physical(v)) && physical(v) % 2 == 0 && name_of(v)) pointer_words.insert(a);
        }
    }

    // the source ----------------------------------------------------------------------------------------------------------------
    std::vector<std::string> head = image.head;
    head.push_back(std::format("! {} functions, {} of {} bytes reached as code", functions.size(), iw::code_bytes(code, flow), image_size));
    head.push_back("");
    translate::l2::Assembly out(head, "gas", image.module);
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    // boundaries (image offsets): a part's end, an instruction, a label, a constant
    std::set<std::uint32_t> boundaries = {static_cast<std::uint32_t>(image_size)};
    for (const Part& part : image.parts) boundaries.insert(part.offset + part.length);
    for (const auto& entry : code) boundaries.insert(layout.offset_of(entry.first));
    for (const auto& entry : labels)
        if (line_start(entry.first)) boundaries.insert(layout.offset_of(entry.first));
    for (const auto& slot : slot_sizes) boundaries.insert(layout.offset_of(slot.first));
    decompile::AddressLabels labels_for_text;
    for (const auto& [a, label] : labels) labels_for_text[a] = label;
    const auto pointer_name = [&](std::uint32_t a) -> std::optional<std::string> {
        if (!pointer_words.contains(a)) return std::nullopt;
        return name_of(layout.word(a));
    };

    for (const Part& part : image.parts) {
        out.part(part.name, part.code ? "code" : "data", std::nullopt, part.length);
        if (part.mapped) out.append(std::format("        .org {}", hex8(part.base)));
        else out.append("! (not loaded: no address names these bytes)");
        std::uint32_t off = part.offset;
        const std::uint32_t end = part.offset + part.length;
        while (off < end) {
            const std::uint32_t addr = layout.address_of(part, off);
            if (!part.mapped) {   // (no label, instruction or constant is here: the bytes only)
                const std::uint32_t limit = std::min(*boundaries.upper_bound(off), end);
                const DataItem item = data_item(image.bytes, off, limit, addr, little, [](std::uint32_t) { return std::nullopt; });
                out.append(item.line);
                off += static_cast<std::uint32_t>(item.took);
                continue;
            }
            if (const auto label = labels.find(addr); label != labels.end() && line_start(addr)) {
                if (const auto fn = functions.find(addr); fn != functions.end()) {
                    out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});   // each function a fragment
                    out.append("");
                }
                out.append(label->second + ":");
            }
            if (const auto it = code.find(addr); it != code.end()) {
                out.append("        " + dm::text(it->second.insn, labels_for_text));
                off += 2;
                continue;
            }
            if (const auto slot = slot_lines.find(addr); slot != slot_lines.end()) {
                out.append(slot->second);
                off += slot_sizes.at(addr);
                continue;
            }
            if (!part.code && image.special) {
                std::vector<std::string> lines;
                if (const std::uint32_t took = image.special(off, lines); took != 0) {
                    for (const std::string& l : lines) out.append(l);
                    off += took;
                    continue;
                }
            }
            const std::uint32_t limit = std::min(*boundaries.upper_bound(off), end);
            const DataItem item = data_item(image.bytes, off, limit, addr, little, pointer_name);
            out.append(item.line);
            off += static_cast<std::uint32_t>(item.took);
        }
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), image_size);
    const build::sh_asm::Assembler assembler(image.sh4 ? isa::sh4_encoder() : isa::sh2_encoder(), image.sh4);
    iw::assembled(r, [&] { return assembler.build(source); }, image.bytes, folder / std::string(built_name), "image");
    return r;
}

}  // namespace cb::source::sh2_writer
