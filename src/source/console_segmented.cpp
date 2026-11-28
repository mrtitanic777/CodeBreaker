// source/console_segmented.cpp -- the console writer's `segmented` shape (console_writer.hpp): a 6502-family program written as ca65 segments,
// each in a memory area of its own that an ld65 configuration (written beside the source) lays out in file order, so the source builds the
// whole file. The loader cuts the file into pieces (Piece): code is linked at the address the processor sees it at and written as instructions
// where flow reaches it, data elsewhere; the other pieces are bytes as their role says (data rows, one `.byte` row, a file of their own taken
// in with `.incbin`, a comment ahead). The NES (its CHR beside the source) and the Atari Lynx (its encrypted first stage and the stretches
// the cartridge's loaders copy to RAM).
//
// Flow runs through the processor's address space (the loader's `memory`); the image offset an address shows (`offset_of`) files each
// instruction, and a piece writes an instruction or a label only where it is the piece the address shows.

#include "source/console_shapes.hpp"

#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/m6502.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer {

namespace {

namespace iw = image_writer;
namespace m6502 = decompile::m6502;

constexpr iw::Spelling kSegSub{"sub_", 4};
constexpr iw::Spelling kSegLocal{"L_", 4};

bool seg_absolute(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

struct SegCpu {
    m6502_source::Cpu cpu = m6502_source::Cpu::M6502;
    m6502::Variant variant = m6502::Variant::Nmos;
};

SegCpu seg_cpu(const Description& d) {
    if (d.segmented.cpu == "6502") return {};
    if (d.segmented.cpu == "65sc02") return {m6502_source::Cpu::M65C02, m6502::Variant::Cmos};
    if (d.segmented.cpu == "huc6280") return {m6502_source::Cpu::Huc6280, m6502::Variant::Cmos};   // (flow is the loader's: console_pce.cpp)
    if (d.segmented.cpu == "65816") return {m6502_source::Cpu::W65816, m6502::Variant::Cmos};      // (console_snes.cpp)
    throw UsageError(std::format("console platform {}: the segmented shape has no processor {}", d.name, d.segmented.cpu));
}

const SegmentRole* role_of(const Segmented& s, const Piece& piece) {
    if (piece.role.empty()) return nullptr;
    for (const SegmentRole& r : s.roles) {
        if (r.name == piece.role) return &r;
    }
    throw UsageError(std::format("a loader gives the role {}, which the description does not have", piece.role));
}

// Flow runs through the address space; the image offset an address shows files the instruction.
class SegFlow final : public iw::CodeModel<m6502::Insn> {
public:
    SegFlow(const Loaded& program, m6502::Variant variant) : memory_(program.memory), offset_of_(program.segments.offset_of), variant_(variant) {}

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override { return m6502::decode(memory_, pc, pc, variant_); }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc <= 0xFFFF && offset_of_(pc).has_value(); }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override {
        const std::optional<std::uint32_t> first = inside(pc) ? offset_of_(pc) : std::nullopt;
        if (!first) return false;
        for (std::uint32_t i = 1; i < insn.size; ++i) {
            if (!inside(pc + i) || offset_of_(pc + i) != *first + i) return false;
        }
        return true;
    }
    bool follows(const m6502::Insn& insn) const override { return insn.target && inside(*insn.target); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return *offset_of_(pc); }   // (fits() has seen it shown)

    // The address an image offset of a code piece is linked at when it is the byte that address shows, else nullopt.
    std::optional<std::uint32_t> shown_at(const Piece& piece, std::uint32_t offset) const {
        const std::uint32_t address = piece.address + (offset - piece.offset);
        if (!inside(address) || offset_of_(address) != std::optional<std::uint32_t>(offset)) return std::nullopt;
        return address;
    }

private:
    const Bytes& memory_;
    const std::function<std::optional<std::uint32_t>(std::uint32_t)>& offset_of_;
    m6502::Variant variant_;
};

// One code piece written as lines. `whole`: the instructions are the neutral text, said in the dialect when the source is done.
class SegListing final : public iw::Listing<m6502::Insn> {
public:
    SegListing(translate::l2::Assembly& out, const Segmented& shape, SegCpu cpu, const Loaded& program, const Piece& piece, const SegFlow& flow,
               const std::map<std::uint32_t, m6502::Insn>& code, const iw::Names& labels, const decompile::AddressLabels& names, const dialect::Dialect& d)
        : out_(out), shape_(shape), cpu_(cpu), program_(program), piece_(piece), flow_(flow), code_(code), labels_(labels), names_(names), d_(d),
          tail_here_(shape.tail > 0 && shape.tail_at >= piece.address && shape.tail_at + shape.tail <= piece.address + piece.size) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.image[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return piece_.address + offset - piece_.offset; }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        if (!flow_.shown_at(piece_, offset)) return nullptr;
        const auto it = code_.find(offset);
        if (it == code_.end() || offset + it->second.size > piece_.offset + piece_.size) return nullptr;
        if (tail_here_ && address + it->second.size > shape_.tail_at) return nullptr;   // (the tail is data)
        at_offset_ = offset, at_address_ = address;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the processor description's text, the names put in)
        const std::string t = d_.fallback(m6502_source::code(cpu_.cpu, ByteView(program_.image).subspan(at_offset_, insn.size), at_address_, names_));
        return shape_.whole ? t : d_.line(t);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return d_.data_lines(data); }

