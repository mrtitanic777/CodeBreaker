// The C# -> C++17 back end, classes: the declaration of each (members in the struct), the bodies out of line, the static state.
#include <algorithm>
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/csharp_cpp17_internal.hpp"

namespace cb::translate::backends::csharp_cpp17::detail {

namespace {

bool cc_has(const std::vector<std::string>& mods, std::string_view m) { return std::ranges::find(mods, m) != mods.end(); }

bool cc_static(const std::vector<std::string>& mods, const ir::SymP& sym) { return cc_has(mods, "static") || (sym && sym->is_static); }

// Whether a class has instance members with initialisers (run by every constructor that does not chain to another).
bool cc_has_field_inits(const ir::ClassDecl& d) {
    for (Node* m : d.members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) {
            if (f->init != nullptr && !cc_static(f->mods, f->sym) && !cc_has(f->mods, "const")) return true;
        } else if (const auto* p = ir::get<ir::Property>(m)) {
            if (p->init != nullptr && !cc_static(p->mods, p->sym)) return true;
        }
    }
    return false;
}

bool cc_has_ctor(const ir::ClassDecl& d) {
    for (Node* m : d.members) {
        const auto* md = ir::get<ir::MethodDecl>(m);
        if (md != nullptr && md->kind.value_or("") == "ctor") return true;
    }
    return false;
}

// The classes whose properties a record prints: the record's base records first.
}  // namespace

void Writer::write_class(const ClassInfo& c) {
    where_ = c.cname;
    member_names_.clear();
    for (Node* m : c.decl->members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) member_names_.insert(f->sym->name);
        else if (const auto* p = ir::get<ir::Property>(m)) member_names_.insert(p->sym->name);
        else if (const auto* md = ir::get<ir::MethodDecl>(m)) member_names_.insert(md->name.value_or(""));
    }
    std::string bases;
    bool has_base_class = false;
    const auto base_text = [&](const TypeP& t) -> std::string {
        const auto* ct = class_of(t);
        if (ct == nullptr || ct->name == "System.Object" || ct->name == "System.ValueType" || ct->name == "System.Enum") return "";
        if (ct->name.starts_with("System.") && !info(ct->name) && ct->name.find("Exception") == std::string::npos) return "";   // a library interface (IDisposable, IComparable)
        return class_text(*ct);
    };
    if (!c.is_struct) {
        const std::string base = base_text(c.decl->extends);
        if (!base.empty()) {
            bases = base;
            has_base_class = true;
        }
        std::string ifaces;
        for (const TypeP& i : c.decl->implements) {
            const std::string b = base_text(i);
            if (!b.empty()) ifaces += (ifaces.empty() ? "" : ", ") + b;
        }
        if (!has_base_class) bases = "virtual cs::Object";
        if (!ifaces.empty()) bases += ", " + ifaces;
    }
    try {
        open(template_head(c) + "struct " + c.cname + (bases.empty() ? "" : " : " + bases));
        write_members_in_class(c, self_type(c));
        close(";");
        line("");
    } catch (const Declined& d) {
        gaps_.push_back({c.cname, d.kind, d.why});
        out_ += "};\n";
        --indent_;
    }
}

