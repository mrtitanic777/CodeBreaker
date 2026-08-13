# -*- coding: utf-8 -*-
r"""
project_scan.py -- from one folder the user picks, resolve everything the conversion needs.

A decompiled Minecraft tree keeps its parts in predictable places under a base folder, e.g.

    <base>\jars\deobfuscated.jar          <- textures/resource jar
    <base>\game\resources\newsound\...    <- the .ogg music/sound tree
    <base>\src\*.java  (or deobfuscated-source.jar)   <- the Java source

Hand-selecting each of those in the wizard is tedious, so given ONE folder (the base, or the src inside
it) this finds the base and resolves the jar, the resources/music dir, the Java source, and the language
file -- so the later questionnaires (Assets jar, Audio resources, Achievements lang) come pre-filled.
The scan is depth-bounded and skips heavy build folders so it stays fast.
"""

import os

_JAR_PREFERRED = ('deobfuscated.jar', 'minecraft.jar')
_SKIP_DIRS = {'.git', '.svn', 'node_modules', 'bin', 'obj', 'debug', 'release',
              'release_ltcg', 'ipch', '.vs', 'x64', 'x86'}
# a dir is a plausible base if it directly contains one of these
_BASE_MARKERS = ('jars', 'game', 'resources')


def _listdir(d):
    try:
        return os.listdir(d)
    except OSError:
        return []


def find_base(project_dir):
    """Walk up from `project_dir` and return the CLOSEST folder that looks like the decompiled base
    (contains jars/ or game/ or resources/). Falls back to `project_dir`."""
    d = os.path.abspath(project_dir)
    for _ in range(8):
        low = set(x.lower() for x in _listdir(d))
        if any(m in low for m in _BASE_MARKERS):
            return d                                      # nearest match wins (don't overshoot upward)
        parent = os.path.dirname(d)
        if parent == d:
            break
        d = parent
    return os.path.abspath(project_dir)


def _walk(root, maxdepth=5):
    root = os.path.abspath(root)
    base = root.rstrip(os.sep).count(os.sep)
    for dp, dirs, files in os.walk(root):
        if dp.count(os.sep) - base >= maxdepth:
            dirs[:] = []
        dirs[:] = [x for x in dirs if x.lower() not in _SKIP_DIRS]
        yield dp, dirs, files


def find_jar(base):
    """The textures/resource jar: prefer deobfuscated.jar / a jar under a 'jars' dir; never a
    *-source.jar."""
    fallback = None
    for dp, _dirs, files in _walk(base, 5):
        in_jars = os.path.basename(dp).lower() == 'jars'
        for f in files:
            fl = f.lower()
            if not fl.endswith('.jar') or 'source' in fl:
                continue
            p = os.path.join(dp, f)
            if fl in _JAR_PREFERRED:
                return p
            if in_jars and fallback is None:
                fallback = p
            elif fallback is None:
                fallback = p
    return fallback


def find_resources(base):
    """The audio/resource tree: a dir named 'resources' that holds a newsound/ or sound/ (or music/)
    child -- i.e. the .ogg tree."""
    for dp, dirs, _files in _walk(base, 5):
        if os.path.basename(dp).lower() == 'resources':
            low = set(x.lower() for x in dirs)
            if low & {'newsound', 'sound', 'newmusic', 'music', 'streaming'}:
                return dp
    return None


def find_java_src(project_dir, base):
    """The Java source ROOT (the folder the packages live under, e.g. `src` -- NOT a deep package like
    src/net/minecraft/client). Prefers a src/ root under the base; else the picked dir if it holds any
    .java beneath it."""
    def has_java(d):
        if not os.path.isdir(d):
            return False
        for _dp, _dirs, files in _walk(d, 6):
            if any(f.endswith('.java') for f in files):
                return True
        return False
    for name in ('src', 'source', 'sources'):        # the package root, not a deep package
        cand = os.path.join(base, name)
        if has_java(cand):
            return cand
    if has_java(project_dir):
        return os.path.abspath(project_dir)
    return None


def find_lang(base):
    """A language file: **/lang/en_US.lang (or the first *.lang under a lang/ dir)."""
    first = None
    for dp, _dirs, files in _walk(base, 6):
        if os.path.basename(dp).lower() == 'lang':
            for f in files:
                if f.lower().endswith('.lang'):
                    p = os.path.join(dp, f)
                    if f.lower() == 'en_us.lang':
                        return p
                    if first is None:
                        first = p
    return first


def resolve(project_dir):
    """Resolve everything from the picked folder. Returns a dict with (possibly None) values:
    base, jar, resources, java_src, lang."""
    base = find_base(project_dir)
    return {
        'base': base,
        'jar': find_jar(base),
        'resources': find_resources(base),
        'java_src': find_java_src(project_dir, base),
        'lang': find_lang(base),
    }
