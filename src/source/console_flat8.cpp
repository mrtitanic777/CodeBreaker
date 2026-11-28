// cb-unity: alone
// source/console_flat8.cpp -- the flat shape's 8-bit processors written through their SLEIGH descriptions (described_source): one ROM at one
// address (the loader's image at its origin), flow from its first byte, every instruction as the description's text when it builds back into
// its bytes (else as its bytes), the rest data in the dialect's rows. The Odyssey 2's 8048 (config/platforms/o2.json) and the Channel F's F8
// (chf.json: a DCI's constant that is a line of the cartridge is a label, the BIOS routines the code calls are named by the knowledge table
// the description's `fixed` says, each an equate).
//
// What a description says: `cpu` (the processor), `dialect`, the head, `built` (the rebuilt file's name); the loader: the ROM and its origin.

#include "source/console_shapes.hpp"

#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/f8.hpp"
#include "decompile/i8048.hpp"
#include "knowledge/package.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/processors.hpp"

namespace cb::source::console_writer::flat8_code {

namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 4};
constexpr iw::Spelling kLocal{"L_", 4};
constexpr iw::Spelling kData{"D_", 4};

// What differs between the processors: the decoder, and an instruction's constant that names a place (the F8's DCI).
struct I8048 {
    using Insn = decompile::i8048::Insn;
    using Flow = decompile::i8048::Flow;
    static std::optional<Insn> decode(ByteView data, std::uint32_t offset, std::uint32_t pc) { return decompile::i8048::decode(data, offset, pc); }
    static std::optional<std::uint32_t> constant(const Insn&) { return std::nullopt; }
};
struct F8 {
    using Insn = decompile::f8::Insn;
    using Flow = decompile::f8::Flow;
    static std::optional<Insn> decode(ByteView data, std::uint32_t offset, std::uint32_t pc) { return decompile::f8::decode(data, offset, pc); }
    static std::optional<std::uint32_t> constant(const Insn& insn) { return insn.op == 0x2A ? std::optional<std::uint32_t>(insn.value) : std::nullopt; }
};

template <class Cpu>
class Flow8 final : public iw::CodeModel<typename Cpu::Insn> {
public:
    using Insn = typename Cpu::Insn;
    using F = typename Cpu::Flow;
    Flow8(ByteView data, std::uint32_t base) : data_(data), base_(base) {}

    std::optional<Insn> decode(std::uint32_t pc) const override { return inside(pc) ? Cpu::decode(data_, pc - base_, pc) : std::nullopt; }
    std::uint32_t size(const Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= base_ && pc - base_ < data_.size(); }
    bool fits(std::uint32_t pc, const Insn& insn) const override { return pc - base_ + insn.size <= data_.size(); }
    bool follows(const Insn& insn) const override {
        return insn.target && inside(*insn.target) && (insn.flow == F::Branch || insn.flow == F::Jump || insn.flow == F::Call);
    }
    bool stops(const Insn& insn) const override { return insn.flow == F::Jump || insn.flow == F::Return || insn.flow == F::Indirect; }
    bool finds_functions() const override { return true; }
    bool starts_function(const Insn& insn) const override { return insn.flow == F::Call; }

private:
    ByteView data_;
    std::uint32_t base_;
};

template <class Cpu>
class Listing8 final : public iw::Listing<typename Cpu::Insn> {
public:
    using Insn = typename Cpu::Insn;
    Listing8(ByteView data, std::uint32_t base, const std::map<std::uint32_t, Insn>& code, const std::map<std::uint32_t, std::string>& text, const dialect::Dialect& dialect)
        : data_(data), base_(base), code_(code), text_(text), dialect_(dialect) {}

    std::uint8_t byte(std::uint32_t offset) const override { return data_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return base_ + offset; }
    const Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.size; }
    std::string text(const Insn& insn) const override { return text_.at(insn.at); }
    std::vector<std::string> data_lines(ByteView data) const override { return dialect_.data_lines(data); }

private:
    ByteView data_;
    std::uint32_t base_;
    const std::map<std::uint32_t, Insn>& code_;
    const std::map<std::uint32_t, std::string>& text_;
    const dialect::Dialect& dialect_;
};

template <class Cpu>
iw::Written write_with(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    using Insn = typename Cpu::Insn;
    fs::make_dirs(folder);
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));
    const processors::Processor& cpu = processors::find(description.executable.cpu);
    const dialect::Dialect& said = dialect::find(description.dialect);
    const ByteView data = loaded.image;
    const std::uint32_t base = loaded.origin;
    const Flow8<Cpu> flow(data, base);
    const std::vector<std::uint32_t> entries = {base};
    const iw::Walked<Insn> walked = iw::walk<Insn>(flow, entries);
    const std::map<std::uint32_t, Insn>& code = walked.code;
    iw::Names names;
    names.try_emplace(base, "start");
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + insn.size; ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return flow.inside(a) && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    for (const auto& [at, insn] : code) {   // a constant that is a line of the cartridge
        if (const auto c = Cpu::constant(insn); c && line_start(*c)) labels.try_emplace(*c, kData(*c));
    }
    const std::map<std::uint32_t, std::string> fixed = description.fixed.empty() ? std::map<std::uint32_t, std::string>{} : knowledge::standard().table_names(description.platform, description.fixed);
    std::set<std::uint32_t> used;   // fixed routines (the BIOS's) the code names
    for (const auto& [at, insn] : code) {
        if (insn.target && fixed.contains(*insn.target)) used.insert(*insn.target);
    }
    decompile::AddressLabels address_labels;
    for (const auto& [a, n] : labels) address_labels[a] = n;
    for (const std::uint32_t a : used) address_labels[a] = fixed.at(a);

    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        // (the description's text, a target or a constant written as its label, kept when it builds back into the instruction's bytes)
        std::optional<std::string> t = described_source::checked_text(cpu, insn.bytes(), at, insn.size);
        if (t) {
            const std::optional<std::uint32_t> named = insn.target ? insn.target : Cpu::constant(insn);
            if (named && address_labels.contains(*named)) *t = described_source::name_number(*t, *named, address_labels.at(*named), false);
            instruction_text[at] = *t;
        } else {
            instruction_text[at] = said.fallback(described_source::byte_line(cpu, insn.bytes(), at, {}, ""));
        }
    }

    translate::l2::Assembly out(head_comment(description, values), description.dialect, file_name);
    for (const std::uint32_t a : used) out.append(described_source::equ(fixed.at(a), a, 4));
    if (!used.empty()) out.append("");
    out.part("ROM", "segment", std::nullopt, data.size());
    out.append(std::format("        .org 0x{:04x}", base));
    Listing8<Cpu> listing(data, base, code, instruction_text, said);
    iw::emit<Insn>(out, 0, static_cast<std::uint32_t>(data.size()), labels, functions, listing);
    out.append("");

    const std::string source = said.print(iw::text(out) + "\n");   // (said in the processor's assembler's: config/dialects/<dialect>.json)
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), data.size());
    iw::assembled(r, [&] { return described_source::assemble(cpu, said.parse(source), folder); }, data,
                  folder / fill(description.executable.built, values));
    return r;
}

}  // namespace

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    if (description.executable.cpu == "i8048") return write_with<I8048>(description, loaded, values, folder);
    if (description.executable.cpu == "f8") return write_with<F8>(description, loaded, values, folder);
    throw UsageError(std::format("console platform {}: the flat shape has no 8-bit processor {}", description.name, description.executable.cpu));
}

}  // namespace cb::source::console_writer::flat8_code
