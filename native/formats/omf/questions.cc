// questions.cc -- the OMF reader's answers, in the shapes codebreaker/formats/omf.py gives them, read by
// cb::formats::omf (native/cb/formats/omf.hpp).
// read() comes back as the attributes of omf.Obj (but raw, the caller's own bytes); the readers
// that take an Obj (holes, code_segments, functions) are handed the attributes they read, and
// the Object they ask about is made from those.
#include <map>
#include <string>
#include <vector>

#include "../core/cb_bridge.h"
#include "../core/text.h"
#include "cb/core/error.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace cbf::omf {
namespace {

namespace M = cb::formats::omf;

const char *target_kind(M::Target::Kind k) {
    switch (k) {
    case M::Target::Kind::Segment: return "seg";
    case M::Target::Kind::Group: return "grp";
    case M::Target::Kind::External: return "ext";
    default: return "?";
    }
}

std::string fixup_kind(const M::Fixup &f) {
    switch (f.kind) {
    case M::FixupKind::Offset: return "off";
    case M::FixupKind::Base: return "base";
    case M::FixupKind::Pointer: return "ptr";
    case M::FixupKind::LowByte: return "lo";
    case M::FixupKind::Relative: return "rel";
    default: return "?" + dec(f.location);
    }
}

M::FixupKind fixup_kind_from(const std::string &k) {
    if (k == "off") return M::FixupKind::Offset;
    if (k == "base") return M::FixupKind::Base;
    if (k == "ptr") return M::FixupKind::Pointer;
    if (k == "lo") return M::FixupKind::LowByte;
    if (k == "rel") return M::FixupKind::Relative;
    return M::FixupKind::Other;
}

void target(Out &o, const M::Target &t) {
    o.begin_tuple();
    o.text(target_kind(t.kind));
    if (t.name) o.text(*t.name);
    else if (t.number) o.integer(*t.number);
    else o.none();
    o.end_tuple();
}

// name(s) and the names raw_object / bss_object take: a str as it is, bytes as latin1
std::string name_text(const Value &v) { return v.kind == Value::Str ? v.s : cb::text::from_latin1(view(v.as_bytes())); }

void str_set(Out &o, const std::set<std::string> &v) {
    o.begin_set();
    for (const std::string &s : v) o.text(s);
    o.end_set();
}

template <class Ints>
void int_set(Out &o, const Ints &v) {
    o.begin_set();
    for (auto k : v) o.integer(int64_t(k));
    o.end_set();
}

// An a.out object (XENIX's compiler) is not OMF: the caller reads it (exact/aout.py), as it did before
void no_aout(Call &c) {
    if (M::is_aout(view(c.data))) fallback("a Version 7 a.out object (exact/aout.py reads it)");
}

void records_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const M::Record &r : M::records(view(c.data))) {
        o.begin_tuple();
        o.integer(r.type);
        put(o, r.payload);
        o.end_tuple();
    }
    o.end_list();
}

void record_q(Call &c) {
    const int64_t t = c.int_arg(0);
    if (t < 0 || t > 255) raise("ValueError", "byte must be in range(0, 256)");
    put(c.out, M::record(uint8_t(t), view(c.data)));
}

void name_q(Call &c) {
    const Value &v = c.arg(0);
    if (v.kind == Value::Str) put(c.out, M::name(cb::as_text(cb::text::to_latin1(v.s))));
    else put(c.out, M::name(cb::as_text(blob(v))));
}

