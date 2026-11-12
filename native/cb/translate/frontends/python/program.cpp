#include "cb/translate/frontends/python/program.hpp"

#include <algorithm>
#include <deque>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::frontends::python::program {

namespace pt = pytypes;

namespace {

using AnyNode = pyast::AnyNode;

// The standard-library modules the front end knows by name (top-level names).
const std::unordered_set<std::string>& program_stdlib_modules() {
    static const std::unordered_set<std::string> names = [] {
        const std::string_view words =
            "abc argparse array ast base64 binascii bisect builtins bz2 calendar codecs collections "
            "concurrent configparser contextlib copy csv ctypes dataclasses datetime decimal difflib dis email enum errno "
            "fnmatch fractions functools gc getpass glob gzip hashlib heapq hmac html http importlib inspect io ipaddress "
            "itertools json keyword locale logging lzma math mimetypes mmap multiprocessing numbers operator os pathlib "
            "pickle platform pprint queue random re reprlib secrets select shlex shutil signal socket sqlite3 ssl stat "
            "statistics string struct subprocess sys sysconfig tarfile tempfile textwrap threading time timeit tkinter "
            "token tokenize traceback types typing unicodedata unittest urllib uuid warnings weakref winreg xml zipfile "
            "zlib zipimport __future__ msvcrt _thread";
        std::unordered_set<std::string> out;
        for (const std::string_view w : text::split_ws(words)) out.emplace(w);
        return out;
    }();
    return names;
}

const std::unordered_set<std::string>& program_builtin_exceptions() {
    static const std::unordered_set<std::string> names = [] {
        const std::string_view words =
            "Exception BaseException ValueError KeyError IndexError TypeError RuntimeError "
            "OSError IOError LookupError ArithmeticError ZeroDivisionError OverflowError NotImplementedError "
            "AssertionError StopIteration EOFError FileNotFoundError AttributeError UnicodeDecodeError "
            "UnicodeEncodeError PermissionError TimeoutError";
        std::unordered_set<std::string> out;
        for (const std::string_view w : text::split_ws(words)) out.emplace(w);
        return out;
    }();
    return names;
}

// The last name of a decorator: `@name`, `@mod.name`, `@name(args)`; "?" for any other form.
std::string program_decorator_name(const syntax::Expr* d) {
    if (const auto* n = syntax::as<syntax::Name>(d)) return n->id;
    if (const auto* a = syntax::as<syntax::Attribute>(d)) return a->attr;
    if (const auto* c = syntax::as<syntax::Call>(d)) return program_decorator_name(c->func);
    return "?";
}

// The names an assignment target binds (a name, or the names inside a tuple/list target).
void program_target_names(const syntax::Expr* target, std::vector<std::string>& out) {
    if (const auto* n = syntax::as<syntax::Name>(target)) {
        out.push_back(n->id);
    } else if (const auto* t = syntax::as<syntax::Tuple>(target)) {
        for (const syntax::Expr* e : t->elts) program_target_names(e, out);
    } else if (const auto* l = syntax::as<syntax::List>(target)) {
        for (const syntax::Expr* e : l->elts) program_target_names(e, out);
    }
}

std::string program_last_part(const std::string& qualified) {
    const std::size_t dot = qualified.rfind('.');
    return dot == std::string::npos ? qualified : qualified.substr(dot + 1);
}

// The parameters a function indexes a sequence with in its first statement, before anything else can happen: once a call
// returns, the argument is within the sequence's bounds.
StringSet program_indexing_params(const Fn& fn) {
    const syntax::Stmt* first = nullptr;
    for (const syntax::Stmt* s : fn.node->body) {
        const auto* e = syntax::as<syntax::ExprStmt>(s);
        if (e != nullptr && syntax::as<syntax::Constant>(e->value) != nullptr) continue;   // the docstring
        first = s;
        break;
    }
    if (first == nullptr) return {};
    if (first->kind != syntax::StmtKind::Return && first->kind != syntax::StmtKind::Assign &&
        first->kind != syntax::StmtKind::Expr) {
        return {};
    }
    StringSet found;
    for (const AnyNode& node : pyast::walk(first)) {
        const syntax::Expr* e = node.expr();
        if (e == nullptr) continue;
        if (const auto* sub = syntax::as<syntax::Subscript>(e)) {
            if (const auto* name = syntax::as<syntax::Name>(sub->slice)) found.insert(name->id);
        } else if (const auto* call = syntax::as<syntax::Call>(e)) {
            const auto* callee = syntax::as<syntax::Attribute>(call->func);
            if (callee != nullptr && callee->attr == "unpack_from" && call->args.size() >= 3) {
                if (const auto* name = syntax::as<syntax::Name>(call->args[2])) found.insert(name->id);
            }
        }
    }
    StringSet result;
    for (const std::string& p : fn.params) {
        if (found.contains(p)) result.insert(p);
    }
    return result;
}

// The parameters a function is ready to get None for (Fn::none_aware).
StringSet program_none_aware(const Fn& fn) {
    StringSet out;
    for (const auto& [name, value] : fn.defaults) {
        if (pyast::is_none_constant(value)) out.insert(name);
    }
    StringSet params(fn.params.begin(), fn.params.end());
    params.insert(fn.kwonly.begin(), fn.kwonly.end());
    const auto param_name = [&](const syntax::Expr* x) -> const std::string* {
        const auto* n = syntax::as<syntax::Name>(x);
        return n != nullptr && params.contains(n->id) ? &n->id : nullptr;
    };
    for (const AnyNode& node : pyast::walk(static_cast<const syntax::Stmt*>(fn.node))) {
        if (const syntax::Expr* e = node.expr()) {
            if (const auto* cmp = syntax::as<syntax::Compare>(e)) {
                std::vector<const syntax::Expr*> operands{cmp->left};
                operands.insert(operands.end(), cmp->comparators.begin(), cmp->comparators.end());
                const bool has_none = std::ranges::any_of(operands, [](const syntax::Expr* x) { return pyast::is_none_constant(x); });
                if (has_none) {
                    for (const syntax::Expr* x : operands) {
                        if (const std::string* n = param_name(x)) out.insert(*n);
                    }
                }
            } else if (const auto* cond = syntax::as<syntax::IfExp>(e)) {
                if (const std::string* n = param_name(cond->test)) out.insert(*n);
            } else if (const auto* un = syntax::as<syntax::UnaryOp>(e)) {
                if (un->op == syntax::UnaryOpKind::Not) {
                    if (const std::string* n = param_name(un->operand)) out.insert(*n);
                }
            } else if (const auto* bo = syntax::as<syntax::BoolOp>(e)) {
                for (const syntax::Expr* v : bo->values) {
                    if (const std::string* n = param_name(v)) out.insert(*n);
                }
            }
        } else if (const syntax::Stmt* s = node.stmt()) {
            const syntax::Expr* test = nullptr;
            if (const auto* i = syntax::as<syntax::If>(s)) test = i->test;
            else if (const auto* w = syntax::as<syntax::While>(s)) test = w->test;
            else if (const auto* a = syntax::as<syntax::Assert>(s)) test = a->test;
            if (const std::string* n = param_name(test)) out.insert(*n);
        }
    }
    return out;
}

// The short name a type is mixed under in a refusal message: a container's kind, an instance's class, else the kind.
std::string program_type_word(const Type& t) {
    if (t.kind == pt::Kind::Inst && t.name) return program_last_part(*t.name);
    return std::string(pt::kind_word(t.kind));
}

Type program_container_type(pt::Kind kind, SiteId site) {
    Type t;
    t.kind = kind;
    t.site = site;
    return t;
}

Type program_inst_type(std::string name) {
    Type t;
    t.kind = pt::Kind::Inst;
    t.name = std::move(name);
    return t;
}

pt::Kind program_kind_of(Container c) {
    switch (c) {
        case Container::List: return pt::Kind::List;
        case Container::Dict: return pt::Kind::Dict;
        case Container::Set: return pt::Kind::Set;
    }
    return pt::Kind::List;
}

bool program_meets(const FactSet& a, const FactSet& b) {
    const FactSet& small = a.size() <= b.size() ? a : b;
    const FactSet& large = a.size() <= b.size() ? b : a;
    return std::ranges::any_of(small, [&](const FactKey& k) { return large.contains(k); });
}

}  // namespace

