"""gen_golden.py -- how the golden files next to this script were made (docs/CPP_DESIGN.md 7.1).

The console lane's C++ modules (native/cb/java/console) replaced Python modules of the same names. The goldens are the
ORIGINAL Python's answers on the fixtures in this folder, written in the plain-text forms the C++ tests render the same
way; each was read by a person and found right before it was checked in. Nothing here runs in the build: run it
from the repository root of a checkout that still has the Python package:

    python native/cb/java/console/testdata/gen_golden.py [xdk scan worklist vcxproj layout xact assets]

It (re)writes the fixtures that are generated (jars, banks, wave files) and every *.golden.* file. Line ends in
goldens are the Windows ones the Python wrote; the tests compare after normalising them.
"""
import hashlib
import io
import json
import os
import re
import shutil
import struct
import sys
import wave
import zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, *([".."] * 5)))
sys.path.insert(0, ROOT)


def w(path, data):
    path = os.path.join(HERE, path)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data if isinstance(data, bytes) else data.encode("utf-8"))


def golden(name, text):
    with open(os.path.join(HERE, name), "wb") as f:
        f.write(text.replace("\r\r\n", "\r\n").encode("utf-8"))


def rel(p, base):
    return os.path.relpath(p, base).replace("\\", "/") if p else "-"


# ---------------------------------------------------------------------------------------------------- xdk_scan
def gen_xdk():
    from codebreaker.java.console import xdk_scan
    inc = "xdk/sdk1/include/xbox/"
    w(inc + "xam.h", "XamLoaderLaunchTitle(a);\nXamUserGetSigninState (0);\nXContentCreate(x);\nXContentFlush(x);\n"
                     "XContentClose(x);\nXShowDeviceSelectorUI(1);\n")
    w(inc + "xinput.h", "XInputSetState(0,&v); XInputGetState(0,&s);\n// XShowKeyboardUI (async)\nXamLoaderLaunchTitle (dup);\n"
                        "XMPGetStatus(&st);\nXMPOpenOverlappedEnumerator(1);\n")
    w(inc + "d3dx.h", "D3DXMatrixIdentity(&m);\nD3DXVec3Normalize (&v);\nMmAllocatePhysicalMemory(1);\n"
                       "KeQuerySystemTime(&t);\nNtCreateFile(&h);\nVdSwap(1);\nXPhysicalAlloc(1,2);\nXPhysicalFree(p);\n")
    w(inc + "notes.txt", "XContentSecret(1);\n")
    w(inc + "BIG.H", "XUpperCase(1);\n")
    w(inc + "bad.h", b"XBadBytes(1); \xff\xfe\nXAfterBad(2);\nXNotifyGetNext(a);\nXNetStartup(b);\n")
    w(inc + "empty.h", "")
    w(inc + "crlf.h", "XLine\r\nBreak(3);\nXCrlf\r\n(4);\n")
    w(inc + "sub/deep.h", "XDeepFunction(1);\nXMemCompress(a);\nXMemDecompress(b);\nXShowMessageBoxUI(c);\nXGetLanguage();\n"
                          "XGetVideoMode(&m);\nXJSONCreateReader(&r);\nXJSONParse (r);\n")
    w(inc + "sub/more/x.h", "X(1);\nXa(2);\nXB(3);\nXBo(4);\nXx12(5);\nXamy(6);\nXam1(7);\nKe(8);\nKeA(9);\nNtZ_(10);\n")
    w("xdk/sdk2/include/foo.h", "XamOnlyInInclude(1);\nXShowKeyboardUI(2);\n")
    w("xdk/sdk3/root.h", "XamAtRoot(1);\nXGetLanguage(2);\nXGetLanguageFull(3);\n")
    w("xdk/emptydir/.gitkeep", b"")   # (a folder that git keeps: it has a file)

    base = os.path.join(HERE, "xdk")
    out = []
    for r in ["sdk1", "sdk2", "sdk3", "emptydir", "nonexistent", "sdk1/include/xbox/bad.h", None, ""]:
        root = os.path.join(base, r) if r else r
        res = xdk_scan.detect(root)
        out.append("detect %s" % (r if r is not None else "<none>") if r != "" else "detect <empty>")
        out.append("  include %s" % rel(res["include"], base))
        for name, header in sorted(res["funcs"].items()):
            out.append("  func %s %s" % (name, header))
        for tag, lst in (("found", res["found"]), ("missing", res["missing"])):
            for p in lst:
                if tag == "found":
                    out.append("  found %s header=%s count=%d fns=%s" % (p["needs"], p["header"], p["func_count"], ",".join(p["functions"])))
                else:
                    out.append("  missing %s" % p["needs"])
    golden("xdk.golden.txt", "\n".join(out) + "\n")


# --------------------------------------------------------------------------------------------- project_scan
def gen_scan():
    from codebreaker.java.console import project_scan
    base = os.path.join(HERE, "scan")
    shutil.rmtree(base, ignore_errors=True)
    for f in ["proj1/jars/deobfuscated.jar", "proj1/jars/other.jar", "proj1/jars/zz-source.jar", "proj1/bin/skipped.jar",
              "proj1/game/resources/newsound/step/grass1.ogg", "proj1/game/resources/lang/de_DE.lang",
              "proj1/game/resources/lang/en_US.lang", "proj1/src/net/minecraft/client/Main.java",
              "proj2/stuff/library.jar", "proj2/resources/sound/x.ogg", "proj2/resources/lang/fr_FR.lang",
              "proj2/sources/a/b/C.java", "proj3/game/readme.txt", "proj3/net/Foo.java", "proj3/deep/a/b/c/d/e/f/g/Deep.java",
              "proj4/DEOBFUSCATED.JAR", "proj4/jars/minecraft.jar", "proj4/resources/Music/x.ogg"]:
        w("scan/" + f, b"")
    out = []
    for d in ["proj1", "proj1/src/net/minecraft/client", "proj1/game/resources/lang", "proj2", "proj2/sources/a", "proj3",
              "proj3/net", "proj3/deep", "proj4"]:
        p = os.path.join(base, d)
        r = project_scan.resolve(p)
        out.append("resolve %s" % d)
        for k in ("base", "jar", "resources", "java_src", "lang"):
            out.append("  %s %s" % (k, rel(r[k], base) if r[k] else "-"))
    golden("scan.golden.txt", "\n".join(out) + "\n")


