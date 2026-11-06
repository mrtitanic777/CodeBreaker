// lib_ast.cc -- the ast module over native/pyfront (a parser whose trees are CPython's, node for node):
// ast.parse gives the same nodes, with the same fields and positions, as CPython's; walk,
// iter_child_nodes, iter_fields, get_source_segment, get_docstring, dump, literal_eval, NodeVisitor and
// NodeTransformer as the module's. (The tree crosses from pyfront through its canonical dump text.)
#include <cstring>
#include <deque>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <set>

#include "pyfront/pyfront.hpp"
#include "rt.hpp"

namespace py {

var coroutine_type_();                    // gen.cc
var async_generator_type_();
bool is_coroutine_function_(const var& f, int kind);

static var g_ast;   // the module
static std::map<std::string, TypeObj*>* g_node_types = new std::map<std::string, TypeObj*>();

// every node class: its fields (CPython 3.14's _fields) and its base
struct NodeSpec {
    const char* name;
    const char* base;
    const char* fields;   // space separated; a trailing * marks a list field
};
static const NodeSpec kNodes[] = {
    {"Module", "mod", "body* type_ignores*"}, {"Interactive", "mod", "body*"}, {"Expression", "mod", "body"},
    {"FunctionDef", "stmt", "name args body* decorator_list* returns type_comment type_params*"},
    {"AsyncFunctionDef", "stmt", "name args body* decorator_list* returns type_comment type_params*"},
    {"ClassDef", "stmt", "name bases* keywords* body* decorator_list* type_params*"}, {"Return", "stmt", "value"},
    {"Delete", "stmt", "targets*"}, {"Assign", "stmt", "targets* value type_comment"}, {"TypeAlias", "stmt", "name type_params* value"},
    {"AugAssign", "stmt", "target op value"}, {"AnnAssign", "stmt", "target annotation value simple"},
    {"For", "stmt", "target iter body* orelse* type_comment"}, {"AsyncFor", "stmt", "target iter body* orelse* type_comment"},
    {"While", "stmt", "test body* orelse*"}, {"If", "stmt", "test body* orelse*"}, {"With", "stmt", "items* body* type_comment"},
    {"AsyncWith", "stmt", "items* body* type_comment"}, {"Match", "stmt", "subject cases*"}, {"Raise", "stmt", "exc cause"},
    {"Try", "stmt", "body* handlers* orelse* finalbody*"}, {"TryStar", "stmt", "body* handlers* orelse* finalbody*"},
    {"Assert", "stmt", "test msg"}, {"Import", "stmt", "names*"}, {"ImportFrom", "stmt", "module names* level"},
    {"Global", "stmt", "names*"}, {"Nonlocal", "stmt", "names*"}, {"Expr", "stmt", "value"}, {"Pass", "stmt", ""},
    {"Break", "stmt", ""}, {"Continue", "stmt", ""},
    {"BoolOp", "expr", "op values*"}, {"NamedExpr", "expr", "target value"}, {"BinOp", "expr", "left op right"},
    {"UnaryOp", "expr", "op operand"}, {"Lambda", "expr", "args body"}, {"IfExp", "expr", "test body orelse"},
    {"Dict", "expr", "keys* values*"}, {"Set", "expr", "elts*"}, {"ListComp", "expr", "elt generators*"},
    {"SetComp", "expr", "elt generators*"}, {"DictComp", "expr", "key value generators*"}, {"GeneratorExp", "expr", "elt generators*"},
    {"Await", "expr", "value"}, {"Yield", "expr", "value"}, {"YieldFrom", "expr", "value"},
    {"Compare", "expr", "left ops* comparators*"}, {"Call", "expr", "func args* keywords*"},
    {"FormattedValue", "expr", "value conversion format_spec"}, {"Interpolation", "expr", "value str conversion format_spec"},
    {"JoinedStr", "expr", "values*"}, {"TemplateStr", "expr", "values*"}, {"Constant", "expr", "value kind"},
    {"Attribute", "expr", "value attr ctx"}, {"Subscript", "expr", "value slice ctx"}, {"Starred", "expr", "value ctx"},
    {"Name", "expr", "id ctx"}, {"List", "expr", "elts* ctx"}, {"Tuple", "expr", "elts* ctx"}, {"Slice", "expr", "lower upper step"},
    {"Load", "expr_context", ""}, {"Store", "expr_context", ""}, {"Del", "expr_context", ""},
    {"And", "boolop", ""}, {"Or", "boolop", ""},
    {"Add", "operator", ""}, {"Sub", "operator", ""}, {"Mult", "operator", ""}, {"MatMult", "operator", ""}, {"Div", "operator", ""},
    {"Mod", "operator", ""}, {"Pow", "operator", ""}, {"LShift", "operator", ""}, {"RShift", "operator", ""}, {"BitOr", "operator", ""},
    {"BitXor", "operator", ""}, {"BitAnd", "operator", ""}, {"FloorDiv", "operator", ""},
    {"Invert", "unaryop", ""}, {"Not", "unaryop", ""}, {"UAdd", "unaryop", ""}, {"USub", "unaryop", ""},
    {"Eq", "cmpop", ""}, {"NotEq", "cmpop", ""}, {"Lt", "cmpop", ""}, {"LtE", "cmpop", ""}, {"Gt", "cmpop", ""}, {"GtE", "cmpop", ""},
    {"Is", "cmpop", ""}, {"IsNot", "cmpop", ""}, {"In", "cmpop", ""}, {"NotIn", "cmpop", ""},
    {"comprehension", "AST", "target iter ifs* is_async"}, {"ExceptHandler", "excepthandler", "type name body*"},
    {"arguments", "AST", "posonlyargs* args* vararg kwonlyargs* kw_defaults* kwarg defaults*"},
    {"arg", "AST", "arg annotation type_comment"}, {"keyword", "AST", "arg value"}, {"alias", "AST", "name asname"},
    {"withitem", "AST", "context_expr optional_vars"}, {"match_case", "AST", "pattern guard body*"},
    {"MatchValue", "pattern", "value"}, {"MatchSingleton", "pattern", "value"}, {"MatchSequence", "pattern", "patterns*"},
    {"MatchMapping", "pattern", "keys* patterns* rest"}, {"MatchClass", "pattern", "cls patterns* kwd_attrs* kwd_patterns*"},
    {"MatchStar", "pattern", "name"}, {"MatchAs", "pattern", "pattern name"}, {"MatchOr", "pattern", "patterns*"},
    {"TypeVar", "type_param", "name bound default_value"}, {"ParamSpec", "type_param", "name default_value"},
    {"TypeVarTuple", "type_param", "name default_value"}, {"TypeIgnore", "type_ignore", "lineno tag"},
};
static std::map<std::string, std::vector<std::pair<std::string, bool>>>* g_fields = new std::map<std::string, std::vector<std::pair<std::string, bool>>>();

static TypeObj* node_type(const std::string& name) {
    auto it = g_node_types->find(name);
    return it == g_node_types->end() ? nullptr : it->second;
}

// ------------------------------------------------------------------------------------------ the dump text, read back
struct DumpReader {
    const std::string& s;
    size_t i = 0;
    [[noreturn]] void fail() { raise(E_SyntaxError, "ast: unreadable tree text at " + std::to_string(i) + ": " + s.substr(i > 60 ? i - 60 : 0, 90)); }
    void ws() { while (i < s.size() && (s[i] == ' ' || s[i] == '\n')) i++; }
    bool eat(char c) { ws(); if (i < s.size() && s[i] == c) { i++; return true; } return false; }
    std::string ident() {
        ws();
        size_t st = i;
        while (i < s.size() && (isalnum((unsigned char)s[i]) || s[i] == '_')) i++;
        return s.substr(st, i - st);
    }
    std::string quoted(char q) {
        // a Python repr's string body: \\ \' \" \n \r \t \xhh \uhhhh \Uhhhhhhhh
        std::string o;
        while (i < s.size() && s[i] != q) {
            char c = s[i++];
            if (c != '\\') { o.push_back(c); continue; }
            char e = s[i++];
            switch (e) {
            case 'n': o.push_back('\n'); break;
            case 'r': o.push_back('\r'); break;
            case 't': o.push_back('\t'); break;
            case '\\': o.push_back('\\'); break;
            case '\'': o.push_back('\''); break;
            case '"': o.push_back('"'); break;
            case 'x': { uint32_t v = (uint32_t)std::stoul(s.substr(i, 2), nullptr, 16); i += 2; append_utf8(o, v); break; }
            case 'u': { uint32_t v = (uint32_t)std::stoul(s.substr(i, 4), nullptr, 16); i += 4; append_utf8(o, v); break; }
            case 'U': { uint32_t v = (uint32_t)std::stoul(s.substr(i, 8), nullptr, 16); i += 8; append_utf8(o, v); break; }
            default: o.push_back('\\'); o.push_back(e);
            }
        }
        i++;
        return o;
    }
    std::string quoted_bytes(char q) {
        std::string o;
        while (i < s.size() && s[i] != q) {
            char c = s[i++];
            if (c != '\\') { o.push_back(c); continue; }
            char e = s[i++];
            switch (e) {
            case 'n': o.push_back('\n'); break;
            case 'r': o.push_back('\r'); break;
            case 't': o.push_back('\t'); break;
            case '\\': o.push_back('\\'); break;
            case '\'': o.push_back('\''); break;
            case '"': o.push_back('"'); break;
            case 'x': o.push_back((char)std::stoul(s.substr(i, 2), nullptr, 16)); i += 2; break;
            default: o.push_back('\\'); o.push_back(e);
            }
        }
        i++;
        return o;
    }
    var value() {
        ws();
        char c = s[i];
        if (c == '[') {
            i++;
            std::vector<var> v;
            if (eat(']')) return new_list(v);
            for (;;) {
                v.push_back(value());
                if (eat(',')) continue;
                if (eat(']')) return new_list(v);
                fail();
            }
        }
        if (c == '\'' || c == '"') { i++; return new_str(quoted(c)); }
        if (c == 'b' && i + 1 < s.size() && (s[i + 1] == '\'' || s[i + 1] == '"')) { char q = s[i + 1]; i += 2; return new_bytes(quoted_bytes(q)); }
        if (c == '-' || isdigit((unsigned char)c)) {
            size_t st = i;
            if (s[i] == '-') i++;
            while (i < s.size() && (isalnum((unsigned char)s[i]) || s[i] == '.' || s[i] == '+' || (s[i] == '-' && (s[i - 1] == 'e' || s[i - 1] == 'E')))) i++;
            std::string t = s.substr(st, i - st);
            if (t.find_first_of(".eE") != std::string::npos && t.find_first_of("xX") == std::string::npos) return var(std::strtod(t.c_str(), nullptr));
            if (!t.empty() && (t.back() == 'j' || t.back() == 'J')) return var(std::strtod(t.c_str(), nullptr));
            return int_from(t, 10);
        }
        if (s.compare(i, 3, "...") == 0) { i += 3; return Ellipsis; }
        std::string id = ident();
        if (id == "None") return None;
        if (id == "Ellipsis") return Ellipsis;   // (a `...` body: Constant(value=Ellipsis), as ast.dump writes it)
        if (id == "True") return true;
        if (id == "False") return false;
        if (id == "inf") return var(HUGE_VAL);
        if (!eat('(')) fail();
        return node(id);
    }
    var node(const std::string& cls) {
        TypeObj* t = node_type(cls);
        if (!t) fail();
        auto* o = new Obj();
        o->type = t;
        var n(o);
        // the fields left out of the text (None, or an empty list)
        for (auto& [f, is_list] : (*g_fields)[cls]) n.setattr(f.c_str(), is_list ? new_list() : var(None));
        if (eat(')')) return n;
        for (;;) {
            std::string f = ident();
            if (!eat('=')) fail();
            n.setattr(f.c_str(), value());
            if (eat(',')) continue;
            if (eat(')')) return n;
            fail();
        }
    }
};

static var parse_source(const std::string& src, const std::string& filename) {
    auto r = pyfront::parse(src, filename);
    if (!r) {
        var e = make_exc(E_SyntaxError, r.error->msg);
        e.setattr("lineno", var((int64_t)r.error->lineno));
        e.setattr("offset", var((int64_t)r.error->offset));
        e.setattr("filename", new_str(filename));
        e.setattr("msg", new_str(r.error->msg));
        raise(e);
    }
    std::string text = pyfront::dump(*r.module);
    DumpReader d{text};
    return d.value();
}

static std::vector<var> child_nodes(const var& n) {
    std::vector<var> out;
    TypeObj* t = type_of(n);
    auto it = g_fields->find(t->name);
    if (it == g_fields->end()) return out;
    TypeObj* ast_t = node_type("AST");
    for (auto& [f, is_list] : it->second) {
        var v = getattr(n, new_str(f), None);
        if (is_list) {
            for (var x : each(v))
                if (x.is_ref() && isinstance_(x, ast_t)) out.push_back(x);
        } else if (v.is_ref() && isinstance_(v, ast_t)) out.push_back(v);
    }
    return out;
}

static std::string dump_node(const var& v, bool attrs) {
    TypeObj* ast_t = node_type("AST");
    if (is_list(v)) {
        std::string o = "[";
        bool first = true;
        for (auto& x : list_items(v)) { o += (first ? "" : ", ") + dump_node(x, attrs); first = false; }
        return o + "]";
    }
    if (!(v.is_ref() && isinstance_(v, ast_t))) return repr_(v);
    TypeObj* t = type_of(v);
    std::string o = t->name + "(";
    bool first = true;
    auto it = g_fields->find(t->name);
    if (it != g_fields->end())
        for (auto& [f, is_list] : it->second) {
            var x = getattr(v, new_str(f), None);
            if (x.is_none() && !(t->name == "Constant" && f == "value")) continue;
            if (is_list && len_(x) == 0) continue;
            o += (first ? "" : ", ") + f + "=" + dump_node(x, attrs);
            first = false;
        }
    if (attrs)
        for (const char* a : {"lineno", "col_offset", "end_lineno", "end_col_offset"}) {
            var x = getattr(v, new_str(a), None);
            if (x.is_none()) continue;
            o += (first ? "" : ", ") + std::string(a) + "=" + repr_(x);
            first = false;
        }
    return o + ")";
}

static var literal_eval(const var& n) {
    std::string k = type_of(n)->name;
    if (k == "Expression") return literal_eval(n.getattr("body"));
    if (k == "Constant") return n.getattr("value");
    if (k == "Tuple" || k == "List" || k == "Set") {
        std::vector<var> v;
        for (var x : each(n.getattr("elts"))) v.push_back(literal_eval(x));
        if (k == "Tuple") return new_tuple(v);
        if (k == "List") return new_list(v);
        var s = new_set();
        for (auto& x : v) s.call("add", x);
        return s;
    }
    if (k == "Dict") {
        var d = new_dict();
        auto keys = collect(n.getattr("keys")), vals = collect(n.getattr("values"));
        for (size_t i = 0; i < keys.size(); i++) d.setitem(literal_eval(keys[i]), literal_eval(vals[i]));
        return d;
    }
    if (k == "UnaryOp") {
        std::string op = type_of(n.getattr("op"))->name;
        var x = literal_eval(n.getattr("operand"));
        if (op == "USub") return -x;
        if (op == "UAdd") return +x;
    }
    if (k == "BinOp") {
        std::string op = type_of(n.getattr("op"))->name;
        var l = literal_eval(n.getattr("left")), r = literal_eval(n.getattr("right"));
        if (op == "Add") return l + r;
        if (op == "Sub") return l - r;
    }
    if (k == "Call" && n.getattr("func").attr("id") == "set"_s && len_(n.getattr("args")) == 0) return new_set();
    raise(E_ValueError, "malformed node or string: " + repr_(n));
}

void init_ast_module(const var& m) {
    g_ast = m;
    // the class tree
    auto mk = [&](const char* name, const char* base) {
        TypeObj* b = std::string(base) == "object" ? T_object : node_type(base);
        TypeObj* t = new_type(name, {b}, Layout::Object, false, "ast");
        (*g_node_types)[name] = t;
        mod_set(m, name, tv(t));
        return t;
    };
    TypeObj* AST = mk("AST", "object");
    add_method(AST, "__init__", [](const CallArgs& a) {
        var self = a.pos.at(0);
        auto it = g_fields->find(type_of(self)->name);
        if (it != g_fields->end()) {
            for (size_t i = 0; i < it->second.size(); i++) {
                auto& [f, is_list] = it->second[i];
                var v = i + 1 < a.pos.size() ? a.pos[i + 1] : kwarg(a, f.c_str(), is_list ? new_list() : var(None));
                self.setattr(f.c_str(), v);
            }
        }
        for (auto& [k, v] : a.kw) self.setattr(str_utf8(k).c_str(), v);
        return None;
    });
    for (const char* b : {"mod", "stmt", "expr", "expr_context", "boolop", "operator", "unaryop", "cmpop", "excepthandler", "pattern", "type_param", "type_ignore"})
        mk(b, "AST");
    for (auto& spec : kNodes) {
        TypeObj* t = mk(spec.name, spec.base);
        std::vector<std::pair<std::string, bool>> fs;
        std::vector<var> names;
        std::string f = spec.fields;
        size_t st = 0;
        for (size_t i = 0; i <= f.size(); i++)
            if (i == f.size() || f[i] == ' ') {
                if (i > st) {
                    std::string w = f.substr(st, i - st);
                    bool lst = w.back() == '*';
                    if (lst) w.pop_back();
                    fs.push_back({w, lst});
                    names.push_back(new_str(w));
                }
                st = i + 1;
            }
        (*g_fields)[spec.name] = fs;
        t->dict->set("_fields"_s, new_tuple(names));
        std::string base = spec.base;
        bool positioned = base == "stmt" || base == "expr" || base == "excepthandler" || base == "pattern" || base == "type_param" ||
                          std::string(spec.name) == "arg" || std::string(spec.name) == "keyword" || std::string(spec.name) == "alias";
        t->dict->set("_attributes"_s, positioned ? new_tuple({"lineno"_s, "col_offset"_s, "end_lineno"_s, "end_col_offset"_s}) : new_tuple());
    }
    mod_set(m, "PyCF_ONLY_AST", 1024);
    mod_def(m, "parse", [](const CallArgs& a) {
        var src = arg(a, 0, "source");
        std::string text = is_bytes(src) ? raw_of(src) : str_utf8(src);
        std::string fn = str_utf8(arg(a, 1, "filename", "<unknown>"_s));
        var mode = arg(a, 2, "mode", "exec"_s);
        if (str_utf8(mode) == "eval") {
            var mod = parse_source("(" + text + "\n)", fn);
            var body = mod.getattr("body");
            var e = call0(tv(node_type("Expression")));
            e.setattr("body", list_items(body).at(0).getattr("value"));
            return e;
        }
        return parse_source(text, fn);
    });
    mod_def(m, "iter_child_nodes", [](const CallArgs& a) { return iter(new_list(child_nodes(a.pos.at(0)))); });
    mod_def(m, "iter_fields", [](const CallArgs& a) {
        var n = a.pos.at(0);
        std::vector<var> out;
        auto it = g_fields->find(type_of(n)->name);
        if (it != g_fields->end())
            for (auto& [f, l] : it->second) out.push_back(new_tuple({new_str(f), getattr(n, new_str(f), None)}));
        return iter(new_list(out));
    });
    mod_def(m, "walk", [](const CallArgs& a) {
        // breadth first, the node itself first (CPython's deque order)
        std::vector<var> out{a.pos.at(0)};
        for (size_t i = 0; i < out.size(); i++)
            for (auto& c : child_nodes(out[i])) out.push_back(c);
        return iter(new_list(out));
    });
    mod_def(m, "dump", [](const CallArgs& a) {
        return new_str(dump_node(a.pos.at(0), kwarg(a, "include_attributes", false).truthy()));
    });
    mod_def(m, "literal_eval", [](const CallArgs& a) {
        var n = a.pos.at(0);
        if (is_str(n)) {
            var mod = parse_source("(" + str_utf8(n) + "\n)", "<unknown>");
            n = list_items(mod.getattr("body")).at(0).getattr("value");
        }
        return literal_eval(n);
    });
    mod_def(m, "get_source_segment", [](const CallArgs& a) {
        std::string src = str_utf8(a.pos.at(0));
        var n = a.pos.at(1);
        var l0 = getattr(n, "lineno"_s, None), l1 = getattr(n, "end_lineno"_s, None);
        var c0 = getattr(n, "col_offset"_s, None), c1 = getattr(n, "end_col_offset"_s, None);
        if (l0.is_none() || l1.is_none() || c0.is_none() || c1.is_none()) return var(None);
        // lines as str.splitlines(keepends) would split them; offsets are UTF-8 bytes
        std::vector<std::string> lines;
        size_t st = 0;
        for (size_t i = 0; i < src.size(); i++) {
            if (src[i] == '\n' || src[i] == '\r') {
                size_t e = i + 1;
                if (src[i] == '\r' && i + 1 < src.size() && src[i + 1] == '\n') e++;
                lines.push_back(src.substr(st, e - st));
                st = e;
                i = e - 1;
            }
        }
        if (st < src.size()) lines.push_back(src.substr(st));
        int64_t a0 = l0.as_int64() - 1, a1 = l1.as_int64() - 1;
        if (a0 < 0 || a1 >= (int64_t)lines.size()) return var(None);
        size_t cs = (size_t)c0.as_int64(), ce = (size_t)c1.as_int64();
        if (a0 == a1) return new_str(lines[(size_t)a0].substr(cs, ce - cs));
        std::string o = lines[(size_t)a0].substr(cs);
        for (int64_t k = a0 + 1; k < a1; k++) o += lines[(size_t)k];
        o += lines[(size_t)a1].substr(0, ce);
        return new_str(o);
    });
    mod_def(m, "get_docstring", [](const CallArgs& a) {
        var n = a.pos.at(0);
        bool clean = arg(a, 1, "clean", true).truthy();
        std::string k = type_of(n)->name;
        if (k != "FunctionDef" && k != "AsyncFunctionDef" && k != "ClassDef" && k != "Module")
            raise(E_TypeError, "'" + k + "' can't have docstrings");
        var body = n.getattr("body");
        if (len_(body) == 0) return var(None);
        var first = list_items(body)[0];
        if (type_of(first)->name != "Expr") return var(None);
        var v = first.getattr("value");
        if (type_of(v)->name != "Constant" || !is_str(v.getattr("value"))) return var(None);
        var s = v.getattr("value");
        if (!clean) return s;
        return import("inspect").call("cleandoc", s);
    });
    // NodeVisitor / NodeTransformer
    var vns = new_dict();
    vns.setitem("visit"_s, def("visit", "self, node", {}, [](var self, var node) {
        var m2 = getattr(self, new_str("visit_" + type_of(node)->name), None);
        if (m2.is_none()) return self.call("generic_visit", node);
        return call1(m2, node);
    }));
    vns.setitem("generic_visit"_s, def("generic_visit", "self, node", {}, [](var self, var node) {
        TypeObj* ast_t = node_type("AST");
        auto it = g_fields->find(type_of(node)->name);
        if (it == g_fields->end()) return var(None);
        for (auto& [f, is_list] : it->second) {
            var v = getattr(node, new_str(f), None);
            if (is_list) {
                for (var x : each(new_list(collect(v)))) if (x.is_ref() && isinstance_(x, ast_t)) self.call("visit", x);
            } else if (v.is_ref() && isinstance_(v, ast_t)) self.call("visit", v);
        }
        return var(None);
    }));
    var visitor = make_class("NodeVisitor", "NodeVisitor", {}, vns, {}, "ast");
    mod_set(m, "NodeVisitor", visitor);
    var tns = new_dict();
    tns.setitem("generic_visit"_s, def("generic_visit", "self, node", {}, [](var self, var node) {
        TypeObj* ast_t = node_type("AST");
        auto it = g_fields->find(type_of(node)->name);
        if (it == g_fields->end()) return node;
        for (auto& [f, is_list] : it->second) {
            var v = getattr(node, new_str(f), None);
            if (is_list) {
                std::vector<var> nv;
                for (var x : each(v)) {
                    if (x.is_ref() && isinstance_(x, ast_t)) {
                        var r = self.call("visit", x);
                        if (r.is_none()) continue;
                        if (py::is_list(r)) { for (auto& y : list_items(r)) nv.push_back(y); continue; }
                        nv.push_back(r);
                    } else nv.push_back(x);
                }
                node.setattr(f.c_str(), new_list(nv));
            } else if (v.is_ref() && isinstance_(v, ast_t)) {
                var r = self.call("visit", v);
                node.setattr(f.c_str(), r);
            }
        }
        return node;
    }));
    mod_set(m, "NodeTransformer", make_class("NodeTransformer", "NodeTransformer", {visitor}, tns, {}, "ast"));
    mod_def(m, "copy_location", [](const CallArgs& a) {
        for (const char* at : {"lineno", "col_offset", "end_lineno", "end_col_offset"}) {
            var v = getattr(a.pos.at(1), new_str(at), None);
            if (!v.is_none()) a.pos.at(0).setattr(at, v);
        }
        return a.pos[0];
    });
    mod_def(m, "fix_missing_locations", [](const CallArgs& a) { return a.pos.at(0); });
    mod_def(m, "unparse", [](const CallArgs&) -> var { raise(E_NotImplementedError, "ast.unparse is not in the C++ runtime yet"); });
}

// ------------------------------------------------------------------------------------------ eval / exec
// eval / exec / compile of source text, and `-c`: parsed by pyfront and run over the tree. Names are
// Python's scopes (a function's locals, the functions around it, the globals, the builtins); def, class,
// lambda, comprehensions and generator expressions (lazy), f-strings, try / with / for / while, import.
// What needs a frame of the compiled program (exec with no globals) or Python's compiler (yield,
// await, match) raises NotImplementedError.
namespace {
using namespace pyfront;

struct Tree {                                  // a parsed text, kept alive by the functions it defines
    pyfront::ParseResult r;
    std::deque<std::string> names;             // (stable strings for make_function)
    const char* keep(const std::string& s) { names.push_back(s); return names.back().c_str(); }
};
using TreeP = std::shared_ptr<Tree>;

[[noreturn]] void syntax_error_(const pyfront::SyntaxError& se, const std::string& filename) {
    var e = make_exc_args(E_SyntaxError, {new_str(se.msg), new_tuple({new_str(filename), var((int64_t)se.lineno),
                                                                       var((int64_t)se.offset), new_str(se.text)})});
    e.setattr("lineno", var((int64_t)se.lineno));
    e.setattr("offset", var((int64_t)se.offset));
    e.setattr("filename", new_str(filename));
    e.setattr("msg", new_str(se.msg));
    e.setattr("text", new_str(se.text));
    raise(e);
}
TreeP parse_tree_(const std::string& text, const std::string& filename) {
    auto t = std::make_shared<Tree>();
    t->r = pyfront::parse(text, filename);
    if (!t->r) syntax_error_(*t->r.error, filename);
    return t;
}

enum class Flow { Normal, Break, Continue, Return };

struct Evaluator {
    var globals, locals, builtins;
    std::vector<var> outer;                    // the enclosing functions' locals, innermost last
    std::set<std::string> global_names, nonlocal_names;
    bool function = false;                     // locals are a function's (not the module's, not a class body's)
    TreeP tree;
    var retval;

