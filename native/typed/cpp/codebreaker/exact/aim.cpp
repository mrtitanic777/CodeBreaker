// codebreaker/exact/aim.cpp -- C++17 port of codebreaker.exact.aim (see aim.hpp).

#include "codebreaker/exact/aim.hpp"

#include <algorithm>
#include <map>
#include <vector>

namespace codebreaker::exact::aim {

const std::tuple<py::str, py::str, py::str, py::str, py::str> KINDS = {py::str("frame"), py::str("width"), py::str("sign"),
                                                                       py::str("order"), py::str("shape")};
const py::dict<py::str, py::str> SIGNED = {
    {py::str("jl"), py::str("jb")},   {py::str("jle"), py::str("jbe")}, {py::str("jg"), py::str("ja")},
    {py::str("jge"), py::str("jae")}, {py::str("sar"), py::str("shr")}, {py::str("imul"), py::str("mul")},
    {py::str("idiv"), py::str("div")}, {py::str("cwd"), py::str("xor")}, {py::str("cbw"), py::str("sub")},
};

namespace {

const py::re::Pattern RE_PROC = py::re::compile(py::str(R"(\s*(_?\w+)\s+PROC\b)"));
const py::re::Pattern RE_ENDP = py::re::compile(py::str(R"(\s*_?\w+\s+ENDP\b)"));
const py::re::Pattern RE_LINE = py::re::compile(py::str(R"(; Line (\d+))"));
const py::re::Pattern RE_CODE = py::re::compile(py::str(R"(\s*\*\*\*\s+([0-9a-f]{6})\s+((?:[0-9a-f]{2} )+))"));
const py::re::Pattern RE_MEM = py::re::compile(py::str(R"(\[([^\]]*)\])"));
const py::re::Pattern RE_SPLIT = py::re::compile(py::str(R"(\s*\+\s*|\s*-\s*)"));
const py::re::Pattern RE_REG = py::re::compile(py::str(R"([a-z]{2})"));
const py::re::Pattern RE_NUM = py::re::compile(py::str(R"(0x[0-9a-f]+|\d+)"));
const py::re::Pattern RE_OPNUM = py::re::compile(py::str(R"((?<![\w\[+-])(0x[0-9a-f]+|\d+)\b)"));
const py::re::Pattern RE_BPSLOT = py::re::compile(py::str(R"(\[bp[-+]\d+\])"));
const py::re::Pattern RE_BYTEREG = py::re::compile(py::str(R"(\b(?:al|ah|bl|cl|dl)\b)"));

// int(text, 0) for a digit string or a 0x.. string: a decimal with leading zeros is a ValueError, as Python's
// (a number beyond int64 is an OverflowError: no real operand comes near it)
int64_t _int0(const py::str& s) {
    if (s.startswith(py::str("0x"))) {
        return py::int_from_str(s, 16);
    }
    if (py::len(s) > 1 && s[0] == py::str("0")) {
        for (int64_t i = 0; i < py::len(s); ++i) {
            if (s[i] != py::str("0")) {
                throw py::ValueError("leading zeros in decimal integer literals are not permitted");
            }
        }
    }
    return py::int_from_str(s, 10);
}

// SIGNED.get(a) == b
bool _signed_is(const py::str& a, const py::str& b) {
    return SIGNED.contains(a) && SIGNED.at(a) == b;
}

// x.split(" ", 1)[0]
py::str _mn(const py::str& x) {
    int64_t at = x.find(py::str(" "));
    return at < 0 ? x : x.slice_to(at);
}

py::str _sk(const py::str& x) {
    return RE_BPSLOT.sub(py::str("[bp]"), x);
}

bool _byte_or_cbw(const py::str& x) {
    return x == py::str("cbw") || x == py::str("cwde") || x.contains(py::str("byte ptr")) || x.startswith(py::str("sub ah"));
}

}  // namespace

py::list<ListingEntry> listing_code(const py::str& text, const std::optional<py::str>& name) {
    py::list<ListingEntry> out;
    std::optional<int64_t> line;
    bool inside = !name.has_value();
    for (const py::str& l : py::split(text, py::str("\n"))) {
        std::optional<py::re::Match> m = RE_PROC.match(l);
        if (m) {
            inside = !name.has_value() || m->group(1).lstrip(py::str("_")) == name->lstrip(py::str("_"));
            continue;
        }
        if (RE_ENDP.match(l)) {
            if (inside && py::truthy(out)) {
                break;
            }
            continue;
        }
        m = RE_LINE.match(l);
        if (m) {
            line = py::int_from_str(m->group(1), 10);
            continue;
        }
        m = RE_CODE.match(l);
        if (m && inside) {
            out.append(ListingEntry{py::int_from_str(m->group(1), 16),
                                    py::bytes::fromhex(m->group(2).replace(py::str(" "), py::str(""))), line});
        }
    }
    return out;
}

py::str _norm(const x86::Insn& ins, int64_t base) {
    (void)base;
    py::str ops = ins.op_str;
    if (ins.mnemonic.startswith(py::str("j")) || ins.mnemonic == py::str("call") || ins.mnemonic == py::str("loop")) {
        return ins.mnemonic + py::str(" T");
    }

    // (a place: a frame's kept; a displacement of the data - large, or the 0 of an unlinked object - masked)
    auto mem = [](const py::re::Match& m) -> py::str {
        py::str inner = m.group(1);
        py::list<py::str> parts;
        for (const py::str& p : RE_SPLIT.split(inner)) {
            parts.append(p.strip());
        }
        py::list<py::str> regs;
        py::list<int64_t> nums;
        for (const py::str& p : parts) {
            if (RE_REG.fullmatch(p)) {
                regs.append(p);
            }
        }
        for (const py::str& p : parts) {
            if (RE_NUM.fullmatch(p)) {
                nums.append(_int0(p));
            }
        }
        if (regs.contains(py::str("bp")) && py::truthy(nums)) {
            return py::percent(py::str("[%s%s%d]"), py::join(py::str("+"), regs),
                               inner.contains(py::str(" - ")) ? py::str("-") : py::str("+"), nums[0]);
        }
        py::list<int64_t> small;
        for (int64_t n : nums) {
            if (0 < n && n < 0x80) {
                small.append(n);
            }
        }
        py::str joined = py::join(py::str("+"), regs);
        py::str reg_part = py::truthy(joined) ? joined : py::str("M");
        py::str disp = (py::truthy(small) && py::truthy(regs)) ? py::percent(py::str("+%d"), small[0]) : py::str("");
        return py::percent(py::str("[%s%s]"), reg_part, disp);
    };
    ops = RE_MEM.sub(mem, ops);
    ops = RE_OPNUM.sub(
        [](const py::re::Match& m) -> py::str {
            int64_t v = _int0(m.group(0));
            return (0 < v && v < 0x80) ? m.group(0) : py::str("N");
        },
        ops);
    return (ins.mnemonic + py::str(" ") + ops).strip();
}

py::list<x86::Insn> _insns(const py::bytes& code) {
    return x86::disasm(code, 0, py::len(code), true);
}

py::str _kind(const py::list<py::str>& a, const py::list<py::str>& b) {
    if (py::len(a) == py::len(b)) {
        bool same_frame = true;
        for (int64_t i = 0; i < py::len(a); ++i) {
            if (_sk(a[i]) != _sk(b[i])) {
                same_frame = false;
                break;
            }
        }
        if (same_frame) {
            return py::str("frame");
        }
        bool all_sign = true;
        bool any_sign = false;
        for (int64_t i = 0; i < py::len(a); ++i) {
            py::str mx = _mn(a[i]);
            py::str my = _mn(b[i]);
            bool related = _signed_is(mx, my) || _signed_is(my, mx);
            if (!(mx == my || related)) {
                all_sign = false;
            }
            if (related) {
                any_sign = true;
            }
        }
        if (all_sign && any_sign) {
            return py::str("sign");
        }
        for (int64_t i = 0; i < py::len(a); ++i) {
            const py::str& x = a[i];
            const py::str& y = b[i];
            if (x.contains(py::str("byte ptr")) != y.contains(py::str("byte ptr")) ||
                (RE_BYTEREG.search(x) && !RE_BYTEREG.search(y))) {
                return py::str("width");
            }
        }
    }
    if (py::sorted(a) == py::sorted(b)) {
        return py::str("order");
    }
    py::list<py::str> both = a + b;
    bool any_marker = false;
    for (const py::str& x : both) {
        if (_byte_or_cbw(x)) {
            any_marker = true;
            break;
        }
    }
    if (any_marker && std::abs(py::len(a) - py::len(b)) <= 2) {
        auto keep = [](const py::list<py::str>& xs) {
            py::list<py::str> out;
            for (const py::str& x : xs) {
                if (x.contains(py::str("byte ptr")) || x == py::str("cbw") || x == py::str("cwde")) {
                    out.append(x);
                }
            }
            return out;
        };
        if (keep(a) != keep(b)) {
            return py::str("width");
        }
    }
    return py::str("shape");
}

Diagnosis diagnose(const py::bytes& orig, const py::bytes& code, const py::list<ListingEntry>& listing) {
    py::list<x86::Insn> oi = _insns(orig);
    py::list<x86::Insn> ci = _insns(code);
    py::list<py::str> on;
    py::list<py::str> cn;
    for (const x86::Insn& i : oi) {
        on.append(_norm(i, 0));
    }
    for (const x86::Insn& i : ci) {
        cn.append(_norm(i, 0));
    }
    py::dict<int64_t, std::optional<int64_t>> at;
    for (const ListingEntry& e : listing) {
        at.set(std::get<0>(e), std::get<2>(e));
    }
    py::list<std::optional<int64_t>> lines_of;
    std::optional<int64_t> last;
    for (const x86::Insn& i : ci) {                            // (each candidate instruction's line: the last given)
        last = at.get(i.address, last);
        lines_of.append(last);
    }
    py::dict<py::str, int64_t> kinds;
    py::list<int64_t> lines;
    for (const Opcode& op : _get_opcodes(on, cn)) {
        const py::str& tag = std::get<0>(op);
        int64_t i1 = std::get<1>(op);
        int64_t i2 = std::get<2>(op);
        int64_t j1 = std::get<3>(op);
        int64_t j2 = std::get<4>(op);
        if (tag == py::str("equal")) {
            continue;
        }
        py::str k = _kind(on.slice(i1, i2), cn.slice(j1, j2));
        kinds.set(k, kinds.get(k, 0) + py::max(i2 - i1, j2 - j1));
        for (int64_t j = j1; j < py::max(j2, j1 + 1); ++j) {
            std::optional<int64_t> ln = py::truthy(lines_of) ? lines_of[py::min(j, py::len(lines_of) - 1)] : std::nullopt;
            if (ln.has_value() && !lines.contains(*ln)) {
                lines.append(*ln);
            }
        }
    }
    // (the same instructions elsewhere: a block the layout moved - order, not shape)
    if (kinds.get(py::str("shape"), 0) != 0 && py::sorted(on) == py::sorted(cn)) {
        int64_t moved = kinds.pop(py::str("shape")) + kinds.get(py::str("order"), 0);
        kinds.set(py::str("order"), moved);
    }
    Diagnosis d;
    d.kinds = kinds;
    d.lines = lines;
    d.first = py::truthy(lines) ? std::optional<int64_t>(lines[0]) : std::nullopt;
    return d;
}

py::bytes code_of(const py::list<ListingEntry>& listing) {
    if (!py::truthy(listing)) {
        return py::bytes("");
    }
    int64_t base = std::get<0>(listing[0]);
    py::bytearray out;
    for (const ListingEntry& e : listing) {
        int64_t off = std::get<0>(e);
        const py::bytes& b = std::get<1>(e);
        out.set_slice(off - base, off - base + py::len(b), b);
    }
    return py::bytes(out);
}

// ---------------------------------------------------------------- difflib.SequenceMatcher (no junk, autojunk off)

namespace {

struct Block {
    int64_t i, j, k;
    bool operator<(const Block& o) const { return std::tie(i, j, k) < std::tie(o.i, o.j, o.k); }
};

class SequenceMatcher {
public:
    SequenceMatcher(const py::list<py::str>& a, const py::list<py::str>& b) : a_(a), b_(b) {
        for (int64_t j = 0; j < py::len(b_); ++j) {
            b2j_[b_[j]].push_back(j);
        }
    }

