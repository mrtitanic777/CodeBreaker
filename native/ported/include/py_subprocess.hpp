// py_subprocess.hpp -- subprocess.run for Windows (CreateProcessW), and the winerror text CPython gives an
// OSError. Separate from py_stdlib.hpp because it needs <windows.h>; include it where a program is run.
//
//   py::subprocess::run(args, opts)  -> CompletedProcess {returncode, stdout_, stderr_ (text) or the bytes}
//                                       opts: cwd, env, timeout (TimeoutExpired), shell, input, stdin/stdout/stderr, text
//   py::subprocess::to_wide(str)     -> std::wstring (UTF-16, a code point past the BMP as a pair)
//   py::raise_winerror(code)         throws the OSError subclass CPython does, "[WinError N] text"
//
// Text mode is what Python's is on a Windows locale: cp1252 decoding (errors strict) and universal newlines;
// text sent on stdin has "\n" written as "\r\n" and is encoded the same way.
#pragma once

#include <string>
#include <thread>
#include <variant>

#include "py.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace py {

// CPython's text for a Windows error code (FormatMessage, trailing blanks and dots removed)
inline std::string winerror_text(DWORD code) {
    wchar_t buf[1024];
    DWORD n = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buf, 1024, nullptr);
    while (n > 0 && (buf[n - 1] <= L' ' || buf[n - 1] == L'.')) n--;
    if (n == 0) return "Unknown error " + std::to_string(code);
    int m = WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), nullptr, 0, nullptr, nullptr);
    std::string u8(static_cast<size_t>(m), '\0');
    WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), u8.data(), m, nullptr, nullptr);
    return u8;
}

[[noreturn]] inline void raise_winerror(DWORD code) {
    std::string msg = "[WinError " + std::to_string(code) + "] " + winerror_text(code);
    switch (code) {   // (CPython's winerror -> errno -> OSError subclass)
        case 2: case 3: case 15: case 18: case 53: case 67: case 161: case 206: throw FileNotFoundError(msg);
        case 5: case 16: case 19: case 20: case 21: case 22: case 23: case 24: case 32: case 33: case 65: case 82: throw PermissionError(msg);
        case 80: case 183: throw FileExistsError(msg);
        case 267: throw NotADirectoryError(msg);
        default: throw OSError(msg);
    }
}

