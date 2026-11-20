#!/usr/bin/env python3
"""
xon_schema.py - reference implementation of the Xbox 360 (17559) XLive "xon"
web-service schema: descriptor parser, request decoder, response encoder,
native-layout calculator.

Source of truth (behaviour reconstructed, not copied):
  dash  CMarshaller::RecursiveMarshal      0x9224F198  (request: native -> wire)
        CSchemaAccess::SkipScope           0x9224EBD8  (full descriptor grammar)
        CSchemaInMemory::Bind              0x9224EA98  (blob layout)
        CConformanceList::*                0x9224FEC8.. (count slots)
  xam   FUN_81A3D7E8 (recursive, resumable unmarshaller; response: wire -> native)
        FUN_81A3D530 (unmarshaller init), FUN_81A3E380 (pump), FUN_81A3D650 (bump alloc)

The schema blob itself is Microsoft's data: the server never stores it. The client sends the copy it has in
memory (XamGetOnlineSchema) once per connection (live_server.op_xon_schema). For study on the user's PC:

  python -I xon_schema.py <xam.image.bin> list | describe <ordinal-hex|#index> | selftest
"""
import struct
import sys

XAM_BASE = 0x815F0000
SCHEMA_VA = 0x81A888C8
SCHEMA_SIZE = 0x7B15

PRIM_NAMES = {0: "u8", 1: "u16", 2: "u32", 3: "u64"}


class SchemaError(Exception):
    pass


# --------------------------------------------------------------------------
# descriptor tree
# --------------------------------------------------------------------------
class Arr:
    """SCHEMA_CONFORMING_INFO: 2 bytes {c0, c1} [+ LE16 const idx if c0&0x80]."""

    def __repr__(self):
        if self.inline:
            return "[const %d]" % self.const
        return "[slot %d]" % self.slot


class Est:
    """SCHEMA_CONFORMANCE_INFO: {k0=slot|log2size<<6, k1=flags} + LE16 max const idx."""


class Prim:
    kind = "prim"


class Scope:
    kind = "scope"


class _Rd:
    def __init__(self, data):
        self.d = data
        self.p = 0

    def eof(self):
        return self.p >= len(self.d)

    def u8(self):
        if self.p >= len(self.d):
            raise SchemaError("descriptor truncated")
        v = self.d[self.p]
        self.p += 1
        return v

    def le16(self):
        if self.p + 2 > len(self.d):
            raise SchemaError("descriptor truncated")
        v = self.d[self.p] | (self.d[self.p + 1] << 8)
        self.p += 2
        return v


