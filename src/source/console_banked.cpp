// source/console_banked.cpp -- the console writer's `banked` shape (console_writer.hpp): a cartridge of fixed-size banks, the first ones mapped at
// power-on into the description's slots, a header that is data, vectors at fixed places, hardware named from the knowledge base. The
// processor is a CPU trait below (decode, flow, operands, text, build); everything else is the platform's description.

#include "source/console_shapes.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <set>

#include "core/fs.hpp"
#include "decompile/sm83.hpp"
#include "decompile/z80.hpp"
#include "knowledge/package.hpp"
#include "source/described_source.hpp"
#include "source/dialect.hpp"
#include "source/isa.hpp"
#include "source/processors.hpp"
#include "source/z80_source.hpp"

namespace cb::source::console_writer {

namespace {

namespace iw = image_writer;

// ---- the processors ----------------------------------------------------------------------------------------------

struct Z80Cpu {
    using Insn = decompile::z80::Insn;
    using Flow = decompile::z80::Flow;
    static std::optional<Insn> decode(ByteView memory, std::uint32_t pc) { return decompile::z80::decode(memory, pc, pc); }
    static bool stops(const Insn& insn) {   // (HALT resumes after an interrupt)
        return insn.flow == Flow::Jump || insn.flow == Flow::Return || insn.flow == Flow::Indirect;
    }
    static bool flows(const Insn& insn) { return insn.flow != Flow::None; }
    static std::optional<std::int64_t> operand(const Insn& insn) {
        if (insn.value_count == 0) return std::nullopt;
        return insn.values[insn.value_count - 1];
    }
    // a word loaded into a pointer register (HL DE IX IY SP; not BC, which mostly holds counts)
    static bool pointer_load(const Insn& insn) {
        return insn.pattern == "LD HL,nn" || insn.pattern == "LD DE,nn" || insn.pattern == "LD IX,nn" || insn.pattern == "LD IY,nn" || insn.pattern == "LD SP,nn";
    }
    static bool hardware_reference(const Insn& insn) { return isa::pattern_has(insn.pattern, "(nn)"); }
    static std::optional<std::uint32_t> port(const Insn& insn) {
        if ((insn.pattern != "OUT (n),A" && insn.pattern != "IN A,(n)") || insn.value_count == 0 || insn.values[0] < 0) return std::nullopt;
        return static_cast<std::uint32_t>(insn.values[0]);
    }
    static std::string text(const Insn& insn, ByteView, std::uint32_t, const decompile::AddressLabels& names, const iw::Names& ports, const dialect::Dialect& d) {
        return z80_source::code(insn, names, ports, d);   // (the description's text, the names put in, said in the dialect)
    }
    static std::vector<std::string> data_lines(ByteView data, const dialect::Dialect& d) { return z80_source::data_lines(data, d); }
    static std::string equ(const std::string& name, std::uint32_t value, unsigned digits, const dialect::Dialect& d) { return z80_source::equ(name, value, digits, d); }
    static Bytes assemble(const std::string& source, const std::filesystem::path& folder, const dialect::Dialect& d) { return z80_source::assemble(source, folder, d); }
};

struct Sm83Cpu {
    using Insn = decompile::sm83::Insn;
    using Flow = decompile::sm83::Flow;
    static const processors::Processor& cpu() {
        static const processors::Processor& p = processors::find("sm83");
        return p;
    }
    static std::optional<Insn> decode(ByteView memory, std::uint32_t pc) { return decompile::sm83::decode(memory, pc, pc); }
    static bool stops(const Insn& insn) {
        return insn.flow == Flow::Jump || insn.flow == Flow::Return || insn.flow == Flow::Indirect || insn.pattern == "STOP";
    }
    static bool flows(const Insn& insn) { return insn.flow != Flow::None; }
    static std::optional<std::int64_t> operand(const Insn& insn) {
        if (!insn.value) return std::nullopt;
        return *insn.value;
    }
    static bool pointer_load(const Insn& insn) { return insn.pattern == "LD HL,nn" || insn.pattern == "LD DE,nn" || insn.pattern == "LD SP,nn"; }
    static bool hardware_reference(const Insn& insn) { return insn.pattern.starts_with("LDH") || isa::pattern_has(insn.pattern, "(nn)"); }
    static std::optional<std::uint32_t> port(const Insn&) { return std::nullopt; }
    static std::string text(const Insn& insn, ByteView bytes, std::uint32_t pc, const decompile::AddressLabels& names, const iw::Names&, const dialect::Dialect& d) {
        std::optional<std::string> t = described_source::checked_text(cpu(), bytes, pc, insn.size);
        if (!t) {   // (the bytes with the description's reading in a comment)
            const std::vector<processors::Line> lines = processors::read_described(cpu(), bytes, pc, 1);
            std::string row;
            for (const std::uint8_t b : bytes) row += std::format("{}0x{:02x}", row.empty() ? "" : ",", b);
            return d.line(".byte " + row) + (lines.empty() || lines[0].text.starts_with(".d") ? std::string() : "  ; " + d.instruction(lines[0].text));
        }
        // the operand the program names: a target, or an address (a pointer loaded, an (nn) or LDH operand)
        std::optional<std::uint32_t> named;
        if (insn.target) named = *insn.target;
        else if (insn.value && *insn.value >= 0 && (pointer_load(insn) || hardware_reference(insn))) named = static_cast<std::uint32_t>(*insn.value);
        if (named && names.contains(*named)) *t = described_source::name_number(*t, *named, names.at(*named), false);
        return d.instruction(*t);
    }
    static std::vector<std::string> data_lines(ByteView data, const dialect::Dialect& d) { return d.data_lines(data); }
    static std::string equ(const std::string& name, std::uint32_t value, unsigned digits, const dialect::Dialect& d) { return d.line(described_source::equ(name, value, digits)); }
    static Bytes assemble(const std::string& source, const std::filesystem::path& folder, const dialect::Dialect& d) {
        return described_source::assemble(cpu(), d.parse(source), folder);
    }
};

// ---- flow and listing ---------------------------------------------------------------------------------------------

bool in_data(const Banked& b, std::uint32_t pc) {
    return std::any_of(b.data.begin(), b.data.end(), [&](const Range& r) { return pc >= r.from && pc < r.to; });
}

// Flow runs through the banks mapped at power-on, around the header.
template <class Cpu>
class BankedFlow final : public iw::CodeModel<typename Cpu::Insn> {
public:
    using Insn = typename Cpu::Insn;
    BankedFlow(const Banked& b, const Loaded& program)
        : b_(b), memory_(program.memory), limit_(static_cast<std::uint32_t>(std::min<std::size_t>(program.image.size(), b.power_on))) {}