bool is_builtin_exception(std::string_view name) { return program_builtin_exceptions().contains(std::string(name)); }
bool is_stdlib_module(std::string_view top) { return program_stdlib_modules().contains(std::string(top)); }

std::string_view fn_kind_word(FnKind kind) {
    switch (kind) {
        case FnKind::Function: return "function";
        case FnKind::Method: return "method";
        case FnKind::Static: return "static";
        case FnKind::ClassMethod: return "classmethod";
        case FnKind::Property: return "property";
        case FnKind::Async: return "async";
    }
    return "function";
}

std::string_view container_word(Container kind) {
    switch (kind) {
        case Container::List: return "list";
        case Container::Dict: return "dict";
        case Container::Set: return "set";
    }
    return "list";
}

std::size_t FactKeyHash::operator()(const FactKey& k) const {
    std::size_t h = std::hash<std::string>{}(k.name);
    h = h * 1000003u ^ static_cast<std::size_t>(k.site);
    return h * 31u + static_cast<std::size_t>(k.tag);
}

// ------------------------------------------------------------------------------------ Fn / Class / Module

Fn::Fn(std::string qualified_name, const syntax::FunctionDef& def, Module& owner, Class* owner_class)
    : qname(std::move(qualified_name)), node(&def), module(&owner), cls(owner_class), name(def.name) {
    for (const syntax::Expr* d : def.decorator_list) decorators.push_back(program_decorator_name(d));
    const auto has_decorator = [&](std::string_view n) { return std::ranges::find(decorators, n) != decorators.end(); };
    if (cls != nullptr) {
        kind = FnKind::Method;
        if (has_decorator("staticmethod")) kind = FnKind::Static;
        else if (has_decorator("classmethod")) kind = FnKind::ClassMethod;
        else if (has_decorator("property")) kind = FnKind::Property;
    }
    const syntax::arguments& a = *def.args;
    for (const syntax::arg* x : a.posonlyargs) params.push_back(x->arg);
    for (const syntax::arg* x : a.args) params.push_back(x->arg);
    for (const syntax::arg* x : a.kwonlyargs) kwonly.push_back(x->arg);
    const auto annotate = [&](const std::vector<syntax::arg*>& group) {
        for (const syntax::arg* x : group) {
            if (x->annotation != nullptr) annotations.insert_or_assign(x->arg, x->annotation);
        }
    };
    annotate(a.posonlyargs);
    annotate(a.args);
    annotate(a.kwonlyargs);
    // the defaults belong to the last parameters
    const std::size_t with_default = a.defaults.size();
    for (std::size_t i = 0; i < with_default && i < params.size(); ++i) {
        defaults.insert_or_assign(params[params.size() - with_default + i], a.defaults[i]);
    }
    for (std::size_t i = 0; i < a.kwonlyargs.size() && i < a.kw_defaults.size(); ++i) {
        if (a.kw_defaults[i] != nullptr) defaults.insert_or_assign(a.kwonlyargs[i]->arg, a.kw_defaults[i]);
    }
    if (a.vararg != nullptr) varargs = a.vararg->arg;
    if (a.kwarg != nullptr) kwargs = a.kwarg->arg;
    const int end = def.pos.end_lineno != 0 ? def.pos.end_lineno : def.pos.lineno;
    lines = static_cast<std::size_t>(end - def.pos.lineno + 1);
}

std::vector<std::string> Fn::bound() const {
    const bool has_receiver = kind == FnKind::Method || kind == FnKind::Property || kind == FnKind::ClassMethod;
    if (has_receiver && !params.empty()) return {params.begin() + 1, params.end()};
    return params;
}

Class::Class(std::string qualified_name, const syntax::ClassDef& def, Module& owner)
    : qname(std::move(qualified_name)), node(&def), module(&owner), name(def.name) {
    for (const syntax::Expr* b : def.bases) base_nodes.push_back(b);
    for (const syntax::Expr* d : def.decorator_list) decorators.push_back(program_decorator_name(d));
}

