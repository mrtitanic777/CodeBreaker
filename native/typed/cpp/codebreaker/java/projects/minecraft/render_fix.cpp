// codebreaker/java/projects/minecraft/render_fix.cpp -- C++ port of codebreaker.java.projects.minecraft.render_fix
// (see the header).

#include "codebreaker/java/projects/minecraft/render_fix.hpp"

#include <array>
#include <map>
#include <memory>
#include <set>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::render_fix {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

const py::re::Pattern _UPDATE_RENDERERS = py::re::compile(py::str(
    R"RX(int\s+(\w+)\s*=\s*this->(\w+)\.size\(\)\s*-\s*1;\s*)RX"          // int var2 = this->FIELD.size() - 1;
    R"RX(int\s+(\w+)\s*=\s*this->\2\.size\(\);\s*)RX"                     // int var3 = this->FIELD.size();
    R"RX(for\s*\(int\s+(\w+)\s*=\s*0;\s*\4\s*<\s*\3;\s*\+\+\4\)\s*\{\s*)RX"
    R"RX((\w+)\*\s*(\w+)\s*=\s*\(\5\*\)this->\2\[\1\s*-\s*\4\];\s*)RX"    // WorldRenderer* var5 = (..)FIELD[var2-var4];
    R"RX(if\(\6->distanceToEntitySquared\((\w+)\)\s*>\s*2500\.0F\s*&&\s*\4\s*>\s*4\)\s*\{\s*)RX"
    R"RX(return;\s*\}\s*)RX"
    R"RX(this->\2\.remove\(\6\);\s*)RX"
    R"RX(\6->updateRenderer\(\);\s*)RX"
    R"RX(\6->needsUpdate\s*=\s*false;\s*)RX"
    R"RX(\})RX"));

const int64_t _BUDGET = 8;

py::str _update_renderers_repl(const py::re::Match& m) {
    py::str field = m.group(2), szv = m.group(1), boundv = m.group(3), loopv = m.group(4), wr = m.group(5), chunk = m.group(6);
    py::str n = py::percent(py::str("%d"), _BUDGET);
    py::str sz = szv, bound = boundv, loop = loopv;
    return py::str("// J2X render-budget: rebuild at most ") + n +
           py::str(" chunks/frame (nearest first), not the whole dirty\n"
                   "\t// set in one blocking frame -- a fresh world is 1000+ dirty chunks and would never present\n"
                   "\t// (stuck on \"Spawning\"). The rest keep needsUpdate and rebuild over the next frames. "
                   "[generator: render_fix.py]\n"
                   "\tint ") +
           bound + py::str(" = ") + n + py::str(";\n\tfor (int ") + loop + py::str(" = 0; ") + loop + py::str(" < ") + bound +
           py::str("; ++") + loop + py::str(") {\n\tint ") + sz + py::str(" = this->") + field + py::str(".size();\n\tif(") + sz +
           py::str(" == 0) {\n\treturn;\n\t}\n\t") + wr + py::str("* ") + chunk + py::str(" = (") + wr + py::str("*)this->") + field +
           py::str("[") + sz + py::str(" - 1];\n\tthis->") + field + py::str(".remove(") + chunk + py::str(");\n\tif(") + chunk +
           py::str(" != NULL) {\n\t") + chunk + py::str("->updateRenderer();\n\t") + chunk +
           py::str("->needsUpdate = false;\n\t}\n\t}");
}

const py::re::Pattern _UPDATE_SURVIVOR =
    py::re::compile(py::str(R"RX(distanceToEntitySquared\(\w+\)\s*>\s*2500\.0F\s*&&\s*\w+\s*>\s*4)RX"));

const py::re::Pattern _HURT =
    py::re::compile(py::str(R"RX(if\((\w+)\s*>=\s*0\.0F\)\s*\{(\s*)(\w+)\s*/=\s*\(float\)(\w+)->maxHurtTime;)RX"));

py::str _hurt_repl(const py::re::Match& m) {
    py::str v3 = m.group(1), ws = m.group(2), v3b = m.group(3), v2 = m.group(4);
    return py::str("if(") + v3 + py::str(" >= 0.0F && ") + v2 +
           py::str("->hurtTime > 0) {  // J2X hurt-NaN guard: on spawn hurtTime=0 & partialTick=0 -> ") + v3 +
           py::str("=0 entered -> 0/0 NaN camera -> invisible world [render_fix.py]") + ws + v3b + py::str(" /= (float)") + v2 +
           py::str("->maxHurtTime;");
}

const py::re::Pattern _HURT_SURVIVOR =
    py::re::compile(py::str(R"RX(if\(\w+\s*>=\s*0\.0F\)\s*\{\s*\w+\s*/=\s*\(float\)\w+->maxHurtTime;)RX"));

