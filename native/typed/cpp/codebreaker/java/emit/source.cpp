// codebreaker/java/emit/source.cpp -- typed C++ port of codebreaker.java.emit.source (see source.hpp).

#include "codebreaker/java/emit/source.hpp"

#include "codebreaker/java/emit/naming.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/typemap.hpp"

namespace codebreaker::java::emit::source {

namespace {

using py::str;
using ledger::Category;
using ledger::Severity;

str S(const char* s) { return str(s); }

// '\t' + s for each line
py::list<str> tabbed(const py::list<str>& lines) {
    py::list<str> out;
    for (const str& l : lines) {
        out.append(S("\t") + l);
    }
    return out;
}

// apply_std_transforms(text, cpp_std): the transform pass reads only the standard's year.
str apply_transforms(const str& text, const std::optional<CppStd>& cpp_std) {
    std::optional<std_transforms::CppStandard> std_;
    if (cpp_std) {
        std_ = std_transforms::CppStandard{cpp_std->year};
    }
    return std_transforms::apply_std_transforms(text, std_);
}

// '\n'.join(L).rstrip() + '\n'
str finish(const py::list<str>& L, const std::optional<CppStd>& cpp_std) {
    return apply_transforms(py::join(S("\n"), L).rstrip() + S("\n"), cpp_std);
}

}  // namespace

Bodies* TREE_BODIES = nullptr;

const py::re::Pattern _NEW_ARRAY_AGG_MULTI = py::re::compile(S(R"RX(^new\s+[\w.]+\s*(?:\[\s*\]\s*){2,}\{)RX"), py::re::S);
const py::set<py::str> _PRIMITIVE_CPP = {S("int"),           S("short"),  S("long long"), S("char"),  S("unsigned char"),
                                         S("signed char"),   S("float"),  S("double"),    S("bool"),  S("long")};

str _banner(const str& cls_name, const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label) {
    return header::_banner(cls_name, cpp_std, platform_label);
}

std::optional<str> _default_value(const str& cpp_type) {
    if (cpp_type.endswith(S("*"))) {
        return S("NULL");
    }
    if (_PRIMITIVE_CPP.contains(cpp_type)) {
        return S("0");
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// static field definitions
// ---------------------------------------------------------------------------
str _array_elem_cpp(const JTypeRef& jtype, const TypeResolver& resolver) {
    JTypeRef elem_jt = jtype->element(jtype);
    if (elem_jt->name == S("byte") && !elem_jt->is_array()) {  // byte[] buffer -> unsigned char (see types.py)
        return S("unsigned char");
    }
    str elem = resolver.cpp_type(elem_jt, S("element"));
    if (resolver.ownership(elem_jt) == S("pointer") && !elem.endswith(S("*"))) {
        elem += S("*");
    }
    return elem;
}

bool _is_deferrable_static_init(const str& init, const str& ft) {
    str head = py::split(ft, S("<"))[0];
    if (py::split(head).contains(S("const"))) {
        return false;
    }
    if (py::re::search(S(R"RX(\bnew\b|::|->)RX"), init)) {
        return true;
    }
    // a call to a lowercase-initial function name (not a PascalCase constructor) -> defer
    return static_cast<bool>(py::re::search(S(R"RX((^|[^\w:.])[a-z]\w*\s*\()RX"), init));
}

std::tuple<py::list<str>, py::list<str>> _render_statics(const ClassRef& cls, const TypeResolver& resolver,
                                                         ledger::Ledger* ledger) {
    py::list<str> defs, inits;
    for (const FieldRef& f : cls->static_fields()) {
        str name = naming::field_name(naming::Field{f->name, f->is_static});
        str ft = resolver.field_type(f->jtype);
        str init_raw = (f->initializer ? *f->initializer : S("")).strip();
        std::optional<str> tree;
        if (TREE_BODIES != nullptr && !init_raw.empty()) {
            tree = TREE_BODIES->field_init(cls->name, f->name);
        }
        std::optional<py::list<str>> items;
        if (TREE_BODIES != nullptr && ownership::aggregate_array_body(f)) {
            items = TREE_BODIES->array_items(cls->name, f->name);
        }
        if (!init_raw.empty() && !tree && !items) {
            defs.append(py::percent(S("%s %s::%s;"), ft, cls->name, name));
            inits.extend(_gap(cls, ledger, py::percent(S("static field %s.%s"), cls->name, f->name), f->line));
            continue;
        }
        if (_NEW_ARRAY_AGG_MULTI.match(init_raw)) {  // new T[][]{...} -> nested JArrayInit (C++03)
            // the nested JArrayInit comma-expression the runtime converts to JArray<JArray<T> >. Deferred
            // into initStatics() like any value-constructing init -- it allocates, so it isn't a constant.
            defs.append(py::percent(S("%s %s::%s;"), ft, cls->name, name));  // zero/default now (order-safe)
            // nested-array value in initStatics()
            inits.append(py::percent(S("%s::%s = %s;"), cls->name, name, tree ? *tree : S("None")));
            if (ledger != nullptr) {
                ledger->add(cls->name + S(".cpp"), Category::RISK,
                            S("multi-dim static array new T[][]{...} -> nested JArrayInit "
                              "(C++03 has no braced new[][]; each level length-tracked)"),
                            f->line, Severity::LOW, S("emit"), S("static-aggregate-array-nd"));
            }
            continue;
        }
        if (items) {  // new T[]{a,b,c} -> file-static data + JArray
            str elem = _array_elem_cpp(f->jtype, resolver);
            str data = py::percent(S("_%s_%s_data"), cls->name, name);
            defs.append(py::percent(S("static %s %s[] = { %s };"), elem, data, py::join(S(", "), *items)));
            defs.append(py::percent(S("%s %s::%s(%s, %d);"), ft, cls->name, name, data, items->size()));
            if (ledger != nullptr) {
                ledger->add(cls->name + S(".cpp"), Category::RISK,
                            S("static array new T[]{...} -> JArray over a file-static data array "
                              "(C++03 has no braced new[]; length tracked)"),
                            f->line, Severity::LOW, S("emit"), S("static-aggregate-array"));
            }
            continue;
        }
        // forward-declared static -> NULL/0/default
        std::optional<str> init = !init_raw.empty() ? tree : _default_value(ft);
        if (!init) {
            defs.append(py::percent(S("%s %s::%s;"), ft, cls->name, name));
        } else if (_is_deferrable_static_init(*init, ft)) {
            defs.append(py::percent(S("%s %s::%s;"), ft, cls->name, name));        // zero-init now (order-safe)
            inits.append(py::percent(S("%s::%s = %s;"), cls->name, name, *init));  // initialize in ordered initStatics()
        } else {
            defs.append(py::percent(S("%s %s::%s = %s;"), ft, cls->name, name, *init));  // compile-time const: eager
        }
    }
    return {defs, inits};
}

// ---------------------------------------------------------------------------
// constructors
// ---------------------------------------------------------------------------
py::list<str> _init_list(const ClassRef& cls, const TypeResolver& resolver) {
    py::list<str> items;
    for (const FieldRef& f : cls->instance_fields()) {
        std::optional<str> dv = _default_value(resolver.field_type(f->jtype));
        if (dv) {
            items.append(py::percent(S("%s(%s)"), naming::field_name(naming::Field{f->name, f->is_static}), *dv));
        }
    }
    return items;
}

py::list<str> _gap(const ClassRef& cls, ledger::Ledger* ledger, const str& what, int64_t line) {
    if (ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::STUBBED,
                    py::percent(S("untranslated %s: the back end declined it -- the body throws "
                                  "UnsupportedOperationException (J2X-GAP)"),
                                what),
                    line, Severity::HIGH, S("emit"), S("untranslated"));
    }
    return py::list<str>{py::percent(S("// J2X-GAP: %s was not translated (see the ledger)"), what),
                         S("throw new UnsupportedOperationException();")};
}

py::list<str> _instance_init_lines(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger) {
    std::optional<py::list<str>> tree;
    if (TREE_BODIES != nullptr) {
        tree = TREE_BODIES->instance_inits(cls->name);
    }
    if (tree) {
        return *tree;
    }
    bool any_init = false;
    for (const FieldRef& f : cls->instance_fields()) {
        if (f->initializer) {
            any_init = true;
            break;
        }
    }
    if (!any_init && cls->inst_inits.empty()) {
        return py::list<str>();
    }
    return _gap(cls, ledger, py::percent(S("instance initialisers of %s"), cls->name));
}

py::list<str> _render_ctor(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver,
                           ledger::Ledger* ledger) {
    str params = ownership::params_str(m, resolver);
    std::optional<CtorParts> tree;
    if (TREE_BODIES != nullptr) {
        tree = TREE_BODIES->ctor(cls->name, m);
    }
    py::list<str> items, body;
    if (tree) {
        if (tree->args && cls->parent && !cls->parent->empty()) {
            items.append(py::percent(S("%s(%s)"), *cls->parent, *tree->args));
        }
        items.extend(_init_list(cls, resolver));
        body = _instance_init_lines(cls, resolver, ledger) + tree->body;
    } else {
        items = _init_list(cls, resolver);
        body = _gap(cls, ledger, py::percent(S("constructor %s(%s)"), cls->name, params), m ? m->line : 0);
    }
    py::list<str> L{py::percent(S("%s::%s(%s)"), cls->name, cls->name, params)};
    if (!items.empty()) {
        L.append(S("\t: ") + py::join(S("\n\t, "), items));
    }
    L.append(S("{"));
    L.extend(tabbed(body));
    L.append(S("}"));
    return L;
}

py::list<str> _render_two_phase(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                                const py::set<str>& two_phase) {
    bool parent_two = cls->parent && two_phase.contains(*cls->parent);
    py::list<str> items = _init_list(cls, resolver);
    py::list<str> L{py::percent(S("%s::%s()"), cls->name, cls->name)};
    if (!items.empty()) {
        L.append(S("\t: ") + py::join(S("\n\t, "), items));
    }
    L.extend(py::list<str>{S("{"), S("}"), S("")});
    py::list<MethodRef> ctors = cls->constructors();
    if (ctors.empty()) {
        ctors.append(nullptr);  // [None]
    }
    for (const MethodRef& m : ctors) {
        str params = m ? ownership::params_str(m, resolver) : S("");
        std::optional<CtorParts> tree;
        if (TREE_BODIES != nullptr) {
            tree = TREE_BODIES->ctor(cls->name, m, true);
        }
        L.extend(py::list<str>{py::percent(S("%s* %s::j2xInit(%s)"), cls->name, cls->name, params), S("{")});
        if (tree) {
            py::list<str> head;
            if (!(tree->kind && *tree->kind == S("this")) && parent_two) {
                // super(...), or Java's implicit one
                head.append(py::percent(S("%s::j2xInit(%s);"), *cls->parent, tree->args ? *tree->args : S("")));
            }
            py::list<str> inits;
            if (!(tree->kind && *tree->kind == S("this"))) {
                inits = _instance_init_lines(cls, resolver, ledger);
            }
            L.extend(tabbed(head + inits + tree->body));
            L.extend(py::list<str>{S("\treturn this;"), S("}"), S("")});
        } else {
            L.extend(tabbed(_gap(cls, ledger, py::percent(S("constructor %s(%s)"), cls->name, params), m ? m->line : 0)));
            L.extend(py::list<str>{S("}"), S("")});
        }
    }
    if (ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::RULE,
                    S("two-phase construction: constructors are j2xInit(...) so their virtual calls reach the "
                      "override, as in Java"),
                    0, Severity::LOW, S("emit"), S("two-phase-ctor"));
    }
    return L.slice(0, L.size() - 1);
}

// ---------------------------------------------------------------------------
// destructor (Rule of Three: free heap allocated in constructors)
// ---------------------------------------------------------------------------
py::list<str> _render_dtor(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger) {
    py::dict<str, bool> owned = ownership::owned_pointers(cls, resolver);
    py::dict<str, str> containers = ownership::owned_containers(cls, resolver);
    py::list<str> L{py::percent(S("%s::~%s()"), cls->name, cls->name), S("{")};
    // container ELEMENTS first (an element may reference a scalar-owned sibling), then scalar pointers
    for (const str& name : py::sorted(py::list<str>(containers.keys()))) {
        L.append(py::percent(S("\tfor (int _i = 0; _i < %s%s; ++_i) { delete %s[_i]; }"), name, containers.at(name), name));
    }
    for (const str& name : py::sorted(py::list<str>(owned.keys()))) {
        L.append(py::percent(S("\tdelete%s %s;"), owned.at(name) ? S("[]") : S(""), name));
        L.append(py::percent(S("\t%s = NULL;"), name));
    }
    L.append(S("}"));
    py::list<str> freed_all;
    for (const str& k : owned.keys()) {
        freed_all.append(k);
    }
    for (const str& k : containers.keys()) {
        freed_all.append(k);
    }
    py::list<str> freed = py::sorted(freed_all);
    if (!freed.empty() && ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::RISK,
                    py::percent(S("Rule-of-Three: synthesized destructor frees %d heap-owning field(s): %s"), freed.size(),
                                py::join(S(", "), freed)),
                    0, Severity::MED, S("emit"), S("rule-of-three-dtor"));
    }
    return L;
}

