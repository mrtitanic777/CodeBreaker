// Tests of cb/store/knowledge_bodies: the body store over the knowledge base -- what is recorded for a module reads back, a name given to a body
// holds in every module that has the body, and the counts of what the modules share.
#include "cb/store/knowledge_bodies.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace stdfs = std::filesystem;

struct BodiesWorld {
    cb::fs::ScopedTempDir dir{"cb-bodies"};
    cb::knowledge::Store kb{dir.path() / "db" / "knowledge.v2.sqlite", cb::knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path())};
    cb::store::KnowledgeBodies bodies{kb};

    static cb::store::Body body(std::string digest, std::optional<std::string> name = std::nullopt, std::optional<std::string> source = std::nullopt) {
        return cb::store::Body{std::move(digest), std::move(name), std::move(source)};
    }
};

const cb::store::ModuleRef kLibA{"sha-a", "liba.dll", "C:/corpus/liba.dll", "pe"};
const cb::store::ModuleRef kLibB{"sha-b", "libb.dll", "C:/corpus/libb.dll", "pe"};

}  // namespace

CB_TEST("store.knowledge_bodies.what_is_recorded_reads_back_and_is_replaced", [] {
    BodiesWorld w;
    CB_CHECK(w.bodies.bodies("sha-a").empty());
    w.bodies.set_bodies(kLibA, {{"1000", BodiesWorld::body("d1", "strlen", "an export")}, {"1040", BodiesWorld::body("d2")}});
    const auto got = w.bodies.bodies("sha-a");
    CB_CHECK_EQ(got.size(), std::size_t{2});
    CB_CHECK_EQ(got.at("1000").digest, "d1");
    CB_CHECK_EQ(got.at("1000").name.value_or(""), "strlen");
    CB_CHECK_EQ(got.at("1000").source.value_or(""), "an export");
    CB_CHECK(!got.at("1040").name.has_value());
    w.bodies.set_bodies(kLibA, {{"1000", BodiesWorld::body("d9")}});   // replaces what the module had
    const auto again = w.bodies.bodies("sha-a");
    CB_CHECK_EQ(again.size(), std::size_t{1});
    CB_CHECK_EQ(again.at("1000").digest, "d9");
    CB_CHECK(w.bodies.bodies("sha-b").empty());
});

CB_TEST("store.knowledge_bodies.a_name_given_to_a_body_holds_in_every_module_with_it", [] {
    BodiesWorld w;
    w.bodies.set_bodies(kLibA, {{"1000", BodiesWorld::body("same")}, {"1100", BodiesWorld::body("only-a")}});
    w.bodies.set_bodies(kLibB, {{"2000", BodiesWorld::body("same")}});
    CB_CHECK(w.bodies.names_from_bodies("sha-b").empty());   // nobody has named it
    w.bodies.name_body("same", "memcpy");
    const auto b = w.bodies.names_from_bodies("sha-b");
    CB_CHECK_EQ(b.size(), std::size_t{1});
    CB_CHECK_EQ(b.at("2000").name, "memcpy");
    CB_CHECK_EQ(b.at("2000").source, "named by hand");
    const auto a = w.bodies.names_from_bodies("sha-a");
    CB_CHECK_EQ(a.size(), std::size_t{1});
    CB_CHECK_EQ(a.at("1000").name, "memcpy");
    w.bodies.name_body("same", "memmove", "the C runtime's source");
    CB_CHECK_EQ(w.bodies.names_from_bodies("sha-b").at("2000").name, "memmove");
    CB_CHECK_EQ(w.bodies.names_from_bodies("sha-b").at("2000").source, "the C runtime's source");
});

CB_TEST("store.knowledge_bodies.a_module_s_own_name_is_shared_with_the_others", [] {
    BodiesWorld w;
    w.bodies.set_bodies(kLibA, {{"1000", BodiesWorld::body("shared", "wsprintf", "an export")}});
    w.bodies.set_bodies(kLibB, {{"2000", BodiesWorld::body("shared")}});
    const auto b = w.bodies.names_from_bodies("sha-b");   // (a body this module has not named, and every other module that names it agrees)
    CB_CHECK_EQ(b.at("2000").name, "wsprintf");
    CB_CHECK_EQ(b.at("2000").source, "the same body in liba.dll");
    CB_CHECK(w.bodies.names_from_bodies("sha-a").empty());   // the module's own name is not "from elsewhere"
});

CB_TEST("store.knowledge_bodies.the_counts_of_what_the_modules_share", [] {
    BodiesWorld w;
    CB_CHECK_EQ(w.bodies.shared_bodies().recorded, 0);
    w.bodies.set_bodies(kLibA, {{"1000", BodiesWorld::body("x")}, {"1010", BodiesWorld::body("y")}, {"1020", BodiesWorld::body("x")}});
    w.bodies.set_bodies(kLibB, {{"2000", BodiesWorld::body("x")}, {"2010", BodiesWorld::body("z")}});
    const cb::store::BodyCounts c = w.bodies.shared_bodies();
    CB_CHECK_EQ(c.recorded, 5);
    CB_CHECK_EQ(c.distinct, 3);
    CB_CHECK_EQ(c.in_several, 1);   // x is in both modules
    CB_CHECK_EQ(c.copies, 3);       // and its three rows
});
