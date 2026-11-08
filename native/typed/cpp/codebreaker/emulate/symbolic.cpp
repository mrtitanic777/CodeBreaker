// codebreaker/emulate/symbolic.cpp -- C++17 port of codebreaker.emulate.symbolic (see the header).

#include "codebreaker/emulate/symbolic.hpp"

namespace codebreaker::emulate::symbolic {

namespace {

bool is(const ExprP& e, const char* op) { return e->op == py::str(op); }

ExprP make(const py::str& op, int64_t bits, std::vector<ExprP> args) {
    auto e = std::make_shared<Expr>();
    e->op = op;
    e->bits = bits;
    e->args = std::move(args);
    return e;
}

}  // namespace

const ExprP TRUE = make(py::str("true"), 0, {});

ExprP var(const py::str& name, int64_t bits) {
    auto e = std::make_shared<Expr>();
    e->op = py::str("var");
    e->bits = bits;
    e->name = name;
    return e;
}

ExprP const_(const int64_t value, int64_t bits) {
    if (bits < 0) throw py::ValueError("negative shift count");
    // value & ((1 << bits) - 1)
    uint64_t mask = bits >= 64 ? ~uint64_t(0) : (uint64_t(1) << bits) - 1;
    auto e = std::make_shared<Expr>();
    e->op = py::str("const");
    e->bits = bits;
    e->value = static_cast<int64_t>(static_cast<uint64_t>(value) & mask);
    return e;
}

int64_t bits(const ExprP& e) { return e->bits; }

ExprP binary(const py::str& op, const ExprP& a, const ExprP& b) { return make(op, a->bits, {a, b}); }

ExprP unary(const py::str& op, const ExprP& a) { return make(op, a->bits, {a}); }

ExprP zext(const ExprP& a, int64_t to) { return a->bits == to ? a : make(py::str("zext"), to, {a}); }

ExprP sext(const ExprP& a, int64_t to) { return a->bits == to ? a : make(py::str("sext"), to, {a}); }

ExprP extract(const ExprP& a, int64_t lo, int64_t width) {
    if (lo == 0 && width == a->bits) return a;
    if (is(a, "concat")) {   // (a part of a part: that part)
        const ExprP& hi_part = a->args[0];
        const ExprP& lo_part = a->args[1];
        if (lo >= lo_part->bits) return extract(hi_part, lo - lo_part->bits, width);
        if (lo + width <= lo_part->bits) return extract(lo_part, lo, width);
    }
    if (is(a, "zext") && lo + width <= a->args[0]->bits) return extract(a->args[0], lo, width);
    if (is(a, "extract")) return extract(a->args[0], a->lo + lo, width);
    auto e = std::make_shared<Expr>();
    e->op = py::str("extract");
    e->bits = width;
    e->args = {a};
    e->lo = lo;
    return e;
}

ExprP concat(const ExprP& hi, const ExprP& lo) {
    if (is(hi, "extract") && is(lo, "extract") && hi->args[0] == lo->args[0] && hi->lo == lo->lo + lo->bits) {
        return extract(hi->args[0], lo->lo, hi->bits + lo->bits);
    }
    if (is(hi, "const") && is(lo, "const")) {
        // (hi.value << lo.bits) | lo.value, over int64: a result past 64 bits does not fit
        uint64_t h = static_cast<uint64_t>(hi->value);
        uint64_t l = static_cast<uint64_t>(lo->value);
        if (h != 0 && lo->bits > 0 && (lo->bits >= 64 || (h >> (64 - lo->bits)) != 0)) throw py::OverflowError("int too large for int64");
        uint64_t v = (h == 0 ? 0 : h << lo->bits) | l;
        return const_(static_cast<int64_t>(v), hi->bits + lo->bits);
    }
    return make(py::str("concat"), hi->bits + lo->bits, {hi, lo});
}

ExprP to_bool(const ExprP& c) { return make(py::str("bool"), 8, {c}); }

ExprP nonzero(const ExprP& e) {
    if (is(e, "bool")) return e->args[0];
    if (is(e, "zext") && is(e->args[0], "bool")) return e->args[0]->args[0];
    return make(py::str("nz"), 0, {e});
}

ExprP compare(const py::str& op, const ExprP& a, const ExprP& b) {
    if (op == py::str("eq")) {
        // (a boolean byte against 0 or 1)
        const ExprP* pairs[2][2] = {{&a, &b}, {&b, &a}};
        for (auto& p : pairs) {
            const ExprP& x = *p[0];
            const ExprP& y = *p[1];
            if (is(x, "bool") && is(y, "const")) {
                if (y->value == 1) return x->args[0];
                if (y->value == 0) return negate(x->args[0]);
                return make(py::str("eq"), 0, {x, y});
            }
        }
    }
    return make(op, 0, {a, b});
}

ExprP negate(const ExprP& c) {
    if (is(c, "bnot")) return c->args[0];
    return make(py::str("bnot"), 0, {c});
}

ExprP logic(const py::str& op, const ExprP& x, const ExprP& y) { return make(op, 0, {x, y}); }

py::set<py::str> inputs(const ExprP& e, InputsMemo& memo) {
    auto got = memo.find(e.get());
    if (got != memo.end()) return got->second.second;
    py::set<py::str> out;
    if (is(e, "var")) {
        out.add(*e->name);
    } else {
        for (const ExprP& x : e->args) {
            py::set<py::str> sub = inputs(x, memo);
            for (const py::str& n : sub) out.add(n);   // out |= inputs(x)
        }
    }
    memo.emplace(e.get(), std::make_pair(e, out));
    return out;
}

z3::context& main_context() {
    static z3::context ctx;
    return ctx;
}

z3::expr z3_of(const ExprP& e, Z3Memo& memo, z3::context& ctx) {
    auto got = memo.find(e.get());
    if (got != memo.end()) return got->second.second;
    const py::str& op = e->op;
    // (z3 numerals are made from their digits: any width)
    auto bv_val = [&](int64_t value, int64_t width) {
        return ctx.bv_val(std::to_string(static_cast<uint64_t>(value)).c_str(), static_cast<unsigned>(width));
    };
    std::optional<z3::expr> t;
    if (op == py::str("var")) {
        t = ctx.bv_const(e->name->utf8().c_str(), static_cast<unsigned>(e->bits));
    } else if (op == py::str("const")) {
        t = bv_val(e->value, e->bits);
    } else if (op == py::str("true")) {
        t = ctx.bool_val(true);
    } else {
        std::vector<z3::expr> args;
        for (const ExprP& x : e->args) args.push_back(z3_of(x, memo, ctx));
        auto bits_of = [&](size_t i) { return static_cast<unsigned>(e->args[i]->bits); };
        // z3py's `a == b`, `a < b`, `a <= b` with a numeral on the right and a non-numeral on the left
        // go to the numeral's own reflected method (Python tries a subclass's first): the term is built
        // as (= b a), (bvsgt b a), (bvsge b a). The same truth, the solver's term in Python's order.
        bool swapped = e->args.size() == 2 && is(e->args[1], "const") && !is(e->args[0], "const");
        if (op == py::str("add")) t = args[0] + args[1];
        else if (op == py::str("sub")) t = args[0] - args[1];
        else if (op == py::str("and")) t = args[0] & args[1];
        else if (op == py::str("or")) t = args[0] | args[1];
        else if (op == py::str("xor")) t = args[0] ^ args[1];
        else if (op == py::str("mul")) t = args[0] * args[1];
        else if (op == py::str("udiv")) t = z3::udiv(args[0], args[1]);
        else if (op == py::str("urem")) t = z3::urem(args[0], args[1]);
        else if (op == py::str("sdiv")) t = args[0] / args[1];   // (a / b on bit-vectors: signed division)
        else if (op == py::str("srem")) t = z3::srem(args[0], args[1]);
        else if (op == py::str("shl")) t = z3::shl(args[0], args[1]);
        else if (op == py::str("lshr")) t = z3::lshr(args[0], args[1]);
        else if (op == py::str("ashr")) t = z3::ashr(args[0], args[1]);
        else if (op == py::str("neg")) t = -args[0];
        else if (op == py::str("not")) t = ~args[0];
        else if (op == py::str("zext")) t = z3::zext(args[0], static_cast<unsigned>(e->bits - e->args[0]->bits));
        else if (op == py::str("sext")) t = z3::sext(args[0], static_cast<unsigned>(e->bits - e->args[0]->bits));
        else if (op == py::str("extract")) t = args[0].extract(static_cast<unsigned>(e->lo + e->bits - 1), static_cast<unsigned>(e->lo));
        else if (op == py::str("concat")) t = z3::concat(args[0], args[1]);
        else if (op == py::str("ite")) t = z3::ite(args[0], args[1], args[2]);
        else if (op == py::str("bool")) t = z3::ite(args[0], bv_val(1, 8), bv_val(0, 8));
        else if (op == py::str("popcount")) {
            const z3::expr& a = args[0];
            int64_t width = e->args[0]->bits;
            z3::expr acc = bv_val(0, e->bits);
            for (int64_t k = 0; k < width; ++k) {
                acc = acc + z3::zext(a.extract(static_cast<unsigned>(k), static_cast<unsigned>(k)), static_cast<unsigned>(e->bits - 1));
            }
            t = acc;
        } else if (op == py::str("eq")) t = swapped ? args[1] == args[0] : args[0] == args[1];
        else if (op == py::str("ult")) t = z3::ult(args[0], args[1]);
        else if (op == py::str("ule")) t = z3::ule(args[0], args[1]);
        else if (op == py::str("slt")) t = swapped ? args[1] > args[0] : args[0] < args[1];    // (< on bit-vectors: signed)
        else if (op == py::str("sle")) t = swapped ? args[1] >= args[0] : args[0] <= args[1];
        else if (op == py::str("bnot")) t = !args[0];
        else if (op == py::str("band") || op == py::str("bor")) {
            z3::expr_vector vec(ctx);
            for (const z3::expr& x : args) vec.push_back(x);
            t = op == py::str("band") ? z3::mk_and(vec) : z3::mk_or(vec);
        } else if (op == py::str("bxor")) t = args[0] ^ args[1];
        else if (op == py::str("nz")) t = args[0] != ctx.bv_val(0, bits_of(0));
        else throw py::ValueError(py::percent("no such expression: %r", op));
    }
    memo.emplace(e.get(), std::make_pair(e, *t));   // (e kept: its address stays its own)
    return *t;
}

z3::expr z3_of(const ExprP& e, Z3Memo& memo) { return z3_of(e, memo, main_context()); }

}  // namespace codebreaker::emulate::symbolic
