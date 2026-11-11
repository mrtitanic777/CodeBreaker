// codebreaker/recompile/title.cpp -- typed C++ from codebreaker.recompile.title (see title.hpp).
#include "codebreaker/recompile/title.hpp"

#include <filesystem>
#include <variant>

#include "json.hpp"
#include "py_json.hpp"

namespace codebreaker::recompile::title {

namespace volumes = codebreaker::formats::xbox::volumes;

const py::list<py::bytes> _STFS = py::list<py::bytes>({py::bytes("CON "), py::bytes("LIVE"), py::bytes("PIRS")});
const py::list<py::str> _DISCS = py::list<py::str>({py::str(".iso"), py::str(".xiso"), py::str(".cso"), py::str(".cci")});
const py::str _MARKER = py::str(".codebreaker-unpacked");
const int64_t _FORMAT = 2;

namespace {

// shutil.copy2(src, dst): the file and its modification time
void copy2_(const py::str& src, const py::str& dst) {
    py::shutil::copyfile(src, dst);
    std::error_code ec;
    auto t = std::filesystem::last_write_time(py::fspath_(src), ec);
    if (!ec) std::filesystem::last_write_time(py::fspath_(dst), t, ec);
}

bool endswith_any_(const py::str& s, const py::list<py::str>& tails) {
    for (const py::str& t : tails)
        if (s.endswith(t)) return true;
    return false;
}

// the dict a marker holds: {"source": ..., "size": ..., "format": ...}
py::V stamp_value_(const py::str& source, int64_t size, int64_t format) {
    py::V d = py::V::dict();
    d.set("source", py::V(source.utf8()));
    d.set("size", py::V(size));
    d.set("format", py::V(format));
    return d;
}

}  // namespace

Title::Title(const py::str& source, const std::optional<py::str>& name, std::shared_ptr<media::Volume> volume,
             const std::optional<py::str>& folder)
    : source(source), name(name), _volume(std::move(volume)), _folder(folder) {}

std::optional<media::Entry> Title::_xex_entry() {
    for (const media::Entry& e : _volume->files()) {
        if (!e.is_dir && e.path.replace(py::str("\\"), py::str("/")).strip(py::str("/")).lower() == py::str("default.xex")) return e;
    }
    return std::nullopt;
}

py::bytes Title::read_xex() {
    if (_folder && !_folder->empty()) {
        std::optional<py::str> n = _default_xex(*_folder);
        if (!n) throw py::TypeError("join() argument must be str, bytes, or os.PathLike object, not 'NoneType'");
        py::file f = py::open(py::os::path::join(*_folder, *n), py::str("rb"));
        py::bytes data = f.read();
        f.close();
        return data;
    }
    std::optional<media::Entry> e = _xex_entry();
    if (!e) throw py::AttributeError("'NoneType' object has no attribute 'path'");
    return _volume->read(*e);
}

int64_t Title::unpack(const py::str& folder) {
    py::str marker = py::os::path::join(folder, _MARKER);
    py::V stamp = stamp_value_(py::os::path::abspath(source), _size(source), _FORMAT);
    try {
        py::file f = py::open(marker, py::str("r"), py::str("utf-8"));
        py::V got = py::json_parse(f.read_text().utf8());
        if (got == stamp) return 0;
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {          // (a marker that is not UTF-8)
    } catch (const py::PyError& e) {           // (json.JSONDecodeError is a ValueError)
        if (e.type != "ValueError") throw;
    }
    int64_t n = 0;
    if (_folder && !_folder->empty()) {
        for (const auto& w : py::os::walk(*_folder)) {
            const py::str& dp = std::get<0>(w);
            for (const py::str& name_ : std::get<2>(w)) {
                py::str src = py::os::path::join(dp, name_);
                py::str dst = py::os::path::join(folder, py::os::path::relpath(src, *_folder));
                py::os::makedirs(py::os::path::dirname(dst), 0777, true);
                copy2_(src, dst);
                n += 1;
            }
        }
    } else {
        for (const media::Entry& e : _volume->files()) {
            if (e.is_dir) continue;
            py::str dst = folder;
            for (const py::str& p : py::split(e.path.replace(py::str("\\"), py::str("/")), py::str("/")))
                if (!p.empty()) dst = py::os::path::join(dst, p);
            py::os::makedirs(py::os::path::dirname(dst), 0777, true);
            py::file f = py::open(dst, py::str("wb"));
            f.write(_volume->read(e));
            f.close();
            n += 1;
        }
    }
    // json.dump(stamp, f): {"source": ..., "size": ..., "format": ...}
    std::string text = "{\"source\": ";
    py::json::put(text, py::os::path::abspath(source));
    text += ", \"size\": " + std::to_string(_size(source)) + ", \"format\": " + std::to_string(_FORMAT) + "}";
    py::file f = py::open(marker, py::str("w"), py::str("utf-8"));
    f.write(py::str(text));
    f.close();
    return n;
}

int64_t _size(const py::str& path) {
    if (py::os::path::isfile(path)) return py::os::path::getsize(path);
    int64_t total = 0;
    for (const auto& w : py::os::walk(path)) {
        const py::str& dp = std::get<0>(w);
        for (const py::str& n : std::get<2>(w)) total += py::os::path::getsize(py::os::path::join(dp, n));
    }
    return total;
}

std::optional<py::str> _default_xex(const py::str& folder) {
    for (const py::str& n : py::os::listdir(folder)) {
        py::str path = py::os::path::join(folder, n);
        if (n.lower() == py::str("default.xex") && py::os::path::isfile(path)) {
            py::file f = py::open(path, py::str("rb"));
            py::bytes head = f.read(4);
            f.close();
            if (head == py::bytes("XEX2")) return n;
        }
    }
    return std::nullopt;
}

std::optional<Title> _from_volumes(const py::str& path, const std::optional<volumes::VolumeList>& found) {
    if (!found) return std::nullopt;
    for (const std::shared_ptr<media::Volume>& v : *found) {
        Title t(path, std::nullopt, v);
        if (t._xex_entry()) {
            // label = v.boot.get("display name") if isinstance(v.boot, dict) else None
            std::optional<py::str> label;
            auto it = v->boot.get(py::str("display name"));
            if (it) {
                if (std::holds_alternative<py::str>(*it)) {
                    if (!std::get<py::str>(*it).empty()) label = std::get<py::str>(*it);
                } else if (std::get<int64_t>(*it) != 0) {
                    throw py::AttributeError("'int' object has no attribute 'lower'");
                }
            }
            py::str name = label ? *label : (!v->label.empty() ? v->label : py::os::path::basename(path));
            if (endswith_any_(name.lower(), _DISCS)) name = std::get<0>(py::os::path::splitext(name));
            std::u32string kept;
            for (char32_t c : name.u32())
                if (c >= 0x20 && c < 0x7F) kept.push_back(c);   // (isascii and isprintable)
            py::str cleaned = py::str(kept).strip();
            t.name = cleaned.empty() ? py::str("title") : cleaned;
            return t;
        }
    }
    return std::nullopt;
}

std::optional<Title> _container(const py::str& path) {
    py::bytes magic;
    try {
        py::file f = py::open(path, py::str("rb"));
        magic = f.read(4);
        f.close();
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    if (!_STFS.contains(magic) && !endswith_any_(path.lower(), _DISCS)) return std::nullopt;
    try {
        return _from_volumes(path, volumes::volumes(path));
    } catch (const py::Exception&) {   // a damaged or foreign file: not this title
        return std::nullopt;
    }
}

std::optional<Title> locate(const py::str& path0) {
    py::str path = py::os::path::abspath(path0);
    if (py::os::path::isfile(path)) return _container(path);
    if (!py::os::path::isdir(path)) return std::nullopt;
    if (_default_xex(path)) return Title(path, py::os::path::basename(path), nullptr, path);
    try {
        std::optional<Title> t = _from_volumes(path, volumes::volumes(path));   // Games on Demand
        if (t) {
            if (!t->name || *t->name == py::str("GoD")) t->name = py::os::path::basename(path);
            return t;
        }
    } catch (const py::Exception&) {
    }
    py::os::walk_t walker(path);
    for (auto it = walker.begin(); it != walker.end(); ++it) {
        const py::str& dp = std::get<0>(it.cur);
        py::list<py::str> dn = std::get<1>(it.cur);   // (shared with the walk: sorting it orders the descent)
        py::list<py::str> fn = std::get<2>(it.cur);
        dn.sort();
        for (const py::str& name : py::sorted(fn)) {
            std::optional<Title> t = _container(py::os::path::join(dp, name));
            if (t) return t;
        }
        for (const py::str& d : dn) {
            if (_default_xex(py::os::path::join(dp, d))) {
                py::str sub = py::os::path::join(dp, d);
                return Title(sub, d, nullptr, sub);
            }
        }
    }
    return std::nullopt;
}

}  // namespace codebreaker::recompile::title
