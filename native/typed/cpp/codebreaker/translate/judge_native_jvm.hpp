// codebreaker/translate/judge_native_jvm.hpp -- SHIM over cb/translate/judge_native_jvm.hpp (docs/CPP_DESIGN.md 6.3): the judge for C/C++ -> Java.
// The old types in (py::str, py::list, the old Verdict and Run, the old front end given by set_clang_frontend), the new judge called, the old types out.
// What the new judge asks of its host is answered from the old layer: the JDK of verify/lanes/java, the runtime folder of the old Java back end, the
// state folder of the settings, the front end given here, and for the Java back end and the Java round trip the OLD ports (backends/java is not a shim
// yet, its idiomatic tier and library mapping still live there; java/transpile and the Java runtime lift are the old ports), reached by L2 JSON. Deleted
// with its last py:: consumer.
//
//     Verdict v = judge({"prog.c"}, work_dir);
//
// Differences from the old port that the shim cannot hide:
//   * A clang front end that was not given is a RuntimeError at the first call, as before; a clang that is not found is a failed build (the old port took
//     the None for an executable).
//   * The bytes in a difference are quoted with C escapes, not as Python's repr; a run that timed out has the word 'timeout' for its code as before.
//   * A front end that cannot read a file raises the old RuntimeError, carried as a tool error and back.
#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <variant>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/translate/judge_native_jvm.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/java/transpile.hpp"
#include "codebreaker/translate/backends/java.hpp"
#include "codebreaker/translate/frontends/clang/j2x.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::translate::judge_native_jvm {

namespace ir = codebreaker::translate::ir;
namespace jback = codebreaker::translate::backends::java;
namespace cbj = cb::translate::judge_native_jvm;

inline constexpr int64_t RUN_TIMEOUT = 120;

// what the host gives of codebreaker.translate.frontends.clang
struct ClangFrontend {
    // the clang executable, or None
    std::function<std::optional<py::str>()> clang;
    // 'c' for a .c file, else 'c++'
    std::function<py::str(const py::str& path)> language_of;
    // clang's command-line options for parsing (and the judge's compiling) a source file
    std::function<py::list<py::str>(const py::str& language, const std::optional<py::str>& std_, const py::list<py::str>& includes,
                                    const py::list<py::str>& quote_includes, const py::list<py::str>& defines)>
        flags;
    // one source file as an L2 document (a dict): the file's declarations lowered, cached by the source and the lowering
    std::function<ir::Dict(const py::str& path, const py::str& language, const std::optional<py::str>& std_, const py::list<py::str>& includes,
                           const py::list<py::str>& quote_includes, const py::list<py::str>& defines, const std::optional<py::list<py::str>>& own)>
        l2_file;
};

namespace shim_ {
inline ClangFrontend& frontend() {
    static ClangFrontend fe;   // what the host gave: the old layer's one front end
    return fe;
}
}  // namespace shim_
inline void set_clang_frontend(const ClangFrontend& fe) { shim_::frontend() = fe; }

// subprocess.TimeoutExpired
struct TimeoutExpired : py::Exception {
    TimeoutExpired() : py::Exception("timed out") {}
    explicit TimeoutExpired(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "TimeoutExpired"; }
};

// what a run gave: (stdout, stderr, returncode); the code is an int, or the word 'timeout'
struct Run {
    py::bytes out;
    py::bytes err;
    std::variant<int64_t, py::str> code;
};

struct Verdict {
    py::str name;
    py::str verdict;
    py::str reason;
    std::optional<py::str> tier;
    std::optional<py::str> java;
    std::optional<Run> native;
    std::optional<Run> jvm;
    py::list<py::str> notes;

    Verdict(const py::str& name, const py::str& verdict, const py::str& reason = py::str(""), const std::optional<py::str>& tier = std::nullopt,
            const std::optional<py::str>& java = std::nullopt, const std::optional<Run>& native = std::nullopt, const std::optional<Run>& jvm = std::nullopt,
            const py::list<py::str>& notes = py::list<py::str>())
        : name(name), verdict(verdict), reason(reason), tier(tier), java(java), native(native), jvm(jvm), notes(notes) {}

