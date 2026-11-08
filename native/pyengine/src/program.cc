// program.cc -- the Program: modules, symbols, imports, containers and the join (program.py).
#include "program.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>

#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;
namespace fs = std::filesystem;

static std::set<std::string> words(const char* s) {
    std::set<std::string> out;
    std::istringstream in(s);
    std::string w;
    while (in >> w) out.insert(w);
    return out;
}

const std::set<std::string> STDLIB = words(
    "abc argparse array ast base64 binascii bisect builtins bz2 calendar codecs collections "
    "concurrent configparser contextlib copy csv ctypes dataclasses datetime decimal difflib dis email enum errno "
    "fnmatch fractions functools gc getpass glob gzip hashlib heapq hmac html http importlib inspect io ipaddress "
    "itertools json keyword locale logging lzma math mimetypes mmap multiprocessing numbers operator os pathlib "
    "pickle platform pprint queue random re reprlib secrets select shlex shutil signal socket sqlite3 ssl stat "
    "statistics string struct subprocess sys sysconfig tarfile tempfile textwrap threading time timeit tkinter "
    "token tokenize traceback types typing unicodedata unittest urllib uuid warnings weakref winreg xml zipfile "
    "zlib zipimport __future__ msvcrt _thread");

const std::set<std::string> EXCEPTIONS = words(
    "Exception BaseException ValueError KeyError IndexError TypeError RuntimeError "
    "OSError IOError LookupError ArithmeticError ZeroDivisionError OverflowError NotImplementedError "
    "AssertionError StopIteration EOFError FileNotFoundError AttributeError UnicodeDecodeError "
    "UnicodeEncodeError PermissionError TimeoutError");

std::string deco_name(const A::Expr* d) {
    if (auto* n = as<pyfront::Name>(d)) return n->id;
    if (auto* a = as<Attribute>(d)) return a->attr;
    if (auto* c = as<pyfront::Call>(d)) return deco_name(c->func);
    return "?";
}

Fn::Fn(std::string q, const A::FunctionDef* n, Module* m, Class* c) : qname(std::move(q)), name(n->name), node(n), module(m), cls(c) {
    for (auto* d : n->decorator_list) decorators.push_back(deco_name(d));
    auto has = [&](const char* w) { return std::find(decorators.begin(), decorators.end(), w) != decorators.end(); };
    if (c) {
        kind = "method";
        if (has("staticmethod")) kind = "static";
        else if (has("classmethod")) kind = "classmethod";
        else if (has("property")) kind = "property";
    }
    const arguments* a = n->args;
    std::vector<const arg*> pos;
    for (auto* x : a->posonlyargs) pos.push_back(x);
    for (auto* x : a->args) pos.push_back(x);
    for (auto* x : pos) params.push_back(x->arg);
    for (auto* x : a->kwonlyargs) kwonly.push_back(x->arg);
    for (auto* x : pos)
        if (x->annotation) annotations.set(x->arg, x->annotation);
    for (auto* x : a->kwonlyargs)
        if (x->annotation) annotations.set(x->arg, x->annotation);
    size_t nd = a->defaults.size();
    for (size_t i = 0; i < nd && i < params.size(); i++) defaults.set(params[params.size() - nd + i], a->defaults[i]);
    for (size_t i = 0; i < a->kwonlyargs.size() && i < a->kw_defaults.size(); i++)
        if (a->kw_defaults[i]) defaults.set(a->kwonlyargs[i]->arg, a->kw_defaults[i]);
    if (a->vararg) varargs = a->vararg->arg;
    if (a->kwarg) kwargs = a->kwarg->arg;
    int end = n->pos.end_lineno ? n->pos.end_lineno : n->pos.lineno;
    lines = end - n->pos.lineno + 1;
}

std::vector<std::string> Fn::bound() const {
    if (kind == "method" || kind == "property" || kind == "classmethod")
        return params.empty() ? params : std::vector<std::string>(params.begin() + 1, params.end());
    return params;
}

Program::Program() {
    parent.push_back(0);
    ckind.push_back(TK::Bottom);
    elem.emplace_back();
    keys.emplace_back();
    fields.emplace_back();
    required.emplace_back();
    site_where.emplace_back();
    site_node.push_back(nullptr);
}
Program::~Program() = default;

// ------------------------------------------------------------------------------------ loading
// (new) a function's `name = lambda args: expr` -- the name bound once, to that lambda -- is the nested
// function `def name(args): return expr`, which the typed path lowers as a C++ lambda: the same function,
// the same name, its body the lambda's one expression
static void count_binds(const std::vector<A::Stmt*>& body, std::map<std::string, int>& n) {
    for (auto* st : body) {
        if (auto* a = as<Assign>(st))
            for (auto* t : a->targets) {
                if (auto* nm = as<pyfront::Name>(t)) n[nm->id]++;
                else n["*"]++;   // (a tuple target: names this pass does not count are left alone)
            }
        if (st->kind == StmtKind::AugAssign || st->kind == StmtKind::AnnAssign || st->kind == StmtKind::For ||
            st->kind == StmtKind::With || st->kind == StmtKind::Global || st->kind == StmtKind::Nonlocal || st->kind == StmtKind::Delete)
            n["*"]++;
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef || st->kind == StmtKind::ClassDef) continue;
        for (auto* l : stmt_lists(st)) count_binds(*l, n);
    }
}
void expand_class_factories(pyfront::Module& tree);   // factories.cc
void hoist_local_imports(pyfront::Module& tree);     // local_imports.cc
void callbacks_as_lambdas(pyfront::Module& tree);    // callbacks.cc
static void lambdas_to_defs(pyfront::Module& tree, std::vector<A::Stmt*>& body, const std::map<std::string, int>& binds) {
    for (auto*& st : body) {
        if (auto* a = as<Assign>(st)) {
            auto* nm = a->targets.size() == 1 ? as<pyfront::Name>(a->targets[0]) : nullptr;
            auto* lam = as<pyfront::Lambda>(a->value);
            auto it = nm ? binds.find(nm->id) : binds.end();
            if (nm && lam && it != binds.end() && it->second == 1 && !binds.count("*")) {
                auto* fd = tree.make<A::FunctionDef>();
                fd->pos = st->pos;
                fd->name = nm->id;
                fd->args = lam->args;
                auto* r = tree.make<A::Return>();
                r->pos = lam->body->pos;
                r->value = lam->body;
                fd->body.push_back(r);
                st = fd;
                continue;
            }
        }
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) {
            auto* f = static_cast<A::FunctionDef*>(st);
            std::map<std::string, int> own;
            count_binds(f->body, own);
            lambdas_to_defs(tree, f->body, own);
            continue;
        }
        if (auto* c = as<pyfront::ClassDef>(st)) {
            lambdas_to_defs(tree, c->body, {{"*", 1}});   // (a class body's names are its attributes: left alone)
            continue;
        }
        for (auto* l : stmt_lists(st)) lambdas_to_defs(tree, *const_cast<std::vector<A::Stmt*>*>(l), binds);
    }
}

Module* Program::add_module(const std::string& name, const std::string& path, const std::string& source, std::string* err) {
    auto r = pyfront::parse(source, path);
    if (!r) {
        if (err) *err = r.error->msg;
        return nullptr;
    }
    if (extended) lambdas_to_defs(*r.module, r.module->body, {{"*", 1}});   // (module level: its globals are left alone)
    if (extended) hoist_local_imports(*r.module);      // (local_imports.cc: a function's own imports as the module's)
    if (extended) expand_class_factories(*r.module);   // (factories.cc: X = factory(...) as the class it makes)
    if (extended) callbacks_as_lambdas(*r.module);     // (callbacks.cc: re.sub(p, f, s) as re.sub(p, lambda m: f(m), s))
    if (extended)                                      // (new) dict((k, v) for ...) as {k: v for ...}: the same pairs, in the same order
        for (auto* st : r.module->body)
            for (const AnyNode& x : walk(AnyNode(st))) {
                auto* c = as<pyfront::Call>(x.expr());
                auto* fn = c ? as<pyfront::Name>(c->func) : nullptr;
                if (!fn || fn->id != "dict" || c->args.size() != 1 || !c->keywords.empty()) continue;
                auto* g = as<pyfront::GeneratorExp>(c->args[0]);
                auto* pair = g ? as<pyfront::Tuple>(g->elt) : nullptr;
                if (!pair || pair->elts.size() != 2 || pair->elts[0]->kind == ExprKind::Starred || pair->elts[1]->kind == ExprKind::Starred) continue;
                auto* dc = r.module->make<A::DictComp>();
                dc->pos = c->pos;
                dc->key = pair->elts[0];
                dc->value = pair->elts[1];
                dc->generators = g->generators;
                dict_gens[c] = dc;
            }
    if (extended) {                                    // (spread.cc: f(**record) as its keywords, checked by the inference)
        std::vector<SpreadCheck> checks;
        spread_record_keywords(*r.module, checks);
        for (auto& c : checks) spread_checks[c.call] = c;
    }
    auto m = std::make_unique<Module>();
    m->name = name;
    m->path = path;
    m->source = source;
    m->tree = std::move(r.module);
    m->lines = line_starts(source);
    m->is_package = fs::path(path).filename().string() == "__init__.py";
    std::string doc;
    m->has_docstring = docstring(m->tree->body, doc);
    Module* raw = m.get();
    owned_modules.push_back(std::move(m));
    modules.set(name, raw);
    return raw;
}

