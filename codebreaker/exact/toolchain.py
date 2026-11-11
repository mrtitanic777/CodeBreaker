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
import re
import shutil
import struct

from codebreaker.base import cache as _cache
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


def kit_path(kit, path, base=None):
    """a file named in a profile or project: "KIT:path" in that kit, else in `kit`, else from `base`"""
    k, sep, rest = path.partition(":")
    if sep and len(k) > 1:
        return os.path.join(kit_dir(k), rest)
    if kit:
        return os.path.join(kit_dir(kit), path)
    return os.path.normpath(os.path.join(base or ".", path))




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
        v = profile.get("variants", {}).get(self.variant, [])
        if isinstance(v, dict):                             # ({"patches": [changes], "hooks": [hook names]})
            on, self.variant_hooks = set(v.get("patches", [])), list(v.get("hooks", []))
        else:
            on, self.variant_hooks = set(v), []
        if self.variant and self.variant not in profile.get("variants", {}):
            raise ValueError("%s has no variant %r (it has %s)" % (self.name, self.variant, sorted(profile.get("variants", {}))))
        self.patches = [x for x in profile.get("patches", []) if x["change"] in on]
        keyed = dict(profile, passes=[{k: v for k, v in ps.items() if k != "listing"} for ps in profile.get("passes", [])])
        self.key = hashlib.sha1(json.dumps([keyed, self.variant], sort_keys=True).encode()).hexdigest()[:12]   # (a
        #  listing's switch changes no object: not in the key)

    # ---- the stage: the tool's files, patched as the variant has them
    def stage(self):
        tag = os.environ.get("CB_STAGE_TAG")                # (a worker of a parallel run: a stage of its own, the
        d = settings.state("exact", "stage", "%s-%s%s" % (self.name, self.key, "-" + tag if tag else ""))   #  cache shared)
        done = os.path.join(d, ".staged")
        if os.path.exists(done):
            return d
        kit = kit_dir(self.p["kit"])
        st = self.p.get("stage", {})

        def where(src):                                     # "otherkit:path" names a file of another kit
            k, sep, rest = src.partition(":")
            return os.path.join(kit_dir(k), rest) if sep and len(k) > 1 else os.path.join(kit, src)
        files = dict(st.get("files", {}))
        if st.get("all"):                                   # (every file of the kit, its folders flattened: a
            for root, _, fs in os.walk(kit):                #  compiler that finds its headers beside itself)
                for f in fs:
                    files.setdefault(f.upper(), os.path.relpath(os.path.join(root, f), kit))
        for dst, src in files.items():
            b = bytearray(open(where(src), "rb").read())
            hdr = struct.unpack_from("<H", b, 8)[0] * 16 if b[:2] == b"MZ" else 0
            for x in self.patches:
                if x["file"] != dst or x.get("in_memory"):
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
            for root, _, fs in os.walk(where(inc)):         # (with its folders: SYS\)
                to = os.path.join(d, "INCLUDE", os.path.relpath(root, where(inc)).upper())
                os.makedirs(to, exist_ok=True)
                for f in fs:
                    shutil.copy(os.path.join(root, f), os.path.join(to, f.upper()))
        open(done, "w").write(self.key)
        return d

    # ---- one source
    def switches_of(self, src, switches=None):
        fs = self.p.get("file_switches")
        if fs:
            if self.p.get("file_switches_comment") == ";":  # (an assembler's: ; NAME: switches)
                m = re.match(rb"; " + fs.encode() + rb": ([^\r\n]*)", src)
            else:
                m = re.match(rb"/\* " + fs.encode() + rb": ([^*]*?) \*/", src)
            if m:
                return m.group(1).decode().strip()
        return switches if switches is not None else self.p.get("switches", "")

    def answers_of(self, src):
        out = {}
        for hname, h in self.p.get("hooks", {}).items():
            if "header" not in h:                           # (a hook the variant sets, no source's answer)
                continue
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

    def compile(self, src, headers=None, include=(), switches=None, cache=True, listing=False):
        """-> (object bytes or None, what the tool printed); listing: (object, printed, the listing's text) - the
        pass the profile's "listing" names writing its listing of the code (the source line each comes from)"""
        if listing:
            lp = next((ps for ps in self.p["passes"] if ps.get("listing")), None)
            if lp is None:
                return self.compile(src, headers, include, switches, cache) + ("",)
            saved = lp["args"]
            lp["args"] = saved + " " + lp["listing"]
            self._listing = None
            try:
                obj, con = self.compile(src + b"\n/* listing */", headers, include, switches, cache, False)
            finally:
                lp["args"] = saved
            return obj, con, getattr(self, "_listing", None) or ""
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
        if cache:
            got = _cache.value("exact", key)
            if got is not None:
                if len(got) > 2:                            # (a listing kept with it)
                    self._listing = got[2]
                return got[:2]
        w = self.stage()
        _SEQ[0] += 1                                        # (a name of its own each time: the emulator may hold
        tag = "T%05d" % ((os.getpid() * 7 + _SEQ[0]) % 100000)   #  the last one's files open a while)
        crlf = lambda b: b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        ix = os.path.join(w, "INCX")
        ext = self.p.get("source_ext", ".C")
        body = crlf(src.replace(b"\x1a", b""))
        if self.p.get("source_eof"):                        # (a ^Z after the text: MASM 1.00 wants it)
            body += b"\x1a"
        out = os.path.join(w, self.p["output"].format(**self._values(sw, tag)))
        lst = os.path.join(w, tag + ".LST")                 # (a listing the pass was asked for)
        if MEMFS and self.p.get("emulator", "dos") == "dos":
            from codebreaker.exact.emu import dos           # (the run's files in memory: nothing written, no other
            fs = dos.MemFS(hide=[ix])                       #  run's files in the way)
            for f, b in headers.items():
                fs.put(os.path.join(w, f.upper()), crlf(b))
            for f, b in incs:
                fs.put(os.path.join(ix, f.upper()), crlf(b))
            fs.put(os.path.join(w, tag + ext), body)
            self._fs = fs
            try:
                code, con = self._run(w, sw, tag, ans)
            finally:
                self._fs = None
            obj = fs.read(out) if code == 0 and fs.isfile(out) else None
            lst_text = fs.read(lst).decode("latin1").replace("\r", "") if fs.isfile(lst) else None
        else:
            for f, b in headers.items():
                open(os.path.join(w, f.upper()), "wb").write(crlf(b))
            os.makedirs(ix, exist_ok=True)
            for f in os.listdir(ix):
                _remove(os.path.join(ix, f))
            for f, b in incs:
                open(os.path.join(ix, f.upper()), "wb").write(crlf(b))
            open(os.path.join(w, tag + ext), "wb").write(body)
            if os.path.exists(out):
                os.remove(out)
            code, con = self._run(w, sw, tag, ans)
            obj = open(out, "rb").read() if code == 0 and os.path.exists(out) else None
            lst_text = open(lst, encoding="latin1").read().replace("\r", "") if os.path.exists(lst) else None
            for f in [tag + ext, os.path.basename(out), tag + ".LST"] + [h.upper() for h in headers]:
                _remove(os.path.join(w, f))
            for f in os.listdir(w):                         # (the passes' own files a failed run leaves: T12345EX ..)
                if f.startswith(tag) and len(f) == len(tag) + 2:
                    _remove(os.path.join(w, f))
        if obj is not None and self.p.get("rename_externs"):
            from codebreaker.formats import omf
            obj = omf.rename_externs(obj, self.p["rename_externs"])
        res = (obj, con.decode("latin1").replace("\r", ""))
        self._listing = lst_text
        _cache.keep("exact", key, res + ((lst_text,) if lst_text is not None else ()))
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
            inst = when = None
            for hname, h in self.p.get("hooks", {}).items():
                if h["pass"] == ps["run"] and ans.get(hname):
                    inst = _hook(h, set(ans[hname]))
            calls = [h for x in self.variant_hooks for h in [self.p.get("hooks", {}).get(x)] if h and h["pass"] == ps["run"] and h.get("call")]
            if calls:                                       # (a hook whose answer is code: where the pass goes on)
                inst = _call_hooks(calls, inst)
            mem = [x for x in self.patches if x["file"] == ps["run"] and x.get("in_memory")]
            if mem:                                         # a packed pass: changed once it has unpacked itself
                inst, when = _in_memory(mem, inst)
            penv = env + b"".join(e.format(**v).encode("latin1") + b"\0" for e in ps.get("env", []))
            r = dos.run(os.path.join(w, ps["run"]), w, ps.get("args", "").format(**v), env=penv, instrument=inst,
                        instrument_when=when, psp=_int(self.p.get("psp", 0x100)), top=_int(self.p.get("top", 0)) or None,
                        fast=inst is None and when is None,    # (no hook of ours in the pass: the fast run)
                        fs=getattr(self, "_fs", None))
            con += r.console
            if r.exit:
                return r.exit, con + b"(the pass that stopped: " + ps["run"].encode() + b")\n"
        return 0, con


