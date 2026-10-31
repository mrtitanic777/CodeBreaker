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
#include <cstdio>
#include <cstring>
#include <string>

#include "engine.hpp"

int main(int argc, char** argv) {
    pyengine::EngineOptions o;
    bool coverage = false;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--coverage") coverage = true;
        else if (a == "--out" && i + 1 < argc) o.out = argv[++i];
        else if (a == "--checked") o.policy = "checked";
        else if (a == "--old") o.extended = false;
        else if (a == "--package" && i + 1 < argc) o.package = argv[++i];
        else if (a == "--module" && i + 1 < argc) o.modules.push_back(argv[++i]);
        else if (a == "--log") o.log = true;
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
