// codebreaker/ui/service.cpp -- typed C++ port of codebreaker.ui.service (see service.hpp).
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>
#include <thread>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/source/module_writer.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/ui/service.hpp"
#include "json.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::ui::service {

namespace F = codebreaker::store::functions;
namespace N = codebreaker::engine::native_engine;

// ---- JSON values ---------------------------------------------------------------------------------
Json& Json::set(const std::string& key, Json v) {
    kind = Obj;
    for (size_t n = 0; n < keys.size(); n++)
        if (keys[n] == key) {
            a[n] = std::move(v);
            return a[n];
        }
    keys.push_back(key);
    a.push_back(std::move(v));
    return a.back();
}

const Json* Json::find(const std::string& key) const {
    if (kind != Obj) return nullptr;
    for (size_t n = 0; n < keys.size(); n++)
        if (keys[n] == key) return &a[n];
    return nullptr;
}

const Json& Json::at(const std::string& key) const {
    const Json* p = find(key);
    if (!p) throw py::KeyError(py::repr(py::str(key)).utf8());
    return *p;
}

Json* Json::find_mut(const std::string& key) {
    if (kind != Obj) return nullptr;
    for (size_t n = 0; n < keys.size(); n++)
        if (keys[n] == key) return &a[n];
    return nullptr;
}

void Json::erase(const std::string& key) {
    for (size_t n = 0; n < keys.size(); n++)
        if (keys[n] == key) {
            keys.erase(keys.begin() + static_cast<std::ptrdiff_t>(n));
            a.erase(a.begin() + static_cast<std::ptrdiff_t>(n));
            return;
        }
}

namespace {

void dump_(std::string& o, const Json& v, const Dump& d, int depth) {
    auto nl = [&](int level) {
        if (d.indent >= 0) {
            o += '\n';
            o.append(static_cast<size_t>(d.indent * level), ' ');
        }
    };
    const std::string item = d.indent >= 0 ? std::string(d.item).substr(0, d.item.find_last_not_of(' ') + 1) : d.item;
    switch (v.kind) {
        case Json::Null: o += "null"; break;
        case Json::Bool: o += v.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(v.i); break;
        case Json::Float:
            if (std::isnan(v.d)) o += "NaN";
            else if (std::isinf(v.d)) o += v.d > 0 ? "Infinity" : "-Infinity";
            else o += py::to_str(v.d).utf8();
            break;
        case Json::Str: {
            py::json::EnsureAscii guard(d.ensure_ascii);
            py::json::put(o, py::str(v.s));
            break;
        }
        case Json::Arr:
            if (v.a.empty()) {
                o += "[]";
                break;
            }
            o += '[';
            for (size_t n = 0; n < v.a.size(); n++) {
                if (n) o += item;
                nl(depth + 1);
                dump_(o, v.a[n], d, depth + 1);
            }
            nl(depth);
            o += ']';
            break;
        case Json::Obj:
            if (v.a.empty()) {
                o += "{}";
                break;
            }
            o += '{';
            for (size_t n = 0; n < v.a.size(); n++) {
                if (n) o += item;
                nl(depth + 1);
                py::json::EnsureAscii guard(d.ensure_ascii);
                py::json::put(o, py::str(v.keys[n]));
                o += d.key;
                dump_(o, v.a[n], d, depth + 1);
            }
            nl(depth);
            o += '}';
            break;
    }
}

// the runtime's JSON (native/l2/src/json.hpp: json.loads / json.dumps as Python's) reads and writes py::V; Json is this port's tagged view
py::V to_v_(const Json& j) {
    switch (j.kind) {
        case Json::Null: return py::V();
        case Json::Bool: return py::V(j.b);
        case Json::Int: return py::V(static_cast<long long>(j.i));
        case Json::Float: return py::V(j.d);
        case Json::Str: return py::V(j.s);
        case Json::Arr: {
            py::V l = py::V::list();
            for (const auto& x : j.a) l.append(to_v_(x));
            return l;
        }
        default: {
            py::V d = py::V::dict();
            for (size_t n = 0; n < j.a.size(); n++) d.set(py::V(j.keys[n]), to_v_(j.a[n]));
            return d;
        }
    }
}

Json from_v_(const py::V& v) {
    if (v.is_none()) return Json();
    if (v.is_bool()) return Json(v.b);
    if (v.is_int()) return Json(static_cast<int64_t>(v.i));
    if (v.is_float()) return Json(v.f);
    if (v.is_str()) return Json(v.str());
    if (v.is_dict()) {
        Json o = Json::object();
        for (const auto& kv : v.items()) o.set(kv.first.str(), from_v_(kv.second));
        return o;
    }
    Json a = Json::array();
    for (const auto& x : v.seq()) a.push(from_v_(x));
    return a;
}

}  // namespace