    var builtins_mod() { return import("builtins"); }
    var name(const std::string& n) {
        var k = new_str(n);
        if (!global_names.count(n)) {
            if (!locals.is_none() && in(k, locals)) return locals.getitem(k);
            for (size_t i = outer.size(); i-- > 0;)
                if (in(k, outer[i])) return outer[i].getitem(k);
        }
        if (!globals.is_none() && in(k, globals)) return globals.getitem(k);
        if (builtins.is_none()) {
            var b = builtins_mod();
            if (hasattr(b, k).truthy()) return b.getattr(n.c_str());
        } else if (is_dict(builtins)) {
            if (in(k, builtins)) return builtins.getitem(k);
        } else if (hasattr(builtins, k).truthy()) return builtins.getattr(n.c_str());
        raise(E_NameError, "name '" + n + "' is not defined");
    }
    var& scope_of_(const std::string& n) {     // where a store of n goes
        if (global_names.count(n) || locals.is_none()) return globals;
        if (nonlocal_names.count(n)) {
            var k = new_str(n);
            for (size_t i = outer.size(); i-- > 0;)
                if (in(k, outer[i])) return outer[i];
        }
        return locals;
    }
    void store(const std::string& n, const var& v) { scope_of_(n).setitem(new_str(n), v); }
    void del_name(const std::string& n) { scope_of_(n).delitem(new_str(n)); }
    std::vector<var> chain() {                 // the scopes a function defined here reads through
        std::vector<var> c = outer;
        if (function) c.push_back(locals);
        return c;
    }
    Evaluator child(const var& new_locals) {
        Evaluator e;
        e.globals = globals;
        e.builtins = builtins;
        e.locals = new_locals;
        e.outer = chain();
        e.function = true;
        e.tree = tree;
        return e;
    }