class Schema:
    def __init__(self, blob):
        self.b = blob
        h = blob[:0x2C]
        self.version = struct.unpack(">HH", h[0:4])
        self.flags = struct.unpack(">I", h[4:8])[0]
        self.desc_end = struct.unpack(">I", h[0x14:0x18])[0]
        self.n_const, self.const_stride = struct.unpack(">HH", h[0x18:0x1C])
        self.n_path = struct.unpack(">H", h[0x20:0x22])[0]
        extra = struct.unpack(">H", h[0x26:0x28])[0]
        self.n_req, self.stride = struct.unpack(">HH", h[0x28:0x2C])
        off = 0x2C + extra
        self.ord_off = off
        off += 4 * self.n_req
        self.ent_off = off
        off += self.stride * self.n_req
        self.desc_off = off
        self.const_off = self.desc_end
        self.consts = [struct.unpack(">I", blob[self.const_off + 4 * i:self.const_off + 4 * i + 4])[0]
                       for i in range(self.n_const)]
        self.path_off = self.const_off + self.const_stride * self.n_const
        self.str_off = self.path_off + 2 * self.n_path
        self.ordinals = {}
        self.index_to_ordinal = {}
        for i in range(self.n_req):
            o, idx = struct.unpack(">HH", blob[self.ord_off + 4 * i:self.ord_off + 4 * i + 4])
            self.ordinals[o] = idx
            self.index_to_ordinal[idx] = o
        self.entries = []
        for i in range(self.n_req):
            e = struct.unpack(">HHIIIIHH", blob[self.ent_off + self.stride * i:self.ent_off + self.stride * i + 24])
            self.entries.append(e)
        self._cache = {}

    @classmethod
    def load(cls, image):
        with open(image, "rb") as f:
            f.seek(SCHEMA_VA - XAM_BASE)
            return cls(f.read(SCHEMA_SIZE))

    # ---- table helpers
    def path(self, i):
        if i >= self.n_path:
            return ""
        po = struct.unpack(">H", self.b[self.path_off + 2 * i:self.path_off + 2 * i + 2])[0]
        s = self.str_off + po
        e = self.b.index(b"\0", s)
        return self.b[s:e].decode("latin-1")

    def const(self, i):
        if i > self.n_const:      # runtime check is 'idx > n' (off by one, as in the original)
            raise SchemaError("const index %d out of range" % i)
        return self.consts[i]

    def entry(self, index):
        rl, sl, ro, so, mq, ms, ci, pi = self.entries[index]
        return dict(index=index, ordinal=self.index_to_ordinal.get(index), req_len=rl, resp_len=sl,
                    req_off=ro, resp_off=so, max_req=mq, max_resp=ms,
                    service=self.consts[ci] if ci < self.n_const else None, const_idx=ci,
                    path=self.path(pi), path_idx=pi)

    def index_of(self, key):
        """key: ordinal (int) or '#n' for a raw index."""
        if isinstance(key, str):
            if key.startswith("#"):
                return int(key[1:], 0)
            key = int(key, 16)
        if key not in self.ordinals:
            raise SchemaError("unknown ordinal 0x%X" % key)
        return self.ordinals[key]

    def raw_desc(self, index, response):
        rl, sl, ro, so = self.entries[index][:4]
        L, O = (sl, so) if response else (rl, ro)
        return self.b[self.desc_off + O:self.desc_off + O + L]

    def tree(self, index, response):
        k = (index, response)
        if k not in self._cache:
            raw = self.raw_desc(index, response)
            self._cache[k] = None if not raw else self._parse(raw)
        return self._cache[k]

    # ---- descriptor grammar (order of optional parts = SkipScope 0x9224EBD8)
    def _arr(self, r):
        a = Arr()
        c0 = r.u8()
        a.c1 = r.u8()
        a.slot = c0 & 0x3F
        a.wide = bool(c0 & 0x40)
        a.inline = bool(c0 & 0x80)
        a.const_idx = a.const = None
        if a.inline:
            a.const_idx = r.le16()
            a.const = self.const(a.const_idx)
        return a

    def _parse(self, raw):
        r = _Rd(raw)
        s = self._scope(r, 0)
        return s

    def _scope(self, r, depth):
        if depth > 15:
            raise SchemaError("nesting > 16")
        s = Scope()
        H = r.u8()
        s.hdr = H
        k = (H >> 2) & 3
        if k not in (1, 2):
            raise SchemaError("scope header 0x%02X is not struct/union" % H)
        s.is_union = (k == 2)
        s.native = r.le16()
        s.align = H & 3
        s.argb = r.u8() if H & 0x80 else None
        s.w40 = r.le16() if H & 0x40 else None
        s.arr = self._arr(r) if H & 0x20 else None
        s.fields = []
        s.cases = []
        s.name = None
        if s.is_union:
            u0 = r.u8()
            n = r.u8()
            s.sel_slot = u0 & 0x3F
            s.required = bool(u0 & 0x80)
            s.mask = self.const(r.le16()) if u0 & 0x40 else 0xFFFFFFFF
            for _ in range(n):
                nl = r.u8()
                labels = [self.const(r.le16()) for _ in range(nl)]
                blen = r.le16()
                start = r.p
                e = self._elem(r, depth)
                if r.p - start != blen:
                    raise SchemaError("union case length mismatch")
                s.cases.append((labels, e))
            if not r.eof():
                end = r.u8()
                if (end & 0xC) != 0xC:
                    raise SchemaError("union not terminated")
        else:
            s.sel_slot = s.mask = s.required = None
            while not r.eof():           # a descriptor may end without END (e.g. empty top struct)
                e = self._elem(r, depth)
                if e is None:
                    break
                s.fields.append(e)
        return s

    def _elem(self, r, depth):
        P = r.d[r.p]
        k = (P >> 2) & 3
        if k == 3:
            r.p += 1
            return None
        if k:
            return self._scope(r, depth + 1)
        r.p += 1
        p = Prim()
        p.log2 = P & 3
        p.size = 1 << p.log2
        p.name = None
        p.argb = r.u8() if P & 0x80 else None
        p.w40 = r.le16() if P & 0x40 else None
        p.arr = self._arr(r) if P & 0x20 else None
        if P & 0x10:
            e = Est()
            k0 = r.u8()
            k1 = r.u8()
            e.slot = k0 & 0x3F
            e.sizebits = k0 >> 6
            e.div = bool(k1 & 1)
            e.max_idx = r.le16()
            e.max = self.const(e.max_idx)
            p.est = e
        else:
            p.est = None
        return p

    # ----------------------------------------------------------------------
    # describe
    # ----------------------------------------------------------------------
    def describe(self, index, out=print):
        e = self.entry(index)
        out("index %d  ordinal 0x%04X  service 0x%X  path %s  maxReq %d  maxResp %d"
            % (index, e["ordinal"] or 0, e["service"] or 0, e["path"], e["max_req"], e["max_resp"]))
        for resp in (False, True):
            t = self.tree(index, resp)
            out("  %s descriptor (%d bytes): %s" % ("RESPONSE" if resp else "REQUEST",
                                                     len(self.raw_desc(index, resp)),
                                                     self.raw_desc(index, resp).hex()))
            if t is None:
                out("    (empty)")
                continue
            _name_tree(t)
            if resp:
                native_layout(t)
            self._desc_scope(t, resp, out, "    ", "")

    def _arr_txt(self, a):
        if a.inline:
            return "[%d] inline" % a.const
        return "[count=slot%d] ->ptr" % a.slot

    def _desc_scope(self, s, resp, out, ind, label):
        bits = []
        if s.argb is not None:
            bits.append("arg#%d%s" % (s.argb & 0x1F, " (not-on-wire)" if s.argb & 0x20 else ""))
        if s.w40 is not None:
            bits.append("w40=%d" % s.w40)
        if s.align:
            bits.append("wire-align %d" % (1 << s.align))
        if s.arr:
            bits.append("array " + self._arr_txt(s.arr))
        noff = getattr(s, "_noff", None)
        pre = ("@%-4s " % noff) if (resp and noff is not None) else ""
        if s.is_union:
            out("%s%s%s%s union native=%d selector=slot%d mask=0x%X%s %s"
                % (ind, pre, label, s.name, s.native, s.sel_slot, s.mask,
                   " required" if s.required else "", " ".join(bits)))
            for labels, e in s.cases:
                lab = "case %s: " % ",".join("0x%X" % x for x in labels)
                self._desc_elem(e, resp, out, ind + "  ", lab)
        else:
            out("%s%s%s%s struct native=%d %s" % (ind, pre, label, s.name, s.native, " ".join(bits)))
            for f in s.fields:
                self._desc_elem(f, resp, out, ind + "  ", "")
            calc = getattr(s, "_ncalc", None)
            if resp and calc is not None and calc != s.native:
                out("%s  !! computed native size %d != declared %d" % (ind, calc, s.native))

    def _desc_elem(self, e, resp, out, ind, label):
        if e.kind == "scope":
            return self._desc_scope(e, resp, out, ind, label)
        t = PRIM_NAMES[e.log2]
        if e.arr:
            if e.arr.wide:
                t = "wstring (wire: utf8 bytes, count=slot%d bytes; native: WCHAR* UTF-16BE, NUL-terminated)" % e.arr.slot
            elif e.log2 == 0 and not e.arr.inline:
                t = "u8 blob %s" % self._arr_txt(e.arr)
            else:
                t = "%s %s" % (t, self._arr_txt(e.arr))
        bits = []
        if e.argb is not None:
            if resp:
                if e.argb & 1:
                    bits.append("HRESULT->task result")
                if e.argb & 2:
                    bits.append("wire-only (not stored)")
                if e.argb & 4:
                    bits.append("native-pad")
            else:
                bits.append("arg#%d%s" % (e.argb & 0x1F, " (not-on-wire)" if e.argb & 0x20 else ""))
        if e.w40 is not None:
            bits.append("w40=%d" % e.w40)
        if e.est:
            bits.append("=> sets slot%d (max %d%s)" % (e.est.slot, e.est.max, ", /size" if e.est.div else ""))
        noff = getattr(e, "_noff", None)
        pre = ("@%-4s " % noff) if (resp and noff is not None) else ""
        out("%s%s%s%s: %s %s" % (ind, pre, label, e.name, t, " ".join(bits)))

    # ----------------------------------------------------------------------
    # decoding (wire -> python).  Works for both directions.
    # ----------------------------------------------------------------------
    def decode(self, index, wire, response=False):
        t = self.tree(index, response)
        if t is None:
            return None
        _name_tree(t)
        st = _DecState(self, wire, response)
        v = st.scope(t, top=True)
        if st.p != len(wire):
            st.warn.append("%d trailing wire bytes" % (len(wire) - st.p))
        if st.warn:
            v = dict(v) if isinstance(v, dict) else {"value": v}
            v["_warnings"] = st.warn
        return v

    def decode_request(self, index, wire):
        return self.decode(index, wire, response=False)

    # ----------------------------------------------------------------------
    # encoding (python -> wire).  Works for both directions.
    # ----------------------------------------------------------------------
    def encode(self, index, values, response=True):
        t = self.tree(index, response)
        if t is None:
            return b""
        _name_tree(t)
        st = _EncState(self, response)
        st.scope(t, values, top=True)
        return bytes(st.w)

    def encode_response(self, index, values):
        return self.encode(index, values, response=True)

    # ----------------------------------------------------------------------
    # native unmarshal simulation (wire -> big-endian user buffer), as XAM does
    # ----------------------------------------------------------------------
    def unmarshal_native(self, index, wire, buf_size, user_base=0, default_count=0):
        t = self.tree(index, True)
        if t is None:
            return b"", 0
        u = _Unmarshal(self, wire, buf_size, user_base, default_count)
        u.run(t)
        return bytes(u.buf[:u.used]), u.hr


