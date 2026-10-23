"""run.py -- `codebreaker xna <package, folder or game .exe> [--name N] [--no-content]`: an XNA game as
a Converted project; `codebreaker xna port <project>` then builds its C# for the PC (xna/port.py).

    Converted/<name>/
      original/   what came in (the package, or the game's .exe)
      assets/     the game's files as it ships them (from a package: unpacked), and content/: every
                  .xnb converted (formats/xnb.py) -- textures and fonts as PNG (+ JSON), at their paths
      src/        C#: one project per assembly of the game's own (ILSpy, then fixups.py)
      reports/    content.json (each .xnb: kind, size, readers, or why it did not convert),
                  decompile.json (each assembly, and what the fix-ups left)

An assembly is the game's own unless it is the framework's: Microsoft.*, System.*, mscorlib. Other
libraries a game ships (a middleware .dll) are decompiled too, into their own project, since they
are what the game was built with. The decompiler is ILSpy's command line (`ilspycmd`, setting
`ilspycmd`; .NET's `dotnet tool install -g ilspycmd`); it is run with DOTNET_ROLL_FORWARD=LatestMajor
so a newer .NET runs it.
"""
import json
import os
import shutil
import subprocess
import sys

FRAMEWORK = ("microsoft.", "system.", "mscorlib", "netstandard")


def ilspycmd():
    from codebreaker.base import settings
    named = settings.get("ilspycmd")
    if named and os.path.isfile(named):
        return named
    found = shutil.which("ilspycmd")
    if found:
        return found
    home = os.path.join(os.path.expanduser("~"), ".dotnet", "tools", "ilspycmd" + (".exe" if os.name == "nt" else ""))
    if os.path.isfile(home):
        return home
    raise RuntimeError("ILSpy's command line is not installed: dotnet tool install -g ilspycmd "
                       "(or name it in the `ilspycmd` setting)")


def is_managed(path):
    """A PE whose CLR runtime header directory (entry 14) is present: a .NET assembly."""
    try:
        with open(path, "rb") as f:
            data = f.read(0x400)
        if data[:2] != b"MZ":
            return False
        pe = int.from_bytes(data[0x3C:0x40], "little")
        if data[pe:pe + 4] != b"PE\0\0":
            return False
        opt = pe + 24
        magic = int.from_bytes(data[opt:opt + 2], "little")
        dirs = opt + (96 if magic == 0x10B else 112)
        rva = int.from_bytes(data[dirs + 14 * 8:dirs + 14 * 8 + 4], "little")
        return rva != 0
    except (OSError, IndexError):
        return False


def assemblies(folder):
    """The game's own assemblies under `folder` ([path]), the .exe first."""
    out = []
    for base, _dirs, files in os.walk(folder):
        for f in files:
            low = f.lower()
            if not low.endswith((".exe", ".dll")) or low.startswith(FRAMEWORK) or ".resources." in low:
                continue
            p = os.path.join(base, f)
            if is_managed(p):
                out.append(p)
    return sorted(out, key=lambda p: (not p.lower().endswith(".exe"), p.lower()))


def decompile(assembly, out_dir):
    env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor")
    # C# 11: newer syntax (C# 12's primary constructors) puts a constructor's assignments into field initializers,
    # where one field may not set another
    r = subprocess.run([ilspycmd(), "-p", "-lv", "CSharp11_0", "-o", out_dir, assembly], capture_output=True, text=True, env=env)
    if r.returncode != 0:
        raise RuntimeError("ilspycmd failed on %s: %s" % (os.path.basename(assembly), (r.stderr or r.stdout).strip()[-500:]))


def convert(source, name=None, content=True):
    from codebreaker.base import converted
    from codebreaker.formats import media, xnb
    from codebreaker.xna import fixups
    source = os.path.abspath(source)
    if os.path.isdir(source):
        games = assemblies(source)
        if not games:
            raise ValueError("%s: no .NET assembly in it" % source)
        p = converted.project(games[0], name=name)
        game_dir = os.path.join(p.assets, "game")
        if not os.path.isdir(game_dir):
            shutil.copytree(source, game_dir)
    elif is_managed(source):
        p = converted.project(source, name=name)
        game_dir = os.path.join(p.assets, "game")
        if not os.path.isdir(game_dir):
            shutil.copytree(os.path.dirname(source), game_dir)
    else:
        p = converted.project(source, name=name)
        game_dir = os.path.join(p.assets, "game")
        if not os.path.isdir(game_dir):
            if not media.extract(source, game_dir):
                raise ValueError("%s: nothing in it to read" % source)
    outputs, report = [], {"assemblies": []}
    for asm in assemblies(game_dir):
        stem = os.path.splitext(os.path.basename(asm))[0]
        out = os.path.join(p.src, stem)
        if not os.path.isdir(out):
            decompile(asm, out)
        fixed = fixups.apply(out)
        report["assemblies"].append({"assembly": os.path.relpath(asm, game_dir), "project": stem,
                                     "ref casts": fixed["ref casts"], "constructors": fixed["constructors"],
                                     "left": fixed["left"]})
        outputs.append(out)
    with open(os.path.join(p.reports, "decompile.json"), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    kinds = {}
    if content:
        records = []
        for base, _dirs, files in os.walk(game_dir):
            for f in files:
                if f.lower().endswith(".xnb"):
                    rel = os.path.relpath(base, game_dir)
                    rec = xnb.convert(os.path.join(base, f), os.path.join(p.assets, "content", rel))
                    rec["folder"] = rel.replace("\\", "/")
                    k = "error" if "error" in rec else rec["kind"]
                    kinds[k] = kinds.get(k, 0) + 1
                    records.append(rec)
        with open(os.path.join(p.reports, "content.json"), "w", encoding="utf-8") as f:
            json.dump(records, f, indent=1)
        outputs.append(os.path.join(p.assets, "content"))
    asm_line = ", ".join(a["project"] for a in report["assemblies"])
    p.record("xna", outputs, notes="C# of %s (ILSpy; %d ref casts and %d constructor calls rewritten, %d left as "
             "they were: reports/decompile.json); content: %s" % (
                 asm_line, sum(a["ref casts"] for a in report["assemblies"]),
                 sum(a["constructors"] for a in report["assemblies"]),
                 sum(len(a["left"]) for a in report["assemblies"]),
                 ", ".join("%d %s" % (v, k) for k, v in sorted(kinds.items())) or "not converted"))
    return p, report, kinds


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    if argv[0] == "port":                     # the decompiled C# built and run on the PC (xna/port.py)
        from codebreaker.xna import port
        return port.main(argv[1:])
    name = argv[argv.index("--name") + 1] if "--name" in argv else None
    try:
        p, report, kinds = convert(argv[0], name=name, content="--no-content" not in argv)
    except (ValueError, RuntimeError) as e:
        print("xna: %s" % e)
        return 1
    for a in report["assemblies"]:
        print("  %-28s C#: %d ref casts, %d constructors rewritten, %d left" % (
            a["project"], a["ref casts"], a["constructors"], len(a["left"])))
    if kinds:
        print("  content: " + ", ".join("%d %s" % (v, k) for k, v in sorted(kinds.items())))
    print("-> %s" % p.root)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
