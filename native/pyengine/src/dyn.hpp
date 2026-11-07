// dyn.hpp -- pyengine's dynamic mode: Python modules to C++ over py::var (native/pyrt), for the code
// the typed path cannot prove types for. Every construct lowers to the runtime's operations; only what
// cannot run without an interpreter is refused (exec / eval of runtime strings, frame introspection,
// async), and each refusal is kept with its reason and the module's line.
//
//     pyengine --dynamic --root <repo> --dyn-out <dir> [--dyn-main <module>] [--dyn-closure] <module>...
//
// writes <dir>/<module path>.cpp per module, <dir>/main.cpp (python -m <module>), <dir>/CMakeLists.txt
// (linking native/pyrt) and <dir>/dynamic.json (per module: lines, functions, refusals).
#pragma once
#include <string>
#include <vector>

namespace pyengine {

struct DynOptions {
    std::string root;                    // the folder module names are found under (the repo)
    std::vector<std::string> modules;    // dotted names (a package: its __init__.py)
    std::string out;
    std::string main_module;             // the module main.cpp runs as __main__
    bool closure = false;                // follow imports of the root's own modules (at any depth)
    std::vector<std::string> exclude;    // dotted prefixes never followed
    bool top_only = false;               // closure: module-level imports only ...
    std::vector<std::string> full_closure;   // ... but every import of these packages' modules
    std::string pyrt;                    // native/pyrt (for the CMakeLists); default <root>/native/pyrt
    std::string exe = "program";         // the executable's name
    std::string stdlib_dir;              // CPython's Lib folder: the modules native/pyrt/stdlib.txt names are translated from it
    std::string site_dir;                // site-packages: the outside packages native/pyrt/site.txt names (unicorn, capstone)
    std::string typed_dir;               // --dyn-typed DIR: the typed path's output (--out DIR --cpp DIR/cpp); its proven
                                         // functions are typed islands (islands.hpp). Empty: none (the output is unchanged)
    std::string islands_file;            // --dyn-islands FILE: the judge's verdicts; only the islands it agreed are written
    std::string native_typed_dir;        // --dyn-native-typed DIR: modules replaced by their typed C++ (native_modules.hpp)
    std::string native_dir;              // --dyn-native DIR: the program's own C++ (DIR/*.cc, compiled into the executable:
                                         // its modules registered with py_native.hpp's register_native_module)
};

struct DynRefusal {
    int line = 0;
    std::string what;
};
struct DynModule {
    std::string name, file, cpp;
    bool package = false;
    int lines = 0, functions = 0;
    std::vector<DynRefusal> refused;
    std::string error;                   // a module that did not parse
};

int run_dynamic(const DynOptions& o, std::vector<DynModule>& out);

}  // namespace pyengine
