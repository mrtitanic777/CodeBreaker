#!/usr/bin/env python3
"""The XNA lane's content side: XNB files (formats/xnb.py) with the LZX decoder (formats/lzx.py), and
the DXT decoder they share with the Xbox 360 textures (formats/xbox/texture.py). An XNB texture
built here reads back pixel for pixel in both byte orders; a DXT1 block whose first colour is not
above its second gives transparent black at index 3; and -- when Arcadecraft's unpacked game is on
this machine (../Arcadecraft/Arcadecraft) -- every texture and font of it converts and one of its
libraries decompiles to a C# project. The C# fix-ups (xna/fixups.py) are checked on the forms they
rewrite. An .xnb's type reader names are rewritten as the port reads them (stub assemblies, XNA's
reflective reader). Real Xbox 360 game effects (tests/fixtures/xbox_effects) are wrapped as XNB effects
and converted; a sound effect built here is converted to a WAV.

    python tests/test_xna.py"""
import glob
import json
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.formats import pe, xnb              # noqa: E402
from codebreaker.formats.xbox import texture         # noqa: E402
from codebreaker.xna import fixups, run              # noqa: E402



def _game():
    """Arcadecraft's unpacked game, a sibling of the CodeBreaker checkout (which a worktree sits inside of)."""
    d = os.path.dirname(ROOT)
    while os.path.dirname(d) != d:
        c = os.path.join(d, "Arcadecraft", "Arcadecraft")
        if os.path.isdir(c):
            return c
        d = os.path.dirname(d)
    return os.path.join(os.path.dirname(ROOT), "Arcadecraft", "Arcadecraft")


GAME = _game()
FIXTURES = os.path.join(ROOT, "tests", "fixtures")
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


def _7bit(n):
    out = bytearray()
    while True:
        b = n & 0x7F
        n >>= 7
        out.append(b | (0x80 if n else 0))
        if not n:
            return bytes(out)


def _texture_xnb(platform, pixels, w, h):
    """An uncompressed XNA 4.0 Texture2D: the reader list, no shared objects, the object."""
    reader = b"Microsoft.Xna.Framework.Content.Texture2DReader"
    body = _7bit(1) + _7bit(len(reader)) + reader + struct.pack("<i", 0) + _7bit(0)
    body += _7bit(1) + struct.pack("<iIII", 0, w, h, 1) + struct.pack("<I", len(pixels)) + pixels
    return b"XNB" + platform.encode() + bytes([5, 0]) + struct.pack("<I", 10 + len(body)) + body


def test_texture_both_byte_orders():
    w, h = 2, 1
    want = [(255, 0, 0, 255), (0, 128, 255, 64)]
    pc = b"".join(bytes(p) for p in want)                                   # R, G, B, A
    xbox = b"".join(bytes((p[3], p[2], p[1], p[0])) for p in want)          # A, B, G, R
    with tempfile.TemporaryDirectory() as d:
        for platform, pixels in (("w", pc), ("x", xbox)):
            path = os.path.join(d, "t_%s.xnb" % platform)
            with open(path, "wb") as f:
                f.write(_texture_xnb(platform, pixels, w, h))
            rec = xnb.convert(path, d)
            assert rec.get("kind") == "texture", rec
            from PIL import Image
            img = Image.open(os.path.join(d, "t_%s.png" % platform)).convert("RGBA")
            assert [img.getpixel((x, 0)) for x in range(w)] == want, platform


def test_dxt1_punch_through():
    # c0 = 0x0000 <= c1 = 0xFFFF: three colours and transparent black; every pixel index 3
    block = struct.pack("<HHI", 0x0000, 0xFFFF, 0xFFFFFFFF)
    img = texture.decode_dxt(block, 4, 4, "dxt1", tiled=False, swapped=False)
    assert img.getpixel((0, 0)) == (0, 0, 0, 0)
    opaque = struct.pack("<HHI", 0xFFFF, 0x0000, 0xFFFFFFFF)                 # c0 > c1: index 3 is a colour
    assert texture.decode_dxt(opaque, 4, 4, "dxt1", tiled=False, swapped=False).getpixel((0, 0))[3] == 255


def test_real_game_converts():
    files = sorted(glob.glob(os.path.join(GAME, "*.xnb")))
    if not files:
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    with tempfile.TemporaryDirectory() as d:
        kinds = {}
        for p in files[:120]:
            rec = xnb.convert(p, d)
            assert "error" not in rec, rec
            kinds[rec["kind"]] = kinds.get(rec["kind"], 0) + 1
        assert kinds.get("texture"), kinds


