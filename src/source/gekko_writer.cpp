// cb-unity: alone (file-local names that other files in a batch also use)
// source/gekko_writer.cpp -- a Nintendo GameCube, Wii or Wii U program as assembly source that builds back into it (gekko_writer.hpp).

#include "source/gekko_writer.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <cctype>
#include <set>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/ppc.hpp"
#include "formats/gekko_map.hpp"
#include "source/isa_ppc.hpp"
#include "source/risc_source.hpp"

namespace cb::source::gekko_writer {

namespace dp = decompile::ppc;
namespace pp = formats::ppc_program;
namespace iw = image_writer;

namespace {

// Two spellings of one instruction (the description's and the decoder's)?
bool same_instruction(const std::string& said, const std::string& decoded) {
    static const std::set<std::pair<std::string, std::string>> same = {
        {"mfspr", "mfctr"}, {"mfspr", "mflr"}, {"mfspr", "mfxer"}, {"mtspr", "mtctr"}, {"mtspr", "mtlr"}, {"mtspr", "mtxer"}, {"nor", "not"},
        {"nor.", "not."},   {"or", "mr"},      {"or.", "mr."},     {"ori", "nop"},     {"subi", "addi"},   {"subic", "addic"},  {"subic.", "addic."},
        {"subis", "addis"}, {"trap", "tw"}};
    if (said.starts_with("tw") && (decoded == "tw" || decoded == "twi")) return true;   // (`twlt`, `tweqi`: the trap's condition in its name)
    return same.contains({said, decoded});
}

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr unsigned kLookAhead = 8;           // instructions after a `lis` that may use what it loaded
constexpr unsigned kStringMin = 4;           // a run of printable bytes this long is a string
constexpr unsigned kStringMax = 96;
constexpr std::size_t kFillMin = 16;         // a run of one value this long is `.space` / `.fill`
constexpr unsigned kRounds = 4;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 400;     // how many instructions a function is looked at for before it is called data

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}
std::uint16_t be16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] << 8 | b[at + 1]); }

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }
std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

// ---- the program's loaded regions by address ------------------------------------------------------------------------

class Space {
public:
    explicit Space(const pp::Program& program) : file_(program.file) {
        for (const pp::Region& r : program.regions)
            if (r.role == pp::Role::Code || r.role == pp::Role::Data) regions_.push_back(&r);
        std::ranges::sort(regions_, {}, &pp::Region::address);
    }

    const pp::Region* find(std::uint32_t a) const {
        auto it = std::ranges::upper_bound(regions_, a, {}, &pp::Region::address);
        if (it == regions_.begin()) return nullptr;
        const pp::Region* r = *--it;
        return a - r->address < r->size ? r : nullptr;
    }
    bool is_code(std::uint32_t a) const {
        const pp::Region* r = find(a);
        return r && r->role == pp::Role::Code;
    }
    // May an instruction stand at `pc`?
    bool inside(std::uint32_t pc) const {
        const pp::Region* r = find(pc);
        return r && r->role == pp::Role::Code && pc % 4 == 0 && pc - r->address + 4 <= r->size;
    }
    std::optional<std::uint32_t> word(std::uint32_t a) const {
        const pp::Region* r = find(a);
        if (!r || a - r->address + 4 > r->size) return std::nullopt;
        return be32(file_, r->offset + (a - r->address));
    }
    std::optional<dp::Insn> decode(std::uint32_t pc) const {
        if (!inside(pc)) return std::nullopt;
        return dp::decode_word(*word(pc), pc);
    }
    ByteView file() const { return file_; }

private:
    ByteView file_;
    std::vector<const pp::Region*> regions_;
};

bool ends_flow(const dp::Insn& i) { return i.flow == dp::Flow::Jump || i.flow == dp::Flow::Return || i.flow == dp::Flow::Indirect || i.flow == dp::Flow::Halt; }

// `stwu r1,-N(r1)` or `mflr r0`: how a compiler opens a function that calls others or keeps a frame.
bool is_prologue(std::uint32_t w) { return ((w & 0xFFFF8003) == 0x94218000) || w == 0x7C0802A6; }