std::tuple<py::str, int64_t, int64_t> process(const py::str& text_in) {
    auto [text, n_update] = _UPDATE_RENDERERS.subn(_update_renderers_repl, text_in);
    auto [text2, n_hurt] = _HURT.subn(_hurt_repl, text);
    return {text2, n_update, n_hurt};
}

Result run(const py::str& out_dir) {
    py::list<py::str> files;
    for (const py::str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(py::str(".cpp"))) {
            files.append(f);
        }
    }
    Result res;
    for (const py::str& f : files) {
        py::str path = py::os::path::join(out_dir, f);
        py::str text;
        try {
            text = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
        } catch (const py::OSError&) {                     // (IOError, OSError)
            continue;
        }
        bool has_update = text.contains(py::str("worldRenderersToUpdate")) && text.contains(py::str("distanceToEntitySquared"));
        bool has_hurt = text.contains(py::str("maxHurtTime"));
        if (!(has_update || has_hurt)) {
            continue;
        }
        auto [new_text, n_u, n_h] = process(text);
        if (new_text != text) {
            py::file fh = py::open(path, py::str("w"), py::str("utf-8"));
            fh.write(new_text);
            fh.close();
            res.files.append(Touched{f, n_u, n_h});
            res.update += n_u;
            res.hurt += n_h;
        }
        if (_UPDATE_SURVIVOR.search(new_text) || _HURT_SURVIVOR.search(new_text)) {
            res.stranded.append(f);
        }
    }
    return res;
}

namespace {

[[noreturn]] void none_attr(const char* type, const char* attr) {
    throw py::AttributeError(std::string("'") + type + "' object has no attribute '" + attr + "'");
}

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> py::ref<T> as(const ir::NodeOrList& v) { return std::dynamic_pointer_cast<T>(ir::node_of(v)); }   // a list is no node

// A body slot (NodeOrList) as the node ir.walk() takes; a bare list of statements has no .children().
ir::NodeP walkable(const ir::NodeOrList& v) {
    if (std::holds_alternative<py::list<ir::NodeP>>(v)) throw py::AttributeError("'list' object has no attribute 'children'");
    return std::get<ir::NodeP>(v);
}

// `x == 0` for the value of a Lit: an int, a float or a bool equal to zero (None and a str are not)
bool equals_zero(const ir::Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v) == 0;
    if (std::holds_alternative<double>(v.v)) return std::get<double>(v.v) == 0.0;
    if (std::holds_alternative<bool>(v.v)) return !std::get<bool>(v.v);
    return false;
}

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    // class name (None possible) -> [update, hurt]
    std::map<std::optional<py::str>, std::array<int64_t, 2>> per;
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(walkable(m->body)).copy()) {
            if (!ss) {
                throw py::TypeError("object of type 'NoneType' has no len()");
            }
            int64_t i = 0;
            while (i < py::len(*ss)) {
                if (i + 2 < py::len(*ss) && _budget(*ss, i)) {
                    per[cls->name][0] += 1;
                }
                if (i < py::len(*ss) && _hurt((*ss)[i])) {
                    per[cls->name][1] += 1;
                }
                i += 1;
            }
        }
    }
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("'<' not supported between instances of 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    Result res;
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second[0], kv.second[1]});
        res.update += kv.second[0];
        res.hurt += kv.second[1];
    }
    return res;
}

