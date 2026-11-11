// codebreaker/source/ne_asm/reachability.cpp -- typed C++ of codebreaker.source.ne_asm.reachability.

#include "codebreaker/source/ne_asm/reachability.hpp"

#include <algorithm>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/ne_asm/datacode.hpp"
#include "py_os.hpp"

namespace codebreaker::source::ne_asm::reachability {

const py::set<py::str> JMPCC = {py::str("jmp"),   py::str("call"),  py::str("je"),    py::str("jne"),   py::str("jz"),     py::str("jnz"),
                                py::str("jl"),    py::str("jle"),   py::str("jg"),    py::str("jge"),   py::str("jb"),     py::str("jbe"),
                                py::str("ja"),    py::str("jae"),   py::str("jc"),    py::str("jnc"),   py::str("jo"),     py::str("jno"),
                                py::str("js"),    py::str("jns"),   py::str("jp"),    py::str("jnp"),   py::str("jpe"),    py::str("jpo"),
                                py::str("jcxz"),  py::str("jecxz"), py::str("loop"),  py::str("loope"), py::str("loopne"), py::str("loopz"),
                                py::str("loopnz")};
const py::set<py::str> NOFALL = {py::str("jmp"),  py::str("ret"),   py::str("retf"),  py::str("retn"),
                                 py::str("iret"), py::str("iretf"), py::str("iretd"), py::str("iretw")};

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

namespace {

const py::re::Pattern& ws_re() {
    static const py::re::Pattern p = py::re::compile(py::str("\\s+"));
    return p;
}
const py::re::Pattern& branch_re() {
    static const py::re::Pattern p = py::re::compile(py::str("([a-z]+) 0x([0-9a-f]+)$"));
    return p;
}
const py::re::Pattern& seg_re() {
    static const py::re::Pattern p = py::re::compile(py::str("seg(\\d+):0x([0-9a-f]+)$"));
    return p;
}

py::str norm(const py::str& a) { return ws_re().sub(py::str(" "), a).strip(); }

// the (op, target) of a direct branch row `op 0xTARGET` in JMPCC, else nothing
std::optional<std::tuple<py::str, int64_t>> direct_branch(const py::str& a) {
    const auto m = branch_re().match(a);
    if (m && JMPCC.contains(m->group(1))) return std::make_tuple(m->group(1), py::int_parse(m->group(2), 16));
    return std::nullopt;
}

// the intra-segment relocation targets: fixups "segN:0xNNNN" with N the segment's own index
std::vector<int64_t> own_fixup_targets(const segment::Seg& sg) {
    std::vector<int64_t> out;
    for (const auto& kv : sg.fixups.items()) {
        const auto m = seg_re().match(std::get<1>(kv));
        if (m && py::int_parse(m->group(1)) == sg.si) out.push_back(py::int_parse(m->group(2), 16));
    }
    return out;
}

}  // namespace

py::set<int64_t> reachable(const segment::Seg& sg) {
    py::dict<int64_t, py::str> rows;
    for (const auto& oa : sg.rows) rows.set(std::get<0>(oa), norm(std::get<1>(oa)));
    std::vector<int64_t> starts = rows.keys();
    std::sort(starts.begin(), starts.end());
    py::dict<int64_t, int64_t> nxt;   // absent: no next row (Python's None)
    for (size_t i = 0; i + 1 < starts.size(); ++i) nxt.set(starts[i], starts[i + 1]);

    std::vector<int64_t> seeds;
    for (int64_t k : sg.funcs.keys()) seeds.push_back(k);
    for (int64_t k : sg.export_names.keys()) seeds.push_back(k);
    for (int64_t t : own_fixup_targets(sg)) seeds.push_back(t);

    py::set<int64_t> reached;
    std::vector<int64_t> wl;
    for (int64_t s : seeds)
        if (rows.contains(s)) wl.push_back(s);
    while (!wl.empty()) {
        const int64_t o = wl.back();
        wl.pop_back();
        if (reached.contains(o) || !rows.contains(o)) continue;
        reached.add(o);
        const py::str& a = rows.at(o);
        const py::str mn = py::split(a, py::str(" "))[0];
        if (const auto b = direct_branch(a)) {   // direct branch/call target
            const int64_t t = std::get<1>(*b);
            if (rows.contains(t)) wl.push_back(t);
        }
        if (!NOFALL.contains(mn) && !a.contains(py::str("(bad)"))) {   // fall-through
            if (nxt.contains(o)) wl.push_back(nxt.at(o));
        }
    }
    return reached;
}

py::set<int64_t> branch_targets(const segment::Seg& sg) {
    py::set<int64_t> tgts;
    for (int64_t k : sg.funcs.keys()) tgts.add(k);
    for (int64_t k : sg.export_names.keys()) tgts.add(k);
    for (const auto& oa : sg.rows) {
        if (const auto b = direct_branch(norm(std::get<1>(oa)))) tgts.add(std::get<1>(*b));
    }
    for (int64_t t : own_fixup_targets(sg)) tgts.add(t);
    return tgts;
}

py::list<std::tuple<int64_t, int64_t>> _branches(const segment::Seg& sg) {
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& oa : sg.rows) {
        if (const auto b = direct_branch(norm(std::get<1>(oa)))) out.append(std::make_tuple(std::get<0>(oa), std::get<1>(*b)));
    }
    return out;
}

