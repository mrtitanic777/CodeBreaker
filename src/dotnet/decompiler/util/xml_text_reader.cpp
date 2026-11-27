#include "dotnet/decompiler/util/xml_text_reader.hpp"

#include <algorithm>
#include <cstdint>

#include "dotnet/decompiler/util/utf8_encode.hpp"
#include "dotnet/decompiler/util/xml_text_writer.hpp"

namespace cb::dotnet::decompiler::util {

namespace {

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

bool is_all_space(std::string_view s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return is_space(c); });
}

bool is_name_end(char c) { return is_space(c) || c == '=' || c == '/' || c == '>' || c == '<' || c == '"' || c == '\'' || c == '?'; }

}  // namespace

XmlTextReader::XmlTextReader(ByteView text, XmlReaderOptions options) : text_(cb::as_text(text)), options_(options) {
    if (text_.size() >= 3 && static_cast<unsigned char>(text_[0]) == 0xEF && static_cast<unsigned char>(text_[1]) == 0xBB &&
        static_cast<unsigned char>(text_[2]) == 0xBF)
        first_ = 3;
    if (text_.size() >= 2 && ((static_cast<unsigned char>(text_[0]) == 0xFF && static_cast<unsigned char>(text_[1]) == 0xFE) ||
                              (static_cast<unsigned char>(text_[0]) == 0xFE && static_cast<unsigned char>(text_[1]) == 0xFF)))
        throw XmlError("XmlTextReader: UTF-16 text is not supported");
    pos_ = first_;
}

void XmlTextReader::fail(const std::string& message) const {
    throw XmlError(message + " Line " + std::to_string(line_number()) + ", position " + std::to_string(line_position()) + ".");
}

std::string XmlTextReader::local_name() const {
    const std::size_t colon = name_.find(':');
    return colon == std::string::npos ? name_ : name_.substr(colon + 1);
}

std::optional<std::string> XmlTextReader::get_attribute(std::string_view name) const {
    for (const auto& [n, v] : attributes_)
        if (n == name) return v;
    return std::nullopt;
}

int XmlTextReader::line_number() const {
    if (line_starts_.empty()) {
        line_starts_.push_back(first_);
        for (std::size_t i = first_; i < text_.size(); ++i) {
            if (text_[i] == '\n') {
                line_starts_.push_back(i + 1);
            } else if (text_[i] == '\r') {
                if (i + 1 < text_.size() && text_[i + 1] == '\n') ++i;
                line_starts_.push_back(i + 1);
            }
        }
    }
    const auto it = std::upper_bound(line_starts_.begin(), line_starts_.end(), node_offset_);
    return static_cast<int>(it - line_starts_.begin());
}

int XmlTextReader::line_position() const {
    const int line = line_number();
    const std::size_t start = line_starts_[static_cast<std::size_t>(line - 1)];
    int units = 1;
    for (std::size_t i = start; i < node_offset_ && i < text_.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(text_[i]);
        if ((c & 0xC0) == 0x80) continue;   // a continuation byte
        units += c >= 0xF0 ? 2 : 1;          // a character outside the BMP is two UTF-16 units
    }
    return units;
}

void XmlTextReader::set_node(XmlNodeType type, std::size_t position) {
    type_ = type;
    node_offset_ = position;
    name_.clear();
    value_.clear();
    attributes_.clear();
    empty_element_ = false;
}

std::size_t XmlTextReader::find(std::string_view what, std::size_t from) {
    const std::size_t at = text_.find(what, from);
    if (at == std::string_view::npos) {
        node_offset_ = std::min(from, text_.size());
        fail("Unexpected end of file while parsing.");
    }
    return at;
}

void XmlTextReader::skip_space() {
    while (pos_ < text_.size() && is_space(text_[pos_])) ++pos_;
}

std::string XmlTextReader::parse_name() {
    const std::size_t start = pos_;
    while (pos_ < text_.size() && !is_name_end(text_[pos_])) ++pos_;
    if (pos_ == start) {
        node_offset_ = pos_;
        fail("Name expected.");
    }
    return std::string(text_.substr(start, pos_ - start));
}

