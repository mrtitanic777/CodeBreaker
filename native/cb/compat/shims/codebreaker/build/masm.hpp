// codebreaker/build/masm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/masm.hpp for the ports that still use the old API
// (build/dos_tools' `masm`, the old verify and exact lanes). No logic: conversion and forwarding. Staged under
// native/cb/compat/shims/ until the exe's build is wired for C++20, like its neighbours native_dos and dos_sessions.
//
// Old API: Assembly, Product, Assembled, Items, DBX, MASMROOT, assemble, assemble_many, functions, SELF_ASM, _selftest, main,
// close_pools. (The cold boot's internals -- _dbx, DOSBOX_CORE, CONF, format_conf, _wait_dosbox -- the pool's knobs (POOL_MAX,
// WARM_POOL, WORKER_MAX_USES, ASM_BATCH), _get_pool, _cancelled, the cache's CACHE_VER, _cache_key, _cache_get, _cache_put and
// _assemble_uncached are the new Assembler's own and are not carried over.)
//
// The one Assembler the old callers share is made on first use from the old settings (`masm`, `dosbox_x`), the environment's
// options (WT_POOL_MAX ...), the old DOS executive (exact/emu/dos, behind native_dos's OldDosExecutive) and the old DOSBox runner
// (build/dos_tools, behind OldDosRunner), and keeps what it assembles in the old cache shim's cache.
// COMPAT(py): remove when exact/emu/dos, build/dos_tools and base/cache are not old.
//
// DIVERGES, for the old callers: an Assembly / Assembled has no work folder (`work` is "" -- nothing is left on disk, the
// products are already read; callers' rmtree of it is a no-op) and never `aborted`; a .LST / .LOG keeps the CRLF line ends the
// tool wrote; the results of assemble_many are a dict in the order the sources were given, and a stem given twice is a ValueError.
#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "dos_runner_adapter.hpp"   // the shim beside this one: OldDosRunner
#include "native_dos.hpp"           // the shim beside this one: Assembly, Product, Assembled, executive()

#include "cb/build/masm.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/base/cache.hpp"

namespace codebreaker::build::masm {

using Assembly = native_dos::Assembly;
using Product = native_dos::Product;
using Assembled = native_dos::Assembled;
using Items = py::list<std::tuple<py::str, py::str>>;   // [(stem, asm_src)]

// settings.json: dosbox_x
inline const py::str& DBX() {
    static const py::str v = py::settings::get_str(py::str("dosbox_x"));
    return v;
}
// settings.json: masm (BIN, INC, LIB)
inline const py::str& MASMROOT() {
    static const py::str v = py::settings::get_str(py::str("masm"));
    return v;
}

namespace shim_ {

namespace nm = ::cb::build::masm;

// The assembler the old callers share (made when first asked, closed at exit).
inline nm::Assembler& assembler() {
    static const OldDosRunner runner;
    static nm::Assembler shared([] {
        nm::Tools tools;
        tools.masm = ::cb::compat::legacy::path_of(MASMROOT());
        tools.dosbox_x = ::cb::compat::legacy::path_of(DBX());
        const auto forbidden = ::cb::fs::get_env("CB_NO_DOSBOX");
        tools.dosbox_forbidden = forbidden && !forbidden->empty();
        return nm::Context{tools, nm::Options::from_environment(), native_dos::executive(), runner, &base::cache::shim::any_cache(), nullptr};
    }());
    return shared;
}

inline std::optional<std::filesystem::path> root_of(const std::optional<py::str>& masmroot) {
    return masmroot && !masmroot->empty() ? std::optional<std::filesystem::path>(::cb::compat::legacy::path_of(*masmroot)) : std::nullopt;
}

inline std::chrono::milliseconds milliseconds_of(double seconds) {
    return std::chrono::milliseconds(static_cast<long long>(seconds * 1000));
}

}  // namespace shim_

// Assemble asm_src with MASM and return {obj, lst, log, work}.
inline Assembly assemble(const py::str& asm_src, const py::str& stem = py::str("t"), double timeout = 180,
                         const std::optional<py::str>& masmroot = std::nullopt) {
    return ::cb::compat::guard([&] {
        const ::cb::build::dos_sessions::Product p =
            shim_::assembler().assemble(asm_src.utf8(), stem.utf8(), shim_::milliseconds_of(timeout), shim_::root_of(masmroot));
        const Product old = native_dos::detail::to_old(p);
        Assembly a;
        a.obj = old.obj;
        a.lst = old.lst;
        a.log = old.log;
        a.work = py::str("");
        return a;
    });
}

// Assemble MANY (stem, asm_src) pairs, using the persistent assembled-bytes cache.
inline Assembled assemble_many(const Items& items, std::optional<double> timeout = std::nullopt,
                               const std::optional<py::str>& masmroot = std::nullopt) {
    return ::cb::compat::guard([&] {
        std::vector<::cb::build::dos_sessions::Source> sources;
        for (const auto& [stem, text] : items.vec()) sources.push_back({stem.utf8(), text.utf8()});
        std::optional<std::chrono::milliseconds> limit;
        if (timeout) limit = shim_::milliseconds_of(*timeout);
        const ::cb::build::dos_sessions::Finished done = shim_::assembler().assemble_many(sources, limit, shim_::root_of(masmroot));
        Assembled out;
        py::dict<py::str, Product> results;
        for (const auto& [stem, p] : done.results) results.set(py::str(stem), native_dos::detail::to_old(p));
        out.results = results;
        out.work = py::str("");
        return out;
    });
}

// Assemble and return {name: (bytes, mask)} via formats/omf (same as cc16). `flags` was never used.
inline py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> functions(const py::str& asm_src,
                                                                              const std::optional<py::list<py::str>>& = std::nullopt) {
    return ::cb::compat::guard([&] {
        py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> out;
        for (const auto& [name, f] : shim_::assembler().functions(asm_src.utf8())) {
            py::set<int64_t> mask;
            for (std::uint64_t at : f.fixup_mask) mask.add(static_cast<int64_t>(at));
            out.set(py::str(name), std::make_tuple(::cb::compat::to_py(::cb::ByteView(f.code)), mask));
        }
        return out;
    });
}

inline const py::str SELF_ASM{std::string(::cb::build::masm::kSelfTestSource)};

// Assemble SELF_ASM and check the bytes of `_addasm` (the account goes to standard output).
inline bool _selftest() {
    return ::cb::compat::guard([&] {
        std::string report;
        const bool ok = ::cb::build::masm::selftest(shim_::assembler(), report);
        ::cb::out("{}", report);
        return ok;
    });
}

// python masmlift.py [asm] [--selftest] -> the exit code
inline int64_t main(const py::list<py::str>& argv) {
    static const OldDosRunner runner;
    std::vector<std::string> args{"masm"};
    for (const py::str& a : argv.vec()) args.push_back(a.utf8());
    const ::cb::build::masm::Context context{[] {
        ::cb::build::masm::Tools tools;
        tools.masm = ::cb::compat::legacy::path_of(MASMROOT());
        tools.dosbox_x = ::cb::compat::legacy::path_of(DBX());
        return tools;
    }(), ::cb::build::masm::Options::from_environment(), native_dos::executive(), runner, &base::cache::shim::any_cache(), nullptr};
    return ::cb::compat::guard([&] { return static_cast<int64_t>(::cb::cli::Cli("codebreaker", {::cb::build::masm::command(context)}).run(args)); });
}

// Close the shared assembler's warm sessions now (they are closed at exit anyway).
inline void close_pools() { shim_::assembler().close_sessions(); }

}  // namespace codebreaker::build::masm
