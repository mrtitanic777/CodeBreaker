// test.cpp -- runs the cases of cases.txt (written by expected.py) against the C++ codebreaker.ui.progress and prints them
// as expected.py does.   test.exe cases.txt > actual.txt     (run.cmd copies seed\ to work\ first; the clock is fixed to NOW)
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

#include "codebreaker/formats/inspect.hpp"
#include "codebreaker/ui/progress.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace P = codebreaker::ui::progress;
namespace R = codebreaker::verify::reconstruct;
namespace I = codebreaker::formats::inspect;
using py::str;

static const double NOW = 1800000000.0;

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

// the reading as json.dumps(sort_keys=True) writes it (the float `rate` is a placeholder swapped for its repr)
static std::string snapshot_json(const P::Snapshot& s) {
    I::Value v = I::Value::dict();
    v.set("active", I::Value(s.active));
    v.set("eta", s.eta ? I::Value(*s.eta) : I::Value());
    v.set("idle", s.idle ? I::Value(*s.idle) : I::Value());
    I::Value kinds = I::Value::list();
    for (const auto& k : s.kinds) {
        I::Value d = I::Value::dict();
        d.set("kind", I::Value(k.kind));
        d.set("name", I::Value(k.name));
        d.set("modules", I::Value(k.modules));
        d.set("measured", I::Value(k.measured));
        d.set("functions", I::Value(k.functions));
        for (const auto& kv : k.classes.items()) d.set(std::get<0>(kv), I::Value(std::get<1>(kv)));
        kinds.push(d);
    }
    v.set("kinds", kinds);
    v.set("log", I::Value(s.log));
    v.set("rate", I::Value("@@RATE@@"));
    I::Value recent = I::Value::list();
    for (const auto& r : s.recent) {
        I::Value d = I::Value::dict();
        d.set("module", I::Value(r.module));
        d.set("kind", I::Value(r.kind));
        d.set("results", I::Value(r.results));
        d.set("age", I::Value(r.age));
        recent.push(d);
    }
    v.set("recent", recent);
    v.set("store", I::Value(s.store));
    v.set("workers", I::Value(s.workers));
    std::string text = jd(v);
    std::string token = "\"@@RATE@@\"";
    size_t at = text.find(token);
    text.replace(at, token.size(), py::to_str(s.rate).utf8());
    return text;
}

static std::string maskt(const std::string& text) { return std::regex_replace(text, std::regex("(^|\n)\\[\\d\\d:\\d\\d:\\d\\d\\]"), "$1[T]"); }

static std::string capture(const py::list<str>& argv, const std::string& tmp) {
    std::fflush(stdout);
    int saved = _dup(1);
    int fd = _open(tmp.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    _dup2(fd, 1);
    _close(fd);
    std::string failure;
    int64_t code = 0;
    try {
        code = P::main(argv);
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
    return "exit " + std::to_string(code) + "\n" + maskt(ss.str()) + failure;
}

static void use(const std::string& state, const std::string& tmp) {
    std::string cwd = std::filesystem::current_path().string();
    std::string s = state != "none" ? cwd + "\\work\\" + state : cwd + "\\work\\no_store";
    std::string t = tmp == "workers" ? cwd + "\\inputs\\tmp_workers" : cwd + "\\inputs\\tmp_none";
    _putenv_s("CB_STATE", s.c_str());
    _putenv_s("CB_FUNCTIONS_DB", (s + "\\functions.sqlite").c_str());
    _putenv_s("TEMP", t.c_str());
    _putenv_s("TMP", t.c_str());
    _putenv_s("TMPDIR", t.c_str());
    py::tempfile::gettempdir_reset();
}

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

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    P::clock() = [] { return NOW; };
    {
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
    P::Snapshot cur, base;    // the reading of the last `snapshot` call (the original lined it right after); the one made-up lines start from
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> f = split(line, '\t');
        const std::string& op = f[0];
        const std::string& label = f.size() > 1 ? f[1] : "";
        auto arg = [&](size_t i) -> std::string { return f.size() > i ? f[i] : std::string(); };
        try {
            if (op == "use") {
                use(arg(2), arg(3));
                emit(label, "");
            } else if (op == "snapshot") {
                P::Snapshot s;
                {
                    Tm t("snapshot");
                    s = P::snapshot(arg(2).empty() ? std::nullopt : std::optional<str>(str(arg(2))));
                }
                cur = s;
                if (label == "snapshot st_a workers log_many.txt") base = s;
                emit(label, snapshot_json(s));
            } else if (op == "line") {
                P::Snapshot s = cur;
                s.time = str("T");
                str text;
                {
                    Tm t("line");
                    text = P::line(s);
                }
                emit(label, text.utf8());
            } else if (op == "line_variant") {
                P::Snapshot s = base;
                s.time = str("T");
                for (size_t i = 2; i < f.size(); i++) {
                    size_t eq = f[i].find('=');
                    std::string k = f[i].substr(0, eq), val = f[i].substr(eq + 1);
                    if (k == "workers") s.workers = std::stoll(val);
                    else if (k == "eta") s.eta = val == "None" ? std::nullopt : std::optional<int64_t>(std::stoll(val));
                    else if (k == "rate") s.rate = std::stod(val);
                    else if (k == "active") s.active = val != "0";
                    else if (k == "log") s.log = str(val);
                    else if (k == "recent") s.recent = py::list<P::Recent>();
                    else if (k == "kinds") s.kinds = py::list<P::KindReading>();
                    else if (k == "idle") s.idle = std::nullopt;
                    else if (k == "classes") {
                        py::list<P::KindReading> fresh;
                        for (const auto& kk0 : s.kinds) {
                            P::KindReading kk = kk0;
                            kk.classes = kk0.classes.copy();      // (a dict is shared on copy)
                            for (const auto& key : kk.classes.keys()) kk.classes.set(key, 0);
                            fresh.append(kk);
                        }
                        s.kinds = fresh;
                    }
                }
                emit(label, P::line(s).utf8());
            } else if (op == "last_line") {
                I::Value v(P::_last_line(str(arg(2))));
                emit(label, jd(v));
            } else if (op == "clock") {
                std::optional<int64_t> secs;
                if (arg(2) != "None") secs = std::stoll(arg(2));
                emit(label, P::_clock(secs).utf8());
            } else if (op == "busy") {
                int64_t n;
                {
                    Tm t("_busy_workers");
                    n = P::_busy_workers();
                }
                emit(label, std::to_string(n));
            } else if (op == "main") {
                py::list<str> args;
                for (size_t i = 2; i < f.size(); i++) args.append(str(f[i]));
                std::string text;
                {
                    Tm t("main");
                    text = capture(args, tmp);
                }
                emit(label, text);
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
