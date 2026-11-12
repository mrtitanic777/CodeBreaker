// cb/translate/frontends/python/infer_module.cpp -- a module's top level run on types (its names, its classes' attributes,
// its functions' parameter defaults), the standard library attributes with a known type, the library that types nothing,
// and the evaluator `Program::solve` runs.
#include <exception>

#include "cb/translate/frontends/python/infer_util.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

// Types no builtin, builtin-type method or standard library call: each is refused with its name.
class UnmappedLibrary final : public Library {
public:
    TR method(Eval& ev, const syntax::Call* call, const Type& recv, const std::string& attr, const Args&, const Kw&,
              const State&) const override {
        return TR(ev.issue(call, IssueClass::Stdlib, std::string(P::kind_word(recv.kind)) + "." + attr));
    }
    TR builtin(Eval& ev, const syntax::Call* call, const std::string& name, const Args&, const Kw&, const State&) const override {
        return TR(ev.issue(call, IssueClass::Stdlib, name));
    }
    TR std_call(Eval& ev, const syntax::Call* call, const std::string& name, const Args&, const Kw&, const State&) const override {
        return TR(ev.issue(call, IssueClass::Stdlib, name));
    }
};

}  // namespace

const Library& unmapped_library() {
    static const UnmappedLibrary library;
    return library;
}

ModuleEval::ModuleEval(Program& program, Module* module, bool record_types, const Library& library)
    : Eval(program, nullptr, record_types, library) {
    m = module;
}

// A value the front end cannot follow is an isolated failure: that name has no type, the rest of the module goes on.
Type ModuleEval::safe_ev(const syntax::Expr* value) {
    try {
        return ev(value, State());
    } catch (const std::exception& e) {
        return P::top(std::string("frontend-error: ") + e.what());
    }
}

Eval& ModuleEval::run() {
    Module& module = *m;
    OrderedMap<std::string, Type>& globals = p.globals[module.name];
    for (const auto& [name, values] : module.assigns) {
        Type t = P::bottom();
        for (const syntax::Expr* v : values) t = p.join(t, v != nullptr ? safe_ev(v) : P::top("module-level unpacking"));
        const Type old_type = get_or(globals, name, P::bottom());
        const Type new_type = p.join(old_type, t);
        if (new_type != old_type) {
            globals.insert_or_assign(name, new_type);
            p.touch(FactKey::globals(module.name));
        }
    }
    for (const auto& [class_name, c] : module.classes) {
        for (const auto& [name, value] : c->class_attrs) {
            const Type t = safe_ev(value);
            const program::NameKey key{c->qname, name};
            const Type old_type = get_or(p.class_attr_types, key, P::bottom());
            const Type new_type = p.join(old_type, t);
            if (new_type != old_type) {
                p.class_attr_types[key] = new_type;
                p.touch(FactKey::class_attrs());
            }
        }
    }
    std::vector<Fn*> fns;
    for (const auto& [name, f] : module.functions) fns.push_back(f);
    for (const auto& [class_name, c] : module.classes) {
        for (const auto& [name, f] : c->methods) fns.push_back(f);
    }
    for (const Fn* f : fns) {
        for (const auto& [name, value] : f->defaults) {
            // a mutable default is shared between calls, which the C++ cannot reproduce
            const bool mutable_default = syntax::as<syntax::List>(value) != nullptr || syntax::as<syntax::Dict>(value) != nullptr ||
                                         syntax::as<syntax::Set>(value) != nullptr || syntax::as<syntax::Call>(value) != nullptr;
            const Type t = mutable_default ? P::top("mutable default") : safe_ev(value);
            const program::NameKey key{f->qname, name};
            const Type old_type = get_or(p.default_types, key, P::bottom());
            const Type new_type = p.join(old_type, t);
            if (new_type != old_type) {
                p.default_types[key] = new_type;
                p.touch(FactKey::defaults());
            }
        }
    }
    return *this;
}

const std::unordered_map<std::string, Type>& std_attrs() {
    static const std::unordered_map<std::string, Type> table = {
        {"sys.maxsize", P::const_int(P::i64_max())},
        {"os.sep", P::str()},
        {"os.linesep", P::str()},
        {"os.pathsep", P::str()},
        {"math.pi", P::floating()},
        {"string.hexdigits", P::str()},
        {"string.digits", P::str()},
        {"string.ascii_letters", P::str()},
        {"string.printable", P::str()},
        {"struct.error", named_type(Kind::Cls, std::string(kExcPrefix) + "struct.error")},
    };
    return table;
}

void Inference::run_module(Program& program, Module& module) { ModuleEval(program, &module, false, lib_).run(); }

void Inference::run_function(Program& program, Fn& fn) { Eval(program, &fn, false, lib_).run(); }

}  // namespace cb::translate::frontends::python::infer
