// codebreaker/build/dos_sessions.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/dos_sessions.hpp for the ports that still
// use the old API (build/masm, build/native_dos, build/dos_tools). No logic: conversion and forwarding. Staged under
// native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: _WAIT_COM, _POLL, _WORKER_BAT, _CONF, Popen, Product, Assembled, DosboxWorker, WorkerPool. (Semaphore, a stand-in for
// threading.Semaphore, is std::counting_semaphore now: not carried over.)
//
// DIVERGES, for the old callers: sources whose stem is not a DOS name (1-8 letters, digits, _ or $) are refused with ValueError;
// a Popen is a kill-on-close job (the old one left grandchildren alone).
#pragma once

#include <memory>
#include <optional>
#include <string>

#include "cb/build/dos_sessions.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::dos_sessions {

namespace nd = cb::build::dos_sessions;

inline const py::bytes _WAIT_COM = py::bytes(std::string(reinterpret_cast<const char*>(nd::kWaitCom.data()), nd::kWaitCom.size()));
inline const py::str _POLL = py::str("rescan >nul\r\nif exist C:\\GO goto work\r\nwait\r\n");
inline const py::str _WORKER_BAT = py::str(nd::worker_script());
inline const py::str _CONF = py::str(nd::dosbox_config("%s"));   // (the %s was the work folder: the old one filled it with `%`)

// A child process: what DosboxWorker uses of subprocess.Popen.
class Popen {
public:
    // starts args[0] with the other arguments, the parent's environment plus `extra_env`, stdout and stderr to the null device
    Popen(const py::list<py::str>& args, const py::dict<py::str, py::str>& extra_env) {
        cb::process::ProcessSpec spec;
        for (const py::str& a : args.vec()) spec.argv.push_back(a.utf8());
        for (const auto& [k, v] : extra_env.items()) spec.env[k.utf8()] = v.utf8();
        spec.out = cb::process::Capture::Discard;
        spec.err = cb::process::Capture::Discard;
        try {
            proc_ = std::make_unique<cb::process::Process>(cb::process::start(spec));
        } catch (const cb::ToolError& e) {
            throw py::FileNotFoundError(e.what());   // the old one raised OSError when the program could not start
        }
    }
    Popen(const Popen&) = delete;
    Popen& operator=(const Popen&) = delete;

    // the exit code, or None while it runs
    std::optional<int64_t> poll() {
        if (code_) return code_;
        if (const auto c = proc_->wait(std::chrono::milliseconds(0))) code_ = static_cast<int64_t>(*c);
        return code_;
    }
    // stops it (nothing when it has ended)
    void kill() { proc_->kill(); }
    // waits up to `timeout` seconds for it to end -> the exit code; TimeoutError past it
    int64_t wait(std::optional<double> timeout = std::nullopt) {
        if (code_) return *code_;
        const auto ms = timeout ? std::chrono::milliseconds(static_cast<long long>(*timeout * 1000)) : std::chrono::milliseconds::max();
        const auto c = proc_->wait(ms);
        if (!c) throw py::TimeoutError("timed out");
        code_ = static_cast<int64_t>(*c);
        return *code_;
    }

private:
    std::unique_ptr<cb::process::Process> proc_;
    std::optional<int64_t> code_;
};

// One file an assembly produced: each present only when the file exists.
struct Product {
    std::optional<py::bytes> obj;   // the .OBJ
    std::optional<py::str> lst;     // the .LST (latin1 text)
    std::optional<py::str> log;     // the .LOG (latin1 text)
};

// What assemble_many answers: {'results': {stem: Product}, 'work': dir}, {'aborted': True} or {'dead': True}.
struct Assembled {
    bool aborted = false;
    bool dead = false;
    std::optional<py::dict<py::str, Product>> results;
    std::optional<py::str> work;
};

// One persistent DOSBox session running the MASM dispatch loop.
class DosboxWorker final : public nd::Session {
public:
    py::str dbx;
    py::str work;

    DosboxWorker(const py::str& masmroot, const py::str& dbx_, double boot_timeout = 20) : dbx(dbx_) {
        inner_ = cb::compat::guard([&] {
            return std::make_unique<nd::DosboxWorker>(cb::fs::from_text(masmroot.utf8()), cb::fs::from_text(dbx_.utf8()),
                                                      std::chrono::milliseconds(static_cast<long long>(boot_timeout * 1000)));
        });
        work = py::str(cb::fs::to_text(inner_->work()));
    }

    // Assemble [(stem, asm_src)] in this warm session.
    Assembled assemble_many(const py::list<std::tuple<py::str, py::str>>& items, std::optional<double> timeout = std::nullopt,
                            const std::function<bool()>& cancelled = nullptr) {
        std::vector<nd::Source> sources;
        for (const auto& [stem, text] : items.vec()) sources.push_back({stem.utf8(), text.utf8()});
        std::optional<std::chrono::milliseconds> limit;
        if (timeout) limit = std::chrono::milliseconds(static_cast<long long>(*timeout * 1000));
        const nd::Outcome o = cb::compat::guard([&] { return inner_->assemble_many(sources, limit, cancelled); });
        Assembled out;
        if (std::holds_alternative<nd::Aborted>(o)) {
            out.aborted = true;
        } else if (std::holds_alternative<nd::Dead>(o)) {
            out.dead = true;
        } else {
            const auto& done = std::get<nd::Finished>(o);
            py::dict<py::str, Product> results;
            for (const auto& [stem, p] : done.results) {
                Product d;
                if (p.obj) d.obj = cb::compat::to_py(cb::ByteView(*p.obj));
                if (p.lst) d.lst = py::str(*p.lst);
                if (p.log) d.log = py::str(*p.log);
                results[py::str(stem)] = d;
            }
            out.results = results;
            out.work = py::str(cb::fs::to_text(done.work));
        }
        return out;
    }

    // The pool's interface (the new Session): the same batch, in the new vocabulary.
    nd::Outcome assemble_many(std::span<const nd::Source> items, std::optional<std::chrono::milliseconds> timeout,
                              const std::function<bool()>& cancelled) override {
        return inner_->assemble_many(items, timeout, cancelled);
    }
    bool alive() override { return inner_ && inner_->alive(); }
    void close() override {
        if (inner_) inner_->close();
    }

private:
    std::unique_ptr<nd::DosboxWorker> inner_;
};

// Thread-safe pool of warm workers, capped at `maxsize`.
class WorkerPool {
public:
    py::str masmroot;
    py::str dbx;

    WorkerPool(const py::str& masmroot_, const py::str& dbx_, int64_t maxsize)
        : masmroot(masmroot_), dbx(dbx_),
          pool_([this]() -> std::shared_ptr<nd::Session> { return std::make_shared<DosboxWorker>(masmroot, dbx); },
                static_cast<std::size_t>(maxsize)) {}

    std::shared_ptr<DosboxWorker> lease() { return std::static_pointer_cast<DosboxWorker>(pool_.lease()); }
    void release(const std::shared_ptr<DosboxWorker>& w, bool keep = true) { pool_.release(w, keep); }
    void close() { pool_.close(); }

private:
    nd::WorkerPool pool_;
};

}  // namespace codebreaker::build::dos_sessions
