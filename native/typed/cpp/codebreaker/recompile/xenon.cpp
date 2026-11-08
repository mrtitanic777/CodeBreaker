// codebreaker/recompile/xenon.cpp -- typed C++ from codebreaker.recompile.xenon.

#include "codebreaker/recompile/xenon.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "py_os.hpp"
#include "py_settings.hpp"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace codebreaker::recompile::xenon {

namespace fs = std::filesystem;

// -- the module's locations --

namespace {

// ROOT as the import derives it: three folders above this module (codebreaker/recompile/xenon.py). The C++ source sits
// six folders below the checkout (native/typed/cpp/codebreaker/recompile/xenon.cpp).
py::str default_root_() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}

}  // namespace

py::str ROOT;
py::str SOURCE;
py::str BUILD;
py::str RECOMP;
py::str ANALYSE;
py::str TESTS;

void set_root(const py::str& root) {
    ROOT = root;
    SOURCE = py::os::path::join(ROOT, py::str("native"), py::str("xenonrecomp"));
    BUILD = py::os::path::join(SOURCE, py::str("build"));
    RECOMP = py::os::path::join(BUILD, py::str("XenonRecomp"), py::str("XenonRecomp.exe"));
    ANALYSE = py::os::path::join(BUILD, py::str("XenonAnalyse"), py::str("XenonAnalyse.exe"));
    TESTS = py::os::path::join(SOURCE, py::str("ppc_tests"));
}

namespace {
const bool root_set_ = (set_root(default_root_()), true);
}

const std::tuple<py::str, py::str> SKIP_TESTS{py::str("ppc_testing_native_thunks"), py::str("seq_jumptable_constants")};

// -- running a program --

namespace {

#ifdef _WIN32

// subprocess.list2cmdline (including its habit of doubling a quoted argument's trailing backslashes)
std::u32string list2cmdline_(const py::list<py::str>& argv) {
    std::u32string result;
    for (const py::str& arg : argv) {
        std::u32string bs_buf;
        if (!result.empty()) result.push_back(U' ');
        bool needquote = arg.contains(py::str(" ")) || arg.contains(py::str("\t")) || arg.empty();
        if (needquote) result.push_back(U'"');
        for (char32_t c : arg.u32()) {
            if (c == U'\\') {
                bs_buf.push_back(c);
            } else if (c == U'"') {
                result.append(bs_buf.size() * 2, U'\\');
                bs_buf.clear();
                result += U"\\\"";
            } else {
                result += bs_buf;
                bs_buf.clear();
                result.push_back(c);
            }
        }
        if (!bs_buf.empty()) result += bs_buf;
        if (needquote) {
            result += bs_buf;
            result.push_back(U'"');
        }
    }
    return result;
}

std::wstring wide_(const std::u32string& s) {
    std::wstring w;
    for (char32_t c : s) {
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

// An OSError the way Python's CreateProcess raises one: "[WinError n] <the system's text>"; the error number picks
// the class (file not found, not a directory, access denied, any other).
struct NotADirectoryError : py::OSError {
    NotADirectoryError() : py::OSError() {}
    explicit NotADirectoryError(const std::string& m) : py::OSError(m) {}
    const char* type() const override { return "NotADirectoryError"; }
};

[[noreturn]] void raise_winerror_(DWORD err) {
    wchar_t buf[512];
    DWORD n = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 512, nullptr);
    while (n > 0 && (buf[n - 1] <= L' ' || buf[n - 1] == L'.')) --n;   // (cr/lf and dots at the end go)
    std::string text;
    if (n > 0) {
        int len = WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), nullptr, 0, nullptr, nullptr);
        text.resize(static_cast<size_t>(len));
        WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), &text[0], len, nullptr, nullptr);
    }
    std::string msg = "[WinError " + std::to_string(err) + "] " + text;
    if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) throw py::FileNotFoundError(msg);
    if (err == ERROR_ACCESS_DENIED) throw py::PermissionError(msg);
    if (err == ERROR_DIRECTORY) throw NotADirectoryError(msg);
    throw py::OSError(msg);
}

