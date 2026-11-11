// codebreaker/jobs/pool.cpp -- typed C++17 port of codebreaker.jobs.pool (see pool.hpp).

#include "codebreaker/jobs/pool.hpp"

#include <process.h>

#include <cmath>

#include "json.hpp"
#include "py_json.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::jobs::pool {

bool processes_safe() { return true; }

int64_t default_jobs() { return base::parallel::workers(); }

namespace detail {

std::vector<char> run_tasks(int64_t jobs, size_t n, const std::function<void()>& init, bool nice,
                            const std::function<void(size_t)>& task) {
    std::vector<char> ran(n, 0);
    if (n == 0) return ran;
    size_t nthreads = std::min<size_t>(static_cast<size_t>(std::max<int64_t>(1, jobs)), n);
    std::atomic<size_t> next(0);
    std::atomic<bool> broken(false);
    auto worker = [&] {
        if (nice) SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);   // (the thread, not the process)
        if (init) {
            try {
                init();
            } catch (...) {
                broken = true;   // a broken pool: what has not started does not run
                return;
            }
        }
        for (;;) {
            if (broken.load()) return;
            size_t i = next.fetch_add(1);
            if (i >= n) return;
            task(i);
            ran[i] = 1;
        }
    };
    std::vector<std::thread> ts;
    for (size_t t = 0; t < nthreads; ++t) ts.emplace_back(worker);
    for (std::thread& t : ts) t.join();
    return ran;
}

}  // namespace detail

// ---------------------------------------------------------------------------------------------
// JSON values: py::V (json_parse's) <-> Json (the typed one)
namespace {

// json.loads: l2's reader (value.cc) raises its own PyError; a ValueError of it is the runtime's ValueError here
py::V parse_json(const std::string& text) {
    try {
        return py::json_parse(text);
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") throw py::ValueError(e.what());
        throw py::Exception(e.what());
    }
}

Json from_v(const py::V& v) {
    Json j;
    switch (v.k) {
        case py::K::None: j.kind = Json::None; break;
        case py::K::Bool: j.kind = Json::Bool; j.b = v.b; break;
        case py::K::Int:
            if (v.u64) throw py::OverflowError("int too large for int64");
            j.kind = Json::Int;
            j.i = v.i;
            break;
        case py::K::Float: j.kind = Json::Float; j.d = v.f; break;
        case py::K::Str: j.kind = Json::Str; j.s = py::str(v.str()); break;
        case py::K::List:
        case py::K::Tuple:
            j.kind = Json::List;
            for (const py::V& x : v.seq()) j.list.push_back(from_v(x));
            break;
        case py::K::Dict:
            j.kind = Json::Dict;
            for (const auto& kv : v.items()) j.dict.set(py::str(kv.first.str()), from_v(kv.second));
            break;
        default: throw py::TypeError("not a JSON value");
    }
    return j;
}

void dump(std::string& o, const Json& j) {
    switch (j.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += j.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(j.i); break;
        case Json::Float:
            if (std::isnan(j.d)) o += "NaN";
            else if (std::isinf(j.d)) o += j.d > 0 ? "Infinity" : "-Infinity";
            else o += py::to_str(j.d).utf8();
            break;
        case Json::Str: py::json::put(o, j.s); break;
        case Json::List: {
            o += '[';
            for (size_t i = 0; i < j.list.size(); ++i) {
                if (i) o += ", ";
                dump(o, j.list[i]);
            }
            o += ']';
            break;
        }
        case Json::Dict: {
            o += '{';
            bool first = true;
            for (const auto& kv : j.dict.items()) {
                if (!first) o += ", ";
                py::json::put(o, std::get<0>(kv));
                o += ": ";
                dump(o, std::get<1>(kv));
                first = false;
            }
            o += '}';
            break;
        }
    }
}

Result as_result(const Json& j, const char* what) {
    if (j.kind != Json::Dict) throw py::TypeError(std::string(what) + " is not an object");
    Result r;
    for (const auto& kv : j.dict.items()) r.set(std::get<0>(kv), std::get<1>(kv));
    return r;
}

Json as_json(const Result& r) {
    Json j;
    j.kind = Json::Dict;
    for (const auto& kv : r.items()) j.dict.set(std::get<0>(kv), std::get<1>(kv));
    return j;
}

py::dict<py::str, Result> as_results(const Json& j, const char* what) {
    if (j.kind != Json::Dict) throw py::TypeError(std::string(what) + " is not an object");
    py::dict<py::str, Result> out;
    for (const auto& kv : j.dict.items()) out.set(std::get<0>(kv), as_result(std::get<1>(kv), what));
    return out;
}

Json str_json(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}

Json int_json(int64_t i) {
    Json j;
    j.kind = Json::Int;
    j.i = i;
    return j;
}

}  // namespace

