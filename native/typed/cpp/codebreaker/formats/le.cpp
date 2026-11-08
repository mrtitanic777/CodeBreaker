// codebreaker/formats/le.cpp -- typed C++ of codebreaker.formats.le (see le.hpp).
//
// The readers are native/formats/le (le.cc, with core/bytes.cc and core/text.cc): linked, and called
// directly with typed results.  The question handlers of native/formats/le/questions.cc and the image
// code of native/formats/le/image.cc (which shaped their answers as pickle / a dynamic Value) are the
// typed functions below.

#include "codebreaker/formats/le.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/detect.hpp"
#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/text.h"
#include "../../../../formats/le/le.h"

namespace codebreaker::formats::le {

namespace {

namespace cb = ::cbf;
namespace cl = ::cbf::le;

// ---- the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") {
        std::string t = m;                                    // message.strip("'"), then str(KeyError(t)) is its repr
        size_t a = t.find_first_not_of('\'');
        size_t b = t.find_last_not_of('\'');
        t = a == std::string::npos ? std::string() : t.substr(a, b - a + 1);
        throw py::KeyError(py::repr(py::str(t)));
    }
    if (k == "UnicodeDecodeError") throw py::UnicodeDecodeError("'ascii' codec can't decode bytes in position 0-0: " + m);
    if (k == "UnicodeEncodeError") throw py::UnicodeEncodeError("'latin-1' codec can't encode characters in position 0-0: " + m);
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "ZeroDivisionError") throw py::ZeroDivisionError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "AttributeError") throw py::AttributeError(m);
    if (k == "OSError") throw py::OSError(m);
    if (k == "AssertionError") throw py::AssertionError(m);
    throw py::RuntimeError(m);
}

// runs a call into the C++ reader, its exceptions made the Python's
template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cb::PyError& e) {
        raise_py(e);
    } catch (const cb::Fallback& f) {
        throw py::RuntimeError(f.reason);
    }
}

cb::Bytes B(const py::bytes& b) { return cb::Bytes(b.raw()); }
py::str S(const std::string& utf8) { return py::str(utf8); }
py::bytes PB(const std::string& s) { return py::bytes(s); }
py::bytes PB(cb::Bytes b) { return py::bytes(b.str()); }

[[noreturn]] void key_error(const std::string& key) { throw py::KeyError(py::repr(py::str(key))); }

Object object_of(const cl::Object& o) {
    Object r;
    r.idx = o.idx;
    r.vsize = o.vsize;
    r.base = o.base;
    r.flags = o.flags;
    r.pmidx = o.pmidx;
    r.pcnt = o.pcnt;
    r.exec = o.exec();
    r.read = o.read();
    r.write = o.write();
    return r;
}

// the C++ reader's image of an LEFile (the image is read again from the bytes and lfa on every call, as LEFile keeps nothing that is not in them)
cl::Image image_of(const LEFile& lf) { return cl::Image(B(lf.d()), lf.lfa); }

using NameVec = std::vector<std::pair<std::string, uint16_t>>;
NameList name_list(const NameVec& v) {
    NameList out;
    for (const auto& n : v) out.append(std::make_tuple(S(n.first), int64_t(n.second)));
    return out;
}

// the `[(name, lfa)]` of questions.cc, opened in order (one that fails raises there)
py::list<std::tuple<py::str, LEFile>> opened(const std::shared_ptr<const py::bytes>& data,
                                             const std::vector<std::pair<std::string, int64_t>>& pairs) {
    py::list<std::tuple<py::str, LEFile>> out;
    for (const auto& p : pairs) out.append(std::make_tuple(S(p.first), LEFile(data, p.second)));
    return out;
}

// questions.cc vxds(): a plain LE/LX is one entry named by the file, a W3 container's members are its entries
std::vector<std::pair<std::string, int64_t>> vxds(cb::Bytes d, const std::string& basename) {
    int64_t lfa = d.u32(0x3c);
    cb::Bytes sig = d.slice(lfa, lfa + 2);
    if (sig.is("W3", 2)) return cl::W3(d, lfa).modules();
    if (sig.is("LE", 2) || sig.is("LX", 2)) return {{basename, lfa}};
    cb::raise("ValueError", "not an LE/LX/W3 image");
}

py::bytes read_file(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read(-1);
    f.close();
    return b;
}

// ---- packing, as image.cc does it (struct.pack with its range check, int.to_bytes with its OverflowError)
void put_le(std::string& out, uint64_t v, int width) {
    for (int k = 0; k < width; ++k) out += char(v >> (8 * k));
}
void pack(std::string& out, int64_t v, int width, bool is_signed = false) {
    const int64_t lo = is_signed ? -(int64_t(1) << (8 * width - 1)) : 0;
    const int64_t hi = is_signed ? (int64_t(1) << (8 * width - 1)) - 1 : (width == 4 ? 0xFFFFFFFFLL : (int64_t(1) << (8 * width)) - 1);
    if (v < lo || v > hi) throw py::StructError("argument out of range");
    put_le(out, uint64_t(v), width);
}
std::string to_bytes(int64_t v, int width, bool big = false) {
    if (v < 0) throw py::OverflowError("can't convert negative int to unsigned");
    if (width < 8 && v >= (int64_t(1) << (8 * width))) throw py::OverflowError("int too big to convert");
    std::string s;
    for (int k = 0; k < width; ++k) s += char(uint64_t(v) >> (8 * (big ? width - 1 - k : k)));
    return s;
}
void put_into(std::string& hdr, int64_t at, int64_t v) {            // struct.pack_into("<I", hdr, at, v)
    if (v < 0 || v > 0xFFFFFFFFLL) throw py::StructError("'I' format requires 0 <= number <= 4294967295");
    for (int k = 0; k < 4; ++k) hdr[size_t(at + k)] = char(uint64_t(v) >> (8 * k));
}
std::string name_byte(size_t n) {                                    // bytes([len(name)])
    if (n > 255) throw py::ValueError("bytes must be in range(0, 256)");
    return std::string(1, char(n));
}
std::string latin1_of(const py::str& s) {                            // s.encode("latin1"), as the native layer raised it
    return native([&] { return cb::to_latin1(s.utf8()); });
}

