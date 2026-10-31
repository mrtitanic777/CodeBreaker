// capi.cc -- the C interface (include/pyengine/pyengine.h).
#include "pyengine/pyengine.h"

#include <string>

#include "engine.hpp"
#include "json.hpp"

struct pyengine_run {
    pyengine::EngineResult r;
    std::string error, ledger, md, unit;
    bool ok = false;
};

extern "C" {

pyengine_run* pyengine_run_package(const char* root, const char* package, const char* policy, int extended, const char* out_dir) {
    auto* run = new pyengine_run();
    pyengine::EngineOptions o;
    o.root = root ? root : "";
    o.package = package ? package : "";
    o.policy = policy ? policy : "strict";
    o.extended = extended != 0;
    o.out = out_dir ? out_dir : "";
    try {
        pyengine::run_engine(o, run->r);
        run->ok = true;
    } catch (const std::exception& e) {
        run->error = e.what();
    }
    return run;
}

int pyengine_ok(const pyengine_run* r) { return r && r->ok; }
const char* pyengine_error(const pyengine_run* r) { return r ? r->error.c_str() : "no run"; }

const char* pyengine_ledger_json(pyengine_run* r, size_t* len) {
    if (r->ledger.empty()) {
        py::V d = py::V::dict();
        for (auto& [q, st] : r->r.fns) {
            py::V x = py::V::dict();
            x.set("status", st.status);
            x.set("reason", st.reason);
            x.set("lines", st.lines);
            if (!st.l2.empty()) x.set("l2", st.l2);
            d.set(q, x);
        }
        r->ledger = py::json_dump(d);
    }
    if (len) *len = r->ledger.size();
    return r->ledger.c_str();
}

const char* pyengine_unit_json(pyengine_run* r, const char* module, size_t* len) {
    auto it = r->r.units.find(module ? module : "");
    if (it == r->r.units.end()) return nullptr;
    if (len) *len = it->second.size();
    return it->second.c_str();
}

const char* pyengine_coverage_md(pyengine_run* r, size_t* len) {
    r->md = pyengine::coverage_markdown(r->r);
    if (len) *len = r->md.size();
    return r->md.c_str();
}

void pyengine_counts(const pyengine_run* r, size_t* functions, size_t* translated, size_t* lines, size_t* translated_lines) {
    if (functions) *functions = r->r.functions;
    if (translated) *translated = r->r.translated;
    if (lines) *lines = r->r.lines;
    if (translated_lines) *translated_lines = r->r.translated_lines;
}

void pyengine_free(pyengine_run* r) { delete r; }

}  // extern "C"
