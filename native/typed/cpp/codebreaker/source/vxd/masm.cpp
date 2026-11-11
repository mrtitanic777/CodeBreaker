// codebreaker/source/vxd/masm.cpp -- typed C++ of codebreaker.source.vxd.masm (see masm.hpp).

#include "codebreaker/source/vxd/masm.hpp"

#include <algorithm>
#include <functional>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/build/native_dos.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::vxd::masm {

namespace dis = codebreaker::source::vxd::disassembly;
namespace paths = codebreaker::base::corpus;

const py::list<py::str> _ML_REF = {py::str("Microsoft Windows 95 DDK"), py::str("MASM611C")};
const py::str _DOSXNT_SRC = py::str("toolchains/period/compilers/msvc-1.52c/BIN/DOSXNT.EXE");

py::str _TC() {
    return py::os::path::join(py::os::getenv(py::str("TEMP"), py::str("/tmp")), py::str("vxdtc"));
}

namespace {

// the text a label has, "" for none (the Python reads a missing label and an empty one alike)
py::str label_at(const VxDDisasm& D, int64_t oi, int64_t a) {
    const auto l = D.labels.get(dis::Place(oi, a));
    return l ? *l : py::str("");
}

// open(path, "w", newline="\r\n").write(text)
void write_crlf(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str("\r\n"));
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

// str(e) of a caught exception
py::str what_of(const std::exception& e) { return py::str(std::string(e.what())); }

}  // namespace

py::str _ddk_root() {
    py::list<py::str> cands;
    cands.append(paths::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK")}));
    cands.append(py::os::path::join(py::os::path::dirname(py::settings::checkout_root()), py::str("Windows95"), py::str("design"),
                                    py::str("reference"), py::str("Microsoft Windows 95 DDK")));
    for (const py::str& c : cands) {
        if (!c.empty() && py::os::path::isdir(c)) {
            return c;
        }
    }
    return cands[0];
}

py::str native_ml() {
    const py::str cand = py::os::path::join(py::settings::checkout_root(), py::str("toolchains"), py::str("period"), py::str("sdks"),
                                            py::str("nt351-ddk"), py::str("DDK"), py::str("BIN"), py::str("I386"), py::str("FREE"),
                                            py::str("ML.EXE"));
    return py::os::path::exists(cand) ? cand : py::os::path::join(_ddk_root(), py::str("MASM611C"), py::str("ML.EXE"));
}

py::str _stage_toolchain(const py::str& root) {
    const py::str tc = _TC();
    py::os::makedirs(tc, 0777, true);
    const py::str ml = paths::reference(_ML_REF);
    for (const py::str& src : {py::os::path::join(ml, py::str("ML.EXE")), py::os::path::join(ml, py::str("ML.ERR")),
                               py::os::path::join(root, _DOSXNT_SRC)}) {
        const py::str dst = py::os::path::join(tc, py::os::path::basename(src));
        if (!py::os::path::exists(dst) && py::os::path::exists(src)) {
            py::shutil::copy(src, dst);
        }
    }
    return tc;
}

// ---- MASM text for a single instruction -----------------------------------------
const py::dict<py::str, py::str> _MNEM = {{py::str("pushal"), py::str("pushad")}, {py::str("popal"), py::str("popad")}};
const py::set<py::str> _STRING_OPS = {py::str("movsb"), py::str("movsw"), py::str("movsd"), py::str("stosb"), py::str("stosw"),
                                      py::str("stosd"), py::str("lodsb"), py::str("lodsw"), py::str("lodsd"), py::str("scasb"),
                                      py::str("scasw"), py::str("scasd"), py::str("cmpsb"), py::str("cmpsw"), py::str("cmpsd"),
                                      py::str("insb"),  py::str("insw"),  py::str("insd"),  py::str("outsb"), py::str("outsw"),
                                      py::str("outsd")};

// jmp/jcc have both a 2-byte rel8 (SHORT) and a 5/6-byte rel32 (NEAR) form; MASM auto-picks the
// short one when the target fits, which drifts from a compiler that emitted the long form (and
// vice-versa). Emitting the explicit size pins ML to the oracle's encoding. call is always near;
// jcxz/loop are always short -- neither takes a size override.
const py::set<py::str> _SHORTABLE = {py::str("jmp"), py::str("je"),   py::str("jne"),  py::str("jz"),   py::str("jnz"),  py::str("ja"),
                                     py::str("jae"), py::str("jb"),   py::str("jbe"),  py::str("jg"),   py::str("jge"),  py::str("jl"),
                                     py::str("jle"), py::str("jo"),   py::str("jno"),  py::str("js"),   py::str("jns"),  py::str("jp"),
                                     py::str("jnp"), py::str("jc"),   py::str("jnc"),  py::str("jpe"),  py::str("jpo"),  py::str("jna"),
                                     py::str("jnae"), py::str("jnb"), py::str("jnbe"), py::str("jng"),  py::str("jnge"), py::str("jnl"),
                                     py::str("jnle")};

py::str _masm_ops(const py::str& ops) {
    // re.sub(r"0x([0-9a-fA-F]+)", _hex, ops): each hex number gets MASM's 0..h spelling
    auto is_hex = [](char32_t c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); };
    const std::u32string& s = ops.u32();
    std::u32string out;
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == U'0' && i + 2 < s.size() + 0 && s[i + 1] == U'x' && is_hex(s[i + 2])) {
            size_t j = i + 2;
            while (j < s.size() && is_hex(s[j])) {
                ++j;
            }
            const std::u32string d = s.substr(i + 2, j - i - 2);
            const bool letter = (d[0] >= 'a' && d[0] <= 'f') || (d[0] >= 'A' && d[0] <= 'F');
            if (letter) {
                out.push_back(U'0');
            }
            out += d;
            out.push_back(U'h');
            i = j;
        } else {
            out.push_back(s[i]);
            ++i;
        }
    }
    return py::str(out);
}

