// cb-unity: alone (file-local helpers named for this module)
// The calls of a function: of the program's own functions and classes, of the builtins, of methods of the builtin types and of
// the standard library.
#include <algorithm>
#include <format>
#include <map>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

namespace {

bool is_true_constant(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    return c != nullptr && c->value.kind == syntax::ConstValue::Kind::True;
}

bool is_none_literal_node(const syntax::Expr* e) { return pyast::is_none_constant(e); }

}  // namespace

const syntax::keyword* Lowerer::keyword_named(const syntax::Call* n, std::string_view name) {
    for (const syntax::keyword* k : n->keywords)
        if (k->has_arg && k->arg == name) return k;
    return nullptr;
}

std::vector<Node*> Lowerer::lowered_args(const syntax::Call* n) {
    std::vector<Node*> args;
    for (const syntax::Expr* a : n->args) {
        if (const auto* gen = syntax::as<syntax::GeneratorExp>(a)) args.push_back(gen_as_list(gen));
        else args.push_back(expr(a));
    }
    return args;
}

Node* Lowerer::e_call(const syntax::Call* n) {
    using K = infer::ResKind;
    const infer::Res* r = res_at(n);
    const std::uint32_t line = line_of(n);
    if (r == nullptr) refuse(RefusalClass::Dynamic, "call", line);
    const Type& t = type_at(n, line);
    const std::string qualified = r->name.value_or("");
    switch (r->kind) {
        case K::Fn:
        case K::Method:
        case K::Super:
        case K::BaseCall: {
            const Fn& f = *p.fns.at(qualified);
            calls.insert(f.qname);
            const std::vector<Node*> args = call_args(n, f, r->kind != K::Fn || f.kind == program::FnKind::Method);
            const TypeP rt = call_ret(f, n);
            if (r->kind == K::Fn) {
                if (f.cls != nullptr && f.kind == program::FnKind::Static) {
                    const ir::SymP sym = ir::make_sym(ir::SymKind::Method, f.name, f.cls->qname, true);
                    sym->ret = rt;
                    return mk(ir::Call{.target = class_name_node(f.cls->qname), .sym = sym, .args = args}, rt, line);
                }
                const ir::SymP sym = ir::make_sym(ir::SymKind::Function, f.name, f.module->name, true);
                sym->ret = rt;
                return mk(ir::Call{.target = nullptr, .sym = sym, .args = args}, rt, line);
            }
            if (r->kind == K::Super || r->kind == K::BaseCall) {   // a base class's own method
                if (f.name == "__init__")
                    return mk(ir::Call{.target = nullptr, .sym = ir::make_sym(ir::SymKind::Ctor, "__init__", f.cls->qname), .args = args, .typeargs = {std::string("base")}}, none_type(), line);
                const ir::SymP sym = ir::make_sym(ir::SymKind::Method, f.name, f.cls->qname);
                sym->ret = rt;
                return mk(ir::Call{.target = class_name_node(f.cls->qname), .sym = sym, .args = args, .typeargs = {std::string("base")}}, rt, line);
            }
            Node* receiver = syntax::as<syntax::Attribute>(n->func) != nullptr ? expr(syntax::cast<syntax::Attribute>(n->func)->value) : nullptr;
            const ir::SymP sym = ir::make_sym(ir::SymKind::Method, f.name, f.cls->qname);
            sym->ret = rt;
            return mk(ir::Call{.target = receiver, .sym = sym, .args = args}, rt, line);
        }
        case K::Ctor: {
            const Class& c = *p.classes.at(qualified);
            const Fn* init = p.find_method(qualified, "__init__");
            std::vector<Node*> args = init != nullptr ? call_args(n, *init, /*bound=*/true) : std::vector<Node*>{};
            if (init != nullptr) calls.insert(init->qname);
            classes_made.insert(qualified);
            const ir::SymP ctor = ir::make_sym(ir::SymKind::Ctor, c.name, qualified);
            if (c.exception) {   // an exception is made and thrown as a value
                if (init == nullptr) {
                    args.clear();
                    for (const syntax::Expr* a : n->args) args.push_back(expr(a));
                    if (!args.empty() && !(args[0]->type == str_type())) args = {py_call("str", args, str_type())};
                }
                return mk(ir::New{.sym = ctor, .args = std::move(args), .memory = ir::MemoryModel::Stack}, ir::make_type(ir::ClassType{.name = qualified, .exception = true}), line);
            }
            return mk(ir::New{.sym = ctor, .args = std::move(args), .memory = ir::MemoryModel::Gc}, ir::make_type(ir::ClassType{.name = qualified, .ref = true}), line);
        }
        case K::Exc: {
            std::vector<Node*> args;
            for (const syntax::Expr* a : n->args) args.push_back(expr(a));
            if (!args.empty() && !(args[0]->type == str_type())) args = {py_call("str", args, str_type())};
            const std::string name = qualified == "struct.error" ? "StructError" : qualified;
            return mk(ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, name, std::string(kPy)), .args = std::move(args)}, py_class("py." + name), line);
        }
        case K::Builtin: return builtin_call(n, qualified, t);
        case K::BMethod: return method_call(n, qualified, t);
        case K::Std: return std_call(n, *r, t);
        case K::Isinstance:
            for (const syntax::Expr* a : n->args)
                if (effects(expr(a))) refuse(RefusalClass::Syntax, "isinstance of a computed value", line);
            return lit(r->flag, bool_type());
        default: refuse(RefusalClass::Dynamic, "call", line);
    }
}

