// test.cpp: reads cases.txt (argv[1]) and prints one JSON line per case, as expected.py does.
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "py.hpp"
#include "codebreaker/exact/readable.hpp"

namespace rd = codebreaker::exact::readable;
namespace fs = std::filesystem;

static std::string unhex(const std::string& h) {
    if (h == "-") return "";
    std::string o;
    for (size_t i = 0; i + 1 < h.size(); i += 2) o += static_cast<char>(std::stoi(h.substr(i, 2), nullptr, 16));
    return o;
}
static py::str S(const std::string& h) { return py::str(unhex(h)); }

// json.dumps(ensure_ascii=True)
static std::string js(const py::str& s) {
    std::string o = "\"";
    for (char32_t c : s.u32()) {
        char b[16];
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c == '\b') o += "\\b";
        else if (c == '\f') o += "\\f";
        else if (c < 0x20 || c > 0x7e) {
            if (c >= 0x10000) {
                uint32_t v = c - 0x10000;
                std::snprintf(b, sizeof b, "\\u%04x\\u%04x", 0xd800 + (v >> 10), 0xdc00 + (v & 0x3ff));
            } else std::snprintf(b, sizeof b, "\\u%04x", static_cast<unsigned>(c));
            o += b;
        } else o += static_cast<char>(c);
    }
    return o + "\"";
}

