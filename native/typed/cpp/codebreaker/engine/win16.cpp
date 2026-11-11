// codebreaker/engine/win16.cpp -- typed C++ port of codebreaker.engine.win16 (see win16.hpp).

#include "codebreaker/engine/win16.hpp"

#include <algorithm>
#include <fstream>
#include <map>
#include <mutex>
#include <set>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/engine/engine.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/verify/gate16/contract16.hpp"
#include "codebreaker/verify/gate16/isolated16.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

extern char** environ;

namespace codebreaker::engine::win16 {

namespace {

using facts::Json;
namespace V = verify::verdicts;
namespace lane = verify::lanes::win16;
namespace mod = build::module;

// the identity of the code this build is made of (the Python digests its source files)
const char* const CODE = "typed-cpp:engine.win16:1";

// ---- JSON values ----------------------------------------------------------------------------------------------------
Json j_null() { return Json(); }
Json j_str(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}
Json j_int(int64_t v) {
    Json j;
    j.kind = Json::Int;
    j.i = v;
    return j;
}
Json j_bool(bool v) {
    Json j;
    j.kind = Json::Bool;
    j.b = v;
    return j;
}
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
py::str jstr(const Json& j) { return j.kind == Json::Str ? j.s : py::str(""); }
Json jat(const Json& d, const char* k) { return d.dict.get(py::str(k)).value_or(Json()); }
Json step_json(const Value& v) {
    if (!std::holds_alternative<Json>(v)) throw py::TypeError("a 16-bit fact is not JSON");
    return std::get<Json>(v);
}
Json step_json_str(const Value& v) {
    if (std::holds_alternative<py::str>(v)) return j_str(std::get<py::str>(v));
    return step_json(v);
}
bool sort_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

// ---- stamps -----------------------------------------------------------------------------------------------------------
py::str switches() {
    static const char* const names[] = {"CB_ASM_ONLY", "CB_ASM_SEGMENTS", "CB_FORCE_C", "CB_ASM_KEEP_C", "CB_NO_GATE", "CB_NO_GUIDED", "CB_FIXED_GROWS",
                                        "CB_SELECTOR_STRUCTS", "CB_CODE_DATA", "CB_CALLER_ARGS", "CB_FASTCALL", "CB_INLINE_HELPERS", "CB_PROTOTYPES",
                                        "CB_CALLER_RESULTS"};
    py::str out;
    for (const char* n : names) {
        const auto v = py::os::environ_::get(py::str(n));
        out += py::str(n) + py::str("=") + (v ? *v : py::str("")) + py::str(";");
    }
    return out;
}

py::str module_stamp() {
    const py::str struct_recovery = py::settings::get_opt_str(py::str("struct_recovery")).value_or(py::str("off")).strip().lower();
    return py::str(CODE) + py::str("|module|") + base::cache::tool(decompile::decompiler::_exe()) + py::str("|") + decompile::decompiler::_language_stamp() +
           py::str("|") + py::to_str(decompile::decompiler::REV) + py::str("|") + struct_recovery + py::str("|") + switches();
}
py::str gate_stamp() {
    return py::str(CODE) + py::str("|gate|") + base::cache::tool(build::watcom::ow_tool(py::str("wcc"))) + py::str("|") + switches();
}
py::str image_stamp() {
    py::str tools;
    for (const char* t : {"wcc", "wlink", "wasm"}) tools += base::cache::tool(build::watcom::ow_tool(py::str(t))) + py::str(";");
    return py::str(CODE) + py::str("|image|") + tools + py::str("|") + switches();
}

// ---- the process's table of what facts stand for -------------------------------------------------------------------
std::mutex g_lock;
std::map<std::string, std::shared_ptr<lane::Prepared>> g_modules;              // module digest -> the module
std::map<std::string, std::shared_ptr<mod::Built>> g_built;                    // binary path -> the Built of ne.image

py::str module_digest(const lane::Prepared& m) {
    std::string s = m.header().utf8() + "\x1e" + m.sig.utf8();
    for (const py::str& n : m.order()) {
        const auto t = m.texts().get(n);
        s += "\x1d" + n.utf8() + "\x1f" + (m.key.contains(n) ? m.key.at(n).utf8() : "") + "\x1f" + (t ? t->utf8() : "");
    }
    for (const py::str& k : m.all_keys) s += "\x1c" + k.utf8();
    return py::hashlib::sha1(py::bytes(s)).hexdigest();
}

std::shared_ptr<lane::Prepared> prepare_into(const py::str& path, const py::str& work) {
    return std::make_shared<lane::Prepared>(lane::prepare(path, work));
}

std::shared_ptr<lane::Prepared> module_of(Context& ctx) {
    const Json handle = step_json(ctx.need(py::str("ne.module")));
    const py::str h = jstr(jat(handle, "handle"));
    {
        std::lock_guard<std::mutex> g(g_lock);
        const auto it = g_modules.find(h.utf8());
        if (it != g_modules.end()) return it->second;
    }
    // kept by an earlier process: made again, as ne.module makes it
    const py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
    auto m = prepare_into(path, ctx.work(py::list<py::str>{py::str("decompiled")}));
    std::lock_guard<std::mutex> g(g_lock);
    g_modules[module_digest(*m).utf8()] = m;
    return m;
}

const std::vector<py::str> NO_NAMES;

// (the options module._build16 compiles a function with, before its code segment's -nt=: the medium model, the 8087, for size, the code
// class it lays out first)
py::list<py::str> cflags16() {
    py::list<py::str> out;
    for (const py::str& f : build::functions::CFLAGS16)
        if (f != py::str("-ms")) out.append(f);
    for (const char* f : {"-mm", "-fpi87", "-os", "-nc=MCODE"}) out.append(py::str(f));
    return out;
}
py::str wcc() { return build::watcom::ow_tool(py::str("wcc")); }
py::list<py::str> flags_of(const Json& j) {
    py::list<py::str> out;
    for (const Json& f : j.list) out.append(f.s);
    return out;
}
Json strs_json(const py::list<py::str>& v) {
    Json j = j_list();
    for (const py::str& s : v) j.list.push_back(j_str(s));
    return j;
}
// module._build16's `named`: a call the decompiler left as an address (`func_0x...`) goes to the function reconstructed there, when it can be
// reached from this segment.
py::str named(const py::str& text, int64_t seg, const Json& funcs) {
    std::map<int64_t, Json> by_va;
    for (const auto& kv : funcs.dict.items()) by_va[jat(std::get<1>(kv), "va").i] = std::get<1>(kv);
    return mod::_BY_ADDRESS.sub(
        [&](const py::re::Match& mm) -> py::str {
            const auto it = by_va.find(py::int_parse(mm.group(1), 16));
            if (it != by_va.end()) {
                const Json& t = it->second;
                if (jat(t, "seg").kind == Json::Int && (!jat(t, "near").b || jat(t, "seg").i == seg)) return jstr(jat(t, "name"));
            }
            return mm.group(0);
        },
        text);
}
// the folder a segment's functions are compiled in, its header beside them
std::mutex g_folders_lock;
std::map<std::string, py::str> g_folders;
py::str folder_of(Context& ctx, int64_t seg, const py::str& header) {
    const py::str d = ctx.work(py::list<py::str>{py::str("objects"), py::percent("s%02d", seg)});
    const py::str want = base::cache::key({base::cache::Part(header)});
    std::lock_guard<std::mutex> g(g_folders_lock);
    const auto it = g_folders.find(d.utf8());
    if (it == g_folders.end() || it->second != want) {
        py::os::makedirs(d, 0777, true);
        std::ofstream(py::os::path::join(d, py::str("m.h")).utf8(), std::ios::binary) << header.utf8();
        g_folders[d.utf8()] = want;
    }
    return d;
}

// ---- the steps --------------------------------------------------------------------------------------------------------
void declare_steps() {
    const py::str P("win16");
    auto reads = [](std::initializer_list<const char*> kinds) {
        py::list<py::str> out;
        for (const char* k : kinds) out.append(py::str(k));
        return out;
    };

    // The segment table: number, kind, where in the file, length, flags (the loader's parts).
    step(py::str("ne.segments"), [](Context& ctx, const py::str&) -> StepResult {
        const py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
        formats::ne::NEFile nef(path);
        Json out = j_list();
        for (const auto& s : nef.segments()) {
            Json d = j_dict();
            d.dict.set(py::str("idx"), j_int(s.idx));
            d.dict.set(py::str("file_off"), j_int(s.file_off));
            d.dict.set(py::str("length"), j_int(s.length));
            d.dict.set(py::str("flags"), j_int(s.flags));
            d.dict.set(py::str("minalloc"), j_int(s.minalloc));
            out.list.push_back(d);
        }
        return Value(out);
    }, reads({"binary"}), P);

    // The module decompiled and made compilable, as module.build prepares it (the lane's prepare: describe16, the decompiler, c_text).
    step(py::str("ne.module"), [](Context& ctx, const py::str&) -> StepResult {
        const py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
        auto m = prepare_into(path, ctx.work(py::list<py::str>{py::str("decompiled")}));
        const py::str h = module_digest(*m);
        {
            std::lock_guard<std::mutex> g(g_lock);
            g_modules[h.utf8()] = m;
        }
        Json j = j_dict();
        j.dict.set(py::str("handle"), j_str(h));
        j.dict.set(py::str("sig"), j_str(m->sig));
        return Value(j);
    }, reads({"binary"}), P, {}, module_stamp);

    // The description's lines (F a function, X an import, O a call's convention, bytes ...).
    step(py::str("ne.description"), [](Context& ctx, const py::str&) -> StepResult {
        auto m = module_of(ctx);
        Json out = j_list();
        if (m->description)
            for (const py::str& l : m->description->lines) out.list.push_back(j_str(l));
        return Value(out);
    }, reads({"ne.module"}), P);

    // Every function: {key: {va, name, seg, off, near, stem}} -- stem the name of its object in its segment's folder.
    step(py::str("ne.functions"), [](Context& ctx, const py::str&) -> StepResult {
        auto m = module_of(ctx);
        Json out = j_dict();
        std::map<int64_t, std::vector<std::pair<int64_t, py::str>>> by_segment;
        std::vector<std::pair<int64_t, py::str>> vas;
        for (const auto& kv : m->by_va().items()) vas.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(vas.begin(), vas.end());
        for (const auto& [va, name] : vas) {
            const auto w = m->where.get(va);
            Json d = j_dict();
            d.dict.set(py::str("va"), j_int(va));
            d.dict.set(py::str("name"), j_str(name));
            const bool placed = w && *w;
            d.dict.set(py::str("seg"), placed ? j_int(std::get<0>(**w)) : j_null());
            d.dict.set(py::str("off"), placed ? j_int(std::get<1>(**w)) : j_null());
            d.dict.set(py::str("near"), j_bool(m->near.contains(name)));
            const py::str key = m->key.at(name);
            out.dict.set(key, d);
            if (placed) by_segment[std::get<0>(**w)].emplace_back(std::get<1>(**w), key);
        }
        for (auto& seg : by_segment) {
            std::sort(seg.second.begin(), seg.second.end(), [](const auto& a, const auto& b) { return a.first != b.first ? a.first < b.first : sort_less(a.second, b.second); });
            int64_t i = 0;
            for (const auto& off_key : seg.second) {
                Json d = *out.dict.get(off_key.second);
                d.dict.set(py::str("stem"), j_str(py::percent("f%04d", i++)));
                out.dict.set(off_key.second, d);
            }
        }
        return Value(out);
    }, reads({"ne.module"}), P);

    // The header of types and prototypes every function is compiled with.
    step(py::str("ne.header"), [](Context& ctx, const py::str&) -> StepResult { return Value(module_of(ctx)->header()); }, reads({"ne.module"}), P);

    // {name: its line in the description} (F and X lines): how the gate answers a call to it.
    step(py::str("ne.lines"), [](Context& ctx, const py::str&) -> StepResult {
        Json out = j_dict();
        for (const Json& l : step_json(ctx.need(py::str("ne.description"))).list) {
            const py::list<py::str> p = py::split(l.s, py::str("\t"));
            if ((p[0] == py::str("F") || p[0] == py::str("X")) && py::len(p) > 2) out.dict.set(p[2], l);
        }
        return Value(out);
    }, reads({"ne.description"}), P);

    // The function's C, as the decompiler and c_text made it (or as given by hand).
    step(py::str("function.c"), [](Context& ctx, const py::str& key) -> StepResult {
        const py::str name = jstr(jat(jat(step_json(ctx.need(py::str("ne.functions"))), key.utf8().c_str()), "name"));
        auto m = module_of(ctx);
        const auto t = m->texts().get(name);
        return Value(t ? j_str(*t) : j_null());
    }, reads({"ne.module", "ne.functions"}), P);

    // What the decompiler said of the function besides its C (decompiler.split_notes: its pointers' sizes, the globals it touches, the
    // structures it recovered), or null.
    step(py::str("function.notes"), [](Context& ctx, const py::str& key) -> StepResult {
        const py::str name = jstr(jat(jat(step_json(ctx.need(py::str("ne.functions"))), key.utf8().c_str()), "name"));
        auto m = module_of(ctx);
        const auto nt = m->notes.get(name);
        if (!nt) return Value(j_null());
        Json out = j_dict();
        Json gl = j_list();
        for (const auto& g : nt->globals) {
            Json one = j_list();
            one.list = {j_int(g.address), j_int(g.bytes), j_int(g.elem)};
            gl.list.push_back(one);
        }
        out.dict.set(py::str("globals"), gl);
        Json pt = j_dict();
        for (const auto& kv : nt->pointers.items()) pt.dict.set(std::get<0>(kv), j_int(std::get<1>(kv)));
        out.dict.set(py::str("pointers"), pt);
        out.dict.set(py::str("structs"), j_int(py::len(nt->structs)));
        out.dict.set(py::str("has_l2"), j_bool(nt->l2.has_value()));
        out.dict.set(py::str("placed"), j_bool(nt->placed));
        out.dict.set(py::str("from_l2"), j_bool(nt->from_l2));
        return Value(out);
    }, reads({"ne.module", "ne.functions"}), P);

    // The function's L2 document, the C of it is written from (as its text), or null: the decompiler made none.
    step(py::str("function.l2"), [](Context& ctx, const py::str& key) -> StepResult {
        const Json notes = step_json(ctx.need(py::str("function.notes"), key));
        if (notes.kind != Json::Dict || !jat(notes, "has_l2").b) return Value(j_null());
        const py::str name = jstr(jat(jat(step_json(ctx.need(py::str("ne.functions"))), key.utf8().c_str()), "name"));
        const auto nt = module_of(ctx)->notes.get(name);
        if (!nt || !nt->l2) return Value(j_null());
        return Value(j_str(translate::ir::Val(*nt->l2).repr()));
    }, reads({"function.notes", "ne.module", "ne.functions"}), P);

    // What the build compiles for the function: {text, seg, stem, flags}.
    step(py::str("function.source"), [](Context& ctx, const py::str& key) -> StepResult {
        const Json funcs = step_json(ctx.need(py::str("ne.functions")));
        const Json f = jat(funcs, key.utf8().c_str());
        const Json c = step_json(ctx.need(py::str("function.c"), key));
        if (c.kind != Json::Str || jat(f, "seg").kind != Json::Int) return Value(j_null());
        const int64_t seg = jat(f, "seg").i;
        Json out = j_dict();
        out.dict.set(py::str("text"), j_str(named(c.s, seg, funcs)));
        out.dict.set(py::str("seg"), j_int(seg));
        out.dict.set(py::str("stem"), jat(f, "stem"));
        py::list<py::str> fl = cflags16();
        fl.append(py::percent("-nt=CODE%d", seg));
        out.dict.set(py::str("flags"), strs_json(fl));
        return Value(out);
    }, reads({"function.c", "ne.functions"}), P, {}, [] { return py::str(CODE) + py::str("|flags|") + py::join(py::str(" "), cflags16()); });

    // The object file the compiler makes of the function: {flags, text, obj} -- or, when it is rejected, {error, stand_in_text, stand_in}.
    step(py::str("function.object"), [](Context& ctx, const py::str& key) -> StepResult {
        const Json src = step_json(ctx.need(py::str("function.source"), key));
        if (src.kind != Json::Dict) return Value(j_null());
        const py::str header = jstr(step_json_str(ctx.need(py::str("ne.header"))));
        const py::str d = folder_of(ctx, jat(src, "seg").i, header);
        const py::dict<py::str, py::str> env = build::watcom::ow_env(py::str("ne"));
        const py::str hk = base::cache::key({base::cache::Part(header)});
        const py::list<py::str> flags = flags_of(jat(src, "flags"));
        const py::str text = jstr(jat(src, "text")), stem = jstr(jat(src, "stem"));
        auto read_obj = [&] {
            std::ifstream in(py::os::path::join(d, stem + py::str(".obj")).utf8(), std::ios::binary);
            return py::bytes(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>())).decode("latin1");
        };
        Json out = j_dict();
        out.dict.set(py::str("flags"), jat(src, "flags"));
        out.dict.set(py::str("text"), j_str(text));
        const py::str why = std::get<0>(build::functions::compile_kept(wcc(), build::functions::_adjust_flags(flags, text), env, d, stem, text, std::optional<py::str>(hk)));
        if (!why.empty()) {
            out.dict.set(py::str("error"), j_str(why));
            const py::str si = build::functions::stand_in(text);
            out.dict.set(py::str("stand_in_text"), j_str(si));
            out.dict.set(py::str("stand_in"), j_null());
            if (std::get<0>(build::functions::compile_kept(wcc(), build::functions::_adjust_flags(flags, si), env, d, stem, si, std::optional<py::str>(hk))).empty())
                out.dict.set(py::str("stand_in"), j_str(read_obj()));
        } else {
            out.dict.set(py::str("obj"), j_str(read_obj()));
        }
        return Value(out);
    }, reads({"function.source", "ne.header"}), P, {}, [] { return py::str(CODE) + py::str("|object|") + base::cache::tool(wcc()); });

