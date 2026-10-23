"""port.py -- an XNA game's decompiled C# (Converted/<name>/src) built and run on a PC with MonoGame.

    codebreaker xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]
    from codebreaker.xna import port
    result = port.build(project_dir)        {"exe", "errors": {code: count}, "first": [error lines]}

The project's src/ stays the decompiled source. The port is built from a copy in build/port/:

  src/<assembly>/      each assembly's C#, rewritten for the PC where it must be:
                         - the Game class's constructor first calls XnaPort.Runtime.Install(this) (the content
                           manager below), and `new GraphicsDeviceManager(this)` is XnaPort.Runtime.Graphics(this)
                           (HiDef, as the Xbox 360 was);
                         - a file using XACT (AudioEngine, WaveBank, SoundBank, Cue, AudioCategory) gets `using`
                           aliases to XnaPort.Xact (xna/runtime/Xact.cs), which plays its cues from WAV;
                         - ((GameComponent)this).Update(...) and the other lifecycle calls ILSpy writes as a cast to
                           the base class are base.Update(...) -- as a cast they call the override again, forever;
  XnaPort/             CodeBreaker's XNA runtime (xna/runtime/*.cs): the content manager, players and the Guide,
                       storage on disk, XACT, Xbox-only calls;
  Stubs/               the rest of XNA's Xbox-side API -- GamerServices, Net, Storage, Avatar -- generated from the
                       XNA 4.0 assemblies' metadata (xna/apistubs; the assemblies are XNA Game Studio's, found in
                       the Windows GAC, read and never copied), members doing nothing;
  <name>.Port.csproj   MonoGame DesktopGL 3.8.1 on .NET 8 (DOTNET_ROLL_FORWARD=LatestMajor), the game's own files
                       copied beside the program, ContentConverted/ the converted content (textures, fonts and
                       sounds the runtime loads in place of the Xbox's .xnb, the XACT banks as WAV, and the
                       songs' music, WMA on the Xbox, as Ogg Vorbis, which MonoGame plays, and the effects'
                       recovered HLSL compiled by mgfxc to Content/shaders/<sha1>.mgfxo); an .xnb
                       naming a type in an XNA assembly MonoGame does not have (Avatar, GamerServices, Net,
                       Storage: the stubs, in this program) names this program instead -- MonoGame would make
                       of "Microsoft.Xna.Framework.Avatar" an assembly name nothing can parse -- and one read by
                       XNA's ReflectiveReader names the runtime's XnaReflectiveReader, which keeps XNA's rules;
  overlay              a folder of the game's own hand work, copied over all of it last (setting `xna_overlays`,
                       <dir>/<name>/; or --overlay): shaders the Xbox had only as microcode, fixes no rewrite makes.
                       Its overlay.json may say more: "replace" [paths removed first], "sources" {path in the port:
                       a folder elsewhere, copied there -- a hand port's own tree}, "drop" [paths removed after: the
                       generic runtime's pieces a hand port replaces], "assets" (a folder copied to assets/ beside
                       the build, where a hand port's runtime looks for open-format assets).

The build runs until it compiles or stops changing: an error the compiler itself names the fix for (CS1620: an
argument passed `ref` where the method wants `out`, or the other way) is fixed and the build run again; what is
left is listed by error code in reports/port.json. `--run` starts the program for that many seconds and keeps its
log (xnaport.log, and what it printed): the first missing piece is what it stops at -- a shader names itself.
`--check-content` has the built program load every .xnb under Content/ through the runtime (models, buffers and
custom types read by MonoGame's readers and the game's own) and lists each one's result in reports/content_check.json.
"""
import json
import os
import re
import shutil
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
RUNTIME = os.path.join(HERE, "runtime")
APISTUBS = os.path.join(HERE, "apistubs")
STUBBED = ("GamerServices", "Net", "Storage", "Avatar")
HAND_WRITTEN = ("Guide", "Gamer", "SignedInGamer", "SignedInGamerCollection", "GamerServicesDispatcher",
                "StorageDevice", "StorageContainer")
MONOGAME = "3.8.1.303"
XACT_TYPES = ("AudioEngine", "WaveBank", "SoundBank", "Cue", "AudioCategory")
LIFECYCLE = ("Update", "Draw", "Initialize", "LoadContent", "UnloadContent", "Dispose", "BeginDraw", "EndDraw")
_ERROR = re.compile(r"^(?P<file>[A-Za-z]:[^(]+\.cs)\((?P<line>\d+),(?P<col>\d+)\): error (?P<code>CS\d+): (?P<text>.*?)(?: \[.*\])?$")


def _env():
    return dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor", DOTNET_CLI_TELEMETRY_OPTOUT="1",
                DOTNET_NOLOGO="1", MSBUILDDISABLENODEREUSE="1")


# ---- the generated XNA API stubs ----------------------------------------------------------------------------------
def _xna_assembly(name):
    root = os.path.join(os.environ.get("WINDIR", r"C:\Windows"), "Microsoft.NET", "assembly")
    for base, _dirs, files in os.walk(root):
        for f in files:
            if f.lower() == ("microsoft.xna.framework.%s.dll" % name).lower():
                return os.path.join(base, f)
    return None


