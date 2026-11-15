# CodeBreaker on Linux

Track 8 of `docs/ROADMAP.md`: `codebreaker` builds and runs on Linux (x86-64, GCC and Clang) as it does on Windows.
This page is the inventory of what is Windows-specific in `native/cb`, the portable layer in `native/cb/core` that
the rest moves onto, and the state of the port.

## 1. Inventory (2026-11-14)

Counted over `native/cb` (test data and the Xbox 360 runtime left out):

- **65 source files** include a Windows or Windows-CRT header (`<windows.h>`, `<winsock2.h>`, `<bcrypt.h>`, `<psapi.h>`,
  `<io.h>`, `<fcntl.h>`): 14 in `core` and **51 outside it**.
- **10 files outside `core`** convert paths and text through `cb::win` (`to_wide`/`from_wide`), several without calling the Windows API
  themselves; they only exist because a path is a wide string on Windows.
- About **300 files** name a `.exe`, `.dll`, `.bat` or `.cmd`, and about **90** write a drive path (`C:/...`) or a
  backslash path in code or a test: tool lookups and test inputs, not API calls, but each needs a platform branch or a
  neutral spelling.
- 17 files (mostly `build`, `decompile`) use compiler extensions (`__declspec`, `__cdecl`, `__stdcall`, `_MSC_VER`,
  `#pragma comment`); nearly all are the Win16/Win32 code the tools *model*, not code CodeBreaker runs.

### Windows API use by area

`core` is the layer itself (section 2). The rest, with the core interface each use goes through:

