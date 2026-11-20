// Tests of the label table (labels.hpp): local labels, `..@` and special names, declaration rules (global, extern, required,
// common), redefinition messages, the count of labels that moved between passes, the "changed during code generation"
// warning, name decoration, and --masm's case folding. The messages are nasm's.
#include "build/x86asm/labels.hpp"

#include <string>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "core/testing.hpp"

namespace {
using namespace cb::build::x86asm;

// One `symdef` call the table made.
struct LbDef {
    std::string name;
    std::int32_t segment;
    std::int64_t offset;
    int is_global;
    std::string special;
    friend bool operator==(const LbDef&, const LbDef&) = default;
};

struct LbFix {
    Assembly as;
    LabelTable labels{as};
    std::vector<LbDef> defs;
    std::vector<LbDef> debug_defs;
    std::int32_t next_seg = 100;

    LbFix() {
        as.src.set(1, "t.asm");
        labels.init();
        labels.hooks.seg_alloc = [this] { const std::int32_t s = next_seg; next_seg += 2; return s; };
        labels.hooks.symdef = [this](std::string_view n, std::int32_t seg, std::int64_t off, int g, std::string_view sp) {
            defs.push_back({std::string(n), seg, off, g, std::string(sp)});
        };
        labels.hooks.debug_deflabel = [this](std::string_view n, std::int32_t seg, std::int64_t off, int g, std::string_view sp) {
            debug_defs.push_back({std::string(n), seg, off, g, std::string(sp)});
        };
    }

    // Move on to the `n`th pass kind: 1 first, 2 optimization, 3 stabilization, 4 final.
    void pass(int n) {
        while (static_cast<int>(as.pass_type()) < static_cast<int>(PASS_FIRST) + n - 1 || as.pass_type() == PASS_INIT) {
            as.error_pass_end();
            as.begin_pass();
        }
    }

    LabelType look(std::string_view name, std::int32_t* seg = nullptr, std::int64_t* off = nullptr) {
        std::int32_t s = -77;
        std::int64_t o = -77;
        const LabelType t = labels.lookup_label(name, s, o);
        if (seg) *seg = s;
        if (off) *off = o;
        return t;
    }

    std::string last() const { return as.diagnostics().empty() ? std::string() : as.diagnostics().back().text; }
};
}  // namespace

