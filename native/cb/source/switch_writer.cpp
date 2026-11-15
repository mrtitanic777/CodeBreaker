// cb-unity: alone
// cb/source/switch_writer.cpp -- a Nintendo Switch program as AArch64 source that builds back into it (switch_writer.hpp).

#include "cb/source/switch_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <unordered_map>

#include "cb/build/a64_asm.hpp"
#include "cb/build/asm_text.hpp"
#include "cb/core/fs.hpp"
#include "cb/decompile/a64.hpp"
#include "cb/emulate/a64.hpp"

namespace cb::source::switch_writer {

namespace da = decompile::a64;
namespace iw = image_writer;
namespace nro = formats::nro;

namespace {

constexpr iw::Spelling kSub{"sub_", 6};
constexpr iw::Spelling kLocal{"L_", 6};
constexpr iw::Spelling kData{"D_", 6};
constexpr iw::Spelling kBss{"B_", 6};

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}
std::uint64_t le64(ByteView b, std::size_t at) { return static_cast<std::uint64_t>(le32(b, at)) | static_cast<std::uint64_t>(le32(b, at + 4)) << 32; }

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

// Flow runs through the text segment, an address being an offset in the image.
class SwitchFlow final : public iw::CodeModel<da::Insn> {
public:
    SwitchFlow(ByteView flat, const nro::Program& program) : flat_(flat), program_(program) {}

    std::optional<da::Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        return da::decode(flat_, pc, pc);
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::optional<std::uint32_t> target(const da::Insn& insn) const override {
        if (!insn.target || *insn.target > 0xFFFFFFFFull) return std::nullopt;
        return static_cast<std::uint32_t>(*insn.target);
    }
    bool inside(std::uint32_t pc) const override { return pc % 4 == 0 && pc + 4ull <= flat_.size() && program_.in_code(pc); }
    bool fits(std::uint32_t pc, const da::Insn&) const override { return pc + 4ull <= flat_.size(); }
    bool follows(const da::Insn& insn) const override {
        return insn.target && *insn.target <= 0xFFFFFFFFull && inside(static_cast<std::uint32_t>(*insn.target)) &&
               (insn.flow == da::Flow::Branch || insn.flow == da::Flow::Jump || insn.flow == da::Flow::Call);
    }
    bool stops(const da::Insn& insn) const override {
        return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect || insn.flow == da::Flow::Stop;
    }
    bool finds_functions() const override { return true; }
    bool starts_function(const da::Insn& insn) const override { return insn.flow == da::Flow::Call; }

private:
    ByteView flat_;
    const nro::Program& program_;
};

// Does the instruction change general register `reg`? (Over-approximate: an unknown instruction might.)
bool may_write(const da::Insn& i, unsigned reg) {
    const std::uint32_t w = i.word;
    const unsigned rd = w & 31, rn = (w >> 5) & 31;
    const bool simd = (w >> 26) & 1;
    switch (i.op) {
        case da::Op::Cbz: case da::Op::Tbz: case da::Op::B: case da::Op::BCond: case da::Op::Br: case da::Op::Ret: case da::Op::Brk: case da::Op::Hlt: case da::Op::Udf:
        case da::Op::Hint: case da::Op::Dsb: case da::Op::Dmb: case da::Op::Isb: case da::Op::Clrex: case da::Op::CondCmp: case da::Op::FpCmp: case da::Op::FpCCmp: case da::Op::Msr:
            return false;
        case da::Op::Bl: case da::Op::Blr: return reg == 30;
        case da::Op::Svc: return reg < 8;
        case da::Op::LdStImm: case da::Op::LdStReg: {
            const unsigned opc = (w >> 22) & 3;
            const bool load = simd ? false : opc != 0;
            const bool unsigned_form = (w >> 24) & 1, writeback = !unsigned_form && !((w >> 21) & 1) && ((w >> 10) & 1) && i.op == da::Op::LdStImm;
            return (load && rd == reg) || (writeback && rn == reg);
        }
        case da::Op::LdStPair: {
            const unsigned mode = (w >> 23) & 3;
            const bool load = (w >> 22) & 1 && !simd;
            return (load && (rd == reg || ((w >> 10) & 31) == reg)) || ((mode == 1 || mode == 3) && rn == reg);
        }
        case da::Op::LdLit: return !simd && rd == reg;
        case da::Op::LdStExcl: return rd == reg || ((w >> 16) & 31) == reg || ((w >> 10) & 31) == reg;
        case da::Op::AddSubImm: case da::Op::AddSubShift: case da::Op::AddSubExt: case da::Op::AddSubCarry: case da::Op::LogicImm: case da::Op::LogicShift: case da::Op::MoveWide:
        case da::Op::Bitfield: case da::Op::Extract: case da::Op::CondSel: case da::Op::Dp1: case da::Op::Dp2: case da::Op::Dp3: case da::Op::Adr: case da::Op::Adrp: case da::Op::Mrs:
        case da::Op::FpInt: case da::Op::FpFixed: case da::Op::SimdCopy:
            return rd == reg;
        default: return false;   // (vector and floating-point forms write vector registers)
    }
}

// The scale of an unsigned-offset load or store.
unsigned access_scale(std::uint32_t w) {
    const unsigned size = (w >> 30) & 3, opc = (w >> 22) & 3;
    return ((w >> 26) & 1) && opc >= 2 ? 4 : size;
}

struct Pair {
    std::uint32_t high_at, low_at;
    std::uint64_t full;
};

class SwitchListing final : public iw::Listing<da::Insn> {
public:
    struct Parts {
        translate::l2::Assembly* out;
        ByteView flat;
        const nro::Program* program;
        const std::map<std::uint32_t, da::Insn>* code;
        const std::map<std::uint32_t, std::string>* text;
        const iw::Names* labels;
        const std::map<std::uint32_t, std::string>* words;         // 8-byte pointers: the operand of `.quad`
        const std::map<std::uint32_t, std::string>* rela;          // a relocation entry: its three operands
        std::filesystem::path folder;
        std::string stem;
    };
    explicit SwitchListing(Parts p) : p_(std::move(p)) {}

