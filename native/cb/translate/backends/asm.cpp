#include "cb/translate/backends/asm.hpp"

#include <format>

namespace cb::translate::backends::assembly {

namespace {

constexpr std::string_view kCommentLead = ";";

std::size_t count_newlines(std::string_view s) {
    std::size_t n = 0;
    for (char c : s) n += c == '\n';
    return n;
}

}  // namespace

std::string comment(std::string_view text, std::string_view syntax, const CommentLeads& leads) {
    const auto found = leads.find(std::string(syntax));
    std::string out = found != leads.end() ? found->second : std::string(kCommentLead);
    if (!text.empty()) {
        out += ' ';
        out += text;
    }
    return out;
}

Result<std::optional<std::string>> unit(const Unit& u, const CommentLeads& leads) {
    if (const auto* fragment = std::get_if<MachineFragment>(&u)) {
        if (!fragment->text) {
            if (fragment->within) return std::optional<std::string>();
            return Declined(std::format("fragment {} has no text", fragment->name.empty() ? "?" : fragment->name));
        }
        return std::optional<std::string>(*fragment->text);
    }
    if (const auto* c = std::get_if<Comment>(&u)) return std::optional<std::string>(comment(c->text, c->syntax, leads));
    return Declined(std::format("document {}", std::get<OtherUnit>(u).kind));
}

Result<Assembled> assemble(const Document& doc, const CommentLeads& leads) {
    if (doc.kind != kModuleNode) return Declined(std::format("document {}", doc.kind));
    Assembled out;
    out.first_line.assign(doc.units.size(), std::nullopt);
    std::size_t line = 1;
    bool first = true;
    for (std::size_t k = 0; k < doc.units.size(); ++k) {
        CB_TRY_ASSIGN(const auto text, unit(doc.units[k], leads));
        if (!text) continue;
        out.first_line[k] = line;
        if (!first) out.text += '\n';
        first = false;
        out.text += *text;
        line += count_newlines(*text) + 1;
    }
    return out;
}

Result<std::string> module(const Document& doc, const CommentLeads& leads) {
    CB_TRY_ASSIGN(auto assembled, assemble(doc, leads));
    return std::move(assembled.text);
}

}  // namespace cb::translate::backends::assembly
