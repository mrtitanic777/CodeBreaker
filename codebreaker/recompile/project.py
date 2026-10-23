"""project.py -- a recompiled Xbox 360 title as a PC project that builds.

    python -m codebreaker recompile-project <image or .xex> -o <folder>
    from codebreaker.recompile import project
    agg = project.scaffold(path, folder)

Lays out the source tree a static recompilation needs, from the image alone:

    <folder>/
      CMakeLists.txt, Play-<name>.bat, README.md, GPU_DESIGN.md
      include/    rex_ppc_min.h (the runtime header), <name>.h (every function declared),
                  <name>_config.h (image base, entry, size), rex_host.h (guest heap, host helpers)
      src/        <name>_NNNN.cpp (the recompiled functions, in translation units of ~1.5 MB),
                  <name>_init.cpp (guest address -> function), rex_runtime.cpp (indirect calls,
                  guest memory, boot), <name>_imports.cpp (one host function per import)
      graphics/   the native GPU subsystem (gpu.py), with the rexglue DXBC shader translator
      kernel/     the rexglue kernel core (native/rexglue/kernel), which the recompiled code
                  bridges onto: its PPCContext, and its real implementations of kernel exports
      assets/     the image (loaded at its base) and the .xex

What it is today: the project builds and links. To run a whole title, the host side has to be
filled in where it is not yet -- the imports the kernel core does not implement are stubs that
log once, and the GPU bridge is a scaffold (GPU_DESIGN.md).
"""
import os
import struct
import sys

from codebreaker.recompile import powerpc

_REXGLUE = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
                        "native", "rexglue")


def _clean_name(module):
    base = os.path.splitext(module)[0]
    for suf in ("XENON", "_xenon", "Xenon"):
        base = base.replace(suf, "")
    return "".join(c for c in base if c.isalnum() or c == "_").strip("_") or "title"


SHARD_BYTES = 1_500_000        # target size of each src/*.cpp translation unit


def _shards(parts, target=SHARD_BYTES):
    """Group per-function C++ into balanced translation units (~target bytes)."""
    out, cur, sz = [], [], 0
    for p in parts:
        cur.append(p)
        sz += len(p)
        if sz >= target:
            out.append(cur); cur, sz = [], 0
    if cur:
        out.append(cur)
    return out


def _write_if_changed(path, data):
    """Write `data` to `path` unless it already holds exactly that: a file left alone keeps its
    time, so rebuilding a regenerated project recompiles only what changed."""
    try:
        with open(path, "rb") as f:
            if f.read() == data:
                return False
    except OSError:
        pass
    with open(path, "wb") as f:
        f.write(data)
    return True


def _sync_tree(src, dst, ignore):
    """Make `dst` a copy of `src` (less what `ignore` drops), touching only files that differ,
    and removing the ones `src` no longer has."""
    keep = set()
    for dp, dirs, files in os.walk(src):
        drop = ignore(dp, dirs + files)
        dirs[:] = [d for d in dirs if d not in drop]
        rel = os.path.relpath(dp, src)
        os.makedirs(os.path.join(dst, rel), exist_ok=True)
        for f in files:
            if f in drop:
                continue
            target = os.path.normpath(os.path.join(dst, rel, f))
            keep.add(target)
            with open(os.path.join(dp, f), "rb") as fh:
                _write_if_changed(target, fh.read())
    for dp, _dirs, files in os.walk(dst):
        for f in files:
            path = os.path.normpath(os.path.join(dp, f))
            if path not in keep:
                os.remove(path)


def _copy_rex_dxbc(dst):
    """Copy the rexglue DXBC shader translator (native/rexglue/dxbc) into a project, without
    build products or standalone tests. -> True when it is there to copy."""
    import shutil
    src = os.path.join(_REXGLUE, "dxbc")
    if not os.path.isdir(src) or not os.path.exists(os.path.join(src, "CMakeLists.txt")):
        return False
    _SKIP_DIRS = {"obj"}
    _SKIP_FILES = {"build_dxbc.sh", "linktest.cpp", "translate_probe.cpp",
                   "client_test.cpp", "wired_main.cpp", "librex_dxbc.a"}

    def _ignore(dir_path, names):
        drop = set()
        for n in names:
            if n in _SKIP_DIRS or n in _SKIP_FILES:
                drop.add(n)
            elif n.endswith((".a", ".o", ".exe")):
                drop.add(n)
        return drop

    _sync_tree(src, dst, _ignore)
    return True


def _copy_rex_kernel(dst):
    """Copy the rexglue kernel core (native/rexglue/kernel) into a project, without build
    products. Its CMake target hands C++23, header-only fmt and its include folders to whatever
    links it, so the recompiled code compiles against its PPCContext (the bridge).
    -> True when it is there to copy."""
    import shutil
    src = os.path.join(_REXGLUE, "kernel")
    if not os.path.isdir(src) or not os.path.exists(os.path.join(src, "CMakeLists.txt")):
        return False
    _SKIP = {"obj", "build_kernel.sh", "sweep.sh", "librex_kernel.a"}

    def _ignore(dir_path, names):
        return {n for n in names
                if n in _SKIP or n.endswith((".a", ".o", ".exe"))}

    _sync_tree(src, dst, _ignore)
    return True