    std::uint32_t limit() const { return limit_; }
    std::optional<Insn> decode(std::uint32_t pc) const override { return Cpu::decode(memory_, pc); }
    std::uint32_t size(const Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        return *insn.target;
    }
    bool inside(std::uint32_t pc) const override { return pc < limit_ && !in_data(b_, pc); }
    bool fits(std::uint32_t pc, const Insn& insn) const override {
        if (pc + insn.size > limit_) return false;
        return std::none_of(b_.data.begin(), b_.data.end(), [&](const Range& r) { return pc < r.from && r.from < pc + insn.size; });
    }
    bool follows(const Insn& insn) const override { return insn.target && *insn.target < limit_ && Cpu::flows(insn); }
    bool stops(const Insn& insn) const override { return Cpu::stops(insn); }

private:
    const Banked& b_;
    ByteView memory_;
    std::uint32_t limit_;
};

// One bank of the cartridge written as lines.
template <class Cpu>
class BankedListing final : public iw::Listing<typename Cpu::Insn> {
public:
    using Insn = typename Cpu::Insn;
    struct Place {
        std::size_t bank;
        std::uint32_t base, first, end;
        bool mapped;
    };
    BankedListing(translate::l2::Assembly& out, const Banked& b, const Loaded& program, const Place& place, const std::map<std::uint32_t, Insn>& code,
                  const decompile::AddressLabels& names, const iw::Names& ports, const dialect::Dialect& d)
        : out_(out), b_(b), image_(program.image), place_(place), code_(code), names_(names), ports_(ports), d_(d) {}

