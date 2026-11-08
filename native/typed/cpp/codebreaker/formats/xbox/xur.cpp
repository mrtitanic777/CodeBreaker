// codebreaker/formats/xbox/xur.cpp -- typed C++ from codebreaker.formats.xbox.xur.

#include "codebreaker/formats/xbox/xur.hpp"

#include "py_os.hpp"

namespace codebreaker::formats::xbox::xur {

const py::str DESCRIPTION("read/edit Xbox 360 XUR (compiled XUI) scenes: info/strings + set (repack)");

// (the runtime has no utf-16 codec: bytes.decode("utf-16-be", "replace") and str.encode("utf-16-be"), as Python's)
static py::str utf16be_decode_replace(const py::bytes& b) {
    std::u32string out;
    int64_t n = b.size(), i = 0;
    auto unit = [&](int64_t at) { return (b.byte_(at) << 8) | b.byte_(at + 1); };
    while (i < n) {
        if (n - i < 2) {   // a byte left over: "truncated data"
            out.push_back(0xFFFD);
            break;
        }
        int64_t ch = unit(i);
        i += 2;
        if (ch < 0xD800 || ch > 0xDFFF) {
            out.push_back(static_cast<char32_t>(ch));
        } else if (ch <= 0xDBFF) {   // a high surrogate: needs its low one
            if (n - i < 2) {         // "unexpected end of data"
                out.push_back(0xFFFD);
                break;
            }
            int64_t ch2 = unit(i);
            if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
                out.push_back(static_cast<char32_t>(0x10000 + ((ch - 0xD800) << 10) + (ch2 - 0xDC00)));
                i += 2;
            } else {
                out.push_back(0xFFFD);   // "illegal UTF-16 surrogate"
            }
        } else {
            out.push_back(0xFFFD);   // a lone low surrogate: "illegal encoding"
        }
    }
    return py::str(out);
}

static py::bytes utf16be_encode(const py::str& s) {
    std::string out;
    for (char32_t c : s.u32()) {
        if (c >= 0xD800 && c <= 0xDFFF)
            throw py::UnicodeEncodeError("'utf-16-be' codec can't encode character: surrogates not allowed");
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            char32_t hi = 0xD800 + (v >> 10), lo = 0xDC00 + (v & 0x3FF);
            out.push_back(static_cast<char>(hi >> 8));
            out.push_back(static_cast<char>(hi & 0xFF));
            out.push_back(static_cast<char>(lo >> 8));
            out.push_back(static_cast<char>(lo & 0xFF));
        } else {
            out.push_back(static_cast<char>(c >> 8));
            out.push_back(static_cast<char>(c & 0xFF));
        }
    }
    return py::bytes(out);
}

py::str _tag(int64_t v) {
    return py::pack(">I", v).decode("latin-1");
}

XUR::XUR(const py::bytes& data) {
    d = data;
    int64_t magic;
    std::tie(magic, version, flags) = py::unpack_from<int64_t, int64_t, int64_t>(">III", data, 0);
    if (magic != MAGIC) throw py::ValueError(py::percent(py::str("not a XUR (magic %r)"), _tag(magic)));
    std::tie(tool_version, file_size, section_count) = py::unpack_from<int64_t, int64_t, int64_t>(">HIH", data, 12);
    // (magic, offset, length)
    for (int64_t i = 0; i < section_count; ++i) {
        int64_t o = 20 + i * 12;
        sections.append(py::unpack_from<int64_t, int64_t, int64_t>(">III", data, o));
    }
    data_base = 20 + section_count * 12;
}

std::optional<py::bytes> XUR::section_data(int64_t magic) const {
    for (const auto& [m, off, length] : sections) {
        if (m == magic) {
            int64_t s = data_base + off;
            return d.slice(s, s + length);
        }
    }
    return std::nullopt;
}

py::list<py::str> XUR::strings() const {
    std::optional<py::bytes> blob = section_data(STRN);
    if (!blob) return py::list<py::str>{};
    py::list<py::str> out;
    int64_t o = 0;
    while (o + 2 <= py::len(*blob)) {
        int64_t n = py::unpack_from1<int64_t>(">H", *blob, o);
        o += 2;
        py::str s = utf16be_decode_replace(blob->slice(o, o + n * 2));
        out.append(s);
        o += n * 2;
    }
    return out;
}

