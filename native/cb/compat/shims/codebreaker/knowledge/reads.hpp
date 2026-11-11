// codebreaker/knowledge/reads.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/knowledge/reads.hpp for the ports that still
// speak the old py:: layer. No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's
// build is wired for C++20 (see the commit message); it replaces native/typed/cpp/codebreaker/knowledge/reads.hpp then.
//
// The old API is kept: knowledge::reads::{KIND, ABSENT, Reads, recording, capture, Adopt, active, join, split, record,
// record_kind, merge, Carried, Carry, unwrap, digest}. `digest()` is the one thing the new module does not own the
// answer to (it asks the knowledge store and the symbols modules, which are not refactored yet): `digest_impl()` is
// where the old reads.cpp installs the answer it always gave.
#pragma once

#include <functional>
#include <memory>
#include <set>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/knowledge/reads.hpp"

namespace codebreaker::knowledge::reads {

inline const py::str KIND = py::str(std::string(cb::knowledge::kReadInputKind));   // "kb"
inline const py::str ABSENT = py::str(std::string(cb::knowledge::kAbsent));       // "absent"

namespace detail {
inline std::vector<cb::knowledge::Read> reads_of(const py::list<py::str>& keys) {
    std::vector<cb::knowledge::Read> out;
    for (const py::str& k : keys) {
        auto r = cb::knowledge::Read::parse(k.utf8());
        if (!r) throw py::ValueError("not enough values to unpack");   // the old split() of a key with fewer than two colons
        out.push_back(std::move(*r));
    }
    return out;
}
inline py::list<py::str> keys_of(const std::vector<cb::knowledge::Read>& reads) {
    py::list<py::str> out;
    for (const auto& r : reads) out.append(py::str(r.text()));
    return out;
}
}  // namespace detail

// The reads of one recording (the new object, seen as the old class).
class Reads {
  public:
    explicit Reads(std::shared_ptr<cb::knowledge::Reads> inner = std::make_shared<cb::knowledge::Reads>())
        : inner_(std::move(inner)) {}
    void add(const py::str& key) { update(py::list<py::str>{key}); }
    void update(const py::list<py::str>& keys) { inner_->merge(detail::reads_of(keys)); }
    py::list<py::str> sorted_keys() const { return detail::keys_of(inner_->sorted()); }
    int64_t size() const { return static_cast<int64_t>(inner_->size()); }
    bool contains(const py::str& key) const {
        auto r = cb::knowledge::Read::parse(key.utf8());
        return r && inner_->contains(*r);
    }
    py::set<py::str> keys() const {
        py::set<py::str> out;
        for (const auto& r : inner_->sorted()) out.add(py::str(r.text()));
        return out;
    }
    const std::shared_ptr<cb::knowledge::Reads>& inner() const { return inner_; }

  private:
    std::shared_ptr<cb::knowledge::Reads> inner_;
};

class recording {
  public:
    recording() : reads_(std::make_shared<Reads>(rec_.reads())) {}
    recording(const recording&) = delete;
    recording& operator=(const recording&) = delete;
    Reads& reads() { return *reads_; }
    py::set<py::str> keys() const { return reads_->keys(); }
    py::list<py::str> sorted_keys() const { return reads_->sorted_keys(); }

  private:
    cb::knowledge::Recording rec_;
    std::shared_ptr<Reads> reads_;
};

// The recording in force in this thread, or null. (The old `Reads` of a context the new code started wraps the same set.)
inline std::shared_ptr<Reads> capture() {
    auto cur = cb::knowledge::current_recording();
    return cur ? std::make_shared<Reads>(cur) : nullptr;
}

class Adopt {
  public:
    explicit Adopt(std::shared_ptr<Reads> reads) : adopt_(reads ? reads->inner() : nullptr) {}
    Adopt(const Adopt&) = delete;
    Adopt& operator=(const Adopt&) = delete;

  private:
    cb::knowledge::AdoptRecording adopt_;
};

inline bool active() { return cb::knowledge::recording_active(); }

inline py::str join(const py::str& platform, const py::str& kind, const py::str& key) {
    return py::str(cb::knowledge::Read{platform.utf8(), kind.utf8(), key.utf8()}.text());
}
inline std::tuple<py::str, py::str, py::str> split(const py::str& read) {
    auto r = cb::knowledge::Read::parse(read.utf8());
    if (!r) throw py::ValueError("not enough values to unpack");
    return {py::str(r->platform), py::str(r->kind), py::str(r->key)};
}

inline void record(const py::str& platform, const py::str& kind, const py::str& key) {
    cb::knowledge::record(platform.utf8(), kind.utf8(), key.utf8());
}
inline void record_kind(const py::str& platform, const py::str& kind) { cb::knowledge::record_kind(platform.utf8(), kind.utf8()); }
inline void merge(const py::list<py::str>& keys) { cb::knowledge::merge_reads(detail::reads_of(keys)); }

// ---- worker processes ----
template <class T>
struct Carried {
    T value;
    py::list<py::str> keys;
};

template <class Context, class Item, class T>
class Carry {
  public:
    explicit Carry(std::function<T(const Context&, const Item&)> work) : work_(std::move(work)) {}
    Carried<T> operator()(const Context& context, const Item& item) const {
        auto carried = cb::knowledge::run_recorded([&] { return work_(context, item); });
        return Carried<T>{std::move(carried.value), detail::keys_of(carried.reads)};
    }

  private:
    std::function<T(const Context&, const Item&)> work_;
};

template <class T>
T unwrap(const Carried<T>& carried, const std::function<void(const py::list<py::str>&)>& into = nullptr) {
    if (into) into(carried.keys);
    else merge(carried.keys);
    return carried.value;
}

// The hash of a read piece now. The old reads.cpp installs the function that brings the derived kinds up to date and
// asks the base (knowledge/store, symbols/*); until then asking is a RuntimeError.
inline std::function<py::str(const py::str&)>& digest_impl() {
    static std::function<py::str(const py::str&)> impl;
    return impl;
}
inline py::str digest(const py::str& read) {
    if (!digest_impl()) throw py::RuntimeError("the knowledge base's digest is not installed");
    return digest_impl()(read);
}

}  // namespace codebreaker::knowledge::reads
