// translate/l2.cpp -- L2 documents over the typed tree, and the module model (see l2.hpp).
#include "translate/l2.hpp"

#include <array>
#include <cmath>
#include <format>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace cb::translate::l2 {
namespace {

using ir::Node;

[[noreturn]] void fail(const std::string& what) { throw FormatError("l2", 0, what); }

// ---- the small forms of the schema ------------------------------------------------------------------------------------

template <class E, std::size_t N>
std::optional<E> enum_from(std::string_view text, const std::array<E, N>& all) {
    for (E e : all) {
        if (ir::to_text(e) == text) return e;
    }
    return std::nullopt;
}

constexpr std::array kPrims = {ir::Prim::Bool, ir::Prim::I8,  ir::Prim::I16, ir::Prim::I32, ir::Prim::I64,
                               ir::Prim::U8,   ir::Prim::U16, ir::Prim::U32, ir::Prim::U64, ir::Prim::F32,
                               ir::Prim::F64,  ir::Prim::F80, ir::Prim::Char};
constexpr std::array kSpaces = {ir::AddressSpace::Flat, ir::AddressSpace::Near, ir::AddressSpace::Far,
                                ir::AddressSpace::Huge, ir::AddressSpace::Real, ir::AddressSpace::Io};
constexpr std::array kMemory = {ir::MemoryModel::Gc,     ir::MemoryModel::Owned,  ir::MemoryModel::Borrowed,
                                ir::MemoryModel::Stack,  ir::MemoryModel::Manual, ir::MemoryModel::Emulated};
constexpr std::array kOverflows = {ir::Overflow::Wrap, ir::Overflow::Saturate, ir::Overflow::Trap};
constexpr std::array kConventions = {ir::CallingConvention::Cdecl,    ir::CallingConvention::Pascal,  ir::CallingConvention::Stdcall,
                                     ir::CallingConvention::Fastcall, ir::CallingConvention::Thiscall, ir::CallingConvention::Register,
                                     ir::CallingConvention::Interrupt};
constexpr std::array kSymKinds = {ir::SymKind::Local,  ir::SymKind::Param,    ir::SymKind::Field,    ir::SymKind::Method,
                                  ir::SymKind::Ctor,   ir::SymKind::Class,    ir::SymKind::EnumConst, ir::SymKind::TypeVar,
                                  ir::SymKind::Package, ir::SymKind::Global,  ir::SymKind::Function, ir::SymKind::Label,
                                  ir::SymKind::Property, ir::SymKind::Event};
constexpr std::array kPassModes = {ir::PassMode::ByValue, ir::PassMode::Ref, ir::PassMode::Out, ir::PassMode::In};

std::string_view encoding_text(ir::CharEncoding e) {
    switch (e) {
        case ir::CharEncoding::Utf16: return "utf16";
        case ir::CharEncoding::Utf8: return "utf8";
        case ir::CharEncoding::Utf32: return "utf32";
    }
    return "utf16";
}
std::optional<ir::CharEncoding> encoding_from(std::string_view text) {
    for (ir::CharEncoding e : {ir::CharEncoding::Utf16, ir::CharEncoding::Utf8, ir::CharEncoding::Utf32}) {
        if (encoding_text(e) == text) return e;
    }
    return std::nullopt;
}

std::string_view register_text(ir::SegmentRegister r) {
    switch (r) {
        case ir::SegmentRegister::Ds: return "ds";
        case ir::SegmentRegister::Ss: return "ss";
        case ir::SegmentRegister::Cs: return "cs";
        case ir::SegmentRegister::Es: return "es";
        case ir::SegmentRegister::Unknown: return "unknown";
    }
    return "unknown";
}
std::optional<ir::SegmentRegister> register_from(std::string_view text) {
    for (auto r : {ir::SegmentRegister::Ds, ir::SegmentRegister::Ss, ir::SegmentRegister::Cs, ir::SegmentRegister::Es,
                   ir::SegmentRegister::Unknown}) {
        if (register_text(r) == text) return r;
    }
    return std::nullopt;
}

std::string_view flag_text(ir::Flag f) {
    switch (f) {
        case ir::Flag::CF: return "CF";
        case ir::Flag::ZF: return "ZF";
        case ir::Flag::SF: return "SF";
        case ir::Flag::OF: return "OF";
    }
    return "CF";
}

std::string_view role_text(ir::RecordRole r) {
    switch (r) {
        case ir::RecordRole::Struct: return "struct";
        case ir::RecordRole::Union: return "union";
        case ir::RecordRole::Frame: return "frame";
        case ir::RecordRole::Segment: return "segment";
    }
    return "struct";
}

// ---- reading helpers --------------------------------------------------------------------------------------------------

const Json* member(const Json& obj, const char* key) {
    if (!obj.is_object()) return nullptr;
    const auto it = obj.find(key);
    return (it == obj.end() || it->is_null()) ? nullptr : &*it;
}

const Json& need(const Json& obj, const char* key, const char* where) {
    const Json* m = member(obj, key);
    if (m == nullptr) fail(std::format("{} has no \"{}\"", where, key));
    return *m;
}

std::string str_of(const Json& v, const char* what) {
    if (!v.is_string()) fail(std::format("{} is not a string", what));
    return v.get<std::string>();
}

std::int64_t int_of(const Json& v, const char* what) {
    if (!v.is_number_integer()) fail(std::format("{} is not an integer", what));
    return v.get<std::int64_t>();
}

std::uint64_t uint_of(const Json& v, const char* what) {
    if (!v.is_number_integer()) fail(std::format("{} is not an integer", what));
    if (v.is_number_unsigned()) return v.get<std::uint64_t>();
    const std::int64_t n = v.get<std::int64_t>();
    if (n < 0) fail(std::format("{} is negative", what));
    return static_cast<std::uint64_t>(n);
}

bool bool_of(const Json& v) {
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number()) return v.get<double>() != 0.0;
    return !v.is_null() && !(v.is_string() && v.get<std::string>().empty());
}

std::vector<std::string> strs_of(const Json& v, const char* what) {
    if (!v.is_array()) fail(std::format("{} is not a list", what));
    std::vector<std::string> out;
    for (const Json& x : v) out.push_back(str_of(x, what));
    return out;
}

// ---- the document: entities and variables kept once, and the facts only one lane has ---------------------------------------
//
// `to_json` and `from_json` of a whole document keep each entity once, in a table at the root ("entities": [{family, kind, key}], and
// a type or symbol says {"e": index}), and each variable or entity symbol once ("vars": [...], and a use says {"v": index}), so a
// variable is one object across a document, as it is in the tree. A symbol that has neither a `role` nor an entity is written whole
// at every use, as before. A type or symbol written alone (`type_to_json`) has its entity inline, and cannot be read back alone.
// `Info` (of a node, type or symbol) is written as an object keyed by `info_keys()`; any other key is refused, both ways.

struct DocContext {
    // writing
    std::unordered_map<std::string, std::size_t> entity_at;
    std::vector<ir::EntityP> entities_out;
    std::unordered_map<const ir::Sym*, std::size_t> var_at;
    std::vector<ir::SymP> vars_out;
    // reading
    std::vector<ir::EntityP> entities_in;
    std::vector<ir::SymP> vars_in;
    const ir::EntityScope* scope = nullptr;
};
thread_local DocContext* g_doc = nullptr;

class DocScope {
public:
    explicit DocScope(DocContext* doc) : saved_(g_doc) { g_doc = doc; }
    ~DocScope() { g_doc = saved_; }
    DocScope(const DocScope&) = delete;
    DocScope& operator=(const DocScope&) = delete;

private:
    DocContext* saved_;
};

Json entity_text_json(const ir::Entity& e) {
    return Json{{"family", std::string(e.family())}, {"kind", std::string(ir::to_text(e.kind()))}, {"key", e.key()}};
}

Json entity_json(const ir::EntityP& e) {
    if (!e) fail("an entity is absent");
    if (g_doc == nullptr) return entity_text_json(*e);   // (alone: inline)
    const std::string id = std::string(e->family()) + '\x1f' + std::string(ir::to_text(e->kind())) + '\x1f' + e->key();
    const auto [it, fresh] = g_doc->entity_at.try_emplace(id, g_doc->entities_out.size());
    if (fresh) g_doc->entities_out.push_back(e);
    return Json(it->second);
}

ir::EntityP entity_from(const Json& j) {
    if (!j.is_number_integer()) fail("an entity is an index into the document's entity table: an entity alone cannot be read, only a whole document with its scope");
    const std::uint64_t at = uint_of(j, "entity index");
    if (g_doc == nullptr || at >= g_doc->entities_in.size()) fail(std::format("entity {} is not in the document's entity table", at));
    return g_doc->entities_in[at];
}

bool kind_allowed(std::string_view kinds, std::string_view kind) {
    std::size_t at = 0;
    while (at < kinds.size()) {
        std::size_t end = kinds.find(' ', at);
        if (end == std::string_view::npos) end = kinds.size();
        if (kinds.substr(at, end - at) == kind) return true;
        at = end + 1;
    }
    return false;
}

[[noreturn]] void bad_info(std::string_view key, const char* why) { fail(std::format("info \"{}\" {}", key, why)); }

const ir::InfoKey& checked_info_key(std::string_view key, ir::InfoOn on, std::string_view node_kind) {
    const ir::InfoKey* k = ir::find_info_key(key);
    if (k == nullptr) bad_info(key, "is not an L2 info key (config/translate/l2.schema.json, InfoKeys)");
    if (k->on != on) bad_info(key, "does not belong on this kind of thing (node, type or symbol)");
    if (on == ir::InfoOn::Node && !kind_allowed(k->kinds, node_kind)) bad_info(key, "may not sit on this kind of node");
    return *k;
}

