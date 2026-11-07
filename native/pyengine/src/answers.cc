// answers.cc -- the types of the native formats' answers, for the typed path.
//
// A method of codebreaker/formats/*.py decorated @native.ask("pe.export_table") (or a function
// @native.answered("pe.is_dotnet")) has no body: formats.dll answers it. Its registration in
// native/formats/**/questions.cc says what the answer is --
//
//     add("pe.export_table", export_table, "ExportTable{module: str, symbols: list[ExportSymbol{...}]}");
//
// (the syntax: core/questions.h). Read here, each such function's return type is that answer's: a
// record is one site per name (every ExportTable of the program is the same struct), a field marked
// `name?:` is not required, list / set / dict are container sites of their item types. A decorator
// with result= (the answer turned into something else in Python) is not bound: its type is not the
// answer's.
#include <cctype>
#include <deque>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "program.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

namespace fs = std::filesystem;
using namespace pyfront;

namespace {

struct Spec {
    enum Kind { Str, Int, BigInt, Bool, Float, Bytes, None, Opt, Tuple, List, Set, Dict, Record, Either } k = None;
    int bits = 0;                                         // a sized int: u8 u16 u32 (unsigned), i8 i16 i32
    std::string name;                                     // a record's name
    std::vector<Spec> args;                               // items / fields' types, in order
    std::vector<std::pair<std::string, bool>> fields;     // a record's (name, required)
};

struct Parser {
    const std::string& s;
    size_t i = 0;
    std::string err;
    explicit Parser(const std::string& text) : s(text) {}
    void ws() {
        while (i < s.size() && isspace((unsigned char)s[i])) i++;
    }
    bool eat(char c) {
        ws();
        if (i < s.size() && s[i] == c) { i++; return true; }
        return false;
    }
    std::string ident() {
        ws();
        size_t b = i;
        while (i < s.size() && (isalnum((unsigned char)s[i]) || s[i] == '_')) i++;
        return s.substr(b, i - b);
    }
    Spec one() {   // a type without its | alternatives or ? suffix
        Spec t;
        if (eat('(')) {
            t.k = Spec::Tuple;
            if (!eat(')')) {
                do t.args.push_back(full()); while (eat(','));
                if (!eat(')')) err = "a tuple without its )";
            }
            return t;
        }
        std::string w = ident();
        if (w.empty()) { err = "a type expected at " + std::to_string(i); return t; }
        if (w == "str") t.k = Spec::Str;
        else if (w == "int") t.k = Spec::Int;
        else if (w == "bigint") t.k = Spec::BigInt;
        else if (w == "u8" || w == "u16" || w == "u32" || w == "i8" || w == "i16" || w == "i32") {
            t.k = Spec::Int;
            t.bits = (w[0] == 'u' ? 1 : -1) * std::stoi(w.substr(1));
        }
        else if (w == "bool") t.k = Spec::Bool;
        else if (w == "float") t.k = Spec::Float;
        else if (w == "bytes") t.k = Spec::Bytes;
        else if (w == "none") t.k = Spec::None;
        else if (w == "list" || w == "set" || w == "dict") {
            t.k = w == "list" ? Spec::List : w == "set" ? Spec::Set : Spec::Dict;
            if (!eat('[')) { err = w + " without its ["; return t; }
            t.args.push_back(full());
            if (t.k == Spec::Dict) {
                if (!eat(',')) { err = "dict without its value type"; return t; }
                t.args.push_back(full());
            }
            if (!eat(']')) err = w + " without its ]";
        } else {   // Name{field: T, ...}
            t.k = Spec::Record;
            t.name = w;
            if (!eat('{')) { err = "record " + w + " without its {"; return t; }
            if (!eat('}')) {
                do {
                    std::string f = ident();
                    bool required = !eat('?');
                    if (f.empty() || !eat(':')) { err = "record " + w + ": a field without its name or :"; return t; }
                    t.fields.push_back({f, required});
                    t.args.push_back(full());
                } while (eat(','));
                if (!eat('}')) err = "record " + w + " without its }";
            }
        }
        return t;
    }
    Spec full() {
        Spec t = one();
        while (eat('|')) {   // A|B
            Spec e;
            e.k = Spec::Either;
            e.args.push_back(t);
            e.args.push_back(one());
            t = e;
        }
        if (eat('?')) {
            Spec o;
            o.k = Spec::Opt;
            o.args.push_back(t);
            t = o;
        }
        return t;
    }
};

// a C++ string literal's text (the escapes questions.cc uses)
std::string literal(const std::string& src, size_t& i) {
    std::string out;
    for (i++; i < src.size() && src[i] != '"'; i++) {
        if (src[i] == '\\' && i + 1 < src.size()) i++;
        out.push_back(src[i]);
    }
    i++;
    return out;
}

// question -> answer type text, from every add("q", fn, "type" "type"...) of the questions.cc files
std::map<std::string, std::string> registrations(const fs::path& formats) {
    std::map<std::string, std::string> out;
    std::error_code ec;
    for (auto it = fs::recursive_directory_iterator(formats, ec); !ec && it != fs::recursive_directory_iterator(); it.increment(ec)) {
        if (it->path().extension() != ".cc") continue;   // (questions.cc, and the readers that register their own)
        std::ifstream f(it->path(), std::ios::binary);
        std::stringstream ss;
        ss << f.rdbuf();
        const std::string src = ss.str();
        for (size_t at = src.find("add(\""); at != std::string::npos; at = src.find("add(\"", at + 5)) {
            if (at > 0 && (isalnum((unsigned char)src[at - 1]) || src[at - 1] == '_')) continue;
            size_t i = at + 4;
            std::string q = literal(src, i);
            size_t close = src.find(')', i);
            size_t comma = src.find(',', i);           // after the question
            if (comma == std::string::npos || comma > close) continue;
            size_t next = src.find(',', comma + 1);    // after the function: the answer's type follows
            size_t end = src.find(");", comma);
            if (next == std::string::npos || end == std::string::npos || next > end) continue;
            std::string type;
            for (size_t j = next + 1; j < end;) {
                if (src[j] == '"') type += literal(src, j);
                else j++;
            }
            if (!type.empty()) out[q] = type;
        }
    }
    return out;
}

struct Binder {
    Program& p;
    std::string scope;                                     // whose sites: "" the shared ones, else a call's
    std::string module;                                    // where its records are declared: the stub's module
    std::vector<int> made_now;                             // the sites this binding made (pinned when filled)
    std::map<std::string, int> record_sites;              // a record's name -> its site (in this scope)
    static std::map<std::string, int>& made() {           // scope + path -> the site made for it (once)
        static std::map<std::string, int> m;
        return m;
    }
    // the node identities of the sites made here: they live as long as the program (a site is found by
    // its node's address, so a freed one would hand a later container this site)
    static std::deque<std::string>& keys() {
        static std::deque<std::string> k;
        return k;
    }
    explicit Binder(Program& prog) : p(prog) {}

