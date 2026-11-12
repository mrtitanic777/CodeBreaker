// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/parse.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/strview.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::parse {

namespace {

using ir::Access;

bool is_modifier(std::string_view token) {
    static constexpr std::string_view kModifiers[] = {"public", "protected", "private", "static",    "final",    "abstract",
                                                      "native", "synchronized", "transient", "volatile", "strictfp"};
    return std::find(std::begin(kModifiers), std::end(kModifiers), token) != std::end(kModifiers);
}

// -- Lexical helpers ----------------------------------------------------------------------------------------------

// The source with // and /* */ comments replaced by spaces (newlines kept), so byte offsets and line numbers are
// preserved. String and char literals are respected.
std::string blank_comments(std::string_view src) {
    std::string out;
    out.reserve(src.size());
    std::size_t i = 0;
    const std::size_t n = src.size();
    bool in_str = false;
    bool in_chr = false;
    while (i < n) {
        const char c = src[i];
        const char next = i + 1 < n ? src[i + 1] : '\0';
        if (in_str || in_chr) {
            out += c;
            if (c == '\\' && next != '\0') {
                out += next;
                i += 2;
                continue;
            }
            if (c == (in_str ? '"' : '\'')) in_str = in_chr = false;
            ++i;
            continue;
        }
        if (c == '"') {
            in_str = true;
            out += c;
            ++i;
            continue;
        }
        if (c == '\'') {
            in_chr = true;
            out += c;
            ++i;
            continue;
        }
        if (c == '/' && next == '/') {
            for (; i < n && src[i] != '\n'; ++i) out += ' ';
            continue;
        }
        if (c == '/' && next == '*') {
            for (; i < n && !(src[i] == '*' && i + 1 < n && src[i + 1] == '/'); ++i) out += src[i] == '\n' ? '\n' : ' ';
            out += "  ";   // the closing */
            i += 2;
            continue;
        }
        out += c;
        ++i;
    }
    return out;
}

// The index just past the `}` matching the `{` at `open` (literal-aware); the end of the text if it never closes.
std::size_t match_brace(std::string_view s, std::size_t open) {
    std::size_t depth = 0;
    std::size_t i = open;
    const std::size_t n = s.size();
    bool in_str = false;
    bool in_chr = false;
    while (i < n) {
        const char c = s[i];
        if (in_str || in_chr) {
            if (c == '\\') {
                i += 2;
                continue;
            }
            if (c == (in_str ? '"' : '\'')) in_str = in_chr = false;
        } else if (c == '"') {
            in_str = true;
        } else if (c == '\'') {
            in_chr = true;
        } else if (c == '{') {
            ++depth;
        } else if (c == '}') {
            if (--depth == 0) return i + 1;
        }
        ++i;
    }
    return n;
}

// Split on commas that are not nested in (), <>, [] or a literal; the pieces are trimmed and empty ones dropped.
std::vector<std::string> split_top_commas(std::string_view s) {
    std::vector<std::string> parts;
    std::string buf;
    int depth = 0;
    bool in_str = false;
    bool in_chr = false;
    for (char c : s) {
        if (in_str || in_chr) {
            buf += c;
            if (c == (in_str ? '"' : '\'')) in_str = in_chr = false;
            continue;
        }
        if (c == '"') {
            in_str = true;
            buf += c;
            continue;
        }
        if (c == '\'') {
            in_chr = true;
            buf += c;
            continue;
        }
        if (c == '(' || c == '<' || c == '[') {
            ++depth;
        } else if (c == ')' || c == '>' || c == ']') {
            --depth;
        }
        if (c == ',' && depth == 0) {
            parts.push_back(std::move(buf));
            buf.clear();
        } else {
            buf += c;
        }
    }
    parts.push_back(std::move(buf));
    std::vector<std::string> out;
    for (const std::string& p : parts) {
        const std::string_view t = text::trim(p);
        if (!t.empty()) out.emplace_back(t);
    }
    return out;
}

// The index of the first `ch` that is not nested in (), <>, [] or a literal, or npos.
std::size_t top_level_index(std::string_view s, char ch) {
    int depth = 0;
    bool in_str = false;
    bool in_chr = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (in_str || in_chr) {
            if (c == (in_str ? '"' : '\'')) in_str = in_chr = false;
            continue;
        }
        if (c == '"') {
            in_str = true;
        } else if (c == '\'') {
            in_chr = true;
        } else if (c == '(' || c == '<' || c == '[') {
            ++depth;
        } else if (c == ')' || c == '>' || c == ']') {
            --depth;
        } else if (c == ch && depth == 0) {
            return i;
        }
    }
    return std::string_view::npos;
}