def scaffold(xex_path, out_dir, jobs=None):
    """Write the project (see the module's notes). -> agg (powerpc.recompile_image's, with the
    project's name, folder, translation units and imports)"""
    from codebreaker.formats import xex
    from codebreaker.recompile import disassemble
    img, image_base, entry, _sections = disassemble.image(xex_path)
    is_xex = xex.is_xex(xex_path)
    h = xex.header(xex_path) if is_xex else None
    name = _clean_name(h.name if h and h.name else os.path.basename(xex_path))
    # the import stubs get host functions, not recompiled bodies (the loader replaces their bytes)
    libraries = (h.imports if h else None) or ["xboxkrnl.exe", "xam.xex"]
    thunks = {}                                # {guest address: (library, ordinal)}
    from codebreaker.formats import powerpc_image
    image = powerpc_image.read(xex.basefile(xex_path) if is_xex else xex_path)
    for addr, symbol in image.imports.items():
        lib, ordinal = (int(n) for n in symbol[len("import"):].split("_ord_"))
        thunks[addr] = (libraries[lib] if lib < len(libraries) else "library%d" % lib, ordinal)
    _body, mapping, agg = powerpc.recompile_image(xex_path, jobs=jobs, skip=set(thunks))
    parts, decls, chosen = agg["parts"], agg["decls"], agg["chosen"]

    inc = os.path.join(out_dir, "include")
    src = os.path.join(out_dir, "src")
    assets = os.path.join(out_dir, "assets")
    for d in (inc, src, assets):
        os.makedirs(d, exist_ok=True)

    # all text files are written UTF-8 (Windows' default cp encoding can't encode
    # some characters and would raise mid-write, leaving a truncated file).
    def _w(path, text):
        _write_if_changed(path, text.encode("utf-8"))

    # --- include/ : the runtime header, the shared declarations, the config ---
    _w(os.path.join(inc, "rex_ppc_min.h"), powerpc.runtime_header())
    _w(os.path.join(inc, name + "_config.h"), _config_h(name, image_base, entry, len(img)))
    _w(os.path.join(inc, name + ".h"), _recomp_header(name, decls))
    _w(os.path.join(inc, "rex_host.h"), _host_h(name))   # guest heap + host helpers

    # --- src/ : the recompiled functions, split into many translation units ---
    shards = _shards(parts)
    for i, group in enumerate(shards):
        text = f'#include "{name}.h"\n\n' + "\n".join(group)
        _w(os.path.join(src, f"{name}_{i:04d}.cpp"), text)
    # the import shims go in the table too: the CRT calls some imports through pointers
    # (TlsGetValue handing back KeTlsGetValue's thunk, then bctrl to it)
    _w(os.path.join(src, name + "_init.cpp"),
       _init_cpp(name, sorted(set(chosen) | set(thunks)), sorted(set(thunks) - set(decls))))
    _w(os.path.join(src, "rex_runtime.cpp"), _runtime_cpp(name, entry))
    # the host-shim layer: one typed stub per imported kernel/XAM function
    _w(os.path.join(src, name + "_imports.cpp"), _imports_cpp(name, thunks))

    # --- graphics/ : the native D3D12 GPU subsystem (gpu.py) ---
    from codebreaker.recompile import gpu
    os.makedirs(os.path.join(out_dir, "graphics"), exist_ok=True)
    for rel, text in gpu.gpu_files(name).items():
        _w(os.path.join(out_dir, rel), text)
    _w(os.path.join(out_dir, "GPU_DESIGN.md"), gpu.design_md(name))
    # the DXBC shader translator (graphics/rex_dxbc/): with it there, the ShaderTranslator's
    # __has_include finds it and shaders are translated to DXBC
    have_dxbc = _copy_rex_dxbc(os.path.join(out_dir, "graphics", "rex_dxbc"))
    # the kernel core (kernel/rex_kernel/): the recompiled code bridges onto its PPCContext
    # and kernel (rex_ppc_min.h finds it by itself)
    have_kernel = _copy_rex_kernel(os.path.join(out_dir, "kernel", "rex_kernel"))

    # --- assets/ : the image (for a standalone boot) and the .xex (the kernel core decrypts,
    # decompresses and loads it itself at boot) ---
    with open(os.path.join(assets, name + ".bin"), "wb") as f:
        f.write(img)
    try:
        import shutil as _sh
        if is_xex:
            _sh.copy2(xex_path, os.path.join(assets, name + ".xex"))
    except Exception:
        pass

    # --- build glue, launcher, docs ---
    _w(os.path.join(out_dir, "CMakeLists.txt"), _cmakelists(name))
    _w(os.path.join(out_dir, f"Play-{name}.bat"), _launcher_bat(name))
    _w(os.path.join(out_dir, "README.md"), _readme(name, agg, image_base, entry, len(shards)))

    agg["name"] = name
    agg["dir"] = out_dir
    agg["shards"] = len(shards)
    agg["imports"] = len(thunks)
    return agg


def _config_h(name, image_base, entry, image_size):
    return f"""// {name}_config.h - generated by CodeBreaker
#pragma once
#include <cstdint>
#define PPC_IMAGE_BASE  0x{image_base:08X}ull
#define PPC_ENTRY_POINT 0x{entry:08X}u
#define PPC_IMAGE_SIZE  0x{image_size:X}ull
#define PPC_IMAGE_ASSET "{name}.bin"
#define PPC_XEX_ASSET   "{name}.xex"   // raw XEX (RexGlue runtime loads it at boot)
"""


def _recomp_header(name, decls):
    """The one shared header every src/*.cpp includes: runtime + all the function
    declarations + the mapping-table type. This is what lets the recompiled code
    be split across many translation units yet still call across them."""
    lines = [f"// {name}.h - generated by CodeBreaker.",
             "// Shared declarations for the recompiled title: the runtime, the",
             "// config, and a forward declaration of every recompiled function so",
             "// the src/*.cpp translation units can call across each other.",
             "#pragma once",
             f'#include "{name}_config.h"',   # config first: the bridge's context.h may want it
             '#include "rex_ppc_min.h"',
             "",
             "// --- every recompiled guest function (defined across src/*.cpp) ---"]
    lines += [f"PPC_FUNC(sub_{a:08X});" for a in decls]
    lines += ["",
              "// guest address -> recompiled function, for indirect (bctr) calls.",
              "// In bridge mode the vendored RexGlue context.h already provides this",
              "// (with the same {guest, host} field layout), so we only define it",
              "// ourselves for the standalone build.",
              "#ifndef REX_PPC_BRIDGE",
              "struct PPCFuncMapping { size_t guest; PPCFunc* host; };",
              "extern PPCFuncMapping PPCFuncMappings[];",
              "#endif",
              ""]
    return "\n".join(lines)


def _init_cpp(name, chosen, undeclared=()):
    lines = [f'#include "{name}.h"', ""]
    if undeclared:   # import shims no recompiled code calls directly
        lines += [f"PPC_FUNC(sub_{va:08X});" for va in undeclared] + [""]
    lines += ["// The function-pointer table: guest address -> recompiled function.",
              "PPCFuncMapping PPCFuncMappings[] = {"]
    for va in chosen:
        lines.append(f"\t{{ 0x{va:08X}, sub_{va:08X} }},")
    lines += ["\t{ 0, nullptr },", "};", ""]
    return "\n".join(lines)