    // The variables the function reads and never sets (functions.never_set: the compiler's flow check), with the options and text it was asked of.
    step(py::str("function.never_set"), [](Context& ctx, const py::str& key) -> StepResult {
        const Json src = step_json(ctx.need(py::str("function.source"), key));
        if (src.kind != Json::Dict) return Value(j_null());
        const py::str header = jstr(step_json_str(ctx.need(py::str("ne.header"))));
        const py::str d = folder_of(ctx, jat(src, "seg").i, header);
        py::str stem = py::str("n") + key.replace(py::str(":"), py::str("_"));         // (only its scratch file's name)
        const py::str text = jstr(jat(src, "text"));
        Json out = j_dict();
        py::list<py::str> fl = cflags16();
        out.dict.set(py::str("flags"), strs_json(fl));
        out.dict.set(py::str("text"), j_str(text));
        out.dict.set(py::str("names"), strs_json(build::functions::never_set_kept(wcc(), fl, build::watcom::ow_env(py::str("ne")), d, stem, text,
                                                                                std::optional<py::str>(base::cache::key({base::cache::Part(header)})))));
        return Value(out);
    }, reads({"function.source", "ne.header"}), P, {}, [] { return py::str(CODE) + py::str("|never|") + base::cache::tool(wcc()) + py::str("|") + py::join(py::str(" "), cflags16()); });
    // The description's lines for the function and each it calls -- the gate answers a call as the description says the callee does.
    step(py::str("function.contract"), [](Context& ctx, const py::str& key) -> StepResult {
        const py::str name = jstr(jat(jat(step_json(ctx.need(py::str("ne.functions"))), key.utf8().c_str()), "name"));
        const Json lines = step_json(ctx.need(py::str("ne.lines")));
        const Json c = step_json(ctx.need(py::str("function.c"), key));
        py::set<py::str> called;
        called.add(name);
        if (c.kind == Json::Str) {
            static const py::re::Pattern call = py::re::compile(py::str("\\b([A-Za-z_]\\w*)\\s*\\("));
            for (const py::str& n : call.findall(c.s).vec()) called.add(n);
        }
        std::vector<py::str> got;
        for (const py::str& n : called)
            if (const auto l = lines.dict.get(n)) got.push_back(l->s);
        std::sort(got.begin(), got.end(), sort_less);
        Json out = j_list();
        for (const py::str& g : got) out.list.push_back(j_str(g));
        return Value(out);
    }, reads({"function.c", "ne.functions", "ne.lines"}), P);

