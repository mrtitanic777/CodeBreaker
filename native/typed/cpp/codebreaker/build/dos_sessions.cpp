// codebreaker/build/dos_sessions.cpp -- typed C++ from codebreaker.build.dos_sessions (see dos_sessions.hpp).

#include "codebreaker/build/dos_sessions.hpp"

#include <algorithm>
#include <random>
#include <thread>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace codebreaker::build::dos_sessions {

using py::str;

const py::bytes _WAIT_COM = py::bytes::from_list(py::list<int64_t>{0xB4, 0x86, 0xB9, 0x00, 0x00, 0xBA, 0x50, 0xC3, 0xCD, 0x15, 0xCD, 0x20});

const str _POLL("rescan >nul\r\nif exist C:\\GO goto work\r\nwait\r\n");
const str _WORKER_BAT = str("echo hi > C:\\BOOTED\r\n"
                            ":loop\r\n") +
                        _POLL + _POLL + _POLL + _POLL +
                        str("choice /C:YN /T:Y,1 /N >nul\r\n"
                            "goto loop\r\n"
                            ":work\r\n"
                            "del C:\\GO\r\n"
                            "rescan >nul\r\n"
                            "call C:\\WORK.BAT\r\n"
                            "echo done > C:\\ACK\r\n"
                            "goto loop\r\n");

const str _CONF("[sdl]\r\nautolock=false\r\n"
                "[cpu]\r\ncputype=386\r\ncore=normal\r\ncycles=max\r\n"
                "[dos]\r\nver=6.22\r\n"
                "[autoexec]\r\nmount c \"%s\"\r\nset TMP=C:\\\r\nc:\r\ncall WORKER.BAT\r\n");

// ------------------------------------------------------------------------------------------ Popen

namespace {

std::wstring wide(const str& s) {
    std::string u8 = s.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), w.data(), n);
    return w;
}

// subprocess.list2cmdline
std::wstring list2cmdline(const py::list<str>& args) {
    std::u32string out;
    bool first = true;
    for (const str& arg : args) {
        const std::u32string& a = arg.u32();
        if (!first) out += U' ';
        first = false;
        bool needquote = a.empty() || a.find(U' ') != std::u32string::npos || a.find(U'\t') != std::u32string::npos;
        std::u32string result;
        if (needquote) result += U'"';
        int bs = 0;
        for (char32_t c : a) {
            if (c == U'\\') {
                bs++;
            } else if (c == U'"') {
                result.append(static_cast<size_t>(bs) * 2, U'\\');
                bs = 0;
                result += U"\\\"";
            } else {
                if (bs) {
                    result.append(static_cast<size_t>(bs), U'\\');
                    bs = 0;
                }
                result += c;
            }
        }
        if (bs) result.append(static_cast<size_t>(bs), U'\\');
        if (needquote) {
            result.append(static_cast<size_t>(bs), U'\\');
            result += U'"';
        }
        out += result;
    }
    return wide(str(out));
}

[[noreturn]] void raise_win(DWORD code) {
    std::string msg = "[WinError " + std::to_string(code) + "] the process could not be started";
    if (code == 2 || code == 3) throw py::FileNotFoundError(msg);
    if (code == 5) throw py::PermissionError(msg);
    if (code == 267) throw py::NotADirectoryError(msg);
    throw py::OSError(msg);
}

HANDLE as_handle(void* p) { return static_cast<HANDLE>(p); }

}  // namespace

Popen::Popen(const py::list<str>& args, const py::dict<str, str>& extra_env) {
    // the environment: the parent's, with extra_env's names set (names compare without regard to case)
    std::wstring block;
    {
        std::vector<std::wstring> names;
        for (const auto& kv : extra_env.items()) names.push_back(wide(std::get<0>(kv)));
        LPWCH env = GetEnvironmentStringsW();
        for (const wchar_t* p = env; *p; p += wcslen(p) + 1) {
            std::wstring entry(p);
            size_t eq = entry.find(L'=', 1);
            std::wstring name = entry.substr(0, eq == std::wstring::npos ? entry.size() : eq);
            bool replaced = false;
            for (const auto& n : names)
                if (_wcsicmp(n.c_str(), name.c_str()) == 0) replaced = true;
            if (replaced) continue;
            block += entry;
            block.push_back(L'\0');
        }
        FreeEnvironmentStringsW(env);
        for (const auto& kv : extra_env.items()) {
            block += wide(std::get<0>(kv)) + L"=" + wide(std::get<1>(kv));
            block.push_back(L'\0');
        }
        block.push_back(L'\0');
    }
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE nul = CreateFileW(L"NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    // stdin: the parent's own (a duplicate that can be inherited), else the null device
    HANDLE stdin_dup = nullptr;
    HANDLE parent_in = GetStdHandle(STD_INPUT_HANDLE);
    if (parent_in && parent_in != INVALID_HANDLE_VALUE)
        DuplicateHandle(GetCurrentProcess(), parent_in, GetCurrentProcess(), &stdin_dup, 0, TRUE, DUPLICATE_SAME_ACCESS);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = stdin_dup ? stdin_dup : nul;
    si.hStdOutput = nul;
    si.hStdError = nul;
    PROCESS_INFORMATION pi{};
    std::wstring line = list2cmdline(args);
    BOOL ok = CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, block.data(), nullptr, &si, &pi);
    DWORD err = GetLastError();
    if (nul != INVALID_HANDLE_VALUE) CloseHandle(nul);
    if (stdin_dup) CloseHandle(stdin_dup);
    if (!ok) raise_win(err);
    CloseHandle(pi.hThread);
    handle_ = pi.hProcess;
}

