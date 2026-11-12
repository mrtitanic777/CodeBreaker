// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/vxd/coalesce.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::source::vxd::coalesce {

namespace le = formats::le;

namespace {

constexpr std::string_view kReader = "vxd.finalize";

// Fix-up record bits (the LE format's).
constexpr std::uint8_t kSourceList = 0x20;          // source type: a list of sources follows
constexpr std::uint8_t kSourceKindMask = 0x0F;
constexpr std::uint8_t kSourceSelector = 0x02;      // a 16-bit selector has no offset in its target
constexpr std::uint8_t kTargetKindMask = 0x03;
constexpr std::uint8_t kTargetInternal = 0, kTargetImportOrdinal = 1;
constexpr std::uint8_t kFlagAdditive = 0x04;
constexpr std::uint8_t kFlagOffset32 = 0x10;        // the target offset is 32 bits
constexpr std::uint8_t kFlagAddend32 = 0x20;        // the additive field is 32 bits
constexpr std::uint8_t kFlagWideIndex = 0x40;       // the object / module number is 16 bits
constexpr std::uint8_t kFlagOrdinal8 = 0x80;        // an import ordinal is 8 bits
constexpr std::size_t kMostSources = 255;           // the count of a source list is a byte

// Fields of the LE header that are moved or copied (offsets from the header; those marked ABSOLUTE are file offsets).
constexpr std::uint32_t kHdrEntryObject = 0x18, kHdrEntryEip = 0x1C;
constexpr std::uint32_t kHdrResources = 0x50, kHdrResidentNames = 0x58, kHdrEntryTable = 0x5C, kHdrDirectives = 0x60, kHdrDirectiveCount = 0x64;
constexpr std::uint32_t kHdrFixupPages = 0x68, kHdrFixupRecords = 0x6C, kHdrImportModules = 0x70, kHdrImportNames = 0x78;
constexpr std::uint32_t kHdrDataPages = 0x80;       // ABSOLUTE
constexpr std::uint32_t kHdrNonResidentOffset = 0x88, kHdrNonResidentSize = 0x8C;   // ABSOLUTE
constexpr std::uint32_t kHdrWinResOffset = 0xB8, kHdrWinResSize = 0xBC;             // ABSOLUTE

std::uint32_t u32_at(ByteView data, std::uint64_t at) {
    io::ByteReader r(data, kReader);
    r.seek(at);
    return r.u32();
}

void put_u32(Bytes& data, std::uint64_t at, std::uint64_t value) {
    if (value > 0xFFFFFFFFull) throw FormatError(kReader, at, std::format("0x{:x} does not fit in a header field", value));
    io::checked_range(data, at, 4);   // FormatError when the field is not in the file
    for (unsigned i = 0; i < 4; ++i) data[at + i] = static_cast<std::uint8_t>(value >> (8 * i));
}

// The sources of a page's records that share everything else, in the order the first of them appeared.
struct RecordKey {
    std::uint8_t source = 0, flags = 0;
    Bytes target, addend;
    friend auto operator<=>(const RecordKey&, const RecordKey&) = default;
};

}  // namespace

Page parse_page(ByteView page) {
    Page out;
    io::ByteReader r(page, kReader);
    while (!r.eof()) {
        const std::size_t start = r.pos();
        PageRecord record;
        const std::uint8_t source = r.u8();
        record.flags = r.u8();
        const bool list = (source & kSourceList) != 0;
        std::size_t count = 1;
        std::int16_t single = 0;
        if (list) count = r.u8();
        else single = r.i16();
        const std::size_t target_at = r.pos();
        const std::uint8_t kind = record.flags & kTargetKindMask;
        if (kind == kTargetInternal) {
            r.skip(record.flags & kFlagWideIndex ? 2 : 1);
            if ((source & kSourceKindMask) != kSourceSelector) r.skip(record.flags & kFlagOffset32 ? 4 : 2);
        } else if (kind == kTargetImportOrdinal) {
            r.skip(record.flags & kFlagWideIndex ? 2 : 1);
            r.skip(record.flags & kFlagOrdinal8 ? 1 : record.flags & kFlagOffset32 ? 4 : 2);
        } else {   // an import by name, an entry: kept as it is, with everything after it
            out.rest.assign(page.begin() + static_cast<std::ptrdiff_t>(start), page.end());
            return out;
        }
        const ByteView target = page.subspan(target_at, r.pos() - target_at);
        record.target.assign(target.begin(), target.end());
        if (record.flags & kFlagAdditive) {
            const ByteView addend = r.bytes(record.flags & kFlagAddend32 ? 4 : 2);
            record.addend.assign(addend.begin(), addend.end());
        }
        if (list) {
            for (std::size_t i = 0; i < count; ++i) record.sources.push_back(r.i16());
        } else {
            record.sources.push_back(single);
        }
        record.source = static_cast<std::uint8_t>(source & ~kSourceList);
        out.records.push_back(std::move(record));
    }
    return out;
}

