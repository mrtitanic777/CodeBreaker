// codebreaker/java/rules.hpp -- typed C++ port of codebreaker.java.rules.
//
// rules.py -- structural crash-class detectors for converted C++.
//
// These encode the bug classes that crash when porting Java to C++ across any
// target platform. Each rule is platform-aware: messages, severity, and
// applicability adapt to the selected target (Xbox 360, PC Windows, PS3, etc.)
// using metadata from platforms.py.
//
// Each rule is a small, named, self-describing object so it's individually
// testable and the report can explain *why* it fired. A rule looks at a converted
// file (and, when needed, its sibling header) and yields Events.
//
// Covered here (the cases plain text-marker / lint scanning misses):
//   uninit-array        new T[n] without () -- Java zero-fills, C++ does not
//   ctor-stub           empty / delegation-only constructor body
//   rule-of-three       owns heap (new in ctor) but missing dtor/copy control
//   value-copy-owner    a heavy/owning type passed or returned BY VALUE
//   virtual-call-in-ctor  ctor calls own virtual -> dispatches to base, not override
//   static-init-order   eager static init with cross-TU dependency
//   endianness          raw multi-byte cast in binary-IO file (big-endian only)
//   null-deref-getter   getter()->member where getter can return NULL
//   null-deref-field    known-nullable field deref without null check
//
// Notes on the port:
//   * A rule is a generator of 6-tuples in Python; here it returns the whole py::list of them (Finding):
//     (line, category, severity, rule id, message, snippet).
//   * codebreaker.java.profiles has no port. The Python reads profiles.active().heavy_types / heavy_types_high
//     once at import and again whenever the active profile changes (_reseed, registered with on_change). Here
//     _HEAVY_TYPES / _HEAVY_HIGH start empty (the `generic` profile's) and _reseed(heavy_types, heavy_types_high)
//     takes the two tuples a profile holds; whoever switches profiles calls it.
//   * Python's set iteration order (hash order, randomised for str) decides the order of findings only where
//     one line / constructor matches several names of a set (virtual-call-in-ctor, map-read-insert,
//     leak-drop-no-delete); here the order is the set's insertion order.
//   * _PlatCtx.override_pairs is None or a frozenset of (class, method): std::optional<py::set<...>>.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/java/ledger.hpp"

namespace codebreaker::java::rules {

// ---------------------------------------------------------------------------
// Platform context: built once per scan_rules() call, passed to every rule.
// ---------------------------------------------------------------------------
class _PlatCtx {
public:
    py::str label;
    bool big_endian = false;
    bool null_page_protected = true;
    py::str null_deref_msg;
    // {(base_class, method)} that a DESCENDANT overrides -- built once by scan_rules() over the whole
    // tree so rule_virtual_call_in_ctor can flag ONLY genuinely-overridden calls. None => whole-tree
    // hierarchy is unknown (a per-file/unit-test call): fall back to the conservative "always flag".
    std::optional<py::set<std::tuple<py::str, py::str>>> override_pairs;

