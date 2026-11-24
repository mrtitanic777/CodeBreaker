// cb-unity: alone
// source/vb_writer.cpp -- a Virtual Boy cartridge as V810 source that builds back into it (vb_writer.hpp).

#include "source/vb_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "decompile/v810.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/processors.hpp"

namespace cb::source::vb_writer {

namespace dv = decompile::v810;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
const processors::Processor& v810_cpu() {
    static const processors::Processor& cpu = processors::find("v810");
    return cpu;
}

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

// A constant address the code builds: a movhi (the high half) and the instruction that completes it.
struct AddressPair {
    std::uint32_t high_at, low_at;
    std::uint32_t value;
};

// `text` with its 16-bit immediate (the number before `[`, the first operand of a movhi / movea / addi) written as `replacement`.
std::optional<std::string> with_immediate(const dv::Insn& insn, const std::string& text, const std::string& replacement) {
    std::size_t begin = 0, end = 0;
    if (insn.mode == dv::Mode::Load || insn.mode == dv::Mode::Store) {
        end = text.find('[');
        if (end == std::string::npos) return std::nullopt;
        begin = text.find_last_of(insn.mode == dv::Mode::Load ? " " : ",", end);
        if (begin == std::string::npos) return std::nullopt;
        ++begin;
    } else {
        begin = text.find(' ');
        end = text.find(',');
        if (begin == std::string::npos || end == std::string::npos) return std::nullopt;
        ++begin;
    }
    return text.substr(0, begin) + replacement + text.substr(end);
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
    std::vector<std::string> data_lines(ByteView data) const override { return described_source::data_lines(data); }

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
                out_.extend(iw::commented(described_source::data_lines(f), what));
            }
        };
        field(0, 20, "title");
        out_.extend(iw::commented(described_source::data_lines(d.subspan(20, 5)), "reserved"));
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

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/vb/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("vb", "hardware");
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
    // the labels those constants name
    std::set<std::uint32_t> used;
    for (const AddressPair& p : pairs) {
        if (hardware().contains(p.value)) used.insert(p.value);
        else labels.try_emplace(p.value, kData(p.value));
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = hardware().at(a);

    // each instruction as the description's text, a jump, branch or call target written as its label, a constant's halves as hi(name) / lo(name),
    // or as bytes when the text would not assemble back to them
    const auto same_bytes = [&](const std::string& t, const dv::Insn& insn, std::uint32_t value, const std::string& name) {
        try {
            const std::string source = std::format("\t.equ {}, 0x{:x}\n\t.org 0x{:x}\n\t{}\n", name, value, insn.at, t);
            const Bytes built = processors::assemble_described(v810_cpu(), source, {}).data;
            return built.size() == insn.size && std::equal(built.begin(), built.end(), insn.bytes().begin());
        } catch (const Error&) {
            return false;
        }
    };
    std::map<std::uint32_t, std::string> instruction_text;
    std::map<std::uint32_t, std::pair<std::string, std::uint32_t>> halves;   // an immediate written as hi(name) / lo(name), and the value the name has
    for (const AddressPair& p : pairs) {
        const std::string& name = address_labels.at(p.value);
        const dv::Insn& high = code.at(p.high_at);
        const dv::Insn& low = code.at(p.low_at);
        const std::optional<std::string> high_text = described_source::checked_text(v810_cpu(), high.bytes(), high.at, high.size);
        const std::optional<std::string> low_text = described_source::checked_text(v810_cpu(), low.bytes(), low.at, low.size);
        if (!high_text || !low_text) continue;
        const std::optional<std::string> hi = with_immediate(high, *high_text, "hi(" + name + ")"), lo = with_immediate(low, *low_text, "lo(" + name + ")");
        if (hi && lo && same_bytes(*hi, high, p.value, name) && same_bytes(*lo, low, p.value, name)) {
            halves.try_emplace(p.high_at, *hi, p.value);
            halves[p.low_at] = {*lo, p.value};
        }
    }
    std::set<std::uint32_t> equates;
    for (const auto& [at, insn] : code) {
        const auto half = halves.find(at);
        if (half != halves.end()) {
            instruction_text[at] = half->second.first;
            if (hardware().contains(half->second.second)) equates.insert(half->second.second);
            continue;
        }
        std::optional<std::string> t = described_source::checked_text(v810_cpu(), insn.bytes(), at, insn.size);
        if (t) {
            const bool named = insn.target && address_labels.contains(*insn.target);
            if (named) *t = described_source::name_number(*t, *insn.target, address_labels.at(*insn.target), false);
            if (!named || same_bytes(*t, insn, *insn.target, address_labels.at(*insn.target))) {
                instruction_text[at] = *t;
                continue;
            }
        }
        instruction_text[at] = described_source::byte_line(v810_cpu(), insn.bytes(), at, {}, "");
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
    for (const std::uint32_t a : equates) out.append(std::format(".equ {}, 0x{:08x}", hardware().at(a), a));
    if (!equates.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, rom.data.size());
    out.append(std::format("        .org 0x{:08x}", vb::kBase));
    VbListing listing(out, rom, code, instruction_text, header_is_data);
    iw::emit<dv::Insn>(out, 0, static_cast<std::uint32_t>(rom.data.size()), labels, functions, listing);
    out.append("");

    const std::string source = dialect::find("v810").print(iw::text(out) + "\n");   // (said as a V810 assembler does: config/dialects/v810.json)
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), rom.data.size());
    iw::assembled(
        r,
        [&] {
            return described_source::assemble(v810_cpu(), dialect::find("v810").parse(source), folder);
        },
        rom.data, folder / (stem + ".built" + std::filesystem::path(file_name).extension().string()));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::vb::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::vb_writer