    // The function's C run beside the original's (verify/gate16/isolated16): ["equivalent", ""] or ["divergent", how]. Made in a build
    // (the hint: its folder and the gate's tables), so it is asked for by ne.image; one that could not be run is not kept.
    struct GateHint {
        py::str folder;
        verify::gate16::isolated16::Tables tables;
        py::dict<py::str, py::str> names;            // key -> name
    };
    batch_step(py::str("function.gate"), [](const py::dict<py::str, Context*>& ctxs, const std::any& hint) -> py::dict<py::str, StepResult> {
        for (const auto& kv : ctxs.items()) {
            Context& c = *std::get<1>(kv);
            c.need(py::str("binary"));
            c.need(py::str("ne.header"));
            c.need(py::str("function.c"), std::get<0>(kv));
            c.need(py::str("function.contract"), std::get<0>(kv));
        }
        const GateHint* h = std::any_cast<GateHint>(&hint);
        if (!h) throw py::LookupError("a gate verdict is made in a build of the module: ask for ne.image");
        py::dict<py::str, StepResult> out;
        const auto its = ctxs.items();
        if (its.empty()) return out;
        const py::str path = jstr(jat(step_json(std::get<1>(its[0])->need(py::str("binary"))), "path"));
        py::dict<py::str, py::str> by_name;
        for (const auto& kv : ctxs.items()) by_name.set(h->names.at(std::get<0>(kv)), std::get<0>(kv));
        std::vector<py::str> sorted;
        for (const py::str& n : by_name.keys()) sorted.push_back(n);
        std::sort(sorted.begin(), sorted.end(), sort_less);
        py::list<py::str> names;
        for (const py::str& n : sorted) names.append(n);
        auto verdict = [](const py::str& v, const py::str& why) {
            Json r = j_list();
            r.list = {j_str(v), j_str(why)};
            return r;
        };
        for (const auto& c : verify::gate16::isolated16::check(path, h->folder, names, std::nullopt, std::nullopt, std::nullopt, h->tables)) {
            if (!by_name.contains(c.name)) continue;
            const py::str k = by_name.at(c.name);
            if (c.verdict == py::str(V::EQUIVALENT)) out.set(k, StepResult(Value(verdict(V::EQUIVALENT, py::str("")))));
            else if (c.verdict == py::str(V::DIVERGENT)) out.set(k, StepResult(Value(verdict(V::DIVERGENT, c.why))));
            else out.set(k, StepResult(Unkept{Value(verdict(V::INCONCLUSIVE, c.why))}));
        }
        for (const auto& kv : ctxs.items())
            if (!out.contains(std::get<0>(kv))) out.set(std::get<0>(kv), StepResult(Unkept{Value(verdict(V::INCONCLUSIVE, py::str("the checker gave no answer")))}));
        return out;
    }, reads({"binary", "ne.header", "function.c", "function.contract"}), P, {}, gate_stamp);
    // (the hint type is named again where ne.image hands it over)

