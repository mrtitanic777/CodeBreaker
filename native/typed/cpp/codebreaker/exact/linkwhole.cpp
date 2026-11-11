// codebreaker/exact/linkwhole.cpp -- typed C++ port of codebreaker.exact.linkwhole (see linkwhole.hpp).

#include "codebreaker/exact/linkwhole.hpp"

#include <algorithm>
#include <cstdio>
#include <ostream>
#include <vector>

#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::linkwhole {

namespace tcm = codebreaker::exact::toolchain;
using py::str;

namespace {

str hex_of(int64_t v, int width) {                                  // "%0*x" % (width, v)
    const bool neg = v < 0;
    char buf[40];
    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(neg ? -v : v));
    std::string s = buf;
    const int w = neg ? width - 1 : width;
    while (static_cast<int>(s.size()) < w) s.insert(s.begin(), '0');
    return str((neg ? "-" : "") + s);
}

int64_t num_of(const Num& n) {                                       // int(x, 0) if it is text
    if (std::holds_alternative<int64_t>(n)) return std::get<int64_t>(n);
    return py::int_parse(std::get<str>(n), 0);
}

std::string read_text(const str& path) {
    py::file f = py::open_rb(path);
    const py::bytes b = f.read();
    f.close();
    return b.raw();
}

void write_file(const str& path, const py::bytes& data) {
    py::file f = py::file::open(path, str("wb"));
    f.write(data);
    f.close();
}

// the text of a file, as open(path).read() gives it (lines end in \n)
str file_text(const str& path) { return py::bytes(read_text(path)).decode("utf-8"); }

// text.splitlines-like: the lines of a file as `for l in open(path)` gives them (each with its newline, if it had one)
std::vector<str> file_lines(const str& path) {
    std::string t = read_text(path);
    std::string norm;
    for (size_t i = 0; i < t.size(); i++) {                          // (universal newlines)
        if (t[i] == '\r') {
            norm.push_back('\n');
            if (i + 1 < t.size() && t[i + 1] == '\n') i++;
        } else {
            norm.push_back(t[i]);
        }
    }
    std::vector<str> out;
    size_t at = 0;
    while (at < norm.size()) {
        size_t nl = norm.find('\n', at);
        if (nl == std::string::npos) nl = norm.size() - 1;
        out.push_back(py::bytes(norm.substr(at, nl + 1 - at)).decode("utf-8"));
        at = nl + 1;
    }
    return out;
}

uint8_t byte_at(const std::string& b, int64_t i, const char* what = "index out of range") {
    if (i < 0 || i >= static_cast<int64_t>(b.size())) throw py::IndexError(what);
    return static_cast<uint8_t>(b[static_cast<size_t>(i)]);
}

bool truthy(const std::optional<str>& s) { return s.has_value() && !s->empty(); }

}  // namespace