# ----------------------------------------------------------------------------------------------- worklist
def gen_worklist():
    from codebreaker.java.console import worklist
    d = os.path.join(HERE, "worklist")
    shutil.rmtree(d, ignore_errors=True)
    # a stand-in compiler: its stderr is the text of the "source" file it is given (the last argument)
    w("worklist/fakecc.cmd", '@echo off\r\nfor %%a in (%*) do set LAST=%%~a\r\nset LAST=%LAST:/=\\%\r\ntype "%LAST%" 1>&2\r\n')
    w("worklist/out/ok.cpp", "// nothing wrong\n")
    w("worklist/out/two.cpp", "two.cpp:3:1: error: unknown type name 'Foo'\ntwo.cpp:9:1: error: unknown type name 'Bar'\n"
                              "two.cpp:12:5: error: expected ';' after expression\ntwo.cpp:12:5: note: here\n")
    w("worklist/out/one.cpp", "one.cpp:5:2: error: no member named 'x' in 'Y' at line 100\n")
    w("worklist/out/B.cpp", "B.cpp:1:1: error: use of undeclared identifier 'q'\n")
    w("worklist/out/tie.cpp", "tie.cpp:1:1: error: first kind 1\ntie.cpp:2:1: error: second kind 2\n")
    w("worklist/out/long.cpp", "long.cpp:1:1: error: " + "x" * 80 + "\n")
    w("worklist/out/header.h", "error: not compiled\n")
    w("worklist/out/UPPER.CPP", "UPPER.CPP:1:1: error: shouted\n")
    cc = os.path.join(d, "fakecc.cmd")
    w("worklist/emptyout/.gitkeep", b"")
    cwd = os.getcwd()
    os.chdir(d)
    try:
        worklist.run("out", cc=cc, std="c++03", worklist_path="WORKLIST.md")
        worklist.run("emptyout", cc=cc, std="c++11", worklist_path="WORKLIST_EMPTY.md")
    finally:
        os.chdir(cwd)
    for name in ("WORKLIST.md", "WORKLIST_EMPTY.md"):
        os.replace(os.path.join(d, name), os.path.join(HERE, "worklist.%s.golden.md" % name[:-3]))


# ---------------------------------------------------------------------------------------------------- vcxproj
VCX_A = """<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">

  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Xbox 360">
      <Configuration>Debug</Configuration>
      <Platform>Xbox 360</Platform>
    </ProjectConfiguration>
  </ItemGroup>

  <PropertyGroup Label="Globals">
    <ProjectName>Game</ProjectName>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release_LTCG|Xbox 360'">
    <ClCompile>
      <Optimization>Full</Optimization>
    </ClCompile>
    <ImageXex>
      <TitleID>
      </TitleID>
    </ImageXex>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Label="J2XTitleId">
    <ImageXex>
      <TitleID>0x11111111</TitleID>
    </ImageXex>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="old1.cpp" />
    <ClCompile Include="stdafx.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Xbox 360'">Create</PrecompiledHeader>
    </ClCompile>
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="old.h" />
  </ItemGroup>
  <ItemGroup>
    <None Include="readme.txt" />
  </ItemGroup>
  <Target Name="StageContent" AfterTargets="Build">
    <Exec Command="x" />
  </Target>
  <Target Name="DeployContent" AfterTargets="Build" Condition="old">
    <Exec Command="y" />
  </Target>
  <Import Project="x.targets" />
</Project>
"""
VCX_B = """<Project>
  <PropertyGroup>
    <A>1</A>
  </PropertyGroup>

  <ItemGroup>
    <ClCompile Include="x.cpp" />
  </ItemGroup>
</Project>
"""
VCX_C = """<Project>
  <ItemDefinitionGroup Condition="a">
    <ImageXex><TitleID></TitleID></ImageXex>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="b">
    <ImageXex><TitleID>   </TitleID></ImageXex>
  </ItemDefinitionGroup>
</Project>
"""
SRC_FILES = {
    "stdafx.cpp": "x", "stdafx.h": "x", "Block.cpp": "x", "Block.h": "class Block {};\n",
    "Fluid.h": "class Fluid : public LiquidBlock {};\n", "LiquidBlock.h": "class LiquidBlock : public Block {};\n",
    "LiquidBlock.cpp": "x", "Fluid.cpp": "x", "Debug.cpp": "x", "Debug.h": "x", "A&B.cpp": "x", "A&B.h": "x",
    "NoExt": "x", "Many.dots.here.cpp": "x", "Many.dots.here.h": "x", "Zed.cpp": "x", "KernelConfig.h": "x",
    "GuiMainMenu.cpp": "x", "GuiMainMenu.h": "x", "xuimenu/Ui.cpp": "x", "xuimenu/Ui.h": "x",
    "xuimenu/Gfx/Draw.cpp": "x", "xuimenu/stdafx.h": "x", "Debug/skipped.cpp": "x", "obj/skipped.cpp": "x",
    "Release/skipped.h": "x", "UPPER.CPP": "x", "UPPER.H": "x", "readme.txt": "x",
}
# the folder rules the vcxproj and layout goldens use: a few of every kind (first match wins)
RULES = [("^(Debug|HeapTrack)$", "Debug"), ("^(stdafx|Common)$", "Runtime"), ("^Block$", "Blocks\\Base"),
         ("^LiquidBlock$", "Blocks\\Liquid"), ("^Gui\\w*", "Gui"), ("^A&B$", "Amp & <Co>"), ("^Zed$", "Zed")]
PMAPS = {
    "none": None, "empty": {}, "chain": {"Fluid": "LiquidBlock", "LiquidBlock": "Block"},
    "cycle": {"X": "Y", "Y": "Z", "Z": "X"}, "self": {"Q": "Q"}, "emptyparent": {"E": ""},
    "deep": {"D1": "D2", "D2": "D3", "D3": "D4", "D4": "GuiButton"},
    "long": dict(("L%d" % i, "L%d" % (i + 1)) for i in range(30)) | {"L30": "Entity"},
}
NAMES = ["Debug", "Kernel", "KernelConfig", "ControllerConfig", "Block", "BlockStone", "GuiMainMenu", "Fluid", "LiquidBlock",
         "A", "Foo", "FooBar", "Base", "", "Mid", "Entity", "EntityPlayer", "j2x_thing", "stdafx", "Packet5Foo", "ItemStack",
         "TileEntityChest", "Random", "D1", "X", "Q", "E", "L0", "RenderGlobal", "NBTTagCompound", "Minecraft", "NoSuchThing",
         "WorldGenTrees", "FileInputStream", "OutOfMemoryError", "AB", "Ab", "ADebug", "GL11", "GLContext", "Keyboard",
         "Socket", "SocketAddress", "Inflater", "Pattern", "Packet", "NetClientHandler", "NetworkManager", "SoundManager",
         "BufferedImage", "ThreadPool", "Save360", "STFS_Foo", "Controllers", "JavaString", "ReflectionFactory", "Zip",
         "ZipEntry", "Tesselator", "ChunkCache", "MobSpawner", "Achievement", "XactAudio", "Throwable"]


def _reseed_vcx(rules):
    from codebreaker.java.console import vcxproj

    class P(object):
        pass
    p = P()
    p.vcxproj_filters = rules
    vcxproj._reseed(p)
    return vcxproj


def _dump_vcx_files(base, d):
    out = {}
    for root, _dirs, files in os.walk(os.path.join(base, d)):
        for fn in files:
            if fn.endswith(".vcxproj") or fn.endswith(".filters") or fn.endswith(".sln"):
                out[os.path.relpath(os.path.join(root, fn), os.path.join(base, d)).replace("\\", "/")] = \
                    open(os.path.join(root, fn), "rb").read().decode("utf-8")
    return out