static std::string read_file(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void Program::add_tree(const std::string& root, const std::string& package) {
    std::function<void(const fs::path&)> visit = [&](const fs::path& dir) {
        std::vector<std::string> files, dirs;
        for (auto& e : fs::directory_iterator(dir)) {
            std::string n = e.path().filename().string();
            if (e.is_directory()) {
                if (n != "__pycache__") dirs.push_back(n);
            } else {
                files.push_back(n);
            }
        }
        std::sort(files.begin(), files.end());
        std::sort(dirs.begin(), dirs.end());
        for (auto& f : files) {
            if (f.size() < 3 || f.substr(f.size() - 3) != ".py") continue;
            fs::path p = dir / f;
            std::string rel = fs::relative(p, root).generic_string();
            rel = rel.substr(0, rel.size() - 3);
            std::replace(rel.begin(), rel.end(), '/', '.');
            if (rel.size() >= 8 && rel.substr(rel.size() - 8) == "__init__") {
                rel = rel.substr(0, rel.size() - 8);
                while (!rel.empty() && rel.back() == '.') rel.pop_back();
            }
            std::string name = package + (rel.empty() ? "" : "." + rel);
            add_module(name, p.string(), read_file(p));
        }
        for (auto& d : dirs) visit(dir / d);
    };
    visit(fs::path(root));
}

void Program::index() {
    for (auto& [_, m] : modules)
        for (auto* st : m->tree->body) index_stmt(m, st);
    for (auto& [_, c] : classes) {
        for (auto* b : c->node->bases) {
            Ref r = resolve_expr_name(c->module, b);
            auto* nm = as<pyfront::Name>(b);
            if (r.k == "class") {
                c->bases.push_back(r.a);
                (*classes.get(r.a))->subclasses.push_back(c->qname);
            } else if (nm && EXCEPTIONS.count(nm->id)) {
                c->exception = nm->id;
            } else if (nm && nm->id == "object") {
            } else {
                c->foreign_base = unparse(b);
            }
        }
    }
    for (auto& [_, c] : classes) {
        Class* k = c;
        int guard = 0;
        while (k->exception.empty() && !k->bases.empty() && guard++ < 100) k = *classes.get(k->bases[0]);
        if (c->exception.empty()) c->exception = k->exception;
    }
    if (extended) {
        std::vector<Fn*> all;
        for (auto& [q, f] : fns) all.push_back(f);
        for (Fn* f : all) index_nested(f);
    }
    if (extended) star_calls();   // (star.cc: f(*tuple) as its arguments, checked by the inference)
    for (auto& [q, f] : fns) index_fn(f);
    for (auto& [q, f] : nested) index_fn(f);
    if (extended) dispatch_hints();
}

void Program::dispatch_hints() {
    std::map<std::string, std::vector<std::string>> by_name;   // a class's simple name -> the classes so named
    for (auto& [q, c] : classes) by_name[c->name].push_back(q);
    for (auto& [cq, c] : classes) {
        std::set<std::string> prefixes;
        // (new) per prefix: the dispatcher and, for each argument after the node that it passes to the method it
        // found (`f = getattr(...); f(s, out, ind)`), the dispatcher's parameter it is ("" if not one); several
        // dispatchers of one prefix: none
        std::map<std::string, std::pair<Fn*, std::vector<std::string>>> passes;
        std::set<std::string> ambiguous;
        auto is_type_name = [](const A::Expr* e) {      // type(e).__name__
            auto* nm = as<Attribute>(e);
            auto* tc = nm ? as<pyfront::Call>(nm->value) : nullptr;
            auto* tf = tc ? as<pyfront::Name>(tc->func) : nullptr;
            return nm && nm->attr == "__name__" && tf && tf->id == "type" && tc->args.size() == 1;
        };
        for (auto& [_, m] : c->methods) {
            if (m->params.empty()) continue;
            // the method's names bound once, and to what (an assignment's value)
            std::map<std::string, std::pair<int, const A::Expr*>> once;
            for (const AnyNode& x : walk(AnyNode(m->node)))
                if (auto* nm = as<pyfront::Name>(x.expr()); nm && nm->ctx == ExprCtx::Store) once[nm->id].first++;
            for (const AnyNode& x : walk(AnyNode(m->node)))
                if (auto* st = x.stmt())
                    if (auto* a = as<A::Assign>(st); a && a->targets.size() == 1)
                        if (auto* t = as<pyfront::Name>(a->targets[0])) once[t->id].second = a->value;
            auto bound_to = [&](const std::string& n) -> const A::Expr* {
                auto it = once.find(n);
                return it != once.end() && it->second.first == 1 ? it->second.second : nullptr;
            };
            for (const AnyNode& x : walk(AnyNode(m->node))) {
                auto* call = as<pyfront::Call>(x.expr());
                auto* fname = call ? as<pyfront::Name>(call->func) : nullptr;
                if (!fname || fname->id != "getattr" || call->args.size() < 2) continue;
                auto* obj = as<pyfront::Name>(call->args[0]);
                auto* add = as<BinOp>(call->args[1]);
                if (!obj || obj->id != m->params[0] || !add || add->op != OpKind::Add) continue;
                std::string prefix;
                const A::Expr* right = add->right;
                if (auto* k = as<pyfront::Name>(right))          // k = type(s).__name__; getattr(self, 's_' + k)
                    if (const A::Expr* b = bound_to(k->id)) right = b;
                if (!is_str_const(add->left, &prefix) || prefix.empty() || !is_type_name(right)) continue;
                prefixes.insert(prefix);
                // f = getattr(...) and the calls f(node, a, b): which of the dispatcher's parameters a, b are
                std::vector<std::string> args;
                bool have = false;
                for (auto& [lname, info] : once) {
                    if (info.first != 1 || info.second != call) continue;
                    for (const AnyNode& y : walk(AnyNode(m->node))) {
                        auto* fc = as<pyfront::Call>(y.expr());
                        auto* fn = fc ? as<pyfront::Name>(fc->func) : nullptr;
                        if (!fn || fn->id != lname || !fc->keywords.empty() || fc->args.empty()) continue;
                        std::vector<std::string> mine;
                        for (size_t i = 1; i < fc->args.size(); i++) {
                            auto* an = as<pyfront::Name>(fc->args[i]);
                            bool is_param = an && std::find(m->params.begin() + 1, m->params.end(), an->id) != m->params.end() &&
                                            !once.count(an->id);
                            mine.push_back(is_param ? an->id : "");
                        }
                        if (have && mine != args) mine.clear();
                        args = mine;
                        have = true;
                    }
                }
                if (passes.count(prefix) && passes[prefix].first != m) ambiguous.insert(prefix);
                passes[prefix] = {m, args};
            }
        }
        if (prefixes.empty()) continue;
        std::vector<Class*> todo = {c};   // the class and its subclasses: the methods the dispatch can reach
        std::set<std::string> seen;
        while (!todo.empty()) {
            Class* k = todo.back();
            todo.pop_back();
            if (!seen.insert(k->qname).second) continue;
            for (auto& sq : k->subclasses)
                if (Class** sc = classes.get(sq)) todo.push_back(*sc);
            for (auto& [mname, m] : k->methods)
                for (auto& pre : prefixes) {
                    if (mname.size() <= pre.size() || mname.compare(0, pre.size(), pre) != 0) continue;
                    auto bn = by_name.find(mname.substr(pre.size()));
                    if (bn == by_name.end()) continue;
                    if (m->params.size() < 2 || !m->varargs.empty() || m->annotations.has(m->params[1])) continue;
                    // (new) the method's parameters after the node: the dispatcher's own, when it passes them on
                    auto ps = passes.find(pre);
                    bool aliased = false;
                    if (ps != passes.end() && !ambiguous.count(pre) && ps->second.second.size() + 2 == m->params.size()) {
                        for (size_t i = 2; i < m->params.size(); i++)
                            if (!ps->second.second[i - 2].empty() && !m->annotations.has(m->params[i]))
                                dispatch_alias[{m->qname, m->params[i]}] = {ps->second.first, ps->second.second[i - 2]};
                        aliased = true;
                    }
                    if (m->params.size() != 2 && !aliased) continue;
                    // the class of that name: the only one, or the only one of a module the visitor's module imports
                    // (ir.Unary of the L2 IR, not the Java front end's Unary)
                    std::vector<std::string> cands = bn->second;
                    if (cands.size() > 1) {
                        std::vector<std::string> near;
                        for (auto& q : cands) {
                            Class* cc = *classes.get(q);
                            bool imported = cc->module == m->module;
                            for (auto& [__, r] : m->module->imports)
                                if ((r.k == "module" && r.a == cc->module->name) || (r.k == "obj" && r.a == cc->module->name)) imported = true;
                            if (imported) near.push_back(q);
                        }
                        cands = near;
                    }
                    if (cands.size() != 1) continue;
                    dispatch_param[m->qname] = {m->params[1], named(TK::Inst, cands[0])};
                }
        }
    }
}

void Program::index_nested(Fn* parent) {
    // the defs of a function's body (in its compound statements, not in nested defs or classes)
    std::vector<const A::Stmt*> todo(parent->node->body.begin(), parent->node->body.end());
    while (!todo.empty()) {
        const A::Stmt* s = todo.back();
        todo.pop_back();
        if (s->kind == StmtKind::FunctionDef || s->kind == StmtKind::AsyncFunctionDef) {
            auto* fd = cast<FunctionDef>(s);
            auto f = std::make_unique<Fn>(parent->qname + ".<locals>." + fd->name, fd, parent->module, nullptr);
            if (s->kind == StmtKind::AsyncFunctionDef) f->kind = "async";
            f->parent = parent;
            Fn* raw = f.get();
            owned_fns.push_back(std::move(f));
            if (nested.has(raw->qname)) raw->qname += "@" + std::to_string(fd->pos.lineno);   // two defs of one name
            nested.set(raw->qname, raw);
            nested_by_node[fd] = raw;
            parent->has_nested = true;
            index_nested(raw);
            continue;
        }
        if (s->kind == StmtKind::ClassDef) continue;
        for (auto* l : stmt_lists(s, true))
            for (auto* b : *l) todo.push_back(b);
    }
}

Fn* Program::any_fn(const std::string& q) {
    if (Fn** f = fns.get(q)) return *f;
    if (Fn** f = nested.get(q)) return *f;
    if (Fn** f = specs.get(q)) return *f;
    return nullptr;
}

Fn* Program::spec_for(Fn* f, const std::string& sig) {
    std::string key = f->qname + "<" + sig + ">";
    if (Fn** got = specs.get(key)) return *got;
    auto c = std::make_unique<Fn>(*f);
    c->qname = key;
    c->spec_of = f;
    c->spec_sig = sig;
    Fn* raw = c.get();
    owned_fns.push_back(std::move(c));
    specs.set(key, raw);
    specs_of[f->qname].push_back(raw);
    touch("spec");
    return raw;
}

T Program::closure_var(Fn* f, const std::string& name) {
    read("cv:" + f->qname);
    auto it = closure_vars.find(f->qname);
    if (it == closure_vars.end()) return BOTTOM();
    auto jt = it->second.find(name);
    return jt == it->second.end() ? BOTTOM() : jt->second;
}

void Program::index_fn(Fn* f) {
    {
        // the parameters indexed with in the first statement (a successful call bounds them)
        std::vector<const A::Stmt*> body;
        for (auto* s : f->node->body) {
            auto* e = as<ExprStmt>(s);
            if (e && is_const(e->value)) continue;
            body.push_back(s);
        }
        if (!body.empty()) {
            const A::Stmt* first = body[0];
            if (first->kind == StmtKind::Return || first->kind == StmtKind::Assign || first->kind == StmtKind::Expr) {
                std::set<std::string> out;
                for (const AnyNode& n : walk(first)) {
                    if (auto* sub = as<Subscript>(n.expr())) {
                        if (auto* nm = as<pyfront::Name>(sub->slice)) out.insert(nm->id);
                    }
                    if (auto* c = as<pyfront::Call>(n.expr())) {
                        auto* at = as<Attribute>(c->func);
                        if (at && at->attr == "unpack_from" && c->args.size() >= 3)
                            if (auto* nm = as<pyfront::Name>(c->args[2])) out.insert(nm->id);
                    }
                }
                for (auto& p : f->params)
                    if (out.count(p)) f->index_params.insert(p);
            }
        }
        // the parameters ready to get None
        for (auto& [n, d] : f->defaults)
            if (is_none_const(d)) f->none_aware.insert(n);
        std::set<std::string> ps(f->params.begin(), f->params.end());
        ps.insert(f->kwonly.begin(), f->kwonly.end());
        for (const AnyNode& n : walk(AnyNode(f->node))) {
            const A::Expr* e = n.expr();
            const A::Stmt* s = n.stmt();
            if (auto* cmp = as<Compare>(e)) {
                std::vector<const A::Expr*> all = {cmp->left};
                for (auto* x : cmp->comparators) all.push_back(x);
                bool nones = false;
                for (auto* x : all)
                    if (is_none_const(x)) nones = true;
                if (nones)
                    for (auto* x : all)
                        if (auto* nm = as<pyfront::Name>(x))
                            if (ps.count(nm->id)) f->none_aware.insert(nm->id);
                continue;
            }
            const A::Expr* test = nullptr;
            if (auto* i = as<pyfront::If>(s)) test = i->test;
            else if (auto* w = as<pyfront::While>(s)) test = w->test;
            else if (auto* ie = as<IfExp>(e)) test = ie->test;
            else if (auto* as_ = as<pyfront::Assert>(s)) test = as_->test;
            if (test) {
                if (auto* nm = as<pyfront::Name>(test))
                    if (ps.count(nm->id)) f->none_aware.insert(nm->id);
                continue;
            }
            if (auto* u = as<UnaryOp>(e)) {
                if (u->op == UnaryOpKind::Not)
                    if (auto* nm = as<pyfront::Name>(u->operand))
                        if (ps.count(nm->id)) f->none_aware.insert(nm->id);
                continue;
            }
            if (auto* b = as<BoolOp>(e)) {
                for (auto* v : b->values)
                    if (auto* nm = as<pyfront::Name>(v))
                        if (ps.count(nm->id)) f->none_aware.insert(nm->id);
            }
        }
        // a generator (new)
        for (const AnyNode& n : walk(AnyNode(f->node))) {
            if (n.expr() && (n.expr()->kind == ExprKind::Yield || n.expr()->kind == ExprKind::YieldFrom)) {
                f->generator = true;
                break;
            }
        }
    }
}

void Program::index_stmt(Module* m, const A::Stmt* st) {
    switch (st->kind) {
        case StmtKind::FunctionDef:
        case StmtKind::AsyncFunctionDef: {
            auto* fd = cast<FunctionDef>(st);
            auto f = std::make_unique<Fn>(m->name + "." + fd->name, fd, m, nullptr);
            if (st->kind == StmtKind::AsyncFunctionDef) f->kind = "async";
            Fn* raw = f.get();
            owned_fns.push_back(std::move(f));
            m->functions.set(fd->name, raw);
            fns.set(raw->qname, raw);
            break;
        }
        case StmtKind::ClassDef: {
            auto* cd = cast<ClassDef>(st);
            auto c = std::make_unique<Class>();
            c->qname = m->name + "." + cd->name;
            c->name = cd->name;
            c->node = cd;
            c->module = m;
            for (auto* d : cd->decorator_list) c->decorators.push_back(deco_name(d));
            Class* raw = c.get();
            owned_classes.push_back(std::move(c));
            m->classes.set(cd->name, raw);
            classes.set(raw->qname, raw);
            for (auto* b : cd->body) {
                if (b->kind == StmtKind::FunctionDef || b->kind == StmtKind::AsyncFunctionDef) {
                    auto* fd = cast<FunctionDef>(b);
                    auto f = std::make_unique<Fn>(raw->qname + "." + fd->name, fd, m, raw);
                    Fn* fr = f.get();
                    owned_fns.push_back(std::move(f));
                    raw->methods.set(fd->name, fr);
                    fns.set(fr->qname, fr);
                } else if (auto* a = as<pyfront::Assign>(b)) {
                    if (a->targets.size() == 1)
                        if (auto* nm = as<pyfront::Name>(a->targets[0])) raw->class_attrs.set(nm->id, a->value);
                } else if (auto* aa = as<AnnAssign>(b)) {
                    if (auto* nm = as<pyfront::Name>(aa->target))
                        if (aa->value) raw->class_attrs.set(nm->id, aa->value);
                }
            }
            break;
        }
        case StmtKind::Import:
            for (auto* a : cast<Import>(st)->names) {
                std::string local = a->has_asname ? a->asname : a->name.substr(0, a->name.find('.'));
                if (m->functions.has(local) || m->classes.has(local)) m->shadowed.insert(local);   // (rebound after its def)
                if (a->has_asname) {
                    m->imports.set(a->asname, module_ref(a->name));
                } else {
                    std::string top = a->name.substr(0, a->name.find('.'));
                    m->imports.set(top, module_ref(top));
                }
            }
            break;
        case StmtKind::ImportFrom: {
            auto* imf = cast<ImportFrom>(st);
            std::string base = from_base(m, imf);
            for (auto* a : imf->names) {
                if (a->name == "*") continue;
                std::string local = a->has_asname ? a->asname : a->name;
                std::string full = base.empty() ? a->name : base + "." + a->name;
                if (m->functions.has(local) || m->classes.has(local)) m->shadowed.insert(local);   // (rebound after its def)
                if (modules.has(full)) m->imports.set(local, Ref{"module", full, ""});
                else if (modules.has(base)) m->imports.set(local, Ref{"obj", base, a->name});
                else m->imports.set(local, Ref{"extern", full, ""});
            }
            break;
        }
        case StmtKind::Assign: {
            auto* a = cast<pyfront::Assign>(st);
            for (auto* t : a->targets) {
                // (new) A, B = (1, 2): each name its own item (a tuple of names and a tuple of as many values, no *)
                auto* tt = as<pyfront::Tuple>(t);
                auto* vt = as<pyfront::Tuple>(a->value);
                if (extended && tt && vt && tt->elts.size() == vt->elts.size()) {
                    bool plain = true;
                    for (size_t i = 0; i < tt->elts.size(); i++)
                        if (!as<pyfront::Name>(tt->elts[i]) || vt->elts[i]->kind == ExprKind::Starred) plain = false;
                    if (plain) {
                        for (size_t i = 0; i < tt->elts.size(); i++) m->assigns[cast<pyfront::Name>(tt->elts[i])->id].push_back(vt->elts[i]);
                        continue;
                    }
                }
                for (auto& n : target_names(t)) m->assigns[n].push_back(as<pyfront::Name>(t) ? a->value : nullptr);
            }
            for (auto* t : a->targets)   // (TABLE[k] = v, obj.a = v: a statement the module runs)
                if (t->kind == ExprKind::Subscript || t->kind == ExprKind::Attribute) {
                    m->statements.push_back(st);
                    break;
                }
            break;
        }
        case StmtKind::AnnAssign: {
            auto* a = cast<AnnAssign>(st);
            if (auto* nm = as<pyfront::Name>(a->target)) m->assigns[nm->id].push_back(a->value);
            else m->statements.push_back(st);
            break;
        }
        case StmtKind::If: {
            auto* i = cast<pyfront::If>(st);
            for (auto* b : i->body) index_stmt(m, b);
            for (auto* b : i->orelse) index_stmt(m, b);
            m->statements.push_back(st);
            break;
        }
        case StmtKind::Try:
        case StmtKind::TryStar: {
            auto* t = cast<pyfront::Try>(st);
            for (auto* b : t->body) index_stmt(m, b);
            for (auto* b : t->orelse) index_stmt(m, b);
            for (auto* h : t->handlers)
                for (auto* b : h->body) index_stmt(m, b);
            m->statements.push_back(st);
            break;
        }
        case StmtKind::Expr:
            if (is_const(cast<ExprStmt>(st)->value)) break;   // a docstring
            m->statements.push_back(st);
            break;
        default:
            m->statements.push_back(st);
    }
}

Ref Program::module_ref(const std::string& name) {
    if (modules.has(name)) return Ref{"module", name, ""};
    std::string top = name.substr(0, name.find('.'));
    return STDLIB.count(top) ? Ref{"stdmod", name, ""} : Ref{"extern", name, ""};
}

std::string Program::from_base(Module* m, const A::ImportFrom* st) {
    if (!st->level) return st->has_module ? st->module : "";
    std::vector<std::string> parts;
    std::string cur;
    for (char c : m->name) {
        if (c == '.') {
            parts.push_back(cur);
            cur.clear();
        } else cur += c;
    }
    parts.push_back(cur);
    int keep = (int)parts.size() - st->level + (m->is_package ? 1 : 0);
    std::string base;
    for (int i = 0; i < keep && i < (int)parts.size(); i++) base += (i ? "." : "") + parts[i];
    if (st->has_module && !st->module.empty()) base += "." + st->module;
    return base;
}

Ref Program::resolve_global(Module* m, const std::string& name) {
    static int depth = 0;
    bool def_live = !m->shadowed.count(name);   // (a def an import after it rebinds: the name is the import's)
    if (Fn** f = def_live ? m->functions.get(name) : nullptr) return Ref{"fn", (*f)->qname, ""};
    if (Class** c = def_live ? m->classes.get(name) : nullptr) return Ref{"class", (*c)->qname, ""};
    if (m->assigns.has(name)) return Ref{"global", m->name, name};
    const Ref* imp = m->imports.get(name);
    if (!imp) return Ref{};
    if (imp->k == "obj") {
        Module* other = *modules.get(imp->a);
        const std::string& n2 = imp->b;
        if (other->functions.has(n2) || other->classes.has(n2) || other->assigns.has(n2) || other->imports.has(n2)) {
            if (depth > 50) return Ref{"missing", imp->a + "." + n2, ""};
            depth++;
            Ref r = resolve_global(other, n2);
            depth--;
            return r;
        }
        return Ref{"missing", imp->a + "." + n2, ""};
    }
    if (imp->k == "extern") {
        std::string top = imp->a.substr(0, imp->a.find('.'));
        if (STDLIB.count(top)) return Ref{"stdobj", imp->a, ""};
    }
    return *imp;
}

Ref Program::resolve_expr_name(Module* m, const A::Expr* node) {
    if (auto* n = as<pyfront::Name>(node)) return resolve_global(m, n->id);
    if (auto* a = as<Attribute>(node)) {
        Ref r = resolve_expr_name(m, a->value);
        if (r.k == "module") return resolve_global(*modules.get(r.a), a->attr);
    }
    return Ref{};
}

// ------------------------------------------------------------------------------------ type lookups
T Program::annotation_type(Fn* f, const std::string& name) {
    const A::Expr* const* node = f->annotations.get(name);
    if (!node) return BOTTOM();
    return ann_to_type(f->module, *node);
}

T Program::ann_to_type(Module* m, const A::Expr* a) {
    auto it = ann_cache.find(a);
    if (it != ann_cache.end()) return it->second;
    T t = ann(m, a);
    ann_cache[a] = t;
    return t;
}

T Program::ann(Module* m, const A::Expr* a) {
    std::string sv;
    if (is_str_const(a, &sv)) {
        auto r = pyfront::parse("(" + sv + "\n)", "<annotation>");
        if (!r || r.module->body.size() != 1 || r.module->body[0]->kind != StmtKind::Expr) return BOTTOM();
        const A::Expr* inner = cast<ExprStmt>(r.module->body[0])->value;
        ann_trees_.push_back(std::move(r.module));
        return ann(m, inner);
    }
    if (is_none_const(a)) return NONE();
    std::string name;
    if (auto* n = as<pyfront::Name>(a)) name = n->id;
    else if (auto* at = as<Attribute>(a)) name = at->attr;
    bool has_name = as<pyfront::Name>(a) || as<Attribute>(a);
    if (has_name) {
        if (name == "int") return INT();
        if (name == "str") return STR();
        if (name == "bytes") return BYTES();
        if (name == "bytearray") return BYTEARRAY();
        if (name == "bool") return BOOL();
        if (name == "float") return FLOAT();
        if (name == "None") return NONE();
        if (name == "list" || name == "List" || name == "dict" || name == "Dict" || name == "set" || name == "Set") {
            TK k = (name == "list" || name == "List") ? TK::List : (name == "dict" || name == "Dict") ? TK::Dict : TK::Set;
            int s = new_site(a, k, {m->name, a->pos.lineno});
            if (k == TK::Dict) make_dynamic(s, "annotated as a dict");
            return container(k, s);
        }
        if (as<pyfront::Name>(a)) {
            Ref r = resolve_global(m, name);
            if (r.k == "class") return named(TK::Inst, r.a);
        } else if (auto* at2 = as<Attribute>(a); at2 && extended) {   // (new) pkg.module.Class, as a stub spells another module's class
            std::string full = unparse(a);
            std::string mod = full.substr(0, full.size() - name.size() - 1);
            if (Module* const* om = modules.get(mod)) {
                Ref r = resolve_global(*om, name);
                if (r.k == "class") return named(TK::Inst, r.a);
            }
            (void)at2;
        }
        return BOTTOM();
    }
    if (auto* b = as<BinOp>(a)) {
        if (b->op == OpKind::BitOr) return join(ann(m, b->left), ann(m, b->right));
        return BOTTOM();
    }
    if (auto* sub = as<Subscript>(a)) {
        std::string head;
        if (auto* n = as<pyfront::Name>(sub->value)) head = n->id;
        else if (auto* at = as<Attribute>(sub->value)) head = at->attr;
        std::vector<const A::Expr*> args;
        if (auto* tu = as<pyfront::Tuple>(sub->slice)) args.assign(tu->elts.begin(), tu->elts.end());
        else args.push_back(sub->slice);
        if (head == "Optional") return opt(ann(m, args[0]));
        if (head == "Union") {
            T t = BOTTOM();
            for (auto* x : args) t = join(t, ann(m, x));
            return t;
        }
        if (head == "list" || head == "List" || head == "set" || head == "Set" || head == "Sequence" || head == "Iterable") {
            TK k = (head == "set" || head == "Set") ? TK::Set : TK::List;
            int s = new_site(a, k, {m->name, a->pos.lineno});
            grow(elem, s, ann(m, args[0]));
            return container(k, s);
        }
        if (head == "dict" || head == "Dict" || head == "Mapping") {
            int s = new_site(a, TK::Dict, {m->name, a->pos.lineno});
            make_dynamic(s, "annotated as a dict");
            if (args.size() == 2) {
                grow(keys, s, ann(m, args[0]));
                grow(elem, s, ann(m, args[1]));
            }
            return container(TK::Dict, s);
        }
        if (head == "tuple" || head == "Tuple") {
            for (auto* x : args) {
                auto* c = as<Constant>(x);
                if (c && c->value.kind == ConstValue::Kind::Ellipsis) return top("variable-length tuple annotation");
            }
            std::vector<T> ts;
            for (auto* x : args) ts.push_back(ann(m, x));
            return tuple_of(ts);
        }
    }
    return BOTTOM();
}

T Program::param_type(Fn* f, const std::string& name) {
    read("p:" + f->qname);
    read("d");
    T t = BOTTOM();
    auto pit = params.find(f->qname);
    if (pit != params.end()) {
        auto it = pit->second.find(name);
        if (it != pit->second.end()) t = it->second;
    }
    if (f->annotations.has(name)) {
        T at = annotation_type(f, name);
        if (at.kind == TK::Int) t = t.kind == TK::Int ? t : at;
        else if (at.kind != TK::Bottom) t = at;
    }
    if (auto dp = dispatch_param.find(f->qname); dp != dispatch_param.end() && dp->second.first == name)
        t = join(t, dp->second.second);   // (reached by a getattr dispatch on its class's name)
    if (auto da = dispatch_alias.find({f->qname, name}); da != dispatch_alias.end()) {
        static thread_local int alias_depth = 0;
        if (alias_depth < 4) {          // (the dispatcher's parameter, which this one is passed)
            alias_depth++;
            T dt = param_type(da->second.first, da->second.second);
            alias_depth--;
            if (dt.kind != TK::Bottom) t = join(t, dt);
        }
    }
    if (f->defaults.has(name)) {
        auto it = default_types.find(f->qname + "\x1f" + name);
        if (it != default_types.end()) t = join(t, it->second);
    }
    // (new) no caller types it: the type its uses demand (demand.cc)
    if (demand_enabled && (t.kind == TK::Bottom || t.kind == TK::None) && !f->annotations.has(name)) {
        T d = demand_type(f, name);
        if (d.kind != TK::Bottom) t = t.kind == TK::None ? opt(d) : d;
    }
    return t;
}

T Program::global_type(const std::string& mod, const std::string& name) {
    read("g:" + mod);
    auto it = globals.find(mod);
    if (it == globals.end()) return BOTTOM();
    auto jt = it->second.find(name);
    return jt == it->second.end() ? BOTTOM() : jt->second;
}

Fn* Program::find_method(const std::string& cq, const std::string& name) {
    for (auto& c : mro(cq)) {
        Fn** f = (*classes.get(c))->methods.get(name);
        if (f) return *f;
    }
    return nullptr;
}

const std::set<std::string>& Program::assigned_attrs(const std::string& cq) {
    auto it = assigned_attrs_.find(cq);
    if (it != assigned_attrs_.end()) return it->second;
    std::set<std::string> got;
    for (auto& [_, f] : (*classes.get(cq))->methods) {
        if (f->params.empty() || f->kind == "static") continue;
        const std::string& me = f->params[0];
        for (const AnyNode& n : walk(AnyNode(f->node))) {
            auto* at = as<Attribute>(n.expr());
            if (at && at->ctx == ExprCtx::Store)
                if (auto* v = as<pyfront::Name>(at->value))
                    if (v->id == me) got.insert(at->attr);
        }
    }
    return assigned_attrs_[cq] = std::move(got);
}

std::set<std::string> Program::assigned_attrs_any(const std::string& cq) {
    std::set<std::string> out = assigned_attrs(cq);
    for (auto& d : descendants(cq)) {
        auto& s = assigned_attrs(d);
        out.insert(s.begin(), s.end());
    }
    return out;
}

std::string Program::attr_owner(const std::string& cq, const std::string& attr) {
    if (!classes.has(cq)) return "";
    auto order = mro(cq);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        Class* c = *classes.get(*it);
        if (c->class_attrs.has(attr) || assigned_attrs(*it).count(attr)) return *it;
        auto ai = attrs.find(*it);
        if (ai != attrs.end() && ai->second.has(attr)) return *it;
    }
    return "";
}

