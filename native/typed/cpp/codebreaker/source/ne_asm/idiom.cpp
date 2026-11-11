// codebreaker/source/ne_asm/idiom.cpp -- typed C++ of codebreaker.source.ne_asm.idiom.

#include "codebreaker/source/ne_asm/idiom.hpp"

#include <algorithm>
#include <memory>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/source/ne_asm/segdiff.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "codebreaker/symbols/debug_build.hpp"
#include "py_glob.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::ne_asm::idiom {

py::str Corpus::resolve(const py::str& module) {
    // (the Python's `corpus.resolve(m) or corpus.retail(m) or m` never reached its fallbacks: resolve() raises
    // ValueError for an unknown name. Here the fallbacks are what they say.)
    try {
        return base::corpus::resolve(module);
    } catch (const py::ValueError&) {
        const std::optional<py::str> r = base::corpus::retail(module);
        return r ? *r : module;
    }
}

Corpus _target() { return Corpus(); }

const py::set<py::str> _JMP = {py::str("jmp")};
const py::set<py::str> _JCC = {py::str("je"),    py::str("jne"),  py::str("jz"),   py::str("jnz"),   py::str("jl"),    py::str("jle"),
                               py::str("jg"),    py::str("jge"),  py::str("jb"),   py::str("jbe"),   py::str("ja"),    py::str("jae"),
                               py::str("js"),    py::str("jns"),  py::str("jo"),   py::str("jno"),   py::str("jp"),    py::str("jnp"),
                               py::str("jpe"),   py::str("jpo"),  py::str("jc"),   py::str("jnc"),   py::str("jcxz"),  py::str("jecxz"),
                               py::str("loop"),  py::str("loope"), py::str("loopne"), py::str("loopz"), py::str("loopnz")};
const py::set<py::str> _CALL = {py::str("call")};

const py::list<py::str> _PREAMBLE = {
    py::str("; idiomatic recovered source -- Microsoft SDK cMacros (ne_idiom.py)"),
    // OPTION M510: MASM 5.10 compat -- enables the cMacros `&' paste AND makes .386 segments default to use16, so 16-bit code keeps
    // its encoding while 32-bit-operand (66/67-prefix) routines still assemble.
    py::str("OPTION M510"),
    py::str("?PLM = 1"),
    py::str("?WIN = 1"),
    py::str(".xlist"),
    py::str("include cmacros.inc"),
    py::str(".list"),
    py::str(""),
    py::str("sBegin  CODE"),
    // .386p INSIDE the open segment: sBegin already fixed _TEXT as use16 (16-bit code keeps its encoding); .386p now enables the 386
    // registers/instructions KERNEL's 32-bit-operand routines use (they get 66/67 prefixes in a use16 seg).
    py::str(".386p"),
    py::str("        assumes cs,CODE"),
    py::str("        assumes ds,nothing"),
    py::str("        assumes es,nothing"),
    py::str(""),
};

namespace {

const py::re::Pattern& ws_re() {
    static const py::re::Pattern p = py::re::compile(py::str("\\s+"));
    return p;
}

// open(path, "w", newline=nl).write(text)   (the locale's encoding: cp1252)
void write_text(const py::str& path, const py::str& text, const char* newline) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str(newline));
    f.write(text);
    f.close();
}

// open(path, errors="replace").read()
py::str read_replace(const py::str& path) {
    py::file f = py::open(path, py::str("r"), py::str("cp1252"), py::str("replace"));
    py::str t = f.read_text();
    f.close();
    return t;
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
py::str state_(const py::str& a, const py::str& b, const py::str& c) {
    const py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b, c);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// repr(e) of a caught exception as CPython writes it
py::str repr_of(const py::Exception& e) {
    static const py::re::Pattern win = py::re::compile(py::str("^\\[WinError (\\d+)\\] (.*)$"));
    const py::str what{std::string(e.what())};
    const std::string type = e.type();
    if (const auto m = win.match(what)) {
        if (type == "FileNotFoundError") return py::str("FileNotFoundError(2, ") + py::repr(m->group(2)) + py::str(")");
    }
    return py::str(type + "(") + (what.empty() ? py::str("") : py::repr(what)) + py::str(")");
}

}  // namespace

