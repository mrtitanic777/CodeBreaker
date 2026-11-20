#include "cb/source/ne_writer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <iterator>
#include <type_traits>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/source/assembly.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::ne_writer {
namespace {

namespace ne = formats::ne;
namespace l2 = translate::l2;
namespace xf = x86_flat;

constexpr std::string_view kReader = "ne_writer";
constexpr std::uint32_t kLastOfChain = 0xFFFF;
constexpr std::uint8_t kMoveableEntry = 0xFF;   // an internal reference to an entry-table ordinal, not to a segment

// A flow ends at one of these.
bool ends_flow(std::string_view m) {
    return m == "ret" || m == "retf" || m == "jmp" || m == "ljmp" || m == "hlt" || m == "iret" || m == "iretd" || m == "ud2";
}

// How many bytes a relocation patches, by its source type.
std::uint32_t patched_width(std::uint8_t source) {
    switch (source) {
    case 0: return 1;
    case 3: return 4;
    case 11: return 6;
    case 13: return 4;
    default: return 2;   // offsets and selectors (2, 5), and any other
    }
}

// The macros that write a register-to-register instruction in the direction NASM does not: width 1 is a 16-bit register, 2 a 32-bit one (it
// takes the operand-size prefix in 16-bit code), 0 a byte register.
const std::vector<std::string>& prelude() {
    static const std::vector<std::string> lines = [] {
        std::vector<std::string> p = {"", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
                                      "; Microsoft's compiler writes; NASM itself writes the other"};
        struct Group {
            std::array<std::string_view, 8> names;
            int width;
        };
        const std::array<Group, 3> groups = {{{{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}, 1},
                                              {{"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"}, 2},
                                              {{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}, 0}}};
        for (const Group& g : groups) {
            for (std::size_t k = 0; k < g.names.size(); ++k) p.push_back(std::format("%assign _r_{} {}", g.names[k], k));
            for (std::size_t k = 0; k < g.names.size(); ++k) p.push_back(std::format("%assign _w_{} {}", g.names[k], g.width));
        }
        for (const char* line : {"%macro _rr 3", "  %if _w_%2 == 2", "    db 0x66", "  %endif", "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3",
                                 "%endmacro"}) {
            p.emplace_back(line);
        }
        for (const xf::RegisterPairOp& op : xf::register_pair_ops()) p.push_back(std::format("%define {}_ _rr 0x{:02x},", op.mnemonic, op.opcode));
        return p;
    }();
    return lines;
}

std::string indented(const std::string& line) { return "    " + line; }

bool same_target(const ne::Relocation& a, const ne::Relocation& b) {
    if (a.target.index() != b.target.index()) return false;
    return std::visit(
        [&](const auto& x) {
            const auto& y = std::get<std::decay_t<decltype(x)>>(b.target);
            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ne::InternalRef>) return x.segment == y.segment && x.entry == y.entry;
            else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ne::ImportOrdinalRef>) return x.module == y.module && x.ordinal == y.ordinal;
            else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ne::ImportNameRef>) return x.module == y.module && x.name_offset == y.name_offset;
            else return x.fixup_type == y.fixup_type;
        },
        a.target);
}

bool same_relocation(const ne::Relocation& a, const ne::Relocation& b) {
    return a.source == b.source && a.flags == b.flags && a.offset == b.offset && same_target(a, b);
}

}  // namespace

std::string_view usage() {
    return R"(ne_writer -- a 16-bit Windows (or OS/2) program as assembly source that builds back into it, byte for byte.

    ne-source <program> [--out DIR] [--keep-stub]

The way pe_writer writes a 32-bit image, for the segmented kind: one NASM source assembled flat (-f bin),
each segment a section at its place in the file (start=) whose addresses are its own offsets (vstart=0),
followed by its relocation table. Whatever the linker made is written as what it refers to, not as a
number: the segment table's sectors and lengths as expressions of the segments' places, every relocation
chain (LINK writes, in each place a relocation patches, where the next place of the same target is) as the
label of the next place, every record's first place as its label, and an internal reference's offset as
the label in its segment. So the file is the linker's output as written down, and stands without the linker
that made it -- whichever version that was.

Code is what is reached from the entry table, from the program's start, and from every internal reference
into a code segment; what is not reached is data. An instruction is written as its mnemonic where NASM
gives back the original's bytes (checked every round from the assembler's map, as pe_writer does); one that
holds a place a relocation patches is written as its bytes, with those places as expressions and its
mnemonic beside it. The DOS program in front (the stub) comes from its own source when that builds back.
)";
}

