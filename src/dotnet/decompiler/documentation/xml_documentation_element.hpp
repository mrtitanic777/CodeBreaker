// dotnet/decompiler/documentation/xml_documentation_element.hpp -- an element of the XML documentation of an entity, with `<inheritdoc/>` replaced by
// the documentation it inherits: its name, attributes, text, children, and the entity a `cref` names.
//
//     XmlDocumentationElement root(document, document->element("doc"), entity, cref_resolver);
//     for (const auto& child : root.children()) child->name(), child->text_content(), child->referenced_entity();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Documentation: XmlDocumentationElement, from XmlDocumentationElement.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The XML is util/xml_linq.hpp's tree: an element that is made from an XElement keeps its
// document alive (a shared_ptr, which upstream's garbage collector does), as do the elements made from an inherited documentation (which this
// element parses itself). Upstream's null strings are std::nullopt. The lazily made parts (the text, the children, the referenced entity) are made
// once, from whichever thread asks first. The text of a documentation is trimmed of white space as char.IsWhiteSpace sees it (Unicode).
#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/xml_linq.hpp"

namespace cb::dotnet::decompiler::documentation {

// The entity a cref (an ID string) names, nullptr for none.
using CrefResolver = std::function<const typesystem::IEntity*(const std::string&)>;

// Represents an element in the XML documentation. Any occurrences of "<inheritdoc/>" are replaced with the inherited documentation.
class XmlDocumentationElement {
public:
    // A new documentation element of the XML element `element`, which `document` (nullptr when the caller keeps it alive otherwise) holds.
    // `declaring_entity` and `cref_resolver` may be null.
    XmlDocumentationElement(std::shared_ptr<const util::XDocument> document, const util::XElement* element, const typesystem::IEntity* declaring_entity,
                            CrefResolver cref_resolver);
    // A new documentation element that is the text.
    XmlDocumentationElement(std::string text, const typesystem::IEntity* declaring_entity);

    // The entity on which this documentation was originally declared. May be null.
    const typesystem::IEntity* declaring_entity() const { return declaring_entity_; }
    // The entity referenced by the 'cref' attribute. May be null.
    const typesystem::IEntity* referenced_entity() const;
    // The element name (empty for a text).
    std::string name() const;
    // The attribute value; nullopt for none.
    std::optional<std::string> get_attribute(const std::optional<std::string>& name) const;
    // Whether this is a pure text node.
    bool is_text_node() const { return element_ == nullptr; }
    // The text content: for an element, that of its children.
    const std::string& text_content() const;
    // The child elements.
    const std::vector<std::unique_ptr<XmlDocumentationElement>>& children() const;
    // "<name>" for an element, the text content for a text.
    std::string to_string() const;

private:
    // Inheritance level; used to prevent cyclic doc inheritance.
    int nesting_level_ = 0;

    // The children of a node list: texts and elements, with `inheritdoc` elements replaced.
    static std::vector<std::unique_ptr<XmlDocumentationElement>> create_elements(const std::vector<const util::XNode*>& child_objects,
                                                                                 const std::shared_ptr<const util::XDocument>& document,
                                                                                 const typesystem::IEntity* declaring_entity, const CrefResolver& cref_resolver,
                                                                                 int nesting_level);

    std::shared_ptr<const util::XDocument> document_;
    const util::XElement* element_ = nullptr;
    const typesystem::IEntity* declaring_entity_ = nullptr;
    CrefResolver cref_resolver_;
    mutable std::recursive_mutex mutex_;
    mutable std::optional<std::string> text_content_;
    mutable bool referenced_entity_initialized_ = false;
    mutable const typesystem::IEntity* referenced_entity_ = nullptr;
    mutable std::optional<std::vector<std::unique_ptr<XmlDocumentationElement>>> children_;
};

}  // namespace cb::dotnet::decompiler::documentation
