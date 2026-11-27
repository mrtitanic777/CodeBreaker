// `score16 <16-bit module> [--segment N] [--json FILE]`: where each 16-bit function differs from the original, and whose work that difference is.
//
// The Microsoft C 8 half is `match <module>`'s (the C as the decompiler wrote it, not folded back: the C the build has); this adds OpenWatcom's half
// -- every source file compiled again by OpenWatcom -zmsc, with the same switches -- and the rows and the report (verify/gate16/scoreboard16).
#include "base/watcom.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "source/pipeline_match16.hpp"
#include "verify/gate16/scoreboard16.hpp"

namespace cb::source::pipeline {

namespace {

namespace sb16 = verify::gate16::scoreboard16;

int s16_show(const sb16::Arguments& parsed) {
    const sb16::Rows rows = sb16::rows_from_json(fs::read_text(fs::from_text(parsed.json)));
    for (const std::string& name : parsed.show) {
        const sb16::Row* row = rows.find(name);
        if (row == nullptr) throw UsageError(name + ": not on the board " + parsed.json);
        cb::out("===== {}\n", name);
        for (const std::string& line : sb16::show_lines(*row)) cb::out("{}\n", line);
    }
    return 0;
}

int s16_command(World& world, const cli::Args& args) {
    const std::optional<sb16::Arguments> parsed = sb16::parse_arguments(args.values("args"));
    if (!parsed) {
        cb::out("{}\n", sb16::kUsage);
        return kExitUsage;
    }
    if (!parsed->show.empty()) return s16_show(*parsed);

    const std::optional<std::filesystem::path> path = world.find(parsed->module);
    if (!path) throw UsageError(parsed->module + ": a 16-bit Windows module is needed");
    if (world.corpus().lane(*path) != base::Lane::Ne) throw UsageError(parsed->module + ": a 16-bit Windows module is needed");
    const base::watcom::OpenWatcom watcom = base::watcom::OpenWatcom::from_settings(world.config().text("openwatcom"));
    if (watcom.cc(base::watcom::Target::Win16).empty()) throw UsageError("OpenWatcom is not set up (settings.json: openwatcom): its 16-bit compiler is needed");

    const auto log = [](std::string_view line) { cb::out("{}\n", line); };
    Match16Options options;
    options.fold = false;   // (the C as the decompiler wrote it, as the build has it)
    const Match16Run run = run_match16(world, *path, options, log);

    sb16::Inputs in;
    in.functions = run.functions;
    in.result = &run.result;
    in.texts = &run.result.texts;
    in.header = run.header;
    in.watcom = &watcom;
    in.toolchain = run.toolchain.get();
    in.segment = parsed->segment;
    in.log = log;
    const sb16::Rows rows = sb16::run(in);
    for (const std::string& line : sb16::report_lines(rows)) cb::out("{}\n", line);
    if (!parsed->json.empty()) fs::write_text(fs::from_text(parsed->json), sb16::rows_to_json(rows));
    return 0;
}

}  // namespace

cli::Command score16(const Environment& environment) {
    cli::Command declared{"score16", "where each 16-bit function differs from the original, and whose work that is",
                          {cli::Positional{"args", "<16-bit module> [--segment N] [--json FILE] | --show FUNCTION --json FILE", false, true}}, {}, {}, "Reconstruct",
                          std::string(sb16::kUsage)};
    declared.passthrough = true;
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) { return s16_command(world, args); });
}

}  // namespace cb::source::pipeline
