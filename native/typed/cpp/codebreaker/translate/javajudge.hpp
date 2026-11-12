// codebreaker/translate/javajudge.hpp -- COMPAT SHIM over cb/translate/javajudge.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/javajudge.{hpp,cpp}: the judge, given what it needs of the rest of CodeBreaker as small
// interfaces. This header is those interfaces over the old ports that still provide them -- the java lane (verify/lanes/java),
// the probe (java/probe), the whole-program back ends (backends/cpp17, backends/c_java) and the Java front end a host gives
// with set_java_frontend() -- and keeps the old entry points: judge_tree, judge_program, lower_program, build and the verdict
// words. The units the front end lowers are old trees; the judge reads them as typed nodes (the old tree's L2 document) and the
// back ends are handed the old trees they were written for.
//
// What the old file held that nothing outside it used (the harness's formats and helpers, probe_methods ...) is the new
// module's now, and is reached there.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <tuple>
#include <vector>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/javajudge.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/java/probe.hpp"
#include "codebreaker/translate/backends/c_java.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::javajudge {

namespace ir = codebreaker::translate::ir;
namespace jj = cb::translate::javajudge;

inline const py::str EQUIVALENT = py::str("equivalent"), DIVERGENT = py::str("divergent"), REFUSED = py::str("refused"),
                     NOT_RUN = py::str("not-run");

// what the host gives of codebreaker.translate.frontends.java
struct JavaFrontend {
    // every .java compilation unit under src_dir as javac's typed tree: {path relative to out_dir without .json: tree}
    std::function<py::dict<py::str, ir::Dict>(const py::str& src_dir, const py::str& out_dir, const std::optional<py::str>& release)> dump;
    // javac's errors and warnings of the last dump into out_dir: [{'kind': 'ERROR' | ..., 'message': ...}, ...]
    std::function<py::list<ir::Dict>(const py::str& out_dir)> diagnostics;
};
inline JavaFrontend& java_frontend_() {
    static JavaFrontend f;
    return f;
}
inline void set_java_frontend(const JavaFrontend& fe) { java_frontend_() = fe; }

// {class: (verdict, gaps, detail)}
using TreeVerdicts = py::dict<py::str, std::tuple<py::str, int64_t, py::str>>;
using Log = std::function<void(const py::str&)>;

// verdicts word of a lane verdict, by gaps
inline py::str _word(const py::str& verdict, int64_t) {
    namespace V = codebreaker::verify::verdicts;
    if (verdict == V::NO_COMPILE) return REFUSED;
    if (verdict == V::DIVERGENT) return DIVERGENT;
    if (V::GOOD().contains(verdict)) return EQUIVALENT;
    return NOT_RUN;
}

// (javac, java); RuntimeError without a JDK
inline std::tuple<py::str, py::str> java_tools() {
    namespace lane = codebreaker::verify::lanes::java;
    auto [javac, java] = lane::jdk();
    if (javac.empty()) throw py::RuntimeError("no JDK (settings.json: java)");
    return {javac, java};
}

// the host C++ compiler; RuntimeError when there is none
inline py::str _host_cxx() {
    py::str cxx = codebreaker::verify::lanes::java::compiler();
    if (cxx.empty()) throw py::RuntimeError("no C++ compiler (settings.json: host_cxx)");
    return cxx;
}

// The Java sources under `src` through javac's tree into L2. -> [ir.Unit]
inline py::list<ir::NodeP> lower_program(const py::str& src, const py::str& work, const std::optional<py::str>& release = std::nullopt) {
    namespace jlower = codebreaker::translate::frontends::java::lower;
    const JavaFrontend& fe = java_frontend_();
    if (!fe.dump || !fe.diagnostics) throw py::ImportError("codebreaker.translate.frontends.java: no front end given (set_java_frontend)");
    const py::str tree = py::os::path::join(work, py::str("tree"));
    py::dict<py::str, ir::Dict> units = fe.dump(src, tree, release);
    for (const ir::Dict& d : fe.diagnostics(tree)) {
        const ir::Val& kind = d.at(py::str("kind"));
        const py::str* k = std::get_if<py::str>(&kind.v);
        if (k && *k == py::str("ERROR")) {
            const ir::Val& msg = d.at(py::str("message"));
            const py::str* m = std::get_if<py::str>(&msg.v);
            throw py::RuntimeError((py::str("javac: ") + (m ? (m->size() > 300 ? m->slice(0, 300) : *m) : py::str(""))).utf8());
        }
    }
    py::list<py::str> keys;
    for (const auto& kv : units.items()) keys.append(std::get<0>(kv));
    keys.sort();
    py::list<ir::NodeP> out;
    for (const py::str& p : keys) out.append(jlower::lower(units.at(p), std::optional<py::str>(p)));
    return out;
}