    std::uint8_t byte(std::uint32_t offset) const override { return p_.flat[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return offset; }
    const da::Insn* code_at(std::uint32_t, std::uint32_t address) const override {
        const auto it = p_.code->find(address);
        return it == p_.code->end() ? nullptr : &it->second;
    }
    std::uint32_t size(const da::Insn&) const override { return 4; }
    std::string text(const da::Insn& insn) const override { return p_.text->at(static_cast<std::uint32_t>(insn.at)); }

    // Strings, runs of one byte as a fill, the rest as rows of bytes.
    std::vector<std::string> data_lines(ByteView data) const override {
        std::vector<std::string> lines;
        std::size_t i = 0;
        while (i < data.size()) {
            std::size_t run = 1;
            while (i + run < data.size() && data[i + run] == data[i]) ++run;
            if (run >= 16) {
                lines.push_back(std::format("        .fill {}, 1, 0x{:02X}", run, data[i]));
                i += run;
                continue;
            }
            std::size_t text_end = i;
            while (text_end < data.size() && printable(data[text_end])) ++text_end;
            if (text_end - i >= 6) {
                const bool zero = text_end < data.size() && data[text_end] == 0;
                lines.push_back(std::format("        {} \"{}\"", zero ? ".asciz" : ".ascii", std::string(reinterpret_cast<const char*>(data.data()) + i, text_end - i)));
                i = text_end + (zero ? 1 : 0);
                continue;
            }
            std::string row = "        .byte ";
            std::size_t n = 0;
            while (i < data.size() && n < 16) {
                std::size_t same = 1;
                while (i + same < data.size() && data[i + same] == data[i]) ++same;
                if (same >= 16 && n > 0) break;
                if (n != 0) row += ", ";
                row += std::format("0x{:02X}", data[i]);
                ++i;
                ++n;
            }
            lines.push_back(std::move(row));
        }
        return lines;
    }

    std::uint32_t special(std::uint32_t offset, std::uint32_t address, const iw::Flush& flush) override {
        if (p_.code->contains(address)) return 0;
        const nro::Program& prog = *p_.program;
        const auto label_here = [&] {
            if (const auto l = p_.labels->find(address); l != p_.labels->end()) p_.out->append(l->second + ":");
        };
        // the asset section
        if (prog.kind == nro::Kind::Nro && offset == prog.asset_offset && p_.flat.size() > prog.asset_offset) {
            flush();
            label_here();
            const std::string name = p_.stem + ".assets.bin";
            fs::write_file(p_.folder / name, ByteView(p_.flat).subspan(static_cast<std::size_t>(prog.asset_offset)));
            p_.out->append("        // the asset section: icon, control data and RomFS, kept as a file");
            p_.out->append(std::format("        .incbin \"{}\"", name));
            return static_cast<std::uint32_t>(p_.flat.size() - prog.asset_offset);
        }
        // the header of a NRO: the words after the branch to the entry point
        if (prog.kind == nro::Kind::Nro && offset == 4) {
            flush();
            label_here();
            static const char* const names[32] = {"entry branch",       "offset of MOD0",  "homebrew magic",   "(homebrew magic, second half)", "magic NRO0",       "version",
                                                  "file size",          "flags",           "text offset",      "text size",                     "rodata offset",    "rodata size",
                                                  "data offset",        "data size",       "bss size",         "reserved",                      "build id",         "build id",
                                                  "build id",           "build id",        "build id",         "build id",                      "build id",         "build id",
                                                  "dso handle offset",  "reserved",        "api info offset",  "api info size",                 "dynstr offset",    "dynstr size",
                                                  "dynsym offset",      "dynsym size"};
            for (unsigned at = 4; at < 0x80; at += 4) {
                if (at == 8) {
                    p_.out->append("        .ascii \"HOMEBREW\"");
                    at += 4;
                } else if (at == 0x10) {
                    p_.out->append("        .ascii \"NRO0\"");
                } else if (at >= 0x40 && at < 0x60) {
                    if (at == 0x40) {
                        std::string row = "        .byte ";
                        for (unsigned k = 0; k < 32; ++k) row += (k ? ", " : "") + std::format("0x{:02X}", p_.flat[0x40 + k]);
                        p_.out->append(row + "  // build id");
                    }
                } else {
                    p_.out->append(std::format("        .word 0x{:08X}  // {}", le32(p_.flat, at), names[at / 4]));
                }
            }
            return 0x7C;
        }
        // the MOD0 block
        if (prog.mod_offset != 0 && offset == prog.mod_offset && offset + 28 <= p_.flat.size()) {
            flush();
            label_here();
            p_.out->append("        .ascii \"MOD0\"");
            static const char* const fields[6] = {"offset of the dynamic section from MOD0", "bss start", "bss end", "eh_frame_hdr start", "eh_frame_hdr end", "offset of the runtime object"};
            for (unsigned k = 0; k < 6; ++k) p_.out->append(std::format("        .word 0x{:08X}  // {}", le32(p_.flat, offset + 4 + 4 * k), fields[k]));
            return 28;
        }
        if (const auto r = p_.rela->find(offset); r != p_.rela->end()) {
            flush();
            label_here();
            p_.out->append("        .quad " + r->second);
            return 24;
        }
        if (const auto w = p_.words->find(offset); w != p_.words->end()) {
            flush();
            label_here();
            p_.out->append("        .quad " + w->second);
            return 8;
        }
        return 0;
    }

private:
    Parts p_;
};

std::string safe_name(const std::string& name) {
    std::string clean;
    for (const char c : name) clean += (std::isalnum(static_cast<unsigned char>(c)) || c == '_') ? c : '_';
    return clean;
}

// Does a function begin here? A prologue that saves the frame or makes room on the stack.
bool prologue(const da::Insn& i) {
    const std::uint32_t w = i.word;
    if (i.op == da::Op::LdStPair) return ((w >> 5) & 31) == 31 && ((w >> 23) & 3) == 3 && !((w >> 22) & 1);   // stp ..., [sp, #-n]!
    if (i.op == da::Op::AddSubImm) return ((w >> 30) & 1) && !((w >> 29) & 1) && (w & 31) == 31 && ((w >> 5) & 31) == 31;   // sub sp, sp, #n
    return false;
}

}  // namespace

iw::Written write(const nro::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const ByteView flat = program.flat;
    const SwitchFlow flow(flat, program);
    const std::uint32_t size = static_cast<std::uint32_t>(flat.size());

    // where flow starts: the entry, the symbols, and the code the relocation tables say is pointed at
    std::set<std::uint32_t> entry_set = {static_cast<std::uint32_t>(program.entry)};
    for (const auto& [at, name] : program.names)
        if (at < size && flow.inside(static_cast<std::uint32_t>(at))) entry_set.insert(static_cast<std::uint32_t>(at));
    for (const nro::Relocation& r : program.relocations) {
        if (r.type != nro::kRelativeReloc || r.addend < 0 || static_cast<std::uint64_t>(r.addend) >= size) continue;
        const std::uint32_t t = static_cast<std::uint32_t>(r.addend);
        if (flow.inside(t)) entry_set.insert(t);
    }
    // a NRO and a NSO begin with `b _start`: that is where the program really starts
    iw::Names start_names;
    if (const std::optional<da::Insn> first = flow.decode(static_cast<std::uint32_t>(program.entry)); first && first->flow == da::Flow::Jump && first->target && *first->target < size) {
        entry_set.insert(static_cast<std::uint32_t>(*first->target));
        start_names[static_cast<std::uint32_t>(*first->target)] = "_start";
    }
    std::vector<std::uint32_t> entries(entry_set.begin(), entry_set.end());
    iw::Walked<da::Insn> walked = iw::walk<da::Insn>(flow, entries);
    // past those: the stretches of the text segment that nothing reaches but that begin like a function
    for (unsigned round = 0; round < 6; ++round) {
        std::vector<std::uint32_t> fresh;
        for (const nro::Segment& s : program.segments) {
            if (!s.code) continue;
            const std::uint32_t end = static_cast<std::uint32_t>(s.address + s.size);
            for (std::uint32_t a = static_cast<std::uint32_t>(s.address); a + 4 <= end; a += 4) {
                if (walked.code.contains(a)) continue;
                const std::optional<da::Insn> insn = flow.decode(a);
                if (!insn || !prologue(*insn)) continue;
                const iw::Walked<da::Insn> trial = iw::walk<da::Insn>(flow, std::vector<std::uint32_t>{a});
                if (trial.code.size() < 6) continue;
                fresh.push_back(a);
                for (const auto& [at, i] : trial.code) walked.code.insert_or_assign(at, i);   // (so the next candidates are past it)
                for (const std::uint32_t t : trial.targets) walked.targets.insert(t);
                for (const std::uint32_t st : trial.starts) fresh.push_back(st);
            }
        }
        if (fresh.empty()) break;
        std::sort(fresh.begin(), fresh.end());
        fresh.erase(std::unique(fresh.begin(), fresh.end()), fresh.end());
        walked.starts.insert(walked.starts.end(), fresh.begin(), fresh.end());
    }
    std::sort(walked.starts.begin(), walked.starts.end());
    walked.starts.erase(std::unique(walked.starts.begin(), walked.starts.end()), walked.starts.end());
    const std::map<std::uint32_t, da::Insn>& code = walked.code;

    iw::Names names;
    names.try_emplace(static_cast<std::uint32_t>(program.entry), "entry");
    for (const auto& [at, name] : start_names) names.try_emplace(at, name);
    for (const auto& [at, name] : program.names) {
        const std::string clean = safe_name(name);
        if (at < size && !clean.empty() && !std::isdigit(static_cast<unsigned char>(clean[0]))) names.try_emplace(static_cast<std::uint32_t>(at), clean);
    }
    {   // two symbols with one name get their address
        std::set<std::string> seen;
        for (auto& [at, name] : names)
            if (!seen.insert(name).second) name = std::format("{}_{:06X}", name, at);
    }
    const iw::Functions functions = iw::name_functions(walked.starts, names, kSub);
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);