// The 1-based line of a byte offset.
class LineIndex {
public:
    explicit LineIndex(std::string_view src) {
        for (std::size_t i = 0; i < src.size(); ++i) {
            if (src[i] == '\n') newlines_.push_back(i);
        }
    }
    std::uint32_t line_of(std::size_t offset) const {
        return static_cast<std::uint32_t>(std::lower_bound(newlines_.begin(), newlines_.end(), offset) - newlines_.begin()) + 1;
    }

private:
    std::vector<std::size_t> newlines_;
};

// -- Types, parameters, modifiers ---------------------------------------------------------------------------------

std::string_view last_dotted_part(std::string_view s) {
    const std::size_t dot = s.rfind('.');
    return dot == std::string_view::npos ? s : s.substr(dot + 1);
}

std::vector<ir::Param> parse_params(std::string_view s) {
    std::vector<ir::Param> out;
    for (const std::string& raw : split_top_commas(s)) {
        const std::string spaced = text::replace_all(raw, "...", "[]");   // varargs -> array
        std::vector<std::string_view> tokens;
        bool is_final = false;
        for (std::string_view t : text::split_ws(spaced)) {
            if (t == "final") {
                is_final = true;
            } else {
                tokens.push_back(t);
            }
        }
        if (tokens.size() < 2) continue;
        const std::string_view name = tokens.back();
        tokens.pop_back();
        out.push_back({std::string(name), parse_type(text::join(tokens, " ")), is_final, false});
    }
    return out;
}

struct Modifiers {
    bool is_public = false, is_protected = false, is_private = false, is_static = false, is_final = false, is_abstract = false;
    std::vector<std::string_view> rest;   // the tokens that are not modifiers

    Access access() const {
        if (is_public) return Access::Public;
        if (is_protected) return Access::Protected;
        if (is_private) return Access::Private;
        return Access::Package;
    }
};

Modifiers split_modifiers(const std::vector<std::string_view>& tokens) {
    Modifiers m;
    for (std::string_view t : tokens) {
        if (!is_modifier(t)) {
            m.rest.push_back(t);
        } else if (t == "public") {
            m.is_public = true;
        } else if (t == "protected") {
            m.is_protected = true;
        } else if (t == "private") {
            m.is_private = true;
        } else if (t == "static") {
            m.is_static = true;
        } else if (t == "final") {
            m.is_final = true;
        } else if (t == "abstract") {
            m.is_abstract = true;
        }
    }
    return m;
}

// -- Members ------------------------------------------------------------------------------------------------------

// `[mods] Type name [= init]` -> a field (one declarator per line, the MCP shape), or nothing when the text has no type.
std::optional<ir::Field> parse_field_decl(std::string_view decl, std::uint32_t line) {
    const std::size_t eq = top_level_index(decl, '=');
    const std::string_view head = text::trim(eq != std::string_view::npos ? decl.substr(0, eq) : decl);
    std::optional<std::string> init;
    if (eq != std::string_view::npos) init = std::string(text::trim(decl.substr(eq + 1)));
    const Modifiers mods = split_modifiers(text::split_ws(head));
    if (mods.rest.size() < 2) return std::nullopt;
    ir::Field f;
    std::string_view name = mods.rest.back();
    while (name.ends_with(',')) name.remove_suffix(1);
    f.name = std::string(name);
    f.type = parse_type(text::join(std::vector<std::string_view>(mods.rest.begin(), mods.rest.end() - 1), " "));
    f.access = mods.access();
    f.is_static = mods.is_static;
    f.is_final = mods.is_final;
    f.initializer = std::move(init);
    f.line = line;
    return f;
}

