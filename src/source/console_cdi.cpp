// cb-unity: alone
// source/console_cdi.cpp -- the executable shape's container `cdi` (console_executable.cpp): a CD-i disc's CD-RTOS modules (or a module file) as 68000 source that builds back into it
// (config/platforms/cdi.json).
//
//     r.source           game.asm (Motorola syntax)
//     r.same             the source, built, puts back every placed module's bytes, so the disc (or file) is the original
//     r.functions        {address: name}; an address is where the model places the module (formats/cdi) plus the offset in it
//
// Every executable module is an `org` at its address; its $30-byte header is `dc` lines with the fields named, its code what flow reaches from the
// module's entry (the base plus the execution offset: `main_<name>`), the rest data. The system is called through `TRAP #0` and a word naming the
// call: the pair is written as `TRAP #0` and `dc.w F_Exit` (the OS-9 function codes by name: F_Link, F_Fork, F_Exit, F_Sleep, I_Open, I_Read, I_Write ...),
// and flow goes on after the word. Functions are `main_<module>` and `sub_<address>`, jump targets `L_<address>`; the CD-i's chips (the MCD212 video
// chip at $200000, the slave processor, the real-time clock) have names. A module that is not executable (a data module, a descriptor) stays bytes of
// the disc. The rebuilt disc is kept beside the source as <stem>.built.<ext>.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/cdi.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "decompile/m68k.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer::cdi_code {

namespace m68k = decompile::m68k;
namespace iw = image_writer;

const std::map<std::uint32_t, std::string>& system_calls();
const std::map<std::uint32_t, std::string>& hardware();
namespace cd = formats::cdi;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};

// `TRAP #0`: the call is the word after it.
bool os9_call(const m68k::Insn& insn) {
    if (insn.mnemonic != "TRAP") return false;
    const auto ops = insn.operands();
    return !ops.empty() && ops[0].value == 0;
}

std::uint32_t be16(ByteView d, std::size_t at) { return static_cast<std::uint32_t>(d[at] << 8 | d[at + 1]); }
std::uint32_t be32(ByteView d, std::size_t at) { return be16(d, at) << 16 | be16(d, at + 2); }

class CdiFlow final : public iw::CodeModel<m68k::Insn> {
public:
    explicit CdiFlow(const cd::Program& program) : program_(program), memory_(program.memory()) {}

    std::optional<m68k::Insn> decode(std::uint32_t pc) const override { return m68k::decode(memory_, pc, pc); }
    std::uint32_t size(const m68k::Insn& insn) const override { return os9_call(insn) ? 4 : insn.length; }
    std::optional<std::uint32_t> target(const m68k::Insn& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc % 2 == 0 && program_.module_at(pc) != nullptr; }
    bool fits(std::uint32_t pc, const m68k::Insn& insn) const override {
        const cd::Module* m = program_.module_at(pc);
        return m && pc + size(insn) <= m->address + m->size;
    }
    bool follows(const m68k::Insn& insn) const override {
        return insn.target && *insn.target % 2 == 0 && program_.module_at(*insn.target) != nullptr && insn.flow != m68k::Flow::None && insn.mnemonic != "TRAP";
    }
    bool stops(const m68k::Insn& insn) const override {
        return insn.flow == m68k::Flow::Jump || insn.flow == m68k::Flow::Return || insn.flow == m68k::Flow::Indirect || insn.flow == m68k::Flow::Halt;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const m68k::Insn& insn) const override { return insn.mnemonic == "JSR" || insn.mnemonic == "BSR"; }

private:
    const cd::Program& program_;
    Bytes memory_;
};

class CdiListing final : public iw::Listing<m68k::Insn> {
public:
    CdiListing(translate::l2::Assembly& out, const cd::Program& program, const cd::Module& module, const std::map<std::uint32_t, m68k::Insn>& code,
               const decompile::AddressLabels& names, std::set<std::string>& used_calls)
        : out_(out), program_(program), module_(module), code_(code), names_(names), used_calls_(used_calls) {}

    std::uint8_t byte(std::uint32_t offset) const override { return program_.file[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return module_.address + (offset - module_.file_offset); }
    const m68k::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = code_.find(address);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const m68k::Insn& insn) const override { return os9_call(insn) ? 4 : insn.length; }
    std::string text(const m68k::Insn& insn) const override {
        std::string line = dialect::find("vasm-m68k").fallback(risc_source::m68k_code(insn, names_));
        if (!os9_call(insn)) return line;
        const std::uint32_t code = be16(program_.file, program_.offset(insn.pc).value() + 2);
        const auto named = system_calls().find(code);
        if (named == system_calls().end()) return line + std::format("\n        .half 0x{:04x}", code);
        used_calls_.insert(named->second);
        return line + "\n        .half " + named->second;
    }
    std::vector<std::string> data_lines(ByteView data) const override { return risc_source::data_lines(data); }

    // The module's header, field by field.
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const iw::Flush& flush) override {
        if (offset != module_.file_offset) return 0;
        flush();
        const ByteView f = program_.file;
        const std::size_t h = offset;
        out_.append(std::format("        .half 0x{:04x}  ; module sync, {}", be16(f, h), module_.name));
        out_.append(std::format("        .half 0x{:04x}  ; system revision", be16(f, h + 2)));
        out_.append(std::format("        .word 0x{:08x}  ; module size", be32(f, h + 4)));
        out_.append(std::format("        .word 0x{:08x}  ; owner", be32(f, h + 8)));
        out_.append(std::format("        .word 0x{:08x}  ; name offset", be32(f, h + 12)));
        out_.append(std::format("        .half 0x{:04x}  ; access", be16(f, h + 16)));
        out_.append(std::format("        .byte 0x{:02x},0x{:02x}  ; type, language", f[h + 18], f[h + 19]));
        out_.append(std::format("        .byte 0x{:02x},0x{:02x}  ; attributes, revision", f[h + 20], f[h + 21]));
        out_.append(std::format("        .half 0x{:04x}  ; edition", be16(f, h + 22)));
        out_.append(std::format("        .word 0x{:08x}  ; usage", be32(f, h + 24)));
        out_.append(std::format("        .word 0x{:08x}  ; symbol table", be32(f, h + 28)));
        out_.extend(iw::commented(risc_source::data_lines(f.subspan(h + 32, 14)), "reserved"));
        out_.append(std::format("        .half 0x{:04x}  ; header parity", be16(f, h + 46)));
        return static_cast<std::uint32_t>(cd::kHeader);
    }

