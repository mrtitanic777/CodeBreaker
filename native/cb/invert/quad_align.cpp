#include "cb/invert/quad_align.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace cb::invert::quad_align {

namespace omf = cb::formats::omf;

namespace {

constexpr std::string_view kReader = "quad_align";
constexpr std::uint8_t kLnames = 0x96, kSegdef = 0x98, kSegdef32 = 0x99, kPubdef = 0x90, kPubdef32 = 0x91,
                       kLedata = 0xa0, kLedata32 = 0xa1;
constexpr std::string_view kCodeSegment = "PROG";   // the segment Lattice C puts its functions in
constexpr std::size_t kMaxSegment = 16u << 20;       // far more than any object of this compiler's era

// An OMF index: one byte, or two when the first has its top bit set.
std::uint32_t read_index(io::ByteReader& r) {
    const std::uint8_t first = r.u8();
    return (first & 0x80) ? (static_cast<std::uint32_t>(first & 0x7f) << 8) | r.u8() : first;
}

}  // namespace

std::vector<QuadFunction> parse_quads(ByteView quad_file) {
    const quad_dump::Scan scan = quad_dump::scan(quad_file);
    if (scan.overrun) {
        throw TruncatedError(kReader, *scan.overrun, 2 + quad_file[*scan.overrun + 1],
                             quad_file.size() - *scan.overrun);
    }
    std::vector<QuadFunction> functions;
    for (const quad_dump::Record& rec : scan.records) {
        if (rec.opcode == quad_dump::kFuncBegin) functions.emplace_back();
        if (!functions.empty()) functions.back().push_back({rec.opcode, Bytes(rec.payload.begin(), rec.payload.end())});
    }
    return functions;
}

std::vector<ObjectFunction> parse_obj_funcs(ByteView object) {
    const omf::Walk walk = omf::walk(object);
    std::vector<std::string> lnames = {""};   // OMF names are numbered from 1
    std::vector<std::string> segments;
    Bytes code;
    struct Public {
        std::string name;
        std::uint32_t offset;
    };
    std::vector<Public> publics;

    for (const omf::RawRecord& rec : walk.records) {
        io::ByteReader r(rec.payload);
        switch (rec.type) {
            case kLnames:
                while (!r.eof()) lnames.emplace_back(text::from_latin1(r.bytes(r.u8())));
                break;
            case kSegdef:
            case kSegdef32: {
                // attribute byte, then the segment length (2 bytes; 4 in the 32-bit record), then the name index
                r.skip(rec.type == kSegdef32 ? 5 : 3);
                const std::uint32_t name = read_index(r);
                segments.push_back(name < lnames.size() ? lnames[name] : std::to_string(name));
                break;
            }
            case kLedata:
            case kLedata32: {
                const std::uint32_t seg = read_index(r);
                const std::uint32_t at = rec.type == kLedata32 ? r.u32() : r.u16();
                const ByteView data = r.bytes(r.remaining());
                if (seg == 0 || seg > segments.size() || segments[seg - 1] != kCodeSegment) break;
                if (static_cast<std::uint64_t>(at) + data.size() > kMaxSegment) {
                    throw FormatError(kReader, rec.offset, "a data record places bytes past 16 MiB of the segment");
                }
                if (at + data.size() > code.size()) code.resize(at + data.size(), 0);
                std::copy(data.begin(), data.end(), code.begin() + at);
                break;
            }
            case kPubdef:
            case kPubdef32: {
                read_index(r);                      // base group
                if (read_index(r) == 0) r.skip(2);  // no base segment: an absolute frame number follows
                while (!r.eof()) {
                    std::string name = text::from_latin1(r.bytes(r.u8()));
                    const std::uint32_t at = rec.type == kPubdef32 ? r.u32() : r.u16();
                    read_index(r);   // type index
                    publics.push_back({std::move(name), at});
                }
                break;
            }
            default:
                break;
        }
    }
    if (walk.error) throw FormatError(kReader, object.size(), *walk.error);

    std::stable_sort(publics.begin(), publics.end(), [](const Public& a, const Public& b) { return a.offset < b.offset; });
    std::vector<ObjectFunction> out;
    for (std::size_t i = 0; i < publics.size(); ++i) {
        // a public past the end of the code, or an end before the start, has no bytes
        const std::size_t start = std::min<std::size_t>(publics[i].offset, code.size());
        const std::size_t end = i + 1 < publics.size() ? std::min<std::size_t>(publics[i + 1].offset, code.size()) : code.size();
        out.push_back({publics[i].name, publics[i].offset, Bytes(code.begin() + start, code.begin() + std::max(start, end))});
    }
    return out;
}

std::vector<Aligned> align(const std::vector<QuadFunction>& quads, const std::vector<ObjectFunction>& functions) {
    // The bytes a function starts with: push bp, push bx, mov r,r/m, mov ax,imm.
    auto looks_like_function = [](const Bytes& code) {
        return !code.empty() && (code[0] == 0x55 || code[0] == 0x53 || code[0] == 0x8b || code[0] == 0xb8);
    };
    std::vector<Aligned> out;
    for (const ObjectFunction& f : functions) {
        if (!looks_like_function(f.code)) continue;
        Aligned a{f.name, f.offset, f.code, {}};
        if (out.size() < quads.size()) a.quads = quads[out.size()];
        out.push_back(std::move(a));
    }
    return out;
}

std::string format_alignment(std::size_t quad_functions, const std::vector<Aligned>& aligned) {
    std::string out = std::format("quad funcs: {} | obj funcs: {}\n", quad_functions, aligned.size());
    for (const Aligned& a : aligned) {
        std::string ops, code;
        for (const Quad& q : a.quads) ops += std::format("{}{:02x}", ops.empty() ? "" : " ", q.opcode);
        for (const std::uint8_t b : a.code) code += std::format("{}{:02x}", code.empty() ? "" : " ", b);
        out += std::format("\n=== {}  ({} bytes) ===\n  quads: {}\n  code : {}\n", a.name, a.code.size(), ops, code);
    }
    return out;
}

std::string report(ByteView quad_file, ByteView object) {
    const std::vector<QuadFunction> quads = parse_quads(quad_file);
    return format_alignment(quads.size(), align(quads, parse_obj_funcs(object)));
}

}  // namespace cb::invert::quad_align
