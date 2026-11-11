// test.cpp -- the typed C++ codebreaker.source.ne_asm.idiom on the cases of expected.py (cases.txt, inputs/, names_*.json); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <fstream>

#include "../_ne_common/necommon.hpp"
#include "codebreaker/source/ne_asm/idiom.hpp"
#include "codebreaker/source/ne_asm/recon.hpp"
#include "codebreaker/source/ne_asm/segdiff.hpp"
#include "py_stdlib.hpp"

namespace I = codebreaker::source::ne_asm::idiom;
namespace R = codebreaker::source::ne_asm::recon;
namespace sd = codebreaker::source::ne_asm::segdiff;
using namespace nh;

static const char* ODD[] = {"9bad-name", "", "ok$?@_1", "n\xc3\xa9 w", "a b", "_x"};

static seg::Seg& ovseg(const string& path, int64_t si) {
    static std::map<string, std::unique_ptr<Box>> cache;
    const string key = path + "|" + std::to_string(si);
    auto it = cache.find(key);
    if (it == cache.end()) {
        seg::Seg& base = seg_of(path, si);
        std::vector<int64_t> ks = base.funcs.keys();
        std::sort(ks.begin(), ks.end());
        py::dict<int64_t, py::str> ov;
        for (size_t i = 0; i < ks.size() && i < 8; ++i) ov.set(ks[i], py::str(ODD[i % 6]));
        auto b = std::make_unique<Box>();
        b->ne = std::make_unique<ne::NEFile>(py::str(path));
        b->sg = std::make_unique<seg::Seg>(*b->ne, si, ordinals(), ov);
        it = cache.emplace(key, std::move(b)).first;
    }
    return *it->second->sg;
}

static bool force_of(int64_t off) { return (off * 37 + 11) % 5 == 0; }

