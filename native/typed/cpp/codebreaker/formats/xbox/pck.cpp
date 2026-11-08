// codebreaker/formats/xbox/pck.cpp -- C++ port of codebreaker.formats.xbox.pck (see the header).

#include "codebreaker/formats/xbox/pck.hpp"

namespace codebreaker::formats::xbox::pck {

const py::str DESCRIPTION = "unpack Minecraft LCE .pck containers (PNG/loc/grf/col payloads)";

// struct.unpack_from(">I" * n, d, off) as Python words it when `d` is too short (the CLI prints the text)
static void need_(const py::bytes& d, int64_t off, int64_t size) {
    if (py::len(d) - off < size) {
        throw py::StructError(py::percent(
            py::str("unpack_from requires a buffer of at least %d bytes for unpacking %d bytes at offset %d (actual buffer size is %d)"),
            off + size, size, off, py::len(d)));
    }
}

// the big-endian uint32 at `off`
static int64_t u32_(const py::bytes& d, int64_t off) {
    need_(d, off, 4);
    return py::unpack_from1<int64_t>(">I", d, off);
}

// the pair of big-endian uint32 at `off`: struct.unpack_from('>II', d, off)
static std::tuple<int64_t, int64_t> u32x2_(const py::bytes& d, int64_t off) {
    need_(d, off, 8);
    return py::unpack_from<int64_t, int64_t>(">II", d, off);
}

// bytes.decode('utf-16-be', 'replace') -- CPython's decoder: a lone surrogate, or a high one not followed by a
// low one, is one U+FFFD; an odd trailing byte (alone or after a lone surrogate) is one U+FFFD
static py::str decode_utf16be_(const py::bytes& raw) {
    std::u32string out;
    int64_t n = py::len(raw);
    int64_t q = 0;
    while (q < n) {
        if (n - q < 2) {
            out.push_back(0xFFFD);                             // truncated data
            break;
        }
        int64_t ch = raw.byte_(q) * 256 + raw.byte_(q + 1);
        q += 2;
        if (ch < 0xD800 || ch > 0xDFFF) {
            out.push_back(static_cast<char32_t>(ch));
            continue;
        }
        if (n - q < 2) {
            out.push_back(0xFFFD);                             // unexpected end of data
            break;
        }
        if (ch <= 0xDBFF) {
            int64_t ch2 = raw.byte_(q) * 256 + raw.byte_(q + 1);
            if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
                q += 2;
                out.push_back(static_cast<char32_t>(0x10000 + ((ch - 0xD800) << 10) + (ch2 - 0xDC00)));
            } else {
                out.push_back(0xFFFD);                         // illegal UTF-16 surrogate (ch2 is read again)
            }
        } else {
            out.push_back(0xFFFD);                             // illegal encoding
        }
    }
    return py::str(out);
}

std::tuple<int64_t, py::str, int64_t> _rec(const py::bytes& d, int64_t p) {
    auto [a, nl] = u32x2_(d, p);
    p += 8;
    if (nl > 8192 || p + nl * 2 > py::len(d)) {
        throw py::ValueError(py::percent(py::str("bad name length %d at 0x%X"), nl, p));
    }
    py::str name = decode_utf16be_(d.slice(p, p + nl * 2));
    p += nl * 2 + 4;
    return {a, name, p};
}

Parsed parse(const py::str& path) {
    py::bytes d = py::open_rb(path).read();
    auto [ver, nprop] = u32x2_(d, 0);
    int64_t p = 8;
    py::list<std::tuple<int64_t, py::str>> props;
    for (int64_t i = 0; i < nprop; ++i) {
        auto [pid, name, next] = _rec(d, p);
        p = next;
        props.append({pid, name});
    }
    int64_t ncount = u32_(d, p);
    p += 4;
    py::list<Entry> entries;
    for (int64_t i = 0; i < ncount; ++i) {
        auto [size, typ] = u32x2_(d, p);
        int64_t nl = u32_(d, p + 8);
        int64_t q = p + 12;
        py::str name = decode_utf16be_(d.slice(q, q + nl * 2));
        p = q + nl * 2 + 4;
        entries.append(Entry{size, typ, name});
    }
    Parsed r;
    r.version = ver;
    r.props = props;
    r.entries = entries;
    r.table_end = p;
    r.data = d;
    return r;
}

