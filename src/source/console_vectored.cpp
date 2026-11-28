// source/console_vectored.cpp -- the console writer's `vectored` shape (console_writer.hpp): a 68000 image whose code starts from a vector table
// or a header, written as its pieces each at its address (an `.org`), a vector table as the handlers' labels, the header's fields each as data
// with its name. The Genesis / Mega Drive (the 64-long vector table and the header at $100) and the Atari Jaguar (a cartridge's boot signature
// and header, or a COFF executable's sections in address order).
//
// Code is what flow reaches from the loader's starts (and, when the description says so, every JSR and BSR target is a function); every byte
// it reaches is written as an instruction, every other byte as data. Functions are named by the loader or `sub_<address>`, jump targets
// `L_<address>`, an absolute, PC-relative or long immediate operand naming the start of a line gets that line's label (`D_<address>` for
// data), and the hardware's registers are written by name.

#include "source/console_shapes.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/m68k.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer {

namespace {

namespace iw = image_writer;
namespace m68k = decompile::m68k;

constexpr iw::Spelling kVecSub{"sub_", 6};
constexpr iw::Spelling kVecLocal{"L_", 6};
constexpr iw::Spelling kVecData{"D_", 6};

std::uint32_t long_at(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}

// Flow runs through the code ranges at even addresses.
class VecFlow final : public iw::CodeModel<m68k::Insn> {
public:
    VecFlow(const Loaded& program, bool calls) : memory_(program.memory), ranges_(program.segments.code_ranges), calls_(calls) {}

    const Range* range_of(std::uint32_t a) const {
        for (const Range& r : ranges_) {
            if (a >= r.from && a < r.to) return &r;
        }
        return nullptr;
    }
    bool line_start(std::uint32_t a) const { return a % 2 == 0 && range_of(a) != nullptr; }

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(memory_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return line_start(pc); }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override {
        const Range* r = range_of(pc);
        return r && pc + insn.length <= r->to;
    }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && line_start(*insn.target) && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }
    bool finds_functions() const override { return calls_; }
    bool starts_function(const m68k::Insn& insn) const override { return insn.mnemonic == "JSR" || insn.mnemonic == "BSR"; }

private:
    const Bytes& memory_;
    const std::vector<Range>& ranges_;
    bool calls_;
};

// The addresses an instruction's operands name (absolute, PC-relative, long immediates), and whether each is an immediate.
struct VecReference {
    std::uint32_t value;
    bool immediate;
};
std::vector<VecReference> vec_references(const m68k::Insn& insn) {
    std::vector<VecReference> out;
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

// One code piece written as lines.
class VecListing final : public iw::Listing<m68k::Insn> {
public:
    VecListing(translate::l2::Assembly& out, const Vectored& shape, const Values& values, const Loaded& program, const Piece& piece,
               const std::map<std::uint32_t, m68k::Insn>& code, const iw::Names& labels, const decompile::AddressLabels& names, const dialect::Dialect& d)
        : out_(out), shape_(shape), values_(values), program_(program), piece_(piece), code_(code), labels_(labels), names_(names), d_(d) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.image[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return piece_.address + (offset - piece_.offset); }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return insn.length; }
    std::string text(const m68k::Insn& insn) const override { return said(d_.fallback(risc_source::m68k_code(insn, names_))); }
    std::vector<std::string> data_lines(ByteView data) const override { return rows(data); }

    // The header's fields, each as data with its name.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        const VectorField* field = nullptr;
        for (const VectorField& f : shape_.fields) {
            if (f.at == offset && (f.when.empty() || is_set(f.when))) field = &f;
        }
        if (!field) return 0;
        flush();
        const ByteView bytes = ByteView(program_.image).subspan(offset, field->size);
        if (field->as == "long") {
            out_.append(said(std::format("        .long 0x{:08x}  ; {}", long_at(bytes, 0), field->what)));
        } else if (field->as == "pointer") {
            const std::uint32_t v = long_at(bytes, 0);
            const auto named = labels_.find(v);
            out_.append(said("        .long " + (named != labels_.end() ? named->second : std::format("0x{:08x}", v)) + "  ; " + field->what));
        } else {
            const bool plain = field->as == "text" &&
                               std::ranges::all_of(bytes, [](std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22 && b != 0x5C && b != 0x3B; });
            out_.extend(iw::commented(plain ? std::vector<std::string>{said("        .ascii \"" + std::string(as_text(bytes)) + "\"")} : rows(bytes), field->what));
        }
        return field->size;
    }

