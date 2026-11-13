// Tests of the `driver-services` command (cb/symbols/driver_services.hpp's `command`) against what `python -m codebreaker driver-services ...` printed
// (testdata/commands/*.txt, made by state/work/cmdlayer/gen_driver_services.sh): the size of the table, how it does on four real drivers, a file that is
// not there, and the table made again from the small driver kit in testdata/driver_kit (the Python's generate() into a file of its own over a corpus
// with no drivers: testdata/commands/generated.json).
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/value.hpp"
#include "cb/symbols/driver_services.hpp"

namespace {

namespace dscmd = cb::symbols::driver_services;

dscmd::Table command_real_table() {
    const cb::knowledge::Value doc =
        cb::knowledge::Value::from_json_text(cb::fs::read_text(cb::testing::data_path("symbols/testdata/driver_services.json")));
    const auto rows = [&](const char* kind) { return cb::knowledge::Rows(*doc.get(kind)->map()); };
    return dscmd::Table::from_rows(rows("devices"), rows("services"), rows("drivers"), rows("messages"));
}

struct DriverServicesCommandWorld {
    cb::fs::ScopedTempDir dir{"cb-driver-services"};

    dscmd::Environment environment() const {
        dscmd::Environment env;
        env.table = [] { return command_real_table(); };
        env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
            if (name == "bios.vxd") return cb::testing::data_path("formats/testdata/bios.vxd");
            if (name == "splitter.vxd") return cb::testing::data_path("formats/testdata/splitter.vxd");
            if (name == "srammtd.vxd") return cb::testing::data_path("formats/testdata/srammtd.vxd");
            if (name == "vmouse.vxd") return cb::testing::data_path("decompile/testdata/describe/vmouse.vxd");
            return std::nullopt;
        };
        env.corpus_drivers = [] { return std::vector<std::filesystem::path>{}; };
        env.table_file = dir.path() / "driver_services.json";
        env.kit_folder = cb::fs::to_text(cb::testing::data_path("symbols/testdata/driver_kit"));
        env.reference_folder = dir.path() / "no-reference";
        return env;
    }

    // What the command prints, then `exit=N` as the golden files have it.
    std::string run(std::vector<std::string> args) const {
        cb::cli::Cli cli("codebreaker", {dscmd::command(environment())});
        cb::testing::CapturedStd cap;
        args.insert(args.begin(), "driver-services");
        const int code = cli.run(args);
        return cap.out() + "exit=" + std::to_string(code) + "\n";
    }

    static std::string golden(const char* name) {
        return cb::fs::read_text(cb::testing::data_path(std::string("symbols/testdata/commands/") + name));
    }
};

}  // namespace

CB_TEST("symbols.driver_services_command.the_size_and_the_coverage_match_the_python", [] {
    DriverServicesCommandWorld w;
    CB_CHECK_EQ(w.run({}), DriverServicesCommandWorld::golden("size.txt"));
    CB_CHECK_EQ(w.run({"bios.vxd", "splitter.vxd", "srammtd.vxd", "vmouse.vxd", "nosuch.vxd"}), DriverServicesCommandWorld::golden("files.txt"));
});

CB_TEST("symbols.driver_services_command.the_table_is_made_again_from_the_kit", [] {
    DriverServicesCommandWorld w;
    const std::string said = w.run({"--generate", cb::fs::to_text(cb::testing::data_path("symbols/testdata/driver_kit"))});
    CB_CHECK_EQ(said, cb::fs::to_text(w.dir.path() / "driver_services.json") + ": 38 service tables, 1147 services, 134 device numbers\nexit=0\n");
    CB_CHECK_EQ(cb::fs::read_text(w.dir.path() / "driver_services.json"), DriverServicesCommandWorld::golden("generated.json"));
    // the kit folder comes from the environment when none is named
    std::filesystem::remove(w.dir.path() / "driver_services.json");
    CB_CHECK(w.run({"--generate"}).ends_with("exit=0\n"));
    CB_CHECK_EQ(cb::fs::read_text(w.dir.path() / "driver_services.json"), DriverServicesCommandWorld::golden("generated.json"));
});

CB_TEST("symbols.driver_services_command.no_include_files_is_a_failure", [] {
    DriverServicesCommandWorld w;
    dscmd::Environment env = w.environment();
    env.kit_folder.clear();
    cb::cli::Cli cli("codebreaker", {dscmd::command(env)});
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(cli.run({"driver-services", "--generate"}), cb::kExitFailed);
    CB_CHECK(cap.err().find("include files were not found") != std::string::npos);
    CB_CHECK(!std::filesystem::exists(w.dir.path() / "driver_services.json"));
});
