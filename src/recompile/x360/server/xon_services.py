"""Our Xbox LIVE's ".ashx" services: the binary Xbox LIVE service tasks (XLiveBase message 0x50000 | schema index:
XMessageSend, XeGetUserInfo, vetstring2, xstatread, ...), answered here instead of Microsoft's front doors.

The client sends the service schema it has in memory once per connection (op `xon_schema`), then each task as op
`xon` {index, wire (the marshalled request, base64), user_base, buf_size, default_count}. We decode the request with
the schema (xon_schema.py), let the path's handler below build the response values, encode them to the wire form and
unmarshal that into the caller's user buffer exactly as XAM does (Schema.unmarshal_native): the reply is
{hr, native (base64; the bytes for the user buffer at user_base), used}. A path with no handler answers
XONLINE_E_LOGON_SERVICE_NOT_REQUESTED as before, so each service can be added on its own.

Handlers: fn(ctx, request_values) -> response values (dict of f0, f1 ...: xon_schema's names, in descriptor
order; count fields fill themselves), or raise XonError(hresult). `ctx` has .server, .store, .xuid (the caller, or
None), .user, .ordinal, .path.
"""
import hashlib
import logging

import xon_schema

log = logging.getLogger("live")

S_OK = 0
E_NOT_REQUESTED = 0x80151100   # XONLINE_E_LOGON_SERVICE_NOT_REQUESTED: "this Xbox LIVE has no such service"
E_FAIL = 0x80004005
E_BUFFER = 0x8007007A          # ERROR_INSUFFICIENT_BUFFER (the user buffer is too small)


class XonError(Exception):
    def __init__(self, hr, why=""):
        super().__init__("%08X %s" % (hr, why))
        self.hr = hr


class Ctx:
    def __init__(self, server, xuid, user, ordinal, path):
        self.server, self.store, self.xuid, self.user = server, server.store, xuid, user
        self.ordinal, self.path = ordinal, path


HANDLERS = {}


def handles(path):
    def register(fn):
        HANDLERS[path.lower()] = fn
        return fn
    return register


_schemas = {}  # sha1 of the blob -> Schema (all consoles send the same 17559 schema)


def load_schema(blob):
    key = hashlib.sha1(blob).hexdigest()
    if key not in _schemas:
        _schemas[key] = xon_schema.Schema(blob)
        log.info("xon: service schema %s (%d bytes, %d requests)", key[:12], len(blob), _schemas[key].n_req)
    return _schemas[key]


def call(server, schema, xuid, msg):
    index = int(msg.get("index", -1))
    if not 0 <= index < schema.n_req:
        raise ValueError("bad schema index")
    entry = schema.entry(index)
    path, ordinal = entry["path"], entry["ordinal"]
    wire = msg.get("wire_bytes", b"")
    handler = HANDLERS.get(path.lower())
    if handler is None:
        log.info("xon %s (0x%04X, index %d): no service here", path or "-", ordinal or 0, index)
        return {"hr": E_NOT_REQUESTED, "path": path}
    request = schema.decode_request(index, wire) if wire else None
    try:
        values = handler(Ctx(server, xuid, msg.get("user"), ordinal, path), request)
        out = schema.encode_response(index, values) if values is not None else b""
        native, hr = schema.unmarshal_native(index, out, int(msg.get("buf_size", 0)), int(msg.get("user_base", 0)),
                                             int(msg.get("default_count", 0)))
    except XonError as e:
        log.info("xon %s: %s", path, e)
        return {"hr": e.hr, "path": path}
    except xon_schema.SchemaError as e:
        too_small = "8007007A" in str(e)
        (log.info if too_small else log.warning)("xon %s: %s", path, e)
        return {"hr": E_BUFFER if too_small else E_FAIL, "path": path}
    hr = hr or S_OK
    log.info("xon %s (0x%04X): %d bytes -> %08X", path, ordinal or 0, len(native), hr & 0xFFFFFFFF)
    return {"hr": hr, "path": path, "native": native, "used": len(native)}


# ---- msgserver ----
@handles("/msgserver/vetstring2.ashx")
def vet_strings(ctx, req):
    """Text vetting (the console asks LIVE whether a motto, a message ... is acceptable): every string is fine
    here (S_OK each). Request f5 = the strings."""
    strings = (req or {}).get("f5") or []
    return {"f1": [S_OK] * len(strings)}


# ---- xuacs (accounts) ----
@handles("/xuacs/GetUserBillingNotifications.ashx")
def billing_notifications(ctx, req):
    """The account's billing notices (expiring card, failed renewal ...), asked at sign-in: there is no billing on
    our Xbox LIVE, so none (f1 = the list)."""
    return {"f1": []}
