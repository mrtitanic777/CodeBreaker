"""plm86.py -- Intel's ISIS-II tools for the 8086 (PL/M-86 V1.1-2.1, ASM86 2.0/2.1; 8080 programs), as toolchains.

A profile names this module (toolchains/plm86.json, asm86.json). The tool runs under the ISIS-II
emulator (emu/isis.py); the object is Intel's OMF-86 (omf.read reads it). The profile says which:

    "tool": folder of the kit (plm86, asm86)   "program": its file (PLM86, ASM86)
    "source": the name the source is given (T.PLM, T.A86)
    "tail": the command line after the program, {controls} the variant's
    "ok": a regular expression the console and listing must match (group 1 "0": no errors), or none

and the variants the version and the controls: {"version": "2.1", "controls": "MEDIUM OPTIMIZE(2) CODE"}.
A source's $INCLUDEd files are the project's headers (the profile's "headers" extensions: .LIT).
"""
import hashlib
import os
import pickle
import re
import shutil
import tempfile

from codebreaker.base import settings
from codebreaker.exact import toolchain as tcm

CACHE_VERSION = "p2"


class PLM86(object):
    def __init__(self, profile, variant=None):
        self.p = profile
        self.name = profile["name"]
        self.variant = variant or profile.get("default_variant")
        v = profile.get("variants", {}).get(self.variant, {})
        self.version = v.get("version", "2.1")
        self.controls = v.get("controls", profile.get("controls", ""))
        self.key = hashlib.sha1((self.name + self.version + self.controls + CACHE_VERSION).encode()).hexdigest()[:12]

    def compile(self, src, headers=None, include=(), switches=None, cache=True):
        crlf = lambda b: b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        src = crlf(src)
        hs = sorted((f.upper(), crlf(b)) for f, b in (headers or {}).items())
        controls = switches or self.controls
        key = hashlib.sha1(b"\0".join([src, repr(hs).encode(), controls.encode(), self.key.encode()])).hexdigest()
        cf = settings.state("exact", "cache", key[:2], key)
        if cache:
            got = tcm.cache_get(cf)
            if got is not None:
                return got
        res = self._compile(src, hs, controls)
        tcm.cache_put(cf, res)
        return res

    def _compile(self, src, headers, controls):
        from codebreaker.exact.emu import isis
        p = self.p
        kit = os.path.join(tcm.kit_dir(p["kit"]), p.get("tool", "plm86"), self.version)
        name = p.get("source", "T.PLM")
        w = tempfile.mkdtemp(prefix="isis_")
        try:
            for f in os.listdir(kit):
                shutil.copy(os.path.join(kit, f), os.path.join(w, f.upper()))
            open(os.path.join(w, name), "wb").write(src)
            for f, b in headers:
                open(os.path.join(w, f), "wb").write(b)
            tail = p.get("tail", " %s {controls}" % name).format(controls=controls)
            r = isis.run(os.path.join(w, p.get("program", "PLM86")), tail, {0: w, 1: w}, limit=p.get("limit", 400_000_000))
            con = r.console.decode("latin1") if isinstance(r.console, bytes) else str(r.console)
            stem = os.path.splitext(name)[0]
            lp = os.path.join(w, stem + ".LST")
            lst = open(lp, "rb").read().decode("latin1") if os.path.exists(lp) else ""
            obj = os.path.join(w, stem + ".OBJ")
            ok = True
            if p.get("ok"):
                m = re.search(p["ok"], con + lst)
                ok = bool(m) and (not m.groups() or m.group(1) == "0")
            if not os.path.exists(obj) or not ok:
                errs = "\n".join(l for l in lst.replace("\r", "").split("\n") if "***" in l or "ERROR" in l)[:1500]
                return None, con + "\n" + errs
            return open(obj, "rb").read(), con
        finally:
            shutil.rmtree(w, ignore_errors=True)
