"""converted.py -- where CodeBreaker puts what it makes of a binary: one project folder per binary.

    from codebreaker.base import converted
    p = converted.project(path)                  # Converted/<name>/, made (and its original copied) once
    out = os.path.join(p.src, "tetris.c")        # ... write the outputs into its folders ...
    p.record("decompile", [out], notes="...")    # project.json and README.txt say what was done

Every binary decompiled, recompiled or otherwise taken apart gets a folder under `Converted/` at
the top of the checkout (setting `converted`):

    Converted/<name>/
      README.txt     what the binary is, what was done to it (each command, when), how to build or
                     run what came out
      project.json   the same for tools: the input, its SHA-1, its kind, every output and the
                     command that made it
      original/      the binary as it came in (a copy)
      src/           source: decompiled C, recompiled C++, assembly, build files
      assets/        resources and data: extracted resources, a title's files
      build/         programs and libraries built from src/
      reports/       analysis, listings, disassembly, logs

A project is the binary's, not the command's: it is found again by the SHA-1 of the original, so
every command run on one file adds to one folder. Another file of the same name gets <name>_2.
`Converted/` holds other people's code and data: it is never committed (.gitignore).
"""
import datetime
import hashlib
import json
import os
import re
import shutil

from codebreaker.base import settings

FOLDERS = ("original", "src", "assets", "build", "reports")
# file names that say nothing about the program (a console's default.xex, a setup.exe): the
# project is named after what the binary calls itself, or its folder, instead
_GENERIC = {"default", "main", "setup", "install", "game", "program", "app", "boot"}


def root():
    """The Converted folder (created on demand)."""
    path = settings.get("converted") or os.path.join(settings.ROOT, "Converted")
    os.makedirs(path, exist_ok=True)
    return path


def _sha1(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def _clean(name):
    name = re.sub(r"[^A-Za-z0-9._-]+", "_", name).strip("._")
    return name or "binary"


def default_name(path):
    """A project name for a binary: its file name, unless that is a generic one (default.xex),
    then the name the image gives itself (an XEX's header), else its folder's."""
    stem = os.path.splitext(os.path.basename(path))[0]
    if stem.lower() not in _GENERIC:
        return _clean(stem)
    try:
        from codebreaker.formats import xex
        if xex.is_xex(path):
            own = os.path.splitext(xex.header(path).name or "")[0]
            if own and own.lower() not in _GENERIC:
                return _clean(own)
    except Exception:
        pass
    parent = os.path.basename(os.path.dirname(os.path.abspath(path)))
    return _clean(parent) if parent else _clean(stem)


def _kind(path):
    try:
        from codebreaker.formats import detect
        found = detect.identify(path)
        if found:
            return found[0].get("format", "")
    except Exception:
        pass
    return ""


class Project:
    def __init__(self, folder):
        self.root = folder
        for name in FOLDERS:
            setattr(self, name, os.path.join(folder, name))
            os.makedirs(getattr(self, name), exist_ok=True)
        self.manifest = os.path.join(folder, "project.json")

    def data(self):
        try:
            with open(self.manifest, encoding="utf-8") as f:
                return json.load(f)
        except (OSError, ValueError):
            return {}

    @property
    def name(self):
        return os.path.basename(self.root)

    @property
    def original_file(self):
        """The copy of the binary in original/."""
        d = self.data()
        return os.path.join(self.original, d.get("file", "")) if d.get("file") else None

    def record(self, command, outputs=(), notes=""):
        """Note a step: the command, the files or folders it made, and anything a reader needs (how
        to build or run them). Rewrites project.json and README.txt."""
        d = self.data()
        rel = []
        here = os.path.abspath(self.root)
        for out in outputs:
            out = os.path.abspath(out)
            # inside the project folder (by whole path components -- Converted/foo_2/x is not inside Converted/foo)
            inside = out == here or out.startswith(here + os.sep)
            rel.append(os.path.relpath(out, self.root) if inside else out)
        step = {"command": command, "when": datetime.datetime.now().isoformat(timespec="seconds"),
                "outputs": rel}
        if notes:
            step["notes"] = notes
        # one entry per command: running it again replaces what it said before
        d["steps"] = [s for s in d.get("steps", []) if s.get("command") != command] + [step]
        self._write(d)

    def _write(self, d):
        with open(self.manifest, "w", encoding="utf-8", newline="\n") as f:
            json.dump(d, f, indent=1)
        lines = [d.get("name", self.name), "=" * len(d.get("name", self.name)), "",
                 "What CodeBreaker made of %s." % d.get("file", "a binary"), "",
                 "  Input:   %s" % d.get("source", ""),
                 "  Kind:    %s" % (d.get("kind") or "unknown"),
                 "  Size:    %s bytes" % d.get("size", "?"),
                 "  SHA-1:   %s" % d.get("sha1", ""), "",
                 "Folders:",
                 "  original/  the binary as it came in",
                 "  src/       source CodeBreaker produced",
                 "  assets/    resources and data",
                 "  build/     what was built from src/",
                 "  reports/   analysis, listings, logs", ""]
        for s in d.get("steps", []):
            lines.append("%s  (%s)" % (s["command"], s["when"].replace("T", " ")))
            for o in s.get("outputs", []):
                lines.append("    %s" % o)
            for n in s.get("notes", "").splitlines():
                lines.append("    %s" % n)
            lines.append("")
        lines.append("The binary and everything made from it are the original's owner's: this folder is "
                     "never committed.")
        with open(os.path.join(self.root, "README.txt"), "w", encoding="utf-8", newline="\r\n") as f:
            f.write("\n".join(lines) + "\n")


def project(path, name=None, base=None):
    """The project folder of a binary: found again by its SHA-1, else made (its original copied in).
    `base`: another folder to keep it in (default: the Converted folder)."""
    path = os.path.abspath(path)
    digest = _sha1(path)
    base = base or root()
    os.makedirs(base, exist_ok=True)
    for entry in sorted(os.listdir(base)):
        manifest = os.path.join(base, entry, "project.json")
        if os.path.isfile(manifest):
            try:
                with open(manifest, encoding="utf-8") as f:
                    if json.load(f).get("sha1") == digest and (name is None or entry == _clean(name)):
                        return Project(os.path.join(base, entry))
            except (OSError, ValueError):
                continue
    wanted = _clean(name) if name else default_name(path)
    folder, n = os.path.join(base, wanted), 2
    while os.path.exists(folder):
        folder = os.path.join(base, "%s_%d" % (wanted, n))
        n += 1
    p = Project(folder)
    copy = os.path.join(p.original, os.path.basename(path))
    shutil.copy2(path, copy)
    p._write({"name": os.path.basename(folder), "file": os.path.basename(path), "source": path,
              "sha1": digest, "size": os.path.getsize(path), "kind": _kind(path), "steps": []})
    return p
