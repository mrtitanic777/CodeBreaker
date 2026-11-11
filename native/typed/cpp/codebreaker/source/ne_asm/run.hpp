// codebreaker/source/ne_asm/run.hpp -- SHIM over cb/source/ne_asm/run.hpp (docs/CPP_DESIGN.md 6.3). The tool table maps a tool name to
// the Python module that runs it; `main` hands the module name to `run_module`, which the caller supplies (base/commands has no typed
// port). Deleted with its last py:: consumer.
#pragma once

#include <functional>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/ne_asm/run.hpp"

namespace codebreaker::source::ne_asm::run {

// The module's help: shown when the tool name is missing or unknown.
inline const py::str DOC = cb::compat::to_py(cb::source::ne_asm::run::usage());

// The tool table: tool name -> the module that runs it.
inline const py::dict<py::str, py::str> TOOLS = {
    {py::str("idiom"), py::str("codebreaker.source.ne_asm.idiom")},
    {py::str("sweep"), py::str("codebreaker.source.ne_asm.sweep")},
    {py::str("recon"), py::str("codebreaker.source.ne_asm.recon")},
    {py::str("segdiff"), py::str("codebreaker.source.ne_asm.segdiff")},
    {py::str("reachability"), py::str("codebreaker.source.ne_asm.reachability")},
    {py::str("datacode"), py::str("codebreaker.source.ne_asm.datacode")},
    {py::str("segment"), py::str("codebreaker.source.ne_asm.segment")},
    {py::str("fixups"), py::str("codebreaker.source.ne_asm.fixups")},
    {py::str("debug-names"), py::str("codebreaker.symbols.debug_build")},
    {py::str("debug-names-estimate"), py::str("codebreaker.symbols.debug_build_estimate")},
    {py::str("msvc152"), py::str("codebreaker.build.msvc152")},
};

using RunModule = std::function<int64_t(const py::str& module, const py::list<py::str>& arguments)>;

// argv[0] names one of TOOLS, the rest are its arguments. With no tool or an unknown one: prints the help and the sorted tool names,
// returns 2.
inline int64_t main(const py::list<py::str>& argv, const RunModule& run_module) {
    return cb::compat::guard([&]() -> int64_t {
        cb::base::commands::ToolTable tools;
        for (const auto& [tool, module] : TOOLS.items()) {
            tools[cb::compat::from_py(tool)] = [module, &run_module](std::span<const std::string> args) {
                py::list<py::str> rest;
                for (const std::string& a : args) rest.append(cb::compat::to_py(a));
                return static_cast<int>(run_module(module, rest));
            };
        }
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return cb::source::ne_asm::run::main(tools, args);
    });
}

}  // namespace codebreaker::source::ne_asm::run