def gen_vcxproj():
    import contextlib
    from codebreaker.java import profiles
    from codebreaker.java.console import vcxproj
    base = os.path.join(HERE, "vcx")
    shutil.rmtree(base, ignore_errors=True)

    # the real xbox360 profile's rules, handed to the C++ tests as data
    profiles.use("minecraft_xbox360")
    real = list(vcxproj._FILTER_RULES)
    golden("vcx.rules.golden.tsv", "".join("%s\t%s\n" % (p, f) for p, f in real))
    real_out = []
    for pm in PMAPS:
        for n in NAMES:
            real_out.append("%s|%s: %s" % (n, pm, vcxproj._classify(n) if PMAPS[pm] is None else vcxproj._classify(n, PMAPS[pm])))
    golden("vcx.classify_real.golden.txt", "\n".join(real_out) + "\n")

    # ---- the small rule set
    vcxproj = _reseed_vcx(RULES)
    out = []
    for pm in PMAPS:
        for n in NAMES:
            out.append("%s|%s: %s" % (n, pm, vcxproj._classify(n) if PMAPS[pm] is None else vcxproj._classify(n, PMAPS[pm])))
    golden("vcx.classify.golden.txt", "\n".join(out) + "\n")

    # ---- guids
    golden("vcx.guid.golden.txt", "".join("%s: %s\n" % (n, vcxproj._guid(n)) for n in
                                          ("", "a", "Block", "Debug", "A&B", "x" * 200, "a\\b", "Platform\\Config", "j2ctool")))

    # ---- parent maps
    w("vcx/pm1/Fluid.h", "#ifndef FLUID_H\nclass Fluid : public Block {\n};\n#endif\n")
    w("vcx/pm1/Loose.h", "class Loose {\n};\n")
    w("vcx/pm1/Fwd.h", "class Block;\nclass Foo : public Bar {};\n")
    w("vcx/pm1/Multi.h", "class A1 : public B1 {};\r\n  class A2 : private B2 {};\r\n\tclass A3 : protected B3 {};\r\nclass A4: B4 {};\r\nclass A5 :B5{};\r\n")
    w("vcx/pm1/Templ.h", "class T1 : public Y<int> {};\nclass T2 : virtual public V2 {};\nclass T3 : public ns::Q {};\nclass T4:public\n  P4 {};\n"
                         "class T5 : public ::Global {};\nclass T6 :\n    Base6 {};\nenum class E7 : int {};\n")
    w("vcx/pm1/Dup1.h", "class Dup : public First {};\n")
    w("vcx/pm1/Dup2.h", "class Dup : public Second {};\n")
    w("vcx/pm1/Struct.h", "struct S1 : public B {};\nclass NotAtStart; class X1 : public B {};\n// class Cm : public B {};\n   class Indented : public Ind {};\n")
    w("vcx/pm1/bad.h", b"class Bad : public Good {};\n\xff\xfe class Bad2 : public Good2 {};\nclass Bad3 : public Good3 {};\n")
    w("vcx/pm1/notheader.hpp", "class H : public Z {};\n")
    w("vcx/pm1/sub/inner.h", "class Inner : public Z {};\n")
    os.makedirs(os.path.join(base, "pm1", "dir.h"), exist_ok=True)
    os.makedirs(os.path.join(base, "pm_empty"), exist_ok=True)
    out = []
    for d in ("pm1", "pm_empty", "nonexistent"):
        pm = vcxproj._build_parent_map(os.path.join(base, d))
        out.append("parent_map %s" % d)
        for k in sorted(pm):
            out.append("  %s => %s" % (k, pm[k]))
    golden("vcx.parent_map.golden.txt", "\n".join(out) + "\n")

    # ---- title ids (the message of a bad one is told by its kind: the wording differs by language)
    cfgs = [("t01", None), ("t02", '{"title_id": "FFFE1234"}'), ("t03", '{"title_id": "0xabcdef12"}'),
            ("t04", '{"title_id": " 0X1f "}'), ("t05", '{"title_id": 12345}'), ("t06", '{"title_id": null}'), ("t07", '{}'),
            ("t08", '{"title_id": ""}'), ("t09", '{"title_id": 0}'), ("t10", '{"title_id": "xyz"}'),
            ("t11", '{"title_id": "123456789"}'), ("t12", '{"title_id": 1.5}'), ("t13", '{"title_id": true}'),
            ("t14", '{"title_id": ["a","b"]}'), ("t15", '{"title_id": {"k": 1}}'), ("t16", '{"title_id": }'), ("t17", ''),
            ("t18", '[1,2]'), ("t19", '"str"'), ("t20", b'\xef\xbb\xbf{"title_id": "ABCD"}'),
            ("t23", '{"title_id": false}'), ("t24", '{"title_id": 0.0}'), ("t25", '{"title_id": -5}'),
            ("t26", '{"title_id": 4294967296}'), ("t27", '{"title_id": "00000000"}'), ("t31", '{"other": 1}'),
            ("t32", '{"title_id": "ab", "title_id": "cd"}'), ("t33", '{"title_id": 255}'),
            ("t34", '{"title_id": "\\u0041\\u0042"}'), ("t35", 'null'), ("t36", '12'), ("t38", '{"title_id": []}'),
            ("t39", '{"title_id": {}}'), ("t43", '{"a": 1} x'), ("t44", '{"title_id": "ok"}'), ("t45", '{"title_id": "\\n"}'),
            ("t46", '{"title_id": "a\\tb"}'), ("t50", '[1, 2'), ("t51", '{"title_id": 12345678901234567890}'),
            ("t52", '{"title_id": 1.0}')]
    out = []
    for tid, content in cfgs:
        os.makedirs(os.path.join(base, tid), exist_ok=True)
        if content is not None:
            w("vcx/%s/platform.config.json" % tid, content)
        try:
            t, msg = vcxproj._title_id_for(os.path.join(base, tid))
        except Exception as e:  # the original crashes on a config that is not an object
            out.append("%s: EXC %s" % (tid, type(e).__name__))
            continue
        kind = ("none" if not msg else "UNSET" if msg.startswith("title id: UNSET") else "UNREADABLE" if "could not read" in msg
                else "INVALID" if "not 1-8 hex" in msg else "?" + msg)
        out.append("%s: id=%s note=%s" % (tid, t, kind))
    os.makedirs(os.path.join(base, "t41", "platform.config.json"), exist_ok=True)   # a folder, not a file
    t, msg = vcxproj._title_id_for(os.path.join(base, "t41"))
    out.append("t41: id=%s note=%s" % (t, "UNREADABLE" if "could not read" in msg else msg))
    golden("vcx.title.golden.txt", "\n".join(out) + "\n")

    # ---- the source lists
    for r in ("a.cpp", "B.H", "c.h", "sub/d.cpp", "sub/e.h", "Debug/x.cpp", "DEBUG/y.cpp", "obj/z.cpp", "sub/bin/q.cpp",
              "sub/x64/r.h", ".git/g.cpp", ".vs/v.cpp", "sub/deeper/f.cpp", "sub/deeper/more/g.h", "file.cpp.txt", "noext",
              "Release_LTCG/a.cpp", "ipch/i.cpp", "sub/Report/x.h", "sub/_decompiled/x.h", "sub/Profile/p.cpp", "fastcap/f.h",
              "CodeAnalysis/c.h", "x.CPP", "y.hpp", "Release/r.cpp", "sub/release/r.h", "proj.vcxproj"):
        w("vcx/es1/" + r, "x")
    os.makedirs(os.path.join(base, "es_empty"), exist_ok=True)
    out = []
    for d in ("es1", "es_empty", "nonexistent"):
        cp, hs = vcxproj._enum_sources(os.path.join(base, d))
        out.append("enum %s\n  cpps=%s\n  hs=%s" % (d, "|".join(cp), "|".join(hs)))
    golden("vcx.enum_sources.golden.txt", "\n".join(out) + "\n")

    # ---- write_filters
    for d in ("wf1", "wf3", "wf5"):
        for rel_, text in SRC_FILES.items():
            w("vcx/%s/%s" % (d, rel_), text)
    cpps_all = [f.replace("/", "\\") for f in SRC_FILES if f.endswith(".cpp")]
    hs_all = [f.replace("/", "\\") for f in SRC_FILES if f.endswith(".h")]
    wf = [("wf1", "Proj", cpps_all, hs_all), ("wf2", "Empty", [], []), ("wf3", "Q&A", ["Block.cpp", "Debug.cpp"], ["Block.h"]),
          ("wf5", "Dir", ["sub\\deep\\er\\x.cpp", "Block.cpp"], ["Block.h", "a\\b\\Block.h"])]
    os.makedirs(os.path.join(base, "wf2"), exist_ok=True)
    out = []
    for d, name, cp, hs in wf:
        path, n = vcxproj.write_filters(os.path.join(base, d), name, cp, hs)
        out.append("write_filters %s %s -> %s folders=%d\n%s" % (d, name, os.path.basename(path), n, open(path, "rb").read().decode("utf-8")))
    golden("vcx.write_filters.golden.txt", "\n".join(out))
    for d in ("wf1", "wf2", "wf3", "wf5"):
        for fn in os.listdir(os.path.join(base, d)):
            if fn.endswith(".filters"):
                os.remove(os.path.join(base, d, fn))

    # ---- run
    cases = (("ra", VCX_A.replace("\n", "\r\n"), '{"title_id": "FFFE0001"}', True, True),
             ("rb", VCX_B, None, True, True), ("rc", VCX_C, '{"title_id": null}', True, True),
             ("rd", VCX_A, '{"title_id": }', False, False), ("re", "<Project>\n  <PropertyGroup/>\n", '{"title_id": "1f"}', False, False),
             ("rf", "", '{"title_id": "FFFE0001"}', True, True),
             ("rg", "<Project>\n\t\n  <!-- \u00e4\u00f6\u00fc -->\n  \n  <PropertyGroup/>\n</Project>\n\n\n", '{"title_id": 255}', True, False),
             ("rh", VCX_A, '{"title_id": "zzz"}', True, True))
    out = []
    for name, vcx, cfg, extra, flt in cases:
        w("vcx/run_%s/Game.vcxproj" % name, vcx)
        if cfg is not None:
            w("vcx/run_%s/platform.config.json" % name, cfg)
        if extra:
            for rel_, text in SRC_FILES.items():
                w("vcx/run_%s/%s" % (name, rel_), text)
    for name, vcx, cfg, extra, flt in cases:   # the inputs stay as they are: the Python runs on a copy
        work = os.path.join(base, "work_" + name)
        shutil.copytree(os.path.join(base, "run_" + name), work)
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            vcxproj.run(work, None, flt)
        out.append("run %s filters=%s\n  stdout: %s" % (name, flt, buf.getvalue().replace("\n", "\\n")))
        files = _dump_vcx_files(base, "work_" + name)
        for k in sorted(files):
            out.append("  file %s:\n%s" % (k, files[k]))
        out.append("  --")
        shutil.rmtree(work)
    golden("vcx.run.golden.txt", "\n".join(out) + "\n")
    _reseed_vcx([])
    profiles.use("generic")


