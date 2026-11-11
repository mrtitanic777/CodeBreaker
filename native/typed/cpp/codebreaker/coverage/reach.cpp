// codebreaker/coverage/reach.cpp -- typed C++ of codebreaker.coverage.reach (see reach.hpp).
//
// reach.py -- throw anything at CodeBreaker and see how far each program gets: what each file is, which platform it belongs
// to, and with --judge how far the platform's lane takes it (`measure`, in a process of its own, stopped after --timeout
// seconds).  Results are kept in state/reach/<run>.json by each file's content hash and the tools' revision.

#include "codebreaker/coverage/reach.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/media.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "json.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
// (last: windows.h defines macros the Xbox headers above use as names, e.g. FILE_ATTRIBUTE_DIRECTORY)
#include "py_subprocess.hpp"

// formats/xbox/volumes.hpp is not included: it reaches py_answers.hpp, which declares its own Win32 prototypes and cannot share a
// translation unit with <windows.h> (judge starts a process).  This is its one function reach calls, as declared there.
namespace codebreaker::formats::xbox::volumes {
std::optional<py::list<std::shared_ptr<media::Volume>>> volumes(const py::str& path, bool long_names = true);
}

namespace codebreaker::coverage::reach {

using formats::inspect::Value;
using py::str;
namespace F = store::functions;

// ---- hooks
PlatformIdentify& platform_identify() {
    static PlatformIdentify h;
    return h;
}
std::function<py::list<str>(const str&)>& measure_argv() {
    static std::function<py::list<str>(const str&)> h = [](const str& path) {
        wchar_t buf[MAX_PATH * 4];
        DWORD n = GetModuleFileNameW(nullptr, buf, static_cast<DWORD>(sizeof buf / sizeof buf[0]));
        std::wstring w(buf, n);
        return py::list<str>{str(std::string(w.begin(), w.end())), str("measure"), path};
    };
    return h;
}
std::function<double()>& clock() {
    static std::function<double()> c = [] { return py::time::time(); };
    return c;
}

// ---- the vocabulary
const py::list<str>& _PROGRAM_EXTS() {
    static const py::list<str> e = {".xex", ".exe", ".dll", ".xbe", ".com", ".sys", ".drv", ".vxd", ".386", ".nes", ".gb",
                                    ".gbc", ".sms", ".gg", ".sg", ".md", ".gen", ".smd", ".bin", ".rom", ".sna", ".z80",
                                    ".tap", ".jar", ".gba", ".nds", ".z64", ".n64", ".v64", ".sfc", ".smc", ".elf", ".self",
                                    ".apk", ".ipa", ".xap"};
    return e;
}
const py::list<str> _SKIP_FAMILIES = {str("media"), str("xna")};
const py::list<str> _CONTAINER_FORMATS = {str("STFS package"), str("CD image"), str("Xbox disc image"), str("disk image"),
                                          str("compressed disc image")};

namespace {

bool starts_with_any(const str& s, const py::list<str>& prefixes) {
    for (const str& p : prefixes)
        if (s.startswith(p)) return true;
    return false;
}

bool ends_with_any(const str& s, const py::list<str>& suffixes) {
    for (const str& p : suffixes)
        if (s.endswith(p)) return true;
    return false;
}

// round(x, 1)
double round1(double x) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.1f", x);
    return std::strtod(buf, nullptr);
}

// settings.state(*parts)
str state_path(const std::vector<str>& parts) {
    str path = py::settings::get_str(str("state"));
    for (const str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

void print_log(const str& text) {
    py::print(text);
    std::fflush(stdout);
}

}  // namespace

// ---- what a file is, where detect.py has nothing to say
std::optional<std::tuple<str, str>> _gap(const str& path, const py::bytes& head) {
    str ext = std::get<1>(py::os::path::splitext(path)).lower();
    const std::string& h = head.raw();
    auto at = [&](size_t i) { return static_cast<int64_t>(static_cast<unsigned char>(h[i])); };
    if (h.compare(0, 8, "PS-X EXE") == 0 && h.size() >= 8) return std::make_tuple(str("PlayStation (PS1)"), str("MIPS R3000"));
    if (h.size() >= 4 && (h.compare(0, 4, "\x80\x37\x12\x40") == 0 || h.compare(0, 4, "\x37\x80\x40\x12") == 0 ||
                          h.compare(0, 4, "\x40\x12\x37\x80") == 0))
        return std::make_tuple(str("Nintendo 64"), str("MIPS R4300"));
    if (h.size() > 0xB3 && at(0xB2) == 0x96 && at(3) == 0xEA) return std::make_tuple(str("Game Boy Advance"), str("ARM7TDMI"));
    if (ext == str(".nds")) return std::make_tuple(str("Nintendo DS"), str("ARM9 / ARM7"));
    if (ext == str(".sfc") || ext == str(".smc")) return std::make_tuple(str("Super Nintendo"), str("65816"));
    if (h.size() >= 20 && h.compare(0, 4, "\x7f" "ELF") == 0) {
        int64_t machine = at(5) == 1 ? at(18) | (at(19) << 8) : (at(18) << 8) | at(19);
        str cpu;
        switch (machine) {
            case 0x08: cpu = str("MIPS"); break;
            case 0x14: cpu = str("PowerPC"); break;
            case 0x15: cpu = str("PowerPC 64"); break;
            case 0x28: cpu = str("ARM"); break;
            case 0x3E: cpu = str("x86-64"); break;
            case 0x03: cpu = str("x86"); break;
            case 0xB7: cpu = str("AArch64"); break;
            case 0x2A: cpu = str("SuperH"); break;
            case 0x17: cpu = str("Cell SPU"); break;
            default: cpu = py::percent("machine 0x%X", machine);
        }
        return std::make_tuple(str("ELF program"), cpu);
    }
    if (h.size() >= 2 && h.compare(0, 2, "PK") == 0 && ext == str(".apk")) return std::make_tuple(str("Android app"), str("Dalvik"));
    return std::nullopt;
}

Described describe(const str& path) {
    py::list<formats::detect::Detected> ids = formats::detect::identify(path);
    formats::detect::Detected found = ids.empty() ? formats::detect::Detected() : ids[0];
    const str& fmt = found.format;
    py::bytes head;
    try {
        head = py::open_rb(path).read(0x200);
    } catch (const py::OSError&) {
    }
    if (fmt.startswith(str("PE")) && found.family == str("windows") && formats::pe::is_dotnet(path)) {   // (the `xna` lane's)
        Described d;
        d.found = str(".NET assembly");
        d.platform = str(".NET (CIL)");
        d.lane = str("xna");
        d.family = str("xna");
        return d;
    }
    bool big = py::os::path::getsize(path) > (int64_t(256) << 20) && fmt.empty();          // (data: not read further)
    std::optional<PlatformInfo> p;
    if (!big) {
        if (!platform_identify()) throw py::ImportError("codebreaker.platforms has no typed port: set reach::platform_identify()");
        p = platform_identify()(path);
    }
    Described d;
    if (p) {
        d.found = !fmt.empty() ? fmt : p->container;
        d.platform = p->title;
        if (p->lane) d.lane = p->name;
        d.family = p->family;
        return d;
    }
    auto gap = _gap(path, head);
    if (gap) {
        d.found = !fmt.empty() ? fmt : std::get<0>(*gap);
        d.platform = py::percent("%s (%s)", std::get<0>(*gap), std::get<1>(*gap));
        d.family = str("gap");
        return d;
    }
    d.found = !fmt.empty() ? fmt : str("unknown");
    d.platform = str("");
    d.family = found.family;
    return d;
}

// ---- finding the programs
str _sha(const str& path) {
    py::hashlib::Hash h = py::hashlib::sha256();
    py::file f = py::open_rb(path);
    for (;;) {
        py::bytes block = f.read(1 << 20);
        if (block.empty()) break;
        h.update(block);
    }
    return h.hexdigest();
}

py::list<str> _unpack(const str& path, int64_t limit, const Log& log) {
    std::vector<std::pair<std::shared_ptr<formats::media::Volume>, formats::media::Entry>> entries;
    try {
        for (const auto& v : formats::media::volumes(path)) {
            for (const formats::media::Entry& e : v->files()) {
                if (!e.is_dir && ends_with_any(e.path.lower(), _PROGRAM_EXTS()) && 0 < e.size && e.size < (int64_t(512) << 20))
                    entries.emplace_back(v, e);
            }
        }
    } catch (const py::Exception& e) {                       // any reader's error, listing too
        log(py::percent("  %s: not opened (%s: %s)", py::os::path::basename(path), str(e.type()), str(std::string(e.what())).slice(0, 80)));
        return py::list<str>();
    }
    std::stable_sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.second.size < b.second.size; });
    str abs = py::os::path::abspath(path);
    str key = py::hashlib::sha1(abs.encode("utf-8", "replace")).hexdigest().slice(0, 12);
    py::list<str> out;
    int64_t n = 0;
    for (const auto& ve : entries) {
        if (n++ >= limit) break;
        std::vector<str> parts = {str("reach"), str("unpacked"), key};
        for (const str& p : py::split(ve.second.path.replace(str("\\"), str("/")), str("/")))
            if (p != str("") && p != str(".") && p != str("..")) parts.push_back(p);
        str dest = state_path(parts);
        if (!py::os::path::isfile(dest)) {
            py::bytes data;
            try {
                data = ve.first->read(ve.second);
            } catch (const py::Exception&) {
                continue;
            }
            py::file f = py::open(dest, str("wb"));
            f.write(data);
            f.close();
        }
        out.append(dest);
    }
    return out;
}

