// pyengine -- the porting engine on a Python package: L2 per module and the coverage ledger.
//
//   pyengine <package dir> [--coverage] [--out DIR] [--checked] [--old] [--package NAME]
//            [--module M ...] [--log]
//
//   --out DIR    writes DIR/l2/<module>.json (each module's L2 Unit), DIR/coverage.json (per
//                function: translated or refused, with the reason, its lines, its L2) and
//                DIR/coverage.md (the per-package table and the top refusal reasons)
//   --coverage   prints the coverage table
//   --checked    the checked int policy (ints past int64 raise OverflowError in C++)
//   --old        the old engine's rules exactly (no extensions): gate 1's reference run
//   --cpp DIR    writes each module that translated anything through the C++17 back end
//                (native/l2's l2cpp17): DIR/<module path>.hpp / .cpp, and DIR/spans.json (per
//                module, each function's first and last source line; a declined unit's reason)
//   --api DIR    the API maps the back end reads (default: <package dir>/java/api_maps)
//   --no-backend do not hold translated functions to what the back end writes (see README)
//   --stubs DIR  a folder of <module>.pyi stubs (MonkeyType's output), read as parameter hints (inference wins; conflicts reported)
//   --hints DIR  the hints (types.json, refuse.json, refuse.<policy>.json; default native/pyengine/hints)
#include <cstdio>
#include <cstring>
#include <string>

#include <algorithm>
#include <filesystem>
#include <map>
#include <vector>

#include "census.hpp"
#include "dyn.hpp"
#include "engine.hpp"
#include "islands.hpp"