# ----------------------------------------------------------------------------------------------- project_layout
def gen_layout():
    import contextlib
    from codebreaker.java.console import project_layout
    base = os.path.join(HERE, "layout")
    shutil.rmtree(base, ignore_errors=True)
    _reseed_vcx(RULES)
    project_layout.HERE = os.path.join(ROOT, "codebreaker", "java", "console")
    project_layout._TEMPLATE = os.path.join(project_layout.HERE, "templates", "xbox360.vcxproj.in")
    flat = {"stdafx.cpp": "x", "stdafx.h": "x", "Block.cpp": "x", "Block.h": "class Block {};\n",
            "Fluid.h": "class Fluid : public Block {};\n", "Zed.cpp": "x", "notes.txt": "n", "Content/a.png": "p",
            "Game.sln": "OLD SOLUTION", "Game.suo": "s", "Game.sdf": "d"}
    for rel_, text in flat.items():
        w("layout/in_flat/Game/" + rel_, text)
    w("layout/in_guid/Proj/Proj/Proj.vcxproj", '<Project>\r\n  <PropertyGroup Label="Globals">\r\n'
      '    <ProjectGuid>{ABCDEF01-2345-6789-ABCD-EF0123456789}</ProjectGuid>\r\n  </PropertyGroup>\r\n</Project>\r\n')
    w("layout/in_guid/Proj/Proj/Block.cpp", "x")
    w("layout/in_guid/Proj/Proj/Block.h", "x")
    w("layout/in_guid/Proj/extra.txt", "x")
    w("layout/in_named/Root/a.cpp", "x")
    w("layout/in_named/Root/a.h", "x")
    w("layout/in_empty/Empty/readme.txt", "x")
    out = []

    def snapshot(label, root, result):
        rows = ["layout %s: name=%s moved=%d guid=%s" % (label, result["name"], result["moved"], result["guid"])]
        for dp, dirs, files in sorted(os.walk(root)):
            dirs.sort()
            for fn in sorted(files):
                p = os.path.join(dp, fn)
                r_ = os.path.relpath(p, root).replace("\\", "/")
                data = open(p, "rb").read()
                if fn.endswith((".sln", ".vcxproj", ".filters")):
                    rows.append("  file %s:\n%s" % (r_, data.decode("utf-8")))
                else:
                    rows.append("  file %s (%d bytes)" % (r_, len(data)))
        return "\n".join(rows)

    for label, src, sub, name, regen in (("flat", "in_flat", "Game", None, True), ("guid", "in_guid", "Proj", None, True),
                                         ("named", "in_named", "Root", "Alt", True), ("noregen", "in_flat", "Game", None, False),
                                         ("empty", "in_empty", "Empty", None, True)):
        work = os.path.join(base, "work")
        shutil.rmtree(work, ignore_errors=True)
        shutil.copytree(os.path.join(base, src), work)
        root = os.path.join(work, sub)
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            res = project_layout.layout(root, name, regen)
        out.append(snapshot(label, root, res))
        out.append("  stdout: %s" % buf.getvalue().replace("\n", "\\n"))
        if label == "flat":      # running again changes nothing
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf):
                res2 = project_layout.layout(root, name, regen)
            out.append(snapshot("flat-again", root, res2))
        shutil.rmtree(work)
    golden("layout.golden.txt", "\n".join(out) + "\n")
    _reseed_vcx([])