std::optional<std::tuple<py::str, int64_t>> _branch(const py::str& objasm) {
    static const py::re::Pattern br = py::re::compile(py::str("([a-z]+) 0x([0-9a-f]+)$"));
    const py::str s = ws_re().sub(py::str(" "), objasm).strip();
    const auto m = br.match(s);
    if (m && (_JMP.contains(m->group(1)) || _JCC.contains(m->group(1)) || _CALL.contains(m->group(1)))) {
        return std::make_tuple(m->group(1), py::int_parse(m->group(2), 16));
    }
    return std::nullopt;
}

py::str _procname(const segment::Seg& sg, int64_t off) {
    static const py::re::Pattern bad = py::re::compile(py::str("[^A-Za-z0-9_$?@]"));
    std::optional<py::str> nm;
    try {
        nm = sg.name_at(off);
    } catch (const std::exception&) {
        nm = std::nullopt;
    }
    if (nm && !nm->empty()) {
        py::str s = bad.sub(py::str("_"), *nm);
        if (s.at_(0) >= U'0' && s.at_(0) <= U'9') s = py::str("_") + s;
        return s;
    }
    return py::percent(py::str("sub_%04x"), off);
}

Emitted _emit(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs_in, int64_t seg_i,
              const py::set<int64_t>& force_db, const py::set<int64_t>& data_off) {
    (void)seg_i;
    static const py::re::Pattern far_re = py::re::compile(py::str("(retf|ret f|lret)"));
    std::vector<int64_t> funcs;
    if (!funcs_in) {
        for (int64_t s : sg.funcs.keys())
            if (start <= s && s < end) funcs.push_back(s);
    } else {
        for (int64_t s : *funcs_in) funcs.push_back(s);
    }
    std::sort(funcs.begin(), funcs.end());
    if (funcs.empty()) {
        // a segment the carve found no function starts in (e.g. code that does not open with `push bp'): treat the whole range as
        // one function so it is emitted rather than dropped to an empty segment (0 _TEXT).
        funcs.push_back(start);
    }
    py::set<int64_t> fstart;
    for (int64_t s : funcs) fstart.add(s);
    py::dict<int64_t, int64_t> fend;
    for (size_t i = 0; i < funcs.size(); ++i) fend.set(funcs[i], i + 1 < funcs.size() ? funcs[i + 1] : end);

    std::vector<std::pair<int64_t, py::str>> rows;
    for (const auto& oa : sg.rows)
        if (start <= std::get<0>(oa) && std::get<0>(oa) < end) rows.emplace_back(std::get<0>(oa), std::get<1>(oa));
    py::dict<int64_t, int64_t> off2idx;
    for (size_t i = 0; i < rows.size(); ++i) off2idx.set(rows[i].first, static_cast<int64_t>(i));
    std::vector<int64_t> row_starts;
    for (const auto& r : rows) row_starts.push_back(r.first);
    std::sort(row_starts.begin(), row_starts.end());
    const py::bytes& code = sg.code;

    // --- pass 1: collect every direct-branch target, classify in-range vs extern
    py::dict<int64_t, py::str> targets;   // off -> kind ('func' | 'local' | 'extern')
    for (const auto& r : rows) {
        const auto b = _branch(r.second);
        if (!b) continue;
        const int64_t tgt = std::get<1>(*b);
        if (fstart.contains(tgt)) {
            targets.set(tgt, py::str("func"));
        } else if (start <= tgt && tgt < end) {
            if (!targets.contains(tgt)) targets.set(tgt, py::str("local"));
        } else {
            targets.set(tgt, py::str("extern"));
        }
    }
    auto label_of = [&](int64_t tgt) -> std::optional<py::str> {
        if (!targets.contains(tgt)) return std::nullopt;
        const py::str& k = targets.at(tgt);
        if (k == py::str("func")) return _procname(sg, tgt);
        if (k == py::str("local")) return py::percent(py::str("L_%04x"), tgt);
        if (k == py::str("extern")) return _procname(sg, tgt);   // a cross-range near ref
        return std::nullopt;
    };
    std::vector<int64_t> externs;
    for (const auto& kv : targets.items())
        if (std::get<1>(kv) == py::str("extern")) externs.push_back(std::get<0>(kv));
    std::sort(externs.begin(), externs.end());

    // --- pass 2: emit each function
    py::list<py::str> body;
    py::set<int64_t> unresolved;
    py::set<int64_t> local_labels;
    for (const auto& kv : targets.items())
        if (std::get<1>(kv) == py::str("local")) local_labels.add(std::get<0>(kv));
    if (!funcs.empty() && funcs[0] > start) {   // range opened mid-function: db the lead-in
        recon::_emit_db(body, code.slice(start, funcs[0]), start, py::str("data/lead-in"));
    }
    for (int64_t s : funcs) {
        const int64_t e = fend.at(s);
        const py::str name = _procname(sg, s);
        // near vs far from the terminating instruction (retf/ret far -> FAR)
        bool is_far = false;
        for (const auto& r : rows)
            if (s <= r.first && r.first < e && far_re.match(r.second.strip())) is_far = true;
        body.append(py::percent(py::str("cProc   %s,<%s>"), name, is_far ? py::str("FAR") : py::str("NEAR")));
        body.append(py::str("cBegin  nogen"));
        // Walk EVERY byte of [s,e): a covered offset emits its instruction (live, symbolic, or db), an uncovered gap is db'd up to
        // the next covered offset -- so no byte is ever dropped even where objdump mis-decodes a run.
        int64_t pos = s;
        while (pos < e) {
            if (local_labels.contains(pos)) body.append(py::percent(py::str("L_%04x:"), pos));
            if (!off2idx.contains(pos)) {   // uncovered bytes -> db
                const size_t j = static_cast<size_t>(std::upper_bound(row_starts.begin(), row_starts.end(), pos) - row_starts.begin());
                int64_t nxt = std::min(j < row_starts.size() ? row_starts[j] : e, e);
                if (nxt <= pos) nxt = e;
                recon::_emit_db(body, code.slice(pos, nxt), pos, py::str("data/uncovered"));
                pos = nxt;
                continue;
            }
            const size_t i = static_cast<size_t>(off2idx.at(pos));
            const int64_t off = rows[i].first;
            const py::str& a = rows[i].second;
            const int64_t nxt = std::min(i + 1 < rows.size() ? rows[i + 1].first : e, e);
            const py::bytes raw = code.slice(off, nxt);
            const auto br = _branch(a);
            const bool far_ins = !raw.empty() && (raw[0] == 0x9A || raw[0] == 0xEA);
            if (force_db.contains(off) || data_off.contains(off) || far_ins || a.contains(py::str("(bad)")) || raw.size() > 8) {
                // pinned as data: a diverger, a safe-data region, a far seg:off pointer, or a run objdump collapsed -- emit the
                // shipped bytes.
                recon::_emit_db(body, raw, off, recon::to_masm(a, [](int64_t) -> std::optional<py::str> { return std::nullopt; }));
                pos = nxt;
                continue;
            }
            std::optional<py::str> emitted;
            if (br) {
                const py::str& op = std::get<0>(*br);
                const int64_t tgt = std::get<1>(*br);
                const std::optional<py::str> lbl = label_of(tgt);
                if (lbl && !lbl->empty()) {
                    const bool is_short = raw.size() == 2;   // eb/7x rel8
                    if (_JMP.contains(op)) {
                        emitted = is_short ? py::percent(py::str("jmp     short %s"), *lbl) : py::percent(py::str("jmp     %s"), *lbl);
                    } else if (_CALL.contains(op)) {
                        emitted = py::percent(py::str("call    %s"), *lbl);
                    } else {   // jcc: rel8 short, rel16 needs .386
                        emitted = py::percent(py::str("%-7s %s"), op, is_short ? py::str("short ") + *lbl : *lbl);
                    }
                    if (targets.at(tgt) == py::str("extern")) {   // disp bytes are unresolved
                        for (int64_t u = off + 1; u < nxt; ++u) unresolved.add(u);
                    }
                }
            }
            if (emitted) {
                body.append(py::percent(py::str("        %-30s ; %04x"), *emitted, off));
            } else {
                body.append(py::percent(py::str("        %-30s ; %04x"), recon::to_masm(a, [](int64_t) -> std::optional<py::str> { return std::nullopt; }), off));
            }
            pos = nxt;
        }
        body.append(py::str("cEnd    nogen"));
        body.append(py::str(""));
    }

    py::list<py::str> ext_lines;
    for (int64_t t : externs) ext_lines.append(py::str("        externNP ") + _procname(sg, t));
    // (_PREAMBLE + body: a new list; ll = _PREAMBLE would share, and extend, the constant)
    py::list<py::str> all = _PREAMBLE + body + ext_lines;
    all.extend(py::list<py::str>{py::str("sEnd    CODE"), py::str("        end"), py::str("")});
    return Emitted{py::join(py::str("\n"), all), unresolved};
}