LinkConfig link_config(const str& json_text) {
    const py::V v = py::json_parse(json_text.utf8());
    LinkConfig c;
    if (!v.is_dict()) return c;
    if (v.has("linker")) {
        c.has_linker = true;
        if (v.get("linker").is_str()) c.linker = str(v.get("linker").str());
    }
    if (v.has("emulate")) {
        c.has_emulate = true;
        if (v.get("emulate").is_str()) c.emulate = str(v.get("emulate").str());
    }
    if (v.get("order").is_str()) c.order = str(v.get("order").str());
    if (v.get("switches").is_str()) c.switches = str(v.get("switches").str());
    if (v.get("output").is_str()) c.output = str(v.get("output").str());
    if (v.has("map")) c.map = py::truthy(v.get("map"));
    if (v.get("names").is_str()) c.names = str(v.get("names").str());
    if (v.get("placed").is_str()) c.placed = str(v.get("placed").str());
    if (v.get("last").is_list())
        for (const py::V& x : v.get("last").seq()) c.last.append(str(x.str()));
    if (v.get("find").is_list()) {
        for (const py::V& x : v.get("find").seq()) {
            Find f;
            f.file = str(x.at("file").str());
            f.toolchain = str(x.at("toolchain").str());
            if (x.get("variant").is_str()) f.variant = str(x.get("variant").str());
            c.find.append(f);
        }
    }
    if (v.get("first").is_list())
        for (const py::V& x : v.get("first").seq()) c.first.append({str(x[0].str()), str(x[1].str())});
    if (v.get("libraries").is_list())
        for (const py::V& x : v.get("libraries").seq()) c.libraries.append(str(x.str()));
    if (v.get("class").is_dict()) {
        for (const auto& kv : v.get("class").items()) {
            ClassSpec s;
            if (kv.second.get("segments").is_list())
                for (const py::V& x : kv.second.get("segments").seq()) s.segments.append(str(x.str()));
            if (kv.second.get("files").is_list())
                for (const py::V& x : kv.second.get("files").seq()) s.files.append(str(x.str()));
            c.class_.set(str(kv.first.str()), s);
        }
    }
    auto num = [](const py::V& x) -> Num { return x.is_str() ? Num(str(x.str())) : Num(x.as_int()); };
    if (v.get("accept").is_list()) {
        for (const py::V& x : v.get("accept").seq()) {
            Accept a;
            if (x.has("tail_zeros") && !x.get("tail_zeros").is_none()) a.tail_zeros = num(x.get("tail_zeros"));
            if (x.has("header")) a.header = std::make_tuple(num(x.get("header")[0]), num(x.get("header")[1]));
            if (x.get("why").is_str()) a.why = str(x.get("why").str());
            c.accept.append(a);
        }
    }
    if (v.has("psp") && !v.get("psp").is_none()) c.psp = num(v.get("psp"));
    return c;
}

bool wanted(const Project& prj) {
    const LinkConfig lk = prj.link();
    return lk.has_linker || lk.has_emulate;
}

py::list<std::tuple<int64_t, str, py::bytes, bool>> listed(const Project& prj, const LinkConfig& lk) {
    py::list<std::tuple<int64_t, str, py::bytes, bool>> out;
    int64_t k = 0;
    for (const str& l : file_lines(py::os::path::join(prj.dir(), lk.order))) {
        if (py::split(l).empty() || l.startswith(str("#"))) continue;
        const py::list<str> f = py::split(py::split(l, str("#"))[0]);
        std::shared_ptr<const Project> part;
        for (const auto& p : prj.sets()) {
            if (py::os::path::exists(py::os::path::join(p->src(), f[0]))) {
                part = p;
                break;
            }
        }
        if (!part) throw py::SystemExit(f[0] + str(": no source set has it"));
        const auto [raw, con] = part->toolchain()->compile(py::bytes(read_text(py::os::path::join(part->src(), f[0]))), part->headers(),
                                                          part->include(), part->switches());
        if (!raw) {
            const int64_t n = con.size();
            throw py::SystemExit(f[0] + str(" does not compile:\n") + con.slice(std::max<int64_t>(n - 1500, 0), n));
        }
        bool odd = false;
        for (int64_t i = 1; i < f.size(); i++)
            if (f[i] == str("odd")) odd = true;
        out.append({k, f[0], *raw, odd});
        k++;
    }
    return out;
}