// What flow reaches from the roots.
struct Walked {
    std::map<std::uint32_t, dp::Insn> code;
    std::set<std::uint32_t> targets;   // every place a jump, branch or call inside the code goes
    std::set<std::uint32_t> calls;     // ... of those, the targets of `bl`
};

Walked walk(const Space& space, const std::set<std::uint32_t>& roots) {
    Walked out;
    std::vector<std::uint32_t> todo(roots.begin(), roots.end());
    while (!todo.empty()) {
        std::uint32_t pc = todo.back();
        todo.pop_back();
        while (space.inside(pc) && !out.code.contains(pc)) {
            std::optional<dp::Insn> insn = space.decode(pc);
            if (!insn) break;
            if (insn->target && space.inside(*insn->target) && insn->flow != dp::Flow::None) {
                out.targets.insert(*insn->target);
                if (insn->flow == dp::Flow::Call) out.calls.insert(*insn->target);
                todo.push_back(*insn->target);
            }
            const bool stop = ends_flow(*insn);
            out.code.emplace(pc, std::move(*insn));
            if (stop) break;
            pc += 4;
        }
    }
    return out;
}

// Do the words at `at` read as a function: instructions that all decode, whose branches stay in the code, that end in a return or a jump
// within a few hundred words? Data does not: random words stop decoding, and zeros never end.
bool reads_as_function(const Space& space, std::uint32_t at) {
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 4 * count;
        const std::optional<dp::Insn> insn = space.decode(pc);
        if (!insn) return false;
        if (insn->target && insn->flow != dp::Flow::Call && !space.inside(*insn->target)) return false;
        if (ends_flow(*insn)) return count >= 1 || insn->flow == dp::Flow::Return;
    }
    return false;
}

// Does the instruction write general register `reg` (or may it: the pairing of a `lis` with what uses it stops there)?
bool writes(const dp::Insn& i, unsigned reg) {
    const unsigned op = i.opcode();
    switch (op) {
        case 7: case 8: case 12: case 13: case 14: case 15: return i.rt() == reg;
        case 20: case 21: case 23: case 24: case 25: case 26: case 27: case 28: case 29: return i.ra() == reg;
        case 32: case 34: case 40: case 42: case 46: return i.rt() == reg;
        case 33: case 35: case 41: case 43: return i.rt() == reg || i.ra() == reg;
        case 37: case 39: case 45: case 47: case 49: case 51: case 53: case 55: return i.ra() == reg;
        case 31: return i.rt() == reg || i.ra() == reg;
        default: return false;
    }
}

// One use of what a `lis` loaded: the instruction that finishes the address.
struct Use {
    std::uint32_t lis;       // the `lis`'s address
    std::uint32_t at;        // the instruction that completes the address
    std::uint32_t address;   // the address they make
    bool ori;                // by `ori`: the high half is not adjusted for the low half's sign
};

// Every `lis` and what it is used for, as far as the code shows (the next few instructions, until the register is written or the flow leaves).
std::vector<Use> address_pairs(const std::map<std::uint32_t, dp::Insn>& code, const std::set<std::uint32_t>& starts) {
    std::vector<Use> out;
    for (const auto& [pc, p] : code) {
        if (p.opcode() != 15 || p.ra() != 0) continue;   // addis rD,0,imm
        const unsigned reg = p.rt();
        const std::uint32_t high = p.uimm() << 16;
        for (unsigned k = 1; k <= kLookAhead; ++k) {
            const std::uint32_t at = pc + 4 * k;
            const auto it = code.find(at);
            if (it == code.end() || starts.contains(at)) break;
            const dp::Insn& q = it->second;
            if (q.flow != dp::Flow::None) break;
            const unsigned op = q.opcode();
            if (op == 14 && q.ra() == reg) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.simm()), false});
            else if (op == 24 && q.rt() == reg) out.push_back({pc, at, high | q.uimm(), true});
            else if (op >= 32 && op <= 55 && q.ra() == reg) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.simm()), false});
            if (writes(q, reg)) break;
        }
    }
    return out;
}

// How an address is named: part of the program, a hardware register, or a variable in memory the program does not hold.
enum class Kind { None, Text, Hardware, Variable };

