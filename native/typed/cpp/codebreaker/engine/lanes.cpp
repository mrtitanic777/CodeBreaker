// codebreaker/engine/lanes.cpp -- typed C++ port of codebreaker.engine.lanes (see lanes.hpp).
//
// Facts are engine::Value (bytes, str or JSON). What the Python pickles into a fact -- the decompiled module, the
// build with its judge context -- has no typed encoding, so such a fact holds a JSON summary (the module's content
// digest; the build's names, signatures and dropped functions) and the object itself is kept by this process in a
// table under that digest / the build's work folder. A process that finds the fact kept but not the object makes the
// object again by the lane's own prepare / build (the Python does the same for a build whose files are gone).

#include "codebreaker/engine/lanes.hpp"
#include "codebreaker/engine/lanes_wiring.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <map>
#include <mutex>
#include <set>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

extern char** environ;

namespace codebreaker::engine::lanes {

namespace {

using facts::Json;
namespace recon = verify::reconstruct;
namespace V = verify::verdicts;

// the identity of the code a module's decompiling and building may run (the Python digests its source files)
const char* const CODE = "typed-cpp:engine.lanes:1";

py::list<LanePlatform> g_platforms;
py::list<py::str> g_kinds;
std::mutex g_lock;

// ---- JSON values ---------------------------------------------------------------------------------------------------
Json j_null() { return Json(); }
Json j_str(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}
Json j_opt(const std::optional<py::str>& s) { return s ? j_str(*s) : j_null(); }
Json j_list() {
    Json j;
    j.kind = Json::List;
    return j;
}
Json j_dict() {
    Json j;
    j.kind = Json::Dict;
    return j;
}
Json j_strs(const py::list<py::str>& v) {
    Json j = j_list();
    for (const py::str& s : v) j.list.push_back(j_str(s));
    return j;
}
py::str jstr(const Json& j) { return j.kind == Json::Str ? j.s : py::str(""); }
std::optional<py::str> jopt(const Json& j) {
    if (j.kind == Json::Str) return j.s;
    return std::nullopt;
}
const Json& jget(const Json& d, const char* k) {
    static const Json none;
    auto it = d.dict.get(py::str(k));
    if (!it) return none;
    static thread_local Json hold;
    hold = *it;
    return hold;
}
Json jat(const Json& d, const char* k) { return d.dict.get(py::str(k)).value_or(Json()); }
Json step_json(const Value& v) {
    if (!std::holds_alternative<Json>(v)) throw py::TypeError("a lane fact is not JSON");
    return std::get<Json>(v);
}
py::str digest_of(const std::string& s) { return py::hashlib::sha1(py::bytes(s)).hexdigest(); }

// ---- the stamps ----------------------------------------------------------------------------------------------------
py::str switches() {
    static const std::set<std::string> skip = {"CB_ENGINE", "CB_NATIVE_ENGINE", "CB_LOW_LOAD", "CB_FUNCTIONS_DB", "CB_STATE", "CB_KNOWLEDGE_DB",
                                               "CB_NO_FP_CACHE", "CB_RESULTS_DB"};
    std::vector<std::string> found;
    for (char** e = environ; e && *e; ++e) {
        std::string kv = *e;
        if (kv.compare(0, 3, "CB_") == 0 && !skip.count(kv.substr(0, kv.find('=')))) found.push_back(kv);
    }
    std::sort(found.begin(), found.end());
    std::string out;
    for (const std::string& f : found) out += f + "\x1f";
    return py::str(out);
}

py::str tools_of(const LanePlatform& p) {
    py::dict<py::str, py::str> tools;
    try {
        if (p.tools) tools = p.tools();
    } catch (const std::exception&) {
    }
    std::vector<std::pair<py::str, py::str>> items;
    for (const auto& kv : tools.items())
        if (!std::get<1>(kv).empty()) items.emplace_back(std::get<0>(kv), base::cache::tool(std::get<1>(kv)));
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    py::str out;
    for (const auto& it : items) out += it.first + py::str("=") + it.second + py::str(";");
    return out;
}

std::function<py::str()> module_stamp(const LanePlatform& p) {
    return [p]() -> py::str {
        py::str struct_recovery = py::settings::get_opt_str(py::str("struct_recovery")).value_or(py::str("")).lower();
        return py::str(CODE) + py::str("|") + p.lane + py::str("|reconstruct|") + base::cache::tool(decompile::decompiler::_exe()) + py::str("|") +
               decompile::decompiler::_language_stamp() + py::str("|") + py::to_str(decompile::decompiler::REV) + py::str("|") + struct_recovery +
               py::str("|") + switches();
    };
}
std::function<py::str()> build_stamp(const LanePlatform& p) {
    return [p]() -> py::str { return py::str(CODE) + py::str("|") + p.lane + py::str("|") + tools_of(p) + py::str("|") + switches(); };
}

// ---- the process's table of what facts stand for -------------------------------------------------------------------
std::map<std::string, std::shared_ptr<recon::Prepared>> g_modules;      // module digest -> the module
struct HeldBuild {
    recon::Built built;
    py::dict<py::str, py::str> dropped;
};
std::map<std::string, std::shared_ptr<build::module::Built>> g_pe_built;    // pe.image: the Built of each module path
std::map<std::string, std::shared_ptr<HeldBuild>> g_builds;             // work folder -> the build

py::str module_digest(const recon::Prepared& m) {
    std::string s = m.header.utf8() + "\x1e" + m.sig.utf8();
    for (const py::str& n : m.order) s += "\x1d" + n.utf8() + "\x1f" + (m.key.contains(n) ? m.key.at(n).utf8() : "") + "\x1f" + (m.texts.contains(n) ? m.texts.at(n).utf8() : "");
    for (const py::str& k : m.all_keys) s += "\x1c" + k.utf8();
    return digest_of(s);
}

py::str work_of(Context& ctx) { return ctx.work(py::list<py::str>{py::str("lane")}); }
py::str mark_of(const py::str& work) { return py::os::path::join(work, py::str("built.mark")); }

std::shared_ptr<recon::Prepared> module_of(Context& ctx, const LanePlatform& p, const py::str& L) {
    Json handle = step_json(ctx.need(L + py::str("module")));
    py::str h = jstr(jat(handle, "handle"));
    auto it = g_modules.find(h.utf8());
    if (it != g_modules.end()) return it->second;
    // kept by an earlier process: made again, as the lane prepares it
    auto m = std::make_shared<recon::Prepared>(p.lane_module()->prepare(jstr(jat(step_json(ctx.need(py::str("binary"))), "path")), work_of(ctx)));
    g_modules[module_digest(*m).utf8()] = m;
    return m;
}

// The module with each function's C as its fact now is (one given by hand in its place).
std::shared_ptr<recon::Prepared> patched(Context& ctx, const LanePlatform& p, const py::str& L) {
    std::shared_ptr<recon::Prepared> m = module_of(ctx, p, L);
    Json keys = jat(step_json(ctx.need(L + py::str("keys"))), "keys");
    py::list<py::str> klist;
    for (const Json& k : keys.list) klist.append(k.s);
    py::dict<py::str, py::str> given;
    for (const auto& kv : ctx.need_many(L + py::str("function"), klist).items()) {
        Json f = step_json(std::get<1>(kv));
        if (f.kind == Json::List && f.list.size() >= 2 && f.list[0].kind == Json::Str) {
            py::str name = f.list[0].s;
            std::optional<py::str> now = m->texts.get(name);
            std::optional<py::str> text = jopt(f.list[1]);
            if (!(now == text)) given.set(name, text ? *text : py::str(""));
        }
    }
    if (given.empty()) return m;
    auto c = std::make_shared<recon::Prepared>(*m);
    py::dict<py::str, py::str> texts = m->texts.copy();
    texts.update(given);
    c->texts = texts;
    std::vector<std::pair<py::str, py::str>> items;
    for (const auto& kv : given.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        if (a.first.u32() != b.first.u32()) return a.first.u32() < b.first.u32();
        return a.second.u32() < b.second.u32();
    });
    std::string s = m->sig.utf8() + std::string(1, '\0');
    for (const auto& it : items) s += it.first.utf8() + std::string(1, '\0') + it.second.utf8() + std::string(1, '\0');
    c->sig = digest_of(s);
    return c;
}

// The lane's build, and what reconstruct.run reads of it.
Json built_of(const LanePlatform& p, const py::str& path, const recon::Prepared& m, const py::str& work) {
    std::shared_ptr<recon::Kind> kind = p.lane_module();
    std::error_code ec;
    std::remove(mark_of(work).utf8().c_str());
    auto held = std::make_shared<HeldBuild>();
    held->built = kind->build(m, path, work);
    held->dropped = held->built.dropped;
    py::dict<py::str, py::str> objects;
    bool have_objects = false;
    if (kind->PER_FUNCTION() && build::functions::OBJECTS.contains(work)) {
        objects = build::functions::OBJECTS.at(work);
        have_objects = !objects.empty();
    }
    Json built = j_dict();
    if (have_objects)
        for (const auto& kv : recon::signatures(m, std::nullopt, objects).items()) built.dict.set(std::get<0>(kv), j_str(std::get<1>(kv)));
    py::list<py::str> names;
    for (const py::str& n : held->built.items.keys()) names.append(n);
    std::vector<py::str> tainted;
    for (const py::str& t : recon::_reaches(m.texts, names, held->dropped)) tainted.push_back(t);
    std::sort(tainted.begin(), tainted.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    {
        py::file f = py::open(mark_of(work), py::str("w"));
        f.write(py::percent(py::str("%.0f\n"), static_cast<double>(std::time(nullptr))));
        f.close();
    }
    g_builds[work.utf8()] = held;
    Json out = j_dict();
    Json dropped = j_dict();
    for (const auto& kv : held->dropped.items()) dropped.dict.set(std::get<0>(kv), j_str(std::get<1>(kv)));
    Json namesj = j_dict();
    for (const auto& kv : m.key.items()) namesj.dict.set(std::get<1>(kv), j_str(std::get<0>(kv)));
    Json items = j_list();
    for (const py::str& n : names) items.list.push_back(j_str(n));
    Json tj = j_list();
    for (const py::str& t : tainted) tj.list.push_back(j_str(t));
    out.dict.set(py::str("dropped"), dropped);
    out.dict.set(py::str("names"), namesj);
    out.dict.set(py::str("items"), items);
    out.dict.set(py::str("built"), built);
    out.dict.set(py::str("tainted"), tj);
    return out;
}

// the hint lane.K.rows hands its verdicts: the names, the tainted ones, and the build the judge runs
struct RowsHint {
    py::dict<py::str, py::str> names;
    std::set<std::string> tainted;
    std::function<std::shared_ptr<HeldBuild>()> ready;
};

}  // namespace

void register_platform(const LanePlatform& p) { g_platforms.append(p); }
// (the platforms that have a lane are registered once, as the Python imports them: lanes_wiring)
const py::list<LanePlatform>& platforms() {
    lanes_wiring::register_core_platforms();
    return g_platforms;
}
py::list<py::str> kinds() {
    lanes_wiring::register_core_platforms();
    return g_kinds;
}

py::dict<py::str, py::str> reconstruct_signatures(const verify::reconstruct::Prepared& m, const verify::reconstruct::Kind& kind) {
    // {name: signature} as reconstruct.run gives them: per function for a lane that builds per function, else the
    // module's own for each.
    if (kind.PER_FUNCTION()) return recon::signatures(m, m.sig);
    py::dict<py::str, py::str> out;
    for (const py::str& n : m.order) out.set(n, m.sig);
    return out;
}

py::str _declare(const LanePlatform& p) {
    const py::str K = p.name;
    const py::str L = py::str("lane.") + K + py::str(".");
    auto lane = [p]() { return p.lane_module(); };
    auto ok = [](const Json& j) -> StepResult { return Value(j); };

    // The lane's profile of the binary: [digest, settings] -- what its verdicts are filed under.
    step(L + py::str("profile"), [=](Context& ctx, const py::str&) -> StepResult {
        auto [digest, settings] = lane()->profile(jstr(jat(step_json(ctx.need(py::str("binary"))), "path")));
        Json j = j_list();
        j.list = {j_str(digest), j_str(settings)};
        return Value(j);
    }, py::list<py::str>{py::str("binary")}, p.id, {}, nullptr, false, true);

    // The module decompiled, as the lane prepares it (its prepare).
    step(L + py::str("module"), [=](Context& ctx, const py::str&) -> StepResult {
        auto m = std::make_shared<recon::Prepared>(lane()->prepare(jstr(jat(step_json(ctx.need(py::str("binary"))), "path")), work_of(ctx)));
        py::str h = module_digest(*m);
        g_modules[h.utf8()] = m;
        Json j = j_dict();
        j.dict.set(py::str("handle"), j_str(h));
        j.dict.set(py::str("sig"), j_str(m->sig));
        return Value(j);
    }, py::list<py::str>{py::str("binary")}, p.id, {}, module_stamp(p));

    // One function: [name, C] (or given by hand: engine.put).
    step(L + py::str("function"), [=](Context& ctx, const py::str& key) -> StepResult {
        auto m = module_of(ctx, p, L);
        for (const py::str& n : m->order)
            if (m->key.contains(n) && m->key.at(n) == key) {
                Json j = j_list();
                j.list = {j_str(n), j_opt(m->texts.get(n))};
                return Value(j);
            }
        return Value(j_null());
    }, py::list<py::str>{L + py::str("module")}, p.id);

    // The keys of the functions that have C, and of every function of the module.
    step(L + py::str("keys"), [=](Context& ctx, const py::str&) -> StepResult {
        auto m = module_of(ctx, p, L);
        std::set<std::string> keys;
        for (const py::str& n : m->order)
            if (m->key.contains(n)) keys.insert(m->key.at(n).utf8());
        std::vector<py::str> sorted;
        for (const std::string& k : keys) sorted.push_back(py::str(k));
        std::sort(sorted.begin(), sorted.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
        Json j = j_dict(), ks = j_list();
        for (const py::str& k : sorted) ks.list.push_back(j_str(k));
        j.dict.set(py::str("keys"), ks);
        j.dict.set(py::str("all_keys"), j_strs(m->all_keys));
        return Value(j);
    }, py::list<py::str>{L + py::str("module")}, p.id);

    // {all_keys, header, order, functions: {key: [name, signature, C]}}: the module's C as it is now (a function given
    // by hand in it), and each function's signature -- what reconstruct.run files its verdict under.
    step(L + py::str("texts"), [=](Context& ctx, const py::str&) -> StepResult {
        auto m = patched(ctx, p, L);
        auto sig = reconstruct_signatures(*m, *lane());
        Json j = j_dict(), order = j_list(), fns = j_dict();
        for (const py::str& n : m->order) {
            if (!m->key.contains(n)) continue;
            order.list.push_back(j_str(m->key.at(n)));
            Json f = j_list();
            f.list = {j_str(n), j_str(sig.get(n, py::str(""))), j_opt(m->texts.get(n))};
            fns.dict.set(m->key.at(n), f);
        }
        j.dict.set(py::str("all_keys"), j_strs(m->all_keys));
        j.dict.set(py::str("header"), j_str(m->header));
        j.dict.set(py::str("order"), order);
        j.dict.set(py::str("functions"), fns);
        return Value(j);
    }, py::list<py::str>{L + py::str("module"), L + py::str("keys"), L + py::str("function")}, p.id);

    // The functions built for judging (the lane's build): {items, dropped, names, built, tainted}.
    step(L + py::str("build"), [=](Context& ctx, const py::str&) -> StepResult {
        auto m = patched(ctx, p, L);
        return Value(built_of(p, jstr(jat(step_json(ctx.need(py::str("binary"))), "path")), *m, work_of(ctx)));
    }, py::list<py::str>{py::str("binary"), L + py::str("module"), L + py::str("keys"), L + py::str("function")}, p.id, {}, build_stamp(p));

    // What the function's verdict is reached for: ["built", signature] or ["dropped", why].
    step(L + py::str("built"), [=](Context& ctx, const py::str& key) -> StepResult {
        Json b = step_json(ctx.need(L + py::str("build")));
        std::optional<Json> name = jat(b, "names").dict.get(key);
        bool in_items = false;
        if (name)
            for (const Json& i : jat(b, "items").list)
                if (i.s == name->s) in_items = true;
        Json out = j_list();
        if (!name || !in_items) {
            std::optional<Json> why = name ? jat(b, "dropped").dict.get(name->s) : std::nullopt;
            out.list = {j_str(py::str("dropped")), j_str(why && !why->s.empty() ? why->s : py::str("not in the build"))};
            return Value(out);
        }
        Json built = jat(b, "built");
        if (!built.dict.empty()) {
            out.list = {j_str(py::str("built")), built.dict.get(name->s).value_or(Json())};
            return Value(out);
        }
        Json t = step_json(ctx.need(L + py::str("texts")));
        out.list = {j_str(py::str("built")), jat(t, "functions").dict.at(key).list.at(1)};
        return Value(out);
    }, py::list<py::str>{L + py::str("build"), L + py::str("texts")}, p.id);

    // The function judged beside the original (the lane's judge): [verdict, reason, limits, detail]. Asked for by
    // lane.K.rows, which hands it the build.
    batch_step(L + py::str("verdict"), [=](const py::dict<py::str, Context*>& ctxs, const std::any& hint) -> py::dict<py::str, StepResult> {
        const RowsHint* h = std::any_cast<RowsHint>(&hint);
        if (!h) throw py::LookupError("a verdict is judged in a build of the module: ask for " + L.utf8() + "rows");
        py::dict<py::str, StepResult> out;
        std::vector<py::str> todo;
        for (const auto& kv : ctxs.items()) {
            Context& c = *std::get<1>(kv);
            Json b = step_json(c.need(L + py::str("built"), std::get<0>(kv)));
            c.need(L + py::str("profile"));
            if (b.list.at(0).s == py::str("dropped")) {
                Json r = j_list();
                r.list = {j_str(V::NO_COMPILE), b.list.at(1), j_str(py::str("")), j_str(py::str(""))};
                out.set(std::get<0>(kv), StepResult(Value(r)));
            } else {
                todo.push_back(std::get<0>(kv));
            }
        }
        if (!todo.empty()) {
            std::shared_ptr<HeldBuild> b = h->ready();
            for (const py::str& k : todo) {
                Context& c = *ctxs.at(k);
                // (each function's judge brings back what it read of the knowledge base -- the argument counts of the
                // imports it calls -- as that verdict's inputs)
                knowledge::reads::recording rec;
                recon::Verdict v = recon::NOT_RUN();
                try {
                    auto item = b->built.items.get(h->names.at(k));
                    if (item) v = lane()->judge(b->built.context, item);
                } catch (const std::exception&) {
                    v = recon::NOT_RUN();
                }
                c.knowledge(rec.sorted_keys());
                py::list<py::str> limits = std::get<2>(v);
                if (h->tainted.count(h->names.at(k).utf8()) && recon::GOOD().contains(std::get<0>(v))) {
                    py::set<py::str> s = py::set<py::str>::from(limits);
                    s.add(py::str("stub-callee"));
                    std::vector<py::str> sorted;
                    for (const py::str& x : s) sorted.push_back(x);
                    std::sort(sorted.begin(), sorted.end(), [](const py::str& a, const py::str& b2) { return a.u32() < b2.u32(); });
                    limits = py::list<py::str>();
                    for (const py::str& x : sorted) limits.append(x);
                }
                Json r = j_list();
                r.list = {j_str(std::get<0>(v)), j_str(std::get<1>(v)), j_str(py::join(py::str(","), limits)), j_str(std::get<3>(v))};
                out.set(k, StepResult(Value(r)));
            }
        }
        return out;
    }, py::list<py::str>{L + py::str("built"), L + py::str("profile")}, p.id);

    // The rows reconstruct.run records: {profile, settings, header, all_keys, rows, built}.
    step(L + py::str("rows"), [=](Context& ctx, const py::str&) -> StepResult {
        py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
        Json t = step_json(ctx.need(L + py::str("texts")));
        Json fns = jat(t, "functions");
        Json prof = step_json(ctx.need(L + py::str("profile")));
        Json out = j_dict(), rows = j_list(), builtmap = j_dict();
        out.dict.set(py::str("profile"), prof.list.at(0));
        out.dict.set(py::str("settings"), prof.list.at(1));
        out.dict.set(py::str("header"), jat(t, "header"));
        out.dict.set(py::str("all_keys"), jat(t, "all_keys"));
        auto row = [](const py::str& addr, const py::str& sig, const Json& name, const Json& source, const py::str& verdict, const Json& reason,
                      const Json& limits, const Json& detail, const py::str& problem) {
            Json r = j_dict();
            r.dict.set(py::str("addr"), j_str(addr));
            r.dict.set(py::str("sig"), j_str(sig));
            r.dict.set(py::str("name"), name);
            r.dict.set(py::str("source"), source);
            r.dict.set(py::str("verdict"), j_str(verdict));
            r.dict.set(py::str("reason"), reason);
            if (!limits.dict.empty() || limits.kind != Json::Dict) r.dict.set(py::str("limits"), limits);
            if (detail.kind != Json::Dict) r.dict.set(py::str("detail"), detail);
            r.dict.set(py::str("problem"), j_str(problem));
            return r;
        };
        for (const Json& k : jat(t, "all_keys").list)
            if (!fns.dict.contains(k.s))
                rows.list.push_back(row(k.s, py::str("-"), j_str(py::str("")), j_null(), V::NO_SOURCE,
                                        j_str(py::str("the decompiler produced nothing for this function")), j_dict(), j_dict(), py::str("none")));
        if (fns.dict.empty()) {
            out.dict.set(py::str("rows"), rows);
            out.dict.set(py::str("built"), builtmap);
            return Value(out);
        }
        Json b = step_json(ctx.need(L + py::str("build")));
        py::list<py::str> mine;
        std::set<std::string> seen;
        for (const Json& k : jat(t, "order").list)
            if (seen.insert(k.s.utf8()).second) mine.append(k.s);        // (judged in the module's order, as reconstruct.run judges)
        ctx.need_many(L + py::str("built"), mine);
        RowsHint hint;
        for (const py::str& k : mine) hint.names.set(k, fns.dict.at(k).list.at(0).s);
        for (const Json& x : jat(b, "tainted").list) hint.tainted.insert(x.s.utf8());
        hint.ready = [&ctx, &p, path, L]() -> std::shared_ptr<HeldBuild> {
            // The build the judge runs: the kept one, or -- its files gone -- made again there.
            py::str work = work_of(ctx);
            auto it = g_builds.find(work.utf8());
            if (py::os::path::isfile(mark_of(work)) && it != g_builds.end()) return it->second;
            built_of(p, path, *patched(ctx, p, L), work);
            return g_builds.at(work.utf8());
        };
        auto verdicts = ctx.need_many(L + py::str("verdict"), mine, std::any(hint));
        for (const py::str& k : mine) {
            const Json& f = fns.dict.at(k);
            Json v = step_json(verdicts.at(k));
            bool nc = v.list.at(0).s == V::NO_COMPILE;
            rows.list.push_back(row(k, f.list.at(1).s, f.list.at(0), f.list.at(2), v.list.at(0).s, v.list.at(1), v.list.at(2), v.list.at(3),
                                    nc ? py::str("compile") : py::str("")));
            if (!nc) rows.list.back().dict.pop(py::str("problem"));
        }
        for (const py::str& k : mine) {
            std::optional<Json> name = jat(b, "names").dict.get(k);
            if (name && jat(b, "built").dict.contains(name->s)) builtmap.dict.set(k, jat(b, "built").dict.at(name->s));
        }
        out.dict.set(py::str("rows"), rows);
        out.dict.set(py::str("built"), builtmap);
        return Value(out);
    }, py::list<py::str>{py::str("binary"), L + py::str("module"), L + py::str("keys"), L + py::str("texts"), L + py::str("function"), L + py::str("build"),
                         L + py::str("built"), L + py::str("verdict"), L + py::str("profile")}, p.id);

    (void)ok;
    if (p.name == py::str("pe")) {
        // The module linked (module.build's build of a 32-bit module: the packager's link, then the check of the image
        // against the original): {name, image (latin-1 text of its bytes), rejected, report}.
        step(py::str("pe.image"), [=](Context& ctx, const py::str&) -> StepResult {
            py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
            auto m = patched(ctx, p, L);          // (a function given by hand in its place)
            py::str folder = ctx.work(py::list<py::str>{py::str("build")});
            auto b = std::make_shared<build::module::Built>(path);
            b->folder = folder;
            b->functions = m->all_keys.size();
            std::optional<py::str> image = p.link ? p.link(*b, path, *m, folder) : std::nullopt;
            Json out = j_dict(), rejected = j_dict();
            out.dict.set(py::str("name"), j_str(b->name));
            for (const auto& kv : b->rejected.items()) rejected.dict.set(std::get<0>(kv), j_str(std::get<1>(kv)));
            Json img = j_null();
            if (image) {
                b->image = py::os::path::join(folder, py::os::path::basename(path).lower());
                py::os::makedirs(folder, 0777, true);
                std::ifstream in(image->utf8(), std::ios::binary);
                std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                py::file f = py::open(*b->image, py::str("wb"));
                f.write(py::bytes(bytes));
                f.close();
                build::module::check(*b);
                img = j_str(py::bytes(bytes).decode("latin1"));
            }
            out.dict.set(py::str("image"), img);
            out.dict.set(py::str("rejected"), rejected);
            out.dict.set(py::str("report"), j_str(b->report()));
            g_pe_built[path.utf8()] = b;
            return Value(out);
        }, py::list<py::str>{py::str("binary"), L + py::str("module"), L + py::str("keys"), L + py::str("function")}, py::str("win32"), {},
             [p]() -> py::str { return py::str(CODE) + py::str("|") + tools_of(p) + py::str("|") + switches(); });
    }
    g_kinds.append(K);
    return K;
}

bool on(const py::str& path) {
    lanes_wiring::register_core_platforms();
    for (const LanePlatform& p : g_platforms)
        if (p.identifies && p.identifies(path)) return !p.lane.empty() && g_kinds.contains(p.name);
    return false;
}

namespace {

const LanePlatform* identify_(const py::str& path) {
    lanes_wiring::register_core_platforms();
    for (const LanePlatform& p : g_platforms)
        if (p.identifies && p.identifies(path)) return &p;
    return nullptr;
}

}  // namespace

std::shared_ptr<native_engine::Database> open_module(const py::str& path) {
    // The module's database: a binary's by its bytes (engine.open); a module that is a folder (a Java program's
    // source) by what its lane files its results under (the lane's sha).
    if (py::os::path::isfile(path)) return engine::open(path);
    const LanePlatform* p = identify_(path);
    if (!p) throw py::ValueError("no platform for " + path.utf8());
    std::optional<py::str> sha = p->lane_module()->sha(path);
    auto db = facts::open_at(py::os::path::join(facts::root(), sha.value_or(py::str("")) + py::str(".sqlite")));
    db->sha = sha;
    if (!db->about(py::str("name"))) db->about(py::str("name"), py::os::path::basename(py::os::path::normpath(path)));
    db->about(py::str("path"), py::os::path::abspath(path));
    Json value = j_dict();
    value.dict.set(py::str("path"), j_str(py::os::path::abspath(path)));
    Json size;
    size.kind = Json::Int;
    value.dict.set(py::str("size"), size);
    engine::put(*db, py::str("binary"), py::str(""), Value(value));
    return db;
}

Judged judged(const py::str& path, const std::function<void(const py::str&)>& log) {
    // reconstruct.run's decompile, build and judge of a module, made by the engine. -> Judged
    const LanePlatform* p = identify_(path);
    if (!p) throw py::ValueError("no platform for " + path.utf8());
    auto db = open_module(path);
    Json got = step_json(engine::get(db, py::str("lane.") + p->name + py::str(".rows"), py::str(""), log));
    Judged out;
    out.profile = jstr(jat(got, "profile"));
    out.settings = jstr(jat(got, "settings"));
    out.header = jstr(jat(got, "header"));
    for (const Json& k : jat(got, "all_keys").list) out.all_keys.append(k.s);
    for (const Json& r : jat(got, "rows").list) {
        JudgedRow row;
        row.addr = jstr(jat(r, "addr"));
        row.sig = jstr(jat(r, "sig"));
        row.name = jstr(jat(r, "name"));
        row.source = jopt(jat(r, "source"));
        row.verdict = jstr(jat(r, "verdict"));
        row.reason = jstr(jat(r, "reason"));
        row.limits = jstr(jat(r, "limits"));
        row.detail = jstr(jat(r, "detail"));
        row.problem = r.dict.contains(py::str("problem")) ? jopt(jat(r, "problem")) : std::optional<py::str>("none");
        out.rows.append(row);
    }
    for (const auto& kv : jat(got, "built").dict.items()) out.built.set(std::get<0>(kv), jstr(std::get<1>(kv)));
    out.made = engine::last_run(*db);
    return out;
}

std::shared_ptr<build::module::Built> build(const py::str& path0, const std::optional<py::str>& outdir, bool call,
                                            const std::function<void(const py::str&)>& log0) {
    // module.build of a module whose platform has these steps: a 32-bit module made by the engine.
    std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    std::optional<py::str> found = base::corpus::retail(path0);
    py::str path = (found && !found->empty()) ? *found : path0;
    const LanePlatform* p = identify_(path);
    if (!p || p->name != py::str("pe"))
        throw py::SystemExit(py::percent(py::str("%s: a %s module is not linked whole here"), py::os::path::basename(path).lower(),
                                         p ? p->name : py::str("?")));
    auto db = engine::open(path);
    log(py::percent(py::str("%s: on the engine (%s)"), py::os::path::basename(path).lower(), db->path));
    Json got = step_json(engine::get(db, py::str("pe.image"), py::str(""), log));
    std::shared_ptr<build::module::Built> b = g_pe_built.count(path.utf8()) ? g_pe_built.at(path.utf8()) : std::make_shared<build::module::Built>(path);
    if (jat(got, "image").kind == Json::Str && !jstr(jat(got, "image")).empty()) {
        py::str where = outdir ? *outdir : facts::work(*db, py::list<py::str>{py::str("build")});
        py::os::makedirs(where, 0777, true);
        b->image = py::os::path::join(where, py::os::path::basename(path).lower());
        py::file f = py::open(*b->image, py::str("wb"));
        f.write(jstr(jat(got, "image")).encode("latin1"));
        f.close();
    } else {
        b->image = std::nullopt;
    }
    if (call && b->image) build::module::run(*b);
    facts::hold();
    return b;
}

}  // namespace codebreaker::engine::lanes