# -------------------------------------------------------------------------------------------------------- xact
DEMAND_SOUNDS = ["mob.pig", "random bow", "random.click", "random.door.close", "step.grass"]
DEMAND_STEPS = ["grass", "stone"]


def _patch_demand():
    from codebreaker.java.projects.minecraft import content
    content.scan_assets = lambda java_dir: {"resources": {}, "sounds": dict((s, []) for s in DEMAND_SOUNDS),
                                            "step_sounds": dict((s, []) for s in DEMAND_STEPS)}


def _wav(path, channels, rate, width, frames):
    os.makedirs(os.path.dirname(os.path.join(HERE, path)), exist_ok=True)
    with wave.open(os.path.join(HERE, path), "wb") as f:
        f.setnchannels(channels)
        f.setsampwidth(width)
        f.setframerate(rate)
        f.writeframes(bytes((i * 7 + 3) & 0xFF for i in range(frames * channels * width)))


def _entry_text(e):
    return "[%d] %s %dch %dHz block=%d %dbit play=%d+%d%s" % (
        e["index"], e["codec"], e["channels"], e["sample_rate"], e["block_align"], e["bits"], e["play_offset"], e["play_length"],
        (" name=%s" % e["name"]) if "name" in e else "")


def _xwb_text(w_):
    out = ["xwb %s magic=%s version=%d header_version=%d name=%r flags=%d entry_count=%d" % (
        w_["file"], w_["magic"], w_["version"], w_["header_version"], w_["name"], w_["flags"], w_["entry_count"])]
    for name, (off, ln) in w_["segments"].items():
        out.append("  segment %s %d %d" % (name, off, ln))
    for e in w_["entries"]:
        out.append("  " + _entry_text(e))
    return "\n".join(out)


