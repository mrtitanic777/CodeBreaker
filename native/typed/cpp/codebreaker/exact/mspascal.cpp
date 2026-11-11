// codebreaker/exact/mspascal.cpp -- typed C++ port of codebreaker.exact.mspascal (see mspascal.hpp).

#include "codebreaker/exact/mspascal.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/omf.hpp"
#include "json.hpp"
#include "py_os.hpp"
#include "zlib.h"

namespace codebreaker::exact::mspascal {

namespace tcm = codebreaker::exact::toolchain;
namespace cache = codebreaker::base::cache;

const py::str CACHE_VERSION("pas1");
const py::list<py::bytes> HELPERS = [] {
    py::list<py::bytes> l;
    l.append(py::bytes::fromhex(py::str("5b32e42e8a07433c807208247f8ae02e8a07432e8b0f4343558bec2be9")));     // 1.10, 2.0x
    l.append(py::bytes::fromhex(py::str("5b2e8b0f4343558bec2be9")));                                          // 3.0x: only the frame size
    l.append(py::bytes::fromhex(py::str("5e2e8b0c4646558bec2be9")));                                          // 3.11: through SI
    return l;
}();
const py::bytes INLINE = py::bytes::fromhex(py::str("558bec81ec"));                                          // (3.11: the frame set up in line)

namespace {

// one open procedure of emit_order: its name, whether its body has begun, and the depth of what is open
struct Frame {
    py::str name;
    bool begun;
    int64_t depth;
};

py::str upper_of(const py::str& s) { return s.upper(); }

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state_path(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

std::string read_all(const py::str& path) {
    py::file f = py::open_rb(path);
    const py::bytes b = f.read();
    f.close();
    return b.raw();
}

void write_file(const py::str& path, const py::bytes& data) {
    py::file f = py::file::open(path, py::str("wb"));
    f.write(data);
    f.close();
}

uint32_t rd32(const std::string& s, size_t at) {
    uint32_t v;
    std::memcpy(&v, s.data() + at, 4);
    return v;
}
uint16_t rd16(const std::string& s, size_t at) {
    uint16_t v;
    std::memcpy(&v, s.data() + at, 2);
    return v;
}

// zipfile.ZipFile(path).extractall(dest): stored and deflated entries
void extractall(const py::str& zip_path, const py::str& dest) {
    const std::string z = read_all(zip_path);
    int64_t eocd = -1;
    for (int64_t i = static_cast<int64_t>(z.size()) - 22; i >= 0; --i) {
        if (rd32(z, static_cast<size_t>(i)) == 0x06054b50u) {
            eocd = i;
            break;
        }
    }
    if (eocd < 0) throw py::Exception("File is not a zip file");
    const size_t n = rd16(z, static_cast<size_t>(eocd) + 10);
    size_t at = rd32(z, static_cast<size_t>(eocd) + 16);
    for (size_t e = 0; e < n; ++e) {
        if (at + 46 > z.size() || rd32(z, at) != 0x02014b50u) throw py::Exception("Bad magic number for central directory");
        const uint16_t method = rd16(z, at + 10);
        const uint32_t csize = rd32(z, at + 20);
        const uint32_t usize = rd32(z, at + 24);
        const size_t fn = rd16(z, at + 28), ex = rd16(z, at + 30), cm = rd16(z, at + 32);
        const size_t local = rd32(z, at + 42);
        const std::string name = z.substr(at + 46, fn);
        at += 46 + fn + ex + cm;
        if (rd32(z, local) != 0x04034b50u) throw py::Exception("Bad magic number for file header");
        const size_t data = local + 30 + rd16(z, local + 26) + rd16(z, local + 28);
        std::string out;
        if (method == 0) {
            out = z.substr(data, csize);
        } else if (method == 8) {
            out.assign(usize, '\0');
            z_stream zs;
            std::memset(&zs, 0, sizeof zs);
            if (inflateInit2(&zs, -15) != Z_OK) throw py::OSError("zlib error");
            zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(z.data() + data));
            zs.avail_in = csize;
            zs.next_out = reinterpret_cast<Bytef*>(&out[0]);
            zs.avail_out = usize;
            const int rc = usize == 0 ? Z_STREAM_END : inflate(&zs, Z_FINISH);
            inflateEnd(&zs);
            if (rc != Z_STREAM_END) throw py::Exception("Error -3 while decompressing data");
        } else {
            throw py::NotImplementedError("That compression method is not supported");
        }
        write_file(py::os::path::join(dest, py::str(name)), py::bytes(out));
    }
}

py::bytes crlf(const py::bytes& b) {
    // b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
    const std::string& s = b.raw();
    std::string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n') continue;
        if (s[i] == '\n') out += "\r\n";
        else out.push_back(s[i]);
    }
    return py::bytes(out);
}

}  // namespace

