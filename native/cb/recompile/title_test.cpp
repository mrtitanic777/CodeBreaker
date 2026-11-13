// Tests of the title finder over folders on disk and over volumes made up in the test (the container readers are formats/xbox/volumes',
// which hands its volumes in through `FindVolumes`) and over the real readers of formats/xbox/volumes: what is a title, what it is called, and what an unpack writes and when it does not.

#include "cb/recompile/title.hpp"

#include <map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/ciso.hpp"
#include "cb/formats/xbox/gdf.hpp"

namespace cb::recompile::title {
namespace {

namespace stdfs = std::filesystem;
using formats::media::BootField;
using formats::media::Entry;

Bytes xex(std::string_view tail = "") {
    Bytes b = {'X', 'E', 'X', '2'};
    b.insert(b.end(), tail.begin(), tail.end());
    return b;
}

// A volume of a few files, made up: entry paths as given (folders by name).
class FakeVolume : public formats::media::Volume {
public:
    FakeVolume(std::string label, std::string display_name, std::vector<std::pair<std::string, Bytes>> files) : files_(std::move(files)) {
        label_ = std::move(label);
        if (!display_name.empty()) boot_.push_back(BootField{"display name", std::move(display_name), std::nullopt});
        for (const auto& [path, data] : files_) {
            Entry e;
            e.path = path;
            e.size = data.size();
            e.is_dir = path.ends_with('/');
            entries_.push_back(e);
        }
    }
    formats::media::VolumeKind kind() const override { return formats::media::VolumeKind::Iso9660; }
    const std::vector<Entry>& files() override { return entries_; }
    Bytes read(const Entry& entry) override { return files_.at(static_cast<std::size_t>(&entry - entries_.data())).second; }

private:
    std::vector<std::pair<std::string, Bytes>> files_;
    std::vector<Entry> entries_;
};

// Finds a volume by the path's file name; counts the asks.
struct Finder {
    std::map<std::string, std::function<Volumes()>> known;   // by file name: how to make its volumes
    std::map<std::string, bool> damaged;
    mutable std::vector<std::string> asked;
    std::optional<Volumes> operator()(const stdfs::path& p) const {
        const std::string name = fs::to_text(p.filename());
        asked.push_back(name);
        if (damaged.contains(name)) throw FormatError("stfs", 0, "damaged");
        const auto it = known.find(name);
        if (it == known.end()) return std::nullopt;
        return it->second();
    }
};

std::function<Volumes()> one(std::string label, std::string display, std::vector<std::pair<std::string, Bytes>> files) {
    return [=] {
        Volumes v;
        v.push_back(std::make_unique<FakeVolume>(label, display, files));
        return v;
    };
}

void put(const stdfs::path& p, ByteView data) { fs::write_file(p, data); }

CB_TEST("recompile.title.a_folder_with_default_xex_is_a_title_named_for_the_folder", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "Halo" / "default.xex", xex("body"));
    put(dir.path() / "Halo" / "media" / "a.bin", Bytes{1, 2, 3});
    const auto t = locate(dir.path() / "Halo", Finder{});
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->name(), "Halo");
    CB_CHECK(t->read_xex() == xex("body"));
});

CB_TEST("recompile.title.default_xex_is_found_by_name_without_case_and_by_its_magic", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "A" / "Default.XEX", xex());
    put(dir.path() / "B" / "default.xex", Bytes{'M', 'Z', 0, 0});   // not an executable of the 360
    CB_CHECK(default_xex(dir.path() / "A") == std::optional<std::string>("Default.XEX"));
    CB_CHECK(!default_xex(dir.path() / "B"));
    CB_CHECK(!default_xex(dir.path() / "absent"));
    CB_CHECK(!locate(dir.path() / "B", Finder{}));
    CB_CHECK(!locate(dir.path() / "nothing-here", Finder{}));
});

