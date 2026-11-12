// cb/translate/frontends/clang/lower_stmt.cpp -- statements: blocks, declarations, control flow, switch, try, range-for.
#include <algorithm>
#include <functional>

#include "cb/translate/frontends/clang/lower_impl.hpp"

namespace cb::translate::frontends::clang {

namespace {

// What stands for the statement of a `case x:` with nothing after it.
const Json& null_statement() {
    static const Json node = Json::parse(R"({"kind":"NullStmt"})");
    return node;
}

bool is_case_kind(std::string_view k) { return k == "CaseStmt" || k == "DefaultStmt"; }

// The child at `index` when it is a real node (clang writes {} for an absent part of a for).
const Json* present_child(const std::vector<Json>& kids, std::size_t index) {
    if (index >= kids.size() || !kids[index].is_object() || !tree::flag(kids[index], "kind")) return nullptr;
    return &kids[index];
}

void append_all(Stmts& to, const Stmts& from) { to.insert(to.end(), from.begin(), from.end()); }

}  // namespace

// One statement, or the list a declaration of several variables gives.
ir::Node* Lowering::as_stmt(Stmts s) {
    if (s.size() == 1) return s[0];
    ir::Block block;
    block.stmts = std::move(s);
    return make_stmt(std::move(block), Pos{});
}

std::vector<ir::Node*> Lowering::commas(ir::Node* e) {
    if (const auto* b = ir::get<ir::Binary>(e); b != nullptr && b->op == ",") {
        std::vector<ir::Node*> out = commas(b->left);
        append_all(out, commas(b->right));
        return out;
    }
    return {e};
}

Stmts Lowering::stmt(const Json& n) {
    const std::optional<std::string> kind_word = tree::maybe_text(n, "kind");
    const std::string k = kind_word.value_or("");
    const Pos p = pos(n);
    const std::vector<Json>& inner = tree::inner(n);
    const auto refused = [&](const std::string& what) -> Stmts {
        return {make_stmt(ir::ExprStmt{.expr = unsupported_node(what, &n)}, p)};
    };
    if (k == "CompoundStmt") {
        ir::Block block;
        for (const Json& c : inner) append_all(block.stmts, stmt(c));
        return {make_stmt(std::move(block), p)};
    }
    if (k == "DeclStmt") {
        Stmts out;
        for (const Json& d : inner) {
            const std::string dk = tree::kind(d);
            if (dk == "VarDecl") {
                out.push_back(local(d));
            } else if (dk == "RecordDecl" || dk == "CXXRecordDecl") {
                if (tree::flag(d, "completeDefinition")) record(d);
            } else if (dk == "EnumDecl") {
                enum_decl(d);
            } else if (dk == "TypedefDecl" || dk == "TypeAliasDecl" || dk == "StaticAssertDecl" || dk == "UsingDecl" || dk == "UsingDirectiveDecl" ||
                       dk == "FunctionDecl") {   // (a prototype in a body)
                // nothing to lower
            } else {
                out.push_back(make_stmt(ir::ExprStmt{.expr = unsupported_node("local:" + dk, &d)}, p));
            }
        }
        return out;
    }
    if (k == "NullStmt") return {make_stmt(ir::Empty{}, p)};
    if (k == "ReturnStmt") return {make_stmt(ir::Return{.expr = inner.empty() ? nullptr : expr(inner[0])}, p)};
    if (k == "IfStmt") {
        std::size_t i = 0;
        Stmts pre;
        if (tree::flag(n, "hasInit")) append_all(pre, stmt(tree::child(n, static_cast<long>(i++))));
        if (tree::flag(n, "hasVar")) append_all(pre, stmt(tree::child(n, static_cast<long>(i++))));
        ir::Node* cond = expr(tree::child(n, static_cast<long>(i)));
        ir::Node* then = as_stmt(stmt(tree::child(n, static_cast<long>(i + 1))));
        ir::Node* orelse = (tree::flag(n, "hasElse") && inner.size() > i + 2) ? as_stmt(stmt(inner[i + 2])) : nullptr;
        ir::If branch;
        branch.cond = cond;
        branch.then = then;
        if (orelse != nullptr) branch.orelse = orelse;
        ir::Node* s = make_stmt(std::move(branch), p);
        if (pre.empty()) return {s};
        pre.push_back(s);
        return {make_stmt(ir::Block{.stmts = std::move(pre)}, p)};
    }
    if (k == "WhileStmt") {
        if (tree::flag(n, "hasVar")) return refused("while-declaration");
        ir::Node* cond = expr(tree::child(n, 0));
        ir::Node* loop_body = as_stmt(stmt(tree::child(n, 1)));
        return {make_stmt(ir::While{.cond = cond, .body = loop_body}, p)};
    }
    if (k == "DoStmt") {
        ir::Node* loop_body = as_stmt(stmt(tree::child(n, 0)));
        ir::Node* cond = expr(tree::child(n, 1));
        return {make_stmt(ir::DoWhile{.body = loop_body, .cond = cond}, p)};
    }
    if (k == "ForStmt") {
        const Json* init = present_child(inner, 0);
        const Json* var = present_child(inner, 1);
        const Json* cond = present_child(inner, 2);
        const Json* inc = present_child(inner, 3);
        const Json* loop_body = present_child(inner, 4);
        if (var != nullptr) return refused("for-declaration");
        std::vector<ir::Node*> inits;
        if (init != nullptr) {
            if (tree::kind(*init) == "DeclStmt") {
                inits = stmt(*init);
            } else {
                for (ir::Node* e : commas(expr(*init))) inits.push_back(make_stmt(ir::ExprStmt{.expr = e}, p));
            }
        }
        std::vector<ir::Node*> update;
        if (inc != nullptr) update = commas(expr(*inc));
        ir::For loop;
        loop.init = std::move(inits);
        if (cond != nullptr) loop.cond = expr(*cond);
        loop.update = std::move(update);
        if (loop_body == nullptr) tree::bad("a for has no body");
        loop.body = as_stmt(stmt(*loop_body));
        return {make_stmt(std::move(loop), p)};
    }
    if (k == "SwitchStmt") {
        if (tree::flag(n, "hasInit") || tree::flag(n, "hasVar")) return refused("switch-declaration");
        return {switch_stmt(n, tree::child(n, 0), tree::child(n, 1), p)};
    }
    if (k == "BreakStmt") return {make_stmt(ir::Break{}, p)};
    if (k == "ContinueStmt") return {make_stmt(ir::Continue{}, p)};
    if (k == "GotoStmt") {
        const auto it = labels_.find(tree::text(n, "targetLabelDeclId"));
        return {make_stmt(ir::Goto{.label = it == labels_.end() ? std::optional<std::string>("L") : it->second}, p)};
    }
    if (k == "LabelStmt") {
        ir::Node* labeled_body = !inner.empty() ? as_stmt(stmt(inner[0])) : make_stmt(ir::Empty{}, p);
        return {make_stmt(ir::Labeled{.label = tree::maybe_text(n, "name"), .body = labeled_body}, p)};
    }
    if (is_case_kind(k)) return refused("case-outside-switch-body");
    if (k == "AttributedStmt") return stmt(tree::child(n, -1));
    if (k == "CXXTryStmt") {
        ir::Node* try_body = as_stmt(stmt(tree::child(n, 0)));
        std::vector<ir::Node*> catches;
        for (std::size_t i = 1; i < inner.size(); ++i) {
            const Json& c = inner[i];
            const std::vector<Json>& ci = tree::inner(c);
            const Json* var = (!ci.empty() && tree::kind(ci[0]) == "VarDecl") ? &ci[0] : nullptr;
            const Json& blk = tree::child(c, -1);
            ir::Catch handler;
            if (var != nullptr) {
                const ir::TypeP t = ty(*var);
                ir::SymP s = sym(ir::SymKind::Local, local_name(*var, *scope_), scope_->owner, false, t);
                scope_->syms[tree::require(*var, "id").get<std::string>()] = s;
                handler.sym = s;
                handler.types = {t};
            }
            handler.body = as_stmt(stmt(blk));
            catches.push_back(make_stmt(std::move(handler), pos(c)));
        }
        return {make_stmt(ir::Try{.body = try_body, .catches = std::move(catches)}, p)};
    }
    if (k == "CXXForRangeStmt") return {range_for(n, p)};
    if (k == "CXXThrowExpr") return {make_stmt(ir::Throw{.expr = inner.empty() ? nullptr : expr(inner[0])}, p)};
    if (k == "ExprWithCleanups" && !inner.empty() && tree::kind(inner[0]) == "CXXThrowExpr") return stmt(inner[0]);
    if (!k.empty() && (k.ends_with("Expr") || k.ends_with("Operator") || k == "ExprWithCleanups")) {
        return {make_stmt(ir::ExprStmt{.expr = expr(n)}, p)};
    }
    return refused("stmt:" + kind_word.value_or("None"));
}

ir::Node* Lowering::switch_stmt(const Json& n, const Json& cond_n, const Json& body_n, Pos p) {
    std::vector<ir::Node*> cases;
    std::vector<const Json*> items;
    if (tree::kind(body_n) == "CompoundStmt") {
        for (const Json& c : tree::inner(body_n)) items.push_back(&c);
    } else {
        items.push_back(&body_n);
    }
    Stmts pre;
    for (const Json* it : items) {
        const Json* cur = it;
        std::vector<ir::Node*> labels;
        bool is_default = false;
        while (is_case_kind(tree::kind(*cur))) {
            if (tree::kind(*cur) == "CaseStmt") {
                const Json& label_node = tree::child(*cur, 0);
                const std::optional<std::int64_t> v = constant(&label_node);
                ir::Node* label = expr(label_node);
                if (v) label = make(ir::Lit{.value = *v}, ty(label_node), pos(*cur));
                labels.push_back(label);
                cur = tree::inner(*cur).size() > 1 ? &tree::child(*cur, -1) : &null_statement();
            } else {
                is_default = true;
                cur = !tree::inner(*cur).empty() ? &tree::child(*cur, -1) : &null_statement();
            }
        }
        if (!labels.empty() || is_default || is_case_kind(tree::kind(*it))) {
            const Stmts lowered = stmt(*cur);
            ir::Case one;
            one.labels = std::move(labels);
            one.is_default = is_default;
            if (!(lowered.size() == 1 && ir::is<ir::Empty>(lowered[0]))) one.body = lowered;
            cases.push_back(make_stmt(std::move(one), pos(*it)));
        } else if (!cases.empty()) {
            append_all(ir::get<ir::Case>(cases.back())->body, stmt(*it));
        } else {
            append_all(pre, stmt(*it));
        }
    }
    if (case_inside(body_n)) return make_stmt(ir::ExprStmt{.expr = unsupported_node("case-label-nested-in-a-statement", &n)}, p);
    ir::Switch sw;
    sw.expr = expr(cond_n);
    sw.cases = std::move(cases);
    ir::Node* switch_node = make_stmt(std::move(sw), p);
    for (ir::Node* x : pre) {
        if (!ir::is<ir::Local>(x) && !ir::is<ir::Empty>(x)) {
            return make_stmt(ir::ExprStmt{.expr = unsupported_node("code-before-first-case", &n)}, p);
        }
    }
    if (pre.empty()) return switch_node;
    pre.push_back(switch_node);
    return make_stmt(ir::Block{.stmts = std::move(pre)}, p);
}

// A case label below a statement of the switch body (Duff's device): not a structured switch.
bool Lowering::case_inside(const Json& body_n) {
    const std::function<bool(const Json&, int)> has = [&has](const Json& x, int depth) -> bool {
        if (!x.is_object()) return false;
        const std::string k = tree::kind(x);
        if (is_case_kind(k) && depth > 0) return true;
        if (k == "SwitchStmt") return false;
        const int next = depth + (is_case_kind(k) ? 0 : 1);
        return std::ranges::any_of(tree::inner(x), [&](const Json& c) { return has(c, next); });
    };
    if (tree::kind(body_n) == "CompoundStmt") {
        return std::ranges::any_of(tree::inner(body_n), [&](const Json& c) { return has(c, 0); });
    }
    return has(body_n, 0);
}

// for (T x : array) -> ForEach over the array; anything else declines.
ir::Node* Lowering::range_for(const Json& n, Pos p) {
    const std::vector<Json>& inner = tree::inner(n);
    const Json* var = nullptr;
    const Json* range = nullptr;
    for (const Json& c : inner) {
        if (tree::kind(c) != "DeclStmt") continue;
        for (const Json& d : tree::inner(c)) {
            if (tree::kind(d) != "VarDecl") continue;
            const std::string name_text = tree::text(d, "name");
            if (name_text == "__range1" && !tree::inner(d).empty()) {
                range = &tree::inner(d)[0];
            } else if (!name_text.starts_with("__")) {
                var = &d;
            }
        }
    }
    const Json& loop_body = tree::child(n, -1);
    if (var == nullptr || range == nullptr) return make_stmt(ir::ExprStmt{.expr = unsupported_node("range-for", &n)}, p);
    ir::Node* iterable = expr(*range);
    if (!iterable->type || !std::holds_alternative<ir::ArrayType>(iterable->type->body)) {
        return make_stmt(ir::ExprStmt{.expr = unsupported_node("range-for:" + spell(iterable->type), &n)}, p);
    }
    const ir::TypeP t = ty(*var);
    ir::SymP s = sym(ir::SymKind::Local, local_name(*var, *scope_), scope_->owner, false, t);
    scope_->syms[tree::require(*var, "id").get<std::string>()] = s;
    ir::Node* loc_node = make_stmt(ir::Local{.sym = s, .type = t}, pos(*var));
    return make_stmt(ir::ForEach{.var = loc_node, .iterable = iterable, .body = as_stmt(stmt(loop_body))}, p);
}

ir::Node* Lowering::local(const Json& d) {
    ir::TypeP t = ty(d);
    const std::string name_text = local_name(d, *scope_);
    ir::SymP s = sym(ir::SymKind::Local, name_text, scope_->owner, false, t);
    scope_->syms[tree::require(d, "id").get<std::string>()] = s;
    ir::Node* init = nullptr;
    for (const Json& c : tree::inner(d)) {
        if (tree::ends_with_attr(tree::kind(c))) continue;
        init = expr(c);
        const auto* ptr = t ? std::get_if<ir::PtrType>(&t->body) : nullptr;
        if (ptr != nullptr && ptr->ref) init = make(ir::Unary{.op = "&", .x = init}, t, pos(c));
    }
    const ir::TypeP completed = complete_array(t, init);
    if (completed.get() != t.get()) {
        t = completed;
        s->type = t;
    }
    std::vector<std::string> mods;
    if (tree::text(d, "storageClass") == "static") mods = {"static"};
    if (tree::text(d, "storageClass") == "extern") mods = {"extern"};
    return make_stmt(ir::Local{.sym = s, .type = t, .init = init, .mods = std::move(mods)}, pos(d));
}

}  // namespace cb::translate::frontends::clang