    var constant(const ConstValue& c) {
        switch (c.kind) {
        case ConstValue::Kind::None: return None;
        case ConstValue::Kind::True: return var(true);
        case ConstValue::Kind::False: return var(false);
        case ConstValue::Kind::Ellipsis: return builtins_mod().getattr("Ellipsis");
        case ConstValue::Kind::Int: return c.fits_i64 ? var(c.i64) : int_lit_(c.text.c_str());
        case ConstValue::Kind::Float: return var(c.f);
        case ConstValue::Kind::Str: return new_str(c.text);
        case ConstValue::Kind::Bytes: return new_bytes(c.text);
        default: raise(E_NotImplementedError, "eval: complex constants");
        }
    }
    var binop(OpKind op, const var& a, const var& b) {
        switch (op) {
        case OpKind::Add: return a + b;
        case OpKind::Sub: return a - b;
        case OpKind::Mult: return a * b;
        case OpKind::MatMult: return matmul(a, b);
        case OpKind::Div: return a / b;
        case OpKind::Mod: return a % b;
        case OpKind::Pow: return pow(a, b);
        case OpKind::LShift: return a << b;
        case OpKind::RShift: return a >> b;
        case OpKind::BitOr: return a | b;
        case OpKind::BitXor: return a ^ b;
        case OpKind::BitAnd: return a & b;
        case OpKind::FloorDiv: return floordiv(a, b);
        }
        return None;
    }
    var inplace(OpKind op, var a, const var& b) {   // x op= y: __iop__, else __op__
        switch (op) {
        case OpKind::Add: a += b; return a;
        case OpKind::Sub: a -= b; return a;
        case OpKind::Mult: a *= b; return a;
        case OpKind::Div: a /= b; return a;
        case OpKind::Mod: a %= b; return a;
        case OpKind::BitOr: a |= b; return a;
        case OpKind::BitAnd: a &= b; return a;
        case OpKind::BitXor: a ^= b; return a;
        case OpKind::LShift: a <<= b; return a;
        case OpKind::RShift: a >>= b; return a;
        default: return binop(op, a, b);
        }
    }
    var cmp_value(CmpOpKind op, const var& a, const var& b) {   // (the rich comparisons' values)
        switch (op) {
        case CmpOpKind::Eq: return compare_value_(a, b, 2);
        case CmpOpKind::NotEq: return compare_value_(a, b, 3);
        case CmpOpKind::Lt: return compare_value_(a, b, 0);
        case CmpOpKind::LtE: return compare_value_(a, b, 1);
        case CmpOpKind::Gt: return compare_value_(a, b, 4);
        case CmpOpKind::GtE: return compare_value_(a, b, 5);
        default: return var(cmp(op, a, b));
        }
    }
    bool cmp(CmpOpKind op, const var& a, const var& b) {
        switch (op) {
        case CmpOpKind::Eq: return a == b;
        case CmpOpKind::NotEq: return a != b;
        case CmpOpKind::Lt: return a < b;
        case CmpOpKind::LtE: return a <= b;
        case CmpOpKind::Gt: return a > b;
        case CmpOpKind::GtE: return a >= b;
        case CmpOpKind::Is: return is(a, b);
        case CmpOpKind::IsNot: return !is(a, b);
        case CmpOpKind::In: return in(a, b);
        case CmpOpKind::NotIn: return !in(a, b);
        }
        return false;
    }
    std::vector<var> items(const std::vector<Expr*>& es) {
        std::vector<var> out;
        for (auto* e : es) {
            if (e->kind == ExprKind::Starred) { for (var x : each(ev(static_cast<Starred*>(e)->value))) out.push_back(x); }
            else out.push_back(ev(e));
        }
        return out;
    }
    var key_of(const Expr* sl) {                  // a subscript's key (a slice object for a:b:c)
        if (sl->kind == ExprKind::Slice) {
            auto* s = static_cast<const Slice*>(sl);
            var lo = s->lower ? ev(s->lower) : var(None), hi = s->upper ? ev(s->upper) : var(None), st = s->step ? ev(s->step) : var(None);
            return slice_obj(lo, hi, st);
        }
        return ev(sl);
    }

