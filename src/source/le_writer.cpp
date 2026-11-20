#include "source/le_writer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <iterator>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "source/assembly.hpp"
#include "source/image_writer.hpp"

namespace cb::source::le_writer {
namespace {

namespace le = formats::le;
namespace l2 = translate::l2;
namespace xf = x86_flat;

constexpr std::string_view kReader = "le_writer";
constexpr std::uint32_t kObject32BitFlag = 0x2000;

// A flow ends at one of these.
bool ends_flow(std::string_view m) {
    return m == "ret" || m == "retf" || m == "jmp" || m == "ljmp" || m == "hlt" || m == "iret" || m == "iretd" || m == "ud2";
}

// How many bytes a fix-up patches, by its source type (a 32-bit offset where the type is none of these).
std::uint32_t patched_width(std::uint8_t source) {
    switch (source) {
    case 0: return 1;
    case 2: return 2;
    case 3: return 4;
    case 5: return 2;
    case 6: return 6;
    case 7: return 4;
    case 8: return 4;
    default: return 4;
    }
}

// The macros that write a register-to-register instruction in the direction NASM does not. Width markers: 32-bit register 4, 16-bit 2,
// byte 0; the operand-size prefix is wanted where the register's width is not the segment's.
const std::vector<std::string>& prelude() {
    static const std::vector<std::string> lines = [] {
        struct Reg {
            std::string_view name;
            unsigned number, width;
        };
        std::vector<Reg> regs;
        const std::array<std::string_view, 8> wide = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
        const std::array<std::string_view, 8> word = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
        const std::array<std::string_view, 8> byte = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
        for (unsigned k = 0; k < 8; ++k) {
            regs.push_back({wide[k], k, 4});
            regs.push_back({word[k], k, 2});
            regs.push_back({byte[k], k, 0});
        }
        std::ranges::sort(regs, {}, &Reg::name);
        std::vector<std::string> p = {"", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
                                      "; Microsoft's tools write; NASM itself writes the other"};
        for (const Reg& r : regs) p.push_back(std::format("%assign _r_{} {}", r.name, r.number));
        for (const Reg& r : regs) p.push_back(std::format("%assign _w_{} {}", r.name, r.width));
        for (const char* line : {"%macro _rr 3", "  %if (_w_%2 == 2 && __?BITS?__ == 32) || (_w_%2 == 4 && __?BITS?__ == 16)", "    db 0x66", "  %endif",
                                 "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3", "%endmacro"}) {
            p.emplace_back(line);
        }
        for (const xf::RegisterPairOp& op : xf::register_pair_ops()) p.push_back(std::format("%define {}_ _rr 0x{:02x},", op.mnemonic, op.opcode));
        return p;
    }();
    return lines;
}

std::string indented(const std::string& line) { return "    " + line; }

std::uint32_t u32_at(ByteView data, std::size_t offset) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    return r.u32();
}

// A little-endian number of up to eight bytes.
std::uint64_t little_endian(ByteView b) {
    std::uint64_t v = 0;
    for (std::size_t k = b.size(); k-- > 0;) v = (v << 8) | b[k];
    return v;
}

// The place a descriptor's pointer names, where it names one.
void add_place(std::vector<Place>& out, const std::optional<le::Place>& p) {
    if (p && p->offset >= 0) out.push_back({p->object, static_cast<std::uint32_t>(p->offset)});
}

}  // namespace

std::string_view usage() {
    return R"(le_writer -- a virtual device driver (LE, and the W3 bundle VMM32.VXD) as assembly source that builds back byte for byte.

    le-source <driver> [--out DIR]

driver_writer writes a driver as the driver kit's macro source, built by wasm and wlink: that is the
driver as it was written, and it comes back the same in structure. This writes the FILE, the way
pe_writer and ne_writer do: one NASM source assembled flat (-f bin). The headers and tables are a
section of their own; each object is a section at its first page's place in the file (start=) whose
addresses are the object's offsets (vstart=0), so a label is what a fix-up names. Every fix-up record's
place is written as its label less the start of its page, every internal target as the label it names,
every 32-bit offset the file stores at a fix-up's place (the target, as LINK386 and the driver linker
leave it) as that label, and the fix-up page table as the distances of each page's records from the
first. An object whose pages are not in the file in order, or not all there, is written as its pages'
bytes in the order the file has them.

Code is what is reached from the entry point, the device descriptor's control procedure, the entry
table and every internal fix-up that names a place in an executable object; an instruction is a
mnemonic where NASM gives back its bytes (checked each round from the assembler's map), its bytes
otherwise, and always its bytes when a fix-up patches a place inside it (that place labelled).
A W3 bundle (VMM32.VXD) is its header and each of its drivers, each written as above, its labels named
by the driver.
)";
}

// ---- one driver ------------------------------------------------------------------------------------------------------------------