Popen::~Popen() {
    if (handle_) CloseHandle(as_handle(handle_));
}

std::optional<int64_t> Popen::poll() {
    if (returncode_) return returncode_;
    if (WaitForSingleObject(as_handle(handle_), 0) == WAIT_OBJECT_0) {
        DWORD code = 0;
        GetExitCodeProcess(as_handle(handle_), &code);
        returncode_ = static_cast<int64_t>(code);
    }
    return returncode_;
}

void Popen::kill() {
    if (poll()) return;
    TerminateProcess(as_handle(handle_), 1);
}

int64_t Popen::wait(std::optional<double> timeout) {
    if (returncode_) return *returncode_;
    DWORD ms = timeout ? static_cast<DWORD>(*timeout * 1000) : INFINITE;
    if (WaitForSingleObject(as_handle(handle_), ms) != WAIT_OBJECT_0) throw py::TimeoutError("timed out");
    return *poll();
}

// ------------------------------------------------------------------------------------------ helpers

namespace {

// tempfile.mkdtemp(prefix=...): a new folder with a random name in the temporary folder
str mkdtemp(const str& prefix) {
    str tmp;
    for (const char* k : {"TMPDIR", "TEMP", "TMP"}) {
        auto v = py::os::environ_::get(str(k));
        if (v && !v->empty()) {
            tmp = *v;
            break;
        }
    }
    if (tmp.empty()) {
        wchar_t buf[MAX_PATH + 1];
        DWORD n = GetTempPathW(MAX_PATH, buf);
        std::string u8(static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), nullptr, 0, nullptr, nullptr)), '\0');
        WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), u8.data(), static_cast<int>(u8.size()), nullptr, nullptr);
        tmp = str(u8);
    }
    tmp = py::os::path::abspath(tmp);
    static std::mt19937_64 rng{std::random_device{}()};
    const std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789_";
    for (int tries = 0; tries < 100; tries++) {
        std::string name;
        for (int i = 0; i < 8; i++) name.push_back(chars[rng() % chars.size()]);
        str path = py::os::path::abspath(py::os::path::join(tmp, prefix + str(name)));
        if (py::os::path::exists(path)) continue;
        py::os::makedirs(path, 0700, false);
        return path;
    }
    throw py::FileExistsError("No usable temporary directory name found");
}

}  // namespace

// ------------------------------------------------------------------------------------------ DosboxWorker

DosboxWorker::DosboxWorker(const str& masmroot, const str& dbx_, double boot_timeout) : dbx(dbx_) {
    work = mkdtemp(str("dosw_"));
    // colocate ONLY MASM.EXE -- the reconstructor's asm uses no INCLUDE, so the
    // mount stays tiny, which is what makes DOSBox's RESCAN reliably see
    // host-created trigger files (a large mount breaks that).
    py::shutil::copy(py::os::path::join(masmroot, str("BIN"), str("MASM.EXE")), py::os::path::join(work, str("MASM.EXE")));
    py::open(py::os::path::join(work, str("WAIT.COM")), str("wb")).write(_WAIT_COM);
    py::open(py::os::path::join(work, str("WORKER.BAT")), str("w"), std::nullopt, std::nullopt, str("")).write(_WORKER_BAT);
    conf = py::os::path::join(work, str("dbx.conf"));
    py::open(conf, str("w"), std::nullopt, std::nullopt, str("\r\n")).write(py::percent(_CONF, work));
    proc = std::make_unique<Popen>(py::list<str>{dbx, str("-conf"), conf, str("-nolog")},
                                   py::dict<str, str>{{str("SDL_VIDEODRIVER"), str("dummy")}});
    _wait_boot(boot_timeout);
}

str DosboxWorker::_p(const str& name) const { return py::os::path::join(work, name); }

void DosboxWorker::_wait_boot(double timeout) {
    double t = py::time::time();
    while (py::time::time() - t < timeout) {
        if (py::os::path::exists(_p(str("BOOTED")))) return;
        if (proc->poll()) throw py::RuntimeError("DOSBox worker died during boot");
        py::time::sleep(0.05);
    }
    throw py::RuntimeError("DOSBox worker boot timed out");
}