template <class T>
const T& field_of(const std::optional<T>& v, const char* key) {      // a dict key the Python needs
    if (!v) key_error(key);
    return *v;
}

}  // namespace

// ---- constants
const int64_t HEADER_SIZE = 0xC4;
const int64_t MODULE_STATIC = 0x28000, MODULE_DYNAMIC = 0x38000, MODULE_OLD = 0x8020;
const py::list<std::tuple<py::str, int64_t, py::str>> _FIELDS = {
    {py::str("byte_order"), 0x02, py::str("B")}, {py::str("word_order"), 0x03, py::str("B")},
    {py::str("level"), 0x04, py::str("I")}, {py::str("cpu"), 0x08, py::str("H")},
    {py::str("os"), 0x0A, py::str("H")}, {py::str("version"), 0x0C, py::str("I")},
    {py::str("module_flags"), 0x10, py::str("I")}, {py::str("pages"), 0x14, py::str("I")},
    {py::str("eip_object"), 0x18, py::str("I")}, {py::str("eip"), 0x1C, py::str("I")},
    {py::str("esp_object"), 0x20, py::str("I")}, {py::str("esp"), 0x24, py::str("I")},
    {py::str("page_size"), 0x28, py::str("I")}, {py::str("last_page_bytes"), 0x2C, py::str("I")},
    {py::str("fixup_size"), 0x30, py::str("I")}, {py::str("loader_size"), 0x38, py::str("I")},
    {py::str("object_table"), 0x40, py::str("I")}, {py::str("objects"), 0x44, py::str("I")},
    {py::str("page_map"), 0x48, py::str("I")}, {py::str("resources"), 0x50, py::str("I")},
    {py::str("resident_names"), 0x58, py::str("I")}, {py::str("entry_table"), 0x5C, py::str("I")},
    {py::str("fixup_pages"), 0x68, py::str("I")}, {py::str("fixup_records"), 0x6C, py::str("I")},
    {py::str("import_modules"), 0x70, py::str("I")}, {py::str("import_module_count"), 0x74, py::str("I")},
    {py::str("import_names"), 0x78, py::str("I")}, {py::str("data_pages"), 0x80, py::str("I")},
    {py::str("preload_pages"), 0x84, py::str("I")}, {py::str("nonresident_names"), 0x88, py::str("I")},
    {py::str("nonresident_size"), 0x8C, py::str("I")}, {py::str("debug_info"), 0x98, py::str("I")},
    {py::str("debug_size"), 0x9C, py::str("I")}, {py::str("resource_data"), 0xB8, py::str("I")},
    {py::str("resource_size"), 0xBC, py::str("I")}, {py::str("device_id"), 0xC0, py::str("H")},
    {py::str("kit_version"), 0xC2, py::str("H")}};

// ---- LEFile
LEFile::LEFile(const py::bytes& data, int64_t lfa_) : lfa(lfa_), data_(std::make_shared<const py::bytes>(data)) { open_(); }
LEFile::LEFile(std::shared_ptr<const py::bytes> data, int64_t lfa_) : lfa(lfa_), data_(std::move(data)) { open_(); }

void LEFile::open_() {
    cl::Image im = native([&] { return image_of(*this); });
    sig = PB(im.sig);
    cpu = im.cpu;
    ostype = im.ostype;
    mpages = im.mpages;
    eip_obj = im.eip_obj;
    eip = im.eip;
    pagesize = im.pagesize;
    last_page_bytes = im.last_page_bytes;
    objtab = im.objtab;
    nobj = im.nobj;
    pagemap = im.pagemap;
    resnames = im.resnames;
    entrytab = im.entrytab;
    fixup_page = im.fixup_page;
    fixup_rec = im.fixup_rec;
    datapages = im.datapages;
    for (const cl::Object& o : im.objects) objects.append(object_of(o));
}

LEFile LEFile::from_buffer(const py::bytes& data) {
    int64_t lfa = native([&] { return cl::Image::from_buffer(B(data)).lfa; });
    return LEFile(data, lfa);
}

LEFile LEFile::from_file(const py::str& path) { return from_buffer(read_file(path)); }

py::bytes LEFile::object_bytes(const Object& obj) const {
    return PB(native([&] {
        cl::Image im = image_of(*this);
        return cl::object_data(im, obj.pmidx, obj.pcnt, obj.vsize);
    }));
}

std::tuple<std::optional<Object>, int64_t> LEFile::page_to_obj(int64_t pg) const {
    auto [idx, off] = native([&] {
        cl::Image im = image_of(*this);
        auto r = im.page_to_obj(pg);
        return std::make_pair(r.first ? std::optional<int64_t>(r.first->idx) : std::nullopt, r.second);
    });
    if (!idx) return std::make_tuple(std::optional<Object>(), off);
    return std::make_tuple(std::optional<Object>(objects[*idx - 1]), off);
}

py::dict<Place, Fixup> LEFile::fixups() const {
    auto fx = native([&] { return image_of(*this).fixups(); });
    py::dict<Place, Fixup> out;
    for (const auto& kv : fx) {
        const cl::Fixup& f = kv.second;
        Fixup v;
        if (f.import) {
            ImportFixup i;
            i.module = f.obj_or_module;
            i.ordinal = f.ordinal;
            v = i;
        } else {
            InternalFixup i;
            i.obj = f.obj_or_module;
            i.target = f.target;
            i.srctype = f.srctype;
            v = i;
        }
        out.set(Place(kv.first.first, kv.first.second), v);
    }
    return out;
}

py::list<py::str> LEFile::import_modules() const {
    auto v = native([&] { return image_of(*this).import_modules(); });
    py::list<py::str> out;
    for (const auto& n : v) out.append(S(n));
    return out;
}

py::str LEFile::resident_name() const { return S(native([&] { return image_of(*this).resident_name(); })); }
py::str LEFile::description() const { return S(native([&] { return image_of(*this).description(); })); }

