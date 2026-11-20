#include "build/x86asm/format_obj.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>

#include "build/x86asm/format_util.hpp"

namespace cb::build::x86asm {

namespace {

using namespace fmt;
using namespace omf;

// The segment name used when the source never names one.
constexpr std::string_view kDefaultSegment = "__NASMDEFSEG";

// FILENAME_MAX of the reference build (the THEADR name is copied into a buffer of this size).
constexpr std::size_t kFilenameMax = 260;

// The fixup location types of the FIXUPP subrecords.
constexpr unsigned FIX_16_OFFSET = 0x8400, FIX_16_SELECTOR = 0x8800, FIX_32_OFFSET = 0xA400;

// The segments the Unix names map to the default class of in `obj2` (conv_table).
struct SegmentToClass {
    std::string_view segment;
    std::string_view segclass;
};
constexpr SegmentToClass kConvTable[] = {
    {"CODE", "CODE"},   {"TEXT", "CODE"},   {"CONST", "CONST"},   {"DATA", "DATA"},   {"BSS", "BSS"},   {"STACK", "STACK"},
    {"CODE32", "CODE"}, {"TEXT32", "CODE"}, {"CONST32", "CONST"}, {"DATA32", "DATA"}, {"BSS32", "BSS"}, {"STACK32", "STACK"},
};

std::optional<std::string_view> default_class(std::string_view segment) {
    if (!segment.empty())
        for (const SegmentToClass& c : kConvTable)
            if (c.segment == segment) return c.segclass;
    return std::nullopt;
}

constexpr char at_or_nul(std::string_view s, std::size_t i) { return i < s.size() ? s[i] : '\0'; }

constexpr bool is_power2(std::uint64_t v) { return v && (v & (v - 1)) == 0; }

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------
// The record builder
// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<ObjRecord> ObjWriter::make() {
    auto orp = std::make_unique<ObjRecord>();
    orp->ori = ObjInit::Null;
    return orp;
}

// Clear an ObjRecord: .type, .ori and .parm are not cleared, so a record can be reused.
ObjRecord* ObjWriter::clear(ObjRecord* orp) {
    orp->used = 0;
    orp->committed = 0;
    orp->x_size = 0;
    orp->child.reset();
    orp->up = nullptr;
    orp->back.reset();
    return orp;
}

// Write a record out, preceded (recursively) by its previous part and followed (recursively) by its child. The
// previous part and the child are freed; the record itself is cleared.
ObjRecord* ObjWriter::emit(ObjRecord* orp) {
    if (orp->back) {
        emit(orp->back.get());
        orp->back.reset();
    }
    if (orp->committed) write_record(*orp);
    if (orp->child) {
        emit(orp->child.get());
        orp->child.reset();
    }
    return clear(orp);
}

ObjRecord* ObjWriter::emit2(ObjRecord* orp) {
    commit(orp);
    return emit(orp);
}

// Advance to the next record because the existing one is full or its x_size is incompatible. Any uncommitted data is
// moved into the next record.
ObjRecord* ObjWriter::bump(ObjRecord* orp) {
    int used = orp->used;
    const int committed = orp->committed;
    ObjRecord* nxt;
    if (orp->up) {
        std::unique_ptr<ObjRecord>& slot = *orp->up;
        std::unique_ptr<ObjRecord> old = std::move(slot);   // owns orp: it becomes the previous part
        slot = make();
        nxt = slot.get();
        nxt->ori = orp->ori;
        nxt->type = orp->type;
        nxt->up = orp->up;
        nxt->parm = orp->parm;
        nxt->back = std::move(old);
    } else {
        nxt = emit(orp);
    }

    used -= committed;
    if (used) {
        nxt->committed = 1;
        run_init(*nxt);
        nxt->committed = nxt->used;
        std::memmove(nxt->buf.data() + nxt->committed, orp->buf.data() + committed, static_cast<std::size_t>(used));
        nxt->used = nxt->committed + used;
    }
    return nxt;
}

// Advance to the next record if necessary to allow the next field to fit.
ObjRecord* ObjWriter::check(ObjRecord* orp, int size) {
    if (orp->used + size > ObjRecord::kRecordMax) orp = bump(orp);
    if (!orp->committed) {
        orp->committed = 1;
        run_init(*orp);
        orp->committed = orp->used;
    }
    return orp;
}

// All data written so far is committed to the current record (it won't be moved to the next one on continuation).
ObjRecord* ObjWriter::commit(ObjRecord* orp) {
    orp->committed = orp->used;
    return orp;
}

ObjRecord* ObjWriter::byte(ObjRecord* orp, std::uint8_t val) {
    orp = check(orp, 1);
    orp->buf[static_cast<std::size_t>(orp->used)] = val;
    orp->used++;
    return orp;
}

ObjRecord* ObjWriter::word(ObjRecord* orp, unsigned val) {
    orp = check(orp, 2);
    orp->buf[static_cast<std::size_t>(orp->used)] = static_cast<std::uint8_t>(val);
    orp->buf[static_cast<std::size_t>(orp->used) + 1] = static_cast<std::uint8_t>(val >> 8);
    orp->used += 2;
    return orp;
}

ObjRecord* ObjWriter::rword(ObjRecord* orp, unsigned val) {
    orp = check(orp, 2);
    orp->buf[static_cast<std::size_t>(orp->used)] = static_cast<std::uint8_t>(val >> 8);
    orp->buf[static_cast<std::size_t>(orp->used) + 1] = static_cast<std::uint8_t>(val);
    orp->used += 2;
    return orp;
}

ObjRecord* ObjWriter::dword(ObjRecord* orp, std::uint32_t val) {
    orp = check(orp, 4);
    const std::size_t u = static_cast<std::size_t>(orp->used);
    orp->buf[u] = static_cast<std::uint8_t>(val);
    orp->buf[u + 1] = static_cast<std::uint8_t>(val >> 8);
    orp->buf[u + 2] = static_cast<std::uint8_t>(val >> 16);
    orp->buf[u + 3] = static_cast<std::uint8_t>(val >> 24);
    orp->used += 4;
    return orp;
}

// All fields of "size x" in one record must be the same size (16 or 32 bits); a one-bit flag in each record says
// which. This forces the current record to the desired x_size (needed outside obj_x only to suit linkers that have
// bugs in their processing of the size bit).
ObjRecord* ObjWriter::force(ObjRecord* orp, int x) {
    if (orp->x_size == (x ^ 48)) orp = bump(orp);
    orp->x_size = x;
    return orp;
}

// A field of size x: the caller need not worry about whether 16 or 32 bits are required.
ObjRecord* ObjWriter::x(ObjRecord* orp, std::uint32_t val) {
    if (orp->type & 1) orp->x_size = 32;
    if (val > 0xFFFF) orp = force(orp, 32);
    if (orp->x_size == 32) {
        ObjRecord* nxt = dword(orp, val);
        nxt->x_size = 32;   // x_size is cleared when a record overflows
        return nxt;
    }
    orp->x_size = 16;
    return word(orp, val);
}

ObjRecord* ObjWriter::index(ObjRecord* orp, unsigned val) {
    if (val < 128) return byte(orp, static_cast<std::uint8_t>(val));
    return word(orp, (val >> 8) | (val << 8) | 0x80);
}

ObjRecord* ObjWriter::value(ObjRecord* orp, std::uint32_t val) {
    if (val <= 128) return byte(orp, static_cast<std::uint8_t>(val));
    if (val <= 0xFFFF) {
        orp = byte(orp, 129);
        return word(orp, val);
    }
    if (val <= 0xFFFFFF) return dword(orp, (val << 8) + 132);
    orp = byte(orp, 136);
    return dword(orp, val);
}

ObjRecord* ObjWriter::name(ObjRecord* orp, std::string_view text) {
    std::size_t len = text.size();
    if (len > 255) {
        a_.warn(WARN_OTHER, "truncating object name `{}...' to {} bytes", text.substr(0, 64), 255u);
        len = 255;
    }
    orp = check(orp, static_cast<int>(len) + 1);
    std::uint8_t* ptr = orp->buf.data() + orp->used;
    *ptr++ = static_cast<std::uint8_t>(len);
    orp->used += static_cast<int>(len) + 1;
    for (std::size_t i = 0; i < len; ++i) *ptr++ = static_cast<std::uint8_t>(uppercase ? upper(text[i]) : text[i]);
    return orp;
}

// The initializers: what each continuation record starts with.
void ObjWriter::run_init(ObjRecord& orp) {
    switch (orp.ori) {
    case ObjInit::Null:
        break;
    case ObjInit::Ledata:
        // parm[0] = offset, parm[1] = segment index. During the use of a LEDATA record parm[0] is constantly updated
        // to the offset that would be required if the record were split at the last commit point; parm[2] is a copy of
        // parm[0] as it was when the current record was started.
        index(&orp, orp.parm[1]);
        orp.parm[2] = orp.parm[0];
        x(&orp, orp.parm[0]);
        break;
    case ObjInit::Pubdef:
        // parm[0] = group index, parm[1] = segment index, parm[2] = frame (only used when both indexes are zero).
        index(&orp, orp.parm[0]);
        index(&orp, orp.parm[1]);
        if (!(orp.parm[0] | orp.parm[1])) word(&orp, orp.parm[2]);
        break;
    case ObjInit::Linnum:
        index(&orp, orp.parm[0]);
        index(&orp, orp.parm[1]);
        break;
    case ObjInit::Local:
        rword(&orp, dSYM);
        break;
    }
}

// obj_fwrite: type, length, data and the checksum byte.
void ObjWriter::write_record(const ObjRecord& orp) {
    unsigned cksum = orp.type;
    if (orp.x_size == 32) cksum |= 1;
    put8(out_, cksum);
    unsigned len = static_cast<unsigned>(orp.committed) + 1;
    cksum += (len & 0xFF) + ((len >> 8) & 0xFF);
    put16(out_, len);
    out_.insert(out_.end(), orp.buf.begin(), orp.buf.begin() + (len - 1));
    for (unsigned i = 0; i + 1 < len; ++i) cksum += orp.buf[i];
    put8(out_, (0u - cksum) & 0xFFu);
}

// ---------------------------------------------------------------------------------------------------------------------
// The format
// ---------------------------------------------------------------------------------------------------------------------

ObjFormat::ObjFormat(FormatHost& host, bool obj2) : host_(host), a_(host.assembly()), obj2_(obj2), w_(host.assembly(), out_) {
    // The format's pragma facility: the final entry without a name is the default one (`%pragma output nodepend`).
    pragmas_.push_back(PragmaFacility{"", [this](const Pragma& pragma) {
                                          if (pragma.opcode == D_NODEPEND) nodepend_ = true;
                                          return DIRR_OK;
                                      }});
}

ObjFormat::~ObjFormat() = default;

std::string_view ObjFormat::fullname() const {
    return obj2_ ? "Intel/Microsoft OMF (i386) (OS/2)" : "Intel/Microsoft OMF (MS-DOS, OS/2, Win16)";
}

// The standard macros of outobj.mac.
const std::vector<std::string>& ObjFormat::stdmac() const {
    static const std::vector<std::string> lines = {
        "%define __?SECT?__ [section .text]",
        "%imacro group 1+.nolist", "[group %1]", "%endmacro",
        "%imacro uppercase 0+.nolist", "[uppercase %1]", "%endmacro",
        "%imacro export 1+.nolist", "[export %1]", "%endmacro",
        "%imacro import 1+.nolist", "[import %1]", "%endmacro",
        "%macro __?NASM_CDecl?__ 1", "%endmacro"};
    return lines;
}

void ObjFormat::init() {
    infile_ = a_.inname.substr(0, kFilenameMax - 1);
    first_seg_ = host_.seg_alloc();
    any_segs_ = false;
    fpubs_.clear();
    externals_.clear();
    ext_slots_.clear();
    dws_.clear();
    segments_.clear();
    groups_.clear();
    imports_.clear();
    exports_.clear();
    seg_needs_update_ = nullptr;
    grp_needs_update_ = nullptr;
    entry_seg_ = NO_SEG;
    w_.uppercase = false;
    use32_ = false;
    current_seg_ = nullptr;
    out_.clear();

    // Convert known Unix sections to OMF segments via macros.
    if (obj2_) {
        host_.pre_define(".text=TEXT32");
        host_.pre_define(".rodata=CONST32");
        host_.pre_define(".data=DATA32");
        host_.pre_define(".bss=BSS32");
    }
}

// Resolve the default-WRT name of an external: a segment or a group already defined, else remember the name until one is.
void ObjFormat::obj_ext_set_defwrt(External& ext, std::string id) {
    for (auto& seg : segments_)
        if (seg->name == id) {
            ext.defwrt_type = DefWrt::Segment;
            ext.defwrt_seg = seg.get();
            return;
        }
    for (auto& grp : groups_)
        if (grp->name == id) {
            ext.defwrt_type = DefWrt::Group;
            ext.defwrt_grp = grp.get();
            return;
        }
    ext.defwrt_type = DefWrt::String;
    ext.defwrt_string = std::move(id);
    dws_.insert(dws_.begin(), &ext);
}

bool ObjFormat::external_block_exists(std::int32_t i) const {
    return i >= 0 && static_cast<std::size_t>(i) < ext_slots_.size();
}

ObjFormat::External* ObjFormat::external_for(std::int32_t i) {
    return external_block_exists(i) ? ext_slots_[static_cast<std::size_t>(i)] : nullptr;
}

ObjFormat::Segment* ObjFormat::find_segment_by_index(std::int32_t index) {
    for (auto& seg : segments_)
        if (seg->index == index) return seg.get();
    return nullptr;
}

ObjFormat::Group* ObjFormat::find_group(std::string_view name) {
    for (auto& grp : groups_)
        if (grp->name == name) return grp.get();
    return nullptr;
}

// Find group FLAT, defining it (empty) if it does not exist.
ObjFormat::Group* ObjFormat::ensure_group_flat() {
    Group* grp = find_group("FLAT");
    if (!grp) {
        directive_group("FLAT");
        grp = find_group("FLAT");
        if (!grp) a_.panic("failure to define FLAT?!");
    }
    return grp;
}

void ObjFormat::define_default_segment() {
    int tempint = 0;
    (void)segment_by_name(kDefaultSegment, tempint);
}

void ObjFormat::symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global,
                       std::string_view special_text) {
    // (The label manager passes no special text as null, which `special_text` empty is here.)
    const std::optional<std::string_view> special =
        special_text.empty() ? std::nullopt : std::optional<std::string_view>(special_text);
    // We have three cases:
    //
    // (i)   `segment' is a segment-base. If so, set the name field for the segment or group structure it refers to,
    //       and then return.
    // (ii)  `segment' is one of our segments, or a SEG_ABS segment. Save the label position for later output of a
    //       PUBDEF record.
    // (iii) `segment' is not one of our segments. Save the label position for later output of an EXTDEF, and also store
    //       a back-reference so that later references to this segment number can be mapped to the external index.
    bool used_special = false;   // have we used the special text?

    // If it's a special-retry from pass two, discard it.
    if (is_global == 3) return;

    // First check for the double-period, signifying something unusual.
    if (at_or_nul(name, 0) == '.' && at_or_nul(name, 1) == '.' && at_or_nul(name, 2) != '@') {
        if (name == "..start") {
            entry_seg_ = segment;
            entry_ofs_ = static_cast<std::int32_t>(offset);
            return;
        }
        a_.nonfatal("unrecognised special symbol `{}'", name);
    }

    // Case (i):
    if (seg_needs_update_) {
        seg_needs_update_->name = std::string(name);
        return;
    }
    if (grp_needs_update_) {
        grp_needs_update_->name = std::string(name);
        return;
    }
    if (segment < SEG_ABS && segment != NO_SEG && segment % 2) return;

    if (segment >= SEG_ABS || segment == NO_SEG) {
        // SEG_ABS subcase of (ii).
        if (is_global) {
            Public pub;
            pub.name = std::string(name);
            pub.offset = static_cast<std::int32_t>(offset);
            pub.segment = (segment == NO_SEG ? 0 : (segment & ~SEG_ABS));
            fpubs_.push_back(std::move(pub));
        }
        if (special) a_.nonfatal("OBJ supports no special symbol features for this symbol type");
        return;
    }

    // If `any_segs' is still false, we might need to define a default segment, if they're trying to declare a label in
    // `first_seg'.
    if (!any_segs_ && segment == first_seg_) {
        int tempint = 0;
        if (segment != segment_by_name(kDefaultSegment, tempint)) a_.panic("strange segment conditions in OBJ driver");
    }

    if (is_global) {
        for (auto& seg : segments_)
            if (seg->index == segment) {
                // Case (ii). Maybe MODPUB someday?
                Public loc;
                loc.name = std::string(name);
                loc.offset = static_cast<std::int32_t>(offset);
                seg->pubs.push_back(std::move(loc));
                if (special) a_.nonfatal("OBJ supports no special symbol features for this symbol type");
                return;
            }
    }

    // Case (iii).
    if (!is_global) return;

    auto owned = std::make_unique<External>();
    External* ext = owned.get();
    externals_.push_back(std::move(owned));
    ext->name = std::string(name);
    // Place by default all externs into the current segment
    ext->defwrt_type = DefWrt::None;

    // (28-Apr-2002 John Coffman: the default-WRT of externals broke fixups on code passed through the MSC 5.1 and 6.00A
    // linkers; it is kept for 32-bit segments only.)
    if (current_seg_ && current_seg_->use32) {
        if (current_seg_->grp) {
            ext->defwrt_type = DefWrt::Group;
            ext->defwrt_grp = current_seg_->grp;
        } else {
            ext->defwrt_type = DefWrt::Segment;
            ext->defwrt_seg = current_seg_;
        }
    }

    if (is_global == 2) {
        ext->commonsize = static_cast<std::int32_t>(offset);
        ext->commonelem = 1;   // default FAR
    } else {
        ext->commonsize = 0;
    }

    // Now process the special text, if any, to find default-WRT specifications and common-variable element-size and
    // near/far specifications.
    std::string_view sp = special.value_or(std::string_view());
    while (!sp.empty()) {
        used_special = true;

        // We might have a default-WRT specification.
        if (starts_nocase(sp, "wrt")) {
            sp.remove_prefix(3);
            sp = skip_spaces(sp);
            const std::size_t len = sp.find(':') == std::string_view::npos ? sp.size() : sp.find(':');
            obj_ext_set_defwrt(*ext, std::string(sp.substr(0, len)));
            sp.remove_prefix(len);
            if (!sp.empty() && sp.front() == ':') sp.remove_prefix(1);
        }

        // The NEAR or FAR keywords specify nearness or farness. FAR gives default element size 1.
        if (starts_nocase(sp, "far")) {
            if (ext->commonsize)
                ext->commonelem = 1;
            else
                a_.nonfatal("`{}': `far' keyword may only be applied to common variables\n", ext->name);
            sp.remove_prefix(3);
            sp = skip_spaces(sp);
        } else if (starts_nocase(sp, "near")) {
            if (ext->commonsize)
                ext->commonelem = 0;
            else
                a_.nonfatal("`{}': `far' keyword may only be applied to common variables\n", ext->name);
            sp.remove_prefix(4);
            sp = skip_spaces(sp);
        }

        // If it's a common, and anything else remains on the line before a further colon, evaluate it as an expression
        // and use that as the element size. Forward references aren't allowed.
        if (!sp.empty() && sp.front() == ':') {
            sp.remove_prefix(1);
        } else if (!sp.empty()) {
            if (ext->commonsize) {
                const ExprAtFront e = host_.evaluate_critical(sp);
                if (e.ok) {
                    if (!e.simple)
                        a_.nonfatal("cannot use relocatable expression as common-variable element size");
                    else
                        ext->commonelem = static_cast<std::int32_t>(e.value);
                }
                sp.remove_prefix(std::min(std::max<std::size_t>(e.consumed, 1), sp.size()));
            } else {
                a_.nonfatal("`{}': element-size specifications only apply to common variables", ext->name);
                while (!sp.empty() && sp.front() != ':') sp.remove_prefix(1);
                if (!sp.empty() && sp.front() == ':') sp.remove_prefix(1);
            }
        }
    }

    const std::int32_t i = segment / 2;
    const std::size_t blocks = static_cast<std::size_t>(i / kExtBlock) + 1;
    if (ext_slots_.size() < blocks * kExtBlock) ext_slots_.resize(blocks * kExtBlock, nullptr);
    ext_slots_[static_cast<std::size_t>(i)] = ext;
    ext->index = static_cast<int>(externals_.size());

    if (special && !used_special) a_.nonfatal("OBJ supports no special symbol features for this symbol type");
}

