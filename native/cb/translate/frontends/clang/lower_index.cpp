// cb/translate/frontends/clang/lower_index.cpp -- reading the dump: the accessors, the one walk that indexes every
// declaration, the names and types a spelling can mean, and integer constant expressions.
#include <algorithm>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <format>
#include <functional>
#include <limits>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/clang/lower_impl.hpp"

namespace cb::translate::frontends::clang {

namespace tree {

void bad(const std::string& what) { throw ParseError("clang dump", 0, 0, what); }

const Json* get(const Json& node, std::string_view key) {
    if (!node.is_object()) bad("a node of the dump is not an object");
    const auto it = node.find(std::string(key));
    if (it == node.end() || it->is_null()) return nullptr;
    return &*it;
}

const Json& require(const Json& node, std::string_view key) {
    const Json* v = get(node, key);
    if (v == nullptr) bad("a node has no member '" + std::string(key) + "'");
    return *v;
}

const Json& object(const Json& node, std::string_view key) {
    static const Json empty = Json::object();
    const Json* v = get(node, key);
    if (v == nullptr || !v->is_object() || v->empty()) return empty;
    return *v;
}

std::string text(const Json& node, std::string_view key) {
    const Json* v = get(node, key);
    if (v == nullptr) return {};
    if (!v->is_string()) bad("member '" + std::string(key) + "' is not text");
    return v->get<std::string>();
}

std::optional<std::string> maybe_text(const Json& node, std::string_view key) {
    const Json* v = get(node, key);
    if (v == nullptr) return std::nullopt;
    if (!v->is_string()) bad("member '" + std::string(key) + "' is not text");
    return v->get<std::string>();
}

bool flag(const Json& node, std::string_view key) {
    const Json* v = get(node, key);
    if (v == nullptr) return false;
    if (v->is_boolean()) return v->get<bool>();
    if (v->is_number_integer()) return v->get<std::int64_t>() != 0;
    if (v->is_number_unsigned()) return v->get<std::uint64_t>() != 0;
    if (v->is_string()) return !v->get_ref<const std::string&>().empty();
    if (v->is_array() || v->is_object()) return !v->empty();
    return true;
}

std::string kind(const Json& node) { return text(node, "kind"); }

const std::vector<Json>& inner(const Json& node) {
    static const std::vector<Json> none;
    const Json* v = get(node, "inner");
    if (v == nullptr) return none;
    if (!v->is_array()) bad("member 'inner' is not a list");
    return v->get_ref<const Json::array_t&>();
}

const Json& child(const Json& node, long index) {
    const std::vector<Json>& kids = inner(node);
    const long n = static_cast<long>(kids.size());
    const long at = index < 0 ? n + index : index;
    if (at < 0 || at >= n) bad("a node has no child " + std::to_string(index));
    return kids[static_cast<std::size_t>(at)];
}

std::int64_t integer(const Json& node, std::string_view key) {
    const Json& v = require(node, key);
    if (v.is_number_integer()) return v.get<std::int64_t>();
    if (v.is_number_unsigned()) {
        const std::uint64_t u = v.get<std::uint64_t>();
        if (u > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) bad("member '" + std::string(key) + "' is past int64");
        return static_cast<std::int64_t>(u);
    }
    if (v.is_boolean()) return v.get<bool>() ? 1 : 0;
    if (v.is_string()) {
        if (auto n = text::parse_int<std::int64_t>(v.get_ref<const std::string&>())) return *n;
    }
    bad("member '" + std::string(key) + "' is not an integer");
}

bool ends_with_attr(std::string_view k) { return k.ends_with("Attr"); }

}  // namespace tree

// ---- paths -----------------------------------------------------------------------------------------------------

std::string normal_path(std::string_view path) {
    std::string joined(path);
    for (char& c : joined) {
        if (c == '/') c = '\\';
    }
    const bool rooted = joined.size() >= 2 && joined[1] == ':';
    if (!rooted && !joined.starts_with('\\')) {
        std::string cwd = std::filesystem::current_path().string();
        for (char& c : cwd) {
            if (c == '/') c = '\\';
        }
        joined = cwd + "\\" + joined;
    }
    std::string drive;
    std::string rest = joined;
    if (joined.size() >= 2 && joined[1] == ':') {
        drive = joined.substr(0, 2);
        rest = joined.substr(2);
    }
    std::vector<std::string> parts;
    for (const std::string_view part : text::split(rest, '\\')) {
        if (part.empty() || part == ".") continue;
        if (part == "..") {
            if (!parts.empty()) parts.pop_back();
            continue;
        }
        parts.emplace_back(part);
    }
    return text::lower(drive + "\\" + text::join(parts, "\\"));
}

std::string directory_of(const std::string& path) {
    const std::size_t sep = path.rfind('\\');
    if (sep == std::string::npos) return {};
    std::string head = path.substr(0, sep);
    if (head.empty() || head.ends_with(':')) head += '\\';
    return head;
}

std::string Scope::fresh(const std::string& base) {
    ++n;
    std::string name = base + std::to_string(n);
    used.insert(name);
    return name;
}

// ---- construction ----------------------------------------------------------------------------------------------------

Lowering::Lowering(Arena& arena, const Json& tree, const Options& options)
    : arena_(arena),
      tree_(tree),
      main_(normal_path(options.main_path)),
      auto_own_(!options.own.has_value()),
      cpp_(options.language == "c++" || options.language == "cpp" || options.language == "cxx"),
      language_(cpp_ ? "c++" : "c"),
      std_(options.standard),
      model_(ctypes::Model::for_triple(options.triple)),
      types_(model_, [this](const std::string& n, const std::vector<ir::TypeP>& args, const std::optional<std::string>& tag) {
          return name(n, args, tag);
      }),
      sizes_(model_, [this](const ir::TypeP& t) { return layout_of(t); }) {
    if (options.own) {
        for (const std::string& p : *options.own) own_.insert(normal_path(p));
    }
    own_.insert(main_);
    std::string base(options.main_path);
    const std::size_t sep = base.find_last_of("/\\");
    if (sep != std::string::npos) base = base.substr(sep + 1);
    const std::size_t dot = base.rfind('.');
    unit_name_ = (dot == std::string::npos || dot == 0) ? base : base.substr(0, dot);
    index();
}

ir::SymP Lowering::sym(ir::SymKind kind, std::string name, std::optional<std::string> owner, bool is_static, ir::TypeP type) {
    return ir::make_sym(kind, std::move(name), std::move(owner), is_static, std::move(type));
}

// ---- the index -----------------------------------------------------------------------------------------------------

namespace {

struct WalkState {
    std::int32_t file = -1;
    std::int64_t line = 0;
};

}  // namespace

const Location& Lowering::loc(const Json& n) const {
    static const Location none;
    const auto it = locations_.find(&n);
    return it == locations_.end() ? none : it->second;
}

// One pre-order walk: each node's file/line/column (clang writes them as deltas), and every declaration by id, with its
// parent and qualified name.
void Lowering::index() {
    WalkState state;
    struct Place {
        std::int32_t file;
        std::int64_t line;
        std::int64_t col;
    };
    const auto file_id = [this](const std::string& file) {
        const auto [it, fresh] = file_ids_.try_emplace(file, static_cast<std::int32_t>(files_.size()));
        if (fresh) files_.push_back(file);
        return it->second;
    };
    // a location object -> where it points, or nothing for an absent one
    const std::function<std::optional<Place>(const Json*)> update = [&](const Json* where) -> std::optional<Place> {
        if (where == nullptr || !where->is_object() || where->empty()) return std::nullopt;
        const bool has_spelling = where->contains("spellingLoc");
        const bool has_expansion = where->contains("expansionLoc");
        if (has_spelling || has_expansion) {
            std::optional<Place> got;
            for (const char* key : {"spellingLoc", "expansionLoc"}) {
                if (!where->contains(key)) continue;
                if (auto r = update(tree::get(*where, key))) got = r;
            }
            return got;
        }
        if (const Json* f = tree::get(*where, "file")) {
            if (!f->is_string()) tree::bad("a location's file is not text");
            state.file = file_id(f->get<std::string>());
        }
        if (tree::get(*where, "line") != nullptr) state.line = tree::integer(*where, "line");
        return Place{state.file, state.line, tree::get(*where, "col") != nullptr ? tree::integer(*where, "col") : 0};
    };

    // the namespaces below a node: shared, built once where one opens
    std::deque<std::vector<std::string>> ns_pool;
    ns_pool.emplace_back();
    struct Item {
        const Json* node;
        const Json* parent;
        const std::vector<std::string>* ns;
    };
    std::vector<Item> stack = {{&tree_, nullptr, &ns_pool.front()}};
    std::vector<std::pair<const Json*, const std::vector<std::string>*>> pending;   // template specialisations, keyed after the walk
    const auto joined = [](const std::vector<std::string>& ns, const std::string& last) {
        std::vector<std::string> parts = ns;
        parts.push_back(last);
        return text::join(parts, ".");
    };
    const auto child_namespaces = [&](const std::vector<std::string>& ns, const std::string& last) {
        ns_pool.push_back(ns);
        ns_pool.back().push_back(last);
        return &ns_pool.back();
    };
    while (!stack.empty()) {
        const Item item = stack.back();
        stack.pop_back();
        const Json& n = *item.node;
        if (!n.is_object() || n.empty()) continue;   // ({}: an absent child, as in a for's parts)
        const std::vector<std::string>& ns = *item.ns;
        Location here_loc;
        const auto here = update(tree::get(n, "loc"));
        const Json& range = tree::object(n, "range");
        const auto begin = update(tree::get(range, "begin"));
        update(tree::get(range, "end"));
        if (begin) {
            here_loc.has_begin = true;
            here_loc.begin_line = begin->line;
            here_loc.begin_col = begin->col;
        }
        if (here) {
            here_loc.known = true;
            here_loc.file = here->file;
            here_loc.line = here->line;
            here_loc.col = here->col;
        } else if (item.parent != nullptr && loc(*item.parent).known) {
            const Location& up = loc(*item.parent);
            here_loc.known = true;
            here_loc.file = up.file;
            here_loc.line = up.line;
            here_loc.col = up.col;
        }
        if (here_loc.known || here_loc.has_begin) locations_[&n] = here_loc;

        const std::string kind = tree::kind(n);
        const std::vector<std::string>* child_ns = item.ns;
        if (kind == "LabelStmt" && tree::flag(n, "declId")) labels_[tree::text(n, "declId")] = tree::maybe_text(n, "name");
        const std::string nid = tree::text(n, "id");
        if ((!nid.empty() && kind.ends_with("Decl")) || kind == "ClassTemplateSpecializationDecl") {
            decls_[nid] = &n;
            if (item.parent != nullptr && tree::kind(*item.parent).ends_with("Decl")) parent_[nid] = item.parent;
            const std::string name_text = tree::text(n, "name");
            if (kind == "NamespaceDecl") {
                child_ns = child_namespaces(ns, name_text.empty() ? "(anonymous)" : name_text);
            } else if (kind == "RecordDecl" || kind == "CXXRecordDecl" || kind == "ClassTemplateSpecializationDecl") {
                const std::string parent_kind = item.parent != nullptr ? tree::kind(*item.parent) : "";
                if ((parent_kind == "CXXRecordDecl" || parent_kind == "ClassTemplateSpecializationDecl") &&
                    name_text == tree::text(*item.parent, "name") && !tree::flag(n, "completeDefinition")) {
                    // the injected class name
                } else if (kind == "ClassTemplateSpecializationDecl") {
                    // (its key spells its arguments, which may name records indexed later: after the walk)
                    pending.emplace_back(&n, &ns);
                    child_ns = child_namespaces(ns, name_text);
                } else {
                    const std::string key = record_key(n, ns);
                    qnames_[nid] = key;
                    if (tree::flag(n, "completeDefinition") || !records_.contains(key)) records_.insert_or_assign(key, &n);
                    if (name_text.empty() && loc(n).has_begin) {   // (clang names it by where it begins too)
                        const std::string alias = joined(ns, "__unnamed_" + ctypes::anon_key(loc(n).begin_line, loc(n).begin_col));
                        records_.try_emplace(alias, &n);
                    }
                    const std::size_t dot = key.rfind('.');
                    child_ns = child_namespaces(ns, dot == std::string::npos ? key : key.substr(dot + 1));
                }
            } else if (kind == "TypedefDecl" || kind == "TypeAliasDecl") {
                const std::vector<Json>& kids = tree::inner(n);
                const Json* underlying = kids.empty() ? nullptr : &kids[0];
                while (underlying != nullptr && tree::kind(*underlying) == "ElaboratedType" && !tree::inner(*underlying).empty()) {
                    underlying = &tree::inner(*underlying)[0];
                }
                const std::string full_name = joined(ns, tree::require(n, "name").get<std::string>());
                const std::string under_kind = underlying != nullptr ? tree::kind(*underlying) : "";
                const Json* target = underlying != nullptr ? tree::get(*underlying, "decl") : nullptr;
                if ((under_kind == "RecordType" || under_kind == "EnumType") && target != nullptr) {
                    typedefs_.insert_or_assign(full_name, std::make_pair(under_kind == "RecordType" ? "record" : "enum",
                                                                         tree::require(*target, "id").get<std::string>()));
                } else {
                    typedefs_.insert_or_assign(full_name, std::make_pair(std::string("type"), tree::text(tree::require(n, "type"), "qualType")));
                }
            } else if (kind == "EnumDecl") {
                std::int64_t value = -1;
                for (const Json& c : tree::inner(n)) {
                    if (tree::kind(c) != "EnumConstantDecl") continue;
                    const std::vector<Json>& init = tree::inner(c);
                    value = !init.empty() ? enum_const(&init[0]) : value + 1;
                    enum_values_[tree::require(c, "id").get<std::string>()] = value;
                }
                if (tree::flag(n, "scopedEnumTag")) scoped_enums_.insert_or_assign(nid, joined(ns, tree::require(n, "name").get<std::string>()));
            } else if (kind == "LabelDecl") {
                labels_[nid] = tree::maybe_text(n, "name");
            }
        }
        // the children are walked in order: pushed reversed
        std::vector<const Json*> kids;
        for (const Json& c : tree::inner(n)) kids.push_back(&c);
        if (const Json* filler = tree::get(n, "array_filler"); filler != nullptr && filler->is_array()) {
            for (const Json& c : *filler) kids.push_back(&c);
        }
        for (auto it = kids.rbegin(); it != kids.rend(); ++it) {
            if ((*it)->is_object()) stack.push_back({*it, &n, child_ns});
        }
    }
    for (int pass = 0; pass < 2; ++pass) {   // (twice: an argument may be another instance)
        types_.clear_cache();
        for (const auto& [n, ns] : pending) {
            const std::string key = record_key(*n, *ns);
            qnames_[tree::text(*n, "id")] = key;
            if (tree::flag(*n, "completeDefinition") || !records_.contains(key)) records_.insert_or_assign(key, n);
        }
    }
    types_.clear_cache();
    if (auto_own_) {   // the program's own headers: those beside its source
        const std::string here = directory_of(main_);
        for (const std::string& f : files_) {
            const std::string p = normal_path(f);
            if (directory_of(p) == here) own_.insert(p);
        }
    }
}

std::string Lowering::record_key(const Json& n, const std::vector<std::string>& ns) {
    const auto joined = [&ns](const std::string& last) {
        std::vector<std::string> parts = ns;
        parts.push_back(last);
        return text::join(parts, ".");
    };
    const std::string name_text = tree::text(n, "name");
    if (name_text.empty()) return joined("__unnamed_" + ctypes::anon_key(loc(n).line, loc(n).col));
    std::string key = joined(name_text);
    if (tree::kind(n) == "ClassTemplateSpecializationDecl") {
        std::vector<std::string> args;
        for (const Json& a : tree::inner(n)) {
            if (tree::kind(a) != "TemplateArgument") continue;
            if (tree::get(a, "type") != nullptr) {
                const std::string spelling = tree::text(tree::require(a, "type"), "qualType");
                try {
                    args.push_back(spell(types_.parse(spelling)));
                } catch (const ParseError&) {
                    args.push_back(spell(ir::class_type(spelling)));
                } catch (const FormatError&) {
                    args.push_back(spell(ir::class_type(spelling)));
                }
            } else if (const Json* v = tree::get(a, "value")) {
                args.push_back(v->is_string() ? v->get<std::string>() : v->dump());
            }
        }
        key += "<" + text::join(args, ", ") + ">";
    }
    return key;
}

std::int64_t Lowering::enum_const(const Json* e) { return constant(e).value_or(0); }

// ---- types -----------------------------------------------------------------------------------------------------------

ir::TypeP Lowering::name(const std::string& type_name, const std::vector<ir::TypeP>& args, const std::optional<std::string>& tag) {
    struct DepthGuard {   // a typedef that names itself is parsed for ever
        int& depth;
        explicit DepthGuard(int& d) : depth(d) {
            if (++depth > 40) {
                --depth;
                tree::bad("a type name that leads back to itself");
            }
        }
        ~DepthGuard() { --depth; }
    } guard(name_depth_);
    const std::string dotted = text::replace_all(type_name, "::", ".");
    if (dotted == "__builtin_va_list") {   // MSVC's va_list: a char pointer
        return ir::pointer_to(ir::prim_type(ir::Prim::Char, model_.char_signed));
    }
    if (dotted.starts_with("std.") && !records_.contains(dotted)) return ir::class_type(dotted, args);   // a library class: named, not laid out
    std::string key = dotted;
    if (!args.empty()) {
        std::vector<std::string> spelled;
        for (const ir::TypeP& a : args) spelled.push_back(spell(a));
        key += "<" + text::join(spelled, ", ") + ">";
    }
    const Json* rec = find_record(key);
    if (rec != nullptr && tag != "enum") return record_type(rec);
    if (const auto td = find_typedef(dotted)) {
        const auto& [td_kind, value] = *td;
        if (td_kind == "record") {
            const auto it = decls_.find(value);
            return it != decls_.end() ? record_type(it->second) : ir::TypeP();
        }
        if (td_kind == "enum") return ir::i32_type();
        return types_.parse(value);
    }
    for (const auto& [id, qualified] : scoped_enums_) {
        if (qualified == dotted || qualified.ends_with("." + dotted)) return ir::i32_type();   // (an enum class is its int: its constants stay named)
    }
    if (tag == "enum" || find_enum(dotted)) return ir::i32_type();
    return {};
}

const Json* Lowering::find_record(const std::string& key) {
    if (const Json* const* hit = records_.find(key)) return *hit;
    if (const auto memo = tail_memo_.find(key); memo != tail_memo_.end() && memo->second.first == records_.size()) return memo->second.second;
    const std::string tail = "." + key;
    const Json* best = nullptr;
    std::size_t best_length = 0;
    for (const auto& [k, node] : records_) {   // the shortest key that ends with it; the first of equals
        if (k.ends_with(tail) && (best == nullptr || k.size() < best_length)) {
            best = node;
            best_length = k.size();
        }
    }
    tail_memo_[key] = {records_.size(), best};
    return best;
}

std::optional<std::pair<std::string, std::string>> Lowering::find_typedef(const std::string& type_name) {
    if (const auto* hit = typedefs_.find(type_name)) return *hit;
    const std::string tail = "." + type_name;
    for (const auto& [k, v] : typedefs_) {
        if (k.ends_with(tail)) return v;
    }
    return std::nullopt;
}

bool Lowering::find_enum(const std::string& type_name) {
    if (!enum_names_) {
        enum_names_.emplace();
        for (const auto& [id, node] : decls_) {
            if (tree::kind(*node) == "EnumDecl") enum_names_->insert(tree::text(*node, "name"));
        }
    }
    const std::size_t dot = type_name.rfind('.');
    return enum_names_->contains(dot == std::string::npos ? type_name : type_name.substr(dot + 1));
}

ir::TypeP Lowering::record_type(const Json* n) {
    if (n == nullptr) return {};
    const auto key_it = qnames_.find(tree::require(*n, "id").get<std::string>());
    if (key_it == qnames_.end()) return {};
    const std::string& key = key_it->second;
    const Json* complete = nullptr;
    if (const Json* const* hit = records_.find(key)) complete = *hit;
    const Json& full = complete != nullptr ? *complete : *n;
    if (tree::kind(full) == "ClassTemplateSpecializationDecl") {
        const std::string base = key.substr(0, key.find('<'));
        std::vector<ir::TypeP> args;
        for (const Json& a : tree::inner(full)) {
            if (tree::kind(a) != "TemplateArgument") continue;
            if (tree::get(a, "type") != nullptr) {
                const std::string spelling = tree::text(tree::require(a, "type"), "qualType");
                try {
                    args.push_back(types_.parse(spelling));
                } catch (const ParseError&) {
                    args.push_back(ir::class_type(spelling));
                } catch (const FormatError&) {
                    args.push_back(ir::class_type(spelling));
                }
            } else {
                const Json* v = tree::get(a, "value");
                args.push_back(ir::class_type(v == nullptr ? "None" : (v->is_string() ? v->get<std::string>() : v->dump())));
            }
        }
        return ir::class_type(base, std::move(args));
    }
    return ir::class_type(key);
}

ir::TypeP Lowering::ty(const Json& n, std::string_view key) {
    const Json& t = tree::object(n, key);
    const std::optional<std::string> spelling = tree::maybe_text(t, "qualType");
    if (!spelling) return ir::any_type();
    try {
        return ctypes::normalise(types_.parse(*spelling));
    } catch (const ParseError&) {
    } catch (const FormatError&) {
    }
    try {
        if (tree::flag(t, "desugaredQualType")) return ctypes::normalise(types_.parse(tree::text(t, "desugaredQualType")));
    } catch (const ParseError&) {
    } catch (const FormatError&) {
    }
    return ir::class_type("<unread:" + *spelling + ">");
}

// sizeof's view of a class type: its record, or an int's for a C++ enum class.
ir::TypeP Lowering::layout_of(const ir::TypeP& t) {
    const std::string spelled = spell(t);
    for (const auto& [id, qualified] : scoped_enums_) {
        if (qualified == spelled || qualified.ends_with("." + spelled)) {
            ir::RecordType rec;
            rec.name = spelled;
            rec.size = 4;
            rec.fields.push_back(ir::RecordField{.name = "value", .offset = 0, .type = ir::i32_type()});
            rec.role = ir::RecordRole::Struct;
            return ir::make_type(std::move(rec));
        }
    }
    return record_layout(t);
}

// The record type of a C/C++ struct (for sizeof and ClassDecl.layout); empty when it has none the front end can state.
ir::TypeP Lowering::record_layout(const ir::TypeP& t) {
    const std::string key = spell(t);
    if (const auto hit = layout_cache_.find(key); hit != layout_cache_.end()) return hit->second;
    const Json* rec = find_record(key);
    if (rec == nullptr) return {};
    layout_cache_[key] = {};   // (a cycle through a value field cannot be)
    std::vector<ctypes::Layouts::Field> fields;
    const bool is_union = tree::text(*rec, "tagUsed") == "union";
    for (const Json& c : tree::inner(*rec)) {
        if (tree::kind(c) == "FieldDecl") {
            if (tree::flag(c, "isBitfield")) return {};
            fields.push_back({tree::text(c, "name"), ty(c)});
        }
    }
    if (polymorphic(*rec) || !bases(*rec).empty()) return {};
    try {
        ir::TypeP laid = sizes_.lay_out(key, fields, is_union);
        layout_cache_[key] = laid;
        return laid;
    } catch (const FormatError&) {
        return {};   // (a field whose size is not known here: no layout)
    }
}

bool Lowering::polymorphic(const Json& rec) {
    const Json& data = tree::object(rec, "definitionData");
    return tree::flag(data, "isPolymorphic") || tree::flag(data, "isAbstract");
}

const std::vector<Json>& Lowering::bases(const Json& rec) {
    static const std::vector<Json> none;
    const Json* v = tree::get(rec, "bases");
    if (v == nullptr || !v->is_array()) return none;
    return v->get_ref<const Json::array_t&>();
}

// ---- helpers ---------------------------------------------------------------------------------------------------------

bool Lowering::is_own(std::int32_t file) const {
    if (file < 0) return false;
    return own_.contains(normal_path(files_[static_cast<std::size_t>(file)]));
}

Pos Lowering::pos(const Json& n) const {
    const Location& l = loc(n);
    Pos p;
    p.line = (l.known && l.file >= 0 && is_own(l.file)) ? static_cast<std::uint32_t>(l.line) : 0;
    p.col = static_cast<std::uint32_t>(l.known ? l.col : 0);
    return p;
}

ir::Node* Lowering::unsupported_node(const std::string& kind, const Json* n, const std::optional<std::string>& raw) {
    ir::Unsupported u;
    u.kind = kind;
    u.raw = raw;
    return make(std::move(u), n != nullptr ? ty(*n) : ir::any_type(), n != nullptr ? pos(*n) : Pos{});
}

// ---- constants -------------------------------------------------------------------------------------------------------

namespace {

std::optional<std::int64_t> add_checked(std::int64_t a, std::int64_t b) {
    if ((b > 0 && a > std::numeric_limits<std::int64_t>::max() - b) || (b < 0 && a < std::numeric_limits<std::int64_t>::min() - b)) return std::nullopt;
    return a + b;
}

std::optional<std::int64_t> subtract_checked(std::int64_t a, std::int64_t b) {
    if ((b < 0 && a > std::numeric_limits<std::int64_t>::max() + b) || (b > 0 && a < std::numeric_limits<std::int64_t>::min() + b)) return std::nullopt;
    return a - b;
}

std::optional<std::int64_t> multiply_checked(std::int64_t a, std::int64_t b) {
    if (a == 0 || b == 0) return 0;
    const std::int64_t r = static_cast<std::int64_t>(static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b));
    if (r / b != a || (a == -1 && b == std::numeric_limits<std::int64_t>::min()) || (b == -1 && a == std::numeric_limits<std::int64_t>::min())) return std::nullopt;
    return r;
}

std::optional<std::int64_t> shift_left_checked(std::int64_t a, std::int64_t b) {
    if (b < 0 || b >= 63) return std::nullopt;
    const std::int64_t r = static_cast<std::int64_t>(static_cast<std::uint64_t>(a) << b);
    if ((r >> b) != a) return std::nullopt;
    return r;
}

}  // namespace

