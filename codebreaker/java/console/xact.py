# -*- coding: utf-8 -*-
r"""
xact.py -- XACT3 wave-bank (.xwb) / sound-bank (.xsb) tool for the Xbox 360 audio pipeline.

360 audio is XACT3 or XAudio2; the port uses XACT3 (Audio.reference routes SoundManager -> XACT cues).
This tool reads/decompiles the banks (Phase 1, here) and -- Phase 2 -- builds them from Content.md's
sound list + the .ogg/.wav sources. It is grounded in the banks the user authored by hand for the
Alpha port (…\MinecraftAlpha\…\Content\Audio\sounds.xwb+.xsb, music.*).

Format (Xbox 360 = BIG-ENDIAN; XACT stores the magic byte-reversed vs the PC LE 'WBND'/'SDBK'):
  XWB  magic 'DNBW'  header{version, headerVersion, 5x(offset,length) segments:
       BANKDATA/ENTRYMETADATA/SEEKTABLES/ENTRYNAMES/ENTRYWAVEDATA}
       BANKDATA{flags, entryCount, name[64], metaElemSize, nameElemSize, alignment, compactFormat,...}
       ENTRYMETADATA[entryCount] of WAVEBANKENTRY{ flagsAndDuration, miniFormat(packed DWORD),
       PlayRegion(offset,length), LoopRegion(...) }  -- miniFormat packs tag/channels/rate/align/bits.
  XSB  magic 'KBDS' ... trailing table of null-terminated CUE NAMES (the sound-event names).
"""

import os
import re
import struct


# ---------------------------------------------------------------------------
def _u32be(b, o):
    return struct.unpack_from('>I', b, o)[0]


def _cue_name(event):
    """Java sound EVENT ('step.grass', 'random.click') -> XACT cue base name ('step_grass',
    'random_click'). Dots/spaces -> underscores; matches the toCue() convention in Audio.reference."""
    return re.sub(r'[.\s]+', '_', event.strip())


WAVEBANK_SEGMENTS = ['BANKDATA', 'ENTRYMETADATA', 'SEEKTABLES', 'ENTRYNAMES', 'ENTRYWAVEDATA']
_FORMAT_TAG = {0: 'PCM', 1: 'XMA', 2: 'ADPCM', 3: 'WMA'}


def _decode_miniformat(dw):
    """WAVEBANKMINIWAVEFORMAT packed DWORD -> (tag, channels, samplesPerSec, blockAlign, bitsPerSample).
    Layout: tag[0:2] channels[2:5] rate[5:23] blockAlign[23:31] bits[31:32]."""
    tag = dw & 0x3
    channels = (dw >> 2) & 0x7
    rate = (dw >> 5) & 0x3FFFF
    block = (dw >> 23) & 0xFF
    bits16 = (dw >> 31) & 0x1
    return {'codec': _FORMAT_TAG.get(tag, '?%d' % tag), 'channels': channels,
            'sample_rate': rate, 'block_align': block, 'bits': 16 if bits16 else 8}


def parse_xwb(path):
    """Decompile an .xwb wave bank -> {name, magic, version, entry_count, segments, entries[]}."""
    with open(path, 'rb') as f:
        b = f.read()
    magic = b[0:4].decode('latin-1')
    if magic not in ('DNBW', 'WBND'):
        raise ValueError('%s: not an XWB (magic %r)' % (os.path.basename(path), magic))
    version, header_version = _u32be(b, 4), _u32be(b, 8)
    segments = {}
    o = 12
    for name in WAVEBANK_SEGMENTS:
        segments[name] = (_u32be(b, o), _u32be(b, o + 4))
        o += 8
    bd_off, _bd_len = segments['BANKDATA']
    flags = _u32be(b, bd_off)
    entry_count = _u32be(b, bd_off + 4)
    bank_name = b[bd_off + 8:bd_off + 8 + 64].split(b'\x00', 1)[0].decode('latin-1', 'replace')
    meta_elem = _u32be(b, bd_off + 8 + 64)
    name_elem = _u32be(b, bd_off + 8 + 64 + 4)

    # entry metadata (non-compact 24-byte WAVEBANKENTRY: flagsDur(4), miniFormat(4), play(8), loop(8))
    entries = []
    meta_off, _meta_len = segments['ENTRYMETADATA']
    names_off, names_len = segments['ENTRYNAMES']
    for i in range(entry_count):
        eo = meta_off + i * (meta_elem if meta_elem >= 24 else 24)
        if eo + 24 > len(b):
            break
        mini = _u32be(b, eo + 4)
        play_off, play_len = _u32be(b, eo + 8), _u32be(b, eo + 12)
        ent = {'index': i, 'play_offset': play_off, 'play_length': play_len}
        ent.update(_decode_miniformat(mini))
        if names_off and name_elem:                      # per-entry name (if the bank kept names)
            no = names_off + i * name_elem
            ent['name'] = b[no:no + name_elem].split(b'\x00', 1)[0].decode('latin-1', 'replace')
        entries.append(ent)
    return {'file': os.path.basename(path), 'magic': magic, 'version': version,
            'header_version': header_version, 'name': bank_name, 'flags': flags,
            'entry_count': entry_count, 'segments': segments, 'entries': entries}


