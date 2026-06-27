#!/usr/bin/env python3
"""Regenerate the Xbox-project .vcxproj file list after the j2x code swap.

The existing .vcxproj is ~50 MB of blank-line padding (real content ~1.5k lines). This:
  1) strips blank-only lines (de-bloat),
  2) removes the old ClCompile / ClInclude <ItemGroup>s (the file lists),
  3) inserts fresh ones listing the CURRENT .cpp/.h in the project dir,
     preserving stdafx.cpp's per-config Create-PCH block verbatim,
and leaves every other ItemGroup (ProjectConfigurations) + ItemDefinitionGroup (compiler/linker
settings, /GR RTTI, libs, _XBOX) untouched. Run: python tools/regen_xbox_vcxproj.py
"""
import os
import re

PROJ = r"C:/Users/mrtit/Documents/Visual Studio 2010/Projects/MinecraftAlpha/MinecraftAlpha"
VCX = os.path.join(PROJ, "MinecraftAlpha.vcxproj")


def main():
    with open(VCX, 'r', encoding='utf-8') as f:
        text = f.read()
    raw_bytes = len(text)
    # 1) de-bloat: drop blank/whitespace-only lines
    text = '\n'.join(ln for ln in text.split('\n') if ln.strip() != '')

    # capture stdafx.cpp's <ClCompile ...>...</ClCompile> block (the PCH=Create per-config settings)
    m = re.search(r'<ClCompile Include="stdafx\.cpp">.*?</ClCompile>', text, re.S)
    stdafx_block = m.group(0) if m else None

    # 2) remove the file-list ItemGroups (bare <ItemGroup> containing ClCompile or ClInclude).
    #    ProjectConfigurations uses <ItemGroup Label=...> so it is NOT matched by bare <ItemGroup>.
    def drop_filelists(mt):
        block = mt.group(0)
        return '' if ('<ClCompile' in block or '<ClInclude' in block) else block
    text = re.sub(r'<ItemGroup>\s*.*?</ItemGroup>', drop_filelists, text, flags=re.S)

    # 3) build fresh file lists from the current project dir
    cpps = sorted(f for f in os.listdir(PROJ) if f.endswith('.cpp'))
    hs = sorted(f for f in os.listdir(PROJ) if f.endswith('.h'))

    def cl_compile(fn):
        if fn == 'stdafx.cpp' and stdafx_block:
            return '    ' + stdafx_block + '\n'
        return '    <ClCompile Include="%s" />\n' % fn

    compile_grp = '  <ItemGroup>\n' + ''.join(cl_compile(f) for f in cpps) + '  </ItemGroup>\n'
    include_grp = '  <ItemGroup>\n' + ''.join('    <ClInclude Include="%s" />\n' % f for f in hs) + '  </ItemGroup>\n'

    # insert the two new ItemGroups just before </Project>
    text = text.replace('</Project>', compile_grp + include_grp + '</Project>')

    with open(VCX, 'w', encoding='utf-8') as f:
        f.write(text)
    print('vcxproj: %d bytes -> %d bytes; %d .cpp, %d .h listed%s'
          % (raw_bytes, len(text), len(cpps), len(hs),
             '' if stdafx_block else '  (WARNING: stdafx.cpp PCH block not found)'))


if __name__ == '__main__':
    main()