std::string dumps(const Json& v, const Dump& opts) {
    if (opts.indent < 0) {   // json.dumps through the runtime
        py::DumpOpts o;
        o.item_sep = opts.item.c_str();
        o.key_sep = opts.key.c_str();
        o.ensure_ascii = opts.ensure_ascii;
        return py::json_dump(to_v_(v), o);
    }
    std::string o;   // (indent: the runtime's writer has none; pydantic's `indent=2` layout)
    dump_(o, v, opts, 0);
    return o;
}

Json loads(const std::string& text) {
    try {
        return from_v_(py::json_parse(text));
    } catch (const py::PyError& e) {   // (the runtime's error for bad JSON: json.JSONDecodeError is a ValueError)
        throw py::ValueError(e.what());
    }
}

// ---- state ---------------------------------------------------------------------------------------
bool START = false;
bool LOCAL = false;

py::str& OWNER() {
    static py::str owner("pid-" + std::to_string(GetCurrentProcessId()));
    return owner;
}

namespace {
thread_local std::optional<py::str> CALL_OWNER;   // in the service: the client a call is answered for
}

std::vector<py::str>& launcher() {
    static std::vector<py::str> l = [] {
        wchar_t buf[MAX_PATH * 4];
        DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH * 4);
        int m = WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), nullptr, 0, nullptr, nullptr);
        std::string u8(static_cast<size_t>(m), '\0');
        WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), u8.data(), m, nullptr, nullptr);
        return std::vector<py::str>{py::str(u8)};
    }();
    return l;
}

py::str root() { return py::settings::checkout_root(); }

py::str state_path(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const auto& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

const py::dict<py::str, py::str>& ACTIONS() {
    static const py::dict<py::str, py::str> a = {
        {py::str("decompile"), py::str("decompile it to C (nothing is built or judged)")},
        {py::str("measure"), py::str("decompile, build and judge every function; results go to the store")},
        {py::str("module"), py::str("a folder of Java source: transpile it to C++, compile every class, write the folder")}};
    return a;
}

std::optional<py::str> stored_path(const py::str& name) {
    auto fs = F::open(F::default_path(), true);
    if (!fs) return std::nullopt;
    std::optional<py::str> p;
    try {
        p = fs->module_path(name);
    } catch (...) {
        fs->close();
        throw;
    }
    fs->close();
    if (p && !p->empty() && py::os::path::exists(*p)) return p;
    return std::nullopt;
}

// ---- jobs ----------------------------------------------------------------------------------------
std::optional<int64_t> Proc::poll() {
    DWORD code = 0;
    if (!GetExitCodeProcess(static_cast<HANDLE>(handle), &code)) return std::nullopt;
    if (code == STILL_ACTIVE) return std::nullopt;
    return static_cast<int64_t>(code);
}

void Proc::terminate() { TerminateProcess(static_cast<HANDLE>(handle), 1); }

namespace {

// subprocess.Popen(argv, cwd=cwd, stdin=DEVNULL, stdout=out, stderr=out [, creationflags]) -> the process
std::shared_ptr<Proc> popen_(const std::vector<py::str>& argv, const py::str& cwd, HANDLE out, DWORD flags) {
    py::list<py::str> args;
    for (const auto& a : argv) args.append(a);
    std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(args));
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE nul = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = nul;
    si.hStdOutput = out;
    si.hStdError = out;
    PROCESS_INFORMATION pi{};
    std::wstring wcwd = py::subprocess::to_wide(cwd);
    std::vector<wchar_t> buf(cmd.begin(), cmd.end());
    buf.push_back(0);
    BOOL ok = CreateProcessW(nullptr, buf.data(), nullptr, nullptr, TRUE, flags, nullptr, wcwd.c_str(), &si, &pi);
    DWORD err = ok ? 0 : GetLastError();
    if (nul != INVALID_HANDLE_VALUE) CloseHandle(nul);
    if (!ok) py::raise_winerror(err);
    CloseHandle(pi.hThread);
    auto p = std::make_shared<Proc>();
    p->handle = pi.hProcess;
    p->pid = pi.dwProcessId;
    return p;
}

HANDLE open_log_(const py::str& path, bool append) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    std::wstring w = py::subprocess::to_wide(path);
    HANDLE h = CreateFileW(w.c_str(), append ? FILE_APPEND_DATA : GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                           append ? OPEN_ALWAYS : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) py::raise_winerror(GetLastError());
    return h;
}