// stdout or stderr read to its end, as text: cp1252, universal newlines
py::str read_pipe_text_(const std::string& raw) {
    py::str text = py::bytes(raw).decode("cp1252");
    std::u32string out;
    const std::u32string& u = text.u32();
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') ++i;
        } else {
            out.push_back(u[i]);
        }
    }
    return py::str(out);
}

void drain_(HANDLE h, std::string* into) {
    char buf[4096];
    DWORD got = 0;
    while (ReadFile(h, buf, sizeof buf, &got, nullptr) && got > 0) into->append(buf, got);
}

CompletedProcess run_command_line_(const std::u32string& command_line, const std::optional<py::str>& cwd,
                                   const std::optional<py::dict<py::str, py::str>>& env) {
    std::wstring cmd = wide_(command_line);
    cmd.push_back(L'\0');   // (CreateProcessW may write into the buffer)
    cmd.pop_back();

    // the environment block: "key=value" for each entry, a NUL after each, and one more at the end
    std::wstring block;
    if (env) {
        for (const auto& kv : env->items()) {
            block += wide_(std::get<0>(kv).u32());
            block.push_back(L'=');
            block += wide_(std::get<1>(kv).u32());
            block.push_back(L'\0');
        }
        if (block.empty()) block.push_back(L'\0');
        block.push_back(L'\0');
    }
    std::wstring wcwd = cwd ? wide_(cwd->u32()) : std::wstring();

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof sa;
    sa.bInheritHandle = TRUE;
    HANDLE out_rd = nullptr, out_wr = nullptr, err_rd = nullptr, err_wr = nullptr;
    if (!CreatePipe(&out_rd, &out_wr, &sa, 0) || !CreatePipe(&err_rd, &err_wr, &sa, 0)) raise_winerror_(GetLastError());
    SetHandleInformation(out_rd, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(err_rd, HANDLE_FLAG_INHERIT, 0);
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in && in != INVALID_HANDLE_VALUE) SetHandleInformation(in, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in;
    si.hStdOutput = out_wr;
    si.hStdError = err_wr;
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, &cmd[0], nullptr, nullptr, TRUE, env ? CREATE_UNICODE_ENVIRONMENT : 0,
                             env ? static_cast<void*>(&block[0]) : nullptr, cwd ? wcwd.c_str() : nullptr, &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(out_wr);
    CloseHandle(err_wr);
    if (!ok) {
        CloseHandle(out_rd);
        CloseHandle(err_rd);
        raise_winerror_(err);
    }
    std::string raw_out, raw_err;
    std::thread err_reader(drain_, err_rd, &raw_err);   // (both pipes are read at once: neither can fill and stall the program)
    drain_(out_rd, &raw_out);
    err_reader.join();
    CloseHandle(out_rd);
    CloseHandle(err_rd);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    CompletedProcess r;
    r.returncode = static_cast<int64_t>(code);   // (Python's is unsigned too)
    r.stdout_text = read_pipe_text_(raw_out);
    r.stderr_text = read_pipe_text_(raw_err);
    return r;
}

#endif  // _WIN32

}  // namespace

CompletedProcess _run(const py::list<py::str>& cmd, const std::optional<py::str>& cwd,
                      const std::optional<py::dict<py::str, py::str>>& env) {
#ifdef _WIN32
    return run_command_line_(list2cmdline_(cmd), cwd, env);
#else
    (void)cmd;
    (void)cwd;
    (void)env;
    throw py::NotImplementedError("xenon._run: running a program is implemented for Windows only");
#endif
}