void Writer::write_members_in_class(const ClassInfo& c, const std::string& self) {
    const ir::ClassDecl& d = *c.decl;
    const bool value_class = c.is_struct;
    for (Node* m : d.members) {
        try {
            if (const auto* f = ir::get<ir::FieldDecl>(m)) {
                where_ = c.cname + "." + f->sym->name;
                const std::string t = type(f->type);
                const std::string name = ident(f->sym->name);
                if (cc_has(f->mods, "const") && f->init != nullptr) line("static inline const " + t + " " + name + " = " + conv(f->init, f->type) + ";");
                else if (cc_static(f->mods, f->sym)) {
                    if (!c.typeparams.empty()) decline("generic-static", "a static field of a generic class");
                    line("static " + t + " " + name + ";");
                } else line(t + " " + name + "{};");
            } else if (const auto* p = ir::get<ir::Property>(m)) {
                where_ = c.cname + "." + p->sym->name;
                const PropInfo pi = *prop_of(p->sym);
                const bool is_static = p->sym->is_static;
                const std::string t = type(p->type);
                const std::string bk = "bk_" + (p->sym->name == "this[]" ? std::string("Item") : p->sym->name);
                if (pi.is_auto) {
                    if (is_static && !c.typeparams.empty()) decline("generic-static", "a static property of a generic class");
                    line(std::string(is_static ? "static " : "") + t + " " + bk + (is_static ? ";" : "{};"));
                }
                const bool virt = !is_static && !value_class && (c.is_interface || cc_has(p->mods, "virtual") || cc_has(p->mods, "abstract") || cc_has(p->mods, "override"));
                const bool pure = (c.is_interface || cc_has(p->mods, "abstract"));
                std::string idx_params = params_text(p->params, p->sym);
                for (Node* a : p->accessors) {
                    const auto* am = ir::get<ir::MethodDecl>(a);
                    const std::string kind = am->kind.value_or("get");
                    const bool getter = kind == "get";
                    std::string sig = std::string(is_static ? "static " : virt ? "virtual " : "");
                    sig += (getter ? t : "void") + " " + accessor_name(p->sym, kind) + "(";
                    sig += idx_params;
                    if (!getter) sig += (idx_params.empty() ? "" : ", ") + t + " value";
                    sig += ")";
                    if (cc_has(p->mods, "override") && !value_class) sig += " override";
                    if (pure && !ir::present(am->body) && !pi.is_auto) {
                        line(sig + " = 0;");
                    } else if (pi.is_auto && !pure) {
                        const std::string field = (is_static ? "" : "this->") + bk;
                        line(sig + " { " + (getter ? "return " + field + ";" : field + " = value;") + " }");
                    } else {
                        line(sig + ";");
                    }
                }
            } else if (const auto* md = ir::get<ir::MethodDecl>(m)) {
                where_ = c.cname + "." + md->name.value_or("?");
                const std::string kind = md->kind.value_or("method");
                if (kind == "ctor") {
                    line(c.cname + "(" + params_text(md->params, md->sym) + ");");
                } else if (kind == "static_init") {
                    line("static void cs_static_init();");
                } else if (kind == "method") {
                    const bool is_static = cc_static(md->mods, md->sym);
                    const bool virt = !is_static && !value_class && (c.is_interface || cc_has(md->mods, "virtual") || cc_has(md->mods, "abstract") || cc_has(md->mods, "override"));
                    std::string sig = template_head(ClassInfo{}, md->typeparams);
                    sig += std::string(is_static ? "static " : virt ? "virtual " : "");
                    sig += type(md->ret) + " " + ident(md->name.value()) + "(" + params_text(md->params, md->sym) + ")";
                    if (cc_has(md->mods, "override") && !value_class) sig += " override";
                    if ((c.is_interface || cc_has(md->mods, "abstract")) && !ir::present(md->body)) sig += " = 0";
                    line(sig + ";");
                }
            }
        } catch (const Declined& dec) {
            gaps_.push_back({where_, dec.kind, dec.why});
        }
    }
    where_ = c.cname;
    if (c.is_record) {   // the properties of a record's primary constructor
        for (Node* m : d.members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md == nullptr || md->kind.value_or("") != "ctor" || !cc_has(md->mods, "primary")) continue;
            for (Node* p : md->params) {
                const auto* pa = ir::get<ir::Param>(p);
                const std::string t = type(pa->type);
                line(t + " bk_" + pa->sym->name + "{};");
                line(t + " get_" + pa->sym->name + "() { return this->bk_" + pa->sym->name + "; }");
            }
        }
    }
    if (value_class) line(c.cname + "() = default;");
    if (!c.is_struct && !c.is_interface) {
        std::string q = d.qname.value_or(c.cname);
        line("std::u16string cs_type_name() const override { return " + u16_literal_text(q) + "; }");
    }
    if (cc_has_field_inits(d)) line("void cs_init_fields();");
    if (!cc_has_ctor(d) && !c.is_interface && cc_has_field_inits(d)) line(c.cname + "();");
    if (c.is_record) {
        line("cs::String ToString() override;");
        line("bool Equals(const cs::Ref<cs::Object>& other) override;");
        line("int GetHashCode() override;");
        line("static bool op_Equality(const cs::Ref<" + self + ">& a, const cs::Ref<" + self + ">& b);");
        line("static bool op_Inequality(const cs::Ref<" + self + ">& a, const cs::Ref<" + self + ">& b);");
    }
}

// ------------------------------------------------------------------------------------------------------------------- bodies

