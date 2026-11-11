// test.cpp -- runs the cases of cases.txt (written by expected.py) against the C++ codebreaker.coverage.reach and prints them
// as expected.py does.   test.exe cases.txt > actual.txt     (run.cmd copies seed\ to state\ first; CB_STATE names state\)
#include <fcntl.h>
#include <io.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "codebreaker/coverage/reach.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "json.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace testkit {
std::map<std::string, std::vector<std::string>>& identities();
}

namespace R = codebreaker::coverage::reach;
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

static std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    size_t p = 0;
    for (;;) {
        size_t q = s.find(sep, p);
        out.push_back(s.substr(p, q == std::string::npos ? q : q - p));
        if (q == std::string::npos) break;
        p = q + 1;
    }
    return out;
}

static std::string sha256_of(const std::string& path) {
    if (!py::os::path::isfile(str(path))) return "(no file)";
    return py::hashlib::sha256(py::open_rb(str(path)).read()).hexdigest().utf8();
}

static I::Value strings(const std::vector<std::string>& v) {
    I::Value l = I::Value::list();
    for (const auto& s : v) l.push(I::Value(str(s)));
    return l;
}

static I::Value conv(const py::V& v) {
    if (v.is_none()) return I::Value();
    if (v.is_bool()) return I::Value(v.b);
    if (v.is_int()) return I::Value(v.as_int());
    if (v.is_float()) return I::Value(v.as_float());
    if (v.is_str()) return I::Value(str(v.str()));
    if (v.is_dict()) {
        I::Value d = I::Value::dict();
        for (const auto& kv : v.items()) d.set(str(kv.first.str()), conv(kv.second));
        return d;
    }
    I::Value l = I::Value::list();
    for (const py::V& x : v.seq()) l.push(conv(x));
    return l;
}

static I::Value judged_value(const R::Judged& j) {
    I::Value d = I::Value::dict();
    I::Value t = I::Value::dict();
    for (const auto& kv : j.tally.items()) t.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
    d.set("tally", t);
    d.set("seconds", I::Value(j.seconds));
    d.set("stopped", I::Value(j.stopped));
    return d;
}

static std::string state_dir() {
    const char* s = std::getenv("CB_STATE");
    return s ? s : "";
}

// the results file with its time removed, as JSON; and a hash of the report beside it
static std::string results_text(const std::string& name) {
    std::string p = state_dir() + "\\reach\\" + name + ".json";
    if (!py::os::path::isfile(str(p))) return "(no results file)";
    py::V d = py::json_parse(py::open_rb(str(p)).read().decode("utf-8").utf8());
    I::Value v = conv(d);
    v.erase("when");
    return jd(v) + "\n" + sha256_of(state_dir() + "\\reach\\" + name + ".md");
}
static std::string md_text(const std::string& name) {
    std::string p = state_dir() + "\\reach\\" + name + ".md";
    if (!py::os::path::isfile(str(p))) return "(no report)";
    std::string t = py::open_rb(str(p)).read().decode("utf-8").utf8();
    std::string out;
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i] == '\r' && i + 1 < t.size() && t[i + 1] == '\n') continue;
        out += t[i];
    }
    return out;
}

// the child `measure` is (the original ran python -m codebreaker measure): a command that behaves as the case needs
static py::list<str> child_for(const str& path) {
    std::string base = py::os::path::basename(path).lower().utf8();
    auto is = [&](const char* key) { return base.compare(0, std::string(key).size() + 1, std::string(key) + "_") == 0 || base == key; };
    if (is("fail")) return py::list<str>{str("cmd"), str("/c"), str("echo line1 & echo boom here & exit 3")};
    if (is("failsilent")) return py::list<str>{str("cmd"), str("/c"), str("exit 5")};
    if (is("failbig")) return py::list<str>{str("cmd"), str("/c"), str("echo " + std::string(300, 'z') + " & exit 9")};
    if (is("hang")) return py::list<str>{str("ping"), str("-n"), str("30"), str("127.0.0.1")};
    return py::list<str>{str("cmd"), str("/c"), str("echo"), str("hello")};
}