namespace {

// the sub-directories and files of a directory, as the OS lists them
void split_dir(const str& d, py::list<str>& dirs, py::list<str>& files) {
    for (const str& n : py::os::listdir(d)) {
        if (py::os::path::isdir(py::os::path::join(d, n))) dirs.append(n);
        else files.append(n);
    }
}

}  // namespace

py::list<std::tuple<str, str>> programs(const py::list<str>& inputs, int64_t containers, const Log& log_in) {
    Log log = log_in ? log_in : Log(print_log);
    py::list<std::tuple<str, str>> out;

    auto one = [&](const str& path, const str& origin) {
        py::list<formats::detect::Detected> ids = formats::detect::identify(path);
        str found = ids.empty() ? str("") : ids[0].format;
        py::list<str> inner;
        bool container = starts_with_any(found, _CONTAINER_FORMATS);
        if (container) inner = _unpack(path, containers, log);
        if (container && !inner.empty()) {
            for (const str& p : inner) out.append(std::make_tuple(p, path));
        } else {                                              // (a container with no program: counted)
            out.append(std::make_tuple(path, origin));
        }
    };
    auto god = [&](const str& folder) {                      // a Games-on-Demand folder is a container
        try {
            auto v = formats::xbox::volumes::volumes(folder);
            return v.has_value() && !v->empty();
        } catch (const py::Exception&) {
            return false;
        }
    };
    auto hidden_or_skipped = [](const str& x) {
        return x.startswith(str(".")) || x == str("__pycache__") || x == str("node_modules");
    };
    for (const str& root : inputs) {
        if (py::os::path::isfile(root)) {
            one(root, str(""));
            continue;
        }
        auto walk = [&](auto&& self, const str& d) -> void {
            py::list<str> dirs, files;
            split_dir(d, dirs, files);
            if (god(d)) {
                for (const str& inner : _unpack(d, containers, log)) out.append(std::make_tuple(inner, d));
                return;
            }
            for (const str& name : files) one(py::os::path::join(d, name), root);
            for (const str& x : dirs) {
                if (hidden_or_skipped(x)) continue;
                str sub = py::os::path::join(d, x);
                if (!py::os::path::islink(sub)) self(self, sub);
            }
        };
        if (py::os::path::isdir(root)) walk(walk, root);
    }
    return out;
}