TypeP Lowerer::call_ret(const Fn& f, const syntax::Call* n) {
    const Type& t = type_at(n, line_of(n));
    if (f.name == "__init__") return none_type();
    return type_or_void(t, n);
}

std::vector<Node*> Lowerer::call_args(const syntax::Call* n, const Fn& f, bool bound) {
    const bool method_like = f.kind == program::FnKind::Method || f.kind == program::FnKind::Property;
    std::vector<std::string> names = bound || !method_like ? f.bound() : f.params;
    if (f.kind == program::FnKind::Static) names = f.params;
    std::map<std::string, const syntax::Expr*> given;
    const infer::Res* r = res_at(n);
    const bool base_call = syntax::as<syntax::Attribute>(n->func) != nullptr && r != nullptr && r->kind == infer::ResKind::BaseCall;
    const std::size_t skip = base_call ? 1 : 0;   // `Base.method(self, ...)`: self is not an argument
    for (std::size_t i = 0; i < names.size() && skip + i < n->args.size(); ++i) given[names[i]] = n->args[skip + i];
    for (const syntax::keyword* k : n->keywords)
        if (k->has_arg) given[k->arg] = k->value;
    std::vector<Node*> out;
    std::vector<std::string> all = names;
    all.insert(all.end(), f.kwonly.begin(), f.kwonly.end());
    for (const std::string& name : all) {
        const TypeP parameter_type = ty(p.param_type(f, name), n, "parameter " + name);
        if (const auto at = given.find(name); at != given.end()) {
            out.push_back(coerce(expr(at->second), parameter_type));
        } else if (const auto* d = f.defaults.find(name); d != nullptr) {
            const auto* constant = syntax::as<syntax::Constant>(*d);
            if (constant == nullptr) refuse(RefusalClass::Syntax, "a computed default of " + name, line_of(n));
            out.push_back(coerce(default_lit(constant), parameter_type));
        } else {
            refuse(RefusalClass::Conflict, "missing argument " + name, line_of(n));
        }
    }
    return out;
}

Node* Lowerer::default_lit(const syntax::Constant* d) {
    using K = syntax::ConstValue::Kind;
    const syntax::ConstValue& v = d->value;
    switch (v.kind) {
        case K::None: return lit_none(none_type());
        case K::True: return lit(true, bool_type());
        case K::False: return lit(false, bool_type());
        case K::Int:
            if (!v.fits_i64) refuse(RefusalClass::IntRange, "literal " + v.text, line_of(d));
            return lit(v.i64, i64_type());
        case K::Str: return lit(v.text, str_type());
        case K::Bytes: return lit(text::from_latin1(as_bytes(v.text)), bytes_type());
        case K::Float: return lit(v.f, f64_type());
        default: refuse(RefusalClass::Syntax, "default", 0);
    }
}