Json info_value_json(const ir::InfoKey& k, const ir::InfoValue& v) {
    switch (k.value) {
        case ir::InfoValueKind::Bool:
            if (const auto* x = std::get_if<bool>(&v)) return Json(*x);
            break;
        case ir::InfoValueKind::Int:
            if (const auto* x = std::get_if<std::int64_t>(&v)) return Json(*x);
            break;
        case ir::InfoValueKind::String:
            if (const auto* x = std::get_if<std::string>(&v)) return Json(*x);
            break;
        case ir::InfoValueKind::Type:
            if (const auto* x = std::get_if<ir::TypeP>(&v)) return type_to_json(*x);
            break;
        case ir::InfoValueKind::Entity:
            if (const auto* x = std::get_if<ir::EntityP>(&v)) return entity_json(*x);
            break;
        case ir::InfoValueKind::Sym:
            if (const auto* x = std::get_if<ir::SymP>(&v)) return sym_to_json(*x);
            break;
        case ir::InfoValueKind::Ints:
            if (const auto* x = std::get_if<std::vector<std::int64_t>>(&v)) return Json(*x);
            break;
        case ir::InfoValueKind::Strings:
            if (const auto* x = std::get_if<std::vector<std::string>>(&v)) return Json(*x);
            break;
        case ir::InfoValueKind::NodeRefs:
            if (const auto* x = std::get_if<ir::NodeRefs>(&v)) return Json(x->at);
            break;
        case ir::InfoValueKind::Types:
            if (const auto* x = std::get_if<std::vector<ir::TypeP>>(&v)) {
                Json out = Json::array();
                for (const ir::TypeP& t : *x) out.push_back(type_to_json(t));
                return out;
            }
            break;
    }
    bad_info(k.key, "holds a value of another kind than the table says");
}

ir::InfoValue info_value_from(const ir::InfoKey& k, const Json& j) {
    switch (k.value) {
        case ir::InfoValueKind::Bool:
            if (!j.is_boolean()) bad_info(k.key, "is not a boolean");
            return j.get<bool>();
        case ir::InfoValueKind::Int: return int_of(j, "an info value");
        case ir::InfoValueKind::String: return str_of(j, "an info value");
        case ir::InfoValueKind::Type: return type_from_json(j);
        case ir::InfoValueKind::Entity: return entity_from(j);
        case ir::InfoValueKind::Sym: return sym_from_json(j);
        case ir::InfoValueKind::Ints: {
            if (!j.is_array()) bad_info(k.key, "is not a list");
            std::vector<std::int64_t> out;
            for (const Json& x : j) out.push_back(int_of(x, "an info value"));
            return out;
        }
        case ir::InfoValueKind::Strings: return strs_of(j, "an info value");
        case ir::InfoValueKind::NodeRefs: {
            if (!j.is_array()) bad_info(k.key, "is not a list");
            ir::NodeRefs out;
            for (const Json& x : j) out.at.push_back(static_cast<std::uint32_t>(uint_of(x, "a node reference")));
            return out;
        }
        case ir::InfoValueKind::Types: {
            if (!j.is_array()) bad_info(k.key, "is not a list");
            std::vector<ir::TypeP> out;
            for (const Json& x : j) out.push_back(type_from_json(x));
            return out;
        }
    }
    bad_info(k.key, "has no value kind");
}

Json info_json(const ir::InfoP& info, ir::InfoOn on, std::string_view node_kind) {
    Json j = Json::object();
    for (const auto& [key, value] : info.get()->items()) j[key] = info_value_json(checked_info_key(key, on, node_kind), value);
    return j;
}

ir::InfoP info_from_json(const Json& j, ir::InfoOn on, std::string_view node_kind) {
    if (!j.is_object()) fail("info is an object");
    ir::InfoP out;
    for (auto it = j.begin(); it != j.end(); ++it) out.set(it.key(), info_value_from(checked_info_key(it.key(), on, node_kind), it.value()));
    return out;
}

// ---- segments, locations, origins -------------------------------------------------------------------------------------

Json segment_json(const ir::SegmentRef& seg) {
    return std::visit(
        [](const auto& s) -> Json {
            using S = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<S, ir::SegmentRegister>) {
                return std::string(register_text(s));
            } else if constexpr (std::is_same_v<S, ir::ModuleSegment>) {
                return Json{{"module_segment", s.index}};
            } else {
                Json j{{"import", s.import}};
                if (s.part) j["part"] = *s.part == ir::ImportPart::Segment ? "segment" : "offset";
                return j;
            }
        },
        seg);
}

ir::SegmentRef segment_from(const Json& j) {
    if (j.is_string()) {
        if (auto r = register_from(j.get<std::string>())) return *r;
        fail("unknown segment register " + j.get<std::string>());
    }
    if (const Json* m = member(j, "module_segment")) return ir::ModuleSegment{static_cast<std::uint32_t>(uint_of(*m, "module_segment"))};
    if (const Json* m = member(j, "import")) {
        ir::ImportedSegment s{str_of(*m, "import"), std::nullopt};
        if (const Json* p = member(j, "part")) s.part = str_of(*p, "part") == "offset" ? ir::ImportPart::Offset : ir::ImportPart::Segment;
        return s;
    }
    fail("a segment is a register name, {module_segment} or {import}");
}

Json location_json(const ir::Location& at) {
    return std::visit(
        [](const auto& l) -> Json {
            using L = std::decay_t<decltype(l)>;
            if constexpr (std::is_same_v<L, ir::NoLocation>) return "none";
            else if constexpr (std::is_same_v<L, ir::StackSlot>) return Json{{"stack", l.offset}};
            else if constexpr (std::is_same_v<L, ir::Register>) return Json{{"reg", l.name}};
            else if constexpr (std::is_same_v<L, ir::RegisterPair>) return Json{{"regs", l.names}};
            else return Json{{"flag", std::string(flag_text(l.flag))}};
        },
        at);
}

ir::Location location_from(const Json* j) {
    if (j == nullptr || j->is_string()) return ir::NoLocation{};
    if (const Json* m = member(*j, "stack")) return ir::StackSlot{int_of(*m, "stack")};
    if (const Json* m = member(*j, "reg")) return ir::Register{str_of(*m, "reg")};
    if (const Json* m = member(*j, "regs")) return ir::RegisterPair{strs_of(*m, "regs")};
    if (const Json* m = member(*j, "flag")) {
        const std::string f = str_of(*m, "flag");
        for (ir::Flag flag : {ir::Flag::CF, ir::Flag::ZF, ir::Flag::SF, ir::Flag::OF}) {
            if (flag_text(flag) == f) return ir::FlagBit{flag};
        }
        fail("unknown flag " + f);
    }
    fail("a location is \"none\", {stack}, {reg}, {regs} or {flag}");
}

Json origin_json(const ir::Origin& origin) {
    return std::visit(
        [](const auto& o) -> Json {
            using O = std::decay_t<decltype(o)>;
            if constexpr (std::is_same_v<O, ir::OriginSegment>) return Json{{"module", o.module}, {"segment", o.segment}, {"offset", o.offset}};
            else if constexpr (std::is_same_v<O, ir::OriginVa>) return Json{{"module", o.module}, {"va", o.va}};
            else if constexpr (std::is_same_v<O, ir::OriginSpan>) return Json{{"start", o.start}, {"end", o.end}};
            else return Json{{"at", o.at}};
        },
        origin);
}

ir::Origin origin_from(const Json& j) {
    if (const Json* at = member(j, "at")) {
        ir::OriginAddresses a;
        if (!at->is_array()) fail("origin at is not a list");
        for (const Json& x : *at) a.at.push_back(uint_of(x, "origin address"));
        return a;
    }
    if (const Json* start = member(j, "start")) {
        ir::OriginSpan s;
        s.start = static_cast<std::uint32_t>(uint_of(*start, "origin start"));
        s.end = static_cast<std::uint32_t>(uint_of(need(j, "end", "an origin span"), "origin end"));
        return s;
    }
    const std::string module = member(j, "module") ? str_of(*member(j, "module"), "origin module") : std::string();
    if (const Json* va = member(j, "va")) return ir::OriginVa{module, uint_of(*va, "origin va")};
    if (const Json* seg = member(j, "segment")) {
        return ir::OriginSegment{module, int_of(*seg, "origin segment"), member(j, "offset") ? int_of(*member(j, "offset"), "origin offset") : 0};
    }
    fail("an origin has {module, va}, {module, segment, offset}, {at} or {start, end}");
}

// ---- types ------------------------------------------------------------------------------------------------------------

Json fn_param_json(const ir::FnParam& p) {
    Json j = Json::object();
    if (p.name) j["name"] = *p.name;
    j["type"] = type_to_json(p.type);
    if (!std::holds_alternative<ir::NoLocation>(p.at)) j["at"] = location_json(p.at);
    return j;
}

}  // namespace