// Write a whole program with `backend` and compile it. -> (exe or None, why)
inline std::tuple<std::optional<py::str>, py::str> build(const py::list<ir::NodeP>& units, const py::str& work, const py::str& backend) {
    namespace cpp17 = codebreaker::translate::backends::cpp17;
    namespace c_java = codebreaker::translate::backends::c_java;
    const py::str out = py::os::path::join(work, backend);
    if (py::os::path::isdir(out)) py::shutil::rmtree(out);
    py::os::makedirs(out);
    if (backend == py::str("cpp17")) {
        const py::str text = cpp17::program(units);
        const py::str src = py::os::path::join(out, py::str("program.cpp"));
        cb::fs::write_text(cb::fs::from_text(src.utf8()), text.utf8());
        const py::str cxx = _host_cxx();
        const py::str exe = py::os::path::join(out, py::str("program.exe"));
        const std::string compiler = cxx.utf8();
        std::vector<std::string> argv = {compiler};
        for (const py::str& f : cpp17::FLAGS) argv.push_back(f.utf8());
        for (const std::string& a : {std::string("-I"), cpp17::RUNTIME.utf8(), src.utf8(), std::string("-o"), exe.utf8()}) argv.push_back(a);
        cb::process::ProcessSpec spec;
        spec.argv = argv;
        spec.timeout = std::chrono::seconds(600);
        const cb::process::ProcessResult r = cb::process::run(spec);
        if (r.exit_code != 0) {
            const std::string err = cb::text::sanitize_utf8(cb::as_text(r.err));
            std::string first = err.substr(0, 300);
            for (const std::string_view l : cb::text::lines(err)) {
                if (l.find("error") != std::string_view::npos) {
                    first = std::string(l);
                    break;
                }
            }
            return {std::nullopt, py::str("does not compile: ") + cb::compat::to_py(first.substr(0, 400))};
        }
        return {std::optional<py::str>(exe), py::str("")};
    }
    if (backend == py::str("c")) return c_java::build(units, out);
    throw py::ValueError("no whole-program back end " + py::repr(backend).utf8());
}

namespace shim_detail {

namespace lane = codebreaker::verify::lanes::java;
namespace V = codebreaker::verify::verdicts;
namespace probe = codebreaker::java::probe;

// the java lane as the tree judge's interface
class OldLane final : public jj::TreeLane {
public:
    std::vector<std::string> prepare(const std::filesystem::path& src, const std::filesystem::path& work, jj::Backend backend) override {
        // (the lane's module variables are the Python's: set for the run, put back after -- see judge_tree below)
        m_ = lane::prepare(cb::compat::to_py(cb::fs::to_text(src)), cb::compat::to_py(cb::fs::to_text(work)));
        built_ = lane::build(m_, cb::compat::to_py(cb::fs::to_text(src)), cb::compat::to_py(cb::fs::to_text(work)));
        (void)backend;
        std::vector<std::string> names;
        for (const py::str& n : m_.order) {
            if (built_.items.contains(n)) names.push_back(n.utf8());
        }
        return names;
    }
    jj::LaneResult judge(const std::string& cls) override {
        const py::str name = cb::compat::to_py(cls);
        lane::Verdict v = lane::judge(built_.context, built_.items.at(name));
        const py::str& word = std::get<0>(v);
        jj::LaneResult out;
        out.reason = std::get<1>(v).utf8();
        out.detail = std::get<3>(v).utf8();
        if (word == V::NO_COMPILE) out.verdict = jj::LaneVerdict::NoCompile;
        else if (word == V::DIVERGENT) out.verdict = jj::LaneVerdict::Divergent;
        else if (V::GOOD().contains(word)) out.verdict = jj::LaneVerdict::Good;
        return out;
    }
    std::size_t gaps(const std::string& cls) override {
        const py::str name = cb::compat::to_py(cls);
        return m_.texts.contains(name) ? static_cast<std::size_t>(m_.texts.at(name).count(py::str("J2X-GAP:"))) : 0;
    }

private:
    lane::Prepared m_;
    lane::Built built_;
};

// the old trees as typed nodes (their L2 documents), made in `arena`
inline std::vector<cb::translate::ir::Node*> typed(cb::Arena& arena, const py::list<ir::NodeP>& old_units) {
    std::vector<cb::translate::ir::Node*> out;
    for (const ir::NodeP& u : old_units) out.push_back(cb::translate::l2::from_json(arena, cb::compat::to_json_value(l2::to_json(u))));
    return out;
}

}  // namespace shim_detail

