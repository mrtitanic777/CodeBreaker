#!/usr/bin/env python3
"""The XNA lane's content side: XNB files (formats/xnb.py) with the LZX decoder (formats/lzx.py), and
the DXT decoder they share with the Xbox 360 textures (formats/xbox/texture.py). An XNB texture
built here reads back pixel for pixel in both byte orders; a DXT1 block whose first colour is not
above its second gives transparent black at index 3; and -- when Arcadecraft's unpacked game is on
this machine (../Arcadecraft/Arcadecraft) -- every texture and font of it converts and one of its
libraries decompiles to a C# project. The C# fix-ups (xna/fixups.py) are checked on the forms they
rewrite.

    python tests/test_xna.py"""
import glob
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.formats import xnb                  # noqa: E402
from codebreaker.formats.xbox import texture         # noqa: E402
from codebreaker.xna import fixups, run              # noqa: E402

GAME = os.path.join(os.path.dirname(ROOT), "Arcadecraft", "Arcadecraft")
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
    assert not run.is_managed(sys.executable)
    lib = os.path.join(GAME, "FirebaseEncryptionLib.dll")
    if not os.path.isfile(lib):
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    assert run.is_managed(lib)
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


def test_xact_sound_bank():
    from codebreaker.formats.xbox import xsb
    bank = os.path.join(GAME, "Sound Bank.xsb")
    if not os.path.isfile(bank):
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    b = xsb.read(bank)
    assert b["wave_banks"] == ["Wave Bank", "Music Wave Bank"] and len(b["cues"]) == 106
    assert all(c["waves"] for c in b["cues"].values())


def test_sound_effect_xma():
    import shutil
    sound = os.path.join(os.path.dirname(ROOT), "CodeBreaker", "Converted", "BlockWorld", "assets", "game", "584E07D1",
                         "Content", "Audio", "AMP_AssaultRifle.xnb")
    if not os.path.isfile(sound):
        raise Skip("no Converted/BlockWorld here")
    if not shutil.which("ffmpeg"):
        raise Skip("ffmpeg is not on PATH")
    with tempfile.TemporaryDirectory() as d:
        rec = xnb.convert(sound, d)
        assert rec.get("kind") == "sound" and rec["codec"] == "XMA2" and rec["rate"] == 44100, rec
        with open(os.path.join(d, "AMP_AssaultRifle.wav"), "rb") as f:
            head = f.read(44)
        assert head[:4] == b"RIFF" and struct.unpack_from("<H", head, 20)[0] in (1, 0xFFFE)   # PCM (ffmpeg: extensible)


def main():
    for fn in (test_texture_both_byte_orders, test_dxt1_punch_through, test_real_game_converts, test_fixups,
               test_managed_and_decompile, test_port_rewrites, test_port_error_fixers, test_xact_sound_bank,
               test_sound_effect_xma):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