std::optional<T> Program::attr_type(const std::string& cq, const std::string& attr) {
    for (auto& c : mro(cq)) read("a:" + c);
    read("ca");
    std::string owner = attr_owner(cq, attr);
    if (owner.empty()) {
        for (auto& d : descendants(cq))
            if (assigned_attrs(d).count(attr)) return top("attribute " + attr + " only set by a subclass");
        return std::nullopt;
    }
    auto pk = poisoned.find({owner, attr});
    if (pk != poisoned.end()) {
        std::string who = pk->second.substr(pk->second.rfind('.') + 1);
        return top("untyped:attribute " + attr + " is also stored a value of no single type (by " + who + ")");
    }
    T t = BOTTOM();
    auto ai = attrs.find(owner);
    if (ai != attrs.end())
        if (const T* x = ai->second.get(attr)) t = *x;
    for (auto& c : mro(cq)) {
        if ((*classes.get(c))->class_attrs.has(attr)) {
            auto it = class_attr_types.find(c + "\x1f" + attr);
            t = join(t, it == class_attr_types.end() ? BOTTOM() : it->second);
        }
    }
    return t;
}

T Program::class_attr_type(const std::string& cq, const std::string& attr) {
    read("ca");
    auto it = class_attr_types.find(cq + "\x1f" + attr);
    return it == class_attr_types.end() ? BOTTOM() : it->second;
}

