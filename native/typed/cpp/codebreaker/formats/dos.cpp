// codebreaker/formats/dos.cpp -- typed C++ of codebreaker.formats.dos (see dos.hpp).
//
// The readers are native/formats/dos (dos.cc, unpack.cc, with core/bytes.cc, core/text.cc, core/files.cc,
// core/uc.cc): linked, and called directly with typed results.  The question handlers of
// native/formats/dos/questions.cc (which shaped their answers as pickle) and the Python's `_nd_*` argument
// helpers are the typed functions and conversions below.

#include "codebreaker/formats/dos.hpp"

#include <algorithm>
#include <filesystem>
#include <new>

#include "codebreaker/base/corpus.hpp"
#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/files.h"
#include "../../../../formats/dos/dos.h"

// (uc.h needs Unicorn's headers; only this one entry point of it is used here)
namespace cbf {
int load_unicorn(const wchar_t* path, char* err, size_t errlen);
}

namespace codebreaker::formats::dos {

namespace nd = ::cbf::dos;

const int64_t LOAD = 0x1000;                  // the segment a program is loaded at
const int64_t KERNEL_SEGMENT = 0x0070;        // where the boot sector and the loader put IO.SYS
const int64_t LOADER_SEGMENT = 0x9000;        // where the loader moves itself with 640 KB of memory
const py::list<py::str> EXTENSIONS = py::list<py::str>{py::str(".exe"), py::str(".com"), py::str(".sys"), py::str(".dos"),
                                                       py::str(".mod"), py::str(".new"), py::str(".vxd"), py::str(".386"),
                                                       py::str(".bin"), py::str(".ovl")};
const int64_t EXEPACK_SEGMENT = 0x5000;       // where the EXEPACK unpacker's code is read (it runs behind SYSINIT)

namespace {

// ---- the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const ::cbf::PyError& e) {
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
    throw py::RuntimeError(m);                                // (RuntimeError, and what the bridge made of an unknown name)
}

// runs a call into the C++ reader, its exceptions made the Python's
template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const ::cbf::PyError& e) {
        raise_py(e);
    } catch (const ::cbf::Fallback& f2) {
        throw py::RuntimeError("formats.dll leaves this question to its caller: " + f2.reason);
    } catch (const std::bad_alloc&) {
        throw py::MemoryError(std::string());
    }
}

::cbf::Bytes B(const py::bytes& b) { return ::cbf::Bytes(b.raw()); }
py::str S(const std::string& utf8) { return py::str(utf8); }

py::list<int64_t> ints_of(const std::vector<int64_t>& v) {
    py::list<int64_t> out;
    for (int64_t x : v) out.append(x);
    return out;
}
std::vector<int64_t> vec_of(const py::list<int64_t>& l) {
    std::vector<int64_t> out;
    for (int64_t x : l) out.push_back(x);
    return out;
}

std::optional<py::str> opt_str(const std::optional<std::string>& s) {
    return s ? std::optional<py::str>(S(*s)) : std::nullopt;
}

Mz mz_of(const nd::Mz& h) {
    Mz m;
    m.nreloc = h.nreloc;
    m.header = h.header;
    m.min_alloc = h.min_alloc;
    m.max_alloc = h.max_alloc;
    m.ss = h.ss;
    m.sp = h.sp;
    m.ip = h.ip;
    m.cs = h.cs;
    m.reloc_off = h.reloc_off;
    m.size = h.size;
    return m;
}

Device device_of(const nd::Device& d) {
    return Device(d.offset, d.next, d.attributes, d.strategy, d.interrupt, S(d.name));
}

py::list<Device> devices_of(const std::vector<nd::Device>& ds) {   // _nd_devices: [Device(*t) for t in answer]
    py::list<Device> out;
    for (const nd::Device& d : ds) out.append(device_of(d));
    return out;
}

Unpacked unpacked_of(const nd::Exepack& e) {
    return Unpacked(py::bytes(e.module), ints_of(e.relocs), e.cs, e.ip, e.ss, e.sp);
}