void ObjFormat::output(const OutData& out) {
    const std::int32_t segto = out.loc.segment;
    const void* data = out.legacy.data;
    const OutType type = out.legacy.type;
    std::uint64_t size = out.legacy.size;
    std::int32_t segment = out.legacy.tsegment;
    const std::int32_t wrt = out.legacy.twrt;

    // If `any_segs' is still false, we must define a default segment.
    if (!any_segs_) {
        int tempint = 0;
        if (segto != segment_by_name(kDefaultSegment, tempint)) a_.panic("strange segment conditions in OBJ driver");
    }

    // Find the segment we are targeting.
    Segment* seg = find_segment_by_index(segto);
    if (!seg) a_.panic("code directed to nonexistent segment?");

    ObjRecord* orp = seg->orp.get();
    orp->parm[0] = seg->currentpos;

    switch (type) {
    case OUT_RAWDATA: {
        const auto* ucdata = static_cast<const std::uint8_t*>(data);
        while (size > 0) {
            orp = w_.check(seg->orp.get(), 1);
            unsigned len = static_cast<unsigned>(ObjRecord::kRecordMax - orp->used);
            if (len > size) len = static_cast<unsigned>(size);
            std::memcpy(orp->buf.data() + orp->used, ucdata, len);
            orp->used += static_cast<int>(len);
            orp->committed = orp->used;
            seg->currentpos += len;
            orp->parm[0] = seg->currentpos;
            ucdata += len;
            size -= len;
        }
        break;
    }

    case OUT_ADDRESS:
    case OUT_REL1ADR:
    case OUT_REL2ADR:
    case OUT_REL4ADR:
    case OUT_REL8ADR: {
        if (type == OUT_ADDRESS) size = static_cast<std::uint64_t>(std::abs(static_cast<int>(size)));

        if (segment == NO_SEG && type != OUT_ADDRESS)
            a_.nonfatal("relative call to absolute address not supported by OBJ format");
        if (segment >= SEG_ABS) a_.nonfatal("far-absolute relocations not supported by OBJ format");

        std::int64_t datum;
        std::memcpy(&datum, data, sizeof datum);
        std::int32_t ldata = static_cast<std::int32_t>(datum);
        if (type != OUT_ADDRESS) {
            // For 16-bit and 32-bit x86 code, the size and realsize() always matches as only jumps, calls and loops
            // use PC relative addressing and the address isn't followed by any other opcode bytes. In 64-bit mode
            // there is RIP relative addressing, so the fixup location can be followed by an immediate value, meaning
            // that size > realsize().
            //
            // The linker's point of reference is the end of the fixup location; the CPU's is the end of the
            // instruction. To get the right effect the displacement is _reduced_ by the number of bytes following the
            // fixup: ldata += realsize() - size.
            ldata = static_cast<std::int32_t>(static_cast<std::uint32_t>(ldata) - static_cast<std::uint32_t>(size));
            switch (type) {
            case OUT_REL1ADR: size = 1; break;
            case OUT_REL2ADR: size = 2; break;
            case OUT_REL4ADR: size = 4; break;
            case OUT_REL8ADR: size = 8; break;
            default: break;
            }
            ldata = static_cast<std::int32_t>(static_cast<std::uint32_t>(ldata) + static_cast<std::uint32_t>(size));
        }

        switch (size) {
        default:
            a_.nonfatal("OBJ format can only handle 16- or 32-byte relocations");
            segment = NO_SEG;   // Don't actually generate a relocation
            break;
        case 2:
            orp = w_.word(orp, static_cast<unsigned>(ldata));
            break;
        case 4:
            orp = w_.dword(orp, static_cast<std::uint32_t>(ldata));
            break;
        }

        int rsize = static_cast<int>(size);
        if (segment < SEG_ABS && (segment != NO_SEG && segment % 2) && size == 4) {
            // This is a 4-byte segment-base relocation such as `MOV EAX,SEG foo'. OBJ format can't actually handle
            // these, but if the constant term has the 16 low bits zero, we can just apply a 2-byte segment-base
            // relocation to the low word instead.
            rsize = 2;
            if (ldata & 0xFFFF) a_.nonfatal("OBJ format cannot handle complex dword-size segment base references");
        }
        if (segment != NO_SEG) write_fixup(orp, rsize, (type == OUT_ADDRESS ? 0x4000 : 0), segment, wrt, seg);
        seg->currentpos += static_cast<std::uint32_t>(size);
        break;
    }

    default:
        a_.nonfatal("Relocation type not supported by output format");
        [[fallthrough]];

    case OUT_RESERVE:
        if (orp->committed) orp = w_.bump(orp);
        seg->currentpos += static_cast<std::uint32_t>(size);
        break;
    }
    ObjWriter::commit(orp);
}