const std::vector<A::Stmt*>* Program::block_of(const A::Stmt* st) const {
    auto it = blocks_.find(st);
    return it == blocks_.end() ? nullptr : it->second;
}

void Program::map_blocks(const A::FunctionDef* node) {
    for (const AnyNode& n : walk(AnyNode(node))) {
        std::vector<const std::vector<A::Stmt*>*> lists;
        if (const A::Stmt* s = n.stmt()) {
            lists = stmt_lists(s, false);
        } else if (n.cat == AnyNode::Handler) {
            lists.push_back(&static_cast<const ExceptHandler*>(n.p)->body);
        } else if (n.cat == AnyNode::MatchCase) {
            lists.push_back(&static_cast<const match_case*>(n.p)->body);
        }
        if (const A::Stmt* s = n.stmt())
            if (s->kind == StmtKind::Match) lists.clear();
        for (auto* l : lists)
            for (auto* s : *l) blocks_[s] = l;
    }
}

// ------------------------------------------------------------------------------------ containers
int Program::find(int s) {
    int root = s;
    while (parent[root] != root) root = parent[root];
    while (parent[s] != root) {
        int nx = parent[s];
        parent[s] = root;
        s = nx;
    }
    if (reads) reads->insert("s:" + std::to_string(root));
    return root;
}