// _program(path, answer): the Program(path, kind) with the attributes the answer carries set
Program program_of(const py::str& path, const nd::Program& n) {
    Program p(path, S(n.kind));
    if (n.name) p.name = S(*n.name);
    p.packer = opt_str(n.packer);
    p.unpacked = opt_str(n.unpacked);
    p.base = n.base;
    p.image = py::bytes(n.image);
    p.spans = py::list<Span>();
    for (const nd::Span& s : n.spans) p.spans.append(Span(s.address, s.length, S(s.label)));
    p.entries = py::list<EntryPoint>();
    for (const nd::Entry& e : n.entries) p.entries.append(EntryPoint(e.address, S(e.name), S(e.kind)));
    p.segments = py::dict<int64_t, int64_t>();
    for (const auto& kv : n.segments) p.segments.set(kv.first, kv.second);
    p.relocations = ints_of(n.relocations);
    p.devices = devices_of(n.devices);
    p.device_segment = n.device_segment;
    p.cs = n.cs;
    p.ip = n.ip;
    p.ss = n.ss;
    p.sp = n.sp;
    p.ds = n.ds;
    p.overlay = py::bytes(n.overlay);
    p.notes = py::list<py::str>();
    for (const std::string& s : n.notes) p.notes.append(S(s));
    p.windows_at = n.windows_at;
    return p;
}

// open(path, "rb").read()
std::string file_bytes(const py::str& path) { return py::open_rb(path).read().raw(); }

// os.path.basename(path).lower()
py::str lower_name(const py::str& path) { return py::os::path::basename(path).lower(); }

}  // namespace

void load_unicorn(const py::str& dll_path) {
    char err[256] = {0};
    std::wstring w = std::filesystem::u8path(dll_path.utf8()).wstring();
    if (::cbf::load_unicorn(w.c_str(), err, sizeof err) != 0) throw py::RuntimeError(std::string(err));
}

// ---- class Device
Device::Device(int64_t offset, const std::optional<int64_t>& nxt, int64_t attributes, int64_t strategy, int64_t interrupt,
               const py::str& name)
    : offset(offset), next(nxt), attributes(attributes), strategy(strategy), interrupt(interrupt), name(name),
      character((attributes & 0x8000) != 0) {}

py::str Device::repr() const {
    return py::percent(py::str("Device(%r at %#x, strategy %#x, interrupt %#x)"), name, offset, strategy, interrupt);
}

// ---- class Program
Program::Program(const py::str& path, const py::str& kind)
    : path(path), kind(kind), name(py::os::path::basename(path).lower()), base(LOAD * 16), device_segment(LOAD), ds(LOAD) {}

bool Program::inside(int64_t linear) const {
    for (const Span& s : spans) {
        int64_t lo = std::get<0>(s), n = std::get<1>(s);
        if (lo <= linear && linear < lo + n) return true;
    }
    return false;
}

std::optional<int64_t> Program::word(int64_t linear) const {
    int64_t i = linear - base;
    if (0 <= i && i <= py::len(image) - 2) return image.byte_(i) | (image.byte_(i + 1) << 8);
    return std::nullopt;
}

// what a file is -------------------------------------------------------------------------------
std::optional<Mz> _mz(const py::bytes& data, bool stub) {
    auto h = native([&] { return nd::mz(B(data), stub); });
    if (!h) return std::nullopt;
    return mz_of(*h);
}

py::list<Device> devices(const py::bytes& image, int64_t at, const py::list<int64_t>& segments, int64_t limit) {
    return devices_of(native([&] { return nd::devices(B(image), at, vec_of(segments), limit); }));
}

std::optional<std::tuple<int64_t, int64_t>> _kernel_body(const py::bytes& data, const Mz& h) {
    nd::Mz n{};
    n.header = h.header;
    n.size = h.size;
    auto r = native([&] { return nd::kernel_body(B(data), n); });
    if (!r) return std::nullopt;
    return std::make_tuple(r->first, r->second);
}

std::optional<py::str> _kind(const py::bytes& data, const py::str& name) {
    return opt_str(native([&] { return nd::kind_of(B(data), name.utf8()); }));
}

py::list<std::tuple<int64_t, int64_t>> chain(const py::bytes& data) {
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& pr : native([&] { return nd::chain(B(data)); })) out.append(std::make_tuple(pr.first, pr.second));
    return out;
}

std::optional<std::tuple<int64_t, int64_t>> driver_and_vxd(const py::bytes& data) {
    auto r = native([&] { return nd::driver_and_vxd(B(data)); });
    if (!r) return std::nullopt;
    return std::make_tuple(r->first, r->second);
}