_SEQ = [0]
MEMFS = os.environ.get("CB_MEMFS", "1") != "0"           # (a compile's files in memory: dos.MemFS)


def _remove(p):
    try:
        if os.path.exists(p):
            os.remove(p)
    except OSError:                                         # (still open: left for the next stage's clean-up)
        pass


def _call_hooks(hooks, then=None):
    """hooks {"at": [addresses], "call": "module:function"}: at each address the function is given (read
    word(seg, off), bp, ss, ds) and says where execution goes on (an IP), or None to go on as it was"""
    import importlib
    from unicorn import UC_HOOK_CODE
    from unicorn.x86_const import UC_X86_REG_BP, UC_X86_REG_SS, UC_X86_REG_DS, UC_X86_REG_IP

    def inst(mu, load):
        for h in hooks:
            mod, _, fn = h["call"].partition(":")
            f = getattr(importlib.import_module(mod), fn)
            for a in h["at"]:
                def hit(uc, addr, size, u, f=f):
                    rd = lambda seg, o: struct.unpack("<H", bytes(uc.mem_read(seg * 16 + (o & 0xFFFF), 2)))[0]
                    to = f(rd, uc.reg_read(UC_X86_REG_BP), uc.reg_read(UC_X86_REG_SS), uc.reg_read(UC_X86_REG_DS))
                    if to is not None:
                        uc.reg_write(UC_X86_REG_IP, to)
                mu.hook_add(UC_HOOK_CODE, hit, begin=load * 16 + _int(a), end=load * 16 + _int(a))
        if then:
            then(mu, load)
    return inst


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


def _in_memory(mem, then=None):
    pairs = [(_int(x["at"]), bytes.fromhex(x["was"]), bytes.fromhex(x["now"])) for x in mem]

    def when(uc, addr, load):
        return all(bytes(uc.mem_read(load * 16 + a, len(was))) == was for a, was, now in pairs)

    def inst(mu, load):
        for a, was, now in pairs:
            mu.mem_write(load * 16 + a, now)
        if then:
            then(mu, load)
    return inst, when


_LOADED = {}


def load(name, variant=None):
    k = (name, variant)
    if k not in _LOADED:
        with open(os.path.join(HERE, "toolchains", name + ".json")) as f:
            p = json.load(f)
        if p.get("plugin"):                                 # (a toolchain whose running is code: module:Class)
            import importlib
            mod, _, cls = p["plugin"].partition(":")
            _LOADED[k] = getattr(importlib.import_module(mod), cls)(p, variant)
        else:
            _LOADED[k] = Toolchain(p, variant)
    return _LOADED[k]
