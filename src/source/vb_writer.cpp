// cb-unity: alone
// source/vb_writer.cpp -- a Virtual Boy cartridge as V810 source that builds back into it (vb_writer.hpp).

#include "source/vb_writer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "build/v810_asm.hpp"
#include "core/fs.hpp"
#include "decompile/v810.hpp"

namespace cb::source::vb_writer {

namespace dv = decompile::v810;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
const iw::DataSyntax kSyntax{.bytes = ".byte", .fill = ".fill", .fill_separator = ","};

bool printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"' && c != '\\'; });
}

std::string hex8(std::uint8_t v) { return std::format("0x{:02X}", v); }

std::int32_t signed16(std::int32_t v) { return static_cast<std::int16_t>(v); }

// The registers an instruction changes, as a mask (bit n: rn).
std::uint32_t writes(const dv::Insn& i) {
    const std::uint32_t r2 = 1u << i.r2;
    if (i.id >= dv::id::kBitStringBase && i.id < dv::id::kExtendedBase) return 0x7C000000u;   // r26-r30
    switch (i.mode) {
        case dv::Mode::None:
        case dv::Mode::Jmp:
        case dv::Mode::LdSr:
        case dv::Mode::Trap:
        case dv::Mode::Branch:
        case dv::Mode::Store: return 0;
        case dv::Mode::R1R2:
            if (i.id == dv::id::kCmp || i.id == dv::id::kCmpfS) return 0;
            return i.id == dv::id::kMul || i.id == dv::id::kMulu || i.id == dv::id::kDiv || i.id == dv::id::kDivu ? r2 | 1u << 30 : r2;
        case dv::Mode::S5R2: return i.id == dv::id::kCmpI5 ? 0 : r2;
        case dv::Mode::Jump26: return i.id == dv::id::kJal ? 1u << 31 : 0;
        default: return r2;
    }
}

class VbFlow final : public iw::CodeModel<dv::Insn> {
public:
    explicit VbFlow(const formats::vb::Rom& rom) : rom_(rom) {}

    std::optional<dv::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        const std::size_t offset = pc - formats::vb::kBase;
        std::optional<dv::Insn> insn = dv::decode(rom_.data, offset, pc);
        if (insn && insn->id == dv::id::kJmp && insn->r1 != 31 && offset >= 8) {   // movhi / movea / jmp [reg]: a jump to a constant address
            const std::optional<dv::Insn> hi = dv::decode(rom_.data, offset - 8, pc - 8), lo = dv::decode(rom_.data, offset - 4, pc - 4);
            if (hi && lo && hi->id == dv::id::kMovhi && hi->r1 == 0 && hi->r2 == insn->r1 && lo->id == dv::id::kMovea && lo->r1 == insn->r1 && lo->r2 == insn->r1) {
                insn->target = (static_cast<std::uint32_t>(hi->imm) << 16) + static_cast<std::uint32_t>(lo->imm);
                insn->flow = dv::Flow::Jump;
            }
        }
        return insn;
    }
    std::uint32_t size(const dv::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const dv::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= formats::vb::kBase && pc - formats::vb::kBase < rom_.data.size() && (pc & 1) == 0; }
    bool fits(std::uint32_t pc, const dv::Insn& insn) const override { return pc - formats::vb::kBase + insn.size <= rom_.data.size(); }
    bool follows(const dv::Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == dv::Flow::Branch || insn.flow == dv::Flow::Jump || insn.flow == dv::Flow::Call);
    }
    bool stops(const dv::Insn& insn) const override {
        return insn.flow == dv::Flow::Jump || insn.flow == dv::Flow::Return || insn.flow == dv::Flow::Indirect || insn.flow == dv::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const dv::Insn& insn) const override { return insn.flow == dv::Flow::Call; }

private:
    const formats::vb::Rom& rom_;
};

class VbListing final : public iw::Listing<dv::Insn> {
public:
    VbListing(translate::l2::Assembly& out, const formats::vb::Rom& rom, const std::map<std::uint32_t, dv::Insn>& code, const std::map<std::uint32_t, std::string>& text,
              bool header_is_data)
        : out_(out), rom_(rom), code_(code), text_(text), header_is_data_(header_is_data) {}

    std::uint8_t byte(std::uint32_t offset) const override { return rom_.data[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return rom_.address(offset); }
    const dv::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const dv::Insn& insn) const override { return insn.size; }
    std::string text(const dv::Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return iw::data_lines(data, kSyntax); }