private:
    std::string said(std::string line) const { return shape_.whole ? line : d_.line(line); }
    std::vector<std::string> rows(ByteView data) const { return shape_.neutral_rows ? risc_source::data_lines(data) : d_.data_lines(data); }
    bool is_set(const std::string& name) const {
        const auto it = values_.find(name);
        return it != values_.end() && std::holds_alternative<std::int64_t>(it->second) && std::get<std::int64_t>(it->second) != 0;
    }

    translate::l2::Assembly& out_;
    const Vectored& shape_;
    const Values& values_;
    const Loaded& program_;
    const Piece& piece_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& names_;
    const dialect::Dialect& d_;
};

}  // namespace

image_writer::Written write_vectored(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                     std::string_view file_name, const std::filesystem::path& folder) {
    const Vectored& shape = d.vectored;
    if (shape.cpu != "m68k") throw UsageError(std::format("console platform {}: the vectored shape has no processor {}", d.name, shape.cpu));
    const dialect::Dialect& said = dialect::find(d.dialect);
    const auto say = [&](std::string line) { return shape.whole ? line : said.line(line); };
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const VecFlow flow(program, shape.calls_start_functions);
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, program.starts);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;

    iw::Names names = program.entries;
    const iw::Functions functions = iw::name_functions(walked.starts, names, kVecSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return flow.line_start(a) && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kVecLocal);
    const iw::Names registers = shape.registers.empty() ? iw::Names{} : knowledge::standard().hardware_names(d.platform, shape.registers);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const VecReference& ref : vec_references(entry.second)) {
            if (registers.contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value) || ref.value >= shape.immediate_floor)) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kVecLocal : kVecData)(ref.value));
            }
        }
    }

    translate::l2::Assembly out(head, shape.assembler.empty() ? d.dialect : shape.assembler, std::string(file_name));
    for (const std::uint32_t a : used) out.append(say(risc_source::equ(registers.at(a), a, 6)));
    if (!used.empty()) out.append("");
    decompile::AddressLabels names_for_text;   // the labels and the registers: a register wins
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = registers.at(a);

    for (const Piece& piece : program.segments.pieces) {
        out.part(piece.segment, piece.role == "vectors" ? "vector table" : "segment", std::nullopt, piece.size);
        if (piece.org) out.append(say(std::format("        .org 0x{:0{}x}", piece.address, shape.org_digits)));
        if (piece.role == "vectors") {   // each long the handler's label (after the plain ones), with what the vector is
            std::vector<std::string> what;
            if (const auto it = values.find(shape.vector_names); it != values.end() && std::holds_alternative<std::vector<std::string>>(it->second)) {
                what = std::get<std::vector<std::string>>(it->second);
            }
            for (std::uint32_t i = 0; i * 4 + 4 <= piece.size; ++i) {
                const std::uint32_t v = long_at(program.image, piece.offset + i * 4);
                std::string target = std::format("0x{:08x}", v);
                if (i >= shape.vectors_plain) {
                    if (const auto named = labels.find(v); named != labels.end()) target = named->second;
                }
                const std::string& w = i < what.size() ? what[i] : std::string();
                out.append(say("        .long " + target + (w.empty() ? std::string() : "  ; " + w)));
            }
        } else if (piece.code) {
            VecListing listing(out, shape, values, program, piece, code, labels, names_for_text, said);
            iw::emit<m68k::Insn>(out, piece.offset, piece.offset + piece.size, labels, functions, listing);
        } else {
            const ByteView bytes = ByteView(program.image).subspan(piece.offset, piece.size);
            out.extend(shape.neutral_rows ? risc_source::data_lines(bytes) : said.data_lines(bytes));
        }
        if (shape.blank_after_piece) out.append("");
    }

    const std::string source = shape.whole ? said.print(iw::text(out) + "\n") : iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + "." + shape.source, source, functions, iw::code_bytes(code, flow), program.segments.measured);
    iw::assembled(
        r,
        [&] {
            Bytes image = risc_source::assemble(shape.cpu, said.parse(source), folder);
            return program.segments.rebuilt ? program.segments.rebuilt(image) : image;
        },
        program.image, folder / fill(shape.built, values));
    return r;
}

}  // namespace cb::source::console_writer
