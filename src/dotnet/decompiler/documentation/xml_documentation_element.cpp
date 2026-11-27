#include "dotnet/decompiler/documentation/xml_documentation_element.hpp"

#include <algorithm>
#include <typeinfo>

#include "dotnet/decompiler/documentation/xml_doc_loader.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"

namespace cb::dotnet::decompiler::documentation {

namespace {

// The code point that ends at `end` in UTF-8 text (its first byte index in `start`), or the one that starts at `i` (its length in `length`).
std::uint32_t decode_at(const std::string& s, std::size_t i, std::size_t& length) {
    const unsigned char c = static_cast<unsigned char>(s[i]);
    std::size_t extra = c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
    std::uint32_t cp = extra == 0 ? c : (c & (0x3F >> extra));
    length = 1;
    while (extra-- > 0 && i + length < s.size() && (static_cast<unsigned char>(s[i + length]) & 0xC0) == 0x80) cp = (cp << 6) | (static_cast<unsigned char>(s[i + length++]) & 0x3F);
    return cp;
}

// char.IsWhiteSpace
bool is_white_space(std::uint32_t cp) {
    return (cp >= 0x09 && cp <= 0x0D) || cp == 0x20 || cp == 0x85 || cp == 0xA0 || cp == 0x1680 || (cp >= 0x2000 && cp <= 0x200A) || cp == 0x2028 ||
           cp == 0x2029 || cp == 0x202F || cp == 0x205F || cp == 0x3000;
}

bool is_null_or_white_space(const std::string& s) {
    for (std::size_t i = 0; i < s.size();) {
        std::size_t length;
        if (!is_white_space(decode_at(s, i, length))) return false;
        i += length;
    }
    return true;
}

std::string trim_start(const std::string& s) {
    std::size_t i = 0;
    while (i < s.size()) {
        std::size_t length;
        if (!is_white_space(decode_at(s, i, length))) break;
        i += length;
    }
    return s.substr(i);
}

std::string trim_end(const std::string& s) {
    std::size_t end = s.size();
    while (end > 0) {
        std::size_t start = end - 1;
        while (start > 0 && (static_cast<unsigned char>(s[start]) & 0xC0) == 0x80) --start;
        std::size_t length;
        if (!is_white_space(decode_at(s, start, length))) break;
        end = start;
    }
    return s.substr(0, end);
}

const char* const kDoNotInheritIfAlreadyPresent[] = {"example", "exclude", "filterpriority", "preliminary", "summary", "remarks", "returns", "threadsafety", "value"};

}  // namespace

XmlDocumentationElement::XmlDocumentationElement(std::shared_ptr<const util::XDocument> document, const util::XElement* element,
                                                 const typesystem::IEntity* declaring_entity, CrefResolver cref_resolver)
    : document_(std::move(document)), element_(element), declaring_entity_(declaring_entity), cref_resolver_(std::move(cref_resolver)) {
    if (element == nullptr) throw UsageError("element");
}

XmlDocumentationElement::XmlDocumentationElement(std::string text, const typesystem::IEntity* declaring_entity)
    : declaring_entity_(declaring_entity), text_content_(std::move(text)) {}

const typesystem::IEntity* XmlDocumentationElement::referenced_entity() const {
    const std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (!referenced_entity_initialized_) {
        const std::optional<std::string> cref = get_attribute(std::string("cref"));
        try {
            if (cref && !cref->empty() && cref_resolver_) referenced_entity_ = cref_resolver_(*cref);
        } catch (...) {
            referenced_entity_ = nullptr;
        }
        referenced_entity_initialized_ = true;
    }
    return referenced_entity_;
}

std::string XmlDocumentationElement::name() const { return element_ != nullptr ? element_->local_name() : std::string(); }

std::optional<std::string> XmlDocumentationElement::get_attribute(const std::optional<std::string>& name) const {
    if (!name || element_ == nullptr) return std::nullopt;
    const util::XAttribute* attribute = element_->attribute(*name);
    if (attribute == nullptr) return std::nullopt;
    return attribute->value();
}

const std::string& XmlDocumentationElement::text_content() const {
    const std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (!text_content_) {
        std::string b;
        for (const auto& child : children()) b += child->text_content();
        text_content_ = std::move(b);
    }
    return *text_content_;
}

const std::vector<std::unique_ptr<XmlDocumentationElement>>& XmlDocumentationElement::children() const {
    static const std::vector<std::unique_ptr<XmlDocumentationElement>> empty;
    if (element_ == nullptr) return empty;
    const std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (!children_) children_ = create_elements(element_->nodes(), document_, declaring_entity_, cref_resolver_, nesting_level_);
    return *children_;
}

std::vector<std::unique_ptr<XmlDocumentationElement>> XmlDocumentationElement::create_elements(const std::vector<const util::XNode*>& child_objects,
                                                                                                const std::shared_ptr<const util::XDocument>& document,
                                                                                                const typesystem::IEntity* declaring_entity,
                                                                                                const CrefResolver& cref_resolver, int nesting_level) {
    std::vector<std::unique_ptr<XmlDocumentationElement>> list;
    for (const util::XNode* child : child_objects) {
        const util::XText* child_text = util::as_xtext(child);   // an XCData is an XText
        const util::XElement* child_element = util::as_xelement(child);
        if (child_text != nullptr) {
            list.push_back(std::make_unique<XmlDocumentationElement>(child_text->value(), declaring_entity));
        } else if (child_element != nullptr) {
            if (nesting_level < 5 && child_element->name() == "inheritdoc") {
                const util::XAttribute* cref_attribute = child_element->attribute("cref");
                std::optional<std::string> cref;
                if (cref_attribute != nullptr) cref = cref_attribute->value();
                const typesystem::IEntity* inherited_from = nullptr;
                std::optional<std::string> inherited_documentation;
                if (cref && cref_resolver) {
                    inherited_from = cref_resolver(*cref);
                    if (inherited_from != nullptr) {
                        // "<doc>" + GetDocumentation() + "</doc>": no documentation is an empty one
                        inherited_documentation = "<doc>" + get_documentation(*inherited_from).value_or(std::string()) + "</doc>";
                    }
                } else if (declaring_entity != nullptr) {
                    const auto& declaring_member = dynamic_cast<const typesystem::IMember&>(*declaring_entity);   // std::bad_cast: not a member
                    for (const typesystem::IMember* base_member : typesystem::inheritance_helper::get_base_members(&declaring_member, /*include_implemented_interfaces=*/true)) {
                        std::optional<std::string> base_documentation = get_documentation(*base_member);
                        if (base_documentation) {
                            inherited_from = base_member;
                            inherited_documentation = "<doc>" + *base_documentation + "</doc>";
                            break;
                        }
                    }
                }

                if (inherited_documentation) {
                    auto inherited_document = std::make_shared<const util::XDocument>(util::XDocument::parse(*inherited_documentation));
                    const util::XElement* doc = inherited_document->element("doc");

                    // XPath filter not yet implemented
                    const bool parent_is_root = child_element->parent() == nullptr || child_element->parent()->parent() == nullptr;
                    if (doc != nullptr && parent_is_root && child_element->attribute("select") == nullptr) {
                        // Inheriting documentation at the root level
                        std::vector<std::string> do_not_inherit{"overloads"};
                        for (const util::XNode* sibling : child_objects) {
                            if (const util::XElement* e = util::as_xelement(sibling)) {
                                const std::string local_name = e->local_name();
                                for (const char* present : kDoNotInheritIfAlreadyPresent)
                                    if (local_name == present && std::find(do_not_inherit.begin(), do_not_inherit.end(), local_name) == do_not_inherit.end())
                                        do_not_inherit.push_back(local_name);
                            }
                        }

                        std::vector<const util::XNode*> inherited_children;
                        for (const util::XNode* inherited_object : doc->nodes()) {
                            const util::XElement* inherited_element = util::as_xelement(inherited_object);
                            if (!(inherited_element != nullptr &&
                                  std::find(do_not_inherit.begin(), do_not_inherit.end(), inherited_element->local_name()) != do_not_inherit.end()))
                                inherited_children.push_back(inherited_object);
                        }

                        for (auto& inherited : create_elements(inherited_children, inherited_document, inherited_from, cref_resolver, nesting_level + 1))
                            list.push_back(std::move(inherited));
                    }
                }
            } else {
                auto element = std::make_unique<XmlDocumentationElement>(document, child_element, declaring_entity, cref_resolver);
                element->nesting_level_ = nesting_level;
                list.push_back(std::move(element));
            }
        }
    }
    if (!list.empty() && list.front()->is_text_node()) {
        if (is_null_or_white_space(*list.front()->text_content_)) list.erase(list.begin());
        else list.front()->text_content_ = trim_start(*list.front()->text_content_);
    }
    if (!list.empty() && list.back()->is_text_node()) {
        if (is_null_or_white_space(*list.back()->text_content_)) list.pop_back();
        else list.back()->text_content_ = trim_end(*list.back()->text_content_);
    }
    return list;
}

std::string XmlDocumentationElement::to_string() const {
    if (element_ != nullptr) return "<" + element_->name() + ">";
    return text_content();
}

}  // namespace cb::dotnet::decompiler::documentation