bool in_memory(pp::Console console, std::uint32_t a) {
    if (console == pp::Console::WiiU) return a >= 0x10000000 && a < 0xE0000000;
    return (a >= 0x80000000 && a < 0x81800000) || (a >= 0x90000000 && a < 0x94000000);
}

// A symbol table's names for the addresses it defines (an ELF built without stripping): functions over objects over the rest, global over local.
std::map<std::uint32_t, std::string> symbol_names(const pp::Program& program, const Space& space) {
    std::map<std::uint32_t, std::string> out;
    if (!program.elf) return out;
    const ByteView file = program.file;
    const auto& sections = program.elf->sections;
    std::map<std::uint32_t, int> rank;
    std::unordered_set<std::string> used;
    for (const formats::ppc_elf::SectionHeader& s : sections) {
        if (s.type != 2 || s.link >= sections.size() || s.entsize != 16) continue;   // SHT_SYMTAB
        const formats::ppc_elf::SectionHeader& strings = sections[s.link];
        if (strings.type == formats::ppc_elf::kTypeNobits || static_cast<std::uint64_t>(strings.offset) + strings.size > file.size()) continue;
        if (static_cast<std::uint64_t>(s.offset) + s.size > file.size()) continue;
        for (std::size_t at = s.offset; at + 16 <= static_cast<std::size_t>(s.offset) + s.size; at += 16) {
            const std::uint32_t name_at = be32(file, at), value = be32(file, at + 4);
            const unsigned info = file[at + 12];
            const unsigned type = info & 15, bind = info >> 4;
            if (be16(file, at + 14) == 0 || name_at == 0 || name_at >= strings.size) continue;   // undefined or unnamed
            if (type > 2) continue;                                                               // NOTYPE, OBJECT, FUNC only
            if (!space.find(value)) continue;
            std::string name;
            for (std::size_t i = strings.offset + name_at; i < static_cast<std::size_t>(strings.offset) + strings.size && file[i] != 0; ++i) {
                const char c = static_cast<char>(file[i]);
                name += (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$') ? c : '_';
            }
            if (name.empty() || name.starts_with(".L") || std::isdigit(static_cast<unsigned char>(name[0]))) continue;
            const int score = (type == 2 ? 4 : type == 1 ? 2 : 0) + (bind == 1 ? 1 : 0);
            const auto it = rank.find(value);
            if (it != rank.end() && it->second >= score) continue;
            rank[value] = score;
            out[value] = std::move(name);
        }
    }
    // one name for one address: a name used for two addresses keeps the first and the other gets its address
    std::map<std::uint32_t, std::string> unique;
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

// Where a string starts inside the word at buf[at] (1 to 3 bytes in), or 0.
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

// One line of data from buf[off], which is at `addr`, to before `limit`: a string, a run of one value, up to four words (a word that
// `pointer_at` names is written as the name), or a few bytes. How many bytes it took, and the line.
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
        if (const std::size_t lead = string_inside_word(buf, off, limit); lead != 0) return {byte_row(buf.subspan(off, lead)), lead};   // (the bytes before a string)
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 4 && off + took + 4 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0)) break;
            const std::optional<std::string> name = pointer_at(addr + static_cast<std::uint32_t>(took));
            items.push_back(name ? *name : hex8(be32(buf, at)));
            took += 4;
        }
        return {"        .long " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 4 != 0 ? 4 - addr % 4 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

}  // namespace

