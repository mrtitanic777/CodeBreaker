// render_fix: the text form is checked against a golden written by the Python pass and reviewed by hand
// (testdata/gen_fixes_golden.py) and against the cases of tests/java/test_render_fix.py; the tree form is built here
// from the shapes the pass matches.
#include <string>

#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"
#include "cb/java/projects/minecraft/ir_test_support.hpp"
#include "cb/java/projects/minecraft/render_fix.hpp"

namespace {

using namespace cb;
namespace rf = cb::java::projects::minecraft::render_fix;
namespace ir = cb::translate::ir;
namespace ts = cb::java::projects::minecraft::ir_test_support;

std::string render_fixture(const char* name) {
    return fs::read_text(testing::data_path(std::string("java/projects/minecraft/testdata/fixes/") + name));
}

bool render_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

CB_TEST("java.minecraft.render_fix.rewrite_matches_golden", [] {
    const rf::Rewritten r = rf::rewrite(render_fixture("render_update.in.cpp"));
    CB_CHECK_EQ(r.update, std::size_t{1});
    CB_CHECK_EQ(r.hurt, std::size_t{1});
    CB_CHECK_EQ(r.text, render_fixture("render_update.golden.cpp"));
});

CB_TEST("java.minecraft.render_fix.update_renderers_is_budgeted_nearest_first", [] {
    const rf::Rewritten r = rf::rewrite(render_fixture("render_update.in.cpp"));
    // the desktop all-chunks early-out is gone; a fixed per-frame budget bounds the loop
    CB_CHECK(!render_has(r.text, "> 2500.0F && var4 > 4"));
    CB_CHECK(!render_has(r.text, "int var2 = this->worldRenderersToUpdate.size() - 1;"));
    CB_CHECK(render_has(r.text, "int var3 = 8;"));
    CB_CHECK_EQ(rf::kChunkBudget, 8);
    // the sort (nearest to the back) is preserved and the loop pops the back
    CB_CHECK(render_has(r.text, "Collections::sort(this->worldRenderersToUpdate"));
    CB_CHECK(render_has(r.text, "this->worldRenderersToUpdate[var2 - 1]"));
    // the rebuild and the dirty flag are kept, now NULL-guarded
    CB_CHECK(render_has(r.text, "if(var5 != NULL) {"));
    CB_CHECK(render_has(r.text, "var5->updateRenderer();"));
    CB_CHECK(render_has(r.text, "var5->needsUpdate = false;"));
});

CB_TEST("java.minecraft.render_fix.hurt_guard_requires_hurt_time", [] {
    const rf::Rewritten r = rf::rewrite(render_fixture("render_update.in.cpp"));
    CB_CHECK(render_has(r.text, "if(var3 >= 0.0F && var2->hurtTime > 0) {"));
    // the division is preserved (now unreachable on spawn)
    CB_CHECK(render_has(r.text, "var3 /= (float)var2->maxHurtTime;"));
});

CB_TEST("java.minecraft.render_fix.rewrite_is_idempotent", [] {
    const rf::Rewritten once = rf::rewrite(render_fixture("render_update.in.cpp"));
    const rf::Rewritten twice = rf::rewrite(once.text);
    CB_CHECK_EQ(twice.update + twice.hurt, std::size_t{0});
    CB_CHECK_EQ(twice.text, once.text);
});

CB_TEST("java.minecraft.render_fix.unrelated_code_is_untouched", [] {
    const std::string src =
        "void RenderGlobal::g() {\n\tint var2 = this->list.size() - 1;\n\tfor (int i = 0; i < 3; ++i) {}\n}\n";
    const rf::Rewritten r = rf::rewrite(src);
    CB_CHECK_EQ(r.update + r.hurt, std::size_t{0});
    CB_CHECK_EQ(r.text, src);
    CB_CHECK_EQ(rf::rewrite("").text, "");
});

CB_TEST("java.minecraft.render_fix.run_reports_and_finds_no_survivor", [] {
    fs::ScopedTempDir dir("cb-render");
    fs::write_text(dir.path() / "RenderGlobal.cpp", render_fixture("render_update.in.cpp"));
    fs::write_text(dir.path() / "Other.cpp", "void Other::f() {}\n");
    const rf::Report r = rf::run(dir.path());
    CB_CHECK_EQ(r.update, std::size_t{1});
    CB_CHECK_EQ(r.hurt, std::size_t{1});
    CB_CHECK(r.stranded.empty());
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "RenderGlobal.cpp");
    CB_CHECK_EQ(fs::read_text(dir.path() / "RenderGlobal.cpp"), render_fixture("render_update.golden.cpp"));
    CB_CHECK(rf::stranded(dir.path()).empty());
    CB_CHECK_EQ(rf::run(dir.path()).files.size(), std::size_t{0});   // a second run changes nothing
});