    // Verdict(name: verdict -- reason)
    py::str repr() const { return py::str("Verdict(") + name + py::str(": ") + verdict + (reason.empty() ? py::str("") : py::str(" -- ") + reason) + py::str(")"); }
    // {'program': name, 'verdict': ..., 'reason': ..., 'tier': tier}
    py::dict<py::str, std::optional<py::str>> row() const {
        py::dict<py::str, std::optional<py::str>> out;
        out.set(py::str("program"), std::optional<py::str>(name));
        out.set(py::str("verdict"), std::optional<py::str>(verdict));
        out.set(py::str("reason"), std::optional<py::str>(reason));
        out.set(py::str("tier"), tier);
        return out;
    }
};

namespace shim_ {

inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }
inline std::vector<std::string> strings(const py::list<py::str>& l) { return cb::compat::from_py(l); }

inline cbj::Run new_run(const Run& r) {
    cbj::Run out{cb::compat::from_py(r.out), cb::compat::from_py(r.err), std::nullopt};
    if (const int64_t* code = std::get_if<int64_t>(&r.code)) out.exit_code = *code;
    return out;
}
inline Run old_run(const cbj::Run& r) {
    return Run{cb::compat::to_py(cb::ByteView(r.out)), cb::compat::to_py(cb::ByteView(r.err)),
               r.exit_code ? std::variant<int64_t, py::str>(*r.exit_code) : std::variant<int64_t, py::str>(py::str("timeout"))};
}

inline Verdict old_verdict(const cbj::Verdict& v) {
    return Verdict(cb::compat::to_py(v.name), cb::compat::to_py(std::string(cbj::word(v.verdict))), cb::compat::to_py(v.reason),
                   v.tier ? std::optional<py::str>(cb::compat::to_py(*v.tier)) : std::nullopt, v.java ? std::optional<py::str>(cb::compat::to_py(*v.java)) : std::nullopt,
                   v.native ? std::optional<Run>(old_run(*v.native)) : std::nullopt, v.jvm ? std::optional<Run>(old_run(*v.jvm)) : std::nullopt, cb::compat::to_py(v.notes));
}

// An old node of the L2 tree <-> a new one, through the JSON both read and write.
inline cb::translate::ir::Node* new_unit(cb::Arena& arena, const ir::Dict& doc) { return cb::translate::l2::from_json(arena, cb::compat::to_json_value(doc)); }
inline ir::NodeP old_node(const cb::translate::ir::Node& node) {
    const ir::Val v = cb::compat::from_json_value(cb::translate::l2::to_json(node));
    return l2::from_json(std::get<ir::Dict>(v.v));
}