int main(int argc, char** argv) {
    pyengine::EngineOptions o;
    bool coverage = false;

    // pyengine --dynamic --root R --dyn-out DIR [--dyn-main M] [--dyn-closure] [--dyn-exclude P] module|name=file ...
    //   --dyn-typed DIR (the typed path's --out DIR --cpp DIR/cpp): its proven functions as typed islands (src/islands.hpp);
    //   --dyn-islands FILE: only those the judge's verdicts agreed
    if (argc > 1 && std::string(argv[1]) == "--dynamic") {
        pyengine::DynOptions d;
        std::vector<std::string> tree, files;
        for (int i = 2; i < argc; i++) {
            std::string a = argv[i];
            if (a == "--root" && i + 1 < argc) d.root = argv[++i];
            else if (a == "--dyn-out" && i + 1 < argc) d.out = argv[++i];
            else if (a == "--dyn-main" && i + 1 < argc) d.main_module = argv[++i];
            else if (a == "--dyn-exe" && i + 1 < argc) d.exe = argv[++i];
            else if (a == "--dyn-stdlib" && i + 1 < argc) d.stdlib_dir = argv[++i];
            else if (a == "--dyn-site" && i + 1 < argc) d.site_dir = argv[++i];
            else if (a == "--dyn-closure") d.closure = true;
            else if (a == "--dyn-top") d.top_only = true;
            else if (a == "--dyn-full" && i + 1 < argc) d.full_closure.push_back(argv[++i]);
            else if (a == "--dyn-exclude" && i + 1 < argc) d.exclude.push_back(argv[++i]);
            else if (a == "--pyrt" && i + 1 < argc) d.pyrt = argv[++i];
            else if (a == "--dyn-tree" && i + 1 < argc) tree.push_back(argv[++i]);
            else if (a == "--dyn-files" && i + 1 < argc) files.push_back(argv[++i]);
            else if (a == "--dyn-typed" && i + 1 < argc) d.typed_dir = argv[++i];
            else if (a == "--dyn-islands" && i + 1 < argc) d.islands_file = argv[++i];
            else if (a == "--dyn-native" && i + 1 < argc) d.native_dir = argv[++i];
            else d.modules.push_back(a);
        }
        // --dyn-files DIR: every .py file under DIR, each named as pytest names a test file: its path from the
        // nearest folder above it that is not a package (tests/java/test_x.py, java a package: java.test_x)
        for (auto& dir : files) {
            std::error_code ec;
            for (auto it = std::filesystem::recursive_directory_iterator(dir, ec); !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec)) {
                if (!it->is_regular_file() || it->path().extension() != ".py") continue;
                std::filesystem::path p = std::filesystem::absolute(it->path());
                std::filesystem::path base = p.parent_path();
                while (std::filesystem::exists(base / "__init__.py") && base.has_parent_path() && base.parent_path() != base) base = base.parent_path();
                std::string r = std::filesystem::relative(p, base).generic_string();
                r.resize(r.size() - 3);
                if (r.size() >= 9 && r.compare(r.size() - 9, 9, "/__init__") == 0) r.resize(r.size() - 9);
                for (auto& c : r) if (c == '/') c = '.';
                if (r.find('-') != std::string::npos || r.find(' ') != std::string::npos) continue;   // (not importable)
                d.modules.push_back(r + "=" + p.generic_string());
            }
        }
        // --dyn-tree PKG: every module of the package's folder (what importlib.import_module may reach)
        for (auto& pkg : tree) {
            std::string rel = pkg;
            for (auto& c : rel) if (c == '.') c = '/';
            std::filesystem::path base = std::filesystem::path(d.root) / rel;
            std::vector<std::string> found;
            std::error_code ec;
            for (auto it = std::filesystem::recursive_directory_iterator(base, ec); !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec)) {
                if (!it->is_regular_file() || it->path().extension() != ".py") continue;
                std::string r = std::filesystem::relative(it->path(), d.root).generic_string();
                r.resize(r.size() - 3);
                if (r.size() >= 9 && r.compare(r.size() - 9, 9, "/__init__") == 0) r.resize(r.size() - 9);
                for (auto& c : r) if (c == '/') c = '.';
                found.push_back(r);
            }
            std::sort(found.begin(), found.end());
            d.modules.insert(d.modules.end(), found.begin(), found.end());
        }
        if (d.root.empty() || d.out.empty() || d.modules.empty()) {
            fprintf(stderr, "usage: pyengine --dynamic --root R --dyn-out DIR [--dyn-main M] [--dyn-exe NAME] [--dyn-closure] [--dyn-tree PKG] module ...\n");
            return 2;
        }
        std::vector<pyengine::DynModule> mods;
        pyengine::run_dynamic(d, mods);
        size_t lines = 0, refused = 0, fns = 0;
        for (auto& m : mods) {
            lines += (size_t)m.lines;
            refused += m.refused.size();
            fns += (size_t)m.functions;
            if (!m.error.empty()) fprintf(stderr, "%s: %s\n", m.name.c_str(), m.error.c_str());
            for (auto& r : m.refused) fprintf(stderr, "%s:%d: refused: %s\n", m.name.c_str(), r.line, r.what.c_str());
        }
        fprintf(stderr, "%zu modules, %zu lines, %zu functions written; %zu constructs refused\n", mods.size(), lines, fns, refused);
        return 0;
    }
    // pyengine --judge-traces <python trace> <C++ trace>: per function, are its calls (arguments and
    // outcomes, in order) the same in both runs? (the traces: state/work's trace_py.py and PYRT_TRACE)
    if (argc > 3 && std::string(argv[1]) == "--judge-traces") {
        auto load = [](const char* path, std::map<std::string, std::vector<std::string>>& by_fn) {
            FILE* f = fopen(path, "rb");
            if (!f) return false;
            char buf[1 << 16];
            while (fgets(buf, sizeof buf, f)) {
                std::string l = buf;
                while (!l.empty() && (l.back() == '\n' || l.back() == '\r')) l.pop_back();
                size_t p = l.find('(');
                if (p == std::string::npos) continue;
                by_fn[l.substr(0, p)].push_back(l.substr(p));
            }
            fclose(f);
            return true;
        };
        std::map<std::string, std::vector<std::string>> py, cpp;
        if (!load(argv[2], py) || !load(argv[3], cpp)) { fprintf(stderr, "cannot read the traces\n"); return 2; }
        size_t agree = 0, differ = 0, only_py = 0, only_cpp = 0, calls = 0;
        for (auto& [fn, calls_py] : py) {
            auto it = cpp.find(fn);
            if (it == cpp.end()) { only_py++; printf("only in Python (called directly in C++): %s (%zu calls)\n", fn.c_str(), calls_py.size()); continue; }
            calls += calls_py.size();
            if (it->second == calls_py) agree++;
            else {
                differ++;
                size_t k = 0;
                while (k < calls_py.size() && k < it->second.size() && calls_py[k] == it->second[k]) k++;
                printf("DIFFERS %s: %zu / %zu calls, first difference at call %zu\n  py:  %s\n  cpp: %s\n", fn.c_str(), calls_py.size(), it->second.size(), k,
                       k < calls_py.size() ? calls_py[k].c_str() : "(none)", k < it->second.size() ? it->second[k].c_str() : "(none)");
            }
        }
        for (auto& [fn, c] : cpp) if (!py.count(fn)) { only_cpp++; printf("only in C++: %s (%zu calls)\n", fn.c_str(), c.size()); }
        printf("functions: %zu agree, %zu differ, %zu only in Python, %zu only in C++ (%zu calls compared)\n", agree, differ, only_py, only_cpp, calls);
        return differ ? 1 : 0;
    }
    // pyengine --judge-islands <islands.json> <traces with islands> <traces without> <verdicts out>: per typed island,
    // do its calls give the same outcomes with its typed body (PYRT_TRACE in the --dyn-typed program) as with its
    // dynamic body (the same program under PYISL_OFF=1)? (src/islands.hpp)
    if (argc > 5 && std::string(argv[1]) == "--judge-islands") return pyengine::judge_islands(argv[2], argv[3], argv[4], argv[5]);
    // pyengine --census <root>... [--census-out FILE]: the libraries, builtins, methods and features used
    if (argc > 1 && std::string(argv[1]) == "--census") {
        std::vector<std::string> roots;
        std::string out;
        for (int i = 2; i < argc; i++) {
            std::string a = argv[i];
            if (a == "--census-out" && i + 1 < argc) out = argv[++i];
            else roots.push_back(a);
        }
        std::string md = pyengine::run_census(roots, "codebreaker");
        if (out.empty()) fputs(md.c_str(), stdout);
        else {
            FILE* f = fopen(out.c_str(), "wb");
            if (!f) return 1;
            fwrite(md.data(), 1, md.size(), f);
            fclose(f);
        }
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--coverage") coverage = true;
        else if (a == "--out" && i + 1 < argc) o.out = argv[++i];
        else if (a == "--checked") o.policy = "checked";
        else if (a == "--old") o.extended = false;
        else if (a == "--package" && i + 1 < argc) o.package = argv[++i];
        else if (a == "--module" && i + 1 < argc) o.modules.push_back(argv[++i]);
        else if (a == "--log") o.log = true;
        else if (a == "--no-backend") o.backend = false;
        else if (a == "--cpp" && i + 1 < argc) o.cpp = argv[++i];
        else if (a == "--api" && i + 1 < argc) o.api = argv[++i];
        else if (a == "--stubs" && i + 1 < argc) o.stubs = argv[++i];
        else if (a == "--hints" && i + 1 < argc) o.hints = argv[++i];
        else if (!a.empty() && a[0] != '-' && o.root.empty()) o.root = a;
        else {
            fprintf(stderr, "pyengine: unknown argument %s\n", a.c_str());
            return 2;
        }
    }
    if (o.root.empty()) {
        fprintf(stderr, "usage: pyengine <package dir> [--coverage] [--out DIR] [--checked] [--old] [--package NAME] [--module M]\n");
        return 2;
    }
    pyengine::EngineResult r;
    try {
        pyengine::run_engine(o, r);
    } catch (const std::exception& e) {
        fprintf(stderr, "pyengine: %s\n", e.what());
        return 1;
    }
    fprintf(stderr, "%zu modules, %zu functions: types in %d rounds, %.1f s; lowered in %.1f s; %zu translated (%.1f%% of lines)\n",
            r.modules, r.functions, r.rounds, r.seconds_solve, r.seconds_lower, r.translated,
            r.lines ? 100.0 * r.translated_lines / r.lines : 0.0);
    if (coverage) fputs(pyengine::coverage_markdown(r).c_str(), stdout);
    return 0;
}