    // ---- assignment targets
    void assign(const Expr* t, const var& v) {
        switch (t->kind) {
        case ExprKind::Name: store(static_cast<const Name*>(t)->id, v); return;
        case ExprKind::Attribute: { auto* a = static_cast<const Attribute*>(t); ev(a->value).setattr(a->attr.c_str(), v); return; }
        case ExprKind::Subscript: { auto* s = static_cast<const Subscript*>(t); var o = ev(s->value); o.setitem(key_of(s->slice), v); return; }
        case ExprKind::Tuple:
        case ExprKind::List: {
            const auto& elts = t->kind == ExprKind::Tuple ? static_cast<const Tuple*>(t)->elts : static_cast<const List*>(t)->elts;
            size_t star = elts.size();
            for (size_t i = 0; i < elts.size(); i++)
                if (elts[i]->kind == ExprKind::Starred) star = i;
            std::vector<var> vals = star == elts.size() ? unpack(v, elts.size()) : unpack_star(v, star, elts.size() - star - 1);
            for (size_t i = 0; i < elts.size(); i++)
                assign(i == star ? static_cast<const Starred*>(elts[i])->value : elts[i], vals[i]);
            return;
        }
        default: raise(E_SyntaxError, std::string("cannot assign to ") + name_of(t->kind));
        }
    }
    void del(const Expr* t) {
        switch (t->kind) {
        case ExprKind::Name: del_name(static_cast<const Name*>(t)->id); return;
        case ExprKind::Attribute: { auto* a = static_cast<const Attribute*>(t); ev(a->value).delattr(a->attr.c_str()); return; }
        case ExprKind::Subscript: { auto* s = static_cast<const Subscript*>(t); var o = ev(s->value); o.delitem(key_of(s->slice)); return; }
        case ExprKind::Tuple: for (auto* e : static_cast<const Tuple*>(t)->elts) del(e); return;
        case ExprKind::List: for (auto* e : static_cast<const List*>(t)->elts) del(e); return;
        default: raise(E_SyntaxError, std::string("cannot delete ") + name_of(t->kind));
        }
    }

    // ---- functions: a real function object (pyrt binds the arguments), its body run over a new scope
    static void declared_(const std::vector<Stmt*>& body, std::set<std::string>& g, std::set<std::string>& nl) {
        for (auto* s : body) {
            switch (s->kind) {
            case StmtKind::Global: for (auto& n : static_cast<const pyfront::Global*>(s)->names) g.insert(n); break;
            case StmtKind::Nonlocal: for (auto& n : static_cast<const Nonlocal*>(s)->names) nl.insert(n); break;
            case StmtKind::If: declared_(static_cast<const If*>(s)->body, g, nl); declared_(static_cast<const If*>(s)->orelse, g, nl); break;
            case StmtKind::For: declared_(static_cast<const For*>(s)->body, g, nl); declared_(static_cast<const For*>(s)->orelse, g, nl); break;
            case StmtKind::While: declared_(static_cast<const While*>(s)->body, g, nl); declared_(static_cast<const While*>(s)->orelse, g, nl); break;
            case StmtKind::With: declared_(static_cast<const With*>(s)->body, g, nl); break;
            case StmtKind::Try: {
                auto* t = static_cast<const Try*>(s);
                declared_(t->body, g, nl);
                for (auto* h : t->handlers) declared_(h->body, g, nl);
                declared_(t->orelse, g, nl);
                declared_(t->finalbody, g, nl);
                break;
            }
            default: break;
            }
        }
    }
    var make_fn(const std::string& fname, const arguments* args, const Expr* lambda_body, const std::vector<Stmt*>* body) {
        std::string sig;
        std::vector<var> defaults;
        std::vector<std::string> pnames;
        size_t npos = args->posonlyargs.size() + args->args.size();
        size_t first_default = npos - args->defaults.size();
        auto add = [&](const std::string& s) { if (!sig.empty()) sig += ", "; sig += s; };
        for (size_t i = 0; i < npos; i++) {
            const pyfront::arg* p = i < args->posonlyargs.size() ? args->posonlyargs[i] : args->args[i - args->posonlyargs.size()];
            bool d = i >= first_default;
            add(p->arg + (d ? "=" : ""));
            if (d) defaults.push_back(ev(args->defaults[i - first_default]));
            pnames.push_back(p->arg);
            if (!args->posonlyargs.empty() && i + 1 == args->posonlyargs.size()) add("/");
        }
        if (args->vararg) { add("*" + args->vararg->arg); pnames.push_back(args->vararg->arg); }
        else if (!args->kwonlyargs.empty()) add("*");
        for (size_t i = 0; i < args->kwonlyargs.size(); i++) {
            bool d = i < args->kw_defaults.size() && args->kw_defaults[i];
            add(args->kwonlyargs[i]->arg + (d ? "=" : ""));
            if (d) defaults.push_back(ev(args->kw_defaults[i]));
            pnames.push_back(args->kwonlyargs[i]->arg);
        }
        if (args->kwarg) { add("**" + args->kwarg->arg); pnames.push_back(args->kwarg->arg); }
        std::set<std::string> g, nl;
        if (body) declared_(*body, g, nl);
        var gl = globals, bl = builtins;
        std::vector<var> ch = chain();
        TreeP tr = tree;
        var modname = in("__name__"_s, globals) ? globals.getitem("__name__"_s) : var(None);
        const char* mod = tr->keep(is_str(modname) ? str_utf8(modname) : std::string());
        const char* nm = tr->keep(fname);
        const char* sg = tr->keep(sig);
        int n = (int)pnames.size();
        invoker fn = [gl, bl, ch, tr, pnames, g, nl, lambda_body, body](var* slots) -> var {
            Evaluator e;
            e.globals = gl;
            e.builtins = bl;
            e.outer = ch;
            e.function = true;
            e.tree = tr;
            e.global_names = g;
            e.nonlocal_names = nl;
            e.locals = new_dict();
            for (size_t i = 0; i < pnames.size(); i++) e.locals.setitem(new_str(pnames[i]), slots[i]);
            if (lambda_body) return e.ev(lambda_body);
            if (e.run(*body) == Flow::Return) return e.retval;
            return None;
        };
        return make_function(nm, nm, sg, std::move(defaults), std::move(fn), n, false, mod);
    }

    // ---- comprehensions: their own scope; a generator expression is lazy (an iterator)
    void comp_run(Evaluator& e, const std::vector<comprehension*>& gens, size_t k, const var& first, const std::function<void()>& emit) {
        var src = k == 0 ? first : e.ev(gens[k]->iter);
        for (var x : each(src)) {
            e.assign(gens[k]->target, x);
            bool ok = true;
            for (auto* c : gens[k]->ifs)
                if (!e.ev(c).truthy()) { ok = false; break; }
            if (!ok) continue;
            if (k + 1 < gens.size()) comp_run(e, gens, k + 1, first, emit);
            else emit();
        }
    }
    var genexp(const GeneratorExp* g) {
        struct CompState {
            Evaluator e;
            const std::vector<comprehension*>* gens;
            std::vector<var> its;
        };
        auto st = std::make_shared<CompState>();
        st->e = child(new_dict());
        st->gens = &g->generators;
        st->its.push_back(iter(ev(g->generators[0]->iter)));   // (the first iterable: evaluated here, as Python does)
        const Expr* elt = g->elt;
        TreeP keep = tree;
        return new_iter([st, elt, keep](var& out) -> bool {
            auto& gens = *st->gens;
            while (!st->its.empty()) {
                size_t k = st->its.size() - 1;
                var x;
                if (!next_(st->its[k], x)) { st->its.pop_back(); continue; }
                st->e.assign(gens[k]->target, x);
                bool ok = true;
                for (auto* c : gens[k]->ifs)
                    if (!st->e.ev(c).truthy()) { ok = false; break; }
                if (!ok) continue;
                if (k + 1 < gens.size()) { st->its.push_back(iter(st->e.ev(gens[k + 1]->iter))); continue; }
                out = st->e.ev(elt);
                return true;
            }
            return false;
        });
    }

