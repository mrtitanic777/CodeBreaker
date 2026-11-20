#include "invert/msvc2_battery.hpp"

namespace cb::invert::msvc2 {

namespace {

Term constant(long long v) { return Term::tuple({Term::string("const"), Term::integer(v)}); }
Term arg(long long k) { return Term::tuple({Term::string("arg"), Term::integer(k)}); }
Term binop(const char* op, const Term& l, const Term& r) { return Term::tuple({Term::string("binop"), Term::string(op), l, r}); }
Term compare(const char* op, const Term& l, const Term& r) { return Term::tuple({Term::string("cmp"), Term::string(op), l, r}); }
Term unary(const char* op, const Term& x) { return Term::tuple({Term::string("unary"), Term::string(op), x}); }
Term kind_of(const char* kind) { return Term::tuple({Term::string(kind)}); }
Term kind_of(const char* kind, const Term& a) { return Term::tuple({Term::string(kind), a}); }
Term kind_of(const char* kind, const Term& a, const Term& b) { return Term::tuple({Term::string(kind), a, b}); }
Term call(const char* convention, std::vector<Term> args) { return Term::tuple({Term::string("call"), Term::string(convention), Term::list(std::move(args))}); }
Term indirect_call(const char* convention, std::vector<Term> args) { return Term::tuple({Term::string("icall"), Term::string(convention), Term::list(std::move(args))}); }
Term call_with(const char* op, const char* convention, std::vector<Term> args, const Term& other) {
    return Term::tuple({Term::string("callbin"), Term::string(op), Term::string(convention), Term::list(std::move(args)), other});
}

Term function(long long nargs, const Term& expr) {
    Term d = Term::dict();
    d.set("nargs", Term::integer(nargs));
    d.set("expr", expr);
    return d;
}

// a op b op c ... over the arguments `idxs`, grouped to the left
Term chain(const char* op, std::initializer_list<long long> idxs) {
    auto it = idxs.begin();
    Term e = arg(*it++);
    for (; it != idxs.end(); ++it) e = binop(op, e, arg(*it));
    return e;
}

}  // namespace

std::vector<Case> battery() {
    std::vector<Case> cases;
    const auto add = [&](std::string name, long long nargs, const Term& expr) { cases.push_back({std::move(name), function(nargs, expr)}); };
    add("ret0", 0, constant(0));
    add("ret1", 0, constant(1));
    add("retBig", 0, constant(0x1234));
    add("retNeg", 0, constant(-7));
    add("arg1", 1, arg(1));
    add("arg2of2", 2, arg(2));
    for (const char* op : {"+", "-", "&", "|", "^", "*"}) {
        add(std::string("a") + op + "c", 1, binop(op, arg(1), constant(5)));
        add(std::string("a") + op + "b", 2, binop(op, arg(1), arg(2)));
    }
    add("a*b*c", 3, chain("*", {1, 2, 3}));
    add("a*b*c*d", 4, chain("*", {1, 2, 3, 4}));
    add("a+b+c", 3, binop("+", binop("+", arg(1), arg(2)), arg(3)));
    add("a+b-c", 3, binop("-", binop("+", arg(1), arg(2)), arg(3)));
    add("a-b+c", 3, binop("+", binop("-", arg(1), arg(2)), arg(3)));
    add("a*b+c", 3, binop("+", binop("*", arg(1), arg(2)), arg(3)));
    add("a+b*c", 3, binop("+", arg(1), binop("*", arg(2), arg(3))));
    add("*p", 1, kind_of("deref", arg(1)));
    add("p[2]", 1, kind_of("index", arg(1), constant(2)));
    add("p[0]", 1, kind_of("index", arg(1), constant(0)));
    for (const char* o : {"<", "<=", ">", ">=", "==", "!="}) add(std::string("a") + o + "b", 2, compare(o, arg(1), arg(2)));
    add("a+1", 1, binop("+", arg(1), constant(1)));
    add("a-1", 1, binop("-", arg(1), constant(1)));
    add("store", 2, kind_of("store", arg(1), arg(2)));
    add("store0", 1, kind_of("store", arg(1), constant(0)));
    add("a-b-c", 3, chain("-", {1, 2, 3}));
    add("a-b-c-d", 4, chain("-", {1, 2, 3, 4}));
    add("a-b-c-d-e", 5, chain("-", {1, 2, 3, 4, 5}));
    add("p[i]", 2, kind_of("index", arg(1), arg(2)));
    add("a+b+c+d", 4, chain("+", {1, 2, 3, 4}));
    add("a+b+c+d+e", 5, chain("+", {1, 2, 3, 4, 5}));
    add("(a+b)*c", 3, binop("*", binop("+", arg(1), arg(2)), arg(3)));
    add("a*b+c*d", 4, binop("+", binop("*", arg(1), arg(2)), binop("*", arg(3), arg(4))));
    add("a&b&c", 3, chain("&", {1, 2, 3}));
    add("a*b-c*d", 4, binop("-", binop("*", arg(1), arg(2)), binop("*", arg(3), arg(4))));
    add("g()", 0, call("__stdcall", {}));
    add("g(a)", 1, call("__stdcall", {arg(1)}));
    add("g(a,b)", 2, call("__stdcall", {arg(1), arg(2)}));
    add("g(a,b,c)", 3, call("__stdcall", {arg(1), arg(2), arg(3)}));
    add("g_cdecl(a)", 1, call("__cdecl", {arg(1)}));
    add("getG", 0, Term::tuple({Term::string("gread"), Term::string("G")}));
    add("setGc", 0, Term::tuple({Term::string("gwrite"), Term::string("G"), constant(5)}));
    add("setG", 1, Term::tuple({Term::string("gwrite"), Term::string("G"), arg(1)}));
    add("incG", 0, Term::tuple({Term::string("gincdec"), Term::string("G"), Term::integer(1)}));
    add("P(a)", 1, indirect_call("__stdcall", {arg(1)}));
    add("P(a,b)", 2, indirect_call("__stdcall", {arg(1), arg(2)}));
    add("empty0", 0, kind_of("empty"));
    add("empty1", 1, kind_of("empty"));
    add("empty2", 2, kind_of("empty"));
    add("neg", 1, unary("neg", arg(1)));
    add("not", 1, unary("not", arg(1)));
    add("lognot", 1, unary("lognot", arg(1)));
    add("a*a", 1, binop("*", arg(1), arg(1)));
    add("g(a)+b", 2, call_with("+", "__stdcall", {arg(1)}, arg(2)));
    add("g(a)+c", 1, call_with("+", "__stdcall", {arg(1)}, constant(1)));
    add("g(a,b)&c", 2, call_with("&", "__stdcall", {arg(1), arg(2)}, constant(7)));
    return cases;
}

std::vector<Case> declined_battery() {
    return {{"a/b", function(2, binop("/", arg(1), arg(2)))}, {"a<<b", function(2, binop("<<", arg(1), arg(2)))}};
}

std::vector<FrameCase> frame_battery() {
    const std::string g = "extern void __stdcall g(void*); ";
    return {
        {"1int", g + "int __stdcall f(void){ int a; g(&a); return a; }", {{4, false}}},
        {"2int", g + "int __stdcall f(void){ int a,b; g(&a); g(&b); return a+b; }", {{4, false}, {4, false}}},
        {"3int", g + "int __stdcall f(void){ int a,b,c; g(&a); g(&b); g(&c); return a+b+c; }", {{4, false}, {4, false}, {4, false}}},
        {"2int_rev", g + "int __stdcall f(void){ int a,b; g(&b); g(&a); return a+b; }", {{4, false}, {4, false}}},
        {"buf8", g + "int __stdcall f(void){ char buf[8]; g(buf); return 0; }", {{8, true}}},
        {"buf10", g + "int __stdcall f(void){ char buf[10]; g(buf); return 0; }", {{10, true}}},
        {"int_buf", g + "int __stdcall f(void){ int a; char buf[8]; g(&a); g(buf); return a; }", {{4, false}, {8, true}}},
        {"buf_int", g + "int __stdcall f(void){ char buf[6]; int a; g(buf); g(&a); return a; }", {{6, true}, {4, false}}},
    };
}

}  // namespace cb::invert::msvc2
