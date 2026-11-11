// codebreaker/verify/sweep.cpp -- typed C++ port of codebreaker.verify.sweep (see sweep.hpp).

#include "codebreaker/verify/sweep.hpp"

#include <algorithm>
#include <cstdio>
#include <optional>
#include <variant>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::sweep {

namespace {

using py::str;
namespace eng = codebreaker::engine::native_engine;
namespace F = codebreaker::store::functions;
namespace R = codebreaker::verify::reconstruct;

str S(const char* s) { return str(s); }

// the text of a store's text cell ("" for None)
str text_of(const eng::Cell& c) { return eng::cell_str(c); }

void flush_stdout() { std::fflush(stdout); }

}  // namespace

const str DOC = S(
    "sweep.py -- reconstruct and judge every module of the corpus, smallest first, resumably.\n"
    "\n"
    "    python -m codebreaker sweep                    every 32-bit module not yet measured\n"
    "    python -m codebreaker sweep --only a.dll,b.dll these\n"
    "    python -m codebreaker sweep --only @list.txt   one module name per line\n"
    "    python -m codebreaker sweep --16               the 16-bit modules; --drivers the drivers;\n"
    "                                                   --dos real-mode programs; --all every kind\n"
    "    python -m codebreaker sweep --status           what has been measured; no work\n"
    "    python -m codebreaker sweep --again            measure modules again even if already done\n"
    "\n"
    "A module counts as measured when every one of its functions has a result under the current\n"
    "verifier settings, so an interrupted sweep picks up at the module it stopped in, and changing\n"
    "the verifier makes the whole corpus due again without anyone having to say so. One module that\n"
    "fails outright is reported and the sweep moves on.\n");

