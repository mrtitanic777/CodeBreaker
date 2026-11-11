// codebreaker/decompile/shape16.cpp -- typed C++ from codebreaker.decompile.shape16 (see the header).
//
// A 16-bit function's statements in the form its instructions say they were written in: `inc` / `dec` of a variable's
// place is x++ / x--, and an if-whose-then-part-returns followed by a return is laid out the way C 8 lays it out.
#include "codebreaker/decompile/shape16.hpp"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/decompile/l2tree.hpp"

namespace codebreaker::decompile::shape16 {

namespace {

namespace T = l2tree;
using translate::ir::Dict;
using translate::ir::Val;
using List = Val::List;

// every list of statements under a node: the dict members named body / then / orelse / stmts that hold a list, a
// list before the lists inside it (the Python's generator, run to the end)
void lists(const Val& n, py::list<List>& out) {
    if (T::is_dict(n)) {
        for (const auto& kv : T::as_dict(n).items()) {
            const py::str& k = std::get<0>(kv);
            const Val& v = std::get<1>(kv);
            if ((k == "body" || k == "then" || k == "orelse" || k == "stmts") && T::is_list(v)) out.append(T::as_list(v));
            lists(v, out);
        }
    } else if (T::is_list(n)) {
        for (const Val& x : T::as_list(n).vec()) lists(x, out);
    }
}

// sorted(a & 0xFFFF for a in (s.get("origin") or {}).get("at") or ())
std::vector<int64_t> at_of(const Dict& s) {
    std::vector<int64_t> out;
    Val at = T::get(T::dict_or_empty(T::get(s, "origin")), "at");
    if (T::is_list(at)) {
        for (const Val& a : T::as_list(at).vec()) {
            if (auto i = std::get_if<int64_t>(&a.v)) out.push_back(*i & 0xFFFF);
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

// The mnemonic of each instruction at `at` -- an inc or dec of anything but a place in the frame ([bp-k]: a register, a
// global) as "inc reg".
std::vector<std::string> mnemonics(const py::bytes& code, const std::vector<int64_t>& at) {
    auto md = describe::decoder16();
    std::vector<std::string> out;
    for (int64_t a : at) {
        py::list<describe::Insn> got = md->disasm(code.slice(a, a + 8), a);
        for (const describe::Insn& i : got.vec()) {
            std::string m = i.mnemonic.utf8();
            if ((m == "inc" || m == "dec") && !i.op_str.contains(py::str("[bp"))) m += " reg";
            out.push_back(m);
            break;
        }
    }
    return out;
}

// "inc" / "dec" where the assignment is x = x + 1 / x - 1 (x + -1) of its own target.
std::optional<std::string> steps(const Dict& e) {
    Dict v = T::dict_or_empty(T::get(e, "value")), t = T::dict_or_empty(T::get(e, "target"));
    if (!T::is_text(T::get(v, "node"), "Binary")) return std::nullopt;
    Val op = T::get(v, "op");
    if (!(T::is_text(op, "+") || T::is_text(op, "-"))) return std::nullopt;
    Dict left = T::dict_or_empty(T::get(v, "left")), right = T::dict_or_empty(T::get(v, "right"));
    Val lname = T::get(T::dict_or_empty(T::get(left, "sym")), "name");
    Val tname = T::get(T::dict_or_empty(T::get(t, "sym")), "name");
    Val rv = T::get(right, "value");
    if (!T::is_text(T::get(left, "node"), "Name") || !T::equal(lname, tname) || !T::is_text(T::get(right, "node"), "Lit") ||
        !(T::equals_number(rv, 1) || T::equals_number(rv, -1))) {
        return std::nullopt;
    }
    return (T::is_text(op, "+") == T::equals_number(rv, 1)) ? "inc" : "dec";
}

bool is_return(const Val& v) { return T::is_dict(v) && T::is_text(T::get(v, "node"), "Return"); }

}  // namespace

std::tuple<translate::ir::Dict, py::list<py::str>> reshape(const translate::ir::Dict& doc_in, const py::bytes& code) {
    Dict doc = T::deepcopy(doc_in);
    py::list<py::str> said;
    T::walk(T::get(doc, "body"), [&](const Dict& st) {
        if (!T::is_text(T::get(st, "node"), "ExprStmt") || !T::is_text(T::get(T::dict_or_empty(T::get(st, "expr")), "node"), "Assign")) return;
        Dict e = T::as_dict(T::get(st, "expr"));
        if (T::truthy(T::get(e, "op")) || !T::is_text(T::get(T::dict_or_empty(T::get(e, "target")), "node"), "Name")) return;
        std::vector<std::string> ms = mnemonics(code, at_of(st));
        std::optional<std::string> step = steps(e);
        if (ms.size() == 1 && (ms[0] == "inc reg" || ms[0] == "dec reg") && step && *step == ms[0].substr(0, 3)) {
            ms = {ms[0].substr(0, 3)};
        }
        if (ms.size() == 1 && (ms[0] == "inc" || ms[0] == "dec") &&
            ((step && *step == ms[0]) || T::is_text(T::get(T::dict_or_empty(T::get(e, "value")), "node"), "Lit"))) {
            Val target = T::at(e, "target");
            Dict un;
            un.set("node", Val("Unary"));
            un.set("op", Val(ms[0] == "inc" ? "++" : "--"));
            un.set("postfix", Val(true));
            un.set("x", target);
            un.set("type", T::get(target, "type"));
            st.set("expr", Val(un));
            Val name = T::at(T::dict_or_empty(T::at(T::as_dict(target), "sym")), "name");
            said.append(T::is_str(name) ? std::get<py::str>(name.v) + py::str(ms[0] == "inc" ? "++" : "--") : py::str());
        }
    });
    // (a Dict handle may be held by a Val: reading `body` again, the Python's doc.get("body") or [])
    py::list<List> bodies;
    List top = T::list_or_empty(T::get(doc, "body"));
    bodies.append(top);
    lists(T::get(doc, "body"), bodies);
    for (const List& body : bodies.vec()) {
        for (int64_t k = 0; k + 1 < body.size(); k++) {
            Val av = body[k], bv = body[k + 1];
            if (!T::is_dict(av)) continue;
            Dict a = T::as_dict(av);
            Val thenv = T::get(a, "then");
            List then;
            bool then_is_dict = T::is_dict(thenv);
            if (then_is_dict) then.append(thenv);
            else if (T::is_list(thenv)) then = T::as_list(thenv);
            if (!T::is_text(T::get(a, "node"), "If") || T::truthy(T::get(a, "orelse")) || then.empty() || then.size() != 1 ||
                !is_return(then[0]) || !is_return(bv)) {
                continue;
            }
            std::vector<int64_t> ta = at_of(T::as_dict(then[0])), tb = at_of(T::as_dict(bv));
            if (!ta.empty() && !tb.empty() && ta[0] > tb[0]) {   // (the original has the other return first)
                Val cond = T::at(a, "cond");
                if (T::is_dict(cond) && T::is_text(T::get(cond, "node"), "Unary") && T::is_text(T::get(cond, "op"), "!")) {
                    cond = T::at(T::as_dict(cond), "x");
                } else {
                    Dict n;
                    n.set("node", Val("Unary"));
                    n.set("op", Val("!"));
                    n.set("x", cond);
                    Dict ty;
                    ty.set("kind", Val("bool"));
                    n.set("type", Val(ty));
                    cond = Val(n);
                }
                Dict repl = a.copy();
                repl.set("cond", cond);
                if (T::is_list(thenv)) {
                    List one;
                    one.append(bv);
                    repl.set("then", Val(one));
                } else {
                    repl.set("then", bv);
                }
                body[k] = Val(repl);
                body[k + 1] = then[0];
                said.append(py::str("an if-return turned round"));
            }
        }
    }
    return {doc, said};
}

}  // namespace codebreaker::decompile::shape16