# Real host implementations for the tractable named imports. Bodies run in the
# recompiled ABI: args in ctx.r3, ctx.r4, ...; result in ctx.r3; guest memory is
# `base + guest_addr`. Best-effort NT/Xbox signatures — the starter that gets a
# title through boot; refine per title as needed.
HOST_IMPL = {
    # --- synchronisation (single-thread bring-up: no-ops) ---
    "RtlInitializeCriticalSection": ["ctx.r3.s64 = 0;"],
    "RtlInitializeCriticalSectionAndSpinCount": ["ctx.r3.s64 = 0;"],
    "RtlEnterCriticalSection": ["/* single-thread bring-up: no-op */"],
    "RtlLeaveCriticalSection": ["/* single-thread bring-up: no-op */"],
    "RtlTryEnterCriticalSection": ["ctx.r3.s64 = 1;"],
    "RtlDeleteCriticalSection": ["/* no-op */"],
    # --- memory block ops (guest addr -> host pointer; no byte-swap) ---
    "RtlZeroMemory": ["std::memset(base + ctx.r3.u32, 0, ctx.r4.u32);"],
    "RtlFillMemory": ["std::memset(base + ctx.r3.u32, (int)ctx.r5.u32, ctx.r4.u32);"],
    "RtlCopyMemory": ["std::memcpy(base + ctx.r3.u32, base + ctx.r4.u32, ctx.r5.u32);"],
    "RtlCompareMemory":
        ["uint32_t n = ctx.r5.u32, i = 0;",
         "const uint8_t* a = base + ctx.r3.u32; const uint8_t* b = base + ctx.r4.u32;",
         "while (i < n && a[i] == b[i]) ++i; ctx.r3.u64 = i;"],
    # --- heap / pool allocation (guest bump heap; real games want a full heap) ---
    "ExAllocatePool": ["ctx.r3.u64 = rex_heap_alloc(ctx.r3.u32);"],
    "ExAllocatePoolWithTag": ["ctx.r3.u64 = rex_heap_alloc(ctx.r3.u32);"],
    "ExAllocatePoolTypeWithTag": ["ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);"],
    "ExFreePool": ["/* bump heap: no free */"],
    "RtlAllocateHeap": ["ctx.r3.u64 = rex_heap_alloc(ctx.r5.u32);"],
    "RtlReAllocateHeap": ["ctx.r3.u64 = rex_heap_alloc(ctx.r6.u32);"],
    "RtlFreeHeap": ["ctx.r3.s64 = 1;"],
    "MmAllocatePhysicalMemory": ["ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);"],
    "MmAllocatePhysicalMemoryEx": ["ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);"],
    "MmFreePhysicalMemory": ["/* bump heap: no free */"],
    # 360 virtual (0x8xxxxxxx) -> physical: mask the top 3 bits. The GPU (fetch
    # constants, textures) addresses guest RAM physically, so this must be real.
    "MmGetPhysicalAddress": ["ctx.r3.u64 = ctx.r3.u32 & 0x1FFFFFFFu;"],
    # --- debug / diagnostics ---
    "DbgPrint": ['std::fprintf(stderr, "%s", (const char*)(base + ctx.r3.u32));'],
    "KeBugCheck": ['std::fprintf(stderr, "[KeBugCheck] 0x%X\\n", ctx.r3.u32); std::abort();'],
    "KeBugCheckEx": ['std::fprintf(stderr, "[KeBugCheckEx] 0x%X\\n", ctx.r3.u32); std::abort();'],
    # --- time ---
    "KeQueryPerformanceCounter":
        ['static uint64_t t = 0; ctx.r3.u64 = (t += 1000);   // monotonic stub'],
    "KeQueryPerformanceFrequency": ["ctx.r3.u64 = 50000000;   // Xenos 50 MHz timebase"],
    "KeQuerySystemTime": ["rex_store64(base, ctx.r3.u32, rex_system_time_100ns());"],
    "KeDelayExecutionThread": ["ctx.r3.u64 = 0;   // STATUS_SUCCESS, no real wait"],
    # --- process / privilege / image (conservative defaults) ---
    "KeGetCurrentProcessType": ["ctx.r3.u64 = 1;   // title process"],
    "XexCheckExecutablePrivilege": ["ctx.r3.u64 = 0;   // privilege not held"],
    "RtlImageXexHeaderField": ["ctx.r3.u64 = 0;"],
    # --- interrupts / cache / firmware (single-thread bring-up) ---
    "KeEnableInterrupts": ["ctx.r3.u64 = 1;"],
    "KeDisableInterrupts": ["ctx.r3.u64 = 1;"],
    "KeLockL2": ["ctx.r3.u64 = 0;"],
    "KeUnlockL2": ["/* no-op */"],
    "DbgBreakPoint": ["/* no-op (no debugger attached) */"],
    "HalReturnToFirmware":
        ['std::fprintf(stderr, "[HalReturnToFirmware] guest requested reboot\\n");'
         ' std::exit(0);'],
    # --- object handles (bring-up: succeed/no-op) ---
    "NtClose": ["ctx.r3.u64 = 0;   // STATUS_SUCCESS"],
    "ObDereferenceObject": ["/* no-op (no object manager yet) */"],
}


def _host_h(name):
    return f"""// rex_host.h - host implementations for tractable kernel/CRT imports.
// A guest bump heap (allocate in guest memory, hand back guest addresses), the
// block-memory helpers, and big-endian guest-memory accessors + a host clock the
// kernel shims use to write results into guest structures. Our from-scratch
// kernel layer, built out per the Xenia decomposition; threading / real VM / XAM
// / the object manager are the ongoing grind.
#pragma once
#include <cstdint>
#include <chrono>
extern uint32_t g_heap_ptr;      // next free guest address
extern uint32_t g_heap_end;
inline void rex_heap_init(uint32_t start, uint32_t size) {{
    g_heap_ptr = start; g_heap_end = start + size;
}}
inline uint32_t rex_heap_alloc(uint32_t size) {{
    uint32_t a = (g_heap_ptr + 15u) & ~15u;      // 16-byte aligned
    g_heap_ptr = a + size;
    return (a + size <= g_heap_end) ? a : 0;     // 0 == out of guest heap
}}

// --- big-endian guest memory access (the guest is big-endian) ----------------
inline void rex_store32(uint8_t* base, uint32_t addr, uint32_t v) {{
    uint8_t* p = base + addr;
    p[0] = uint8_t(v >> 24); p[1] = uint8_t(v >> 16);
    p[2] = uint8_t(v >> 8);  p[3] = uint8_t(v);
}}
inline void rex_store64(uint8_t* base, uint32_t addr, uint64_t v) {{
    rex_store32(base, addr, uint32_t(v >> 32));
    rex_store32(base, addr + 4, uint32_t(v));
}}
inline uint32_t rex_load32(const uint8_t* base, uint32_t addr) {{
    const uint8_t* p = base + addr;
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) |
           (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}}

// Windows FILETIME: 100ns intervals since 1601-01-01 (what Ke*Time returns).
inline uint64_t rex_system_time_100ns() {{
    using namespace std::chrono;
    uint64_t unix_100ns =
        (uint64_t)duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() / 100;
    return unix_100ns + 116444736000000000ull;   // 1601->1970 epoch offset
}}
"""