// A C integer constant expression's value, or nothing when it is not one (or leaves int64).
std::optional<std::int64_t> Lowering::constant(const Json* n) {
    if (n == nullptr) return std::nullopt;
    const std::string k = tree::kind(*n);
    if (tree::get(*n, "value") != nullptr && (k == "ConstantExpr" || k == "IntegerLiteral")) {
        const Json& v = *tree::get(*n, "value");
        if (v.is_string()) {
            if (const auto parsed = text::parse_int<std::int64_t>(v.get_ref<const std::string&>())) return parsed;
        } else if (v.is_number_integer() || v.is_number_unsigned()) {
            return tree::integer(*n, "value");
        }
    }
    if (k == "CharacterLiteral") return tree::integer(*n, "value");
    if (k == "CXXBoolLiteralExpr") return tree::flag(*n, "value") ? 1 : 0;
    if (k == "ConstantExpr" || k == "ParenExpr" || k == "ImplicitCastExpr" || k == "CStyleCastExpr" || k == "CXXStaticCastExpr" ||
        k == "CXXFunctionalCastExpr" || k == "SubstNonTypeTemplateParmExpr") {
        std::optional<std::int64_t> v = !tree::inner(*n).empty() ? constant(&tree::inner(*n).back()) : std::nullopt;
        if (v && k != "ParenExpr" && k != "ConstantExpr") v = wrap_to_type(*v, ty(*n));
        return v;
    }
    if (k == "DeclRefExpr") {
        const Json& d = tree::object(*n, "referencedDecl");
        const std::string id = tree::text(d, "id");
        if (tree::kind(d) == "EnumConstantDecl") {
            const auto it = enum_values_.find(id);
            return it == enum_values_.end() ? std::nullopt : std::optional<std::int64_t>(it->second);
        }
        const auto node = decls_.find(id);
        if (node != decls_.end() && tree::kind(*node->second) == "VarDecl" && tree::flag(*node->second, "constexpr") &&
            !tree::inner(*node->second).empty()) {
            return constant(&tree::inner(*node->second)[0]);
        }
        return std::nullopt;
    }
    if (k == "UnaryOperator") {
        const auto v = constant(&tree::child(*n, 0));
        if (!v) return std::nullopt;
        const std::string op = tree::require(*n, "opcode").get<std::string>();
        if (op == "-") return *v == std::numeric_limits<std::int64_t>::min() ? std::nullopt : std::optional<std::int64_t>(-*v);
        if (op == "+") return v;
        if (op == "~") return ~*v;
        if (op == "!") return *v == 0 ? 1 : 0;
        return std::nullopt;
    }
    if (k == "BinaryOperator") {
        const auto a = constant(&tree::child(*n, 0));
        const auto b = constant(&tree::child(*n, 1));
        if (!a || !b) return std::nullopt;
        const std::string op = tree::require(*n, "opcode").get<std::string>();
        std::optional<std::int64_t> v;
        if (op == "+") {
            v = add_checked(*a, *b);
        } else if (op == "-") {
            v = subtract_checked(*a, *b);
        } else if (op == "*") {
            v = multiply_checked(*a, *b);
        } else if (op == "<<") {
            v = shift_left_checked(*a, *b);
        } else if (op == ">>") {
            if (*b >= 0) v = *b >= 63 ? (*a < 0 ? -1 : 0) : (*a >> *b);
        } else if (op == "&") {
            v = *a & *b;
        } else if (op == "|") {
            v = *a | *b;
        } else if (op == "^") {
            v = *a ^ *b;
        } else if (op == "==") {
            v = *a == *b;
        } else if (op == "!=") {
            v = *a != *b;
        } else if (op == "<") {
            v = *a < *b;
        } else if (op == ">") {
            v = *a > *b;
        } else if (op == "<=") {
            v = *a <= *b;
        } else if (op == ">=") {
            v = *a >= *b;
        } else if (op == "&&") {
            v = (*a != 0 && *b != 0) ? 1 : 0;
        } else if (op == "||") {
            v = (*a != 0 || *b != 0) ? 1 : 0;
        } else if (op == "/") {
            if (*b == 0 || (*a == std::numeric_limits<std::int64_t>::min() && *b == -1)) return std::nullopt;
            v = *a / *b;   // C truncates toward zero
        } else if (op == "%") {
            if (*b == 0 || (*a == std::numeric_limits<std::int64_t>::min() && *b == -1)) return std::nullopt;
            v = *a % *b;
        }
        return v ? std::optional<std::int64_t>(wrap_to_type(*v, ty(*n))) : std::nullopt;
    }
    if (k == "UnaryExprOrTypeTraitExpr") {
        const ir::Node* lit = size_of(*n);
        const auto* l = ir::get<ir::Lit>(lit);
        if (l != nullptr) {
            if (const auto* i = std::get_if<std::int64_t>(&l->value)) return *i;
            if (const auto* u = std::get_if<std::uint64_t>(&l->value); u != nullptr && *u <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
                return static_cast<std::int64_t>(*u);
            }
        }
        return std::nullopt;
    }
    if (k == "ConditionalOperator") {
        const auto c = constant(&tree::child(*n, 0));
        if (!c) return std::nullopt;
        return constant(&tree::child(*n, *c != 0 ? 1 : 2));
    }
    return std::nullopt;
}

}  // namespace cb::translate::frontends::clang
