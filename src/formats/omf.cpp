// formats/omf.cpp -- the OMF reader and the few records the exact lanes write (omf.hpp).
#include "formats/omf.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <tuple>

#include "formats/format_builtin.hpp"
#include "formats/read.hpp"

namespace cb::formats::omf {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "omf";constexpr std::uint64_t kMaxExpansion = std::uint64_t{1} << 28;  // a segment or an expansion past this is not a real object

// Record types.
constexpr std::uint8_t kTheadr = 0x80, kModend = 0x8A, kModend32 = 0x8B, kExtdef = 0x8C, kPubdef = 0x90, kPubdef32 = 0x91,
                       kLnames = 0x96, kSegdef = 0x98, kSegdef32 = 0x99, kGrpdef = 0x9A, kFixupp = 0x9C, kFixupp32 = 0x9D,
                       kLedata = 0xA0, kLedata32 = 0xA1, kLidata = 0xA2, kLidata32 = 0xA3, kBakpat = 0xB2, kBakpat32 = 0xB3,
                       kComdef = 0xB0, kLextdef = 0xB4, kLextdef2 = 0xB5, kLcomdef = 0xB8, kCextdef = 0xBC, kLibhdr = 0xF0,
                       kLibend = 0xF1, kLibraryModule = 0x80;

std::uint8_t u8(ByteView r, std::uint64_t at) { return get<std::uint8_t>(r, at, kReader); }
std::uint16_t u16(ByteView r, std::uint64_t at) { return get<std::uint16_t>(r, at, kReader); }

// An unsigned little-endian value of `n` bytes (1, 2, 3, 4), strict.
std::uint64_t le(ByteView r, std::uint64_t at, std::size_t n) {
    const ByteView b = slice(r, at, n, kReader);
    std::uint64_t v = 0;
    for (std::size_t k = n; k-- > 0;) v = (v << 8) | b[k];
    return v;
}

// An OMF index at q: one byte, or two when the top bit is set -> (value, the offset past it).
std::pair<std::int64_t, std::uint64_t> idx(ByteView r, std::uint64_t q) {
    const std::uint8_t b = u8(r, q);
    if (b & 0x80) return {static_cast<std::int64_t>(((b & 0x7F) << 8) | u8(r, q + 1)), q + 2};
    return {b, q + 1};
}

// The name table / segment table entry for a 1-based OMF index.
const std::string& indexed(const std::vector<std::string>& table, std::int64_t index, std::string_view what) {
    if (index < 1 || static_cast<std::uint64_t>(index) > table.size())
        throw FormatError(kReader, 0, std::format("{} index {} is out of range", what, index));
    return table[static_cast<std::size_t>(index - 1)];
}

// A length-prefixed name at q (clamped to the record), latin1 decoded.
std::string name_at(ByteView r, std::uint64_t q) { return text::from_latin1(clamped(r, q + 1, u8(r, q))); }

void lay(Bytes& b, std::uint64_t off, ByteView d) {
    if (off + d.size() > kMaxExpansion) throw UnsupportedError(kReader, off, "segment data is too large");
    if (off + d.size() > b.size()) b.resize(off + d.size(), 0);
    std::copy(d.begin(), d.end(), b.begin() + static_cast<std::ptrdiff_t>(off));
}

// The module's records as omf.fmt reads them (up to and including the MODEND).
eng::Tree omf_tree(ByteView data) {
    eng::ReadOptions options;
    options.root = "Module";
    options.keep_source = false;   // only read from
    return eng::builtin().read("omf", data, options);
}

// The bytes a node of the tree covers.
ByteView omf_bytes(ByteView data, const eng::Node& n) { return data.subspan(n.offset, n.size); }

// A length-prefixed name of the tree, latin1 decoded.
std::string omf_name(ByteView data, const eng::Node& n) { return text::from_latin1(omf_bytes(data, n.get("text"))); }

std::int64_t omf_index(const eng::Node& n, std::string_view field) { return static_cast<std::int64_t>(n.get(field).u("value")); }

// A list of a record's body that stops before the body's end: the entry there runs past the record.
void omf_whole(const eng::Node& list, const eng::Node& body) {
    const std::uint64_t end = list.children.empty() ? list.offset : list.children.back().offset + list.children.back().size;
    if (end < body.offset + body.size) throw TruncatedError(kReader, end, 1, body.offset + body.size - end);
}

}  // namespace

bool is_aout(ByteView data) {
    if (data.size() < 16) return false;
    const std::uint16_t m = get<std::uint16_t>(data, 0, kReader);
    return m == 0x107 || m == 0x108 || m == 0x109;
}

std::vector<Record> records(ByteView data) {
    std::vector<Record> out;
    const eng::Tree tree = omf_tree(data);
    for (const eng::Node& rec : tree.root.get("records").children)
        out.push_back({static_cast<std::uint8_t>(rec.u("type")), omf_bytes(data, rec.get("payload"))});   // a record cut short by the end of the file is what is there
    return out;
}

