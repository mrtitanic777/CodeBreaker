// codebreaker/ui/mcp_server.cpp -- typed C++ port of codebreaker.ui.mcp_server (see mcp_server.hpp).
#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <set>
#include <sstream>
#include <thread>

#include "codebreaker/__main__.hpp"
#include "codebreaker/analyze/call_graph.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/image_bridge.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/exact/aim.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/source/placement.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/demangle.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "codebreaker/ui/mcp_server.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

namespace codebreaker::ui::mcp_server {

namespace service_ = codebreaker::ui::service;
namespace F = codebreaker::store::functions;
namespace V = codebreaker::verify::verdicts;
namespace A = codebreaker::analyze;
namespace I = codebreaker::analyze::image;
namespace corpus = codebreaker::base::corpus;
using service_::Dump;
using service_::dumps;
using service_::loads;
using service_::quiet;

const char* const INSTRUCTIONS =
    R"DOC(CodeBreaker reconstructs readable source from binaries (Windows 16/32-bit, DOS, VxD
drivers, Xbox, Xbox 360, 8/16-bit consoles, XNA, Java) and checks that the source behaves like the
original by running both in an emulator. Start with `triage` on a module (everything below at a
glance), `describe` on a file or `modules` for the corpus; `inspect` for headers/imports/exports;
`functions` and `function` for what the store holds (each function's reconstructed C and its verdict:
verified, weak, unverified, not-judged, no-compile, no-source); `decompile_function` for one function's
C now; `disasm` for its instructions, `xrefs` for who calls it and what it calls, `hex_dump` by address.
What was made lives in `projects`/`project` (Converted/), `source_tree` (the product's source tree)
and `reach_results`; `read_file` reads any of it. `compare_versions` matches two builds function by
function, `diff_function` shows one pair side by side. Anything long (decompile, measure, reach
--judge, pe-source, recompile-project) goes through `run_command`, then `job` to follow it.
A module is a corpus name (lz32.dll) or a path; an address is hex, or a function's name.)DOC";

const std::vector<py::str>& INTERACTIVE() {
    static const std::vector<py::str> v = {py::str("ui"), py::str("progress"), py::str("mcp")};
    return v;
}

Hooks& hooks() {
    static Hooks h;
    return h;
}

// ---- small helpers -----------------------------------------------------------------------------------
namespace {

std::string hx(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v));
    return b;
}
std::string hx5(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%05llx", static_cast<unsigned long long>(v));
    return b;
}
// str.lower() of UTF-8 text
std::string lower8(const std::string& s) { return py::str(s).lower().utf8(); }
// `needle in text`
bool contains8(const std::string& text, const std::string& needle) { return text.find(needle) != std::string::npos; }

[[noreturn]] void no_hook(const char* what) {
    throw py::RuntimeError(std::string("mcp_server: ") + what + " has no typed port yet: set hooks()." + what);
}

// bool(x) of a JSON value
bool truthy(const Json* j) {
    if (!j) return false;
    switch (j->kind) {
        case Json::Null: return false;
        case Json::Bool: return j->b;
        case Json::Int: return j->i != 0;
        case Json::Float: return j->d != 0.0;
        case Json::Str: return !j->s.empty();
        default: return !j->a.empty();
    }
}
int64_t as_int(const Json* j, int64_t dflt = 0) {
    if (!j) return dflt;
    if (j->kind == Json::Int) return j->i;
    if (j->kind == Json::Bool) return j->b ? 1 : 0;
    if (j->kind == Json::Float) return static_cast<int64_t>(j->d);
    return dflt;
}

std::string type_name_(const std::exception& e) {
    if (auto p = dynamic_cast<const py::Exception*>(&e)) return p->type();
    return "Exception";
}

// the Python repr of a JSON value (what pydantic prints as input_value)
std::string py_repr(const Json& v) {
    switch (v.kind) {
        case Json::Null: return "None";
        case Json::Bool: return v.b ? "True" : "False";
        case Json::Int: return std::to_string(v.i);
        case Json::Float: return py::to_str(v.d).utf8();
        case Json::Str: return py::repr(py::str(v.s)).utf8();
        case Json::Arr: {
            std::string o = "[";
            for (size_t n = 0; n < v.a.size(); n++) o += (n ? ", " : "") + py_repr(v.a[n]);
            return o + "]";
        }
        default: {
            std::string o = "{";
            for (size_t n = 0; n < v.a.size(); n++) o += (n ? ", " : "") + py::repr(py::str(v.keys[n])).utf8() + ": " + py_repr(v.a[n]);
            return o + "}";
        }
    }
}
const char* py_type_(const Json& v) {
    switch (v.kind) {
        case Json::Null: return "NoneType";
        case Json::Bool: return "bool";
        case Json::Int: return "int";
        case Json::Float: return "float";
        case Json::Str: return "str";
        case Json::Arr: return "list";
        default: return "dict";
    }
}

}  // namespace

// ---- plumbing ------------------------------------------------------------------------------------------
namespace {

double now_() { return py::time::time(); }

// Values that take long to work out, worked out in a thread and remembered for a while.
}  // namespace

template <class T> class Slow {
  public:
    using Work = std::function<T()>;
    using MakeError = std::function<T(const std::string&)>;
    explicit Slow(MakeError e) : error_(std::move(e)) {}

    // `work()`'s value if known and younger than `age` seconds, else start it and wait up to `wait` seconds.
    // -> (value or none, seconds it has been working when not ready)
    std::pair<std::optional<T>, int64_t> get(const std::string& key, const Work& work, double age = 600.0, double wait = 20.0,
                                             bool again = false) {
        std::shared_ptr<Run> run;
        {
            std::lock_guard<std::mutex> g(lock_);
            auto it = got_.find(key);
            if (it != got_.end() && !again && now_() - it->second.when < age) return {it->second.value, 0};
            auto w = working_.find(key);
            if (w == working_.end()) {
                run = std::make_shared<Run>();
                run->started = now_();
                working_[key] = run;
                std::thread([this, key, work, run] {
                    T value;
                    try {
                        value = work();
                    } catch (const std::exception& e) {   // remembered, said once
                        std::string m = e.what();
                        value = error_(type_name_(e) + ": " + m.substr(0, 300));
                    }
                    std::lock_guard<std::mutex> g2(lock_);
                    got_[key] = Entry{now_(), value};
                    working_.erase(key);
                    {
                        std::lock_guard<std::mutex> g3(run->m);
                        run->done = true;
                    }
                    run->cv.notify_all();
                }).detach();
            } else {
                run = w->second;
            }
        }
        {
            std::unique_lock<std::mutex> u(run->m);
            run->cv.wait_for(u, std::chrono::duration<double>(wait), [&] { return run->done; });
        }
        std::lock_guard<std::mutex> g(lock_);
        auto it = got_.find(key);
        if (it != got_.end() && run->done) return {it->second.value, 0};
        return {std::nullopt, static_cast<int64_t>(now_() - run->started)};
    }

  private:
    struct Run {
        std::mutex m;
        std::condition_variable cv;
        bool done = false;
        double started = 0;
    };
    struct Entry {
        double when;
        T value;
    };
    std::mutex lock_;
    std::map<std::string, Entry> got_;
    std::map<std::string, std::shared_ptr<Run>> working_;
    MakeError error_;
};

namespace {

Json error_json_(const std::string& m) {
    Json j = Json::object();
    j.set("error", m);
    return j;
}
Slow<Json>& SLOW() {
    static Slow<Json> s(error_json_);
    return s;
}

// The module as the analyses see it (analyze/image.py), read once and kept an hour; or what went wrong.
struct ImageSlot {
    I::ImageP img;
    std::string error;
};
Slow<ImageSlot>& SLOW_IMAGE() {
    static Slow<ImageSlot> s([](const std::string& m) { return ImageSlot{nullptr, m}; });
    return s;
}
// {addr hex: library}; empty on error
struct IdentSlot {
    std::map<std::string, std::string> found;
};
Slow<IdentSlot>& SLOW_IDENT() {
    static Slow<IdentSlot> s([](const std::string&) { return IdentSlot{}; });
    return s;
}
struct VersionsSlot {
    std::shared_ptr<VersionsResult> value;
    std::string error;
};
Slow<VersionsSlot>& SLOW_VERSIONS() {
    static Slow<VersionsSlot> s([](const std::string& m) { return VersionsSlot{nullptr, m}; });
    return s;
}

}  // namespace

Json _not_ready(const py::str& what, int64_t seconds) {
    Json j = Json::object();
    j.set("status", "computing");
    j.set("what", what);
    j.set("seconds_so_far", seconds);
    j.set("say", "still being worked out in the background; call again in a little while");
    return j;
}

py::str _path(const py::str& module_in) {
    py::str module = module_in.strip().strip(py::str("\""));
    if (module.empty()) throw py::ValueError("name a module (a corpus name such as lz32.dll, or a path)");
    std::optional<py::str> path;
    if (py::os::path::isdir(module)) path = module;
    else {
        path = corpus::retail(module);
        if (!path || path->empty()) path = service_::stored_path(module);
    }
    if (!path || path->empty())
        throw py::ValueError("no module " + py::repr(module).utf8() + ": not a file, not in the corpus (" + corpus::root().utf8() +
                             "), not in the store");
    return *path;
}

Json _fit(const Json& value, int64_t most) {
    Dump d;
    std::string text = dumps(value, d);
    if (static_cast<int64_t>(text.size()) <= most) return value;
    if (value.is_obj()) {
        std::vector<int64_t> sizes;
        for (const auto& v : value.a) sizes.push_back(static_cast<int64_t>(dumps(v, d).size()));
        Json keep = Json::object();
        for (size_t n = 0; n < value.a.size(); n++)
            if (sizes[n] < 2000) keep.set(value.keys[n], value.a[n]);
        Json cut = Json::object();
        cut.set("say", "too long for one answer; the long parts are named with their sizes. Ask for one part (a section, or offset/limit).");
        Json parts = Json::object();
        for (size_t n = 0; n < value.a.size(); n++)
            if (!keep.has(value.keys[n])) parts.set(value.keys[n], sizes[n]);
        cut.set("parts", parts);
        keep.set("_cut", cut);
        return keep;
    }
    Json j = Json::object();
    j.set("_cut", "too long for one answer (" + std::to_string(py::str(text).size()) + " characters)");
    j.set("start", py::str(text).u32().substr(0, static_cast<size_t>(most)).empty() ? std::string() : text.substr(0, static_cast<size_t>(most)));
    return j;
}

Json _page(const std::vector<Json>& rows, int64_t offset, int64_t limit, int64_t most) {
    offset = std::max<int64_t>(0, offset);
    limit = std::max<int64_t>(1, std::min(limit, most));
    const int64_t n = static_cast<int64_t>(rows.size());
    const int64_t lo = std::min(offset, n), hi = std::min(offset + limit, n);
    Json out = Json::object();
    out.set("total", n);
    out.set("offset", offset);
    out.set("count", hi - lo);
    Json r = Json::array();
    for (int64_t k = lo; k < hi; k++) r.push(rows[static_cast<size_t>(k)]);
    out.set("rows", r);
    out.set("next", offset + limit < n ? Json(offset + limit) : Json());
    return out;
}

py::str _addr(const py::str& text) {
    py::str a = text.strip().lower();
    if (a.startswith(py::str("0x"))) return py::str(a.utf8().substr(2));
    if (a.endswith(py::str("h")) && a.size() > 1) return a.rstrip(py::str("h"));
    return a;
}

namespace {

std::vector<Json> rows_of_(const Json& arr) { return arr.a; }

// service.functions for a module, remembered a few seconds (it reads the ledger).
Json store_view_(const py::str& module) {
    auto got = SLOW().get("functions\x01" + module.utf8(), [module] { return quiet([&] { return service_::functions(module); }); }, 15.0, 120.0);
    return got.first ? *got.first : Json();
}

I::ImageP image_(const py::str& p) {
    auto got = SLOW_IMAGE().get("image\x01" + p.utf8(), [p] { return ImageSlot{quiet([&] { return I::load(p); }), ""}; }, 3600.0, 120.0);
    if (!got.first)
        throw py::ValueError("still reading " + py::os::path::basename(p).utf8() + " (" + std::to_string(got.second) + " s so far); call again");
    if (!got.first->error.empty()) throw py::ValueError(got.first->error);
    return got.first->img;
}

// "KERNEL32!#17" as "KERNEL32!CommonUnimpStub (#17)" when the number has a known name.
py::str import_name_(const py::str& text) {
    const std::string t = text.utf8();
    auto bang = t.find('!');
    if (bang == std::string::npos) return text;
    std::string mod = t.substr(0, bang), nm = t.substr(bang + 1);
    if (nm.size() > 1 && nm[0] == '#' && nm.find_first_not_of("0123456789", 1) == std::string::npos) {
        int64_t o = std::stoll(nm.substr(1));
        auto real = symbols::ordinals::name(py::str(mod), o);
        std::optional<py::str> shown;
        if (real && !real->empty()) shown = real;
        else if (auto doc = symbols::ordinals::documented(py::str(mod), o)) shown = std::get<0>(*doc);
        if (shown) return py::str(mod + "!" + shown->utf8() + " (#" + std::to_string(o) + ")");
    }
    return text;
}

bool all_hex_(const std::string& s) { return !s.empty() && s.find_first_not_of("0123456789abcdef") == std::string::npos; }

// The address `text` stands for in an image: hex (0x optional; an RVA below a 32-bit module's base counts from the base),
// segment:offset in a 16-bit module, or a name.
int64_t place_(I::Image& img, const py::str& text) {
    std::string a = _addr(text).utf8();
    auto colon = a.find(':');
    if (colon != std::string::npos && all_hex_(a.substr(0, colon)) && all_hex_(a.substr(colon + 1)) && img.bits == 16) {
        return decompile::describe::address(std::stoll(a.substr(0, colon), nullptr, 16), std::stoll(a.substr(colon + 1), nullptr, 16));
    }
    if (all_hex_(a)) {
        int64_t va = std::stoll(a, nullptr, 16);
        return img.flat() && va < img.base ? va + img.base : va;
    }
    const std::string low = lower8(text.strip().utf8());
    for (int pass = 0; pass < 2; pass++) {
        std::vector<std::pair<int64_t, std::string>> table;
        const auto& d = pass == 0 ? img.names : img.exports;
        for (const auto& kv : d.items()) table.emplace_back(std::get<0>(kv), std::get<1>(kv).utf8());
        std::sort(table.begin(), table.end());
        for (const auto& kv : table)
            if (!kv.second.empty() && lower8(kv.second) == low) return kv.first;
    }
    // names given elsewhere: the store, other modules
    auto got = quiet([&] { return symbols::names::of_module(img.path); });
    for (const auto& f : got.functions)
        if (!f.name.empty() && lower8(f.name.utf8()) == low) return f.address;
    throw py::ValueError("no address or function named " + py::repr(text).utf8() + " in " + img.name.utf8() +
                         " (an address in hex, or a name `names` lists)");
}

// Where a 32-bit module's address is in its file, or None (not stored there, or not a PE).
std::optional<int64_t> file_offset_(const py::str& p, int64_t va) {
    auto lane = corpus::lane(p);
    if (!lane || *lane != py::str("pe")) return std::nullopt;
    auto pe = formats::pe::open_pe(p);
    const int64_t base = static_cast<int64_t>(pe.image_base);
    return va >= base ? pe._rva_to_off(va - base) : std::nullopt;
}

// The inspector's description with addresses as hex text and by-number imports named. File offsets and sizes stay numbers.
void hex_addresses_(Json& info) {
    auto h = [](Json& v) {
        if (v.kind == Json::Int) v = Json("0x" + hx(v.i));
    };
    auto fix_part = [&](Json* part) {
        if (!part || !part->is_obj()) return;
        for (const char* k : {"image_base", "entry", "entry_point", "code_base", "data_base"})
            if (Json* v = part->find_mut(k)) h(*v);
    };
    fix_part(info.find_mut("summary"));
    if (Json* hd = info.find_mut("headers"))
        if (hd->is_obj()) fix_part(hd->find_mut("optional"));
    if (Json* secs = info.find_mut("sections"))
        for (auto& s : secs->a)
            if (s.is_obj()) {
                Json v = s.find("address") ? *s.find("address") : Json();
                h(v);
                s.set("address", v);
            }
    if (Json* ex = info.find_mut("exports"))
        if (ex->is_obj())
            if (Json* syms = ex->find_mut("symbols"))
                for (auto& s : syms->a)
                    if (s.is_obj()) {
                        Json v = s.find("address") ? *s.find("address") : Json();
                        h(v);
                        s.set("address", v);
                    }
    if (Json* imps = info.find_mut("imports"))
        for (auto& imp : imps->a) {
            if (!imp.is_obj()) continue;
            const Json* mod = imp.find("module");
            py::str stem = std::get<0>(py::os::path::splitext(mod && mod->is_str() ? mod->str() : py::str("")));
            Json* syms = imp.find_mut("symbols");
            if (!syms) continue;
            for (auto& s : syms->a) {
                if (!s.is_obj()) continue;
                Json slot = s.find("slot") ? *s.find("slot") : Json();
                h(slot);
                s.set("slot", slot);
                const Json* name = s.find("name");
                const Json* ord = s.find("ordinal");
                if (!truthy(name) && ord && ord->kind == Json::Int && ord->i >= 0) {
                    auto real = symbols::ordinals::name(stem, ord->i);
                    std::optional<std::tuple<py::str, py::str>> doc;
                    if (!real || real->empty()) doc = symbols::ordinals::documented(stem, ord->i);
                    if ((real && !real->empty()) || doc) {
                        s.set("name", real && !real->empty() ? *real : std::get<0>(*doc));
                        s.set("named_from", real && !real->empty() ? symbols::ordinals::source(stem, ord->i) : std::get<1>(*doc));
                    }
                }
            }
        }
}

// {addr hex: library} of a module's functions a runtime library or tool made (identify).
std::map<std::string, std::string> identified_(const py::str& p) {
    auto got = SLOW_IDENT().get("identify\x01" + p.utf8(), [p] {
        IdentSlot out;
        auto found = quiet([&] { return symbols::runtime_library::identify(p); });
        for (const auto& kv : found.items()) out.found[hx(std::get<0>(kv))] = std::get<1>(std::get<1>(kv)).utf8();
        return out;
    }, 3600.0, 120.0);
    return got.first ? got.first->found : std::map<std::string, std::string>{};
}

}  // namespace

