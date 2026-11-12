// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/rules.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/platforms.hpp"
#include "cb/java/strview.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::rules {

namespace {

namespace stdfs = std::filesystem;
using ledger::Category;
using ledger::Severity;
using std::size_t;
using std::string_view;
using Text = std::string_view;

constexpr size_t kNone = string_view::npos;

// ---- small text helpers -------------------------------------------------------------------------------------------

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
bool word_before(Text t, size_t pos) { return pos == 0 || !is_word(t[pos - 1]); }
bool word_after(Text t, size_t end) { return end >= t.size() || !is_word(t[end]); }

size_t skip_blanks(Text t, size_t i) {
    while (i < t.size() && text::is_space(t[i])) ++i;
    return i;
}

size_t skip_word(Text t, size_t i) {
    while (i < t.size() && is_word(t[i])) ++i;
    return i;
}

// The first index at or after `from` holding one of `stops`, or the end of the text.
size_t find_stop(Text t, size_t from, Text stops) {
    const size_t at = t.find_first_of(stops, from);
    return at == kNone ? t.size() : at;
}

// Is `word` present as a whole word wholly inside [from, to)?
bool has_word(Text t, Text word, size_t from, size_t to) {
    for (size_t p = t.find(word, from); p != kNone && p + word.size() <= to; p = t.find(word, p + 1)) {
        if (word_before(t, p) && word_after(t, p + word.size())) return true;
    }
    return false;
}

// `\bdelete\b[^<stops>]*\b<var>\b`: a `delete` followed, before the next stop character, by the variable.
bool deletes_variable(Text region, Text var, Text stops) {
    for (size_t p = region.find("delete"); p != kNone; p = region.find("delete", p + 1)) {
        const size_t after = p + 6;
        if (!word_before(region, p) || !word_after(region, after)) continue;
        if (has_word(region, var, after, find_stop(region, after, stops))) return true;
    }
    return false;
}

std::uint32_t line_at(Text t, size_t pos) {
    return static_cast<std::uint32_t>(std::count(t.begin(), t.begin() + static_cast<std::ptrdiff_t>(std::min(pos, t.size())), '\n')) + 1;
}

Text clip(Text s, size_t columns) { return text::utf8_prefix(s, columns); }
std::string clipped(Text s, size_t columns) { return std::string(clip(s, columns)); }

Text strip(Text s) { return text::trim(s); }

// The text with /* ... */ comments replaced by one space each (an unclosed one stays).
std::string strip_block_comments(Text t) {
    std::string out;
    size_t pos = 0;
    while (true) {
        const size_t open = t.find("/*", pos);
        if (open == kNone) break;
        const size_t close = t.find("*/", open + 2);
        if (close == kNone) break;
        out.append(t.substr(pos, open - pos));
        out += ' ';
        pos = close + 2;
    }
    out.append(t.substr(pos));
    return out;
}

// One line of the file: its number, the raw text, and the code before any `//` ("" when there is none).
struct CodeLine {
    std::uint32_t number = 0;
    Text raw;
    Text code;
};

std::vector<CodeLine> code_lines(Text t) {
    std::vector<CodeLine> out;
    std::uint32_t n = 0;
    for (Text raw : text::split(t, '\n')) {
        const Text code = raw.substr(0, raw.find("//"));
        out.push_back({++n, raw, strip(code).empty() ? Text() : code});
    }
    return out;
}

Finding finding(std::uint32_t line, Category category, Severity severity, std::string rule, std::string message,
                std::string snippet) {
    return {line, category, severity, std::move(rule), std::move(message), std::move(snippet)};
}

// ---- constructor definitions -----------------------------------------------------------------------------------------

// A definition `Cls::Cls( ... ) [: init list] {` found in a .cpp.
struct CtorDef {
    size_t start = 0;       // where `Cls::Cls` begins
    size_t body_open = 0;   // just past the `{`
    Text params;            // between the parentheses
    Text init_list;         // from the `:` to the `{`, or empty
};

std::vector<CtorDef> find_constructors(Text t, Text cls) {
    std::vector<CtorDef> out;
    if (cls.empty()) return out;
    const std::string needle = std::string(cls) + "::" + std::string(cls);
    size_t from = 0;
    while (true) {
        const size_t p = t.find(needle, from);
        if (p == kNone) break;
        from = p + 1;
        if (!word_before(t, p)) continue;
        const size_t lp = skip_blanks(t, p + needle.size());
        if (lp >= t.size() || t[lp] != '(') continue;
        const size_t rp = t.find(')', lp + 1);
        if (rp == kNone) continue;
        const size_t j = skip_blanks(t, rp + 1);
        if (j >= t.size()) continue;
        CtorDef d;
        d.start = p;
        d.params = t.substr(lp + 1, rp - lp - 1);
        if (t[j] == '{') {
            d.body_open = j + 1;
        } else if (t[j] == ':') {
            const size_t brace = t.find('{', j);
            if (brace == kNone) continue;
            d.init_list = t.substr(j, brace - j);
            d.body_open = brace + 1;
        } else {
            continue;
        }
        out.push_back(d);
        from = d.body_open;   // a match is not searched again
    }
    return out;
}

// The text of a constructor's body: up to the brace that balances the opening one (braces are counted as written, not
// aware of literals); an unclosed body ends one before the end of the text.
Text ctor_body(Text t, const CtorDef& d) {
    size_t depth = 1;
    size_t j = d.body_open;
    while (j < t.size() && depth != 0) {
        if (t[j] == '{') ++depth;
        if (t[j] == '}') --depth;
        ++j;
    }
    const size_t end = j > 0 ? j - 1 : 0;
    return end > d.body_open ? t.substr(d.body_open, end - d.body_open) : Text();
}

bool ends_with_c_file(Text name) { return name.ends_with(".cpp"); }
bool is_source_or_header(Text name) { return name.ends_with(".cpp") || name.ends_with(".h"); }

Text file_stem(Text name) { return name.substr(0, name.size() - (name.ends_with(".cpp") ? 4 : name.ends_with(".h") ? 2 : 0)); }

// ---- 1. uninit-array --------------------------------------------------------------------------------------------------

Findings rule_uninit_array(const FileInput& f, const Context&) {
    static constexpr Text kPrim =
        "bool|char|signed char|unsigned char|wchar_t|short|unsigned short|int|unsigned int|unsigned|long long|long|float|"
        "double|byte|jbyte|jboolean|jint|jshort|jlong|jfloat|jdouble";
    // `new <prim>[ ... ]` (with whether a trailing value-init () is present)
    static const Regex new_prim(R"(\bnew\s+()" + std::string(kPrim) + R"()\s*\[[^\]]*\]\s*(\(\s*\))?)");
    // `new <Type> * [ ... ]` -> an array of pointers: uninitialized = garbage vtables
    static const Regex new_ptr(R"(\bnew\s+\w[\w:]*\s*\*\s*\[[^\]]*\]\s*(\(\s*\))?)");
    Findings out;
    if (!is_source_or_header(f.name)) return out;
    for (const CodeLine& l : code_lines(f.text)) {
        if (!contains(l.code, "new ") || !contains(l.code, '[')) continue;
        const std::optional<RegexMatch> m = new_prim.search(l.code);
        if (m && !m->has(2)) {   // no trailing ()
            const std::string t(m->group(1));
            out.push_back(finding(l.number, Category::Risk, t == "bool" ? Severity::High : Severity::Med, "uninit-array",
                                  std::format("new {}[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new {}[N]().", t, t),
                                  clipped(strip(l.raw), 160)));
            continue;
        }
        const std::optional<RegexMatch> mp = new_ptr.search(l.code);
        if (mp && !mp->has(1)) {
            out.push_back(finding(l.number, Category::Risk, Severity::High, "uninit-array",
                                  "new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash. Use new "
                                  "T*[N]() and NULL-check before deref.",
                                  clipped(strip(l.raw), 160)));
        }
    }
    return out;
}

// ---- 2. ctor-stub -----------------------------------------------------------------------------------------------------

Findings rule_ctor_stub(const FileInput& f, const Context&) {
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    const Text cls = file_stem(f.name);
    for (const CtorDef& d : find_constructors(f.text, cls)) {
        const Text params = strip(d.params);
        const Text init_list = strip(d.init_list);   // `: Base(args), member(0), ...`
        const Text body = ctor_body(f.text, d);
        // strip comments + whitespace to see if there is any real statement
        const std::string no_blocks = strip_block_comments(body);
        bool has_statement = false;
        for (Text ln : text::split(no_blocks, '\n')) {
            if (!strip(ln.substr(0, ln.find("//"))).empty()) has_statement = true;
        }
        const bool has_delegation_marker = contains(body, "constructor delegation") || contains(body, "was: this(");
        // an empty body WITH an init list (base/member init) is valid C++, not a stub
        if (!params.empty() && init_list.empty() && (!has_statement || has_delegation_marker)) {
            out.push_back(finding(line_at(f.text, d.start), Category::Stubbed, Severity::High, "ctor-stub",
                                  std::format("constructor {}({}) has an empty/delegation-only body -- field init and static REGISTRATION "
                                              "may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). "
                                              "Replicate the real ctor body.",
                                              cls, clip(params, 60)),
                                  clipped(std::format("{}({}) {{...}}", cls, params), 160)));
        }
    }
    return out;
}

// ---- 3. rule-of-three -------------------------------------------------------------------------------------------------

// `~Cls\s*\(`
bool declares_destructor(Text h, Text cls) {
    const std::string needle = "~" + std::string(cls);
    for (size_t p = h.find(needle); p != kNone; p = h.find(needle, p + 1)) {
        const size_t i = skip_blanks(h, p + needle.size());
        if (i < h.size() && h[i] == '(') return true;
    }
    return false;
}

// `<prefix>\s*const\s+Cls\s*&` at `i`: the rest of a copy declaration.
bool const_ref_to(Text h, size_t i, Text cls) {
    i = skip_blanks(h, i);
    if (!h.substr(i).starts_with("const")) return false;
    i += 5;
    const size_t after_ws = skip_blanks(h, i);
    if (after_ws == i) return false;
    if (!h.substr(after_ws).starts_with(cls)) return false;
    i = skip_blanks(h, after_ws + cls.size());
    return i < h.size() && h[i] == '&';
}

// `\bCls\s*\(\s*const\s+Cls\s*&`
bool declares_copy_ctor(Text h, Text cls) {
    for (size_t p = h.find(cls); p != kNone; p = h.find(cls, p + 1)) {
        if (!word_before(h, p)) continue;
        const size_t lp = skip_blanks(h, p + cls.size());
        if (lp < h.size() && h[lp] == '(' && const_ref_to(h, lp + 1, cls)) return true;
    }
    return false;
}

// `operator\s*=\s*\(\s*const\s+Cls\s*&`
bool declares_copy_assignment(Text h, Text cls) {
    for (size_t p = h.find("operator"); p != kNone; p = h.find("operator", p + 1)) {
        size_t i = skip_blanks(h, p + 8);
        if (i >= h.size() || h[i] != '=') continue;
        i = skip_blanks(h, i + 1);
        if (i < h.size() && h[i] == '(' && const_ref_to(h, i + 1, cls)) return true;
    }
    return false;
}

Findings rule_rule_of_three(const FileInput& f, const Context&) {
    static const Regex assigns_new(R"(=\s*new\s+\w)");
    static const Regex new_array(R"(\bnew\s+\w[\w:]*\s*\[)");
    Findings out;
    if (!ends_with_c_file(f.name) || !f.header) return out;
    const Text cls = file_stem(f.name);
    bool owns_heap = false;
    std::uint32_t ctor_line = 0;
    for (const CtorDef& d : find_constructors(f.text, cls)) {
        const Text body = ctor_body(f.text, d);
        if (assigns_new.test(body) || new_array.test(body)) {
            owns_heap = true;
            ctor_line = line_at(f.text, d.start);
            break;
        }
    }
    if (!owns_heap) return out;
    const std::string h = strip_block_comments(*f.header);
    const bool has_dtor = declares_destructor(h, cls);
    const bool has_copy_ctor = declares_copy_ctor(h, cls);
    const bool has_copy_assign = declares_copy_assignment(h, cls);
    const bool copy_disabled = (contains(h, "private:") && has_copy_ctor) || contains(h, "DISALLOW_COPY");
    if (!has_dtor) {
        out.push_back(finding(ctor_line, Category::Risk, Severity::High, "rule-of-three",
                              std::format("{} allocates with `new` in its constructor but declares NO destructor -> leak (Java GC'd this; "
                                          "C++ does not).",
                                          cls),
                              ""));
    } else if (!(has_copy_ctor && has_copy_assign) && !copy_disabled) {
        out.push_back(finding(ctor_line, Category::Risk, Severity::Med, "rule-of-three",
                              std::format("{} owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy "
                                          "double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or "
                                          "make copying private.",
                                          cls),
                              ""));
    }
    return out;
}

// ---- 4. value-copy-owner ------------------------------------------------------------------------------------------------

// The text with `//` comments and `/* */` comments blanked to spaces (length- and newline-preserving), so a type name
// inside a comment like "// glDrawArrays (Tessellator path)" is NOT mistaken for a by-value parameter.
std::string blank_comments_in_place(Text t) {
    std::string s(t);
    for (size_t p = s.find("//"); p != kNone; p = s.find("//", p)) {
        const size_t end = find_stop(s, p, "\n");
        std::fill(s.begin() + static_cast<std::ptrdiff_t>(p), s.begin() + static_cast<std::ptrdiff_t>(end), ' ');
        p = end;
    }
    for (size_t p = s.find("/*"); p != kNone; p = s.find("/*", p)) {
        const size_t close = s.find("*/", p + 2);
        if (close == kNone) break;
        for (size_t i = p; i < close + 2; ++i) {
            if (s[i] != '\n') s[i] = ' ';
        }
        p = close + 2;
    }
    return s;
}

Findings rule_value_copy_owner(const FileInput& f, const Context& ctx) {
    Findings out;
    if (!is_source_or_header(f.name)) return out;
    const std::string src = blank_comments_in_place(f.text);
    const std::vector<Text> lines = text::split(f.text, '\n');
    auto line_text = [&](std::uint32_t line) { return line >= 1 && line <= lines.size() ? lines[line - 1] : Text(); };
    for (const std::string& t : ctx.heavy_types) {
        const bool high = std::find(ctx.heavy_types_high.begin(), ctx.heavy_types_high.end(), t) != ctx.heavy_types_high.end();
        const Severity sev = high ? Severity::High : Severity::Med;
        // by-value param: `(Type name` / `, Type name` -- but NOT Type& / Type*
        for (size_t i = 0; i < src.size();) {
            if (src[i] != '(' && src[i] != ',') {
                ++i;
                continue;
            }
            const size_t j = skip_blanks(src, i + 1);
            const Text rest = Text(src).substr(j);
            size_t next = i + 1;
            if (rest.starts_with(t)) {
                const size_t k = j + t.size();
                const size_t name_at = skip_blanks(src, k);
                const size_t name_end = skip_word(src, name_at);
                const size_t after = skip_blanks(src, name_end);
                if (name_at > k && name_end > name_at && after < src.size() && (src[after] == ',' || src[after] == ')')) {
                    const std::uint32_t line = line_at(f.text, i);
                    out.push_back(finding(line, Category::Guessed, sev, "value-copy-owner",
                                          std::format("{} passed BY VALUE -- copies its whole buffer every call. Take it by reference: "
                                                      "const {}& / {}&.",
                                                      t, t, t),
                                          clipped(strip(line_text(line)), 160)));
                    next = name_end;   // the terminator is the next parameter's introducer: it is not consumed
                }
            }
            i = next;
        }
        // by-value return: a line that starts `Type name(`
        for (size_t p = src.find(t); p != kNone; p = src.find(t, p + 1)) {
            size_t b = p;   // first on its line: only blanks back to the line start
            while (b > 0 && text::is_space(src[b - 1]) && src[b - 1] != '\n') --b;
            if (!(b == 0 || src[b - 1] == '\n')) continue;
            const size_t k = p + t.size();
            const size_t name_at = skip_blanks(src, k);
            if (name_at == k || name_at >= src.size() || !is_word(src[name_at])) continue;
            size_t name_end = name_at;
            while (name_end < src.size() && (is_word(src[name_end]) || src[name_end] == ':')) ++name_end;
            const size_t paren = skip_blanks(src, name_end);
            if (paren >= src.size() || src[paren] != '(') continue;
            const std::uint32_t line = line_at(f.text, p);
            out.push_back(finding(line, Category::Guessed, sev, "value-copy-owner",
                                  std::format("{} returned BY VALUE -- returns a full copy. getInstance() must return a reference.", t),
                                  clipped(strip(line_text(line)), 160)));
        }
    }
    return out;
}

// ---- 5. virtual-call-in-ctor --------------------------------------------------------------------------------------------

// A call `name(` or `this->name(`: bare, not as a member of something else (`obj->name(`, `obj.name(`, `Class::name(`).
bool calls_own_method(Text body, Text name) {
    for (size_t p = body.find(name); p != kNone; p = body.find(name, p + 1)) {
        const size_t after = skip_blanks(body, p + name.size());
        if (after >= body.size() || body[after] != '(') continue;
        if (p == 0 || !(is_word(body[p - 1]) || body[p - 1] == '.' || body[p - 1] == '>' || body[p - 1] == ':')) return true;   // a bare call
        // `this->name(`: this, then `->`, then the name
        size_t q = p;
        while (q > 0 && text::is_space(body[q - 1])) --q;
        if (q >= 2 && body[q - 1] == '>' && body[q - 2] == '-') {
            q -= 2;
            while (q > 0 && text::is_space(body[q - 1])) --q;
            if (q >= 4 && body.substr(q - 4, 4) == "this" && word_before(body, q - 4)) return true;
        }
    }
    return false;
}

Findings rule_virtual_call_in_ctor(const FileInput& f, const Context& ctx) {
    // `virtual ... name(`: the first name followed by a parenthesis after the keyword
    static const Regex virtual_decl(R"(\bvirtual\b[^;{]*?\b(\w+)\s*\()");
    Findings out;
    if (!ends_with_c_file(f.name) || !f.header) return out;
    const std::string cls(file_stem(f.name));
    const std::string h = strip_block_comments(*f.header);
    std::set<std::string> virt;
    virtual_decl.for_each_match(h, [&](const RegexMatch& m) {
        virt.emplace(m.group(1));
        return true;
    });
    virt.erase(cls);   // not the constructor itself
    if (virt.empty()) return out;
    for (const CtorDef& d : find_constructors(f.text, cls)) {
        const std::string body = strip_block_comments(ctor_body(f.text, d));
        for (const std::string& name : virt) {   // (sorted: the findings of one constructor come in one order)
            if (!calls_own_method(body, name)) continue;
            // PRECISION: the base-vs-override divergence only bites if a DESCENDANT actually overrides this method. When the
            // whole-tree hierarchy is known, skip calls no subclass overrides -- they dispatch identically in C++ and Java.
            if (ctx.override_pairs && !ctx.override_pairs->contains({cls, name})) continue;
            out.push_back(finding(line_at(f.text, d.start), Category::Risk, Severity::Med, "virtual-call-in-ctor",
                                  std::format("{}'s constructor calls virtual {}() -- C++ dispatches to {}'s own version, NOT a subclass "
                                              "override (Java would call the override). Wrong data may be recorded (the Block "
                                              "light-opacity bug). Set the field directly or finish init post-ctor.",
                                              cls, name, cls),
                                  ""));
        }
    }
    return out;
}

// ---- 6. static-init-order -----------------------------------------------------------------------------------------------

Findings rule_static_init_order(const FileInput& f, const Context&) {
    // A static field DEFINITION with an initializer: `<type> <Class>::s_<name> = <init>;`
    static const Regex static_def(R"(^[A-Za-z_][\w:<>*&,\s]*?\b(\w+::s_\w+)\s*=\s*(.+);\s*$)", RegexFlags::Multiline);
    // constructing a user class (not a safe runtime value type) -> its ctor may read other statics
    static const Regex new_user(R"(\bnew\s+(?!JavaString\b|JArray\b|JList\b|JObject\b|JCat\b)[A-Z]\w*)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    static_def.for_each_match(f.text, [&](const RegexMatch& m) {
        const Text init = m.group(2);
        if (contains(init, "::s_") || new_user.test(init)) {
            out.push_back(finding(line_at(f.text, m.start()), Category::Risk, Severity::Med, "static-init-order",
                                  std::format("eager static init `{} = ...` runs in UNDEFINED cross-translation-unit order (it constructs a "
                                              "class or reads another class's statics) -- the static-init-order crash class. Defer it "
                                              "into initStatics() (run in dependency order by j2x_initAll).",
                                              m.group(1)),
                                  clipped(strip(m.whole()), 160)));
        }
        return true;
    });
    return out;
}