Module::Module(std::string module_name, std::filesystem::path file, std::string text)
    : name(std::move(module_name)), path(std::move(file)), source(std::move(text)) {
    syntax::ParseResult parsed = syntax::parse(source, path.empty() ? name : path.generic_string());
    if (!parsed) {
        const syntax::SyntaxError err = parsed.error.value_or(syntax::SyntaxError{});
        throw ParseError(path.empty() ? name : path.generic_string(), static_cast<std::uint32_t>(err.lineno),
                         static_cast<std::uint32_t>(err.offset), err.msg.empty() ? "invalid syntax" : err.msg);
    }
    tree = std::move(parsed.module);
    is_package = path.filename() == "__init__.py";
}

bool Module::has_docstring() const {
    if (tree->body.empty()) return false;
    const auto* first = syntax::as<syntax::ExprStmt>(tree->body.front());
    return first != nullptr && pyast::string_constant(first->value).has_value();
}

// ------------------------------------------------------------------------------------ loading

Module& Program::add_module(const std::string& name, std::string source, const std::filesystem::path& path) {
    auto module = std::make_unique<Module>(name, path, std::move(source));
    Module& ref = *module;
    modules.insert_or_assign(name, std::move(module));
    return ref;
}

Module& Program::add_file(const std::string& name, const std::filesystem::path& path) {
    return add_module(name, fs::read_text(path), path);
}

std::vector<std::filesystem::path> Program::add_tree(const std::filesystem::path& root, const std::string& package) {
    std::vector<std::filesystem::path> skipped;
    if (!std::filesystem::is_directory(root)) throw NotFoundError(root, 0, "not a folder");
    // A folder's files before its subfolders, each in name order (what os.walk with sorted names gives).
    const auto visit = [&](const auto& self, const std::filesystem::path& dir) -> void {
        std::vector<std::filesystem::path> files;
        std::vector<std::filesystem::path> folders;
        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            std::error_code entry_ec;
            if (entry.is_directory(entry_ec)) {
                if (entry.path().filename() != "__pycache__") folders.push_back(entry.path());
            } else if (entry.path().extension() == ".py") {
                files.push_back(entry.path());
            }
        }
        const auto by_name = [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return a.filename().generic_string() < b.filename().generic_string();
        };
        if (ec) throw IoError(dir, ec.value(), ec.message());
        std::ranges::sort(files, by_name);
        std::ranges::sort(folders, by_name);
        for (const std::filesystem::path& file : files) {
            std::string rel = file.lexically_relative(root).generic_string();
            rel.resize(rel.size() - 3);   // ".py"
            std::ranges::replace(rel, '/', '.');
            if (rel.ends_with("__init__")) {
                rel.resize(rel.size() - 8);
                while (!rel.empty() && rel.back() == '.') rel.pop_back();
            }
            const std::string name = package + (rel.empty() ? std::string() : "." + rel);
            try {
                add_file(name, file);
            } catch (const ParseError&) {
                skipped.push_back(file);
            }
        }
        for (const std::filesystem::path& folder : folders) self(self, folder);
    };
    visit(visit, root);
    return skipped;
}

void Program::index_stmt(Module& module, const syntax::Stmt& stmt) {
    using K = syntax::StmtKind;
    switch (stmt.kind) {
        case K::FunctionDef:
        case K::AsyncFunctionDef: {
            const auto& def = static_cast<const syntax::FunctionDef&>(stmt);
            auto fn = std::make_unique<Fn>(module.name + "." + def.name, def, module);
            if (stmt.kind == K::AsyncFunctionDef) fn->kind = FnKind::Async;
            module.functions.insert_or_assign(def.name, fn.get());
            const std::string qname = fn->qname;
            fns.insert_or_assign(qname, std::move(fn));
            break;
        }
        case K::ClassDef: {
            const auto& def = static_cast<const syntax::ClassDef&>(stmt);
            auto cls = std::make_unique<Class>(module.name + "." + def.name, def, module);
            Class& c = *cls;
            module.classes.insert_or_assign(def.name, &c);
            classes.insert_or_assign(c.qname, std::move(cls));
            for (const syntax::Stmt* b : def.body) {
                if (b->kind == K::FunctionDef || b->kind == K::AsyncFunctionDef) {
                    const auto& method = static_cast<const syntax::FunctionDef&>(*b);
                    auto fn = std::make_unique<Fn>(c.qname + "." + method.name, method, module, &c);
                    c.methods.insert_or_assign(method.name, fn.get());
                    const std::string qname = fn->qname;
                    fns.insert_or_assign(qname, std::move(fn));
                } else if (const auto* assign = syntax::as<syntax::Assign>(b)) {
                    if (assign->targets.size() == 1) {
                        if (const auto* target = syntax::as<syntax::Name>(assign->targets[0])) {
                            c.class_attrs.insert_or_assign(target->id, assign->value);
                        }
                    }
                } else if (const auto* ann = syntax::as<syntax::AnnAssign>(b)) {
                    if (const auto* target = syntax::as<syntax::Name>(ann->target); target != nullptr && ann->value != nullptr) {
                        c.class_attrs.insert_or_assign(target->id, ann->value);
                    }
                }
            }
            break;
        }
        case K::Import:
            for (const syntax::alias* a : static_cast<const syntax::Import&>(stmt).names) {
                if (a->has_asname && !a->asname.empty()) {
                    module.imports.insert_or_assign(a->asname, module_ref(a->name));
                } else {
                    const std::string top = a->name.substr(0, a->name.find('.'));
                    module.imports.insert_or_assign(top, module_ref(top));
                }
            }
            break;
        case K::ImportFrom: {
            const auto& from = static_cast<const syntax::ImportFrom&>(stmt);
            const std::string base = from_base(module, from);
            for (const syntax::alias* a : from.names) {
                if (a->name == "*") continue;
                const std::string local = a->has_asname && !a->asname.empty() ? a->asname : a->name;
                const std::string full = base.empty() ? a->name : base + "." + a->name;
                if (modules.contains(full)) {
                    module.imports.insert_or_assign(local, Ref{Ref::Kind::Module, full, {}});
                } else if (modules.contains(base)) {
                    module.imports.insert_or_assign(local, Ref{Ref::Kind::Member, base, a->name});
                } else {
                    module.imports.insert_or_assign(local, Ref{Ref::Kind::Extern, full, {}});
                }
            }
            break;
        }
        case K::Assign: {
            const auto& assign = static_cast<const syntax::Assign&>(stmt);
            for (const syntax::Expr* target : assign.targets) {
                std::vector<std::string> names;
                program_target_names(target, names);
                for (const std::string& n : names) {
                    // an unpacking has no single value
                    module.assigns[n].push_back(syntax::as<syntax::Name>(target) != nullptr ? assign.value : nullptr);
                }
            }
            break;
        }
        case K::AnnAssign: {
            const auto& ann = static_cast<const syntax::AnnAssign&>(stmt);
            if (const auto* target = syntax::as<syntax::Name>(ann.target)) {
                module.assigns[target->id].push_back(ann.value);
            } else {
                module.statements.push_back(&stmt);
            }
            break;
        }
        case K::If: {
            // `if TYPE_CHECKING:` -- index what it defines
            const auto& cond = static_cast<const syntax::If&>(stmt);
            for (const syntax::Stmt* s : cond.body) index_stmt(module, *s);
            for (const syntax::Stmt* s : cond.orelse) index_stmt(module, *s);
            module.statements.push_back(&stmt);
            break;
        }
        case K::Try: {
            // `try: import x  except ImportError:` -- index what it defines
            const auto& attempt = static_cast<const syntax::Try&>(stmt);
            for (const syntax::Stmt* s : attempt.body) index_stmt(module, *s);
            for (const syntax::Stmt* s : attempt.orelse) index_stmt(module, *s);
            for (const syntax::ExceptHandler* h : attempt.handlers) {
                for (const syntax::Stmt* s : h->body) index_stmt(module, *s);
            }
            module.statements.push_back(&stmt);
            break;
        }
        case K::Expr:
            // a docstring (or any other constant on a line of its own): nothing to index
            if (syntax::as<syntax::Constant>(static_cast<const syntax::ExprStmt&>(stmt).value) == nullptr) {
                module.statements.push_back(&stmt);
            }
            break;
        default: module.statements.push_back(&stmt); break;
    }
}