# ---- naming / layout -------------------------------------------------------
def _name_tree(s):
    if getattr(s, "_named", False):
        return
    s._named = True
    if s.name is None:
        s.name = "root"
    if s.is_union:
        for i, (_, e) in enumerate(s.cases):
            e.name = e.name or "case%d" % i
            if e.kind == "scope":
                _name_tree(e)
    else:
        for i, f in enumerate(s.fields):
            f.name = f.name or "f%d" % i
            if f.kind == "scope":
                _name_tree(f)


def native_layout(s, off=0, suppressed=False):
    """Annotate native offsets (relative to the enclosing allocation) the way
    FUN_81A3D7E8 writes them: fields are written back-to-back with no implicit
    padding; pointer arrays take 4 bytes; unions occupy their declared size."""
    s._noff = off
    if s.arr and not (s.arr.inline):
        _scope_body_layout(s, 0, suppressed)       # elements live in a fresh allocation
        return off + 4
    if s.arr and s.arr.inline:
        _scope_body_layout(s, 0, suppressed)
        return off + s.native * s.arr.const
    _scope_body_layout(s, off, suppressed)
    return off + (s.native if s.is_union else s._ncalc)


def _scope_body_layout(s, off, suppressed):
    start = off
    if s.is_union:
        for _, e in s.cases:
            if e.kind == "scope":
                native_layout(e, off, suppressed)
            else:
                e._noff = off
        s._ncalc = s.native
        return
    for f in s.fields:
        if f.kind == "scope":
            off = native_layout(f, off, suppressed)
        else:
            sup = suppressed or (f.argb is not None and f.argb & 2)
            f._noff = None if sup and not f.arr else off
            if f.arr and not f.arr.inline:
                off += 4
            elif sup:
                pass
            elif f.argb is not None and f.argb & 4:
                off += f.size
            elif f.arr and f.arr.inline:
                off += f.size * f.arr.const
            else:
                off += f.size
    s._ncalc = off - start