// ---- reading -----------------------------------------------------------------------------------------
Json describe(const py::str& path) {
    py::str p = _path(path);
    if (py::os::path::isdir(p)) {
        Json j = Json::object();
        j.set("path", p);
        j.set("found", "a folder");
        j.set("say", "a folder: use reach through run_command to go through it");
        return j;
    }
    if (!hooks().reach_describe) no_hook("reach_describe");
    if (!hooks().inspect_describe) no_hook("inspect_describe");
    Json out = quiet([&] { return hooks().reach_describe(p); });
    out.set("path", p);
    out.set("size", py::os::path::getsize(p));
    Json info = quiet([&] { return hooks().inspect_describe(p); });
    Json wrap = Json::object();
    const Json* sm = info.find("summary");
    wrap.set("summary", sm ? *sm : Json::object());
    hex_addresses_(wrap);
    out.set("summary", wrap.at("summary"));
    return out;
}

Json inspect_module(const py::str& module, const py::str& section) {
    py::str p = _path(module);
    if (py::os::path::isdir(p)) {
        if (!hooks().java_discover) no_hook("java_discover");
        Json j = Json::object();
        j.set("path", p);
        j.set("kind", "java");
        j.set("classes", static_cast<int64_t>(hooks().java_discover(p).size()));
        return j;
    }
    if (!hooks().inspect_describe) no_hook("inspect_describe");
    Json info = quiet([&] { return hooks().inspect_describe(p); });
    hex_addresses_(info);
    if (!section.empty()) {
        if (!info.has(section.utf8())) {
            std::string have;
            for (const auto& k : info.keys) have += (have.empty() ? "" : ", ") + k;
            throw py::ValueError("no part " + py::repr(section).utf8() + "; there are: " + have);
        }
        Json one = Json::object();
        one.set(section.utf8(), info.at(section.utf8()));
        return _fit(one);
    }
    return _fit(info);
}

Json strings(const py::str& module, const py::str& find, int64_t min_len, int64_t offset, int64_t limit) {
    py::str p = _path(module);
    if (!hooks().inspect_strings) no_hook("inspect_strings");
    Json rows = quiet([&] { return hooks().inspect_strings(p, std::max<int64_t>(2, std::min<int64_t>(min_len, 64))); });
    std::vector<Json> keep;
    const std::string low = lower8(find.utf8());
    for (const auto& r : rows.a)
        if (find.empty() || contains8(lower8(r.at("text").s), low)) keep.push_back(r);
    return _page(keep, offset, limit);
}

Json hex_dump(const py::str& module, int64_t offset, int64_t length, const py::str& addr) {
    py::str p = _path(module);
    int64_t n = std::max<int64_t>(1, std::min<int64_t>(length, 16384));
    Json out = Json::object();
    if (!addr.empty()) {
        auto img = image_(p);
        int64_t va = place_(*img, addr);
        auto off = file_offset_(p, va);
        if (!off)
            throw py::ValueError(hx(va) + " is not stored in the file (or " + py::os::path::basename(p).utf8() +
                                 " is not a 32-bit module: give `offset`)");
        offset = *off;
        out.set("addr", hx(va));
    }
    out.set("offset", offset);
    out.set("length", n);
    out.set("size", py::os::path::getsize(p));
    if (!hooks().inspect_hexdump) no_hook("inspect_hexdump");
    out.set("text", hooks().inspect_hexdump(p, std::max<int64_t>(0, offset), n));
    return out;
}

ToolValue resource(const py::str& module, int64_t offset, int64_t size, int64_t type_id, bool wide) {
    py::str rp = _path(module);
    if (!hooks().inspect_resource_body) no_hook("inspect_resource_body");
    Json body = quiet([&] { return hooks().inspect_resource_body(rp, offset, size, type_id, wide); });
    ToolValue v;
    const Json* kind = body.find("kind");
    if (kind && kind->is_str() && kind->s == "image") {
        v.image = true;
        v.data = py::str(body.at("bytes").s).encode("latin1");
        const std::string ct = body.at("content_type").s;
        v.format = py::str(ct.substr(ct.rfind('/') + 1));
        return v;
    }
    v.json = _fit(body);
    return v;
}

namespace {

// The ledger, worked out, and kept on disk so a server started later has it at once.
Json ledger_() {
    auto& hk = service_::ledger_hooks();
    if (!hk.measure) no_hook("ledger (service::ledger_hooks().measure)");
    Json led = quiet([&] { return hk.measure(py::str("")); });
    if (!led.has("error")) {
        py::str path = service_::state_path({py::str("mcp"), py::str("ledger.json")});
        Json doc = Json::object();
        doc.set("time", now_());
        doc.set("ledger", led);
        {
            std::ofstream f(std::filesystem::u8path((path + py::str(".tmp")).utf8()), std::ios::binary);
            f << dumps(doc);
        }
        py::os::replace(path + py::str(".tmp"), path);
    }
    return led;
}

// (ledger, none) when it is known; (the one kept on disk, a note how old) while it is being worked out again; (none, not-ready answer)
// when there is neither.
std::pair<std::optional<Json>, Json> ledger_now_(bool refresh) {
    auto got = SLOW().get("ledger", [] { return ledger_(); }, 600.0, 20.0, refresh);
    if (got.first) return {*got.first, Json()};
    try {
        std::ifstream f(std::filesystem::u8path(service_::state_path({py::str("mcp"), py::str("ledger.json")}).utf8()), std::ios::binary);
        if (!f) throw py::OSError("no ledger");
        std::stringstream ss;
        ss << f.rdbuf();
        Json kept = loads(ss.str());
        double t = kept.at("time").kind == Json::Int ? static_cast<double>(kept.at("time").i) : kept.at("time").d;
        std::time_t tt = static_cast<std::time_t>(t);
        std::tm tm{};
        localtime_s(&tm, &tt);
        char b[64];
        std::strftime(b, sizeof b, "%Y-%m-%d %I:%M %p", &tm);
        Json note = Json::object();
        note.set("as_of", std::string(b));
        note.set("say", "the ledger kept from then; a fresh one is being worked out");
        return {kept.at("ledger"), note};
    } catch (const std::exception&) {
        return {std::nullopt, _not_ready(py::str("the coverage ledger"), got.second)};
    }
}

}  // namespace

Json modules(const py::str& kind, const py::str& find, bool only_in_store, int64_t offset, int64_t limit, bool refresh) {
    auto led = ledger_now_(refresh);
    if (!led.first) return led.second;
    std::vector<std::string> keep = {"name", "kind", "size", "in_store", "measured", "judged"};
    for (const auto& c : service_::ledger_hooks().counts) keep.push_back(c.utf8());
    std::vector<Json> rows;
    const Json* ms = led.first->find("modules");
    const std::string low = lower8(find.utf8());
    if (ms)
        for (const auto& r : ms->a) {
            const bool in_store = truthy(r.find("in_store"));
            if ((kind.empty() || r.at("kind").s == kind.utf8()) && (find.empty() || contains8(r.at("name").s, low)) && (in_store || !only_in_store)) {
                Json row = Json::object();
                for (const auto& k : keep) {
                    const Json* v = r.find(k);
                    row.set(k, v ? *v : Json());
                }
                rows.push_back(row);
            }
        }
    Json out = _page(rows, offset, limit);
    if (!led.second.is_null()) out.set("kept", led.second);
    return out;
}

Json coverage(bool refresh) {
    auto led = ledger_now_(refresh);
    if (!led.first) return led.second;
    Json out = Json::object();
    for (const char* k : {"total", "kinds", "profiles", "error"})
        if (const Json* v = led.first->find(k)) out.set(k, *v);
    if (!led.second.is_null()) out.set("kept", led.second);
    return out;
}

Json progress() {
    if (!hooks().progress_snapshot) no_hook("progress_snapshot");
    auto got = SLOW().get("progress", [] { return quiet([&] { return hooks().progress_snapshot(); }); }, 30.0);
    return got.first ? *got.first : _not_ready(py::str("the progress reading"), got.second);
}

Json functions(const py::str& module, const py::str& cls, const py::str& find, int64_t offset, int64_t limit) {
    py::str p = _path(module);
    Json found = store_view_(py::os::path::basename(p));
    if (found.is_null()) throw py::ValueError(module.utf8() + " is not a module of the corpus or the store");
    if (found.has("error") && !found.has("functions")) return found;
    auto made = identified_(p);
    std::vector<Json> rows;
    int64_t identified_n = 0;
    const std::string low = lower8(find.utf8());
    const std::string want_addr = _addr(find).utf8();
    for (const auto& r0 : found.at("functions").a) {
        Json r = r0;
        auto lib = made.find(r.at("addr").s);
        const bool has_lib = lib != made.end() && !lib->second.empty();
        if (lib != made.end() && !lib->second.empty()) identified_n++;
        if (has_lib) {   // made by a tool or a runtime library: not authored code
            r.set("library", lib->second);
            if (r.at("class").s == V::NO_COMPILE.utf8()) r.set("why", "identified, not reconstructed by design (" + lib->second + ")");
        }
        const bool class_ok = cls.empty() || r.at("class").s == cls.utf8() || (cls.utf8() == "identified" && r.has("library"));
        const bool find_ok = find.empty() || contains8(lower8(r.at("name").s), low) || contains8(r.at("addr").s, want_addr);
        if (class_ok && find_ok) rows.push_back(r);
    }
    Json out = Json::object();
    for (const char* k : {"module", "kind_title", "sha", "in_store", "measured", "counts", "reasons"}) out.set(k, found.at(k));
    Json counts = out.at("counts");
    counts.set("identified", identified_n);
    out.set("counts", counts);
    if (!truthy(found.find("in_store"))) out.set("say", "not measured yet: run_command('measure', ['" + module.utf8() + "'])");
    Json pg = _page(rows, offset, limit);
    for (size_t n = 0; n < pg.keys.size(); n++) out.set(pg.keys[n], pg.a[n]);
    return out;
}

Json function(const py::str& module, const py::str& addr) {
    py::str p = _path(module);
    py::str name = py::os::path::basename(p);
    Json found = store_view_(name);
    if (found.is_null() || !truthy(found.find("in_store")))
        throw py::ValueError(module.utf8() + " has nothing in the store yet; measure it first");
    const std::string a = _addr(addr).utf8();
    const std::vector<Json>& rows = found.at("functions").a;
    const Json* hit = nullptr;
    for (const auto& r : rows)
        if (r.at("addr").s == a) { hit = &r; break; }
    auto lstrip0 = [](const std::string& s) {
        size_t k = s.find_first_not_of('0');
        return k == std::string::npos ? std::string() : s.substr(k);
    };
    if (!hit)
        for (const auto& r : rows) {
            const std::string& ra = r.at("addr").s;
            const std::string suffix = ":" + a;
            if ((ra.size() >= suffix.size() && ra.compare(ra.size() - suffix.size(), suffix.size(), suffix) == 0) || lstrip0(ra) == lstrip0(a)) {
                hit = &r;
                break;
            }
        }
    const std::string want_name = lower8(addr.strip().utf8());
    if (!hit)
        for (const auto& r : rows)
            if (!r.at("name").s.empty() && lower8(r.at("name").s) == want_name) { hit = &r; break; }
    if (!hit)
        throw py::ValueError("no function " + py::repr(addr).utf8() + " in " + name.utf8() + " (" + std::to_string(rows.size()) +
                             " functions; `functions` lists them)");
    Json fn_answer = quiet([&] { return service_::functions(name, py::str(hit->at("addr").s)); });
    Json one = fn_answer.at("function");
    if (!truthy(one.find("source"))) {   // not judged under these settings: what was made of it
        auto fs = F::open();
        struct Closer {
            decltype(fs) fs;
            ~Closer() { if (fs) fs->close(); }
        } closer{fs};
        Json made = Json::array();
        for (const auto& c : fs->stored(found.at("sha").str(), py::str(hit->at("addr").s))) {
            auto text = [](const engine::native_engine::Cell& x) -> Json {
                if (auto s = std::get_if<py::str>(&x)) return Json(*s);
                if (auto i = std::get_if<int64_t>(&x)) return Json(*i);
                if (auto d = std::get_if<double>(&x)) return Json(*d);
                if (auto b = std::get_if<py::bytes>(&x)) return Json(b->decode("utf-8", "replace"));
                return Json();
            };
            Json m = Json::object();
            m.set("engine", text(c.engine));
            m.set("rev", text(c.rev));
            m.set("name", text(c.name));
            m.set("source", text(c.source));
            m.set("problem", text(c.problem));
            Json older = Json::array();
            size_t k = 0;
            for (const auto& v : c.verdicts) {
                if (k++ >= 5) break;
                Json o = Json::object();
                o.set("profile", text(v.profile));
                o.set("verdict", text(v.verdict));
                o.set("reason", text(v.reason));
                o.set("limits", text(v.limits));
                older.push(o);
            }
            m.set("older_verdicts", older);
            made.push(m);
        }
        if (!made.a.empty()) {
            one.set("candidates", made);
            const Json* prof = found.find("profile");
            one.set("say", "no verdict under the verifier settings in force (profile " + (prof && prof->is_str() ? prof->s : std::string("None")) +
                               "); these are the stored reconstructions, with any verdicts from earlier settings");
        }
    }
    return _fit(one);
}

Json names(const py::str& module, const py::str& find, int64_t offset, int64_t limit) {
    auto got = quiet([&] { return symbols::names::of_module(_path(module)); });
    std::vector<Json> fns;
    const std::string low = lower8(find.utf8());
    for (const auto& f : got.functions) {
        if (!find.empty() && !contains8(lower8(f.name.utf8()), low)) continue;
        Json r = Json::object();
        r.set("addr", hx(f.address));
        r.set("key", f.key);
        r.set("name", f.name);
        r.set("source", f.source);
        fns.push_back(r);
    }
    Json out = _page(fns, offset, limit);
    out.set("imports", static_cast<int64_t>(got.imports.size()));
    return out;
}