Ref Program::module_ref(const std::string& name) const {
    if (modules.contains(name)) return {Ref::Kind::Module, name, {}};
    const std::string top = name.substr(0, name.find('.'));
    return {is_stdlib_module(top) ? Ref::Kind::StdModule : Ref::Kind::Extern, name, {}};
}

// The module a relative import `from ..x import y` names: the importing module's name, `level` parts up, then `x`.
std::string Program::from_base(const Module& module, const syntax::ImportFrom& stmt) const {
    if (stmt.level == 0) return stmt.module;
    std::vector<std::string_view> parts = text::split(module.name, '.');
    // `parts[:keep]`: a negative count drops that many parts from the end (down to none)
    const long size = static_cast<long>(parts.size());
    const long keep = size - stmt.level + (module.is_package ? 1 : 0);
    const long kept = keep >= 0 ? std::min(keep, size) : std::max(0L, size + keep);
    parts.resize(static_cast<std::size_t>(kept));
    std::string base = text::join(parts, ".");
    if (!stmt.module.empty()) base += "." + stmt.module;
    return base;
}

void Program::index() {
    for (auto& [name, module] : modules) {
        for (const syntax::Stmt* stmt : module->tree->body) index_stmt(*module, *stmt);
    }
    for (auto& [qname, cls] : classes) {
        for (const syntax::Expr* base : cls->base_nodes) {
            const std::optional<Ref> resolved = resolve_expr_name(*cls->module, base);
            const auto* name = syntax::as<syntax::Name>(base);
            if (resolved && resolved->kind == Ref::Kind::Class) {
                cls->bases.push_back(resolved->name);
                classes.at(resolved->name)->subclasses.push_back(cls->qname);
            } else if (name != nullptr && is_builtin_exception(name->id)) {
                cls->exception = name->id;
            } else if (name != nullptr && name->id == "object") {
                // no base to record
            } else {
                cls->foreign_base = pyast::describe_expr(base);
            }
        }
    }
    // exceptions derived from exceptions
    for (auto& [qname, cls] : classes) {
        const Class* k = cls.get();
        std::unordered_set<const Class*> seen;
        while (!k->exception && !k->bases.empty() && seen.insert(k).second) k = classes.at(k->bases[0]).get();
        if (!cls->exception) cls->exception = k->exception;
    }
    for (auto& [qname, fn] : fns) {
        index_params[qname] = program_indexing_params(*fn);
        fn->none_aware = program_none_aware(*fn);
    }
}

// ------------------------------------------------------------------------------------ type lookups

Type Program::annotation_type(const Fn& fn, const std::string& param) {
    const syntax::Expr* const* node = fn.annotations.find(param);
    return node == nullptr ? pt::bottom() : ann_to_type(*fn.module, *node);
}

Type Program::ann_to_type(Module& module, const syntax::Expr* annotation) {
    if (const auto it = ann_cache_.find(annotation); it != ann_cache_.end()) return it->second;
    Type t = annotation_(module, annotation);
    ann_cache_.emplace(annotation, t);
    return t;
}