Json type_to_json(const ir::TypeP& type) {
    if (!type) return Json(nullptr);
    Json out = std::visit(
        [](const auto& t) -> Json {
            using T = std::decay_t<decltype(t)>;
            Json j = Json::object();
            if constexpr (std::is_same_v<T, ir::PrimType>) {
                j["kind"] = std::string(ir::to_text(t.prim));
                if (t.is_signed) j["signed"] = *t.is_signed;
                if (t.encoding) j["enc"] = std::string(encoding_text(*t.encoding));
            } else if constexpr (std::is_same_v<T, ir::VoidType>) {
                j["kind"] = "void";
            } else if constexpr (std::is_same_v<T, ir::NullType>) {
                j["kind"] = "null";
            } else if constexpr (std::is_same_v<T, ir::AnyType>) {
                j["kind"] = "any";
            } else if constexpr (std::is_same_v<T, ir::DynamicType>) {
                j["kind"] = "dynamic";
            } else if constexpr (std::is_same_v<T, ir::ClassType>) {
                j["kind"] = "class";
                j["name"] = t.name;
                if (!t.args.empty()) {
                    Json args = Json::array();
                    for (const ir::TypeP& a : t.args) args.push_back(type_to_json(a));
                    j["args"] = std::move(args);
                }
                if (t.nullable) j["nullable"] = true;
                if (t.record) j["record"] = *t.record;
                if (t.ref) j["ref"] = *t.ref;
                if (t.exception) j["exception"] = *t.exception;
            } else if constexpr (std::is_same_v<T, ir::TypeVarType>) {
                j["kind"] = "typevar";
                j["name"] = t.name;
                if (t.index) j["index"] = *t.index;
                if (t.owner) j["owner"] = *t.owner;
                if (t.of_method) j["of_method"] = true;
            } else if constexpr (std::is_same_v<T, ir::WildcardType>) {
                j["kind"] = "wildcard";
                if (t.bound) {
                    j["bound"] = t.bound->mode == ir::BoundMode::Extends ? "extends" : "super";
                    j["type"] = type_to_json(t.bound->type);
                }
            } else if constexpr (std::is_same_v<T, ir::ArrayType>) {
                j["kind"] = "array";
                j["elem"] = type_to_json(t.elem);
                if (t.count) j["count"] = *t.count;
                if (t.rank != 1) j["rank"] = t.rank;
            } else if constexpr (std::is_same_v<T, ir::PtrType>) {
                j["kind"] = "ptr";
                j["space"] = std::string(ir::to_text(t.space));
                j["to"] = type_to_json(t.to);
                if (t.seg) j["seg"] = segment_json(*t.seg);
                if (t.ref) j["ref"] = true;
                if (t.managed) j["managed"] = true;
            } else if constexpr (std::is_same_v<T, ir::SelectorType>) {
                j["kind"] = "selector";
                if (t.of) j["of"] = segment_json(*t.of);
            } else if constexpr (std::is_same_v<T, ir::FnType>) {
                j["kind"] = "fn";
                j["conv"] = std::string(ir::to_text(t.conv));
                Json params = Json::array();
                for (const ir::FnParam& p : t.params) params.push_back(fn_param_json(p));
                j["params"] = std::move(params);
                Json result = Json::object();
                if (t.result.type) result["type"] = type_to_json(t.result.type);
                result["at"] = location_json(t.result.at);
                j["result"] = std::move(result);
                if (t.distance) j["distance"] = std::string(ir::to_text(*t.distance));
                if (t.pops) j["pops"] = *t.pops;
                if (t.loadds) j["loadds"] = true;
                if (!t.preserves.empty()) j["preserves"] = t.preserves;
                if (t.variadic) j["variadic"] = true;
                if (t.has_this) j["has_this"] = true;
                if (t.explicit_this) j["explicit_this"] = true;
            } else if constexpr (std::is_same_v<T, ir::FnPtrType>) {
                j["kind"] = "fnptr";
                j["space"] = std::string(ir::to_text(t.space));
                j["fn"] = type_to_json(t.fn);
            } else if constexpr (std::is_same_v<T, ir::TupleType>) {
                j["kind"] = "tuple";
                Json elems = Json::array();
                for (const ir::TypeP& e : t.elems) elems.push_back(type_to_json(e));
                j["elems"] = std::move(elems);
                bool named = false;
                for (const auto& n : t.names) named = named || n.has_value();
                if (named) {
                    Json names = Json::array();
                    for (const auto& n : t.names) names.push_back(n ? Json(*n) : Json(nullptr));
                    j["names"] = std::move(names);
                }
            } else if constexpr (std::is_same_v<T, ir::ModType>) {
                j["kind"] = "mod";
                j["inner"] = type_to_json(t.inner);
                j["required"] = t.required;
                j["modifier"] = type_to_json(t.modifier);
            } else {
                static_assert(std::is_same_v<T, ir::RecordType>);
                j["kind"] = "record";
                j["size"] = t.size;
                Json fields = Json::array();
                for (const ir::RecordField& f : t.fields) {
                    Json fj{{"name", f.name}, {"offset", f.offset}, {"type", type_to_json(f.type)}};
                    if (f.pad) fj["pad"] = true;
                    fields.push_back(std::move(fj));
                }
                j["fields"] = std::move(fields);
                if (t.name && !t.name->empty()) j["name"] = *t.name;
                if (t.role) j["role"] = std::string(role_text(*t.role));
                if (t.packed) j["packed"] = *t.packed;
            }
            return j;
        },
        type->body);
    if (type->entity) out["e"] = entity_json(type->entity);
    if (type->info) out["info"] = info_json(type->info, ir::InfoOn::Type, {});
    return out;
}

namespace {

ir::TypeP type_list_item(const Json& j) { return type_from_json(j); }

ir::FnParam fn_param_from(const Json& j) {
    ir::FnParam p;
    if (const Json* n = member(j, "name")) p.name = str_of(*n, "parameter name");
    p.type = type_from_json(need(j, "type", "a parameter"));
    p.at = location_from(member(j, "at"));
    return p;
}

}  // namespace

namespace {

ir::TypeP type_core_from_json(const Json& d) {
    if (d.is_null()) return {};
    if (!d.is_object()) fail("an L2 type is an object");
    const std::string kind = str_of(need(d, "kind", "a type"), "type kind");
    if (kind == "void") return ir::make_type(ir::VoidType{});
    if (kind == "null") return ir::null_type();
    if (kind == "any") return ir::any_type();
    if (kind == "dynamic") return ir::make_type(ir::DynamicType{});
    if (const auto prim = enum_from(kind, kPrims)) {
        ir::PrimType t{*prim, std::nullopt, std::nullopt};
        if (const Json* s = member(d, "signed")) t.is_signed = bool_of(*s);
        if (const Json* e = member(d, "enc")) {
            t.encoding = encoding_from(str_of(*e, "enc"));
            if (!t.encoding) fail("unknown character encoding " + str_of(*e, "enc"));
        }
        return ir::make_type(t);
    }
    if (kind == "class") {
        ir::ClassType t;
        t.name = str_of(need(d, "name", "a class type"), "class name");
        if (const Json* a = member(d, "args")) {
            if (!a->is_array()) fail("class type args are not a list");
            for (const Json& x : *a) t.args.push_back(type_list_item(x));
        }
        if (const Json* n = member(d, "nullable")) t.nullable = bool_of(*n);
        if (const Json* v = member(d, "record")) t.record = bool_of(*v);
        if (const Json* v = member(d, "ref")) t.ref = bool_of(*v);
        if (const Json* v = member(d, "exception")) t.exception = bool_of(*v);
        return ir::make_type(std::move(t));
    }
    if (kind == "typevar") {
        ir::TypeVarType t;
        t.name = str_of(need(d, "name", "a type variable"), "type variable name");
        if (const Json* v = member(d, "index")) t.index = static_cast<std::uint32_t>(uint_of(*v, "type variable index"));
        if (const Json* v = member(d, "owner")) t.owner = str_of(*v, "type variable owner");
        if (const Json* v = member(d, "of_method")) t.of_method = bool_of(*v);
        return ir::make_type(std::move(t));
    }
    if (kind == "wildcard") {
        ir::WildcardType t;
        if (const Json* b = member(d, "bound")) {
            ir::WildcardBound bound;
            bound.mode = str_of(*b, "wildcard bound") == "super" ? ir::BoundMode::Super : ir::BoundMode::Extends;
            bound.type = type_from_json(need(d, "type", "a wildcard bound"));
            t.bound = std::move(bound);
        }
        return ir::make_type(std::move(t));
    }
    if (kind == "array") {
        ir::ArrayType t;
        t.elem = type_from_json(need(d, "elem", "an array type"));
        if (const Json* c = member(d, "count")) t.count = uint_of(*c, "array count");
        if (const Json* r = member(d, "rank")) t.rank = static_cast<std::uint32_t>(uint_of(*r, "array rank"));
        return ir::make_type(std::move(t));
    }
    if (kind == "ptr") {
        ir::PtrType t;
        const std::string space = str_of(need(d, "space", "a pointer type"), "address space");
        const auto s = enum_from(space, kSpaces);
        if (!s) fail("unknown address space " + space);
        t.space = *s;
        t.to = type_from_json(need(d, "to", "a pointer type"));
        if (const Json* g = member(d, "seg")) t.seg = segment_from(*g);
        if (const Json* r = member(d, "ref")) t.ref = bool_of(*r);
        if (const Json* m = member(d, "managed")) t.managed = bool_of(*m);
        return ir::make_type(std::move(t));
    }
    if (kind == "selector") {
        ir::SelectorType t;
        if (const Json* o = member(d, "of")) t.of = segment_from(*o);
        return ir::make_type(std::move(t));
    }
    if (kind == "fn") {
        ir::FnType t;
        const std::string conv = member(d, "conv") ? str_of(*member(d, "conv"), "conv") : "cdecl";
        const auto c = enum_from(conv, kConventions);
        if (!c) fail("unknown calling convention " + conv);
        t.conv = *c;
        if (const Json* v = member(d, "distance")) {
            t.distance = enum_from(str_of(*v, "distance"), kSpaces);
            if (!t.distance) fail("unknown distance " + str_of(*v, "distance"));
        }
        if (const Json* v = member(d, "pops")) t.pops = int_of(*v, "pops");
        if (const Json* v = member(d, "loadds")) t.loadds = bool_of(*v);
        for (const Json& p : need(d, "params", "a function type")) t.params.push_back(fn_param_from(p));
        const Json& result = need(d, "result", "a function type");
        if (const Json* rt = member(result, "type")) t.result.type = type_from_json(*rt);
        t.result.at = location_from(member(result, "at"));
        if (const Json* v = member(d, "preserves")) t.preserves = strs_of(*v, "preserves");
        if (const Json* v = member(d, "variadic")) t.variadic = bool_of(*v);
        if (const Json* v = member(d, "has_this")) t.has_this = bool_of(*v);
        if (const Json* v = member(d, "explicit_this")) t.explicit_this = bool_of(*v);
        return ir::make_type(std::move(t));
    }
    if (kind == "fnptr") {
        ir::FnPtrType t;
        const std::string space = str_of(need(d, "space", "a function pointer type"), "address space");
        const auto s = enum_from(space, kSpaces);
        if (!s) fail("unknown address space " + space);
        t.space = *s;
        t.fn = type_from_json(need(d, "fn", "a function pointer type"));
        return ir::make_type(std::move(t));
    }
    if (kind == "tuple") {
        ir::TupleType t;
        const Json& elems = need(d, "elems", "a tuple type");
        if (!elems.is_array()) fail("tuple elems are not a list");
        for (const Json& e : elems) t.elems.push_back(type_from_json(e));
        if (const Json* names = member(d, "names")) {
            if (!names->is_array() || names->size() != elems.size()) fail("tuple names are a list as long as its elems");
            for (const Json& n : *names) t.names.push_back(n.is_null() ? std::nullopt : std::optional<std::string>(str_of(n, "tuple element name")));
        }
        return ir::make_type(std::move(t));
    }
    if (kind == "mod") {
        ir::ModType t;
        t.inner = type_from_json(need(d, "inner", "a modified type"));
        if (const Json* r = member(d, "required")) t.required = bool_of(*r);
        t.modifier = type_from_json(need(d, "modifier", "a modified type"));
        return ir::make_type(std::move(t));
    }
    if (kind == "record") {
        ir::RecordType t;
        t.size = uint_of(need(d, "size", "a record type"), "record size");
        if (const Json* n = member(d, "name")) t.name = str_of(*n, "record name");
        if (const Json* r = member(d, "role")) {
            const std::string role = str_of(*r, "record role");
            for (ir::RecordRole x : {ir::RecordRole::Struct, ir::RecordRole::Union, ir::RecordRole::Frame, ir::RecordRole::Segment}) {
                if (role_text(x) == role) t.role = x;
            }
            if (!t.role) fail("unknown record role " + role);
        }
        if (const Json* p = member(d, "packed")) t.packed = bool_of(*p);
        for (const Json& f : need(d, "fields", "a record type")) {
            ir::RecordField field;
            field.name = str_of(need(f, "name", "a record field"), "field name");
            field.offset = uint_of(need(f, "offset", "a record field"), "field offset");
            field.type = type_from_json(need(f, "type", "a record field"));
            if (const Json* p = member(f, "pad")) field.pad = bool_of(*p);
            t.fields.push_back(std::move(field));
        }
        return ir::make_type(std::move(t));
    }
    fail("no IR form for L2 type kind " + kind);
}

}  // namespace