Json identify(const py::str& module) {
    py::str p = _path(module);
    auto found = quiet([&] { return symbols::runtime_library::identify(p); });
    std::vector<std::pair<int64_t, std::pair<std::string, std::string>>> sorted;
    for (const auto& kv : found.items())
        sorted.push_back({std::get<0>(kv), {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()}});
    std::sort(sorted.begin(), sorted.end());
    Json rows = Json::array();
    for (size_t n = 0; n < sorted.size() && n < 2000; n++) {
        Json r = Json::object();
        r.set("addr", hx(sorted[n].first));
        r.set("name", sorted[n].second.first);
        r.set("library", sorted[n].second.second);
        rows.push(r);
    }
    Json out = Json::object();
    out.set("identified", static_cast<int64_t>(sorted.size()));
    out.set("rows", rows);
    try {
        auto c = quiet([&] { return symbols::runtime_library::compiler(p, found); });
        Json cj = Json::object();
        cj.set("linker", c.linker);
        cj.set("compiler", c.compiler);
        cj.set("evidence", Json::list_of(c.evidence));
        out.set("compiler", cj);
    } catch (const std::exception& e) {
        out.set("compiler", "unknown (" + type_name_(e) + ")");
    }
    return _fit(out);
}

Json demangle(const py::list<py::str>& decorated) {
    Json out = Json::object();
    size_t k = 0;
    for (const auto& n : decorated) {
        if (k++ >= 200) break;
        Json r = Json::object();
        r.set("readable", symbols::demangle::readable(n));
        r.set("declaration", symbols::demangle::declaration(n));
        r.set("stack_bytes", symbols::demangle::stack_bytes(n));
        out.set(n.utf8(), r);
    }
    return out;
}

Json commands() {
    Json out = Json::object();
    out.set("usage", main_module::DOC());
    std::vector<std::string> run;
    for (const auto& kv : main_module::COMMAND_TABLE()) {
        bool interactive = false;
        for (const auto& i : INTERACTIVE())
            if (i == kv.first) interactive = true;
        if (!interactive) run.push_back(kv.first.utf8());
    }
    std::sort(run.begin(), run.end());
    run.erase(std::unique(run.begin(), run.end()), run.end());
    out.set("runnable", Json::list_of(run));
    return out;
}

Json command_help(const py::str& command) {
    const main_module::Command* c = main_module::command(command);
    if (!c) throw py::ValueError("no command " + py::repr(command).utf8() + "; `commands` lists them");
    if (!hooks().module_doc) no_hook("module_doc");
    auto doc = hooks().module_doc(c->module);
    py::str text = doc && !doc->empty() ? *doc : py::str("(no description)");
    Json out = Json::object();
    out.set("command", command);
    out.set("module", c->module);
    out.set("fixed_args", Json::list_of(c->fixed));
    out.set("help", py::str(text.u32().substr(0, static_cast<size_t>(MOST))));
    return out;
}