Type Program::annotation_(Module& module, const syntax::Expr* a) {
    const auto origin = [&](const syntax::Expr* x) { return Origin{module.name, static_cast<std::uint32_t>(x->pos.lineno)}; };
    if (const auto quoted = pyast::string_constant(a)) {
        // a forward reference: the string is an expression. A string that is not one has no type.
        syntax::ParseResult parsed = syntax::parse(*quoted, "<annotation>");
        if (!parsed || parsed.module->body.size() != 1 || parsed.module->body[0]->kind != syntax::StmtKind::Expr) {
            return pt::bottom();
        }
        const syntax::Expr* inner = syntax::cast<syntax::ExprStmt>(parsed.module->body[0])->value;
        annotation_trees_.push_back(std::move(parsed.module));   // the tree must outlive the sites made from it
        return annotation_(module, inner);
    }
    if (pyast::is_none_constant(a)) return pt::none();
    const std::string* name = nullptr;
    if (const auto* n = syntax::as<syntax::Name>(a)) name = &n->id;
    else if (const auto* attr = syntax::as<syntax::Attribute>(a)) name = &attr->attr;
    if (name != nullptr) {
        if (*name == "int") return pt::int_range();
        if (*name == "str") return pt::str();
        if (*name == "bytes") return pt::bytes();
        if (*name == "bytearray") return pt::bytearray();
        if (*name == "bool") return pt::boolean();
        if (*name == "float") return pt::floating();
        if (*name == "None") return pt::none();
        if (*name == "list" || *name == "List" || *name == "dict" || *name == "Dict" || *name == "set" || *name == "Set") {
            const Container kind = *name == "list" || *name == "List" ? Container::List
                                   : *name == "dict" || *name == "Dict" ? Container::Dict
                                                                         : Container::Set;
            const SiteId s = new_site(a, kind, origin(a));
            if (kind == Container::Dict) make_dynamic(s, "annotated as a dict");
            return program_container_type(program_kind_of(kind), s);
        }
        if (syntax::as<syntax::Name>(a) != nullptr) {
            const std::optional<Ref> r = resolve_global(module, *name);
            if (r && r->kind == Ref::Kind::Class) return program_inst_type(r->name);
        }
        return pt::bottom();
    }
    if (const auto* bin = syntax::as<syntax::BinOp>(a); bin != nullptr && bin->op == syntax::OpKind::BitOr) {
        const Type left = annotation_(module, bin->left);
        return join(left, annotation_(module, bin->right));
    }
    if (const auto* sub = syntax::as<syntax::Subscript>(a)) {
        std::string head;
        if (const auto* n = syntax::as<syntax::Name>(sub->value)) head = n->id;
        else if (const auto* attr = syntax::as<syntax::Attribute>(sub->value)) head = attr->attr;
        std::vector<const syntax::Expr*> args;
        if (const auto* tuple = syntax::as<syntax::Tuple>(sub->slice)) args.assign(tuple->elts.begin(), tuple->elts.end());
        else args.push_back(sub->slice);
        if (head == "Optional") return pt::optional_of(annotation_(module, args.at(0)));
        if (head == "Union") {
            Type t = pt::bottom();
            for (const syntax::Expr* x : args) t = join(t, annotation_(module, x));
            return t;
        }
        if (head == "list" || head == "List" || head == "set" || head == "Set" || head == "Sequence" || head == "Iterable") {
            const Container kind = head == "set" || head == "Set" ? Container::Set : Container::List;
            const SiteId s = new_site(a, kind, origin(a));
            grow(Table::Elem, s, annotation_(module, args.at(0)));
            return program_container_type(program_kind_of(kind), s);
        }
        if (head == "dict" || head == "Dict" || head == "Mapping") {
            const SiteId s = new_site(a, Container::Dict, origin(a));
            make_dynamic(s, "annotated as a dict");
            if (args.size() == 2) {
                grow(Table::Keys, s, annotation_(module, args[0]));
                grow(Table::Elem, s, annotation_(module, args[1]));
            }
            return program_container_type(pt::Kind::Dict, s);
        }
        if (head == "tuple" || head == "Tuple") {
            if (std::ranges::any_of(args, [](const syntax::Expr* x) { return pyast::is_ellipsis(x); })) {
                return pt::top("variable-length tuple annotation");
            }
            std::vector<Type> items;
            for (const syntax::Expr* x : args) items.push_back(annotation_(module, x));
            return pt::tuple_of(std::move(items));
        }
    }
    return pt::bottom();
}

Type Program::param_type(const Fn& fn, const std::string& param) {
    read(FactKey::params(fn.qname));
    read(FactKey::defaults());
    Type t = pt::bottom();
    if (const auto it = params.find(fn.qname); it != params.end()) {
        if (const Type* known = it->second.find(param)) t = *known;
    }
    if (fn.annotations.contains(param)) {
        const Type annotated = annotation_type(fn, param);
        if (annotated.kind == pt::Kind::Int) {
            if (t.kind != pt::Kind::Int) t = annotated;
        } else if (annotated.kind != pt::Kind::Bottom) {
            t = annotated;
        }
    }
    if (fn.defaults.contains(param)) {
        if (const auto it = default_types.find({fn.qname, param}); it != default_types.end()) t = join(t, it->second);
    }
    return t;
}

Type Program::global_type(const std::string& module, const std::string& name) {
    read(FactKey::globals(module));
    if (const auto it = globals.find(module); it != globals.end()) {
        if (const Type* t = it->second.find(name)) return *t;
    }
    return pt::bottom();
}

Fn* Program::find_method(const std::string& cls, const std::string& name) {
    for (const std::string& c : mro(cls)) {
        if (Fn* const* method = classes.at(c)->methods.find(name)) return *method;
    }
    return nullptr;
}

const StringSet& Program::assigned_attrs(const std::string& cls) {
    if (const auto it = assigned_attrs_.find(cls); it != assigned_attrs_.end()) return it->second;
    StringSet found;
    for (const auto& [method_name, fn] : classes.at(cls)->methods) {
        if (fn->params.empty() || fn->kind == FnKind::Static) continue;
        const std::string& receiver = fn->params.front();
        for (const AnyNode& node : pyast::walk(static_cast<const syntax::Stmt*>(fn->node))) {
            const auto* attr = syntax::as<syntax::Attribute>(node.expr());
            if (attr == nullptr || attr->ctx != syntax::ExprCtx::Store) continue;
            const auto* base = syntax::as<syntax::Name>(attr->value);
            if (base != nullptr && base->id == receiver) found.insert(attr->attr);
        }
    }
    return assigned_attrs_.emplace(cls, std::move(found)).first->second;
}

StringSet Program::assigned_attrs_any(const std::string& cls) {
    StringSet out = assigned_attrs(cls);
    for (const std::string& d : descendants(cls)) {
        const StringSet& more = assigned_attrs(d);
        out.insert(more.begin(), more.end());
    }
    return out;
}

std::optional<std::string> Program::attr_owner(const std::string& cls, const std::string& attr) {
    if (!classes.contains(cls)) return std::nullopt;
    const std::vector<std::string> chain = mro(cls);
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto known = attrs.find(*it);
        if (classes.at(*it)->class_attrs.contains(attr) || assigned_attrs(*it).contains(attr) ||
            (known != attrs.end() && known->second.contains(attr))) {
            return *it;
        }
    }
    return std::nullopt;
}

