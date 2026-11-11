"""mspascal.py -- the Microsoft Pascal compiler (PAS1/PAS2; 1.10 of November 1981 to 3.11), as a toolchain.

A profile names this module (toolchains/mspascal.json). The passes run under the DOS emulator:
`PAS1 T,T,T,T;` (source, object, listing, code listing), then `PAS2`. The object is OMF. Most of a
compiland's procedures are not public; the plug-in names them, so that the engine sees them as it
sees a C file's functions: from the code listing (T.COD: "Procedure/Function : NAME" and the first
address of its code) - or, for a compiler that writes none (3.04, 3.11), from where the code calls the
compiler's entry helper (or sets up a frame in line), in the order the source emits its procedures
(each at the end of its body: after those declared inside it). The names are published in the
object (a PUBDEF of the code segment): omf.read and the ledger see them like a C file's functions.

Every compiland ends with the compiler's entry helper (a near call to it opens each procedure); it
is published as "(helper)".

Variants: the kit's version ("version": a folder of the kit, or "1.10": the pre-release kit's zips).
"""
import hashlib
import os
import re
import shutil
import struct
import tempfile
import zipfile

from codebreaker.base import cache as _cache
from codebreaker.base import settings
from codebreaker.formats import omf
from codebreaker.exact import toolchain as tcm

CACHE_VERSION = "pas1"
HELPERS = [bytes.fromhex("5b32e42e8a07433c807208247f8ae02e8a07432e8b0f4343558bec2be9"),     # 1.10, 2.0x
           bytes.fromhex("5b2e8b0f4343558bec2be9"),                                   # 3.0x: only the frame size
           bytes.fromhex("5e2e8b0c4646558bec2be9")]                                   # 3.11: through SI
INLINE = bytes.fromhex("558bec81ec")                                                  # (3.11: the frame set up in line)


def emit_order(text):
    """the procedures of a source in the order their code is written: each at the end of its body, after the
    procedures declared inside it -> [name]"""
    text = re.sub(r"\{[^}]*\}|\(\*.*?\*\)", " ", text, flags=re.S)
    text = re.sub(r"'[^'\n]*'", "''", text)
    out, st = [], []
    toks = re.findall(r"[A-Za-z_][A-Za-z_0-9]*|[;()]", text)
    k = 0
    while k < len(toks):
        w = toks[k].upper()
        if w in ("PROCEDURE", "FUNCTION") and (not st or not st[-1][1]) and not (st and st[-1][2]):
            m = k + 2
            if m < len(toks) and toks[m] == "(":
                d = 1
                m += 1
                while d:
                    d += (toks[m] == "(") - (toks[m] == ")")
                    m += 1
            while toks[m] != ";":
                m += 1
            m += 1
            if m < len(toks) and toks[m].upper() in ("FORWARD", "EXTERN", "EXTERNAL"):
                k = m + 1
                continue
            st.append([toks[k + 1].upper(), False, 0])
            k = m
            continue
        if st:
            top = st[-1]
            if not top[1]:
                if w == "RECORD":
                    top[2] += 1
                elif w == "END" and top[2]:
                    top[2] -= 1
                elif w == "BEGIN" and not top[2]:
                    top[1] = True
                    top[2] = 1
            else:
                if w in ("BEGIN", "CASE"):
                    top[2] += 1
                elif w == "END":
                    top[2] -= 1
                    if top[2] == 0:
                        out.append(top[0])
                        st.pop()
        k += 1
    return out


def _segnames(obj):
    names, segs = [""], []
    for t, r in omf.records(obj):
        if t == 0x96:
            q = 0
            while q < len(r):
                names.append(r[q + 1:q + 1 + r[q]].decode("latin1"))
                q += 1 + r[q]
        elif t == 0x98:
            q = 1 if r[0] >> 5 else 4
            ni, _ = omf._idx(r, q + 2)
            segs.append(names[ni] if ni < len(names) else "?")
    return segs


def publish(obj, procs, code_seg):
    """the object with a PUBDEF of the code segment naming procs {name: offset} (before its MODEND)"""
    segs = _segnames(obj)
    si = segs.index(code_seg) + 1
    body = bytes([0]) + (bytes([si]) if si < 0x80 else bytes([0x80 | (si >> 8), si & 0xFF]))     # (no group; the segment)
    for n, off in sorted(procs.items(), key=lambda kv: kv[1]):
        body += omf.name(n) + struct.pack("<H", off) + b"\x00"
    rec = omf.record(0x90, body)
    out = b""
    for t, r in omf.records(obj):
        if t in (0x8A, 0x8B):
            out += rec
        out += omf.record(t, r)
    return out


