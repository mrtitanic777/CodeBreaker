// Tests of the driver lane, on three real Windows 95 drivers (BIOS, SPLITTER, SRAMMTD: formats/testdata) against what the live Python's
// verify/lanes/driver.py answered (testdata/driver_lane.golden.json, made by testdata/gen_driver_lane.py): the inputs of each entry point, a
// fingerprint of one input of each, the verdict of every entry point against itself, and -- for every equivalent one -- the verdicts of copies
// with one executed instruction broken (which byte of the file, which instruction, what the judge said and why); plus the command, the kind
// interface (prepare, build) and the profile on stand-in tools.
//
// DIVERGES from verify/lanes/driver.py:
//   * the profile and the digest of what a run left in memory are made over our own bytes, not the SHA-1 of Python's repr of a tuple (a verdict store
//     the Python wrote is rebuilt, hence REV 5); a tool that cannot be found is the same as any other in the profile;
//   * a driver that cannot be read is an `inconclusive` verdict "judge: <error kind>" where the Python's text named the exception and its message;
//   * the path explorer is on only when the lane is given the p-code lifters; the golden was made where the Python had none either (concolic_ready
//     false), so no `paths=N` limit is compared;
//   * `driver-judge` says "not found" with exit code 3 (the Python: 2); the services' placeholder, the settings and the paths come from the lane's spec.
#include "cb/verify/lanes/driver.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/package.hpp"

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::lanes;
namespace sfs = std::filesystem;
using nlohmann::ordered_json;

namespace {

const ordered_json& drv_golden() {
    static const ordered_json g = json::load(testing::data_path("verify/lanes/testdata/driver_lane.golden.json"));
    return g;
}

sfs::path drv_vxd(const std::string& name) { return testing::data_path("formats/testdata/" + name); }

struct DriverWorld {
    fs::ScopedTempDir dir{"cb-driver-lane"};
    knowledge::Store store{dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path())};
    knowledge::Knowledge kb{store};
    symbols::driver_services::Table table = [] {
        const knowledge::Value doc = knowledge::Value::from_json_text(fs::read_text(testing::data_path("symbols/testdata/driver_services.json")));
        auto rows = [&](const char* kind) { return knowledge::Rows(*doc.get(kind)->map()); };
        return symbols::driver_services::Table::from_rows(rows("devices"), rows("services"), rows("drivers"), rows("messages"));
    }();

    DriverLaneSpec spec() {
        DriverLaneSpec s;
        s.services = &table;
        s.resolver = emulate::driver::resolver_of([this](std::uint32_t device, std::uint32_t service) { return table.service_name(device, service); },
                                                  [this](std::string_view name) -> std::optional<std::string> {
                                                      const std::optional<knowledge::Value> model = kb.api_model("vxd", name);
                                                      if (!model) return std::nullopt;
                                                      const knowledge::Value* where = model->get("answers_in");
                                                      return where && where->text() ? std::optional<std::string>(*where->text()) : std::nullopt;
                                                  });
        s.state = dir.path() / "state";
        s.retail = [](std::string_view name) -> std::optional<sfs::path> {
            const sfs::path p = drv_vxd(std::string(name));
            std::error_code ec;
            return sfs::is_regular_file(p, ec) ? std::optional<sfs::path>(p) : std::nullopt;
        };
        s.segment_classes = "LCODE:preload";
        return s;
    }
};

std::string drv_limits(const std::vector<std::string>& limits) { return text::join(limits, ","); }

void drv_check_judgement(const std::string& what, const Judgement& got, const ordered_json& want_verdict, const ordered_json& want_reason,
                         const ordered_json& want_limits, const ordered_json& want_detail) {
    testing::count_check();
    const std::string verdict(word(got.verdict));
    if (verdict != want_verdict.get<std::string>() || got.reason != want_reason.get<std::string>() ||
        drv_limits(got.limits) != drv_limits(want_limits.get<std::vector<std::string>>()) || got.detail != want_detail.get<std::string>()) {
        testing::record_failure(__FILE__, __LINE__,
                                std::format("{}: the judge said {} / {} / [{}] / {} but the Python said {} / {} / [{}] / {}", what, verdict, got.reason,
                                            drv_limits(got.limits), got.detail, want_verdict.get<std::string>(), want_reason.get<std::string>(),
                                            drv_limits(want_limits.get<std::vector<std::string>>()), want_detail.get<std::string>()));
    }
}

}  // namespace