    var joined(const std::vector<Expr*>& values) {
        std::string out;
        for (auto* v : values) {
            if (v->kind == ExprKind::FormattedValue) {
                auto* f = static_cast<const FormattedValue*>(v);
                var spec = f->format_spec ? ev(f->format_spec) : var(None);
                out += str_utf8(fmt(ev(f->value), f->conversion < 0 ? (char)0 : (char)f->conversion, spec));
            } else out += str_utf8(ev(v));
        }
        return new_str(std::move(out));
    }

    var ev(const Expr* e) {
        switch (e->kind) {
        case ExprKind::Constant: return constant(static_cast<const Constant*>(e)->value);
        case ExprKind::Name: return name(static_cast<const Name*>(e)->id);
        case ExprKind::BinOp: { auto* b = static_cast<const BinOp*>(e); var l = ev(b->left); return binop(b->op, l, ev(b->right)); }
        case ExprKind::UnaryOp: {
            auto* u = static_cast<const UnaryOp*>(e);
            var v = ev(u->operand);
            switch (u->op) {
            case UnaryOpKind::Not: return var(!v.truthy());
            case UnaryOpKind::USub: return -v;
            case UnaryOpKind::UAdd: return +v;
            case UnaryOpKind::Invert: return ~v;
            }
            return None;
        }
        case ExprKind::BoolOp: {
            auto* b = static_cast<const BoolOp*>(e);
            var v;
            for (auto* x : b->values) {
                v = ev(x);
                if (b->op == BoolOpKind::And ? !v.truthy() : v.truthy()) return v;
            }
            return v;
        }
        case ExprKind::Compare: {
            auto* c = static_cast<const Compare*>(e);
            // a < b < c is (a < b) and (b < c), each comparison's own value (an __eq__ may return anything)
            var l = ev(c->left), v;
            for (size_t i = 0; i < c->ops.size(); i++) {
                var r = ev(c->comparators[i]);
                v = cmp_value(c->ops[i], l, r);
                if (!v.truthy()) return v;
                l = r;
            }
            return v;
        }
        case ExprKind::IfExp: { auto* x = static_cast<const IfExp*>(e); return ev(x->test).truthy() ? ev(x->body) : ev(x->orelse); }
        case ExprKind::NamedExpr: { auto* x = static_cast<const NamedExpr*>(e); var v = ev(x->value); assign(x->target, v); return v; }
        case ExprKind::Tuple: return new_tuple(items(static_cast<const Tuple*>(e)->elts));
        case ExprKind::List: return new_list(items(static_cast<const List*>(e)->elts));
        case ExprKind::Set: { var s = call0(builtins_mod().getattr("set")); for (var x : items(static_cast<const Set*>(e)->elts)) s.call("add", x); return s; }
        case ExprKind::Dict: {
            auto* d = static_cast<const Dict*>(e);
            var o = new_dict();
            for (size_t i = 0; i < d->keys.size(); i++) {
                if (!d->keys[i]) { var m = ev(d->values[i]); for (var k : each(m)) o.setitem(k, m.getitem(k)); }
                else { var k = ev(d->keys[i]); o.setitem(k, ev(d->values[i])); }
            }
            return o;
        }
        case ExprKind::Subscript: { auto* s = static_cast<const Subscript*>(e); var v = ev(s->value); return v.getitem(key_of(s->slice)); }
        case ExprKind::Slice: return key_of(e);
        case ExprKind::Attribute: { auto* a = static_cast<const Attribute*>(e); return ev(a->value).getattr(a->attr.c_str()); }
        case ExprKind::Call: {
            auto* c = static_cast<const Call*>(e);
            var f = ev(c->func);
            CallArgs ca;
            ca.pos = items(c->args);
            for (auto* k : c->keywords) {
                if (!k->has_arg) { var m = ev(k->value); for (var key : each(m)) ca.kw.push_back({key, m.getitem(key)}); }
                else ca.kw.push_back({new_str(k->arg), ev(k->value)});
            }
            return call_obj(f, ca);
        }
        case ExprKind::Lambda: { auto* l = static_cast<const Lambda*>(e); return make_fn("<lambda>", l->args, l->body, nullptr); }
        case ExprKind::JoinedStr: return joined(static_cast<const JoinedStr*>(e)->values);
        case ExprKind::FormattedValue: return joined({const_cast<Expr*>(e)});
        case ExprKind::ListComp: {
            auto* c = static_cast<const ListComp*>(e);
            var first = ev(c->generators[0]->iter);
            Evaluator s = child(new_dict());
            std::vector<var> out;
            comp_run(s, c->generators, 0, first, [&] { out.push_back(s.ev(c->elt)); });
            return new_list(out);
        }
        case ExprKind::SetComp: {
            auto* c = static_cast<const SetComp*>(e);
            var first = ev(c->generators[0]->iter);
            Evaluator s = child(new_dict());
            var out = call0(builtins_mod().getattr("set"));
            comp_run(s, c->generators, 0, first, [&] { out.call("add", s.ev(c->elt)); });
            return out;
        }
        case ExprKind::DictComp: {
            auto* c = static_cast<const DictComp*>(e);
            var first = ev(c->generators[0]->iter);
            Evaluator s = child(new_dict());
            var out = new_dict();
            comp_run(s, c->generators, 0, first, [&] { var k = s.ev(c->key); out.setitem(k, s.ev(c->value)); });
            return out;
        }
        case ExprKind::GeneratorExp: return genexp(static_cast<const GeneratorExp*>(e));
        default: raise(E_NotImplementedError, std::string("eval: ") + name_of(e->kind) + " needs Python's compiler");
        }
    }