Driver::Driver(std::shared_ptr<const Bytes> data, std::uint64_t lfa, std::string tag, platforms::DecoderSet& decoders)
    : data_(data), module_(le::Module::parse(data, lfa)), tag_(std::move(tag)) {
    const le::Header& h = module_.header();
    const std::vector<le::PageEntry> pages = module_.page_map();
    for (const le::PageEntry& e : pages) last_page_ = std::max(last_page_, e.number);
    for (const le::Object& o : module_.objects()) {
        Object g;
        g.object = o;
        // its pages in the page map; it is whole when all are in the file, one after the other
        std::vector<std::uint32_t> numbers;
        bool all_present = true;
        const std::size_t first = o.first_page == 0 ? pages.size() : o.first_page - 1;
        for (std::size_t i = first; i < pages.size() && i < first + o.page_count; ++i) {
            numbers.push_back(pages[i].number);
            if (!(pages[i].kind == le::kPagePresent && pages[i].number != 0)) all_present = false;
        }
        bool consecutive = !numbers.empty();
        for (std::size_t q = 0; q < numbers.size(); ++q) {
            if (numbers[q] != numbers[0] + q) consecutive = false;
        }
        g.whole = consecutive && all_present;
        if (g.whole) {
            g.start = static_cast<std::size_t>(h.data_pages) + static_cast<std::size_t>(numbers[0] - 1) * h.page_size;
            for (const std::uint32_t n : numbers) g.size += page_length(n);
        }
        g.bits = (o.flags & kObject32BitFlag) ? 32 : 16;
        object_at_[o.index] = objects_.size();
        objects_.push_back(g);
    }
    // fix-ups: the place each patches, (object, offset) -> (record, width); the first record to name a place keeps it
    records_ = module_.fixup_records();
    for (std::size_t k = 0; k < records_.size(); ++k) {
        const le::FixupRecord& rec = records_[k];
        const auto where = module_.page_to_object(rec.page);
        if (!where) continue;
        for (const std::int16_t s : rec.sources) {
            const std::int64_t offset = static_cast<std::int64_t>(where->second) + s;
            if (offset < 0 || offset > 0xFFFFFFFFll) continue;   // (a fix-up that starts before its object has no place to be written at)
            const Place key{where->first->index, static_cast<std::uint32_t>(offset)};
            if (sites_.try_emplace(key, Site{k, patched_width(rec.source)}).second) site_order_.push_back(key);
        }
    }
    decode_code(decoders);
}

std::size_t Driver::page_length(std::uint32_t page) const {
    const le::Header& h = module_.header();
    return page == last_page_ && h.last_page_bytes != 0 ? h.last_page_bytes : h.page_size;
}

std::string Driver::name(Place at) const {
    return insns_.contains(at) ? std::format("{}o{}_L{:x}", tag_, at.object, at.offset) : std::format("{}o{}_D{:x}", tag_, at.object, at.offset);
}

void Driver::decode_code(platforms::DecoderSet& decoders) {
    const le::Header& h = module_.header();
    std::vector<Place> seeds;
    if (has_object(h.eip_object)) seeds.push_back({h.eip_object, h.eip});
    try {   // (a damaged table gives no entry points; the file is still written whole)
        for (const le::EntryPoint& e : module_.entries()) {
            if (has_object(e.object)) seeds.push_back({e.object, e.offset});
        }
    } catch (const Error&) {
    }
    try {
        if (const std::optional<le::Descriptor> ddb = module_.descriptor()) add_place(seeds, ddb->at_control);
    } catch (const Error&) {
    }
    const std::vector<Place> entries = seeds;   // (where the system enters: each a function)
    for (const Place& key : site_order_) {
        const le::FixupRecord& rec = records_[sites_.at(key).record];
        if (rec.kind == le::FixupRecord::Kind::Internal && rec.target && has_object(rec.first)) seeds.push_back({rec.first, *rec.target});
    }
    std::map<Place, std::uint32_t> site_bytes;   // each byte a fix-up patches -> where its place starts
    for (const auto& [at, site] : sites_) {
        for (std::uint32_t j = 0; j < site.width; ++j) site_bytes[{at.object, at.offset + j}] = at.offset;
    }
    std::vector<Place> work;   // (taken from the end)
    for (const Place& p : seeds) {
        if (has_object(p.object) && object(p.object).object.executable() && object(p.object).whole) work.push_back(p);
    }
    while (!work.empty()) {
        const Place start = work.back();
        work.pop_back();
        const Object& o = object(start.object);
        const std::size_t limit = std::min<std::size_t>(o.object.vsize, o.size);
        const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, o.bits == 32 ? 32 : 16, true);
        std::uint32_t a = start.offset;
        while (a < limit && !insns_.contains({start.object, a}) && !covered_.contains({start.object, a})) {
            const std::size_t from = o.start + a, to = o.start + std::min<std::size_t>(a + 15, limit);
            const ByteView all = *data_;
            const ByteView window = from >= all.size() ? ByteView() : all.subspan(from, std::min(to, all.size()) - from);
            const std::optional<xf::Insn> one = xf::decode(decoder, window, a);
            if (!one) break;
            bool overlap = false;
            for (std::uint32_t k = 0; k < one->size && !overlap; ++k) overlap = covered_.contains({start.object, a + k});
            if (overlap) break;
            std::set<std::uint32_t> starts_in;
            for (std::uint32_t k = 0; k < one->size; ++k) {
                if (const auto hit = site_bytes.find({start.object, a + k}); hit != site_bytes.end()) starts_in.insert(hit->second);
            }
            bool cut = false;
            for (const std::uint32_t st : starts_in) {
                if (st < a || st + sites_.at({start.object, st}).width > a + one->size) cut = true;
            }
            if (cut) break;
            insns_[{start.object, a}] = *one;
            for (std::uint32_t k = 0; k < one->size; ++k) covered_[{start.object, a + k}] = a;
            const std::optional<std::uint32_t> target = xf::branch_target(*one);
            if (target && starts_in.empty()) work.push_back({start.object, *target});
            if (ends_flow(one->mnemonic)) break;
            a += one->size;
        }
    }
    starts_ = find_function_starts(entries);
}