def test_fixups():
    text, n = fixups.ref_casts("((Color)(ref white)).A = 64; x = ((IDisposable)(Enumerator)(ref e)).Dispose();")
    # the same-type cast goes; a real conversion (to the interface) stays
    assert text == "(white).A = 64; x = ((IDisposable)e).Dispose();" and n == 2, (text, n)
    ctor = ("class A : B {\n    public A(int x)\n    {\n        base._002Ector(x);\n        Vector2 v = default;\n"
            "        v._002Ector(1f, 2f);\n    }\n}\n")
    text, n, left = fixups.constructors(ctor)
    assert n == 3 and not left, (n, left)
    assert "public A(int x) : base(x)" in text and "v = new Vector2(1f, 2f);" in text and "_002Ector" not in text, text


def test_managed_and_decompile():
    assert not pe.is_dotnet(sys.executable)
    lib = os.path.join(GAME, "FirebaseEncryptionLib.dll")
    if not os.path.isfile(lib):
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    assert pe.is_dotnet(lib)
    try:
        run.ilspycmd()
    except RuntimeError as e:
        raise Skip(str(e))
    with tempfile.TemporaryDirectory() as d:
        run.decompile(lib, d)
        sources = glob.glob(os.path.join(d, "**", "*.cs"), recursive=True)
        assert sources and glob.glob(os.path.join(d, "*.csproj")), os.listdir(d)
        assert any("class " in open(p, encoding="utf-8-sig").read() for p in sources)


def test_port_rewrites():
    from codebreaker.xna import port
    src = ("using Microsoft.Xna.Framework;\nusing Microsoft.Xna.Framework.Audio;\nusing Microsoft.Xna.Framework.Content;\n"
           "public class MyGame : Game {\n    GraphicsDeviceManager g;\n    AudioEngine engine;\n    WaveBank waves;\n"
           "    public MyGame()\n    {\n        g = new GraphicsDeviceManager(this);\n    }\n"
           "    protected override void Update(GameTime t) { ((Game)this).Update(t); }\n}\n"
           "class Loader : ContentManager { }\n")
    out, changed = port.rewrite(src)
    assert "XnaPort.Runtime.Install(this);" in out and "XnaPort.Runtime.Graphics(this)" in out, out
    assert "using AudioEngine = XnaPort.Xact.AudioEngine;" in out and "base.Update(t)" in out
    assert "class Loader : XnaPort.ContentManager" in out, out
    # a type the game declares itself is not aliased away
    out2, _ = port.rewrite(src, own={"AudioEngine"})
    assert "using AudioEngine =" not in out2 and "using WaveBank = XnaPort.Xact.WaveBank;" in out2


def test_port_error_fixers():
    from codebreaker.xna import port
    with tempfile.TemporaryDirectory() as d:
        p = os.path.join(d, "a.cs")
        lines = ["x = new Color(c.R, c.G, c.B, 0);",
                 "var b = new VertexBufferBinding[2] { vb, inst };",
                 "if (rect.Inflate(0f, y).Contains(v)) { }",
                 "var t = new Texture2D(gd, 4, 4, mipMap: false, SurfaceFormat.Color);"]
        with open(p, "w", encoding="utf-8") as f:
            f.write("\n".join(lines))
        err = lambda line, code, text: {"file": p, "line": str(line), "col": "1", "code": code, "text": text}
        errors = [err(1, "CS0121", "between 'Color.Color(int, int, int, int)' and 'Color.Color(byte, byte, byte, byte)'"),
                  err(2, "CS0029", "Cannot implicitly convert type 'VertexBuffer' to 'Microsoft.Xna.Framework.Graphics.VertexBufferBinding'"),
                  err(3, "CS0023", "Operator '.' cannot be applied to operand of type 'void'"),
                  err(4, "CS1739", "The best overload for 'Texture2D' does not have a parameter named 'mipMap'")]
        assert port.fix_color(errors) == 1 and port.fix_vertex_binding(errors) == 1
        assert port.fix_shadowed_extensions(errors, {"Inflate": [("global::Game.RectExt", "Rectangle")]}) == 1
        assert port.fix_named_arguments(errors) == 1
        got = open(p, encoding="utf-8").read().split("\n")
    assert got[0] == "x = new Color((int)(c.R), (int)(c.G), (int)(c.B), (int)(0));", got[0]
    assert "new VertexBufferBinding(vb), new VertexBufferBinding(inst)" in got[1], got[1]
    assert got[2] == "if (global::Game.RectExt.Inflate(rect, 0f, y).Contains(v)) { }", got[2]
    assert "mipmap: false" in got[3]


