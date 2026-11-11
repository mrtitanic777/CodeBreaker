// codebreaker/invert/early_c_order_probe.cpp -- C++17 port of codebreaker/invert/early_c_order_probe.py (see the header).

#include "codebreaker/invert/early_c_order_probe.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "cs.h"

namespace codebreaker::invert::early_c_order_probe {

const py::list<py::str> TESTS = {
    py::str("s1(){int a,b,c,d; a=11; b=22; c=a; d=44;}"),
    py::str("s2(){int a,b,c; a=11; b=a; c=33;}"),
    py::str("s3(){int a,b,c; a=11; c=a; b=22;}"),
    py::str("s4(){int a,b,c,d; a=11; b=22; c=33; d=a;}"),
    py::str("s5(){int a,b,c; a=11; b=22; c=33;}"),
    py::str("s6(){int a,b; a=11; b=a;}"),
    py::str("s7(){int a,b,c; a=11; b=22; c=a;}"),
    py::str("s8(){int a,b,c,d; a=11; b=a; c=33; d=c;}"),
};

namespace {

void ensure_capstone() {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
}

bool is_reg(const py::str& s) { return s == py::str("ax") || s == py::str("bx") || s == py::str("cx") || s == py::str("dx"); }

// int(m.group(1), 0) if (m and m.group(1)) else 0, for re.search(r"bp \+ (0x[0-9a-f]+|\d+)|\[bp\]", text)
int64_t bp_offset(const py::str& text) {
    py::re::Pattern pat = py::re::compile(py::str("bp \\+ (0x[0-9a-f]+|\\d+)|\\[bp\\]"));
    auto m = pat.search(text);
    if (m && m->group_opt(1)) return py::int_parse(*m->group_opt(1), 0);
    return 0;
}

}  // namespace

py::list<py::str> stream(const py::str& src) {
    ensure_capstone();
    std::optional<py::bytes> obj = compilers::mc_compile_multi(src + py::str("\n"));
    // (omf.function_bytes(None) is {}: nothing in it, so the next() below raises StopIteration)
    auto fns = obj ? formats::omf::function_bytes(*obj) : py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>>();
    if (fns.empty()) throw py::StopIteration();
    py::bytes code = std::get<0>(std::get<1>(fns.items()[0]));
    py::list<py::str> ops;
    for (const cbinv::CsIns& ins : cbinv::disasm16(reinterpret_cast<const uint8_t*>(code.raw().data()), code.raw().size(), 0)) {
        if (ins.address < 0xf) continue;
        py::str mn(ins.mnemonic), o(ins.op_str);
        py::list<py::str> parts = py::split(o, py::str(", "));
        if (mn == py::str("mov") && is_reg(parts[0]) && (parts[1].startswith(py::str("0x")) || parts[1].isdigit())) {
            ops.append(py::percent(py::str("LD %s=%s"), parts[0], parts[1]));
        } else if (mn == py::str("mov") && is_reg(parts[0]) && parts[1].contains(py::str("[bp"))) {
            ops.append(py::percent(py::str("%s=[%d]"), parts[0], bp_offset(parts[1])));
        } else if ((mn == py::str("mov") || mn == py::str("add") || mn == py::str("sub")) && parts[0].contains(py::str("[bp"))) {
            ops.append(py::percent(py::str("ST[%d]=%s"), bp_offset(parts[0]), parts[1]));
        } else if (mn == py::str("inc") || mn == py::str("dec") || mn == py::str("imul") || mn == py::str("idiv")) {
            ops.append(py::percent(py::str("%s %s"), mn, o));
        }
    }
    return ops;
}

void main() {
    for (const py::str& src : TESTS.vec()) {
        py::list<py::str> s = stream(src);
        std::string r = "[";
        for (int64_t i = 0; i < s.size(); ++i) r += (i ? ", '" : "'") + s[i].utf8() + "'";
        py::print(py::percent(py::str("%-44s %s"), src, py::str(r + "]")));
    }
}

}  // namespace codebreaker::invert::early_c_order_probe
