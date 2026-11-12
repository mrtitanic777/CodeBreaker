// cb-unity: alone (file-local helpers named for this module)
// An inference type as an L2 type, or the reason it has none.
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

Context::Context(Program& program, const infer::Library& library, program::IntPolicy int_policy)
    : p_(program), library_(library), int_policy_(int_policy), records_(program) {}

std::string Context::qualified_record(SiteId root) {
    root = p_.find(root);
    return records_.module(root) + "." + records_.name(root);
}

ir::TypeP Context::type_of(const Type& t, std::uint32_t line, std::string_view what) {
    using pt::Kind;
    const std::string what_text(what);
    switch (t.kind) {
        case Kind::Int:
            if (t.fits_i64() || int_policy_ == program::IntPolicy::Checked) return i64_type();
            throw Refused(RefusalClass::IntRange, std::format("{} in [{}, {}]", what, infer::bound_text(t.lo), infer::bound_text(t.hi)), line);
        case Kind::Bool: return bool_type();
        case Kind::Float: return f64_type();
        case Kind::None:
            if (what != "value" && what != "return value") throw Refused(RefusalClass::Untyped, what_text + " is only ever None", line);
            return none_type();
        case Kind::Str: return str_type();
        case Kind::Bytes: return bytes_type();
        case Kind::Bytearray: return py_class("py.bytearray");
        case Kind::Opt: return optional_type(type_of(t.args.at(0), line, what));
        case Kind::Tuple: {
            std::vector<ir::TypeP> items;
            for (const Type& a : t.args) items.push_back(type_of(a, line, what));
            return py_class("py.tuple", std::move(items));
        }
        case Kind::List:
        case Kind::Set:
        case Kind::Dict: break;
        case Kind::Inst: {
            const std::string& name = t.name.value_or("");
            if (name.starts_with(infer::kExcPrefix)) {
                std::string exception = name.substr(infer::kExcPrefix.size());
                if (exception == "struct.error") exception = "StructError";
                return py_class("py." + exception);
            }
            const auto cls = p_.classes.find(name);
            if (cls != nullptr && (*cls)->exception) {
                return ir::make_type(ir::ClassType{.name = name, .exception = true});
            }
            return ir::make_type(ir::ClassType{.name = name, .ref = true});
        }
        case Kind::Extern:
            if (t.name == "file") return py_class("py.file");
            throw Refused(RefusalClass::Conflict, "a extern as a value", line);
        case Kind::Iter:
            // an iterator (range, enumerate, zip, dict.items ...) is consumed where it is made: a loop or a call that takes an iterable; kept in
            // a name, passed or returned it has no C++ type here
            if (what != "value") throw Refused(RefusalClass::Syntax, "an iterator as a " + what_text, line);
            return py_class("py.iter", {type_of(t.args.at(0), line, "value")});
        case Kind::Top: {
            const std::string why = t.why.value_or("");
            const std::size_t colon = why.find(':');
            if (colon != std::string::npos) {
                if (const auto cls = refusal_class_from_word(why.substr(0, colon))) throw Refused(*cls, why.substr(colon + 1), line);
            }
            throw Refused(RefusalClass::Conflict, why, line);
        }
        case Kind::Bottom: throw Refused(RefusalClass::Untyped, what_text, line);
        default: throw Refused(RefusalClass::Conflict, "a " + std::string(pt::kind_word(t.kind)) + " as a value", line);
    }

    // a container: a list, a set, a dict, or a record
    const std::string word(pt::kind_word(t.kind));
    const SiteId root = p_.find(t.site.value_or(0));
    const program::Site& site = p_.site(root);
    if (site.poisoned) throw Refused(RefusalClass::Untyped, "a " + word + " also given items of no single type", line);
    const std::string_view want_kind = t.kind == Kind::List ? "list" : t.kind == Kind::Set ? "set" : "dict";
    const auto kind_ok = [&] { return std::string_view(program::container_word(site.kind)) == want_kind; };
    if (t.kind == Kind::List || t.kind == Kind::Set) {
        if (site.dynamic && !kind_ok()) throw Refused(RefusalClass::Conflict, *site.dynamic, line);
        if (site.elem.kind == Kind::Bottom) throw Refused(RefusalClass::Untyped, "the items of an empty " + word, line);
        return py_class("py." + word, {type_of(site.elem, line, word + " item")});
    }
    if (!kind_ok()) throw Refused(RefusalClass::Conflict, site.dynamic.value_or("not a dict"), line);
    if (p_.is_record(root)) {
        for (const RecordField& field : records_.fields(root)) {
            if (field.type.kind == Kind::Bottom) throw Refused(RefusalClass::Untyped, "record field '" + field.key + "'", line);
            if (!field.required && field.type.kind == Kind::Opt)
                throw Refused(RefusalClass::Conflict, "record field '" + field.key + "' both optional and None", line);
        }
        used_records_.insert(root);
        return ir::make_type(ir::ClassType{.name = qualified_record(root), .record = true, .ref = records_.is_ref(root)});
    }
    if (site.keys.kind == Kind::Bottom || site.elem.kind == Kind::Bottom) throw Refused(RefusalClass::Untyped, "the keys or values of an empty dict", line);
    if (site.elem.kind == Kind::Top) throw Refused(RefusalClass::Conflict, "dict values: " + site.elem.why.value_or(""), line);
    return py_class("py.dict", {type_of(site.keys, line, "dict key"), type_of(site.elem, line, "dict value")});
}

}  // namespace cb::translate::frontends::python::lower