Header LEFile::header() const {
    return native([&] {
        cl::Image im = image_of(*this);
        auto u = [&](int64_t o) { return int64_t(im.header_u(o, 4)); };
        auto h = [&](int64_t o) { return int64_t(im.header_u(o, 2)); };
        Header r;
        r.signature = S(cb::latin1(im.sig));
        r.cpu = im.cpu;
        r.os = im.ostype;
        r.module_version = u(0x0c);
        r.module_flags = u(0x10);
        r.pages = im.mpages;
        r.entry_object = im.eip_obj;
        r.entry = im.eip;
        r.stack_object = u(0x20);
        r.stack = u(0x24);
        r.page_size = im.pagesize;
        r.last_page_bytes = im.last_page_bytes;
        r.fixup_section_size = u(0x30);
        r.loader_section_size = u(0x38);
        r.objects = im.nobj;
        r.imported_modules = u(0x74);
        r.data_pages_offset = im.datapages;
        r.device_id = h(0xC0);
        r.ddk_version = h(0xC2);
        return r;
    });
}

// The scan of native/formats/le/le.cc Image::ddb(), written out here: that function returns the device name as a
// view of object bytes it has already freed, so its answer cannot be read after the call.
std::optional<Ddb> LEFile::ddb() const {
    return native([&]() -> std::optional<Ddb> {
        cl::Image im = image_of(*this);
        auto printable = [](cb::Bytes b) {
            for (uint8_t c : b)
                if (c < 32 || c >= 127) return false;
            return true;
        };
        auto decode = [&](const cl::Object& obj, cb::Bytes b, int64_t off) -> std::optional<Ddb> {
            cb::Bytes name = b.slice(off + 8, off + 16);
            if (name.empty() || !printable(name)) return std::nullopt;
            cb::Bytes first = name.slice(0, 1);
            if (!(cb::b_isalpha(first) || cb::b_isdigit(first))) return std::nullopt;
            Ddb r;
            r.obj = obj.idx;
            r.rva = off - 4;
            r.version = uint16_t(b.at(off) | (b.at(off + 1) << 8));
            r.device_id = uint16_t(b.at(off + 2) | (b.at(off + 3) << 8));
            r.major = b.at(off + 4);
            r.minor = b.at(off + 5);
            r.flags = uint16_t(b.at(off + 6) | (b.at(off + 7) << 8));
            r.name = S(cb::latin1(name.rstrip(" \x00", 2)));
            r.init_order = b.u32(off + 16);
            r.control_proc = b.u32(off + 20);
            return r;
        };
        // each object's bytes, read when the scan reaches it (a damaged page map raises there, not before)
        std::vector<std::optional<std::string>> bytes_of(im.objects.size());
        auto object_bytes = [&](size_t k) {
            if (!bytes_of[k]) bytes_of[k] = cl::object_data(im, im.objects[k].pmidx, im.objects[k].pcnt, im.objects[k].vsize);
            return cb::Bytes(*bytes_of[k]);
        };
        // strict DDK version stamps -- the common case
        for (size_t k = 0; k < im.objects.size(); ++k) {
            cb::Bytes b = object_bytes(k);
            for (int64_t off = 0; off < b.size() - 26; off += 4) {
                int v = b.at(off) | (b.at(off + 1) << 8);
                if (v != 0x030a && v != 0x0400 && v != 0x040a) continue;
                if (auto r = decode(im.objects[k], b, off)) return r;
            }
        }
        // any 0x03xx/0x04xx stamp whose device id is the header's (+0xC0)
        uint16_t hdr_dev = 0;
        try {
            hdr_dev = im.d.u16(im.lfa + 0xC0);
        } catch (const cb::PyError&) {
            hdr_dev = 0;
        }
        if (hdr_dev)
            for (size_t k = 0; k < im.objects.size(); ++k) {
                cb::Bytes b = object_bytes(k);
                for (int64_t off = 0; off < b.size() - 26; off += 4) {
                    int ver = b.at(off) | (b.at(off + 1) << 8);
                    if ((ver >> 8) != 3 && (ver >> 8) != 4) continue;
                    if ((b.at(off + 2) | (b.at(off + 3) << 8)) != hdr_dev) continue;
                    if (auto r = decode(im.objects[k], b, off)) return r;
                }
            }
        return std::nullopt;
    });
}

// ---- W3File
W3File::W3File(const py::bytes& data, int64_t lfa_) : lfa(lfa_), data_(std::make_shared<const py::bytes>(data)) {
    cl::W3 w = native([&] { return cl::W3(B(*data_), lfa); });
    version = w.version;
    count = w.count;
    for (const cl::W3Entry& e : w.entries) {
        W3Entry r;
        r.name = S(e.name);
        r.off = e.off;
        r.size = e.size;
        entries.append(r);
    }
}

py::list<std::tuple<py::str, LEFile>> W3File::modules() const {
    auto mods = native([&] { return cl::W3(B(*data_), lfa).modules(); });
    return opened(data_, mods);
}

W3File W3File::from_file(const py::str& path) {
    py::bytes d = read_file(path);
    py::bytes at = d.slice(0x3C, 0x40);                              // int.from_bytes(d[0x3C:0x40], "little")
    int64_t lfa = 0;
    for (int64_t k = at.size() - 1; k >= 0; --k) lfa = (lfa << 8) | at[k];
    return W3File(d, lfa);
}

py::list<std::tuple<py::str, LEFile>> open_vxds(const py::str& path) {
    auto data = std::make_shared<const py::bytes>(read_file(path));
    auto pairs = native([&] { return vxds(B(*data), py::os::path::basename(path).utf8()); });
    return opened(data, pairs);
}

// ---- the module-level readers
py::dict<py::str, int64_t> header_fields(const LEFile& lf) {
    auto h = native([&] { return cl::header_fields(image_of(lf)); });
    py::dict<py::str, int64_t> out;
    for (const auto& kv : h) out.set(py::str(kv.first), int64_t(kv.second));
    return out;
}

bool is_member(const LEFile& lf) { return native([&] { return cl::is_member(image_of(lf)); }); }

py::list<std::tuple<int64_t, int64_t>> page_map(const LEFile& lf) {
    auto pm = native([&] { return cl::page_map(image_of(lf)); });
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& e : pm) out.append(std::make_tuple(e.number, int64_t(e.kind)));
    return out;
}

py::bytes object_data(const LEFile& lf, const Object& obj) { return lf.object_bytes(obj); }