def _kernel_h(name):
    return """// rex_kernel.h - from-scratch guest kernel-object layer (host-backed).
// Guest threads run on host std::threads; dispatcher events are keyed by their
// guest KEVENT address (the Ke* ABI passes object pointers, not handles);
// critical sections map to host recursive_mutexes. Modeled on the Xenia XObject
// decomposition but our own, minimal implementation. Threading / object manager
// are what let a title get past early init.
#pragma once
#include <cstdint>

// the runtime registers how to invoke a recompiled guest function on a fresh
// context + stack (used to start guest threads).
typedef void (*RexGuestRunner)(uint32_t entry_addr, uint32_t arg);
void rex_kernel_set_runner(RexGuestRunner runner);

// guest threads (host std::thread) -> handle
uint32_t rex_thread_create(uint32_t entry_addr, uint32_t arg, bool suspended);
void     rex_thread_resume(uint32_t handle);
void     rex_thread_join(uint32_t handle);

// dispatcher events keyed by guest KEVENT address (Ke* ABI)
void     rex_event_init(uint32_t guest_addr, bool manual_reset, bool initial);
void     rex_event_set(uint32_t guest_addr);
void     rex_event_reset(uint32_t guest_addr);
uint32_t rex_event_wait(uint32_t guest_addr, uint32_t timeout_ms);  // 0=signaled,0x102=timeout

// critical sections keyed by guest CRITICAL_SECTION address
void rex_cs_enter(uint32_t guest_addr);
void rex_cs_leave(uint32_t guest_addr);
"""


def _kernel_cpp(name):
    return """// rex_kernel.cpp - host-backed implementation of the guest kernel objects.
#include "rex_kernel.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

static RexGuestRunner g_runner = nullptr;
void rex_kernel_set_runner(RexGuestRunner r) { g_runner = r; }

// --- guest threads --------------------------------------------------------
struct RexThread { uint32_t entry = 0, arg = 0; bool suspended = false; std::thread th; };
static std::mutex g_thr_mtx;
static std::unordered_map<uint32_t, RexThread*> g_threads;
static uint32_t g_next_handle = 0xF0000000u;

static void start_thread(RexThread* t) {
    if (!g_runner) return;
    RexGuestRunner run = g_runner;
    uint32_t e = t->entry, a = t->arg;
    t->th = std::thread([run, e, a] { run(e, a); });
}
uint32_t rex_thread_create(uint32_t entry, uint32_t arg, bool suspended) {
    std::lock_guard<std::mutex> lk(g_thr_mtx);
    RexThread* t = new RexThread(); t->entry = entry; t->arg = arg; t->suspended = suspended;
    uint32_t h = g_next_handle++;
    g_threads[h] = t;
    if (!suspended) start_thread(t);
    return h;
}
void rex_thread_resume(uint32_t h) {
    std::lock_guard<std::mutex> lk(g_thr_mtx);
    auto it = g_threads.find(h);
    if (it != g_threads.end() && it->second->suspended && !it->second->th.joinable()) {
        it->second->suspended = false; start_thread(it->second);
    }
}
void rex_thread_join(uint32_t h) {
    RexThread* t = nullptr;
    { std::lock_guard<std::mutex> lk(g_thr_mtx);
      auto it = g_threads.find(h); if (it != g_threads.end()) t = it->second; }
    if (t && t->th.joinable()) t->th.join();
}

// --- dispatcher events (keyed by guest KEVENT address) --------------------
struct RexEvent { std::mutex m; std::condition_variable cv; bool signaled = false, manual = true; };
static std::mutex g_ev_mtx;
static std::unordered_map<uint32_t, RexEvent*> g_events;
static RexEvent* get_event(uint32_t addr) {
    std::lock_guard<std::mutex> lk(g_ev_mtx);
    auto it = g_events.find(addr);
    if (it != g_events.end()) return it->second;
    RexEvent* e = new RexEvent(); g_events[addr] = e; return e;
}
void rex_event_init(uint32_t addr, bool manual, bool initial) {
    RexEvent* e = get_event(addr); e->manual = manual; e->signaled = initial;
}
void rex_event_set(uint32_t addr) {
    RexEvent* e = get_event(addr);
    { std::lock_guard<std::mutex> lk(e->m); e->signaled = true; }
    e->cv.notify_all();
}
void rex_event_reset(uint32_t addr) {
    RexEvent* e = get_event(addr);
    std::lock_guard<std::mutex> lk(e->m); e->signaled = false;
}
uint32_t rex_event_wait(uint32_t addr, uint32_t timeout_ms) {
    RexEvent* e = get_event(addr);
    std::unique_lock<std::mutex> lk(e->m);
    if (timeout_ms == 0xFFFFFFFFu) {
        e->cv.wait(lk, [&] { return e->signaled; });
    } else if (!e->cv.wait_for(lk, std::chrono::milliseconds(timeout_ms),
                               [&] { return e->signaled; })) {
        return 0x102u;   // STATUS_TIMEOUT
    }
    if (!e->manual) e->signaled = false;
    return 0u;           // STATUS_SUCCESS (signaled)
}

// --- critical sections (keyed by guest CRITICAL_SECTION address) ----------
static std::mutex g_cs_mtx;
static std::unordered_map<uint32_t, std::recursive_mutex*> g_cs;
static std::recursive_mutex* get_cs(uint32_t addr) {
    std::lock_guard<std::mutex> lk(g_cs_mtx);
    auto it = g_cs.find(addr); if (it != g_cs.end()) return it->second;
    std::recursive_mutex* m = new std::recursive_mutex(); g_cs[addr] = m; return m;
}
void rex_cs_enter(uint32_t addr) { get_cs(addr)->lock(); }
void rex_cs_leave(uint32_t addr) { get_cs(addr)->unlock(); }
"""


_REX_EXPORTS = None