// ---- measuring
str _tools() {
    try {
        py::subprocess::RunOptions o;
        o.cwd = py::settings::checkout_root();
        py::subprocess::CompletedProcess r = py::subprocess::run(py::list<str>{str("git"), str("rev-parse"), str("--short"), str("HEAD")}, o);
        str text = r.stdout_ ? r.stdout_->strip() : str("");
        return text.empty() ? str("?") : text;
    } catch (const py::OSError&) {
        return str("?");
    }
}

Judged judge(const str& path, int64_t timeout) {
    double t0 = clock()();
    str stopped;
    // Output to a file, not a pipe, and the whole process tree in one job: a stopped `measure` leaves its worker processes
    // behind otherwise, and they hold a pipe (and the run) open.
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    std::wstring tmpdir = py::subprocess::to_wide(py::tempfile::gettempdir());
    wchar_t name[MAX_PATH];
    if (!GetTempFileNameW(tmpdir.c_str(), L"cbr", 0, name)) py::raise_winerror(GetLastError());
    HANDLE out = CreateFileW(name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, &sa, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, nullptr);
    if (out == INVALID_HANDLE_VALUE) py::raise_winerror(GetLastError());
    struct Closer {
        HANDLE h;
        ~Closer() { CloseHandle(h); }
    } close_out{out};

    std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(measure_argv()(path)));
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = out;
    si.hStdError = out;
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) py::raise_winerror(GetLastError());
    CloseHandle(pi.hThread);
    // _job: a Windows job that holds the process and everything it starts
    HANDLE job = CreateJobObjectW(nullptr, nullptr);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION lim{};
    lim.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(job, JobObjectExtendedLimitInformation, &lim, sizeof lim);
    AssignProcessToJobObject(job, pi.hProcess);
    std::optional<DWORD> code;
    DWORD waited = WaitForSingleObject(pi.hProcess, static_cast<DWORD>(timeout * 1000));
    if (waited == WAIT_OBJECT_0) {
        DWORD c = 0;
        GetExitCodeProcess(pi.hProcess, &c);
        code = c;
    } else {
        stopped = py::percent("timed out after %d s", timeout);
    }
    // _end: stop the process and everything it started
    if (job) {
        TerminateJobObject(job, 1);
        CloseHandle(job);
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    if (code && *code) {
        SetFilePointer(out, 0, nullptr, FILE_BEGIN);
        std::string data;
        char buf[65536];
        DWORD got = 0;
        while (ReadFile(out, buf, sizeof buf, &got, nullptr) && got) data.append(buf, got);
        py::list<str> text = py::splitlines(py::bytes(data).decode("utf-8", "replace").strip());
        stopped = (!text.empty() ? text[-1] : py::percent("exit %d", static_cast<int64_t>(*code))).slice(0, 160);
    }
    auto tw = tally_of(path);
    Judged j;
    j.tally = std::get<0>(tw);
    j.seconds = round1(clock()() - t0);
    j.stopped = !stopped.empty() ? stopped : std::get<1>(tw);
    return j;
}