def gen_xact():
    import contextlib
    from codebreaker.java.console import xact
    _patch_demand()
    base = os.path.join(HERE, "xact")
    shutil.rmtree(base, ignore_errors=True)
    out = []

    # ---- the PCM bank writer, byte for byte, and what reading it back says
    _wav("xact/wav/step_grass.wav", 1, 22050, 2, 100)
    _wav("xact/wav/random_click.wav", 2, 44100, 2, 50)
    _wav("xact/wav/dig_stone.wav", 1, 11025, 1, 30)
    wavs = [os.path.join(base, "wav", n + ".wav") for n in ("step_grass", "random_click", "dig_stone")]
    entries = xact.build_pcm_xwb(wavs, os.path.join(base, "sounds.golden.xwb"))
    out.append("build_pcm_xwb: " + " ".join("%s:%d:%d:%d" % (e["name"], e["mini"], e["off"], e["len"]) for e in entries))
    out.append(_xwb_text(xact.parse_xwb(os.path.join(base, "sounds.golden.xwb"))))
    out.append(xact.dump(os.path.join(base, "sounds.golden.xwb")))
    # a bank with entry names, handmade: BANKDATA, metadata, names, waves
    names = [b"alpha", b"beta", b"gamma_longer_name"]
    bankdata = struct.pack(">II", 1, 3) + b"named".ljust(64, b"\0") + struct.pack(">III", 24, 16, 2048)
    meta_off = 52 + len(bankdata)
    meta = b"".join(struct.pack(">IIIIII", 4, xact._miniformat(1, 22050, 16), i * 2048, 100 + i, 0, 0) for i in range(3))
    names_off = meta_off + len(meta)
    name_tab = b"".join(n[:16].ljust(16, b"\0") for n in names)
    wave_off = (names_off + len(name_tab) + 2047) & ~2047
    hdr = b"DNBW" + struct.pack(">II", 46, 44) + b"".join(struct.pack(">II", o, l) for o, l in [
        (52, len(bankdata)), (meta_off, len(meta)), (0, 0), (names_off, len(name_tab)), (wave_off, 4096)])
    named = (hdr + bankdata + meta + name_tab).ljust(wave_off, b"\0") + b"\1" * 4096
    w("xact/named.xwb", named)
    out.append(_xwb_text(xact.parse_xwb(os.path.join(base, "named.xwb"))))
    out.append(xact.dump(os.path.join(base, "named.xwb")))
    w("xact/cut.xwb", named[:meta_off + 24 + 10])            # cut in the second entry: the one whole entry is listed
    out.append(_xwb_text(xact.parse_xwb(os.path.join(base, "cut.xwb"))))
    # many entries: only the first 64 are listed by dump
    big_meta = b"".join(struct.pack(">IIIIII", 4, 0, 0, 0, 0, 0) for _ in range(70))
    big_bd = struct.pack(">II", 0, 70) + b"big".ljust(64, b"\0") + struct.pack(">III", 24, 0, 2048)
    big_hdr = b"DNBW" + struct.pack(">II", 46, 44) + b"".join(struct.pack(">II", o, l) for o, l in [
        (52, len(big_bd)), (52 + len(big_bd), len(big_meta)), (0, 0), (0, 0), (0, 0)])
    w("xact/big.xwb", big_hdr + big_bd + big_meta)
    out.append(xact.dump(os.path.join(base, "big.xwb")))
    golden("xact.xwb.golden.txt", "\n".join(out) + "\n")

    # ---- sound banks
    xsb = (b"KBDS" + b"\x2e\x00\x00\x00" + b"\x00" * 8 + b"sounds\x00" + b"ab\x00" + b"random_click\x00\x01\x02" + b"step_grass\x00"
           + b"step_grass 2\x00" + b"sounds\x00" + b"dig_stone\x00" + b"\xff\xfe\xfd" + b"step_stone\x00" + b"unused_one\x00" + b"tail")
    w("xact/sounds.xsb", xsb)
    w("xact/pc.xsb", b"SDBK" + b"\x00" * 12 + b"only_name\x00cue_a\x00")
    w("xact/empty.xsb", b"KBDS" + b"\x00" * 12)
    out = []
    for n in ("sounds.xsb", "pc.xsb", "empty.xsb"):
        s = xact.parse_xsb(os.path.join(base, n))
        out.append("xsb %s magic=%s name=%r cues=%s" % (s["file"], s["magic"], s["name"], s["cue_names"]))
        out.append(xact.dump(os.path.join(base, n)))
    golden("xact.xsb.golden.txt", "\n".join(out) + "\n")

    # ---- the demand, the coverage, the sources
    out = []
    needed = xact.needed_cues("<java>")
    out.append("needed:")
    for c in sorted(needed):
        out.append("  %s <- %s" % (c, ", ".join(needed[c])))
    cov = xact.cue_coverage("<java>", os.path.join(base, "sounds.xsb"))
    out.append("coverage authored=%s missing=%s unused=%s" % (cov["authored"], cov["missing"], cov["unused"]))
    out.append(xact.render_coverage(cov))
    for rel_ in ("newsound/step/grass1.ogg", "newsound/step/grass2.ogg", "newsound/step/grass3.ogg", "newsound/step/stone1.ogg",
                 "newsound/random/click.ogg", "newsound/random/door_close.ogg", "newsound/mob/pig1.ogg", "newsound/mob/pig2.ogg",
                 "newsound/note.txt", "music/calm1.ogg", "music/calm3.ogg", "sound/random/explode.ogg", "readme.OGG", "sound.ogg"):
        w("xact/res/" + rel_, b"")
    res = os.path.join(base, "res")
    out.append("ogg_keys:")
    for p in ("newsound/step/grass1.ogg", "random/door_close.ogg", "Sound/x.ogg", "sound/a_b/c_d2.ogg", "streaming/x9.OGG", "music.ogg",
              "sound.ogg", "plain", "a/b/c12", "x_.ogg", "_a.ogg", "resources/z.ogg", "newsound/newsound/q1.ogg", "12.ogg"):
        out.append("  %s -> %s" % (p, sorted(xact._ogg_keys(p))))
    idx = xact.scan_ogg_sources(res)
    out.append("index:")
    for k in sorted(idx):
        out.append("  %s -> %s" % (k, idx[k]))
    m = xact.match_sources("<java>", res)
    out.append("matched:")
    for c in sorted(m["matched"]):
        out.append("  %s event=%s files=%s" % (c, m["matched"][c]["event"], m["matched"][c]["files"]))
    out.append("missing:")
    for c in sorted(m["missing"]):
        out.append("  %s <- %s" % (c, m["missing"][c]))
    out.append("index_keys=%d" % m["index_keys"])
    golden("xact.sources.golden.txt", "\n".join(out) + "\n")

    # ---- the files that get written
    def tree_text(root, repl):
        rows = []
        for dp, dirs, files in os.walk(root):
            dirs.sort()
            for fn in sorted(files):
                p = os.path.join(dp, fn)
                data = open(p, "rb").read().decode("utf-8", "replace").replace(repl[0], repl[1])
                rows.append("== %s\n%s" % (os.path.relpath(p, root).replace("\\", "/"), data))
        return "\n".join(rows)

    ff_dir = os.path.join(base, "ff")
    os.makedirs(ff_dir)
    bat = xact.write_ffmpeg_script(m, ff_dir, res)
    xact.write_build_script(ff_dir)
    xact.write_build_script(ff_dir, bank="music", bat_name="music_banks.bat")
    cues = xact.build_xap(None, os.path.join(ff_dir, "cue_list.xap"), cue_list=["step_grass", "random_click", "dig_stone"])
    cues2 = xact.build_xap(None, os.path.join(ff_dir, "music_like.xap"), bank="music", cue_list=["calm3", "calm1"],
                           cue_namer=lambda c: c, sound_category="Music", streaming=True)
    os.makedirs(os.path.join(ff_dir, "gated", "wav"))
    for n in ("a", "c"):
        w("xact/ff/gated/wav/%s.wav" % n, b"")
    xact.build_xap(None, os.path.join(ff_dir, "gated", "g.xap"), cue_list=["c", "b", "a"])
    xact.build_xap(None, os.path.join(ff_dir, "gated", "none_present.xap"), cue_list=["x", "y"])    # no wav at all: all are kept
    xact.build_xap(None, os.path.join(ff_dir, "gated", "dotted.xap"), cue_list=["step_grass"], cue_namer=lambda c: c.replace("_", "."),
                   wave_dir="other")
    out = [tree_text(ff_dir, (res, "<RES>"))]
    golden("xact.files.golden.txt", "\n".join(out) + "\n")

    # ---- music and the one-shot build (no tools run)
    mdir = os.path.join(base, "music_out")
    found = xact.build_music(mdir, res) if os.makedirs(mdir) is None else None
    none_dir = os.path.join(base, "nomusic")
    os.makedirs(none_dir)
    found_none = xact.build_music(none_dir, os.path.join(base, "wav"))
    bdir = os.path.join(base, "build_out")
    xap, cues = xact.build("<java>", bdir, resources_dir=res)
    out = ["music found=%s none=%s" % (found, found_none), "build cues=%s" % cues, "build xap=%s" % os.path.basename(xap),
           "-- music_out", tree_text(mdir, (res, "<RES>")), "-- build_out", tree_text(bdir, (res, "<RES>"))]
    golden("xact.build.golden.txt", "\n".join(out) + "\n")

    # ---- dist cleanup: what is left once banks exist
    cdir = os.path.join(base, "clean_out")
    for f in ("sounds.xwb", "sounds.xsb", "x.bat", "y.xap", "z.h", "g.xgs", "n.md", "t.txt", "keep.dat"):
        w("xact/clean_out/" + f, b"x")
    w("xact/clean_out/wav/a.wav", b"x")
    xact._clean_audio_intermediates(cdir)
    nb = os.path.join(base, "clean_nobank")
    for f in ("y.xap", "t.txt"):
        w("xact/clean_nobank/" + f, b"x")
    w("xact/clean_nobank/wav/a.wav", b"x")
    xact._clean_audio_intermediates(nb)
    left = lambda d: sorted(os.path.relpath(os.path.join(dp, f), d).replace("\\", "/") for dp, _d, fs_ in os.walk(d) for f in fs_)
    golden("xact.clean.golden.txt", "banks built: %s\nno banks: %s\n" % (left(cdir), left(nb)))
    shutil.rmtree(cdir)
    shutil.rmtree(nb)
    shutil.rmtree(ff_dir)
    shutil.rmtree(mdir)
    shutil.rmtree(none_dir)
    shutil.rmtree(bdir)
    shutil.rmtree(os.path.join(base, "wav"), ignore_errors=False) if False else None


# ------------------------------------------------------------------------------------------------------ assets
def _chunk(tag, data):
    import zlib
    body = tag + data
    return struct.pack(">I", len(data)) + body + struct.pack(">I", zlib.crc32(body) & 0xFFFFFFFF)


def _pack_row(samples, depth):
    """One row of samples (each 0 .. 2**depth-1) packed the way PNG stores them."""
    if depth == 8:
        return bytes(samples)
    if depth == 16:
        return b"".join(struct.pack(">H", s) for s in samples)
    out = bytearray()
    acc, bits = 0, 0
    for s in samples:
        acc = (acc << depth) | s
        bits += depth
        if bits == 8:
            out.append(acc)
            acc, bits = 0, 0
    if bits:
        out.append(acc << (8 - bits))
    return bytes(out)


def _filtered(rows, bpp):
    """Scanlines with the filter types 0..4 in turn (so each predictor is read), as PNG stores them."""
    out = bytearray()
    prev = bytes(len(rows[0])) if rows else b""
    for y, row in enumerate(rows):
        ft = y % 5
        out.append(ft)
        for x, v in enumerate(row):
            a = row[x - bpp] if x >= bpp else 0
            b = prev[x]
            c = prev[x - bpp] if x >= bpp else 0
            if ft == 0:
                p = 0
            elif ft == 1:
                p = a
            elif ft == 2:
                p = b
            elif ft == 3:
                p = (a + b) >> 1
            else:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                p = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
            out.append((v - p) & 0xFF)
        prev = row
    return bytes(out)


