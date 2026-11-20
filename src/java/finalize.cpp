#include "java/finalize.hpp"

#include "core/text.hpp"
#include "java/emit/std_transforms.hpp"

namespace cb::java::finalize {

namespace {

bool fin_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

std::size_t fin_skip_space(std::string_view s, std::size_t i) {
    while (i < s.size() && text::is_space(s[i])) ++i;
    return i;
}

// `new Object ( )`, the `new` standing at a word boundary: its end, or npos
std::size_t fin_bare_object_end(std::string_view s, std::size_t at) {
    std::size_t i = at + 3;
    const std::size_t after_new = i;
    i = fin_skip_space(s, i);
    if (i == after_new || !s.substr(i).starts_with("Object")) return std::string_view::npos;
    i = fin_skip_space(s, i + 6);
    if (i >= s.size() || s[i] != '(') return std::string_view::npos;
    i = fin_skip_space(s, i + 1);
    if (i >= s.size() || s[i] != ')') return std::string_view::npos;
    return i + 1;
}

// `new Object()` -> `JObject()`
std::string fin_replace_bare_objects(std::string_view text) {
    std::string out;
    std::size_t copied = 0;
    for (std::size_t at = text.find("new"); at != std::string_view::npos; at = text.find("new", at + 1)) {
        if (at > 0 && fin_is_word(text[at - 1])) continue;
        const std::size_t end = fin_bare_object_end(text, at);
        if (end == std::string_view::npos) continue;
        out.append(text.substr(copied, at - copied));
        out += "JObject()";
        copied = end;
        at = end - 1;
    }
    out.append(text.substr(copied));
    return out;
}

// A statement that is a bare `new X(...);` on a line of its own: its end (one past the `;`), or npos. The statement begins at
// `at` (the `new`), the rest of the line after the `;` is blank.
std::size_t fin_discarded_new_end(std::string_view s, std::size_t at) {
    if (!s.substr(at).starts_with("new")) return std::string_view::npos;
    std::size_t i = at + 3;
    const std::size_t after_new = i;
    i = fin_skip_space(s, i);
    if (i == after_new || i >= s.size() || !(std::isalpha(static_cast<unsigned char>(s[i])) || s[i] == '_')) return std::string_view::npos;
    while (i < s.size() && (fin_is_word(s[i]) || s[i] == ':')) ++i;   // the (possibly qualified) class name
    i = fin_skip_space(s, i);
    if (i >= s.size() || s[i] != '(') return std::string_view::npos;
    const std::size_t semicolon = s.find(';', i);
    if (semicolon == std::string_view::npos) return std::string_view::npos;
    std::size_t close = semicolon;   // the arguments end at the last non-blank character before the `;`, which must be `)`
    while (close > i && text::is_space(s[close - 1])) --close;
    if (close <= i + 1 || s[close - 1] != ')') return std::string_view::npos;
    // only blanks after the `;` up to the end of the line
    std::size_t rest = semicolon + 1;
    while (rest < s.size() && s[rest] != '\n' && text::is_space(s[rest])) ++rest;
    if (rest < s.size() && s[rest] != '\n') return std::string_view::npos;
    return semicolon + 1;
}

// A discarded allocation (the result is never stored) is a no-op in Java -- the temporary is collected at once -- but LEAKS in C++.
// It is faithful even in the original: RubyDung.render() has Notch's stray `new Cube(0, 0);`. It becomes `delete new X(...);` so the
// constructor's side effects still run and the object is freed. Skipped: arguments containing a nested `new` (deleting the outer
// object could double-free the inner one) -- left for manual review -- and a class whose constructor hands out `this` (kept by
// someone else; FIXED BUG discarded-new-escapes: deleting an Item left Item.itemsList a dangling pointer).
std::string fin_free_discarded_news(std::string_view text, const std::set<std::string, std::less<>>* escapes) {
    std::string out;
    std::size_t line_start = 0;
    while (line_start <= text.size()) {
        std::size_t line_end = text.find('\n', line_start);
        const bool last = line_end == std::string_view::npos;
        if (last) line_end = text.size();
        // a statement may span lines: it starts at the first non-blank character of this line
        std::size_t at = line_start;
        while (at < line_end && text::is_space(text[at])) ++at;
        std::size_t consumed_to = line_end;   // where this iteration's input ends (exclusive)
        bool rewritten = false;
        if (at < line_end) {
            const std::size_t end = fin_discarded_new_end(text, at);
            if (end != std::string_view::npos) {
                const std::string_view stmt = text.substr(at, end - at);
                bool nested_new = false;
                for (std::size_t n = stmt.find("new", 3); n != std::string_view::npos; n = stmt.find("new", n + 1)) {
                    const bool starts = !fin_is_word(stmt[n - 1]);
                    const bool ends = n + 3 >= stmt.size() || !fin_is_word(stmt[n + 3]);
                    nested_new = nested_new || (starts && ends);
                }
                std::size_t k = fin_skip_space(stmt, 3);
                const std::size_t name_begin = k;
                while (k < stmt.size() && fin_is_word(stmt[k])) ++k;
                const std::string name(stmt.substr(name_begin, k - name_begin));
                if (!nested_new && !(escapes != nullptr && escapes->contains(name))) {
                    out.append(text.substr(line_start, at - line_start));
                    out += "delete ";
                    out.append(stmt);
                    out += "  // J2X-BUGFIX discarded-new-leak";
                    // the statement may have run over several lines; the rest of its last line is blank
                    const std::size_t newline = text.find('\n', end);
                    consumed_to = newline == std::string_view::npos ? text.size() : newline;
                    rewritten = true;
                }
            }
        }
        if (!rewritten) out.append(text.substr(line_start, line_end - line_start));
        else line_end = consumed_to;
        if (last || line_end >= text.size()) break;
        out += '\n';
        line_start = line_end + 1;
    }
    return out;
}

}  // namespace

std::string convert_gl_to_d3d9(std::string_view text, std::span<const std::string_view> namespaces) {
    std::string out;
    out.reserve(text.size());
    std::size_t i = 0;
    while (i < text.size()) {
        bool replaced = false;
        if (!fin_is_word(i > 0 ? text[i - 1] : ' ')) {
            for (const std::string_view ns : namespaces) {
                if (text.substr(i).starts_with(ns) && text.substr(i + ns.size()).starts_with("::")) {
                    out += "d3d9::";
                    i += ns.size() + 2;
                    replaced = true;
                    break;
                }
            }
        }
        if (!replaced) out += text[i++];
    }
    return out;
}

std::string trace_guest_throws(std::string_view text) {
    std::string out;
    out.reserve(text.size() + text.size() / 16);
    bool first = true;
    for (const std::string_view line : text::split(text, '\n')) {
        if (!first) out += '\n';
        first = false;
        const std::string_view stripped = text::ltrim(line);
        const bool traced = line.find("J2X_GUEST_THROW") != std::string_view::npos;
        const char after = stripped.size() > 5 ? stripped[5] : '\0';   // the character right after `throw`
        if (traced || !stripped.starts_with("throw") || !(after == ' ' || after == '\t' || after == ';')) {   // 'throw <expr>' or bare 'throw;', not 'throws'
            out.append(line);
            continue;
        }
        out.append(line.substr(0, line.size() - stripped.size()));
        out += "J2X_GUEST_THROW(); ";
        out.append(stripped);
    }
    return out;
}

std::string finalize_emitted(std::string_view text_in, const Options& options) {
    std::string text = emit::std_transforms::sanitize_reserved_members(text_in);
    if (options.gl_to_d3d9) {
        // the project's last word (Minecraft: GL picking -> a raycast), BEFORE the namespace rename
        if (options.project != nullptr) text = options.project->finalize_text(std::move(text), "xbox360");
        text = convert_gl_to_d3d9(text);
    }
    if (options.trace_throws) text = trace_guest_throws(text);   // tag every throw with its site: throws are no-op landmines here
    text = text::replace_all(text, "'\\u0000'", "'\\0'");
    text = fin_replace_bare_objects(text);
    return fin_free_discarded_news(text, options.this_escapes);
}

}  // namespace cb::java::finalize
