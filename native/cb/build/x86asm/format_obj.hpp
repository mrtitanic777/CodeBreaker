// cb/build/x86asm/format_obj.hpp -- the OMF object format (`-f obj`, and `-f obj2` for 32-bit OS/2): the port of NASM's
// output/outobj.c.
//
// An OMF object is a sequence of records (THEADR, COMENT, LNAMES, SEGDEF, GRPDEF, PUBDEF, EXTDEF/COMDEF, LEDATA with its
// FIXUPP, MODEND). The first half of outobj.c is a record builder: an `ObjRecord` fills in the bytes of one record, and
// when a record gets full (1021 bytes) it carries on in the next one, moving the data written since the last "commit"
// point along so no field is split. `ObjWriter` is that half. The second half, `ObjFormat`, takes the calls of the
// assembler core (segments, groups, labels, data and relocations) and keeps the segment, group, public, external,
// import and export tables the records are written from when the module ends.
//
// The Borland debug records of outobj.c (`-g`, line numbers and local symbols) are ported too: the core calls
// debug_linnum / debug_deflabel / debug_typevalue only when a debug format was asked for.
#pragma once

#include <array>
#include <deque>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/directives.hpp"
#include "cb/build/x86asm/format.hpp"

namespace cb::build::x86asm {

// ---- the record builder (the first half of outobj.c) ----

// OMF record types and the comment classes.
namespace omf {
inline constexpr unsigned THEADR = 0x80, COMENT = 0x88, LINNUM = 0x94, LNAMES = 0x96, SEGDEF = 0x98, GRPDEF = 0x9A,
                          EXTDEF = 0x8C, PUBDEF = 0x90, COMDEF = 0xB0, LEDATA = 0xA0, FIXUPP = 0x9C, FIXU32 = 0x9D,
                          MODEND = 0x8A, MODE32 = 0x8B;
inline constexpr unsigned dTRANSL = 0x0000, dOMFEXT = 0xC0A0, dEXTENDED = 0xC0A1, dLINKPASS = 0x40A2, dTYPEDEF = 0xC0E3,
                          dSYM = 0xC0E6, dFILNAME = 0xC0E8, dDEPFILE = 0xC0E9, dCOMPDEF = 0xC0EA;
}  // namespace omf

// What to write at the start of each record of a continued run (an ObjRecord's `ori`).
enum class ObjInit : int {
    Null,     // nothing
    Ledata,   // segment index, then the offset (parm[1] = segment index, parm[0] = offset)
    Pubdef,   // group index, segment index and, when both are zero, the frame (parm[0], parm[1], parm[2])
    Linnum,   // group index, segment index (parm[0], parm[1])
    Local     // the local-symbols comment class
};

// One object record under construction or complete.
struct ObjRecord {
    static constexpr int kRecordMax = 1024 - 3;   // the most data in a record (past its type and length)
    static constexpr int kParms = 3;

    ObjInit ori = ObjInit::Null;                // how a continuation record starts
    int used = 0;                               // current data size
    int committed = 0;                          // data size at the last boundary
    int x_size = 0;                             // 16 or 32 once a size-dependent field has been written (obj_x)
    unsigned type = 0;                          // the record type
    std::unique_ptr<ObjRecord> child;           // the record written right after this one (its FIXUPP)
    std::unique_ptr<ObjRecord>* up = nullptr;   // the master slot this record is kept in, if it is saved when continued
    std::unique_ptr<ObjRecord> back;            // the previous part of this record
    std::array<std::uint32_t, kParms> parm{};   // parameters for the initializer
    std::array<std::uint8_t, kRecordMax + 3> buf{};
};

// The record builder: appends into `out` as records are emitted. The functions that can continue a record return the
// record to carry on with (the same one unless it moved to a new part).
class ObjWriter {
public:
    ObjWriter(Assembly& assembly, Bytes& out) : a_(assembly), out_(out) {}

    bool uppercase = false;   // write all names in upper case