CB_TEST("recompile.title.a_package_names_its_title_from_the_display_name_or_label", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "pkg1", Bytes{'L', 'I', 'V', 'E', 0, 0});
    put(dir.path() / "pkg2", Bytes{'C', 'O', 'N', ' ', 0, 0});
    put(dir.path() / "disc.ISO", Bytes{0, 0, 0, 0});
    put(dir.path() / "dlc", Bytes{'L', 'I', 'V', 'E', 0, 0});
    Finder f;
    f.known["pkg1"] = one("ignored label", "  Halo 3\xC3\xA9 \t ", {{"default.xex", xex()}});       // non-ASCII and blanks go
    f.known["pkg2"] = one("Forza", "", {{"media/", {}}, {"\\Default.XEX", xex()}});               // a label; a path as the console writes it
    f.known["disc.ISO"] = one("", "", {{"default.xex", xex()}});                                  // the file's name, minus its extension
    f.known["dlc"] = one("A DLC pack", "", {{"content.bin", Bytes{1}}});                          // no default.xex: not a title

    const auto a = locate(dir.path() / "pkg1", f);
    CB_CHECK(a && a->name() == "Halo 3");
    const auto b = locate(dir.path() / "pkg2", f);
    CB_CHECK(b && b->name() == "Forza");
    CB_CHECK(b->read_xex() == xex());
    const auto d = locate(dir.path() / "disc.ISO", f);
    CB_CHECK(d && d->name() == "disc");
    CB_CHECK(!locate(dir.path() / "dlc", f));
});

CB_TEST("recompile.title.a_name_with_nothing_printable_is_title", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "x.iso", Bytes{0, 0, 0, 0});
    Finder f;
    f.known["x.iso"] = one("\xC3\xA9\xC3\xA8", "", {{"default.xex", xex()}});
    const auto t = locate(dir.path() / "x.iso", f);
    CB_CHECK(t && t->name() == "title");
});

CB_TEST("recompile.title.a_file_is_asked_of_the_reader_only_if_it_looks_like_a_container", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "readme.txt", Bytes{'h', 'e', 'l', 'p'});
    put(dir.path() / "broken", Bytes{'P', 'I', 'R', 'S', 1, 2});
    Finder f;
    f.damaged["broken"] = true;
    const FindVolumes find = std::ref(f);               // (the finder itself, not a copy: it counts what it is asked)
    CB_CHECK(!locate(dir.path() / "readme.txt", find));
    CB_CHECK(f.asked.empty());                          // not a package by its magic, not a disc by its extension
    CB_CHECK(!locate(dir.path() / "broken", find));     // a damaged one is not a title, and does not stop a scan
    CB_CHECK_EQ(f.asked.size(), 1u);
});

CB_TEST("recompile.title.a_folder_of_content_is_searched_depth_first_files_before_folders", [] {
    fs::ScopedTempDir dir("cb_title");
    // <root>/000D0000/pkg (a package: no title here but a LIVE magic); <root>/game/default.xex (a folder title)
    put(dir.path() / "root" / "000D0000" / "pkg", Bytes{'L', 'I', 'V', 'E', 0, 0});
    put(dir.path() / "root" / "game" / "default.xex", xex());
    Finder f;
    f.known["pkg"] = one("Content", "", {{"default.xex", xex()}});
    // the package in 000D0000 comes first in name order, but the folder with default.xex is taken at its parent's level, before descending
    const auto t = locate(dir.path() / "root", f);
    CB_CHECK(t && t->name() == "game");
    CB_CHECK(t->source() == stdfs::absolute(dir.path() / "root" / "game").lexically_normal());
    // without that folder the package is found where it lies
    stdfs::remove_all(dir.path() / "root" / "game");
    const auto p = locate(dir.path() / "root", f);
    CB_CHECK(p && p->name() == "Content");
    CB_CHECK(p->source().filename() == "pkg");
});

CB_TEST("recompile.title.a_games_on_demand_folder_takes_the_folders_name", [] {
    fs::ScopedTempDir dir("cb_title");
    stdfs::create_directories(dir.path() / "MyGame");
    Finder f;
    f.known["MyGame"] = one("GoD", "", {{"default.xex", xex()}});
    const auto t = locate(dir.path() / "MyGame", f);
    CB_CHECK(t && t->name() == "MyGame");
    CB_CHECK(t->read_xex() == xex());
    // a GoD volume that names itself keeps its name
    f.known["MyGame"] = one("Real Name", "", {{"default.xex", xex()}});
    const auto u = locate(dir.path() / "MyGame", f);
    CB_CHECK(u && u->name() == "Real Name");
});