def _stub_tool():
    from codebreaker.base import settings
    out = settings.state("xna", "apistubs")
    os.makedirs(out, exist_ok=True)
    exe = os.path.join(out, "apistubs.exe" if os.name == "nt" else "apistubs")
    src = max(os.path.getmtime(os.path.join(APISTUBS, f)) for f in ("Program.cs", "apistubs.csproj"))
    if not os.path.isfile(exe) or os.path.getmtime(exe) < src:
        r = subprocess.run(["dotnet", "build", APISTUBS, "-c", "Release", "-o", out, "--nologo", "-v", "q"],
                           capture_output=True, text=True, env=_env())
        if r.returncode != 0:
            raise RuntimeError("apistubs did not build: " + (r.stdout + r.stderr)[-800:])
    return exe


def stubs(out_dir):
    """The XNA Xbox-side API stubs, generated once (state/xna/stubs) and copied to `out_dir`."""
    from codebreaker.base import settings
    cache = settings.state("xna", "stubs")
    os.makedirs(cache, exist_ok=True)
    tool = None
    os.makedirs(out_dir, exist_ok=True)
    for name in STUBBED:
        cached = os.path.join(cache, name + ".stubs.cs")
        if not os.path.isfile(cached):
            asm = _xna_assembly(name)
            if asm is None:
                raise RuntimeError("XNA Game Studio 4.0's Microsoft.Xna.Framework.%s.dll is not installed (the stubs are "
                                   "generated from its metadata)" % name)
            tool = tool or _stub_tool()
            r = subprocess.run([tool, asm, cached, "Microsoft.Xna.Framework"] + list(HAND_WRITTEN), capture_output=True,
                               text=True, env=_env())
            if r.returncode != 0:
                raise RuntimeError("apistubs failed on %s: %s" % (name, r.stderr or r.stdout))
        shutil.copy2(cached, os.path.join(out_dir, name + ".stubs.cs"))


# ---- source rewrites ------------------------------------------------------------------------------------------------
_GAME_CLASS = re.compile(r"\bclass\s+(\w+)\s*:\s*(?:Microsoft\.Xna\.Framework\.)?Game\b")
_BASE_CAST = re.compile(r"\(\((?:[\w.]+\.)?(?:GameComponent|DrawableGameComponent|Game|IDisposable)\)\s*this\)\.(%s)\("
                        % "|".join(LIFECYCLE))


def declared_types(folder):
    """Names of the types the game's C# under `folder` declares (classes, structs, enums, interfaces, delegates)."""
    names = set()
    rx = re.compile(r"\b(?:class|struct|enum|interface)\s+(\w+)")
    for base, _dirs, files in os.walk(folder):
        for f in files:
            if f.endswith(".cs"):
                names.update(rx.findall(open(os.path.join(base, f), encoding="utf-8-sig", errors="replace").read()))
    return names


def rewrite(text, own=frozenset()):
    """The port's changes to one file of the game's C#: (text, [what changed]). `own`: type names the game declares
    itself (never aliased away)."""
    changed = []
    m = _GAME_CLASS.search(text)
    if m:
        name = m.group(1)
        ctor = re.compile(r"\b(?:public|protected|internal|private)?\s*%s\s*\([^)]*\)\s*(?::\s*base\s*\([^)]*\)\s*)?\{" % re.escape(name))
        cm = ctor.search(text, m.end())
        if cm and "XnaPort.Runtime.Install" not in text:
            text = text[:cm.end()] + "\n\t\tXnaPort.Runtime.Install(this);" + text[cm.end():]
            changed.append("Install in %s()" % name)
    text, n = re.subn(r"\bnew\s+(?:Microsoft\.Xna\.Framework\.)?GraphicsDeviceManager\s*\(\s*this\s*\)",
                      "XnaPort.Runtime.Graphics(this)", text)
    if n:
        changed.append("GraphicsDeviceManager x%d" % n)
    xact = [t for t in XACT_TYPES if t not in own]
    if xact and "Microsoft.Xna.Framework.Audio" in text and any(re.search(r"\b%s\b" % t, text) for t in xact):
        aliases = "".join("using %s = XnaPort.Xact.%s;\n" % (t, t) for t in xact)
        text = re.sub(r"(?m)^(using Microsoft\.Xna\.Framework\.Audio;\s*\n)", lambda mm: mm.group(1) + aliases, text, count=1)
        text = re.sub(r"\bMicrosoft\.Xna\.Framework\.Audio\.(%s)\b" % "|".join(xact), r"XnaPort.Xact.\1", text)
        changed.append("XACT aliases")
    # the game's own content managers read through the runtime's, which knows the converted content
    text, n = re.subn(r"(\bclass\s+\w+\s*(?:<[^>]*>)?\s*:\s*)(?:Microsoft\.Xna\.Framework\.Content\.)?ContentManager\b",
                      r"\1XnaPort.ContentManager", text)
    text, n2 = re.subn(r"\bnew\s+(?:Microsoft\.Xna\.Framework\.Content\.)?ContentManager\s*\(", "new XnaPort.ContentManager(", text)
    if n + n2:
        changed.append("content manager x%d" % (n + n2))
    text, n = _BASE_CAST.subn(r"base.\1(", text)
    if n:
        changed.append("base calls x%d" % n)
    # an effect made from the game's own bytes (Xbox 360 microcode) gets the PC's compiled one instead
    text, n = _effect_bytes(text)
    if n:
        changed.append("effect bytes x%d" % n)
    # a stock effect's ShaderIndex picks its technique on the PC (the effect has one per value)
    if _DERIVES_EFFECT.search(text):
        m = re.search(r"\b(\w+)\s*=\s*(?:base\.|this\.)?Parameters\s*\[\s*\"ShaderIndex\"\s*\]", text)
        if m:
            text, n = re.subn(r"\b%s\.SetValue\s*\(" % re.escape(m.group(1)),
                              "XnaPort.Effects.SetShaderIndex(this, %s, " % m.group(1), text)
            if n:
                changed.append("ShaderIndex x%d" % n)
    # the controller is read through the runtime's, which adds the keyboard and scripted presses
    if "GamePad" not in own:
        text, n = re.subn(r"\b(?:Microsoft\.Xna\.Framework\.Input\.)?GamePad\.GetState\s*\(", "XnaPort.Input.GetState(", text)
        if n:
            changed.append("GamePad.GetState x%d" % n)
    return text, changed