// ---------------------------------------------------------------------------
// methods
// ---------------------------------------------------------------------------
py::list<str> _render_method(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver,
                             ledger::Ledger* ledger) {
    str ret = ownership::return_cpp_type(m, resolver);
    str params = ownership::params_str(m, resolver);
    str name = resolver.renamed_method(cls->name, m->name);  // field/method name-collision -> name_
    py::list<str> L{py::percent(S("%s %s::%s(%s)"), ret, cls->name, name, params), S("{")};
    std::optional<py::list<str>> tree;
    if (TREE_BODIES != nullptr) {
        tree = TREE_BODIES->body(cls->name, m);
    }
    py::list<str> body = tree ? tree->copy() : _gap(cls, ledger, py::percent(S("method %s"), m->name), m->line);
    L.extend(tabbed(body));
    L.append(S("}"));
    return L;
}

py::list<str> _render_mi_forward(const ClassRef& cls, const MethodRef& im, const str& base, const TypeResolver& resolver,
                                 ledger::Ledger* ledger) {
    str ret = ownership::return_cpp_type(im, resolver);
    str params = ownership::params_str(im, resolver);
    py::list<str> names;
    for (const py::ref<ir::Param>& p : im->params) {
        names.append(p->name);
    }
    str call_args = py::join(S(", "), names);
    str ret_kw = ret == S("void") ? S("") : S("return ");
    if (ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::RULE,
                    py::percent(S("MI-diamond: synthesized %s() forwarding to %s:: (interface decl + base impl "
                                  "unified into one vtable override)"),
                                im->name, base),
                    0, Severity::LOW, S("emit"), S("mi-forward"));
    }
    return py::list<str>{py::percent(S("%s %s::%s(%s)"), ret, cls->name, im->name, params), S("{"),
                         py::percent(S("\t%s%s::%s(%s);"), ret_kw, base, im->name, call_args), S("}")};
}

