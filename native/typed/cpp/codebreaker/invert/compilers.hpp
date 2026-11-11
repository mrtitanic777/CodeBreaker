// codebreaker/invert/compilers.hpp -- C++17 port of codebreaker.invert.compilers.
//
// compilers.py -- the compilers a model is checked against, as the models call them.
//
// A model of a code generator is only as good as its agreement with the compiler itself. Each
// model has a battery of small functions; this compiles them with the real compiler and hands
// back its bytes. The compilers are build/period.py's -- this module only gives them the shapes
// the models were written against.
//
//     compilers::available()                 Microsoft C 6 is set up (settings: dosbox_x, msc6)
//     compilers::mc6_compile(text, flags)    the code bytes Microsoft C 6 gives for one source text
//     compilers::mc6_compile_batch(jobs)     the same for [(text, flags)], one emulator session per flag set
//     compilers::msvc2()                     the 32-bit compiler of 1994: .compile_c(text, flags) and .Obj
//     compilers::mc_compile(text)            Microsoft C 2.03, MC1 then MC2 (settings: kits.msc203 -- the
//                                            folder holding them): the object file's bytes, or None
//
// Typing notes:
//   * mc6_compile_batch answers one std::optional<py::bytes> per job (None: the compiler gave no object).
//   * msvc2() answers the Msvc2 object; its Obj class is the nested `Obj`; compile_c's dict {"obj": bytes} or
//     {"err": text} is the record CompileResult (one of the two is set).
//   * build.period has no typed port: mc_compile does what period.get("msc203") + Msc203.object do, through
//     the typed exact::toolchain (kit_dir / load / compile) -- the same checks, the same RuntimeError.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::compilers {

bool available();

// [(text, flags)] -> [code bytes or None], in order.
py::list<std::optional<py::bytes>> mc6_compile_batch(const py::list<std::tuple<py::str, py::str>>& jobs, int64_t timeout = 900);

py::bytes mc6_compile(const py::str& csrc, const py::str& flags = py::str("/AM /G2 /Gsw /Ox"), int64_t timeout = 300);

class Obj {
  public:
    explicit Obj(const py::bytes& data_) : data(data_) {}
    py::bytes data;
    // {symbol: (bytes, [positions the linker fills in])}
    py::dict<py::str, std::tuple<py::bytes, py::list<int64_t>>> functions() const;
};

// {"obj": the object file's bytes} or {"err": what the compiler said}
struct CompileResult {
    std::optional<py::bytes> obj;
    std::optional<py::str> err;
};

class Msvc2 {
  public:
    using Obj = compilers::Obj;
    explicit Msvc2(const py::str& cl_);
    py::str cl;
    py::dict<py::str, py::str> env;
    py::str work;
    CompileResult compile_c(const py::str& text, const py::str& flags = py::str("/c /O1 /Oy-")) const;
};

Msvc2 msvc2();

// Compile one source text through MC1 and MC2 (build/period.py). -> the .OBJ's bytes, or None
std::optional<py::bytes> mc_compile(const py::str& csrc, const py::str& stem = py::str("cand"), int64_t timeout = 120);

// Compile a battery of probe functions in one run. -> the .OBJ's bytes, or None
std::optional<py::bytes> mc_compile_multi(const py::str& csrc);

}  // namespace codebreaker::invert::compilers
