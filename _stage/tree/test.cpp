// test.cpp -- the typed C++ codebreaker.source.vxd.tree on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <filesystem>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/tree.hpp"
#include "py_settings.hpp"

namespace T = codebreaker::source::vxd::tree;
namespace dis = codebreaker::source::vxd::disassembly;
namespace fs = std::filesystem;
using namespace vh;

static const string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }

static string lines_d(const py::list<py::str>& ls) {
    string all;
    for (int64_t i = 0; i < ls.size(); ++i) all += (i ? "\n" : "") + ls[i].utf8();
    return jarr({jint(ls.size()), jstr(sha1(all))});
}

// [[relative path, sha1]] of the files under `folder`, sorted (null when it is not a folder)
static string files_d(const string& folder) {
    if (folder.empty() || !fs::is_directory(folder)) return "null";
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

static py::str wasm_path() {
    const py::str ow = py::settings::get_str(py::str("openwatcom"));
    return py::os::path::join(py::os::path::dirname(ow), py::str("bld"), py::str("wasm"), py::str("nt386"), py::str("wasm.exe"));
}

static string probe_j(const T::ProbeResult& r) { return jarr({jint(std::get<0>(r)), jstr(std::get<1>(r).utf8()), jstr(std::get<2>(r).pattern().utf8())}); }

static dis::VxDDisasm fresh(const le::LEFile& v) {
    dis::VxDDisasm D(v);
    D.run();
    return D;
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    const string here = py::os::getcwd().utf8();
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        const py::str nasm = py::settings::get_str(py::str("nasm"));
        if (op == "seg_class") {
            const auto r = T::seg_class(std::stoll(f[1]));
            return jarr({jstr(std::get<0>(r).utf8()), jstr(std::get<1>(r).utf8())});
        }
        if (op == "entrytab") {
            const auto r = T::_entrytab_ddb(load_spec(f[1]));
            return r ? jarr({jint(std::get<0>(*r)), jint(std::get<1>(*r))}) : string("null");
        }
        if (op == "nprobe") return probe_j(T::_nasm_probe(nasm)(py::str(f[1])));
        if (op == "wprobe") return probe_j(T::_wasm_probe(wasm_path())(py::str(f[1])));
        if (op == "probe_missing") {
            const py::str missing(here + "\\no\\such\\" + (f[1] == "nasm" ? "nasm.exe" : "wasm.exe"));
            const T::Probe p = f[1] == "nasm" ? T::_nasm_probe(missing) : T::_wasm_probe(missing);
            const auto r = p(py::str("inputs/probe_0.asm"));
            return jarr({jint(std::get<0>(r)), jstr(std::get<1>(r).utf8())});
        }
        if (op == "conv") {
            const le::LEFile v = load_spec(f[1]);
            const dis::VxDDisasm D = fresh(v);
            const int64_t oi = std::stoll(f[2]);
            const le::Object* o = nullptr;
            for (const le::Object& x : v.objects)
                if (x.idx == oi) o = &x;
            if (!o) throw py::KeyError("object");
            const auto cs = T::seg_class(o->flags);
            const py::str name = f[1].compare(0, 2, "B:") == 0 ? py::str("DMG") : py::str("");
            (void)name;
            py::dict<int64_t, py::str> extra;
            if (D.ddb && D.ddb->obj == oi) extra.set(D.ddb->rva, py::str("VXD_DDB"));
            // (the name in the header lines is the case's own: expected.py passes the driver's file name)
            const py::str fn = py::str(f[5]);
            const py::list<py::str> head = {py::percent(py::str("; x.asm -- %s object %d segment %s"), fn, oi, std::get<1>(cs)), py::str(".386p"),
                                            py::percent(py::str("%s segment dword public use32 '%s'"), std::get<1>(cs), std::get<0>(cs)), py::str("")};
            const py::list<py::str> tail = {py::percent(py::str("%s ends"), std::get<1>(cs)), py::str("        end"), py::str("")};
            const T::Probe probe = f[3] == "nasm" ? T::_nasm_probe(nasm) : T::_wasm_probe(wasm_path());
            return lines_d(T::_converge_buildable(D, oi, 0, D.code.at(oi).size(), extra, head, tail, probe, std::stoll(f[4])));
        }
        if (op == "tree") {
            const le::LEFile v = load_spec(f[1]);
            const string& mode = f[2];
            const int64_t maxins = std::stoll(f[3]);
            const string& folder = f[4];
            const py::str name(f[5]);
            std::optional<codebreaker::source::vxd::link::ForceRaw> fr;
            if (mode == "force") {
                const dis::VxDDisasm D = fresh(v);
                codebreaker::source::vxd::link::ForceRaw m;
                for (const auto& k : D.insns.keys())
                    if ((std::get<1>(k) * 31 + 7) % 5 == 0) {
                        if (!m.contains(std::get<0>(k))) m.set(std::get<0>(k), py::set<int64_t>());
                        m.at(std::get<0>(k)).add(std::get<1>(k));
                    }
                fr = m;
            }
            const py::list<py::str> files = T::emit_tree(v, name, py::str(folder), maxins, mode == "nasm" ? std::optional<py::str>(nasm) : std::nullopt,
                                                         mode == "wasm" ? std::optional<py::str>(wasm_path()) : std::nullopt, fr);
            vector<string> fl;
            for (const py::str& s : files) fl.push_back(jstr(s.utf8()));
            return jarr({jarr(fl), files_d(folder)});
        }
        if (op == "main") {
            const string out_dir = unhex(f[1]);
            py::list<py::str> args;
            for (size_t i = 2; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = T::main(args, out, err);
            string state = py::settings::get_str(py::str("state")).utf8();
            string text = out.str();
            for (size_t p = text.find(state); !state.empty() && p != string::npos; p = text.find(state, p + 7)) text.replace(p, state.size(), "<STATE>");
            string folder = out_dir;
            if (folder.compare(0, 7, "<STATE>") == 0) folder = state + folder.substr(7);
            return jarr({jint(status), jstr(text), files_d(folder)});
        }
        throw py::ValueError("bad case " + op);
    });
}