// ---- reading the program ---------------------------------------------------------------------------------------------------------

Image::Image(ne::Module module, std::string file_name) : module_(std::move(module)), file_name_(std::move(file_name)) {
    shift_ = module_.header().alignment_shift;
    for (const ne::Segment& s : module_.segments()) {
        if (s.file_offset == 0) continue;   // (no data in the file)
        Segment g;
        g.index = s.index;
        g.file_offset = static_cast<std::size_t>(s.file_offset);
        g.length = s.length;
        g.flags = s.flags;
        g.relocations = module_.relocations(s);
        g.table = (s.flags & ne::kRelocInfo) ? 2 + 8 * g.relocations.size() : 0;
        g.code = (s.flags & ne::kSegmentData) == 0;
        segment_at_[g.index] = segments_.size();
        segments_.push_back(std::move(g));
    }
    // every place a relocation patches: (segment, offset) -> (record, width, chained); a chain of places ends at 0xFFFF or where it repeats
    for (const Segment& s : segments_) {
        for (std::size_t k = 0; k < s.relocations.size(); ++k) {
            const ne::Relocation& r = s.relocations[k];
            const std::uint32_t width = patched_width(r.source);
            std::uint32_t at = r.offset;
            std::set<std::uint32_t> seen;
            while (!seen.contains(at) && at + width <= s.length) {
                seen.insert(at);
                std::uint32_t next = kLastOfChain;
                if (!r.additive()) {
                    io::ByteReader reader(data(), kReader);
                    reader.seek(s.file_offset + at);
                    next = reader.u16();
                }
                sites_[{s.index, at}] = Site{k, width, !r.additive(), next};
                if (r.additive() || next == kLastOfChain) break;
                at = next;
            }
        }
    }
    decode_code();
}

Image Image::open(const std::filesystem::path& program) {
    return Image(ne::Module::open_file(program), fs::to_text(program.filename()));
}

ByteView Image::bytes(std::size_t lo, std::size_t hi) const {
    // (the file may end inside what a table says: what is there is all there is, as the original writer's slices were)
    const ByteView all = data();
    lo = std::min(lo, all.size());
    hi = std::min(std::max(hi, lo), all.size());
    return all.subspan(lo, hi - lo);
}

// ---- code ------------------------------------------------------------------------------------------------------------------------