CB_TEST("java.minecraft.render_fix.stranded_finds_a_loop_that_drifted", [] {
    fs::ScopedTempDir dir("cb-render");
    // the early-out is there but the loop is not the generated shape (an extra statement before the test)
    fs::write_text(dir.path() / "Drifted.cpp",
                   "void f(EntityPlayer* var1) {\n\tfor (int var4 = 0; var4 < var3; ++var4) {\n\tlog();\n"
                   "\tif(var5->distanceToEntitySquared(var1) > 2500.0F && var4 > 4) {\n\treturn;\n\t}\n\t}\n}\n"
                   "// worldRenderersToUpdate\n");
    const rf::Report r = rf::run(dir.path());
    CB_CHECK_EQ(r.update, std::size_t{0});
    CB_CHECK_EQ(r.stranded.size(), std::size_t{1});
    CB_CHECK_EQ(rf::stranded(dir.path()).size(), std::size_t{1});
});

// ---- the tree form -------------------------------------------------------------------------------------------------

//     int A = this.F.size() - 1;  int B = this.F.size();
//     for (int L = 0; L < B; ++L) {
//         T W = (T)this.F.get(A - L);
//         if (W.distanceToEntitySquared(P) > 2500.0F && L > 4) return;
//         this.F.remove(W); W.updateRenderer(); W.needsUpdate = false;
//     }
struct BudgetTree {
    ts::Program program;
    ir::Node* a_local = nullptr;
    ir::Node* b_local = nullptr;
    ir::Node* loop = nullptr;
    ir::Node* index = nullptr;   // the `A - L` expression
    ir::Node* update_call = nullptr;
    ir::Node* flag_assign = nullptr;
};

BudgetTree make_budget(Arena& a, bool with_return = true) {
    BudgetTree t;
    ir::SymP sym_a = ts::sym(ir::SymKind::Local, "var2", ir::i32_type());
    ir::SymP sym_b = ts::sym(ir::SymKind::Local, "var3", ir::i32_type());
    ir::SymP sym_l = ts::sym(ir::SymKind::Local, "var4", ir::i32_type());
    ir::SymP sym_w = ts::sym(ir::SymKind::Local, "var5", ir::class_type("WorldRenderer"));
    auto size_call = [&] { return ts::call(a, ts::this_field(a, "worldRenderersToUpdate"), "size", {}, ir::i32_type()); };
    t.a_local = ts::local(a, sym_a, ts::binary(a, "-", size_call(), ts::int_lit(a, 1), ir::i32_type()), ir::i32_type());
    t.b_local = ts::local(a, sym_b, size_call(), ir::i32_type());
    t.index = ts::binary(a, "-", ts::name(a, sym_a), ts::name(a, sym_l), ir::i32_type());
    ir::Node* get = ts::call(a, ts::this_field(a, "worldRenderersToUpdate"), "get", {t.index});
    ir::Node* cast = ir::make(a, ir::Cast{.x = get}, ir::class_type("WorldRenderer"));
    ir::Node* w_local = ts::local(a, sym_w, cast);
    ir::Node* distance = ts::call(a, ts::name(a, sym_w), "distanceToEntitySquared", {ts::name(a, ts::sym(ir::SymKind::Param, "var1"))});
    ir::Node* cond = ts::binary(a, "&&", ts::binary(a, ">", distance, ts::float_lit(a, 2500.0)),
                                ts::binary(a, ">", ts::name(a, sym_l), ts::int_lit(a, 4)));
    ir::Node* ret = ir::make(a, ir::Return{});
    ir::Node* early_out = ts::if_(a, cond, ts::block(a, {with_return ? ret : ts::expr_stmt(a, ts::call(a, nullptr, "log"))}));
    ir::Node* remove = ts::expr_stmt(a, ts::call(a, ts::this_field(a, "worldRenderersToUpdate"), "remove", {ts::name(a, sym_w)}));
    t.update_call = ts::expr_stmt(a, ts::call(a, ts::name(a, sym_w), "updateRenderer"));
    t.flag_assign = ts::expr_stmt(a, ts::assign(a, ts::field(a, ts::name(a, sym_w), "needsUpdate"), ts::int_lit(a, 0)));
    ir::For loop;
    loop.cond = ts::binary(a, "<", ts::name(a, sym_l), ts::name(a, sym_b));
    loop.body = ts::block(a, {w_local, early_out, remove, t.update_call, t.flag_assign});
    t.loop = ir::make(a, std::move(loop));
    t.program = ts::program(a, "RenderGlobal", "updateRenderers", {ts::expr_stmt(a, ts::call(a, nullptr, "sort")), t.a_local, t.b_local, t.loop});
    return t;
}

