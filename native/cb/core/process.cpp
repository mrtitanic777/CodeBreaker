#include "cb/core/process.hpp"

#include <windows.h>

#include <algorithm>
#include <format>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

namespace cb::process {

namespace {

using Clock = std::chrono::steady_clock;

// RAII for a Win32 HANDLE.
class Handle {
public:
    Handle() = default;
    explicit Handle(HANDLE h) : h_(h) {}
    Handle(Handle&& o) noexcept : h_(o.release()) {}
    Handle& operator=(Handle&& o) noexcept {
        if (this != &o) reset(o.release());
        return *this;
    }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    ~Handle() { reset(); }
    HANDLE get() const { return h_; }
    bool valid() const { return h_ != nullptr && h_ != INVALID_HANDLE_VALUE; }
    HANDLE release() {
        HANDLE h = h_;
        h_ = nullptr;
        return h;
    }
    void reset(HANDLE h = nullptr) {
        if (valid()) CloseHandle(h_);
        h_ = h;
    }

private:
    HANDLE h_ = nullptr;
};

[[noreturn]] void cannot_start(const std::string& tool, DWORD code) {
    throw ToolError(tool, 0, win::error_message(code), "cannot start");
}

// ---- finding the program ------------------------------------------------------------------------------

using Env = text::IMap<std::string>;   // Windows environment names are case-insensitive

Env inherited_env() {
    Env env;
    wchar_t* block = GetEnvironmentStringsW();
    if (block == nullptr) return env;
    for (const wchar_t* p = block; *p != L'\0'; p += wcslen(p) + 1) {
        const std::wstring_view entry(p);
        const std::size_t eq = entry.find(L'=', 1);   // (names like "=C:" start with '=')
        if (eq == std::wstring_view::npos) continue;
        env[win::from_wide(entry.substr(0, eq))] = win::from_wide(entry.substr(eq + 1));
    }
    FreeEnvironmentStringsW(block);
    return env;
}

Env effective_env(const ProcessSpec& spec) {
    Env env = inherited_env();
    for (const auto& name : spec.env_remove) env.erase(name);
    for (const auto& [name, value] : spec.env) env[name] = value;
    return env;
}

// The environment block CreateProcessW wants: NAME=VALUE\0 ... \0\0, sorted case-insensitively (IMap is).
std::wstring env_block(const Env& env) {
    std::wstring block;
    for (const auto& [name, value] : env) {
        block += win::to_wide(name);
        block += L'=';
        block += win::to_wide(value);
        block += L'\0';
    }
    if (block.empty()) block += L'\0';
    block += L'\0';
    return block;
}

bool has_path_part(std::string_view s) { return s.find_first_of("/\\:") != std::string_view::npos; }

bool is_file(const std::filesystem::path& p) {
    const DWORD attrs = GetFileAttributesW(p.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

// argv[0] resolved the way CreateProcess would, but through the *child's* environment (a spec that overrides
// PATH finds its tools there) and its working directory.
std::optional<std::filesystem::path> resolve_program(const std::string& program, const Env& env,
                                                     const std::optional<std::filesystem::path>& cwd) {
    std::vector<std::string> exts = {""};
    const bool has_ext = std::filesystem::path(fs::from_text(program)).has_extension();
    if (!has_ext) {
        std::string pathext = ".COM;.EXE;.BAT;.CMD";
        if (auto it = env.find("PATHEXT"); it != env.end() && !it->second.empty()) pathext = it->second;
        exts.clear();
        for (auto e : text::split(pathext, ';')) {
            if (!e.empty()) exts.emplace_back(e);
        }
    }
    auto try_in = [&](const std::filesystem::path& base) -> std::optional<std::filesystem::path> {
        for (const auto& ext : exts) {
            std::filesystem::path candidate = base;
            candidate += win::to_wide(ext);
            if (is_file(candidate)) return candidate;
        }
        return std::nullopt;
    };
    const std::filesystem::path prog = fs::from_text(program);
    if (has_path_part(program)) {
        if (prog.is_relative() && cwd) return try_in(*cwd / prog);
        return try_in(prog);
    }
    if (cwd) {
        if (auto found = try_in(*cwd / prog)) return found;
    }
    if (auto it = env.find("PATH"); it != env.end()) {
        for (auto dir : text::split(it->second, ';')) {
            if (dir.empty()) continue;
            if (auto found = try_in(fs::from_text(text::trim(dir, "\"")) / prog)) return found;
        }
    }
    return std::nullopt;
}

// ---- pipes and the job --------------------------------------------------------------------------------

struct Pipe {
    Handle read, write;
};

SECURITY_ATTRIBUTES inheritable() {
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    return sa;
}

// A pipe whose `child_end` is inheritable and whose other end is ours only.
Pipe make_pipe(bool child_reads) {
    SECURITY_ATTRIBUTES sa = inheritable();
    HANDLE r = nullptr, w = nullptr;
    if (!CreatePipe(&r, &w, &sa, 1 << 16)) throw IoError("<pipe>", static_cast<int>(GetLastError()), "cannot create pipe");
    Pipe p{Handle(r), Handle(w)};
    SetHandleInformation(child_reads ? p.write.get() : p.read.get(), HANDLE_FLAG_INHERIT, 0);
    return p;
}

Handle open_nul(bool for_read) {
    SECURITY_ATTRIBUTES sa = inheritable();
    Handle h(CreateFileW(L"NUL", for_read ? GENERIC_READ : GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
    if (!h.valid()) throw IoError("NUL", static_cast<int>(GetLastError()), "cannot open NUL");
    return h;
}

Handle open_output_file(const std::filesystem::path& file) {
    if (file.has_parent_path()) fs::make_dirs(file.parent_path());
    SECURITY_ATTRIBUTES sa = inheritable();
    Handle h(CreateFileW(file.c_str(), GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
    if (!h.valid()) throw IoError(file, static_cast<int>(GetLastError()), "cannot open output file");
    return h;
}

// An inheritable duplicate of one of our own standard handles (or NUL if we have none).
Handle inherit_std(DWORD which) {
    HANDLE std_h = GetStdHandle(which);
    if (std_h == nullptr || std_h == INVALID_HANDLE_VALUE) return open_nul(which == STD_INPUT_HANDLE);
    HANDLE dup = nullptr;
    if (!DuplicateHandle(GetCurrentProcess(), std_h, GetCurrentProcess(), &dup, 0, TRUE, DUPLICATE_SAME_ACCESS))
        return open_nul(which == STD_INPUT_HANDLE);
    return Handle(dup);
}

Handle make_job(const ProcessSpec& spec) {
    Handle job(CreateJobObjectW(nullptr, nullptr));
    if (!job.valid()) throw ToolError("<job>", 0, win::error_message(GetLastError()), "cannot create job object");
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
    limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (spec.memory_limit != 0) {
        limits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
        limits.JobMemoryLimit = static_cast<SIZE_T>(spec.memory_limit);
    }
    SetInformationJobObject(job.get(), JobObjectExtendedLimitInformation, &limits, sizeof(limits));
    return job;
}

}  // namespace

// ---- quoting ------------------------------------------------------------------------------------------

std::string quote_arg(std::string_view arg) {
    if (!arg.empty() && arg.find_first_of(" \t\n\v\"") == std::string_view::npos) return std::string(arg);
    std::string out = "\"";
    std::size_t backslashes = 0;
    for (char c : arg) {
        if (c == '\\') {
            ++backslashes;
            continue;
        }
        if (c == '"') {
            out.append(backslashes * 2 + 1, '\\');   // double the run before a quote, then escape the quote
            out.push_back('"');
        } else {
            out.append(backslashes, '\\');
            out.push_back(c);
        }
        backslashes = 0;
    }
    out.append(backslashes * 2, '\\');   // a trailing run must not escape the closing quote
    out.push_back('"');
    return out;
}

std::string command_line(const std::vector<std::string>& argv) {
    std::string out;
    for (const auto& a : argv) {
        if (!out.empty()) out.push_back(' ');
        out += quote_arg(a);
    }
    return out;
}

// ---- starting -----------------------------------------------------------------------------------------

struct Process::Impl {
    Handle process;
    Handle job;
    Handle in_w, out_r, err_r;        // our ends (each may be unset)
    std::string tool;
    Clock::time_point started;
};

namespace {

// Start the child. `stdin_pipe`: give it a pipe on stdin (streaming) rather than NUL.
std::unique_ptr<Process::Impl> launch(const ProcessSpec& spec, bool stdin_pipe) {
    if (spec.argv.empty()) throw UsageError("process: empty argv");
    auto impl = std::make_unique<Process::Impl>();
    impl->tool = spec.argv[0];

    const Env env = effective_env(spec);
    const auto program = resolve_program(spec.argv[0], env, spec.cwd);
    if (!program) throw ToolError(spec.argv[0], 0, "", "cannot start: program not found");

    // Standard handles for the child.
    Handle child_in, child_out, child_err;
    if (stdin_pipe) {
        Pipe p = make_pipe(true);
        child_in = std::move(p.read);
        impl->in_w = std::move(p.write);
    } else {
        child_in = open_nul(true);
    }
    auto setup_out = [&](const Capture& cap, Handle& child_end, Handle& our_end) {
        switch (cap.mode) {
            case Capture::Collect: {
                Pipe p = make_pipe(false);
                child_end = std::move(p.write);
                our_end = std::move(p.read);
                break;
            }
            case Capture::Inherit: child_end = inherit_std(&child_end == &child_out ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE); break;
            case Capture::Discard: child_end = open_nul(false); break;
            case Capture::File: child_end = open_output_file(cap.file); break;
            case Capture::Merge: break;   // handled by the caller
        }
    };
    if (spec.out.mode == Capture::Merge) throw UsageError("process: Capture::Merge is only for err");
    setup_out(spec.out, child_out, impl->out_r);
    bool merged = false;
    if (spec.err.mode == Capture::Merge) {
        merged = true;   // the child's stderr is its stdout handle
    } else {
        setup_out(spec.err, child_err, impl->err_r);
    }
    HANDLE err_handle = merged ? child_out.get() : child_err.get();

    // Only these handles may be inherited by the child (so concurrent launches never leak pipe ends into
    // each other's children, which would keep readers waiting forever).
    std::vector<HANDLE> inherit = {child_in.get(), child_out.get()};
    if (err_handle != child_out.get()) inherit.push_back(err_handle);

    SIZE_T attr_size = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attr_size);
    std::vector<char> attr_storage(attr_size);
    auto* attrs = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attr_storage.data());
    if (!InitializeProcThreadAttributeList(attrs, 1, 0, &attr_size)) cannot_start(impl->tool, GetLastError());
    struct AttrGuard {
        LPPROC_THREAD_ATTRIBUTE_LIST a;
        ~AttrGuard() { DeleteProcThreadAttributeList(a); }
    } attr_guard{attrs};
    if (!UpdateProcThreadAttribute(attrs, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inherit.data(),
                                   inherit.size() * sizeof(HANDLE), nullptr, nullptr))
        cannot_start(impl->tool, GetLastError());

    STARTUPINFOEXW si{};
    si.StartupInfo.cb = sizeof(si);
    si.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    si.StartupInfo.hStdInput = child_in.get();
    si.StartupInfo.hStdOutput = child_out.get();
    si.StartupInfo.hStdError = err_handle;
    si.lpAttributeList = attrs;

    // The program path goes in as the command line's first token so that batch files and the CRT's argv[0] work.
    std::vector<std::string> argv = spec.argv;
    argv[0] = win::from_wide(program->native());   // (backslashes: cmd.exe reads a "/" in its own name as a switch)
    std::wstring cmdline = win::to_wide(command_line(argv));
    cmdline.push_back(L'\0');
    const std::wstring block = env_block(env);

    DWORD flags = CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT | EXTENDED_STARTUPINFO_PRESENT;
    if (spec.priority == Priority::BelowNormal) flags |= BELOW_NORMAL_PRIORITY_CLASS;
    if (spec.out.mode != Capture::Inherit) flags |= CREATE_NO_WINDOW;

    impl->job = make_job(spec);
    PROCESS_INFORMATION pi{};
    const std::wstring cwd = spec.cwd ? spec.cwd->native() : std::wstring();
    const bool is_batch = text::iends_with(argv[0], ".bat") || text::iends_with(argv[0], ".cmd");
    const BOOL created = CreateProcessW(is_batch ? nullptr : program->c_str(), cmdline.data(), nullptr, nullptr, TRUE,
                                        flags, const_cast<wchar_t*>(block.c_str()), spec.cwd ? cwd.c_str() : nullptr,
                                        &si.StartupInfo, &pi);
    if (!created) cannot_start(impl->tool, GetLastError());
    impl->process = Handle(pi.hProcess);
    Handle thread(pi.hThread);

    // Into the job before it runs a single instruction, so that nothing it starts can escape.
    if (!AssignProcessToJobObject(impl->job.get(), impl->process.get())) {
        const DWORD code = GetLastError();
        TerminateProcess(impl->process.get(), 1);
        cannot_start(impl->tool, code);
    }
    ResumeThread(thread.get());
    impl->started = Clock::now();
    return impl;
}

// Read a pipe to its end.
Bytes drain(HANDLE h) {
    Bytes out;
    std::uint8_t buf[1 << 14];
    while (true) {
        DWORD got = 0;
        if (!ReadFile(h, buf, sizeof(buf), &got, nullptr) || got == 0) break;
        out.insert(out.end(), buf, buf + got);
    }
    return out;
}

void write_all(HANDLE h, ByteView data) {
    std::size_t done = 0;
    while (done < data.size()) {
        DWORD wrote = 0;
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(data.size() - done, 1u << 20));
        if (!WriteFile(h, data.data() + done, chunk, &wrote, nullptr)) {
            const DWORD code = GetLastError();
            if (code == ERROR_BROKEN_PIPE || code == ERROR_NO_DATA) return;   // the child stopped reading: not our error
            throw IoError("<stdin>", static_cast<int>(code), "write to child failed");
        }
        done += wrote;
    }
}

}  // namespace

ProcessResult run(const ProcessSpec& spec) {
    auto impl = launch(spec, !spec.stdin_data.empty());
    ProcessResult result;

    // Readers and the stdin feeder run on threads: a child that fills a pipe while we wait must not deadlock.
    std::vector<std::jthread> helpers;
    Bytes out_bytes, err_bytes;
    if (impl->out_r.valid()) helpers.emplace_back([&] { out_bytes = drain(impl->out_r.get()); });
    if (impl->err_r.valid()) helpers.emplace_back([&] { err_bytes = drain(impl->err_r.get()); });
    if (impl->in_w.valid()) {
        helpers.emplace_back([&] {
            try {
                write_all(impl->in_w.get(), spec.stdin_data);
            } catch (const Error&) {
                // A failed write shows up as the child's own failure; there is nobody to throw to on this thread.
            }
            impl->in_w.reset();
        });
    }

    const DWORD wait_ms = spec.timeout.count() > 0 ? static_cast<DWORD>(spec.timeout.count()) : INFINITE;
    if (WaitForSingleObject(impl->process.get(), wait_ms) == WAIT_TIMEOUT) {
        result.timed_out = true;
        TerminateJobObject(impl->job.get(), 1);
        WaitForSingleObject(impl->process.get(), INFINITE);
    }
    // The program is done; whatever it left running goes with the job so the pipes can reach their end.
    TerminateJobObject(impl->job.get(), result.timed_out ? 1 : 0);
    helpers.clear();   // joins

    DWORD code = 0;
    GetExitCodeProcess(impl->process.get(), &code);
    result.exit_code = code;
    result.out = std::move(out_bytes);
    result.err = std::move(err_bytes);
    result.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - impl->started);
    return result;
}

ProcessResult run_checked(const ProcessSpec& spec) {
    ProcessResult r = run(spec);
    std::string tool = spec.argv.empty() ? std::string() : spec.argv[0];
    std::string output(as_text(r.out));
    output += as_text(r.err);
    if (r.timed_out) throw TimeoutError(tool, output);
    if (r.exit_code != 0) throw ToolError(tool, r.exit_code, output);
    return r;
}

// ---- streaming ----------------------------------------------------------------------------------------

Process start(const ProcessSpec& spec) { return Process(launch(spec, true)); }

Process::Process(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Process::Process(Process&& other) noexcept = default;
Process& Process::operator=(Process&& other) noexcept {
    if (this != &other) {
        kill();
        impl_ = std::move(other.impl_);
    }
    return *this;
}

Process::~Process() { kill(); }

void Process::kill() {
    if (impl_ && impl_->job.valid()) TerminateJobObject(impl_->job.get(), 1);
}

std::size_t Process::read(Stream which, ByteSpan buf) {
    Handle& h = which == Stream::Out ? impl_->out_r : impl_->err_r;
    if (!h.valid()) return 0;
    DWORD got = 0;
    if (!ReadFile(h.get(), buf.data(), static_cast<DWORD>(std::min<std::size_t>(buf.size(), 1u << 20)), &got, nullptr))
        return 0;   // broken pipe: the child closed its end
    return got;
}

Bytes Process::read_all(Stream which) {
    Handle& h = which == Stream::Out ? impl_->out_r : impl_->err_r;
    return h.valid() ? drain(h.get()) : Bytes{};
}

void Process::write(ByteView data) {
    if (!impl_->in_w.valid()) throw UsageError("process: stdin is closed");
    write_all(impl_->in_w.get(), data);
}

void Process::close_stdin() { impl_->in_w.reset(); }

std::optional<std::uint32_t> Process::wait(std::chrono::milliseconds timeout) {
    DWORD ms = INFINITE;
    if (timeout != std::chrono::milliseconds::max()) ms = static_cast<DWORD>(std::max<std::int64_t>(0, timeout.count()));
    if (WaitForSingleObject(impl_->process.get(), ms) == WAIT_TIMEOUT) return std::nullopt;
    DWORD code = 0;
    GetExitCodeProcess(impl_->process.get(), &code);
    return code;
}

bool Process::running() const { return WaitForSingleObject(impl_->process.get(), 0) == WAIT_TIMEOUT; }

}  // namespace cb::process
