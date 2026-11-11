// codebreaker/exact/plm86.cpp -- typed C++ port of codebreaker.exact.plm86 (see plm86.hpp).

#include "codebreaker/exact/plm86.hpp"

#include <algorithm>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/exact/emu/isis.hpp"
#include "py_os.hpp"
#include "json.hpp"

namespace codebreaker::exact::plm86 {

namespace tcm = codebreaker::exact::toolchain;
namespace cache = codebreaker::base::cache;

const py::str CACHE_VERSION("p2");

// the profile's JSON, as the Python's dict
struct PLM86::Raw {
    py::V profile;
    // p.get(key, default) of a string entry
    py::str str_or(const char* key, const char* dflt) const {
        const py::V& v = profile.get(key);
        return v.is_str() ? py::str(v.str()) : py::str(dflt);
    }
};

namespace {

py::bytes crlf(const py::bytes& b) {
    // b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
    const std::string& s = b.raw();
    std::string lf;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n') continue;
        lf.push_back(s[i]);
    }
    std::string out;
    for (char c : lf) {
        if (c == '\n') out += "\r\n";
        else out.push_back(c);
    }
    return py::bytes(out);
}

void write_file(const py::str& path, const py::bytes& data) {
    py::file f = py::file::open(path, py::str("wb"));
    f.write(data);
    f.close();
}

}  // namespace

PLM86::PLM86(const toolchain::Profile& profile, const std::optional<py::str>& variant_in)
    : toolchain::Toolchain(profile, std::nullopt) {
    auto raw = std::make_shared<Raw>();
    raw->profile = py::json_parse(profile.keyed.utf8());
    raw_ = raw;
    name = profile.name;
    // self.variant = variant or profile.get("default_variant")
    if (variant_in && !variant_in->empty()) variant = *variant_in;
    else if (profile.default_variant && !profile.default_variant->empty()) variant = *profile.default_variant;
    else variant = py::str("");
    // v = profile.get("variants", {}).get(self.variant, {})
    const py::V& variants = raw->profile.get("variants");
    const py::V& v = variants.is_dict() ? variants.get(variant.utf8().c_str()) : variants;
    const py::V& pv = raw->profile.get("controls");
    const py::str profile_controls = pv.is_str() ? py::str(pv.str()) : py::str("");
    version = (v.is_dict() && v.get("version").is_str()) ? py::str(v.get("version").str()) : py::str("2.1");
    controls = (v.is_dict() && v.get("controls").is_str()) ? py::str(v.get("controls").str()) : profile_controls;
    key = py::hashlib::sha1((name + version + controls + CACHE_VERSION).encode()).hexdigest().slice(0, 12);
}

std::tuple<std::optional<py::bytes>, py::str> PLM86::compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                             const py::list<py::str>& include,
                                                             const std::optional<py::str>& switches) const {
    return compile(src, std::optional<py::dict<py::str, py::bytes>>(headers), include, switches, true);
}

std::tuple<std::optional<py::bytes>, py::str> PLM86::compile(const py::bytes& src_in,
                                                             const std::optional<py::dict<py::str, py::bytes>>& headers,
                                                             const py::list<py::str>& include, const std::optional<py::str>& switches,
                                                             bool use_cache) const {
    (void)include;
    const py::bytes src = crlf(src_in);
    // hs = sorted((f.upper(), crlf(b)) for f, b in (headers or {}).items())
    std::vector<std::tuple<py::str, py::bytes>> sorted_hs;
    if (headers) {
        for (const auto& kv : headers->items()) sorted_hs.emplace_back(std::get<0>(kv).upper(), crlf(std::get<1>(kv)));
    }
    std::sort(sorted_hs.begin(), sorted_hs.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a).u32() != std::get<0>(b).u32()) return std::get<0>(a).u32() < std::get<0>(b).u32();
        return std::get<1>(a).raw() < std::get<1>(b).raw();
    });
    py::list<std::tuple<py::str, py::bytes>> hs;
    for (const auto& h : sorted_hs) hs.append(h);
    // controls = switches or self.controls
    const py::str ctl = (switches && !switches->empty()) ? *switches : controls;
    // key = sha1(b"\0".join([src, repr(hs).encode(), controls.encode(), self.key.encode()])).hexdigest()
    py::list<py::str> shown;
    for (const auto& h : hs) shown.append(py::str("(") + py::repr(std::get<0>(h)) + py::str(", ") + py::repr(std::get<1>(h)) + py::str(")"));
    const py::str hs_repr = py::str("[") + py::join(py::str(", "), shown) + py::str("]");
    std::string joined = src.raw();
    joined.push_back('\0');
    joined += hs_repr.utf8();
    joined.push_back('\0');
    joined += ctl.utf8();
    joined.push_back('\0');
    joined += key.utf8();
    const py::str k = py::hashlib::sha1(py::bytes(joined)).hexdigest();
    using Result = std::tuple<std::optional<py::bytes>, py::str>;
    if (use_cache) {
        const std::optional<Result> got = cache::value<Result>(py::str("exact"), k);
        if (got) return *got;
    }
    const Result res = _compile(src, hs, ctl);
    cache::keep<Result>(py::str("exact"), k, res);
    return res;
}

