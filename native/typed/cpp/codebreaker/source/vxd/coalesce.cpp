// codebreaker/source/vxd/coalesce.cpp -- typed C++ of codebreaker.source.vxd.coalesce (see coalesce.hpp).

#include "codebreaker/source/vxd/coalesce.hpp"

namespace codebreaker::source::vxd::coalesce {

namespace {

// d[i] of a bytes / bytearray: IndexError past the end
int64_t at_(const py::bytes& d, int64_t i) {
    if (i < 0 || i >= d.size()) throw py::IndexError("bytearray index out of range");
    return d.byte_(i);
}

// struct.unpack_from("<I", buf, base + off)[0]
int64_t U_(const py::bytes& buf, int64_t base, int64_t off) { return py::unpack_from1<int64_t>("<I", buf, base + off); }

// bytes(b) of a bytearray
py::bytes frozen_(const py::bytearray& b) { return b.view(); }

}  // namespace

std::tuple<py::list<PageRecord>, int64_t> _parse_page_records(const py::bytes& d, int64_t p, int64_t end) {
    py::list<PageRecord> recs;
    while (p < end) {
        const int64_t start = p;
        const int64_t src = at_(d, p);
        const int64_t tflags = at_(d, p + 1);
        p += 2;
        const bool srclist = (src & 0x20) != 0;
        int64_t cnt;
        std::optional<int64_t> soff_single;
        if (srclist) {
            cnt = at_(d, p);
            p += 1;
        } else {
            cnt = 1;
            soff_single = py::unpack_from1<int64_t>("<h", d, p);
            p += 2;
        }
        const int64_t tt = tflags & 0x03;
        const int64_t tstart = p;
        if (tt == 0x00) {
            p += (tflags & 0x40) ? 2 : 1;
            if ((src & 0x0f) == 0x02) {
            } else if (tflags & 0x10) {
                p += 4;
            } else {
                p += 2;
            }
        } else if (tt == 0x01) {
            p += (tflags & 0x40) ? 2 : 1;
            if (tflags & 0x80) {
                p += 1;
            } else if (tflags & 0x10) {
                p += 4;
            } else {
                p += 2;
            }
        } else {
            PageRecord r;
            r.verbatim = d.slice(start, end);
            recs.append(r);
            return {recs, end};
        }
        const py::bytes tbytes = d.slice(tstart, p);
        py::bytes add;
        if (tflags & 0x04) {
            const int64_t n = (tflags & 0x20) ? 4 : 2;
            add = d.slice(p, p + n);
            p += n;
        }
        py::list<int64_t> soffs;
        if (srclist) {
            for (int64_t j = 0; j < cnt; ++j) soffs.append(py::unpack_from1<int64_t>("<h", d, p + j * 2));
            p += cnt * 2;
        } else {
            soffs.append(*soff_single);
        }
        PageRecord r;
        r.src = src & ~static_cast<int64_t>(0x20);
        r.tflags = tflags;
        r.tbytes = tbytes;
        r.add = add;
        r.soffs = soffs;
        recs.append(r);
    }
    return {recs, p};
}

py::bytes _emit_record(int64_t src_base, int64_t tflags, const py::bytes& tbytes, const py::bytes& add, const py::list<int64_t>& soffs) {
    // (bytes([...]) raises ValueError for a value past 255, which a source count can reach)
    if (soffs.size() == 1)
        return py::bytes::from_list(std::vector<int64_t>{src_base, tflags}) + py::pack("<h", soffs[0]) + tbytes + add;
    py::bytes out = py::bytes::from_list(std::vector<int64_t>{src_base | 0x20, tflags, soffs.size()}) + tbytes + add;
    std::string tail;
    for (const int64_t s : soffs) tail += py::pack("<h", s).raw();
    return out + py::bytes(tail);
}

py::bytes coalesce(const py::bytes& vxd_bytes) {
    le::LEFile m = le::LEFile::from_buffer(vxd_bytes);
    py::bytearray b(vxd_bytes);
    const int64_t fp = m.fixup_page, fr = m.fixup_rec;
    std::vector<int64_t> pageoffs;
    for (int64_t i = 0; i < m.mpages + 1; ++i) pageoffs.push_back(py::unpack_from1<int64_t>("<I", b, fp + i * 4));
    std::vector<int64_t> new_pageoffs{0};
    std::string new_records;
    for (int64_t pg = 0; pg < m.mpages; ++pg) {
        auto [recs, _end] = _parse_page_records(b, fr + pageoffs[static_cast<size_t>(pg)], fr + pageoffs[static_cast<size_t>(pg) + 1]);
        (void)_end;
        using Key = std::tuple<int64_t, int64_t, py::bytes, py::bytes>;
        py::dict<Key, py::list<int64_t>> groups;
        std::vector<Key> order;
        std::string page_out;
        for (const PageRecord& r : recs) {
            if (r.verbatim) {
                page_out += r.verbatim->raw();
                continue;
            }
            const Key key(r.src, r.tflags, r.tbytes, r.add);
            if (!groups.contains(key)) {
                groups.set(key, py::list<int64_t>());
                order.push_back(key);
            }
            groups.at(key).extend(r.soffs);
        }
        for (const Key& key : order)
            page_out += _emit_record(std::get<0>(key), std::get<1>(key), std::get<2>(key), std::get<3>(key), groups.at(key)).raw();
        new_records += page_out;
        new_pageoffs.push_back(static_cast<int64_t>(new_records.size()));
    }
    const int64_t new_total = (m.mpages + 1) * 4 + static_cast<int64_t>(new_records.size());
    if (new_total > m.datapages - fp) throw py::RuntimeError("coalesced fixups larger than original");
    for (size_t i = 0; i < new_pageoffs.size(); ++i) py::pack_into("<I", b, fp + static_cast<int64_t>(i) * 4, new_pageoffs[i]);
    b.set_slice(fr, fr + static_cast<int64_t>(new_records.size()), py::bytes(new_records));
    for (int64_t k = fr + static_cast<int64_t>(new_records.size()); k < m.datapages; ++k) b[k] = 0;
    return frozen_(b);
}

py::bytes restore_entry_point(const py::bytes& vxd_bytes, const le::LEFile& v) {
    py::bytearray b(vxd_bytes);
    le::LEFile m = le::LEFile::from_buffer(frozen_(b));
    py::pack_into("<I", b, m.lfa + 0x18, v.eip_obj);
    py::pack_into("<I", b, m.lfa + 0x1c, v.eip);
    return frozen_(b);
}

py::bytes restore_metadata(const py::bytes& vxd_bytes, const le::LEFile& v) {
    py::bytearray b(vxd_bytes);
    le::LEFile m = le::LEFile::from_buffer(frozen_(b));
    const int64_t L = m.lfa, rL = v.lfa;
    const py::bytes& vd = v.d();
    const int64_t o_res = U_(b, L, 0x58), o_fxp = U_(b, L, 0x68), o_dp = U_(b, L, 0x80);
    const int64_t r_res = U_(vd, rL, 0x58), r_ent = U_(vd, rL, 0x5c), r_fxp = U_(vd, rL, 0x68);
    // 0x40..0x78 are lfa-relative; the data-pages offset (0x80) is FILE-ABSOLUTE -- object data begins at
    // file offset o_dp, not L+o_dp. Mixing the two frames would fold the first 0x80 bytes of object data
    // into the moved table block and shift every object left (relocation breaks).
    if (!(r_fxp > r_ent && r_ent > r_res && r_res > 0 && o_fxp > o_res && o_res > 0 && o_dp > L + o_fxp))
        return frozen_(b);                                     // no recoverable loader tables -> leave as built
    if (o_res != r_res) return frozen_(b);                     // objtab/pagemap sizes differ (not diff==0) -> skip
    const int64_t delta = o_fxp - r_fxp;   // >0 when our tables are longer than retail's (shrink), <0 when
    //                                        shorter (grow -- the LINK-2.60 compile lane emits a more compact
    //                                        resident-name/entry region than retail, so the region must GROW).
    const py::bytes blob = vd.slice(rL + r_res, rL + r_fxp);                  // retail resource+resident-name+entry region
    const py::bytes tables = b.view().slice(L + o_fxp, o_dp);                 // our coalesced fixup page/record tables + tail
    py::bytes pre = b.view().slice(0, L + o_res) + blob + tables;
    if (pre.size() < o_dp) {
        pre = pre + py::bytes::zeros(o_dp - pre.size());       // shrink: pad the freed bytes before data pages
    } else if (pre.size() > o_dp) {
        const int64_t over = pre.size() - o_dp;                // grow: absorb it by trimming trailing SLACK
        if (pre.slice(o_dp, pre.size()) != py::bytes::zeros(over)) return frozen_(b);   //  (zero padding between fixup records and data
        pre = pre.slice(0, o_dp);                              //  pages); if the overrun isn't pure slack, bail
    }
    py::bytearray nw(pre + b.view().slice(o_dp, b.size()));
    for (const int64_t off : {0x50, 0x54, 0x58, 0x5c, 0x60, 0x64})   // resource/resident-name/entry/directives ptrs
        py::pack_into("<I", nw, L + off, U_(vd, rL, off));
    py::pack_into("<I", nw, L + 0x68, r_fxp);                  // fixup page table now at retail's offset
    py::pack_into("<I", nw, L + 0x6c, U_(b, L, 0x6c) - delta); // fixup records slid left by delta
    for (const int64_t off : {0x70, 0x78})                     // import module / procedure name tables slid too
        py::pack_into("<I", nw, L + off, U_(b, L, off) - delta);
    return frozen_(nw);
}

py::bytes restore_version_resource(const py::bytes& vxd_bytes, const le::LEFile& v) {
    py::bytearray b(vxd_bytes);
    le::LEFile m = le::LEFile::from_buffer(frozen_(b));
    const int64_t L = m.lfa, rL = v.lfa;
    const py::bytes& vd = v.d();
    if (U_(b, L, 0xbc)) return frozen_(b);                     // already has a version resource -> leave it
    const int64_t nro = U_(vd, rL, 0x88), nrl = U_(vd, rL, 0x8c);
    const int64_t wro = U_(vd, rL, 0xb8), wrl = U_(vd, rL, 0xbc);
    const py::bytes nres = (nro && nrl && nro + nrl <= vd.size()) ? vd.slice(nro, nro + nrl) : py::bytes();
    const py::bytes winres = (wro && wrl && wro + wrl <= vd.size()) ? vd.slice(wro, wro + wrl) : py::bytes();
    if (winres.empty() && nres.empty()) return frozen_(b);
    if (!nres.empty()) {
        py::pack_into("<I", b, L + 0x88, b.size());
        py::pack_into("<I", b, L + 0x8c, nres.size());
        b += nres;
    }
    if (!winres.empty()) {
        py::pack_into("<I", b, L + 0xb8, b.size());
        py::pack_into("<I", b, L + 0xbc, winres.size());
        b += winres;
    }
    return frozen_(b);
}

py::bytes finalize(const py::bytes& vxd_bytes_in, const le::LEFile& v) {
    py::bytes vxd_bytes = vxd_bytes_in;
    try {
        vxd_bytes = coalesce(vxd_bytes);
    } catch (const py::Exception&) {
    }
    try {
        vxd_bytes = restore_metadata(vxd_bytes, v);
    } catch (const py::Exception&) {
    }
    try {
        vxd_bytes = restore_version_resource(vxd_bytes, v);
    } catch (const py::Exception&) {
    }
    try {
        vxd_bytes = restore_entry_point(vxd_bytes, v);
    } catch (const py::Exception&) {
    }
    return vxd_bytes;
}

}  // namespace codebreaker::source::vxd::coalesce
