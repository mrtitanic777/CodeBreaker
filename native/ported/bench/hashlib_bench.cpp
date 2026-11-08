#include <chrono>
#include <cstdio>
#include "py.hpp"
static double ms(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count();
}
template <class F> static double best(F f) {
    double b = 1e9;
    for (int i = 0; i < 15; i++) {
        auto t = std::chrono::steady_clock::now();
        f();
        double d = ms(t);
        if (d < b) b = d;
    }
    return b;
}
int main() {
    // correctness: lengths around every block boundary
    for (int n : {0, 1, 3, 55, 56, 57, 63, 64, 65, 111, 112, 119, 120, 127, 128, 129, 1000, 4096}) {
        std::string s(n, 0);
        uint32_t x = 12345u + n;
        for (auto& c : s) {
            x = x * 1664525u + 1013904223u;
            c = static_cast<char>(x >> 24);
        }
        py::bytes b(s);
        std::printf("%d %s %s %s %s\n", n, py::hashlib::md5(b).hexdigest().utf8().c_str(), py::hashlib::sha1(b).hexdigest().utf8().c_str(),
                    py::hashlib::sha256(b).hexdigest().utf8().c_str(), py::hashlib::sha512(b).hexdigest().utf8().c_str());
    }
    std::string big(2 * 1024 * 1024, 'x');
    uint32_t x = 7;
    for (auto& c : big) {
        x = x * 1664525u + 1013904223u;
        c = static_cast<char>(x >> 24);
    }
    py::bytes b(big);
    std::fprintf(stderr, "cpp md5    2MB best ms %.2f\n", best([&] { py::hashlib::md5(b).digest(); }));
    std::fprintf(stderr, "cpp sha1   2MB best ms %.2f\n", best([&] { py::hashlib::sha1(b).digest(); }));
    std::fprintf(stderr, "cpp sha256 2MB best ms %.2f\n", best([&] { py::hashlib::sha256(b).digest(); }));
    std::fprintf(stderr, "cpp sha512 2MB best ms %.2f\n", best([&] { py::hashlib::sha512(b).digest(); }));
}