std::optional<Type> Program::attr_type(const std::string& cls, const std::string& attr) {
    const std::vector<std::string> chain = mro(cls);
    for (const std::string& c : chain) read(FactKey::attrs(c));
    read(FactKey::class_attrs());
    const std::optional<std::string> owner = attr_owner(cls, attr);
    if (!owner) {
        for (const std::string& d : descendants(cls)) {
            if (assigned_attrs(d).contains(attr)) return pt::top("attribute " + attr + " only set by a subclass");
        }
        return std::nullopt;
    }
    if (const auto it = poisoned.find({*owner, attr}); it != poisoned.end()) {
        return pt::top("untyped:attribute " + attr + " is also stored a value of no single type (by " +
                       program_last_part(it->second) + ")");
    }
    Type t = pt::bottom();
    if (const auto it = attrs.find(*owner); it != attrs.end()) {
        if (const Type* stored = it->second.find(attr)) t = *stored;
    }
    for (const std::string& c : chain) {
        if (classes.at(c)->class_attrs.contains(attr)) {
            if (const auto it = class_attr_types.find({c, attr}); it != class_attr_types.end()) t = join(t, it->second);
        }
    }
    return t;
}

Type Program::class_attr_type(const std::string& cls, const std::string& attr) {
    read(FactKey::class_attrs());
    const auto it = class_attr_types.find({cls, attr});
    return it == class_attr_types.end() ? pt::bottom() : it->second;
}

const std::vector<syntax::Stmt*>* Program::block_of(const syntax::Stmt* stmt) const {
    const auto it = blocks_.find(stmt);
    return it == blocks_.end() ? nullptr : it->second;
}

void Program::map_blocks(const syntax::FunctionDef& fn) {
    for (const AnyNode& node : pyast::walk(static_cast<const syntax::Stmt*>(&fn))) {
        for (const std::vector<syntax::Stmt*>* list : pyast::statement_lists(node)) {
            for (const syntax::Stmt* s : *list) blocks_[s] = list;
        }
    }
}

// ------------------------------------------------------------------------------------ solving

std::size_t Program::solve(Evaluator& evaluator, std::size_t max_rounds, const std::function<void(const std::string&)>& log) {
    for (auto& [qname, fn] : fns) map_blocks(*fn->node);
    std::unordered_map<std::string, FactSet> module_deps;   // module -> the facts its last run read
    std::unordered_map<std::string, FactSet> fn_deps;       // fn -> the facts its last run read
    std::optional<FactSet> previous;                        // the facts the last round changed
    for (std::size_t r = 0; r < max_rounds; ++r) {
        round = static_cast<int>(r);
        dirty_.clear();
        std::size_t ran = 0;
        const auto stale = [&](const std::unordered_map<std::string, FactSet>& deps, const std::string& key) {
            if (!previous) return true;   // the first round runs everything
            const auto it = deps.find(key);
            return it != deps.end() && program_meets(it->second, *previous);
        };
        for (auto& [name, module] : modules) {
            if (!stale(module_deps, name)) continue;
            FactSet reads_now;
            reads_ = &reads_now;
            suspend_ = 0;
            try {
                evaluator.run_module(*this, *module);
            } catch (...) {
                reads_ = nullptr;
                throw;   // a module's top level that cannot be evaluated is not recoverable
            }
            module_deps[name] = std::move(reads_now);
            reads_ = nullptr;
        }
        for (auto& [qname, fn] : fns) {
            if (crashes.contains(qname) || !stale(fn_deps, qname)) continue;
            ++ran;
            FactSet reads_now;
            reads_ = &reads_now;
            suspend_ = 0;
            try {
                evaluator.run_function(*this, *fn);
            } catch (const std::exception& e) {   // a front end bug: the function is refused, the others go on
                crashes.insert_or_assign(qname, e.what());
            }
            fn_deps[qname] = std::move(reads_now);
            reads_ = nullptr;
        }
        if (log) log(std::format("round {}: ran {}, {} facts changed", r, ran, dirty_.size()));
        if (dirty_.empty()) return r + 1;
        previous = dirty_;
        // a merged container's old root is read under its new root from now on
        std::vector<SiteId> changed_sites;
        for (const FactKey& k : *previous) {
            if (k.tag == FactKey::Tag::Site) changed_sites.push_back(k.site);
        }
        for (const SiteId s : changed_sites) previous->insert(FactKey::of_site(find(s)));
    }
    return max_rounds;
}

std::optional<Ref> Program::resolve_global(Module& module, const std::string& name) {
    return resolve_global_(module, name, 0);
}

std::optional<Ref> Program::resolve_global_(Module& module, const std::string& name, int depth) {
    if (const Fn* const* f = module.functions.find(name)) return Ref{Ref::Kind::Function, (*f)->qname, {}};
    if (const Class* const* c = module.classes.find(name)) return Ref{Ref::Kind::Class, (*c)->qname, {}};
    if (module.assigns.contains(name)) return Ref{Ref::Kind::Global, module.name, name};
    const Ref* imported = module.imports.find(name);
    if (imported == nullptr) return std::nullopt;
    if (imported->kind == Ref::Kind::Member) {
        Module& other = *modules.at(imported->name);
        const std::string& member = imported->member;
        if (other.functions.contains(member) || other.classes.contains(member) || other.assigns.contains(member) ||
            other.imports.contains(member)) {
            // import cycles (a imports x from b, b imports x from a) never resolve
            constexpr int kMaxHops = 64;
            if (depth >= kMaxHops) return Ref{Ref::Kind::Missing, imported->name, member};
            return resolve_global_(other, member, depth + 1);
        }
        return Ref{Ref::Kind::Missing, imported->name, member};
    }
    if (imported->kind == Ref::Kind::Extern) {
        if (is_stdlib_module(imported->name.substr(0, imported->name.find('.')))) {
            return Ref{Ref::Kind::StdObject, imported->name, {}};
        }
    }
    return *imported;
}

std::optional<Ref> Program::resolve_expr_name(Module& module, const syntax::Expr* expr) {
    if (const auto* n = syntax::as<syntax::Name>(expr)) return resolve_global(module, n->id);
    if (const auto* a = syntax::as<syntax::Attribute>(expr)) {
        const std::optional<Ref> base = resolve_expr_name(module, a->value);
        if (base && base->kind == Ref::Kind::Module) return resolve_global(*modules.at(base->name), a->attr);
    }
    return std::nullopt;
}

// ------------------------------------------------------------------------------------ containers

SiteId Program::find(SiteId id) {
    SiteId root = id;
    while (sites_.at(root).parent != root) root = sites_[root].parent;
    while (sites_[id].parent != root) {
        const SiteId next = sites_[id].parent;
        sites_[id].parent = root;
        id = next;
    }
    read(FactKey::of_site(root));
    return root;
}