// Every class of the Java tree `src` converted by the transpiler with the body back end `backend` ('cpp03' or 'cpp17') and
// judged by the java lane (probe.py). `only`: just these classes; `workers`: threads judging at once.
inline TreeVerdicts judge_tree(const py::str& src, const py::str& work, const py::str& backend = py::str("cpp03"),
                               const std::optional<py::set<py::str>>& only = std::nullopt, int64_t workers = 2, const Log& log_in = nullptr) {
    namespace lane = shim_detail::lane;
    namespace probe = shim_detail::probe;
    const bool cpp17 = backend == py::str("cpp17");
    if (!cpp17 && backend != py::str("cpp03")) throw py::KeyError(py::repr(backend).utf8());
    const py::str label = py::str(cpp17 ? "C++17" : "C++03"), std_ = py::str(cpp17 ? "c++17" : "c++03");
    // (lane.CPP_STD, lane.STD, lane.FLAGS, probe.CXX_STD are module variables the Python reassigns and puts back)
    py::str& lane_std = const_cast<py::str&>(lane::STD);
    py::str& lane_cpp_std = const_cast<py::str&>(lane::CPP_STD);
    const py::str saved_cpp_std = lane_cpp_std, saved_std = lane_std, saved_cxx = probe::CXX_STD;
    const py::list<py::str> saved_flags = lane::FLAGS.copy();
    struct Restore {
        std::function<void()> f;
        ~Restore() { f(); }
    } restore{[&] {
        lane_cpp_std = saved_cpp_std;
        lane_std = saved_std;
        probe::CXX_STD = saved_cxx;
        lane::FLAGS.clear();
        lane::FLAGS.extend(saved_flags);
    }};
    lane_cpp_std = label;
    lane_std = std_;
    probe::CXX_STD = std_;
    {
        py::list<py::str> flags{py::str("-std=") + std_};
        for (const py::str& f : saved_flags) {
            if (!f.startswith(py::str("-std="))) flags.append(f);
        }
        lane::FLAGS.clear();
        lane::FLAGS.extend(flags);
    }
    jj::TreeOptions options;
    options.workers = static_cast<unsigned>(workers < 1 ? 1 : workers);
    if (only) {
        options.only.emplace();
        for (const py::str& n : *only) options.only->push_back(n.utf8());
    }
    options.log = [&](std::string_view line) {
        const py::str text = cb::compat::to_py(line);
        if (log_in) {
            log_in(text);
        } else {
            std::fwrite(line.data(), 1, line.size(), stdout);
            std::fputc('\n', stdout);
        }
    };
    shim_detail::OldLane old_lane;
    const auto judged = jj::judge_tree(old_lane, cb::fs::from_text(src.utf8()), cb::fs::from_text(work.utf8()),
                                       cpp17 ? jj::Backend::Cpp17 : jj::Backend::Cpp03, options);
    TreeVerdicts out;
    for (const auto& [cls, j] : judged) {
        out.set(cb::compat::to_py(cls), std::make_tuple(cb::compat::to_py(jj::word(j.verdict)), static_cast<int64_t>(j.gaps), cb::compat::to_py(j.detail)));
    }
    return out;
}

