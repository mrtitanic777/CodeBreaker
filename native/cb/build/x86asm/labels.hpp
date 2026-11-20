// cb/build/x86asm/labels.hpp -- the label table (asm/labels.c, include/labels.h).
//
// Every label of the program: where it is (a segment number and an offset in it), whether it is local to the code, static,
// global, extern, required, common or one of NASM's special symbols, and where it was defined. The assembler makes several
// passes over the source; a label is defined again in every pass, and when its offset differs from what the previous pass
// found the table counts a change (`Assembly::global_offset_changed`) so the driver runs another pass. After the code
// generating pass a label that still moves is reported ("label `x' changed during code generation").
//
// A name starting with one period (`.loop`) is local to the last non-local label before it: it is stored as `prev.loop`.
// Names starting with two periods are NASM's own (`..start`, `..@12.x`, `..gotpc`). In --masm mode names are matched
// without regard to case, but the spelling of the defining occurrence is what the object file shows.
//
// The table holds no global state: it is one object per assembly run, which reads and writes the run's `Assembly`.
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

class Assembly;

// What the table calls out to: the output format, the debug format, and the segment allocator. Unset ones do nothing.
struct LabelHooks {
    // The output format's symdef(name, segment, offset, is_global, special): a symbol is defined. `is_global` is 0 for a
    // local, 1 global, 2 common (the offset is then the size), 3 a special fixup of a global/common.
    std::function<void(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special)> symdef;
    // The debug format's debug_deflabel, with the same arguments. (Unset: the null debug format.)
    std::function<void(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special)> debug_deflabel;
    // The output format's herelabel: a label is defined at the current location, and the format may move it to another
    // section; returns the segment it is now in. Unset: the format has none (NASM's null pointer).
    std::function<std::int32_t(std::string_view name, LabelType type, std::int32_t segment, std::int32_t& subsection, bool& copyoffset)> herelabel;
    // segalloc.c's seg_alloc: a fresh segment number, for the pseudo-segment of an extern, required or common symbol.
    std::function<std::int32_t()> seg_alloc;
};

// Which of the four label name decorations a directive sets: `[PREFIX]`/`[GPREFIX]`, `[SUFFIX]`/`[POSTFIX]`/`[GSUFFIX]`/
// `[GPOSTFIX]`, `[LPREFIX]`, `[LSUFFIX]`/`[LPOSTFIX]` (set_label_mangle).
enum class LabelMangle { GlobalPrefix, GlobalSuffix, LocalPrefix, LocalSuffix };

class LabelTable {
public:
    explicit LabelTable(Assembly& assembly) : as_(assembly) {}

    LabelHooks hooks;

    // Start with an empty table (init_labels). Until this is called (and after `cleanup`) lookups find nothing; the other
    // calls start the table themselves if needed.
    void init();
    // Throw everything away (cleanup_labels).
    void cleanup();

    // The type of a label that has been defined in an earlier pass or earlier in this one, with its segment and offset;
    // LBL_none, leaving them alone, for one that is not (lookup_label). Marks it as referenced in this pass.
    LabelType lookup_label(std::string_view label, std::int32_t& segment, std::int64_t& offset);

    // Define a label at `segment:offset` (define_label). A segment of 0 says it is an extern, required or common symbol:
    // it gets a pseudo-segment of its own. `normal` says whether it sets the base for the local labels that follow (false
    // for labels the backend makes). Counts a change of place in `Assembly::global_offset_changed`, and reports a
    // redefinition or a label that moves during code generation with NASM's messages.
    void define_label(std::string_view label, std::int32_t segment, std::int64_t offset, bool normal);

    // Define a special label for the output format (backend_label).
    void backend_label(std::string_view label, std::int32_t segment, std::int64_t offset);

    // Declare a label GLOBAL, EXTERN, REQUIRED, COMMON ... (declare_label), with the output format's attribute text if any
    // (`special`, "" for none). Returns false when define_label must not be called after it (an EXTERN of a symbol that is
    // already GLOBAL), or on an error ("symbol `x' declared both as global and extern" and the like).
    bool declare_label(std::string_view label, LabelType type, std::string_view special = {});

    // Set a prefix or suffix for label names handed to the output format; once set it does not change (set_label_mangle).
    void set_label_mangle(LabelMangle which, std::string_view what);

    // The prefix a local label name gets: the last non-local label's name for `.local`, else "" (local_scope).
    std::string local_scope(std::string_view label) const;

    // The label the local labels are currently relative to.
    const std::string& prev_label() const { return prevlabel_; }

    // How many labels there are.
    std::size_t size() const { return labels_.size(); }

private:
    struct Label {
        std::int32_t segment = 0;
        std::int32_t subsection = NO_SEG;   // available for the output format's herelabel
        std::int64_t offset = 0;
        std::int64_t size = 0;
        std::int64_t defined = 0;   // 0 if undefined, pass number + 1 for when the definition was seen
        std::int64_t lastref = 0;   // the last pass where a reference was seen
        std::string label;          // as written (with the local prefix added)
        std::string mangled;        // the name with its prefix and suffix; "" until made
        std::string special;        // "" for none
        SrcLocation def;            // where defined
        LabelType type = LBL_LOCAL;
        LabelType mangled_type = LBL_LOCAL;
    };

    bool islocal(std::string_view l) const;
    static bool ismagic(std::string_view l);
    bool set_prevlabel(std::string_view l) const;
    void out_symdef(Label& l);
    Label* find_label(std::string_view label, bool create, bool* created);
    const std::string& mangle_label_name(Label& l);
    void handle_herelabel(Label& l, std::int32_t& segment, std::int64_t& offset);
    bool declare_label_lptr(Label& l, LabelType type, std::string_view special);

    Assembly& as_;
    bool initialized_ = false;
    std::deque<Label> labels_;   // a deque: references stay valid as labels are added
    std::unordered_map<std::string, std::size_t> index_;   // key (folded in MASM mode) -> position in labels_
    std::string prevlabel_;
    std::string mangle_[4];      // by LabelMangle
    bool mangle_set_[4] = {false, false, false, false};
};

}  // namespace cb::build::x86asm