py::str _stage_cmacros(const py::str& root) {
    const py::str tc = vxd::masm::_stage_toolchain(root);
    const py::str incdst = py::os::path::join(tc, py::str("inc"));
    py::os::makedirs(incdst, 0777, true);
    const py::str inc16 = base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("INC16")});
    for (const py::str& f : py::glob::glob(inc16 + py::str("/*.INC"))) {
        const py::str d = py::os::path::join(incdst, py::os::path::basename(f));
        if (!py::os::path::exists(d)) py::shutil::copy(f, d);
    }
    return tc;
}

AsmResult assemble(const py::str& asm_text, const py::str& root) {
    namespace dt = build::dos_tools;
    const py::str tc = _stage_cmacros(root);
    dt::Mounts mounts;
    mounts.append({py::str("c"), tc});
    py::dict<py::str, dt::FileData> files;
    files.set(py::str("m.asm"), dt::FileData(asm_text));
    const auto r = dt::run(mounts, py::list<py::str>{py::str("c:\\ml.exe /c /Ic:\\inc /Fom.obj /Flm.lst m.asm > a.txt")},
                           py::list<py::str>{py::str("m.obj"), py::str("m.lst"), py::str("a.txt")}, 300, files);
    const py::dict<py::str, py::bytes>& out = std::get<1>(r);
    const std::optional<py::str>& work = std::get<2>(r);
    auto rd = [&](const char* n) {
        if (!work) return py::str("");
        const py::str p = py::os::path::join(*work, py::str(n));
        return py::os::path::exists(p) ? read_replace(p) : py::str("");
    };
    AsmResult res;
    if (out.contains(py::str("m.obj"))) res.obj = out.at(py::str("m.obj"));
    res.lst = rd("m.lst");
    res.log = rd("a.txt");
    if (work) py::shutil::rmtree(*work, true);   // (the work folder was left behind: one temp folder per call)
    return res;
}