def parse_xsb(path):
    """Decompile an .xsb sound bank -> {name, magic, cue_names[]}. Cue names are the sound EVENTS the
    game triggers (step_grass, cave chimes, ...). Extracts the trailing null-terminated name table."""
    with open(path, 'rb') as f:
        b = f.read()
    magic = b[0:4].decode('latin-1')
    if magic not in ('KBDS', 'SDBK'):
        raise ValueError('%s: not an XSB (magic %r)' % (os.path.basename(path), magic))
    # bank name + cue names are null-terminated printable-ASCII runs; harvest them in order.
    names, cur = [], []
    for byte in b[16:]:
        c = byte if isinstance(byte, int) else ord(byte)
        if 32 <= c < 127:
            cur.append(chr(c))
        else:
            if len(cur) >= 3:
                names.append(''.join(cur))
            cur = []
    if len(cur) >= 3:
        names.append(''.join(cur))
    bank_name = names[0] if names else ''
    # drop the bank-name repeat(s) at the head; the rest are cue names
    cues = [n for n in names[1:] if n != bank_name]
    return {'file': os.path.basename(path), 'magic': magic, 'name': bank_name, 'cue_names': cues}


# ---------------------------------------------------------------------------
def dump(path):
    """Human-readable decompile of an .xwb or .xsb."""
    ext = os.path.splitext(path)[1].lower()
    if ext == '.xwb':
        w = parse_xwb(path)
        L = ['WaveBank "%s"  (%s v%d)  %d entries' % (w['name'], w['magic'], w['version'], w['entry_count'])]
        for name, (off, ln) in w['segments'].items():
            L.append('  segment %-14s off=0x%X len=0x%X' % (name, off, ln))
        L.append('  entries:')
        for e in w['entries'][:64]:
            L.append('    [%3d] %-5s %dch %6dHz %2dbit  wave@0x%X len=%d%s'
                     % (e['index'], e['codec'], e['channels'], e['sample_rate'], e['bits'],
                        e['play_offset'], e['play_length'],
                        ('  "%s"' % e['name']) if e.get('name') else ''))
        if w['entry_count'] > 64:
            L.append('    … (%d more)' % (w['entry_count'] - 64))
        return '\n'.join(L)
    if ext == '.xsb':
        s = parse_xsb(path)
        L = ['SoundBank "%s"  (%s)  %d cues' % (s['name'], s['magic'], len(s['cue_names']))]
        for c in s['cue_names']:
            L.append('  cue: %s' % c)
        return '\n'.join(L)
    raise ValueError('unknown bank type: %s (expected .xwb / .xsb)' % path)


# ===========================================================================
# BUILD side -- from the game's sound EVENTS to an XACT project / bank.
# ===========================================================================
def needed_cues(java_dir):
    """The XACT cues the converted game will request, derived from the sound EVENTS + step-sound
    families it references (via content.scan_assets). Returns {cue_name: [source_event(s)]}. This is
    the demand side; diff it against a bank's authored cues to find the silent-audio gaps."""
    from codebreaker.java import content
    a = content.scan_assets(java_dir)
    cues = {}
    for ev in a['sounds']:                               # playSound("random.click") -> random_click
        cues.setdefault(_cue_name(ev), []).append(ev)
    for fam in a['step_sounds']:                         # StepSound("stone") -> step_stone / dig_stone families
        for prefix in ('step', 'dig'):
            cues.setdefault('%s_%s' % (prefix, fam), []).append('%s.%s' % (prefix, fam))
    return cues


def cue_coverage(java_dir, xsb_path):
    """Diff the cues the game NEEDS against those AUTHORED in an existing sound bank. Returns
    (needed, authored, missing, unused). `missing` = cues the game plays that the bank lacks ->
    GetCueIndex fails -> the sound silently no-ops (the class Audio.reference warns about)."""
    needed = needed_cues(java_dir)
    authored = set(re.sub(r'\s+\d+$', '', c) for c in parse_xsb(xsb_path)['cue_names'])  # drop " 2".." 4" variant suffix
    needset = set(needed)
    missing = sorted(needset - authored)
    unused = sorted(authored - needset)
    return {'needed': needed, 'authored': sorted(authored), 'missing': missing, 'unused': unused}


