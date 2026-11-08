// codebreaker/formats/xbox/bink.hpp -- C++ from codebreaker.formats.xbox.bink.
//
// Bink video (.bik) -- the most common Xbox 360 cutscene/video format.
//
// `bink info` parses the container header natively (version, frame count,
// dimensions, fps, audio tracks) with no external tool. `bink decode` extracts and
// decodes via ffmpeg (found through `config`/PATH) -- the Bink1/Bink2 video and DCT
// audio codecs are RAD-proprietary and ffmpeg maintains the decoders, so the same
// driven-not-embedded policy as chdman/xma applies.
//
// Commands: info / decode.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::bink {

extern const py::str DESCRIPTION;

// (Python's tuples of the magics: lists here, used only for "in")
extern const py::list<py::bytes> _BINK1;
extern const py::list<py::bytes> _BINK2;

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

bool is_bink(const py::bytes& data);

// Parse a Bink header (little-endian). Returns a dict.
BinkHeader read_header(const py::str& path);

// Decode a .bik via ffmpeg. The output container/codec is chosen by the
// out_path extension (.wav/.mp4/.png-sequence/...). Returns output size.
int64_t decode(const py::str& bik_path, const py::str& out_path, bool audio_only = false, bool video_only = false);

// main(): Python reads sys.argv; here the caller hands sys.argv in (program name first). Returns the exit
// status: 0, or 2 where Python calls sys.exit(2). Exceptions propagate as Python's would.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::bink