T Program::settle(const T& old, const T& nw, int depth) {
    if (round <= 3 || nw == old) return nw;
    if (nw.kind == TK::Int && old.kind == TK::Int) return widen_int(old, nw);
    if ((nw.kind == TK::Tuple || nw.kind == TK::Opt) && old.kind == nw.kind && nw.args().size() == old.args().size()) {
        if (depth > 6) return top("a type that keeps growing");
        std::vector<T> a;
        for (size_t i = 0; i < nw.args().size(); i++) a.push_back(settle(old.args()[i], nw.args()[i], depth + 1));
        return T(nw.kind, a);
    }
    return nw;
}

void Program::touch(const std::string& key) {
    changed = true;
    dirty.insert(key);
}
void Program::read(const std::string& key) {
    if (reads) reads->insert(key);
}
T Program::ret(const std::string& fq) {
    read("r:" + fq);
    auto it = returns.find(fq);
    return it == returns.end() ? BOTTOM() : it->second;
}

const std::set<std::string>& Program::fn_locals(Fn* f) {
    auto it = fn_locals_.find(f->qname);
    if (it != fn_locals_.end()) return it->second;
    std::set<std::string> got = assigned_names(f->node);
    got.insert(f->params.begin(), f->params.end());
    got.insert(f->kwonly.begin(), f->kwonly.end());
    return fn_locals_[f->qname] = std::move(got);
}