    std::uint8_t byte(std::uint32_t offset) const override { return image_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return place_.base + offset - place_.first; }
    const Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {   // (flow was followed in the mapped banks only)
        if (!place_.mapped) return nullptr;
        const auto it = code_.find(address);
        if (it == code_.end() || offset + it->second.size > place_.end) return nullptr;
        at_offset_ = offset, at_address_ = address;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const Insn& insn) const override { return insn.size; }
    std::string text(const Insn& insn) const override {
        return Cpu::text(insn, ByteView(image_).subspan(at_offset_, insn.size), at_address_, names_, ports_, d_);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return Cpu::data_lines(data, d_); }
    std::string label_line(const std::string& name) const override { return d_.label_line(name); }
    // A header field, with its name.
    std::uint32_t special(std::uint32_t, std::uint32_t address, const iw::Flush& flush) override {
        if (place_.bank != 0) return 0;
        const auto field = std::find_if(b_.header_fields.begin(), b_.header_fields.end(), [&](const HeaderField& f) { return f.at == address; });
        if (field == b_.header_fields.end()) return 0;
        flush();
        out_.extend(iw::commented(d_.data_lines(ByteView(image_).subspan(address, field->size)), field->what));
        return field->size;
    }

private:
    translate::l2::Assembly& out_;
    const Banked& b_;
    const Bytes& image_;
    Place place_;
    const std::map<std::uint32_t, Insn>& code_;
    const decompile::AddressLabels& names_;
    const iw::Names& ports_;
    const dialect::Dialect& d_;
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and its address)
};

// ---- the shape ----------------------------------------------------------------------------------------------------

// The hardware the description names, as ports (written as they are used by IN/OUT) and constants (addresses the code reads and writes).
struct Hardware {
    iw::Names ports, constants;
};

Hardware hardware_of(const Banked& b, const Description& d, const Values& values) {
    Hardware h;
    std::vector<HardwareTable> tables = b.hardware;
    const Variant* variant = nullptr;
    if (!b.variant_by.empty()) {
        const auto it = values.find(b.variant_by);
        if (it != values.end() && std::holds_alternative<std::string>(it->second)) {
            for (const Variant& v : b.variants) {
                if (v.when == std::get<std::string>(it->second)) variant = &v;
            }
        }
    }
    if (variant) tables.insert(tables.end(), variant->hardware.begin(), variant->hardware.end());
    for (const HardwareTable& t : tables) {
        if (!t.port && variant && !variant->constants) continue;
        const std::map<std::uint32_t, std::string> names = knowledge::standard().hardware_names(d.platform, t.section, t.spelling);
        (t.port ? h.ports : h.constants).insert(names.begin(), names.end());   // (an address named twice keeps the first table's name)
    }
    if (variant) {
        for (const std::uint32_t p : variant->drop_ports) h.ports.erase(p);
        for (const auto& [p, name] : variant->add_ports) h.ports[p] = name;
    }
    return h;
}

const HardwareTable* table_of(const Banked& b, bool port) {
    for (const HardwareTable& t : b.hardware) {
        if (t.port == port) return &t;
    }
    return nullptr;
}

template <class Cpu>
iw::Written write_banked_with(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values, std::string_view file_name,
                              const std::filesystem::path& folder) {
    using Insn = typename Cpu::Insn;
    const Banked& b = d.banked;
    const dialect::Dialect& said = dialect::find(d.dialect);
    const iw::Spelling sub{"sub_", 4}, local{"L_", 4}, data{"D_", 4};
    fs::make_dirs(folder);
    const std::filesystem::path file(file_name);
    const std::string stem = file.stem().string();
    const BankedFlow<Cpu> flow(b, program);
    const std::uint32_t limit = flow.limit();
    const iw::Walked<Insn> walked = iw::walk<Insn>(flow, program.starts);
    const std::map<std::uint32_t, Insn>& code = walked.code;
    const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());