CB_TEST("x86asm.labels.define_and_lookup", [] {
    LbFix f;
    std::int32_t seg = 0;
    std::int64_t off = 0;
    CB_CHECK_EQ(static_cast<int>(f.look("foo", &seg, &off)), static_cast<int>(LBL_none));
    CB_CHECK_EQ(seg, -77);   // untouched when there is no label
    f.labels.define_label("foo", 2, 0x20, true);
    CB_CHECK_EQ(static_cast<int>(f.look("foo", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(seg, 2);
    CB_CHECK_EQ(off, std::int64_t{0x20});
    CB_CHECK_EQ(f.labels.size(), std::size_t{1});
    // Declared but not defined: not found.
    CB_CHECK(f.labels.declare_label("bar", LBL_GLOBAL));
    CB_CHECK_EQ(static_cast<int>(f.look("bar")), static_cast<int>(LBL_none));
    f.labels.define_label("bar", 2, 4, true);
    CB_CHECK_EQ(static_cast<int>(f.look("bar")), static_cast<int>(LBL_GLOBAL));
    // Names are case-sensitive in NASM mode.
    CB_CHECK_EQ(static_cast<int>(f.look("FOO")), static_cast<int>(LBL_none));
    // Nothing is found before init or after cleanup.
    f.labels.cleanup();
    CB_CHECK_EQ(static_cast<int>(f.look("foo")), static_cast<int>(LBL_none));
    f.labels.init();
    CB_CHECK_EQ(f.labels.size(), std::size_t{0});
});

CB_TEST("x86asm.labels.local_labels", [] {
    LbFix f;
    CB_CHECK_EQ(f.labels.local_scope(".x"), "");
    f.labels.define_label("main", 2, 0x40, true);
    CB_CHECK_EQ(f.labels.prev_label(), "main");
    f.labels.define_label(".loop", 2, 0x48, true);
    CB_CHECK_EQ(f.labels.prev_label(), "main");   // a local label does not change the base
    f.labels.define_label("next", 2, 0x50, true);
    f.labels.define_label(".loop", 2, 0x58, true);
    CB_CHECK_EQ(f.labels.local_scope(".loop"), "next");
    CB_CHECK_EQ(f.labels.local_scope("loop"), "");
    CB_CHECK_EQ(f.labels.local_scope("..@4"), "");    // two periods: not local
    CB_CHECK_EQ(f.labels.local_scope("..start"), "");
    std::int64_t off = 0;
    std::int32_t seg = 0;
    CB_CHECK_EQ(static_cast<int>(f.look(".loop", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(off, std::int64_t{0x58});
    CB_CHECK_EQ(static_cast<int>(f.look("main.loop", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(off, std::int64_t{0x48});
    CB_CHECK_EQ(static_cast<int>(f.look("next.loop", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(off, std::int64_t{0x58});
    // A label the backend makes (normal = false) does not change the base either; neither do ..@ and special names.
    f.labels.define_label("hidden", 2, 0x60, false);
    CB_CHECK_EQ(f.labels.prev_label(), "next");
    f.labels.define_label("..@7", 2, 0x60, true);
    CB_CHECK_EQ(f.labels.prev_label(), "next");
});

CB_TEST("x86asm.labels.special_names", [] {
    LbFix f;
    f.labels.define_label("main", 2, 0, true);
    f.labels.define_label("..start", 2, 4, true);
    CB_CHECK_EQ(f.labels.prev_label(), "main");   // the ..names do not become the base
    CB_CHECK_EQ(static_cast<int>(f.look("..start")), static_cast<int>(LBL_SPECIAL));
    // ..@ names are ordinary labels (not special, and not local to anything), and do not set the base either.
    f.labels.define_label("..@1.x", 2, 8, true);
    CB_CHECK_EQ(static_cast<int>(f.look("..@1.x")), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(f.labels.prev_label(), "main");
});

CB_TEST("x86asm.labels.tasm_local_labels", [] {
    LbFix f;
    f.as.tasm_compatible_mode = true;
    f.labels.define_label("main", 2, 0x40, true);
    f.labels.define_label("@@a", 2, 0x44, true);
    CB_CHECK_EQ(f.labels.prev_label(), "main");
    CB_CHECK_EQ(f.labels.local_scope("@@a"), "main");
    std::int64_t off = 0;
    std::int32_t seg = 0;
    CB_CHECK_EQ(static_cast<int>(f.look("main@@a", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(off, std::int64_t{0x44});
});

CB_TEST("x86asm.labels.declare_rules", [] {
    LbFix f;
    // nasm: extern a / global a / a: dd 0   and   global a / extern a   are fine.
    CB_CHECK(f.labels.declare_label("a", LBL_EXTERN));
    CB_CHECK(f.labels.declare_label("a", LBL_GLOBAL));   // extern is replaced by global
    f.labels.define_label("a", 2, 0, true);
    CB_CHECK_EQ(static_cast<int>(f.look("a")), static_cast<int>(LBL_GLOBAL));
    CB_CHECK(!f.labels.declare_label("a", LBL_EXTERN));   // a later extern is ignored: do not define it again
    CB_CHECK(f.labels.declare_label("b", LBL_GLOBAL));
    CB_CHECK(!f.labels.declare_label("b", LBL_EXTERN));
    CB_CHECK(f.labels.declare_label("b", LBL_GLOBAL));
    CB_CHECK(f.as.diagnostics().empty());
    // required is a kind of extern; extern is promoted to it, not the other way.
    CB_CHECK(f.labels.declare_label("r", LBL_EXTERN));
    CB_CHECK(f.labels.declare_label("r", LBL_REQUIRED));
    CB_CHECK(!f.labels.declare_label("r", LBL_EXTERN));   // "required ignores subsequent extern"
    // nasm: global x / common x 4 -> "symbol `x' declared both as global and common"
    CB_CHECK(f.labels.declare_label("x", LBL_GLOBAL));
    CB_CHECK(!f.labels.declare_label("x", LBL_COMMON));
    CB_CHECK_EQ(f.last(), "t.asm:1: error: symbol `x' declared both as global and common");
    // nasm: common z 4 / global z -> "symbol `z' declared both as common and global"
    f.as.clear_diagnostics();
    CB_CHECK(f.labels.declare_label("z", LBL_COMMON));
    CB_CHECK(!f.labels.declare_label("z", LBL_GLOBAL));
    CB_CHECK_EQ(f.last(), "t.asm:1: error: symbol `z' declared both as common and global");
    // A label already defined as local (static) in the first pass can be made global; later passes are stricter.
    f.as.clear_diagnostics();
    f.labels.define_label("w", 2, 0, true);
    CB_CHECK(f.labels.declare_label("w", LBL_GLOBAL));   // pass not stable: the type is taken
    CB_CHECK_EQ(static_cast<int>(f.look("w")), static_cast<int>(LBL_GLOBAL));
    f.pass(3);
    f.labels.define_label("v", 2, 0, true);
    CB_CHECK(!f.labels.declare_label("v", LBL_GLOBAL));   // a defined local, in a stable pass: an error
    CB_CHECK_EQ(f.last(), "t.asm:1: error: symbol `v' declared both as local and global");
});

CB_TEST("x86asm.labels.special_attributes", [] {
    LbFix f;
    // nasm (-f obj): global x:function / global x:data -> "symbol `x' has inconsistent attributes `function' and `data'"
    CB_CHECK(f.labels.declare_label("x", LBL_GLOBAL, "function"));
    CB_CHECK(f.labels.declare_label("x", LBL_GLOBAL, "FUNCTION"));   // the same, whatever the case
    CB_CHECK(f.as.diagnostics().empty());
    CB_CHECK(f.labels.declare_label("x", LBL_GLOBAL, "data"));
    CB_CHECK_EQ(f.last(), "t.asm:1: error: symbol `x' has inconsistent attributes `function' and `data'");
    // An extern given a global's attribute: kept unless there is none.
    CB_CHECK(f.labels.declare_label("y", LBL_GLOBAL));
    CB_CHECK(!f.labels.declare_label("y", LBL_EXTERN, "data"));
    // Replacing an extern by a global overrides the attribute.
    CB_CHECK(f.labels.declare_label("e", LBL_EXTERN, "a1"));
    CB_CHECK(f.labels.declare_label("e", LBL_GLOBAL, "a2"));
});

CB_TEST("x86asm.labels.redefinition_messages", [] {
    LbFix f;
    // nasm: x equ 5 / x equ 5 / x equ 6 -> only the third is an error, with a note where it was defined.
    f.as.src.set(1, "t.asm");
    f.labels.define_label("x", SEG_ABS, 5, true);
    f.as.src.set(2, "t.asm");
    f.labels.define_label("x", SEG_ABS, 5, true);
    CB_CHECK(f.as.diagnostics().empty());   // identical: a warning that is off by default
    f.as.src.set(3, "t.asm");
    f.labels.define_label("x", SEG_ABS, 6, true);
    CB_CHECK_EQ(f.as.diagnostics().size(), std::size_t{2});
    CB_CHECK_EQ(f.as.diagnostics()[0].text, "t.asm:3: error: label `x' inconsistently redefined");
    CB_CHECK_EQ(f.as.diagnostics()[1].text, "t.asm:1: info: label `x' originally defined here");
    CB_CHECK_EQ(f.as.src.get_linnum(), 3);   // the position is put back
    // The value it had in the first place is kept as the label's definition place; the failed one changes it.
    // With the warning on, an identical redefinition is mentioned.
    LbFix g;
    CB_CHECK(g.as.set_warning_status("+label-redef"));
    g.as.src.set(1, "t.asm");
    g.labels.define_label("y", 2, 4, true);
    g.as.src.set(2, "t.asm");
    g.labels.define_label("y", 2, 4, true);
    CB_CHECK_EQ(g.as.diagnostics().size(), std::size_t{2});
    CB_CHECK_EQ(g.as.diagnostics()[0].text, "t.asm:2: warning: info: label `y' redefined to an identical value [-w+label-redef]");
});

CB_TEST("x86asm.labels.offset_change_counting", [] {
    LbFix f;
    f.pass(1);
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{0});
    f.labels.define_label("a", 2, 0x10, true);   // a new label counts as a change
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{1});
    f.labels.define_label("b", 2, 0x14, true);
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{2});
    f.pass(2);   // a new pass resets the count
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{0});
    f.labels.define_label("a", 2, 0x10, true);   // seen before, same place: no change
    f.labels.define_label("b", 2, 0x14, true);
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{0});
    CB_CHECK(f.as.diagnostics().empty());
    f.labels.define_label("a", 2, 0x12, true);   // moved
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{1});
    f.labels.define_label("b", 4, 0x14, true);   // another segment
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{2});
    f.labels.define_label("c", 2, 0x20, true);   // a new one
    CB_CHECK_EQ(f.as.global_offset_changed, std::uint64_t{3});
});

CB_TEST("x86asm.labels.changed_during_code_generation", [] {
    // A label that moves after the last optimization pass is an error by default (label-redef-late is an error class), with
    // the old and new offsets when only the offset changed. Each case is its own run: after the first error the rest are
    // not reported (they are "undead" messages).
    const auto run = [](const char* name, std::int32_t seg, std::int64_t off) {
        LbFix f;
        f.pass(1);
        f.labels.define_label("moves", 2, 0x10, true);
        f.labels.define_label("..start", 2, 0x10, true);
        f.pass(4);   // the code generation pass
        f.as.src.set(7, "t.asm");
        f.labels.define_label(name, seg, off, true);
        return f.as.diagnostics().empty() ? std::string("<none>") : f.last();
    };
    CB_CHECK_EQ(run("moves", 2, 0x10), "<none>");   // as it was: nothing
    CB_CHECK_EQ(run("moves", 2, 0x1c), "t.asm:7: error: label `moves' changed during code generation (offset 0x10 -> 0x1c) [-w+error=label-redef-late]");
    CB_CHECK_EQ(run("moves", 4, 0x10), "t.asm:7: error: label `moves' changed during code generation [-w+error=label-redef-late]");   // another segment
    CB_CHECK_EQ(run("..start", 2, 0x99), "<none>");   // NASM's special symbols may move even now
    CB_CHECK_EQ(run("new", 2, 0), "t.asm:7: error: label `new' defined during code generation [-w+error=label-redef-late]");   // first seen in the last pass

    // With the class turned into a warning, it is one.
    LbFix f;
    CB_CHECK(f.as.set_warning_status("-error=label-redef-late"));
    f.pass(1);
    f.labels.define_label("moves", 2, 0x10, true);
    f.pass(4);
    f.as.src.set(7, "t.asm");
    f.labels.define_label("moves", 2, 0x1c, true);
    CB_CHECK_EQ(f.last(), "t.asm:7: warning: label `moves' changed during code generation (offset 0x10 -> 0x1c) [-w+label-redef-late]");
    // ... and with it off, nothing.
    LbFix g;
    CB_CHECK(g.as.set_warning_status("-label-redef-late"));
    g.pass(1);
    g.labels.define_label("moves", 2, 0x10, true);
    g.pass(4);
    g.labels.define_label("moves", 2, 0x1c, true);
    CB_CHECK(g.as.diagnostics().empty());
});

CB_TEST("x86asm.labels.symdef_calls", [] {
    LbFix f;
    f.labels.set_label_mangle(LabelMangle::GlobalPrefix, "_");
    f.labels.set_label_mangle(LabelMangle::GlobalSuffix, "@0");
    f.labels.set_label_mangle(LabelMangle::GlobalPrefix, "ignored");   // once set, it does not change
    f.pass(1);
    f.labels.declare_label("g", LBL_GLOBAL, "export");   // declarations are made in every pass
    f.pass(3);   // the symbols are announced in the stabilization pass
    f.labels.declare_label("g", LBL_GLOBAL, "export");
    f.labels.define_label("g", 2, 0x30, true);
    f.labels.define_label("loc", 2, 0x34, true);
    f.labels.define_label("loc", 2, 0x34, true);   // defined again in the same pass: announced once only
    CB_CHECK(f.as.diagnostics().empty());
    CB_CHECK_EQ(f.defs.size(), std::size_t{2});
    CB_CHECK(f.defs[0] == (LbDef{"_g@0", 2, 0x30, 1, "export"}));
    CB_CHECK(f.defs[1] == (LbDef{"loc", 2, 0x34, 0, ""}));   // a local gets the local decoration (none)
    CB_CHECK_EQ(f.debug_defs.size(), std::size_t{2});
    CB_CHECK(f.debug_defs[0] == f.defs[0]);

    // Local decoration, and the common symbol (the offset is the size, the segment a pseudo-segment of its own).
    LbFix g;
    g.labels.set_label_mangle(LabelMangle::LocalPrefix, "L");
    g.labels.set_label_mangle(LabelMangle::LocalSuffix, "$");
    g.pass(1);
    g.labels.declare_label("c", LBL_COMMON);
    g.pass(3);
    g.labels.declare_label("c", LBL_COMMON);
    g.labels.define_label("c", 0, 16, true);
    g.labels.define_label("l", 2, 1, true);
    CB_CHECK_EQ(g.defs.size(), std::size_t{2});
    CB_CHECK(g.defs[0] == (LbDef{"Lc$", 100, 16, 2, ""}));
    CB_CHECK(g.defs[1] == (LbDef{"Ll$", 2, 1, 0, ""}));
    // Other passes announce nothing (but backend symbols).
    LbFix h;
    h.pass(2);
    h.labels.define_label("p", 2, 1, true);
    CB_CHECK(h.defs.empty());
    h.labels.backend_label("..got", 8, 0);
    CB_CHECK_EQ(h.defs.size(), std::size_t{1});
    CB_CHECK_EQ(h.defs[0].name, "..got");
    CB_CHECK(h.debug_defs.empty());   // special symbols are not passed to the debug format
});

CB_TEST("x86asm.labels.extern_handling", [] {
    LbFix f;
    // Externs are declared in every pass; each gets a pseudo-segment of its own when it is "defined" with segment 0.
    f.pass(1);
    CB_CHECK(f.labels.declare_label("unused", LBL_EXTERN));
    CB_CHECK(f.labels.declare_label("used", LBL_EXTERN));
    CB_CHECK(f.labels.declare_label("late", LBL_EXTERN));
    f.pass(3);
    CB_CHECK(f.labels.declare_label("unused", LBL_EXTERN));
    f.labels.define_label("unused", 0, 0, true);
    CB_CHECK(f.labels.declare_label("used", LBL_EXTERN));
    f.labels.define_label("used", 0, 0, true);
    // An extern no expression has mentioned is not announced to the output format.
    CB_CHECK(f.defs.empty());
    std::int32_t seg = 0;
    std::int64_t off = -1;
    CB_CHECK_EQ(static_cast<int>(f.look("used", &seg, &off)), static_cast<int>(LBL_EXTERN));
    CB_CHECK_EQ(seg, 102);   // its pseudo-segment: the second one allocated
    CB_CHECK_EQ(off, std::int64_t{0});
    // Defining an extern for real makes it global.
    CB_CHECK(f.labels.declare_label("late", LBL_EXTERN));
    f.labels.define_label("late", 0, 0, true);
    f.labels.define_label("late", 2, 0x40, true);
    CB_CHECK_EQ(static_cast<int>(f.look("late")), static_cast<int>(LBL_GLOBAL));
});

CB_TEST("x86asm.labels.here_labels", [] {
    LbFix f;
    // An output format may move a label defined at the current location into another section (herelabel). The hook is
    // consulted from the second pass on (a label seen before).
    f.labels.hooks.herelabel = [](std::string_view name, LabelType, std::int32_t seg, std::int32_t& subsection, bool& copyoffset) {
        subsection = 9;
        copyoffset = name == "copy";
        return seg + 2;
    };
    f.as.location.segment = 2;
    f.as.location.offset = 0x10;
    f.pass(1);
    f.as.location.segment = 2;
    f.as.location.offset = 0x10;
    f.labels.define_label("here", 2, 0x10, true);   // first pass: not consulted
    CB_CHECK_EQ(static_cast<int>(f.look("here")), static_cast<int>(LBL_LOCAL));
    std::int32_t seg = 0;
    std::int64_t off = 0;
    f.look("here", &seg, &off);
    CB_CHECK_EQ(seg, 2);
    f.pass(2);
    f.as.location.segment = 2;
    f.as.location.offset = 0x10;
    f.labels.define_label("here", 2, 0x10, true);    // pass 2, at the location: moved to segment 4, at that segment's own offset
    f.look("here", &seg, &off);
    CB_CHECK_EQ(seg, 4);
    CB_CHECK_EQ(off, std::int64_t{0});
    CB_CHECK_EQ(f.as.location.segment, 4);   // and the assembly is now in it
    // Not at the location: not consulted.
    f.as.location.segment = 2;
    f.as.location.offset = 0x10;
    f.labels.define_label("here", 2, 0x10, true);
    f.labels.define_label("elsewhere", 2, 0x50, true);
    f.pass(3);
    f.as.location.segment = 2;
    f.as.location.offset = 0x10;
    f.labels.define_label("elsewhere", 2, 0x50, true);
    f.look("elsewhere", &seg, &off);
    CB_CHECK_EQ(seg, 2);
});

CB_TEST("x86asm.labels.masm_case_folding", [] {
    LbFix f;
    f.as.masm_mode = true;
    f.labels.define_label("CurTDB", 2, 0x30, true);
    std::int64_t off = 0;
    std::int32_t seg = 0;
    CB_CHECK_EQ(static_cast<int>(f.look("curtdb", &seg, &off)), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(off, std::int64_t{0x30});
    CB_CHECK_EQ(static_cast<int>(f.look("CURTDB")), static_cast<int>(LBL_LOCAL));
    CB_CHECK_EQ(f.labels.size(), std::size_t{1});
    // The spelling of the definition is kept in the symbol table (what the output format sees).
    f.pass(1);
    f.labels.declare_label("MixedCase", LBL_GLOBAL);
    f.pass(3);
    f.labels.declare_label("mixedcase", LBL_GLOBAL);   // the same symbol
    f.labels.define_label("MixedCase", 2, 1, true);
    f.labels.define_label("mixedcase", 2, 1, true);   // the same symbol, in this pass: identical
    CB_CHECK_EQ(f.defs.size(), std::size_t{1});
    CB_CHECK_EQ(f.defs[0].name, "MixedCase");
    // Local labels fold too.
    f.labels.define_label("Main", 2, 0, true);
    f.labels.define_label(".Loop", 2, 4, true);
    CB_CHECK_EQ(static_cast<int>(f.look("main.loop")), static_cast<int>(LBL_LOCAL));
    // In NASM mode two spellings are two labels.
    LbFix n;
    n.labels.define_label("a", 2, 0, true);
    n.labels.define_label("A", 2, 4, true);
    CB_CHECK_EQ(n.labels.size(), std::size_t{2});
    CB_CHECK(n.as.diagnostics().empty());
});
