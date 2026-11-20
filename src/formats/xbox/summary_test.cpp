// Tests of cb/formats/xbox/summary.hpp: what `info` adds to the format line of an Xbox file. The expected facts are the recorded answers for the same
// files (testdata/assets/make_asset_goldens: the fixtures of the readers' tests, an XACT wave bank and an XUI package built for the purpose);
// the files are identified by cb::formats::detect as `info` does.
//
// Behaviour pinned here: a boolean is shown true / false (not True / False) and a format name that is none is left out (not "None"); the section tags of a XUR scene are
// the tags.
#include "formats/xbox/summary.hpp"

#include <map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/detect.hpp"
#include "formats/inspect.hpp"
#include "formats/xbox/asset_dump_kit.hpp"

namespace summary_test {

using namespace cb;
using namespace cb::formats;

// The golden facts of one fixture, as ours show them.
static xbox::SummaryFacts expected(const std::string& name) {
    xbox::SummaryFacts out;
    const std::string prefix = "summary." + name + ".";
    const std::string body = fs::read_text(fs::from_text(xbox::asset_dump::fixture("python_goldens.txt")));
    for (const std::string_view line : text::lines(body)) {
        if (!line.starts_with(prefix)) continue;
        const std::string_view rest = line.substr(prefix.size());
        const std::size_t eq = rest.find('=');
        std::string key(rest.substr(0, eq)), value(rest.substr(eq + 1));
        if (value == "None") continue;
        if (value == "True") value = "true";
        if (value == "False") value = "false";
        out.emplace_back(std::move(key), std::move(value));
    }
    return out;
}

static xbox::SummaryFacts ours(const std::string& name) {
    const std::filesystem::path path = fs::from_text(xbox::asset_dump::fixture(name));
    const std::vector<detect::Detected> found = detect::identify(path);
    CB_CHECK(!found.empty());
    return found.empty() ? xbox::SummaryFacts{} : xbox::summary(path, found.front());
}

static std::string show(const xbox::SummaryFacts& facts) {
    std::string out;
    for (const auto& [k, v] : facts) out += "  " + k + "=" + v + "\n";
    return out;
}

CB_TEST("formats.xbox.summary.each_fixture_is_summarised", [] {
    for (const char* name : {"gpd.bin", "xbe_demo.bin", "xpr0.bin", "xpr2.bin", "xur.bin", "t.bik", "t.chd", "xwb.bin", "pack.xzp"}) {
        const xbox::SummaryFacts want = expected(name), got = ours(name);
        CB_CHECK(!want.empty());
        CB_CHECK(want == got);
        if (want != got) cb::testing::record_failure(__FILE__, __LINE__, std::string(name) + "\n want:\n" + show(want) + " ours:\n" + show(got));
    }
});

CB_TEST("formats.xbox.summary.the_info_command_describes_an_xbox_file_by_its_own_reader", [] {
    // `info` of a default.xbe says its title and title id
    fs::ScopedTempDir dir("summary");
    fs::write_file(dir.path() / "default.xbe", fs::read_file(fs::from_text(xbox::asset_dump::fixture("xbe_small.bin"))));
    testing::CapturedStd cap;
    CB_CHECK_EQ(cli::Cli("codebreaker", {inspect::info_command()}).run({"info", fs::to_text(dir.path() / "default.xbe")}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("original Xbox executable (XBE)") != std::string::npos);
    CB_CHECK(out.find("Halo") != std::string::npos);
    CB_CHECK(out.find("4D530037") != std::string::npos);
    CB_CHECK(out.find("0x00010000") != std::string::npos);   // the base
});

CB_TEST("formats.xbox.summary.a_format_with_nothing_to_add_gives_no_facts", [] {
    detect::Detected other{"Xbox 360 executable (XEX2)", "", "xbox360", "xex"};
    CB_CHECK(xbox::summary(fs::from_text(xbox::asset_dump::fixture("gpd.bin")), other).empty());
});

CB_TEST("formats.xbox.summary.a_damaged_file_is_a_format_error", [] {
    fs::ScopedTempDir dir("summary");
    // an XBE whose certificate is outside the file, a Bink header cut short, and an XDBF of nothing
    const Bytes xbe = fs::read_file(fs::from_text(xbox::asset_dump::fixture("xbe_demo.bin")));
    fs::write_file(dir.path() / "short.xbe", ByteView(xbe).first(0x150));
    fs::write_file(dir.path() / "short.bik", to_bytes("BIKi\x01\x02"));
    fs::write_file(dir.path() / "empty.gpd", to_bytes("XDBF"));
    CB_CHECK_THROWS_AS(FormatError, (void)xbox::summary(dir.path() / "short.xbe", detect::Detected{"original Xbox executable (XBE)", "", "xbox", "info"}));
    CB_CHECK_THROWS_AS(FormatError, (void)xbox::summary(dir.path() / "short.bik", detect::Detected{"Bink video", "", "media", "info"}));
    CB_CHECK_THROWS_AS(FormatError, (void)xbox::summary(dir.path() / "empty.gpd", detect::Detected{"Xbox 360 database (XDBF: GPD / SPA)", "", "xbox360", "info"}));
});

}  // namespace summary_test
