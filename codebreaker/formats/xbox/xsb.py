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
import struct


class _R:
    def __init__(self, data, order):
        self.d, self.o, self.order = data, 0, order

    def seek(self, o):
        self.o = o

    def _u(self, fmt, n):
        v = struct.unpack_from(self.order + fmt, self.d, self.o)[0]
        self.o += n
        return v

    def u8(self): return self._u("B", 1)
    def u16(self): return self._u("H", 2)
    def i16(self): return self._u("h", 2)
    def u32(self): return self._u("I", 4)
    def f32(self): return self._u("f", 4)

    def skip(self, n):
        self.o += n


def _clip_waves(r):
    """One clip's play-wave events: [(waves, variation, weights, loop)]."""
    r.skip(1)                                  # volume
    events_at = r.u32()
    r.skip(2 + 2)                              # filter flags, filter frequency
    back = r.o
    r.seek(events_at)
    out = []
    for _ in range(r.u8()):
        info = r.u32()
        r.skip(2)                              # random offset
        kind = info & 0x1F
        if kind == 1:                          # play one wave
            r.skip(2)
            track, bank, loop = r.u16(), r.u8(), r.u8()
            r.skip(4)
            out.append(([[bank, track]], 0, None, loop))
        elif kind == 3:                        # play a list of waves
            r.skip(2)
            loop = r.u8()
            r.skip(4)
            count, flags = r.u16(), r.u8()
            r.skip(5)
            waves, weights = [], []
            for _ in range(count):
                track, bank, lo, hi = r.u16(), r.u8(), r.u8(), r.u8()
                waves.append([bank, track])
                weights.append(hi - lo)
            out.append((waves, flags & 0xF, weights, loop))
        elif kind == 4:                        # one wave with variation of pitch, volume, filter
            r.skip(2)
            track, bank, loop = r.u16(), r.u8(), r.u8()
            r.skip(4 + 4 + 2 + 16 + 2)
            out.append(([[bank, track]], 0, None, loop))
        elif kind == 6:                        # a list with those variations
            r.skip(2)
            loop = r.u8()
            r.skip(4 + 4 + 2 + 16 + 2)
            count, flags = r.u16(), r.u8()
            r.skip(5)
            waves, weights = [], []
            for _ in range(count):
                track, bank, lo, hi = r.u16(), r.u8(), r.u8(), r.u8()
                waves.append([bank, track])
                weights.append(hi - lo)
            out.append((waves, flags & 0xF, weights, loop))
        elif kind == 8:                        # volume
            r.skip(2 + 1 + 4 + 9)
        else:
            raise ValueError("clip event %d" % kind)
    r.seek(back)
    return out


def _sound(r):
    """A sound at the reader: (waves, variation, weights, loop, category)."""
    flags = r.u8()
    complex_ = flags & 1
    category = r.u16()
    r.skip(1 + 2 + 1 + 2)                      # volume, pitch, priority, entry length
    clips = 0
    waves = None
    if complex_:
        clips = r.u8()
    else:
        track, bank = r.u16(), r.u8()
        waves = ([[bank, track]], 0, None, 0)
    if flags & 0xE:                            # RPC curves
        start = r.o
        size = r.u16()
        r.seek(start + size)
    if flags & 0x10:                           # reverb (DSP presets)
        r.skip(7)
    if complex_:
        found = []
        for _ in range(clips):
            found += _clip_waves(r)
        if found:
            w = [x for f in found for x in f[0]]
            weights = None if all(f[2] is None for f in found) else [x for f in found for x in (f[2] or [1] * len(f[0]))]
            waves = (w, found[0][1], weights, max(f[3] for f in found))
        else:
            waves = ([], 0, None, 0)
    return waves + (category,)


def read(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] == b"SDBK":
        order = "<"
    elif data[:4] == b"KBDS":
        order = ">"
    else:
        raise ValueError("not an XACT sound bank (magic %r)" % data[:4])
    r = _R(data, order)
    r.seek(4)
    r.skip(2 + 2 + 2 + 4 + 4 + 1)              # tool version, format version, crc, timestamps, platform
    simple, complex_ = r.u16(), r.u16()
    r.skip(2 + 2)
    banks = r.u8()
    r.skip(2)                                  # sounds
    names_len = r.u32()                        # a 32-bit field (MonoGame reads its low half on the PC)
    simple_at, complex_at, names_at = r.u32(), r.u32(), r.u32()
    r.skip(4 + 4 + 4)
    banks_at = r.u32()
    r.seek(banks_at)
    bank_names = [data[r.o + 64 * i:r.o + 64 * (i + 1)].split(b"\0")[0].decode("utf-8", "replace") for i in range(banks)]
    names = data[names_at:names_at + names_len].decode("utf-8", "replace").split("\0")
    cues = {}

    def put(name, waves, variation, weights, loop, category):
        cues[name] = {"waves": waves, "variation": variation, "weights": weights, "loop": loop, "category": category}

    r.seek(simple_at)
    for i in range(simple):
        r.skip(1)
        sound_at = r.u32()
        back = r.o
        r.seek(sound_at)
        try:
            put(names[i], *_sound(r))
        except (ValueError, struct.error):
            put(names[i], [], 0, None, 0, 0)
        r.seek(back)
    r.seek(complex_at)
    for k in range(complex_):
        name = names[simple + k] if simple + k < len(names) else "cue%d" % (simple + k)
        flags = r.u8()
        if (flags >> 2) & 1:                    # one sound
            sound_at = r.u32()
            r.skip(4)
            back = r.o
            r.seek(sound_at)
            try:
                put(name, *_sound(r))
            except (ValueError, struct.error):
                put(name, [], 0, None, 0, 0)
            r.seek(back)
        else:                                   # a variation table of sounds or waves
            table_at = r.u32()
            r.skip(4)
            back = r.o
            r.seek(table_at)
            count, vflags = r.u16(), r.u16()
            r.skip(1 + 2 + 1)
            kind = (vflags >> 3) & 7
            waves, weights, loop, category = [], [], 0, 0
            try:
                for _ in range(count):
                    if kind in (0, 4):
                        track, bank = r.u16(), r.u8()
                        if kind == 0:
                            lo, hi = r.u8(), r.u8()
                            weights.append(hi - lo)
                        waves.append([bank, track])
                    elif kind in (1, 3):
                        sound_at = r.u32()
                        if kind == 1:
                            lo, hi = r.u8(), r.u8()
                            weights.append(hi - lo)
                        else:
                            r.skip(4 + 4 + 4)
                        here = r.o
                        r.seek(sound_at)
                        w, _v, _wt, lp, category = _sound(r)
                        waves += w
                        loop = max(loop, lp)
                        r.seek(here)
                    else:
                        raise ValueError("variation kind %d" % kind)
            except (ValueError, struct.error):
                pass
            put(name, waves, vflags & 7, weights or None, loop, category)
            r.seek(back)
        r.skip(1 + 2 + 2 + 1)                   # instance limit, fades, max instance behaviour
    return {"wave_banks": bank_names, "cues": cues}