    explicit _PlatCtx(const std::optional<py::str>& platform_id);
};

// (line, category, severity, rule, message, snippet)
using Finding = std::tuple<int64_t, py::str, py::str, py::str, py::str, py::str>;

// Primitive element types whose `new T[n]` is left uninitialized in C++.
extern const py::str _PRIM;
// `new <prim>[ ... ]`  (capture whether a trailing value-init () is present)
extern const py::re::Pattern _NEW_PRIM_ARRAY;
// `new <Type> * [ ... ]`  -> array of pointers, uninitialized = garbage vtables
extern const py::re::Pattern _NEW_PTR_ARRAY;

// A type that is expensive or owns a resource and must never be copied by value.
extern py::list<py::str> _HEAVY_TYPES;
extern py::list<py::str> _HEAVY_HIGH;

// Re-derive the heavy-types tuples when the active target profile changes (profiles.on_change).
void _reseed(const py::list<py::str>& heavy_types, const py::list<py::str>& heavy_types_high);

py::str _read(const py::str& path);
py::str _strip_block_comments(const py::str& text);

// Yield (lineno, raw, code-without-line-comment) skipping pure comment lines.
py::list<std::tuple<int64_t, py::str, py::str>> _iter_code_lines(const py::str& text);

using RuleFn = py::list<Finding> (*)(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text,
                                     const _PlatCtx& plat);

py::list<Finding> rule_uninit_array(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_ctor_stub(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_rule_of_three(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_value_copy_owner(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

// Yield (line, body) for each ClassName::ClassName(...) definition.
py::list<std::tuple<int64_t, py::str>> _ctor_bodies(const py::str& text, const py::str& cls);

py::list<Finding> rule_virtual_call_in_ctor(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _STATIC_DEF;
extern const py::re::Pattern _NEW_USER;

py::list<Finding> rule_static_init_order(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _BINARY_IO_FILE;
extern const py::re::Pattern _RAW_CAST;

py::list<Finding> rule_endianness_raw_cast(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _GETTER_DEREF;

py::list<Finding> rule_null_deref_getter(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _FIELD_DEREF;

py::list<Finding> rule_null_deref_field(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

// Given the index of a `for`/`while` keyword, return (body_start, body_end) covering its body:
// the balanced `{...}` if the next non-space after the header `)` is `{`, else up to the next `;`. None: no keyword there.
std::optional<std::tuple<int64_t, int64_t>> _loop_body_span(const py::str& text, int64_t hdr_open);

// hot per-frame / per-chunk / per-entity methods where a big loop runs on the 360's tiny budget
extern const py::re::Pattern _HOTPATH;
extern const py::re::Pattern _NEW_OBJ;

py::list<Finding> rule_hotpath_alloc(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_infinite_loop(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_leak_loop_reassign(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _IMPURE;
extern const py::re::Pattern _ERR_GUARD;
extern const py::re::Pattern _CLEANUP_CALL;

py::list<Finding> rule_side_effect_in_branch(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _DEBUG_BYPASS;

py::list<Finding> rule_debug_bypass(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _FUNC_DEF;

// Yield (signature, body_text, body_start_offset) for each top-level function DEFINITION (brace-matched,
// string/char-literal aware). Shared by the leak detectors, which are function-scoped.
py::list<std::tuple<py::str, py::str, int64_t>> _iter_functions(const py::str& text);

py::list<Finding> rule_leak_temp_new(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_leak_dtor_member(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_ptr_container_compare(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_map_read_insert(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_throw_fallthrough(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);
py::list<Finding> rule_leak_drop_no_delete(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

extern const py::re::Pattern _DISPATCH_IF;

py::list<Finding> rule_silent_dispatch(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx& plat);

// Registry -- (id, description, function). Add new rules here.
extern const py::list<std::tuple<py::str, py::str, RuleFn>> RULES;

extern const py::re::Pattern _CLASS_HDR;
extern const py::set<py::str> _NOT_METHODS;

// Index of the '}' matching the '{' at open_idx (len(src) if unbalanced).
int64_t _match_brace(const py::str& src, int64_t open_idx);

// CROSS-FILE ODR: a class/struct DEFINED with a body in MORE THAN ONE header -> one-definition-rule
// violation (the ledger's outputstream-odr / inputstream-odr / file-odr / stream-odr C4743 class). The
// per-file rules can't see this; it needs the whole tree. Keep ONE canonical definition per type.
int64_t _scan_odr(ledger::Ledger& ledger, const py::str& output_dir);

// {(ancestor_class, method_name)} for every method a DESCENDANT class declares -- i.e. the calls
// that a base constructor invoking `this->method()` would REALLY dispatch wrong (Java calls the
// override; C++ calls the base). Over-capturing a name only makes the rule MORE conservative (flag),
// never less, so a loose method-name scan is safe. Whole-tree; the per-file rule can't see subclasses.
py::set<std::tuple<py::str, py::str>> _build_override_pairs(const py::str& output_dir);

// Run every structural rule over the converted tree, appending Events. Returns count.
int64_t scan_rules(ledger::Ledger& ledger, const py::str& output_dir, const std::optional<py::str>& platform = std::nullopt);

}  // namespace codebreaker::java::rules