def test_rename_readers():
    raw = _texture_xnb("x", bytes(range(16)), 2, 2)
    assert xnb.rename_readers(raw, lambda n: n) is None
    new = xnb.rename_readers(raw, lambda n: n.replace("Texture2DReader", "SomeOtherTextureReader"))
    a, b = xnb.Xnb(raw), xnb.Xnb(new)
    assert b.readers == ["Microsoft.Xna.Framework.Content.SomeOtherTextureReader"] and b.platform == "x"
    assert a.data[a.offset:] == b.data[b.offset:]                      # the object itself as it was


def test_port_reader_names():
    from codebreaker.xna import port
    avatar = ("Microsoft.Xna.Framework.Content.EnumReader`1[[Microsoft.Xna.Framework.GamerServices.AvatarEye, "
              "Microsoft.Xna.Framework.Avatar, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553]]")
    assert port.port_reader_name(avatar, "Game") == \
        "Microsoft.Xna.Framework.Content.EnumReader`1[[Microsoft.Xna.Framework.GamerServices.AvatarEye, Game]]"
    reflective = "Microsoft.Xna.Framework.Content.ReflectiveReader`1[[Lib.Thing, Lib Asm, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]"
    assert port.port_reader_name(reflective, "Game") == \
        "XnaPort.XnaReflectiveReader`1[[Lib.Thing, Lib Asm, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]], Game"
    plain = "Microsoft.Xna.Framework.Content.VertexBufferReader, Microsoft.Xna.Framework.Graphics, Version=4.0.0.0"
    assert port.port_reader_name(plain, "Game") == plain


def test_obfuscated_forms():
    from codebreaker.xna import port
    holder = "internal class X\n{\n    private struct Blob16 { }\n    internal static Blob16 A/* Not supported: data(00) */;\n}"
    text, n = fixups.data_holders(holder)
    assert n == 1 and "internal struct Blob16" in text, text
    assert fixups.data_holders("private struct S { }") == ("private struct S { }", 0)
    with tempfile.TemporaryDirectory() as d:
        p = os.path.join(d, "a.cs")
        with open(p, "w", encoding="utf-8") as f:
            f.write("gd.ReferenceStencil = 3;\nint r = gd.ReferenceStencil;\nvar f = binding.CubeMapFace;")
        err = lambda line, member: {"file": p, "line": str(line), "col": "1", "code": "CS1061",
                                    "text": "'X' does not contain a definition for '%s'" % member}
        assert port.fix_moved_members([err(1, "ReferenceStencil"), err(2, "ReferenceStencil"), err(3, "CubeMapFace")]) == 3
        got = open(p, encoding="utf-8").read().split("\n")
    assert got[0] == "XnaPort.Compat.SetReferenceStencil(gd, 3);", got[0]
    assert got[1] == "int r = XnaPort.Compat.GetReferenceStencil(gd);", got[1]
    assert got[2] == "var f = ((Microsoft.Xna.Framework.Graphics.CubeMapFace)binding.ArraySlice);", got[2]


def test_de4dot_detects():
    if run.de4dot() is None:
        raise Skip("de4dot is not built (NOT_IN_REPO.md)")
    sunburn = glob.glob(os.path.join(GAME, "**", "SynapseGaming-SunBurn-Pro.dll"), recursive=True)
    if not sunburn:
        raise Skip("Arcadecraft's game is not at %s" % GAME)
    assert run.obfuscator(sunburn[0]), "de4dot found no obfuscator in SunBurn"
    plain = [a for a in run.assemblies(os.path.dirname(sunburn[0])) if "SunBurn" not in a and "Synapse" not in a]
    assert plain and run.obfuscator(plain[0]) is None


def _xnb(platform, reader, payload):
    """An uncompressed XNA 4.0 file of one object: the reader list, no shared objects, the payload."""
    name = ("Microsoft.Xna.Framework.Content." + reader).encode()
    body = _7bit(1) + _7bit(len(name)) + name + struct.pack("<i", 0) + _7bit(0) + _7bit(1) + payload
    return b"XNB" + platform.encode() + bytes([5, 0]) + struct.pack("<I", 10 + len(body)) + body


