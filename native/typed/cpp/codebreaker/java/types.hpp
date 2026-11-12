// codebreaker/java/types.hpp -- SHIM over cb/java/types.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// The old resolver is a shared, mutable record that the pipeline fills in after construction (`markers`,
// `own_catalog`) and the emitters read field by field (`typed_enums`, `values`, `known` ...), so the shim keeps the
// data members as they were. What the resolver DECIDES goes through the new resolver, which is built from the
// classification sets on first use and rebuilt if one of them grows (the pipeline only ever adds to them).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/types.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/typemap.hpp"
#include "py.hpp"

namespace codebreaker::java::types {

namespace cbt = cb::java::types;

using JTypeRef = py::ref<ir::JType>;

// Whole-program no-GC caller-frees facts the source emitter consults:
//   return_methods  -- method names that return a freshly-built OWNED container (Pattern A cleanup)
//   scalar_methods  -- {name: cpp_elem_type} of methods that return a fresh heap SCALAR (Pattern B)
//   retained_params -- {name: {arg positions}} the method retains -> a temp there must NOT be freed
//   corpus_methods  -- every method name in the corpus (so retention of unknown callees stays unsafe)
struct OwnershipCatalog {
    py::set<py::str> return_methods;
    py::dict<py::str, py::str> scalar_methods;
    py::dict<py::str, py::set<int64_t>> retained_params;
    py::set<py::str> corpus_methods;

    static OwnershipCatalog empty() {
        return OwnershipCatalog{py::set<py::str>(), py::dict<py::str, py::str>(), py::dict<py::str, py::set<int64_t>>(),
                                py::set<py::str>()};
    }
};

// One class's entry of the member environment (inheritance already flattened in).
struct MemberEnv {
    py::dict<py::str, JTypeRef> fields;
    py::dict<py::str, JTypeRef> methods;
    std::optional<py::str> parent;
};

class TypeResolver {
  public:
    py::set<py::str> known;
    py::set<py::str> values;
    py::set<py::str> interfaces;
    py::set<py::str> enums;
    // TYPED enums -> emitted as real pointer-classes; still in `enums`
    py::set<py::str> typed_enums;
    py::set<py::str> thread_like;
    // dropped MCP EmptyN marker types -> strip `(EmptyN*)NULL` bridge args at call sites.
    py::set<py::str> markers;
    py::dict<py::str, MemberEnv> members;
    py::dict<py::str, py::set<py::str>> method_renames;
    OwnershipCatalog own_catalog;

    TypeResolver(const std::optional<py::set<py::str>>& known_classes = std::nullopt,
                 const std::optional<py::set<py::str>>& value_types = std::nullopt,
                 const std::optional<py::set<py::str>>& interface_types = std::nullopt,
                 const std::optional<py::set<py::str>>& enum_types = std::nullopt,
                 const std::optional<py::dict<py::str, MemberEnv>>& members_ = std::nullopt,
                 const std::optional<py::dict<py::str, py::set<py::str>>>& method_renames_ = std::nullopt,
                 const std::optional<py::set<py::str>>& typed_enum_types = std::nullopt,
                 const std::optional<py::set<py::str>>& thread_like_ = std::nullopt)
        : known(copy_of_(known_classes)),
          values(value_types ? py::set<py::str>::from(*value_types) : py::set<py::str>::from(typemap::DEFAULT_VALUE_TYPES)),
          interfaces(copy_of_(interface_types)),
          enums(copy_of_(enum_types)),
          typed_enums(copy_of_(typed_enum_types)),
          thread_like(copy_of_(thread_like_)),
          members(members_ && !members_->empty() ? *members_ : py::dict<py::str, MemberEnv>()),
          method_renames(method_renames_ && !method_renames_->empty() ? *method_renames_
                                                                      : py::dict<py::str, py::set<py::str>>()),
          own_catalog(OwnershipCatalog::empty()) {}

    py::str renamed_method(const py::str& class_name, const py::str& method_name) const {
        return cb::compat::to_py(impl_().renamed_method(cb::compat::from_py(class_name), cb::compat::from_py(method_name)));
    }