// ---- 7. endianness ------------------------------------------------------------------------------------------------------

Findings rule_endianness_raw_cast(const FileInput& f, const Context& ctx) {
    // Files that read/write the on-disk binary formats (NBT, save/region, streams, compressed IO).
    static const Regex binary_io_file(R"((NBT|Stream|Save|Region|Compressed|Chunk.*(Loader|Provider)))", RegexFlags::IgnoreCase);
    // A raw reinterpret over a >1-byte scalar, or memcpy -- byte-swaps on big-endian PowerPC.
    static const Regex raw_cast(
        R"(reinterpret_cast\s*<|memcpy\s*\(|\*\s*\(\s*(?:int|short|long|long long|float|double|unsigned)\b[\w ]*\*\s*\))");
    Findings out;
    if (!ctx.big_endian) return out;
    if (!ends_with_c_file(f.name) || !binary_io_file.test(f.name)) return out;
    for (const CodeLine& l : code_lines(f.text)) {
        if (l.code.empty() || !raw_cast.test(l.code)) continue;
        out.push_back(finding(l.number, Category::Warning, Severity::Med, "endianness",
                              std::format("raw multi-byte cast/memcpy in a binary-serialization file -- on big-endian {} this byte-swaps "
                                          "and corrupts saves/protocol. Read/write byte-at-a-time (MSB-first), then bit-cast the "
                                          "assembled int/long to float/double.",
                                          ctx.platform_label),
                              clipped(strip(l.raw), 160)));
    }
    return out;
}