void ObjFormat::write_fixup(ObjRecord* orp, int bytes, int segrel, std::int32_t seg, std::int32_t wrt, Segment* segto) {
    unsigned locat;
    int method;
    bool base;
    std::int32_t tidx, fidx;
    Segment* s = nullptr;
    Group* g = nullptr;
    External* e = nullptr;

    if (bytes != 2 && bytes != 4) {
        a_.nonfatal("`obj' output driver does not support {}-bit relocations", bytes << 3);
        return;
    }

    ObjRecord* forp = orp->child.get();
    if (forp == nullptr) {
        orp->child = w_.make();
        forp = orp->child.get();
        forp->up = &orp->child;
        // We should choose between FIXUPP and FIXU32 record type. If we're targeting a 32-bit segment, use a FIXU32.
        forp->type = segto->use32 ? FIXU32 : FIXUPP;
    }

    if (seg % 2) {
        base = true;
        locat = FIX_16_SELECTOR;
        seg--;
        if (bytes != 2) a_.panic("OBJ: 4-byte segment base fixup got through sanity check");
    } else {
        base = false;
        locat = (bytes == 2) ? FIX_16_OFFSET : FIX_32_OFFSET;
        if (!segrel)
            // There is a bug in tlink that makes it process self relative fixups incorrectly if the x_size doesn't
            // match the location size.
            forp = w_.force(forp, bytes << 3);
    }

    forp = w_.rword(forp, locat | static_cast<unsigned>(segrel) | (orp->parm[0] - orp->parm[2]));

    tidx = fidx = -1;
    method = 0;

    // See if we can find the segment ID in our segment list. If so, we have a T4 (LSEG) target.
    s = find_segment_by_index(seg);
    if (s) {
        method = 4;
        tidx = s->obj_index;
    } else {
        g = nullptr;
        for (auto& grp : groups_)
            if (grp->index == seg) {
                g = grp.get();
                break;
            }
        if (g) {
            method = 5;
            tidx = g->obj_index;
        } else {
            const std::int32_t i = seg / 2;
            if (external_block_exists(i)) {
                method = 6;
                e = external_for(i);
                if (!e) a_.panic_here();
                tidx = e->index;
            } else {
                a_.panic("unrecognised segment value in obj_write_fixup");
            }
        }
    }

    // If no WRT given, assume the natural default, which is method F5 unless:
    //  - we are doing an OFFSET fixup for a grouped segment, in which case we require F1 (group).
    //  - we are doing an OFFSET fixup for an external with a default WRT, in which case we must honour the default WRT.
    if (wrt == NO_SEG) {
        if (!base && s && s->grp) {
            method |= 0x10;
            fidx = s->grp->obj_index;
        } else if (!base && e && e->defwrt_type != DefWrt::None) {
            if (e->defwrt_type == DefWrt::Segment) {
                method |= 0x00;
                fidx = e->defwrt_seg->obj_index;
            } else if (e->defwrt_type == DefWrt::Group) {
                method |= 0x10;
                fidx = e->defwrt_grp->obj_index;
            } else {
                a_.nonfatal("default WRT specification for external `{}' unresolved", e->name);
                method |= 0x50;
                fidx = -1;   // got to do _something_
            }
        } else {
            method |= 0x50;
            fidx = -1;
        }
    } else {
        // See if we can find the WRT-segment ID in our segment list. If so, we have a F0 (LSEG) frame.
        s = find_segment_by_index(wrt - 1);
        if (s) {
            method |= 0x00;
            fidx = s->obj_index;
        } else {
            g = nullptr;
            for (auto& grp : groups_)
                if (grp->index == wrt - 1) {
                    g = grp.get();
                    break;
                }
            if (g) {
                method |= 0x10;
                fidx = g->obj_index;
            } else {
                const std::int32_t i = wrt / 2;
                if (external_block_exists(i)) {
                    External* we = external_for(i);
                    if (!we) a_.panic_here();
                    method |= 0x20;
                    fidx = we->index;
                } else {
                    a_.panic("unrecognised WRT value in obj_write_fixup");
                }
            }
        }
    }

    forp = w_.byte(forp, static_cast<std::uint8_t>(method));
    if (fidx != -1) forp = w_.index(forp, static_cast<unsigned>(fidx));
    forp = w_.index(forp, static_cast<unsigned>(tidx));
    ObjWriter::commit(forp);
}

