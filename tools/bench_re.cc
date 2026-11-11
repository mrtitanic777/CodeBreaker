// bench_re <patterns.txt> <files...>: per pattern (lines "flags<TAB>pattern"), the time of finditer over every file,
// and a checksum of the matches (count and the sum of the spans) to compare with bench_re.py (the same in Python's re).
#include <chrono>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "pyre.hpp"

static std::string slurp(const char* p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {
    std::vector<std::pair<int, std::string>> pats;
    {
        std::string all = slurp(argv[1]);
        std::stringstream ss(all);
        std::string line;
        while (std::getline(ss, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            size_t t = line.find('\t');
            if (t == std::string::npos) continue;
            pats.push_back({atoi(line.substr(0, t).c_str()), line.substr(t + 1)});
        }
    }
    std::vector<std::string> texts;
    for (int i = 2; i < argc; i++) texts.push_back(slurp(argv[i]));
    double total = 0;
    for (auto& pr : pats) {
        re::Regex rx(pr.second, pr.first);
        long long n = 0, span = 0;
        auto t0 = std::chrono::steady_clock::now();
        for (int rep = 0; rep < 3; rep++)
            for (auto& tx : texts) {
                auto ms = rx.finditer(tx);
                if (rep == 0) {
                    n += (long long)ms.size();
                    for (auto& m : ms) span += m.end() - m.start() + m.start() % 7;
                }
            }
        double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count() / 3;
        total += ms;
        std::string shown = pr.second.substr(0, 60);
        printf("%8.1f ms  n=%lld span=%lld  %s\n", ms, n, span, shown.c_str());
    }
    printf("%8.1f ms total\n", total);
}