void Image::decode_code() {
    const ne::Header& h = module_.header();
    std::vector<Place> seeds;
    for (const ne::Entry& e : module_.entries()) {
        if (e.kind != ne::Entry::Kind::Unused && has_segment(e.segment)) seeds.push_back({e.segment, e.offset});
    }
    const std::uint32_t cs = h.cs_ip >> 16;
    if (has_segment(cs)) seeds.push_back({cs, h.cs_ip & 0xFFFF});
    const std::vector<Place> entries = seeds;   // (where the module is entered: each a function)
    for (const Segment& s : segments_) {
        for (const ne::Relocation& r : s.relocations) {
            if (const auto* in = std::get_if<ne::InternalRef>(&r.target); in && in->segment != kMoveableEntry && has_segment(in->segment)) {
                seeds.push_back({in->segment, in->entry});
            }
        }
    }
    std::vector<Place> work;   // (taken from the end)
    for (const Place& p : seeds) {
        if (has_segment(p.segment) && segment(p.segment).code) work.push_back(p);
    }
    std::map<Place, std::uint32_t> site_bytes;   // each byte a relocation patches -> where its place starts
    for (const auto& [at, site] : sites_) {
        for (std::uint32_t j = 0; j < site.width; ++j) site_bytes[{at.segment, at.offset + j}] = at.offset;
    }
    const platforms::Decoder& decoder = decoders_.get(platforms::Arch::X86, 16, true);
    while (!work.empty()) {
        const Place start = work.back();
        work.pop_back();
        const Segment& s = segment(start.segment);
        std::uint32_t a = start.offset;
        while (a < s.length && !insns_.contains({start.segment, a}) && !covered_.contains({start.segment, a})) {
            const std::size_t o = s.file_offset + a;
            const std::optional<xf::Insn> one = xf::decode(decoder, bytes(o, std::min(o + 15, s.file_offset + s.length)), a);
            if (!one) break;
            bool overlap = false;
            for (std::uint32_t k = 0; k < one->size && !overlap; ++k) overlap = covered_.contains({start.segment, a + k});
            if (overlap) break;
            // a relocated place must lie wholly inside the instruction
            std::set<std::uint32_t> starts_in;
            for (std::uint32_t k = 0; k < one->size; ++k) {
                if (const auto hit = site_bytes.find({start.segment, a + k}); hit != site_bytes.end()) starts_in.insert(hit->second);
            }
            bool cut = false;
            for (const std::uint32_t st : starts_in) {
                if (st < a || st + sites_.at({start.segment, st}).width > a + one->size) cut = true;
            }
            if (cut) break;
            insns_[{start.segment, a}] = *one;
            for (std::uint32_t k = 0; k < one->size; ++k) covered_[{start.segment, a + k}] = a;
            const std::optional<std::uint32_t> target = xf::branch_target(*one);
            if (target && starts_in.empty()) work.push_back({start.segment, *target});
            if (ends_flow(one->mnemonic)) break;
            a += one->size;
        }
    }
    starts_ = find_function_starts(entries);
}

// Where each function begins, for the fragment each is in the file's L2 tree: the entry points and every place a call lands -- a near call,
// or a far call through a relocation to a segment of the module's own -- where an instruction was found.
std::set<Place> Image::find_function_starts(const std::vector<Place>& entries) const {
    std::set<Place> out(entries.begin(), entries.end());
    for (const auto& [at, insn] : insns_) {
        const std::optional<std::uint32_t> target = xf::branch_target(insn);
        if (insn.mnemonic == "call" && target) out.insert({at.segment, *target});
    }
    for (const auto& [at, site] : sites_) {
        const auto start = covered_.find(at);
        const ne::Relocation& r = segment(at.segment).relocations[site.record];
        const auto* in = std::get_if<ne::InternalRef>(&r.target);
        if (start != covered_.end() && static_cast<std::int64_t>(start->second) == static_cast<std::int64_t>(at.offset) - 1 &&
            insns_.at({at.segment, start->second}).mnemonic == "lcall" && in && in->segment != kMoveableEntry) {
            out.insert({in->segment, in->entry});
        }
    }
    std::set<Place> found;
    for (const Place& p : out) {
        if (insns_.contains(p)) found.insert(p);
    }
    return found;
}

// ---- names -----------------------------------------------------------------------------------------------------------------------

std::string Image::label(Place at) const {
    return insns_.contains(at) ? std::format("s{}_L{:x}", at.segment, at.offset) : std::format("s{}_D{:x}", at.segment, at.offset);
}

std::string Image::ref(std::uint32_t segment, std::uint32_t offset) const {
    if (const auto p = placed_.find({segment, offset}); p != placed_.end()) return p->second;
    if (const auto start = covered_.find({segment, offset}); start != covered_.end()) {
        // (an instruction start that something points into is always placed)
        return std::format("{}+{}", placed_.at({segment, start->second}), offset - start->second);
    }
    return std::format("0x{:x}", offset);
}

// The places that need a label: what relocations patch and name, the entry points, the branch targets, and with `mark_all` every instruction.
std::set<Place> Image::wanted(bool mark_all) const {
    std::set<Place> want;
    for (const Segment& s : segments_) {
        for (const ne::Relocation& r : s.relocations) {
            want.insert({s.index, r.offset});
            if (const auto* in = std::get_if<ne::InternalRef>(&r.target); in && has_segment(in->segment)) want.insert({in->segment, in->entry});
        }
    }
    for (const auto& [at, site] : sites_) {
        want.insert(at);
        if (site.chained && site.next != kLastOfChain) want.insert({at.segment, site.next});
    }
    for (const ne::Entry& e : module_.entries()) {
        if (e.kind != ne::Entry::Kind::Unused && has_segment(e.segment)) want.insert({e.segment, e.offset});
    }
    for (const auto& [at, insn] : insns_) {
        if (const auto target = xf::branch_target(insn)) want.insert({at.segment, *target});
    }
    if (mark_all) {
        for (const auto& entry : insns_) want.insert(entry.first);
    }
    return want;
}