# ---- decoder ---------------------------------------------------------------
class _DecState:
    def __init__(self, sch, wire, response):
        self.s = sch
        self.w = wire
        self.p = 0
        self.resp = response
        self.slots = {}
        self.warn = []

    def take(self, n):
        if self.p + n > len(self.w):
            raise SchemaError("wire truncated at %d (+%d)" % (self.p, n))
        v = self.w[self.p:self.p + n]
        self.p += n
        return v

    def rd(self, size):
        return int.from_bytes(self.take(size), "little")

    def count(self, a):
        if a.inline:
            return a.const
        if a.slot == 0x3F:
            return None
        if a.slot not in self.slots:
            raise SchemaError("slot %d used before it was established" % a.slot)
        return self.slots[a.slot]

    def scope(self, s, top=False):
        if s.arr:
            n = self.count(s.arr)
            if n is None:
                raise SchemaError("array of structs with unknown count")
            return [self.instance(s) for _ in range(n)]
        return self.instance(s)

    def instance(self, s):
        if s.is_union:
            if s.sel_slot not in self.slots:
                raise SchemaError("union selector slot %d not set" % s.sel_slot)
            sel = (self.slots[s.sel_slot] or 0) & s.mask
            val = None
            hit = None
            for labels, e in s.cases:
                if sel in labels:
                    hit = e
                    break
            if hit is None:
                if s.required:
                    raise SchemaError("no union case for selector 0x%X" % sel)
            else:
                val = {"_case": sel, hit.name: self.elem(hit)}
            self.align(s)
            return val
        out = {}
        for f in s.fields:
            out[f.name] = self.elem(f)
        self.align(s)
        return out

    def align(self, s):
        if s.align:
            a = 1 << s.align
            pad = (-self.p) & (a - 1)
            self.take(pad)

    def elem(self, f):
        if f.kind == "scope":
            return self.scope(f)
        if not self.resp and f.argb is not None and f.argb & 0x20:
            # conformance-only argument: never written to the wire
            if f.est:
                self.slots[f.est.slot] = None
            return None
        if f.arr:
            n = self.count(f.arr)
            if n is None:
                raise SchemaError("array count unknown (slot from a not-on-wire argument)")
            if f.arr.wide:
                raw = self.take(n)
                return raw.decode("utf-8", "replace").rstrip("\0")
            if f.size == 1:
                return self.take(n)
            return [self.rd(f.size) for _ in range(n)]
        v = self.rd(f.size)
        if f.est:
            self.slots[f.est.slot] = v // f.size if f.est.div else v
        return v


