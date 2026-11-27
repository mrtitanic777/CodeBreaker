#include "dotnet/decompiler/util/xml_linq.hpp"

#include "core/bytes.hpp"

namespace cb::dotnet::decompiler::util {

std::string XElement::local_name() const {
    const std::size_t colon = name_.find(':');
    return colon == std::string::npos ? name_ : name_.substr(colon + 1);
}

const XAttribute* XElement::attribute(std::string_view name) const {
    for (const XAttribute& a : attributes_)
        if (a.name() == name) return &a;
    return nullptr;
}

std::vector<const XNode*> XElement::nodes() const {
    std::vector<const XNode*> result;
    result.reserve(children_.size());
    for (const auto& child : children_) result.push_back(child.get());
    return result;
}

const XElement* XElement::element(std::string_view name) const {
    for (const auto& child : children_)
        if (const XElement* e = as_xelement(child.get()); e != nullptr && e->name() == name) return e;
    return nullptr;
}

void XElement::add(std::unique_ptr<XNode> node) {
    node->parent_ = this;
    children_.push_back(std::move(node));
}

const XElement* XDocument::element(std::string_view name) const { return root_ && root_->name() == name ? root_.get() : nullptr; }

XDocument XDocument::parse(std::string_view text) {
    XmlReaderOptions options;
    options.normalize = true;
    XmlTextReader reader(as_bytes(text), options);
    XDocument document;
    std::vector<XElement*> open;
    std::vector<bool> preserve;   // per open element: white space is significant (xml:space="preserve")
    std::string pending_text;
    auto flush_text = [&] {
        if (pending_text.empty() || open.empty()) {
            pending_text.clear();
            return;
        }
        open.back()->add(std::make_unique<XText>(std::move(pending_text)));
        pending_text.clear();
    };
    while (reader.read()) {
        switch (reader.node_type()) {
        case XmlNodeType::Element: {
            flush_text();
            auto element = std::make_unique<XElement>(reader.name());
            bool keep = !preserve.empty() && preserve.back();
            for (const auto& [name, value] : reader.attributes()) {
                element->add_attribute(XAttribute(name, value));
                if (name == "xml:space") keep = value == "preserve";
            }
            XElement* raw = element.get();
            if (open.empty()) {
                document.root_ = std::move(element);
            } else {
                open.back()->add(std::move(element));
            }
            if (!reader.is_empty_element()) {
                open.push_back(raw);
                preserve.push_back(keep);
            }
            break;
        }
        case XmlNodeType::EndElement:
            flush_text();
            open.pop_back();
            preserve.pop_back();
            break;
        case XmlNodeType::Text: pending_text += reader.value(); break;
        case XmlNodeType::Whitespace:
            if (!preserve.empty() && preserve.back()) pending_text += reader.value();
            break;
        case XmlNodeType::CDATA:
            flush_text();
            if (!open.empty()) open.back()->add(std::make_unique<XText>(reader.value(), true));
            break;
        default: break;
        }
    }
    return document;
}

}  // namespace cb::dotnet::decompiler::util