Bytes record(std::uint8_t type, ByteView payload) {
    const std::size_t n = payload.size() + 1;
    if (n > 0xFFFF) throw FormatError(kReader, 0, "record is too long");
    Bytes b{type, static_cast<std::uint8_t>(n), static_cast<std::uint8_t>(n >> 8)};
    append(b, payload);
    unsigned sum = 0;
    for (std::uint8_t c : b) sum += c;
    b.push_back(static_cast<std::uint8_t>(-static_cast<int>(sum)));
    return b;
}

Bytes name(std::string_view latin1_bytes) {
    if (latin1_bytes.size() > 255) throw FormatError(kReader, 0, "name is longer than 255 bytes");
    Bytes b{static_cast<std::uint8_t>(latin1_bytes.size())};
    append(b, as_bytes(latin1_bytes));
    return b;
}

// ---- read ---------------------------------------------------------------------------------------------------

// The layout of the records is omf.fmt's; the order things are checked in, and what an index out of range, data for a
// segment that is not there or too much data mean, are this reader's.
Object read(ByteView data) {
    if (is_aout(data)) throw UnsupportedError(kReader, 0, "a Version 7 a.out object, not OMF");
    const eng::Tree tree = omf_tree(data);
    Object o;
    std::vector<std::string> names, segs;
    std::vector<Bytes> bufs;  // each segment's bytes, indexed by segment number - 1
    std::vector<std::pair<std::size_t, std::uint64_t>> filled;  // (segment index, bytes covered), in the order first filled
    std::map<std::pair<int, int>, std::pair<std::optional<std::int64_t>, std::optional<std::int64_t>>> threads;  // lookup only
    std::optional<std::pair<std::size_t, std::uint64_t>> last;  // the segment and offset of the latest LEDATA/LIDATA
    auto note_filled = [&filled](std::size_t si, std::uint64_t end) {
        for (auto& kv : filled)
            if (kv.first == si) {
                kv.second = std::max(kv.second, end);
                return;
            }
        filled.emplace_back(si, end);
    };
    auto buffer = [&bufs](std::int64_t si) -> Bytes& {
        if (si < 1 || static_cast<std::uint64_t>(si) > bufs.size()) throw FormatError(kReader, 0, "data for an undefined segment");
        return bufs[static_cast<std::size_t>(si - 1)];
    };
    for (const eng::Node& rec : tree.root.get("records").children) {
        const std::uint8_t t = static_cast<std::uint8_t>(rec.u("type"));
        const eng::Node* body = rec.child("body");
        const ByteView r = omf_bytes(data, rec.get("payload"));
        const bool handled = t == kLnames || t == kSegdef || t == kSegdef32 || ((t == kModend || t == kModend32) && !r.empty()) || t == kGrpdef || t == kExtdef ||
                             t == kComdef || t == kLextdef || t == kLextdef2 || t == kLcomdef || t == kCextdef || t == kPubdef || t == kPubdef32 || t == kLedata ||
                             t == kLedata32 || t == kLidata || t == kLidata32 || t == kFixupp || t == kFixupp32 || t == kBakpat || t == kBakpat32;
        if (!handled) continue;
        if (!body) throw TruncatedError(kReader, rec.offset + 3, r.size() + 1, r.size());   // a field runs past the record
        if (t == kLnames) {
            for (const eng::Node& n : body->get("names").children) names.push_back(omf_name(data, n));
        } else if (t == kSegdef || t == kSegdef32) {
            const eng::Node& s = body->get("segdef");
            const std::uint8_t attr = static_cast<std::uint8_t>(s.u("attr"));
            const std::uint8_t align = attr >> 5;
            std::int64_t ln = static_cast<std::int64_t>(s.get("length").u("value"));
            if (attr & 2) ln = t == kSegdef ? 0x10000 : 0;  // (a 4 GB 32-bit segment: its data say how much)
            const std::int64_t ni = omf_index(s, "name"), ci = omf_index(s, "class_name");
            const std::string& nm = indexed(names, ni, "segment name");
            segs.push_back(nm);
            o.segdefs.push_back({nm, ci ? indexed(names, ci, "class name") : std::string(), ln, align});
            o.combine.insert_or_assign(nm, static_cast<std::uint8_t>((attr >> 2) & 7));  // (5: a stack segment)
            bufs.emplace_back(static_cast<std::size_t>(std::clamp<std::int64_t>(ln, 0, std::int64_t{1} << 24)), std::uint8_t{0});
        } else if (t == kModend || t == kModend32) {  // MODEND with the start address
            const eng::Node& m = body->get("modend");
            if (!(m.u("mtype") & 0x40)) continue;
            const std::uint64_t fd = m.u("fd");
            if (!(fd & 8)) {
                const std::int64_t ti = omf_index(m, "target");
                const std::int64_t disp = !(fd & 4) ? static_cast<std::int64_t>(m.u("disp")) : 0;
                if ((fd & 3) == 0 && ti) o.start = StartAddress{indexed(segs, ti, "segment"), disp};
            }
        } else if (t == kGrpdef) {
            const eng::Node& members_node = body->get("members");
            std::vector<std::string> members;
            for (const eng::Node& m : members_node.children) members.push_back(indexed(segs, omf_index(m, "segment"), "segment"));
            omf_whole(members_node, *body);
            o.groups.insert_or_assign(indexed(names, omf_index(*body, "group"), "group name"), std::move(members));
        } else if (t == kExtdef || t == kComdef || t == kLextdef || t == kLextdef2 || t == kLcomdef) {
            const eng::Node& list = body->get("externals");
            omf_whole(list, *body);
            for (const eng::Node& e : list.children) {
                const std::string nm = omf_name(data, e.get("name"));
                o.externals.push_back(nm);
                if (t == kComdef || t == kLcomdef) {  // a communal variable: its kind and size follow
                    const bool is_far = e.u("kind") == 0x61;
                    std::vector<std::uint64_t> vals;
                    for (const std::string_view part : {std::string_view("first"), std::string_view("second")}) {
                        const eng::Node* v = e.child(part);
                        if (!v) break;
                        const eng::Node* wide = v->child("v");
                        vals.push_back(wide ? wide->value : v->u("c"));
                    }
                    o.communals.insert_or_assign(nm, Communal{is_far ? vals[0] * vals[1] : vals[0], is_far});
                }
            }
        } else if (t == kCextdef) {  // named by a name index
            const eng::Node& list = body->get("externals");
            omf_whole(list, *body);
            for (const eng::Node& e : list.children) {
                const std::int64_t ni = omf_index(e, "name");
                o.externals.push_back(0 < ni && static_cast<std::uint64_t>(ni) <= names.size() ? names[static_cast<std::size_t>(ni - 1)] : "?");
            }
        } else if (t == kPubdef || t == kPubdef32) {
            const std::int64_t si = omf_index(*body, "segment");
            const eng::Node& list = body->get("publics");
            for (const eng::Node& p : list.children)
                o.publics.push_back({omf_name(data, p.get("name")), si ? std::optional<std::string>(indexed(segs, si, "segment")) : std::nullopt,
                                     static_cast<std::int64_t>(p.get("offset").u("value"))});
            omf_whole(list, *body);
        } else if (t == kLedata || t == kLedata32) {
            const std::int64_t si = omf_index(*body, "segment");
            const std::uint64_t off = body->get("offset").u("value");
            const ByteView d = omf_bytes(data, body->get("data"));
            lay(buffer(si), off, d);
            last = {static_cast<std::size_t>(si), off};
            note_filled(static_cast<std::size_t>(si), off + d.size());
        } else if (t == kLidata || t == kLidata32) {  // repeated blocks
            const std::int64_t si = omf_index(*body, "segment");
            const std::uint64_t off = body->get("offset").u("value");
            // one iterated-data block: repeat count, block count, then nested blocks or the data
            std::function<Bytes(const eng::Node&, int)> block = [&](const eng::Node& b, int depth) -> Bytes {
                const std::uint64_t rep = b.get("repeat").u("value");
                const std::uint64_t count = b.u("count");
                Bytes s;
                if (count == 0) {
                    const ByteView piece = omf_bytes(data, b.get("data"));
                    s.assign(piece.begin(), piece.end());
                } else {
                    const std::vector<eng::Node>& children = b.get("blocks").children;
                    for (const eng::Node& child : children) append(s, block(child, depth + 1));
                    if (children.size() < count) {   // the next block could not be read: nested too deeply, or past the record
                        if (depth + 1 > 64) throw FormatError(kReader, b.offset, "iterated data is nested too deeply");
                        throw TruncatedError(kReader, b.offset + b.size, 1, 0);
                    }
                }
                if (rep > 0 && s.size() * rep > kMaxExpansion) throw UnsupportedError(kReader, b.offset + b.size, "iterated data is too large");
                Bytes out;
                for (std::uint64_t k = 0; k < rep; ++k) append(out, s);
                return out;
            };
            const eng::Node& blocks = body->get("blocks");
            Bytes out;
            for (const eng::Node& b : blocks.children) append(out, block(b, 0));
            omf_whole(blocks, *body);
            lay(buffer(si), off, out);
            last = {static_cast<std::size_t>(si), off};
            note_filled(static_cast<std::size_t>(si), off + out.size());
        } else if (t == kFixupp || t == kFixupp32) {
            const eng::Node& list = body->get("subrecords");
            for (const eng::Node& sub : list.children) {
                const std::uint64_t c = sub.u("c");
                if (!(c & 0x80)) {  // a thread
                    const int d = static_cast<int>((c >> 6) & 1), meth = static_cast<int>((c >> 2) & 7);
                    std::optional<std::int64_t> ix;
                    if (const eng::Node* i = sub.child("index")) ix = static_cast<std::int64_t>(i->u("value"));
                    threads[{d, static_cast<int>(c & 3)}] = {meth, ix};
                    continue;
                }
                const int segment_relative = static_cast<int>((c >> 6) & 1), loc = static_cast<int>((c >> 2) & 7);
                const std::int64_t off = static_cast<std::int64_t>(((c & 3) << 8) | sub.u("low"));
                const std::uint64_t fd = sub.u("fd");
                std::optional<std::int64_t> tm, ti;
                if (fd & 8) {
                    if (auto it = threads.find({0, static_cast<int>(fd & 3)}); it != threads.end()) tm = it->second.first, ti = it->second.second;
                } else {
                    tm = static_cast<std::int64_t>(fd & 3);
                    ti = omf_index(sub, "target");
                }
                const std::int64_t disp = !(fd & 4) ? static_cast<std::int64_t>(sub.get("disp").u("value")) : 0;
                Target tgt;
                if (tm) {
                    if (!ti) throw FormatError(kReader, sub.offset + sub.size, "fix-up target thread has no index");
                    if (*ti == 0) {
                        // index 0 names nothing; the target stays Unknown (it is not read as the last
                        // segment / group / external defined)
                    } else if ((*tm & 3) == 0) {
                        tgt.kind = Target::Kind::Segment;
                        tgt.name = indexed(segs, *ti, "segment");
                    } else if ((*tm & 3) == 1) {
                        tgt.kind = Target::Kind::Group;
                        if (*ti && *ti - 1 < static_cast<std::int64_t>(names.size())) tgt.name = names[static_cast<std::size_t>(*ti - 1)];
                        else tgt.number = *ti;
                    } else {
                        tgt.kind = Target::Kind::External;
                        tgt.name = indexed(o.externals, *ti, "external");
                    }
                }
                FixupKind kind = loc == 1 || loc == 5 ? FixupKind::Offset
                                 : loc == 2           ? FixupKind::Base
                                 : loc == 3           ? FixupKind::Pointer
                                 : loc == 0           ? FixupKind::LowByte
                                                      : FixupKind::Other;
                if (!segment_relative) kind = FixupKind::Relative;
                if (!last) continue;  // a fix-up before any data: nothing to place it in
                o.fixups.push_back({segs[last->first - 1], static_cast<std::int64_t>(last->second) + off, kind, std::move(tgt), disp,
                                    static_cast<std::uint8_t>((c >> 2) & 0x0F)});
            }
            omf_whole(list, *body);
        } else if (t == kBakpat || t == kBakpat32) {  // values added in once the segment is written
            const std::uint64_t sz = body->u("size_code");
            const std::size_t size = sz == 0 ? 1 : sz == 1 ? 2 : sz == 2 ? 4 : 2;
            Bytes& b = buffer(omf_index(*body, "segment"));
            for (const eng::Node& patch : body->get("patches").children) {
                const std::uint64_t at = patch.get("place").u("value"), val = patch.get("value").u("value");
                if (at + size <= b.size()) {
                    std::uint64_t cur = 0;
                    for (std::size_t k = size; k-- > 0;) cur = (cur << 8) | b[at + k];
                    const std::uint64_t v = (cur + val) & (size == 4 ? 0xFFFFFFFFull : (1ull << (8 * size)) - 1);
                    for (std::size_t k = 0; k < size; ++k) b[at + k] = static_cast<std::uint8_t>(v >> (8 * k));
                }
            }
        }
    }
    for (std::size_t k = 0; k < bufs.size(); ++k) o.segments.insert_or_assign(segs[k], std::move(bufs[k]));
    for (const auto& [si, n] : filled) o.filled.insert_or_assign(segs[si - 1], n);
    return o;
}