// ---- 8. null-deref-getter / null-deref-field ----------------------------------------------------------------------------

Findings rule_null_deref_getter(const FileInput& f, const Context& ctx) {
    static const Regex getter_deref(R"((\w+)\s*\([^)]*\)\s*->\s*\w+)");
    Findings out;
    if (!is_source_or_header(f.name)) return out;
    for (const CodeLine& l : code_lines(f.text)) {
        if (l.code.empty() || !contains(l.code, "->")) continue;
        getter_deref.for_each_match(l.code, [&](const RegexMatch& m) {
            if (ctx.nullable_getters.contains(m.group(1))) {
                out.push_back(finding(l.number, Category::Risk, Severity::High, "null-deref-getter",
                                      std::format("{}() can return NULL -- dereferencing without a null check is a {}. Capture into a local "
                                                  "and check before use.",
                                                  m.group(1), ctx.null_deref_msg),
                                      clipped(strip(l.raw), 160)));
            }
            return true;
        });
    }
    return out;
}

// `\b<field>\s*[!=]= (?:NULL|nullptr|0)\b` -- a null comparison of the field (the single space is the converter's form).
bool compares_to_null(Text code, Text field) {
    for (size_t p = code.find(field); p != kNone; p = code.find(field, p + 1)) {
        if (!word_before(code, p)) continue;
        size_t i = skip_blanks(code, p + field.size());
        if (i + 2 >= code.size() || (code[i] != '!' && code[i] != '=') || code[i + 1] != '=' || code[i + 2] != ' ') continue;
        i += 3;
        for (Text lit : {"NULL", "nullptr", "0"}) {
            if (code.substr(i).starts_with(lit) && word_after(code, i + lit.size())) return true;
        }
    }
    return false;
}