CB_TEST("verify.driver.the_inputs_of_each_entry_point_are_the_pythons", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    for (const auto& [file, rec] : drv_golden().at("files").items()) {
        for (const auto& [label, counts] : rec.at("inputs").items()) {
            const DriverInputs in = lane.inputs(label);
            CB_CHECK_EQ(in.first.size(), counts.at(0).get<std::size_t>());
            CB_CHECK_EQ(in.more.size(), counts.at(1).get<std::size_t>());
        }
    }
    // the shape of a vector: eight registers' worth of arguments and the stack words, the carry set on entry, ebp at the client
    const DriverVector v = driver_vector({.eax = 5, .stack = {7, 8}});
    CB_CHECK_EQ(v.args.size(), std::size_t(8));
    CB_CHECK_EQ(v.args[0], 5);
    CB_CHECK_EQ(v.args[6], 7);
    CB_CHECK_EQ(v.flags, 1u);
    CB_CHECK_EQ(v.reg(DriverRegister::Ebp), emulate::driver::kClient);
    CB_CHECK_EQ(v.memory.size(), std::size_t(5));
    CB_CHECK_EQ(v.memory[0].data.size(), std::size_t(0x100));
    CB_CHECK_EQ(driver_memory(0x12345678)[0].data[0], 0x78);
    CB_CHECK_EQ(driver_key(2, 0x1a0), "2:1a0");
    CB_CHECK(driver_entry_before("control", "service_0"));
    CB_CHECK(driver_entry_before("service_9", "pm_api"));
    CB_CHECK(driver_entry_before("service_9", "service_10"));
    CB_CHECK(!driver_entry_before("v86_api", "service_0"));
});

CB_TEST("verify.driver.a_fingerprint_is_what_the_python_recorded", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    for (const auto& [file, rec] : drv_golden().at("files").items()) {
        const auto image = lane.image(drv_vxd(file));
        for (const auto& [label, want] : rec.at("first").items()) {
            const DriverInputs in = lane.inputs(label);
            const DriverVector& v = in.first[std::min<std::size_t>(1, in.first.size() - 1)];
            const auto [fp, detail] = lane.run_detailed(*image, *image->entries.find(label), v, true);
            const std::string name = file + " " + label;
            CB_CHECK(!fp.err);
            CB_CHECK_EQ(fp.exit, want.at("exit").get<std::string>());
            CB_CHECK_EQ(fp.ret.value_or(-1), want.at("ret").get<std::int64_t>());
            CB_CHECK(fp.deps == want.at("deps").get<std::vector<std::string>>());
            CB_CHECK_EQ(fp.pop.value_or(-1), want.at("pop").is_null() ? -1 : want.at("pop").get<std::int64_t>());
            CB_CHECK_EQ(fp.cov.size(), want.at("cov").get<std::size_t>());
            CB_CHECK_EQ(detail.memory ? static_cast<std::int64_t>(detail.memory->first) : -1, want.at("memory").is_null() ? -1 : want.at("memory").get<std::int64_t>());
            // (the flags, then the registers it hands back as the comparer sees them)
            CB_CHECK(detail.result == want.at("result").get<std::vector<std::string>>());
            // the same run twice is the same fingerprint, from the lane's own entry point too
            CB_CHECK(lane.fingerprint(*image, *image->entries.find(label), v) == fp);
        }
    }
});

CB_TEST("verify.driver.each_entry_point_judged_against_itself_is_the_pythons_verdict", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    for (const auto& [file, rec] : drv_golden().at("files").items()) {
        const sfs::path path = drv_vxd(file);
        const auto judged = lane.judge_files(path, path);
        CB_CHECK_EQ(judged.size(), rec.at("self").size());
        std::vector<std::string> order;   // entry order: control, the services, then the others, the shorter labels first
        for (const auto& [label, j] : judged) order.push_back(label);
        CB_CHECK_EQ(order.front(), "control");
        for (const auto& [label, j] : judged) {
            const ordered_json& want = rec.at("self").at(label);
            drv_check_judgement(file + " " + label + " against itself", j, want.at(0), want.at(1), want.at(2), want.at(3));
        }
    }
});

CB_TEST("verify.driver.a_copy_with_one_executed_instruction_broken_is_judged_as_the_python_judged_it", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    fs::ScopedTempDir out("cb-driver-lane-breaks");
    std::size_t caught = 0, missed = 0;
    for (const auto& [file, rec] : drv_golden().at("files").items()) {
        const sfs::path path = drv_vxd(file);
        for (const auto& [key, want] : rec.at("broken").items()) {
            const std::size_t space = key.rfind(' ');
            const std::string label = key.substr(0, space);
            const int nth = std::stoi(key.substr(space + 1));
            const sfs::path copy = out.path() / std::format("{}_{}_{}.vxd", file, label, nth);
            const auto changed = lane.break_instruction(path, copy, label, nth);
            CB_CHECK(changed.has_value());
            if (!changed) continue;
            CB_CHECK_EQ(changed->offset, want.at(0).get<std::uint64_t>());   // the same byte of the file
            CB_CHECK_EQ(changed->instruction, want.at(1).get<std::string>());   // the same instruction
            const Judgement j = lane.judge_entry(path, copy, label);
            drv_check_judgement(file + " " + key + " broken", j, want.at(2), want.at(3), want.at(4), want.at(5));
            (j.verdict == Verdict::Divergent ? caught : missed) += 1;
        }
    }
    CB_CHECK(caught >= 30);   // the judge has teeth
    CB_CHECK(missed >= 1);    // ... and the golden holds a break no input reaches the effect of
});

