// codebreaker/base/corpus.cpp -- typed C++ of codebreaker.base.corpus (see corpus.hpp).

#include "codebreaker/base/corpus.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::corpus {

namespace detect = ::codebreaker::formats::detect;
namespace ne = ::codebreaker::formats::ne;
namespace xex = ::codebreaker::formats::xex;
namespace settings = ::py::settings;

const py::list<py::str> EXTS_ = py::list<py::str>{py::str(".dll"), py::str(".exe"), py::str(".drv"), py::str(".ocx"),
                                                  py::str(".cpl"), py::str(".vxd"), py::str(".386"), py::str(".sys"),
                                                  py::str(".com"), py::str(".acm"), py::str(".ax")};
const py::str ENV = py::str("CB_CORPUS_NAME");       // selects a product for a whole run; use() sets it for workers

std::function<py::str(const py::str&)> xbe_to_pe;

namespace {

// ---- module state
py::dict<py::str, py::dict<py::str, py::list<py::str>>> index_cache_;        // _index_cache: root -> {basename_lower: [paths, best first]}
py::dict<py::str, std::optional<py::str>> lane_cache_;                        // _lane_cache: path -> lane
std::optional<py::str> selected_;                                             // _selected
py::dict<std::tuple<py::str, int64_t, int64_t>, py::str> sha_memo_;           // _SHA_MEMO

// ---- the settings module's other parts (the typed settings give str settings only)
// settings.ROOT / settings.repo_root(): the checkout root
py::str repo_root_() { return settings::checkout_root(); }

// settings._load().get("corpora") or {}: {name: folder as written in settings.json}
py::dict<py::str, py::str> listed_() {
    static std::optional<py::dict<py::str, py::str>> cached;
    if (cached) return *cached;
    py::dict<py::str, py::str> out;
    std::ifstream f(std::filesystem::u8path(py::os::path::join(repo_root_(), py::str("settings.json")).utf8()), std::ios::binary);
    if (f) {                                                   // (a missing settings.json: no corpora)
        std::stringstream ss;
        ss << f.rdbuf();
        const std::string s = ss.str();
        size_t i = 0;
        settings::json_ws_(s, i);
        if (i >= s.size() || s[i] != '{') throw py::ValueError("settings.json: not a JSON object");
        i++;
        for (;;) {
            settings::json_ws_(s, i);
            if (i < s.size() && s[i] == '}') break;
            if (i >= s.size() || s[i] != '"') throw py::ValueError("settings.json: a key expected");
            std::string k = settings::json_str_(s, i);
            settings::json_ws_(s, i);
            if (i >= s.size() || s[i] != ':') throw py::ValueError("settings.json: ':' expected");
            i++;
            settings::json_ws_(s, i);
            if (k == "corpora" && i < s.size() && s[i] == '{') {
                i++;
                for (;;) {
                    settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == '}') {
                        i++;
                        break;
                    }
                    if (i >= s.size() || s[i] != '"') throw py::ValueError("settings.json: a key expected");
                    std::string name = settings::json_str_(s, i);
                    settings::json_ws_(s, i);
                    if (i >= s.size() || s[i] != ':') throw py::ValueError("settings.json: ':' expected");
                    i++;
                    settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == '"') out.set(py::str(name), py::str(settings::json_str_(s, i)));
                    else settings::json_skip_(s, i);          // (a folder that is not a str: nothing to look in)
                    settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == ',') i++;
                }
            } else if (i < s.size() && s[i] == '"') {
                settings::json_str_(s, i);
            } else {
                settings::json_skip_(s, i);
            }
            settings::json_ws_(s, i);
            if (i < s.size() && s[i] == ',') i++;
        }
    }
    cached = out;
    return out;
}