    std::unique_ptr<ObjRecord> make();                         // obj_new
    ObjRecord* clear(ObjRecord* orp);                          // obj_clear
    ObjRecord* emit(ObjRecord* orp);                           // obj_emit
    ObjRecord* emit2(ObjRecord* orp);                          // obj_emit2: commit, then emit
    ObjRecord* bump(ObjRecord* orp);                           // obj_bump
    ObjRecord* check(ObjRecord* orp, int size);                // obj_check
    static ObjRecord* commit(ObjRecord* orp);                  // obj_commit
    ObjRecord* byte(ObjRecord* orp, std::uint8_t value);       // obj_byte
    ObjRecord* word(ObjRecord* orp, unsigned value);           // obj_word
    ObjRecord* rword(ObjRecord* orp, unsigned value);          // obj_rword: high byte first
    ObjRecord* dword(ObjRecord* orp, std::uint32_t value);     // obj_dword
    ObjRecord* force(ObjRecord* orp, int x);                   // obj_force
    ObjRecord* x(ObjRecord* orp, std::uint32_t value);         // obj_x: a word, or a dword in a 32-bit record
    ObjRecord* index(ObjRecord* orp, unsigned value);          // obj_index
    ObjRecord* value(ObjRecord* orp, std::uint32_t value);     // obj_value: a variable length value
    ObjRecord* name(ObjRecord* orp, std::string_view name);    // obj_name: a counted string

private:
    void run_init(ObjRecord& orp);
    void write_record(const ObjRecord& orp);                   // obj_fwrite

    Assembly& a_;
    Bytes& out_;
};

// ---- the format (the second half of outobj.c) ----

class ObjFormat final : public OutputFormat {
public:
    // `obj2` is `-f obj2`: 32-bit segments in group FLAT by default, with the Unix section names mapped to OMF ones.
    ObjFormat(FormatHost& host, bool obj2 = false);
    ~ObjFormat() override;

    std::string_view fullname() const override;
    std::string_view shortname() const override { return obj2_ ? "obj2" : "obj"; }
    std::string_view extension() const override { return ".obj"; }
    unsigned flags() const override { return 0; }
    int maxbits() const override { return 32; }
    std::vector<const DebugFormat*> debug_formats() const override { return {&borland_, &null_debug_format()}; }
    const DebugFormat* default_dfmt() const override { return &borland_; }
    const std::vector<std::string>& stdmac() const override;
    const std::vector<PragmaFacility>& pragmas() const override { return pragmas_; }

    void init() override;
    void output(const OutData& data) override;
    void symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global,
                std::string_view special) override;
    std::int32_t section(std::optional<std::string_view> name, int& bits) override;
    void sectalign(std::int32_t segment, unsigned value) override;
    std::int32_t segbase(std::int32_t segment) override;
    DirectiveResult directive(Directive directive, std::optional<std::string_view> value) override;
    void cleanup() override;

    // The Borland debug format (`-g`): the core calls these through `borland_` only when it was chosen (-g, -F borland).
    void debug_linnum(std::string_view filename, std::int32_t lineno, std::int32_t segto);
    void debug_deflabel(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global);
    void debug_typevalue(std::int32_t type);

private:
    // A public symbol (or, in the debug lists, a local one with its debug type).
    struct Public {
        std::string name;
        std::int32_t offset = 0;
        std::int32_t segment = 0;   // only if it's far-absolute
        int type = 0;               // only for local debug symbols
    };
    struct Segment;
    struct Group;
    enum class DefWrt { None, String, Segment, Group };
    struct External {
        std::string name;
        std::int32_t commonsize = 0;
        std::int32_t commonelem = 0;   // element size if FAR, else zero
        int index = 0;                 // the OBJ-file external index
        DefWrt defwrt_type = DefWrt::None;
        std::string defwrt_string;     // DefWrt::String: a name not yet understood
        Segment* defwrt_seg = nullptr;
        Group* defwrt_grp = nullptr;
    };
    enum Combine : int { CMB_PRIVATE = 0, CMB_PUBLIC = 2, CMB_STACK = 5, CMB_COMMON = 6 };
    struct Segment {
        std::string name;
        std::int32_t index = 0;        // the NASM segment id
        std::int32_t obj_index = 0;    // the OBJ-file segment index
        Group* grp = nullptr;          // the group it belongs to
        std::uint32_t currentpos = 0;
        std::int32_t align = 0;        // can be SEG_ABS + absolute address
        std::uint64_t origalign = 0;   // originally requested alignment
        std::int64_t pass_last_seen = 0;
        std::vector<Public> pubs;      // public symbols
        std::deque<Public> locs;       // local symbols (debug only; stable addresses)
        std::optional<std::string> segclass, overlay;
        std::unique_ptr<ObjRecord> orp;
        Combine combine = CMB_PUBLIC;
        bool use32 = false;
    };
    // One entry of a group: the OBJ index of a segment already seen, or the name of one not yet defined.
    struct GroupSeg {
        std::int32_t index = 0;
        std::string name;
    };
    struct Group {
        std::string name;
        std::int32_t index = 0;        // the NASM segment id
        std::int32_t obj_index = 0;    // the OBJ-file group index
        int nentries = 0;              // number of elements...
        int nindices = 0;              // ...and number of index elements; the first nindices entries are indices, the rest names
        std::vector<GroupSeg> segs;
    };
    struct ImpDef {
        std::string extname, libname;
        unsigned impindex = 0;
        std::optional<std::string> impname;
    };
    struct ExpDef {
        std::string intname, extname;
        unsigned ordinal = 0;
        int flags = 0;
    };
    struct LineNumber {
        Segment* segment = nullptr;
        std::int32_t offset = 0;
        std::int32_t lineno = 0;
    };
    struct FileName {
        std::string name;
        std::vector<LineNumber> lines;
    };
    struct ArrayType {
        unsigned size = 0;
        int basetype = 0;
    };

