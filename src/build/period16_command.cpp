// build/period16_command.cpp -- the `build16` command over period16 (see period16.hpp).

#include <format>

#include "build/dos_tools.hpp"
#include "build/period16.hpp"
#include "exact/emu/native_dos_executive.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"

namespace cb::build::period16 {

namespace stdfs = std::filesystem;

namespace {

// A file named on the command line, else the corpus's retail binary of that name.
stdfs::path p16_resolve(const base::Corpus& corpus, const std::string& named) {
    if (const auto found = corpus.retail(named)) return *found;
    throw NotFoundError(fs::from_text(named), 2, "not a file, and the corpus has no such module");
}

}  // namespace

Tools tools_from(const config::Config& config, const base::Corpus& corpus) {
    Tools tools;
    tools.dosbox_x = config.path("dosbox_x").value_or(stdfs::path());
    tools.msc6 = config.path("msc6").value_or(stdfs::path());
    tools.msvc152 = config.path("msvc152").value_or(stdfs::path());
    tools.win31_kit = find_kit(config.checkout_root());
    if (!tools.win31_kit) tools.win31_kit = find_kit(config.main_checkout());   // a worktree keeps its toolchains in the main checkout
    tools.libentry_fallback = corpus.reference({"Microsoft Windows 95 DDK", "LIB"});
    return tools;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "build16";
    c.summary = "a 16-bit DLL built with Microsoft C 6 and LINK 5.60, compared by segment";
    c.area = "Build";
    c.params = {
        cli::Positional{"folder", "the source folder: the .c and .h files, the .def, and the .lnk response file"},
        cli::Option{"--original", "", "the original module to compare with (a file or a corpus name; default: the DLL's own name)", cli::Arg::Value, "MODULE"},
        cli::Option{"--work", "", "the folder to build in (default: a new one, kept; it keeps CL.TXT, LINK.TXT and RC.TXT)", cli::Arg::Value, "DIR"},
        cli::Option{"--stub", "", "put this module's DOS stub in front of the built DLL (a file or a corpus name)", cli::Arg::Value, "DONOR"},
    };
    c.handler = [environment](const cli::Args& a) -> int {
        std::shared_ptr<base::Corpus> corpus = environment.corpus;
        if (!corpus) corpus = std::make_shared<base::Corpus>(environment.config.products());
        const Tools tools = tools_from(environment.config, *corpus);
        // (CodeBreaker's own DOS executive runs the period tools; DOSBox-X only for a run it cannot complete)
        const exact::emu::NativeDosExecutive dos(environment.config);
        const std::unique_ptr<DosRunner> dos_runner = exact::emu::dos_runner(environment.config, dos);
        const DosRunner& runner = *dos_runner;
        std::optional<stdfs::path> donor;   // found before the build, which takes minutes
        if (const auto d = a.value("stub")) donor = p16_resolve(*corpus, *d);
        std::optional<stdfs::path> work;
        if (const auto w = a.value("work")) work = fs::from_text(*w);
        const auto dll = build(runner, tools, fs::from_text(a.required("folder")), work);
        if (!dll) {
            out("no DLL was made (see CL.TXT and LINK.TXT in the work folder)\n");
            return kExitFailed;
        }
        out("built: {}\n", fs::to_text(*dll));
        const std::string original_name = a.value("original").value_or(fs::to_text(dll->filename()));
        const auto original = corpus->retail(original_name);
        if (donor) fs::write_file(*dll, normalize(fs::read_file(*dll), fs::read_file(*donor)));
        if (original) {
            for (const std::string& line : compare(fs::read_file(*dll), fs::read_file(*original)).lines) out("{}\n", line);
        } else {
            out("no original named {} to compare with\n", original_name);
        }
        return kExitOk;
    };
    return c;
}

}  // namespace cb::build::period16
