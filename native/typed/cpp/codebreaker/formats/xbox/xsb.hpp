// codebreaker/formats/xbox/xsb.hpp -- typed C++ from codebreaker.formats.xbox.xsb.
//
// xsb.py -- an XACT sound bank (.xsb): every cue, and the waves it plays.
//
//     Bank bank = xsb::read(path);   {"wave_banks": [name], "cues": {cue: {"waves": [[bank, track]], "variation",
//                                     "weights", "loop", "category"}}}
//
// A cue is either simple (one sound) or complex (a list of sounds chosen by variation). A sound plays one wave
// (simple) or clips of events, the play-wave events naming waves -- one, or a list played by variation (ordered,
// random, ...). Each wave is a wave bank index (into `wave_banks`) and a track in that bank. `loop` is the play
// event's loop count (255: forever); `weights` the variation's weight per entry when it has them.
//
// The layout is XACT3's, as MonoGame's SoundBank / XactSound / XactClip read the PC form; the Xbox 360 writes the
// same fields big-endian (magic "KBDS" where the PC has "SDBK"). Events other than play-wave (volume, pitch, marker)
// are skipped by their sizes where MonoGame knows them; a sound with one it does not is listed without waves.
//
// Typing notes:
//   * read() is formats.dll's xsb.read (native/formats/xbox/assets.cc, linked in and asked through cbf_call); the dict it
//     answers is the record Bank, a cue's dict the record Cue.  A wave [bank, track] is a two-item list.
//   * Replaces the hook `xsb_read` of xna::port (which takes the bank as JSON): Bank holds the same data.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::xsb {

// one cue of the bank
struct Cue {
    py::list<py::list<int64_t>> waves;           // [[wave bank index, track]]
    int64_t variation = 0;
    std::optional<py::list<int64_t>> weights;    // the variation's weight per entry, when it has them
    int64_t loop = 0;                            // the play event's loop count (255: forever)
    int64_t category = 0;
};
template <class F> void for_each_field_mut(Cue& c, F&& f) {
    f("waves", c.waves, true);
    f("variation", c.variation, true);
    f("weights", c.weights, true);
    f("loop", c.loop, true);
    f("category", c.category, true);
}

// the sound bank: {"wave_banks": [...], "cues": {...}}
struct Bank {
    py::list<py::str> wave_banks;
    py::dict<py::str, Cue> cues;
};
template <class F> void for_each_field_mut(Bank& b, F&& f) {
    f("wave_banks", b.wave_banks, true);
    f("cues", b.cues, true);
}

// The sound bank at `path`: {"wave_banks": [...], "cues": {...}} (formats.dll's, native/formats/xbox/assets.cc).
Bank read(const py::str& path);

}  // namespace codebreaker::formats::xbox::xsb
