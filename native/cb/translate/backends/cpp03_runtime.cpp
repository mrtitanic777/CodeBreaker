// cb/translate/backends/cpp03_runtime.cpp -- what the runtime's own headers declare: the return type of a method, the type and
// spelling of a field. The C++03 back end asks these where the converter's resolver does not know a library member.

#include "cb/core/text.hpp"
#include "cb/translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace runtime_impl {

bool word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// `\bword\b` somewhere in the line
bool contains_word(std::string_view line, std::string_view word) {
    for (std::size_t at = line.find(word); at != std::string_view::npos; at = line.find(word, at + 1)) {
        const bool left = at == 0 || !word_char(line[at - 1]);
        const bool right = at + word.size() >= line.size() || !word_char(line[at + word.size()]);
        if (left && right) return true;
    }
    return false;
}

// A name as a pattern literal (a Java identifier may hold `$`).
std::string escaped(std::string_view name) {
    std::string out;
    for (const char c : name) {
        if (!word_char(c)) out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

}  // namespace runtime_impl

const std::string& Context::runtime_header(std::string_view owner) const {
    static const std::string kNone;
    const auto& files = runtime.files();
    const auto it = files.find(std::string(owner) + ".h");
    return it != files.end() ? it->second : kNone;
}

std::optional<std::string> Context::runtime_return(std::string_view owner, std::string_view name) const {
    const std::string key = std::string(owner) + "|" + std::string(name);
    if (const auto it = return_memo_.find(key); it != return_memo_.end()) return it->second;
    std::unique_ptr<Regex>& pattern = return_patterns_[std::string(name)];
    if (!pattern) {   // a declaration of `name` at the start of a line: its qualifiers, then the type it returns (one per name)
        pattern = std::make_unique<Regex>(R"(^\s*(?:(?:inline|virtual|static)\s+)*([A-Za-z_][\w:<>,* &]*?)\s*\b)" +
                                          runtime_impl::escaped(name) + R"(\s*\()");
    }
    std::optional<std::string> found;
    for (const std::string_view line : text::lines(runtime_header(owner))) {
        const auto m = pattern->match(line);
        if (!m) continue;
        const std::string_view declared = text::trim(m->group(1));
        if (declared == "return" || declared == "else" || declared == "new" || declared == "delete") continue;
        std::string t = text::replace_all(m->group(1), "virtual ", "");
        t = text::replace_all(t, "inline ", "");
        t = text::replace_all(t, "static ", "");
        const std::string_view trimmed = text::trim(t);
        found = trimmed == "std::string" ? std::string("JavaString") : std::string(trimmed);
        break;
    }
    return return_memo_.emplace(key, found).first->second;
}

std::optional<std::pair<std::string, std::string>> Context::runtime_field(std::string_view owner, std::string_view name) const {
    const std::string key = std::string(owner) + "|" + std::string(name);
    if (const auto it = field_memo_.find(key); it != field_memo_.end()) return it->second;
    static const Regex declaration(R"(^\s*(?:static\s+)?(?:const\s+)?([A-Za-z_][\w:<>* &]*?)\s+[A-Za-z_])");
    std::optional<std::pair<std::string, std::string>> found;
    const std::string& header = runtime_header(owner);
    for (const std::string spelled : {"s_" + std::string(name), std::string(name)}) {   // the static spelling first
        for (const std::string_view line : text::lines(header)) {
            if (!runtime_impl::contains_word(line, spelled) || detail::has(line.substr(0, line.find(spelled)), "(")) continue;
            if (const auto m = declaration.match(line)) {
                found = std::make_pair(std::string(text::trim(m->group(1))), spelled);
                break;
            }
        }
        if (found) break;
    }
    return field_memo_.emplace(key, found).first->second;
}

}  // namespace cb::translate::backends::cpp03
