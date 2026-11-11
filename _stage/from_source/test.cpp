// test.cpp -- the typed C++ codebreaker.source.vxd.from_source on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <cstdio>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/from_source.hpp"
#include "codebreaker/source/vxd/le_writer.hpp"
#include "py_stdlib.hpp"
#include "py_settings.hpp"

namespace F = codebreaker::source::vxd::from_source;
namespace dis = codebreaker::source::vxd::disassembly;
namespace lw = codebreaker::source::vxd::le_writer;
using namespace vh;

static string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }

static dis::VxDDisasm fresh(const le::LEFile& v) {
    dis::VxDDisasm D(v);
    D.run();
    return D;
}

static string strs(const py::list<py::str>& l) {
    vector<string> o;
    for (const py::str& s : l) o.push_back(jstr(s.utf8()));
    return jarr(o);
}
static string opt_i(const std::optional<int64_t>& v) { return v ? jint(*v) : string("null"); }
static string b2s(bool b) { return b ? "true" : "false"; }

// a log without temp folder names, hashed (the Python's re.sub(r"cb_idiom_\w+", ...))
static string norm_sha(const py::str& s) {
    static const py::re::Pattern p = py::re::compile(py::str("cb_idiom_\\w+"));
    return jstr(sha1(p.sub(py::str("cb_idiom_X"), s).utf8()));
}

static string result_d(const F::Built& r) {
    return jarr({b2s(r.ok), jint(r.vxd_len), jint(r.oracle_len), jint(r.objects), b2s(r.page_map_identity), opt_i(r.ddb_rva), opt_i(r.oracle_ddb_rva), jint(r.device_id),
                 b2s(r.device_id_ok), jint(r.resource_len), jint(r.obj1_diffs), jstr(sha1(r.vxd)), r.asm_ ? jstr(sha1(r.asm_->utf8())) : string("null"),
                 r.def_ ? jstr(sha1(r.def_->utf8())) : string("null"), norm_sha(r.link_log), norm_sha(r.res_log), r.compile_ok ? b2s(*r.compile_ok) : string("null"),
                 r.link_ok ? b2s(*r.link_ok) : string("null"), "null"});
}

