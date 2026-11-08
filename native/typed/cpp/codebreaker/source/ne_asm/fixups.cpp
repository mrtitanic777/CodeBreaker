// codebreaker/source/ne_asm/fixups.cpp -- typed C++ of codebreaker.source.ne_asm.fixups (see fixups.hpp).

#include "codebreaker/source/ne_asm/fixups.hpp"

#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::ne_asm::fixups {

namespace ne_fmt = codebreaker::formats::ne;

formats::ne::NEFile open_ne(const py::str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
        f.close();
    }
    if (data.slice(0, 2) == py::bytes("MZ")) {
        return ne_fmt::NEFile(path);
    }
    if (data.slice(0, 2) == py::bytes("NE")) {
        return ne_fmt::NEFile::from_buffer(data, 0, path);
    }
    throw py::ValueError(path.utf8() + ": not MZ or NE");
}

// the name a module reference gets: the module's name, or "mod<N>" when the reference is out of range
static py::str module_label(const py::list<py::str>& mods, int64_t module) {
    if (0 < module && module <= py::len(mods)) {
        return mods[module - 1];
    }
    return py::percent("mod%d", module);
}

py::dict<int64_t, py::str> build_fixup_map(const ne_fmt::NEFile& ne, const ne_fmt::Segment& seg, const py::list<py::str>& mods) {
    const py::bytes& data = ne.data;
    int64_t base = seg.file_off;
    int64_t seglen = seg.length;
    py::dict<int64_t, py::str> fixups;
    auto label = [&](const ne_fmt::Relocation& r) -> py::str {
        if (r.kind == py::str("IMPORTORD")) {
            py::str m = module_label(mods, *r.module);
            return py::percent("%s.%d", m, *r.ordinal);
        }
        if (r.kind == py::str("IMPORTNAME")) {
            py::str m = module_label(mods, *r.module);
            return py::percent("%s.%s", m, ne.imported_name(*r.name_off));
        }
        if (r.kind == py::str("INTERNAL")) {
            // seg 0xFF is not a segment: it means "this module's OWN entry-table
            // ordinal N" -- the fixup for a call into a MOVEABLE segment, which
            // must go through the entry table so the segment can be relocated.
            // Resolving it names the target as a self-call (e.g. GDI -> GDI.273)
            // instead of a meaningless "seg255:0x0111".
            if (*r.seg == 0xFF) {
                return py::percent("%s.%d", ne.module_name(), *r.entry);
            }
            return py::percent("seg%d:0x%04x", *r.seg, *r.entry);
        }
        // r.get('ftype'): None when the record has no such key
        return py::str("osfixup#") + (r.ftype.has_value() ? py::percent("%d", *r.ftype) : py::str("None"));
    };
    for (const ne_fmt::Relocation& r : ne.relocations(seg)) {
        py::str lbl = label(r);
        int64_t cur = r.offset;
        int64_t seen = 0;
        while (cur != 0xFFFF && cur + 1 < seglen && seen < 10000) {
            fixups.set(cur, lbl);
            int64_t nxt = py::unpack_from1<int64_t>("<H", data, base + cur);
            if (r.additive) {
                break;
            }
            if (nxt == cur) {
                break;
            }
            cur = nxt;
            seen += 1;
        }
    }
    return fixups;
}

// tempfile.mkstemp(suffix=...): a new file in the temporary folder, made exclusively; -> its path
static py::str mkstemp(const py::str& suffix) {
    py::str dir = py::tempfile::gettempdir();
    for (int attempt = 0; attempt < 100; ++attempt) {
        py::str name = py::os::path::join(dir, py::str("tmp") + py::str(py::tempfile::detail_::random_name()) + suffix);
        try {
            py::file f = py::open(name, py::str("xb"));
            f.close();
            return name;
        } catch (const py::FileExistsError&) {
        }
    }
    throw py::FileExistsError("No usable temporary file name found");
}

