// cb-unity: alone (file-local names that other files in a batch also use)
// cb/source/mips_elf_writer.cpp -- a little-endian MIPS ELF program as assembly source that builds back into it (mips_elf_writer.hpp).

#include "cb/source/mips_elf_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/build/mips_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::source::mips_elf_writer {

namespace dm = decompile::mips;
namespace iw = image_writer;
namespace me = formats::mips_elf;

namespace {

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr unsigned kLookAhead = 8;           // instructions after a `lui` that may use what it loaded
constexpr unsigned kStringMin = 4;           // a run of printable bytes this long is a string
constexpr unsigned kStringMax = 96;
constexpr std::size_t kFillMin = 16;         // a run of one value this long is `.space` / `.fill`
constexpr unsigned kRounds = 4;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 400;     // how many instructions a function is looked at for before it is called data
constexpr std::uint32_t kPointerMin = 0x100; // a word below this is a number, not an address

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }
std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

// ---- the file's pieces ----------------------------------------------------------------------------------------------

enum class Role : std::uint8_t { Other, Header, ProgramHeaders, SectionHeaders, Code, Data };

// A run of the file: where it is, what it is, and where it is loaded.
struct Piece {
    std::size_t offset = 0, size = 0;       // in the file
    Role role = Role::Other;
    bool mapped = false;
    std::uint32_t address = 0;
    std::string name;
};

// The file cut into pieces: the headers, the loaded code and data (by the sections, or the segments when there are none), and the rest.
std::vector<Piece> plan_pieces(ByteView file, const me::Elf& elf, std::size_t base) {
    const std::size_t n = file.size();
    std::vector<std::uint8_t> role(n, static_cast<std::uint8_t>(Role::Other));
    std::vector<std::int32_t> owner(n, -1);        // the section that holds the byte
    std::vector<std::int32_t> segment(n, -1);      // the segment that loads the byte
    const auto in_file = [&](std::uint64_t at, std::uint64_t size) { return base + at + size <= n; };
    for (std::size_t i = 0; i < elf.program_headers.size(); ++i) {
        const me::ProgramHeader& p = elf.program_headers[i];
        if (p.type != 1 || p.filesz == 0 || !in_file(p.offset, p.filesz)) continue;
        for (std::size_t k = 0; k < p.filesz; ++k) {
            const std::size_t at = base + p.offset + k;
            if (segment[at] >= 0) continue;
            segment[at] = static_cast<std::int32_t>(i);
            role[at] = static_cast<std::uint8_t>((p.flags & 1) != 0 ? Role::Code : Role::Data);
        }
    }
    for (std::size_t i = 0; i < elf.sections.size(); ++i) {
        const me::SectionHeader& s = elf.sections[i];
        if (s.type == me::kTypeNobits || s.size == 0 || s.type == 0 || !in_file(s.offset, s.size)) continue;
        for (std::size_t k = 0; k < s.size; ++k) {
            const std::size_t at = base + s.offset + k;
            owner[at] = static_cast<std::int32_t>(i);
            if ((s.flags & me::kFlagAlloc) != 0) role[at] = static_cast<std::uint8_t>((s.flags & me::kFlagExec) != 0 ? Role::Code : Role::Data);
            else if (segment[at] < 0) role[at] = static_cast<std::uint8_t>(Role::Other);
        }
    }
    const auto mark = [&](std::uint64_t at, std::uint64_t size, Role r) {
        if (!in_file(at, size)) return;
        for (std::uint64_t k = 0; k < size; ++k) {
            role[base + at + k] = static_cast<std::uint8_t>(r);
            owner[base + at + k] = -2;
        }
    };
    mark(0, std::min<std::uint64_t>(me::kHeaderSize, n - base), Role::Header);
    mark(elf.phoff, static_cast<std::uint64_t>(elf.phnum) * me::kProgramHeaderSize, Role::ProgramHeaders);
    mark(elf.shoff, static_cast<std::uint64_t>(elf.shnum) * me::kSectionHeaderSize, Role::SectionHeaders);

    std::vector<Piece> pieces;
    std::size_t at = 0;
    while (at < n) {
        std::size_t end = at + 1;
        while (end < n && role[end] == role[at] && owner[end] == owner[at] && segment[end] == segment[at]) ++end;
        Piece p;
        p.offset = at;
        p.size = end - at;
        p.role = static_cast<Role>(role[at]);
        if (p.role == Role::Code || p.role == Role::Data) {
            if (segment[at] >= 0) {
                const me::ProgramHeader& seg = elf.program_headers[static_cast<std::size_t>(segment[at])];
                p.mapped = true;
                p.address = seg.vaddr + static_cast<std::uint32_t>(at - base - seg.offset);
            } else if (owner[at] >= 0) {
                const me::SectionHeader& s = elf.sections[static_cast<std::size_t>(owner[at])];
                p.mapped = true;
                p.address = s.address + static_cast<std::uint32_t>(at - base - s.offset);
            }
            if (!p.mapped) p.role = Role::Other;
            if (p.mapped && p.role == Role::Code && (p.address % 4 != 0 || p.size < 4)) p.role = Role::Data;   // (code stands at a multiple of four)
            if (owner[at] >= 0) p.name = elf.sections[static_cast<std::size_t>(owner[at])].name;
        } else if (p.role == Role::Header) {
            p.name = "elf header";
        } else if (p.role == Role::ProgramHeaders) {
            p.name = "program headers";
        } else if (p.role == Role::SectionHeaders) {
            p.name = "section headers";
        } else if (owner[at] >= 0) {
            p.name = elf.sections[static_cast<std::size_t>(owner[at])].name;
        }
        if (p.name.empty()) p.name = p.role == Role::Other ? "data" : p.role == Role::Code ? "code" : "data";
        // a piece is cut at 16 MB so nothing larger than the assembler's file limit is one part
        pieces.push_back(std::move(p));
        at = end;
    }
    return pieces;
}

