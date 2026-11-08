// codebreaker/java/ir.cpp -- typed C++ port of codebreaker.java.ir.

#include "codebreaker/java/ir.hpp"

namespace codebreaker::java::ir {

const py::str PUBLIC("public");
const py::str PROTECTED("protected");
const py::str PACKAGE("package");
const py::str PRIVATE("private");

// ---------------------------------------------------------------------------------------------- JType
JType::JType(const py::str& name, int64_t array_dims, const py::list<py::ref<JType>>& args)
    : name(name), array_dims(array_dims), args(args.copy()) {}

bool JType::is_array() const { return array_dims > 0; }

bool JType::is_generic() const { return !args.empty(); }

py::ref<JType> JType::element(const py::ref<JType>& self) const {
    if (!array_dims) {
        return self;
    }
    return std::make_shared<JType>(name, array_dims - 1, args);
}

bool JType::operator==(const JType& other) const {
    if (!(name == other.name && array_dims == other.array_dims && args.size() == other.args.size())) {
        return false;
    }
    for (int64_t i = 0; i < args.size(); ++i) {
        if (!(*args[i] == *other.args[i])) {
            return false;
        }
    }
    return true;
}

bool JType::operator!=(const JType& other) const { return !(*this == other); }

int64_t JType::hash() const {
    uint64_t h = static_cast<uint64_t>(py::str_hash()(name));
    h = h * 1000003u ^ static_cast<uint64_t>(array_dims);
    for (const py::ref<JType>& a : args) {
        h = h * 1000003u ^ static_cast<uint64_t>(a->hash());
    }
    return static_cast<int64_t>(h);
}

py::str JType::repr() const {
    py::str s = name;
    if (!args.empty()) {
        py::list<py::str> parts;
        for (const py::ref<JType>& a : args) {
            parts.append(a->repr());
        }
        s = s + py::str("<") + py::join(py::str(", "), parts) + py::str(">");
    }
    return s + py::repeat(py::str("[]"), array_dims);
}

// ---------------------------------------------------------------------------------------------- Field
Field::Field(const py::str& name, const py::ref<JType>& jtype, const py::str& access, bool is_static, bool is_final,
             const std::optional<py::str>& initializer, int64_t line)
    : name(name), jtype(jtype), access(access), is_static(is_static), is_final(is_final), initializer(initializer),
      line(line) {}

py::str Field::repr() const {
    py::str mods = (is_static ? py::str("static ") : py::str("")) + (is_final ? py::str("final ") : py::str(""));
    return py::percent(py::str("<Field %s%s %s>"), mods, jtype->repr(), name);
}

// ---------------------------------------------------------------------------------------------- Param
Param::Param(const py::str& name, const py::ref<JType>& jtype, bool is_final)
    : name(name), jtype(jtype), is_final(is_final), by_ref(false) {}

py::str Param::repr() const { return py::percent(py::str("<Param %s %s>"), jtype->repr(), name); }

// ---------------------------------------------------------------------------------------------- Method
Method::Method(const py::str& name, const py::ref<JType>& return_type, const py::list<py::ref<Param>>& params,
               const py::str& access, bool is_static, bool is_abstract, bool is_final, bool is_ctor,
               const py::list<std::tuple<int64_t, py::str>>& body, int64_t line)
    : name(name), return_type(return_type), params(params.copy()), access(access), is_static(is_static),
      is_abstract(is_abstract), is_final(is_final), is_ctor(is_ctor), body(body.copy()), line(line) {}

int64_t Method::arity() const { return params.size(); }

py::list<py::ref<JType>> Method::param_types() const {
    py::list<py::ref<JType>> out;
    for (const py::ref<Param>& p : params) {
        out.append(p->jtype);
    }
    return out;
}

std::tuple<py::str, py::list<py::str>> Method::signature() const {
    py::list<py::str> reprs;
    for (const py::ref<Param>& p : params) {
        reprs.append(p->jtype->repr());
    }
    return std::make_tuple(name, reprs);
}

py::str Method::repr() const {
    py::str kind = is_ctor ? py::str("ctor") : (return_type ? return_type->repr() : py::str("void"));
    return py::percent(py::str("<Method %s %s(%d)>"), kind, name, arity());
}

// ---------------------------------------------------------------------------------------------- EnumConst
EnumConst::EnumConst(const py::str& name, const py::list<py::str>& args) : name(name), args(args.copy()) {}

py::str EnumConst::repr() const { return py::percent(py::str("<EnumConst %s(%d)>"), name, args.size()); }

// ---------------------------------------------------------------------------------------------- JClass
JClass::JClass(const py::str& name, const std::optional<py::str>& package, const std::optional<py::str>& parent,
               const py::list<py::str>& interfaces, bool is_interface, bool is_abstract, bool is_enum,
               const std::optional<py::str>& source_path)
    : name(name), package(package), parent(parent), interfaces(interfaces.copy()), is_interface(is_interface),
      is_abstract(is_abstract), is_enum(is_enum), source_path(source_path), has_init_statics(false) {}

py::ref<Field> JClass::field(const py::str& name) const {
    for (const py::ref<Field>& f : fields) {
        if (f->name == name) {
            return f;
        }
    }
    return nullptr;
}

py::list<py::ref<Field>> JClass::instance_fields() const {
    py::list<py::ref<Field>> out;
    for (const py::ref<Field>& f : fields) {
        if (!f->is_static) {
            out.append(f);
        }
    }
    return out;
}

py::list<py::ref<Field>> JClass::static_fields() const {
    py::list<py::ref<Field>> out;
    for (const py::ref<Field>& f : fields) {
        if (f->is_static) {
            out.append(f);
        }
    }
    return out;
}

py::list<py::ref<Method>> JClass::constructors() const {
    py::list<py::ref<Method>> out;
    for (const py::ref<Method>& m : methods) {
        if (m->is_ctor) {
            out.append(m);
        }
    }
    return out;
}

py::ref<Method> JClass::method(const py::str& name) const {
    for (const py::ref<Method>& m : methods) {
        if (m->name == name) {
            return m;
        }
    }
    return nullptr;
}

bool JClass::owns_heap_field() const {
    for (const py::ref<Field>& f : fields) {
        // `f.initializer and 'new ' in f.initializer`: None and '' are both falsy
        if (f->initializer && f->initializer->size() > 0 && f->initializer->contains(py::str("new "))) {
            return true;
        }
    }
    return false;
}

bool JClass::is_typed_enum() const {
    if (!is_enum) {
        return false;
    }
    for (const py::ref<EnumConst>& ec : enum_consts) {
        if (!ec->args.empty()) {
            return true;
        }
    }
    if (!instance_fields().empty()) {
        return true;
    }
    for (const py::ref<Method>& m : methods) {
        if (!m->is_ctor) {
            return true;
        }
    }
    return false;
}

py::str JClass::repr() const {
    py::str kind = is_interface ? py::str("interface") : (is_enum ? py::str("enum") : py::str("class"));
    py::str ext = parent ? py::percent(py::str(" extends %s"), *parent) : py::str("");
    return py::percent(py::str("<JClass %s %s%s f=%d m=%d>"), kind, name, ext, fields.size(), methods.size());
}

}  // namespace codebreaker::java::ir
