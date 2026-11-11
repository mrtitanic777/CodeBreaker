#!/usr/bin/env python3
"""Xbox 360 effects to the PC: formats/xbox/effect.py and shader.py read what the SDK's fxc wrote, decompile/xenos_hlsl.py
and xbox_effect.py recover the HLSL, xna/mgfx.py gives MonoGame's compiled effect back its parameters.

The effects in tests/data/xbox_effects are our own HLSL, each with the SDK's fxc output beside it (<name>.fxo,
fx_2_0). Read back, they hold what fxc's listing of the same source says (the constant table, the literal and loop
constants, the input and output tables). And the recovered HLSL behaves as the source: both compiled by MonoGame's
mgfxc and drawn by xna/shadercheck with the same vertices, parameters and texture, every pixel the same. With the
SDK installed, the .fxo files are compiled afresh first and must match.

    python tests/test_xbox_effect.py"""
import glob
import json
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import xbox_effect, xenos_hlsl   # noqa: E402
from codebreaker.formats.xbox import effect, shader         # noqa: E402
from codebreaker.formats import xnb                         # noqa: E402
from codebreaker.xna import mgfx                            # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "xbox_effects")
FXC = os.path.join(os.environ.get("XEDK", r"C:\Program Files (x86)\Microsoft Xbox 360 SDK"), "bin", "win32", "fxc.exe")
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _fx(name):
    with open(os.path.join(DATA, name + ".fxo"), "rb") as f:
        return f.read()


def _shaders(fx):
    return {k: shader.read(v) for k, v in fx.objects.items() if v[:3] == b"\x10\x2a\x11"}


def test_effect_layout():
    fx = effect.read(_fx("sprite"))
    assert not fx.big_endian                                   # fxc's own; an XNA content effect is big-endian
    names = [(p.name, p.type, p.cls) for p in fx.parameters]
    assert names[:3] == [("WorldViewProj", "float", "matrix_columns"), ("Tint", "float", "vector"),
                         ("Brightness", "float", "scalar")], names
    assert fx.parameter("Tint").value == [1.0, 0.5, 0.25, 1.0] and fx.parameter("Brightness").value == [2.0]
    sampler = {s.name: s.text for s in fx.parameter("DiffuseSampler").value if s.name != "Texture"}
    assert sampler == {"MinFilter": "Linear", "MagFilter": "Linear", "AddressU": "Clamp", "AddressV": "Wrap"}, sampler
    p = fx.techniques[0].passes[0]
    assert (fx.techniques[0].name, p.name) == ("Probe", "P0")
    states = {s.name: s.text for s in p.states if s.text is not None}
    assert states == {"AlphaBlendEnable": "true", "SrcBlend": "SrcAlpha", "DestBlend": "InvSrcAlpha", "CullMode": "None"}, states


def test_shader_tables():
    # what fxc /Fc /Xudsrd lists for skinned.fx's shaders
    shaders = sorted(_shaders(effect.read(_fx("skinned"))).values(), key=lambda s: s.stage)
    ps, vs = shaders
    assert vs.inputs == [(5, "POSITION", 0), (6, "NORMAL", 0), (7, "TEXCOORD", 0), (8, "TEXCOORD", 3), (9, "COLOR", 1),
                         (10, "BLENDINDICES", 0), (11, "BLENDWEIGHT", 0)], vs.inputs
    assert vs.outputs == [(0, "TEXCOORD", 0, 3), (1, "TEXCOORD", 1, 7), (2, "TEXCOORD", 2, 1), (3, "TEXCOORD", 5, 3),
                          (4, "COLOR", 0, 15)], vs.outputs
    assert [(c.name, c.register, c.count) for c in sorted(vs.constants, key=lambda c: c.register)] == \
        [("World", 0, 4), ("ViewProj", 4, 4), ("Bones", 8, 60), ("UseFog", 68, 1)]
    assert vs.literals[255][0] == 3.0 and abs(vs.literals[255][2] - 0.01) < 1e-7
    assert [r for r, _, _, _ in ps.inputs] == [0, 1, 2, 3, 4] and ps.inputs[3][1:3] == ("TEXCOORD", 5)
    loops = [s for s in _shaders(effect.read(_fx("literal_loop"))).values() if s.stage == "pixel"][0]
    assert loops.loop_literals == {30: 3, 31: 10}, loops.loop_literals     # defi i30, 3 / defi i31, 10


