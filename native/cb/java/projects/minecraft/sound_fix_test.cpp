// sound_fix: the text form is checked against goldens written by the Python pass and reviewed by hand
// (testdata/gen_fixes_golden.py); the tree form is built here from the shapes the pass matches.
#include <string>

#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"
#include "cb/java/projects/minecraft/ir_test_support.hpp"
#include "cb/java/projects/minecraft/sound_fix.hpp"

namespace {

using namespace cb;
namespace sf = cb::java::projects::minecraft::sound_fix;
namespace ir = cb::translate::ir;
namespace ts = cb::java::projects::minecraft::ir_test_support;

std::string sound_fixture(const char* name) {
    return fs::read_text(testing::data_path(std::string("java/projects/minecraft/testdata/fixes/") + name));
}

bool sound_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

CB_TEST("java.minecraft.sound_fix.sfx_matches_golden", [] {
    const sf::Rewritten r = sf::rewrite(sound_fixture("sound_sfx.in.cpp"));
    CB_CHECK_EQ(r.sfx, std::size_t{2});
    CB_CHECK_EQ(r.music, std::size_t{0});
    CB_CHECK_EQ(r.text, sound_fixture("sound_sfx.golden.cpp"));
    // the pool query is dropped (it hangs on the empty console pool), the volume gate is kept, the sound name is the cue
    CB_CHECK(sound_has(r.text, "SoundPoolEntry* var7 = NULL;"));
    CB_CHECK(sound_has(r.text, "if(var5 > 0.0F) {"));
    CB_CHECK(sound_has(r.text, "newSource(false, (void*)NULL, name,"));
    CB_CHECK(!sound_has(r.text, "var7->"));
    CB_CHECK(!sound_has(r.text, "var4->"));
    // the streaming pool is left alone
    CB_CHECK(sound_has(r.text, "soundPoolStreaming->getRandomSoundFromSoundPool(var1);\n\tif(var5 != NULL) {"));
    CB_CHECK(sound_has(r.text, "newStreamingSource(var5->soundName)"));
});

CB_TEST("java.minecraft.sound_fix.music_matches_golden", [] {
    const sf::Rewritten r = sf::rewrite(sound_fixture("sound_music.in.cpp"));
    CB_CHECK_EQ(r.sfx, std::size_t{0});
    CB_CHECK_EQ(r.music, std::size_t{1});
    CB_CHECK_EQ(r.text, sound_fixture("sound_music.golden.cpp"));
    // play() runs unconditionally; only the line that dereferences the entry stays guarded
    CB_CHECK(sound_has(r.text, "\tif(true) {\n"));
    CB_CHECK(sound_has(r.text, "\tif(var1 != NULL) s_sndSystem->backgroundMusic("));
    CB_CHECK(sound_has(r.text, "\ts_sndSystem->play(\"BgMusic\");"));
    CB_CHECK(!sound_has(r.text, "\ts_sndSystem->setVolume(\"BgMusic\", this->options->musicVolume);\n\tif(var1"));
});

CB_TEST("java.minecraft.sound_fix.rewrite_is_idempotent", [] {
    const sf::Rewritten once = sf::rewrite(sound_fixture("sound_sfx.in.cpp") + sound_fixture("sound_music.in.cpp"));
    CB_CHECK_EQ(once.sfx, std::size_t{2});
    CB_CHECK_EQ(once.music, std::size_t{1});
    const sf::Rewritten twice = sf::rewrite(once.text);
    CB_CHECK_EQ(twice.sfx + twice.music, std::size_t{0});
    CB_CHECK_EQ(twice.text, once.text);
});

CB_TEST("java.minecraft.sound_fix.rewrite_leaves_other_code", [] {
    const std::string src = "void f() {\n\tSoundPoolEntry* e = this->other->getRandomSound();\n\tif(e != NULL) { g(); }\n}\n";
    const sf::Rewritten r = sf::rewrite(src);
    CB_CHECK_EQ(r.sfx + r.music, std::size_t{0});
    CB_CHECK_EQ(r.text, src);
    CB_CHECK_EQ(sf::rewrite("").text, "");
});

CB_TEST("java.minecraft.sound_fix.sfx_without_a_volume_gate_becomes_if_true", [] {
    const std::string src =
        "SoundPoolEntry* e = this->soundPoolSounds->getRandomSoundFromSoundPool(n);\nif( e != NULL ) {\n"
        "play(e->soundUrl, e -> soundName, e->soundNameX);\n}\n";
    const sf::Rewritten r = sf::rewrite(src);
    CB_CHECK_EQ(r.sfx, std::size_t{1});
    CB_CHECK(sound_has(r.text, "SoundPoolEntry* e = NULL;"));
    CB_CHECK(sound_has(r.text, "if(true)"));
    // `e -> soundName` (spaced arrow) is routed; `e->soundNameX` is another member and stays
    CB_CHECK(sound_has(r.text, "play((void*)NULL, n, e->soundNameX);"));
});

CB_TEST("java.minecraft.sound_fix.sfx_routing_is_per_site_variable", [] {
    // `var1` is rewritten, but `xvar1->soundUrl` and `var10->soundUrl` are other names
    const std::string src =
        "SoundPoolEntry* var1 = this->soundPoolSounds->getRandomSoundFromSoundPool(a);\nif(var1 != NULL) {\n"
        "use(var1->soundUrl, xvar1->soundUrl, var10->soundUrl);\n}\n";
    const sf::Rewritten r = sf::rewrite(src);
    CB_CHECK(sound_has(r.text, "use((void*)NULL, xvar1->soundUrl, var10->soundUrl);"));
});

CB_TEST("java.minecraft.sound_fix.run_reports_and_finds_no_survivor", [] {
    fs::ScopedTempDir dir("cb-sound");
    fs::write_text(dir.path() / "SoundManager.cpp", sound_fixture("sound_sfx.in.cpp") + sound_fixture("sound_music.in.cpp"));
    fs::write_text(dir.path() / "Other.cpp", "void Other::f() { getRandomSound(); }\n");
    const sf::Report r = sf::run(dir.path());
    CB_CHECK_EQ(r.sites, std::size_t{3});
    CB_CHECK(r.stranded.empty());
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "SoundManager.cpp");
    CB_CHECK_EQ(r.files[0].sfx, std::size_t{2});
    CB_CHECK_EQ(r.files[0].music, std::size_t{1});
    CB_CHECK(sf::stranded(dir.path()).empty());
    CB_CHECK_EQ(sf::run(dir.path()).sites, std::size_t{0});
});

