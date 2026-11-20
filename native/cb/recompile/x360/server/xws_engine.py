"""XAM's web-service engine (XLiveBase 0x5806A..0x5806F: XOnlineCallWebService and the Begin/Continue calls),
reconstructed on the server: the client sends the raw call (URL template, method, flags, the request and response
schema strings and the caller's request bytes) and gets back the bytes XAM would have left in the caller's response
buffer. Behaviour follows XAM 17559 (addresses are XAM's; the engine is our own code):

  schema grammar      field := NAME "{" key:value,... "}" ["[" fields "]"]       (0x81A62298, 0x81A630D0)
  URL / query / hdrs  {LiveHive:X} and {field} tokens; "?"/"&" + name=value (percent-encoded); header order
                      (0x818F74B8, 0x818F7D00, 0x818F7E30)
  JSON request body   members in schema order, opt/count-0 skipped (0x81900600)
  response            buffer zeroed; loc:hdr from headers; a stream body gets its length; an item body is parsed by
                      Content-Type (JSON / XML) with XAM's path mapper (0x818F1318 ...); overflow -> 0x003E0001

All values in the caller's buffers are big-endian; wstr is UTF-16BE; offsets may be unaligned.
"""
import base64
import datetime
import json
import re
import urllib.parse
import xml.sax
import xml.sax.handler

T = {"stream": 1, "item": 2, "str": 3, "wstr": 4, "int": 5, "uint": 6, "bool": 7, "datetime": 8, "bin": 9,
     "xuid": 10, "hrfacility": 11, "hrcode": 12}
LOC = {"url": 1, "qs": 2, "hdr": 3, "dom": 4, "body": 5}
ENC = {"url": 1, "json": 2, "xml": 3, "binary": 4, "hex": 5, "base64": 6, "wav16000": 7, "sirensr": 8}
CONTENT_TYPES = {"json": "application/json", "xml": "application/xml",
                 "binary": "application/octet-stream\r\nContent-Transfer-Encoding: binary",
                 "wav16000": 'audio/x-wav; codec="audio/pcm"; samplingrate=16000', "sirensr": "audio/sirensr"}
S_OK, S_TRUNCATED = 0, 0x003E0001
E_INVALIDARG, E_NOTIMPL = 0x80070057, 0x80004001


class SchemaError(ValueError):
    pass


class Field:
    __slots__ = ("name", "t", "loc", "part", "enc", "offset", "size", "max", "countoffset", "countsize", "opt",
                 "children")

    def __init__(self, name):
        self.name, self.t, self.loc, self.part, self.enc = name, None, None, 0, None
        self.offset = self.size = self.max = 0
        self.countoffset, self.countsize, self.opt = -1, 0, None
        self.children = []

    @property
    def is_array(self):
        return self.max and self.countoffset >= 0 and self.countsize

    @property
    def elemsize(self):
        return self.size // self.max if self.is_array else self.size

    def __repr__(self):
        return "%s{t:%s,loc:%s,off:%d,size:%d}" % (self.name, self.t, self.loc, self.offset, self.size)


def parse_schema(text):
    """The field list of a schema string (attributes as XAM 0x81A62298 reads them)."""
    if not text:
        return []
    pos = 0

    def fields(end_char):
        nonlocal pos
        out = []
        while pos < len(text) and text[pos] != end_char:
            brace = text.index("{", pos)
            f = Field(text[pos:brace])
            close = text.index("}", brace)
            for attr in text[brace + 1:close].split(","):
                if not attr:
                    continue
                key, _, value = attr.partition(":")
                if key == "t":
                    f.t = value
                elif key == "loc":
                    if value.startswith("part"):
                        f.loc, f.part = "part", int(value[4:])
                    else:
                        f.loc = value
                elif key == "enc":
                    f.enc = value
                elif key in ("offset", "size", "max", "countoffset", "countsize"):
                    setattr(f, key, int(value))
                elif key == "opt":
                    f.opt = int(value)
                elif key == "itemsize":
                    pass  # accepted and ignored by XAM: the element size is size / max
                else:
                    raise SchemaError("unknown schema key " + key)
            if f.t not in T or (f.loc not in LOC and f.loc != "part"):
                raise SchemaError("bad field " + text[pos:close + 1])
            pos = close + 1
            if pos < len(text) and text[pos] == "[":
                pos += 1
                f.children = fields("]")
                pos += 1  # ']'
            out.append(f)
            if pos < len(text) and text[pos] == ",":
                pos += 1
        return out

    return fields("\0")