// what a whole program's judgement found
struct ProgramVerdict {
    py::str verdict = NOT_RUN;
    int64_t methods = 0;
    int64_t cases = 0;
    // (method, values, the JVM's result, the other's)
    py::list<std::tuple<py::str, py::list<py::str>, py::str, py::str>> differs;
    std::optional<py::dict<py::str, py::str>> main;
    py::str why;
    py::str backend;
};

// A whole Java program (a folder of sources, default package) beside the JVM, translated by a whole-program back end
// ('cpp17' or 'c'). `log` is unused (the Python's, too).
inline ProgramVerdict judge_program(const py::str& src, const py::str& work, const py::str& backend = py::str("cpp17"), const Log& = nullptr) {
    namespace lane = shim_detail::lane;
    namespace probe = shim_detail::probe;
    jj::ProgramHost host;
    {
        auto [javac, java] = lane::jdk();
        if (!javac.empty()) host.jdk = cb::translate::dotnet::judge::Jdk{cb::fs::from_text(javac.utf8()), cb::fs::from_text(java.utf8())};
    }
    host.probe.cases = [](const std::vector<std::string>& types) {
        std::vector<std::vector<std::string>> out;
        for (const py::list<py::str>& c : probe::cases(cb::compat::to_py(types))) out.push_back(cb::compat::from_py(c));
        return out;
    };
    host.probe.literal = [](const std::string& type, const std::string& value) {
        return probe::_java_literal(cb::compat::to_py(type), cb::compat::to_py(value)).utf8();
    };
    host.probe.results = [](std::string_view output) {
        cb::OrderedMap<std::string, std::string> out;
        for (const auto& kv : probe::_results(cb::compat::to_py(output)).items()) out.insert_or_assign(std::get<0>(kv).utf8(), std::get<1>(kv).utf8());
        return out;
    };
    host.probe.same = [](const std::string& type, const std::string& a, const std::string& b) {
        return probe::same(cb::compat::to_py(type), cb::compat::to_py(a), cb::compat::to_py(b));
    };
    py::list<ir::NodeP> last_lowered;   // (the old trees the back end is handed: what the front end gave last)
    host.lower = [&](cb::Arena& arena, const std::filesystem::path& dir, const std::filesystem::path& out_dir) {
        last_lowered = lower_program(cb::compat::to_py(cb::fs::to_text(dir)), cb::compat::to_py(cb::fs::to_text(out_dir)));
        return shim_detail::typed(arena, last_lowered);
    };
    host.build = [&](std::span<cb::translate::ir::Node* const>, const std::filesystem::path& dir, std::string_view backend_name) {
        auto [exe, why] = build(last_lowered, cb::compat::to_py(cb::fs::to_text(dir)), cb::compat::to_py(backend_name));
        jj::Built built;
        if (exe) built.exe = cb::fs::from_text(exe->utf8());
        built.why = why.utf8();
        return built;
    };
    const jj::ProgramVerdict r = jj::judge_program(host, cb::fs::from_text(src.utf8()), cb::fs::from_text(work.utf8()), backend.utf8());
    ProgramVerdict out;
    out.verdict = cb::compat::to_py(jj::word(r.verdict));
    out.methods = static_cast<int64_t>(r.methods);
    out.cases = static_cast<int64_t>(r.cases);
    for (const jj::Difference& d : r.differs) {
        out.differs.append(std::make_tuple(cb::compat::to_py(d.method), cb::compat::to_py(d.values), cb::compat::to_py(d.java), cb::compat::to_py(d.native)));
    }
    if (r.main) out.main = cb::compat::to_py(*r.main);
    out.why = cb::compat::to_py(r.why);
    out.backend = cb::compat::to_py(r.backend);
    return out;
}

}  // namespace codebreaker::translate::javajudge