static py::bytes built_bytes(const dis::VxDDisasm& D, const le::LEFile& v) { return lw::build_le(D, v, D.ddb ? D.ddb->name : py::str("DEVICE")); }

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    const py::str ROOT("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") {
            vector<py::str> rs;
            for (const py::str& s : F::_MASM_RESERVED) rs.push_back(s);
            std::sort(rs.begin(), rs.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            vector<string> r;
            for (const py::str& s : rs) r.push_back(jstr(s.utf8()));
            return jarr({jstr(F::_RC_DIR.utf8()), strs(F::_ADRC_REF), strs(F::_INC16_REF), jarr(r)});
        }
        if (op == "seg_name") return jstr(F::_seg_name(py::str(unhex(f[1]))).utf8());
        if (op == "object_segs" || op == "realseg_def_names") {
            py::list<py::str> tags;
            if (f[1] != "-")
                for (const string& t : split(f[1], ',')) tags.append(py::str(unhex(t)));
            const int64_t ddb = std::stoll(f[2]);
            if (op == "realseg_def_names") return jstr(F::realseg_def(py::str("NAME"), tags, ddb).utf8());
            const auto segs = F::_object_segs(tags, ddb);
            vector<int64_t> ks = segs.keys();
            std::sort(ks.begin(), ks.end());
            vector<string> o;
            for (int64_t k : ks) o.push_back(jarr({jint(k), jstr(std::get<0>(segs.at(k)).utf8()), jstr(std::get<1>(segs.at(k)).utf8())}));
            return jarr(o);
        }
        if (op == "object_names") return strs(F::object_names(load_spec(f[1])));
        if (op == "reclass") {
            const le::LEFile v = load_spec(f[1]);
            dis::VxDDisasm D = fresh(v);
            const auto t = F::_reclassify_ddb_tail_as_data(D, v);
            vector<std::pair<int64_t, int64_t>> ks;
            for (const auto& k : D.insns.keys()) ks.emplace_back(std::get<0>(k), std::get<1>(k));
            std::sort(ks.begin(), ks.end());
            string all;
            for (size_t i = 0; i < ks.size(); ++i) all += (i ? "," : "") + std::to_string(ks[i].first) + ":" + std::to_string(ks[i].second);
            vector<std::pair<std::pair<int64_t, int64_t>, string>> ls;
            for (const auto& k : D.labels.keys()) ls.push_back({{std::get<0>(k), std::get<1>(k)}, D.labels.at(k).utf8()});
            std::sort(ls.begin(), ls.end());
            string lall;
            for (size_t i = 0; i < ls.size(); ++i) lall += (i ? "," : "") + std::to_string(ls[i].first.first) + ":" + std::to_string(ls[i].first.second) + "=" + ls[i].second;
            return jarr({opt_i(t), jarr({jint(static_cast<int64_t>(ks.size())), jstr(sha1(all))}), jarr({jint(static_cast<int64_t>(ls.size())), jstr(sha1(lall))})});
        }
        if (op == "realseg") {
            const le::LEFile v = load_spec(f[1]);
            dis::VxDDisasm D = fresh(v);
            const auto names = F::object_names(v);
            const auto t = F::_reclassify_ddb_tail_as_data(D, v);
            const py::str a = F::realseg_source(D, v, names, t);
            const py::str d = F::realseg_def(D.ddb ? D.ddb->name : py::str("N"), names, D.ddb ? D.ddb->obj : 1);
            return jarr({jint(py::len(a)), jstr(sha1(a.utf8())), jint(py::len(d)), jstr(sha1(d.utf8()))});
        }
        if (op == "ver") {
            const auto d = F::ver_strings(py::bytes(read_file(f[1])));
            string o = "{";
            bool first = true;
            for (const auto& kv : d.items()) {
                o += (first ? "" : ",") + jstr(std::get<0>(kv).utf8()) + ":" + jstr(std::get<1>(kv).utf8());
                first = false;
            }
            return o + "}";
        }
        if (op == "vrc") {
            const le::LEFile v = load_spec(f[1]);
            const dis::VxDDisasm D = fresh(v);
            const int64_t woff = py::unpack_from1<int64_t>("<I", v.d(), v.lfa + 0xb8);
            const int64_t wlen = py::unpack_from1<int64_t>("<I", v.d(), v.lfa + 0xbc);
            return jstr(F::version_rc(D, v.d().slice(woff, woff + wlen)).utf8());
        }
        if (op == "stage") {
            const auto td = F::_stage_res_tools(ROOT);
            vector<string> both;
            for (const py::str& d : {std::get<0>(td), std::get<1>(td)}) {
                vector<py::str> names;
                for (const py::str& n : py::os::listdir(d)) names.push_back(n);
                std::sort(names.begin(), names.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
                vector<string> o;
                for (const py::str& n : names) o.push_back(jarr({jstr(n.utf8()), jint(py::os::path::getsize(py::os::path::join(d, n)))}));
                both.push_back(jarr(o));
            }
            return jarr(both);
        }
        if (op == "result") {
            const le::LEFile v = load_spec(f[1]);
            const dis::VxDDisasm D = fresh(v);
            return result_d(F::_result(built_bytes(D, v), v, D));
        }
        if (op == "embed") {
            const le::LEFile v = load_spec(f[1]);
            const dis::VxDDisasm D = fresh(v);
            const auto r = F::embed_version_resource(built_bytes(D, v), D, ROOT);
            const auto& out = std::get<0>(r);
            return jarr({out ? jarr({jint(out->size()), jstr(sha1(*out))}) : string("null"), norm_sha(std::get<1>(r)), jint(py::len(std::get<1>(r)))});
        }
        if (op == "build" || op == "build_named") {
            const le::LEFile v = load_spec(f[1]);
            return result_d(F::build_from_source(v, op == "build_named" ? std::optional<py::str>(py::str("MYNAME")) : std::nullopt, ROOT));
        }
        if (op == "main") {
            const string out_file = unhex(f[1]);
            py::list<py::str> args;
            for (size_t i = 2; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = F::main(args, out, err);
            string extra = "null";
            if (!out_file.empty()) {
                std::ifstream in(out_file, std::ios::binary);
                if (in) {
                    std::stringstream ss;
                    ss << in.rdbuf();
                    in.close();
                    extra = jstr(sha1(ss.str()));
                    std::remove(out_file.c_str());
                }
            }
            return jarr({jint(status), jstr(out.str()), extra});
        }
        throw py::ValueError("bad case " + op);
    });
}
