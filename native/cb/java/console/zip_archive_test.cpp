#include "cb/java/console/zip_archive.hpp"

#include <cstring>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;

std::string text_of(const Bytes& b) { return std::string(as_text(b)); }

// A zip assembled by hand, stored entries only, so that the odd cases (names, flags, damage) can be built exactly.
struct Member {
    std::string name;                 // as written (bytes)
    std::string data;
    std::uint16_t flags = 0;
    std::uint16_t method = 0;
    std::optional<std::string> header_name;   // the name in the local header, when it differs
    std::optional<std::uint32_t> crc;         // a wrong CRC, when asked for
};

Bytes make_zip(const std::vector<Member>& members, const std::string& comment = "", std::size_t prefix = 0) {
    io::ByteWriter w;
    for (std::size_t i = 0; i < prefix; ++i) w.u8('x');
    std::vector<std::uint32_t> offsets;
    for (const Member& m : members) {
        offsets.push_back(static_cast<std::uint32_t>(w.size() - prefix));
        const std::string& header_name = m.header_name.value_or(m.name);
        w.bytes(as_bytes("PK\x03\x04"));
        w.u16(20);
        w.u16(m.flags);
        w.u16(m.method);
        w.u32(0);
        w.u32(m.crc.value_or(crypto::crc32(as_bytes(m.data))));
        w.u32(static_cast<std::uint32_t>(m.data.size()));
        w.u32(static_cast<std::uint32_t>(m.data.size()));
        w.u16(static_cast<std::uint16_t>(header_name.size()));
        w.u16(0);
        w.bytes(as_bytes(header_name));
        w.bytes(as_bytes(m.data));
    }
    const std::size_t directory_start = w.size();
    for (std::size_t i = 0; i < members.size(); ++i) {
        const Member& m = members[i];
        w.bytes(as_bytes("PK\x01\x02"));
        w.u16(20);
        w.u16(20);
        w.u16(m.flags);
        w.u16(m.method);
        w.u32(0);
        w.u32(m.crc.value_or(crypto::crc32(as_bytes(m.data))));
        w.u32(static_cast<std::uint32_t>(m.data.size()));
        w.u32(static_cast<std::uint32_t>(m.data.size()));
        w.u16(static_cast<std::uint16_t>(m.name.size()));
        w.u16(0);
        w.u16(0);
        w.u16(0);
        w.u16(0);
        w.u32(0);
        w.u32(offsets[i]);
        w.bytes(as_bytes(m.name));
    }
    const std::size_t directory_size = w.size() - directory_start;
    w.bytes(as_bytes("PK\x05\x06"));
    w.u16(0);
    w.u16(0);
    w.u16(static_cast<std::uint16_t>(members.size()));
    w.u16(static_cast<std::uint16_t>(members.size()));
    w.u32(static_cast<std::uint32_t>(directory_size));
    w.u32(static_cast<std::uint32_t>(directory_start - prefix));
    w.u16(static_cast<std::uint16_t>(comment.size()));
    w.bytes(as_bytes(comment));
    return w.take();
}

CB_TEST("java.console.zip.reads_a_real_jar", [] {
    const ZipArchive jar = ZipArchive::open(test::data("assets/game.jar"));
    std::vector<std::string> names;
    for (const auto& e : jar.entries()) names.push_back(e.name);
    CB_CHECK_EQ(names.size(), std::size_t{18});
    CB_CHECK_EQ(names.front(), "gui/gui.png");
    CB_CHECK_EQ(names[14], "com/mojang/Thing.class");
    CB_CHECK_EQ(names[15], "empty/");
    CB_CHECK(jar.find("empty/")->is_directory());
    CB_CHECK(!jar.find("lang/en_US.lang")->is_directory());
    CB_CHECK_EQ(text_of(jar.read("lang/en_US.lang")), "menu.play=Play\nmenu.quit=Quit\n");
    CB_CHECK_EQ(text_of(jar.read("misc/notes.txt")), "hello\r\nworld\n");
    CB_CHECK_EQ(jar.find("misc/stored.properties")->method, 0);               // a stored entry
    CB_CHECK_EQ(text_of(jar.read("misc/stored.properties")), "a=1\n");
    CB_CHECK_EQ(jar.find("lang/en_US.lang")->method, 8);                       // a deflated one
    CB_CHECK_EQ(jar.read("empty/").size(), std::size_t{0});
    CB_CHECK(jar.find("nothing/here.txt") == nullptr);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)jar.read("nothing/here.txt"));
    // the name that occurs twice finds the later entry
    const auto* later = jar.find("gui/gui.png");
    CB_CHECK(later == &jar.entries().back());
    for (const auto& e : jar.entries()) CB_CHECK_NOTHROW((void)jar.read(e));    // every entry reads and passes its CRC
});

CB_TEST("java.console.zip.names", [] {
    const std::string utf8 = "caf\xC3\xA9/x.txt";
    const Bytes z = make_zip({{"dir\\file.txt", "1"},
                              {utf8, "2", 0x800},                    // the UTF-8 flag
                              {"caf\x82/y.txt", "3"},                // no flag: the DOS code page, 0x82 is e-acute
                              {std::string("nul\0cut", 7) + ".txt", "4"},
                              {"Mixed/Case.TXT", "5"}});
    const ZipArchive a = ZipArchive::from_bytes(z);
    CB_CHECK_EQ(text_of(a.read("dir/file.txt")), "1");                   // a backslash is a folder separator
    CB_CHECK_EQ(a.entries()[0].stored_name, "dir\\file.txt");
    CB_CHECK_EQ(text_of(a.read(utf8)), "2");
    CB_CHECK_EQ(text_of(a.read("caf\xC3\xA9/y.txt")), "3");              // decoded to UTF-8
    CB_CHECK_EQ(text_of(a.read("nul")), "4");                            // cut at the NUL
    CB_CHECK(a.find("mixed/case.txt") == nullptr);                       // names are exact
    CB_CHECK_EQ(text_of(a.read("Mixed/Case.TXT")), "5");
    // a repeated name: the last wins
    const ZipArchive dup = ZipArchive::from_bytes(make_zip({{"a.txt", "old"}, {"a.txt", "new"}}));
    CB_CHECK_EQ(dup.entries().size(), std::size_t{2});
    CB_CHECK_EQ(text_of(dup.read("a.txt")), "new");
    CB_CHECK_EQ(text_of(dup.read(dup.entries()[0])), "old");             // while an entry in hand is its own
});