// ---- the program's code space ---------------------------------------------------------------------------------------

class Space {
public:
    Space(ByteView file, const std::vector<Piece>& pieces, dm::Variant variant) : file_(file), variant_(variant) {
        for (const Piece& p : pieces)
            if (p.mapped && (p.role == Role::Code || p.role == Role::Data)) pieces_.push_back(&p);
        std::ranges::sort(pieces_, {}, &Piece::address);
    }

    const Piece* find(std::uint32_t a) const {
        auto it = std::ranges::upper_bound(pieces_, a, {}, &Piece::address);
        if (it == pieces_.begin()) return nullptr;
        const Piece* p = *--it;
        return a - p->address < p->size ? p : nullptr;
    }
    bool is_code(std::uint32_t a) const {
        const Piece* p = find(a);
        return p && p->role == Role::Code;
    }
    bool inside(std::uint32_t pc) const {
        const Piece* p = find(pc);
        return p && p->role == Role::Code && pc % 4 == 0 && pc - p->address + 4 <= p->size;
    }
    std::optional<std::uint32_t> word(std::uint32_t a) const {
        const Piece* p = find(a);
        if (!p || a - p->address + 4 > p->size) return std::nullopt;
        return le32(file_, p->offset + (a - p->address));
    }
    std::optional<dm::Insn> decode(std::uint32_t pc) const {
        if (!inside(pc)) return std::nullopt;
        return dm::decode_word(*word(pc), pc, variant_);
    }

private:
    ByteView file_;
    dm::Variant variant_;
    std::vector<const Piece*> pieces_;
};

bool ends_flow(const dm::Insn& i) { return i.delay_slot && (i.flow == dm::Flow::Jump || i.flow == dm::Flow::Return || i.flow == dm::Flow::Indirect); }

// `addiu $sp, $sp, -N` or `daddiu $sp, $sp, -N`: how a compiler opens a function that calls others or keeps a frame.
bool is_prologue(std::uint32_t word) { return ((word >> 16) == 0x27BD || (word >> 16) == 0x67BD) && (word & 0x8000) != 0 && (word & 3) == 0; }

struct Walked {
    std::map<std::uint32_t, dm::Insn> code;
    std::set<std::uint32_t> targets;
    std::set<std::uint32_t> calls;
};