std::optional<py::set<str>> _wanted(const py::list<str>& argv) {
    if (!argv.contains(S("--only"))) return std::nullopt;
    int64_t at = 0;
    for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
        if (argv[i] == S("--only")) {
            at = i;
            break;
        }
    if (at + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
    const str arg = argv[at + 1];
    py::set<str> out;
    if (arg.startswith(S("@"))) {
        py::file f = py::open(arg.slice_from(1), S("r"), std::optional<str>(S("utf-8")));
        for (const str& ln : f)
            if (!ln.strip().empty()) out.add(ln.strip().lower());
        return out;
    }
    for (const str& x : py::split(arg, S(",")))
        if (!x.strip().empty()) out.add(x.strip().lower());
    return out;
}

py::dict<str, str> status(const std::shared_ptr<eng::FunctionStore>& fs, const py::list<str>& paths,
                          const std::function<void(const str&)>& log) {
    auto digests = R::digests();
    py::dict<str, str> stored;
    {
        // set(digests.values()) - {None}: each distinct digest once (the order of a set is not the language's)
        py::list<str> seen;
        for (const auto& kv : digests.items()) {
            const auto& d = std::get<1>(kv);
            if (d && !seen.contains(*d)) seen.append(*d);
        }
        for (const str& digest : seen)
            for (const auto& row : fs->modules(digest))
                if (!row.empty()) stored.set(text_of(row[0]), digest);
    }
    py::dict<str, str> done;
    py::dict<str, int64_t> tot;
    for (const str& k : F::SCORED()) tot.set(k, 0);
    for (const str& path : paths) {
        const std::optional<str> sha = base::corpus::sha(path);
        const auto kn = R::kind_name(path);
        std::optional<str> want;
        if (kn && digests.contains(*kn)) want = digests.at(*kn);
        if (!sha || !stored.contains(*sha) || stored.at(*sha) != want) continue;
        if (!want) continue;   // (stored digests are never None)
        done.set(path, *sha);
        for (const auto& kv : fs->scores(stored.at(*sha), *sha).items()) {
            if (tot.contains(std::get<0>(kv))) tot.set(std::get<0>(kv), tot.at(std::get<0>(kv)) + eng::cell_int(std::get<1>(kv)));
        }
    }
    int64_t total = 0;
    py::list<str> parts;
    for (const str& k : F::SCORED()) {
        total += tot.at(k);
        parts.append(py::percent(S("%s %d"), F::TITLE().at(k), tot.at(k)));
    }
    const str line = py::percent(S("%d of %d modules measured under the current verifier settings; %d functions: %s"),
                                 static_cast<int64_t>(done.size()), static_cast<int64_t>(paths.size()), total, py::join(S(", "), parts));
    if (log) log(line);
    else py::print(line);
    return done;
}

int64_t main(const py::list<str>& argv) {
    if (build::watcom::ow_tool(S("wcc386")).empty())
        throw py::SystemExit("OpenWatcom is not set up (settings.json: openwatcom): nothing could be built");
    auto fs = F::open();
    py::list<str> kinds;
    if (argv.contains(S("--all"))) {
        kinds = py::list<str>{S("pe"), S("ne"), S("le"), S("dos")};
    } else if (argv.contains(S("--16"))) {
        kinds = py::list<str>{S("ne")};
    } else if (argv.contains(S("--drivers"))) {
        kinds = py::list<str>{S("le")};
    } else if (argv.contains(S("--dos"))) {
        kinds = py::list<str>{S("dos")};
    } else {
        kinds = py::list<str>{S("pe")};
    }
    py::list<str> paths;
    for (const str& k : kinds) {
        if (k == S("le")) {
            for (const str& p : base::corpus::modules(std::optional<str>(k), py::list<str>{S(".vxd"), S(".386")})) paths.append(p);
        } else if (k == S("dos")) {
            std::vector<str> progs = formats::dos::programs().vec();
            std::stable_sort(progs.begin(), progs.end(), [](const str& a, const str& b) { return py::os::path::getsize(a) < py::os::path::getsize(b); });
            for (const str& p : progs) paths.append(p);
        } else {
            for (const str& p : base::corpus::modules(std::optional<str>(k))) paths.append(p);
        }
    }
    const auto only = _wanted(argv);
    if (only) {
        py::list<str> kept;
        for (const str& p : paths)
            if (only->contains(py::os::path::basename(p).lower())) kept.append(p);
        paths = kept;
    }
    py::dict<str, str> done = status(fs, paths);
    if (argv.contains(S("--status"))) return 0;
    int64_t failed = 0;
    for (int64_t i = 0; i < static_cast<int64_t>(paths.size()); ++i) {
        const str path = paths[i];
        const str name = py::os::path::basename(path).lower();
        auto [sha, digest] = R::identity(path);
        if (!argv.contains(S("--again")) && done.contains(path)) {
            auto judged = fs->best(sha, digest);
            bool all = true;
            for (const str& a : fs->addresses(sha))
                if (!judged.contains(a)) {
                    all = false;
                    break;
                }
            if (all) continue;
        }
        const double t0 = py::time::time();
        try {
            R::run(path, fs, [](const str&) {});
        } catch (const py::SystemExit&) {
            throw;
        } catch (const py::Exception& e) {   // one module must not end the sweep
            ++failed;
            py::print(py::percent(S("[%d/%d] %s FAILED: %s: %s"), i + 1, static_cast<int64_t>(paths.size()), name, str(e.type()),
                                  str(e.what()).slice(0, 120)));
            flush_stdout();
            continue;
        }
        auto c = fs->scores(digest, sha);
        py::list<str> parts;
        for (const str& k : F::SCORED())
            if (eng::cell_int(c.at(k))) parts.append(py::percent(S("%s %d"), F::TITLE().at(k), eng::cell_int(c.at(k))));
        py::print(py::percent(S("[%d/%d] %s: %s  (%.0fs)"), i + 1, static_cast<int64_t>(paths.size()), name, py::join(S(", "), parts),
                              py::time::time() - t0));
        flush_stdout();
    }
    status(fs, paths);
    return failed ? 1 : 0;
}

}  // namespace codebreaker::verify::sweep