py::list<std::tuple<int64_t, str, py::bytes>> modules(const Project& prj) {
    const LinkConfig lk = prj.link();
    struct Entry {
        int64_t at;
        str path;
        std::shared_ptr<const Project> part;               // null: compiled already
        std::shared_ptr<modules::Toolchain> tc;
        std::optional<py::bytes> raw;
    };
    std::vector<Entry> out;
    const auto sets = prj.sets();
    for (const auto& part : sets) {
        const auto tc = part->toolchain();
        for (const auto& s : part->sources()) out.push_back({std::get<0>(s), std::get<1>(s), part, tc, std::nullopt});
    }
    const auto main = sets[0];
    if (lk.placed) {
        std::vector<str> names;
        for (const str& f : py::os::listdir(main->src())) names.push_back(f);
        std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        for (const str& f : names) {
            const auto m = py::re::fullmatch(*lk.placed, f, py::re::I);
            if (m) out.push_back({py::int_parse(m->group(1), 16), py::os::path::join(main->src(), f), main, main->toolchain(), std::nullopt});
        }
    }
    for (const Find& x : lk.find) {
        const auto tc = tcm::load(x.toolchain, x.variant);
        const str path = py::os::path::join(main->src(), x.file);
        const auto [raw, con] = tc->compile(py::bytes(read_text(path)), main->headers(), main->include(), std::nullopt);
        (void)con;
        const std::optional<int64_t> at = raw ? _found(prj, *raw) : std::nullopt;
        if (!at) throw py::SystemExit(x.file + str(": ") + (!raw ? str("does not compile") : str("its code is not found in the program")));
        out.push_back({*at, path, nullptr, nullptr, raw});
    }
    std::stable_sort(out.begin(), out.end(), [](const Entry& a, const Entry& b) { return a.at < b.at; });
    int64_t k = 0;
    for (const str& f : lk.last) {
        out.push_back({0x100000 + k, py::os::path::join(main->src(), f), main, main->toolchain(), std::nullopt});
        k++;
    }
    py::list<std::tuple<int64_t, str, py::bytes>> objs;
    for (const Entry& e : out) {
        if (!e.part) {                                     // (compiled already: placed by its bytes)
            objs.append({e.at, py::os::path::basename(e.path), *e.raw});
            continue;
        }
        const auto [raw, con] = e.tc->compile(py::bytes(read_text(e.path)), e.part->headers(), e.part->include(), e.part->switches());
        if (!raw) {
            const int64_t n = con.size();
            throw py::SystemExit(py::os::path::basename(e.path) + str(" does not compile:\n") + con.slice(std::max<int64_t>(n - 1500, 0), n));
        }
        objs.append({e.at, py::os::path::basename(e.path), *raw});
    }
    return objs;
}