# ---- encoder ---------------------------------------------------------------
class _EncState:
    def __init__(self, sch, response):
        self.s = sch
        self.resp = response
        self.w = bytearray()
        self.slots = {}          # slot -> [pos, size, value, explicit]

    def put(self, v, size):
        self.w += int(v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")

    def patch(self, slot, value):
        pos, size, _, _ = self.slots[slot]
        self.w[pos:pos + size] = int(value).to_bytes(size, "little")
        self.slots[slot][2] = value

    @staticmethod
    def pick(values, f, i):
        if values is None:
            return None
        if isinstance(values, dict):
            return values.get(f.name)
        if isinstance(values, (list, tuple)):
            return values[i] if i < len(values) else None
        return None

    def scope(self, s, v, top=False):
        if s.arr:
            items = list(v or [])
            if s.arr.inline:
                if len(items) > s.arr.const:
                    raise SchemaError("%s: %d items > fixed %d" % (s.name, len(items), s.arr.const))
                items += [None] * (s.arr.const - len(items))
            else:
                self.consume(s.arr.slot, len(items), s.name)
            for it in items:
                self.instance(s, it)
        else:
            self.instance(s, v)

    def consume(self, slot, n, who):
        if slot == 0x3F:
            return
        if slot not in self.slots:
            raise SchemaError("%s: count slot %d not established before use" % (who, slot))
        self.patch(slot, n)

    def instance(self, s, v):
        if s.is_union:
            sel = None
            if isinstance(v, dict) and "_case" in v:
                sel = v["_case"]
                if s.sel_slot in self.slots and not self.slots[s.sel_slot][3]:
                    self.patch(s.sel_slot, sel)
            if sel is None:
                sel = self.slots.get(s.sel_slot, [0, 0, 0])[2]
            sel &= s.mask
            hit = next((e for labels, e in s.cases if sel in labels), None)
            if hit is None and s.required:
                raise SchemaError("%s: no case for selector 0x%X" % (s.name, sel))
            if hit is not None:
                hv = v.get(hit.name) if isinstance(v, dict) else v
                self.elem(hit, hv)
        else:
            for i, f in enumerate(s.fields):
                self.elem(f, self.pick(v, f, i))
        if s.align:
            a = 1 << s.align
            self.w += b"\0" * ((-len(self.w)) & (a - 1))

    def elem(self, f, v):
        if f.kind == "scope":
            return self.scope(f, v)
        if not self.resp and f.argb is not None and f.argb & 0x20:
            if f.est:
                self.slots[f.est.slot] = [len(self.w), 0, v or 0, True]
            return
        if f.arr:
            if f.arr.wide:
                data = (v or "").encode("utf-8")
                n = len(data)
            elif f.size == 1:
                data = bytes(v or b"")
                n = len(data)
            else:
                vals = list(v or [])
                n = len(vals)
                data = b"".join(int(x & ((1 << (8 * f.size)) - 1)).to_bytes(f.size, "little") for x in vals)
            if f.arr.inline:
                if n > f.arr.const:
                    raise SchemaError("%s: %d > fixed %d" % (f.name, n, f.arr.const))
                data += b"\0" * (f.size * (f.arr.const - n))
            else:
                self.consume(f.arr.slot, n, f.name)
            self.w += data
            return
        explicit = v is not None
        val = int(v or 0)
        if f.est:
            if f.est.div:
                val *= f.size
            self.slots[f.est.slot] = [len(self.w), f.size, val, explicit]
        self.put(val, f.size)


# ---- native unmarshal simulation (what the C++ runtime must do) -------------
class _Unmarshal:
    """Big-endian native image of the user buffer, laid out like FUN_81A3D7E8."""

    def __init__(self, sch, wire, size, base, default_count):
        self.s = sch
        self.w = wire
        self.p = 0
        self.buf = bytearray(size)
        self.size = size
        self.used = 0
        self.base = base
        self.slots = {}          # slot -> [value, native_addr, size]
        self.hr = 0
        self.default = default_count

    def alloc(self, n):           # FUN_81A3D650
        if self.size - self.used < n:
            raise SchemaError("0x8007007A ERROR_INSUFFICIENT_BUFFER (need %d, have %d)" % (n, self.size - self.used))
        a = self.used
        self.used += n
        return a

    def rd(self, n):
        if self.p + n > len(self.w):
            raise SchemaError("0x80182001 wire exhausted (need more data)")
        v = self.w[self.p:self.p + n]
        self.p += n
        return v

    def run(self, t):
        top = [self.alloc(t.native)]   # level-0 struct allocated at the start of the user buffer
        self.frame(t, top, False)

    def wr(self, cur, data, sup):
        if sup:
            return
        self.buf[cur[0]:cur[0] + len(data)] = data
        cur[0] += len(data)

    def count(self, a):
        if a.inline:
            return a.const
        if a.slot == 0x3F:
            return self.default
        return self.slots[a.slot][0]

    def ptr(self, cur, target, n, sup):
        self.buf[cur[0]:cur[0] + 4] = struct.pack(">I", (self.base + target) if n else 0)
        cur[0] += 4

    def frame(self, s, cur, sup):
        n = 1
        if s.arr:
            n = self.count(s.arr)
            if not s.arr.inline:
                tgt = self.used
                self.ptr(cur, tgt, n, sup)
                cur = [self.alloc(n * s.native)] if n else [tgt]
        for _ in range(n):
            if s.is_union:
                start = cur[0]
                sel = self.slots.get(s.sel_slot, [0])[0] & s.mask
                hit = next((e for labels, e in s.cases if sel in labels), None)
                if hit is None and s.required:
                    raise SchemaError("union selector 0x%X has no case" % sel)
                if hit is not None:
                    self.elem(hit, cur, sup)
                cur[0] = start + s.native
            else:
                for f in s.fields:
                    self.elem(f, cur, sup)
            if s.align:
                a = 1 << s.align
                self.rd((-self.p) & (a - 1))

    def elem(self, f, cur, sup):
        if f.kind == "scope":
            return self.frame(f, cur, sup)
        fsup = sup or (f.argb is not None and bool(f.argb & 2))
        if f.argb is not None and f.argb & 4:
            cur[0] += f.size
            return
        if f.arr:
            n = self.count(f.arr)
            out = cur
            if not f.arr.inline:
                if n == 0:
                    self.ptr(cur, 0, 0, fsup)
                else:
                    if f.arr.wide:
                        raw = self.w[self.p:self.p + n]
                        need = (len(raw.decode("utf-8", "replace")) + 1) * 2
                    else:
                        need = n * f.size
                    tgt = self.used
                    self.ptr(cur, tgt, n, fsup)
                    out = [self.alloc(need)]
            if n == 0:
                return
            if f.arr.wide:
                txt = self.rd(n).decode("utf-8", "replace")
                data = txt.encode("utf-16-be") + b"\0\0"
                self.wr(out, data, fsup)
                ent = self.slots.get(f.arr.slot)
                if ent and ent[1] is not None:              # FUN_81833948: rewrite native count
                    self.buf[ent[1]:ent[1] + ent[2]] = len(data).to_bytes(ent[2], "big")
                return
            for _ in range(n):
                v = self.rd(f.size)
                self.wr(out, v[::-1], fsup)
            return
        raw = self.rd(f.size)
        v = int.from_bytes(raw, "little")
        self.wr(cur, raw[::-1], fsup)
        if f.argb is not None and f.argb & 1:
            if v & 0x80000000:
                self.hr = v
        if f.est:
            self.slots[f.est.slot] = [v // f.size if f.est.div else v,
                                      None if fsup else cur[0] - f.size, f.size]


# --------------------------------------------------------------------------
def list_requests(sch, out=print):
    out("%-5s %-7s %-9s %-7s %-7s %s" % ("index", "ordinal", "service", "maxReq", "maxResp", "path"))
    for i in range(sch.n_req):
        e = sch.entry(i)
        out("%-5d 0x%04X  0x%-7X %-7d %-7d %s" % (i, e["ordinal"] or 0, e["service"] or 0,
                                                  e["max_req"], e["max_resp"], e["path"]))


def _sample_value(node, depth=0):
    """Build a plausible python value for a response tree (for tests)."""
    if node.kind == "scope":
        def one():
            if node.is_union:
                if not node.cases:
                    return None
                labels, e = node.cases[0]
                return {"_case": labels[0], e.name: _sample_value(e, depth + 1)}
            return {f.name: _sample_value(f, depth + 1) for f in node.fields}
        if node.arr:
            n = node.arr.const if node.arr.inline else 2
            return [one() for _ in range(n)]
        return one()
    if node.arr:
        if node.arr.wide:
            return "Gamer\u00e9 %d" % depth
        n = node.arr.const if node.arr.inline else 3
        if node.size == 1:
            return bytes(range(1, n + 1))
        return list(range(1, n + 1))
    if node.est:
        return None                 # auto-filled by the encoder
    return 0x11223344 & ((1 << (8 * node.size)) - 1)


def selftest(sch):
    print("schema version %d.%d  consts %d  paths %d  requests %d" %
          (sch.version[0], sch.version[1], sch.n_const, sch.n_path, sch.n_req))
    bad = 0
    for i in range(sch.n_req):
        for resp in (False, True):
            try:
                t = sch.tree(i, resp)
                if t is not None and resp:
                    _name_tree(t)
                    native_layout(t)
            except SchemaError as e:
                bad += 1
                print("parse error", i, resp, e)
    print("all %d x 2 descriptors parsed, %d errors" % (sch.n_req, bad))
    for o in (0x801, 0x604, 0x1102, 0x703, 0x70B):
        idx = sch.index_of(o)
        print("=" * 78)
        sch.describe(idx)
        t = sch.tree(idx, True)
        if t is None:
            continue
        v = _sample_value(t)
        wire = sch.encode_response(idx, v)
        print("  sample response wire (%d bytes): %s" % (len(wire), wire.hex()))
        back = sch.decode(idx, wire, response=True)
        print("  decoded back:", back)
        nat, hr = sch.unmarshal_native(idx, wire, 4096, user_base=0x40000000)
        print("  native user buffer (%d bytes, base 0x40000000): %s" % (len(nat), nat.hex()))
        # request round trip
        tq = sch.tree(idx, False)
        if tq is not None:
            vq = _sample_value(tq)
            wq = sch.encode(idx, vq, response=False)
            print("  sample request wire (%d bytes): %s -> %s" % (len(wq), wq.hex(), sch.decode_request(idx, wq)))


def main(argv):
    if len(argv) < 3:
        return print(__doc__)
    sch = Schema.load(argv[1])
    if argv[2] == "selftest":
        return selftest(sch)
    if argv[2] == "list":
        return list_requests(sch)
    if argv[2] == "describe":
        return sch.describe(sch.index_of(argv[3]))
    print(__doc__)


if __name__ == "__main__":
    main(sys.argv)