// Where each function begins, for the fragment each is in the file's L2 tree: the entry points and every place a call lands -- a near call
// inside an object, or one into another object through a fix-up -- where an instruction was found.
std::set<Place> Driver::find_function_starts(const std::vector<Place>& entries) const {
    std::set<Place> out(entries.begin(), entries.end());
    try {   // (and the services, the API procedures)
        if (const std::optional<le::Descriptor> d = module_.descriptor()) {
            std::vector<Place> procedures;
            add_place(procedures, d->at_control);
            add_place(procedures, d->at_v86_api);
            add_place(procedures, d->at_pm_api);
            for (const std::optional<le::Place>& p : module_.service_table(*d)) add_place(procedures, p);
            out.insert(procedures.begin(), procedures.end());
        }
    } catch (const Error&) {
    }
    for (const auto& [at, insn] : insns_) {
        const std::optional<std::uint32_t> target = xf::branch_target(insn);
        if (insn.mnemonic == "call" && target) out.insert({at.object, *target});
    }
    for (const auto& [at, site] : sites_) {
        const auto start = covered_.find(at);
        const le::FixupRecord& rec = records_[site.record];
        if (rec.kind == le::FixupRecord::Kind::Internal && rec.target && start != covered_.end() &&
            static_cast<std::int64_t>(start->second) == static_cast<std::int64_t>(at.offset) - 1 &&
            insns_.at({at.object, start->second}).mnemonic == "call") {
            out.insert({rec.first, *rec.target});
        }
    }
    std::set<Place> found;
    for (const Place& p : out) {
        if (insns_.contains(p)) found.insert(p);
    }
    return found;
}

// ---- the file --------------------------------------------------------------------------------------------------------------------

Image::Image(Bytes file, std::string file_name) : data_(std::make_shared<const Bytes>(std::move(file))), file_name_(std::move(file_name)) {
    const std::uint64_t lfa = u32_at(*data_, 0x3C);
    bundle_ = lfa + 2 <= data_->size() && (*data_)[lfa] == 'W' && (*data_)[lfa + 1] == '3';
    if (bundle_) {
        const std::optional<le::Container> bundle = le::container(*data_);
        if (!bundle || bundle->kind != le::Container::Kind::W3) throw FormatError(kReader, lfa, "not a W3 container");
        for (std::size_t k = 0; k < bundle->members.size(); ++k) {
            const le::ContainerMember& m = bundle->members[k];
            const ByteView sig = bytes(m.offset, m.offset + 2);
            if (sig.size() == 2 && sig[0] == 'L' && (sig[1] == 'E' || sig[1] == 'X')) drivers_.emplace_back(data_, m.offset, std::format("m{}_", k), decoders_);
        }
    } else {
        drivers_.emplace_back(data_, lfa, "", decoders_);
    }
}

Image Image::open(const std::filesystem::path& file) { return Image(fs::read_file(file), fs::to_text(file.filename())); }

ByteView Image::bytes(std::size_t lo, std::size_t hi) const {
    // (the file may end inside what a table says: what is there is all there is, as the original writer's slices were)
    const ByteView all = data();
    lo = std::min(lo, all.size());
    hi = std::min(std::max(hi, lo), all.size());
    return all.subspan(lo, hi - lo);
}

std::size_t Image::instruction_count() const {
    std::size_t n = 0;
    for (const Driver& d : drivers_) n += d.insns_.size();
    return n;
}

