// dotnet/decompiler/util/xml_text_writer.hpp -- an XML writer that writes what System.Xml.XmlTextWriter writes, character for character, for the
// calls ILSpy's project writers make: indentation by two spaces, `<?xml version="1.0" encoding="utf-8"?>`, double-quoted attributes, `<a />` for
// an element with no content, a closing tag on the line of its text, and no indentation inside mixed content.
//
//     StringWriter out;
//     XmlTextWriter xml(out);   xml.set_formatting(XmlFormatting::Indented);
//     xml.write_start_element("Project");  xml.write_attribute_string("Sdk", "Microsoft.NET.Sdk");
//     xml.write_element_string("OutputType", "Exe");
//     xml.write_end_element();    // xml.close() closes what is open and the TextWriter
//
// Ported from ILSpy's use of System.Xml.XmlTextWriter (ProjectFileWriterDefault, ProjectFileWriterSdkStyle, ResXResourceWriter), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Written to the writer's behaviour on .NET 9 (the cases are in
// work/ilspy-port/il4/X/xmlw): not everything of XmlTextWriter is here, only the members the port's callers use, and a call outside what XML
// allows (an attribute after content, an end element with none open) is a UsageError.
//
// Where the rules are not obvious: an element is "mixed" once text, raw text or white space is written in it; a mixed element and all that
// it holds is written without indentation, and its end tag stays on the line of its content. A start tag is closed (`>`) when the first
// content comes. An element ended with no content is `<a />` (write_end_element) or `<a></a>` (write_full_end_element). A default
// namespace is declared (`xmlns="..."`) after the element's other attributes when the element names one that is not already in effect.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/util/text_writer.hpp"

namespace cb::dotnet::decompiler::util {

enum class XmlFormatting { None, Indented };

class XmlTextWriter {
public:
    // The writer takes text to `out`, which must stay alive as long as this writer; close() closes it too (as XmlTextWriter.Close does).
    explicit XmlTextWriter(TextWriter& out) : out_(&out) {}
    XmlTextWriter(const XmlTextWriter&) = delete;
    XmlTextWriter& operator=(const XmlTextWriter&) = delete;
    // Closes the writer as Dispose does (what is open is ended); it does not throw.
    ~XmlTextWriter();

    XmlFormatting formatting() const { return formatting_; }
    void set_formatting(XmlFormatting value) { formatting_ = value; }
    int indentation() const { return indentation_; }
    void set_indentation(int value) { indentation_ = value; }
    // A fragment: any number of top-level elements and text (what the inner XML of an element is). Off by default (a document: one root).
    void set_fragment(bool value) { fragment_ = value; }

    // `<?xml version="1.0" encoding="utf-8"?>`, the encoding being the TextWriter's.
    void write_start_document();
    // Ends every open element.
    void write_end_document();
    // `<name`; with `ns`, the element's default namespace.
    void write_start_element(std::string_view name, std::optional<std::string_view> ns = std::nullopt);
    // Ends the element: `<a />` when nothing was written in it, else `</a>`.
    void write_end_element();
    // Ends the element with `</a>` even when nothing was written in it.
    void write_full_end_element();
    // ` name="value"` on the start tag just written.
    void write_attribute_string(std::string_view name, std::string_view value);
    // Text: & < > escaped, control characters as character references; nothing for an empty string.
    void write_string(std::string_view text);
    void write_value(std::string_view text) { write_string(text); }
    // Text as it is.
    void write_raw(std::string_view text);
    // White space as it is (UsageError for anything else).
    void write_whitespace(std::string_view whitespace);
    // `<![CDATA[text]]>`, `<!--text-->` and `<?name text?>`.
    void write_cdata(std::string_view text);
    void write_comment(std::string_view text);
    void write_processing_instruction(std::string_view name, std::string_view text);
    // `<name>value</name>`, `<name />` for an empty value.
    void write_element_string(std::string_view name, std::string_view value);

    void flush() { out_->flush(); }
    // Ends what is open and closes the TextWriter.
    void close();

private:
    enum class State { Start, Prolog, Element, Content, Epilog, Closed };
    struct Open {
        std::string name;
        bool mixed = false;           // text or white space was written in this element or in one around it
        std::string default_ns;       // the default namespace in effect inside the element
        std::optional<std::string> declared_ns;   // to write on the start tag: xmlns="..."
    };

    void close_start_tag(bool empty);
    // Before text, raw text or white space.
    void begin_content();
    void write_indent(std::size_t levels);
    void write_escaped(std::string_view text, bool in_attribute);
    void end_element(bool full);
    void end_all();

    TextWriter* out_;
    XmlFormatting formatting_ = XmlFormatting::None;
    int indentation_ = 2;
    bool fragment_ = false;
    State state_ = State::Start;
    std::vector<Open> stack_;
};

}  // namespace cb::dotnet::decompiler::util
