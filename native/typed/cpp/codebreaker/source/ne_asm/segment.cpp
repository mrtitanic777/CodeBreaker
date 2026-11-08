// codebreaker/source/ne_asm/segment.cpp -- typed C++ of codebreaker.source.ne_asm.segment (see segment.hpp).

#include "codebreaker/source/ne_asm/segment.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "codebreaker/source/ne_asm/fixups.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::ne_asm::segment {

namespace ne_fmt = codebreaker::formats::ne;

// the module's own text (the Python's module docstring), which the script prints as its usage
static const char* const USAGE = R"DOC(
trace.py — Call-graph-tracing disassembler for the Windows 1.0 RE project.

Where disasm.py dumps one segment and wndproc.py decodes one dispatch table,
trace.py automates the tedious part of reconstruction: given an exported entry,
it discovers *every* function reachable from it, carves each function's
boundaries, disassembles it, and resolves *every* call to a real name —
KERNEL/GDI/USER ordinals via ORDINALS.md, internal calls to their function
label. One command replaces the dozens of hand-run disasm.py invocations and
the manual call-graph chase that Level-2 reconstruction otherwise needs.

Function discovery combines four signals: exported entry points, internal
relocation targets (segN:0xNNNN fixups), intra-segment near-call targets, and
`push bp`/`mov bp,sp` prologue scanning. Boundaries run start-to-next-start,
trimming trailing padding.

Usage:
    python3 trace.py FILE --map                 # exports, segments, function inventory
    python3 trace.py FILE --list [seg]          # every discovered function in a segment
    python3 trace.py FILE --fn 0xADDR [seg]     # one function, annotated
    python3 trace.py FILE --trace ORD           # an export + its whole reachable call graph
    python3 trace.py FILE --trace 0xADDR [seg]  # same, from a raw address
Add --ords PATH to point at a non-default ORDINALS.md.
)DOC";

// Window message names (mirrors wndproc.py's table).
const py::dict<int64_t, py::str> WM = {
    {0x0001, "WM_CREATE"}, {0x0002, "WM_DESTROY"}, {0x0003, "WM_MOVE"}, {0x0005, "WM_SIZE"},
    {0x0006, "WM_ACTIVATE"}, {0x0007, "WM_SETFOCUS"}, {0x0008, "WM_KILLFOCUS"}, {0x000A, "WM_ENABLE"},
    {0x000B, "WM_SETREDRAW"}, {0x000C, "WM_SETTEXT"}, {0x000D, "WM_GETTEXT"}, {0x000E, "WM_GETTEXTLENGTH"},
    {0x000F, "WM_PAINT"}, {0x0010, "WM_CLOSE"}, {0x0011, "WM_QUERYENDSESSION"}, {0x0012, "WM_QUIT"},
    {0x0013, "WM_QUERYOPEN"}, {0x0014, "WM_ERASEBKGND"}, {0x0018, "WM_SHOWWINDOW"},
    {0x001A, "WM_WININICHANGE"}, {0x0020, "WM_SETCURSOR"}, {0x0021, "WM_MOUSEACTIVATE"},
    {0x0024, "WM_GETMINMAXINFO"}, {0x0081, "WM_NCCREATE"}, {0x0083, "WM_NCCALCSIZE"},
    {0x0084, "WM_NCHITTEST"}, {0x0085, "WM_NCPAINT"}, {0x0086, "WM_NCACTIVATE"},
    {0x0100, "WM_KEYDOWN"}, {0x0101, "WM_KEYUP"}, {0x0102, "WM_CHAR"}, {0x0104, "WM_SYSKEYDOWN"},
    {0x0105, "WM_SYSKEYUP"}, {0x0106, "WM_SYSCHAR"}, {0x0110, "WM_INITDIALOG"}, {0x0111, "WM_COMMAND"},
    {0x0112, "WM_SYSCOMMAND"}, {0x0113, "WM_TIMER"}, {0x0114, "WM_HSCROLL"}, {0x0115, "WM_VSCROLL"},
    {0x0116, "WM_INITMENU"}, {0x0200, "WM_MOUSEMOVE"}, {0x0201, "WM_LBUTTONDOWN"}, {0x0202, "WM_LBUTTONUP"},
    {0x0203, "WM_LBUTTONDBLCLK"}, {0x0204, "WM_RBUTTONDOWN"}, {0x0205, "WM_RBUTTONUP"},
    {0x0206, "WM_RBUTTONDBLCLK"}, {0x0207, "WM_MBUTTONDOWN"}, {0x0208, "WM_MBUTTONUP"},
    {0x0300, "WM_CUT"}, {0x0301, "WM_COPY"}, {0x0302, "WM_PASTE"}, {0x0303, "WM_CLEAR"}, {0x0304, "WM_UNDO"},
};
// DOS INT 21h function names, for annotating the KERNEL wrappers' actual syscalls.
const py::dict<int64_t, py::str> DOS21 = {
    {0x02, "DISPLAY_CHAR"}, {0x09, "DISPLAY_STRING"}, {0x19, "GET_DEFAULT_DRIVE"}, {0x1A, "SET_DTA"},
    {0x25, "SET_VECTOR"}, {0x2A, "GET_DATE"}, {0x2C, "GET_TIME"}, {0x30, "GET_VERSION"}, {0x35, "GET_VECTOR"},
    {0x36, "GET_DISK_SPACE"}, {0x39, "MKDIR"}, {0x3A, "RMDIR"}, {0x3B, "CHDIR"}, {0x3C, "CREATE"}, {0x3D, "OPEN"},
    {0x3E, "CLOSE"}, {0x3F, "READ"}, {0x40, "WRITE"}, {0x41, "UNLINK"}, {0x42, "LSEEK"}, {0x43, "GET_SET_ATTR"},
    {0x44, "IOCTL"}, {0x47, "GET_CUR_DIR"}, {0x48, "ALLOC"}, {0x49, "FREE"}, {0x4A, "REALLOC"}, {0x4B, "EXEC"},
    {0x4C, "EXIT"}, {0x4E, "FINDFIRST"}, {0x4F, "FINDNEXT"}, {0x56, "RENAME"}, {0x57, "FILE_TIME"},
};

