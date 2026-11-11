// codebreaker/formats/xbox/xdbf.cpp -- see xdbf.hpp.

#include "codebreaker/formats/xbox/xdbf.hpp"

#include <algorithm>

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::xdbf {

namespace {

namespace na = native_ask;

// struct.unpack_from(">I", b, at)[0] of the bytes in `b`
int64_t be32(const std::string& b, size_t at) {
    return (int64_t(uint8_t(b[at])) << 24) | (int64_t(uint8_t(b[at + 1])) << 16) | (int64_t(uint8_t(b[at + 2])) << 8) |
           int64_t(uint8_t(b[at + 3]));
}

// struct.pack_into(">I", b, at, v)
void put_be32(std::string& b, size_t at, int64_t v) {
    for (int k = 0; k < 4; ++k) b[at + size_t(k)] = char((v >> (8 * (3 - k))) & 0xFF);
}

// struct.pack_into(">Q", b, at, v)
void put_be64(std::string& b, size_t at, int64_t v) {
    for (int k = 0; k < 8; ++k) b[at + size_t(k)] = char((v >> (8 * (7 - k))) & 0xFF);
}

// NS.get(ns, "ns%d" % ns)
py::str ns_name(int64_t ns) { return NS.get(ns, py::percent(py::str("ns%d"), ns)); }

// eid.to_bytes(8, "big")
py::bytes id_bytes(int64_t eid) {
    if (eid < 0) throw py::OverflowError("can't convert negative int to unsigned");
    std::string s(8, '\0');
    put_be64(s, 0, eid);
    return py::bytes(s);
}

}  // namespace

const py::str DESCRIPTION("read Xbox 360 XDBF/GPD (profiles, achievements): info/list/ach/extract");
const py::bytes MAGIC("XDBF");
const py::dict<int64_t, py::str> NS = {{1, py::str("achievement")}, {2, py::str("image")},        {3, py::str("setting")},
                                       {4, py::str("title")},       {5, py::str("string")},       {6, py::str("avatar-award")},
                                       {7, py::str("avatar-award-1")}};

XDBF::XDBF(const py::bytes& data) {
    d = data;
    // (the header, the tables: formats.dll's)
    auto r = na::ask<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, py::list<Entry>>>("xdbf.parse", data, na::no_own);
    version = std::get<0>(r);
    entry_table_len = std::get<1>(r);
    entry_count = std::get<2>(r);
    free_table_len = std::get<3>(r);
    free_count = std::get<4>(r);
    entries = std::get<5>(r);
    data_start = 0x18 + entry_table_len * 0x12 + free_table_len * 8;
}

std::optional<py::bytes> XDBF::payload(int64_t ns, int64_t eid) const {
    for (const Entry& e : entries) {
        if (std::get<0>(e) == ns && std::get<1>(e) == eid) {
            int64_t start = data_start + std::get<2>(e);
            return d.slice(start, start + std::get<3>(e));
        }
    }
    return std::nullopt;
}

py::bytes XDBF::_entry_bytes(int64_t off, int64_t length) const {
    int64_t start = data_start + off;
    return d.slice(start, start + length);
}

py::list<Achievement> XDBF::achievements() const {
    py::list<std::tuple<int64_t, int64_t, int64_t>> where;
    for (const Entry& e : entries) where.append(std::make_tuple(std::get<0>(e), std::get<2>(e), std::get<3>(e)));
    return na::ask<py::list<Achievement>>("xdbf.achievements", d, na::no_own, data_start, where);
}

py::dict<int64_t, int64_t> XDBF::counts() const {
    py::dict<int64_t, int64_t> c;
    for (const Entry& e : entries) c[std::get<0>(e)] = c.get(std::get<0>(e), 0) + 1;
    return c;
}

py::dict<std::tuple<int64_t, int64_t>, py::bytes> XDBF::_payload_map() const {
    py::dict<std::tuple<int64_t, int64_t>, py::bytes> m;
    for (const Entry& e : entries) {
        auto key = std::make_tuple(std::get<0>(e), std::get<1>(e));
        m[key] = _ov.contains(key) ? _ov.at(key) : _entry_bytes(std::get<2>(e), std::get<3>(e));
    }
    return m;
}

void XDBF::set_payload(int64_t ns, int64_t eid, const py::bytes& data) {
    _ov[std::make_tuple(ns, eid)] = data;
    bool found = false;
    for (const Entry& e : entries)
        if (std::get<0>(e) == ns && std::get<1>(e) == eid) {
            found = true;
            break;
        }
    if (!found) entries.append(std::make_tuple(ns, eid, int64_t(0), data.size()));
}

bool XDBF::unlock_achievement(int64_t ach_id, bool unlock) {
    for (int64_t k = 0; k < entries.size(); ++k) {        // (live: set_payload may add to the list)
        Entry e = entries[k];
        int64_t ns = std::get<0>(e), eid = std::get<1>(e);
        if (ns != 1) continue;
        auto key = std::make_tuple(ns, eid);
        std::string b = (_ov.contains(key) ? _ov.at(key) : _entry_bytes(std::get<2>(e), std::get<3>(e))).raw();
        if (b.size() < 0x1C || be32(b, 4) != ach_id) continue;
        int64_t flags = be32(b, 0x10);
        flags = unlock ? (flags | 0x20000) : (flags & ~int64_t(0x20000));
        put_be32(b, 0x10, flags);
        put_be64(b, 0x14, unlock ? UNLOCK_FILETIME : 0);
        set_payload(ns, eid, py::bytes(b));
        return true;
    }
    return false;
}

int64_t XDBF::unlock_all() {
    int64_t n = 0;
    for (const Achievement& a : achievements())
        if (unlock_achievement(a.id, true)) n += 1;
    return n;
}

py::bytes XDBF::build() const {
    auto pm = _payload_map();
    py::list<std::tuple<int64_t, py::bytes, py::bytes>> items;
    for (const Entry& e : entries)
        items.append(std::make_tuple(std::get<0>(e), id_bytes(std::get<1>(e)), pm.at(std::make_tuple(std::get<0>(e), std::get<1>(e)))));
    return na::ask<py::bytes>("xdbf.build", py::bytes(), na::no_own, version, entry_table_len, free_table_len, items);
}

int64_t XDBF::extract_all(const py::str& outdir) const {
    py::os::makedirs(outdir, 0777, true);
    for (const Entry& e : entries) {
        int64_t ns = std::get<0>(e);
        py::str name = py::percent(py::str("%s_%016X.bin"), ns_name(ns), std::get<1>(e));
        py::file f = py::open(py::os::path::join(outdir, name), py::str("wb"));
        f.write(_entry_bytes(std::get<2>(e), std::get<3>(e)));
        f.close();
    }
    return entries.size();
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& a) {
    auto S = [](const char* s) { return py::str(s); };
    if (a.empty() || a[0] == S("-h") || a[0] == S("--help")) {
        py::print(S("usage: xdbf {info|list|ach|extract} <file.gpd> [outdir]\n"
                    "       xdbf unlock <file.gpd> <ach_id> [out.gpd]\n"
                    "       xdbf unlock-all <file.gpd> [out.gpd]\n"
                    "       xdbf set <file.gpd> <ns> <id> <payload.bin> [out.gpd]"));
        return;
    }
    py::str op = a[0];
    if (py::len(a) < 2) {
        py::print(py::percent(S("usage: xdbf %s <file.gpd>"), op));
        throw py::SystemExit(2);
    }
    XDBF x(py::open_rb(a[1]).read());
    if (op == S("info")) {
        py::print(py::percent(S("file        : %s"), py::os::path::basename(a[1])));
        py::print(py::percent(S("version     : 0x%X"), x.version));
        py::print(py::percent(S("entries     : %d (table %d)  free: %d"), x.entry_count, x.entry_table_len, x.free_count));
        py::print(py::percent(S("data start  : 0x%X"), x.data_start));
        py::print(S("by namespace:"));
        auto c = x.counts();
        std::vector<std::tuple<int64_t, int64_t>> items = c.items();
        std::sort(items.begin(), items.end());
        for (const auto& kv : items)
            py::print(py::percent(S("   %-14s %d"), ns_name(std::get<0>(kv)), std::get<1>(kv)));
    } else if (op == S("list")) {
        for (const Entry& e : x.entries)
            py::print(py::percent(S("  %-14s id=%016X  %8d bytes"), ns_name(std::get<0>(e)), std::get<1>(e), std::get<3>(e)));
        py::print(py::percent(S("total: %d entries"), x.entries.size()));
    } else if (op == S("ach")) {
        auto ach = x.achievements();
        int64_t unlocked = 0, gamerscore = 0;
        for (const Achievement& a2 : ach) {
            py::str mark = a2.unlocked ? S("*") : S(" ");
            py::print(py::percent(S("  [%s] %5dG  %-40s | %s"), mark, a2.score, a2.name.slice(0, 40), a2.unlocked_desc.slice(0, 50)));
            if (a2.unlocked) unlocked += 1;
            gamerscore += a2.score;
        }
        py::print(py::percent(S("total: %d achievements, %d unlocked, %dG"), ach.size(), unlocked, gamerscore));
    } else if (op == S("extract")) {
        py::str outdir = py::len(a) > 2 ? a[2] : std::get<0>(py::os::path::splitext(a[1])) + S("_xdbf");
        py::print(py::percent(S("extracted %d entries -> %s"), x.extract_all(outdir), outdir));
    } else if (op == S("unlock") || op == S("unlock-all") || op == S("set")) {
        py::str out;
        if (op == S("set")) {                                    // xdbf set <gpd> <ns> <id> <payload> [out]
            if (py::len(a) < 5) {
                py::print(S("usage: xdbf set <file.gpd> <ns> <id> <payload.bin> [out.gpd]"));
                throw py::SystemExit(2);
            }
            x.set_payload(py::int_parse(a[2], 0), py::int_parse(a[3], 0), py::open_rb(a[4]).read());
            out = py::len(a) > 5 ? a[5] : a[1];
            py::print(py::percent(S("set ns=%s id=%s (%d bytes)"), a[2], a[3], py::os::path::getsize(a[4])));
        } else if (op == S("unlock")) {                          // xdbf unlock <gpd> <ach_id> [out]
            if (py::len(a) < 3) {
                py::print(S("usage: xdbf unlock <file.gpd> <ach_id> [out.gpd]"));
                throw py::SystemExit(2);
            }
            bool ok = x.unlock_achievement(py::int_parse(a[2], 0), true);
            out = py::len(a) > 3 ? a[3] : a[1];
            py::print(py::percent(S("unlocked achievement %s: %s"), a[2], ok ? S("ok") : S("NOT FOUND")));
        } else {                                                 // unlock-all
            int64_t n = x.unlock_all();
            out = py::len(a) > 2 ? a[2] : a[1];
            py::print(py::percent(S("unlocked %d achievements"), n));
        }
        {
            py::file f = py::open(out, S("wb"));
            f.write(x.build());
            f.close();
        }
        py::print(py::percent(S("wrote %s"), out));
    } else {
        py::print(py::percent(S("unknown op %r"), op));
        throw py::SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::xdbf
