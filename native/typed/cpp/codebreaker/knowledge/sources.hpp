// codebreaker/knowledge/sources.hpp -- C++ of codebreaker.knowledge.sources.
//
// sources.py -- where each kind of the knowledge base comes from, and how it is loaded.
//
//     AUTHORED    files people write and keep in the repository: each is the knowledge, the rows
//                 in the base its copy, loaded again when the file's hash changes
//     authored(platform, kind)      the kind's rows, loaded first when the file changed
//
// The derived and learned kinds are made by the modules that know how (symbols/prototypes.py reads
// the headers, symbols/api_arguments.py an export's code, ...): they keep what they make here
// through store.ensure() and store.put(), and nothing of it is kept anywhere else.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "py.hpp"

namespace codebreaker::knowledge::sources {

// A JSON value (what json.load gives): the one place the Python genuinely mixes types, so it is a
// tagged union spelled as a struct (only the field of `kind` means anything). An integer that does not
// fit int64_t is refused (OverflowError); Python's would be a big int.
struct Json {
    enum Kind { None, Bool, Int, Float, Str, List, Dict };
    Kind kind = None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    py::str s;
    std::vector<Json> list;
    py::dict<py::str, Json> dict;
};

using Rows = py::dict<py::str, Json>;                     // {key: value} of a kind
using Reader = std::function<Rows(const py::str&)>;       // how a kind's rows are read from its file

// The knowledge base as sources.py uses it (store.open()'s object): what authored() and changed() ask of it.
class KnowledgeBase {
  public:
    virtual ~KnowledgeBase() = default;
    // kb.ensure(platform, kind, stamp, make, source): the kind's rows, made again by make() when stamp is new
    virtual Rows ensure(const py::str& platform, const py::str& kind, const py::str& stamp,
                        const std::function<Rows()>& make, const py::str& source) = 0;
    // kb._ensured.pop((platform, kind), None)
    virtual void forget_ensured(const py::str& platform, const py::str& kind) = 0;
};

// store.open(): the process's knowledge base. Whoever links the native base sets this; unset, asking is a RuntimeError.
std::function<std::shared_ptr<KnowledgeBase>()>& store_open();

// _HERE, the folder of this module (codebreaker/knowledge). Python takes it from __file__; here it is
// set_here()'s, else CB_CHECKOUT_ROOT's codebreaker\knowledge, else the working folder's.
py::str _HERE();
void set_here(const py::str& here);
py::str _SYMBOLS();     // codebreaker/symbols
py::str _MODELS();      // _HERE/models

Json _json(const py::str& path);
Reader _signatures(const py::str& bits);
Reader _part(const py::str& name, const std::function<py::str(const py::str&)>& keyed = nullptr);   // nullptr: keys as they are
Rows _whole(const py::str& path);

// (platform, kind) -> (file, how its rows are read from it, what it is)
struct Source {
    py::str file;
    Reader read;
    py::str what;
};
const py::dict<std::tuple<py::str, py::str>, Source>& AUTHORED();

py::str file_stamp(const py::str& path);
py::dict<py::str, py::str>& _stamps();
// A file of AUTHORED was written by this process: its kinds are loaded again at the next ask.
void changed(const py::str& path);
// {key: value} of an authored kind, loaded from its file first when the file changed (kb null: store.open()).
Rows authored(const py::str& platform, const py::str& kind, const std::shared_ptr<KnowledgeBase>& kb = nullptr);

}  // namespace codebreaker::knowledge::sources