    static constexpr int kExtBlock = 256;       // block size of the externals list
    static constexpr int kArrayBot = 31;        // first array type index
    static constexpr int kExpdefOrdinal = 0x80, kExpdefResident = 0x40, kExpdefNodata = 0x20, kExpdefParmCnt = 0x1F;

    void obj_ext_set_defwrt(External& ext, std::string id);
    External* external_for(std::int32_t index_div2);   // the externals slot for segment/2 (nullptr: no such block or empty)
    bool external_block_exists(std::int32_t index_div2) const;
    void write_fixup(ObjRecord* orp, int bytes, int segrel, std::int32_t seg, std::int32_t wrt, Segment* segto);
    std::uint32_t check_segment_alignment(std::uint64_t origalign);
    Group* find_group(std::string_view name);
    Group* ensure_group_flat();
    Segment* find_segment_by_index(std::int32_t index);
    void write_file();
    void define_default_segment();
    std::int32_t segment_by_name(std::string_view name_with_attributes, int& bits);
    void debug_init();
    DirectiveResult directive_group(std::string_view value);
    DirectiveResult directive_import(std::string_view value);
    DirectiveResult directive_export(std::string_view value);

    // The Borland debug format object: a DebugFormat whose hooks are this format's debug_* functions.
    class BorlandDebug final : public DebugFormat {
    public:
        explicit BorlandDebug(ObjFormat& owner) : owner_(owner) {}
        std::string_view fullname() const override { return "Borland Debug Records"; }
        std::string_view shortname() const override { return "borland"; }
        void init() override { owner_.debug_init(); }
        void linenum(std::string_view filename, std::int32_t lineno, std::int32_t segto) override {
            owner_.debug_linnum(filename, lineno, segto);
        }
        void deflabel(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global,
                      std::string_view special) override {
            (void)special;
            owner_.debug_deflabel(name, segment, offset, is_global);
        }
        void typevalue(std::int32_t type) override { owner_.debug_typevalue(type); }

    private:
        ObjFormat& owner_;
    };

    FormatHost& host_;
    Assembly& a_;
    bool obj2_;
    Bytes out_;
    ObjWriter w_;
    BorlandDebug borland_{*this};
    std::vector<PragmaFacility> pragmas_;

    std::string infile_;                  // the input file name (THEADR)
    std::int32_t first_seg_ = 0;
    bool any_segs_ = false;
    bool use32_ = false;                  // at least one segment is 32-bit
    bool nodepend_ = false;               // don't emit file dependencies
    bool debug_ = false;                  // the Borland debug format is on

    std::vector<Public> fpubs_;           // far-absolute publics
    std::vector<std::unique_ptr<External>> externals_;
    std::vector<External*> ext_slots_;    // external by segment/2, in blocks of kExtBlock (ExtBack)
    std::vector<External*> dws_;          // externals with a default WRT string not yet resolved, newest first
    std::vector<std::unique_ptr<Segment>> segments_;
    std::vector<std::unique_ptr<Group>> groups_;
    std::vector<ImpDef> imports_;
    std::vector<ExpDef> exports_;
    std::int32_t entry_seg_ = NO_SEG, entry_ofs_ = 0;
    Segment* current_seg_ = nullptr;
    Segment* seg_needs_update_ = nullptr;
    Group* grp_needs_update_ = nullptr;

    // Borland debug records.
    std::vector<FileName> filenames_;
    std::vector<ArrayType> arrays_;
    int arrindex_ = kArrayBot;
    Public* last_defined_ = nullptr;
};

}  // namespace cb::build::x86asm
