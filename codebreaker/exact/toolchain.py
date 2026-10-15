"""toolchain.py -- a compiler (or assembler) of the period, run in this process from its profile.

A profile (toolchains/NAME.json) says what the tool is and how its driver ran it; nothing about one
compiler is written here. Its parts:

    kit         the settings.json `kits` entry holding the tool's files (they are not in the repository)
    emulator    dos | xenix | isis
    stage       {"files": {name in the stage: path in the kit}, "include": kit folder of headers}
    psp         the PSP's segment (a pass that must be loaded low or high)
    env         environment strings; "env_unless": {string: regex} -- left out when the switches match
    switches    the default switches; "file_switches": NAME -- a source's first line /* NAME: ... */
                gives its own
    derive      {name: {"regex", "then", "else"}} -- values made from the switches, for the command
                lines ({1} in "then" is the regex's first group)
    passes      [{"run": program, "args": template}] -- {tag} the file's stem, {tag6} its first six
                letters, {name} any derived value; one pass's failure stops the compile
    output      the object file's name, a template
    patches     [{"change", "file", "at", "was", "now", "note"}] -- bytes changed in a copy of a pass
                (at: an offset in its load image; was: what must be there, else the kit is not this
                one); "variants": {name: [changes]} -- which of them a variant of the compiler made
    hooks       {name: {"pass", "at": [addresses], "set": {register: value}, "header": WORD}} -- each
                time the pass reaches an address the count goes up; at the counts a source lists in a
                /* WORD: 1,2 */ line of its head, the registers are set (a pass's question answered)
    rename_externs  {name: name} -- external names changed in the object (a compiler that calls its
                helpers by other names than the library the program was linked with)

    tc = toolchain.load("msc300", variant="own")
    obj, console = tc.compile(source_bytes, headers={"tree.h": b"..."}, include=[folder],
                              switches=None)     # obj: the object file's bytes, or None
"""
import hashlib
import json
import os
import pickle
import re
import shutil
import struct

from codebreaker.base import settings

HERE = os.path.dirname(os.path.abspath(__file__))
CACHE_VERSION = "1"


def kit_dir(name):
    kits = settings._load().get("kits", {})
    path = os.environ.get("CB_KIT_" + name.upper()) or kits.get(name, "")
    if not path:
        raise RuntimeError("the kit %r is not set up (settings.json: \"kits\": {\"%s\": folder})" % (name, name))
    if not os.path.isabs(path):
        path = os.path.normpath(os.path.join(settings.ROOT, path))
    return path


def profiles():
    d = os.path.join(HERE, "toolchains")
    return sorted(f[:-5] for f in os.listdir(d) if f.endswith(".json"))


def _int(x):
    return int(x, 0) if isinstance(x, str) else x