// settings.corpora(): {name: folder} of every product's original binaries: the "corpora" setting, and the
// single "corpus" under the name in "target" when that name is not listed there.
py::dict<py::str, py::str> corpora_() {
    py::dict<py::str, py::str> out;
    for (const auto& kv : listed_().items()) {
        const py::str& value = std::get<1>(kv);
        out.set(std::get<0>(kv), py::os::path::isabs(value) ? value : py::os::path::normpath(py::os::path::join(repo_root_(), value)));
    }
    py::str target = settings::get_str(py::str("target"));
    py::str corpus = settings::get_str(py::str("corpus"));
    out.setdefault(target, corpus);
    return out;
}

bool truthy_(const std::optional<py::str>& s) { return s && !s->empty(); }

// the sorted keys of a dict of str
std::vector<py::str> sorted_keys_(const std::vector<py::str>& keys) {
    std::vector<py::str> out = keys;
    std::sort(out.begin(), out.end(), [](const py::str& a, const py::str& b) { return a < b; });
    return out;
}

}  // namespace

// Every product that has a folder of original binaries, by name.
py::list<py::str> names() {
    return py::list<py::str>(sorted_keys_(corpora_().keys()));
}

// The selected product's name.
py::str current() {
    if (truthy_(selected_)) return *selected_;
    std::optional<py::str> env = py::os::environ_::get(ENV);
    if (truthy_(env)) return *env;
    return settings::get_str(py::str("target"));
}

py::str use(const std::optional<py::str>& name) {
    py::str before = current();
    if (name && !corpora_().contains(*name))
        throw py::ValueError(py::percent(py::str("no corpus %r (settings.json lists: %s)"), *name, py::join(py::str(", "), names())));
    selected_ = name;
    if (!name) py::os::environ_::pop(ENV);
    else py::os::environ_::set(ENV, *name);
    return before;
}

selected::selected(const std::optional<py::str>& name) {
    was_var_ = selected_;
    was_env_ = py::os::environ_::get(ENV);
    use(name);
}

selected::~selected() {
    // (the Python's finally: an error of its own cannot leave a destructor, and use() of the name that was
    // selected before cannot fail)
    try {
        use(was_var_);
    } catch (...) {
    }
    if (was_env_) py::os::environ_::set(ENV, *was_env_);
}

py::list<py::str> take_option(const py::list<py::str>& argv_) {
    py::list<py::str> argv = argv_.copy();
    if (argv.contains(py::str("--corpus"))) {
        int64_t i = argv.index(py::str("--corpus"));
        if (i + 1 >= py::len(argv)) throw py::SystemExit(py::percent(py::str("--corpus needs a name: %s"), py::join(py::str(", "), names())));
        use(argv[i + 1]);
        argv.set_slice(i, i + 2, py::list<py::str>());
    }
    return argv;
}

py::str root(const std::optional<py::str>& name_) {
    std::optional<py::str> name = name_;
    if (!truthy_(name)) name = selected_;
    if (!truthy_(name)) name = py::os::environ_::get(ENV);
    py::dict<py::str, py::str> listed = listed_();
    if (truthy_(name) && (listed.contains(*name) || *name != settings::get_str(py::str("target")))) {
        py::dict<py::str, py::str> all = corpora_();
        if (!all.contains(*name))
            throw py::ValueError(py::percent(py::str("no corpus %r (settings.json lists: %s)"), *name, py::join(py::str(", "), names())));
        return all.at(*name);
    }
    std::optional<py::str> env = py::os::environ_::get(py::str("CB_CORPUS_ROOT"));
    if (truthy_(env) && py::os::path::isdir(*env)) return *env;
    py::str sib = py::os::path::join(py::os::path::dirname(repo_root_()), py::str("Windows95"), py::str("design"), py::str("corpus"),
                                     py::str("files"));
    if (py::os::path::isdir(sib)) return sib;
    env = py::os::environ_::get(py::str("WT_DATA_ROOT"));
    if (truthy_(env) && py::os::path::isdir(*env)) return *env;
    return sib;                                   // a sensible path to report when nothing exists
}