std::optional<int64_t> file_offset(const LEFile& lf, int64_t obj_index, int64_t off) {
    return native([&] { return cl::file_offset(image_of(lf), obj_index, off); });
}

py::list<std::tuple<py::str, int64_t>> _names(const py::bytes& d, int64_t p, std::optional<int64_t> end) {
    return name_list(native([&] { return cl::names_at(B(d), p, end); }));
}

std::tuple<NameList, NameList> names(const LEFile& lf) {
    auto r = native([&] { return cl::names(image_of(lf)); });
    return std::make_tuple(name_list(r.first), name_list(r.second));
}

py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> entries(const LEFile& lf) {
    auto es = native([&] { return cl::entries(image_of(lf)); });
    py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> out;
    for (const auto& e : es) out.append(std::make_tuple(e.ordinal, e.obj, e.off, int64_t(e.flags)));
    return out;
}

int64_t _entry_table_size(const LEFile& lf) { return native([&] { return cl::entry_table_size(image_of(lf)); }); }

py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> exports(const LEFile& lf) {
    auto es = native([&] { return cl::exports(image_of(lf)); });
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> out;
    for (const auto& e : es) out.append(std::make_tuple(S(e.name), e.ordinal, e.obj, e.off));
    return out;
}

py::list<std::tuple<int64_t, FixupRecord>> fixup_records(const LEFile& lf) {
    auto rs = native([&] { return cl::fixup_records(image_of(lf)); });
    py::list<std::tuple<int64_t, FixupRecord>> out;
    for (const cl::FixupRecord& r : rs) {
        FixupRecord f;
        f.source = r.source;
        f.source_flags = r.source_flags;
        f.flags = r.flags;
        switch (r.kind) {
        case cl::FixupRecord::Internal:
            f.kind = py::str("internal");
            f.obj = r.first;
            if (r.target) f.target = int64_t(*r.target);
            break;
        case cl::FixupRecord::Import:
            f.kind = py::str("import");
            f.module = r.first;
            f.ordinal = int64_t(r.ordinal);
            break;
        case cl::FixupRecord::ImportName:
            f.kind = py::str("import-name");
            f.module = r.first;
            f.name = int64_t(r.name);
            break;
        case cl::FixupRecord::Entry:
            f.kind = py::str("entry");
            f.entry = r.first;
            break;
        }
        if (r.addend) f.addend = int64_t(*r.addend);
        for (int64_t s : r.sources) f.sources.append(s);
        f.raw = PB(r.raw);
        out.append(std::make_tuple(r.page, f));
    }
    return out;
}

// encode_fixup(rec): the bytes of a fix-up record as fixup_records gives it
py::bytes encode_fixup(const FixupRecord& rec) {
    const int64_t fl = rec.flags;
    const int64_t src = rec.source | rec.source_flags;
    const bool many = src & 0x20;
    if (src < 0 || src > 255 || fl < 0 || fl > 255) throw py::ValueError("byte must be in range(0, 256)");
    std::string out = to_bytes(src, 1) + to_bytes(fl, 1);            // bytearray([src, fl])
    if (many) out += name_byte(size_t(rec.sources.size()));
    else {
        if (rec.sources.empty()) throw py::IndexError("list index out of range");
        pack(out, rec.sources[0], 2, true);
    }
    const int wide = (fl & 0x40) ? 2 : 1;
    const std::string kind = rec.kind.utf8();
    const std::optional<int64_t>* first = kind == "internal" ? &rec.obj : kind == "import" ? &rec.module
                                          : kind == "import-name" ? &rec.module : kind == "entry" ? &rec.entry : nullptr;
    if (!first) key_error(kind);
    const char* first_key = kind == "internal" ? "obj" : kind == "entry" ? "entry" : "module";
    out += to_bytes(field_of(*first, first_key), wide);
    if (kind == "internal" && rec.source != 2)
        out += to_bytes(field_of(rec.target, "target"), (fl & 0x10) ? 4 : 2);
    else if (kind == "import")
        out += to_bytes(field_of(rec.ordinal, "ordinal"), (fl & 0x80) ? 1 : ((fl & 0x10) ? 4 : 2));
    else if (kind == "import-name")
        out += to_bytes(field_of(rec.name, "name"), (fl & 0x10) ? 4 : 2);
    if (fl & 0x04) out += to_bytes(field_of(rec.addend, "addend"), (fl & 0x20) ? 4 : 2);
    if (many)
        for (int64_t s : rec.sources) pack(out, s, 2, true);
    return py::bytes(out);
}

py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>> references(const LEFile& lf) {
    auto rs = native([&] { return cl::references(image_of(lf)); });
    py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>> out;
    for (const auto& r : rs) out.append(std::make_tuple(r.obj, r.off, r.source, r.target_obj, r.target_off));
    return out;
}

py::dict<Place, Place> pointers(const LEFile& lf) {
    auto ps = native([&] { return cl::pointers(image_of(lf)); });
    py::dict<Place, Place> out;
    for (const auto& kv : ps) out.set(Place(kv.first.first, kv.first.second), Place(kv.second.first, kv.second.second));
    return out;
}

bool _plausible(const py::bytes& bytes_, int64_t at) {
    cb::Bytes b = B(bytes_);
    if (at < 0 || at + 0x38 > b.size()) return false;
    cb::Bytes name = b.slice(at + 0x0C, at + 0x14);
    int version = b.at(at + 4) | (b.at(at + 5) << 8);
    bool printable = true;
    for (uint8_t ch : name) printable = printable && ch >= 32 && ch < 127;
    return 0x0300 <= version && version <= 0x04FF && cb::b_isalnum(name.slice(0, 1)) && printable;
}