Findings rule_null_deref_field(const FileInput& f, const Context& ctx) {
    static const Regex field_deref(R"((?:this->)?(\w+)->)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    const std::vector<Text> lines = text::split(f.text, '\n');
    for (size_t i = 0; i < lines.size(); ++i) {
        const Text code = lines[i].substr(0, lines[i].find("//"));
        if (strip(code).empty() || !contains(code, "->")) continue;
        field_deref.for_each_match(code, [&](const RegexMatch& m) {
            const Text field = m.group(1);
            if (!ctx.nullable_fields.contains(field)) return true;
            if (compares_to_null(code, field)) return true;
            bool guarded = false;
            for (size_t j = i >= 5 ? i - 5 : 0; j < i; ++j) {
                if (compares_to_null(lines[j].substr(0, lines[j].find("//")), field)) {
                    guarded = true;
                    break;
                }
            }
            if (guarded) return true;
            out.push_back(finding(static_cast<std::uint32_t>(i + 1), Category::Risk, Severity::Med, "null-deref-field",
                                  std::format("{} can be NULL -- dereferencing without a null check is a {}. Add `if ({} != NULL)` guard.",
                                              field, ctx.null_deref_msg, field),
                                  clipped(strip(lines[i]), 160)));
            return false;   // one finding per line
        });
    }
    return out;
}

// ---- 9. loops: hotpath-alloc, infinite-loop, leak-loop-reassign ----------------------------------------------------------

struct Span {
    size_t begin = 0;
    size_t end = 0;
};

// Given the keyword of a `for`/`while`, its body: the balanced `{...}` if the next non-blank after the header `)` is
// `{`, else up to the next `;`. nullopt when the header has no parenthesis at all.
std::optional<Span> loop_body_span(Text t, size_t keyword_start) {
    const size_t keyword_len = t.substr(keyword_start).starts_with("while") ? 5 : 3;
    const size_t p = t.find('(', keyword_start + keyword_len);
    if (p == kNone) return std::nullopt;
    size_t depth = 0;
    size_t j = p;
    for (; j < t.size(); ++j) {   // balance the header parens
        if (t[j] == '(') ++depth;
        if (t[j] == ')' && --depth == 0) break;
    }
    size_t k = j + 1;
    while (k < t.size() && (t[k] == ' ' || t[k] == '\t' || t[k] == '\r' || t[k] == '\n')) ++k;
    if (k < t.size() && t[k] == '{') {
        depth = 0;
        for (size_t u = k; u < t.size(); ++u) {
            if (t[u] == '{') ++depth;
            if (t[u] == '}' && --depth == 0) return Span{k, u};
        }
        return Span{k, t.size()};
    }
    const size_t semi = k < t.size() ? t.find(';', k) : kNone;
    return Span{k, semi == kNone ? t.size() : semi};
}

Text span_text(Text t, const Span& s) { return s.begin < s.end && s.begin < t.size() ? t.substr(s.begin, std::min(s.end, t.size()) - s.begin) : Text(); }

const Regex& for_or_while() {
    static const Regex re(R"(\b(?:for|while)\b)");
    return re;
}

Findings rule_hotpath_alloc(const FileInput& f, const Context& ctx) {
    // hot per-frame / per-chunk / per-entity methods where a big loop runs on the 360's tiny budget
    static const Regex hotpath(R"(\b(generate|populate|render\w*|doRender\w*|tick|onUpdate|updateEntity|onLivingUpdate|)"
                               R"(updateRenderer\w*|getBlock\w*|relightBlock|updateLighting|func_\w+)\s*\()");
    static const Regex new_object(R"(\bnew\s+([A-Z]\w*)\s*[({])");   // heap object (not new T[]) construction
    static const Regex definition(R"(\b([A-Za-z_]\w*)\s*::\s*(~?)\s*([A-Za-z_]\w*)\s*\()");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    std::unordered_set<std::uint32_t> seen;
    for_or_while().for_each_match(f.text, [&](const RegexMatch& lm) {
        const std::optional<Span> span = loop_body_span(f.text, lm.start());
        if (!span) return true;
        const Text body = span_text(f.text, *span);
        // is this loop lexically inside a hot-path method? (cheap: scan back to the enclosing signature)
        const size_t head_from = lm.start() > 4000 ? lm.start() - 4000 : 0;
        const Text head = Text(f.text).substr(head_from, lm.start() - head_from);
        const size_t blank = head.rfind("\n\n");
        const bool hot = hotpath.test(blank == kNone ? head : head.substr(blank + 2));
        // One-time init: a `new` in a loop inside a CONSTRUCTOR runs once per object (building fixed structures -- the chunk
        // grid, the entity roster), NOT once per frame. That is not an allocation hot path, so don't flag it. The nearest
        // `X::Y(` before the loop is the enclosing definition; `X::X(` is a constructor. Per-frame churn lives in
        // tick/render/collision methods (still flagged).
        std::optional<std::string> nearest_class, nearest_tilde, nearest_name;
        definition.for_each_match(head, [&](const RegexMatch& sm) {
            nearest_class = std::string(sm.group(1));
            nearest_tilde = std::string(sm.group(2));
            nearest_name = std::string(sm.group(3));
            return true;
        });
        if (nearest_class && nearest_tilde->empty() && *nearest_class == *nearest_name) return true;
        new_object.for_each_match(body, [&](const RegexMatch& nm) {
            const std::uint32_t line = line_at(f.text, span->begin + nm.start());
            if (!seen.insert(line).second) return true;
            const Text from = Text(f.text).substr(span->begin + nm.start(), 120);
            out.push_back(finding(line, Category::Risk, hot ? Severity::High : Severity::Med, "hotpath-alloc",
                                  std::format("heap `new {}` inside a loop allocates once PER ITERATION -- on {} this is an allocation storm / "
                                              "heap-exhaustion hang when the loop is large (Java's GC hid it). Hoist it out of the loop, pool "
                                              "it, or stack-allocate it.",
                                              nm.group(1), ctx.platform_label),
                                  std::string(strip(from.substr(0, from.find('\n'))))));
            return true;
        });
        return true;
    });
    return out;
}

Findings rule_infinite_loop(const FileInput& f, const Context& ctx) {
    static const Regex unbounded(R"(\b(while\s*\(\s*(?:true|1)\s*\)|for\s*\(\s*;\s*;\s*\)))");
    // A loop that Sleeps / waits / yields is an INTENTIONAL service/daemon loop, not a hang -- don't flag it.
    static const Regex exits(R"(\b(break|return|goto|throw|exit|Sleep|WaitFor\w+|SwitchToThread|MsgWaitFor\w+)\b)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    unbounded.for_each_match(f.text, [&](const RegexMatch& lm) {
        const std::optional<Span> span = loop_body_span(f.text, lm.start());
        if (!span) return true;
        if (!exits.test(span_text(f.text, *span))) {
            out.push_back(finding(line_at(f.text, lm.start()), Category::Risk, Severity::Med, "infinite-loop",
                                  std::format("unbounded loop (`{}`) with no break/return/goto in its body -- wedges the thread on {} (no "
                                              "watchdog). Verify it has an exit; a mistranslated labeled break lands here.",
                                              lm.group(1), ctx.platform_label),
                                  std::string(lm.whole())));
        }
        return true;
    });
    return out;
}

Findings rule_leak_loop_reassign(const FileInput& f, const Context&) {
    static const Regex reassign(R"(\b(\w+)\s*=\s*new\s+[A-Z]\w*\s*[({])");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    for_or_while().for_each_match(f.text, [&](const RegexMatch& lm) {
        const std::optional<Span> span = loop_body_span(f.text, lm.start());
        if (!span) return true;
        const Text body = span_text(f.text, *span);
        reassign.for_each_match(body, [&](const RegexMatch& am) {
            const Text var = am.group(1);
            const size_t near_from = am.start() > 200 ? am.start() - 200 : 0;
            if (deletes_variable(body.substr(near_from, am.start() - near_from), var, ";")) return true;   // deleted just before
            const size_t before_from = span->begin > 500 ? span->begin - 500 : 0;
            if (deletes_variable(Text(f.text).substr(before_from, span->begin - before_from), var, ";{}")) return true;   // freed BEFORE the loop
            // the reassign is immediately followed by return/break: not a per-iteration leak
            const Text tail = body.substr(am.end(), 60);
            const size_t stop = tail.find_first_of(";{}");
            if (stop != kNone && tail[stop] == ';') {
                const Text after = tail.substr(skip_blanks(tail, stop + 1));
                for (Text kw : {"return", "break"}) {
                    if (after.starts_with(kw) && word_after(after, kw.size())) return true;
                }
            }
            out.push_back(finding(line_at(f.text, span->begin + am.start()), Category::Risk, Severity::Med, "leak-loop-reassign",
                                  std::format("`{} = new ...` inside a loop with no `delete {}` first -- each iteration leaks the previous "
                                              "object (Java GC'd it). Add `delete {};` before the reassign and once after the loop.",
                                              var, var, var),
                                  clipped(am.whole(), 120)));
            return false;   // one finding per loop is enough
        });
        return true;
    });
    return out;
}

// ---- 10. side-effect-in-branch --------------------------------------------------------------------------------------------