// ---- what was made: reach runs, project folders, the source tree ----------------------------------------
namespace {

Json read_json_file_(const py::str& path) {
    std::ifstream f(std::filesystem::u8path(path.utf8()), std::ios::binary);
    if (!f) throw py::OSError("cannot open " + path.utf8());
    std::stringstream ss;
    ss << f.rdbuf();
    return loads(ss.str());
}

const Json* get_(const Json& r, const char* k) { return r.find(k); }
std::string text_(const Json* v) { return v && v->is_str() ? v->s : std::string(); }
// `r.get(k) or ""`'s text for a platform/path (a non-str is not expected)
bool ends_with_(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// the files of a folder as os.listdir gives them, UTF-8 sorted (Python's order of str)
void list_dir_(const std::string& folder, std::vector<std::string>& dirs, std::vector<std::string>& files) {
    std::error_code ec;
    for (std::filesystem::directory_iterator it(std::filesystem::u8path(folder), ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code e2;
        std::string name = it->path().filename().u8string();
        if (it->is_directory(e2)) dirs.push_back(name);
        else files.push_back(name);
    }
}

}  // namespace

Json reach_results(const py::str& run, const py::str& platform, bool judged_only, const py::str& find, int64_t offset, int64_t limit) {
    py::str folder = py::os::path::dirname(service_::state_path({py::str("reach"), py::str("x")}));
    if (run.empty()) {
        std::vector<std::string> dirs, files;
        list_dir_(folder.utf8(), dirs, files);
        std::vector<std::string> all = dirs;
        all.insert(all.end(), files.begin(), files.end());
        std::sort(all.begin(), all.end());
        Json runs = Json::array();
        for (const auto& name : all) {
            if (!ends_with_(name, ".json")) continue;
            Json d;
            try {
                d = read_json_file_(py::os::path::join(folder, py::str(name)));
            } catch (const py::ValueError&) {
                continue;
            } catch (const py::OSError&) {
                continue;
            }
            const Json* progs = d.find("programs");
            int64_t judged = 0;
            if (progs)
                for (const auto& r : progs->a)
                    if (truthy(r.find("judged"))) judged++;
            Json one = Json::object();
            one.set("run", name.substr(0, name.size() - 5));
            const Json* when = d.find("when");
            one.set("when", when ? *when : Json());
            const Json* inputs = d.find("inputs");
            one.set("inputs", inputs ? *inputs : Json());
            one.set("programs", progs ? static_cast<int64_t>(progs->a.size()) : 0);
            one.set("judged", judged);
            runs.push(one);
        }
        Json out = Json::object();
        out.set("runs", runs);
        return out;
    }
    py::str path = py::os::path::join(folder, py::os::path::basename(run) + py::str(".json"));
    if (!py::os::path::isfile(path)) throw py::ValueError("no reach run " + py::repr(run).utf8() + " (reach_results without a run lists them)");
    Json d = read_json_file_(path);
    Json per = Json::object();
    std::vector<Json> rows;
    const Json* progs = d.find("programs");
    const std::string want_platform = lower8(platform.utf8());
    const std::string want_find = lower8(find.utf8());
    if (progs)
        for (const auto& r : progs->a) {
            const Json* pl = r.find("platform");
            const std::string key = truthy(pl) ? text_(pl) : std::string("(unknown)");
            Json* p = per.find_mut(key);
            if (!p) {
                Json fresh = Json::object();
                fresh.set("programs", 0);
                fresh.set("judged", 0);
                const Json* lane = r.find("lane");
                fresh.set("lane", lane ? *lane : Json());
                p = &per.set(key, fresh);
            }
            p->set("programs", p->at("programs").i + 1);
            p->set("judged", p->at("judged").i + (truthy(r.find("judged")) ? 1 : 0));
            if (!platform.empty() && !contains8(lower8(truthy(pl) ? text_(pl) : std::string()), want_platform)) continue;
            if (!find.empty() && !contains8(lower8(truthy(r.find("path")) ? text_(r.find("path")) : std::string()), want_find)) continue;
            if (judged_only && !truthy(r.find("judged"))) continue;
            Json row = Json::object();
            for (const char* k : {"path", "origin", "found", "platform", "lane", "family", "size", "picked", "judged"}) {
                const Json* v = r.find(k);
                if (v && !v->is_null() && !(v->is_str() && v->s.empty())) row.set(k, *v);
            }
            rows.push_back(row);
        }
    Json out = _page(rows, offset, limit);
    const Json* when = d.find("when");
    out.set("when", when ? *when : Json());
    const Json* inputs = d.find("inputs");
    out.set("inputs", inputs ? *inputs : Json());
    out.set("platforms", per);
    return out;
}

namespace {

// The folders read_file may read: project folders, the source tree, state, this checkout.
std::vector<py::str> roots_() {
    std::vector<py::str> out;
    out.push_back(py::settings::get_str(py::str("converted")));
    out.push_back(py::settings::get_str(py::str("state")));
    out.push_back(service_::root());
    try {
        out.push_back(source::placement::tree_root());
    } catch (const std::exception&) {
    }
    std::vector<py::str> real;
    for (const auto& r : out)
        if (!r.empty() && py::os::path::isdir(r)) real.push_back(py::os::path::realpath(r));
    return real;
}

// os.path.commonpath([path, root]) == root: root is a leading run of path's components (case-insensitive, as Windows)
bool inside_(const py::str& path, const py::str& root) {
    auto parts = [](const py::str& s) {
        std::vector<std::string> v;
        std::string cur;
        for (char c : py::os::path::normcase(s).utf8()) {
            if (c == '\\' || c == '/') {
                if (!cur.empty()) v.push_back(cur);
                cur.clear();
            } else cur += c;
        }
        if (!cur.empty()) v.push_back(cur);
        return v;
    };
    auto drive = [](const py::str& s) { return py::os::path::normcase(s).utf8().substr(0, 2); };
    if (drive(path) != drive(root)) return false;   // another drive
    auto a = parts(path), b = parts(root);
    if (b.size() > a.size()) return false;
    for (size_t n = 0; n < b.size(); n++)
        if (a[n] != b[n]) return false;
    return true;
}

void listing_walk_(const std::string& top, const std::string& base, const std::string& want, int64_t most, Json& rows, bool& more) {
    std::vector<std::string> dirs, files;
    list_dir_(top, dirs, files);
    std::vector<std::string> keep_dirs;
    for (const auto& d : dirs)
        if (d.empty() || d[0] != '.') keep_dirs.push_back(d);
    std::sort(keep_dirs.begin(), keep_dirs.end());
    std::sort(files.begin(), files.end());
    for (const auto& name : files) {
        std::string full = top + "\\" + name;
        std::string rel = py::os::path::relpath(py::str(full), py::str(base)).utf8();
        for (auto& c : rel)
            if (c == '\\') c = '/';
        if (want.empty() || contains8(lower8(rel), want)) {
            Json row = Json::object();
            row.set("file", rel);
            row.set("size", py::os::path::getsize(py::str(full)));
            rows.push(row);
            if (static_cast<int64_t>(rows.a.size()) >= most) {
                more = true;
                return;
            }
        }
    }
    for (const auto& d : keep_dirs) {
        listing_walk_(top + "\\" + d, base, want, most, rows, more);
        if (more) return;
    }
}

// -> (the files {file, size}, whether there were more)
std::pair<Json, bool> listing_(const py::str& folder, const py::str& find = py::str(""), int64_t most = 2000) {
    Json rows = Json::array();
    bool more = false;
    listing_walk_(folder.utf8(), folder.utf8(), lower8(find.utf8()), most, rows, more);
    return {rows, more};
}

}  // namespace

Json projects(const py::str& find) {
    py::str root = py::settings::get_str(py::str("converted"));
    Json out = Json::array();
    if (py::os::path::isdir(root)) {
        std::vector<std::string> dirs, files;
        list_dir_(root.utf8(), dirs, files);
        std::vector<std::string> all = dirs;
        all.insert(all.end(), files.begin(), files.end());
        std::sort(all.begin(), all.end());
        for (const auto& name : all) {
            py::str pj = py::os::path::join(py::os::path::join(root, py::str(name)), py::str("project.json"));
            if (!py::os::path::isfile(pj)) continue;
            Json d;
            try {
                d = read_json_file_(pj);
            } catch (const py::ValueError&) {
                d = Json::object();
            } catch (const py::OSError&) {
                d = Json::object();
            }
            Json row = Json::object();
            row.set("name", name);
            const Json* file = d.find("file");
            row.set("file", file ? *file : Json(""));
            const Json* kind = d.find("kind");
            row.set("kind", kind ? *kind : Json(""));
            const Json* size = d.find("size");
            row.set("size", size ? *size : Json());
            Json cmds = Json::array();
            if (const Json* steps = d.find("steps"))
                for (const auto& s : steps->a) {
                    const Json* c = s.find("command");
                    cmds.push(c ? *c : Json());
                }
            row.set("commands", cmds);
            const std::string file_text = text_(row.find("file"));
            if (find.empty() || contains8(lower8(name + " " + file_text), lower8(find.utf8()))) out.push(row);
        }
    }
    Json r = Json::object();
    r.set("root", root);
    r.set("projects", out);
    return r;
}

Json project(const py::str& name, const py::str& find) {
    py::str folder = py::os::path::join(py::settings::get_str(py::str("converted")), py::os::path::basename(name));
    if (!py::os::path::isdir(folder))
        throw py::ValueError("no project " + py::repr(name).utf8() + " under " + py::settings::get_str(py::str("converted")).utf8() +
                             " (`projects` lists them)");
    Json d = Json::object();
    py::str pj = py::os::path::join(folder, py::str("project.json"));
    if (py::os::path::isfile(pj)) d = read_json_file_(pj);
    if (Json* steps = d.find_mut("steps"))
        for (auto& step : steps->a) {
            Json* outs = step.find_mut("outputs");
            if (outs && truthy(outs) && outs->a.size() > 20) {
                const size_t total = outs->a.size();
                outs->a.resize(20);
                outs->push("... " + std::to_string(total - 20) + " more");
            }
        }
    auto l = listing_(folder, find);
    Json out = Json::object();
    out.set("folder", folder);
    out.set("project", d);
    out.set("files", l.first);
    out.set("more_files", l.second);
    return _fit(out);
}

namespace {

// "built" (made by a build, or in a folder a build writes), "notes", or "source".
std::string file_kind_(const std::string& rel) {
    static const std::set<std::string> built = {".obj", ".o", ".lib", ".exp", ".res", ".map", ".dll", ".exe", ".drv", ".vxd", ".386",
                                                 ".sym", ".log", ".pdb", ".ilk", ".err", ".lst", ".com", ".sys", ".bin"};
    std::vector<std::string> parts;
    std::string cur;
    for (char c : lower8(rel)) {
        if (c == '/') {
            parts.push_back(cur);
            cur.clear();
        } else cur += c;
    }
    parts.push_back(cur);
    std::string ext = std::get<1>(py::os::path::splitext(py::str(parts.back()))).utf8();
    bool in_build = false;
    for (size_t n = 0; n + 1 < parts.size(); n++)
        if (parts[n].rfind("_build", 0) == 0 || parts[n] == "build" || parts[n] == "out" || parts[n] == "obj") in_build = true;
    if (built.count(ext) || in_build) return "built";
    return ext == ".md" || ext == ".txt" ? "notes" : "source";
}

}  // namespace

Json source_tree(const py::str& module, const py::str& find) {
    py::str p = _path(module);
    auto resolved = source::placement::resolve(py::os::path::basename(p), p);
    const std::string rel = std::get<0>(resolved).utf8();
    py::str folder = source::placement::tree_root();
    {
        std::string cur;
        for (char c : rel + "/") {
            if (c == '/') {
                folder = py::os::path::join(folder, py::str(cur));
                cur.clear();
            } else cur += c;
        }
    }
    std::pair<Json, bool> l = py::os::path::isdir(folder) ? listing_(folder, find) : std::pair<Json, bool>{Json::array(), false};
    std::map<std::string, int> order = {{"source", 0}, {"notes", 1}, {"built", 2}};
    std::vector<Json> files;
    for (const auto& f : l.first.a) {
        Json g = f;
        g.set("kind", file_kind_(f.at("file").s));
        files.push_back(g);
    }
    std::stable_sort(files.begin(), files.end(), [&](const Json& a, const Json& b) { return order[a.at("kind").s] < order[b.at("kind").s]; });
    Json counts = Json::object();
    for (const char* k : {"source", "notes", "built"}) {
        int64_t n = 0;
        for (const auto& f : files)
            if (f.at("kind").s == k) n++;
        counts.set(k, n);
    }
    Json out = Json::object();
    out.set("tree", source::placement::tree_root());
    out.set("folder", folder);
    out.set("relative", rel);
    out.set("decided", std::get<1>(resolved));
    out.set("exists", py::os::path::isdir(folder));
    out.set("counts", counts);
    out.set("files", Json::list_of(files));
    out.set("more_files", l.second);
    return out;
}

Json read_file(const py::str& path, int64_t offset, int64_t length) {
    py::str real = py::os::path::realpath(path);
    auto roots = roots_();
    bool ok = false;
    for (const auto& r : roots)
        if (inside_(real, r)) ok = true;
    if (!ok) {
        std::string names;
        for (const auto& r : roots) names += (names.empty() ? "" : ", ") + r.utf8();
        throw py::ValueError(path.utf8() + " is outside the folders this reads: " + names);
    }
    if (!py::os::path::isfile(real)) throw py::ValueError("no file " + path.utf8());
    std::string data;
    {
        std::ifstream f(std::filesystem::u8path(real.utf8()), std::ios::binary);
        std::stringstream ss;
        ss << f.rdbuf();
        data = ss.str();
    }
    if (data.substr(0, 4096).find('\0') != std::string::npos) throw py::ValueError(path.utf8() + " is binary; hex_dump reads it");
    py::str text;
    try {
        text = py::bytes(data).decode("utf-8");
    } catch (const py::ValueError&) {   // the period's files: Windows-1252
        text = py::bytes(data).decode("cp1252", "replace");
    }
    const int64_t at = std::max<int64_t>(0, offset), n = std::max<int64_t>(1, std::min<int64_t>(length, MOST));
    const int64_t total = static_cast<int64_t>(text.size());
    Json out = Json::object();
    out.set("path", real);
    out.set("characters", total);
    out.set("offset", at);
    const std::u32string u = text.u32();
    out.set("text", py::str(at < total ? u.substr(static_cast<size_t>(at), static_cast<size_t>(n)) : std::u32string()));
    out.set("next", at + n < total ? Json(at + n) : Json());
    return out;
}

// ---- one function now; two builds side by side -------------------------------------------------------
namespace {

// Does a function header line name the place `addr` is (hex, or segment:offset)? Compared as numbers: sub_00132 is 132, sub_1_0116 is
// 1:0116, func_0x7f021036 is 7f021036.
bool same_place_(const std::string& addr, const std::string& header) {
    static const py::re::Pattern pat(R"(\b(?:sub|func|FUN)_(?:0x)?([0-9a-fA-F]+)(?:_([0-9a-fA-F]+))?\s*\()", 0);
    auto m = pat.search(py::str(header));
    if (!m) return false;
    try {
        auto colon = addr.find(':');
        auto num = [](const std::string& s) { return std::stoull(s, nullptr, 16); };
        if (colon != std::string::npos) {
            py::str g2 = m->group(2);
            return !g2.empty() && num(m->group(1).utf8()) == num(addr.substr(0, colon)) && num(g2.utf8()) == num(addr.substr(colon + 1));
        }
        py::str g2 = m->group(2);
        return g2.empty() && num(m->group(1).utf8()) == num(addr);
    } catch (const std::exception&) {
        return false;   // ValueError in the Python
    }
}

std::vector<std::string> splitlines_(const std::string& t) {
    std::vector<std::string> v;
    std::string cur;
    for (size_t n = 0; n < t.size(); n++) {
        char c = t[n];
        if (c == '\n' || c == '\r') {
            if (c == '\r' && n + 1 < t.size() && t[n + 1] == '\n') n++;
            v.push_back(cur);
            cur.clear();
        } else cur += c;
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

std::string strip8_(const std::string& s) { return py::str(s).strip().utf8(); }
std::string rstrip8_(const std::string& s) { return py::str(s).rstrip().utf8(); }

// The function in a whole module's C at `addr` (or named `addr`), with the comment lines just above its header.
std::optional<std::string> cut_function_(const std::string& text, const std::string& addr) {
    bool plain = !addr.empty() && addr.find_first_not_of("0123456789abcdef:") == std::string::npos;
    std::optional<py::re::Pattern> named;
    if (!plain) named = py::re::Pattern("\\b" + py::re::escape(py::str(addr)).utf8() + "\\s*\\(", py::re::IGNORECASE);
    auto lines = splitlines_(text);
    for (size_t i = 0; i < lines.size(); i++) {
        const std::string& line = lines[i];
        if (line.empty() || py::str(line.substr(0, 1)).isspace() || line.find('(') == std::string::npos) continue;
        const std::string r = rstrip8_(line);
        const bool brace = !r.empty() && r.back() == '{';
        if (!(brace || (i + 1 < lines.size() && (strip8_(lines[i + 1]) == "{" || strip8_(lines[i + 1]).empty())))) continue;
        if (!(named ? named->search(py::str(line)).has_value() : same_place_(addr, line))) continue;
        size_t start = i;
        auto comment = [](const std::string& l) { return l.rfind("/*", 0) == 0 || l.rfind(" *", 0) == 0 || l.rfind("//", 0) == 0; };
        while (start > 0 && comment(lines[start - 1])) start--;
        size_t end = i + 1;
        while (end < lines.size() && lines[end] != "}") end++;
        std::string out;
        for (size_t k = start; k <= end && k < lines.size(); k++) out += (k > start ? "\n" : "") + lines[k];
        return out;
    }
    return std::nullopt;
}

}  // namespace

Json decompile_function(const py::str& module, const py::str& addr, int64_t wait) {
    py::str p = _path(module);
    std::string a = _addr(addr).utf8();
    wait = std::max<int64_t>(1, std::min<int64_t>(wait, 240));
    auto lane = corpus::lane(p);
    const bool x86 = lane && *lane == py::str("pe") && hooks().is_x86 && hooks().is_x86(p);
    std::optional<Json> value;
    int64_t waited = 0;
    if (x86) {
        int64_t va = place_(*image_(p), addr);
        a = hx(va);
        if (!hooks().decompile_one) no_hook("decompile_one");
        auto got = SLOW().get("decompile\x01" + p.utf8() + "\x01" + std::to_string(va), [p, va] {
            auto r = quiet([&] { return hooks().decompile_one(p, va); });
            if (!r) return error_json_("no function at " + hx(va));
            Json j = Json::object();
            j.set("name", r->first);
            j.set("text", r->second);
            return j;
        }, 3600.0, static_cast<double>(wait));
        value = got.first;
        waited = got.second;
    } else {
        if (!hooks().decompile_whole) no_hook("decompile_whole");
        auto got = SLOW().get("decompile\x01" + p.utf8(), [p] {
            Json j = Json::object();
            j.set("whole", quiet([&] { return hooks().decompile_whole(p); }));
            return j;
        }, 3600.0, static_cast<double>(wait));
        value = got.first;
        waited = got.second;
        if (value && value->has("whole")) {
            auto cut = cut_function_(value->at("whole").s, a);
            if (cut) {
                Json j = Json::object();
                j.set("text", *cut);
                value = j;
            } else {
                value = error_json_("no function header naming " + addr.utf8() + " in the module's C; run_command decompile writes all of it");
            }
        }
    }
    if (!value) return _not_ready(py::str("decompiling " + py::os::path::basename(p).utf8() + " at " + addr.utf8()), waited);
    if (value->has("error")) throw py::ValueError(value->at("error").s);
    Json out = *value;
    out.set("module", py::os::path::basename(p));
    out.set("addr", a);
    out.set("note", "decompiled now; not built or judged");
    return out;
}

namespace {

const char* const PAIR_FIELDS =
    "level: what agrees -- bytes (the same machine code but for what the loader fills in), text "
    "(the same instructions, addresses aside), shape (the same kinds of instruction and operand: "
    "registers or constants differ); how: which matching step paired them (bytes, name, text, "
    "shape, calls, likeness, position); score: likeness of the shapes, so a changed pair at 1.0 "
    "differs only in registers or constants. diff_function shows one pair side by side.";

// compare/versions.py's Comparison of two builds, worked out once and kept an hour.
std::pair<std::optional<VersionsSlot>, int64_t> versions_(const py::str& pa, const py::str& pb, int64_t wait) {
    if (!hooks().versions_compare) no_hook("versions_compare");
    return SLOW_VERSIONS().get("versions\x01" + pa.utf8() + "\x01" + pb.utf8(), [pa, pb] {
        return VersionsSlot{quiet([&] { return hooks().versions_compare(pa, pb); }), ""};
    }, 3600.0, static_cast<double>(std::max<int64_t>(1, std::min<int64_t>(wait, 240))));
}

}  // namespace

Json compare_versions(const py::str& module_a, const py::str& module_b, const py::str& status, int64_t offset, int64_t limit, int64_t wait) {
    auto v = versions_(_path(module_a), _path(module_b), wait);
    if (!v.first) return _not_ready(py::str("comparing the two builds"), v.second);
    if (!v.first->error.empty()) return error_json_(v.first->error);
    const VersionsResult& r = *v.first->value;
    std::vector<Json> pairs;
    for (const auto& x : r.pairs)
        if (status.empty() || x.at("status").s == status.utf8()) pairs.push_back(x);
    std::stable_sort(pairs.begin(), pairs.end(), [](const Json& a, const Json& b) {
        return (a.at("status").s != "changed") < (b.at("status").s != "changed");
    });
    Json out = Json::object();
    out.set("report", r.report);
    out.set("counts", r.counts);
    out.set("code", r.code);
    out.set("fields", PAIR_FIELDS);
    auto head = [](const Json& j) {
        Json o = Json::array();
        for (size_t n = 0; n < j.a.size() && n < 500; n++) o.push(j.a[n]);
        return o;
    };
    out.set("added", head(r.added));
    out.set("removed", head(r.removed));
    out.set("names", head(r.names));
    Json pg = _page(pairs, offset, limit);
    for (size_t n = 0; n < pg.keys.size(); n++) out.set(pg.keys[n], pg.a[n]);
    return _fit(out);
}

Json diff_function(const py::str& module_a, const py::str& module_b, const py::str& addr, int64_t wait) {
    py::str pa = _path(module_a), pb = _path(module_b);
    auto v = versions_(pa, pb, wait);
    if (!v.first) return _not_ready(py::str("comparing the two builds"), v.second);
    if (!v.first->error.empty()) return error_json_(v.first->error);
    const VersionsResult& c = *v.first->value;
    const std::string want = _addr(addr).utf8(), low = lower8(addr.strip().utf8());
    const Json* pair = nullptr;
    for (const auto& x : c.pairs)
        if (x.at("from").s == want || x.at("to").s == want) { pair = &x; break; }
    if (!pair)
        for (const auto& x : c.pairs)
            if (lower8(x.at("from_name").s) == low || lower8(x.at("to_name").s) == low) { pair = &x; break; }
    auto listing = [](const VersionFunction& f) {
        std::string o;
        for (size_t n = 0; n < f.rows.size(); n++) o += (n ? "\n" : "") + hx(f.rows[n].first) + "  " + f.rows[n].second.utf8();
        return o;
    };
    if (!pair) {
        struct Side {
            const std::map<std::string, VersionFunction>& fns;
            const Json& keys;
            const char* what;
        } sides[2] = {{c.a_functions, c.removed, "removed: only in the first build"}, {c.b_functions, c.added, "added: only in the second build"}};
        for (const auto& s : sides)
            for (const auto& k : s.keys.a) {
                auto f = s.fns.find(k.s);
                if (k.s == want || (f != s.fns.end() && lower8(f->second.name.utf8()) == low)) {
                    Json o = Json::object();
                    o.set("status", s.what);
                    o.set("addr", k.s);
                    o.set("name", f->second.name);
                    o.set("listing", listing(f->second));
                    return o;
                }
            }
        throw py::ValueError("no function " + py::repr(addr).utf8() + " in either build (compare_versions lists the pairs)");
    }
    const VersionFunction& fa = c.a_functions.at(pair->at("from").s);
    const VersionFunction& fb = c.b_functions.at(pair->at("to").s);
    Json out = Json::object();
    for (const char* k : {"from", "to", "from_name", "to_name", "status", "level", "score", "sizes"}) out.set(k, pair->at(k));
    std::vector<std::string> lines;
    // lined up by shape, so other registers or constants do not break the line-up
    py::list<py::str> sa, sb;
    for (const auto& s : fa.shape) sa.append(s);
    for (const auto& s : fb.shape) sb.append(s);
    auto pad = [](const std::string& s, size_t w, bool left) {
        const size_t n = py::str(s).size();
        return n >= w ? s : (left ? s + std::string(w - n, ' ') : std::string(w - n, ' ') + s);
    };
    for (const auto& op : exact::aim::_get_opcodes(sa, sb)) {
        const std::string tag = std::get<0>(op).utf8();
        const int64_t a1 = std::get<1>(op), a2 = std::get<2>(op), b1 = std::get<3>(op), b2 = std::get<4>(op);
        if (tag == "equal") {
            for (int64_t i = a1, j = b1; i < a2 && j < b2; i++, j++)
                lines.push_back(std::string(fa.text[static_cast<size_t>(i)] == fb.text[static_cast<size_t>(j)] ? " " : "~") + " " +
                                hx(fa.rows[static_cast<size_t>(i)].first) + " " + pad(fa.rows[static_cast<size_t>(i)].second.utf8(), 36, true) + " " +
                                hx(fb.rows[static_cast<size_t>(j)].first) + " " + fb.rows[static_cast<size_t>(j)].second.utf8());
            continue;
        }
        for (int64_t i = a1; i < a2; i++) lines.push_back("- " + hx(fa.rows[static_cast<size_t>(i)].first) + " " + fa.rows[static_cast<size_t>(i)].second.utf8());
        for (int64_t j = b1; j < b2; j++)
            lines.push_back("+ " + std::string(47, ' ') + " " + hx(fb.rows[static_cast<size_t>(j)].first) + " " + fb.rows[static_cast<size_t>(j)].second.utf8());
    }
    std::string diff;
    int64_t differ = 0;
    for (size_t n = 0; n < lines.size(); n++) {
        diff += (n ? "\n" : "") + lines[n];
        if (lines[n][0] != ' ') differ++;
    }
    out.set("diff", diff);
    out.set("differ", differ);
    return _fit(out);
}

// ---- code: disassembly, cross-references, a module at a glance ----------------------------------------
namespace {

// The text stored at an address, when there is some (4 printable characters or more).
std::optional<std::string> ascii_at_(I::Image& img, int64_t va, int64_t most = 40) {
    const auto* s = img.span(va);
    if (!s || !img.initialised(va)) return std::nullopt;
    const std::string& raw = std::get<1>(*s).raw();
    const int64_t from = va - std::get<0>(*s);
    if (from < 0 || from > static_cast<int64_t>(raw.size())) return std::nullopt;
    std::string text = raw.substr(static_cast<size_t>(from), static_cast<size_t>(most));
    text = text.substr(0, text.find('\0'));
    if (text.size() < 4) return std::nullopt;
    for (unsigned char c : text)
        if (c < 32 || c >= 127) return std::nullopt;
    return text;
}

using Thunk = std::optional<std::pair<int64_t, py::str>>;

// The import a `jmp [slot]` at `va` goes to, or None.
Thunk thunk_(I::Image& img, int64_t va) {
    const auto* s = img.span(va);
    I::InsP ins;
    if (s && std::get<2>(*s)) {
        auto l = img.linear(va, 1);
        if (!l.empty()) ins = l[0];
    }
    std::optional<int64_t> slot;
    if (ins && ins->flow && *ins->flow == py::str("jmp") && !ins->target) slot = I::_slot(*ins);
    if (slot && img.imports.contains(*slot)) return std::make_pair(*slot, img.imports[*slot]);
    return std::nullopt;
}

// A function's name, with the import it only jumps to; else name+offset, else the address.
std::string label_(I::Image& img, int64_t va) {
    auto t = *img.memo<Thunk>(py::str("mcp_thunk:" + std::to_string(va)), [va](I::Image& im) { return std::make_shared<Thunk>(thunk_(im, va)); });
    if (img.names.contains(va)) {
        return t ? img.names[va].utf8() + " (thunk: " + import_name_(t->second).utf8() + ")" : img.names[va].utf8();
    }
    if (t) return "thunk at " + hx(va) + ": " + import_name_(t->second).utf8();   // a thunk no function starts at
    auto owner = img.owner(va);
    return owner ? img.names[*owner].utf8() + "+" + hx(va - *owner) : hx(va);
}

// What an instruction's destination or address operand stands for.
std::string note_(I::Image& img, const I::Ins& ins, int64_t fn_start) {
    if (ins.flow && (*ins.flow == py::str("call") || *ins.flow == py::str("jmp") || *ins.flow == py::str("jcc"))) {
        auto dest = I::destination(img, ins);
        if (dest && img.imports.contains(*dest)) return import_name_(img.imports[*dest]).utf8();
        if (dest && *ins.flow != py::str("jcc")) {
            auto owner = img.owner(*dest);
            if (img.names.contains(*dest) || (owner && *owner != fn_start)) return label_(img, *dest);
            if (!owner) return hx(*dest) + ": no known function starts there";
        }
    }
    for (const auto& op : ins.ops) {
        std::optional<int64_t> va;
        if (op.kind == py::str("i") && op.size == 4) va = op.value;
        else if (op.kind == py::str("m") && !op.base) va = op.disp;
        if (!va) continue;
        if (img.imports.contains(*va)) return import_name_(img.imports[*va]).utf8();
        if (auto text = ascii_at_(img, *va)) return "\"" + *text + "\"";
        if (img.names.contains(*va)) return img.names[*va].utf8();
    }
    return "";
}

std::string hex_upper_spaced_(const std::string& raw) {
    std::string o;
    char b[4];
    for (size_t n = 0; n < raw.size(); n++) {
        std::snprintf(b, sizeof b, "%02X", static_cast<unsigned char>(raw[n]));
        o += (n ? " " : "") + std::string(b);
    }
    return o;
}

}  // namespace

Json disasm(const py::str& module, const py::str& addr, int64_t count) {
    py::str p = _path(module);
    auto imgp = image_(p);
    I::Image& img = *imgp;
    int64_t va = place_(img, addr);
    const int64_t n = std::max<int64_t>(1, std::min<int64_t>(count ? count : 64, 4000));
    py::list<I::InsP> insns;
    std::string how;
    Json head = Json::object();
    if (img.names.contains(va) && count == 0) {
        auto fn = img.function(va);
        insns = fn->insns;
        how = "the function, as decoded from its start";
        head.set("function", img.names[va]);
        head.set("start", hx(va));
        head.set("end", hx(fn->end));
        head.set("limit", hx(fn->limit));
    } else {
        if (!img.span(va)) throw py::ValueError(hx(va) + " is not in " + img.name.utf8());
        insns = img.linear(va, n);
        how = std::to_string(insns.size()) + " instructions read straight on";
        auto owner = img.owner(va);
        head.set("inside", owner ? Json(img.names[*owner].utf8() + "+" + hx(va - *owner)) : Json());
    }
    auto lane = corpus::lane(p);
    const bool pe = lane && *lane == py::str("pe");
    std::string listing;
    int64_t rows = 0;
    for (const auto& ins : insns) {
        const auto* s = img.span(ins->addr);
        std::string code;
        if (s) {
            const std::string& raw = std::get<1>(*s).raw();
            int64_t from = ins->addr - std::get<0>(*s);
            if (from >= 0 && from <= static_cast<int64_t>(raw.size())) code = raw.substr(static_cast<size_t>(from), static_cast<size_t>(ins->size));
        }
        std::optional<int64_t> off = pe ? file_offset_(p, ins->addr) : std::nullopt;
        std::string note = note_(img, *ins, va);
        std::string c = hex_upper_spaced_(code), t = ins->text.utf8();
        std::string line = hx(ins->addr) + (off ? "  " + hx5(*off) : "") + "  " + c + std::string(c.size() < 20 ? 20 - c.size() : 0, ' ') + " " + t +
                           std::string(py::str(t).size() < 34 ? 34 - py::str(t).size() : 0, ' ') + (note.empty() ? "" : "; " + note);
        listing += (rows ? "\n" : "") + rstrip8_(line);
        rows++;
    }
    Json out = head;
    out.set("module", img.name);
    out.set("addr", hx(va));
    out.set("how", how);
    out.set("count", rows);
    out.set("listing", listing);
    return _fit(out);
}

namespace {

struct XrefSite {
    int64_t fn, site;
    std::string how;
    bool operator<(const XrefSite& o) const { return std::tie(site, fn, how) < std::tie(o.site, o.fn, o.how); }
    bool operator==(const XrefSite& o) const { return site == o.site && fn == o.fn && how == o.how; }
};
using XrefIndex = std::map<int64_t, std::vector<XrefSite>>;

// {address: [(function, site, how)]}: every direct call, jump and import-slot use in the code.
std::shared_ptr<XrefIndex> xref_index_(I::Image& img) {
    auto out = std::make_shared<XrefIndex>();
    for (const auto& fn : img.each())
        for (const auto& ins : fn->insns) {
            const bool cj = ins->flow && (*ins->flow == py::str("call") || *ins->flow == py::str("jmp"));
            const bool jcc = ins->flow && *ins->flow == py::str("jcc") && ins->target && !(fn->start <= *ins->target && *ins->target < fn->limit);
            if ((ins->target && cj) || jcc) (*out)[*ins->target].push_back({fn->start, ins->addr, ins->flow->utf8()});
            auto slot = I::_slot(*ins);
            if (slot && img.imports.contains(*slot)) (*out)[*slot].push_back({fn->start, ins->addr, (ins->flow ? ins->flow->utf8() : std::string("reads")) + " [slot]"});
        }
    return out;
}

}  // namespace

Json xrefs(const py::str& module, const py::str& addr) {
    py::str p = _path(module);
    auto imgp = image_(p);
    I::Image& img = *imgp;
    auto g = A::bridge::graph(img);
    auto index = img.memo<XrefIndex>(py::str("mcp_xrefs"), xref_index_);
    std::vector<int64_t> targets;
    try {
        targets = {place_(img, addr)};
    } catch (const py::ValueError&) {
        const std::string low = lower8(addr.strip().utf8());
        std::vector<std::pair<int64_t, std::string>> imps;
        for (const auto& kv : img.imports.items()) imps.emplace_back(std::get<0>(kv), std::get<1>(kv).utf8());
        std::sort(imps.begin(), imps.end());
        for (const auto& kv : imps) {
            auto tail = [](const std::string& s) { return s.substr(s.rfind('!') == std::string::npos ? 0 : s.rfind('!') + 1); };
            const std::string nm = kv.second, named = import_name_(py::str(nm)).utf8();
            std::string named_tail = tail(named);
            named_tail = named_tail.substr(0, named_tail.find(" ("));
            if (low == lower8(nm) || low == lower8(tail(nm)) || low == lower8(named) || low == lower8(named_tail)) targets.push_back(kv.first);
        }
        if (targets.empty()) throw;
    }
    auto labt = [&](int64_t v) { return label_(img, v); };
    Json out = Json::object();
    std::string joined;
    for (size_t k = 0; k < targets.size(); k++) joined += (k ? " " : "") + hx(targets[k]);
    out.set("module", img.name);
    out.set("addr", joined);
    const int64_t va = targets[0];
    if (img.imports.contains(va)) {
        out.set("import", import_name_(img.imports[va]));
        if (img.bits == 32) {   // the thunks that jump to it (jmp [slot]), function or not
            std::string pattern = "\xff\x25";
            for (int k = 0; k < 4; k++) pattern += static_cast<char>((va >> (8 * k)) & 0xff);
            for (const auto& s : img.spans) {
                if (!std::get<2>(s)) continue;
                const std::string& data = std::get<1>(s).raw();
                for (size_t at = data.find(pattern); at != std::string::npos; at = data.find(pattern, at + 1))
                    targets.push_back(std::get<0>(s) + static_cast<int64_t>(at));
            }
            if (targets.size() > 1) {
                Json th = Json::array();
                for (size_t k = 1; k < targets.size(); k++) th.push(labt(targets[k]));
                out.set("thunks", th);
            }
        }
    } else if (img.names.contains(va)) {
        out.set("function", img.names[va]);
    } else if (img.owner(va)) {
        out.set("inside", labt(va));
    }
    if (img.exports.contains(va)) out.set("exported_as", img.exports[va]);
    if (!img.imports.contains(va))
        if (auto t = thunk_(img, va)) out.set("is", "a thunk: jmp [" + hx(t->first) + "] = " + import_name_(t->second).utf8());
    std::vector<XrefSite> sites;
    for (int64_t t : targets) {
        auto it = index->find(t);
        if (it != index->end()) sites.insert(sites.end(), it->second.begin(), it->second.end());
    }
    std::sort(sites.begin(), sites.end());
    sites.erase(std::unique(sites.begin(), sites.end()), sites.end());
    Json from_code = Json::array();
    for (size_t k = 0; k < sites.size() && k < 500; k++) {
        Json r = Json::object();
        r.set("site", hx(sites[k].site));
        r.set("in", labt(sites[k].fn));
        r.set("how", sites[k].how);
        from_code.push(r);
    }
    out.set("from_code", from_code);
    Json referred = Json::array();
    int64_t nrefs = 0;
    for (const auto& r : g->refs) {
        if (std::find(targets.begin(), targets.end(), r.address) == targets.end()) continue;
        nrefs++;
        if (referred.a.size() >= 500) continue;
        auto ins = img.covering(r.instruction->addr);
        const std::string how = r.how.utf8();
        Json o = Json::object();
        o.set("site", hx(r.instruction->addr));
        o.set("in", labt(r.function));
        o.set("how", how == "r" ? "reads" : how == "w" ? "writes" : how == "rw" ? "reads and writes" : "takes the address");
        o.set("ins", ins ? ins->text : py::str(""));
        referred.push(o);
    }
    out.set("referred", referred);
    Json in_data = Json::array();
    if (g->taken.contains(va))
        for (const auto& t : g->taken[va])
            if (t.kind == py::str("data") && in_data.a.size() < 200) in_data.push(hx(t.at));
    out.set("in_data", in_data);
    if (img.names.contains(va)) {   // every call, and every jump that leaves the function
        auto fn = img.function(va);
        Json calls = Json::array();
        for (const auto& ins : fn->insns) {
            if (!ins->flow || !(*ins->flow == py::str("call") || *ins->flow == py::str("jmp"))) continue;
            auto d = I::destination(img, *ins);
            if (*ins->flow == py::str("jmp")) {
                bool table_site = false;
                for (const auto& t : fn->tables)
                    if (t.site == ins->addr) table_site = true;
                if ((!d && table_site) || (d && !img.imports.contains(*d) && fn->start <= *d && *d < fn->limit)) continue;
            }
            Json c = Json::object();
            c.set("site", hx(ins->addr));
            c.set("kind", *ins->flow == py::str("call") ? "call" : "tail");
            c.set("to", !d ? Json("(decided at run time)") : img.imports.contains(*d) ? Json(import_name_(img.imports[*d])) : Json(labt(*d)));
            calls.push(c);
        }
        out.set("calls", calls);
    }
    if (sites.empty() && nrefs == 0 && in_data.a.empty()) {
        bool in_roots = false;
        for (const auto& r : img.roots)
            if (r == va) in_roots = true;
        const bool exported = img.exports.contains(va) || in_roots;
        out.set("say", exported ? "nothing in the module refers to it: it is entered from outside (an export)" : "no code or data here refers to it");
    }
    return _fit(out);
}

Json triage(const py::str& module) {
    py::str p = _path(module);
    py::str name = py::os::path::basename(p);
    Json d = describe(p);
    Json out = Json::object();
    out.set("module", name);
    out.set("path", p);
    Json is = Json::object();
    for (const char* k : {"found", "platform", "lane", "family", "size"}) is.set(k, d.find(k) ? *d.find(k) : Json());
    out.set("is", is);
    out.set("summary", d.find("summary") ? *d.find("summary") : Json());
    auto said = [](const std::exception& e) { return type_name_(e) + ": " + e.what(); };
    try {
        Json info = quiet([&] { return hooks().inspect_describe(p); });
        hex_addresses_(info);
        Json ex = Json::array();
        const Json* e = info.find("exports");
        const Json* syms = e && e->is_obj() ? e->find("symbols") : nullptr;
        if (syms)
            for (size_t k = 0; k < syms->a.size() && k < 200; k++) {
                const Json* nm = syms->a[k].find("name");
                const Json* od = syms->a[k].find("ordinal");
                ex.push(truthy(nm) ? *nm : Json("#" + (od ? py_repr(*od) : std::string("None"))));
            }
        out.set("exports", ex);
        Json imps = Json::object();
        if (const Json* im = info.find("imports"))
            for (const auto& i : im->a) {
                Json names_ = Json::array();
                if (const Json* ss = i.find("symbols"))
                    for (size_t k = 0; k < ss->a.size() && k < 100; k++) {
                        const Json* nm = ss->a[k].find("name");
                        const Json* od = ss->a[k].find("ordinal");
                        names_.push(truthy(nm) ? *nm : Json("#" + (od ? py_repr(*od) : std::string("None"))));
                    }
                imps.set(i.at("module").s, names_);
            }
        out.set("imports", imps);
    } catch (const std::exception& e) {
        out.set("inspect", said(e));
    }
    try {
        Json ident = identify(p);
        std::map<std::string, int64_t> libs;
        std::vector<std::string> order;
        for (const auto& r : ident.at("rows").a) {
            if (!libs.count(r.at("library").s)) order.push_back(r.at("library").s);
            libs[r.at("library").s]++;
        }
        Json lj = Json::object();
        for (const auto& k : order) lj.set(k, libs[k]);
        Json mb = Json::object();
        mb.set("compiler", ident.find("compiler") ? *ident.find("compiler") : Json());
        mb.set("identified", lj);
        out.set("made_by", mb);
    } catch (const std::exception& e) {
        out.set("made_by", said(e));
    }
    try {
        Json fns = functions(p, py::str(""), py::str(""), 0, 5000);
        Json store = Json::object();
        store.set("counts", fns.at("counts"));
        store.set("reasons", fns.at("reasons"));
        out.set("store", store);
        std::vector<Json> bad;
        for (const auto& r : fns.at("rows").a) {
            const std::string c = r.at("class").s;
            if ((c == V::UNVERIFIED.utf8() || c == V::WEAK.utf8() || c == V::NO_COMPILE.utf8() || c == V::NO_SOURCE.utf8()) && !r.has("library")) bad.push_back(r);
        }
        Json look = Json::array();
        for (size_t k = 0; k < bad.size() && k < 12; k++) {
            Json one = function(p, py::str(bad[k].at("addr").s));
            Json o = Json::object();
            o.set("addr", bad[k].at("addr"));
            o.set("name", bad[k].at("name"));
            o.set("class", bad[k].at("class"));
            o.set("why", bad[k].at("why"));
            o.set("detail", one.find("detail") ? *one.find("detail") : Json(""));
            std::string src = truthy(one.find("source")) ? one.at("source").s : std::string();
            o.set("source", py::str(py::str(src).u32().substr(0, 1500)));
            look.push(o);
        }
        out.set("to_look_at", look);
        if (bad.size() > 12) out.set("more_to_look_at", static_cast<int64_t>(bad.size() - 12));
        if (!truthy(fns.find("in_store"))) {
            Json st = out.at("store");
            st.set("say", fns.find("say") ? *fns.find("say") : Json());
            out.set("store", st);
        }
    } catch (const py::ValueError& e) {
        out.set("store", std::string(e.what()));
    }
    try {
        Json st = source_tree(p);
        Json s = Json::object();
        s.set("folder", st.at("folder"));
        s.set("exists", st.at("exists"));
        s.set("counts", st.at("counts"));
        Json notes = Json::array(), source = Json::array();
        for (const auto& f : st.at("files").a) {
            if (f.at("kind").s == "notes" && notes.a.size() < 20) notes.push(f.at("file"));
            if (f.at("kind").s == "source" && source.a.size() < 40) source.push(f.at("file"));
        }
        s.set("notes", notes);
        s.set("source", source);
        out.set("source", s);
    } catch (const std::exception& e) {
        out.set("source", said(e));
    }
    out.set("next", "disasm and xrefs at each function to look at; read_file the notes; compare_versions against another build of it");
    return _fit(out);
}

// ---- jobs ---------------------------------------------------------------------------------------------
Json run_command(const py::str& command, const std::optional<py::list<py::str>>& args_in) {
    py::list<py::str> args = args_in ? *args_in : py::list<py::str>();
    if (!main_module::command(command)) throw py::ValueError("no command " + py::repr(command).utf8() + "; `commands` lists them");
    for (const auto& i : INTERACTIVE())
        if (i == command) throw py::ValueError(py::repr(command).utf8() + " is an interface or a server; it is not run as a job");
    py::str label("");
    for (const auto& a : args)
        if (!a.startswith(py::str("-"))) {
            label = py::os::path::basename(py::os::path::normpath(a));
            break;
        }
    return service_::JOBS().launch(command, args, label.lower());
}

Json jobs() {
    Json out = Json::object();
    out.set("jobs", service_::JOBS().all());
    return out;
}

Json job(const py::str& job_id, int64_t offset, int64_t tail) {
    auto j = service_::JOBS().get(job_id);
    if (!j) throw py::ValueError("no job " + py::repr(job_id).utf8() + " (this server's jobs: `jobs`)");
    Json out = service_::JOBS().view(*j, tail ? 0 : offset);
    py::str log = out.at("log").str();
    if (tail) {
        const size_t n = static_cast<size_t>(std::max<int64_t>(1, std::min<int64_t>(tail, MOST)));
        const std::u32string u = log.u32();
        out.set("log", py::str(u.size() > n ? u.substr(u.size() - n) : u));
    } else if (static_cast<int64_t>(log.size()) > MOST) {
        py::str cut(log.u32().substr(0, static_cast<size_t>(MOST)));
        out.set("next", offset + static_cast<int64_t>(cut.utf8().size()));
        out.set("log", cut);
    }
    return out;
}

Json job_file(const py::str& job_id, int64_t offset, int64_t length) {
    auto j = service_::JOBS().get(job_id);
    if (!j || !(!j->output.empty() && py::os::path::isfile(j->output))) throw py::ValueError("job " + py::repr(job_id).utf8() + " wrote no file");
    std::ifstream f(std::filesystem::u8path(j->output.utf8()), std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    py::str text = py::bytes(ss.str()).decode("utf-8", "replace");
    const int64_t n = std::max<int64_t>(1, std::min<int64_t>(length, MOST)), total = static_cast<int64_t>(text.size());
    Json out = Json::object();
    out.set("path", j->output);
    out.set("characters", total);
    out.set("offset", offset);
    out.set("text", py::str(offset < total ? text.u32().substr(static_cast<size_t>(offset), static_cast<size_t>(n)) : std::u32string()));
    out.set("next", offset + n < total ? Json(offset + n) : Json());
    return out;
}

Json stop_job(const py::str& job_id) {
    if (!service_::JOBS().stop(job_id)) throw py::ValueError("no job " + py::repr(job_id).utf8());
    return service_::JOBS().view(*service_::JOBS().get(job_id));
}

// ---- writing ------------------------------------------------------------------------------------------
Json name_function(const py::str& module, const py::str& addr, const py::str& name) {
    py::str flat = name.replace(py::str("_"), py::str("a")).replace(py::str("::"), py::str("a"));
    if (name.empty() || !flat.isalnum()) throw py::ValueError("a name is letters, digits, _ and ::");
    py::str p = _path(module);
    std::string a = _addr(addr).utf8();
    int64_t address = py::int_parse(py::str(a.substr(a.rfind(':') == std::string::npos ? 0 : a.rfind(':') + 1)), 16);
    auto digest = quiet([&] { return symbols::names::set_name(p, address, name); });
    if (!digest) throw py::ValueError("no body recorded at " + addr.utf8() + " in " + py::os::path::basename(p).utf8() + " (too small to tell apart, or not a function start)");
    auto fs = F::open();
    Json shared;
    try {
        auto c = fs->modules_with_body(*digest);
        if (auto i = std::get_if<int64_t>(&c)) shared = Json(*i);
        else if (auto s = std::get_if<py::str>(&c)) shared = Json(*s);
        else if (auto d = std::get_if<double>(&c)) shared = Json(*d);
    } catch (...) {
        fs->close();
        throw;
    }
    fs->close();
    Json out = Json::object();
    out.set("named", name);
    out.set("body", *digest);
    out.set("modules_with_this_body", shared);
    return out;
}

// ---- the server ----------------------------------------------------------------------------------------
namespace {

// A tool's parameter: name, JSON-schema type ("string", "integer", "boolean", "array" (list of str), "array|null"),
// and its default as JSON text (null pointer: required).
struct Param {
    const char* name;
    const char* type;
    const char* def;
};
struct ToolSpec {
    const char* name;       // as the assistant sees it
    const char* function;   // the Python function's name
    const char* doc;        // its docstring (inspect.cleandoc)
    std::vector<Param> params;
};

// (generated from mcp_server.py's own functions: the order is READING, then run_command, stop_job, name_function)
const std::vector<ToolSpec>& TOOL_SPECS() {
    static const std::vector<ToolSpec> t = {
    {"describe", "describe", R"DOC(What a file is, the platform it belongs to, and the lane CodeBreaker reconstructs it with
(None when there is none yet). A corpus name is accepted too.)DOC",
     {{"path", "string", nullptr}}},
    {"inspect", "inspect_module", R"DOC(The inspector: headers, sections, imports, exports, resources, version. Without `section`
the whole description (long parts are listed instead when it is too big); with it, one part:
summary, dos, headers, sections, exports, imports, resources, directories, relocations, ...
Addresses are hex text; file offsets and sizes are numbers (as hex_dump and resource take
them). Imports by number carry the name when one is known, and where it came from.)DOC",
     {{"module", "string", nullptr}, {"section", "string", "\"\""}}},
    {"strings", "strings", R"DOC(Text found in a file (ASCII and UTF-16), with file offsets; `find` keeps those containing it.)DOC",
     {{"module", "string", nullptr}, {"find", "string", "\"\""}, {"min_len", "integer", "4"}, {"offset", "integer", "0"}, {"limit", "integer", "300"}}},
    {"hex_dump", "hex_dump", R"DOC(A hex dump of a file's bytes from file `offset`, or from the place a 32-bit module's
address (or function name) `addr` is stored (at most 16 KB).)DOC",
     {{"module", "string", nullptr}, {"offset", "integer", "0"}, {"length", "integer", "512"}, {"addr", "string", "\"\""}}},
    {"resource", "resource", R"DOC(One resource, by the file offset and size `inspect` gives for it: a picture (icon,
bitmap, cursor) as an image, anything else as text.)DOC",
     {{"module", "string", nullptr}, {"offset", "integer", nullptr}, {"size", "integer", nullptr}, {"type_id", "integer", "-1"}, {"wide", "boolean", "true"}}},
    {"modules", "modules", R"DOC(The corpus modules with their ledger numbers (functions, verified, weak, unverified, ...).
`kind`: pe, ne, le, dos, powerpc, ...; `find`: part of a name.)DOC",
     {{"kind", "string", "\"\""}, {"find", "string", "\"\""}, {"only_in_store", "boolean", "false"}, {"offset", "integer", "0"}, {"limit", "integer", "200"}, {"refresh", "boolean", "false"}}},
    {"coverage", "coverage", R"DOC(How the corpus stands: totals and one row per kind of module.)DOC",
     {{"refresh", "boolean", "false"}}},
    {"progress", "progress", R"DOC(What a running sweep is doing: results per minute, recent modules, busy workers, a rough ETA.)DOC",
     {}},
    {"functions", "functions", R"DOC(Every function the store holds for a module, with its class (verified, weak, unverified,
not-judged, no-compile, no-source), name and why; plus counts and the commonest reasons. A
function a runtime library or a tool (the thunk compiler) made carries `library`, and is
counted under `identified`. `cls` keeps one class (or "identified"); `find` matches a name or
an address.)DOC",
     {{"module", "string", nullptr}, {"cls", "string", "\"\""}, {"find", "string", "\"\""}, {"offset", "integer", "0"}, {"limit", "integer", "300"}}},
    {"function", "function", R"DOC(One function: its reconstructed source, the verdict and the verdict's detail. `addr` is
its address (hex, with or without 0x) or its name.)DOC",
     {{"module", "string", nullptr}, {"addr", "string", nullptr}}},
    {"names", "names", R"DOC(Every function and import of a module with its best name and where the name came from
(exports, debug symbols, runtime libraries, bodies named in other modules).)DOC",
     {{"module", "string", nullptr}, {"find", "string", "\"\""}, {"offset", "integer", "0"}, {"limit", "integer", "300"}}},
    {"identify", "identify", R"DOC(Functions of a module that belong to a runtime library (by signature), and what built it.)DOC",
     {{"module", "string", nullptr}}},
    {"demangle", "demangle", R"DOC(Decorated linker names (Microsoft C++ ?Foo@@..., _Bar@8) as readable declarations.)DOC",
     {{"decorated", "array", nullptr}}},
    {"commands", "commands", R"DOC(Every command of `python -m codebreaker` with its one-line usage (any of them can be run as
a job through run_command, except the interfaces and servers).)DOC",
     {}},
    {"command_help", "command_help", R"DOC(The full description of one command: its module's text (arguments, options, what it writes).)DOC",
     {{"command", "string", nullptr}}},
    {"jobs", "jobs", R"DOC(The jobs started, newest first: with the service running, those of every interface (the
terminal's, each MCP server's), each with its owner; else this server's.)DOC",
     {}},
    {"job", "job", R"DOC(A job's state and its output from byte `offset` (up to 256 KB; `next` is where to go on).
`tail`: just the last that many characters of the output instead.)DOC",
     {{"job_id", "string", nullptr}, {"offset", "integer", "0"}, {"tail", "integer", "0"}}},
    {"job_file", "job_file", R"DOC(The file a job wrote (a decompile's C), from character `offset`.)DOC",
     {{"job_id", "string", nullptr}, {"offset", "integer", "0"}, {"length", "integer", "60000"}}},
    {"reach_results", "reach_results", R"DOC(The results `reach` kept (state/reach/<run>.json). Without `run`, the runs there are. With
it, the programs found: what each is, its platform and lane, and for those judged the tally;
`platform` and `find` (part of a path) narrow it, plus a count per platform.)DOC",
     {{"run", "string", "\"\""}, {"platform", "string", "\"\""}, {"judged_only", "boolean", "false"}, {"find", "string", "\"\""}, {"offset", "integer", "0"}, {"limit", "integer", "100"}}},
    {"projects", "projects", R"DOC(The project folders under Converted/ (one per binary CodeBreaker has taken apart): name,
the original's file name, its kind and the commands run on it.)DOC",
     {{"find", "string", "\"\""}}},
    {"project", "project", R"DOC(One project folder: its project.json (what was done, each output) and its files.)DOC",
     {{"name", "string", nullptr}, {"find", "string", "\"\""}}},
    {"source_tree", "source_tree", R"DOC(Where a module's source lives in the product's source tree (and why there), and its files,
each marked source, notes (.md, .txt) or built (a build's output or working folder); counts
of each, source first.)DOC",
     {{"module", "string", nullptr}, {"find", "string", "\"\""}}},
    {"read_file", "read_file", R"DOC(A text file from a project folder, the source tree, state/ or this checkout, from character
`offset` (a binary file: use hex_dump).)DOC",
     {{"path", "string", nullptr}, {"offset", "integer", "0"}, {"length", "integer", "40000"}}},
    {"decompile_function", "decompile_function", R"DOC(One function decompiled now, without starting a job: a 32-bit x86 function by itself (cached
by its inputs); any other kind decompiles its whole module once (kept) and the function is cut
out of it. `addr` is an address (hex) or a name. Not built or judged, and not necessarily the
text the store holds (that was made by the measure that judged it: `function`). Says so when it
takes longer than `wait` seconds: call again.)DOC",
     {{"module", "string", nullptr}, {"addr", "string", nullptr}, {"wait", "integer", "60"}}},
    {"compare_versions", "compare_versions", R"DOC(Two builds of a module function by function (compare/versions.py): identical, changed (with
a likeness score), added, removed, and the names the old build gives functions the new one
leaves unnamed. Changed pairs come first. `status` keeps identical or changed pairs only.)DOC",
     {{"module_a", "string", nullptr}, {"module_b", "string", nullptr}, {"status", "string", "\"\""}, {"offset", "integer", "0"}, {"limit", "integer", "200"}, {"wait", "integer", "60"}}},
    {"diff_function", "diff_function", R"DOC(One function in two builds, instruction by instruction: `addr` is its address (or name) in
the first build, or in the second. The listings are lined up by the kinds of instruction and
operand, each line marked ' ' the same (addresses aside), '~' the same instruction with other
registers or constants, '-' only in the first, '+' only in the second.)DOC",
     {{"module_a", "string", nullptr}, {"module_b", "string", nullptr}, {"addr", "string", nullptr}, {"wait", "integer", "60"}}},
    {"disasm", "disasm", R"DOC(The instructions at an address or function name. At a function's start (count 0): the whole
function as the analyses decode it, following its branches. Anywhere else, or with `count`: that
many instructions read straight on from there (64 when count is 0). Each line: address, file
offset (32-bit modules), bytes, instruction, and what it refers to (a function, an import by its
name, a string).)DOC",
     {{"module", "string", nullptr}, {"addr", "string", nullptr}, {"count", "integer", "0"}}},
    {"xrefs", "xrefs", R"DOC(What refers to an address or function, and what a function calls: the call and jump sites
that go there, the code that reads, writes or takes the address, the data that holds it, and,
for a function, each call it makes. `addr` may also be an import, as MODULE!name, its name
alone, or MODULE!#ordinal: then the code that calls it, directly or through a thunk.)DOC",
     {{"module", "string", nullptr}, {"addr", "string", nullptr}}},
    {"triage", "triage", R"DOC(A module at a glance: what it is, its exports and imports, what made it, where the function
store stands (counts, and each unverified function with its verdict's detail), the functions
a tool or runtime library made, and where its authored source lives. What to look at next.)DOC",
     {{"module", "string", nullptr}}},
    {"run_command", "run_command", R"DOC(Start `python -m codebreaker <command> <args...>` as a background job; returns at once with
a job id (follow it with `job`). For anything that takes more than a few seconds: decompile,
measure, reach --judge, pe-source, recompile-project, xna, java, ... At most two jobs run at a
time and only one measure.)DOC",
     {{"command", "string", nullptr}, {"args", "array|null", "null"}}},
    {"stop_job", "stop_job", R"DOC(Stop a job this server started (only that process is stopped).)DOC",
     {{"job_id", "string", nullptr}}},
    {"name_function", "name_function", R"DOC(Give the function at `addr` a name in the function store. The name goes on its body, so
every module holding the same code gets it too (the next module write uses it).)DOC",
     {{"module", "string", nullptr}, {"addr", "string", nullptr}, {"name", "string", nullptr}}},
    };
    return t;
}