# ---- reading the caller's request data ----------------------------------------------------------------------------
class Buf:
    def __init__(self, data):
        self.d = data

    def uint(self, at, size):
        return int.from_bytes(self.d[at:at + size].ljust(size, b"\0"), "big") if at >= 0 else 0

    def sint(self, at, size):
        return int.from_bytes(self.d[at:at + size].ljust(size, b"\0"), "big", signed=True)


def enabled(buf, f, parent_base):
    return f.opt is None or buf.uint(parent_base + f.opt, 4) != 0


def count_of(buf, f, parent_base):
    return min(buf.uint(parent_base + f.countoffset, f.countsize), f.max)


def value_text(buf, f, at, mode):
    """A scalar field's text (0x818FBCB0): mode 0 raw, 1 URL, 2 XML, 3 JSON."""
    if f.enc == "url" and mode == 0:
        mode = 1
    t = f.t
    if t == "str":
        raw = buf.d[at:at + f.size]
        if b"\0" not in raw:
            return ""
        s = raw[:raw.index(b"\0")].decode("utf-8", "replace")
    elif t == "wstr":
        raw = buf.d[at:at + f.size]
        units = [int.from_bytes(raw[i:i + 2], "big") for i in range(0, len(raw) - 1, 2)]
        if 0 in units:
            units = units[:units.index(0)]
        s = "".join(chr(u) for u in units).encode("utf-16", "surrogatepass").decode("utf-16", "replace")
    elif t == "int":
        s = str(buf.sint(at, f.size))
    elif t in ("uint", "xuid"):
        s = str(buf.uint(at, f.size))
    elif t == "bool":
        s = "true" if buf.uint(at, f.size) else "false"
    elif t == "datetime":
        y, mo, _dow, d, h, mi, sec, ms = (buf.uint(at + 2 * i, 2) for i in range(8))
        s = "%04d-%02d-%02dT%02d:%02d:%02d.%03d0000Z" % (y, mo, d, h, mi, sec, ms)
    elif t == "bin":
        n = buf.uint(at - f.offset + f.countoffset, f.countsize) if f.countoffset >= 0 else f.size
        data = buf.d[at:at + min(n, f.size)]
        s = data.hex() if f.enc == "hex" else base64.b64encode(data).decode("ascii")
    else:
        raise SchemaError("type %s cannot be sent" % t)
    if mode == 1:
        return urllib.parse.quote(s, safe="-._~")
    if mode == 2:
        return (s.replace("&", "&amp;").replace('"', "&quot;").replace("'", "&apos;").replace("<", "&lt;")
                .replace(">", "&gt;"))
    if mode == 3:
        return json.dumps(s, ensure_ascii=False)[1:-1].replace("/", "\\/")
    return s


def json_body(buf, item, base):
    """The JSON a request `item` body generates (0x81900600): the root `?` object, members in schema order."""
    root = next((c for c in item.children if c.loc == "dom"), None)
    if root is None:
        raise SchemaError("body item without a dom root")

    def obj(rec, b):
        parts = []
        for c in rec.children:
            if c.loc != "dom" or not enabled(buf, c, b):
                continue
            name = "root" if c.name == "?" else c.name
            if c.is_array:
                n = count_of(buf, c, b)
                if n == 0:
                    continue
                elems = [element(c, b + c.offset + i * c.elemsize) for i in range(n)]
                parts.append('"%s":[%s]' % (name, ",".join(elems)))
            else:
                parts.append('"%s":%s' % (name, element(c, b + c.offset)))
        return "{" + ",".join(parts) + "}"

    def element(c, at):
        if c.t == "item":
            return obj(c, at)
        text = value_text(buf, c, at, 3)
        return '"%s"' % text if c.t in ("str", "wstr", "datetime", "bin") else text

    return obj(root, base + root.offset).encode("utf-8")


def build_request(url_template, method, req_schema, req_data, livehive):
    """(method, url, [(header, value)], body bytes) as XAM sends them."""
    fields = parse_schema(req_schema)
    buf = Buf(req_data)
    by_name = {f.name: f for f in fields if f.loc == "url"}

    def token(m):
        name = m.group(1)
        if name.lower().startswith("livehive:") and len(name) >= 10:
            return str(livehive.get(name[9:], ""))[:127]
        f = by_name.get(name)
        if f is None or not enabled(buf, f, 0):
            return ""
        return value_text(buf, f, f.offset, 0)

    url = re.sub(r"\{([^}]*)\}", token, url_template)
    first = True
    for f in fields:
        if f.loc == "qs" and enabled(buf, f, 0):
            url += ("?" if first else "&") + f.name + "=" + value_text(buf, f, f.offset, 1)
            first = False
    headers, body = [], b""
    for f in fields:
        if not enabled(buf, f, 0):
            continue
        if f.loc == "hdr":
            headers.append(("root" if f.name == "?" else f.name, value_text(buf, f, f.offset, 0)))
        elif f.loc == "body":
            if f.enc in CONTENT_TYPES:
                for line in CONTENT_TYPES[f.enc].split("\r\n"):
                    k, _, v = line.partition(": ") if ": " in line else ("Content-Type", "", line)
                    headers.append((k, v))
            if f.t == "item":
                if f.enc == "xml":
                    raise SchemaError("XML request bodies are not generated (no 17559 caller uses one)")
                body = json_body(buf, f, f.offset)
            elif f.t == "stream":
                raise SchemaError("stream request bodies (ContinueRequest) are not supported yet")
        elif f.loc == "part":
            raise SchemaError("multipart request bodies are not supported yet")
    return method, url, headers, body