py::str _nasm_bin() {
    py::str configured;
    try {
        configured = py::settings::get_str(py::str("nasm"));
    } catch (const std::exception&) {
        configured = py::str("");
    }
    if (!configured.empty()) return configured;
    const std::optional<py::str> env = py::os::getenv(py::str("NASM_MASM"));
    if (env && !env->empty()) return *env;
    return py::str("nasm.exe");
}

py::str _nasm_inc() {
    const py::str d = py::os::path::join(py::tempfile::gettempdir(), py::str("ne_idiom_nasm_inc"));
    py::os::makedirs(d, 0777, true);
    // (the shim is in <project>/test/masm: beside nasm.exe when it is in the project folder, two folders up from
    // <project>/build/bin/nasm.exe -- the Python looked beside the exe only, so the in-repo build never found it and every
    // `include cmacros.inc' failed: the whole nasm backend db-pinned everything; the Python was changed alike)
    const py::str bindir = py::os::path::dirname(_nasm_bin());
    py::str shim;
    for (const py::str& base : {bindir, py::os::path::dirname(py::os::path::dirname(bindir))}) {
        shim = py::os::path::join(base, py::str("test"), py::str("masm"), py::str("cmacros_shim.inc"));
        if (py::os::path::exists(shim)) break;
    }
    const py::str dst = py::os::path::join(d, py::str("cmacros_shim.inc"));
    if (py::os::path::exists(shim) && (!py::os::path::exists(dst) || py::os::path::getmtime(shim) > py::os::path::getmtime(dst))) {
        py::shutil::copy(shim, dst);
    }
    return d;
}

