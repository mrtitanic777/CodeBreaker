// source/dos_build.cpp -- `dos-build` (see dos_build.hpp).
#include "source/dos_build.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"

namespace cb::source::dos_build {

cli::Command command(const dos_writer::Options& options) {
    return cli::Command{
        "dos-build",
        "a real-mode program made from the folder dos-source wrote, without the original",
        {cli::Positional{"folder", "the folder dos-source wrote"},
         cli::Option{"--against", "", "the original program: the file made is compared with it byte for byte", cli::Arg::Value, "ORIGINAL"}},
        [options](const cli::Args& a) -> int {
            std::filesystem::path made;
            try {
                made = dos_writer::rebuild(fs::from_text(a.required("folder")), options);
            } catch (const ToolError& e) {   // the tool's own words say why it refused: they go with the error
                if (!e.output().empty()) err("{}\n", e.output());
                throw;
            }
            const auto against = a.value("against");
            if (!against) {
                out("{}\n", fs::to_text(made));
                return kExitOk;
            }
            const bool same = fs::read_file(made) == fs::read_file(fs::from_text(*against));
            out("{}: {}\n", fs::to_text(made), same ? "THE SAME as " + *against : "differs from " + *against);
            return same ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
}

}  // namespace cb::source::dos_build