static std::string capture_main(const py::list<str>& argv, const std::string& tmp) {
    std::fflush(stdout);
    std::fflush(stderr);
    int so = _dup(1), se = _dup(2);
    int f1 = _open((tmp + ".out").c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    int f2 = _open((tmp + ".err").c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    _dup2(f1, 1);
    _dup2(f2, 2);
    _close(f1);
    _close(f2);
    std::string code;
    try {
        code = std::to_string(R::main(argv));
    } catch (const py::SystemExit& x) {
        code = "SystemExit(" + std::string(x.what()) + ")";
    } catch (const py::Exception& x) {
        code = std::string(x.type()) + ": " + x.what();
    }
    std::fflush(stdout);
    std::fflush(stderr);
    _dup2(so, 1);
    _dup2(se, 2);
    _close(so);
    _close(se);
    auto slurp = [](const std::string& p) {
        std::ifstream in(p, std::ios::binary);
        std::stringstream ss;
        ss << in.rdbuf();
        return ss.str();
    };
    std::string out = slurp(tmp + ".out"), err = slurp(tmp + ".err");
    std::remove((tmp + ".out").c_str());
    std::remove((tmp + ".err").c_str());
    return "exit " + code + "\n-- stdout\n" + out + "-- stderr\n" + err;
}

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    R::clock() = [] { return 1000.0; };
    R::measure_argv() = [](const str& path) { return child_for(path); };
    {   // what the lanes' registry and reconstruct.identity answered in the original
        std::map<std::string, R::PlatformInfo> plats;
        std::map<std::string, bool> none;
        std::ifstream k("knowledge.txt", std::ios::binary);
        std::string line;
        while (std::getline(k, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            std::vector<std::string> f = split(line, '\t');
            if (f[0] == "P") {
                if (f[2] == "NONE") {
                    none[f[1]] = true;
                } else {
                    R::PlatformInfo p;
                    p.title = str(f[2]);
                    p.name = str(f[3]);
                    p.lane = f[4] == "1";
                    p.family = str(f[5]);
                    p.container = str(f[6]);
                    plats[f[1]] = p;
                }
            } else if (f[0] == "I") {
                testkit::identities()[f[1]] = std::vector<std::string>(f.begin() + 2, f.end());
            }
        }
        R::platform_identify() = [plats, none](const str& path) -> std::optional<R::PlatformInfo> {
            auto it = plats.find(path.utf8());
            if (it != plats.end()) return it->second;
            if (none.count(path.utf8())) return std::nullopt;
            throw py::RuntimeError("stub: platforms.identify was not asked about " + path.utf8());
        };
    }
    std::ifstream in(argv[1], std::ios::binary);
    std::string tmp = std::string(argv[1]) + ".capture.tmp";
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> f = split(line, '\t');
        const std::string& op = f[0];
        const std::string& label = f.size() > 1 ? f[1] : "";
        auto arg = [&](size_t i) -> std::string { return f.size() > i ? f[i] : std::string(); };
        try {
            if (op == "describe") {
                I::Value v;
                try {
                    R::Described d;
                    {
                        Tm t("describe");
                        d = R::describe(str(arg(2)));
                    }
                    v = I::Value::dict();
                    v.set("found", I::Value(d.found));
                    v.set("platform", I::Value(d.platform));
                    v.set("lane", d.lane ? I::Value(*d.lane) : I::Value());
                    v.set("family", I::Value(d.family));
                    emit(label, jd(v));
                } catch (const py::Exception& e) {
                    emit(label, "\"" + std::string(e.type()) + "\"");
                }
            } else if (op == "gap" || op == "gaphead") {
                py::bytes head;
                if (op == "gap") {
                    if (py::os::path::isfile(str(arg(2)))) head = py::open_rb(str(arg(2))).read(0x200);
                } else {
                    head = py::bytes::fromhex(str(arg(3)));
                }
                auto g = R::_gap(str(arg(2)), head);
                I::Value v;
                if (g) {
                    v = I::Value::list();
                    v.push(I::Value(std::get<0>(*g)));
                    v.push(I::Value(std::get<1>(*g)));
                }
                emit(label, jd(v));
            } else if (op == "sha") {
                std::string r;
                try {
                    Tm t("_sha");
                    r = R::_sha(str(arg(2))).utf8();
                } catch (const py::OSError&) {
                    r = "";
                }
                emit(label, r);
            } else if (op == "programs") {
                std::vector<std::string> logs;
                std::vector<std::string> in_list = split(arg(3), ';');
                py::list<str> inputs;
                if (!arg(3).empty()) for (const auto& s : in_list) inputs.append(str(s));
                py::list<std::tuple<str, str>> got;
                {
                    Tm t("programs");
                    got = R::programs(inputs, std::stoll(arg(2)), [&](const str& s) { logs.push_back(s.utf8()); });
                }
                I::Value l = I::Value::list();
                for (const auto& po : got) {
                    I::Value p = I::Value::list();
                    p.push(I::Value(std::get<0>(po)));
                    p.push(I::Value(std::get<1>(po)));
                    l.push(p);
                }
                emit(label, jd(l) + "\n" + jd(strings(logs)));
            } else if (op == "unpack") {
                std::vector<std::string> logs;
                py::list<str> got;
                {
                    Tm t("_unpack");
                    got = R::_unpack(str(arg(2)), std::stoll(arg(3)), [&](const str& s) { logs.push_back(s.utf8()); });
                }
                I::Value paths = I::Value::list(), hashes = I::Value::list();
                for (const str& p : got) {
                    paths.push(I::Value(p));
                    hashes.push(I::Value(str(sha256_of(p.utf8()))));
                }
                I::Value all = I::Value::list();
                all.push(paths);
                all.push(strings(logs));
                all.push(hashes);
                emit(label, jd(all));
            } else if (op == "env") {
                _putenv_s(arg(2).c_str(), arg(3).c_str());
                emit(label, "");
            } else if (op == "tools") {
                str t;
                {
                    Tm tm("_tools");
                    t = R::_tools();
                }
                emit(label, t.utf8());
            } else if (op == "spread") {
                py::list<R::Row> rows;
                std::vector<std::string> sizes = split(arg(2), ',');
                for (size_t i = 0; i < sizes.size(); i++) {
                    if (sizes[i].empty()) continue;
                    R::Row r;
                    r.size = std::stoll(sizes[i]);
                    r.path = str("p" + sizes[i] + "_" + std::to_string(i));
                    rows.append(r);
                }
                py::list<R::Row> got = R::spread(rows, std::stoll(arg(3)), std::stoll(arg(4)));
                I::Value l = I::Value::list();
                for (const R::Row& r : got) l.push(I::Value(r.path));
                emit(label, jd(l));
            } else if (op == "report") {
                py::list<R::Row> rows = R::read_results(str(arg(2)));
                if (label == "report big") {
                    py::list<R::Row> twice;
                    for (const auto& r : rows) twice.append(r);
                    for (const auto& r : rows) twice.append(r);
                    rows = twice;
                }
                str text;
                {
                    Tm t("report");
                    text = R::report(rows);
                }
                emit(label, text.utf8());
            } else if (op == "judge") {
                R::Judged j;
                {
                    Tm t("judge");
                    j = R::judge(str(arg(2)), std::stoll(arg(3)));
                }
                emit(label, jd(judged_value(j)));
            } else if (op == "tally") {
                auto r = R::tally_of(str(arg(2)));
                I::Value t = I::Value::dict();
                for (const auto& kv : std::get<0>(r).items()) t.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
                I::Value l = I::Value::list();
                l.push(t);
                l.push(I::Value(std::get<1>(r)));
                emit(label, jd(l));
            } else if (op == "put_old") {
                std::filesystem::copy_file(arg(2), state_dir() + "\\reach\\r_old.json", std::filesystem::copy_options::overwrite_existing);
                emit(label, "");
            } else if (op == "run") {
                std::vector<std::string> logs;
                py::list<str> inputs;
                for (const auto& s : split(arg(9), ';')) inputs.append(str(s));
                py::list<R::Row> rows;
                str text;
                {
                    Tm t("run");
                    auto r = R::run(inputs, std::stoll(arg(3)), arg(4) == "1", std::stoll(arg(5)), std::stoll(arg(6)), str(arg(2)), arg(7) == "1",
                                    std::stoll(arg(8)), [&](const str& s) { logs.push_back(s.utf8()); });
                    rows = std::get<0>(r);
                    text = std::get<1>(r);
                }
                I::Value l = I::Value::list();
                for (const R::Row& r : rows) l.push(R::to_value(r));
                I::Value all = I::Value::list();
                all.push(l);
                all.push(I::Value(text));
                all.push(strings(logs));
                emit(label, jd(all) + "\n" + results_text(arg(2)) + "\n" + md_text(arg(2)));
            } else if (op == "main") {
                py::list<str> args;
                for (size_t i = 2; i < f.size(); i++) args.append(str(f[i]));
                std::string text;
                {
                    Tm t("main");
                    text = capture_main(args, tmp);
                }
                emit(label, text);
            }
        } catch (const py::Exception& e) {
            emit(label, std::string("EXC ") + e.type() + ": " + e.what());
        }
    }
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