AsmResult assemble_nasm(const py::str& asm_text) {
    const py::str inc = _nasm_inc();
    const py::str work = py::tempfile::mkdtemp(std::nullopt, py::str("ne_nasm_"));
    const py::str src = py::os::path::join(work, py::str("m.asm"));
    const py::str obj = py::os::path::join(work, py::str("m.obj"));
    const py::str lst = py::os::path::join(work, py::str("m.lst"));
    write_text(src, asm_text, "\n");
    py::str log;
    try {
        py::subprocess::RunOptions o;
        o.timeout = int64_t(120);
        const py::subprocess::CompletedProcess p = py::subprocess::run(
            py::list<py::str>{_nasm_bin(), py::str("--masm"), py::str("-f"), py::str("obj"), py::str("-I"), inc + py::str("\\"), py::str("-l"), lst,
                              py::str("-o"), obj, src},
            o);
        log = (p.stderr_ ? *p.stderr_ : py::str("")) + (p.stdout_ ? *p.stdout_ : py::str(""));
    } catch (const py::Exception& e) {
        log = repr_of(e);
    }
    AsmResult res;
    if (py::os::path::exists(obj)) {
        py::file f = py::open_rb(obj);
        res.obj = f.read();
        f.close();
    }
    res.lst = py::os::path::exists(lst) ? read_replace(lst) : py::str("");
    py::shutil::rmtree(work, true);   // don't leak a tempdir per pass
    res.log = log;
    return res;
}

const py::str _NLST("^\\s*\\d+\\s+([0-9A-Fa-f]{8})\\s+([0-9A-Fa-f]{2})");

std::tuple<py::set<int64_t>, py::set<int64_t>> _nasm_divergers(const py::str& lst, const py::bytes& our, const py::set<int64_t>& omask,
                                                               const py::bytes& shipped, const py::set<int64_t>& smask,
                                                               const py::dict<int64_t, int64_t>& spans, int64_t start) {
    static const py::re::Pattern nlst = py::re::compile(_NLST);
    static const py::re::Pattern cmt = py::re::compile(py::str(";\\s*([0-9a-f]{4})\\b"));
    std::vector<std::pair<int64_t, std::optional<int64_t>>> pairs;
    for (const py::str& ln : py::splitlines(lst)) {
        const auto m = nlst.match(ln);
        if (!m) continue;
        const auto co = cmt.search(ln);
        pairs.emplace_back(py::int_parse(m->group(1), 16), co ? std::optional<int64_t>(py::int_parse(co->group(1), 16)) : std::nullopt);
    }
    py::set<int64_t> lendiff, reencode;
    for (size_t i = 0; i < pairs.size(); ++i) {
        const int64_t moff = pairs[i].first;
        const std::optional<int64_t> ooff = pairs[i].second;
        if (!ooff || !spans.contains(*ooff)) continue;
        const int64_t mlen = i + 1 < pairs.size() ? pairs[i + 1].first - moff : our.size() - moff;
        const int64_t olen = spans.at(*ooff) - *ooff;
        const int64_t oi = *ooff - start;
        if (mlen != olen) {
            lendiff.add(*ooff);
            continue;
        }
        for (int64_t k = 0; k < olen; ++k) {
            if (omask.contains(moff + k) || smask.contains(oi + k)) continue;
            if (moff + k >= our.size() || oi + k >= shipped.size()) break;
            if (our[moff + k] != shipped[oi + k]) {
                reencode.add(*ooff);
                break;
            }
        }
    }
    return std::make_tuple(lendiff, reencode);
}