const std::vector<AnyNode>& Program::access_nodes(AnyNode node) {
    auto it = access_.find(node.p);
    if (it != access_.end()) return it->second;
    std::vector<AnyNode> got, todo = {node}, kids;
    while (!todo.empty()) {
        AnyNode n = todo.back();
        todo.pop_back();
        if (const A::Expr* e = n.expr()) {
            auto k = e->kind;
            if (k == ExprKind::Lambda || k == ExprKind::ListComp || k == ExprKind::GeneratorExp || k == ExprKind::SetComp ||
                k == ExprKind::DictComp)
                continue;
            if (k == ExprKind::Subscript || k == ExprKind::Call) got.push_back(n);
        }
        kids.clear();
        child_nodes(n, kids);
        todo.insert(todo.end(), kids.begin(), kids.end());
    }
    std::reverse(got.begin(), got.end());
    return access_[node.p] = std::move(got);
}

int Program::new_site(const void* node, TK kind, std::pair<std::string, int> where) {
    auto it = site_of_node.find(node);
    if (it != site_of_node.end()) return it->second;
    int s = (int)parent.size();
    site_of_node[node] = s;
    parent.push_back(s);
    ckind.push_back(kind);
    elem.push_back(BOTTOM());
    keys.push_back(BOTTOM());
    fields.emplace_back();
    required.emplace_back();
    site_where.push_back(std::move(where));
    site_node.push_back(node);
    touch("s:" + std::to_string(s));
    return s;
}

static std::string kname(TK k) { return kind_name(k); }

int Program::unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return a;
    if (a > b) std::swap(a, b);
    if (site_frozen(a)) {   // (new) a frozen container may only absorb one that adds nothing
        bool adds = site_frozen(b) || (elem[b].kind != TK::Bottom && !(elem[b] == elem[a])) ||
                    (keys[b].kind != TK::Bottom && !(keys[b] == keys[a]));
        for (auto& [k, t] : fields[b])
            if (!fields[a].has(k) || !(*fields[a].get(k) == t)) adds = true;
        if (adds) {
            veto("joins two containers whose item types are fixed by other code");
            return a;
        }
    }
    parent[b] = a;
    touch("s:" + std::to_string(a));
    touch("s:" + std::to_string(b));
    if (ckind[a] != ckind[b]) dynamic[a] = "a " + kname(ckind[a]) + " and a " + kname(ckind[b]) + " flow together";
    elem[a] = join(elem[a], elem[b]);
    keys[a] = join(keys[a], keys[b]);
    OMap<T> fb = fields[b];
    for (auto& [k, t] : fb) {
        T cur = fields[a].has(k) ? *fields[a].get(k) : BOTTOM();
        fields[a].set(k, join(cur, t));
    }
    auto ra = required[a], rb = required[b];
    if (!ra) required[a] = rb;
    else if (rb) {
        std::set<std::string> both;
        for (auto& k : *ra)
            if (rb->count(k)) both.insert(k);
        required[a] = both;
    }
    if (dynamic.count(b) && !dynamic.count(a)) dynamic[a] = dynamic[b];
    auto hb = hints.find(b);
    if (hb != hints.end()) {
        auto moved = hb->second;
        hints.erase(b);
        auto& ha = hints[a];
        ha.insert(ha.end(), moved.begin(), moved.end());
    } else {
        hints[a];
    }
    if (fixed.count(b)) fixed.insert(a);
    if (mutated.count(b)) mutated.insert(a);
    if (poisoned_sites.count(b) && !poisoned_sites.count(a)) poisoned_sites[a] = poisoned_sites[b];
    return a;
}

void Program::mark_mutated(int s) {
    s = find(s);
    if (!mutated.count(s) && site_frozen(s)) {
        veto("changes a record that other code keeps as a value");
        return;
    }
    if (!mutated.count(s)) {
        mutated.insert(s);
        touch("s:" + std::to_string(s));
    }
}

void Program::grow(std::vector<T>& table, int s, const T& t) {
    s = find(s);
    if (t.kind == TK::Top && !suspend && !poisoned_sites.count(s)) {
        if (site_frozen(s)) {
            veto("adds an item of no single type to a container other code reads");
            return;
        }
        poisoned_sites[s] = t.why().empty() ? "?" : t.why();
        touch("s:" + std::to_string(s));
    }
    if (suspend || t.kind == TK::Top) return;
    T old = table[s];
    if (site_frozen(s)) {   // (new) the items of a frozen container: only what fits them
        if (old.kind != TK::Bottom && !(join(old, t) == old)) veto("adds a " + t.repr() + " to a container of " + old.repr() + " fixed by other code");
        else if (old.kind == TK::Bottom && t.kind != TK::Bottom) veto("fills a container other code keeps empty");
        return;
    }
    T nw = settle(old, join(old, t));
    if (nw != old) {
        table[s] = nw;
        touch("s:" + std::to_string(s));
    }
}

void Program::grow_field(int s, const std::string& key, const T& t) {
    s = find(s);
    if (t.kind == TK::Top) {
        if (!fields[s].has(key)) fields[s].set(key, BOTTOM());
        if (!suspend && !poisoned_sites.count(s)) {
            poisoned_sites[s] = t.why().empty() ? "?" : t.why();
            touch("s:" + std::to_string(s));
        }
        return;
    }
    if (suspend) {
        if (!fields[s].has(key) && !site_frozen(s)) fields[s].set(key, BOTTOM());
        return;
    }
    bool had = fields[s].has(key);
    if (site_frozen(s)) {   // (new) a frozen record: its fields stay as they are
        if (!had || !(join(*fields[s].get(key), t) == *fields[s].get(key))) veto("changes field " + key + " of a record fixed by other code");
        return;
    }
    T old = had ? *fields[s].get(key) : BOTTOM();
    T nw = settle(old, join(old, t));
    if (!had || nw != old) {
        fields[s].set(key, nw);
        touch("s:" + std::to_string(s));
    }
    grow(elem, s, t);
    grow(keys, s, STR());
}

void Program::created(int s, const std::vector<std::string>& ks) {
    s = find(s);
    if (suspend) return;
    if (site_frozen(s)) {
        if (required[s])
            for (auto& k : *required[s])
                if (std::find(ks.begin(), ks.end(), k) == ks.end()) {
                    veto("makes a record without its field " + k);
                    return;
                }
        return;
    }
    std::set<std::string> k(ks.begin(), ks.end());
    auto old = required[s];
    std::set<std::string> nw;
    if (!old) nw = k;
    else
        for (auto& x : *old)
            if (k.count(x)) nw.insert(x);
    if (!old || nw != *old) {
        required[s] = nw;
        touch("s:" + std::to_string(s));
    }
}

void Program::make_dynamic(int s, const std::string& why) {
    s = find(s);
    if (!dynamic.count(s) && site_frozen(s) && ckind[s] == TK::Dict) {
        veto("uses a record as a dict (" + why + ")");
        return;
    }
    if (!dynamic.count(s)) {
        dynamic[s] = why;
        touch("s:" + std::to_string(s));
    }
}

bool Program::is_record(int s) {
    s = find(s);
    return ckind[s] == TK::Dict && !dynamic.count(s);
}

void Program::hint(int s, const std::string& name) {
    s = find(s);
    auto& h = hints[s];
    if (std::find(h.begin(), h.end(), name) == h.end()) h.push_back(name);
}

// ------------------------------------------------------------------------------------ the join
std::string tname(const T& t) {
    if (t.kind == TK::List || t.kind == TK::Dict || t.kind == TK::Set) return kind_name(t.kind);
    if (t.kind == TK::Inst) {
        auto p = t.name().rfind('.');
        return p == std::string::npos ? t.name() : t.name().substr(p + 1);
    }
    return kind_name(t.kind);
}