// ---- libraries ----------------------------------------------------------------------------------------------

std::vector<std::pair<std::string, ByteView>> library(ByteView data) {
    std::vector<std::pair<std::string, ByteView>> out;
    if (u8(data, 0) != kLibhdr) {
        out.emplace_back("(object)", data);
        return out;
    }
    const std::uint64_t page = std::uint64_t{u16(data, 1)} + 3;
    const std::uint64_t dict_off = get<std::uint32_t>(data, 3, kReader);
    std::map<std::string, std::uint64_t> seen;  // how many modules of a name so far; lookup only
    std::uint64_t p = page;
    while (p < dict_off) {
        if (u8(data, p) != kLibraryModule) {
            p += (p % page) ? page - p % page : page;
            continue;
        }
        std::uint64_t q = p;
        while (q + 3 <= data.size()) {
            const std::uint8_t t = u8(data, q);
            q += 3 + (u8(data, q + 1) | (u8(data, q + 2) << 8));
            if (t == kModend || t == kModend32) break;
        }
        const std::string nm = text::from_latin1(clamped(data, p + 4, u8(data, p + 3)));
        const std::uint64_t n = ++seen[nm];
        out.emplace_back(nm + (n > 1 ? "#" + std::to_string(n) : std::string()), clamped(data, p, q > p ? q - p : 0));
        p = q;
        if (p % page) p += page - p % page;
    }
    return out;
}

