# -*- coding: utf-8 -*-
r"""
sound_fix.py -- make converted ports actually play SOUND EFFECTS on the console.

Java SoundManager gates every SFX on the OGG sound pool:

    SoundPoolEntry* var7 = this->soundPoolSounds->getRandomSoundFromSoundPool(name);
    if (var7 != NULL && var5 > 0.0F) {
        ... s_sndSystem->newSource(.., var7->soundUrl, var7->soundName, ..);   // soundName -> XACT cue
        s_sndSystem->play(..);
    }

On the console there are NO OGG files -- sound comes from XACT wave/sound banks through the Audio.reference
SoundSystem->XACT bridge -- so soundPoolSounds is EMPTY, getRandomSoundFromSoundPool() returns NULL, and the
whole play block is skipped => ALL sound effects are silent. The bridge is ready; the generated game code
just never reaches it.

The proven hand-fix (MinecraftAlpha / MinecraftIndev SoundManager.cpp) defeats the guard and routes the
sound NAME straight through as the XACT cue: drop the `var7 != NULL` conjunct, and pass `(void*)NULL, name`
where the decompiled code passed `var7->soundUrl, var7->soundName`. newSource's 4th arg (`identifier`,
std::string) IS the cue key and its 3rd arg (`url`) is a void* -- see Audio.reference.h. No XACT call is
emitted into game code; the bridge does the routing.

This pass migrates that hand-fix into the GENERATOR so EVERY conversion gets working SFX from ONE place --
the fragmentation it replaces: Alpha/Indev hand-patched it in their trees, while Beta shipped a full-file
SoundManager overlay that FORGOT it and stayed silent. It keys on the paulscode `soundPoolSounds->
getRandomSoundFromSoundPool` guard STRUCTURE, not on method names/signatures, so a single rule covers
Indev/Alpha/Beta (whose enclosing method signatures differ) and any future version keeping the paulscode
SoundManager.

SCOPE -- deliberately narrow, to match exactly what the proven hand-fix changes:
  * SFX only: the `soundPoolSounds` pool (playSound / playSoundFX).
  * soundPoolStreaming (ambient/records) is LEFT ALONE -- the hand-fix left it guarded; forcing it would
    turn looping streams into non-looping one-shots.
  * soundPoolMusic (getRandomSound) is LEFT ALONE -- the music path has a different, NULL-deref-prone shape
    and different XACT routing; it needs its own targeted fix, not this SFX rule.

Runs as a post-emit pass over the converted tree (xbox360), BEFORE overlays, alongside reflection_factory.
Idempotent: after the rewrite the guard no longer contains `!= NULL`, so a re-run matches nothing. Also
exposed as `j2ctool sound-fix <dir>`.
"""

# FIXED BUG sfx-bypass: the converted SoundManager gates every sound effect on the empty console OGG pool
# (getRandomSoundFromSoundPool != NULL) -> ALL sound effects silent; this pass drops the guard and routes the
# sound name to the XACT bridge. Migrated here from per-game SoundManager overlays so every convert gets it.
# FIXED BUG music-bypass: playRandomMusic's play("BgMusic") (which starts the XACT playlist) sits inside the
# same empty-pool guard -> background music never starts; this pass makes the play() path unconditional,
# keeping only the SoundPoolEntry deref (backgroundMusic) guarded.

import os
import re

# The SFX guard site the generator emits:
#     SoundPoolEntry* <V> = <recv>->soundPoolSounds->getRandomSoundFromSoundPool(<NAME>);
#     if(<V> != NULL[ && <EXTRA>]) {
# <recv> is usually `this` but kept permissive ([^;]*?). <NAME> is the sound-name argument (a simple
# identifier). ANCHORED to `soundPoolSounds` so the streaming (soundPoolStreaming) and music
# (soundPoolMusic->getRandomSound) sites are NOT matched.
_SFX_SITE = re.compile(
    r'(SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolSounds\s*->\s*'
    r'getRandomSoundFromSoundPool\s*\(\s*(\w+)\s*\)\s*;)'
    r'(\s*)if\s*\(\s*\2\s*!=\s*NULL\s*(?:&&\s*([^)]*?)\s*)?\)')

_NOTE = ('   // J2X sfx-bypass: console OGG pool is empty; drop the null-guard and route the sound name to '
         'the XACT bridge (newSource identifier == cue). [generator: sound_fix.py]')

# fail-loud: a soundPoolSounds SFX guard still gated on `!= NULL` after the pass ran (a future decompile
# whose guard shape changed) -- signals a silent-SFX regression that this rule no longer catches.
_SURVIVOR = re.compile(r'soundPoolSounds\s*->\s*getRandomSoundFromSoundPool[^;]*;\s*if\s*\([^)]*!=\s*NULL')