void Writer::write_bodies(const ClassInfo& c) {
    where_ = c.cname;
    member_names_.clear();
    const ir::ClassDecl& d = *c.decl;
    for (Node* m : d.members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) member_names_.insert(f->sym->name);
        else if (const auto* p = ir::get<ir::Property>(m)) member_names_.insert(p->sym->name);
        else if (const auto* md = ir::get<ir::MethodDecl>(m)) member_names_.insert(md->name.value_or(""));
    }
    const std::string self = self_type(c);
    for (Node* m : d.members) {
        const std::size_t mark = out_.size();
        const int indent = indent_;
        try {
            if (const auto* md = ir::get<ir::MethodDecl>(m)) {
                where_ = c.cname + "." + md->name.value_or("?");
                const std::string kind = md->kind.value_or("method");
                if (kind == "ctor") write_ctor_body(c, *md);
                else if (kind == "static_init" || (kind == "method" && ir::present(md->body))) write_method_body(c, *md, self);
            } else if (const auto* p = ir::get<ir::Property>(m)) {
                where_ = c.cname + "." + p->sym->name;
                const PropInfo pi = *prop_of(p->sym);
                for (Node* a : p->accessors) {
                    const auto* am = ir::get<ir::MethodDecl>(a);
                    if (!ir::present(am->body) || pi.is_auto) continue;
                    const std::string kind = am->kind.value_or("get");
                    const bool getter = kind == "get";
                    std::string idx = params_text(p->params, p->sym);
                    const std::string t = type(p->type);
                    boxed_.clear();
                    hoisted_.clear();
                    returns_.push_back(getter ? p->type : ir::void_type());
                    collect_captures(am->body.index() == 0 ? std::get<Node*>(am->body) : nullptr);
                    std::string head = template_head(c) + (getter ? t : "void") + " " + self + "::" + accessor_name(p->sym, kind) + "(" + idx;
                    if (!getter) head += (idx.empty() ? "" : ", ") + t + " value";
                    open(head + ")");
                    write_hoists(am->body.index() == 0 ? std::get<Node*>(am->body) : nullptr);
                    body(am->body);
                    close();
                    returns_.pop_back();
                }
            }
        } catch (const Declined& dec) {
            gaps_.push_back({where_, dec.kind, dec.why});
            out_.resize(mark);   // nothing of a member that was declined stays in the text
            indent_ = indent;
            returns_.clear();
        }
    }
    where_ = c.cname;
    try {
        if (cc_has_field_inits(d)) write_field_init(c);
        if (!cc_has_ctor(d) && !c.is_interface && cc_has_field_inits(d)) {
            open(template_head(c) + self + "::" + c.cname + "()");
            line("this->cs_init_fields();");
            close();
        }
        if (c.is_record) write_record_members(c);
    } catch (const Declined& dec) {
        gaps_.push_back({where_, dec.kind, dec.why});
    }
    line("");
}

void Writer::write_method_body(const ClassInfo& c, const ir::MethodDecl& m, const std::string& self) {
    boxed_.clear();
    hoisted_.clear();
    returns_.push_back(m.ret);
    const std::vector<Node*> list = flat(m.body);
    Node* root = m.body.index() == 0 ? std::get<Node*>(m.body) : nullptr;
    const std::size_t mark = out_.size();
    const int indent = indent_;
    try {
        collect_captures(root);
        for (Node* s : list) collect_captures(s);
        const bool is_static_init = m.kind.value_or("") == "static_init";
        std::string head = template_head(c, m.typeparams);
        head += (is_static_init ? "void" : type(m.ret)) + " " + self + "::" + (is_static_init ? "cs_static_init" : ident(m.name.value())) + "(" + params_text(m.params, m.sym) + ")";
        open(head);
        // An iterator or async method returns at once with an object that holds its body, which runs on a coroutine (csrt.hpp, cs::Coroutine) as
        // far as the consumer or the awaited task lets it: the body is a lambda that has copies of the parameters and the object.
        const bool iterator = std::ranges::find(m.mods, "iterator") != m.mods.end();
        const bool is_async = std::ranges::find(m.mods, "async") != m.mods.end();
        // the body outlives the call, so it holds the object it runs on (`this` is only a raw pointer)
        const bool instance = !(m.sym && m.sym->is_static) && std::ranges::find(m.mods, "static") == m.mods.end() && !c.is_struct;
        const std::string keep = instance ? ", cs_self = this->shared_from_this()" : "";
        TypeP element;
        if (iterator) {
            const auto* seq = class_of(m.ret);
            if (seq == nullptr || seq->args.size() != 1 || seq->name != "System.Collections.Generic.IEnumerable") {
                decline("iterator", "an iterator method whose result is not IEnumerable<T>");
            }
            element = seq->args[0];
            yields_.push_back(element);
            returns_.back() = ir::void_type();
            line("return cs::generate<" + type(element) + ">([=" + keep + "](cs::Yielder<" + type(element) + ">& cs_y) mutable {");
            ++indent_;
        } else if (is_async) {
            const auto* task = class_of(m.ret);
            if (task == nullptr || task->name != "System.Threading.Tasks.Task") decline("async", "an async method whose result is not a Task");
            const TypeP result = task->args.size() == 1 ? task->args[0] : ir::void_type();
            returns_.back() = result;
            const std::string rt = std::holds_alternative<ir::VoidType>(result->body) ? "void" : type(result);
            line(std::string("return cs::run_async<") + (std::holds_alternative<ir::VoidType>(result->body) ? "void" : rt) + ">([=" + keep + "]() mutable -> " + rt + " {");
            ++indent_;
        }
        for (Node* p : m.params) {
            const auto* pa = ir::get<ir::Param>(p);
            if (boxed_.contains(sym_key(*pa->sym))) line("auto " + ident(pa->sym->name) + " = std::make_shared<" + type(pa->type) + ">(std::move(" + ident(pa->sym->name) + "__in));");
        }
        for (Node* s : list) write_hoists(s);
        body(m.body);
        if (iterator || is_async) {
            --indent_;
            line("});");
        }
        if (iterator) yields_.pop_back();
        close();
        line("");
    } catch (...) {
        out_.resize(mark);   // nothing of a member that was declined stays in the text
        indent_ = indent;
        if (std::ranges::find(m.mods, "iterator") != m.mods.end() && !yields_.empty()) yields_.pop_back();
        returns_.pop_back();
        throw;
    }
    returns_.pop_back();
}

