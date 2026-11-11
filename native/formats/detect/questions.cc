// questions.cc -- what kind of file a file is (codebreaker/formats/detect.py), read by cb::formats::detect
// (native/cb/formats/detect.hpp): the new-header signature of the MZ family, the lane it goes to, the DOS header,
// and identify() for any file.
#include <string>

#include "../core/cb_bridge.h"
#include "cb/formats/detect.hpp"

namespace cbf::detect {
namespace {

namespace T = cb::formats::detect;

// signature(path): b"PE" ... b"W4", b"" for a plain DOS program; None when unreadable or not MZ
void signature_q(Call &c) {
    auto sig = T::new_header(path_of(c.str_arg(0)));
    if (!sig) {
        c.out.none();
        return;
    }
    const std::string s = T::signature_text(*sig);
    c.out.bytes(Bytes(s));
}

void format_of_q(Call &c) { put_opt_text(c.out, T::format_of(path_of(c.str_arg(0)))); }

void lane_q(Call &c) {
    auto lane = T::lane(path_of(c.str_arg(0)));
    if (lane) c.out.text(T::lane_name(*lane));
    else c.out.none();
}

// (path, bundles): bundles defaults to (b"W3", b"W4")
void holds_drivers_q(Call &c) {
    bool w3 = true, w4 = true;
    if (c.argc() >= 2) {
        w3 = w4 = false;
        for (const Value &v : c.arg(1).items)
            if (v.kind == Value::Bytes) {
                w3 = w3 || v.s == "W3";
                w4 = w4 || v.s == "W4";
            }
    }
    c.out.boolean(T::holds_drivers(path_of(c.str_arg(0)), w3, w4));
}

void dos_header_q(Call &c) {
    auto h = T::dos_header(view(c.data));
    Out &o = c.out;
    if (!h) {
        o.none();
        return;
    }
    o.begin_dict();
    o.item("bytes_in_last_page", h->bytes_in_last_page);
    o.item("pages", h->pages);
    o.item("relocations", h->relocations);
    o.item("header_paragraphs", h->header_paragraphs);
    o.item("header_size", h->header_size);
    o.item("min_alloc", h->min_alloc);
    o.item("max_alloc", h->max_alloc);
    o.item("ss", h->ss);
    o.item("sp", h->sp);
    o.item("checksum", h->checksum);
    o.item("ip", h->ip);
    o.item("cs", h->cs);
    o.item("relocation_table", h->relocation_table);
    o.item("overlay", h->overlay);
    o.item("new_header", h->new_header);
    o.item("load_size", int64_t(h->load_size));
    o.key("relocation_entries");
    o.begin_list();
    for (const auto &r : h->relocation_entries) {
        o.begin_tuple();
        o.integer(r.segment);
        o.integer(r.offset);
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

void identify_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const T::Detected &d : T::identify(path_of(c.str_arg(0)))) {
        o.begin_dict();
        o.item_text("format", d.format);
        o.item_text("detail", d.detail);
        o.item_text("family", d.family);
        o.item_text("command", d.command);
        o.end_dict();
    }
    o.end_list();
}

}  // namespace

CBF_QUESTIONS(detect) {
    add("detect.signature", signature_q, "bytes?");
    add("detect.format_of", format_of_q, "str?");
    add("detect.lane", lane_q, "str?");
    add("detect.holds_drivers", holds_drivers_q, "bool");
    add("detect.dos_header", dos_header_q, "DosHeader{bytes_in_last_page: u16, pages: u16, relocations: u16, header_paragraphs: u16, header_size: u32, min_alloc: u16, max_alloc: u16, ss: u16, sp: u16, checksum: u16, ip: u16, cs: u16, relocation_table: u16, overlay: u16, new_header: u32, load_size: int, relocation_entries: list[(u16, u16)]}?");
    add("detect.identify", identify_q, "list[Detected{format: str, detail: str, family: str, command: str}]");
}

}  // namespace cbf::detect