py::list<py::str> emit_order(const py::str& text_in) {
    py::str text = py::re::sub(py::str("\\{[^}]*\\}|\\(\\*.*?\\*\\)"), py::str(" "), text_in, 0, py::re::S);
    text = py::re::sub(py::str("'[^'\\n]*'"), py::str("''"), text);
    py::list<py::str> out;
    std::vector<Frame> st;
    const py::list<py::str> toks = py::re::findall(py::str("[A-Za-z_][A-Za-z_0-9]*|[;()]"), text);
    int64_t k = 0;
    const int64_t nt = toks.size();
    while (k < nt) {
        const py::str w = upper_of(toks[k]);
        if ((w == py::str("PROCEDURE") || w == py::str("FUNCTION")) && (st.empty() || !st.back().begun) && !(!st.empty() && st.back().depth != 0)) {
            int64_t m = k + 2;
            if (m < nt && toks[m] == py::str("(")) {
                int64_t d = 1;
                m += 1;
                while (d) {
                    d += static_cast<int64_t>(toks[m] == py::str("(")) - static_cast<int64_t>(toks[m] == py::str(")"));
                    m += 1;
                }
            }
            while (toks[m] != py::str(";")) m += 1;
            m += 1;
            if (m < nt) {
                const py::str u = upper_of(toks[m]);
                if (u == py::str("FORWARD") || u == py::str("EXTERN") || u == py::str("EXTERNAL")) {
                    k = m + 1;
                    continue;
                }
            }
            st.push_back(Frame{upper_of(toks[k + 1]), false, 0});
            k = m;
            continue;
        }
        if (!st.empty()) {
            Frame& top = st.back();
            if (!top.begun) {
                if (w == py::str("RECORD")) {
                    top.depth += 1;
                } else if (w == py::str("END") && top.depth != 0) {
                    top.depth -= 1;
                } else if (w == py::str("BEGIN") && top.depth == 0) {
                    top.begun = true;
                    top.depth = 1;
                }
            } else {
                if (w == py::str("BEGIN") || w == py::str("CASE")) {
                    top.depth += 1;
                } else if (w == py::str("END")) {
                    top.depth -= 1;
                    if (top.depth == 0) {
                        out.append(top.name);
                        st.pop_back();
                    }
                }
            }
        }
        k += 1;
    }
    return out;
}

py::list<py::str> _segnames(const py::bytes& obj) {
    py::list<py::str> names;
    names.append(py::str(""));
    py::list<py::str> segs;
    for (const auto& rec : omf::records(obj)) {
        const int64_t t = std::get<0>(rec);
        const py::bytes& r = std::get<1>(rec);
        if (t == 0x96) {
            int64_t q = 0;
            while (q < r.size()) {
                names.append(r.slice(q + 1, q + 1 + r[q]).decode("latin1"));
                q += 1 + r[q];
            }
        } else if (t == 0x98) {
            int64_t q = (r[0] >> 5) ? 1 : 4;
            const int64_t ni = std::get<0>(omf::_idx(r, q + 2));
            segs.append(ni < names.size() ? names[ni] : py::str("?"));
        }
    }
    return segs;
}