ir::TypeP type_from_json(const Json& d) {
    ir::TypeP t = type_core_from_json(d);
    if (!t) return t;
    const Json* e = member(d, "e");
    const Json* info = member(d, "info");
    if (e == nullptr && info == nullptr) return t;   // (the common case: the shared singletons stay)
    return ir::make_type(t->body, e != nullptr ? entity_from(*e) : nullptr, info != nullptr ? info_from_json(*info, ir::InfoOn::Type, {}) : ir::InfoP{});
}

// ---- symbols ----------------------------------------------------------------------------------------------------------

namespace {

// A variable or an entity's symbol is one object in the tree, so a document says it once (see the document's section above).
bool tabled(const ir::Sym& s) { return s.role.has_value() || s.entity != nullptr; }

Json sym_body_json(const ir::Sym& sym) {
    const ir::Sym* s = &sym;
    Json j{{"kind", std::string(ir::to_text(s->kind))}, {"name", s->name}};
    if (s->owner) j["owner"] = *s->owner;
    if (s->is_static) j["static"] = true;
    if (s->type) j["type"] = type_to_json(s->type);
    if (!s->params.empty() || s->kind == ir::SymKind::Method || s->kind == ir::SymKind::Ctor || s->kind == ir::SymKind::Function) {
        Json params = Json::array();
        for (const ir::TypeP& p : s->params) params.push_back(type_to_json(p));
        j["params"] = std::move(params);
    }
    if (s->ret) j["ret"] = type_to_json(s->ret);
    if (s->varargs) j["varargs"] = true;
    bool by_ref = false;
    for (ir::PassMode m : s->refs) by_ref = by_ref || m != ir::PassMode::ByValue;
    if (by_ref) {
        Json refs = Json::array();
        for (ir::PassMode m : s->refs) refs.push_back(std::string(ir::to_text(m)));
        j["refs"] = std::move(refs);
    }
    if (!s->flags.empty()) j["flags"] = s->flags;
    if (s->role) j["role"] = std::string(ir::to_text(*s->role));
    if (s->index) j["index"] = *s->index;
    if (s->stack != ir::Stack::Unknown) j["stk"] = std::string(ir::to_text(s->stack));
    if (s->generated_name) j["generated_name"] = true;
    if (s->ref_readonly) j["ref_readonly"] = true;
    if (s->entry_initialized) j["entry_initialized"] = true;
    if (s->entry_used) j["entry_used"] = true;
    if (s->entity) j["e"] = entity_json(s->entity);
    if (s->info) j["info"] = info_json(s->info, ir::InfoOn::Sym, {});
    return j;
}

}  // namespace

Json sym_to_json(const ir::SymP& s) {
    if (!s) return Json(nullptr);
    if (g_doc != nullptr && tabled(*s)) {
        const auto [it, fresh] = g_doc->var_at.try_emplace(s.get(), g_doc->vars_out.size());
        if (fresh) g_doc->vars_out.push_back(s);
        return Json{{"v", it->second}};
    }
    return sym_body_json(*s);
}

namespace {

// Fills a symbol from its document (a whole one, not a reference).
void fill_sym(ir::Sym& sym, const Json& d) {
    ir::Sym* s = &sym;
    if (!d.is_object()) fail("an L2 symbol is an object");
    const std::string kind = str_of(need(d, "kind", "a symbol"), "symbol kind");
    const auto k = enum_from(kind, kSymKinds);
    if (!k) fail("unknown symbol kind " + kind);
    s->kind = *k;
    s->name = str_of(need(d, "name", "a symbol"), "symbol name");
    if (const Json* v = member(d, "role")) {
        s->role = ir::from_text<ir::VarRole>(str_of(*v, "symbol role"));
        if (!s->role) fail("unknown variable role " + str_of(*v, "symbol role"));
    }
    if (const Json* v = member(d, "index")) s->index = static_cast<std::int32_t>(int_of(*v, "symbol index"));
    if (const Json* v = member(d, "stk")) {
        const auto st = ir::from_text<ir::Stack>(str_of(*v, "symbol stk"));
        if (!st) fail("unknown stack kind " + str_of(*v, "symbol stk"));
        s->stack = *st;
    }
    if (const Json* v = member(d, "generated_name")) s->generated_name = bool_of(*v);
    if (const Json* v = member(d, "ref_readonly")) s->ref_readonly = bool_of(*v);
    if (const Json* v = member(d, "entry_initialized")) s->entry_initialized = bool_of(*v);
    if (const Json* v = member(d, "entry_used")) s->entry_used = bool_of(*v);
    if (const Json* v = member(d, "e")) s->entity = entity_from(*v);
    if (const Json* v = member(d, "info")) s->info = info_from_json(*v, ir::InfoOn::Sym, {});
    if (const Json* v = member(d, "owner")) s->owner = str_of(*v, "symbol owner");

    if (const Json* v = member(d, "static")) s->is_static = bool_of(*v);
    if (const Json* v = member(d, "type")) s->type = type_from_json(*v);
    if (const Json* v = member(d, "params")) {
        if (!v->is_array()) fail("symbol params are not a list");
        for (const Json& p : *v) s->params.push_back(type_from_json(p));
    }
    if (const Json* v = member(d, "ret")) s->ret = type_from_json(*v);
    if (const Json* v = member(d, "varargs")) s->varargs = bool_of(*v);
    if (const Json* v = member(d, "refs")) {
        for (const std::string& r : strs_of(*v, "symbol refs")) {
            const auto m = enum_from(r, kPassModes);
            if (!m) fail("unknown pass mode " + r);
            s->refs.push_back(*m);
        }
    }
    if (const Json* v = member(d, "flags")) s->flags = strs_of(*v, "symbol flags");
}

}  // namespace

ir::SymP sym_from_json(const Json& d) {
    if (d.is_null()) return {};
    if (const Json* v = member(d, "v")) {
        const std::uint64_t at = uint_of(*v, "variable index");
        if (g_doc == nullptr || at >= g_doc->vars_in.size()) fail(std::format("variable {} is not in the document's variable table", at));
        return g_doc->vars_in[at];
    }
    auto s = std::make_shared<ir::Sym>();
    fill_sym(*s, d);
    return s;
}

// ---- nodes ------------------------------------------------------------------------------------------------------------
//
// Every kind lists its slots once, in the order a document writes them (`describe`), to a visitor that either writes the
// slot into a document (Writer) or reads it from one (Reader): one description, both directions.

namespace {

Json node_json(const Node& node);

Json value_json(const ir::LitValue& v) {
    return std::visit(
        [](const auto& x) -> Json {
            using X = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<X, std::monostate>) return Json(nullptr);
            else if constexpr (std::is_same_v<X, double>) {
                if (std::isfinite(x)) return Json(x);
                return std::isnan(x) ? "nan" : (x > 0 ? "inf" : "-inf");   // (JSON has no form for them: text)
            } else return Json(x);
        },
        v);
}

ir::LitValue value_from(const Json& j) {
    if (j.is_null()) return std::monostate{};
    if (j.is_boolean()) return j.get<bool>();
    if (j.is_number_float()) return j.get<double>();
    if (j.is_number_integer()) {
        if (j.is_number_unsigned() && j.get<std::uint64_t>() > static_cast<std::uint64_t>(INT64_MAX)) return j.get<std::uint64_t>();
        return j.get<std::int64_t>();
    }
    if (j.is_string()) return j.get<std::string>();
    fail("a literal's value is null, a boolean, a number or text");
}

Json target_json(const ir::CompilerTarget& t) {
    Json j = Json::object();
    if (t.triple) j["triple"] = *t.triple;
    if (t.pointer) j["pointer"] = *t.pointer;
    if (t.long_bits) j["long"] = *t.long_bits;
    if (t.wchar) j["wchar"] = *t.wchar;
    if (t.char_signed) j["char_signed"] = *t.char_signed;
    if (t.language) j["language"] = *t.language;
    if (t.standard) j["std"] = *t.standard;
    return j;
}

ir::CompilerTarget target_from(const Json& j) {
    ir::CompilerTarget t;
    if (const Json* v = member(j, "triple")) t.triple = str_of(*v, "target triple");
    if (const Json* v = member(j, "pointer")) t.pointer = static_cast<std::uint32_t>(uint_of(*v, "target pointer"));
    if (const Json* v = member(j, "long")) t.long_bits = static_cast<std::uint32_t>(uint_of(*v, "target long"));
    if (const Json* v = member(j, "wchar")) t.wchar = static_cast<std::uint32_t>(uint_of(*v, "target wchar"));
    if (const Json* v = member(j, "char_signed")) t.char_signed = bool_of(*v);
    if (const Json* v = member(j, "language")) t.language = str_of(*v, "target language");
    if (const Json* v = member(j, "std")) t.standard = str_of(*v, "target std");
    return t;
}

Json typearg_json(const ir::TypeArg& a) {
    return std::visit(
        [](const auto& x) -> Json {
            using X = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<X, ir::TypeP>) return type_to_json(x);
            else return Json(x);
        },
        a);
}