// The places that need a label: what fix-ups patch and name, the branch targets, and with `mark_all` every instruction.
void Image::place_labels(Driver& dr, bool mark_all) {
    std::set<Place> want;
    for (const auto& [at, site] : dr.sites_) {
        const le::FixupRecord& rec = dr.records_[site.record];
        want.insert(at);
        if (rec.kind == le::FixupRecord::Kind::Internal && rec.target) want.insert({rec.first, *rec.target});
    }
    for (const auto& [at, insn] : dr.insns_) {
        if (const auto target = xf::branch_target(insn)) want.insert({at.object, *target});
    }
    if (mark_all) {
        for (const auto& entry : dr.insns_) want.insert(entry.first);
    }
    dr.placed_.clear();
    for (const Place& key : want) {
        if (!dr.has_object(key.object)) continue;
        const Object& o = dr.object(key.object);
        if (!o.whole || key.offset > o.size) continue;
        if (dr.insns_.contains(key) || !dr.covered_.contains(key)) dr.placed_[key] = dr.name(key);
    }
    for (const Place& key : want) {
        const auto start = dr.covered_.find(key);
        if (start == dr.covered_.end() || dr.insns_.contains(key) || !dr.sites_.contains(key)) continue;
        dr.placed_[key] = std::format("{}o{}_S{:x}", dr.tag_, key.object, key.offset);
        const Place first{key.object, start->second};
        dr.placed_.try_emplace(first, dr.name(first));
    }
}

std::string Image::ref(const Driver& dr, std::uint32_t object, std::uint32_t offset) const {
    if (const auto p = dr.placed_.find({object, offset}); p != dr.placed_.end()) return p->second;
    if (const auto start = dr.covered_.find({object, offset}); start != dr.covered_.end()) {
        if (const auto first = dr.placed_.find({object, start->second}); first != dr.placed_.end()) {
            return std::format("{}+{}", first->second, offset - start->second);
        }
    }
    if (dr.has_object(object) && dr.object(object).whole) return std::format("{}o{}_start+0x{:x}", dr.tag_, object, offset);
    return std::format("0x{:x}", offset);
}

flat_rounds::Source<Key> Image::write(const std::set<Key>& raw, bool final) { return source(raw, !final, final); }

flat_rounds::Source<Key> Image::source(const std::set<Key>& raw, bool mark_all, bool exact) {
    for (Driver& d : drivers_) place_labels(d, mark_all);

    enum class Kind { Object, FixupRecords, FixupPages };
    struct Region {
        std::size_t lo, hi;
        Kind kind;
        std::size_t driver;
        const Object* object;
        std::vector<std::uint32_t> pages;   // the fix-up page table
    };
    std::vector<Region> regions;   // (file offset, end, what)
    for (std::size_t di = 0; di < drivers_.size(); ++di) {
        const Driver& dr = drivers_[di];
        for (const Object& o : dr.objects_) {
            if (o.whole && o.size != 0) regions.push_back({o.start, o.start + o.size, Kind::Object, di, &o, {}});
        }
        const le::Header& h = dr.module_.header();
        const std::size_t fixup_page = dr.module_.lfa() + h.fixup_pages, fixup_rec = dr.module_.lfa() + h.fixup_records;
        std::vector<std::uint32_t> table;
        for (std::uint32_t i = 0; i < h.pages + 1; ++i) table.push_back(u32_at(data(), fixup_page + 4 * static_cast<std::size_t>(i)));
        if (table.back() != 0) {
            regions.push_back({fixup_rec, fixup_rec + table.back(), Kind::FixupRecords, di, nullptr, table});
            regions.push_back({fixup_page, fixup_page + 4 * (static_cast<std::size_t>(h.pages) + 1), Kind::FixupPages, di, nullptr, table});
        }
    }
    std::ranges::stable_sort(regions, {}, &Region::lo);
    regions.push_back({data().size(), data().size(), Kind::Object, 0, nullptr, {}});   // (the end of the file)

    l2::Assembly out({}, "nasm", file_name_);   // (the file as one L2 tree, as it is written)
    std::map<std::size_t, Key> lines_of;
    out.append(std::format("; {} -- assembled with NASM: nasm -f bin -o {} this-file", file_name_, file_name_));
    out.extend(prelude());
    for (const Driver& dr : drivers_) {
        for (const Object& o : dr.objects_) {
            if (o.whole && o.size != 0) out.append(std::format("{}o{}_file equ 0x{:x}", dr.tag_, o.object.index, o.start));
        }
    }
    std::size_t pos = 0, n = 0;
    std::optional<std::string> prev;
    const auto place_of_section = [&](const std::string& start) { return exact || !prev ? start : "follows=" + *prev; };
    for (std::size_t r = 0; r < regions.size(); ++r) {
        const Region& rg = regions[r];
        const bool end_of_file = r + 1 == regions.size();
        if (rg.lo < pos) continue;   // (overlapping tables: written with what they overlap)
        if (rg.lo > pos) {
            out.part(std::format("part{}", n), "outside the objects", std::nullopt, rg.lo - pos);
            out.append("");
            out.append(std::format("section part{} {} vstart=0x{:x} align=1", n, place_of_section(std::format("start=0x{:x}", pos)), pos));
            prev = std::format("part{}", n);
            for (const std::string& x : assembly::data_lines(bytes(pos, rg.lo))) out.append(indented(x));
            ++n;
        }
        if (end_of_file) break;
        const Driver& dr = drivers_[rg.driver];
        if (rg.kind == Kind::Object) {
            out.part(std::format("{}o{}", dr.tag_, rg.object->object.index), "object",
                     translate::ir::OriginSegment{file_name_, rg.object->object.index, 0}, rg.object->size);
        } else {
            out.part(dr.tag_ + (rg.kind == Kind::FixupRecords ? "fixrec" : "fixpage"), "fixups", std::nullopt, rg.hi - rg.lo);
        }
        out.append("");
        if (rg.kind == Kind::Object) {
            const Object& o = *rg.object;
            const std::string section = std::format("{}o{}", dr.tag_, o.object.index);
            out.append(std::format("section {} {} vstart=0 align=1", section, place_of_section(std::format("start={}_file", section))));
            prev = section;
            out.append(std::format("bits {}", o.bits));
            out.append(section + "_start:");
            write_object(out, rg.driver, o, raw, lines_of);
        } else if (rg.kind == Kind::FixupRecords) {
            const std::string section = dr.tag_ + "fixrec";
            out.append(std::format("section {} {} vstart=0 align=1", section, place_of_section(std::format("start=0x{:x}", rg.lo))));
            prev = section;
            write_fixup_records(out, dr);
        } else {
            const std::string section = dr.tag_ + "fixpage";
            out.append(std::format("section {} {} vstart=0 align=1", section, place_of_section(std::format("start=0x{:x}", rg.lo))));
            prev = section;
            for (std::size_t k = 0; k < rg.pages.size(); ++k) out.append(std::format("    dd {}fr_p{}-{}fr_p0", dr.tag_, k, dr.tag_));
        }
        pos = rg.hi;
    }
    out.append("");
    assembly_ = out;
    return {image_writer::text(out), std::move(lines_of)};
}