CompletedProcess _run_shell(const py::str& command) {
#ifdef _WIN32
    // Popen(shell=True): COMSPEC /c "<command>"
    py::str comspec = py::os::environ_::get(py::str("COMSPEC"), py::str("cmd.exe"));
    return run_command_line_((comspec + py::str(" /c \"") + command + py::str("\"")).u32(), std::nullopt, std::nullopt);
#else
    (void)command;
    throw py::NotImplementedError("xenon._run: running a program is implemented for Windows only");
#endif
}

// -- finding Visual Studio --

py::str visual_studio() {
    py::str vs = py::settings::get_str(py::str("visual_studio"));
    if (py::truthy(vs)) return vs;
    py::str where = py::os::path::join(
        py::os::environ_::get(py::str("ProgramFiles(x86)"), py::str("C:\\Program Files (x86)")),
        py::str("Microsoft Visual Studio"), py::str("Installer"), py::str("vswhere.exe"));
    if (py::os::path::isfile(where)) {
        py::str out = _run({where, py::str("-latest"), py::str("-prerelease"), py::str("-products"), py::str("*"),
                            py::str("-requires"), py::str("Microsoft.VisualStudio.Component.VC.Tools.x86.x64"),
                            py::str("-property"), py::str("installationPath")})
                          .stdout_text.strip();
        if (py::truthy(out)) return py::splitlines(out)[0];
    }
    throw py::RuntimeError("no Visual Studio with the C++ x64 tools found (setting visual_studio)");
}

py::str clang_cl() {
    py::str path = py::os::path::join(visual_studio(), py::str("VC"), py::str("Tools"), py::str("Llvm"), py::str("x64"),
                                      py::str("bin"), py::str("clang-cl.exe"));
    if (!py::os::path::isfile(path)) {
        throw py::RuntimeError(
            "clang-cl is not installed: add \"C++ Clang Compiler for Windows\" in the "
            "Visual Studio Installer");
    }
    return path;
}

std::optional<py::dict<py::str, py::str>> _env;

py::dict<py::str, py::str> msvc_env() {
    if (!_env) {
        py::str bat = py::os::path::join(visual_studio(), py::str("VC"), py::str("Auxiliary"), py::str("Build"),
                                         py::str("vcvars64.bat"));
        py::str out = _run_shell(py::str("cmd /c \"\"") + bat + py::str("\" >nul && set\"")).stdout_text;
        py::dict<py::str, py::str> env;
        for (const py::str& line : py::splitlines(out)) {
            if (line.contains(py::str("="))) {
                auto [key, eq, value] = line.partition(py::str("="));
                env.set(key, value);
            }
        }
        _env = env;
    }
    return *_env;
}

// -- building XenonRecomp --

namespace {

// glob's idea of a hidden name: one that starts with a dot (a pattern of "*" does not match it)
bool hidden_(const py::str& name) { return !name.empty() && name.u32()[0] == U'.'; }

// glob.glob(os.path.join(path, "**", "*"), recursive=True), the files among them: every file below `path`,
// not through a hidden folder
void collect_files_(const py::str& path, std::vector<py::str>& out) {
    if (!py::os::path::isdir(path)) return;
    for (const py::str& name : py::os::listdir(path)) {
        if (hidden_(name)) continue;
        py::str full = py::os::path::join(path, name);
        if (py::os::path::isdir(full)) collect_files_(full, out);
        else if (py::os::path::isfile(full)) out.push_back(full);
    }
}

// glob.glob(os.path.join(folder, "*", "*" + ext)): the entries of that name in the folders of `folder`
// (the match is not case-sensitive on Windows), in no order
std::vector<py::str> glob_in_subfolders_(const py::str& folder, const py::str& ext) {
    std::vector<py::str> out;
    if (!py::os::path::isdir(folder)) return out;
    for (const py::str& d : py::os::listdir(folder)) {
        if (hidden_(d)) continue;
        py::str dir = py::os::path::join(folder, d);
        if (!py::os::path::isdir(dir)) continue;
        for (const py::str& name : py::os::listdir(dir)) {
            if (!hidden_(name) && name.lower().endswith(ext)) out.push_back(py::os::path::join(dir, name));
        }
    }
    return out;
}

// glob.glob(os.path.join(folder, "*" + ext))
std::vector<py::str> glob_in_folder_(const py::str& folder, const py::str& ext) {
    std::vector<py::str> out;
    if (!py::os::path::isdir(folder)) return out;
    for (const py::str& name : py::os::listdir(folder)) {
        if (!hidden_(name) && name.lower().endswith(ext)) out.push_back(py::os::path::join(folder, name));
    }
    return out;
}

py::list<py::str> sorted_(const std::vector<py::str>& v) {
    py::list<py::str> out(v);
    out.sort();
    return out;
}

// os.cpu_count()
int64_t cpu_count_() {
#ifdef _WIN32
    return static_cast<int64_t>(GetMaximumProcessorCount(ALL_PROCESSOR_GROUPS));
#else
    return static_cast<int64_t>(std::thread::hardware_concurrency());
#endif
}

}  // namespace

