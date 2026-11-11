#include "cb/decompile/ctree.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::ctree {

namespace {

bool is_word(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

bool is_alpha_(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// The decompiler's own type names (decompile/c_text HEADER) and C's. The grammar does not know which names are
// types, so `(uint4)*p` reads as `(uint4) * p`, a product, `(uint2)&x` as a bitwise and, and `(uint2)(uint1)*p` as
// a call: misread() tells such a node from the cast it is.
#define CB_CTREE_TYPE                                                                                    \
    R"re((?:u?int\d+|xunknown\d+|undefined\d*|unk(?:byte|int|uint)\d+|float\d+|code(?:_callee_pops)?|bool|char|)re" \
    R"re(short|int|long|unsigned|signed|double|float|void))re"

// A string that is nothing but casts: `(uint4)`, `(uint2)(uint1)`, `(xunknown1 [4])`, `(char __far *)`.
const Regex& casts_pattern() {
    static const Regex re(R"re((?:\(\s*)re" CB_CTREE_TYPE R"re((?:\s+)re" CB_CTREE_TYPE
                          R"re()*(?:\s*(?:__far|__near))?(?:\s*\*+)?(?:\s*\[\d*\])?\s*\)\s*)+)re");
    return re;
}

// One type name, whole.
const Regex& type_pattern() {
    static const Regex re(CB_CTREE_TYPE);
    return re;
}

bool is_qualifier_word(std::string_view w) {
    static constexpr std::string_view kExact[] = {"__far",    "__near",     "__huge",     "__export",
                                                  "__loadds", "__saveregs", "__interrupt", "__thiscall",
                                                  "__fastcall", "_Packed"};
    static constexpr std::string_view kPrefixes[] = {"__cdecl", "__stdcall", "__pascal"};   // then any word characters
    for (std::string_view e : kExact) {
        if (w == e) return true;
    }
    for (std::string_view p : kPrefixes) {
        if (w.starts_with(p)) return true;
    }
    return false;
}

bool is_blank(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v'; }

}  // namespace

std::vector<const Node*> Node::named_children() const {
    std::vector<const Node*> out;
    for (const Node* c : children) {
        if (c->is_named) out.push_back(c);
    }
    return out;
}

const Node* Node::child_by_field_name(std::string_view name) const {
    for (const auto& [field, child] : fields) {
        if (field == name) return child;
    }
    return nullptr;
}

Node& Tree::add(std::string type, std::uint32_t start_byte, std::uint32_t end_byte, bool is_named, Node* parent) {
    Node& n = nodes_.emplace_back();
    n.type = std::move(type);
    n.start_byte = start_byte;
    n.end_byte = end_byte;
    n.is_named = is_named;
    n.parent = parent;
    if (parent) parent->children.push_back(&n);
    return n;
}

void Tree::set_field(Node& parent, std::string field, const Node& child) {
    parent.fields.emplace_back(std::move(field), &child);
}

const Node& Tree::root() const {
    CB_ASSERT(!nodes_.empty(), "ctree: the parser returned an empty tree");
    return nodes_.front();
}

std::string mask_for_parser(std::string_view text) {
    std::string masked(text);
    // 1. the qualifiers: whole words (a word is a run of [A-Za-z0-9_]), blanked to spaces of the same length
    for (std::size_t i = 0; i < masked.size();) {
        if (!is_word(masked[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < masked.size() && is_word(masked[end])) ++end;
        if (is_qualifier_word(std::string_view(masked).substr(i, end - i))) std::fill(masked.begin() + i, masked.begin() + end, ' ');
        i = end;
    }
    // 2. `L:` at the end of a block: a label that is the first thing on its line, then the `}` closing the block
    //    (anything but whitespace between them is something else). `case` and `default` are not labels.
    for (std::size_t i = 0; i < masked.size();) {
        if (!is_alpha_(masked[i]) || (i > 0 && is_word(masked[i - 1]))) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < masked.size() && is_word(masked[end])) ++end;
        const std::string_view word = std::string_view(masked).substr(i, end - i);
        std::size_t back = i;                                   // first on its line: only blanks before it
        while (back > 0 && is_blank(masked[back - 1])) --back;
        const bool first_on_line = back == 0 || masked[back - 1] == '\n';
        std::size_t colon = end;
        while (colon < masked.size() && text::is_space(masked[colon])) ++colon;
        std::size_t after = colon + 1;
        while (after < masked.size() && text::is_space(masked[after])) ++after;
        if (first_on_line && word != "case" && word != "default" && colon < masked.size() && masked[colon] == ':' &&
            after < masked.size() && masked[after] == '}') {
            masked[colon] = ';';
            end = colon + 1;
        }
        i = end;
    }
    return masked;
}

Source::Source(std::string_view text, const Parser& parser) : data_(text) {
    tree_ = parser(as_bytes(mask_for_parser(text)));
    (void)tree_.root();   // (an empty tree is the parser's bug: fail here, not in the first query)
}

std::string_view Source::slice(std::uint32_t start, std::uint32_t end) const {
    if (start >= data_.size() || end <= start) return {};
    return std::string_view(data_).substr(start, std::min<std::size_t>(end, data_.size()) - start);
}

std::string_view Source::at(const Node& node) const { return slice(node.start_byte, node.end_byte); }

std::vector<const Node*> Source::find(std::span<const std::string_view> types, const Node* under) const {
    std::vector<const Node*> out, todo{under ? under : &root()};
    while (!todo.empty()) {
        const Node* n = todo.back();
        todo.pop_back();
        if (std::find(types.begin(), types.end(), n->type) != types.end()) out.push_back(n);
        todo.insert(todo.end(), n->children.rbegin(), n->children.rend());
    }
    return out;
}

std::vector<const Node*> Source::find(std::initializer_list<std::string_view> types, const Node* under) const {
    return find(std::span<const std::string_view>(types.begin(), types.size()), under);
}

namespace {

// The function_declarator inside a function_definition's declarator chain (through the pointers it may return).
const Node* function_declarator(const Node& definition) {
    const Node* dcl = definition.child_by_field_name("declarator");
    while (dcl && dcl->type != "function_declarator") dcl = dcl->child_by_field_name("declarator");
    return dcl;
}

}  // namespace

const Node* Source::body() const {
    auto defs = find({"function_definition"});
    return defs.empty() ? nullptr : defs.front()->child_by_field_name("body");
}

const Node* Source::function_name() const {
    auto defs = find({"function_definition"});
    if (defs.empty()) return nullptr;
    const Node* dcl = function_declarator(*defs.front());
    const Node* name = dcl ? dcl->child_by_field_name("declarator") : nullptr;
    return name && name->type == "identifier" ? name : nullptr;
}

const Node* Source::parameter_list() const {
    auto defs = find({"function_definition"});
    if (defs.empty()) return nullptr;
    const Node* dcl = function_declarator(*defs.front());
    return dcl ? dcl->child_by_field_name("parameters") : nullptr;
}

std::optional<std::vector<std::string>> Source::parameters() const {
    const Node* plist = parameter_list();
    if (!plist) return std::nullopt;
    const std::string_view inner = slice(plist->start_byte + 1, plist->end_byte - 1);
    std::vector<std::string> out;
    if (inner.find('(') == std::string_view::npos) {
        for (std::string_view piece : text::split(inner, ',')) out.emplace_back(text::trim(piece));
    } else {
        // a parameter with parentheses in it (a function pointer): the grammar's own split
        for (const Node* c : plist->named_children()) out.emplace_back(at(*c));
    }
    return out;
}

const Node* Source::leading(const Node& node, std::string_view kind) const {
    const Node* n = &node;
    while (n && n->start_byte == node.start_byte) {
        if (n->type == kind) return n;
        n = n->children.empty() ? nullptr : n->children.front();
    }
    return nullptr;
}

bool Source::is_call(const Node& call) const {
    const Node* fn = call.child_by_field_name("function");
    if (!fn) return false;
    const std::string_view f = at(*fn);
    return !(casts_pattern().full_match(f) || type_pattern().full_match(text::trim(f)));
}

std::optional<std::string_view> Source::misread(const Node& node) const {
    if (node.type != "binary_expression") return std::nullopt;
    const Node* op = node.child_by_field_name("operator");
    const Node* left = node.child_by_field_name("left");
    if (!op || !left) return std::nullopt;
    if ((op->type == "*" || op->type == "&" || op->type == "-" || op->type == "+") &&
        casts_pattern().full_match(at(*left))) {
        return std::string_view(op->type);
    }
    return std::nullopt;
}

std::optional<Source::UnaryOp> Source::unary(const Node& node) const {
    const Node* up = node.parent;
    if (!up) return std::nullopt;
    if ((up->type == "pointer_expression" || up->type == "unary_expression") &&
        up->child_by_field_name("argument") == &node) {
        if (const Node* op = up->child_by_field_name("operator")) return UnaryOp{at(*op), op->start_byte};
        return std::nullopt;
    }
    if (misread(*up) && up->child_by_field_name("right") == &node) {
        const Node* op = up->child_by_field_name("operator");
        return UnaryOp{at(*op), op->start_byte};
    }
    return std::nullopt;
}

Source::Span Source::line_span(const Node& node) const {
    const std::size_t from = std::min<std::size_t>(node.start_byte, data_.size());
    const std::size_t before = from == 0 ? std::string::npos : data_.rfind('\n', from - 1);
    const std::size_t s = before == std::string::npos ? 0 : before + 1;
    const std::size_t e = data_.find('\n', from);
    return {static_cast<std::uint32_t>(s), static_cast<std::uint32_t>(e == std::string::npos ? data_.size() : e + 1)};
}

std::string_view Source::line(const Node& node) const {
    const Span span = line_span(node);
    std::string_view s = slice(span.start, span.end);
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.remove_suffix(1);
    return s;
}

std::vector<const Node*> Source::ancestors(const Node& node, std::span<const std::string_view> types) const {
    std::vector<const Node*> out;
    for (const Node* up = node.parent; up; up = up->parent) {
        if (types.empty() || std::find(types.begin(), types.end(), up->type) != types.end()) out.push_back(up);
    }
    return out;
}

std::vector<const Node*> Source::ancestors(const Node& node, std::initializer_list<std::string_view> types) const {
    return ancestors(node, std::span<const std::string_view>(types.begin(), types.size()));
}

std::vector<std::string_view> Source::raw_arguments(const Node& call) const {
    std::vector<std::string_view> out;
    const Node* args = call.child_by_field_name("arguments");
    if (!args) return out;
    std::vector<const Node*> delims;
    for (const Node* c : args->children) {
        if (c->type == "(" || c->type == "," || c->type == ")") delims.push_back(c);
    }
    for (std::size_t i = 0; i + 1 < delims.size(); ++i) {
        const std::string_view piece = slice(delims[i]->end_byte, delims[i + 1]->start_byte);
        if (delims[i + 1]->type == ")" && text::trim(piece).empty()) continue;   // (nothing after the last comma, or at all)
        out.push_back(piece);
    }
    return out;
}

std::optional<std::string_view> Source::declared_name(const Node& declarator) const {
    const Node* n = &declarator;
    while (n && (n->type == "pointer_declarator" || n->type == "array_declarator" || n->type == "init_declarator" ||
                 n->type == "parenthesized_declarator")) {
        const Node* inner = n->child_by_field_name("declarator");
        if (!inner) {
            auto named = n->named_children();
            inner = named.empty() ? nullptr : named.front();
        }
        n = inner;
    }
    if (n && n->type == "identifier") return at(*n);
    return std::nullopt;
}

std::string Source::render(const Node& node, const std::function<std::optional<std::string>(const Node&)>& override_) const {
    std::string out;
    std::uint32_t pos = node.start_byte;
    for (const Node* c : node.children) {
        std::optional<std::string> replacement = override_(*c);
        if (!replacement && !c->child_count()) continue;
        out.append(slice(pos, c->start_byte));
        out.append(replacement ? *replacement : render(*c, override_));
        pos = c->end_byte;
    }
    out.append(slice(pos, node.end_byte));
    return out;
}

bool Source::in_text(const Node& node) {
    for (const Node* n = &node; n; n = n->parent) {
        if (n->type == "string_literal" || n->type == "char_literal" || n->type == "comment" ||
            n->type == "system_lib_string") {
            return true;
        }
    }
    return false;
}

void Source::replace(const Node& node, std::string text) { replace(Span{node.start_byte, node.end_byte}, std::move(text)); }

void Source::replace(Span span, std::string text) {
    if (span.start > span.end || span.end > data_.size()) {
        throw UsageError("ctree: replace() outside the text: bytes " + std::to_string(span.start) + ".." +
                         std::to_string(span.end) + " of " + std::to_string(data_.size()));
    }
    edits_.push_back({span.start, span.end, std::move(text)});
}

void Source::insert(std::uint32_t at, std::string text) { replace(Span{at, at}, std::move(text)); }

std::string Source::text() const {
    if (edits_.empty()) return data_;
    std::vector<const Edit*> order;
    for (const Edit& e : edits_) order.push_back(&e);
    std::stable_sort(order.begin(), order.end(), [](const Edit* a, const Edit* b) {
        return a->start != b->start ? a->start < b->start : a->end < b->end;
    });
    std::string out;
    std::size_t pos = 0;
    for (const Edit* e : order) {
        if (e->start < pos) throw UsageError("ctree: overlapping edits at byte " + std::to_string(e->start));
        out.append(data_, pos, e->start - pos);
        out.append(e->text);
        pos = e->end;
    }
    out.append(data_, pos, std::string::npos);
    return out;
}

std::string rewrite(std::string_view text, const Parser& parser, const std::function<void(Source&)>& fn) {
    Source src(text, parser);
    fn(src);
    return src.text();
}

}  // namespace cb::decompile::ctree