_NEW_EFFECT = re.compile(r"\bnew\s+(?:Microsoft\.Xna\.Framework\.Graphics\.)?Effect\s*\(")
_DERIVES_EFFECT = re.compile(r"\bclass\s+\w+\s*:\s*(?:Microsoft\.Xna\.Framework\.Graphics\.)?Effect\b")
_BASE_CALL = re.compile(r"\)\s*:\s*base\s*\(")


def _effect_bytes(text):
    """`new Effect(device, bytes)`, and `: base(device, bytes)` in a class deriving from Effect, with the bytes
    passed through XnaPort.Effects.ForPC (generic: any other argument goes through as it is). -> (text, wrapped)"""
    spots = [m.end() - 1 for m in _NEW_EFFECT.finditer(text)]
    if _DERIVES_EFFECT.search(text):
        spots += [m.end() - 1 for m in _BASE_CALL.finditer(text)]
    n = 0
    for paren in sorted(spots, reverse=True):
        spans, _ = _args(text, paren)
        if not spans or len(spans) != 2:
            continue
        a, b = spans[1]
        arg = text[a:b].strip()
        if arg.startswith("XnaPort.Effects.ForPC("):
            continue
        lead = len(text[a:b]) - len(text[a:b].lstrip())
        text = text[:a + lead] + "XnaPort.Effects.ForPC(" + arg + ")" + text[b:]
        n += 1
    return text, n


def fix_access(errors):
    """CS0507 (an override changes the access it overrides): one project holds what were two assemblies, where a
    `protected` override of a `protected internal` member was right; here it must say `protected internal`. -> fixes."""
    n = 0
    for e in errors:
        if e["code"] == "CS0507" and "'protected internal' inherited member" in e["text"]:
            lines = open(e["file"], encoding="utf-8-sig").read().split("\n")
            ln = int(e["line"]) - 1
            fixed = re.sub(r"\bprotected\s+override\b", "protected internal override", lines[ln], count=1)
            if fixed != lines[ln]:
                lines[ln] = fixed
                n += 1
                with open(e["file"], "w", encoding="utf-8") as h:
                    h.write("\n".join(lines))
    return n


def _args(text, open_paren):
    """The top-level arguments of the call whose '(' is at open_paren: ([(start, end)], index after ')')."""
    depth, start, spans, quote = 0, open_paren + 1, [], None
    for i in range(open_paren, len(text)):
        c = text[i]
        if quote:
            if c == quote and text[i - 1] != "\\":
                quote = None
            continue
        if c in "\"'":
            quote = c
        elif c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                if text[start:i].strip():
                    spans.append((start, i))
                return spans, i + 1
        elif c == "," and depth == 1:
            spans.append((start, i))
            start = i + 1
    return None, -1


def _edit_lines(errors, code, wanted, edit):
    """Each error line of `code` whose text has `wanted`: edit(line) -> new line. -> fixes."""
    by_file = {}
    for e in errors:
        if e["code"] == code and wanted in e["text"]:
            by_file.setdefault(e["file"], set()).add(int(e["line"]))
    n = 0
    for path, lines_at in by_file.items():
        lines = open(path, encoding="utf-8-sig").read().split("\n")
        for ln in sorted(lines_at):
            new = edit(lines[ln - 1])
            if new != lines[ln - 1]:
                lines[ln - 1] = new
                n += 1
        with open(path, "w", encoding="utf-8") as h:
            h.write("\n".join(lines))
    return n


