#include "dotnet/decompiler/util/xml_text_writer.hpp"

#include <format>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

XmlTextWriter::~XmlTextWriter() {
    try {
        close();
    } catch (...) {
    }
}

void XmlTextWriter::write_start_document() {
    if (state_ != State::Start) throw UsageError("XmlTextWriter: the document has been started already");
    state_ = State::Prolog;
    out_->write("<?xml version=\"1.0\" encoding=\"");
    out_->write(out_->encoding_web_name());
    out_->write("\"?>");
}

void XmlTextWriter::write_end_document() {
    end_all();
    if (state_ != State::Epilog) throw UsageError("XmlTextWriter: the document has no root element");
    state_ = State::Start;
}

void XmlTextWriter::write_start_element(std::string_view name, std::optional<std::string_view> ns) {
    if (state_ == State::Closed || state_ == State::Epilog) throw UsageError("XmlTextWriter: an element cannot be started here");
    if (state_ == State::Element) close_start_tag(false);
    if (formatting_ == XmlFormatting::Indented && state_ != State::Start && (stack_.empty() || !stack_.back().mixed))
        write_indent(stack_.size());
    Open open;
    open.name = std::string(name);
    open.mixed = !stack_.empty() && stack_.back().mixed;
    open.default_ns = stack_.empty() ? std::string() : stack_.back().default_ns;
    if (ns && *ns != open.default_ns) {
        open.default_ns = std::string(*ns);
        open.declared_ns = open.default_ns;
    }
    stack_.push_back(std::move(open));
    out_->write_char('<');
    out_->write(name);
    state_ = State::Element;
}

void XmlTextWriter::close_start_tag(bool empty) {
    Open& top = stack_.back();
    if (top.declared_ns) {
        out_->write(" xmlns=\"");
        write_escaped(*top.declared_ns, true);
        out_->write_char('"');
        top.declared_ns.reset();
    }
    out_->write(empty ? " />" : ">");
    state_ = State::Content;
}

void XmlTextWriter::begin_content() {
    if (stack_.empty()) {
        if (!fragment_ || state_ == State::Closed) throw UsageError("XmlTextWriter: text cannot be written outside an element");
        return;
    }
    if (state_ == State::Element) close_start_tag(false);
    stack_.back().mixed = true;
}

void XmlTextWriter::write_indent(std::size_t levels) {
    out_->write_line();
    if (indentation_ > 0 && levels > 0) out_->write(std::string(levels * static_cast<std::size_t>(indentation_), ' '));
}

void XmlTextWriter::write_attribute_string(std::string_view name, std::string_view value) {
    if (state_ != State::Element) throw UsageError("XmlTextWriter: an attribute can only follow a start tag");
    out_->write_char(' ');
    out_->write(name);
    out_->write("=\"");
    write_escaped(value, true);
    out_->write_char('"');
}

void XmlTextWriter::write_escaped(std::string_view text, bool in_attribute) {
    std::size_t run = 0;
    auto flush_run = [&](std::size_t end) {
        if (end > run) out_->write(text.substr(run, end - run));
    };
    for (std::size_t i = 0; i < text.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(text[i]);
        const char* entity = nullptr;
        std::string numeric;
        switch (c) {
        case '&': entity = "&amp;"; break;
        case '<': entity = "&lt;"; break;
        case '>': entity = "&gt;"; break;
        case '"':
            if (in_attribute) entity = "&quot;";
            break;
        case '\n':
        case '\r':
            if (in_attribute) {
                numeric = c == '\n' ? "&#xA;" : "&#xD;";
                entity = numeric.c_str();
            }
            break;
        case '\t': break;
        default:
            if (c < 0x20) {
                numeric = std::format("&#x{:X};", static_cast<unsigned>(c));
                entity = numeric.c_str();
            }
            break;
        }
        if (entity != nullptr) {
            flush_run(i);
            out_->write(entity);
            run = i + 1;
        }
    }
    flush_run(text.size());
}

void XmlTextWriter::write_string(std::string_view text) {
    if (text.empty()) return;
    begin_content();
    write_escaped(text, false);
}

void XmlTextWriter::write_raw(std::string_view text) {
    begin_content();
    out_->write(text);
}

void XmlTextWriter::write_whitespace(std::string_view whitespace) {
    for (char c : whitespace)
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') throw UsageError("XmlTextWriter: only white space can be written as white space");
    begin_content();
    out_->write(whitespace);
}

void XmlTextWriter::write_cdata(std::string_view text) {
    begin_content();
    out_->write("<![CDATA[");
    out_->write(text);
    out_->write("]]>");
}

void XmlTextWriter::write_comment(std::string_view text) {
    if (state_ == State::Closed) throw UsageError("XmlTextWriter: the writer is closed");
    if (state_ == State::Element) close_start_tag(false);
    if (formatting_ == XmlFormatting::Indented && state_ != State::Start && (stack_.empty() || !stack_.back().mixed)) write_indent(stack_.size());
    out_->write("<!--");
    out_->write(text);
    out_->write("-->");
}

void XmlTextWriter::write_processing_instruction(std::string_view name, std::string_view text) {
    if (state_ == State::Closed) throw UsageError("XmlTextWriter: the writer is closed");
    if (state_ == State::Element) close_start_tag(false);
    if (formatting_ == XmlFormatting::Indented && state_ != State::Start && (stack_.empty() || !stack_.back().mixed)) write_indent(stack_.size());
    out_->write("<?");
    out_->write(name);
    if (!text.empty()) {
        out_->write_char(' ');
        out_->write(text);
    }
    out_->write("?>");
}

void XmlTextWriter::write_element_string(std::string_view name, std::string_view value) {
    write_start_element(name);
    if (!value.empty()) write_string(value);
    write_end_element();
}

void XmlTextWriter::write_end_element() { end_element(false); }
void XmlTextWriter::write_full_end_element() { end_element(true); }

void XmlTextWriter::end_element(bool full) {
    if (stack_.empty() || state_ == State::Closed) throw UsageError("XmlTextWriter: no element is open");
    if (state_ == State::Element) {
        if (full) {
            close_start_tag(false);
        } else {
            close_start_tag(true);
            stack_.pop_back();
            state_ = stack_.empty() ? (fragment_ ? State::Start : State::Epilog) : State::Content;
            return;
        }
    }
    const Open& top = stack_.back();
    if (formatting_ == XmlFormatting::Indented && !top.mixed) write_indent(stack_.size() - 1);
    out_->write("</");
    out_->write(top.name);
    out_->write_char('>');
    stack_.pop_back();
    state_ = stack_.empty() ? (fragment_ ? State::Start : State::Epilog) : State::Content;
}

void XmlTextWriter::end_all() {
    while (!stack_.empty()) end_element(false);
}

void XmlTextWriter::close() {
    if (state_ == State::Closed) return;
    try {
        end_all();
    } catch (...) {
    }
    state_ = State::Closed;
    out_->close();
}

}  // namespace cb::dotnet::decompiler::util
