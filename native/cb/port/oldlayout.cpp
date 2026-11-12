// cb/port/oldlayout.cpp -- see oldlayout.hpp.

#include "cb/port/oldlayout.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "cb/base/cache_codec.hpp"
#include "cb/base/ctext.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::port::oldlayout {

namespace {
namespace ol {

using arith16::FileId;
using arith16::Kind;
using arith16::Node;
using arith16::Op;
using arith16::CastKind;

using Parents = std::vector<const Node*>;   // a node's ancestors, outermost first (the translation unit itself is not one)

// A node standing for "no node": what an expression with no operand is taken for.
const Node& nothing() {
    static const Node none;
    return none;
}

const Node& first_inner(const Node& n) { return n.inner.empty() ? nothing() : n.inner.front(); }

template <class Fn>
void walk(const Node& n, Parents& parents, Fn& fn) {
    fn(n, parents);
    parents.push_back(&n);
    for (const Node& child : n.inner) walk(child, parents, fn);
    parents.pop_back();
}

template <class Fn>
void walk_tree(const arith16::Tree& tree, Fn&& fn) {
    Parents parents;
    for (const Node& top : tree.top()) walk(top, parents, fn);
}

bool is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

std::string without_blanks(std::string_view s) {
    std::string out;
    for (const char c : s)
        if (!text::is_space(c)) out += c;
    return out;
}

// ---- the records ----------------------------------------------------------------------------------------------------

struct RecordInfo {
    std::string tag;                       // "struct expr"; "" for an unnamed one
    std::vector<std::uint64_t> fields;
    std::uint64_t parent = 0;              // the record it is inside, 0 for none
};

struct FieldInfo {
    std::uint64_t record = 0;
    std::string name;
    std::string type;
};

// Lookup only: the ids are clang's.
struct Records {
    std::unordered_map<std::uint64_t, RecordInfo> records;
    std::unordered_map<std::uint64_t, FieldInfo> fields;
};

Records records_of(const arith16::Tree& tree) {
    Records out;
    walk_tree(tree, [&](const Node& n, const Parents& parents) {
        if (n.kind != Kind::RecordDecl || !n.complete) return;
        RecordInfo info;
        for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
            if ((*it)->kind == Kind::RecordDecl) {
                info.parent = (*it)->id;
                break;
            }
        }
        if (!n.name.empty()) info.tag = std::string(n.is_union ? "union " : "struct ") + n.name;
        for (const Node& x : n.inner) {
            if (x.kind != Kind::FieldDecl) continue;
            info.fields.push_back(x.id);
            out.fields[x.id] = {n.id, x.name, x.qual_type};
        }
        out.records[n.id] = std::move(info);
    });
    return out;
}

// The named record a record is inside (itself when named).
std::string top_tag(const Records& r, std::uint64_t id) {
    std::unordered_set<std::uint64_t> seen;
    for (;;) {
        const auto it = r.records.find(id);
        if (it == r.records.end() || !it->second.tag.empty() || it->second.parent == 0 || seen.contains(id)) break;
        seen.insert(id);
        id = it->second.parent;
    }
    const auto it = r.records.find(id);
    return it == r.records.end() ? std::string() : it->second.tag;
}

// "unsigned short int" and its like: a word-sized integer the port made, that a pointer may have been kept in.
bool is_word_int(std::string_view type) {
    std::string_view s = text::trim(type);
    for (const std::string_view prefix : {"unsigned ", "signed "}) {
        if (s.starts_with(prefix)) {
            s.remove_prefix(prefix.size());
            break;
        }
    }
    for (const std::string_view base : {"short", "int", "uint16_t", "int16_t", "intptr_t"}) {
        if (!s.starts_with(base)) continue;
        const std::string_view rest = s.substr(base.size());
        if (rest.empty() || rest == " int") return true;
    }
    return false;
}

// The function a CallExpr calls, through its decay to a pointer and its parentheses; "" when that is not a name.
std::string_view callee_name(const Node* call) {
    if (!call) return {};
    const Node* c = &first_inner(*call);
    while ((c->kind == Kind::ImplicitCastExpr || c->kind == Kind::ParenExpr) && !c->inner.empty()) c = &c->inner.front();
    return c->kind == Kind::DeclRefExpr ? std::string_view(c->name) : std::string_view();
}

// read, write, fread, fwrite, or those with a "dos_" before.
bool is_io_call(std::string_view name) {
    if (name.starts_with("dos_")) name.remove_prefix(4);
    return name == "read" || name == "write" || name == "fread" || name == "fwrite";
}

const Node* enclosing_call(const Parents& parents) {
    for (auto it = parents.rbegin(); it != parents.rend(); ++it)
        if ((*it)->kind == Kind::CallExpr) return *it;
    return nullptr;
}

// ---- places ---------------------------------------------------------------------------------------------------------

// A '(' next, within 40 bytes (blanks between): the name before it is a macro that takes arguments.
bool paren_follows(std::string_view text, std::uint64_t end) {
    if (end > text.size()) return false;
    for (const char c : text.substr(static_cast<std::size_t>(end), 40)) {
        if (c == '(') return true;
        if (!text::is_space(c)) return false;
    }
    return false;
}

struct Place {
    std::uint64_t start = 0;
    std::uint64_t end = 0;
};

// Where an expression is in the file `path`: its first token where it is written, its last where the macro that gave
// it is used (e->e_sym, e_sym a macro). Nothing when it is not all in the file.
std::optional<Place> place_of(const Node& n, FileId path, std::string_view text) {
    struct Spot {
        std::optional<std::uint64_t> offset;
        std::uint64_t len = 0;
        FileId file = arith16::kNoFile;
    };
    const auto spot = [](const arith16::Loc& x) -> Spot {
        if (x.expansion) {
            const arith16::Loc& e = *x.expansion;
            return {e.offset, e.tok_len, e.file != arith16::kNoFile ? e.file : x.file};
        }
        if (x.has_spelling) return {};
        return {x.offset, x.tok_len, x.file};
    };
    const arith16::Loc& b = n.range.begin;
    const Spot first = spot(b);
    const Spot last = spot(n.range.end);
    if (!first.offset || !last.offset || first.file != path || last.file != path || *last.offset < *first.offset) return std::nullopt;
    if (*first.offset == *last.offset && b.expansion && paren_follows(text, *last.offset + last.len)) return std::nullopt;   // (all inside a macro with arguments)
    return Place{*first.offset, *last.offset + last.len};
}

// ---- reading the tree -------------------------------------------------------------------------------------------------

class Analyser {
public:
    Analyser(const arith16::Tree& tree, std::string_view text, FileId path, const RecordSet& olds, const PointerParams& params)
        : text_(text), path_(path), params_(params), records_(records_of(tree)) {
        for (const auto& [id, f] : records_.fields) {
            if (!olds.contains(top_tag(records_, f.record))) continue;
            if (text::rtrim(f.type).ends_with('*') && f.type.find('(') == std::string::npos) pointer_fields_[id] = f.type;
            if (is_word_int(f.type)) word_fields_.insert(id);
        }
    }