const size_t N_READING = 29;   // the tools before run_command

using Invoke = std::function<ToolValue(const Json& a)>;

py::str S_(const Json& a, const char* k) { return a.at(k).str(); }
int64_t I_(const Json& a, const char* k) { return a.at(k).i; }
bool B_(const Json& a, const char* k) { return a.at(k).b; }
ToolValue TV_(Json j) {
    ToolValue v;
    v.json = std::move(j);
    return v;
}

const std::map<std::string, Invoke>& invokers() {
    static const std::map<std::string, Invoke> m = {
        {"describe", [](const Json& a) { return TV_(describe(S_(a, "path"))); }},
        {"inspect_module", [](const Json& a) { return TV_(inspect_module(S_(a, "module"), S_(a, "section"))); }},
        {"strings", [](const Json& a) { return TV_(strings(S_(a, "module"), S_(a, "find"), I_(a, "min_len"), I_(a, "offset"), I_(a, "limit"))); }},
        {"hex_dump", [](const Json& a) { return TV_(hex_dump(S_(a, "module"), I_(a, "offset"), I_(a, "length"), S_(a, "addr"))); }},
        {"resource", [](const Json& a) { return resource(S_(a, "module"), I_(a, "offset"), I_(a, "size"), I_(a, "type_id"), B_(a, "wide")); }},
        {"modules", [](const Json& a) { return TV_(modules(S_(a, "kind"), S_(a, "find"), B_(a, "only_in_store"), I_(a, "offset"), I_(a, "limit"), B_(a, "refresh"))); }},
        {"coverage", [](const Json& a) { return TV_(coverage(B_(a, "refresh"))); }},
        {"progress", [](const Json&) { return TV_(progress()); }},
        {"functions", [](const Json& a) { return TV_(functions(S_(a, "module"), S_(a, "cls"), S_(a, "find"), I_(a, "offset"), I_(a, "limit"))); }},
        {"function", [](const Json& a) { return TV_(function(S_(a, "module"), S_(a, "addr"))); }},
        {"names", [](const Json& a) { return TV_(names(S_(a, "module"), S_(a, "find"), I_(a, "offset"), I_(a, "limit"))); }},
        {"identify", [](const Json& a) { return TV_(identify(S_(a, "module"))); }},
        {"demangle", [](const Json& a) {
             py::list<py::str> l;
             for (const auto& x : a.at("decorated").a) l.append(x.str());
             return TV_(demangle(l));
         }},
        {"commands", [](const Json&) { return TV_(commands()); }},
        {"command_help", [](const Json& a) { return TV_(command_help(S_(a, "command"))); }},
        {"jobs", [](const Json&) { return TV_(jobs()); }},
        {"job", [](const Json& a) { return TV_(job(S_(a, "job_id"), I_(a, "offset"), I_(a, "tail"))); }},
        {"job_file", [](const Json& a) { return TV_(job_file(S_(a, "job_id"), I_(a, "offset"), I_(a, "length"))); }},
        {"reach_results", [](const Json& a) { return TV_(reach_results(S_(a, "run"), S_(a, "platform"), B_(a, "judged_only"), S_(a, "find"), I_(a, "offset"), I_(a, "limit"))); }},
        {"projects", [](const Json& a) { return TV_(projects(S_(a, "find"))); }},
        {"project", [](const Json& a) { return TV_(project(S_(a, "name"), S_(a, "find"))); }},
        {"source_tree", [](const Json& a) { return TV_(source_tree(S_(a, "module"), S_(a, "find"))); }},
        {"read_file", [](const Json& a) { return TV_(read_file(S_(a, "path"), I_(a, "offset"), I_(a, "length"))); }},
        {"decompile_function", [](const Json& a) { return TV_(decompile_function(S_(a, "module"), S_(a, "addr"), I_(a, "wait"))); }},
        {"compare_versions", [](const Json& a) { return TV_(compare_versions(S_(a, "module_a"), S_(a, "module_b"), S_(a, "status"), I_(a, "offset"), I_(a, "limit"), I_(a, "wait"))); }},
        {"diff_function", [](const Json& a) { return TV_(diff_function(S_(a, "module_a"), S_(a, "module_b"), S_(a, "addr"), I_(a, "wait"))); }},
        {"disasm", [](const Json& a) { return TV_(disasm(S_(a, "module"), S_(a, "addr"), I_(a, "count"))); }},
        {"xrefs", [](const Json& a) { return TV_(xrefs(S_(a, "module"), S_(a, "addr"))); }},
        {"triage", [](const Json& a) { return TV_(triage(S_(a, "module"))); }},
        {"run_command", [](const Json& a) {
             std::optional<py::list<py::str>> args;
             const Json& l = a.at("args");
             if (!l.is_null()) {
                 py::list<py::str> v;
                 for (const auto& x : l.a) v.append(x.str());
                 args = v;
             }
             return TV_(run_command(S_(a, "command"), args));
         }},
        {"stop_job", [](const Json& a) { return TV_(stop_job(S_(a, "job_id"))); }},
        {"name_function", [](const Json& a) { return TV_(name_function(S_(a, "module"), S_(a, "addr"), S_(a, "name"))); }},
    };
    return m;
}