    // The tail (the vectors), as the description's lines say it ({wN}: the word at N, by its label; {bN}: the byte).
    std::uint32_t special(std::uint32_t offset, std::uint32_t address, const iw::Flush& flush) override {
        if (!tail_here_ || address != shape_.tail_at) return 0;
        flush();
        Values v;
        for (std::uint32_t i = 0; i < shape_.tail; ++i) {
            const std::uint32_t at = offset + i;
            v.insert_or_assign(std::format("b{}", i), static_cast<std::int64_t>(program_.image[at]));
            if (i + 1 < shape_.tail) {
                const std::uint32_t word = program_.image[at] | static_cast<std::uint32_t>(program_.image[at + 1]) << 8;
                const auto named = labels_.find(word);
                v.insert_or_assign(std::format("w{}", i), named != labels_.end() ? named->second : std::format("0x{:04x}", word));
            }
        }
        for (const std::string& line : shape_.tail_lines) out_.append(shape_.whole ? fill(line, v) : d_.line(fill(line, v)));
        return shape_.tail;
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the image offset of the instruction code_at() last gave, and the address it is shown at)
    translate::l2::Assembly& out_;
    const Segmented& shape_;
    SegCpu cpu_;
    const Loaded& program_;
    const Piece& piece_;
    const SegFlow& flow_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& names_;
    const dialect::Dialect& d_;
    bool tail_here_;
};

// The plain flow (the NES, the Lynx): through one address space from the loader's starts, the loader's entries named.
class FlatCode final : public SegmentCode {
public:
    FlatCode(const Loaded& program, SegCpu cpu) : program_(program), cpu_(cpu), flow_(program, cpu.variant) {}

    void follow(const Description& d) override {
        d_ = &d;
        said_ = &dialect::find(d.dialect);
        const Segmented& shape = d.segmented;
        walked_ = iw::walk<m6502::Insn>(flow_, program_.starts);
        const std::map<std::uint32_t, m6502::Insn>& code = walked_.code;
        iw::Names names = program_.entries;
        functions_ = iw::name_functions(walked_.starts, names, kSegSub);

        // a label can only stand where a line starts, in the piece that shows the address
        const std::set<std::uint32_t> inside_insn = iw::covered(code, flow_);   // image offsets inside an instruction
        std::set<std::uint32_t> line_at;
        for (const Piece& piece : program_.segments.pieces) {
            if (!piece.code) continue;
            for (std::uint32_t off = piece.offset; off < piece.offset + piece.size; ++off) {
                if (const auto address = flow_.shown_at(piece, off); address && !inside_insn.contains(off)) line_at.insert(*address);
            }
        }
        labels_ = iw::make_labels(walked_.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, kSegLocal);
        for (const auto& entry : code) {   // absolute operands that name a line's start
            const m6502::Insn& insn = entry.second;
            if (seg_absolute(insn.mode) && insn.mnemonic != "JMP" && insn.mnemonic != "JSR" && insn.operand && *insn.operand >= shape.data_floor &&
                line_at.contains(*insn.operand)) {
                labels_.try_emplace(*insn.operand, std::format("D_{:04X}", *insn.operand));
            }
        }
        registers_ = shape.registers.empty() ? iw::Names{} : knowledge::standard().hardware_names(d.platform, shape.registers);
        for (const auto& entry : code) {
            const m6502::Insn& insn = entry.second;
            if (seg_absolute(insn.mode) && insn.operand && registers_.contains(*insn.operand)) used_.insert(*insn.operand);
        }
        for (const std::uint32_t a : used_) names_for_text_[a] = registers_.at(a);   // (the registers and the labels: a label wins)
        for (const auto& [a, label] : labels_) names_for_text_[a] = label;
    }
    iw::Functions functions() const override { return functions_; }
    std::size_t code_bytes() const override { return iw::code_bytes(walked_.code, flow_); }
    std::vector<std::string> names() const override {
        std::vector<std::string> lines;
        for (const std::uint32_t a : used_) {
            const std::string e = m6502_source::equ(registers_.at(a), a, 4);
            lines.push_back(d_->segmented.whole ? e : said_->line(e));
        }
        if (!used_.empty()) lines.push_back("");
        return lines;
    }
    void write(translate::l2::Assembly& out, const Piece& piece) override {
        iw::Names mine;   // the labels of the addresses this piece shows
        for (const auto& [a, label] : labels_) {
            if (a >= piece.address && a < piece.address + piece.size && flow_.shown_at(piece, piece.offset + (a - piece.address))) mine[a] = label;
        }
        SegListing listing(out, d_->segmented, cpu_, program_, piece, flow_, walked_.code, labels_, names_for_text_, *said_);
        iw::emit<m6502::Insn>(out, piece.offset, piece.offset + piece.size, mine, functions_, listing);
    }

private:
    const Loaded& program_;
    SegCpu cpu_;
    SegFlow flow_;
    const Description* d_ = nullptr;
    const dialect::Dialect* said_ = nullptr;
    iw::Walked<m6502::Insn> walked_;
    iw::Functions functions_;
    iw::Names labels_, registers_;
    std::set<std::uint32_t> used_;
    decompile::AddressLabels names_for_text_;
};