std::vector<std::string> publics(ByteView data) {
    std::vector<std::string> out;
    std::set<std::string> seen;
    auto add = [&](std::string s) {
        if (seen.insert(s).second) out.push_back(std::move(s));
    };
    const std::uint64_t n = data.size();
    std::uint64_t i = 0;
    while (i + 3 <= n) {
        const std::uint8_t rectype = data[i];
        const std::uint64_t length = data[i + 1] | (data[i + 2] << 8);
        const std::uint64_t body = i + 3, end = i + 3 + length;
        if (end > n) break;
        if (rectype == kPubdef || rectype == kPubdef32) {
            std::uint64_t p = idx(data, body).second;
            const auto [seg, p2] = idx(data, p);
            p = p2;
            if (seg == 0) p += 2;  // base frame present when the segment index is 0
            const std::uint64_t offsz = rectype == kPubdef ? 2 : 4;
            while (p + 1 < end) {  // end-1 is the record checksum
                const std::uint64_t ln = u8(data, p);
                p += 1;
                if (p + ln > end) break;
                add(text::from_latin1(slice(data, p, ln, kReader)));
                p += ln + offsz;
                p = idx(data, p).second;
            }
        }
        i = end;
    }
    return out;
}

std::vector<std::string> externs(ByteView data) {
    std::vector<std::string> out;
    const std::uint64_t n = data.size();
    std::uint64_t i = 0;
    while (i + 3 <= n) {
        const std::uint8_t rectype = data[i];
        const std::uint64_t end = i + 3 + (data[i + 1] | (data[i + 2] << 8));
        if (end > n) break;
        if (rectype == kExtdef) {
            std::uint64_t q = i + 3;
            while (q + 1 < end) {  // end-1 is the record checksum
                const std::uint64_t ln = u8(data, q);
                out.push_back(text::from_latin1(clamped(data, q + 1, ln)));
                q += 1 + ln;
                q += (q < end && (u8(data, q) & 0x80)) ? 2 : 1;  // type index
            }
        }
        i = end;
    }
    return out;
}