// ---- source ----------------------------------------------------------------------------------------------------------------------

// An instruction's lines: its mnemonic in NASM's spelling, or its bytes with the relocated places and branch distances as expressions (`raw`).
std::vector<std::string> Image::text(std::uint32_t segment, const xf::Insn& insn, bool raw) const {
    const std::uint32_t a = insn.address;
    const ByteView b = insn.raw();
    const std::optional<std::uint32_t> target = xf::branch_target(insn);
    std::vector<std::uint32_t> inside;   // the offsets in the instruction where a relocation patches
    for (std::uint32_t k = 0; k < insn.size; ++k) {
        if (sites_.contains({segment, a + k})) inside.push_back(k);
    }
    if (!raw && inside.empty()) {
        if (const std::optional<std::string> t = nasm_text(segment, insn, target); t && !t->empty()) return {*t};
    }
    std::vector<std::string> out;
    std::uint32_t k = 0;
    const std::uint32_t n = insn.size;
    while (k < n) {
        if (std::ranges::find(inside, k) != inside.end()) {
            const Site& site = sites_.at({segment, a + k});
            if (k != 0) out.push_back(placed_.at({segment, a + k}) + ":");
            if (site.chained) {
                out.push_back("dw " + (site.next == kLastOfChain ? std::string("0xffff") : ref(segment, site.next)));
                if (site.width > 2) out.push_back("db " + xf::db_bytes(b, k + 2, std::min(k + site.width, n)));
            } else {
                out.push_back("db " + xf::db_bytes(b, k, std::min(k + site.width, n)));
            }
            k += site.width;
        } else if (target && k == insn.imm_offset) {
            const char* directive = insn.imm_size == 4 ? "dd" : insn.imm_size == 2 ? "dw" : "db";
            CB_ASSERT(insn.imm_size == 4 || insn.imm_size == 2 || insn.imm_size == 1, "a branch's displacement is 1, 2 or 4 bytes");
            out.push_back(std::format("{} {}-($+{})", directive, ref(segment, *target), insn.imm_size));
            k += insn.imm_size;
        } else {
            std::uint32_t stop = n;
            for (const std::uint32_t x : inside) {
                if (x > k) stop = std::min(stop, x);
            }
            if (target && insn.imm_offset > k) stop = std::min<std::uint32_t>(stop, insn.imm_offset);
            out.push_back("db " + xf::db_bytes(b, k, stop));
            k = stop;
        }
    }
    std::size_t first = 0;
    while (first < out.size() && out[first].ends_with(':')) ++first;
    CB_ASSERT(first < out.size(), "an instruction has bytes to write");
    out[first] = xf::with_original(out[first], insn);
    return out;
}

