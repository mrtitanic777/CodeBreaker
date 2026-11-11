// test.cpp -- runs the cases of cases.txt (written by expected.py) against the C++ codebreaker.formats.inspect
// and prints them as expected.py does.   test.exe cases.txt > actual.txt
#include <fcntl.h>
#include <io.h>

#include <chrono>
#include <cstdio>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "codebreaker/formats/inspect.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace I = codebreaker::formats::inspect;
using py::str;


// seconds spent inside the module's functions, by kind of call (printed to stderr at the end; JSON writing is not counted)
static std::map<std::string, double> T;
struct Tm {
    std::string k;
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    explicit Tm(const std::string& key) : k(key) {}
    ~Tm() { T[k] += std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count(); }
};
static std::map<std::string, I::Value> infos;   // path -> the description (expected.py describes a file once)

static void emit(const std::string& label, const std::string& text) {
    std::string o = "== " + label + "\n" + text + "\n";
    std::fwrite(o.data(), 1, o.size(), stdout);
    std::fflush(stdout);
}

static std::string jd(const I::Value& v) { return I::dumps(v, std::nullopt, true).utf8(); }

static std::string res_json(const I::ResourceBody& b) {
    I::Value v = I::Value::dict();
    v.set("kind", I::Value(b.kind));
    if (b.content_type) v.set("content_type", I::Value(*b.content_type));
    if (b.bytes) v.set("bytes", I::Value(b.bytes->decode("latin1")));
    if (b.text) v.set("text", I::Value(*b.text));
    return jd(v);
}

static std::string yn(bool b) { return b ? "True" : "False"; }

// stdout of main(argv), captured (fd 1 redirected to a file while it runs)
static std::string capture(const py::list<str>& argv, int64_t& code, const std::string& tmp) {
    std::fflush(stdout);
    int saved = _dup(1);
    int fd = _open(tmp.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    _dup2(fd, 1);
    _close(fd);
    std::string failure;
    try {
        code = I::main(argv);
    } catch (const py::Exception& e) {
        failure = std::string("EXC ") + e.type() + ": " + e.what() + "\n";
        code = -1;
    }
    std::fflush(stdout);
    _dup2(saved, 1);
    _close(saved);
    std::ifstream in(tmp, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str() + failure;
}

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    std::ifstream in(argv[1], std::ios::binary);
    std::string tmp = std::string(argv[1]) + ".capture.tmp";
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> f;
        size_t p = 0;
        for (;;) {
            size_t q = line.find('\t', p);
            f.push_back(line.substr(p, q == std::string::npos ? q : q - p));
            if (q == std::string::npos) break;
            p = q + 1;
        }
        const std::string& op = f[0];
        try {
            if (op == "describe") {
                I::Value v;
                {
                    Tm t("describe");
                    v = I::to_value(I::describe(str(f[1])));
                }
                infos[f[1]] = v;
                emit("describe " + f[1], jd(v));
            } else if (op == "render") {
                const I::Value& v = infos[f[1]];
                str key(f[2]);
                py::list<str> lines;
                {
                    Tm t("render");
                    lines = I::render(v.at(key), key);
                }
                emit("render " + f[1] + " " + f[2], py::join(str("\n"), lines).utf8());
            } else if (op == "markdown") {
                str md;
                {
                    Tm t("markdown");
                    md = I::markdown(infos[f[1]]);
                }
                emit("markdown " + f[1], md.utf8());
            } else if (op == "strings") {
                int64_t min_len = std::stoll(f[2]);
                bool wide = f[3] == "1";
                std::optional<int64_t> limit;
                if (!f[4].empty()) limit = std::stoll(f[4]);
                py::list<I::StringRow> rows;
                {
                    Tm t("strings");
                    rows = I::strings(str(f[1]), min_len, wide, limit);
                }
                I::Value v = I::Value::list();
                int64_t n = 0;
                for (const auto& r : rows) {
                    if (++n <= 400) v.push(I::to_value(r));   // the first 400 only (as expected.py)
                }
                std::string label = "strings " + f[1] + " " + f[2] + " " + yn(wide) + " " + (limit ? std::to_string(*limit) : "None");
                // the big files (>= 400000 bytes) are listed with the limit 100 and no count line
                bool big = py::os::path::getsize(str(f[1])) >= 400000;
                emit(label, jd(v) + (big ? "" : "\ncount " + std::to_string(rows.size())));
            } else if (op == "hexdump") {
                str text;
                {
                    Tm t("hexdump");
                    text = I::hexdump(str(f[1]), std::stoll(f[2]), std::stoll(f[3]));
                }
                emit("hexdump " + f[1] + " " + f[2] + " " + f[3], text.utf8());
            } else if (op == "resource_body") {
                int64_t off = std::stoll(f[2]), size = std::stoll(f[3]), tid = std::stoll(f[4]);
                bool wide = f[5] == "1";
                I::ResourceBody body;
                {
                    Tm t("resource_body");
                    body = I::resource_body(str(f[1]), off, size, tid, wide);
                }
                emit("resource_body " + f[1] + " " + f[2] + " " + f[3] + " " + f[4] + " " + yn(wide), res_json(body));
            } else if (op == "folder") {
                I::Value v;
                {
                    Tm t("folder_map");
                    v = I::to_value(I::folder_map(str(f[1])));
                }
                emit("folder " + f[1], jd(v));
            } else if (op == "folder_markdown") {
                emit("folder_markdown " + f[1], I::markdown(I::to_value(I::folder_map(str(f[1])))).utf8());
            } else if (op == "text_kind") {
                auto r = I::_text_kind(str(f[1]), str(f[2]));
                std::string name = f[2];
                I::Value v;
                if (r) {
                    v = I::Value::dict();
                    v.set("command", I::Value(r->command));
                    v.set("family", I::Value(r->family));
                    v.set("format", I::Value(r->format));
                }
                emit("text_kind " + name, jd(v));
            } else if (op == "size") {
                emit("size " + f[1], I::_size(static_cast<double>(std::stoll(f[1]))).utf8());
            } else if (op == "xex_facts") {
                emit("xex_facts", jd(I::xex_facts(str(f[1]))));
            } else if (op == "main") {
                py::list<str> args;
                for (size_t i = 1; i < f.size(); i++) args.append(str(f[i]));
                int64_t code = 0;
                std::string text;
                {
                    Tm t("main");
                    text = capture(args, code, tmp);
                }
                std::string label = "main ";
                for (size_t i = 1; i < f.size(); i++) label += (i == 1 ? "" : " ") + f[i];
                emit(label, "exit " + std::to_string(code) + "\n" + text);
            } else if (op == "file") {
                py::bytes data = py::open_rb(str(f[1])).read();
                std::string name = f[1].substr(f[1].find_last_of("/\\") + 1);
                emit("file " + name, py::hashlib::sha256(data).hexdigest().utf8());
            }
        } catch (const py::Exception& e) {
            emit(op + " " + (f.size() > 1 ? f[1] : ""), std::string("EXC ") + e.type() + ": " + e.what());
        }
    }
    std::remove(tmp.c_str());
    double total = 0;
    std::string report = "C++ seconds by call:";
    for (const auto& kv : T) {
        char b[64];
        std::snprintf(b, sizeof b, " %s %.2f,", kv.first.c_str(), kv.second);
        report += b;
        total += kv.second;
    }
    std::fprintf(stderr, "%s (total %.2f)\n", report.c_str(), total);
    return 0;
}