    // The module linked: {image, report, gated, gate_set, rejected}: module.build's build of a 16-bit module, made of the facts above.
    step(py::str("ne.image"), [](Context& ctx, const py::str&) -> StepResult {
        const py::str path = jstr(jat(step_json(ctx.need(py::str("binary"))), "path"));
        const Json funcs = step_json(ctx.need(py::str("ne.functions")));
        std::vector<py::str> keys;
        for (const auto& kv : funcs.dict.items())
            if (jat(std::get<1>(kv), "seg").kind == Json::Int) keys.push_back(std::get<0>(kv));
        std::sort(keys.begin(), keys.end(), sort_less);
        py::list<py::str> klist;
        for (const py::str& k : keys) klist.append(k);
        const auto texts = ctx.need_many(py::str("function.c"), klist);
        const auto objects = ctx.need_many(py::str("function.object"), klist);
        const auto never = ctx.need_many(py::str("function.never_set"), klist);
        ctx.file(mod::KEPT_AS_ASM());
        const py::str folder = ctx.work(py::list<py::str>{py::str("build")});
        const py::str decompiled = py::os::path::join(folder, py::str("decompiled"));
        py::os::makedirs(decompiled, 0777, true);
        const Json desc = step_json(ctx.need(py::str("ne.description")));
        py::list<py::str> dlines;
        for (const Json& l : desc.list) dlines.append(l.s);
        py::dict<py::str, py::str> key_of;                                  // name -> key
        for (const auto& kv : funcs.dict.items()) key_of.set(jstr(jat(std::get<1>(kv), "name")), std::get<0>(kv));
        // (_copy(m): the module again, as the lane makes it, the description and the C as the facts now are)
        auto fresh = [&]() {
            auto m = prepare_into(path, decompiled);
            if (m->description) {
                m->description->lines = dlines;
                m->description->write(py::os::path::join(decompiled, py::str("description.txt")));
            }
            for (const py::str& k : klist) {
                const Json t = step_json(texts.at(k));
                if (t.kind == Json::Str) m->set_text(jstr(jat(funcs.dict.at(k), "name")), t.s);
            }
            return m;
        };
        // the verdicts already known, of the functions the gate ran last time (module._gate16_known)
        py::dict<py::str, py::str> gated;
        if (const auto prev = ctx.previous()) {
            const Json p = step_json(*prev);
            for (const Json& n : jat(p, "gate_set").list) {
                if (!key_of.contains(n.s)) continue;
                const auto v = ctx.peek(py::str("function.gate"), key_of.at(n.s));
                if (v) {
                    const Json vj = step_json(*v);
                    if (vj.kind == Json::List && vj.list.size() >= 2 && vj.list[0].s == py::str(V::DIVERGENT)) gated.set(n.s, vj.list[1].s);
                }
            }
        }
        bool gate_now = !py::os::environ_::get(py::str("CB_NO_GATE"));
        std::vector<py::str> gate_set;
        mod::Built b(path);
        std::optional<py::str> image;
        {
            auto m0 = fresh();
            verify::gate16::isolated16::Tables tables;
            tables.funcs = verify::gate16::isolated16::_functions(*m0);
            for (const py::str& w : m0->wide) tables.wide.append(w);
            std::vector<py::str> w;
            for (const py::str& x : tables.wide) w.push_back(x);
            std::sort(w.begin(), w.end(), sort_less);
            tables.wide = py::list<py::str>();
            for (const py::str& x : w) tables.wide.append(x);
            for (const auto& kv : m0->by_va().items()) {
                const auto wh = m0->where.get(std::get<0>(kv));
                if (wh && *wh) tables.place.set(std::get<1>(kv), *(*wh));
            }
            const int64_t functions = py::len(m0->all_keys);
            // module._build16 handed the engine's objects and flow checks while it runs (functions.serve)
            py::dict<py::str, build::functions::Served> served_objects;
            py::dict<py::str, py::list<py::str>> served_never;
            {
                const py::str hk = base::cache::key({base::cache::Part(m0->header())});
                for (const py::str& k : klist) {
                    const Json o = step_json(objects.at(k));
                    if (o.kind == Json::Dict) {
                        if (jat(o, "error").kind == Json::Str) {
                            served_objects.set(build::functions::served_key(hk, flags_of(jat(o, "flags")), jstr(jat(o, "text"))), build::functions::Served{jstr(jat(o, "error")), py::bytes()});
                            if (jat(o, "stand_in").kind == Json::Str)
                                served_objects.set(build::functions::served_key(hk, flags_of(jat(o, "flags")), jstr(jat(o, "stand_in_text"))),
                                                   build::functions::Served{py::str(""), jstr(jat(o, "stand_in")).encode("latin1")});
                        } else {
                            served_objects.set(build::functions::served_key(hk, flags_of(jat(o, "flags")), jstr(jat(o, "text"))),
                                               build::functions::Served{py::str(""), jstr(jat(o, "obj")).encode("latin1")});
                        }
                    }
                    const Json nv = step_json(never.at(k));
                    if (nv.kind == Json::Dict) served_never.set(build::functions::served_key(hk, flags_of(jat(nv, "flags")), jstr(jat(nv, "text"))), flags_of(jat(nv, "names")));
                }
            }
            struct Serving {
                Serving(const py::dict<py::str, build::functions::Served>& o, const py::dict<py::str, py::list<py::str>>& nv) { build::functions::serve(o, nv); }
                ~Serving() { build::functions::unserve(); }
            };
            while (true) {
                b = mod::Built(path);
                b.gated = gated.copy();
                b.folder = folder;
                b.functions = functions;
                // (_copy(m): _build16 adds to its segments -- and to nothing else -- so a pass gets its own dict of them, not a second decompile)
                auto mm = std::make_shared<lane::Prepared>(*m0);
                mm->segments = m0->segments.copy();
                {
                    Serving serving(served_objects, served_never);
                    image = mod::_build16(b, path, *mm, folder);
                }
                if (!b.resident_overflow.empty()) {                       // (as module.build: again -- and gated still, if this build was to be)
                    py::dict<py::str, py::str> merged = b.gated.copy();
                    for (const auto& kv : b.resident_overflow.items()) merged.set(std::get<0>(kv), std::get<1>(kv));
                    gated = merged;
                    continue;
                }
                if (!image || image->empty()) break;
                b.image = py::os::path::join(folder, py::os::path::basename(path).lower());
                py::shutil::copyfile(*image, *b.image);
                mod::_whole_file_image(path, *b.image);
                if (!gate_now) break;
                gate_now = false;
                std::set<std::string> cset;
                for (const py::str& n : b.placed_c) cset.insert(n.utf8());
                for (const py::str& n : b.moved_c) cset.insert(n.utf8());
                py::list<py::str> c;
                std::vector<py::str> cv;
                for (const auto& n : cset) cv.push_back(py::str(n));
                std::sort(cv.begin(), cv.end(), sort_less);
                for (const py::str& n : cv) c.append(n);
                py::list<py::str> gkeys;
                GateHint hint;
                hint.folder = folder;
                hint.tables = tables;
                for (const py::str& n : c)
                    if (key_of.contains(n)) {
                        gkeys.append(key_of.at(n));
                        hint.names.set(key_of.at(n), n);
                    }
                const auto verdicts = ctx.need_many(py::str("function.gate"), gkeys, std::any(hint));
                py::dict<py::str, py::str> differ;
                std::vector<std::pair<py::str, py::str>> failed;
                for (const py::str& n : c) {
                    if (!key_of.contains(n)) continue;
                    const Json v = step_json(verdicts.at(key_of.at(n)));
                    const py::str word = V::verdict(std::optional<py::str>(v.list[0].s)).value_or(v.list[0].s);
                    if (word == py::str(V::DIVERGENT)) differ.set(n, v.list[1].s);
                    if (word == py::str(V::INCONCLUSIVE) && v.list[1].s.startswith(py::str("the checker failed"))) failed.emplace_back(n, v.list[1].s);
                }
                if (!failed.empty()) b.gate_error = py::percent("%d functions (%s)", static_cast<int64_t>(failed.size()), failed[0].second.slice(0, 120));
                try {
                    for (const auto& kv : verify::gate16::contract16::mismatches(path, folder, *b.image, c).items())
                        if (!differ.contains(std::get<0>(kv))) differ.set(std::get<0>(kv), py::str("its calls are not the original's: ") + std::get<1>(kv));
                } catch (const py::Exception& e) {
                    b.contract_error = py::percent("%s: %s", py::str(e.type()), py::str(e.what()));
                } catch (const std::exception& e) {
                    b.contract_error = py::percent("Exception: %s", py::str(e.what()));
                }
                std::set<std::string> all = cset;
                for (const py::str& n : gated.keys()) all.insert(n.utf8());
                gate_set.clear();
                for (const auto& n : all) gate_set.push_back(py::str(n));
                std::sort(gate_set.begin(), gate_set.end(), sort_less);
                if (!differ.empty()) {
                    gated = gated.copy();
                    for (const auto& kv : differ.items()) gated.set(std::get<0>(kv), std::get<1>(kv));
                    continue;
                }
                break;
            }
        }
        Json out = j_dict();
        out.dict.set(py::str("name"), j_str(b.name));
        Json g = j_dict();
        for (const auto& kv : b.gated.items()) g.dict.set(std::get<0>(kv), j_str(std::get<1>(kv)));
        out.dict.set(py::str("gated"), g);
        Json gs = j_list();
        for (const py::str& n : gate_set) gs.list.push_back(j_str(n));
        out.dict.set(py::str("gate_set"), gs);
        Json rj = j_dict();
        for (const auto& kv : b.rejected.items()) rj.dict.set(std::get<0>(kv), j_str(std::get<1>(kv)));
        out.dict.set(py::str("rejected"), rj);
        Json img = j_null();
        if (b.image) {
            mod::check(b);
            std::ifstream in(b.image->utf8(), std::ios::binary);
            const std::string raw((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            img = j_str(py::bytes(raw).decode("latin1"));
        }
        out.dict.set(py::str("image"), img);
        out.dict.set(py::str("report"), j_str(b.report()));
        {
            std::lock_guard<std::mutex> lk(g_lock);
            g_built[path.utf8()] = std::make_shared<mod::Built>(b);
        }
        return Value(out);
    }, reads({"binary", "ne.module", "ne.description", "ne.functions", "function.c", "function.object", "function.never_set", "function.gate"}), P, {}, image_stamp);
}

}  // namespace

void declare() {
    static std::once_flag once;
    std::call_once(once, declare_steps);
}

bool handles(const py::str& path) { return platforms::win16::recognises(path); }

std::shared_ptr<mod::Built> build(const py::str& module_name, const std::optional<py::str>& outdir, bool call,
                                  const std::function<void(const py::str&)>& log0) {
    declare();
    const std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    const auto found = base::corpus::retail(module_name);
    if (!found || found->empty()) throw py::SystemExit(py::percent("no module %s in the corpus", py::repr(module_name)));
    const py::str path = *found;
    const double t = py::time::time();
    auto db = engine::open(path);
    log(py::percent("%s: on the engine (%s)", py::os::path::basename(path).lower(), db->path));
    const Json got = step_json(engine::get(db, py::str("ne.image"), py::str(""), log));
    const auto made = engine::last_run(*db);
    py::list<py::str> parts;
    std::vector<py::str> ks;
    for (const py::str& k : made.keys()) ks.push_back(k);
    std::sort(ks.begin(), ks.end(), sort_less);
    for (const py::str& k : ks) parts.append(py::percent("%s %d/%d", k, made.at(k)[0], made.at(k)[1]));
    log(py::percent("%s: %.1fs; facts kept/made: %s", py::os::path::basename(path).lower(), py::time::time() - t, py::join(py::str(", "), parts)));
    std::shared_ptr<mod::Built> b;
    {
        std::lock_guard<std::mutex> g(g_lock);
        const auto it = g_built.find(path.utf8());
        if (it != g_built.end()) b = it->second;
    }
    if (!b) b = std::make_shared<mod::Built>(path);
    const py::str folder = facts::work(*db, py::list<py::str>{py::str("build")});
    if (jat(got, "image").kind == Json::Str && !jstr(jat(got, "image")).empty()) {
        const py::str where = outdir ? *outdir : folder;
        py::os::makedirs(where, 0777, true);
        b->image = py::os::path::join(where, py::os::path::basename(path).lower());
        py::file f = py::open(*b->image, py::str("wb"));
        f.write(jstr(jat(got, "image")).encode("latin1"));
        f.close();
        b->folder = folder;
    } else {
        b->image = std::nullopt;
    }
    if (call && b->image) mod::run(*b);
    facts::hold();
    return b;
}

}  // namespace codebreaker::engine::win16