def test_recovered_hlsl():
    text = xbox_effect.to_fx(_fx("skinned"), "skinned")
    for wanted in ("float4x3 Bones[20];", "Bones[a0 / 3]._m00_m10_m20_m30", "if (0.0 > r1.x) discard;",
                   "BlendIndices0 : BLENDINDICES0", "VertexShader = compile vs_3_0 VS_T_P();"):
        assert wanted in text, wanted
    text = xbox_effect.to_fx(_fx("loop"), "loop")
    assert "[loop] for (int i0 = 0; i0 < Steps; i0++)" in text, text
    text = xbox_effect.to_fx(_fx("branch"), "branch")
    assert "&& !Invert) { skip_to = " in text      # the bool parameter's branch, a jump over its block


REAL = os.path.join(ROOT, "tests", "fixtures", "xbox_effects")


def test_stock_effect_arrays():
    # XNA's stock effects choose a pass's shaders from arrays: one technique per ShaderIndex
    # (a stock BasicEffect as a game's content writes it, tests/fixtures/xbox_effects)
    with open(os.path.join(REAL, "bw_BasicClipEffect360.bin"), "rb") as f:
        text = xbox_effect.to_fx(f.read(), "BasicClipEffect360")
    assert "technique BasicEffect\n" in text and "technique BasicEffect__31\n" in text
    assert "VertexShader = compile vs_3_0 VSArray_2();\n        PixelShader = compile ps_3_0 PSArray_0();" in text


def test_real_effects_recover():
    # every real effect saved from games' content (the .xnb effects) and the stock ones: read, recovered as HLSL,
    # and written as the port writes them (.fx and its parameters)
    blobs = sorted(glob.glob(os.path.join(REAL, "*.bin")))
    assert len(blobs) == 23, len(blobs)
    with tempfile.TemporaryDirectory() as d:
        for path in blobs:
            name = os.path.splitext(os.path.basename(path))[0]
            with open(path, "rb") as f:
                code = f.read()
            fx = effect.read(code)
            pc = name == "bw_BasicClipEffect"                  # the stock effect's PC build: little-endian
            assert fx.big_endian == (not pc) and fx.techniques and fx.techniques[0].passes, name
            text = xbox_effect.to_fx(code, name)
            assert "technique " in text, name
            rec = xnb.convert_effect_file(path, d)
            if pc:
                assert rec is None                            # only an Xbox 360 effect is converted on its own
                continue
            assert rec and rec["kind"] == "effect" and "error" not in rec, (name, rec)
            assert os.path.isfile(os.path.join(d, name + ".fx")) and os.path.isfile(os.path.join(d, name + ".fx.json"))


def _mgfxc():
    tool = shutil.which("mgfxc")
    if tool is None:
        raise Skip("MonoGame's mgfxc is not installed (dotnet tool install -g dotnet-mgfxc)")
    return tool


def _compile(tool, src, out):
    env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor")
    r = subprocess.run([tool, src, out, "/Profile:OpenGL"], capture_output=True, text=True, env=env)
    assert r.returncode == 0, (src, (r.stdout + r.stderr)[-800:])


def test_mgfx_parameters():
    tool = _mgfxc()
    with tempfile.TemporaryDirectory() as d:
        src, out = os.path.join(d, "s.fx"), os.path.join(d, "s.mgfxo")
        with open(src, "w") as f:
            f.write(xbox_effect.to_fx(_fx("skinned"), "skinned"))
        _compile(tool, src, out)
        data = open(out, "rb").read()
        extra = [{"name": "Unused", "semantic": "", "type": "float", "cls": "vector", "rows": 1, "columns": 4,
                  "elements": 0, "value": [1, 2, 3, 4], "members": []},
                 {"name": "Indices", "semantic": "", "type": "int", "cls": "scalar", "rows": 1, "columns": 1,
                  "elements": 2, "value": None, "members": [
                      {"name": "Indices", "semantic": "", "type": "int", "cls": "scalar", "rows": 1, "columns": 1,
                       "elements": 0, "value": [v], "members": []} for v in (5, 6)]}]
        patched = mgfx.add_parameters(data, extra)
        assert len(patched) > len(data) and mgfx.add_parameters(patched, extra) == patched   # once only
        assert b"Unused" in patched and b"Indices" in patched


