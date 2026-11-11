#include "cb/exact/linkemu.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <optional>
#include <set>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::linkemu {
namespace {

constexpr std::string_view kTool = "link24";
constexpr std::size_t kRunFileHeaderBytes = 30;          // the MZ header proper, before the relocation table
constexpr std::size_t kHeaderPad = 512;                  // the header is padded to a multiple of this
constexpr std::uint32_t kParagraphShift = 4;

[[noreturn]] void fail(const std::string& what) { throw ToolError(kTool, 1, what, what); }

// The OMF alignment code as bytes (0 and 1 byte, 2 word, 3 paragraph, 4 page, 5 dword); an unknown code is byte.
std::uint32_t align_bytes(std::uint32_t code) {
    switch (code) {
        case 2: return 2;
        case 3: return 16;
        case 4: return 256;
        case 5: return 4;
        default: return 1;
    }
}

std::uint16_t word_of(const Bytes& b, std::uint32_t offset) {
    io::ByteReader r(b, "link24 segment");
    r.seek(offset);
    return r.u16();
}

// The linker's names are without regard to case; the first spelling a module gave is the one used.
class SegmentNames {
public:
    const std::string& canonical(const std::string& s) { return spellings_.try_emplace(text::upper(s), s).first->second; }
    std::string lookup(const std::string& s) const {
        auto it = spellings_.find(text::upper(s));
        return it == spellings_.end() ? s : it->second;
    }

private:
    std::unordered_map<std::string, std::string> spellings_;
};

struct Piece {
    std::string segment;
    std::uint32_t align = 1;
    Bytes data;
    std::uint32_t length = 0;
    bool has_data = false;
    bool is_stack = false;
    std::optional<std::uint32_t> at;   // none until placed
};

enum class How { Abs, Rel, Seg };           // the address, from the end of the word, a paragraph
enum class Names { Symbol, Segment, Group };

struct Fix {
    std::string segment;
    std::uint32_t offset = 0;
    How how = How::Abs;
    Names names = Names::Group;
    std::string target;                    // a symbol (lower case) or a segment (canonical); empty for a group
    std::uint16_t addend = 0;
};

struct Module {
    std::string name;
    OrderedMap<std::string, Piece> pieces;
    OrderedMap<std::string, std::pair<std::string, std::uint32_t>> defined;   // lower-case name -> (segment, offset), in the order the object lists them
    OrderedMap<std::string, std::uint32_t> common;                          // lower-case name -> length
    std::vector<Fix> fixes;
    std::optional<std::pair<std::string, std::uint32_t>> start;
};

Module load(const LinkInput& in, SegmentNames& real) {
    const ObjectFile& o = in.object;
    const bool even = o.kind == ObjectKind::Aout;   // (a C object's pieces each begin at an even address)
    Module m;
    m.name = in.name;
    for (const SegDef& sd : o.segs) {
        const std::string& name = real.canonical(sd.name);
        Piece p;
        p.segment = name;
        p.align = align_bytes(even ? 2 : sd.align);
        const std::uint32_t filled = get_or(o.filled, sd.name, 0u);
        if (const Bytes* bytes = o.seg.find(sd.name)) p.data.assign(bytes->begin(), bytes->begin() + std::min<std::size_t>(filled, bytes->size()));
        p.length = sd.length;
        if (in.odd && text::iequals(sd.cls, "CODE") && !p.data.empty()) {
            p.data.pop_back();   // (the compiler's filler, which is not there)
            p.length = static_cast<std::uint32_t>(p.data.size());
        }
        p.has_data = filled != 0;
        const std::uint8_t* combine = o.combine.find(sd.name);
        p.is_stack = (combine != nullptr && *combine == 5) || text::iequals(sd.cls, "STACK");
        m.pieces.insert_or_assign(name, std::move(p));
    }
    for (const Public& pub : o.pubs)
        if (pub.segment) m.defined.insert_or_assign(text::lower(pub.name), std::make_pair(real.canonical(*pub.segment), pub.offset));
    for (const auto& [name, var] : o.comm) m.common.insert_or_assign(text::lower(name), var.size);

    for (const Fixup& f : o.fix) {
        const Bytes* segment_bytes = o.seg.find(f.segment);
        if (segment_bytes == nullptr) throw FormatError("link24", f.offset, "a fix-up in segment " + f.segment + ", which the object lacks");
        const std::uint16_t add = static_cast<std::uint16_t>(word_of(*segment_bytes, f.offset) + f.displacement);
        Fix x;
        x.segment = real.canonical(f.segment);
        x.offset = f.offset;
        switch (f.target.kind) {
            case TargetKind::External: x.names = Names::Symbol; x.target = text::lower(f.target.name); break;
            case TargetKind::Segment: x.names = Names::Segment; x.target = real.canonical(f.target.name); break;
            case TargetKind::Group: x.names = Names::Group; break;
        }
        switch (f.kind) {
            case FixKind::Base: x.how = How::Seg; x.addend = add; break;
            case FixKind::Rel: x.how = How::Rel; x.addend = static_cast<std::uint16_t>(add - f.offset - 2); break;
            default: x.how = How::Abs; x.addend = add; break;
        }
        m.fixes.push_back(std::move(x));
    }
    if (o.start) m.start = {real.canonical(o.start->segment), o.start->offset};
    return m;
}

std::uint32_t placed(const Module& m, const Piece& p) {
    if (!p.at) fail(std::format("segment {} of {} was never placed", p.segment, m.name));
    return *p.at;
}

Piece& piece_of(Module& m, const std::string& segment) {
    Piece* p = m.pieces.find(segment);
    if (p == nullptr) fail(std::format("{} has no segment {}", m.name, segment));
    return *p;
}

std::uint16_t as_u16(std::int64_t v, const char* what) {
    if (v < 0 || v > 0xFFFF) fail(std::format("{} ({}) does not fit a run file's header word", what, v));
    return static_cast<std::uint16_t>(v);
}

}  // namespace

