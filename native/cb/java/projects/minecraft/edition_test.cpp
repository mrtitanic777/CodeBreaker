// Edition resolution. The expected values of the version-id and corpus cases were produced by the Python module
// (codebreaker.java.projects.minecraft.edition) and checked by hand against the era each id/corpus names.
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/edition.hpp"

namespace {

using namespace cb::java::projects::minecraft::edition;
using cb::java::ir::JClass;

std::string edition_of(std::string_view v) {
    const auto e = edition_for_version(v);
    return e ? std::string(edition_name(*e)) : "-";
}

CorpusIndex make_corpus(std::initializer_list<std::pair<const char*, const char*>> classes) {
    CorpusIndex c;
    for (const auto& [name, pkg] : classes) c.add(name, pkg ? std::optional<std::string>(pkg) : std::nullopt);
    return c;
}

std::string detected(const CorpusIndex& c) {
    const auto e = detect_edition(c);
    return e ? std::string(edition_name(*e)) : "-";
}

CB_TEST("java.minecraft.edition.version_ids", [] {
    CB_CHECK_EQ(edition_of("rd-132211"), "rubydung");
    CB_CHECK_EQ(edition_of("RD_132211"), "rubydung");
    CB_CHECK_EQ(edition_of("c0.30_01c"), "rubydung");
    CB_CHECK_EQ(edition_of("c.1"), "rubydung");
    CB_CHECK_EQ(edition_of("classic-0.30"), "rubydung");
    CB_CHECK_EQ(edition_of("inf-20100618"), "infdev");
    CB_CHECK_EQ(edition_of("infdev"), "infdev");
    CB_CHECK_EQ(edition_of("in-20100223"), "indev");
    CB_CHECK_EQ(edition_of("indev-0.31"), "indev");
    CB_CHECK_EQ(edition_of("a1.2.6"), "alpha");
    CB_CHECK_EQ(edition_of("b1.6.6"), "beta");
    CB_CHECK_EQ(edition_of("1.2.5"), "release");
    CB_CHECK_EQ(edition_of("  1.12  "), "release");
    CB_CHECK_EQ(edition_of("10.1"), "release");
    // not recognised
    CB_CHECK_EQ(edition_of(""), "-");
    CB_CHECK_EQ(edition_of("   "), "-");
    CB_CHECK_EQ(edition_of("1"), "-");
    CB_CHECK_EQ(edition_of("1."), "-");
    CB_CHECK_EQ(edition_of("x1.2"), "-");
    CB_CHECK_EQ(edition_of("rd"), "-");
    CB_CHECK_EQ(edition_of("in"), "-");
    // 'in' with no separator and no 'indev' is not an Indev id
    CB_CHECK_EQ(edition_of("input"), "-");
});

CB_TEST("java.minecraft.edition.names", [] {
    for (Edition e : kEditions) {
        CB_CHECK(parse_edition(edition_name(e)) == e);
        CB_CHECK(parse_edition(profile_name(e)) == e);
        CB_CHECK_EQ(profile_name(e), "minecraft_" + std::string(edition_name(e)));
    }
    CB_CHECK(!parse_edition("gamma"));
    CB_CHECK(!parse_edition(""));
});

CB_TEST("java.minecraft.edition.detect", [] {
    CB_CHECK_EQ(detected(make_corpus({})), "-");
    CB_CHECK_EQ(detected(make_corpus({{"Main", "com.mojang.rubydung"}})), "rubydung");
    CB_CHECK_EQ(detected(make_corpus({{"RubyDung", nullptr}})), "rubydung");
    CB_CHECK_EQ(detected(make_corpus({{"Game", "com.mojang.rubydung.level"}, {"Chunk", nullptr}})), "rubydung");
    // a package that merely starts with the name is not below it
    CB_CHECK_EQ(detected(make_corpus({{"Chunk", "com.mojang.rubydungeon"}})), "alpha");
    CB_CHECK_EQ(detected(make_corpus({{"World", "net.minecraft.server"}})), "release");
    CB_CHECK_EQ(detected(make_corpus({{"IBlockState", nullptr}})), "release");
    CB_CHECK_EQ(detected(make_corpus({{"BiomeGenBase", nullptr}, {"Chunk", nullptr}})), "beta");
    CB_CHECK_EQ(detected(make_corpus({{"StatList", nullptr}})), "beta");
    // release beats beta
    CB_CHECK_EQ(detected(make_corpus({{"RegionFile", nullptr}, {"WorldProviderEnd", nullptr}})), "release");
    // Chunk streaming: the game-package layout is infdev, a flat src package is alpha
    CB_CHECK_EQ(detected(make_corpus({{"Chunk", "net.minecraft.game.level"}})), "infdev");
    CB_CHECK_EQ(detected(make_corpus({{"ChunkProviderGenerate", "net.minecraft.game.level"}})), "infdev");
    CB_CHECK_EQ(detected(make_corpus({{"Chunk", "net.minecraft.game.level"}, {"Block", "net.minecraft.src"}})), "alpha");
    CB_CHECK_EQ(detected(make_corpus({{"Chunk", "net.minecraft.src"}})), "alpha");
    CB_CHECK_EQ(detected(make_corpus({{"ChunkProvider", nullptr}})), "alpha");
    CB_CHECK_EQ(detected(make_corpus({{"WorldChunkManager", "net.minecraft.src"}, {"ChunkProviderGenerate", nullptr}})), "alpha");
    // a name that only starts like ChunkProvider but sorts before it is not a chunk provider
    CB_CHECK_EQ(detected(make_corpus({{"ChunkCoordinates", nullptr}})), "-");
    // finite world: indev
    CB_CHECK_EQ(detected(make_corpus({{"World", "net.minecraft.game.level"}})), "indev");
    CB_CHECK_EQ(detected(make_corpus({{"LevelLoader", nullptr}})), "indev");
    CB_CHECK_EQ(detected(make_corpus({{"World", nullptr}, {"Material", nullptr}})), "indev");
    CB_CHECK_EQ(detected(make_corpus({{"Level", nullptr}, {"Tile", nullptr}})), "indev");
    CB_CHECK_EQ(detected(make_corpus({{"World", nullptr}})), "-");
    CB_CHECK_EQ(detected(make_corpus({{"Foo", "bar"}})), "-");
});

CB_TEST("java.minecraft.edition.index_of_classes", [] {
    std::vector<JClass> classes(2);
    classes[0].name = "Chunk";
    classes[0].package = "net.minecraft.src";
    classes[1].name = "Block";
    const CorpusIndex idx = CorpusIndex::of(classes);
    CB_CHECK_EQ(idx.names.size(), std::size_t{2});
    CB_CHECK(idx.packages.contains("net.minecraft.src"));
    CB_CHECK_EQ(idx.packages.size(), std::size_t{1});
    CB_CHECK(detect_edition(idx) == Edition::Alpha);
});

CB_TEST("java.minecraft.edition.resolve_precedence", [] {
    const CorpusIndex alpha = make_corpus({{"Chunk", "net.minecraft.src"}});
    // explicit beats version beats detection
    CB_CHECK_EQ(*resolve("beta", "a1.2.6", &alpha), "minecraft_beta");
    CB_CHECK_EQ(*resolve("minecraft_infdev", "a1.2.6", &alpha), "minecraft_infdev");
    CB_CHECK_EQ(*resolve("", "b1.6.6", &alpha), "minecraft_beta");
    CB_CHECK_EQ(*resolve("", "", &alpha), "minecraft_alpha");
    // an unrecognised version id falls through to detection
    CB_CHECK_EQ(*resolve("", "zzz", &alpha), "minecraft_alpha");
    // nothing conclusive: keep the default
    CB_CHECK(!resolve());
    CB_CHECK(!resolve("", "zzz"));
    const CorpusIndex nothing = make_corpus({{"Foo", "bar"}});
    CB_CHECK(!resolve("", "", &nothing));
});

CB_TEST("java.minecraft.edition.resolve_unknown_explicit", [] {
    CB_CHECK_THROWS_AS(cb::UsageError, resolve("gamma"));
    CB_CHECK_THROWS_AS(cb::UsageError, resolve("gamma", "a1.2.6"));
    try {
        (void)resolve("gamma");
        CB_CHECK(false);
    } catch (const cb::UsageError& e) {
        const std::string msg = e.what();
        CB_CHECK(msg.find("gamma") != std::string::npos);
        CB_CHECK(msg.find("alpha, beta, indev, infdev, release, rubydung") != std::string::npos);
    }
});

}  // namespace