def _shadercheck():
    """xna/shadercheck built into state/xna/shadercheck (again when its source is newer)."""
    from codebreaker.base import settings
    src = os.path.join(ROOT, "codebreaker", "xna", "shadercheck")
    out = settings.state("xna", "shadercheck")
    exe = os.path.join(out, "shadercheck.exe" if os.name == "nt" else "shadercheck")
    newest = max(os.path.getmtime(os.path.join(src, f)) for f in ("Program.cs", "shadercheck.csproj"))
    if not os.path.isfile(exe) or os.path.getmtime(exe) < newest:
        if shutil.which("dotnet") is None:
            raise Skip("no dotnet")
        env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor", DOTNET_CLI_TELEMETRY_OPTOUT="1")
        r = subprocess.run(["dotnet", "build", os.path.join(src, "shadercheck.csproj"), "-c", "Release", "-o", out,
                            "--nologo", "-v", "q"], capture_output=True, text=True, env=env)
        assert r.returncode == 0 and os.path.isfile(exe), r.stdout[-1500:]
    return exe


def test_fixtures_current():
    if not os.path.isfile(FXC):
        raise Skip("the Xbox 360 SDK's fxc is not here (the committed .fxo files are used as they are)")
    with tempfile.TemporaryDirectory() as d:
        for src in sorted(glob.glob(os.path.join(DATA, "*.fx"))):
            name = os.path.splitext(os.path.basename(src))[0]
            out = os.path.join(d, name + ".fxo")
            r = subprocess.run([FXC, "/nologo", "/Tfx_2_0", "/Fo", out, src], capture_output=True, text=True)
            assert r.returncode == 0, r.stdout[-500:]
            assert open(out, "rb").read() == _fx(name), "%s.fxo is not what fxc makes of %s.fx" % (name, name)


def test_behaves_as_source():
    tool = _mgfxc()
    exe = _shadercheck()
    env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor")
    with tempfile.TemporaryDirectory() as d:
        for src in sorted(glob.glob(os.path.join(DATA, "*.fx"))):
            name = os.path.splitext(os.path.basename(src))[0]
            original = os.path.join(d, name + "_pc.fx")
            with open(src) as f, open(original, "w") as g:
                g.write(f.read().replace("row_major ", ""))    # MonoGame takes no row_major parameter: the same
            recovered = os.path.join(d, name + "_recovered.fx")    # function, for a matrix set by its elements
            with open(recovered, "w") as f:
                f.write(xbox_effect.to_fx(_fx(name), name))
            a, b, result = (os.path.join(d, name + s) for s in ("_a.mgfxo", "_b.mgfxo", ".json"))
            _compile(tool, original, a)
            _compile(tool, recovered, b)
            subprocess.run([exe, a, b, result], env=env, timeout=180)
            with open(result) as f:
                compared = json.load(f)
            assert "error" not in compared, (name, compared)
            for technique, r in compared.items():
                assert r["drawn"] > 0, (name, technique, "nothing drawn: the comparison would prove nothing")
                assert r["worst"] <= 1, (name, technique, r)


def test_unsupported_names_itself():
    s = shader.read([v for v in effect.read(_fx("sprite")).objects.values() if v[:4] == b"\x10\x2a\x11\x00"][0])
    s.words = list(s.words)
    s.words[1] = (s.words[1] & ~0xF000) | (9 << 12)         # turn the first CF into a call
    try:
        xenos_hlsl.translate(s, "Broken")
    except xenos_hlsl.Unsupported as e:
        assert "Broken" in str(e) and "call" in str(e), e
    else:
        raise AssertionError("a call was translated")


def main():
    for fn in (test_effect_layout, test_shader_tables, test_recovered_hlsl, test_stock_effect_arrays,
               test_real_effects_recover,
               test_mgfx_parameters, test_fixtures_current, test_behaves_as_source, test_unsupported_names_itself):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
