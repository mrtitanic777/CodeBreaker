// decompile/describe_ne.cpp -- describe16: a 16-bit Windows module to the decompiler (see describe.hpp).
// cb-unity: alone

#include "core/fs.hpp"
#include "decompile/describe_internal.hpp"

namespace cb::decompile::describe {

using namespace detail;
using formats::ne::Entry;
using formats::ne::Segment;

namespace {

constexpr std::string_view kReader = "describe16";

std::filesystem::path native_path(const std::filesystem::path& p) {
    return std::filesystem::absolute(p).lexically_normal().make_preferred();
}

// KERNEL's exports that are plain numbers, the same in every protected-mode Windows (by ordinal, and by name): a huge
// pointer's selector step
std::optional<std::int64_t> kernel_constant(std::string_view name) {
    if (name == "KERNEL_113" || name == "__AHSHIFT") return 3;
    if (name == "KERNEL_114" || name == "__AHINCR") return 8;
    return std::nullopt;
}
// the import's name without its module: KERNEL_n stays whole
std::string bare_name(std::string_view name) {
    const std::string up = text::upper(name);
    if (up.starts_with("KERNEL_")) return up;
    const std::size_t k = up.rfind('.');
    return k == std::string::npos ? up : up.substr(k + 1);
}

void put16(Bytes& img, std::int64_t off, std::int64_t v) {
    if (v < 0 || v > 0xFFFF) throw FormatError(kReader, static_cast<std::uint64_t>(off), "a relocated word does not fit 16 bits");
    img[static_cast<std::size_t>(off)] = static_cast<std::uint8_t>(v & 0xFF);
    img[static_cast<std::size_t>(off) + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
}

std::map<std::uint32_t, Entry> entry_table(const formats::ne::Module& m) {
    std::map<std::uint32_t, Entry> entries;
    for (const Entry& e : m.entries())
        if (e.kind != Entry::Kind::Unused) entries[e.ordinal] = e;
    return entries;
}

// The names of the module's resident and non-resident tables without the first of each (the module's own name and its
// description), in that order.
std::vector<formats::ne::Name> exported_names(const formats::ne::Module& m) {
    std::vector<formats::ne::Name> all;
    const std::vector<formats::ne::Name> resident = m.resident_names(), nonresident = m.nonresident_names();
    if (resident.size() > 1) all.insert(all.end(), resident.begin() + 1, resident.end());
    if (nonresident.size() > 1) all.insert(all.end(), nonresident.begin() + 1, nonresident.end());
    return all;
}

}  // namespace

std::map<Va, std::string> decorated_names(const formats::ne::Module& m) {
    std::map<Va, std::string> out;
    const std::map<std::uint32_t, Entry> entries = entry_table(m);
    for (const formats::ne::Name& nm : exported_names(m)) {
        const auto e = entries.find(nm.ordinal);
        if (e != entries.end() && !text::is_identifier(nm.name)) out.try_emplace(address(e->second.segment, e->second.offset), nm.name);
    }
    return out;
}

std::map<std::string, std::string> import_names16(const formats::ne::Module& m, Knowledge& knowledge) {
    const std::vector<std::string> refs = m.module_refs();
    std::map<std::string, std::optional<std::string>> got;   // the first ask of each symbol stands
    for (const Segment& s : m.segments()) {
        for (const formats::ne::Relocation& r : m.relocations(s)) {
            const auto* by_number = std::get_if<formats::ne::ImportOrdinalRef>(&r.target);
            if (by_number && 0 < by_number->module && by_number->module <= refs.size()) {
                const std::string& mod = refs[by_number->module - 1];
                const std::string symbol = text::sanitize_identifier(mod + "_" + std::to_string(by_number->ordinal));
                if (!got.contains(symbol)) got[symbol] = knowledge.import_name(mod, by_number->ordinal);
            }
        }
    }
    std::map<std::string, std::string> out;
    for (const auto& [symbol, name] : got)
        if (name && !name->empty()) out[symbol] = *name;
    return out;
}

// Write the description (and the relocated image it points at).
NeDescription describe16(const formats::ne::Module& m, const std::filesystem::path& outdir, Knowledge& knowledge, const Policy& policy) {
    const std::vector<Segment> segs = m.segments();
    const std::map<std::uint32_t, Entry> entries = entry_table(m);
    const std::vector<std::string> refs = m.module_refs();
    const ByteView file = m.data();
    std::map<std::int64_t, Bytes> images;
    for (const Segment& s : segs) {
        Bytes data;
        if (s.file_offset != 0 && s.file_offset < file.size()) {   // (cut to the file, as the segment reads elsewhere)
            const ByteView part = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
            data.assign(part.begin(), part.end());
        }
        const std::int64_t size = std::max<std::int64_t>(static_cast<std::int64_t>(data.size()), std::min<std::int64_t>(s.min_alloc, 0x10000));
        data.resize(static_cast<std::size_t>(size), 0);
        images[s.index] = std::move(data);
    }
    std::map<std::string, Va> imports;   // name -> stand-in address
    std::vector<std::string> order;
    auto extern_address = [&](const std::string& name) {
        if (!imports.contains(name)) {
            imports[name] = (kExternSelector << 16) + 16 * static_cast<Va>(order.size());
            order.push_back(name);
        }
        return imports[name];
    };
    std::map<std::string, Va> selectors;                 // import name -> its own made-up selector
    std::map<std::string, SelectorUse> used_as;          // import name -> "segment" | "offset"
    auto selector_of = [&](const std::string& name) {
        if (!selectors.contains(name)) selectors[name] = kImportSelector + static_cast<Va>(selectors.size());
        return selectors[name];
    };

    for (const Segment& s : segs) {
        Bytes& img = images[s.index];
        for (const formats::ne::Relocation& r : m.relocations(s)) {
            Va tsel, toff;
            std::string nm;
            bool is_import = false;
            if (const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target)) {
                Va tseg;
                if (internal->segment == 0xFF) {   // by entry-table ordinal
                    const auto e = entries.find(internal->entry);
                    if (e == entries.end()) continue;
                    tseg = e->second.segment;
                    toff = e->second.offset;
                } else {
                    tseg = internal->segment;
                    toff = internal->entry;
                }
                tsel = selector(tseg);
            } else if (const auto* by_number = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                const std::string mod = (0 < by_number->module && by_number->module <= refs.size()) ? refs[by_number->module - 1] : "?";
                nm = mod + "_" + std::to_string(by_number->ordinal);
                const Va a = extern_address(text::sanitize_identifier(nm));
                tsel = a >> 16;
                toff = a & 0xFFFF;
                is_import = true;
            } else if (const auto* by_name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                nm = m.imported_name(by_name->name_offset);
                const Va a = extern_address(text::sanitize_identifier(nm));
                tsel = a >> 16;
                toff = a & 0xFFFF;
                is_import = true;
            } else {
                continue;   // floating-point emulator fixups
            }
            const std::string nid = is_import ? text::sanitize_identifier(nm) : std::string();
            // Where the same target is needed in several places the file chains them: each place holds the offset of the next,
            // ending at 0xFFFF. An "additive" record is one place.
            std::int64_t off = r.offset, hops = 0;
            const std::int64_t len = static_cast<std::int64_t>(img.size());
            while (off != 0xFFFF && off + 1 < len && hops < 0x10000) {
                const std::int64_t nxt = word_at(img, off);
                const int src = r.source;
                const std::int64_t add = r.additive() ? nxt : 0;
                if (src == 3 && off + 3 < len) {   // far pointer: offset, selector
                    put16(img, off, (toff + add) & 0xFFFF);
                    put16(img, off + 2, tsel);
                } else if (src == 2) {   // selector only
                    // an imported selector is a segment of its own: what is read through it is told apart, and named by its
                    // import (not one shared made-up selector)
                    Va sel;
                    if (is_import) {
                        sel = selector_of(nid);
                        used_as[nid] = SelectorUse::Segment;
                    } else {
                        sel = tsel;
                    }
                    put16(img, off, sel);
                } else if (src == 5 && is_import && kernel_constant(bare_name(nm))) {
                    // a number that is the same in every protected-mode Windows (__AHSHIFT 3, __AHINCR 8: a huge pointer's step):
                    // the number itself -- a made-up one would be folded into the arithmetic as if it were the value
                    put16(img, off, (*kernel_constant(bare_name(nm)) + add) & 0xFFFF);
                } else if (src == 5 && is_import) {
                    // an import's offset alone: a value KERNEL exports (__0040H, __WINFLAGS ...), which the loader writes in. A value
                    // of its own, so it is told apart
                    put16(img, off, (selector_of(nid) + add) & 0xFFFF);
                    used_as.try_emplace(nid, SelectorUse::Offset);
                } else if (src == 5) {   // offset only
                    put16(img, off, (toff + add) & 0xFFFF);
                } else if (src == 0) {   // low byte
                    img[static_cast<std::size_t>(off)] = static_cast<std::uint8_t>((toff + add) & 0xFF);
                }
                if (r.additive()) break;
                off = nxt;
                ++hops;
            }
        }
    }