CB_TEST("java.minecraft.render_fix.patch_budget_rebuilds_the_loop", [] {
    Arena a;
    BudgetTree t = make_budget(a);
    const rf::Report r = rf::patch(a, {t.program.unit});
    CB_CHECK_EQ(r.update, std::size_t{1});
    CB_CHECK_EQ(r.hurt, std::size_t{0});
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "RenderGlobal.cpp");
    // `int A = size()-1` moved into the loop: the method body keeps the sort, B and the loop
    const auto& body = t.program.body->stmts;
    CB_CHECK_EQ(body.size(), std::size_t{3});
    CB_CHECK(body[1] == t.b_local);
    CB_CHECK(body[2] == t.loop);
    // B is the budget now
    const ir::Lit* budget = ir::get<ir::Lit>(ir::get<ir::Local>(t.b_local)->init);
    CB_CHECK(budget != nullptr);
    CB_CHECK_EQ(std::get<std::int64_t>(budget->value), std::int64_t{rf::kChunkBudget});
    // the loop body: A = size(); if (A == 0) return; W = get(A - 1); remove; if (W != null) { update; flag }
    const ir::Block* loop_body = ir::get<ir::Block>(std::get<ir::Node*>(ir::get<ir::For>(t.loop)->body));
    CB_CHECK_EQ(loop_body->stmts.size(), std::size_t{5});
    CB_CHECK(loop_body->stmts[0] == t.a_local);
    const ir::Call* a_init = ir::get<ir::Call>(ir::get<ir::Local>(t.a_local)->init);
    CB_CHECK(a_init != nullptr && a_init->sym->name == "size");
    const ir::If* empty = ir::get<ir::If>(loop_body->stmts[1]);
    CB_CHECK(empty != nullptr);
    CB_CHECK(ir::is<ir::Return>(ir::get<ir::Block>(std::get<ir::Node*>(empty->then))->stmts[0]));
    CB_CHECK(ir::is<ir::Local>(loop_body->stmts[2]));
    CB_CHECK(ir::is<ir::ExprStmt>(loop_body->stmts[3]));   // the remove
    const ir::If* present = ir::get<ir::If>(loop_body->stmts[4]);
    CB_CHECK(present != nullptr);
    const ir::Block* kept = ir::get<ir::Block>(std::get<ir::Node*>(present->then));
    CB_CHECK_EQ(kept->stmts.size(), std::size_t{2});
    CB_CHECK(kept->stmts[0] == t.update_call && kept->stmts[1] == t.flag_assign);
    // the index is `A - 1` now
    const ir::Binary* idx = ir::get<ir::Binary>(t.index);
    CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(idx->right)->value), std::int64_t{1});
    // idempotent: the early-out is gone
    CB_CHECK_EQ(rf::patch(a, {t.program.unit}).update, std::size_t{0});
});