std::optional<Descriptor> descriptor(const LEFile& lf) {
    return native([&]() -> std::optional<Descriptor> {
        auto d = cl::descriptor(image_of(lf));
        if (!d) return std::nullopt;
        auto place = [&](const char* f) -> std::optional<Place> {
            auto p = d->addresses.at(f);
            return p ? std::optional<Place>(Place(p->first, p->second)) : std::nullopt;
        };
        Descriptor r;
        r.obj = d->obj;
        r.offset = d->offset;
        r.name = S(d->name);
        r.next = int64_t(d->field("next"));
        r.kit_version = int64_t(d->field("kit_version"));
        r.device_id = int64_t(d->field("device_id"));
        r.major = int64_t(d->field("major"));
        r.minor = int64_t(d->field("minor"));
        r.flags = int64_t(d->field("flags"));
        r.init_order = int64_t(d->field("init_order"));
        r.control = place("control");
        r.v86_api = place("v86_api");
        r.pm_api = place("pm_api");
        r.v86_api_address = int64_t(d->field("v86_api_address"));
        r.pm_api_address = int64_t(d->field("pm_api_address"));
        r.reference_data = place("reference_data");
        r.service_table = place("service_table");
        r.service_count = int64_t(d->field("service_count"));
        r.control_raw = int64_t(d->field("control"));
        r.v86_api_raw = int64_t(d->field("v86_api"));
        r.pm_api_raw = int64_t(d->field("pm_api"));
        r.service_table_raw = int64_t(d->field("service_table"));
        r.reference_data_raw = int64_t(d->field("reference_data"));
        r.size = d->size;
        r.bytes = PB(d->bytes);
        return r;
    });
}

py::list<std::optional<Place>> service_table(const LEFile& lf, const std::optional<Descriptor>& given) {
    return native([&]() -> py::list<std::optional<Place>> {
        cl::Image im = image_of(lf);
        std::optional<cl::Place> st;
        int64_t count = 0;
        py::list<std::optional<Place>> out;
        if (given) {                                                 // a descriptor the caller holds
            if (!given->service_table) return out;
            count = given->service_count;
            if (0 < count && count < 0x1000) st = cl::Place{std::get<0>(*given->service_table), std::get<1>(*given->service_table)};
        } else {
            auto d = cl::descriptor(im);
            if (d) {
                st = d->addresses.at("service_table");
                count = int64_t(d->field("service_count"));
            }
        }
        for (const auto& p : cl::service_table(im, st, count))
            out.append(p ? std::optional<Place>(Place(p->first, p->second)) : std::nullopt);
        return out;
    });
}

py::dict<py::str, Place> entry_points(const LEFile& lf) {
    return native([&]() -> py::dict<py::str, Place> {
        cl::Image im = image_of(lf);
        auto d = cl::descriptor(im);
        py::dict<py::str, Place> out;
        if (d) {
            for (const char* f : {"control", "v86_api", "pm_api"}) {
                auto p = d->addresses.at(f);
                if (p) out.set(py::str(f), Place(p->first, p->second));
            }
            auto st = cl::service_table(im, d->addresses.at("service_table"), int64_t(d->field("service_count")));
            for (size_t i = 0; i < st.size(); ++i)
                if (st[i]) out.set(py::str("service_" + std::to_string(i)), Place(st[i]->first, st[i]->second));
        }
        return out;
    });
}

py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, bool>> service_calls(const LEFile& lf) {
    auto cs = native([&] { return cl::service_calls(image_of(lf)); });
    py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, bool>> out;
    for (const auto& s : cs) out.append(std::make_tuple(s.obj, s.off, s.device, s.service, s.jump));
    return out;
}

// ---- read_image (image.cc)
Image read_image(const LEFile& lf_) {
    return native([&]() -> Image {
        cl::Image lf = image_of(lf_);
        const cb::Bytes d = lf.d;
        auto h = cl::header_fields(lf);
        auto H = [&h](const char* n) -> int64_t {
            for (auto& kv : h)
                if (std::string(kv.first) == n) return int64_t(kv.second);
            return 0;
        };
        const bool member = cl::is_member(lf);
        auto pm = cl::page_map(lf);
        int64_t last = 0;
        for (auto& e : pm) last = std::max(last, e.number);
        auto page = [&](int64_t number, uint8_t kind) -> cb::Bytes {
            if (kind == 2 || kind == 3 || !number) return cb::Bytes();
            int64_t off = int64_t(lf.datapages) + (number - 1) * int64_t(lf.pagesize);
            return d.slice(off, off + ((number == last && lf.last_page_bytes) ? lf.last_page_bytes : lf.pagesize));
        };
        std::vector<cb::Bytes> bodies;
        for (auto& e : pm) bodies.push_back(page(e.number, e.kind));
        cb::Bytes kept_last;
        bool kept = false;
        for (size_t k = 0; k < pm.size(); ++k)
            if (pm[k].kind != 2 && pm[k].kind != 3 && !bodies[k].empty()) {
                kept = true;
                kept_last = bodies[k];
            }
        const bool full_last = kept && kept_last.size() == lf.pagesize;
        std::map<int64_t, int64_t> page_of;                          // a page's number -> its first place in the map
        for (int64_t i = int64_t(pm.size()) - 1; i >= 0; --i)
            if (pm[size_t(i)].number) page_of[pm[size_t(i)].number] = i + 1;
        std::vector<int64_t> fix_order;                              // fixups: {page: [record bytes]}, in first-seen order
        std::map<int64_t, std::vector<cb::Bytes>> fix;
        std::vector<cl::FixupRecord> records = cl::fixup_records(lf);
        for (const cl::FixupRecord& r : records) {
            auto it = page_of.find(r.page);
            int64_t key = it != page_of.end() ? it->second : r.page;
            if (!fix.count(key)) fix_order.push_back(key);
            fix[key].push_back(r.raw);
        }
        auto [res, non] = cl::names(lf);
        const int64_t names_end = !non.empty() ? H("nonresident_names") + H("nonresident_size") : 0;
        const int64_t pages_end = int64_t(lf.datapages) +
            (last ? (last - 1) * int64_t(lf.pagesize) + (lf.last_page_bytes ? lf.last_page_bytes : lf.pagesize) : 0);
        int64_t align = 1;
        while (align < 0x1000 && int64_t(lf.datapages) % (align * 2) == 0) align *= 2;
        const int64_t ip = lf.lfa + H("import_names"), ie = lf.lfa + H("fixup_size") + H("fixup_pages");
        const int64_t ets = cl::entry_table_size(lf);
        auto mods = lf.import_modules();

        Image r;
        if (!member) r.stub = PB(d.slice(0, lf.lfa));
        else {
            std::string s = "MZ" + std::string(0x3A, '\0');
            put_le(s, 0x40, 4);
            r.stub = PB(s);
        }
        r.header = PB(d.slice(lf.lfa, lf.lfa + cl::kHeaderSize));
        for (size_t i = 0; i < lf.objects.size(); ++i) {
            const cl::Object& ob = lf.objects[i];
            ImageObject o;
            o.vsize = ob.vsize;
            o.base = ob.base;
            o.flags = ob.flags;
            o.first_page = ob.pmidx;
            o.page_count = ob.pcnt;
            o.reserved = PB(d.slice(lf.objtab + 24 * int64_t(i) + 20, lf.objtab + 24 * int64_t(i) + 24));
            r.objects.append(o);
        }
        for (size_t k = 0; k < pm.size(); ++k) r.pages.append(std::make_tuple(int64_t(pm[k].kind), PB(bodies[k])));
        r.resident = name_list(res);
        r.nonresident = name_list(non);
        r.entry_table = PB(d.slice(lf.entrytab, lf.entrytab + ets));
        for (int64_t k : fix_order) {
            py::list<py::bytes> recs;
            for (cb::Bytes b : fix[k]) recs.append(PB(b));
            r.fixups.set(k, recs);
        }
        for (auto& m : mods) r.import_modules.append(S(m));
        r.import_names = PB(H("import_names") && lf.lfa < ip && ip < ie && ie <= d.size() ? d.slice(ip, ie) : cb::Bytes());
        r.align = align;
        if (!lf.last_page_bytes && full_last) r.last_page_bytes = 0;
        const int64_t nr = H("nonresident_names");
        r.gaps = std::make_tuple(PB(lf.lfa < ie && ie <= int64_t(lf.datapages) && !member ? d.slice(ie, lf.datapages) : cb::Bytes()),
                                 PB(!non.empty() && pages_end <= nr && nr < pages_end + 16 ? d.slice(pages_end, nr) : cb::Bytes()));
        r.after = PB(member ? cb::Bytes() : d.slice(std::max(pages_end, names_end)));
        r.moved.debug_info = H("debug_info");
        r.moved.resource_data = H("resource_data");
        r.moved.from = std::max(pages_end, names_end);
        return r;
    });
}