std::string XmlTextReader::decode(std::string_view raw, bool in_attribute) const {
    std::string out;
    out.reserve(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        const char c = raw[i];
        if (c == '&') {
            const std::size_t semi = raw.find(';', i);
            if (semi == std::string_view::npos) fail("An entity reference is not terminated.");
            const std::string_view ref = raw.substr(i + 1, semi - i - 1);
            if (ref == "lt") {
                out.push_back('<');
            } else if (ref == "gt") {
                out.push_back('>');
            } else if (ref == "amp") {
                out.push_back('&');
            } else if (ref == "quot") {
                out.push_back('"');
            } else if (ref == "apos") {
                out.push_back('\'');
            } else if (ref.size() >= 2 && ref[0] == '#') {
                std::uint32_t cp = 0;
                const bool hex = ref[1] == 'x';
                const std::string_view digits = ref.substr(hex ? 2 : 1);
                if (digits.empty()) fail("A character reference has no digits.");
                for (char d : digits) {
                    int v;
                    if (d >= '0' && d <= '9') v = d - '0';
                    else if (hex && d >= 'a' && d <= 'f') v = d - 'a' + 10;
                    else if (hex && d >= 'A' && d <= 'F') v = d - 'A' + 10;
                    else fail("A character reference is not a number.");
                    cp = cp * (hex ? 16 : 10) + static_cast<std::uint32_t>(v);
                    if (cp > 0x10FFFF) fail("A character reference is out of range.");
                }
                append_utf8_code_point(out, cp);
            } else {
                fail("Reference to undeclared entity '" + std::string(ref) + "'.");
            }
            i = semi;
        } else if (options_.normalize && c == '\r') {
            if (in_attribute) {
                out.push_back(' ');
                if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
            } else {
                out.push_back('\n');
                if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
            }
        } else if (options_.normalize && in_attribute && (c == '\n' || c == '\t')) {
            out.push_back(' ');
        } else {
            out.push_back(c);
        }
    }
    return out;
}

bool XmlTextReader::read() {
    if (end_) return false;
    started_ = true;
    if (pos_ >= text_.size()) {
        end_ = true;
        set_node(XmlNodeType::None, text_.size());
        depth_ = 0;
        if (!open_.empty()) fail("Unexpected end of file has occurred. The following elements are not closed: " + open_.back() + ".");
        if (!options_.fragment && !root_seen_) fail("Root element is missing.");
        return false;
    }
    if (text_[pos_] == '<') parse_markup();
    else parse_text();
    return true;
}

void XmlTextReader::parse_text() {
    const std::size_t start = pos_;
    std::size_t end = text_.find('<', pos_);
    if (end == std::string_view::npos) end = text_.size();
    pos_ = end;
    const std::string_view raw = text_.substr(start, end - start);
    set_node(XmlNodeType::Text, start);
    depth_ = static_cast<int>(open_.size());
    if (is_all_space(raw)) {
        type_ = XmlNodeType::Whitespace;
        value_ = options_.normalize ? decode(raw, false) : std::string(raw);
        return;
    }
    if (!options_.fragment && open_.empty()) fail("Data at the root level is invalid.");
    value_ = decode(raw, false);
}

void XmlTextReader::parse_markup() {
    const std::string_view rest = text_.substr(pos_);
    if (rest.starts_with("<?")) {
        parse_declaration_or_pi();
    } else if (rest.starts_with("<!--")) {
        set_node(XmlNodeType::Comment, pos_ + 4);
        const std::size_t end = find("-->", pos_ + 4);
        value_ = std::string(text_.substr(pos_ + 4, end - pos_ - 4));
        pos_ = end + 3;
        depth_ = static_cast<int>(open_.size());
    } else if (rest.starts_with("<![CDATA[")) {
        set_node(XmlNodeType::CDATA, pos_ + 9);
        const std::size_t end = find("]]>", pos_ + 9);
        value_ = std::string(text_.substr(pos_ + 9, end - pos_ - 9));
        if (options_.normalize) {
            std::string normalized;
            for (std::size_t i = 0; i < value_.size(); ++i) {
                if (value_[i] == '\r') {
                    normalized.push_back('\n');
                    if (i + 1 < value_.size() && value_[i + 1] == '\n') ++i;
                } else {
                    normalized.push_back(value_[i]);
                }
            }
            value_ = std::move(normalized);
        }
        pos_ = end + 3;
        depth_ = static_cast<int>(open_.size());
        if (!options_.fragment && open_.empty()) fail("Data at the root level is invalid.");
    } else if (rest.starts_with("<!")) {
        parse_doctype();
    } else if (rest.starts_with("</")) {
        parse_end_element();
    } else {
        parse_element();
    }
}