# ---- filling the caller's response buffer --------------------------------------------------------------------------
class Out:
    def __init__(self, size):
        self.d = bytearray(size)
        self.overflow = False

    def put(self, at, data):
        if at < 0 or at + len(data) > len(self.d):
            self.overflow = True
            return
        self.d[at:at + len(data)] = data

    def uint(self, at, size):
        return int.from_bytes(self.d[at:at + size], "big")

    def set_uint(self, at, size, v):
        self.put(at, (v & ((1 << (8 * size)) - 1)).to_bytes(size, "big"))


def c_strtoi64(text, signed):
    """_strtoi64 / _strtoui64 with base 0 over the longest valid prefix; out of range -> all ones (clamp)."""
    m = re.match(r"\s*([+-]?)(0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)", text)
    if not m:
        return 0, True
    digits = m.group(2)
    v = int(digits, 16) if digits[:2].lower() == "0x" else int(digits, 8) if digits.startswith("0") else int(digits)
    if m.group(1) == "-":
        v = -v
    lo, hi = (-(1 << 63), (1 << 63) - 1) if signed else (0, (1 << 64) - 1)
    if not signed and v < 0:
        v &= (1 << 64) - 1
    return (v, False) if lo <= v <= hi else (-1, True)


def parse_datetime(text):
    m = re.match(r"(\d{4})-(\d\d)-(\d\d)T(\d\d):(\d\d):(\d\d)(\.\d+)?(Z|[+-]\d\d:\d\d)?$", text.strip())
    if not m:
        return None
    y, mo, d, h, mi, s = (int(m.group(i)) for i in range(1, 7))
    ms = int((m.group(7) or ".0")[1:4].ljust(3, "0"))
    if y < 1601:
        return (1601, 1, 1, 1, 0, 0, 0, 0)
    dt = datetime.datetime(y, mo, d, h, mi, s)
    tz = m.group(8)
    if tz and tz != "Z":
        sign = 1 if tz[0] == "+" else -1
        dt -= sign * datetime.timedelta(hours=int(tz[1:3]), minutes=int(tz[4:6]))
    return (dt.year, dt.month, (dt.weekday() + 1) % 7, dt.day, dt.hour, dt.minute, dt.second, ms)