    // ---- statements
    Flow run(const std::vector<Stmt*>& body) {
        for (auto* s : body) {
            Flow f = exec(s);
            if (f != Flow::Normal) return f;
        }
        return Flow::Normal;
    }
    Flow run_with(const With* w, size_t k) {
        if (k == w->items.size()) return run(w->body);
        var mgr = ev(w->items[k]->context_expr);
        var exit_fn = with_exit_of(mgr);
        var v = with_enter(mgr);
        Flow f;
        try {
            if (w->items[k]->optional_vars) assign(w->items[k]->optional_vars, v);
            f = run_with(w, k + 1);
        } catch (error& e) {
            bool suppressed;
            {
                handling_ h(e);
                suppressed = with_exit_exc(exit_fn, e);
            }
            if (!suppressed) throw;
            return Flow::Normal;
        }
        call_obj(exit_fn, CallArgs{{None, None, None}, {}});
        return f;
    }
    Flow run_try(const Try* t) {
        Flow f = Flow::Normal;
        try {
            bool raised = false;
            try {
                f = run(t->body);
            } catch (error& e) {
                raised = true;
                bool handled = false;
                for (auto* h : t->handlers) {
                    if (h->type && !e.matches(ev(h->type))) continue;
                    handled = true;
                    handling_ hg(e);
                    if (h->has_name) store(h->name, e.value);
                    f = run(h->body);
                    if (h->has_name) {
                        var k = new_str(h->name);
                        var& sc = scope_of_(h->name);
                        if (in(k, sc)) sc.delitem(k);
                    }
                    break;
                }
                if (!handled) throw;
            }
            if (!raised && f == Flow::Normal) f = run(t->orelse);
        } catch (error&) {
            if (!t->finalbody.empty()) {
                Flow ff = run(t->finalbody);
                if (ff != Flow::Normal) return ff;     // (a return / break in finally drops the exception)
            }
            throw;
        }
        if (!t->finalbody.empty()) {
            var keep = retval;
            Flow ff = run(t->finalbody);
            if (ff != Flow::Normal) return ff;
            retval = keep;
        }
        return f;
    }
    Flow exec(const Stmt* s) {
        switch (s->kind) {
        case StmtKind::Expr: ev(static_cast<const ExprStmt*>(s)->value); return Flow::Normal;
        case StmtKind::Assign: {
            auto* a = static_cast<const Assign*>(s);
            var v = ev(a->value);
            for (auto* t : a->targets) assign(t, v);
            return Flow::Normal;
        }
        case StmtKind::AugAssign: {
            auto* a = static_cast<const AugAssign*>(s);
            const Expr* t = a->target;
            if (t->kind == ExprKind::Name) {
                var cur = name(static_cast<const Name*>(t)->id);
                store(static_cast<const Name*>(t)->id, inplace(a->op, cur, ev(a->value)));
            } else if (t->kind == ExprKind::Attribute) {
                auto* at = static_cast<const Attribute*>(t);
                var o = ev(at->value);
                var cur = o.getattr(at->attr.c_str());
                o.setattr(at->attr.c_str(), inplace(a->op, cur, ev(a->value)));
            } else if (t->kind == ExprKind::Subscript) {
                auto* sb = static_cast<const Subscript*>(t);
                var o = ev(sb->value);
                var k = key_of(sb->slice);
                var cur = o.getitem(k);
                o.setitem(k, inplace(a->op, cur, ev(a->value)));
            } else raise(E_SyntaxError, "illegal expression for augmented assignment");
            return Flow::Normal;
        }
        case StmtKind::AnnAssign: {
            auto* a = static_cast<const AnnAssign*>(s);
            if (a->value) assign(a->target, ev(a->value));
            return Flow::Normal;
        }
        case StmtKind::Delete: for (auto* t : static_cast<const Delete*>(s)->targets) del(t); return Flow::Normal;
        case StmtKind::Pass: return Flow::Normal;
        case StmtKind::Break: return Flow::Break;
        case StmtKind::Continue: return Flow::Continue;
        case StmtKind::Return: {
            auto* r = static_cast<const Return*>(s);
            retval = r->value ? ev(r->value) : var(None);
            return Flow::Return;
        }
        case StmtKind::If: {
            auto* i = static_cast<const If*>(s);
            return ev(i->test).truthy() ? run(i->body) : run(i->orelse);
        }
        case StmtKind::While: {
            auto* w = static_cast<const While*>(s);
            while (ev(w->test).truthy()) {
                Flow f = run(w->body);
                if (f == Flow::Break) return Flow::Normal;
                if (f == Flow::Return) return f;
            }
            return run(w->orelse);
        }
        case StmtKind::For: {
            auto* fr = static_cast<const For*>(s);
            var src = ev(fr->iter);
            for (var x : each(src)) {
                assign(fr->target, x);
                Flow f = run(fr->body);
                if (f == Flow::Break) return Flow::Normal;
                if (f == Flow::Return) return f;
            }
            return run(fr->orelse);
        }
        case StmtKind::With: return run_with(static_cast<const With*>(s), 0);
        case StmtKind::Try: return run_try(static_cast<const Try*>(s));
        case StmtKind::Raise: {
            auto* r = static_cast<const Raise*>(s);
            if (!r->exc) reraise();
            var x = ev(r->exc);
            if (r->cause) raise_from(x, ev(r->cause));
            raise(x);
        }
        case StmtKind::Assert: {
            auto* a = static_cast<const Assert*>(s);
            if (!ev(a->test).truthy()) assert_(var(false), a->msg ? ev(a->msg) : var(None));
            return Flow::Normal;
        }
        case StmtKind::Import:
            for (auto* al : static_cast<const Import*>(s)->names) {
                if (al->has_asname) store(al->asname, import(al->name.c_str()));
                else store(al->name.substr(0, al->name.find('.')), import_top(al->name.c_str()));
            }
            return Flow::Normal;
        case StmtKind::ImportFrom: {
            auto* f = static_cast<const ImportFrom*>(s);
            if (f->level > 0) raise(E_NotImplementedError, "exec: a relative import needs the importing module");
            var m = import(f->module.c_str());
            for (auto* al : f->names) {
                if (al->name == "*") {
                    var all = getattr(m, "__all__"_s, None);
                    if (all.is_none()) {
                        for (var k : each(dir(m)))
                            if (str_utf8(k).rfind("_", 0) != 0) store(str_utf8(k), m.getattr(str_utf8(k).c_str()));
                    } else for (var k : each(all)) store(str_utf8(k), m.getattr(str_utf8(k).c_str()));
                    continue;
                }
                store(al->has_asname ? al->asname : al->name, import_from(m, al->name.c_str()));
            }
            return Flow::Normal;
        }
        case StmtKind::Global: for (auto& n : static_cast<const pyfront::Global*>(s)->names) global_names.insert(n); return Flow::Normal;
        case StmtKind::Nonlocal: for (auto& n : static_cast<const Nonlocal*>(s)->names) nonlocal_names.insert(n); return Flow::Normal;
        case StmtKind::FunctionDef: {
            auto* d = static_cast<const FunctionDef*>(s);
            std::vector<var> decos;
            for (auto* x : d->decorator_list) decos.push_back(ev(x));
            var f = make_fn(d->name, d->args, nullptr, &d->body);
            for (size_t i = decos.size(); i-- > 0;) f = call1(decos[i], f);
            store(d->name, f);
            return Flow::Normal;
        }
        case StmtKind::ClassDef: {
            auto* c = static_cast<const ClassDef*>(s);
            std::vector<var> decos;
            for (auto* x : c->decorator_list) decos.push_back(ev(x));
            std::vector<var> bases = items(c->bases);
            std::vector<std::pair<const char*, var>> kws;
            for (auto* k : c->keywords) kws.push_back({tree->keep(k->arg), ev(k->value)});
            var ns = new_dict();
            var modname = in("__name__"_s, globals) ? globals.getitem("__name__"_s) : var(None);
            if (is_str(modname)) ns.setitem("__module__"_s, modname);
            ns.setitem("__qualname__"_s, new_str(c->name));
            Evaluator body;                  // (a class body's names are not seen by the functions in it)
            body.globals = globals;
            body.builtins = builtins;
            body.locals = ns;
            body.outer = chain();
            body.function = false;
            body.tree = tree;
            body.run(c->body);
            var cls = make_class(tree->keep(c->name), tree->keep(c->name), bases, ns, kws, is_str(modname) ? tree->keep(str_utf8(modname)) : nullptr);
            for (size_t i = decos.size(); i-- > 0;) cls = call1(decos[i], cls);
            store(c->name, cls);
            return Flow::Normal;
        }
        default: raise(E_NotImplementedError, std::string("exec: ") + name_of(s->kind) + " needs Python's compiler");
        }
    }
};

TypeObj* T_code = nullptr;
struct CodeObj : DataObj {
    TreeP tree;
    std::string filename, mode;
    bool expr = false;                         // mode 'eval': the body is one expression
};

Evaluator evaluator_(const var& globals, const var& locals, const TreeP& tree) {
    Evaluator ev;
    ev.globals = globals;
    ev.locals = locals.is_none() ? globals : locals;
    ev.builtins = None;
    ev.tree = tree;
    if (!globals.is_none()) {
        if (in("__builtins__"_s, globals)) ev.builtins = globals.getitem("__builtins__"_s);
        else globals.setitem("__builtins__"_s, import("builtins"));   // (as CPython puts it there)
    }
    return ev;
}
TreeP tree_of_(const var& src, const std::string& filename, bool expr) {
    std::string text = is_str(src) ? str_utf8(src) : raw_of(src);
    if (!expr) return parse_tree_(text, filename);
    size_t k = text.find_first_not_of(" \t\r\n");
    text = k == std::string::npos ? "" : text.substr(k);
    auto t = std::make_shared<Tree>();
    t->r = pyfront::parse("(" + text + "\n)", filename);
    if (!t->r || t->r.module->body.size() != 1 || t->r.module->body[0]->kind != pyfront::StmtKind::Expr) {
        if (!t->r) syntax_error_(*t->r.error, filename);
        raise(E_SyntaxError, "invalid syntax");
    }
    return t;
}
}  // namespace

var eval_expr_(const var& src, const var& globals, const var& locals) {
    TreeP t;
    if (src.is_ref() && src.ref()->type == T_code) {
        auto* c = static_cast<CodeObj*>(src.ref());
        if (!c->expr) {
            if (globals.is_none()) raise(E_NotImplementedError, "eval of statements with no globals needs the caller's frame");
            Evaluator ev = evaluator_(globals, locals, c->tree);
            ev.run(c->tree->r.module->body);
            return None;
        }
        t = c->tree;
    } else t = tree_of_(src, "<string>", true);
    Evaluator ev = evaluator_(globals, locals, t);
    if (globals.is_none()) ev.locals = None;
    return ev.ev(static_cast<pyfront::ExprStmt*>(t->r.module->body[0])->value);
}

var exec_source_(const var& src, const var& globals, const var& locals) {
    if (globals.is_none()) raise(E_NotImplementedError, "exec with no globals needs the caller's frame (pass a namespace)");
    TreeP t;
    if (src.is_ref() && src.ref()->type == T_code) {
        auto* c = static_cast<CodeObj*>(src.ref());
        if (c->expr) { Evaluator ev = evaluator_(globals, locals, c->tree); ev.ev(static_cast<pyfront::ExprStmt*>(c->tree->r.module->body[0])->value); return None; }
        t = c->tree;
    } else t = tree_of_(src, "<string>", false);
    Evaluator ev = evaluator_(globals, locals, t);
    ev.run(t->r.module->body);
    return None;
}

var compile_source_(const CallArgs& a) {
    var src = arg(a, 0, "source");
    std::string filename = fs_path(arg(a, 1, "filename"));
    std::string mode = str_utf8(arg(a, 2, "mode"));
    int64_t flags = arg(a, 3, "flags", 0).as_int64();
    if (flags & 1024) return call_obj(import("ast").getattr("parse"), CallArgs{{src, new_str(filename), new_str(mode)}, {}});
    if (!is_str(src) && !is_bytes(src)) raise(E_NotImplementedError, "compile of an AST needs Python's compiler (give the source text)");
    if (mode != "exec" && mode != "eval" && mode != "single") raise(E_ValueError, "compile() mode must be 'exec', 'eval' or 'single'");
    if (!T_code) T_code = new_type("code", {T_object}, Layout::NativeData, true, "builtins");
    auto* c = new CodeObj();
    c->type = T_code;
    c->filename = filename;
    c->mode = mode;
    c->expr = mode == "eval";
    c->tree = tree_of_(src, filename, c->expr);
    var r(static_cast<Obj*>(c));
    return r;
}

// `python -c text args`: the text runs as __main__
int run_source_main_(const std::string& text) {
    var g = new_dict();
    g.setitem("__name__"_s, "__main__"_s);
    g.setitem("__doc__"_s, None);
    g.setitem("__package__"_s, None);
    g.setitem("__builtins__"_s, import("builtins"));
    TreeP t = parse_tree_(text, "<string>");
    Evaluator ev = evaluator_(g, None, t);
    ev.run(t->r.module->body);
    return 0;
}

// ------------------------------------------------------------------------------------------ getsource
// The source of a function or class, from the file it was translated from: the def / class found by its
// qualified name (scope by scope), its decorators above it, and its block by indentation as Python's
// tokenizer sees it (lines inside a bracket or a triple-quoted string belong to the statement).
namespace {
struct SrcLines {
    std::vector<std::string> lines;     // with their newlines
    std::vector<bool> continued;        // the line starts inside a bracket or a multi-line string
};
SrcLines read_lines(const std::string& text) {
    SrcLines s;
    size_t st = 0;
    while (st < text.size()) {
        size_t e = text.find('\n', st);
        if (e == std::string::npos) e = text.size() - 1;
        s.lines.push_back(text.substr(st, e - st + 1));
        st = e + 1;
    }
    int depth = 0;
    char q = 0;
    bool triple = false;
    for (auto& ln : s.lines) {
        s.continued.push_back(depth > 0 || triple);
        for (size_t i = 0; i < ln.size(); i++) {
            char c = ln[i];
            if (q) {
                if (c == '\\') { i++; continue; }
                if (triple) {
                    if (c == q && i + 2 < ln.size() + 1 && ln.compare(i, 3, std::string(3, q)) == 0) { q = 0; triple = false; i += 2; }
                } else if (c == q || c == '\n') q = 0;
                continue;
            }
            if (c == '#') break;
            if (c == '"' || c == '\'') {
                if (ln.compare(i, 3, std::string(3, c)) == 0) { q = c; triple = true; i += 2; }
                else q = c;
                continue;
            }
            if (c == '(' || c == '[' || c == '{') depth++;
            else if ((c == ')' || c == ']' || c == '}') && depth > 0) depth--;
        }
    }
    return s;
}
size_t indent_of(const std::string& l) { return l.find_first_not_of(" \t"); }
bool blank(const std::string& l) { size_t k = l.find_first_not_of(" \t\r\n"); return k == std::string::npos; }
bool comment(const std::string& l) { size_t k = l.find_first_not_of(" \t"); return k != std::string::npos && l[k] == '#'; }
// the line of `def name` / `class name` between lines [lo, hi), at indentation over `min_indent`
int find_def(const SrcLines& s, size_t lo, size_t hi, const std::string& name, size_t& indent) {
    size_t want = std::string::npos;
    for (size_t i = lo; i < hi && i < s.lines.size(); i++) {
        if (s.continued[i]) continue;
        const std::string& l = s.lines[i];
        size_t k = indent_of(l);
        if (k == std::string::npos) continue;
        if (want != std::string::npos && k > want) continue;   // (deeper: inside another statement of this scope)
        std::string rest = l.substr(k);
        for (const char* kw : {"def ", "async def ", "class "}) {
            size_t n = strlen(kw);
            if (rest.compare(0, n, kw) == 0) {
                size_t p = n;
                while (p < rest.size() && rest[p] == ' ') p++;
                if (rest.compare(p, name.size(), name) == 0 && p + name.size() < rest.size() &&
                    (rest[p + name.size()] == '(' || rest[p + name.size()] == ':' || rest[p + name.size()] == ' ' || rest[p + name.size()] == '[')) {
                    indent = k;
                    return (int)i;
                }
            }
        }
        if (want == std::string::npos) want = k;
    }
    return -1;
}
// the end (exclusive) of the block whose header is at line h
size_t block_end(const SrcLines& s, size_t h, size_t indent) {
    size_t i = h + 1;
    while (i < s.lines.size() && s.continued[i]) i++;   // (a header across lines)
    size_t last = i;
    for (; i < s.lines.size(); i++) {
        if (s.continued[i]) { last = i + 1; continue; }
        const std::string& l = s.lines[i];
        if (blank(l)) continue;
        size_t k = indent_of(l);
        if (comment(l) && k <= indent) {
            // a comment at the def's depth: in the block only when code of the block follows it
            size_t j = i + 1;
            while (j < s.lines.size() && (blank(s.lines[j]) || comment(s.lines[j]))) j++;
            if (j < s.lines.size() && indent_of(s.lines[j]) > indent) continue;
            break;
        }
        if (k <= indent) break;
        last = i + 1;
    }
    return last;
}
}  // namespace

var getsource_(const var& obj, int64_t* first_line) {
    var o = obj;
    if (o.is_ref() && o.ref()->type == T_method) o = static_cast<MethodObj*>(o.ref())->func;
    var w = getattr(o, "__wrapped__"_s, var());
    if (!w.is_unbound() && !w.is_none()) o = w;
    std::string file, qual;
    if (o.is_ref() && o.ref()->type == T_module) file = str_utf8(o.getattr("__file__"));
    else {
        var mod = getattr(o, "__module__"_s, None);
        if (mod.is_none()) raise(E_TypeError, "module, class, method, function, traceback, frame, or code object was expected, got " + tname(o));
        var sm = import("sys").getattr("modules").call("get", mod);
        if (sm.is_none()) raise(E_OSError, "could not get source code");
        file = str_utf8(sm.getattr("__file__"));
        qual = str_utf8(getattr(o, "__qualname__"_s, getattr(o, "__name__"_s, ""_s)));
    }
    std::string text;
    {
        FILE* f = _wfopen(std::filesystem::u8path(file).c_str(), L"rb");
        if (!f) raise(E_OSError, "could not get source code");
        char buf[65536];
        size_t n;
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) text.append(buf, n);
        fclose(f);
    }
    // universal newlines, as linecache reads it (a CRLF checkout gives the same source, and the same cache keys)
    if (text.find('\r') != std::string::npos) {
        std::string u;
        u.reserve(text.size());
        for (size_t i = 0; i < text.size(); i++) {
            if (text[i] != '\r') u.push_back(text[i]);
            else {
                u.push_back('\n');
                if (i + 1 < text.size() && text[i + 1] == '\n') i++;
            }
        }
        text.swap(u);
    }
    if (qual.empty()) { if (first_line) *first_line = 0; return new_str(text); }
    SrcLines s = read_lines(text);
    size_t lo = 0, hi = s.lines.size(), indent = 0;
    int at = -1;
    size_t st = 0;
    while (st <= qual.size()) {
        size_t e = qual.find('.', st);
        std::string part = qual.substr(st, e == std::string::npos ? std::string::npos : e - st);
        st = e == std::string::npos ? qual.size() + 1 : e + 1;
        if (part == "<locals>") continue;
        at = find_def(s, lo, hi, part, indent);
        if (at < 0) raise(E_OSError, "could not find class or function definition");
        lo = (size_t)at + 1;
        hi = block_end(s, (size_t)at, indent);
    }
    size_t start = (size_t)at;
    while (start > 0 && !s.continued[start - 1] && indent_of(s.lines[start - 1]) == indent && s.lines[start - 1][indent] == '@') start--;
    // (a decorator across lines: its continuation lines too)
    while (start > 0 && s.continued[start]) start--;
    std::string out;
    for (size_t i = start; i < hi; i++) out += s.lines[i];
    if (first_line) *first_line = (int64_t)start + 1;
    return new_str(out);
}