CB_TEST("verify.driver.what_cannot_be_judged_is_said", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    const sfs::path small = drv_vxd("srammtd.vxd"), big = drv_vxd("bios.vxd");
    // an entry point the original has not
    const Judgement none = lane.judge_entry(small, small, "service_77");
    CB_CHECK(none.verdict == Verdict::Inconclusive);
    CB_CHECK_EQ(none.reason, "the original has no such entry point");
    // one the rebuilt driver has not: a divergence that names it
    const Judgement missing = lane.judge_entry(big, small, "service_3");
    CB_CHECK(missing.verdict == Verdict::Divergent);
    CB_CHECK_EQ(missing.reason, "entry-missing");
    CB_CHECK_EQ(missing.detail, "the rebuilt driver has no service_3");
    // a file that is not a driver: the one entry point does not sink the rest
    fs::ScopedTempDir dir("cb-driver-lane-bad");
    const sfs::path junk = dir.path() / "junk.vxd";
    fs::write_text(junk, "this is not an LE image");
    const Judgement bad = lane.judge_entry(small, junk, "control");
    CB_CHECK(bad.verdict == Verdict::Inconclusive);
    CB_CHECK(bad.reason.starts_with("judge: "));
    // a file that is not there is a broken setup, not an observation
    CB_CHECK_THROWS_AS(NotFoundError, (void)lane.judge_entry(small, dir.path() / "absent.vxd", "control"));
    // the lane needs the names of the services
    DriverLaneSpec none_given;
    CB_CHECK_THROWS_AS(UsageError, (void)DriverLane(none_given));
    // break_instruction: an entry point that is not there, or a break past the last instruction that runs
    CB_CHECK(!lane.break_instruction(small, dir.path() / "b.vxd", "nope", 0).has_value());
    CB_CHECK(!lane.break_instruction(small, dir.path() / "b.vxd", "control", 1000).has_value());
});

CB_TEST("verify.driver.the_judge_is_judged", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    const DriverSelfCheck r = lane.self_check(drv_vxd("splitter.vxd"), std::nullopt, 3, 12);
    CB_CHECK_EQ(r.self.size(), std::size_t(1));
    CB_CHECK(r.self[0].first == Verdict::Equivalent);
    CB_CHECK_EQ(r.self[0].second, std::size_t(6));
    const auto& want = drv_golden().at("files").at("splitter.vxd").at("broken");
    CB_CHECK_EQ(r.broken, want.size());   // three breaks of each of the six entry points
    std::size_t caught = 0;
    for (const auto& [key, v] : want.items()) caught += v.at(2).get<std::string>() == "divergent" ? 1 : 0;
    CB_CHECK_EQ(r.caught, caught);
    CB_CHECK_EQ(r.missed.size(), r.broken - r.caught);
    // fewer entries judged when asked
    CB_CHECK_EQ(lane.self_check(drv_vxd("splitter.vxd"), std::nullopt, 1, 2).broken, std::size_t(2));
});