// The ld65 configuration: each piece a memory area of its size, in image order, filled (the dialect reads it back).
std::string link_config(const std::vector<Piece>& pieces, const SegmentCode& code, unsigned digits) {
    std::vector<std::string> cfg = {"MEMORY {"};
    for (const Piece& p : pieces) {
        cfg.push_back(std::format("    {}: start = ${:0{}X}, size = ${:04X}, fill = yes, file = %O;", p.area, p.code ? code.linked_at(p) : p.address, digits, p.size));
    }
    cfg.insert(cfg.end(), {"}", "SEGMENTS {"});
    for (const Piece& p : pieces) cfg.push_back(std::format("    {}: load = {}, type = ro;", p.segment, p.area));
    cfg.push_back("}");
    return text::join(cfg, "\n") + "\n";
}

}  // namespace

image_writer::Written write_segmented(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                      std::string_view file_name, const std::filesystem::path& folder) {
    const Segmented& shape = d.segmented;
    const std::vector<Piece>& pieces = program.segments.pieces;
    const SegCpu cpu = seg_cpu(d);
    const dialect::Dialect& said = dialect::find(d.dialect);
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    FlatCode flat(program, cpu);
    SegmentCode& code = program.segments.code ? *program.segments.code : static_cast<SegmentCode&>(flat);
    code.follow(d);

    std::vector<std::string> lines = head;
    lines.insert(lines.end(), shape.prologue.begin(), shape.prologue.end());
    for (std::string& l : code.names()) lines.push_back(std::move(l));
    translate::l2::Assembly out(lines, shape.assembler.empty() ? d.dialect : shape.assembler, std::string(file_name));

    for (const Piece& piece : pieces) {
        const ByteView bytes = ByteView(program.image).subspan(piece.offset, piece.size);
        const SegmentRole* role = role_of(shape, piece);
        out.part(piece.segment, "segment", std::nullopt, piece.size);
        out.extend(said.block("segment", {{"name", piece.segment}}));
        if (role && !role->note.empty()) out.append("; " + fill(role->note, values));
        if (piece.code) {
            code.write(out, piece);
        } else if (role && role->as == "incbin") {
            const std::string name = fill(role->file, values);
            fs::write_file(folder / name, bytes);   // (beside the source, which names it)
            out.append(std::format("        .incbin \"{}\"", name));
        } else if (role && role->as == "row") {
            std::vector<std::string> items;
            for (const std::uint8_t b : bytes) items.push_back(said.number(b, 2));
            out.append("        .byte " + text::join(items, ", "));
        } else {
            std::vector<std::string> rows = role && role->neutral ? m6502_source::data_lines(bytes) : said.data_lines(bytes);
            out.extend(role && !role->what.empty() ? iw::commented(std::move(rows), role->what) : rows);
        }
        out.append("");
    }
    const std::string source = shape.whole ? said.print(iw::text(out) + "\n") : iw::text(out) + "\n";
    const std::string config = link_config(pieces, code, shape.address_digits);
    const std::filesystem::path config_path = folder / (stem + ".cfg");
    fs::write_file(config_path, as_bytes(config));

    iw::Written r = iw::finish(folder, stem + "." + shape.source, source, code.functions(), code.code_bytes(), program.segments.measured);
    r.config = config_path;
    iw::assembled(r, [&] { return m6502_source::assemble(cpu.cpu, said.parse(source, {config, folder}), folder); }, program.image, folder / fill(shape.built, values));
    return r;
}

}  // namespace cb::source::console_writer