double _newest_source() {
    double newest = 0.0;
    for (const char* sub : {"XenonRecomp", "XenonUtils", "XenonAnalyse", "CMakeLists.txt"}) {
        py::str path = py::os::path::join(SOURCE, py::str(sub));
        std::vector<py::str> files;
        if (py::os::path::isfile(path)) files.push_back(path);
        else collect_files_(path, files);
        for (const py::str& f : files) {
            if (py::os::path::isfile(f)) newest = std::max(newest, py::os::path::getmtime(f));
        }
    }
    return newest;
}

py::str build(bool force) {
    if (!force && py::os::path::isfile(RECOMP) && py::os::path::isfile(ANALYSE) &&
        std::min(py::os::path::getmtime(RECOMP), py::os::path::getmtime(ANALYSE)) >= _newest_source()) {
        return RECOMP;
    }
    py::dict<py::str, py::str> env = msvc_env();
    py::str cc = clang_cl();
    if (force || !py::os::path::isfile(py::os::path::join(BUILD, py::str("CMakeCache.txt")))) {
        CompletedProcess r = _run({py::str("cmake"), py::str("-S"), SOURCE, py::str("-B"), BUILD, py::str("-G"),
                                   py::str("Ninja"), py::str("-DCMAKE_BUILD_TYPE=Release"),
                                   py::str("-DCMAKE_C_COMPILER=") + cc, py::str("-DCMAKE_CXX_COMPILER=") + cc},
                                  std::nullopt, env);
        if (r.returncode) {
            throw py::RuntimeError(py::str("configuring XenonRecomp failed:\n") +
                                   (r.stdout_text + r.stderr_text).slice_from(-3000));
        }
    }
    CompletedProcess r = _run({py::str("cmake"), py::str("--build"), BUILD, py::str("-j"),
                               py::to_str(std::max<int64_t>(1, (cpu_count_() ? cpu_count_() : 2) / 2))},
                              std::nullopt, env);
    if (r.returncode) {
        throw py::RuntimeError(py::str("building XenonRecomp failed:\n") + (r.stdout_text + r.stderr_text).slice_from(-3000));
    }
    return RECOMP;
}

py::str recompile(const py::str& config0) {
    build();
    // it runs in the configuration's folder: a relative path would not reach it
    py::str config = py::os::path::abspath(config0);
    CompletedProcess r = _run({RECOMP, config, py::os::path::join(SOURCE, py::str("XenonUtils"), py::str("ppc_context.h"))},
                              py::os::path::dirname(config));
    if (r.returncode) {
        throw py::RuntimeError(py::str("XenonRecomp failed:\n") + (r.stdout_text + r.stderr_text).slice_from(-3000));
    }
    return r.stdout_text;
}