// ---------------------------------------------------------------------------
py::list<str> _referenced_types(const ClassRef& cls, const TypeResolver& resolver, const str& body_text) {
    py::set<str> names;

    // (recursive)
    struct Adder {
        py::set<str>& names;
        void operator()(const JTypeRef& jt) const {
            if (jt) {
                names.add(jt->name);
                for (const JTypeRef& a : jt->args) {
                    (*this)(a);
                }
            }
        }
    } add{names};

    if (cls->parent && !cls->parent->empty()) {
        names.add(*cls->parent);
    }
    names.update(cls->interfaces);
    py::list<FieldRef> all_fields = cls->fields + cls->inherited_fields;
    for (const FieldRef& f : all_fields) {
        add(f->jtype);
    }
    for (const MethodRef& m : cls->methods) {
        add(m->return_type);
        for (const py::ref<ir::Param>& p : m->params) {
            add(p->jtype);
        }
    }
    // new X / X:: / casts / locals
    for (const py::re::Match& m : py::re::finditer(S(R"RX(\b([A-Z]\w*)\b)RX"), body_text)) {
        names.add(m.group(1));
    }
    if (body_text.contains(S("j2xClassOf")) || body_text.contains(S("typeid"))) {  // `.class` / getClass() -> needs Class.h
        names.add(S("Class"));
    }
    // transitive CLOSURE: the statement translator emits arbitrarily deep chains
    // (`this->mc->gameSettings->keyBindInventory->keyCode`), so a .cpp needs the FULL
    // definition of every type reachable through field/method links -- not just one hop.
    // Iterate to a fixpoint over the resolved member environment.
    py::set<str> seen;
    py::list<str> frontier = py::list<str>(py::sorted(names));
    while (!frontier.empty()) {
        str n = frontier.pop();
        if (seen.contains(n)) {
            continue;
        }
        seen.add(n);
        if (!resolver.members.contains(n)) {
            continue;
        }
        const types::MemberEnv& env = resolver.members.at(n);
        for (const JTypeRef& jt : env.fields.values()) {
            add(jt);  // may grow `names`
        }
        for (const JTypeRef& jt : env.methods.values()) {
            add(jt);
        }
        for (const str& x : names - seen) {
            frontier.append(x);
        }
    }
    py::set<str> includable = py::set<str>::from(resolver.known) | typemap::RUNTIME_HEADERS;
    py::set<str> only_cls;
    only_cls.add(cls->name);
    return py::sorted((names & includable) - only_cls);
}

