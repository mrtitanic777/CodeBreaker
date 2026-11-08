// codebreaker/base/converted.cpp -- typed C++ of codebreaker.base.converted (see converted.hpp).

#include "codebreaker/base/converted.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>

#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_json.hpp"
#include "py_os.hpp"

namespace codebreaker::base::converted {

namespace {

bool is_cb_char(char32_t c) {   // [A-Za-z0-9._-]
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
}

const char* type_of(const Json& j) {
    switch (j.kind) {
        case Json::None: return "NoneType";
        case Json::Bool: return "bool";
        case Json::Int: return "int";
        case Json::Float: return "float";
        case Json::Str: return "str";
        case Json::List: return "list";
        default: return "dict";
    }
}
// d.get(...) of a JSON document that is not an object
[[noreturn]] void no_attr(const Json& j, const char* attr) {
    throw py::AttributeError(std::string("'") + type_of(j) + "' object has no attribute '" + attr + "'");
}

// "%s" of a scalar value of project.json
py::str scalar_text(const Json& j) {
    switch (j.kind) {
        case Json::None: return py::str("None");
        case Json::Bool: return py::str(j.b ? "True" : "False");
        case Json::Int: return py::str(std::to_string(j.i));
        case Json::Float: return py::to_str(j.d);
        case Json::Str: return j.s;
        default: throw py::NotImplementedError("README.txt of a value that is a list or an object");
    }
}

std::optional<py::str> str_of(const Json& d, const char* key) {
    auto it = d.dict.get(py::str(key));
    if (!it || it->kind != Json::Str) return std::nullopt;
    return it->s;
}

// ---- json.dump(d, indent=1)
void put_str(std::string& o, const py::str& s) { py::json::put(o, s); }

void put_json(std::string& o, const Json& j, int depth) {
    const std::string in(size_t(depth + 1), ' ');
    switch (j.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += j.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(j.i); break;
        case Json::Float: o += py::to_str(j.d).utf8(); break;
        case Json::Str: put_str(o, j.s); break;
        case Json::List:
            if (j.list.empty()) {
                o += "[]";
                break;
            }
            o += "[\n";
            for (size_t i = 0; i < j.list.size(); ++i) {
                o += in;
                put_json(o, j.list[i], depth + 1);
                o += i + 1 < j.list.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "]";
            break;
        default: {
            auto items = j.dict.items();
            if (items.empty()) {
                o += "{}";
                break;
            }
            o += "{\n";
            for (size_t i = 0; i < items.size(); ++i) {
                o += in;
                put_str(o, std::get<0>(items[i]));
                o += ": ";
                put_json(o, std::get<1>(items[i]), depth + 1);
                o += i + 1 < items.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "}";
        }
    }
}

// the keys of a dict in dump order: the file's, then the known keys that are new
template <class Data, size_t N>
std::vector<py::str> key_order(const Data& d, const char* const (&canonical)[N], const std::function<bool(const char*)>& present) {
    std::vector<py::str> out;
    for (const py::str& k : d.order) out.push_back(k);
    for (const char* c : canonical)
        if (present(c) && std::find(out.begin(), out.end(), py::str(c)) == out.end()) out.push_back(py::str(c));
    return out;
}

void put_list(std::string& o, const py::list<py::str>& v, int depth) {
    if (v.empty()) {
        o += "[]";
        return;
    }
    o += "[\n";
    for (int64_t i = 0; i < v.size(); ++i) {
        o += std::string(size_t(depth + 1), ' ');
        put_str(o, v[i]);
        o += i + 1 < v.size() ? ",\n" : "\n";
    }
    o += std::string(size_t(depth), ' ') + "]";
}

void put_step(std::string& o, const Step& s, int depth) {
    if (s.not_a_dict) {
        put_json(o, *s.not_a_dict, depth);
        return;
    }
    static const char* const canonical[] = {"command", "when", "outputs", "notes"};
    auto present = [&](const char* k) {
        std::string n = k;
        return (n == "command" && s.command) || (n == "when" && s.when) || (n == "outputs" && s.outputs) || (n == "notes" && s.notes);
    };
    std::vector<std::pair<std::string, std::string>> kv;
    for (const py::str& key : key_order(s, canonical, present)) {
        std::string k = key.utf8(), t;
        if (k == "command" && s.command) put_str(t, *s.command);
        else if (k == "when" && s.when) put_str(t, *s.when);
        else if (k == "notes" && s.notes) put_str(t, *s.notes);
        else if (k == "outputs" && s.outputs) put_list(t, *s.outputs, depth + 1);
        else if (auto it = s.other.get(key)) put_json(t, *it, depth + 1);
        else continue;
        kv.push_back({k, t});
    }
    if (kv.empty()) {
        o += "{}";
        return;
    }
    o += "{\n";
    for (size_t i = 0; i < kv.size(); ++i) {
        o += std::string(size_t(depth + 1), ' ');
        put_str(o, py::str(kv[i].first));
        o += ": " + kv[i].second;
        o += i + 1 < kv.size() ? ",\n" : "\n";
    }
    o += std::string(size_t(depth), ' ') + "}";
}

std::string dump(const ProjectData& d) {
    static const char* const canonical[] = {"name", "file", "source", "sha1", "size", "kind", "steps"};
    auto present = [&](const char* k) {
        std::string n = k;
        return (n == "name" && d.name) || (n == "file" && d.file) || (n == "source" && d.source) || (n == "sha1" && d.sha1) ||
               (n == "size" && d.size) || (n == "kind" && d.kind) || (n == "steps" && d.steps);
    };
    std::vector<std::pair<std::string, std::string>> kv;
    for (const py::str& key : key_order(d, canonical, present)) {
        std::string k = key.utf8(), t;
        if (k == "name" && d.name) put_str(t, *d.name);
        else if (k == "file" && d.file) put_str(t, *d.file);
        else if (k == "source" && d.source) put_str(t, *d.source);
        else if (k == "sha1" && d.sha1) put_str(t, *d.sha1);
        else if (k == "kind" && d.kind) put_str(t, *d.kind);
        else if (k == "size" && d.size) t = std::to_string(*d.size);
        else if (k == "steps" && d.steps) {
            if (d.steps->empty()) t = "[]";
            else {
                t = "[\n";
                for (int64_t i = 0; i < d.steps->size(); ++i) {
                    t += "  ";
                    put_step(t, (*d.steps)[i], 2);
                    t += i + 1 < d.steps->size() ? ",\n" : "\n";
                }
                t += " ]";
            }
        } else if (auto it = d.other.get(key)) put_json(t, *it, 1);
        else continue;
        kv.push_back({k, t});
    }
    if (kv.empty()) return "{}";
    std::string o = "{\n";
    for (size_t i = 0; i < kv.size(); ++i) {
        o += " ";
        put_str(o, py::str(kv[i].first));
        o += ": " + kv[i].second;
        o += i + 1 < kv.size() ? ",\n" : "\n";
    }
    return o + "}";
}

Step step_of(const Json& e) {
    Step s;
    if (e.kind != Json::Dict) {
        s.not_a_dict = e;
        return s;
    }
    for (const auto& kv : e.dict.items()) {
        const py::str& key = std::get<0>(kv);
        const Json& v = std::get<1>(kv);
        const std::string k = key.utf8();
        s.order.append(key);
        if (k == "command" && v.kind == Json::Str) s.command = v.s;
        else if (k == "when" && v.kind == Json::Str) s.when = v.s;
        else if (k == "notes" && v.kind == Json::Str) s.notes = v.s;
        else if (k == "outputs" && v.kind == Json::List && std::all_of(v.list.begin(), v.list.end(), [](const Json& x) { return x.kind == Json::Str; })) {
            py::list<py::str> outs;
            for (const Json& x : v.list) outs.append(x.s);
            s.outputs = outs;
        } else s.other.set(key, v);
    }
    return s;
}

// project.json's document as a ProjectData (what project.json.get(...) answers)
ProjectData data_of(const Json& j) {
    if (j.kind != Json::Dict) no_attr(j, "get");
    ProjectData d;
    for (const auto& kv : j.dict.items()) {
        const py::str& key = std::get<0>(kv);
        const Json& v = std::get<1>(kv);
        const std::string k = key.utf8();
        d.order.append(key);
        if (k == "name" && v.kind == Json::Str) d.name = v.s;
        else if (k == "file" && v.kind == Json::Str) d.file = v.s;
        else if (k == "source" && v.kind == Json::Str) d.source = v.s;
        else if (k == "sha1" && v.kind == Json::Str) d.sha1 = v.s;
        else if (k == "kind" && v.kind == Json::Str) d.kind = v.s;
        else if (k == "size" && v.kind == Json::Int) d.size = v.i;
        else if (k == "steps" && v.kind == Json::List) {
            py::list<Step> steps;
            for (const Json& e : v.list) steps.append(step_of(e));
            d.steps = steps;
        } else d.other.set(key, v);
    }
    return d;
}

py::str sha1_hex(const py::str& path) {
    py::hashlib::Hash h = py::hashlib::sha1();
    py::file f = py::open_rb(path);
    while (true) {
        py::bytes block = f.read(1 << 20);
        if (block.empty()) break;
        h.update(block);
    }
    f.close();
    return h.hexdigest();
}

}  // namespace

const py::list<py::str> FOLDERS = {py::str("original"), py::str("src"), py::str("assets"), py::str("build"), py::str("reports")};
const py::set<py::str> _GENERIC = {py::str("default"), py::str("main"), py::str("setup"), py::str("install"),
                                   py::str("game"), py::str("program"), py::str("app"), py::str("boot")};

std::function<py::str()>& _clock() {
    static std::function<py::str()> c = [] {
        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm{};
        localtime_s(&tm, &t);
        char b[32];
        std::strftime(b, sizeof b, "%Y-%m-%dT%H:%M:%S", &tm);
        return py::str(b);
    };
    return c;
}

py::str root() {
    py::str configured = py::settings::get_str(py::str("converted"));
    py::str path = !configured.empty() ? configured : py::os::path::join(py::settings::checkout_root(), py::str("Converted"));
    py::os::makedirs(path, 0777, true);
    return path;
}

py::str _sha1(const py::str& path) { return sha1_hex(path); }

py::str _clean(const py::str& name) {
    // re.sub(r"[^A-Za-z0-9._-]+", "_", name).strip("._"); the name or "binary"
    std::u32string out;
    bool in_run = false;
    for (char32_t c : name.u32()) {
        if (is_cb_char(c)) {
            out.push_back(c);
            in_run = false;
        } else if (!in_run) {
            out.push_back('_');
            in_run = true;
        }
    }
    py::str s = py::str(out).strip(py::str("._"));
    return s.empty() ? py::str("binary") : s;
}

py::str default_name(const py::str& path) {
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    if (!_GENERIC.contains(stem.lower())) return _clean(stem);
    try {
        if (formats::xex::is_xex(path)) {
            std::optional<py::str> n = formats::xex::header(path).name;
            py::str own = std::get<0>(py::os::path::splitext(n && !n->empty() ? *n : py::str("")));
            if (!own.empty() && !_GENERIC.contains(own.lower())) return _clean(own);
        }
    } catch (const py::Exception&) {
    } catch (const std::exception&) {
    }
    py::str parent = py::os::path::basename(py::os::path::dirname(py::os::path::abspath(path)));
    return !parent.empty() ? _clean(parent) : _clean(stem);
}

py::str _kind(const py::str& path) {
    try {
        py::list<formats::detect::Detected> found = formats::detect::identify(path);
        if (!found.empty()) return found[0].format;
    } catch (const py::Exception&) {
    } catch (const std::exception&) {
    }
    return py::str("");
}

// ---- Project
Project::Project(const py::str& folder) : root(folder) {
    original = py::os::path::join(folder, py::str("original"));
    src = py::os::path::join(folder, py::str("src"));
    assets = py::os::path::join(folder, py::str("assets"));
    build = py::os::path::join(folder, py::str("build"));
    reports = py::os::path::join(folder, py::str("reports"));
    for (const py::str& n : FOLDERS) py::os::makedirs(py::os::path::join(folder, n), 0777, true);
    manifest = py::os::path::join(folder, py::str("project.json"));
}

ProjectData Project::data() const {
    try {
        return data_of(knowledge::sources::_json(manifest));
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    return ProjectData{};
}

py::str Project::name() const { return py::os::path::basename(root); }

std::optional<py::str> Project::original_file() const {
    ProjectData d = data();
    if (d.file && !d.file->empty()) return py::os::path::join(original, *d.file);
    return std::nullopt;
}

void Project::record(const py::str& command, const py::list<py::str>& outputs, const py::str& notes) const {
    ProjectData d = data();
    py::list<py::str> rel;
    for (const py::str& o : outputs) {
        py::str out = py::os::path::abspath(o);
        rel.append(out.startswith(py::os::path::abspath(root)) ? py::os::path::relpath(out, root) : out);
    }
    Step step;
    step.command = command;
    step.when = _clock()();
    step.outputs = rel;
    if (!notes.empty()) step.notes = notes;
    // one entry per command: running it again replaces what it said before
    if (!d.steps && d.other.contains(py::str("steps"))) {
        const Json& v = d.other.at(py::str("steps"));
        throw py::TypeError(std::string("'") + type_of(v) + "' object is not iterable");
    }
    py::list<Step> steps;
    if (d.steps)
        for (const Step& s : *d.steps) {
            if (s.not_a_dict) no_attr(*s.not_a_dict, "get");
            // (s.get("command") != command: a step without a command is kept)
            if (!(s.command && *s.command == command)) steps.append(s);
        }
    steps.append(step);
    d.steps = steps;
    _write(d);
}

void Project::_write(const ProjectData& d) const {
    {
        py::file f = py::open(manifest, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(py::str(dump(d)));
        f.close();
    }
    // d.get(key): the typed field, else what the dict holds under it (another type), else the default
    auto text_of = [&](const std::optional<py::str>& typed, const char* key, const py::str& dflt) -> py::str {
        if (typed) return *typed;
        if (auto it = d.other.get(py::str(key))) return scalar_text(*it);
        return dflt;
    };
    py::str nm = d.name ? *d.name : name();
    if (!d.name)
        if (auto it = d.other.get(py::str("name"))) {
            if (it->kind != Json::List && it->kind != Json::Dict) throw py::TypeError(std::string("object of type '") + type_of(*it) + "' has no len()");
            throw py::NotImplementedError("README.txt of a name that is a list or an object");
        }
    py::list<py::str> lines;
    lines.append(nm);
    lines.append(py::str(std::u32string(size_t(nm.size()), U'=')));
    lines.append(py::str(""));
    lines.append(py::str("What CodeBreaker made of ") + text_of(d.file, "file", py::str("a binary")) + py::str("."));
    lines.append(py::str(""));
    lines.append(py::str("  Input:   ") + text_of(d.source, "source", py::str("")));
    {
        // d.get("kind") or "unknown"
        py::str kind = text_of(d.kind, "kind", py::str(""));
        auto it = d.other.get(py::str("kind"));
        bool falsy = kind.empty() || (it && ((it->kind == Json::None) || (it->kind == Json::Bool && !it->b) || (it->kind == Json::Int && it->i == 0) ||
                                             (it->kind == Json::Float && it->d == 0.0)));
        lines.append(py::str("  Kind:    ") + (falsy ? py::str("unknown") : kind));
    }
    lines.append(py::str("  Size:    ") + (d.size ? py::str(std::to_string(*d.size)) : text_of(std::nullopt, "size", py::str("?"))) + py::str(" bytes"));
    lines.append(py::str("  SHA-1:   ") + text_of(d.sha1, "sha1", py::str("")));
    lines.append(py::str(""));
    lines.append(py::str("Folders:"));
    lines.append(py::str("  original/  the binary as it came in"));
    lines.append(py::str("  src/       source CodeBreaker produced"));
    lines.append(py::str("  assets/    resources and data"));
    lines.append(py::str("  build/     what was built from src/"));
    lines.append(py::str("  reports/   analysis, listings, logs"));
    lines.append(py::str(""));
    if (d.steps)
        for (const Step& s : *d.steps) {
            auto need_text = [&](const std::optional<py::str>& typed, const char* key) -> py::str {
                if (typed) return *typed;
                if (auto it = s.other.get(py::str(key))) return scalar_text(*it);
                throw py::KeyError(py::repr(py::str(key)));
            };
            py::str command = need_text(s.command, "command");
            py::str when;
            if (s.when) when = *s.when;
            else if (auto it = s.other.get(py::str("when"))) no_attr(*it, "replace");
            else throw py::KeyError(py::repr(py::str("when")));
            lines.append(command + py::str("  (") + when.replace(py::str("T"), py::str(" ")) + py::str(")"));
            if (s.outputs)
                for (const py::str& o : *s.outputs) lines.append(py::str("    ") + o);
            else if (s.other.contains(py::str("outputs")))
                throw py::NotImplementedError("README.txt of outputs that are not a list of texts");
            if (s.notes)
                for (const py::str& n : py::splitlines(*s.notes)) lines.append(py::str("    ") + n);
            else if (auto it = s.other.get(py::str("notes")))
                no_attr(*it, "splitlines");
            lines.append(py::str(""));
        }
    lines.append(py::str("The binary and everything made from it are the original's owner's: this folder is "
                         "never committed."));
    py::file f = py::open(py::os::path::join(root, py::str("README.txt")), py::str("w"), py::str("utf-8"), std::nullopt, py::str("\r\n"));
    f.write(py::join(py::str("\n"), lines) + py::str("\n"));
    f.close();
}

Project project(const py::str& path_, const std::optional<py::str>& name, const std::optional<py::str>& base_) {
    py::str path = py::os::path::abspath(path_);
    py::str digest = _sha1(path);
    py::str base = base_ && !base_->empty() ? *base_ : root();
    py::os::makedirs(base, 0777, true);
    for (const py::str& entry : py::sorted(py::os::listdir(base))) {
        py::str manifest = py::os::path::join(py::os::path::join(base, entry), py::str("project.json"));
        if (py::os::path::isfile(manifest)) {
            try {
                Json j = knowledge::sources::_json(manifest);
                if (j.kind != Json::Dict) no_attr(j, "get");
                std::optional<py::str> sha = str_of(j, "sha1");
                if (sha && *sha == digest && (!name || entry == _clean(*name))) return Project(py::os::path::join(base, entry));
            } catch (const py::OSError&) {
                continue;
            } catch (const py::ValueError&) {
                continue;
            }
        }
    }
    py::str wanted = name && !name->empty() ? _clean(*name) : default_name(path);
    py::str folder = py::os::path::join(base, wanted);
    int64_t n = 2;
    while (py::os::path::exists(folder)) {
        folder = py::os::path::join(base, wanted + py::str("_" + std::to_string(n)));
        n += 1;
    }
    Project p(folder);
    py::str copy = py::os::path::join(p.original, py::os::path::basename(path));
    py::shutil::copy2(path, copy);
    ProjectData d;
    d.name = py::os::path::basename(folder);
    d.file = py::os::path::basename(path);
    d.source = path;
    d.sha1 = digest;
    d.size = py::os::path::getsize(path);
    d.kind = _kind(path);
    d.steps = py::list<Step>();
    p._write(d);
    return p;
}

}  // namespace codebreaker::base::converted
