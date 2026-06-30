# -*- coding: utf-8 -*-
"""
vcxproj.py -- regenerate the Xbox-project .vcxproj file list after a j2x code swap.

    python -m j2x vcxproj [--proj <path>]

Strips blank-line padding, removes old ClCompile/ClInclude ItemGroups, inserts fresh
ones listing the current .cpp/.h, preserving stdafx.cpp's per-config PCH block.
"""

import os
import re

_DEFAULT_PROJ = os.path.normpath(os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    '..', 'MinecraftAlpha', 'MinecraftAlpha'))


def run(proj_dir=None):
    proj_dir = proj_dir or _DEFAULT_PROJ
    vcx = os.path.join(proj_dir, 'MinecraftAlpha.vcxproj')

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

    text = text.replace('</Project>', compile_grp + include_grp + '</Project>')

    with open(vcx, 'w', encoding='utf-8') as f:
        f.write(text)
    print('vcxproj: %d bytes -> %d bytes; %d .cpp, %d .h listed%s'
          % (raw_bytes, len(text), len(cpps), len(hs),
             '' if stdafx_block else '  (WARNING: stdafx.cpp PCH block not found)'))