T Program::join(const T& a, const T& b) {
    if (a == b) return a;
    TK ka = a.kind, kb = b.kind;
    if (ka == TK::Bottom) return b;
    if (kb == TK::Bottom) return a;
    if (ka == TK::Top) return a;
    if (kb == TK::Top) return b;
    if (ka == TK::None || kb == TK::None) {
        const T& other = ka == TK::None ? b : a;
        if (other.kind == TK::Opt) return other;
        return opt(other);
    }
    if (ka == TK::Opt || kb == TK::Opt) {
        T j = join(strip_opt(a), strip_opt(b));
        return j.kind == TK::Top ? j : opt(j);
    }
    if (demand_enabled && !no_variants && variant_ok(a) && variant_ok(b) && (ka != kb || ka == TK::Variant)) return variant_join(a, b);   // (new)
    if (ka != kb) {
        if ((ka == TK::Bytes && kb == TK::Bytearray) || (ka == TK::Bytearray && kb == TK::Bytes)) return top("bytes and bytearray mixed");
        return top(tname(a) + " and " + tname(b) + " mixed");
    }
    switch (ka) {
        case TK::Int: return int_join(a, b);
        case TK::Bytes:
        case TK::Bytearray: {
            OBig hi;
            if (a.hi() && b.hi()) hi = std::max(*a.hi(), *b.hi());
            return T(ka, {}, {}, hi);
        }
        case TK::Tuple: {
            if (a.args().size() != b.args().size())
                return top("tuples of " + std::to_string(a.args().size()) + " and " + std::to_string(b.args().size()) + " items mixed");
            std::vector<T> parts;
            for (size_t i = 0; i < a.args().size(); i++) parts.push_back(join(a.args()[i], b.args()[i]));
            for (auto& p : parts)
                if (p.kind == TK::Top) return p;
            return tuple_of(parts);
        }
        case TK::List:
        case TK::Dict:
        case TK::Set: return container(ka, unite(a.site, b.site));
        case TK::Inst: {
            std::string c = common_base(a.name(), b.name());
            return !c.empty() ? named(TK::Inst, c) : top("instances of " + a.name() + " and " + b.name() + " mixed");
        }
        default: return top(tname(a) + " and " + tname(b) + " mixed");
    }
}

// (new) tagged variants: scalars of different kinds meeting in one place
bool variant_ok(const T& t) {
    switch (t.kind) {
        case TK::Int:
        case TK::Str:
        case TK::Float:
        case TK::Bool:
        case TK::Bytes:
        case TK::Variant: return true;
        default: return false;
    }
}

T Program::variant_join(const T& a, const T& b) {
    std::vector<T> alts;
    auto add = [&](const T& x) {
        for (auto& y : alts)
            if (y.kind == x.kind) {
                if (x.kind == TK::Int) y = int_join(y, x);
                else if (x.kind == TK::Bytes) y = BYTES();
                return;
            }
        alts.push_back(x);
    };
    for (const T* t : {&a, &b}) {
        if (t->kind == TK::Variant)
            for (auto& x : t->args()) add(x);
        else add(*t);
    }
    if (alts.size() > 4) return top(tname(a) + " and " + tname(b) + " mixed");
    std::sort(alts.begin(), alts.end(), [](const T& x, const T& y) { return (int)x.kind < (int)y.kind; });
    if (alts.size() == 1) return alts[0];
    return T(TK::Variant, alts);
}

std::string Program::common_base(const std::string& x, const std::string& y) {
    auto anc = mro(x);
    for (auto& c : mro(y))
        if (std::find(anc.begin(), anc.end(), c) != anc.end()) return c;
    return "";
}

std::vector<std::string> Program::mro(const std::string& cq) {
    std::vector<std::string> out;
    std::vector<std::string> todo = {cq};
    size_t i = 0;
    while (i < todo.size()) {
        std::string c = todo[i++];
        if (std::find(out.begin(), out.end(), c) != out.end() || !classes.has(c)) continue;
        out.push_back(c);
        for (auto& b : (*classes.get(c))->bases) todo.push_back(b);
    }
    return out;
}

std::vector<std::string> Program::descendants(const std::string& cq) {
    std::vector<std::string> out, todo = {cq};
    while (!todo.empty()) {
        std::string c = todo.back();
        todo.pop_back();
        for (auto& s : (*classes.get(c))->subclasses)
            if (std::find(out.begin(), out.end(), s) == out.end()) {
                out.push_back(s);
                todo.push_back(s);
            }
    }
    return out;
}

const pyfront::Attribute* Program::getattr_attr(const pyfront::Call* n, const std::string& attr) {
    auto& slot = getattr_synth[n];
    if (!slot) {
        slot = std::make_unique<pyfront::Attribute>();
        slot->value = n->args[0];
        slot->attr = attr;
        slot->pos = n->pos;
    }
    return slot.get();
}

int Program::getattr_choice(const std::string& cq, const std::string& attr) {
    auto self_store = [&](const A::Expr* t) {
        auto* a = dynamic_cast<const pyfront::Attribute*>(t);
        auto* s = a ? dynamic_cast<const pyfront::Name*>(a->value) : nullptr;
        return s && s->id == "self" && a->attr == attr;
    };
    std::vector<std::string> up = mro(cq);
    for (auto& c : up) {
        Class* const* cl = classes.get(c);
        if (!cl || !(*cl)->foreign_base.empty()) return 0;
    }
    if (find_method(cq, attr)) return 1;
    for (auto& c : up) {
        Class* k = *classes.get(c);
        if (k->class_attrs.has(attr)) return 1;
        Fn* const* init = k->methods.get("__init__");
        if (init && (*init)->node)
            for (auto* s : (*init)->node->body) {   // (set at the top of __init__: always there after it)
                if (auto* as = dynamic_cast<const pyfront::Assign*>(s)) {
                    for (auto* t : as->targets)
                        if (self_store(t)) return 1;
                } else if (auto* an = dynamic_cast<const pyfront::AnnAssign*>(s)) {
                    if (an->value && self_store(an->target)) return 1;
                }
            }
    }
    // never there: nothing up or down the hierarchy has it or stores it, and no fact says otherwise
    if (attr_type(cq, attr)) return 0;
    std::vector<std::string> all = up, todo{cq};
    while (!todo.empty()) {
        std::string c = todo.back();
        todo.pop_back();
        Class* const* cl = classes.get(c);
        if (!cl) continue;
        for (auto& s : (*cl)->subclasses)
            if (std::find(all.begin(), all.end(), s) == all.end()) {
                all.push_back(s);
                todo.push_back(s);
            }
    }
    for (auto& c : all) {
        Class* const* cl = classes.get(c);
        if (!cl) return 0;
        Class* k = *cl;
        if (k->methods.has(attr) || k->class_attrs.has(attr) || !k->foreign_base.empty()) return 0;
        bool stored = false;
        if (k->node)
            walk_each(AnyNode(k->node), [&](const AnyNode& x) {
                auto* a = dynamic_cast<const pyfront::Attribute*>(x.expr());
                if (a && a->attr == attr && a->ctx == pyfront::ExprCtx::Store) stored = true;
            });
        if (stored) return 0;
    }
    return 2;
}

int Program::apply_type_hints() {
    int n = 0;
    for (auto& [q, ps] : type_hints) {
        Fn* const* fp = fns.get(q);
        if (!fp) continue;
        Fn* f = *fp;
        for (auto& [name, text] : ps) {
            if (std::find(f->params.begin(), f->params.end(), name) == f->params.end() &&
                std::find(f->kwonly.begin(), f->kwonly.end(), name) == f->kwonly.end())
                continue;
            if (f->annotations.has(name)) continue;
            auto pit = params.find(q);
            if (pit != params.end() && pit->second.count(name) && pit->second[name].kind != TK::Bottom &&
                pit->second[name].kind != TK::None)
                continue;   // (the classic rules typed it: its contract stands)
            auto pr = pyfront::parse("_hint: " + text + "\n", "<hint " + q + ">");
            if (!pr || pr.module->body.empty()) continue;
            auto* aa = dynamic_cast<const pyfront::AnnAssign*>(pr.module->body[0]);
            if (!aa) continue;
            f->annotations.set(name, aa->annotation);
            hint_trees.push_back(std::move(pr.module));
            touch("p:" + q);
            n++;
        }
    }
    n += apply_stub_hints();
    return n;
}

// stub text: every bare name a stub imports (from codebreaker.x import Y) is spelled out in full, so it
// resolves from any function's module (names after a '.' are left alone)
static std::string qualify_stub_names(const std::string& t, const std::map<std::string, std::string>& alias) {
    std::string out;
    size_t i = 0;
    auto idc = [](char c) { return std::isalnum((unsigned char)c) || c == '_'; };
    while (i < t.size()) {
        if (idc(t[i]) && !std::isdigit((unsigned char)t[i])) {
            size_t j = i;
            while (j < t.size() && idc(t[j])) j++;
            std::string w = t.substr(i, j - i);
            bool dotted = i > 0 && t[i - 1] == '.';
            auto it = alias.find(w);
            out += (!dotted && it != alias.end()) ? it->second : w;
            i = j;
        } else if (t[i] == '\'' || t[i] == '"') {   // a string: copied
            size_t j = t.find(t[i], i + 1);
            if (j == std::string::npos) j = t.size() - 1;
            out += t.substr(i, j - i + 1);
            i = j + 1;
        } else {
            out += t[i++];
        }
    }
    return out;
}

