// codebreaker/exact/toolchain.cpp -- typed C++ of codebreaker.exact.toolchain (see toolchain.hpp).

#include "codebreaker/exact/toolchain.hpp"

#include <process.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/exact/hooks_lattice.hpp"
#include "codebreaker/formats/omf.hpp"
#include "json.hpp"
#include "py_settings.hpp"
#include "value.hpp"

namespace codebreaker::exact::toolchain {

using py::str;
namespace re = py::re;
namespace dos = codebreaker::exact::emu::dos;
namespace cache = codebreaker::base::cache;
namespace omf = codebreaker::formats::omf;

const str CACHE_VERSION("1");

namespace {

std::filesystem::path fspath(const str& s) { return std::filesystem::u8path(s.utf8()); }

str slurp_text(const str& path) {
    py::file f = py::open(path, str("r"), str("utf-8"));
    str t = f.read_text();
    f.close();
    return t;
}

py::bytes read_bytes(const str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read();
    f.close();
    return b;
}

void write_bytes(const str& path, const py::bytes& b) {
    std::ofstream f(fspath(path), std::ios::binary);
    if (!f) throw py::OSError("[Errno 22] Invalid argument: " + py::repr(path).utf8());
    f.write(b.raw().data(), static_cast<std::streamsize>(b.raw().size()));
}

// the settings.json of the checkout, read once (settings._load)
const py::V& settings_json() {
    static py::V loaded;
    static bool done = false;
    if (!done) {
        py::V v = py::V::dict();
        std::ifstream f(fspath(py::os::path::join(py::settings::checkout_root(), str("settings.json"))), std::ios::binary);
        if (f) {
            std::stringstream ss;
            ss << f.rdbuf();
            v = py::json_parse(ss.str());
        }
        loaded = v;
        done = true;
    }
    return loaded;
}

// settings.state(*parts): a path inside the state folder; the folder of the path is made
str state_path(const std::vector<str>& parts) {
    str path = py::settings::get_str(str("state"));
    for (const str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

bool truthy(const std::optional<str>& s) { return s.has_value() && !s->empty(); }

std::string lower_hex(unsigned char c) {
    static const char* d = "0123456789abcdef";
    return std::string{d[c >> 4], d[c & 15]};
}

// json.dumps(v, sort_keys=True): the keys of every dict in order
std::string dump_sorted(const py::V& v) {
    if (v.is_dict()) {
        std::vector<std::pair<py::V, py::V>> items = v.items();
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first.str() < b.first.str(); });
        std::string o = "{";
        for (size_t i = 0; i < items.size(); i++) {
            if (i) o += ", ";
            o += py::json_dump(items[i].first) + ": " + dump_sorted(items[i].second);
        }
        return o + "}";
    }
    if (v.is_list() || v.is_tuple()) {
        std::string o = "[";
        const auto& s = v.seq();
        for (size_t i = 0; i < s.size(); i++) {
            if (i) o += ", ";
            o += dump_sorted(s[i]);
        }
        return o + "]";
    }
    return py::json_dump(v);
}

Num num_of(const py::V& v) {
    if (v.is_str()) return str(v.str());
    if (v.is_intlike()) return v.as_int();
    throw py::TypeError("a number or a number written as text was expected");
}

Change change_of(const py::V& v) {
    if (v.is_str()) return str(v.str());
    if (v.is_intlike()) return v.as_int();
    throw py::TypeError("a patch's change is a number or a word");
}

str need_str(const py::V& v) {
    if (!v.is_str()) throw py::TypeError("a str was expected");
    return str(v.str());
}

const py::V& need(const py::V& d, const char* key) {
    if (!d.has(key)) throw py::KeyError(py::repr(str(key)).utf8());
    return d.get(key);
}

}  // namespace

str HERE() { return py::os::path::join(py::settings::checkout_root(), str("codebreaker"), str("exact")); }

str kit_dir(const str& name) {
    const py::V& all = settings_json();
    std::optional<str> env = py::os::getenv(str("CB_KIT_") + name.upper());
    str path;
    if (truthy(env)) {
        path = *env;
    } else {
        const py::V& kits = all.has("kits") ? all.get("kits") : py::NONE;
        if (all.has("kits") && !kits.is_dict()) throw py::AttributeError("object has no attribute 'get'");
        const py::V& e = kits.is_dict() ? kits.get(name.utf8().c_str()) : py::NONE;
        path = e.is_str() ? str(e.str()) : str("");
    }
    if (path.empty())
        throw py::RuntimeError(py::percent(str("the kit %r is not set up (settings.json: \"kits\": {\"%s\": folder})"), name, name).utf8());
    if (!py::os::path::isabs(path)) path = py::os::path::normpath(py::os::path::join(py::settings::checkout_root(), path));
    return path;
}

str kit_path(const std::optional<str>& kit, const str& path, const std::optional<str>& base) {
    int64_t at = path.find(str(":"));
    if (at >= 0) {
        str k = path.slice(0, at), rest = path.slice_from(at + 1);
        if (py::len(k) > 1) return py::os::path::join(kit_dir(k), rest);
    }
    if (truthy(kit)) return py::os::path::join(kit_dir(*kit), path);
    return py::os::path::normpath(py::os::path::join(base.has_value() ? *base : str("."), path));
}

py::list<str> profiles() {
    py::list<str> out;
    std::vector<str> names;
    for (const str& f : py::os::listdir(py::os::path::join(HERE(), str("toolchains"))))
        if (f.endswith(str(".json"))) names.push_back(f.slice(0, py::len(f) - 5));
    std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
    for (const str& n : names) out.append(n);
    return out;
}

int64_t _int(const Num& x) {
    if (std::holds_alternative<str>(x)) return py::int_parse(std::get<str>(x), 0);
    return std::get<int64_t>(x);
}

// ------------------------------------------------------------------------------------------ the profile
Profile profile_from_json(const str& text) {
    py::V v = py::json_parse(text.utf8());
    if (!v.is_dict()) throw py::TypeError("a profile is a JSON object");
    Profile p;
    p.name = need_str(need(v, "name"));
    if (v.get("kit").is_str()) p.kit = str(v.get("kit").str());
    if (v.get("default_variant").is_str()) p.default_variant = str(v.get("default_variant").str());
    if (v.has("emulator") && v.get("emulator").is_str()) p.emulator = str(v.get("emulator").str());
    if (v.get("stage").is_dict()) {
        const py::V& st = v.get("stage");
        if (st.get("files").is_dict())
            for (const auto& kv : st.get("files").items()) p.stage.files[str(kv.first.str())] = need_str(kv.second);
        if (st.get("include").is_str()) p.stage.include = str(st.get("include").str());
        p.stage.all = py::truthy(st.get("all"));
    }
    p.psp_set = v.has("psp");
    if (!v.get("psp").is_none()) p.psp = num_of(v.get("psp"));
    if (!v.get("top").is_none()) p.top = num_of(v.get("top"));
    if (v.get("env").is_list())
        for (const py::V& e : v.get("env").seq()) p.env.append(need_str(e));
    if (v.get("env_unless").is_dict())
        for (const auto& kv : v.get("env_unless").items()) p.env_unless[str(kv.first.str())] = need_str(kv.second);
    if (v.get("switches").is_str()) p.switches = str(v.get("switches").str());
    if (v.get("file_switches").is_str()) p.file_switches = str(v.get("file_switches").str());
    if (v.get("file_switches_comment").is_str()) p.file_switches_comment = str(v.get("file_switches_comment").str());
    if (v.get("derive").is_dict()) {
        for (const auto& kv : v.get("derive").items()) {
            Derive d;
            d.regex = need_str(need(kv.second, "regex"));
            if (kv.second.get("then").is_str()) d.then_ = str(kv.second.get("then").str());
            if (kv.second.get("else").is_str()) d.else_ = str(kv.second.get("else").str());
            p.derive[str(kv.first.str())] = d;
        }
    }
    if (v.has("passes")) {
        p.has_passes = true;
        for (const py::V& ps : v.get("passes").seq()) {
            Pass x;
            x.run = need_str(need(ps, "run"));
            if (ps.get("args").is_str()) x.args = str(ps.get("args").str());
            if (ps.get("env").is_list())
                for (const py::V& e : ps.get("env").seq()) x.env.append(need_str(e));
            if (ps.get("listing").is_str()) x.listing = str(ps.get("listing").str());
            p.passes.append(x);
        }
    }
    if (v.get("output").is_str()) p.output = str(v.get("output").str());
    if (v.get("source_ext").is_str()) p.source_ext = str(v.get("source_ext").str());
    p.source_eof = py::truthy(v.get("source_eof"));
    if (v.get("patches").is_list()) {
        for (const py::V& x : v.get("patches").seq()) {
            Patch pt;
            pt.change = change_of(need(x, "change"));
            pt.file = need_str(need(x, "file"));
            pt.at = num_of(need(x, "at"));
            pt.was = need_str(need(x, "was"));
            pt.now = need_str(need(x, "now"));
            pt.in_memory = py::truthy(x.get("in_memory"));
            p.patches.append(pt);
        }
    }
    if (v.get("variants").is_dict()) {
        for (const auto& kv : v.get("variants").items()) {
            Variant var;
            if (kv.second.is_dict()) {
                if (kv.second.get("patches").is_list())
                    for (const py::V& c : kv.second.get("patches").seq()) var.patches.append(change_of(c));
                if (kv.second.get("hooks").is_list())
                    for (const py::V& c : kv.second.get("hooks").seq()) var.hooks.append(need_str(c));
            } else {
                for (const py::V& c : kv.second.seq()) var.patches.append(change_of(c));
            }
            p.variants[str(kv.first.str())] = var;
        }
    }
    if (v.get("hooks").is_dict()) {
        for (const auto& kv : v.get("hooks").items()) {
            Hook h;
            h.pass_ = need_str(need(kv.second, "pass"));
            if (kv.second.get("header").is_str()) h.header = str(kv.second.get("header").str());
            if (kv.second.get("call").is_str()) h.call = str(kv.second.get("call").str());
            if (kv.second.has("at"))
                for (const py::V& a : kv.second.get("at").seq()) h.at.append(num_of(a));
            if (kv.second.get("set").is_dict())
                for (const auto& rv : kv.second.get("set").items()) h.set[str(rv.first.str())] = num_of(rv.second);
            p.hooks[str(kv.first.str())] = h;
        }
    }
    if (v.get("rename_externs").is_dict()) {
        p.has_rename_externs = true;
        for (const auto& kv : v.get("rename_externs").items()) p.rename_externs[str(kv.first.str())] = need_str(kv.second);
    }
    if (v.get("plugin").is_str()) p.plugin = str(v.get("plugin").str());
    for (const auto& kv : v.items())
        if (kv.second.is_str()) p.strings[str(kv.first.str())] = str(kv.second.str());
    // keyed = dict(profile, passes=[{k: v for k, v in ps.items() if k != "listing"} for ps in profile.get("passes", [])])
    py::V keyed = py::copy_dict(v);
    py::V passes = py::V::list();
    if (v.get("passes").is_list()) {
        for (const py::V& ps : v.get("passes").seq()) {
            py::V q = py::V::dict();
            for (const auto& kv : ps.items())
                if (!(kv.first.is_str() && kv.first.str() == "listing")) q.set(kv.first, kv.second);
            passes.append(q);
        }
    }
    keyed.set("passes", passes);
    p.keyed = str(dump_sorted(keyed));
    return p;
}

// ------------------------------------------------------------------------------------------ str.format
str format_named(const str& tmpl, const py::dict<str, str>& values) {
    const std::u32string& u = tmpl.u32();
    std::u32string out;
    for (size_t i = 0; i < u.size(); ++i) {
        char32_t c = u[i];
        if (c == U'{') {
            if (i + 1 < u.size() && u[i + 1] == U'{') {
                out += U'{';
                ++i;
                continue;
            }
            size_t j = u.find(U'}', i);
            if (j == std::u32string::npos) throw py::ValueError("expected '}' before end of string");
            str field(u.substr(i + 1, j - i - 1));
            if (field.empty()) throw py::IndexError("Replacement index 0 out of range for positional args tuple");
            for (char32_t fc : field.u32())
                if (fc == U'!' || fc == U':' || fc == U'.' || fc == U'[' || fc == U'{') throw py::ValueError("unsupported replacement field " + field.utf8());
            if (!values.contains(field)) throw py::KeyError(py::repr(field).utf8());
            out += values.at(field).u32();
            i = j;
        } else if (c == U'}') {
            if (i + 1 < u.size() && u[i + 1] == U'}') {
                out += U'}';
                ++i;
            } else {
                throw py::ValueError("Single '}' encountered in format string");
            }
        } else {
            out += c;
        }
    }
    return str(out);
}

// ------------------------------------------------------------------------------------------ hooks
int64_t x86_reg(const str& name) {
    static const std::map<std::string, int64_t> regs = {
        {"AH", 1}, {"AL", 2}, {"AX", 3}, {"BH", 4}, {"BL", 5}, {"BP", 6}, {"BPL", 7}, {"BX", 8}, {"CH", 9}, {"CL", 10}, {"CS", 11},
        {"CX", 12}, {"DH", 13}, {"DI", 14}, {"DIL", 15}, {"DL", 16}, {"DS", 17}, {"DX", 18}, {"EAX", 19}, {"EBP", 20}, {"EBX", 21},
        {"ECX", 22}, {"EDI", 23}, {"EDX", 24}, {"EFLAGS", 25}, {"EIP", 26}, {"ES", 28}, {"ESI", 29}, {"ESP", 30}, {"FS", 32},
        {"GS", 33}, {"IP", 34}, {"SI", 45}, {"SIL", 46}, {"SP", 47}, {"SPL", 48}, {"SS", 49}, {"FLAGS", 252}};
    auto it = regs.find(name.utf8());
    if (it == regs.end()) throw py::AttributeError("module 'unicorn.x86_const' has no attribute 'UC_X86_REG_" + name.utf8() + "'");
    return it->second;
}

namespace {
std::map<std::u32string, HookFn>& hook_registry() {
    static std::map<std::u32string, HookFn> r = {
        {std::u32string(U"codebreaker.exact.hooks_lattice:cmp_test"),
         [](const Reader& rd, int64_t bp, int64_t ss, int64_t ds) -> std::optional<int64_t> {
             return codebreaker::exact::hooks_lattice::cmp_test(rd, bp, ss, ds);
         }}};
    return r;
}
}  // namespace

void register_hook(const str& module_function, const HookFn& fn) { hook_registry()[module_function.u32()] = fn; }

Instrument _hook(const Hook& h, const py::set<int64_t>& counts) {
    auto n = std::make_shared<int64_t>(0);
    return [h, counts, n](HookEngine& mu, int64_t load) {
        for (const Num& a0 : h.at) {
            int64_t a = load * 16 + _int(a0);
            mu.hook_code(a, a, [&mu, h, counts, n](HookEngine&, int64_t, int64_t) {
                *n += 1;
                if (counts.contains(*n))
                    for (const auto& rv : h.set.items()) mu.reg_write(x86_reg(std::get<0>(rv).upper()), _int(std::get<1>(rv)));
            });
        }
    };
}

Instrument _call_hooks(const py::list<Hook>& hooks, const std::optional<Instrument>& then) {
    return [hooks, then](HookEngine& mu, int64_t load) {
        for (const Hook& h : hooks) {
            // mod, _, fn = h["call"].partition(":"); f = getattr(importlib.import_module(mod), fn)
            str call = h.call.has_value() ? *h.call : str("");
            int64_t colon = call.find(str(":"));
            str mod = colon >= 0 ? call.slice(0, colon) : call;
            auto it = hook_registry().find(call.u32());
            if (it == hook_registry().end()) {
                bool known = false;                                      // (a module with functions registered: it imports, the function is not in it)
                for (const auto& r : hook_registry()) {
                    size_t c = r.first.find(U':');
                    if (c != std::u32string::npos && r.first.substr(0, c) == mod.u32()) known = true;
                }
                if (known) throw py::AttributeError("module '" + mod.utf8() + "' has no attribute '" + call.slice_from(colon + 1).utf8() + "'");
                throw py::ModuleNotFoundError("No module named '" + mod.utf8() + "'");
            }
            HookFn f = it->second;
            for (const Num& a0 : h.at) {
                int64_t a = load * 16 + _int(a0);
                mu.hook_code(a, a, [f](HookEngine& uc, int64_t, int64_t) {
                    Reader rd = [&uc](int64_t seg, int64_t o) {
                        return py::unpack_from1<int64_t>("<H", uc.mem_read(seg * 16 + (o & 0xFFFF), 2));
                    };
                    std::optional<int64_t> to = f(rd, uc.reg_read(x86_reg(str("BP"))), uc.reg_read(x86_reg(str("SS"))), uc.reg_read(x86_reg(str("DS"))));
                    if (to.has_value()) uc.reg_write(x86_reg(str("IP")), *to);
                });
            }
        }
        if (then.has_value()) (*then)(mu, load);
    };
}

std::tuple<Instrument, When> _in_memory(const py::list<Patch>& mem, const std::optional<Instrument>& then) {
    struct P { int64_t at; py::bytes was, now; };
    auto pairs = std::make_shared<std::vector<P>>();
    for (const Patch& x : mem) pairs->push_back({_int(x.at), py::bytes::fromhex(x.was), py::bytes::fromhex(x.now)});
    When when = [pairs](HookEngine& uc, int64_t, int64_t load) {
        for (const P& p : *pairs)
            if (uc.mem_read(load * 16 + p.at, py::len(p.was)) != p.was) return false;
        return true;
    };
    Instrument inst = [pairs, then](HookEngine& mu, int64_t load) {
        for (const P& p : *pairs) mu.mem_write(load * 16 + p.at, p.now);
        if (then.has_value()) (*then)(mu, load);
    };
    return {inst, when};
}

// ------------------------------------------------------------------------------------------ running
namespace {

dos::Result builtin_run(const RunArgs& a) {
    if (a.instrument.has_value() || a.instrument_when.has_value())
        throw py::NotImplementedError(
            "the pass needs hooks in the emulator (counted hooks, call hooks or in-memory patches): set_runner with an instrumenting executive");
    return dos::run(a.exe, a.workdir, a.cmdline, py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false, a.env, a.psp, a.top, a.fast, a.fs);
}

Runner& runner() {
    static Runner r = builtin_run;
    return r;
}

int64_t& seq() {
    static int64_t n = 0;
    return n;
}

// MEMFS = os.environ.get("CB_MEMFS", "1") != "0", read once (and assignable: set_memfs)
std::optional<bool>& memfs_override() {
    static std::optional<bool> o;
    return o;
}
bool memfs() {
    static const bool on = py::os::getenv(str("CB_MEMFS"), str("1")) != "0";
    return memfs_override().has_value() ? *memfs_override() : on;
}

void remove_quietly(const str& p) {   // _remove
    try {
        if (py::os::path::exists(p)) py::os::remove(p);
    } catch (const py::OSError&) {    // (still open: left for the next stage's clean-up)
    }
}

py::bytes crlf(const py::bytes& b) { return b.replace(py::bytes("\r\n"), py::bytes("\n")).replace(py::bytes("\n"), py::bytes("\r\n")); }

// bytes.decode("latin1") for the patterns written for bytes
str latin1(const py::bytes& b) { return b.decode("latin1"); }

// the slice b[lo:hi] of a string of n bytes, as Python normalises it
std::pair<size_t, size_t> py_slice(size_t n, int64_t lo, int64_t hi) {
    int64_t len = static_cast<int64_t>(n);
    if (lo < 0) { lo += len; if (lo < 0) lo = 0; }
    if (hi < 0) { hi += len; if (hi < 0) hi = 0; }
    if (lo > len) lo = len;
    if (hi > len) hi = len;
    if (hi < lo) hi = lo;
    return {static_cast<size_t>(lo), static_cast<size_t>(hi)};
}

}  // namespace

void set_runner(const Runner& r) { runner() = r ? r : Runner(builtin_run); }
void set_memfs(bool on) { memfs_override() = on; }
void set_tag_counter(int64_t n) { seq() = n; }

// ------------------------------------------------------------------------------------------ the toolchain
Toolchain::Toolchain(const Profile& profile, const std::optional<str>& variant_in) : p(profile), name(profile.name) {
    // self.variant = variant or profile.get("default_variant") or ""
    if (truthy(variant_in)) variant = *variant_in;
    else if (truthy(profile.default_variant)) variant = *profile.default_variant;
    else variant = str("");
    py::list<Change> on;
    if (profile.variants.contains(variant)) {
        const Variant& v = profile.variants.at(variant);
        on = v.patches;
        variant_hooks = v.hooks;
    }
    if (!variant.empty() && !profile.variants.contains(variant)) {
        std::vector<str> keys = profile.variants.keys();
        std::sort(keys.begin(), keys.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        py::list<str> sorted_keys;
        for (const str& k : keys) sorted_keys.append(k);
        throw py::ValueError(py::percent(str("%s has no variant %r (it has %s)"), name, variant, py::repr(sorted_keys)).utf8());
    }
    for (const Patch& x : profile.patches)
        if (on.contains(x.change)) patches.append(x);
    // a listing's switch changes no object: not in the key
    std::string text = "[" + profile.keyed.utf8() + ", " + py::json_dump(py::V(variant.utf8())) + "]";
    key = py::hashlib::sha1(py::bytes(text)).hexdigest().slice(0, 12);
}

std::optional<str> Toolchain::p_get(const str& k) const {
    if (p.strings.contains(k)) return p.strings.at(k);
    return std::nullopt;
}

str Toolchain::stage() const {
    std::optional<str> tag = py::os::getenv(str("CB_STAGE_TAG"));
    bool has_tag = truthy(tag);
    str d = state_path({str("exact"), str("stage"), py::percent(str("%s-%s%s"), name, key, has_tag ? str("-") + *tag : str(""))});
    str done = py::os::path::join(d, str(".staged"));
    if (py::os::path::exists(done)) return d;
    if (!p.kit.has_value()) throw py::KeyError("'kit'");
    str kit = kit_dir(*p.kit);
    const Stage& st = p.stage;
    auto where = [&](const str& src) {                      // "otherkit:path" names a file of another kit
        int64_t at = src.find(str(":"));
        if (at >= 0 && py::len(src.slice(0, at)) > 1) return py::os::path::join(kit_dir(src.slice(0, at)), src.slice_from(at + 1));
        return py::os::path::join(kit, src);
    };
    py::dict<str, str> files = st.files.copy();
    if (st.all) {                                           // (every file of the kit, its folders flattened: a compiler that finds its headers beside itself)
        for (const auto& w : py::os::walk(kit)) {
            for (const str& f : std::get<2>(w)) {
                str up = f.upper();
                if (!files.contains(up)) files[up] = py::os::path::relpath(py::os::path::join(std::get<0>(w), f), kit);
            }
        }
    }
    for (const auto& kv : files.items()) {
        const str& dst = std::get<0>(kv);
        const str& src = std::get<1>(kv);
        std::string b = read_bytes(where(src)).raw();
        int64_t hdr = (b.size() >= 2 && b[0] == 'M' && b[1] == 'Z') ? py::unpack_from1<int64_t>("<H", py::bytes(b), 8) * 16 : 0;
        for (const Patch& x : patches) {
            if (x.file != dst || x.in_memory) continue;
            int64_t at = hdr + _int(x.at);
            py::bytes was = py::bytes::fromhex(x.was), now = py::bytes::fromhex(x.now);
            auto [lo, hi] = py_slice(b.size(), at, at + py::len(was));
            if (b.substr(lo, hi - lo) != was.raw()) {
                std::string change = std::holds_alternative<int64_t>(x.change) ? std::to_string(std::get<int64_t>(x.change)) : std::get<str>(x.change).utf8();
                throw py::RuntimeError(py::percent(str("%s: %s at %x is not what change %s expects - not this compiler's kit"), name, dst, _int(x.at), str(change)).utf8());
            }
            auto [lo2, hi2] = py_slice(b.size(), at, at + py::len(now));
            b.replace(lo2, hi2 - lo2, now.raw());
        }
        str target = py::os::path::join(d, dst);
        str dirn = py::os::path::dirname(target);
        py::os::makedirs(dirn.empty() ? d : dirn, 0777, true);
        write_bytes(target, py::bytes(b));
    }
    if (truthy(st.include)) {
        str top = where(*st.include);
        for (const auto& w : py::os::walk(top)) {
            str to = py::os::path::join(d, str("INCLUDE"), py::os::path::relpath(std::get<0>(w), top).upper());
            py::os::makedirs(to, 0777, true);
            for (const str& f : std::get<2>(w)) py::shutil::copy(py::os::path::join(std::get<0>(w), f), py::os::path::join(to, f.upper()));
        }
    }
    write_bytes(done, py::bytes(key.utf8()));
    return d;
}

str Toolchain::switches_of(const py::bytes& src, const std::optional<str>& switches) const {
    if (truthy(p.file_switches)) {
        std::optional<re::Match> m;
        str s = latin1(src);
        if (p.file_switches_comment.has_value() && *p.file_switches_comment == ";")      // (an assembler's: ; NAME: switches)
            m = re::match(str("; ") + *p.file_switches + str(": ([^\\r\\n]*)"), s);
        else
            m = re::match(str("/\\* ") + *p.file_switches + str(": ([^*]*?) \\*/"), s);
        if (m) return m->group(1).encode("latin1").decode("utf-8").strip();
    }
    if (switches.has_value()) return *switches;
    return p.switches.has_value() ? *p.switches : str("");
}

py::dict<str, py::list<int64_t>> Toolchain::answers_of(const py::bytes& src) const {
    py::dict<str, py::list<int64_t>> out;
    str head = latin1(src.slice(0, 400));
    for (const auto& kv : p.hooks.items()) {
        const Hook& h = std::get<1>(kv);
        if (!h.header.has_value()) continue;                // (a hook the variant sets, no source's answer)
        std::optional<re::Match> m = re::search(str("/\\* ") + *h.header + str(": ([0-9, ]+) \\*/"), head);
        if (m) {
            std::vector<int64_t> xs;
            for (const str& x : py::split(m->group(1).replace(str(","), str(" ")))) xs.push_back(py::int_parse(x));
            std::sort(xs.begin(), xs.end());
            out[std::get<0>(kv)] = py::list<int64_t>(xs);
        }
    }
    return out;
}

py::dict<str, str> Toolchain::_values(const str& sw, const str& tag) const {
    py::dict<str, str> v;
    v[str("tag")] = tag;
    v[str("tag6")] = tag.slice(0, 6);
    v[str("switches")] = sw;
    for (const auto& kv : p.derive.items()) {
        const Derive& rule = std::get<1>(kv);
        std::optional<re::Match> m = re::search(rule.regex, sw);
        if (m) {
            str t = rule.then_;
            for (int64_t i = 1; i <= m->raw().ngroups(); i++) {
                std::optional<str> g = m->group_opt(i);
                t = t.replace(py::percent(str("{%d}"), i), g.has_value() ? *g : str(""));
            }
            v[std::get<0>(kv)] = t;
        } else {
            v[std::get<0>(kv)] = rule.else_;
        }
    }
    return v;
}

py::bytes Toolchain::_env(const str& sw) const {
    std::string out;
    for (const str& e : p.env) out += e.encode("latin1").raw() + std::string(1, '\0');
    for (const auto& kv : p.env_unless.items())
        if (!re::search(std::get<1>(kv), sw)) out += std::get<0>(kv).encode("latin1").raw() + std::string(1, '\0');
    return py::bytes(out);
}

std::tuple<std::optional<int64_t>, py::bytes> Toolchain::_run(const str& w, const str& sw, const str& tag,
                                                              const py::dict<str, py::list<int64_t>>& ans) const {
    if (p.emulator != "dos") throw py::NotImplementedError(py::percent(str("emulator %r"), p.emulator).utf8());
    py::dict<str, str> v = _values(sw, tag);
    py::bytes env = _env(sw);
    std::string con;
    if (!p.has_passes) throw py::KeyError("'passes'");
    for (int64_t idx = 0; idx < py::len(p.passes); idx++) {
        const Pass& ps = p.passes[idx];
        std::optional<Instrument> inst;
        std::optional<When> when;
        for (const auto& kv : p.hooks.items()) {
            const Hook& h = std::get<1>(kv);
            if (h.pass_ == ps.run && ans.contains(std::get<0>(kv)) && !ans.at(std::get<0>(kv)).empty()) {
                py::set<int64_t> counts;
                for (int64_t c : ans.at(std::get<0>(kv))) counts.add(c);
                inst = _hook(h, counts);
            }
        }
        py::list<Hook> calls;
        for (const str& x : variant_hooks)
            if (p.hooks.contains(x)) {
                const Hook& h = p.hooks.at(x);
                if (h.pass_ == ps.run && truthy(h.call)) calls.append(h);
            }
        if (!calls.empty()) inst = _call_hooks(calls, inst);          // (a hook whose answer is code: where the pass goes on)
        py::list<Patch> mem;
        for (const Patch& x : patches)
            if (x.file == ps.run && x.in_memory) mem.append(x);
        if (!mem.empty()) {                                           // a packed pass: changed once it has unpacked itself
            auto im = _in_memory(mem, inst);
            inst = std::get<0>(im);
            when = std::get<1>(im);
        }
        std::string penv = env.raw();
        for (const str& e : ps.env) penv += format_named(e, v).encode("latin1").raw() + std::string(1, '\0');
        str args = (args_override_.has_value() && std::get<0>(*args_override_) == idx) ? std::get<1>(*args_override_) : ps.args;
        RunArgs ra;
        ra.exe = py::os::path::join(w, ps.run);
        ra.workdir = w;
        ra.cmdline = format_named(args, v);
        ra.env = py::bytes(penv);
        ra.instrument = inst;
        ra.instrument_when = when;
        // (p.get("psp", 0x100): the key left out is 0x100, a null is None)
        if (p.psp.has_value()) ra.psp = _int(*p.psp);
        else ra.psp = p.psp_set ? std::nullopt : std::optional<int64_t>(0x100);
        int64_t top = p.top.has_value() ? _int(*p.top) : 0;
        if (top) ra.top = top;
        ra.fast = !inst.has_value() && !when.has_value();            // (no hook of ours in the pass: the fast run)
        ra.fs = fs_;
        dos::Result r = runner()(ra);
        con += r.console.raw();
        if (r.exit.has_value() && *r.exit != 0)
            return {r.exit, py::bytes(con + "(the pass that stopped: " + ps.run.encode("utf-8").raw() + ")\n")};
    }
    return {0, py::bytes(con)};
}

namespace {

// repr(sorted(headers.items())) -- headers is {str: bytes}
str repr_headers(const py::dict<str, py::bytes>& headers) {
    std::vector<std::tuple<str, py::bytes>> items = headers.items();
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        return std::get<1>(a) < std::get<1>(b);
    });
    return py::repr(py::list<std::tuple<str, py::bytes>>(items));
}

py::list<std::tuple<str, py::bytes>> include_files(const py::list<str>& include) {
    py::list<std::tuple<str, py::bytes>> incs;
    for (const str& d : include) {
        std::vector<str> names;
        for (const str& f : py::os::listdir(d)) names.push_back(f);
        std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        for (const str& f : names)
            if (py::os::path::isfile(py::os::path::join(d, f))) incs.append(std::make_tuple(f, read_bytes(py::os::path::join(d, f))));
    }
    return incs;
}

}  // namespace