CB_TEST("java.minecraft.sound_fix.stranded_finds_a_guard_that_drifted", [] {
    fs::ScopedTempDir dir("cb-sound");
    // the guard is `!= NULL` but the declaration shape is not the one the pass knows (a cast on the query)
    fs::write_text(dir.path() / "Drifted.cpp",
                   "void f() {\n\tauto e = (SoundPoolEntry*)this->soundPoolSounds->getRandomSoundFromSoundPool(n);\n"
                   "\tif(e != NULL) {\n\tg(e);\n\t}\n}\n");
    const sf::Report r = sf::run(dir.path());
    CB_CHECK_EQ(r.sites, std::size_t{0});
    CB_CHECK_EQ(r.stranded.size(), std::size_t{1});
    CB_CHECK_EQ(sf::stranded(dir.path()).size(), std::size_t{1});
});

// ---- the tree form -------------------------------------------------------------------------------------------------

// SoundPoolEntry* var7 = this.soundPoolSounds.getRandomSoundFromSoundPool(name);
// if (var7 != null [&& var5 > 0.0F]) { newSource(var7.soundUrl, var7.soundName); }
struct SoundTree {
    ts::Program program;
    ir::Node* local = nullptr;
    ir::Node* guard = nullptr;
    ir::Node* url_select = nullptr;
    ir::Node* name_select = nullptr;
    ir::Node* sound_arg = nullptr;
    ir::Node* volume_gate = nullptr;
};

SoundTree make_sfx(Arena& a, bool with_gate) {
    SoundTree t;
    ir::SymP name = ts::sym(ir::SymKind::Param, "name");
    ir::SymP v = ts::sym(ir::SymKind::Local, "var7");
    t.sound_arg = ts::name(a, name);
    ir::Node* query = ts::call(a, ts::this_field(a, "soundPoolSounds"), "getRandomSoundFromSoundPool", {t.sound_arg});
    t.local = ts::local(a, v, query);
    ir::Node* test = ts::binary(a, "!=", ts::name(a, v), ts::null_lit(a));
    t.volume_gate = ts::binary(a, ">", ts::name(a, ts::sym(ir::SymKind::Param, "var5")), ts::float_lit(a, 0.0));
    ir::Node* cond = with_gate ? ts::binary(a, "&&", test, t.volume_gate) : test;
    t.url_select = ts::field(a, ts::name(a, v), "soundUrl");
    t.name_select = ts::field(a, ts::name(a, v), "soundName");
    ir::Node* use = ts::expr_stmt(a, ts::call(a, nullptr, "newSource", {t.url_select, t.name_select}));
    t.guard = ts::if_(a, cond, ts::block(a, {use}));
    t.program = ts::program(a, "SoundManager", "playSound", {t.local, t.guard});
    return t;
}

CB_TEST("java.minecraft.sound_fix.patch_sfx_drops_the_query_and_routes_the_name", [] {
    Arena a;
    SoundTree t = make_sfx(a, /*with_gate=*/true);
    const sf::Report r = sf::patch(a, {t.program.unit});
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK_EQ(r.files.size(), std::size_t{1});
    CB_CHECK_EQ(r.files[0].file, "SoundManager.cpp");
    CB_CHECK_EQ(r.files[0].sfx, std::size_t{1});
    CB_CHECK_EQ(r.files[0].music, std::size_t{0});
    // the local is now NULL, the guard is the volume gate alone
    const ir::Local* local = ir::get<ir::Local>(t.local);
    const ir::Lit* init = ir::get<ir::Lit>(local->init);
    CB_CHECK(init != nullptr && std::holds_alternative<std::monostate>(init->value));
    CB_CHECK(ir::get<ir::If>(t.guard)->cond == t.volume_gate);
    // the arguments: url -> NULL, name -> the sound-name argument itself
    const ir::Block* then = ir::get<ir::Block>(std::get<ir::Node*>(ir::get<ir::If>(t.guard)->then));
    const ir::Call* use = ir::get<ir::Call>(ir::get<ir::ExprStmt>(then->stmts[0])->expr);
    CB_CHECK(ir::is<ir::Lit>(use->args[0]));
    CB_CHECK(use->args[1] == t.sound_arg);
});