// _name_table(pairs): bytes as stored, or built from [(name, ordinal)]
py::bytes _name_table(const NameTable& pairs) {
    if (std::holds_alternative<py::bytes>(pairs)) return std::get<py::bytes>(pairs);
    std::string out;
    for (const auto& p : std::get<NameList>(pairs)) {
        std::string n = latin1_of(std::get<0>(p));
        out += name_byte(n.size()) + n;
        pack(out, std::get<1>(p), 2);
    }
    return py::bytes(out + '\0');
}

// object_table(objects)
py::bytes object_table(const py::list<ImageObject>& objects) {
    std::string out;
    for (const ImageObject& o : objects) {
        pack(out, o.vsize, 4);
        pack(out, o.base, 4);
        pack(out, o.flags, 4);
        pack(out, o.first_page, 4);
        pack(out, o.page_count, 4);
        out += o.reserved ? o.reserved->raw() : std::string(4, '\0');
    }
    return py::bytes(out);
}

namespace {
std::pair<std::string, std::string> fixup_tables_(const std::map<int64_t, std::vector<std::string>>& fixups, int64_t pages) {
    std::string table, records;
    for (int64_t pg = 1; pg <= pages; ++pg) {
        pack(table, int64_t(records.size()), 4);
        auto it = fixups.find(pg);
        if (it != fixups.end())
            for (const std::string& r : it->second) records += r;
    }
    pack(table, int64_t(records.size()), 4);
    return {table, records};
}
}  // namespace

// fixup_tables(fixups, pages) -> (page table, records)
std::tuple<py::bytes, py::bytes> fixup_tables(const py::dict<int64_t, py::list<py::bytes>>& fixups, int64_t pages) {
    std::map<int64_t, std::vector<std::string>> m;
    for (const auto& kv : fixups.items()) {
        std::vector<std::string> recs;
        for (const py::bytes& r : std::get<1>(kv)) recs.push_back(r.raw());
        m[std::get<0>(kv)] = recs;
    }
    auto r = fixup_tables_(m, pages);
    return std::make_tuple(py::bytes(r.first), py::bytes(r.second));
}

// ---- write_image (image.cc)
namespace {
const std::map<std::string, int> kFieldAt = {
    {"byte_order", 0x02}, {"word_order", 0x03}, {"level", 0x04}, {"cpu", 0x08}, {"os", 0x0A}, {"version", 0x0C},
    {"module_flags", 0x10}, {"pages", 0x14}, {"eip_object", 0x18}, {"eip", 0x1C}, {"esp_object", 0x20}, {"esp", 0x24},
    {"page_size", 0x28}, {"last_page_bytes", 0x2C}, {"fixup_size", 0x30}, {"loader_size", 0x38},
    {"object_table", 0x40}, {"objects", 0x44}, {"page_map", 0x48}, {"resources", 0x50}, {"resident_names", 0x58},
    {"entry_table", 0x5C}, {"fixup_pages", 0x68}, {"fixup_records", 0x6C}, {"import_modules", 0x70},
    {"import_module_count", 0x74}, {"import_names", 0x78}, {"data_pages", 0x80}, {"preload_pages", 0x84},
    {"nonresident_names", 0x88}, {"nonresident_size", 0x8C}, {"debug_info", 0x98}, {"debug_size", 0x9C},
    {"resource_data", 0xB8}, {"resource_size", 0xBC}, {"device_id", 0xC0}, {"kit_version", 0xC2}};
}  // namespace

