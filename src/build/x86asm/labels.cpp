// build/x86asm/labels.cpp -- the label table; see labels.hpp. A port of asm/labels.c, with the --masm case folding.
#include "build/x86asm/labels.hpp"

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/nctype.hpp"

namespace cb::build::x86asm {

namespace {

// The names of the label types, for messages (types[]).
constexpr const char* labels_type_names[] = {"local", "static", "global", "extern", "required", "common", "special", "output format special"};

const char* labels_type_name(LabelType t) { return labels_type_names[static_cast<int>(t)]; }

bool labels_is_global(LabelType type) { return type == LBL_GLOBAL || type == LBL_COMMON; }

}  // namespace

// A dot-local label is one that begins with exactly one period. Things that begin with _two_ periods are NASM-specific things.
// If TASM compatibility is enabled, a local label can also begin with @@.
bool LabelTable::islocal(std::string_view l) const {
    if (as_.tasm_compatible_mode) {
        if (l.size() >= 2 && l[0] == '@' && l[1] == '@') return true;
    }
    return l.size() >= 1 && l[0] == '.' && !(l.size() >= 2 && l[1] == '.');
}

// True if this falls into NASM's '..' namespace (but not the ..@ names).
bool LabelTable::ismagic(std::string_view l) {
    return l.size() >= 2 && l[0] == '.' && l[1] == '.' && !(l.size() >= 3 && l[2] == '@');
}

// True if the local label base should be updated as a result of this symbol: local labels and any kind of special label,
// including the ..@ ones, are excluded.
bool LabelTable::set_prevlabel(std::string_view l) const {
    if (as_.tasm_compatible_mode) {
        if (l.size() >= 2 && l[0] == '@' && l[1] == '@') return false;
    }
    return l.empty() || l[0] != '.';
}

void LabelTable::init() {
    labels_.clear();
    index_.clear();
    prevlabel_.clear();
    initialized_ = true;
}

void LabelTable::cleanup() {
    initialized_ = false;
    labels_.clear();
    index_.clear();
    prevlabel_.clear();
}

// Emit a symdef to the output and the debug format backends.
void LabelTable::out_symdef(Label& lptr) {
    // Backend-defined special segments are passed to symdef immediately.
    if (as_.pass_final()) {
        // Emit special fixups for globals and commons.
        switch (lptr.type) {
        case LBL_GLOBAL:
        case LBL_REQUIRED:
        case LBL_COMMON:
            if (!lptr.special.empty() && hooks.symdef) hooks.symdef(lptr.mangled, 0, 0, 3, lptr.special);
            break;
        default:
            break;
        }
        return;
    }

    if (as_.pass_type() != PASS_STAB && lptr.type != LBL_BACKEND) return;

    int backend_type;
    std::int64_t backend_offset;
    switch (lptr.type) {
    case LBL_EXTERN:
        // If not seen in the previous or this pass, drop it.
        if (lptr.lastref < as_.pass_count()) return;
        // Otherwise, promote to LBL_REQUIRED at this time.
        lptr.type = LBL_REQUIRED;
        [[fallthrough]];
    case LBL_GLOBAL:
    case LBL_REQUIRED:
        backend_type = 1;
        backend_offset = lptr.offset;
        break;
    case LBL_COMMON:
        backend_type = 2;
        backend_offset = lptr.size;
        break;
    default:
        backend_type = 0;
        backend_offset = lptr.offset;
        break;
    }

    // Might be necessary for a backend symbol.
    mangle_label_name(lptr);

    if (hooks.symdef) hooks.symdef(lptr.mangled, lptr.segment, backend_offset, backend_type, lptr.special);

    // NASM special symbols are not passed to the debug format; none of the current backends want to see them.
    if (lptr.type == LBL_SPECIAL || lptr.type == LBL_BACKEND) return;

    if (hooks.debug_deflabel) hooks.debug_deflabel(lptr.mangled, lptr.segment, lptr.offset, backend_type, lptr.special);
}

// Finds the label of the given name. Creates a new one if it isn't found and `create` is true.
LabelTable::Label* LabelTable::find_label(std::string_view label, bool create, bool* created) {
    if (!initialized_) init();

    std::string full;
    if (islocal(label)) {
        full = prevlabel_;
        full.append(label);
        label = full;
    }

    // MASM matches identifiers case-insensitively, so hash on a folded lower-case copy (`CurTDB' and `curTDB' are the same
    // symbol). But ML preserves the *defining* spelling in the object's symbol table, so the label is kept as written and
    // only the key is folded. (NASM's own special labels -- ..@, ..start, section symbols -- are already lower case, so
    // folding is a no-op.)
    std::string key(label);
    if (as_.masm_mode) {
        for (char& c : key) c = nasm_tolower(c);
    }

    const auto it = index_.find(key);
    if (it != index_.end()) {
        if (created) *created = false;
        return &labels_[it->second];
    }
    if (!create) {
        if (created) *created = false;
        return nullptr;
    }

    // Create a new label.
    if (created) *created = true;
    Label& l = labels_.emplace_back();
    l.label = std::string(label);   // original case, as written
    l.subsection = NO_SEG;
    index_.emplace(std::move(key), labels_.size() - 1);
    return &l;
}

LabelType LabelTable::lookup_label(std::string_view label, std::int32_t& segment, std::int64_t& offset) {
    if (!initialized_) return LBL_none;

    Label* lptr = find_label(label, false, nullptr);
    if (lptr && lptr->defined) {
        const std::int64_t lpass = as_.pass_count() + 1;
        lptr->lastref = lpass;
        segment = lptr->segment;
        offset = lptr->offset;
        return lptr->type;
    }
    return LBL_none;
}

void LabelTable::set_label_mangle(LabelMangle which, std::string_view what) {
    const auto i = static_cast<std::size_t>(which);
    if (mangle_set_[i]) return;   // once set, do not change
    mangle_[i] = std::string(what);
    mangle_set_[i] = true;
}

// Format a label name with the appropriate prefixes and suffixes.
const std::string& LabelTable::mangle_label_name(Label& lptr) {
    if (!lptr.mangled.empty() && lptr.mangled_type == lptr.type) return lptr.mangled;   // already mangled

    const std::string* prefix;
    const std::string* suffix;
    static const std::string kNone;
    switch (lptr.type) {
    case LBL_GLOBAL:
    case LBL_STATIC:
    case LBL_EXTERN:
    case LBL_REQUIRED:
        prefix = &mangle_[static_cast<int>(LabelMangle::GlobalPrefix)];
        suffix = &mangle_[static_cast<int>(LabelMangle::GlobalSuffix)];
        break;
    case LBL_BACKEND:
    case LBL_SPECIAL:
        prefix = suffix = &kNone;
        break;
    default:
        prefix = &mangle_[static_cast<int>(LabelMangle::LocalPrefix)];
        suffix = &mangle_[static_cast<int>(LabelMangle::LocalSuffix)];
        break;
    }

    lptr.mangled_type = lptr.type;
    lptr.mangled = *prefix + lptr.label + *suffix;
    return lptr.mangled;
}

void LabelTable::handle_herelabel(Label& lptr, std::int32_t& segment, std::int64_t& offset) {
    if (!hooks.herelabel) return;
    if (as_.location.segment == NO_SEG) return;

    const std::int32_t oldseg = segment;
    if (oldseg == as_.location.segment && offset == as_.location.offset) {
        // This label is defined at this location.
        bool copyoffset = false;
        const std::int32_t newseg = hooks.herelabel(lptr.mangled, lptr.type, oldseg, lptr.subsection, copyoffset);
        if (newseg == oldseg) return;

        segment = newseg;
        if (copyoffset) {
            // Maintain the offset from the old to the new segment.
            as_.switch_segment(newseg);
            as_.location.offset = offset;
        } else {
            // Keep a separate offset for the new segment.
            offset = as_.switch_segment(newseg);
        }
    }
}

bool LabelTable::declare_label_lptr(Label& lptr, LabelType type, std::string_view special) {
    LabelType oldtype = lptr.type;

    if (!as_.pass_stable() && oldtype == LBL_LOCAL) {
        if (is_extern(type) && lptr.defined) oldtype = LBL_GLOBAL;   // already defined, promote to global
        else oldtype = type;
        lptr.type = oldtype;
    }

    if (oldtype == type || (oldtype == LBL_EXTERN && type == LBL_REQUIRED)) {
        lptr.type = type;

        if (!special.empty()) {
            if (lptr.special.empty()) {
                lptr.special = std::string(special);
            } else {
                // nasm_stricmp: the same text, whatever its case.
                bool same = lptr.special.size() == special.size();
                for (std::size_t i = 0; same && i < special.size(); ++i) same = nasm_tolower(lptr.special[i]) == nasm_tolower(special[i]);
                if (!same) as_.nonfatal("symbol `{}' has inconsistent attributes `{}' and `{}'", lptr.label, lptr.special, special);
            }
        }
        return true;
    }
    if (is_extern(oldtype) && labels_is_global(type)) {
        // EXTERN or REQUIRED can be replaced with GLOBAL or COMMON.
        lptr.type = type;
        lptr.defined = 0;

        // Override special unconditionally.
        if (!special.empty()) lptr.special = std::string(special);
        return true;
    }
    if (is_extern(type) && (labels_is_global(oldtype) || is_extern(oldtype))) {
        // GLOBAL or COMMON ignore subsequent EXTERN or REQUIRED; REQUIRED ignores subsequent EXTERN. Ignore special unless we
        // don't already have one.
        if (lptr.special.empty()) lptr.special = std::string(special);
        return false;   // do not call define_label() after this!
    }

    as_.nonfatal("symbol `{}' declared both as {} and {}", lptr.label, labels_type_name(lptr.type), labels_type_name(type));
    return false;
}

bool LabelTable::declare_label(std::string_view label, LabelType type, std::string_view special) {
    Label* lptr = find_label(label, true, nullptr);
    return declare_label_lptr(*lptr, type, special);
}

void LabelTable::define_label(std::string_view label, std::int32_t segment, std::int64_t offset, bool normal) {
    // The backend may invoke this during initialization, at which pass_count() is zero, so add one so we never have a zero
    // value for a defined variable.
    const std::int64_t lpass = as_.pass_count() + 1;

    // Phase errors here can be one of two types: a new label appears, or the offset changes. Increment global_offset_changed
    // when that happens, to tell the assembler core to make another pass.
    bool created = false;
    Label* lptr = find_label(label, true, &created);

    std::int64_t lastdef = lptr->defined;

    if (segment) {
        // We are actually defining this label.
        if (is_extern(lptr->type)) {
            // Auto-promote EXTERN/REQUIRED to GLOBAL.
            lptr->type = LBL_GLOBAL;
            lastdef = 0;   // we are "re-creating" this label
        }
    } else {
        // It's a pseudo-segment (extern, required, common).
        segment = lptr->segment ? lptr->segment : (hooks.seg_alloc ? hooks.seg_alloc() : 0);
    }

    if (lastdef || lptr->type == LBL_BACKEND) {
        // We have seen this on at least one previous pass, or potentially earlier in this same pass (in which case we will
        // probably error out further down).
        mangle_label_name(*lptr);
        handle_herelabel(*lptr, segment, offset);
    }

    if (ismagic(label) && lptr->type == LBL_LOCAL) lptr->type = LBL_SPECIAL;

    if (set_prevlabel(label) && normal) prevlabel_ = lptr->label;

    std::int64_t size;
    if (lptr->type == LBL_COMMON) {
        size = offset;
        offset = 0;
    } else {
        size = 0;   // this is a hack...
    }

    // A "large change" is one which is not the offset.
    const bool largechange = created || !lastdef || lptr->segment != segment || lptr->size != size;
    const bool changed = largechange || (lptr->offset != offset);
    as_.global_offset_changed += changed;

    if (lastdef == lpass) {
        // Defined elsewhere in the program, seen in this pass.
        ErrFlags noteflags;
        if (changed) {
            as_.nonfatal("label `{}' inconsistently redefined", lptr->label);
            noteflags = ERR_NONFATAL | ERR_HERE | ERR_NO_SEVERITY;
        } else {
            as_.warn(WARN_LABEL_REDEF, "info: label `{}' redefined to an identical value", lptr->label);
            noteflags = ERR_WARNING | ERR_HERE | ERR_NO_SEVERITY | WARN_LABEL_REDEF;
        }

        const SrcLocation saved = as_.src.update(lptr->def);
        as_.error(noteflags, "info: label `{}' originally defined", lptr->label);
        as_.src.update(saved);
    } else if (changed && as_.pass_final() && lptr->type != LBL_SPECIAL) {
        // As a special case, LBL_SPECIAL symbols are allowed to be changed even during the last pass.
        if (!largechange) {
            as_.warn(WARN_LABEL_REDEF_LATE, "label `{}' changed during code generation (offset 0x{:x} -> 0x{:x})", lptr->label,
                     static_cast<std::uint64_t>(lptr->offset), static_cast<std::uint64_t>(offset));
        } else {
            as_.warn(WARN_LABEL_REDEF_LATE | ERR_UNDEAD, "label `{}' {} during code generation", lptr->label, created ? "defined" : "changed");
        }
    }
    lptr->segment = segment;
    lptr->offset = offset;
    lptr->size = size;
    lptr->defined = lpass;

    if (changed || lastdef != lpass) lptr->def = as_.src.where();

    if (lastdef != lpass) out_symdef(*lptr);
}

void LabelTable::backend_label(std::string_view label, std::int32_t segment, std::int64_t offset) {
    if (!declare_label(label, LBL_BACKEND, {})) return;
    define_label(label, segment, offset, false);
}

std::string LabelTable::local_scope(std::string_view label) const { return islocal(label) ? prevlabel_ : std::string(); }

}  // namespace cb::build::x86asm
