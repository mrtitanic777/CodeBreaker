// test.cpp -- the typed C++ codebreaker.source.vxd.owbuild on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <filesystem>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/owbuild.hpp"
#include "py_settings.hpp"

namespace O = codebreaker::source::vxd::owbuild;
namespace dis = codebreaker::source::vxd::disassembly;
namespace fs = std::filesystem;
using namespace vh;

static string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }

static string files_d(const string& folder) {
    if (folder.empty() || !fs::is_directory(folder)) return "null";
    vector<std::pair<string, string>> v;
    for (const auto& e : fs::recursive_directory_iterator(folder)) {
        if (!e.is_regular_file()) continue;
        string rel = fs::relative(e.path(), folder).string();
        for (char& c : rel)
            if (c == '\\') c = '/';
        string data = read_file(e.path().string());
        if (rel.size() > 4 && rel.compare(rel.size() - 4, 4, ".obj") == 0 && data.size() >= 8) data.replace(4, 4, string(4, '\0'));   // (a COFF object holds the time it was made)
        v.emplace_back(rel, sha1(data));
    }
    std::sort(v.begin(), v.end());
    vector<string> o;
    for (const auto& kv : v) o.push_back(jarr({jstr(kv.first), jstr(kv.second)}));
    return jarr(o);
}

// os.path.relpath(p, base) of a path under base, with "/"
static string rel(const py::str& p, const py::str& base) {
    string s = p.utf8();
    const string b = base.utf8();
    if (s.compare(0, b.size(), b) == 0) s = s.substr(b.size() + (s.size() > b.size() ? 1 : 0));
    for (char& c : s)
        if (c == '\\') c = '/';
    return s;
}

static string opt_s(const std::optional<py::str>& s) { return s ? jstr(s->utf8()) : string("null"); }

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") return jarr({jstr(rel(O::_WASM(), O::_OW())), jstr(rel(O::_WLINK(), O::_OW())), jstr(py::os::path::basename(O::_NASM()).utf8())});
        if (op == "env") {
            const auto e = O::_env();
            const auto cur = py::os::environ_::copy();
            const string path = e.at(py::str("PATH")).utf8();
            const size_t semi = path.find(';');
            bool keep = true;
            for (const auto& kv : cur.items()) {
                const py::str& k = std::get<0>(kv);
                if (k == py::str("PATH") || k == py::str("WATCOM")) continue;
                if (!e.contains(k) || e.at(k) != std::get<1>(kv)) keep = false;
            }
            const std::optional<py::str> oldp = cur.contains(py::str("PATH")) ? std::optional<py::str>(cur.at(py::str("PATH"))) : std::nullopt;
            return jarr({jstr(rel(e.at(py::str("WATCOM")), O::_OW())), jstr(rel(py::str(path.substr(0, semi)), O::_OW())), keep ? "true" : "false",
                         path.substr(semi + 1) == (oldp ? oldp->utf8() : string()) ? "true" : "false"});
        }
        if (op == "w3comp") {
            const auto r = O::_w3_components(py::bytes(read_file(f[1])));
            vector<string> recs;
            for (const auto& x : std::get<1>(r)) recs.push_back(jarr({jstr(std::get<0>(x).utf8()), jint(std::get<1>(x)), jint(std::get<2>(x))}));
            return jarr({std::get<0>(r) ? jint(*std::get<0>(r)) : string("null"), jarr(recs)});
        }
        if (op == "build") {
            const string& folder = f[4];
            const O::BuildResult r = O::build_vxd(py::str(f[1]), py::str(unhex(f[2])), py::str(folder), f[3] == "1");
            const string sha = (r.ok && r.output && py::os::path::exists(*r.output)) ? jstr(sha1(read_file(r.output->utf8()))) : string("null");
            return jarr({r.ok ? "true" : "false", r.output ? jstr(rel(*r.output, py::str(folder))) : string("null"), opt_s(r.error), r.ok ? jint(r.bytes) : string("null"),
                         r.ok ? jint(r.objects) : string("null"), sha});
        }
        if (op == "build_w3") {
            const string& folder = f[2];
            const O::W3Result r = O::build_w3(py::str(f[1]), py::str(folder));
            string comps = "null";
            if (r.ok) {
                vector<string> c;
                for (const py::str& n : r.components) c.push_back(jstr(n.utf8()));
                comps = jarr(c);
            } else if (!r.status.empty()) {
                vector<string> c;
                for (const auto& s : r.status) c.push_back(jarr({jstr(std::get<0>(s).utf8()), std::get<1>(s) ? (*std::get<1>(s) ? "true" : "false") : "null"}));
                comps = jarr(c);
            }
            const string sha = (r.ok && r.output && py::os::path::exists(*r.output)) ? jstr(sha1(read_file(r.output->utf8()))) : string("null");
            return jarr({r.ok ? "true" : "false", opt_s(r.error), r.output ? jstr(rel(*r.output, py::str(folder))) : string("null"), r.ok ? jint(r.bytes) : string("null"), comps, sha});
        }
        if (op == "main") {
            const string out_dir = unhex(f[1]);
            py::list<py::str> args;
            for (size_t i = 2; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = O::main(args, out, err);
            const string state = py::settings::get_str(py::str("state")).utf8();
            string text = out.str();
            for (size_t p = text.find(state); !state.empty() && p != string::npos; p = text.find(state, p + 7)) text.replace(p, state.size(), "<STATE>");
            return jarr({jint(status), jstr(status == 0 ? text : string()), files_d(out_dir)});
        }
        throw py::ValueError("bad case " + op);
    });
}