    Analysis run(const arith16::Tree& tree) {
        walk_tree(tree, [&](const Node& n, const Parents& parents) {
            if (n.kind == Kind::MemberExpr) member(n, parents);
        });
        return std::move(result_);
    }

private:
    void open(std::uint64_t at, std::size_t depth, std::string text) {
        result_.splices.push_back({at, false, static_cast<std::int32_t>(depth), std::move(text)});
    }
    void close(std::uint64_t at, std::size_t depth, std::string text) {
        result_.splices.push_back({at, true, static_cast<std::int32_t>(depth), std::move(text)});
    }
    // dos_hp( ... ) or similar around the places [p.start, p.end)
    void wrap(const Place& p, std::size_t depth, std::string head, std::string tail) {
        open(p.start, depth, std::move(head));
        close(p.end, depth, std::move(tail));
    }

    void member(const Node& n, const Parents& parents) {
        if (word_fields_.contains(n.member_decl)) {
            word_member(n, parents);
            return;
        }
        const auto field = pointer_fields_.find(n.member_decl);
        if (field == pointer_fields_.end()) return;
        const std::optional<Place> p = place_of(n, path_, text_);
        if (!p) return;
        const std::string& type = field->second;
        const FieldInfo& info = records_.fields.at(n.member_decl);
        result_.found[{top_tag(records_, info.record), info.name}] = type;
        const std::size_t depth = parents.size();
        const Node* par = parents.empty() ? nullptr : parents.back();
        const Node& parent = par ? *par : nothing();
        if (par && parent.kind == Kind::BinaryOperator && parent.op == Op::Assign && &first_inner(parent) == &n) {
            // a write: the handle of what is given
            const Node& rhs = parent.inner.size() > 1 ? parent.inner[1] : nothing();
            if (const std::optional<Place> q = place_of(rhs, path_, text_)) {
                wrap(*q, depth, "dos_ph(", ")");
                const Node* grand = parents.size() > 1 ? parents[parents.size() - 2] : nullptr;
                const std::optional<Place> w = place_of(parent, path_, text_);
                if (w && value_used(grand, parent)) wrap(*w, depth - 1, std::format("(({})dos_hp(", type), "))");   // (its value used - t = n->left = x: the pointer again, not the handle)
            }
            return;
        }
        if ((par && parent.kind == Kind::UnaryOperator && (parent.op == Op::AddrOf || parent.op == Op::Inc || parent.op == Op::Dec)) ||
            (par && parent.kind == Kind::CompoundAssignOperator && &first_inner(parent) == &n)) {
            result_.refused.push_back({p->start, std::string(arith16::op_text(parent.op))});
            return;
        }
        wrap(*p, depth, std::format("(({})dos_hp(", type), "))");   // a read: the pointer the handle stands for
    }

