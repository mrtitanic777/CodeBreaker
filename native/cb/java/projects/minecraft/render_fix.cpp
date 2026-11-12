#include "cb/java/projects/minecraft/render_fix.hpp"

#include <format>
#include <map>
#include <set>
#include <utility>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/java/projects/minecraft/converted_tree.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"

namespace cb::java::projects::minecraft::render_fix {

namespace ir = translate::ir;
namespace edit = ir_edit;
namespace tree = converted_tree;

namespace {

// [1] updateRenderers: the all-chunks-in-one-frame rebuild loop. The generated names are captured, so the match is
// edition-agnostic:
//   1 size-holder var   2 field (worldRenderersToUpdate)   3 loop-bound var   4 loop var
//   5 WorldRenderer type   6 chunk var   7 player var
const Regex& update_loop() {
    static const Regex re(
        R"(int\s+(\w+)\s*=\s*this->(\w+)\.size\(\)\s*-\s*1;\s*)"         // int var2 = this->FIELD.size() - 1;
        R"(int\s+(\w+)\s*=\s*this->\2\.size\(\);\s*)"                     // int var3 = this->FIELD.size();
        R"(for\s*\(int\s+(\w+)\s*=\s*0;\s*\4\s*<\s*\3;\s*\+\+\4\)\s*\{\s*)"
        R"((\w+)\*\s*(\w+)\s*=\s*\(\5\*\)this->\2\[\1\s*-\s*\4\];\s*)"    // WorldRenderer* var5 = (..)FIELD[var2-var4];
        R"(if\(\6->distanceToEntitySquared\((\w+)\)\s*>\s*2500\.0F\s*&&\s*\4\s*>\s*4\)\s*\{\s*)"
        R"(return;\s*\}\s*)"
        R"(this->\2\.remove\(\6\);\s*)"
        R"(\6->updateRenderer\(\);\s*)"
        R"(\6->needsUpdate\s*=\s*false;\s*)"
        R"(\})");
    return re;
}

// Survivor: the original all-chunks loop's tell-tale (the desktop distance early-out) still present.
const Regex& update_survivor() {
    static const Regex re(R"(distanceToEntitySquared\(\w+\)\s*>\s*2500\.0F\s*&&\s*\w+\s*>\s*4)");
    return re;
}

// The replacement loop: at most kChunkBudget chunks, popped from the back of the (nearest-last) list.
std::string update_loop_replacement(const RegexMatch& m) {
    const std::string size_var(m.group(1)), field(m.group(2)), bound(m.group(3)), loop(m.group(4)), type(m.group(5)),
        chunk(m.group(6));
    return std::format(
        "// J2X render-budget: rebuild at most {0} chunks/frame (nearest first), not the whole dirty\n"
        "\t// set in one blocking frame -- a fresh world is 1000+ dirty chunks and would never present\n"
        "\t// (stuck on \"Spawning\"). The rest keep needsUpdate and rebuild over the next frames. "
        "[generator: render_fix]\n"
        "\tint {3} = {0};\n"
        "\tfor (int {4} = 0; {4} < {3}; ++{4}) {{\n"
        "\tint {2} = this->{1}.size();\n"
        "\tif({2} == 0) {{\n"
        "\treturn;\n"
        "\t}}\n"
        "\t{5}* {6} = ({5}*)this->{1}[{2} - 1];\n"
        "\tthis->{1}.remove({6});\n"
        "\tif({6} != NULL) {{\n"
        "\t{6}->updateRenderer();\n"
        "\t{6}->needsUpdate = false;\n"
        "\t}}\n"
        "\t}}",
        kChunkBudget, field, size_var, bound, loop, type, chunk);
}

// [2] hurtCameraEffect: the 0/0 NaN on spawn.  1 var3   2 whitespace   3 var3 again   4 var2 (player)
const Regex& hurt_guard() {
    static const Regex re(R"(if\((\w+)\s*>=\s*0\.0F\)\s*\{(\s*)(\w+)\s*/=\s*\(float\)(\w+)->maxHurtTime;)");
    return re;
}

std::string hurt_guard_replacement(const RegexMatch& m) {
    const std::string v3(m.group(1)), ws(m.group(2)), v3b(m.group(3)), v2(m.group(4));
    return std::format(
        "if({0} >= 0.0F && {1}->hurtTime > 0) {{  // J2X hurt-NaN guard: on spawn hurtTime=0 & partialTick=0 -> "
        "{0}=0 entered -> 0/0 NaN camera -> invisible world [render_fix]{2}{3} /= (float){1}->maxHurtTime;",
        v3, v2, ws, v3b);
}

// Survivor: an unguarded `if(x >= 0.0F) {  x /= (float)y->maxHurtTime;` still present.
const Regex& hurt_survivor() {
    static const Regex re(R"(if\(\w+\s*>=\s*0\.0F\)\s*\{\s*\w+\s*/=\s*\(float\)\w+->maxHurtTime;)");
    return re;
}

}  // namespace

Rewritten rewrite(std::string_view text) {
    Rewritten out;
    std::string updated = update_loop().replace_all(text, [&](const RegexMatch& m) {
        ++out.update;
        return update_loop_replacement(m);
    });
    out.text = hurt_guard().replace_all(updated, [&](const RegexMatch& m) {
        ++out.hurt;
        return hurt_guard_replacement(m);
    });
    return out;
}

Report run(const std::filesystem::path& out_dir) {
    Report report;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        const std::filesystem::path path = out_dir / fs::from_text(name);
        auto file = tree::read_cpp(path);
        if (!file) continue;
        const std::string& text = file->text;
        const bool has_update = text.find("worldRenderersToUpdate") != std::string::npos &&
                                text.find("distanceToEntitySquared") != std::string::npos;
        const bool has_hurt = text.find("maxHurtTime") != std::string::npos;
        if (!has_update && !has_hurt) continue;
        Rewritten r = rewrite(text);
        if (r.text != text) {
            tree::write_cpp(path, r.text, file->crlf);
            report.files.push_back({name, r.update, r.hurt});
            report.update += r.update;
            report.hurt += r.hurt;
        }
        if (update_survivor().test(r.text) || hurt_survivor().test(r.text)) report.stranded.push_back(name);
    }
    return report;
}