ir::TypeArg typearg_from(const Json& j) {
    if (j.is_object()) return type_from_json(j);
    if (j.is_string()) return j.get<std::string>();
    if (j.is_number_integer()) return j.get<std::int64_t>();
    fail("a type argument is a type, a format string or a count");
}

// A node's prefix bits as a list of names; a name or a bit this build does not know is refused (both ways).
Json flags_json(std::uint32_t flags) {
    Json list = Json::array();
    std::uint32_t known = 0;
    for (ir::NodeFlag f : ir::all_node_flags()) {
        known |= ir::bit(f);
        if ((flags & ir::bit(f)) != 0) list.push_back(std::string(ir::to_text(f)));
    }
    if ((flags & ~known) != 0) fail(std::format("node flags {:#x} have a bit that is no NodeFlag", flags & ~known));
    return list;
}

std::uint32_t flags_from(const Json& j) {
    if (!j.is_array()) fail("fl is a list of flag names");
    std::uint32_t flags = 0;
    for (const Json& x : j) {
        const std::string name = str_of(x, "a flag name");
        const auto f = ir::from_text<ir::NodeFlag>(name);
        if (!f) fail("unknown node flag " + name);
        flags |= ir::bit(*f);
    }
    return flags;
}

// An Intrinsic's name is in `ir::intrinsic_names()` (the schema's IntrinsicNames): the rule that keeps it from becoming a dumping ground.
void check_intrinsic(const std::string& name) {
    if (ir::find_intrinsic(name) == nullptr) {
        fail(std::format("\"{}\" is not an L2 intrinsic name: a name needs a row in ir.cpp and in the schema's IntrinsicNames, with its reason and its test", name));
    }
}

class Writer {
public:
    explicit Writer(Json& out) : out_(out) {}

    void child(const char* key, Node* const& v) {
        if (v != nullptr) out_[key] = node_json(*v);
    }
    // A list slot the walk follows: written even when empty.
    void children(const char* key, const std::vector<Node*>& v) {
        Json list = Json::array();
        for (const Node* n : v) {
            if (n != nullptr) list.push_back(node_json(*n));
        }
        out_[key] = std::move(list);
    }
    // A list of nodes the walk does not follow (type parameters): written when it has some.
    void nodes(const char* key, const std::vector<Node*>& v) {
        if (!v.empty()) children(key, v);
    }
    // A list slot whose absence means something other than "empty" (a NewArray's initialiser: `new int[8]` has none, `new int[]{}`
    // has an empty one): written when it has some, or when `empty_means_present`.
    void children_unless_absent(const char* key, const std::vector<Node*>& v, bool empty_means_present) {
        if (!v.empty() || empty_means_present) children(key, v);
    }
    void suite(const char* key, const ir::Suite& v) {
        if (const auto* one = std::get_if<Node*>(&v)) {
            child(key, *one);
        } else {
            children(key, std::get<std::vector<Node*>>(v));
        }
    }
    void type(const char* key, const ir::TypeP& v) {
        if (v) out_[key] = type_to_json(v);
    }
    void types(const char* key, const std::vector<ir::TypeP>& v) {
        if (v.empty()) return;
        Json list = Json::array();
        for (const ir::TypeP& t : v) list.push_back(type_to_json(t));
        out_[key] = std::move(list);
    }
    void sym(const char* key, const ir::SymP& v) {
        if (v) out_[key] = sym_to_json(v);
    }
    void text(const char* key, const std::optional<std::string>& v) {
        if (v) out_[key] = *v;
    }
    void texts(const char* key, const std::vector<std::string>& v) {
        if (!v.empty()) out_[key] = v;
    }
    void flag(const char* key, bool v) {
        if (v) out_[key] = true;
    }
    void maybe(const char* key, const std::optional<bool>& v) {
        if (v) out_[key] = *v;
    }
    void integer(const char* key, const std::optional<std::int64_t>& v) {
        if (v) out_[key] = *v;
    }
    void unsigned_integer(const char* key, const std::optional<std::uint64_t>& v) {
        if (v) out_[key] = *v;
    }
    void count(const char* key, std::uint32_t v) { out_[key] = v; }
    void overflow(const char* key, const std::optional<ir::Overflow>& v) {
        if (v) out_[key] = std::string(ir::to_text(*v));
    }
    void memory(const char* key, const std::optional<ir::MemoryModel>& v) {
        if (v) out_[key] = std::string(ir::to_text(*v));
    }
    void value(const char* key, const ir::LitValue& v) {
        if (!std::holds_alternative<std::monostate>(v)) out_[key] = value_json(v);
    }
    void typeargs(const char* key, const std::vector<ir::TypeArg>& v) {
        if (v.empty()) return;
        Json list = Json::array();
        for (const ir::TypeArg& a : v) list.push_back(typearg_json(a));
        out_[key] = std::move(list);
    }
    void target(const char* key, const std::optional<ir::CompilerTarget>& v) {
        if (v) out_[key] = target_json(*v);
    }
    // An enumerated slot written when it is not its default (the stack kinds, lift, form ...); one written when present.
    template <class E> void named(const char* key, const E& v, E dflt) {
        if (v != dflt) out_[key] = std::string(ir::to_text(v));
    }
    template <class E> void opt_named(const char* key, const std::optional<E>& v) {
        if (v) out_[key] = std::string(ir::to_text(*v));
    }
    void syms(const char* key, const std::vector<ir::SymP>& v) {
        if (v.empty()) return;
        Json list = Json::array();
        for (const ir::SymP& s : v) list.push_back(sym_to_json(s));
        out_[key] = std::move(list);
    }
    void ints(const char* key, const std::vector<std::int64_t>& v) {
        if (!v.empty()) out_[key] = v;
    }
    void ranges(const char* key, const std::vector<std::pair<std::int64_t, std::int64_t>>& v) {
        if (v.empty()) return;
        Json list = Json::array();
        for (const auto& [low, high] : v) list.push_back(Json::array({low, high}));
        out_[key] = std::move(list);
    }
    void argmeta(const char* key, const std::vector<ir::ArgMeta>& v) {
        if (v.empty()) return;
        Json list = Json::array();
        for (const ir::ArgMeta& m : v) {
            Json one = Json::object();
            if (m.name) one["name"] = *m.name;
            if (m.flags != 0) one["flags"] = m.flags;
            if (m.type) one["type"] = type_to_json(m.type);
            list.push_back(std::move(one));
        }
        out_[key] = std::move(list);
    }

private:
    Json& out_;
};

Node* node_from(Arena& arena, const Json& j);

class Reader {
public:
    Reader(Arena& arena, const Json& in) : arena_(arena), in_(in) {}

    void child(const char* key, Node*& v) {
        if (const Json* j = member(in_, key)) v = slot(*j);
    }
    void children(const char* key, std::vector<Node*>& v) {
        if (const Json* j = member(in_, key)) v = list(*j);
    }
    void nodes(const char* key, std::vector<Node*>& v) { children(key, v); }
    void children_unless_absent(const char* key, std::vector<Node*>& v, bool) { children(key, v); }
    void suite(const char* key, ir::Suite& v) {
        const Json* j = member(in_, key);
        if (j == nullptr) return;
        if (j->is_array()) v = list(*j);
        else v = slot(*j);
    }
    void type(const char* key, ir::TypeP& v) {
        if (const Json* j = member(in_, key)) v = type_from_json(*j);
    }
    void types(const char* key, std::vector<ir::TypeP>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail(std::format("\"{}\" is not a list", key));
            v.clear();
            for (const Json& t : *j) v.push_back(type_from_json(t));
        }
    }
    void sym(const char* key, ir::SymP& v) {
        if (const Json* j = member(in_, key)) v = sym_from_json(*j);
    }
    void text(const char* key, std::optional<std::string>& v) {
        if (const Json* j = member(in_, key)) v = str_of(*j, key);
    }
    void texts(const char* key, std::vector<std::string>& v) {
        if (const Json* j = member(in_, key)) v = strs_of(*j, key);
    }
    void flag(const char* key, bool& v) {
        if (const Json* j = member(in_, key)) v = bool_of(*j);
    }
    void maybe(const char* key, std::optional<bool>& v) {
        if (const Json* j = member(in_, key)) v = bool_of(*j);
    }
    void integer(const char* key, std::optional<std::int64_t>& v) {
        if (const Json* j = member(in_, key)) v = int_of(*j, key);
    }
    void unsigned_integer(const char* key, std::optional<std::uint64_t>& v) {
        if (const Json* j = member(in_, key)) v = uint_of(*j, key);
    }
    void count(const char* key, std::uint32_t& v) {
        if (const Json* j = member(in_, key)) v = static_cast<std::uint32_t>(uint_of(*j, key));
    }
    void overflow(const char* key, std::optional<ir::Overflow>& v) {
        if (const Json* j = member(in_, key)) {
            v = enum_from(str_of(*j, key), kOverflows);
            if (!v) fail("unknown overflow rule " + str_of(*j, key));
        }
    }
    void memory(const char* key, std::optional<ir::MemoryModel>& v) {
        if (const Json* j = member(in_, key)) {
            v = enum_from(str_of(*j, key), kMemory);
            if (!v) fail("unknown memory model " + str_of(*j, key));
        }
    }
    void value(const char* key, ir::LitValue& v) {
        if (const Json* j = member(in_, key)) v = value_from(*j);
    }
    void typeargs(const char* key, std::vector<ir::TypeArg>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail("typeargs are not a list");
            v.clear();
            for (const Json& a : *j) v.push_back(typearg_from(a));
        }
    }
    void target(const char* key, std::optional<ir::CompilerTarget>& v) {
        if (const Json* j = member(in_, key)) v = target_from(*j);
    }
    template <class E> void named(const char* key, E& v, E) {
        if (const Json* j = member(in_, key)) {
            const std::string text = str_of(*j, key);
            const auto e = ir::from_text<E>(text);
            if (!e) fail(std::format("unknown value \"{}\" for \"{}\"", text, key));
            v = *e;
        }
    }
    template <class E> void opt_named(const char* key, std::optional<E>& v) {
        if (const Json* j = member(in_, key)) {
            const std::string text = str_of(*j, key);
            v = ir::from_text<E>(text);
            if (!v) fail(std::format("unknown value \"{}\" for \"{}\"", text, key));
        }
    }
    void syms(const char* key, std::vector<ir::SymP>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail(std::format("\"{}\" is not a list", key));
            v.clear();
            for (const Json& s : *j) v.push_back(sym_from_json(s));
        }
    }
    void ints(const char* key, std::vector<std::int64_t>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail(std::format("\"{}\" is not a list", key));
            v.clear();
            for (const Json& x : *j) v.push_back(int_of(x, key));
        }
    }
    void ranges(const char* key, std::vector<std::pair<std::int64_t, std::int64_t>>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail(std::format("\"{}\" is not a list", key));
            v.clear();
            for (const Json& x : *j) {
                if (!x.is_array() || x.size() != 2) fail(std::format("a \"{}\" entry is [low, high]", key));
                v.emplace_back(int_of(x[0], key), int_of(x[1], key));
            }
        }
    }
    void argmeta(const char* key, std::vector<ir::ArgMeta>& v) {
        if (const Json* j = member(in_, key)) {
            if (!j->is_array()) fail(std::format("\"{}\" is not a list", key));
            v.clear();
            for (const Json& x : *j) {
                ir::ArgMeta m;
                if (const Json* n = member(x, "name")) m.name = str_of(*n, "argument name");
                if (const Json* f = member(x, "flags")) m.flags = static_cast<std::uint32_t>(uint_of(*f, "argument flags"));
                if (const Json* t = member(x, "type")) m.type = type_from_json(*t);
                v.push_back(std::move(m));
            }
        }
    }

