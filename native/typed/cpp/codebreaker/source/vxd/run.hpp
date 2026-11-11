// codebreaker/source/vxd/run.hpp -- compat shim for cb/source/vxd/run.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer.
//
// The `vxd` command: its help text is DOC; the tools are the modules of TOOLS, run by the caller's RunModule.
#pragma once

#include <functional>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/run.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::run {

// The module's docstring: shown when the tool name is missing or unknown.
inline const py::str DOC = cb::compat::to_py(cb::source::vxd::kVxdUsage);

// The tool table: tool name -> the module that runs it.
inline const py::dict<py::str, py::str> TOOLS = {
    {py::str("idiom"), py::str("codebreaker.source.vxd.idiom")},
    {py::str("tree"), py::str("codebreaker.source.vxd.tree")},
    {py::str("masm"), py::str("codebreaker.source.vxd.masm")},
    {py::str("link"), py::str("codebreaker.source.vxd.link")},
    {py::str("from-source"), py::str("codebreaker.source.vxd.from_source")},
    {py::str("owbuild"), py::str("codebreaker.source.vxd.owbuild")},
    {py::str("finalize"), py::str("codebreaker.source.vxd.coalesce")},
    {py::str("disassembly"), py::str("codebreaker.source.vxd.disassembly")},
    {py::str("corpus"), py::str("codebreaker.source.vxd.corpus")},
    {py::str("loadable-corpus"), py::str("codebreaker.source.vxd.loadable_corpus")},
    {py::str("boot"), py::str("codebreaker.source.vxd.boot")},
};

// "run `module` as `python -m module arguments...`" is a Python facility (runpy); the caller hands it in.
// Returns the module's exit code.
using RunModule = std::function<int64_t(const py::str& module, const py::list<py::str>& arguments)>;

// argv[0] names one of TOOLS, the rest are its arguments. With no tool or an unknown one: prints DOC and the sorted
// tool names to stdout, returns 2.
inline int64_t main(const py::list<py::str>& argv, const RunModule& run_module) {
    cb::base::commands::ToolTable tools;
    for (const auto& kv : TOOLS.items()) {
        const py::str module = std::get<1>(kv);
        tools[cb::compat::from_py(std::get<0>(kv))] = [module, &run_module](std::span<const std::string> args) {
            py::list<py::str> rest;
            for (const std::string& a : args) rest.append(cb::compat::to_py(a));
            return static_cast<int>(run_module(module, rest));
        };
    }
    const std::vector<std::string> args = cb::compat::from_py(argv);
    return cb::source::vxd::run(args, tools);
}

}  // namespace codebreaker::source::vxd::run