| Area (files) | What it calls | Goes through |
|---|---|---|
| `core` (14) | CreateFile/ReadFile/WriteFile/MapViewOfFile, CreateDirectory, MoveFileEx, SetFileTime, LockFileEx, CreateProcess + Job Objects + pipes, LoadLibrary/GetProcAddress, Get/SetEnvironmentVariable, console code page, GetLocalTime, GlobalMemoryStatusEx, SetPriorityClass, BCrypt hashes | `fs`, `file_lock`, `process`, `dynlib`, `sys`, `crypto` (done) |
| `ui` (11) | Winsock (`serve`, `mcp_http`, `service_client`), CreateProcess/Job Objects for the service and its jobs (`service_client`, `service_jobs`), `_dup`/`_dup2`/`_setmode` to take stdout (`mcp_protocol`, `service`), `_isatty` (`progress`), GetCurrentProcessId, GetModuleFileName | `socket`, `process`, `sys::is_terminal`, `sys::StreamRedirect`, `fs::process_id`, `fs::executable_path` |
| `java` (5 + 2) | `platforms.cpp`: a software platform of Winsock, threads, VirtualAlloc, QueryPerformanceCounter that the translated Java programs run on; `runtime_texts.cpp`, `debug.cpp`: CreateDirectory, Sleep, GetTickCount | `socket`, `std::thread`, `fs::monotonic_ticks`, `fs::make_dirs` (platform runtime for the translated programs: its own POSIX twin) |
| `base` (5) | `config.cpp`: GetModuleFileName; `corpus.cpp`: SetEnvironmentVariableW; `parallel.cpp`: CreateFile for a lock | `fs::executable_path`, `fs::set_env`, `file_lock` |
| `app` (4) | `allocator_main.cpp`: VirtualAlloc reserve; `commands.cpp`: `_isatty`; `codebreaker_main.cpp`: GetEnvironmentVariable, wide PYTHONPATH; `exe_gate_main.cpp`: console code page | `sys` (`is_terminal`, `use_utf8_console`), `fs::get_env`/`set_env`; the allocator is a Windows-only link-time choice (ordinary `malloc` elsewhere) |
| `jobs` (4) | `workers.cpp`: GetModuleFileName; `pool.cpp`: SetThreadPriority; `worker_probe_main.cpp`: TerminateProcess | `fs::executable_path`, `sys::lower_priority`, `process` |
| `decompile` (5) | tests: SetEnvironmentVariableA, GetModuleFileName; `fake_decompiler_main.cpp`: CreateFile/WriteFile on stdout | `fs::set_env`, `fs::executable_path`, `sys::write_stream` |
| `verify` (3) | Winsock for the QEMU monitor (`setup16`, `fake_qemu_main`), `_putenv_s`, GetModuleFileName (`regress`) | `socket`, `fs::set_env`, `fs::executable_path` |
| `build` (4) | Winsock names in `flat_thunk` (the thunk compiler's own `connect` grammar, not sockets), headers only | none (false hits) |
| `formats` (3) | `xbox/crypto`: BCrypt AES, RSA, random, sign/verify (`xecrypt`, `xecrypt_rsa`, `stfs`) | `crypto` (to extend: AES-CBC/ECB, RSA key pairs, signatures, random bytes; portable implementations) |
| `exact` (2) | `emu/dos.cpp`: wide paths to the DOS engine DLL; `toolchain.cpp`: headers | `fs::to_text`, `dynlib` (the engine DLL is a Windows PE; its Linux build is track 8 phase 3) |
| `emulate` (1) | `tracer/libs.cpp`: LoadLibrary/GetProcAddress of the caller's Unicorn | `dynlib` |
| `coverage` (1) | `reach.cpp`: GetModuleFileName | `fs::executable_path` |
| `recompile` (1) | `gpu.cpp`: D3D12 text (a generated file, not a call) | none |
| `port` (1) | `tool_run.cpp`: GetProcessMemoryInfo | `sys` (to add: peak memory of a child) |
| `translate` (1) | test data text | none |

### Process, file and text idioms outside the files above

| Idiom | Where | Goes through |
|---|---|---|
| `win::to_wide(path text)` to make a `std::filesystem::path` | `testing`, `exact/emu/dos`, `port/verify`, `ui/service_client` | `fs::from_text` / `fs::to_text` |
| `path.make_preferred().native()` as a program name | `port/verify`, `ui/service_client` | `process::ProcessSpec::argv` (the layer resolves the program) |
| `cmd /c`, `.bat`, `.cmd`, `where`, `findstr`, `ping`, `sort` | tests and tool drivers in `process`, `port`, `jobs`, `ui`, `coverage` | the test names the platform's word (`process_test.cpp`), the driver is a Windows-lane tool |
| `;` as the PATH separator, `PATHEXT` | `process` | inside the layer |
| `\\?\` long paths, case-insensitive `same_path` | `fs` | inside the layer |
| `C:/...` literals | tests of `symbols`, `build`, `source`, `formats`, `recompile`, `port` | `testing::data_path` / `fs::ScopedTempDir` instead of literals |
| 8.3 short names of the temp folder | `fs::full_names` | inside the layer (identity on POSIX) |

## 2. The portable layer (`native/cb/core`)

One interface per concern, in a header with no platform types; the implementation is the same file name with a
`_win.cpp` or `_posix.cpp` suffix (each guarded by `#ifdef _WIN32`, so a glob compiles both on every platform and
CMake needs no per-platform source list).