std::optional<py::str> _masm_line(const VxDDisasm& D, int64_t oi, const Row& row) {
    const py::str mnem = _MNEM.get(row.mnem, row.mnem);
    // A far absolute jmp/call decodes (in Capstone) as `ljmp seg:off' / `lcall seg:off', which is neither
    // MASM nor NASM syntax and mostly appears where data was walked as code. Byte-preserve it exactly.
    if (row.mnem == "ljmp" || row.mnem == "lcall") {
        return std::nullopt;
    }
    const py::list<py::str> words = py::split(mnem);
    if (words.empty()) {
        throw py::IndexError("list index out of range");
    }
    if (_STRING_OPS.contains(words[-1])) {                      // `lodsb al, [esi]` -> `lodsb` (+ rep)
        return py::str("        ") + mnem.rstrip();
    }
    auto [ops, resolved] = dis::_render_ops(D, oi, row);
    if (dis::_BRANCHES.contains(mnem) && !resolved && dis::_direct_target_row(row)) {
        return std::nullopt;                                    // direct branch to an unlabelled address
    }
    ops = _masm_ops(ops);
    if (_SHORTABLE.contains(mnem) && resolved) {                // pin the branch size to the oracle's
        ops = py::str(row.size == 2 ? "SHORT " : "NEAR PTR ") + ops;
    }
    return py::percent(py::str("        %-6s %s"), mnem, ops).rstrip();
}

py::str _db(const py::bytes& bs) {
    py::list<py::str> parts;
    for (int64_t b : bs) {
        parts.append(py::percent(py::str("0%02Xh"), b));
    }
    return py::str("        db  ") + py::join(py::str(", "), parts);
}

const py::str _ABS_MEM = py::str("\\[\\s*(?:0x[0-9a-fA-F]+|[0-9]+)\\s*\\]");   // [number] -- no base/index register

// True if an operand is a bare absolute-address memory reference `[0x1234]` / `[0]`. MASM is unreliable with these
// (`mov eax,[disp]` is the 5-byte moffs form it may re-encode to 6 bytes; `add [disp],reg` it rejects outright), so they
// are byte-preserved.
bool has_abs_mem(const Row& row) {
    static const py::re::Pattern pat = py::re::compile(_ABS_MEM);
    return pat.search(row.ops).has_value();
}

const py::set<int64_t> _SEG_PREFIX = {0x2E, 0x26, 0x36, 0x3E, 0x64, 0x65};       // CS ES SS DS FS GS override bytes

// True if the instruction carries a segment-override prefix (2E/26/36/3E/64/65). Ring-0 VxD code uses these to reach
// V86/other segments, and often carries a redundant CS: prefix that MASM silently drops -- a length drift that cascades
// into every following call/jmp target. Pre-forcing byte-preserves the prefix exactly.
bool has_seg_prefix(const Row& row) {
    return !row.raw.empty() && _SEG_PREFIX.contains(row.raw[0]);
}

