// test.cpp -- the typed C++ codebreaker.source.ne_asm.recon on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <fstream>

#include "../_ne_common/necommon.hpp"
#include "codebreaker/source/ne_asm/recon.hpp"
#include "codebreaker/source/ne_asm/segdiff.hpp"

namespace R = codebreaker::source::ne_asm::recon;
namespace sd = codebreaker::source::ne_asm::segdiff;
using namespace nh;

static string lines_d(const py::list<py::str>& ls) {
    string all;
    for (int64_t i = 0; i < ls.size(); ++i) all += (i ? "\n" : "") + ls[i].utf8();
    return jarr({jint(ls.size()), jstr(sha1(all))});
}

static string spans_j(const py::list<std::tuple<int64_t, int64_t, py::str>>& sp) {
    vector<string> o;
    for (const auto& t : sp) o.push_back(jarr({jint(std::get<0>(t)), jint(std::get<1>(t)), jstr(std::get<2>(t).utf8())}));
    return jarr(o);
}

static string set_j(const py::set<int64_t>& s) {
    vector<int64_t> v;
    for (int64_t x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    vector<string> o;
    for (int64_t x : v) o.push_back(jint(x));
    return jarr(o);
}

static string res_d(const std::optional<sd::DiffResult>& r) {
    if (!r) return "null";
    return jarr({r->exact ? "true" : "false", r->length_match ? "true" : "false", jint(r->our_len), jint(r->shipped_len), jint(r->n_diff),
                 r->first_diff ? jint(*r->first_diff) : "null", jint(r->masked)});
}

static bool force_of(int64_t off) { return (off * 37 + 11) % 5 == 0; }

static R::IsTarget target_of(int mode) {
    if (!mode) return [](int64_t) -> std::optional<py::str> { return std::nullopt; };
    return [](int64_t o) -> std::optional<py::str> {
        if (o % 2 == 0) return py::percent(py::str("L_%04x"), o);
        return std::nullopt;
    };
}

static vector<string> split_commas(const string& s) {
    if (s == "~") return {};
    return split(s, ',');
}

static string reconcile_case(const string& path, int64_t si, int64_t lo, int64_t hi, int64_t passes) {
    auto& sg = seg_of(path, si);
    const auto shipped = sd::shipped_ne_segment(py::str(path), si);
    const R::Reconciled r = passes < 0 ? R::reconcile(sg, lo, hi, std::get<1>(shipped)) : R::reconcile(sg, lo, hi, std::get<1>(shipped), passes);
    return jarr({jstr(sha1(r.asm_.utf8())), res_d(r.res), set_j(r.force_db), jint(r.n_code)});
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    return run_cases(argc, argv, [](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") {
            vector<py::str> v;
            for (const py::str& x : R::JMPCC) v.push_back(x);
            std::sort(v.begin(), v.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            vector<string> j, m;
            for (const py::str& x : v) j.push_back(jstr(x.utf8()));
            for (const py::str& x : R::RETF_MACRO) m.push_back(jstr(x.utf8()));
            return jarr({jarr(j), jarr(m)});
        }
        if (op == "hex") return jstr(R::masm_hex(std::stoll(f[1])).utf8());
        if (op == "tomasm") return jstr(R::to_masm(py::str(unhex(f[1])), target_of(std::stoi(f[2]))).utf8());
        if (op == "tomasm_rows") {
            auto& sg = seg_of(f[1], std::stoll(f[2]));
            py::list<py::str> out;
            const auto t = target_of(std::stoi(f[3]));
            for (const auto& oa : sg.rows) out.append(R::to_masm(std::get<1>(oa), t));
            return lines_d(out);
        }
        if (op == "sds") {
            auto& sg = seg_of(f[1], std::stoll(f[2]));
            return spans_j(R::_safe_data_spans(sg, std::stoll(f[3]), std::stoll(f[4])));
        }
        if (op == "emit") {
            auto& sg = seg_of(f[1], std::stoll(f[2]));
            const int64_t lo = std::stoll(f[3]), hi = std::stoll(f[4]);
            const int mode = std::stoi(f[5]);
            py::set<int64_t> force;
            if (mode)
                for (const auto& oa : sg.rows)
                    if (lo <= std::get<0>(oa) && std::get<0>(oa) < hi && force_of(std::get<0>(oa))) force.add(std::get<0>(oa));
            py::list<std::tuple<int64_t, int64_t, py::str>> spans;
            if (mode == 2) spans = R::_safe_data_spans(sg, lo, hi);
            return lines_d(R::emit_function(sg, lo, hi, force, spans));
        }
        if (op == "dbline") return jstr(R::_dbline(from_hex(f[1] == "~" ? "" : f[1]), std::stoll(f[2]), py::str(unhex(f[3]))).utf8());
        if (op == "emitdb") {
            py::list<py::str> out;
            R::_emit_db(out, from_hex(f[1] == "~" ? "" : f[1]), std::stoll(f[2]), py::str(unhex(f[3])));
            vector<string> o;
            for (const py::str& s : out) o.push_back(jstr(s.utf8()));
            return jarr(o);
        }
        if (op == "emitdata") {
            py::list<py::str> out;
            const py::bytes raw = from_hex(f[1] == "~" ? "" : f[1]);
            R::_emit_data(out, raw, 0, raw.size(), py::str(f[2]));
            vector<string> o;
            for (const py::str& s : out) o.push_back(jstr(s.utf8()));
            return jarr(o);
        }
        if (op == "wrap") {
            py::list<py::str> ls;
            const int64_t n = std::stoll(f[1]);
            for (int64_t i = 0; i < n; ++i) ls.append(py::str("        db      01h" + string(static_cast<size_t>(i), ' ')));
            return jstr(R::wrap(ls).utf8());
        }
        if (op == "div") {
            const string base = "inputs/div_" + f[1];
            const string lst = read_file(base + ".lst");
            std::istringstream meta(read_file(base + ".meta"));
            string start, our, shipped, om, sm, sp;
            std::getline(meta, start);
            std::getline(meta, our);
            std::getline(meta, shipped);
            std::getline(meta, om);
            std::getline(meta, sm);
            std::getline(meta, sp);
            py::set<int64_t> omask, smask;
            for (const string& x : split_commas(om)) omask.add(std::stoll(x));
            for (const string& x : split_commas(sm)) smask.add(std::stoll(x));
            py::dict<int64_t, int64_t> spans;
            for (const string& x : split_commas(sp)) {
                const auto kv = split(x, ':');
                spans.set(std::stoll(kv[0]), std::stoll(kv[1]));
            }
            const auto r = R::_divergers(py::str(lst), from_hex(our == "~" ? "" : our), omask, from_hex(shipped == "~" ? "" : shipped), smask, spans,
                                         std::stoll(start));
            return jarr({set_j(std::get<0>(r)), set_j(std::get<1>(r))});
        }
        if (op == "recon") return reconcile_case(f[1], std::stoll(f[2]), std::stoll(f[3]), std::stoll(f[4]), f.size() > 5 ? std::stoll(f[5]) : -1);
        if (op == "pick") {
            const auto p = R::_pick();
            if (!p) return jarr({"null", "null", "null"});
            return jarr({jstr(std::get<0>(*p).utf8()), jint(std::get<1>(*p)), jint(std::get<2>(*p))});
        }
        if (op == "main") {
            py::list<py::str> args;
            vector<string> files;
            for (size_t i = 1; i < f.size(); ++i) {
                const string a = unhex(f[i]);
                args.append(py::str(a));
                const string b = a.substr(a.rfind('=') == string::npos ? 0 : a.rfind('=') + 1);
                if (b.compare(0, 4, "out_") == 0) files.push_back(b);
            }
            std::ostringstream out, err;
            const int64_t status = R::main(args, out, err);
            string extra = "null";
            for (const string& b : files) {
                std::ifstream in(b, std::ios::binary);
                if (in) {
                    std::stringstream ss;
                    ss << in.rdbuf();
                    in.close();
                    extra = jstr(sha1(ss.str()));
                    std::remove(b.c_str());
                }
            }
            return jarr({jint(status), jstr(status == 0 ? out.str() : ""), extra});
        }
        throw py::ValueError("bad case " + op);
    });
}