| Header | Interface | Windows | POSIX |
|---|---|---|---|
| `fs.hpp` | `read_file`, `write_file`, `write_file_atomic`, `append_file`, `make_dirs`, `remove_tree`, `file_size`, `copy_tree`, `MappedFile`, `ScopedTempDir`, `to_text`/`from_text` (UTF-8 at the interface), `same_path`, `long_path`, `full_names` | Win32 W calls (`fs_win.cpp`) | `open`/`mmap`/`rename` (`fs_posix.cpp`) |
| | `last_write_time`, `set_last_write_time` (100 ns, files and folders) | `GetFileTime`/`SetFileTime` | `stat`/`utimensat` |
| | `get_env`, `set_env`, `ScopedEnv`, `process_id`, `monotonic_ticks`, `unique_suffix`, `executable_path`, `set_read_only` | `GetEnvironmentVariableW` + CRT copy, `GetModuleFileNameW` | `getenv`/`setenv`, `/proc/self/exe`, `chmod` |
| `file_lock.hpp` | `try_acquire`, `acquire`, `holder_of`: a byte-range lock that goes with the process | `LockFileEx` | `fcntl(F_OFD_SETLK)` (open-file-description lock: refused inside one process too) |
| `process.hpp` | `run`, `run_checked`, `start`/`Process` (read, write, wait, kill): argv, cwd, env, capture modes, timeout, priority, memory limit; every child in a job that dies with it | Job Object, handle list, `CreateProcessW` (`process_win.cpp`) | `fork`/`execve` in a session of its own, `killpg`, close-on-exec pipes, `setrlimit`, `nice` (`process_posix.cpp`) |
| `dynlib.hpp` | `DynLib::load_beside_exe("unicorn.dll")` (the name is the stem on every platform), `load`, `function<Fn>`, `file_name_for` | `LoadLibraryExW` | `dlopen` (beside the executable, then the system's `libcapstone.so.N`) |
| `sys.hpp` | `use_utf8_console`, `is_terminal`, `write_stream`, `StreamRedirect`, `local_time`, `local_tm`, `available_memory_gb`, `total_memory_gb`, `cpu_times`, `lower_priority`, `lower_thread_priority`, `limit_cores`, `error_message`, `last_error` | console code page, `WriteFile`, `SetStdHandle`, `GetLocalTime`, `localtime_s`, `GlobalMemoryStatusEx`, `GetSystemTimes` | `isatty`, `write`, `dup2`, `localtime_r`, `/proc/meminfo`, `/proc/stat`, `nice`, `sched_setaffinity` |
| `socket.hpp` | `net::Listener::listen/accept`, `net::connect`, `net::Socket::receive/send/set_receive_timeout`: TCP over IPv4 | Winsock (`socket.cpp`) | BSD sockets (the same file: the two differ in a dozen names, spelled once at its top) |
| `random_file.hpp` | positioned `read_at`/`write_at`, `size`, `flush` | `OVERLAPPED` offsets | `pread`/`pwrite` |
| `crypto.hpp` | `Hasher` (MD5, SHA-1, SHA-256, SHA-512), `crc32`, `adler32`, hex | CNG (`crypto_win.cpp`) | own implementation (`crypto_posix.cpp`) |
| `win.hpp` | the wide-string boundary: Windows-only; nothing outside the Windows implementations (and the Windows-lane tools) should include it | | (not built) |
| `log.hpp`, `parallel.hpp`, `testing.hpp` | built on the above; no Windows calls of their own | | |

Rules for code outside `core`: no `<windows.h>`, no `_w*` CRT, no wide strings, no `\\` or drive letters in paths made
by code, no `cmd`: go through the interface above, or put the code in a `_win.cpp`/`_posix.cpp` pair of its own area.

## 3. State

2026-11-14, Ubuntu 24.04 (WSL), GCC 13, `cmake -S native -B state/work/linux-build -G Ninja`:

- **`cb_core` and its tests: 139 of 139 pass** (`cmake --build state/work/linux-build --target cb_tests_core`, then
  `state/work/linux-build/bin/cb_tests_core`). The Capstone and Unicorn tests are skipped on a machine that has no
  `libcapstone.so`/`libunicorn.so` (Z3 is found: the system's `libz3.so.4`). On Windows the same tests, MSVC: 140 of 140;
  GCC and clang-cl keep building.
- **Compile on Linux with `core` portable** (static libraries `cb_base cb_jobs cb_knowledge cb_platforms cb_symbols
  cb_compare cb_analyze cb_emulate cb_engine`): all but six files. What stops each:
  - `formats/xbox/crypto/xecrypt.cpp`, `xecrypt_rsa.cpp`, `stfs.cpp`: BCrypt AES, RSA and signatures. They need portable
    implementations or OpenSSL; this machine has `libcrypto.so.3` but not its headers.
  - `verify/gate16/setup16.cpp`: Winsock (move onto `core/socket.hpp`).
  - `translate/frontends/python/wide.cpp`: `<intrin.h>` (`_umul128` and the like).
  - `verify/regress.cpp`: moved onto `fs::set_env`, `fs::process_id`, `fs::executable_path` (not yet rebuilt on Linux).
- Needed for the tests that run engines: `libunicorn.so` and `libcapstone.so` (build them from the sources the prebuilt
  DLLs came from, or install `libcapstone-dev` and a Unicorn 2 package) and, for the DOS engine, a Linux build of
  `native/dosexec` (its C interface takes `wchar_t` paths: 32-bit on Linux).

Next, in order: `socket` into `ui`, `verify` and `java`; the xbox crypto; the engines as shared libraries (roadmap track 8,
phase 3); `tools/` shell equivalents.