def _rex_exports():
    """Read the rexglue kernel core: (ord2name, implemented), where
    ord2name maps (module, ordinal)->export name (from src/kernel/*/export_table.inc)
    and `implemented` is the set of export names with a real __imp__<name> function
    (XBOXKRNL_EXPORT(__imp__<name>, ...)). An import thunk whose export is
    implemented can call that symbol directly: the kernel core's own code."""
    global _REX_EXPORTS
    if _REX_EXPORTS is not None:
        return _REX_EXPORTS
    import re, glob
    root = os.path.join(_REXGLUE, "kernel")
    ord2name, impl = {}, set()
    if os.path.isdir(root):
        exp_re = re.compile(r'\s*XE_EXPORT\(\s*(\w+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(\w+)')
        for inc in glob.glob(os.path.join(root, "src", "kernel", "*", "export_table.inc")):
            for line in open(inc, encoding="utf-8", errors="replace"):
                m = exp_re.match(line)
                if m:
                    ord2name[(m.group(1).lower(), int(m.group(2), 16))] = m.group(3)
        imp_re = re.compile(r'EXPORT\(__imp__(\w+)')
        left_out = re.compile(r"graphics[/\\](command_processor|graphics_system|trace_writer)|"
                              r"xboxkrnl[/\\](cert_monitor|debug_monitor|xboxkrnl_video|xboxkrnl_audio)|"
                              r"xam[/\\](xam_input|xam_ui|xam_nui)")   # (its CMakeLists)
        for cpp in glob.glob(os.path.join(root, "src", "kernel", "**", "*.cpp"), recursive=True):
            if left_out.search(cpp):
                continue
            try:
                txt = open(cpp, encoding="utf-8", errors="replace").read()
            except Exception:
                continue
            impl.update(imp_re.findall(txt))
    _REX_EXPORTS = (ord2name, impl)
    return _REX_EXPORTS


def _imports_cpp(name, thunks):
    """One typed host-shim per imported kernel/XAM function - the recompiled-ABI
    sub_<addr> the game calls. In order: (1) Vd* -> the native GPU subsystem (xe_gpu);
    (2) an export the kernel core implements -> its __imp__<name>; (3) a host implementation
    of CodeBreaker's (HOST_IMPL); (4) a stub that logs once. Names: symbols/console_ordinals.json."""
    import json
    with open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "symbols",
                           "console_ordinals.json"), encoding="utf-8") as f:
        ordinal_names = json.load(f)

    by_lib = {}
    for addr, (lib, ordv) in thunks.items():
        api = ordinal_names.get(lib.lower(), {}).get(str(ordv), "")
        by_lib.setdefault(lib, []).append((addr, ordv, api))
    named = sum(1 for recs in by_lib.values() for _a, _o, api in recs if api)

    # a few Vd* imports are the guest<->GPU seam -> route them to the native GPU
    # subsystem (graphics/) instead of a generic stub.
    vd_routes = {
        "VdInitializeRingBuffer":
            "xe_gpu::gpu().InitializeRingBuffer(ctx.r3.u32, ctx.r4.u32);",
        "VdEnableRingBufferRPtrWriteBack":
            "xe_gpu::gpu().EnableReadPointerWriteBack(ctx.r3.u32, ctx.r4.u32);",
        "VdSwap":
            "xe_gpu::gpu().Swap(ctx.r3.u32, 1280, 720); ctx.r3.s64 = 0;",
        "VdQueryVideoFlags":
            "ctx.r3.u64 = 0x00000007;   // widescreen + progressive (bootstrap)",
    }

    ord2name, rex_impl = _rex_exports()

    def _rexname(lib, ordv):
        if ordv is None:
            return None
        return ord2name.get((lib.split(".")[0].lower(), ordv))

    imp_externs = set()                 # __imp__<name> exports we call directly
    body = []
    n_rex = n_host = n_vd = n_stub = 0
    for lib in sorted(by_lib):
        recs = sorted(by_lib[lib])
        body.append(f"// ---- {lib}  ({len(recs)} functions) ----")
        for addr, ordv, api in recs:
            o = str(ordv) if ordv is not None else "?"
            rexname = _rexname(lib, ordv)
            label = api or rexname or f"ordinal {o}"
            body.append(f"PPC_FUNC(sub_{addr:08X}) {{   // {lib}: {label}")
            body.append(f'    REX_TRACE_IN("{label}");')
            if api in vd_routes:                        # (1) GPU seam -> our xe_gpu
                body.append(f"    {vd_routes[api]}")
                body.append("    (void)ctx; (void)base;")
                n_vd += 1
            elif rexname and rexname in rex_impl:        # (2) real RexGlue kernel export
                imp_externs.add(rexname)
                body.append(f"    __imp__{rexname}(ctx, base);")
                n_rex += 1
            elif api in HOST_IMPL:                       # (3) our own host impl
                for stmt in HOST_IMPL[api]:
                    body.append(f"    {stmt}")
                body.append("    (void)ctx; (void)base;")
                n_host += 1
            else:                                        # (4) log-once stub
                body.append("    static bool once = false;")
                body.append(f'    if (!once) {{ once = true; std::fprintf(stderr,'
                            f' "[import] TODO {lib}: {label}  (sub_{addr:08X})\\n"); }}')
                body.append("    (void)ctx; (void)base;")
                n_stub += 1
            body.append(f'    REX_TRACE_OUT("{label}");')
            body.append("}")
        body.append("")

    lines = [f'#include "{name}.h"', '#include "xe_gpu.h"', '#include "rex_host.h"',
             "#include <cstdio>", "#include <cstring>", "#include <cstdlib>", "",
             "// ==========================================================================",
             f"// Host-shim layer - {len(thunks)} imported functions this title calls.",
             f"//   {n_rex} dispatch to the vendored RexGlue kernel's real __imp__ exports,",
             f"//   {n_vd} route to our native xe_gpu (Vd*), {n_host} use our host impls,",
             f"//   {n_stub} are log-once stubs (no implementation yet).",
             "// Each is the recompiled-ABI function the game invokes (args in ctx.r3, ...).",
             "// REX_TRACE_IMPORTS=1 in the environment prints every call: its first four arguments,",
             "// where it was called from, and what it returned.",
             "// ==========================================================================",
             "",
             "static bool rex_trace_imports() {",
             '    static const bool on = std::getenv("REX_TRACE_IMPORTS") != nullptr;',
             "    return on;",
             "}",
             "static inline uint32_t rex_trace_peek(uint8_t* base, uint32_t a) {  // (an out-argument,",
             "    if (a < 0x70000000u || a >= 0x80000000u) return 0xFFFFFFFFu;     //  when it is on the",
             "    return PPC_LOAD_U32(a);                                          //  guest stack)",
             "}",
             "#define REX_TRACE_IN(n) const uint32_t _a0 = ctx.r3.u32, _a1 = ctx.r4.u32; \\",
             "    do { if (rex_trace_imports()) std::fprintf(stderr, \\",
             '    "[call] %s(%08X, %08X, %08X, %08X) from %08X\\n", n, ctx.r3.u32, ctx.r4.u32, \\',
             "    ctx.r5.u32, ctx.r6.u32, (uint32_t)ctx.lr); } while (0)",
             "#define REX_TRACE_OUT(n) do { if (rex_trace_imports()) \\",
             '    std::fprintf(stderr, "[ret ] %s -> %08X  (*a0=%08X *a1=%08X)\\n", n, ctx.r3.u32, \\',
             "        rex_trace_peek(base, _a0), rex_trace_peek(base, _a1)); } while (0)",
             ""]
    if imp_externs:
        lines.append("// Real RexGlue kernel exports (linked from kernel/rex_kernel). Calling")
        lines.append("// __imp__<name> runs the vendored kernel's actual implementation.")
        for nm in sorted(imp_externs):
            lines.append(f'extern "C" PPC_FUNC(__imp__{nm});')
        lines.append("")
    lines += body
    return "\n".join(lines)


