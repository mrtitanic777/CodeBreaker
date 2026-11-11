// codebreaker/knowledge/show.hpp -- typed C++ port of codebreaker.knowledge.show.
//
// show.py -- what the knowledge base holds, and where each piece came from.
//
//     python -m codebreaker knowledge                          every platform's kinds
//     python -m codebreaker knowledge <platform>               ... of one platform
//     python -m codebreaker knowledge <platform> <kind>        its keys (the first 200)
//     python -m codebreaker knowledge <platform> <kind> <key>  one piece: its value, source, origin
//     python -m codebreaker knowledge --load                   load every authored file first
//
// Typing notes:
//   * main(argv) takes the command line after "knowledge" (a list of str) and returns the exit status;
//     what it prints goes through py::print (stdout). It opens the process's knowledge base
//     (knowledge::store::open(): CB_KNOWLEDGE_DB, else state/db/knowledge.sqlite).
//   * `knowledge.platform` and `knowledge.table` are the package's (knowledge/package).
//   * A value is facts::Value (bytes | str | JSON). json.dumps(value, indent=1, default=repr) is dumps():
//     JSON as json.dumps writes it with indent 1 (ensure_ascii, NaN/Infinity), bytes as the JSON string
//     of their repr (what default=repr does).
//   * time.strftime("%Y-%m-%d %I:%M %p", time.localtime(made)) is the C library's strftime on the local time.
#pragma once

#include <cstdint>
#include <string>

#include "codebreaker/engine/facts.hpp"
#include "py.hpp"

namespace codebreaker::knowledge::show {

// json.dumps(value, indent=1, default=repr)
py::str dumps(const engine::facts::Value& value);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::knowledge::show