    // The header: title, reserved bytes, maker, game code, version.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (!header_is_data_ || offset != rom_.header_offset()) return 0;
        flush();
        const ByteView d = ByteView(rom_.data).subspan(offset, 0x20);
        const auto field = [&](std::size_t at, std::size_t n, const char* what) {
            const ByteView f = d.subspan(at, n);
            if (printable(f)) {
                out_.append(std::format("        .ascii \"{}\"  ; {}", std::string(reinterpret_cast<const char*>(f.data()), f.size()), what));
            } else {
                out_.extend(iw::commented(iw::data_lines(f, kSyntax), what));
            }
        };
        field(0, 20, "title");
        out_.extend(iw::commented(iw::data_lines(d.subspan(20, 5), kSyntax), "reserved"));
        field(25, 2, "maker");
        field(27, 4, "game code");
        out_.append(std::format("        .byte {}  ; version", hex8(d[31])));
        return 0x20;
    }

private:
    translate::l2::Assembly& out_;
    const formats::vb::Rom& rom_;
    const std::map<std::uint32_t, dv::Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    bool header_is_data_;
};

// A constant address the code builds: a movhi (the high half) and the instruction that completes it.
struct AddressPair {
    std::uint32_t high_at, low_at;
    std::uint32_t value;
};

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        const std::pair<std::uint32_t, const char*> vip[] = {{0x5F800, "VIP_INTPND"}, {0x5F802, "VIP_INTENB"}, {0x5F804, "VIP_INTCLR"}, {0x5F820, "VIP_DPSTTS"}, {0x5F822, "VIP_DPCTRL"},
                                                              {0x5F824, "VIP_BRTA"},   {0x5F826, "VIP_BRTB"},   {0x5F828, "VIP_BRTC"},   {0x5F82A, "VIP_REST"},   {0x5F82E, "VIP_FRMCYC"},
                                                              {0x5F830, "VIP_CTA"},    {0x5F840, "VIP_XPSTTS"}, {0x5F842, "VIP_XPCTRL"}, {0x5F844, "VIP_VER"},    {0x5F848, "VIP_SPT0"},
                                                              {0x5F84A, "VIP_SPT1"},   {0x5F84C, "VIP_SPT2"},   {0x5F84E, "VIP_SPT3"},   {0x5F860, "VIP_GPLT0"},  {0x5F862, "VIP_GPLT1"},
                                                              {0x5F864, "VIP_GPLT2"},  {0x5F866, "VIP_GPLT3"},  {0x5F868, "VIP_JPLT0"},  {0x5F86A, "VIP_JPLT1"},  {0x5F86C, "VIP_JPLT2"},
                                                              {0x5F86E, "VIP_JPLT3"},  {0x5F870, "VIP_BKCOL"}};
        for (const auto& [a, n] : vip) t[a] = n;
        for (unsigned w = 0; w < 5; ++w) t[0x01000000 + 0x80 * w] = std::format("VSU_WAVE{}", w + 1);
        t[0x01000280] = "VSU_MODDATA";
        t[0x01000580] = "VSU_SSTOP";
        const char* channel[] = {"INT", "LRV", "FQL", "FQH", "EV0", "EV1", "RAM", "SWP"};
        for (unsigned c = 0; c < 6; ++c) {
            for (unsigned k = 0; k < 8; ++k) {
                if (k == 7 && c != 4) continue;   // (only channel 5 has the sweep / modulation register)
                if (k == 6 && c == 5) continue;   // (the noise channel has no waveform select)
                t[0x01000400 + 0x40 * c + 4 * k] = std::format("VSU_S{}{}", c + 1, channel[k]);
            }
        }
        const std::pair<std::uint32_t, const char*> hw[] = {{0x02000000, "HW_CCR"}, {0x02000004, "HW_CCSR"}, {0x02000008, "HW_CDTR"}, {0x0200000C, "HW_CDRR"}, {0x02000010, "HW_SDLR"},
                                                             {0x02000014, "HW_SDHR"}, {0x02000018, "HW_TLR"},  {0x0200001C, "HW_THR"},  {0x02000020, "HW_TCR"},  {0x02000024, "HW_WCR"},
                                                             {0x02000028, "HW_SCR"}};
        for (const auto& [a, n] : hw) t[a] = n;
        return t;
    }();
    return table;
}