Assembled DosboxWorker::assemble_many(const py::list<std::tuple<str, str>>& items, std::optional<double> timeout,
                                      const std::function<bool()>& cancelled) {
    for (const str& f : py::os::listdir(work)) {
        if (f.endswith(str(".ASM")) || f.endswith(str(".OBJ")) || f.endswith(str(".LST")) || f.endswith(str(".LOG")) ||
            f == str("ACK") || f == str("GO") || f == str("WORK.BAT")) {
            try {
                py::os::remove(_p(f));
            } catch (const py::OSError&) {
            }
        }
    }
    for (const auto& item : items) {
        const str& stem = std::get<0>(item);
        const str& src = std::get<1>(item);
        py::file fh = py::open(_p(stem + str(".ASM")), str("w"), std::nullopt, std::nullopt, str("\r\n"));
        fh.write(src);
        fh.close();
    }
    str cmds;
    for (const auto& item : items) {
        const str& s = std::get<0>(item);
        cmds = cmds + py::percent(str("MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG\r\n"), s, s, s, s);
    }
    py::open(_p(str("WORK.BAT")), str("w"), std::nullopt, std::nullopt, str("")).write(cmds);
    py::open(_p(str("GO")), str("w")).write(str("x"));   // trigger -- written LAST
    double limit = timeout ? *timeout : static_cast<double>(60 + 3 * items.size());
    double t = py::time::time();
    bool acked = false;
    while (py::time::time() - t < limit) {
        if (py::os::path::exists(_p(str("ACK")))) {
            acked = true;
            break;
        }
        if (cancelled && cancelled()) {
            close();
            Assembled r;
            r.aborted = true;
            return r;
        }
        if (proc->poll()) {
            Assembled r;
            r.dead = true;
            return r;
        }
        py::time::sleep(0.02);
    }
    if (!acked) {                                           // hung -> treat as dead
        Assembled r;
        r.dead = true;
        return r;
    }
    py::dict<str, Product> results;
    for (const auto& item : items) {
        const str& stem = std::get<0>(item);
        Product d;
        str p = _p(stem + str(".OBJ"));
        if (py::os::path::exists(p)) d.obj = py::open(p, str("rb")).read();
        p = _p(stem + str(".LST"));
        if (py::os::path::exists(p)) d.lst = py::open(p, str("r"), str("latin1")).read_text();
        p = _p(stem + str(".LOG"));
        if (py::os::path::exists(p)) d.log = py::open(p, str("r"), str("latin1")).read_text();
        results[stem] = d;
    }
    Assembled r;
    r.results = results;
    r.work = work;
    return r;
}

bool DosboxWorker::alive() { return !proc->poll().has_value(); }

void DosboxWorker::close() {
    try {
        proc->kill();
        proc->wait(3.0);
    } catch (const py::Exception&) {
    }
    py::shutil::rmtree(work, true);
}

// ------------------------------------------------------------------------------------------ pool

void Semaphore::acquire() {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [&] { return n_ > 0; });
    n_--;
}

void Semaphore::release() {
    {
        std::lock_guard<std::mutex> lk(m_);
        n_++;
    }
    cv_.notify_one();
}

WorkerPool::WorkerPool(const str& masmroot_, const str& dbx_, int64_t maxsize) : masmroot(masmroot_), dbx(dbx_), _sem(maxsize) {}

std::shared_ptr<DosboxWorker> WorkerPool::lease() {
    _sem.acquire();
    {
        std::lock_guard<std::mutex> lk(_lock);
        if (!_idle.empty()) {
            std::shared_ptr<DosboxWorker> w = _idle.back();
            _idle.pop_back();
            return w;
        }
    }
    std::shared_ptr<DosboxWorker> w;
    try {
        w = std::make_shared<DosboxWorker>(masmroot, dbx);
    } catch (...) {
        _sem.release();
        throw;
    }
    std::lock_guard<std::mutex> lk(_lock);
    _all.push_back(w);
    return w;
}

void WorkerPool::release(const std::shared_ptr<DosboxWorker>& w, bool keep) {
    {
        std::lock_guard<std::mutex> lk(_lock);
        auto it = std::find(_all.begin(), _all.end(), w);
        if (keep && it != _all.end() && w->alive()) {
            _idle.push_back(w);
        } else {
            if (it != _all.end()) _all.erase(it);
            w->close();
        }
    }
    _sem.release();
}

void WorkerPool::close() {
    std::lock_guard<std::mutex> lk(_lock);
    for (const auto& w : _all) w->close();
    _all.clear();
    _idle.clear();
}

}  // namespace codebreaker::build::dos_sessions
