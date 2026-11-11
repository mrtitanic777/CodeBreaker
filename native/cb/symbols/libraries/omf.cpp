// cb/symbols/libraries/omf.cpp -- 16-bit OMF libraries (see omf.hpp).
#include "cb/symbols/libraries/omf.hpp"

#include <algorithm>
#include <deque>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::libraries::omf {
namespace {

using std::string;

// OMF record types this reader follows.
constexpr std::uint8_t kLnames = 0x96;
constexpr std::uint8_t kSegdef = 0x98;      // 0x99: the 32-bit form (every record type's low bit says "wide")
constexpr std::uint8_t kPubdef = 0x90;
constexpr std::uint8_t kLedata = 0xA0;
constexpr std::uint8_t kComdat = 0xC2;
constexpr std::uint8_t kFixupp = 0x9C;
constexpr std::uint8_t kModend = 0x8A;      // 0x8B: the 32-bit form
constexpr std::uint8_t kLibraryEnd = 0xF1;
constexpr std::uint8_t kLibraryHeader = 0xF0;
constexpr std::uint8_t kFarCallOpcode = 0x9A;

constexpr std::uint8_t kFieldByte = 1;      // a byte a fix-up's field covers
constexpr std::uint8_t kFarCallByte = 2;    // the opcode of a far call a linker may rewrite

// A segment's bytes are built up from data records; a 32-bit segment can claim 4 GiB, and no library holds one.
constexpr std::uint64_t kMaxSegmentBytes = 64ull << 20;

// Bytes a 16-bit fix-up covers, by its location kind (the low and high bytes, an offset, a base, a pointer ...).
std::uint32_t fixup_width(unsigned location) {
    switch (location) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 4;
        case 4: return 1;
        case 5: return 2;
        case 9: return 4;
        case 11: return 6;
        case 13: return 4;
        default: return 2;
    }
}

// An index or length that takes one byte, or two when the high bit of the first is set.
std::uint32_t number(io::ByteReader& r) {
    const std::uint32_t first = r.u8();
    return (first & 0x80) ? (((first & 0x7F) << 8) | r.u8()) : first;
}

std::uint32_t offset_field(io::ByteReader& r, bool wide) { return wide ? r.u32() : r.u16(); }

// What a segment or a piece is built from: its bytes and the mask of the places a fix-up fills.
struct Block {
    Bytes buf;
    Bytes mask;   // one per byte, may be longer than buf (a fix-up can be named before its data arrives)

    void write(std::uint32_t off, ByteView chunk) {
        const std::uint64_t end = std::uint64_t{off} + chunk.size();
        if (end > kMaxSegmentBytes) throw FormatError("omf", off, "a data record lies beyond any segment a library holds");
        if (end > buf.size()) buf.resize(static_cast<std::size_t>(end), 0);
        std::copy(chunk.begin(), chunk.end(), buf.begin() + off);
    }
    void leave_out(std::uint64_t at, std::uint32_t count, std::uint8_t flag) {
        if (at + count > kMaxSegmentBytes + 1024) throw FormatError("omf", at, "a fix-up lies beyond any segment a library holds");
        if (at + count > mask.size()) mask.resize(static_cast<std::size_t>(at + count), 0);
        for (std::uint32_t k = 0; k < count; ++k) mask[static_cast<std::size_t>(at + k)] = flag;
    }
    // The mask of `size` bytes from `start`.
    Bytes mask_of(std::size_t start, std::size_t size) const {
        Bytes out(size, 0);
        for (std::size_t k = 0; k < size && start + k < mask.size(); ++k) out[k] = mask[start + k];
        return out;
    }
};

struct Segment : Block {
    string cls;   // its class name: CODE, DATA ...
};

struct Piece : Block {
    bool code = false;   // a COMDAT piece of code (a function), not data
};

struct Public {
    std::uint32_t seg = 0;
    std::uint32_t off = 0;
    string name;
    friend auto operator<=>(const Public&, const Public&) = default;
};

bool holds_code(const string& class_name) { return text::upper(class_name).find("CODE") != string::npos; }

