#include "cb/formats/xbox/disc_command.hpp"

#include <algorithm>
#include <fstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::disc_command {

namespace stdfs = std::filesystem;

namespace {

constexpr int kExitNoSuchFile = 3;

}  // namespace

Image open_image(const stdfs::path& path) {
    std::error_code ec;
    if (stdfs::is_directory(path, ec)) {
        std::shared_ptr<Device> device = SvodDevice::open(path);
        return Image{device, gdf::Disc::open(device, 0)};
    }
    if (!stdfs::exists(path, ec)) throw NotFoundError(path, 0, "the image doesn't exist");
    std::shared_ptr<Device> device = std::make_shared<FileDevice>(path);
    return Image{device, gdf::Disc::open(device)};
}

void write_whole(const stdfs::path& path, ByteView data) {
    if (path.has_parent_path()) fs::make_dirs(path.parent_path());
    stdfs::path part = path;
    part += ".part";
    fs::write_file(part, data);
    stdfs::rename(part, path);
}

std::uint64_t unpack(Image& image, const stdfs::path& folder, const std::function<void(std::uint64_t, std::uint64_t)>& progress) {
    const std::vector<gdf::Entry> entries = image.disc.entries();
    std::uint64_t total = 0;
    for (const gdf::Entry& e : entries)
        if (!e.is_dir) total += e.size;
    const std::uint64_t shown_total = std::max<std::uint64_t>(total, 1);
    std::uint64_t done = 0;
    std::int64_t last_percent = -1;
    for (const gdf::Entry& e : entries) {
        const stdfs::path out = folder / fs::from_text(e.path);
        if (e.is_dir) {
            fs::make_dirs(out);
            continue;
        }
        fs::make_dirs(out.parent_path());
        std::error_code ec;
        if (!(stdfs::is_regular_file(out, ec) && stdfs::file_size(out, ec) == e.size)) {
            stdfs::path part = out;
            part += ".part";
            {
                std::ofstream writer(part, std::ios::binary | std::ios::trunc);
                if (!writer) throw IoError(part, 0, "cannot write");
                for (std::uint64_t at = 0; at < e.size;) {
                    const std::size_t piece = static_cast<std::size_t>(std::min<std::uint64_t>(kPiece, e.size - at));
                    const Bytes data = image.device->read_at(static_cast<std::uint64_t>(image.disc.base()) + std::uint64_t{e.start} * gdf::kSector + at, piece);
                    if (data.size() != piece) throw TruncatedError("xbox-disc", at, piece, data.size());
                    writer.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
                    at += piece;
                    const std::int64_t percent = static_cast<std::int64_t>((done + at) * 100 / shown_total);
                    if (percent != last_percent) {
                        last_percent = percent;
                        progress(done + at, shown_total);
                    }
                }
                if (!writer) throw IoError(part, 0, "cannot write");
            }
            stdfs::rename(part, out);
        }
        done += e.size;
    }
    progress(total, shown_total);
    return total;
}

cli::Command command() {
    cli::Command files{
        "files",
        "every file of the image: its size, a tab, its path",
        {cli::Positional{"image", "a disc image or a Games on Demand data folder"}},
        [](const cli::Args& args) {
            Image image = open_image(fs::from_text(args.required("image")));
            for (const gdf::Entry& e : image.disc.files()) cb::out("{}\t{}\n", e.size, e.path);
            return 0;
        },
        {}, "", ""};
    cli::Command read{
        "read",
        "one file of the image, written to a file (exit 3 when the image has none)",
        {cli::Positional{"image", "a disc image or a Games on Demand data folder"}, cli::Positional{"path", "the file's path on the disc"},
         cli::Positional{"out", "the file to write"}},
        [](const cli::Args& args) {
            Image image = open_image(fs::from_text(args.required("image")));
            const std::optional<Bytes> data = image.disc.try_read(args.required("path"));
            if (!data) {
                cb::err("{} has no {}\n", args.required("image"), args.required("path"));
                return kExitNoSuchFile;
            }
            write_whole(fs::from_text(args.required("out")), *data);
            return 0;
        },
        {}, "", ""};
    cli::Command unpack_command{
        "unpack",
        "every file of the image, written under a folder (what is there at its full size is kept)",
        {cli::Positional{"image", "a disc image or a Games on Demand data folder"}, cli::Positional{"folder", "the folder to write the files under"}},
        [](const cli::Args& args) {
            Image image = open_image(fs::from_text(args.required("image")));
            unpack(image, fs::from_text(args.required("folder")),
                   [](std::uint64_t done, std::uint64_t total) { cb::out("PROGRESS {} {}\n", done, total); });
            return 0;
        },
        {}, "", ""};
    return cli::Command{"xbox-disc", "the files of an Xbox 360 disc image: list, read one, unpack all", {}, {},
                        {std::move(files), std::move(read), std::move(unpack_command)}, "Xbox 360", ""};
}

}  // namespace cb::formats::xbox::disc_command