void init_inspect_module(const var& m) {
    mod_def(m, "getsource", [](const CallArgs& a) { return getsource_(a.pos.at(0), nullptr); });
    mod_def(m, "getsourcelines", [](const CallArgs& a) {
        int64_t first = 0;
        var text = getsource_(a.pos.at(0), &first);
        return new_tuple({text.call("splitlines", var(true)), var(first)});
    });
    // getfile: a module's __file__, or that of the module a function or class was defined in
    mod_def(m, "getfile", [](const CallArgs& a) -> var {
        var o = a.pos.at(0);
        if (o.is_ref() && o.ref()->type == T_module) return o.getattr("__file__");
        var mod = getattr(o, "__module__"_s, None);
        if (mod.is_none()) raise(E_TypeError, "module, class, method, function, traceback, frame, or code object was expected, got " + tname(o));
        var sm = import("sys").getattr("modules").call("get", mod);
        if (sm.is_none()) raise(E_TypeError, "no file for " + repr_(o));
        return sm.getattr("__file__");
    });
    mod_def(m, "getsourcefile", [](const CallArgs& a) { return import("inspect").call("getfile", a.pos.at(0)); });
    // signature(fn): Parameter / Signature / BoundArguments over the function's own parameter table
    var empty = make_class("_empty", "_empty", {}, new_dict(), {}, "inspect");
    mod_set(m, "_empty", empty);
    var pns = new_dict();
    const char* kinds[] = {"POSITIONAL_ONLY", "POSITIONAL_OR_KEYWORD", "VAR_POSITIONAL", "KEYWORD_ONLY", "VAR_KEYWORD"};
    for (int k = 0; k < 5; k++) pns.setitem(new_str(kinds[k]), var((int64_t)k));
    pns.setitem("empty"_s, empty);
    pns.setitem("__repr__"_s, new_native("__repr__", [](const CallArgs& a) { return new_str("<Parameter \"" + str_utf8(a.pos.at(0).getattr("name")) + "\">"); }));
    var param_cls = make_class("Parameter", "Parameter", {}, pns, {}, "inspect");
    mod_set(m, "Parameter", param_cls);
    var bns = new_dict();
    bns.setitem("args"_s, property(new_native("args", [](const CallArgs& a) {
        var self = a.pos.at(0), args = self.getattr("arguments");
        std::vector<var> out;
        for (var p : each(self.getattr("_params"))) {
            int64_t k = p.getattr("kind").as_int64();
            var n = p.getattr("name");
            if (k == 3 || k == 4) break;
            if (!in(n, args)) break;
            if (k == 2) for (var x : each(args.getitem(n))) out.push_back(x);
            else out.push_back(args.getitem(n));
        }
        return new_tuple(out);
    })));
    bns.setitem("kwargs"_s, property(new_native("kwargs", [](const CallArgs& a) {
        var self = a.pos.at(0), args = self.getattr("arguments"), out = new_dict();
        bool kw_started = false;
        for (var p : each(self.getattr("_params"))) {
            int64_t k = p.getattr("kind").as_int64();
            var n = p.getattr("name");
            if (!kw_started && (k == 3 || k == 4 || !in(n, args))) kw_started = true;
            if (!kw_started || !in(n, args)) continue;
            if (k == 4) { var d = args.getitem(n); for (var x : each(d)) out.setitem(x, d.getitem(x)); }
            else if (k != 2) out.setitem(n, args.getitem(n));
        }
        return out;
    })));
    bns.setitem("apply_defaults"_s, new_native("apply_defaults", [empty](const CallArgs& a) {
        var self = a.pos.at(0), args = self.getattr("arguments"), out = new_dict();
        for (var p : each(self.getattr("_params"))) {
            var n = p.getattr("name");
            int64_t k = p.getattr("kind").as_int64();
            if (in(n, args)) out.setitem(n, args.getitem(n));
            else if (!is(p.getattr("default"), empty)) out.setitem(n, p.getattr("default"));
            else if (k == 2) out.setitem(n, new_tuple());
            else if (k == 4) out.setitem(n, new_dict());
        }
        self.setattr("arguments", out);
        return None;
    }));
    var bound_cls = make_class("BoundArguments", "BoundArguments", {}, bns, {}, "inspect");
    var sns = new_dict();
    auto bind = [empty, bound_cls](const CallArgs& a, bool partial) -> var {
        var self = a.pos.at(0);
        auto params = collect(self.getattr("parameters").call("values"));
        var args = new_dict();
        size_t ai = 1;
        std::vector<std::pair<var, var>> kws = a.kw;
        auto take_kw = [&](const var& n, var& out) {
            for (size_t i = 0; i < kws.size(); i++)
                if (kws[i].first == n) { out = kws[i].second; kws.erase(kws.begin() + (long long)i); return true; }
            return false;
        };
        for (auto& p : params) {
            int64_t k = p.getattr("kind").as_int64();
            var n = p.getattr("name"), v;
            if (k == 0 || k == 1) {
                if (ai < a.pos.size()) {
                    if (k == 1 && take_kw(n, v)) raise(E_TypeError, "multiple values for argument '" + str_utf8(n) + "'");
                    args.setitem(n, a.pos[ai++]);
                } else if (k == 1 && take_kw(n, v)) args.setitem(n, v);
                else if (is(p.getattr("default"), empty) && !partial) raise(E_TypeError, "missing a required argument: '" + str_utf8(n) + "'");
            } else if (k == 2) {
                std::vector<var> rest(a.pos.begin() + (long long)std::min(ai, a.pos.size()), a.pos.end());
                ai = a.pos.size();
                if (!rest.empty()) args.setitem(n, new_tuple(rest));
            } else if (k == 3) {
                if (take_kw(n, v)) args.setitem(n, v);
                else if (is(p.getattr("default"), empty) && !partial) raise(E_TypeError, "missing a required argument: '" + str_utf8(n) + "'");
            } else {
                var d = new_dict();
                for (auto& [kk, vv] : kws) d.setitem(kk, vv);
                kws.clear();
                if (len_(d)) args.setitem(n, d);
            }
        }
        if (ai < a.pos.size()) raise(E_TypeError, "too many positional arguments");
        if (!kws.empty()) raise(E_TypeError, "got an unexpected keyword argument '" + str_utf8(kws[0].first) + "'");
        var b = call0(bound_cls);
        b.setattr("arguments", args);
        b.setattr("_params", new_list(params));
        b.setattr("signature", self);
        return b;
    };
    sns.setitem("bind"_s, new_native("bind", [bind](const CallArgs& a) { return bind(a, false); }));
    sns.setitem("bind_partial"_s, new_native("bind_partial", [bind](const CallArgs& a) { return bind(a, true); }));
    sns.setitem("__str__"_s, new_native("__str__", [empty](const CallArgs& a) {
        std::string s = "(";
        int i = 0;
        for (var p : each(a.pos.at(0).getattr("parameters").call("values"))) {
            int64_t k = p.getattr("kind").as_int64();
            s += i++ ? ", " : "";
            s += (k == 2 ? "*" : k == 4 ? "**" : "") + str_utf8(p.getattr("name"));
            if (!is(p.getattr("default"), empty)) s += "=" + repr_(p.getattr("default"));
        }
        return new_str(s + ")");
    }));
    var sig_cls = make_class("Signature", "Signature", {}, sns, {}, "inspect");
    mod_set(m, "Signature", sig_cls);
    mod_set(m, "BoundArguments", bound_cls);
    mod_def(m, "signature", [empty, param_cls, sig_cls](const CallArgs& a) -> var {
        var f = a.pos.at(0);
        bool drop_first = false;
        for (int depth = 0; depth < 20; depth++) {
            if (f.is_ref() && f.ref()->type == T_method) { drop_first = true; f = static_cast<MethodObj*>(f.ref())->func; continue; }
            if (f.is_ref() && f.ref()->type->layout == Layout::Type) {
                const var* init = static_cast<TypeObj*>(f.ref())->lookup("__init__");
                if (!init || !init->is_ref() || init->ref()->type != T_function) raise(E_ValueError, "no signature found for " + repr_(f));
                f = *init;
                drop_first = true;
                continue;
            }
            var w = getattr(f, "__wrapped__"_s, var());
            if (!w.is_unbound() && !w.is_none() && kwarg(a, "follow_wrapped", var(true)).truthy()) { f = w; continue; }
            break;
        }
        if (!f.is_ref() || f.ref()->type != T_function) raise(E_ValueError, "no signature found for builtin " + repr_(f));
        auto* fo = static_cast<FuncObj*>(f.ref());
        var params = new_dict();
        for (size_t i = drop_first ? 1 : 0; i < fo->params.size(); i++) {
            auto& pr = fo->params[i];
            var p = call0(param_cls);
            p.setattr("name", new_str(pr.name));
            p.setattr("kind", var((int64_t)(pr.kind == Param::PosOnly ? 0 : pr.kind == Param::Normal ? 1 : pr.kind == Param::VarArgs ? 2 : pr.kind == Param::KwOnly ? 3 : 4)));
            p.setattr("default", pr.has_default && i < fo->defaults.size() && !fo->defaults[i].is_unbound() ? fo->defaults[i] : empty);
            var* an = fo->annotations.is_ref() ? static_cast<DictObj*>(fo->annotations.ref())->get(new_str(pr.name)) : nullptr;
            p.setattr("annotation", an ? *an : empty);   // (the annotation's source text, as the translation keeps it)
            params.setitem(new_str(pr.name), p);
        }
        var s = call0(sig_cls);
        s.setattr("parameters", params);
        var* rn = fo->annotations.is_ref() ? static_cast<DictObj*>(fo->annotations.ref())->get("return"_s) : nullptr;
        s.setattr("return_annotation", rn ? *rn : empty);
        return s;
    });
    // (frames are not kept by translated code)
    mod_def(m, "currentframe", [](const CallArgs&) { return None; });
    mod_def(m, "cleandoc", [](const CallArgs& a) {
        // inspect.cleandoc: tabs expanded, the common indent of the lines after the first removed, blank edges dropped
        std::string doc = str_utf8(a.pos.at(0).call("expandtabs"));
        std::vector<std::string> lines;
        size_t st = 0;
        for (size_t i = 0; i <= doc.size(); i++)
            if (i == doc.size() || doc[i] == '\n') { lines.push_back(doc.substr(st, i - st)); st = i + 1; }
        size_t margin = std::string::npos;
        for (size_t i = 1; i < lines.size(); i++) {
            size_t k = lines[i].find_first_not_of(' ');
            if (k == std::string::npos) continue;
            margin = std::min(margin, k);
        }
        auto lstrip = [](const std::string& s) { size_t k = s.find_first_not_of(" \t"); return k == std::string::npos ? std::string() : s.substr(k); };
        if (!lines.empty()) lines[0] = lstrip(lines[0]);
        if (margin != std::string::npos)
            for (size_t i = 1; i < lines.size(); i++) lines[i] = lines[i].size() > margin ? lines[i].substr(margin) : lstrip(lines[i]);
        auto blank = [](const std::string& s) { return s.find_first_not_of(" \t\r") == std::string::npos; };
        while (!lines.empty() && blank(lines.back())) lines.pop_back();
        while (!lines.empty() && blank(lines.front())) lines.erase(lines.begin());
        std::string o;
        for (size_t i = 0; i < lines.size(); i++) o += (i ? "\n" : "") + lines[i];
        return new_str(o);
    });
    mod_def(m, "ismodule", [](const CallArgs& a) { return var(type_of(a.pos.at(0)) == T_module); });
    mod_def(m, "isclass", [](const CallArgs& a) { return var(a.pos.at(0).is_ref() && a.pos[0].ref()->type->layout == Layout::Type); });
    mod_def(m, "isfunction", [](const CallArgs& a) { return var(type_of(a.pos.at(0)) == T_function); });
    {
        // async def: coroutine functions, async generator functions, and their objects
        mod_def(m, "iscoroutinefunction", [](const CallArgs& a) {
            var f = a.pos.at(0);
            if (is_coroutine_function_(f, 1)) return var(true);
            var mark = getattr(f, "_is_coroutine_marker"_s, None);
            return var(!mark.is_none() && !mark.is_unbound());
        });
        mod_def(m, "markcoroutinefunction", [](const CallArgs& a) {
            a.pos.at(0).setattr("_is_coroutine_marker", var(true));
            return a.pos[0];
        });
        mod_def(m, "isasyncgenfunction", [](const CallArgs& a) { return var(is_coroutine_function_(a.pos.at(0), 2)); });
        mod_def(m, "isgeneratorfunction", [](const CallArgs& a) {
            var f = a.pos.at(0);
            while (f.is_ref() && f.ref()->type == T_method) f = static_cast<MethodObj*>(f.ref())->func;
            return var(f.is_ref() && f.ref()->type == T_function && static_cast<FuncObj*>(f.ref())->generator &&
                       static_cast<FuncObj*>(f.ref())->async_kind != 1 && static_cast<FuncObj*>(f.ref())->async_kind != 2);
        });
        mod_def(m, "iscoroutine", [](const CallArgs& a) { return var(is(tv(type_of(a.pos.at(0))), coroutine_type_())); });
        mod_def(m, "isasyncgen", [](const CallArgs& a) { return var(is(tv(type_of(a.pos.at(0))), async_generator_type_())); });
        mod_def(m, "isgenerator", [](const CallArgs& a) { return var(type_of(a.pos.at(0)) == T_generator); });
        mod_def(m, "isawaitable", [](const CallArgs& a) {
            var x = a.pos.at(0);
            if (is(tv(type_of(x)), coroutine_type_())) return var(true);
            var aw;
            return var(lookup_special(x, "__await__", aw));
        });
    }
    mod_def(m, "getdoc", [](const CallArgs& a) {
        var d = getattr(a.pos.at(0), "__doc__"_s, None);
        if (!is_str(d)) return var(None);
        return import("inspect").call("cleandoc", d);
    });
    for (const char* n : {"stack"})
        mod_def(m, n, [n](const CallArgs&) -> var {
            raise(E_NotImplementedError, std::string("inspect.") + n + ": the translated program has no Python source or frames to inspect");
        });
}

}  // namespace py