py::bytes build_strn(const py::list<py::str>& strings) {
    py::bytes body;
    for (const py::str& s : strings) {
        py::bytes e = utf16be_encode(s);
        body = body + py::pack(">H", py::len(s)) + e;
    }
    return body;
}

py::bytes set_strings(const py::bytes& data, const py::list<py::str>& new_strings) {
    XUR x(data);
    // the first section whose magic is STRN: (index, offset, length)
    std::optional<std::tuple<int64_t, int64_t, int64_t>> strn;
    for (int64_t i = 0; i < py::len(x.sections); ++i) {
        const auto& [m, off, length] = x.sections[i];
        if (m == STRN) {
            strn = std::make_tuple(i, off, length);
            break;
        }
    }
    if (!strn) throw py::ValueError("no STRN section to edit");
    auto [idx, soff, slen] = *strn;
    py::bytes body = build_strn(new_strings);
    int64_t delta = py::len(body) - slen;
    int64_t db = x.data_base;
    py::bytes area = data.slice_from(db);
    py::bytes new_area = area.slice_to(soff) + body + area.slice_from(soff + slen);
    py::bytearray out(data.slice_to(db));   // header + section table
    py::pack_into(">I", out, 14, x.file_size + delta);   // file size
    for (int64_t i = 0; i < py::len(x.sections); ++i) {
        const auto& [m, off, length] = x.sections[i];
        int64_t eo = 20 + i * 12;
        if (i == idx) {
            py::pack_into(">I", out, eo + 8, py::len(body));   // STRN new length
        } else if (off > soff) {
            py::pack_into(">I", out, eo + 4, off + delta);   // shift later section
        }
    }
    return py::bytes(out.view()) + new_area;
}

py::bytes set_string(const py::bytes& data, int64_t index, const py::str& text) {
    py::list<py::str> strs = XUR(data).strings();
    if (!(0 <= index && index < py::len(strs)))
        throw py::IndexError(py::percent(py::str("string index %d out of range (0..%d)"), index, py::len(strs) - 1));
    strs[index] = text;
    return set_strings(data, strs);
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage:\n  xur {info|strings} <file.xur>\n"
                          "  xur set <file.xur> <index> <text> <out.xur>   edit a UI string, repack"));
        return;
    }
    if (a[0] == py::str("set")) {
        if (py::len(a) < 5) {
            py::print(py::str("usage: xur set <file.xur> <index> <text> <out.xur>"));
            return;
        }
        py::bytes out = set_string(py::open_rb(a[1]).read(), py::int_from_str(a[2], 0), a[3]);
        py::open(a[4], py::str("wb")).write(out);
        py::print(py::percent(py::str("set string [%s] -> %s (%d bytes)"), a[2], a[4], py::len(out)));
        return;
    }
    bool is_op = a[0] == py::str("info") || a[0] == py::str("strings");
    py::str op = is_op ? a[0] : py::str("info");
    py::str path = is_op ? a[1] : a[0];
    XUR x(py::open_rb(path).read());
    if (op == py::str("info")) {
        py::print(py::percent(py::str("magic       : XUIB  version %d  toolVersion 0x%X"), x.version, x.tool_version));
        py::print(py::percent(py::str("file size   : %d (actual %d)"), x.file_size, py::len(x.d)));
        py::print(py::percent(py::str("sections    : %d"), x.section_count));
        for (const auto& [m, off, length] : x.sections)
            py::print(py::percent(py::str("   %-6s offset=0x%-6X length=%d"), _tag(m), off, length));
    } else {
        py::list<py::str> strs = x.strings();
        int64_t i = 0;
        for (const py::str& s : strs) {
            py::print(py::percent(py::str("  [%3d] %s"), i, s));
            ++i;
        }
        py::print(py::percent(py::str("total: %d strings"), py::len(strs)));
    }
}

}  // namespace codebreaker::formats::xbox::xur