std::uint32_t ObjFormat::check_segment_alignment(std::uint64_t origalign) {
    // Supported alignment values
    constexpr std::uint32_t alignments = 1 | 2 | 4 | 16 | 256 | 4096;
    std::uint32_t align = static_cast<std::uint32_t>(origalign);

    if (origalign && (!is_power2(origalign) || origalign > alignments)) {
        a_.nonfatal("invalid alignment value {}", origalign);
        return 1;
    }

    if (align == 0) align = 1;

    if (align & alignments) return align;   // All good!

    while (!(align & alignments)) align <<= 1;

    a_.warn(WARN_SECTION_ALIGNMENT_ROUNDED, "alignment of {} not supported, using {}", origalign, align);
    return align;
}

std::int32_t ObjFormat::section(std::optional<std::string_view> name, int& bits) {
    if (!name) {
        bits = 16;
        current_seg_ = nullptr;
        return first_seg_;
    }
    return segment_by_name(*name, bits);
}

std::int32_t ObjFormat::segment_by_name(std::string_view text, int& bits) {
    // Look for segment attributes.
    while (!text.empty() && text.front() == '.') text.remove_prefix(1);   // hack, but a documented one
    const std::size_t wl = word_length(text);
    const std::string name(text.substr(0, wl));
    std::string_view p = text.substr(wl);
    if (!p.empty()) {
        p.remove_prefix(1);
        p = skip_spaces(p);
    }

    int obj_idx = 1;
    for (auto& existing : segments_) {
        Segment* seg = existing.get();
        if (seg->name == name) {
            if (seg->pass_last_seen == a_.pass_count()) {
                if (!p.empty()) a_.warn(WARN_OTHER, "segment attributes specified on redeclaration of segment: ignoring");
            } else {
                // Reissue alignment warning on this pass if necessary
                if (seg->align < SEG_ABS) check_segment_alignment(seg->origalign);
            }
            bits = seg->use32 ? 32 : 16;
            current_seg_ = seg;
            seg->pass_last_seen = a_.pass_count();
            return seg->index;
        }
        ++obj_idx;
    }

    auto owned = std::make_unique<Segment>();
    Segment* seg = owned.get();
    segments_.push_back(std::move(owned));
    seg->index = any_segs_ ? host_.seg_alloc() : first_seg_;
    seg->obj_index = obj_idx;
    seg->grp = nullptr;
    any_segs_ = true;
    seg->name = name;
    seg->currentpos = 0;
    if (!obj2_) {
        seg->align = 1;       // default for obj
        seg->origalign = 1;
        seg->use32 = false;
    } else {
        seg->align = 16;      // default for obj2
        seg->origalign = 16;
        seg->use32 = true;
    }
    seg->combine = CMB_PUBLIC;   // default
    seg->orp = w_.make();
    seg->orp->up = &seg->orp;
    seg->orp->ori = ObjInit::Ledata;
    seg->orp->type = LEDATA;
    seg->orp->parm[1] = static_cast<std::uint32_t>(obj_idx);

    // Process the segment attributes.
    while (!p.empty()) {
        const std::string_view q = take_word(p);

        if (equals_nocase(q, "private"))
            seg->combine = CMB_PRIVATE;
        else if (equals_nocase(q, "public"))
            seg->combine = CMB_PUBLIC;
        else if (equals_nocase(q, "common"))
            seg->combine = CMB_COMMON;
        else if (equals_nocase(q, "stack"))
            seg->combine = CMB_STACK;
        else if (equals_nocase(q, "use16"))
            seg->use32 = false;
        else if (equals_nocase(q, "use32"))
            seg->use32 = true;
        else if (equals_nocase(q, "flat")) {
            // This segment is an OS/2 FLAT segment. That means that its default group is group FLAT, even if the group
            // FLAT does not explicitly _contain_ the segment. So create the group `FLAT' (containing no segments) if
            // it does not already exist, and make it the default group of this segment.
            seg->grp = ensure_group_flat();
        } else if (starts_nocase(q, "class="))
            seg->segclass = std::string(q.substr(6));
        else if (starts_nocase(q, "overlay="))
            seg->overlay = std::string(q.substr(8));
        else if (starts_nocase(q, "align=")) {
            bool rn_error = false;
            std::uint64_t n = static_cast<std::uint64_t>(a_.hooks.readnum(q.substr(6), rn_error));
            if (rn_error) {
                a_.nonfatal("segment alignment should be numeric");
                n = 1;
            }
            seg->origalign = n;
            seg->align = static_cast<std::int32_t>(check_segment_alignment(n));
        } else if (starts_nocase(q, "absolute=")) {
            bool rn_error = false;
            std::uint64_t n = static_cast<std::uint64_t>(a_.hooks.readnum(q.substr(9), rn_error));
            if (rn_error) {
                a_.nonfatal("argument to `absolute' segment attribute should be numeric");
                n = 0;
            } else if (n >= static_cast<std::uint64_t>(SEG_ABS)) {
                // Probably could even be max 64K?
                a_.nonfatal("unsupported `absolute' segment number {:#x}", n);
                n = 0;
            }
            seg->align = static_cast<std::int32_t>(SEG_ABS + n);
        }
    }

    if (!seg->use32 && seg->grp && seg->grp->name == "FLAT") a_.panic("wrong combination of USE16(16-bit segment) and FLAT");

    if (obj2_) {
        if (seg->use32 && !seg->grp) seg->grp = ensure_group_flat();
        if (!seg->segclass) {
            if (const auto segclass = default_class(seg->name)) seg->segclass = std::string(*segclass);
        }
    }

    // We need to know whenever we have at least one 32-bit segment
    use32_ = use32_ || seg->use32;

    // Trying to create a symbol on the last pass will end in tears. This can happen if there is no SEGMENT directive,
    // and there are no labels.
    if (!a_.pass_final()) {
        seg_needs_update_ = seg;
        if (seg->align >= SEG_ABS)
            host_.define_label(name, NO_SEG, seg->align - SEG_ABS, false);
        else
            host_.define_label(name, seg->index + 1, 0, false);
        seg_needs_update_ = nullptr;
    }

    // See if this segment is defined in any groups.
    for (auto& grp : groups_) {
        for (int i = grp->nindices; i < grp->nentries; i++) {
            if (grp->segs[static_cast<std::size_t>(i)].name == seg->name) {
                grp->segs[static_cast<std::size_t>(i)] = grp->segs[static_cast<std::size_t>(grp->nindices)];
                grp->segs[static_cast<std::size_t>(grp->nindices++)].index = seg->obj_index;
                // The group FLAT is a pseudo group. Therefore, it is allowed to redefine a segment in the group FLAT
                // as other group.
                if (seg->grp && seg->grp->name != "FLAT")
                    a_.warn(WARN_OTHER, "segment `{}' is already part of a group: first one takes precedence", seg->name);
                else
                    seg->grp = grp.get();
            }
        }
    }

    // Walk through the list of externals with unresolved default-WRT clauses, and resolve any that point at this segment.
    for (auto it = dws_.begin(); it != dws_.end();) {
        External* e = *it;
        if (e->defwrt_type == DefWrt::String && e->defwrt_string == seg->name) {
            e->defwrt_string.clear();
            e->defwrt_type = DefWrt::Segment;
            e->defwrt_seg = seg;
            it = dws_.erase(it);
        } else {
            ++it;
        }
    }

    bits = seg->use32 ? 32 : 16;
    current_seg_ = seg;
    return seg->index;
}