def fix_color(errors):
    """CS0121 between Color(int, ...) and Color(byte, ...): MonoGame has both, XNA had the int one; the arguments
    are cast to int so the call is XNA's. -> fixes."""
    def edit(line):
        out, i = line, 0
        while True:
            m = re.search(r"\bnew\s+(?:Microsoft\.Xna\.Framework\.)?Color\s*\(", out[i:])
            if not m:
                return out
            at = i + m.end() - 1
            spans, end = _args(out, at)
            if not spans or len(spans) not in (3, 4) or "(int)(" in out[at:end]:
                i = at + 1
                continue
            body = ", ".join("(int)(%s)" % out[a:b].strip() for a, b in spans)
            out = out[:at + 1] + body + ")" + out[end:]
            i = at + len(body) + 2
    return _edit_lines(errors, "CS0121", "Color.Color(byte", edit)


def fix_vertex_binding(errors):
    """CS0029 VertexBuffer -> VertexBufferBinding: XNA converted it implicitly, MonoGame does not."""
    def edit(line):
        m = re.search(r"new\s+VertexBufferBinding\s*\[[^\]]*\]\s*\{([^}]*)\}", line)
        if not m:
            return line
        items = ", ".join(x.strip() if x.strip().startswith("new ") else "new VertexBufferBinding(%s)" % x.strip()
                          for x in m.group(1).split(",") if x.strip())
        return line[:m.start(1)] + " " + items + " " + line[m.end(1):]
    return _edit_lines(errors, "CS0029", "to 'Microsoft.Xna.Framework.Graphics.VertexBufferBinding'", edit)


def extension_methods(folder):
    """{method name: [(full class name, first parameter's type)]}: the game's own extension methods."""
    out = {}
    ns_rx = re.compile(r"^\s*namespace\s+([\w.]+)", re.M)
    cls_rx = re.compile(r"\bstatic\s+(?:partial\s+)?class\s+(\w+)")
    ext_rx = re.compile(r"\bstatic\s+[\w<>\[\],. ]+?\s+(\w+)\s*(?:<[^>]*>)?\s*\(\s*this\s+(?:ref\s+)?([\w.<>\[\]]+)")
    for base, _dirs, files in os.walk(folder):
        for f in files:
            if not f.endswith(".cs"):
                continue
            text = open(os.path.join(base, f), encoding="utf-8-sig", errors="replace").read()
            ns = ns_rx.search(text)
            for cm in cls_rx.finditer(text):
                cls = (ns.group(1) + "." if ns else "") + cm.group(1)
                end = text.find("\nclass ", cm.end())
                for em in ext_rx.finditer(text, cm.end(), end if end > 0 else len(text)):
                    out.setdefault(em.group(1), []).append(("global::" + cls, em.group(2)))
    return out


def fix_shadowed_extensions(errors, extensions):
    """A call that means one of the game's extension methods but finds a MonoGame instance method of the same name
    (Rectangle.Inflate(float, float) returns void in MonoGame; the game's Inflate returns the rectangle): the
    extension called by its class. -> fixes."""
    if not extensions:
        return 0
    names = "|".join(re.escape(n) for n in extensions)
    rx = re.compile(r"((?:[A-Za-z_]\w*)(?:\.[A-Za-z_]\w*|\[[^\]]*\])*)\.(%s)\s*\(" % names)

    def edit(line):
        out, i = line, 0
        while True:
            m = rx.search(out, i)
            if not m:
                return out
            receiver, name = m.group(1), m.group(2)
            cls = extensions[name][0][0]
            if receiver.split(".")[0] in ("this", "base") or receiver.endswith(cls.split(".")[-1]):
                i = m.end()
                continue
            at = m.end() - 1
            spans, end = _args(out, at)
            if spans is None:
                return out
            args = ", ".join([receiver] + [out[a:b].strip() for a, b in spans])
            call = "%s.%s(%s)" % (cls, name, args)
            out = out[:m.start()] + call + out[end:]
            i = m.start() + len(call)
    n = 0
    for code, wanted in (("CS0029", "type 'void'"), ("CS1503", "from 'void'"), ("CS0023", "type 'void'")):
        n += _edit_lines(errors, code, wanted, edit)
    return n


# Named arguments whose parameter MonoGame spells otherwise than XNA did.
RENAMED_PARAMETERS = {"mipMap": "mipmap", "vector1": "value1", "vector2": "value2"}


def fix_named_arguments(errors):
    """CS1739 (no parameter of that name): XNA's name for the argument -> MonoGame's. -> fixes."""
    n = 0
    for old, new in RENAMED_PARAMETERS.items():
        n += _edit_lines(errors, "CS1739", "named '%s'" % old, lambda line: re.sub(r"\b%s\s*:" % old, new + ":", line))
    return n


def fix_moved_members(errors):
    """CS1061 for an XNA member MonoGame keeps elsewhere: GraphicsDevice.ReferenceStencil (in the
    DepthStencilState: XnaPort.Compat), RenderTargetBinding.CubeMapFace (its ArraySlice). -> fixes."""
    def stencil(line):
        line = re.sub(r"([\w.\[\]()]+)\.ReferenceStencil\s*=\s*([^;]+);", r"XnaPort.Compat.SetReferenceStencil(\1, \2);", line)
        return re.sub(r"([\w.\[\]()]+)\.ReferenceStencil\b", r"XnaPort.Compat.GetReferenceStencil(\1)", line)

    def cube_face(line):
        return re.sub(r"([\w.\[\]()]+)\.CubeMapFace\b", r"((Microsoft.Xna.Framework.Graphics.CubeMapFace)\1.ArraySlice)", line)
    return (_edit_lines(errors, "CS1061", "definition for 'ReferenceStencil'", stencil) +
            _edit_lines(errors, "CS1061", "definition for 'CubeMapFace'", cube_face))


