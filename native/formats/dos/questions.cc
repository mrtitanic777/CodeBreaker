// questions.cc -- the real-mode program reader's answers, in the shapes codebreaker/formats/dos.py gives them, read
// by cb::formats::dos (native/cb/formats/dos.hpp).  A Program comes back as the dict of its attributes (devices as
// tuples): the shim makes the dos.Program and dos.Device objects from it.
#include <optional>
#include <string>
#include <vector>

#include "../core/cb_bridge.h"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/dos.hpp"

namespace cbf::dos {
namespace {

namespace D = cb::formats::dos;

// A kind as the Python names it, back to the reader's: "mz", "com", any other a driver.
D::Kind kind_from(const std::string &k) {
    for (D::Kind kind : {D::Kind::Com, D::Kind::Mz, D::Kind::Driver, D::Kind::Kernel, D::Kind::KernelPart,
                         D::Kind::DriverWithVxd, D::Kind::DosWithWindows})
        if (k == D::kind_name(kind)) return kind;
    return D::Kind::Driver;
}

void mz_dict(Out &o, const D::MzHeader &h) {
    o.begin_dict();
    o.item("nreloc", h.relocation_count);
    o.item("header", h.header_size);
    o.item("min_alloc", h.min_alloc);
    o.item("max_alloc", h.max_alloc);
    o.item("ss", h.ss);
    o.item("sp", h.sp);
    o.item("ip", h.ip);
    o.item("cs", h.cs);
    o.item("reloc_off", h.relocation_offset);
    o.item("size", int64_t(h.size));
    o.end_dict();
}

void device_tuple(Out &o, const D::Device &d) {
    o.begin_tuple();
    o.integer(d.offset);
    put_opt(o, d.next);
    o.integer(d.attributes);
    o.integer(d.strategy);
    o.integer(d.interrupt);
    o.text(d.name);
    o.end_tuple();
}

void devices_list(Out &o, const std::vector<D::Device> &ds) {
    o.begin_list();
    for (const D::Device &d : ds) device_tuple(o, d);
    o.end_list();
}

void opt_text(Out &o, const char *s) {
    if (s) o.text(s);
    else o.none();
}

void program(Out &o, const D::Program &p, const std::optional<std::string> &name) {
    o.begin_dict();
    if (name) o.item_text("name", *name);
    o.key("packer");
    opt_text(o, p.packer ? D::packer_name(*p.packer) : nullptr);
    o.key("unpacked");
    opt_text(o, !p.unpacked ? nullptr : *p.unpacked == D::Unpacked::Static ? "static" : "emulated");
    o.item("base", p.base);
    o.key("image");
    put(o, p.image);
    o.key("spans");
    o.begin_list();
    for (const D::Span &s : p.spans) {
        o.begin_tuple();
        o.integer(s.address);
        o.integer(s.length);
        o.text(s.label);
        o.end_tuple();
    }
    o.end_list();
    o.key("entries");
    o.begin_list();
    for (const D::Entry &e : p.entries) {
        o.begin_tuple();
        o.integer(e.address);
        o.text(e.name);
        o.text(e.call == D::Entry::Call::Far ? "far" : "near");
        o.end_tuple();
    }
    o.end_list();
    o.key("segments");
    o.begin_dict();
    for (const auto &[address, segment] : p.segments) {
        o.integer(address);
        o.integer(segment);
    }
    o.end_dict();
    o.key("relocations");
    o.begin_list();
    for (uint32_t r : p.relocations) o.integer(r);
    o.end_list();
    o.key("devices");
    devices_list(o, p.devices);
    o.item("device_segment", p.device_segment);
    o.item("cs", p.cs);
    o.item("ip", p.ip);
    o.item("ss", p.ss);
    o.item("sp", p.sp);
    o.key("ds");
    put_opt(o, p.ds);
    o.key("overlay");
    put(o, p.overlay);
    o.key("notes");
    o.begin_list();
    for (const std::string &n : p.notes) o.text(n);
    o.end_list();
    if (p.windows_at) o.item("windows_at", int64_t(*p.windows_at));
    o.end_dict();
}

// the result as (kind, attributes): the shim makes Program(path, kind) and sets the rest
void program_answer(Out &o, const D::Program &p, const std::optional<std::string> &name = std::nullopt) {
    o.begin_tuple();
    o.text(D::kind_name(p.kind));
    program(o, p, name);
    o.end_tuple();
}

std::vector<uint32_t> ints(const Value &v) {
    std::vector<uint32_t> out;
    for (const Value &x : v.items) out.push_back(uint32_t(x.as_int()));
    return out;
}

void unpack_answer(Out &o, const D::Unpack &e) {
    o.begin_tuple();
    put(o, e.module);
    o.begin_list();
    for (uint32_t r : e.relocations) o.integer(r);
    o.end_list();
    o.integer(e.cs);
    o.integer(e.ip);
    o.integer(e.ss);
    o.integer(e.sp);
    o.end_tuple();
}

void pair(Out &o, uint64_t a, uint64_t b) {
    o.begin_tuple();
    o.integer(int64_t(a));
    o.integer(int64_t(b));
    o.end_tuple();
}

void mz_q(Call &c) {
    auto h = D::mz_header(view(c.data), c.bool_arg(0));
    if (h) mz_dict(c.out, *h);
    else c.out.none();
}

void devices_q(Call &c) {
    std::vector<uint32_t> segs = {0, 0xFFFF};
    if (c.argc() > 1) segs = ints(c.arg(1));
    devices_list(c.out, D::devices(view(c.data), uint64_t(c.int_arg(0, 0)), segs, size_t(c.int_arg(2, 64))));
}

void kernel_body_q(Call &c) {
    const Value &hv = c.arg(0);
    D::MzHeader h;
    h.header_size = uint32_t(key_int(hv, "header"));
    h.size = uint64_t(key_int(hv, "size"));
    auto r = D::kernel_body(view(c.data), h);
    if (r) pair(c.out, r->first, r->second);
    else c.out.none();
}

void kind_q(Call &c) {
    auto k = D::kind_of(view(c.data), c.str_arg(0));
    opt_text(c.out, k ? D::kind_name(*k) : nullptr);
}

// kind(path): None for a file that cannot be read
void kind_path_q(Call &c) {
    const std::filesystem::path path = path_of(c.str_arg(0));
    cb::Bytes data;
    try {
        data = cb::fs::read_file(path);
    } catch (const cb::IoError &) {
        c.out.none();
        return;
    }
    auto k = D::kind_of(data, cb::fs::to_text(path.filename()));
    opt_text(c.out, k ? D::kind_name(*k) : nullptr);
}

void chain_q(Call &c) {
    c.out.begin_list();
    for (auto &p : D::chain(view(c.data))) pair(c.out, p.first, p.second);
    c.out.end_list();
}

void driver_and_vxd_q(Call &c) {
    auto r = D::driver_and_vxd(view(c.data));
    if (r) pair(c.out, r->first, r->second);
    else c.out.none();
}

void dos_and_windows_q(Call &c) {
    auto r = D::dos_and_windows(view(c.data));
    if (!r) {
        c.out.none();
        return;
    }
    c.out.begin_tuple();
    mz_dict(c.out, r->first);
    c.out.integer(int64_t(r->second));
    c.out.end_tuple();
}

void packer_q(Call &c) {
    const Value &hv = c.arg(0);
    D::MzHeader h;
    h.header_size = uint32_t(key_int(hv, "header"));
    h.cs = uint16_t(key_int(hv, "cs"));
    h.ip = uint16_t(key_int(hv, "ip"));
    auto p = D::packer_of(view(c.data), h);
    opt_text(c.out, p ? D::packer_name(*p) : nullptr);
}

void packer_path_q(Call &c) {
    auto h = D::mz_header(view(c.data));
    auto p = h ? D::packer_of(view(c.data), *h) : std::nullopt;
    opt_text(c.out, p ? D::packer_name(*p) : nullptr);
}

void unpack_exepack_q(Call &c) {
    unpack_answer(c.out, D::unpack_exepack(view(c.data), uint32_t(c.int_arg(0)), uint32_t(c.int_arg(1))));
}

// unpack_emulated(module, relocations, cs, ip, ss, sp, moved=False): the unpacker run (emulate/unpack.py's unpacked)
void unpack_emulated_q(Call &c) {
    unpack_answer(c.out, D::unpack_emulated(view(c.data), ints(c.arg(0)), uint32_t(c.int_arg(1)), uint32_t(c.int_arg(2)),
                                            uint32_t(c.int_arg(3)), uint32_t(c.int_arg(4)), c.bool_arg(5, false)));
}

void data_segment_q(Call &c) {
    put_opt(c.out, D::data_segment(view(c.data), uint32_t(c.int_arg(0)), uint32_t(c.int_arg(1)), kind_from(c.str_arg(2)),
                                   ints(c.arg(3)), uint32_t(c.int_arg(4))));
}

// unpacked_file(p): (kind, name, relocations, cs, ss, sp, ip, overlay) beside the image
void unpacked_file_q(Call &c) {
    D::Program p;
    p.kind = kind_from(c.str_arg(0));
    p.image = owned(c.data);
    p.relocations = ints(c.arg(2));
    p.cs = uint32_t(c.int_arg(3));
    p.ss = uint32_t(c.int_arg(4));
    p.sp = uint32_t(c.int_arg(5));
    p.ip = uint32_t(c.int_arg(6));
    p.overlay = blob(c.arg(7));
    put(c.out, D::unpacked_file(p, c.str_arg(1)));
}

// _read_mz(data, _mz(data), Program(path, "mz"), "mz"): the MZ program an image's header describes
// (kernel_parts: IO.SYS's SYSINIT)
void read_mz_q(Call &c) {
    if (!D::mz_header(view(c.data))) raise("TypeError", "'NoneType' object is not subscriptable");
    D::Program p = D::read_load_module(view(c.data), true);
    p.kind = D::Kind::Mz;
    program_answer(c.out, p);
}

void read_q(Call &c) { program_answer(c.out, D::read_program(view(c.data), c.str_arg(0), c.bool_arg(1, true))); }

void driver_part_q(Call &c) {
    program_answer(c.out, D::read_driver_part(view(c.data)), "the real-mode driver of " + c.str_arg(0));
}

void pklite_head_q(Call &c) {
    program_answer(c.out, D::read_pklite_head(view(c.data), uint64_t(c.int_arg(0))),
                   "pklite (the entry code and decompressor PKLITE put in front of " + c.str_arg(1) + ")");
}

}  // namespace

CBF_QUESTIONS(dos) {
    add("dos._mz", mz_q, "MzHeader{nreloc: u16, header: u32, min_alloc: u16, max_alloc: u16, ss: u16, sp: u16, ip: u16, cs: u16, reloc_off: u16, size: u32}?");
    add("dos.devices", devices_q, "list[(int, int?, int, int, int, str)]");
    add("dos._kernel_body", kernel_body_q, "(u32, u32)?");
    add("dos._kind", kind_q, "str?");
    add("dos.kind", kind_path_q, "str?");
    add("dos.chain", chain_q, "list[(u32, u32)]");
    add("dos.driver_and_vxd", driver_and_vxd_q, "(u32, u32)?");
    add("dos.dos_and_windows", dos_and_windows_q, "(MzHeader{nreloc: u16, header: u32, min_alloc: u16, max_alloc: u16, ss: u16, sp: u16, ip: u16, cs: u16, reloc_off: u16, size: u32}, u32)?");
    add("dos._packer", packer_q, "str?");
    add("dos.packer", packer_path_q, "str?");
    add("dos.unpack_exepack", unpack_exepack_q, "(bytes, list[u32], u16, u16, u16, u16)");
    add("dos.unpack_emulated", unpack_emulated_q, "(bytes, list[u32], u16, u16, u16, u16)");
    add("dos._data_segment", data_segment_q, "int?");
    add("dos.unpacked_file", unpacked_file_q, "bytes");
    add("dos.read", read_q, "(str, DosProgram{name?: str, packer: str?, unpacked: str?, base: int, image: bytes, spans: list[(int, int, str)], entries: list[(int, str, str)], segments: dict[int, int], relocations: list[int], devices: list[(int, int?, int, int, int, str)], device_segment: int, cs: int, ip: int, ss: int, sp: int, ds: int?, overlay: bytes, notes: list[str], windows_at?: int})");
    add("dos.read_mz", read_mz_q, "(str, DosProgram{name?: str, packer: str?, unpacked: str?, base: int, image: bytes, spans: list[(int, int, str)], entries: list[(int, str, str)], segments: dict[int, int], relocations: list[int], devices: list[(int, int?, int, int, int, str)], device_segment: int, cs: int, ip: int, ss: int, sp: int, ds: int?, overlay: bytes, notes: list[str], windows_at?: int})");
    add("dos.driver_part", driver_part_q, "(str, DosProgram{name?: str, packer: str?, unpacked: str?, base: int, image: bytes, spans: list[(int, int, str)], entries: list[(int, str, str)], segments: dict[int, int], relocations: list[int], devices: list[(int, int?, int, int, int, str)], device_segment: int, cs: int, ip: int, ss: int, sp: int, ds: int?, overlay: bytes, notes: list[str], windows_at?: int})");
    add("dos.pklite_head", pklite_head_q, "(str, DosProgram{name?: str, packer: str?, unpacked: str?, base: int, image: bytes, spans: list[(int, int, str)], entries: list[(int, str, str)], segments: dict[int, int], relocations: list[int], devices: list[(int, int?, int, int, int, str)], device_segment: int, cs: int, ip: int, ss: int, sp: int, ds: int?, overlay: bytes, notes: list[str], windows_at?: int})");
}

}  // namespace cbf::dos