py::list<std::tuple<int64_t, int64_t, py::str>> safe_data_regions(const segment::Seg& sg) {
    const auto branches = _branches(sg);
    const std::vector<int64_t> funcs = sg.funcs.keys();
    const std::vector<int64_t> exps = sg.export_names.keys();
    const std::vector<int64_t> ext_fix = own_fixup_targets(sg);
    py::list<std::tuple<int64_t, int64_t, py::str>> out;
    for (const auto& reg : datacode::data_regions(sg)) {
        const int64_t s = std::get<0>(reg), e = std::get<1>(reg);
        auto in = [&](int64_t x) { return s <= x && x < e; };
        bool entered = std::any_of(funcs.begin(), funcs.end(), in) || std::any_of(exps.begin(), exps.end(), in) ||
                       std::any_of(ext_fix.begin(), ext_fix.end(), in);
        if (!entered) {
            for (const auto& b : branches) {
                const int64_t src = std::get<0>(b), t = std::get<1>(b);
                if (in(t) && !in(src)) {
                    entered = true;
                    break;
                }
            }
        }
        if (!entered) out.append(std::make_tuple(s, e, std::get<2>(reg)));
    }
    return out;
}

namespace {
const char* const USAGE = "usage: reachability.py [-h] [--seg SEG] module\n";
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    std::optional<py::str> module;
    int64_t seg = 1;
    auto fail = [&](const std::string& msg) {
        err << USAGE << "reachability.py: error: " << msg << "\n";
        return int64_t(2);
    };
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str a = argv[i];
        if (a == py::str("-h") || a == py::str("--help")) {
            out << USAGE;
            return 0;
        }
        py::str val;
        bool is_seg = false;
        if (a == py::str("--seg")) {
            if (i + 1 >= argv.size()) return fail("argument --seg: expected one argument");
            val = argv[++i];
            is_seg = true;
        } else if (a.startswith(py::str("--seg="))) {
            val = a.slice(6, py::len(a));
            is_seg = true;
        }
        if (is_seg) {
            try {
                seg = py::int_parse(val);
            } catch (const py::ValueError&) {
                return fail("argument --seg: invalid int value: " + py::repr(val).utf8());
            }
        } else if (!a.empty() && a.at_(0) == U'-' && py::len(a) > 1) {
            return fail("unrecognized arguments: " + a.utf8());
        } else if (!module) {
            module = a;
        } else {
            return fail("unrecognized arguments: " + a.utf8());
        }
    }
    if (!module) return fail("the following arguments are required: module");
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    const formats::ne::NEFile ne(_target().resolve(*module));
    const segment::Seg sg(ne, seg, o);
    const py::set<int64_t> reached = reachable(sg);
    const int64_t total = sg.rows.size();
    const int64_t unreached = total - reached.size();
    out << py::percent(py::str("%s seg%d: %d rows, %d reachable code, %d unreachable (%.1f%% -> data candidates)"), *module, seg, total,
                       reached.size(), unreached, 100.0 * static_cast<double>(unreached) / static_cast<double>(std::max<int64_t>(1, total)))
               .utf8()
        << "\n";
    return 0;
}

}  // namespace codebreaker::source::ne_asm::reachability
