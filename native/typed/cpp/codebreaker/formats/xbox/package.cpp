// codebreaker/formats/xbox/package.cpp -- typed C++ port of codebreaker/formats/xbox/__init__.py (see package.hpp).
#include "codebreaker/formats/xbox/package.hpp"

#include "codebreaker/formats/xbox/bink.hpp"
#include "codebreaker/formats/xbox/chd.hpp"
#include "codebreaker/formats/xbox/xur.hpp"
#include "codebreaker/formats/xbox/xwb.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox {

using py::str;

namespace {

Hooks& hooks_() {
    static Hooks h;
    return h;
}

py::bytes read_all_(const str& path) {
    py::file f = py::open_rb(path);
    return f.read();
}

Summary dict_() { return Summary(); }

}  // namespace

void set_hooks(const Hooks& hooks) { hooks_() = hooks; }

Summary _container(const str& path) {
    if (!hooks_().volumes) throw py::ImportError("formats.media.volumes has no typed port: install it with set_hooks");
    py::list<Volume> vols = hooks_().volumes(path);
    Summary out;
    out.set(str("volumes"), Field(int64_t(py::len(vols))));
    for (int64_t i = 0; i < py::len(vols) && i < 4; i++) {
        const Volume& v = vols[i];
        int64_t n = 0, total = 0;
        for (const FileEntry& e : v.files) {
            if (!e.is_dir) {
                n++;
                total += e.size;
            }
        }
        str key = py::len(vols) == 1 ? str("volume") : py::percent(str("volume %d"), i + 1);
        str where = (v.where && !v.where->empty()) ? py::percent(str(" (%s)"), *v.where) : str("");
        out.set(key, Field(py::percent(str("%s %r: %d files, %d bytes%s"), v.kind, v.label, n, total, where)));
        for (const auto& kv : v.boot.items()) {
            str k = py::len(vols) > 1 ? py::percent(str("%s %s"), key, std::get<0>(kv)) : std::get<0>(kv);
            out.set(k, std::get<1>(kv));
        }
    }
    return out;
}

Summary _xbe(const str& path) {
    py::bytes data = read_all_(path);
    if (!hooks_().xbe) throw py::ImportError("formats.xbox.xbe has no typed port: install it with set_hooks");
    XbeInfo x = hooks_().xbe(data);
    Summary out;
    out.set(str("title"), Field(x.title_name));
    out.set(str("title id"), Field(py::percent(str("%08X"), x.title_id)));
    out.set(str("base"), Field(py::percent(str("0x%08X"), x.base)));
    out.set(str("image size"), Field(x.size_of_image));
    out.set(str("sections"), Field(x.section_count));
    out.set(str("region"), Field(py::percent(str("0x%X"), x.region)));
    out.set(str("allowed media"), Field(py::percent(str("0x%X"), x.allowed_media)));
    return out;
}

Summary _xdbf(const str& path) {
    py::bytes data = read_all_(path);
    if (!hooks_().xdbf) throw py::ImportError("formats.xbox.xdbf has no typed port: install it with set_hooks");
    XdbfInfo x = hooks_().xdbf(data);
    Summary out;
    out.set(str("version"), Field(py::percent(str("0x%X"), x.version)));
    out.set(str("entries"), Field(x.entry_count));
    out.set(str("free"), Field(x.free_count));
    for (const auto& ns : x.namespaces) out.set(py::percent(str("namespace %s"), std::get<0>(ns)), Field(std::get<1>(ns)));
    if (x.achievements) {   // (try: ... except Exception: pass)
        int64_t unlocked = 0, score = 0;
        for (const auto& a : *x.achievements) {
            if (std::get<0>(a)) unlocked++;
            score += std::get<1>(a);
        }
        out.set(str("achievements"), Field(py::percent(str("%d (%d unlocked, %dG)"), static_cast<int64_t>(x.achievements->size()), unlocked, score)));
    }
    return out;
}

