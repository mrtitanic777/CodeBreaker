// codebreaker/engine/show.hpp -- typed C++ port of codebreaker.engine.show.
//
// show.py -- what a binary's database holds, and why.
//
//     python -m codebreaker facts <binary>                  every kind: how many facts, their size, who made them
//     python -m codebreaker facts <binary> <kind>           each fact of the kind: its key, hash, size, when made,
//                                                           whether its step's version is the version of now
//     python -m codebreaker facts <binary> <kind> <key>     one fact: its value (in short) and what it was made from
//     python -m codebreaker facts <binary> <kind> [key] --make      made current first (the engine runs)
//     python -m codebreaker facts --databases               every database, its size, and the cap (db_max_gb)
//
// <binary> is a path or a corpus name. Nothing is made unless --make is given.
//
// When the CodeBreaker service runs (ui/serve) it answers (its databases open, its step versions
// known; what is printed comes when it is done); else the command answers itself, here.
//
// All signatures are plain-typed except `_short`, which takes a fact's value (bytes, str or JSON: facts::Value).
#pragma once

#include <cstdint>

#include "codebreaker/engine/facts.hpp"
#include "py.hpp"

namespace codebreaker::engine::show {

// `n` bytes as "1.5 MB" or "12.0 KB"
py::str _mb(int64_t n);

// A value in a line or a few.
py::str _short(const facts::Value& v, int64_t width = 100);

// A time as "2026-11-11 02:41:19 AM" in local time, "-" for none (0).
py::str _when(double t);

// python -m codebreaker facts ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::engine::show
