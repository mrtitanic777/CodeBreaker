// dotnet/decompiler/csharp/transforms/add_xml_documentation_transform.cpp -- see add_xml_documentation_transform.hpp.
#include "dotnet/decompiler/csharp/transforms/add_xml_documentation_transform.hpp"

#include <optional>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/documentation/idocumentation_provider.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

namespace {

bool is_white_space_byte(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }

bool is_null_or_white_space(const std::string& s) {
    for (char c : s)
        if (!is_white_space_byte(c)) return false;
    return true;
}

// StringReader.ReadLine: a line ends at \r, \n or \r\n; nullopt at the end of the text.
std::optional<std::string> read_line(const std::string& text, std::size_t& pos) {
    if (pos >= text.size()) return std::nullopt;
    std::size_t end = pos;
    while (end < text.size() && text[end] != '\r' && text[end] != '\n') end++;
    std::string line = text.substr(pos, end - pos);
    if (end < text.size()) {
        if (text[end] == '\r' && end + 1 < text.size() && text[end + 1] == '\n') end++;
        end++;
    }
    pos = end;
    return line;
}

}  // namespace

void AddXmlDocumentationTransform::insert_xml_documentation(AstNode* node, const std::string& documentation) {
    std::size_t pos = 0;
    // Find the first non-empty line:
    std::optional<std::string> first_line;
    do {
        first_line = read_line(documentation, pos);
        if (!first_line) return;
    } while (is_null_or_white_space(*first_line));
    std::size_t indent_length = 0;
    while (indent_length < first_line->size() && is_white_space_byte((*first_line)[indent_length])) indent_length++;
    std::string indentation = first_line->substr(0, indent_length);
    std::optional<std::string> line = first_line;
    int skipped_whitespace_lines = 0;
    // Copy all lines from input to output, except for empty lines at the end.
    while (line) {
        if (is_null_or_white_space(*line)) {
            skipped_whitespace_lines++;
        } else {
            while (skipped_whitespace_lines > 0) {
                Comment* empty_line = make<Comment>(std::string(), CommentType::Documentation);
                empty_line->add_annotation(get_resolve_result(node));
                node->add_leading_trivia(empty_line);
                skipped_whitespace_lines--;
            }
            std::string text = *line;
            if (text.starts_with(indentation)) text = text.substr(indentation.size());
            Comment* comment = make<Comment>(" " + text, CommentType::Documentation);
            comment->add_annotation(get_resolve_result(node));
            node->add_leading_trivia(comment);
        }
        line = read_line(documentation, pos);
    }
}

void AddXmlDocumentationTransform::run(AstNode* root_node, TransformContext& context) {
    if (!context.settings.show_xml_documentation || context.decompile_run.documentation_provider == nullptr) return;
    try {
        auto* provider = context.decompile_run.documentation_provider;
        for (AstNode* node : root_node->descendants_and_self()) {
            auto* entity_decl = as<EntityDeclaration>(node);
            if (entity_decl == nullptr) continue;
            const auto* entity = dynamic_cast<const typesystem::IEntity*>(get_symbol(entity_decl));
            if (entity == nullptr) continue;
            std::optional<std::string> doc = provider->get_documentation(entity);
            if (!doc) {
                const auto* accessor = dynamic_cast<const typesystem::IMethod*>(entity);
                const auto* owner = accessor != nullptr ? dynamic_cast<const typesystem::IProperty*>(accessor->accessor_owner()) : nullptr;
                if (owner != nullptr && typesystem::is_parameterized_property(owner)) {
                    const typesystem::IMethod* first = owner->getter() != nullptr ? owner->getter() : owner->setter();
                    if (accessor->equals(first)) {
                        // A parameterized property is decompiled to its accessor methods; show the property's documentation on the first accessor.
                        doc = provider->get_documentation(owner);
                    }
                }
            }
            if (doc) {
                context.step("Add XML documentation", entity_decl);
                insert_xml_documentation(entity_decl, *doc);
            }
        }
    } catch (const documentation::XmlException& ex) {
        std::string message = std::string(" Exception while reading XmlDoc: ") + ex.what();
        std::size_t pos = 0;
        while (auto line = read_line(message, pos)) {
            if (line->empty()) continue;
            root_node->add_leading_trivia(make<Comment>(*line, CommentType::Documentation));
        }
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