// ---- small helpers -----------------------------------------------------------------------------------------------

// re.match / re.search with a pattern compiled once (the patterns are written as the Python has them)
static const py::re::Pattern& rx(const char* pattern) {
    static std::unordered_map<std::string, py::re::Pattern> cache;
    auto it = cache.find(pattern);
    if (it == cache.end()) {
        it = cache.emplace(pattern, py::re::compile(py::str(pattern))).first;
    }
    return it->second;
}
static std::optional<py::re::Match> rmatch(const char* pattern, const py::str& s) { return rx(pattern).match(s); }

// int(text, 16) of a regex group
static int64_t hex_of(const py::re::Match& m, int64_t g = 1) { return py::int_parse(m.group(g), 16); }

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

// sorted(list of (a, b) int pairs)
static std::vector<std::tuple<int64_t, int64_t>> sorted_pairs(const py::list<std::tuple<int64_t, int64_t>>& xs) {
    std::vector<std::tuple<int64_t, int64_t>> v = xs.vec();
    std::sort(v.begin(), v.end());
    return v;
}

// ---- ORDINALS.md -------------------------------------------------------------------------------------------------

py::dict<py::str, py::str> load_ordinals(const py::str& path) {
    py::dict<py::str, py::str> names;
    if (!py::os::path::exists(path)) {
        return names;
    }
    const py::re::Pattern& pattern = rx(R"(\|\s*([A-Za-z_][A-Za-z0-9_]*)\.(\d+)\s*\|\s*`?([^|`]+?)`?\s*\|)");
    py::file f = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace"));
    for (const py::str& line : f) {
        std::optional<py::re::Match> m = pattern.search(line);
        if (m.has_value()) {
            names.set(py::percent("%s.%d", m->group(1), py::int_parse(m->group(2))), m->group(3).strip());
        }
    }
    return names;
}