class MSPascal(object):
    def __init__(self, profile, variant=None):
        self.p = profile
        self.name = profile["name"]
        self.variant = variant or profile.get("default_variant")
        v = profile.get("variants", {}).get(self.variant, {})
        self.version = v.get("version", "1.10")
        self.helper_data = v.get("helper_data", profile.get("helper_data", 3))
        self.key = hashlib.sha1((self.name + self.version + CACHE_VERSION).encode()).hexdigest()[:12]

    def kit(self):
        k = tcm.kit_dir(self.p["kit"] if self.version != "1.10" else self.p.get("kit110", self.p["kit"]))
        if self.version == "1.10":                          # (the pre-release kit: its files in zips)
            d = settings.state("exact", "stage", "mspascal110")
            if not os.path.exists(os.path.join(d, "PAS2.EXE")):
                os.makedirs(d, exist_ok=True)
                for z in sorted(os.listdir(k)):
                    if z.lower().endswith(".zip"):
                        zipfile.ZipFile(os.path.join(k, z)).extractall(d)
            return d
        return os.path.join(k, self.version)

    def compile(self, src, headers=None, include=(), switches=None, cache=True, name="T.PAS"):
        src = src.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        key = hashlib.sha1(b"\0".join([src, name.encode(), self.key.encode()])).hexdigest()
        if cache:
            got = _cache.value("exact", key)
            if got is not None:
                return got
        res = self._compile(src, name)
        _cache.keep("exact", key, res)
        return res

    def _compile(self, src, name):
        from codebreaker.exact.emu import dos
        k = self.kit()
        w = tempfile.mkdtemp(prefix="pas_")
        try:
            for f in ("PAS1.EXE", "PAS2.EXE", "PASKEY"):
                g = f if os.path.exists(os.path.join(k, f)) else "PAS1EM.EXE"   # (3.11: PAS1EM and PAS187)
                if os.path.exists(os.path.join(k, g)):
                    shutil.copy(os.path.join(k, g), os.path.join(w, f))
            open(os.path.join(w, name), "wb").write(src)
            stem = os.path.splitext(name)[0]
            r1 = dos.run(os.path.join(w, "PAS1.EXE"), w, "%s,T,T,T;" % (stem if name != "T.PAS" else "T"))
            con = r1.console.decode("latin1")
            if "No Errors" not in con.split("Pass One")[-1]:
                lst = os.path.join(w, "T.LST")
                errs = open(lst, "rb").read().decode("latin1") if os.path.exists(lst) else ""
                return None, con + "\n" + "\n".join(l for l in errs.replace("\r", "").split("\n") if "^" in l or "Error" in l)[:1500]
            r2 = dos.run(os.path.join(w, "PAS2.EXE"), w, "")
            con += r2.console.decode("latin1")
            op = os.path.join(w, "T.OBJ")
            if not os.path.exists(op):
                return None, con
            obj = open(op, "rb").read()
            cp = os.path.join(w, "T.COD")
            cod = open(cp, "rb").read().decode("latin1").replace("\r", "").replace("\x0c", "") if os.path.exists(cp) else ""
            return self._named(obj, cod, src.decode("latin1")), con
        finally:
            shutil.rmtree(w, ignore_errors=True)

    def _named(self, obj, cod, text):
        o = omf.read(obj)
        cs = [s for s, cls, ln, al in o.segs if s not in ("HEAP", "STACK", "DATA", "CONST", "MEMORY") and o.filled.get(s)]
        if not cs:
            return obj
        seg = cs[0]
        code = o.seg[seg][:o.filled[seg]]
        procs = {}
        name = None
        for line in cod.split("\n"):                        # (the code listing: each procedure's first address)
            m = re.match(r"\s*Procedure/Function : (\S+)", line)
            if m:
                name = m.group(1).upper()
                continue
            m = re.match(r"\s*\*\*\s*([0-9A-F]{6})\s", line)
            if m and name and name not in procs:
                procs[name] = int(m.group(1), 16)
        hs = [code.find(h) for h in HELPERS]
        helper = max(hs) if max(hs) >= 0 else None
        if not procs:                                       # (no listing: where the code calls the entry helper)
            end = helper if helper is not None else len(code)
            st = set()
            if helper is not None:
                for m in re.finditer(rb"\xe8", code[:end]):
                    a = m.start()
                    if a + 3 <= end and a + 3 + struct.unpack_from("<h", code, a + 1)[0] == helper:
                        st.add(a)
            for m in re.finditer(re.escape(INLINE), code[:end]):
                st.add(m.start())
            ss = sorted(st)
            order = emit_order(text)
            prog = re.search(r"(?im)^\s*(?:PROGRAM|MODULE)\s+([A-Z_0-9]+)", text)
            names = order + ([prog.group(1).upper()] if prog else [])
            if len(ss) == len(names) - 1:
                names = names[:-1]                          # (a module's empty block is no code)
            procs = {n: a for n, a in zip(names, ss)}
        have = {n for n, s, off in o.pubs}
        procs = {n: a for n, a in procs.items() if n not in have}
        if helper is not None:
            procs["(helper)"] = helper
        return publish(obj, procs, seg) if procs else obj