def write_value(out, f, at, text, flags):
    t = f.t
    if t == "str":
        raw = text.encode("utf-8")
        if len(raw) + 1 > f.size:
            out.overflow = True
            raw = raw[:f.size - 1] if flags & 4 else b""
        out.put(at, raw + b"\0")
    elif t == "wstr":
        units = text.encode("utf-16-be")[:max(0, (f.size // 2 - 1) * 2)]
        out.put(at, units + b"\0\0")
    elif t in ("int", "uint", "xuid"):
        v, bad = c_strtoi64(text, t == "int")
        if bad or not (-(1 << (8 * f.size - 1)) <= v < (1 << (8 * f.size))):
            out.overflow, v = True, -1
        out.set_uint(at, f.size, v)
    elif t == "bool":
        out.set_uint(at, 4, 1 if text.lower().startswith("true") else 0)
    elif t == "datetime":
        st = parse_datetime(text)
        if st:
            out.put(at, b"".join(v.to_bytes(2, "big") for v in st))
    elif t == "bin":
        try:
            data = bytes.fromhex(text) if f.enc == "hex" else base64.b64decode(text)
        except ValueError:
            data = b""
        out.put(at, data[:f.size])
        if f.countoffset >= 0:
            out.set_uint(at - f.offset + f.countoffset, f.countsize, min(len(data), f.size))
    elif t == "hrfacility":
        out.set_uint(at, 4, out.uint(at, 4) | 0x83800000)  # (facility names: not mapped; XAM's unknown-name form)
    elif t == "hrcode":
        v, _ = c_strtoi64(text, False)
        out.set_uint(at, 4, out.uint(at, 4) | 0x80000000 | (v & 0xFFFF))


class Mapper:
    """XAM's response mapper (0x818F1318 open / 0x818F0E58 value / 0x818F0D10 close / path append-strip)."""

    def __init__(self, out, body, flags):
        self.out, self.flags = out, flags
        self.root = next((c for c in body.children if c.loc == "dom"), None)
        if self.root is None:
            raise SchemaError("response body item without a dom root")
        self.body_base = body.offset
        self.frames = []  # (record, base, parent_base, dropped)
        self.path = []

    def append(self, name):
        self.path.extend(name.split("/"))

    def strip(self):
        if self.path:
            self.path.pop()

    def open(self):
        if not self.frames:
            self.frames.append((self.root, self.body_base + self.root.offset, self.body_base, False))
            self.path = []
            return
        rec, base, _, dropped = self.frames[-1]
        want = "/".join(self.path)
        child = next((c for c in rec.children if c.loc == "dom" and (c.name == want or c.name == "?")), None)
        if child is None:
            return
        if dropped:
            self.frames.append((child, 0, 0, True))
        elif child.is_array:
            n = self.out.uint(base + child.countoffset, child.countsize)
            if n >= child.max:
                self.out.overflow = True
                self.frames.append((child, 0, 0, True))
            else:
                self.out.set_uint(base + child.countoffset, child.countsize, n + 1)
                self.frames.append((child, base + child.offset + n * child.elemsize, base, False))
        else:
            self.frames.append((child, base + child.offset, base, False))
        self.path = []

    def value(self, text):
        if self.path or not self.frames:
            return
        rec, base, _, dropped = self.frames[-1]
        if not dropped and rec.t != "item":
            write_value(self.out, rec, base, text, self.flags)

    def close(self):
        if self.path or not self.frames:
            return
        rec, base, parent_base, dropped = self.frames.pop()
        if not dropped and rec.opt is not None and self.frames:
            self.out.set_uint(parent_base + rec.opt, 4, 1)
        self.path = rec.name.split("/") if self.frames else []

    def scalar(self, text):
        self.open()
        self.value(text)
        self.close()

    # JSON events (0x818F15F0)
    def json(self, v):
        if isinstance(v, dict):
            self.open()
            for i, (k, member) in enumerate(v.items()):
                if i:
                    self.strip()
                self.append(k)
                self.json(member)
            if v:
                self.strip()
            self.close()
        elif isinstance(v, list):
            for e in v:
                self.json(e)
        elif v is None:
            pass
        elif isinstance(v, bool):
            self.scalar("true" if v else "false")
        elif isinstance(v, float):
            self.scalar(repr(v))
        else:
            self.scalar(str(v))


class _XmlEvents(xml.sax.handler.ContentHandler):
    def __init__(self, mapper):
        super().__init__()
        self.m, self.text = mapper, []

    def startElement(self, name, attrs):
        self.flush()
        self.m.append(name.split(":")[-1])
        self.m.open()
        for k in attrs.getNames():
            self.m.append("@" + k.split(":")[-1])
            self.m.scalar(attrs.getValue(k))
            self.m.strip()

    def characters(self, content):
        self.text.append(content)

    def flush(self):
        text = "".join(self.text)
        self.text = []
        if text.strip():
            self.m.value(text)

    def endElement(self, name):
        self.flush()
        self.m.close()
        self.m.strip()


def fill_response(resp_schema, resp_size, status, headers, body, flags):
    """(response buffer bytes, HRESULT, body length or None) as XAM leaves them after BeginResponse / a whole call:
    a stream body only gets its length (the bytes go out through ContinueResponse)."""
    out = Out(resp_size)
    hdrs = {k.lower(): v for k, v in headers.items()}
    ctype = hdrs.get("content-type", "").lower()
    stream_len = None
    for f in parse_schema(resp_schema):
        if f.loc == "hdr":
            name = "root" if f.name == "?" else f.name
            if name.lower() in hdrs:
                write_value(out, f, f.offset, hdrs[name.lower()], flags)
                if f.opt is not None:
                    out.set_uint(f.opt, 4, 1)
        elif f.loc == "body":
            if f.t == "stream":
                out.set_uint(f.offset, f.size, len(body))
                stream_len = len(body)
            elif f.t == "item" and body:
                if f.offset + f.size > resp_size:
                    return bytes(resp_size), E_INVALIDARG, None
                mapper = Mapper(out, f, flags)
                if "json" in ctype or "text/javascript" in ctype:
                    text = body.decode("utf-8", "replace")
                    if flags & 0x2000:
                        text = '{"default":' + text + "}"
                    mapper.json(json.loads(text))
                elif "xml" in ctype:
                    xml.sax.parseString(body, _XmlEvents(mapper))
        else:
            break
    return bytes(out.d), (S_TRUNCATED if out.overflow else S_OK), stream_len
