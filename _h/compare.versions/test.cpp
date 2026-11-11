// test.cpp -- runs the cases of cases.txt (written by expected.py) against the C++ codebreaker.compare.versions and prints
// them as expected.py does.   test.exe cases.txt > actual.txt     (run.cmd copies seed\ to state\ first)
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

#include "codebreaker/compare/versions.hpp"
#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace testkit {
std::map<std::string, std::vector<std::string>>& identities();
std::map<std::string, std::vector<std::tuple<int64_t, int64_t, std::string>>>& starts();
}

namespace Vs = codebreaker::compare::versions;
namespace I = codebreaker::formats::inspect;
namespace E = codebreaker::engine::native_engine;
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

static I::Value cell_value(const E::Cell& c) {
    if (std::holds_alternative<str>(c)) return I::Value(std::get<str>(c));
    if (std::holds_alternative<int64_t>(c)) return I::Value(std::get<int64_t>(c));
    return I::Value();
}

static I::Value side_value(const Vs::Side& s) {
    I::Value v = I::Value::dict();
    v.set("kind", I::Value(s.kind));
    v.set("name", I::Value(s.name));
    v.set("code", I::Value(s.code));
    v.set("order", I::Value::from_strings(s.order));
    I::Value callers = I::Value::dict();
    for (const auto& kv : s.callers) {
        I::Value l = I::Value::list();
        for (const auto& c : kv.second) l.push(I::Value(str(c)));
        callers.set(str(kv.first), l);
    }
    v.set("callers", callers);
    I::Value fns = I::Value::dict();
    for (const auto& kv : s.functions.items()) {
        const Vs::Function& f = *std::get<1>(kv);
        I::Value d = I::Value::dict();
        d.set("key", I::Value(f.key));
        d.set("name", I::Value(f.name));
        d.set("named", I::Value(f.named));
        d.set("at", I::Value(f.at));
        d.set("size", I::Value(f.size));
        I::Value rows = I::Value::list();
        for (const auto& r : f.rows) {
            I::Value row = I::Value::list();
            row.push(I::Value(std::get<0>(r)));
            row.push(I::Value(std::get<1>(r)));
            rows.push(row);
        }
        d.set("rows", rows);
        py::list<str> text, shape;
        for (const str& t : f.text) text.append(t);
        for (const str& t : f.shape) shape.append(t);
        d.set("text", I::Value::from_strings(text));
        d.set("shape", I::Value::from_strings(shape));
        I::Value ids = I::Value::dict();
        ids.set("bytes", I::Value(f.ids.bytes));
        ids.set("text", I::Value(f.ids.text));
        ids.set("shape", I::Value(f.ids.shape));
        d.set("ids", ids);
        I::Value calls = I::Value::list();
        for (const auto& c : f.calls) {
            I::Value pr = I::Value::list();
            pr.push(I::Value(c.kind));
            pr.push(I::Value(c.key));
            calls.push(pr);
        }
        d.set("calls", calls);
        I::Value marks = I::Value::list();
        for (const auto& m : f.marks) marks.push(I::Value(str(m)));
        d.set("marks", marks);
        d.set("jumps", I::Value(f.jumps));
        d.set("count", I::Value(f.count()));
        fns.set(std::get<0>(kv), d);
    }
    v.set("functions", fns);
    return v;
}

static I::Value counts_value(const Vs::Comparison& c) {
    Vs::Counts n = c.counts();
    I::Value cv = I::Value::dict();
    cv.set("identical", I::Value(n.identical));
    cv.set("identical_bytes", I::Value(n.identical_bytes));
    cv.set("identical_text", I::Value(n.identical_text));
    cv.set("changed", I::Value(n.changed));
    cv.set("added", I::Value(n.added));
    cv.set("removed", I::Value(n.removed));
    Vs::CodeBytes cb = c.code();
    I::Value cbv = I::Value::dict();
    cbv.set("to_bytes", I::Value(cb.to_bytes));
    cbv.set("identical", I::Value(cb.identical));
    cbv.set("changed", I::Value(cb.changed));
    cbv.set("added", I::Value(cb.added));
    I::Value names = I::Value::list();
    for (const auto& nm : c.names()) {
        I::Value d = I::Value::dict();
        d.set("to", I::Value(nm.to));
        d.set("name", I::Value(nm.name));
        d.set("from", I::Value(nm.from));
        d.set("identical", I::Value(nm.identical));
        names.push(d);
    }
    I::Value all = I::Value::list();
    all.push(cv);
    all.push(cbv);
    all.push(names);
    return all;
}

