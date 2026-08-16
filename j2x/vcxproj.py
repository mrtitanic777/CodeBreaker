# -*- coding: utf-8 -*-
"""
vcxproj.py -- regenerate the Xbox-project .vcxproj file list after a j2x code swap.

    python -m j2x vcxproj [--proj <path>]

Strips blank-line padding, removes old ClCompile/ClInclude ItemGroups, inserts fresh
ones listing the current .cpp/.h, preserving stdafx.cpp's per-config PCH block.
"""

import os
import re
import hashlib

# Ordered (regex, filter) classification: FIRST match wins, so put SPECIFIC before GENERAL.
# Purely name-based -- the decompiled corpus has consistent class-name prefixes. Filters are NESTED
# (backslash = one Solution-Explorer sub-folder level) so big subsystems break into readable sub-folders
# instead of one giant flat list. EVERY file resolves to a filter (fallback 'Game (misc)') -- a file must
# NEVER land at the project root, which is why the pipeline regenerates this on every conversion.
# The (basename regex -> filter path) rules are game-specific and come from the active target
# profile. EVERY file must resolve to a filter (fallback 'Game (misc)'), never the project root.
from . import profiles as _profiles
_FILTER_RULES = _profiles.active().vcxproj_filters


def _classify(basename, parent_of=None):
    """Assign a class to a Solution-Explorer filter.

    First try the profile's name rules -- they anchor the well-known bases and all the
    runtime/platform/Java-stdlib buckets (which have no game inheritance to key off). If a class
    name matches nothing, walk its INHERITANCE chain (recovered from the emitted headers) and adopt
    the filter of its nearest ancestor that a rule DOES match. This is the signature-driven part:
    a convention-breaking subclass (e.g. a Block descendant not named `Block*`) is filed with its
    base instead of dumped in 'Game (misc)', so the tree follows the real converted class graph."""
    for pat, filt in _FILTER_RULES:
        if re.match(pat, basename):
            return filt
    if parent_of:
        seen = set()
        cur = parent_of.get(basename)
        while cur and cur not in seen:
            seen.add(cur)
            for pat, filt in _FILTER_RULES:
                if re.match(pat, cur):
                    return filt
            cur = parent_of.get(cur)
    return 'Game (misc)'


_CLASS_DECL = re.compile(r'^\s*class\s+(\w+)\s*:\s*(?:public\s+|private\s+|protected\s+)?(\w+)', re.M)


def _build_parent_map(proj_dir):
    """Scan the emitted .h headers for `class X : public Y` and return {X: Y} (primary base only).
    This recovers the converted codebase's real inheritance graph so filters follow it -- no naming
    convention required. Forward decls (`class Foo;`) and root classes (`class Foo {`) are skipped
    (no `:` base). Works standalone, since the headers are right there in the output directory."""
    parent_of = {}
    try:
        headers = [f for f in os.listdir(proj_dir) if f.endswith('.h')]
    except OSError:
        return parent_of
    for fn in headers:
        try:
            with open(os.path.join(proj_dir, fn), 'r', encoding='utf-8', errors='replace') as fh:
                text = fh.read()
        except OSError:
            continue
        for child, base in _CLASS_DECL.findall(text):
            parent_of.setdefault(child, base)
    return parent_of


def _guid(name):
    h = hashlib.md5(('j2x-filter-' + name).encode('utf-8')).hexdigest()   # stable per filter name
    return '{%s-%s-%s-%s-%s}' % (h[:8], h[8:12], h[12:16], h[16:20], h[20:32])


def write_filters(proj_dir, proj_name, cpps, hs):
    """Emit <proj_name>.vcxproj.filters grouping every .cpp/.h into subsystem virtual folders, so
    Solution Explorer shows folder icons (Blocks, Entities, Rendering, World & Generation, Network,
    GUI, Runtime, Platform, …) instead of two flat lists. Deterministic (stable filter GUIDs)."""
    parent_of = _build_parent_map(proj_dir)                   # real inheritance graph of the output
    file_filter = {}
    filters = set()
    for fn in cpps + hs:
        filt = _classify(fn.rsplit('.', 1)[0], parent_of)
        file_filter[fn] = filt
        parts = filt.split('\\')                              # register each nesting level
        for i in range(len(parts)):
            filters.add('\\'.join(parts[:i + 1]))

    def esc(v):                                          # XML-escape filter/file names (& < >)
        return v.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')

    L = ['<?xml version="1.0" encoding="utf-8"?>',
         '<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">']
    L.append('  <ItemGroup>')
    for filt in sorted(filters):
        L.append('    <Filter Include="%s">' % esc(filt))
        L.append('      <UniqueIdentifier>%s</UniqueIdentifier>' % _guid(filt))
        L.append('    </Filter>')
    L.append('  </ItemGroup>')
    L.append('  <ItemGroup>')
    for fn in cpps:
        L.append('    <ClCompile Include="%s"><Filter>%s</Filter></ClCompile>' % (esc(fn), esc(file_filter[fn])))
    L.append('  </ItemGroup>')
    L.append('  <ItemGroup>')
    for fn in hs:
        L.append('    <ClInclude Include="%s"><Filter>%s</Filter></ClInclude>' % (esc(fn), esc(file_filter[fn])))
    L.append('  </ItemGroup>')
    L.append('</Project>')
    path = os.path.join(proj_dir, proj_name + '.vcxproj.filters')
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L) + '\n')
    return path, len(filters)