Summary _xpr(const str& path) {
    py::bytes data = read_all_(path);
    if (!hooks_().xpr) throw py::ImportError("formats.xbox.xpr has no typed port: install it with set_hooks");
    py::dict<str, Field> info = hooks_().xpr(data);
    Summary out;
    for (const auto& kv : info.items())
        if (!std::holds_alternative<NonScalar>(std::get<1>(kv))) out.set(std::get<0>(kv), std::get<1>(kv));
    return out;
}

Summary _xwb(const str& path) {
    xwb::XWB w(read_all_(path));
    Summary out;
    out.set(str("version"), Field(w.version));
    out.set(str("byte order"), Field(w.en == str(">") ? str("big") : str("little")));
    // for k in ("entries", "names", "bank_name"): v = getattr(w, k, None); if v is not None: ... (XWB has no `names`)
    out.set(str("entries"), Field(static_cast<int64_t>(py::len(w.entries))));
    out.set(str("bank_name"), Field(w.bank_name));
    return out;
}

Summary _xur(const str& path) {
    xur::XUR x(read_all_(path));
    Summary out;
    out.set(str("version"), Field(x.version));
    out.set(str("tool version"), Field(x.tool_version));
    out.set(str("sections"), Field(x.section_count));
    py::list<str> tags;
    for (const auto& s : x.sections) tags.append(py::to_str(std::get<0>(s)));   // (the magic is an int: str(m))
    out.set(str("section tags"), Field(py::join(str(" "), tags)));
    return out;
}

Summary _bink(const str& path) {
    bink::BinkHeader h = bink::read_header(path);
    Summary out;
    out.set(str("generation"), Field(h.generation));
    out.set(str("codec"), Field(h.codec));
    out.set(str("file_size"), Field(h.file_size));
    out.set(str("frames"), Field(h.frames));
    out.set(str("largest_frame"), Field(h.largest_frame));
    out.set(str("width"), Field(h.width));
    out.set(str("height"), Field(h.height));
    out.set(str("fps"), Field(h.fps));
    out.set(str("video_flags"), Field(h.video_flags));
    out.set(str("audio_tracks"), Field(h.audio_tracks));
    return out;
}

Summary _chd(const str& path) {
    chd::Header h = chd::read_header(path);
    Summary out;
    out.set(str("version"), Field(h.version));
    out.set(str("header_len"), Field(h.header_len));
    out.set(str("logical_bytes"), Field(h.logical_bytes));
    out.set(str("hunk_bytes"), Field(h.hunk_bytes));
    out.set(str("unit_bytes"), Field(h.unit_bytes));
    out.set(str("compressors"), Field(py::join(str(", "), h.compressors)));
    out.set(str("meta_offset"), Field(h.meta_offset));
    out.set(str("map_offset"), Field(h.map_offset));
    return out;
}

const py::list<std::tuple<str, std::function<Summary(const str&)>>>& _BY_FORMAT() {
    static const py::list<std::tuple<str, std::function<Summary(const str&)>>> by = [] {
        py::list<std::tuple<str, std::function<Summary(const str&)>>> l;
        l.append({str("XBE"), _xbe});
        l.append({str("XDBF"), _xdbf});
        l.append({str("resource bundle"), _xpr});
        l.append({str("wave bank"), _xwb});
        l.append({str("XUI scene"), _xur});
        l.append({str("Bink"), _bink});
        l.append({str("CHD"), _chd});
        return l;
    }();
    return by;
}

Summary summary(const str& path, const py::dict<str, str>& match) {
    auto command = match.get(str("command"));
    if (py::os::path::isdir(path) || (command && *command == str("media"))) return _container(path);
    str format = match.get(str("format"), str(""));
    for (const auto& kf : _BY_FORMAT()) {
        if (format.contains(std::get<0>(kf))) return std::get<1>(kf)(path);   // key in match.get("format", "")
    }
    return dict_();
}

}  // namespace codebreaker::formats::xbox
