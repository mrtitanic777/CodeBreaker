// applet_ping: the text form is checked against goldens written by the Python pass and reviewed by hand
// (testdata/gen_fixes_golden.py); the tree form is built here from the shape the pass matches.
#include <string>

#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/applet_ping.hpp"
#include "cb/java/projects/minecraft/ir_test_support.hpp"

namespace {

using namespace cb;
namespace ap = cb::java::projects::minecraft::applet_ping;
namespace ir = cb::translate::ir;
namespace ts = cb::java::projects::minecraft::ir_test_support;

std::string ping_fixture(const char* name) {
    return fs::read_text(testing::data_path(std::string("java/projects/minecraft/testdata/fixes/") + name));
}

CB_TEST("java.minecraft.applet_ping.rewrite_matches_golden", [] {
    const ap::Rewritten r = ap::rewrite(ping_fixture("applet_setlevel.in.cpp"));
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK_EQ(r.text, ping_fixture("applet_setlevel.golden.cpp"));
    // the network code is gone, the level is still set (unconditionally), the real load path is untouched
    CB_CHECK(r.text.find("new URL(") == std::string::npos);
    CB_CHECK(r.text.find("var2->readLine") == std::string::npos);
    CB_CHECK(r.text.find("catch (") == std::string::npos);
    CB_CHECK(r.text.find("this->theWorld = var1;") != std::string::npos);
    CB_CHECK(r.text.find("var1->load();") != std::string::npos);
    // the applet's own getDocumentBase() use is not a ping and stays
    CB_CHECK(r.text.find("this->getDocumentBase()->getHost()") != std::string::npos);
});

CB_TEST("java.minecraft.applet_ping.rewrite_is_idempotent", [] {
    const ap::Rewritten once = ap::rewrite(ping_fixture("applet_setlevel.in.cpp"));
    const ap::Rewritten twice = ap::rewrite(once.text);
    CB_CHECK_EQ(twice.sites, std::size_t{0});
    CB_CHECK_EQ(twice.text, once.text);
});

CB_TEST("java.minecraft.applet_ping.rewrite_leaves_other_code", [] {
    const std::string src =
        "void MinecraftApplet::init() {\n\tthis->mc->minecraftUri = this->getDocumentBase()->getHost();\n}\n";
    const ap::Rewritten r = ap::rewrite(src);
    CB_CHECK_EQ(r.sites, std::size_t{0});
    CB_CHECK_EQ(r.text, src);
    CB_CHECK_EQ(ap::rewrite("").text, "");
});

CB_TEST("java.minecraft.applet_ping.run_reports_and_finds_no_survivor", [] {
    fs::ScopedTempDir dir("cb-ping");
    fs::write_text(dir.path() / "Minecraft.cpp", ping_fixture("applet_setlevel.in.cpp"));
    fs::write_text(dir.path() / "Other.cpp", "void Other::f() {}\n");
    fs::write_text(dir.path() / "notes.txt", "getDocumentBase openStream");   // not a source file
    const ap::Report r = ap::run(dir.path());
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK(r.stranded.empty());
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "Minecraft.cpp");
    CB_CHECK_EQ(r.files[0].sites, std::size_t{1});
    CB_CHECK_EQ(fs::read_text(dir.path() / "Minecraft.cpp"), ping_fixture("applet_setlevel.golden.cpp"));
    CB_CHECK_EQ(fs::read_text(dir.path() / "Other.cpp"), "void Other::f() {}\n");
    CB_CHECK(ap::stranded(dir.path()).empty());
    // a second run changes nothing
    const ap::Report again = ap::run(dir.path());
    CB_CHECK_EQ(again.sites, std::size_t{0});
    CB_CHECK(again.files.empty());
});

CB_TEST("java.minecraft.applet_ping.run_keeps_crlf_files_crlf", [] {
    fs::ScopedTempDir dir("cb-ping");
    std::string crlf;
    for (char c : ping_fixture("applet_setlevel.in.cpp")) {
        if (c == '\n') crlf += '\r';
        crlf += c;
    }
    fs::write_text(dir.path() / "Minecraft.cpp", crlf);
    CB_CHECK_EQ(ap::run(dir.path()).sites, std::size_t{1});
    const std::string out = fs::read_text(dir.path() / "Minecraft.cpp");
    CB_CHECK(out.find("\r\n") != std::string::npos);
    CB_CHECK(out.find("new URL(") == std::string::npos);
    std::size_t bare_lf = 0;
    for (std::size_t i = 0; i < out.size(); ++i) bare_lf += out[i] == '\n' && (i == 0 || out[i - 1] != '\r');
    CB_CHECK_EQ(bare_lf, std::size_t{0});
});

CB_TEST("java.minecraft.applet_ping.stranded_finds_a_ping_that_drifted", [] {
    fs::ScopedTempDir dir("cb-ping");
    // the try opens the stream but the guarded assignment is not `this->f = v;` -- the pass cannot rewrite it
    fs::write_text(dir.path() / "Drifted.cpp",
                   "void X::f() {\n\ttry {\n\tthis->applet->getDocumentBase()->openStream();\n\tif(ok()) {\n\tthis->level = make(1);\n"
                   "\t}\n\t} catch (Throwable*) {\n\t}\n}\n");
    const ap::Report r = ap::run(dir.path());
    CB_CHECK_EQ(r.sites, std::size_t{0});
    CB_CHECK_EQ(r.stranded.size(), std::size_t{1});
    CB_CHECK_EQ(r.stranded[0], "Drifted.cpp");
    CB_CHECK_EQ(ap::stranded(dir.path()).size(), std::size_t{1});
});