bool _budget(const py::list<ir::NodeP>& ss, int64_t i) {
    ir::NodeP a_n = ss[i], b_n = ss[i + 1], loop_n = ss[i + 2];
    auto a = as<ir::Local>(a_n);
    auto b = as<ir::Local>(b_n);
    auto loop = as<ir::For>(loop_n);
    if (!(a && b && loop)) {
        return false;
    }
    ir::NodeP size_b = b->init ? rw::strip(b->init) : ir::NodeP();
    auto size_call = as<ir::Call>(size_b);
    if (!(size_call && size_call->sym && size_call->sym->name == py::str("size"))) {
        return false;
    }
    auto loop_body = as<ir::Block>(loop->body);
    ir::Nodes body = loop_body ? loop_body->stmts : ir::Nodes();
    if (!body || py::len(*body) == 0 || py::len(*body) != 5 || !as<ir::Local>((*body)[0]) || !as<ir::If>((*body)[1])) {
        return false;
    }
    auto w = as<ir::Local>((*body)[0]);
    auto test = as<ir::If>((*body)[1]);
    std::set<py::str> calls;
    for (const ir::NodeP& n : ir::walk(test->cond)) {
        auto c = as<ir::Call>(n);
        if (c && c->sym) {
            calls.insert(c->sym->name);
        }
    }
    if (!calls.count(py::str("distanceToEntitySquared"))) {
        return false;
    }
    py::list<ir::NodeP> then;
    if (auto tb = as<ir::Block>(test->then)) {
        if (!tb->stmts) {
            throw py::TypeError("'NoneType' object is not iterable");
        }
        then = *tb->stmts;
    } else {
        then.append(ir::node_of(test->then));   // a bare list is no Return either
    }
    bool has_return = false;
    for (const ir::NodeP& x : then) {
        if (as<ir::Return>(x)) {
            has_return = true;
            break;
        }
    }
    if (!has_return) {
        return false;
    }
    ir::NodeP get_n = w->init ? rw::strip(w->init) : ir::NodeP();
    while (auto cast = as<ir::Cast>(get_n)) {
        get_n = rw::strip(cast->x);
    }
    auto get = as<ir::Call>(get_n);
    if (!(get && get->sym && get->sym->name == py::str("get"))) {
        return false;
    }
    if (!get->args) {
        throw py::TypeError("object of type 'NoneType' has no len()");
    }
    if (py::len(*get->args) != 1) {
        return false;
    }
    auto idx = as<ir::Binary>(rw::strip((*get->args)[0]));
    if (!(idx && idx->op && *idx->op == py::str("-") && rw::is_name(idx->left, a->sym))) {
        return false;
    }
    ir::TypeP a_type = a->type;
    b->init = rw::lit(ir::Val(_BUDGET), a_type);          // rebuild at most _BUDGET chunks this frame
    a->init = size_b;                                     // A = this.F.size(), now inside the loop
    idx->right = rw::lit(ir::Val(int64_t(1)), a_type);    // the last (nearest, sorted) one
    auto ret = std::make_shared<ir::Return>();
    ir::NodeP empty = rw::if_(rw::binary(py::str("=="), rw::name(a->sym, a_type), rw::lit(ir::Val(int64_t(0)), a_type), ir::BOOL), ret);
    auto kept = std::make_shared<ir::Block>();
    kept->stmts = body->slice(3, 5);
    ir::NodeP present = rw::if_(rw::binary(py::str("!="), rw::name(w->sym, w->type), rw::null(), ir::BOOL), kept);
    loop_body->stmts = py::list<ir::NodeP>{a, empty, w, (*body)[2], present};
    ss.pop(i);                                            // A moved into the loop
    return true;
}

bool _hurt(const ir::NodeP& s_n) {
    auto s = as<ir::If>(s_n);
    if (!s) {
        return false;
    }
    auto then = as<ir::Block>(s->then);
    if (!then || !then->stmts || py::len(*then->stmts) == 0) {
        return false;
    }
    ir::NodeP c_n = rw::strip(s->cond);
    auto c = as<ir::Binary>(c_n);
    if (!(c && c->op && *c->op == py::str(">="))) {
        return false;
    }
    auto left = as<ir::Name>(rw::strip(c->left));
    auto right = as<ir::Lit>(rw::strip(c->right));
    if (!(left && right && equals_zero(right->value))) {
        return false;
    }
    auto first = as<ir::ExprStmt>((*then->stmts)[0]);
    if (!first) {
        return false;
    }
    auto d = as<ir::Assign>(rw::strip(first->expr));
    if (!(d && d->op && *d->op == py::str("/") && rw::is_name(d->target, left->sym))) {
        return false;
    }
    ir::NodeP v_n = rw::strip(d->value);
    while (auto cast = as<ir::Cast>(v_n)) {
        v_n = rw::strip(cast->x);
    }
    auto v = as<ir::Select>(v_n);
    if (!(v && v->sym && v->sym->name == py::str("maxHurtTime"))) {
        return false;
    }
    auto hurt = std::make_shared<ir::Select>();
    hurt->obj = v->obj;
    hurt->sym = std::make_shared<ir::Sym>(py::str("field"), py::str("hurtTime"), v->sym->owner, false, ir::I32);
    hurt->type = ir::I32;
    s->cond = rw::binary(py::str("&&"), s->cond, rw::binary(py::str(">"), hurt, rw::lit(ir::Val(int64_t(0)), ir::I32), ir::BOOL), ir::BOOL);
    return true;
}

py::list<py::str> stranded(const py::str& out_dir) {
    py::list<py::str> out;
    for (const py::str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(py::str(".cpp"))) {
            py::str text;
            try {
                text = py::open(py::os::path::join(out_dir, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            if (_UPDATE_SURVIVOR.search(text) || _HURT_SURVIVOR.search(text)) {
                out.append(f);
            }
        }
    }
    return out;
}

}  // namespace codebreaker::java::projects::minecraft::render_fix