def _runtime_cpp(name, entry):
    return f'''// rex_runtime.cpp - generated by CodeBreaker.
// The host runtime the recompiled code links against: guest memory, the
// address-keyed indirect-call table, host-shim registry (xboxkrnl/XAM/CRT
// stubs), setjmp/longjmp, and a boot main() that loads the image and calls the
// entry point. Fill the stubs to run a whole title (see README).
#include "{name}.h"
#include "xe_gpu.h"
#include "rex_host.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// guest bump heap (ExAllocatePool / RtlAllocateHeap / Mm* draw from this) --------
uint32_t g_heap_ptr = 0;
uint32_t g_heap_end = 0;

static PPCFunc* ppc_lookup(uint32_t addr) {{
    for (PPCFuncMapping* m = PPCFuncMappings; m->host; ++m)
        if (m->guest == addr) return m->host;
    return nullptr;
}}

extern "C" void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr) {{
    if (PPCFunc* f = ppc_lookup(addr)) {{ f(ctx, base); return; }}
    std::fprintf(stderr, "[rex] missing indirect target 0x%08X\\n", addr);   // log + no-op
}}

#if defined(REX_PPC_BRIDGE) && __has_include(<rex/runtime.h>)
// ============================================================================
//  BRIDGE BOOT - the vendored RexGlue runtime owns guest memory, decrypts +
//  decompresses + loads the raw .xex, brings up the kernel (so the __imp__
//  exports + heap are live), and launches the guest entry on an XThread. Our
//  recompiled functions run on that thread with base = the runtime's membase.
// ============================================================================
#include <rex/runtime.h>
#include <rex/kernel/crt/heap.h>
#include <rex/kernel/init.h>
#include <rex/system/xthread.h>

int main() {{
    // unbuffered: what the runtime and the kernel say is still there when the title crashes
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);
    // our native GPU; the guest drives it through the Vd* shims
    xe_gpu::IGpuBackend* backend = xe_gpu::CreateD3D12Backend();
    backend->Initialize(1280, 720);

    rex::Runtime runtime(".", ".", ".");
    rex::RuntimeConfig config{{}};             // our own GPU/audio/input (Vd* -> xe_gpu)
    // the kernel modules (xboxkrnl, xam) and xam's apps: their exports, and the kernel variables
    // the loader writes into the title's import slots (XexExecutableModuleHandle ...)
    config.kernel_init = rex::kernel::InitializeKernel;
    if (runtime.Setup((uint32_t)PPC_IMAGE_BASE, (uint32_t)PPC_IMAGE_SIZE,
                      (uint32_t)PPC_IMAGE_BASE, (uint32_t)PPC_IMAGE_SIZE,
                      PPCFuncMappings, std::move(config)) != 0) {{   /* X_STATUS_SUCCESS */
        std::fprintf(stderr, "[boot] Runtime::Setup failed\\n"); return 2;
    }}
    // (a guest path: the kernel mounts the project folder as game:)
    if (runtime.LoadXexImage("game:\\\\assets\\\\" PPC_XEX_ASSET) != 0) {{   /* X_STATUS_SUCCESS */
        std::fprintf(stderr, "[boot] LoadXexImage failed\\n"); return 3;
    }}
    rex::kernel::crt::InitHeap(64, runtime.memory());

    // hand the real guest memory base to our GPU (shared memory / vertex fetch)
    uint8_t* base = runtime.memory()->virtual_membase();
    backend->SetGuestMemory(base, (uint32_t)PPC_IMAGE_BASE, (uint32_t)PPC_IMAGE_SIZE);
    xe_gpu::gpu().SetTranslators(xe_gpu::CreateShaderTranslator(),
                                 xe_gpu::CreateTextureCache(base),
                                 xe_gpu::CreateRenderTargetCache());
    xe_gpu::gpu().Initialize(base, backend);

    std::printf("[boot] launching {name} at entry 0x%08X\\n", PPC_ENTRY_POINT);
    auto thread = runtime.LaunchModule();     // XThread at the guest entry point
    if (!thread) {{ std::fprintf(stderr, "[boot] LaunchModule failed\\n"); return 4; }}
    thread->Wait(0, 0, 0, nullptr);           // block until the guest returns
    return 0;
}}

#else
// ============================================================================
//  STANDALONE BOOT - our minimal runtime (flat buffer + direct entry call), for
//  a build WITHOUT the vendored kernel. No real kernel/XEX load.
// ============================================================================
static uint8_t* g_buf = nullptr;
static const uint32_t REX_STACK_SIZE = 1u << 20;         // 1 MB guest stack
static const uint32_t REX_HEAP_SIZE  = 256u << 20;       // 256 MB guest heap

static uint8_t* load_image() {{
    const size_t SLACK = (size_t)REX_STACK_SIZE + REX_HEAP_SIZE + 0x10000;
    g_buf = (uint8_t*)std::calloc(1, (size_t)PPC_IMAGE_SIZE + SLACK);
    FILE* f = std::fopen("assets/" PPC_IMAGE_ASSET, "rb");
    if (!f) {{ std::fprintf(stderr, "cannot open assets/%s\\n", PPC_IMAGE_ASSET); return nullptr; }}
    std::fread(g_buf, 1, (size_t)PPC_IMAGE_SIZE, f);
    std::fclose(f);
    return (uint8_t*)((uintptr_t)g_buf - (uintptr_t)PPC_IMAGE_BASE);  // flat map
}}

int main() {{
    uint8_t* base = load_image();
    if (!base) return 2;
    xe_gpu::IGpuBackend* backend = xe_gpu::CreateD3D12Backend();
    backend->Initialize(1280, 720);
    xe_gpu::gpu().Initialize(base, backend);
    xe_gpu::gpu().SetTranslators(xe_gpu::CreateShaderTranslator(),
                                 xe_gpu::CreateTextureCache(base),
                                 xe_gpu::CreateRenderTargetCache());
    backend->SetGuestMemory(base, (uint32_t)PPC_IMAGE_BASE, (uint32_t)PPC_IMAGE_SIZE);
    uint32_t stack_top = PPC_IMAGE_BASE + PPC_IMAGE_SIZE + REX_STACK_SIZE;
    rex_heap_init(stack_top, REX_HEAP_SIZE);
    PPCContext ctx{{}};
    ctx.r1.u32 = stack_top - 0x100;
    std::printf("[rex] booting {name} at entry 0x%08X\\n", PPC_ENTRY_POINT);
    if (PPCFunc* e = ppc_lookup(PPC_ENTRY_POINT)) e(ctx, base);
    else std::fprintf(stderr, "[rex] entry 0x%08X not recompiled\\n", PPC_ENTRY_POINT);
    return 0;
}}
#endif
'''