DirectiveResult ObjFormat::directive_group(std::string_view value) {
    // Is pass_first() really correct?
    std::string_view q = value;
    while (!q.empty() && q.front() == '.') q.remove_prefix(1);   // hack, but a documented one
    const std::string v(take_word(q));

    // (Here a sanity check that a group contains segments used to be; OS/2 declares empty groups as standard practice.)
    int obj_idx = 1;
    Group* grp = nullptr;
    for (auto& g : groups_) {
        obj_idx++;
        if (g->name == v) {
            grp = g.get();
            break;
        }
    }

    if (!grp) {
        auto owned = std::make_unique<Group>();
        grp = owned.get();
        groups_.push_back(std::move(owned));
        grp->index = host_.seg_alloc();
        grp->obj_index = obj_idx;
        grp->nindices = grp->nentries = 0;
        grp->name = v;   // (the label manager's name replaces it below)

        grp_needs_update_ = grp;
        host_.backend_label(v, grp->index + 1, 0);
        grp_needs_update_ = nullptr;
    }

    auto slot = [&](int i) -> GroupSeg& {
        if (grp->segs.size() <= static_cast<std::size_t>(i)) grp->segs.resize(static_cast<std::size_t>(i) + 1);
        return grp->segs[static_cast<std::size_t>(i)];
    };

    while (!q.empty()) {
        const std::string_view p = take_word(q);
        // Now p contains a segment name. Find it.
        int i;
        for (i = 0; i < grp->nentries; i++) {
            std::optional<std::string_view> segname;
            if (i < grp->nindices) {
                for (auto& seg : segments_)
                    if (slot(i).index == seg->obj_index) {
                        segname = seg->name;
                        break;
                    }
            } else {
                segname = std::string_view(slot(i).name);
            }
            // See if this segment is defined in this group.
            if (segname && *segname == p) break;
        }
        if (i < grp->nentries) continue;   // We have already this segment. Skip.

        Segment* found = nullptr;
        for (auto& seg : segments_)
            if (seg->name == p) {
                found = seg.get();
                break;
            }
        if (found) {
            // We have a segment index. Shift a name entry to the end of the array to make room.
            slot(grp->nentries) = slot(grp->nindices);
            ++grp->nentries;
            slot(grp->nindices++).index = found->obj_index;
            // The group FLAT is a pseudo group. Therefore, it is allowed to redefine a segment in the group FLAT as
            // other group.
            if (found->grp && found->grp->name != "FLAT")
                a_.warn(WARN_OTHER, "segment `{}' is already part of a group: first one takes precedence", found->name);
            else
                found->grp = grp;
        } else {
            // We have an as-yet undefined segment. Remember its name, for later.
            slot(grp->nentries++).name = std::string(p);
        }
    }

    // Walk through the list of externals with unresolved default-WRT clauses, and resolve any that point at this group.
    for (auto it = dws_.begin(); it != dws_.end();) {
        External* e = *it;
        if (e->defwrt_type == DefWrt::String && e->defwrt_string == grp->name) {
            e->defwrt_string.clear();
            e->defwrt_type = DefWrt::Group;
            e->defwrt_grp = grp;
            it = dws_.erase(it);
        } else {
            ++it;
        }
    }
    return DIRR_OK;
}

