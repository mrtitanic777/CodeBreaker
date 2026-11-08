// codebreaker/decompile/ctree.cpp -- typed C++ port of codebreaker.decompile.ctree.

#include "codebreaker/decompile/ctree.hpp"

#include <algorithm>

namespace codebreaker::decompile::ctree {

// ---------------------------------------------------------------------------------------------- Node
py::ref<Node> Node::parent() const { return parent_.lock(); }

int64_t Node::child_count() const { return children.size(); }

py::list<py::ref<Node>> Node::named_children() const {
    py::list<py::ref<Node>> out;
    for (const py::ref<Node>& c : children) {
        if (c->is_named) {
            out.append(c);
        }
    }
    return out;
}

py::ref<Node> Node::child_by_field_name(const py::str& name) const {
    return fields.get(name);  // (null when the node has no such field)
}

void link_parents(const NodeP& root) {
    for (const NodeP& c : root->children) {
        c->parent_ = root;
        link_parents(c);
    }
}

// ---------------------------------------------------------------------------------------------- patterns
const py::str _TYPE(
    "(?:u?int\\d+|xunknown\\d+|undefined\\d*|unk(?:byte|int|uint)\\d+|float\\d+|code(?:_callee_pops)?|bool|char|"
    "short|int|long|unsigned|signed|double|float|void)");

// (the Python's bytes patterns; run as ASCII patterns over the bytes read as Latin-1)
const py::str _QUALIFIERS_PATTERN(
    "\\b(?:__(?:far|near|huge|export|loadds|saveregs|interrupt|cdecl\\w*|stdcall\\w*|pascal\\w*|"
    "thiscall|fastcall)|_Packed)\\b");
const py::str _LAST_LABEL_PATTERN("(?m)^(\\s*(?!case\\b|default\\b)[A-Za-z_]\\w*\\s*):(?=\\s*\\})");

const py::str _CASTS_PATTERN = py::percent(
    py::str("(?:\\(\\s*%s(?:\\s+%s)*(?:\\s*(?:__far|__near))?(?:\\s*\\*+)?(?:\\s*\\[\\d*\\])?\\s*\\)\\s*)+\\Z"),
    _TYPE, _TYPE);

// ---------------------------------------------------------------------------------------------- the parser hook
namespace {
Parser& installed_parser() {
    static Parser p;
    return p;
}

// Python's AttributeError for `None.<attribute>`: what a missing field child gives when it is used.
const NodeP& need(const NodeP& n) {
    if (!n) {
        throw py::AttributeError("'NoneType' object has no attribute");
    }
    return n;
}
}  // namespace

void set_parser(const Parser& parser) { installed_parser() = parser; }

const Parser& _parser() {
    if (!installed_parser()) {
        throw py::RuntimeError("no tree-sitter C parser is installed (ctree::set_parser)");
    }
    return installed_parser();
}

// ---------------------------------------------------------------------------------------------- Source
Source::Source(const py::str& text) {
    original = text;
    data = text.encode("utf-8");
    // (parsed with the 16-bit qualifiers and the calling conventions blanked, byte for byte:
    // the grammar has no word for `__far`, and took `(char __far *)` for a cast to a type
    // named __far; every place in the tree is still a place in the text as written)
    // A label at the end of a block (`L:` then `}`) is not C -- a label needs a statement -- and
    // the grammar's recovery took what came after it for something else; read as `L;`.
    py::str latin = data.decode("latin1");
    py::str masked = py::re::sub(_QUALIFIERS_PATTERN,
                                 [](const py::re::Match& m) { return py::repeat(py::str(" "), m.group(0).size()); },
                                 latin, 0, py::re::ASCII);
    masked = py::re::sub(_LAST_LABEL_PATTERN, [](const py::re::Match& m) { return m.group(1) + py::str(";"); }, masked,
                         0, py::re::ASCII);
    tree = _parser()(masked.encode("latin1"));
    root = tree;
    edits = py::list<std::tuple<int64_t, int64_t, py::bytes>>();
}

py::str Source::at(const NodeP& node) const {
    need(node);
    return data.slice(node->start_byte, node->end_byte).decode("utf-8");
}

py::list<NodeP> Source::find(const py::list<py::str>& types, const NodeP& under) const {
    py::list<NodeP> out;
    py::list<NodeP> todo;
    todo.append(under ? under : root);
    while (!todo.empty()) {
        NodeP n = todo.pop();
        if (types.contains(n->type)) {
            out.append(n);
        }
        for (int64_t i = n->children.size() - 1; i >= 0; --i) {  // (extend(reversed(children)))
            todo.append(n->children[i]);
        }
    }
    return out;
}

NodeP Source::body() const {
    for (const NodeP& n : find(py::list<py::str>{py::str("function_definition")})) {
        return n->child_by_field_name(py::str("body"));
    }
    return nullptr;
}

NodeP Source::function_name() const {
    for (const NodeP& n : find(py::list<py::str>{py::str("function_definition")})) {
        NodeP dcl = n->child_by_field_name(py::str("declarator"));
        while (dcl && dcl->type != py::str("function_declarator")) {
            dcl = dcl->child_by_field_name(py::str("declarator"));
        }
        NodeP name = dcl ? dcl->child_by_field_name(py::str("declarator")) : nullptr;
        return (name && name->type == py::str("identifier")) ? name : nullptr;
    }
    return nullptr;
}

NodeP Source::parameter_list() const {
    for (const NodeP& n : find(py::list<py::str>{py::str("function_definition")})) {
        NodeP dcl = n->child_by_field_name(py::str("declarator"));
        while (dcl && dcl->type != py::str("function_declarator")) {
            dcl = dcl->child_by_field_name(py::str("declarator"));
        }
        return dcl ? dcl->child_by_field_name(py::str("parameters")) : nullptr;
    }
    return nullptr;
}

std::optional<py::list<py::str>> Source::parameters() const {
    NodeP plist = parameter_list();
    if (!plist) {
        return std::nullopt;
    }
    py::str inner = data.slice(plist->start_byte + 1, plist->end_byte - 1).decode("utf-8");
    py::list<py::str> out;
    if (!inner.contains(py::str("("))) {
        for (const py::str& x : py::split(inner, py::str(","))) {
            out.append(x.strip());
        }
    } else {
        for (const NodeP& c : plist->named_children()) {
            out.append(at(c));
        }
    }
    return out;
}

NodeP Source::leading(const NodeP& node, const py::str& kind) const {
    NodeP n = node;
    while (n && n->start_byte == node->start_byte) {
        if (n->type == kind) {
            return n;
        }
        n = !n->children.empty() ? n->children[0] : nullptr;
    }
    return nullptr;
}

bool Source::is_call(const NodeP& node) const {
    py::str f = at(node->child_by_field_name(py::str("function")));
    return !(py::re::match(_CASTS_PATTERN, f).has_value() || py::re::fullmatch(_TYPE, f.strip()).has_value());
}

std::optional<py::str> Source::misread(const NodeP& node) const {
    if (node->type != py::str("binary_expression")) {
        return std::nullopt;
    }
    py::str op = need(node->child_by_field_name(py::str("operator")))->type;
    if ((op == py::str("*") || op == py::str("&") || op == py::str("-") || op == py::str("+")) &&
        py::re::match(_CASTS_PATTERN, at(node->child_by_field_name(py::str("left")))).has_value()) {
        return op;
    }
    return std::nullopt;
}

std::optional<std::tuple<py::str, int64_t>> Source::unary(const NodeP& node) const {
    NodeP up = node->parent();
    if (!up) {
        return std::nullopt;
    }
    if ((up->type == py::str("pointer_expression") || up->type == py::str("unary_expression")) &&
        up->child_by_field_name(py::str("argument")) == node) {
        NodeP op = need(up->child_by_field_name(py::str("operator")));
        return std::make_tuple(at(op), op->start_byte);
    }
    if (misread(up).has_value() && up->child_by_field_name(py::str("right")) == node) {
        NodeP op = need(up->child_by_field_name(py::str("operator")));
        return std::make_tuple(at(op), op->start_byte);
    }
    return std::nullopt;
}

py::str Source::line(const NodeP& node) const {
    auto [s, e] = line_span(node);
    py::str text = data.slice(s, e).decode("utf-8");
    return text.rstrip(py::str("\r\n"));
}

std::tuple<int64_t, int64_t> Source::line_span(const NodeP& node) const {
    int64_t s = data.rfind(py::bytes("\n"), 0, node->start_byte) + 1;
    int64_t e = data.find(py::bytes("\n"), node->start_byte);
    return std::make_tuple(s, e < 0 ? data.size() : e + 1);
}

py::list<NodeP> Source::ancestors(const NodeP& node, const py::list<py::str>& types) const {
    py::list<NodeP> out;
    NodeP up = node->parent();
    while (up) {
        if (types.empty() || types.contains(up->type)) {
            out.append(up);
        }
        up = up->parent();
    }
    return out;
}

py::list<py::str> Source::raw_arguments(const NodeP& call) const {
    NodeP args = need(call->child_by_field_name(py::str("arguments")));
    py::list<NodeP> delims;
    for (const NodeP& c : args->children) {
        if (c->type == py::str("(") || c->type == py::str(",") || c->type == py::str(")")) {
            delims.append(c);
        }
    }
    py::list<py::str> out;
    for (int64_t i = 0; i + 1 < delims.size(); ++i) {
        const NodeP& a = delims[i];
        const NodeP& b = delims[i + 1];
        py::str piece = data.slice(a->end_byte, b->start_byte).decode("utf-8");
        if (b->type == py::str(")") && piece.strip().size() == 0) {
            continue;  // (no argument after the last comma, or none at all)
        }
        out.append(piece);
    }
    return out;
}

std::optional<py::str> Source::declared_name(const NodeP& declarator) const {
    NodeP n = declarator;
    while (n && (n->type == py::str("pointer_declarator") || n->type == py::str("array_declarator") ||
                 n->type == py::str("init_declarator") || n->type == py::str("parenthesized_declarator"))) {
        NodeP next = n->child_by_field_name(py::str("declarator"));
        if (!next) {
            py::list<NodeP> named = n->named_children();
            next = !named.empty() ? named[0] : nullptr;
        }
        n = next;
    }
    if (n && n->type == py::str("identifier")) {
        return at(n);
    }
    return std::nullopt;
}

py::str Source::render(const NodeP& node, const std::function<std::optional<py::str>(const NodeP&)>& override_) const {
    py::list<py::str> out;
    int64_t pos = node->start_byte;
    for (const NodeP& c : node->children) {
        std::optional<py::str> replacement = override_(c);
        if (!replacement.has_value()) {
            if (c->child_count()) {
                out.append(data.slice(pos, c->start_byte).decode("utf-8"));
                out.append(render(c, override_));
                pos = c->end_byte;
            }
            continue;
        }
        out.append(data.slice(pos, c->start_byte).decode("utf-8"));
        out.append(*replacement);
        pos = c->end_byte;
    }
    out.append(data.slice(pos, node->end_byte).decode("utf-8"));
    return py::join(py::str(""), out);
}

bool Source::in_text(NodeP node) {
    while (node) {
        const py::str& t = node->type;
        if (t == py::str("string_literal") || t == py::str("char_literal") || t == py::str("comment") ||
            t == py::str("system_lib_string")) {
            return true;
        }
        node = node->parent();
    }
    return false;
}

void Source::replace(const NodeP& node, const py::str& text) {
    edits.append(std::make_tuple(node->start_byte, node->end_byte, text.encode("utf-8")));
}

void Source::replace(const std::tuple<int64_t, int64_t>& span, const py::str& text) {
    edits.append(std::make_tuple(std::get<0>(span), std::get<1>(span), text.encode("utf-8")));
}

void Source::insert(int64_t at, const py::str& text) { edits.append(std::make_tuple(at, at, text.encode("utf-8"))); }

py::str Source::text() const {
    if (edits.empty()) {
        return original;
    }
    std::vector<std::tuple<int64_t, int64_t, py::bytes>> sorted = edits.vec();
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return std::make_tuple(std::get<0>(a), std::get<1>(a)) < std::make_tuple(std::get<0>(b), std::get<1>(b));
    });
    py::bytes out;
    int64_t pos = 0;
    for (const auto& [s, e, t] : sorted) {
        if (s < pos) {
            throw py::ValueError(py::percent(py::str("overlapping edits at byte %d"), s));
        }
        out = out + data.slice(pos, s);
        out = out + t;
        pos = e;
    }
    out = out + data.slice(pos, data.size());
    return out.decode("utf-8");
}

py::str rewrite(const py::str& text, const std::function<void(Source&)>& fn) {
    Source src(text);
    fn(src);
    return src.text();
}

}  // namespace codebreaker::decompile::ctree