std::string strftime_(const char* fmt, double t) {
    std::time_t tt = static_cast<std::time_t>(t);
    std::tm tm{};
    localtime_s(&tm, &tt);
    char b[64];
    std::strftime(b, sizeof b, fmt, &tm);
    return b;
}

}  // namespace

Json Jobs::start(const py::str& action, const py::str& module) {
    std::optional<py::str> path;
    if (!module.empty()) {
        if (py::os::path::isdir(module)) path = module;
        else {
            path = codebreaker::base::corpus::retail(module);
            if (!path || path->empty()) path = stored_path(module);
        }
    }
    if (!ACTIONS().contains(action)) throw py::ValueError("no such action " + py::repr(action).utf8());
    if (!path || path->empty()) throw py::ValueError("no module " + py::repr(module).utf8() + " in the corpus");
    py::str name = py::os::path::basename(py::os::path::normpath(*path)).lower();
    py::str output = action == py::str("decompile")
                         ? py::os::path::join(state_path({py::str("ui_jobs")}), std::get<0>(py::os::path::splitext(name)) + py::str(".c"))
                         : py::str("");
    py::list<py::str> args;
    args.append(*path);
    if (!output.empty()) {
        args.append(py::str("--out"));
        args.append(output);
    }
    return launch(action, args, name, output);
}

Json Jobs::launch(const py::str& action, const py::list<py::str>& args, const py::str& name, const py::str& output,
                  const std::optional<py::str>& owner) {
    std::lock_guard<std::mutex> g(lock_);
    std::vector<std::shared_ptr<Job>> running;
    for (auto& j : order_)
        if (!j->proc->poll()) running.push_back(j);
    bool another_measure = false;
    if (action == py::str("measure"))
        for (auto& j : running)
            if (j->action == py::str("measure")) another_measure = true;
    if (running.size() >= 2 || another_measure) {
        std::string who;
        for (size_t n = 0; n < running.size(); n++) {
            if (n) who += ", ";
            who += running[n]->action.utf8() + " " + running[n]->module.utf8();
        }
        throw py::ValueError("busy: " + who + " is still running");
    }
    count_++;
    py::str jid = py::str(strftime_("%H%M%S", py::time::time()) + "-" + std::to_string(count_));
    py::str folder_ = state_path({py::str("ui_jobs")});
    py::os::makedirs(folder_, 0777, true);
    py::str log = py::os::path::join(folder_, jid + py::str(".log"));
    std::vector<py::str> argv = launcher();
    argv.push_back(action);
    for (const auto& a : args) argv.push_back(a);
    HANDLE h = open_log_(log, false);
    std::shared_ptr<Proc> proc;
    try {
        proc = popen_(argv, root(), h, 0x00004000);   // below normal
    } catch (...) {
        CloseHandle(h);
        throw;
    }
    CloseHandle(h);
    auto job = std::make_shared<Job>();
    job->id = jid;
    job->action = action;
    job->module = name;
    job->proc = proc;
    job->log = log;
    job->output = output;
    job->started = py::time::time();
    job->owner = owner ? *owner : caller();
    order_.push_back(job);
    return view(*job);
}

Json Jobs::view(Job& job, const std::optional<int64_t>& offset) {
    auto code = job.proc->poll();
    if (code && !job.ended) job.ended = py::time::time();
    Json out = Json::object();
    out.set("id", job.id);
    out.set("action", job.action);
    out.set("module", job.module);
    out.set("pid", static_cast<int64_t>(job.proc->pid));
    out.set("status", !code ? "running" : *code == 0 ? "done" : "failed");
    out.set("exit", code ? Json(*code) : Json());
    out.set("seconds", static_cast<int64_t>((job.ended ? *job.ended : py::time::time()) - job.started));
    out.set("started", strftime_("%H:%M:%S", job.started));
    out.set("owner", job.owner);
    out.set("has_output", !job.output.empty() && code && *code == 0 && py::os::path::isfile(job.output));
    if (offset) {
        std::string chunk;
        int64_t at = std::max<int64_t>(0, *offset);
        std::ifstream f(std::filesystem::u8path(job.log.utf8()), std::ios::binary);
        if (f) {
            f.seekg(at);
            std::string buf(1 << 18, '\0');
            f.read(buf.data(), static_cast<std::streamsize>(buf.size()));
            chunk = buf.substr(0, static_cast<size_t>(f.gcount()));
        }
        out.set("log", py::bytes(chunk).decode("utf-8", "replace"));
        out.set("next", at + static_cast<int64_t>(chunk.size()));
    }
    return out;
}

