// codebreaker/port/verify.cpp -- typed C++ port of codebreaker.port.verify (see verify.hpp).

#include "codebreaker/port/verify.hpp"

#include <algorithm>
#include <map>
#include <mutex>
#include <ostream>
#include <set>
#include <thread>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/emu/native_dosexec.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "json.hpp"
#include "py_os.hpp"
#include "py_difflib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::port::verify {

namespace cache = codebreaker::base::cache;
namespace dos = codebreaker::exact::emu::dos;
namespace V = codebreaker::verify::verdicts;
using py::str;

// =====================================================================================================================
// difflib: the runtime's py::difflib (SequenceMatcher(None, a, b) with autojunk on, unified_diff)
namespace difflib {

py::list<Opcode> get_opcodes(const py::list<str>& a, const py::list<str>& b) {
    py::list<Opcode> out;
    for (const Opcode& op : py::difflib::SequenceMatcher<str>(a, b).get_opcodes()) out.append(op);
    return out;
}

py::list<py::list<Opcode>> get_grouped_opcodes(const py::list<str>& a, const py::list<str>& b, int64_t n) {
    py::list<py::list<Opcode>> groups;
    for (const auto& g : py::difflib::SequenceMatcher<str>(a, b).get_grouped_opcodes(n)) {
        py::list<Opcode> group;
        for (const Opcode& op : g) group.append(op);
        groups.append(group);
    }
    return groups;
}

py::list<str> unified_diff(const py::list<str>& a, const py::list<str>& b, const str& fromfile, const str& tofile, int64_t n, const str& lineterm) {
    return py::difflib::unified_diff(a, b, fromfile, tofile, str(""), str(""), n, lineterm);
}

}  // namespace difflib

// =====================================================================================================================
namespace {

std::string read_all(const str& path) {
    py::file f = py::open_rb(path);
    const py::bytes b = f.read();
    f.close();
    return b.raw();
}

void write_bytes(const str& path, const py::bytes& data) {
    py::file f = py::file::open(path, str("wb"));
    f.write(data);
    f.close();
}

std::vector<str> sorted_names(const str& folder) {
    std::vector<str> names;
    for (const str& n : py::os::listdir(folder)) names.push_back(n);
    std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
    return names;
}

str sha256_hex(const std::string& data) { return py::hashlib::sha256(py::bytes(data)).hexdigest(); }

str exit_text(const Exit& x) {
    if (std::holds_alternative<int64_t>(x)) return str(std::to_string(std::get<int64_t>(x)));
    if (std::holds_alternative<str>(x)) return std::get<str>(x);
    return str("None");
}

// (xa or 0): None and 0 are 0, the text stays
Exit truth_or_zero(const Exit& x) {
    if (std::holds_alternative<std::monostate>(x)) return Exit(int64_t(0));
    if (std::holds_alternative<str>(x) && std::get<str>(x).empty()) return Exit(int64_t(0));
    return x;
}

std::optional<str> g_emulator_key;

}  // namespace

// ---- cases.json
namespace {

Case case_of(const py::V& v) {
    Case c;
    c.name = str(v.at("name").str());
    if (v.get("cmd").is_str()) c.cmd = str(v.get("cmd").str());
    if (v.get("stdin").is_str()) c.stdin_ = str(v.get("stdin").str());
    if (v.get("files").is_list())
        for (const py::V& x : v.get("files").seq()) c.files.append(str(x.str()));
    if (v.get("make").is_dict())
        for (const auto& kv : v.get("make").items()) c.make.set(str(kv.first.str()), str(kv.second.str()));
    if (v.get("then").is_list())
        for (const py::V& x : v.get("then").seq()) c.then.append(case_of(x));
    if (v.get("ignore").is_list())
        for (const py::V& x : v.get("ignore").seq()) c.ignore.append(str(x.str()));
    if (v.get("program").is_str()) c.program = str(v.get("program").str());
    return c;
}

}  // namespace