    const std::filesystem::path blob = native_path(outdir / "describe.bin");
    std::vector<std::string> lines;
    lines.push_back("arch\t" + std::string(kArchNe));
    Bytes blob_bytes;
    for (const Segment& s : segs) {
        const Bytes& img = images[s.index];
        if (img.empty()) continue;
        lines.push_back(std::format("bytes\t{}\t{}\t{}\t{}", hx(address(s.index, 0)), blob.string(), hx(static_cast<Va>(blob_bytes.size())), hx(static_cast<Va>(img.size()))));
        append(blob_bytes, img);
    }
    fs::write_file(blob, blob_bytes);
    const std::int64_t data_seg = m.header().autodata_segment;
    if (data_seg) lines.push_back("reg\tDS\t" + hx(selector(data_seg)));
    std::map<SegOff, std::string> exported;   // lookup only
    for (const formats::ne::Name& nm : exported_names(m)) {
        const auto e = entries.find(nm.ordinal);
        if (e != entries.end() && text::is_identifier(nm.name)) exported.try_emplace(SegOff{e->second.segment, e->second.offset}, nm.name);
    }
    NeDescription d;
    d.policy = policy;
    std::vector<std::tuple<Va, std::string, Distance>> funcs;
    // A far call whose offset the linker wrote and whose selector the loader fills in (a fix-up of the selector word alone)
    // calls a far function: `call far seg:off` in the relocated code.
    std::map<Va, std::int64_t> code_sel;
    for (const Segment& s : segs)
        if (!s.is_data()) code_sel[selector(s.index)] = s.index;
    for (const Segment& s : segs) {
        if (s.is_data()) continue;
        const Bytes& img = images[s.index];
        for (std::size_t i = 0; i + 4 < img.size(); ++i) {
            if (img[i] != 0x9A) continue;
            const std::int64_t o = word_at(img, static_cast<std::int64_t>(i) + 1), sl = word_at(img, static_cast<std::int64_t>(i) + 3);
            if (const auto cs = code_sel.find(sl); cs != code_sel.end())
                d.far_called.insert(SegOff{static_cast<std::uint32_t>(cs->second), static_cast<std::uint32_t>(o)});
        }
    }
    for (const auto& [place, found] : find_functions(m)) {   // (ascending: segment, then offset)
        const Distance dist = d.far_called.contains(place) ? Distance::Far : found;
        const Va va = address(place.seg, place.off);
        const auto ex = exported.find(place);
        std::string nm = ex != exported.end() ? ex->second : std::string();
        if (nm.empty() || imports.contains(nm)) nm = std::format("sub_{}_{}", place.seg, hex_upper(place.off, 4));
        d.names[va] = nm;
        d.where_map[va] = place;
        funcs.emplace_back(va, nm, dist);
    }
    d.imports = order;
    d.import_names = import_names16(m, knowledge);   // symbol -> the import's real name
    for (const auto& [name, sel] : selectors) {      // made-up value -> (import symbol, its selector or its offset)
        const auto use = used_as.find(name);
        d.selector_imports[sel] = {name, use == used_as.end() ? SelectorUse::Segment : use->second};
    }
    for (const Segment& s : segs)
        if (!s.is_data()) d.code[s.index] = s.length;   // segment -> bytes of code
    d.images = images;                                    // segment -> its bytes, relocated
    d.data_segment = data_seg;
    // (as the loader has them: the stored bytes, then zeros to the size it allocates -- a function's last instruction can end in
    // them: `retf 4` stored as CA 04, its 00 not stored)
    for (const Segment& s : segs) {
        if (!s.is_data() && s.file_offset != 0 && s.file_offset < file.size()) {
            const std::int64_t alloc = std::min<std::int64_t>(s.min_alloc ? s.min_alloc : 0x10000, 0x10000);
            const ByteView part = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
            Bytes b(part.begin(), part.end());
            b.resize(b.size() + static_cast<std::size_t>(std::max<std::int64_t>(0, alloc - static_cast<std::int64_t>(s.length))), 0);
            d.segment_bytes[s.index] = std::move(b);
        }
    }
    d.results = results(d);
    std::map<std::int64_t, std::vector<std::int64_t>> starts16;
    for (const auto& [va, nm, dist] : funcs) {
        const SegOff w = d.where_map.at(va);
        starts16[w.seg].push_back(w.off);
    }
    for (const auto& [va, nm, dist] : funcs) {
        const SegOff w = d.where_map.at(va);
        std::optional<std::int64_t> later;
        for (std::int64_t o : starts16[w.seg])
            if (o > w.off && (!later || o < *later)) later = o;
        if (dist == Distance::Near && !exported.contains(w) && policy.inline_helpers && register_helper(d.loaded(w.seg), w.off, later)) {
            d.inlined.insert(va);
            lines.push_back(std::format("F\t{}\t{}\t-1\tinline", hx(va), nm));
            continue;
        }
        lines.push_back(model_line(d, va, nm, dist));
    }
    if (policy.call_overrides)
        for (std::string& l : call_overrides(d)) lines.push_back(std::move(l));
    for (std::string& l : jump_tables(d)) lines.push_back(std::move(l));
    // (each return that is a jump -- jump_by_return -- is a call through the address pushed, followed by a return: the
    // decompiler's CALL_RETURN flow override, `R <address>`)
    for (Va va : return_jumps(d)) lines.push_back("R\t" + hx(va));
    for (const std::string& nm : order) {   // Windows API: far, and the callee removes its arguments --
        const std::optional<std::int64_t> size = knowledge.win16_argument_bytes(nm);   // how many bytes is read from the module that exports it
        lines.push_back(std::format("X\t{}\t{}\t{}\t__stdcall16far", hx(imports.at(nm)), nm, size ? *size : -1));
    }
    d.lines = std::move(lines);
    d.write(outdir / "description.txt");
    return d;
}

}  // namespace cb::decompile::describe
