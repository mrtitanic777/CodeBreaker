// test.cpp -- the typed C++ codebreaker.source.ne_asm.reachability on the cases of expected.py (cases.txt); prints expected.txt's lines.
#include <algorithm>

#include "../_ne_common/necommon.hpp"
#include "codebreaker/source/ne_asm/reachability.hpp"

namespace R = codebreaker::source::ne_asm::reachability;
using namespace nh;

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

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    return run_cases(argc, argv, [](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") return jarr({strs(R::JMPCC), strs(R::NOFALL)});
        if (op == "resolve") {
            const string nm = unhex(f[1]);
            const py::str r = R::_target().resolve(py::str(nm));
            if (r.utf8() == nm) return jstr("same");
            string b = py::os::path::basename(r).utf8();
            for (char& c : b) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            return jstr(b);
        }
        if (op == "main") {
            py::list<py::str> args;
            for (size_t i = 1; i < f.size(); ++i) args.append(py::str(unhex(f[i])));
            std::ostringstream out, err;
            const int64_t status = R::main(args, out, err);
            return jarr({jint(status), jstr(status == 0 ? out.str() : "")});
        }
        const string& path = f[1];
        const int64_t si = std::stoll(f[2]);
        codebreaker::source::ne_asm::segment::Seg& sg = seg_of(path, si);
        if (op == "reach") return set_d(R::reachable(sg));
        if (op == "bt") return set_d(R::branch_targets(sg));
        if (op == "br") {
            vector<string> xs;
            for (const auto& t : R::_branches(sg)) xs.push_back("(" + std::to_string(std::get<0>(t)) + ", " + std::to_string(std::get<1>(t)) + ")");
            return agg(xs);
        }
        if (op == "sdr") {
            vector<string> xs;
            for (const auto& t : R::safe_data_regions(sg))
                xs.push_back("(" + std::to_string(std::get<0>(t)) + ", " + std::to_string(std::get<1>(t)) + ", " + py::repr(std::get<2>(t)).utf8() + ")");
            return agg(xs);
        }
        throw py::ValueError("bad case " + op);
    });
}