CB_TEST("recompile.title.unpack_copies_a_folders_files_once", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "Halo" / "default.xex", xex());
    put(dir.path() / "Halo" / "media" / "deep" / "a.bin", Bytes{1, 2, 3});
    auto t = locate(dir.path() / "Halo", Finder{});
    CB_CHECK(t.has_value());
    const stdfs::path out = dir.path() / "out";
    CB_CHECK_EQ(t->unpack(out), 2u);
    CB_CHECK(fs::read_file(out / "media" / "deep" / "a.bin") == (Bytes{1, 2, 3}));
    CB_CHECK(fs::read_file(out / "default.xex") == xex());
    CB_CHECK_EQ(t->unpack(out), 0u);                                    // the marker says it is there
    // the marker is the Python's layout: a JSON object with the source, the size and the format
    const std::string marker = fs::read_text(out / kMarkerName);
    CB_CHECK(marker.find("\"source\": ") != std::string::npos && marker.find("\"size\": 7") != std::string::npos && marker.find("\"format\": 2") != std::string::npos);
    // a changed source (another size) is unpacked again
    put(dir.path() / "Halo" / "media" / "deep" / "a.bin", Bytes{1, 2, 3, 4});
    CB_CHECK_EQ(t->unpack(out), 2u);
    CB_CHECK(fs::read_file(out / "media" / "deep" / "a.bin") == (Bytes{1, 2, 3, 4}));
    // a marker of another format, or one that is not JSON, is not trusted
    fs::write_text(out / kMarkerName, "not json");
    CB_CHECK_EQ(t->unpack(out), 2u);
    CB_CHECK_EQ(t->unpack(out), 0u);
});

CB_TEST("recompile.title.unpack_writes_a_containers_files_and_keeps_them_inside_the_folder", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "pkg", Bytes{'L', 'I', 'V', 'E', 0, 0});
    Finder f;
    f.known["pkg"] = one("Game", "",
                         {{"folder/", {}}, {"default.xex", xex()}, {"folder\\a.bin", Bytes{7}}, {"..\\..\\evil.bin", Bytes{9}},
                          {"data/sub/x.dat", Bytes{4, 5}}});
    auto t = locate(dir.path() / "pkg", f);
    CB_CHECK(t.has_value());
    const stdfs::path out = dir.path() / "out" / "unpacked";
    CB_CHECK_EQ(t->unpack(out), 4u);
    CB_CHECK(fs::read_file(out / "default.xex") == xex());
    CB_CHECK(fs::read_file(out / "folder" / "a.bin") == (Bytes{7}));
    CB_CHECK(fs::read_file(out / "evil.bin") == (Bytes{9}));              // ".." is dropped
    CB_CHECK(!stdfs::exists(dir.path() / "evil.bin") && !stdfs::exists(dir.path() / "out" / "evil.bin"));
    CB_CHECK(fs::read_file(out / "data" / "sub" / "x.dat") == (Bytes{4, 5}));
    CB_CHECK_EQ(t->unpack(out), 0u);
    CB_CHECK(t->read_xex() == xex());
});

CB_TEST("recompile.title.the_size_of_a_source_is_a_files_or_all_the_files_under_a_folder", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "one.bin", Bytes(10, 0));
    put(dir.path() / "f" / "a", Bytes(3, 0));
    put(dir.path() / "f" / "d" / "b", Bytes(4, 0));
    CB_CHECK_EQ(size_of(dir.path() / "one.bin"), 10u);
    CB_CHECK_EQ(size_of(dir.path() / "f"), 7u);
    CB_CHECK_EQ(size_of(dir.path() / "absent"), 0u);
});