def render_coverage(cov):
    L = ['# AUDIO_COVERAGE.md -- XACT cue coverage', '',
         'Cues the game plays vs cues authored in the bank. A **MISSING** cue means the game calls it '
         'but the bank has no such entry -> XACT GetCueIndex fails -> the sound silently no-ops.', '',
         '- needed: %d   authored: %d   **MISSING: %d**   unused-in-bank: %d'
         % (len(cov['needed']), len(cov['authored']), len(cov['missing']), len(cov['unused'])), '']
    if cov['missing']:
        L += ['## MISSING cues -- author these into the bank (or the sound is silent)', '',
              '| Cue | Triggered by |', '|---|---|']
        for c in cov['missing']:
            L.append('| `%s` | %s |' % (c, ', '.join(cov['needed'][c][:3])))
        L.append('')
    return '\n'.join(L) + '\n'


# --- .xap project generation (Route A: the XDK XACTBLD compiles it to .xwb/.xsb) ----------------
def build_xap(java_dir, out_path, wave_dir='wav', bank='sounds', only_cues=None,
              cue_list=None, cue_namer=None, sound_category='Default', streaming=False):
    """Generate an XACT3 project (.xap) that AUTHORS the cues, each referencing a .wav in `wave_dir`.
    XACTBLD compiles it to <bank>.xwb + <bank>.xsb. Returns the cue list.
    only_cues: author ONLY these (the matched set).  cue_list: use this EXPLICIT cue list instead of the
    game's SFX cues (for the music bank).  cue_namer: cue Name mapper (default underscores->dots; music
    passes identity so calm1/2/3 stay bare to match XactAudio g_MusicCues).  sound_category: XACT category
    the Sounds go in ('Default' SFX / 'Music').  streaming: mark the Wave Bank streaming (music.xwb)."""
    # FIXED BUG audio-cue-name-mismatch: the XACT cue name must MATCH what Audio.cpp toCue() emits, which is
    # the UNDERSCORE form ("random.click" -> "random_click", "step.grass" -> "step_grass"). The old default
    # dotted it ("random.click"), so GetCueIndex("random_click") failed ("cue not found in cue table") and
    # every SFX was silent. Keep the cue name identical to the wave/sound name (underscore) = toCue's output.
    if cue_namer is None:
        cue_namer = lambda c: c
    if cue_list is not None:
        cues = sorted(cue_list)
    else:
        cues = sorted(needed_cues(java_dir))
        if only_cues is not None:                        # author ONLY these cues (the matched set)
            cues = [c for c in cues if c in only_cues]
    # FIXED BUG audio-xap-missing-wav: author ONLY cues whose .wav was actually PRODUCED. Some source oggs are
    # 169-byte placeholders (failed downloads) that ffmpeg can't decode -> no wav -> XACTBLD hard-aborts on
    # ALL banks ("wave file could not be found"). match_sources matches on ogg existence, not validity, so we
    # additionally gate on the real wav. (When the wav dir isn't populated yet, keep the list as-is.)
    wave_abs = os.path.join(os.path.dirname(os.path.abspath(out_path)), wave_dir)
    if os.path.isdir(wave_abs):
        present = set(os.path.splitext(f)[0] for f in os.listdir(wave_abs) if f.lower().endswith('.wav'))
        kept = [c for c in cues if c in present]
        if kept:
            cues = kept
    wb, sb = 'WaveBank', 'SoundBank'                     # internal bank names (linked by the Sound's Wave Entry)

    # FIXED BUG audio-xap-grammar: XACT3 .xap blocks require the FULL field set the XDK XACTBLD validates.
    # The old compact form (`Category { Name=X; Volume=0; }`, `Track { Play Wave Event { Wave Entry {..} } }`,
    # `Sound Entry { Name=X; }`) is REJECTED with "project file is missing one or more required values", so no
    # .xwb/.xsb were ever built. Grammar mirrored from the XDK sample Source\Samples\Audio\XactBasicSound.
    def category(name, parent=None, bgm=0):
        out = ['    Category', '    {', '        Name = %s;' % name, '        Public = 1;',
               '        Background Music = %d;' % bgm, '        Volume = 0;', '',
               '        Category Entry', '        {']
        if parent:
            out.append('            Name = %s;' % parent)
        out += ['        }', '', '        Instance Limit', '        {', '            Max Instances = 255;',
                '            Behavior = 0;', '', '            Crossfade', '            {',
                '                Fade In = 0;', '                Fade Out = 0;',
                '                Crossfade Type = 0;', '            }', '        }', '    }', '']
        return out

    # bare output filenames: XACTBLD runs from Content\Audio with output path '.', so a stored
    # 'Content\Audio\X' would nest to Content\Audio\Content\Audio\X.
    L = ['Signature = XACT3;', 'Version = 18;', 'Content Version = 46;', 'Release = March 2008;', '',
         'Options', '{', '    Verbose Report = 0;', '    Generate C/C++ Headers = 1;', '}', '',
         'Global Settings', '{', '    Xbox File = Global.xgs;', '    Header File = Global.h;',
         '    Exclude Category Names = 0;', '    Exclude Variable Names = 0;', '']
    L += category('Global')
    L += category('Default', parent='Global')
    L += category('Music', parent='Global', bgm=1)
    L += ['}', '']
    # Wave Bank -- PCM (no compression preset): XACTBLD packs the ffmpeg 16-bit .wav as-is. `streaming`
    # marks it for CreateStreamingWaveBank (music.xwb: large tracks are streamed off disk, not RAM).
    L += ['Wave Bank', '{', '    Name = %s;' % wb, '    Xbox File = %s.xwb;' % bank,
          '    Xbox Bank Path Edited = 1;'] + (['    Streaming = 1;'] if streaming else []) + \
         ['    Entry Names = 1;', '    Seek Tables = 1;', '']
    for c in cues:
        L += ['    Wave', '    {', '        Name = %s;' % c,
              '        File = %s\\%s.wav;' % (wave_dir, c), '    }', '']
    L += ['}', '']
    # Sound Bank
    L += ['Sound Bank', '{', '    Name = %s;' % sb, '    Xbox File = %s.xsb;' % bank,
          '    Xbox Header File = %s.h;' % bank, '']
    # FIXED BUG audio-wave-entry-index: the Sound's Wave Entry index MUST be the wave's real position in the
    # wave bank (waves are authored in `cues` order below, so cue i -> wave i). It was hardcoded 0, so every
    # cue played wave #0 -> every SFX was the SAME wrong sound. Set Entry Index = the enumerated wave index.
    for _wi, c in enumerate(cues):
        L += ['    Sound', '    {', '        Name = %s;' % c, '        Volume = 0;', '        Pitch = 0;',
              '        Priority = 0;', '', '        Category Entry', '        {', '            Name = %s;' % sound_category,
              '        }', '', '        Track', '        {', '            Volume = 0;',
              '            Use Filter = 0;', '', '            Play Wave Event', '            {',
              '                Break Loop = 0;', '                Use Speaker Position = 0;',
              '                Use Center Speaker = 1;', '                New Speaker Position On Loop = 1;',
              '                Speaker Position Angle = 0.000000;',
              '                Speaker Position Arc = 0.000000;', '', '                Event Header',
              '                {', '                    Timestamp = 0;', '                    Relative = 0;',
              '                    Random Recurrence = 0;', '                    Random Offset = 0;',
              '                }', '', '                Wave Entry', '                {',
              '                    Bank Name = %s;' % wb, '                    Bank Index = 0;',
              '                    Entry Name = %s;' % c, '                    Entry Index = %d;' % _wi,
              '                    Weight = 0;', '                    Weight Min = 0;', '                }',
              '            }', '        }', '    }', '']
    # FIXED BUG audio-cue-sound-index: the Cue's Sound Entry index MUST be the sound's real position in the
    # sound bank (sounds are authored in `cues` order above, so cue i -> sound i). It was hardcoded 0, so
    # EVERY cue resolved to Sound #0 (the first sorted cue) -> every SFX played the SAME wrong sound (e.g. the
    # button click fired break_grass1). Fixing the Wave Entry index alone was not enough: the Cue->Sound hop
    # was still collapsed to 0. Set Index = the enumerated sound index (== the matching wave index).
    for _si, c in enumerate(cues):                       # cue keyed by the DOTTED event so toCue() maps back
        ev = cue_namer(c)
        L += ['    Cue', '    {', '        Name = %s;' % ev, '', '        Variation', '        {',
              '            Variation Type = 3;', '            Variation Table Type = 1;',
              '            New Variation on Loop = 0;', '        }', '', '        Sound Entry', '        {',
              '            Name = %s;' % c, '            Index = %d;' % _si, '            Weight Min = 0;',
              '            Weight Max = 0;', '        }', '    }', '']
    L += ['}', '']
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
    return cues