// The instruction in NASM's spelling, or nothing where only its bytes will do.
std::optional<std::string> Image::nasm_text(std::uint32_t segment, const xf::Insn& insn, const std::optional<std::uint32_t>& target) const {
    static const Regex base_and_index(R"([a-z]{2}\s*[+-])");
    const std::string& m = insn.mnemonic;
    if (m == "ljmp" || m == "lcall") return std::nullopt;
    if (xf::is_string_op(m)) {
        for (const std::uint8_t p : insn.prefix) {
            if (xf::is_segment_prefix(p)) return std::nullopt;
        }
        return m;
    }
    if (target) {
        if (m == "call") {
            if (insn.imm_size == 2 && insn.size == 3) return "call " + ref(segment, *target);
            return std::nullopt;
        }
        if (m == "loop" || m == "loope" || m == "loopne" || m == "jcxz" || m == "jecxz") {
            if (insn.size == 2) return m + " " + ref(segment, *target);
            return std::nullopt;
        }
        if (insn.size != 2 && insn.size != 3 && insn.size != 4) return std::nullopt;
        return std::format("{} {} {}", m, insn.imm_size == 1 ? "short" : "near", ref(segment, *target));
    }
    if (const std::optional<std::string> pair = xf::register_pair_text(insn)) return pair;
    std::string ops = xf::nasm_operands(insn.op_str);
    if (insn.imm_size == 2 && xf::is_arith(m)) {
        if (const auto n = xf::trailing_number(ops); n && ((-128 <= n->value && n->value <= 127) || n->value >= 0xFF80)) {
            ops.insert(n->start, "strict word ");
        }
    }
    if (insn.disp_size == 2) {
        if (const std::size_t open = ops.find('['); open != std::string::npos) {
            if (const std::size_t close = ops.find(']', open + 1); close != std::string::npos) {
                const std::string inner = ops.substr(open + 1, close - open - 1);
                if (base_and_index.test(inner)) {
                    const auto n = xf::trailing_number(inner);   // a displacement of at most 0x7f after "+ " or "- " is the short form
                    if (n && n->start >= 2 && inner[n->start - 1] == ' ' && (inner[n->start - 2] == '+' || inner[n->start - 2] == '-') &&
                        inner[n->start] != '-' && n->value <= 0x7F) {
                        ops = ops.substr(0, open) + "[word " + inner + "]" + ops.substr(close + 1);
                    }
                }
            }
        }
    }
    return std::string(text::trim(m + " " + ops));
}

flat_rounds::Source<Place> Image::write(const std::set<Place>& raw, bool final) {
    return final ? source(raw, false, true) : source(raw, true, false);
}

flat_rounds::Source<Place> Image::source(const std::set<Place>& raw_in, bool mark_all, bool exact) {
    std::set<Place> raw = raw_in;
    const std::set<Place> want = wanted(mark_all);
    placed_.clear();
    for (const Place& key : want) {
        if (has_segment(key.segment) && (insns_.contains(key) || !covered_.contains(key)) && key.offset <= segment(key.segment).length) {
            placed_[key] = label(key);
        }
    }
    for (const Place& key : want) {
        const auto start = covered_.find(key);
        if (start == covered_.end() || insns_.contains(key)) continue;
        const Place first{key.segment, start->second};
        if (sites_.contains(key) && insns_.contains(first)) {
            placed_[key] = std::format("s{}_S{:x}", key.segment, key.offset);   // (a place inside an instruction written as bytes)
            raw.insert(first);
        }
        placed_.try_emplace(first, label(first));
    }
    written_raw_ = raw;

    l2::Assembly out({}, "nasm", file_name_);   // (the file as one L2 tree, as it is written)
    std::map<std::size_t, Place> lines_of;
    out.append(std::format("; {} -- assembled with NASM: nasm -f bin -o {} this-file", file_name_, file_name_));
    out.append("bits 16");
    out.append(std::format("SHIFT equ {}", shift_));
    out.append("; where each segment lies in the file: its section's place, and its sector in the segment table");
    for (const Segment& s : segments_) out.append(std::format("seg{}_start_file equ 0x{:x}", s.index, s.file_offset));
    out.extend(prelude());

    struct Region {
        std::size_t lo, hi;
        const Segment* segment;
    };
    std::vector<Region> regions;
    for (const Segment& s : segments_) regions.push_back({s.file_offset, s.file_offset + s.length + s.table, &s});
    std::ranges::stable_sort(regions, {}, &Region::lo);
    regions.push_back({data().size(), data().size(), nullptr});

    std::size_t pos = 0, n = 0;
    std::optional<std::string> prev;
    const auto place_of_section = [&](std::string_view start) {
        return exact || !prev ? std::string(start) : "follows=" + *prev;
    };
    for (const Region& rg : regions) {
        if (rg.lo > pos) {
            out.part(std::format("part{}", n), "outside the segments");
            out.append("");
            out.append(std::format("section part{} {} vstart=0x{:x} align=1", n, place_of_section(std::format("start=0x{:x}", pos)), pos));
            prev = std::format("part{}", n);
            if (pos == 0 && stub_) {
                out.append(std::format("incbin \"{}\"", fs::to_text(*stub_)));
                pos = static_cast<std::size_t>(fs::file_size(*stub_));
            }
            data_outside_segments(out, pos, rg.lo);
            ++n;
        }
        if (rg.segment == nullptr) break;
        const Segment& s = *rg.segment;
        out.part(std::format("seg{}", s.index), "segment", translate::ir::OriginSegment{file_name_, s.index, 0}, s.length);
        out.append("");
        // (while the rounds look for what NASM does not give back, a section that came out longer must not run into the next: each follows
        // the one before; the last round places them)
        std::string where;
        if (exact) {
            where = std::format("start=seg{}_start_file", s.index);
        } else {
            CB_ASSERT(prev.has_value(), "the sections after the first follow one that was written");
            where = "follows=" + *prev;
        }
        out.append(std::format("section seg{} {} vstart=0 align=1", s.index, where));
        prev = std::format("seg{}", s.index);
        out.append(std::format("seg{}_start:", s.index));
        write_segment(out, s, lines_of);
        pos = rg.hi;
    }
    out.append("");
    assembly_ = out;
    return {image_writer::text(out), std::move(lines_of)};
}

