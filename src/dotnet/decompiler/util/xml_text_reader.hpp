// dotnet/decompiler/util/xml_text_reader.hpp -- a pull reader for XML text, the part of System.Xml.XmlTextReader that ILSpy's documentation
// provider uses: node by node, with each node's type, name, value, attributes, line and column, and the reading of an element's inner XML.
//
//     XmlTextReader r(bytes);   r.move_to_content();           // on the root element
//     while (r.read()) if (r.is_start_element() && r.local_name() == "members") ...
//     r.line_number(), r.line_position()                       // where the current element's name starts (1-based, UTF-16 units)
//     std::string inner = r.read_inner_xml();                 // the markup inside the current element, written by XmlTextWriter
//
// Ported from ILSpy's use of System.Xml.XmlTextReader (XmlDocumentationProvider), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). Written to the reader's behaviour on .NET 9 (the cases are in work/ilspy-port/il4/X/xmlw).
//
// What is here: elements (an empty one, `<a/>`, is one Element node with is_empty_element(), and no EndElement), attributes, text (entities
// and character references decoded; no DTD, so only the five predefined entities), CDATA, comments, processing instructions, the XML
// declaration, a document type (skipped as a whole), white space as its own node type. A node's line and position are where its name or text
// starts; line breaks are "\r\n", "\n" and "\r". The text is UTF-8 (a byte order mark is skipped); an XML declaration naming another
// encoding than UTF-8 or US-ASCII is an XmlError. Not here: namespaces (a name is as written, local_name() cuts its prefix), DTD entities,
// validation. A document that is not well formed in what the reader meets (a mismatched end tag, an unterminated tag, text outside the
// root) is an XmlError, as upstream's XmlException.
//
// Two ways to read: a document (the default), and a fragment (`fragment = true`: any number of top-level nodes, as XmlNodeType.Element
// parsing context allows), which is how the provider reads one member out of the file. `normalize` is XmlReader.Create's behaviour: line breaks
// in text become "\n", and tabs and line breaks in attribute values become spaces (XmlTextReader leaves both as they are).
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

// System.Xml.XmlException: the text is not well-formed XML.
class XmlError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(XmlError)
};

enum class XmlNodeType {
    None,
    Element,
    Text,
    CDATA,
    ProcessingInstruction,
    Comment,
    DocumentType,
    Whitespace,
    EndElement,
    XmlDeclaration,
};

struct XmlReaderOptions {
    bool fragment = false;
    bool normalize = false;
};

class XmlTextReader {
public:
    explicit XmlTextReader(ByteView text, XmlReaderOptions options = {});

    XmlNodeType node_type() const { return type_; }
    // The name as written, and its part after a prefix ("a:b" -> "b"); empty for a node with no name.
    const std::string& name() const { return name_; }
    std::string local_name() const;
    // A text, CDATA, white space, comment or processing instruction node's content (a processing instruction's text after its target).
    const std::string& value() const { return value_; }
    bool is_empty_element() const { return empty_element_; }
    int depth() const { return depth_; }
    // The attribute of the current element by name (as written); nullopt for none.
    std::optional<std::string> get_attribute(std::string_view name) const;
    const std::vector<std::pair<std::string, std::string>>& attributes() const { return attributes_; }
    // The line (1-based) and position in it (1-based, in UTF-16 units) of the node's name (an element's or an end element's) or text.
    int line_number() const;
    int line_position() const;
    // Encoding.WebName of the encoding of the text: "utf-8" or "us-ascii" (what the XML declaration said, else "utf-8").
    const std::string& encoding_web_name() const { return encoding_; }
    bool at_end() const { return end_; }

    // The next node; false at the end of the text. XmlError for text that is not well formed.
    bool read();
    // Reads on until the current node is an element, end element, CDATA or text (what the reader is on already, it stays on).
    XmlNodeType move_to_content();
    // MoveToContent, then whether the node is an Element.
    bool is_start_element();
    // On an element: past it and everything in it, to the node after its end tag; on another node: the next node.
    void skip();
    // On an element: its content as markup (children written by XmlTextWriter: `<a />` for an empty element, entities escaped, attributes
    // double-quoted), and the reader moves to the node after the end tag; "" for an empty element or another node.
    std::string read_inner_xml();

private:
    [[noreturn]] void fail(const std::string& message) const;
    void parse_text();
    void parse_markup();
    void parse_element();
    void parse_end_element();
    void parse_declaration_or_pi();
    void parse_doctype();
    std::string decode(std::string_view raw, bool in_attribute) const;
    std::size_t find(std::string_view what, std::size_t from);
    void skip_space();
    std::string parse_name();
    void set_node(XmlNodeType type, std::size_t position);
    void check_encoding(const std::string& declared);

    std::string_view text_;
    XmlReaderOptions options_;
    std::string encoding_ = "utf-8";
    std::size_t first_ = 0;       // after the byte order mark
    std::size_t pos_ = 0;         // the next byte to read
    bool started_ = false, end_ = false, root_seen_ = false;
    std::vector<std::string> open_;   // names of the open elements
    XmlNodeType type_ = XmlNodeType::None;
    std::string name_, value_;
    bool empty_element_ = false;
    int depth_ = 0;
    std::size_t node_offset_ = 0;
    std::vector<std::pair<std::string, std::string>> attributes_;
    mutable std::vector<std::size_t> line_starts_;
};

}  // namespace cb::dotnet::decompiler::util