py::bytes write_image(const Image& image) {
    std::string hdr = image.header.raw();
    if (int64_t(hdr.size()) < cl::kHeaderSize) hdr.append(size_t(cl::kHeaderSize - int64_t(hdr.size())), '\0');
    const int64_t page_size = native([&] { return int64_t(cb::Bytes(hdr).u32(0x28)); });
    const std::string table = object_table(image.objects).raw();
    const int64_t npages = image.pages.size();
    auto kind_of = [&](int64_t i) { return std::get<0>(image.pages[i]); };
    auto body_of = [&](int64_t i) { return std::get<1>(image.pages[i]).raw(); };
    std::vector<int64_t> stored;
    for (int64_t i = 0; i < npages; ++i) {
        int64_t k = kind_of(i);
        if (k != 2 && k != 3 && !body_of(i).empty()) stored.push_back(i);
    }
    std::vector<std::pair<int64_t, int64_t>> number;                // {place in the map: page number}, in stored order
    std::string pmap;
    if (image.page_map) {
        cb::Bytes p = B(*image.page_map);
        for (int64_t i : stored) number.emplace_back(i, int64_t(p.slice(4 * i, 4 * i + 3).from_be()));
        pmap = image.page_map->raw();
    } else {
        std::map<int64_t, int64_t> num;
        for (size_t n = 0; n < stored.size(); ++n) {
            number.emplace_back(stored[n], int64_t(n) + 1);
            num[stored[n]] = int64_t(n) + 1;
        }
        for (int64_t i = 0; i < npages; ++i) {
            auto it = num.find(i);
            pmap += to_bytes(it != num.end() ? it->second : 0, 3, true);
            pmap += to_bytes(kind_of(i), 1);
        }
    }
    std::map<int64_t, int64_t> at;                                   // {page number: place in the map}, last place wins
    for (auto& kv : number) at[kv.second] = kv.first;
    const int64_t top = at.empty() ? 0 : at.rbegin()->first;
    if (top > 0 && page_size * top > (int64_t(1) << 31)) throw py::MemoryError("");
    std::string data;
    for (int64_t n = 1; n <= top; ++n) {
        auto it = at.find(n);
        if (it == at.end()) {
            data.append(size_t(std::max<int64_t>(page_size, 0)), '\0');
            continue;
        }
        std::string b = body_of(it->second);
        if (n != top && int64_t(b.size()) < page_size) b.append(size_t(page_size - int64_t(b.size())), '\0');
        data += b;
    }
    const int64_t last_len = top ? int64_t(body_of(at[top]).size()) : 0;
    const std::string resident = _name_table(image.resident).raw();
    std::map<int64_t, std::vector<std::string>> fix;
    std::map<int64_t, int64_t> num_of;
    for (auto& kv : number) num_of[kv.first] = kv.second;
    for (const auto& kv : image.fixups.items()) {
        int64_t pg = std::get<0>(kv);
        auto it = num_of.find(pg - 1);
        std::vector<std::string> recs;
        for (const py::bytes& r : std::get<1>(kv)) recs.push_back(r.raw());
        fix[it != num_of.end() ? it->second : pg] = recs;           // (a later page with the same number wins)
    }
    auto [fix_pages, fix_records] = fixup_tables_(fix, npages);
    std::string modules;
    for (const py::str& m : image.import_modules) {
        std::string n = latin1_of(m);
        modules += name_byte(n.size()) + n;
    }
    const std::string tail = image.tables_tail ? image.tables_tail->raw() : "";
    const std::string entry_table = image.entry_table.raw();
    const std::string import_names = image.import_names.raw();
    std::map<std::string, int64_t> where;
    int64_t pos = cl::kHeaderSize;
    const std::pair<const char*, const std::string*> parts[] = {
        {"object_table", &table}, {"page_map", &pmap}, {"resident_names", &resident}, {"entry_table", &entry_table},
        {"fixup_pages", &fix_pages}, {"fixup_records", &fix_records}, {"import_modules", &modules},
        {"import_names", &import_names}, {"tables_tail", &tail}};
    for (auto& p : parts) {
        where[p.first] = pos;
        pos += int64_t(p.second->size());
    }
    const std::string stub = image.stub.raw();
    const int64_t base = int64_t(stub.size());
    int64_t pages_at = image.data_pages ? *image.data_pages : 0;
    if (pages_at < base + pos) {
        const int64_t al = field_of(image.align, "align");
        if (al == 0) throw py::ZeroDivisionError("integer division or modulo by zero");
        pages_at = py::floordiv(base + pos + al - 1, al) * al;
    }
    put_into(hdr, 0x14, npages);
    put_into(hdr, 0x2C, image.last_page_bytes ? *image.last_page_bytes : last_len);
    put_into(hdr, 0x30, int64_t(fix_pages.size() + fix_records.size() + modules.size() + import_names.size()));
    put_into(hdr, 0x38, where["fixup_pages"] - cl::kHeaderSize);
    put_into(hdr, 0x40, where["object_table"]);
    put_into(hdr, 0x44, image.objects.size());
    put_into(hdr, 0x48, where["page_map"]);
    put_into(hdr, 0x58, where["resident_names"]);
    put_into(hdr, 0x5C, where["entry_table"]);
    put_into(hdr, 0x68, where["fixup_pages"]);
    put_into(hdr, 0x6C, where["fixup_records"]);
    put_into(hdr, 0x70, where["import_modules"]);
    put_into(hdr, 0x74, image.import_modules.size());
    put_into(hdr, 0x78, where["import_names"]);
    put_into(hdr, 0x80, pages_at);
    std::string out = stub + hdr + table + pmap + resident + entry_table + fix_pages + fix_records + modules + import_names + tail;
    std::string gap, names_gap;
    if (image.gaps) {
        gap = std::get<0>(*image.gaps).raw();
        names_gap = std::get<1>(*image.gaps).raw();
    }
    if (int64_t(gap.size()) == pages_at - int64_t(out.size())) out += gap;
    else if (pages_at < int64_t(out.size())) throw py::ValueError("negative count");
    else out.append(size_t(pages_at - int64_t(out.size())), '\0');
    out += data;
    const bool has_nonres = std::holds_alternative<py::bytes>(image.nonresident) ? !std::get<py::bytes>(image.nonresident).empty()
                                                                                 : !std::get<NameList>(image.nonresident).empty();
    const std::string non = has_nonres ? _name_table(image.nonresident).raw() : "";
    out += names_gap;
    put_into(hdr, 0x8C, int64_t(non.size()));
    if (!(image.in_pages && image.in_pages->contains(py::str("nonresident_names")))) {   // else already among the data pages
        put_into(hdr, 0x88, non.empty() ? 0 : int64_t(out.size()));
        out += non;
    }
    const std::string after = image.after.raw();
    const int64_t shift = int64_t(out.size()) - image.moved.from;
    for (auto& f : {std::make_pair(0x98, image.moved.debug_info), std::make_pair(0xB8, image.moved.resource_data)}) {
        int64_t v = f.second;
        put_into(hdr, f.first, v && !after.empty() ? v + shift : v);
    }
    if (image.in_pages)
        for (const auto& kv : image.in_pages->items()) {
            const std::string key = std::get<0>(kv).utf8();
            auto it = kFieldAt.find(key);
            if (it == kFieldAt.end()) key_error(key);
            put_into(hdr, it->second, std::get<1>(kv) + pages_at - field_of(image.data_pages, "data_pages"));
        }
    out += after;
    if (image.fill_from && !image.fill_from->empty()) {
        cb::Bytes f = B(*image.fill_from);
        cb::Bytes rest = f.slice(int64_t(out.size()));
        out.append(reinterpret_cast<const char*>(rest.data()), size_t(rest.size()));
    }
    // out[base:base + HEADER_SIZE] = hdr: the slice clamped to the end, so past it the header is appended
    out.replace(size_t(std::min<int64_t>(base, int64_t(out.size()))), size_t(cl::kHeaderSize), hdr);
    return py::bytes(out);
}