def fix_ref_out(errors):
    """CS1620 (an argument needs `ref` / `out`): the keyword at the column the compiler names, flipped. -> fixes."""
    want = {}
    for e in errors:
        if e["code"] == "CS1620":
            m = re.search(r"must be passed with the '(ref|out)' keyword", e["text"])
            if m:
                want.setdefault(e["file"], {})[(int(e["line"]), int(e["col"]))] = m.group(1)
    n = 0
    for path, sites in want.items():
        lines = open(path, encoding="utf-8-sig").read().split("\n")
        for (ln, col), kw in sites.items():
            line = lines[ln - 1]
            at = col - 1
            for other in ("ref ", "out ", "in "):
                if line[at:at + len(other)] == other:
                    lines[ln - 1] = line[:at] + kw + " " + line[at + len(other):]
                    n += 1
                    break
            else:
                pos = line.find(" ", at)
                if line[at:].startswith(("ref", "out")):
                    lines[ln - 1] = line[:at] + kw + line[at + 3:]
                    n += 1
        with open(path, "w", encoding="utf-8") as f:
            f.write("\n".join(lines))
    return n


# ---- content ----------------------------------------------------------------------------------------------------
def convert_xact(game_dir, converted):
    """Every .xwb as <bank file>/stream_NNN.wav (+ bank.txt, the bank's own name) and every .xsb as <file>.cues.json,
    under `converted` at the same relative folders. -> counts."""
    from codebreaker.formats.xbox import xma, xsb, xwb
    counts = {"wave banks": 0, "sound banks": 0}
    for base, _dirs, files in os.walk(game_dir):
        for f in files:
            low, src = f.lower(), os.path.join(base, f)
            rel = os.path.relpath(base, game_dir)
            if low.endswith(".xwb"):
                out = os.path.join(converted, rel, os.path.splitext(f)[0])
                if not os.path.isfile(os.path.join(out, "bank.txt")):
                    xma.decode_bank(src, out)
                    with open(os.path.join(out, "bank.txt"), "w", encoding="utf-8") as h:
                        h.write(xwb.XWB(open(src, "rb").read()).bank_name)
                counts["wave banks"] += 1
            elif low.endswith(".xsb"):
                os.makedirs(os.path.join(converted, rel), exist_ok=True)
                with open(os.path.join(converted, rel, os.path.splitext(f)[0] + ".cues.json"), "w", encoding="utf-8") as h:
                    json.dump(xsb.read(src), h, indent=1)
                counts["sound banks"] += 1
    return counts


def compile_effects(converted, shaders):
    """Every recovered effect (.fx under `converted`, its first line "// sha1 <the Xbox effect's sha1>") compiled
    by MonoGame's mgfxc to `shaders`/<sha1>.mgfxo, which the runtime loads for that effect (cached by the
    source's contents under state/xna/mgfxo). -> {"compiled", "failed": {file: why}}"""
    import hashlib
    from codebreaker.base import settings
    from codebreaker.xna import mgfx
    cache = settings.state("xna", "mgfxo")
    os.makedirs(cache, exist_ok=True)
    os.makedirs(shaders, exist_ok=True)
    tool = shutil.which("mgfxc")
    report = {"compiled": 0, "failed": {}}
    for base, _dirs, files in os.walk(converted):
        for f in files:
            if not f.endswith(".fx"):
                continue
            src = os.path.join(base, f)
            with open(src, encoding="utf-8") as h:
                text = h.read()
            first = text.split("\n", 1)[0]
            if not first.startswith("// sha1 "):
                continue
            digest = first[len("// sha1 "):].strip()
            built = os.path.join(cache, hashlib.sha1(text.encode("utf-8")).hexdigest() + ".mgfxo")
            if not os.path.isfile(built):
                if tool is None:
                    report["failed"][os.path.relpath(src, converted)] = "mgfxc is not installed (NOT_IN_REPO.md)"
                    continue
                r = subprocess.run([tool, src, built, "/Profile:OpenGL"], capture_output=True, text=True, env=_env())
                if r.returncode != 0 or not os.path.isfile(built):
                    report["failed"][os.path.relpath(src, converted)] = (r.stdout + r.stderr).strip()[-600:]
                    continue
            data = open(built, "rb").read()
            params = src + ".json"
            if os.path.isfile(params):
                with open(params, encoding="utf-8") as h:
                    data = mgfx.add_parameters(data, json.load(h))
            with open(os.path.join(shaders, digest + ".mgfxo"), "wb") as h:
                h.write(data)
            report["compiled"] += 1
    return report