// `public final Vec3D subtract(Vec3D var1)` -> a method without its body. The first '(' opens the parameter list;
// a return type or name never contains one.
ir::Method parse_signature(std::string_view header, std::string_view class_name) {
    const std::size_t lp = header.find('(');
    const std::size_t rp = header.rfind(')');
    ir::Method m;
    if (lp != std::string_view::npos && rp != std::string_view::npos && rp > lp) m.params = parse_params(header.substr(lp + 1, rp - lp - 1));
    const Modifiers mods = split_modifiers(text::split_ws(lp == std::string_view::npos ? header : header.substr(0, lp)));
    if (mods.rest.empty()) throw ParseError("", 0, 0, "a member without a name: " + std::string(text::utf8_prefix(header, 60)));
    m.name = std::string(mods.rest.back());
    const std::string ret = text::join(std::vector<std::string_view>(mods.rest.begin(), mods.rest.end() - 1), " ");
    m.is_ctor = m.name == class_name && ret.empty();
    if (!m.is_ctor) m.return_type = ret.empty() ? ir::JType{"void"} : parse_type(ret);
    m.access = mods.access();
    m.is_static = mods.is_static;
    m.is_abstract = mods.is_abstract;
    m.is_final = mods.is_final;
    return m;
}

// A member decl is a method (not a field) if its first '(' is not preceded by '='. (A field initializer's '(' always
// follows the '='.)
bool looks_like_method(std::string_view decl) {
    const std::size_t lp = decl.find('(');
    if (lp == std::string_view::npos) return false;
    const std::size_t eq = top_level_index(decl, '=');
    return eq == std::string_view::npos || lp < eq;
}

// One member of a class body, as the walk finds it.
struct Member {
    enum class Kind { Decl, Method, StaticInit, InstInit } kind = Kind::Decl;
    std::string_view text;    // the declaration, the method header, or the block's inner text
    std::string_view inner;   // the method body text (Method only)
    std::uint32_t line = 0;   // the line of the declaration / method header / block body
    std::uint32_t body_line = 0;   // the line of the method body (Method only)
};

bool contains_type_keyword(const std::vector<std::string_view>& tokens) {
    return std::any_of(tokens.begin(), tokens.end(), [](std::string_view t) { return t == "class" || t == "interface" || t == "enum"; });
}

// A member's own first line is where its first non-blank character is (the text since the previous member ended also
// holds the line breaks and blank lines between them).
std::size_t first_non_blank(std::string_view body, std::size_t from) {
    while (from < body.size() && text::is_space(body[from])) ++from;
    return from;
}

// Walk a class body: fields and abstract methods end in `;` at depth 0, methods and blocks have a `{ }` body.
std::vector<Member> walk_members(std::string_view body, std::size_t body_offset, const LineIndex& lines) {
    std::vector<Member> out;
    std::size_t i = 0;
    const std::size_t n = body.size();
    std::size_t start = 0;
    bool in_str = false;
    bool in_chr = false;
    bool seen_eq = false;   // a '=' since the member start: an initializer follows
    while (i < n) {
        const char c = body[i];
        if (in_str || in_chr) {
            if (c == '\\') {
                i += 2;
                continue;
            }
            if (c == (in_str ? '"' : '\'')) in_str = in_chr = false;
            ++i;
            continue;
        }
        if (c == '"') {
            in_str = true;
            ++i;
            continue;
        }
        if (c == '\'') {
            in_chr = true;
            ++i;
            continue;
        }
        if (c == '=') {
            seen_eq = true;
            ++i;
            continue;
        }
        if (c == ';') {
            const std::string_view decl = text::trim(body.substr(start, i - start));
            if (!decl.empty()) out.push_back({Member::Kind::Decl, decl, {}, lines.line_of(body_offset + first_non_blank(body, start)), 0});
            ++i;
            start = i;
            seen_eq = false;
            continue;
        }
        if (c == '{') {
            if (seen_eq) {   // a field initializer `= ... { ... }` (array/aggregate), not a method body: keep the field
                i = match_brace(body, i);
                continue;
            }
            const std::string_view header = text::trim(body.substr(start, i - start));
            const std::size_t close = match_brace(body, i);
            const std::string_view inner = close - 1 > i ? body.substr(i + 1, close - 1 - (i + 1)) : std::string_view();
            const std::vector<std::string_view> tokens = text::split_ws(header);
            const bool only_modifiers = std::all_of(tokens.begin(), tokens.end(), is_modifier);
            const bool has_static = std::find(tokens.begin(), tokens.end(), "static") != tokens.end();
            if (contains_type_keyword(tokens)) {
                // an inner type: skipped
            } else if (has_static && only_modifiers) {
                out.push_back({Member::Kind::StaticInit, inner, {}, lines.line_of(body_offset + i + 1), 0});   // static { ... }
            } else if (header.empty()) {
                out.push_back({Member::Kind::InstInit, inner, {}, lines.line_of(body_offset + i + 1), 0});   // instance { ... }
            } else if (only_modifiers) {
                // another modifier-only block: skipped
            } else {
                out.push_back({Member::Kind::Method, header, inner, lines.line_of(body_offset + first_non_blank(body, start)), lines.line_of(body_offset + i + 1)});
            }
            i = close;
            start = i;
            seen_eq = false;
            continue;
        }
        ++i;
    }
    return out;
}