std::optional<std::tuple<Mz, int64_t>> dos_and_windows(const py::bytes& data) {
    auto r = native([&] { return nd::dos_and_windows(B(data)); });
    if (!r) return std::nullopt;
    return std::make_tuple(mz_of(r->first), r->second);
}

Program driver_part(const py::str& path) {
    std::string data = file_bytes(path);
    py::str name = lower_name(path);
    nd::Program n = native([&] { return nd::driver_part(::cbf::Bytes(data)); });
    n.name = "the real-mode driver of " + name.utf8();
    return program_of(path, n);
}

std::optional<py::str> kind(const py::str& path) {
    // kind_path_q: None for a file that cannot be read
    std::string data;
    const std::string p = path.utf8();
    if (!::cbf::read_file(p, data)) return std::nullopt;
    std::string base = p.substr(p.find_last_of("/\\") == std::string::npos ? 0 : p.find_last_of("/\\") + 1);
    return opt_str(native([&] { return nd::kind_of(::cbf::Bytes(data), base); }));
}

std::optional<py::str> _packer(const py::bytes& data, const Mz& h) {
    nd::Mz n{};
    n.header = h.header;
    n.cs = h.cs;
    n.ip = h.ip;
    return opt_str(native([&] { return nd::packer_of(B(data), n); }));
}

std::optional<py::str> packer(const py::str& path) {
    std::string data = file_bytes(path);
    return opt_str(native([&]() -> std::optional<std::string> {
        auto h = nd::mz(::cbf::Bytes(data));
        if (h) return nd::packer_of(::cbf::Bytes(data), *h);
        return std::nullopt;
    }));
}

py::list<py::str> programs(const py::list<py::str>& extensions) {
    // One path per program name of the corpus that kind() accepts, by name.
    py::dict<py::str, py::list<py::str>> idx = base::corpus::index();
    std::vector<py::str> names = idx.keys();
    std::sort(names.begin(), names.end(), [](const py::str& a, const py::str& b) { return a < b; });
    py::list<py::str> out;
    for (const py::str& name : names) {
        bool ends = false;
        for (const py::str& e : extensions)
            if (name.endswith(e)) ends = true;
        if (!ends) continue;
        py::list<py::str> cands = idx.at(name);
        std::optional<py::str> k = kind(cands[0]);
        if (k && !k->empty()) out.append(cands[0]);
    }
    return out;
}

// EXEPACK --------------------------------------------------------------------------------------
Unpacked unpack_exepack(const py::bytes& module, int64_t cs, int64_t ip) {
    return unpacked_of(native([&] { return nd::unpack_exepack(B(module), cs, ip); }));
}

Unpacked unpack_emulated(const py::bytes& module, const py::list<int64_t>& relocations, int64_t cs, int64_t ip, int64_t ss,
                         int64_t sp, bool moved) {
    return unpacked_of(native([&] { return nd::unpack_emulated(module.raw(), vec_of(relocations), cs, ip, ss, sp, moved); }));
}

py::bytes unpacked_file(const Program& p) {
    nd::Program n;
    n.kind = p.kind.utf8();
    n.image = p.image.raw();
    n.relocations = vec_of(p.relocations);
    n.cs = p.cs;
    n.ss = p.ss;
    n.sp = p.sp;
    n.ip = p.ip;
    n.overlay = p.overlay.raw();
    return py::bytes(native([&] { return nd::unpacked_file(n, p.name.utf8()); }));
}

std::optional<int64_t> _data_segment(const Program& p, int64_t entry) {
    return native([&] { return nd::data_segment(B(p.image), p.base, p.ip, p.kind.utf8(), vec_of(p.relocations), entry); });
}

Program read(const py::str& path, bool run_movers) {
    std::string data = file_bytes(path);
    std::string base = py::os::path::basename(path).utf8();
    return program_of(path, native([&] { return nd::read(::cbf::Bytes(data), base, run_movers); }));
}

Program pklite_head(const py::str& path, int64_t start) {
    std::string data = file_bytes(path);
    py::str name = lower_name(path);
    nd::Program n = native([&] { return nd::pklite_head(::cbf::Bytes(data), start); });
    n.name = "pklite (the entry code and decompressor PKLITE put in front of " + name.utf8() + ")";
    return program_of(path, n);
}