CB_TEST("recompile.title.reading_default_xex_of_a_title_whose_file_went_is_an_error", [] {
    fs::ScopedTempDir dir("cb_title");
    put(dir.path() / "T" / "default.xex", xex());
    auto t = locate(dir.path() / "T", Finder{});
    CB_CHECK(t.has_value());
    stdfs::remove(dir.path() / "T" / "default.xex");
    CB_CHECK_THROWS_AS(NotFoundError, t->read_xex());
});

// ---- through the real container readers (formats/xbox/volumes) -------------------------------------------------------------------------

CB_TEST("recompile.title.a_disc_image_plain_or_compressed_is_a_title_through_the_xbox_readers", [] {
    fs::ScopedTempDir dir("cb_title");
    const std::vector<formats::xbox::gdf::SourceFile> files = {{"default.xex", xex("an xex body")}, {"readme.txt", Bytes{'h', 'i'}}};
    put(dir.path() / "Game One.iso", formats::xbox::gdf::build(files));
    const auto iso = locate(dir.path() / "Game One.iso");
    CB_CHECK(iso && iso->name() == "Game One");   // the image's own name, its extension gone
    CB_CHECK(iso->read_xex() == xex("an xex body"));

    (void)formats::xbox::ciso::write_cso(dir.path() / "Game One.iso", dir.path() / "Game Two.cso");
    const auto cso = locate(dir.path() / "Game Two.cso");
    CB_CHECK(cso && cso->name() == "Game Two");
    CB_CHECK(cso->read_xex() == xex("an xex body"));

    // an image without a default.xex is not a title
    const std::vector<formats::xbox::gdf::SourceFile> no_xex = {{"readme.txt", Bytes{'h', 'i'}}};
    put(dir.path() / "Data.iso", formats::xbox::gdf::build(no_xex));
    CB_CHECK(!locate(dir.path() / "Data.iso"));

    // a folder of images: the first that holds a title (files in name order)
    stdfs::create_directories(dir.path() / "library");
    stdfs::copy_file(dir.path() / "Data.iso", dir.path() / "library" / "A Data.iso");
    stdfs::copy_file(dir.path() / "Game One.iso", dir.path() / "library" / "B Game.iso");
    const auto found = locate(dir.path() / "library");
    CB_CHECK(found && found->name() == "B Game");

    // the unpack writes the title's files
    const auto unpacked = dir.path() / "out";
    CB_CHECK_EQ(iso->unpack(unpacked), 2u);
    CB_CHECK(fs::read_file(unpacked / "default.xex") == xex("an xex body"));
});

CB_TEST("recompile.title.minecraft_from_the_nexia_library", [] {
    // The real titles (silent where they are not): a LIVE package of Minecraft: Story Mode inside a console's content folders, and
    // Minecraft Xbox 360 Edition as a folder of files. The Python's title.locate gives the same names and the same default.xex.
    const stdfs::path games = "C:/Nexia360/Games";
    std::error_code ec;
    if (stdfs::exists(games / "Minecraft Story Mode", ec)) {
        const auto t = locate(games / "Minecraft Story Mode");
        CB_CHECK(t.has_value());
        CB_CHECK_EQ(t->name(), "Minecraft: Story Mode");
        CB_CHECK(t->source().filename() == "E0836555A46901ACC670DA05E0AD637543C8CA1358");
        const Bytes xex_bytes = t->read_xex();
        CB_CHECK_EQ(xex_bytes.size(), 4026368u);
        CB_CHECK_EQ(crypto::to_hex(crypto::sha1(xex_bytes)), "ffa8d8178e9b62b05cf0561c91ee5f5986f243b8");
    }
    if (stdfs::exists(games / "Minecraft Xbox 360 Edition" / "default.xex", ec)) {
        const auto t = locate(games / "Minecraft Xbox 360 Edition");
        CB_CHECK(t.has_value());
        CB_CHECK_EQ(t->name(), "Minecraft Xbox 360 Edition");
        CB_CHECK_EQ(crypto::to_hex(crypto::sha1(t->read_xex())), "6349a9e55198b2224de4e3f86d1ff3fc873b907c");
    }
});


}  // namespace
}  // namespace cb::recompile::title