def _cmakelists(name):
    return f"""cmake_minimum_required(VERSION 3.20)
project({name}_pc CXX)

# c++23 when bridged onto the vendored RexGlue runtime (its context.h uses
# std::byteswap etc.); harmless for the standalone path too.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# the rexglue kernel core. Its PUBLIC target settings (c++23, header-only fmt, include
# roots) propagate to the recompiled code that links it, so the generated functions
# compile against its PPCContext (rex_ppc_min.h bridges onto <rex/ppc/context.h> when
# present). Added first: the shader translator below leaves out its stand-ins for what
# the kernel's base library provides.
if(EXISTS "${{CMAKE_SOURCE_DIR}}/kernel/rex_kernel/CMakeLists.txt")
    add_subdirectory(kernel/rex_kernel)
endif()

# the rexglue (Xenia-derived) DXBC shader translator builds as its own static lib
# (c++23 + header-only fmt) and exposes only the clean rex_dxbc_translate.h API.
if(EXISTS "${{CMAKE_SOURCE_DIR}}/graphics/rex_dxbc/CMakeLists.txt")
    add_subdirectory(graphics/rex_dxbc)
endif()

# every translation unit under src/ (the sharded recomp + init + runtime) and the
# native GPU subsystem under graphics/ (top level only; rex_dxbc/ builds itself)
file(GLOB {name.upper()}_SOURCES CONFIGURE_DEPENDS
     "${{CMAKE_SOURCE_DIR}}/src/*.cpp" "${{CMAKE_SOURCE_DIR}}/graphics/*.cpp")
add_executable({name} ${{{name.upper()}_SOURCES}})
target_include_directories({name} PRIVATE
     "${{CMAKE_SOURCE_DIR}}/include" "${{CMAKE_SOURCE_DIR}}/graphics")

# link the shader translator (also propagates the clean-header include path, which
# is what makes the ShaderTranslator seam's __has_include fire).
if(TARGET rex_dxbc)
    target_link_libraries({name} PRIVATE rex_dxbc)
endif()

# link the RexGlue kernel core. PUBLIC on rex_kernel means this also puts its
# include roots + c++23 + FMT_HEADER_ONLY onto the recompiled code, so rex_ppc_min.h
# bridges onto RexGlue's PPCContext for the whole title.
if(TARGET rex_kernel)
    target_link_libraries({name} PUBLIC rex_kernel)
endif()

# native D3D12 backend (see xe_d3d12.cpp) links the system graphics libraries; on
# MSVC these arrive via #pragma comment(lib, ...).
if(WIN32 AND NOT MSVC)
    target_link_libraries({name} PRIVATE d3d12 dxgi dxguid)
endif()

# recompiled code uses __builtin_bswap / rotate; GCC or Clang recommended.
if (MSVC)
    target_compile_options({name} PRIVATE /bigobj /Od /wd4267 /wd4244 /wd4018)
else()
    target_compile_options({name} PRIVATE -O1 -fno-strict-aliasing -w)
endif()

# so assets/{name}.bin resolves when debugging from the IDE
set_target_properties({name} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${{CMAKE_SOURCE_DIR}}")
"""


def _launcher_bat(name):
    return f"""@echo off
title {name} - recompiled by CodeBreaker
cd /d "%~dp0"

rem locate a built executable (build layout varies by generator)
set "EXE="
if exist "build\\{name}.exe" set "EXE=build\\{name}.exe"
if exist "build\\Release\\{name}.exe" set "EXE=build\\Release\\{name}.exe"
if exist "build\\Debug\\{name}.exe" set "EXE=build\\Debug\\{name}.exe"

if "%EXE%"=="" (
    echo First run - building {name} with CMake...
    where cmake >nul 2>nul || (echo(  CMake not found in PATH. Install CMake, then run this again. & pause & exit /b 1)
    cmake -S . -B build && cmake --build build --config Release
    if exist "build\\{name}.exe" set "EXE=build\\{name}.exe"
    if exist "build\\Release\\{name}.exe" set "EXE=build\\Release\\{name}.exe"
    if exist "build\\Debug\\{name}.exe" set "EXE=build\\Debug\\{name}.exe"
)

if "%EXE%"=="" (
    echo(  Build did not produce {name}.exe - see the output above.
    pause & exit /b 1
)

echo Launching {name} ...
"%EXE%"
echo.
echo {name} exited.
pause
"""