def write_build_script(out_dir, bank='sounds', bat_name='build_banks.bat'):
    """Emit <bat_name>: run the XDK XACTBLD to compile <bank>.xap -> .xwb/.xsb for the 360."""
    bat = ('@echo off\r\n'
           'rem Build the XACT audio banks for Xbox 360 (needs the Xbox 360 SDK on PATH).\r\n'
           'set XACTBLD="%%XEDK%%\\bin\\win32\\xactbld3.exe"\r\n'
           # FIXED BUG audio-xactbld-args: XACTBLD needs /XBOX360 (arch is MANDATORY) and the project as a
           # POSITIONAL arg. The old `/X:%s.xap` was parsed as an invalid "/X: do-not-build" option (no
           # project) -> XACTBLD built nothing yet exited 0, so no .xwb/.xsb were ever produced.
           '%%XACTBLD%% /L /F /XBOX360 %s.xap .\r\n'
           'echo Built %s.xwb / %s.xsb into Content\\Audio\\\r\n' % (bank, bank, bank))
    p = os.path.join(out_dir, bat_name)
    with open(p, 'w', encoding='utf-8') as f:
        f.write(bat)
    return p


# --- direct PCM wave-bank writer (Route B: no XACTBLD; PCM only) --------------------------------
def _parse_wav(path):
    """Minimal RIFF/WAVE PCM reader -> (channels, rate, bits, data_bytes)."""
    with open(path, 'rb') as f:
        b = f.read()
    if b[0:4] != b'RIFF' or b[8:12] != b'WAVE':
        raise ValueError('%s: not a RIFF/WAVE file' % os.path.basename(path))
    ch = rate = bits = 0
    data = b''
    o = 12
    while o + 8 <= len(b):
        cid, sz = b[o:o + 4], struct.unpack_from('<I', b, o + 4)[0]
        body = b[o + 8:o + 8 + sz]
        if cid == b'fmt ':
            ch = struct.unpack_from('<H', body, 2)[0]
            rate = struct.unpack_from('<I', body, 4)[0]
            bits = struct.unpack_from('<H', body, 14)[0]
        elif cid == b'data':
            data = body
        o += 8 + sz + (sz & 1)
    return ch, rate, bits, data


