// cb/translate/l2.cpp -- L2 documents over the typed tree, and the module model (see l2.hpp).
#include "cb/translate/l2.hpp"

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
    const std::string module = member(j, "module") ? str_of(*member(j, "module"), "origin module") : std::string();
    if (const Json* va = member(j, "va")) return ir::OriginVa{module, uint_of(*va, "origin va")};
    if (const Json* seg = member(j, "segment")) {
        return ir::OriginSegment{module, int_of(*seg, "origin segment"), member(j, "offset") ? int_of(*member(j, "offset"), "origin offset") : 0};
    }
    fail("an origin has {module, va}, {module, segment, offset} or {at}");
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
    return std::visit(
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
            } else if constexpr (std::is_same_v<T, ir::FnPtrType>) {
                j["kind"] = "fnptr";
                j["space"] = std::string(ir::to_text(t.space));
                j["fn"] = type_to_json(t.fn);
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

ir::TypeP type_from_json(const Json& d) {
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
    if (kind == "typevar") return ir::typevar_type(str_of(need(d, "name", "a type variable"), "type variable name"));
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

// ---- symbols ----------------------------------------------------------------------------------------------------------

Json sym_to_json(const ir::SymP& s) {
    if (!s) return Json(nullptr);
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
    return j;
}

ir::SymP sym_from_json(const Json& d) {
    if (d.is_null()) return {};
    if (!d.is_object()) fail("an L2 symbol is an object");
    const std::string kind = str_of(need(d, "kind", "a symbol"), "symbol kind");
    const auto k = enum_from(kind, kSymKinds);
    if (!k) fail("unknown symbol kind " + kind);
    ir::SymP s = ir::make_sym(*k, str_of(need(d, "name", "a symbol"), "symbol name"));
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
template <class V> void describe(ir::Module& k, V& v) { v.children("units", k.units); v.text("header", k.header); }
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
template <class V> void describe(ir::Switch& k, V& v) { v.child("expr", k.expr); v.children("cases", k.cases); }
template <class V> void describe(ir::Case& k, V& v) { v.children("labels", k.labels); v.flag("default", k.is_default); v.children("body", k.body); }
template <class V> void describe(ir::Break& k, V& v) { v.text("label", k.label); }
template <class V> void describe(ir::Continue& k, V& v) { v.text("label", k.label); }
template <class V> void describe(ir::Return& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::Throw& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::Try& k, V& v) { v.children("resources", k.resources); v.child("body", k.body); v.children("catches", k.catches); v.child("final", k.finally_body); }
template <class V> void describe(ir::Catch& k, V& v) { v.sym("sym", k.sym); v.types("types", k.types); v.child("body", k.body); }
template <class V> void describe(ir::Labeled& k, V& v) { v.text("label", k.label); v.suite("body", k.body); }
template <class V> void describe(ir::Goto& k, V& v) { v.text("label", k.label); }
template <class V> void describe(ir::MachineFragment& k, V& v) {
    v.unsigned_integer("length", k.length); v.text("reason", k.reason); v.text("why", k.why); v.text("asm", k.assembly);
    v.text("name", k.name); v.text("syntax", k.syntax); v.text("within", k.within);
}
template <class V> void describe(ir::Sync& k, V& v) { v.child("lock", k.lock); v.child("body", k.body); }
template <class V> void describe(ir::Empty&, V&) {}
template <class V> void describe(ir::Comment& k, V& v) { v.text("text", k.text); }
template <class V> void describe(ir::LocalClass& k, V& v) { v.child("decl", k.decl); }
template <class V> void describe(ir::Assert& k, V& v) { v.child("cond", k.cond); v.child("detail", k.detail); }
template <class V> void describe(ir::CtorCall& k, V& v) { v.text("kind", k.kind); v.sym("sym", k.sym); v.children("args", k.args); v.child("target", k.target); }
template <class V> void describe(ir::Yield& k, V& v) { v.child("expr", k.expr); }
template <class V> void describe(ir::Lit& k, V& v) { v.value("value", k.value); v.text("text", k.text); }
template <class V> void describe(ir::Name& k, V& v) { v.sym("sym", k.sym); }
template <class V> void describe(ir::Select& k, V& v) { v.child("obj", k.obj); v.sym("sym", k.sym); }
template <class V> void describe(ir::TypeName&, V&) {}
template <class V> void describe(ir::This& k, V& v) { v.type("qualifier", k.qualifier); }
template <class V> void describe(ir::Super& k, V& v) { v.type("qualifier", k.qualifier); }
template <class V> void describe(ir::Call& k, V& v) { v.child("target", k.target); v.sym("sym", k.sym); v.children("args", k.args); v.typeargs("typeargs", k.typeargs); }
template <class V> void describe(ir::New& k, V& v) { v.sym("sym", k.sym); v.children("args", k.args); v.child("body", k.body); v.child("outer", k.outer); v.memory("memory", k.memory); }
template <class V> void describe(ir::NewArray& k, V& v) { v.type("elem", k.elem); v.children("dims", k.dims); v.children("init", k.init); }
template <class V> void describe(ir::Index& k, V& v) { v.child("arr", k.arr); v.suite("index", k.index); v.sym("sym", k.sym); }
template <class V> void describe(ir::Length& k, V& v) { v.child("arr", k.arr); }
template <class V> void describe(ir::Concat& k, V& v) { v.children("parts", k.parts); }
template <class V> void describe(ir::Assign& k, V& v) { v.text("op", k.op); v.child("target", k.target); v.child("value", k.value); v.overflow("overflow", k.overflow); }
template <class V> void describe(ir::Cond& k, V& v) { v.child("cond", k.cond); v.child("then", k.then); v.child("orelse", k.orelse); }
template <class V> void describe(ir::Cast& k, V& v) { v.child("x", k.x); v.overflow("overflow", k.overflow); v.flag("implicit", k.implicit); }
template <class V> void describe(ir::InstanceOf& k, V& v) { v.child("x", k.x); v.type("test", k.test); v.child("binding", k.binding); }
template <class V> void describe(ir::ClassLit& k, V& v) { v.type("of", k.of); }
template <class V> void describe(ir::Lambda& k, V& v) { v.children("params", k.params); v.child("body", k.body); }
template <class V> void describe(ir::MethodRef& k, V& v) { v.child("target", k.target); v.sym("sym", k.sym); v.text("mode", k.mode); }
template <class V> void describe(ir::SwitchExpr& k, V& v) { v.child("expr", k.expr); v.children("cases", k.cases); }
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
}
template <class V> void describe(ir::Binary& k, V& v) {
    describe_op("op", k.op, v); v.child("left", k.left); v.child("right", k.right); v.overflow("overflow", k.overflow);
    v.maybe("signed", k.is_signed);
}

Json node_json(const Node& node) {
    Json j = Json::object();
    j["node"] = std::string(ir::kind_name(node));
    if (node.line != 0) j["line"] = node.line;
    if (node.col != 0) j["col"] = node.col;
    if (node.origin) j["origin"] = origin_json(*node.origin);
    if (ir::category(node) == ir::Category::Expression && node.type) j["type"] = type_to_json(node.type);
    Writer w(j);
    ir::visit(const_cast<Node&>(node), [&](auto& k) { describe(k, w); });
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
    return node;
}

}  // namespace

Json to_json(const ir::Node& node) { return node_json(node); }

ir::Node* from_json(Arena& arena, const Json& doc) { return node_from(arena, doc); }

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
