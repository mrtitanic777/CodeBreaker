// dotnet/decompiler/util/xml_linq.hpp -- a small XML document tree (System.Xml.Linq's XElement, XText, XCData, XDocument as ILSpy's documentation
// code uses them): parse a text, walk the elements, their attributes and text.
//
//     XDocument doc = XDocument::parse("<doc>a<b x='1'>t</b></doc>");
//     const XElement* root = doc.element("doc");        // the first child element of that name
//     for (const XNode* n : root->nodes()) ...          // XText "a", XElement b
//     root->nodes()[1]... as XElement: attribute("x")->value() == "1"
//
// Ported from ILSpy's use of System.Xml.Linq (XmlDocumentationElement), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Parsing is XDocument.Parse with its default options: the text is read by util/xml_text_reader.hpp as XmlReader.Create
// reads it (line breaks "\n"), and a text node of white space only is dropped unless an enclosing element says xml:space="preserve" (the
// default of LoadOptions.None). Adjacent text and character references make one XText; CDATA is an XCData (an XText, as upstream's
// subclass, so `as_xtext` finds both). Comments and processing instructions are not kept (nothing reads them). Names are as written: no
// namespaces (a name is compared as written). Not here: building or changing a tree, XPath, serialization (ToString).
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/util/xml_text_reader.hpp"

namespace cb::dotnet::decompiler::util {

class XElement;

class XNode {
public:
    virtual ~XNode() = default;
    // The element holding this node; nullptr for the root's children's parent chain's end (the document level).
    const XElement* parent() const { return parent_; }

protected:
    XNode() = default;

private:
    friend class XElement;
    friend class XDocument;
    const XElement* parent_ = nullptr;
};

// Text; and CDATA text, which is an XText with is_cdata().
class XText : public XNode {
public:
    explicit XText(std::string value, bool cdata = false) : value_(std::move(value)), cdata_(cdata) {}
    const std::string& value() const { return value_; }
    bool is_cdata() const { return cdata_; }

private:
    std::string value_;
    bool cdata_;
};

class XAttribute {
public:
    XAttribute(std::string name, std::string value) : name_(std::move(name)), value_(std::move(value)) {}
    const std::string& name() const { return name_; }
    const std::string& value() const { return value_; }

private:
    std::string name_, value_;
};

class XElement : public XNode {
public:
    explicit XElement(std::string name) : name_(std::move(name)) {}
    // The name as written; local_name() is the part after a prefix.
    const std::string& name() const { return name_; }
    std::string local_name() const;
    const std::vector<XAttribute>& attributes() const { return attributes_; }
    // The attribute of that name (as written); nullptr for none.
    const XAttribute* attribute(std::string_view name) const;
    // The child nodes in order (XElement and XText).
    std::vector<const XNode*> nodes() const;
    // The first child element of that name; nullptr for none.
    const XElement* element(std::string_view name) const;

    void add_attribute(XAttribute attribute) { attributes_.push_back(std::move(attribute)); }
    void add(std::unique_ptr<XNode> node);

private:
    std::string name_;
    std::vector<XAttribute> attributes_;
    std::vector<std::unique_ptr<XNode>> children_;
};

// Downcasts of nodes (`child as XElement`, `child as XText`).
inline const XElement* as_xelement(const XNode* node) { return dynamic_cast<const XElement*>(node); }
inline const XText* as_xtext(const XNode* node) { return dynamic_cast<const XText*>(node); }

class XDocument {
public:
    // XmlError for text that is not well-formed XML.
    static XDocument parse(std::string_view text);
    XDocument() = default;
    XDocument(XDocument&&) noexcept = default;
    XDocument& operator=(XDocument&&) noexcept = default;

    const XElement* root() const { return root_.get(); }
    // The root element when it is named so (upstream: the first child element of the document of that name); nullptr otherwise.
    const XElement* element(std::string_view name) const;

private:
    std::unique_ptr<XElement> root_;
};

}  // namespace cb::dotnet::decompiler::util