namespace {

bool render_is_zero(const ir::Lit& lit) {
    if (const auto* i = std::get_if<std::int64_t>(&lit.value)) return *i == 0;
    if (const auto* u = std::get_if<std::uint64_t>(&lit.value)) return *u == 0;
    if (const auto* d = std::get_if<double>(&lit.value)) return *d == 0.0;
    return false;
}

// The expression under any casts and parentheses.
ir::Node* render_uncast(ir::Node* e) {
    e = edit::strip(e);
    while (const ir::Cast* c = ir::get<ir::Cast>(e)) e = edit::strip(c->x);
    return e;
}

// [1] at list[i]:
//     int A = this.F.size() - 1;  int B = this.F.size();
//     for (int L = 0; L < B; ++L) {
//         T W = (T)this.F.get(A - L);
//         if (W.distanceToEntitySquared(P) > 2500.0F && L > 4) return;
//         this.F.remove(W); W.updateRenderer(); W.needsUpdate = false;
//     }
// ->  int B = kChunkBudget;
//     for (int L = 0; L < B; ++L) {
//         int A = this.F.size();  if (A == 0) return;
//         T W = (T)this.F.get(A - 1);  this.F.remove(W);
//         if (W != null) { W.updateRenderer(); W.needsUpdate = false; }
//     }
// On a match the statement A leaves the list (it moved into the loop) and true is returned.
bool render_patch_budget(Arena& arena, std::vector<ir::Node*>& list, std::size_t i) {
    ir::Local* a = ir::get<ir::Local>(list[i]);
    ir::Local* b = ir::get<ir::Local>(list[i + 1]);
    ir::For* loop = ir::get<ir::For>(list[i + 2]);
    if (a == nullptr || b == nullptr || loop == nullptr || a->sym == nullptr) return false;
    ir::Node* size_b = b->init != nullptr ? edit::strip(b->init) : nullptr;
    const ir::Call* size_call = ir::get<ir::Call>(size_b);
    if (size_call == nullptr || size_call->sym == nullptr || size_call->sym->name != "size") return false;
    const auto* loop_body_node = std::get_if<ir::Node*>(&loop->body);
    ir::Block* loop_body = loop_body_node != nullptr ? ir::get<ir::Block>(*loop_body_node) : nullptr;
    if (loop_body == nullptr || loop_body->stmts.size() != 5) return false;
    ir::Local* w = ir::get<ir::Local>(loop_body->stmts[0]);
    ir::If* test = ir::get<ir::If>(loop_body->stmts[1]);
    if (w == nullptr || test == nullptr || w->sym == nullptr) return false;
    std::set<std::string, std::less<>> calls;
    for (ir::Node* n : ir::walk(test->cond)) {
        const ir::Call* c = ir::get<ir::Call>(n);
        if (c != nullptr && c->sym != nullptr) calls.insert(c->sym->name);
    }
    if (std::holds_alternative<std::vector<ir::Node*>>(test->then)) return false;
    bool returns = false;
    for (ir::Node* s : edit::block_or_single(test->then)) returns = returns || ir::is<ir::Return>(s);
    if (!calls.contains("distanceToEntitySquared") || !returns) return false;
    const ir::Call* get = w->init != nullptr ? ir::get<ir::Call>(render_uncast(w->init)) : nullptr;
    if (get == nullptr || get->sym == nullptr || get->sym->name != "get" || get->args.size() != 1) return false;
    ir::Binary* idx = ir::get<ir::Binary>(edit::strip(get->args[0]));
    if (idx == nullptr || idx->op != "-" || !edit::is_name(idx->left, *a->sym)) return false;

    const ir::TypeP a_type = a->type;
    b->init = edit::lit_int(arena, kChunkBudget, a_type);   // rebuild at most kChunkBudget chunks this frame
    a->init = size_b;                                        // A = this.F.size(), now inside the loop
    idx->right = edit::lit_int(arena, 1, a_type);            // the last (nearest, sorted) one
    ir::Node* a_node = list[i];
    ir::Node* empty = edit::if_then(
        arena, edit::binary(arena, "==", edit::name_of(arena, a->sym, a_type), edit::lit_int(arena, 0, a_type),
                            ir::bool_type()),
        ir::make(arena, ir::Return{}));
    ir::Node* present = edit::if_then(
        arena,
        edit::binary(arena, "!=", edit::name_of(arena, w->sym, w->type), edit::null_lit(arena), ir::bool_type()),
        ir::make(arena, ir::Block{.stmts = {loop_body->stmts[3], loop_body->stmts[4]}}));
    loop_body->stmts = {a_node, empty, loop_body->stmts[0], loop_body->stmts[2], present};
    list.erase(list.begin() + static_cast<std::ptrdiff_t>(i));   // A moved into the loop
    return true;
}

// [2] `if (X >= 0.0F) { X /= (float)Y.maxHurtTime; ...` -> `if (X >= 0.0F && Y.hurtTime > 0)`.
bool render_patch_hurt(Arena& arena, ir::Node* stmt) {
    ir::If* s = ir::get<ir::If>(stmt);
    if (s == nullptr) return false;
    const auto* then_node = std::get_if<ir::Node*>(&s->then);
    const ir::Block* then = then_node != nullptr ? ir::get<ir::Block>(*then_node) : nullptr;
    if (then == nullptr || then->stmts.empty()) return false;
    const ir::Binary* c = ir::get<ir::Binary>(edit::strip(s->cond));
    if (c == nullptr || c->op != ">=") return false;
    const ir::Name* var = ir::get<ir::Name>(edit::strip(c->left));
    const ir::Lit* zero = ir::get<ir::Lit>(edit::strip(c->right));
    if (var == nullptr || var->sym == nullptr || zero == nullptr || !render_is_zero(*zero)) return false;
    const ir::ExprStmt* first = ir::get<ir::ExprStmt>(then->stmts[0]);
    if (first == nullptr) return false;
    const ir::Assign* d = ir::get<ir::Assign>(edit::strip(first->expr));
    if (d == nullptr || d->op != "/" || !edit::is_name(d->target, *var->sym)) return false;
    const ir::Select* max_hurt = ir::get<ir::Select>(render_uncast(d->value));
    if (max_hurt == nullptr || max_hurt->sym == nullptr || max_hurt->sym->name != "maxHurtTime") return false;

    ir::Node* hurt = ir::make(
        arena,
        ir::Select{.obj = max_hurt->obj,
                   .sym = ir::make_sym(ir::SymKind::Field, "hurtTime", max_hurt->sym->owner, false, ir::i32_type())},
        ir::i32_type());
    ir::Node* positive = edit::binary(arena, ">", hurt, edit::lit_int(arena, 0, ir::i32_type()), ir::bool_type());
    s->cond = edit::binary(arena, "&&", s->cond, positive, ir::bool_type());
    return true;
}

}  // namespace