CB_TEST("verify.driver.the_command_prints_a_line_for_each_entry_point", [] {
    DriverWorld w;
    const DriverLane lane(w.spec());
    const cli::Command command = lane.command();
    CB_CHECK_EQ(command.name, "driver-judge");
    {
        testing::CapturedStd cap;
        const cli::Cli cli("codebreaker", {lane.command()});
        CB_CHECK_EQ(cli.run({"driver-judge", fs::to_text(drv_vxd("splitter.vxd")), "splitter.vxd"}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.starts_with("control      equivalent   "));
        CB_CHECK(out.find("13 instructions of the original ran") != std::string::npos);
        CB_CHECK(out.ends_with("splitter.vxd vs splitter.vxd: equivalent 6\n"));
    }
    {
        testing::CapturedStd cap;
        const cli::Cli cli("codebreaker", {lane.command()});
        CB_CHECK_EQ(cli.run({"driver-judge", "--self-check", "srammtd.vxd"}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.starts_with("srammtd.vxd against itself: equivalent 1\n"));
        CB_CHECK(out.find("one instruction broken: 3 copies, 0 reported divergent\n") != std::string::npos);
        CB_CHECK(out.find("    not caught: control with `") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        const cli::Cli cli("codebreaker", {lane.command()});
        CB_CHECK_EQ(cli.run({"driver-judge", "nowhere.vxd", "splitter.vxd"}), 3);
        CB_CHECK_EQ(cap.out(), "nowhere.vxd: not found\n");
    }
});

CB_TEST("verify.driver.prepare_and_build_make_the_entry_points_the_units_judged", [] {
    DriverWorld w;
    DriverLaneSpec spec = w.spec();
    int written = 0;
    spec.writer = [&](const sfs::path&, const sfs::path& out_dir) {
        ++written;
        Written wr;
        wr.source = out_dir / "bios.asm";
        fs::write_text(wr.source, "; the driver as assembly\n");
        wr.linked = true;
        wr.built = drv_vxd("bios.vxd");
        return wr;
    };
    const DriverLane lane(spec);
    CB_CHECK_EQ(lane.name(), "le");
    fs::ScopedTempDir work("cb-driver-lane-prepare");
    const std::unique_ptr<Prepared> prepared = lane.prepare(drv_vxd("bios.vxd"), work.path());
    CB_CHECK_EQ(written, 1);
    const auto& m = static_cast<const DriverLane::PreparedDriver&>(*prepared);
    CB_CHECK_EQ(m.header, "; the driver as assembly\n");
    // the entry points in the order they are judged, each procedure once (v86_api and pm_api are one: the shorter label stands)
    CB_CHECK_EQ(text::join(m.order, " "), "control service_0 service_1 service_2 service_3 pm_api");
    CB_CHECK_EQ(m.texts.at("service_1"), "; entry point service_1 of bios.asm");
    CB_CHECK(m.key.at("control").find(':') != std::string::npos);
    CB_CHECK_EQ(m.all_keys.size(), m.order.size());
    CB_CHECK(std::is_sorted(m.all_keys.begin(), m.all_keys.end()));
    CB_CHECK_EQ(m.sig.size(), std::size_t(40));
    // built: every entry point can be judged, and the judge is the lane's
    const Built built = lane.build(*prepared, drv_vxd("bios.vxd"), work.path());
    CB_CHECK_EQ(built.items.size(), m.order.size());
    CB_CHECK(built.missing.empty());
    const Judgement j = built.judge("service_0");
    CB_CHECK(j.verdict == Verdict::Equivalent);
    // a driver that did not build leaves every entry point out, with the build's last note
    spec.writer = [](const sfs::path&, const sfs::path&) {
        Written wr;
        wr.linked = false;
        wr.notes = {"assembling", "wlink: error: undefined symbol"};
        return wr;
    };
    const DriverLane failing(spec);
    const auto p2 = failing.prepare(drv_vxd("bios.vxd"), work.path());
    const Built none = failing.build(*p2, drv_vxd("bios.vxd"), work.path());
    CB_CHECK(none.items.empty());
    CB_CHECK_EQ(none.missing.size(), m.order.size());
    CB_CHECK_EQ(none.missing.at("control"), "wlink: error: undefined symbol");
    // no writer: the lane cannot say where the source is
    DriverLaneSpec bare = w.spec();
    CB_CHECK_THROWS_AS(UsageError, (void)DriverLane(bare).prepare(drv_vxd("bios.vxd"), work.path()));
});

CB_TEST("verify.driver.the_profile_depends_on_what_a_verdict_depends_on", [] {
    DriverWorld w;
    DriverLaneSpec a = w.spec();
    const Profile p1 = DriverLane(a).profile({}), p2 = DriverLane(a).profile({});
    CB_CHECK_EQ(p1.digest, p2.digest);
    CB_CHECK_EQ(p1.digest.size(), std::size_t(12));
    CB_CHECK(p1.settings.find("kind=le") != std::string::npos);
    CB_CHECK(p1.settings.find(std::format("judge={}", DriverLane::kRev)) != std::string::npos);
    DriverLaneSpec b = w.spec();
    b.segment_classes = "LCODE:preload MCODE:preload iopl";
    CB_CHECK(DriverLane(b).profile({}).digest != p1.digest);
    // a tool replaced is another verdict
    fs::ScopedTempDir tools("cb-driver-lane-tools");
    fs::write_text(tools.path() / "wasm.exe", "one");
    DriverLaneSpec c = w.spec();
    c.tools = [&] { return std::map<std::string, sfs::path>{{"wasm", tools.path() / "wasm.exe"}}; };
    const std::string before = DriverLane(c).profile({}).digest;
    fs::write_text(tools.path() / "wasm.exe", "another one, longer");
    CB_CHECK(DriverLane(c).profile({}).digest != before);
    // no lifters: the path explorer is off
    CB_CHECK(!DriverLane(a).concolic_ready());
});