void XmlTextReader::parse_declaration_or_pi() {
    const std::size_t start = pos_;
    pos_ += 2;
    node_offset_ = pos_;
    const std::string target = parse_name();
    const std::size_t end = find("?>", pos_);
    std::string content(text_.substr(pos_, end - pos_));
    pos_ = end + 2;
    const std::size_t content_start = content.find_first_not_of(" \t\r\n");
    content = content_start == std::string::npos ? std::string() : content.substr(content_start);
    set_node(XmlNodeType::ProcessingInstruction, start + 2);
    depth_ = static_cast<int>(open_.size());
    name_ = target;
    value_ = content;
    if (target == "xml") {
        type_ = XmlNodeType::XmlDeclaration;
        // encoding="..."
        const std::size_t at = content.find("encoding");
        if (at != std::string::npos) {
            std::size_t q = content.find_first_of("\"'", at);
            if (q != std::string::npos) {
                const std::size_t q2 = content.find(content[q], q + 1);
                if (q2 != std::string::npos) check_encoding(content.substr(q + 1, q2 - q - 1));
            }
        }
    }
}

void XmlTextReader::check_encoding(const std::string& declared) {
    std::string lower;
    for (char c : declared) lower.push_back(static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c));
    if (lower == "utf-8" || lower == "utf8") {
        encoding_ = "utf-8";
    } else if (lower == "us-ascii" || lower == "ascii") {
        encoding_ = "us-ascii";
    } else {
        throw XmlError("XmlTextReader: the encoding '" + declared + "' is not supported (only UTF-8 and US-ASCII are)");
    }
}

void XmlTextReader::parse_doctype() {
    set_node(XmlNodeType::DocumentType, pos_ + 2);
    // up to the matching '>' (an internal subset in [ ] may hold '>' of its own, and quoted text anything)
    std::size_t i = pos_ + 2;
    int brackets = 0;
    char quote = 0;
    for (; i < text_.size(); ++i) {
        const char c = text_[i];
        if (quote != 0) {
            if (c == quote) quote = 0;
        } else if (c == '"' || c == '\'') {
            quote = c;
        } else if (c == '[') {
            ++brackets;
        } else if (c == ']') {
            --brackets;
        } else if (c == '>' && brackets <= 0) {
            break;
        }
    }
    if (i >= text_.size()) fail("Unexpected end of file while parsing a document type.");
    value_ = std::string(text_.substr(pos_ + 2, i - pos_ - 2));
    pos_ = i + 1;
    depth_ = 0;
}

