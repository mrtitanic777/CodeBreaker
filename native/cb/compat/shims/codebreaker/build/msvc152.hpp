// codebreaker/build/msvc152.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/msvc152.hpp for the ports that still use the old
// API (source/ne_asm/segdiff). Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: DOC, Compiled, compile_c, main. The old module's two settings values DBX and MSVC (read when the program started, assignable)
// are the functions DBX_() and MSVC_() here, read when first asked: nothing in the typed tree named them, and a namespace-scope
// variable that reads settings at start-up fails wherever the settings hook is not installed (the test program). main() keeps the old command line (`cc16 [--flags FLAGS] [--selftest] [csrc]`), parsed by
// cb::cli rather than the argparse look-alike: -h/--help, --flags, --selftest, one positional; abbreviated long options are not
// accepted.
//
// DOS programs run through OldDosRunner (dos_runner_adapter.hpp) -- COMPAT(py): remove when build/dos_tools is refactored.
#pragma once

#include <iostream>
#include <optional>

#include "cb/build/msvc152.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/fs.hpp"
#include "dos_runner_adapter.hpp"
#include "../../../../../ported/include/py_settings.hpp"

namespace codebreaker::build::msvc152 {

inline py::str& DBX_() {
    static py::str v = [] { const py::str s = py::settings::get_str(py::str("dosbox_x")); return s.empty() ? py::str("dosbox-x") : s; }();
    return v;
}
inline py::str& MSVC_() {
    static py::str v = py::settings::get_str(py::str("msvc152"));
    return v;
}
inline const py::str DOC("cc16.py -- compile 16-bit C with the PERIOD compiler (MSVC 1.52c) under DOSBox-X.\n");

struct Compiled {
    std::optional<py::str> work;   // the folder it ran in (kept for inspection; the caller cleans it up)
    std::optional<py::bytes> obj;
    std::optional<py::str> cod;    // the /Fc listing
    std::optional<py::str> log;    // the raw compiler log
};

// Compile csrc with MSVC 1.52c under DOSBox-X. Raises SystemExit with the text when the tools are missing, as the old one did.
inline Compiled compile_c(const py::str& csrc, const py::str& flags = py::str("/AM /c"), const py::str& stem = py::str("t"),
                          double timeout = 180) {
    static const OldDosRunner runner;
    const cb::build::msvc152::Tools tools{cb::fs::from_text(DBX_().utf8()), cb::fs::from_text(MSVC_().utf8())};
    try {
        const cb::build::msvc152::Compiled c = cb::build::msvc152::compile_c(
            runner, tools, csrc.utf8(), flags.utf8(), stem.utf8(), std::chrono::seconds(static_cast<long long>(timeout)));
        Compiled out;
        if (c.work) out.work = py::str(cb::fs::to_text(*c.work));
        if (c.obj) out.obj = cb::compat::to_py(cb::ByteView(*c.obj));
        if (c.cod) out.cod = py::str(*c.cod);
        if (c.log) out.log = py::str(*c.log);
        return out;
    } catch (const cb::NotFoundError& e) {
        throw py::SystemExit(py::str(e.what()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

// The command line (the SystemExit it raises is the Python's, and the result is 0 when it returns)
inline int64_t main(const py::list<py::str>& argv, std::ostream& out = std::cout, std::ostream& err = std::cerr) {
    namespace cli = cb::cli;
    cli::Command command{"cc16", "compile 16-bit C with the period compiler",
                         {cli::Positional{"csrc", "C source file", false}, cli::Option{"--flags", "", "compiler flags", cli::Arg::Value, "FLAGS", "/AM /c"},
                          cli::Option{"--selftest", "", "compile a trivial function", cli::Arg::Flag}},
                         nullptr};
    const cli::Cli parser("cc16", {command});
    std::vector<std::string> args{"cc16"};
    for (int64_t i = 1; i < argv.size(); ++i) args.push_back(argv[i].utf8());
    cli::Cli::Parsed parsed;
    try {
        parsed = parser.parse(args);
    } catch (const cb::UsageError& e) {
        err << "cc16: error: " << e.what() << "\n";
        throw py::SystemExit(2);
    }
    if (parsed.help) {
        out << parser.help(*parsed.command, "cc16");
        throw py::SystemExit(0);
    }
    const std::string flags = parsed.args.value_or("flags", "/AM /c");
    std::string text;
    if (parsed.args.flag("selftest")) {
        text = std::string(cb::build::msvc152::kSelfTestSource);
    } else if (const auto csrc = parsed.args.value("csrc")) {
        text = cb::fs::read_text(cb::fs::from_text(*csrc));
    } else {
        throw py::SystemExit(py::str("give a C file or --selftest"));
    }
    const Compiled c = compile_c(py::str(text), py::str(flags));
    cb::build::msvc152::Compiled report;
    if (c.work) report.work = cb::fs::from_text(c.work->utf8());
    if (c.obj) report.obj = cb::compat::from_py(*c.obj);
    if (c.cod) report.cod = c.cod->utf8();
    if (c.log) report.log = c.log->utf8();
    out << cb::build::msvc152::report(report);
    return 0;
}

}  // namespace codebreaker::build::msvc152