iw::Written write(const pp::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView file = program.file;
    const Space space(program);
    const pp::Console console = program.console;

    // function starts: the entry point; what code calls; code that begins as a function does after another one's end; what a pointer names
    std::set<std::uint32_t> entries;     // function roots
    std::set<std::uint32_t> blocks;      // other places flow is known to continue (a jump table's cases)
    if (space.inside(program.entry)) entries.insert(program.entry);
    Walked walked;
    std::vector<Use> uses;
    std::set<std::uint32_t> function_starts;
    // the words of the data (and of the code sections that are no instruction) that hold an address of the program
    std::vector<std::uint32_t> pointer_values;
    for (const pp::Region& r : program.regions) {
        if (r.role == pp::Role::Header || r.role == pp::Role::Other) continue;
        for (std::size_t off = 0; off + 4 <= r.size; off += 4) {
            const std::uint32_t v = be32(file, r.offset + off);
            if (v % 4 != 0 || !space.find(v)) continue;
            if (r.role == pp::Role::Code && dp::decode_word(v, 0)) continue;   // (a word of code that reads as an instruction is one)
            pointer_values.push_back(v);
        }
    }
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
            } else if (a >= 4 && walked.code.contains(a - 4) && ends_flow(walked.code.at(a - 4)) && space.decode(a)) {
                added |= blocks.insert(a).second;   // a case of a jump table: the code after an unconditional transfer
            }
        }
        // code that begins as a function does after another one's end
        for (const pp::Region& r : program.regions) {
            if (r.role != pp::Role::Code) continue;
            for (std::size_t off = 4; off + 4 <= r.size; off += 4) {
                const std::uint32_t at = r.address + static_cast<std::uint32_t>(off);
                if (walked.code.contains(at) || entries.contains(at)) continue;
                const std::uint32_t w = be32(file, r.offset + off), before = be32(file, r.offset + off - 4);
                if (!is_prologue(w)) continue;
                if (before == 0x4E800020 || before == 0x4E800420 || before == 0 || before == 0x60000000 || (before >> 26) == 18)
                    added |= entries.insert(at).second;
            }
        }
        if (!added) break;
    }
    std::set<std::uint32_t> roots = entries;
    roots.insert(blocks.begin(), blocks.end());
    walked = walk(space, roots);
    function_starts = entries;
    for (const std::uint32_t c : walked.calls) function_starts.insert(c);
    uses = address_pairs(walked.code, function_starts);
    std::erase_if(function_starts, [&](std::uint32_t a) { return !walked.code.contains(a); });   // (an entry whose bytes are not an instruction is no function)
    const std::map<std::uint32_t, dp::Insn>& code = walked.code;
    const auto line_start = [&](std::uint32_t a) { return space.find(a) != nullptr && (a % 4 == 0 || !code.contains(a & ~3u)); };

    // names: symbols the file has, the entry point, `sub_<address>`
    iw::Names names = symbol_names(program, space);
    if (!names.contains(program.entry) && function_starts.contains(program.entry)) names[program.entry] = "entry";
    const std::vector<std::uint32_t> starts(function_starts.begin(), function_starts.end());
    const iw::Functions functions = iw::name_functions(starts, names, kSub);
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;                 // the names an address outside the labels is written with
    std::unordered_map<std::uint32_t, std::string> immediate;    // by the instruction's address: how its immediate is written
    const auto name_of = [&](std::uint32_t a) -> std::optional<std::string> {
        if (space.find(a)) {
            if (!line_start(a)) return std::nullopt;
            const auto it = labels.find(a);
            if (it != labels.end()) return it->second;
            const std::string made = (code.contains(a) ? kLocal : kData)(a);
            labels[a] = made;
            return made;
        }
        if (const auto hardware = formats::gekko_map::hardware_name(console, a)) {
            equates[*hardware] = a;
            return *hardware;
        }
        if (in_memory(console, a)) {
            std::string n = std::format("var_{:08X}", a);
            equates[n] = a;
            return n;
        }
        return std::nullopt;
    };
    // a `lis` is written from one of its uses: an addi, load or store's (the high half is adjusted for the low half's sign) over an ori's, and
    // of those the lowest address
    struct HighHalf {
        std::uint32_t address;
        bool ori;
        std::string expression;
    };
    std::map<std::uint32_t, HighHalf> high_half;   // by the `lis`'s address
    for (const Use& u : uses) {
        const std::optional<std::string> name = name_of(u.address);
        if (!name) continue;
        immediate[u.at] = *name + "@l";
        HighHalf candidate{u.address, u.ori, *name + (u.ori ? "@h" : "@ha")};
        const auto it = high_half.find(u.lis);
        if (it == high_half.end() || std::tie(candidate.ori, candidate.address) < std::tie(it->second.ori, it->second.address)) high_half[u.lis] = std::move(candidate);
    }
    for (const auto& [pc, high] : high_half) immediate[pc] = high.expression;

    // words in the data that hold an address of the program (a jump table, a table of pointers, a vtable)
    std::unordered_set<std::uint32_t> pointer_words;
    for (const pp::Region& r : program.regions) {
        if (r.role == pp::Role::Header || r.role == pp::Role::Other) continue;
        for (std::size_t off = 0; off + 4 <= r.size; off += 4) {
            const std::uint32_t at = r.address + static_cast<std::uint32_t>(off);
            if (r.role == pp::Role::Code && (code.contains(at) || dp::decode_word(be32(file, r.offset + off), 0))) continue;   // (a word of code, or one that reads as an instruction)
            const std::uint32_t v = be32(file, r.offset + off);
            if (v % 4 == 0 && space.find(v) && name_of(v)) pointer_words.insert(at);
        }
    }

    // the labels that an instruction names, and how each instruction is written: its text, kept only if it assembles back to its word
    build::Labels symbols;
    const auto refresh_symbols = [&] {
        symbols.clear();
        for (const auto& [a, n] : labels) symbols[n] = a;
        for (const auto& [n, a] : equates) symbols[n] = a;
    };
    refresh_symbols();
    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    const auto instruction_text = [&](const dp::Insn& insn) -> std::string {
        // the description's text: a branch by its label, an address half by its expression; the bytes and the text in a comment when it is not an instruction
        // that assembles back to itself
        const std::array<std::uint8_t, 4> bytes = {static_cast<std::uint8_t>(insn.word >> 24), static_cast<std::uint8_t>(insn.word >> 16),
                                                   static_cast<std::uint8_t>(insn.word >> 8), static_cast<std::uint8_t>(insn.word)};
        std::map<std::uint32_t, std::string> by_value;
        std::vector<std::uint32_t> operands;
        if (insn.target)
            if (const auto named = names_for_text.find(*insn.target); named != names_for_text.end()) by_value[*insn.target] = named->second, operands.push_back(*insn.target);
        const auto it = immediate.find(insn.pc);
        const std::string expression = it != immediate.end() && dp::has_immediate(insn) ? it->second : std::string();
        std::string line = risc_source::code("ppc", bytes, insn.pc, by_value, operands, expression, {}, &symbols);
        // (the PowerPC description does not know the Gekko's paired singles: it may read their bits as another instruction that assembles back to them. The
        // decoder knows them: a text whose mnemonic is not the decoder's stays as its bits, with the decoder's text.)
        if (!line.starts_with(".byte")) {
            const std::string said = text::lower(line.substr(0, line.find_first_of(" \t")));
            if (said != text::lower(insn.mnemonic) && !same_instruction(said, text::lower(insn.mnemonic))) {
                std::string raw;
                for (const std::uint8_t b : bytes) raw += std::format("{}0x{:02x}", raw.empty() ? "" : ",", b);
                line = ".byte " + raw;
            }
        }
        if (line.starts_with(".byte")) {
            line.resize(std::min(line.find("  ; "), line.size()));
            line += "  ; " + dp::text(insn, names_for_text);
        }
        return line;
    };

    // the source ----------------------------------------------------------------------------------------------------------------
    const std::string kind = std::string(pp::kind_text(program.kind));
    std::vector<std::string> head = {
        std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
        std::format("; {} {}{}: {} ({}), entry 0x{:08X}, {} bytes, {} functions", pp::console_text(console), kind,
                    program.kind == pp::Kind::Rpx && program.elf && program.elf->inflated_sections != 0 ? " (decompressed)" : "", pp::cpu_text(console), "PowerPC 750 family", program.entry,
                    file.size(), functions.size()),
        std::format("; loaded: {}", [&] {
            std::vector<std::string> parts;
            for (const formats::ppc_segment::Segment& s : program.segments) parts.push_back(std::format("{} at 0x{:08X} ({} bytes)", s.name, s.address, s.size));
            return text::join(parts, ", ");
        }()),
        ""};
    head.insert(head.end() - 1, "; (PowerPC syntax as the processor's SLEIGH description writes it: build with `assemble --cpu ppc --sleigh`)");
    translate::l2::Assembly out(head, "sleigh-ppc", std::string(file_name));

    std::map<const pp::Region*, std::string> region_labels;   // for a DOL: the labels its header's addresses and sizes are written with
    for (const pp::Region& r : program.regions) {
        if (program.dol && (r.role == pp::Role::Code || r.role == pp::Role::Data)) region_labels[&r] = r.name;
    }

    // the equates come first: every name an instruction or a word uses that is not a label
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    const auto entry_text = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && line_start(a) ? it->second : hex8(a);
    };

    const auto data_lines_of = [&](const pp::Region& r, std::size_t from, std::size_t to) {
        for (std::size_t at = from; at < to;) {
            const std::uint32_t addr = r.address + static_cast<std::uint32_t>(at);
            const DataItem item = data_item(ByteView(file).subspan(r.offset, r.size), at, to, addr, [&](std::uint32_t a) -> std::optional<std::string> {
                if (!pointer_words.contains(a)) return std::nullopt;
                return name_of(be32(file, r.offset + (a - r.address)));
            });
            out.append(item.line);
            at += item.took;
        }
    };

    for (const pp::Region& r : program.regions) {
        const ByteView bytes = ByteView(file).subspan(r.offset, r.size);
        if (r.role == pp::Role::Header) {
            out.part(r.name, "header", std::nullopt, r.size);
            out.append("");
            out.append(std::format("        .org {}    ; {} (file offset)", hex8(static_cast<std::uint32_t>(r.offset)), r.name));
            if (program.dol && r.name == "dol header") {
                const formats::dol::Dol& d = *program.dol;
                const auto row = [&](std::string_view what, std::size_t from, std::size_t count, const std::function<std::string(std::size_t)>& item) {
                    std::vector<std::string> items;
                    for (std::size_t i = from; i < from + count; ++i) items.push_back(item(i));
                    out.append(std::format("        .long {}    ; {}", text::join(items, ", "), what));
                };
                const auto slot_label = [&](std::size_t i) -> std::string {
                    const std::string name = i < formats::dol::kTextSections ? std::format("text{}", i) : std::format("data{}", i - formats::dol::kTextSections);
                    return name;
                };
                row("file offsets of text0-6", 0, 7, [&](std::size_t i) { return hex8(d.offsets[i]); });
                row("file offsets of data0-10", 7, 11, [&](std::size_t i) { return hex8(d.offsets[i]); });
                row("load addresses of text0-6", 0, 7, [&](std::size_t i) { return d.sizes[i] ? slot_label(i) : std::string("0"); });
                row("load addresses of data0-10", 7, 11, [&](std::size_t i) { return d.sizes[i] ? slot_label(i) : std::string("0"); });
                row("sizes of text0-6", 0, 7, [&](std::size_t i) { return d.sizes[i] ? std::format("{0}_end - {0}", slot_label(i)) : std::string("0"); });
                row("sizes of data0-10", 7, 11, [&](std::size_t i) { return d.sizes[i] ? std::format("{0}_end - {0}", slot_label(i)) : std::string("0"); });
                out.append(std::format("        .long {}, {}    ; bss address and size", hex8(d.bss_address), hex8(d.bss_size)));
                out.append(std::format("        .long {}    ; entry point", entry_text(d.entry)));
                out.append(byte_row(bytes.subspan(0xE4, 28)) + "    ; padding");
            } else if (program.elf && r.name == "elf header") {
                const formats::ppc_elf::Elf& e = *program.elf;
                out.append(byte_row(bytes.first(16)) + "    ; e_ident: ELF, 32-bit, big-endian, version, OS/ABI, ABI version");
                out.append(std::format("        .short 0x{:04X}, 0x{:04X}    ; type, machine", e.type, e.machine));
                out.append(std::format("        .long 0x{:08X}    ; version", e.version));
                out.append(std::format("        .long {}    ; entry point", entry_text(e.entry)));
                out.append(std::format("        .long {}, {}    ; program header table offset, section header table offset", hex8(e.phoff), hex8(e.shoff)));
                out.append(std::format("        .long 0x{:08X}    ; flags", e.flags));
                out.append(std::format("        .short {}, {}, {}, {}, {}, {}    ; header size, program header size and count, section header size and count, section name table", e.ehsize,
                                       e.phentsize, e.phnum, e.shentsize, e.shnum, e.shstrndx));
            } else if (program.elf && r.name == "program headers") {
                for (std::size_t at = 0; at + formats::ppc_elf::kProgramHeaderSize <= bytes.size(); at += formats::ppc_elf::kProgramHeaderSize) {
                    std::vector<std::string> items;
                    for (std::size_t k = 0; k < 8; ++k) items.push_back(hex8(be32(bytes, at + 4 * k)));
                    out.append(std::format("        .long {}    ; program header {}: type, offset, vaddr, paddr, file size, memory size, flags, alignment", text::join(items, ", "),
                                           at / formats::ppc_elf::kProgramHeaderSize));
                }
            } else if (program.elf && r.name == "section headers") {
                for (std::size_t at = 0, n = 0; at + formats::ppc_elf::kSectionHeaderSize <= bytes.size(); at += formats::ppc_elf::kSectionHeaderSize, ++n) {
                    std::vector<std::string> items;
                    for (std::size_t k = 0; k < 10; ++k) items.push_back(hex8(be32(bytes, at + 4 * k)));
                    const std::string what = n < program.elf->sections.size() ? program.elf->sections[n].name : std::string();
                    out.append(std::format("        .long {}    ; section {} {}: name, type, flags, address, offset, size, link, info, alignment, entry size", text::join(items, ", "), n, what));
                }
            } else {
                data_lines_of(r, 0, r.size);
            }
            continue;
        }
        if (r.role == pp::Role::Other) {
            out.part(r.name, "data", std::nullopt, r.size);
            out.append("");
            out.append(std::format("        .org {}    ; {} (file offset)", hex8(static_cast<std::uint32_t>(r.offset)), r.name));
            data_lines_of(r, 0, r.size);
            continue;
        }

        // loaded code or data, at its address
        out.part(r.name, r.role == pp::Role::Code ? "code" : "data", std::nullopt, r.size);
        out.append("");
        out.append(std::format("        .org {}    ; {}", hex8(r.address), r.name));
        if (region_labels.contains(&r)) out.append(r.name + ":");
        std::size_t at = 0;
        while (at < r.size) {
            const std::uint32_t addr = r.address + static_cast<std::uint32_t>(at);
            if (const auto label = labels.find(addr); label != labels.end() && line_start(addr)) {
                if (const auto fn = functions.find(addr); fn != functions.end()) {
                    out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});   // each function a fragment
                    out.append("");
                }
                out.append(label->second + ":");
            }
            if (r.role == pp::Role::Code) {
                if (const auto it = code.find(addr); it != code.end()) {
                    out.append("        " + instruction_text(it->second));
                    at += 4;
                    continue;
                }
            }
            // data: up to the next label or instruction
            std::size_t limit = r.size;
            if (const auto next = labels.upper_bound(addr); next != labels.end() && next->first - r.address < limit) limit = next->first - r.address;
            if (r.role == pp::Role::Code) {
                if (const auto next = code.upper_bound(addr); next != code.end() && next->first - r.address < limit) limit = next->first - r.address;
            }
            const std::size_t room = std::max<std::size_t>(limit, at + 1);
            const DataItem item = data_item(bytes, at, std::min(room, r.size), addr, [&](std::uint32_t a) -> std::optional<std::string> {
                if (!pointer_words.contains(a)) return std::nullopt;
                return name_of(be32(file, r.offset + (a - r.address)));
            });
            out.append(item.line);
            at += item.took;
        }
        if (region_labels.contains(&r)) out.append(r.name + "_end:");
    }

    const std::string source = iw::text(out) + "\n";
    std::size_t code_total = 0;
    code_total = code.size() * 4;
    iw::Written w = iw::finish(folder, stem + ".s", source, functions, code_total, file.size());
    const std::string extension = program.kind == pp::Kind::Dol ? ".dol" : program.kind == pp::Kind::Rpx ? ".rpx" : ".elf";
    iw::assembled(w, [&] { return risc_source::assemble("ppc", source, folder); }, file, folder / (stem + ".built" + extension), "program");
    return w;
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    const std::optional<pp::Program> program = pp::read(file);
    if (!program) throw FormatError("gekko", 0, "not a GameCube, Wii or Wii U program (a DOL, an ELF or an RPX)");
    return write(*program, file.filename().string(), folder);
}

}  // namespace cb::source::gekko_writer