// The lines first..last (1-based, inclusive, clamped to the file) of the ORIGINAL source, with their numbers.
ir::Body body_lines(const std::vector<std::string_view>& src_lines, std::uint32_t first, std::uint32_t last) {
    ir::Body out;
    const auto stop = static_cast<std::uint32_t>(std::min<std::size_t>(last, src_lines.size()));
    for (std::uint32_t ln = first; ln <= stop; ++ln) out.push_back({ln, std::string(src_lines[ln - 1])});
    return out;
}

std::uint32_t newline_count(std::string_view s) { return static_cast<std::uint32_t>(std::count(s.begin(), s.end(), '\n')); }

// Add the members of a class (or enum tail) body to the class.
void add_members(ir::JClass& cls, const std::vector<Member>& members, const std::vector<std::string_view>& src_lines) {
    for (const Member& item : members) {
        switch (item.kind) {
            case Member::Kind::Decl:
                if (looks_like_method(item.text)) {   // an abstract / interface method
                    ir::Method m = parse_signature(item.text, cls.name);
                    if (cls.is_interface) m.is_abstract = true;
                    cls.methods.push_back(std::move(m));
                } else if (std::optional<ir::Field> f = parse_field_decl(item.text, item.line)) {
                    cls.fields.push_back(std::move(*f));
                }
                break;
            case Member::Kind::StaticInit:
            case Member::Kind::InstInit: {   // static { } / instance { } block: the lines of the original source
                ir::Body block = body_lines(src_lines, item.line, item.line + newline_count(item.text));
                (item.kind == Member::Kind::StaticInit ? cls.static_inits : cls.inst_inits).push_back(std::move(block));
                break;
            }
            case Member::Kind::Method: {
                ir::Method m = parse_signature(item.text, cls.name);
                m.line = item.line;
                // the body lines come from the ORIGINAL source (real text + real line numbers)
                m.body = body_lines(src_lines, item.body_line, item.body_line + newline_count(item.inner));
                cls.methods.push_back(std::move(m));
                break;
            }
        }
    }
}

