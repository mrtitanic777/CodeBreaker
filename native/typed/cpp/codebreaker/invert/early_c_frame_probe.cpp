// codebreaker/invert/early_c_frame_probe.cpp -- C++17 port of codebreaker/invert/early_c_frame_probe.py (see the header).

#include "codebreaker/invert/early_c_frame_probe.hpp"

#include <algorithm>
#include <filesystem>
#include <mutex>
#include <set>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "cs.h"

namespace codebreaker::invert::early_c_frame_probe {

const py::str BATTERY = py::str(
    "\n"
    "g01(){int a,b; a=(b+1)*2;}\n"
    "g02(){int a,b,c; a=(b+c)*(b-c);}\n"
    "g03(){long a,b; a=b+1;}\n"
    "g04(){long a; int b; a=b;}\n"
    "g05(){int a; long b,c; a=1;}\n"
    "g06(){char a,b; a=1; b=2;}\n"
    "g07(){int a; char b; long c; a=1; b=2; c=3;}\n"
    "g08(){int a[3]; a[0]=1;}\n"
    "g09(){int a,b; a=1; {int c; c=a; b=c;}}\n"
    "g10(p)int p;{int a; a=p+1;}\n"
    "g11(){int a,b,c; a=b+c+1;}\n"
    "g12(){int a; a=1; a=a*a+a;}\n"
    "g13(){int a,b; a=1;b=2;return a*b+a-b;}\n"
    "g14(){long a,b,c; a=b*c;}\n"
    "g15(){int a,b,c,d,e; a=b=c=d=e=0;}\n");

namespace {
void ensure_capstone() {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
}
}  // namespace

std::tuple<std::optional<int64_t>, py::list<int64_t>> analyze(const py::str& name, const py::bytes& code) {
    (void)name;
    ensure_capstone();
    std::optional<int64_t> fs;
    std::vector<int64_t> refs;
    py::re::Pattern pat = py::re::compile(py::str("\\[bp\\s*([+\\-]\\s*0x[0-9a-f]+|[+\\-]\\s*\\d+)?\\]"));
    for (const cbinv::CsIns& ins : cbinv::disasm16(reinterpret_cast<const uint8_t*>(code.raw().data()), code.raw().size(), 0, true)) {
        py::str op(ins.op_str);
        if (ins.mnemonic == "sub" && op.startswith(py::str("sp,"))) {
            try {
                fs = py::int_parse(py::split(op, py::str(","))[1], 0);
            } catch (const py::Exception&) {
            }
        }
        for (const auto& mm : pat.finditer(op).vec()) {
            std::optional<py::str> d = mm.group_opt(1);
            refs.push_back(d ? py::int_parse(d->replace(py::str(" "), py::str("")), 0) : 0);
        }
    }
    std::set<int64_t> uniq(refs.begin(), refs.end());
    py::list<int64_t> disps;
    for (int64_t d : uniq) disps.append(d);
    return std::make_tuple(fs, disps);
}

void main() {
    std::optional<py::bytes> obj = compilers::mc_compile_multi(BATTERY);
    if (!obj || obj->size() == 0) {
        py::print(py::str("COMPILE FAILED"));
        return;
    }
    auto fns = formats::omf::function_bytes(*obj);
    std::vector<py::str> order;
    for (const auto& kv : fns.items()) order.push_back(std::get<0>(kv));
    std::stable_sort(order.begin(), order.end(), [](const py::str& x, const py::str& y) { return x.upper() < y.upper(); });
    for (const py::str& k : order) {
        py::bytes code = std::get<0>(fns.at(k));
        auto [fs, disps] = analyze(k, code);
        std::string pos = "[";
        bool first = true;
        for (int64_t d : disps.vec())
            if (d >= 0) {
                pos += (first ? "" : ", ") + std::to_string(d);
                first = false;
            }
        pos += "]";
        py::print(py::percent(py::str("%-6s fs=%-4s  bp-disps=%s"), k.lstrip(py::str("_")), fs ? py::str(std::to_string(*fs)) : py::str("None"), py::str(pos)));
    }
}

}  // namespace codebreaker::invert::early_c_frame_probe