LinkResult link24(const std::vector<LinkInput>& objects, const SymbolTable& symbols, const Log& log) {
    const auto say = [&](const std::string& s) {
        if (log) log(s);
    };
    SegmentNames real;
    std::vector<Module> mods;
    for (const LinkInput& in : objects) mods.push_back(load(in, real));

    // The segments: a group's in the order the group lists them (an assembler writes its SEGDEFs in the order of its
    // names, its GRPDEF as the source says), the others in the order the modules name them; code first.
    std::vector<std::string> order;
    const auto in_order = [&](const std::string& s) { return std::find(order.begin(), order.end(), s) != order.end(); };
    for (const LinkInput& in : objects)
        for (const auto& [group, members] : in.object.groups)
            for (const std::string& sg : members)
                if (const std::string name = real.lookup(sg); !in_order(name)) order.push_back(name);
    for (const Module& m : mods) {
        for (const auto& [sg, piece] : m.pieces) {
            if (in_order(sg)) continue;
            if (text::iends_with(sg, "TEXT")) order.insert(order.begin(), sg);   // (each later one before the earlier)
            else order.push_back(sg);
        }
    }
    std::vector<std::string> code;
    for (const std::string& sg : order) {
        const bool coded = text::iends_with(sg, "TEXT") && std::any_of(mods.begin(), mods.end(), [&](const Module& m) {
            const Piece* p = m.pieces.find(sg);
            return p != nullptr && p->has_data;
        });
        if (coded) code.push_back(sg);
    }
    if (code.empty() && !order.empty()) code.push_back(order.front());
    const auto is_code = [&](const std::string& sg) { return std::find(code.begin(), code.end(), sg) != code.end(); };

    // Place every module's piece of `seg` one after another from `at`; returns where the segment ends.
    const auto place = [&](const std::string& seg, std::uint32_t at) {
        for (Module& m : mods) {
            Piece* p = m.pieces.find(seg);
            if (p == nullptr) continue;
            at = (at + p->align - 1) / p->align * p->align;
            p->at = at;
            at += p->length;
        }
        return at;
    };

    std::uint32_t at = 0;
    for (const std::string& sg : code) at = place(sg, at);
    const std::uint32_t end = at;
    const std::uint32_t dgroup = (end + 15) & ~15u;
    at = dgroup;
    std::vector<std::string> rest;
    for (const std::string& sg : order)
        if (!is_code(sg)) rest.push_back(sg);
    const auto comm_it = std::find_if(rest.begin(), rest.end(), [](const std::string& s) { return text::iequals(s, "c_common"); });
    const bool has_comm_seg = comm_it != rest.end();
    const std::size_t comm_at = static_cast<std::size_t>(comm_it - rest.begin());
    for (std::size_t i = 0; i < (has_comm_seg ? comm_at : rest.size()); ++i)
        if (!text::iequals(rest[i], "STACK")) at = place(rest[i], at);

    // The communal variables: in the order of their places in SYMBOLS, one after another.
    OrderedMap<std::string, std::uint32_t> common;
    std::set<std::string> names;
    std::map<std::string, std::int64_t> known;
    for (const auto& [name, where] : symbols) known[text::lower(name)] = where.address;
    for (const Module& m : mods) {
        for (const auto& [k, v] : m.common) common.insert_or_assign(k, std::max(get_or(common, k, 0u), v));
        for (const auto& [k, where] : m.defined) {
            if (names.contains(k) && !k.starts_with('l')) fail(std::format("{} is defined twice ({})", k, m.name));
            names.insert(k);
        }
    }
    OrderedMap<std::string, std::uint32_t> wanted;
    for (const auto& [k, v] : common)
        if (!names.contains(k)) wanted.insert_or_assign(k, v);
    std::vector<std::string> missing;
    for (const auto& [k, v] : wanted)
        if (!known.contains(k)) missing.push_back(k);
    if (!missing.empty()) {
        std::sort(missing.begin(), missing.end());
        fail("communal variables SYMBOLS has no place for: " + text::join(missing, " "));
    }
    std::map<std::string, std::int64_t> gdef;
    if (has_comm_seg) {
        const std::string& comm_seg = rest[comm_at];
        Piece* p = nullptr;
        for (Module& m : mods)
            if ((p = m.pieces.find(comm_seg)) != nullptr) break;
        if (p == nullptr) fail("no module has the segment " + comm_seg);
        at = (at + p->align - 1) / p->align * p->align;
        p->at = at;
        std::vector<std::pair<std::string, std::uint32_t>> vars(wanted.begin(), wanted.end());
        std::stable_sort(vars.begin(), vars.end(), [&](const auto& a, const auto& b) { return known.at(a.first) < known.at(b.first); });
        for (const auto& [k, size] : vars) {
            if (known.at(k) != static_cast<std::int64_t>(at) - dgroup)
                say(std::format("  {}: SYMBOLS has it at {:04x}; the variables before it end at {:04x}", k, known.at(k),
                                at - dgroup));
            gdef[k] = static_cast<std::int64_t>(at) - dgroup;
            at += (size + 1) & ~1u;
        }
        for (std::size_t i = comm_at + 1; i < rest.size(); ++i)
            if (!text::iequals(rest[i], "STACK")) at = place(rest[i], at);
    }
    const auto stack_it = std::find_if(rest.begin(), rest.end(), [](const std::string& s) { return text::iequals(s, "STACK"); });
    if (stack_it != rest.end()) at = place(*stack_it, at);
    const std::uint32_t top = at;

    const auto frame_of = [&](const std::string& sg) -> std::int64_t { return is_code(sg) ? 0 : dgroup; };
    for (Module& m : mods)
        for (const auto& [k, where] : m.defined)
            gdef[k] = static_cast<std::int64_t>(placed(m, piece_of(m, where.first))) + where.second - frame_of(where.first);

    const Piece* stack = nullptr;
    for (const Module& m : mods) {
        for (const auto& [sg, p] : m.pieces)
            if (stack == nullptr && p.is_stack) stack = &p;
    }

    // The image goes as far as the last segment that has data in it does - to its end.
    std::set<std::string> with_data;
    for (const Module& m : mods)
        for (const auto& [sg, p] : m.pieces)
            if (p.has_data && p.length != 0) with_data.insert(p.segment);
    std::optional<std::uint32_t> last_data;
    for (const Module& m : mods)
        for (const auto& [sg, p] : m.pieces)
            if (with_data.contains(p.segment) && p.at) last_data = std::max(last_data.value_or(0), *p.at + p.length);
    if (!last_data) fail("no segment has data");
    Bytes image(*last_data);

    std::vector<std::uint32_t> relocs;
    std::optional<std::uint32_t> entry;
    for (Module& m : mods) {
        for (const auto& [sg, p] : m.pieces) {
            if (!p.has_data) continue;
            const std::uint32_t from = placed(m, p);
            if (from + p.data.size() > image.size()) image.resize(from + p.data.size());
            std::copy(p.data.begin(), p.data.end(), image.begin() + from);
        }
        if (m.start) entry = placed(m, piece_of(m, m.start->first)) + m.start->second;
        for (const Fix& f : m.fixes) {
            const std::uint32_t seg_at = placed(m, piece_of(m, f.segment));
            const std::uint32_t loc = seg_at + f.offset;
            std::int64_t v = 0;
            switch (f.names) {
                case Names::Symbol: {
                    auto it = gdef.find(f.target);
                    if (it == gdef.end()) fail(std::format("{}: {} is not defined", m.name, f.target));
                    v = it->second;
                    break;
                }
                case Names::Segment: v = static_cast<std::int64_t>(placed(m, piece_of(m, f.target))) - frame_of(f.target); break;
                case Names::Group: break;
            }
            switch (f.how) {
                case How::Seg: v = dgroup >> kParagraphShift; relocs.push_back(loc); break;
                case How::Rel: v = v - (static_cast<std::int64_t>(seg_at) - frame_of(f.segment)) + f.addend; break;
                case How::Abs: v += f.addend; break;
            }
            if (static_cast<std::size_t>(loc) + 2 > image.size()) fail(std::format("{}: a fix-up at {:05x} lies beyond the image", m.name, loc));
            image[loc] = static_cast<std::uint8_t>(v & 0xFF);
            image[loc + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
        }
    }

    // A place in the run file's terms: the code's by its own offset, DGROUP's by the offset from DGROUP.
    const auto far_of = [&](std::uint32_t r) {
        return r < dgroup ? Far16{.seg = 0, .off = as_u16(r, "entry offset")}
                          : Far16{.seg = static_cast<std::uint16_t>(dgroup >> 4), .off = as_u16(r - dgroup, "entry offset")};
    };
    const std::size_t reloc_bytes = kRunFileHeaderBytes + 4 * relocs.size();
    const std::size_t header_len = (reloc_bytes + kHeaderPad - 1) / kHeaderPad * kHeaderPad;
    const std::size_t size = header_len + image.size();
    const Far16 start = far_of(entry.value_or(0));
    io::ByteWriter w;
    w.u16(0x5A4D);
    w.u16(as_u16(static_cast<std::int64_t>(size & 511), "size"));
    w.u16(as_u16(static_cast<std::int64_t>((size + 511) >> 9), "size"));
    w.u16(as_u16(static_cast<std::int64_t>(relocs.size()), "relocations"));
    w.u16(as_u16(static_cast<std::int64_t>(header_len >> 4), "header paragraphs"));
    w.u16(as_u16((static_cast<std::int64_t>(top) - static_cast<std::int64_t>(image.size()) + 15) >> 4, "minimum allocation"));
    w.u16(0xFFFF);
    w.u16(stack != nullptr ? as_u16(static_cast<std::int64_t>(*stack->at >> 4), "stack segment") : 0);
    w.u16(stack != nullptr ? as_u16(stack->length, "stack size") : 0);
    w.u16(0);   // the checksum, filled in below
    w.u16(start.off);
    w.u16(start.seg);
    w.u16(static_cast<std::uint16_t>(kRunFileHeaderBytes));
    w.u16(0);
    w.u16(1);
    for (std::uint32_t r : relocs) {
        const Far16 f = far_of(r);
        w.u16(f.off);
        w.u16(f.seg);
    }
    w.zeros(header_len - w.size());
    w.bytes(image);
    Bytes exe = w.take();
    std::uint64_t sum = 0;
    for (std::size_t i = 0; i < exe.size(); i += 2) sum += exe[i] | (i + 1 < exe.size() ? exe[i + 1] << 8 : 0);
    const std::uint16_t checksum = static_cast<std::uint16_t>(~sum & 0xFFFF);   // makes the file's words add up to FFFF
    exe[18] = static_cast<std::uint8_t>(checksum & 0xFF);
    exe[19] = static_cast<std::uint8_t>(checksum >> 8);

    say(std::format("  the text ends at {:05x}, DGROUP is at {:05x}; the image is {:04x} bytes, the program ends at {:05x}", end,
                    dgroup, image.size(), top));
    LinkResult result;
    result.exe = std::move(exe);
    result.dgroup = dgroup;
    for (const Module& m : mods) {
        ModuleMap map{m.name, {}};
        for (const auto& [sg, p] : m.pieces)
            if (p.at) map.pieces.push_back({p.segment, *p.at, p.length});
        result.modules.push_back(std::move(map));
    }
    return result;
}

}  // namespace cb::exact::linkemu