def _readme(name, agg, image_base, entry, shards):
    pct = 100.0 * agg["translated"] / max(1, agg["total"])
    return f"""# {name} - recompiled by CodeBreaker

Static-recompiled from the Xbox 360 title to native C++ (the XenonRecomp method, as the
rexglue SDK carries it out; codebreaker/recompile). Laid out as a real source tree.

## Run it
Double-click **`Play-{name}.bat`** - it builds the project on first run (needs
CMake + a C++ compiler on PATH) and then launches it.

Or build by hand:
```
cmake -S . -B build
cmake --build build --config Release
```

## Source tree
```
include/
  {name}.h            declarations of every recompiled function + the mapping table
  {name}_config.h     image base 0x{image_base:08X}, entry 0x{entry:08X}
  rex_ppc_min.h       PowerPC runtime header (PPCContext, byte-swap memory, helpers)
src/
  {name}_0000.cpp ..  the {agg['funcs']:,} recompiled functions, split across {shards} files
  {name}_init.cpp     the guest-address -> function-pointer table
  rex_runtime.cpp     guest memory, indirect-call table, host-shim registry, boot main()
assets/
  {name}.bin          the decoded guest image, loaded flat at the image base
Play-{name}.bat       double-click launcher
```

Coverage: {agg['translated']:,}/{agg['total']:,} instructions translated
({pct:.1f}%).

## Honest status
This **builds and links** as scaffolded. To actually **run** the whole title the
host runtime still needs filling in (the scaffold gives the exact place for each):
- **Kernel/XAM/CRT host shims** - the imports currently log + no-op.
- **GPU bridge** - the Xenos command processor / shader translation.
- **setjmp/longjmp & thread/heap setup** - scaffolded, needs wiring.
- **What is left untranslated** - `PPC_UNIMPLEMENTED` marks it (padding, and any instruction no
  builder covers); it traps if reached.

See CodeBreaker's `docs/RECOMPILE.md` for how it works and the runtime work that remains.
"""


def main(argv):
    """python -m codebreaker recompile-project <title> [-o <folder>] [--name N] [--title-files <dir>] [--build] [--check [seconds]] [--runtime x360|rexglue] [-j N]

    x360 (the default): XenonRecomp's C++ on CodeBreaker's Xbox 360 runtime (x360project.py), in
    the binary's project folder (Converted/<name>/) unless -o names another. <title> is a .xex, or
    the title as it comes (title.py): a LIVE/CON package, a disc image, a Games-on-Demand folder, a
    folder of its files, or a folder holding one; its files are unpacked into the project's assets/.
    --build builds the program (4 compiles at a time); --check builds it and runs it headless for
    the seconds given (default 60), reporting frames presented, unimplemented imports and any stop.
    rexglue: this module's own translation on the rexglue kernel core (scaffold, below; needs -o)."""
    if not argv or argv[0].startswith("-"):
        print(main.__doc__)
        return 2
    out = argv[argv.index("-o") + 1] if "-o" in argv else None
    runtime = argv[argv.index("--runtime") + 1] if "--runtime" in argv else "x360"
    if runtime == "x360":
        from codebreaker.base import converted
        from codebreaker.recompile import title, x360project
        files = argv[argv.index("--title-files") + 1] if "--title-files" in argv else None
        name = argv[argv.index("--name") + 1] if "--name" in argv else None
        xex_path, found = argv[0], None
        if not (os.path.isfile(xex_path) and open(xex_path, "rb").read(4) == b"XEX2"):
            # a package, disc image, Games-on-Demand folder or folder of files: its default.xex, and its files
            # unpacked into the project's assets/ (once)
            found = title.locate(xex_path)
            if found is None:
                print("%s: no Xbox 360 title found (a default.xex in a package, disc image or folder)" % xex_path)
                return 1
            name = name or found.name
            staging = os.path.join(converted.root(), "_unpacking")
            os.makedirs(staging, exist_ok=True)
            xex_path = os.path.join(staging, "default.xex")
            with open(xex_path, "wb") as f:
                f.write(found.read_xex())
        proj = converted.project(xex_path, name=name) if out is None else None
        if found:
            target = os.path.join(proj.root if proj else out, "assets")
            n = found.unpack(target)
            print("%s: %s" % (found.name, "%d files unpacked from %s" % (n, found.source) if n else "files unpacked before"))
        info = x360project.scaffold(xex_path, proj.root if proj else out, name=name or (proj and proj.name),
                                    title_files=files)
        if proj:
            proj.record("recompile-project", [os.path.join(proj.root, "CMakeLists.txt"), proj.src, proj.assets,
                                              os.path.join(proj.reports, "recompile.log")],
                        notes="%d functions recompiled to C++ (XenonRecomp) for CodeBreaker's Xbox 360 runtime.\n"
                              "Build: Build-%s.bat   Play: Play-%s.bat (keyboard or controller)"
                              % (info["functions"], info["name"], info["name"]))
        print("%s: %d functions, %d switch tables, %d unimplemented instructions" % (
            info["name"], info["functions"], info["switches"], info["unimplemented"]))
        print("  -> %s" % info["dir"])
        print("  build: %s" % os.path.join(info["dir"], "Build-%s.bat" % info["name"]))
        print("  play:  %s" % os.path.join(info["dir"], "Play-%s.bat" % info["name"]))
        if "--build" in argv or "--check" in argv:
            ok, lines = x360project.build(info["dir"], info["name"])
            print("build: %s" % ("ok" if ok else "FAILED"))
            if not ok:
                print("\n".join("  " + line[:300] for line in lines))
                return 1
        if "--check" in argv:
            i = argv.index("--check")
            seconds = int(argv[i + 1]) if i + 1 < len(argv) and argv[i + 1].isdigit() else 60
            r = x360project.check(info["dir"], info["name"], seconds)
            print("check (%d s headless): %d frames presented" % (seconds, r["frames"]))
            for imp, calls in sorted(r["unimplemented"].items(), key=lambda kv: -kv[1]):
                print("  unimplemented import %s (%d calls)" % (imp, calls))
            if r["fatal"]:
                print("  stopped: %s" % r["fatal"][:400])
            print("  log: %s" % r["log"])
        return 0
    if out is None:
        print(main.__doc__)
        return 2
    jobs = int(argv[argv.index("-j") + 1]) if "-j" in argv else None
    agg = scaffold(argv[0], out, jobs=jobs)
    pct = 100.0 * agg["translated"] / max(1, agg["total"])
    print("%s: %d functions in %d source files, %.1f%% of %d instructions translated, %d imports" % (
        agg["name"], agg["funcs"], agg.get("shards", 0), pct, agg["total"], agg.get("imports", 0)))
    print("  -> %s" % agg["dir"])
    print('  build: cmake -S "%s" -B "%s/build" && cmake --build "%s/build"' % (agg["dir"], agg["dir"], agg["dir"]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