// A length-prefixed name (Latin-1), the whole of it or a FormatError.
string name_at(io::ByteReader& r) {
    const std::uint8_t length = r.u8();
    return text::from_latin1(r.bytes(length));
}

}  // namespace

Bytes fields_only(ByteView mask) {
    Bytes out(mask.size(), 0);
    for (std::size_t i = 0; i < mask.size(); ++i) out[i] = mask[i] == kFieldByte ? 1 : 0;
    return out;
}

std::vector<Function> module_functions(ByteView module) {
    std::vector<string> names{""};   // LNAMES, indexed from 1
    std::deque<Segment> segments;    // (stable references: a data record points at one)
    std::deque<Piece> pieces;
    std::unordered_map<std::uint32_t, std::size_t> piece_of_name;   // lookup only; `pieces` keeps the order
    std::vector<std::uint32_t> piece_names;
    std::vector<Public> publics;
    Block* last = nullptr;           // the data record fix-ups apply to
    std::uint32_t last_offset = 0;

    std::size_t at = 0;
    while (at + 3 <= module.size()) {
        const std::uint8_t kind = module[at];
        const std::size_t size = module[at + 1] | (module[at + 2] << 8);
        const std::size_t available = module.size() - (at + 3);
        io::ByteReader r(module.subspan(at + 3, std::min(size > 0 ? size - 1 : 0, available)), "omf record");   // (the checksum byte is not read)
        at += 3 + size;
        const bool wide = (kind & 1) != 0;   // the 32-bit form of a record

        if (kind == kLnames) {
            while (!r.eof()) names.push_back(name_at(r));
        } else if (kind == kSegdef || kind == (kSegdef | 1)) {
            const std::uint8_t attributes = r.u8();
            if ((attributes >> 5) == 0) r.skip(3);   // an absolute segment: frame number and offset
            r.skip(wide ? 4 : 2);                    // its length
            number(r);                               // its name
            const std::uint32_t class_name = number(r);
            segments.emplace_back();
            segments.back().cls = class_name < names.size() ? names[class_name] : string();
        } else if (kind == kLedata || kind == (kLedata | 1) || kind == kComdat || kind == (kComdat | 1)) {
            Block* block = nullptr;
            std::uint32_t off = 0;
            ByteView chunk;
            if (kind >= kComdat) {   // a piece of its own
                const std::uint8_t flags = r.u8();
                const unsigned how = r.u8() & 0x0F;
                r.seek(3);
                off = offset_field(r, wide);
                number(r);   // its data type
                bool code = how == 1 || how == 3;
                if (how == 0) {   // explicit allocation: a segment says whether it is code
                    number(r);    // the group
                    const std::uint32_t seg = number(r);
                    if (seg == 0) r.skip(2);
                    code = seg >= 1 && seg <= segments.size() && holds_code(segments[seg - 1].cls);
                }
                const std::uint32_t name = number(r);
                auto [it, fresh] = piece_of_name.try_emplace(name, pieces.size());
                if (fresh) {
                    pieces.emplace_back();
                    pieces.back().code = code && (flags & 2) == 0;
                    piece_names.push_back(name);
                }
                block = &pieces[it->second];
                chunk = r.bytes(r.remaining());
            } else {
                const std::uint32_t seg = number(r);
                off = offset_field(r, wide);
                chunk = r.bytes(r.remaining());
                if (seg < 1 || seg > segments.size()) {
                    last = nullptr;
                    continue;
                }
                block = &segments[seg - 1];
            }
            block->write(off, chunk);
            last = block;
            last_offset = off;
        } else if ((kind == kFixupp || kind == (kFixupp | 1)) && last != nullptr) {
            while (!r.eof()) {
                const std::uint8_t first = r.u8();
                if (!(first & 0x80)) {   // a thread: says how later fix-ups are framed
                    if (((first >> 2) & 7) < 3) number(r);
                    continue;
                }
                const std::uint32_t where = ((first & 3u) << 8) | r.u8();
                const unsigned location = (first >> 2) & 0xF;
                const std::uint8_t fix = r.u8();
                if (!(fix & 0x80) && ((fix >> 4) & 7) < 3) number(r);   // the frame datum
                if (!(fix & 0x08)) number(r);                           // the target datum
                if (!(fix & 0x04)) r.skip(wide ? 4 : 2);                // the target displacement
                const std::uint64_t start = std::uint64_t{last_offset} + where;
                if (location == 3 && start > 0) {
                    if (start - 1 >= last->buf.size()) throw FormatError("omf", start, "a fix-up lies past the end of its segment");
                    if (last->buf[static_cast<std::size_t>(start - 1)] == kFarCallOpcode &&
                        (start - 1 >= last->mask.size() || last->mask[static_cast<std::size_t>(start - 1)] == 0)) {
                        last->leave_out(start - 1, 1, kFarCallByte);   // a far call: the linker may rewrite all of it
                    }
                }
                last->leave_out(start, fixup_width(location), kFieldByte);
            }
        } else if (kind == kPubdef || kind == (kPubdef | 1)) {
            number(r);   // the group
            const std::uint32_t seg = number(r);
            if (seg == 0) r.skip(2);
            while (!r.eof()) {
                Public p;
                p.seg = seg;
                p.name = name_at(r);
                p.off = offset_field(r, wide);
                number(r);   // its type
                publics.push_back(std::move(p));
            }
        }
    }

    std::vector<Function> out;
    for (std::size_t k = 0; k < pieces.size(); ++k) {
        const Piece& p = pieces[k];
        if (!p.code || piece_names[k] >= names.size()) continue;
        out.push_back({names[piece_names[k]], p.buf, p.mask_of(0, p.buf.size()), {}, 0});
    }
    std::sort(publics.begin(), publics.end());
    for (std::size_t k = 0; k < publics.size(); ++k) {
        const Public& pub = publics[k];
        if (pub.seg < 1 || pub.seg > segments.size() || !holds_code(segments[pub.seg - 1].cls)) continue;
        const Segment& seg = segments[pub.seg - 1];
        // up to the next public of the same segment that lies further on
        std::size_t end = seg.buf.size();
        for (std::size_t j = k + 1; j < publics.size() && publics[j].seg == pub.seg; ++j) {
            if (publics[j].off > pub.off) {
                end = publics[j].off;
                break;
            }
        }
        // (a public past the end of its segment names no bytes)
        const std::size_t from = std::min<std::size_t>(pub.off, seg.buf.size());
        const std::size_t to = std::max(from, std::min(end, seg.buf.size()));
        Function f;
        f.name = pub.name;
        f.code.assign(seg.buf.begin() + static_cast<std::ptrdiff_t>(from), seg.buf.begin() + static_cast<std::ptrdiff_t>(to));
        f.mask = seg.mask_of(from, to - from);
        f.at = pub.off;
        out.push_back(std::move(f));
    }
    return out;
}

std::vector<Function> functions(ByteView data) {
    std::vector<Function> out;
    std::size_t at = 0;
    std::size_t page = 1;
    std::optional<std::size_t> start;
    if (!data.empty() && data[0] == kLibraryHeader) {
        if (data.size() < 3) return out;
        page = static_cast<std::size_t>(data[1] | (data[2] << 8)) + 3;
        at = page;
    }
    while (at + 3 <= data.size() && data[at] != kLibraryEnd) {
        const std::uint8_t kind = data[at];
        const std::size_t size = data[at + 1] | (data[at + 2] << 8);
        if (!start) start = at;
        at += 3 + size;
        if (kind == kModend || kind == (kModend | 1)) {   // the end of one object module
            const std::size_t end = std::min(at, data.size());
            try {
                std::vector<Function> found = module_functions(data.subspan(*start, end - *start));
                out.insert(out.end(), std::make_move_iterator(found.begin()), std::make_move_iterator(found.end()));
            } catch (const FormatError&) {   // one module of a library the reader cannot follow: it gives nothing
            }
            start.reset();
            at += (page - at % page) % page;
        }
    }
    return out;
}

}  // namespace cb::symbols::libraries::omf