namespace subprocess {

inline std::wstring to_wide(const str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (v >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (v & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// where a child's stream goes
enum class Stream { Capture, Devnull, Inherit, Stdout /* (stderr only: into stdout's pipe) */ };

struct RunOptions {
    std::optional<str> cwd;
    std::optional<str> input;   // text to send on stdin (stdin is then a pipe); without it stdin is `stdin_`
    Stream stdin_ = Stream::Inherit;
    Stream stdout_ = Stream::Capture;
    Stream stderr_ = Stream::Capture;
    bool text = true;           // decode what was captured (else it is bytes)
    std::string encoding = "cp1252";
    std::string errors = "strict";
    DWORD creationflags = 0;
    std::optional<std::variant<int64_t, double>> timeout;   // seconds (an int prints as 5, a float as 5.0 in TimeoutExpired)
    std::optional<dict<str, str>> env;                      // the child's whole environment (else it inherits ours)
    bool shell = false;
};

struct CompletedProcess {
    int64_t returncode = 0;   // (an unsigned exit code, as Windows gives it: 0xC0000005 is 3221225477)
    std::optional<str> stdout_, stderr_;                 // text mode
    std::optional<bytes> stdout_bytes, stderr_bytes;     // bytes mode
};

namespace detail_ {
// the text of what a child wrote: decoded, "\r\n" and "\r" as "\n"
inline str to_text(const std::string& raw, const RunOptions& o) {
    std::u32string u = bytes(raw).decode(o.encoding.c_str(), o.errors.c_str()).u32();
    std::u32string out;
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') ++i;
        } else {
            out.push_back(u[i]);
        }
    }
    return str(out);
}
inline HANDLE dup_inheritable(HANDLE h) {
    if (!h || h == INVALID_HANDLE_VALUE) return nullptr;
    HANDLE d = nullptr;
    if (!DuplicateHandle(GetCurrentProcess(), h, GetCurrentProcess(), &d, 0, TRUE, DUPLICATE_SAME_ACCESS)) return nullptr;
    return d;
}
}  // namespace detail_

// subprocess.TimeoutExpired: the child was ended (TerminateProcess) and what it had written is attached
struct TimeoutExpired : Exception {
    std::string cmd_text;             // str(cmd): the list's repr, or the command line
    std::variant<int64_t, double> timeout = 0.0;   // as given: an int stays an int in the text
    std::optional<str> stdout_, stderr_;                 // text mode
    std::optional<bytes> stdout_bytes, stderr_bytes;     // bytes mode
    explicit TimeoutExpired(const std::string& message) : Exception(message) {}
    const char* type() const override { return "TimeoutExpired"; }
};

namespace detail_ {
// the environment block of CreateProcessW for `env`: "k=v\0k=v\0\0" (CPython: no name may hold an '=' past its first
// character, and no text an embedded NUL)
inline std::wstring env_block(const dict<str, str>& env) {
    std::wstring block;
    for (const auto& kv : env.items()) {
        const str& k = std::get<0>(kv);
        const str& v = std::get<1>(kv);
        if (k.find(str("="), 1) != -1) throw ValueError("illegal environment variable name");
        if (k.find(str(std::string(1, '\0'))) != -1 || v.find(str(std::string(1, '\0'))) != -1) throw ValueError("embedded null character");
        block += to_wide(k) + L"=" + to_wide(v);
        block.push_back(L'\0');
    }
    if (block.empty()) block.push_back(L'\0');   // (an empty environment is two NULs)
    block.push_back(L'\0');
    return block;
}
inline std::string timeout_text(const std::variant<int64_t, double>& t) {
    return std::holds_alternative<int64_t>(t) ? std::to_string(std::get<int64_t>(t)) : to_str(std::get<double>(t)).utf8();
}
}  // namespace detail_

namespace detail_ {
// CreateProcessW over `cmd` (a Windows command line) with the options; `shown` is what str(cmd) is in a TimeoutExpired.
inline CompletedProcess run_cmdline(const std::wstring& cmd_in, const std::string& shown, const RunOptions& o, bool hide_window) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr, in_r = nullptr, in_w = nullptr;
    std::vector<HANDLE> to_close;   // child-side handles closed once the child has them
    auto fail = [&](DWORD code) {
        for (HANDLE h : {out_r, err_r, in_w})
            if (h) CloseHandle(h);
        for (HANDLE h : to_close) CloseHandle(h);
        raise_winerror(code);
    };
    auto nul = [&](DWORD access) {
        HANDLE h = CreateFileW(L"NUL", access, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
        if (h != INVALID_HANDLE_VALUE) to_close.push_back(h);
        return h == INVALID_HANDLE_VALUE ? nullptr : h;
    };
    std::wstring block;
    if (o.env) block = env_block(*o.env);   // (a bad environment is a ValueError before anything is made)
    bool in_pipe = o.input.has_value() || o.stdin_ == Stream::Capture;
    if (in_pipe) {
        if (!CreatePipe(&in_r, &in_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(in_r);
        SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    } else if (o.stdin_ == Stream::Devnull) {
        in_r = nul(GENERIC_READ);
    } else {
        in_r = dup_inheritable(GetStdHandle(STD_INPUT_HANDLE));
        if (in_r) to_close.push_back(in_r);
    }
    if (o.stdout_ == Stream::Capture) {
        if (!CreatePipe(&out_r, &out_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(out_w);
        SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    } else if (o.stdout_ == Stream::Devnull) {
        out_w = nul(GENERIC_WRITE);
    } else {
        out_w = dup_inheritable(GetStdHandle(STD_OUTPUT_HANDLE));
        if (out_w) to_close.push_back(out_w);
    }
    HANDLE child_err = nullptr;
    if (o.stderr_ == Stream::Capture) {
        if (!CreatePipe(&err_r, &err_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(err_w);
        SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);
        child_err = err_w;
    } else if (o.stderr_ == Stream::Stdout) {
        child_err = out_w;
    } else if (o.stderr_ == Stream::Devnull) {
        child_err = nul(GENERIC_WRITE);
    } else {
        child_err = dup_inheritable(GetStdHandle(STD_ERROR_HANDLE));
        if (child_err) to_close.push_back(child_err);
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    if (hide_window) {   // (shell=True: CPython hides the console window)
        si.dwFlags |= STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
    }
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = child_err;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = cmd_in;
    std::wstring wcwd = o.cwd ? to_wide(*o.cwd) : std::wstring();
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, o.creationflags | CREATE_UNICODE_ENVIRONMENT,
                             o.env ? block.data() : nullptr, o.cwd ? wcwd.c_str() : nullptr, &si, &pi);
    DWORD err = GetLastError();
    if (!ok) fail(err);
    CloseHandle(pi.hThread);
    // the child's ends belong to the child now
    for (HANDLE h : to_close) CloseHandle(h);
    to_close.clear();
    auto drain = [](HANDLE r, std::string& dst) {
        char buf[8192];
        DWORD got = 0;
        while (r && ReadFile(r, buf, sizeof buf, &got, nullptr) && got > 0) dst.append(buf, got);
    };
    std::string raw_out, raw_err, raw_in;
    if (o.input) {
        str t = o.input->replace(str("\n"), str("\r\n"));
        raw_in = o.text ? t.encode(o.encoding.c_str(), o.errors.c_str()).raw() : t.encode("utf-8").raw();
    }
    std::thread t_out([&] { drain(out_r, raw_out); });
    std::thread t_err([&] { drain(err_r, raw_err); });
    std::thread t_in([&] {   // (the child may not read its input: a thread, so a timeout still comes)
        if (!in_w) return;
        size_t off = 0;
        DWORD wrote = 0;
        while (off < raw_in.size() && WriteFile(in_w, raw_in.data() + off, static_cast<DWORD>(raw_in.size() - off), &wrote, nullptr) && wrote > 0)
            off += wrote;
        CloseHandle(in_w);
        in_w = nullptr;
    });
    DWORD wait_ms = INFINITE;
    if (o.timeout) {
        double secs = std::holds_alternative<int64_t>(*o.timeout) ? static_cast<double>(std::get<int64_t>(*o.timeout)) : std::get<double>(*o.timeout);
        wait_ms = secs <= 0 ? 0 : static_cast<DWORD>(std::min(secs * 1000.0, 4000000000.0));
    }
    bool timed_out = WaitForSingleObject(pi.hProcess, wait_ms) == WAIT_TIMEOUT;
    if (timed_out) {   // CPython: process.kill() (TerminateProcess, exit code 1), then communicate() collects what is left
        TerminateProcess(pi.hProcess, 1);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    t_in.join();
    t_out.join();
    t_err.join();
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    if (out_r) CloseHandle(out_r);
    if (err_r) CloseHandle(err_r);
    CompletedProcess r;
    r.returncode = static_cast<int64_t>(code);
    // With two or more pipes CPython's communicate reads on threads, and a stream whose text cannot be decoded
    // (errors "strict") ends the thread's read: that stream comes back None and nothing is raised. With one
    // pipe it reads in place and the UnicodeDecodeError is raised.
    int pipes = (in_pipe ? 1 : 0) + (o.stdout_ == Stream::Capture ? 1 : 0) + (o.stderr_ == Stream::Capture ? 1 : 0);
    auto text_of = [&](const std::string& raw) -> std::optional<str> {
        try {
            return detail_::to_text(raw, o);
        } catch (const UnicodeDecodeError&) {
            if (pipes < 2) throw;
            return std::nullopt;
        }
    };
    if (o.stdout_ == Stream::Capture) {
        if (o.text) r.stdout_ = text_of(raw_out);
        else r.stdout_bytes = bytes(raw_out);
    }
    if (o.stderr_ == Stream::Capture) {
        if (o.text) r.stderr_ = text_of(raw_err);
        else r.stderr_bytes = bytes(raw_err);
    }
    if (timed_out) {
        std::variant<int64_t, double> t = *o.timeout;
        TimeoutExpired e("Command '" + shown + "' timed out after " + timeout_text(t) + " seconds");
        e.cmd_text = shown;
        e.timeout = t;
        e.stdout_ = r.stdout_;
        e.stderr_ = r.stderr_;
        e.stdout_bytes = r.stdout_bytes;
        e.stderr_bytes = r.stderr_bytes;
        throw e;
    }
    return r;
}

// shell=True: COMSPEC /c "command"
inline std::wstring shell_cmdline(const str& command) {
    str comspec = os::environ_::get(str("COMSPEC")).value_or(str("cmd.exe"));
    return to_wide(comspec + str(" /c \"") + command + str("\""));
}
}  // namespace detail_

// subprocess.run(args, cwd=, env=, timeout=, shell=, input=, stdin=, stdout=, stderr=, text=, creationflags=)
// A list is joined as list2cmdline does. With shell=True the command line is COMSPEC /c "..." and the window is hidden.
// env is the whole environment of the child (not additions to ours: os.environ.copy() and then set, as Python's callers do).
// timeout: the child is ended and TimeoutExpired is raised, holding what it wrote.
inline CompletedProcess run(const list<str>& args, const RunOptions& o = RunOptions()) {
    str line = list2cmdline(args);
    if (o.shell) return detail_::run_cmdline(detail_::shell_cmdline(line), repr(args).utf8(), o, true);
    return detail_::run_cmdline(to_wide(line), repr(args).utf8(), o, false);
}
// subprocess.run("command line", ...): a string is the command line itself (with shell=True it goes through COMSPEC /c)
inline CompletedProcess run(const str& command, const RunOptions& o = RunOptions()) {
    if (o.shell) return detail_::run_cmdline(detail_::shell_cmdline(command), command.utf8(), o, true);
    return detail_::run_cmdline(to_wide(command), command.utf8(), o, false);
}

}  // namespace subprocess
}  // namespace py
