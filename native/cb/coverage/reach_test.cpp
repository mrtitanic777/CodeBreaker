// cb-unity: alone
#include "cb/coverage/reach.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
namespace r = cb::coverage::reach;
namespace stdfs = std::filesystem;

Bytes bytes_of(std::string_view s) { return Bytes(s.begin(), s.end()); }

// A volume made of a list of files, as a disc or package gives them.
class ListVolume : public formats::media::Volume {
public:
    explicit ListVolume(std::vector<std::pair<std::string, std::string>> files) {
        for (auto& [path, data] : files) {
            formats::media::Entry e;
            e.path = path;
            e.size = data.size();
            entries_.push_back(e);
            data_.push_back(bytes_of(data));
        }
    }
    formats::media::VolumeKind kind() const override { return formats::media::VolumeKind::Iso9660; }
    const std::vector<formats::media::Entry>& files() override { return entries_; }
    Bytes read(const formats::media::Entry& entry) override {
        for (std::size_t i = 0; i < entries_.size(); ++i) {
            if (&entries_[i] == &entry) return data_[i];
        }
        return {};
    }

private:
    std::vector<formats::media::Entry> entries_;
    std::vector<Bytes> data_;
};

// Services with everything stubbed: platforms by extension, a fixed revision and clock, a state folder of its own.
r::Services services_in(const stdfs::path& state) {
    r::Services s;
    s.identify_platform = [](const stdfs::path& p) -> std::optional<r::PlatformInfo> {
        const std::string ext = text::lower(fs::to_text(p.extension()));
        if (ext == ".nes") return r::PlatformInfo{"NES", "nes", "6502", "iNES (.nes)", true};
        if (ext == ".gb") return r::PlatformInfo{"Game Boy", "gb", "sm83", "GB", true};
        if (ext == ".dat") return r::PlatformInfo{"Strange", "strange", "x", "", false};
        return std::nullopt;
    };
    s.tools_revision = [] { return std::string("abc1234"); };
    s.clock = [] { return 1700000000.0; };
    s.log = [](std::string_view) {};
    s.state_dir = state;
    return s;
}

void write(const stdfs::path& p, std::string_view data) {
    fs::make_dirs(p.parent_path());
    fs::write_file(p, bytes_of(data));
}

r::Row row(std::string platform, std::optional<std::string> lane, std::uint64_t size, std::string sha, std::string found = "x") {
    r::Row rw;
    rw.found = std::move(found);
    rw.platform = std::move(platform);
    rw.lane = std::move(lane);
    rw.family = rw.lane ? "x" : "gap";
    rw.path = "C:/p/" + sha + ".bin";
    rw.size = size;
    rw.sha = std::move(sha);
    return rw;
}

}  // namespace

CB_TEST("coverage.reach.gap_of_names_platforms_without_a_lane", [] {
    const auto gap = [](std::string_view head, const char* name = "x.bin") {
        return r::gap_of(name, as_bytes(head));
    };
    CB_CHECK_EQ(gap("PS-X EXE and the rest")->title, "PlayStation (PS1)");
    CB_CHECK_EQ(gap("\x80\x37\x12\x40")->title, "Nintendo 64");
    CB_CHECK_EQ(gap("\x37\x80\x40\x12")->cpu, "MIPS R4300");
    CB_CHECK_EQ(gap("\x40\x12\x37\x80")->title, "Nintendo 64");
    CB_CHECK(!gap("\x80\x37\x12"));   // a short head says nothing

    std::string gba(0x100, '\0');
    gba[3] = static_cast<char>(0xEA);
    gba[0xB2] = static_cast<char>(0x96);
    CB_CHECK_EQ(gap(gba)->title, "Game Boy Advance");
    CB_CHECK(!gap(std::string_view(gba).substr(0, 0xB3)));   // too short to carry the Game Boy Advance's mark

    CB_CHECK_EQ(gap("", "game.NDS")->title, "Nintendo DS");
    CB_CHECK_EQ(gap("", "game.sfc")->cpu, "65816");
    CB_CHECK_EQ(gap("", "game.SMC")->title, "Super Nintendo");

    // ELF: the machine field follows the file's byte order
    std::string elf(24, '\0');
    elf.replace(0, 4, "\x7f" "ELF");
    elf[5] = 1;   // little-endian
    elf[18] = 0x28;
    CB_CHECK_EQ(gap(elf)->title, "ELF program");
    CB_CHECK_EQ(gap(elf)->cpu, "ARM");
    elf[5] = 2;   // big-endian
    elf[18] = 0;
    elf[19] = 0x14;
    CB_CHECK_EQ(gap(elf)->cpu, "PowerPC");
    elf[19] = 0x77;
    CB_CHECK_EQ(gap(elf)->cpu, "machine 0x77");
    CB_CHECK(!gap(std::string_view(elf).substr(0, 19)));

    CB_CHECK_EQ(gap("PK\x03\x04", "a.apk")->cpu, "Dalvik");
    CB_CHECK(!gap("PK\x03\x04", "a.zip"));
    CB_CHECK(!gap("plain text"));
});

