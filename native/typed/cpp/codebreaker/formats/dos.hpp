// codebreaker/formats/dos.hpp -- COMPAT shim for the old typed port of codebreaker.formats.dos (deleted when its last py:: consumer is
// refactored). The reader is native/cb/formats/dos.hpp; this header only converts the records and keeps the old names. No logic,
// except the three functions that depend on code that is not refactored yet and stay here tied to it:
//   COMPAT(py): programs        -- remove when base.corpus is refactored (the corpus index)
//   COMPAT(py): exepack_stub    -- remove when build.exepack is refactored (its Layout)
//   COMPAT(py): kernel_parts    -- remove when build.exepack is refactored
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/dos.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/exepack.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::dos {

namespace cd = cb::formats::dos;

inline const int64_t LOAD = 0x1000;            // the segment a program is loaded at
inline const int64_t KERNEL_SEGMENT = 0x0070;  // where the boot sector and the loader put IO.SYS
inline const int64_t LOADER_SEGMENT = 0x9000;  // where the loader moves itself with 640 KB of memory
inline const py::list<py::str> EXTENSIONS = py::list<py::str>{py::str(".exe"), py::str(".com"), py::str(".sys"), py::str(".dos"), py::str(".mod"),
                                                              py::str(".new"), py::str(".vxd"), py::str(".386"), py::str(".bin"), py::str(".ovl")};
inline const int64_t EXEPACK_SEGMENT = 0x5000;  // where the EXEPACK unpacker's code is read (it runs behind SYSINIT)

// The reader finds Unicorn beside the executable now (cb::emu::unicorn); nothing to load from a path.
inline void load_unicorn(const py::str&) {}

struct Mz {
    int64_t nreloc = 0, header = 0, min_alloc = 0, max_alloc = 0, ss = 0, sp = 0, ip = 0, cs = 0, reloc_off = 0, size = 0;
};

class Device {
  public:
    int64_t offset;
    std::optional<int64_t> next;  // the link to the next header (None: the chain ends)
    int64_t attributes, strategy, interrupt;
    py::str name;
    bool character;

    Device(int64_t offset_, const std::optional<int64_t>& nxt, int64_t attributes_, int64_t strategy_, int64_t interrupt_, const py::str& name_)
        : offset(offset_), next(nxt), attributes(attributes_), strategy(strategy_), interrupt(interrupt_), name(name_), character((attributes_ & 0x8000) != 0) {}
    py::str repr() const { return py::percent(py::str("Device(%r at %#x, strategy %#x, interrupt %#x)"), name, offset, strategy, interrupt); }
};

using Span = std::tuple<int64_t, int64_t, py::str>;        // (linear address, length, label)
using EntryPoint = std::tuple<int64_t, py::str, py::str>;  // (linear address, name, "far" | "near")

class Program {
  public:
    py::str path, kind, name;
    std::optional<py::str> packer;    // how the load module was stored
    std::optional<py::str> unpacked;  // "static" | "emulated" when it was unpacked
    int64_t base;
    py::bytes image;
    py::list<Span> spans;
    py::list<EntryPoint> entries;
    py::dict<int64_t, int64_t> segments;  // entry address -> the code segment it runs with
    py::list<int64_t> relocations;
    py::list<Device> devices;
    int64_t device_segment;
    int64_t cs = 0, ip = 0, ss = 0, sp = 0;
    std::optional<int64_t> ds;
    py::bytes overlay;
    py::list<py::str> notes;
    std::optional<int64_t> windows_at;
    bool in_kernel = false;  // SYSINIT of kernel_parts: built into IO.SYS, not a file of its own

    Program(const py::str& path_, const py::str& kind_)
        : path(path_), kind(kind_), name(py::os::path::basename(path_).lower()), base(LOAD * 16), device_segment(LOAD), ds(LOAD) {}

    bool inside(int64_t linear) const {
        for (const Span& s : spans) {
            const int64_t lo = std::get<0>(s), n = std::get<1>(s);
            if (lo <= linear && linear < lo + n) return true;
        }
        return false;
    }
    std::optional<int64_t> word(int64_t linear) const {
        const int64_t i = linear - base;
        if (0 <= i && i <= py::len(image) - 2) return image.byte_(i) | (image.byte_(i + 1) << 8);
        return std::nullopt;
    }
};

using Unpacked = std::tuple<py::bytes, py::list<int64_t>, int64_t, int64_t, int64_t, int64_t>;

namespace conv {
inline Mz mz(const cd::MzHeader& h) {
    return Mz{h.relocation_count, h.header_size, h.min_alloc, h.max_alloc, h.ss, h.sp, h.ip, h.cs, h.relocation_offset, static_cast<int64_t>(h.size)};
}
inline cd::MzHeader mz(const Mz& m) {
    cd::MzHeader h;
    h.header_size = static_cast<uint32_t>(m.header), h.size = static_cast<uint64_t>(m.size), h.cs = static_cast<uint16_t>(m.cs), h.ip = static_cast<uint16_t>(m.ip);
    h.relocation_count = static_cast<uint16_t>(m.nreloc), h.min_alloc = static_cast<uint16_t>(m.min_alloc), h.max_alloc = static_cast<uint16_t>(m.max_alloc);
    h.ss = static_cast<uint16_t>(m.ss), h.sp = static_cast<uint16_t>(m.sp), h.relocation_offset = static_cast<uint16_t>(m.reloc_off);
    return h;
}
inline Device device(const cd::Device& d) {
    return Device(d.offset, d.next ? std::optional<int64_t>(*d.next) : std::nullopt, d.attributes, d.strategy, d.interrupt, cb::compat::to_py(d.name));
}
inline py::list<Device> devices(const std::vector<cd::Device>& ds) {
    py::list<Device> out;
    for (const auto& d : ds) out.append(device(d));
    return out;
}
inline py::list<int64_t> ints(const std::vector<uint32_t>& v) {
    py::list<int64_t> out;
    for (uint32_t x : v) out.append(x);
    return out;
}
inline std::vector<uint32_t> ints(const py::list<int64_t>& l) {
    std::vector<uint32_t> out;
    for (int64_t x : l) out.push_back(static_cast<uint32_t>(x));
    return out;
}
inline Unpacked unpacked(const cd::Unpack& u) {
    return Unpacked(cb::compat::to_py(cb::ByteView(u.module)), ints(u.relocations), u.cs, u.ip, u.ss, u.sp);
}
inline std::optional<py::str> opt(const std::optional<const char*>& s) { return s ? std::optional<py::str>(py::str(*s)) : std::nullopt; }
// _program(path, answer): the Program(path, kind) with the attributes the answer carries set
inline Program program(const py::str& path, const cd::Program& n) {
    Program p(path, py::str(cd::kind_name(n.kind)));
    if (n.packer) p.packer = py::str(cd::packer_name(*n.packer));
    if (n.unpacked) p.unpacked = py::str(*n.unpacked == cd::Unpacked::Static ? "static" : "emulated");
    p.base = n.base;
    p.image = cb::compat::to_py(cb::ByteView(n.image));
    for (const auto& s : n.spans) p.spans.append(Span(s.address, s.length, cb::compat::to_py(s.label)));
    for (const auto& e : n.entries) p.entries.append(EntryPoint(e.address, cb::compat::to_py(e.name), py::str(e.call == cd::Entry::Call::Far ? "far" : "near")));
    for (const auto& kv : n.segments) p.segments.set(kv.first, kv.second);
    p.relocations = ints(n.relocations);
    p.devices = devices(n.devices);
    p.device_segment = n.device_segment;
    p.cs = n.cs, p.ip = n.ip, p.ss = n.ss, p.sp = n.sp;
    p.ds = n.ds ? std::optional<int64_t>(*n.ds) : std::nullopt;
    p.overlay = cb::compat::to_py(cb::ByteView(n.overlay));
    for (const auto& s : n.notes) p.notes.append(cb::compat::to_py(s));
    if (n.windows_at) p.windows_at = static_cast<int64_t>(*n.windows_at);
    return p;
}
inline cb::Bytes file(const py::str& path) { return cb::fs::read_file(cb::fs::from_text(path.utf8())); }
inline py::str lower_name(const py::str& path) { return py::os::path::basename(path).lower(); }
}  // namespace conv

inline std::optional<Mz> _mz(const py::bytes& data, bool stub = false) {
    return cb::compat::guard([&]() -> std::optional<Mz> {
        auto h = cd::mz_header(cb::compat::from_py(data), stub);
        return h ? std::optional<Mz>(conv::mz(*h)) : std::nullopt;
    });
}

inline py::list<Device> devices(const py::bytes& image, int64_t at = 0, const py::list<int64_t>& segments = py::list<int64_t>{0, 0xFFFF}, int64_t limit = 64) {
    return cb::compat::guard([&] { return conv::devices(cd::devices(cb::compat::from_py(image), static_cast<uint64_t>(at), conv::ints(segments), static_cast<size_t>(limit))); });
}

inline std::optional<std::tuple<int64_t, int64_t>> _kernel_body(const py::bytes& data, const Mz& h) {
    return cb::compat::guard([&]() -> std::optional<std::tuple<int64_t, int64_t>> {
        auto r = cd::kernel_body(cb::compat::from_py(data), conv::mz(h));
        if (!r) return std::nullopt;
        return std::make_tuple(static_cast<int64_t>(r->first), static_cast<int64_t>(r->second));
    });
}

inline std::optional<py::str> _kind(const py::bytes& data, const py::str& name = py::str("")) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        auto k = cd::kind_of(cb::compat::from_py(data), name.utf8());
        return k ? std::optional<py::str>(py::str(cd::kind_name(*k))) : std::nullopt;
    });
}