recon::Reconciled reconcile_range(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs, const py::str& root,
                                  int64_t max_passes, const py::str& backend) {
    static const py::re::Pattern err_nasm = py::re::compile(py::str(":(\\d+):\\s*(?:error|fatal)"), py::re::I);
    static const py::re::Pattern err_ml = py::re::compile(py::str("\\((\\d+)\\)\\s*:\\s*(?:error|fatal)"), py::re::I);
    static const py::re::Pattern off_re = py::re::compile(py::str(";\\s*([0-9a-f]{4})\\b"));
    const bool nasm = backend == py::str("nasm");
    const py::bytes shipped = sg.code.slice(start, end);
    const py::set<int64_t> smask;   // (getattr(sg, "mask", set()): a Seg has none)
    const auto dspans = recon::_safe_data_spans(sg, start, end);
    py::set<int64_t> data_off;
    for (const auto& d : dspans)
        for (const auto& oa : sg.rows)
            if (std::get<0>(d) <= std::get<0>(oa) && std::get<0>(oa) < std::get<1>(d)) data_off.add(std::get<0>(oa));
    std::vector<int64_t> rows;
    for (const auto& oa : sg.rows)
        if (start <= std::get<0>(oa) && std::get<0>(oa) < end && !data_off.contains(std::get<0>(oa))) rows.push_back(std::get<0>(oa));
    std::sort(rows.begin(), rows.end());
    py::dict<int64_t, int64_t> spans;
    for (size_t i = 0; i < rows.size(); ++i) spans.set(rows[i], i + 1 < rows.size() ? rows[i + 1] : end);
    std::vector<int64_t> funcs_list;
    if (funcs) {
        for (int64_t f : *funcs)
            if (start <= f && f < end) funcs_list.push_back(f);
    } else {
        for (int64_t f : sg.funcs.keys())
            if (start <= f && f < end) funcs_list.push_back(f);
    }
    std::sort(funcs_list.begin(), funcs_list.end());

    // the function range [s,e) containing absoff (for whole-function db)
    auto func_of = [&](int64_t absoff) -> std::optional<std::tuple<int64_t, int64_t>> {
        const int64_t j = static_cast<int64_t>(std::upper_bound(funcs_list.begin(), funcs_list.end(), absoff) - funcs_list.begin()) - 1;
        if (0 <= j && j < static_cast<int64_t>(funcs_list.size())) {
            const int64_t s = funcs_list[static_cast<size_t>(j)];
            return std::make_tuple(s, static_cast<size_t>(j) + 1 < funcs_list.size() ? funcs_list[static_cast<size_t>(j) + 1] : end);
        }
        return std::nullopt;
    };
    const py::re::Pattern& err_re = nasm ? err_nasm : err_ml;
    py::set<int64_t> force_db, errored;
    std::optional<segdiff::DiffResult> res;
    py::str asm_ = py::str("");
    for (int64_t pass = 0; pass < max_passes; ++pass) {
        const Emitted em = _emit(sg, start, end, funcs, 1, force_db, data_off);
        asm_ = em.text;
        const py::list<py::str> asm_lines = py::splitlines(asm_);
        const AsmResult r = nasm ? assemble_nasm(asm_) : assemble(asm_, root);
        // localize any MASM error to the offending original offset via its `; NNNN' comment and db just that instruction (it is
        // data MASM cannot encode).
        py::set<int64_t> errs;
        for (const auto& m : err_re.finditer(r.log + py::str("\n") + r.lst)) {
            const int64_t ln = py::int_parse(m.group(1));
            if (1 <= ln && ln <= asm_lines.size()) {
                if (const auto cm = off_re.search(asm_lines[ln - 1])) errs.add(py::int_parse(cm->group(1), 16));
            }
        }
        if (!r.obj || r.obj->empty() || !errs.empty()) {
            const py::set<int64_t> fresh = errs - force_db;
            if (!fresh.empty()) {
                force_db.update(fresh);
                errored.update(fresh);
                continue;
            }
            if (force_db.size() >= static_cast<int64_t>(rows.size())) break;
            force_db = py::set<int64_t>::from(rows);
            continue;
        }
        const auto seg_ = formats::omf::code_segment(*r.obj);
        const py::bytes& our = std::get<1>(seg_);
        py::set<int64_t> omask = std::get<2>(seg_);
        for (int64_t u : em.unresolved) omask.add(u - start);
        res = segdiff::diff(our, omask, shipped, smask);
        if (res->exact) break;
        const auto dv = nasm ? _nasm_divergers(r.lst, our, omask, shipped, smask, spans, start)
                             : recon::_divergers(r.lst, our, omask, shipped, smask, spans, start);
        const py::set<int64_t> new_len = std::get<0>(dv) - force_db;   // phase 1: fix length-differers first
        if (!new_len.empty()) {
            force_db.update(new_len);
            continue;
        }
        const py::set<int64_t> new_re = std::get<1>(dv) - force_db;   // phase 2: same-length re-encodes
        if (!new_re.empty()) {
            force_db.update(new_re);
            continue;
        }
        // last resort: the listing can't localize the divergence (a mis-decoded region, e.g. an inline jump table objdump read as
        // code). db the whole function containing the first diff -- pure db is byte-exact in a contiguous emission -- which re-aligns
        // everything after it.
        std::optional<std::tuple<int64_t, int64_t>> fr;
        if (res->first_diff) fr = func_of(start + *res->first_diff);
        if (fr) {
            py::set<int64_t> rr;
            for (int64_t o : rows)
                if (std::get<0>(*fr) <= o && o < std::get<1>(*fr)) rr.add(o);
            if (!(rr - force_db).empty()) {
                force_db.update(rr);
                continue;
            }
        }
        break;
    }
    int64_t code_rows = 0;
    for (int64_t o : rows)
        if (!errored.contains(o)) code_rows += 1;
    return recon::Reconciled{asm_, res, force_db - errored, code_rows};
}