private:
    // A node slot: a node; a list of statements where a node is wanted is made one block.
    Node* slot(const Json& j) {
        if (j.is_array()) {
            ir::Block block;
            block.stmts = list(j);
            return ir::make(arena_, std::move(block));
        }
        return node_from(arena_, j);
    }
    std::vector<Node*> list(const Json& j) {
        if (!j.is_array()) {
            std::vector<Node*> one;
            one.push_back(node_from(arena_, j));
            return one;
        }
        std::vector<Node*> out;
        out.reserve(j.size());
        for (const Json& x : j) out.push_back(node_from(arena_, x));
        return out;
    }

    Arena& arena_;
    const Json& in_;
};

// -- the slots of every kind, in document order. (A kind with no slots has no description.)
template <class V> void describe(ir::Module& k, V& v) { v.children("units", k.units); v.text("header", k.header); v.texts("scope_refs", k.scope_refs); }
template <class V> void describe(ir::Unit& k, V& v) {
    v.text("path", k.path); v.text("package", k.package); v.texts("imports", k.imports); v.children("types", k.types);
    v.text("lang", k.lang); v.target("target", k.target);
}
template <class V> void describe(ir::ClassDecl& k, V& v) {
    v.text("name", k.name); v.text("qname", k.qname); v.text("kind", k.kind); v.texts("mods", k.mods);
    v.texts("typeparams", k.typeparams); v.type("extends", k.extends); v.types("implements", k.implements);
    v.children("members", k.members); v.text("outer", k.outer); v.nodes("tparams", k.tparams); v.type("layout", k.layout);
}
template <class V> void describe(ir::FieldDecl& k, V& v) { v.sym("sym", k.sym); v.type("type", k.type); v.child("init", k.init); v.texts("mods", k.mods); }
template <class V> void describe(ir::MethodDecl& k, V& v) {
    v.sym("sym", k.sym); v.text("name", k.name); v.text("kind", k.kind); v.children("params", k.params); v.type("ret", k.ret);
    v.suite("body", k.body); v.texts("mods", k.mods); v.types("throws", k.throws); v.texts("typeparams", k.typeparams);
    v.texts("comments", k.comments); v.text("text", k.text); v.nodes("tparams", k.tparams);
    v.syms("locals", k.locals); v.syms("captures", k.captures); v.nodes("nested", k.nested); v.text("scope", k.scope);
}
template <class V> void describe(ir::Param& k, V& v) { v.sym("sym", k.sym); v.type("type", k.type); v.texts("mods", k.mods); }
template <class V> void describe(ir::EnumConst& k, V& v) {
    v.sym("sym", k.sym); v.children("args", k.args); v.child("body", k.body); v.sym("ctor", k.ctor); v.integer("value", k.value);
}
template <class V> void describe(ir::Property& k, V& v) {
    v.sym("sym", k.sym); v.type("type", k.type); v.texts("mods", k.mods); v.children("params", k.params);
    v.children("accessors", k.accessors); v.child("init", k.init);
}
template <class V> void describe(ir::TypeParam& k, V& v) { v.text("name", k.name); v.types("bounds", k.bounds); v.texts("flags", k.flags); v.text("variance", k.variance); }
template <class V> void describe(ir::Block& k, V& v) { v.children("stmts", k.stmts); }
template <class V> void describe(ir::Local& k, V& v) { v.sym("sym", k.sym); v.type("type", k.type); v.child("init", k.init); v.texts("mods", k.mods); }
template <class V> void describe(ir::ExprStmt& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::If& k, V& v) { v.child("cond", k.cond); v.suite("then", k.then); v.suite("orelse", k.orelse); }
template <class V> void describe(ir::While& k, V& v) { v.child("cond", k.cond); v.suite("body", k.body); }
template <class V> void describe(ir::DoWhile& k, V& v) { v.suite("body", k.body); v.child("cond", k.cond); }
template <class V> void describe(ir::For& k, V& v) { v.children("init", k.init); v.child("cond", k.cond); v.children("update", k.update); v.suite("body", k.body); }
template <class V> void describe(ir::ForEach& k, V& v) { v.child("var", k.var); v.child("iterable", k.iterable); v.child("body", k.body); }
template <class V> void describe(ir::Switch& k, V& v) { v.child("expr", k.expr); v.children("cases", k.cases); v.flag("lifted", k.lifted); v.type("of", k.of); }
template <class V> void describe(ir::Case& k, V& v) { v.children("labels", k.labels); v.flag("default", k.is_default); v.children("body", k.body); v.ranges("ranges", k.ranges); v.flag("null_label", k.null_label); }
template <class V> void describe(ir::Break& k, V& v) { v.text("label", k.label); v.child("value", k.value); }
template <class V> void describe(ir::Continue& k, V& v) { v.text("label", k.label); }
template <class V> void describe(ir::Return& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::Throw& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::Try& k, V& v) { v.children("resources", k.resources); v.child("body", k.body); v.children("catches", k.catches); v.child("final", k.finally_body); v.child("fault", k.fault_body); }
template <class V> void describe(ir::Catch& k, V& v) { v.sym("sym", k.sym); v.types("types", k.types); v.child("body", k.body); v.child("filter", k.filter); }
template <class V> void describe(ir::Labeled& k, V& v) { v.text("label", k.label); v.suite("body", k.body); }
template <class V> void describe(ir::Goto& k, V& v) { v.text("label", k.label); }
template <class V> void describe(ir::MachineFragment& k, V& v) {
    v.unsigned_integer("length", k.length); v.text("reason", k.reason); v.text("why", k.why); v.text("asm", k.assembly);
    v.text("name", k.name); v.text("syntax", k.syntax); v.text("within", k.within);
}
template <class V> void describe(ir::Sync& k, V& v) { v.child("lock", k.lock); v.child("body", k.body); }
template <class V> void describe(ir::Empty& k, V& v) { v.text("note", k.note); v.named("kind", k.kind, ir::EmptyKind::Normal); }
template <class V> void describe(ir::Comment& k, V& v) { v.text("text", k.text); }
template <class V> void describe(ir::LocalClass& k, V& v) { v.child("decl", k.decl); }
template <class V> void describe(ir::Assert& k, V& v) { v.child("cond", k.cond); v.child("detail", k.detail); }
template <class V> void describe(ir::CtorCall& k, V& v) { v.text("kind", k.kind); v.sym("sym", k.sym); v.children("args", k.args); v.child("target", k.target); }
template <class V> void describe(ir::Yield& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::YieldReturn& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::YieldBreak&, V&) {}
template <class V> void describe(ir::Await& k, V& v) { v.child("x", k.x); v.sym("awaiter", k.awaiter); v.sym("result", k.result); }
template <class V> void describe(ir::Lit& k, V& v) { v.value("value", k.value); v.text("text", k.text); }
template <class V> void describe(ir::Name& k, V& v) { v.sym("sym", k.sym); }
template <class V> void describe(ir::Select& k, V& v) { v.child("obj", k.obj); v.sym("sym", k.sym); }
template <class V> void describe(ir::TypeName&, V&) {}
template <class V> void describe(ir::This& k, V& v) { v.type("qualifier", k.qualifier); }
template <class V> void describe(ir::Super& k, V& v) { v.type("qualifier", k.qualifier); }
template <class V> void describe(ir::Call& k, V& v) { v.child("target", k.target); v.sym("sym", k.sym); v.children("args", k.args); v.typeargs("typeargs", k.typeargs); v.named("form", k.form, ir::CallForm::Default); }
template <class V> void describe(ir::New& k, V& v) { v.sym("sym", k.sym); v.children("args", k.args); v.child("body", k.body); v.child("outer", k.outer); v.memory("memory", k.memory); }
template <class V> void describe(ir::NewArray& k, V& v) { v.type("elem", k.elem); v.children("dims", k.dims); v.children_unless_absent("init", k.init, k.dims.empty()); }
template <class V> void describe(ir::Index& k, V& v) { v.child("arr", k.arr); v.suite("index", k.index); v.sym("sym", k.sym); v.type("elem", k.elem); }
template <class V> void describe(ir::Length& k, V& v) { v.child("arr", k.arr); }
template <class V> void describe(ir::Concat& k, V& v) { v.children("parts", k.parts); }
template <class V> void describe(ir::Assign& k, V& v) {
    v.text("op", k.op); v.child("target", k.target); v.child("value", k.value); v.overflow("overflow", k.overflow);
    v.opt_named("eval", k.eval); v.opt_named("via", k.via); v.maybe("signed", k.is_signed);
    v.named("left_stk", k.left_stk, ir::Stack::Unknown); v.named("right_stk", k.right_stk, ir::Stack::Unknown);
    v.named("lift", k.lift, ir::Lift::None); v.sym("sym", k.sym);
}
template <class V> void describe(ir::Cond& k, V& v) { v.child("cond", k.cond); v.child("then", k.then); v.child("orelse", k.orelse); }
template <class V> void describe(ir::Cast& k, V& v) {
    v.child("x", k.x); v.overflow("overflow", k.overflow); v.flag("implicit", k.implicit);
    v.opt_named("conv", k.conv); v.named("from_stk", k.from_stk, ir::Stack::Unknown); v.maybe("from_signed", k.from_signed);
    v.named("lift", k.lift, ir::Lift::None);
}
template <class V> void describe(ir::InstanceOf& k, V& v) { v.child("x", k.x); v.type("test", k.test); v.child("binding", k.binding); }
template <class V> void describe(ir::ClassLit& k, V& v) { v.type("of", k.of); }
template <class V> void describe(ir::Lambda& k, V& v) {
    v.children("params", k.params); v.child("body", k.body); v.syms("locals", k.locals); v.syms("captures", k.captures);
    v.text("scope", k.scope); v.texts("comments", k.comments); v.sym("sym", k.sym); v.texts("mods", k.mods); v.nodes("nested", k.nested);
    v.type("ret", k.ret);
}
template <class V> void describe(ir::MethodRef& k, V& v) { v.child("target", k.target); v.sym("sym", k.sym); v.text("mode", k.mode); v.type("of", k.of); }
template <class V> void describe(ir::SwitchExpr& k, V& v) { v.child("expr", k.expr); v.children("cases", k.cases); v.flag("lifted", k.lifted); v.type("of", k.of); }
template <class V> void describe(ir::AddressOf& k, V& v) { v.sym("sym", k.sym); }
template <class V> void describe(ir::FarAddress& k, V& v) { v.child("selector", k.selector); v.child("offset", k.offset); }
template <class V> void describe(ir::Piece& k, V& v) { v.child("x", k.x); v.count("byte", k.byte); v.count("width", k.width); }
template <class V> void describe(ir::Join& k, V& v) { v.child("hi", k.hi); v.child("lo", k.lo); }
template <class V> void describe(ir::Paren& k, V& v) { v.child("x", k.x); }
template <class V> void describe(ir::Unsupported& k, V& v) { v.text("kind", k.kind); v.text("raw", k.raw); }

