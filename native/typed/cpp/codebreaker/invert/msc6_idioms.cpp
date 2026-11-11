// codebreaker/invert/msc6_idioms.cpp -- C++17 port of codebreaker/invert/msc6_idioms.py (see the header).

#include "codebreaker/invert/msc6_idioms.hpp"

#include "codebreaker/invert/compilers.hpp"

namespace codebreaker::invert::msc6_idioms {

namespace ora = codebreaker::invert::compilers;

const py::list<py::str> FLAGS = {py::str("/AM /G2 /Gsw /Ox"), py::str("/AM /G2 /Gsw /Os"), py::str("/AM /G2 /Gsw /Ot"),
                                 py::str("/AM /G2 /Gsw /O1"), py::str("/AM /G2 /Gsw /O2"), py::str("/AM /G2 /Gsw")};

const py::list<std::tuple<py::str, py::str>> IDIOMS = {
    {py::str("ret-const"), py::str("int far pascal F(void){ return 1; }")},
    {py::str("ret-param"), py::str("int far pascal F(int a){ return a; }")},
    {py::str("frame-local"), py::str("int far pascal F(void){ int i; i=0; return i; }")},
    {py::str("bool-eq"), py::str("int far pascal F(int a){ return a == 1; }")},
    {py::str("add-params"), py::str("int far pascal F(int a,int b){ return a+b; }")},
    {py::str("far-deref"), py::str("int far pascal F(int far *p){ return *p; }")},
    {py::str("far-field"),
     py::str("typedef unsigned int WORD; int far pascal F(char far *p){ return *(WORD far*)((char far*)p+4); }")},
    {py::str("cdecl-clean"), py::str("typedef unsigned int WORD; extern WORD far G(); int far pascal F(int a){ return G(a); }")},
    {py::str("loop-count"), py::str("int far pascal F(int n){ int i,s; s=0; for(i=0;i<n;i++) s+=i; return s; }")},
    {py::str("long-shl"), py::str("long far pascal F(long a){ return a << 4; }")},
    {py::str("global-store"), py::str("extern int gW; void far pascal F(void){ gW = -1; }")},
    {py::str("if-else"), py::str("int far pascal F(int a){ if(a) return 1; return 0; }")},
};

const py::bytes _PRO6 = py::bytes::fromhex(py::str("1e589045558bec1e8ed8"));
const py::bytes _EPI6 = py::bytes::fromhex(py::str("4d4d8be51f5d4d"));

py::str _tag(const py::bytes& b) {
    py::list<py::str> t;
    t.append(py::str(b.startswith(_PRO6) ? "pro" : "PRO?"));
    if (b.slice(0, 16).contains(py::bytes::fromhex(py::str("83ec")))) t.append(py::str("frame"));   // sub sp,N (a stack frame)
    if (b.contains(_EPI6)) t.append(py::str("epi"));
    if (b.contains(py::bytes::fromhex(py::str("1bc0f7d8")))) t.append(py::str("sbb/neg"));         // boolean idiom
    return py::join(py::str(","), t);
}

py::dict<py::str, py::dict<py::str, py::str>> probe(const std::optional<py::list<py::str>>& flags_, const std::optional<py::list<std::tuple<py::str, py::str>>>& idioms_) {
    // (`flags or FLAGS`: an empty list is the default too)
    const py::list<py::str>& flags = (flags_ && !flags_->empty()) ? *flags_ : FLAGS;
    const py::list<std::tuple<py::str, py::str>>& idioms = (idioms_ && !idioms_->empty()) ? *idioms_ : IDIOMS;
    py::dict<py::str, py::dict<py::str, py::str>> out;
    for (const auto& ns : idioms.vec()) {
        const py::str& name = std::get<0>(ns);
        out[name] = py::dict<py::str, py::str>();
        for (const py::str& fl : flags.vec()) {
            try {
                out[name][fl] = ora::mc6_compile(std::get<1>(ns), fl).hex();
            } catch (const py::Exception&) {
                out[name][fl] = py::str("ERR");
            }
        }
    }
    return out;
}

py::list<std::tuple<py::str, int64_t, py::dict<py::str, py::list<py::str>>>> flag_sensitivity(const py::dict<py::str, py::dict<py::str, py::str>>& res) {
    py::list<std::tuple<py::str, int64_t, py::dict<py::str, py::list<py::str>>>> rows;
    for (const auto& nb : res.items()) {
        py::dict<py::str, py::list<py::str>> forms;
        for (const auto& fh : std::get<1>(nb).items()) {
            py::list<py::str> words = py::split(std::get<0>(fh));
            if (!forms.contains(std::get<1>(fh))) forms[std::get<1>(fh)] = py::list<py::str>();
            forms[std::get<1>(fh)].append(words[-1]);   // group flags by identical output
        }
        rows.append(std::make_tuple(std::get<0>(nb), forms.size(), forms));
    }
    return rows;
}

int64_t main() {
    if (!ora::available()) {
        py::print(py::str("oracle not configured"));
        return 1;
    }
    auto res = probe();
    py::print(py::str("MSC 6.0a idiom flag-sensitivity (distinct byte forms across the flag matrix):\n"));
    for (const auto& row : flag_sensitivity(res).vec()) {
        int64_t nd = std::get<1>(row);
        py::print(py::percent(py::str("%-14s %d form(s)%s"), std::get<0>(row), nd, py::str(nd > 1 ? "  FLAG-SENSITIVE" : "")));
        for (const auto& hf : std::get<2>(row).items()) {
            const py::str& hx = std::get<0>(hf);
            py::str joined = py::join(py::str(","), std::get<1>(hf));
            if (hx == py::str("ERR")) {
                py::print(py::percent(py::str("      ERR under %s"), joined));
                continue;
            }
            py::print(py::percent(py::str("      [%s] %s"), joined, _tag(py::bytes::fromhex(hx))));
        }
    }
    return 0;
}

}  // namespace codebreaker::invert::msc6_idioms