Verified verify_range(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs, const py::str& root,
                      const py::str& backend) {
    const recon::Reconciled r = reconcile_range(sg, start, end, funcs, root, 60, backend);
    const bool exact = r.res && r.res->exact;
    Verified out;
    out.ok = exact;
    out.n = end - start;
    out.n_code = r.n_code;
    out.db = r.force_db.size();
    out.live = r.n_code - r.force_db.size();
    out.asm_ = r.asm_;
    if (!exact && r.res) out.res = r.res;
    return out;
}

namespace {
py::str pyrepr_bool(bool b) { return py::str(b ? "True" : "False"); }
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    (void)err;
    const py::str module = argv.size() > 0 ? argv[0] : py::str("KRNL386.EXE");
    const int64_t seg_i = argv.size() > 1 ? py::int_parse(argv[1]) : 1;
    const int64_t lo = argv.size() > 2 ? py::int_parse(argv[2], 0) : 0x04a4;
    const py::str hi_arg = argv.size() > 3 ? argv[3] : py::str("0x4fa");   // a number or "whole"
    const std::optional<py::str> save = py::os::getenv(py::str("NE_SAVE"));   // write the .asm here
    py::str path;
    try {
        path = _target().resolve(module);
    } catch (const std::exception&) {
        const std::optional<py::str> h = base::corpus::retail(module);
        path = h ? *h : module;
    }
    const formats::ne::NEFile ne(path);
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    // apply recovered real internal names if a names.json exists for this module (see debug_names.py) -- a pure label rename,
    // byte-exactness is unaffected.
    py::dict<int64_t, py::dict<int64_t, py::str>> override_;
    const py::str tag = std::get<0>(py::os::path::splitext(py::os::path::basename(module))).lower();
    const py::str names_json = state_(py::str("idiomatic"), tag, py::str("names.json"));
    if (py::os::path::exists(names_json)) {
        override_ = symbols::debug_build::load_override(names_json);
        int64_t total = 0;
        for (const auto& v : override_.values()) total += v.size();
        out << py::percent(py::str("applying %d recovered real names from %s"), total, names_json).utf8() << "\n";
    }
    std::unique_ptr<segment::Seg> sg;
    const auto segs = ne.segments();
    for (int64_t i = 1; i <= segs.size(); ++i) {
        if (i == seg_i && segs[i - 1].kind == py::str("CODE")) {
            std::optional<py::dict<int64_t, py::str>> ov;
            if (override_.contains(i)) ov = override_.at(i);
            sg = std::make_unique<segment::Seg>(ne, i, o, ov);
            break;
        }
    }
    if (!sg) {
        out << "no code segment " << seg_i << "\n";
        return 1;
    }
    const int64_t hi = hi_arg == py::str("whole") ? sg->code.size() : py::int_parse(hi_arg, 0);
    const Verified r = verify_range(*sg, lo, hi);
    out << py::percent(py::str("=== %s seg%d [0x%x,0x%x) ==="), module, seg_i, lo, hi).utf8() << "\n";
    if (r.ok) {
        const double pct = r.n_code ? 100.0 * static_cast<double>(r.live) / static_cast<double>(r.n_code) : 100.0;
        out << py::percent(py::str("*** BYTE-EXACT (%d bytes) -- %d/%d instructions live idiomatic, %d db-pinned (%.1f%% live) ***"), r.n, r.live,
                           r.n_code, r.db, pct)
                   .utf8()
            << "\n";
        if (save) {
            py::os::makedirs(py::os::path::dirname(py::os::path::abspath(*save)), 0777, true);
            write_text(*save, r.asm_, "\r\n");
            int64_t lines = 0;
            for (int64_t k = 0; k < r.asm_.size(); ++k)
                if (r.asm_.at_(k) == U'\n') lines += 1;
            out << py::percent(py::str("saved idiomatic source -> %s (%d lines)"), *save, lines).utf8() << "\n";
        }
    } else {
        // print("NOT exact:", {k: v for k, v in r.items() if k != "asm"})   -- the dict as Python prints it
        std::string d = "{'ok': " + pyrepr_bool(r.ok).utf8() + ", 'n': " + std::to_string(r.n) + ", 'n_code': " + std::to_string(r.n_code) +
                        ", 'db': " + std::to_string(r.db) + ", 'live': " + std::to_string(r.live);
        if (r.res) {
            const segdiff::DiffResult& x = *r.res;
            d += ", 'res': {'exact': " + pyrepr_bool(x.exact).utf8() + ", 'length_match': " + pyrepr_bool(x.length_match).utf8() +
                 ", 'our_len': " + std::to_string(x.our_len) + ", 'shipped_len': " + std::to_string(x.shipped_len) +
                 ", 'n_diff': " + std::to_string(x.n_diff) + ", 'first_diff': " + (x.first_diff ? std::to_string(*x.first_diff) : std::string("None")) +
                 ", 'masked': " + std::to_string(x.masked) + "}";
        }
        d += "}";
        out << "NOT exact: " << d << "\n";
    }
    return 0;
}

}  // namespace codebreaker::source::ne_asm::idiom