str _render_typed_enum_source(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                              const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label) {
    py::list<str> body;
    // static definitions: the singleton constants + real enum statics (zero-init) + values backing
    for (const py::ref<ir::EnumConst>& ec : cls->enum_consts) {
        body.append(py::percent(S("%s* %s::%s;"), cls->name, cls->name, naming::enum_member(ec->name)));
    }
    for (const FieldRef& f : cls->static_fields()) {
        body.append(py::percent(S("%s %s::%s;"), resolver.field_type(f->jtype), cls->name, f->name));
    }
    body.append(py::percent(S("JArray<%s* > %s::_j2x_values;"), cls->name, cls->name));
    body.append(S(""));

    for (const MethodRef& m : cls->constructors()) {
        body.extend(_render_ctor(cls, m, resolver, ledger) + py::list<str>{S("")});
    }
    body.extend(_render_dtor(cls, resolver, ledger) + py::list<str>{S("")});
    for (const MethodRef& m : cls->methods) {
        if (!m->is_ctor && !m->is_abstract) {
            body.extend(_render_method(cls, m, resolver, ledger) + py::list<str>{S("")});
        }
    }
    body.extend(py::list<str>{py::percent(S("int %s::ordinal()"), cls->name), S("{"), S("\treturn this->__ordinal;"), S("}"),
                              S("")});
    body.extend(py::list<str>{py::percent(S("JArray<%s* >& %s::values()"), cls->name, cls->name), S("{"),
                              S("\treturn _j2x_values;"), S("}"), S("")});

    // initStatics(): construct each singleton, set its ordinal, run real static inits, fill values().
    py::list<str> init;
    int64_t i = 0;
    for (const py::ref<ir::EnumConst>& ec : cls->enum_consts) {
        std::optional<py::list<str>> args;
        if (TREE_BODIES != nullptr) {
            args = TREE_BODIES->enum_args(cls->name, ec->name);
        }
        if (!args) {
            init.extend(_gap(cls, ledger, py::percent(S("enum constant %s.%s"), cls->name, ec->name)));
            ++i;
            continue;
        }
        str nm = naming::enum_member(ec->name);
        init.append(py::percent(S("%s = new %s(%s);"), nm, cls->name, py::join(S(", "), *args)));
        init.append(py::percent(S("%s->__ordinal = %d;"), nm, i));
        ++i;
    }
    for (const FieldRef& f : cls->static_fields()) {
        if (f->initializer && !f->initializer->empty()) {
            std::optional<str> value;
            if (TREE_BODIES != nullptr) {
                value = TREE_BODIES->field_init(cls->name, f->name);
            }
            if (!value) {
                init.extend(_gap(cls, ledger, py::percent(S("static field %s.%s"), cls->name, f->name)));
            } else {
                init.append(py::percent(S("%s = %s;"), f->name, *value));
            }
        }
    }
    init.extend(_static_block_lines(cls, ledger));
    init.append(py::percent(S("_j2x_values = JArray<%s* >(%d);"), cls->name, cls->enum_consts.size()));
    i = 0;
    for (const py::ref<ir::EnumConst>& ec : cls->enum_consts) {
        init.append(py::percent(S("_j2x_values[%d] = %s;"), i, naming::enum_member(ec->name)));
        ++i;
    }
    cls->has_init_statics = true;  // always: the constant table must be built
    body.extend(py::list<str>{py::percent(S("void %s::initStatics()"), cls->name), S("{")});
    body.extend(tabbed(init));
    body.extend(py::list<str>{S("}"), S("")});

    py::list<str> used = _referenced_types(cls, resolver, py::join(S("\n"), body));
    py::list<str> L{S("#include \"stdafx.h\""), S("#include \"JavaRT.h\""), S(""),
                    source::_banner(cls->name, cpp_std, platform_label), S(""), py::percent(S("#include \"%s.h\""), cls->name)};
    for (const str& n : used) {
        L.append(py::percent(S("#include \"%s.h\""), n));
    }
    L.append(S(""));
    L.extend(body);
    if (ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::PORTED,
                    py::percent(S("typed enum source: %d singletons built in initStatics()"), cls->enum_consts.size()), 0,
                    std::nullopt, S("emit"), S("typed-enum"));
    }
    return finish(L, cpp_std);
}