def convert_songs(game_dir, converted):
    """Every song's music (.wma, beside its .xnb) as Ogg Vorbis under `converted`, at the same relative folder --
    what MonoGame plays on the PC (XnaPort.XboxSongReader looks there). -> songs"""
    from codebreaker.formats.xbox import xma
    n = 0
    for base, _dirs, files in os.walk(game_dir):
        for f in files:
            if not f.lower().endswith(".wma"):
                continue
            out = os.path.join(converted, os.path.relpath(base, game_dir), os.path.splitext(f)[0] + ".ogg")
            if not os.path.isfile(out):
                os.makedirs(os.path.dirname(out), exist_ok=True)
                r = subprocess.run([xma._need_ffmpeg(), "-y", "-hide_banner", "-loglevel", "error", "-i",
                                    os.path.join(base, f), "-c:a", "libvorbis", "-q:a", "5", out],
                                   capture_output=True, text=True)
                if r.returncode != 0:
                    raise RuntimeError("ffmpeg could not convert %s: %s" % (f, r.stderr.strip()[-300:]))
            n += 1
    return n


# ---- the project ------------------------------------------------------------------------------------------------
def _csproj(name, exe_rel, assemblies, defines):
    sources = '    <Compile Include="src/**/*.cs" Exclude="src/**/obj/**;src/**/bin/**;src/**/AssemblyInfo.cs" />'
    return """<Project Sdk="Microsoft.NET.Sdk">
  <!-- Written by CodeBreaker's xna port step (codebreaker/xna/port.py). -->
  <PropertyGroup>
    <OutputType>WinExe</OutputType>
    <TargetFramework>net8.0</TargetFramework>
    <RollForward>Major</RollForward>
    <Nullable>disable</Nullable>
    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>
    <GenerateAssemblyInfo>false</GenerateAssemblyInfo>
    <EnableDefaultCompileItems>false</EnableDefaultCompileItems>
    <AssemblyName>%s</AssemblyName>
    <RootNamespace>%s</RootNamespace>
    <DefineConstants>WINDOWS;XNAPORT%s</DefineConstants>
    <NoWarn>$(NoWarn);CS0108;CS0114;CS0162;CS0168;CS0169;CS0219;CS0414;CS0436;CS0649;CS0067;CS0618;CS1998;CA1416</NoWarn>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="MonoGame.Framework.DesktopGL" Version="%s" />
  </ItemGroup>
  <ItemGroup>
%s
    <Compile Include="XnaPort/**/*.cs" />
    <Compile Include="Stubs/**/*.cs" />
    <Compile Include="Port/**/*.cs" Exclude="Port/**/obj/**;Port/**/bin/**" />
  </ItemGroup>
  <ItemGroup>
    <None Include="game/**/*" Exclude="game/**/*.dll;game/**/*.exe;game/**/*.pdb;game/*.xml" CopyToOutputDirectory="PreserveNewest" Link="%%(RecursiveDir)%%(Filename)%%(Extension)" />
    <None Include="ContentConverted/**/*" CopyToOutputDirectory="PreserveNewest" Link="ContentConverted/%%(RecursiveDir)%%(Filename)%%(Extension)" />
    <None Include="Content/**/*" CopyToOutputDirectory="PreserveNewest" Link="Content/%%(RecursiveDir)%%(Filename)%%(Extension)" />
  </ItemGroup>
</Project>
""" % (name, name, "".join(";" + d for d in defines), MONOGAME, sources)


def _overlay_dir(project, overlay):
    if overlay:
        return overlay
    from codebreaker.base import settings
    root = settings.get("xna_overlays") or settings.state("xna", "overlays")
    d = os.path.join(root, project.name)
    return d if os.path.isdir(d) else None


def _copy_tree(src, dst):
    """src over dst; a file already there with the same size and time is left as it is."""
    for base, dirs, files in os.walk(src):
        dirs[:] = [d for d in dirs if d not in ("obj", "bin", ".git")]
        rel = os.path.relpath(base, src)
        os.makedirs(os.path.join(dst, rel), exist_ok=True)
        for f in files:
            if f == "overlay.json":
                continue
            a, b = os.path.join(base, f), os.path.join(dst, rel, f)
            if os.path.isfile(b):
                sa, sb = os.stat(a), os.stat(b)
                if sa.st_size == sb.st_size and int(sa.st_mtime) == int(sb.st_mtime):
                    continue
            shutil.copy2(a, b)


# XNA assemblies whose types MonoGame does not have; the port's stubs give them, in the program itself
STUB_ASSEMBLIES = re.compile(r", Microsoft\.Xna\.Framework\.(?:Avatar|GamerServices|Net|Storage)(?:, Version=[^\],]*)?"
                             r"(?:, Culture=[^\],]*)?(?:, PublicKeyToken=[^\],]*)?")


_REFLECTIVE = "Microsoft.Xna.Framework.Content.ReflectiveReader`1["


def port_reader_name(name, program):
    """A type reader's name in an .xnb as the port reads it (see the module's notes)."""
    name = STUB_ASSEMBLIES.sub(", " + program, name)
    if name.startswith(_REFLECTIVE):
        name = "XnaPort.XnaReflectiveReader`1[" + name[len(_REFLECTIVE):] + ", " + program
    return name