    // the longest matching block in a[alo:ahi] and b[blo:bhi]: earliest in a, then in b, among the longest
    Block find_longest_match(int64_t alo, int64_t ahi, int64_t blo, int64_t bhi) const {
        int64_t besti = alo, bestj = blo, bestsize = 0;
        std::map<int64_t, int64_t> j2len;
        for (int64_t i = alo; i < ahi; ++i) {
            std::map<int64_t, int64_t> newj2len;
            auto it = b2j_.find(a_[i]);
            if (it != b2j_.end()) {
                for (int64_t j : it->second) {
                    if (j < blo) {
                        continue;
                    }
                    if (j >= bhi) {
                        break;
                    }
                    auto prev = j2len.find(j - 1);
                    int64_t k = (prev == j2len.end() ? 0 : prev->second) + 1;
                    newj2len[j] = k;
                    if (k > bestsize) {
                        besti = i - k + 1;
                        bestj = j - k + 1;
                        bestsize = k;
                    }
                }
            }
            j2len = newj2len;
        }
        // (no junk elements: the extensions below find nothing, kept as difflib has them)
        while (besti > alo && bestj > blo && a_[besti - 1] == b_[bestj - 1]) {
            --besti;
            --bestj;
            ++bestsize;
        }
        while (besti + bestsize < ahi && bestj + bestsize < bhi && a_[besti + bestsize] == b_[bestj + bestsize]) {
            ++bestsize;
        }
        return {besti, bestj, bestsize};
    }