static I::Value recon_value(const std::vector<Vs::Reconstruction>& rs) {
    I::Value l = I::Value::list();
    for (const auto& r : rs) {
        I::Value d = I::Value::dict();
        d.set("from", I::Value(r.from));
        d.set("to", I::Value(r.to));
        d.set("class", I::Value(r.klass));
        d.set("engine", r.engine ? I::Value(*r.engine) : I::Value());
        d.set("name", r.name ? I::Value(*r.name) : I::Value());
        d.set("source", I::Value(r.source));
        l.push(d);
    }
    return l;
}

static std::string capture_main(const py::list<str>& argv, const std::string& tmp) {
    std::fflush(stdout);
    int so = _dup(1);
    int f1 = _open(tmp.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0600);
    _dup2(f1, 1);
    _close(f1);
    std::string code;
    try {
        code = std::to_string(Vs::main(argv));
    } catch (const py::SystemExit& x) {
        code = "SystemExit(" + std::string(x.what()) + ")";
    } catch (const py::Exception& x) {
        code = std::string(x.type()) + ": " + x.what();
    }
    std::fflush(stdout);
    _dup2(so, 1);
    _close(so);
    std::ifstream in(tmp, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    std::remove(tmp.c_str());
    return "exit " + code + "\n" + ss.str();
}

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    {
        std::ifstream c("capstone.txt");
        std::string path;
        std::getline(c, path);
        Vs::set_capstone_dll(str(path));
        codebreaker::formats::pe_functions::set_capstone_dll(str(path));
        codebreaker::analyze::dos_functions::set_capstone_dll(str(path));
    }
    Vs::clock() = [] { return 100.0; };
    {
        std::ifstream k("knowledge.txt", std::ios::binary);
        std::string line;
        while (std::getline(k, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            std::vector<std::string> f = split(line, '\t');
            if (f[0] == "I") testkit::identities()[f[1]] = std::vector<std::string>(f.begin() + 2, f.end());
            if (f[0] == "D") testkit::starts()[f[1]].emplace_back(std::stoll(f[2]), std::stoll(f[3]), f.size() > 4 ? f[4] : std::string());
        }
    }
    std::ifstream in(argv[1], std::ios::binary);
    std::string tmp = std::string(argv[1]) + ".capture.tmp";
    std::string line;
    Vs::Store fs;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> f = split(line, '\t');
        const std::string& op = f[0];
        const std::string& label = f.size() > 1 ? f[1] : "";
        auto arg = [&](size_t i) -> std::string { return f.size() > i ? f[i] : std::string(); };
        try {
            if (op == "load") {
                std::shared_ptr<Vs::Side> s;
                try {
                    Tm t("load");
                    s = Vs::load(str(arg(2)));
                } catch (const py::Exception& e) {
                    emit(label, std::string("EXC ") + e.type() + ": " + e.what());
                    continue;
                }
                emit(label, jd(side_value(*s)));
            } else if (op == "match") {
                auto sa = Vs::load(str(arg(2)));
                auto sb = Vs::load(str(arg(3)));
                py::dict<str, std::tuple<str, str>> m;
                {
                    Tm t("match");
                    m = Vs::match(*sa, *sb, std::stod(arg(4)));
                }
                I::Value l = I::Value::list();
                for (const auto& kv : m.items()) {
                    I::Value e = I::Value::list();
                    e.push(I::Value(std::get<0>(kv)));
                    e.push(I::Value(std::get<0>(std::get<1>(kv))));
                    e.push(I::Value(std::get<1>(std::get<1>(kv))));
                    l.push(e);
                }
                emit(label, jd(l));
            } else if (op == "compare") {
                bool listings = arg(5) == "1";
                try {
                    std::optional<Vs::Comparison> c;
                    {
                        Tm t("compare");
                        c.emplace(Vs::compare(str(arg(2)), str(arg(3)), std::stod(arg(4))));
                    }
                    if (label.compare(0, 13, "compare error") == 0) {
                        emit(label, "no error");
                    } else if (listings) {
                        emit(label, jd(c->as_json()) + "\n" + c->report().utf8() + "\n" + jd(counts_value(*c)));
                    } else {
                        emit(label, jd(c->as_json(false)));
                    }
                } catch (const py::Exception& e) {
                    emit(label, std::string("EXC ") + e.type() + ": " + e.what());
                }
            } else if (op == "likeness") {
                auto sa = Vs::load(str(arg(2)));
                auto sb = Vs::load(str(arg(3)));
                emit(label, py::to_str(Vs::likeness(*sa->functions.at(str(arg(4))), *sb->functions.at(str(arg(5))))).utf8());
            } else if (op == "shared") {
                py::list<str> paths;
                for (const auto& p : split(arg(4), ';')) paths.append(str(p));
                std::vector<std::pair<int64_t, Vs::Where>> r;
                {
                    Tm t("shared");
                    r = Vs::shared(paths, str(arg(2)), std::stoll(arg(3)));
                }
                I::Value l = I::Value::list();
                for (const auto& e : r) {
                    I::Value pr = I::Value::list();
                    pr.push(I::Value(e.first));
                    I::Value w = I::Value::dict();
                    for (const auto& pw : e.second) w.set(pw.first, I::Value::from_strings(pw.second));
                    pr.push(w);
                    l.push(pr);
                }
                emit(label, jd(l));
            } else if (op == "carry") {
                auto sa = Vs::load(str(arg(2)));
                (void)sa;
                Vs::Comparison c = Vs::compare(str(arg(2)), str(arg(3)));
                if (label == "no store results") {
                    if (fs) { fs->close(); fs = nullptr; }
                    I::Value all = I::Value::list();
                    all.push(recon_value(c.reconstructions()));
                    auto cn = c.carry_names();
                    all.push(cn ? I::Value(*cn) : I::Value());
                    all.push(I::Value(c.carry()));
                    emit(label, jd(all));
                    continue;
                }
                if (!fs) fs = codebreaker::store::functions::open();
                if (label == "reconstructions") {
                    emit(label, jd(recon_value(c.reconstructions(fs))));
                } else if (label == "carry_names" || label == "carry_names again") {
                    auto cn = c.carry_names(fs);
                    emit(label, jd(cn ? I::Value(*cn) : I::Value()));
                } else if (label == "carry" || label == "carry again") {
                    emit(label, jd(I::Value(c.carry(fs))));
                } else if (label == "store after") {
                    auto id = codebreaker::verify::reconstruct::identity(str(arg(3)));
                    I::Value best = I::Value::dict();
                    for (const auto& kv : fs->best(std::get<0>(id), std::get<1>(id)).items()) {
                        const E::Candidate& c2 = std::get<1>(kv);
                        I::Value row = I::Value::list();
                        row.push(cell_value(c2.klass));
                        row.push(cell_value(c2.engine));
                        row.push(cell_value(c2.name));
                        row.push(cell_value(c2.source));
                        row.push(cell_value(c2.verdict));
                        row.push(cell_value(c2.reason));
                        best.set(std::get<0>(kv), row);
                    }
                    I::Value names = I::Value::dict();
                    for (const auto& kv : fs->names_from_bodies(std::get<0>(id)).items()) {
                        I::Value pr = I::Value::list();
                        pr.push(cell_value(std::get<0>(std::get<1>(kv))));
                        pr.push(cell_value(std::get<1>(std::get<1>(kv))));
                        names.set(std::get<0>(kv), pr);
                    }
                    I::Value all = I::Value::dict();
                    all.set("best", best);
                    all.set("names", names);
                    emit(label, jd(all));
                }
            } else if (op == "product") {
                if (fs) { fs->close(); fs = nullptr; }
                std::vector<std::string> toks = split(label, ' ');
                std::vector<std::string> logs;
                Vs::Product r;
                {
                    Tm t("product");
                    r = Vs::product(str(toks[1]), str(toks[2]), std::stoll(arg(2)), std::stoll(arg(3)), std::stod(arg(4)),
                                    [&](const str& s) { logs.push_back(s.utf8()); });
                }
                I::Value all = I::Value::list();
                all.push(Vs::to_value(r));
                I::Value lv = I::Value::list();
                for (const auto& s : logs) lv.push(I::Value(str(s)));
                all.push(lv);
                emit(label, jd(all));
            } else if (op == "main") {
                if (fs) { fs->close(); fs = nullptr; }
                py::list<str> args;
                for (size_t i = 2; i < f.size(); i++) args.append(str(f[i]));
                std::string text;
                {
                    Tm t("main");
                    text = capture_main(args, tmp);
                }
                emit(label, text);
            } else if (op == "file") {
                emit(label, sha256_of(arg(2)));
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