CB_TEST("coverage.reach.describe_by_platform_gap_or_nothing", [] {
    fs::ScopedTempDir tmp("reach-describe");
    const r::Services s = services_in(tmp.path() / "state");
    write(tmp.path() / "game.nes", "NES\x1a" "program");
    write(tmp.path() / "odd.dat", "whatever");
    write(tmp.path() / "console.bin", "PS-X EXE.......");
    write(tmp.path() / "notes.txt", "hello");

    const r::Described nes = r::describe(tmp.path() / "game.nes", s);
    CB_CHECK_EQ(nes.platform, "NES");
    CB_CHECK(nes.lane == std::optional<std::string>("nes"));
    CB_CHECK_EQ(nes.family, "6502");

    const r::Described odd = r::describe(tmp.path() / "odd.dat", s);   // a platform without a lane: named, no lane
    CB_CHECK_EQ(odd.platform, "Strange");
    CB_CHECK(!odd.lane);

    const r::Described gap = r::describe(tmp.path() / "console.bin", s);
    CB_CHECK_EQ(gap.platform, "PlayStation (PS1) (MIPS R3000)");
    CB_CHECK_EQ(gap.family, "gap");
    CB_CHECK(!gap.lane);

    const r::Described nothing = r::describe(tmp.path() / "notes.txt", s);
    CB_CHECK_EQ(nothing.platform, "");
    CB_CHECK(!nothing.lane);
    CB_CHECK(!nothing.found.empty());   // "unknown", or what detect says of text
});

CB_TEST("coverage.reach.spread_samples_evenly_across_sizes", [] {
    std::vector<r::Row> rows;
    for (std::uint64_t size : {900, 100, 500, 300, 700, 5000, 200, 400, 600, 800, 1000})
        rows.push_back(row("P", "p", size, "s" + std::to_string(size)));
    std::vector<r::Row*> all;
    for (r::Row& rw : rows) all.push_back(&rw);

    // the smallest, the largest under the limit, and between (10 rows under 1000: 100..1000 step 100)
    const auto picked = r::spread(all, 4, 1000);
    CB_CHECK_EQ(picked.size(), 4u);
    CB_CHECK_EQ(picked[0]->size, 100u);
    CB_CHECK_EQ(picked[3]->size, 1000u);
    CB_CHECK(picked[1]->size < picked[2]->size);
    CB_CHECK_EQ(picked[1]->size, 400u);   // index round(1 * 9 / 3) = 3
    CB_CHECK_EQ(picked[2]->size, 700u);   // index round(2 * 9 / 3) = 6

    // fewer rows than asked for: all of them, smallest first; a limit under the largest drops it
    const auto few = r::spread(all, 50, 1000);
    CB_CHECK_EQ(few.size(), 10u);
    CB_CHECK_EQ(few.front()->size, 100u);
    CB_CHECK_EQ(few.back()->size, 1000u);

    CB_CHECK_EQ(r::spread(all, 1, 1000).size(), 1u);
    CB_CHECK_EQ(r::spread(all, 1, 1000)[0]->size, 100u);
    CB_CHECK(r::spread({}, 3, 1000).empty());
    CB_CHECK_EQ(r::spread(all, 0, 1000).size(), 1u);   // none asked for still gives the smallest
    // round half to even at .5: 3 of 6 rows, indexes 0, round(2.5) = 2, 5
    std::vector<r::Row> six;
    for (std::uint64_t i = 1; i <= 6; ++i) six.push_back(row("P", "p", i * 10, "t" + std::to_string(i)));
    std::vector<r::Row*> six_ptrs;
    for (r::Row& rw : six) six_ptrs.push_back(&rw);
    const auto three = r::spread(six_ptrs, 3, 1000);
    CB_CHECK_EQ(three[1]->size, 30u);
});

