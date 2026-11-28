// source/console_cart6502.cpp -- the console writer's `cart6502` shape (console_writer.hpp): a 6502 cartridge of one or more banks, each seen in
// a window of the address space, its last bytes data (the vectors, a header), its hardware named by the chips' read and write names (a store
// names the write register, a load the read one: the chips have a different register at the same address). The Atari 2600 and 5200.
//
// Flow addresses are `bank << 16 | address in the window`; the loader says (Cart) which addresses a bank shows, where a target folds to and
// which file offset an address is.

#include "source/console_shapes.hpp"

#include <format>
#include <set>

#include "core/fs.hpp"
#include "decompile/m6502.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer {

namespace {

namespace iw = image_writer;
namespace m6502 = decompile::m6502;

// ---- the 6502 -----------------------------------------------------------------------------------------------------

// Does the instruction write the address it names (so the write register's name stands)?
bool writes(std::string_view mnemonic) {
    return mnemonic == "STA" || mnemonic == "STX" || mnemonic == "STY" || mnemonic == "STZ" || mnemonic == "INC" || mnemonic == "DEC" ||
           mnemonic == "ASL" || mnemonic == "LSR" || mnemonic == "ROL" || mnemonic == "ROR";
}

// Does the operand name a memory address (and not a value, a branch target or a pointer)?
bool names_address(m6502::Mode mode) {
    switch (mode) {
        case m6502::Mode::Zp:
        case m6502::Mode::Zpx:
        case m6502::Mode::Zpy:
        case m6502::Mode::Abs:
        case m6502::Mode::Abx:
        case m6502::Mode::Aby: return true;
        default: return false;
    }
}

bool absolute_mode(m6502::Mode mode) { return mode == m6502::Mode::Abs || mode == m6502::Mode::Abx || mode == m6502::Mode::Aby; }

std::uint32_t flow_address(std::size_t bank, std::uint32_t address) { return static_cast<std::uint32_t>(bank) << 16 | address; }

// The chips' registers, read and write names (the description's `hardware`: {read, write} knowledge sections, in the order they are looked in).
struct Registers {
    std::vector<std::pair<iw::Names, iw::Names>> chips;   // (read, write)

    const std::string* name(const m6502::Insn& insn) const {
        if (!names_address(insn.mode) || !insn.operand) return nullptr;
        const bool store = writes(insn.mnemonic);
        for (const auto& [read, write] : chips) {
            const iw::Names& table = store ? write : read;
            if (const auto it = table.find(*insn.operand); it != table.end()) return &it->second;
        }
        return nullptr;
    }
    bool reads_as(std::uint32_t value, const std::string& name) const {
        return std::any_of(chips.begin(), chips.end(), [&](const auto& c) { const auto it = c.first.find(value); return it != c.first.end() && it->second == name; });
    }
    bool writes_as(std::uint32_t value, const std::string& name) const {
        return std::any_of(chips.begin(), chips.end(), [&](const auto& c) { const auto it = c.second.find(value); return it != c.second.end() && it->second == name; });
    }
};

Registers registers_of(const Description& d) {
    Registers r;
    for (const ChipTable& c : d.cart.chips) {
        r.chips.emplace_back(knowledge::standard().hardware_names(d.platform, c.read), knowledge::standard().hardware_names(d.platform, c.write));
    }
    return r;
}

// ---- flow and listing ---------------------------------------------------------------------------------------------

class CartFlow final : public iw::CodeModel<m6502::Insn> {
public:
    explicit CartFlow(const Cart& cart) : cart_(cart) {}