DirectiveResult ObjFormat::directive_import(std::string_view value) {
    // Is pass_first() really correct?
    std::string_view q = value;
    const std::string_view extname = take_word(q);
    const std::string_view libname = take_word(q);
    const std::string_view impname = q;

    if (extname.empty() || libname.empty()) {
        a_.nonfatal("`import' directive requires symbol name and library name");
    } else {
        ImpDef imp;
        imp.extname = std::string(extname);
        imp.libname = std::string(libname);
        bool err = false;
        imp.impindex = static_cast<unsigned>(a_.hooks.readnum(impname, err));
        if (impname.empty() || err)
            imp.impname = std::string(impname);
        else
            imp.impname = std::nullopt;
        imports_.push_back(std::move(imp));
    }
    return DIRR_OK;
}

DirectiveResult ObjFormat::directive_export(std::string_view value) {
    // Is pass_first() really correct?
    std::string_view q = value;
    std::string_view intname = take_word(q);
    std::string_view extname = take_word(q);
    int flags = 0;
    unsigned ordinal = 0;

    if (intname.empty()) {
        a_.nonfatal("`export' directive requires export name");
        return DIRR_OK;
    }
    if (extname.empty()) {
        extname = intname;
        intname = std::string_view();
    }
    while (!q.empty()) {
        const std::string_view v = take_word(q);
        if (equals_nocase(v, "resident")) {
            flags |= kExpdefResident;
        } else if (equals_nocase(v, "nodata")) {
            flags |= kExpdefNodata;
        } else if (starts_nocase(v, "parm=")) {
            bool err = false;
            flags |= kExpdefParmCnt & static_cast<int>(a_.hooks.readnum(v.substr(5), err));
            if (err) {
                a_.nonfatal("value `{}' for `parm' is non-numeric", v.substr(5));
                return DIRR_ERROR;
            }
        } else {
            bool err = false;
            ordinal = static_cast<unsigned>(a_.hooks.readnum(v, err));
            if (err) {
                a_.nonfatal("unrecognised export qualifier `{}'", v);
                return DIRR_ERROR;
            }
            flags |= kExpdefOrdinal;
        }
    }

    ExpDef exp;
    exp.extname = std::string(extname);
    exp.intname = std::string(intname);
    exp.ordinal = ordinal;
    exp.flags = flags;
    exports_.push_back(std::move(exp));
    return DIRR_OK;
}

DirectiveResult ObjFormat::directive(Directive d, std::optional<std::string_view> value) {
    switch (d) {
    case D_GROUP:
        if (value && a_.pass_first()) return directive_group(*value);
        return DIRR_OK;
    case D_UPPERCASE:
        if (value) host_.get_boolean_option(*value, w_.uppercase);
        return DIRR_OK;
    case D_IMPORT:
        if (value && a_.pass_first()) return directive_import(*value);
        return DIRR_OK;
    case D_EXPORT:
        if (value && a_.pass_first()) return directive_export(*value);
        return DIRR_OK;
    default:
        return DIRR_UNKNOWN;
    }
}

void ObjFormat::sectalign(std::int32_t seg, unsigned value) {
    Segment* s = find_segment_by_index(seg);

    // it should not be too big value and applied on non-absolute sections
    if (!s || !is_power2(value) || value > 4096 || s->align >= SEG_ABS) return;

    // FIXME (NASM): no code duplication please; consider making a helper for this mapping since the section handler
    // has to do the same.
    switch (value) {
    case 8:
        value = 16;
        break;
    case 32:
    case 64:
    case 128:
        value = 256;
        break;
    case 512:
    case 1024:
    case 2048:
        value = 4096;
        break;
    default:
        break;
    }

    if (s->align < static_cast<int>(value)) s->align = static_cast<std::int32_t>(value);
}

std::int32_t ObjFormat::segbase(std::int32_t segment) {
    // Find the segment in our list.
    Segment* seg = find_segment_by_index(segment - 1);

    if (!seg) {
        // Might be an external with a default WRT.
        const std::int32_t i = segment / 2;
        if (external_block_exists(i)) {
            External* e = external_for(i);
            if (!e) {
                // Not available yet, probably a forward reference
                if (a_.pass_final()) a_.assert_failed("!pass_final()");
                return NO_SEG;
            }
            switch (e->defwrt_type) {
            case DefWrt::None:
                return segment;   // fine
            case DefWrt::Segment:
                return e->defwrt_seg->index + 1;
            case DefWrt::Group:
                return e->defwrt_grp->index + 1;
            default:
                return NO_SEG;   // can't tell what it is
            }
        }
        return segment;   // not one of ours - leave it alone
    }

    if (seg->align >= SEG_ABS) return seg->align;           // absolute segment
    if (seg->grp) return seg->grp->index + 1;               // grouped segment
    return segment;                                         // no special treatment
}

// ---- the Borland debug format ----

// dbgbi_init: the start of a debug session (the debug format was chosen). Turns the debug records on.
void ObjFormat::debug_init() {
    debug_ = true;
    filenames_.clear();
    arrays_.clear();
    arrindex_ = kArrayBot;
    last_defined_ = nullptr;
}

void ObjFormat::debug_linnum(std::string_view lnfname, std::int32_t lineno, std::int32_t segto) {
    if (segto == NO_SEG) return;

    // If `any_segs' is still false, we must define a default segment.
    if (!any_segs_) {
        int tempint = 0;
        if (segto != segment_by_name(kDefaultSegment, tempint)) a_.panic("strange segment conditions in OBJ driver");
    }

    // Find the segment we are targeting.
    Segment* seg = find_segment_by_index(segto);
    if (!seg) a_.panic("lineno directed to nonexistent segment?");

    FileName* fn = nullptr;
    for (auto& f : filenames_)
        if (equals_nocase(lnfname, f.name)) {
            fn = &f;
            break;
        }
    if (!fn) {
        filenames_.push_back(FileName{std::string(lnfname), {}});
        fn = &filenames_.back();
    }
    fn->lines.push_back(LineNumber{seg, static_cast<std::int32_t>(seg->currentpos), lineno});
}

void ObjFormat::debug_deflabel(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global) {
    (void)is_global;
    // (Note: ..[^@] special symbols are filtered in labels.c.)
    if (is_global == 3) return;   // a special-retry from pass two: discard it

    if (seg_needs_update_ || grp_needs_update_) return;   // case (i)
    if (segment < SEG_ABS && segment != NO_SEG && segment % 2) return;
    if (segment >= SEG_ABS || segment == NO_SEG) return;

    // If `any_segs' is still false, a prior call to symdef has defined the default segment.
    for (auto& seg : segments_)
        if (seg->index == segment) {
            Public loc;
            loc.name = std::string(name);
            loc.offset = static_cast<std::int32_t>(offset);
            seg->locs.push_back(std::move(loc));
            last_defined_ = &seg->locs.back();
        }
}

void ObjFormat::debug_typevalue(std::int32_t type) {
    int vsize;
    const unsigned elem = tym_elements(static_cast<unsigned>(type));
    const unsigned ty = tym_type(static_cast<unsigned>(type));

    if (!last_defined_) return;

    switch (ty) {
    case TY_BYTE:
        last_defined_->type = 8;   // byte
        vsize = 1;
        break;
    case TY_WORD:
        last_defined_->type = 10;   // unsigned word
        vsize = 2;
        break;
    case TY_DWORD:
        last_defined_->type = 12;   // unsigned dword
        vsize = 4;
        break;
    case TY_FLOAT:
        last_defined_->type = 14;   // float
        vsize = 4;
        break;
    case TY_QWORD:
        last_defined_->type = 15;   // qword
        vsize = 8;
        break;
    case TY_TBYTE:
        last_defined_->type = 16;   // TBYTE
        vsize = 10;
        break;
    default:
        last_defined_->type = 0x19;   // label
        vsize = 0;
        break;
    }

    if (elem > 1) {
        ArrayType arr;
        const int vtype = last_defined_->type;
        arr.size = static_cast<unsigned>(vsize) * elem;
        arr.basetype = vtype;
        last_defined_->type = arrindex_++;
        arrays_.push_back(arr);
    }
    last_defined_ = nullptr;
}