def port_reader_names(game_dir, program):
    """Each .xnb under game_dir rewritten to port_reader_name's names (once: a stamp records it). -> files
    rewritten"""
    from codebreaker.formats import xnb
    stamp = os.path.join(os.path.dirname(game_dir), "reader-names.stamp")
    if os.path.isfile(stamp):
        return 0
    n = 0
    for base, _dirs, files in os.walk(game_dir):
        for f in files:
            if not f.lower().endswith(".xnb"):
                continue
            path = os.path.join(base, f)
            with open(path, "rb") as h:
                raw = h.read()
            if raw[:3] != b"XNB":
                continue
            new = xnb.rename_readers(raw, lambda name: port_reader_name(name, program))
            if new is not None:
                with open(path, "wb") as h:
                    h.write(new)
                n += 1
    with open(stamp, "w") as h:
        h.write("%d\n" % n)
    return n


def prepare(project, overlay=None):
    """build/port/ written from the project. -> (port dir, project name, report)"""
    from codebreaker.xna import run
    game_dir = os.path.join(project.assets, "game")
    games = run.assemblies(game_dir)
    if not games:
        raise ValueError("%s: no game assemblies (run `codebreaker xna` on the game first)" % project.root)
    exe = next((g for g in games if g.lower().endswith(".exe")), games[0])
    exe_dir = os.path.dirname(exe)
    exe_rel = os.path.relpath(exe_dir, game_dir)
    name = os.path.splitext(os.path.basename(exe))[0].replace(" ", "")
    port = os.path.join(project.build, "port")
    os.makedirs(port, exist_ok=True)
    report = {"rewrites": {}, "content": {}}
    # the game's C#, rewritten
    assemblies = []
    own = declared_types(project.src)
    for asm in sorted(os.listdir(project.src)):
        src = os.path.join(project.src, asm)
        if not os.path.isdir(src):
            continue
        dst = os.path.join(port, "src", asm)
        if os.path.isdir(dst):
            shutil.rmtree(dst)
        _copy_tree(src, dst)
        for base, _dirs, files in os.walk(dst):
            for f in files:
                if f.endswith(".csproj"):
                    os.remove(os.path.join(base, f))
                    continue
                if not f.endswith(".cs"):
                    continue
                p = os.path.join(base, f)
                text = open(p, encoding="utf-8-sig").read()
                new, changed = rewrite(text, own)
                if changed:
                    report["rewrites"][os.path.relpath(p, port)] = changed
                    with open(p, "w", encoding="utf-8") as h:
                        h.write(new)
        assemblies.append(asm)
    # the runtime and the stubs
    rt = os.path.join(port, "XnaPort")
    os.makedirs(rt, exist_ok=True)
    for f in os.listdir(RUNTIME):
        if f.endswith(".cs"):
            shutil.copy2(os.path.join(RUNTIME, f), os.path.join(rt, f))
    stubs(os.path.join(port, "Stubs"))
    # the game's own files beside the program, and the converted content
    game_link = os.path.join(port, "game")
    if not os.path.isdir(game_link):
        _copy_tree(exe_dir, game_link)
    report["xnb reader names"] = port_reader_names(game_link, name)
    converted = os.path.join(port, "ContentConverted")
    content_src = os.path.join(project.assets, "content", exe_rel)
    if os.path.isdir(content_src):
        _copy_tree(content_src, converted)
    os.makedirs(converted, exist_ok=True)
    report["content"] = convert_xact(exe_dir, converted)
    report["content"]["songs"] = convert_songs(exe_dir, converted)
    report["content"]["effects"] = compile_effects(converted, os.path.join(port, "Content", "shaders"))
    # the game's own hand work
    ov = _overlay_dir(project, overlay)
    if ov:
        manifest = {}
        if os.path.isfile(os.path.join(ov, "overlay.json")):
            with open(os.path.join(ov, "overlay.json"), encoding="utf-8") as h:
                manifest = json.load(h)
        for rel in manifest.get("replace", []):
            shutil.rmtree(os.path.join(port, rel), ignore_errors=True)
        for rel, folder in manifest.get("sources", {}).items():
            _copy_tree(folder, os.path.join(port, rel))
        _copy_tree(ov, port)
        for rel in manifest.get("drop", []):
            p = os.path.join(port, rel)
            if os.path.isdir(p):
                shutil.rmtree(p)
            elif os.path.isfile(p):
                os.remove(p)
        if manifest.get("assets") and not os.path.isdir(os.path.join(port, "assets")):
            _copy_tree(manifest["assets"], os.path.join(port, "assets"))
        report["overlay"] = ov
    with open(os.path.join(port, name + ".Port.csproj"), "w", encoding="utf-8") as h:
        h.write(_csproj(name, exe_rel, assemblies, ()))
    return port, name, report


def _build_once(port, name):
    r = subprocess.run(["dotnet", "build", os.path.join(port, name + ".Port.csproj"), "-c", "Release", "--nologo",
                        "-v", "q", "-clp:NoSummary"], capture_output=True, text=True, env=_env(), cwd=port)
    errors, seen = [], set()
    for line in (r.stdout + "\n" + r.stderr).splitlines():
        m = _ERROR.match(line.strip())
        if m and line.strip() not in seen:
            seen.add(line.strip())
            errors.append(m.groupdict())
    return r.returncode == 0 and not errors, errors, (r.stdout + r.stderr)