    // the places a source line cannot begin: inside a special line (the header, MOD0, an 8-byte pointer, a relocation entry, the assets)
    std::set<std::uint32_t> blocked;
    if (program.kind == nro::Kind::Nro) {
        for (std::uint32_t a = 5; a < 0x80 && a < size; ++a) blocked.insert(a);
        if (program.mod_offset != 0)
            for (std::uint32_t a = static_cast<std::uint32_t>(program.mod_offset) + 1; a < program.mod_offset + 28 && a < size; ++a) blocked.insert(a);
        if (program.asset_offset < size)
            for (std::uint64_t a = program.asset_offset + 1; a < size; ++a) blocked.insert(static_cast<std::uint32_t>(a));
    }
    std::map<std::uint32_t, std::string> rela_entries;   // offset of an entry -> its operands (filled below)
    std::vector<std::uint32_t> rela_starts;
    for (const auto& [at, length] : program.relocation_tables) {
        for (std::uint64_t e = at; e + 24 <= at + length && e + 24 <= size; e += 24) {
            rela_starts.push_back(static_cast<std::uint32_t>(e));
            for (std::uint32_t k = 1; k < 24; ++k) blocked.insert(static_cast<std::uint32_t>(e) + k);
        }
    }
    const auto line_start = [&](std::uint32_t a) { return a < size && !inside_insn.contains(a) && !blocked.contains(a); };
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // the words the relocations patch
    std::map<std::uint32_t, std::string> words;   // word offset -> the operand of its `.quad`
    std::map<std::uint32_t, std::uint32_t> equates;
    const auto name_of = [&](std::uint64_t target) -> std::string {
        if (target >= size) {
            if (target < program.image_size) {   // the bss: a name defined by .equ
                const std::uint32_t t = static_cast<std::uint32_t>(target);
                equates[t] = t;
                return kBss(t);
            }
            return std::format("0x{:X}", target);
        }
        const std::uint32_t t = static_cast<std::uint32_t>(target);
        if (!line_start(t)) return std::format("0x{:X}", target);
        const auto it = labels.find(t);
        if (it != labels.end()) return it->second;
        labels.emplace(t, kData(t));
        return kData(t);
    };
    for (const nro::Relocation& r : program.relocations) {
        if (r.type != nro::kRelativeReloc || r.offset % 8 != 0 || r.offset + 8 > size) continue;
        const std::uint32_t w = static_cast<std::uint32_t>(r.offset);
        if (code.contains(w) || inside_insn.contains(w) || blocked.contains(w)) continue;
        const std::uint64_t v = le64(flat, w);
        if (v == static_cast<std::uint64_t>(r.addend) && r.addend >= 0) words[w] = name_of(static_cast<std::uint64_t>(r.addend));
        else words[w] = std::format("0x{:X}", v);
        for (std::uint32_t k = 1; k < 8; ++k) blocked.insert(w + k);
    }
    for (const std::uint32_t e : rela_starts) {
        const std::uint64_t off = le64(flat, e), info = le64(flat, e + 8), add = le64(flat, e + 16);
        const std::string a = off < size && words.contains(static_cast<std::uint32_t>(off)) ? name_of(off) : std::format("0x{:X}", off);
        const std::string c = info == nro::kRelativeReloc && add < program.image_size ? name_of(add) : std::format("0x{:X}", add);
        rela_entries[e] = std::format("{}, 0x{:X}, {}", a, info, c);
    }