// Unary and Binary carry an operator that is not optional (written always, read when present).
void describe_op(const char* key, std::string& op, Writer& v) { v.text(key, std::optional<std::string>(op)); }
void describe_op(const char* key, std::string& op, Reader& v) {
    std::optional<std::string> got;
    v.text(key, got);
    if (got) op = *got;
}
template <class V> void describe(ir::Unary& k, V& v) {
    describe_op("op", k.op, v); v.child("x", k.x); v.flag("postfix", k.postfix); v.overflow("overflow", k.overflow);
    v.named("left_stk", k.left_stk, ir::Stack::Unknown); v.named("lift", k.lift, ir::Lift::None);
}
template <class V> void describe(ir::Binary& k, V& v) {
    describe_op("op", k.op, v); v.child("left", k.left); v.child("right", k.right); v.overflow("overflow", k.overflow);
    v.maybe("signed", k.is_signed);
    v.named("left_stk", k.left_stk, ir::Stack::Unknown); v.named("right_stk", k.right_stk, ir::Stack::Unknown);
    v.named("lift", k.lift, ir::Lift::None); v.opt_named("coalesce", k.coalesce); v.sym("sym", k.sym);
}

// -- the kinds of a lowered bytecode or binary
template <class V> void describe(ir::Region& k, V& v) { v.named("kind", k.kind, ir::RegionKind::Normal); v.children("blocks", k.blocks); v.text("entry", k.entry); }
template <class V> void describe(ir::BlockExpr& k, V& v) { v.text("role", k.role); v.children("stmts", k.stmts); v.child("result", k.result); }
template <class V> void describe(ir::Fixed& k, V& v) { v.child("var", k.var); v.child("body", k.body); }
template <class V> void describe(ir::ThrowExpr& k, V& v) { v.child("x", k.x); }
template <class V> void describe(ir::OptionalChain& k, V& v) { v.child("x", k.x); }
template <class V> void describe(ir::OptionalUnwrap& k, V& v) { v.child("x", k.x); v.flag("ref_in", k.ref_in); }
template <class V> void describe(ir::PatternMatch& k, V& v) {
    v.sym("var", k.var); v.child("tested", k.tested); v.type("test", k.test); v.flag("check_not_null", k.check_not_null);
    v.sym("deconstruct", k.deconstruct); v.flag("is_tuple", k.is_tuple); v.children("subs", k.subs);
}
template <class V> void describe(ir::Destructure& k, V& v) { v.children("init", k.init); v.child("pattern", k.pattern); v.child("conversions", k.conversions); v.child("assignments", k.assignments); }
template <class V> void describe(ir::Default&, V&) {}
template <class V> void describe(ir::SizeOf& k, V& v) { v.type("of", k.of); }
template <class V> void describe(ir::StackAlloc& k, V& v) { v.type("elem", k.elem); v.child("count", k.count); v.flag("span", k.span); }
template <class V> void describe(ir::Intrinsic& k, V& v) {
    describe_op("name", k.name, v); v.children("args", k.args); v.types("targs", k.targs); v.syms("syms", k.syms);
    v.texts("strs", k.strs); v.ints("imms", k.imms); v.argmeta("argmeta", k.argmeta);
}

// Finds a slot by its document name (or lists the names), as the Writer and Reader walk them: the one description of each kind.
class Finder {
public:
    Finder(std::string_view want, std::vector<std::string_view>* keys) : want_(want), keys_(keys) {}
    SlotPtr found;

    void child(const char* key, Node*& v) { hit(key, v); }
    void children(const char* key, std::vector<Node*>& v) { hit(key, v); }
    void nodes(const char* key, std::vector<Node*>& v) { hit(key, v); }
    void children_unless_absent(const char* key, std::vector<Node*>& v, bool) { hit(key, v); }
    void suite(const char* key, ir::Suite& v) { hit(key, v); }
    void type(const char* key, ir::TypeP& v) { hit(key, v); }
    void types(const char* key, std::vector<ir::TypeP>& v) { hit(key, v); }
    void sym(const char* key, ir::SymP& v) { hit(key, v); }
    void text(const char* key, std::optional<std::string>& v) { hit(key, v); }
    void texts(const char* key, std::vector<std::string>& v) { hit(key, v); }
    void flag(const char* key, bool& v) { hit(key, v); }
    void maybe(const char* key, std::optional<bool>& v) { hit(key, v); }
    void integer(const char* key, std::optional<std::int64_t>& v) { hit(key, v); }
    void unsigned_integer(const char* key, std::optional<std::uint64_t>& v) { hit(key, v); }
    void count(const char* key, std::uint32_t& v) { hit(key, v); }
    void overflow(const char* key, std::optional<ir::Overflow>& v) { hit(key, v); }
    void memory(const char* key, std::optional<ir::MemoryModel>& v) { hit(key, v); }
    void value(const char* key, ir::LitValue& v) { hit(key, v); }
    void typeargs(const char* key, std::vector<ir::TypeArg>& v) { hit(key, v); }
    void target(const char* key, std::optional<ir::CompilerTarget>& v) { hit(key, v); }
    void plain(const char* key, std::string& v) { hit(key, v); }   // (an operator: always there)
    template <class E> void named(const char* key, E& v, E) { hit(key, v); }
    template <class E> void opt_named(const char* key, std::optional<E>& v) { hit(key, v); }
    void syms(const char* key, std::vector<ir::SymP>& v) { hit(key, v); }
    void ints(const char* key, std::vector<std::int64_t>& v) { hit(key, v); }
    void ranges(const char* key, std::vector<std::pair<std::int64_t, std::int64_t>>& v) { hit(key, v); }
    void argmeta(const char* key, std::vector<ir::ArgMeta>& v) { hit(key, v); }

private:
    template <class T> void hit(const char* key, T& v) {
        if (keys_ != nullptr) keys_->push_back(key);
        if (std::holds_alternative<std::monostate>(found) && want_ == key) found = &v;
    }
    std::string_view want_;
    std::vector<std::string_view>* keys_;
};
void describe_op(const char* key, std::string& op, Finder& v) { v.plain(key, op); }

Json node_json(const Node& node) {
    Json j = Json::object();
    j["node"] = std::string(ir::kind_name(node));
    if (node.line != 0) j["line"] = node.line;
    if (node.col != 0) j["col"] = node.col;
    if (node.origin) j["origin"] = origin_json(*node.origin);
    if (ir::category(node) == ir::Category::Expression && node.type) j["type"] = type_to_json(node.type);
    Writer w(j);
    ir::visit(const_cast<Node&>(node), [&](auto& k) { describe(k, w); });
    if (node.stack != ir::Stack::Unknown) j["stk"] = std::string(ir::to_text(node.stack));
    if (node.flags != 0) j["fl"] = flags_json(node.flags);
    if (node.info) j["info"] = info_json(node.info, ir::InfoOn::Node, ir::kind_name(node));
    if (const auto* in = ir::get<ir::Intrinsic>(&node)) check_intrinsic(in->name);
    return j;
}

Node* node_from(Arena& arena, const Json& j) {
    if (!j.is_object()) fail("an L2 node is an object");
    const std::string kind = str_of(need(j, "node", "a node"), "node kind");
    Node* node = ir::make_node(arena, kind);
    if (node == nullptr) fail("unknown L2 node kind " + kind);
    if (const Json* v = member(j, "line")) node->line = static_cast<std::uint32_t>(uint_of(*v, "line"));
    if (const Json* v = member(j, "col")) node->col = static_cast<std::uint32_t>(uint_of(*v, "col"));
    if (const Json* v = member(j, "origin")) node->origin = std::make_shared<const ir::Origin>(origin_from(*v));
    if (ir::category(*node) == ir::Category::Expression) {
        if (const Json* v = member(j, "type")) node->type = type_from_json(*v);
    }
    Reader r(arena, j);
    ir::visit(*node, [&](auto& k) { describe(k, r); });
    if (const Json* v = member(j, "stk")) {
        const auto s = ir::from_text<ir::Stack>(str_of(*v, "stk"));
        if (!s) fail("unknown stack kind " + str_of(*v, "stk"));
        node->stack = *s;
    }
    if (const Json* v = member(j, "fl")) node->flags = flags_from(*v);
    if (const Json* v = member(j, "info")) node->info = info_from_json(*v, ir::InfoOn::Node, ir::kind_name(*node));
    if (const auto* in = ir::get<ir::Intrinsic>(node)) check_intrinsic(in->name);
    return node;
}

}  // namespace