std::pair<std::set<std::string>, std::set<std::string>> names_of(ByteView data) {
    const Object o = read(data);
    std::set<std::string> pubs;
    for (const Public& p : o.publics) pubs.insert(p.name);
    return {std::move(pubs), std::set<std::string>(o.externals.begin(), o.externals.end())};
}

// ---- walking and listing ----------------------------------------------------------------------------------

Walk walk(ByteView data, std::uint64_t start) {
    Walk w;
    try {
        for (std::uint64_t i = start; i < data.size();) {
            const std::uint8_t t = data[i];
            if (t == 0) {
                ++i;
                continue;
            }
            const std::uint64_t n = u16(data, i + 1);
            w.records.push_back({i, t, clamped(data, i + 3, n ? n - 1 : 0)});
            i += 3 + n;
            if (t == kLibend) break;
        }
    } catch (const FormatError& e) {  // a damaged record ends the walk; what came before it is kept
        w.error = e.what();
    }
    return w;
}

std::string record_name(std::uint8_t type) {
    switch (type) {
        case 0x80: return "THEADR";
        case 0x82: return "LHEADR";
        case 0x88: return "COMENT";
        case 0x8A:
        case 0x8B: return "MODEND";
        case 0x8C: return "EXTDEF";
        case 0x8E: return "TYPDEF";
        case 0x90:
        case 0x91: return "PUBDEF";
        case 0x94:
        case 0x95: return "LINNUM";
        case 0x96: return "LNAMES";
        case 0x98:
        case 0x99: return "SEGDEF";
        case 0x9A: return "GRPDEF";
        case 0x9C:
        case 0x9D: return "FIXUPP";
        case 0xA0:
        case 0xA1: return "LEDATA";
        case 0xA2:
        case 0xA3: return "LIDATA";
        case 0xB0: return "COMDEF";
        case 0xF0: return "LIBHDR";
        case 0xF1: return "LIBEND";
        default: return std::format("?{:02X}", type);
    }
}

std::string listing(ByteView data, bool raw) {
    std::string out;
    if (raw) {
        const Walk w = walk(data);
        for (const RawRecord& r : w.records)
            out += std::format("@{:06x} {:<8} len={}\n", r.offset, record_name(r.type), r.payload.size() + 1);
        if (w.error) throw FormatError(kReader, 0, *w.error);  // (the damaged record, after the ones before it)
        return out;
    }
    for (const auto& [nm, module] : library(data)) {
        const Object o = read(module);
        out += std::format("== {} ({} bytes)\n", nm, module.size());
        for (const SegDef& s : o.segdefs) out += std::format("  segment {:<12} class={:<8} {:>6} bytes\n", s.name, s.class_name, s.length);
        for (const Public& p : o.publics) out += std::format("  public  {:<24} {}:{:04x}\n", p.name, p.segment.value_or("-"), p.offset);
        for (const std::string& e : o.externals) out += std::format("  extern  {}\n", e);
    }
    return out;
}

// ---- the code of a module ---------------------------------------------------------------------------------

std::size_t fixup_width(std::uint8_t location) {
    switch (location) {
        case 0:
        case 4: return 1;
        case 3:
        case 9:
        case 13: return 4;
        case 11: return 6;
        default: return 2;
    }
}