void Image::write_object(l2::Assembly& out, std::size_t driver_index, const Object& o, const std::set<Key>& raw, std::map<std::size_t, Key>& lines_of) const {
    const Driver& dr = drivers_[driver_index];
    const std::uint32_t oi = o.object.index;
    const std::size_t base = o.start, size = o.size;
    std::vector<std::uint32_t> mine;   // the function starts in this object, ascending
    for (const Place& p : dr.starts_) {
        if (p.object == oi && p.offset < size) mine.push_back(p.offset);
    }
    std::map<std::uint32_t, std::size_t> ends;   // (a function runs to the next one)
    for (std::size_t k = 0; k < mine.size(); ++k) ends[mine[k]] = k + 1 < mine.size() ? mine[k + 1] : size;
    std::size_t a = 0;
    while (a < size) {
        const Place key{oi, static_cast<std::uint32_t>(a)};
        if (const auto found = dr.insns_.find(key); found != dr.insns_.end()) {
            const x86_flat::Insn& insn = found->second;
            const auto name = dr.placed_.find(key);
            if (const auto fe = ends.find(key.offset); fe != ends.end()) {   // (each function a fragment of its own)
                out.part(name != dr.placed_.end() && !name->second.empty() ? name->second : dr.name(key), "function",
                         translate::ir::OriginSegment{file_name_, oi, static_cast<std::int64_t>(a)}, fe->second - a);
            }
            if (name != dr.placed_.end()) out.append(name->second + ":");
            const Key whole{static_cast<std::uint32_t>(driver_index), key};
            for (const std::string& t : text(dr, oi, insn, raw.contains(whole), o.bits)) {
                lines_of[out.lines().size() + 1] = whole;
                out.append(t.ends_with(':') ? t : indented(t));
            }
            a += insn.size;
            continue;
        }
        std::size_t stop = a + 1;
        while (stop < size && !dr.insns_.contains({oi, static_cast<std::uint32_t>(stop)})) ++stop;
        write_data(out, dr, oi, base, a, stop);
        a = stop;
    }
    if (const auto end = dr.placed_.find({oi, static_cast<std::uint32_t>(size)}); end != dr.placed_.end()) out.append(end->second + ":");
    // the places past the end that something names (the object's virtual size is longer than its bytes)
    std::vector<std::uint32_t> over;
    for (const auto& entry : dr.placed_) {
        if (entry.first.object == oi && entry.first.offset > size) over.push_back(entry.first.offset);
    }
    for (const std::uint32_t v : over) out.append(std::format("{} equ $+0x{:x}", dr.placed_.at({oi, v}), v - size));
}