Node* Lowerer::builtin_call(const syntax::Call* n, const std::string& name, const Type& t) {
    const std::uint32_t line = line_of(n);
    const TypeP rt = type_or_void(t, n);
    if (name == "any" || name == "all") return any_all(n, name);
    std::vector<Node*> args = lowered_args(n);
    if (name == "sorted" || name == "list.sort") {
        const syntax::keyword* key = keyword_named(n, "key");
        const syntax::keyword* reverse = keyword_named(n, "reverse");
        std::vector<Node*> extra;
        if (key != nullptr) {
            const auto* lambda = syntax::as<syntax::Lambda>(key->value);
            if (lambda == nullptr) refuse(RefusalClass::Dynamic, "key function", line);
            extra.push_back(lambda_expr(lambda, {class_type_of(rt)->args.at(0)}));
        } else {
            extra.push_back(lit_none(none_type()));
        }
        if (reverse != nullptr) extra.push_back(truthy(expr(reverse->value)));
        args.insert(args.end(), extra.begin(), extra.end());
        return py_call("sorted", std::move(args), rt, nullptr, line);
    }
    if (name == "dict") {
        if (is_record_literal(n)) return record_value(n);
        refuse(RefusalClass::Syntax, "dict() call", line);
    }
    if (name == "min" || name == "max" || name == "min_iter" || name == "max_iter") {
        if (keyword_named(n, "default") != nullptr) refuse(RefusalClass::Syntax, name + "(default=)", line);
        for (Node*& a : args) a = coerce(a, rt);
    }
    if (name == "print") {
        for (const syntax::keyword* k : n->keywords) args.push_back(py_call("kw", {expr(k->value)}, none_type(), nullptr, 0, {k->arg}));
    } else if (!n->keywords.empty() && name != "enumerate") {
        refuse(RefusalClass::Syntax, name + " keywords", line);
    }
    if (name == "enumerate" && !n->keywords.empty()) args.push_back(expr(n->keywords[0]->value));
    static const std::set<std::string, std::less<>> plain = {"sorted", "enumerate", "print", "zip", "reversed"};
    return py_call(plain.contains(name) ? name : "builtins." + name, std::move(args), rt, nullptr, line);
}

Node* Lowerer::method_call(const syntax::Call* n, const std::string& name, const Type& t) {
    const std::uint32_t line = line_of(n);
    const TypeP rt = type_or_void(t, n);
    const auto* attribute = syntax::as<syntax::Attribute>(n->func);
    if (attribute == nullptr) refuse(RefusalClass::Dynamic, "call", line);
    Node* receiver = expr(attribute->value);
    if (name == "record.get") {
        const auto key = pyast::string_constant(n->args.at(0));
        const Type& record = type_at(attribute->value, line);
        const std::vector<RecordField> fields = ctx.records().fields(record.site.value_or(0));
        const auto field = key ? std::find_if(fields.begin(), fields.end(), [&](const RecordField& f) { return f.key == *key; }) : fields.end();
        if (field == fields.end()) return n->args.size() > 1 ? coerce(expr(n->args[1]), rt) : lit_none(rt);   // no such key: the default
        Node* selected = field_ref(receiver, *key, attribute->value, line, /*store=*/false, /*raw=*/true);
        if (field->required) return coerce(selected, rt);
        if (n->args.size() < 2 || is_none_literal_node(n->args[1])) return selected;   // .get(k) of an optional field: the optional itself
        return py_call("value_or", {selected, coerce(expr(n->args[1]), rt)}, rt, nullptr, line);
    }
    if (name == "list.sort") return sort_call(n, receiver);
    std::vector<Node*> args = lowered_args(n);
    for (const syntax::keyword* k : n->keywords) args.push_back(py_call("kw", {expr(k->value)}, none_type(), nullptr, 0, {k->arg}));
    const ir::ClassType* container = class_type_of(receiver->type);
    if ((name == "list.append" || name == "list.insert" || name == "set.add") && container != nullptr && !container->args.empty() && !args.empty())
        args.back() = coerce(args.back(), container->args[0]);
    if (name == "bytes.decode" || name == "str.encode") {
        const auto text_of = [&](const syntax::Expr* e) {
            const auto s = pyast::string_constant(e);
            if (!s) refuse(RefusalClass::Dynamic, "a computed encoding", line);
            return *s;
        };
        std::string encoding = "utf-8", errors = "strict";
        if (!n->args.empty()) encoding = text_of(n->args[0]);
        else if (const syntax::keyword* k = keyword_named(n, "encoding")) encoding = text_of(k->value);
        if (n->args.size() > 1) errors = text_of(n->args[1]);
        else if (const syntax::keyword* k = keyword_named(n, "errors")) errors = text_of(k->value);
        encoding = text::lower(encoding);
        if (encoding == "latin-1" || encoding == "iso-8859-1") encoding = "latin1";
        else if (encoding == "utf8") encoding = "utf-8";
        return py_call(name, {lit(encoding, str_type()), lit(errors, str_type())}, rt, receiver, line);
    }
    return py_call(name, std::move(args), rt, receiver, line);
}