    // Whether the value of the assignment `assign`, a child of `grand`, is used: it is not where it stands alone as a
    // statement, or as the condition of an if / while / switch / do / for.
    static bool value_used(const Node* grand, const Node& assign) {
        if (!grand) return false;
        switch (grand->kind) {
            case Kind::CompoundStmt:
            case Kind::CaseStmt:
            case Kind::DefaultStmt:
            case Kind::LabelStmt:
                return false;
            case Kind::IfStmt:
            case Kind::WhileStmt:
            case Kind::SwitchStmt:
                return !(grand->inner.size() > 0 && &grand->inner[0] == &assign);
            case Kind::DoStmt:
                return !(grand->inner.size() > 1 && &grand->inner[1] == &assign);
            case Kind::ForStmt:
                return !(grand->inner.size() > 2 && &grand->inner[2] == &assign);
            default:
                return true;
        }
    }

    // An int member of a record kept old given a pointer, or read as one: the handle of it.
    void word_member(const Node& n, const Parents& parents) {
        const Node* par = parents.empty() ? nullptr : parents.back();
        const Node& parent = par ? *par : nothing();
        const std::optional<Place> p = place_of(n, path_, text_);
        if (!p) return;
        const std::size_t depth = parents.size();
        if (par && parent.kind == Kind::BinaryOperator && parent.op == Op::Assign && &first_inner(parent) == &n) {
            const Node* rhs = parent.inner.size() > 1 ? &parent.inner[1] : &nothing();   // (n->i2 = (intptr_t)(f()): to 16 bits too)
            while (((rhs->kind == Kind::ParenExpr) || (rhs->kind == Kind::ImplicitCastExpr && rhs->cast_kind == CastKind::IntegralCast)) &&
                   !rhs->inner.empty())
                rhs = &rhs->inner.front();
            const bool pointer_cast = (rhs->kind == Kind::ImplicitCastExpr || rhs->kind == Kind::CStyleCastExpr) &&
                                      rhs->cast_kind == CastKind::PointerToIntegral;
            if (pointer_cast) {
                std::optional<Place> q = place_of(*rhs, path_, text_);
                if (rhs->kind == Kind::CStyleCastExpr && !rhs->inner.empty()) {   // ((intptr_t)(f()): the pointer inside the cast made the handle
                    if (const std::optional<Place> inside = place_of(rhs->inner.front(), path_, text_)) q = inside;
                }
                if (q) {   // (a pointer given it: its handle)
                    wrap(*q, depth, "dos_ph(", ")");
                    const Node* grand = nullptr;   // (the value of it as a pointer: the pointer again)
                    for (auto it = parents.rbegin() + 1; it != parents.rend(); ++it) {
                        if ((*it)->kind != Kind::ParenExpr) {
                            grand = *it;
                            break;
                        }
                    }
                    const std::optional<Place> w = place_of(parent, path_, text_);
                    if (w && grand && grand->cast_kind == CastKind::IntegralToPointer) wrap(*w, depth - 1, "dos_hp(", ")");
                }
            }
            return;
        }
        const Node* up = nullptr;   // what the member is read for: past the parentheses and plain conversions
        for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
            const Node& x = **it;
            const bool plain = x.kind == Kind::ParenExpr || (x.kind == Kind::ImplicitCastExpr && (x.cast_kind == CastKind::LValueToRValue ||
                                                                                                   x.cast_kind == CastKind::IntegralCast));
            if (!plain) {
                up = &x;
                break;
            }
        }
        if (!up) return;
        if ((up->kind == Kind::ImplicitCastExpr || up->kind == Kind::CStyleCastExpr) && up->cast_kind == CastKind::IntegralToPointer) {
            wrap(*p, depth, "dos_hp(", ")");   // (read as a pointer: what the handle stands for)
            return;
        }
        if (up->kind == Kind::BinaryOperator && up->op == Op::Assign && up->inner.size() == 2 && &up->inner[0] != &n &&
            up->inner[0].kind == Kind::DeclRefExpr) {
            const std::string t = without_blanks(up->inner[0].qual_type);   // (g_74be = n->i2, the variable one the port made hold pointers: the pointer the handle stands for)
            if (t == "intptr_t" || t == "longlong") {
                wrap(*p, depth, "(intptr_t)dos_hp(", ")");
                return;
            }
        }
        if (up->kind == Kind::CallExpr && !params_.empty()) {   // (given to an old-style function's parameter that is a pointer: no conversion said so, the definition's parameter does)
            const auto at = std::find(parents.begin(), parents.end(), up);
            const Node* arg = nullptr;
            if (at != parents.end()) arg = std::next(at) != parents.end() ? *std::next(at) : &n;
            std::size_t k = up->inner.size();
            for (std::size_t i = 1; i < up->inner.size(); ++i) {
                if (&up->inner[i] == arg) {
                    k = i - 1;
                    break;
                }
            }
            const auto ps = params_.find(std::string(callee_name(up)));
            if (k < up->inner.size() && ps != params_.end() && k < ps->second.size() && ps->second[k]) wrap(*p, depth, "dos_hp(", ")");
        }
    }