CB_TEST("java.minecraft.render_fix.patch_budget_declines_without_the_early_out", [] {
    Arena a;
    BudgetTree t = make_budget(a, /*with_return=*/false);
    CB_CHECK_EQ(rf::patch(a, {t.program.unit}).update, std::size_t{0});
    CB_CHECK_EQ(t.program.body->stmts.size(), std::size_t{4});   // nothing moved
});

// if (X >= 0.0F) { X /= (float)Y.maxHurtTime; ... }
struct HurtTree {
    ts::Program program;
    ir::Node* guard = nullptr;
    ir::Node* original_cond = nullptr;
};

HurtTree make_hurt(Arena& a, std::string_view field = "maxHurtTime", std::string_view op = "/") {
    HurtTree t;
    ir::SymP x = ts::sym(ir::SymKind::Local, "var3", ir::prim_type(ir::Prim::F32));
    ir::SymP player = ts::sym(ir::SymKind::Local, "var2", ir::class_type("EntityPlayerSP"));
    t.original_cond = ts::binary(a, ">=", ts::name(a, x), ts::float_lit(a, 0.0));
    ir::Node* max_hurt = ts::field(a, ts::name(a, player), std::string(field), ir::i32_type());
    ir::Node* cast = ir::make(a, ir::Cast{.x = max_hurt}, ir::prim_type(ir::Prim::F32));
    ir::Node* div = ir::make(a, ir::Assign{.op = std::string(op), .target = ts::name(a, x), .value = cast});
    t.guard = ts::if_(a, t.original_cond, ts::block(a, {ts::expr_stmt(a, div)}));
    t.program = ts::program(a, "EntityRenderer", "hurtCameraEffect", {t.guard});
    return t;
}

CB_TEST("java.minecraft.render_fix.patch_hurt_adds_the_hurt_time_test", [] {
    Arena a;
    HurtTree t = make_hurt(a);
    const rf::Report r = rf::patch(a, {t.program.unit});
    CB_CHECK_EQ(r.hurt, std::size_t{1});
    CB_CHECK_EQ(r.update, std::size_t{0});
    CB_CHECK_EQ(r.files[0].file, "EntityRenderer.cpp");
    // cond is now `X >= 0 && Y.hurtTime > 0`
    const ir::Binary* c = ir::get<ir::Binary>(ir::get<ir::If>(t.guard)->cond);
    CB_CHECK(c != nullptr && c->op == "&&");
    CB_CHECK(c->left == t.original_cond);
    const ir::Binary* positive = ir::get<ir::Binary>(c->right);
    CB_CHECK(positive != nullptr && positive->op == ">");
    const ir::Select* hurt_time = ir::get<ir::Select>(positive->left);
    CB_CHECK(hurt_time != nullptr && hurt_time->sym->name == "hurtTime");
    CB_CHECK(hurt_time->sym->kind == ir::SymKind::Field);
    CB_CHECK(cb::java::projects::minecraft::ir_edit::is_name(hurt_time->obj, *ts::sym(ir::SymKind::Local, "var2")));
    CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(positive->right)->value), std::int64_t{0});
    // the cond now contains hurtTime, so a re-run still matches the `>= 0` shape only if cond is a bare `>=`
    CB_CHECK_EQ(rf::patch(a, {t.program.unit}).hurt, std::size_t{0});
});

CB_TEST("java.minecraft.render_fix.patch_hurt_declines_other_shapes", [] {
    Arena a;
    HurtTree other_field = make_hurt(a, "otherField");
    CB_CHECK_EQ(rf::patch(a, {other_field.program.unit}).hurt, std::size_t{0});
    HurtTree plain_assign = make_hurt(a, "maxHurtTime", "*");
    CB_CHECK_EQ(rf::patch(a, {plain_assign.program.unit}).hurt, std::size_t{0});
    // `X >= 1` is not the zero test
    HurtTree not_zero = make_hurt(a);
    ir::Binary* cond = ir::get<ir::Binary>(not_zero.original_cond);
    cond->right = ts::float_lit(a, 1.0);
    CB_CHECK_EQ(rf::patch(a, {not_zero.program.unit}).hurt, std::size_t{0});
});

}  // namespace
