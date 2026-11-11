// codebreaker/symbols/api_arguments.hpp -- interface of codebreaker.symbols.api_arguments (typed C++; implementation pending).
//
// How many bytes of arguments an imported function removes from the stack -- read from the
// retail module that EXPORTS it.
//
//     stack_bytes("USER32", "FindWindowA")  -> 8
//     stack_bytes("CRTDLL", "memchr")       -> 0        (cdecl: the caller cleans up)
//     stack_bytes("KERNEL32", "nonesuch")   -> None     (not found / not decidable)
//
// Typing notes:
//   * _Mod is a class (a PE module read for its exports/imports/code); the capstone decoder it holds
//     (platforms.decoder(32)) has no typed port, so it is an opaque handle `md`.
//   * _mod() answers None as a null shared_ptr.
//   * _module() answers None or (NEFile, {ordinal: Entry}, {segment idx: Segment}) -- `NeModule`.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::symbols::api_arguments {

inline constexpr int64_t _MAX_INSNS = 4000;   // per function
inline constexpr int64_t _MAX_HOPS = 6;       // call / forwarder depth
// used by: symbols.ordinals
extern const py::list<py::str> _EXTS;         // (".dll", ".exe", ".drv", ".cpl", ".ocx")

// A PE module opened for reading its exports (used by decompile.describe, symbols.ordinals via _mod).
class _Mod {
  public:
    std::shared_ptr<formats::pe::PEFile> pe;
    uint64_t base = 0;
    py::dict<py::str, int64_t> exports;   // name -> rva
    py::dict<int64_t, int64_t> ords;      // ordinal -> rva
    py::dict<uint64_t, std::tuple<py::str, py::str>> imports;   // iat slot va -> (dll, name)
    std::shared_ptr<void> md;             // platforms.decoder(32): the capstone decoder (opaque)
    int64_t exp_lo = 0, exp_hi = 0;       // the export directory's rva range
    py::dict<int64_t, std::optional<int64_t>> _memo;   // _scan's results by rva (Python: m.__dict__["_memo"])

    explicit _Mod(const py::str& path);
    py::bytes bytes_at(int64_t rva, int64_t n) const;
    // 'DLL.Name' when an export's address points into the export directory.
    std::optional<py::str> forwarder(int64_t rva) const;
};

// the 16-bit module of an export name: (NEFile, {ordinal: entry}, {segment: segment})
using NeModule = std::tuple<formats::ne::NEFile, py::dict<int64_t, formats::ne::Entry>, py::dict<int64_t, formats::ne::Segment>>;

// DLL stem (upper-cased, extension removed)
py::str _stem(const py::str& dll);
// used by: symbols.ordinals, decompile.describe
std::shared_ptr<_Mod> _mod(const py::str& stem);   // null: None
// Stack bytes removed by the function at `rva`, or None.
std::optional<int64_t> _scan(_Mod& m, int64_t rva, int64_t hops);
std::optional<int64_t> _resolve(const py::str& dll, const py::str& name, int64_t hops);
// The table an older CodeBreaker kept for `stem` beside the corpus (apiarity/<stem>.json), or {}.
py::dict<py::str, std::optional<int64_t>> _apiarity(const py::str& stem);

// knowledge._table(platform, kind): the kind's rows, an authored kind loaded from its file first when the file changed
// (used by symbols.ordinals)
py::dict<py::str, knowledge::sources::Json> _knowledge_table(const py::str& platform, const py::str& kind);

// The headers' table, read once (tests reset it to None: tests.test_knowledge_reads)
extern std::optional<py::dict<py::str, std::optional<int64_t>>> _TABLE;

// Argument bytes for `name` as the Win32 headers declare it, or None when the headers do not settle it.
std::optional<int64_t> header_bytes(const py::str& name);
// Bytes of arguments `dll!name` pops on return (0 for cdecl), or None if unknown.
std::optional<int64_t> stack_bytes(const py::str& dll, const py::str& name);
// Argument count for the oracle's import stub (stack bytes / 4), or None if unknown.
std::optional<int64_t> argc(const py::str& dll, const py::str& name);

// The table of sizes found so far for this product and reading, as bytes (sorted JSON).
py::bytes sizes16();
// (NEFile, entries, segments) of the module that exports under `name`, or None. (used by symbols.ordinals)
std::optional<NeModule> _module(const py::str& name);
// Bytes of arguments the export removes on return; None when unknown.
std::optional<int64_t> arg_bytes(const py::str& module, int64_t ordinal);
// arg_bytes for a name as the emulator reports it: 'USER.@171' or 'USER_171'.
std::optional<int64_t> of(const std::optional<py::str>& api);

}  // namespace codebreaker::symbols::api_arguments