    // constants the code builds with adrp and an add or a load: the address of something
    std::vector<Pair> pairs;
    {
        struct Pending {
            std::uint32_t at;
            std::uint64_t page;
        };
        std::array<std::optional<Pending>, 31> pending;
        std::uint32_t expected = 0;
        for (const auto& [at, insn] : code) {
            if (at != expected || labels.contains(at)) pending = {};
            expected = at + 4;
            const std::uint32_t w = insn.word;
            const unsigned rn = (w >> 5) & 31;
            if (insn.op == da::Op::Adrp && (w & 31) != 31 && insn.target) {
                pending[w & 31] = Pending{at, *insn.target};
            } else if (rn < 31 && pending[rn] && at - pending[rn]->at <= 64) {
                std::optional<std::uint64_t> full;
                if (insn.op == da::Op::AddSubImm && ((w >> 31) & 1) && !((w >> 30) & 1) && !((w >> 22) & 1)) full = pending[rn]->page + ((w >> 10) & 0xFFF);
                else if (insn.op == da::Op::LdStImm && ((w >> 24) & 1)) full = pending[rn]->page + (static_cast<std::uint64_t>((w >> 10) & 0xFFF) << access_scale(w));
                if (full && *full < program.image_size) pairs.push_back({pending[rn]->at, at, *full});
            }
            for (unsigned r = 0; r < 31; ++r)
                if (pending[r] && insn.op != da::Op::Adrp && may_write(insn, r)) pending[r].reset();
            if (insn.op == da::Op::Adrp && (w & 31) < 31) {   // (a second adrp into the register replaced the first)
            }
            if (insn.flow != da::Flow::None) pending = {};
        }
    }
    // names for what they address
    std::map<std::uint32_t, std::string> pair_high_name, pair_low_name;
    for (const Pair& p : pairs) {
        const std::string name = name_of(p.full);
        if (name.starts_with("0x")) continue;
        pair_low_name[p.low_at] = name;
        pair_high_name.try_emplace(p.high_at, name);
    }
    decompile::AddressLabels address_labels;
    decompile::NameLabels name_values;
    for (const auto& [a, n] : labels) {
        address_labels[a] = n;
        name_values[n] = a;
    }
    for (const auto& [a, v] : equates) name_values[kBss(a)] = v;