    std::string_view text_;
    FileId path_;
    const PointerParams& params_;
    Records records_;
    std::unordered_map<std::uint64_t, std::string> pointer_fields_;   // field id -> its pointer type (lookup only)
    std::unordered_set<std::uint64_t> word_fields_;
    Analysis result_;
};

// ---- the definitions ------------------------------------------------------------------------------------------------

// The text of a definition's body from just after its '{' to the '}' that closes it (the end of the text if none).
std::size_t closing_brace(std::string_view text, std::size_t open_at) {
    int depth = 0;
    std::size_t k = open_at;
    for (; k < text.size(); ++k) {
        if (text[k] == '{') ++depth;
        else if (text[k] == '}') --depth;
        if (depth == 0) break;
    }
    return k;
}

// One declaration for each declarator of `struct cse *c_next, *c_prev;`, on its one line still.
std::string split_declarators(std::string_view body) {
    static const Regex several(
        R"(^([ \t]*)((?:struct|union)\s+\w+|\w+(?:\s+\w+)*?)\s*(\**\s*\w+(?:\s*\[[^\]]*\])?(?:\s*,\s*\**\s*\w+(?:\s*\[[^\]]*\])?)+)\s*;)",
        RegexFlags::Multiline);
    return several.replace_all(body, [](const RegexMatch& m) {
        std::string out(m.group(1));
        bool first = true;
        for (const std::string_view declarator : text::split(m.group(3), ',')) {
            if (!first) out += ' ';
            first = false;
            out += std::format("{} {};", m.group(2), text::trim(declarator));
        }
        return out;
    });
}