    // -- classification ------------------------------------------------------
    bool is_primitive(const JTypeRef& jtype) const { return impl_().is_primitive(ir::to_new(*jtype)); }
    py::str ownership(const JTypeRef& jtype) const {
        return cb::compat::to_py(cbt::to_string(impl_().ownership(ir::to_new(*jtype))));
    }
    bool is_pointer(const JTypeRef& jtype) const { return impl_().is_pointer(ir::to_new(*jtype)); }
    bool is_value(const JTypeRef& jtype) const { return impl_().is_value(ir::to_new(*jtype)); }

    // -- member environment (for chained '.' vs '->' inference) ---------------
    JTypeRef member_field_type(const py::str& class_name, const py::str& field_name) const {
        const auto* t = impl_().member_field_type(cb::compat::from_py(class_name), cb::compat::from_py(field_name));
        return t ? ir::from_new(*t) : nullptr;
    }
    JTypeRef member_method_type(const py::str& class_name, const py::str& method_name) const {
        const auto* t = impl_().member_method_type(cb::compat::from_py(class_name), cb::compat::from_py(method_name));
        return t ? ir::from_new(*t) : nullptr;
    }
    bool extends_value_container(const py::str& class_name) const {
        return impl_().extends_value_container(cb::compat::from_py(class_name));
    }
    std::optional<py::str> port_note(const JTypeRef& jtype) const {
        const auto note = impl_().port_note(ir::to_new(*jtype));
        if (!note) return std::nullopt;
        return cb::compat::to_py(*note);
    }

    // -- C++ spelling --------------------------------------------------------
    py::str base_cpp(const py::str& name) const { return cb::compat::to_py(impl_().base_cpp(cb::compat::from_py(name))); }
    // context: 'field' | 'param' | 'return' | 'local' | 'element'
    py::str cpp_type(const JTypeRef& jtype, const py::str& context = py::str("field")) const {
        return cb::compat::to_py(impl_().cpp_type(ir::to_new(*jtype), use_(context)));
    }
    py::str field_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("field")); }
    py::str param_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("param")); }
    py::str return_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("return")); }

  private:
    static py::set<py::str> copy_of_(const std::optional<py::set<py::str>>& x) {
        return x ? py::set<py::str>::from(*x) : py::set<py::str>();
    }
    static cbt::Use use_(const py::str& context) {
        const std::string c = cb::compat::from_py(context);
        if (c == "param") return cbt::Use::Param;
        if (c == "return") return cbt::Use::Return;
        if (c == "local") return cbt::Use::Local;
        if (c == "element") return cbt::Use::Element;
        return cbt::Use::Field;
    }
    static cb::java::typemap::NameSet names_(const py::set<py::str>& s) {
        cb::java::typemap::NameSet out;
        for (const py::str& n : s) out.insert(cb::compat::from_py(n));
        return out;
    }

    // The new resolver, built on first use from the sets the old code fills in, and again when one of them has
    // grown (they are only ever added to).
    const cbt::TypeResolver& impl_() const {
        const std::int64_t stamp = known.size() * 3 + values.size() * 5 + interfaces.size() * 7 + enums.size() * 11 +
                                   typed_enums.size() * 13 + members.size() * 17 + method_renames.size() * 19;
        if (!impl_cache_ || stamp != stamp_) {
            cbt::ResolverData d;
            d.known = names_(known);
            d.values = names_(values);
            d.interfaces = names_(interfaces);
            d.enums = names_(enums);
            d.typed_enums = names_(typed_enums);
            for (const auto& kv : method_renames.items()) {
                d.method_renames[cb::compat::from_py(std::get<0>(kv))] = names_(std::get<1>(kv));
            }
            for (const auto& kv : members.items()) {
                cbt::MemberEnv& e = d.members[cb::compat::from_py(std::get<0>(kv))];
                const MemberEnv& old = std::get<1>(kv);
                for (const auto& f : old.fields.items()) e.fields.emplace(cb::compat::from_py(std::get<0>(f)), ir::to_new(*std::get<1>(f)));
                for (const auto& m : old.methods.items()) e.methods.emplace(cb::compat::from_py(std::get<0>(m)), ir::to_new(*std::get<1>(m)));
                if (old.parent) e.parent = cb::compat::from_py(*old.parent);
            }
            impl_cache_ = std::make_shared<cbt::TypeResolver>(std::move(d));
            stamp_ = stamp;
        }
        return *impl_cache_;
    }

    mutable std::shared_ptr<cbt::TypeResolver> impl_cache_;
    mutable std::int64_t stamp_ = -1;
};

}  // namespace codebreaker::java::types
