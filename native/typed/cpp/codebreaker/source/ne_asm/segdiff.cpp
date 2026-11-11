// codebreaker/source/ne_asm/segdiff.cpp -- typed C++ port of codebreaker.source.ne_asm.segdiff (see segdiff.hpp).

#include "codebreaker/source/ne_asm/segdiff.hpp"

#include <algorithm>
#include <cstdio>
#include <ostream>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/msvc152.hpp"
#include "codebreaker/compare/mask.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/omf.hpp"
#include "py_os.hpp"

namespace codebreaker::source::ne_asm::segdiff {

namespace cc16 = codebreaker::build::msvc152;
namespace masks = codebreaker::compare::mask;
namespace omf = codebreaker::formats::omf;
using py::str;

const py::dict<int64_t, int64_t> NE_SRC_WIDTH = [] {
    py::dict<int64_t, int64_t> d;
    d.set(0, 1);
    d.set(2, 2);
    d.set(3, 4);
    d.set(5, 2);
    d.set(11, 6);
    return d;
}();

str Corpus::resolve(const str& module) {
    // corpus.resolve(module) or corpus.retail(module) or module  (resolve raises ValueError for a module the corpus does not have)
    const str found = base::corpus::resolve(module);
    if (!found.empty()) return found;
    const std::optional<str> retail = base::corpus::retail(module);
    if (retail && !retail->empty()) return *retail;
    return module;
}

Corpus _target() { return Corpus(); }

// ---------------------------------------------------------------------------
// our side: compile a group, get each function's bytes + fixup mask
// ---------------------------------------------------------------------------
py::dict<str, std::tuple<py::bytes, py::set<int64_t>>> compile_group(const str& csrc, const str& flags) {
    const cc16::Compiled r = cc16::compile_c(csrc, flags);
    if (!r.obj) throw py::RuntimeError(("compile produced no OBJ:\n" + (r.log ? r.log->slice(0, 600) : str(""))).utf8());
    return omf::function_bytes(*r.obj);
}

// ---------------------------------------------------------------------------
// shipped side: pull a segment's bytes + a relocation mask from an NE module
// ---------------------------------------------------------------------------
std::tuple<py::bytes, py::set<int64_t>> shipped_ne_segment(const str& module_path, int64_t seg_index) {
    const formats::ne::NEFile ne(module_path);
    const formats::ne::Segment seg = ne.segments()[seg_index - 1];
    const py::bytes code_b = ne.data.slice(seg.file_off, seg.file_off + seg.length);
    const std::string code = code_b.raw();
    const int64_t len = static_cast<int64_t>(code.size());
    py::set<int64_t> mask;
    // The masked set MUST match linkfile's NE relocation restore pass exactly: the
    // rebuild leaves shipped bytes only at the sites linkfile restores, so any byte
    // masked here but NOT restored there is a false "already-matching" that lets a
    // mis-disassembled table entry stay live and then diff in the real rebuild.
    // linkfile requires the WHOLE fixup site (w bytes) to fit in the segment before
    // visiting it (`cur + size <= length`); the old bound here (`cur < len(code)-1`)
    // only required the 2-byte next-link to fit, so a record whose site overran the
    // segment end was still followed, over-masking a whole chain of interior sites
    // linkfile never touches. Use linkfile's bound so the two agree.
    for (const formats::ne::Relocation& rec : ne.relocations(seg)) {
        const int64_t w = NE_SRC_WIDTH.get(rec.src, 2);
        const int64_t off = rec.offset;
        if (rec.additive) {
            if (off + w <= len)
                for (int64_t k = 0; k < w; k++) mask.add(off + k);
            continue;
        }
        py::set<int64_t> seen;                              // non-additive: follow the fixup chain
        int64_t cur = off;
        while (cur != 0xFFFF && 0 <= cur && cur + w <= len && !seen.contains(cur)) {
            seen.add(cur);
            for (int64_t k = 0; k < w; k++) mask.add(cur + k);
            if (cur + 1 >= len) break;                      // can't read the 2-byte next-link
            cur = static_cast<uint8_t>(code[static_cast<size_t>(cur)]) | (static_cast<uint8_t>(code[static_cast<size_t>(cur + 1)]) << 8);
        }
    }
    return {code_b, mask};
}

// ---------------------------------------------------------------------------
// the differ
// ---------------------------------------------------------------------------
DiffResult diff(const py::bytes& our, const py::set<int64_t>& omask, const py::bytes& shipped, const py::set<int64_t>& smask) {
    py::set<int64_t> skip;
    for (const int64_t k : omask) skip.add(k);
    for (const int64_t k : smask) skip.add(k);
    const int64_t n = std::min(our.size(), shipped.size());
    py::list<masks::MaybeMask> ms;
    ms.append(masks::MaybeMask(masks::Mask(py::bytes(masks::of(skip, n).raw()))));
    const py::list<int64_t> found = masks::differing(our.slice(0, n), shipped.slice(0, n), ms);
    DiffResult r;
    const int64_t ndiff = found.size();
    r.exact = our.size() == shipped.size() && ndiff == 0;
    r.length_match = our.size() == shipped.size();
    r.our_len = our.size();
    r.shipped_len = shipped.size();
    r.n_diff = ndiff;
    if (ndiff) r.first_diff = found[0];
    r.masked = skip.size();
    return r;
}

py::str render(const DiffResult& res, const std::optional<py::bytes>& our, const std::optional<py::bytes>& shipped, const py::set<int64_t>& omask,
               const py::set<int64_t>& smask) {
    const str v = res.exact ? str("BYTE-EXACT") : (res.length_match ? str("length ok, content differs") : str("length differs"));
    py::list<str> out;
    out.append(str("verdict: ") + v);
    char buf[128];
    std::snprintf(buf, sizeof buf, "  our=%lld shipped=%lld bytes, masked=%lld, diffs=%lld", static_cast<long long>(res.our_len),
                  static_cast<long long>(res.shipped_len), static_cast<long long>(res.masked), static_cast<long long>(res.n_diff));
    str line(buf);
    if (res.first_diff) {
        std::snprintf(buf, sizeof buf, ", first@0x%llx", static_cast<long long>(*res.first_diff));
        line = line + str(buf);
    }
    out.append(line);
    if (!res.exact && our && res.first_diff) {
        const int64_t i = *res.first_diff;
        const int64_t lo = std::max<int64_t>(0, i - 4);
        const int64_t hi = std::min({our->size(), shipped ? shipped->size() : int64_t(0), i + 8});
        if (!shipped) throw py::TypeError("object of type 'NoneType' has no len()");
        py::set<int64_t> skip;
        for (const int64_t k : omask) skip.add(k);
        for (const int64_t k : smask) skip.add(k);
        auto hex2 = [](int64_t b) {
            char h[8];
            std::snprintf(h, sizeof h, "%02x", static_cast<unsigned>(b));
            return str(h);
        };
        py::list<str> o, s;
        for (int64_t k = lo; k < hi; k++) {
            o.append(skip.contains(k) ? str("--") : hex2((*our)[k]));
            s.append(skip.contains(k) ? str("--") : hex2((*shipped)[k]));
        }
        std::snprintf(buf, sizeof buf, "  @0x%llx ours    : ", static_cast<long long>(lo));
        out.append(str(buf) + py::join(str(" "), o));
        std::snprintf(buf, sizeof buf, "  @0x%llx shipped : ", static_cast<long long>(lo));
        out.append(str(buf) + py::join(str(" "), s));
    }
    return py::join(str("\n"), out);
}

// ---------------------------------------------------------------------------
// selftest: prove the mechanism without needing a retail anchor
// ---------------------------------------------------------------------------
const str GROUP("\nint helper(int x)        { return x + 1; }\nint caller(int y)        { return helper(y) * 2 + helper(y + 1); }\n");

namespace {

str first_line(const str& s) { return py::splitlines(s)[0]; }

}  // namespace

bool _selftest(std::ostream& out) {
    out << "segdiff selftest\n\n";
    bool ok = true;

    // (a) round-trip: a group matches itself exactly
    const auto g = compile_group(GROUP);
    const py::bytes cb = std::get<0>(g.at(str("_caller")));
    const py::set<int64_t> cm = std::get<1>(g.at(str("_caller")));
    const DiffResult res = diff(cb, cm, cb, cm);
    out << "(a) round-trip caller vs itself: " << first_line(render(res)).utf8() << "\n";
    ok = ok && res.exact;

    // (b) mutation is detected and located
    str mutated = GROUP;
    {
        const str from("* 2 +"), to("* 3 +");
        mutated = GROUP.replace(from, to);
    }
    const auto g2 = compile_group(mutated);
    const py::bytes cb2 = std::get<0>(g2.at(str("_caller")));
    const py::set<int64_t> cm2 = std::get<1>(g2.at(str("_caller")));
    const DiffResult res2 = diff(cb2, cm2, cb, cm);
    out << "(b) mutated caller vs original: " << first_line(render(res2)).utf8() << " " << (!res2.exact ? "" : "(BUG: should differ)") << "\n";
    ok = ok && !res2.exact;

    // (c) whole-compiland resolution: compiling the callee IN the group makes
    //     caller's call to it CONCRETE (fewer masked bytes) than when the callee
    //     is external. This is the reason isolation fails.
    const int64_t grouped_masked = cm.size();
    const auto alone = compile_group(str("extern int helper(int);\nint caller(int y){return helper(y)*2+helper(y+1);}\n"));
    const py::set<int64_t> am = std::get<1>(alone.at(str("_caller")));
    out << "(c) caller masked bytes: grouped=" << grouped_masked << ", callee-external=" << am.size() << "  -> "
        << (grouped_masked < am.size() ? "grouping resolves the calls" : "??") << "\n";
    ok = ok && grouped_masked < am.size();

    out << "\nSELFTEST: " << (ok ? "PASS" : "FAIL") << "\n";
    return ok;
}

namespace {

// argparse int(x) / type=lambda x: int(x, 0): nullopt when it does not parse
std::optional<int64_t> parse_int(const str& s, int64_t base) {
    try {
        return py::int_parse(s, base);
    } catch (const py::ValueError&) {
        return std::nullopt;
    }
}

}  // namespace

int64_t main(const py::list<str>& argv, std::ostream& out, std::ostream& err) {
    std::optional<str> csrc, module, off, func;
    std::optional<int64_t> seg, size;
    str flags("/AM /Ox /c");
    bool selftest = false;
    auto fail = [&err](const std::string& msg) {
        err << "usage: segdiff [-h] [--module MODULE] [--seg SEG] [--off OFF] [--size SIZE] [--func FUNC] [--flags FLAGS] [--selftest] [csrc]\n"
            << "segdiff: error: " << msg << "\n";
        return int64_t(2);
    };
    for (int64_t i = 0; i < argv.size(); i++) {
        const str a = argv[i];
        auto value = [&](const char* name) -> std::optional<str> {
            if (i + 1 >= argv.size()) return std::nullopt;
            (void)name;
            return argv[++i];
        };
        if (a == str("--selftest")) {
            selftest = true;
        } else if (a == str("--module") || a == str("--off") || a == str("--func") || a == str("--flags")) {
            const auto v = value(a.utf8().c_str());
            if (!v) return fail("argument " + a.utf8() + ": expected one argument");
            if (a == str("--module")) module = *v;
            else if (a == str("--off")) off = *v;
            else if (a == str("--func")) func = *v;
            else flags = *v;
        } else if (a == str("--seg") || a == str("--size")) {
            const auto v = value(a.utf8().c_str());
            if (!v) return fail("argument " + a.utf8() + ": expected one argument");
            const auto n = parse_int(*v, a == str("--seg") ? 10 : 0);
            if (!n) return fail("argument " + a.utf8() + ": invalid value: " + py::repr(*v).utf8());
            if (a == str("--seg")) seg = n;
            else size = n;
        } else if (a.startswith(str("--")) && a.size() > 2) {
            return fail("unrecognized arguments: " + a.utf8());
        } else {
            if (csrc) return fail("unrecognized arguments: " + a.utf8());
            csrc = a;
        }
    }

    if (selftest) return _selftest(out) ? 0 : 1;

    if (!(csrc && !csrc->empty() && module && !module->empty() && seg && off && !off->empty() && func && !func->empty()))
        return fail("need csrc + --module --seg --off --func (or --selftest)");
    const Corpus target = _target();
    const str path = target.resolve(*module);
    const int64_t o = py::int_parse(*off, 0);
    py::file f = py::file::open(*csrc, str("rb"));
    const str text = f.read().decode("utf-8");      // (open(..).read(): text)
    f.close();
    const auto g = compile_group(text, flags);
    if (!g.contains(*func)) {
        py::list<str> have;
        for (const auto& kv : g.items()) have.append(std::get<0>(kv));
        err << ("function " + func->utf8() + " not in the compiled group; have: " + py::join(str(", "), have).utf8()) << "\n";
        return 1;
    }
    const py::bytes our = std::get<0>(g.at(*func));
    const py::set<int64_t> omask = std::get<1>(g.at(*func));
    const auto [seg_bytes, seg_mask] = shipped_ne_segment(path, *seg);
    const int64_t sz = (size && *size != 0) ? *size : our.size();
    const py::bytes shipped = seg_bytes.slice(o, o + sz);
    py::set<int64_t> smask;
    for (const int64_t m : seg_mask)
        if (o <= m && m < o + sz) smask.add(m - o);
    const DiffResult res = diff(our, omask, shipped, smask);
    out << render(res, our, shipped, omask, smask).utf8() << "\n";
    return res.exact ? 0 : 1;
}

}  // namespace codebreaker::source::ne_asm::segdiff