class Toolchain(object):
    def __init__(self, profile, variant=None):
        self.p = profile
        self.name = profile["name"]
        self.variant = variant or profile.get("default_variant") or ""
        on = set(profile.get("variants", {}).get(self.variant, []))
        if self.variant and self.variant not in profile.get("variants", {}):
            raise ValueError("%s has no variant %r (it has %s)" % (self.name, self.variant, sorted(profile.get("variants", {}))))
        self.patches = [x for x in profile.get("patches", []) if x["change"] in on]
        self.key = hashlib.sha1(json.dumps([profile, self.variant], sort_keys=True).encode()).hexdigest()[:12]

    # ---- the stage: the tool's files, patched as the variant has them
    def stage(self):
        d = settings.state("exact", "stage", "%s-%s" % (self.name, self.key))
        done = os.path.join(d, ".staged")
        if os.path.exists(done):
            return d
        kit = kit_dir(self.p["kit"])
        st = self.p.get("stage", {})
        for dst, src in st.get("files", {}).items():
            b = bytearray(open(os.path.join(kit, src), "rb").read())
            hdr = struct.unpack_from("<H", b, 8)[0] * 16 if b[:2] == b"MZ" else 0
            for x in self.patches:
                if x["file"] != dst:
                    continue
                at = hdr + _int(x["at"])
                was, now = bytes.fromhex(x["was"]), bytes.fromhex(x["now"])
                if bytes(b[at:at + len(was)]) != was:
                    raise RuntimeError("%s: %s at %x is not what change %s expects - not this compiler's kit" % (self.name, dst, _int(x["at"]), x["change"]))
                b[at:at + len(now)] = now
            os.makedirs(os.path.dirname(os.path.join(d, dst)) or d, exist_ok=True)
            open(os.path.join(d, dst), "wb").write(b)
        inc = st.get("include")
        if inc:
            os.makedirs(os.path.join(d, "INCLUDE"), exist_ok=True)
            for f in os.listdir(os.path.join(kit, inc)):
                p = os.path.join(kit, inc, f)
                if os.path.isfile(p):
                    shutil.copy(p, os.path.join(d, "INCLUDE", f.upper()))
        open(done, "w").write(self.key)
        return d

    # ---- one source
    def switches_of(self, src, switches=None):
        fs = self.p.get("file_switches")
        if fs:
            m = re.match(rb"/\* " + fs.encode() + rb": ([^*]*?) \*/", src)
            if m:
                return m.group(1).decode()
        return switches if switches is not None else self.p.get("switches", "")

    def answers_of(self, src):
        out = {}
        for hname, h in self.p.get("hooks", {}).items():
            m = re.search(rb"/\* " + h["header"].encode() + rb": ([0-9, ]+) \*/", src[:400])
            if m:
                out[hname] = sorted(int(x) for x in m.group(1).decode().replace(",", " ").split())
        return out

    def _values(self, sw, tag):
        v = {"tag": tag, "tag6": tag[:6], "switches": sw}
        for k, rule in self.p.get("derive", {}).items():
            m = re.search(rule["regex"], sw)
            if m:
                t = rule.get("then", "")
                for i, g in enumerate(m.groups(), 1):
                    t = t.replace("{%d}" % i, g or "")
                v[k] = t
            else:
                v[k] = rule.get("else", "")
        return v

    def _env(self, sw):
        out = b""
        for e in self.p.get("env", []):
            out += e.encode("latin1") + b"\0"
        for e, rx in self.p.get("env_unless", {}).items():
            if not re.search(rx, sw):
                out += e.encode("latin1") + b"\0"
        return out

    def compile(self, src, headers=None, include=(), switches=None, cache=True):
        """-> (object bytes or None, what the tool printed)"""
        src = src.replace(b"\r\n", b"\n")
        headers = dict(headers or {})
        incs = []
        for d in include:
            for f in sorted(os.listdir(d)):
                if os.path.isfile(os.path.join(d, f)):
                    incs.append((f, open(os.path.join(d, f), "rb").read()))
        sw = self.switches_of(src, switches)
        ans = self.answers_of(src)
        key = hashlib.sha1(b"\0".join([src, repr(sorted(headers.items())).encode(), repr(incs).encode(), sw.encode(),
                                       repr(sorted(ans.items())).encode(), self.key.encode(), CACHE_VERSION.encode()])).hexdigest()
        cf = settings.state("exact", "cache", key[:2], key)
        if cache and os.path.exists(cf):
            with open(cf, "rb") as f:
                return pickle.load(f)
        w = self.stage()
        tag = "T%05d" % (os.getpid() % 100000)
        crlf = lambda b: b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        for f, b in headers.items():
            open(os.path.join(w, f.upper()), "wb").write(crlf(b))
        ix = os.path.join(w, "INCX")
        os.makedirs(ix, exist_ok=True)
        for f in os.listdir(ix):
            os.remove(os.path.join(ix, f))
        for f, b in incs:
            open(os.path.join(ix, f.upper()), "wb").write(crlf(b))
        open(os.path.join(w, tag + ".C"), "wb").write(crlf(src))
        out = os.path.join(w, self.p["output"].format(**self._values(sw, tag)))
        if os.path.exists(out):
            os.remove(out)
        code, con = self._run(w, sw, tag, ans)
        obj = open(out, "rb").read() if code == 0 and os.path.exists(out) else None
        if obj is not None and self.p.get("rename_externs"):
            from codebreaker.exact import omf
            obj = omf.rename_externs(obj, self.p["rename_externs"])
        for f in (tag + ".C", os.path.basename(out)):
            if os.path.exists(os.path.join(w, f)):
                os.remove(os.path.join(w, f))
        for f in headers:
            if os.path.exists(os.path.join(w, f.upper())):
                os.remove(os.path.join(w, f.upper()))
        res = (obj, con.decode("latin1").replace("\r", ""))
        os.makedirs(os.path.dirname(cf), exist_ok=True)
        with open(cf, "wb") as f:
            pickle.dump(res, f)
        return res

    def _run(self, w, sw, tag, ans):
        emu = self.p.get("emulator", "dos")
        if emu != "dos":
            raise NotImplementedError("emulator %r" % emu)
        from codebreaker.exact.emu import dos
        v = self._values(sw, tag)
        env = self._env(sw)
        con = b""
        for ps in self.p["passes"]:
            inst = None
            for hname, h in self.p.get("hooks", {}).items():
                if h["pass"] == ps["run"] and ans.get(hname):
                    inst = _hook(h, set(ans[hname]))
            r = dos.run(os.path.join(w, ps["run"]), w, ps["args"].format(**v), env=env, instrument=inst,
                        psp=_int(self.p.get("psp", 0x100)))
            con += r.console
            if r.exit:
                return r.exit, con + b"(the pass that stopped: " + ps["run"].encode() + b")\n"
        return 0, con


def _hook(h, counts):
    from unicorn import UC_HOOK_CODE
    from unicorn import x86_const
    n = [0]

    def inst(mu, load):
        def hit(uc, a, size, u):
            n[0] += 1
            if n[0] in counts:
                for reg, val in h["set"].items():
                    mu.reg_write(getattr(x86_const, "UC_X86_REG_" + reg.upper()), _int(val))
        for a in h["at"]:
            a = load * 16 + _int(a)
            mu.hook_add(UC_HOOK_CODE, hit, begin=a, end=a)
    return inst


_LOADED = {}


def load(name, variant=None):
    k = (name, variant)
    if k not in _LOADED:
        with open(os.path.join(HERE, "toolchains", name + ".json")) as f:
            _LOADED[k] = Toolchain(json.load(f), variant)
    return _LOADED[k]