CB_TEST("coverage.reach.unpack_takes_the_smallest_programs_and_keeps_names_inside_the_folder", [] {
    fs::ScopedTempDir tmp("reach-unpack");
    r::Services s = services_in(tmp.path() / "state");
    s.open_volumes = [](const stdfs::path&) {
        std::vector<std::unique_ptr<formats::media::Volume>> v;
        v.push_back(std::make_unique<ListVolume>(std::vector<std::pair<std::string, std::string>>{
            {"BIG/GAME.EXE", std::string(500, 'b')},
            {"README.TXT", "not a program"},
            {"SMALL.COM", "tiny"},
            {"..\\..\\ESCAPE.EXE", "sneaky"},
            {"MID/MID.DLL", std::string(50, 'm')},
        }));
        return v;
    };
    const auto out = r::unpack(tmp.path() / "disc.iso", 3, s);
    CB_CHECK_EQ(out.size(), 3u);   // smallest first: SMALL.COM (4), ESCAPE.EXE (6), MID.DLL (50)
    CB_CHECK_EQ(fs::to_text(out[0].filename()), "SMALL.COM");
    CB_CHECK_EQ(fs::to_text(out[1].filename()), "ESCAPE.EXE");
    CB_CHECK_EQ(fs::to_text(out[2].filename()), "MID.DLL");
    const stdfs::path unpacked = stdfs::weakly_canonical(tmp.path() / "state" / "reach" / "unpacked");
    for (const stdfs::path& p : out) {
        CB_CHECK(stdfs::exists(p));
        const std::string rel = fs::to_text(stdfs::relative(stdfs::weakly_canonical(p), unpacked));
        CB_CHECK(!rel.starts_with(".."));   // a name that climbs stays inside the folder
    }
    CB_CHECK(fs::read_file(out[0]) == bytes_of("tiny"));

    // not a container: nothing; a container that will not open: nothing, and the reason is said
    r::Services none = services_in(tmp.path() / "state2");
    none.open_volumes = [](const stdfs::path&) { return std::vector<std::unique_ptr<formats::media::Volume>>(); };
    CB_CHECK(r::unpack(tmp.path() / "x.bin", 5, none).empty());
    std::vector<std::string> said;
    r::Services broken = services_in(tmp.path() / "state3");
    broken.log = [&](std::string_view l) { said.emplace_back(l); };
    broken.open_volumes = [](const stdfs::path&) -> std::vector<std::unique_ptr<formats::media::Volume>> {
        throw FormatError("iso", 0, "damaged");
    };
    CB_CHECK(r::unpack(tmp.path() / "bad.iso", 5, broken).empty());
    CB_CHECK_EQ(said.size(), 1u);
    CB_CHECK(said[0].find("bad.iso: not opened") != std::string::npos);
});