Config load_config(const str& path) {
    const py::V v = py::json_parse(read_all(path));
    Config c;
    if (v.get("original").is_str()) c.original = str(v.get("original").str());
    if (v.get("port").is_str()) c.port = str(v.get("port").str());
    if (v.get("inputs").is_dict())
        for (const auto& kv : v.get("inputs").items()) c.inputs.set(str(kv.first.str()), str(kv.second.str()));
    if (v.get("env").is_dict())
        for (const auto& kv : v.get("env").items()) c.env.set(str(kv.first.str()), str(kv.second.str()));
    if (v.get("programs").is_dict()) {
        for (const auto& kv : v.get("programs").items()) {
            Program p;
            p.original = str(kv.second.at("original").str());
            p.port = str(kv.second.at("port").str());
            c.programs.set(str(kv.first.str()), p);
        }
    }
    if (v.get("volatile").is_list())
        for (const py::V& x : v.get("volatile").seq()) c.volatile_.append(str(x.str()));
    for (const py::V& x : v.at("cases").seq()) c.cases.append(case_of(x));
    return c;
}

// ---- the emulator, the files, the keys
void set_emulator_key(const std::optional<str>& key) { g_emulator_key = key; }

str _emulator_key() {
    static std::optional<str> computed;
    if (g_emulator_key) return *g_emulator_key;
    if (!computed) {
        wchar_t buf[MAX_PATH * 4];
        const DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH * 4);
        std::string u8(static_cast<size_t>(n) * 4, '\0');
        const int m = WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), u8.data(), static_cast<int>(u8.size()), nullptr, nullptr);
        u8.resize(static_cast<size_t>(m));
        computed = sha256_hex("cbdos" + read_all(str(u8)));
    }
    return *computed;
}

str _file_hash(const str& path, double mtime, int64_t size) {
    static std::mutex lock;
    static std::map<std::tuple<std::u32string, double, int64_t>, str> kept;
    const auto key = std::make_tuple(path.u32(), mtime, size);
    {
        std::lock_guard<std::mutex> g(lock);
        auto it = kept.find(key);
        if (it != kept.end()) return it->second;
    }
    const str h = sha256_hex(read_all(path));
    std::lock_guard<std::mutex> g(lock);
    kept[key] = h;
    return h;
}

str _original_key(const str& exe, const str& folder, const str& cmd, const str& stdin_) {
    std::string all;
    const std::string size_probe = read_all(exe);                   // (os.stat(exe): its size)
    const str fh = _file_hash(py::os::path::abspath(exe), py::os::path::getmtime(exe), static_cast<int64_t>(size_probe.size()));
    all += fh.utf8();
    all += _emulator_key().utf8();
    all += (str("(") + py::repr(cmd) + str(", ") + py::repr(stdin_) + str(")")).encode("latin1", "replace").raw();
    for (const str& n : sorted_names(folder)) {
        const str f = py::os::path::join(folder, n);
        if (py::os::path::isfile(f)) {
            all += n.encode("latin1", "replace").raw();
            all.push_back('\0');
            all += read_all(f);
            all.push_back('\0');
        }
    }
    return sha256_hex(all);
}

// ---- running the original
namespace {

// the pickle of {"console": bytes, "exit": code, "files": {name: bytes}}
py::bytes pickle_run(const py::bytes& console, const Exit& code, const py::dict<str, py::bytes>& files) {
    namespace pk = cache::pickle;
    std::string out("\x80\x03", 2);
    pk::put_op(out, '}');
    pk::put_mark(out);
    pk::put_str(out, str("console"));
    pk::put_bytes(out, console);
    pk::put_str(out, str("exit"));
    if (std::holds_alternative<int64_t>(code)) pk::put_int(out, std::get<int64_t>(code));
    else if (std::holds_alternative<str>(code)) pk::put_str(out, std::get<str>(code));
    else pk::put_op(out, 'N');
    pk::put_str(out, str("files"));
    pk::put_op(out, '}');
    if (!files.empty()) {
        pk::put_mark(out);
        for (const auto& kv : files.items()) {
            pk::put_str(out, std::get<0>(kv));
            pk::put_bytes(out, std::get<1>(kv));
        }
        pk::put_op(out, 'u');
    }
    pk::put_op(out, 'u');
    out += '.';
    return py::bytes(out);
}

struct Kept {
    py::bytes console;
    Exit code;
    py::dict<str, py::bytes> files;
};

std::optional<Kept> unpickle_run(const py::bytes& data) {
    namespace pk = cache::pickle;
    const std::optional<pk::Node> node = pk::loads(data);
    if (!node || node->kind != pk::Node::Dict) return std::nullopt;
    Kept k;
    bool have_console = false, have_exit = false, have_files = false;
    for (size_t i = 0; i + 1 < node->items.size(); i += 2) {
        const pk::Node& key = node->items[i];
        const pk::Node& val = node->items[i + 1];
        if (key.kind != pk::Node::Str) continue;
        if (key.s == str("console") && val.kind == pk::Node::Bytes) {
            k.console = val.by;
            have_console = true;
        } else if (key.s == str("exit")) {
            have_exit = true;
            if (val.kind == pk::Node::Int) k.code = Exit(val.i);
            else if (val.kind == pk::Node::Str) k.code = Exit(val.s);
            else k.code = Exit(std::monostate{});
        } else if (key.s == str("files") && val.kind == pk::Node::Dict) {
            have_files = true;
            for (size_t j = 0; j + 1 < val.items.size(); j += 2) k.files.set(val.items[j].s, val.items[j + 1].by);
        }
    }
    if (!have_console || !have_exit || !have_files) return std::nullopt;      // (a KeyError: run again)
    return k;
}

}  // namespace

