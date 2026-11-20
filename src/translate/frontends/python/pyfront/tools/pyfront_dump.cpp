// pyfront_dump -- parse Python files and print their trees in the canonical text form
// (CPython's ast.dump(tree, include_attributes=True)).
//
//   pyfront_dump FILE...                 one dump per file on stdout
//   pyfront_dump --batch LIST OUT        LIST: one path per line; OUT gets, per file,
//                                        "== PATH\n" then the dump or "ERROR msg (line, offset)"
//   pyfront_dump --time N LIST           parse every listed file N times; print seconds
//   --no-attributes                      leave positions out
//
// Exit status: 0 when every file parsed, 1 when any had a syntax error, 2 on bad usage.
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "pyfront/pyfront.hpp"

namespace {

bool read_file(const std::string& path, std::string& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    return true;
}

std::vector<std::string> read_list(const std::string& path) {
    std::vector<std::string> v;
    std::ifstream f(path, std::ios::binary);
    std::string line;
    while (std::getline(f, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
        if (!line.empty()) v.push_back(line);
    }
    return v;
}

std::string error_line(const pyfront::SyntaxError& e) {
    return "ERROR " + e.msg + " (" + std::to_string(e.lineno) + ", " + std::to_string(e.offset) + ")";
}

}  // namespace

int main(int argc, char** argv) {
    bool attrs = true;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--no-attributes")) attrs = false;
        else args.push_back(argv[i]);
    }
    if (args.empty()) {
        fprintf(stderr, "usage: pyfront_dump [--no-attributes] FILE... | --batch LIST OUT | --time N LIST\n");
        return 2;
    }
    int status = 0;
    if (args[0] == "--batch" && args.size() == 3) {
        std::ofstream out(args[2], std::ios::binary);
        for (const std::string& path : read_list(args[1])) {
            std::string src;
            out << "== " << path << "\n";
            if (!read_file(path, src)) { out << "ERROR cannot read\n"; status = 1; continue; }
            auto r = pyfront::parse(src, path);
            if (r) out << pyfront::dump(*r.module, attrs) << "\n";
            else { out << error_line(*r.error) << "\n"; status = 1; }
        }
        return status;
    }
    if (args[0] == "--time" && args.size() == 3) {
        int n = std::atoi(args[1].c_str());
        std::vector<std::string> srcs;
        size_t bytes = 0;
        for (const std::string& path : read_list(args[2])) {
            std::string s;
            if (read_file(path, s)) { bytes += s.size(); srcs.push_back(std::move(s)); }
        }
        double best = 1e30;
        size_t nodes = 0;
        for (int k = 0; k < n; ++k) {
            auto t0 = std::chrono::steady_clock::now();
            nodes = 0;
            for (const std::string& s : srcs) {
                auto r = pyfront::parse(s);
                if (r) nodes += r.module->arena.size();
                else status = 1;
            }
            double sec = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
            if (sec < best) best = sec;
        }
        printf("files %zu bytes %zu nodes %zu best-of-%d parse %.3f s\n", srcs.size(), bytes, nodes, n, best);
        return status;
    }
    for (const std::string& path : args) {
        std::string src;
        if (!read_file(path, src)) { fprintf(stderr, "%s: cannot read\n", path.c_str()); status = 1; continue; }
        auto r = pyfront::parse(src, path);
        if (r) {
            std::string d = pyfront::dump(*r.module, attrs);
            fwrite(d.data(), 1, d.size(), stdout);
            fputc('\n', stdout);
        } else {
            const auto& e = *r.error;
            fprintf(stderr, "  File \"%s\", line %d\n    %s\nSyntaxError: %s (offset %d)\n", path.c_str(), e.lineno, e.text.c_str(), e.msg.c_str(), e.offset);
            status = 1;
        }
    }
    return status;
}