Findings rule_side_effect_in_branch(const FileInput& f, const Context&) {
    // An impure statement: a call, a `new`, or an assignment to field/global/native state. Broad on purpose -- the RenderPig bug
    // was a plain method CALL (`loadTexture`) in a collapsed guard, so calls must count. The call pattern EXCLUDES control keywords
    // (`if (x) ;`, `while (y) ;`): a guard body reduced to `if (cond) ;` must not read as an impure statement.
    static const Regex impure(R"(\b(?!(?:if|for|while|switch|return|sizeof|catch|do)\b)\w+\s*\([^)]*\)\s*;)"
                              R"(|\bnew\s+[A-Za-z_]|->\s*\w+\s*=(?!=)|::\s*\w+\s*=(?!=)|\bthis->\w+\s*=(?!=))");
    // NOISE FILTER, keyed on the two real false-positive shapes: (1) the guard is a FAILURE / NULL / edge check -- the branch is an
    // error/early-out path where a side effect + return is expected, not the RenderPig shape; (2) the body's only impure statements
    // are CLEANUP -- Release/delete/free/close/Unlock -- ordinary teardown. The RenderPig collapse only threatens a *plain
    // predicate* guard (`if (cond)`, `if (isSolidTile(...))`) whose branch does constructive work; those have no comparison operator.
    static const Regex err_guard(R"(\bFAILED\s*\(|\bSUCCEEDED\s*\(|[=!<>]=|!\s*[\w(]|(?<![-\w<>])[<>](?![<>=]))");
    static const Regex cleanup_call(R"(\b(?:Release|delete|free|close|Unlock|EndScene|Reset|clear|dispose|destroy)\b)", RegexFlags::IgnoreCase);
    static const Regex if_open(R"(\bif\s*\()");
    static const Regex return_statement(R"(\breturn\b[^;]*;)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    const Text t = f.text;
    if_open.for_each_match(t, [&](const RegexMatch& m) {
        const size_t p = m.end() - 1;   // balance the condition parens
        size_t depth = 0;
        size_t j = p;
        for (; j < t.size(); ++j) {
            if (t[j] == '(') ++depth;
            if (t[j] == ')' && --depth == 0) break;
        }
        size_t k = j + 1;
        while (k < t.size() && (t[k] == ' ' || t[k] == '\t' || t[k] == '\r' || t[k] == '\n')) ++k;
        if (k >= t.size() || t[k] != '{') return true;   // only braced blocks (single-statement is low-risk)
        depth = 0;
        size_t u = k;
        for (; u < t.size(); ++u) {
            if (t[u] == '{') ++depth;
            if (t[u] == '}' && --depth == 0) break;
        }
        const Text cond = t.substr(m.end(), std::min(j, t.size()) - m.end());
        const Text body = t.substr(k + 1, std::min(u, t.size()) - (k + 1));
        if (err_guard.test(cond)) return true;   // (1) failure / null / edge guard -> the expected path
        // (2) a body whose impure work is ONLY cleanup (every impure statement matches a teardown verb)
        bool any_impure = false;
        bool all_cleanup = true;
        for (Text part : text::split(body, ';')) {
            if (!impure.test(std::string(part) + ";")) continue;
            any_impure = true;
            if (!cleanup_call.test(part)) all_cleanup = false;
        }
        if (any_impure && all_cleanup) return true;
        // (3) a call/construction that IS the returned value (`return f(x);`) is not a side-effecting STATEMENT -- it just computes
        // the result. Blank the return expressions before the impure test so only a real separate side effect still flags.
        const std::string body_side = return_statement.replace_all(body, " ");
        if (contains(body, "return") && impure.test(body_side) && !contains(body, "J2X-REVIEW")) {
            std::string header(t.substr(m.start(), k - m.start()));
            std::replace(header.begin(), header.end(), '\n', ' ');
            out.push_back(finding(line_at(t, m.start()), Category::Risk, Severity::Low, "side-effect-in-branch",
                                  "J2X-REVIEW[side-effect-in-branch]: a guarded branch runs an impure statement AND returns -- confirm the "
                                  "side effect still executes ONLY under this guard (it must never be collapsed/hoisted into an "
                                  "unconditional prelude; the RenderPig class).",
                                  clipped(header, 120)));
        }
        return true;
    });
    return out;
}

// ---- 11. debug-bypass -------------------------------------------------------------------------------------------------------

Findings rule_debug_bypass(const FileInput& f, const Context&) {
    static const Regex bypass(R"(\b(OutputDebugString[AW]?|printf|fprintf|std::(?:cout|cerr)|puts)\s*[\(<])");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    if (contains(f.name, "Debug.cpp")) return out;   // the backend itself legitimately uses printf
    bypass.for_each_match(f.text, [&](const RegexMatch& m) {
        const size_t ls = m.start() == 0 ? kNone : f.text.rfind('\n', m.start() - 1);
        const size_t line_start = ls == kNone ? 0 : ls + 1;
        const size_t le = f.text.find('\n', m.start());
        const Text line_text = f.text.substr(line_start, (le == kNone ? f.text.size() : le) - line_start);
        // skip lines already routed/annotated
        if (contains(line_text, "DBG_") || contains(line_text, "J2X-REVIEW") || contains(line_text, "Debug_Printf")) return true;
        out.push_back(finding(line_at(f.text, m.start()), Category::Risk, Severity::Low, "debug-bypass",
                              "J2X-REVIEW[debug-bypass]: raw debug output not routed through Debug.h -- convert to a DBG_* macro so the "
                              "master DEBUG_ENABLED switch governs it (else it stays hot in retail and spams the console).",
                              clipped(strip(line_text), 120)));
        return true;
    });
    return out;
}

// ---- 12. function-scoped leak detectors -------------------------------------------------------------------------------------

struct Function {
    Text signature;
    Text body;
    size_t body_start = 0;
};

// Each top-level function DEFINITION (brace-matched, string/char-literal aware).
std::vector<Function> iterate_functions(Text t) {
    static const Regex func_def(R"(^([A-Za-z_][\w:<>,*&\s]*?\b[\w:~]+)\s*\(([^;{}]*)\)\s*(?:const\s*)?\{)", RegexFlags::Multiline);
    std::vector<Function> out;
    func_def.for_each_match(t, [&](const RegexMatch& m) {
        const size_t open = m.end() - 1;
        size_t depth = 0;
        size_t j = open;
        while (j < t.size()) {
            const char c = t[j];
            if (c == '"' || c == '\'') {
                ++j;
                while (j < t.size()) {
                    if (t[j] == '\\') {
                        j += 2;
                        continue;
                    }
                    if (t[j] == c) break;
                    ++j;
                }
            } else if (c == '{') {
                ++depth;
            } else if (c == '}') {
                if (--depth == 0) break;
            }
            ++j;
        }
        const size_t end = std::min(j, t.size());
        out.push_back({strip(m.group(1)), t.substr(open + 1, end - (open + 1)), open + 1});
        return true;
    });
    return out;
}

// `(?!\s*(?:->|\.|\(|\[))` at `i`: the variable is used as a VALUE, not as a receiver (`v->m()`) or indexed (`v[i]`).
bool used_as_value(Text t, size_t i) {
    i = skip_blanks(t, i);
    const Text rest = t.substr(i);
    return !(rest.starts_with("->") || rest.starts_with(".") || rest.starts_with("(") || rest.starts_with("["));
}

// Does the variable escape the rest of its function: freed, returned, stored, or added to a container?
bool variable_escapes(Text rest, Text var) {
    if (deletes_variable(rest, var, ";{}")) return true;   // freed
    // returned (the value itself)
    for (size_t p = rest.find("return"); p != kNone; p = rest.find("return", p + 1)) {
        if (!word_before(rest, p) || !word_after(rest, p + 6)) continue;
        const size_t seg_end = find_stop(rest, p + 6, ";{}");
        for (size_t v = rest.find(var, p + 6); v != kNone && v + var.size() <= seg_end; v = rest.find(var, v + 1)) {
            if (word_before(rest, v) && word_after(rest, v + var.size()) && used_as_value(rest, v + var.size())) return true;
        }
    }
    // stored: x = v / field = v
    for (size_t p = rest.find('='); p != kNone; p = rest.find('=', p + 1)) {
        if (p > 0 && std::string_view("=<>!").find(rest[p - 1]) != kNone) continue;
        const size_t v = skip_blanks(rest, p + 1);
        if (rest.substr(v).starts_with(var) && word_after(rest, v + var.size()) && used_as_value(rest, v + var.size())) return true;
    }
    // added to a container/registry
    static const Regex registers(R"((?:\.|->)(?:add\w*|push_back|push_front|put|insert|append|offer|set\w*|register\w*)\s*\()");
    bool added = false;
    registers.for_each_match(rest, [&](const RegexMatch& m) {
        if (has_word(rest, var, m.end(), find_stop(rest, m.end(), ";{}"))) {
            added = true;
            return false;
        }
        return true;
    });
    return added;
}

Findings rule_leak_temp_new(const FileInput& f, const Context&) {
    static const Regex local_new(R"(\b[A-Za-z_][\w:]*\s*\*\s*(\w+)\s*=\s*new\b)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    for (const Function& fn : iterate_functions(f.text)) {
        std::optional<Finding> found;
        local_new.for_each_match(fn.body, [&](const RegexMatch& am) {
            const Text var = am.group(1);
            if (variable_escapes(fn.body.substr(am.end()), var)) return true;
            found = finding(line_at(f.text, fn.body_start + am.start()), Category::Risk, Severity::Med, "leak-temp-new",
                            std::format("local `{} = new ...` is never delete'd, returned, stored, or added to a container in this function "
                                        "-> leaks on the GC-less heap (Java GC'd it). `delete {};` on every exit path.",
                                        var, var),
                            clipped(am.whole(), 120));
            return false;   // one finding per function is enough
        });
        if (found) out.push_back(std::move(*found));
    }
    return out;
}

// `\bphrase\b` for a phrase that starts and ends with a word character.
bool contains_phrase(Text t, Text phrase) {
    for (size_t p = t.find(phrase); p != kNone; p = t.find(phrase, p + 1)) {
        if (word_before(t, p) && word_after(t, p + phrase.size())) return true;
    }
    return false;
}

Findings rule_leak_dtor_member(const FileInput& f, const Context&) {
    static const Regex local_pointer(R"(\b[A-Za-z_][\w:<>]*\s*\*+\s*(\w+)\s*(?:=|;))");
    static const Regex assigned_new(R"((?:this->)?(\w+)\s*=\s*new\b)");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    const std::string cls(file_stem(f.name));
    const std::string dtor_phrase = cls + "::~" + cls;
    const std::string ctor_phrase = cls + "::" + cls;
    std::map<std::string, std::uint32_t> ctor_new;   // member -> the line of its first `new`, sorted by member
    std::optional<std::string> dtor_body;
    for (const Function& fn : iterate_functions(f.text)) {
        if (contains_phrase(fn.signature, dtor_phrase)) {
            dtor_body = dtor_body.value_or("") + std::string(fn.body);
        } else if (contains_phrase(fn.signature, ctor_phrase)) {
            // the constructor's own locals (`T* v = NULL;` first, `v = new ...` later: the back end's ordered-operand temporaries
            // j2xoN, a decompiler local) are not members
            std::set<std::string, std::less<>> locals;
            local_pointer.for_each_match(fn.body, [&](const RegexMatch& m) {
                locals.emplace(m.group(1));
                return true;
            });
            assigned_new.for_each_match(fn.body, [&](const RegexMatch& m) {
                if (locals.contains(m.group(1)) && !m.whole().starts_with("this->")) return true;
                ctor_new.emplace(std::string(m.group(1)), line_at(f.text, fn.body_start + m.start()));
                return true;
            });
        }
    }
    if (ctor_new.empty() || !dtor_body) return out;   // no dtor at all -> rule-of-three already reports it
    for (const auto& [member, line] : ctor_new) {
        if (deletes_variable(*dtor_body, member, ";{}")) continue;
        out.push_back(finding(line, Category::Risk, Severity::High, "leak-dtor-member",
                              std::format("member '{}' is `new`'d in the constructor but never `delete`d in ~{}() -> leaks for the object's "
                                          "whole lifetime on the GC-less heap. Add `delete {};` to the destructor.",
                                          member, cls, member),
                              member + " = new ..."));
    }
    return out;
}

// ---- 13. containers -----------------------------------------------------------------------------------------------------------

Findings rule_ptr_container_compare(const FileInput& f, const Context&) {
    static const Regex pointer_set(R"(\b(?:std::set|std::unordered_set|JSet)\s*<\s*([\w:]+)\s*\*\s*>)");
    static const Regex pointer_map(R"(\b(?:std::map|std::unordered_map|JMap)\s*<\s*([\w:]+)\s*\*\s*,)");
    static const std::unordered_set<std::string_view> primitives = {
        "int",    "char",    "short",    "long",     "float",    "double",   "bool",     "unsigned", "size_t",
        "void",   "int8_t",  "uint8_t",  "int16_t",  "uint16_t", "int32_t",  "uint32_t", "int64_t",  "uint64_t", "wchar_t"};
    Findings out;
    if (!is_source_or_header(f.name)) return out;
    for (const CodeLine& l : code_lines(f.text)) {
        pointer_set.for_each_match(l.code, [&](const RegexMatch& m) {
            if (!primitives.contains(m.group(1))) {
                out.push_back(finding(l.number, Category::Risk, Severity::Med, "ptr-container-compare",
                                      std::format("std::set<{}*> compares by POINTER ADDRESS, not value (Java HashSet uses .equals/.hashCode) "
                                                  "-- logically-equal entries duplicate and lookups miss. Use a value comparator or key by "
                                                  "a value type. (scheduledtick-pointer-compare class.)",
                                                  m.group(1)),
                                      clipped(strip(l.code), 150)));
            }
            return true;
        });
        pointer_map.for_each_match(l.code, [&](const RegexMatch& m) {
            if (!primitives.contains(m.group(1))) {
                out.push_back(finding(l.number, Category::Risk, Severity::Med, "ptr-container-compare",
                                      std::format("std::map<{}*, ...> keys by POINTER ADDRESS, not value -- a lookup with an equal-but-"
                                                  "different key MISSES. Key by a value type or add a comparator. (regionfile-cache-never-hits "
                                                  "class.)",
                                                  m.group(1)),
                                      clipped(strip(l.code), 150)));
            }
            return true;
        });
    }
    return out;
}

// `\b<name>\s*\[[^\]]+\]`: the end of the first indexing of the variable, or npos.
size_t find_index_expression(Text code, Text name) {
    for (size_t p = code.find(name); p != kNone; p = code.find(name, p + 1)) {
        if (!word_before(code, p)) continue;
        const size_t lb = skip_blanks(code, p + name.size());
        if (lb >= code.size() || code[lb] != '[') continue;
        const size_t rb = code.find(']', lb + 1);
        if (rb != kNone && rb > lb + 1) return rb + 1;
    }
    return kNone;
}

Findings rule_map_read_insert(const FileInput& f, const Context&) {
    static const Regex map_variable(
        R"(\b(?:std::map|std::unordered_map|JMap|HashMap\w*|TreeMap\w*|LinkedHashMap\w*)\s*<[^;{}]*>\s*\*?\s*(\w+))");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    std::string hay(f.text);
    if (f.header) hay += "\n" + std::string(*f.header);
    std::set<std::string> maps;
    map_variable.for_each_match(hay, [&](const RegexMatch& m) {
        maps.emplace(m.group(1));
        return true;
    });
    if (maps.empty()) return out;
    for (const CodeLine& l : code_lines(f.text)) {
        for (const std::string& name : maps) {   // (sorted: the findings of one line come in one order)
            const size_t end = find_index_expression(l.code, name);
            if (end == kNone) continue;
            const Text after = text::ltrim(l.code.substr(end));
            if (after.starts_with("=") && !after.starts_with("==")) continue;   // a write: map[k] = v (the intended insert)
            out.push_back(finding(l.number, Category::Risk, Severity::High, "map-read-insert",
                                  std::format("{}[...] READ: C++ map::operator[] INSERTS a default on read (Java .get() returns null). The "
                                              "silent insert makes a 'contains?' check lie -- the chunkExists->gen-recursion FREEZE class. "
                                              "Use .count()/.find() for lookups; reserve [] for writes.",
                                              name),
                                  clipped(strip(l.raw), 150)));
            break;
        }
    }
    return out;
}

Findings rule_throw_fallthrough(const FileInput& f, const Context&) {
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    std::vector<std::pair<std::uint32_t, Text>> rows;
    for (const CodeLine& l : code_lines(f.text)) {
        if (!strip(l.code).empty()) rows.emplace_back(l.number, strip(l.code));
    }
    for (size_t idx = 0; idx + 1 < rows.size(); ++idx) {
        const auto& [number, s] = rows[idx];
        if (!s.starts_with("throw")) continue;
        if (s.size() > 5 && is_word(s[5])) continue;   // 'throws' / 'throwFoo', not a throw statement
        const Text next = rows[idx + 1].second;
        // the throw is last in its block -> abort == return here
        if (next.empty() || next[0] == '}' || next[0] == ')' || next.starts_with("case ") || next.starts_with("default:") ||
            next.starts_with("#")) {
            continue;
        }
        out.push_back(finding(number, Category::Risk, Severity::Med, "throw-fallthrough",
                              "`throw` does NOT unwind on this console -- it is a no-op and the code after it STILL RUNS (Java aborted "
                              "here). Replace with an early `return` / guarded skip so the fall-through can't corrupt state or return "
                              "garbage. (The _TRACE_THROWS runtime tracer names any that fire.)",
                              clipped(s, 150)));
    }
    return out;
}

// `(?i)(listener|observer|callback|handler|watcher|subscriber)s?$`: observer lists hold NON-owned references.
bool is_observer_name(Text name) {
    const std::string lower = text::lower(name);
    for (Text word : {"listener", "observer", "callback", "handler", "watcher", "subscriber"}) {
        if (lower.ends_with(word) || (lower.ends_with("s") && Text(lower).substr(0, lower.size() - 1).ends_with(word))) return true;
    }
    return false;
}

Findings rule_leak_drop_no_delete(const FileInput& f, const Context&) {
    static const Regex owning_container(
        R"(\b(?:JList|std::vector|JSet|std::set|JMap|std::map|std::deque)\s*<[^;{}]*\*\s*>\s*\*?\s*(\w+))");
    static const Regex removal(R"((?:\.|->)(clear|erase|remove|pop_back|pop_front|pop)\s*\()");
    Findings out;
    if (!ends_with_c_file(f.name)) return out;
    std::string hay(f.text);
    if (f.header) hay += "\n" + std::string(*f.header);
    std::set<std::string> owners;
    owning_container.for_each_match(hay, [&](const RegexMatch& m) {
        if (!is_observer_name(m.group(1))) owners.emplace(m.group(1));
        return true;
    });
    if (owners.empty()) return out;
    const std::vector<CodeLine> rows = code_lines(f.text);
    for (size_t idx = 0; idx < rows.size(); ++idx) {
        const Text code = rows[idx].code;
        for (const std::string& name : owners) {   // (sorted: the findings of one line come in one order)
            // `\bNAME(?:\.|->)(clear|...)\s*\(`: the first such removal on the line
            std::optional<std::string> method;
            for (size_t p = code.find(name); p != kNone && !method; p = code.find(name, p + 1)) {
                if (!word_before(code, p)) continue;
                const Text rest = code.substr(p + name.size());
                const std::optional<RegexMatch> m = removal.match(rest);
                if (m) method = std::string(m->group(1));
            }
            if (!method) continue;
            bool deleted_nearby = false;   // a delete of the removed element is nearby
            for (size_t w = idx >= 6 ? idx - 6 : 0; w <= idx; ++w) deleted_nearby = deleted_nearby || contains(rows[w].code, "delete");
            if (deleted_nearby) continue;
            out.push_back(finding(rows[idx].number, Category::Risk, Severity::Med, "leak-drop-no-delete",
                                  std::format("{}.{}() removes from an OWNING pointer container with no `delete` -- the object is dropped but "
                                              "never freed on the GC-less heap. Capture the removed pointer and delete it first. "
                                              "(dead-particle / clear-effects / chunk-drop / packet leak class.)",
                                              name, *method),
                                  clipped(strip(code), 150)));
            break;
        }
    }
    return out;
}

// ---- 14. silent-dispatch ---------------------------------------------------------------------------------------------------------

// How many `else if (<var> == CONSTANT` branches the text holds: `\belse\s+if\s*\(\s*VAR\s*==\s*[A-Z][A-Z0-9_]{2,}`.
size_t count_else_if_branches(Text region, Text var) {
    size_t count = 0;
    size_t from = 0;
    while (true) {
        const size_t p = region.find("else", from);
        if (p == kNone) break;
        from = p + 1;
        if (!word_before(region, p)) continue;
        size_t i = skip_blanks(region, p + 4);
        if (i == p + 4 || !region.substr(i).starts_with("if")) continue;
        i = skip_blanks(region, i + 2);
        if (i >= region.size() || region[i] != '(') continue;
        i = skip_blanks(region, i + 1);
        if (!region.substr(i).starts_with(var)) continue;
        i = skip_blanks(region, i + var.size());
        if (!region.substr(i).starts_with("==")) continue;
        i = skip_blanks(region, i + 2);
        if (i >= region.size() || region[i] < 'A' || region[i] > 'Z') continue;
        size_t n = 1;
        while (i + n < region.size() && ((region[i + n] >= 'A' && region[i + n] <= 'Z') || (region[i + n] >= '0' && region[i + n] <= '9') || region[i + n] == '_')) ++n;
        if (n < 3) continue;
        ++count;
        from = i + n;
    }
    return count;
}

Findings rule_silent_dispatch(const FileInput& f, const Context&) {
    static const Regex dispatch_if(R"(\bif\s*\(\s*(\w+)\s*==\s*[A-Z][A-Z0-9_]{2,})");
    static const Regex terminal_else(R"(\belse\b(?!\s*if))");
    Findings out;
    if (!is_source_or_header(f.name)) return out;
    const std::vector<Text> lines = text::split(f.text, '\n');
    for (size_t i = 0; i < lines.size(); ++i) {
        const Text ln = lines[i];
        const std::optional<RegexMatch> m = dispatch_if.search(ln);
        if (!m) continue;
        if (text::rtrim(ln.substr(0, m->start())).ends_with("else")) continue;   // this `if` is an `else if` -- not a chain start
        const Text var = m->group(1);
        // gather the chain: this line + following `else*` lines, skipping interior comment/blank lines so an explanatory comment
        // between two else-ifs doesn't truncate the chain
        std::string region(ln);
        for (size_t k = i + 1; k < lines.size() && (k - i) < 40; ++k) {
            const Text s = strip(lines[k]);
            if (s.starts_with("else") || s.empty() || s.starts_with("//") || s.starts_with("/*") || s.starts_with("*")) {
                region += "\n";
                region.append(lines[k]);
            } else {
                break;
            }
        }
        const size_t elseifs = count_else_if_branches(region, var);
        if (elseifs < 1) continue;   // a lone `if` (e.g. `if (rc==A || rc==B)`): not a multi-branch DISPATCH; nothing to fall through
        const size_t branches = elseifs + 1;
        if (contains(region, "J2X_UNHANDLED") || contains(region, "J2X_NOOP")) continue;   // reports the gap, or explicitly blessed
        if (terminal_else.test(region)) continue;   // has a terminal else (assume it handles/reports)
        out.push_back(finding(static_cast<std::uint32_t>(i + 1), Category::Risk, Severity::High, "silent-dispatch",
                              std::format("enum dispatch on '{}' ({} cases) falls through SILENTLY on an unhandled value -- the fog-bug class. "
                                          "End the chain with J2X_UNHANDLED(\"<fn>\", {}), or J2X_NOOP(\"why\") if the silent fall-through is "
                                          "deliberate.",
                                          var, branches, var),
                              clipped(strip(ln), 160)));
    }
    return out;
}

// ---- the whole-tree passes ---------------------------------------------------------------------------------------------------

const Regex& class_header() {
    static const Regex re(R"(\b(?:class|struct)\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::([^{;()]*))?\{)");
    return re;
}

// The index of the '}' matching the '{' at `open` (the end of the text if unbalanced).
size_t matching_close_brace(Text src, size_t open) {
    size_t depth = 0;
    for (size_t j = open; j < src.size(); ++j) {
        if (src[j] == '{') ++depth;
        if (src[j] == '}' && --depth == 0) return j;
    }
    return src.size();
}

std::vector<std::string> sorted_headers(const stdfs::path& dir) {
    std::vector<std::string> names;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (name.ends_with(".h")) names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::string collapse_blanks(Text t) {
    std::string out;
    bool in_blank = false;
    for (char c : t) {
        if (text::is_space(c)) {
            in_blank = true;
            continue;
        }
        if (in_blank && !out.empty()) out += ' ';
        in_blank = false;
        out += c;
    }
    return out;
}

}  // namespace

Context Context::make(std::string_view platform_key, const profiles::Profile* profile) {
    Context c;
    if (!platform_key.empty()) {
        c.platform_label = std::string(platform_key);
        if (const platforms::Platform* p = platforms::find_platform(platform_key)) {
            c.platform_label = std::string(p->label);
            c.big_endian = p->big_endian;
            c.null_deref_msg = std::string(p->null_deref_msg);
        }
    }
    if (profile != nullptr) {
        c.heavy_types = profile->heavy_types;
        c.heavy_types_high = profile->heavy_types_high;
        c.nullable_getters.insert(profile->type.nullable_getters.begin(), profile->type.nullable_getters.end());
        c.nullable_fields.insert(profile->type.nullable_fields.begin(), profile->type.nullable_fields.end());
    }
    return c;
}

const std::vector<RuleInfo>& all_rules() {
    static const std::vector<RuleInfo> rules = {
        {"silent-dispatch", "enum dispatcher can fall through silently on an unhandled value (fail-loud doctrine)", rule_silent_dispatch},
        {"map-read-insert", "map[key] READ inserts a default (Java .get() returns null) -> chunkExists lies -> freeze", rule_map_read_insert},
        {"throw-fallthrough", "throw is a no-op that does not unwind on the 360 -> code after it still runs", rule_throw_fallthrough},
        {"leak-drop-no-delete", "element removed from an owning pointer container without delete (no-GC drop leak)", rule_leak_drop_no_delete},
        {"leak-temp-new", "local new never freed/returned/stored/added -> no-GC temp leak", rule_leak_temp_new},
        {"leak-dtor-member", "member new-d in ctor but not deleted in dtor -> no-GC lifetime leak", rule_leak_dtor_member},
        {"ptr-container-compare", "std::set/map keyed by a non-primitive pointer -> compares by address, not value", rule_ptr_container_compare},
        {"uninit-array", "new T[n] without () leaves primitives/pointers uninitialized", rule_uninit_array},
        {"ctor-stub", "empty or delegation-only constructor body (missing registration)", rule_ctor_stub},
        {"rule-of-three", "owns heap but missing destructor / copy control", rule_rule_of_three},
        {"value-copy-owner", "heavy/owning type passed or returned by value", rule_value_copy_owner},
        {"virtual-call-in-ctor", "constructor calls own virtual method (dispatches to base, not override)", rule_virtual_call_in_ctor},
        {"static-init-order", "eager static init with a cross-TU dependency (undefined init order)", rule_static_init_order},
        {"endianness", "raw multi-byte cast in binary-IO file (big-endian hazard)", rule_endianness_raw_cast},
        {"null-deref-getter", "getter return dereferenced without null check", rule_null_deref_getter},
        {"null-deref-field", "known-nullable field dereferenced without null check", rule_null_deref_field},
        {"hotpath-alloc", "heap allocation inside a loop (per-iteration alloc storm / hang)", rule_hotpath_alloc},
        {"infinite-loop", "unbounded loop with no break/return in body (thread wedge)", rule_infinite_loop},
        {"leak-loop-reassign", "heap local reassigned in a loop without freeing the prior value", rule_leak_loop_reassign},
        {"side-effect-in-branch", "impure statement in a guarded branch that returns (RenderPig class)", rule_side_effect_in_branch},
        {"debug-bypass", "raw debug output not routed through the single Debug.h master switch", rule_debug_bypass},
    };
    return rules;
}

Findings check(std::string_view rule_id, const FileInput& file, const Context& context) {
    for (const RuleInfo& r : all_rules()) {
        if (r.id == rule_id) return r.check(file, context);
    }
    throw UsageError("unknown rule: " + std::string(rule_id));
}

std::size_t scan_odr(ledger::Ledger& ledger, const stdfs::path& output_dir) {
    // class name -> {header: whitespace-normalised body}; sorted, so the findings come in one order
    std::map<std::string, std::map<std::string, std::string>> defs;
    for (const std::string& fname : sorted_headers(output_dir)) {
        const std::string src = strip_block_comments(textutil::read_text(output_dir / fs::from_text(fname)));
        class_header().for_each_match(src, [&](const RegexMatch& m) {
            const size_t open = m.end() - 1;
            defs[std::string(m.group(1))][fname] = strip(collapse_blanks(Text(src).substr(open, matching_close_brace(src, open) - open)));
            return true;
        });
    }
    std::size_t found = 0;
    for (const auto& [name, file_bodies] : defs) {
        // Identical definitions across guarded headers are LEGAL (ODR permits it) and compile fine -- only DIFFERING definitions of
        // the same name are a real violation (the C4743 / mismatched-layout class).
        std::set<std::string_view> bodies;
        std::vector<std::string> files;
        for (const auto& [file, body] : file_bodies) {
            bodies.insert(body);
            files.push_back(file);
        }
        if (files.size() > 1 && bodies.size() > 1) {
            ledger.add(files.front(), Category::Risk,
                       std::format("class '{}' has DIFFERING definitions across {} headers: {} -- one-definition-rule violation (mismatched "
                                   "layout / C4743 class). Keep ONE canonical definition; the rest must #include it, not redefine it "
                                   "differently.",
                                   name, files.size(), text::join(files, ", ")),
                       {.severity = Severity::High, .source = "rule", .rule = "odr-duplicate-def"});
            ++found;
        }
    }
    return found;
}

ClassMethodPairs build_override_pairs(const stdfs::path& output_dir) {
    static const Regex method_call(R"((?<![\w>])([A-Za-z_]\w*)\s*\()");
    static const Regex last_identifier(R"(([A-Za-z_]\w*)\s*$)");
    static const std::unordered_set<std::string_view> not_methods = {"if", "for", "while", "switch", "return", "sizeof", "catch", "new", "delete"};
    std::map<std::string, std::vector<std::string>> parents;
    std::map<std::string, std::set<std::string>> methods;
    for (const std::string& fname : sorted_headers(output_dir)) {
        const std::string src = strip_block_comments(textutil::read_text(output_dir / fs::from_text(fname)));
        class_header().for_each_match(src, [&](const RegexMatch& m) {
            const std::string name(m.group(1));
            std::vector<std::string> bases;
            if (m.has(2)) {
                for (Text b : text::split(m.group(2), ',')) {
                    if (const auto bm = last_identifier.search(strip(b))) bases.emplace_back(bm->group(1));
                }
            }
            const size_t open = m.end() - 1;
            const Text body = Text(src).substr(open + 1, matching_close_brace(src, open) - (open + 1));
            std::set<std::string>& declared = methods[name];
            method_call.for_each_match(body, [&](const RegexMatch& c) {
                if (!not_methods.contains(c.group(1)) && c.group(1) != name) declared.emplace(c.group(1));   // (not the constructor itself)
                return true;
            });
            auto& p = parents[name];
            p.insert(p.end(), bases.begin(), bases.end());
            return true;
        });
    }
    ClassMethodPairs pairs;
    for (const auto& [cls, declared] : methods) {
        std::set<std::string> ancestors;
        std::vector<std::string> todo = {cls};
        while (!todo.empty()) {
            const std::string c = std::move(todo.back());
            todo.pop_back();
            const auto it = parents.find(c);
            if (it == parents.end()) continue;
            for (const std::string& p : it->second) {
                if (ancestors.insert(p).second) todo.push_back(p);
            }
        }
        for (const std::string& anc : ancestors) {
            for (const std::string& meth : declared) pairs.emplace(anc, meth);
        }
    }
    return pairs;
}

std::size_t scan_rules(ledger::Ledger& ledger, const stdfs::path& output_dir, const ScanOptions& options) {
    Context ctx = Context::make(options.platform, options.profile);
    ctx.override_pairs = build_override_pairs(output_dir);   // the cross-file hierarchy for virtual-call-in-ctor
    std::vector<std::string> files;
    std::error_code ec;
    for (stdfs::directory_iterator it(output_dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (is_source_or_header(name)) files.push_back(name);
    }
    std::sort(files.begin(), files.end());
    std::size_t n = 0;
    for (const std::string& fname : files) {
        const std::string text = textutil::read_text(output_dir / fs::from_text(fname));
        std::optional<std::string> header;
        if (fname.ends_with(".cpp")) {
            const stdfs::path hp = output_dir / fs::from_text(fname.substr(0, fname.size() - 4) + ".h");
            std::error_code isfile_ec;
            if (stdfs::is_regular_file(hp, isfile_ec)) header = textutil::read_text(hp);
        }
        const FileInput input{fname, text, header ? std::optional<Text>(*header) : std::nullopt};
        for (const RuleInfo& rule : all_rules()) {
            for (const Finding& found : rule.check(input, ctx)) {
                ledger.add(fname, found.category, found.message,
                           {.line = found.line, .severity = found.severity, .source = "rule", .rule = found.rule, .snippet = found.snippet});
                ++n;
            }
        }
    }
    n += scan_odr(ledger, output_dir);   // the cross-file ODR pass (per-file rules cannot see it)
    return n;
}

}  // namespace cb::java::rules
