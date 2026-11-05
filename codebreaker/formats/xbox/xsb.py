"""xsb.py -- an XACT sound bank (.xsb): every cue, and the waves it plays.

    from codebreaker.formats.xbox import xsb
    bank = xsb.read(path)       {"wave_banks": [name], "cues": {cue: {"waves": [[bank, track]], "variation",
                                 "weights", "loop", "category"}}}

A cue is either simple (one sound) or complex (a list of sounds chosen by variation). A sound plays one wave
(simple) or clips of events, the play-wave events naming waves -- one, or a list played by variation (ordered,
random, ...). Each wave is a wave bank index (into `wave_banks`) and a track in that bank. `loop` is the play
event's loop count (255: forever); `weights` the variation's weight per entry when it has them.

The layout is XACT3's, as MonoGame's SoundBank / XactSound / XactClip read the PC form; the Xbox 360 writes the
same fields big-endian (magic "KBDS" where the PC has "SDBK"). Events other than play-wave (volume, pitch, marker)
are skipped by their sizes where MonoGame knows them; a sound with one it does not is listed without waves.
"""
from codebreaker.formats import native


@native.answered("xsb.read", data_arg=None)
def read(path):
    """The sound bank at `path`: {"wave_banks": [...], "cues": {...}} (formats.dll's, native/formats/xbox/assets.cc)."""