std::tuple<py::list<py::str>, LineMap> object_body(const VxDDisasm& D, int64_t oi, const std::optional<py::set<int64_t>>& force_raw_in,
                                                   int64_t base_line, const std::optional<py::dict<int64_t, py::str>>& extra_labels_in,
                                                   bool probe, const std::optional<std::tuple<int64_t, int64_t>>& crange) {
    const py::set<int64_t> force_raw = force_raw_in ? *force_raw_in : py::set<int64_t>();
    const py::dict<int64_t, py::str> extra_labels = extra_labels_in ? *extra_labels_in : py::dict<int64_t, py::str>();
    const py::bytes& code = D.code.at(oi);
    const int64_t n = code.size();                              // actual page bytes (may be < vsize when
    const int64_t lo = crange ? std::get<0>(*crange) : 0;       //  the object has a zero-fill/BSS tail)
    const int64_t hi = crange ? std::get<1>(*crange) : n;
    py::list<py::str> L;
    LineMap linemap;
    int64_t a = lo;
    while (a < hi) {
        if (extra_labels.contains(a)) {
            L.append(py::str("public ") + extra_labels.at(a));
            L.append(extra_labels.at(a) + py::str(" label byte"));
        }
        if (!label_at(D, oi, a).empty()) {
            L.append(label_at(D, oi, a) + py::str(":"));
        }
        const bool is_insn = D.insns.contains(dis::Place(oi, a));
        if (!is_insn) {                                         // data byte(s)
            int64_t b = a;
            while (b < hi && !D.insns.contains(dis::Place(oi, b)) && !(b > a && !label_at(D, oi, b).empty()) &&
                   !(b > a && extra_labels.contains(b))) {
                b += 1;
            }
            for (int64_t i = a; i < b; i += 16) {
                L.append(_db(code.slice(i, std::min<int64_t>(i + 16, b))));
            }
            a = b;
            continue;
        }
        const Row& row = D.insns.at(dis::Place(oi, a));
        // An extra_label (e.g. VXD_DDB) can land STRICTLY INSIDE a decoded instruction when data was walked as code --
        // the DDB's rva is the 2nd byte of a 2-byte insn. Stepping by instruction size would skip that offset and the label
        // would never be emitted. Emit this instruction as raw db, split at each interior label offset so the label sits
        // exactly on its byte.
        py::list<int64_t> interior;
        for (int64_t o : py::sorted(extra_labels.keys())) {
            if (a < o && o < a + row.size) {
                interior.append(o);
            }
        }
        if (!interior.empty()) {
            py::list<int64_t> segs;
            segs.append(a);
            segs.extend(interior);
            segs.append(a + row.size);
            for (int64_t k = 0; k + 1 < segs.size(); ++k) {
                const int64_t s = segs[k], e = segs[k + 1];
                if (extra_labels.contains(s)) {
                    L.append(py::str("public ") + extra_labels.at(s));
                    L.append(extra_labels.at(s) + py::str(" label byte"));
                }
                L.append(_db(code.slice(s, e)));
            }
            a += row.size;
            continue;
        }
        if (probe) {
            L.append(py::percent(py::str("public Q%x"), a));    // per-instruction offset probe (public so
            L.append(py::percent(py::str("Q%x:"), a));          //  it lands in the COFF symbol table)
        }
        std::optional<py::str> line;
        if (!(row.fx || row.vxdcall || force_raw.contains(a))) {
            line = _masm_line(D, oi, row);
        }
        if (!line) {
            L.append(_db(row.raw));
        } else {
            L.append(*line);
            linemap.set(base_line + L.size(), a);               // this symbolic line -> its offset
        }
        a += row.size;
    }
    return {L, linemap};
}

std::tuple<py::str, LineMap> emit_object_masm(const VxDDisasm& D, int64_t oi, const py::str& segname, const std::optional<py::set<int64_t>>& force_raw,
                                              bool probe, const std::optional<std::tuple<int64_t, int64_t>>& crange) {
    py::list<py::str> head = {py::str(".386p"), py::percent(py::str("%s segment dword public use32 'CODE'"), segname), py::str("")};
    auto [body, linemap] = object_body(D, oi, force_raw, head.size(), std::nullopt, probe, crange);
    py::list<py::str> L = head + body;
    L.append(py::percent(py::str("%s ends"), segname));
    L.append(py::str("        end"));
    L.append(py::str(""));
    return {py::join(py::str("\r\n"), L), linemap};
}

py::list<std::tuple<int64_t, int64_t>> split_ranges(const VxDDisasm& D, int64_t oi, int64_t max_ins) {
    py::list<int64_t> offs;
    for (const dis::Place& k : D.insns) {
        if (std::get<0>(k) == oi) {
            offs.append(std::get<1>(k));
        }
    }
    offs.sort();
    const int64_t n = D.code.at(oi).size();
    py::set<int64_t> fs;
    for (const auto& kv : D.labels.items()) {
        if (std::get<0>(std::get<0>(kv)) == oi && std::get<1>(kv).startswith(py::str("sub_"))) {
            fs.add(std::get<1>(std::get<0>(kv)));
        }
    }
    py::list<std::tuple<int64_t, int64_t>> ranges;
    int64_t lo = 0, count = 0;
    for (int64_t a : offs) {
        if (count >= max_ins && fs.contains(a) && a > lo) {
            ranges.append({lo, a});
            lo = a;
            count = 0;
        }
        count += 1;
    }
    ranges.append({lo, n});
    return ranges;
}

py::set<int64_t> cross_range_force(const VxDDisasm& D, int64_t oi, int64_t lo, int64_t hi) {
    py::set<int64_t> out;
    for (const auto& kv : D.insns.items()) {
        const int64_t o = std::get<0>(std::get<0>(kv)), a = std::get<1>(std::get<0>(kv));
        const Row& row = std::get<1>(kv);
        if (o != oi || !(lo <= a && a < hi)) {
            continue;
        }
        if (dis::_BRANCHES.contains(row.mnem)) {
            const auto t = dis::_direct_target_row(row);
            if (t && !(lo <= *t && *t < hi)) {
                out.add(a);
            }
        }
    }
    return out;
}