    std::vector<Block> get_matching_blocks() const {
        int64_t la = py::len(a_), lb = py::len(b_);
        std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t>> queue = {{0, la, 0, lb}};
        std::vector<Block> matching;
        while (!queue.empty()) {
            auto [alo, ahi, blo, bhi] = queue.back();
            queue.pop_back();
            Block x = find_longest_match(alo, ahi, blo, bhi);
            if (x.k) {
                matching.push_back(x);
                if (alo < x.i && blo < x.j) {
                    queue.push_back({alo, x.i, blo, x.j});
                }
                if (x.i + x.k < ahi && x.j + x.k < bhi) {
                    queue.push_back({x.i + x.k, ahi, x.j + x.k, bhi});
                }
            }
        }
        std::sort(matching.begin(), matching.end());
        // collapse adjacent blocks
        int64_t i1 = 0, j1 = 0, k1 = 0;
        std::vector<Block> non_adjacent;
        for (const Block& m : matching) {
            if (i1 + k1 == m.i && j1 + k1 == m.j) {
                k1 += m.k;
            } else {
                if (k1) {
                    non_adjacent.push_back({i1, j1, k1});
                }
                i1 = m.i;
                j1 = m.j;
                k1 = m.k;
            }
        }
        if (k1) {
            non_adjacent.push_back({i1, j1, k1});
        }
        non_adjacent.push_back({la, lb, 0});
        return non_adjacent;
    }

private:
    py::list<py::str> a_;
    py::list<py::str> b_;
    std::map<py::str, std::vector<int64_t>> b2j_;
};

}  // namespace

py::list<Opcode> _get_opcodes(const py::list<py::str>& a, const py::list<py::str>& b) {
    SequenceMatcher sm(a, b);
    int64_t i = 0, j = 0;
    py::list<Opcode> answer;
    for (const Block& m : sm.get_matching_blocks()) {
        const char* tag = "";
        if (i < m.i && j < m.j) {
            tag = "replace";
        } else if (i < m.i) {
            tag = "delete";
        } else if (j < m.j) {
            tag = "insert";
        }
        if (*tag) {
            answer.append(Opcode{py::str(tag), i, m.i, j, m.j});
        }
        i = m.i + m.k;
        j = m.j + m.k;
        if (m.k) {
            answer.append(Opcode{py::str("equal"), i - m.k, i, j - m.k, j});
        }
    }
    return answer;
}

}  // namespace codebreaker::exact::aim