// ---- pydantic's validation of a tool's arguments -----------------------------------------------------
struct ToolError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Problem {
    std::string loc, msg, type;
    const Json* value;   // null: the whole input
};

std::string trunc_repr_(const Json& v) {
    std::string r = py_repr(v);
    return r.size() > 50 && v.kind == Json::Str ? r.substr(0, 25) + "..." + r.substr(r.size() - 24) : r;
}

// the arguments of a tool call as the function takes them: every parameter, defaults filled in, types as pydantic's lax mode makes them
Json bind_(const ToolSpec& spec, const Json& raw) {
    Json given = raw.is_obj() ? raw : Json::object();
    Json out = Json::object();
    std::vector<Problem> bad;
    Json coerced[16];
    size_t slot = 0;
    for (const auto& p : spec.params) {
        const Json* v = given.find(p.name);
        const std::string type = p.type;
        if (!v) {
            if (!p.def) {
                bad.push_back({p.name, "Field required", "missing", nullptr});
                continue;
            }
            out.set(p.name, loads(p.def));
            continue;
        }
        if (type == "string") {
            if (!v->is_str()) bad.push_back({p.name, "Input should be a valid string", "string_type", v});
            else out.set(p.name, *v);
        } else if (type == "integer") {
            if (v->kind == Json::Int) out.set(p.name, *v);
            else if (v->kind == Json::Bool) out.set(p.name, Json(static_cast<int64_t>(v->b)));
            else if (v->kind == Json::Float && v->d == static_cast<double>(static_cast<int64_t>(v->d))) out.set(p.name, Json(static_cast<int64_t>(v->d)));
            else if (v->kind == Json::Float) bad.push_back({p.name, "Input should be a valid integer, got a number with a fractional part", "int_from_float", v});
            else if (v->is_str()) {
                try {
                    out.set(p.name, Json(py::int_parse(py::str(v->s), 10)));
                } catch (const py::ValueError&) {
                    bad.push_back({p.name, "Input should be a valid integer, unable to parse string as an integer", "int_parsing", v});
                }
            } else bad.push_back({p.name, "Input should be a valid integer", "int_type", v});
        } else if (type == "boolean") {
            static const std::set<std::string> yes = {"true", "1", "yes", "on", "t", "y"}, no = {"false", "0", "no", "off", "f", "n"};
            if (v->kind == Json::Bool) out.set(p.name, *v);
            else if (v->kind == Json::Int && (v->i == 0 || v->i == 1)) out.set(p.name, Json(v->i == 1));
            else if (v->is_str() && yes.count(lower8(v->s))) out.set(p.name, Json(true));
            else if (v->is_str() && no.count(lower8(v->s))) out.set(p.name, Json(false));
            else if (v->is_str() || v->kind == Json::Int) bad.push_back({p.name, "Input should be a valid boolean, unable to interpret input", "bool_parsing", v});
            else bad.push_back({p.name, "Input should be a valid boolean", "bool_type", v});
        } else {   // "array" / "array|null": list[str]
            if (type == "array|null" && v->is_null()) {
                out.set(p.name, *v);
                continue;
            }
            if (!v->is_arr()) {
                bad.push_back({p.name, "Input should be a valid list", "list_type", v});
                continue;
            }
            bool ok = true;
            for (size_t k = 0; k < v->a.size(); k++)
                if (!v->a[k].is_str()) {
                    bad.push_back({std::string(p.name) + "." + std::to_string(k), "Input should be a valid string", "string_type", &v->a[k]});
                    ok = false;
                }
            if (ok) out.set(p.name, *v);
        }
    }
    (void)coerced;
    (void)slot;
    if (!bad.empty()) {
        std::string m = std::to_string(bad.size()) + " validation error" + (bad.size() > 1 ? "s" : "") + " for " + spec.function + "Arguments";
        for (const auto& b : bad) {
            const Json& val = b.value ? *b.value : given;
            m += "\n" + b.loc + "\n  " + b.msg + " [type=" + b.type + ", input_value=" + trunc_repr_(val) + ", input_type=" + py_type_(val) +
                 "]\n    For further information visit https://errors.pydantic.dev/2.12/v/" + b.type;
        }
        throw ToolError(m);
    }
    return out;
}