    // each instruction as text, or as a word when the text would not assemble back to it
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [at, insn] : code) {
        const auto same_word = [&](const std::string& t) {
            try {
                return da::assemble_word(t, at, name_values) == insn.word;
            } catch (const Error&) {
                return false;
            }
        };
        std::string t;
        bool ok = false;
        if (const auto h = pair_high_name.find(at); h != pair_high_name.end()) {
            const std::string plain = da::text(insn, address_labels);
            t = plain.substr(0, plain.find(',') + 2) + h->second;
            ok = same_word(t);
        } else if (const auto l = pair_low_name.find(at); l != pair_low_name.end()) {
            t = da::text(insn, address_labels, l->second);
            ok = same_word(t);
        }
        if (!ok) {
            t = da::text(insn, address_labels);
            ok = same_word(t);
        }
        std::string line = ok ? std::move(t) : std::format(".word 0x{:08X}  // {}", insn.word, t);
        if (insn.op == da::Op::Svc) {
            const std::string_view svc = emulate::a64::svc_name((insn.word >> 5) & 0xFFFF);
            if (!svc.empty()) line += std::format("  // svc{}", svc);
        }
        instruction_text[at] = std::move(line);
    }

    std::vector<std::string> head = {std::format("// {}.s -- written by CodeBreaker from {}", stem, std::filesystem::path(file_name).filename().string()),
                                     std::format("// Nintendo Switch {}, {} bytes, entry 0x{:06X}; AArch64, loaded at 0 (position independent)",
                                                 program.kind == nro::Kind::Nro ? "NRO" : program.kind == nro::Kind::Nso ? "NSO" : "ELF", size, program.entry),
                                     "// addresses in this file are offsets in the image; a word the loader relocates is its target's label",
                                     ""};
    if (!program.note.empty()) head.push_back("// " + program.note);
    for (const auto& [a, v] : equates) head.push_back(std::format("        .equ {}, 0x{:X}", kBss(a), v));
    if (!equates.empty()) head.push_back("");
    translate::l2::Assembly out(head, "gnu-as", std::string(file_name));
    out.part("image", "segment", std::nullopt, size);

    SwitchListing listing({&out, flat, &program, &code, &instruction_text, &labels, &words, &rela_entries, folder, stem});
    iw::emit<da::Insn>(out, 0, size, labels, functions, listing);
    out.append("");

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), size);
    const char* extension = program.kind == nro::Kind::Nro ? ".built.nro" : ".built.bin";
    iw::assembled(
        r,
        [&] {
            build::a64_asm::Image image = build::a64_asm::build(source, folder);
            if (image.base != 0) throw build::AsmError("the image starts at the wrong address");
            return std::move(image.data);
        },
        flat, folder / (stem + extension));
    if (program.kind != nro::Kind::Nro && r.same) r.notes.push_back("the image is the loaded program: the " + std::string(program.kind == nro::Kind::Nso ? "NSO" : "ELF") + " container is not written back");
    return r;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(nro::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::switch_writer