std::tuple<std::optional<py::bytes>, py::str> PLM86::_compile(const py::bytes& src,
                                                              const py::list<std::tuple<py::str, py::bytes>>& headers,
                                                              const py::str& ctl) const {
    const Raw& p = *raw_;
    if (!p.profile.get("kit").is_str()) throw py::KeyError("'kit'");
    const py::str kit = py::os::path::join(tcm::kit_dir(py::str(p.profile.get("kit").str())), p.str_or("tool", "plm86"), version);
    const py::str name_ = p.str_or("source", "T.PLM");
    const py::str w = py::tempfile::mkdtemp(std::nullopt, py::str("isis_"));
    try {
        for (const py::str& f : py::os::listdir(kit)) py::shutil::copy(py::os::path::join(kit, f), py::os::path::join(w, f.upper()));
        write_file(py::os::path::join(w, name_), src);
        for (const auto& h : headers) write_file(py::os::path::join(w, std::get<0>(h)), std::get<1>(h));
        py::dict<py::str, py::str> values;
        values.set(py::str("controls"), ctl);
        const py::V& tv = p.profile.get("tail");
        const py::str tail = tcm::format_named(tv.is_str() ? py::str(tv.str()) : py::str(" ") + name_ + py::str(" {controls}"), values);
        py::dict<int64_t, py::str> drives;
        drives.set(0, w);
        drives.set(1, w);
        const py::V& lv = p.profile.get("limit");
        const int64_t limit = lv.is_intlike() ? lv.as_int() : 400000000;
        const emu::isis::Result r = emu::isis::run(py::os::path::join(w, p.str_or("program", "PLM86")), tail, drives, false, limit);
        const py::str con = r.console;
        const py::str stem = std::get<0>(py::os::path::splitext(name_));
        const py::str lp = py::os::path::join(w, stem + py::str(".LST"));
        py::str lst("");
        if (py::os::path::exists(lp)) {
            py::file f = py::open_rb(lp);
            lst = f.read().decode("latin1");
            f.close();
        }
        const py::str obj = py::os::path::join(w, stem + py::str(".OBJ"));
        bool ok = true;
        const py::V& okv = p.profile.get("ok");
        if (okv.is_str() && !okv.str().empty()) {
            const std::optional<py::re::Match> m = py::re::search(py::str(okv.str()), con + lst);
            bool matched = static_cast<bool>(m);
            if (matched) {
                const py::list<std::optional<py::str>> groups = m->groups_opt();
                ok = groups.size() == 0 || (groups[0] && *groups[0] == py::str("0"));
            } else {
                ok = false;
            }
        }
        std::tuple<std::optional<py::bytes>, py::str> result;
        if (!py::os::path::exists(obj) || !ok) {
            py::list<py::str> bad;
            for (const py::str& line : py::split(lst.replace(py::str("\r"), py::str("")), py::str("\n"))) {
                if (line.contains(py::str("***")) || line.contains(py::str("ERROR"))) bad.append(line);
            }
            const py::str errs = py::join(py::str("\n"), bad).slice(0, 1500);
            result = {std::nullopt, con + py::str("\n") + errs};
        } else {
            py::file f = py::open_rb(obj);
            const py::bytes data = f.read();
            f.close();
            result = {data, con};
        }
        py::shutil::rmtree(w, true);
        return result;
    } catch (...) {
        py::shutil::rmtree(w, true);
        throw;
    }
}

void register_plugin() {
    tcm::register_plugin(py::str("codebreaker.exact.plm86:PLM86"),
                         [](const toolchain::Profile& p, const std::optional<py::str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                             return std::make_shared<PLM86>(p, variant);
                         });
}

}  // namespace codebreaker::exact::plm86
