// questions.cc -- the real-mode program reader's answers, in the shapes codebreaker/formats/dos.py
// gives them.  A Program comes back as the dict of its attributes (devices as tuples): the shim
// makes the dos.Program and dos.Device objects from it.
#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "dos.h"

namespace cbf::dos {
namespace {

void mz_dict(Out &o, const Mz &h) {
    o.begin_dict();
    o.item("nreloc", h.nreloc);
    o.item("header", h.header);
    o.item("min_alloc", h.min_alloc);
    o.item("max_alloc", h.max_alloc);
    o.item("ss", h.ss);
    o.item("sp", h.sp);
    o.item("ip", h.ip);
    o.item("cs", h.cs);
    o.item("reloc_off", h.reloc_off);
    o.item("size", h.size);
    o.end_dict();
}

void device_tuple(Out &o, const Device &d) {
    o.begin_tuple();
    o.integer(d.offset);
    if (d.next) o.integer(*d.next);
    else o.none();
    o.integer(d.attributes);
    o.integer(d.strategy);
    o.integer(d.interrupt);
    o.text(d.name);
    o.end_tuple();
}

void devices_list(Out &o, const std::vector<Device> &ds) {
    o.begin_list();
    for (const Device &d : ds) device_tuple(o, d);
    o.end_list();
}

void opt_text(Out &o, const std::optional<std::string> &s) {
    if (s) o.text(*s);
    else o.none();
}

void program(Out &o, const Program &p) {
    o.begin_dict();
    if (p.name) o.item_text("name", *p.name);
    o.key("packer");
    opt_text(o, p.packer);
    o.key("unpacked");
    opt_text(o, p.unpacked);
    o.item("base", p.base);
    o.key("image");
    o.bytes(Bytes(p.image));
    o.key("spans");
    o.begin_list();
    for (const Span &s : p.spans) {
        o.begin_tuple();
        o.integer(s.address);
        o.integer(s.length);
        o.text(s.label);
        o.end_tuple();
    }
    o.end_list();
    o.key("entries");
    o.begin_list();
    for (const Entry &e : p.entries) {
        o.begin_tuple();
        o.integer(e.address);
        o.text(e.name);
        o.text(e.kind);
        o.end_tuple();
    }
    o.end_list();
    o.key("segments");
    o.begin_dict();
    for (auto &kv : p.segments) {
        o.integer(kv.first);
        o.integer(kv.second);
    }
    o.end_dict();
    o.key("relocations");
    o.begin_list();
    for (int64_t r : p.relocations) o.integer(r);
    o.end_list();
    o.key("devices");
    devices_list(o, p.devices);
    o.item("device_segment", p.device_segment);
    o.item("cs", p.cs);
    o.item("ip", p.ip);
    o.item("ss", p.ss);
    o.item("sp", p.sp);
    o.key("ds");
    if (p.ds) o.integer(*p.ds);
    else o.none();
    o.key("overlay");
    o.bytes(Bytes(p.overlay));
    o.key("notes");
    o.begin_list();
    for (auto &n : p.notes) o.text(n);
    o.end_list();
    if (p.windows_at) o.item("windows_at", *p.windows_at);
    o.end_dict();
}

// the result as (kind, attributes): the shim makes Program(path, kind) and sets the rest
void program_answer(Out &o, const Program &p) {
    o.begin_tuple();
    o.text(p.kind);
    program(o, p);
    o.end_tuple();
}

int64_t key(const Value &h, const char *k) {
    if (h.kind != Value::Dict) raise("TypeError", "a header that is not a dict");
    const Value *v = h.get(k);
    if (!v) raise("KeyError", std::string("'") + k + "'");
    return v->as_int();
}

void mz_q(Call &c) {
    auto h = mz(c.data, c.bool_arg(0));
    if (h) mz_dict(c.out, *h);
    else c.out.none();
}

void devices_q(Call &c) {
    std::vector<int64_t> segs = {0, 0xFFFF};
    if (c.argc() > 1) {
        segs.clear();
        for (const Value &v : c.arg(1).items) segs.push_back(v.as_int());
    }
    devices_list(c.out, devices(c.data, c.int_arg(0, 0), segs, c.int_arg(2, 64)));
}

void kernel_body_q(Call &c) {
    const Value &hv = c.arg(0);
    Mz h{};
    h.header = key(hv, "header");
    h.size = key(hv, "size");
    auto r = kernel_body(c.data, h);
    if (!r) {
        c.out.none();
        return;
    }
    c.out.begin_tuple();
    c.out.integer(r->first);
    c.out.integer(r->second);
    c.out.end_tuple();
}

void kind_q(Call &c) { opt_text(c.out, kind_of(c.data, c.str_arg(0))); }

// kind(path): None for a file that cannot be read
void kind_path_q(Call &c) {
    std::string data;
    const std::string path = c.str_arg(0);
    if (!read_file(path, data)) {
        c.out.none();
        return;
    }
    std::string base = path.substr(path.find_last_of("/\\") == std::string::npos ? 0 : path.find_last_of("/\\") + 1);
    opt_text(c.out, kind_of(Bytes(data), base));
}

void pairs(Out &o, const std::vector<std::pair<int64_t, int64_t>> &ps) {
    o.begin_list();
    for (auto &p : ps) {
        o.begin_tuple();
        o.integer(p.first);
        o.integer(p.second);
        o.end_tuple();
    }
    o.end_list();
}

void chain_q(Call &c) { pairs(c.out, chain(c.data)); }

void driver_and_vxd_q(Call &c) {
    auto r = driver_and_vxd(c.data);
    if (!r) {
        c.out.none();
        return;
    }
    c.out.begin_tuple();
    c.out.integer(r->first);
    c.out.integer(r->second);
    c.out.end_tuple();
}

void dos_and_windows_q(Call &c) {
    auto r = dos_and_windows(c.data);
    if (!r) {
        c.out.none();
        return;
    }
    c.out.begin_tuple();
    mz_dict(c.out, r->first);
    c.out.integer(r->second);
    c.out.end_tuple();
}

void packer_q(Call &c) {
    const Value &hv = c.arg(0);
    Mz h{};
    h.header = key(hv, "header");
    h.cs = key(hv, "cs");
    h.ip = key(hv, "ip");
    opt_text(c.out, packer_of(c.data, h));
}

void packer_path_q(Call &c) {
    auto h = mz(c.data);
    if (h) opt_text(c.out, packer_of(c.data, *h));
    else c.out.none();
}

void unpack_exepack_q(Call &c) {
    Exepack e = unpack_exepack(c.data, c.int_arg(0), c.int_arg(1));
    Out &o = c.out;
    o.begin_tuple();
    o.bytes(Bytes(e.module));
    o.begin_list();
    for (int64_t r : e.relocs) o.integer(r);
    o.end_list();
    o.integer(e.cs);
    o.integer(e.ip);
    o.integer(e.ss);
    o.integer(e.sp);
    o.end_tuple();
}

std::vector<int64_t> ints(const Value &v);

// unpack_emulated(module, relocations, cs, ip, ss, sp, moved=False): the unpacker run (emulate/unpack.py's unpacked)
void unpack_emulated_q(Call &c) {
    Exepack e = unpack_emulated(c.data.str(), ints(c.arg(0)), c.int_arg(1), c.int_arg(2), c.int_arg(3), c.int_arg(4),
                                c.bool_arg(5, false));
    Out &o = c.out;
    o.begin_tuple();
    o.bytes(Bytes(e.module));
    o.begin_list();
    for (int64_t r : e.relocs) o.integer(r);
    o.end_list();
    o.integer(e.cs);
    o.integer(e.ip);
    o.integer(e.ss);
    o.integer(e.sp);
    o.end_tuple();
}

std::vector<int64_t> ints(const Value &v) {
    std::vector<int64_t> out;
    for (const Value &x : v.items) out.push_back(x.as_int());
    return out;
}

void data_segment_q(Call &c) {
    auto r = data_segment(c.data, c.int_arg(0), c.int_arg(1), c.str_arg(2), ints(c.arg(3)), c.int_arg(4));
    if (r) c.out.integer(*r);
    else c.out.none();
}

// unpacked_file(p): (kind, name, relocations, cs, ss, sp, ip, overlay) beside the image
void unpacked_file_q(Call &c) {
    Program p;
    p.kind = c.str_arg(0);
    p.image = c.data.str();
    p.relocations = ints(c.arg(2));
    p.cs = c.int_arg(3);
    p.ss = c.int_arg(4);
    p.sp = c.int_arg(5);
    p.ip = c.int_arg(6);
    p.overlay = c.arg(7).as_bytes().str();
    c.out.bytes(Bytes(unpacked_file(p, c.str_arg(1))));
}

// _read_mz(data, _mz(data), Program(path, "mz"), "mz"): the MZ program an image's header describes
// (kernel_parts: IO.SYS's SYSINIT)
void read_mz_q(Call &c) {
    std::optional<Mz> h = mz(c.data);
    if (!h) raise("TypeError", "'NoneType' object is not subscriptable");
    Program p;
    p.kind = "mz";
    read_mz(c.data, *h, p, "mz", true);
    program_answer(c.out, p);
}

void read_q(Call &c) { program_answer(c.out, read(c.data, c.str_arg(0), c.bool_arg(1, true))); }

void driver_part_q(Call &c) {
    Program p = driver_part(c.data);
    p.name = "the real-mode driver of " + c.str_arg(0);
    program_answer(c.out, p);
}

void pklite_head_q(Call &c) {
    Program p = pklite_head(c.data, c.int_arg(0));
    p.name = "pklite (the entry code and decompressor PKLITE put in front of " + c.str_arg(1) + ")";
    program_answer(c.out, p);
}

}  // namespace

CBF_QUESTIONS(dos) {
    add("dos._mz", mz_q);
    add("dos.devices", devices_q);
    add("dos._kernel_body", kernel_body_q);
    add("dos._kind", kind_q);
    add("dos.kind", kind_path_q);
    add("dos.chain", chain_q);
    add("dos.driver_and_vxd", driver_and_vxd_q);
    add("dos.dos_and_windows", dos_and_windows_q);
    add("dos._packer", packer_q);
    add("dos.packer", packer_path_q);
    add("dos.unpack_exepack", unpack_exepack_q);
    add("dos.unpack_emulated", unpack_emulated_q);
    add("dos._data_segment", data_segment_q);
    add("dos.unpacked_file", unpacked_file_q);
    add("dos.read", read_q);
    add("dos.read_mz", read_mz_q);
    add("dos.driver_part", driver_part_q);
    add("dos.pklite_head", pklite_head_q);
}

}  // namespace cbf::dos