_DEFAULT_PROJ = os.path.normpath(os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    '..', 'MinecraftAlpha', 'MinecraftAlpha'))


def _find_vcxproj(proj_dir, name=None):
    """The .vcxproj to operate on: <name>.vcxproj if given, else the single .vcxproj in the dir."""
    if name:
        return os.path.join(proj_dir, name + '.vcxproj')
    cands = [f for f in os.listdir(proj_dir) if f.endswith('.vcxproj')]
    return os.path.join(proj_dir, cands[0]) if cands else os.path.join(proj_dir, 'MinecraftAlpha.vcxproj')


def run(proj_dir=None, name=None, write_filters_too=True):
    """Regenerate the .vcxproj ClCompile/ClInclude file list from the .cpp/.h in proj_dir, and (by
    default) also (re)write the subsystem <name>.vcxproj.filters so Solution Explorer shows folders."""
    proj_dir = proj_dir or _DEFAULT_PROJ
    vcx = _find_vcxproj(proj_dir, name)
    proj_name = os.path.splitext(os.path.basename(vcx))[0]

    with open(vcx, 'r', encoding='utf-8') as f:
        text = f.read()
    raw_bytes = len(text)

    text = '\n'.join(ln for ln in text.split('\n') if ln.strip() != '')

    m = re.search(r'<ClCompile Include="stdafx\.cpp">.*?</ClCompile>', text, re.S)
    stdafx_block = m.group(0) if m else None

    def drop_filelists(mt):
        block = mt.group(0)
        return '' if ('<ClCompile' in block or '<ClInclude' in block) else block
    text = re.sub(r'<ItemGroup>\s*.*?</ItemGroup>', drop_filelists, text, flags=re.S)

    cpps = sorted(f for f in os.listdir(proj_dir) if f.endswith('.cpp'))
    hs = sorted(f for f in os.listdir(proj_dir) if f.endswith('.h'))

    def cl_compile(fn):
        if fn == 'stdafx.cpp' and stdafx_block:
            return '    ' + stdafx_block + '\n'
        return '    <ClCompile Include="%s" />\n' % fn

    compile_grp = '  <ItemGroup>\n' + ''.join(cl_compile(f) for f in cpps) + '  </ItemGroup>\n'
    include_grp = '  <ItemGroup>\n' + ''.join('    <ClInclude Include="%s" />\n' % f for f in hs) + '  </ItemGroup>\n'

    # FIXED BUG assets-in-project-root: assets are emitted DIRECTLY into Debug\Content (= game:\Content next to
    # the Debug .xex) by the asset/audio/achievement tools -- so there is NO Content\ at the project root.
    # FIXED BUG release-ltcg-no-content: game: maps to $(OutDir), which DIFFERS per config (Debug\, Release\,
    # Release_LTCG\...), so a Release/LTCG build's game:\Content was EMPTY (assets only in Debug\Content). The
    # old StageContent copied $(ProjectDir)Content (never exists) -> inert. Emit a DeployContent target that
    # mirrors the canonical Debug\Content into any NON-Debug config's OutDir (Debug already has it). /D copies
    # only changed files (skips the big audio banks on rebuilds).
    import re as _re
    text = _re.sub(r'\s*<Target Name="StageContent".*?</Target>', '', text, flags=_re.S)
    text = _re.sub(r'\s*<Target Name="DeployContent".*?</Target>', '', text, flags=_re.S)
    deploy_target = (
        '  <Target Name="DeployContent" AfterTargets="Build" '
        'Condition="\'$(Configuration)\' != \'Debug\' And Exists(\'$(ProjectDir)Debug\\Content\')">\n'
        '    <Exec Command="xcopy /E /I /Y /Q /D &quot;$(ProjectDir)Debug\\Content&quot; '
        '&quot;$(OutDir)Content\\&quot;" />\n'
        '  </Target>\n'
    )

    text = text.replace('</Project>', compile_grp + include_grp + deploy_target + '</Project>')

    with open(vcx, 'w', encoding='utf-8') as f:
        f.write(text)
    msg = 'vcxproj: %d bytes -> %d bytes; %d .cpp, %d .h listed%s' % (
        raw_bytes, len(text), len(cpps), len(hs),
        '' if stdafx_block else '  (WARNING: stdafx.cpp PCH block not found)')
    if write_filters_too:
        fp, nfilt = write_filters(proj_dir, proj_name, cpps, hs)
        msg += '\nfilters: %s (%d subsystem folders)' % (os.path.basename(fp), nfilt)
    print(msg)
