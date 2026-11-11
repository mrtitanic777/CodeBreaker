// test.cpp -- the typed C++ codebreaker.source.vxd.corpus on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/source/vxd/corpus.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "py_settings.hpp"

namespace C = codebreaker::source::vxd::corpus;
namespace dis = codebreaker::source::vxd::disassembly;
using namespace vh;

static string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }
static string b2s(bool b) { return b ? "true" : "false"; }
static string opt_i(const std::optional<int64_t>& v) { return v ? jint(*v) : string("null"); }

// the entry as json.dumps writes it (compact), its seconds as 0
static string entry_j(const C::Entry& e) {
    if (e.record) {
        const C::ModuleRecord& r = *e.record;
        vector<string> objs;
        for (const C::ObjectRow& x : r.objects)
            objs.push_back("{\"obj\":" + opt_i(x.obj) + ",\"match\":" + b2s(x.match) + ",\"ok\":" + b2s(x.ok) + ",\"len\":" + opt_i(x.len) + ",\"symbolic\":" + opt_i(x.symbolic) +
                           ",\"forced_raw\":" + opt_i(x.forced_raw) + ",\"raw_fallback\":" + b2s(x.raw_fallback) + ",\"diffs\":" + opt_i(x.diffs) + ",\"err\":" +
                           (x.err ? jstr(x.err->utf8()) : string("null")) + "}");
        return "{\"byte_exact\":" + b2s(r.byte_exact) + ",\"objects\":" + jarr(objs) + ",\"n_obj\":" + jint(r.n_obj) + ",\"n_exact\":" + jint(r.n_exact) + ",\"symbolic\":" +
               jint(r.symbolic) + ",\"forced_raw\":" + jint(r.forced_raw) + ",\"raw_fallback_objs\":" + jint(r.raw_fallback_objs) + ",\"seconds\":0}";
    }
    string o = "{\"error\":" + jstr(e.error ? e.error->utf8() : string());
    if (e.trace) o += ",\"trace\":" + jstr(e.trace->utf8());
    return o + "}";
}

static string results_j(const C::Results& r) {
    string o = "{";
    bool first = true;
    for (const auto& kv : r.items()) {
        o += (first ? "" : ",") + jstr(std::get<0>(kv).utf8()) + ":" + entry_j(std::get<1>(kv));
        first = false;
    }
    return o + "}";
}

static string cwd_;

static string norm_text(string s) {
    static const py::re::Pattern sec = py::re::compile(py::str("\"seconds\": [0-9.]+"));
    static const py::re::Pattern br = py::re::compile(py::str("\\[\\d+s\\]"));
    static const py::re::Pattern eq = py::re::compile(py::str("\\d+s =="));
    py::str t = py::str(s);
    t = sec.sub(py::str("\"seconds\": 0"), t);
    t = br.sub(py::str("[Ns]"), t);
    t = eq.sub(py::str("Ns =="), t);
    const string state = py::settings::get_str(py::str("state")).utf8();
    string u = t.utf8();
    for (size_t p = u.find(state); !state.empty() && p != string::npos; p = u.find(state, p + 7)) u.replace(p, state.size(), "<STATE>");
    for (size_t p = u.find(cwd_); p != string::npos; p = u.find(cwd_, p + 3)) u.replace(p, cwd_.size(), "<H>");
    return u;
}

static string file_text(const string& path) {
    if (!std::filesystem::exists(path)) return "null";
    return jstr(norm_text(read_file(path)));
}

static string run_case(const string& scen) {
    const string out_json = "runs/" + scen + ".json";
    std::filesystem::path p0;
    const string base = scen == "resume" || scen == "redo" ? "fresh" : scen == "seeded_redo" ? "seeded" : "";
    if (!base.empty()) std::filesystem::copy_file("runs/" + base + ".json", out_json, std::filesystem::copy_options::overwrite_existing);
    if (scen == "seeded") {
        std::ofstream f(out_json, std::ios::binary);
        f << "{\r\n \"splitter\": {\r\n  \"byte_exact\": true,\r\n  \"objects\": [],\r\n  \"n_obj\": 0,\r\n  \"n_exact\": 0,\r\n  \"symbolic\": 0,\r\n  \"forced_raw\": 0,\r\n"
             "  \"raw_fallback_objs\": 0,\r\n  \"seconds\": 1.5\r\n },\r\n \"vsd\": {\r\n  \"error\": \"boom\",\r\n  \"trace\": \"tb\"\r\n },\r\n \"other\": {\r\n  \"error\": \"old\"\r\n }\r\n}";
    }
    std::optional<py::list<py::str>> redo;
    if (scen == "redo") redo = py::list<py::str>{py::str("splitter")};
    if (scen == "seeded_redo") redo = py::list<py::str>{py::str("splitter"), py::str("other"), py::str("vsd")};
    std::ostringstream out;
    const C::Results res = C::run(out, py::str(cwd_ + "\\inputs\\mini"), py::str(out_json), redo, 1);
    return jarr({jstr(norm_text(out.str())), file_text(out_json), results_j(res)});
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    cwd_ = py::os::getcwd().utf8();
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") {
            vector<string> a, b;
            for (const py::str& s : C::_VXD_EXTS) a.push_back(jstr(s.utf8()));
            for (const py::str& s : C::_EXTRA_FILES) b.push_back(jstr(s.utf8()));
            return jarr({jarr(a), jarr(b), "true"});
        }
        if (op == "is_le") return b2s(C::_is_le_vxd(py::str(cwd_ + "\\" + f[1])));
        if (op == "one") {
            const le::LEFile v = load_spec(f[1]);
            return entry_j(C::_one_module(py::str("x"), v));
        }
        if (op == "run") return run_case(f[1]);
        if (op == "save") {
            C::Results res;
            C::Entry a;
            C::ModuleRecord m;
            C::ObjectRow o;
            o.obj = 1;
            o.match = true;
            o.ok = true;
            o.len = 10;
            o.symbolic = 2;
            o.forced_raw = 0;
            o.raw_fallback = false;
            o.diffs = 0;
            m.byte_exact = true;
            m.objects.append(o);
            m.n_obj = 1;
            m.n_exact = 1;
            m.symbolic = 2;
            m.seconds = 0.25;
            a.record = m;
            res.set(py::str("a"), a);
            C::Entry b;
            b.error = py::str("\xc3\xa9\xe4\xb8\xad");
            b.trace = py::str("line1\nline2");
            res.set(py::str("b\xc3\xa9"), b);
            C::Entry c;
            C::ModuleRecord m2;
            m2.forced_raw = 1;
            m2.raw_fallback_objs = 1;
            m2.seconds = 3.0;
            c.record = m2;
            res.set(py::str("c"), c);
            C::_save(res, py::str("runs/save.json"));
            std::ostringstream out;
            C::_summary(out, res, 12.4);
            C::_summary(out, C::Results(), 0.0);
            return jarr({file_text("runs/save.json"), jstr(norm_text(out.str())), b2s(std::filesystem::exists("runs/save.json.tmp"))});
        }
        if (op == "main") {
            py::list<py::str> args;
            for (size_t i = 1; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = C::main(args, out, err);
            string outj = "null";
            for (const py::str& a : args)
                if (a.endswith(py::str(".json"))) {
                    outj = file_text(a.utf8());
                    break;
                }
            return jarr({jint(status), jstr(norm_text(out.str())), outj});
        }
        throw py::ValueError("bad case " + op);
    });
}