private:
    translate::l2::Assembly& out_;
    const cd::Program& program_;
    const cd::Module& module_;
    const std::map<std::uint32_t, m68k::Insn>& code_;
    const decompile::AddressLabels& names_;
    std::set<std::string>& used_calls_;
};

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

// What a module's name makes of a label: letters and digits, lower case.
std::string label_of(const std::string& name, std::size_t index) {
    std::string s;
    for (const char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c))) s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    if (s.empty() || std::isdigit(static_cast<unsigned char>(s[0]))) s = "module" + std::to_string(index) + s;
    return "main_" + s;
}

// The file with its modules' bytes replaced by those the source builds: the modules in the order of their addresses, each `size` bytes (the source's `.org` says
// the address a module is seen at; it does not move the bytes in the build).
Bytes rebuilt_file(const cd::Program& program, const Bytes& image) {
    Bytes out = program.file;
    std::vector<cd::Module> order = program.placed();
    std::sort(order.begin(), order.end(), [](const cd::Module& a, const cd::Module& b) { return a.address < b.address; });
    std::size_t at = 0;
    for (const cd::Module& m : order) {
        for (std::uint32_t i = 0; i < m.size; ++i) out[m.file_offset + i] = at + i < image.size() ? image[at + i] : 0;
        at += m.size;
    }
    return out;
}

}  // namespace

// CD-RTOS's system calls by number and the machine's registers by address (config/knowledge/cdi/registers.json).
const std::map<std::uint32_t, std::string>& system_calls() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().table_names("cdi", "system_calls");
    return table;
}

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("cdi", "hardware");
    return table;
}

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    const cd::Program& program = contained_as<cd::Program>(loaded);
    fs::make_dirs(folder);
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));
    const CdiFlow flow(program);
    const std::vector<std::uint32_t> entries = program.entries();
    const iw::Walked<m68k::Insn> walked = iw::walk<m68k::Insn>(flow, entries);
    const std::map<std::uint32_t, m68k::Insn>& code = walked.code;

    iw::Names names;
    std::size_t index = 0;
    for (const cd::Module& m : program.modules) {
        if (m.runs()) names.try_emplace(m.entry(), label_of(m.name, index));
        ++index;
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    std::set<std::uint32_t> inside_insn;
    for (const auto& [at, insn] : code) {
        for (std::uint32_t a = at + 1; a < at + flow.size(insn); ++a) inside_insn.insert(a);
    }
    const auto line_start = [&](std::uint32_t a) { return a % 2 == 0 && program.module_at(a) != nullptr && !inside_insn.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);
    std::set<std::uint32_t> used;
    for (const auto& entry : code) {
        for (const Reference& ref : references(entry.second)) {
            if (hardware().contains(ref.value)) {
                used.insert(ref.value);
            } else if (line_start(ref.value) && (!ref.immediate || code.contains(ref.value))) {
                const auto named = names.find(ref.value);
                labels.try_emplace(ref.value, named != names.end() ? named->second : (code.contains(ref.value) ? kLocal : kData)(ref.value));
            }
        }
    }
    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const std::uint32_t a : used) names_for_text[a] = hardware().at(a);

    std::set<std::string> used_calls;
    translate::l2::Assembly out(head_comment(description, values),
                                "asm68k", std::string(file_name));
    std::vector<cd::Module> order = program.placed();
    std::sort(order.begin(), order.end(), [](const cd::Module& a, const cd::Module& b) { return a.address < b.address; });
    // the segments are written into a second assembly first, so the equates for the calls and chips used can come before them
    translate::l2::Assembly body({}, "asm68k", std::string(file_name));
    for (const cd::Module& m : order) {
        body.part(m.name.empty() ? "module" : m.name, "segment", std::nullopt, m.size);
        body.append(std::format("        .org 0x{:06x}", m.address));
        CdiListing listing(body, program, m, code, names_for_text, used_calls);
        iw::emit<m68k::Insn>(body, m.file_offset, m.file_offset + m.size, labels, functions, listing);
        body.append("");
    }
    for (const std::uint32_t a : used) out.append(risc_source::equ(hardware().at(a), a, 6));
    for (const std::string& name : used_calls) {
        for (const auto& [code_number, n] : system_calls()) {
            if (n == name) out.append(risc_source::equ(name, code_number, 2));
        }
    }
    out.append("");
    const std::string source = dialect::find("vasm-m68k").print(iw::text(out) + "\n" + iw::text(body) + "\n");   // (said in vasm's Motorola syntax)
    iw::Written r = iw::finish(folder, stem + ".asm", source, functions, iw::code_bytes(code, flow), program.file.size());
    iw::assembled(r, [&] { return rebuilt_file(program, risc_source::assemble("m68k", dialect::find("vasm-m68k").parse(source), folder)); }, program.file, folder / fill(description.executable.built, values));
    return r;
}


}  // namespace cb::source::console_writer::cdi_code