void disasm_segment(const py::str& path, int64_t seg_index, int64_t start, const std::optional<int64_t>& count) {
    ne_fmt::NEFile ne = open_ne(path);
    py::list<py::str> mods = ne.module_refs();
    py::list<ne_fmt::Segment> segs = ne.segments();
    ne_fmt::Segment seg = segs[seg_index - 1];
    if (seg.kind != py::str("CODE")) {
        py::print(py::percent("(segment %d is %s)", seg_index, seg.kind));
    }
    py::bytes code = ne.data.slice(seg.file_off, seg.file_off + seg.length);
    py::dict<int64_t, py::str> fixups = build_fixup_map(ne, seg, mods);

    int64_t end = !count.has_value() ? seg.length : py::min(seg.length, start + *count);
    py::bytes blob = code.slice(start, end);
    // Unique temp file per process so concurrent disasm.py runs don't race.
    py::str tmp = mkstemp(py::str("_ne_seg.bin"));
    py::str out;
    try {
        {
            py::file fh = py::open(tmp, py::str("wb"));
            fh.write(blob);
            fh.close();
        }
        py::list<py::str> argv{py::str("objdump"), py::str("-D"), py::str("-b"), py::str("binary"), py::str("-mi386"),
                               py::str("-Maddr16,data16,intel"), py::percent("--adjust-vma=%d", start), tmp};
        py::subprocess::CompletedProcess done = py::subprocess::run(argv);
        out = *done.stdout_;
    } catch (...) {
        try {
            py::os::remove(tmp);
        } catch (const py::OSError&) {
        }
        throw;
    }
    try {
        py::os::remove(tmp);
    } catch (const py::OSError&) {
    }

    py::print(py::percent("# %s  segment #%d (%s, 0x%x bytes)  imports: %s", py::os::path::basename(path), seg_index, seg.kind, seg.length,
                          py::join(py::str(", "), mods)));
    py::print(py::percent("# %d fixup sites in this segment\n", py::len(fixups)));
    // First pass: collect instruction offsets so we know each instruction's span.
    py::list<std::tuple<std::optional<int64_t>, py::str>> parsed;   // (offset|None, raw_line)
    for (const py::str& line : py::splitlines(out)) {
        if (line.contains(py::str(":\t"))) {
            py::str head = py::split(line, py::str(":\t"))[0].strip();
            try {
                parsed.append(std::make_tuple(std::optional<int64_t>(py::int_parse(head, 16)), line));
                continue;
            } catch (const py::ValueError&) {
            }
        }
        parsed.append(std::make_tuple(std::optional<int64_t>(), line));
    }
    py::list<int64_t> offsets;
    for (const auto& [o, line] : parsed) {
        (void)line;
        if (o.has_value()) {
            offsets.append(*o);
        }
    }
    py::dict<int64_t, int64_t> nextoff;
    for (int64_t i = 0; i < py::len(offsets); ++i) {
        int64_t o = offsets[i];
        nextoff.set(o, i + 1 < py::len(offsets) ? offsets[i + 1] : o + 8);
    }
    for (const auto& [off, line] : parsed) {
        if (!off.has_value()) {
            py::print(line);
            continue;
        }
        py::str annot("");
        for (int64_t k = *off; k < nextoff.at(*off); ++k) {   // only THIS instruction's bytes
            if (fixups.contains(k)) {
                annot = py::str("   ; -> ") + fixups.at(k);
                break;
            }
        }
        py::print(line + annot);
    }
}

void disasm_entry(const py::str& path, int64_t ordinal) {
    ne_fmt::NEFile ne = open_ne(path);
    py::dict<int64_t, ne_fmt::Entry> ents;
    for (const ne_fmt::Entry& e : ne.entries()) {
        ents.set(e.ordinal, e);
    }
    std::optional<ne_fmt::Entry> e = ents.get(ordinal);
    if (!e.has_value() || e->kind == py::str("UNUSED")) {
        py::print(py::percent("ordinal %d not exported", ordinal));
        return;
    }
    py::print(py::percent("# entry ordinal %d -> segment %d offset 0x%x", ordinal, *e->seg, *e->offset));
    disasm_segment(path, *e->seg, *e->offset, 128);
}

int64_t main(const py::list<py::str>& a) {
    py::str path = a[0];
    if (a.contains(py::str("--entry"))) {
        disasm_entry(path, py::int_parse(a[a.index(py::str("--entry")) + 1], 0));
        return 0;
    }
    int64_t seg = py::len(a) > 1 && !a[1].startswith(py::str("--")) ? py::int_parse(a[1]) : 1;
    int64_t start = a.contains(py::str("--start")) ? py::int_parse(a[a.index(py::str("--start")) + 1], 0) : 0;
    std::optional<int64_t> count;
    if (a.contains(py::str("--count"))) {
        count = py::int_parse(a[a.index(py::str("--count")) + 1], 0);
    }
    disasm_segment(path, seg, start, count);
    return 0;
}

}  // namespace codebreaker::source::ne_asm::fixups
