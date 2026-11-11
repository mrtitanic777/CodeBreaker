// codebreaker/formats/xbox/xbe.cpp -- see xbe.hpp.

#include "codebreaker/formats/xbox/xbe.hpp"

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::xbe {

namespace {

namespace na = native_ask;

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

}  // namespace

const py::bytes MAGIC("XBEH");
const py::dict<py::str, int64_t> ENTRY_KEYS = {{py::str("retail"), 0xA8FC57AB}, {py::str("debug"), 0x94859D4B}, {py::str("chihiro"), 0x40B5C16E}};
const py::dict<py::str, int64_t> THUNK_KEYS = {{py::str("retail"), 0x5B6D40B6}, {py::str("debug"), 0xEFB1F152}, {py::str("chihiro"), 0x2290059D}};
const py::dict<int64_t, py::str> REGION = {{0x1, py::str("North America")}, {0x2, py::str("Japan")}, {0x4, py::str("Rest of World")},
                                           {0x80000000, py::str("Manufacturing/Debug")}};
const py::dict<int64_t, py::str> MEDIA = {{0x1, py::str("HDD")},         {0x2, py::str("DVD-X2")},        {0x4, py::str("DVD-CD")},
                                          {0x8, py::str("CD")},          {0x10, py::str("DVD-5-RO")},     {0x20, py::str("DVD-9-RO")},
                                          {0x40, py::str("DVD-5-RW")},   {0x80, py::str("DVD-9-RW")},     {0x100, py::str("Dongle")},
                                          {0x200, py::str("Media Board")}, {0x40000000, py::str("Nonsecure Hard Disk")},
                                          {0x80000000, py::str("Nonsecure Mode")}};
const py::str DESCRIPTION("read an Original Xbox XBE executable: header/certificate/sections + entry decrypt");

XBE::XBE(const py::bytes& data) {
    d = data;
    // (the header and certificate: formats.dll's)
    Header h = na::ask<Header>("xbe.parse", data, na::no_own);
    base = h.base;
    size_of_image = h.size_of_image;
    size_of_headers = h.size_of_headers;
    cert_addr = h.cert_addr;
    section_count = h.section_count;
    section_hdr_addr = h.section_hdr_addr;
    init_flags = h.init_flags;
    _entry_raw = h._entry_raw;
    _thunk_raw = h._thunk_raw;
    cert_size = h.cert_size;
    title_id = h.title_id;
    title_name = h.title_name;
    allowed_media = h.allowed_media;
    region = h.region;
    ratings = h.ratings;
    disk_number = h.disk_number;
    version = h.version;
}

py::str XBE::title_id_str() const {
    int64_t pub = (title_id >> 16) & 0xFFFF;
    int64_t num = title_id & 0xFFFF;
    int64_t a = (pub >> 8) & 0xFF, b = pub & 0xFF;
    py::str pubs = (0x20 <= a && a < 0x7F && 0x20 <= b && b < 0x7F) ? py::chr(a) + py::chr(b) : py::percent(py::str("%04X"), pub);
    return py::percent(py::str("%s-%03d"), pubs, num);
}

std::tuple<int64_t, py::str> XBE::_decrypt(int64_t raw, const py::dict<py::str, int64_t>& keys) const {
    for (const auto& kv : keys.items()) {
        int64_t v = raw ^ std::get<1>(kv);
        if (base <= v && v < base + size_of_image + 0x100000) return std::make_tuple(v, std::get<0>(kv));
    }
    return std::make_tuple(raw ^ keys.at(py::str("retail")), py::str("unknown"));
}

std::tuple<int64_t, py::str> XBE::entry_point() const { return _decrypt(_entry_raw, ENTRY_KEYS); }

std::tuple<int64_t, py::str> XBE::kernel_thunk() const { return _decrypt(_thunk_raw, THUNK_KEYS); }

py::list<Section> XBE::sections() const {
    return na::ask<py::list<Section>>("xbe.sections", d, na::no_own, base, section_count, section_hdr_addr);
}

py::str XBE::region_str() const {
    py::list<py::str> names;
    for (const auto& kv : REGION.items())
        if (region & std::get<0>(kv)) names.append(std::get<1>(kv));
    py::str joined = py::join(py::str("|"), names);
    return joined.empty() ? py::percent(py::str("0x%X"), region) : joined;
}

py::str XBE::media_str() const {
    py::list<py::str> names;
    for (const auto& kv : MEDIA.items())
        if (allowed_media & std::get<0>(kv)) names.append(std::get<1>(kv));
    py::str joined = py::join(py::str("|"), names);
    return joined.empty() ? py::percent(py::str("0x%X"), allowed_media) : joined;
}

py::str to_pe(const py::str& path) {
    py::bytes data = py::open_rb(path).read();
    py::str digest = py::hashlib::sha1(data).hexdigest().slice(0, 12);
    py::str out = state({py::str("unpacked"), py::str("xbe"),
                         py::percent(py::str("%s-%s.exe"), std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower(), digest)});
    if (!py::os::path::isfile(out)) {
        {
            py::file f = py::open(out + py::str(".part"), py::str("wb"));
            f.write(pe_image(XBE(data)));
            f.close();
        }
        py::os::replace(out + py::str(".part"), out);
    }
    return out;
}

py::bytes pe_image(const XBE& x) { return na::ask<py::bytes>("xbe.pe_image", x.d, na::no_own); }

void main(const py::list<py::str>& a) {
    auto S = [](const char* s) { return py::str(s); };
    if (a.empty() || a[0] == S("-h") || a[0] == S("--help")) {
        py::print(S("usage: xbe {info|sections} <default.xbe>"));
        return;
    }
    bool is_op = a[0] == S("info") || a[0] == S("sections");
    py::str op = is_op ? a[0] : S("info");
    py::str path = is_op ? a[1] : a[0];
    XBE x(py::open_rb(path).read());
    if (op == S("sections")) {
        for (const Section& s : x.sections())
            py::print(py::percent(S("  %-12s va=0x%08X vsize=%-8d raw=0x%06X size=%-8d flags=0x%X"), s.name, s.vaddr, s.vsize,
                                  s.raw_addr, s.raw_size, s.flags));
        py::print(py::percent(S("total: %d sections"), x.section_count));
        return;
    }
    auto ep = x.entry_point();
    auto kt = x.kernel_thunk();
    py::print(S("magic        : XBEH"));
    py::print(py::percent(S("title        : %r"), x.title_name));
    py::print(py::percent(S("title id     : 0x%08X  (%s)"), x.title_id, x.title_id_str()));
    py::print(py::percent(S("base / size  : 0x%08X / %d bytes"), x.base, x.size_of_image));
    py::print(py::percent(S("entry point  : 0x%08X  (%s key)"), std::get<0>(ep), std::get<1>(ep)));
    py::print(py::percent(S("kernel thunk : 0x%08X  (%s key)"), std::get<0>(kt), std::get<1>(kt)));
    py::print(py::percent(S("region       : %s"), x.region_str()));
    py::print(py::percent(S("media        : %s"), x.media_str()));
    py::print(py::percent(S("ratings      : 0x%X   version: %d   disk: %d"), x.ratings, x.version, x.disk_number));
    py::print(py::percent(S("sections     : %d"), x.section_count));
}

}  // namespace codebreaker::formats::xbox::xbe
