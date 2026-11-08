// codebreaker/knowledge/reads.hpp -- typed C++ port of codebreaker.knowledge.reads.
//
// reads.py -- what a step read of the knowledge base, so that a changed piece makes stale exactly the facts made from it.
//
//     {
//         reads::recording r;                   // (the engine does this around every step it runs)
//         auto n = api_arguments::argc("USER32", "FindWindowA");
//         r.keys();                             // {"win32:argument_bytes.header:FindWindowA"}
//     }
//     reads::digest("win32:argument_bytes.header:FindWindowA")    // the piece's hash now ("absent")
//
// A read is named "platform:kind:key" -- the piece's place in the base (knowledge/store.hpp) -- or
// "platform:kind:*" for a whole kind read at once. The accessors that answer from the base record what they
// read (record()): so whatever asks them is recorded with no change of its own.
//
// The engine runs each step inside a recording and adds every read to the fact's inputs as ("kb", read,
// digest): a fact is current only while each piece it read has the hash it had. A piece not there is
// "absent" -- adding it later is a change too.
//
// Work handed to threads is recorded into the asker's recording; work handed to worker processes is run
// inside Carry, which brings each item's reads back with its result.
//
// Notes on the port:
//   * contextvars.ContextVar is a thread-local pointer to the Reads in force (`current()`); to hand work to
//     a thread, the asker's pointer is captured with `capture()` and put in force there with `Adopt`
//     (contextvars.copy_context().run).
//   * recording is the class `with recording() as r` is: construct it, use r.keys() / r.reads(), and the
//     destructor resets the context (the `with` block's end). It is not copyable.
//   * Carry wraps work(context, item): a std::function; Carried is its result with the reads. Work handed
//     to worker *processes* (pickled by reference) has no meaning here: Carry works in-process.
//   * digest()'s _fresh() step calls knowledge.sources, symbols.prototypes, symbols.sdk_structs and
//     symbols.xdk_libraries; the xdk one is called only for a kind of the xbox360 platform, as the Python.
#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <tuple>
#include <vector>

#include "py.hpp"

namespace codebreaker::knowledge::reads {

extern const py::str KIND;      // the engine's name for an input that is a piece of the base: "kb"
extern const py::str ABSENT;

// The reads of one recording.
class Reads {
  public:
    void add(const py::str& key);
    void update(const py::list<py::str>& keys);
    // iter(): the keys, sorted
    py::list<py::str> sorted_keys() const;
    int64_t size() const;
    bool contains(const py::str& key) const;
    // the set itself, as the Python's `.keys`
    py::set<py::str> keys() const;

  private:
    mutable std::mutex lock_;
    py::set<py::str> keys_;
};

// with recording() as r: ... -- every read of the base in the block (and in threads it hands work to) is in
// r.keys(). A recording inside another keeps its own: the outer one does not see the inner's reads.
class recording {
  public:
    recording();
    ~recording();
    recording(const recording&) = delete;
    recording& operator=(const recording&) = delete;
    Reads& reads() { return *reads_; }
    py::set<py::str> keys() const { return reads_->keys(); }
    py::list<py::str> sorted_keys() const { return reads_->sorted_keys(); }

  private:
    std::shared_ptr<Reads> reads_;
    std::shared_ptr<Reads> previous_;
};

// The recording in force in this thread, or null.
std::shared_ptr<Reads> capture();
// Put a captured recording in force for a scope (the asker's context run on a pool's thread).
class Adopt {
  public:
    explicit Adopt(std::shared_ptr<Reads> reads);
    ~Adopt();
    Adopt(const Adopt&) = delete;
    Adopt& operator=(const Adopt&) = delete;

  private:
    std::shared_ptr<Reads> previous_;
};

bool active();

py::str join(const py::str& platform, const py::str& kind, const py::str& key);
std::tuple<py::str, py::str, py::str> split(const py::str& read);

// A piece was read (nothing happens outside a recording).
void record(const py::str& platform, const py::str& kind, const py::str& key);
void record_kind(const py::str& platform, const py::str& kind);

// Reads made elsewhere (a worker process) for the current recording.
void merge(const py::list<py::str>& keys);

// ---- worker processes ----
// A worker's result with the reads it made.
template <class T>
struct Carried {
    T value;
    py::list<py::str> keys;
};

// work(context, item) for jobs/pool.run, its reads brought back with each result.
template <class Context, class Item, class T>
class Carry {
  public:
    explicit Carry(std::function<T(const Context&, const Item&)> work) : work_(std::move(work)) {}
    Carried<T> operator()(const Context& context, const Item& item) const {
        recording r;
        T value = work_(context, item);
        return Carried<T>{value, r.sorted_keys()};
    }

  private:
    std::function<T(const Context&, const Item&)> work_;
};

// A Carry's result: its value, its reads merged into the current recording (or `into`, a callable given them).
template <class T>
T unwrap(const Carried<T>& carried, const std::function<void(const py::list<py::str>&)>& into = nullptr) {
    if (into) into(carried.keys);
    else merge(carried.keys);
    return carried.value;
}

// ---- what a read is now ----
// The hash of a read piece now: its value's hash, ABSENT when it is not there; for a whole kind ("*"), a
// hash of every piece's; for the name store, of what it answers.
py::str digest(const py::str& read);

}  // namespace codebreaker::knowledge::reads
