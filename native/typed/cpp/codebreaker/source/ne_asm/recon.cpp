// codebreaker/source/ne_asm/recon.cpp -- typed C++ of codebreaker.source.ne_asm.recon.

#include "codebreaker/source/ne_asm/recon.hpp"

#include <algorithm>
#include <fstream>
#include <memory>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/masm.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/ne_asm/reachability.hpp"
#include "py_os.hpp"

namespace codebreaker::source::ne_asm::recon {

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

const py::set<py::str> JMPCC = {py::str("jmp"),   py::str("call"),  py::str("je"),    py::str("jne"),   py::str("jz"),     py::str("jnz"),
                                py::str("jl"),    py::str("jle"),   py::str("jg"),    py::str("jge"),   py::str("jb"),     py::str("jbe"),
                                py::str("ja"),    py::str("jae"),   py::str("jc"),    py::str("jnc"),   py::str("jo"),     py::str("jno"),
                                py::str("js"),    py::str("jns"),   py::str("jp"),    py::str("jnp"),   py::str("jpe"),    py::str("jpo"),
                                py::str("jcxz"),  py::str("loop"),  py::str("loope"), py::str("loopne"), py::str("loopz"), py::str("loopnz"),
                                py::str("jecxz")};

namespace {

const py::re::Pattern& ws_re() {
    static const py::re::Pattern p = py::re::compile(py::str("\\s+"));
    return p;
}
const py::re::Pattern& branch_re() {
    static const py::re::Pattern p = py::re::compile(py::str("([a-z]+) 0x([0-9a-f]+)$"));
    return p;
}

// the (op, target) of a row `op 0xTARGET` whose op is a branch/call mnemonic
std::optional<std::tuple<py::str, int64_t>> branch_of(const py::str& a) {
    const auto m = branch_re().match(ws_re().sub(py::str(" "), a).strip());
    if (m && JMPCC.contains(m->group(1))) return std::make_tuple(m->group(1), py::int_parse(m->group(2), 16));
    return std::nullopt;
}

std::vector<int64_t> sorted_vec(const py::set<int64_t>& s) {
    std::vector<int64_t> v;
    for (int64_t x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    return v;
}

}  // namespace

py::str masm_hex(int64_t v) {
    const py::str s = py::percent(py::str("%x"), v);
    const bool alpha = !s.empty() && py::str("abcdef").contains(s.slice(0, 1));
    return alpha ? py::str("0") + s + py::str("h") : s + py::str("h");
}

py::str to_masm(const py::str& asm0, const IsTarget& is_target) {
    static const py::re::Pattern pre_re = py::re::compile(py::str("(rep|repz|repe|repnz|repne|lock) (.*)$"));
    static const py::re::Pattern tgt_re = py::re::compile(py::str("0x([0-9a-f]+)$"));
    static const py::re::Pattern hex_re = py::re::compile(py::str("0x([0-9a-f]+)"));
    py::str asm_ = ws_re().sub(py::str(" "), asm0).strip();
    py::str pre;
    if (const auto mpre = pre_re.match(asm_)) {
        pre = mpre->group(1) + py::str(" ");
        asm_ = mpre->group(2);
    }
    const auto parts = asm_.partition(py::str(" "));
    const py::str op = std::get<0>(parts);
    py::str rest = std::get<2>(parts);
    if (op == py::str("lods") || op == py::str("stos") || op == py::str("movs") || op == py::str("scas") || op == py::str("cmps")) {
        return pre + op + (asm_.lower().contains(py::str("word ptr")) ? py::str("w") : py::str("b"));
    }
    if (JMPCC.contains(op)) {
        if (const auto m = tgt_re.match(rest)) {
            const std::optional<py::str> lbl = is_target(py::int_parse(m->group(1), 16));
            if (lbl && !lbl->empty()) return py::percent(py::str("%s %s"), op, *lbl);
        }
    }
    rest = rest.replace(py::str("DWORD PTR"), py::str("dword ptr"))
               .replace(py::str("WORD PTR"), py::str("word ptr"))
               .replace(py::str("BYTE PTR"), py::str("byte ptr"))
               .replace(py::str("FWORD PTR"), py::str("fword ptr"));
    rest = hex_re.sub([](const py::re::Match& m) { return masm_hex(py::int_parse(m.group(1), 16)); }, rest);
    return !rest.empty() ? (op + py::str(" ") + rest).strip() : op;
}

const py::list<py::str> RETF_MACRO = {py::str("retf    macro   n"), py::str("        ifb     <n>"), py::str("        db      0cbh"),
                                      py::str("        else"),      py::str("        db      0cah"),  py::str("        dw      n"),
                                      py::str("        endif"),     py::str("        endm"),          py::str("")};

bool _cancelled() { return false; }

py::str _dbline(const py::bytes& raw, int64_t off, const py::str& note) {
    py::list<py::str> items;
    for (int64_t b : raw) {
        const py::str h = py::percent(py::str("%x"), b);
        items.append(py::str(py::str("abcdef").contains(h.slice(0, 1)) ? py::percent(py::str("0%02xh"), b) : py::percent(py::str("%02xh"), b)));
    }
    return py::percent(py::str("        db      %-28s ; %04x %s"), py::join(py::str(", "), items), off, note);
}

void _emit_db(py::list<py::str>& lines, const py::bytes& raw, int64_t off, const py::str& note) {
    for (int64_t c = 0; c < raw.size(); c += 16) {
        lines.append(_dbline(raw.slice(c, c + 16), off + c, c == 0 ? note : py::str("")));
    }
}

void _emit_data(py::list<py::str>& lines, const py::bytes& code, int64_t s, int64_t e, const py::str& kind) {
    if (kind != py::str("str")) {
        _emit_db(lines, code.slice(s, e), s, py::str("data"));
        return;
    }
    auto printable = [&](int64_t j) { return 0x20 <= code[j] && code[j] < 0x7F && code[j] != 0x27; };
    int64_t i = s;
    while (i < e) {
        if (printable(i)) {
            int64_t j = i;
            // Cap the run at 40 chars/line: MASM 4.00 rejects an over-long source line (~128 char limit), so a long
            // printable data run must be split across several `db 'text'` lines -- otherwise the whole module fails to
            // assemble (3 severe errors, no OBJ). Same reason _emit_db chunks hex.
            while (j < e && printable(j) && j - i < 40) j += 1;
            lines.append(py::percent(py::str("        db      %-24s ; %04x string"), py::str("'") + code.slice(i, j).decode("latin1") + py::str("'"), i));
            i = j;
        } else {
            int64_t j = i;
            while (j < e && !printable(j)) j += 1;
            _emit_db(lines, code.slice(i, j), i, py::str("data"));
            i = j;
        }
    }
}

py::list<py::str> emit_function(const segment::Seg& sg, int64_t start, int64_t end, const py::set<int64_t>& force_db,
                                const py::list<std::tuple<int64_t, int64_t, py::str>>& data_spans) {
    std::vector<std::pair<int64_t, py::str>> rows;
    for (const auto& oa : sg.rows)
        if (start <= std::get<0>(oa) && std::get<0>(oa) < end) rows.emplace_back(std::get<0>(oa), std::get<1>(oa));
    py::set<int64_t> starts;
    for (const auto& r : rows) starts.add(r.first);
    py::dict<int64_t, py::str> labels;
    for (const auto& r : rows) {
        if (const auto b = branch_of(r.second)) {
            const int64_t t = std::get<1>(*b);
            if (start <= t && t < end && starts.contains(t)) labels.set(t, py::percent(py::str("L_%04x"), t));
        }
    }
    auto is_target = [&](int64_t o) -> std::optional<py::str> {
        return labels.contains(o) ? std::optional<py::str>(labels.at(o)) : std::nullopt;
    };

    std::vector<std::tuple<int64_t, int64_t, py::str>> dspans;
    for (const auto& d : data_spans) dspans.push_back(d);
    std::sort(dspans.begin(), dspans.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
        return std::get<2>(a).u32() < std::get<2>(b).u32();
    });
    auto span_at = [&](int64_t p) -> const std::tuple<int64_t, int64_t, py::str>* {
        for (const auto& d : dspans)
            if (std::get<0>(d) <= p && p < std::get<1>(d)) return &d;
        return nullptr;
    };
    auto next_data = [&](int64_t p) {
        int64_t best = end;
        bool found = false;
        for (const auto& d : dspans)
            if (std::get<0>(d) > p && (!found || std::get<0>(d) < best)) {
                best = std::get<0>(d);
                found = true;
            }
        return found ? best : end;
    };

