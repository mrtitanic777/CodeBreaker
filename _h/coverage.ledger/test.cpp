// test.cpp -- runs the cases of cases.txt (written by expected.py) against the C++ codebreaker.coverage.ledger and prints
// them as expected.py does.   test.exe cases.txt > actual.txt     (CB_STATE and CB_FUNCTIONS_DB name the state folder: run.cmd)
#include <fcntl.h>
#include <io.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "codebreaker/coverage/ledger.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace L = codebreaker::coverage::ledger;
namespace R = codebreaker::verify::reconstruct;
namespace I = codebreaker::formats::inspect;
using py::str;

static std::map<std::string, double> T;
struct Tm {
    std::string k;
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    explicit Tm(const std::string& key) : k(key) {}
    ~Tm() { T[k] += std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count(); }
};

static void emit(const std::string& label, const std::string& text) {
    std::string o = "== " + label + "\n" + text + "\n";
    std::fwrite(o.data(), 1, o.size(), stdout);
    std::fflush(stdout);
}
static std::string jd(const I::Value& v) { return I::dumps(v, std::nullopt, true).utf8(); }

// what reconstruct.digests() answered in the original (the lanes' own modules are not part of this port)
class StubKind : public R::Kind {
  public:
    StubKind(const std::string& n, const std::optional<std::string>& d) : name(n), digest(d) {}
    str KIND() const override { return str(name); }
    std::tuple<str, str> profile(const str&) const override {
        if (!digest) throw py::RuntimeError("not set up on this machine");
        return {str(*digest), str("{}")};
    }
    R::Prepared prepare(const str&, const str&) const override { throw py::RuntimeError("stub"); }
    R::Built build(const R::Prepared&, const str&, const str&) const override { throw py::RuntimeError("stub"); }
    R::Verdict judge(const R::Context&, const R::Item&) const override { throw py::RuntimeError("stub"); }

  private:
    std::string name;
    std::optional<std::string> digest;
};

static I::Value counts_val(const L::Counts& c) {
    I::Value v = I::Value::dict();
    v.set("functions", I::Value(c.functions));
    v.set("decompiled", I::Value(c.decompiled));
    v.set("compiled", I::Value(c.compiled));
    for (const auto& kv : c.classes.items()) v.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
    v.set("code_bytes", I::Value(c.code_bytes));
    v.set("bytes_known", I::Value(c.bytes_known));
    v.set("bytes_verified", I::Value(c.bytes_verified));
    v.set("bytes_weak", I::Value(c.bytes_weak));
    v.set("judged", I::Value(c.judged));
    return v;
}

static std::string led_json(const L::Ledger& led) {
    I::Value v = L::to_value(led);
    v.erase("generated");
    return jd(v);
}

static std::string mask(const std::string& text) {
    return std::regex_replace(text, std::regex("\"generated\": \"[^\"]*\""), "\"generated\": \"T\"");
}

static std::string state_dir() {
    const char* s = std::getenv("CB_STATE");
    return s ? s : "";
}