// How the bytes at a fix-up's place are written: the label of the target the file holds there.
std::string Image::site_value(const Driver& dr, const le::FixupRecord& rec, std::uint32_t width, ByteView b) const {
    if (rec.kind == le::FixupRecord::Kind::Internal && rec.source == 7 && width == 4 && rec.target && b.size() >= 4 && dr.has_object(rec.first)) {
        if (little_endian(b.first(4)) == *rec.target) return "dd " + ref(dr, rec.first, *rec.target);
    }
    return "db " + xf::db_bytes(b, 0, b.size());
}

// An instruction's lines: its mnemonic in NASM's spelling, or its bytes with the fixed-up places and branch distances as expressions (`raw`).
std::vector<std::string> Image::text(const Driver& dr, std::uint32_t oi, const xf::Insn& insn, bool raw, unsigned bits) const {
    const std::uint32_t a = insn.address;
    const ByteView b = insn.raw();
    const std::optional<std::uint32_t> target = xf::branch_target(insn);
    std::vector<std::uint32_t> inside;   // the offsets in the instruction where a fix-up patches
    for (std::uint32_t k = 0; k < insn.size; ++k) {
        if (dr.sites_.contains({oi, a + k})) inside.push_back(k);
    }
    if (!raw && inside.empty()) {
        const auto spell = [&](std::uint32_t v) { return ref(dr, oi, v); };
        if (const std::optional<std::string> t = nasm_text(insn, bits, spell); t && !t->empty()) return {*t};
    }
    std::vector<std::string> out;
    std::uint32_t k = 0;
    const std::uint32_t n = insn.size;
    while (k < n) {
        if (std::ranges::find(inside, k) != inside.end()) {
            const Site& site = dr.sites_.at({oi, a + k});
            if (k != 0) out.push_back(dr.placed_.at({oi, a + k}) + ":");
            out.push_back(site_value(dr, dr.records_[site.record], site.width, b.subspan(k, std::min<std::size_t>(site.width, n - k))));
            k += site.width;
        } else if (target && k == insn.imm_offset) {
            CB_ASSERT(insn.imm_size == 4 || insn.imm_size == 2 || insn.imm_size == 1, "a branch's displacement is 1, 2 or 4 bytes");
            const char* directive = insn.imm_size == 4 ? "dd" : insn.imm_size == 2 ? "dw" : "db";
            out.push_back(std::format("{} {}-($+{})", directive, ref(dr, oi, *target), insn.imm_size));
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
std::optional<std::string> Image::nasm_text(const xf::Insn& insn, unsigned bits, const std::function<std::string(std::uint32_t)>& ref) const {
    static const Regex nosplit(R"(\[(\w\w:)?(e\w\w\*[248]))");
    static const Regex bracket(R"(\[([^\]]*)\])");
    static const Regex base_and_index(R"([a-z]{2,3}\s*[+-])");
    const std::string& m = insn.mnemonic;
    if (m == "ljmp" || m == "lcall") return std::nullopt;
    if (xf::is_string_op(m, true)) {
        for (const std::uint8_t p : insn.prefix) {
            if (xf::is_segment_prefix(p)) return std::nullopt;
        }
        return m;
    }
    if (const std::optional<std::uint32_t> target = xf::branch_target(insn)) {
        const unsigned near = bits == 32 ? 4 : 2;
        if (m == "call") {
            if (insn.imm_size == near && insn.size == 1 + near) return "call " + ref(*target);
            return std::nullopt;
        }
        if (m == "loop" || m == "loope" || m == "loopne" || m == "jcxz" || m == "jecxz") {
            if (insn.size == 2) return m + " " + ref(*target);
            return std::nullopt;
        }
        if ((insn.imm_size != 1 && insn.imm_size != near) || (insn.imm_size == 1 && insn.size != 2)) return std::nullopt;
        return std::format("{} {} {}", m, insn.imm_size == 1 ? "short" : "near", ref(*target));
    }
    if (const std::optional<std::string> pair = xf::register_pair_text(insn)) return pair;
    std::string ops = xf::nasm_operands(insn.op_str);
    if (bits == 32) ops = nosplit.replace_all(ops, "[$1nosplit $2");
    if ((insn.imm_size == 2 || insn.imm_size == 4) && xf::is_arith(m)) {
        if (const auto n = xf::trailing_number(ops)) {
            const std::int64_t long_form_from = insn.imm_size == 2 ? 0xFF80 : 0xFFFFFF80;
            if ((-128 <= n->value && n->value <= 127) || n->value >= long_form_from) ops.insert(n->start, insn.imm_size == 2 ? "strict word " : "strict dword ");
        }
    }
    if (insn.disp_size == 2 || insn.disp_size == 4) {
        if (const auto br = bracket.search(ops); br && base_and_index.test(br->group(1))) {
            const std::string inner(br->group(1));
            ops = ops.substr(0, br->start()) + std::format("[{} ", insn.disp_size == 4 ? "dword" : "word") + inner + "]" + ops.substr(br->end());
        }
    }
    return std::string(text::trim(m + " " + ops));
}

void Image::write_data(l2::Assembly& out, const Driver& dr, std::uint32_t oi, std::size_t base, std::size_t from, std::size_t to) const {
    std::set<std::size_t> cut_set;
    const auto in_range = [&](const Place& k) { return k.object == oi && from <= k.offset && k.offset < to; };
    for (const auto& entry : dr.placed_) {
        if (in_range(entry.first)) cut_set.insert(entry.first.offset);
    }
    for (const auto& entry : dr.sites_) {
        if (in_range(entry.first)) cut_set.insert(entry.first.offset);
    }
    std::vector<std::size_t> cuts(cut_set.begin(), cut_set.end());
    cuts.push_back(to);
    std::size_t pos = from;
    for (const std::size_t c : cuts) {
        const Place key{oi, static_cast<std::uint32_t>(c)};
        const auto name = dr.placed_.find(key);
        if (c < pos) {   // (inside the place just written)
            if (name != dr.placed_.end()) out.append(std::format("{} equ $-{}", name->second, pos - c));
            continue;
        }
        if (c > pos) {
            for (const std::string& x : assembly::data_lines(bytes(base + pos, base + c))) out.append(indented(x));
            pos = c;
        }
        if (c == to) break;
        if (name != dr.placed_.end()) out.append(name->second + ":");
        const auto site = dr.sites_.find(key);
        if (site == dr.sites_.end() || c + site->second.width > to) continue;
        out.append(indented(site_value(dr, dr.records_[site->second.record], site->second.width, bytes(base + c, base + c + site->second.width))));
        pos = c + site->second.width;
    }
}

// The fix-up records, page by page: each place as its label less its page's start, each internal target as its label.
void Image::write_fixup_records(l2::Assembly& out, const Driver& dr) const {
    const le::Header& h = dr.module_.header();
    std::map<std::uint32_t, std::vector<std::size_t>> by_page;   // page -> indices of its records, in file order
    for (std::size_t k = 0; k < dr.records_.size(); ++k) by_page[dr.records_[k].page].push_back(k);
    for (std::uint32_t pg = 0; pg < h.pages + 1; ++pg) {
        out.append(std::format("{}fr_p{}:", dr.tag_, pg));
        if (pg == h.pages) break;
        const auto where = dr.module_.page_to_object(pg + 1);
        const Object* obj = where ? &dr.object(where->first->index) : nullptr;
        const std::uint64_t page_at = where ? where->second : 0;
        const auto records = by_page.find(pg + 1);
        if (records == by_page.end()) continue;
        for (const std::size_t ri : records->second) {
            const le::FixupRecord& rec = dr.records_[ri];
            const ByteView raw = rec.raw;
            const auto byte = [&](std::size_t i) -> std::uint8_t {
                if (i >= raw.size()) throw FormatError(kReader, i, "a fix-up record is shorter than its flags say");
                return raw[i];
            };
            const unsigned src = byte(0), fl = byte(1);
            const bool many = (src & 0x20) != 0;
            std::vector<std::string> parts{std::format("db 0x{:02x},0x{:02x}", src, fl)};
            const auto source_text = [&](std::int64_t s) -> std::string {
                if (obj != nullptr && obj->whole) {
                    const std::int64_t at = static_cast<std::int64_t>(page_at) + s;
                    if (at >= 0) {
                        if (const auto label = dr.placed_.find({obj->object.index, static_cast<std::uint32_t>(at)}); label != dr.placed_.end() && !label->second.empty()) {
                            return std::format("dw {}-0x{:x}", label->second, page_at);
                        }
                    }
                }
                return std::format("dw 0x{:04x}", s & 0xFFFF);
            };
            std::size_t p = 2;
            if (many) {
                parts.push_back(std::format("db {}", byte(2)));
                p = 3;
            } else {
                parts.push_back(source_text(rec.sources.at(0)));
                p = 4;
            }
            const std::size_t wide = (fl & 0x40) ? 2 : 1;
            const std::uint64_t value = little_endian(raw.subspan(std::min(p, raw.size()), std::min(wide, raw.size() - std::min(p, raw.size()))));
            parts.push_back(wide == 2 ? std::format("dw 0x{:04x}", value) : std::format("db 0x{:02x}", value));
            p += wide;
            ByteView rest = raw.subspan(std::min(p, raw.size()));
            if (rec.kind == le::FixupRecord::Kind::Internal && rec.target && dr.has_object(rec.first) && dr.object(rec.first).whole) {
                const std::size_t width = (fl & 0x10) ? 4 : 2;
                parts.push_back(std::format("{} {}", width == 4 ? "dd" : "dw", ref(dr, rec.first, *rec.target)));
                rest = rest.subspan(std::min(width, rest.size()));
            }
            if (many) {
                const std::size_t tail = 2 * rec.sources.size();
                const ByteView mid = rest.first(rest.size() > tail ? rest.size() - tail : 0);
                if (!mid.empty()) parts.push_back("db " + xf::db_bytes(mid, 0, mid.size()));
                for (const std::int16_t s : rec.sources) parts.push_back(source_text(s));
            } else if (!rest.empty()) {
                parts.push_back("db " + xf::db_bytes(rest, 0, rest.size()));
            }
            for (const std::string& x : parts) out.append(indented(x));
        }
    }
}

// The instructions the assembler did not give back: compared where each one went, its length (to the next instruction's label) and its
// bytes, outside the places whose value depends on where things are (fix-up targets, branch distances).
std::set<Key> Image::differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Key>& raw) {
    std::set<Key> bad;
    for (std::size_t di = 0; di < drivers_.size(); ++di) {
        const Driver& dr = drivers_[di];
        const auto label_of = [&](const Place& p) -> const flat_rounds::Label* {
            const auto name = dr.placed_.find(p);
            if (name == dr.placed_.end()) return nullptr;
            const auto hit = labels.find(name->second);
            return hit == labels.end() ? nullptr : &hit->second;
        };
        for (auto it = dr.insns_.begin(); it != dr.insns_.end(); ++it) {
            const Place at = it->first;
            const xf::Insn& insn = it->second;
            const Key key{static_cast<std::uint32_t>(di), at};
            if (raw.contains(key)) continue;
            const flat_rounds::Label* here = label_of(at);
            if (here == nullptr) {
                bad.insert(key);
                continue;
            }
            if (const auto next = std::next(it); next != dr.insns_.end() && next->first.object == at.object && next->first.offset == at.offset + insn.size) {
                const flat_rounds::Label* there = label_of(next->first);
                if (there != nullptr && static_cast<std::int64_t>(there->address) - static_cast<std::int64_t>(here->address) != insn.size) {
                    bad.insert(key);
                    continue;
                }
            }
            const std::size_t real = std::min<std::size_t>(here->place, built.size());
            const ByteView mine = built.subspan(real, std::min<std::size_t>(insn.size, built.size() - real));
            const std::size_t original_at = dr.object(at.object).start + at.offset;
            const ByteView original = bytes(original_at, original_at + insn.size);
            const bool relative = xf::branch_target(insn).has_value();   // (its displacement depends on where things are)
            bool differs = mine.size() != original.size();
            for (std::size_t k = 0; !differs && k < original.size(); ++k) {
                const bool skipped = relative && k >= insn.imm_offset && k < static_cast<std::size_t>(insn.imm_offset) + insn.imm_size;
                if (!skipped && mine[k] != original[k]) differs = true;
            }
            if (differs) bad.insert(key);
        }
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

Result reconstruct(const std::filesystem::path& driver, const std::filesystem::path& outdir_, const Options& options) {
    const std::function<void(std::string_view)> say = options.say ? options.say : [](std::string_view) {};
    const std::filesystem::path outdir = std::filesystem::absolute(outdir_);
    fs::make_dirs(outdir);
    Image image = Image::open(driver);
    const std::string name = text::lower(fs::to_text(driver.filename()));
    // (the assembler rejecting only what is already bytes ends the rounds: new_only)
    const flat_rounds::FlatResult<Key> fr = flat_rounds::flat<Key>(image.data(), image, outdir, name, {.new_only = true, .say = say});
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = image.instruction_count();
    r.raw = fr.raw.size();
    return r;
}

int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say) {
    std::vector<std::string> drivers;
    std::optional<std::string> out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out") {
            if (i + 1 >= args.size()) throw UsageError("--out needs a folder");
            out = args[++i];
        } else if (!args[i].starts_with("--")) {
            drivers.push_back(args[i]);
        }
    }
    if (drivers.empty()) {
        say(usage());
        return 2;
    }
    const std::filesystem::path driver = fs::from_text(drivers.front());
    const std::filesystem::path outdir = out ? fs::from_text(*out) : env.state / "le_source" / text::lower(fs::to_text(driver.stem()));
    const flat_rounds::ReportText report = flat_rounds::report(driver, reconstruct(driver, outdir, {.say = say}).report());
    say(text::rtrim(report.text));
    return report.exit_code;
}

}  // namespace cb::source::le_writer