// ---- writing the module ----

void ObjFormat::cleanup() {
    out_.clear();
    write_file();
    Bytes& ofile = host_.ofile();
    ofile.insert(ofile.end(), out_.begin(), out_.end());
}

// ---- the factory (format.hpp) ----

std::unique_ptr<OutputFormat> make_obj_format(FormatHost& host, std::string_view shortname) {
    if (shortname == "obj") return std::make_unique<ObjFormat>(host, false);
    if (shortname == "obj2") return std::make_unique<ObjFormat>(host, true);
    return nullptr;
}

void ObjFormat::write_file() {
    Segment* entry_seg_ptr = nullptr;
    Segment* seg = nullptr;
    const bool debuginfo = debug_;

    // Write the THEADR module header.
    std::unique_ptr<ObjRecord> orp_owner = w_.make();
    ObjRecord* orp = orp_owner.get();
    orp->type = THEADR;
    w_.name(orp, infile_);
    w_.emit2(orp);

    // Write the NASM boast comment.
    orp->type = COMENT;
    w_.rword(orp, dTRANSL);
    w_.name(orp, a_.reproducible ? std::string_view("The Netwide Assembler")
                                 : std::string_view("The Netwide Assembler 3.02"));
    w_.emit2(orp);

    // Output file dependency information
    if (!nodepend_) {
        for (const DependencyFile& depfile : host_.dependency_files()) {
            if (depfile.dos_timestamp) {
                orp->type = COMENT;
                w_.rword(orp, dDEPFILE);
                w_.dword(orp, depfile.dos_timestamp);
                w_.name(orp, depfile.name);
                w_.emit2(orp);
            }
        }
    }

    orp->type = COMENT;
    // Write the IMPDEF records, if any.
    for (const ImpDef& imp : imports_) {
        w_.rword(orp, dOMFEXT);
        w_.byte(orp, 1);   // subfunction 1: IMPDEF
        if (imp.impname)
            w_.byte(orp, 0);   // import by name
        else
            w_.byte(orp, 1);   // import by ordinal
        w_.name(orp, imp.extname);
        w_.name(orp, imp.libname);
        if (imp.impname)
            w_.name(orp, *imp.impname);
        else
            w_.word(orp, imp.impindex);
        w_.emit2(orp);
    }

    // Write the EXPDEF records, if any.
    for (const ExpDef& exp : exports_) {
        w_.rword(orp, dOMFEXT);
        w_.byte(orp, 2);   // subfunction 2: EXPDEF
        w_.byte(orp, static_cast<std::uint8_t>(exp.flags));
        w_.name(orp, exp.extname);
        w_.name(orp, exp.intname);
        if (exp.flags & kExpdefOrdinal) w_.word(orp, exp.ordinal);
        w_.emit2(orp);
    }

    // we're using extended OMF if we put in debug info
    if (debuginfo) {
        orp->type = COMENT;
        w_.rword(orp, dEXTENDED);
        w_.emit2(orp);
    }

    // Write the first LNAMES record, containing LNAME one, which is null. Also initialize the LNAME counter.
    orp->type = LNAMES;
    w_.byte(orp, 0);
    int lname_idx = 1;
    // Write some LNAMES for the segment names
    for (auto& s : segments_) {
        orp = w_.name(orp, s->name);
        if (s->segclass) orp = w_.name(orp, *s->segclass);
        if (s->overlay) orp = w_.name(orp, *s->overlay);
        ObjWriter::commit(orp);
    }
    // Write some LNAMES for the group names
    for (auto& grp : groups_) {
        orp = w_.name(orp, grp->name);
        ObjWriter::commit(orp);
    }
    w_.emit(orp);

    // Write the SEGDEF records.
    orp->type = SEGDEF;
    for (auto& sp : segments_) {
        Segment* s = sp.get();
        unsigned acbp;
        std::uint32_t seglen = s->currentpos;

        acbp = static_cast<unsigned>(s->combine) << 2;   // C field

        if (s->use32) {
            acbp |= 0x01;   // P bit is Use32 flag
        } else if (seglen == 0x10000L) {
            seglen = 0;     // This special case may be needed for old linkers
            acbp |= 0x02;   // B bit
        }

        // A field
        if (s->align >= SEG_ABS) {
            /* acbp |= 0x00 */
        } else if (s->align >= 4096) {
            if (s->align > 4096) a_.nonfatal("segment `{}' requires more alignment than OBJ format supports", s->name);
            acbp |= 0xC0;   // PharLap extension
        } else if (s->align >= 256) {
            acbp |= 0x80;
        } else if (s->align >= 16) {
            acbp |= 0x60;
        } else if (s->align >= 4) {
            acbp |= 0xA0;
        } else if (s->align >= 2) {
            acbp |= 0x40;
        } else {
            acbp |= 0x20;
        }

        w_.byte(orp, static_cast<std::uint8_t>(acbp));
        if (s->align & SEG_ABS) {
            w_.x(orp, static_cast<std::uint32_t>(s->align - SEG_ABS));   // Frame
            w_.byte(orp, 0);                                             // Offset
        }
        w_.x(orp, seglen);
        w_.index(orp, static_cast<unsigned>(++lname_idx));
        w_.index(orp, s->segclass ? static_cast<unsigned>(++lname_idx) : 1u);
        w_.index(orp, s->overlay ? static_cast<unsigned>(++lname_idx) : 1u);
        w_.emit2(orp);
    }

    // Write the GRPDEF records.
    orp->type = GRPDEF;
    for (auto& grp : groups_) {
        if (grp->nindices != grp->nentries) {
            for (int i = grp->nindices; i < grp->nentries; i++) {
                GroupSeg& gs = grp->segs[static_cast<std::size_t>(i)];
                a_.nonfatal("group `{}' contains undefined segment `{}'", grp->name, gs.name);
                gs.name.clear();
            }
        }
        w_.index(orp, static_cast<unsigned>(++lname_idx));
        for (int i = 0; i < grp->nindices; i++) {
            w_.byte(orp, 0xFF);
            w_.index(orp, static_cast<unsigned>(grp->segs[static_cast<std::size_t>(i)].index));
        }
        w_.emit2(orp);
    }

    // Write the PUBDEF records: first the ones in the segments, then the far-absolutes.
    orp->type = PUBDEF;
    orp->ori = ObjInit::Pubdef;
    for (auto& sp : segments_) {
        Segment* s = sp.get();
        orp->parm[0] = s->grp ? static_cast<std::uint32_t>(s->grp->obj_index) : 0;
        orp->parm[1] = static_cast<std::uint32_t>(s->obj_index);
        for (const Public& pub : s->pubs) {
            orp = w_.name(orp, pub.name);
            orp = w_.x(orp, static_cast<std::uint32_t>(pub.offset));
            orp = w_.byte(orp, 0);   // type index
            ObjWriter::commit(orp);
        }
        w_.emit(orp);
    }
    orp->parm[0] = 0;
    orp->parm[1] = 0;
    for (const Public& pub : fpubs_) {   // pub-crawl :-)
        if (orp->parm[2] != static_cast<std::uint32_t>(pub.segment)) {
            w_.emit(orp);
            orp->parm[2] = static_cast<std::uint32_t>(pub.segment);
        }
        orp = w_.name(orp, pub.name);
        orp = w_.x(orp, static_cast<std::uint32_t>(pub.offset));
        orp = w_.byte(orp, 0);   // type index
        ObjWriter::commit(orp);
    }
    w_.emit(orp);

    // Write the EXTDEF and COMDEF records, in order.
    orp->ori = ObjInit::Null;
    for (auto& ep : externals_) {
        External* ext = ep.get();
        if (ext->commonsize == 0) {
            if (orp->type != EXTDEF) {
                w_.emit(orp);
                orp->type = EXTDEF;
            }
            orp = w_.name(orp, ext->name);
            orp = w_.index(orp, 0);
        } else {
            if (orp->type != COMDEF) {
                w_.emit(orp);
                orp->type = COMDEF;
            }
            orp = w_.name(orp, ext->name);
            orp = w_.index(orp, 0);
            if (ext->commonelem) {
                orp = w_.byte(orp, 0x61);   // far communal
                orp = w_.value(orp, static_cast<std::uint32_t>(ext->commonsize / ext->commonelem));
                orp = w_.value(orp, static_cast<std::uint32_t>(ext->commonelem));
            } else {
                orp = w_.byte(orp, 0x62);   // near communal
                orp = w_.value(orp, static_cast<std::uint32_t>(ext->commonsize));
            }
        }
        ObjWriter::commit(orp);
    }
    w_.emit(orp);

    // Write a COMENT record stating that the linker's first pass may stop processing at this point. Exception is if
    // our MODEND record specifies a start point, in which case, according to some variants of the documentation, this
    // COMENT should be omitted. So we'll omit it just in case. But, TASM puts it in all the time so if we are using
    // TASM debug stuff we are putting it in.
    if (debuginfo || entry_seg_ == NO_SEG) {
        orp->type = COMENT;
        w_.rword(orp, dLINKPASS);
        w_.byte(orp, 1);
        w_.emit2(orp);
    }

    // 1) put out the compiler type
    // 2) Put out the type info.  The only type we are using is near label #19
    if (debuginfo) {
        orp->type = COMENT;
        w_.rword(orp, dCOMPDEF);
        w_.byte(orp, 4);
        w_.byte(orp, 0);
        w_.emit2(orp);

        auto typedef_header = [&](unsigned number, unsigned size) {
            w_.rword(orp, dTYPEDEF);
            w_.word(orp, number);   // type # for linking
            w_.word(orp, size);     // size of type
        };
        typedef_header(0x18, 6);
        w_.byte(orp, 0x2a);   // absolute type for debugging
        w_.emit2(orp);
        typedef_header(0x19, 0);
        w_.byte(orp, 0x24);   // absolute type for debugging
        w_.byte(orp, 0);      // near/far specifier
        w_.emit2(orp);
        typedef_header(0x1A, 0);
        w_.byte(orp, 0x24);
        w_.byte(orp, 1);
        w_.emit2(orp);
        typedef_header(0x1b, 0);
        w_.byte(orp, 0x23);
        w_.byte(orp, 0);
        w_.byte(orp, 0);
        w_.byte(orp, 0);
        w_.emit2(orp);
        typedef_header(0x1c, 0);
        w_.byte(orp, 0x23);
        w_.byte(orp, 0);
        w_.byte(orp, 4);
        w_.byte(orp, 0);
        w_.emit2(orp);
        typedef_header(0x1d, 0);
        w_.byte(orp, 0x23);
        w_.byte(orp, 0);
        w_.byte(orp, 1);
        w_.byte(orp, 0);
        w_.emit2(orp);
        typedef_header(0x1e, 0);
        w_.byte(orp, 0x23);
        w_.byte(orp, 0);
        w_.byte(orp, 5);
        w_.byte(orp, 0);
        w_.emit2(orp);

        // put out the array types
        std::size_t arr = 0;
        for (int i = kArrayBot; i < arrindex_; i++, arr++) {
            w_.rword(orp, dTYPEDEF);
            w_.word(orp, static_cast<unsigned>(i));   // type # for linking
            w_.word(orp, arrays_[arr].size);          // size of type
            w_.byte(orp, 0x1A);                       // absolute type for debugging (array)
            w_.byte(orp, static_cast<std::uint8_t>(arrays_[arr].basetype));   // base type
            w_.emit2(orp);
        }
    }

    // write out line number info with a LINNUM record switch records when we switch segments, and output the file in
    // a pseudo-TASM fashion. The record switch is naive; that is that one file may have many records for the same
    // segment if there are lots of segment switches
    if (!filenames_.empty() && debuginfo && !filenames_.front().lines.empty()) {
        seg = filenames_.front().lines.front().segment;

        for (const FileName& fn : filenames_) {
            // write out current file name
            orp->type = COMENT;
            orp->ori = ObjInit::Null;
            w_.rword(orp, dFILNAME);
            w_.byte(orp, 0);
            w_.name(orp, fn.name);
            w_.dword(orp, 0);
            w_.emit2(orp);

            // write out line numbers this file
            orp->type = LINNUM;
            orp->ori = ObjInit::Linnum;
            for (const LineNumber& ln : fn.lines) {
                if (seg != ln.segment) {
                    // if we get here have to flush the buffer and start a new record for a new segment
                    seg = ln.segment;
                    w_.emit(orp);
                }
                orp->parm[0] = seg->grp ? static_cast<std::uint32_t>(seg->grp->obj_index) : 0;
                orp->parm[1] = static_cast<std::uint32_t>(seg->obj_index);
                orp = w_.word(orp, static_cast<unsigned>(ln.lineno));
                orp = w_.x(orp, static_cast<std::uint32_t>(ln.offset));
                ObjWriter::commit(orp);
            }
            w_.emit(orp);
        }
    }

    // we are going to locate the entry point segment now rather than wait until the MODEND record, because, then we
    // can output a special symbol to tell where the entry point is.
    if (entry_seg_ != NO_SEG) {
        seg = nullptr;
        for (auto& s : segments_) {
            if (s->index == entry_seg_) {
                entry_seg_ptr = s.get();
                seg = s.get();
                break;
            }
        }
        if (!seg) a_.nonfatal("entry point is not in this module");
    }

    // get ready to put out symbol records
    orp->type = COMENT;
    orp->ori = ObjInit::Local;

    // put out a symbol for the entry point; no dots in this symbol, because Borland does not (officially) support dots
    // in label names and I don't know what various versions of TLINK will do
    if (debuginfo && entry_seg_ != NO_SEG && seg) {
        orp = w_.name(orp, "start_of_program");
        orp = w_.word(orp, 0x19);   // type: near label
        orp = w_.index(orp, seg->grp ? static_cast<unsigned>(seg->grp->obj_index) : 0u);
        orp = w_.index(orp, static_cast<unsigned>(seg->obj_index));
        orp = w_.x(orp, static_cast<std::uint32_t>(entry_ofs_));
        ObjWriter::commit(orp);
    }

    // put out the local labels
    if (debuginfo) {
        for (auto& s : segments_) {
            // labels this seg
            for (const Public& loc : s->locs) {
                orp = w_.name(orp, loc.name);
                orp = w_.word(orp, static_cast<unsigned>(loc.type));
                orp = w_.index(orp, s->grp ? static_cast<unsigned>(s->grp->obj_index) : 0u);
                orp = w_.index(orp, static_cast<unsigned>(s->obj_index));
                orp = w_.x(orp, static_cast<std::uint32_t>(loc.offset));
                ObjWriter::commit(orp);
            }
        }
    }
    if (orp->used) w_.emit(orp);

    // Write the LEDATA/FIXUPP pairs.
    for (auto& s : segments_) {
        w_.emit(s->orp.get());
        s->orp.reset();
    }

    // Write the MODEND module end marker.
    orp->type = use32_ ? MODE32 : MODEND;
    orp->ori = ObjInit::Null;
    if (entry_seg_ptr) {
        orp->type = entry_seg_ptr->use32 ? MODE32 : MODEND;
        w_.byte(orp, 0xC1);
        seg = entry_seg_ptr;
        if (seg->grp) {
            w_.byte(orp, 0x10);
            w_.index(orp, static_cast<unsigned>(seg->grp->obj_index));
        } else {
            // the below changed to prevent TLINK crashing. Previous more efficient version read: obj_byte (orp, 0x50);
            w_.byte(orp, 0x00);
            w_.index(orp, static_cast<unsigned>(seg->obj_index));
        }
        w_.index(orp, static_cast<unsigned>(seg->obj_index));
        w_.x(orp, static_cast<std::uint32_t>(entry_ofs_));
    } else {
        w_.byte(orp, 0);
    }
    w_.emit2(orp);
}

}  // namespace cb::build::x86asm
