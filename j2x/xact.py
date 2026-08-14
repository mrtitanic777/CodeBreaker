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
    from . import content
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
def build_xap(java_dir, out_path, wave_dir='wav', bank='sounds'):
    """Generate an XACT3 project (.xap) that AUTHORS every cue the game needs, each referencing a
    .wav in `wave_dir`. XACTBLD compiles it to <bank>.xwb + <bank>.xsb. Returns the cue list."""
    cues = sorted(needed_cues(java_dir))
    L = ['Signature = XACT3;', 'Version = 18;', 'Content Version = 46;', 'Release = March 2008;', '',
         'Options', '{', '    Verbose Report = 0;', '    Generate C/C++ Headers = 1;', '}', '',
         'Global Settings', '{', '    Xbox File = Content\\Audio\\Global.xgs;',
         '    Xbox Header File = Global.h;', '    Category { Name = Global; Volume = 0; }',
         '    Category { Name = Default; Volume = 0; Parent = Global; }',
         '    Category { Name = Music; Volume = 0; Parent = Global; }', '}', '',
         'Wave Bank', '{', '    Name = %s;' % bank, '    Xbox File = Content\\Audio\\%s.xwb;' % bank,
         '    Xbox Bank Path Edited = 1;', '    Compression Preset Name = XWMA_Q3;', '']
    for c in cues:
        L += ['    Wave', '    {', '        Name = %s;' % c,
              '        File = %s\\%s.wav;' % (wave_dir, c), '    }', '']
    L += ['}', '', 'Sound Bank', '{', '    Name = %s;' % bank,
          '    Xbox File = Content\\Audio\\%s.xsb;' % bank, '    Xbox Header File = %s.h;' % bank, '']
    for c in cues:
        L += ['    Sound', '    {', '        Name = %s;' % c, '        Category Name = Default;',
              '        Track { Play Wave Event { Wave Entry { Name = %s; } } }' % c, '    }', '']
    for c in cues:                                       # cue keyed by the DOTTED event so toCue() maps back
        ev = c.replace('_', '.')
        L += ['    Cue', '    {', '        Name = %s;' % ev,
              '        Sound Entry { Name = %s; }' % c, '    }', '']
    L += ['}', '']
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
    return cues


def write_build_script(out_dir, bank='sounds'):
    """Emit build_banks.bat: run the XDK XACTBLD to compile <bank>.xap -> .xwb/.xsb for the 360."""
    bat = ('@echo off\r\n'
           'rem Build the XACT audio banks for Xbox 360 (needs the Xbox 360 SDK on PATH).\r\n'
           'set XACTBLD="%%XEDK%%\\bin\\win32\\xactbld3.exe"\r\n'
           '%%XACTBLD%% /L /X:%s.xap .\r\n'
           'echo Built %s.xwb / %s.xsb into Content\\Audio\\\r\n' % (bank, bank, bank))
    p = os.path.join(out_dir, 'build_banks.bat')
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


def write_ffmpeg_script(match, out_dir, resources_dir, wav_subdir='wav'):
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
    p = os.path.join(out_dir, 'convert_audio.bat')
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


def build(java_dir, out_dir, resources_dir=None, autorun=False, sdk_root=None, log=None):
    """One-shot XACT3 audio build for a converted game: write the .xap project, the XACTBLD build script,
    the ogg->wav ffmpeg script, and AUDIO_COVERAGE.md. When `autorun` is set, ALSO run the scripts
    (convert_audio.bat then build_banks.bat) so the .xwb/.xsb banks are produced as part of conversion --
    no manual step. `log` is an optional callable(str) for progress. Returns (xap_path, cues)."""
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    xap = os.path.join(out_dir, 'sounds.xap')
    cues = build_xap(java_dir, xap)
    write_build_script(out_dir)
    resources = resources_dir or _find_resources_dir(java_dir)
    if resources:                                        # auto-emit the ogg->wav decode script if we can find the sources
        write_ffmpeg_script(match_sources(java_dir, resources), out_dir, resources)
    with open(os.path.join(out_dir, 'AUDIO_COVERAGE.md'), 'w', encoding='utf-8') as f:
        cov_needed = needed_cues(java_dir)
        f.write('# AUDIO cues the game needs (%d)\n\nAuthor a .wav per cue under `wav/`, then run '
                '`build_banks.bat` (XACTBLD).\n\n' % len(cov_needed)
                + '\n'.join('- `%s.wav`  <- %s' % (c, ', '.join(v[:2])) for c, v in sorted(cov_needed.items())) + '\n')
    if autorun:
        run_build(out_dir, sdk_root=sdk_root, log=log)
    return xap, cues


# J2X-BUGFIX 2026-08-13 audio-never-built: the wizard audio step only wrote convert_audio.bat and never
# generated the .xap/build_banks.bat, so the XACT banks were never produced -> no game audio. build() now
# runs the FULL XACT3 build, and run_build() (below) AUTORUNS both bats (ffmpeg then XACTBLD), skipping
# gracefully if a tool is missing. Content path is Content\Audio\ (not Content\resources).
def run_build(out_dir, sdk_root=None, log=None):
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
            r = subprocess.Popen(['cmd', '/c', bat], cwd=out_dir, env=env,
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
    ok1 = _run('convert_audio.bat', lambda: which('ffmpeg') is not None, 'ffmpeg (add it to PATH)')
    ok2 = _run('build_banks.bat', have_xactbld, 'XACTBLD (set %XEDK% / install the Xbox 360 SDK)')
    return (ok1 and ok2), lines