// ---- assemble + compare ----------------------------------------------------------
std::optional<Asm> _tnt_ml(const py::str& args, const py::str& masm_text, const py::str& root) {
    namespace nd = codebreaker::build::native_dos;
    if (!nd::enabled(py::str("ml"))) {
        return std::nullopt;
    }
    py::dict<py::str, nd::FileData> files;
    files.set(py::str("m.asm"), nd::FileData(masm_text));
    try {
        auto [made, log] = nd::ml(args, files, py::list<py::str>{py::str("m.obj")}, root);
        return Asm{made.get(py::str("m.obj")), log};
    } catch (const nd::NativeDosError&) {
        return std::nullopt;
    }
}

Asm assemble(const py::str& masm_text, const py::str& root) {
    if (auto got = _tnt_ml(py::str("/c /Fom.obj m.asm"), masm_text, root)) {
        return *got;
    }
    const py::str tc = _stage_toolchain(root);
    // a large all-raw object is thousands of db lines; give ML headroom (DOSBox at 20k cycles is slow) so a big
    // object's fallback assembly doesn't time out and spuriously "fail".
    const double tmo = masm_text.size() > 60000 ? 300 : 120;
    namespace dt = codebreaker::build::dos_tools;
    py::dict<py::str, dt::FileData> files;
    files.set(py::str("m.asm"), dt::FileData(masm_text));
    dt::Mounts mounts;
    mounts.append({py::str("c"), tc});
    auto [ok, out, work] = dt::run(mounts, py::list<py::str>{py::str("c:\\ml.exe /c /Fo m.obj m.asm > asm.txt")},
                                   py::list<py::str>{py::str("m.obj"), py::str("asm.txt")}, tmo, files);
    py::str log;
    if (work && py::os::path::exists(py::os::path::join(*work, py::str("asm.txt")))) {
        log = read_replace(py::os::path::join(*work, py::str("asm.txt")));
    }
    if (work) {
        py::shutil::rmtree(*work, true);
    }
    return {out.get(py::str("m.obj")), log};
}

// --- Native MASM-compatible assembler (nasm --masm) -------------------------
// When a MASM-compatible nasm is installed it drives the byte-exact convergence natively -- no DOSBox/ML round-trip --
// typically 10-20x faster, and it matches the shipped Win95 bytes even on the SIB base/index orderings where ML 6.11c
// differs. It is resolved from settings.json's `nasm`, then the NASM_MASM env override, then a `nasm-masm`/`nasm` on
// PATH. When none qualifies the code transparently falls back to ML.EXE under DOSBox.
//
// The probe assembles a mixed-case PUBLIC and checks it survives verbatim in the object.
const py::str _NASM_PROBE = py::str("bits 32\r\nglobal Q7X\r\nQ7X:\r\n mov eax, ebx\r\n");

namespace {
// a subprocess.run(args, cwd=..., capture_output=True, text=True) of the tool; its returncode is not looked at
py::subprocess::CompletedProcess run_capture(const py::list<py::str>& args, const std::optional<py::str>& cwd) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    return py::subprocess::run(args, o);
}
}  // namespace

std::optional<py::str> find_nasm() {
    static bool probed = false;
    static py::str cache;   // "" = probed, unavailable
    if (py::os::getenv(py::str("CB_FORCE_ML"))) {       // force the authentic DDK ML.EXE (DOSBox), not native nasm
        probed = true;
        cache = py::str("");
        return std::nullopt;
    }
    if (probed) {
        return cache.empty() ? std::nullopt : std::optional<py::str>(cache);
    }
    py::list<std::optional<py::str>> cands;
    try {
        cands.append(py::settings::get_opt_str(py::str("nasm")));    // settings.json
    } catch (const py::Exception&) {
    }
    cands.append(py::os::getenv(py::str("NASM_MASM")));
    cands.append(py::shutil::which(py::str("nasm-masm")));
    cands.append(py::shutil::which(py::str("nasm")));
    for (const auto& cand : cands) {
        if (!cand || cand->empty() || !py::os::path::exists(*cand)) {
            continue;
        }
        try {
            const py::str wd = py::tempfile::mkdtemp();
            const py::str ap = py::os::path::join(wd, py::str("p.asm"));
            const py::str op = py::os::path::join(wd, py::str("p.obj"));
            write_crlf(ap, _NASM_PROBE);
            run_capture(py::list<py::str>{*cand, py::str("--masm"), py::str("-f"), py::str("win32"), ap, py::str("-o"), op}, std::nullopt);
            py::bytes obj;
            if (py::os::path::exists(op)) {
                py::file f = py::open_rb(op);
                obj = f.read();
                f.close();
            }
            py::shutil::rmtree(wd, true);
            if (!obj.empty() && obj.contains(py::bytes("Q7X"))) {   // --masm accepted, COFF emitted, public case PRESERVED (ML-like)
                probed = true;
                cache = *cand;
                return cache;
            }
        } catch (const py::Exception&) {
        }
    }
    probed = true;                                              // probed, unavailable -> ML fallback
    cache = py::str("");
    return std::nullopt;
}

