// codebreaker/base/commands.hpp -- SHIM over cb/base/commands.hpp (docs/CPP_DESIGN.md 6.3). No logic of its own beyond
// the module-by-name idea the new code does not have: deleted with its last py:: consumer.
//
// Python's run_module starts a module by name as `python -m module`. C++ has no import-by-name, so the modules a program
// can run are registered with set_module_runner(): one function that, given a module name, runs it as that module's
// main, reading its command line from sys_argv (what sys.argv is in Python) and leaving through SystemExit like
// sys.exit(). The new commands::dispatch takes tools as functions; here each tool is "run that module".
#pragma once

#include <functional>
#include <span>
#include <string>
#include <variant>
#include <vector>

#include "cb/base/commands.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/log.hpp"

namespace codebreaker::base::commands {

using py::ImportError;
using py::SystemExit;

// sys.argv, for the running module to read
inline py::list<py::str> sys_argv;

// Runs `module` as its own main (python -m module): reads sys_argv, throws SystemExit to exit.
using ModuleRunner = std::function<void(const py::str& module)>;

inline ModuleRunner& module_runner_() {
    static ModuleRunner runner;
    return runner;
}

inline void set_module_runner(ModuleRunner runner) { module_runner_() = std::move(runner); }

// Run `module` as if started with `python -m module arguments...`. -> its exit code.
inline int64_t run_module(const py::str& module, const py::list<py::str>& arguments) {
    struct Restore {   // `finally: sys.argv = saved`
        py::list<py::str> saved;
        ~Restore() { sys_argv = saved; }
    } restore{sys_argv};
    py::list<py::str> argv;
    argv.append(module);
    for (const auto& a : arguments) argv.append(a);
    sys_argv = argv;
    try {
        if (!module_runner_()) throw ImportError("No module named " + module.utf8());
        module_runner_()(module);
    } catch (const SystemExit& e) {
        if (const int64_t* code = std::get_if<int64_t>(&e.code)) return *code;
        return std::holds_alternative<std::monostate>(e.code) ? 0 : 1;
    }
    return 0;
}

// argv[0] names one of `tools` {name: module}; the rest are its arguments.
inline int64_t dispatch(const py::dict<py::str, py::str>& tools, const py::list<py::str>& argv,
                        const py::str& doc = py::str("")) {
    cb::base::commands::ToolTable table;
    for (const auto& entry : tools.items()) {
        const py::str module = std::get<1>(entry);
        table[cb::compat::from_py(std::get<0>(entry))] = [module](std::span<const std::string> args) {
            py::list<py::str> rest;
            for (const std::string& a : args) rest.append(cb::compat::to_py(a));
            return static_cast<int>(run_module(module, rest));
        };
    }
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
    const int code = cb::base::commands::dispatch(table, args, cb::compat::from_py(doc));
    cb::flush_out();   // (the old layer prints through its own stream: keep the two in order)
    return code;
}

}  // namespace codebreaker::base::commands