std::tuple<py::dict<str, int64_t>, str> tally_of(const str& path) {
    py::dict<str, int64_t> tally;
    str stopped;
    try {
        std::optional<str> retail = base::corpus::retail(path);
        if (!retail) throw py::AttributeError("'NoneType' object has no attribute 'lower'");   // (identity(None), as the original fails)
        auto id = verify::reconstruct::identity(*retail);     // (filed as `measure` files it: a .xex under the basefile inside it)
        auto fs = F::open();
        try {
            for (const auto& kv : fs->tally(std::get<1>(id), std::get<0>(id)).items())
                tally.set(std::get<0>(kv), engine::native_engine::cell_int(std::get<1>(kv)));
        } catch (...) {
            fs->close();
            throw;
        }
        fs->close();
    } catch (const py::Exception& e) {
        stopped = py::percent("no tally: %s", str(std::string(e.what())).slice(0, 120));
    }
    return {tally, stopped};
}

py::list<Row> spread(const py::list<Row>& rows, int64_t n, int64_t max_size) {
    std::vector<Row> rs;
    for (const Row& r : rows)
        if (r.size <= max_size) rs.push_back(r);
    std::stable_sort(rs.begin(), rs.end(), [](const Row& a, const Row& b) { return a.size < b.size; });
    py::list<Row> out;
    if (static_cast<int64_t>(rs.size()) <= n) {
        for (const Row& r : rs) out.append(r);
        return out;
    }
    if (n > 1) {
        for (int64_t i = 0; i < n; i++) {
            double at = static_cast<double>(i * (static_cast<int64_t>(rs.size()) - 1)) / static_cast<double>(n - 1);
            out.append(rs[static_cast<size_t>(std::nearbyint(at))]);   // round(): half to even
        }
    } else {
        out.append(rs[0]);
    }
    return out;
}

// ---- the results file
namespace {

Value conv(const py::V& v) {
    if (v.is_none()) return Value();
    if (v.is_bool()) return Value(v.b);
    if (v.is_int()) return Value(v.as_int());
    if (v.is_float()) return Value(v.as_float());
    if (v.is_str()) return Value(str(v.str()));
    if (v.is_dict()) {
        Value d = Value::dict();
        for (const auto& kv : v.items()) d.set(str(kv.first.str()), conv(kv.second));
        return d;
    }
    Value l = Value::list();
    for (const py::V& x : v.seq()) l.push(conv(x));
    return l;
}

Judged judged_from(const py::V& j) {
    Judged out;
    const py::V& t = j.get("tally");
    if (t.is_dict())
        for (const auto& kv : t.items()) out.tally.set(str(kv.first.str()), kv.second.as_int());
    if (j.get("seconds").is_float() || j.get("seconds").is_intlike()) out.seconds = j.get("seconds").as_float();
    if (j.get("stopped").is_str()) out.stopped = str(j.get("stopped").str());
    return out;
}

Row row_from(const py::V& v) {
    Row r;
    for (const auto& kv : v.items()) {
        std::string k = kv.first.str();
        const py::V& x = kv.second;
        if (k == "found" && x.is_str()) r.found = str(x.str());
        else if (k == "platform" && x.is_str()) r.platform = str(x.str());
        else if (k == "lane") { if (x.is_str()) r.lane = str(x.str()); }
        else if (k == "family" && x.is_str()) r.family = str(x.str());
        else if (k == "path" && x.is_str()) r.path = str(x.str());
        else if (k == "origin" && x.is_str()) r.origin = str(x.str());
        else if (k == "size" && x.is_intlike()) r.size = x.as_int();
        else if (k == "sha" && x.is_str()) r.sha = str(x.str());
        else if (k == "picked") r.picked = x.is_bool() ? x.b : x.is_intlike() && x.as_int() != 0;
        else if (k == "judged") { if (x.is_dict() && x.size()) r.judged = judged_from(x); }
        else if (k == "tools") { if (x.is_str()) r.tools = str(x.str()); }
        else r.extra.set(str(k), conv(x));
    }
    return r;
}

Value judged_value(const Judged& j) {
    Value d = Value::dict();
    Value t = Value::dict();
    for (const auto& kv : j.tally.items()) t.set(std::get<0>(kv), Value(std::get<1>(kv)));
    d.set(str("tally"), std::move(t));
    d.set(str("seconds"), Value(j.seconds));
    d.set(str("stopped"), Value(j.stopped));
    return d;
}

}  // namespace

Value to_value(const Row& r) {
    Value v = Value::dict();
    v.set(str("found"), Value(r.found));
    v.set(str("platform"), Value(r.platform));
    v.set(str("lane"), r.lane ? Value(*r.lane) : Value());
    v.set(str("family"), Value(r.family));
    v.set(str("path"), Value(r.path));
    v.set(str("origin"), Value(r.origin));
    v.set(str("size"), Value(r.size));
    v.set(str("sha"), Value(r.sha));
    if (r.picked) v.set(str("picked"), Value(true));
    if (r.judged) v.set(str("judged"), judged_value(*r.judged));
    if (r.tools) v.set(str("tools"), Value(*r.tools));
    for (size_t i = 0; i < r.extra.keys.size(); i++) v.set(r.extra.keys[i], r.extra.items[i]);
    return v;
}

