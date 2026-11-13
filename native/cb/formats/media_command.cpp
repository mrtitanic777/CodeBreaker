// The `media` command: list or extract the files of a floppy, hard-disk or CD image, a module bundle or an Xbox container.
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/media.hpp"

namespace cb::formats::media {

cli::Command command(const Environment&) {
    const cli::Option plain{"--plain", "", "show the 8.3 / primary names", cli::Arg::Flag};
    cli::Command list{"list",
                      "list the volumes of an image and their files",
                      {cli::Positional{"image", "a floppy, hard-disk or CD image, a module bundle or an Xbox container"},
                       cli::Option{"--boot", "", "also print each volume's boot sector fields", cli::Arg::Flag}, plain},
                      [](const cli::Args& a) {
                          for (const auto& volume : volumes(fs::from_text(a.required("image")), !a.flag("plain")))
                              out_text(listing_text(*volume, !a.flag("plain"), a.flag("boot")));
                          return kExitOk;
                      },
                      {},
                      {},
                      {}};
    cli::Command extract{"extract",
                         "write the files of an image under a folder",
                         {cli::Positional{"image", "the image"}, cli::Positional{"folder", "where to write"},
                          cli::Positional{"name", "only files whose name or path matches (wildcards allowed)", false, true}, plain},
                         [](const cli::Args& a) {
                             const std::string folder = a.required("folder");
                             const std::vector<std::string> names = a.values("name");
                             const std::size_t n = media::extract(fs::from_text(a.required("image")), fs::from_text(folder), names, !a.flag("plain"));
                             out("{} files written to {}\n", n, folder);
                             return kExitOk;
                         },
                         {},
                         {},
                         {}};
    return cli::Command{"media", "list or extract the files of a floppy, hard-disk or CD image or a Xbox container", {}, {}, {list, extract}, "Formats", ""};
}

}  // namespace cb::formats::media
