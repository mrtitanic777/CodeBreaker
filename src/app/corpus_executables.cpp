#include "app/corpus_executables.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"

namespace cb::app::corpus_executables {

cli::Command command(const Environment& environment) {
    return cli::Command{
        "corpus-executables",
        "every executable of the selected corpus: its format, a tab, its path",
        {},
        [environment](const cli::Args&) {
            if (!environment.corpus) throw InternalError("corpus-executables: no corpus");
            std::vector<std::string> paths;
            // (the paths as Windows writes them: they sort and print with backslashes)
            for (const base::Corpus::Executable& e : environment.corpus->executables()) paths.push_back(std::filesystem::path(e.path).make_preferred().u8string());
            std::sort(paths.begin(), paths.end());
            paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
            const auto name_of = [](const std::string& path) { return text::lower(fs::to_text(fs::from_text(path).filename())); };
            std::stable_sort(paths.begin(), paths.end(), [&](const std::string& a, const std::string& b) { return name_of(a) < name_of(b); });
            for (const std::string& path : paths) {
                std::optional<std::string> format;
                try {
                    format = formats::detect::format_of(fs::from_text(path));
                } catch (const Error&) {
                }
                cb::out("{}\t{}\n", format.value_or("?"), path);
            }
            return 0;
        },
        {}, "", ""};
}

}  // namespace cb::app::corpus_executables
