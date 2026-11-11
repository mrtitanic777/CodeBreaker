#include <chrono>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "json.hpp"
static double ms(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count();
}
int main(int argc, char** argv) {
    std::ifstream f(argv[1], std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    std::string text = ss.str();
    py::V v;
    double best = 1e9, bestfree = 1e9, bestd = 1e9;
    for (int i = 0; i < 12; i++) {
        auto t2 = std::chrono::steady_clock::now();
        v = py::V();
        double fr = ms(t2);
        auto t = std::chrono::steady_clock::now();
        v = py::json_parse(text);
        double d = ms(t);
        if (d < best) best = d;
        if (fr < bestfree) bestfree = fr;
    }
    std::string out;
    for (int i = 0; i < 12; i++) {
        auto t = std::chrono::steady_clock::now();
        out = py::json_dump(v);
        double d = ms(t);
        if (d < bestd) bestd = d;
    }
    printf("cpp loads (parse only) best ms %f, freeing the tree %f; dumps best ms %f\n", best, bestfree, bestd);
    std::ofstream(argv[2], std::ios::binary) << out;
}