// A pointer member declared as a handle, in the body of its record.
std::string declare_handle(std::string_view body, const std::string& name, const std::string& type) {
    // The pattern holds the member's name, so it is made for each (a handful per record, found by the pass).
    const Regex pointer(std::format(R"((^[ \t]*|;[ \t]*|\*/[ \t]*)(?:const\s+)?(?:struct\s+|union\s+)?\w+(?:\s+\w+)*\s*\*+\s*{}\s*;)", name),
                        RegexFlags::Multiline);
    return pointer.replace_all(body, [&](const RegexMatch& m) {
        return std::format("{}uint16_t {};\t/* (a handle: {}) */", m.group(1), name, type);
    });
}

// An int the port made as wide as a pointer: one word again.
std::string narrow_words(std::string_view body) {
    static const Regex wide(R"((^[ \t]*|;[ \t]*|\{[ \t]*)intptr_t(\s+\w+\s*;))", RegexFlags::Multiline);
    return wide.replace_all(body, [](const RegexMatch& m) { return std::format("{}int16_t{}\t/* (one word, as it was) */", m.group(1), m.group(2)); });
}

// ---- reading a file of the port ----------------------------------------------------------------------------------------

// A `#include "name"` and the names so included, in the order they stand (a system header, <name>, is not the program's).
std::vector<std::string> quoted_includes(std::string_view text) {
    std::vector<std::string> names;
    for (std::size_t at = text.find('#'); at != std::string_view::npos; at = text.find('#', at + 1)) {
        std::size_t i = at + 1;
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (!text.substr(i).starts_with("include")) continue;
        i += 7;
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (i >= text.size() || text[i] != '"') continue;
        const std::size_t end = text.find('"', i + 1);
        if (end == std::string_view::npos || end == i + 1) continue;
        names.emplace_back(text.substr(i + 1, end - i - 1));
    }
    return names;
}

bool contains_word(std::string_view text, std::string_view word) {
    for (std::size_t at = text.find(word); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const bool before = at == 0 || !is_word_char(text[at - 1]);
        const bool after = at + word.size() >= text.size() || !is_word_char(text[at + word.size()]);
        if (before && after) return true;
    }
    return false;
}

bool mentions_in(const std::filesystem::path& folder, const std::string& file, std::span<const std::string> tags, std::set<std::string>& seen) {
    if (!seen.insert(file).second) return false;
    std::string text;
    try {
        text = fs::read_text(folder / fs::from_text(file));
    } catch (const IoError&) {
        return false;   // (a file that is not there: it names nothing)
    }
    for (const std::string& tag : tags)
        if (contains_word(text, tag)) return true;
    for (const std::string& header : quoted_includes(text))
        if (mentions_in(folder, header, tags, seen)) return true;
    return false;
}

}  // namespace ol
}  // namespace

// ---- what a file's pass keeps in the cache ------------------------------------------------------------------------------

