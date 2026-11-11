// test.cpp -- the typed C++ codebreaker.source.vxd.loadable_corpus on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/loadable_corpus.hpp"
#include "py_settings.hpp"

namespace C = codebreaker::source::vxd::loadable_corpus;
namespace dis = codebreaker::source::vxd::disassembly;
using namespace vh;

static string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }
static string b2s(bool b) { return b ? "true" : "false"; }
static string stage_j(const std::optional<py::str>& s) { return s ? jstr(s->utf8()) : string("null"); }

// the entry as json.dumps writes it (compact), its seconds as 0
static string entry_j(const C::Entry& e) {
    string o = "{\"ok\":" + b2s(e.ok);
    auto kv = [&](const char* k, const string& v) { o += string(",\"") + k + "\":" + v; };
    switch (e.kind) {
        case C::Entry::Kind::Built:
            kv("stage", stage_j(e.stage));
            kv("seconds", "0");
            kv("objects", jint(e.objects));
            kv("device_id", jint(e.device_id));
            kv("device_id_ok", b2s(e.device_id_ok));
            kv("identity", b2s(e.identity));
            kv("resource_len", jint(e.resource_len));
            kv("vxd_len", jint(e.vxd_len));
            break;
        case C::Entry::Kind::Failed:
            kv("stage", stage_j(e.stage));
            kv("seconds", "0");
            kv("log", jstr(e.log.utf8()));
            break;
        case C::Entry::Kind::Exception:
            kv("stage", stage_j(e.stage));
            kv("log", jstr(e.log.utf8()));
            kv("trace", jstr(e.trace.utf8()));
            kv("seconds", "0");
            break;
        case C::Entry::Kind::Open:
            kv("stage", stage_j(e.stage));
            kv("log", jstr(e.log.utf8()));
            break;
    }
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
    static const py::re::Pattern br = py::re::compile(py::str("\\[[0-9.]+s\\]"));
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
    const string base = scen == "resume" || scen == "redo" ? "fresh" : scen == "seeded_redo" ? "seeded" : "";
    if (!base.empty()) std::filesystem::copy_file("runs/" + base + ".json", out_json, std::filesystem::copy_options::overwrite_existing);
    if (scen == "seeded") std::filesystem::copy_file("inputs/seeded.json", out_json, std::filesystem::copy_options::overwrite_existing);
    std::optional<py::list<py::str>> redo;
    if (scen == "redo") redo = py::list<py::str>{py::str("splitter")};
    if (scen == "seeded_redo") redo = py::list<py::str>{py::str("splitter"), py::str("failed1"), py::str("vsd")};
    std::ostringstream out;
    const C::Results res = C::run(out, py::str(cwd_ + "\\inputs\\mini"), py::str(out_json), redo, 1, py::str("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker"));
    return jarr({jstr(norm_text(out.str())), file_text(out_json), results_j(res)});
}

static C::Entry mk(C::Entry::Kind k, bool ok, const char* stage, double secs, int64_t objs, int64_t dev, bool dev_ok, bool ident, int64_t res, int64_t len, const char* log,
                   const char* trace) {
    C::Entry e;
    e.kind = k;
    e.ok = ok;
    if (stage) e.stage = py::str(stage);
    e.seconds = secs;
    e.objects = objs;
    e.device_id = dev;
    e.device_id_ok = dev_ok;
    e.identity = ident;
    e.resource_len = res;
    e.vxd_len = len;
    e.log = py::str(log);
    e.trace = py::str(trace);
    return e;
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    cwd_ = py::os::getcwd().utf8();
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") return jarr({"true"});
        if (op == "one") {
            const le::LEFile v = load_spec(f[1]);
            return entry_j(C::_one(py::str("x"), v, py::str("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker")));
        }
        if (op == "run") return run_case(f[1]);
        if (op == "save") {
            C::Results res;
            res.set(py::str("a"), mk(C::Entry::Kind::Built, true, nullptr, 0.25, 3, 12, true, false, 9, 4096, "", ""));
            res.set(py::str("b\xc3\xa9"), mk(C::Entry::Kind::Exception, false, "exception", 3.0, 0, 0, false, false, 0, 0, "\xc3\xa9\xe4\xb8\xad", "line1\nline2"));
            res.set(py::str("c"), mk(C::Entry::Kind::Open, false, "open", 0, 0, 0, false, false, 0, 0, "bad", ""));
            res.set(py::str("d"), mk(C::Entry::Kind::Failed, false, nullptr, 1.0, 0, 0, false, false, 0, 0, "L", ""));
            res.set(py::str("e"), mk(C::Entry::Kind::Exception, false, "exception", 0.0, 0, 0, false, false, 0, 0, "y", "t"));
            res.set(py::str("f"), mk(C::Entry::Kind::Built, true, nullptr, 2.0, 1, 1, false, true, 0, 1, "", ""));
            C::_save(res, py::str("runs/save.json"));
            std::ostringstream out;
            C::_summary(out, res, 12.4);
            C::_summary(out, C::Results(), 0.0);
            C::Results one;
            one.set(py::str("x"), mk(C::Entry::Kind::Built, true, nullptr, 0, 0, 0, true, true, 0, 0, "", ""));
            C::_summary(out, one, 1.0);
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
