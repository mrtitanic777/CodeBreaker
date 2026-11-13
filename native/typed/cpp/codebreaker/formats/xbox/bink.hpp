// codebreaker/formats/xbox/bink.hpp -- compat shim over cb/formats/xbox/bink.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/bink.{hpp,cpp}; this header keeps the old py:: surface for the ports that have not been
// refactored yet (formats/xbox/package) and is deleted with the last of them. Conversion and forwarding only; `main` is the old command-line
// wrapper over the new command, and the program decode drives (settings.tool("ffmpeg")) is found through the old layer, as before.
#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/bink.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::bink {

inline const py::str DESCRIPTION = py::str("Xbox 360 Bink video (.bik): native header info + ffmpeg-driven decode/extract");

// The dict read_header returns: a record of fixed keys, in the dict's order
struct BinkHeader {
    int64_t generation = 0;
    py::str codec;
    int64_t file_size = 0;
    int64_t frames = 0;
    int64_t largest_frame = 0;
    int64_t width = 0;
    int64_t height = 0;
    double fps = 0.0;
    py::str video_flags;
    int64_t audio_tracks = 0;
};
template <class F> void for_each_field_mut(BinkHeader& h, F&& f) {
    f("generation", h.generation, true);
    f("codec", h.codec, true);
    f("file_size", h.file_size, true);
    f("frames", h.frames, true);
    f("largest_frame", h.largest_frame, true);
    f("width", h.width, true);
    f("height", h.height, true);
    f("fps", h.fps, true);
    f("video_flags", h.video_flags, true);
    f("audio_tracks", h.audio_tracks, true);
}

inline bool is_bink(const py::bytes& data) {
    const cb::Bytes b = cb::compat::from_py(data);
    return cb::formats::xbox::bink::is_bink(cb::ByteView(b).first(std::min<std::size_t>(b.size(), 4)));
}

// Parse a Bink header (little-endian).
inline BinkHeader read_header(const py::str& path) {
    namespace nb = cb::formats::xbox::bink;
    const nb::Header n = cb::compat::guard([&] { return nb::read_header_file(cb::fs::from_text(cb::compat::from_py(path))); });
    BinkHeader h;
    h.generation = n.generation;
    h.codec = cb::compat::to_py(n.codec);
    h.file_size = cb::compat::to_py(n.file_size);
    h.frames = n.frames;
    h.largest_frame = n.largest_frame;
    h.width = n.width;
    h.height = n.height;
    h.fps = n.fps;
    h.video_flags = cb::compat::to_py(std::format("0x{:08X}", n.video_flags));
    h.audio_tracks = n.audio_tracks;
    return h;
}

namespace shim_ {
// settings.tool(key): the program settings.json names, else one on PATH. -> its path, nullopt when there is none.
inline std::optional<std::filesystem::path> tool_(const char* key) {
    const py::str named = py::settings::get_str(py::str(key));
    if (!named.empty() && (py::os::path::exists(named) || py::shutil::which(named))) return cb::fs::from_text(named.utf8());
    if (const auto found = py::shutil::which(py::str(key))) return cb::fs::from_text(found->utf8());
    return std::nullopt;
}
}  // namespace shim_

// Decode a .bik via ffmpeg; the output container/codec is chosen by the extension of out_path. Returns output size.
inline int64_t decode(const py::str& bik_path, const py::str& out_path, bool audio_only = false, bool video_only = false) {
    namespace nb = cb::formats::xbox::bink;
    return cb::compat::to_py(cb::compat::guard([&] {
        return nb::decode(cb::fs::from_text(cb::compat::from_py(bik_path)), cb::fs::from_text(cb::compat::from_py(out_path)), shim_::tool_("ffmpeg"),
                          audio_only, video_only);
    }));
}

// main(): the caller hands sys.argv in (program name first). Returns the exit status: 0, or 2 where the usage was wrong.
inline int64_t main(const py::list<py::str>& argv) {
    namespace nb = cb::formats::xbox::bink;
    std::vector<std::string> args = {"bink"};
    const std::vector<std::string> all = cb::compat::from_py(argv);
    args.insert(args.end(), all.begin() + (all.empty() ? 0 : 1), all.end());
    return cb::compat::guard([&] { return cb::cli::Cli("bink", {nb::command(nb::Environment{shim_::tool_("ffmpeg")})}).run(args); });
}

}  // namespace codebreaker::formats::xbox::bink