CodeSegment code_segment(ByteView data) {
    const Object o = read(data);
    CodeSegment r;
    for (const Public& p : o.publics) r.publics.insert_or_assign(p.name, std::pair{p.segment.value_or("?"), p.offset});
    for (const SegDef& s : o.segdefs) {
        if (text::upper(s.class_name).find("CODE") != std::string::npos || text::upper(s.name).ends_with("TEXT")) {
            r.segment = s.name;
            break;
        }
    }
    if (!r.segment && !o.segdefs.empty()) r.segment = o.segdefs.front().name;
    if (!r.segment) return r;
    for (const Fixup& f : o.fixups)
        if (f.segment == *r.segment)
            for (std::uint64_t k = static_cast<std::uint64_t>(f.at); k < static_cast<std::uint64_t>(f.at) + fixup_width(f.location); ++k)
                r.fixup_mask.insert(k);
    const Bytes* bytes = o.segments.find(*r.segment);
    const std::uint64_t* n = o.filled.find(*r.segment);
    if (bytes) r.code.assign(bytes->begin(), bytes->begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(n ? *n : 0, bytes->size())));
    return r;
}

OrderedMap<std::string, FunctionBytes> function_bytes(ByteView data) {
    const CodeSegment c = code_segment(data);
    OrderedMap<std::string, FunctionBytes> out;
    if (!c.segment) return out;
    std::vector<std::pair<std::int64_t, std::string>> here;
    for (const auto& [name_, where] : c.publics)
        if (where.first == *c.segment) here.emplace_back(where.second, name_);
    std::sort(here.begin(), here.end());
    for (std::size_t i = 0; i < here.size(); ++i) {  // (a name met again: its last cut wins, first place kept)
        const std::uint64_t off = static_cast<std::uint64_t>(here[i].first);
        const std::uint64_t end = i + 1 < here.size() ? static_cast<std::uint64_t>(here[i + 1].first) : c.code.size();
        FunctionBytes f;
        if (off < end) {
            const ByteView piece = clamped(c.code, off, end - off);
            f.code.assign(piece.begin(), piece.end());
        }
        for (std::uint64_t k : c.fixup_mask)
            if (off <= k && k < end) f.fixup_mask.insert(k - off);
        out.insert_or_assign(here[i].second, std::move(f));
    }
    return out;
}

std::set<std::int64_t> holes(const Object& o, std::string_view seg) {
    std::set<std::int64_t> out;
    for (const Fixup& f : o.fixups) {
        if (f.segment != seg) continue;
        const std::int64_t n = f.kind == FixupKind::LowByte ? 1 : f.kind == FixupKind::Pointer ? 4 : 2;  // (off base rel, and any other: 2)
        for (std::int64_t k = f.at; k < f.at + n; ++k) out.insert(k);
    }
    return out;
}

std::vector<std::string> code_segments(const Object& o) {
    std::vector<std::string> out;
    for (const SegDef& s : o.segdefs) {
        const std::string cls = text::upper(s.class_name), nu = text::upper(s.name);
        const bool text_segment = s.name.size() >= 5 && s.name.ends_with("_TEXT");
        if (text_segment || cls == "CODE" || cls == "PROG" || nu == "PROG" || nu == "_PROG" || nu == "_CODE") out.push_back(s.name);
    }
    return out;
}

std::vector<FunctionSpan> functions(const Object& o, std::string_view seg) {
    std::vector<std::pair<std::int64_t, std::string>> ps;
    for (const Public& p : o.publics)
        if (p.segment && *p.segment == seg) ps.emplace_back(p.offset, p.name);
    std::sort(ps.begin(), ps.end());
    const std::string key(seg);
    const Bytes* bytes = o.segments.find(key);
    const std::uint64_t* filled = o.filled.find(key);
    const std::int64_t n = static_cast<std::int64_t>(std::min<std::uint64_t>(bytes ? bytes->size() : 0, filled ? *filled : 0));
    if ((!ps.empty() && ps.front().first > 0) || (ps.empty() && n)) ps.insert(ps.begin(), {0, "(module start)"});
    std::vector<FunctionSpan> out;
    for (std::size_t k = 0; k < ps.size(); ++k)
        out.push_back({ps[k].second, ps[k].first, (k + 1 < ps.size() ? ps[k + 1].first : n) - ps[k].first});
    return out;
}

// ---- rewriting -------------------------------------------------------------------------------------------------

Bytes rename_externs(ByteView data, const std::map<std::string, std::string>& mapping) {
    Bytes out;
    for (const Record& rec : records(data)) {
        Bytes payload(rec.payload.begin(), rec.payload.end());
        if (rec.type == kExtdef) {
            Bytes nb;
            std::uint64_t q = 0;
            const ByteView p = rec.payload;
            while (q < p.size()) {
                std::string nm = name_at(p, q);
                q += 1 + u8(p, q);
                // the type index as stored: a record cut after its last name (seen in MSVC 1.52's MFC objects) has
                // none, and is written back the same
                const ByteView type_index = clamped(p, q, 1);
                q += 1;
                auto it = mapping.find(nm);
                const Bytes encoded = text::to_latin1(it != mapping.end() ? it->second : nm);
                append(nb, name(as_text(encoded)));
                append(nb, type_index);
            }
            payload = std::move(nb);
        }
        append(out, record(rec.type, payload));
    }
    return out;
}