void Writer::write_ctor_body(const ClassInfo& c, const ir::MethodDecl& m) {
    boxed_.clear();
    hoisted_.clear();
    const std::string self = self_type(c);
    returns_.push_back(ir::void_type());
    const std::vector<Node*> list = flat(m.body);
    const std::size_t mark = out_.size();
    const int indent = indent_;
    try {
        for (Node* s : list) collect_captures(s);
        std::string init;
        bool chained = false;
        std::size_t first = 0;
        if (!list.empty()) {
            if (const auto* cc = ir::get<ir::CtorCall>(list[0])) {
                first = 1;
                const std::string kind = cc->kind.value_or("");
                const std::string args = args_text(cc->args, cc->sym);
                if (kind.starts_with("this")) {
                    init = " : " + c.cname + "(" + args + ")";
                    chained = true;
                } else if (const auto* ext = class_of(c.decl->extends); ext != nullptr && ext->name != "System.Object" && ext->name != "System.ValueType" && !c.is_struct) {
                    if (!args.empty() || kind == "super") init = " : " + class_text(*ext) + "(" + args + ")";
                }
            }
        }
        const std::string head = template_head(c) + self + "::" + c.cname + "(" + params_text(m.params, m.sym) + ")" + init;
        open(head);
        for (Node* p : m.params) {
            const auto* pa = ir::get<ir::Param>(p);
            if (boxed_.contains(sym_key(*pa->sym))) line("auto " + ident(pa->sym->name) + " = std::make_shared<" + type(pa->type) + ">(std::move(" + ident(pa->sym->name) + "__in));");
        }
        if (!chained && cc_has_field_inits(*c.decl)) line("this->cs_init_fields();");
        if (cc_has(m.mods, "primary")) {
            for (Node* p : m.params) {
                const auto* pa = ir::get<ir::Param>(p);
                line("this->bk_" + pa->sym->name + " = " + ident(pa->sym->name) + ";");
            }
        }
        for (std::size_t i = first; i < list.size(); ++i) write_hoists(list[i]);
        stmts(std::vector<Node*>(list.begin() + static_cast<std::ptrdiff_t>(first), list.end()));
        close();
        line("");
    } catch (...) {
        out_.resize(mark);
        indent_ = indent;
        returns_.pop_back();
        throw;
    }
    returns_.pop_back();
}

void Writer::write_field_init(const ClassInfo& c) {
    boxed_.clear();
    hoisted_.clear();
    open(template_head(c) + "void " + self_type(c) + "::cs_init_fields()");
    for (Node* m : c.decl->members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) {
            if (f->init == nullptr || cc_static(f->mods, f->sym) || cc_has(f->mods, "const")) continue;
            collect_captures(f->init);
            line("this->" + ident(f->sym->name) + " = " + conv(f->init, f->type) + ";");
        } else if (const auto* p = ir::get<ir::Property>(m)) {
            if (p->init == nullptr || p->sym->is_static) continue;
            collect_captures(p->init);
            line("this->bk_" + p->sym->name + " = " + conv(p->init, p->type) + ";");
        }
    }
    close();
}