void read_q(Call &c) {
    no_aout(c);
    const M::Object ob = M::read(view(c.data));
    Out &o = c.out;
    o.begin_dict();
    o.key("pubs");
    o.begin_list();
    for (const M::Public &p : ob.publics) {
        o.begin_tuple();
        o.text(p.name);
        put_opt_text(o, p.segment);
        o.integer(p.offset);
        o.end_tuple();
    }
    o.end_list();
    o.key("ext");
    o.begin_list();
    for (const std::string &e : ob.externals) o.text(e);
    o.end_list();
    o.key("fix");
    o.begin_list();
    for (const M::Fixup &f : ob.fixups) {
        o.begin_tuple();
        o.text(f.segment);
        o.integer(f.at);
        o.text(fixup_kind(f));
        target(o, f.target);
        o.integer(f.displacement);
        o.end_tuple();
    }
    o.end_list();
    o.key("groups");
    o.begin_dict();
    for (const auto &[group, segs] : ob.groups) {
        o.text(group);
        o.begin_list();
        for (const std::string &s : segs) o.text(s);
        o.end_list();
    }
    o.end_dict();
    o.key("comm");
    o.begin_dict();
    for (const auto &[name, cm] : ob.communals) {
        o.text(name);
        o.begin_tuple();
        o.uinteger(cm.size);
        o.boolean(cm.far_pointer);
        o.end_tuple();
    }
    o.end_dict();
    o.key("fixloc");
    o.begin_list();
    for (const M::Fixup &f : ob.fixups) o.integer(f.location);
    o.end_list();
    o.key("combine");
    o.begin_dict();
    for (const auto &[seg, comb] : ob.combine) {
        o.text(seg);
        o.integer(comb);
    }
    o.end_dict();
    o.key("start");
    if (ob.start) {
        o.begin_tuple();
        o.text(ob.start->segment);
        o.integer(ob.start->offset);
        o.end_tuple();
    } else {
        o.none();
    }
    o.key("seg");
    o.begin_dict();
    for (const auto &[seg, bytes] : ob.segments) {
        o.text(seg);
        put(o, bytes);
    }
    o.end_dict();
    o.key("segs");
    o.begin_list();
    for (const M::SegDef &s : ob.segdefs) {
        o.begin_tuple();
        o.text(s.name);
        o.text(s.class_name);
        o.integer(s.length);
        o.integer(s.alignment);
        o.end_tuple();
    }
    o.end_list();
    o.key("filled");
    o.begin_dict();
    for (const auto &[seg, n] : ob.filled) {
        o.text(seg);
        o.uinteger(n);
    }
    o.end_dict();
    o.end_dict();
}

void library_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const auto &[name, bytes] : M::library(view(c.data))) {
        o.begin_tuple();
        o.text(name);
        put(o, bytes);
        o.end_tuple();
    }
    o.end_list();
}

// holes(o, seg): (o.fix, seg)
void holes_q(Call &c) {
    const Value &fix = c.arg(0), &seg = c.arg(1);
    M::Object ob;
    for (const Value &f : fix.items) {
        if (f.items.size() != 5) raise("ValueError", "not enough values to unpack");
        if (f.items[0].kind != Value::Str) continue;
        M::Fixup x;
        x.segment = f.items[0].s;
        x.at = f.items[1].as_int();
        x.kind = fixup_kind_from(f.items[2].as_str());
        ob.fixups.push_back(std::move(x));
    }
    if (seg.kind != Value::Str) {
        c.out.begin_set();
        c.out.end_set();
        return;
    }
    int_set(c.out, M::holes(ob, seg.s));
}

// code_segments(o): (o.segs,)
void code_segments_q(Call &c) {
    M::Object ob;
    for (const Value &s : c.arg(0).items) {
        M::SegDef d;
        d.name = item(s, 0).as_str();
        d.class_name = item(s, 1).as_str();
        ob.segdefs.push_back(std::move(d));
    }
    Out &o = c.out;
    o.begin_list();
    for (const std::string &n : M::code_segments(ob)) o.text(n);
    o.end_list();
}

// functions(o, seg): (o.pubs, len(o.seg.get(seg, b"")), o.filled.get(seg, 0), seg)
void functions_q(Call &c) {
    const Value &pubs = c.arg(0);
    const std::string seg = c.str_arg(3);
    M::Object ob;
    for (const Value &p : pubs.items) {
        M::Public x;
        x.name = item(p, 0).as_str();
        if (!item(p, 1).is_none()) x.segment = item(p, 1).as_str();
        x.offset = item(p, 2).as_int();
        ob.publics.push_back(std::move(x));
    }
    ob.segments[seg] = cb::Bytes(size_t(std::max<int64_t>(0, c.int_arg(1))));
    ob.filled[seg] = uint64_t(std::max<int64_t>(0, c.int_arg(2)));
    Out &o = c.out;
    o.begin_list();
    for (const M::FunctionSpan &f : M::functions(ob, seg)) {
        o.begin_tuple();
        o.text(f.name);
        o.integer(f.offset);
        o.integer(f.length);
        o.end_tuple();
    }
    o.end_list();
}