Json Jobs::all() {
    std::lock_guard<std::mutex> g(lock_);
    std::vector<std::shared_ptr<Job>> sorted = order_;
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a->started > b->started; });
    Json out = Json::array();
    for (auto& j : sorted) out.push(view(*j));
    return out;
}

std::shared_ptr<Job> Jobs::get(const py::str& jid) {
    for (auto& j : order_)
        if (j->id == jid) return j;
    return nullptr;
}

bool Jobs::stop(const py::str& jid) {
    auto job = get(jid);
    if (job && !job->proc->poll()) job->proc->terminate();   // only a process started here
    return static_cast<bool>(job);
}

void Jobs::stop_all(const std::optional<py::str>& owner) {
    std::vector<py::str> ids;
    for (auto& j : order_) ids.push_back(j->id);
    for (const auto& jid : ids) {
        auto job = get(jid);
        if (!owner || job->owner == *owner) stop(jid);
    }
}

int64_t Jobs::running() {
    std::lock_guard<std::mutex> g(lock_);
    int64_t n = 0;
    for (auto& j : order_)
        if (!j->proc->poll()) n++;
    return n;
}

Jobs& JOBS() {
    static Jobs j;
    return j;
}

// ---- the function store's view ---------------------------------------------------------------------
LedgerHooks& ledger_hooks() {
    static LedgerHooks h;
    return h;
}

namespace {

py::str cell_text_(const N::Cell& c) {
    if (auto s = std::get_if<py::str>(&c)) return *s;
    if (auto b = std::get_if<py::bytes>(&c)) return b->decode("utf-8", "replace");
    if (auto i = std::get_if<int64_t>(&c)) return py::str(std::to_string(*i));
    if (auto d = std::get_if<double>(&c)) return py::to_str(*d);
    return py::str("");
}
bool cell_truthy_(const N::Cell& c) {
    if (auto s = std::get_if<py::str>(&c)) return !s->empty();
    if (auto b = std::get_if<py::bytes>(&c)) return b->size() > 0;
    if (auto i = std::get_if<int64_t>(&c)) return *i != 0;
    if (auto d = std::get_if<double>(&c)) return *d != 0.0;
    return false;
}
Json cell_json_(const N::Cell& c) {
    if (auto s = std::get_if<py::str>(&c)) return Json(*s);
    if (auto b = std::get_if<py::bytes>(&c)) return Json(b->decode("utf-8", "replace"));
    if (auto i = std::get_if<int64_t>(&c)) return Json(*i);
    if (auto d = std::get_if<double>(&c)) return Json(*d);
    return Json();
}
// `x or ""`
Json or_empty_(const N::Cell& c) { return cell_truthy_(c) ? cell_json_(c) : Json(""); }

// One function for the page: the store's best row for it, in words.
Json row_(const N::Candidate* row, const py::str& addr) {
    Json o = Json::object();
    if (!row) {
        o.set("addr", addr);
        o.set("name", "");
        o.set("class", "no-source");
        o.set("title", F::TITLE()[py::str("no-source")]);
        o.set("why", "no result under the verifier settings in force");
        o.set("engine", "");
        o.set("has_source", false);
        return o;
    }
    py::str klass = cell_text_(row->klass);
    o.set("addr", addr);
    o.set("name", cell_truthy_(row->name) ? cell_text_(row->name) : py::str(""));
    o.set("class", klass);
    o.set("title", F::TITLE()[klass]);
    o.set("why", codebreaker::source::module_writer::describe(*row));
    o.set("engine", cell_json_(row->engine));
    o.set("has_source", cell_truthy_(row->source));
    return o;
}

}  // namespace