Asm _assemble_coff_nasm(const py::str& masm_text, const py::str& nasm) {
    const py::str wd = py::tempfile::mkdtemp();
    const py::str ap = py::os::path::join(wd, py::str("m.asm"));
    const py::str op = py::os::path::join(wd, py::str("m.obj"));
    write_crlf(ap, masm_text);
    std::optional<py::bytes> obj;
    py::str log;
    try {
        auto r = run_capture(py::list<py::str>{nasm, py::str("--masm"), py::str("-f"), py::str("win32"), ap, py::str("-o"), op}, std::nullopt);
        log = (r.stderr_ ? *r.stderr_ : py::str("")) + (r.stdout_ ? *r.stdout_ : py::str(""));
        if (py::os::path::exists(op)) {
            py::file f = py::open_rb(op);
            obj = f.read();
            f.close();
        }
    } catch (const py::Exception& e) {
        obj = std::nullopt;
        log = py::str("nasm: ") + what_of(e);
    }
    py::shutil::rmtree(wd, true);
    return {obj, log};
}

bool _native_ml_ok() {
    static std::optional<bool> cache;
    if (py::os::getenv(py::str("CB_FORCE_ML"))) {
        return false;
    }
    if (!cache) {
        try {
            cache = py::os::path::exists(native_ml());
        } catch (const py::Exception&) {
            cache = false;
        }
    }
    return *cache;
}

Asm _assemble_coff_native_ml(const py::str& masm_text, const py::str& root) {
    (void)root;
    const py::str inc = py::os::path::join(_ddk_root(), py::str("INC32"));
    const py::str ml = native_ml();
    const py::str wd = py::tempfile::mkdtemp();
    const py::str ap = py::os::path::join(wd, py::str("m.asm"));
    const py::str op = py::os::path::join(wd, py::str("m.obj"));
    write_crlf(ap, masm_text);
    std::optional<py::bytes> obj;
    py::str log;
    try {
        auto p = run_capture(py::list<py::str>{ml, py::str("-coff"), py::str("-c"), py::str("-DBLD_COFF"), py::str("-DIS_32"), py::str("-DMASM6"),
                                               py::str("-I") + inc, py::str("-Fom.obj"), py::str("m.asm")},
                             wd);
        log = (p.stdout_ ? *p.stdout_ : py::str("")) + (p.stderr_ ? *p.stderr_ : py::str(""));
        if (py::os::path::exists(op)) {
            py::file f = py::open_rb(op);
            obj = f.read();
            f.close();
        }
    } catch (const py::Exception& e) {
        obj = std::nullopt;
        log = py::str("native ml: ") + what_of(e);
    }
    py::shutil::rmtree(wd, true);
    return {obj, log};
}

Asm assemble_coff(const py::str& masm_text, const py::str& root) {
    if (auto nasm = find_nasm()) {
        return _assemble_coff_nasm(masm_text, *nasm);
    }
    if (_native_ml_ok()) {
        return _assemble_coff_native_ml(masm_text, root);
    }
    if (auto got = _tnt_ml(py::str("-coff -c -Fom.obj m.asm"), masm_text, root)) {
        return *got;
    }
    const py::str tc = _stage_toolchain(root);
    const double tmo = masm_text.size() > 60000 ? 300 : 120;
    namespace dt = codebreaker::build::dos_tools;
    py::dict<py::str, dt::FileData> files;
    files.set(py::str("m.asm"), dt::FileData(masm_text));
    dt::Mounts mounts;
    mounts.append({py::str("c"), tc});
    auto [ok, out, work] = dt::run(mounts, py::list<py::str>{py::str("c:\\ml.exe -coff -c -Fom.obj m.asm > asm.txt")},
                                   py::list<py::str>{py::str("m.obj"), py::str("asm.txt")}, tmo, files);
    py::str log;
    if (work && py::os::path::exists(py::os::path::join(*work, py::str("asm.txt")))) {
        log = read_replace(py::os::path::join(*work, py::str("asm.txt")));
    }
    if (work) {                                                 // don't accumulate mkdtemp dirs (a corpus run makes thousands)
        py::shutil::rmtree(*work, true);
    }
    return {out.get(py::str("m.obj")), log};
}