static std::string capture(const py::list<str>& argv, const std::string& tmp) {
    std::fflush(stdout);
    int saved = _dup(1);
    int fd = _open(tmp.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    _dup2(fd, 1);
    _close(fd);
    std::string failure;
    int64_t code = 0;
    try {
        code = L::main(argv);
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
    return "exit " + std::to_string(code) + "\n" + mask(ss.str()) + failure;
}

static std::string sha256_of(const std::string& path) {
    if (!py::os::path::isfile(str(path))) return "(no file)";
    return py::hashlib::sha256(py::open_rb(str(path)).read()).hexdigest().utf8();
}

static py::list<str> split_commas(const std::string& s) {
    py::list<str> out;
    if (s.empty()) return out;
    size_t p = 0;
    for (;;) {
        size_t q = s.find(',', p);
        out.append(str(s.substr(p, q == std::string::npos ? q : q - p)));
        if (q == std::string::npos) break;
        p = q + 1;
    }
    return out;
}

static std::optional<str> opt_digest(const std::string& s) { return s == "None" ? std::nullopt : std::optional<str>(str(s)); }

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    {   // the lanes' profiles, as the original answered them
        std::ifstream d("digests.txt");
        std::string line;
        while (std::getline(d, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            size_t t = line.find('\t');
            if (t == std::string::npos) continue;
            std::string kind = line.substr(0, t), dig = line.substr(t + 1);
            R::register_kind(str(kind), std::make_shared<StubKind>(kind, dig == "None" ? std::nullopt : std::optional<std::string>(dig)));
        }
    }
    std::ifstream in(argv[1], std::ios::binary);
    std::string tmp = std::string(argv[1]) + ".capture.tmp";
    std::string line;
    std::map<std::string, L::Ledger> leds;     // module -> its ledger (the original measured it once, then rendered it)
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
        const std::string& label = f.size() > 1 ? f[1] : "";
        auto arg = [&](size_t i) -> std::string { return f.size() > i ? f[i] : std::string(); };
        try {
            if (op == "corpus_modules") {
                I::Value v = I::Value::list();
                {
                    Tm t("corpus_modules");
                    for (const auto& m : L::corpus_modules(arg(2) == "1")) {
                        I::Value d = I::Value::dict();
                        d.set("name", I::Value(m.name));
                        d.set("path", I::Value(m.path));
                        d.set("kind", I::Value(m.kind));
                        d.set("size", I::Value(m.size));
                        d.set("code_bytes", I::Value(m.code_bytes));
                        v.push(d);
                    }
                }
                emit(label, jd(v));
            } else if (op == "profiles") {
                auto fs = L::open_store();
                auto p = L::profiles(arg(2) == "1" ? fs : nullptr);
                I::Value v = I::Value::dict();
                for (const auto& kv : p.items()) v.set(std::get<0>(kv), std::get<1>(kv) ? I::Value(*std::get<1>(kv)) : I::Value());
                emit(label, jd(v));
                if (fs) fs->close();
            } else if (op == "extents") {
                py::dict<str, int64_t> r;
                {
                    Tm t("extents");
                    r = L::extents(str(arg(2)), str(arg(3)), split_commas(arg(4)));
                }
                I::Value v = I::Value::dict();
                for (const auto& kv : r.items()) v.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
                emit(label, jd(v));
            } else if (op == "classes") {
                auto fs = L::open_store();
                auto r = L::_classes(fs, str(arg(2)), opt_digest(arg(3)));
                I::Value cls = I::Value::dict();
                for (const auto& kv : std::get<0>(r).items()) cls.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
                I::Value v = I::Value::list();
                v.push(cls);
                v.push(I::Value(std::get<1>(r)));
                emit(label, jd(v));
                fs->close();
            } else if (op == "counts") {
                auto fs = L::open_store();
                std::optional<std::tuple<int64_t, int64_t, codebreaker::engine::native_engine::Cell>> stamp;
                if (arg(6) == "1") {
                    auto st = fs->stamps();
                    if (st.contains(str(arg(2)))) stamp = st.at(str(arg(2)));
                }
                L::Counts c;
                {
                    Tm t("counts");
                    c = L::_counts(fs, str(arg(2)), opt_digest(arg(3)), str(arg(4)), str(arg(5)), stamp);
                }
                emit(label, jd(counts_val(c)));
                fs->close();
            } else if (op == "measure") {
                L::Ledger led;
                {
                    Tm t("measure");
                    std::string m = arg(2);
                    led = L::measure(nullptr, m.empty() ? std::nullopt : std::optional<str>(str(m)));
                }
                leds[arg(2)] = led;
                emit(label, led_json(led));
            } else if (op == "measure_store") {
                auto fs = L::open_store();
                std::string m = arg(2);
                L::Ledger led;
                {
                    Tm t("measure");
                    led = L::measure(fs, m.empty() ? std::nullopt : std::optional<str>(str(m)));
                }
                emit(label, led_json(led));
                fs->close();
            } else if (op == "render") {
                py::list<str> lines;
                {
                    Tm t("render");
                    lines = L::render(leds.at(arg(2)), arg(3) == "1");
                }
                emit(label, py::join(str("\n"), lines).utf8());
            } else if (op == "states") {
                I::Value v = I::Value::list();
                for (const L::Row& r : leds.at(arg(2)).modules) v.push(I::Value(L::_state(r)));
                emit(label, jd(v));
            } else if (op == "csv") {
                int64_t n;
                {
                    Tm t("write_csv");
                    n = L::write_csv(leds.at(arg(2)), str(arg(3)));
                }
                emit(label, std::to_string(n) + " " + sha256_of(arg(3)));
            } else if (op == "reasons") {
                auto fs = L::open_store();
                py::dict<str, py::list<std::tuple<str, int64_t>>> r;
                {
                    Tm t("reasons");
                    r = L::reasons(fs, str(arg(2)), str(arg(3)), std::stoll(arg(4)));
                }
                I::Value v = I::Value::dict();
                for (const auto& kv : r.items()) {
                    I::Value l = I::Value::list();
                    for (const auto& e : std::get<1>(kv)) {
                        I::Value pair = I::Value::list();
                        pair.push(I::Value(std::get<0>(e)));
                        pair.push(I::Value(std::get<1>(e)));
                        l.push(pair);
                    }
                    v.set(std::get<0>(kv), l);
                }
                emit(label, jd(v));
                fs->close();
            } else if (op == "pct") {
                emit(label, py::repr(L::_pct(std::stoll(arg(2)), std::stoll(arg(3)))).utf8());
            } else if (op == "table") {
                py::list<py::list<str>> rows = {py::list<str>{str("xx"), str("1"), str("22")}, py::list<str>{str("y"), str("333"), str("4")}};
                emit(label, py::join(str("\n"), L::_table(py::list<str>{str("a"), str("bb"), str("c")}, rows)).utf8());
            } else if (op == "rm_memo") {
                std::filesystem::remove(state_dir() + "/coverage_corpus.json");
                emit(label, "");
            } else if (op == "copy_memo") {
                std::filesystem::copy_file(arg(2), state_dir() + "/coverage_corpus.json", std::filesystem::copy_options::overwrite_existing);
                emit(label, "");
            } else if (op == "memo") {
                emit(label, sha256_of(state_dir() + "/coverage_corpus.json"));
            } else if (op == "main") {
                py::list<str> args;
                for (size_t i = 2; i < f.size(); i++) args.append(str(f[i]));
                std::string text;
                auto t0 = std::chrono::steady_clock::now();
                {
                    Tm t("main");
                    text = capture(args, tmp);
                }
                double dt = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
                if (dt > 0.3) std::fprintf(stderr, "slow: %s %.2f s\n", label.c_str(), dt);
                emit(label, text);
            } else if (op == "file") {
                emit(label, sha256_of(arg(2)));
            } else if (op == "nostore") {
                _putenv_s("CB_FUNCTIONS_DB", (state_dir() + "\\none.sqlite").c_str());   // (a store that is not there)
                emit(label, "");
            }
        } catch (const py::Exception& e) {
            emit(label, std::string("EXC ") + e.type() + ": " + e.what());
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