def make_png(width, height, ct, depth, pixel, palette=None, trns=None, interlace=0):
    """A PNG written by hand. pixel(x, y) -> the samples of that pixel (a tuple)."""
    import zlib
    nch = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ct]
    bpp = max(1, nch * depth // 8)
    if interlace:
        passes = [(0, 0, 8, 8), (4, 0, 8, 8), (0, 4, 4, 8), (2, 0, 4, 4), (0, 2, 2, 4), (1, 0, 2, 2), (0, 1, 1, 2)]
    else:
        passes = [(0, 0, 1, 1)]
    raw = bytearray()
    for x0, y0, dx, dy in passes:
        if x0 >= width or y0 >= height:
            continue
        rows = []
        for y in range(y0, height, dy):
            samples = []
            for x in range(x0, width, dx):
                samples.extend(pixel(x, y))
            rows.append(_pack_row(samples, depth))
        raw += _filtered(rows, bpp)
    data = b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, depth, ct, 0, 0, interlace))
    if palette is not None:
        data += _chunk(b"PLTE", bytes(b for rgb in palette for b in rgb))
    if trns is not None:
        data += _chunk(b"tRNS", trns)
    half = len(raw) // 2 or 1            # two IDAT chunks, so that the data stream is joined
    comp = zlib.compress(bytes(raw))
    data += _chunk(b"IDAT", comp[:half // 2 or 1]) + _chunk(b"IDAT", comp[half // 2 or 1:])
    return data + _chunk(b"IEND", b"")


def _png_cases():
    """name -> PNG bytes: every colour type and depth, the transparency forms, both interlace settings."""
    def k(max_):       # a 16-bit sample is spread over the whole range, so that its high byte is not zero
        return 997 if max_ == 65535 else 1

    def g(max_):
        return lambda x, y: ((x * 3 + y * 5 + 1) * (k(max_) if (x + y) % 2 else 1) % (max_ + 1),)   # some 16-bit greys stay below 256

    def rgb(max_):
        return lambda x, y: ((x * 40 + y) * k(max_) % (max_ + 1), (y * 50 + x * 3) * k(max_) % (max_ + 1),
                             (x * y * 9 + 7) * k(max_) % (max_ + 1))

    def rgba(max_):
        return lambda x, y: rgb(max_)(x, y) + ((x * 31 + y * 17) * k(max_) % (max_ + 1),)

    def ga(max_):
        return lambda x, y: ((x * 29 + y * 7) * k(max_) % (max_ + 1), (x * 13 + y * 61) * k(max_) % (max_ + 1))

    pal = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0), (10, 20, 30), (200, 100, 50), (1, 2, 3), (250, 251, 252)]

    def idx(n):
        return lambda x, y: ((x + 2 * y) % n,)
    cases = {
        "grey1": make_png(9, 3, 0, 1, g(1)),
        "grey1_trns0": make_png(9, 3, 0, 1, g(1), trns=struct.pack(">H", 0)),
        "grey2": make_png(5, 4, 0, 2, g(3)),
        "grey4": make_png(6, 4, 0, 4, g(15)),
        "grey8": make_png(4, 6, 0, 8, g(255)),
        "grey8_trns": make_png(4, 6, 0, 8, lambda x, y: (200 if (x + y) % 3 == 0 else 17 * x,), trns=struct.pack(">H", 200)),
        "grey16": make_png(3, 2, 0, 16, g(65535)),
        "rgb8": make_png(5, 5, 2, 8, rgb(255)),
        "rgb8_trns": make_png(5, 2, 2, 8, lambda x, y: (1, 2, 3) if x % 2 == 0 else (x * 9, y, 4),
                              trns=struct.pack(">HHH", 1, 2, 3)),
        "rgb16": make_png(3, 3, 2, 16, rgb(65535)),
        "pal1": make_png(9, 2, 3, 1, idx(2), palette=pal[:2]),
        "pal2_trns": make_png(5, 3, 3, 2, idx(4), palette=pal[:4], trns=bytes([0, 128])),
        "pal4": make_png(6, 4, 3, 4, idx(8), palette=pal),
        "pal4_short_palette": make_png(6, 2, 3, 4, idx(8), palette=pal[:3]),     # indexes past the palette read as black
        "pal8_trns": make_png(7, 3, 3, 8, idx(6), palette=pal[:6], trns=bytes([255, 0, 7, 255, 255, 1, 2, 3, 4])),
        "ga8": make_png(4, 3, 4, 8, ga(255)),
        "ga16": make_png(3, 3, 4, 16, ga(65535)),
        "rgba8": make_png(4, 3, 6, 8, rgba(255)),
        "rgba16": make_png(3, 2, 6, 16, rgba(65535)),
        "pal4_adam7": make_png(7, 5, 3, 4, idx(8), palette=pal, interlace=1),
        "rgb8_adam7": make_png(9, 9, 2, 8, rgb(255), interlace=1),
        "grey2_adam7": make_png(3, 2, 0, 2, g(3), interlace=1),
        "grey8_1x1": make_png(1, 1, 0, 8, lambda x, y: (77,)),
    }
    return cases


def _pixels(png_bytes):
    from PIL import Image
    im = Image.open(io.BytesIO(png_bytes))
    return im.size, im.convert("RGBA").tobytes()


