// codebreaker/java/rules.cpp -- typed C++ port of codebreaker.java.rules (see rules.hpp).

#include "codebreaker/java/rules.hpp"

#include <algorithm>

#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/platforms.hpp"
#include "codebreaker/java/textutil.hpp"
#include "codebreaker/java/typemap.hpp"

namespace codebreaker::java::rules {

namespace {

using ledger::Category;
using ledger::Severity;
using py::str;

str S(const char* s) { return str(s); }

constexpr int64_t DOTALL = 16;
constexpr int64_t IGNORECASE = 2;
constexpr int64_t MULTILINE = 8;

Finding found(int64_t line, const str& cat, const str& sev, const char* rule, const str& msg, const str& snippet) {
    return Finding(line, cat, sev, S(rule), msg, snippet);
}

// text.count('\n', 0, at) + 1 -- the line a position is on
int64_t line_at(const str& text, int64_t at) { return text.slice(0, at).count(S("\n")) + 1; }

// raw.strip()[:n]
str head(const str& s, int64_t n) { return s.strip().slice(0, n); }

// raw.split('//', 1)[0]
str before_comment(const str& raw) {
    int64_t i = raw.find(S("//"));
    return i < 0 ? raw : raw.slice(0, i);
}

// the text from `at` on, up to its newline (text[at:].split('\n')[0] style helper: text[a:b].split('\n')[0])
str first_line(const str& s) {
    int64_t i = s.find(S("\n"));
    return i < 0 ? s : s.slice(0, i);
}

}  // namespace

// ---------------------------------------------------------------------------
// Platform context
// ---------------------------------------------------------------------------
_PlatCtx::_PlatCtx(const std::optional<py::str>& platform_id) {
    std::optional<platforms::Platform> plat;
    if (platform_id.has_value()) {
        plat = platforms::get_platform(*platform_id);
    }
    if (plat.has_value()) {
        label = plat->label;
        big_endian = plat->big_endian;
        null_page_protected = plat->null_page_protected;
        null_deref_msg = plat->null_deref_msg;
    } else {
        // plat.get('label', platform_id or 'unknown')
        label = platform_id.has_value() && !platform_id->empty() ? *platform_id : S("unknown");
        big_endian = false;
        null_page_protected = true;
        null_deref_msg = S("NULL dereference -- undefined behaviour in C++ (Java would throw NPE)");
    }
    override_pairs = std::nullopt;
}

// Primitive element types whose `new T[n]` is left uninitialized in C++.
const py::str _PRIM("bool|char|signed char|unsigned char|wchar_t|short|unsigned short|"
                    "int|unsigned int|unsigned|long long|long|float|double|"
                    "byte|jbyte|jboolean|jint|jshort|jlong|jfloat|jdouble");

const py::re::Pattern _NEW_PRIM_ARRAY = py::re::compile(S("\\bnew\\s+(") + _PRIM + S(")\\s*\\[[^\\]]*\\]\\s*(\\(\\s*\\))?"));
const py::re::Pattern _NEW_PTR_ARRAY = py::re::compile(S("\\bnew\\s+\\w[\\w:]*\\s*\\*\\s*\\[[^\\]]*\\]\\s*(\\(\\s*\\))?"));

// profiles.active() is the `generic` profile (no heavy types) until _reseed says otherwise
py::list<py::str> _HEAVY_TYPES;
py::list<py::str> _HEAVY_HIGH;

void _reseed(const py::list<py::str>& heavy_types, const py::list<py::str>& heavy_types_high) {
    _HEAVY_TYPES = py::list<py::str>::from(heavy_types);
    _HEAVY_HIGH = py::list<py::str>::from(heavy_types_high);
}

py::str _read(const py::str& path) {
    return textutil::read_text(path, S("replace"), false);   // errors='replace', but raise on missing
}

py::str _strip_block_comments(const py::str& text) {
    return py::re::sub(S("/\\*.*?\\*/"), S(" "), text, 0, DOTALL);
}

py::list<std::tuple<int64_t, py::str, py::str>> _iter_code_lines(const py::str& text) {
    py::list<std::tuple<int64_t, py::str, py::str>> out;
    int64_t i = 0;
    for (const str& raw : py::split(text, S("\n"))) {
        ++i;
        str code = before_comment(raw);
        if (code.strip().empty()) {
            out.append(std::make_tuple(i, raw, S("")));   // keep index alignment; caller skips empties
        } else {
            out.append(std::make_tuple(i, raw, code));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: uninitialized primitive / pointer arrays
// ---------------------------------------------------------------------------
py::list<Finding> rule_uninit_array(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp")) && !fname.endswith(S(".h"))) {
        return out;
    }
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        if (!code.contains(S("new ")) || !code.contains(S("["))) {
            continue;
        }
        std::optional<py::re::Match> m = _NEW_PRIM_ARRAY.search(code);
        if (m && !m->group_opt(2)) {   // no trailing ()
            str t = m->group(1);
            str sev = t == S("bool") ? Severity::HIGH : Severity::MED;
            out.append(found(i, Category::RISK, sev, "uninit-array",
                             py::percent(S("new %s[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. "
                                           "Use new %s[N]()."),
                                         t, t),
                             head(raw, 160)));
            continue;
        }
        std::optional<py::re::Match> mp = _NEW_PTR_ARRAY.search(code);
        if (mp && !mp->group_opt(1)) {
            out.append(found(i, Category::RISK, Severity::HIGH, "uninit-array",
                             S("new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> "
                               "deref crash. Use new T*[N]() and NULL-check before deref."),
                             head(raw, 160)));
        }
    }
    return out;
}

namespace {

// the '}' (exclusive end) that closes a body opened just before `from`: depth starts at 1
int64_t body_end(const str& text, int64_t from) {
    const std::u32string& u = text.u32();
    int64_t depth = 1, j = from, n = static_cast<int64_t>(u.size());
    while (j < n && depth) {
        if (u[static_cast<size_t>(j)] == U'{') {
            depth += 1;
        } else if (u[static_cast<size_t>(j)] == U'}') {
            depth -= 1;
        }
        j += 1;
    }
    return j;
}

}  // namespace

// ---------------------------------------------------------------------------
// Rule: empty / delegation-only constructor bodies
// ---------------------------------------------------------------------------
py::list<Finding> rule_ctor_stub(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    str cls = fname.slice(0, -4);
    // ClassName::ClassName( ... ) [: init list] { body }
    py::re::Pattern pat = py::re::compile(S("\\b") + py::re::escape(cls) + S("::") + py::re::escape(cls) +
                                          S("\\s*\\(([^)]*)\\)\\s*(:[^{]*)?\\{"));
    for (const py::re::Match& m : pat.finditer(text)) {
        str params = m.group(1).strip();
        str init_list = m.group_opt(2).value_or(S("")).strip();   // `: Base(args), member(0), ...`
        // extract balanced body
        int64_t j = body_end(text, m.end());
        str body = text.slice(m.end(), j - 1);
        int64_t line = line_at(text, m.start());
        // strip comments + whitespace to see if there's any real statement
        str stripped = _strip_block_comments(body);
        py::list<str> kept;
        for (const str& ln : py::split(stripped, S("\n"))) {
            kept.append(before_comment(ln));
        }
        stripped = py::join(S("\n"), kept).strip();
        bool has_delegation_marker = body.contains(S("constructor delegation")) || body.contains(S("was: this("));
        // an empty body WITH an init list (base/member init) is valid C++, not a stub
        if (!params.empty() && init_list.empty() && (stripped.empty() || has_delegation_marker)) {
            out.append(found(line, Category::STUBBED, Severity::HIGH, "ctor-stub",
                             py::percent(S("constructor %s(%s) has an empty/delegation-only body -- field init and static "
                                           "REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer "
                                           "bugs). Replicate the real ctor body."),
                                         cls, params.slice(0, 60)),
                             (cls + "(" + params + ") {...}").slice(0, 160)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: Rule of Three -- owns heap but missing destructor / copy control
// ---------------------------------------------------------------------------
py::list<Finding> rule_rule_of_three(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text,
                                     const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp")) || !header_text.has_value()) {
        return out;
    }
    str cls = fname.slice(0, -4);
    py::re::Pattern ctor_pat = py::re::compile(S("\\b") + py::re::escape(cls) + S("::") + py::re::escape(cls) +
                                               S("\\s*\\([^)]*\\)\\s*(?::[^{]*)?\\{"));
    bool owns_heap = false;
    int64_t ctor_line = 0;
    for (const py::re::Match& m : ctor_pat.finditer(text)) {
        int64_t j = body_end(text, m.end());
        str body = text.slice(m.end(), j - 1);
        if (py::re::search(S("=\\s*new\\s+\\w"), body) || py::re::search(S("\\bnew\\s+\\w[\\w:]*\\s*\\["), body)) {
            owns_heap = true;
            ctor_line = line_at(text, m.start());
            break;
        }
    }
    if (!owns_heap) {
        return out;
    }
    str h = _strip_block_comments(*header_text);
    str ec = py::re::escape(cls);
    bool has_dtor = bool(py::re::search(S("~") + ec + S("\\s*\\("), h));
    bool has_copy_ctor = bool(py::re::search(S("\\b") + ec + S("\\s*\\(\\s*const\\s+") + ec + S("\\s*&"), h));
    bool has_copy_assign = bool(py::re::search(S("operator\\s*=\\s*\\(\\s*const\\s+") + ec + S("\\s*&"), h));
    bool copy_disabled = (h.contains(S("private:")) && has_copy_ctor) || h.contains(S("DISALLOW_COPY"));
    if (!has_dtor) {
        out.append(found(ctor_line, Category::RISK, Severity::HIGH, "rule-of-three",
                         py::percent(S("%s allocates with `new` in its constructor but declares NO destructor -> leak "
                                       "(Java GC'd this; C++ does not)."),
                                     cls),
                         S("")));
    } else if (!(has_copy_ctor && has_copy_assign) && !copy_disabled) {
        out.append(found(ctor_line, Category::RISK, Severity::MED, "rule-of-three",
                         py::percent(S("%s owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: "
                                       "a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). "
                                       "Add them or make copying private."),
                                     cls),
                         S("")));
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: heavy/owning type passed or returned BY VALUE
// ---------------------------------------------------------------------------
py::list<Finding> rule_value_copy_owner(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!(fname.endswith(S(".cpp")) || fname.endswith(S(".h")))) {
        return out;
    }
    // Blank comments to spaces (length- and newline-preserving) so a type name inside a comment like
    // "// glDrawArrays (Tessellator path)" is NOT mistaken for a by-value parameter. Offsets stay valid.
    str src = py::re::sub(S("//[^\\n]*"), [](const py::re::Match& m) { return py::repeat(S(" "), py::len(m.group(0))); }, text);
    src = py::re::sub(S("/\\*.*?\\*/"), [](const py::re::Match& m) { return py::re::sub(S("[^\\n]"), S(" "), m.group(0)); }, src, 0,
                      DOTALL);
    for (const str& t : _HEAVY_TYPES) {
        // by-value param:  (Type name  |  , Type name   -- but NOT Type& / Type*
        py::re::Pattern param_pat = py::re::compile(S("[(,]\\s*") + py::re::escape(t) + S("\\s+(?!&|\\*)\\w+\\s*[,)]"));
        // by-value return: ^Type Class::method(   or   Type method(
        py::re::Pattern ret_pat = py::re::compile(S("(?:^|\\n)\\s*") + py::re::escape(t) + S("\\s+(?!&|\\*)\\w[\\w:]*\\s*\\("));
        for (const py::re::Match& m : param_pat.finditer(src)) {
            int64_t line = line_at(text, m.start());
            str sev = _HEAVY_HIGH.contains(t) ? Severity::HIGH : Severity::MED;
            out.append(found(line, Category::GUESSED, sev, "value-copy-owner",
                             py::percent(S("%s passed BY VALUE -- copies its whole buffer every call. Take it by reference: "
                                           "const %s& / %s&."),
                                         t, t, t),
                             head(py::split(text, S("\n"))[line - 1], 160)));
        }
        for (const py::re::Match& m : ret_pat.finditer(src)) {
            int64_t line = line_at(text, m.start());
            str sev = _HEAVY_HIGH.contains(t) ? Severity::HIGH : Severity::MED;
            out.append(found(line, Category::GUESSED, sev, "value-copy-owner",
                             py::percent(S("%s returned BY VALUE -- returns a full copy. getInstance() must return a reference."), t),
                             head(py::split(text, S("\n"))[line - 1], 160)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: virtual method called from a constructor (dispatches to base, not override)
// ---------------------------------------------------------------------------
py::list<std::tuple<int64_t, py::str>> _ctor_bodies(const py::str& text, const py::str& cls) {
    py::list<std::tuple<int64_t, py::str>> out;
    py::re::Pattern pat = py::re::compile(S("\\b") + py::re::escape(cls) + S("::") + py::re::escape(cls) +
                                          S("\\s*\\([^)]*\\)\\s*(?::[^{]*)?\\{"));
    for (const py::re::Match& m : pat.finditer(text)) {
        int64_t j = body_end(text, m.end());
        out.append(std::make_tuple(line_at(text, m.start()), text.slice(m.end(), j - 1)));
    }
    return out;
}

// A constructor that calls one of its own virtual methods: in C++ the call dispatches to THIS
// class's version (the vtable isn't the derived one yet during construction), NOT a subclass
// override -- unlike Java. So overridden behaviour is silently skipped and wrong data gets recorded.
// Flag it; set the value directly or finish it after construction.
py::list<Finding> rule_virtual_call_in_ctor(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text,
                                            const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp")) || !header_text.has_value()) {
        return out;
    }
    str cls = fname.slice(0, -4);
    str h = _strip_block_comments(*header_text);
    py::set<str> virt;
    for (const str& n : py::re::findall(S("\\bvirtual\\b[^;{]*?\\b(\\w+)\\s*\\("), h)) {
        virt.add(n);
    }
    virt.discard(cls);   // not the ctor itself
    py::set<str> virt2;  // not the dtor
    for (const str& n : virt) {
        if (!n.startswith(S("~"))) {
            virt2.add(n);
        }
    }
    if (virt2.size() == 0) {
        return out;
    }
    for (const auto& [line0, body] : _ctor_bodies(text, cls)) {
        str bstr = _strip_block_comments(body);
        for (const str& name : py::sorted(virt2)) {   // (sorted: the findings of one constructor come in one order)
            // `this->name(` or a bare `name(` (NOT obj->name / obj.name / Class::name -- those are fine)
            bool m = bool(py::re::search(S("\\bthis\\s*->\\s*") + py::re::escape(name) + S("\\s*\\("), bstr)) ||
                     bool(py::re::search(S("(?<![\\w.>:])") + py::re::escape(name) + S("\\s*\\("), bstr));
            if (!m) {
                continue;
            }
            // PRECISION: the base-vs-override divergence only bites if a DESCENDANT actually overrides
            // this method. When the whole-tree hierarchy is known (scan_rules populates override_pairs),
            // skip calls no subclass overrides -- they dispatch identically in C++ and Java. If the index
            // is absent (per-file/unit-test call), stay conservative and flag.
            if (plat.override_pairs.has_value() && !plat.override_pairs->contains(std::make_tuple(cls, name))) {
                continue;
            }
            out.append(found(line0, Category::RISK, Severity::MED, "virtual-call-in-ctor",
                             py::percent(S("%s's constructor calls virtual %s() -- C++ dispatches to %s's own version, NOT "
                                           "a subclass override (Java would call the override). Wrong data may be recorded "
                                           "(the Block light-opacity bug). Set the field directly or finish init post-ctor."),
                                         cls, name, cls),
                             S("")));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: eager static init with a cross-translation-unit dependency (init-order crash class)
// ---------------------------------------------------------------------------
// A static field DEFINITION with an initializer: `<type> <Class>::s_<name> = <init>;`
const py::re::Pattern _STATIC_DEF = py::re::compile(S("^[A-Za-z_][\\w:<>\\*&,\\s]*?\\b(\\w+::s_\\w+)\\s*=\\s*(.+);\\s*$"), MULTILINE);
// constructing a user class (not a safe runtime value type) -> its ctor may read other statics
const py::re::Pattern _NEW_USER = py::re::compile(S("\\bnew\\s+(?!JavaString\\b|JArray\\b|JList\\b|JObject\\b|JCat\\b)[A-Z]\\w*"));

// An EAGER static field definition that constructs a user class or reads another class's static
// (`::s_`) runs at static-init time in UNDEFINED cross-translation-unit order -- the static-init-
// order crash class. The converter defers these into initStatics(), run by j2x_initAll() in
// dependency order; this rule flags any that slipped.
py::list<Finding> rule_static_init_order(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    for (const py::re::Match& m : _STATIC_DEF.finditer(text)) {
        str init = m.group(2);
        if (init.contains(S("::s_")) || _NEW_USER.search(init)) {
            int64_t line = line_at(text, m.start());
            out.append(found(line, Category::RISK, Severity::MED, "static-init-order",
                             py::percent(S("eager static init `%s = ...` runs in UNDEFINED cross-translation-unit order "
                                           "(it constructs a class or reads another class's statics) -- the static-init-order "
                                           "crash class. Defer it into initStatics() (run in dependency order by j2x_initAll)."),
                                         m.group(1)),
                             m.group(0).strip().slice(0, 160)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: raw multi-byte cast/copy in a binary-serialization file (big-endian hazard)
// ---------------------------------------------------------------------------
// Files that read/write the on-disk binary formats (NBT, save/region, streams, compressed IO).
const py::re::Pattern _BINARY_IO_FILE = py::re::compile(S("(NBT|Stream|Save|Region|Compressed|Chunk.*(Loader|Provider))"), IGNORECASE);
// A raw reinterpret over a >1-byte scalar, or memcpy -- byte-swaps on big-endian PowerPC.
const py::re::Pattern _RAW_CAST = py::re::compile(
    S("reinterpret_cast\\s*<|memcpy\\s*\\(|\\*\\s*\\(\\s*(?:int|short|long|long long|float|double|unsigned)\\b[\\w ]*\\*\\s*\\)"));

// Big-endian targets (Xbox 360, PS3, Wii) byte-swap multi-byte values. The NBT / save / stream
// code reads & writes one byte at a time (MSB-first) on purpose -- that is endian-agnostic. A raw
// `reinterpret_cast`, `*(int*)ptr`, or `memcpy` over a serialized byte buffer silently corrupts
// saves / desyncs the protocol on big-endian. Skipped entirely on little-endian targets.
py::list<Finding> rule_endianness_raw_cast(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!plat.big_endian) {
        return out;
    }
    if (!fname.endswith(S(".cpp")) || !_BINARY_IO_FILE.search(fname)) {
        return out;
    }
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        if (code.empty()) {
            continue;
        }
        if (_RAW_CAST.search(code)) {
            out.append(found(i, Category::WARNING, Severity::MED, "endianness",
                             py::percent(S("raw multi-byte cast/memcpy in a binary-serialization file -- on big-endian "
                                           "%s this byte-swaps and corrupts saves/protocol. Read/write byte-at-a-time "
                                           "(MSB-first), then bit-cast the assembled int/long to float/double."),
                                         plat.label),
                             head(raw, 160)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: nullable getter return dereferenced without null check
// ---------------------------------------------------------------------------
const py::re::Pattern _GETTER_DEREF = py::re::compile(S("(\\w+)\\s*\\([^)]*\\)\\s*->\\s*\\w+"));

// getter()->member where the getter can return NULL. Dereferencing without a null check
// is undefined behaviour in C++ (Java would throw NPE). The return must be captured into
// a local and null-checked before dereference.
py::list<Finding> rule_null_deref_getter(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!(fname.endswith(S(".cpp")) || fname.endswith(S(".h")))) {
        return out;
    }
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        if (code.empty() || !code.contains(S("->"))) {
            continue;
        }
        for (const py::re::Match& m : _GETTER_DEREF.finditer(code)) {
            str fn_name = m.group(1);
            if (typemap::KNOWN_NULLABLE_GETTERS.contains(fn_name)) {
                out.append(found(i, Category::RISK, Severity::HIGH, "null-deref-getter",
                                 py::percent(S("%s() can return NULL -- dereferencing without a null check is a %s. "
                                               "Capture into a local and check before use."),
                                             fn_name, plat.null_deref_msg),
                                 head(raw, 160)));
            }
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Rule: known-nullable field dereferenced without a preceding null check
// ---------------------------------------------------------------------------
const py::re::Pattern _FIELD_DEREF = py::re::compile(S("(?:this->)?(\\w+)->"));

// A known-nullable field (worldObj, mc, thePlayer, etc.) dereferenced on a line
// where there is no preceding null check.
py::list<Finding> rule_null_deref_field(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    py::list<str> lines = py::split(text, S("\n"));
    int64_t i = -1;
    for (const str& raw_line : lines) {
        ++i;
        str code = before_comment(raw_line);
        if (code.strip().empty() || !code.contains(S("->"))) {
            continue;
        }
        for (const py::re::Match& m : _FIELD_DEREF.finditer(code)) {
            str field = m.group(1);
            if (!typemap::KNOWN_NULLABLE_FIELDS.contains(field)) {
                continue;
            }
            str guard = S("\\b") + py::re::escape(field) + S("\\s*[!=]= (?:NULL|nullptr|0)\\b");
            if (py::re::search(guard, code)) {
                continue;
            }
            bool guarded = false;
            for (int64_t j = py::max(static_cast<int64_t>(0), i - 5); j < i; ++j) {
                str prev = before_comment(lines[j]);
                if (py::re::search(guard, prev)) {
                    guarded = true;
                    break;
                }
            }
            if (!guarded) {
                int64_t lineno = i + 1;
                out.append(found(lineno, Category::RISK, Severity::MED, "null-deref-field",
                                 py::percent(S("%s can be NULL -- dereferencing without a null check is a %s. "
                                               "Add `if (%s != NULL)` guard."),
                                             field, plat.null_deref_msg, field),
                                 head(raw_line, 160)));
                break;
            }
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Hang / heap-exhaustion class: per-iteration heap allocation inside a loop
// ---------------------------------------------------------------------------
std::optional<std::tuple<int64_t, int64_t>> _loop_body_span(const py::str& text, int64_t hdr_open) {
    static const py::re::Pattern kw = py::re::compile(S("\\b(for|while)\\b"));
    const std::u32string& u = text.u32();
    int64_t n = static_cast<int64_t>(u.size());
    std::optional<py::re::Match> m = kw.match(text, hdr_open);
    if (!m) {
        return std::nullopt;
    }
    int64_t p = text.find(S("("), m->end());
    if (p < 0) {
        return std::nullopt;
    }
    int64_t depth = 0, j = p;
    while (j < n) {   // balance the header parens
        if (u[static_cast<size_t>(j)] == U'(') {
            depth += 1;
        } else if (u[static_cast<size_t>(j)] == U')') {
            depth -= 1;
            if (depth == 0) {
                break;
            }
        }
        j += 1;
    }
    int64_t k = j + 1;
    auto space = [&](int64_t at) {
        char32_t c = u[static_cast<size_t>(at)];
        return c == U' ' || c == U'\t' || c == U'\r' || c == U'\n';
    };
    while (k < n && space(k)) {
        k += 1;
    }
    if (k < n && u[static_cast<size_t>(k)] == U'{') {
        int64_t d = 0, t = k;
        while (t < n) {
            if (u[static_cast<size_t>(t)] == U'{') {
                d += 1;
            } else if (u[static_cast<size_t>(t)] == U'}') {
                d -= 1;
                if (d == 0) {
                    return std::make_tuple(k, t);
                }
            }
            t += 1;
        }
        return std::make_tuple(k, n);
    }
    int64_t end = text.find(S(";"), k);
    return std::make_tuple(k, end >= 0 ? end : n);
}

// hot per-frame / per-chunk / per-entity methods where a big loop runs on the 360's tiny budget
const py::re::Pattern _HOTPATH = py::re::compile(S("\\b(generate|populate|render\\w*|doRender\\w*|tick|onUpdate|updateEntity|"
                                                   "onLivingUpdate|updateRenderer\\w*|getBlock\\w*|relightBlock|updateLighting|"
                                                   "func_\\w+)\\s*\\("));
const py::re::Pattern _NEW_OBJ = py::re::compile(S("\\bnew\\s+([A-Z]\\w*)\\s*[({]"));   // heap object (not new T[]) construction

// PERF/HANG (flag-only): a heap `new` INSIDE a loop allocates once PER ITERATION. Java's GC hides
// this; on the 360 a large loop (a chunk/tree/entity scan run millions of times) becomes an
// allocation storm -> heap exhaustion / multi-second stall / apparent hang. Higher severity inside a
// hot-path method. Not auto-fixed (the fix -- hoist the allocation, pool it, or stack it -- needs
// human judgement); flagged for review.
py::list<Finding> rule_hotpath_alloc(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    py::set<int64_t> seen;
    for (const py::re::Match& lm : py::re::finditer(S("\\b(?:for|while)\\b"), text)) {
        auto span = _loop_body_span(text, lm.start());
        if (!span) {
            continue;
        }
        auto [bstart, bend] = *span;
        str body = text.slice(bstart, bend);
        // is this loop lexically inside a hot-path method? (cheap: scan back to the enclosing signature)
        str hd = text.slice(py::max(static_cast<int64_t>(0), lm.start() - 4000), lm.start());
        str tail_part = hd;
        if (hd.contains(S("\n\n"))) {   // head.rsplit('\n\n', 1)[-1]
            tail_part = hd.slice_from(hd.rfind(S("\n\n")) + 2);
        }
        bool hot = bool(_HOTPATH.search(tail_part));
        // One-time init: a `new` in a loop inside a CONSTRUCTOR runs once per object (building fixed
        // structures -- the chunk grid, the entity roster), NOT once per frame. That is not an allocation
        // hot path, so don't flag it. The nearest `X::Y(` before the loop is the enclosing definition;
        // `X::X(` is a constructor. Per-frame churn lives in tick/render/collision methods (still flagged).
        std::optional<py::re::Match> sigm;
        for (const py::re::Match& sm : py::re::finditer(S("\\b([A-Za-z_]\\w*)\\s*::\\s*(~?)\\s*([A-Za-z_]\\w*)\\s*\\("), hd)) {
            sigm = sm;
        }
        if (sigm && sigm->group(2).empty() && sigm->group(1) == sigm->group(3)) {
            continue;
        }
        for (const py::re::Match& nm : _NEW_OBJ.finditer(body)) {
            int64_t line = line_at(text, bstart + nm.start());
            if (seen.contains(line)) {
                continue;
            }
            seen.add(line);
            str sev = hot ? Severity::HIGH : Severity::MED;
            out.append(found(line, Category::RISK, sev, "hotpath-alloc",
                             py::percent(S("heap `new %s` inside a loop allocates once PER ITERATION -- on %s this is an "
                                           "allocation storm / heap-exhaustion hang when the loop is large (Java's GC hid it). "
                                           "Hoist it out of the loop, pool it, or stack-allocate it."),
                                         nm.group(1), plat.label),
                             first_line(text.slice(bstart + nm.start(), bstart + nm.start() + 120)).strip()));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Hang class: an unbounded loop with no exit in its body
// ---------------------------------------------------------------------------
// HANG (flag-only): `while (true)` / `for (;;)` whose body contains no break / return / goto /
// throw -- an unconditional infinite loop. On the 360 this wedges the (often main) thread with no
// watchdog. Flagged for review (a converter mistranslation of a labeled break, or a genuinely
// unbounded loop, both surface here).
py::list<Finding> rule_infinite_loop(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx& plat) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    for (const py::re::Match& lm : py::re::finditer(S("\\b(while\\s*\\(\\s*(?:true|1)\\s*\\)|for\\s*\\(\\s*;\\s*;\\s*\\))"), text)) {
        auto span = _loop_body_span(text, lm.start());
        if (!span) {
            continue;
        }
        auto [bstart, bend] = *span;
        str body = text.slice(bstart, bend);
        // A loop that Sleeps / waits / yields is an INTENTIONAL service/daemon loop (e.g. the HeapTrack
        // monitor `for(;;){ Sleep(2500); ... }`), not a hang -- don't flag it.
        if (!py::re::search(S("\\b(break|return|goto|throw|exit|Sleep|WaitFor\\w+|SwitchToThread|MsgWaitFor\\w+)\\b"), body)) {
            int64_t line = line_at(text, lm.start());
            out.append(found(line, Category::RISK, Severity::MED, "infinite-loop",
                             py::percent(S("unbounded loop (`%s`) with no break/return/goto in its body -- wedges the thread "
                                           "on %s (no watchdog). Verify it has an exit; a mistranslated labeled break lands here."),
                                         lm.group(1), plat.label),
                             lm.group(0)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Leak class: a heap local reassigned in a loop without freeing the prior value
// ---------------------------------------------------------------------------
// LEAK (flag-only): a local pointer reassigned via `new` inside a loop, with no `delete` of the
// prior value first -- every iteration leaks the previous object (the WorldGenBigTree reuse pattern
// the live tree fixed by hand). The throwaway `(new T())->m()` case is auto-fixed elsewhere; this is
// the harder reassign-in-loop case, flagged for a human `delete before reassign` + `delete after loop`.
py::list<Finding> rule_leak_loop_reassign(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    for (const py::re::Match& lm : py::re::finditer(S("\\b(?:for|while)\\b"), text)) {
        auto span = _loop_body_span(text, lm.start());
        if (!span) {
            continue;
        }
        auto [bstart, bend] = *span;
        str body = text.slice(bstart, bend);
        for (const py::re::Match& am : py::re::finditer(S("\\b(\\w+)\\s*=\\s*new\\s+[A-Z]\\w*\\s*[({]"), body)) {
            str var = am.group(1);
            str ev = py::re::escape(var);
            if (py::re::search(S("\\bdelete\\b[^;]*\\b") + ev + S("\\b"), body.slice(py::max(static_cast<int64_t>(0), am.start() - 200), am.start()))) {
                continue;   // a delete of this var precedes the reassign
            }
            if (py::re::search(S("\\bdelete\\b[^;{}]*\\b") + ev + S("\\b"), text.slice(py::max(static_cast<int64_t>(0), bstart - 500), bstart))) {
                continue;   // freed BEFORE the loop (e.g. the raycast: delete at top,
                            // reassign once then return -- not a per-iteration leak)
            }
            str tail = body.slice(am.end(), am.end() + 60);
            if (py::re::match(S("[^;{}]*;\\s*(?:return|break)\\b"), tail)) {
                continue;   // reassign is immediately followed by return/break
            }
            int64_t line = line_at(text, bstart + am.start());
            out.append(found(line, Category::RISK, Severity::MED, "leak-loop-reassign",
                             py::percent(S("`%s = new ...` inside a loop with no `delete %s` first -- each iteration leaks the "
                                           "previous object (Java GC'd it). Add `delete %s;` before the reassign and once after "
                                           "the loop."),
                                         var, var, var),
                             am.group(0).slice(0, 120)));
            break;   // one finding per loop is enough
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Side-effect preservation net (the RenderPig class): an impure statement inside a guarded branch
// ---------------------------------------------------------------------------
// impure = a call, an allocation, or an assignment to non-local (field/global/native) state.
// An impure statement: a call, a `new`, or an assignment to field/global/native state. Broad on purpose --
// the RenderPig bug was a plain method CALL (`loadTexture`) in a collapsed guard, so calls must count. The
// call pattern EXCLUDES control keywords (`if (x) ;`, `while (y) ;`) -- those are not function calls, and a
// guard body reduced to `if (cond) ;` must not read as an impure statement.
const py::re::Pattern _IMPURE = py::re::compile(S("\\b(?!(?:if|for|while|switch|return|sizeof|catch|do)\\b)\\w+\\s*\\([^)]*\\)\\s*;"
                                                  "|\\bnew\\s+[A-Za-z_]|->\\s*\\w+\\s*=(?!=)|::\\s*\\w+\\s*=(?!=)|\\bthis->\\w+\\s*=(?!=)"));
// NOISE FILTER, keyed on the two real false-positive shapes (not on statement kind):
//  (1) the guard is a FAILURE / NULL / edge check -- `if (FAILED(hr) || !t->tex)`, `if (n == 0)` -- so the
//      branch is an error/early-out path where a side effect + return is expected, not the RenderPig shape;
//  (2) the body's only impure statements are CLEANUP -- Release/delete/free/close/Unlock -- ordinary teardown.
// The RenderPig shape is a *normal* guard whose body does constructive work (loadTexture) then returns.
// A guard is an early-out (error / null / bounds / state-compare) -- NOT the RenderPig shape -- when it
// contains a FAILED/SUCCEEDED call, a negation, or ANY relational/equality operator against a sentinel.
// The RenderPig collapse only threatens a *plain predicate* guard (`if (cond)`, `if (isSolidTile(...))`)
// whose branch does constructive work; those have no comparison operator and stay flagged.
const py::re::Pattern _ERR_GUARD = py::re::compile(S("\\bFAILED\\s*\\(|\\bSUCCEEDED\\s*\\(|[=!<>]=|!\\s*[\\w(]|(?<![-\\w<>])[<>](?![<>=])"));
const py::re::Pattern _CLEANUP_CALL =
    py::re::compile(S("\\b(?:Release|delete|free|close|Unlock|EndScene|Reset|clear|dispose|destroy)\\b"), IGNORECASE);

// SIDE-EFFECT PRESERVATION audit net. Flags an `if (...) { ... }` block that contains BOTH an
// impure statement (call / `new` / assignment to field/global/native state) AND a `return` -- the
// exact shape a naive 'collapse the guard into a predicate return' would corrupt by dropping or
// unconditionally hoisting the side effect (the RenderPig loadTexture bug). The converter is
// order-preserving so these are almost always intact; the tag `J2X-REVIEW[side-effect-in-branch]`
// makes every one greppable so a full-codebase pass can confirm the guard survived.
py::list<Finding> rule_side_effect_in_branch(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    const std::u32string& u = text.u32();
    for (const py::re::Match& m : py::re::finditer(S("\\bif\\s*\\("), text)) {
        int64_t p = m.end() - 1;   // balance the condition parens
        int64_t depth = 0, j = p, n = static_cast<int64_t>(u.size());
        while (j < n) {
            if (u[static_cast<size_t>(j)] == U'(') {
                depth += 1;
            } else if (u[static_cast<size_t>(j)] == U')') {
                depth -= 1;
                if (depth == 0) {
                    break;
                }
            }
            j += 1;
        }
        int64_t k = j + 1;
        while (k < n && (u[static_cast<size_t>(k)] == U' ' || u[static_cast<size_t>(k)] == U'\t' ||
                         u[static_cast<size_t>(k)] == U'\r' || u[static_cast<size_t>(k)] == U'\n')) {
            k += 1;
        }
        if (k >= n || u[static_cast<size_t>(k)] != U'{') {   // only braced blocks (single-stmt is low-risk)
            continue;
        }
        depth = 0;
        int64_t t = k;
        while (t < n) {
            if (u[static_cast<size_t>(t)] == U'{') {
                depth += 1;
            } else if (u[static_cast<size_t>(t)] == U'}') {
                depth -= 1;
                if (depth == 0) {
                    break;
                }
            }
            t += 1;
        }
        str cond = text.slice(m.end(), j);   // the condition text, between the parens
        str body = text.slice(k + 1, t);
        if (_ERR_GUARD.search(cond)) {   // (1) failure / null / edge guard -> expected path
            continue;
        }
        // (2) body whose impure work is ONLY cleanup (every impure line matches a teardown verb)
        py::list<str> impure_lines;
        for (const str& ln : py::split(body, S(";"))) {
            if (_IMPURE.search(ln + S(";"))) {
                impure_lines.append(ln);
            }
        }
        bool all_cleanup = true;
        for (const str& ln : impure_lines) {
            if (!_CLEANUP_CALL.search(ln)) {
                all_cleanup = false;
                break;
            }
        }
        if (py::len(impure_lines) > 0 && all_cleanup) {
            continue;
        }
        // (3) a call/construction that IS the returned value (`return f(x);`, `return std::string(s);`) is
        // not a side-effecting STATEMENT -- it just computes the result. Blank return expressions before the
        // impure test so only a real separate side effect (`this->x = new ...; return;`) still flags.
        str body_side = py::re::sub(S("\\breturn\\b[^;]*;"), S(" "), body);
        if (body.contains(S("return")) && _IMPURE.search(body_side) && !body.contains(S("J2X-REVIEW"))) {
            int64_t line = line_at(text, m.start());
            out.append(found(line, Category::RISK, Severity::LOW, "side-effect-in-branch",
                             S("J2X-REVIEW[side-effect-in-branch]: a guarded branch runs an impure statement AND "
                               "returns -- confirm the side effect still executes ONLY under this guard (it must "
                               "never be collapsed/hoisted into an unconditional prelude; the RenderPig class)."),
                             text.slice(m.start(), k).replace(S("\n"), S(" ")).slice(0, 120)));
        }
    }
    return out;
}

const py::re::Pattern _DEBUG_BYPASS = py::re::compile(S("\\b(OutputDebugString[AW]?|printf|fprintf|std::(?:cout|cerr)|puts)\\s*[\\(<]"));

// Flags raw debug output (printf / OutputDebugString / cout) that does NOT go through the single
// Debug.h `DBG_*` macros. The user's rule: all debug instrumentation must route through the one
// master switch (DEBUG_ENABLED) so toggling it off silences everything. A stray printf stays hot in
// a retail build and spams the console -- tag it `J2X-REVIEW[debug-bypass]` so it is greppable and
// can be converted to a DBG_* macro (or removed).
py::list<Finding> rule_debug_bypass(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    if (fname.contains(S("Debug.cpp"))) {   // the backend itself legitimately uses printf
        return out;
    }
    for (const py::re::Match& m : _DEBUG_BYPASS.finditer(text)) {
        int64_t line = line_at(text, m.start());
        // skip lines already routed/annotated
        int64_t ls = text.slice(0, m.start()).rfind(S("\n")) + 1;
        int64_t le = text.find(S("\n"), m.start());
        str linetext = text.slice(ls, le != -1 ? le : py::len(text));
        if (linetext.contains(S("DBG_")) || linetext.contains(S("J2X-REVIEW")) || linetext.contains(S("Debug_Printf"))) {
            continue;
        }
        out.append(found(line, Category::RISK, Severity::LOW, "debug-bypass",
                         S("J2X-REVIEW[debug-bypass]: raw debug output not routed through Debug.h -- convert to a "
                           "DBG_* macro so the master DEBUG_ENABLED switch governs it (else it stays hot in retail "
                           "and spams the console)."),
                         linetext.strip().slice(0, 120)));
    }
    return out;
}

const py::re::Pattern _FUNC_DEF = py::re::compile(S("^([A-Za-z_][\\w:<>,\\*&\\s]*?\\b[\\w:~]+)\\s*\\(([^;{}]*)\\)\\s*(?:const\\s*)?\\{"), MULTILINE);

py::list<std::tuple<py::str, py::str, int64_t>> _iter_functions(const py::str& text) {
    py::list<std::tuple<py::str, py::str, int64_t>> out;
    const std::u32string& u = text.u32();
    for (const py::re::Match& m : _FUNC_DEF.finditer(text)) {
        int64_t ob = m.end() - 1;
        int64_t depth = 0, j = ob, n = static_cast<int64_t>(u.size());
        while (j < n) {
            char32_t c = u[static_cast<size_t>(j)];
            if (c == U'"' || c == U'\'') {
                char32_t q = c;
                j += 1;
                while (j < n) {
                    if (u[static_cast<size_t>(j)] == U'\\') {
                        j += 2;
                        continue;
                    }
                    if (u[static_cast<size_t>(j)] == q) {
                        break;
                    }
                    j += 1;
                }
            } else if (c == U'{') {
                depth += 1;
            } else if (c == U'}') {
                depth -= 1;
                if (depth == 0) {
                    break;
                }
            }
            j += 1;
        }
        out.append(std::make_tuple(m.group(1).strip(), text.slice(ob + 1, j), ob + 1));
    }
    return out;
}

// No-GC TEMP LEAK: a local `T* v = new ...` that is never delete'd, returned, stored to a field/local,
// or handed to a container in its function -> leaks on the GC-less 360 heap. The per-call Random /
// comparator / temp-point class (renderstars-random-leak, entitysorter-comparator-leak, noisegen-random-
// temp-leak, pathfinder-dims-point-leak). NOTE: a receiver-only use like `(new Random())->nextInt()` or
// passing to `Arrays::sort` does NOT transfer ownership -- still a leak.
py::list<Finding> rule_leak_temp_new(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    for (const auto& [sig, body, off] : _iter_functions(text)) {
        for (const py::re::Match& am : py::re::finditer(S("\\b[A-Za-z_][\\w:]*\\s*\\*\\s*(\\w+)\\s*=\\s*new\\b"), body)) {
            str v = py::re::escape(am.group(1));
            str rest = body.slice_from(am.end());
            str nr = S("(?!\\s*(?:->|\\.|\\(|\\[))");   // var used as a VALUE, not a receiver (`v->m()`) or `v[i]`
            bool escaped =
                bool(py::re::search(S("\\bdelete\\b[^;{}]*\\b") + v + S("\\b"), rest)) ||                      // freed
                bool(py::re::search(S("\\breturn\\b[^;{}]*\\b") + v + S("\\b") + nr, rest)) ||                  // returned (the value itself)
                bool(py::re::search(S("(?<![=<>!])=\\s*") + v + S("\\b") + nr, rest)) ||                        // stored: x = v / field = v
                bool(py::re::search(S("(?:\\.|->)(?:add\\w*|push_back|push_front|put|insert|append|offer|set\\w*|register\\w*)"
                                      "\\s*\\([^;{}]*\\b") + v + S("\\b"), rest));                            // added to a container/registry
            if (escaped) {
                continue;
            }
            int64_t line = line_at(text, off + am.start());
            out.append(found(line, Category::RISK, Severity::MED, "leak-temp-new",
                             py::percent(S("local `%s = new ...` is never delete'd, returned, stored, or added to a container in this "
                                           "function -> leaks on the GC-less heap (Java GC'd it). `delete %s;` on every exit path."),
                                         am.group(1), am.group(1)),
                             am.group(0).slice(0, 120)));
            break;   // one finding per function is enough
        }
    }
    return out;
}

// No-GC DTOR-MEMBER LEAK: a pointer field assigned `new` in the constructor but NOT `delete`d in the
// destructor -> leaks for the object's whole lifetime. Stronger than rule-of-three (which only checks the
// dtor EXISTS): this names each unfreed member. The ledger's chunkprovider-mapgen-dtor-leak /
// mapgenbase-rand-dtor-leak / noiseoctaves-subgen-dtor-leak class.
py::list<Finding> rule_leak_dtor_member(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    str cls = fname.slice(0, -4);
    str ec = py::re::escape(cls);
    py::set<str> ctor_new;
    std::optional<str> dtor_body;
    py::dict<str, int64_t> ctor_line;
    for (const auto& [sig, body, off] : _iter_functions(text)) {
        if (py::re::search(S("\\b") + ec + S("::~") + ec + S("\\b"), sig)) {
            dtor_body = dtor_body.value_or(S("")) + body;
        } else if (py::re::search(S("\\b") + ec + S("::") + ec + S("\\b"), sig)) {
            // the constructor's own locals (`T* v = NULL;` first, `v = new ...` later: the back end's ordered-
            // operand temporaries j2xoN, a decompiler local) are not members
            py::set<str> local;
            for (const str& x : py::re::findall(S("\\b[A-Za-z_][\\w:<>]*\\s*\\*+\\s*(\\w+)\\s*(?:=|;)"), body)) {
                local.add(x);
            }
            for (const py::re::Match& mm : py::re::finditer(S("(?:this->)?(\\w+)\\s*=\\s*new\\b"), body)) {
                if (local.contains(mm.group(1)) && !mm.group(0).startswith(S("this->"))) {
                    continue;
                }
                ctor_new.add(mm.group(1));
                if (!ctor_line.contains(mm.group(1))) {
                    ctor_line.set(mm.group(1), line_at(text, off + mm.start()));
                }
            }
        }
    }
    if (ctor_new.size() == 0) {
        return out;
    }
    if (!dtor_body.has_value()) {
        return out;   // no dtor at all -> rule-of-three already reports it
    }
    for (const str& mem : py::sorted(ctor_new)) {
        if (py::re::search(S("\\bdelete\\b[^;{}]*\\b") + py::re::escape(mem) + S("\\b"), *dtor_body)) {
            continue;
        }
        out.append(found(ctor_line.at(mem), Category::RISK, Severity::HIGH, "leak-dtor-member",
                         py::percent(S("member '%s' is `new`'d in the constructor but never `delete`d in ~%s() -> leaks for the "
                                       "object's whole lifetime on the GC-less heap. Add `delete %s;` to the destructor."),
                                     mem, cls, mem),
                         mem + " = new ..."));
    }
    return out;
}

// A std::set<T*> / std::map<T*,..> / JSet<T*> compares & keys by POINTER ADDRESS, not by value. Where
// the game means value-equality (Java HashSet/HashMap use .equals/.hashCode), two logically-equal objects
// both count as 'present' (duplicates) and lookups by an equal-but-different pointer MISS. The ledger's
// scheduledtick-pointer-compare-explosion / regionfile-cache-never-hits class. Flags a set/map whose KEY
// is a non-primitive pointer.
py::list<Finding> rule_ptr_container_compare(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!(fname.endswith(S(".h")) || fname.endswith(S(".cpp")))) {
        return out;
    }
    py::set<str> prim{S("int"),    S("char"),   S("short"),   S("long"),     S("float"),    S("double"),   S("bool"),
                      S("unsigned"), S("size_t"), S("void"),    S("int8_t"),   S("uint8_t"),  S("int16_t"),  S("uint16_t"),
                      S("int32_t"), S("uint32_t"), S("int64_t"), S("uint64_t"), S("wchar_t")};
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        for (const py::re::Match& m :
             py::re::finditer(S("\\b(?:std::set|std::unordered_set|JSet)\\s*<\\s*([\\w:]+)\\s*\\*\\s*>"), code)) {
            if (!prim.contains(m.group(1))) {
                out.append(found(i, Category::RISK, Severity::MED, "ptr-container-compare",
                                 py::percent(S("std::set<%s*> compares by POINTER ADDRESS, not value (Java HashSet uses .equals/"
                                               ".hashCode) -- logically-equal entries duplicate and lookups miss. Use a value "
                                               "comparator or key by a value type. (scheduledtick-pointer-compare class.)"),
                                             m.group(1)),
                                 head(code, 150)));
            }
        }
        for (const py::re::Match& m :
             py::re::finditer(S("\\b(?:std::map|std::unordered_map|JMap)\\s*<\\s*([\\w:]+)\\s*\\*\\s*,"), code)) {
            if (!prim.contains(m.group(1))) {
                out.append(found(i, Category::RISK, Severity::MED, "ptr-container-compare",
                                 py::percent(S("std::map<%s*, ...> keys by POINTER ADDRESS, not value -- a lookup with an equal-but-"
                                               "different key MISSES. Key by a value type or add a comparator. "
                                               "(regionfile-cache-never-hits class.)"),
                                             m.group(1)),
                                 head(code, 150)));
            }
        }
    }
    return out;
}

// C++ `map[key]` INSERTS a default on READ, where Java's HashMap.get() returns null. This is the
// map-get-inserts / datawatcher-null-deref-hang class from the ledger: a lookup like `if (chunks[key])`
// silently grows the map with a null entry -> chunkExists() lies -> infinite generation recursion ->
// FREEZE. Flags operator[] READS on map-typed variables. Writes (`map[k] = v`) are the legitimate insert
// and are left alone.
py::list<Finding> rule_map_read_insert(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    str hay = text + (header_text.has_value() && !header_text->empty() ? S("\n") + *header_text : S(""));
    py::set<str> maps;
    for (const str& x : py::re::findall(
             S("\\b(?:std::map|std::unordered_map|JMap|HashMap\\w*|TreeMap\\w*|LinkedHashMap\\w*)\\s*<[^;{}]*>\\s*\\*?\\s*(\\w+)"), hay)) {
        maps.add(x);
    }
    if (maps.size() == 0) {
        return out;
    }
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        for (const str& name : py::sorted(maps)) {   // (sorted: the findings of one line come in one order)
            std::optional<py::re::Match> m = py::re::search(S("\\b") + py::re::escape(name) + S("\\s*\\[[^\\]]+\\]"), code);
            if (!m) {
                continue;
            }
            str after = code.slice_from(m->end()).lstrip();
            if (after.startswith(S("=")) && !after.startswith(S("=="))) {
                continue;   // a write: map[k] = v  (intended insert)
            }
            out.append(found(i, Category::RISK, Severity::HIGH, "map-read-insert",
                             py::percent(S("%s[...] READ: C++ map::operator[] INSERTS a default on read (Java .get() returns null). "
                                           "The silent insert makes a 'contains?' check lie -- the chunkExists->gen-recursion FREEZE "
                                           "class. Use .count()/.find() for lookups; reserve [] for writes."),
                                         name),
                             head(raw, 150)));
            break;
        }
    }
    return out;
}

// On this console C++ `throw` is a NO-OP that does NOT unwind -- execution FALLS THROUGH past it. When
// Java relied on the throw to abort, the code after it now runs -> state corruption / garbage returns.
// The ledger's tessellator-state-throw / path-addpoint-state-throw / root-tag-not-compound-throw class
// (and readfully-eof). Flags a `throw` that is followed by more code in its block (not right before the
// block's `}`).
py::list<Finding> rule_throw_fallthrough(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    py::list<std::tuple<int64_t, str>> rows;
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        if (!code.strip().empty()) {
            rows.append(std::make_tuple(i, code.strip()));
        }
    }
    for (int64_t idx = 0; idx < py::len(rows) - 1; ++idx) {
        const auto& [i, s] = rows[idx];
        if (!s.startswith(S("throw"))) {
            continue;
        }
        if (py::len(s) > 5 && (s[5].isalnum() || s[5] == S("_"))) {
            continue;   // 'throws'/'throwFoo', not a throw statement
        }
        str nxt = std::get<1>(rows[idx + 1]);
        if (nxt.empty() || nxt[0] == S("}") || nxt[0] == S(")") ||
            nxt.startswith(std::make_tuple("case ", "default:", "#"))) {
            continue;   // throw is last in its block -> abort == return here
        }
        out.append(found(i, Category::RISK, Severity::MED, "throw-fallthrough",
                         S("`throw` does NOT unwind on this console -- it is a no-op and the code after it STILL RUNS "
                           "(Java aborted here). Replace with an early `return` / guarded skip so the fall-through can't "
                           "corrupt state or return garbage. (The _TRACE_THROWS runtime tracer names any that fire.)"),
                         s.slice(0, 150)));
    }
    return out;
}

// No-GC DROP LEAK: an OWNING container of pointers (JList<T*>, std::vector<T*>, JMap<..,T*>, set...) has
// an element removed (.clear/.erase/.remove/.pop*) with no `delete` of that element nearby -> the object
// is dropped but never freed on the GC-less heap. The ledger's dead-particle-leak / clear-effects-leak /
// client-chunk-drop-leak / net-*-packets-leak class.
py::list<Finding> rule_leak_drop_no_delete(const py::str& fname, const py::str& text, const std::optional<py::str>& header_text,
                                           const _PlatCtx&) {
    py::list<Finding> out;
    if (!fname.endswith(S(".cpp"))) {
        return out;
    }
    str hay = text + (header_text.has_value() && !header_text->empty() ? S("\n") + *header_text : S(""));
    py::set<str> owners0;
    for (const str& x : py::re::findall(
             S("\\b(?:JList|std::vector|JSet|std::set|JMap|std::map|std::deque)\\s*<[^;{}]*\\*\\s*>\\s*\\*?\\s*(\\w+)"), hay)) {
        owners0.add(x);
    }
    if (owners0.size() == 0) {
        return out;
    }
    // Observer/callback lists hold NON-owned references (registered from outside) -- removing does not free.
    py::set<str> owners;
    for (const str& o : owners0) {
        if (!py::re::search(S("(?i)(listener|observer|callback|handler|watcher|subscriber)s?$"), o)) {
            owners.add(o);
        }
    }
    py::list<std::tuple<int64_t, str>> rows;
    for (const auto& [i, raw, code] : _iter_code_lines(text)) {
        rows.append(std::make_tuple(i, code));
    }
    for (int64_t idx = 0; idx < py::len(rows); ++idx) {
        const auto& [i, code] = rows[idx];
        for (const str& name : py::sorted(owners)) {   // (sorted: the findings of one line come in one order)
            std::optional<py::re::Match> m =
                py::re::search(S("\\b") + py::re::escape(name) + S("(?:\\.|->)(clear|erase|remove|pop_back|pop_front|pop)\\s*\\("), code);
            if (!m) {
                continue;
            }
            py::list<str> win;
            for (int64_t w = py::max(static_cast<int64_t>(0), idx - 6); w < idx + 1; ++w) {
                win.append(std::get<1>(rows[w]));
            }
            str window = py::join(S(" "), win);
            if (window.contains(S("delete"))) {
                continue;   // a delete of the removed element is nearby
            }
            out.append(found(i, Category::RISK, Severity::MED, "leak-drop-no-delete",
                             py::percent(S("%s.%s() removes from an OWNING pointer container with no `delete` -- the object is dropped "
                                           "but never freed on the GC-less heap. Capture the removed pointer and delete it first. "
                                           "(dead-particle / clear-effects / chunk-drop / packet leak class.)"),
                                         name, m->group(1)),
                             head(code, 150)));
            break;
        }
    }
    return out;
}

const py::re::Pattern _DISPATCH_IF = py::re::compile(S("\\bif\\s*\\(\\s*(\\w+)\\s*==\\s*[A-Z][A-Z0-9_]{2,}"));

// FAIL-LOUD DOCTRINE. An enum DISPATCHER that can fall through SILENTLY on a value it doesn't handle --
// exactly how the fog bug hid: `glFogf`/`glFogi` were `if(pname==GL_FOG_START) else if(pname==GL_FOG_END)`
// chains with no final else, so GL_FOG_DENSITY / GL_FOG_MODE matched nothing and were dropped with no
// error. RULE: a dispatcher must END in a loud report (J2X_UNHANDLED) or an explicit J2X_NOOP. This flags
// `if(x==CONST) else if(x==CONST) ...` chains (2+ branches on the same var) that have neither a terminal
// else nor a marker. Chain-scoped (gathers only the if + following `else*` lines) to avoid bleeding into
// neighbouring code.
py::list<Finding> rule_silent_dispatch(const py::str& fname, const py::str& text, const std::optional<py::str>&, const _PlatCtx&) {
    py::list<Finding> out;
    if (!(fname.endswith(S(".cpp")) || fname.endswith(S(".h")))) {
        return out;
    }
    py::list<str> lines = py::split(text, S("\n"));
    int64_t n = py::len(lines);
    for (int64_t i = 0; i < n; ++i) {
        str ln = lines[i];
        std::optional<py::re::Match> m = _DISPATCH_IF.search(ln);
        if (!m) {
            continue;
        }
        if (ln.slice(0, m->start()).rstrip().endswith(S("else"))) {
            continue;   // this `if` is an `else if` -- not a chain start
        }
        str var = m->group(1);
        py::list<str> chain;   // gather the chain: this line + following `else*` lines
        chain.append(ln);
        int64_t k = i + 1;
        while (k < n && (k - i) < 40) {   // skip interior comment/blank lines so an explanatory
            str s = lines[k].strip();     // comment between two else-ifs doesn't truncate the chain
            if (s.startswith(S("else")) || s.empty() || s.startswith(std::make_tuple("//", "/*", "*"))) {
                chain.append(lines[k]);
                k += 1;
            } else {
                break;
            }
        }
        str region = py::join(S("\n"), chain);
        int64_t elseifs = py::len(py::re::findall(
            S("\\belse\\s+if\\s*\\(\\s*") + py::re::escape(var) + S("\\s*==\\s*[A-Z][A-Z0-9_]{2,}"), region));
        if (elseifs < 1) {
            continue;   // a lone `if` (e.g. `if (rc==A || rc==B)`) -- not a multi-branch else-if DISPATCH;
                        // nothing to fall through
        }
        int64_t branches = elseifs + 1;
        if (region.contains(S("J2X_UNHANDLED")) || region.contains(S("J2X_NOOP"))) {
            continue;   // reports the gap, or explicitly blessed
        }
        if (py::re::search(S("\\belse\\b(?!\\s*if)"), region)) {
            continue;   // has a terminal else (assume it handles/reports)
        }
        out.append(found(i + 1, Category::RISK, Severity::HIGH, "silent-dispatch",
                         py::percent(S("enum dispatch on '%s' (%d cases) falls through SILENTLY on an unhandled value -- the "
                                       "fog-bug class. End the chain with J2X_UNHANDLED(\"<fn>\", %s), or J2X_NOOP(\"why\") if the "
                                       "silent fall-through is deliberate."),
                                     var, branches, var),
                         ln.strip().slice(0, 160)));
    }
    return out;
}

// Registry -- (id, description, function). Add new rules here.
const py::list<std::tuple<py::str, py::str, RuleFn>> RULES{
    std::make_tuple(S("silent-dispatch"), S("enum dispatcher can fall through silently on an unhandled value (fail-loud doctrine)"), &rule_silent_dispatch),
    std::make_tuple(S("map-read-insert"), S("map[key] READ inserts a default (Java .get() returns null) -> chunkExists lies -> freeze"), &rule_map_read_insert),
    std::make_tuple(S("throw-fallthrough"), S("throw is a no-op that does not unwind on the 360 -> code after it still runs"), &rule_throw_fallthrough),
    std::make_tuple(S("leak-drop-no-delete"), S("element removed from an owning pointer container without delete (no-GC drop leak)"), &rule_leak_drop_no_delete),
    std::make_tuple(S("leak-temp-new"), S("local new never freed/returned/stored/added -> no-GC temp leak"), &rule_leak_temp_new),
    std::make_tuple(S("leak-dtor-member"), S("member new-d in ctor but not deleted in dtor -> no-GC lifetime leak"), &rule_leak_dtor_member),
    std::make_tuple(S("ptr-container-compare"), S("std::set/map keyed by a non-primitive pointer -> compares by address, not value"), &rule_ptr_container_compare),
    std::make_tuple(S("uninit-array"), S("new T[n] without () leaves primitives/pointers uninitialized"), &rule_uninit_array),
    std::make_tuple(S("ctor-stub"), S("empty or delegation-only constructor body (missing registration)"), &rule_ctor_stub),
    std::make_tuple(S("rule-of-three"), S("owns heap but missing destructor / copy control"), &rule_rule_of_three),
    std::make_tuple(S("value-copy-owner"), S("heavy/owning type passed or returned by value"), &rule_value_copy_owner),
    std::make_tuple(S("virtual-call-in-ctor"), S("constructor calls own virtual method (dispatches to base, not override)"), &rule_virtual_call_in_ctor),
    std::make_tuple(S("static-init-order"), S("eager static init with a cross-TU dependency (undefined init order)"), &rule_static_init_order),
    std::make_tuple(S("endianness"), S("raw multi-byte cast in binary-IO file (big-endian hazard)"), &rule_endianness_raw_cast),
    std::make_tuple(S("null-deref-getter"), S("getter return dereferenced without null check"), &rule_null_deref_getter),
    std::make_tuple(S("null-deref-field"), S("known-nullable field dereferenced without null check"), &rule_null_deref_field),
    std::make_tuple(S("hotpath-alloc"), S("heap allocation inside a loop (per-iteration alloc storm / hang)"), &rule_hotpath_alloc),
    std::make_tuple(S("infinite-loop"), S("unbounded loop with no break/return in body (thread wedge)"), &rule_infinite_loop),
    std::make_tuple(S("leak-loop-reassign"), S("heap local reassigned in a loop without freeing the prior value"), &rule_leak_loop_reassign),
    std::make_tuple(S("side-effect-in-branch"), S("impure statement in a guarded branch that returns (RenderPig class)"), &rule_side_effect_in_branch),
    std::make_tuple(S("debug-bypass"), S("raw debug output not routed through the single Debug.h master switch"), &rule_debug_bypass),
};

const py::re::Pattern _CLASS_HDR = py::re::compile(S("\\b(?:class|struct)\\s+([A-Za-z_]\\w*)\\s*(?:final\\s*)?(?::([^{;()]*))?\\{"));
const py::set<py::str> _NOT_METHODS{S("if"), S("for"), S("while"), S("switch"), S("return"), S("sizeof"), S("catch"), S("new"), S("delete")};

int64_t _match_brace(const py::str& src, int64_t open_idx) {
    const std::u32string& u = src.u32();
    int64_t depth = 0;
    for (int64_t j = open_idx; j < static_cast<int64_t>(u.size()); ++j) {
        if (u[static_cast<size_t>(j)] == U'{') {
            depth += 1;
        } else if (u[static_cast<size_t>(j)] == U'}') {
            depth -= 1;
            if (depth == 0) {
                return j;
            }
        }
    }
    return static_cast<int64_t>(u.size());
}

int64_t _scan_odr(ledger::Ledger& ledger, const py::str& output_dir) {
    py::dict<str, py::dict<str, str>> defs;   // class name -> { file: normalized-body }
    py::list<str> headers;
    for (const str& f : py::sorted(py::os::listdir(output_dir))) {
        if (f.endswith(S(".h"))) {
            headers.append(f);
        }
    }
    for (const str& fname : headers) {
        str src = _strip_block_comments(_read(py::os::path::join(output_dir, fname)));
        for (const py::re::Match& m : _CLASS_HDR.finditer(src)) {
            int64_t ob = m.end() - 1;
            str norm = py::re::sub(S("\\s+"), S(" "), src.slice(ob, _match_brace(src, ob))).strip();   // whitespace-insensitive body
            defs.setdefault(m.group(1), py::dict<str, str>()).set(fname, norm);
        }
    }
    int64_t n = 0;
    py::list<str> names = py::sorted(py::list<str>::from(defs.keys()));
    for (const str& name : names) {
        const py::dict<str, str>& filemap = defs.at(name);
        // Identical definitions across guarded headers are LEGAL (ODR permits it) and compile fine -- only
        // DIFFERING definitions of the same name are a real violation (the C4743 / mismatched-layout class).
        py::set<str> bodies;
        for (const str& v : filemap.values()) {
            bodies.add(v);
        }
        if (py::len(filemap) > 1 && bodies.size() > 1) {
            py::list<str> fs = py::sorted(py::list<str>::from(filemap.keys()));
            ledger.add(fs[0], Category::RISK,
                       py::percent(S("class '%s' has DIFFERING definitions across %d headers: %s -- one-definition-rule "
                                     "violation (mismatched layout / C4743 class). Keep ONE canonical definition; the rest "
                                     "must #include it, not redefine it differently."),
                                   name, py::len(fs), py::join(S(", "), fs)),
                       0, Severity::HIGH, S("rule"), S("odr-duplicate-def"));
            n += 1;
        }
    }
    return n;
}

py::set<std::tuple<py::str, py::str>> _build_override_pairs(const py::str& output_dir) {
    py::dict<str, py::list<str>> parents;
    py::dict<str, py::set<str>> methods;
    py::list<str> headers;
    for (const str& f : py::sorted(py::os::listdir(output_dir))) {
        if (f.endswith(S(".h"))) {
            headers.append(f);
        }
    }
    for (const str& fname : headers) {
        str src = _strip_block_comments(_read(py::os::path::join(output_dir, fname)));
        for (const py::re::Match& m : _CLASS_HDR.finditer(src)) {
            str name = m.group(1);
            py::list<str> bases;
            std::optional<str> g2 = m.group_opt(2);
            if (g2 && !g2->empty()) {
                for (const str& b : py::split(*g2, S(","))) {
                    std::optional<py::re::Match> bm = py::re::search(S("([A-Za-z_]\\w*)\\s*$"), b.strip());
                    if (bm) {
                        bases.append(bm->group(1));
                    }
                }
            }
            int64_t ob = m.end() - 1;
            str body = src.slice(ob + 1, _match_brace(src, ob));
            py::set<str> mset;
            for (const str& x : py::re::findall(S("(?<![\\w>])([A-Za-z_]\\w*)\\s*\\("), body)) {
                if (!_NOT_METHODS.contains(x)) {
                    mset.add(x);
                }
            }
            mset.discard(name);   // the constructor itself
            parents.setdefault(name, py::list<str>()).extend(bases);
            methods.setdefault(name, py::set<str>()).update(mset);
        }
    }

    // ancestors(c, seen)
    std::function<py::set<str>(const str&, py::set<str>)> ancestors = [&](const str& c, py::set<str> seen) {
        if (parents.contains(c)) {
            for (const str& p : parents.at(c)) {
                if (!seen.contains(p)) {
                    seen.add(p);
                    ancestors(p, seen);
                }
            }
        }
        return seen;
    };

    py::set<std::tuple<str, str>> pairs;
    for (const str& cls : methods.keys()) {
        for (const str& anc : ancestors(cls, py::set<str>())) {
            for (const str& meth : methods.at(cls)) {
                pairs.add(std::make_tuple(anc, meth));
            }
        }
    }
    return pairs;
}

int64_t scan_rules(ledger::Ledger& ledger, const py::str& output_dir, const std::optional<py::str>& platform) {
    _PlatCtx plat(platform);
    plat.override_pairs = _build_override_pairs(output_dir);   // cross-file hierarchy for virtual-call-in-ctor
    int64_t n = 0;
    py::list<str> files;
    for (const str& f : py::sorted(py::os::listdir(output_dir))) {
        if (f.endswith(S(".cpp")) || f.endswith(S(".h"))) {
            files.append(f);
        }
    }
    for (const str& fname : files) {
        str text = _read(py::os::path::join(output_dir, fname));
        std::optional<str> header_text;
        if (fname.endswith(S(".cpp"))) {
            str hp = py::os::path::join(output_dir, fname.slice(0, -4) + ".h");
            if (py::os::path::isfile(hp)) {
                header_text = _read(hp);
            }
        }
        for (const auto& [_rid, _desc, fn] : RULES) {
            for (const auto& [line, cat, sev, rule, msg, snip] : fn(fname, text, header_text, plat)) {
                ledger.add(fname, cat, msg, line, sev, S("rule"), rule, snip);
                n += 1;
            }
        }
    }
    n += _scan_odr(ledger, output_dir);   // cross-file ODR pass (per-file rules can't see it)
    return n;
}

}  // namespace codebreaker::java::rules