Node* Lowerer::sort_call(const syntax::Call* n, Node* recv) {
    const syntax::keyword* key = keyword_named(n, "key");
    const syntax::keyword* reverse = keyword_named(n, "reverse");
    std::vector<Node*> args;
    const ir::ClassType* container = class_type_of(recv->type);
    if (container == nullptr || container->args.empty()) refuse(RefusalClass::Conflict, "sort of a value that is not a list", line_of(n));
    if (key != nullptr) {
        const auto* lambda = syntax::as<syntax::Lambda>(key->value);
        if (lambda == nullptr) refuse(RefusalClass::Dynamic, "key function", line_of(n));
        args.push_back(lambda_expr(lambda, {container->args[0]}));
    } else {
        args.push_back(lit_none(none_type()));
    }
    if (reverse != nullptr) args.push_back(truthy(expr(reverse->value)));
    return py_call("list.sort", std::move(args), none_type(), recv, line_of(n));
}

Node* Lowerer::std_call(const syntax::Call* n, const infer::Res& r, const Type& t) {
    const std::uint32_t line = line_of(n);
    const TypeP rt = type_or_void(t, n);
    const std::string name = r.name.value_or("");
    std::vector<Node*> args;
    for (const syntax::Expr* a : n->args) args.push_back(expr(a));
    if (name.starts_with("struct.")) return py_call(name, std::move(args), rt, nullptr, line, {r.member});   // the format is a generic argument of the call
    if (name == "int.from_bytes") {
        std::string order = "big";
        if (n->args.size() > 1) {
            const auto s = pyast::string_constant(n->args[1]);
            if (!s) refuse(RefusalClass::Dynamic, "a computed byte order", line);
            order = *s;
        } else if (const syntax::keyword* k = keyword_named(n, "byteorder")) {
            const auto s = pyast::string_constant(k->value);
            if (!s) refuse(RefusalClass::Dynamic, "a computed byte order", line);
            order = *s;
        }
        bool is_signed = false;
        if (const syntax::keyword* k = keyword_named(n, "signed")) {
            const auto* constant = syntax::as<syntax::Constant>(k->value);
            if (constant == nullptr) refuse(RefusalClass::Syntax, "a computed signed flag", line);
            is_signed = is_true_constant(k->value) || (constant->value.kind == syntax::ConstValue::Kind::Int && constant->value.i64 != 0);
        }
        return py_call(name, {args.at(0), lit(order, str_type()), lit(is_signed, bool_type())}, rt, nullptr, line);
    }
    if (!n->keywords.empty()) refuse(RefusalClass::Syntax, name + " keywords", line);
    return py_call(name, std::move(args), rt, nullptr, line);
}

}  // namespace cb::translate::frontends::python::lower