    std::optional<m6502::Insn> decode(std::uint32_t pc) const override {
        const std::size_t bank = pc >> 16;
        if (bank >= cart_.banks.size()) return std::nullopt;
        const std::uint32_t at = pc & 0xFFFF;
        std::optional<m6502::Insn> insn = m6502::decode(cart_.banks[bank].memory, at, at);
        if (insn) insn->at = pc;
        return insn;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const m6502::Insn& insn) const override {
        if (!insn.target) return std::nullopt;
        const auto folded = cart_.fold(*insn.target);
        if (!folded) return std::nullopt;
        return flow_address(insn.at >> 16, *folded);
    }
    bool inside(std::uint32_t pc) const override { return (pc >> 16) < cart_.banks.size() && cart_.inside(pc >> 16, pc & 0xFFFF); }
    bool fits(std::uint32_t pc, const m6502::Insn& insn) const override { return cart_.fits(pc >> 16, pc & 0xFFFF, insn.size); }
    bool follows(const m6502::Insn& insn) const override { return target(insn).has_value(); }
    bool stops(const m6502::Insn& insn) const override { return m6502::ends_flow(insn.mnemonic); }
    std::uint32_t key(std::uint32_t pc) const override { return cart_.offset(pc >> 16, pc & 0xFFFF); }
    bool finds_functions() const override { return true; }
    bool starts_function(const m6502::Insn& insn) const override { return insn.mnemonic == "JSR"; }

private:
    const Cart& cart_;
};

// One bank of the cartridge written as lines. `whole`: the lines are the neutral text, said in the dialect when the source is done.
class CartListing final : public iw::Listing<m6502::Insn> {
public:
    CartListing(translate::l2::Assembly& out, const CartShape& shape, const Loaded& program, std::size_t bank, const std::map<std::uint32_t, m6502::Insn>& code,
                const iw::Names& labels, const decompile::AddressLabels& read_names, const decompile::AddressLabels& write_names, const dialect::Dialect& d)
        : out_(out), shape_(shape), program_(program), bank_(bank), b_(program.cart.banks[bank]), code_(code), labels_(labels), read_names_(read_names),
          write_names_(write_names), d_(d) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.image[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return flow_address(bank_, b_.window + offset - b_.offset); }
    const m6502::Insn* code_at(std::uint32_t offset, std::uint32_t address) const override {
        const auto it = code_.find(offset);
        if (it == code_.end()) return nullptr;
        if (offset + it->second.size > b_.offset + b_.size - shape_.tail) return nullptr;   // (the tail is data)
        at_offset_ = offset, at_address_ = address & 0xFFFF;   // (text() of this instruction follows)
        return &it->second;
    }
    std::uint32_t size(const m6502::Insn& insn) const override { return insn.size; }
    std::string text(const m6502::Insn& insn) const override {   // (the description's text, the names put in)
        const decompile::AddressLabels& names = writes(insn.mnemonic) ? write_names_ : read_names_;
        const std::string t = m6502_source::in_dialect(d_, insn, names, m6502_source::code(m6502_source::Cpu::M6502, ByteView(program_.image).subspan(at_offset_, insn.size), at_address_, names));
        return shape_.whole ? t : d_.line(t);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return shape_.whole ? m6502_source::data_lines(data) : d_.data_lines(data); }
    std::string label_line(const std::string& name) const override { return shape_.whole ? name + ":" : d_.label_line(name); }

    // The bank's last bytes: the vectors or the header, as the description's tail lines say them ({wN}: the word at N, by its label; {bN}: the byte).
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (shape_.tail == 0 || offset != b_.offset + b_.size - shape_.tail) return 0;
        flush();
        Values v;
        for (std::uint32_t i = 0; i < shape_.tail; ++i) {
            const std::uint32_t at = offset + i;
            v.insert_or_assign(std::format("b{}", i), static_cast<std::int64_t>(program_.image[at]));
            if (i + 1 < shape_.tail) {
                const std::uint32_t word = program_.image[at] | static_cast<std::uint32_t>(program_.image[at + 1]) << 8;
                const auto named = labels_.find(flow_address(bank_, word));
                v.insert_or_assign(std::format("w{}", i), named != labels_.end() ? named->second : std::format("0x{:04x}", word));
            }
        }
        for (const std::string& line : shape_.tail_lines) out_.append(shape_.whole ? fill(line, v) : d_.line(fill(line, v)));
        return shape_.tail;
    }

private:
    mutable std::uint32_t at_offset_ = 0, at_address_ = 0;   // (the file offset of the instruction code_at() last gave, and the window address it is shown at)
    translate::l2::Assembly& out_;
    const CartShape& shape_;
    const Loaded& program_;
    std::size_t bank_;
    const CartBank& b_;
    const std::map<std::uint32_t, m6502::Insn>& code_;
    const iw::Names& labels_;
    const decompile::AddressLabels& read_names_;
    const decompile::AddressLabels& write_names_;
    const dialect::Dialect& d_;
};

}  // namespace