    int site(const std::string& key, TK kind) {
        auto it = made().find(scope + "|" + key);
        if (it != made().end()) return it->second;
        keys().push_back("answer:" + scope + "|" + key);
        int st = p.new_site(&keys().back(), kind, {module, 0});
        made()[scope + "|" + key] = st;
        made_now.push_back(st);
        return st;
    }
    T type(const Spec& s, const std::string& where) {
        switch (s.k) {
            case Spec::Str: return STR();
            case Spec::Int:
                if (s.bits > 0) return u(s.bits);
                if (s.bits < 0) return pyengine::s(-s.bits);
                return INT(I64_MIN, I64_MAX);
            case Spec::BigInt: return INT();
            case Spec::Bool: return BOOL();
            case Spec::Float: return FLOAT();
            case Spec::Bytes: return BYTES();
            case Spec::None: return NONE();
            case Spec::Opt: return opt(type(s.args[0], where));
            case Spec::Tuple: {
                std::vector<T> ts;
                for (size_t i = 0; i < s.args.size(); i++) ts.push_back(type(s.args[i], where + "." + std::to_string(i)));
                return tuple_of(ts);
            }
            case Spec::Either: return p.variant_join(type(s.args[0], where), type(s.args[1], where));
            case Spec::List: case Spec::Set: {
                int st = site(where, s.k == Spec::List ? TK::List : TK::Set);
                p.grow(p.elem, st, type(s.args[0], where + "[]"));
                return container(s.k == Spec::List ? TK::List : TK::Set, st);
            }
            case Spec::Dict: {
                int st = site(where, TK::Dict);
                p.make_dynamic(st, "a dict a native question gives");
                p.grow(p.keys, st, type(s.args[0], where + "{k}"));
                p.grow(p.elem, st, type(s.args[1], where + "{v}"));
                return container(TK::Dict, st);
            }
            case Spec::Record: {
                auto it = record_sites.find(scope + "|" + s.name);
                if (it != record_sites.end()) return container(TK::Dict, it->second);
                bool fresh = !made().count(scope + "|" + s.name);
                int st = site(s.name, TK::Dict);
                if (!fresh) {   // (made in an earlier round: its fields are there)
                    record_sites[scope + "|" + s.name] = st;
                    return container(TK::Dict, st);
                }
                record_sites[scope + "|" + s.name] = st;
                p.hint(st, s.name);
                std::vector<std::string> required;
                for (size_t i = 0; i < s.fields.size(); i++) {
                    T ft = type(s.args[i], s.name + "." + s.fields[i].first);
                    if (!s.fields[i].second) ft = opt(ft);
                    else required.push_back(s.fields[i].first);
                    p.fields[p.find(st)].set(s.fields[i].first, ft);   // (not grow_field: its dict-of-all-values join would
                                                                        // poison a record of mixed fields)
                }
                p.grow(p.keys, st, STR());
                p.created(st, required);
                return container(TK::Dict, st);
            }
        }
        return top("an answer type not read");
    }
};

// what a @native.ask / @native.answered decorator says: its question, and where the call's bytes come from --
// "self:<attribute>" (ask: data="data" by default), "arg:<i>" (answered: data_arg=0 by default), "none"
// (data_arg=None). A decorator with result=, where= or args_of= is not bound (its answer is turned into
// something else, or the call has arguments of its own making): question "" then.
struct Deco {
    std::string question, data;
};
Deco deco_of(const A::Expr* d) {
    Deco out;
    auto* c = as<pyfront::Call>(d);
    if (!c || c->args.empty()) return out;
    auto* fa = as<Attribute>(c->func);
    if (!fa || (fa->attr != "ask" && fa->attr != "answered")) return out;
    auto* q = as<Constant>(c->args[0]);
    if (!q || q->value.kind != ConstValue::Kind::Str) return out;
    bool ask = fa->attr == "ask";
    out.data = ask ? "self:data" : "arg:0";
    for (auto* k : c->keywords) {
        if (!k->has_arg || k->arg == "result" || k->arg == "where" || k->arg == "args_of") return Deco{};
        auto* v = as<Constant>(k->value);
        if (k->arg == "data" && ask && v && v->value.kind == ConstValue::Kind::Str) out.data = "self:" + v->value.text;
        else if (k->arg == "data_arg" && !ask && v && v->value.kind == ConstValue::Kind::None) out.data = "none";
        else if (k->arg == "data_arg" && !ask && v && v->value.kind == ConstValue::Kind::Int) out.data = "arg:" + v->value.text;
        else return Deco{};
    }
    out.question = q->value.text;
    return out;
}

std::map<std::string, Spec>& specs() {   // a bound function -> its answer's type
    static std::map<std::string, Spec> m;
    return m;
}

}  // namespace