Report patch(Arena& arena, const std::vector<ir::Node*>& units) {
    std::map<std::string, std::pair<std::size_t, std::size_t>> per_class;   // sorted: class -> (update, hurt)
    for (const edit::MethodInClass& m : edit::methods(units)) {
        const std::string cls = m.cls->name.value_or("(anonymous)");
        for (ir::Node* root : ir::statements(m.method->body)) {
            for (std::vector<ir::Node*>* list : edit::stmt_lists(root)) {
                std::size_t i = 0;
                while (i < list->size()) {
                    if (i + 2 < list->size() && render_patch_budget(arena, *list, i)) ++per_class[cls].first;
                    if (i < list->size() && render_patch_hurt(arena, (*list)[i])) ++per_class[cls].second;
                    ++i;
                }
            }
        }
    }
    Report report;
    for (const auto& [cls, counts] : per_class) {
        report.files.push_back({cls + ".cpp", counts.first, counts.second});
        report.update += counts.first;
        report.hurt += counts.second;
    }
    return report;
}

std::vector<std::string> stranded(const std::filesystem::path& out_dir) {
    std::vector<std::string> out;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        auto file = tree::read_cpp(out_dir / fs::from_text(name));
        if (file && (update_survivor().test(file->text) || hurt_survivor().test(file->text))) out.push_back(name);
    }
    return out;
}

}  // namespace cb::java::projects::minecraft::render_fix
