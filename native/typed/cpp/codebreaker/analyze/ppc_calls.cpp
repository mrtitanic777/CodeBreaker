// codebreaker/analyze/ppc_calls.cpp -- typed C++ port of codebreaker.analyze.ppc_calls (see ppc_calls.hpp).

#include "codebreaker/analyze/ppc_calls.hpp"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

#include "codebreaker/formats/xex.hpp"
#include "codebreaker/formats/xex_imports.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::ppc_calls {

namespace powerpc_image = codebreaker::formats::powerpc_image;

const py::list<int64_t> ARG_REGS = {3, 4, 5, 6, 7, 8, 9, 10};
const py::list<int64_t> VOLATILE = {0, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int64_t VOLATILE_MASK = (1 << 0) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12);
const int64_t FP_VOLATILE_MASK = (1 << 14) - 1;   // f0-f13

namespace {

// the module's docstring (what main() prints without arguments)
const char* const DOC =
    "ppc_calls.py -- what a PowerPC program's calls show about its functions, from the binary alone.\n"
    "\n"
    "    python -m codebreaker ppc-calls <module.xex or image> [-o calls.tsv]\n"
    "    from codebreaker.analyze import ppc_calls\n"
    "    r = ppc_calls.analyse(path)\n"
    "    r.arity        {address: argument registers r3.. its callers set up}   (internal functions)\n"
    "    r.imports      {name: argument registers}                            (imported functions)\n"
    "    r.clobber      {address: mask of r0, r4-r12 it may change}           (only those that keep some)\n"
    "    r.fpclobber    {address: mask of f0-f13 it may change}               (only those that keep some)\n"
    "    r.variadic     {address: fixed arguments before the variable ones}\n"
    "\n"
    "Each is evidence, never a guess:\n"
    "\n"
    "  * Arity: before a `bl`, the argument registers r3, r4, ... set within the 14 instructions back\n"
    "    (stopping at an earlier call), counted while they run on without a gap. A function's arity is\n"
    "    the count most of its call sites agree on: at least 3 sites, 60 % agreeing (85 % for 8, which\n"
    "    a longer argument list also gives), and 1-7 (0 is a pass-through as often as no arguments). An\n"
    "    import is a stub with no body to pass through, so 0 and 8 count there; it needs 3 sites and\n"
    "    85 %, and the printf family, whose count varies by call, is left out.\n"
    "  * Clobber: the volatile registers a function may change -- those it writes, and those every\n"
    "    function it calls may change, to a fixed point over the call graph. A call to an import, through\n"
    "    a pointer or out of the known functions may change them all. The compiler's register save and\n"
    "    restore runs (__savegprlr_N ...) change nothing the caller sees, and neither does a register\n"
    "    reloaded from where the function stored it on its frame. What is reported is what a function\n"
    "    provably keeps: a caller may read those registers across the call.\n"
    "  * Variadic: a function that stores r8, r9 and r10 to its caller's frame before its own `stwu`\n"
    "    is homing the va_list area; its fixed arguments are the registers below the lowest one homed.\n"
    "\n"
    "These passes come from the clean-room xam decompile (../xam, tools/infer_*.py), where they turned\n"
    "thousands of refused functions into faithful C; here they run on CodeBreaker's own listing\n"
    "(recompile/disassemble.py) and function discovery (recompile/discover.py), for any module.\n";

using Pyset = py::set<py::str>;

// instructions whose first operand is a register they set (for the call-site count)
const Pyset& SETS() {
    static const Pyset s = {
        py::str("li"), py::str("lis"), py::str("mr"), py::str("addi"), py::str("addis"), py::str("subi"), py::str("ori"),
        py::str("oris"), py::str("lwz"), py::str("lhz"), py::str("lbz"), py::str("ld"), py::str("add"), py::str("subf"),
        py::str("or"), py::str("and"), py::str("rlwinm"), py::str("mulli"), py::str("extsb"), py::str("extsh"), py::str("neg"),
        py::str("lwzx"), py::str("divwu"), py::str("mullw"), py::str("srawi"), py::str("xor"), py::str("slw"), py::str("srw"),
        py::str("addic"), py::str("rlwimi"), py::str("clrlwi"), py::str("slwi"), py::str("srwi"), py::str("rotlwi"),
        py::str("lha"), py::str("lwa"), py::str("extsw"), py::str("nor"), py::str("not")};
    return s;
}
const char* const NO_WRITE_PREFIX[] = {"cmp", "b", "st", "dcb", "tw", "td", "tlb", "mt"};
const Pyset& NO_WRITE() {
    static const Pyset s = {py::str("nop"), py::str("sync"), py::str("isync"), py::str("eieio"), py::str("lwsync"), py::str("twi"),
                            py::str("tdi"), py::str("ptesync"), py::str("tlbsync"), py::str("dcbz128")};
    return s;
}
const Pyset& UPDATE_STORE() {
    static const Pyset s = {py::str("stwu"), py::str("sthu"), py::str("stbu"), py::str("stdu"), py::str("stwux"), py::str("sthux"),
                            py::str("stbux"), py::str("stdux"), py::str("stfsu"), py::str("stfdu"), py::str("stfsux"),
                            py::str("stfdux"), py::str("lfsu"), py::str("lfdu"), py::str("lfsux"), py::str("lfdux")};
    return s;
}
const Pyset& UPDATE_LOAD() {
    static const Pyset s = {py::str("lwzu"), py::str("lhzu"), py::str("lbzu"), py::str("ldu"), py::str("lhau"), py::str("lwau"),
                            py::str("lwzux"), py::str("lhzux"), py::str("lbzux"), py::str("ldux"), py::str("lhaux")};
    return s;
}
const Pyset& INDIRECT() {
    static const Pyset s = {py::str("bctrl"), py::str("bctr"), py::str("bcctr"), py::str("bcctrl"), py::str("bclrl")};
    return s;
}

// _VARIADIC_IMPORTS.search(name): the pattern is a list of plain words
bool variadic_import(const py::str& name) {
    static const char* const words[] = {"printf", "scanf", "DbgPrint", "ProducerLog", "vscw", "vsnw", "_vsn", "_vsc"};
    for (const char* w : words)
        if (name.contains(py::str(w))) return true;
    return false;
}

bool ascii_only(const std::u32string& t) {
    for (char32_t c : t)
        if (c >= 0x80) return false;
    return true;
}

bool digit(char32_t c) { return c >= '0' && c <= '9'; }
bool hexdigit(char32_t c) { return digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

// `$`: the end of the text, or just before a final newline
bool at_dollar(const std::u32string& t, size_t i) { return i == t.size() || (i + 1 == t.size() && t[i] == U'\n'); }

int64_t to_int(const std::u32string& t, size_t a, size_t b) {
    if (b - a <= 18) {
        int64_t v = 0;
        for (size_t i = a; i < b; ++i) v = v * 10 + static_cast<int64_t>(t[i] - '0');
        return v;
    }
    return py::int_parse(py::str(t.substr(a, b - a)));
}

// _REG / _FREG .match(tok): the register number, or -1 (a register is never negative)
int64_t reg_of(const py::str& tok, bool fp) {
    const std::u32string& t = tok.u32();
    if (ascii_only(t)) {
        if (t.empty() || t[0] != (fp ? U'f' : U'r')) return -1;
        size_t i = 1;
        if (fp && i < t.size() && t[i] == U'r') ++i;
        const size_t d0 = i;
        while (i < t.size() && digit(t[i])) ++i;
        if (i == d0 || !at_dollar(t, i)) return -1;
        return to_int(t, d0, i);
    }
    // (a non-ASCII token: \d is any Unicode digit)
    static const py::re::Pattern RE_REG("^r(\\d+)$", 0);
    static const py::re::Pattern RE_FREG("^f(?:r)?(\\d+)$", 0);
    const auto m = (fp ? RE_FREG : RE_REG).match(tok);
    return m ? py::int_parse(m->group(1)) : -1;
}

// _MEM.match(tok): group 1 (the displacement's text), group 2 ("r<n>") and its number
struct Mem {
    py::str disp;
    py::str base;
    int64_t reg = -1;
    bool base_is_r1 = false;
};

std::optional<Mem> mem_of(const py::str& tok) {
    const std::u32string& t = tok.u32();
    if (ascii_only(t)) {
        size_t i = 0;
        if (i < t.size() && t[i] == U'-') ++i;
        // -?(?:0x)?[0-9a-fA-F]+  then \(
        size_t end = 0;
        bool ok = false;
        if (i + 1 < t.size() && t[i] == U'0' && t[i + 1] == U'x') {
            size_t k = i + 2;
            while (k < t.size() && hexdigit(t[k])) ++k;
            if (k > i + 2 && k < t.size() && t[k] == U'(') {
                end = k;
                ok = true;
            }
        }
        if (!ok) {
            size_t k = i;
            while (k < t.size() && hexdigit(t[k])) ++k;
            if (k > i && k < t.size() && t[k] == U'(') {
                end = k;
                ok = true;
            }
        }
        if (!ok) return std::nullopt;
        // (r\d+)\)$
        size_t j = end + 1;
        if (j >= t.size() || t[j] != U'r') return std::nullopt;
        const size_t d0 = j + 1;
        size_t k = d0;
        while (k < t.size() && digit(t[k])) ++k;
        if (k == d0 || k >= t.size() || t[k] != U')' || !at_dollar(t, k + 1)) return std::nullopt;
        Mem m;
        m.disp = py::str(t.substr(0, end));
        m.base = py::str(t.substr(j, k - j));
        m.reg = to_int(t, d0, k);
        m.base_is_r1 = (k - d0 == 1 && t[d0] == U'1');
        return m;
    }
    static const py::re::Pattern RE_MEM("^(-?(?:0x)?[0-9a-fA-F]+)\\((r\\d+)\\)$", 0);
    const auto m = RE_MEM.match(tok);
    if (!m) return std::nullopt;
    Mem out;
    out.disp = m->group(1);
    out.base = m->group(2);
    out.reg = py::int_parse(out.base.slice_from(1));
    out.base_is_r1 = out.base == py::str("r1");
    return out;
}

// the registers an instruction writes (an over-approximation), appended to `out`
void gpr_writes_into(const py::str& mnem, const py::list<py::str>& ops, std::vector<int64_t>& out) {
    const py::str b = mnem.rstrip(py::str("."));
    if (ops.empty()) return;
    if (b == py::str("lmw")) {
        const int64_t r = reg_of(ops[0], false);
        if (r >= 0)
            for (int64_t k = r; k < 32; ++k) out.push_back(k);
        return;
    }
    int64_t base = -1;
    if (ops.size() >= 2) {
        const auto m = mem_of(ops[1]);
        base = m ? m->reg : reg_of(ops[1], false);
    }
    if (UPDATE_STORE().contains(b)) {
        if (base >= 0) out.push_back(base);
        return;
    }
    if (UPDATE_LOAD().contains(b)) {
        const int64_t r = reg_of(ops[0], false);
        if (r >= 0) out.push_back(r);
        if (base >= 0) out.push_back(base);
        return;
    }
    for (const char* p : NO_WRITE_PREFIX)
        if (b.startswith(py::str(p))) return;
    if (NO_WRITE().contains(b)) return;
    const int64_t r = reg_of(ops[0], false);
    if (r >= 0) out.push_back(r);
}

void fpr_writes_into(const py::str& mnem, const py::list<py::str>& ops, std::vector<int64_t>& out) {
    const py::str b = mnem.rstrip(py::str("."));
    if (ops.empty()) return;
    for (const char* p : {"stf", "fcmp", "mtfsf", "b"})
        if (b.startswith(py::str(p))) return;
    const int64_t r = reg_of(ops[0], true);
    if (r >= 0) out.push_back(r);
}

}  // namespace

py::list<py::str> _ops(const py::str& text) {
    py::list<py::str> out;
    if (text.strip().empty()) return out;
    const std::u32string& t = text.u32();
    size_t a = 0;
    for (;;) {
        const size_t b = t.find(U',', a);
        const py::str piece(t.substr(a, b == std::u32string::npos ? std::u32string::npos : b - a));
        out.append(piece.strip());
        if (b == std::u32string::npos) break;
        a = b + 1;
    }
    return out;
}

std::optional<int64_t> _reg(const std::optional<py::str>& tok, bool fp) {
    const int64_t r = reg_of(tok ? *tok : py::str(), fp);
    if (r < 0) return std::nullopt;
    return r;
}

std::optional<int64_t> _target(const py::list<py::str>& ops) {
    if (!ops.empty() && ops[0].lower().startswith(py::str("0x"))) return py::int_parse(ops[0], 16);
    return std::nullopt;
}

py::set<int64_t> gpr_writes(const py::str& mnem, const py::list<py::str>& ops) {
    std::vector<int64_t> v;
    gpr_writes_into(mnem, ops, v);
    py::set<int64_t> out;
    for (int64_t r : v) out.add(r);
    return out;
}

py::set<int64_t> fpr_writes(const py::str& mnem, const py::list<py::str>& ops) {
    std::vector<int64_t> v;
    fpr_writes_into(mnem, ops, v);
    py::set<int64_t> out;
    for (int64_t r : v) out.add(r);
    return out;
}

std::tuple<py::dict<int64_t, Body>, py::ref<powerpc_image::Image>> _functions(const py::str& path) {
    auto [funcs, sources, img] = recompile::discover::discover(path);
    (void)sources;
    const int64_t lo = std::get<0>(img->code), hi = std::get<1>(img->code);
    const py::bytes code = img->bytes_at(lo, hi - lo);
    std::vector<int64_t> starts;
    for (const auto& kv : funcs.items()) {
        const int64_t a = std::get<0>(kv);
        if (!(lo <= a && a < hi) || img->imports.contains(a)) continue;
        if (!img->run_names) throw py::AttributeError("'Image' object has no attribute 'run_names'");
        if (img->run_names->contains(a)) continue;
        starts.push_back(a);
    }
    std::sort(starts.begin(), starts.end());
    py::dict<int64_t, Body> out;
    for (size_t i = 0; i < starts.size(); ++i) {
        const int64_t a = starts[i];
        const int64_t size = funcs.at(a);
        int64_t end = size ? a + size : (i + 1 < starts.size() ? starts[i + 1] : hi);
        if (i + 1 < starts.size()) end = std::min(end, starts[i + 1]);
        end = std::min(end, hi);
        Body body;
        for (const auto& row : recompile::disassemble::listing(code, lo, a - lo, py::floordiv(end - a, 4)))
            body.append(Row(std::get<0>(row), std::get<2>(row), _ops(std::get<3>(row))));
        out.set(a, body);
    }
    return {out, img};
}

int64_t _site_arity(const Body& body, int64_t i) {
    bool regs[8] = {false, false, false, false, false, false, false, false};   // r3 .. r10
    for (int64_t j = i - 1; j > std::max<int64_t>(-1, i - LOOKBACK); --j) {
        const Row& row = body[j];
        const py::str& mn = std::get<1>(row);
        const py::list<py::str>& ops = std::get<2>(row);
        if (mn == py::str("bl") || mn == py::str("bla")) break;
        if (SETS().contains(mn.rstrip(py::str("."))) && !ops.empty()) {
            const int64_t r = reg_of(ops[0], false);
            if (r >= 3 && r <= 10) regs[r - 3] = true;
        }
    }
    int64_t k = 0;
    while (k < 8 && regs[k]) ++k;
    return k;
}

std::optional<int64_t> _consensus(const py::list<int64_t>& arities, bool stub) {
    if (arities.size() < 3) return std::nullopt;
    // Counter(arities).most_common(1)[0]: the most common value, the first seen among equals
    std::vector<std::pair<int64_t, int64_t>> counts;
    for (int64_t a : arities) {
        auto it = std::find_if(counts.begin(), counts.end(), [&](const std::pair<int64_t, int64_t>& c) { return c.first == a; });
        if (it == counts.end()) counts.emplace_back(a, 1);
        else ++it->second;
    }
    int64_t top = counts[0].first, n = counts[0].second;
    for (const auto& c : counts)
        if (c.second > n) top = c.first, n = c.second;
    const double share = static_cast<double>(n) / static_cast<double>(arities.size());
    if (stub) return share >= 0.85 ? std::optional<int64_t>(top) : std::nullopt;
    if (top == 8) return share >= 0.85 ? std::optional<int64_t>(8) : std::nullopt;
    return (share >= 0.6 && 1 <= top && top <= 7) ? std::optional<int64_t>(top) : std::nullopt;
}

Result analyse(const py::str& path) {
    auto [funcs, img] = _functions(path);
    std::unordered_map<int64_t, py::str> names;
    if (formats::xex::is_xex(path)) {
        for (const auto& imp : formats::xex_imports::imports(path)) {
            const auto& thunk = std::get<5>(imp);
            if (thunk && *thunk) names[*thunk] = std::get<2>(imp);
        }
    }
    for (const auto& kv : img->imports.items()) names.emplace(std::get<0>(kv), std::get<1>(kv));
    if (!img->run_names) throw py::AttributeError("'Image' object has no attribute 'run_names'");
    const auto& transparent = *img->run_names;

    // the functions in order, each with its index
    std::vector<int64_t> addrs;
    std::vector<Body> bodies;
    std::unordered_map<int64_t, size_t> index;
    for (const auto& kv : funcs.items()) {
        index.emplace(std::get<0>(kv), addrs.size());
        addrs.push_back(std::get<0>(kv));
        bodies.push_back(std::get<1>(kv));
    }
    const size_t nf = addrs.size();

    std::vector<int64_t> site_order;                                   // sites' keys in first-seen order
    std::unordered_map<int64_t, py::list<int64_t>> sites;
    std::vector<int64_t> local(nf, 0), fplocal(nf, 0);
    std::vector<std::vector<size_t>> callees(nf);
    std::vector<bool> everything(nf, false);
    Result r;
    r.functions = funcs.size();
    std::vector<int64_t> w;
    for (size_t fi = 0; fi < nf; ++fi) {
        const int64_t a = addrs[fi];
        const Body& body = bodies[fi];
        std::vector<std::pair<int64_t, std::u32string>> saved;
        for (const Row& row : body.vec()) {
            const py::list<py::str>& ops = std::get<2>(row);
            if (std::get<1>(row) == py::str("stw") && ops.size() >= 2) {
                const auto m = mem_of(ops[1]);
                if (m && m->base_is_r1) saved.emplace_back(reg_of(ops[0], false), m->disp.u32());
            }
        }
        std::sort(saved.begin(), saved.end());
        std::vector<int64_t> homed;
        for (int64_t k = 0; k < std::min<int64_t>(16, body.size()); ++k) {
            const Row& row = body[k];
            const py::str& mn = std::get<1>(row);
            const py::list<py::str>& ops = std::get<2>(row);
            if (mn == py::str("stwu")) break;
            if ((mn == py::str("stw") || mn == py::str("std")) && ops.size() >= 2 && ops[1].endswith(py::str("(r1)"))) {
                const int64_t reg = reg_of(ops[0], false);
                if (reg >= 0) homed.push_back(reg);
            }
        }
        auto has = [&](int64_t x) { return std::find(homed.begin(), homed.end(), x) != homed.end(); };
        if (has(8) && has(9) && has(10)) {
            int64_t lowest = INT64_MAX;
            for (int64_t reg : homed)
                if (3 <= reg && reg <= 10) lowest = std::min(lowest, reg);
            const int64_t fixed = lowest - 3;
            if (0 <= fixed && fixed <= 7) r.variadic.set(a, fixed);
        }
        for (int64_t i = 0; i < body.size(); ++i) {
            const Row& row = body[i];
            const py::str& mn = std::get<1>(row);
            const py::list<py::str>& ops = std::get<2>(row);
            const py::str b = mn.rstrip(py::str("."));
            if (b == py::str("bl") || b == py::str("bla")) {
                const std::optional<int64_t> t = _target(ops);
                if (t) {
                    auto it = sites.find(*t);
                    if (it == sites.end()) {
                        it = sites.emplace(*t, py::list<int64_t>()).first;
                        site_order.push_back(*t);
                    }
                    it->second.append(_site_arity(body, i));
                }
                if (t && transparent.contains(*t)) continue;
                const auto fit = t ? index.find(*t) : index.end();
                if (fit != index.end()) callees[fi].push_back(fit->second);
                else everything[fi] = true;
                continue;
            }
            if (INDIRECT().contains(b)) {
                everything[fi] = true;
                continue;
            }
            if ((b == py::str("lwz") || b == py::str("lhz") || b == py::str("lbz") || b == py::str("ld")) && ops.size() >= 2) {
                const auto m = mem_of(ops[1]);
                if (m && m->base_is_r1 &&
                    std::binary_search(saved.begin(), saved.end(), std::make_pair(reg_of(ops[0], false), m->disp.u32())))
                    continue;                          // the value the function saved: not a change
            }
            w.clear();
            gpr_writes_into(mn, ops, w);
            for (int64_t x : w)
                if (x <= 12 && ((VOLATILE_MASK >> x) & 1)) local[fi] |= int64_t(1) << x;
            w.clear();
            fpr_writes_into(mn, ops, w);
            for (int64_t x : w)
                if (x < 14) fplocal[fi] |= int64_t(1) << x;
        }
    }
    for (int64_t t : site_order) {
        const py::list<int64_t>& ars = sites.at(t);
        const auto nit = names.find(t);
        if (nit != names.end()) {
            if (!variadic_import(nit->second)) {
                const auto n = _consensus(ars, true);
                if (n) r.imports.set(nit->second, *n);
            }
        } else if (index.count(t)) {
            const auto n = _consensus(ars, false);
            if (n) r.arity.set(t, *n);
        }
    }
    struct Pass {
        py::dict<int64_t, int64_t>& out;
        const std::vector<int64_t>& loc;
        int64_t full;
    };
    for (const Pass& p : {Pass{r.clobber, local, VOLATILE_MASK}, Pass{r.fpclobber, fplocal, FP_VOLATILE_MASK}}) {
        std::vector<int64_t> mask(nf);
        for (size_t i = 0; i < nf; ++i) mask[i] = everything[i] ? p.full : p.loc[i];
        bool changed = true;
        while (changed) {
            changed = false;
            for (size_t i = 0; i < nf; ++i) {
                int64_t m = mask[i];
                if (m == p.full) continue;
                for (size_t g : callees[i]) m |= mask[g];
                if (m != mask[i]) {
                    mask[i] = m;
                    changed = true;
                }
            }
        }
        for (size_t i = 0; i < nf; ++i)
            if (mask[i] != p.full) p.out.set(addrs[i], mask[i]);
    }
    return r;
}

void write_tsv(const Result& r, const py::str& out) {
    auto sorted_items = [](const py::dict<int64_t, int64_t>& d) {
        std::vector<std::pair<int64_t, int64_t>> v;
        for (const auto& kv : d.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(v.begin(), v.end());
        return v;
    };
    py::str text("kind\taddress or name\tvalue\n");
    std::u32string acc;
    auto add = [&](const py::str& s) { acc += s.u32(); };
    for (const auto& kv : sorted_items(r.arity)) add(py::percent(py::str("arity\t%08X\t%d\n"), kv.first, kv.second));
    std::vector<std::pair<py::str, int64_t>> imps;
    for (const auto& kv : r.imports.items()) imps.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(imps.begin(), imps.end());
    for (const auto& kv : imps) add(py::percent(py::str("import arity\t%s\t%d\n"), kv.first, kv.second));
    for (const auto& kv : sorted_items(r.variadic)) add(py::percent(py::str("variadic\t%08X\t%d\n"), kv.first, kv.second));
    for (const auto& kv : sorted_items(r.clobber)) {
        py::list<py::str> regs;
        for (int64_t k : VOLATILE)
            if ((kv.second >> k) & 1) regs.append(py::percent(py::str("r%d"), k));
        add(py::percent(py::str("clobber\t%08X\t%s\n"), kv.first, regs.empty() ? py::str("-") : py::join(py::str(" "), regs)));
    }
    for (const auto& kv : sorted_items(r.fpclobber)) {
        py::list<py::str> regs;
        for (int64_t k = 0; k < 14; ++k)
            if ((kv.second >> k) & 1) regs.append(py::percent(py::str("f%d"), k));
        add(py::percent(py::str("fpclobber\t%08X\t%s\n"), kv.first, regs.empty() ? py::str("-") : py::join(py::str(" "), regs)));
    }
    py::file f = py::open(out, py::str("w"), py::str("utf-8"));
    f.write(text + py::str(acc));
    f.close();
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == py::str("-h") || argv[0] == py::str("--help")) {
        py::print(py::str(DOC));
        return argv.empty() ? 2 : 0;
    }
    std::optional<py::str> out;
    if (argv.contains(py::str("-o"))) out = argv[argv.index(py::str("-o")) + 1];
    const Result r = analyse(argv[0]);
    py::print(py::percent(py::str("%d functions: arity for %d, import arity for %d, %d variadic, %d keep a volatile register, "
                                  "%d keep a volatile FP register"),
                          r.functions, r.arity.size(), r.imports.size(), r.variadic.size(), r.clobber.size(), r.fpclobber.size()));
    if (out) {
        write_tsv(r, *out);
        py::print(py::percent(py::str("written: %s"), *out));
    }
    return 0;
}

}  // namespace codebreaker::analyze::ppc_calls