py::list<Asm> assemble_coff_batch(const py::list<py::str>& masm_list, const py::str& root, int64_t budget, int64_t maxfiles) {
    py::list<Asm> results;
    if (auto nasm = find_nasm()) {
        for (const py::str& m : masm_list) {
            results.append(_assemble_coff_nasm(m, *nasm));
        }
        return results;
    }
    if (_native_ml_ok()) {                                      // native DDK ML.EXE: no cold start either
        for (const py::str& m : masm_list) {
            results.append(_assemble_coff_native_ml(m, root));
        }
        return results;
    }
    for (int64_t i = 0; i < masm_list.size(); ++i) {
        results.append(Asm{std::nullopt, py::str("")});
    }
    py::list<int64_t> rest;
    for (int64_t i = 0; i < masm_list.size(); ++i) {            // TNT ML on our own executive: no cold start
        auto got = _tnt_ml(py::str("-coff -c -Fom.obj m.asm"), masm_list[i], root);
        if (!got) {
            rest.append(i);
        } else {
            results[i] = *got;
        }
    }
    if (rest.empty()) {
        return results;
    }
    const py::str tc = _stage_toolchain(root);
    py::list<py::list<int64_t>> batches;
    py::list<int64_t> cur;
    int64_t cl = 0;
    for (int64_t i : py::sorted(rest, [&](int64_t k) { return masm_list[k].size(); })) {
        const int64_t sz = masm_list[i].size();
        if (sz > budget) {
            batches.append(py::list<int64_t>{i});
            continue;
        }
        if (!cur.empty() && (cl + sz > budget || cur.size() >= maxfiles)) {
            batches.append(cur);
            cur = py::list<int64_t>();
            cl = 0;
        }
        cur.append(i);
        cl += sz;
    }
    if (!cur.empty()) {
        batches.append(cur);
    }
    namespace dt = codebreaker::build::dos_tools;
    for (const py::list<int64_t>& batch : batches) {
        py::dict<py::str, dt::FileData> files;
        py::list<py::str> bat, arts;
        for (int64_t j = 0; j < batch.size(); ++j) {
            const py::str fn = py::percent(py::str("m%d"), j);
            files.set(fn + py::str(".asm"), dt::FileData(masm_list[batch[j]]));
            bat.append(py::percent(py::str("c:\\ml.exe -coff -c -Fo%s.obj %s.asm > %s.txt"), fn, fn, fn));
            arts.append(fn + py::str(".obj"));
            arts.append(fn + py::str(".txt"));
        }
        int64_t tot = 0;
        for (int64_t i : batch) {
            tot += masm_list[i].size();
        }
        dt::Mounts mounts;
        mounts.append({py::str("c"), tc});
        auto [ok, out, work] = dt::run(mounts, bat, arts, static_cast<double>(std::min<int64_t>(600, 90 + tot / 400)), files);
        for (int64_t j = 0; j < batch.size(); ++j) {
            const py::bytes log = out.get(py::str(py::percent(py::str("m%d.txt"), j)), py::bytes());
            results[batch[j]] = Asm{out.get(py::str(py::percent(py::str("m%d.obj"), j))),
                                    !log.empty() ? log.decode("latin1", "replace") : py::str("")};
        }
        if (work) {
            py::shutil::rmtree(*work, true);
        }
    }
    return results;
}

std::tuple<py::bytes, py::dict<py::str, int64_t>> coff_text_syms(const py::bytes& obj) {
    const int64_t nsec = py::unpack_from1<int64_t>("<H", obj, 2);
    const int64_t symptr = py::unpack_from1<int64_t>("<I", obj, 8);
    const int64_t nsym = py::unpack_from1<int64_t>("<I", obj, 12);
    const int64_t strtab = symptr + nsym * 18;
    // (section number, raw size, raw pointer, flags)
    py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> secs;
    for (int64_t i = 0; i < nsec; ++i) {
        const int64_t h = 20 + i * 40;
        const int64_t rawsize = py::unpack_from1<int64_t>("<I", obj, h + 16);
        const int64_t rawptr = py::unpack_from1<int64_t>("<I", obj, h + 20);
        const int64_t flags = py::unpack_from1<int64_t>("<I", obj, h + 36);
        secs.append({i + 1, rawsize, rawptr, flags});
    }
    py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> code;
    for (const auto& s : secs) {
        if ((std::get<3>(s) & 0x20) && std::get<1>(s) > 0) {
            code.append(s);
        }
    }
    if (code.empty()) {
        // sorted(secs, key=size, reverse=True)[:1]
        code = py::sorted(secs, [](const auto& s) { return std::get<1>(s); }, true).slice(0, 1);
    }
    if (code.empty()) {
        throw py::IndexError("list index out of range");
    }
    const auto [secnum, rawsize, rawptr, unused] = code[0];
    (void)unused;
    const py::bytes text = obj.slice(rawptr, rawptr + rawsize);
    py::dict<py::str, int64_t> syms;
    int64_t i = 0;
    while (i < nsym) {
        const int64_t off = symptr + i * 18;
        const py::bytes name8 = obj.slice(off, off + 8);
        py::str name;
        if (name8.slice(0, 4) == py::bytes(std::string("\0\0\0\0", 4))) {
            const int64_t so = py::unpack_from1<int64_t>("<I", name8, 4);
            const int64_t end = obj.index(py::bytes(std::string("\0", 1)), strtab + so);
            name = obj.slice(strtab + so, end).decode("latin1", "replace");
        } else {
            name = name8.rstrip(py::bytes(std::string("\0", 1))).decode("latin1", "replace");
        }
        const auto [value, sec, typ, sclass, naux] = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t>("<IhHBB", obj, off + 8);
        (void)typ;
        (void)sclass;
        if (sec == secnum) {
            syms.set(name, value);
        }
        i += 1 + naux;
    }
    return {text, syms};
}