Type Program::settle(const Type& old_type, const Type& new_type, int depth) {
    if (round <= 3 || new_type == old_type) return new_type;
    if (new_type.kind == pt::Kind::Int && old_type.kind == pt::Kind::Int) return pt::widen_int(old_type, new_type);
    if ((new_type.kind == pt::Kind::Tuple || new_type.kind == pt::Kind::Opt) && old_type.kind == new_type.kind &&
        old_type.args.size() == new_type.args.size()) {
        if (depth > 6) return pt::top("a type that keeps growing");
        Type settled;
        settled.kind = new_type.kind;
        for (std::size_t i = 0; i < new_type.args.size(); ++i) {
            settled.args.push_back(settle(old_type.args[i], new_type.args[i], depth + 1));
        }
        return settled;
    }
    return new_type;
}

void Program::touch(const FactKey& key) { dirty_.insert(key); }

void Program::read(const FactKey& key) {
    if (reads_ != nullptr) reads_->insert(key);
}

Type Program::ret(const std::string& fn) {
    read(FactKey::returns(fn));
    const auto it = returns.find(fn);
    return it == returns.end() ? pt::bottom() : it->second;
}

const StringSet& Program::fn_locals(const Fn& fn) {
    if (const auto it = fn_locals_.find(fn.qname); it != fn_locals_.end()) return it->second;
    StringSet names = pyast::bound_names(*fn.node);
    names.insert(fn.params.begin(), fn.params.end());
    names.insert(fn.kwonly.begin(), fn.kwonly.end());
    return fn_locals_.emplace(fn.qname, std::move(names)).first->second;
}

const std::vector<const syntax::Expr*>& Program::access_nodes(AnyNode node) {
    if (const auto it = access_.find(node.node); it != access_.end()) return it->second;
    std::vector<const syntax::Expr*> found;
    std::vector<AnyNode> todo{node};
    std::vector<AnyNode> kids;
    while (!todo.empty()) {
        const AnyNode current = todo.back();
        todo.pop_back();
        if (const syntax::Expr* e = current.expr()) {
            const syntax::ExprKind k = e->kind;
            if (k == syntax::ExprKind::Lambda || k == syntax::ExprKind::ListComp || k == syntax::ExprKind::GeneratorExp ||
                k == syntax::ExprKind::SetComp || k == syntax::ExprKind::DictComp) {
                continue;   // their own scopes
            }
            if (k == syntax::ExprKind::Subscript || k == syntax::ExprKind::Call) found.push_back(e);
        }
        kids.clear();
        pyast::children(current, kids);
        todo.insert(todo.end(), kids.begin(), kids.end());
    }
    std::ranges::reverse(found);
    return access_.emplace(node.node, std::move(found)).first->second;
}

SiteId Program::new_site(const syntax::Node* node, Container kind, std::optional<Origin> where) {
    if (const auto it = site_of_node_.find(node); it != site_of_node_.end()) return it->second;
    const SiteId id = static_cast<SiteId>(sites_.size());
    Site s;
    s.parent = id;
    s.kind = kind;
    s.where = std::move(where);
    s.node = node;
    sites_.push_back(std::move(s));
    site_of_node_.emplace(node, id);
    touch(FactKey::of_site(id));
    return id;
}

SiteId Program::union_sites(SiteId a, SiteId b) {
    a = find(a);
    b = find(b);
    if (a == b) return a;
    if (a > b) std::swap(a, b);
    sites_[b].parent = a;
    touch(FactKey::of_site(a));
    touch(FactKey::of_site(b));
    if (sites_[a].kind != sites_[b].kind) {
        sites_[a].dynamic = std::format("a {} and a {} flow together", container_word(sites_[a].kind), container_word(sites_[b].kind));
    }
    // (the joins may merge more sites, never add one: references into sites_ stay valid)
    Type elem = join(sites_[a].elem, sites_[b].elem);
    sites_[a].elem = std::move(elem);
    Type keys = join(sites_[a].keys, sites_[b].keys);
    sites_[a].keys = std::move(keys);
    for (const auto& [key, type] : std::vector(sites_[b].fields.begin(), sites_[b].fields.end())) {
        const Type* existing = sites_[a].fields.find(key);
        Type merged = join(existing != nullptr ? *existing : pt::bottom(), type);
        sites_[a].fields.insert_or_assign(key, std::move(merged));
    }
    Site& sa = sites_[a];
    Site& sb = sites_[b];
    if (!sb.required) {
        // b saw no creation: a's keys stand
    } else if (!sa.required) {
        sa.required = sb.required;
    } else {
        StringSet both;
        std::ranges::set_intersection(*sa.required, *sb.required, std::inserter(both, both.begin()));
        sa.required = std::move(both);
    }
    if (sb.dynamic && !sa.dynamic) sa.dynamic = sb.dynamic;
    sa.hints.insert(sa.hints.end(), sb.hints.begin(), sb.hints.end());
    sb.hints.clear();
    if (sb.mutated) sa.mutated = true;
    if (sb.poisoned && !sa.poisoned) sa.poisoned = sb.poisoned;
    return a;
}

void Program::mark_mutated(SiteId id) {
    const SiteId root = find(id);
    if (!sites_[root].mutated) {
        sites_[root].mutated = true;
        touch(FactKey::of_site(root));
    }
}

Type& Program::table_of(Table table, SiteId root) { return table == Table::Elem ? sites_[root].elem : sites_[root].keys; }

SiteId Program::grow(Table table, SiteId id, const Type& type) {
    const SiteId root = find(id);
    if (type.kind == pt::Kind::Top && !suspended() && !sites_[root].poisoned) {
        sites_[root].poisoned = type.why && !type.why->empty() ? *type.why : "?";   // its readers cannot know what its items are
        touch(FactKey::of_site(root));
    }
    if (suspended() || type.kind == pt::Kind::Top) return root;
    const Type old_type = table_of(table, root);
    Type joined = join(old_type, type);
    Type grown = settle(old_type, joined);
    if (grown != old_type) {
        table_of(table, root) = std::move(grown);
        touch(FactKey::of_site(root));
    }
    return root;
}

