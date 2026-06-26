#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
regen_vcxproj.py — Rewrite a .vcxproj's source file lists to match the .cpp/.h actually on disk.

After dropping a fresh converted code set into a project folder, the project's ClCompile/ClInclude
ItemGroups are stale (they list the OLD file set). This scans the project dir and rebuilds those two
ItemGroups (everything else — configs, build settings, the <None> group — is left untouched). stdafx.cpp
keeps its per-config 'Create' precompiled-header block.

Usage:  python regen_vcxproj.py <path-to.vcxproj>
        (the .cpp/.h are read from the directory the .vcxproj lives in)
"""
import os
import re
import sys

CONFIGS = ['CodeAnalysis', 'Debug', 'Profile', 'Profile_FastCap', 'Release', 'Release_LTCG']


def build_groups(proj_dir):
    heads = sorted(f for f in os.listdir(proj_dir) if f.lower().endswith('.h'))
    cpps = sorted(f for f in os.listdir(proj_dir) if f.lower().endswith('.cpp'))

    inc = ['  <ItemGroup>']
    for h in heads:
        inc.append('    <ClInclude Include="%s" />' % h)
    inc.append('  </ItemGroup>')

    comp = ['  <ItemGroup>']
    for c in cpps:
        if c.lower() == 'stdafx.cpp':
            comp.append('    <ClCompile Include="%s">' % c)
            for cfg in CONFIGS:
                comp.append("      <PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=="
                            "'%s|Xbox 360'\">Create</PrecompiledHeader>" % cfg)
            comp.append('    </ClCompile>')
        else:
            comp.append('    <ClCompile Include="%s" />' % c)
    comp.append('  </ItemGroup>')
    return '\n'.join(inc), '\n'.join(comp), len(heads), len(cpps)


def main():
    if len(sys.argv) < 2:
        print('Usage: python regen_vcxproj.py <path-to.vcxproj>')
        sys.exit(1)
    proj = sys.argv[1]
    if not os.path.isfile(proj):
        print('ERROR: not found: %s' % proj)
        sys.exit(1)
    proj_dir = os.path.dirname(os.path.abspath(proj))
    with open(proj, 'r', encoding='utf-8') as f:
        content = f.read()

    inc_group, comp_group, nh, nc = build_groups(proj_dir)

    # Replace the existing ClInclude ItemGroup and ClCompile ItemGroup (leave the <None> group alone).
    new, n1 = re.subn(r'[ \t]*<ItemGroup>\s*<ClInclude\b.*?</ItemGroup>', inc_group, content,
                      count=1, flags=re.S)
    new, n2 = re.subn(r'[ \t]*<ItemGroup>\s*<ClCompile\b.*?</ItemGroup>', comp_group, new,
                      count=1, flags=re.S)
    if not n1 or not n2:
        print('ERROR: could not locate ClInclude (%d) / ClCompile (%d) ItemGroups' % (n1, n2))
        sys.exit(2)

    bak = proj + '.bak'
    if not os.path.exists(bak):
        with open(bak, 'w', encoding='utf-8') as f:
            f.write(content)
    # Preserve CRLF (MSVC) line endings.
    new = new.replace('\r\n', '\n').replace('\n', '\r\n')
    with open(proj, 'w', encoding='utf-8') as f:
        f.write(new)
    print('Regenerated %s: %d ClInclude (.h) + %d ClCompile (.cpp). Backup: %s'
          % (os.path.basename(proj), nh, nc, os.path.basename(bak)))


if __name__ == '__main__':
    main()