py::bytes publish(const py::bytes& obj, const py::dict<py::str, int64_t>& procs, const py::str& code_seg) {
    const py::list<py::str> segs = _segnames(obj);
    int64_t found = -1;
    for (int64_t i = 0; i < segs.size(); i++) {
        if (segs[i] == code_seg) {
            found = i;
            break;
        }
    }
    if (found < 0) throw py::ValueError(code_seg.utf8() + " is not in list");
    const int64_t si = found + 1;
    std::string body(1, '\0');                       // (no group; the segment)
    if (si < 0x80) {
        body.push_back(static_cast<char>(si));
    } else {
        body.push_back(static_cast<char>(0x80 | (si >> 8)));
        body.push_back(static_cast<char>(si & 0xFF));
    }
    std::vector<std::pair<py::str, int64_t>> items;
    for (const auto& kv : procs.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    for (const auto& it : items) {
        body += omf::name(it.first).raw();
        if (it.second < 0 || it.second > 0xFFFF) throw py::StructError("ushort format requires 0 <= number <= 65535");
        body.push_back(static_cast<char>(it.second & 0xFF));
        body.push_back(static_cast<char>(it.second >> 8));
        body.push_back('\0');
    }
    const py::bytes rec = omf::record(0x90, py::bytes(body));
    std::string out;
    for (const auto& r : omf::records(obj)) {
        const int64_t t = std::get<0>(r);
        if (t == 0x8A || t == 0x8B) out += rec.raw();
        out += omf::record(t, std::get<1>(r)).raw();
    }
    return py::bytes(out);
}

// the profile's JSON, as the Python's dict
struct MSPascal::Raw {
    py::V profile;
};

MSPascal::MSPascal(const toolchain::Profile& profile, const std::optional<py::str>& variant_in)
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
    version = (v.is_dict() && v.get("version").is_str()) ? py::str(v.get("version").str()) : py::str("1.10");
    const py::V& hv = (v.is_dict() && v.get("helper_data").is_intlike()) ? v.get("helper_data") : raw->profile.get("helper_data");
    helper_data = hv.is_intlike() ? hv.as_int() : 3;
    key = py::hashlib::sha1((name + version + CACHE_VERSION).encode()).hexdigest().slice(0, 12);
}

py::str MSPascal::kit() const {
    const py::V& pk = raw_->profile.get("kit");
    if (!pk.is_str()) throw py::KeyError("'kit'");                 // (self.p["kit"] is read for the default of kit110 too)
    py::str which(pk.str());
    if (version == py::str("1.10")) {
        const py::V& k110 = raw_->profile.get("kit110");
        if (k110.is_str()) which = py::str(k110.str());
    }
    const py::str k = tcm::kit_dir(which);
    if (version == py::str("1.10")) {                              // (the pre-release kit: its files in zips)
        const py::str d = state_path({py::str("exact"), py::str("stage"), py::str("mspascal110")});
        if (!py::os::path::exists(py::os::path::join(d, py::str("PAS2.EXE")))) {
            py::os::makedirs(d, 0777, true);
            std::vector<py::str> zips;
            for (const py::str& z : py::os::listdir(k)) zips.push_back(z);
            std::sort(zips.begin(), zips.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            for (const py::str& z : zips) {
                if (z.lower().endswith(py::str(".zip"))) extractall(py::os::path::join(k, z), d);
            }
        }
        return d;
    }
    return py::os::path::join(k, version);
}

std::tuple<std::optional<py::bytes>, py::str> MSPascal::compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                                const py::list<py::str>& include,
                                                                const std::optional<py::str>& switches) const {
    return compile(src, std::optional<py::dict<py::str, py::bytes>>(headers), include, switches, true, py::str("T.PAS"));
}

