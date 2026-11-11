// prof.cpp -- where corpus_modules spends its time (scratch)
#include <sys/stat.h>

#include <chrono>
#include <cstdio>
#include <filesystem>

#include "codebreaker/base/corpus.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "json.hpp"

using clk = std::chrono::steady_clock;
static double since(clk::time_point t) { return std::chrono::duration<double>(clk::now() - t).count(); }

int main() {
    namespace C = codebreaker::base::corpus;
    py::list<py::str> swept = {py::str(".dll"), py::str(".exe"), py::str(".drv"), py::str(".ocx"), py::str(".cpl")};
    py::list<py::str> le = {py::str(".vxd"), py::str(".386"), py::str(".pdr"), py::str(".sys"), py::str(".drv"), py::str(".exe")};
    py::list<py::str> dos = {py::str(".exe"), py::str(".com"), py::str(".sys"), py::str(".vxd"), py::str(".386")};
    for (int r = 0; r < 3; r++) {
        auto t = clk::now();
        py::list<py::str> all;
        for (const char* k : {"pe", "ne", "le", "dos"}) {
            auto t1 = clk::now();
            auto m = C::modules(py::str(k), std::string(k) == "le" ? le : std::string(k) == "dos" ? dos : swept);
            std::printf("  modules(%s) %.3f (%lld)\n", k, since(t1), (long long)m.size());
            for (const auto& p : m) all.append(p);
        }
        std::printf("modules x4 %.3f\n", since(t));
        t = clk::now();
        int64_t s = 0;
        for (const auto& p : all) {
            struct _stat64 st;
            _wstat64(std::filesystem::u8path(p.utf8()).wstring().c_str(), &st);
            s += st.st_size;
        }
        std::printf("stat x all %.3f\n", since(t));
        t = clk::now();
        std::string text = py::open_rb(py::str("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\state\\coverage_corpus.json")).read().raw();
        auto v = py::json_parse(text);
        std::printf("memo read+parse %.3f (%lld)\n", since(t), (long long)v.size());
    }
    return 0;
}