void Program::load_stub(const std::string& module, const std::string& source_) {
    // MonkeyType on Python 3.14 writes typing.Union as <class 'Union'>[...]: read it as Union[...]
    std::string source = source_;
    for (size_t at = 0; (at = source.find("<class '", at)) != std::string::npos;) {
        size_t end = source.find("'>", at);
        size_t nl = source.find('\n', at);
        if (end == std::string::npos || (nl != std::string::npos && nl < end)) {
            at += 8;
            continue;
        }
        std::string nm = source.substr(at + 8, end - at - 8);
        source.replace(at, end + 2 - at, nm);
    }
    auto pr = pyfront::parse(source, module + ".pyi");
    if (!pr) return;
    stub_files++;
    std::map<std::string, std::string> alias;
    for (auto* st : pr.module->body)
        if (auto* im = as<ImportFrom>(st))
            if (im->level == 0 && im->has_module && im->module != "typing" && im->module != "__future__")
                for (auto* a : im->names) alias[a->has_asname ? a->asname : a->name] = im->module + "." + a->name;
    std::function<void(const std::vector<Stmt*>&, const std::string&)> walk = [&](const std::vector<Stmt*>& body, const std::string& prefix) {
        for (auto* st : body) {
            if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) {
                auto* fd = cast<FunctionDef>(st);
                std::string q = prefix + "." + fd->name;
                auto add = [&](const arg* x) {
                    if (x->annotation) stub_hints[q][x->arg] = qualify_stub_names(unparse(x->annotation), alias);
                };
                for (auto* x : fd->args->posonlyargs) add(x);
                for (auto* x : fd->args->args) add(x);
                for (auto* x : fd->args->kwonlyargs) add(x);
            } else if (auto* cd = as<ClassDef>(st)) {
                walk(cd->body, prefix + "." + cd->name);
            }
        }
    };
    walk(pr.module->body, module);
    hint_trees.push_back(std::move(pr.module));
}

int Program::apply_stub_hints() {
    int n = 0;
    for (auto& [q, ps] : stub_hints) {
        Fn* const* fp = fns.get(q);
        if (!fp) continue;
        Fn* f = *fp;
        for (auto& [name, text] : ps) {
            if (name == "self" || name == "cls") continue;
            StubNote note{q, name, text, "", ""};
            auto done = [&](const char* st, const std::string& d) {
                note.status = st;
                note.detail = d;
                stub_notes.push_back(note);
            };
            if (std::find(f->params.begin(), f->params.end(), name) == f->params.end() &&
                std::find(f->kwonly.begin(), f->kwonly.end(), name) == f->kwonly.end()) {
                done("ignored", "no such parameter");
                continue;
            }
            auto th = type_hints.find(q);
            if (th != type_hints.end() && th->second.count(name)) {
                done("ignored", "types.json hints it");
                continue;
            }
            if (f->annotations.has(name)) {
                done("ignored", "annotated in the source");
                continue;
            }
            auto pr = pyfront::parse("_hint: " + text + "\n", "<stub hint " + q + ">");
            const pyfront::AnnAssign* aa = (pr && !pr.module->body.empty()) ? dynamic_cast<const pyfront::AnnAssign*>(pr.module->body[0]) : nullptr;
            if (!aa) {
                done("ignored", "the engine cannot read the type text");
                continue;
            }
            const A::Expr* ann_node = aa->annotation;
            hint_trees.push_back(std::move(pr.module));
            T t = ann_to_type(f->module, ann_node);
            if (t.kind == TK::Bottom) {
                done("ignored", "type the engine cannot model");
                continue;
            }
            if (t.kind == TK::Top) {
                done("ignored", "no single type: " + t.why());
                continue;
            }
            auto pit = params.find(q);
            if (pit != params.end() && pit->second.count(name) && pit->second[name].kind != TK::Bottom &&
                pit->second[name].kind != TK::None) {
                auto plain = [](const T& x) {   // the type's text without the container sites' numbers
                    std::string r = x.repr(), o;
                    for (size_t i = 0; i < r.size(); i++) {
                        if (r[i] == '#') {
                            while (i + 1 < r.size() && std::isdigit((unsigned char)r[i + 1])) i++;
                        } else o += r[i];
                    }
                    return o;
                };
                std::string inferred = plain(pit->second[name]);
                if (inferred == plain(t)) done("agree", inferred);
                else done("conflict", "inference wins: " + inferred + " (stub " + plain(t) + ")");
                continue;
            }
            f->annotations.set(name, ann_node);
            touch("p:" + q);
            done("applied", t.repr());
            n++;
        }
    }
    return n;
}

void Program::freeze() {
    frozen = true;
    frozen_sites = (int)parent.size();
    frozen_params = params;
    frozen_attrs = attrs;
    // the functions whose callers disagree on a parameter's kind
    for (auto& [q, f] : fns) {
        if ((f->kind != "function" && f->kind != "static") || !f->varargs.empty() || !f->kwargs.empty() || f->generator) continue;
        auto pi = params.find(q);
        if (pi == params.end()) continue;
        for (auto& [name, t] : pi->second)
            if (t.kind == TK::Top && t.why().find(" mixed") != std::string::npos) spec_candidates.insert(q);
    }
    // a fact the classic rules left with no single type is read by no translated function: it is
    // recomputed under the new rules (facts only grow, so it would stay so otherwise)
    for (auto& [q, t] : returns)
        if (t.kind == TK::Top) t = BOTTOM();
    for (auto& [q, ps] : params)
        for (auto& [n, t] : ps)
            if (t.kind == TK::Top) t = BOTTOM();
    for (auto& [c, as] : attrs)
        for (auto& [n, t] : as)
            if (t.kind == TK::Top) t = BOTTOM();
    frozen_params = params;
    frozen_attrs = attrs;
    for (int s = 1; s < (int)parent.size(); s++) {
        if (parent[s] != s) continue;
        bool any = elem[s].kind == TK::Top || keys[s].kind == TK::Top;
        for (auto& [k, t] : fields[s])
            if (t.kind == TK::Top) {
                any = true;
                t = BOTTOM();
            }
        if (any) {
            if (elem[s].kind == TK::Top) elem[s] = BOTTOM();
            if (keys[s].kind == TK::Top) keys[s] = BOTTOM();
            unfrozen.insert(s);
        }
    }
}

T Program::canon(const T& t) {
    if (t.kind == TK::List || t.kind == TK::Dict || t.kind == TK::Set) {
        int r = find(t.site);
        return r == t.site ? t : container(t.kind, r);
    }
    if (!t.args().empty()) {
        std::vector<T> a;
        for (auto& x : t.args()) a.push_back(canon(x));
        return T(t.kind, a, t.lo(), t.hi(), t.name(), t.site, t.why());
    }
    return t;
}

// ------------------------------------------------------------------------------------ names
std::vector<std::string> target_names(const A::Expr* t) {
    if (auto* n = as<pyfront::Name>(t)) return {n->id};
    std::vector<std::string> out;
    const std::vector<pyfront::Expr*>* elts = nullptr;
    if (auto* tu = as<pyfront::Tuple>(t)) elts = &tu->elts;
    else if (auto* l = as<pyfront::List>(t)) elts = &l->elts;
    if (elts)
        for (auto* e : *elts) {
            auto sub = target_names(e);
            out.insert(out.end(), sub.begin(), sub.end());
        }
    return out;
}

std::set<std::string> assigned_names(const A::FunctionDef* fn) {
    std::set<std::string> out;
    std::vector<AnyNode> todo, kids;
    for (auto* s : fn->body) todo.emplace_back(s);
    while (!todo.empty()) {
        AnyNode n = todo.back();
        todo.pop_back();
        if (const A::Stmt* s = n.stmt()) {
            if (s->kind == StmtKind::FunctionDef || s->kind == StmtKind::AsyncFunctionDef) {
                out.insert(cast<FunctionDef>(s)->name);
                continue;
            }
            if (s->kind == StmtKind::ClassDef) {
                out.insert(cast<ClassDef>(s)->name);
                continue;
            }
            if (auto* im = as<Import>(s))
                for (auto* a : im->names) {
                    std::string l = a->has_asname ? a->asname : a->name;
                    out.insert(l.substr(0, l.find('.')));
                }
            if (auto* im = as<ImportFrom>(s))
                for (auto* a : im->names) {
                    std::string l = a->has_asname ? a->asname : a->name;
                    out.insert(l.substr(0, l.find('.')));
                }
        }
        if (const A::Expr* e = n.expr()) {
            if (e->kind == ExprKind::Lambda) continue;
            if (auto* nm = as<pyfront::Name>(e))
                if (nm->ctx == ExprCtx::Store || nm->ctx == ExprCtx::Del) out.insert(nm->id);
            if (e->kind == ExprKind::ListComp || e->kind == ExprKind::SetComp || e->kind == ExprKind::DictComp ||
                e->kind == ExprKind::GeneratorExp)
                continue;
        }
        if (n.cat == AnyNode::Handler) {
            auto* h = static_cast<const ExceptHandler*>(n.p);
            if (h->has_name) out.insert(h->name);
        }
        kids.clear();
        child_nodes(n, kids);
        todo.insert(todo.end(), kids.begin(), kids.end());
    }
    return out;
}

}  // namespace pyengine