std::tuple<py::bytes, Exit> run_original(const Config& cfg, const str& exe, const str& folder, const str& cmd, const str& stdin_) {
    (void)cfg;
    const str key = _original_key(exe, folder, cmd, stdin_);
    const std::optional<py::bytes> got = cache::get(str("port-originals"), key);
    if (got) {
        const std::optional<Kept> k = unpickle_run(*got);
        if (k) {
            try {
                for (const str& n : py::os::listdir(folder))                 // (the folder as the run left it)
                    if (py::os::path::isfile(py::os::path::join(folder, n))) py::os::remove(py::os::path::join(folder, n));
                for (const auto& kv : k->files.items()) write_bytes(py::os::path::join(folder, std::get<0>(kv)), std::get<1>(kv));
                return {k->console, k->code};
            } catch (const py::OSError&) {
            }
        }
    }
    codebreaker::base::parallel::wait_for_memory(str("verify"));
    py::bytes console;
    Exit code;
    try {
        const dos::Result r = dos::run(exe, folder, !cmd.empty() ? str(" ") + cmd : str(""), stdin_.encode("latin1"), dos::DEFAULT_MAX_BLOCKS,
                                       false, py::bytes(std::string("PATH=\0", 6)), std::nullopt, std::nullopt, true);
        console = r.console;
        code = r.exit ? Exit(*r.exit) : Exit(std::monostate{});
    } catch (const py::RuntimeError& e) {                    // (a run that does not end: what it showed, and that)
        console = str(e.what()).encode("latin1", "replace");
        code = Exit(str("did not end"));
    }
    try {
        py::dict<str, py::bytes> files;
        for (const str& n : py::os::listdir(folder))
            if (py::os::path::isfile(py::os::path::join(folder, n))) files.set(n, py::bytes(read_all(py::os::path::join(folder, n))));
        const py::bytes data = pickle_run(console, code, files);
        cache::put(str("port-originals"), key, data);
    } catch (const py::OSError&) {
    }
    return {console, code};
}