// Bytes of the file outside the segments; the segment table's sectors and lengths as expressions.
void Image::data_outside_segments(l2::Assembly& out, std::size_t from, std::size_t to) const {
    const ne::Header& h = module_.header();
    const std::size_t table = module_.ne_offset() + h.segment_table;
    std::map<std::size_t, std::uint32_t> rows;   // where each segment table entry lies -> its segment
    for (std::uint32_t k = 0; k < h.segment_count; ++k) rows[table + 8 * k] = k + 1;
    std::size_t p = from;
    while (p < to) {
        const auto row = rows.find(p);
        if (row != rows.end() && has_segment(row->second) && p + 8 <= to) {
            const Segment& s = segment(row->second);
            out.append(std::format("    dw seg{}_start_file >> SHIFT, seg{}_end{}", row->second, row->second, s.length < 0x10000 ? "" : " & 0xffff"));
            for (const std::string& x : assembly::data_lines(bytes(p + 4, p + 8))) out.append(indented(x));
            p += 8;
            continue;
        }
        const auto after = rows.upper_bound(p);
        const std::size_t next = after == rows.end() ? to : std::min(to, after->first);
        for (const std::string& x : assembly::data_lines(bytes(p, next))) out.append(indented(x));
        p = next;
    }
}

void Image::write_segment(l2::Assembly& out, const Segment& s, std::map<std::size_t, Place>& lines_of) const {
    const std::uint32_t si = s.index;
    const std::size_t base = s.file_offset, length = s.length;
    std::vector<std::uint32_t> mine;   // the function starts in this segment, ascending
    for (const Place& p : starts_) {
        if (p.segment == si && p.offset < length) mine.push_back(p.offset);
    }
    std::map<std::uint32_t, std::size_t> ends;   // (a function runs to the next one)
    for (std::size_t k = 0; k < mine.size(); ++k) ends[mine[k]] = k + 1 < mine.size() ? mine[k + 1] : length;
    std::size_t a = 0;
    while (a < length) {
        const Place key{si, static_cast<std::uint32_t>(a)};
        if (const auto found = insns_.find(key); found != insns_.end()) {
            const xf::Insn& insn = found->second;
            if (const auto fe = ends.find(key.offset); fe != ends.end()) {   // (each function a fragment of its own)
                const auto name = placed_.find(key);
                out.part(name != placed_.end() && !name->second.empty() ? name->second : label(key), "function",
                         translate::ir::OriginSegment{file_name_, si, static_cast<std::int64_t>(a)}, fe->second - a);
            }
            if (const auto name = placed_.find(key); name != placed_.end()) out.append(name->second + ":");
            for (const std::string& t : text(si, insn, written_raw_.contains(key))) {
                lines_of[out.lines().size() + 1] = key;
                out.append(t.ends_with(':') ? t : indented(t));
            }
            a += insn.size;
            continue;
        }
        std::size_t stop = a + 1;
        while (stop < length && !insns_.contains({si, static_cast<std::uint32_t>(stop)})) ++stop;
        write_data(out, si, base, a, stop);
        a = stop;
    }
    if (const auto end = placed_.find({si, static_cast<std::uint32_t>(length)}); end != placed_.end()) out.append(end->second + ":");
    out.append(std::format("seg{}_end:", si));
    if (s.table != 0) {
        out.append(std::format("    dw {}{}; relocations", s.relocations.size(), std::string(35, ' ')));
        for (const ne::Relocation& r : s.relocations) {
            // (the record's place in the table is that of the first record equal to it)
            std::size_t index = 0;
            for (std::size_t q = 0; q < s.relocations.size(); ++q) {
                if (same_relocation(s.relocations[q], r)) {
                    index = q;
                    break;
                }
            }
            const std::size_t p = base + length + 2 + 8 * index;
            if (p + 8 > data().size()) throw FormatError(kReader, p, "a relocation record lies past the end of the file");
            const ByteView rec = bytes(p, p + 8);
            const std::string head = std::format("db 0x{:02x},0x{:02x}", rec[0], rec[1]);
            const std::string where = ref(si, r.offset);
            const auto* in = std::get_if<ne::InternalRef>(&r.target);
            if (in && in->segment != kMoveableEntry && has_segment(in->segment)) {
                out.append("    " + head);
                out.append("    dw " + where);
                out.append(std::format("    db 0x{:02x},0x{:02x}", rec[4], rec[5]));
                out.append("    dw " + ref(in->segment, in->entry));
            } else {
                out.append("    " + head);
                out.append("    dw " + where);
                out.append("    db " + xf::db_bytes(rec, 4, 8));
            }
        }
    }
}