CB_TEST("java.console.zip.archives_that_are_not_plain", [] {
    // a comment after the end record, and data before the archive (a self-extracting file)
    CB_CHECK_EQ(text_of(ZipArchive::from_bytes(make_zip({{"a.txt", "hello"}}, "a comment of some length")).read("a.txt")), "hello");
    CB_CHECK_EQ(text_of(ZipArchive::from_bytes(make_zip({{"a.txt", "hello"}}, "", 100)).read("a.txt")), "hello");
    CB_CHECK_EQ(text_of(ZipArchive::from_bytes(make_zip({{"a.txt", "hello"}}, "tail", 5000)).read("a.txt")), "hello");
    // no members at all is a valid archive
    CB_CHECK(ZipArchive::from_bytes(make_zip({})).entries().empty());
    // zip64 is recognised and refused
    Bytes z64 = make_zip({{"a.txt", "x"}});
    for (std::size_t i = z64.size() - 22 + 16; i < z64.size() - 22 + 20; ++i) z64[i] = 0xFF;   // the directory offset
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)ZipArchive::from_bytes(z64));
});

CB_TEST("java.console.zip.damage", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, (void)ZipArchive::from_bytes({}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)ZipArchive::from_bytes(to_bytes("not a zip file at all, just some text")));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)ZipArchive::open(test::data("assets/nowhere.jar")));
    const Bytes good = make_zip({{"a.txt", "hello"}, {"b.txt", "world"}});
    for (std::size_t n = 0; n < good.size(); ++n) {   // every cut: a clean error or (a cut of only the comment) the same archive
        const Bytes cut(good.begin(), good.begin() + static_cast<std::ptrdiff_t>(n));
        try {
            (void)ZipArchive::from_bytes(cut);
            CB_CHECK(false);   // a cut archive has no end record
        } catch (const cb::FormatError&) {
        }
    }
    // a bad CRC, an encrypted entry, a compression method nobody wrote here, a header that disagrees with the directory
    const ZipArchive crc = ZipArchive::from_bytes(make_zip({{"a.txt", "hello", 0, 0, std::nullopt, 12345u}}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)crc.read("a.txt"));
    const ZipArchive enc = ZipArchive::from_bytes(make_zip({{"a.txt", "hello", 1}}));
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)enc.read("a.txt"));
    const ZipArchive method = ZipArchive::from_bytes(make_zip({{"a.txt", "hello", 0, 12}}));
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)method.read("a.txt"));
    const ZipArchive differ = ZipArchive::from_bytes(make_zip({{"a.txt", "hello", 0, 0, std::string("b.txt")}}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)differ.read("a.txt"));
    // an entry offset that points outside the file
    Bytes wild = make_zip({{"a.txt", "hello"}});
    const std::size_t dir_at = wild.size() - 22 - (46 + 5);
    wild[dir_at + 42] = 0xFF; wild[dir_at + 43] = 0xFF; wild[dir_at + 44] = 0x7F;
    const ZipArchive wild_archive = ZipArchive::from_bytes(wild);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)wild_archive.read("a.txt"));
    // a deflated entry that claims an impossible size does not allocate it
    const ZipArchive real = ZipArchive::open(test::data("assets/game.jar"));
    Bytes bomb = fs::read_file(test::data("assets/game.jar"));
    const ZipArchive::Entry* lang = real.find("lang/de_DE.lang");
    // the uncompressed size sits at offset 24 of that entry's central-directory record: find it by its name
    for (std::size_t i = 0; i + 46 < bomb.size(); ++i) {
        if (std::memcmp(bomb.data() + i, "PK\x01\x02", 4) == 0 &&
            std::string_view(reinterpret_cast<const char*>(bomb.data() + i + 46), lang->name.size()) == lang->name) {
            bomb[i + 24] = 0xFF; bomb[i + 25] = 0xFF; bomb[i + 26] = 0xFF; bomb[i + 27] = 0x7F;
            break;
        }
    }
    const ZipArchive bombed = ZipArchive::from_bytes(bomb);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)bombed.read("lang/de_DE.lang"));
    // a damaged deflate stream
    Bytes broken = fs::read_file(test::data("assets/game.jar"));
    const std::size_t body = 30 + std::string("lang/en_US.lang").size();
    for (std::size_t i = 0; i + 4 < broken.size(); ++i)
        if (std::memcmp(broken.data() + i, "PK\x03\x04", 4) == 0 &&
            std::string_view(reinterpret_cast<const char*>(broken.data() + i + 30), 15) == "lang/en_US.lang") {
            for (std::size_t k = 0; k < 6; ++k) broken[i + body + k] ^= 0xA5;
            break;
        }
    CB_CHECK_THROWS_AS(cb::FormatError, (void)ZipArchive::from_bytes(broken).read("lang/en_US.lang"));
});

}  // namespace