CB_TEST("java.minecraft.applet_ping.run_on_a_missing_folder_is_empty", [] {
    const ap::Report r = ap::run(testing::data_path("java/projects/minecraft/testdata/no_such_folder"));
    CB_CHECK_EQ(r.sites, std::size_t{0});
    CB_CHECK(r.files.empty() && r.stranded.empty());
});

// ---- the tree form -------------------------------------------------------------------------------------------------

// try { <open stream>; if (<host check>) { this.theWorld = var1; } } catch (...) { }
struct PingTree {
    ts::Program program;
    ir::Node* try_node = nullptr;
    ir::Node* catch_body = nullptr;
    ir::Node* guard_then = nullptr;
};

PingTree make_ping(Arena& a, bool empty_catch = true, bool with_finally = false) {
    PingTree t;
    ir::SymP var1 = ts::sym(ir::SymKind::Local, "var1");
    ir::Node* open = ts::expr_stmt(
        a, ts::call(a, ts::call(a, ts::this_field(a, "mcApplet"), "getDocumentBase"), "openStream"));
    ir::Node* assign = ts::expr_stmt(a, ts::assign(a, ts::this_field(a, "theWorld"), ts::name(a, var1)));
    t.guard_then = ts::block(a, {assign});
    ir::Node* guard = ts::if_(a, ts::call(a, nullptr, "hostOk"), t.guard_then);
    t.catch_body = ts::block(a, empty_catch ? std::vector<ir::Node*>{}
                                            : std::vector<ir::Node*>{ts::expr_stmt(a, ts::call(a, nullptr, "log"))});
    ir::Node* handler = ir::make(a, ir::Catch{.body = t.catch_body});
    ir::Try tr;
    tr.body = ts::block(a, {open, guard});
    tr.catches = {handler};
    if (with_finally) tr.finally_body = ts::block(a, {});
    t.try_node = ir::make(a, std::move(tr));
    t.program = ts::program(a, "Minecraft", "setLevel",
                            {ts::expr_stmt(a, ts::call(a, nullptr, "before")), t.try_node, ts::expr_stmt(a, ts::call(a, nullptr, "after"))});
    return t;
}

CB_TEST("java.minecraft.applet_ping.patch_replaces_the_try_with_the_assignment", [] {
    Arena a;
    PingTree t = make_ping(a);
    const ap::Report r = ap::patch({t.program.unit});
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "Minecraft.cpp");
    CB_CHECK(r.stranded.empty());
    const auto& stmts = t.program.body->stmts;
    CB_CHECK_EQ(stmts.size(), std::size_t{3});
    CB_CHECK(!ir::is<ir::Try>(stmts[1]));
    const ir::ExprStmt* es = ir::get<ir::ExprStmt>(stmts[1]);
    CB_CHECK(es != nullptr);
    CB_CHECK(ir::is<ir::Assign>(es->expr));
    // idempotent: no try is left to match
    CB_CHECK_EQ(ap::patch({t.program.unit}).sites, std::size_t{0});
});

CB_TEST("java.minecraft.applet_ping.patch_declines_what_is_not_the_ping", [] {
    Arena a;
    PingTree handler_does_work = make_ping(a, /*empty_catch=*/false);
    CB_CHECK_EQ(ap::patch({handler_does_work.program.unit}).sites, std::size_t{0});
    CB_CHECK(ir::is<ir::Try>(handler_does_work.program.body->stmts[1]));

    PingTree has_finally = make_ping(a, true, /*with_finally=*/true);
    CB_CHECK_EQ(ap::patch({has_finally.program.unit}).sites, std::size_t{0});

    // a try that never touches getDocumentBase/openStream
    ir::SymP var1 = ts::sym(ir::SymKind::Local, "var1");
    ir::Try other;
    other.body = ts::block(a, {ts::if_(a, ts::call(a, nullptr, "ok"),
                                       ts::block(a, {ts::expr_stmt(a, ts::assign(a, ts::this_field(a, "f"), ts::name(a, var1)))}))});
    other.catches = {ir::make(a, ir::Catch{.body = ts::block(a, {})})};
    ts::Program p = ts::program(a, "X", "m", {ir::make(a, std::move(other))});
    CB_CHECK_EQ(ap::patch({p.unit}).sites, std::size_t{0});
});

CB_TEST("java.minecraft.applet_ping.patch_needs_a_this_field_assignment_of_a_name", [] {
    Arena a;
    PingTree t = make_ping(a);
    // the guarded statement now assigns a literal, not a variable: not the ping
    ir::Block* then = ir::get<ir::Block>(t.guard_then);
    ir::Assign* asg = ir::get<ir::Assign>(ir::get<ir::ExprStmt>(then->stmts[0])->expr);
    asg->value = ts::int_lit(a, 1);
    CB_CHECK_EQ(ap::patch({t.program.unit}).sites, std::size_t{0});
});

CB_TEST("java.minecraft.applet_ping.patch_reports_each_class_by_name", [] {
    Arena a;
    PingTree one = make_ping(a);
    PingTree two = make_ping(a);
    ir::ClassDecl* second = ir::get<ir::ClassDecl>(ir::get<ir::Unit>(two.program.unit)->types[0]);
    second->name = "Alpha";
    const ap::Report r = ap::patch({one.program.unit, two.program.unit});
    CB_CHECK_EQ(r.sites, std::size_t{2});
    CB_CHECK_EQ(r.files.size(), std::size_t{2});
    CB_CHECK_EQ(r.files[0].file, "Alpha.cpp");   // sorted by class name
    CB_CHECK_EQ(r.files[1].file, "Minecraft.cpp");
});

}  // namespace