// the function's own answer to a call (the service when one runs: the Python's _forward)
ToolValue run_tool_(const ToolSpec& spec, const Json& bound) {
    auto fn = invokers().find(spec.function);
    const Json* jid = bound.find("job_id");
    if (jid && service_::JOBS().get(jid->str())) return fn->second(bound);   // a job this process started itself stays here
    if (std::string(spec.function) == "resource") return fn->second(bound);   // (an image does not go through JSON)
    return TV_(service_::call(py::str(std::string("mcp.") + spec.function), bound));
}

struct Registrar {
    Registrar() {
        for (const auto& spec : TOOL_SPECS()) {
            const ToolSpec* s = &spec;
            auto fn = invokers().find(spec.function);
            service_::tool_calls()[spec.function] = [fn](const Json& a) { return fn->second(a).json; };
            (void)s;
        }
    }
} g_registrar;

Json text_content_(const std::string& text) {
    Json c = Json::object();
    c.set("text", text);
    c.set("type", "text");
    return c;
}

Json tool_result_(Json content, bool is_error) {
    Json r = Json::object();
    Json arr = Json::array();
    arr.push(std::move(content));
    r.set("content", arr);
    r.set("isError", is_error);
    return r;
}

Json annotations_(const ToolSpec& s, size_t index) {
    Json a = Json::object();
    const std::string f = s.function;
    if (index < N_READING) {
        a.set("readOnlyHint", true);
    } else if (f == "run_command") {
        a.set("destructiveHint", true);
        a.set("openWorldHint", false);
        a.set("readOnlyHint", false);
    } else if (f == "stop_job") {
        a.set("destructiveHint", true);
        a.set("readOnlyHint", false);
    } else {
        a.set("destructiveHint", false);
        a.set("idempotentHint", true);
        a.set("readOnlyHint", false);
    }
    return a;
}

Json tool_listing_(const ToolSpec& s, size_t index) {
    static const std::string titles_unused;
    Json props = Json::object(), required = Json::array();
    for (const auto& p : s.params) {
        Json one = Json::object();
        const std::string type = p.type;
        if (type == "array|null") {
            Json any = Json::array(), arr = Json::object(), nul = Json::object();
            Json items = Json::object();
            items.set("type", "string");
            arr.set("items", items);
            arr.set("type", "array");
            nul.set("type", "null");
            any.push(arr);
            any.push(nul);
            one.set("anyOf", any);
        }
        if (p.def) one.set("default", loads(p.def));
        std::string title;
        bool up = true;
        for (char c : std::string(p.name)) {
            if (c == '_') {
                title += ' ';
                up = true;
            } else {
                title += up ? static_cast<char>(std::toupper(static_cast<unsigned char>(c))) : c;
                up = false;
            }
        }
        if (type == "array") {
            Json items = Json::object();
            items.set("type", "string");
            one.set("items", items);
        }
        one.set("title", title);
        if (type == "string" || type == "integer" || type == "boolean") one.set("type", type);
        else if (type == "array") one.set("type", "array");
        props.set(p.name, one);
        if (!p.def) required.push(p.name);
    }
    Json schema = Json::object();
    schema.set("properties", props);
    if (!required.a.empty()) schema.set("required", required);
    schema.set("type", "object");
    schema.set("title", std::string(s.function) + "Arguments");
    Json t = Json::object();
    t.set("annotations", annotations_(s, index));
    t.set("description", s.doc);
    t.set("inputSchema", schema);
    t.set("name", s.name);
    return t;
}

const char* const PROMPT_DESCRIPTION = "Take a binary apart: what it is, what is in it, where its reconstruction stands";
const char* const RESOURCE_DESCRIPTION = "One function's reconstructed source, as the store holds it";

std::string version_() {
    try {
        return hooks().version ? hooks().version().utf8() : std::string();
    } catch (const std::exception&) {
        return "";
    }
}

Json response_(const Json& id, const char* field, Json body) {
    Json r = Json::object();
    r.set("jsonrpc", "2.0");
    r.set("id", id);
    r.set(field, std::move(body));
    return r;
}
Json error_(const Json& id, int64_t code, const std::string& message, const Json* data = nullptr) {
    Json e = Json::object();
    e.set("code", code);
    e.set("message", message);
    if (data) e.set("data", *data);
    return response_(id, "error", e);
}

}  // namespace

Json handle_message(const Json& msg, bool writes) {
    if (!msg.is_obj()) return error_(Json(), -32600, "Invalid Request");
    const Json* idp = msg.find("id");
    const Json* method = msg.find("method");
    if (!method || !method->is_str()) return Json();
    const std::string m = method->s;
    if (!idp) return Json();   // a notification: no answer
    const Json id = *idp;
    const Json* params_p = msg.find("params");
    Json params = params_p && params_p->is_obj() ? *params_p : Json::object();
    if (m == "initialize") {
        static const std::vector<std::string> known = {"2024-11-05", "2025-03-26", "2025-06-18", "2025-11-25"};
        std::string want = params.find("protocolVersion") && params.at("protocolVersion").is_str() ? params.at("protocolVersion").s : "";
        std::string use = std::find(known.begin(), known.end(), want) != known.end() ? want : known.back();
        auto flag = [](bool subscribe) {
            Json o = Json::object();
            o.set("listChanged", false);
            if (subscribe) o.set("subscribe", false);
            return o;
        };
        Json caps = Json::object();
        caps.set("prompts", flag(false));
        caps.set("resources", flag(true));
        caps.set("tools", flag(false));
        Json info = Json::object();
        info.set("name", "codebreaker");
        info.set("title", "CodeBreaker");
        info.set("version", version_());
        Json r = Json::object();
        r.set("capabilities", caps);
        r.set("instructions", INSTRUCTIONS);
        r.set("protocolVersion", use);
        r.set("serverInfo", info);
        return response_(id, "result", r);
    }
    if (m == "ping") return response_(id, "result", Json::object());
    if (m == "tools/list") {
        Json tools = Json::array();
        const auto& specs = TOOL_SPECS();
        for (size_t k = 0; k < specs.size(); k++)
            if (writes || k < N_READING) tools.push(tool_listing_(specs[k], k));
        Json r = Json::object();
        r.set("tools", tools);
        return response_(id, "result", r);
    }
    if (m == "tools/call") {
        const std::string name = params.find("name") && params.at("name").is_str() ? params.at("name").s : "";
        const auto& specs = TOOL_SPECS();
        const ToolSpec* spec = nullptr;
        for (size_t k = 0; k < specs.size(); k++)
            if (name == specs[k].name && (writes || k < N_READING)) spec = &specs[k];
        if (!spec) return response_(id, "result", tool_result_(text_content_("Unknown tool: " + name), true));
        try {
            Json bound = bind_(*spec, params.find("arguments") ? *params.find("arguments") : Json::object());
            ToolValue v = run_tool_(*spec, bound);
            if (v.image) {
                static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
                const std::string& raw = v.data.raw();
                std::string enc;
                for (size_t i = 0; i < raw.size(); i += 3) {
                    unsigned n = static_cast<unsigned char>(raw[i]) << 16 | (i + 1 < raw.size() ? static_cast<unsigned char>(raw[i + 1]) << 8 : 0) |
                                 (i + 2 < raw.size() ? static_cast<unsigned char>(raw[i + 2]) : 0);
                    enc += b64[n >> 18 & 63];
                    enc += b64[n >> 12 & 63];
                    enc += i + 1 < raw.size() ? b64[n >> 6 & 63] : '=';
                    enc += i + 2 < raw.size() ? b64[n & 63] : '=';
                }
                Json c = Json::object();
                c.set("data", enc);
                c.set("mimeType", "image/" + v.format.utf8());
                c.set("type", "image");
                return response_(id, "result", tool_result_(c, false));
            }
            Dump d;
            d.item = ", ";
            d.key = ": ";
            d.indent = 2;
            d.ensure_ascii = false;
            return response_(id, "result", tool_result_(text_content_(dumps(v.json, d)), false));
        } catch (const ToolError& e) {
            return response_(id, "result", tool_result_(text_content_("Error executing tool " + name + ": " + e.what()), true));
        } catch (const py::ValueError& e) {   // _said: the failures the tools mean to report
            return response_(id, "result", tool_result_(text_content_("Error executing tool " + name + ": " + e.what()), true));
        } catch (const py::OSError& e) {
            return response_(id, "result", tool_result_(text_content_("Error executing tool " + name + ": " + e.what()), true));
        } catch (const std::exception&) {
            return response_(id, "result", tool_result_(text_content_("Error executing tool " + name), true));
        }
    }
    if (m == "prompts/list") {
        Json arg = Json::object();
        arg.set("name", "path");
        arg.set("required", true);
        Json args = Json::array();
        args.push(arg);
        Json p = Json::object();
        p.set("arguments", args);
        p.set("description", PROMPT_DESCRIPTION);
        p.set("name", "triage");
        Json arr = Json::array();
        arr.push(p);
        Json r = Json::object();
        r.set("prompts", arr);
        return response_(id, "result", r);
    }
    if (m == "prompts/get") {
        const std::string name = params.find("name") && params.at("name").is_str() ? params.at("name").s : "";
        if (name != "triage") return error_(id, 0, "Unknown prompt: " + name);
        const Json* args = params.find("arguments");
        const Json* path = args && args->is_obj() ? args->find("path") : nullptr;
        if (!path) return error_(id, 0, "Missing required arguments: {'path'}");
        std::string text = "Triage " + path->s +
                           " with the CodeBreaker tools: start with the `triage` tool (what it is, its imports and exports, the store's counts and "
                           "each unverified function with its verdict); if nothing is stored, offer to `run_command` measure on it and follow the job; "
                           "then for each function to look at, `disasm` and `xrefs` it and say what most likely went wrong and what the original code was for.";
        Json content = Json::object();
        content.set("text", text);
        content.set("type", "text");
        Json msg1 = Json::object();
        msg1.set("content", content);
        msg1.set("role", "user");
        Json msgs = Json::array();
        msgs.push(msg1);
        Json r = Json::object();
        r.set("description", PROMPT_DESCRIPTION);
        r.set("messages", msgs);
        return response_(id, "result", r);
    }
    if (m == "resources/list") {
        Json r = Json::object();
        r.set("resources", Json::array());
        return response_(id, "result", r);
    }
    if (m == "resources/templates/list") {
        Json t = Json::object();
        t.set("description", RESOURCE_DESCRIPTION);
        t.set("mimeType", "text/x-c");
        t.set("name", "function_source");
        t.set("uriTemplate", "codebreaker://function/{module}/{addr}");
        Json arr = Json::array();
        arr.push(t);
        Json r = Json::object();
        r.set("resourceTemplates", arr);
        return response_(id, "result", r);
    }
    if (m == "resources/read") {
        const std::string uri = params.find("uri") && params.at("uri").is_str() ? params.at("uri").s : "";
        Json data = Json::object();
        data.set("uri", uri);
        const std::string prefix = "codebreaker://function/";
        std::string rest = uri.rfind(prefix, 0) == 0 ? uri.substr(prefix.size()) : "";
        auto slash = rest.find('/');
        if (rest.empty() || slash == std::string::npos || slash == 0 || slash + 1 >= rest.size() || rest.find('/', slash + 1) != std::string::npos)
            return error_(id, -32602, "Unknown resource: " + uri, &data);
        try {
            Json f = function(py::str(rest.substr(0, slash)), py::str(rest.substr(slash + 1)));
            std::string text = truthy(f.find("source")) ? f.at("source").s : "/* no source: see the verdict (the `function` tool) */";
            Json c = Json::object();
            c.set("mimeType", "text/x-c");
            c.set("text", text);
            c.set("uri", uri);
            Json arr = Json::array();
            arr.push(c);
            Json r = Json::object();
            r.set("contents", arr);
            return response_(id, "result", r);
        } catch (const std::exception&) {
            return error_(id, -32603, "Error creating resource from template " + uri, &data);
        }
    }
    Json data(m);
    return error_(id, -32601, "Method not found", &data);
}

