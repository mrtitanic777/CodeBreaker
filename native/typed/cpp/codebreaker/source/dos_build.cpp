// codebreaker/source/dos_build.cpp -- see dos_build.hpp.

#include "codebreaker/source/dos_build.hpp"

#include "py_os.hpp"

namespace codebreaker::source::dos_build {

std::function<py::str(const py::str& folder)> rebuild;

const py::str DOC(
    "python -m codebreaker dos-build <source folder> [--against ORIGINAL]\n"
    "\n"
    "Make a real-mode program from the folder dos-source wrote, without the original.");

int64_t main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    py::list<py::str> args;
    for (const py::str& a : argv)
        if (!a.startswith(S("--"))) args.append(a);
    std::optional<py::str> against;
    if (argv.contains(S("--against"))) against = argv[argv.index(S("--against")) + 1];
    if (against) {
        py::list<py::str> kept;
        for (const py::str& a : args)
            if (a != *against) kept.append(a);
        args = kept;
    }
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    if (!rebuild) throw py::RuntimeError("dos_writer.rebuild is not set");
    py::str made = rebuild(args[0]);
    if (against && !against->empty()) {                            // `if against:` (an empty name is none)
        bool same = py::open_rb(made).read() == py::open_rb(*against).read();
        py::print(py::percent(S("%s: %s"), made, same ? py::percent(S("THE SAME as %s"), *against) : py::percent(S("differs from %s"), *against)));
        return same ? 0 : 1;
    }
    py::print(made);
    return 0;
}

}  // namespace codebreaker::source::dos_build