def gen_assets():
    import contextlib
    from PIL import Image
    from codebreaker.java.console import assets
    base = os.path.join(HERE, "assets")
    shutil.rmtree(base, ignore_errors=True)
    os.makedirs(base)
    pngs = _png_cases()
    out = []
    for name, raw in pngs.items():
        norm, changed = assets.normalize_png_to_rgba(raw)
        (w_, h_), px = _pixels(norm)
        # the input read by the imaging library directly must give the same pixels (it is what normalise relies on)
        _, direct = _pixels(raw)
        assert direct == px, name
        out.append("png %s: ct=%d needs=%s changed=%s %dx%d %s" % (name, assets._png_colortype(raw), assets.png_needs_rgba(raw), changed,
                                                                   w_, h_, px.hex()))
    for name, raw in pngs.items():
        w("assets/png/%s.png" % name, raw)
    for bad_name, bad in (("not_png", b"GIF89a....................."), ("short", b"\x89PNG\r\n\x1a\n"), ("empty", b""),
                          ("bad_crc", pngs["grey8"][:30] + b"\xff" + pngs["grey8"][31:]),
                          ("cut", pngs["rgb8"][:60]), ("noidat", pngs["pal4"][:33] + _chunk(b"IEND", b""))):
        try:
            r = assets.normalize_png_to_rgba(bad)
            out.append("bad %s: needs=%s changed=%s" % (bad_name, assets.png_needs_rgba(bad), r[1]))
        except Exception as e:
            out.append("bad %s: needs=%s raises %s" % (bad_name, assets.png_needs_rgba(bad), type(e).__name__))
        w("assets/png/bad_%s.png" % bad_name, bad)
    golden("assets.png.golden.txt", "\n".join(out) + "\n")

    # ---- a jar
    jar = os.path.join(base, "game.jar")
    with zipfile.ZipFile(jar, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("gui/gui.png", pngs["rgba8"])
        z.writestr("gui/items.png", pngs["pal4"])
        z.writestr("terrain.png", pngs["rgb8"])
        z.writestr("misc/water.png", pngs["grey8_trns"])
        z.writestr("font/default.png", pngs["ga8"])
        z.writestr("misc/tiny.png", pngs["grey1"])
        z.writestr("misc/wide.png", pngs["rgb16"])
        z.writestr("misc/Mixed.PNG", pngs["pal2_trns"])
        z.writestr("misc/spiral.png", pngs["rgb8_adam7"])
        z.writestr("lang/en_US.lang", "menu.play=Play\nmenu.quit=Quit\n")
        z.writestr("lang/de_DE.lang", "menu.play=Spielen\n")
        z.writestr("misc/notes.txt", "hello\r\nworld\n")
        z.writestr("shaders/post.fsh", "void main() {}\n")
        z.writestr("readme.html", "<html/>")
        z.writestr("com/mojang/Thing.class", b"\xca\xfe\xba\xbe")
        z.writestr("empty/", b"")
        info = zipfile.ZipInfo("misc/stored.properties")
        info.compress_type = zipfile.ZIP_STORED
        z.writestr(info, "a=1\n")
        z.writestr("gui/gui.png", pngs["rgba8"])        # a name twice: the later one is the one read
    # the same names with the entry case drifting from the code's, and a jar that would write outside Content
    with zipfile.ZipFile(os.path.join(base, "drift.jar"), "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("gui/items.png", pngs["pal4"])
        z.writestr("lang/en.lang", "x=1\n")
        z.writestr("other/unused.png", pngs["grey8"])
    with zipfile.ZipFile(os.path.join(base, "evil.jar"), "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("ok.png", pngs["rgba8"])
        z.writestr("../evil.png", pngs["rgba8"])

    # ---- the code that loads resources, and the console-only bundle
    w("assets/code/Foo.java", '''// "/commented/line.png" is documentation
package a;
/* a block comment mentioning "/in/block.png"
   over two lines "/in/block2.png" */
class Foo {
    void f() {
        load("/gui/gui.png"); load("/terrain.png"); load("/misc/water.png");
        load("/gui/GUI.PNG");                 // upper-case extension: not a resource path
        load("/misc/tiny.png"); load( "/misc/tiny.png" );
        load("//double/slash.png");
        load("/lang/en_US.lang"); load("/missing/nothing.png"); load("/x.jpg"); load("/a-b_c/d.e.txt");
        load("/spaces in/name.png"); load("no/leading/slash.png"); load("/short.p"); load("/.png"); load("/a.png");
        String url = "http://example.com/skin.png"; load("/after/url.png");
        load("/cursor.png"); load("/shaders/post.fsh"); load("/font/default.png");
    }
}
''')
    w("assets/code/sub/Bar.cpp", 'void g() { JString s("/gui/gui.png"); open("/font/default.png"); open("/misc/water.png"); }\n')
    w("assets/code/sub/Baz.h", '/* "/hidden/in/header.png" */ const char* p = "/gui/items.png";\n#define X "/misc/stored.properties"\n')
    w("assets/code/notes.txt", 'ignored: "/in/text.png"\n')
    w("assets/code/Upper.JAVA", 'x("/upper/java.png");\n')
    w("assets/code_drift/A.java", 'x("/Gui/Items.png"); y("/lang/en.lang"); z("/Missing/One.png");\n')
    w("assets/bundle/cursor.png", pngs["rgba8"])
    w("assets/bundle/extra/readme.txt", "bundled\n")

    # ---- the answers
    _assets_console = os.path.join(base, "bundle")
    assets._CONSOLE_ASSETS_DIR = _assets_console

    def listing(d):
        rows = []
        for dp, dirs, files in os.walk(d):
            dirs.sort()
            for fn in sorted(files):
                p = os.path.join(dp, fn)
                rel_ = os.path.relpath(p, d).replace("\\", "/")
                data = open(p, "rb").read()
                if fn.lower().endswith(".png") and data[:8] == b"\x89PNG\r\n\x1a\n":
                    (w_, h_), px = _pixels(data)
                    rows.append("  %s png %dx%d %s" % (rel_, w_, h_, px.hex()))
                else:
                    rows.append("  %s %d %s" % (rel_, len(data), hashlib.sha1(data).hexdigest()))
        return "\n".join(rows)

    out = ["jar assets: %s" % assets.list_jar_assets(jar), "console assets: %s" % assets.list_console_assets()]
    code = os.path.join(base, "code")
    refs = assets.code_resource_refs(code)
    out.append("refs:")
    for k in sorted(refs):
        out.append("  %s <- %s" % (k, refs[k]))
    cov = assets.coverage(jar, code)
    out.append("coverage present=%s\n  missing=%s\n  extra=%s\n  jar_count=%d" % (cov["present"], cov["missing"], cov["extra"], cov["jar_count"]))
    for name, only_needed, normalize in (("all", False, True), ("needed", True, True), ("raw", False, False)):
        cdir = os.path.join(base, "run_" + name, "Content")
        res = assets.run(jar, cdir, code, only_needed, normalize)
        out.append("run %s: written=%s\n  converted=%s unconverted=%s has_pil=%s console=%s" % (
            name, res["written"], res["converted"], res["unconverted"], res["has_pil"], res["console"]))
        out.append("  report: %s" % os.path.relpath(res["report"], os.path.join(base, "run_" + name)).replace("\\", "/"))
        out.append("  report text:\n" + open(res["report"], "rb").read().decode("utf-8").replace("\r\n", "\n"))
        out.append(listing(cdir))
        shutil.rmtree(os.path.join(base, "run_" + name))
    res = assets.run(jar, os.path.join(base, "nocode", "Content"))
    out.append("run nocode: written=%d report=%s" % (len(res["written"]), res["report"]))
    shutil.rmtree(os.path.join(base, "nocode"))
    # a coverage report with nothing missing and nothing converted
    cov2 = {"needed": {"a.png": ["A.java"]}, "present": ["a.png"], "missing": [], "extra": ["b.txt"], "jar_count": 2}
    out.append(assets.render_coverage(cov2))
    out.append(assets.render_coverage(cov2, converted=["x.png", "y.png"]))
    out.append("strip: %r" % assets._strip_comments('a /* x */ b // c\nd "/p//q.png" e /* open'))
    golden("assets.golden.txt", "\n".join(out) + "\n")


if __name__ == "__main__":
    which = sys.argv[1:] or ["xdk", "scan", "worklist", "vcxproj", "layout"]
    for name in which:
        globals()["gen_" + name]()
        print("generated", name)