def _miniformat(ch, rate, bits):
    """Pack PCM params into a WAVEBANKMINIWAVEFORMAT DWORD (tag=0 PCM)."""
    block = ch * (bits // 8)
    return (0 & 0x3) | ((ch & 0x7) << 2) | ((rate & 0x3FFFF) << 5) | ((block & 0xFF) << 23) | (((1 if bits == 16 else 0) & 0x1) << 31)


def build_pcm_xwb(wav_files, out_xwb, bank='sounds'):
    """Write a REAL Xbox-360 (big-endian) .xwb wave bank from PCM .wav files -- no XACTBLD needed.
    PCM only (XWMA/XMA compression needs the XDK encoder); good for small SFX / bring-up. Returns entries."""
    entries, wavedata = [], bytearray()
    for path in wav_files:
        ch, rate, bits, data = _parse_wav(path)
        pad = (-len(wavedata)) % 2048                    # 360 wave alignment
        wavedata += b'\x00' * pad
        entries.append({'name': os.path.splitext(os.path.basename(path))[0],
                        'mini': _miniformat(ch, rate, bits), 'off': len(wavedata), 'len': len(data)})
        wavedata += data
    n = len(entries)
    ENTRY = 24
    seg_meta = 52 + 8                                    # header(52) + bankdata... computed below
    # BANKDATA
    bankdata = struct.pack('>II', 0, n) + bank.encode('latin-1')[:64].ljust(64, b'\x00') + struct.pack('>III', ENTRY, 0, 2048)
    meta_off = 52 + len(bankdata)
    meta = b''.join(struct.pack('>IIIIII', 4, e['mini'], e['off'], e['len'], 0, 0) for e in entries)
    wave_off = (meta_off + len(meta) + 2047) & ~2047
    header = b'DNBW' + struct.pack('>II', 46, 44)
    segs = [(52, len(bankdata)), (meta_off, len(meta)), (0, 0), (0, 0), (wave_off, len(wavedata))]
    header += b''.join(struct.pack('>II', o, l) for o, l in segs)
    out = bytearray(header + bankdata + meta)
    out += b'\x00' * (wave_off - len(out))
    out += wavedata
    with open(out_xwb, 'wb') as f:
        f.write(out)
    return entries


# --- source matching: cue -> original .ogg file(s) ---------------------------------------------
# Resource-tree roots to strip so a file path lines up with an event name. Minecraft keeps the same
# sound under several roots across versions (`sound/`, `newsound/`, `music/`, `newmusic/`).
_OGG_ROOTS = ('newsound', 'sound', 'newmusic', 'music', 'streaming', 'resources')
_TRAIL_NUM = re.compile(r'(\d+)$')


def _ogg_keys(rel_path):
    """The lookup keys a .ogg registers under. rel_path is forward-slash, relative to resources_dir.
    We strip a known root prefix, drop the `.ogg`, drop the trailing variant number (grass1 -> grass),
    and also register an underscores->slashes variant so `random/door_close` matches event
    `random.door.close`. Returns a set of slash-joined keys."""
    p = rel_path[:-4] if rel_path.lower().endswith('.ogg') else rel_path
    parts = p.split('/')
    if parts and parts[0] in _OGG_ROOTS:
        parts = parts[1:]
    if not parts:
        return set()
    parts[-1] = _TRAIL_NUM.sub('', parts[-1])             # grass1 -> grass, say3 -> say
    base = '/'.join(parts)
    keys = {base, base.replace('_', '/')}                 # door_close  AND  door/close
    return set(k for k in keys if k)


def scan_ogg_sources(resources_dir):
    """Index every .ogg under resources_dir by lookup key -> sorted[relative paths]. One key can map
    to several files (the numbered variants grass1..grass4)."""
    index = {}
    for dp, _dirs, files in os.walk(resources_dir):
        for f in files:
            if not f.lower().endswith('.ogg'):
                continue
            rel = os.path.relpath(os.path.join(dp, f), resources_dir).replace(os.sep, '/')
            for k in _ogg_keys(rel):
                index.setdefault(k, set()).add(rel)
    return dict((k, sorted(v)) for k, v in index.items())


def match_sources(java_dir, resources_dir):
    """Map each needed cue to the original .ogg file(s) that supply it. Handles two early-Minecraft
    quirks: `dig.*` reuses the `step.*` samples (no separate dig oggs), and multi-word leaf names are
    stored with an underscore (`door_close.ogg`) rather than a sub-folder. Returns
    {'matched': {cue: {'event':.., 'files':[..]}}, 'missing': {cue:[events]}}."""
    index = scan_ogg_sources(resources_dir)
    needed = needed_cues(java_dir)
    matched, missing = {}, {}
    for cue, events in sorted(needed.items()):
        files, used_event = [], None
        for ev in events:
            key = ev.replace('.', '/')
            cand = index.get(key)
            if not cand and key.startswith('dig/'):       # dig.grass -> step.grass (same samples)
                cand = index.get('step/' + key[len('dig/'):])
            if cand:
                files, used_event = cand, ev
                break
        if files:
            matched[cue] = {'event': used_event, 'files': files}
        else:
            missing[cue] = events
    return {'matched': matched, 'missing': missing, 'index_keys': len(index)}


def write_ffmpeg_script(match, out_dir, resources_dir, wav_subdir='wav', bat_name='convert_audio.bat'):
    """Emit convert_audio.bat: an ffmpeg command per matched cue that decodes its .ogg to a 16-bit
    PCM .wav named after the cue, under out_dir\\<wav_subdir>\\. That .wav is what build_banks.bat /
    build_pcm_xwb consumes. Uses the FIRST variant file per cue (bank builder handles variants
    separately). Returns the .bat path."""
    wav_dir = os.path.join(out_dir, wav_subdir)
    if not os.path.isdir(wav_dir):
        os.makedirs(wav_dir)
    lines = ['@echo off',
             'rem Decode the matched Minecraft .ogg sources to 16-bit PCM .wav for the XACT banks.',
             'rem Needs ffmpeg on PATH. Source: %s' % resources_dir,
             'setlocal', 'set SRC=%s' % resources_dir, 'set OUT=%s' % wav_subdir, '']
    for cue in sorted(match['matched']):
        src = match['matched'][cue]['files'][0].replace('/', '\\')
        lines.append('ffmpeg -y -i "%%SRC%%\\%s" -ar 44100 -ac 1 -sample_fmt s16 "%%OUT%%\\%s.wav"'
                     % (src, cue))
    lines += ['', 'echo Decoded %d cues to %%OUT%%\\' % len(match['matched']), 'endlocal', '']
    p = os.path.join(out_dir, bat_name)
    with open(p, 'w', encoding='utf-8') as f:
        f.write('\r\n'.join(lines))
    return p


def _find_resources_dir(java_dir):
    """Best-effort locate the original .ogg resources tree near the java source (a sibling/ancestor
    `resources` folder with a `newsound` or `sound` child). Returns the path or None."""
    d = os.path.abspath(java_dir)
    for _ in range(5):
        for cand in (os.path.join(d, 'resources'), os.path.join(d, 'game', 'resources')):
            if os.path.isdir(os.path.join(cand, 'newsound')) or os.path.isdir(os.path.join(cand, 'sound')):
                return cand
        parent = os.path.dirname(d)
        if parent == d:
            break
        d = parent
    return None


def _clean_audio_intermediates(out_dir):
    """FIXED BUG assets-in-project-root: the audio out dir IS the deploy dir (Debug\\Content\\Audio, shipped
    to the console), so after the banks build, drop the build intermediates -- the wav/ dir alone is ~60MB
    (the decoded music) of pure shippable junk. Keep ONLY the runtime .xwb/.xsb banks. Only runs once the
    banks exist, so a failed build never nukes the sources."""
    import shutil
    try:
        entries = os.listdir(out_dir)
    except OSError:
        return
    if not any(f.endswith('.xwb') for f in entries):
        return                                           # banks didn't build -> leave everything for a retry
    shutil.rmtree(os.path.join(out_dir, 'wav'), ignore_errors=True)
    for f in entries:
        if f.endswith(('.bat', '.xap', '.h', '.xgs', '.md', '.txt')):
            try:
                os.remove(os.path.join(out_dir, f))
            except OSError:
                pass


def build_music(out_dir, resources_dir, tracks=('calm1', 'calm2', 'calm3')):
    """Set up the STREAMING music bank (music.xwb/.xsb) from the calm* background tracks. Cues are the bare
    track names (calm1/2/3) to match XactAudio's g_MusicCues playlist, placed in the Music category. Writes
    music_convert.bat (ogg->wav) + music.xap + music_banks.bat. Returns the tracks actually found.
    FIXED BUG audio-no-music-bank: `build` only ever authored the SFX bank; music.xwb/.xsb were never built,
    so XactAudio::loadMusicBank failed -> g_musicDisabled -> no music. This adds the second (music) bank."""
    music_src = os.path.join(resources_dir, 'music')
    found = [t for t in tracks if os.path.isfile(os.path.join(music_src, t + '.ogg'))]
    if not found:
        return []
    # synthetic match dict (files relative to resources_dir, like match_sources output) -> music_convert.bat
    match = {'matched': dict((t, {'event': t, 'files': ['music/%s.ogg' % t]}) for t in found)}
    write_ffmpeg_script(match, out_dir, resources_dir, bat_name='music_convert.bat')
    build_xap(None, os.path.join(out_dir, 'music.xap'), bank='music', cue_list=found,
              cue_namer=lambda c: c, sound_category='Music', streaming=True)
    write_build_script(out_dir, bank='music', bat_name='music_banks.bat')
    return found


def build(java_dir, out_dir, resources_dir=None, autorun=False, sdk_root=None, log=None):
    """One-shot XACT3 audio build for a converted game: write the .xap project, the XACTBLD build script,
    the ogg->wav ffmpeg script, and AUDIO_COVERAGE.md. When `autorun` is set, ALSO run the scripts
    (convert_audio.bat then build_banks.bat) so the .xwb/.xsb banks are produced as part of conversion --
    no manual step. `log` is an optional callable(str) for progress. Returns (xap_path, cues)."""
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    xap = os.path.join(out_dir, 'sounds.xap')
    # FIXED BUG audio-xap-missing-wav: match sources FIRST, then author ONLY the cues that have a source wav.
    # Authoring every needed cue made XACTBLD fail hard on `wav\<cue>.wav could not be found` for the few
    # cues with no source ogg (random_bow, random_door_*) -> the whole bank build aborted, no banks. Unmatched
    # cues are simply absent -> GetCueIndex fails at runtime -> XaPlayCue no-ops (that one sound is silent).
    resources = resources_dir or _find_resources_dir(java_dir)
    avail = None
    music = []
    if resources:                                        # auto-emit the ogg->wav decode script if we can find the sources
        m = match_sources(java_dir, resources)
        avail = set(m['matched'].keys())
        write_ffmpeg_script(m, out_dir, resources)
        music = build_music(out_dir, resources)          # SECOND bank: streaming music.xwb/.xsb (calm1/2/3)
    write_build_script(out_dir)
    with open(os.path.join(out_dir, 'AUDIO_COVERAGE.md'), 'w', encoding='utf-8') as f:
        cov_needed = needed_cues(java_dir)
        f.write('# AUDIO cues the game needs (%d)\n\nAuthor a .wav per cue under `wav/`, then run '
                '`build_banks.bat` (XACTBLD).\n\n' % len(cov_needed)
                + '\n'.join('- `%s.wav`  <- %s' % (c, ', '.join(v[:2])) for c, v in sorted(cov_needed.items())) + '\n')
    if autorun:
        # Decode oggs -> wavs FIRST, author the xaps from the wavs that ACTUALLY landed (build_xap gates on
        # present wavs), THEN compile the banks. Ordering matters: some source oggs are placeholders that
        # ffmpeg drops, so the cue list must be finalized AFTER conversion, else XACTBLD aborts on all banks.
        run_build(out_dir, sdk_root=sdk_root, log=log, only=['convert_audio.bat', 'music_convert.bat'])
        # FIXED BUG audio-sfx-streaming: the SFX bank MUST be built streaming (Streaming=1). XactAudio::
        # loadSFXBank uses CreateStreamingWaveBank (no in-memory fallback, unlike loadMusicBank), so a
        # non-streaming sounds.xwb triggers XACT "Attempting to stream an in-memory wavebank" -> DbgBreakPoint.
        cues = build_xap(java_dir, xap, only_cues=avail, streaming=True)
        if music:
            build_music(out_dir, resources)              # re-author music.xap now that calmN.wav exist
        run_build(out_dir, sdk_root=sdk_root, log=log, only=['build_banks.bat', 'music_banks.bat'])
        _clean_audio_intermediates(out_dir)              # ship only the .xwb/.xsb banks, not wav/ + scripts
    else:
        cues = build_xap(java_dir, xap, only_cues=avail, streaming=True)
    return xap, cues


# J2X-BUGFIX 2026-08-13 audio-never-built: the wizard audio step only wrote convert_audio.bat and never
# generated the .xap/build_banks.bat, so the XACT banks were never produced -> no game audio. build() now
# runs the FULL XACT3 build, and run_build() (below) AUTORUNS both bats (ffmpeg then XACTBLD), skipping
# gracefully if a tool is missing. Content path is Content\Audio\ (not Content\resources).
def run_build(out_dir, sdk_root=None, log=None, only=None):
    """AUTORUN the two audio scripts in out_dir: convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat
    (XACTBLD xap -> sounds.xwb/.xsb). Each step is SKIPPED gracefully (never fails the conversion) if its
    tool is missing -- ffmpeg on PATH, or XACTBLD at %XEDK%\\bin\\win32\\xactbld3.exe. Returns (ok, lines)."""
    import subprocess
    try:
        from shutil import which
    except ImportError:
        which = lambda _n: None
    _log = log or (lambda _m: None)
    lines = []
    env = dict(os.environ)
    if sdk_root and not env.get('XEDK'):
        env['XEDK'] = sdk_root

    def emit(m):
        lines.append(m); _log(m)

    def _run(bat, tool_ok, tool_hint):
        p = os.path.join(out_dir, bat)
        if not os.path.isfile(p):
            emit('[audio] skip %s (not generated)' % bat); return True
        if not tool_ok():
            emit('[audio] SKIP %s -- %s not found; the .bat is written, run it later.' % (bat, tool_hint))
            return True
        try:
            # FIXED BUG audio-autorun-cwd: pass the FULL path to the .bat. `cmd /c convert_audio.bat`
            # (bare name) fails with "not recognized" when cwd-search is disabled, so the banks never built.
            r = subprocess.Popen(['cmd', '/c', p], cwd=out_dir, env=env,
                                 stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            out, _ = r.communicate(timeout=1800)
            txt = (out or b'').decode('utf-8', 'replace')
            emit('[audio] %s -> exit %d' % (bat, r.returncode))
            if r.returncode != 0:
                emit(txt[-800:])
            return r.returncode == 0
        except Exception as e:                           # noqa: BLE001 -- audio must never abort the convert
            emit('[audio] %s failed: %s' % (bat, e)); return False

    xedk = env.get('XEDK')
    have_xactbld = lambda: bool(xedk) and os.path.isfile(os.path.join(xedk, 'bin', 'win32', 'xactbld3.exe'))
    have_ffmpeg = lambda: which('ffmpeg') is not None
    # convert bats (ffmpeg) before bank bats (XACTBLD); a missing .bat is skipped gracefully by _run.
    bats = [('convert_audio.bat', have_ffmpeg,  'ffmpeg (add it to PATH)'),
            ('music_convert.bat', have_ffmpeg,  'ffmpeg (add it to PATH)'),
            ('build_banks.bat',   have_xactbld, 'XACTBLD (set %XEDK% / install the Xbox 360 SDK)'),
            ('music_banks.bat',   have_xactbld, 'XACTBLD (set %XEDK% / install the Xbox 360 SDK)')]
    ok = True
    for name, chk, hint in bats:
        if only is None or name in only:
            ok = _run(name, chk, hint) and ok
    return ok, lines