inline py::list<std::tuple<int64_t, int64_t>> chain(const py::bytes& data) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& p : cd::chain(cb::compat::from_py(data))) out.append(std::make_tuple(static_cast<int64_t>(p.first), static_cast<int64_t>(p.second)));
        return out;
    });
}

inline std::optional<std::tuple<int64_t, int64_t>> driver_and_vxd(const py::bytes& data) {
    return cb::compat::guard([&]() -> std::optional<std::tuple<int64_t, int64_t>> {
        auto r = cd::driver_and_vxd(cb::compat::from_py(data));
        if (!r) return std::nullopt;
        return std::make_tuple(static_cast<int64_t>(r->first), static_cast<int64_t>(r->second));
    });
}

inline std::optional<std::tuple<Mz, int64_t>> dos_and_windows(const py::bytes& data) {
    return cb::compat::guard([&]() -> std::optional<std::tuple<Mz, int64_t>> {
        auto r = cd::dos_and_windows(cb::compat::from_py(data));
        if (!r) return std::nullopt;
        return std::make_tuple(conv::mz(r->first), static_cast<int64_t>(r->second));
    });
}

inline Program driver_part(const py::str& path) {
    return cb::compat::guard([&] {
        Program p = conv::program(path, cd::read_driver_part(conv::file(path)));
        p.name = py::str("the real-mode driver of " + conv::lower_name(path).utf8());
        return p;
    });
}