def build(project, overlay=None, rounds=6):
    port, name, report = prepare(project, overlay)
    ok, errors, out = False, [], ""
    report["rounds"] = []
    extensions = extension_methods(os.path.join(port, "src"))
    for _ in range(rounds):
        ok, errors, out = _build_once(port, name)
        fixed = 0 if ok else (fix_ref_out(errors) + fix_access(errors) + fix_color(errors) + fix_vertex_binding(errors) + fix_named_arguments(errors) + fix_moved_members(errors) +
                              fix_shadowed_extensions(errors, extensions))
        report["rounds"].append({"errors": len(errors), "fixed": fixed})
        if ok or not fixed:
            break
    by_code = {}
    for e in errors:
        by_code[e["code"]] = by_code.get(e["code"], 0) + 1
    exe = os.path.join(port, "bin", "Release", "net8.0", name + ".exe")
    result = {"ok": ok, "exe": exe if ok else None, "errors": by_code,
              "first": ["%s(%s): %s %s" % (os.path.relpath(e["file"], port), e["line"], e["code"], e["text"]) for e in errors[:60]],
              "build_output_tail": "" if ok or errors else out[-2000:], **report}
    with open(os.path.join(project.reports, "port.json"), "w", encoding="utf-8") as h:
        json.dump(result, h, indent=1)
    return result


def run_for(exe, seconds, env=None):
    """Start the built game for `seconds`; -> {"exit", "output", "log"} (its own xnaport.log and what it printed)."""
    folder = os.path.dirname(exe)
    log = os.path.join(folder, "xnaport.log")
    if os.path.isfile(log):
        os.remove(log)
    p = subprocess.Popen([exe], cwd=folder, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, env=dict(_env(), **(env or {})))
    try:
        out, _ = p.communicate(timeout=seconds)
        code = p.returncode
    except subprocess.TimeoutExpired:
        p.kill()
        out, _ = p.communicate()
        code = None                                         # still running: it did not stop on its own
    text = open(log, encoding="utf-8", errors="replace").read() if os.path.isfile(log) else ""
    return {"exit": code, "output": out[-6000:], "log": text[-6000:]}


def check_content(exe, seconds=600):
    """Every .xnb under the built program's Content/ loaded by the program itself -> {asset: type or "error: why"},
    or None when it wrote nothing."""
    out = os.path.join(os.path.dirname(exe), "content_check.json")
    if os.path.isfile(out):
        os.remove(out)
    run_for(exe, seconds, {"XNAPORT_CHECK_CONTENT": "1"})
    if not os.path.isfile(out):
        return None
    with open(out, encoding="utf-8") as h:
        return json.load(h)


def main(argv):
    from codebreaker.base import converted
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    target = argv[0]
    path = target if os.path.isdir(target) else os.path.join(converted.root(), target)
    if not os.path.isfile(os.path.join(path, "project.json")):
        print("xna port: %s is not a Converted project" % target)
        return 2
    project = converted.Project(path)
    overlay = argv[argv.index("--overlay") + 1] if "--overlay" in argv else None
    t0 = time.time()
    try:
        result = build(project, overlay)
    except (ValueError, RuntimeError) as e:
        print("xna port: %s" % e)
        return 1
    if result["ok"]:
        print("built %s (%.0f s)" % (result["exe"], time.time() - t0))
    else:
        print("not built: %d errors (%s); reports/port.json lists them" % (
            sum(result["errors"].values()), ", ".join("%s %d" % kv for kv in sorted(result["errors"].items(), key=lambda kv: -kv[1]))))
        for line in result["first"][:15]:
            print("  " + line)
    if result["ok"] and "--run" in argv:
        seconds = int(argv[argv.index("--run") + 1])
        r = run_for(result["exe"], seconds)
        print("ran %d s: %s" % (seconds, "still running" if r["exit"] is None else "exited %s" % r["exit"]))
        print((r["log"] or r["output"])[-2500:])
        with open(os.path.join(project.reports, "port_run.json"), "w", encoding="utf-8") as h:
            json.dump(r, h, indent=1)
    if result["ok"] and "--check-content" in argv:
        checked = check_content(result["exe"])
        if checked is None:
            print("content check: the program wrote no result")
        else:
            failed = {k: v for k, v in checked.items() if v.startswith("error:")}
            other = sum(1 for v in checked.values() if v.startswith(("not an XNB", "empty")))
            print("content check: %d of %d .xnb loaded, %d failed; %d are not XNB content (empty, or a file the "
                  "game reads itself)" % (len(checked) - len(failed) - other, len(checked) - other, len(failed), other))
            for name, why in sorted(failed.items())[:20]:
                print("  %s: %s" % (name, why[len("error: "):][:160]))
            with open(os.path.join(project.reports, "content_check.json"), "w", encoding="utf-8") as h:
                json.dump(checked, h, indent=1)
    project.record("xna port", [result["exe"]] if result["ok"] else [],
                   notes="MonoGame build of the decompiled C# in build/port: %s" % (
                       "built" if result["ok"] else "%d errors left (reports/port.json)" % sum(result["errors"].values())))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