const py::str _ERRLINE = py::str("m\\d*\\.asm[:(](\\d+)\\)?:?\\s*(?:error|fatal)");   // ML m.asm(N): / m<N>.asm / nasm m.asm:N:

ConvergeResult _converge_range(const VxDDisasm& D, int64_t oi, const py::str& root, int64_t lo, int64_t hi, const py::bytes& oracle,
                               int64_t max_repair,
                               const std::optional<std::tuple<std::optional<py::bytes>, py::str, LineMap>>& first) {
    static const py::re::Pattern errline = py::re::compile(_ERRLINE, 2);   // re.I
    py::list<int64_t> offs;
    for (const dis::Place& k : D.insns) {
        if (std::get<0>(k) == oi && lo <= std::get<1>(k) && std::get<1>(k) < hi) {
            offs.append(std::get<1>(k));
        }
    }
    py::set<int64_t> inrange = py::set<int64_t>::from(offs);
    py::set<int64_t> force = cross_range_force(D, oi, lo, hi);   // branches out of the range -> byte-preserve
    for (int64_t it = 0; it < max_repair; ++it) {
        std::optional<py::bytes> obj;
        py::str log;
        LineMap linemap;
        if (it == 0 && first) {
            obj = std::get<0>(*first);                           // batched first pass (assembled with `force`)
            log = std::get<1>(*first);
            linemap = std::get<2>(*first);
        } else {
            auto [masm, lm] = emit_object_masm(D, oi, py::str("_LTEXT"), force, true, std::make_tuple(lo, hi));
            linemap = lm;
            auto r = assemble_coff(masm, root);
            obj = std::get<0>(r);
            log = std::get<1>(r);
        }
        if (!obj || obj->empty()) {                              // (`if not obj`)
            py::set<int64_t> bad;
            for (const auto& m : errline.finditer(log)) {
                const int64_t line = py::int_parse(m.group(1));
                if (linemap.contains(line)) {
                    bad.add(linemap.at(line));
                }
            }
            if (bad.empty() || bad.issubset(force)) {
                break;                                           // -> range raw fallback
            }
            force = force | bad;
            continue;
        }
        auto [text, syms] = coff_text_syms(*obj);
        py::set<int64_t> newbad;
        for (int64_t a : offs) {
            if (force.contains(a)) {
                continue;
            }
            const auto ao = syms.get(py::percent(py::str("Q%x"), a));
            if (!ao) {
                continue;
            }
            const int64_t olen = D.insns.at(dis::Place(oi, a)).size;
            if (text.slice(*ao, *ao + olen) != oracle.slice(a, a + olen)) {
                newbad.add(a);
            }
        }
        const bool exact = text.size() >= (hi - lo) && text.slice(0, hi - lo) == oracle.slice(lo, hi);
        if (exact || newbad.empty()) {
            const py::set<int64_t> fr = force & inrange;
            ConvergeResult r;
            r.sym = inrange.size() - fr.size();
            r.raw = fr.size();
            r.match = exact;
            r.force = force;
            r.text = text.slice(0, hi - lo);
            return r;
        }
        force = force | newbad;
    }
    force = force | inrange;                                     // range fallback: byte-preserve all of it
    auto [masm, unused] = emit_object_masm(D, oi, py::str("_LTEXT"), force, false, std::make_tuple(lo, hi));
    (void)unused;
    auto [obj, log] = assemble_coff(masm, root);
    py::bytes text;
    if (obj && !obj->empty()) {
        text = std::get<0>(coff_text_syms(*obj));
    }
    const bool exact = obj.has_value() && text.size() >= (hi - lo) && text.slice(0, hi - lo) == oracle.slice(lo, hi);
    ConvergeResult r;
    r.sym = 0;
    r.raw = inrange.size();
    r.match = exact;
    r.force = force;
    r.text = (obj && !obj->empty()) ? text.slice(0, hi - lo) : py::bytes();
    return r;
}

