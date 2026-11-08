// codebreaker/verify/concolic.cpp -- typed C++ port of codebreaker.verify.concolic (see concolic.hpp;
// explore() is a template and lives there).
#include "codebreaker/verify/concolic.hpp"

namespace codebreaker::verify::concolic {

namespace symbolic = codebreaker::emulate::symbolic;

const std::tuple<py::str, py::str> TURNED = std::tuple<py::str, py::str>{py::str("jump"), py::str("inner")};

bool _turned(const py::str& kind) {
    return kind == std::get<0>(TURNED) || kind == std::get<1>(TURNED);
}

py::str ExplorationStats::summary() const {
    py::str what = complete ? py::percent(py::str("all %d paths"), paths) : py::percent(py::str("%d paths (sampled)"), paths);
    return py::percent(py::str("%s, %d runs, %d questions (%d impossible)"), what, runs, queries, unsat);
}

z3::expr _as_went(const PathEntry& entry, symbolic::Z3Memo& memo, z3::context& ctx) {
    z3::expr t = symbolic::z3_of(entry.cond, memo, ctx);
    return entry.taken ? t : !t;
}

void _assert(z3::solver& s, const z3::expr& term) {
    s.add(term);
}

py::list<z3::expr> _edges(const symbolic::ExprP& cond, symbolic::Z3Memo& memo, z3::context& ctx) {
    const py::str& op = cond->op;
    py::list<z3::expr> out;
    if (op == py::str("bnot")) {
        return _edges(cond->args[0], memo, ctx);
    }
    if (op == py::str("nz")) {
        z3::expr x = symbolic::z3_of(cond->args[0], memo, ctx);
        out.append(x == 1);
        return out;
    }
    if (op == py::str("eq") || op == py::str("ult") || op == py::str("ule") || op == py::str("slt") || op == py::str("sle")) {
        z3::expr a = symbolic::z3_of(cond->args[0], memo, ctx);
        z3::expr b = symbolic::z3_of(cond->args[1], memo, ctx);
        // z3py's `a == b` with a numeral on the right and a non-numeral on the left goes to the
        // numeral's own reflected method: the term is built as (= b a) (see symbolic.z3_of)
        bool swapped = cond->args[1]->op == py::str("const") && cond->args[0]->op != py::str("const");
        out.append(swapped ? b == a : a == b);
        out.append(a == b + 1);
        out.append(a == b - 1);
        return out;
    }
    return out;
}

Values _model_values(const z3::model& m) {
    Values values;
    for (unsigned i = 0; i < m.size(); ++i) {
        z3::func_decl d = m[i];
        z3::expr value = m.get_const_interp(d);
        values.set(py::str(d.name().str()), static_cast<int64_t>(value.get_numeral_uint64()));
    }
    return values;
}

}  // namespace codebreaker::verify::concolic