inline std::string repr_of(const py::dict<py::str, int64_t>& d) {
    std::string out = "{";
    for (const auto& kv : d.items()) out += std::format("{}'{}': {}", out.size() > 1 ? ", " : "", cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
    return out + "}";
}

// The host the new judge asks its questions of, answered from the old layer.
inline cbj::Host host() {
    const ClangFrontend fe = frontend();
    if (!fe.clang || !fe.language_of || !fe.flags || !fe.l2_file) throw py::RuntimeError("the clang front end was not given (set_clang_frontend)");
    cbj::Host h;
    const auto [javac, java_] = codebreaker::verify::lanes::java::jdk();
    h.jdk = {path_of(javac), path_of(java_)};
    h.runtime_sources = path_of(jback::RUNTIME());
    h.state_folder = path_of(py::os::path::join(py::settings::get_str(py::str("state")), py::str("translate")));
    h.clang = [fe]() -> std::optional<std::filesystem::path> {
        const auto c = fe.clang();
        return c ? std::optional<std::filesystem::path>(path_of(*c)) : std::nullopt;
    };
    h.language_of = [fe](const std::filesystem::path& p) { return cb::compat::from_py(fe.language_of(cb::compat::to_py(cb::fs::to_text(p)))); };
    h.flags = [fe](const std::string& language, const std::optional<std::string>& std_, const std::vector<std::string>& includes, const std::vector<std::string>& quote,
                   const std::vector<std::string>& defines) {
        return cb::compat::from_py(fe.flags(cb::compat::to_py(language), std_ ? std::optional<py::str>(cb::compat::to_py(*std_)) : std::nullopt, cb::compat::to_py(includes),
                                            cb::compat::to_py(quote), cb::compat::to_py(defines)));
    };
    h.lower = [fe](cb::Arena& arena, const cbj::LoweringRequest& r) -> cb::translate::ir::Node* {
        std::optional<py::list<py::str>> own;
        if (r.own) {
            py::list<py::str> files;
            for (const auto& f : *r.own) files.append(cb::compat::to_py(cb::fs::to_text(f)));
            own = files;
        }
        try {
            return new_unit(arena, fe.l2_file(cb::compat::to_py(cb::fs::to_text(r.path)), cb::compat::to_py(r.language),
                                              r.standard ? std::optional<py::str>(cb::compat::to_py(*r.standard)) : std::nullopt, cb::compat::to_py(r.includes),
                                              cb::compat::to_py(r.quote_includes), cb::compat::to_py(r.defines), own));
        } catch (const py::RuntimeError& e) {   // the front end could not read the file
            throw cb::ToolError("clang front end", 1, "", e.what());
        }
    };
    h.write_java = [](std::span<cb::translate::ir::Node* const> units, const std::string& name) {
        py::list<ir::NodeP> old;
        for (const cb::translate::ir::Node* u : units) old.append(old_node(*u));
        try {
            const jback::Program p = jback::program(old, cb::compat::to_py(name));
            cb::translate::backends::java::Program out;
            out.name = cb::compat::from_py(p.name);
            out.source = cb::compat::from_py(p.source);
            out.tier = cb::compat::from_py(p.tier) == "idiomatic" ? cb::translate::backends::java::Tier::Idiomatic : cb::translate::backends::java::Tier::Faithful;
            out.notes = cb::compat::from_py(p.notes);
            return out;
        } catch (const jback::Refused& e) {
            throw cb::translate::backends::java::Refused(e.what());
        }
    };
    h.java_to_cpp = [](const std::filesystem::path& src, const std::filesystem::path& cpp) -> std::string {
        try {
            const codebreaker::java::transpile::Summary s = codebreaker::java::transpile::convert(cb::compat::to_py(cb::fs::to_text(src)), cb::compat::to_py(cb::fs::to_text(cpp)),
                                                                                                std::nullopt, nullptr, false);
            const py::dict<py::str, int64_t> declined = std::get<1>(s.tree_bodies);
            return declined.empty() ? std::string() : repr_of(declined);
        } catch (const py::Exception& e) {   // the old lane's error, named as before
            throw cb::ToolError("java lane", 1, "", std::string(e.type()) + ": " + e.what());
        }
    };
    h.lift_java_runtime = [](cb::Arena& arena, std::span<cb::translate::ir::Node* const> units) {
        py::list<py::ref<ir::Unit>> old;
        for (const cb::translate::ir::Node* u : units) old.append(std::dynamic_pointer_cast<ir::Unit>(old_node(*u)));
        const py::list<py::ref<ir::Unit>> lifted = codebreaker::translate::frontends::clang::j2x::lift(old);
        std::size_t i = 0;
        for (const auto& u : lifted) {
            const ir::Val v = ir::Val(l2::to_json(ir::NodeP(u)));
            *units[i++] = *cb::translate::l2::from_json(arena, cb::compat::to_json_value(v));   // changed in place, as the lift changed the old nodes
        }
    };
    return h;
}

}  // namespace shim_

// the folder of crt compiled once (cached by its sources and the JDK)
inline py::str runtime_classes() {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::fs::to_text(cbj::runtime_classes(shim_::host()))); });
}

// the program's original, as the original of the judge builds it: clang -O0 -> (ok, the end of clang's message)
inline std::tuple<bool, py::str> native(const py::list<py::str>& files, const py::str& out_exe, const py::str& language, const std::optional<py::str>& std_ = std::nullopt,
                                        const py::list<py::str>& defines = py::list<py::str>(), const py::list<py::str>& includes = py::list<py::str>()) {
    return cb::compat::guard([&] {
        std::vector<std::filesystem::path> paths;
        for (const py::str& f : files) paths.push_back(shim_::path_of(f));
        const cbj::Compiled c = cbj::build_native(shim_::host(), paths, shim_::path_of(out_exe), cb::compat::from_py(language),
                                                  std_ ? std::optional<std::string>(std_->utf8()) : std::nullopt, shim_::strings(defines), shim_::strings(includes));
        return std::make_tuple(c.ok, cb::compat::to_py(c.message));
    });
}

