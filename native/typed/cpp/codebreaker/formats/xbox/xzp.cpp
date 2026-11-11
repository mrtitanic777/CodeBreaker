// codebreaker/formats/xbox/xzp.cpp -- see xzp.hpp.

#include "codebreaker/formats/xbox/xzp.hpp"

#include <algorithm>

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::xzp {

namespace {

namespace na = native_ask;

// struct.pack_into(">I", b, at, v): struct.error when the field does not fit the buffer or the number the format
void pack_into_be32(std::string& b, size_t at, int64_t v) {
    if (b.size() < at + 4)
        throw py::StructError("pack_into requires a buffer of at least " + std::to_string(at + 4) + " bytes for packing 4 bytes at offset " +
                              std::to_string(at) + " (actual buffer size is " + std::to_string(b.size()) + ")");
    if (v < 0 || v > 0xFFFFFFFFLL) throw py::StructError("argument out of range");
    for (int k = 0; k < 4; ++k) b[at + size_t(k)] = char((v >> (8 * (3 - k))) & 0xFF);
}

// int(v).to_bytes(width, "big"): OverflowError when it does not fit
void put_be(std::string& out, int64_t v, int width) {
    if (v < 0) throw py::OverflowError("can't convert negative int to unsigned");
    if (width < 8 && v >= (int64_t(1) << (8 * width))) throw py::OverflowError("int too big to convert");
    for (int k = width - 1; k >= 0; --k) out += char((v >> (8 * k)) & 0xFF);
}

// text.encode("utf-16-le")
std::string utf16le(const py::str& s) {
    std::string out;
    for (char32_t c : s.u32()) {
        auto unit = [&](uint32_t u) {
            out += char(u & 0xFF);
            out += char((u >> 8) & 0xFF);
        };
        if (c < 0x10000) unit(uint32_t(c));
        else {
            uint32_t v = uint32_t(c) - 0x10000;
            unit(0xD800 + (v >> 10));
            unit(0xDC00 + (v & 0x3FF));
        }
    }
    return out;
}

Parsed parse_data(const py::bytes& d, const py::str& label) {
    Parsed r = na::ask<Parsed>("xzp.parse", d, na::no_own, label);
    r.data = d;
    return r;
}

}  // namespace

const py::str DESCRIPTION("unpack XUIZ .xzp containers (Minecraft v1 + dashboard v3)");
const py::bytes PNG = py::bytes::fromhex(py::str("89504e470d0a1a0a"));
const py::bytes IEND = py::bytes::fromhex(py::str("49454e44ae426082"));

Parsed parse(const py::str& path) { return parse_data(py::open_rb(path).read(), path); }

Parsed parse(const py::bytes& data) { return parse_data(data, py::str("<bytes>")); }

std::tuple<int64_t, int64_t, int64_t> verify(const Parsed& r) {
    const py::bytes& d = r.data;
    int64_t base = r.base;
    int64_t n = 0, hit = 0, tail = 0;
    for (const Entry& e : r.entries) {
        if (!e.name.lower().endswith(py::str(".png"))) continue;
        n += 1;
        if (d.slice(base + e.offset, base + e.offset + 8) == PNG) hit += 1;
        if (d.slice(base + e.offset + e.size - 8, base + e.offset + e.size) == IEND) tail += 1;
    }
    return std::make_tuple(n, hit, tail);
}

py::list<std::tuple<py::str, py::bytes>> payloads(const py::str& path) {
    py::bytes d = py::open_rb(path).read();
    return na::ask<py::list<std::tuple<py::str, py::bytes>>>("xzp.payloads", d, na::no_own, path);
}

py::list<std::tuple<py::str, py::bytes>> payloads(const py::bytes& data) {
    return na::ask<py::list<std::tuple<py::str, py::bytes>>>("xzp.payloads", data, na::no_own, py::str("<bytes>"));
}

namespace {
int64_t write_out(const py::list<std::tuple<py::str, py::bytes>>& all, const py::str& outdir) {
    int64_t n = 0;
    for (const auto& rb : all) {
        py::str dest = py::os::path::join(outdir, std::get<0>(rb));
        py::str folder = py::os::path::dirname(dest);
        py::os::makedirs(folder.empty() ? outdir : folder, 0777, true);
        py::file f = py::open(dest, py::str("wb"));
        f.write(std::get<1>(rb));
        f.close();
        n += 1;
    }
    return n;
}
}  // namespace

int64_t extract(const py::str& path, const py::str& outdir) { return write_out(payloads(path), outdir); }

int64_t extract(const py::bytes& data, const py::str& outdir) { return write_out(payloads(data), outdir); }

namespace {
py::str build_from(const py::str& folder, const py::str& out_path, const py::bytes& tpl) {
    if (tpl.slice(0, 4) != py::bytes("XUIZ")) throw py::ValueError("template is not a XUIZ container");
    std::string hdr = tpl.slice(0, TBL).raw();                          // 0x17: header incl. the skew byte
    // Recurse: v1 Minecraft entry names are nested, backslash-separated and
    // carry their directory prefix (e.g. 'media\\skin_Minecraft.xur',
    // 'media\\de-DE\\skin_Minecraft.xus'). Name each entry by its path relative
    // to `folder`, with '\\' separators, so packing folder=".../XZP" reproduces
    // the 'media\\...' layout the game loads.
    std::vector<std::tuple<py::str, py::bytes>> files;
    for (const auto& step : py::os::walk(folder)) {
        const py::str& root = std::get<0>(step);
        std::vector<py::str> fnames;
        for (const py::str& n : std::get<2>(step)) fnames.push_back(n);
        std::sort(fnames.begin(), fnames.end());
        for (const py::str& fn : fnames) {
            py::str fp = py::os::path::join(root, fn);
            py::str rel = py::os::path::relpath(fp, folder).replace(py::str("/"), py::str("\\")).replace(py::str("\\"), py::str("\\"));
            files.emplace_back(rel, py::open_rb(fp).read());
        }
    }
    std::stable_sort(files.begin(), files.end(), [](const auto& a, const auto& b) { return std::get<0>(a).lower() < std::get<0>(b).lower(); });
    std::string table, payloads;
    int64_t offset = 0;
    for (const auto& nd : files) {
        const py::bytes& data = std::get<1>(nd);
        std::string name = utf16le(std::get<0>(nd));
        put_be(table, data.size(), 3);                                  // uint24be size
        put_be(table, offset, 4);                                       // uint32be offset
        int64_t units = int64_t(name.size()) / 2;                       // uint16le name length
        if (units > 0xFFFF) throw py::StructError("ushort format requires 0 <= number <= 65535");
        table += char(units & 0xFF);
        table += char((units >> 8) & 0xFF);
        table += name;                                                  // utf-16le name
        payloads += data.raw();
        offset += data.size();
    }
    int64_t base_rel = (TBL + int64_t(table.size())) - BASE_REL;
    pack_into_be32(hdr, 0x10, base_rel);
    std::string out = hdr + table + payloads;
    pack_into_be32(out, 0x08, int64_t(out.size()));                     // file_size
    py::file f = py::open(out_path, py::str("wb"));
    f.write(py::bytes(out));
    f.close();
    return out_path;
}
}  // namespace

py::str build(const py::str& folder, const py::str& out_path, const py::str& template_path) {
    return build_from(folder, out_path, py::open_rb(template_path).read());
}

py::str build(const py::str& folder, const py::str& out_path, const py::bytes& template_data) {
    return build_from(folder, out_path, template_data);
}

void main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    if (argv.size() >= 1 && argv[0] == S("pack")) {
        if (py::len(argv) + 1 < 5) {                  // len(sys.argv) < 5: the program's name counts
            py::print(S("usage: xzp pack <folder> <out.xzp> <template.xzp>"));
            throw py::SystemExit(2);
        }
        py::str out = build(argv[1], argv[2], argv[3]);
        Parsed r = parse(out);
        py::print(py::percent(S("packed %d entries -> %s"), r.entries.size(), out));
        throw py::SystemExit(0);
    }
    py::list<py::str> args;
    bool doext = false;
    for (const py::str& a : argv) {
        if (a != S("-x")) args.append(a);
        else doext = true;
    }
    for (const py::str& path : args) {
        Parsed r = parse(path);
        auto [npng, hit, tail] = verify(r);
        int64_t used = 0;
        for (const Entry& e : r.entries) used = std::max(used, r.base + e.offset + e.size);
        py::print(py::os::path::basename(path));
        py::print(py::percent(S("  version=%d  declared_size=%d  actual=%d  %s"), r.version, r.file_size, py::len(r.data),
                              r.file_size == py::len(r.data) ? S("OK") : S("MISMATCH")));
        py::print(py::percent(S("  entries=%d  table_end=0x%X  payload_base=0x%X"), r.entries.size(), r.table_end, r.base));
        py::print(py::percent(S("  png payloads: %d   valid signature: %d   valid IEND: %d"), npng, hit, tail));
        py::print(py::percent(S("  consumed %d of %d bytes (%d trailing)"), used, py::len(r.data), py::len(r.data) - used));
        for (int64_t k = 0; k < r.entries.size() && k < 6; ++k) {
            const Entry& e = r.entries[k];
            py::print(py::percent(S("     %-9d @%-9d %s"), e.size, e.offset, e.name));
        }
        if (r.entries.size() > 6) py::print(py::percent(S("     ... %d more"), r.entries.size() - 6));
        if (doext) {
            py::str out = std::get<0>(py::os::path::splitext(path)) + S("_extracted");
            py::print(py::percent(S("  extracted %d files -> %s"), extract(path, out), out));
        }
        py::print(S(""));
    }
}

}  // namespace codebreaker::formats::xbox::xzp