image_writer::Written write_cart6502(const Description& d, const Loaded& program, const std::vector<std::string>& head, std::string_view file_name,
                                     const std::filesystem::path& folder) {
    const CartShape& shape = d.cart;
    const Cart& cart = program.cart;
    const dialect::Dialect& said = dialect::find(d.dialect);
    fs::make_dirs(folder);
    const std::filesystem::path file(file_name);
    const std::string stem = file.stem().string();
    const bool banked = cart.banks.size() > 1;
    const iw::Spelling sub{"sub_", banked ? 6u : 4u}, local{"L_", banked ? 6u : 4u};
    const CartFlow flow(cart);

    iw::Names names = program.entries;
    const iw::Walked<m6502::Insn> walked = iw::walk<m6502::Insn>(flow, program.starts);
    const std::map<std::uint32_t, m6502::Insn>& code = walked.code;
    const iw::Functions functions = iw::name_functions(walked.starts, names, sub);

    // a label can only stand where a line starts
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);   // file offsets inside an instruction
    std::set<std::uint32_t> line_at;
    for (std::size_t b = 0; b < cart.banks.size(); ++b) {
        const CartBank& bank = cart.banks[b];
        for (std::uint32_t off = bank.offset; off < bank.offset + bank.size; ++off) {
            if (!inside_insn.contains(off)) line_at.insert(flow_address(b, bank.window + off - bank.offset));
        }
    }
    iw::Names labels = iw::make_labels(walked.targets, names, [&](std::uint32_t a) { return line_at.contains(a); }, local);
    for (const auto& entry : code) {   // absolute operands that name a line's start (exactly: a mirror's address is left as it is)
        const m6502::Insn& insn = entry.second;
        if (!absolute_mode(insn.mode) || !insn.operand || *insn.operand < shape.data_floor || cart.fold(*insn.operand) != std::optional<std::uint32_t>(*insn.operand)) continue;
        const std::uint32_t at = flow_address(insn.at >> 16, *insn.operand);
        if (line_at.contains(at)) labels.try_emplace(at, std::format("D_{:0{}X}", banked ? at : *insn.operand, banked ? 6 : 4));
    }
    const Registers registers = registers_of(d);
    std::map<std::uint32_t, std::set<std::string>> equates;   // the register names the code uses, by address
    for (const auto& entry : code) {
        if (const std::string* name = registers.name(entry.second)) equates[*entry.second.operand].insert(*name);
    }

    std::vector<std::string> lines = head;
    for (const std::string& l : said.block(shape.prologue, {{"stem", stem}, {"ext", shape.ext}})) lines.push_back(l);
    for (const auto& [value, set] : equates) {
        for (const std::string& name : set) {
            const std::string e = m6502_source::equ(name, value, shape.equ_digits ? shape.equ_digits : (value > 0xFF ? 4 : 2));
            lines.push_back(shape.whole ? e : said.line(e));
        }
    }
    if (!equates.empty()) lines.push_back("");
    translate::l2::Assembly out(lines, d.banked.assembler.empty() ? d.dialect : d.banked.assembler, std::string(file_name));

    for (std::size_t b = 0; b < cart.banks.size(); ++b) {
        const CartBank& bank = cart.banks[b];
        // the names of the registers and the labels by address, for the instruction text (a label wins)
        decompile::AddressLabels read_names, write_names;
        for (const auto& [value, set] : equates) {
            for (const std::string& name : set) {
                if (registers.reads_as(value, name)) read_names[value] = name;
                if (registers.writes_as(value, name)) write_names[value] = name;
            }
        }
        for (const auto& [at, label] : labels) {
            if ((at >> 16) != b) continue;
            read_names[static_cast<std::uint16_t>(at & 0xFFFF)] = label;
            write_names[static_cast<std::uint16_t>(at & 0xFFFF)] = label;
        }
        out.part(fill(shape.part, {{"bank", static_cast<std::int64_t>(b)}}), "segment", std::nullopt, bank.size);
        if (shape.bank_block.empty()) {
            out.append(std::format("        .org 0x{:04x}", bank.window));
        } else {
            out.extend(said.block(shape.bank_block, {{"offset", said.number(bank.offset, 4)}, {"window", said.number(bank.window, 4)}}));
        }
        CartListing listing(out, shape, program, b, code, labels, read_names, write_names, said);
        iw::emit<m6502::Insn>(out, bank.offset, bank.offset + bank.size, labels, functions, listing);
        out.append("");
    }
    const std::string source = shape.whole ? said.print(iw::text(out) + "\n") : iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.image.size());
    iw::assembled(r, [&] { return m6502_source::assemble(m6502_source::Cpu::M6502, said.parse(source), folder); }, program.image,
                  folder / (stem + ".built." + shape.ext));
    return r;
}

}  // namespace cb::source::console_writer
