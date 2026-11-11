// test.cpp -- the typed C++ codebreaker.source.ne_asm.sweep on the cases of expected.py (cases.txt, names_*.json); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>

#include "../_ne_common/necommon.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/source/ne_asm/sweep.hpp"

namespace S = codebreaker::source::ne_asm::sweep;
using namespace nh;
namespace fs = std::filesystem;

static string lower(string s) {
    for (char& c : s) {
        if (c == '/') c = '\\';
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

static string rec_j(const S::ModuleRecord& r) {
    vector<string> segs;
    for (const S::SegRecord& s : r.segs)
        segs.push_back("{\"seg\":" + jint(s.seg) + ",\"n\":" + jint(s.n) + ",\"ok\":" + (s.ok ? "true" : "false") + ",\"n_code\":" + jint(s.n_code) +
                       ",\"db\":" + jint(s.db) + ",\"live\":" + jint(s.live) + "}");
    return "{\"module\":" + jstr(r.module.utf8()) + ",\"segs\":" + jarr(segs) + ",\"ok\":" + (r.ok ? "true" : "false") + ",\"nseg\":" + jint(r.nseg) +
           ",\"nok\":" + jint(r.nok) + ",\"code\":" + jint(r.code) + ",\"live\":" + jint(r.live) + "}";
}

static string summary_j(const py::list<S::Summary>& l) {
    vector<string> o;
    for (const S::Summary& s : l) {
        if (s.record) o.push_back(rec_j(*s.record));
        else o.push_back("{\"module\":" + jstr(s.module.utf8()) + ",\"error\":" + jstr(s.error->utf8()) + "}");
    }
    return jarr(o);
}

// [[relative path, sha1]] of the files under `folder`, sorted ("null" when it is not a folder)
static string files_d(const string& folder) {
    if (!fs::is_directory(folder)) return "null";
    vector<std::pair<string, string>> v;
    for (const auto& e : fs::recursive_directory_iterator(folder)) {
        if (!e.is_regular_file()) continue;
        string rel = fs::relative(e.path(), folder).string();
        for (char& c : rel)
            if (c == '\\') c = '/';
        v.emplace_back(rel, sha1(read_file(e.path().string())));
    }
    std::sort(v.begin(), v.end());
    vector<string> o;
    for (const auto& kv : v) o.push_back(jarr({jstr(kv.first), jstr(kv.second)}));
    return jarr(o);
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    const string here = py::os::getcwd().utf8();
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        py::os::chdir(py::str(here));   // (a case may change the working folder)
        const string& op = f[0];
        if (op == "consts") return jarr({lower(S::ROOT().utf8()) == lower("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker") ? "true" : "false"});
        if (op == "setup") {
            S::_setup();
            return lower(py::os::getcwd().utf8()) == lower(S::ROOT().utf8()) ? "true" : "false";
        }
        if (op == "ne_modules") {
            const auto mods = S::ne_modules();
            const bool ok = lower(py::os::getcwd().utf8()) == lower(S::ROOT().utf8());
            const py::str croot = codebreaker::base::corpus::root();
            string all;
            vector<string> first;
            for (int64_t i = 0; i < mods.size(); ++i) {
                all += (i ? "\n" : "") + std::get<0>(mods[i]).utf8() + "|" + std::get<1>(mods[i]).replace(croot, py::str("<C>")).utf8();
                if (i < 6) first.push_back(jstr(std::get<0>(mods[i]).utf8()));
            }
            return jarr({jint(mods.size()), jstr(sha1(all)), jarr(first), ok ? "true" : "false"});
        }
        if (op == "resolve") {
            const string nm = unhex(f[1]);
            const py::str r = S::_resolve(py::str(nm));
            if (r.utf8() == nm) return jstr("same");
            string b = py::os::path::basename(r).utf8();
            for (char& c : b) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            return jstr(b);
        }
        if (op == "module") {
            const string emit = f[4];
            const S::ModuleRecord r = S::sweep_module(py::str(unhex(f[1])), py::str(f[2]), py::str(f[3]), emit == "~" ? std::nullopt : std::optional<py::str>(py::str(emit)));
            const string rj = rec_j(r);
            return jarr({rj, emit == "~" ? string("null") : files_d(emit)});
        }
        if (op == "sweep") {
            const int64_t start = std::stoll(f[1]);
            const std::optional<int64_t> limit = f[2] == "-" ? std::nullopt : std::optional<int64_t>(std::stoll(f[2]));
            const string emit = f[3];
            std::ostringstream out;
            const auto summ = S::sweep(out, limit, start, emit == "~" ? std::nullopt : std::optional<py::str>(py::str(emit)), py::str(f[4]));
            string led = "null";
            if (emit != "~" && fs::exists(emit + "/_ne_idiom_summary.json")) led = jstr(sha1(read_file(emit + "/_ne_idiom_summary.json")));
            return jarr({jstr(sha1(out.str())), summary_j(summ), led, emit == "~" ? string("null") : files_d(emit)});
        }
        if (op == "main") {
            py::list<py::str> args;
            for (size_t i = 1; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = S::main(args, out, err);
            return jarr({jint(status), status == 0 ? jstr(sha1(out.str())) : string("null")});
        }
        throw py::ValueError("bad case " + op);
    });
}