py::str design_dir() {
    std::optional<py::str> name = selected_;
    if (!truthy_(name)) name = py::os::environ_::get(ENV);
    if (truthy_(name) && *name != settings::get_str(py::str("target")))
        return py::os::path::join(settings::get_str(py::str("state")), py::str("products"), *name, py::str("design"));
    return py::os::path::dirname(py::os::path::dirname(root()));
}

py::str reference(const py::list<py::str>& parts) {
    py::str base = settings::get_str(py::str("reference"));
    if (base.empty()) base = py::os::path::join(design_dir(), py::str("reference"));
    py::str out = base;
    for (const py::str& p : parts) out = py::os::path::join(out, p);
    return out;
}

std::optional<py::str> owner(const py::str& path_) {
    py::str path = py::os::path::normcase(py::os::path::abspath(path_));
    std::optional<std::tuple<py::str, py::str>> best;
    for (const py::str& name : names()) {
        py::str r = py::os::path::normcase(py::os::path::abspath(root(name)));
        if (path.startswith(r + py::os::sep) && (!best || py::len(r) > py::len(std::get<1>(*best)))) best = std::make_tuple(name, r);
    }
    if (best) return std::get<0>(*best);
    return std::nullopt;
}

std::optional<py::str> lane(const py::str& path) {
    if (lane_cache_.contains(path)) return lane_cache_.at(path);
    std::optional<py::str> out = detect::lane(path);
    lane_cache_.set(path, out);
    return out;
}

std::tuple<int64_t, int64_t, py::str> _rank(const py::str& path) {
    py::list<py::str> parts;
    for (const py::str& p : py::split(path.replace(py::str("\\"), py::str("/")), py::str("/"))) parts.append(p.lower());
    int64_t tier = parts.contains(py::str("system")) ? 0 : (parts.contains(py::str("windows")) ? 1 : 2);
    return std::make_tuple(tier, py::len(parts), path.lower());
}

py::dict<py::str, py::list<py::str>> index(bool refresh) {
    py::str r = root();
    if (refresh || !index_cache_.contains(r)) {
        py::dict<py::str, py::list<py::str>> idx;
        for (const auto& step : py::os::walk(r)) {
            const py::str& dp = std::get<0>(step);
            for (const py::str& fn : std::get<2>(step)) idx.setdefault(fn.lower(), py::list<py::str>()).append(py::os::path::join(dp, fn));
        }
        for (const py::str& name : idx.keys()) idx.at(name).sort([](const py::str& p) { return _rank(p); });
        _unpack_boot_images(idx);
        index_cache_.set(r, idx);
    }
    return index_cache_.at(r);
}

void _unpack_boot_images(const py::dict<py::str, py::list<py::str>>& idx) {
    std::vector<py::str> todo;
    for (const py::str& n : idx.keys())
        if (n.endswith(py::str(".bin")) && idx.contains(n.slice(0, py::len(n) - 4) + ".ovl")) todo.push_back(n);
    for (const py::str& name : todo) {
        py::str stem = name.slice(0, py::len(name) - 4);
        py::str image = idx.at(name)[0];
        py::str overlay = idx.at(stem + ".ovl")[0];
        std::optional<py::str> l = lane(image);
        if (!(l && *l == py::str("ne"))) continue;
        py::str out = py::os::path::join(settings::get_str(py::str("state")), py::str("unpacked"),
                                         py::percent(py::str("%s-%d"), stem, py::os::path::getsize(image)));
        py::dict<py::str, ne::BootImageModule> modules;
        try {
            modules = ne::open_boot_image(image, overlay);
        } catch (const py::OSError&) {
            continue;
        } catch (const py::ValueError&) {
            continue;
        }
        py::os::makedirs(out, 0777, true);
        for (const auto& kv : modules.items()) {
            const py::str& module = std::get<0>(kv);
            const ne::BootImageModule& nef = std::get<1>(kv);
            py::str path = py::os::path::join(out, module.lower() + ".exe");
            if (!py::os::path::isfile(path)) {
                py::file f = py::open(path, py::str("wb"));
                f.write(ne::standalone(nef));
                f.close();
            }
            py::list<py::str> have = idx.setdefault(module.lower() + ".exe", py::list<py::str>());   // (MSDOS.EXE on the disk is a 1-byte stub)
            bool any = false;
            for (const py::str& p : have) {
                std::optional<py::str> lp = lane(p);
                if (lp && !lp->empty()) any = true;
            }
            if (!any) have.insert(0, path);
        }
    }
}