// An enum: its constants (up to the first top-level `;`), then ordinary members. Bodies are captured so typed-enum
// constructors and methods translate; a constant-only enum ignores them (header-only int constants).
void parse_enum_body(ir::JClass& cls, std::string_view body, std::size_t body_offset, const LineIndex& lines,
                     const std::vector<std::string_view>& src_lines) {
    const std::size_t semi = top_level_index(body, ';');
    const std::string_view consts_part = semi != std::string_view::npos ? body.substr(0, semi) : body;
    for (const std::string& raw : split_top_commas(consts_part)) {
        const std::size_t lp = raw.find('(');
        ir::EnumConst c;
        if (lp != std::string::npos && text::rtrim(raw).ends_with(')')) {
            c.name = std::string(text::trim(std::string_view(raw).substr(0, lp)));
            const std::size_t rp = raw.rfind(')');
            for (std::string& a : split_top_commas(std::string_view(raw).substr(lp + 1, rp - lp - 1))) c.args.push_back(std::move(a));
        } else {
            c.name = std::string(text::trim(raw));
        }
        if (!c.name.empty()) cls.enum_consts.push_back(std::move(c));
    }
    if (semi != std::string_view::npos) {
        add_members(cls, walk_members(body.substr(semi + 1), body_offset + semi + 1, lines), src_lines);
    }
}

}  // namespace

ir::JType parse_type(std::string_view text_in) {
    std::string_view s = text::trim(text_in);
    std::uint32_t dims = 0;
    while (s.ends_with("[]")) {
        ++dims;
        s = text::trim(s.substr(0, s.size() - 2), " \t\n\r\f\v").substr(0);
        // (only trailing blanks were to go: rstrip)
    }
    ir::JType t;
    const std::size_t lt = s.find('<');
    if (lt != std::string_view::npos && s.ends_with('>')) {
        for (const std::string& a : split_top_commas(s.substr(lt + 1, s.size() - lt - 2))) t.args.push_back(parse_type(a));
        s = text::rtrim(s.substr(0, lt));
    }
    t.name = std::string(last_dotted_part(s));   // drop the package qualifier; emit uses simple names
    t.array_dims = dims;
    return t;
}

ir::JClass parse_source(std::string_view src, const std::optional<std::filesystem::path>& source_path) {
    static const Regex package_re(R"(\bpackage\s+([\w.]+)\s*;)");
    static const Regex type_decl(
        R"((?<mods>(?:\b(?:public|final|abstract|strictfp)\b\s+)*)(?<kind>class|interface|enum)\s+(?<name>\w+)(?<rest>[^{]*)\{)");
    static const Regex extends_re(R"(\bextends\s+([\w.]+))");
    static const Regex implements_re(R"(\bimplements\s+([\w.,\s]+))");

    const std::string blanked = blank_comments(src);
    const std::optional<RegexMatch> decl = type_decl.search(blanked);
    if (!decl) {
        throw ParseError(source_path ? fs::to_text(*source_path) : "", 0, 0, "no type declaration found");
    }

    ir::JClass cls;
    if (const auto pkg = package_re.search(blanked)) cls.package = std::string(pkg->group(1));
    const std::string_view kind = decl->named("kind").value_or("");
    cls.name = std::string(decl->named("name").value_or(""));
    const std::string_view rest = decl->named("rest").value_or("");
    if (const auto pm = extends_re.search(rest)) cls.parent = std::string(last_dotted_part(pm->group(1)));
    if (const auto im = implements_re.search(rest)) {
        for (std::string_view x : text::split(im->group(1), ',')) {
            if (!text::trim(x).empty()) cls.interfaces.emplace_back(text::trim(last_dotted_part(x)));
        }
    }
    cls.is_interface = kind == "interface";
    cls.is_abstract = contains(decl->named("mods").value_or(""), "abstract");
    cls.is_enum = kind == "enum";
    cls.source_path = source_path;

    const std::size_t open_brace = decl->end() - 1;
    const std::size_t close = match_brace(blanked, open_brace);
    const std::size_t body_offset = open_brace + 1;
    const std::string_view body = close - 1 > body_offset ? std::string_view(blanked).substr(body_offset, close - 1 - body_offset) : std::string_view();

    const LineIndex lines(blanked);
    const std::vector<std::string_view> src_lines = text::split(src, '\n');
    if (cls.is_enum) {
        parse_enum_body(cls, body, body_offset, lines, src_lines);
    } else {
        add_members(cls, walk_members(body, body_offset, lines), src_lines);
    }
    return cls;
}

ir::JClass parse_file(const std::filesystem::path& path) {
    const std::string src = textutil::read_text(path);
    return parse_source(src, path);
}

}  // namespace cb::java::parse