// ---------------------------------------------------------------------------------------------
// the state file
State _load_state(const py::str& path) {
    State st;
    try {
        py::file f = py::open(path);
        py::str text = f.read_text();
        f.close();
        py::V v = parse_json(text.utf8());
        if (!v.is_dict()) throw py::AttributeError("object has no attribute 'setdefault'");
        for (const auto& kv : v.items()) st.top.set(py::str(kv.first.str()), from_v(kv.second));
        Json empty;
        empty.kind = Json::Dict;
        st.top.setdefault(py::str("done"), empty);
        st.top.setdefault(py::str("failed"), empty);
        st.done = as_results(st.top.at(py::str("done")), "done");
        st.failed = as_results(st.top.at(py::str("failed")), "failed");
        return st;
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    State fresh;
    Json empty;
    empty.kind = Json::Dict;
    fresh.top.set(py::str("done"), empty);
    fresh.top.set(py::str("failed"), empty);
    return fresh;
}

void _save_state(const py::str& path, const State& st) {
    py::str tmp = py::str(path.utf8() + ".tmp." + std::to_string(_getpid()));
    std::string o = "{";
    bool first = true;
    for (const auto& kv : st.top.items()) {
        if (!first) o += ", ";
        first = false;
        py::json::put(o, std::get<0>(kv));
        o += ": ";
        const py::str& k = std::get<0>(kv);
        if (k == py::str("done") || k == py::str("failed")) {
            const py::dict<py::str, Result>& d = k == py::str("done") ? st.done : st.failed;
            Json whole;
            whole.kind = Json::Dict;
            for (const auto& e : d.items()) whole.dict.set(std::get<0>(e), as_json(std::get<1>(e)));
            dump(o, whole);
        } else {
            dump(o, std::get<1>(kv));
        }
    }
    o += "}";
    {
        py::file f = py::open(tmp, py::str("w"));
        f.write(py::str(o));
        f.close();
    }
    py::os::replace(tmp, path);
}

std::optional<Result> _last_json(const std::optional<py::str>& text) {
    py::str t = text ? text->strip() : py::str("");
    py::list<py::str> lines = py::splitlines(t);
    for (int64_t i = lines.size() - 1; i >= 0; --i) {
        const py::str& ln = lines[i];
        if (ln.startswith(py::str("{"))) {
            try {
                return as_result(from_v(parse_json(ln.utf8())), "line");
            } catch (const py::ValueError&) {
                return std::nullopt;
            }
        }
    }
    return std::nullopt;
}

Result with_tries(const Result& res, int64_t tries) {
    Result out = res.copy();
    out.set(py::str("tries"), int_json(tries));
    return out;
}

// ---------------------------------------------------------------------------------------------
// one subprocess, with a wall-clock limit
namespace {

struct Captured {
    bool timed_out = false;
    DWORD code = 0;
    std::string out, err;
};

// subprocess.run(argv, capture_output=True, timeout=timeout, cwd=cwd, env=env): the child's stdin is the parent's
Captured run_timed(const py::list<py::str>& argv, int64_t timeout, const std::optional<py::str>& cwd,
                   const std::optional<py::dict<py::str, py::str>>& env) {
    py::subprocess::RunOptions o;
    o.text = false;
    o.cwd = cwd;
    o.env = env;
    o.timeout = timeout;
    Captured c;
    try {
        py::subprocess::CompletedProcess p = py::subprocess::run(argv, o);
        c.code = static_cast<DWORD>(p.returncode);
        c.out = p.stdout_bytes ? p.stdout_bytes->raw() : std::string();
        c.err = p.stderr_bytes ? p.stderr_bytes->raw() : std::string();
    } catch (const py::subprocess::TimeoutExpired&) {
        c.timed_out = true;
    }
    return c;
}

// text mode as Python's on a Windows locale: cp1252, strict, universal newlines
py::str text_of(const std::string& raw) {
    py::subprocess::RunOptions o;
    return py::subprocess::detail_::to_text(raw, o);
}

}  // namespace

Result run_one_process(const std::function<py::list<py::str>()>& argv_for, int64_t timeout, const std::optional<py::str>& cwd,
                       const std::optional<py::dict<py::str, py::str>>& env) {
    Result res;
    try {
        Captured c = run_timed(argv_for(), timeout, cwd, env);
        if (c.timed_out) {
            res.set(py::str("error"), str_json(py::str("timeout")));
            res.set(py::str("timeout"), int_json(timeout));
            return res;
        }
        py::str out = text_of(c.out);
        py::str err = text_of(c.err);
        std::optional<Result> got = _last_json(out);
        if (!got) {
            py::str s = err.strip();
            const std::u32string& u = s.u32();
            std::u32string tail = u.size() > 300 ? u.substr(u.size() - 300) : u;
            res.set(py::str("error"), str_json(py::str("no-output")));
            res.set(py::str("rc"), int_json(static_cast<int64_t>(c.code)));
            res.set(py::str("stderr"), str_json(py::str(tail)));
            return res;
        }
        return *got;
    } catch (const py::Exception& e) {
        Result r;
        py::str detail(std::string(e.what()));
        const std::u32string& u = detail.u32();
        r.set(py::str("error"), str_json(py::str(std::string(e.type()))));
        r.set(py::str("detail"), str_json(py::str(u.size() > 200 ? u.substr(0, 200) : u)));
        return r;
    }
}

}  // namespace codebreaker::jobs::pool