std::tuple<py::list<std::tuple<py::str, py::bytes>>, int64_t, int64_t> payloads(const py::str& path) {
    Parsed r = parse(path);
    const py::bytes& d = r.data;
    int64_t p = r.table_end + 12;                              // 12-byte gap before first payload
    const py::bytes PNG = py::bytes::fromhex(py::str("89504e470d0a1a0a"));
    py::list<std::tuple<py::str, py::bytes>> out;
    for (const Entry& e : r.entries) {
        if (!e.size) {
            continue;
        }
        int64_t at;
        if (e.name.lower().endswith(py::str(".png"))) {
            at = d.find(PNG, p);                               // exact: PNG is self-delimiting
            if (at < 0) {
                continue;
            }
        } else {
            at = p;
            while (at < py::len(d) && d[at] == 0 && at - p < 512 && p != r.table_end + 12) {
                at += 1;
            }
        }
        py::bytes blob = d.slice(at, at + e.size);
        p = at + e.size;
        py::str safe = e.name.replace(py::str("/"), py::str("_")).replace(py::chr(92), py::str("_")).strip(py::chr(0));
        out.append({py::truthy(safe) ? safe : py::percent(py::str("entry%d"), py::len(out)), blob});
    }
    return {out, p, py::len(d)};
}

std::tuple<int64_t, int64_t, int64_t> extract(const py::str& path, const py::str& outdir) {
    auto [found, end, size] = payloads(path);
    py::os::makedirs(outdir, 0777, true);
    for (const auto& [name, blob] : found) {
        py::open(py::os::path::join(outdir, name), py::str("wb")).write(blob);
    }
    return {py::len(found), end, size};
}

void main(const py::list<py::str>& sys_argv) {
    py::list<py::str> argv = sys_argv.slice_from(1);
    py::list<py::str> args;
    bool doext = false;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("-x"))) {
            args.append(a);
        }
        if (a.startswith(py::str("-x"))) {
            doext = true;
        }
    }
    for (const py::str& path : args) {
        Parsed r;
        try {
            r = parse(path);
        } catch (const py::Exception& ex) {
            py::print(py::percent(py::str("%s: PARSE FAILED (%s)"), py::os::path::basename(path), py::str(std::string(ex.what()))));
            continue;
        }
        int64_t tot = 0;
        for (const Entry& e : r.entries) {
            tot += e.size;
        }
        py::print(py::percent(py::str("%s"), py::os::path::basename(path)));
        py::print(py::percent(py::str("  version=%d  properties=%s  entries=%d"), r.version,
                              py::truthy(r.props) ? py::repr(r.props) : py::str("-"), py::len(r.entries)));
        py::print(py::percent(py::str("  table_end=0x%X  sum(sizes)=%d  table+payload=%d  file=%d  delta=%d"), r.table_end, tot,
                              r.table_end + tot, py::len(r.data), py::len(r.data) - (r.table_end + tot)));
        py::dict<int64_t, int64_t> byt;
        for (const Entry& e : r.entries) {
            byt[e.type] = byt.get(e.type, 0) + 1;
        }
        py::list<py::str> hist;
        for (const auto& [k, v] : py::sorted(byt.items())) {
            hist.append(py::percent(py::str("t%d x%d"), k, v));
        }
        py::print(py::percent(py::str("  type histogram: %s"), py::join(py::str(", "), hist)));
        for (const Entry& e : r.entries.slice(0, 10)) {
            py::print(py::percent(py::str("     %-9d t%-3d %s"), e.size, e.type, e.name));
        }
        if (py::len(r.entries) > 10) {
            py::print(py::percent(py::str("     ... %d more"), py::len(r.entries) - 10));
        }
        if (doext) {
            py::str outdir = std::get<0>(py::os::path::splitext(path)) + py::str("_extracted");
            auto [n, end, sz] = extract(path, outdir);
            py::print(py::percent(py::str("  extracted %d files -> %s  (consumed %d of %d)"), n, outdir, end, sz));
        }
        py::print();
    }
}

}  // namespace codebreaker::formats::xbox::pck
