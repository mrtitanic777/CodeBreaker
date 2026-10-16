"""plm86.py -- Intel's PL/M-86 compiler for ISIS-II (V1.1, V1.2, V2.1; 8080 programs), as a toolchain.

The profile (toolchains/plm86.json) names this module. The compiler runs under the ISIS-II emulator
(emu/isis.py): `PLM86 T.PLM <controls>`; the object is Intel's OMF-86 (omf.read reads it), the
listing says how it went ("n PROGRAM ERROR(S)"). The profile's variants are versions and controls:
{"version": "2.1", "controls": "MEDIUM OPTIMIZE(2) CODE"}. A source's $INCLUDEd files are the
project's headers (the profile's "headers" extension: .LIT).
"""
import hashlib
import os
import pickle
import re
import shutil
import tempfile

from codebreaker.base import settings
from codebreaker.exact import toolchain as tcm

CACHE_VERSION = "p1"


class PLM86(object):
    def __init__(self, profile, variant=None):
        self.p = profile
        self.name = profile["name"]
        self.variant = variant or profile.get("default_variant")
        v = profile.get("variants", {}).get(self.variant, {})
        self.version = v.get("version", "2.1")
        self.controls = v.get("controls", "MEDIUM OPTIMIZE(2) CODE")
        self.key = hashlib.sha1((self.version + self.controls + CACHE_VERSION).encode()).hexdigest()[:12]

    def compile(self, src, headers=None, include=(), switches=None, cache=True):
        crlf = lambda b: b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        src = crlf(src)
        hs = sorted((f.upper(), crlf(b)) for f, b in (headers or {}).items())
        controls = switches or self.controls
        key = hashlib.sha1(b"\0".join([src, repr(hs).encode(), controls.encode(), self.key.encode()])).hexdigest()
        cf = settings.state("exact", "cache", key[:2], key)
        if cache and os.path.exists(cf):
            with open(cf, "rb") as f:
                return pickle.load(f)
        res = self._compile(src, hs, controls)
        os.makedirs(os.path.dirname(cf), exist_ok=True)
        with open(cf, "wb") as f:
            pickle.dump(res, f)
        return res

    def _compile(self, src, headers, controls):
        from codebreaker.exact.emu import isis
        kit = os.path.join(tcm.kit_dir(self.p["kit"]), "plm86", self.version)
        w = tempfile.mkdtemp(prefix="plm86_")
        try:
            for f in os.listdir(kit):
                shutil.copy(os.path.join(kit, f), os.path.join(w, f.upper()))
            open(os.path.join(w, "T.PLM"), "wb").write(src)
            for f, b in headers:
                open(os.path.join(w, f), "wb").write(b)
            r = isis.run(os.path.join(w, "PLM86"), " T.PLM " + controls, {0: w, 1: w}, limit=200_000_000)
            con = r.console.decode("latin1") if isinstance(r.console, bytes) else str(r.console)
            lst = open(os.path.join(w, "T.LST"), "rb").read().decode("latin1") if os.path.exists(os.path.join(w, "T.LST")) else ""
            m = re.search(r"(\d+) PROGRAM ERROR", con + lst)
            obj = os.path.join(w, "T.OBJ")
            if not os.path.exists(obj) or not m or m.group(1) != "0":
                errs = "\n".join(l for l in lst.replace("\r", "").split("\n") if "***" in l)[:1500]
                return None, con + "\n" + errs
            return open(obj, "rb").read(), con
        finally:
            shutil.rmtree(w, ignore_errors=True)
