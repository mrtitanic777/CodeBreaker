// codebreaker/__main__.hpp -- COMPAT SHIM over cb/app/commands.hpp (docs/CPP_DESIGN.md 6.3).
//
// The command table and its dispatch are native/cb/app/commands.{hpp,cpp} now: the catalog of commands (name,
// synopsis, summary), `--corpus`, help, the terminal default and the exit codes. This header keeps what the old ports
// still use -- the old COMMAND_TABLE (command name -> the Python module that runs it, with its fixed arguments), the
// registry of handlers by module name, and `main` -- and runs the old table through the new dispatch: every command
// of the table gets a runner that finds its module's registered handler (a command whose module has no handler raises
// ModuleNotFoundError, as an import that fails does). The usage text (DOC) is the new `help`.
//
// The commands run by "codebreaker_native" (C++ already: native/cbexe/commands.cc) are registered under that name,
// the fixed arguments (the command's name) first. ui.mcp_server's `main` is registered here (the one module this
// table shares a cycle with).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "cb/app/commands.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/ui/mcp_server.hpp"

namespace codebreaker::main_module {

// A COMMANDS value: the module that runs it and the arguments put before the command's own.
struct Command {
    py::str module;
    py::list<py::str> fixed;
};

// importlib.import_module(module).main: a handler registered under the module's dotted name.
using Handler = std::function<int64_t(const py::list<py::str>& argv)>;

namespace compat_detail {

struct Legacy {
    const char* name;
    const char* module;
    std::vector<const char*> fixed;
};

// COMMANDS, in the old table's order (generated from the Python's __main__.COMMANDS).
inline const std::vector<Legacy>& legacy() {
    static const std::vector<Legacy> t = {
    {"module", "codebreaker.source.module_writer", {}},
    {"scoreboard", "codebreaker.source.module_writer", {"--scoreboard"}},
    {"place", "codebreaker.source.placement", {}},
    {"sweep", "codebreaker.verify.sweep", {}},
    {"measure", "codebreaker.verify.measure", {}},
    {"match", "codebreaker.verify.authored.match", {}},
    {"score16", "codebreaker.verify.gate16.scoreboard16", {}},
    {"repair", "codebreaker.verify.authored.repair", {}},
    {"teeth", "codebreaker.verify.teeth", {}},
    {"invert", "codebreaker.invert.run", {}},
    {"exact", "codebreaker.exact.run", {}},
    {"vxd", "codebreaker.source.vxd.run", {}},
    {"ne-asm", "codebreaker.source.ne_asm.run", {}},
    {"build16", "codebreaker.build.period16", {}},
    {"flat-thunk", "codebreaker.build.flat_thunk", {}},
    {"decompile", "codebreaker.decompile.decompiler", {}},
    {"names", "codebreaker.symbols.names", {}},
    {"identify", "codebreaker.symbols.runtime_library", {}},
    {"demangle", "codebreaker.symbols.demangle", {}},
    {"media", "codebreaker.formats.media", {}},
    {"dos-functions", "codebreaker.analyze.dos_functions", {}},
    {"dos-interrupts", "codebreaker.symbols.dos_interrupts", {}},
    {"analyze", "codebreaker.analyze.report", {}},
    {"driver-services", "codebreaker.symbols.driver_services", {}},
    {"driver-source", "codebreaker.source.driver_writer", {}},
    {"dos-source", "codebreaker.source.dos_writer", {}},
    {"dos-build", "codebreaker.source.dos_build", {}},
    {"pe-source", "codebreaker.source.pe_writer", {}},
    {"ne-source", "codebreaker.source.ne_writer", {}},
    {"le-source", "codebreaker.source.le_writer", {}},
    {"origin", "codebreaker.analyze.origin", {}},
    {"boot-floppy", "codebreaker.build.floppy", {}},
    {"driver-build", "codebreaker.build.driver", {}},
    {"driver-judge", "codebreaker.verify.lanes.driver", {}},
    {"build", "codebreaker.build.module", {}},
    {"resources", "codebreaker.build.resources", {}},
    {"cab", "codebreaker.formats.cab", {}},
    {"disc", "codebreaker.build.disc_image", {}},
    {"versions", "codebreaker.compare.versions", {}},
    {"fonts", "codebreaker.formats.fonts", {}},
    {"info", "codebreaker.formats.inspect", {}},
    {"xex", "codebreaker.formats.xex", {}},
    {"coverage", "codebreaker.coverage.ledger", {}},
    {"reach", "codebreaker.coverage.reach", {}},
    {"mcp", "codebreaker.ui.mcp_server", {}},
    {"serve", "codebreaker.ui.serve", {}},
    {"ui", "codebreaker.ui.terminal", {}},
    {"progress", "codebreaker.ui.progress", {}},
    {"settings", "codebreaker.base.settings", {}},
    {"cache", "codebreaker.base.cache", {}},
    {"facts", "codebreaker.engine.show", {}},
    {"knowledge", "codebreaker.knowledge.show", {}},
    {"tidy-state", "codebreaker.base.tidy_state", {}},
    {"recompile", "codebreaker.recompile.powerpc", {}},
    {"recompile-project", "codebreaker.recompile.project", {}},
    {"xenon-recompile", "codebreaker.recompile.xenon", {}},
    {"xdk-match", "codebreaker.symbols.xdk_libraries", {}},
    {"xex-imports", "codebreaker.formats.xex_imports", {}},
    {"xex-edit", "codebreaker.formats.xex_edit", {}},
    {"ppc-calls", "codebreaker.analyze.ppc_calls", {}},
    {"heavyiron", "codebreaker.formats.xbox.heavyiron", {}},
    {"gfx", "codebreaker.formats.gfx", {}},
    {"classes", "codebreaker.symbols.cpp_classes", {}},
    {"system-ui", "codebreaker.formats.xbox.system_ui", {}},
    {"crypto", "codebreaker.formats.xbox.crypto", {}},
    {"java", "codebreaker.java.run", {}},
    {"xna", "codebreaker.xna.run", {}},
    {"xbox-disc", "codebreaker_native", {"xbox-disc"}},
    {"xex-basic", "codebreaker_native", {"xex-basic"}},
    {"xenon-config", "codebreaker_native", {"xenon-config"}},
    {"switch-tables", "codebreaker_native", {"switch-tables"}},
    {"corpus-executables", "codebreaker_native", {"corpus-executables"}},
    };
    return t;
}

inline std::map<std::string, Handler>& handlers() {
    static std::map<std::string, Handler> h = [] {
        std::map<std::string, Handler> m;
        m["codebreaker.ui.mcp_server"] = [](const py::list<py::str>& argv) { return ui::mcp_server::main(argv); };
        return m;
    }();
    return h;
}

inline std::function<bool()>& terminal_hook() {
    static std::function<bool()> f;
    return f;
}

}  // namespace compat_detail

// The usage text.
inline const std::string& DOC_TEXT() {
    static const std::string d = cb::app::CommandTable().usage();
    return d;
}
inline py::str DOC() { return py::str(DOC_TEXT()); }

// COMMANDS, in the Python dict's order.
inline const std::vector<std::pair<py::str, Command>>& COMMAND_TABLE() {
    static const std::vector<std::pair<py::str, Command>> t = [] {
        std::vector<std::pair<py::str, Command>> out;
        for (const compat_detail::Legacy& l : compat_detail::legacy()) {
            Command c{py::str(l.module), {}};
            for (const char* f : l.fixed) c.fixed.append(py::str(f));
            out.emplace_back(py::str(l.name), std::move(c));
        }
        return out;
    }();
    return t;
}
// COMMANDS[name], or nothing.
inline const Command* command(const py::str& name) {
    for (const auto& kv : COMMAND_TABLE())
        if (kv.first == name) return &kv.second;
    return nullptr;
}
// the command names, in order
inline py::list<py::str> command_names() {
    py::list<py::str> out;
    for (const auto& kv : COMMAND_TABLE()) out.append(kv.first);
    return out;
}

inline void register_command(const py::str& module, Handler handler) { compat_detail::handlers()[module.utf8()] = std::move(handler); }
// The module's handler, or null.
inline const Handler* handler(const py::str& module) {
    auto it = compat_detail::handlers().find(module.utf8());
    return it == compat_detail::handlers().end() ? nullptr : &it->second;
}

// find_spec("textual") (the terminal interface's library): set by the program that has it.
inline std::function<bool()>& has_terminal_interface() { return compat_detail::terminal_hook(); }
// stdin and stdout are both terminals, and the interface is there
inline bool terminal_available() {
    return cb::app::stdio_are_terminals() && has_terminal_interface() && has_terminal_interface()();
}

// The command line: -> the exit status.
inline int64_t main(const py::list<py::str>& argv) {
    cb::app::Environment env;
    env.use_corpus = [](const std::string& name) { base::corpus::use(py::str(name)); };
    env.corpus_names = [] { return cb::compat::from_py(base::corpus::names()); };
    env.terminal_interface_available = [] { return cb::app::stdio_are_terminals() && has_terminal_interface() && has_terminal_interface()(); };
    cb::app::CommandTable table(env);
    for (const compat_detail::Legacy& l : compat_detail::legacy()) {
        const py::str module(l.module);
        py::list<py::str> fixed;
        for (const char* f : l.fixed) fixed.append(py::str(f));
        table.set(l.name, [module, fixed](std::span<const std::string> args) -> int {
            const Handler* h = handler(module);
            if (!h) throw py::ModuleNotFoundError("No module named " + py::repr(module).utf8());
            py::list<py::str> all = fixed;
            for (const std::string& a : args) all.append(py::str(a));
            return static_cast<int>((*h)(all));
        });
    }
    return cb::compat::guard([&] { return table.run(cb::compat::from_py(argv)); });
}

// The `codebreaker` command an install provides (sys.exit(main(sys.argv[1:])) in the Python): the status.
inline int64_t _entry(const py::list<py::str>& argv) { return main(argv); }

}  // namespace codebreaker::main_module