Json _functions(const py::str& module, const std::optional<py::str>& addr) {
    auto fs = F::open(F::default_path(), true);
    struct Closer {
        std::shared_ptr<N::FunctionStore> fs;
        ~Closer() { if (fs) fs->close(); }
    } closer{fs};
    auto& hooks = ledger_hooks();
    if (!hooks.measure) throw py::RuntimeError("service.functions: the ledger (coverage.ledger) has no typed port: set ledger_hooks()");
    Json led = hooks.measure(module);
    const Json& mods = led.at("modules");
    if (mods.a.empty()) return Json();
    const Json& m = mods.a[0];
    const Json* digest_j = led.at("profiles").find(m.at("kind").s);
    py::str digest = digest_j && digest_j->is_str() ? digest_j->str() : py::str("");
    Json out = Json::object();
    out.set("module", m.at("name"));
    out.set("kind", m.at("kind"));
    auto title = hooks.title.find(m.at("kind").s);
    out.set("kind_title", title == hooks.title.end() ? Json() : Json(title->second));
    const Json* sha_j = m.find("sha");
    out.set("sha", sha_j ? *sha_j : Json(""));
    out.set("profile", digest_j ? *digest_j : Json());
    out.set("in_store", m.at("in_store"));
    out.set("measured", m.at("measured"));
    Json counts = Json::object();
    for (const auto& k : hooks.counts) counts.set(k.utf8(), m.at(k.utf8()));
    out.set("counts", counts);
    out.set("functions", Json::array());
    out.set("reasons", Json::object());
    const Json& in_store = m.at("in_store");
    bool stored = in_store.kind == Json::Bool ? in_store.b : in_store.kind == Json::Int ? in_store.i != 0 : !in_store.is_null();
    if (!stored) return out;
    py::str sha = sha_j && sha_j->is_str() ? sha_j->str() : py::str("");
    py::dict<py::str, N::Candidate> best;
    if (!digest.empty()) best = fs->best(sha, digest);
    auto get_best = [&](const py::str& a) -> const N::Candidate* { return best.contains(a) ? &best[a] : nullptr; };
    if (addr) {
        const N::Candidate* row = get_best(*addr);
        Json one = row_(row, *addr);
        if (row) {
            one.set("source", or_empty_(row->source));
            one.set("verdict", cell_json_(row->verdict));
            one.set("reason", or_empty_(row->reason));
            one.set("limits", or_empty_(row->limits));
            one.set("detail", or_empty_(row->detail));
            one.set("rev", or_empty_(row->rev));
            one.set("problem", or_empty_(row->problem));
            Json weak = Json::array();
            for (const auto& c : row->weak) {
                Json w = Json::object();
                w.set("code", c);
                w.set("text", F::WEAK()[c]);
                weak.push(w);
            }
            one.set("weak", weak);
        }
        out.set("function", one);
        return out;
    }
    Json fns = Json::array();
    for (const auto& a : fs->addresses(sha)) fns.push(row_(get_best(a), a));
    out.set("functions", fns);
    if (!digest.empty() && hooks.reasons) out.set("reasons", hooks.reasons(sha, digest));
    return out;
}

Json functions(const py::str& module, const std::optional<py::str>& addr) {
    Json args = Json::object();
    args.set("module", module);
    args.set("addr", addr);
    return call(py::str("functions"), args);
}

// ---- what can be asked ---------------------------------------------------------------------------
const std::vector<py::str>& QUERIES() {
    static const std::vector<py::str> q = {py::str("facts")};
    return q;
}

std::function<int64_t(const py::str&, const py::list<py::str>&)>& command_main() {
    static std::function<int64_t(const py::str&, const py::list<py::str>&)> f;
    return f;
}

Json _cli(const py::str& command, const py::list<py::str>& argv) {
    bool known = false;
    for (const auto& q : QUERIES())
        if (q == command) known = true;
    if (!known) {
        std::string names;
        for (const auto& q : QUERIES()) names += (names.empty() ? "" : ", ") + q.utf8();
        throw py::ValueError(py::repr(command).utf8() + " is not a query the service answers (" + names + ")");
    }
    if (!command_main()) throw py::RuntimeError("service._cli: no command runner (set command_main())");
    int64_t code;
    Captured cap;
    code = command_main()(command, argv);
    Json out = Json::object();
    out.set("exit", code);
    out.set("out", cap.getvalue());
    return out;
}

std::map<std::string, Call>& tool_calls() {
    static std::map<std::string, Call> t;
    return t;
}

namespace {

py::str arg_str_(const Json& a, const char* key) {
    const Json* v = a.find(key);
    if (!v || v->is_null()) throw py::TypeError(std::string("missing argument ") + key);
    return v->str();
}
std::optional<py::str> arg_opt_str_(const Json& a, const char* key) {
    const Json* v = a.find(key);
    if (!v || v->is_null()) return std::nullopt;
    return v->str();
}

std::map<std::string, Call>& calls_() {
    static std::map<std::string, Call> c = [] {
        std::map<std::string, Call> m;
        m["functions"] = [](const Json& a) { return _functions(arg_str_(a, "module"), arg_opt_str_(a, "addr")); };
        m["store.overview"] = [](const Json&) {
            Json out = Json::array();
            for (const auto& o : F::overview()) {
                Json r = Json::object();
                r.set("sha", o.sha);
                r.set("name", o.name);
                r.set("kind", o.kind);
                r.set("path", o.path);
                r.set("functions", o.functions);
                r.set("identified", o.identified);
                Json latest = Json::object();
                for (const auto& kv : o.latest.items()) latest.set(std::get<0>(kv).utf8(), std::get<1>(kv));
                r.set("latest", latest);
                out.push(r);
            }
            return out;
        };
        m["store.latest"] = [](const Json& a) {
            Json out = Json::object();
            for (const auto& kv : F::latest(arg_str_(a, "module_sha")).items()) out.set(std::get<0>(kv).utf8(), cell_json_(std::get<1>(kv)));
            return out;
        };
        m["cli"] = [](const Json& a) {
            py::list<py::str> argv;
            if (auto* v = a.find("argv"))
                for (const auto& x : v->a) argv.append(x.str());
            return _cli(arg_str_(a, "command"), argv);
        };
        m["jobs.all"] = [](const Json&) { return JOBS().all(); };
        m["jobs.running"] = [](const Json&) { return Json(JOBS().running()); };
        m["jobs.stop_all"] = [](const Json& a) {
            JOBS().stop_all(arg_opt_str_(a, "owner"));
            return Json();
        };
        return m;
    }();
    return c;
}

}  // namespace

