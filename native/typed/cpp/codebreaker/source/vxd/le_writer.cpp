// codebreaker/source/vxd/le_writer.cpp -- typed C++ of codebreaker.source.vxd.le_writer (see le_writer.hpp).

#include "codebreaker/source/vxd/le_writer.hpp"

#include <algorithm>
#include <unordered_map>
#include <variant>
#include <vector>

namespace codebreaker::source::vxd::le_writer {

namespace dis = codebreaker::source::vxd::disassembly;

namespace {

// struct.unpack_from("<I", buf, off)[0]
int64_t U32_(const py::bytes& buf, int64_t off) { return py::unpack_from1<int64_t>("<I", buf, off); }

}  // namespace

int64_t _src_type(const VxDDisasm& D, int64_t oi, int64_t off, const py::set<int64_t>& ends, const py::bytes& code) {
    const dis::Place fk(oi, off);
    if (D.fx.contains(fk) && std::holds_alternative<le::InternalFixup>(D.fx.at(fk))) return std::get<le::InternalFixup>(D.fx.at(fk)).srctype;   // told
    const int64_t n = code.size();
    const bool has1 = 0 <= off - 1 && off - 1 < n, has2 = 0 <= off - 2 && off - 2 < n;
    const int64_t b1 = has1 ? code.byte_(off - 1) : -1;
    const int64_t b2 = has2 ? code.byte_(off - 2) : -1;
    if (D.insns.contains(dis::Place(oi, off - 1))) {
        const dis::Row& r1 = D.insns.at(dis::Place(oi, off - 1));
        if (r1.size == 5 && (b1 == 0xE8 || b1 == 0xE9)) return 0x08;
    }
    if (D.insns.contains(dis::Place(oi, off - 2))) {
        const dis::Row& r2 = D.insns.at(dis::Place(oi, off - 2));
        if (r2.size == 6 && b2 == 0x0F && has1 && 0x80 <= b1 && b1 <= 0x8F) return 0x08;
    }
    if (!ends.contains(off + 4)) {                                 // not a decoded insn tail -> trust the opcode
        if (b1 == 0xE8 || b1 == 0xE9) return 0x08;
        if (b2 == 0x0F && has1 && 0x80 <= b1 && b1 <= 0x8F) return 0x08;
    }
    return 0x07;
}

std::tuple<py::list<std::tuple<int64_t, py::bytes>>, int64_t> _oracle_pages(const le::LEFile& v, std::optional<int64_t> first) {
    const py::bytes& vd = v.d();
    if (!first) first = U32_(vd, v.lfa + 0x80);
    const int64_t ps = v.pagesize;
    const auto pm = le::page_map(v);
    py::list<std::tuple<int64_t, py::bytes>> out;
    int64_t top = 0;
    for (const auto& e : pm) {
        const int64_t n = std::get<0>(e);
        out.append({n, vd.slice(*first + (n - 1) * ps, *first + n * ps)});
        top = std::max(top, n);
    }
    return {out, top};
}

py::dict<int64_t, py::list<py::bytes>> _emit_fixups(const VxDDisasm& D, const le::LEFile& v) {
    std::unordered_map<int64_t, py::set<int64_t>> ends;           // per-object instruction-end offsets
    for (const dis::Place& k : D.insns) ends[std::get<0>(k)].add(std::get<1>(k) + D.insns.at(k).size);
    const int64_t ps = v.pagesize;                                // object bytes from its first page to the end
    const py::bytes& vd = v.d();
    const int64_t o_dp = U32_(vd, v.lfa + 0x80);                  // of the module's data pages -- covers a fixup
    // whose offset overruns the object's own pages into a later object. Derive the end from the page
    // layout (container-absolute), NOT the nres field: W3 bundling leaves nres module-relative, which
    // would make this empty for a bundled module and break every src_type read.
    const int64_t data_end = std::min(o_dp + (v.mpages - 1) * ps + v.last_page_bytes, vd.size());
    // the pages in page-map order (each found by its number), the highest-numbered one cut at data_end
    const auto [found, top] = _oracle_pages(v);
    py::list<py::bytes> logical;
    for (const auto& nb : found) {
        const int64_t n = std::get<0>(nb);
        const py::bytes& b = std::get<1>(nb);
        logical.append(n != top ? b : b.slice(0, std::max<int64_t>(0, data_end - (o_dp + (n - 1) * ps))));
    }
    std::unordered_map<int64_t, py::bytes> code_of;
    for (const le::Object& o : v.objects) {
        std::string joined;
        for (const py::bytes& b : logical.slice(o.pmidx - 1, logical.size())) joined += b.raw();
        code_of[o.idx] = py::bytes(joined);
    }
    // key: (source type, target flags, kind (0 internal, 1 import), target object / module, target offset / ordinal)
    using Key = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>;
    py::dict<int64_t, py::dict<Key, py::list<int64_t>>> by_page;
    const py::bytes none_code;
    const py::set<int64_t> no_ends;
    for (const dis::Place& fk : D.fx) {
        const le::Fixup& f = D.fx.at(fk);
        const int64_t oi = std::get<0>(fk), off = std::get<1>(fk);
        if (std::holds_alternative<le::InternalFixup>(f) && !std::get<le::InternalFixup>(f).target) continue;   // selector-only fixup, no 32-bit field
        const le::Object* obj = nullptr;
        for (const le::Object& o : v.objects)
            if (o.idx == oi) {
                obj = &o;
                break;
            }
        if (!obj || off < 0) continue;
        const int64_t pg = obj->pmidx + py::floordiv(off, v.pagesize);
        const int64_t po = py::mod(off, v.pagesize);
        auto ce = code_of.find(oi);
        auto ee = ends.find(oi);
        const int64_t st = _src_type(D, oi, off, ee != ends.end() ? ee->second : no_ends, ce != code_of.end() ? ce->second : none_code);
        Key key;
        if (std::holds_alternative<le::InternalFixup>(f)) {
            const le::InternalFixup& i = std::get<le::InternalFixup>(f);
            const int64_t tflags = *i.target > 0xffff ? 0x10 : 0x00;
            key = Key(st, tflags, 0, i.obj, *i.target);
        } else {                                                  // import by ordinal
            const le::ImportFixup& m = std::get<le::ImportFixup>(f);
            key = Key(st, 0x01 | 0x80, 1, m.module, m.ordinal & 0xff);
        }
        by_page.setdefault(pg, py::dict<Key, py::list<int64_t>>()).setdefault(key, py::list<int64_t>()).append(po);
        // a 4-byte source that crosses into the next page is ALSO listed there, with a negative offset
        if (po > v.pagesize - 4 && pg < v.mpages)
            by_page.setdefault(pg + 1, py::dict<Key, py::list<int64_t>>()).setdefault(key, py::list<int64_t>()).append(po - v.pagesize);
    }
    py::dict<int64_t, py::list<py::bytes>> records;
    for (int64_t pg = 1; pg <= v.mpages; ++pg) {
        const py::dict<Key, py::list<int64_t>> groups = by_page.get(pg, py::dict<Key, py::list<int64_t>>());
        records.set(pg, py::list<py::bytes>());
        std::vector<std::pair<int64_t, Key>> order;               // sorted(groups, key=lambda k: min(groups[k])), stable
        for (const Key& k : groups) order.emplace_back(*std::min_element(groups.at(k).vec().begin(), groups.at(k).vec().end()), k);
        std::stable_sort(order.begin(), order.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& ok : order) {
            const Key& key = ok.second;
            std::vector<int64_t> offs = groups.at(key).vec();
            std::sort(offs.begin(), offs.end());
            const auto& [st, tflags, kind, a, b] = key;
            for (size_t i = 0; i < offs.size(); i += 255) {       // source count is one byte -> spill past 255
                py::list<int64_t> chunk(std::vector<int64_t>(offs.begin() + static_cast<std::ptrdiff_t>(i),
                                                             offs.begin() + static_cast<std::ptrdiff_t>(std::min(offs.size(), i + 255))));
                le::FixupRecord rec;
                rec.source = st;
                rec.source_flags = chunk.size() > 1 ? 0x20 : 0;
                rec.flags = tflags;
                rec.sources = chunk;
                if (kind == 0) {                                  // target: obj# then 16/32-bit offset
                    rec.kind = py::str("internal");
                    rec.obj = a;
                    rec.target = b;
                } else {                                          // import: module# then 8-bit ordinal
                    rec.kind = py::str("import");
                    rec.module = a;
                    rec.ordinal = b;
                }
                records.at(pg).append(le::encode_fixup(rec));
            }
        }
    }
    return records;
}

py::bytes build_le(const VxDDisasm& D, const le::LEFile& v, const py::str& vxdname) {
    const int64_t ps = v.pagesize;
    const int64_t lfa = 0x80;
    const py::bytes& vd = v.d();
    auto at = [&](int64_t off) { return U32_(vd, lfa + off); };
    const int64_t o_dp = at(0x80);
    // data pages: each recovered object's pages come from D.code, laid out by logical page index.
    // A logical page that no object maps ("gap" page) is not recovered code -- LINK386 left stale
    // buffer bytes there; those are copied from the oracle (the loader ignores them, they are not
    // derivable). The last page holds only last_page_bytes (any tail is BSS, not stored).
    // The oracle's own page bytes are those at each page's NUMBER in its page map (LINK386 numbered
    // some drivers' pages out of order); le.write_image stores each page at its number again.
    py::dict<int64_t, py::bytes> page_content;
    for (const le::Object& o : v.objects)
        for (int64_t k = 0; k < o.pcnt; ++k) page_content.set(o.pmidx + k, D.code.at(o.idx).slice(k * ps, (k + 1) * ps));
    const auto [found, top] = _oracle_pages(v, o_dp);
    py::list<std::tuple<int64_t, py::bytes>> pages;
    int64_t pg = 0;
    for (const auto& np : found) {
        pg += 1;
        const int64_t number = std::get<0>(np);
        const py::bytes& own = std::get<1>(np);
        py::bytes b = page_content.contains(pg) ? page_content.at(pg) : own;   // gap page: orphaned linker bytes
        if (b.size() < ps && number != top)                        // object ends mid-page: the rest of
            b = b + own.slice(b.size(), own.size());               // the page is oracle leftover
        pages.append({0, number == top ? b.slice(0, v.last_page_bytes) : b + py::bytes::zeros(std::max<int64_t>(0, ps - b.size()))});
    }
    // trailing bytes between the last object page and the non-resident name table (LINK386 zero-padding
    // or stale bytes): recovered from the oracle so the whole data region is byte-exact
    int64_t stored = 0;
    for (const auto& p : pages) stored += std::get<1>(p).size();
    int64_t data_end = at(0x88);
    if (!data_end) data_end = at(0xb8);
    if (!data_end) data_end = vd.size();
    const py::bytes names_gap = (0 <= stored && stored < data_end - o_dp) ? vd.slice(o_dp + stored, data_end) : py::bytes();

    const int64_t otab_abs = v.lfa + U32_(vd, v.lfa + 0x40);
    py::list<le::ImageObject> objects;
    int64_t oi = 0;
    for (const le::Object& o : v.objects) {
        le::ImageObject io;
        io.vsize = o.vsize;
        io.base = o.base;
        io.flags = o.flags;
        io.first_page = o.pmidx;
        io.page_count = o.pcnt;
        io.reserved = vd.slice(otab_abs + oi * 24 + 20, otab_abs + oi * 24 + 24);   // 4-char object name (recovered)
        objects.append(io);
        oi += 1;
    }
    // page map: recovered verbatim from the oracle (each 4-byte entry's page number + flag byte are
    // LINK386's; the numbers are a permutation for some modules, and write_image stores each page and
    // its fix-up row at its number)
    const int64_t pm_abs = v.lfa + U32_(vd, v.lfa + 0x48);
    // resident-name table (the module's own name + any exported names): recovered verbatim from the
    // oracle so it is byte-exact regardless of the file name the caller injects under
    const int64_t rn_off = U32_(vd, v.lfa + 0x58);
    const int64_t et_off = U32_(vd, v.lfa + 0x5c);
    le::NameTable resident;
    if (et_off > rn_off && rn_off > 0) resident = vd.slice(v.lfa + rn_off, v.lfa + et_off);
    else resident = le::NameList{{vxdname, 0}};
    // entry table (the export directory: DDB @1 + any further exports): recovered verbatim from the
    // oracle -- the DDB obj/rva it points at is independently verified by the byte-exact object bytes
    const int64_t fp_off = U32_(vd, v.lfa + 0x68);
    py::bytes entrytab;
    if (fp_off > et_off && et_off > 0) {
        entrytab = vd.slice(v.lfa + et_off, v.lfa + fp_off);
    } else {
        const auto& db = D.ddb;
        entrytab = py::pack("<BBH", 1, 0x03, db ? db->obj : 1) + py::pack("<BI", 0x03, db ? db->rva : 0) + py::bytes("\x00", 1);   // flags 0x03: EXPORTED entry
    }
    // loader-section tail after the fixup records -- the import module + procedure name tables (always
    // empty/count 0 in this corpus) plus whatever dead symbol strings LINK386 left in the gap up to the
    // data pages: recovered verbatim from the oracle as the tables' tail (the loader ignores it; it is
    // not derivable), zero-padded up to the data pages. So the import tables are empty, and the fixup
    // section size counts the fixup tables alone.
    const int64_t o_impmod = lfa + at(0x70);
    const py::bytes imp_region = o_dp > o_impmod ? vd.slice(o_impmod, o_dp) : py::bytes();   // empty when impmod == datapages

    py::bytearray H = py::bytearray::zeros(le::HEADER_SIZE);
    py::pack_into("<2sBBI", H, 0, py::bytes("LE"), 0, 0, 0);
    py::pack_into("<HH", H, 0x08, v.cpu, v.ostype);
    py::pack_into("<II", H, 0x18, v.eip_obj, v.eip);
    py::pack_into("<I", H, 0x28, ps);
    // header fields the real Win95 loader reads, preserved from the oracle (module version/flags, the
    // initial ESP, preload count, and the VxD device id + DDK version packed at 0xc0) -- recovered values
    for (const int64_t foff : {0x0c, 0x10, 0x20, 0x24, 0x84, 0x94, 0xc0}) py::pack_into("<I", H, foff, at(foff));
    // non-resident name table (the module description string): preserved, placed after the data pages
    const int64_t nres_off = at(0x88), nres_sz = at(0x8c);
    py::bytes nres = (nres_off && nres_sz && nres_off + nres_sz <= vd.size()) ? vd.slice(nres_off, nres_off + nres_sz) : py::bytes();
    if (!nres.empty()) {
        bool plausible = 0 < nres.byte_(0) && nres.byte_(0) < 64;
        if (plausible)
            for (const int64_t c : nres.slice(1, 1 + nres.byte_(0)))
                if (!((31 < c && c < 127) || c == 9)) {
                    plausible = false;
                    break;
                }
        if (!plausible) nres = py::bytes();                       // not a plausible description (e.g. a W3 module's container-relative offset)
    }
    // Windows resource block (the VxD's version resource): recovered from the oracle and re-pointed.
    // winresoff/winreslen (0xb8/0xbc) locate it; it sits at end-of-file, just past the non-res table.
    const int64_t wro = at(0xb8), wrl = at(0xbc);
    const py::bytes winres = (wro && wrl && wro + wrl <= vd.size()) ? vd.slice(wro, wro + wrl) : py::bytes();
    if (!winres.empty()) py::pack_into("<I", H, 0xbc, winres.size());

    py::bytearray mz = (vd.slice(0, 2) == py::bytes("MZ") && vd.size() >= lfa) ? py::bytearray(vd.slice(0, lfa)) : py::bytearray::zeros(lfa);
    mz.set_slice(0, 2, py::bytes("MZ"));                           // DOS stub preserved from the oracle
    py::pack_into("<I", mz, 0x3c, lfa);
    // the data pages start at the oracle's recorded offset (LINK386's own alignment varies with page
    // size), at a 0x200 alignment only if our loader content would not fit under it; a file that comes
    // out shorter than the oracle ends with the oracle's own end-of-file padding
    le::Image img;
    img.stub = mz.view();
    img.header = H.view();
    img.objects = objects;
    img.pages = pages;
    img.page_map = vd.slice(pm_abs, pm_abs + v.mpages * 4);
    img.last_page_bytes = v.last_page_bytes;
    img.resident = resident;
    img.nonresident = nres;
    img.entry_table = entrytab;
    img.fixups = _emit_fixups(D, v);
    img.tables_tail = imp_region;
    img.align = 0x200;
    img.data_pages = o_dp;
    img.gaps = std::make_tuple(py::bytes(), names_gap);
    img.after = winres;
    img.moved = {0, winres.empty() ? 0 : wro, wro};
    img.fill_from = vd;
    return le::write_image(img);
}

py::bytes build_w3(const VxDDisasm& D, const le::LEFile& v, const py::str& /*name*/) {
    const int64_t L = v.lfa;
    const py::bytes& vd = v.d();
    auto U = [&](int64_t o) { return U32_(vd, L + o); };
    const int64_t ps = v.pagesize;
    const int64_t first = U(0x80);                                // data pages, container-absolute
    // data pages: the container's own in page-map order (each found by its number), each recovered
    // object's bytes laid over them at its first page, in object order (an object whose recovered
    // bytes run past its pages -- its uninitialised tail -- spills into the next page, and the next
    // object is laid over that); le.write_image stores each page at its number again
    const auto [found, top] = _oracle_pages(v);
    std::string joined;
    for (const auto& nb : found) joined += std::get<1>(nb).ljust(ps, py::bytes("\0", 1)).raw();
    py::bytearray data{py::bytes(joined)};
    for (const le::Object& o : v.objects) {
        const int64_t at = (o.pmidx - 1) * ps;
        const py::bytes& cb = D.code.at(o.idx);
        data.set_slice(at, at + cb.size(), cb);
    }
    py::list<std::tuple<int64_t, py::bytes>> pages;
    int64_t i = 0;
    for (const auto& nb : found) {
        const int64_t n = std::get<0>(nb);
        pages.append({0, data.view().slice(i * ps, (i + 1) * ps).slice(0, n == top ? v.last_page_bytes : ps)});
        i += 1;
    }
    const int64_t otab = L + U(0x40);
    py::list<le::ImageObject> objects;
    int64_t oi = 0;
    for (const le::Object& o : v.objects) {
        le::ImageObject io;
        io.vsize = o.vsize;
        io.base = o.base;
        io.flags = o.flags;
        io.first_page = o.pmidx;
        io.page_count = o.pcnt;
        io.reserved = vd.slice(otab + oi * 24 + 20, otab + oi * 24 + 24);
        objects.append(io);
        oi += 1;
    }
    // the loader's tables and the names as the container stores them, but the fix-up tables rebuilt
    // from D.fx; the container's own bytes behind them up to the data pages are kept where they lie
    // (the data pages move to the next 0x200 only when the rebuilt tables run into them). The
    // non-resident names and the version resource sit among the data pages, and move with them.
    const py::dict<int64_t, py::list<py::bytes>> fixups = _emit_fixups(D, v);
    const auto [fix_pages, fix_records] = le::fixup_tables(fixups, v.mpages);
    const int64_t tables_end = L + U(0x68) + fix_pages.size() + fix_records.size();
    const int64_t nres_off = U(0x88), nres_sz = U(0x8c);
    le::Image img;
    img.stub = vd.slice(0, L);
    img.header = vd.slice(L, L + le::HEADER_SIZE);
    img.objects = objects;
    img.pages = pages;
    img.page_map = vd.slice(L + U(0x48), L + U(0x48) + v.mpages * 4);
    img.last_page_bytes = v.last_page_bytes;
    img.resident = vd.slice(L + U(0x58), L + U(0x5c));
    img.entry_table = vd.slice(L + U(0x5c), L + U(0x68));
    img.nonresident = vd.slice(nres_off, nres_off + nres_sz);
    img.fixups = fixups;
    img.tables_tail = vd.slice(tables_end, first);
    img.align = 0x200;
    img.data_pages = first;
    img.gaps = std::make_tuple(py::bytes(), py::bytes());
    img.in_pages = py::dict<py::str, int64_t>{{py::str("nonresident_names"), nres_off}, {py::str("resource_data"), U(0xb8)}};
    img.after = py::bytes();
    img.moved = {U(0x98), 0, vd.size()};
    img.fill_from = vd;
    py::bytearray out{le::write_image(img)};
    // the container's directory, each member's size being its header and loader tables as written
    std::optional<le::Container> c = le::container(vd);
    if (!c) throw py::TypeError("'NoneType' object is not subscriptable");
    auto loader_end = [&](int64_t at) { return U32_(out, at + 0x68) + U32_(out, at + 0x30); };
    py::list<std::tuple<py::str, int64_t, int64_t>> members;
    for (const auto& m : c->members) members.append({std::get<0>(m), std::get<1>(m), std::get<1>(m) == L ? loader_end(std::get<1>(m)) : std::get<2>(m)});
    c->members = members;
    const py::bytes directory = le::pack_directory(*c);
    out.set_slice(c->at, c->at + directory.size(), directory);
    return out.view();
}

}  // namespace codebreaker::source::vxd::le_writer
