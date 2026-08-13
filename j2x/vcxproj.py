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

# Runtime-support / external-stub file basenames (no .h) -> the "Runtime" filter.
_RUNTIME = {'JavaRT', 'JavaString', 'JArray', 'JList', 'JObject', 'JCat', 'Class',
            'j2x_init', 'stdafx'}

# Ordered (regex, filter) classification: FIRST match wins, so put specific before general.
# Purely name-based -- the decompiled corpus has consistent class-name prefixes.
_FILTER_RULES = [
    # j2x-emitted glue -- give it its own findable folders. Debug first so the single debug file
    # is trivial to locate in Solution Explorer.
    (r'^Debug$', 'Debug'),
    (r'^(Kernel|KernelConfig|ControllerConfig)$', 'Platform\\Config'),
    (r'^(JavaRT|JavaString|JArray|JList|JObject|JCat|j2x_init|stdafx)$', 'Runtime'),
    (r'^(GL\d*|GLU|GLContext|Keyboard|Mouse|Display|DisplayMode|Controllers|ARB\w+|BufferUtils|Sys)$', 'Platform\\LWJGL-GL'),
    (r'^(Socket|File\w*|Buffered\w*|Data(Input|Output)\w*|InputStream|OutputStream|Print\w*|Zip\w*|URL\w*|ImageIO|Graphics|Canvas|Applet|Frame|Panel|Component|Cursor|Robot|Color|Dimension|Pattern|Matcher)$', 'Platform\\Java-stdlib stubs'),
    (r'^(Packet\w*|NetworkManager|Net(Server|Client)\w*|TcpConnection|Tcp\w*|MemoryConnection|Thread\w*)$', 'Network'),
    (r'^Gui\w*|^FontRenderer$|^ScaledResolution$|^LoadingScreen\w*', 'GUI'),
    (r'^TileEntity\w*', 'TileEntities'),
    (r'^Enum\w*', 'Enums'),
    (r'^Block\w*|^Material$|^StepSound$', 'Blocks'),
    (r'^Item\w*|^ItemStack$|^CraftingManager$|^\w*Recipe\w*|^Slot\w*|^Container\w*|^Inventory\w*', 'Items & Inventory'),
    (r'^(Render\w*|Model\w*|Tessellator|Textures?\w*|TexturePack\w*|\w+FX|EffectRenderer|IconCoord)$', 'Rendering'),
    (r'^Entity\w*|^\w*Entity$|^MobSpawner\w*|^Path\w*', 'Entities'),
    (r'^(World\w*|Chunk\w*|BiomeGen\w*|MapGen\w*|NoiseGen\w*|WorldGen\w*|Spawner\w*|IChunk\w*|IBlockAccess|NextTickListEntry)$', 'World & Generation'),
    (r'^(NBT\w*|\w*Stream|Compressed\w*|RegionFile\w*|\w*ChunkLoader|Inflate|Deflate|\w*Storage|Save\w*|AnvilSave\w*|\w*Cache)$', 'IO & NBT'),
    (r'^(Sound\w*|Music\w*|Audio\w*|Codec\w*)$', 'Audio'),
    (r'^Colorizer\w*|^\w*Colorizer$', 'Rendering'),
    (r'^(MathHelper|Vec3D\w*|AxisAlignedBB|MovingObjectPosition|Random|Direction|ChunkCoordinates\w*|Facing|WeightedRandom\w*)$', 'Math & Util'),
    (r'^(Minecraft|MinecraftApplet|Session|Timer|Main|StatList|Achievement\w*|Stat\w*)$', 'Core'),
]


def _classify(basename):
    for pat, filt in _FILTER_RULES:
        if re.match(pat, basename):
            return filt
    return 'Game (misc)'


def _guid(name):
    h = hashlib.md5(('j2x-filter-' + name).encode('utf-8')).hexdigest()   # stable per filter name
    return '{%s-%s-%s-%s-%s}' % (h[:8], h[8:12], h[12:16], h[16:20], h[20:32])


def write_filters(proj_dir, proj_name, cpps, hs):
    """Emit <proj_name>.vcxproj.filters grouping every .cpp/.h into subsystem virtual folders, so
    Solution Explorer shows folder icons (Blocks, Entities, Rendering, World & Generation, Network,
    GUI, Runtime, Platform, …) instead of two flat lists. Deterministic (stable filter GUIDs)."""
    file_filter = {}
    filters = set()
    for fn in cpps + hs:
        filt = _classify(fn.rsplit('.', 1)[0])
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

    text = text.replace('</Project>', compile_grp + include_grp + '</Project>')

    with open(vcx, 'w', encoding='utf-8') as f:
        f.write(text)
    msg = 'vcxproj: %d bytes -> %d bytes; %d .cpp, %d .h listed%s' % (
        raw_bytes, len(text), len(cpps), len(hs),
        '' if stdafx_block else '  (WARNING: stdafx.cpp PCH block not found)')
    if write_filters_too:
        fp, nfilt = write_filters(proj_dir, proj_name, cpps, hs)
        msg += '\nfilters: %s (%d subsystem folders)' % (os.path.basename(fp), nfilt)
    print(msg)