Bytes emit_record(const PageRecord& record) {
    io::ByteWriter w;
    if (record.sources.size() == 1) {
        w.u8(record.source);
        w.u8(record.flags);
        w.i16(record.sources.front());
        w.bytes(record.target);
        w.bytes(record.addend);
        return w.take();
    }
    // a source list: at least one record, and one more for each 255 sources
    std::size_t done = 0;
    do {
        const std::size_t n = std::min(kMostSources, record.sources.size() - done);
        w.u8(static_cast<std::uint8_t>(record.source | kSourceList));
        w.u8(record.flags);
        w.u8(static_cast<std::uint8_t>(n));
        w.bytes(record.target);
        w.bytes(record.addend);
        for (std::size_t i = 0; i < n; ++i) w.i16(record.sources[done + i]);
        done += n;
    } while (done < record.sources.size());
    return w.take();
}

Bytes coalesce(ByteView vxd) {
    const le::Module module = le::Module::from_buffer(Bytes(vxd.begin(), vxd.end()));
    const le::Header& h = module.header();
    const std::uint64_t page_table = module.lfa() + h.fixup_pages, records = module.lfa() + h.fixup_records;
    const std::uint32_t pages = h.pages;

    io::ByteReader table(vxd, kReader);
    table.seek(page_table);
    std::vector<std::uint32_t> page_offsets;
    for (std::uint32_t i = 0; i <= pages; ++i) page_offsets.push_back(table.u32());

    std::vector<std::uint32_t> new_offsets{0};
    Bytes new_records;
    for (std::uint32_t page = 0; page < pages; ++page) {
        const std::uint32_t from = page_offsets[page], to = page_offsets[page + 1];
        if (to < from) throw FormatError(kReader, page_table + 4ull * page, std::format("fix-up page {} ends before it starts", page + 1));
        const Page parsed = parse_page(io::checked_range(vxd, records + from, to - from));
        // Records with the same everything but the sources become one record. (The part the parse left alone comes first: it
        // is kept in one piece and the records before it are written after it, as the first version of this code did.)
        std::map<RecordKey, std::size_t> index;   // lookup only
        std::vector<PageRecord> merged;
        for (const PageRecord& r : parsed.records) {
            const RecordKey key{r.source, r.flags, r.target, r.addend};
            const auto [at, added] = index.try_emplace(key, merged.size());
            if (added) merged.push_back({r.source, r.flags, r.target, r.addend, {}});
            merged[at->second].sources.insert(merged[at->second].sources.end(), r.sources.begin(), r.sources.end());
        }
        append(new_records, parsed.rest);
        for (const PageRecord& r : merged) append(new_records, emit_record(r));
        new_offsets.push_back(static_cast<std::uint32_t>(new_records.size()));
    }

    const std::uint64_t new_total = 4ull * (pages + 1) + new_records.size();
    const std::uint64_t data_pages = h.data_pages;
    if (data_pages < page_table || new_total > data_pages - page_table) throw UnsupportedError(kReader, page_table, "the coalesced fix-ups are larger than the original");
    if (records + new_records.size() > data_pages || data_pages > vxd.size()) throw FormatError(kReader, records, "the fix-up tables do not lie before the data pages");

    Bytes b(vxd.begin(), vxd.end());
    for (std::size_t i = 0; i < new_offsets.size(); ++i) put_u32(b, page_table + 4 * i, new_offsets[i]);
    std::copy(new_records.begin(), new_records.end(), b.begin() + static_cast<std::ptrdiff_t>(records));
    std::fill(b.begin() + static_cast<std::ptrdiff_t>(records + new_records.size()), b.begin() + static_cast<std::ptrdiff_t>(data_pages), std::uint8_t{0});   // harmless slack
    return b;
}