void names_of_q(Call &c) {
    no_aout(c);
    const auto [pubs, exts] = M::names_of(view(c.data));
    c.out.begin_tuple();
    str_set(c.out, pubs);
    str_set(c.out, exts);
    c.out.end_tuple();
}

void publics_q(Call &c) {
    c.out.begin_set();
    for (const std::string &s : M::publics(view(c.data))) c.out.text(s);
    c.out.end_set();
}

void externs_q(Call &c) {
    c.out.begin_list();
    for (const std::string &e : M::externs(view(c.data))) c.out.text(e);
    c.out.end_list();
}

// walk(data, start) -> (records up to the first failure, (exception, message) or None)
void walk_q(Call &c) {
    const M::Walk w = M::walk(view(c.data), uint64_t(c.int_arg(0, 0)));
    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    for (const M::RawRecord &r : w.records) {
        o.begin_tuple();
        o.integer(int64_t(r.offset));
        o.integer(r.type);
        put(o, r.payload);
        o.end_tuple();
    }
    o.end_list();
    if (w.error) {
        o.begin_tuple();
        o.text("struct.error");   // a record cut short
        o.text(*w.error);
        o.end_tuple();
    } else {
        o.none();
    }
    o.end_tuple();
}

void code_q(Call &c) {
    no_aout(c);
    const M::CodeSegment r = M::code_segment(view(c.data));
    Out &o = c.out;
    o.begin_tuple();
    put_opt_text(o, r.segment);
    put(o, r.code);
    int_set(o, r.fixup_mask);
    o.begin_dict();
    for (const auto &[name, where] : r.publics) {
        o.text(name);
        o.begin_tuple();
        o.text(where.first);
        o.integer(where.second);
        o.end_tuple();
    }
    o.end_dict();
    o.end_tuple();
}

void code_segment_q(Call &c) {
    no_aout(c);
    const M::CodeSegment r = M::code_segment(view(c.data));
    Out &o = c.out;
    o.begin_tuple();
    put_opt_text(o, r.segment);
    put(o, r.code);
    int_set(o, r.fixup_mask);
    o.end_tuple();
}

void function_bytes_q(Call &c) {
    no_aout(c);
    Out &o = c.out;
    o.begin_dict();
    for (const auto &[name, f] : M::function_bytes(view(c.data))) {
        o.text(name);
        o.begin_tuple();
        put(o, f.code);
        int_set(o, f.fixup_mask);
        o.end_tuple();
    }
    o.end_dict();
}

void rename_externs_q(Call &c) {
    std::map<std::string, std::string> mapping;
    const Value &m = c.arg(0);
    for (size_t k = 0; k + 1 < m.items.size(); k += 2) mapping[m.items[k].as_str()] = m.items[k + 1].as_str();
    put(c.out, M::rename_externs(view(c.data), mapping));
}

cb::OrderedMap<std::string, int64_t> name_values(const Value &d) {
    cb::OrderedMap<std::string, int64_t> out;
    for (size_t k = 0; k + 1 < d.items.size(); k += 2) out.insert_or_assign(name_text(d.items[k]), d.items[k + 1].as_int());
    return out;
}

// raw_object(title, code, pubs, data=b"", dpubs=None, text="_TEXT", dalign=1)
void raw_object_q(Call &c) {
    const cb::Bytes code = blob(c.arg(1));
    const cb::Bytes data = c.argc() > 3 ? blob(c.arg(3)) : cb::Bytes{};
    const cb::OrderedMap<std::string, int64_t> dpubs =
        c.argc() > 4 && c.arg(4).truthy() ? name_values(c.arg(4)) : cb::OrderedMap<std::string, int64_t>{};
    put(c.out, M::raw_object(name_text(c.arg(0)), code, name_values(c.arg(2)), data, dpubs,
                             c.argc() > 5 ? name_text(c.arg(5)) : std::string("_TEXT"), int(c.int_arg(6, 1))));
}