Walked walk(const Space& space, const std::set<std::uint32_t>& roots) {
    Walked out;
    std::vector<std::uint32_t> todo(roots.begin(), roots.end());
    while (!todo.empty()) {
        std::uint32_t pc = todo.back();
        todo.pop_back();
        while (space.inside(pc) && !out.code.contains(pc)) {
            std::optional<dm::Insn> insn = space.decode(pc);
            if (!insn) break;
            if (insn->target && space.inside(*insn->target) && insn->flow != dm::Flow::None) {
                out.targets.insert(*insn->target);
                if (insn->flow == dm::Flow::Call) out.calls.insert(*insn->target);
                todo.push_back(*insn->target);
            }
            const bool stop = ends_flow(*insn);
            out.code.emplace(pc, std::move(*insn));
            pc += 4;
            if (stop) {   // the delay slot is part of the flow
                if (space.inside(pc) && !out.code.contains(pc)) {
                    if (std::optional<dm::Insn> slot = space.decode(pc)) {
                        if (slot->target && space.inside(*slot->target) && slot->flow != dm::Flow::None) {
                            out.targets.insert(*slot->target);
                            todo.push_back(*slot->target);
                        }
                        out.code.emplace(pc, std::move(*slot));
                    }
                }
                break;
            }
        }
    }
    return out;
}

bool reads_as_function(const Space& space, std::uint32_t at) {
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 4 * count;
        const std::optional<dm::Insn> insn = space.decode(pc);
        if (!insn) return false;
        if (insn->target && insn->flow != dm::Flow::Call && !space.inside(*insn->target)) return false;
        if (!ends_flow(*insn)) continue;
        if (!space.decode(pc + 4)) return false;
        return count >= 1;
    }
    return false;
}

// Which register an instruction writes (32: none).
unsigned written(const dm::Insn& i) {
    switch (i.form) {
        case dm::Form::RdRsRt: case dm::Form::RdRtSa: case dm::Form::RdRtRs: case dm::Form::Rd: case dm::Form::RdRs: case dm::Form::RdRt: return i.rd;
        case dm::Form::RtRsImm: case dm::Form::RtImm: case dm::Form::RtCop:
            return i.mnemonic == "mtc0" || i.mnemonic == "mtc2" || i.mnemonic == "ctc2" || i.mnemonic == "qmtc2" || i.mnemonic == "mtic" ? 32 : i.rt;
        case dm::Form::RtMem: return i.mnemonic[0] == 'l' && i.mnemonic != "lwc2" ? i.rt : 32;
        case dm::Form::Jump: return i.flow == dm::Flow::Call ? 31 : 32;
        case dm::Form::RsBranch: return i.flow == dm::Flow::Call ? 31 : 32;
        default: return 32;
    }
}

struct Use {
    std::uint32_t lui, at, address;
    bool ori;
};

std::vector<Use> address_pairs(const std::map<std::uint32_t, dm::Insn>& code, const std::set<std::uint32_t>& starts) {
    std::vector<Use> out;
    for (const auto& [pc, p] : code) {
        if (p.mnemonic != "lui") continue;
        const unsigned reg = p.rt;
        if (reg == 0) continue;
        const std::uint32_t high = static_cast<std::uint32_t>(p.imm) << 16;
        bool last = false;
        for (unsigned k = 1; k <= kLookAhead; ++k) {
            const std::uint32_t at = pc + 4 * k;
            const auto it = code.find(at);
            if (it == code.end() || starts.contains(at)) break;
            const dm::Insn& q = it->second;
            if (q.rs == reg) {
                if (q.mnemonic == "addiu" || q.mnemonic == "daddiu") out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
                else if (q.mnemonic == "ori") out.push_back({pc, at, high | static_cast<std::uint32_t>(q.imm), true});
                else if (q.form == dm::Form::RtMem && q.mnemonic != "lqc2" && q.mnemonic != "sqc2") out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
            }
            if (written(q) == reg || last) break;
            if (q.delay_slot) last = true;
        }
    }
    return out;
}

enum class Kind { None, Text, Hardware, Variable };