void XmlTextReader::parse_element() {
    const std::size_t start = ++pos_;   // after '<'
    set_node(XmlNodeType::Element, start);
    if (!options_.fragment && root_seen_ && open_.empty()) fail("There are multiple root elements.");
    name_ = parse_name();
    for (;;) {
        skip_space();
        if (pos_ >= text_.size()) {
            node_offset_ = pos_;
            fail("Unexpected end of file while parsing a start tag.");
        }
        const char c = text_[pos_];
        if (c == '>') {
            ++pos_;
            break;
        }
        if (c == '/') {
            if (pos_ + 1 >= text_.size() || text_[pos_ + 1] != '>') {
                node_offset_ = pos_;
                fail("'>' is expected.");
            }
            pos_ += 2;
            empty_element_ = true;
            break;
        }
        const std::size_t attribute_start = pos_;
        std::string attribute_name = parse_name();
        skip_space();
        if (pos_ >= text_.size() || text_[pos_] != '=') {
            node_offset_ = pos_;
            fail("'=' is expected.");
        }
        ++pos_;
        skip_space();
        if (pos_ >= text_.size() || (text_[pos_] != '"' && text_[pos_] != '\'')) {
            node_offset_ = pos_;
            fail("A quote is expected.");
        }
        const char quote = text_[pos_++];
        const std::size_t end = text_.find(quote, pos_);
        if (end == std::string_view::npos) {
            node_offset_ = pos_;
            fail("Unexpected end of file while parsing an attribute value.");
        }
        const std::string_view raw = text_.substr(pos_, end - pos_);
        if (raw.find('<') != std::string_view::npos) {
            node_offset_ = pos_;
            fail("'<' is an unexpected token in an attribute value.");
        }
        for (const auto& existing : attributes_)
            if (existing.first == attribute_name) {
                node_offset_ = attribute_start;
                fail("'" + attribute_name + "' is a duplicate attribute name.");
            }
        attributes_.emplace_back(std::move(attribute_name), decode(raw, true));
        pos_ = end + 1;
    }
    node_offset_ = start;
    depth_ = static_cast<int>(open_.size());
    root_seen_ = true;
    if (!empty_element_) open_.push_back(name_);
}

void XmlTextReader::parse_end_element() {
    pos_ += 2;
    set_node(XmlNodeType::EndElement, pos_);
    name_ = parse_name();
    skip_space();
    if (pos_ >= text_.size() || text_[pos_] != '>') {
        node_offset_ = pos_;
        fail("'>' is expected.");
    }
    ++pos_;
    if (open_.empty() || open_.back() != name_) {
        fail("The '" + name_ + "' end tag does not match its start tag.");
    }
    open_.pop_back();
    depth_ = static_cast<int>(open_.size());
}

XmlNodeType XmlTextReader::move_to_content() {
    for (;;) {
        switch (type_) {
        case XmlNodeType::Element:
        case XmlNodeType::EndElement:
        case XmlNodeType::CDATA:
        case XmlNodeType::Text:
            return type_;
        default: break;
        }
        if (!read()) return type_;
    }
}

bool XmlTextReader::is_start_element() { return move_to_content() == XmlNodeType::Element; }

void XmlTextReader::skip() {
    if (!started_ || end_) return;
    if (type_ == XmlNodeType::Element && !empty_element_) {
        const int depth = depth_;
        while (read()) {
            if (type_ == XmlNodeType::EndElement && depth_ == depth) break;
        }
    }
    read();
}

std::string XmlTextReader::read_inner_xml() {
    if (!started_ || end_ || type_ != XmlNodeType::Element) return {};
    if (empty_element_) {
        read();
        return {};
    }
    StringWriter sink;
    {
        XmlTextWriter w(sink);
        w.set_fragment(true);
        const int depth = depth_;
        // Each start tag is written when it is met; the end of the element being read stops the loop.
        bool more = true;
        while (more && read()) {
            switch (type_) {
            case XmlNodeType::Element:
                w.write_start_element(name_);
                for (const auto& [n, v] : attributes_) w.write_attribute_string(n, v);
                if (empty_element_) w.write_end_element();
                break;
            case XmlNodeType::EndElement:
                if (depth_ == depth) {
                    more = false;
                } else {
                    w.write_full_end_element();
                }
                break;
            case XmlNodeType::Text: w.write_string(value_); break;
            case XmlNodeType::Whitespace: w.write_whitespace(value_); break;
            case XmlNodeType::CDATA: w.write_cdata(value_); break;
            case XmlNodeType::Comment: w.write_comment(value_); break;
            case XmlNodeType::ProcessingInstruction: w.write_processing_instruction(name_, value_); break;
            default: break;
            }
        }
        // The writer holds no open element (the element being read is not written), and writes nothing more.
    }
    read();
    return sink.str();
}

}  // namespace cb::dotnet::decompiler::util