py::list<py::str> candidates(const py::str& name_) {
    py::str name = py::os::path::basename(name_).lower();
    py::dict<py::str, py::list<py::str>> idx = index();
    if (idx.contains(name)) return idx.at(name).copy();
    if (!name.contains(py::str("."))) {
        py::list<py::str> out;
        for (const py::str& ext : EXTS_) {
            if (idx.contains(name + ext)) out.extend(idx.at(name + ext));
        }
        return py::sorted(out, [](const py::str& p) { return _rank(p); });
    }
    return py::list<py::str>();
}

py::str resolve(const py::str& name) {
    std::optional<py::str> found = retail(name);
    if (!truthy_(found)) throw py::ValueError(py::percent(py::str("no module %r in the corpus"), name));
    return *found;
}

std::optional<py::str> retail(const py::str& name) {
    if (py::os::path::isfile(name)) return _readable(name);
    py::list<py::str> c = candidates(name);
    if (!c.empty()) return _readable(c[0]);
    return std::nullopt;
}

py::str _readable(const py::str& path_) {
    py::str path = path_;
    // platforms.named("powerpc").readable: a .xex is handed over as the PowerPC image inside it
    if (xex::is_xex(path)) path = xex::basefile(path);
    // platforms.named("xbe").readable: an .xbe as the PE image made of it
    bool xbe = py::open_rb(path).read(4) == py::bytes("XBEH");
    if (xbe) {
        if (!xbe_to_pe) throw py::RuntimeError("corpus::xbe_to_pe is not set: formats.xbox.xbe has no C++ port yet");
        path = xbe_to_pe(path);
    }
    return path;
}

py::list<py::str> modules(const std::optional<py::str>& want_lane, const py::list<py::str>& exts) {
    py::list<py::str> out;
    for (const auto& kv : index().items()) {
        const py::str& name = std::get<0>(kv);
        bool ends = false;
        for (const py::str& e : exts)
            if (name.endswith(e)) ends = true;
        if (!ends) continue;
        py::str p = std::get<1>(kv)[0];
        std::optional<py::str> l = lane(p);
        if (!want_lane || (l && *l == *want_lane)) out.append(p);
    }
    return py::sorted(out, [](const py::str& p) { return std::make_tuple(py::os::path::getsize(p), p.lower()); });
}

py::list<std::tuple<py::str, py::str>> executables() {
    py::list<std::tuple<py::str, py::str>> out;
    for (const py::list<py::str>& cands : index().values()) {
        for (const py::str& p : cands) {
            std::optional<py::str> kind = lane(p);
            if (kind && !kind->empty()) out.append(std::make_tuple(p, *kind));
        }
    }
    return py::sorted(out);
}

std::optional<py::str> sha(const py::str& path) {
    int64_t size, mtime;
    try {
        size = py::os::path::getsize(path);                       // os.stat(path)
        mtime = static_cast<int64_t>(py::os::path::getmtime(path));
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    std::tuple<py::str, int64_t, int64_t> key(path, mtime, size);
    if (sha_memo_.contains(key)) return sha_memo_.at(key);
    py::hashlib::Hash h = py::hashlib::sha256();
    try {
        py::file f = py::open_rb(path);
        for (;;) {
            py::bytes chunk = f.read(int64_t(1) << 20);
            if (chunk.empty()) break;
            h.update(chunk);
        }
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    py::str s = h.hexdigest();
    sha_memo_.set(key, s);
    return s;
}

}  // namespace codebreaker::base::corpus