// The value semantics of a record: its properties printed, compared and hashed.
void Writer::write_record_members(const ClassInfo& c) {
    std::vector<std::pair<std::string, TypeP>> fields;   // base records' first
    std::vector<const ClassInfo*> chain;
    for (const ClassInfo* k = &c; k != nullptr;) {
        chain.push_back(k);
        const auto* ext = class_of(k->decl->extends);
        k = ext != nullptr ? info(ext->name) : nullptr;
    }
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        for (Node* m : (*it)->decl->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md == nullptr || md->kind.value_or("") != "ctor" || !cc_has(md->mods, "primary")) continue;
            for (Node* p : md->params) {
                const auto* pa = ir::get<ir::Param>(p);
                fields.emplace_back(pa->sym->name, pa->type);
            }
        }
    }
    const std::string self = self_type(c);
    const std::string head = template_head(c);
    open(head + "cs::String " + self + "::ToString()");
    if (fields.empty()) {
        line("return cs::String(" + u16_literal_text(c.cname + " { }") + ");");
    } else {
        std::string parts = "cs::String(" + u16_literal_text(c.cname + " { ") + ")";
        for (std::size_t i = 0; i < fields.size(); ++i) {
            parts += ", cs::String(" + u16_literal_text((i ? ", " : "") + fields[i].first + " = ") + "), this->bk_" + fields[i].first;
        }
        line("return cs::cat(" + parts + ", cs::String(u\" }\"));");
    }
    close();
    open(head + "bool " + self + "::Equals(const cs::Ref<cs::Object>& other)");
    line("const auto o = std::dynamic_pointer_cast<" + self + ">(other);");
    line("if (!o || typeid(*o) != typeid(*this)) return false;");
    std::string cmp = "true";
    for (const auto& [name, t] : fields) cmp += " && this->bk_" + name + " == o->bk_" + name;
    line("return " + cmp + ";");
    close();
    open(head + "int " + self + "::GetHashCode()");
    line("int h = 17;");
    for (const auto& [name, t] : fields) {
        if (is_string(t)) line("h = h * 31 + this->bk_" + name + ".GetHashCode();");
        else if (is_numeric(t) || (t && t->is_prim())) line("h = h * 31 + static_cast<int>(std::hash<" + type(t) + ">()(this->bk_" + name + "));");
        else line("h = h * 31 + (this->bk_" + name + " ? this->bk_" + name + "->GetHashCode() : 0);");
    }
    line("return h;");
    close();
    open(head + "bool " + self + "::op_Equality(const cs::Ref<" + self + ">& a, const cs::Ref<" + self + ">& b)");
    line("if (a == b) return true;");
    line("if (!a || !b) return false;");
    line("return a->Equals(b);");
    close();
    open(head + "bool " + self + "::op_Inequality(const cs::Ref<" + self + ">& a, const cs::Ref<" + self + ">& b)");
    line("return !op_Equality(a, b);");
    close();
}

void Writer::write_statics(const ClassInfo& c) {
    where_ = c.cname;
    member_names_.clear();
    for (Node* m : c.decl->members) {
        try {
            if (const auto* f = ir::get<ir::FieldDecl>(m)) {
                if (!cc_static(f->mods, f->sym) || cc_has(f->mods, "const")) continue;
                where_ = c.cname + "." + f->sym->name;
                boxed_.clear();
                collect_captures(f->init);
                const std::string t = type(f->type);
                line(t + " " + c.cname + "::" + ident(f->sym->name) + (f->init != nullptr ? " = " + conv(f->init, f->type) : "{}") + ";");
            } else if (const auto* p = ir::get<ir::Property>(m)) {
                if (!p->sym->is_static || !prop_of(p->sym)->is_auto) continue;
                where_ = c.cname + "." + p->sym->name;
                collect_captures(p->init);
                line(type(p->type) + " " + c.cname + "::bk_" + p->sym->name + (p->init != nullptr ? " = " + conv(p->init, p->type) : "{}") + ";");
            }
        } catch (const Declined& dec) {
            gaps_.push_back({where_, dec.kind, dec.why});
        }
    }
}

}  // namespace cb::translate::backends::csharp_cpp17::detail