// The program's files -> jback::Program (raises jback::Refused / py::RuntimeError).
inline jback::Program translate(const py::list<py::str>& files, const py::str& name, const std::optional<py::str>& language = std::nullopt,
                                const std::optional<py::str>& std_ = std::nullopt, const py::list<py::str>& defines = py::list<py::str>(),
                                const py::list<py::str>& includes = py::list<py::str>()) {
    try {
        cb::Arena arena;
        std::vector<std::filesystem::path> paths;
        for (const py::str& f : files) paths.push_back(shim_::path_of(f));
        const cb::translate::backends::java::Program p =
            cbj::translate(shim_::host(), arena, paths, cb::compat::from_py(name), language ? std::optional<std::string>(language->utf8()) : std::nullopt,
                           std_ ? std::optional<std::string>(std_->utf8()) : std::nullopt, shim_::strings(defines), shim_::strings(includes));
        return jback::Program(cb::compat::to_py(p.name), cb::compat::to_py(p.source), cb::compat::to_py(std::string(cb::translate::backends::java::to_text(p.tier))),
                              cb::compat::to_py(p.notes));
    } catch (const cb::translate::backends::java::Refused& e) {
        throw jback::Refused(e.what());
    } catch (const cb::Error& e) {
        throw py::RuntimeError(e.what());
    }
}

// A source file's IR, through the front end's cached L2 document (keyed by the source and the lowering).
inline ir::NodeP _unit(const py::str& path, const py::str& language, const std::optional<py::str>& std_, const py::list<py::str>& includes, const py::list<py::str>& quote_includes,
                       const py::list<py::str>& defines, const std::optional<py::list<py::str>>& own = std::nullopt) {
    const ClangFrontend& fe = shim_::frontend();
    if (!fe.l2_file) throw py::RuntimeError("the clang front end was not given (set_clang_frontend)");
    return l2::from_json(fe.l2_file(path, language, std_, includes, quote_includes, defines, own));
}

inline Verdict judge(const py::list<py::str>& files, const py::str& work, const std::optional<py::str>& name = std::nullopt, const std::optional<py::str>& language = std::nullopt,
                     const std::optional<py::str>& std_ = std::nullopt, const py::list<py::str>& defines = py::list<py::str>(), const py::list<py::str>& includes = py::list<py::str>(),
                     const py::bytes& stdin_ = py::bytes(), const py::list<py::str>& args = py::list<py::str>()) {
    return cb::compat::guard([&] {
        std::vector<std::filesystem::path> paths;
        for (const py::str& f : files) paths.push_back(shim_::path_of(f));
        cbj::JudgeOptions options;
        if (name) options.name = name->utf8();
        if (language) options.language = language->utf8();
        if (std_) options.standard = std_->utf8();
        options.defines = shim_::strings(defines);
        options.includes = shim_::strings(includes);
        options.stdin_data = cb::compat::from_py(stdin_);
        options.args = shim_::strings(args);
        return shim_::old_verdict(cbj::judge(shim_::host(), paths, shim_::path_of(work), options));
    });
}

// Java -> C++ -> Java, judged (see the module's header): -> Verdict (java: the Java written; notes: what was translated).
inline Verdict roundtrip(const py::list<py::str>& java_files, const py::str& main_class, const py::str& work, const py::str& name = py::str("RoundTripped")) {
    return cb::compat::guard([&] {
        std::vector<std::filesystem::path> paths;
        for (const py::str& f : java_files) paths.push_back(shim_::path_of(f));
        return shim_::old_verdict(cbj::roundtrip(shim_::host(), paths, cb::compat::from_py(main_class), shim_::path_of(work), cb::compat::from_py(name)));
    });
}

// run(cmd, stdin, cwd, timeout) -> (stdout, stderr, returncode); a timeout is (b'', b'(timed out)', 'timeout')
inline Run _run(const py::list<py::str>& cmd, const py::bytes& stdin_, const py::str& cwd, int64_t timeout = RUN_TIMEOUT) {
    return cb::compat::guard([&] {
        return shim_::old_run(cbj::run_program(shim_::strings(cmd), cb::compat::from_py(stdin_), shim_::path_of(cwd), std::chrono::seconds(timeout)));
    });
}

// replace "\r\n" by "\n"
inline py::bytes _norm(const py::bytes& b) { return b.replace(py::bytes("\r\n"), py::bytes("\n")); }

// None when the two runs agree, else what differs first.
inline std::optional<py::str> _compare(const Run& a, const Run& b) {
    const auto found = cbj::compare(shim_::new_run(a), shim_::new_run(b));
    return found ? std::optional<py::str>(cb::compat::to_py(*found)) : std::nullopt;
}

inline py::str _first_error(const py::str& text) { return cb::compat::to_py(cbj::first_error(cb::compat::from_py(text))); }

inline py::str _class_name(const py::str& stem) { return cb::compat::to_py(cbj::class_name(cb::compat::from_py(stem))); }

}  // namespace codebreaker::translate::judge_native_jvm