Bytes rename_symbols(ByteView data, const std::function<std::string(std::string_view)>& rename) {
    Bytes out;
    auto put_name = [&](Bytes& nb, const std::string& original, ByteView rest) {
        const Bytes neu = text::to_latin1(rename(original));
        if (neu.size() > 255) throw FormatError(kReader, 0, "a renamed symbol is longer than 255 bytes");
        nb.push_back(static_cast<std::uint8_t>(neu.size()));
        append(nb, neu);
        append(nb, rest);
    };
    for (const Record& rec : records(data)) {
        const std::uint8_t t = rec.type;
        const ByteView rb = rec.payload;
        Bytes payload(rb.begin(), rb.end());
        if (t == kExtdef || t == kComdef) {
            std::uint64_t q = 0;
            Bytes nb;
            while (q < rb.size()) {
                const std::string nm = name_at(rb, q);
                q += 1 + u8(rb, q);
                ByteView rest;
                if (t == kExtdef) {
                    rest = clamped(rb, q, 1);
                    q += 1;
                } else {
                    const std::uint64_t st = q;
                    q += 1;
                    const std::uint8_t kind = u8(rb, q);
                    q += 1;
                    for (int k = 0; k < (kind == 0x61 ? 2 : 1); ++k) {
                        const std::uint8_t b0 = u8(rb, q);
                        q += 1 + (b0 == 0x81 ? 2 : b0 == 0x84 ? 3 : b0 == 0x88 ? 4 : 0);
                    }
                    rest = clamped(rb, st, q - st);
                }
                put_name(nb, nm, rest);
            }
            payload = std::move(nb);
        } else if (t == kPubdef || t == kPubdef32) {
            std::uint64_t q = idx(rb, 0).second;
            const auto [si, q2] = idx(rb, q);
            q = q2;
            if (si == 0) q += 2;
            Bytes nb(rb.begin(), rb.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(q, rb.size())));
            const std::size_t w = t == kPubdef32 ? 4 : 2;
            while (q < rb.size()) {
                const std::string nm = name_at(rb, q);
                q += 1 + u8(rb, q);
                const ByteView stored = clamped(rb, q, w);
                Bytes rest(stored.begin(), stored.end());
                q += w;
                const std::uint64_t q3 = idx(rb, q).second;
                append(rest, clamped(rb, q, q3 - q));
                q = q3;
                put_name(nb, nm, rest);
            }
            payload = std::move(nb);
        }
        append(out, record(t, payload));
    }
    return out;
}

Bytes patched(ByteView data, std::string_view spec) {
    // "<segment>+<hex offset>=<hex bytes>"
    const auto plus = spec.find('+'), eq = spec.find('=');
    auto word_chars = [](std::string_view s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; });
    };
    if (plus == std::string_view::npos || eq == std::string_view::npos || eq < plus) throw UsageError("bad patch spec: " + std::string(spec));
    const std::string_view seg = spec.substr(0, plus), off_text = spec.substr(plus + 1, eq - plus - 1), hex = spec.substr(eq + 1);
    const auto off = text::parse_int<std::int64_t>(off_text, 16);
    if (!word_chars(seg) || !off || hex.empty() || hex.size() % 2 != 0 || off_text.starts_with("-") || off_text.starts_with("0x"))
        throw UsageError("bad patch spec: " + std::string(spec));
    Bytes value;
    for (std::size_t i = 0; i < hex.size(); i += 2) {
        const auto byte = text::parse_int<unsigned>(hex.substr(i, 2), 16);
        if (!byte || hex[i] == '-' || hex[i] == '+') throw UsageError("bad patch spec: " + std::string(spec));
        value.push_back(static_cast<std::uint8_t>(*byte));
    }
    return patched(data, seg, *off, value);
}

Bytes patched(ByteView data, std::string_view seg, std::int64_t offset, ByteView value) {
    const std::optional<std::int64_t> off = offset;
    std::vector<std::string> names, segs;
    Bytes out;
    for (const Record& rec : records(data)) {
        const std::uint8_t t = rec.type;
        const ByteView rb = rec.payload;
        Bytes r(rb.begin(), rb.end());
        if (t == kLnames) {
            for (std::uint64_t q = 0; q < rb.size(); q += 1 + u8(rb, q)) names.push_back(name_at(rb, q));
        } else if (t == kSegdef) {
            const std::uint64_t q = (u8(rb, 0) >> 5) ? 1 : 4;
            segs.push_back(indexed(names, idx(rb, q + 2).first, "segment name"));
        } else if (t == kLedata) {
            const auto [si, q] = idx(rb, 0);
            const std::int64_t lo = u8(rb, q) | (u8(rb, q + 1) << 8);
            if (indexed(segs, si, "segment") == seg) {
                for (std::size_t k = 0; k < value.size(); ++k) {
                    const std::int64_t at = *off + static_cast<std::int64_t>(k);
                    if (lo <= at && at < lo + static_cast<std::int64_t>(r.size()) - static_cast<std::int64_t>(q) - 2)
                        r[q + 2 + static_cast<std::uint64_t>(at - lo)] = value[k];
                }
            }
        }
        append(out, record(t, r));
    }
    return out;
}