Json to_json(const ir::Node& node) {
    DocContext doc;
    DocScope scope(&doc);
    Json out = node_json(node);
    // The variables, and then the entities they and the rest name (a variable's own document may name more of each: the tables grow as they are written).
    if (!doc.vars_out.empty()) {
        Json vars = Json::array();
        for (std::size_t i = 0; i < doc.vars_out.size(); ++i) {
            const ir::SymP s = doc.vars_out[i];
            vars.push_back(sym_body_json(*s));
        }
        out["vars"] = std::move(vars);
    }
    if (!doc.entities_out.empty()) {
        Json entities = Json::array();
        for (const ir::EntityP& e : doc.entities_out) entities.push_back(entity_text_json(*e));
        out["entities"] = std::move(entities);
    }
    return out;
}

SlotPtr slot(ir::Node& node, std::string_view key) {
    if (key == "type" && ir::category(node) == ir::Category::Expression) return &node.type;
    Finder f(key, nullptr);
    ir::visit(node, [&](auto& k) { describe(k, f); });
    return f.found;
}

std::vector<std::string_view> slot_keys(const ir::Node& node) {
    std::vector<std::string_view> keys;
    if (ir::category(node) == ir::Category::Expression) keys.push_back("type");
    Finder f("", &keys);
    ir::visit(const_cast<ir::Node&>(node), [&](auto& k) { describe(k, f); });
    return keys;
}

namespace {

// The root's tables: entities (made by the scope), then variables (made empty first, so one may name another).
void read_tables(const Json& root, DocContext& doc) {
    if (const Json* table = member(root, "entities")) {
        if (!table->is_array()) fail("\"entities\" is not a list");
        if (!table->empty() && doc.scope == nullptr) fail("the document names entities: it is read with the scope that resolves them (from_json with an EntityScope)");
        for (const Json& e : *table) {
            const std::string family = str_of(need(e, "family", "an entity"), "entity family");
            const std::string kind = str_of(need(e, "kind", "an entity"), "entity kind");
            const std::string key = str_of(need(e, "key", "an entity"), "entity key");
            if (family != doc.scope->family()) fail(std::format("entity \"{}\" is of family \"{}\", the scope's is \"{}\"", key, family, doc.scope->family()));
            ir::EntityP resolved = doc.scope->resolve(key);
            if (!resolved) fail(std::format("the scope cannot resolve entity \"{}\"", key));
            if (ir::to_text(resolved->kind()) != kind || resolved->key() != key) fail(std::format("entity \"{}\" resolves to another entity ({} {})", key, ir::to_text(resolved->kind()), resolved->key()));
            doc.entities_in.push_back(std::move(resolved));
        }
    }
    if (const Json* table = member(root, "vars")) {
        if (!table->is_array()) fail("\"vars\" is not a list");
        for (std::size_t i = 0; i < table->size(); ++i) doc.vars_in.push_back(std::make_shared<ir::Sym>());
        for (std::size_t i = 0; i < table->size(); ++i) fill_sym(*doc.vars_in[i], (*table)[i]);
    }
}

}  // namespace

ir::Node* from_json(Arena& arena, const Json& doc, std::shared_ptr<const ir::EntityScope> scope) {
    DocContext ctx;
    ctx.scope = scope.get();
    DocScope guard(&ctx);
    read_tables(doc, ctx);
    ir::Node* root = node_from(arena, doc);
    if (scope != nullptr) {
        if (auto* m = ir::get<ir::Module>(root)) m->scope = std::move(scope);
    }
    return root;
}

ir::Node* from_json(Arena& arena, const Json& doc) { return from_json(arena, doc, nullptr); }

// ---- the module model -------------------------------------------------------------------------------------------------

namespace {

Node* with_origin(Node* node, const std::optional<ir::Origin>& origin) {
    if (origin) node->origin = std::make_shared<const ir::Origin>(*origin);
    return node;
}

}  // namespace

ir::Node* module_node(Arena& arena, std::span<ir::Node* const> units, std::optional<std::string> header) {
    ir::Module m;
    m.units.assign(units.begin(), units.end());
    m.header = std::move(header);
    return ir::make(arena, std::move(m));
}

ir::Node* function_node(Arena& arena, const FunctionSpec& spec) {
    ir::MethodDecl m;
    m.name = spec.name;
    m.kind = "function";
    m.comments = spec.comments;
    m.text = spec.text;
    return with_origin(ir::make(arena, std::move(m)), spec.origin);
}

ir::Node* fragment_node(Arena& arena, ir::MachineFragment fragment, std::optional<ir::Origin> origin) {
    return with_origin(ir::make(arena, std::move(fragment)), origin);
}

ir::Node* comment_node(Arena& arena, std::string text) {
    ir::Comment c;
    c.text = std::move(text);
    return ir::make(arena, std::move(c));
}

std::vector<ir::Node*> units(const ir::Node* module) {
    const auto* m = ir::get<ir::Module>(module);
    if (m == nullptr) return {};
    return m->units;
}

OrderedMap<std::string, std::string> texts(const ir::Node* module) {
    OrderedMap<std::string, std::string> out;
    for (const ir::Node* u : units(module)) {
        const auto* f = ir::get<ir::MethodDecl>(u);
        if (f != nullptr && f->text && f->name) out.insert_or_assign(*f->name, *f->text);
    }
    return out;
}

Made Made::of(std::string name, std::string header, const OrderedMap<std::string, std::string>& texts,
              std::span<const std::string> order, const OrderedMap<std::uint64_t, std::string>& by_va) {
    Made m(std::move(name), std::move(header));
    std::unordered_map<std::string, std::uint64_t> place;
    for (const auto& [va, fn] : by_va) place[fn] = va;
    std::vector<std::string> names;
    if (order.empty()) {
        for (const auto& kv : texts) names.push_back(kv.first);
    } else {
        names.assign(order.begin(), order.end());
        std::unordered_map<std::string, bool> listed;
        for (const std::string& n : order) listed[n] = true;
        for (const auto& kv : texts) {
            if (!listed.contains(kv.first)) names.push_back(kv.first);
        }
    }
    for (const std::string& n : names) {
        MadeFunction f;
        f.name = n;
        if (const std::string* t = texts.find(n)) f.text = *t;
        if (const auto p = place.find(n); p != place.end()) f.va = p->second;
        m.functions_.push_back(std::move(f));
    }
    return m;
}

void Made::add(std::string name, std::optional<std::string> text, std::optional<std::uint64_t> va) {
    functions_.push_back({std::move(name), std::move(text), va});
}

OrderedMap<std::string, std::string> Made::texts() const {
    OrderedMap<std::string, std::string> out;
    for (const MadeFunction& f : functions_) {
        if (f.text) out.insert_or_assign(f.name, *f.text);
    }
    return out;
}

void Made::update_texts(const OrderedMap<std::string, std::string>& new_texts) {
    std::unordered_map<std::string, bool> have;
    for (MadeFunction& f : functions_) {
        have[f.name] = true;
        if (const std::string* t = new_texts.find(f.name)) f.text = *t;
    }
    for (const auto& [name, text] : new_texts) {
        if (!have.contains(name)) functions_.push_back({name, text, std::nullopt});
    }
}

void Made::set_text(const std::string& name, std::string text) {
    OrderedMap<std::string, std::string> one;
    one.insert_or_assign(name, std::move(text));
    update_texts(one);
}

std::vector<std::string> Made::order() const {
    std::vector<std::string> out;
    for (const MadeFunction& f : functions_) out.push_back(f.name);
    return out;
}

OrderedMap<std::uint64_t, std::string> Made::by_va() const {
    OrderedMap<std::uint64_t, std::string> out;
    for (const MadeFunction& f : functions_) {
        if (f.va) out.insert_or_assign(*f.va, f.name);
    }
    return out;
}

OrderedMap<std::uint64_t, std::pair<std::string, std::optional<std::string>>> Made::functions() const {
    OrderedMap<std::uint64_t, std::pair<std::string, std::optional<std::string>>> out;
    for (const MadeFunction& f : functions_) {
        if (f.va) out.insert_or_assign(*f.va, std::make_pair(f.name, f.text));
    }
    return out;
}

ir::Node* Made::document(Arena& arena) const {
    std::vector<ir::Node*> units;
    for (const MadeFunction& f : functions_) {
        FunctionSpec spec{f.name, f.text, std::nullopt, {}};
        if (f.va) spec.origin = ir::OriginVa{name_, *f.va};
        units.push_back(function_node(arena, spec));
    }
    return module_node(arena, units, header_);
}

Assembly::Assembly(std::vector<std::string> lines, std::optional<std::string> syntax, std::optional<std::string> module,
                   std::string name, std::string reason)
    : lines_(std::move(lines)), syntax_(std::move(syntax)), module_(std::move(module)) {
    parts_.push_back({0, std::move(name), std::move(reason), std::nullopt, std::nullopt});
}

void Assembly::part(std::string name, std::string reason, std::optional<ir::Origin> origin, std::optional<std::uint64_t> length) {
    parts_.push_back({lines_.size(), std::move(name), std::move(reason), std::move(origin), length});
}

ir::Node* Assembly::document(Arena& arena) const {
    std::vector<ir::Node*> out;
    for (std::size_t k = 0; k < parts_.size(); ++k) {
        const Part& p = parts_[k];
        const std::size_t end = k + 1 < parts_.size() ? parts_[k + 1].start : lines_.size();
        if (end <= p.start) continue;   // (a part with no lines is left out)
        std::string text;
        for (std::size_t i = p.start; i < end; ++i) {
            if (i > p.start) text += '\n';
            text += lines_[i];
        }
        ir::MachineFragment f;
        f.assembly = std::move(text);
        f.name = p.name;
        f.syntax = syntax_;
        f.reason = p.reason;
        f.length = p.length;
        out.push_back(fragment_node(arena, std::move(f), p.origin));
    }
    return module_node(arena, out);
}

}  // namespace cb::translate::l2