def test_real_effects_convert():
    """Real Xbox 360 effects from games' content (the stock BasicEffect, post-processing, sky, water), wrapped as the
    .xnb a game ships: each one converts to its .fx and parameters."""
    blobs = [p for p in sorted(glob.glob(os.path.join(FIXTURES, "xbox_effects", "*.bin")))
             if not p.endswith("bw_BasicClipEffect.bin")]            # (that one is the PC build)
    assert len(blobs) == 22, len(blobs)
    with tempfile.TemporaryDirectory() as d:
        for p in blobs:
            name = os.path.splitext(os.path.basename(p))[0]
            code = open(p, "rb").read()
            path = os.path.join(d, name + ".xnb")
            with open(path, "wb") as f:
                f.write(_xnb("x", "EffectReader", struct.pack("<i", len(code)) + code))
            rec = xnb.convert(path, os.path.join(d, "out"))
            assert rec.get("kind") == "effect" and "error" not in rec and rec["sha1"], (name, rec)
            fx = open(os.path.join(d, "out", name + ".fx"), encoding="utf-8").read()
            assert fx.startswith("// sha1 " + rec["sha1"]) and "technique " in fx, name
            assert json.load(open(os.path.join(d, "out", name + ".fx.json"), encoding="utf-8")) is not None


def test_port_effect_and_input():
    from codebreaker.xna import port
    src = ("public class Clip : Effect\n{\n    public Clip(GraphicsDevice d) : base(d, Bytes()) { }\n"
           "    public Clip(Clip c) : base(c) { }\n    void Cache() { shaderIndexParam = base.Parameters[\"ShaderIndex\"]; }\n"
           "    void OnApply() { shaderIndexParam.SetValue(num); }\n"
           "    void Make() { var e = new Effect(dev, File.ReadAllBytes(p)); var g = GamePad.GetState(PlayerIndex.One); }\n}\n")
    text, changed = port.rewrite(src)
    assert "base(d, XnaPort.Effects.ForPC(Bytes()))" in text and "base(c)" in text, text
    assert "new Effect(dev, XnaPort.Effects.ForPC(File.ReadAllBytes(p)))" in text, text
    assert "XnaPort.Effects.SetShaderIndex(this, shaderIndexParam, num);" in text, text
    assert "XnaPort.Input.GetState(PlayerIndex.One)" in text, text
    assert port.rewrite(text)[0] == text                     # once only


def test_xact_sound_bank():
    from codebreaker.formats.xbox import xsb
    bank = os.path.join(GAME, "Sound Bank.xsb")
    if not os.path.isfile(bank):
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    b = xsb.read(bank)
    assert b["wave_banks"] == ["Wave Bank", "Music Wave Bank"] and len(b["cues"]) == 106
    assert all(c["waves"] for c in b["cues"].values())


def test_sound_effect():
    """A SoundEffect .xnb (PCM, as a PC game ships them) converts to a plain WAV with its samples. (An XMA2 one
    needs a real XMA2 stream and ffmpeg: no such file is kept here.)"""
    samples = struct.pack("<8h", 0, 1000, 2000, -2000, 3000, -3000, 100, -100)
    fmt = struct.pack("<HHIIHHH", 1, 1, 44100, 88200, 2, 16, 0)
    payload = struct.pack("<i", len(fmt)) + fmt + struct.pack("<i", len(samples)) + samples + struct.pack("<iii", 0, 8, 0)
    with tempfile.TemporaryDirectory() as d:
        path = os.path.join(d, "Beep.xnb")
        with open(path, "wb") as f:
            f.write(_xnb("w", "SoundEffectReader", payload))
        rec = xnb.convert(path, d)
        assert rec.get("kind") == "sound" and rec["codec"] == "PCM" and rec["rate"] == 44100 and rec["channels"] == 1, rec
        with open(os.path.join(d, "Beep.wav"), "rb") as f:
            wav = f.read()
        assert wav[:4] == b"RIFF" and struct.unpack_from("<H", wav, 20)[0] == 1 and wav.endswith(samples), wav[:48]


def main():
    for fn in (test_texture_both_byte_orders, test_dxt1_punch_through, test_real_game_converts, test_fixups,
               test_managed_and_decompile, test_port_rewrites, test_port_error_fixers, test_xact_sound_bank,
               test_sound_effect, test_real_effects_convert, test_port_effect_and_input, test_rename_readers, test_port_reader_names, test_obfuscated_forms,
               test_de4dot_detects):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