Call registered(const py::str& name) {
    auto& c = calls_();
    auto it = c.find(name.utf8());
    if (it != c.end()) return it->second;
    if (name.startswith(py::str("mcp."))) {
        auto& t = tool_calls();
        auto jt = t.find(name.utf8().substr(4));
        if (jt != t.end()) return jt->second;
    }
    throw py::LookupError("no call " + py::repr(name).utf8());
}

py::str caller() { return CALL_OWNER && !CALL_OWNER->empty() ? *CALL_OWNER : OWNER(); }

AnsweringFor::AnsweringFor(const std::optional<py::str>& owner) : was(CALL_OWNER) { CALL_OWNER = owner; }
AnsweringFor::~AnsweringFor() { CALL_OWNER = was; }

// ---- captured ------------------------------------------------------------------------------------
namespace {
std::recursive_mutex& capture_mutex_() {
    static std::recursive_mutex m;
    return m;
}
}  // namespace

Captured::Captured() : lock_(capture_mutex_()) {
    std::fflush(stdout);
    char dir[MAX_PATH + 1];
    GetTempPathA(MAX_PATH, dir);
    char name[MAX_PATH + 1];
    GetTempFileNameA(dir, "cbc", 0, name);
    path_ = name;
    int fd = _open(name, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, 0600);
    saved_ = _dup(1);
    _dup2(fd, 1);
    _close(fd);
}

py::str Captured::getvalue() {
    if (!done_) {
        std::fflush(stdout);
        _dup2(saved_, 1);
        _close(saved_);
        std::ifstream f(path_, std::ios::binary);
        std::stringstream ss;
        ss << f.rdbuf();
        std::string raw = ss.str();
        for (size_t n = 0; (n = raw.find("\r\n", n)) != std::string::npos;) raw.erase(n, 1);   // (text mode: \n was written as \r\n)
        text_ = raw;
        std::remove(path_.c_str());
        done_ = true;
    }
    return py::bytes(text_).decode("utf-8", "replace");
}

Captured::~Captured() {
    if (!done_) getvalue();
}

namespace {
std::mutex g_silence_lock;
int g_silenced = 0;
int g_silence_saved = -1;
}  // namespace

Silenced::Silenced() {
    std::lock_guard<std::mutex> g(g_silence_lock);
    if (g_silenced++ == 0) {
        std::fflush(stdout);
        g_silence_saved = _dup(1);
        int nul = _open("NUL", _O_WRONLY | _O_BINARY);
        _dup2(nul, 1);
        _close(nul);
    }
}

Silenced::~Silenced() {
    std::lock_guard<std::mutex> g(g_silence_lock);
    if (--g_silenced == 0) {
        std::fflush(stdout);
        _dup2(g_silence_saved, 1);
        _close(g_silence_saved);
    }
}

// ---- the client ----------------------------------------------------------------------------------
py::str mode() {
    py::str v = py::os::environ_::get(py::str("CB_SERVICE"), py::str("")).strip().lower();
    if (v == py::str("0") || v == py::str("off") || v == py::str("no") || v == py::str("false")) return py::str("off");
    if (v == py::str("1") || v == py::str("on") || v == py::str("yes") || v == py::str("true")) return py::str("on");
    return py::str("");
}

py::str folder() { return py::os::path::dirname(state_path({py::str("service"), py::str("service.json")})); }

Json record() {
    try {
        std::ifstream f(std::filesystem::u8path(py::os::path::join(folder(), py::str("service.json")).utf8()), std::ios::binary);
        if (!f) return Json();
        std::stringstream ss;
        ss << f.rdbuf();
        return loads(ss.str());
    } catch (const py::ValueError&) {
        return Json();
    } catch (const py::OSError&) {
        return Json();
    }
}