// ---- running the port
std::tuple<py::bytes, int64_t> run_port(const Config& cfg, const str& exe, const str& folder, const str& cmd, const str& stdin_,
                                        const py::dict<str, str>& env) {
    (void)cfg;
    // the environment: the process' own (names upper-cased, as os.environ has them on Windows) and the case's
    std::vector<std::pair<std::wstring, std::wstring>> vars;
    if (LPWCH block = GetEnvironmentStringsW()) {
        for (const wchar_t* p = block; *p; p += wcslen(p) + 1) {
            const std::wstring kv = p;
            const size_t eq = kv.find(L'=', 1);
            if (eq == std::wstring::npos) continue;
            std::wstring k = kv.substr(0, eq);
            for (wchar_t& c : k) c = static_cast<wchar_t>(towupper(c));
            bool seen = false;
            for (auto& v : vars)
                if (v.first == k) {
                    v.second = kv.substr(eq + 1);
                    seen = true;
                }
            if (!seen) vars.emplace_back(k, kv.substr(eq + 1));
        }
        FreeEnvironmentStringsW(block);
    }
    for (const auto& kv : env.items()) {
        const std::wstring k = py::subprocess::to_wide(std::get<0>(kv)), v = py::subprocess::to_wide(std::get<1>(kv));
        bool seen = false;
        for (auto& e : vars)
            if (e.first == k) {
                e.second = v;
                seen = true;
            }
        if (!seen) vars.emplace_back(k, v);
    }
    std::wstring envblock;
    for (const auto& v : vars) envblock += v.first + L"=" + v.second + L'\0';
    envblock += L'\0';

    py::list<str> args;
    args.append(exe);
    for (const str& a : py::split(cmd)) args.append(a);
    std::wstring cmdline = py::subprocess::to_wide(py::subprocess::list2cmdline(args));
    const std::wstring wcwd = py::subprocess::to_wide(folder);
    const std::string input = stdin_.encode("latin1").raw();

    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE in_r = nullptr, in_w = nullptr, out_r = nullptr, out_w = nullptr;
    if (!CreatePipe(&in_r, &in_w, &sa, 0) || !CreatePipe(&out_r, &out_w, &sa, 0)) py::raise_winerror(GetLastError());
    SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = out_w;
    PROCESS_INFORMATION pi{};
    const BOOL ok = CreateProcessW(nullptr, cmdline.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, envblock.data(), wcwd.c_str(), &si, &pi);
    const DWORD err = GetLastError();
    CloseHandle(in_r);
    CloseHandle(out_w);
    if (!ok) {
        CloseHandle(in_w);
        CloseHandle(out_r);
        py::raise_winerror(err);
    }
    CloseHandle(pi.hThread);
    std::string raw;
    std::thread reader([&] {
        char buf[8192];
        DWORD got = 0;
        while (ReadFile(out_r, buf, sizeof buf, &got, nullptr) && got > 0) raw.append(buf, got);
    });
    std::thread writer([&] {
        size_t off = 0;
        DWORD wrote = 0;
        while (off < input.size() && WriteFile(in_w, input.data() + off, static_cast<DWORD>(input.size() - off), &wrote, nullptr) && wrote > 0) off += wrote;
        CloseHandle(in_w);
    });
    const DWORD w = WaitForSingleObject(pi.hProcess, 60000);
    bool timed_out = false;
    if (w == WAIT_TIMEOUT) {
        timed_out = true;
        TerminateProcess(pi.hProcess, 1);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    writer.join();
    reader.join();
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(out_r);
    if (timed_out) throw TimeoutExpired(py::bytes(raw));
    return {py::bytes(raw), static_cast<int64_t>(code)};
}

// ---- the cases alone
py::list<std::tuple<str, str>> run_cases(const str& path, const str& exe, const std::optional<py::dict<str, str>>& env_in) {
    const str base = py::os::path::dirname(py::os::path::abspath(path));
    const Config cfg = load_config(path);
    auto at = [&base](const str& p) { return py::os::path::isabs(p) ? p : py::os::path::join(base, p); };
    py::dict<str, str> inputs;
    for (const auto& kv : cfg.inputs.items()) inputs.set(std::get<0>(kv), at(std::get<1>(kv)));
    codebreaker::exact::emu::native_dosexec::enabled();      // (as main does)
    py::dict<str, str> e = cfg.env.copy();
    if (env_in)
        for (const auto& kv : env_in->items()) e.set(std::get<0>(kv), std::get<1>(kv));

    std::function<void(const Case&, const str&, std::vector<std::tuple<str, str>>&)> one;
    one = [&](const Case& c, const str& fb, std::vector<std::tuple<str, str>>& out) {
        for (const str& n : c.files) py::shutil::copy(inputs.at(n), py::os::path::join(fb, n));
        for (const auto& kv : c.make.items()) write_bytes(py::os::path::join(fb, std::get<0>(kv)), std::get<1>(kv).encode("latin1"));
        std::optional<Program> prog;
        if (c.program && cfg.programs.contains(*c.program)) prog = cfg.programs.at(*c.program);
        if (prog) {                                         // (another pass: its own port)
            const str exe2 = at(prog->port);
            if (!py::os::path::isfile(exe2)) return;
        }
        py::bytes con;
        try {
            con = std::get<0>(run_port(cfg, prog ? at(prog->port) : exe, fb, c.cmd, c.stdin_, e));
        } catch (const TimeoutExpired& ex) {                // (what it said before it was stopped: the sanitizer's reports among it)
            const std::string o = ex.output.raw();
            con = py::bytes((o.size() > 200000 ? o.substr(o.size() - 200000) : o) + "\n(did not end)");
        }
        out.emplace_back(c.name, con.decode("latin1"));
        for (const Case& c2 : c.then) one(c2, fb, out);
    };
    auto top = [&](const Case& c) {
        std::vector<std::tuple<str, str>> o;
        const str fb = py::tempfile::mkdtemp(std::nullopt, str("cbs_"));
        try {
            one(c, fb, o);
        } catch (...) {
            py::shutil::rmtree(fb, true);
            throw;
        }
        py::shutil::rmtree(fb, true);
        py::list<std::tuple<str, str>> l;
        for (const auto& x : o) l.append(x);
        return l;
    };
    py::list<std::tuple<str, str>> out;
    for (const auto& o : codebreaker::jobs::pool::threads(top, cfg.cases))
        for (const auto& x : o) out.append(x);
    return out;
}

py::dict<str, py::bytes> files_of(const str& folder) {
    py::dict<str, py::bytes> out;
    for (const str& n : sorted_names(folder)) {
        const str p = py::os::path::join(folder, n);
        if (py::os::path::isfile(p)) out.set(n.upper(), py::bytes(read_all(p)));
    }
    return out;
}

py::list<str> show(const py::bytes& b) { return py::splitlines(b.decode("latin1").replace(str("\r\n"), str("\n"))); }

bool fnmatch(const str& name_in, const str& pat_in) {
    auto norm = [](const str& s) { return s.lower().replace(str("/"), str("\\")); };
    const std::u32string name = norm(name_in).u32(), pat = norm(pat_in).u32();
    std::function<bool(size_t, size_t)> m = [&](size_t i, size_t j) -> bool {
        while (i < pat.size()) {
            const char32_t c = pat[i];
            if (c == U'*') {
                while (i < pat.size() && pat[i] == U'*') i++;
                if (i == pat.size()) return true;
                for (size_t k = j; k <= name.size(); k++)
                    if (m(i, k)) return true;
                return false;
            }
            if (c == U'?') {
                if (j >= name.size()) return false;
                i++;
                j++;
                continue;
            }
            if (c == U'[') {
                size_t k = i + 1;
                if (k < pat.size() && pat[k] == U'!') k++;
                if (k < pat.size() && pat[k] == U']') k++;
                while (k < pat.size() && pat[k] != U']') k++;
                if (k >= pat.size()) {                      // (no closing bracket: the bracket itself)
                    if (j >= name.size() || name[j] != U'[') return false;
                    i++;
                    j++;
                    continue;
                }
                if (j >= name.size()) return false;
                size_t s = i + 1;
                bool neg = false;
                if (pat[s] == U'!') {
                    neg = true;
                    s++;
                }
                bool hit = false;
                for (size_t q = s; q < k; q++) {
                    if (q + 2 < k && pat[q + 1] == U'-') {
                        if (pat[q] <= name[j] && name[j] <= pat[q + 2]) hit = true;
                        q += 2;
                    } else if (pat[q] == name[j]) {
                        hit = true;
                    }
                }
                if (hit == neg) return false;
                i = k + 1;
                j++;
                continue;
            }
            if (j >= name.size() || name[j] != c) return false;
            i++;
            j++;
        }
        return j == name.size();
    };
    return m(0, 0);
}

// ---- main
namespace {

using Row = std::tuple<str, str, str, str>;      // (case, command, verdict, what)

}  // namespace

int64_t main(const py::list<str>& argv, std::ostream& out) {
    auto has = [&argv](const char* flag) {
        for (const str& a : argv)
            if (a == str(flag)) return true;
        return false;
    };
    auto after = [&argv](const char* flag) {
        int64_t at = 0;
        while (argv[at] != str(flag)) at++;
        return argv[at + 1];
    };
    codebreaker::base::parallel::throttle(std::nullopt, true);
    codebreaker::exact::emu::native_dosexec::enabled();      // (the C++ executive is loaded here, before the cases run at once on threads)
    const str path = argv[0];
    const str base = py::os::path::dirname(py::os::path::abspath(path));
    const Config cfg = load_config(path);
    auto at = [&base](const str& p) { return py::os::path::isabs(p) ? p : py::os::path::join(base, p); };
    if (!cfg.original) throw py::KeyError("'original'");
    const str orig = at(*cfg.original);
    str port;
    if (has("--port")) {
        port = py::os::path::abspath(after("--port"));
    } else {
        if (!cfg.port) throw py::KeyError("'port'");
        port = at(*cfg.port);
    }
    if (!py::os::path::isfile(port)) {
        out << (str("the port is not built: ") + port).utf8() << "\n";
        return 2;
    }
    py::dict<str, str> inputs;
    for (const auto& kv : cfg.inputs.items()) inputs.set(std::get<0>(kv), at(std::get<1>(kv)));

    std::function<void(const Case&, const str&, const str&, std::vector<Row>&, std::vector<str>&)> one;
    one = [&](const Case& c, const str& fa, const str& fb, std::vector<Row>& rows, std::vector<str>& detail) {
        for (const str& n : c.files)
            for (const str* f : {&fa, &fb}) py::shutil::copy(inputs.at(n), py::os::path::join(*f, n));
        for (const auto& kv : c.make.items())                // (small inputs written out: a response file)
            for (const str* f : {&fa, &fb}) write_bytes(py::os::path::join(*f, std::get<0>(kv)), std::get<1>(kv).encode("latin1"));
        const str& cmd = c.cmd;
        const str& stdin_ = c.stdin_;
        std::optional<Program> prog;
        if (c.program && cfg.programs.contains(*c.program)) prog = cfg.programs.at(*c.program);
        const str oa = prog ? at(prog->original) : orig, pb = prog ? at(prog->port) : port;
        py::bytes ca, cb;
        Exit xa, xb;
        std::tie(ca, xa) = run_original(cfg, oa, fa, cmd, stdin_);
        try {
            int64_t code;
            std::tie(cb, code) = run_port(cfg, pb, fb, cmd, stdin_, cfg.env);
            xb = Exit(code);
        } catch (const TimeoutExpired&) {                    // (a port that does not end: so recorded)
            cb = py::bytes(std::string("(did not end)"));
            xb = Exit(str("did not end"));
        }
        py::dict<str, py::bytes> A = files_of(fa), B = files_of(fb);
        if (!cfg.volatile_.empty()) {                        // (what depends on the machine - free memory - masked alike in both)
            auto mask = [&](const py::bytes& b) {
                str t = b.decode("latin1");
                for (const str& v : cfg.volatile_) t = py::re::sub(v, str("<varies>"), t, 0, py::re::A);
                return t.encode("latin1");
            };
            ca = mask(ca);
            cb = mask(cb);
            auto masked = [&](const py::dict<str, py::bytes>& d) {
                py::dict<str, py::bytes> r;
                for (const auto& kv : d.items()) {
                    const str& n = std::get<0>(kv);
                    const bool m = n.endswith(str(".LST")) || n.endswith(str(".MAP")) || n.endswith(str(".TXT"));
                    r.set(n, m ? mask(std::get<1>(kv)) : std::get<1>(kv));
                }
                return r;
            };
            A = masked(A);
            B = masked(B);
        }
        py::list<str> bad;
        if (ca != cb) {
            bad.append(str("console"));
            detail.push_back(str("### ") + c.name + str(": console\n\n```diff\n") +
                             py::join(str("\n"), difflib::unified_diff(show(ca), show(cb), str("original"), str("port"), 2, str(""))) + str("\n```\n"));
        }
        if (truth_or_zero(xa) != truth_or_zero(xb)) bad.append(str("exit ") + exit_text(xa) + str(" / ") + exit_text(xb));
        std::set<std::u32string> all;
        for (const auto& kv : A.items()) all.insert(std::get<0>(kv).u32());
        for (const auto& kv : B.items()) all.insert(std::get<0>(kv).u32());
        for (const std::u32string& nu : all) {
            const str n(nu);
            bool skip = false;
            for (const str& g : c.ignore)
                if (fnmatch(n, g.upper())) skip = true;
            if (skip) continue;                              // (not compared: the case says so)
            const bool ina = A.contains(n), inb = B.contains(n);
            if (!ina || !inb || A.at(n) != B.at(n)) {
                str what;
                if (!inb) {
                    what = str("only the original's");
                } else if (!ina) {
                    what = str("only the port's");
                } else {
                    const std::string& x = A.at(n).raw();
                    const std::string& y = B.at(n).raw();
                    const size_t m = std::min(x.size(), y.size());
                    size_t first = m;
                    for (size_t i = 0; i < m; i++)
                        if (x[i] != y[i]) {
                            first = i;
                            break;
                        }
                    what = str("differs (") + str(std::to_string(x.size())) + str(" / ") + str(std::to_string(y.size())) + str(" bytes, first at ") +
                           str(std::to_string(first)) + str(")");
                }
                bad.append(n + str(" ") + what);
            }
        }
        py::set<str> given;
        for (const str& x : c.files) given.add(x.upper());
        for (const auto& kv : c.make.items()) given.add(std::get<0>(kv).upper());
        std::vector<str> made;
        for (const auto& kv : A.items())
            if (!given.contains(std::get<0>(kv))) made.push_back(std::get<0>(kv));
        std::sort(made.begin(), made.end(), [](const str& p, const str& q) { return p.u32() < q.u32(); });
        py::list<str> made_l;
        for (const str& m : made) made_l.append(m);
        str what = py::join(str(", "), bad);
        if (what.empty()) what = str("same: console") + (!made.empty() ? str(", ") + py::join(str(", "), made_l) : str(""));
        rows.emplace_back(c.name, cmd, V::pair(bad.empty()), what);
        for (const Case& c2 : c.then) one(c2, fa, fb, rows, detail);
    };
    auto top = [&](const Case& c) {                          // (a case and those run after it in its folders)
        std::vector<Row> r;
        std::vector<str> d;
        const str fa = py::tempfile::mkdtemp(std::nullopt, str("cbv_o_")), fb = py::tempfile::mkdtemp(std::nullopt, str("cbv_p_"));
        try {
            one(c, fa, fb, r, d);
        } catch (...) {
            py::shutil::rmtree(fa, true);
            py::shutil::rmtree(fb, true);
            throw;
        }
        py::shutil::rmtree(fa, true);
        py::shutil::rmtree(fb, true);
        return std::make_tuple(r, d);
    };
    std::vector<Row> rows;
    std::vector<str> detail;
    for (const auto& rd : codebreaker::jobs::pool::threads(top, cfg.cases)) {      // (the cases at once, the machine's budget of them; reported in their order)
        for (const Row& r : std::get<0>(rd)) rows.push_back(r);
        for (const str& d : std::get<1>(rd)) detail.push_back(d);
    }
    int64_t same = 0;
    for (const Row& r : rows)
        if (std::get<2>(r) == V::EQUIVALENT) same++;
    const str n_rows(std::to_string(rows.size()));
    py::list<str> L;
    L.append(str("# Behaviour: the original against the port"));
    L.append(str(""));
    L.append(str("`") + py::os::path::basename(orig) + str("` (under CodeBreaker's emulator) against `") + py::os::path::relpath(port, base) +
             str("` (native): **") + str(std::to_string(same)) + str(" of ") + n_rows + str(" cases the same**."));
    L.append(str(""));
    L.append(str("| case | command | | what |"));
    L.append(str("|---|---|---|---|"));
    for (const Row& r : rows) {
        const str& v = std::get<2>(r);
        const str said = V::SAID().at(v);
        L.append(str("| ") + std::get<0>(r) + str(" | `") + (std::get<1>(r).empty() ? str("(none)") : std::get<1>(r)) + str("` | ") +
                 (v == V::EQUIVALENT ? said : str("**") + said.upper() + str("**")) + str(" | ") + std::get<3>(r) + str(" |"));
    }
    if (!detail.empty()) {
        L.append(str(""));
        L.append(str("## Differences"));
        L.append(str(""));
        for (const str& d : detail) L.append(d);
    }
    const str text = py::join(str("\n"), L) + str("\n");
    const bool rep = has("--report");
    if (rep) {
        const str file = after("--report");
        write_bytes(file, text.replace(str("\n"), str("\r\n")).encode("utf-8"));      // (text mode on Windows: \r\n)
    }
    out << (!rep ? text : str(std::to_string(same)) + str(" of ") + n_rows + str(" cases the same (") + after("--report") + str(")")).utf8() << "\n";
    return same == static_cast<int64_t>(rows.size()) ? 0 : 1;
}

}  // namespace codebreaker::port::verify