// ---------------------------------------------------------------------------
std::optional<str> _detect_self_singleton(const str& clsname, const py::list<str>& static_init_lines) {
    py::re::Pattern pat = py::re::compile(S("^") + py::re::escape(clsname) + S(R"RX(::(\w+)\s*=\s*new\s+)RX") +
                                          py::re::escape(clsname) + S(R"RX(\b)RX"));
    for (const str& l : static_init_lines) {
        std::optional<py::re::Match> m = pat.match(l.strip());
        if (m) {
            return m->group(1);
        }
    }
    return std::nullopt;
}

py::list<str> _inject_lazy_singleton(py::list<str> mlines, const str& clsname, const str& field) {
    int64_t open_i = -1;
    for (int64_t k = 0; k < mlines.size(); ++k) {
        if (mlines[k] == S("{")) {
            open_i = k;
            break;
        }
    }
    if (open_i < 0) {  // ValueError: no '{'
        return mlines;
    }
    py::list<int64_t> stmts;
    for (int64_t k = open_i + 1; k < mlines.size(); ++k) {
        str t = mlines[k].strip();
        if (!(t == S("") || t == S("}"))) {
            stmts.append(k);
        }
    }
    if (stmts.size() != 1) {
        return mlines;
    }
    int64_t i = stmts[0];
    str t = mlines[i].strip();
    if (t == py::percent(S("return %s;"), field) || t == py::percent(S("return %s::%s;"), clsname, field)) {
        str indent = mlines[i].slice(0, mlines[i].size() - mlines[i].lstrip().size());
        mlines.insert(i, py::percent(S("%sif (%s == NULL) initStatics();   // J2X: lazy singleton (Java <clinit> on static access)"),
                                     indent, field));
    }
    return mlines;
}

