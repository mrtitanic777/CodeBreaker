// cb/translate/judge_py_cpp_command.hpp -- `codebreaker judge-py-cpp`: Python -> C++17 judged by running both.
//
//     codebreaker judge-py-cpp [--cxx gcc|clang|msvc|PATH] [--keep] [--work DIR] [FILE.py ...]
//
// A program is a Python module whose zero-argument functions named `run_*` each give a JSON-able answer (the form of the snippet
// translate/testdata/cpp17/semantics.*): a number, text, a bool, a list or tuple, a dict, or the exception class it ends with. Each goes
// through the whole path and is scored at each step:
//
//     lowers     the Python front end (inference, lowering) translated every run_* function (what else it refused is only listed)
//     writes     the L2 core's C++17 back end (translate/l2) wrote the module
//     compiles   the host C++ compiler built it with the runtime (data/translate/runtime/cpp17: py.hpp ...) and a main that prints each answer
//     agrees     CPython and the C++ gave the same answer for every run_* function
//
// The result is a table, one row per module (the same as judge-cs-cpp's), and the exit code is 0 when every module agrees. The default
// corpus is tests/translate/corpus/python, one module per file. Needs a `python` on the PATH (or in the CB_PYTHON environment variable).
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/translate/judge_cs_cpp_command.hpp"

namespace cb::translate::judge_py_cpp {

using Environment = judge_cs_cpp::Environment;
using Score = judge_cs_cpp::Score;

// Judge one module file in `work` (made new).
Score judge_module(const Environment& environment, const judge_cs_cpp::Cxx& cxx, const std::filesystem::path& file, const std::filesystem::path& work);

cli::Command command(const Environment& environment);

}  // namespace cb::translate::judge_py_cpp
