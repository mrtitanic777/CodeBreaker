// codebreaker/build/masm.hpp -- typed C++17 port of codebreaker.build.masm.
//
// masmlift.py -- assemble reconstructed 16-bit ASM with the period MASM (stage F).
//
// Much of KERNEL/USER/GDI was hand-written assembly, and assembly can only be proven byte-exact by
// RE-ASSEMBLING it and diffing the bytes. MASM is a 16-bit DOS program (no NTVDM on x64): this drives it on
// CodeBreaker's own emulator (build/native_dos) or, failing that, under the vendored DOSBox-X.
//
// MASM emits an OMF `.OBJ` exactly like the C compiler, so the whole downstream is REUSED: `formats/omf`
// pulls the code bytes + relocation mask. This module only has to run the assembler.
//
//     masm::main({"--selftest"})       assemble a tiny proc, extract it
//     masm::main({"foo.asm"})          assemble foo.asm, dump OBJ segments
//
// Notes on the port:
//   - the Python's import-time constants that read the environment or settings (DBX, MASMROOT, DOSBOX_CORE,
//     POOL_MAX, WARM_POOL, WORKER_MAX_USES, ASM_BATCH) are functions here, read when first asked (an import cannot
//     fail in C++; the Python's int() of a bad WT_POOL_MAX fails at import, here at first use) and cached.
//   - results are the structs of native_dos (Assembly) and dos_sessions (Assembled, Product) instead of dicts.
//   - the assembled-bytes cache stores zlib streams as the Python does (kind "masm"); zlib is not in the runtime,
//     so inflate (reads what Python wrote) and a stored-block deflate (valid zlib any reader inflates, larger than
//     level 6's) are written here (zlib_decompress / zlib_compress).
//   - Python's atexit(close_pools) is std::atexit, registered when the first pool is made.
//   - warm workers: `worker._uses` is kept in a side table here (DosboxWorker has no such field).
//   - main(): argparse is a small hand parse (`asm` or `--selftest`, `-h`); its usage text is not argparse's.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/build/dos_sessions.hpp"
#include "codebreaker/build/native_dos.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::masm {

using Assembly = native_dos::Assembly;
using Product = dos_sessions::Product;
using Assembled = dos_sessions::Assembled;
using Items = py::list<std::tuple<py::str, py::str>>;   // [(stem, asm_src)]

const py::str& DBX();           // settings.json: dosbox_x
const py::str& MASMROOT();      // settings.json: masm (BIN, INC, LIB)

// The DOSBox-X path for an asm byte-match re-assemble, read at call time so CB_NO_DOSBOX is honoured (this is a
// byte-match spot-check path, never a routine sweep/build path).
py::str _dbx();

// DOSBox CPU core. `normal` is the safe interpreter; `dynamic` JITs the emulated x86 (WT_DOSBOX_CORE overrides).
const py::str& DOSBOX_CORE();

// the DOSBox configuration of a cold-boot assemble: {masm} {work} {cmd} are filled by format_conf
const py::str& CONF();
py::str format_conf(const py::str& masm, const py::str& work, const py::str& cmd);

// Poll a cold-boot DOSBox assemble run. Returns "aborted" | "done" | "timeout".
py::str _wait_dosbox(dos_sessions::Popen& proc, const py::str& work, double timeout, const std::function<bool()>& aborted);

// Assemble asm_src with MASM and return {obj, lst, log, work}.
//
// By default this runs on CodeBreaker's OWN in-process emulator (build/native_dos) -- proven byte-identical to
// DOSBox for MASM -- and falls back to DOSBox-X automatically if the native run cannot complete. CB_NATIVE_DOS
// forces one or the other. masmroot selects the assembler: the default (MASM 4.00, 16-bit) or an override such as
// the vendored MASM 5.10A (32-bit `.386`/USE32).
Assembly assemble(const py::str& asm_src, const py::str& stem = py::str("t"), double timeout = 180,
                  const std::optional<py::str>& masmroot = std::nullopt);

// ---- warm worker pool
int64_t POOL_MAX();        // Workers PER pool (16-bit MASM 4.00 and 32-bit MASM 5.10A each have one): WT_POOL_MAX, default 3
bool WARM_POOL();          // WT_NO_WARM_POOL=1 forces the cold-boot path
int64_t WORKER_MAX_USES(); // Retire a warm worker after this many assemble batches (0 = never, the default)
int64_t ASM_BATCH();       // Max functions assembled per DOSBox dispatch (WT_ASM_BATCH, default 40)

std::shared_ptr<dos_sessions::WorkerPool> _get_pool(const py::str& masmroot);
void close_pools();
bool _cancelled();

// ---- assembled-bytes cache: MASM is DETERMINISTIC, so the result is kept (base/cache, kind "masm") by the
// source, the assembler and its folder. Bump CACHE_VER if the invocation changes in a way that alters output.
inline constexpr int64_t CACHE_VER = 1;

// Every input of what MASM makes: the invocation's version, every file under the assembler's folder by its bytes,
// the source, and the stem.
py::str _cache_key(const py::str& src, const py::str& masmroot, const py::str& stem = py::str(""));
std::optional<Product> _cache_get(const py::str& key);
void _cache_put(const py::str& key, const Product& res);

// zlib.decompress (nullopt: zlib.error) / zlib.compress (a valid stream, stored blocks)
std::optional<py::bytes> zlib_decompress(const py::bytes& data);
py::bytes zlib_compress(const py::bytes& data);

// Assemble MANY (stem, asm_src) pairs, using the persistent assembled-bytes cache. Each item whose .asm source is
// already cached is served instantly; only the misses are assembled, then cached.
Assembled assemble_many(const Items& items, std::optional<double> timeout = std::nullopt,
                        const std::optional<py::str>& masmroot = std::nullopt);

// Assemble (stem, asm_src) pairs WITHOUT the cache -- the real MASM run.
Assembled _assemble_uncached(const Items& items, std::optional<double> timeout = std::nullopt,
                             const std::optional<py::str>& masmroot = std::nullopt);

// Assemble and return {name: (bytes, mask)} via formats/omf (same as cc16).
py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> functions(const py::str& asm_src,
                                                                      const std::optional<py::list<py::str>>& flags = std::nullopt);

extern const py::str SELF_ASM;

bool _selftest();

// python masmlift.py [asm] [--selftest] -> the exit code
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::masm