Bytes restore_entry_point(ByteView vxd, const le::Module& original) {
    const le::Module module = le::Module::from_buffer(Bytes(vxd.begin(), vxd.end()));
    Bytes b(vxd.begin(), vxd.end());
    put_u32(b, module.lfa() + kHdrEntryObject, original.header().eip_object);
    put_u32(b, module.lfa() + kHdrEntryEip, original.header().eip);
    return b;
}

Bytes restore_metadata(ByteView vxd, const le::Module& original) {
    const le::Module module = le::Module::from_buffer(Bytes(vxd.begin(), vxd.end()));
    const Bytes unchanged(vxd.begin(), vxd.end());
    const std::int64_t L = static_cast<std::int64_t>(module.lfa()), rL = static_cast<std::int64_t>(original.lfa());
    const ByteView od = original.data();
    const std::int64_t o_res = u32_at(vxd, L + kHdrResidentNames), o_fxp = u32_at(vxd, L + kHdrFixupPages), o_dp = u32_at(vxd, L + kHdrDataPages);
    const std::int64_t r_res = u32_at(od, rL + kHdrResidentNames), r_ent = u32_at(od, rL + kHdrEntryTable), r_fxp = u32_at(od, rL + kHdrFixupPages);
    // The header offsets up to 0x78 are relative to the LE header; the data-pages offset (0x80) is a FILE offset: the object
    // data begins at o_dp, not at L + o_dp. Mixing the two would fold the first bytes of object data into the moved block
    // and shift every object (the relocation breaks).
    if (!(r_fxp > r_ent && r_ent > r_res && r_res > 0 && o_fxp > o_res && o_res > 0 && o_dp > L + o_fxp)) return unchanged;   // no recoverable loader tables
    if (o_res != r_res) return unchanged;   // the object table and page map differ in size: not a driver that matches
    const std::int64_t delta = o_fxp - r_fxp;   // > 0: our tables are longer than the original's (shrink); < 0: shorter (grow)
    if (o_dp > static_cast<std::int64_t>(vxd.size())) throw FormatError(kReader, kHdrDataPages, "the data pages begin past the end of the file");

    const ByteView blob = io::checked_range(od, rL + r_res, r_fxp - r_res);                    // the original's resource, resident-name and entry tables
    const ByteView tables = io::checked_range(vxd, L + o_fxp, o_dp - (L + o_fxp));             // our fix-up page and record tables, and what follows them
    Bytes pre(vxd.begin(), vxd.begin() + static_cast<std::ptrdiff_t>(L + o_res));
    append(pre, blob);
    append(pre, tables);
    if (static_cast<std::int64_t>(pre.size()) < o_dp) {
        pre.resize(static_cast<std::size_t>(o_dp), 0);   // shrink: the freed bytes are zeros before the data pages
    } else if (static_cast<std::int64_t>(pre.size()) > o_dp) {
        // grow: the extra bytes take the slack between the fix-up records and the data pages; anything but zeros there is not slack
        if (!std::all_of(pre.begin() + static_cast<std::ptrdiff_t>(o_dp), pre.end(), [](std::uint8_t byte) { return byte == 0; })) return unchanged;
        pre.resize(static_cast<std::size_t>(o_dp));
    }
    Bytes out = std::move(pre);
    out.insert(out.end(), vxd.begin() + static_cast<std::ptrdiff_t>(o_dp), vxd.end());

    for (std::uint32_t field = kHdrResources; field <= kHdrDirectiveCount; field += 4) put_u32(out, L + field, u32_at(od, rL + field));   // resources, names, entries, directives
    put_u32(out, L + kHdrFixupPages, static_cast<std::uint64_t>(r_fxp));   // the fix-up page table is now where the original has it
    const auto slid = [&](std::uint32_t field) {   // the fix-up records and the import tables slid by `delta`
        const std::int64_t moved = static_cast<std::int64_t>(u32_at(vxd, L + field)) - delta;
        if (moved < 0) throw FormatError(kReader, L + field, "the table would slide before the start of the header");
        put_u32(out, L + field, static_cast<std::uint64_t>(moved));
    };
    slid(kHdrFixupRecords);
    slid(kHdrImportModules);
    slid(kHdrImportNames);
    return out;
}