CB_TEST("coverage.reach.programs_walks_folders_and_opens_containers", [] {
    fs::ScopedTempDir tmp("reach-programs");
    r::Services s = services_in(tmp.path() / "state");
    write(tmp.path() / "in" / "a.nes", "NES");
    write(tmp.path() / "in" / "sub" / "b.gb", "GB");
    write(tmp.path() / "in" / ".hidden" / "c.nes", "NES");
    write(tmp.path() / "in" / "node_modules" / "d.nes", "NES");
    write(tmp.path() / "in" / "pack" / "inside.txt", "text");
    write(tmp.path() / "single.nes", "NES");
    // the folder `pack` is a container (a Games-on-Demand title): its programs are taken, it is not walked
    s.open_volumes = [](const stdfs::path& p) {
        std::vector<std::unique_ptr<formats::media::Volume>> v;
        if (p.filename() == "pack") {
            v.push_back(std::make_unique<ListVolume>(std::vector<std::pair<std::string, std::string>>{{"DEFAULT.XEX", "xex"}}));
        }
        return v;
    };
    const auto found = r::programs({tmp.path() / "in", tmp.path() / "single.nes"}, 20, s);
    std::vector<std::string> names;
    for (const r::Found& f : found) names.push_back(fs::to_text(f.path.filename()));
    // files of a folder before its subfolders, the subfolders in name order (pack, sub); the hidden and node_modules folders are skipped
    CB_CHECK_EQ(text::join(names, ","), "a.nes,DEFAULT.XEX,b.gb,single.nes");
    CB_CHECK_EQ(found[0].origin, fs::to_text(tmp.path() / "in"));
    CB_CHECK_EQ(found[1].origin, fs::to_text(tmp.path() / "in" / "pack"));   // out of the container folder
    CB_CHECK_EQ(found[2].origin, fs::to_text(tmp.path() / "in"));
    CB_CHECK_EQ(found[3].origin, "");
});

CB_TEST("coverage.reach.judge_reports_what_the_measure_did", [] {
    fs::ScopedTempDir tmp("reach-judge");
    r::Services s = services_in(tmp.path() / "state");
    s.tally = [](const stdfs::path&) { return r::Tally{{"verified", 3}, {"weak", 1}}; };

    double clock = 100.0;
    s.clock = [&] { return clock; };
    s.measure_command = [&](const stdfs::path&) {
        clock = 102.26;   // the measure took 2.26 s of the harness's time
        return std::vector<std::string>{"cmd", "/c", "exit 0"};
    };
    r::Judged ok = r::judge("p.nes", 60, s);
    CB_CHECK_EQ(ok.stopped, "");
    CB_CHECK_EQ(ok.tally.at("verified"), 3);
    CB_CHECK(ok.seconds > 2.1 && ok.seconds < 2.4);

    // a failing measure: the last line it wrote says why, to 160 characters
    s.measure_command = [](const stdfs::path&) {
        return std::vector<std::string>{"cmd", "/c", "echo first & echo the last words & exit 3"};
    };
    CB_CHECK_EQ(r::judge("p.nes", 60, s).stopped, "the last words");
    s.measure_command = [](const stdfs::path&) { return std::vector<std::string>{"cmd", "/c", "exit 5"}; };
    CB_CHECK_EQ(r::judge("p.nes", 60, s).stopped, "exit 5");

    // a program that cannot be started
    s.measure_command = [](const stdfs::path&) { return std::vector<std::string>{"C:/no/such/program.exe"}; };
    CB_CHECK(!r::judge("p.nes", 60, s).stopped.empty());

    // no tally: the run says why, and the tally is empty
    s.measure_command = [](const stdfs::path&) { return std::vector<std::string>{"cmd", "/c", "exit 0"}; };
    s.tally = [](const stdfs::path&) -> r::Tally { throw UsageError("no store"); };
    const r::Judged none = r::judge("p.nes", 60, s);
    CB_CHECK(none.tally.empty());
    CB_CHECK(none.stopped.starts_with("no tally: "));
});

CB_TEST("coverage.reach.judge_stops_a_measure_at_its_time_limit", [] {
    fs::ScopedTempDir tmp("reach-timeout");
    r::Services s = services_in(tmp.path() / "state");
    s.tally = [](const stdfs::path&) { return r::Tally{}; };
    s.measure_command = [](const stdfs::path&) { return std::vector<std::string>{"cmd", "/c", "ping -n 30 127.0.0.1 >nul"}; };
    const r::Judged j = r::judge("p.nes", 1, s);
    CB_CHECK_EQ(j.stopped, "timed out after 1 s");
});