namespace {

// sha1 of what a compile's answer depends on, joined with NUL as the Python joins it
str key_of(const py::bytes& src, const py::dict<str, py::bytes>& headers, const py::list<std::tuple<str, py::bytes>>& incs, const str& sw,
           const py::dict<str, py::list<int64_t>>& ans, const str& tool_key) {
    std::vector<std::tuple<str, py::list<int64_t>>> ai = ans.items();
    std::sort(ai.begin(), ai.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    std::string nul(1, '\0');
    std::string joined = src.raw() + nul + repr_headers(headers).utf8() + nul + py::repr(incs).utf8() + nul + sw.utf8() + nul +
                         py::repr(py::list<std::tuple<str, py::list<int64_t>>>(ai)).utf8() + nul + tool_key.utf8() + nul + CACHE_VERSION.utf8();
    return py::hashlib::sha1(py::bytes(joined)).hexdigest();
}

}  // namespace

str Toolchain::compile_key(const py::bytes& src0, const py::dict<str, py::bytes>& headers0, const py::list<str>& include,
                           const std::optional<str>& switches) const {
    py::bytes src = src0.replace(py::bytes("\r\n"), py::bytes("\n"));
    auto incs = include_files(include);
    str sw = switches_of(src, switches);
    return key_of(src, headers0, incs, sw, answers_of(src), key);
}

std::tuple<std::optional<py::bytes>, str> Toolchain::compile(const py::bytes& src, const py::dict<str, py::bytes>& headers,
                                                             const py::list<str>& include, const std::optional<str>& switches) const {
    return compile(src, headers, include, switches, true);
}

std::tuple<std::optional<py::bytes>, str> Toolchain::compile(const py::bytes& src, const py::dict<str, py::bytes>& headers,
                                                             const py::list<str>& include, const std::optional<str>& switches, bool use_cache) const {
    return compile_(src, headers, include, switches, use_cache);
}

std::tuple<std::optional<py::bytes>, str, str> Toolchain::compile_listing(const py::bytes& src, const py::dict<str, py::bytes>& headers,
                                                                          const py::list<str>& include, const std::optional<str>& switches,
                                                                          bool use_cache) const {
    int64_t at = -1;
    for (int64_t i = 0; i < py::len(p.passes); i++)
        if (p.passes[i].listing.has_value()) {
            at = i;
            break;
        }
    if (at < 0) {
        auto r = compile_(src, headers, include, switches, use_cache);
        return {std::get<0>(r), std::get<1>(r), str("")};
    }
    args_override_ = std::make_tuple(at, p.passes[at].args + str(" ") + *p.passes[at].listing);
    listing_ = std::nullopt;
    std::tuple<std::optional<py::bytes>, str> r;
    try {
        r = compile_(src + py::bytes("\n/* listing */"), headers, include, switches, use_cache);
    } catch (...) {
        args_override_ = std::nullopt;
        throw;
    }
    args_override_ = std::nullopt;
    return {std::get<0>(r), std::get<1>(r), truthy(listing_) ? *listing_ : str("")};
}

std::tuple<std::optional<py::bytes>, str> Toolchain::compile_(const py::bytes& src0, const py::dict<str, py::bytes>& headers0,
                                                              const py::list<str>& include, const std::optional<str>& switches,
                                                              bool use_cache) const {
    py::bytes src = src0.replace(py::bytes("\r\n"), py::bytes("\n"));
    py::dict<str, py::bytes> headers = headers0.copy();
    auto incs = include_files(include);
    str sw = switches_of(src, switches);
    auto ans = answers_of(src);
    str key_ = key_of(src, headers, incs, sw, ans, key);
    if (use_cache) {
        using Two = std::tuple<std::optional<py::bytes>, str>;
        using Three = std::tuple<std::optional<py::bytes>, str, str>;
        if (auto got3 = cache::value<Three>(str("exact"), key_)) {            // (a listing kept with it)
            listing_ = std::get<2>(*got3);
            return {std::get<0>(*got3), std::get<1>(*got3)};
        }
        if (auto got2 = cache::value<Two>(str("exact"), key_)) return *got2;
    }
    str w = stage();
    seq() += 1;                                       // a name of its own each time: the emulator may hold the last one's files open a while
    str tag = py::percent(str("T%05d"), ((static_cast<int64_t>(_getpid()) * 7 + seq()) % 100000));
    str ix = py::os::path::join(w, str("INCX"));
    str ext = p.source_ext;
    py::bytes body = crlf(src.replace(py::bytes("\x1a"), py::bytes("")));
    if (p.source_eof) body = body + py::bytes("\x1a");     // (a ^Z after the text: MASM 1.00 wants it)
    if (!p.output.has_value()) throw py::KeyError("'output'");
    str out = py::os::path::join(w, format_named(*p.output, _values(sw, tag)));
    str lst = py::os::path::join(w, tag + str(".LST"));    // (a listing the pass was asked for)
    std::optional<py::bytes> obj;
    std::optional<str> lst_text;
    std::optional<int64_t> code;
    py::bytes con;
    if (memfs() && p.emulator == "dos") {
        dos::MemFS fs(py::list<str>{ix});                // (the run's files in memory: nothing written, no other run's files in the way)
        for (const auto& kv : headers.items()) fs.put(py::os::path::join(w, std::get<0>(kv).upper()), crlf(std::get<1>(kv)));
        for (const auto& kv : incs) fs.put(py::os::path::join(ix, std::get<0>(kv).upper()), crlf(std::get<1>(kv)));
        fs.put(py::os::path::join(w, tag + ext), body);
        fs_ = &fs;
        std::tuple<std::optional<int64_t>, py::bytes> r;
        try {
            r = _run(w, sw, tag, ans);
        } catch (...) {
            fs_ = nullptr;
            throw;
        }
        fs_ = nullptr;
        code = std::get<0>(r);
        con = std::get<1>(r);
        if (code.has_value() && *code == 0 && fs.isfile(out)) obj = fs.read(out);
        if (fs.isfile(lst)) lst_text = fs.read(lst).decode("latin1").replace(str("\r"), str(""));
    } else {
        for (const auto& kv : headers.items()) write_bytes(py::os::path::join(w, std::get<0>(kv).upper()), crlf(std::get<1>(kv)));
        py::os::makedirs(ix, 0777, true);
        for (const str& f : py::os::listdir(ix)) remove_quietly(py::os::path::join(ix, f));
        for (const auto& kv : incs) write_bytes(py::os::path::join(ix, std::get<0>(kv).upper()), crlf(std::get<1>(kv)));
        write_bytes(py::os::path::join(w, tag + ext), body);
        if (py::os::path::exists(out)) py::os::remove(out);
        auto r = _run(w, sw, tag, ans);
        code = std::get<0>(r);
        con = std::get<1>(r);
        if (code.has_value() && *code == 0 && py::os::path::exists(out)) obj = read_bytes(out);
        if (py::os::path::exists(lst)) {
            py::file f = py::open(lst, str("r"), str("latin1"));
            lst_text = f.read_text().replace(str("\r"), str(""));
            f.close();
        }
        std::vector<str> gone{tag + ext, py::os::path::basename(out), tag + str(".LST")};
        for (const auto& kv : headers.items()) gone.push_back(std::get<0>(kv).upper());
        for (const str& f : gone) remove_quietly(py::os::path::join(w, f));
        for (const str& f : py::os::listdir(w))               // (the passes' own files a failed run leaves: T12345EX ..)
            if (f.startswith(tag) && py::len(f) == py::len(tag) + 2) remove_quietly(py::os::path::join(w, f));
    }
    if (obj.has_value() && p.has_rename_externs) obj = omf::rename_externs(*obj, p.rename_externs);
    std::tuple<std::optional<py::bytes>, str> res{obj, con.decode("latin1").replace(str("\r"), str(""))};
    listing_ = lst_text;
    if (lst_text.has_value()) cache::keep(str("exact"), key_, std::make_tuple(std::get<0>(res), std::get<1>(res), *lst_text));
    else cache::keep(str("exact"), key_, res);
    return res;
}

// ------------------------------------------------------------------------------------------ loading
namespace {
std::map<std::u32string, Plugin>& plugins() {
    static std::map<std::u32string, Plugin> r;
    return r;
}
}  // namespace

void register_plugin(const str& module_class, const Plugin& make) { plugins()[module_class.u32()] = make; }

std::shared_ptr<Toolchain> load(const str& name, const std::optional<str>& variant) {
    static std::map<std::pair<std::u32string, std::optional<std::u32string>>, std::shared_ptr<Toolchain>> loaded;
    auto k = std::make_pair(name.u32(), variant.has_value() ? std::optional<std::u32string>(variant->u32()) : std::nullopt);
    auto it = loaded.find(k);
    if (it != loaded.end()) return it->second;
    Profile p = profile_from_json(slurp_text(py::os::path::join(HERE(), str("toolchains"), name + str(".json"))));
    std::shared_ptr<Toolchain> tc;
    if (p.plugin.has_value()) {                          // (a toolchain whose running is code: module:Class)
        auto pl = plugins().find(p.plugin->u32());
        if (pl == plugins().end()) {
            int64_t colon = p.plugin->find(str(":"));
            throw py::ModuleNotFoundError("No module named '" + (colon >= 0 ? p.plugin->slice(0, colon) : *p.plugin).utf8() + "'");
        }
        tc = pl->second(p, variant);
    } else {
        tc = std::make_shared<Toolchain>(p, variant);
    }
    loaded[k] = tc;
    return tc;
}

}  // namespace codebreaker::exact::toolchain
