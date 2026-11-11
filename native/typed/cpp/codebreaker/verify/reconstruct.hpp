// codebreaker/verify/reconstruct.hpp -- interface of codebreaker.verify.reconstruct (typed C++; implementation pending).
//
// reconstruct.py -- the path from a module to judged reconstructions of its functions.
//
//     auto [sha, profile] = reconstruct::run(module_path);        // results are in the function store
//
// Decompile the module, build the functions that have no verdict yet for the text they have now, run each beside the
// original on the same inputs, and record what was found. The path is the same whatever the module is; what differs
// between kinds of module (32-bit or 16-bit Windows, PowerPC, drivers, real-mode programs, Java) is one lane per kind,
// which in Python was a module with KIND, profile(), prepare(), build(), judge() (and optionally sha(), PER_FUNCTION,
// LIBRARY, SOURCE_REV).
//
// Typing notes:
//   * A lane "module" duck-typed by attribute lookup becomes the abstract class `Kind` below; each lane
//     (verify/lanes/win32.py, win16.py, powerpc.py, driver.py, dos.py, java ...) is one subclass, registered under its
//     KIND name with register_kind().  KINDS (name -> the lane's module path in Python) is `kinds()`: the registered names.
//   * What prepare() returns (header, texts, order, key, all_keys, sig + the lane's own extras) is `Prepared`; a lane's
//     own extras ride in `lane` (opaque to this module; the lane casts it back).  Likewise `Context` and `Item` are the
//     lane's: opaque shared pointers, since reconstruct only hands them from build() to judge().
//   * A judge's answer is (verdict, reason, limits, detail): `Verdict` (the same tuple as verdicts::NotRun).
//   * `signatures(m, whole, own)`: `m` is a Prepared; `m.texts` / `m.header` may be missing in the Python (getattr) -> empty.
//   * `run`'s fs is the function store (engine::native_engine::FunctionStore) or None (opened); `log` defaults to print.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::reconstruct {

// who a candidate in the store came from
extern const py::str SOURCE;   // "decompiler"
// the judge's words (verify/verdicts.py)
const verdicts::NotRun& NOT_RUN();
const py::list<py::str>& GOOD();

// (verdict, reason, limits, detail) -- what a lane's judge says of one function
using Verdict = verdicts::NotRun;

// The decompiled module a lane prepares.
struct Prepared {
    py::str header;                               // the C header every text is built after
    py::dict<py::str, py::str> texts;             // {name: C text}; empty for a module that is not made of separate texts
    py::list<py::str> order;                      // function names, by address
    py::dict<py::str, py::str> key;               // {name: address key}
    py::list<py::str> all_keys;                   // every function's key (also those the decompiler produced nothing for)
    py::str sig;                                  // what the whole preparation was made from
    std::shared_ptr<void> lane;                   // the lane's own extras (opaque here)
};

using Context = std::shared_ptr<void>;            // the lane's build context handed to every judge
using Item = std::shared_ptr<void>;               // one function's thing to judge

// build()'s answer: (context, {name: item to judge}, {name: why it was not built})
struct Built {
    Context context;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> dropped;
};

// One kind of module: what a lane provides.
class Kind {
  public:
    virtual ~Kind() = default;
    // the name results are filed under
    virtual py::str KIND() const = 0;
    // (digest, settings): everything but the source a verdict depends on (module_path "" : the settings in force)
    virtual std::tuple<py::str, py::str> profile(const py::str& module_path) const = 0;
    // the decompiled module
    virtual Prepared prepare(const py::str& module_path, const py::str& work) const = 0;
    virtual Built build(const Prepared& m, const py::str& module_path, const py::str& work) const = 0;
    // runs in a worker process
    virtual Verdict judge(const Context& context, const Item& item) const = 0;
    // optional hooks (the Python getattr defaults)
    virtual std::optional<py::str> sha(const py::str& module_path) const { (void)module_path; return std::nullopt; }
    virtual bool PER_FUNCTION() const { return false; }   // getattr(kind, "PER_FUNCTION", False)
    virtual bool LIBRARY() const { return true; }         // it has runtime-library functions to identify
    virtual std::optional<py::str> SOURCE_REV() const { return std::nullopt; }   // default: decompiler::REV
};

// the lanes of every platform (name -> its Kind)
void register_kind(const py::str& name, const std::shared_ptr<Kind>& kind);
py::dict<py::str, py::str> kinds();               // KINDS: {kind name: lane module path in the Python}; here its registered names
std::shared_ptr<Kind> kind_named(const py::str& name);

// Which kind a file is: 'pe', 'ne', 'powerpc', 'le', 'dos', 'java' -- or None when nothing reconstructs it.
std::optional<py::str> kind_name(const py::str& module_path);
// raises ValueError ("no way to reconstruct a ... file yet") when kind_name is None
std::shared_ptr<Kind> kind_of(const py::str& module_path);

// {kind: digest of the verifier settings in force now} (None for a kind whose tools are not set up on this machine)
py::dict<py::str, std::optional<py::str>> digests();

// (content hash, profile digest) -- what results for this module are filed under.
std::tuple<py::str, py::str> identity(const py::str& module_path);

// What a module's results are filed under: the kind's own `sha`, else the file's sha256.
py::str module_sha(const Kind& kind, const py::str& module_path);

// Names in `built` that call a dropped function (now a stand-in), directly or through others.
// `dropped`: the dropped names (the Python passes a dict of {name: why}; only the names are read).
py::set<py::str> _reaches(const py::dict<py::str, py::str>& texts, const py::list<py::str>& built,
                          const py::dict<py::str, py::str>& dropped);

// {name: what that function's verdict was reached for}: its own text, the header, and the text of every function it can
// reach.  `whole`: the signature for a module not made of separate texts.  With `own` {name: digest of the function's
// compiled code}, that stands in the signature in place of the text and the header.
py::dict<py::str, py::str> signatures(const Prepared& m, const std::optional<py::str>& whole,
                                      const std::optional<py::dict<py::str, py::str>>& own = std::nullopt);

// Reconstruct and judge a module. -> (module hash, profile digest).
// fs: None -> functions::open(); log: where the one-line report goes (default print).
std::tuple<py::str, py::str> run(const py::str& module_path,
                                 const std::shared_ptr<engine::native_engine::FunctionStore>& fs = nullptr,
                                 const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::verify::reconstruct