Program exepack_stub(const py::str& path, const build::exepack::Layout& layout) {
    Program stub(path, py::str("kernel part"));
    stub.name = py::percent(py::str("exepack (the unpacker LINK /EXEPACK put behind %s)"), lower_name(path));
    stub.base = EXEPACK_SEGMENT * 16 + 2 * layout.words;       // its code follows the header words
    stub.image = layout.stub_code;
    stub.spans = py::list<Span>{Span(stub.base, py::len(stub.image), py::str("exepack"))};
    stub.cs = EXEPACK_SEGMENT;
    stub.ds = EXEPACK_SEGMENT;
    stub.entries = py::list<EntryPoint>{EntryPoint(stub.base, py::str("unpack"), py::str("far"))};
    stub.segments = py::dict<int64_t, int64_t>{{stub.base, EXEPACK_SEGMENT}};
    return stub;
}

std::tuple<py::list<std::tuple<py::str, Program>>, build::exepack::Layout> kernel_parts(const py::str& path) {
    py::bytes data(file_bytes(path));
    Program whole = read(path);
    if (whole.kind != py::str("kernel")) throw py::ValueError(py::percent(py::str("%s: not a kernel image"), whole.name));
    std::optional<Mz> h_ = _mz(data);
    if (!h_) throw py::TypeError("'NoneType' object is not subscriptable");
    Mz h = *h_;
    auto body = _kernel_body(data, h);
    if (!body) throw py::TypeError("cannot unpack non-iterable NoneType object");
    int64_t start = std::get<0>(*body);
    py::list<std::tuple<py::str, Program>> out;
    struct Part {
        const char* stem;
        int64_t segment, lo, hi;
        const char* label;
    };
    const Part parts[] = {{"msload", LOADER_SEGMENT, 0, start, "loader"},
                          {"msdos", KERNEL_SEGMENT, start, h.header, "bios and dos"}};
    for (const Part& part : parts) {
        Program p(path, py::str("kernel part"));
        p.name = py::percent(py::str("%s (the %s of %s)"), py::str(part.stem), py::str(part.label), whole.name);
        p.base = part.segment * 16;
        p.image = data.slice(part.lo, part.hi);
        p.spans = py::list<Span>{Span(p.base, py::len(p.image), py::str(part.label))};
        p.entries = py::list<EntryPoint>();
        for (const EntryPoint& e : whole.entries)
            if (p.base <= std::get<0>(e) && std::get<0>(e) < p.base + py::len(p.image)) p.entries.append(e);
        p.segments = py::dict<int64_t, int64_t>();
        for (const auto& kv : whole.segments.items()) {
            int64_t va = std::get<0>(kv);
            if (p.base <= va && va < p.base + py::len(p.image)) p.segments.set(va, std::get<1>(kv));
        }
        p.ds = part.segment == KERNEL_SEGMENT ? whole.ds : std::optional<int64_t>(part.segment);
        if (part.segment == KERNEL_SEGMENT) {
            p.devices = whole.devices;
            p.device_segment = whole.device_segment;
        }
        out.append(std::make_tuple(py::str(part.stem), p));
    }
    py::bytes stored = data.slice(h.header, h.size);
    build::exepack::Layout layout = build::exepack::layout(stored, h.cs, h.ip);
    // _program(path, native.call("dos.read_mz", data))
    nd::Program n;
    n.kind = "mz";
    {
        std::optional<nd::Mz> hn = native([&] { return nd::mz(B(data)); });
        if (!hn) throw py::TypeError("'NoneType' object is not subscriptable");
        native([&] { nd::read_mz(B(data), *hn, n, "mz", true); return 0; });
    }
    Program p = program_of(path, n);
    p.name = py::percent(py::str("sysinit (of %s)"), whole.name);
    p.in_kernel = true;                                       // its file is IO.SYS: built into it, not a file of its own
    if (!p.unpacked || *p.unpacked != py::str("static"))
        throw py::ValueError(py::percent(py::str("%s: SYSINIT is not stored as EXEPACK"), whole.name));
    out.append(std::make_tuple(py::str("sysinit"), p));
    out.append(std::make_tuple(py::str("exepack"), exepack_stub(path, layout)));
    return std::make_tuple(out, layout);
}

}  // namespace codebreaker::formats::dos
