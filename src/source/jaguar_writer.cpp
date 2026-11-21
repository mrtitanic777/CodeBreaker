// cb-unity: alone
// source/jaguar_writer.cpp -- an Atari Jaguar program as assembly source that builds back into it (jaguar_writer.hpp).

#include "source/jaguar_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m68k.hpp"
#include "source/dialect.hpp"
#include "source/isa.hpp"
#include "source/risc_source.hpp"

namespace cb::source::jaguar_writer {

namespace m68k = decompile::m68k;
namespace iw = image_writer;
namespace jag = formats::jaguar;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

// Flow runs through the segments at even addresses.
class JaguarFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit JaguarFlow(const jag::Program& program) : program_(program), memory_(program.memory()) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(memory_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && program_.segment_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override {
        const jag::Segment* s = program_.segment_at(pc);
        return s && pc + insn.length <= s->address + s->size;
    }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target % 2 == 0 && program_.segment_at(*insn.target) != nullptr && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const m68k::Insn& insn) const override { return insn.mnemonic == "JSR" || insn.mnemonic == "BSR"; }

private:
    const jag::Program& program_;
    Bytes memory_;
};

class JaguarListing final : public iw::Listing<m68k::Insn> {
public:
    JaguarListing(translate::l2::Assembly& out, const jag::Program& program, const jag::Segment& segment, const std::map<std::uint32_t, m68k::Insn>& code,
                  const iw::Names& labels, const decompile::AddressLabels& names)
        : out_(out), program_(program), segment_(segment), code_(code), labels_(labels), names_(names) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.file[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return segment_.address + (offset - segment_.offset); }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return dialect::find("vasm-m68k").fallback(risc_source::m68k_code(insn, names_)); }   // (the description's text, the names put in)
    std::vector<std::string> data_lines(ByteView data) const override { return risc_source::data_lines(data); }

    // A cartridge's boot signature and its header: the flags long and the start address.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (program_.kind != jag::Kind::Cartridge) return 0;
        if (offset == 0) {
            flush();
            out_.extend(iw::commented(risc_source::data_lines(ByteView(program_.file).first(jag::kHeaderAt)), "the boot signature"));
            return static_cast<std::uint32_t>(jag::kHeaderAt);
        }
        if (offset == jag::kHeaderAt) {
            flush();
            out_.append(std::format("        .long 0x{:08x}  ; header flags", program_.header_flags));
            const auto named = labels_.find(program_.entry);
            out_.append("        .long " + (named != labels_.end() ? named->second : std::format("0x{:08x}", program_.entry)) + "  ; start address");
            return 8;
        }
        return 0;
    }

private:
    translate::l2::Assembly& out_;
    const jag::Program& program_;
    const jag::Segment& segment_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& names_;
};

// The addresses an instruction's operands name, and whether it is a long immediate.
struct Reference {
    std::uint32_t value;
    bool immediate;
};
std::vector<Reference> references(const m68k::Insn& insn) {
    std::vector<Reference> out;
    for (const m68k::Op& o : insn.operands()) {
        switch (o.kind) {
            case m68k::OpKind::AbsW:
            case m68k::OpKind::AbsL:
            case m68k::OpKind::PcDisp:
            case m68k::OpKind::PcIndex: out.push_back({static_cast<std::uint32_t>(o.value & 0xFFFFFFFF), false}); break;
            case m68k::OpKind::Imm:
                if (o.size == m68k::Size::Long && o.value >= 0 && o.value <= 0xFFFFFFFF) out.push_back({static_cast<std::uint32_t>(o.value), true});
                break;
            default: break;
        }
    }
    return out;
}

// What the assembled image makes of the file: the segments' bytes follow one another in the order of their addresses (the order they are written in); a
// cartridge is those bytes, a COFF is the original with each section's bytes replaced.
Bytes rebuilt_file(const jag::Program& program, const Bytes& image) {
    Bytes out = program.file;
    std::vector<jag::Segment> order = program.segments;
    std::sort(order.begin(), order.end(), [](const jag::Segment& a, const jag::Segment& b) { return a.address < b.address; });
    std::size_t at = 0;
    for (const jag::Segment& s : order) {
        for (std::uint32_t i = 0; i < s.size; ++i, ++at) out[s.offset + i] = at < image.size() ? image[at] : 0;
    }
    return out;
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/jaguar/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("jaguar", "hardware");
    return table;
}

iw::Written write(const jag::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const bool cartridge = program.kind == jag::Kind::Cartridge;
    const JaguarFlow flow(program);
    const std::vector<std::uint32_t> entries = {program.entry};
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;

    iw::Names names;
    names.try_emplace(program.entry, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a % 2 == 0 && program.segment_at(a) != nullptr && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const Reference& ref : references(entry.second)) {
            if (hardware().contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value) || ref.value >= 0x4000)) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kLocal : kData)(ref.value));
            }
        }
    }

    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker from {}", stem, base_name),
                                     std::format("; Atari Jaguar {}, {} bytes, start ${:06X}", cartridge ? "cartridge" : "COFF executable", program.file.size(), program.entry),
                                     ""};
    translate::l2::Assembly out(head, "asm68k", std::string(file_name));
    for (const std::uint32_t a : used) out.append(risc_source::equ(hardware().at(a), a, 6));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    std::vector<jag::Segment> order = program.segments;
    std::sort(order.begin(), order.end(), [](const jag::Segment& a, const jag::Segment& b) { return a.address < b.address; });
    for (const jag::Segment& s : order) {
        out.part(s.name.empty() ? "segment" : s.name, "segment", std::nullopt, s.size);
        out.append(std::format("        .org 0x{:06x}", s.address));
        JaguarListing listing(out, program, s, code, labels, names_for_text);
        iw::emit<m68k::Insn>(out, s.offset, s.offset + s.size, labels, functions, listing);
        out.append("");
    }

    const std::string source = dialect::find("vasm-m68k").print(iw::text(out) + "\n");   // (said in vasm's Motorola syntax, config/dialects/vasm-m68k.json)
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.file.size());
    const std::string extension = cartridge ? ".j64" : ".cof";
    iw::assembled(r, [&] { return rebuilt_file(program, risc_source::assemble("m68k", dialect::find("vasm-m68k").parse(source), folder)); }, program.file, folder / (stem + ".built" + extension));
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(jag::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::jaguar_writer