std::tuple<std::optional<py::bytes>, py::str> MSPascal::compile(const py::bytes& src_in, const std::optional<py::dict<py::str, py::bytes>>& headers,
                                                                const py::list<py::str>& include, const std::optional<py::str>& switches,
                                                                bool use_cache, const py::str& name_) const {
    (void)headers;
    (void)include;
    (void)switches;
    const py::bytes src = crlf(src_in);
    std::string joined = src.raw();
    joined.push_back('\0');
    joined += name_.encode().raw();
    joined.push_back('\0');
    joined += key.utf8();
    const py::str k = py::hashlib::sha1(py::bytes(joined)).hexdigest();
    using Result = std::tuple<std::optional<py::bytes>, py::str>;
    if (use_cache) {
        const std::optional<Result> got = cache::value<Result>(py::str("exact"), k);
        if (got) return *got;
    }
    const Result res = _compile(src, name_);
    cache::keep<Result>(py::str("exact"), k, res);
    return res;
}

std::tuple<std::optional<py::bytes>, py::str> MSPascal::_compile(const py::bytes& src, const py::str& name_) const {
    const py::str k = kit();
    const py::str w = py::tempfile::mkdtemp(std::nullopt, py::str("pas_"));
    try {
        for (const char* fc : {"PAS1.EXE", "PAS2.EXE", "PASKEY"}) {
            const py::str f(fc);
            const py::str g = py::os::path::exists(py::os::path::join(k, f)) ? f : py::str("PAS1EM.EXE");   // (3.11: PAS1EM and PAS187)
            if (py::os::path::exists(py::os::path::join(k, g))) py::shutil::copy(py::os::path::join(k, g), py::os::path::join(w, f));
        }
        write_file(py::os::path::join(w, name_), src);
        const py::str stem = std::get<0>(py::os::path::splitext(name_));
        const emu::dos::Result r1 = emu::dos::run(py::os::path::join(w, py::str("PAS1.EXE")), w,
                                                  (name_ != py::str("T.PAS") ? stem : py::str("T")) + py::str(",T,T,T;"));
        py::str con = r1.console.decode("latin1");
        const py::list<py::str> after = py::split(con, py::str("Pass One"));
        if (!after[-1].contains(py::str("No Errors"))) {
            const py::str lst = py::os::path::join(w, py::str("T.LST"));
            const py::str errs = py::os::path::exists(lst) ? py::open_rb(lst).read().decode("latin1") : py::str("");
            py::list<py::str> bad;
            for (const py::str& l : py::split(errs.replace(py::str("\r"), py::str("")), py::str("\n"))) {
                if (l.contains(py::str("^")) || l.contains(py::str("Error"))) bad.append(l);
            }
            std::tuple<std::optional<py::bytes>, py::str> res{std::nullopt, con + py::str("\n") + py::join(py::str("\n"), bad).slice(0, 1500)};
            py::shutil::rmtree(w, true);
            return res;
        }
        const emu::dos::Result r2 = emu::dos::run(py::os::path::join(w, py::str("PAS2.EXE")), w, py::str(""));
        con = con + r2.console.decode("latin1");
        const py::str op = py::os::path::join(w, py::str("T.OBJ"));
        std::tuple<std::optional<py::bytes>, py::str> res;
        if (!py::os::path::exists(op)) {
            res = {std::nullopt, con};
        } else {
            const py::bytes obj = py::bytes(read_all(op));
            const py::str cp = py::os::path::join(w, py::str("T.COD"));
            py::str cod("");
            if (py::os::path::exists(cp))
                cod = py::open_rb(cp).read().decode("latin1").replace(py::str("\r"), py::str("")).replace(py::str("\x0c"), py::str(""));
            res = {_named(obj, cod, src.decode("latin1")), con};
        }
        py::shutil::rmtree(w, true);
        return res;
    } catch (...) {
        py::shutil::rmtree(w, true);
        throw;
    }
}