namespace {

bool same_(const py::str& a, const py::str& b) {
    return py::os::path::normcase(py::os::path::abspath(a)) == py::os::path::normcase(py::os::path::abspath(b));
}

void wsa_() {
    static bool done = [] {
        WSADATA d;
        WSAStartup(MAKEWORD(2, 2), &d);
        return true;
    }();
    (void)done;
}

}  // namespace

Json Client::request(const py::str& method, const py::str& path, const std::optional<Json>& body, double timeout) {
    wsa_();
    std::string data = body ? dumps(*body) : std::string();
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) throw Gone("socket");
    struct Closer {
        SOCKET s;
        ~Closer() { closesocket(s); }
    } closer{s};
    if (timeout > 0) {
        DWORD ms = static_cast<DWORD>(timeout * 1000);
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
        setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
    }
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (connect(s, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0) {
        int e = WSAGetLastError();
        if (e == WSAETIMEDOUT) throw py::TimeoutError("timed out");
        throw Gone("[WinError " + std::to_string(e) + "] connect");
    }
    std::string req = method.utf8() + " " + path.utf8() + " HTTP/1.1\r\nHost: 127.0.0.1:" + std::to_string(port) +
                      "\r\nAccept-Encoding: identity\r\n";
    if (body) req += "Content-Length: " + std::to_string(data.size()) + "\r\n";
    req += "X-CodeBreaker-Token: " + secret.utf8() + "\r\nContent-Type: application/json\r\n\r\n" + data;
    for (size_t sent = 0; sent < req.size();) {
        int n = send(s, req.data() + sent, static_cast<int>(req.size() - sent), 0);
        if (n <= 0) {
            if (WSAGetLastError() == WSAETIMEDOUT) throw py::TimeoutError("timed out");
            throw Gone("send");
        }
        sent += static_cast<size_t>(n);
    }
    std::string in;
    char buf[8192];
    size_t head_end = std::string::npos;
    size_t want = 0;
    for (;;) {
        if (head_end != std::string::npos && in.size() >= head_end + 4 + want) break;
        int n = recv(s, buf, sizeof buf, 0);
        if (n == 0) {
            if (head_end == std::string::npos) throw Gone("Remote end closed connection without response");
            break;
        }
        if (n < 0) {
            if (WSAGetLastError() == WSAETIMEDOUT) throw py::TimeoutError("timed out");
            throw Gone("recv");
        }
        in.append(buf, static_cast<size_t>(n));
        if (head_end == std::string::npos && (head_end = in.find("\r\n\r\n")) != std::string::npos) {
            std::string head = in.substr(0, head_end);
            std::string low = head;
            std::transform(low.begin(), low.end(), low.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            size_t cl = low.find("content-length:");
            want = cl == std::string::npos ? 0 : static_cast<size_t>(std::stoull(low.substr(cl + 15)));
        }
    }
    int status = 0;
    if (in.compare(0, 5, "HTTP/") == 0 && in.size() > 12) status = std::atoi(in.c_str() + in.find(' ') + 1);
    if (status == 401) throw Gone("the service refused the token");
    std::string text = in.substr(head_end + 4, want);
    return loads(text);
}

Json Client::ping(double timeout) { return request(py::str("GET"), py::str("/ping"), std::nullopt, timeout); }

Json Client::call(const py::str& name, const Json& args, double timeout) {
    Json body = Json::object();
    body.set("name", name);
    body.set("args", args);
    body.set("owner", OWNER());
    Json got = request(py::str("POST"), py::str("/call"), body, timeout);
    if (const Json* e = got.find("error")) {
        const Json* k = got.find("kind");
        std::string kind = k && k->is_str() ? k->s : "";
        if (kind == "ValueError") throw py::ValueError(e->s);
        if (kind == "OSError") throw py::OSError(e->s);
        if (kind == "LookupError") throw py::LookupError(e->s);
        throw RemoteError(e->s);
    }
    const Json* v = got.find("value");
    return v ? *v : Json();
}

void Client::stop() {
    try {
        request(py::str("POST"), py::str("/stop"), Json::object(), 5.0);
    } catch (const Gone&) {
    } catch (const py::TimeoutError&) {
    }
}

bool Client::wait_gone(double seconds) {
    double end = py::time::time() + seconds;
    while (py::time::time() < end) {
        try {
            ping(1.0);
        } catch (const Gone&) {
            return true;
        } catch (const py::OSError&) {
            return true;
        }
        py::time::sleep(0.2);
    }
    return false;
}

std::shared_ptr<Client> find() {
    Json rec = record();
    if (rec.is_null()) return nullptr;
    const Json* port = rec.find("port");
    if (!port || (port->is_null()) || (port->is_int() && port->i == 0)) return nullptr;
    std::string secret;
    {
        std::ifstream f(std::filesystem::u8path(py::os::path::join(folder(), py::str("token")).utf8()), std::ios::binary);
        if (!f) return nullptr;
        std::stringstream ss;
        ss << f.rdbuf();
        secret = py::str(ss.str()).strip().utf8();
    }
    const Json* pid = rec.find("pid");
    auto c = std::make_shared<Client>(port->i, py::str(secret),
                                      pid && pid->is_int() ? std::optional<int64_t>(pid->i) : std::nullopt);
    Json pong;
    try {
        pong = c->ping();
    } catch (const Gone&) {
        return nullptr;
    } catch (const py::OSError&) {
        return nullptr;
    }
    const Json* st = pong.find("state");
    if (!same_(st ? st->str() : py::str(""), py::settings::get_str(py::str("state")))) return nullptr;
    const Json* stale = pong.find("stale");
    const Json* jobs = pong.find("jobs");
    bool is_stale = stale && stale->kind == Json::Bool && stale->b;
    bool has_jobs = jobs && jobs->is_int() && jobs->i != 0;
    if (is_stale && !has_jobs) {
        c->stop();
        c->wait_gone();
        return nullptr;
    }
    return c;
}

std::vector<int64_t>& LAUNCHED() {
    static std::vector<int64_t> l;
    return l;
}

std::shared_ptr<Client> launch(double wait) {
    HANDLE log = open_log_(py::os::path::join(folder(), py::str("serve.log")), true);
    std::vector<py::str> argv = launcher();
    argv.push_back(py::str("serve"));
    std::shared_ptr<Proc> proc;
    try {
        const DWORD flags = 0x08000000 | 0x00000200;   // no window, a process group of its own
        try {
            proc = popen_(argv, root(), log, flags | 0x01000000);   // out of our job
        } catch (const py::OSError&) {
            proc = popen_(argv, root(), log, flags);
        }
    } catch (...) {
        CloseHandle(log);
        throw;
    }
    CloseHandle(log);
    LAUNCHED().push_back(proc->pid);
    double end = py::time::time() + wait;
    while (py::time::time() < end) {
        auto c = find();
        if (c) {
            c->started = c->pid && *c->pid == static_cast<int64_t>(proc->pid);
            return c;
        }
        if (proc->poll()) return find();   // it found another one running, or failed
        py::time::sleep(0.2);
    }
    return nullptr;
}

namespace {
std::shared_ptr<Client> g_client;
double g_looked = 0.0;
std::mutex g_lock;
}  // namespace

std::shared_ptr<Client> client(const std::optional<bool>& start_in) {
    py::str m = mode();
    if (LOCAL || m == py::str("off")) return nullptr;
    bool start = m == py::str("on") ? true : start_in ? *start_in : START;
    std::lock_guard<std::mutex> g(g_lock);
    if (g_client) return g_client;
    if (py::time::time() - g_looked < 5.0) return nullptr;
    g_looked = py::time::time();
    g_client = find();
    if (!g_client && start) g_client = launch();
    return g_client;
}

namespace {
void drop_() {
    std::lock_guard<std::mutex> g(g_lock);
    g_client = nullptr;
    g_looked = 0.0;
}
}  // namespace

Json call(const py::str& what, const Json& args) {
    Call fn = registered(what);
    for (int n = 0; n < 2; n++) {
        auto c = client();
        if (!c) break;
        try {
            return c->call(what, args);
        } catch (const Gone&) {
            drop_();   // gone: look again (start it again) once
        }
    }
    return fn(args);
}

std::optional<int64_t> cli(const py::str& command, const py::list<py::str>& argv) {
    auto c = client(false);
    if (!c) return std::nullopt;
    Json got;
    try {
        Json args = Json::object();
        args.set("command", command);
        args.set("argv", Json::list_of(argv));
        got = c->call(py::str("cli"), args);
    } catch (const Gone&) {
        drop_();
        return std::nullopt;
    }
    std::string out = got.at("out").s;
    std::fwrite(out.data(), 1, out.size(), stdout);
    return got.at("exit").i;
}

void stop_mine() {
    std::shared_ptr<Client> c;
    {
        std::lock_guard<std::mutex> g(g_lock);
        c = g_client;
    }
    if (c) {
        try {
            Json args = Json::object();
            args.set("owner", OWNER());
            c->call(py::str("jobs.stop_all"), args, 10.0);
        } catch (const Gone&) {
        } catch (const py::OSError&) {
        } catch (const RemoteError&) {
        }
    }
    JOBS().stop_all();
}

}  // namespace codebreaker::ui::service
