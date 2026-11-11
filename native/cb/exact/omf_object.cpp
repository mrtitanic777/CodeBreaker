#include "cb/exact/omf_object.hpp"

#include <limits>
#include <string>

#include "cb/core/error.hpp"

namespace cb::exact {

namespace {

// A count the object stores in 32 bits; a module that claims more is damaged.
std::uint32_t narrow32(std::uint64_t v, const char* what) {
    if (v > std::numeric_limits<std::uint32_t>::max()) throw FormatError("omf object", 0, std::string(what) + " does not fit 32 bits");
    return static_cast<std::uint32_t>(v);
}

FixKind fix_kind(formats::omf::FixupKind k) {
    using K = formats::omf::FixupKind;
    switch (k) {
        case K::Offset: return FixKind::Off;
        case K::Base: return FixKind::Base;
        case K::Pointer: return FixKind::Ptr;
        case K::Relative: return FixKind::Rel;
        case K::LowByte:
        case K::Other: break;
    }
    return FixKind::Lo;
}

Target target_of(const formats::omf::Target& t) {
    using K = formats::omf::Target::Kind;
    switch (t.kind) {
        case K::Segment: return {TargetKind::Segment, t.name.value_or("?")};
        case K::Group: return {TargetKind::Group, t.name.value_or(t.number ? std::to_string(*t.number) : "?")};
        case K::External: return {TargetKind::External, t.name.value_or("?")};
        case K::Unknown: break;
    }
    return {TargetKind::Segment, "?"};
}

}  // namespace

ObjectFile object_of(const formats::omf::Object& o, ByteView raw) {
    ObjectFile out;
    out.kind = ObjectKind::Omf;
    for (const auto& [name, bytes] : o.segments) out.seg.insert_or_assign(name, bytes);
    for (const auto& d : o.segdefs)
        out.segs.push_back({d.name, d.class_name, narrow32(static_cast<std::uint64_t>(d.length), "a segment's length"), d.alignment});
    for (const auto& [name, n] : o.filled) out.filled.insert_or_assign(name, narrow32(n, "a segment's filled length"));
    for (const auto& [name, members] : o.groups) out.groups.insert_or_assign(name, members);
    for (const auto& p : o.publics) out.pubs.push_back({p.name, p.segment, narrow32(static_cast<std::uint64_t>(p.offset), "a public's offset")});
    out.ext = o.externals;
    for (const auto& [name, c] : o.communals) out.comm.insert_or_assign(name, CommonVar{narrow32(c.size, "a communal's size"), c.far_pointer});
    for (const auto& f : o.fixups)
        out.fix.push_back({f.segment, narrow32(static_cast<std::uint64_t>(f.at), "a fix-up's offset"), fix_kind(f.kind), target_of(f.target),
                           static_cast<std::int32_t>(f.displacement)});
    for (const auto& [name, c] : o.combine) out.combine.insert_or_assign(name, c);
    if (o.start) out.start = StartAddress{o.start->segment, narrow32(static_cast<std::uint64_t>(o.start->offset), "the start offset")};
    out.raw.assign(raw.begin(), raw.end());
    return out;
}

ObjectFile read_object(ByteView omf_module) { return object_of(formats::omf::read(omf_module), omf_module); }

}  // namespace cb::exact