inline std::optional<py::str> kind(const py::str& path) {
    try {
        auto k = cd::kind_of(conv::file(path), cb::fs::from_text(path.utf8()).filename().string());
        return k ? std::optional<py::str>(py::str(cd::kind_name(*k))) : std::nullopt;
    } catch (const cb::IoError&) {  // None for a file that cannot be read
        return std::nullopt;
    }
}

inline std::optional<py::str> _packer(const py::bytes& data, const Mz& h) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        auto p = cd::packer_of(cb::compat::from_py(data), conv::mz(h));
        return p ? std::optional<py::str>(py::str(cd::packer_name(*p))) : std::nullopt;
    });
}

inline std::optional<py::str> packer(const py::str& path) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        const cb::Bytes d = conv::file(path);
        auto h = cd::mz_header(d);
        if (!h) return std::nullopt;
        auto p = cd::packer_of(d, *h);
        return p ? std::optional<py::str>(py::str(cd::packer_name(*p))) : std::nullopt;
    });
}

// COMPAT(py): remove when base.corpus is refactored
inline py::list<py::str> programs(const py::list<py::str>& extensions = EXTENSIONS) {
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

inline Unpacked unpack_exepack(const py::bytes& module, int64_t cs, int64_t ip) {
    return cb::compat::guard([&] { return conv::unpacked(cd::unpack_exepack(cb::compat::from_py(module), static_cast<uint32_t>(cs), static_cast<uint32_t>(ip))); });
}

inline Unpacked unpack_emulated(const py::bytes& module, const py::list<int64_t>& relocations, int64_t cs, int64_t ip, int64_t ss, int64_t sp, bool moved = false) {
    return cb::compat::guard([&] {
        return conv::unpacked(cd::unpack_emulated(cb::compat::from_py(module), conv::ints(relocations), static_cast<uint32_t>(cs), static_cast<uint32_t>(ip),
                                                  static_cast<uint32_t>(ss), static_cast<uint32_t>(sp), moved));
    });
}

inline py::bytes unpacked_file(const Program& p) {
    return cb::compat::guard([&] {
        cd::Program n;
        const std::string k = p.kind.utf8();
        n.kind = k == "mz" ? cd::Kind::Mz : k == "com" ? cd::Kind::Com : cd::Kind::Driver;
        n.image = cb::compat::from_py(p.image);
        n.relocations = conv::ints(p.relocations);
        n.cs = static_cast<uint32_t>(p.cs), n.ss = static_cast<uint32_t>(p.ss), n.sp = static_cast<uint32_t>(p.sp), n.ip = static_cast<uint32_t>(p.ip);
        n.overlay = cb::compat::from_py(p.overlay);
        return cb::compat::to_py(cb::ByteView(cd::unpacked_file(n, p.name.utf8())));
    });
}

inline std::optional<int64_t> _data_segment(const Program& p, int64_t entry) {
    return cb::compat::guard([&]() -> std::optional<int64_t> {
        const std::string k = p.kind.utf8();
        auto r = cd::data_segment(cb::compat::from_py(p.image), static_cast<uint32_t>(p.base), static_cast<uint32_t>(p.ip),
                                  k == "mz" ? cd::Kind::Mz : k == "com" ? cd::Kind::Com : cd::Kind::Driver, conv::ints(p.relocations), static_cast<uint32_t>(entry));
        return r ? std::optional<int64_t>(*r) : std::nullopt;
    });
}

inline Program read(const py::str& path, bool run_movers = true) {
    return cb::compat::guard([&] {
        return conv::program(path, cd::read_program(conv::file(path), cb::fs::from_text(path.utf8()).filename().string(), run_movers));
    });
}

inline Program pklite_head(const py::str& path, int64_t start) {
    return cb::compat::guard([&] {
        Program p = conv::program(path, cd::read_pklite_head(conv::file(path), static_cast<uint64_t>(start)));
        p.name = py::str("pklite (the entry code and decompressor PKLITE put in front of " + conv::lower_name(path).utf8() + ")");
        return p;
    });
}

// COMPAT(py): remove when build.exepack is refactored
inline Program exepack_stub(const py::str& path, const build::exepack::Layout& layout) {
    Program stub(path, py::str("kernel part"));
    stub.name = py::percent(py::str("exepack (the unpacker LINK /EXEPACK put behind %s)"), conv::lower_name(path));
    stub.base = EXEPACK_SEGMENT * 16 + 2 * layout.words;  // its code follows the header words
    stub.image = layout.stub_code;
    stub.spans = py::list<Span>{Span(stub.base, py::len(stub.image), py::str("exepack"))};
    stub.cs = EXEPACK_SEGMENT;
    stub.ds = EXEPACK_SEGMENT;
    stub.entries = py::list<EntryPoint>{EntryPoint(stub.base, py::str("unpack"), py::str("far"))};
    stub.segments = py::dict<int64_t, int64_t>{{stub.base, EXEPACK_SEGMENT}};
    return stub;
}

// COMPAT(py): remove when build.exepack is refactored
inline std::tuple<py::list<std::tuple<py::str, Program>>, build::exepack::Layout> kernel_parts(const py::str& path) {
    py::bytes data(cb::compat::to_py(cb::ByteView(conv::file(path))));
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
    const Part parts[] = {{"msload", LOADER_SEGMENT, 0, start, "loader"}, {"msdos", KERNEL_SEGMENT, start, h.header, "bios and dos"}};
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
    Program p = cb::compat::guard([&] { return conv::program(path, cd::read_load_module(conv::file(path))); });
    p.name = py::percent(py::str("sysinit (of %s)"), whole.name);
    p.in_kernel = true;  // its file is IO.SYS: built into it, not a file of its own
    if (!p.unpacked || *p.unpacked != py::str("static")) throw py::ValueError(py::percent(py::str("%s: SYSINIT is not stored as EXEPACK"), whole.name));
    out.append(std::make_tuple(py::str("sysinit"), p));
    out.append(std::make_tuple(py::str("exepack"), exepack_stub(path, layout)));
    return std::make_tuple(out, layout);
}

}  // namespace codebreaker::formats::dos