// bss_object(names, start, end, title="bssvars", seg="_BSS", para=False)
void bss_object_q(Call &c) {
    put(c.out, M::bss_object(name_values(c.arg(0)), c.int_arg(1), c.int_arg(2),
                             c.argc() > 3 ? name_text(c.arg(3)) : std::string("bssvars"),
                             c.argc() > 4 ? name_text(c.arg(4)) : std::string("_BSS"), c.bool_arg(5)));
}

// omf.patched(raw, segment, offset, value bytes) -> the module with the value written into the segment's LEDATA
void patched_q(Call &c) {
    const cb::Bytes value = blob(c.arg(2));
    put(c.out, M::patched(view(c.data), c.str_arg(0), c.int_arg(1), value));
}

// omf.symbol_names(data) -> ([the external and public names, in the order rename_symbols meets them],
// None | (exception, message) the walk raised after them): the caller names them in that order before the
// exception is raised, as the one Python walk did
void symbol_names_q(Call &c) {
    std::vector<std::string> seen;
    std::optional<std::pair<std::string, std::string>> failed;
    try {
        (void)M::rename_symbols(view(c.data), [&](std::string_view n) {
            seen.emplace_back(n);
            return std::string(n);
        });
    } catch (const cb::TruncatedError &e) {
        failed = std::make_pair(std::string("struct.error"), std::string(e.what()));
    } catch (const cb::Error &e) {
        failed = std::make_pair(std::string("ValueError"), std::string(e.what()));
    }
    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    for (const std::string &n : seen) o.text(n);
    o.end_list();
    if (!failed) {
        o.none();
    } else {
        o.begin_tuple();
        o.text(failed->first);
        o.text(failed->second);
        o.end_tuple();
    }
    o.end_tuple();
}

// omf.rename_symbols(data, [new names in that order]) -> the object
void rename_symbols_q(Call &c) {
    const Value &names = c.arg(0);
    size_t k = 0;
    put(c.out, M::rename_symbols(view(c.data), [&](std::string_view) {
            if (k >= names.items.size()) raise("IndexError", "list index out of range");
            return names.items[k++].as_str();
        }));
}

}  // namespace

CBF_QUESTIONS(omf) {
    add("omf.patched", patched_q, "bytes");
    add("omf.symbol_names", symbol_names_q, "(list[str], (str, str)?)");
    add("omf.rename_symbols", rename_symbols_q, "bytes");
    add("omf.records", records_q, "list[(u8, bytes)]");
    add("omf.record", record_q, "bytes");
    add("omf.name", name_q, "bytes");
    add("omf.read", read_q, "OmfObject{pubs: list[(str, str?, u32)], ext: list[str], fix: list[(str, u32, str, (str, str|int|none), int)], groups: dict[str, list[str]], comm: dict[str, (int, bool)], fixloc: list[u8], combine: dict[str, int], start: (str, u32)?, seg: dict[str, bytes], segs: list[(str, str, u32, u8)], filled: dict[str, u32]}");
    add("omf.library", library_q, "list[(str, bytes)]");
    add("omf.holes", holes_q, "set[int]");
    add("omf.code_segments", code_segments_q, "list[str]");
    add("omf.functions", functions_q, "list[(str, u32, int)]");
    add("omf.names_of", names_of_q, "(set[str], set[str])");
    add("omf.publics", publics_q, "set[str]");
    add("omf.externs", externs_q, "list[str]");
    add("omf.walk", walk_q, "(list[(int, u8, bytes)], (str, str)?)");
    add("omf._code", code_q, "(str?, bytes, set[int], dict[str, (str, u32)])");
    add("omf.code_segment", code_segment_q, "(str?, bytes, set[int])");
    add("omf.function_bytes", function_bytes_q, "dict[str, (bytes, set[int])]");
    add("omf.rename_externs", rename_externs_q, "bytes");
    add("omf.raw_object", raw_object_q, "bytes");
    add("omf.bss_object", bss_object_q, "bytes");
}

}  // namespace cbf::omf