py::bytes MSPascal::_named(const py::bytes& obj, const py::str& cod, const py::str& text) const {
    const omf::Obj o = omf::read(obj);
    py::list<py::str> cs;
    for (const auto& sg : o.segs) {
        const py::str& s = std::get<0>(sg);
        if (s == py::str("HEAP") || s == py::str("STACK") || s == py::str("DATA") || s == py::str("CONST") || s == py::str("MEMORY")) continue;
        if (o.filled.get(s, 0) != 0) cs.append(s);
    }
    if (cs.empty()) return obj;
    const py::str seg = cs[0];
    const std::string code = o.seg.at(seg).slice(0, o.filled.at(seg)).raw();
    py::dict<py::str, int64_t> procs;
    py::str name_("");
    bool have_name = false;
    for (const py::str& line : py::split(cod, py::str("\n"))) {          // (the code listing: each procedure's first address)
        std::optional<py::re::Match> m = py::re::match(py::str("\\s*Procedure/Function : (\\S+)"), line);
        if (m) {
            name_ = m->group(1).upper();
            have_name = true;
            continue;
        }
        m = py::re::match(py::str("\\s*\\*\\*\\s*([0-9A-F]{6})\\s"), line);
        if (m && have_name && !name_.empty() && !procs.contains(name_)) procs.set(name_, py::int_parse(m->group(1), 16));
    }
    int64_t helper = -1;
    for (const py::bytes& h : HELPERS) {
        const size_t at = code.find(h.raw());
        if (at != std::string::npos) helper = std::max<int64_t>(helper, static_cast<int64_t>(at));
    }
    const bool has_helper = helper >= 0;
    if (procs.empty()) {                                  // (no listing: where the code calls the entry helper)
        const int64_t end = has_helper ? helper : static_cast<int64_t>(code.size());
        std::vector<int64_t> ss;
        if (has_helper) {
            for (int64_t a = 0; a < end; a++) {
                if (static_cast<unsigned char>(code[static_cast<size_t>(a)]) != 0xE8) continue;
                if (a + 3 <= end) {
                    const int16_t rel = static_cast<int16_t>(rd16(code, static_cast<size_t>(a) + 1));
                    if (a + 3 + rel == helper) ss.push_back(a);
                }
            }
        }
        const std::string inl = INLINE.raw();
        const std::string head = code.substr(0, static_cast<size_t>(end));
        for (size_t p = head.find(inl); p != std::string::npos; p = head.find(inl, p + inl.size())) ss.push_back(static_cast<int64_t>(p));
        std::sort(ss.begin(), ss.end());
        ss.erase(std::unique(ss.begin(), ss.end()), ss.end());
        const py::list<py::str> order = emit_order(text);
        const std::optional<py::re::Match> prog = py::re::search(py::str("(?im)^\\s*(?:PROGRAM|MODULE)\\s+([A-Z_0-9]+)"), text);
        py::list<py::str> names;
        for (const py::str& n : order) names.append(n);
        if (prog) names.append(prog->group(1).upper());
        if (static_cast<int64_t>(ss.size()) == names.size() - 1) names = names.slice(0, names.size() - 1);
        for (size_t i = 0; i < ss.size() && static_cast<int64_t>(i) < names.size(); i++) procs.set(names[static_cast<int64_t>(i)], ss[i]);
    }
    py::set<py::str> have;
    for (const auto& pb : o.pubs) have.add(std::get<0>(pb));
    py::dict<py::str, int64_t> kept;
    for (const auto& kv : procs.items()) {
        if (!have.contains(std::get<0>(kv))) kept.set(std::get<0>(kv), std::get<1>(kv));
    }
    if (has_helper) kept.set(py::str("(helper)"), helper);
    return !kept.empty() ? publish(obj, kept, seg) : obj;
}

void register_plugin() {
    tcm::register_plugin(py::str("codebreaker.exact.mspascal:MSPascal"),
                         [](const toolchain::Profile& p, const std::optional<py::str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                             return std::make_shared<MSPascal>(p, variant);
                         });
}

}  // namespace codebreaker::exact::mspascal
