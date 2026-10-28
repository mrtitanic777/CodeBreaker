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
are what the game was built with. An assembly an obfuscator has been over (de4dot names one: SunBurn's
Spices.Net) is cleaned by de4dot first -- names restored, strings decrypted, control flow and field data
put back -- and the cleaned copy is decompiled (setting `de4dot`; built from its source, see
NOT_IN_REPO.md); de4dot's "Unknown Obfuscator" is its word for none found, and such an assembly is
decompiled as it is. The decompiler is ILSpy's command line (`ilspycmd`, setting
`ilspycmd`; .NET's `dotnet tool install -g ilspycmd`); it is run with DOTNET_ROLL_FORWARD=LatestMajor
so a newer .NET runs it.
"""
import json
import os
import shutil
import subprocess
import sys

from codebreaker.formats import pe

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


def assemblies(folder):
    """The game's own assemblies under `folder` ([path]), the .exe first."""
    out = []
    for base, _dirs, files in os.walk(folder):
        for f in files:
            low = f.lower()
            if not low.endswith((".exe", ".dll")) or low.startswith(FRAMEWORK) or ".resources." in low:
                continue
            p = os.path.join(base, f)
            if pe.is_dotnet(p):
                out.append(p)
    return sorted(out, key=lambda p: (not p.lower().endswith(".exe"), p.lower()))


def de4dot():
    """de4dot's program, or None: the `de4dot` setting, PATH, or a build beside the checkout (../_tools/de4dot)."""
    from codebreaker.base import settings
    named = settings.get("de4dot")
    if named and os.path.isfile(named):
        return named
    found = shutil.which("de4dot")
    if found:
        return found
    beside = os.path.join(os.path.dirname(settings.ROOT), "_tools", "de4dot", "Release", "net8.0", "de4dot.exe")
    return beside if os.path.isfile(beside) else None


def obfuscator(assembly):
    """The obfuscator de4dot finds in `assembly`, or None (none, or de4dot not installed)."""
    tool = de4dot()
    if tool is None:
        return None
    env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor")
    r = subprocess.run([tool, "-d", assembly], capture_output=True, text=True, env=env)
    for line in (r.stdout or "").splitlines():
        if line.startswith("Detected ") and "(" in line:
            name = line[len("Detected "):line.rfind("(")].strip()
            return None if name == "Unknown Obfuscator" else name
    return None


def deobfuscated(assembly):
    """`assembly` cleaned by de4dot (cached under state/xna/deobfuscated by its contents), with what it was
    obfuscated with -- or (assembly, None) when nothing was."""
    import hashlib
    from codebreaker.base import settings
    name = obfuscator(assembly)
    if name is None:
        return assembly, None
    with open(assembly, "rb") as f:
        digest = hashlib.sha1(f.read()).hexdigest()[:12]
    cache = settings.state("xna", "deobfuscated")
    os.makedirs(cache, exist_ok=True)
    stem, ext = os.path.splitext(os.path.basename(assembly))
    out = os.path.join(cache, "%s-%s%s" % (stem, digest, ext))
    if not os.path.isfile(out):
        env = dict(os.environ, DOTNET_ROLL_FORWARD="LatestMajor")
        r = subprocess.run([de4dot(), assembly, "-o", out], capture_output=True, text=True, env=env)
        if r.returncode != 0 or not os.path.isfile(out):
            raise RuntimeError("de4dot failed on %s: %s" % (os.path.basename(assembly), (r.stdout + r.stderr)[-500:]))
    return out, name


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
    elif pe.is_dotnet(source):
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
        source, obfuscated_with = deobfuscated(asm)
        if not os.path.isdir(out):
            decompile(source, out)
        fixed = fixups.apply(out)
        report["assemblies"].append({"assembly": os.path.relpath(asm, game_dir), "project": stem,
                                     "deobfuscated": obfuscated_with,
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
                rel = os.path.relpath(base, game_dir)
                if not f.lower().endswith(".xnb"):
                    rec = xnb.convert_effect_file(os.path.join(base, f), os.path.join(p.assets, "content", rel))
                    if rec is None:
                        continue
                else:
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
        print("  %-28s C#: %d ref casts, %d constructors rewritten, %d left%s" % (
            a["project"], a["ref casts"], a["constructors"], len(a["left"]),
            "  (deobfuscated: %s)" % a["deobfuscated"] if a["deobfuscated"] else ""))
    if kinds:
        print("  content: " + ", ".join("%d %s" % (v, k) for k, v in sorted(kinds.items())))
    print("-> %s" % p.root)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