py::list<std::tuple<int64_t, py::str>> disasm_whole(const py::bytes& code, int64_t base) {
    py::str tmp = mkstemp(py::str("_trace.bin"));
    py::str out;
    try {
        {
            py::file f = py::open(tmp, py::str("wb"));
            f.write(code);
            f.close();
        }
        py::list<py::str> argv{py::str("objdump"), py::str("-D"), py::str("-b"), py::str("binary"), py::str("-mi386"),
                               py::str("-Maddr16,data16,intel"), py::percent("--adjust-vma=%d", base), tmp};
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
    py::list<std::tuple<int64_t, py::str>> rows;
    for (const py::str& l : py::splitlines(out)) {
        if (!l.contains(py::str(":\t"))) {
            continue;
        }
        py::list<py::str> parts = py::split(l, py::str(":\t"), 1);
        py::str head = parts[0];
        py::str rest = parts[1];
        int64_t off;
        try {
            off = py::int_parse(head.strip(), 16);
        } catch (const py::ValueError&) {
            continue;
        }
        py::list<py::str> fields = py::split(rest, py::str("\t"));
        py::str asm_ = fields[-1].strip();
        rows.append(std::make_tuple(off, asm_));
    }
    return rows;
}

// ---- Seg ---------------------------------------------------------------------------------------------------------

Seg::Seg(const ne_fmt::NEFile& ne_, int64_t seg_index, const py::dict<py::str, py::str>& ordinals_,
         const std::optional<py::dict<int64_t, py::str>>& name_override_)
    : ne(ne_), si(seg_index), seg(), ordinals(ordinals_) {
    seg = ne.segments()[seg_index - 1];
    name_override = name_override_.has_value() ? name_override_->copy() : py::dict<int64_t, py::str>();
    mods = ne.module_refs();
    fixups = source::ne_asm::fixups::build_fixup_map(ne, seg, mods);
    code = ne.data.slice(seg.file_off, seg.file_off + seg.length);
    rows = disasm_whole(code);
    for (int64_t i = 0; i < py::len(rows); ++i) {
        off2idx.set(std::get<0>(rows[i]), i);
    }
    for (int64_t o : off2idx.keys()) {
        valid.add(o);
    }
    export_names = _export_names();
    starts = _find_starts();
    funcs = _carve();
}

py::dict<int64_t, py::str> Seg::_ord2name() const {
    py::list<std::tuple<py::str, int64_t>> res = ne.resident_names();
    py::list<std::tuple<py::str, int64_t>> non = ne.nonresident_names();
    py::dict<int64_t, py::str> m;
    for (const auto& [n, o] : res.slice_from(1) + non.slice_from(1)) {
        m.set(o, n);
    }
    return m;
}

py::dict<int64_t, py::str> Seg::_export_names() const {
    py::dict<int64_t, py::str> o2n = _ord2name();
    py::dict<int64_t, py::str> out;
    for (const ne_fmt::Entry& e : ne.entries()) {
        if (e.kind == py::str("UNUSED")) {
            continue;
        }
        if (e.seg.has_value() && *e.seg == si && e.offset.has_value()) {
            std::optional<py::str> nm = o2n.get(e.ordinal);
            if (nm.has_value() && !nm->empty()) {
                out.set(*e.offset, *nm);
            }
        }
    }
    return out;
}

py::set<int64_t> Seg::_find_starts() const {
    py::set<int64_t> st;
    // (1) exported entries landing in this segment.
    //     export_names only covers entries that HAVE a name, which for the
    //     stripped system modules is none of them -- so seed from the entry
    //     table directly as well. An exported entry point is a function
    //     start by definition, and without this the carve misses the first
    //     functions of every driver module.
    for (int64_t o : export_names.keys()) {
        st.add(o);
    }
    for (const ne_fmt::Entry& e : ne.entries()) {
        if (e.kind != py::str("UNUSED") && e.seg.has_value() && *e.seg == si && e.offset.has_value()) {
            st.add(*e.offset);
        }
    }
    // (2) internal relocation targets pointing into this segment
    for (const py::str& lbl : fixups.values()) {
        std::optional<py::re::Match> m = rmatch(R"(seg(\d+):0x([0-9a-fA-F]+)$)", lbl);
        if (m.has_value() && py::int_parse(m->group(1)) == si) {
            st.add(hex_of(*m, 2));
        }
    }
    // (3) intra-segment near-call targets
    for (const auto& [off, asm_] : rows) {
        (void)off;
        std::optional<py::re::Match> m = rmatch(R"(call\s+0x([0-9a-f]+)$)", asm_);
        if (m.has_value()) {
            st.add(hex_of(*m));
        }
    }
    // (4) prologue scan: push bp ; mov bp,sp
    for (int64_t i = 0; i < py::len(rows) - 1; ++i) {
        if (std::get<1>(rows[i]) == py::str("push bp") && std::get<1>(rows[i + 1]).startswith(py::str("mov    bp,sp"))) {
            // back up over a MakeProcInstance-style export thunk if present
            int64_t s = std::get<0>(rows[i]);
            st.add(s);
        }
    }
    // (5) the `enter N,0` prologue -- the other standard 16-bit far-proc
    //     frame setup (opcode 0xC8). The GDI/USER modules use it alongside the
    //     push-bp form; without it a function that no intra-segment call
    //     reaches (called only via far pointer / from another segment) is
    //     invisible as a start, and the PRECEDING function's carve swallows
    //     it -- e.g. GDI MulDiv (0x6942) merged 145 bytes of its `enter`-
    //     prologued neighbour, inflating the credited span.
    for (const auto& [off, asm_] : rows) {
        if (asm_.startswith(py::str("enter "))) {
            st.add(off);
        }
    }
    // (6) THUNK tail-JMP targets: a routine reached by a tail-`jmp` from
    //     >= 2 short thunks is a function start too -- e.g. GDI's ~17 `Get*`
    //     accessor stubs (`mov cl,IMM; jmp 0x45fe`) all tail-jump to one
    //     shared getter at 0x45fe. Without this the getter is not a start,
    //     the preceding function's carve swallows it, and verify.py reports
    //     it "unresolved" when a reconstruction cites it.
    //
    //     STRICTLY a thunk pattern: the source `jmp` must be within 8 bytes
    //     of an already-known start (i.e. the jmp is the tail of a tiny
    //     stub), and >= 2 such thunks must target it. This deliberately does
    //     NOT count a `jmp` that sits deep inside a function -- that is a
    //     shared epilogue / local control flow, not a boundary -- so the carve
    //     is not over-split into thousands of fake functions.
    std::vector<int64_t> known(st.begin(), st.end());
    std::sort(known.begin(), known.end());
    py::dict<int64_t, int64_t> jt;
    for (const auto& [off, asm_] : rows) {
        std::optional<py::re::Match> m = rmatch(R"(jmp\s+0x([0-9a-f]+)$)", asm_);
        if (!m.has_value()) {
            continue;
        }
        // bisect.bisect_right(known, off) - 1
        int64_t i = static_cast<int64_t>(std::upper_bound(known.begin(), known.end(), off) - known.begin()) - 1;
        if (i >= 0 && 0 <= off - known[static_cast<size_t>(i)] && off - known[static_cast<size_t>(i)] <= 8) {   // tail of a short stub
            int64_t t = hex_of(*m);
            jt.set(t, jt.get(t, 0) + 1);
        }
    }
    for (const auto& [tgt, n] : jt.items()) {
        if (n >= 2) {
            st.add(tgt);
        }
    }
    py::set<int64_t> out;
    for (int64_t s : st) {
        if (valid.contains(s)) {
            out.add(s);
        }
    }
    return out;
}

py::dict<int64_t, Func> Seg::_carve() const {
    std::vector<int64_t> order(starts.begin(), starts.end());
    std::sort(order.begin(), order.end());
    py::dict<int64_t, Func> out;
    for (size_t i = 0; i < order.size(); ++i) {
        int64_t s = order[i];
        int64_t end = i + 1 < order.size() ? order[i + 1] : seg.length;
        Func f;
        f.start = s;
        f.end = end;
        f.name = name_at(s);
        out.set(s, f);
    }
    return out;
}

py::str Seg::name_at(int64_t off) const {
    if (export_names.contains(off)) {
        return export_names.at(off);
    }
    if (name_override.contains(off)) {
        return name_override.at(off);
    }
    return py::percent("sub_%04x", off);
}

Resolved Seg::resolve_call(int64_t off, const py::str& asm_) const {
    // a fixup anywhere in this instruction's bytes names the target
    for (int64_t k = off; k < off + 7; ++k) {
        std::optional<py::str> lbl = fixups.get(k);
        if (lbl.has_value() && !lbl->empty()) {
            std::optional<py::re::Match> mm = rmatch(R"(seg(\d+):0x([0-9a-fA-F]+)$)", *lbl);
            if (mm.has_value()) {
                Resolved r;
                if (py::int_parse(mm->group(1)) == si) {
                    r.kind = CallKind::Int;
                    r.target = hex_of(*mm, 2);
                    return r;
                }
                r.kind = CallKind::Far;
                r.label = *lbl;
                return r;
            }
            Resolved r;
            r.kind = CallKind::Sys;
            r.label = *lbl;
            r.name = ordinals.get(*lbl);
            return r;
        }
    }
    std::optional<py::re::Match> m = rmatch(R"(call\s+0x([0-9a-f]+)$)", asm_);   // intra-segment near call
    if (m.has_value()) {
        Resolved r;
        r.kind = CallKind::Int;
        r.target = hex_of(*m);
        return r;
    }
    return Resolved();
}

std::tuple<Func, py::list<std::tuple<int64_t, py::str>>> Seg::func_rows(int64_t start) const {
    std::optional<Func> found = funcs.get(start);
    Func f;
    if (found.has_value()) {
        f = *found;
    } else {
        // not a known start: synthesize a span to the next known start
        std::vector<int64_t> order(starts.begin(), starts.end());
        std::sort(order.begin(), order.end());
        int64_t nxt = seg.length;
        for (int64_t s : order) {
            if (s > start) {
                nxt = s;
                break;
            }
        }
        f.start = start;
        f.end = nxt;
        f.name = name_at(start);
    }
    std::optional<int64_t> i0 = off2idx.get(start);
    py::list<std::tuple<int64_t, py::str>> out;
    if (!i0.has_value()) {
        return std::make_tuple(f, out);
    }
    for (int64_t k = *i0; k < py::len(rows); ++k) {
        const auto& [off, asm_] = rows[k];
        if (off >= f.end) {
            break;
        }
        out.append(std::make_tuple(off, asm_));
    }
    // trim trailing padding (nop / int3 / lea esp fillers)
    while (!out.empty()) {
        const py::str& last = std::get<1>(out[-1]);
        if (last == py::str("nop") || last == py::str("int3") || last == py::str("(bad)")) {
            out.pop();
        } else {
            break;
        }
    }
    return std::make_tuple(f, out);
}

py::list<std::tuple<CallKind, py::str>> Seg::calls_in(int64_t start) const {
    py::list<std::tuple<int64_t, py::str>> frows = std::get<1>(func_rows(start));
    std::vector<std::tuple<int, py::str>> seen;   // the Python's set of (kind, label|target) keys
    py::list<std::tuple<CallKind, py::str>> out;
    for (const auto& [off, asm_] : frows) {
        if (!(asm_.startswith(py::str("call")) || asm_.startswith(py::str("jmp")))) {
            continue;
        }
        Resolved r = resolve_call(off, asm_);
        std::tuple<int, py::str> key;
        py::str disp;
        if (r.kind == CallKind::Sys) {
            key = std::make_tuple(0, r.label);
            disp = r.label + ((r.name.has_value() && !r.name->empty()) ? py::str(" = ") + *r.name : py::str("  [unidentified]"));
        } else if (r.kind == CallKind::Int) {
            key = std::make_tuple(1, py::str(std::to_string(r.target)));
            disp = py::percent("-> %s (seg%d:0x%04x)", name_at(r.target), si, r.target);
        } else if (r.kind == CallKind::Far) {
            key = std::make_tuple(2, r.label);
            disp = py::str("-> ") + r.label + py::str(" (other segment)");
        } else {
            continue;
        }
        // only record jmp as a (tail)call when it leaves via a fixup/other target
        if (asm_.startswith(py::str("jmp")) && r.kind == CallKind::Int) {
            // intra-function jump, not a call -- skip local control flow
            if (!funcs.get(r.target).has_value()) {
                continue;
            }
        }
        if (std::find(seen.begin(), seen.end(), key) == seen.end()) {
            seen.push_back(key);
            out.append(std::make_tuple(r.kind, disp));
        }
    }
    return out;
}

py::list<int64_t> Seg::args_used(int64_t start) const {
    py::list<std::tuple<int64_t, py::str>> frows = std::get<1>(func_rows(start));
    py::set<int64_t> slots;
    for (const auto& [off, asm_] : frows) {
        (void)off;
        for (const py::re::Match& m : rx(R"(\[bp\+0x([0-9a-f]+)\])").finditer(asm_)) {
            slots.add(hex_of(m));
        }
    }
    std::vector<int64_t> v;
    for (int64_t s : slots) {
        if (s >= 4) {
            v.push_back(s);
        }
    }
    std::sort(v.begin(), v.end());
    return py::list<int64_t>(v);
}

std::optional<int64_t> Seg::word_at(int64_t off) const {
    if (0 <= off && off <= py::len(code) - 2) {
        return code[off] | (code[off + 1] << 8);
    }
    return std::nullopt;
}

py::list<std::tuple<int64_t, int64_t>> Seg::dispatch(int64_t start) const {
    py::list<std::tuple<int64_t, py::str>> frows = std::get<1>(func_rows(start));
    py::list<std::tuple<int64_t, int64_t>> disp;
    py::set<int64_t> seen;
    bool ax_is_msg = false;

    // Branch target for a message compare at row i. Handles three forms:
    // `je H`; `jne SKIP; jmp H`; and `jne ELSE; <inline handler falls
    // through>` (handler = the instruction right after the jne).
    auto handler_after = [&](int64_t i) -> std::optional<int64_t> {
        py::list<std::tuple<int64_t, py::str>> window = frows.slice(i + 1, i + 4);
        for (int64_t j = 0; j < py::len(window); ++j) {
            const py::str& a2 = std::get<1>(window[j]);
            std::optional<py::re::Match> jm = rmatch(R"(je     0x([0-9a-f]+))", a2);
            if (jm.has_value()) {
                return hex_of(*jm);
            }
            std::optional<py::re::Match> jn = rmatch(R"(jne    0x([0-9a-f]+))", a2);
            if (jn.has_value()) {
                int64_t k = i + 1 + j + 1;
                if (k < py::len(frows)) {
                    std::optional<py::re::Match> jm2 = rmatch(R"(jmp    0x([0-9a-f]+))", std::get<1>(frows[k]));
                    return jm2.has_value() ? hex_of(*jm2) : std::get<0>(frows[k]);
                }
                return std::nullopt;
            }
        }
        return std::nullopt;
    };

    for (int64_t i = 0; i < py::len(frows); ++i) {
        const py::str& asm_ = std::get<1>(frows[i]);
        // ---- dialog form: direct memory compare of the message slot -------
        std::optional<py::re::Match> md = rmatch(R"(cmp    WORD PTR \[bp\+0xc\],0x([0-9a-f]+)$)", asm_);
        if (md.has_value()) {
            int64_t val = hex_of(*md);
            std::optional<int64_t> h = handler_after(i);
            if (h.has_value() && !seen.contains(val)) {
                disp.append(std::make_tuple(val, *h));
                seen.add(val);
            }
            continue;
        }
        // ---- jump-table dispatch: jmp WORD cs:[bx+TAB] --------------------
        std::optional<py::re::Match> jt = rmatch(R"(jmp    WORD PTR cs:\[bx\+0x([0-9a-f]+)\])", asm_);
        if (jt.has_value()) {
            int64_t tab = hex_of(*jt);
            std::optional<int64_t> count;
            int64_t base = 0;
            bool saw_msg = false;
            // look back for the range check
            py::list<std::tuple<int64_t, py::str>> back = frows.slice(std::max<int64_t>(0, i - 20), i);
            for (int64_t b = py::len(back) - 1; b >= 0; --b) {
                const py::str& a2 = std::get<1>(back[b]);
                std::optional<py::re::Match> mm = rmatch(R"(cmp    ax,0x([0-9a-f]+)$)", a2);
                if (mm.has_value() && !count.has_value()) {
                    count = hex_of(*mm);
                }
                std::optional<py::re::Match> sm = rmatch(R"(sub    ax,0x([0-9a-f]+)$)", a2);
                if (sm.has_value()) {
                    base += hex_of(*sm);
                }
                if (rmatch(R"(mov    ax,WORD PTR \[bp\+0xc\])", a2).has_value()) {
                    saw_msg = true;
                }
            }
            if (count.has_value() && saw_msg) {
                for (int64_t idx = 0; idx < *count + 1; ++idx) {
                    std::optional<int64_t> h = word_at(tab + 2 * idx);
                    if (h.has_value() && !seen.contains(idx + base)) {
                        disp.append(std::make_tuple(idx + base, *h));
                        seen.add(idx + base);
                    }
                }
            }
            continue;
        }
        if (rmatch(R"(mov    ax,WORD PTR \[bp\+0xc\])", asm_).has_value()) {
            ax_is_msg = true;
            continue;
        }
        if (rmatch(R"((mov    ax,|pop    ax|lea    ax|call|xor    ax,ax|sub    ax,ax|xchg .*ax))", asm_).has_value()) {
            if (!asm_.startswith(py::str("cmp"))) {
                ax_is_msg = false;
            }
        }
        std::optional<py::re::Match> m = rmatch(R"(cmp    ax,0x([0-9a-f]+)$)", asm_);
        if (!m.has_value() || !ax_is_msg) {
            continue;
        }
        int64_t val = hex_of(*m);
        std::optional<int64_t> handler = handler_after(i);
        if (handler.has_value() && !seen.contains(val)) {
            disp.append(std::make_tuple(val, *handler));
            seen.add(val);
        }
    }
    return disp;
}

py::list<py::str> Seg::handler_calls(int64_t start, int64_t handler_off, const py::list<int64_t>& handler_starts) const {
    py::list<std::tuple<int64_t, py::str>> frows = std::get<1>(func_rows(start));
    py::dict<int64_t, int64_t> idx;
    for (int64_t k = 0; k < py::len(frows); ++k) {
        idx.set(std::get<0>(frows[k]), k);
    }
    std::optional<int64_t> kk = idx.get(handler_off);
    if (!kk.has_value()) {
        return py::list<py::str>();
    }
    int64_t k = *kk;
    std::optional<int64_t> nxt;   // min((s for s in handler_starts if s > handler_off), default=None)
    for (int64_t s : handler_starts) {
        if (s > handler_off && (!nxt.has_value() || s < *nxt)) {
            nxt = s;
        }
    }
    py::list<py::str> out;
    std::vector<std::tuple<int, py::str>> seen;
    while (k < py::len(frows)) {
        const auto& [off, asm_] = frows[k];
        if (off != handler_off && handler_starts.contains(off)) {
            break;
        }
        if (asm_.startswith(py::str("ret"))) {
            break;
        }
        std::optional<py::re::Match> jm = rmatch(R"(jmp    0x([0-9a-f]+)$)", asm_);
        if (jm.has_value() && !asm_.startswith(py::str("call"))) {
            int64_t t = hex_of(*jm);
            if (t < handler_off || (nxt.has_value() && *nxt != 0 && t >= *nxt)) {
                break;   // tail-jump to shared epilogue
            }
        }
        if (asm_.startswith(py::str("call")) || asm_.startswith(py::str("jmp"))) {
            Resolved r = resolve_call(off, asm_);
            std::tuple<int, py::str> key;
            py::str disp;
            if (r.kind == CallKind::Sys) {
                key = std::make_tuple(0, r.label);
                disp = r.label + ((r.name.has_value() && !r.name->empty()) ? py::str("=") + *r.name : py::str(" [unident]"));
            } else if (r.kind == CallKind::Int) {
                key = std::make_tuple(1, py::str(std::to_string(r.target)));
                disp = py::str("->") + name_at(r.target);
            } else if (r.kind == CallKind::Far) {
                key = std::make_tuple(2, r.label);
                disp = py::str("->") + r.label;
            } else {
                k += 1;
                continue;
            }
            if (std::find(seen.begin(), seen.end(), key) == seen.end()) {
                seen.push_back(key);
                out.append(disp);
            }
        }
        k += 1;
    }
    return out;
}

std::optional<int64_t> Seg::is_thunk(int64_t start) const {
    py::list<std::tuple<int64_t, py::str>> frows = std::get<1>(func_rows(start));
    if (py::len(frows) >= 3 && std::get<1>(frows[0]).startswith(py::str("mov    bx,0x")) && std::get<1>(frows[1]) == py::str("push   bx")) {
        std::optional<py::re::Match> jm = rmatch(R"(jmp    0x([0-9a-f]+))", std::get<1>(frows[2]));
        if (jm.has_value()) {
            return hex_of(*jm);
        }
    }
    return std::nullopt;
}

std::optional<std::tuple<int64_t, int64_t>> find_seg_of_entry(const ne_fmt::NEFile& ne, int64_t ordinal) {
    for (const ne_fmt::Entry& e : ne.entries()) {
        if (e.ordinal == ordinal && e.offset.has_value()) {
            return std::make_tuple(*e.seg, *e.offset);
        }
    }
    return std::nullopt;
}

// ---- annotated printing ------------------------------------------------------------------------------------------

py::str annotate_row(const Seg& sg, int64_t off, const py::str& asm_) {
    py::str line = py::percent("    %04x:  %s", off, asm_);
    if (asm_.startswith(py::str("call")) || asm_.startswith(py::str("jmp"))) {
        Resolved r = sg.resolve_call(off, asm_);
        if (r.kind == CallKind::Sys) {
            line = line + py::str("      ; ") + r.label +
                   ((r.name.has_value() && !r.name->empty()) ? py::str(" = ") + *r.name : py::str("  [UNIDENTIFIED — leave raw]"));
        } else if (r.kind == CallKind::Int && (sg.funcs.get(r.target).has_value() || asm_.startswith(py::str("call")))) {
            line = line + py::str("      ; -> ") + sg.name_at(r.target);
        } else if (r.kind == CallKind::Far) {
            line = line + py::str("      ; -> ") + r.label;
        }
    }
    std::optional<py::re::Match> m = rmatch(R"(mov    ah,0x([0-9a-f]+))", asm_);   // flag DOS syscalls
    if (m.has_value() && DOS21.contains(hex_of(*m))) {
        line = line + py::percent("      ; AH=%s DOS %s", m->group(1), DOS21.at(hex_of(*m)));
    }
    if (asm_.startswith(py::str("int    0x21"))) {
        line = line + py::str("      ; DOS call");
    }
    return line;
}

void print_function(const Seg& sg, int64_t start, bool body) {
    auto fr = sg.func_rows(start);
    const Func& f = std::get<0>(fr);
    const py::list<std::tuple<int64_t, py::str>>& rows = std::get<1>(fr);
    py::list<int64_t> args = sg.args_used(start);
    std::vector<std::tuple<int64_t, int64_t>> disp = sorted_pairs(sg.dispatch(start));
    py::str hdr = py::percent("==== %s  seg%d:0x%04x", f.name, sg.si, start);
    hdr = hdr + py::percent("  (%d instrs, 0x%04x..0x%04x)", py::len(rows), start, f.end);
    py::print(hdr);
    if (!args.empty()) {
        py::list<py::str> parts;
        for (int64_t a : args) {
            parts.append(py::percent("[bp+0x%02x]", a));
        }
        py::print(py::str("   args (PASCAL frame slots read): ") + py::join(py::str(", "), parts));
    }
    if (!disp.empty()) {
        py::print(py::percent("   WndProc dispatch (%d messages):", static_cast<int64_t>(disp.size())));
        for (const auto& [val, h] : disp) {
            py::print(py::percent("      %-20s 0x%04x -> handler @0x%04x", WM.get(val, py::percent("msg_0x%04x", val)), val, h));
        }
    }
    py::list<std::tuple<CallKind, py::str>> calls = sg.calls_in(start);
    if (!calls.empty()) {
        py::list<py::str> sysn, intn, farn;
        for (const auto& [k, d] : calls) {
            if (k == CallKind::Sys) sysn.append(d);
            if (k == CallKind::Int) intn.append(d);
            if (k == CallKind::Far) farn.append(d);
        }
        if (!sysn.empty()) py::print(py::str("   syscalls: ") + py::join(py::str("; "), sysn));
        if (!intn.empty()) py::print(py::str("   internal: ") + py::join(py::str("; "), intn));
        if (!farn.empty()) py::print(py::str("   far:      ") + py::join(py::str("; "), farn));
    }
    if (body) {
        py::print(py::str(""));
        for (const auto& [off, asm_] : rows) {
            py::print(annotate_row(sg, off, asm_));
        }
    }
    py::print(py::str(""));
}

py::list<int64_t> trace_callgraph(const Seg& sg, int64_t start) {
    py::list<int64_t> order;
    py::set<int64_t> seen;
    py::list<int64_t> queue{start};
    while (!queue.empty()) {
        int64_t s = queue.pop(0);
        if (seen.contains(s)) {
            continue;
        }
        seen.add(s);
        order.append(s);
        for (const auto& [kind, disp] : sg.calls_in(s)) {
            if (kind == CallKind::Int) {
                std::optional<py::re::Match> m = rx(R"(seg\d+:0x([0-9a-f]+))").search(disp);
                if (m.has_value()) {
                    int64_t t = hex_of(*m);
                    if (!seen.contains(t)) {
                        queue.append(t);
                    }
                }
            }
        }
    }
    return order;
}

// ---- commands ----------------------------------------------------------------------------------------------------

void cmd_map(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals) {
    py::list<std::tuple<py::str, int64_t>> res = ne.resident_names();
    py::list<std::tuple<py::str, int64_t>> non = ne.nonresident_names();
    py::str modname = !res.empty() ? std::get<0>(res[0]) : py::str("?");
    py::str desc = !non.empty() ? std::get<0>(non[0]) : py::str("");
    py::list<ne_fmt::Segment> segs = ne.segments();
    py::print(py::str("# ") + modname + py::str("   ") + desc);
    py::print(py::str("# imports: ") + py::join(py::str(", "), ne.module_refs()));
    py::print(py::percent("# %d segments\n", py::len(segs)));
    py::dict<int64_t, py::str> o2n;
    for (const auto& [n, o] : res.slice_from(1) + non.slice_from(1)) {
        o2n.set(o, n);
    }
    py::print(py::str("Exports:"));
    std::vector<ne_fmt::Entry> ents = ne.entries().vec();
    std::stable_sort(ents.begin(), ents.end(), [](const ne_fmt::Entry& a, const ne_fmt::Entry& b) { return a.ordinal < b.ordinal; });
    for (const ne_fmt::Entry& e : ents) {
        if (e.kind == py::str("UNUSED") || !e.offset.has_value()) {
            continue;
        }
        py::str nm = o2n.get(e.ordinal, py::str(""));
        py::print(py::percent("   @%-4d %-24s seg%d:0x%04x", e.ordinal, nm, *e.seg, *e.offset));
    }
    py::print(py::str(""));
    for (const ne_fmt::Segment& s : segs) {
        if (s.kind != py::str("CODE")) {
            py::print(py::percent("seg%d: %s 0x%x bytes", s.idx, s.kind, s.length));
            continue;
        }
        Seg sg(ne, s.idx, ordinals);
        int64_t nsys = 0;
        for (const py::str& l : sg.fixups.values()) {
            if (!l.startswith(py::str("seg"))) {
                nsys += 1;
            }
        }
        py::print(py::percent("seg%d: CODE 0x%x bytes  %d functions discovered  %d fixups (%d syscalls)", s.idx, s.length,
                              py::len(sg.funcs), py::len(sg.fixups), nsys));
    }
}

void cmd_list(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index) {
    Seg sg(ne, seg_index, ordinals);
    py::print(py::percent("# %d functions in seg%d\n", py::len(sg.funcs), seg_index));
    std::vector<int64_t> keys = sg.funcs.keys();
    std::sort(keys.begin(), keys.end());
    for (int64_t s : keys) {
        print_function(sg, s, false);
    }
}

void cmd_fn(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t addr) {
    Seg sg(ne, seg_index, ordinals);
    print_function(sg, addr, true);
}

void cmd_cskel(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start) {
    Seg sg(ne, seg_index, ordinals);
    std::vector<std::tuple<int64_t, int64_t>> disp = sorted_pairs(sg.dispatch(start));
    py::str name = sg.name_at(start);
    py::list<int64_t> hs;
    for (const auto& [v, h] : disp) {
        (void)v;
        hs.append(h);
    }
    std::sort(hs.vec().begin(), hs.vec().end());
    bool is_dlg = false;   // WM_INITDIALOG => dialog proc
    for (const auto& [v, h] : disp) {
        (void)h;
        if (v == 0x110) {
            is_dlg = true;
        }
    }
    py::str ret_t = is_dlg ? py::str("BOOL") : py::str("long");
    py::str hval = is_dlg ? py::str("hDlg") : py::str("hWnd");
    py::print(py::percent("/* AUTO-SKELETON from trace.py --cskel  seg%d:0x%04x", seg_index, start) +
              py::percent("  (%s) */", is_dlg ? py::str("dialog proc") : py::str("window proc")));
    py::print(py::percent("%s FAR PASCAL %s(HWND %s, unsigned message, WORD wParam, LONG lParam)", ret_t, name, hval));
    py::print(py::str("{"));
    py::print(py::str("    switch (message) {"));
    for (const auto& [val, h] : disp) {
        py::str nm = WM.get(val, py::percent("0x%04x", val));
        py::list<py::str> calls = sg.handler_calls(start, h, hs);
        py::str cs = !calls.empty() ? py::str("calls: ") + py::join(py::str("; "), calls) : py::str("(no resolved calls)");
        py::print(py::percent("    case %s:  /* @0x%04x  %s */", nm, h, cs));
        py::print(py::str("        break;"));
    }
    if (is_dlg) {
        py::print(py::str("    }\n    return FALSE;\n}"));
    } else {
        py::print(py::str("    default:"));
        py::print(py::str("        return DefWindowProc(") + hval + py::str(", message, wParam, lParam); /* USER.107 */"));
        py::print(py::str("    }\n    return 0L;\n}"));
    }
    py::list<int64_t> order;
    for (int64_t s : trace_callgraph(sg, start)) {
        if (s != start) {
            order.append(s);
        }
    }
    if (!order.empty()) {
        py::print(py::percent("\n/* --- %d reachable internal functions (documented-partial templates) --- */", py::len(order)));
        for (int64_t s : order) {
            auto fr = sg.func_rows(s);
            const Func& f = std::get<0>(fr);
            const py::list<std::tuple<int64_t, py::str>>& rows = std::get<1>(fr);
            py::list<int64_t> args = sg.args_used(s);
            py::list<std::tuple<CallKind, py::str>> calls = sg.calls_in(s);
            py::list<py::str> sysn, intn;
            for (const auto& [k, d] : calls) {
                if (k == CallKind::Sys) sysn.append(d);
                if (k == CallKind::Int) intn.append(d);
            }
            py::list<py::str> parts;
            for (int64_t a : args) {
                parts.append(py::percent("[bp+0x%02x]", a));
            }
            py::str astr = !parts.empty() ? py::join(py::str(", "), parts) : py::str("none");
            py::print(py::percent("/* %s  seg%d:0x%04x  (%d instrs; args %s)", f.name, sg.si, s, py::len(rows), astr));
            if (!sysn.empty()) py::print(py::str(" *   syscalls: ") + py::join(py::str("; "), sysn));
            if (!intn.empty()) py::print(py::str(" *   internal: ") + py::join(py::str("; "), intn));
            py::print(py::str(" *   [documented partial] */"));
        }
    }
}

void cmd_wndprocs(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals, std::optional<int64_t> only_seg) {
    py::dict<int64_t, py::str> o2n;
    for (const auto& [n, o] : ne.resident_names().slice_from(1) + ne.nonresident_names().slice_from(1)) {
        o2n.set(o, n);
    }
    // map seg:offset -> export name via the entry table
    py::dict<std::tuple<int64_t, int64_t>, py::str> ent;
    for (const ne_fmt::Entry& e : ne.entries()) {
        if (e.offset.has_value()) {
            std::optional<py::str> nm = o2n.get(e.ordinal);
            if (nm.has_value() && !nm->empty()) {
                ent.set(std::make_tuple(*e.seg, *e.offset), *nm);
            }
        }
    }
    for (const ne_fmt::Segment& s : ne.segments()) {
        if (s.kind != py::str("CODE") || (only_seg.has_value() && *only_seg != 0 && s.idx != *only_seg)) {
            continue;
        }
        Seg sg(ne, s.idx, ordinals);
        std::vector<int64_t> keys = sg.funcs.keys();
        std::sort(keys.begin(), keys.end());
        for (int64_t start : keys) {
            std::vector<std::tuple<int64_t, int64_t>> disp = sorted_pairs(sg.dispatch(start));
            if (disp.size() < 2) {
                continue;
            }
            py::str nm = ent.get(std::make_tuple(s.idx, start), py::str(""));
            py::str tag = py::str("  [export ") + nm + py::str("]");
            if (nm.empty()) {
                tag = py::str("");
            }
            py::print(py::percent("seg%d:0x%04x%s  - %d messages", s.idx, start, tag, static_cast<int64_t>(disp.size())));
            for (const auto& [val, h] : disp) {
                py::print(py::percent("    %-20s 0x%04x -> 0x%04x", WM.get(val, py::percent("msg_0x%04x", val)), val, h));
            }
            py::print(py::str(""));
        }
    }
}

void cmd_trace(const ne_fmt::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start) {
    Seg sg(ne, seg_index, ordinals);
    py::list<int64_t> order = trace_callgraph(sg, start);
    py::print(py::percent("# call graph from seg%d:0x%04x - ", seg_index, start) +
              py::percent("%d reachable functions in this segment\n", py::len(order)));
    // roll up the full external API surface
    py::dict<py::str, py::str> allsys;
    for (int64_t s : order) {
        for (const auto& [kind, disp] : sg.calls_in(s)) {
            if (kind == CallKind::Sys) {
                py::str lbl = py::split(disp)[0];
                allsys.set(lbl, disp);
            }
        }
    }
    py::print(py::str("External API surface (all reachable functions):"));
    // sorted(allsys, key=lambda x: (x.split(".")[0], int(x.split(".")[1]))): every key is computed before the sort
    struct SortKey {
        py::str module;
        int64_t n;
        py::str label;
    };
    std::vector<SortKey> keyed;
    for (const py::str& x : allsys.keys()) {
        py::str module = py::split(x, py::str("."))[0];
        int64_t n = py::int_parse(py::split(x, py::str("."))[1]);
        keyed.push_back(SortKey{module, n, x});
    }
    std::stable_sort(keyed.begin(), keyed.end(), [](const SortKey& a, const SortKey& b) {
        if (a.module != b.module) return a.module < b.module;
        return a.n < b.n;
    });
    for (const SortKey& k : keyed) {
        py::print(py::str("   ") + allsys.at(k.label));
    }
    py::print(py::str("\n") + py::str(std::string(70, '=')) + py::str("\n"));
    for (int64_t s : order) {
        print_function(sg, s, true);
    }
}

// ---- the script --------------------------------------------------------------------------------------------------

int64_t main(const py::list<py::str>& a) {
    if (a.empty()) {
        py::print(py::str(USAGE));
        return 1;   // sys.exit(1)
    }
    py::str path = a[0];
    py::str ords_path = a.contains(py::str("--ords")) ? a[a.index(py::str("--ords")) + 1] : py::str("ORDINALS.md");
    py::dict<py::str, py::str> ordinals = load_ordinals(ords_path);
    ne_fmt::NEFile ne = source::ne_asm::fixups::open_ne(path);

    auto int0 = [](const py::str& x) -> int64_t { return py::int_parse(x, 0); };
    // [x for x in a[i:] if not x.startswith("--")]
    auto rest_from = [&](int64_t i) -> py::list<py::str> {
        py::list<py::str> rest;
        for (const py::str& x : a.slice_from(i)) {
            if (!x.startswith(py::str("--"))) {
                rest.append(x);
            }
        }
        return rest;
    };

    if (a.contains(py::str("--map"))) {
        cmd_map(ne, ordinals);
    } else if (a.contains(py::str("--wndprocs"))) {
        py::list<py::str> rest = rest_from(a.index(py::str("--wndprocs")) + 1);
        cmd_wndprocs(ne, ordinals, !rest.empty() ? std::optional<int64_t>(int0(rest[0])) : std::nullopt);
    } else if (a.contains(py::str("--cskel"))) {
        int64_t addr = int0(a[a.index(py::str("--cskel")) + 1]);
        py::list<py::str> rest = rest_from(a.index(py::str("--cskel")) + 2);
        cmd_cskel(ne, ordinals, !rest.empty() ? int0(rest[0]) : 1, addr);
    } else if (a.contains(py::str("--list"))) {
        py::list<py::str> rest = rest_from(a.index(py::str("--list")) + 1);
        int64_t seg = !rest.empty() ? int0(rest[0]) : 1;
        cmd_list(ne, ordinals, seg);
    } else if (a.contains(py::str("--fn"))) {
        int64_t addr = int0(a[a.index(py::str("--fn")) + 1]);
        py::list<py::str> rest = rest_from(a.index(py::str("--fn")) + 2);
        int64_t seg = !rest.empty() ? int0(rest[0]) : 1;
        cmd_fn(ne, ordinals, seg, addr);
    } else if (a.contains(py::str("--trace"))) {
        py::str tgt = a[a.index(py::str("--trace")) + 1];
        py::list<py::str> rest = rest_from(a.index(py::str("--trace")) + 2);
        int64_t seg;
        int64_t start;
        if (tgt.startswith(py::str("0x"))) {
            seg = !rest.empty() ? int0(rest[0]) : 1;
            start = int0(tgt);
        } else {   // an ordinal: look up its seg:offset
            std::optional<std::tuple<int64_t, int64_t>> found = find_seg_of_entry(ne, int0(tgt));
            if (!found.has_value()) {
                py::print(py::str("ordinal ") + tgt + py::str(" not found in entry table"));
                return 1;   // sys.exit(1)
            }
            std::tie(seg, start) = *found;
        }
        cmd_trace(ne, ordinals, seg, start);
    } else {
        py::print(py::str(USAGE));
    }
    return 0;
}

}  // namespace codebreaker::source::ne_asm::segment