    const py::bytes& code = sg.code;
    py::dict<int64_t, int64_t> off2idx;
    for (size_t i = 0; i < rows.size(); ++i) off2idx.set(rows[i].first, static_cast<int64_t>(i));
    std::vector<int64_t> row_starts;
    for (const auto& r : rows) row_starts.push_back(r.first);
    std::sort(row_starts.begin(), row_starts.end());
    py::list<py::str> lines;
    int64_t pos = start;
    while (pos < end) {
        if (const auto* sp = span_at(pos)) {   // readable DATA region
            const int64_t s = std::get<0>(*sp), e = std::get<1>(*sp);
            (void)s;
            const int64_t ee = std::min(e, end);
            if (labels.contains(pos)) lines.append(labels.at(pos) + py::str(":"));
            _emit_data(lines, code, pos, ee, std::get<2>(*sp));
            pos = ee;
            continue;
        }
        const int64_t clamp = next_data(pos);
        if (off2idx.contains(pos)) {
            const size_t i = static_cast<size_t>(off2idx.at(pos));
            const int64_t off = rows[i].first;
            const py::str& a = rows[i].second;
            const int64_t nxt = std::min({i + 1 < rows.size() ? rows[i + 1].first : end, end, clamp});
            const py::bytes raw = code.slice(off, nxt);
            if (labels.contains(off)) lines.append(labels.at(off) + py::str(":"));
            const py::str masm = to_masm(a, is_target);
            const auto mbr = branch_of(a);
            const bool external_branch = mbr && !labels.contains(std::get<1>(*mbr));
            // far call/jmp (9A/EA) is an absolute seg:off pointer -- inherently external, not a live MASM target here;
            // always db.
            const bool far = !raw.empty() && (raw[0] == 0x9A || raw[0] == 0xEA);
            // raw longer than any real 16-bit instruction (>8 bytes) means objdump collapsed a run of identical data
            // bytes into this row (its `...`); emitting the mnemonic live would DROP those bytes, so db the whole raw.
            // (This is invisible to the per-instruction check when the instr is a macro like retf, so catch it here.)
            if (force_db.contains(off) || external_branch || far || a.contains(py::str("(bad)")) || raw.size() > 8) {
                _emit_db(lines, raw, off, masm);
            } else {
                py::list<py::str> hx;
                for (int64_t k = 0; k < std::min<int64_t>(10, raw.size()); ++k) hx.append(py::percent(py::str("%02x"), raw[k]));
                const py::str hexc = py::join(py::str(" "), hx) + (raw.size() > 10 ? py::str(" ...") : py::str(""));
                lines.append(py::percent(py::str("        %-30s ; %04x %s"), masm, off, hexc));
            }
            pos = nxt;
        } else {
            // bytes not covered by any instruction row (carve start misaligned with objdump, or embedded data) -> db, so
            // coverage is complete and the all-db fallback is always byte-exact.
            int64_t after = end;
            for (int64_t o : row_starts)
                if (o > pos) {
                    after = o;
                    break;
                }
            const int64_t nextrow = std::min(after, clamp);
            if (labels.contains(pos)) lines.append(labels.at(pos) + py::str(":"));
            _emit_db(lines, code.slice(pos, nextrow), pos, py::str("data/uncovered"));
            pos = nextrow;
        }
    }
    return lines;
}

py::str wrap(const py::list<py::str>& body) {
    py::list<py::str> all = {py::str("        title   asmrecon")};
    all.extend(RETF_MACRO);
    all.extend(py::list<py::str>{py::str("_TEXT   segment byte public 'CODE'"), py::str("        assume  cs:_TEXT")});
    all.extend(body);
    all.extend(py::list<py::str>{py::str("_TEXT   ends"), py::str("        end")});
    return py::join(py::str("\n"), all) + py::str("\n");
}

std::tuple<py::set<int64_t>, py::set<int64_t>> _divergers(const py::str& lst_text, const py::bytes& our, const py::set<int64_t>& omask,
                                                          const py::bytes& shipped, const py::set<int64_t>& smask,
                                                          const py::dict<int64_t, int64_t>& spans, int64_t start) {
    static const py::re::Pattern lst_off = py::re::compile(py::str("^\\s{0,4}([0-9A-Fa-f]{4})\\b"));
    // origoff comment width is 4 for NE (<=64 KB segments) but grows to 5-6 hex for PE sections (asmrecon32 reuses this)
    // -- match the whole run, not exactly four.
    static const py::re::Pattern lst_cmt = py::re::compile(py::str(";\\s*([0-9a-f]{4,6})\\b"));
    static const py::re::Pattern bytefield = py::re::compile(py::str("\\s+[0-9A-Fa-f]{2}\\b"));
    // Build (masm_off, orig_off) for every BYTE-EMITTING listing line, in order. A byte-emitting line has a leading offset
    // AND a bytefield; label/segment directive lines have an offset but no bytes (skip). MASM wraps a long `db` line so its
    // `; NNNN` origoff comment lands on the CONTINUATION line -- pick it up from there, or the db's bytes get misattributed
    // to the prior instr's length (the bug that snowballed length-differers).
    const py::list<py::str> lines = py::splitlines(lst_text);
    std::vector<std::pair<int64_t, std::optional<int64_t>>> pairs;
    for (int64_t idx = 0; idx < lines.size(); ++idx) {
        const py::str& line = lines[idx];
        const auto mo = lst_off.match(line);
        if (!mo) continue;
        if (!bytefield.match(line.slice(mo->end(), py::len(line)))) continue;   // directive/label: no bytefield
        auto co = lst_cmt.search(line);
        if (!co && idx + 1 < lines.size() && !lst_off.match(lines[idx + 1])) co = lst_cmt.search(lines[idx + 1]);   // wrapped comment on next line
        pairs.emplace_back(py::int_parse(mo->group(1), 16), co ? std::optional<int64_t>(py::int_parse(co->group(1), 16)) : std::nullopt);
    }
    py::set<int64_t> lendiff, reencode;
    for (size_t i = 0; i < pairs.size(); ++i) {
        const int64_t moff = pairs[i].first;
        const std::optional<int64_t> ooff = pairs[i].second;
        if (!ooff || !spans.contains(*ooff)) continue;   // uncovered-data db line: already exact
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

py::list<std::tuple<int64_t, int64_t, py::str>> _safe_data_spans(const segment::Seg& sg, int64_t start, int64_t end) {
    py::list<std::tuple<int64_t, int64_t, py::str>> regs;
    py::set<int64_t> bt;
    try {
        regs = reachability::safe_data_regions(sg);
        bt = reachability::branch_targets(sg);
    } catch (const std::exception&) {
        return py::list<std::tuple<int64_t, int64_t, py::str>>();
    }
    std::vector<int64_t> rowset;
    for (const auto& oa : sg.rows) rowset.push_back(std::get<0>(oa));
    std::sort(rowset.begin(), rowset.end());
    py::list<std::tuple<int64_t, int64_t, py::str>> out;
    for (const auto& reg : regs) {
        const int64_t s = std::get<0>(reg), e = std::get<1>(reg);
        if (e <= start || s >= end) continue;
        int64_t lo = s;
        if (!rowset.empty() && rowset[0] <= s) lo = rowset[static_cast<size_t>(std::upper_bound(rowset.begin(), rowset.end(), s) - rowset.begin()) - 1];
        const size_t hi_i = static_cast<size_t>(std::lower_bound(rowset.begin(), rowset.end(), e) - rowset.begin());
        int64_t hi = hi_i < rowset.size() ? rowset[hi_i] : end;
        lo = std::max(lo, start);
        hi = std::min(hi, end);
        bool hit = hi <= lo;
        if (!hit)
            for (int64_t t : bt)
                if (lo <= t && t < hi) {
                    hit = true;   // snapping hit a target -> skip
                    break;
                }
        if (hit) continue;
        out.append(std::make_tuple(lo, hi, std::get<2>(reg)));
    }
    return out;
}

Reconciled reconcile(const segment::Seg& sg, int64_t start, int64_t end, const py::set<int64_t>& seg_mask, int64_t max_passes, bool verbose) {
    (void)verbose;
    static const py::re::Pattern err_re = py::re::compile(py::str("\\((\\d+)\\)\\s*:\\s*(?:error|fatal)"), py::re::I);
    static const py::re::Pattern off_re = py::re::compile(py::str(";\\s*([0-9a-f]{4})\\b"));
    const py::bytes shipped = sg.code.slice(start, end);
    py::set<int64_t> smask;
    for (int64_t m : seg_mask)
        if (start <= m && m < end) smask.add(m - start);
    const auto dspans = _safe_data_spans(sg, start, end);
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
    py::set<int64_t> force_db;
    py::set<int64_t> errored;   // offsets MASM can't assemble = definitively DATA
    std::optional<segdiff::DiffResult> res;
    py::str asm_ = py::str("");
    for (int64_t pass = 0; pass < max_passes; ++pass) {
        if (_cancelled()) break;   // Cancel pressed: bail immediately
        const py::list<py::str> body = emit_function(sg, start, end, force_db, dspans);
        asm_ = wrap(body);
        const py::list<py::str> asm_lines = py::splitlines(asm_);
        const build::masm::Assembly r = build::masm::assemble(asm_);
        // localize MASM errors to the offending instruction(s) via the `; NNNN` offset comment on each emitted line, and
        // db just those -- NOT the whole function -- so live instructions are preserved.
        py::set<int64_t> errs;
        const py::str logtext = (r.log ? *r.log : py::str("")) + py::str("\n") + (r.lst ? *r.lst : py::str(""));
        for (const auto& m : err_re.finditer(logtext)) {
            const int64_t ln = py::int_parse(m.group(1));
            if (1 <= ln && ln <= asm_lines.size()) {
                if (const auto cm = off_re.search(asm_lines[ln - 1])) errs.add(py::int_parse(cm->group(1), 16));
            }
        }
        if (!r.obj || !errs.empty()) {
            const py::set<int64_t> fresh = errs - force_db;
            if (!fresh.empty()) {
                force_db.update(fresh);
                errored.update(fresh);   // MASM errored on these -> data, not code
                continue;
            }
            if (force_db.size() >= static_cast<int64_t>(rows.size())) break;   // can't localize: last-resort db-all
            force_db = py::set<int64_t>::from(rows);
            continue;
        }
        const auto seg_ = formats::omf::code_segment(*r.obj);
        const py::bytes& our = std::get<1>(seg_);
        const py::set<int64_t>& omask = std::get<2>(seg_);
        res = segdiff::diff(our, omask, shipped, smask);
        if (res->exact) break;
        // two-phase per-instruction compare via the listing. Phase 1: db all LENGTH-differers and re-assemble -- this
        // restores the original layout, so a neighbouring near-jump MASM would otherwise re-encode comes out right and
        // stays LIVE (minimises db). Phase 2 (only once no length differs): db the true same-length re-encodes. Converges
        // in a few passes with near-minimal db.
        if (!r.lst) throw py::KeyError("lst");
        const auto dv = _divergers(*r.lst, our, omask, shipped, smask, spans, start);
        const py::set<int64_t> new_len = std::get<0>(dv) - force_db;
        if (!new_len.empty()) {
            force_db.update(new_len);
            continue;
        }
        const py::set<int64_t> new_re = std::get<1>(dv) - force_db;
        if (new_re.empty()) break;
        force_db.update(new_re);
    }
    // MASM-error instructions are data (objdump garbage MASM can't assemble): exclude them from the CODE count and the
    // reported code-db so live% reflects real instructions only. They stay db'd in the asm -> byte-exact unchanged.
    int64_t code_rows = 0;
    for (int64_t o : rows)
        if (!errored.contains(o)) code_rows += 1;
    return Reconciled{asm_, res, force_db - errored, code_rows};
}

std::optional<std::tuple<py::str, int64_t, int64_t>> _pick() {
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    for (const char* nm : {"MOUSE", "KEYBOARD", "SYSTEM", "GDI", "USER"}) {
        const py::str name(nm);
        std::unique_ptr<formats::ne::NEFile> ne;
        try {
            ne = std::make_unique<formats::ne::NEFile>(_target().resolve(name));
        } catch (const std::exception&) {
            continue;
        }
        const auto segs = ne->segments();
        for (int64_t i = 1; i <= segs.size(); ++i) {
            if (segs[i - 1].kind != py::str("CODE")) continue;
            const segment::Seg sg(*ne, i, o);
            std::vector<std::pair<int64_t, int64_t>> cands;
            for (const auto& f : sg.funcs.values()) {
                const int64_t n = f.end - f.start;
                if (16 <= n && n <= 120) cands.emplace_back(n, f.start);
            }
            if (!cands.empty()) {
                std::sort(cands.begin(), cands.end());
                return std::make_tuple(name, i, cands[0].second);
            }
        }
    }
    return std::nullopt;
}

std::tuple<py::str, std::optional<segdiff::DiffResult>> _run(const py::str& module, int64_t seg_index, std::optional<int64_t> func_off,
                                                             std::ostream& out) {
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    const formats::ne::NEFile ne(_target().resolve(module));
    const segment::Seg sg(ne, seg_index, o);
    const auto shipped = segdiff::shipped_ne_segment(_target().resolve(module), seg_index);
    const py::set<int64_t>& seg_mask = std::get<1>(shipped);
    if (!func_off) {   // smallest carved function in this seg
        std::vector<std::pair<int64_t, int64_t>> cands;
        for (const auto& f : sg.funcs.values())
            if (f.end > f.start) cands.emplace_back(f.end - f.start, f.start);
        if (cands.empty()) throw py::SystemExit(py::percent(py::str("no carved functions in %s seg%d"), module, seg_index));
        std::sort(cands.begin(), cands.end());
        func_off = cands[0].second;
    }
    if (!sg.funcs.contains(*func_off)) throw py::SystemExit(py::percent(py::str("no function carved at %s seg%d 0x%04x"), module, seg_index, *func_off));
    const segment::Func fn = sg.funcs.at(*func_off);
    const int64_t start = fn.start, end = fn.end;
    const Reconciled rc = reconcile(sg, start, end, seg_mask);
    const int64_t n = rc.n_code;
    const int64_t live = n - rc.force_db.size();
    out << py::percent(py::str("%s seg%d func 0x%04x  [%d bytes, %d instrs]"), module, seg_index, start, end - start, n).utf8() << "\n";
    out << "  verdict: " << ((rc.res && rc.res->exact) ? "BYTE-EXACT" : "not exact") << "\n";
    out << py::percent(py::str("  live: %d/%d instructions (%.0f%%), %d db"), live, n,
                       100.0 * static_cast<double>(live) / static_cast<double>(std::max<int64_t>(1, n)), rc.force_db.size())
               .utf8()
        << "\n";
    return std::make_tuple(rc.asm_, rc.res);
}

const py::str USAGE("usage: asmrecon.py [-h] [--seg SEG] [--func FUNC] [--selftest] [--out OUT] [module]");

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    std::optional<py::str> module, outfile;
    int64_t seg = 1;
    std::optional<int64_t> func;
    bool selftest = false;
    auto fail = [&](const std::string& msg) {
        err << USAGE.utf8() << "\nasmrecon.py: error: " << msg << "\n";
        return int64_t(2);
    };
    // the value of `--name value` / `--name=value` at argv[i] (advancing i), or nothing when it is missing
    auto value_of = [&](int64_t& i, const py::str& name, std::optional<py::str>& val) -> bool {
        const py::str a = argv[i];
        if (a == name) {
            if (i + 1 >= argv.size() || (argv[i + 1].startswith(py::str("-")) && py::len(argv[i + 1]) > 1 && !py::re::match(py::str("-\\.?\\d"), argv[i + 1]))) {
                val = std::nullopt;
                return true;
            }
            val = argv[++i];
            return true;
        }
        if (a.startswith(name + py::str("="))) {
            val = a.slice(py::len(name) + 1, py::len(a));
            return true;
        }
        return false;
    };
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str a = argv[i];
        if (a == py::str("-h") || a == py::str("--help")) {
            out << USAGE.utf8() << "\n";
            return 0;
        }
        std::optional<py::str> val;
        if (value_of(i, py::str("--seg"), val)) {
            if (!val) return fail("argument --seg: expected one argument");
            try {
                seg = py::int_parse(*val);
            } catch (const py::ValueError&) {
                return fail("argument --seg: invalid int value: " + py::repr(*val).utf8());
            }
        } else if (value_of(i, py::str("--func"), val)) {
            if (!val) return fail("argument --func: expected one argument");
            try {
                func = py::int_parse(*val, 0);
            } catch (const py::ValueError&) {
                return fail("argument --func: invalid <lambda> value: " + py::repr(*val).utf8());
            }
        } else if (value_of(i, py::str("--out"), val)) {
            if (!val) return fail("argument --out: expected one argument");
            outfile = val;
        } else if (a == py::str("--selftest")) {
            selftest = true;
        } else if (!a.empty() && a.at_(0) == U'-' && py::len(a) > 1 && !py::re::match(py::str("-\\.?\\d"), a)) {
            return fail("unrecognized arguments: " + a.utf8());
        } else if (!module) {
            module = a;
        } else {
            return fail("unrecognized arguments: " + a.utf8());
        }
    }
    try {
        if (selftest) {
            const auto pick = _pick();
            if (!pick) throw py::SystemExit(py::str("no suitable NE function found for selftest"));
            out << "asmrecon selftest on " << std::get<0>(*pick).utf8() << "\n\n";
            const auto r = _run(std::get<0>(*pick), std::get<1>(*pick), std::get<2>(*pick), out);
            const bool ok = std::get<1>(r) && std::get<1>(r)->exact;
            out << "\nSELFTEST: " << (ok ? "PASS - real function reconstructed byte-exact" : "FAIL") << "\n";
            return ok ? 0 : 1;
        }
        if (!module) return fail("give a module or --selftest");
        const auto r = _run(*module, seg, func, out);
        if (outfile) {
            const py::str text = std::get<0>(r).replace(py::str("\n"), py::str("\r\n"));   // newline="\r\n"
            std::ofstream f(outfile->utf8(), std::ios::binary);
            if (!f) throw py::OSError("cannot open " + outfile->utf8());
            f << text.utf8();
            f.close();
            out << "  wrote " << outfile->utf8() << "\n";
        }
        return (std::get<1>(r) && std::get<1>(r)->exact) ? 0 : 1;
    } catch (const py::SystemExit& e) {
        if (std::holds_alternative<py::str>(e.code)) err << std::get<py::str>(e.code).utf8() << "\n";
        return e.exit_status();
    }
}

}  // namespace codebreaker::source::ne_asm::recon