// ---- writing objects -----------------------------------------------------------------------------------------------

namespace {

Bytes u16le(std::int64_t v) {
    if (v < 0 || v > 0xFFFF) throw FormatError(kReader, 0, "value does not fit 16 bits");
    return {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8)};
}

Bytes concat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) append(out, p);
    return out;
}

// the public names of a dict, sorted by value (ties in the dict's order)
std::vector<std::pair<Bytes, std::int64_t>> by_value(const OrderedMap<std::string, std::int64_t>& d) {
    std::vector<std::pair<Bytes, std::int64_t>> v;
    for (const auto& [k, val] : d) v.emplace_back(text::to_latin1(k), val);
    std::stable_sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    return v;
}

Bytes nm(std::string_view latin1) { return name(latin1); }
Bytes nm(const Bytes& latin1) { return name(as_text(latin1)); }

}  // namespace

Bytes raw_object(std::string_view title, ByteView code, const OrderedMap<std::string, std::int64_t>& pubs, ByteView data,
                 const OrderedMap<std::string, std::int64_t>& dpubs, std::string_view text_segment, int data_alignment) {
    const Bytes tl = text::to_latin1(title), text_name = text::to_latin1(text_segment);
    Bytes out = record(0x80, nm(tl));
    append(out, record(0x96, concat({nm(""), nm("DGROUP"), nm(text_name), nm("CODE"), nm("_DATA"), nm("DATA")})));
    append(out, record(0x98, concat({Bytes{0x28}, u16le(static_cast<std::int64_t>(code.size())), Bytes{3, 4, 1}})));  // byte aligned, public
    append(out, record(0x98, concat({Bytes{static_cast<std::uint8_t>(data_alignment == 2 ? 0x48 : 0x28)},
                                     u16le(static_cast<std::int64_t>(data.size())), Bytes{5, 6, 1}})));
    append(out, record(0x9A, Bytes{2, 0xFF, 2}));  // DGROUP: _DATA
    const std::pair<std::uint8_t, ByteView> parts[] = {{std::uint8_t{1}, code}, {std::uint8_t{2}, data}};
    for (const auto& [seg, bytes] : parts)
        for (std::size_t k = 0; k < bytes.size(); k += 1000)
            append(out, record(0xA0, concat({Bytes{seg}, u16le(static_cast<std::int64_t>(k)), Bytes(bytes.begin() + static_cast<std::ptrdiff_t>(k), bytes.begin() + static_cast<std::ptrdiff_t>(std::min(bytes.size(), k + 1000)))})));
    const std::tuple<std::uint8_t, std::uint8_t, const OrderedMap<std::string, std::int64_t>*> groups[] = {{std::uint8_t{0}, std::uint8_t{1}, &pubs}, {std::uint8_t{1}, std::uint8_t{2}, &dpubs}};
    for (const auto& [grp, seg, ps] : groups) {
        if (ps->empty()) continue;
        Bytes body{grp, seg};
        for (const auto& [n, value] : by_value(*ps)) append(body, concat({nm(n), u16le(value), Bytes{0}}));
        append(out, record(0x90, body));
    }
    append(out, record(0x8A, Bytes{0}));
    return out;
}

Bytes bss_object(const OrderedMap<std::string, std::int64_t>& names, std::int64_t start, std::int64_t end, std::string_view title,
                 std::string_view segment, bool paragraph_aligned) {
    const Bytes title_bytes = text::to_latin1(title), seg_bytes = text::to_latin1(segment);
    Bytes out = record(0x80, nm(title_bytes));
    append(out, record(0x96, concat({nm(""), nm("DGROUP"), nm(seg_bytes), nm("BSS")})));
    append(out, record(0x98, concat({Bytes{static_cast<std::uint8_t>(paragraph_aligned ? 0x68 : 0x48)}, u16le(end - start), Bytes{3, 4, 1}})));
    append(out, record(0x9A, Bytes{2, 0xFF, 1}));
    std::vector<std::pair<Bytes, std::int64_t>> items;
    for (const auto& [k, v] : names) items.emplace_back(text::to_latin1(k), v);
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.second != b.second ? a.second < b.second : a.first < b.first; });
    std::size_t k = 0;
    while (k < items.size()) {
        Bytes body{1, 1};
        while (k < items.size() && body.size() < 900) {
            append(body, concat({nm(items[k].first), u16le(items[k].second - start), Bytes{0}}));
            ++k;
        }
        append(out, record(0x90, body));
    }
    append(out, record(0x8A, Bytes{0}));
    return out;
}

}  // namespace cb::formats::omf