py::list<Row> read_results(const str& path) {
    std::string text = py::open_rb(path).read().decode("utf-8").utf8();
    py::V data = py::json_parse(text);
    py::list<Row> out;
    const py::V& progs = data.get("programs");
    if (progs.is_seq())
        for (const py::V& p : progs.seq()) out.append(row_from(p));
    return out;
}

std::tuple<py::list<Row>, str> run(const py::list<str>& inputs, int64_t per_platform, bool do_judge, int64_t timeout,
                                   int64_t containers, const str& name, bool again, int64_t max_size, const Log& log_in) {
    Log log = log_in ? log_in : Log(print_log);
    str store = state_path({str("reach"), name + str(".json")});
    std::vector<Row> old;                                     // {sha: row}, in the file's order
    if (py::os::path::isfile(store)) {
        for (const Row& r : read_results(store)) {
            bool dup = false;
            for (Row& o : old)
                if (o.sha == r.sha) { o = r; dup = true; }
            if (!dup) old.push_back(r);
        }
    }
    str tools = _tools();
    py::list<std::tuple<str, str>> found = programs(inputs, containers, log);
    log(py::percent("%d files found", found.size()));
    std::vector<Row> rows;
    py::set<str> seen;
    for (const auto& po : found) {
        const str& path = std::get<0>(po);
        Row row;
        try {
            Described d = describe(path);
            row.found = d.found;
            row.platform = d.platform;
            row.lane = d.lane;
            row.family = d.family;
            row.path = path;
            row.origin = std::get<1>(po);
            row.size = py::os::path::getsize(path);
            // only programs are hashed (a game library is mostly data: never read whole)
            row.sha = (!row.platform.empty() || row.family == str("gap")) ? _sha(path) : str("path:") + path;
        } catch (const py::OSError&) {
            continue;
        }
        if (seen.contains(row.sha)) continue;
        seen.add(row.sha);
        rows.push_back(row);
    }
    auto kept_rows = [&]() {
        py::list<Row> out;
        for (const Row& r : rows) out.append(r);
        for (const Row& o : old)
            if (!seen.contains(o.sha)) out.append(o);
        return out;
    };
    auto save = [&]() {                                       // after every program: a stopped run keeps its results
        Value doc = Value::dict();
        {
            std::time_t t = std::time(nullptr);
            std::tm tmv{};
            localtime_s(&tmv, &t);
            char buf[64];
            std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M %p", &tmv);
            doc.set(str("when"), Value(str(buf)));
        }
        Value ins = Value::list();
        for (const str& i : inputs) ins.push(Value(py::os::path::abspath(i)));
        doc.set(str("inputs"), std::move(ins));
        doc.set(str("tools"), Value(tools));
        Value progs = Value::list();
        for (const Row& r : kept_rows()) progs.push(to_value(r));
        doc.set(str("programs"), std::move(progs));
        py::file f = py::open(store, str("w"), str("utf-8"));
        f.write(formats::inspect::dumps(doc, 1));
        f.close();
    };

    // which get judged: programs of a platform with a lane, smallest first, per_platform of each
    std::vector<std::pair<str, std::vector<size_t>>> by_platform;      // platform -> indexes into rows, in first-seen order
    for (size_t i = 0; i < rows.size(); i++) {
        const Row& r = rows[i];
        if (r.lane && !r.lane->empty() && !_SKIP_FAMILIES.contains(r.family)) {
            auto it = std::find_if(by_platform.begin(), by_platform.end(), [&](const auto& p) { return p.first == r.platform; });
            if (it == by_platform.end()) {
                by_platform.emplace_back(r.platform, std::vector<size_t>());
                it = by_platform.end() - 1;
            }
            it->second.push_back(i);
        }
    }
    std::stable_sort(by_platform.begin(), by_platform.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    for (const auto& pl : by_platform) {
        py::list<Row> group;
        for (size_t i : pl.second) group.append(rows[i]);
        for (const Row& pick : spread(group, per_platform, max_size)) {
            Row* r = nullptr;
            for (Row& cand : rows)
                if (cand.sha == pick.sha) { r = &cand; break; }
            r->picked = true;
            const Row* prev = nullptr;
            for (const Row& o : old)
                if (o.sha == r->sha) { prev = &o; break; }
            if (prev && prev->tools && *prev->tools == tools && prev->judged && !again) {
                r->judged = prev->judged;
                r->tools = tools;
                continue;
            }
            if (do_judge) {
                log(py::percent("judging %s (%s, %d KB)", py::os::path::basename(r->path), pl.first, r->size / 1024));
                r->judged = judge(r->path, timeout);
                r->tools = tools;
                const Judged& j = *r->judged;
                py::list<str> parts;
                for (const auto& kv : j.tally.items())
                    if (std::get<1>(kv)) parts.append(py::percent("%s %d", std::get<0>(kv), std::get<1>(kv)));
                str shown = py::join(str(", "), parts);
                log(py::percent("  %s%s", shown.empty() ? str("nothing recorded") : shown,
                                !j.stopped.empty() ? str("; ") + j.stopped : str("")));
                save();
            }
        }
    }
    save();
    str text = report(kept_rows());
    {
        py::file f = py::open(state_path({str("reach"), name + str(".md")}), str("w"), str("utf-8"));
        f.write(text);
        f.close();
    }
    py::list<Row> result;
    for (const Row& r : rows) result.append(r);
    return {result, text};
}

// ---- the report
str report(const py::list<Row>& rows) {
    std::vector<const Row*> progs, others;
    for (const Row& r : rows) (!r.platform.empty() || r.family == str("gap") ? progs : others).push_back(&r);
    std::stable_sort(progs.begin(), progs.end(), [](const Row* a, const Row* b) {
        bool an = !a->lane, bn = !b->lane;
        return an != bn ? !an && bn : a->platform.u32() < b->platform.u32();
    });
    std::vector<std::pair<str, std::vector<const Row*>>> plats;
    for (const Row* r : progs) {
        auto it = std::find_if(plats.begin(), plats.end(), [&](const auto& p) { return p.first == r->platform; });
        if (it == plats.end()) {
            plats.emplace_back(r->platform, std::vector<const Row*>());
            it = plats.end() - 1;
        }
        it->second.push_back(r);
    }
    py::list<str> lines = {
        str("# Reach"), str(""), str("What CodeBreaker made of every program it was given (`python -m codebreaker reach`)."),
        str("Functions and percentages are those of the judged programs that finished; a program stopped at"),
        str("its time limit is counted under Stopped only."), str(""),
        str("| Platform | Programs | Lane | Judged | Functions | Verified | Weak | Unverified | Stopped |"),
        str("|---|---|---|---|---|---|---|---|---|")};
    for (const auto& pl : plats) {
        std::vector<const Row*> judged;
        for (const Row* r : pl.second)
            if (r->judged) judged.push_back(r);
        std::vector<std::pair<str, int64_t>> t;                // Counter
        for (const Row* r : judged) {                          // (a program stopped at its time limit: its functions never
            if (r->judged->stopped.startswith(str("timed out"))) continue;   // reached are not failures; counted as stopped)
            for (const auto& kv : r->judged->tally.items()) {
                auto it = std::find_if(t.begin(), t.end(), [&](const auto& p) { return p.first == std::get<0>(kv); });
                if (it == t.end()) t.emplace_back(std::get<0>(kv), std::get<1>(kv));
                else it->second += std::get<1>(kv);
            }
        }
        int64_t total = 0;
        for (const auto& e : t) total += e.second;
        auto count = [&](const char* k) {
            for (const auto& e : t)
                if (e.first == str(k)) return e.second;
            return int64_t(0);
        };
        auto pct = [&](int64_t n) { return total ? py::percent("%d (%d%%)", n, py::floordiv(100 * n, total)) : str("-"); };
        int64_t stopped = 0;
        for (const Row* r : judged)
            if (!r->judged->stopped.empty()) stopped++;
        lines.append(py::percent("| %s | %d | %s | %d | %s | %s | %s | %s | %s |", pl.first, static_cast<int64_t>(pl.second.size()),
                                 pl.second[0]->lane && !pl.second[0]->lane->empty() ? *pl.second[0]->lane : str("**none yet**"),
                                 static_cast<int64_t>(judged.size()), total ? py::to_str(total) : str("-"), pct(count("verified")),
                                 pct(count("weak")), pct(count("unverified") + count("no-compile")),
                                 stopped ? py::to_str(stopped) : str("")));
    }
    std::vector<std::pair<str, int64_t>> other;                // Counter(r["found"]) in the order met
    for (const Row* r : others) {
        auto it = std::find_if(other.begin(), other.end(), [&](const auto& p) { return p.first == r->found; });
        if (it == other.end()) other.emplace_back(r->found, 1);
        else it->second++;
    }
    int64_t other_total = 0;
    for (const auto& e : other) other_total += e.second;
    std::stable_sort(other.begin(), other.end(), [](const auto& a, const auto& b) { return a.second > b.second; });   // most_common
    py::list<str> common;
    int64_t n = 0;
    for (const auto& e : other) {
        if (n++ >= 8) break;
        common.append(py::percent("%s %d", e.first, e.second));
    }
    lines.append(str(""));
    lines.append(py::percent("Not programs, or not recognised: %d files (%s).", other_total, py::join(str(", "), common)));
    std::vector<const Row*> stops;
    for (const Row& r : rows)
        if (r.judged && !r.judged->stopped.empty()) stops.push_back(&r);
    if (!stops.empty()) {
        lines.append(str(""));
        lines.append(str("## Where judging stopped"));
        lines.append(str(""));
        for (const Row* r : stops)
            lines.append(py::percent("- %s (%s): %s", py::os::path::basename(r->path), r->platform, r->judged->stopped));
    }
    return py::join(str("\n"), lines) + str("\n");
}

// ---- the command
const char* DOC =
    "reach.py -- throw anything at CodeBreaker and see how far each program gets.\n"
    "\n"
    "    python -m codebreaker reach <file or folder> ... [--per-platform N] [--judge] [--timeout S]\n"
    "                                                     [--containers N] [--name RUN] [--again]\n"
    "    python -m codebreaker reach --report [RUN]       the last results again, without running anything\n"
    "\n"
    "Every file given -- and every file in every folder given, and the programs inside every disc\n"
    "image, Xbox 360 package or Games-on-Demand folder found -- is taken through these steps, as far\n"
    "as it goes:\n"
    "\n"
    "    found        what the file is (formats/detect.py), or \"unknown\"\n"
    "    platform     the platform it belongs to (codebreaker/platforms), or the platform CodeBreaker\n"
    "                 has no lane for yet, named (\"Game Boy Advance (ARM7)\": a gap, counted)\n"
    "    judged       with --judge: the platform's lane reconstructs it and runs its functions beside\n"
    "                 the original's (`measure`, in a process of its own, stopped after --timeout\n"
    "                 seconds); the function store's tally of what came of it\n"
    "\n"
    "Load stays low on purpose: one program at a time, at most --per-platform programs of each\n"
    "platform (default 5: spread evenly across their sizes, from the smallest to the largest under\n"
    "--max-size KB, default 4096), at most --containers programs taken out of each container\n"
    "(default 20). Results are kept in state/reach/<run>.json by each file's content hash and the\n"
    "tools' revision, so a program already measured with the same tools is not measured again\n"
    "(--again measures it anyway). The report is written beside them, <run>.md.\n";

namespace {

// argparse's text for this one parser (COLUMNS=80)
const char* USAGE =
    "usage: codebreaker reach [-h] [--per-platform PER_PLATFORM] [--judge]\n"
    "                         [--timeout TIMEOUT] [--containers CONTAINERS]\n"
    "                         [--name NAME] [--again] [--max-size MAX_SIZE]\n"
    "                         [--report [REPORT]]\n"
    "                         [inputs ...]\n";
const char* HELP_BODY =
    "\n"
    "reach.py -- throw anything at CodeBreaker and see how far each program gets.\n"
    "\n"
    "positional arguments:\n"
    "  inputs\n"
    "\n"
    "options:\n"
    "  -h, --help            show this help message and exit\n"
    "  --per-platform PER_PLATFORM\n"
    "  --judge\n"
    "  --timeout TIMEOUT\n"
    "  --containers CONTAINERS\n"
    "  --name NAME\n"
    "  --again\n"
    "  --max-size MAX_SIZE   KB: larger programs are not judged\n"
    "  --report [REPORT]\n";

[[noreturn]] void usage_error(const str& message) {
    std::string text = std::string(USAGE) + "codebreaker reach: error: " + message.utf8() + "\n";
    std::fwrite(text.data(), 1, text.size(), stderr);
    std::fflush(stderr);
    throw py::SystemExit(2);
}

struct Args {
    py::list<str> inputs;
    int64_t per_platform = 5, timeout = 300, containers = 20, max_size = 4096;
    bool judge = false, again = false;
    str name = str("reach");
    std::optional<str> report;
};

struct OptionSpec {
    const char* name;
    enum Kind { Help, Flag, Int, Text, Optional } kind;
};
const OptionSpec OPTIONS[] = {{"--help", OptionSpec::Help},       {"--per-platform", OptionSpec::Int}, {"--judge", OptionSpec::Flag},
                              {"--timeout", OptionSpec::Int},     {"--containers", OptionSpec::Int},   {"--name", OptionSpec::Text},
                              {"--again", OptionSpec::Flag},      {"--max-size", OptionSpec::Int},     {"--report", OptionSpec::Optional}};

bool negative_number(const std::string& s) {   // ^-\d+$|^-\d*\.\d+$
    static const py::re::Pattern p = py::re::compile("^-\\d+$|^-\\d*\\.\\d+$");
    return p.search(str(s)).has_value();
}

// how argparse reads one argument string: 'A' (a value or positional), or an option (index into OPTIONS, an explicit value) or
// 'O' (an option it does not know)
struct Classified {
    char kind = 'A';
    int option = -1;
    std::optional<str> explicit_value;
};

Classified classify(const str& arg) {
    Classified c;
    std::string s = arg.utf8();
    if (s.empty() || s[0] != '-') return c;
    auto exact = [&](const std::string& name) -> int {
        if (name == "-h") return 0;
        for (size_t i = 0; i < sizeof OPTIONS / sizeof OPTIONS[0]; i++)
            if (name == OPTIONS[i].name) return static_cast<int>(i);
        return -1;
    };
    int e = exact(s);
    if (e >= 0) { c.kind = 'O'; c.option = e; return c; }
    if (s.size() == 1) return c;
    size_t eq = s.find('=');
    if (eq != std::string::npos) {
        int x = exact(s.substr(0, eq));
        if (x >= 0) { c.kind = 'O'; c.option = x; c.explicit_value = str(s.substr(eq + 1)); return c; }
    }
    // abbreviations of the long options (and -h given more letters)
    if (s.compare(0, 2, "--") == 0) {
        std::string prefix = eq != std::string::npos ? s.substr(0, eq) : s;
        std::vector<int> hits;
        for (size_t i = 0; i < sizeof OPTIONS / sizeof OPTIONS[0]; i++)
            if (std::string(OPTIONS[i].name).compare(0, prefix.size(), prefix) == 0) hits.push_back(static_cast<int>(i));
        if (hits.size() > 1) {
            std::string names;
            for (size_t k = 0; k < hits.size(); k++) names += (k ? ", " : "") + std::string(OPTIONS[hits[k]].name);
            usage_error(str(std::string("ambiguous option: ") + prefix + " could match " + names));
        }
        if (hits.size() == 1) {
            c.kind = 'O';
            c.option = hits[0];
            if (eq != std::string::npos) c.explicit_value = str(s.substr(eq + 1));
            return c;
        }
    } else if (s.size() > 2 && s.compare(0, 2, "-h") == 0) {                     // -hx: -h with an argument it does not take
        c.kind = 'O';
        c.option = 0;
        c.explicit_value = str(s.substr(2));
        return c;
    }
    if (negative_number(s)) return c;
    if (s.find(' ') != std::string::npos) return c;
    c.kind = 'X';                                                                  // an option it does not know
    return c;
}

Args parse_args(const py::list<str>& argv) {
    Args a;
    std::vector<str> extras;
    bool inputs_done = false, in_run = false, after_dd = false;
    auto positional = [&](const str& tok) {
        if (!inputs_done) {
            a.inputs.append(tok);
            in_run = true;
        } else {
            extras.push_back(tok);
        }
    };
    auto end_run = [&]() {
        if (in_run) inputs_done = true;
        in_run = false;
    };
    for (int64_t i = 0; i < argv.size(); i++) {
        const str& tok = argv[i];
        if (after_dd) { positional(tok); continue; }
        if (tok == str("--")) { after_dd = true; continue; }
        Classified c = classify(tok);
        if (c.kind == 'A') { positional(tok); continue; }
        end_run();
        if (c.kind == 'X') { extras.push_back(tok); continue; }
        const OptionSpec& spec = OPTIONS[c.option];
        std::string shown = spec.kind == OptionSpec::Help ? "-h/--help" : spec.name;
        if (spec.kind == OptionSpec::Help) {
            std::string text = std::string(USAGE) + HELP_BODY;
            std::fwrite(text.data(), 1, text.size(), stdout);
            std::fflush(stdout);
            throw py::SystemExit(0);
        }
        if (spec.kind == OptionSpec::Flag) {
            if (c.explicit_value) usage_error(py::percent("argument %s: ignored explicit argument %s", str(shown), py::repr(*c.explicit_value)));
            if (std::string(spec.name) == "--judge") a.judge = true;
            else a.again = true;
            continue;
        }
        std::optional<str> value = c.explicit_value;
        if (!value) {
            if (i + 1 < argv.size() && !after_dd && argv[i + 1] != str("--") && classify(argv[i + 1]).kind == 'A') {
                value = argv[i + 1];
                i++;
            } else if (spec.kind != OptionSpec::Optional) {
                usage_error(py::percent("argument %s: expected one argument", str(shown)));
            }
        }
        if (spec.kind == OptionSpec::Optional) {
            a.report = value ? *value : str("reach");
            continue;
        }
        if (spec.kind == OptionSpec::Text) {
            a.name = *value;
            continue;
        }
        int64_t n;
        try {
            n = py::int_parse(*value, 10);
        } catch (const py::ValueError&) {
            usage_error(py::percent("argument %s: invalid int value: %s", str(shown), py::repr(*value)));
        }
        std::string name = spec.name;
        if (name == "--per-platform") a.per_platform = n;
        else if (name == "--timeout") a.timeout = n;
        else if (name == "--containers") a.containers = n;
        else a.max_size = n;
    }
    if (!extras.empty()) {
        py::list<str> e;
        for (const str& x : extras) e.append(x);
        usage_error(str("unrecognized arguments: ") + py::join(str(" "), e));
    }
    return a;
}

}  // namespace

int64_t main(const py::list<str>& argv) {
    Args a = parse_args(argv);
    if (a.report && !a.report->empty()) {
        py::file f = py::open(state_path({str("reach"), *a.report + str(".md")}), str("r"), str("utf-8"));
        py::print(f.read_text());
        return 0;
    }
    if (a.inputs.empty()) {
        std::string text = std::string(USAGE) + HELP_BODY;
        std::fwrite(text.data(), 1, text.size(), stdout);
        std::fflush(stdout);
        return 2;
    }
    auto result = run(a.inputs, a.per_platform, a.judge, a.timeout, a.containers, a.name, a.again, a.max_size * 1024);
    py::print(std::get<1>(result));
    return 0;
}

}  // namespace codebreaker::coverage::reach