py::str token() {
    auto env = py::os::environ_::get(py::str("CB_MCP_TOKEN"));
    if (env && !env->empty()) return *env;
    py::str path = service_::state_path({py::str("mcp"), py::str("http_token.txt")});
    if (!py::os::path::isfile(path)) {
        unsigned char b[32];
        std::random_device rd;
        for (int k = 0; k < 32; k++) b[k] = static_cast<unsigned char>(rd());
        static const char* a = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
        std::string t;
        for (int i = 0; i < 32; i += 3) {
            unsigned n = b[i] << 16 | (i + 1 < 32 ? b[i + 1] << 8 : 0) | (i + 2 < 32 ? b[i + 2] : 0);
            t += a[n >> 18 & 63];
            t += a[n >> 12 & 63];
            if (i + 1 < 32) t += a[n >> 6 & 63];
            if (i + 2 < 32) t += a[n & 63];
        }
        std::ofstream f(std::filesystem::u8path(path.utf8()), std::ios::binary);
        f << t;
    }
    std::ifstream f(std::filesystem::u8path(path.utf8()), std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return py::str(ss.str()).strip();
}

// ---- streamable HTTP (`--http`): the Python's uvicorn + mcp streamable_http_app behind the bearer token -------------------------
namespace {

struct Request {
    std::string method, path, body;
    std::map<std::string, std::string> headers;   // lower-case names
    const std::string& header(const char* k) const {
        static const std::string none;
        auto it = headers.find(k);
        return it == headers.end() ? none : it->second;
    }
};

std::string http_date_() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    gmtime_s(&tm, &t);
    char b[64];
    std::strftime(b, sizeof b, "%a, %d %b %Y %H:%M:%S GMT", &tm);
    return b;
}

std::string session_id_() {
    std::random_device rd;
    char b[40];
    std::snprintf(b, sizeof b, "%08x%08x%08x%08x", rd(), rd(), rd(), rd());
    return b;
}

// `*` at the end of a pattern stands for a port (the library's "127.0.0.1:*")
bool match_pattern_(const std::string& value, const std::string& pattern) {
    if (!pattern.empty() && pattern.back() == '*') return value.compare(0, pattern.size() - 1, pattern, 0, pattern.size() - 1) == 0;
    return value == pattern;
}

struct HttpState {
    std::vector<std::string> hosts, allowed_hosts, allowed_origins;
    std::string secret;
    bool is_public = false;
    std::mutex lock;
    std::set<std::string> sessions;
};

struct Reply {
    int status = 200;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
    bool chunked = false;
};

const char* reason_(int s) {
    switch (s) {
        case 200: return "OK";
        case 202: return "Accepted";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 406: return "Not Acceptable";
        case 421: return "Misdirected Request";
        default: return "OK";
    }
}

std::string wire_(const Reply& r) {
    std::string o = "HTTP/1.1 " + std::to_string(r.status) + " " + reason_(r.status) + "\r\ndate: " + http_date_() + "\r\nserver: uvicorn\r\n";
    for (const auto& h : r.headers) o += h.first + ": " + h.second + "\r\n";
    if (r.chunked) {
        char n[32];
        std::snprintf(n, sizeof n, "%zx", r.body.size());
        o += "transfer-encoding: chunked\r\n\r\n" + std::string(n) + "\r\n" + r.body + "\r\n0\r\n\r\n";
    } else {
        o += "content-length: " + std::to_string(r.body.size()) + "\r\n\r\n" + r.body;
    }
    return o;
}

Json rpc_error_(int64_t code, const std::string& message) {
    Json e = Json::object();
    e.set("code", code);
    e.set("message", message);
    Json r = Json::object();
    r.set("jsonrpc", "2.0");
    r.set("id", Json());
    r.set("error", e);
    return r;
}

std::string compact_(const Json& j) {
    Dump d;
    d.item = ",";
    d.key = ":";
    d.ensure_ascii = false;
    return dumps(j, d);
}

Reply json_reply_(int status, const Json& j, const std::string& sid) {
    Reply r;
    r.status = status;
    r.headers = {{"content-type", "application/json"}};
    if (!sid.empty()) r.headers.push_back({"mcp-session-id", sid});
    r.body = compact_(j);
    return r;
}

Reply plain_(int status, const std::string& text, bool with_type = true) {
    Reply r;
    r.status = status;
    if (with_type) r.headers = {{"content-type", "text/plain; charset=utf-8"}};
    r.body = text;
    return r;
}

Reply answer_(HttpState& st, Request req) {
    // the bearer token (ASGI Bearer middleware): the header, or in --public the URL's first segment
    std::string got = req.header("authorization");
    const std::string prefix = "/" + st.secret;
    if (st.is_public && req.path.compare(0, prefix.size() + 1, prefix + "/") == 0) {
        req.path = req.path.substr(prefix.size());
        got = "Bearer " + st.secret;
    }
    if (got != "Bearer " + st.secret) {
        Reply r;
        r.status = 401;
        r.headers = {{"content-type", "text/plain"}, {"www-authenticate", "Bearer"}};
        r.body = "a bearer token is needed";
        r.chunked = true;
        return r;
    }
    if (req.path != "/mcp") return plain_(404, "Not Found");
    // the transport's DNS-rebinding protection: Host, Origin, and a POST's Content-Type
    bool host_ok = false;
    for (const auto& p : st.allowed_hosts)
        if (match_pattern_(req.header("host"), p)) host_ok = true;
    if (!host_ok) return plain_(421, "Invalid Host header", false);
    const std::string origin = req.header("origin");
    if (!origin.empty()) {
        bool ok = false;
        for (const auto& p : st.allowed_origins)
            if (match_pattern_(origin, p)) ok = true;
        if (!ok) return plain_(403, "Invalid Origin header", false);
    }
    if (req.method == "POST") {
        const std::string ct = lower8(req.header("content-type"));
        if (ct.compare(0, 16, "application/json") != 0) return plain_(400, "Invalid Content-Type header", false);
    }
    std::string sid = req.header("mcp-session-id");
    auto known = [&](const std::string& s) {
        std::lock_guard<std::mutex> g(st.lock);
        return st.sessions.count(s) > 0;
    };
    if (req.method == "POST" || req.method == "DELETE" || req.method == "GET") {
        if (!sid.empty() && !known(sid)) return json_reply_(404, rpc_error_(-32600, "Session not found"), "");
    }
    if (req.method == "DELETE") {
        if (sid.empty()) return json_reply_(400, rpc_error_(-32600, "Bad Request: Missing session ID"), session_id_());
        {
            std::lock_guard<std::mutex> g(st.lock);
            st.sessions.erase(sid);
        }
        Reply r;
        r.headers = {{"content-type", "application/json"}, {"mcp-session-id", sid}};
        return r;
    }
    if (req.method != "POST") {
        if (sid.empty()) return json_reply_(400, rpc_error_(-32600, "Bad Request: Missing session ID"), session_id_());
        return json_reply_(405, rpc_error_(-32600, "Method Not Allowed"), sid);   // (the library holds a stream open here: not ported)
    }
    const std::string accept = lower8(req.header("accept"));
    const bool accepts = accept.find("application/json") != std::string::npos && accept.find("text/event-stream") != std::string::npos;
    Json msg;
    bool parsed = true;
    std::string perr;
    try {
        msg = loads(req.body);
    } catch (const py::ValueError&) {
        parsed = false;
    }
    const std::string reply_sid = sid.empty() ? session_id_() : sid;
    if (!accepts) return json_reply_(406, rpc_error_(-32600, "Not Acceptable: Client must accept both application/json and text/event-stream"), reply_sid);
    if (!parsed) return json_reply_(400, rpc_error_(-32700, "Parse error: expected value at line 1 column 1"), reply_sid);
    if (!msg.is_obj()) return json_reply_(400, rpc_error_(-32602, "Validation error: the message is not a JSON-RPC object"), reply_sid);
    const Json* method = msg.find("method");
    const bool is_init = method && method->is_str() && method->s == "initialize" && msg.has("id");
    if (sid.empty()) {
        if (!is_init) return json_reply_(400, rpc_error_(-32600, "Bad Request: Missing session ID"), reply_sid);
        std::lock_guard<std::mutex> g(st.lock);
        st.sessions.insert(reply_sid);
    }
    Json reply = handle_message(msg, !st.is_public);
    if (reply.is_null()) {
        Reply r;
        r.status = 202;
        r.headers = {{"content-type", "application/json"}, {"mcp-session-id", reply_sid}};
        return r;
    }
    Reply r;
    r.headers = {{"cache-control", "no-cache, no-transform"}, {"connection", "keep-alive"}, {"content-type", "text/event-stream"},
                 {"mcp-session-id", reply_sid}, {"x-accel-buffering", "no"}};
    r.body = "event: message\r\ndata: " + compact_(reply) + "\r\n\r\n";
    r.chunked = true;
    return r;
}

void serve_connection_(HttpState& st, SOCKET s) {
    std::string buf;
    char tmp[8192];
    for (;;) {
        size_t head_end;
        while ((head_end = buf.find("\r\n\r\n")) == std::string::npos) {
            int n = recv(s, tmp, sizeof tmp, 0);
            if (n <= 0) {
                closesocket(s);
                return;
            }
            buf.append(tmp, static_cast<size_t>(n));
        }
        Request req;
        std::istringstream hs(buf.substr(0, head_end));
        std::string line;
        std::getline(hs, line);
        {
            std::istringstream rl(line);
            std::string version;
            rl >> req.method >> req.path >> version;
        }
        while (std::getline(hs, line)) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
            auto c = line.find(':');
            if (c == std::string::npos) continue;
            std::string k = lower8(line.substr(0, c)), v = line.substr(c + 1);
            req.headers[k] = py::str(v).strip().utf8();
        }
        size_t want = req.headers.count("content-length") ? static_cast<size_t>(std::stoull(req.headers["content-length"])) : 0;
        while (buf.size() < head_end + 4 + want) {
            int n = recv(s, tmp, sizeof tmp, 0);
            if (n <= 0) {
                closesocket(s);
                return;
            }
            buf.append(tmp, static_cast<size_t>(n));
        }
        req.body = buf.substr(head_end + 4, want);
        buf.erase(0, head_end + 4 + want);
        Reply r;
        try {
            r = answer_(st, req);
        } catch (const std::exception&) {
            r = plain_(500, "Internal Server Error");
        }
        const std::string out = wire_(r);
        for (size_t sent = 0; sent < out.size();) {
            int n = send(s, out.data() + sent, static_cast<int>(out.size() - sent), 0);
            if (n <= 0) {
                closesocket(s);
                return;
            }
            sent += static_cast<size_t>(n);
        }
    }
}

int64_t serve_http_(int64_t port, const std::string& host, const std::vector<std::string>& hosts, bool is_public) {
    WSADATA d;
    WSAStartup(MAKEWORD(2, 2), &d);
    HttpState st;
    st.secret = token().utf8();
    st.is_public = is_public;
    st.allowed_hosts = {"127.0.0.1:*", "localhost:*"};
    st.allowed_origins = {"http://127.0.0.1:*", "http://localhost:*"};
    for (const auto& h : hosts) {
        st.allowed_hosts.push_back(h);
        st.allowed_origins.push_back("https://" + h);
    }
    SOCKET ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, host.c_str(), &a.sin_addr);
    if (bind(ls, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0 || listen(ls, 64) != 0) {
        std::fprintf(stderr, "cannot listen on %s:%lld\n", host.c_str(), static_cast<long long>(port));
        return 1;
    }
    for (;;) {
        SOCKET c = accept(ls, nullptr, nullptr);
        if (c == INVALID_SOCKET) break;
        std::thread([&st, c] { serve_connection_(st, c); }).detach();
    }
    return 0;
}

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    service_::START = true;   // the service, started on demand
    std::thread([] {
        try {
            service_::client();
        } catch (const std::exception&) {
        }
    }).detach();   // (found or started while the client connects)
    auto after = [&](const char* flag) -> std::optional<std::string> {
        for (size_t i = 0; i + 1 < argv.size(); i++)
            if (argv[i] == py::str(flag)) return argv[i + 1].utf8();
        return std::nullopt;
    };
    bool http = false;
    for (const auto& a : argv)
        if (a == py::str("--http")) http = true;
    if (http) {
        int64_t port = py::int_parse(py::str(*after("--http")), 10);
        std::string host = after("--host").value_or("127.0.0.1");
        std::vector<std::string> hosts;
        for (size_t i = 0; i + 1 < argv.size(); i++)
            if (argv[i] == py::str("--allow-host")) hosts.push_back(argv[i + 1].utf8());
        bool is_public = false;
        for (const auto& a : argv)
            if (a == py::str("--public")) is_public = true;
        token();
        std::fprintf(stderr, "CodeBreaker MCP on http://%s:%lld/mcp%s -- token in %s\n", host.c_str(), static_cast<long long>(port),
                     is_public ? " (public: reading tools only, also at /<token>/mcp)" : "",
                     py::os::environ_::get(py::str("CB_MCP_TOKEN")) ? "CB_MCP_TOKEN" : service_::state_path({py::str("mcp"), py::str("http_token.txt")}).utf8().c_str());
        std::fflush(stderr);
        int64_t rc = serve_http_(port, host, hosts, is_public);
        service_::stop_mine();
        return rc;
    }
    // Stdout is the protocol: file 1 is pointed at stderr while serving, so anything the commands print cannot corrupt it.
    std::fflush(stdout);
    int proto = _dup(1);
    _dup2(2, 1);
    _setmode(proto, _O_BINARY);
    // The library answers each request on a task of its own, so replies come as the requests finish (the order of a client that
    // waits for each reply is the same); at end of input what is still running is cancelled, as here (the process ends).
    auto write_lock = std::make_shared<std::mutex>();
    std::string line;
    while (std::getline(std::cin, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
        if (strip8_(line).empty()) continue;
        std::thread([line, proto, write_lock] {
            Json reply;
            try {
                reply = handle_message(loads(line), true);
            } catch (const py::ValueError& e) {
                Json data(std::string(e.what()));
                reply = error_(Json(), -32700, "Parse error", &data);
            }
            if (reply.is_null()) return;
            std::string out = compact_(reply) + "\n";
            std::lock_guard<std::mutex> g(*write_lock);
            _write(proto, out.data(), static_cast<unsigned>(out.size()));
        }).detach();
    }
    service_::stop_mine();   // (only the jobs this server started)
    return 0;
}

}  // namespace codebreaker::ui::mcp_server