CompareObject compare_object(const le::LEFile& v, int64_t oi, const py::str& root_in, int64_t max_repair, int64_t max_ins) {
    std::shared_ptr<VxDDisasm> Dp = std::make_shared<VxDDisasm>(v);
    Dp->run();
    const VxDDisasm& D = *Dp;
    const py::str root = py::os::path::abspath(root_in);
    const py::bytes oracle = D.code.at(oi);
    py::list<int64_t> offs;
    for (const dis::Place& k : D.insns) {
        if (std::get<0>(k) == oi) {
            offs.append(std::get<1>(k));
        }
    }
    py::list<std::tuple<int64_t, int64_t>> ranges;
    if (offs.size() <= max_ins) {
        ranges.append({0, oracle.size()});
    } else {
        ranges = split_ranges(D, oi, max_ins);
    }
    int64_t sym = 0, raw = 0;
    bool match = true;
    py::set<int64_t> fr_all;
    py::bytes asm_bytes;
    for (const auto& [lo, hi] : ranges) {
        ConvergeResult r = _converge_range(D, oi, root, lo, hi, oracle, max_repair);
        sym += r.sym;
        raw += r.raw;
        match = match && r.match;
        fr_all = fr_all | r.force;
        asm_bytes = asm_bytes + r.text;
    }
    CompareObject out;
    out.ok = true;
    out.match = match;
    out.oracle_len = oracle.size();
    out.asm_len = asm_bytes.size();
    out.n_diffs = match ? 0 : 1;
    out.symbolic_insns = sym;
    out.forced_raw = raw;
    out.force_raw_set = fr_all;
    out.raw_fallback = sym == 0 && (sym + raw) > 0;              // code, all forced
    out.n_files = ranges.size();
    out.asm_bytes = asm_bytes.slice(0, oracle.size());
    out.oracle_bytes = oracle;
    out.D = Dp;
    return out;
}

ModuleReport compare_module(const le::LEFile& v, const py::str& root_in, int64_t max_repair, int64_t max_ins) {
    VxDDisasm D(v);
    D.run();
    const py::str root = py::os::path::abspath(root_in);
    struct Range {
        int64_t oi, lo, hi;
        py::bytes oracle;
    };
    std::vector<Range> ranges;                                   // (oi, lo, hi, oracle)
    py::list<py::str> masms;                                     // each range's first-pass (probe) MASM
    std::vector<LineMap> linemaps;
    for (const le::Object& o : v.objects) {
        if (!o.exec) {
            continue;
        }
        const int64_t oi = o.idx;
        const py::bytes oracle = D.code.at(oi);
        int64_t noffs = 0;
        for (const dis::Place& k : D.insns) {
            if (std::get<0>(k) == oi) {
                noffs += 1;
            }
        }
        py::list<std::tuple<int64_t, int64_t>> rr;
        if (noffs <= max_ins) {
            rr.append({0, oracle.size()});
        } else {
            rr = split_ranges(D, oi, max_ins);
        }
        for (const auto& [lo, hi] : rr) {
            const py::set<int64_t> force0 = cross_range_force(D, oi, lo, hi);
            auto [masm, linemap] = emit_object_masm(D, oi, py::str("_LTEXT"), force0, true, std::make_tuple(lo, hi));
            ranges.push_back({oi, lo, hi, oracle});
            linemaps.push_back(linemap);
            masms.append(masm);
        }
    }
    const py::list<Asm> firsts = assemble_coff_batch(masms, root);   // batched first pass
    std::vector<std::pair<int64_t, ConvergeResult>> per;             // (oi, result) in range order
    for (size_t k = 0; k < ranges.size(); ++k) {
        const Range& r = ranges[k];
        const auto& [obj, log] = firsts[static_cast<int64_t>(k)];
        per.push_back({r.oi, _converge_range(D, r.oi, root, r.lo, r.hi, r.oracle, max_repair,
                                             std::make_tuple(obj, log, linemaps[k]))});
    }
    ModuleReport rep;
    for (const le::Object& o : v.objects) {
        if (!o.exec) {
            continue;
        }
        const int64_t oi = o.idx;
        const py::bytes oracle = D.code.at(oi);
        int64_t sym = 0, raw = 0, nparts = 0;
        bool match = true;
        py::bytes asm_bytes;
        py::set<int64_t> fr_all;
        for (const auto& [poi, p] : per) {
            if (poi != oi) {
                continue;
            }
            nparts += 1;
            sym += p.sym;
            raw += p.raw;
            match = match && p.match;
            asm_bytes = asm_bytes + p.text;
            fr_all = fr_all | p.force;
        }
        ObjectReport r;
        r.ok = true;
        r.obj = oi;
        r.match = match;                                         // (all() of no parts is True)
        r.oracle_len = oracle.size();
        r.asm_len = asm_bytes.size();
        r.n_diffs = match ? 0 : 1;
        r.symbolic_insns = sym;
        r.forced_raw = raw;
        r.force_raw_set = fr_all;
        r.n_files = nparts;
        r.raw_fallback = sym == 0 && (sym + raw) > 0;
        r.asm_bytes = asm_bytes.slice(0, oracle.size());
        r.oracle_bytes = oracle;
        rep.objects.append(r);
    }
    bool allok = true;
    for (const ObjectReport& r : rep.objects) {
        allok = allok && r.match;
        rep.symbolic += r.symbolic_insns;
        rep.forced_raw += r.forced_raw;
        rep.raw_fallback_objs += r.raw_fallback ? 1 : 0;
    }
    rep.byte_exact = allok;
    return rep;
}

}  // namespace codebreaker::source::vxd::masm