namespace {

// tempfile.TemporaryDirectory(dir=parent): a folder of a random name in `parent`, removed (with what is in it) at
// the end of the scope, however it ends
class TemporaryDirectory {
  public:
    explicit TemporaryDirectory(const py::str& parent) {
        static const char32_t letters[] = U"abcdefghijklmnopqrstuvwxyz0123456789_";
        std::srand(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
        for (int attempt = 0; attempt < 100; ++attempt) {
            std::u32string name = U"tmp";
            for (int i = 0; i < 8; ++i) name.push_back(letters[std::rand() % 37]);
            py::str path = py::os::path::join(parent, py::str(name));
            std::error_code ec;
            if (fs::create_directory(fs::u8path(path.utf8()), ec)) {
                path_ = path;
                return;
            }
        }
        throw py::FileExistsError("No usable temporary directory name found");
    }
    ~TemporaryDirectory() {
        try {
            py::shutil::rmtree(path_, true);
        } catch (...) {
        }
    }
    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
    const py::str& name() const { return path_; }

  private:
    py::str path_;
};

// shutil.copy2: the file copied with its modification time
void copy2_(const py::str& src, const py::str& dst) {
    py::shutil::copyfile(src, dst);
    std::error_code ec;
    fs::last_write_time(fs::u8path(dst.utf8()), fs::last_write_time(fs::u8path(src.utf8()), ec), ec);
}

// filecmp.cmp(a, b, shallow=False): the same size and the same bytes
bool same_file_(const py::str& a, const py::str& b) {
    if (py::os::path::getsize(a) != py::os::path::getsize(b)) return false;
    return py::open_rb(a).read() == py::open_rb(b).read();
}

}  // namespace

SyncResult recompile_synced(const py::str& config0, const std::function<void(const py::str&)>& report0) {
    std::function<void(const py::str&)> report = report0 ? report0 : [](const py::str& line) { py::print(line); };
    build();
    py::str config = py::os::path::abspath(config0);
    py::str folder = py::os::path::dirname(config);
    py::str text = py::open(config, py::str("r"), py::str("utf-8")).read_text();
    std::optional<py::re::Match> m =
        py::re::search(py::str("^out_directory_path\\s*=\\s*\"([^\"]*)\""), text, py::re::M);
    if (!m) throw py::ValueError(py::percent(py::str("%s: no out_directory_path"), config));
    py::str out_dir = py::os::path::normpath(py::os::path::join(folder, m->group(1)));
    py::os::makedirs(py::os::path::dirname(out_dir), 0777, true);

    py::list<py::str> log;
    py::set<py::str> new_;
    int64_t changed = 0;
    py::list<py::str> stale;
    {
        TemporaryDirectory tmp(py::os::path::dirname(out_dir));
        py::str fresh = py::os::path::join(tmp.name(), py::str("ppc"));
        py::os::makedirs(fresh);
        // the same configuration beside the original (its relative paths still reach), with the fresh folder; passed
        // by its name, as XenonRecomp joins the paths inside to the configuration's own
        py::str work = "_recompile_tmp.toml";
        {
            py::file f = py::open(py::os::path::join(folder, work), py::str("w"), py::str("utf-8"), std::nullopt,
                                  py::str("\n"));
            f.write(py::re::sub(py::str("^out_directory_path\\s*=.*$"),
                                py::percent(py::str("out_directory_path = \"%s\""),
                                            py::os::path::relpath(fresh, folder).replace(py::str("\\"), py::str("/"))),
                                text, 0, py::re::M));
            f.close();
        }
        CompletedProcess r;
        try {
            r = _run({RECOMP, work, py::os::path::join(SOURCE, py::str("XenonUtils"), py::str("ppc_context.h"))}, folder);
        } catch (...) {
            py::os::remove(py::os::path::join(folder, work));
            throw;
        }
        py::os::remove(py::os::path::join(folder, work));
        for (const py::str& l : py::splitlines(r.stdout_text)) {
            if (!l.startswith(py::str("Recompiling functions"))) log.append(l);
        }
        if (r.returncode) {
            throw py::RuntimeError(py::percent(py::str("XenonRecomp failed (%d):\n%s"), r.returncode,
                                               py::join(py::str("\n"), log.slice_from(-40)) + r.stderr_text));
        }
        py::os::makedirs(out_dir, 0777, true);
        new_ = py::set<py::str>::from(py::os::listdir(fresh));
        for (const py::str& name : py::sorted(new_)) {
            py::str dst = py::os::path::join(out_dir, name);
            if (!py::os::path::exists(dst) || !same_file_(py::os::path::join(fresh, name), dst)) {
                copy2_(py::os::path::join(fresh, name), dst);
                changed += 1;
            }
        }
        py::list<py::str> not_new;
        for (const py::str& n : py::os::listdir(out_dir)) {
            if (!new_.contains(n)) not_new.append(n);
        }
        stale = py::sorted(not_new);
        for (const py::str& name : stale) py::os::remove(py::os::path::join(out_dir, name));
    }
    for (const py::str& line : log) report(line);
    report(py::percent(py::str("%d files generated, %d changed, %d stale removed%s; %d warning lines"), new_.size(), changed,
                       stale.size(), !stale.empty() ? py::str(": ") + py::join(py::str(" "), stale) : py::str(""), log.size()));
    SyncResult result;
    result.generated = new_.size();
    result.changed = changed;
    result.stale = stale;
    result.log = log;
    return result;
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.size() != 1) {
        py::print(py::str("usage: python -m codebreaker xenon-recompile <config.toml>"));
        return 2;
    }
    try {
        recompile_synced(argv[0]);
    } catch (const py::RuntimeError& e) {
        py::print(py::str(e.what()));
        return 1;
    } catch (const py::ValueError& e) {
        py::print(py::str(e.what()));
        return 1;
    }
    return 0;
}