// A symbol table's names for the addresses it defines: functions over objects over the rest, global over local.
std::map<std::uint32_t, std::string> symbol_names(const me::Elf& elf, const Space& space) {
    std::map<std::uint32_t, std::string> out;
    std::map<std::uint32_t, int> rank;
    for (const me::Symbol& s : elf.symbols()) {
        if (s.section == 0 || s.name.empty() || s.type > 2) continue;
        if (s.section >= 0xFF00) continue;   // (absolute and common symbols)
        if (!space.find(s.value)) continue;
        std::string name;
        for (const char c : s.name) name += (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.') ? c : '_';
        if (name.starts_with(".L") || name.starts_with("$") || std::isdigit(static_cast<unsigned char>(name[0])) || name == ".") continue;
        if (name[0] == '.') name = "_" + name.substr(1);
        const int score = (s.type == 2 ? 4 : s.type == 1 ? 2 : 0) + (s.bind == 1 ? 1 : 0);
        const auto it = rank.find(s.value);
        if (it != rank.end() && it->second >= score) continue;
        rank[s.value] = score;
        out[s.value] = std::move(name);
    }
    // one name for one address: a name used for two addresses keeps the first and the other gets its address
    std::map<std::uint32_t, std::string> unique;
    std::unordered_set<std::string> used;
    for (const auto& [a, n] : out) {
        if (used.insert(n).second) unique[a] = n;
        else unique[a] = std::format("{}_{:08X}", n, a);
    }
    return unique;
}

std::size_t printable_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 0;
    while (off + run < limit && run < kStringMax && printable(buf[off + run])) ++run;
    return run;
}
std::size_t same_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 1;
    while (off + run < limit && buf[off + run] == buf[off]) ++run;
    return run;
}
std::size_t string_inside_word(ByteView buf, std::size_t at, std::size_t limit) {
    for (std::size_t k = 1; k < 4 && at + k < limit; ++k)
        if (printable_run(buf, at + k, limit) >= kStringMin) return k;
    return 0;
}
std::string byte_row(ByteView bytes) {
    std::vector<std::string> items;
    for (const std::uint8_t b : bytes) items.push_back(std::format("0x{:02X}", b));
    return "        .byte " + text::join(items, ", ");
}

struct DataItem {
    std::string line;
    std::size_t took = 0;
};
DataItem data_item(ByteView buf, std::size_t off, std::size_t limit, std::uint32_t addr, const std::function<std::optional<std::string>(std::uint32_t)>& pointer_at) {
    if (const std::size_t run = printable_run(buf, off, limit); run >= kStringMin) {
        const bool terminated = off + run < limit && buf[off + run] == 0;
        return {std::format("        {} \"{}\"", terminated ? ".asciz" : ".ascii", as_text(buf.subspan(off, run))), run + (terminated ? 1u : 0u)};
    }
    if (const std::size_t same = same_run(buf, off, limit); same >= kFillMin)
        return {buf[off] == 0 ? std::format("        .space {}", same) : std::format("        .fill {}, 1, 0x{:02X}", same, buf[off]), same};
    if (addr % 4 == 0 && off + 4 <= limit) {
        if (const std::size_t lead = string_inside_word(buf, off, limit); lead != 0) return {byte_row(buf.subspan(off, lead)), lead};
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 4 && off + took + 4 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0)) break;
            const std::optional<std::string> name = pointer_at(addr + static_cast<std::uint32_t>(took));
            items.push_back(name ? *name : hex8(le32(buf, at)));
            took += 4;
        }
        return {"        .word " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 4 != 0 ? 4 - addr % 4 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

}  // namespace