void Program::grow_field(SiteId id, const std::string& key, const Type& type) {
    const SiteId root = find(id);
    if (type.kind == pt::Kind::Top) {
        if (!sites_[root].fields.contains(key)) sites_[root].fields.insert_or_assign(key, pt::bottom());
        if (!suspended() && !sites_[root].poisoned) {
            sites_[root].poisoned = type.why && !type.why->empty() ? *type.why : "?";
            touch(FactKey::of_site(root));
        }
        return;
    }
    if (suspended()) {
        if (!sites_[root].fields.contains(key)) sites_[root].fields.insert_or_assign(key, pt::bottom());
        return;
    }
    const Type* existing = sites_[root].fields.find(key);
    const bool known = existing != nullptr;
    const Type old_type = known ? *existing : pt::bottom();
    Type joined = join(old_type, type);
    Type grown = settle(old_type, joined);
    if (!known || grown != old_type) {
        sites_[root].fields.insert_or_assign(key, std::move(grown));
        touch(FactKey::of_site(root));
    }
    grow(Table::Elem, root, type);
    grow(Table::Keys, root, pt::str());
}

void Program::created(SiteId id, const StringSet& keys) {
    const SiteId root = find(id);
    if (suspended()) return;
    std::optional<StringSet>& required = sites_[root].required;
    StringSet updated = keys;
    if (required) {
        updated.clear();
        std::ranges::set_intersection(*required, keys, std::inserter(updated, updated.begin()));
    }
    if (!required || *required != updated) {
        required = std::move(updated);
        touch(FactKey::of_site(root));
    }
}

void Program::make_dynamic(SiteId id, const std::string& why) {
    const SiteId root = find(id);
    if (!sites_[root].dynamic) {
        sites_[root].dynamic = why;
        touch(FactKey::of_site(root));
    }
}

bool Program::is_record(SiteId id) {
    const Site& s = sites_[find(id)];
    return s.kind == Container::Dict && !s.dynamic;
}

void Program::hint(SiteId id, const std::string& name) {
    std::vector<std::string>& hints = sites_[find(id)].hints;
    if (std::ranges::find(hints, name) == hints.end()) hints.push_back(name);
}

// ------------------------------------------------------------------------------------ the join

Type Program::join(const Type& a, const Type& b) {
    if (a == b) return a;
    const pt::Kind ka = a.kind;
    const pt::Kind kb = b.kind;
    if (ka == pt::Kind::Bottom) return b;
    if (kb == pt::Kind::Bottom) return a;
    if (ka == pt::Kind::Top) return a;
    if (kb == pt::Kind::Top) return b;
    if (ka == pt::Kind::None || kb == pt::Kind::None) {
        const Type& other = ka == pt::Kind::None ? b : a;
        return other.kind == pt::Kind::Opt ? other : pt::optional_of(other);
    }
    if (ka == pt::Kind::Opt || kb == pt::Kind::Opt) {
        Type joined = join(pt::strip_optional(a), pt::strip_optional(b));
        return joined.kind == pt::Kind::Top ? joined : pt::optional_of(joined);
    }
    if (ka != kb) return pt::top(program_type_word(a) + " and " + program_type_word(b) + " mixed");
    switch (ka) {
        case pt::Kind::Int: return pt::join_int(a, b);
        case pt::Kind::Bytes:
        case pt::Kind::Bytearray: {
            if (!a.hi || !b.hi) return pt::bytes_of(std::nullopt, ka);
            return pt::bytes_of(*a.hi < *b.hi ? b.hi : a.hi, ka);
        }
        case pt::Kind::Tuple: {
            if (a.args.size() != b.args.size()) {
                return pt::top(std::format("tuples of {} and {} items mixed", a.args.size(), b.args.size()));
            }
            std::vector<Type> parts;
            for (std::size_t i = 0; i < a.args.size(); ++i) {
                Type part = join(a.args[i], b.args[i]);
                if (part.kind == pt::Kind::Top) return part;
                parts.push_back(std::move(part));
            }
            return pt::tuple_of(std::move(parts));
        }
        case pt::Kind::List:
        case pt::Kind::Dict:
        case pt::Kind::Set: return program_container_type(ka, union_sites(a.site.value(), b.site.value()));
        case pt::Kind::Inst: {
            const std::optional<std::string> base = common_base(a.name.value_or(""), b.name.value_or(""));
            if (base) return program_inst_type(*base);
            return pt::top("instances of " + a.name.value_or("") + " and " + b.name.value_or("") + " mixed");
        }
        default: return pt::top(program_type_word(a) + " and " + program_type_word(b) + " mixed");
    }
}

std::optional<std::string> Program::common_base(const std::string& x, const std::string& y) {
    const std::vector<std::string> ancestors = mro(x);
    for (const std::string& c : mro(y)) {
        if (std::ranges::find(ancestors, c) != ancestors.end()) return c;
    }
    return std::nullopt;
}

std::vector<std::string> Program::mro(const std::string& cls) {
    std::vector<std::string> out;
    std::deque<std::string> todo{cls};
    while (!todo.empty()) {
        std::string c = std::move(todo.front());
        todo.pop_front();
        if (std::ranges::find(out, c) != out.end() || !classes.contains(c)) continue;
        const Class& k = *classes.at(c);
        todo.insert(todo.end(), k.bases.begin(), k.bases.end());
        out.push_back(std::move(c));
    }
    return out;
}

std::vector<std::string> Program::descendants(const std::string& cls) {
    std::vector<std::string> out;
    std::vector<std::string> todo{cls};
    while (!todo.empty()) {
        const std::string c = std::move(todo.back());
        todo.pop_back();
        for (const std::string& s : classes.at(c)->subclasses) {
            if (std::ranges::find(out, s) == out.end()) {
                out.push_back(s);
                todo.push_back(s);
            }
        }
    }
    return out;
}

Type Program::canon(const Type& type) {
    if (type.kind == pt::Kind::List || type.kind == pt::Kind::Dict || type.kind == pt::Kind::Set) {
        const SiteId root = find(type.site.value());
        return root == type.site ? type : program_container_type(type.kind, root);
    }
    if (!type.args.empty()) {
        Type copy = type;
        for (Type& arg : copy.args) arg = canon(arg);
        return copy;
    }
    return type;
}

}  // namespace cb::translate::frontends::python::program