namespace cached {

// What the pass over one file found: its text with the conversions (nothing when it needs none), the pointer members
// it met, and the uses it refused.
struct FileOutcome {
    std::optional<std::string> converted;
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> found;   // (record, (member, type))
    std::vector<std::pair<std::uint64_t, std::string>> refused;                       // (offset, operator)
};

}  // namespace cached
}  // namespace cb::port::oldlayout

namespace cb::base::cache {

template <>
struct Codec<port::oldlayout::cached::FileOutcome> {
    using Type = port::oldlayout::cached::FileOutcome;
    static constexpr std::uint32_t kSchema = 0x6F4C0002;
    static void write(io::ByteWriter& w, const Type& v) {
        Codec<std::optional<std::string>>::write(w, v.converted);
        Codec<decltype(v.found)>::write(w, v.found);
        Codec<decltype(v.refused)>::write(w, v.refused);
    }
    static Type read(io::ByteReader& r) {
        Type v;
        v.converted = Codec<std::optional<std::string>>::read(r);
        v.found = Codec<decltype(v.found)>::read(r);
        v.refused = Codec<decltype(v.refused)>::read(r);
        return v;
    }
};

}  // namespace cb::base::cache

namespace cb::port::oldlayout {

std::string record_tag(std::string_view type) {
    // "const " goes, blanks become one, the ends are trimmed: then `(struct|union) name` and perhaps " *".
    const std::string without_const = text::replace_all(type, "const ", "");
    std::string flat;
    for (const char c : without_const) {
        if (text::is_space(c)) {
            if (flat.empty() || flat.back() != ' ') flat += ' ';
        } else {
            flat += c;
        }
    }
    const std::string_view t = text::trim(flat);
    std::size_t at = 0;
    if (t.starts_with("struct ")) at = 7;
    else if (t.starts_with("union ")) at = 6;
    else return {};
    std::size_t end = at;
    while (end < t.size() && ol::is_word_char(t[end])) ++end;
    if (end == at) return {};
    const std::string_view rest = t.substr(end);
    if (!rest.empty() && rest != " *") return {};
    return std::string(t.substr(0, end));
}

RecordSet byte_records(const arith16::Tree& tree) {
    using namespace ol;
    RecordSet out;
    walk_tree(tree, [&](const Node& n, const Parents& parents) {
        if (n.kind != Kind::ImplicitCastExpr && n.kind != Kind::CStyleCastExpr) return;
        const std::string cast_type = without_blanks(n.qual_type);
        // a record's pointer given to read / write as the bytes to fill or to write
        const auto io_record = [&](const Node& x) {
            const std::string tag = x.qual_type.ends_with('*') ? record_tag(x.qual_type) : std::string();
            if (!tag.empty() && is_io_call(callee_name(enclosing_call(parents)))) out.insert(tag);
        };
        if (cast_type == "void*" || cast_type == "constvoid*") {
            io_record(arith16::bare(first_inner(n)));
            return;
        }
        if (n.kind != Kind::CStyleCastExpr || (cast_type != "char*" && cast_type != "unsignedchar*")) return;
        const Node* x = &arith16::bare(first_inner(n));
        if (x->kind == Kind::ImplicitCastExpr) x = &arith16::bare(first_inner(*x));
        if (x->kind == Kind::UnaryOperator && x->op == Op::AddrOf) {
            const Node* y = &arith16::bare(first_inner(*x));
            if (y->kind == Kind::MemberExpr) {   // (the bytes of one number - (char *)&p->type: its bytes the same in either layout)
                const std::string& mt = y->qual_type;
                if (record_tag(mt).empty() && mt.find('[') == std::string::npos && !mt.starts_with("struct") && !mt.starts_with("union")) return;
            }
            while (y->kind == Kind::MemberExpr) {   // (a member of a record: the record it is in)
                const Node& base = arith16::bare(first_inner(*y));
                const std::string tag = record_tag(base.qual_type);
                if (!tag.empty()) {
                    out.insert(tag);
                    return;
                }
                y = &base;
            }
            const std::string tag = record_tag(y->qual_type);
            if (!tag.empty()) out.insert(tag);
        } else {   // ((char *)h given to read or write: its bytes)
            io_record(*x);
        }
    });
    return out;
}

Analysis analyse(const arith16::Tree& tree, std::string_view text, arith16::FileId main, const RecordSet& olds, const PointerParams& pointer_params) {
    if (main == arith16::kNoFile) return {};
    ol::Analyser analyser(tree, text, main, olds, pointer_params);
    return analyser.run(tree);
}

std::string apply(std::string_view text, std::span<const Splice> splices) {
    std::map<std::uint64_t, std::vector<const Splice*>> by_place;   // sorted: the text is built from its start
    for (const Splice& s : splices) {
        if (s.at > text.size()) throw UsageError(std::format("a splice at {} is past the end of a text of {}", s.at, text.size()));
        by_place[s.at].push_back(&s);
    }
    std::string out;
    out.reserve(text.size() + splices.size() * 8);
    std::size_t done = 0;
    for (auto& [at, here] : by_place) {
        std::vector<const Splice*> opens, closes;
        for (const Splice* s : here) (s->closing ? closes : opens).push_back(s);
        // the outer expression opens first; the inner one is closed first
        std::ranges::stable_sort(opens, [](const Splice* a, const Splice* b) { return a->depth < b->depth; });
        std::ranges::stable_sort(closes, [](const Splice* a, const Splice* b) { return a->depth > b->depth; });
        out.append(text.substr(done, static_cast<std::size_t>(at) - done));
        for (const Splice* s : closes) out.append(s->text);
        for (const Splice* s : opens) out.append(s->text);
        done = static_cast<std::size_t>(at);
    }
    out.append(text.substr(done));
    return out;
}

std::string null_reads(std::string_view text) {
    static const Regex cast(R"(\(\((?:struct|union)\s+\w+\s*\*\s*\)dos_hp\()");
    const std::string code = base::ctext::blank(text);
    std::vector<std::size_t> at;   // where "dos_hp(" starts, to become "dos_hpr("
    constexpr std::string_view kHandle = "dos_hp(";
    cast.for_each_match(code, [&](const RegexMatch& m) {
        // the ')' that closes the '(' the match begins with
        int depth = 0;
        std::size_t k = m.start();
        for (; k < code.size(); ++k) {
            if (code[k] == '(') ++depth;
            else if (code[k] == ')') --depth;
            if (depth == 0) break;
        }
        if (k < code.size()) {
            std::size_t i = k + 1;
            while (i < code.size() && text::is_space(code[i])) ++i;
            if (std::string_view(code).substr(i).starts_with("->")) at.push_back(m.end() - kHandle.size());
        }
        return true;
    });
    // (a pointer variable is left as it is: given on, a callee would take the null segment for a place - a run's fault
    //  there names it, and sanitize.low_reads reads through it alone)
    std::string out;
    out.reserve(text.size() + at.size());
    std::size_t done = 0;
    for (const std::size_t i : at) {
        out.append(text.substr(done, i - done));
        out.append("dos_hpr(");
        done = i + kHandle.size();
    }
    out.append(text.substr(done));
    return out;
}

std::string declare(std::string_view text_in, const RecordSet& olds, const Members& found) {
    // For each record: its pointer members by name (a record with none is still visited: its declarations are split and
    // its words made right).
    std::map<std::string, std::map<std::string, std::string>> names;   // sorted: a record at a time, in one order
    for (const std::string& rec : olds) names[rec];
    for (const auto& [member, type] : found) names[member.record][member.name] = type;
    static const Regex definition(R"(\b(?:struct|union)\s+(\w+)\s*\{)");
    std::string text(text_in);
    for (const auto& [rec, members] : names) {
        const std::size_t space = rec.rfind(' ');
        const std::string tag = space == std::string::npos ? rec : rec.substr(space + 1);
        std::size_t from = 0;
        while (const std::optional<RegexMatch> m = definition.search(text, from)) {
            if (m->group(1) != tag) {
                from = m->end();
                continue;
            }
            const std::size_t body_at = m->end();
            const std::size_t k = ol::closing_brace(text, body_at - 1);
            std::string body = text.substr(body_at, k - body_at);
            body = ol::split_declarators(body);
            for (const auto& [name, type] : members) body = ol::declare_handle(body, name, type);
            body = ol::narrow_words(body);
            from = body_at + body.size();
            text = text.substr(0, body_at) + body + text.substr(std::min(k, text.size()));
        }
    }
    return text;
}

bool mentions(const std::filesystem::path& folder, std::string_view file, std::span<const std::string> tags) {
    std::set<std::string> seen;
    return ol::mentions_in(folder, std::string(file), tags, seen);
}

Conversions run(base::cache::Cache& cache, const std::filesystem::path& clang, const std::filesystem::path& folder,
           std::span<const std::string> files, const PointerParams& pointer_params) {
    cache.declare_kind(kCacheKind, {.scope = base::cache::Scope::State, .generation = std::string(kCodeVersion)});
    Conversions out;
    // one file's tree at a time: a tree holds the system's headers too - all of them at once were gigabytes
    for (const std::string& f : files) {
        const std::string key = arith16::pass_key(clang, folder, f, "records", kCodeVersion);
        if (const std::optional<std::vector<std::string>> kept = cache.value<std::vector<std::string>>(kCacheKind, key)) {
            out.olds.insert(kept->begin(), kept->end());
            continue;
        }
        std::vector<std::string> mine;
        if (const std::optional<arith16::Tree> tree = arith16::dump(clang, f, folder)) {
            const RecordSet found = byte_records(*tree);
            mine.assign(found.begin(), found.end());
            out.olds.insert(found.begin(), found.end());
        }
        cache.keep(kCacheKind, key, mine);
    }
    if (out.olds.empty()) return out;
    std::vector<std::string> tags;
    for (const std::string& rec : out.olds) tags.push_back(rec.substr(rec.rfind(' ') + 1));
    std::vector<std::string> extra(out.olds.begin(), out.olds.end());   // what the second pass depends on besides the file
    for (const auto& [name, ptrs] : pointer_params) {
        std::string item = name + ":";
        for (const bool p : ptrs) item += p ? 'p' : '-';
        extra.push_back(std::move(item));
    }
    for (const std::string& f : files) {
        if (!mentions(folder, f, tags)) continue;   // (a file that never names them, nor its headers: none)
        const std::string key = arith16::pass_key(clang, folder, f, "oldlayout", kCodeVersion, extra);
        cached::FileOutcome got;
        if (const std::optional<cached::FileOutcome> kept = cache.value<cached::FileOutcome>(kCacheKind, key)) {
            got = *kept;
        } else {
            const std::optional<arith16::Tree> tree = arith16::dump(clang, f, folder);
            if (!tree) continue;
            const std::string text = fs::read_text(folder / fs::from_text(f));
            const Analysis a = analyse(*tree, text, tree->find_file(f), out.olds, pointer_params);
            if (!a.splices.empty()) got.converted = oldlayout::apply(text, a.splices);
            for (const auto& [member, type] : a.found) got.found.push_back({member.record, {member.name, type}});
            for (const Refusal& r : a.refused) got.refused.push_back({r.at, r.op});
            cache.keep(kCacheKind, key, got);
        }
        for (const auto& [record, member_type] : got.found) out.found[{record, member_type.first}] = member_type.second;
        for (const auto& [at, op] : got.refused) out.refused.push_back({f, at, op});
        if (got.converted) out.texts[f] = std::move(*got.converted);
    }
    return out;
}

}  // namespace cb::port::oldlayout