static string set_j(const py::set<int64_t>& s) {
    vector<int64_t> v;
    for (int64_t x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    vector<string> o;
    for (int64_t x : v) o.push_back(jint(x));
    return jarr(o);
}
static string set_d(const py::set<int64_t>& s) {
    vector<int64_t> v;
    for (int64_t x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    string all;
    for (size_t i = 0; i < v.size(); ++i) all += (i ? "," : "") + std::to_string(v[i]);
    return jarr({jint(static_cast<int64_t>(v.size())), jstr(sha1(all))});
}
static string strs(const py::set<py::str>& s) {
    vector<py::str> v;
    for (const py::str& x : s) v.push_back(x);
    std::sort(v.begin(), v.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    vector<string> o;
    for (const py::str& x : v) o.push_back(jstr(x.utf8()));
    return jarr(o);
}
static string res_d(const std::optional<sd::DiffResult>& r) {
    if (!r) return "null";
    return jarr({r->exact ? "true" : "false", r->length_match ? "true" : "false", jint(r->our_len), jint(r->shipped_len), jint(r->n_diff),
                 r->first_diff ? jint(*r->first_diff) : "null", jint(r->masked)});
}
static vector<string> split_commas(const string& s) {
    if (s == "~") return {};
    return split(s, ',');
}
// "[n lines, sha1]" of text split on \n
static string lines_d(const string& text) {
    int64_t n = 1;
    for (char c : text)
        if (c == '\n') n++;
    return jint(n) + "," + jstr(sha1(text));
}

// a listing / log without what changes from run to run
static string norm_asm_out(const py::str& text) {
    static const py::re::Pattern tmp = py::re::compile(py::str("ne_nasm_\\w{8}"));
    py::list<py::str> keep;
    for (const py::str& ln : py::splitlines(text))
        if (!ln.contains(py::str("Macro Assembler")) && !ln.contains(py::str("Page"))) keep.append(ln);
    return tmp.sub(py::str("ne_nasm_X"), py::join(py::str("\n"), keep)).utf8();
}
static string asm_d(const I::AsmResult& r) {
    string normed;
    if (r.obj) {
        static const py::re::Pattern tmpn = py::re::compile(py::str("ne_nasm_[0-9a-z_]{8}"));   // (nasm's object names its source: the temp folder, whose name is random)
        normed = tmpn.sub(py::str("ne_nasm_XXXXXXXX"), r.obj->decode("latin1")).encode("latin1").raw();
    }
    return jarr({r.obj ? jarr({jint(r.obj->size()), jstr(sha1(normed))}) : string("null"), jstr(sha1(norm_asm_out(r.lst))), jstr(sha1(norm_asm_out(r.log)))});
}

static string rec_d(const R::Reconciled& r) { return jarr({jstr(sha1(r.asm_.utf8())), res_d(r.res), set_j(r.force_db), jint(r.n_code)}); }
static string ver_d(const I::Verified& r) {
    return jarr({r.ok ? "true" : "false", jint(r.n), jint(r.n_code), jint(r.db), jint(r.live), jstr(sha1(r.asm_.utf8())), res_d(r.res)});
}

static std::optional<py::list<int64_t>> funcs_of(const seg::Seg& sg, int64_t lo, int64_t hi, int fmode) {
    if (fmode == 0) return std::nullopt;
    py::list<int64_t> out;
    if (fmode == 1) return out;
    std::vector<int64_t> fl = sg.funcs.keys();
    std::sort(fl.begin(), fl.end());
    std::vector<int64_t> inr;
    for (int64_t x : fl)
        if (lo <= x && x < hi) inr.push_back(x);
    for (int64_t x : inr) out.append(x);
    if (fmode == 2) out.append(lo + 3);
    else if (!inr.empty()) out.append(inr[0]);
    return out;
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    const string here = py::os::getcwd().utf8();
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") {
            vector<string> pre;
            for (const py::str& x : I::_PREAMBLE) pre.push_back(jstr(x.utf8()));
            return jarr({strs(I::_JMP), strs(I::_JCC), strs(I::_CALL), jarr(pre), jstr(I::_NLST.utf8())});
        }
        if (op == "branch") {
            const auto b = I::_branch(py::str(unhex(f[1])));
            if (!b) return "[null,null]";
            return jarr({jstr(std::get<0>(*b).utf8()), jint(std::get<1>(*b))});
        }
        if (op == "procname") {
            seg::Seg& sg = std::stoi(f[4]) ? ovseg(f[1], std::stoll(f[2])) : seg_of(f[1], std::stoll(f[2]));
            return jstr(I::_procname(sg, std::stoll(f[3])).utf8());
        }
        if (op == "emit") {
            seg::Seg& sg = seg_of(f[1], std::stoll(f[2]));
            const int64_t lo = std::stoll(f[3]), hi = std::stoll(f[4]);
            const int fmode = std::stoi(f[5]), mode = std::stoi(f[6]);
            py::set<int64_t> force, dat;
            if (mode == 1)
                for (const auto& oa : sg.rows)
                    if (lo <= std::get<0>(oa) && std::get<0>(oa) < hi && force_of(std::get<0>(oa))) force.add(std::get<0>(oa));
            if (mode == 2)
                for (const auto& d : R::_safe_data_spans(sg, lo, hi))
                    for (const auto& oa : sg.rows)
                        if (std::get<0>(d) <= std::get<0>(oa) && std::get<0>(oa) < std::get<1>(d)) dat.add(std::get<0>(oa));
            const I::Emitted e = I::_emit(sg, lo, hi, funcs_of(sg, lo, hi, fmode), 1, force, dat);
            return "[" + lines_d(e.text.utf8()) + "," + set_d(e.unresolved).substr(1);
        }
        if (op == "stage") {
            const py::str tc = I::_stage_cmacros(py::str("."));
            string tmp = py::os::getenv(py::str("TEMP"), py::str("")).utf8();
            string rel = tc.utf8();
            if (rel.compare(0, tmp.size(), tmp) == 0) rel = rel.substr(tmp.size() + 1);
            for (char& c : rel)
                if (c == '\\') c = '/';
            const py::str inc = py::os::path::join(tc, py::str("inc"));
            vector<py::str> names;
            for (const py::str& n : py::os::listdir(inc)) names.push_back(n);
            std::sort(names.begin(), names.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            vector<string> o;
            for (const py::str& n : names) o.push_back(jarr({jstr(n.utf8()), jint(py::os::path::getsize(py::os::path::join(inc, n)))}));
            return jarr({jstr(rel), jarr(o)});
        }
        if (op == "asm" || op == "nasm") {
            seg::Seg& sg = seg_of(f[1], std::stoll(f[2]));
            const int64_t lo = std::stoll(f[3]), hi = std::min<int64_t>(std::stoll(f[4]), sg.code.size());
            const py::str text = I::_emit(sg, lo, hi).text;
            return asm_d(op == "asm" ? I::assemble(text) : I::assemble_nasm(text));
        }
        if (op == "nasm_text") return asm_d(I::assemble_nasm(py::str(unhex(f[1]))));
        if (op == "nasm_missing") {
            const auto old = py::os::getenv(py::str("CB_NASM"));
            _putenv_s("CB_NASM", (here + "\\no\\such\\nasm.exe").c_str());
            string r;
            try {
                r = asm_d(I::assemble_nasm(py::str(unhex(f[1]))));
            } catch (...) {
                _putenv_s("CB_NASM", old ? old->utf8().c_str() : "");
                throw;
            }
            _putenv_s("CB_NASM", old ? old->utf8().c_str() : "");
            return r;
        }
        if (op == "nasm_bin") return jarr({jstr(py::os::path::basename(I::_nasm_bin()).utf8()), jstr(py::os::path::basename(I::_nasm_inc()).utf8())});
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
            const auto r = I::_nasm_divergers(py::str(lst), from_hex(our == "~" ? "" : our), omask, from_hex(shipped == "~" ? "" : shipped), smask, spans,
                                              std::stoll(start));
            return jarr({set_j(std::get<0>(r)), set_j(std::get<1>(r))});
        }
        if (op == "rec" || op == "ver") {
            seg::Seg& sg = seg_of(f[2], std::stoll(f[3]));
            const int64_t lo = std::stoll(f[4]), hi = std::stoll(f[5]);
            const py::str backend(f[1]);
            if (op == "ver") return ver_d(I::verify_range(sg, lo, hi, std::nullopt, py::str("."), backend));
            if (f.size() > 6) {
                const int v = std::stoi(f[6]);
                py::list<int64_t> fl;
                if (v == 1) {
                    fl.append(0);
                    fl.append(20);
                    return rec_d(I::reconcile_range(sg, lo, hi, fl, py::str("."), 60, backend));
                }
                if (v == 2) return rec_d(I::reconcile_range(sg, lo, hi, fl, py::str("."), 60, backend));
                return rec_d(I::reconcile_range(sg, lo, hi, std::nullopt, py::str("."), v == 3 ? 1 : 0, backend));
            }
            return rec_d(I::reconcile_range(sg, lo, hi, std::nullopt, py::str("."), 60, backend));
        }
        if (op == "main") {
            const string save = unhex(f[1]);
            py::list<py::str> args;
            for (size_t i = 2; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            const auto old = py::os::getenv(py::str("NE_SAVE"));
            _putenv_s("NE_SAVE", save.c_str());   // (empty: removed)
            std::ostringstream out, err;
            int64_t status = 0;
            try {
                status = I::main(args, out, err);
            } catch (...) {
                _putenv_s("NE_SAVE", old ? old->utf8().c_str() : "");
                throw;
            }
            _putenv_s("NE_SAVE", old ? old->utf8().c_str() : "");
            string text = out.str();
            const string state = py::os::getenv(py::str("CB_STATE"), py::str("")).utf8();
            for (size_t p = text.find(state); !state.empty() && p != string::npos; p = text.find(state, p + 8)) text.replace(p, state.size(), "<STATE>");
            string extra = "null";
            if (!save.empty()) {
                std::ifstream in(save, std::ios::binary);
                if (in) {
                    std::stringstream ss;
                    ss << in.rdbuf();
                    in.close();
                    extra = jstr(sha1(ss.str()));
                    std::remove(save.c_str());
                }
            }
            return jarr({jint(status), jstr(text), extra});
        }
        throw py::ValueError("bad case " + op);
    });
}
