// codebreaker/knowledge/store.hpp -- typed C++ port of codebreaker.knowledge.store.
//
// store.py -- the knowledge base's database: one SQLite file, every platform's knowledge keyed by platform.
//
//     auto kb = store::open();                                  // state/db/knowledge.sqlite (CB_KNOWLEDGE_DB)
//     kb->get("win32", "prototype", "GetDlgItem");              // a value, or None
//     kb->put("win32", "argument_bytes.code", "USER32.FindWindowA", 8, "user32.dll's code");
//     kb->table("win16", "prototype");                          // {key: value} of a kind, read once per process
//     kb->ensure_values("win32", "prototype", stamp, make, source);   // a whole kind made again when its stamp changes
//
// A piece of knowledge is (platform, kind, key, value), kept the way the program database keeps a fact
// (engine/facts.hpp: a value is bytes, text, JSON, with the SHA-1 of it), and with where it came from:
//
//     source     what it was read or worked out from: a file of the repository, the headers, a module's
//                code, a person
//     origin     authored   loaded from a file people write: the file is the knowledge, the rows its copy,
//                           made again when the file changes
//                derived    worked out from inputs outside the repository (the compiler's headers): made
//                           again when their stamp changes
//                learned    found once by a step and kept: written once, seen by every later ask
//                given      given by hand (a body named); never dropped
//
// A whole kind that is authored or derived is made in one go by ensure(): the stamp it was made under is
// kept in `loaded`, and when it differs the kind's authored/derived rows are made again -- learned and given
// rows stay.
//
// Bodies (the name store) are kept here too: `body` -- a function's body digest in a module, with the name
// the module itself gives it -- and `body_name` -- a name given to a body, which holds in every module with
// that body.
//
// The knowledge base is C++ (native/engine: engine/native_engine.hpp's KnowledgeBase), values decoded on
// this side.
//
// Notes on the port:
//   * Opening this header makes the process's knowledge base what knowledge::sources::store_open() hands
//     out (sources.hpp left that hook to "whoever links the native base"): see the initializer in the .cpp.
#pragma once

#include <memory>
#include <optional>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::knowledge::store {

extern const py::str AUTHORED, DERIVED, LEARNED, GIVEN;

py::str default_path();

// The knowledge base of the state folder a function store is in (a test's own store in a folder of its own
// has a knowledge base of its own).
py::str beside(const py::str& store_path);

// The knowledge base in the file `path`: the C++ one's.
std::shared_ptr<engine::native_engine::KnowledgeBase> KnowledgeBase(const py::str& path);

// The knowledge base in `path` (default: default_path()), one per process.
std::shared_ptr<engine::native_engine::KnowledgeBase> open(const std::optional<py::str>& path = std::nullopt);

}  // namespace codebreaker::knowledge::store