static std::string read_all(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// the test double: SYMBOLS parsed as Project.symbols() does, one-function files f<5 hex>.c, and a ledger that
// calls a file "differs" when it holds BREAK
class Fake : public rd::Project {
    std::string d_, src_;

public:
    Fake(std::string d, std::string src) : d_(std::move(d)), src_(std::move(src)) {}
    py::str dir() const override { return py::str(d_); }
    py::str src() const override { return py::str(src_); }
    rd::Symbols symbols() const override {
        rd::Symbols s;
        fs::path p = fs::u8path(src_) / "SYMBOLS";
        if (!fs::exists(p)) return s;
        std::istringstream in(read_all(p));
        std::string l;
        while (std::getline(in, l)) {
            std::istringstream ls(l);
            std::vector<std::string> c;
            std::string w;
            while (ls >> w) c.push_back(w);
            if (l.rfind("#", 0) == 0 || c.size() < 2) continue;
            if (c.size() >= 3 && (c[1] == "C" || c[1] == "D" || c[1] == "S"))
                s.set(py::str(c[0]), std::make_tuple(py::str(c[1]), static_cast<int64_t>(std::stoll(c[2], nullptr, 16))));
            else
                s.set(py::str(c[0]), std::make_tuple(py::str("?"), static_cast<int64_t>(std::stoll(c[1], nullptr, 16))));
        }
        return s;
    }
    py::list<std::tuple<int64_t, py::str>> sources() const override {
        std::vector<std::string> names;
        for (auto& e : fs::directory_iterator(fs::u8path(src_))) names.push_back(e.path().filename().string());
        std::sort(names.begin(), names.end());
        py::list<std::tuple<int64_t, py::str>> out;
        for (auto& f : names) {
            if (f.size() == 8 && f[0] == 'f' && f.substr(6) == ".c" &&
                f.substr(1, 5).find_first_not_of("0123456789abcdef") == std::string::npos)
                out.append(std::make_tuple(static_cast<int64_t>(std::stoll(f.substr(1, 5), nullptr, 16)),
                                           py::str((fs::u8path(src_) / f).generic_u8string())));
        }
        return out;
    }
    py::list<rd::Row> ledger_run() const override {
        py::list<rd::Row> rows;
        for (const auto& s : sources()) {
            fs::path p = fs::u8path(std::get<1>(s).utf8());
            std::string t = read_all(p);
            char nm[32];
            std::snprintf(nm, sizeof nm, "fn%05llx", static_cast<unsigned long long>(std::get<0>(s)));
            rd::Row r;
            r.file = py::str(p.filename().string());
            r.name = py::str(nm);
            r.address = std::get<0>(s);
            r.length = 10;
            r.verdict = py::str(t.find("BREAK") != std::string::npos ? "differs" : "identical");
            rows.append(r);
        }
        return rows;
    }
    std::unique_ptr<rd::Project> with_src(const py::str& src) const override {
        return std::make_unique<Fake>(d_, src.utf8());
    }
};

int main(int argc, char** argv) {
    std::ifstream in(argv[1]);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string op;
        ls >> op;
        std::string out;
        try {
            if (op == "R") {
                std::string t;
                int n;
                ls >> t >> n;
                rd::Names names;
                for (int i = 0; i < n; ++i) {
                    std::string k, a, nm;
                    ls >> k >> a >> nm;
                    names.set(rd::Place(py::str(k), std::stoll(a)), S(nm));
                }
                out = js(rd::rename(S(t), names));
            } else if (op == "M") {
                int n;
                ls >> n;
                rd::Symbols syms;
                for (int i = 0; i < n; ++i) {
                    std::string nm, t, a;
                    ls >> nm >> t >> a;
                    syms.set(S(nm), std::make_tuple(py::str(t), static_cast<int64_t>(std::stoll(a))));
                }
                out = "[";
                bool first = true;
                for (const auto& kv : rd::mapping(syms).items()) {
                    const auto& k = std::get<0>(kv);
                    out += (first ? "" : ",") + std::string("[") + js(std::get<0>(k)) + "," + std::to_string(std::get<1>(k)) + "," +
                           js(std::get<1>(kv)) + "]";
                    first = false;
                }
                out += "]";
            } else if (op == "C") {
                int n;
                ls >> n;
                py::list<rd::Row> rows;
                for (int i = 0; i < n; ++i) {
                    std::string a, b, c;
                    ls >> a >> b >> c;
                    rd::Row r;
                    r.file = S(a);
                    r.name = S(b);
                    r.verdict = S(c);
                    rows.append(r);
                }
                out = "[";
                bool first = true;
                for (const auto& kv : rd::counts(rows).items()) {
                    const auto& k = std::get<0>(kv);
                    out += (first ? "" : ",") + std::string("[") + js(std::get<0>(k)) + "," + js(std::get<1>(k)) + "," +
                           js(std::get<1>(kv)) + "]";
                    first = false;
                }
                out += "]";
            } else if (op == "P") {
                std::string d;
                int n;
                ls >> d >> n;
                std::string dir = unhex(d);
                py::list<py::str> av;
                for (int i = 0; i < n; ++i) {
                    std::string a;
                    ls >> a;
                    av.append(S(a));
                }
                Fake prj(dir, dir + "/src");
                // capture stdout
                std::fflush(stdout);
                std::string cap = (fs::temp_directory_path() / "readable_test_stdout.txt").string();
                int saved = _dup(1); int ofd = _open(cap.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0666); _dup2(ofd, 1); _close(ofd);
                int64_t rc = 0;
                std::string exc;
                try {
                    rc = rd::main(prj, av);
                } catch (const std::exception& e) {
                    exc = e.what();
                }
                std::fflush(stdout);
                _dup2(saved, 1); _close(saved);
                std::string o = read_all(cap);
                std::string files;
                std::vector<std::string> names;
                for (auto& e : fs::directory_iterator(fs::u8path(dir) / "src")) names.push_back(e.path().filename().string());
                std::sort(names.begin(), names.end());
                bool first = true;
                for (auto& f : names) {
                    std::string b = read_all(fs::u8path(dir) / "src" / f);
                    files += (first ? "" : ",") + js(py::str(f)) + ":" + js(py::bytes(b).decode("latin1"));
                    first = false;
                }
                out = "{\"rc\":" + std::to_string(rc) + ",\"out\":" + js(py::str(o)) + ",\"files\":{" + files + "}}";
            }
        } catch (const py::Exception& e) {
            out = std::string("{\"exc\":\"") + e.type() + "\"}";
        }
        std::cout << out << "\n";
    }
    return 0;
}
