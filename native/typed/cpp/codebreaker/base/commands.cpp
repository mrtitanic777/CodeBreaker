// codebreaker/base/commands.cpp -- C++17 port of codebreaker.base.commands (see commands.hpp).
#include "codebreaker/base/commands.hpp"

namespace codebreaker::base::commands {

py::list<py::str> sys_argv;

static ModuleRunner g_runner;

void set_module_runner(ModuleRunner runner) {
    g_runner = std::move(runner);
}

// `finally: sys.argv = saved`
struct ArgvRestore {
    py::list<py::str> saved;
    ~ArgvRestore() { sys_argv = saved; }
};

int64_t run_module(const py::str& module, const py::list<py::str>& arguments) {
    ArgvRestore restore{sys_argv};
    py::list<py::str> argv;
    argv.append(module);
    for (const auto& a : arguments) argv.append(a);
    sys_argv = argv;
    try {
        if (!g_runner) throw ImportError("No module named " + module.utf8());
        g_runner(module);
    } catch (const SystemExit& e) {
        if (const int64_t* code = std::get_if<int64_t>(&e.code)) return *code;
        return std::holds_alternative<std::monostate>(e.code) ? 0 : 1;
    }
    return 0;
}

int64_t dispatch(const py::dict<py::str, py::str>& tools, const py::list<py::str>& argv, const py::str& doc) {
    if (argv.size() == 0 || !tools.contains(argv[0])) {
        py::print(doc);
        py::print(py::str("tools: ") + py::join(py::str(", "), py::sorted(tools.keys())));
        return 2;
    }
    return run_module(tools.at(argv[0]), argv.slice_from(1));
}

}  // namespace codebreaker::base::commands