CB_TEST("coverage.reach.report_is_by_platform_with_the_stopped_ones_counted_apart", [] {
    std::vector<r::Row> rows;
    r::Row a = row("NES", "nes", 100, "a");
    a.judged = r::Judged{{{"verified", 6}, {"weak", 2}, {"unverified", 1}, {"no-compile", 1}}, 3.0, ""};
    r::Row b = row("NES", "nes", 200, "b");
    b.judged = r::Judged{{{"verified", 100}}, 300.0, "timed out after 300 s"};   // counted under Stopped only
    r::Row c = row("Game Boy", "gb", 50, "c");
    c.judged = r::Judged{{}, 1.0, ""};
    r::Row d = row("PlayStation (PS1) (MIPS R3000)", std::nullopt, 70, "d");
    r::Row e = row("", std::nullopt, 10, "e", "ZIP archive");
    e.family = "archive";
    r::Row f = row("", std::nullopt, 10, "f", "ZIP archive");
    f.family = "archive";
    r::Row g = row("", std::nullopt, 10, "g", "text");
    g.family = "";
    rows = {a, b, c, d, e, f, g};
    const std::string text = r::report(rows);

    const std::string expected =
        "# Reach\n\n"
        "What CodeBreaker made of every program it was given (`codebreaker reach`).\n"
        "Functions and percentages are those of the judged programs that finished; a program stopped at\n"
        "its time limit is counted under Stopped only.\n\n"
        "| Platform | Programs | Lane | Judged | Functions | Verified | Weak | Unverified | Stopped |\n"
        "|---|---|---|---|---|---|---|---|---|\n"
        "| Game Boy | 1 | gb | 1 | - | - | - | - |  |\n"
        "| NES | 2 | nes | 2 | 10 | 6 (60%) | 2 (20%) | 2 (20%) | 1 |\n"
        "| PlayStation (PS1) (MIPS R3000) | 1 | **none yet** | 0 | - | - | - | - |  |\n"
        "\nNot programs, or not recognised: 3 files (ZIP archive 2, text 1).\n"
        "\n## Where judging stopped\n\n"
        "- b.bin (NES): timed out after 300 s\n";
    CB_CHECK_EQ(text, expected);
});

CB_TEST("coverage.reach.run_files_results_and_measures_each_program_once", [] {
    fs::ScopedTempDir tmp("reach-run");
    const stdfs::path inputs = tmp.path() / "in";
    write(inputs / "small.nes", "NES-small");
    write(inputs / "big.nes", "NES-" + std::string(300, 'x'));
    write(inputs / "copy.nes", "NES-small");   // the same bytes as small.nes: counted once
    write(inputs / "boot.bin", "PS-X EXE ....");
    write(inputs / "readme.txt", "hello");
    write(inputs / "data.xna", "x");

    r::Services s = services_in(tmp.path() / "state");
    std::vector<std::string> log;
    s.log = [&](std::string_view l) { log.emplace_back(l); };
    int measured = 0, tallied = 0;
    s.measure_command = [&](const stdfs::path&) {
        ++measured;
        return std::vector<std::string>{"cmd", "/c", "exit 0"};
    };
    s.tally = [&](const stdfs::path&) {
        ++tallied;
        return r::Tally{{"verified", 4}, {"weak", 1}};
    };

    r::Options o;
    o.judge = true;
    o.per_platform = 5;
    const r::Result first = r::run({inputs}, o, s);
    CB_CHECK_EQ(first.rows.size(), 5u);   // small.nes, copy.nes deduplicated, big.nes, boot.bin, readme.txt, data.xna
    CB_CHECK_EQ(measured, 2);             // the two distinct NES programs; the gap and the unknown are not judged
    CB_CHECK_EQ(tallied, 2);
    CB_CHECK_EQ(log.front(), "6 files found");

    const stdfs::path results = tmp.path() / "state" / "reach" / "reach.json";
    CB_CHECK(stdfs::exists(results));
    CB_CHECK(stdfs::exists(tmp.path() / "state" / "reach" / "reach.md"));
    const Json doc = json::load(results);
    CB_CHECK_EQ(doc["tools"].get<std::string>(), "abc1234");
    CB_CHECK_EQ(doc["programs"].size(), 5u);
    const std::vector<r::Row> back = r::read_results(results);
    CB_CHECK_EQ(back.size(), 5u);
    std::size_t judged = 0;
    for (const r::Row& rw : back) {
        if (rw.judged) {
            ++judged;
            CB_CHECK(rw.picked);
            CB_CHECK_EQ(rw.tools.value(), "abc1234");
            CB_CHECK_EQ(rw.judged->tally.at("verified"), 4);
        }
        CB_CHECK_EQ(rw.sha.size() == 64 || rw.sha.starts_with("path:"), true);
    }
    CB_CHECK_EQ(judged, 2u);

    // the same run again with the same tools measures nothing, and keeps the results
    const r::Result second = r::run({inputs}, o, s);
    CB_CHECK_EQ(measured, 2);
    std::size_t kept = 0;
    for (const r::Row& rw : second.rows) kept += rw.judged.has_value();
    CB_CHECK_EQ(kept, 2u);

    // --again measures them anyway; other tools measure them again too
    o.again = true;
    r::run({inputs}, o, s);
    CB_CHECK_EQ(measured, 4);
    o.again = false;
    s.tools_revision = [] { return std::string("def5678"); };
    r::run({inputs}, o, s);
    CB_CHECK_EQ(measured, 6);

    // a run without --judge only describes and files
    r::Options plain;
    plain.name = "plain";
    s.tools_revision = [] { return std::string("zzz"); };
    const r::Result described = r::run({inputs}, plain, s);
    CB_CHECK_EQ(measured, 6);
    CB_CHECK(std::none_of(described.rows.begin(), described.rows.end(), [](const r::Row& rw) { return rw.judged.has_value(); }));
    CB_CHECK(std::any_of(described.rows.begin(), described.rows.end(), [](const r::Row& rw) { return rw.picked; }));
});

