// source/z80_writer.cpp -- a flat Z80 program as assembly source that builds back into it (z80_writer.hpp).

#include "source/z80_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "source/isa.hpp"
#include "source/z80_source.hpp"

namespace cb::source::z80_writer {

namespace z80 = decompile::z80;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};
bool port_access(const z80::Insn& insn) { return insn.pattern == "OUT (n),A" || insn.pattern == "IN A,(n)"; }

// Flow runs through the block, in a 64 KB memory with the block at its origin.
class BlockFlow final : public iw::CodeModel<z80::Insn> {
public:
    explicit BlockFlow(const Block& block) : origin_(block.origin), end_(block.origin + static_cast<std::uint32_t>(block.image.size())), memory_(0x10000, 0) {
        if (origin_ < memory_.size()) {
            const std::size_t room = std::min<std::size_t>(block.image.size(), memory_.size() - origin_);
            std::copy_n(block.image.begin(), room, memory_.begin() + origin_);
        }
    }

    std::uint32_t end() const { return end_; }
    std::optional<z80::Insn> decode(std::uint32_t pc) const override { return z80::decode(memory_, pc, pc); }
    std::uint32_t size(const z80::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const z80::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc >= origin_ && pc < end_; }
    bool fits(std::uint32_t pc, const z80::Insn& insn) const override { return pc + insn.size <= end_; }
    bool follows(const z80::Insn& insn) const override {
        return insn.target && *insn.target >= origin_ && *insn.target < end_ && insn.flow != z80::Flow::None;
    }
    bool stops(const z80::Insn& insn) const override {
        return insn.flow == z80::Flow::Jump || insn.flow == z80::Flow::Return || insn.flow == z80::Flow::Indirect;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const z80::Insn& insn) const override {
        return (insn.flow == z80::Flow::Call || insn.flow == z80::Flow::ConditionalCall) && !insn.pattern.starts_with("RST");
    }

private:
    std::uint32_t origin_, end_;
    Bytes memory_;
};

class BlockListing final : public iw::Listing<z80::Insn> {
public:
    BlockListing(const Block& block, const std::map<std::uint32_t, z80::Insn>& code, const decompile::AddressLabels& names, const iw::Names& ports)
        : block_(block), code_(code), names_(names), ports_(ports) {}

    std::uint8_t byte(std::uint32_t address) const override { return block_.image[address - block_.origin]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const z80::Insn* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const z80::Insn& insn) const override { return insn.size; }
    std::string text(const z80::Insn& insn) const override { return z80_source::code(insn, names_, ports_); }   // (the description's text, the names put in)
    std::vector<std::string> data_lines(ByteView data) const override { return z80_source::data_lines(data); }

private:
    const Block& block_;
    const std::map<std::uint32_t, z80::Insn>& code_;
    const decompile::AddressLabels& names_;
    const iw::Names& ports_;
};

}  // namespace

iw::Walked<z80::Insn> walk(const Block& block) {
    const BlockFlow flow(block);
    std::vector<std::uint32_t> inside;
    for (const auto& [address, name] : block.entries) {
        if (address >= block.origin && address < flow.end()) inside.push_back(address);
    }
    return iw::walk<z80::Insn>(flow, inside);
}

iw::Written write(const Block& block, const Machine& machine, const std::filesystem::path& folder, std::string_view stem) {
    fs::make_dirs(folder);
    const BlockFlow flow(block);
    const iw::Walked<z80::Insn> walked = walk(block);
    const std::map<std::uint32_t, z80::Insn>& code = walked.code;
    const std::uint32_t origin = block.origin;
    const std::uint32_t end = flow.end();
    iw::Names function_names = block.entries;
    const iw::Functions functions = iw::name_functions(walked.starts, function_names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_at = [&](std::int64_t a) { return a >= origin && a < end && !inside_insn.contains(static_cast<std::uint32_t>(a)); };
    iw::Names labels = iw::make_labels(walked.targets, function_names, [&](std::uint32_t a) { return line_at(a); }, kLocal);
    iw::Names used_names, used_ports;
    for (const auto& entry : code) {
        const z80::Insn& insn = entry.second;
        if (port_access(insn) && insn.value_count > 0 && insn.values[0] >= 0) {
            const auto named = machine.ports.find(static_cast<std::uint32_t>(insn.values[0]));
            if (named != machine.ports.end()) used_ports[named->first] = named->second;
        }
        if (insn.value_count == 0) continue;
        const std::int64_t v = insn.values[insn.value_count - 1];
        const bool outside = v < origin || v >= end;
        if (v >= 0 && outside && machine.names.contains(static_cast<std::uint32_t>(v)) && (isa::pattern_has(insn.pattern, "nn") || insn.flow != z80::Flow::None)) {
            used_names[static_cast<std::uint32_t>(v)] = machine.names.at(static_cast<std::uint32_t>(v));
        } else if (line_at(v)) {
            const bool pointer = insn.pattern == "LD HL,nn" || insn.pattern == "LD DE,nn" || insn.pattern == "LD IX,nn" || insn.pattern == "LD IY,nn" ||
                                 insn.pattern == "LD SP,nn";
            if (pointer || (insn.flow == z80::Flow::None && isa::pattern_has(insn.pattern, "(nn)"))) {
                const std::uint32_t at = static_cast<std::uint32_t>(v);
                labels.try_emplace(at, code.contains(at) ? (function_names.contains(at) ? function_names.at(at) : kLocal(at)) : kData(at));
            }
        }
    }
    std::vector<std::string> head = {std::format("; {}.asm -- written by CodeBreaker", stem)};
    for (const std::string& line : block.header) head.push_back("; " + line);
    head.push_back("");
    translate::l2::Assembly out(head, "sleigh-z80", std::string(stem));
    for (const auto& [port, name] : used_ports) out.append(z80_source::equ(name, port, 2));
    for (const auto& [address, name] : used_names) out.append(z80_source::equ(name, address, 4));
    if (!used_ports.empty() || !used_names.empty()) out.append("");
    out.append(std::format("        .org 0x{:04x}", origin));
    decompile::AddressLabels names_for_text;   // the labels and the fixed addresses: a fixed address wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const auto& [a, name] : used_names) names_for_text[a] = name;
    std::set<std::uint32_t> fixed;
    for (const auto& entry : used_names) fixed.insert(entry.first);
    iw::Names early;   // (labels the assembler's first pass meets before their lines: defined once here, see z80_source::forward_reference)
    for (const auto& entry : code) {
        if (const auto a = z80_source::forward_reference(entry.second, names_for_text, fixed)) early[*a] = names_for_text.at(*a);
    }
    if (!early.empty()) {
        out.append("; read ahead of their lines (behind a register the assembler's first pass takes them for unknown names)");
        for (const auto& [a, name] : early) out.append(z80_source::equ(name, a, 4));
        out.append("");
    }

    BlockListing listing(block, code, names_for_text, used_ports);
    iw::emit<z80::Insn>(out, origin, end, labels, functions, listing);
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, std::string(stem) + ".asm", source, functions, iw::code_bytes(code, flow), block.image.size());

    // (the one directive layer says where the first byte stands only through the `.org` this writer put first)
    const std::optional<Bytes> built = iw::assembled(
        r, [&] { return z80_source::assemble(source, folder); }, block.image, std::nullopt, "block",
        [&](ByteView made) { return made.size() == block.image.size() && std::equal(made.begin(), made.end(), block.image.begin()); });
    if (built) r.built_image = *built;
    return r;
}

}  // namespace cb::source::z80_writer
