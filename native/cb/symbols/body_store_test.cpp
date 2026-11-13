// cb-unity: alone (test helpers named for this module)
// Tests of symbols/body_store: the name store as symbols/names asks it, over a knowledge base of the test's own.
#include "cb/symbols/body_store.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/sources.hpp"

namespace stdfs = std::filesystem;

namespace {

cb::store::ModuleRef bs_module(std::string_view sha, std::string_view name) { return {std::string(sha), std::string(name), "", "pe"}; }

}  // namespace

CB_TEST("symbols.body_store.bodies_are_recorded_per_module_and_named_once_for_all", [] {
    cb::fs::ScopedTempDir dir("cbbodies");
    cb::knowledge::Store kb(dir.path() / "k.sqlite", cb::knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path()));
    cb::symbols::KnowledgeBodies bodies(kb);
    // two modules link the same function; the first names it itself
    bodies.set_bodies(bs_module("aaaa", "one.dll"), {{"1000", {"d1", "memcpy", "export"}}, {"1040", {"d2", std::nullopt, std::nullopt}}});
    bodies.set_bodies(bs_module("bbbb", "two.dll"), {{"2000", {"d1", std::nullopt, std::nullopt}}});
    const auto one = bodies.bodies("aaaa");
    CB_CHECK_EQ(one.size(), 2U);
    CB_CHECK_EQ(one.at("1000").digest, "d1");
    CB_CHECK_EQ(*one.at("1000").name, "memcpy");
    CB_CHECK(!one.at("1040").name.has_value());
    CB_CHECK(bodies.bodies("cccc").empty());
    // the second module has the name the first gave the body
    const auto shared = bodies.names_from_bodies("bbbb");
    CB_CHECK_EQ(shared.size(), 1U);
    CB_CHECK_EQ(shared.at("2000").name, "memcpy");
    CB_CHECK_EQ(shared.at("2000").source, "the same body in one.dll");
    // a name given by hand holds for every module with the body, and for the module that named it
    bodies.name_body("d2", "Hand", "named by hand");
    CB_CHECK_EQ(bodies.names_from_bodies("aaaa").at("1040").name, "Hand");
    // three bodies recorded, two different, one in both modules; the two rows of that one are the copies
    const cb::store::BodyCounts counts = bodies.shared_bodies();
    CB_CHECK_EQ(counts.recorded, 3);
    CB_CHECK_EQ(counts.distinct, 2);
    CB_CHECK_EQ(counts.in_several, 1);
    CB_CHECK_EQ(counts.copies, 2);
    // recording a module again replaces what it had
    bodies.set_bodies(bs_module("bbbb", "two.dll"), {});
    CB_CHECK_EQ(bodies.shared_bodies().recorded, 2);
    CB_CHECK_EQ(bodies.shared_bodies().copies, 0);
});