// Bytes of segment `segment` from `from` to `to` (offsets in it; `base` is where it starts in the file): labels where they stand, the places
// a relocation patches as what they hold, the rest as data.
void Image::write_data(l2::Assembly& out, std::uint32_t segment, std::size_t base, std::size_t from, std::size_t to) const {
    std::set<std::size_t> cut_set;
    const auto in_range = [&](const Place& k) { return k.segment == segment && from <= k.offset && k.offset < to; };
    for (const auto& entry : placed_) {
        if (in_range(entry.first)) cut_set.insert(entry.first.offset);
    }
    for (const auto& entry : sites_) {
        if (in_range(entry.first)) cut_set.insert(entry.first.offset);
    }
    std::vector<std::size_t> cuts(cut_set.begin(), cut_set.end());
    cuts.push_back(to);
    std::size_t pos = from;
    for (const std::size_t c : cuts) {
        const Place key{segment, static_cast<std::uint32_t>(c)};
        const auto name = placed_.find(key);
        if (c < pos) {   // (inside the place just written)
            if (name != placed_.end()) out.append(std::format("{} equ $-{}", name->second, pos - c));
            continue;
        }
        if (c > pos) {
            for (const std::string& x : assembly::data_lines(bytes(base + pos, base + c))) out.append(indented(x));
            pos = c;
        }
        if (c == to) break;
        if (name != placed_.end()) out.append(name->second + ":");
        const auto site = sites_.find(key);
        if (site == sites_.end() || c + site->second.width > to) continue;
        const Site& w = site->second;
        if (w.chained) {
            out.append("    dw " + (w.next == kLastOfChain ? std::string("0xffff") : ref(segment, w.next)));
            if (w.width > 2) {
                for (const std::string& x : assembly::data_lines(bytes(base + c + 2, base + c + w.width))) out.append(indented(x));
            }
        } else {
            for (const std::string& x : assembly::data_lines(bytes(base + c, base + c + w.width))) out.append(indented(x));
        }
        pos = c + w.width;
    }
}