iw::Written write(const Machine& machine, ByteView file, const me::Elf& elf, std::size_t base, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const std::vector<Piece> pieces = plan_pieces(file, elf, base);
    const Space space(file, pieces, machine.variant);
    const dm::Variant variant = machine.variant;

    // function starts: the entry point, what the container and the symbol table name, what code calls, what looks like a function after another's end,
    // what a pointer names
    iw::Names names = symbol_names(elf, space);
    for (const auto& [a, n] : machine.names)
        if (space.find(a)) names[a] = n;
    std::set<std::uint32_t> entries;
    std::set<std::uint32_t> blocks;   // places flow goes on from that nothing calls: a jump table's cases
    if (space.inside(elf.entry)) entries.insert(elf.entry);
    for (const me::Symbol& s : elf.symbols())
        if (s.type == 2 && s.section != 0 && space.inside(s.value)) entries.insert(s.value);
    for (const auto& [a, n] : machine.names)
        if (space.inside(a)) entries.insert(a);

    std::vector<std::uint32_t> pointer_values;   // words of data (and of code that no instruction reads) that hold an address of the program
    for (const Piece& p : pieces) {
        if (!p.mapped || (p.role != Role::Code && p.role != Role::Data)) continue;
        for (std::size_t off = 0; off + 4 <= p.size; off += 4) {
            if ((p.address + off) % 4 != 0) continue;
            const std::uint32_t v = le32(file, p.offset + off);
            if (v < kPointerMin || v % 4 != 0 || !space.find(v)) continue;
            if (p.role == Role::Code && dm::decode_word(v, 0, variant)) continue;
            pointer_values.push_back(v);
        }
    }
    Walked walked;
    std::vector<Use> uses;
    std::set<std::uint32_t> function_starts;
    for (unsigned round = 0; round < kRounds; ++round) {
        std::set<std::uint32_t> roots = entries;
        roots.insert(blocks.begin(), blocks.end());
        walked = walk(space, roots);
        function_starts = entries;
        for (const std::uint32_t c : walked.calls) function_starts.insert(c);
        uses = address_pairs(walked.code, function_starts);
        std::set<std::uint32_t> pointed(pointer_values.begin(), pointer_values.end());
        for (const Use& u : uses) pointed.insert(u.address);
        bool added = false;
        for (const std::uint32_t a : pointed) {
            if (!space.inside(a) || walked.code.contains(a)) continue;
            const std::uint32_t w = *space.word(a);
            if (is_prologue(w) || reads_as_function(space, a)) {
                added |= entries.insert(a).second;
            } else if (a >= 8 && walked.code.contains(a - 8) && ends_flow(walked.code.at(a - 8)) && space.decode(a)) {
                added |= blocks.insert(a).second;   // a case of a jump table: the code after a jump
            }
        }
        // code that begins as a function does after another one's end (a jump and its delay slot)
        for (const Piece& p : pieces) {
            if (p.role != Role::Code || !p.mapped) continue;
            for (std::size_t off = 8; off + 4 <= p.size; off += 4) {
                const std::uint32_t at = p.address + static_cast<std::uint32_t>(off);
                if (walked.code.contains(at) || entries.contains(at)) continue;
                if (!is_prologue(le32(file, p.offset + off))) continue;
                const std::optional<dm::Insn> before = space.decode(at - 8);
                if (before && ends_flow(*before)) added |= entries.insert(at).second;
            }
        }
        if (!added) break;
    }
    {
        std::set<std::uint32_t> roots = entries;
        roots.insert(blocks.begin(), blocks.end());
        walked = walk(space, roots);
        function_starts = entries;
        for (const std::uint32_t c : walked.calls) function_starts.insert(c);
        uses = address_pairs(walked.code, function_starts);
    }
    std::erase_if(function_starts, [&](std::uint32_t a) { return !walked.code.contains(a); });
    const std::map<std::uint32_t, dm::Insn>& code = walked.code;
    const auto line_start = [&](std::uint32_t a) { return space.find(a) != nullptr && (a % 4 == 0 || !code.contains(a & ~3u)); };

    if (!names.contains(elf.entry) && function_starts.contains(elf.entry)) names[elf.entry] = "entry";
    const std::vector<std::uint32_t> starts(function_starts.begin(), function_starts.end());
    const iw::Functions functions = iw::name_functions(starts, names, kSub);
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;
    std::unordered_map<std::uint32_t, std::string> immediate;
    const auto name_of = [&](std::uint32_t a) -> std::optional<std::string> {
        if (space.find(a)) {
            if (!line_start(a)) return std::nullopt;
            const auto it = labels.find(a);
            if (it != labels.end()) return it->second;
            const std::string made = (code.contains(a) ? kLocal : kData)(a);
            labels[a] = made;
            return made;
        }
        if (machine.hardware) {
            if (const std::optional<std::string> hw = machine.hardware(a)) {
                equates[*hw] = a;
                return *hw;
            }
        }
        if (machine.in_ram && machine.in_ram(a)) {
            std::string n = std::format("var_{:08X}", a);
            equates[n] = a;
            return n;
        }
        return std::nullopt;
    };
    struct HighHalf {
        std::uint32_t address;
        bool ori;
        std::string expression;
    };
    std::map<std::uint32_t, HighHalf> high_half;
    for (const Use& u : uses) {
        if (u.address < kPointerMin && !space.find(u.address) && !(machine.hardware && machine.hardware(u.address))) continue;   // (a small number is not an address)
        const std::optional<std::string> name = name_of(u.address);
        if (!name) continue;
        immediate[u.at] = "%lo(" + *name + ")";
        HighHalf candidate{u.address, u.ori, u.ori ? "(" + *name + " >> 16)" : "%hi(" + *name + ")"};
        const auto it = high_half.find(u.lui);
        if (it == high_half.end() || std::tie(candidate.ori, candidate.address) < std::tie(it->second.ori, it->second.address)) high_half[u.lui] = std::move(candidate);
    }
    for (const auto& [pc, high] : high_half) immediate[pc] = high.expression;

    std::unordered_set<std::uint32_t> pointer_words;
    for (const Piece& p : pieces) {
        if (!p.mapped || (p.role != Role::Code && p.role != Role::Data)) continue;
        for (std::size_t off = 0; off + 4 <= p.size; off += 4) {
            const std::uint32_t at = p.address + static_cast<std::uint32_t>(off);
            if (at % 4 != 0 || code.contains(at)) continue;
            const std::uint32_t v = le32(file, p.offset + off);
            if (v < kPointerMin || v % 4 != 0) continue;
            if (p.role == Role::Code && dm::decode_word(v, 0, variant) && !space.find(v)) continue;
            if (space.find(v) && name_of(v)) pointer_words.insert(at);
        }
    }

    // each instruction's text, kept only if it assembles back to its word
    build::Labels symbols;
    for (const auto& [a, n] : labels) symbols[n] = a;
    for (const auto& [n, a] : equates) symbols[n] = a;
    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    const auto instruction_text = [&](const dm::Insn& insn) -> std::string {
        const auto expected = [&](const std::string& line) {
            try {
                const Bytes b = dm::assemble(line, insn.pc, symbols, variant);
                return b.size() == 4 && le32(b, 0) == insn.word;
            } catch (const Error&) {
                return false;
            }
        };
        const auto it = immediate.find(insn.pc);
        if (it != immediate.end() && (insn.form == dm::Form::RtRsImm || insn.form == dm::Form::RtImm || insn.form == dm::Form::RtMem)) {
            const std::string line = dm::text(insn, names_for_text, it->second);
            if (expected(line)) return line;
        }
        const std::string plain = dm::text(insn, names_for_text);
        if (expected(plain)) return plain;
        return std::format(".word {}    # not written as an instruction: {}", hex8(insn.word), plain);
    };

    // the source ----------------------------------------------------------------------------------------------------------------
    std::vector<std::string> head = {
        std::format("# {}.s -- written by CodeBreaker from {}", stem, base_name),
        std::format("# {}: entry 0x{:08X}, {} bytes, {} functions", machine.description, elf.entry, file.size(), functions.size()),
        "# assemble with mips-as (GNU as, .set noreorder), or with CodeBreaker's own assembler",
        ""};
    translate::l2::Assembly out(head, "gas", std::string(file_name));
    out.append("        .set noreorder");
    out.append("        .set noat");
    out.append("");
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    const auto entry_text = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && line_start(a) ? it->second : hex8(a);
    };
    const auto pointer_name = [&](ByteView bytes, std::uint32_t piece_address) {
        return [&, bytes, piece_address](std::uint32_t a) -> std::optional<std::string> {
            if (!pointer_words.contains(a)) return std::nullopt;
            return name_of(le32(bytes, a - piece_address));
        };
    };
    const auto data_lines_of = [&](const Piece& p, std::size_t from, std::size_t to) {
        const ByteView bytes = file.subspan(p.offset, p.size);
        for (std::size_t at = from; at < to;) {
            const std::uint32_t addr = p.mapped ? p.address + static_cast<std::uint32_t>(at) : 0;
            const std::function<std::optional<std::string>(std::uint32_t)> none = [](std::uint32_t) -> std::optional<std::string> { return std::nullopt; };
            const DataItem item = data_item(bytes, at, to, addr, p.mapped ? std::function<std::optional<std::string>(std::uint32_t)>(pointer_name(bytes, p.address)) : none);
            out.append(item.line);
            at += item.took;
        }
    };

    for (const Piece& p : pieces) {
        const ByteView bytes = file.subspan(p.offset, p.size);
        if (p.role == Role::Header) {
            out.part("elf header", "header", std::nullopt, p.size);
            out.append("");
            out.append(std::format("        .org {}    # file offset", hex8(static_cast<std::uint32_t>(p.offset))));
            if (p.size == me::kHeaderSize) {
                out.append(byte_row(bytes.first(16)) + "    # e_ident: ELF, 32-bit, little-endian, version, OS/ABI");
                out.append(std::format("        .half 0x{:04X}, {}    # type, machine", elf.type, elf.machine));
                out.append(std::format("        .word 0x{:08X}    # version", elf.version));
                out.append(std::format("        .word {}    # entry point", entry_text(elf.entry)));
                out.append(std::format("        .word {}, {}    # program header table offset, section header table offset", hex8(elf.phoff), hex8(elf.shoff)));
                out.append(std::format("        .word 0x{:08X}    # flags", elf.flags));
                out.append(std::format("        .half {}, {}, {}, {}, {}, {}    # header size, program header size and count, section header size and count, section name table",
                                       elf.ehsize, elf.phentsize, elf.phnum, elf.shentsize, elf.shnum, elf.shstrndx));
            } else {
                data_lines_of(p, 0, p.size);
            }
            continue;
        }
        if (p.role == Role::ProgramHeaders) {
            out.part("program headers", "header", std::nullopt, p.size);
            out.append("");
            out.append(std::format("        .org {}    # file offset", hex8(static_cast<std::uint32_t>(p.offset))));
            for (std::size_t at = 0; at + me::kProgramHeaderSize <= bytes.size(); at += me::kProgramHeaderSize) {
                std::vector<std::string> items;
                for (std::size_t k = 0; k < 8; ++k) items.push_back(hex8(le32(bytes, at + 4 * k)));
                out.append(std::format("        .word {}    # program header {}: type, offset, vaddr, paddr, file size, memory size, flags, alignment", text::join(items, ", "), at / me::kProgramHeaderSize));
            }
            if (bytes.size() % me::kProgramHeaderSize != 0) out.append(byte_row(bytes.subspan(bytes.size() - bytes.size() % me::kProgramHeaderSize)));
            continue;
        }
        if (p.role == Role::SectionHeaders) {
            out.part("section headers", "header", std::nullopt, p.size);
            out.append("");
            out.append(std::format("        .org {}    # file offset", hex8(static_cast<std::uint32_t>(p.offset))));
            for (std::size_t at = 0, n = 0; at + me::kSectionHeaderSize <= bytes.size(); at += me::kSectionHeaderSize, ++n) {
                std::vector<std::string> items;
                for (std::size_t k = 0; k < 10; ++k) items.push_back(hex8(le32(bytes, at + 4 * k)));
                const std::string what = n < elf.sections.size() ? elf.sections[n].name : std::string();
                out.append(std::format("        .word {}    # section {} {}: name, type, flags, address, offset, size, link, info, alignment, entry size", text::join(items, ", "), n, what));
            }
            if (bytes.size() % me::kSectionHeaderSize != 0) out.append(byte_row(bytes.subspan(bytes.size() - bytes.size() % me::kSectionHeaderSize)));
            continue;
        }
        if (p.role == Role::Other) {
            out.part(p.name, "data", std::nullopt, p.size);
            out.append("");
            out.append(std::format("        .org {}    # {} (file offset)", hex8(static_cast<std::uint32_t>(p.offset)), p.name));
            data_lines_of(p, 0, p.size);
            continue;
        }
        // loaded code or data, at its address
        out.part(p.name, p.role == Role::Code ? "code" : "data", std::nullopt, p.size);
        out.append("");
        out.append(std::format("        .org {}    # {}", hex8(p.address), p.name));
        std::size_t at = 0;
        while (at < p.size) {
            const std::uint32_t addr = p.address + static_cast<std::uint32_t>(at);
            if (const auto label = labels.find(addr); label != labels.end() && line_start(addr)) {
                if (const auto fn = functions.find(addr); fn != functions.end()) {
                    out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});
                    out.append("");
                }
                out.append(label->second + ":");
            }
            if (p.role == Role::Code) {
                if (const auto it = code.find(addr); it != code.end() && at + 4 <= p.size) {
                    out.append("        " + instruction_text(it->second));
                    at += 4;
                    continue;
                }
            }
            std::size_t limit = p.size;
            if (const auto next = labels.upper_bound(addr); next != labels.end() && next->first - p.address < limit) limit = next->first - p.address;
            if (p.role == Role::Code) {
                if (const auto next = code.upper_bound(addr); next != code.end() && next->first - p.address < limit) limit = next->first - p.address;
            }
            limit = std::max<std::size_t>(limit, at + 1);
            const DataItem item = data_item(bytes, at, std::min(limit, p.size), addr, pointer_name(bytes, p.address));
            out.append(item.line);
            at += item.took;
        }
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written w = iw::finish(folder, stem + ".s", source, functions, code.size() * 4, file.size());
    const build::mips_asm::Assembler assembler(*machine.encoder);
    iw::assembled(w, [&] { return assembler.build(source); }, file, folder / (stem + ".built" + machine.extension), "program");
    return w;
}

}  // namespace cb::source::mips_elf_writer