py::str _unix(const py::str& p) { return p.replace(py::str("\\"), py::str("/")); }

namespace {

// text.splitlines(True): the lines with their line ends kept
py::list<py::str> splitlines_keepends_(const py::str& s) {
    py::list<py::str> out;
    const std::u32string& u = s.u32();
    size_t i = 0, n = u.size();
    while (i < n) {
        size_t j = i;
        while (j < n && u[j] != '\n' && u[j] != '\r' && u[j] != 0x0B && u[j] != 0x0C && u[j] != 0x1C && u[j] != 0x1D &&
               u[j] != 0x1E && u[j] != 0x85 && u[j] != 0x2028 && u[j] != 0x2029)
            ++j;
        if (j < n) {
            if (u[j] == '\r' && j + 1 < n && u[j + 1] == '\n') ++j;
            ++j;
        }
        out.append(py::str(u.substr(i, j - i)));
        i = j;
    }
    return out;
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state_(const py::str& a, const py::str& b) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

}  // namespace

InstructionTestsResult instruction_tests(const std::optional<py::list<py::str>>& names, const std::optional<py::str>& work0,
                                          const py::str& march) {
    py::str binutils = py::settings::get_str(py::str("ppc_binutils"));
    py::str asm_ = py::os::path::join(binutils, py::str("powerpc-none-elf-as.exe"));
    py::str objdump = py::os::path::join(binutils, py::str("powerpc-none-elf-objdump.exe"));
    if (!py::os::path::isfile(asm_)) {
        throw py::RuntimeError(py::percent(py::str("no PowerPC binutils at %s (setting ppc_binutils)"), binutils));
    }
    build();
    py::str work = work0 && py::truthy(*work0) ? *work0 : state_(py::str("xenonrecomp"), py::str("ppc_tests"));
    py::str src = py::os::path::join(work, py::str("src"));
    py::str objs = py::os::path::join(work, py::str("src"), py::str("bin"));
    py::str out = py::os::path::join(work, py::str("out"));
    for (const py::str& d : {objs, out}) {
        py::shutil::rmtree(d, true);
        py::os::makedirs(d);
    }
    py::list<py::str> tested;
    for (const py::str& s : sorted_(glob_in_subfolders_(TESTS, py::str(".s")))) {
        py::str name = std::get<0>(py::os::path::splitext(py::os::path::basename(s)));
        if ((names && !names->empty() && !names->contains(name)) || py::in(name, SKIP_TESTS)) continue;
        py::shutil::copy(s, src);
        py::str o = py::os::path::join(objs, name + py::str(".o"));
        CompletedProcess r = _run({asm_, py::str("-a32"), py::str("-be"), py::str("-mregnames"), py::str("-mpower7"),
                                   py::str("-maltivec"), py::str("-mvsx"), py::str("-mvmx128"), py::str("-R"),
                                   py::str("-o") + _unix(o), _unix(py::os::path::join(src, name + py::str(".s")))});
        if (r.returncode) {
            throw py::RuntimeError(py::percent(py::str("assembling %s failed: %s"), name, r.stderr_text.strip().slice(0, 300)));
        }
        r = _run({objdump, py::str("--adjust-vma=0x100000"), py::str("-Mpower7"), py::str("-Mvmx128"), py::str("-D"),
                  py::str("-EB"), _unix(o)});
        {
            py::file f = py::open(py::os::path::join(objs, name + py::str(".dis")), py::str("w"));
            f.writelines(splitlines_keepends_(r.stdout_text).slice_from(4));
        }
        tested.append(name);
    }
    // a test of an instruction XenonRecomp does not implement is left out (and listed): a title's
    // own recompile reports any instruction it uses
    CompletedProcess r = _run({RECOMP, objs, out});
    py::set<py::str> unimplemented;
    for (const py::str& l : py::splitlines(r.stdout_text)) {
        if (l.contains(py::str("has unimplemented"))) unimplemented.add(py::split(py::split(l, py::str(" in "))[1])[0]);
    }
    py::list<py::str> skipped = py::sorted(unimplemented);
    if (!skipped.empty()) {
        for (const py::str& name : skipped) {
            for (const char* ext : {".o", ".dis"}) py::os::remove(py::os::path::join(objs, name + py::str(ext)));
        }
        py::shutil::rmtree(out);
        py::os::makedirs(out);
        _run({RECOMP, objs, out});
    }
    py::str exe = py::os::path::join(work, py::str("ppc_tests.exe"));
    py::list<py::str> cmd = {clang_cl(), py::str("/nologo"), py::str("/std:c++20"), py::str("/O1"), py::str("/fp:strict"),
                             py::str("-fno-strict-aliasing"), py::str("-march=") + march, py::str("-Wno-everything"),
                             py::str("/DFMT_HEADER_ONLY"),
                             py::str("/I") + py::os::path::join(SOURCE, py::str("XenonUtils")),
                             py::str("/I") + py::os::path::join(SOURCE, py::str("thirdparty"), py::str("simde")),
                             py::str("/I") + py::os::path::join(SOURCE, py::str("thirdparty"), py::str("fmt"), py::str("include")),
                             py::str("/Fe") + exe, py::str("/Fo") + work + py::str("\\")};
    cmd.extend(sorted_(glob_in_folder_(out, py::str(".cpp"))));
    r = _run(cmd, work, msvc_env());
    if (r.returncode) {
        throw py::RuntimeError(py::str("building the instruction tests failed:\n") +
                               (r.stdout_text + r.stderr_text).slice_from(-4000));
    }
    r = _run({exe}, work);
    InstructionTestsResult result;
    py::set<py::str> failed;
    for (const py::str& l : py::splitlines(r.stdout_text)) {
        if (py::truthy(l.strip())) {
            result.mismatches.append(l);
            failed.add(py::split(l)[0]);
        }
    }
    result.tested = tested.size() - skipped.size();
    result.skipped = skipped;
    result.failed = py::sorted(failed);
    return result;
}

}  // namespace codebreaker::recompile::xenon