py::list<str> _static_block_lines(const ClassRef& cls, ledger::Ledger* ledger) {
    if (cls->static_inits.empty()) {
        return py::list<str>();
    }
    std::optional<py::list<py::list<str>>> tree;
    if (TREE_BODIES != nullptr) {
        tree = TREE_BODIES->static_blocks(cls->name);
    }
    if (!tree || tree->size() != cls->static_inits.size()) {
        return _gap(cls, ledger, py::percent(S("static {} blocks of %s"), cls->name));
    }
    py::list<str> out;
    for (const py::list<str>& lines : *tree) {
        for (const str& line : lines) {
            out.append(line);
        }
    }
    return out;
}

std::optional<str> render_source(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                                 const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label,
                                 const std::optional<py::set<str>>& two_phase_) {
    if (cls->is_enum && resolver.typed_enums.contains(cls->name)) {
        return _render_typed_enum_source(cls, resolver, ledger, cpp_std, platform_label);
    }
    if (cls->is_interface || cls->is_enum) {
        return std::nullopt;
    }

    py::set<str> two_phase_set = two_phase_ ? *two_phase_ : py::set<str>();  // getattr(resolver, 'two_phase', ())
    py::list<str> body;
    auto [static_defs, static_init_lines] = _render_statics(cls, resolver, ledger);
    std::optional<str> singleton_field = _detect_self_singleton(cls->name, static_init_lines);  // lazy-init emulation
    if (!static_defs.empty()) {
        body.extend(static_defs + py::list<str>{S("")});
    }
    bool two_phase = two_phase_set.contains(cls->name);
    if (two_phase) {
        body.extend(_render_two_phase(cls, resolver, ledger, two_phase_set) + py::list<str>{S("")});
    } else {
        for (const MethodRef& m : cls->constructors()) {
            body.extend(_render_ctor(cls, m, resolver, ledger) + py::list<str>{S("")});
        }
    }
    // synthesized zero-initializing default ctor for a value type that lacks one (so it can be
    // held by value as a field); matches the declaration emitted by header.py
    py::list<MethodRef> ctors = cls->constructors();
    bool has_arity0 = false;
    for (const MethodRef& c : ctors) {
        if (c->arity() == 0) {
            has_arity0 = true;
        }
    }
    if (two_phase) {
        // (nothing)
    } else if (resolver.values.contains(cls->name) && !ctors.empty() && !has_arity0) {
        py::list<str> items = _init_list(cls, resolver);
        py::list<str> L2{py::percent(S("%s::%s()"), cls->name, cls->name)};
        if (!items.empty()) {
            L2.append(S("\t: ") + py::join(S("\n\t, "), items));
        }
        L2.extend(py::list<str>{S("{"), S("}")});
        body.extend(L2 + py::list<str>{S("")});
    }
    // A class with NO explicit ctor relies on Java's implicit default ctor, which zero-inits ALL fields
    // and runs any initializers/init-blocks. C++'s implicit ctor leaves primitives/pointers as garbage
    // (the uninitialized-field crash class), so synthesize a zero-init default ctor whenever the class
    // has ANY instance fields (or init-blocks). Matches the declaration emitted by header.py.
    else if (ctors.empty() && (!cls->instance_fields().empty() || !cls->inst_inits.empty())) {
        py::list<str> items = _init_list(cls, resolver);
        py::list<str> inits = _instance_init_lines(cls, resolver, ledger);
        py::list<str> L2{py::percent(S("%s::%s()"), cls->name, cls->name)};
        if (!items.empty()) {
            L2.append(S("\t: ") + py::join(S("\n\t, "), items));
        }
        L2.append(S("{"));
        L2.extend(tabbed(inits));
        L2.append(S("}"));
        body.extend(L2 + py::list<str>{S("")});
    }
    body.extend(_render_dtor(cls, resolver, ledger) + py::list<str>{S("")});
    for (const MethodRef& m : cls->methods) {
        if (m->is_ctor || m->is_abstract) {
            continue;
        }
        py::list<str> mlines = _render_method(cls, m, resolver, ledger);
        if (singleton_field) {
            mlines = _inject_lazy_singleton(mlines, cls->name, *singleton_field);
        }
        body.extend(mlines + py::list<str>{S("")});
    }
    // An abstract method CALLED from a constructor (Entity's ctor calls the abstract entityInit()) needs
    // an out-of-line definition: from the base ctor the call resolves NON-virtually to THIS class's version,
    // so a pure `= 0` with no body is an unresolved external. A pure virtual is allowed to have a body;
    // emit an empty one (subclasses still override it). Java would dispatch to the override here, but C++
    // calls the base during base-construction -- the standard virtual-call-in-ctor divergence.
    py::list<str> ctor_lines;
    for (const MethodRef& c : ctors) {
        for (const auto& ln_t : c->body) {
            ctor_lines.append(std::get<1>(ln_t));
        }
    }
    str ctor_text = py::join(S("\n"), ctor_lines);
    for (const MethodRef& m : cls->methods) {
        if (!m->is_abstract) {
            continue;
        }
        if (py::re::search(S(R"RX((?<![\w.>])(?:this\s*(?:\.|->)\s*)?)RX") + py::re::escape(m->name) + S(R"RX(\s*\()RX"),
                           ctor_text)) {
            str rt = ownership::return_cpp_type(m, resolver);
            str sig = py::percent(S("%s %s::%s(%s)"), rt, cls->name, m->name, ownership::params_str(m, resolver));
            if (rt == S("void")) {
                body.extend(py::list<str>{sig, S("{"), S("}"), S("")});
            } else {
                std::optional<str> dv = _default_value(rt);
                body.extend(py::list<str>{sig, S("{"), S("\treturn ") + (dv ? *dv : rt + S("()")) + S(";"), S("}"), S("")});
            }
            if (ledger != nullptr) {
                ledger->add(cls->name + S(".cpp"), Category::RISK,
                            py::percent(S("abstract %s() is called from a constructor -- emitted an empty base definition "
                                          "(C++ calls the base, not the override, during base construction)"),
                                        m->name),
                            m->line, Severity::MED, S("emit"), S("abstract-called-in-ctor"));
            }
        }
    }
    for (const auto& im_base : cls->mi_forwards) {
        body.extend(_render_mi_forward(cls, std::get<0>(im_base), std::get<1>(im_base), resolver, ledger) +
                    py::list<str>{S("")});
    }

    // ORDERED STATIC INIT: deferred static field initializers + Java static{} blocks are run by a
    // single `void <Class>::initStatics()`, which the generated driver (j2x_init.cpp / j2x_initAll())
    // calls in dependency order -- NOT by a file-local trigger (whose cross-translation-unit order is
    // undefined: the static-init-order crash class -- Block initializing before Material -> null vtable).
    // Static field DEFS above are zero-initialized; initStatics() assigns the real values, in order.
    py::list<str> init_body = static_init_lines.copy();  // deferred static FIELD initializers first
    init_body.extend(_static_block_lines(cls, ledger));  // then the Java static{} block bodies
    // ALWAYS emit initStatics() (empty when nothing to defer) so the driver can call it on any class
    // without a header/source mismatch. header.py declares it for every concrete class to match.
    // cls.has_init_statics lets the pipeline driver call only the classes that actually defer work.
    cls->has_init_statics = !init_body.empty();
    body.extend(py::list<str>{py::percent(S("void %s::initStatics()"), cls->name), S("{")});
    if (singleton_field && !init_body.empty()) {  // idempotent: a lazy getter may have already built the singleton
        body.append(py::percent(S("\tif (%s != NULL) return;   // J2X: idempotent (may be lazily inited via a singleton getter)"),
                                *singleton_field));
    }
    body.extend(tabbed(init_body));
    body.extend(py::list<str>{S("}"), S("")});
    if (!init_body.empty() && ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::RISK,
                    S("static field inits + static{} blocks deferred into initStatics(), run in "
                      "dependency order by j2x_initAll() (kills the static-init-order crash class)"),
                    0, Severity::MED, S("emit"), S("static-init-ordered"));
    }

    // a .cpp needs the FULL definition of every user type it touches (member access,
    // new/cast/static), not just the forward decls in its own header. Collect from the
    // class's signatures (parent, fields incl. inherited, params, returns) AND capitalized
    // tokens in the body -- the signatures catch types reached only through lowercase vars
    // (e.g. entity->posX, where the type 'Entity' is never spelled in the body).
    py::list<str> used = _referenced_types(cls, resolver, py::join(S("\n"), body));
    py::list<str> L{S("#include \"stdafx.h\""), S("#include \"JavaRT.h\""), S(""),
                    source::_banner(cls->name, cpp_std, platform_label), S(""), py::percent(S("#include \"%s.h\""), cls->name)};
    for (const str& n : used) {
        L.append(py::percent(S("#include \"%s.h\""), n));
    }
    L.append(S(""));
    L.extend(body);

    if (ledger != nullptr) {
        ledger->add(cls->name + S(".cpp"), Category::PORTED, py::percent(S("generated source for %s"), cls->name), 0,
                    std::nullopt, S("emit"), S("source"));
    }
    return finish(L, cpp_std);
}

}  // namespace codebreaker::java::emit::source