Bytes restore_version_resource(ByteView vxd, const le::Module& original) {
    const le::Module module = le::Module::from_buffer(Bytes(vxd.begin(), vxd.end()));
    Bytes b(vxd.begin(), vxd.end());
    const std::uint64_t L = module.lfa(), rL = original.lfa();
    if (u32_at(vxd, L + kHdrWinResSize) != 0) return b;   // it has one
    const ByteView od = original.data();
    const std::uint64_t nr_at = u32_at(od, rL + kHdrNonResidentOffset), nr_size = u32_at(od, rL + kHdrNonResidentSize);
    const std::uint64_t wr_at = u32_at(od, rL + kHdrWinResOffset), wr_size = u32_at(od, rL + kHdrWinResSize);
    const auto section = [&](std::uint64_t at, std::uint64_t size) {
        return at != 0 && size != 0 && at + size <= od.size() ? ByteView(od).subspan(at, size) : ByteView();
    };
    const ByteView non_resident = section(nr_at, nr_size), win_res = section(wr_at, wr_size);
    if (non_resident.empty() && win_res.empty()) return b;
    // both offsets are FILE offsets, so they point at the bytes appended here; the object data and fix-ups are untouched
    if (!non_resident.empty()) {
        put_u32(b, L + kHdrNonResidentOffset, b.size());
        put_u32(b, L + kHdrNonResidentSize, non_resident.size());
        append(b, non_resident);
    }
    if (!win_res.empty()) {
        put_u32(b, L + kHdrWinResOffset, b.size());
        put_u32(b, L + kHdrWinResSize, win_res.size());
        append(b, win_res);
    }
    return b;
}

Finalized finalize(ByteView vxd, const le::Module& original) {
    Finalized out;
    out.bytes.assign(vxd.begin(), vxd.end());
    // Each fix is best effort: a failure of one is recorded and the next one runs on what there is. (This is an isolation
    // boundary: a module that defeats a step must still get the others.)
    const auto step = [&](std::string_view name, auto&& fix) {
        try {
            out.bytes = fix(ByteView(out.bytes));
        } catch (const Error& e) {
            out.skipped.push_back(std::format("{}: {}", name, e.what()));
            log::debug(std::format("vxd finalize: {} skipped: {}", name, e.what()));
        }
    };
    step("coalesce", [](ByteView v) { return coalesce(v); });
    step("restore_metadata", [&](ByteView v) { return restore_metadata(v, original); });
    step("restore_version_resource", [&](ByteView v) { return restore_version_resource(v, original); });
    step("restore_entry_point", [&](ByteView v) { return restore_entry_point(v, original); });
    return out;
}

int main(std::span<const std::string> args) {
    const cli::Command command{
        "finalize",
        "what the loader of Windows 95 needs of a linked driver that the linkers leave out",
        {cli::Positional{"built", "the linked driver"},
         cli::Positional{"original", "the original driver, the source of the loader metadata"},
         cli::Positional{"out", "where to write the finished driver"}},
        [](const cli::Args& a) -> int {
            const Bytes built = fs::read_file(fs::from_text(a.required("built")));
            const le::Module original = le::Module::open(fs::from_text(a.required("original")));
            const Finalized done = finalize(built, original);
            fs::write_file(fs::from_text(a.required("out")), done.bytes);
            for (const std::string& why : done.skipped) cb::err("skipped {}\n", why);
            cb::out("wrote {} ({} bytes)\n", a.required("out"), done.bytes.size());
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("finalize", {command});
    std::vector<std::string> line = {"finalize"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::coalesce