std::optional<int64_t> _found(const Project& prj, const py::bytes& raw) {
    const omf::Obj o = omf::read(raw);
    py::list<str> cs = omf::code_segments(o);
    if (cs.empty()) {
        for (const auto& sg : o.segs)
            if (o.filled.get(std::get<0>(sg), 0) != 0) cs.append(std::get<0>(sg));
    }
    const std::string code = o.seg.at(cs[0]).slice(0, o.filled.get(cs[0], 0)).raw();
    const py::set<int64_t> hole = omf::holes(o, cs[0]);
    const int64_t len = static_cast<int64_t>(code.size());
    std::tuple<int64_t, int64_t> best{0, 0};
    int64_t r = 0;
    while (r < len) {                                       // the longest run with no hole, to look for
        int64_t e = r;
        while (e < len && !hole.contains(e)) e++;
        const std::tuple<int64_t, int64_t> cand{e - r, r};
        if (cand > best) best = cand;
        r = e + 1;
    }
    const int64_t n = std::get<0>(best);
    r = std::get<1>(best);
    if (n < 8) return std::nullopt;
    const std::string img = prj.image().raw();
    std::vector<int64_t> hits;
    const std::string needle = code.substr(static_cast<size_t>(r), static_cast<size_t>(n));
    size_t k = img.find(needle);
    while (k != std::string::npos) {
        const int64_t a = static_cast<int64_t>(k) - r;
        bool ok = a >= 0 && a + len <= static_cast<int64_t>(img.size());
        if (ok) {
            for (int64_t x = 0; x < len; x++) {
                if (hole.contains(x)) continue;
                if (img[static_cast<size_t>(a + x)] != code[static_cast<size_t>(x)]) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) hits.push_back(a);
        k = img.find(needle, k + 1);
    }
    return hits.size() == 1 ? std::optional<int64_t>(hits[0]) : std::nullopt;
}

py::bytes _own(const py::bytes& bb) {
    const std::string& b = bb.raw();
    int64_t p = 0;
    const int64_t n = static_cast<int64_t>(b.size());
    while (p + 3 <= n) {
        const int64_t t = byte_at(b, p);
        p += 3 + (byte_at(b, p + 1) | (byte_at(b, p + 2) << 8));
        if (t == 0x8A || t == 0x8B) return py::bytes(b.substr(0, static_cast<size_t>(p)));
    }
    return bb;
}

str _file(const Project& prj, const str& spec) { return tcm::kit_path(std::nullopt, spec, prj.dir()); }

str _name(const std::optional<str>& scheme, int64_t k, const str& f) {
    const str s = truthy(scheme) ? *scheme : str("sequence:M");
    const int64_t colon = s.find(str(":"));
    const str kind = colon >= 0 ? s.slice(0, colon) : s;
    const str pre = colon >= 0 ? s.slice_from(colon + 1) : str("");
    if (kind == str("files")) {
        const str stem = std::get<0>(py::os::path::splitext(f)).upper();
        return py::re::sub(str("[^A-Z0-9]"), str(""), stem).slice(0, 8);
    }
    if (kind == str("pairs")) {
        const str p = pre.empty() ? str("D") : pre;
        if (p.size() != 1) throw py::TypeError("ord() expected a character, but string of length " + std::to_string(p.size()) + " found");
        return str(static_cast<char32_t>(p.u32()[0] + k / 26)) + str(static_cast<char32_t>(65 + k % 26));
    }
    str tail;
    if (k < 26) tail = str(static_cast<char32_t>(65 + k));
    else if (k < 36) tail = str(static_cast<char32_t>(48 + k - 26));
    else tail = (k < 10 ? str("0") : str("")) + str(std::to_string(k));
    return (pre.empty() ? str("M") : pre) + tail;
}

std::tuple<int64_t, int64_t> _index(const py::bytes& bb, int64_t q) {
    const std::string& b = bb.raw();
    auto at = [&b](int64_t i) {                              // b[i], a negative i counting from the end
        if (i < 0) i += static_cast<int64_t>(b.size());
        return byte_at(b, i);
    };
    if (at(q) & 0x80) return {((at(q) & 0x7F) << 8) | at(q + 1), q + 2};
    return {at(q), q + 1};
}

py::bytes reclass(const py::bytes& objb, const str& cls, const py::list<str>& segments) {
    const std::string& obj = objb.raw();
    std::string out;
    std::vector<str> names;
    int64_t added = -1;                                     // None
    int64_t i = 0;
    py::set<str> want;
    for (const str& s : segments) want.add(s.upper());
    const int64_t total = static_cast<int64_t>(obj.size());
    while (i + 3 <= total) {
        const int64_t t = byte_at(obj, i);
        const int64_t n = byte_at(obj, i + 1) | (byte_at(obj, i + 2) << 8);
        std::string body = obj.substr(static_cast<size_t>(i + 3), static_cast<size_t>(std::max<int64_t>(n - 1, 0)));
        i += 3 + n;
        if (t == 0x96) {                                    // LNAMES: the class's name added to the first
            int64_t p = 0;
            while (p < static_cast<int64_t>(body.size())) {
                const int64_t len = byte_at(body, p);
                names.push_back(py::bytes(body.substr(static_cast<size_t>(p + 1), static_cast<size_t>(len))).decode("latin1"));
                p += 1 + len;
            }
            if (added < 0) {
                if (cls.size() > 255) throw py::ValueError("bytes must be in range(0, 256)");
                body.push_back(static_cast<char>(cls.size()));
                body += cls.encode().raw();
                names.push_back(cls);
                added = static_cast<int64_t>(names.size());
            }
        } else if (t == 0x98 && added >= 0) {               // SEGDEF: ACBP [frame offset] length name class overlay
            const int64_t p = 1 + (((byte_at(body, 0) >> 5) == 0) ? 3 : 0) + 2;
            const auto [nm, q] = _index(py::bytes(body), p);
            const str nmq = (0 < nm && nm <= static_cast<int64_t>(names.size())) ? names[static_cast<size_t>(nm - 1)].upper() : str("");
            bool hit = want.contains(nmq);
            if (!hit) {
                for (const str& s : want)
                    if (nmq.endswith(str("_") + s)) hit = true;
            }
            if (hit) {
                if ((byte_at(body, q) & 0x80) || added > 0x7F) throw py::SystemExit(str("reclass: a two-byte name index is not handled"));
                body[static_cast<size_t>(q)] = static_cast<char>(added);
            }
        }
        std::string r;
        r.push_back(static_cast<char>(t));
        const int64_t rl = static_cast<int64_t>(body.size()) + 1;
        if (rl > 0xFFFF) throw py::OverflowError("int too big to convert");
        r.push_back(static_cast<char>(rl & 0xFF));
        r.push_back(static_cast<char>(rl >> 8));
        r += body;
        int64_t sum = 0;
        for (const char c : r) sum += static_cast<uint8_t>(c);
        out += r;
        out.push_back(static_cast<char>((-sum) & 0xFF));
    }
    return py::bytes(out);
}

std::tuple<std::optional<py::bytes>, str, str> run(const Project& prj, const py::list<std::tuple<int64_t, str, py::bytes>>& objs,
                                                   const std::optional<str>& keep) {
    const LinkConfig lk = prj.link();
    const str w = truthy(keep) ? *keep : py::tempfile::mkdtemp(std::nullopt, str("exlinkw_"));
    py::os::makedirs(w, 0777, true);
    try {
        if (!lk.linker) throw py::KeyError("'linker'");
        py::shutil::copy(_file(prj, *lk.linker), py::os::path::join(w, str("LINK.EXE")));
        py::list<str> names;
        for (const auto& nf : lk.first) {
            py::shutil::copy(_file(prj, std::get<1>(nf)), py::os::path::join(w, std::get<0>(nf) + str(".OBJ")));
            names.append(std::get<0>(nf));
        }
        py::dict<str, std::tuple<str, py::list<str>>> cls;
        for (const auto& kv : lk.class_.items())
            for (const str& f : std::get<1>(kv).files) cls.set(f, {std::get<0>(kv), std::get<1>(kv).segments});
        int64_t k = 0;
        for (const auto& o : objs) {
            const str& f = std::get<1>(o);
            py::bytes raw = _own(std::get<2>(o));
            if (cls.contains(f)) raw = reclass(raw, std::get<0>(cls.at(f)), std::get<1>(cls.at(f)));
            const str n = _name(lk.names, k, f);
            names.append(n);
            write_file(py::os::path::join(w, n + str(".OBJ")), raw);
            k++;
        }
        py::list<str> libs;
        for (const str& f : lk.libraries) {
            py::shutil::copy(_file(prj, f), w);
            libs.append(std::get<0>(py::os::path::splitext(py::os::path::basename(f))).upper());
        }
        const str out = lk.output;
        str cmd = py::join(str("+"), names) + str(",") + out + str(",") + (lk.map ? out : str("NUL")) +
                  (!libs.empty() ? str(",") + py::join(str("+"), libs) : str("")) + lk.switches + str(";");
        std::optional<int64_t> psp;
        if (lk.psp) psp = num_of(*lk.psp);
        emu::dos::Result r;
        if (cmd.size() > 120) {                             // (a DOS command line holds 127 characters: the answers in
            str body = cmd;                                 // a response file, one a line)
            while (!body.empty() && body.u32().back() == U';') body = body.slice(0, body.size() - 1);
            write_file(py::os::path::join(w, str("L.LNK")), (py::join(str("\r\n"), py::split(body, str(","))) + str(";\r\n")).encode());
            r = emu::dos::run(py::os::path::join(w, str("LINK.EXE")), w, str("@L.LNK"), py::bytes(std::string()), emu::dos::DEFAULT_MAX_BLOCKS,
                              false, py::bytes(std::string("PATH=\0", 6)), psp);
        } else {
            r = emu::dos::run(py::os::path::join(w, str("LINK.EXE")), w, cmd, py::bytes(std::string()), emu::dos::DEFAULT_MAX_BLOCKS, false,
                              py::bytes(std::string("PATH=\0", 6)), psp);
        }
        const str con = r.console.decode("latin1").replace(str("\r"), str(""));
        const str p = py::os::path::join(w, out + str(".EXE"));
        std::optional<py::bytes> exe;
        if (py::os::path::exists(p)) exe = py::bytes(read_text(p));
        const str mp = py::os::path::join(w, out + str(".MAP"));
        const str map_text = py::os::path::exists(mp) ? py::bytes(read_text(mp)).decode("latin1") : str("");
        if (!truthy(keep)) py::shutil::rmtree(w, true);
        return {exe, con, map_text};
    } catch (...) {
        if (!truthy(keep)) py::shutil::rmtree(w, true);
        throw;
    }
}

std::tuple<bool, py::list<str>> compare(const Project& prj, const py::bytes& exe_b) {
    const std::string ref = prj.file().raw();
    const std::string& exe = exe_b.raw();
    py::list<str> said;
    if (exe == ref) return {true, said};
    const LinkConfig lk = prj.link();
    py::set<int64_t> skip;
    int64_t n = static_cast<int64_t>(exe.size());
    const int64_t le = static_cast<int64_t>(exe.size()), lr = static_cast<int64_t>(ref.size());
    for (const Accept& a : lk.accept) {
        int64_t tz = 0;
        bool have_tz = false;                               // (tz is truthy)
        if (a.tail_zeros) {
            if (std::holds_alternative<str>(*a.tail_zeros)) {
                const str& s = std::get<str>(*a.tail_zeros);
                if (s == str("record")) {
                    tz = ((-le) % 128 + 128) % 128;
                    have_tz = tz != 0;
                } else if (!s.empty()) {
                    throw py::TypeError("unsupported operand type(s) for +: 'int' and 'str'");
                }
            } else {
                tz = std::get<int64_t>(*a.tail_zeros);
                have_tz = tz != 0;
            }
        }
        if (have_tz && lr == le + tz) {
            bool zeros = true;
            for (int64_t i = le; i < lr; i++)
                if (ref[static_cast<size_t>(i)] != 0) zeros = false;
            if (zeros) {
                n = lr;
                said.append(str("the original has ") + str(std::to_string(tz)) + str(" zero bytes more at its end (") + a.why + str(")"));
            }
        }
        if (a.header) {
            const int64_t lo = num_of(std::get<0>(*a.header)), hi = num_of(std::get<1>(*a.header));
            bool d = false;
            for (int64_t k = lo; k < hi; k++)
                if (k < std::min(lr, le) && ref[static_cast<size_t>(k)] != exe[static_cast<size_t>(k)]) {
                    d = true;
                    break;
                }
            if (d) {
                for (int64_t k = lo; k < hi; k++) skip.add(k);
                skip.add(0x12);
                skip.add(0x13);
                said.append(str("header bytes ") + hex_of(lo, 2) + str("..") + hex_of(hi - 1, 2) + str(" and the checksum (") + a.why + str(")"));
            }
        }
    }
    if (n != lr) {
        py::list<str> r;
        r.append(str("the file is ") + str(std::to_string(le)) + str(" bytes, the original ") + str(std::to_string(lr)));
        return {false, r};
    }
    std::vector<int64_t> bad;
    for (int64_t k = 0; k < std::min(le, lr); k++)
        if (!skip.contains(k) && exe[static_cast<size_t>(k)] != ref[static_cast<size_t>(k)]) bad.push_back(k);
    if (!bad.empty()) {
        if (lr < 10) throw py::StructError("unpack_from requires a buffer of at least 10 bytes");
        const int64_t h = (static_cast<uint8_t>(ref[8]) | (static_cast<uint8_t>(ref[9]) << 8)) * 16;
        py::list<str> r;
        r.append(str(std::to_string(bad.size())) + str(" bytes differ, the first at file offset ") + hex_of(bad[0], 5) + str(" (") +
                 (bad[0] < h ? str("the header") : str("image ") + hex_of(bad[0] - h, 5)) + str(")"));
        return {false, r};
    }
    return {true, said};
}

int64_t main(const Project& prj, const py::list<str>& argv, std::ostream& out) {
    auto print = [&out](const str& s) { out << s.utf8() << "\n"; };
    auto has = [&argv](const char* flag) {
        for (const str& a : argv)
            if (a == str(flag)) return true;
        return false;
    };
    std::optional<str> keep;
    if (has("--keep")) {
        int64_t at = 0;
        while (argv[at] != str("--keep")) at++;
        keep = argv[at + 1];
    }
    const LinkConfig lk = prj.link();
    if (!lk.emulate.empty()) {                              // (a linker done here: exact/linkemu.py)
        py::list<std::tuple<int64_t, str, py::bytes, bool>> objs;
        if (!lk.order.empty()) {
            objs = listed(prj, lk);
        } else {
            int64_t k = 0;
            for (const auto& m : modules(prj)) objs.append({k++, std::get<1>(m), std::get<2>(m), false});
        }
        if (lk.emulate != str("link24"))
            throw py::AttributeError("module 'codebreaker.exact.linkemu' has no attribute '" + lk.emulate.utf8() + "'");
        py::list<linkemu::Object> objects;
        for (const auto& o : objs) objects.append({std::get<1>(o), std::get<2>(o), std::get<3>(o)});
        std::function<void(const str&)> log;
        if (has("-v")) log = print;
        const py::bytes exe = linkemu::link24(objects, prj.symbols(), log);
        const auto [ok, lines] = compare(prj, exe);
        for (const str& l : lines) print(str("   ") + l);
        const bool same = exe == prj.file();
        print(py::os::path::basename(prj.exe()) + str(" linked from ") + str(std::to_string(objs.size())) + str(" modules (") + lk.emulate +
              str(", done here): ") + str(std::to_string(exe.size())) + str(" bytes; ") +
              (ok ? (same ? str("the FILE IDENTICAL") : str("identical but for what the project accepts")) : str("not identical")));
        return ok ? 0 : 1;
    }
    const auto objs = modules(prj);
    if (has("-v")) {
        for (const auto& o : objs) {
            const int64_t at = std::get<0>(o);
            print(str("   ") + hex_of(at < 0x100000 ? at : 0, 5) + str(" ") + std::get<1>(o));
        }
    }
    const auto [exe, con, mp] = run(prj, objs, keep);
    (void)mp;
    py::list<str> msgs;
    for (const str& l : py::split(con, str("\n")))
        if (!py::split(l).empty() && !l.contains(str("Microsoft")) && !l.contains(str("Copyright")) && !l.contains(str("Version"))) msgs.append(l);
    for (int64_t i = 0; i < std::min<int64_t>(msgs.size(), 12); i++) print(str("   LINK: ") + msgs[i]);
    if (!exe) {
        print(str("no program was written"));
        return 1;
    }
    const auto [ok, lines] = compare(prj, *exe);
    for (const str& l : lines) print(str("   ") + l);
    const bool same = *exe == prj.file();
    print(py::os::path::basename(prj.exe()) + str(" linked from ") + str(std::to_string(objs.size())) + str(" modules: ") +
          str(std::to_string(exe->size())) + str(" bytes; ") +
          (ok ? (same ? str("the FILE IDENTICAL") : str("identical but for what the project accepts")) : str("not identical")));
    return ok ? 0 : 1;
}

}  // namespace codebreaker::exact::linkwhole