// the answer's type for one call: sites of its own (a caller that changes its answer -- adds a key to a
// record, pops one -- changes that call's, not every caller's), the same on every round
T native_answer_at(Program& p, const std::string& fq, const void* call) {
    auto it = specs().find(fq);
    if (it == specs().end()) return p.ret(fq);
    Binder b(p);
    char buf[32];
    snprintf(buf, sizeof buf, "%p", call);
    b.scope = buf;
    return b.type(it->second, p.native_bound[fq]);
}

size_t bind_native_answers(Program& p, const std::string& formats_dir, FILE* log) {
    std::map<std::string, std::string> regs = registrations(fs::path(formats_dir));
    Binder b(p);
    size_t bound = 0;
    for (auto& [q, f] : p.fns) {
        for (auto* d : f->node->decorator_list) {
            Deco dc = deco_of(d);
            const std::string& question = dc.question;
            if (question.empty()) continue;
            auto r = regs.find(question);
            if (r == regs.end()) continue;
            Parser ps(r->second);
            Spec s = ps.full();
            ps.ws();
            if (!ps.err.empty() || ps.i != r->second.size()) {
                if (log) fprintf(log, "answer type of %s not read: %s\n", question.c_str(), ps.err.empty() ? "text after the type" : ps.err.c_str());
                continue;
            }
            b.module = f->module->name;
            b.scope = f->module->name;   // (a record is one struct per module that answers it)
            p.returns[q] = b.type(s, question);
            for (int st : b.made_now) p.pinned.insert(p.find(st));
            b.made_now.clear();
            specs()[q] = s;
            p.native_bound[q] = question;
            p.native_data[q] = dc.data;
            bound++;
        }
    }
    if (log) fprintf(log, "native answers: %zu registered with a type, %zu functions bound\n", regs.size(), bound);
    return bound;
}

}  // namespace pyengine
