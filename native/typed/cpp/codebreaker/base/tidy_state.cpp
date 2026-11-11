// codebreaker/base/tidy_state.cpp -- typed C++ port of codebreaker.base.tidy_state.

#include "codebreaker/base/tidy_state.hpp"

#include <filesystem>
#include <system_error>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/runs.hpp"
#include "py_glob.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::tidy_state {

const char* const DOC = R"DOC(tidy_state.py -- how big state/ is, part by part, and what of it is stale, gone.

    python -m codebreaker tidy-state              the sizes, and the stale removed
    python -m codebreaker tidy-state --dry        the sizes, and what would be removed
    python -m codebreaker tidy-state --stray      also remove what no part of CodeBreaker writes
    python -m codebreaker tidy-state --keep-builds    leave Converted/*/build/ alone

state/ is CodeBreaker's to fill, and once filled 70 GB: fingerprints under comparer revisions
long gone, millions of cache files, the disk and CD copies of killed boot runs. What fills it now
looks after itself -- the cache (base/cache.py) holds its generation of now under a cap, and every
run that copies a disk works in a run folder deleted when it ends (base/runs.py). This is the
look over all of it, and the clean-up of what came before or slipped through. The rules:

    cache          older generations of every kind dropped, the cap held (cache --prune)
    old caches     the fingerprints' own file of before (state/fingerprints.sqlite), now in the cache
    runs           run folders older than a day (killed runs), of every tool that has them, and
                   those of before run folders were (phase3/walk_*, setup16_*, t_*; vxdboot/run_*;
                   record16's memory dumps)
    kept disks     an installed disk a Setup walk kept to compare (record16/walk/<tag>/hd.img),
                   a week old
    builds         the object files of a Converted/<name>/build/ not built for two weeks (the
                   programs and their symbols stay; the next build compiles them again)
    stray          anything at the top of state/ no part of CodeBreaker writes -- listed, removed
                   only with --stray (it may be someone's hand work)

What a run of the code would make again is all that goes; the stores (results.sqlite,
functions.sqlite), hand work (java/overlays, xna/overlays) and the tools' outputs are left.
)DOC";

const int64_t DAY = 24 * 3600;
const py::list<py::str> RUN_TOOLS = py::list<py::str>{py::str("phase3"), py::str("vxdboot"), py::str("record16")};   // a tuple
const py::list<py::str> NAMED_ELSEWHERE = py::list<py::str>{py::str("msc6"), py::str("msvc152"), py::str("win95"), py::str("products"), py::str("golden")};   // a tuple
const py::list<py::str> OLD_CACHES = py::list<py::str>{py::str("fingerprints.sqlite")};   // a tuple

using py::glob::glob;   // glob.glob

namespace {

// x in xs
bool in_(const py::str& x, const py::list<py::str>& xs) {
    for (const py::str& y : xs)
        if (y == x) return true;
    return false;
}

// os.path.getmtime(p) older than `age` seconds before `now` (False where p cannot be read)
bool old_(const py::str& p, double age, double now) {
    try {
        return now - py::os::path::getmtime(p) > age;
    } catch (const py::OSError&) {
        return false;
    }
}

}  // namespace

int64_t _size(const py::str& path) {
    if (py::os::path::isfile(path)) {
        return py::os::path::getsize(path);
    }
    int64_t total = 0;
    for (const auto& [d, _dirs, names] : py::os::walk(path)) {
        for (const py::str& n : names) {
            try {
                total = py::checked_add(total, py::os::path::getsize(py::os::path::join(d, n)));
            } catch (const py::OSError&) {
            }
        }
    }
    return total;
}

py::str _gb(int64_t n) {
    return n >= INT64_C(1) << 30 ? py::percent("%.2f GB", py::truediv(n, static_cast<double>(INT64_C(1) << 30))) : py::percent("%.1f MB", py::truediv(n, static_cast<double>(INT64_C(1) << 20)));
}

py::set<py::str> known() {
    py::set<py::str> names = py::set<py::str>{py::str("cache")};
    for (const py::str& n : NAMED_ELSEWHERE) names.add(n);
    py::re::Pattern pattern = py::re::compile(py::str("(?:\\bstate\\(\\s*|\\bruns\\.\\w+\\(\\s*|[\"']state[\"']\\s*,\\s*)[\"']([^\"']+)[\"']"));
    const py::str root = py::settings::checkout_root();
    for (const py::str& top : py::list<py::str>{py::str("codebreaker"), py::str("tools")}) {
        for (const auto& [d, dirs, files] : py::os::walk(py::os::path::join(root, top))) {
            py::list<py::str> keep;
            for (const py::str& x : dirs) {
                if (x != py::str("__pycache__")) keep.append(x);
            }
            dirs.set_slice(py::none, py::none, keep);
            for (const py::str& f : files) {
                if (f.endswith(py::str(".py"))) {
                    py::file fh = py::open(py::os::path::join(d, f), py::str("r"), std::optional<py::str>(py::str("utf-8")), std::optional<py::str>(py::str("replace")));
                    for (const py::str& m : pattern.findall(fh.read_text())) {
                        names.add(py::split(m, py::str("/"))[0]);
                    }
                }
            }
        }
    }
    return names;
}

bool _is_known(const py::str& name, const py::set<py::str>& names) {
    py::str base = py::re::sub(py::str("-(wal|shm|journal)$"), py::str(""), name);
    return names.contains(base) || in_(base, OLD_CACHES);
}

py::list<std::tuple<py::str, int64_t, py::str>> stale(const std::optional<double>& now_) {
    const double now = (now_ && *now_ != 0.0) ? *now_ : py::time::time();
    const py::str state = py::settings::get_str(py::str("state"));
    py::list<std::tuple<py::str, int64_t, py::str>> out;

    for (const py::str& name : OLD_CACHES) {
        for (const py::str& suffix : py::list<py::str>{py::str(""), py::str("-wal"), py::str("-shm")}) {
            py::str p = py::os::path::join(state, name + suffix);
            if (py::os::path::exists(p)) {
                out.append(std::make_tuple(p, _size(p), py::str("old caches")));
            }
        }
    }
    for (const py::str& tool : RUN_TOOLS) {
        for (const py::str& p : glob(py::os::path::join(state, tool, py::str("runs"), py::str("*")))) {
            if (old_(p, static_cast<double>(runs::AGE), now)) {
                out.append(std::make_tuple(p, _size(p), py::str("runs")));
            }
        }
    }
    for (const py::str& pattern : py::list<py::str>{py::str("phase3/walk_*"), py::str("phase3/setup16_*"), py::str("phase3/t_*"), py::str("vxdboot/run_*"),
                                                  py::str("record16/rec_mem_*.bin"), py::str("record16/rec_hit_*.bin")}) {
        py::str joined = state;
        for (const py::str& part : py::split(pattern, py::str("/"))) joined = py::os::path::join(joined, part);
        for (const py::str& p : glob(joined)) {
            if (old_(p, static_cast<double>(DAY), now)) {
                out.append(std::make_tuple(p, _size(p), py::str("runs")));
            }
        }
    }
    for (const py::str& p : glob(py::os::path::join(state, py::str("record16"), py::str("walk"), py::str("*"), py::str("hd.img")))) {
        if (old_(p, 7.0 * static_cast<double>(DAY), now)) {
            out.append(std::make_tuple(p, _size(p), py::str("kept disks")));
        }
    }
    for (const py::str& build : _builds()) {
        py::list<py::str> objs;
        double latest = 0;
        for (const auto& [d, _dirs, names] : py::os::walk(build)) {
            for (const py::str& n : names) {
                py::str p = py::os::path::join(d, n);
                try {
                    latest = py::max(latest, py::os::path::getmtime(p));
                } catch (const py::OSError&) {
                    continue;
                }
                if (n.lower().endswith(std::make_tuple(py::str(".obj"), py::str(".o")))) {
                    objs.append(p);
                }
            }
        }
        if (!objs.empty() && now - latest > static_cast<double>(14 * DAY)) {
            for (const py::str& p : objs) out.append(std::make_tuple(p, _size(p), py::str("builds")));
        }
    }
    return out;
}

py::list<py::str> _builds() {
    py::str croot = py::settings::get_str(py::str("converted"));
    if (croot.empty()) croot = py::os::path::join(py::settings::checkout_root(), py::str("Converted"));
    py::list<py::str> out;
    for (const py::str& b : glob(py::os::path::join(croot, py::str("*"), py::str("build")))) {
        if (py::os::path::isdir(b)) out.append(b);
    }
    return out;
}

py::list<std::tuple<py::str, int64_t>> stray() {
    const py::str state = py::settings::get_str(py::str("state"));
    py::set<py::str> names = known();
    py::list<std::tuple<py::str, int64_t>> out;
    for (const py::str& n : py::sorted(py::os::listdir(state))) {
        if (!_is_known(n, names)) {
            out.append(std::make_tuple(py::os::path::join(state, n), _size(py::os::path::join(state, n))));
        }
    }
    return out;
}

py::list<std::tuple<py::str, int64_t>> parts() {
    py::str state = py::settings::get_str(py::str("state"));
    py::list<std::tuple<py::str, int64_t>> items;
    for (const py::str& n : py::os::listdir(state)) {
        items.append(std::make_tuple(n, _size(py::os::path::join(state, n))));
    }
    return py::sorted(items, [&](const std::tuple<py::str, int64_t>& x) { return -std::get<1>(x); }, false);
}

bool _delete(const py::str& p) {
    if (py::os::path::isdir(p)) {
        py::shutil::rmtree(p, true);
    } else {
        try {
            py::os::remove(p);
        } catch (const py::OSError&) {
        }
    }
    return !py::os::path::exists(p);
}

int64_t main(const py::list<py::str>& argv) {
    if (!argv.empty() && (argv[0] == py::str("-h") || argv[0] == py::str("--help"))) {
        py::print(py::str(DOC));
        return 0;
    }
    const bool dry = in_(py::str("--dry"), argv);
    const py::str state = py::settings::get_str(py::str("state"));
    py::os::makedirs(state, 0777, true);
    py::list<std::tuple<py::str, int64_t>> before = parts();
    int64_t before_total = 0;
    for (const auto& [_n, s] : before) before_total += s;
    py::print(py::percent("state/ (%s): %s", state, _gb(before_total)));
    for (int64_t i = 0; i < py::len(before) && i < 25; i++) {
        py::print(py::percent("  %-28s %10s", std::get<0>(before[i]), _gb(std::get<1>(before[i]))));
    }
    py::list<py::str> builds = _builds();
    if (!builds.empty()) {
        int64_t b_total = 0;
        for (const py::str& b : builds) b_total += _size(b);
        py::print(py::percent("Converted/*/build/ (%d): %s", py::len(builds), _gb(b_total)));
    }
    py::list<std::tuple<py::str, int64_t, py::str>> gone;
    for (const auto& item : stale()) {
        if (!(in_(py::str("--keep-builds"), argv) && std::get<2>(item) == py::str("builds"))) gone.append(item);
    }
    py::list<std::tuple<py::str, int64_t>> strays = stray();
    if (!strays.empty()) {
        py::print(py::percent("not written by CodeBreaker (%s):", (in_(py::str("--stray"), argv) && !dry) ? py::str("removed") : py::str("kept; --stray removes them")));
        for (const auto& [p, s] : strays) {
            py::print(py::percent("  %-28s %10s", py::os::path::basename(p), _gb(s)));
        }
        if (in_(py::str("--stray"), argv)) {
            for (const auto& [p, s] : strays) gone.append(std::make_tuple(p, s, py::str("stray")));
        }
    }
    py::dict<py::str, std::tuple<int64_t, int64_t>> by_rule;
    for (const auto& [p, s, why] : gone) {
        if (dry || _delete(p)) {
            std::tuple<int64_t, int64_t> nb = by_rule.get(why, std::make_tuple(INT64_C(0), INT64_C(0)));
            by_rule.set(why, std::make_tuple(std::get<0>(nb) + 1, std::get<1>(nb) + s));
        }
    }
    for (const py::str& why : py::sorted(by_rule.keys())) {
        const auto& [n, b] = by_rule.at(why);
        py::print(py::percent("%s %-12s %5d item(s)  %10s", dry ? py::str("would remove") : py::str("removed"), why, n, _gb(b)));
    }
    if (!dry) {
        for (const auto& [kind, names] : cache::prune()) {
            py::print(py::percent("removed cache %s: old generation(s) %s", kind, py::join(py::str(", "), names)));
        }
        int64_t after = 0;
        for (const auto& [_n, s] : parts()) after += s;
        py::print(py::percent("state/ now %s (the cache's cap: %s, setting cache_max_gb)", _gb(after), _gb(cache::max_bytes())));
    }
    return 0;
}

}  // namespace codebreaker::base::tidy_state