// ---- containers
std::optional<Container> container(const py::bytes& data) {
    return native([&]() -> std::optional<Container> {
        auto k = cl::container(B(data));
        if (!k) return std::nullopt;
        Container c;
        c.kind = S(k->kind);
        c.at = k->at;
        c.version = k->version;
        if (k->kind == "W3") c.reserved = PB(k->reserved);
        for (const auto& m : k->members) c.members.append(std::make_tuple(S(std::get<0>(m)), int64_t(std::get<1>(m)), int64_t(std::get<2>(m))));
        return c;
    });
}

py::bytes pack_directory(const Container& c) {
    std::string out = "W3";
    pack(out, c.version, 2);
    pack(out, c.members.size(), 2);
    out += field_of(c.reserved, "reserved").raw();
    for (const auto& m : c.members) {
        std::string n = latin1_of(std::get<0>(m));
        if (n.size() < 8) n.append(8 - n.size(), ' ');
        out += n;
        pack(out, std::get<1>(m), 4);
        pack(out, std::get<2>(m), 4);
    }
    return py::bytes(out);
}

py::bytes expand_page(const py::bytes& buf, int64_t size) { return PB(native([&] { return cl::expand_page(B(buf), size); })); }
py::bytes compress_page(const py::bytes& page) { return PB(native([&] { return cl::compress_page(B(page)); })); }
py::bytes expand(const py::bytes& data) { return PB(native([&] { return cl::expand(B(data)); })); }

py::bytes compress(const py::bytes& data_, const py::bytes& like_) {
    return native([&]() -> py::bytes {
        cb::Bytes data = B(data_), like = B(like_);
        auto c = cl::container(like);
        if (!c) cb::raise("TypeError", "'NoneType' object is not subscriptable");
        const int64_t at = c->at;
        const uint16_t size = like.u16(at + 4);
        if (size == 0) cb::raise("ValueError", "range() arg 3 must not be zero");
        cb::Bytes body = data.slice(at);
        std::vector<std::string> pages;
        for (int64_t i = 0; i < body.size(); i += size) {
            std::string page = body.slice(i, i + size).str();
            if (int64_t(page.size()) < size) page.append(size_t(size - int64_t(page.size())), '\0');
            pages.push_back(cl::compress_page(cb::Bytes(page)));
        }
        std::string head = like.slice(0, at + 0x10).str();
        if (at + 6 + 2 > int64_t(head.size()) || pages.size() > 0xFFFF) throw py::StructError("pack_into out of range");
        head[size_t(at + 6)] = char(pages.size());
        head[size_t(at + 7)] = char(pages.size() >> 8);
        int64_t pos = at + 0x10 + 4 * int64_t(pages.size());
        std::string table;
        for (const std::string& pg : pages) {
            pack(table, pos, 4);
            pos += int64_t(pg.size());
        }
        std::string out = head + table;
        for (const std::string& pg : pages) out += pg;
        return py::bytes(out);
    });
}

py::list<std::tuple<py::str, LEFile>> members(const py::str& path) {
    py::bytes data = read_file(path);
    auto shared = std::make_shared<const py::bytes>(data);
    return native([&]() -> py::list<std::tuple<py::str, LEFile>> {
        auto cont = cl::container(B(*shared));
        if (!cont) return opened(shared, vxds(B(*shared), py::os::path::basename(path).utf8()));
        std::string expanded = cl::expand(B(*shared));
        cb::Bytes d(expanded);
        auto c2 = cl::container(d);
        if (!c2) cb::raise("TypeError", "'NoneType' object is not subscriptable");
        std::vector<std::pair<std::string, int64_t>> mods;
        for (auto& m : c2->members) {
            cb::Bytes s = d.slice(std::get<1>(m), int64_t(std::get<1>(m)) + 2);
            if (s.is("LE", 2) || s.is("LX", 2)) mods.emplace_back(std::get<0>(m), std::get<1>(m));
        }
        // (the expanded bytes where expanding changed them, else the file's own)
        auto use = expanded == shared->raw() ? shared : std::make_shared<const py::bytes>(py::bytes(expanded));
        return opened(use, mods);
    });
}

py::list<py::str> corpus_files() {
    struct Entry {
        int64_t size;
        std::u32string lower;
        py::str path;
    };
    std::vector<Entry> found;
    for (const auto& kv : base::corpus::index().items()) {
        const py::list<py::str>& cands = std::get<1>(kv);
        if (cands.empty()) throw py::IndexError("list index out of range");
        const py::str& first = cands[0];
        if (detect::holds_drivers(first)) found.push_back({0, std::u32string(), first});
    }
    for (Entry& e : found) {
        e.size = py::os::path::getsize(e.path);
        e.lower = e.path.lower().u32();
    }
    std::stable_sort(found.begin(), found.end(), [](const Entry& a, const Entry& b) {
        return a.size != b.size ? a.size < b.size : a.lower < b.lower;
    });
    py::list<py::str> out;
    for (const Entry& e : found) out.append(e.path);
    return out;
}

}  // namespace codebreaker::formats::le