iw::Written write(const formats::vb::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    namespace vb = formats::vb;
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const VbFlow flow(rom);
    iw::Names names;
    std::vector<std::uint32_t> entries;
    for (const vb::Rom::Entry& e : rom.entries()) {
        const std::uint32_t a = rom.address(e.offset);
        entries.push_back(a);
        names.try_emplace(a, e.name == "reset" ? std::string("reset") : "vector_" + std::string(e.name));
    }
    const iw::Walked<dv::Insn> walked = iw::walk<dv::Insn>(flow, entries);
    const std::map<std::uint32_t, dv::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return a >= vb::kBase && a - vb::kBase < rom.data.size() && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // constants the code builds with movhi and a completing instruction, that are the address of something named
    std::vector<AddressPair> pairs;
    {
        struct Known {
            std::uint32_t value = 0, origin = 0;
        };
        std::array<std::optional<Known>, 32> known;
        std::uint32_t expected = 0;
        const auto named = [&](std::uint32_t v) { return hardware().contains(v) || (line_start(v) && v >= vb::kBase); };
        for (const auto& [at, insn] : code) {
            if (at != expected || labels.contains(at)) known = {};
            expected = at + insn.size;
            std::optional<std::uint32_t> completed;
            std::uint32_t origin = 0;
            if (insn.id == dv::id::kMovhi && insn.r1 == 0 && insn.r2 != 0) {
                known[insn.r2] = Known{static_cast<std::uint32_t>(insn.imm) << 16, at};
                continue;
            }
            const std::optional<Known>& base = known[insn.r1];
            if (base && (insn.id == dv::id::kMovea || insn.id == dv::id::kAddi || insn.mode == dv::Mode::Load || insn.mode == dv::Mode::Store)) {
                const std::uint32_t value = base->value + static_cast<std::uint32_t>(signed16(insn.imm));
                if (named(value)) {
                    completed = value;
                    origin = base->origin;
                }
            }
            if (const std::uint32_t w = writes(insn)) {
                for (unsigned r = 0; r < 32; ++r) {
                    if (w & 1u << r) known[r].reset();
                }
            }
            if (insn.flow != dv::Flow::None) known = {};
            if (completed) pairs.push_back({origin, at, *completed});
        }
    }
    // the labels those constants name, and the equates
    std::set<std::uint32_t> used;
    for (const AddressPair& p : pairs) {
        if (hardware().contains(p.value)) used.insert(p.value);
        else labels.try_emplace(p.value, kData(p.value));
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = hardware().at(a);
    decompile::NameLabels name_values;
    for (const auto& [a, n] : address_labels) name_values[n] = a;

    // each instruction as text, or as bytes when the text would not assemble back to them
    const auto same_bytes = [&](const std::string& t, const dv::Insn& insn) {
        try {
            const Bytes built = dv::assemble(t, insn.at, name_values);
            return built.size() == insn.size && std::equal(built.begin(), built.end(), insn.raw.begin());
        } catch (const Error&) {
            return false;
        }
    };
    std::map<std::uint32_t, std::string> instruction_text;
    std::map<std::uint32_t, std::string> immediates;   // an immediate written as hi(name) / lo(name)
    for (const AddressPair& p : pairs) {
        const std::string& name = address_labels.at(p.value);
        const dv::Insn& high = code.at(p.high_at);
        const dv::Insn& low = code.at(p.low_at);
        const std::string hi_text = dv::text(high, address_labels, "hi(" + name + ")"), lo_text = dv::text(low, address_labels, "lo(" + name + ")");
        if (same_bytes(hi_text, high) && same_bytes(lo_text, low)) {
            immediates.try_emplace(p.high_at, "hi(" + name + ")");
            immediates[p.low_at] = "lo(" + name + ")";
        }
    }
    std::set<std::uint32_t> equates;
    for (const auto& [at, insn] : code) {
        const auto imm = immediates.find(at);
        std::string t = dv::text(insn, address_labels, imm == immediates.end() ? std::string_view() : std::string_view(imm->second));
        if (same_bytes(t, insn)) {
            instruction_text[at] = std::move(t);
            if (imm != immediates.end()) {
                for (const std::uint32_t a : used) {
                    if (imm->second.find("(" + address_labels.at(a) + ")") != std::string::npos) equates.insert(a);
                }
            }
        } else {
            std::string row = ".byte ";
            for (std::size_t k = 0; k < insn.size; ++k) row += (k ? "," : "") + hex8(insn.raw[k]);
            instruction_text[at] = row + "  ; " + dv::text(insn, address_labels);
        }
    }

    const std::size_t header_at = rom.header_offset();
    bool header_is_data = true;
    for (std::size_t k = 0; k < 0x20; ++k) {
        const std::uint32_t a = rom.address(header_at + k);
        if (code.contains(a) || inside_insn.contains(a)) header_is_data = false;
    }

    translate::l2::Assembly out({std::format("; {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                 std::format("; Virtual Boy cartridge, {} bytes, \"{}\", reset at 0x{:08X}", rom.data.size(), rom.header.title, rom.reset_address()), ""},
                                "v810", std::string(file_name));
    for (const std::uint32_t a : equates) out.append(std::format(".equ {}, 0x{:08X}", hardware().at(a), a));
    if (!equates.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:08X}", vb::kBase));
    VbListing listing(out, rom, code, instruction_text, header_is_data);
    iw::emit<dv::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            build::v810_asm::Image image = build::v810_asm::build(source);
            if (image.base != vb::kBase) throw build::AsmError("the image starts at the wrong address");
            return std::move(image.data);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::vb::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::vb_writer