CB_TEST("java.minecraft.sound_fix.patch_sfx_without_a_gate_is_if_true", [] {
    Arena a;
    SoundTree t = make_sfx(a, /*with_gate=*/false);
    CB_CHECK_EQ(sf::patch(a, {t.program.unit}).sites, std::size_t{1});
    const ir::Lit* cond = ir::get<ir::Lit>(ir::get<ir::If>(t.guard)->cond);
    CB_CHECK(cond != nullptr);
    CB_CHECK(std::get<bool>(cond->value));
    // idempotent: the query is gone, so the pair no longer matches
    CB_CHECK_EQ(sf::patch(a, {t.program.unit}).sites, std::size_t{0});
});

CB_TEST("java.minecraft.sound_fix.patch_leaves_other_pools_and_shapes", [] {
    Arena a;
    // the streaming pool
    ir::SymP v = ts::sym(ir::SymKind::Local, "var5");
    ir::Node* local = ts::local(a, v, ts::call(a, ts::this_field(a, "soundPoolStreaming"), "getRandomSoundFromSoundPool",
                                              {ts::name(a, ts::sym(ir::SymKind::Param, "n"))}));
    ir::Node* guard = ts::if_(a, ts::binary(a, "!=", ts::name(a, v), ts::null_lit(a)), ts::block(a, {}));
    ts::Program p = ts::program(a, "SoundManager", "playStreaming", {local, guard});
    CB_CHECK_EQ(sf::patch(a, {p.unit}).sites, std::size_t{0});
    CB_CHECK(ir::is<ir::Call>(ir::get<ir::Local>(local)->init));   // the query is still there

    // a null test on another variable
    SoundTree t = make_sfx(a, false);
    ir::get<ir::If>(t.guard)->cond =
        ts::binary(a, "!=", ts::name(a, ts::sym(ir::SymKind::Local, "other")), ts::null_lit(a));
    CB_CHECK_EQ(sf::patch(a, {t.program.unit}).sites, std::size_t{0});

    // a query with no argument has no sound name to route
    SoundTree no_arg = make_sfx(a, false);
    ir::get<ir::Call>(ir::get<ir::Local>(no_arg.local)->init)->args.clear();
    CB_CHECK_EQ(sf::patch(a, {no_arg.program.unit}).sites, std::size_t{0});
});

// SoundPoolEntry* var1 = this.soundPoolMusic.getRandomSound(); if (var1 != null) { a(); b(var1.soundName); c(); }
CB_TEST("java.minecraft.sound_fix.patch_music_runs_play_always_and_guards_the_deref", [] {
    Arena a;
    ir::SymP v = ts::sym(ir::SymKind::Local, "var1");
    ir::Node* local = ts::local(a, v, ts::call(a, ts::this_field(a, "soundPoolMusic"), "getRandomSound"));
    ir::Node* s1 = ts::expr_stmt(a, ts::call(a, nullptr, "delay"));
    ir::Node* s2 = ts::expr_stmt(a, ts::call(a, nullptr, "backgroundMusic", {ts::field(a, ts::name(a, v), "soundName")}));
    ir::Node* s3 = ts::expr_stmt(a, ts::call(a, nullptr, "play"));
    ir::Node* guard =
        ts::if_(a, ts::binary(a, "!=", ts::name(a, v), ts::null_lit(a)), ts::block(a, {s1, s2, s3}));
    ts::Program p = ts::program(a, "SoundManager", "playRandomMusic", {local, guard});
    const sf::Report r = sf::patch(a, {p.unit});
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK_EQ(r.files[0].sfx, std::size_t{0});
    CB_CHECK_EQ(r.files[0].music, std::size_t{1});
    const ir::If* g = ir::get<ir::If>(guard);
    CB_CHECK(std::get<bool>(ir::get<ir::Lit>(g->cond)->value));
    const ir::Block* body = ir::get<ir::Block>(std::get<ir::Node*>(g->then));
    CB_CHECK_EQ(body->stmts.size(), std::size_t{3});
    CB_CHECK(body->stmts[0] == s1);   // statements that do not mention the entry stay as they are
    CB_CHECK(body->stmts[2] == s3);
    const ir::If* inner = ir::get<ir::If>(body->stmts[1]);   // the one that does is guarded on its own
    CB_CHECK(inner != nullptr);
    CB_CHECK(cb::java::projects::minecraft::ir_edit::is_null_test(inner->cond, *v));
});

}  // namespace