// The instructions the assembler did not give back: compared where each one went, its length (to the next instruction's label) and its
// bytes, outside the places whose value depends on where things are (relocated addresses, branch distances).
std::set<Place> Image::differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Place>& raw_in) {
    std::set<Place> raw = raw_in;
    raw.insert(written_raw_.begin(), written_raw_.end());
    std::set<Place> bad;
    const auto label_of = [&](const Place& p) -> const flat_rounds::Label* {
        const auto name = placed_.find(p);
        if (name == placed_.end()) return nullptr;
        const auto hit = labels.find(name->second);
        return hit == labels.end() ? nullptr : &hit->second;
    };
    for (auto it = insns_.begin(); it != insns_.end(); ++it) {
        const Place key = it->first;
        const xf::Insn& insn = it->second;
        if (raw.contains(key)) continue;
        const flat_rounds::Label* here = label_of(key);
        if (here == nullptr) {
            bad.insert(key);
            continue;
        }
        if (const auto next = std::next(it); next != insns_.end() && next->first.segment == key.segment && next->first.offset == key.offset + insn.size) {
            const flat_rounds::Label* there = label_of(next->first);
            if (there != nullptr && static_cast<std::int64_t>(there->address) - static_cast<std::int64_t>(here->address) != insn.size) {
                bad.insert(key);
                continue;
            }
        }
        const std::size_t real = static_cast<std::size_t>(here->place);
        const std::size_t original_at = segment(key.segment).file_offset + key.offset;
        const ByteView mine = built.subspan(std::min(real, built.size()), std::min<std::size_t>(insn.size, built.size() - std::min(real, built.size())));
        const ByteView original = bytes(original_at, original_at + insn.size);
        const bool relative = xf::branch_target(insn).has_value();   // (its displacement depends on where things are)
        bool differs = mine.size() != original.size();
        for (std::size_t k = 0; !differs && k < original.size(); ++k) {
            const bool skipped = relative && k >= insn.imm_offset && k < static_cast<std::size_t>(insn.imm_offset) + insn.imm_size;
            if (!skipped && mine[k] != original[k]) differs = true;
        }
        if (differs) bad.insert(key);
    }
    return bad;
}

// ---- the result ------------------------------------------------------------------------------------------------------------------

flat_rounds::FlatReport Result::report() const {
    flat_rounds::FlatReport r;
    r.instructions = instructions;
    r.raw = raw;
    r.rounds = rounds;
    r.same = same;
    r.built = image.has_value();
    r.source = source;
    return r;
}

Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir_, const Options& options) {
    const std::function<void(std::string_view)> say = options.say ? options.say : [](std::string_view) {};
    const std::filesystem::path outdir = std::filesystem::absolute(outdir_);
    fs::make_dirs(outdir);
    Image image = Image::open(program);
    const std::string name = text::lower(fs::to_text(program.filename()));
    std::optional<std::filesystem::path> stub;
    if (std::holds_alternative<SearchStub>(options.stub)) {
        if (options.find_stub) stub = options.find_stub(image.module().data(), image.module().ne_offset(), outdir, say);
    } else if (const auto* given = std::get_if<std::filesystem::path>(&options.stub)) {
        stub = *given;
    }
    if (stub && stub->empty()) stub.reset();
    image.set_stub(stub);

    const flat_rounds::FlatResult<Place> fr =
        flat_rounds::flat<Place>(image.module().data(), image, outdir, name, {.nasm = options.nasm, .say = say});
    std::set<Place> raw = fr.raw;
    raw.insert(image.written_as_bytes().begin(), image.written_as_bytes().end());
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = image.instruction_count();
    r.raw = raw.size();
    return r;
}

int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say) {
    std::vector<std::string> programs;
    std::optional<std::string> out;
    bool keep_stub = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out") {
            if (i + 1 >= args.size()) throw UsageError("--out needs a folder");
            out = args[++i];
        } else if (args[i] == "--keep-stub") {
            keep_stub = true;
        } else if (!args[i].starts_with("--")) {
            programs.push_back(args[i]);
        }
    }
    if (programs.empty()) {
        say(usage());
        return 2;
    }
    const std::filesystem::path program = fs::from_text(programs.front());
    const std::filesystem::path outdir = out ? fs::from_text(*out) : env.state / "ne_source" / text::lower(fs::to_text(program.stem()));
    Options options{.nasm = env.nasm, .stub = SearchStub{}, .find_stub = env.find_stub, .say = say};
    if (keep_stub) options.stub = KeepStub{};
    const flat_rounds::ReportText report = flat_rounds::report(program, reconstruct(program, outdir, options).report());
    say(text::rtrim(report.text));
    return report.exit_code;
}

}  // namespace cb::source::ne_writer
