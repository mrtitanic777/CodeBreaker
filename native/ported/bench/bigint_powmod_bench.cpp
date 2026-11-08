#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
using codebreaker::formats::xbox::crypto::MemCrypto::BigInt;
static std::string hex(const BigInt& v) {
    if (v.is_zero()) return "0";
    std::string s;
    const char* d = "0123456789abcdef";
    int64_t n = (v.bit_length() + 3) / 4;
    for (int64_t i = n - 1; i >= 0; --i) s.push_back(d[(v.shr(i * 4).low_byte()) & 15]);
    return s;
}
int main(int argc, char** argv) {
    std::ifstream f(argv[1]);
    std::string a, e, m;
    if (argc > 2) {
        f >> a >> e >> m;
        BigInt A = BigInt::from_hex(a), E = BigInt::from_hex(e), M = BigInt::from_hex(m);
        auto t = std::chrono::steady_clock::now();
        BigInt r;
        for (int i = 0; i < 20; i++) r = BigInt::powmod(A, E, M);
        printf("cpp 1024-bit powmod ms %f\n", std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count() / 20);
        return 0;
    }
    while (f >> a >> e >> m) {
        BigInt A = BigInt::from_hex(a), E = BigInt::from_hex(e), M = BigInt::from_hex(m);
        printf("%s %s %s 0\n", hex(BigInt::powmod(A, E, M)).c_str(), hex(A % M).c_str(), hex(A / M).c_str());
    }
}