# ---- MUSIC (playRandomMusic) -----------------------------------------------------------------------------
# The music site the generator emits:
#     SoundPoolEntry* <V> = <recv>->soundPoolMusic->getRandomSound();
#     if(<V> != NULL) { <BODY: flat statements, no nested braces> }
# On console soundPoolMusic is EMPTY, so <V>==NULL and the whole block (incl. play("BgMusic")) is skipped ->
# the XACT music playlist never starts. UNLIKE SFX, the fix is NOT (void*)NULL routing: the proven Alpha
# playRandomMusic hand-fix hoists play/setVolume/delay OUT of the guard so play("BgMusic") (which starts the
# XACT playlist / StartMusic) fires UNCONDITIONALLY, while backgroundMusic -- the only call that needs a real
# SoundPoolEntry -- stays guarded and is simply skipped. We reproduce that behaviour more robustly by making
# the outer guard `if(true)` and inline-guarding ONLY the <V>-> deref line, no statement code-motion.
# Requires a flat (no nested-brace) block, which the paulscode playRandomMusic body is.
_MUSIC_SITE = re.compile(
    r'(SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;)'
    r'(\s*)if\s*\(\s*\2\s*!=\s*NULL\s*\)\s*\{([^{}]*)\}')

_MUSIC_NOTE = ('   // J2X music-bypass: console music pool is empty; run the play() path unconditionally so '
               'the XACT playlist starts, keeping only the SoundPoolEntry deref (backgroundMusic) guarded. '
               '[generator: sound_fix.py]')

# fail-loud twin for music: a soundPoolMusic guard still gated on `!= NULL` immediately after the decl.
_MUSIC_SURVIVOR = re.compile(r'soundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;\s*if\s*\([^)]*!=\s*NULL')


def process(text):
    """Rewrite the SFX (soundPoolSounds) and music (soundPoolMusic) empty-pool guards in one .cpp.
    Returns (new_text, n_sfx, n_music)."""
    sfx = []

    def _sfx_repl(m):
        decl, V, NAME, ws, extra = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        guard = extra.strip() if (extra and extra.strip()) else 'true'   # keep any volume gate; else run always
        sfx.append((V, NAME))
        # DROP the empty-pool query itself, don't just ignore its result. On console soundPoolSounds is empty
        # and getRandomSoundFromSoundPool(absent) HANGS: JMap.get(absent) returns a NON-null garbage value,
        # JList::operator==(long long) is hardcoded false so the `<V> == NULL` guard never fires, and it reaches
        # `list[rand.nextInt(garbageSize)]` where nextInt(negative bound) infinite-loops. <V> is unused after the
        # guard becomes if(true) (SFX routes the NAME to the XACT cue), so declare it NULL instead of querying.
        return 'SoundPoolEntry* %s = NULL;%s%sif(%s)' % (V, _NOTE, ws, guard)

    new = _SFX_SITE.sub(_sfx_repl, text)
    # SFX: kill the now-NULL SoundPoolEntry derefs for each rewritten site -- the entry is NULL on console, so
    # route (void*)NULL as the url and the sound NAME as the cue identifier (matches the hand-fix). <V> is a
    # unique per-site local, so a whole-file replace of `<V>->soundUrl/soundName` only touches this method.
    for V, NAME in sfx:
        new = re.sub(r'\b%s\b\s*->\s*soundUrl\b'  % re.escape(V), '(void*)NULL', new)
        new = re.sub(r'\b%s\b\s*->\s*soundName\b' % re.escape(V), NAME,         new)

    # MUSIC: make the outer guard unconditional (play("BgMusic") -> XACT playlist fires) and inline-guard only
    # the SoundPoolEntry deref line (backgroundMusic), which is simply skipped on the empty console pool.
    n_music = [0]

    def _music_repl(m):
        decl, V, ws, body = m.group(1), m.group(2), m.group(3), m.group(4)
        newbody = re.sub(r'(\n[ \t]*)([^\n]*\b%s\s*->[^\n]*;)' % re.escape(V),
                         r'\1if(%s != NULL) \2' % V, body)
        n_music[0] += 1
        return '%s%s%sif(true) {%s}' % (decl, _MUSIC_NOTE, ws, newbody)

    new = _MUSIC_SITE.sub(_music_repl, new)
    return new, len(sfx), n_music[0]


def run(out_dir):
    """Apply the SFX + music bypass across the converted tree. Returns
    {'files': [{'file','sfx','music'}...], 'sites': N, 'stranded': [files]}. `stranded` = files where a
    soundPool*->getRandomSound* null-guard SURVIVED the rewrite (fail-loud)."""
    files = [f for f in sorted(os.listdir(out_dir)) if f.endswith('.cpp')]
    touched, total, stranded = [], 0, []
    for f in files:
        path = os.path.join(out_dir, f)
        try:
            text = open(path, 'r', encoding='utf-8', errors='replace').read()
        except (IOError, OSError):
            continue
        if 'getRandomSound' not in text or ('soundPoolSounds' not in text and 'soundPoolMusic' not in text):
            continue
        new, n_sfx, n_music = process(text)
        if new != text:
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new)
            touched.append({'file': f, 'sfx': n_sfx, 'music': n_music})
            total += n_sfx + n_music
        if _SURVIVOR.search(new) or _MUSIC_SURVIVOR.search(new):
            stranded.append(f)
    return {'files': touched, 'sites': total, 'stranded': stranded}