    iw::Names names;   // a function is named for the vector it is at
    for (const VectorName& v : b.vectors) {
        if (v.always || starts.contains(v.at)) names.try_emplace(v.at, v.name);
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, sub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a < limit && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, local);
    // pointers into the program: a word loaded into a pointer register, or read and written through (nn), that starts a line past the floor
    for (const auto& entry : code) {
        const Insn& insn = entry.second;
        const std::optional<std::int64_t> operand = Cpu::operand(insn);
        if (!operand || *operand < 0) continue;
        const std::uint32_t v = static_cast<std::uint32_t>(*operand);
        if (!line_start(v) || v < b.data_floor) continue;
        if (std::any_of(b.no_data_labels.begin(), b.no_data_labels.end(), [&](const std::string& p) { return insn.pattern.starts_with(p); })) continue;
        if (Cpu::pointer_load(insn) || (!Cpu::flows(insn) && isa::pattern_has(insn.pattern, "(nn)"))) {
            labels.try_emplace(v, code.contains(v) ? (names.contains(v) ? names.at(v) : local(v)) : data(v));
        }
    }
    // the hardware the code names
    const Hardware known = hardware_of(b, d, values);
    iw::Names port_names, constants;
    for (const auto& entry : code) {
        const Insn& insn = entry.second;
        if (const auto p = Cpu::port(insn); p && known.ports.contains(*p)) port_names[*p] = known.ports.at(*p);
        const std::optional<std::int64_t> operand = Cpu::operand(insn);
        if (operand && *operand >= 0 && Cpu::hardware_reference(insn) && known.constants.contains(static_cast<std::uint32_t>(*operand))) {
            constants[static_cast<std::uint32_t>(*operand)] = known.constants.at(static_cast<std::uint32_t>(*operand));
        }
    }

    const std::size_t bank_count = (program.image.size() + b.bank_size - 1) / b.bank_size;
    translate::l2::Assembly out(head, b.assembler, std::string(file_name));
    if (!b.prologue.empty()) out.extend(said.block(b.prologue, {{"pages", std::to_string(bank_count)}}));
    const HardwareTable* port_table = table_of(b, true);
    const HardwareTable* constant_table = table_of(b, false);
    for (const auto& [a, name] : port_names) out.append(Cpu::equ(name, a, port_table ? port_table->digits : 2, said));
    for (const auto& [a, name] : constants) out.append(Cpu::equ(name, a, constant_table ? constant_table->digits : 4, said));
    if (!port_names.empty() || !constants.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const auto& [a, name] : constants) names_for_text[a] = name;

    const iw::Names no_labels;
    for (std::size_t bank = 0; bank < bank_count; ++bank) {
        const std::size_t slot = std::min(bank, b.slots.size() - 1);
        const std::uint32_t first = static_cast<std::uint32_t>(bank * b.bank_size);
        const std::uint32_t size = static_cast<std::uint32_t>(std::min<std::size_t>(b.bank_size, program.image.size() - first));
        const typename BankedListing<Cpu>::Place place{bank, b.slots[slot], first, first + size, bank < b.slots.size() && first < limit};
        out.part(std::format("bank{}", bank), "bank", std::nullopt, size);
        out.extend(said.block(b.bank_blocks[std::min(bank, b.bank_blocks.size() - 1)], {{"bank", std::to_string(bank)}, {"slot", std::to_string(slot)}}));
        BankedListing<Cpu> listing(out, b, program, place, code, names_for_text, port_names, said);
        iw::emit<Insn>(out, place.first, place.end, place.mapped ? labels : no_labels, functions, listing);
        out.append("");
    }
    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.image.size());
    iw::assembled(r, [&] { return Cpu::assemble(source, folder, said); }, program.image, folder / (stem + ".built" + file.extension().string()));
    return r;
}

}  // namespace

image_writer::Written write_banked(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                   std::string_view file_name, const std::filesystem::path& folder) {
    if (d.banked.cpu == "z80") return write_banked_with<Z80Cpu>(d, program, head, values, file_name, folder);
    if (d.banked.cpu == "sm83") return write_banked_with<Sm83Cpu>(d, program, head, values, file_name, folder);
    throw UsageError(std::format("console platform {}: the banked shape has no processor {}", d.name, d.banked.cpu));
}

}  // namespace cb::source::console_writer