CB_TEST("coverage.reach.results_of_earlier_runs_are_kept_and_damaged_ones_are_not_read", [] {
    fs::ScopedTempDir tmp("reach-results");
    r::Services s = services_in(tmp.path() / "state");
    write(tmp.path() / "one.nes", "NES-one");
    r::Options o;
    o.name = "mine";
    r::run({tmp.path() / "one.nes"}, o, s);
    write(tmp.path() / "two.nes", "NES-two");
    r::run({tmp.path() / "two.nes"}, o, s);   // a second run on other input: the first stays in the file
    const std::vector<r::Row> rows = r::read_results(tmp.path() / "state" / "reach" / "mine.json");
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK_EQ(fs::to_text(stdfs::path(rows[0].path).filename()), "two.nes");
    CB_CHECK_EQ(fs::to_text(stdfs::path(rows[1].path).filename()), "one.nes");

    write(tmp.path() / "state" / "reach" / "bad.json", "{not json");
    CB_CHECK(r::read_results(tmp.path() / "state" / "reach" / "bad.json").empty());
    CB_CHECK(r::read_results(tmp.path() / "state" / "reach" / "absent.json").empty());
    write(tmp.path() / "state" / "reach" / "odd.json", R"({"programs": [1, {"sha": "q", "size": "big"}, {"sha": "r", "size": 7}]})");
    const auto odd = r::read_results(tmp.path() / "state" / "reach" / "odd.json");
    CB_CHECK_EQ(odd.size(), 2u);   // a row that is not an object is not a row; a field of the wrong kind is its default
    CB_CHECK_EQ(odd[0].size, 0u);
    CB_CHECK_EQ(odd[1].size, 7u);
});

CB_TEST("coverage.reach.command_parses_its_options", [] {
    fs::ScopedTempDir tmp("reach-command");
    r::Services s = services_in(tmp.path() / "state");
    write(tmp.path() / "g.nes", "NES-g");
    cli::Cli cli("codebreaker", {r::command(s)});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"reach", (tmp.path() / "g.nes").string(), "--name", "cmd", "--per-platform", "2"}), 0);
        CB_CHECK(cap.out().find("# Reach") != std::string::npos);
        CB_CHECK(cap.out().find("| NES | 1 | nes | 0 |") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"reach", "--report", "cmd"}), 0);   // the run's report again, nothing run
        CB_CHECK(cap.out().find("| NES | 1 |") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_THROWS_AS(UsageError, cli.run({"reach"}));   // nothing to look at: a usage error (the program's main maps it to exit 2)
    }
});
