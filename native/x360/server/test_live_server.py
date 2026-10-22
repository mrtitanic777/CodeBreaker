"""End-to-end test of live_server.py: starts the server on a test port with a temporary data file and drives every
operation and event over the real TCP (requests / replies / events) and UDP (party voice) protocol, as a console's
client does (network_protocol.md). No dependencies; run it from any directory.

    python -I test_live_server.py [-v]

Each test gets its own fresh server (its own port and empty data file), so tests do not share accounts, friendships
or parties. Exits 0 when every check passes, 1 otherwise (the first failure of each test is printed, with the server's
log). It is the server's regression test: run it after any change to live_server.py.
"""
import json
import os
import queue
import socket
import struct
import subprocess
import sys
import tempfile
import traceback
import threading
import time

HOST = "127.0.0.1"
VOICE_MAGIC = b"XV01"
SERVER = os.path.join(os.path.dirname(os.path.abspath(__file__)), "live_server.py")
# A 1x1 PNG served as the test's content file.
TEST_PNG = bytes.fromhex("89504e470d0a1a0a0000000d4948445200000001000000010806000000"
                         "1f15c4890000000d49444154789c6360f8cfc0f01f0005fe02fea7d6a4e90000000049454e44ae426082")


class Client:
    """One connected console: a TCP line protocol with request/reply by id and an event queue, plus a UDP voice
    socket. Mirrors native/x360/system/live/live/client.cpp."""

    _ids = iter(range(1, 1 << 30))

    def __init__(self, port, client="Xbox 360 Dashboard", title_id="FFFE07D1"):
        self.sock = socket.create_connection((HOST, port))
        self.sock.settimeout(5)
        self.buf = b""
        self.replies = {}
        self.reply_ready = threading.Condition()
        self.events = queue.Queue()
        self.closed = False
        self.voice = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.voice_port = port
        self.lock = threading.Lock()
        threading.Thread(target=self._read, daemon=True).start()
        self.hello = self.request("hello", version=1, client=client, title_id=title_id,
                                  console="TESTCONSOLE%04X" % (id(self) & 0xFFFF))

    def _read(self):
        try:
            while True:
                while b"\n" not in self.buf:
                    chunk = self.sock.recv(1 << 16)
                    if not chunk:
                        return
                    self.buf += chunk
                line, self.buf = self.buf.split(b"\n", 1)
                if not line.strip():
                    continue
                obj = json.loads(line)
                if obj.get("op") == "reply":
                    with self.reply_ready:
                        self.replies[obj["id"]] = obj
                        self.reply_ready.notify_all()
                else:
                    self.events.put(obj)
        except (OSError, ValueError):
            pass
        finally:
            self.closed = True
            with self.reply_ready:
                self.reply_ready.notify_all()

    def _send(self, op, fields):
        line = json.dumps(dict(fields, op=op), separators=(",", ":")) + "\n"
        with self.lock:
            self.sock.sendall(line.encode("utf-8"))

    def _await(self, mid, op):
        deadline = time.time() + 5
        with self.reply_ready:
            while mid not in self.replies:
                if self.closed or not self.reply_ready.wait(max(0.0, deadline - time.time())):
                    raise AssertionError("no reply to %s" % op)
            return self.replies.pop(mid)

    def request(self, op, **fields):
        mid = next(self._ids)
        self._send(op, dict(fields, id=mid))
        reply = self._await(mid, op)
        if not reply.get("ok"):
            raise AssertionError("%s failed: %s" % (op, reply.get("error")))
        return reply

    def fails(self, op, **fields):
        """A request expected to be refused; returns the error text."""
        mid = next(self._ids)
        self._send(op, dict(fields, id=mid))
        reply = self._await(mid, op)
        assert not reply.get("ok"), "%s was expected to fail but did not" % op
        return reply.get("error", "")

    def notify(self, op, **fields):
        """A fire-and-forget request (no reply; `id` here is a payload field such as a message id, as the C++ client
        sends message_read / message_delete)."""
        self._send(op, fields)

    def event(self, op=None, timeout=3):
        deadline = time.time() + timeout
        while True:
            try:
                e = self.events.get(timeout=max(0.01, deadline - time.time()))
            except queue.Empty:
                raise AssertionError("no %s event" % (op or "any"))
            if op is None or e.get("op") == op:
                return e

    def drain(self):
        while not self.events.empty():
            self.events.get_nowait()

    def voice_send(self, token_hex, seq, payload):
        self.voice.sendto(VOICE_MAGIC + bytes.fromhex(token_hex) + struct.pack(">I", seq) + payload,
                          (HOST, self.voice_port))

    def voice_recv(self, timeout=2):
        self.voice.settimeout(timeout)
        data, _ = self.voice.recvfrom(1 << 16)
        assert data[:4] == VOICE_MAGIC
        return struct.unpack(">Q", data[4:12])[0], struct.unpack(">I", data[12:16])[0], data[16:]

    def close(self):
        try:
            self.sock.close()
        except OSError:
            pass
        self.voice.close()


TESTS = []


def test(fn):
    TESTS.append(fn)
    return fn


def xuid(n):
    return "000900000000%04X" % n


def signin(c, user, n, tag, **extra):
    r = c.request("signin", user=user, xuid=xuid(n), gamertag=tag, **extra)
    assert r["account"]["xuid"] == xuid(n) and r["account"]["gamertag"] == tag
    return r


def befriend(ctx, a, ua, na, b, ub, nb):
    """Makes the two signed-in users friends (a asks, b accepts) and drains the resulting events."""
    a.request("friend_add", user=ua, gamertag=b.tag[nb])
    b.event("friend")
    b.request("friend_accept", user=ub, xuid=xuid(na))
    a.drain(); b.drain()


@test
def test_register_and_signin(ctx):
    a = ctx.client()
    reg = a.request("register", gamertag="Registrar", key="key-A")
    x = reg["account"]["xuid"]
    assert x.startswith("0009") and len(x) == 16
    assert a.request("register", gamertag="Registrar", key="key-A")["account"]["xuid"] == x
    b = ctx.client()
    assert "taken" in b.fails("register", gamertag="Registrar", key="key-B")
    r = a.request("signin", user=0, xuid=x, gamertag="Registrar")
    assert len(r["voice_token"]) == 16


@test
def test_signin_fields(ctx):
    a = ctx.client()
    assert "bad" in a.fails("signin", user=9, xuid=xuid(1), gamertag="Tag")
    assert "bad" in a.fails("signin", user=0, xuid="SHORT", gamertag="Tag")
    signin(a, 0, 1, "Alice")
    b = ctx.client()
    assert "taken" in b.fails("signin", user=0, xuid=xuid(2), gamertag="Alice")


@test
def test_profile_roundtrip(ctx):
    a = ctx.client()
    signin(a, 0, 1, "Alice", profile={"gamerscore": 1495, "motto": "hi", "name": "Al", "reputation": 80})
    b = ctx.client(); signin(b, 0, 2, "Bob")
    p = b.request("profile", user=0, xuid=xuid(1))
    assert p["account"]["gamertag"] == "Alice"
    assert p["profile"]["gamerscore"] == 1495 and p["profile"]["motto"] == "hi"
    a.request("profile_update", user=0, profile={"motto": "x" * 50})
    assert len(b.request("profile", user=0, xuid=xuid(1))["profile"]["motto"]) == 21


@test
def test_friends_flow(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    f = a.request("friend_add", user=0, gamertag="Bob")["friend"]
    assert f["xuid"] == xuid(2) and f["request"] == "outgoing"
    e = b.event("friend")
    assert e["friend"]["xuid"] == xuid(1) and e["friend"]["request"] == "incoming"
    b.request("friend_accept", user=0, xuid=xuid(1))
    fa = {x["xuid"]: x for x in a.request("friends", user=0)["friends"]}
    assert fa[xuid(2)]["request"] == "" and fa[xuid(2)]["online"] is True
    a.request("friend_remove", user=0, xuid=xuid(2))
    assert b.event("friend_removed")["xuid"] == xuid(1)
    assert a.request("friends", user=0)["friends"] == []


@test
def test_friends_mutual_add(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.request("friend_add", user=0, gamertag="Bob"); b.drain()
    f = b.request("friend_add", user=0, gamertag="Alice")["friend"]
    assert f["request"] == ""
    fl = a.request("friends", user=0)["friends"]
    assert {x["xuid"] for x in fl} == {xuid(2)} and fl[0]["request"] == ""


@test
def test_friend_decline(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.request("friend_add", user=0, gamertag="Bob")
    b.event("friend")
    b.request("friend_decline", user=0, xuid=xuid(1))
    assert a.event("friend_removed")["xuid"] == xuid(2)
    assert a.request("friends", user=0)["friends"] == []


@test
def test_friends_of(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    c = ctx.client(); signin(c, 0, 3, "Carol")
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}; c.tag = {3: "Carol"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    befriend(ctx, b, 0, 2, c, 0, 3)
    # Alice views Bob's friends: she sees Carol.
    fo = {x["xuid"] for x in a.request("friends_of", user=0, xuid=xuid(2))["friends"]}
    assert xuid(3) in fo


@test
def test_messages(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.request("message_send", user=0, to=[xuid(2)], text="hello Bob")
    m = b.event("message")["message"]
    assert m["from"]["gamertag"] == "Alice" and m["text"] == "hello Bob" and m["read"] is False
    inbox = b.request("messages", user=0)["messages"]
    assert len(inbox) == 1 and inbox[0]["text"] == "hello Bob"
    b.notify("message_read", user=0, id=m["id"])
    time.sleep(0.2)
    assert b.request("messages", user=0)["messages"][0]["read"] is True
    b.notify("message_delete", user=0, id=m["id"])
    time.sleep(0.2)
    assert b.request("messages", user=0)["messages"] == []


@test
def test_parties(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    p = a.request("party_create", user=0)["party"]
    pid = p["id"]
    assert p["leader"] == xuid(1) and [m["xuid"] for m in p["members"]] == [xuid(1)]
    # Alice invites Bob, then Bob joins by id.
    a.request("party_invite", user=0, xuid=xuid(2))
    b.event("message")  # the party invite arrives as a message
    jb = b.request("party_join", user=0, party=pid)["party"]
    assert {m["xuid"] for m in jb["members"]} == {xuid(1), xuid(2)}
    assert {m["xuid"] for m in a.event("party")["party"]["members"]} == {xuid(1), xuid(2)}
    assert "leader" in b.fails("party_settings", user=0, invite_only=True)
    a.request("party_settings", user=0, invite_only=True)
    time.sleep(0.2); b.drain()  # Bob's earlier party events (his join, the settings change)
    a.request("party_kick", user=0, xuid=xuid(2))
    assert b.event("party")["party"] is None  # the kicked member is told the party is gone
    a.request("party_leave", user=0)
    assert a.request("party", user=0)["party"] is None


@test
def test_party_join_by_friend(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    a.request("party_create", user=0)
    j = b.request("party_join", user=0, friend=xuid(1))["party"]
    assert {m["xuid"] for m in j["members"]} == {xuid(1), xuid(2)}


@test
def test_party_carryover(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    a.request("party_create", user=0)
    b.request("party_join", user=0, friend=xuid(1))
    a.drain()
    # Bob's console drops (starts a game): the connection closes without signing out.
    b.close()
    time.sleep(0.3)
    members = {m["xuid"] for m in a.request("party", user=0)["party"]["members"]}
    assert xuid(2) in members, "a dropped member keeps their seat"
    # Bob signs in again (from the game): back in the same party.
    b2 = ctx.client(client="Minecraft: Xbox 360 Edition", title_id="584109C2")
    signin(b2, 0, 2, "Bob")
    assert b2.request("party", user=0)["party"] is not None, "signing in again restores the party seat"


@test
def test_signed_in_elsewhere(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client()
    signin(b, 0, 1, "Alice")  # same XUID, another console
    e = a.event("signed_out")
    assert e["reason"] and e["user"] == 0


@test
def test_status_offline(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    a.request("status", user=0, status="offline")
    deadline = time.time() + 3
    fb = {}
    while time.time() < deadline:
        fb = {x["xuid"]: x for x in b.request("friends", user=0)["friends"]}
        if fb[xuid(1)]["online"] is False:
            break
        time.sleep(0.1)
    assert fb[xuid(1)]["online"] is False


@test
def test_recent_players(ctx):
    a = ctx.client(client="Minecraft: Xbox 360 Edition", title_id="584109C2"); signin(a, 0, 1, "Alice")
    b = ctx.client(client="Minecraft: Xbox 360 Edition", title_id="584109C2"); signin(b, 0, 2, "Bob")
    time.sleep(0.3)
    ra = {r["xuid"]: r for r in a.request("recent_players", user=0)["players"]}
    assert xuid(2) in ra and ra[xuid(2)]["gamertag"] == "Bob" and ra[xuid(2)]["title_id"] == "584109C2"


@test
def test_titles_and_achievements(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.request("titles_update", user=0, titles=[
        {"id": "584109C2", "name": "Minecraft", "achievements": 100, "earned": 42, "gamerscore": 420}])
    ts = b.request("titles_of", user=0, xuid=xuid(1))["titles"]
    assert ts and ts[0]["name"] == "Minecraft" and ts[0]["earned"] == 42
    a.request("title_achievements_update", user=0, title_id="584109C2", achievements=[
        {"name": "Getting Wood", "description": "Punch a tree", "gamerscore": 20, "earned": True, "shown": True}])
    ac = b.request("title_achievements_of", user=0, xuid=xuid(1), title_id="584109C2")["achievements"]
    assert ac and ac[0]["name"] == "Getting Wood" and ac[0]["earned"] is True


@test
def test_feedback(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")  # feedback is about a known player
    assert "yourself" in a.fails("feedback", user=0, xuid=xuid(1), kind="prefer", category="", reason="")
    assert "no such" in a.fails("feedback", user=0, xuid=xuid(9), kind="prefer", category="", reason="")
    a.request("feedback", user=0, xuid=xuid(2), kind="prefer", category="good", reason="great teammate")


@test
def test_voice_relay(ctx):
    a = ctx.client(); ta = signin(a, 0, 1, "Alice")["voice_token"]
    b = ctx.client(); tb = signin(b, 0, 2, "Bob")["voice_token"]
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    a.request("party_create", user=0)
    b.request("party_join", user=0, friend=xuid(1))
    a.drain()
    a.voice_send(ta, 0, b"")  # keep-alives tell the server each member's UDP address
    b.voice_send(tb, 0, b"")
    time.sleep(0.2)
    frame = bytes(range(164))
    a.voice_send(ta, 1, frame)
    sender, seq, payload = b.voice_recv()
    assert sender == int(xuid(1), 16) and seq == 1 and payload == frame


@test
def test_tenure(ctx):
    a = ctx.client()
    reg = a.request("register", gamertag="Senior", key="k")
    x = reg["account"]["xuid"]
    assert reg["account"]["created"] > 0 and reg["account"]["tenure"] == 0
    a.request("signin", user=0, xuid=x, gamertag="Senior")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    acc = b.request("profile", user=0, xuid=x)["account"]
    assert acc["created"] == reg["account"]["created"] and acc["tenure"] == 0


@test
def test_beacons(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    assert "game" in a.fails("beacon_set", user=0, title_id="FFFE07D1", title="Dash", text="x")
    a.request("beacon_set", user=0, title_id="584109C2", title="Minecraft", text="building, join me")
    # Bob sees Alice's beacon in her friend entry (the friend event) and in his beacons list.
    e = b.event("friend")
    assert e["friend"]["beacon"]["title_id"] == "584109C2"
    beacons = b.request("beacons", user=0)["beacons"]
    assert len(beacons) == 1 and beacons[0]["xuid"] == xuid(1) and beacons[0]["text"] == "building, join me"
    # Clearing it removes the beacon.
    a.request("beacon_clear", user=0)
    assert b.event("friend")["friend"]["beacon"] is None
    assert b.request("beacons", user=0)["beacons"] == []


@test
def test_caps(ctx):
    a = ctx.client(); signin(a, 0, 1, "Alice")
    b = ctx.client(); signin(b, 0, 2, "Bob")
    assert "long" in a.fails("message_send", user=0, to=[xuid(2)], text="x" * 300)
    assert "recipient" in a.fails("message_send", user=0, to=[], text="hi")
    # A beacon note is cut to the console's limit.
    a.tag = {1: "Alice"}; b.tag = {2: "Bob"}
    befriend(ctx, a, 0, 1, b, 0, 2)
    r = a.request("beacon_set", user=0, title_id="584109C2", title="M", text="y" * 500)
    assert len(r["beacon"]["text"]) == 168


@test
def test_content(ctx):
    import base64 as b64, hashlib
    a = ctx.client()
    man = a.request("content_manifest")["content"]
    assert "tenure" in man and "crown10.png" in man["tenure"], "the tenure content package is served"
    entry = man["tenure"]["crown10.png"]
    got = a.request("content_get", package="tenure", file="crown10.png")
    data = b64.b64decode(got["data"])
    assert data[:8] == b"\x89PNG\r\n\x1a\n", "content is the real PNG"
    assert hashlib.sha1(data).hexdigest() == got["hash"] == entry["hash"], "hash matches the manifest"
    assert len(data) == entry["size"]
    # Path traversal and unknown content are refused.
    assert "content" in a.fails("content_get", package="tenure", file="../../live_server.py")
    assert "content" in a.fails("content_get", package="nope", file="x.png")


@test
def test_web_services(ctx):
    import base64 as b64
    a = ctx.client(); signin(a, 0, 1, "Alice")
    hive = a.request("hello", client="test")["livehive"]
    assert hive["SearchKillSwitch"] == "false" and hive["EDSUriRoot"].startswith("http://live.local")

    def web(method, url, body=b"", user=0):
        r = a.request("web", user=user, method=method, url=url, headers={}, body=b64.b64encode(body).decode())
        return r["status"], b64.b64decode(r["body"])

    # The search catalog is the players' played titles.
    a.request("titles_update", user=0, titles=[{"id": "584111F7", "name": "Minecraft", "achievements": 0,
                                                 "earned": 0, "gamerscore": 0}])
    st, body = web("GET", "http://epix.localhost/en-US/searchsettings.xml")
    assert st == 200 and b"<name>GamesEnabled</name><value>TRUE</value>" in body
    st, body = web("GET", "http://live.local/eds/media/en-US/crossMediaGroupSearch?q=mine&maxItems=10")
    assert st == 200 and b"<Name>Minecraft</Name>" in body and b"<Count>1</Count>" in body, body
    st, body = web("GET", "http://live.local/eds/media/en-US/crossMediaGroupSearch?q=zzz")
    assert st == 200 and b"<Items></Items>" in body and b"<Totals></Totals>" in body
    st, body = web("GET", "http://live.local/search/qsonhs.aspx?q=min&ds=bbxcomplete")
    assert st == 200 and json.loads(body)["AS"]["Results"][0]["Suggests"][0]["Txt"] == "Minecraft"
    # My Pins: add, read back, delete; only the user's lists.
    me = "http://live.local/lists/users/xuid(%d)/lists/PINS/XBLPins" % int(xuid(1), 16)
    st, _ = web("POST", me, json.dumps({"Items": [{"ItemId": "584111F7", "ContentType": "Game"}]}).encode())
    assert st == 200
    st, body = web("GET", me + "?skipItems=0&maxItems=25")
    assert st == 200 and json.loads(body)["ListItems"][0]["ItemId"] == "584111F7"
    st, _ = web("GET", "http://live.local/lists/users/xuid(%d)/lists/PINS/XBLPins" % int(xuid(2), 16))
    assert st == 403
    st, body = web("DELETE", me)
    assert st == 200 and json.loads(web("GET", me)[1])["ListItems"] == []
    assert web("GET", "http://live.local/nope")[0] == 404


@test
def test_web_service_calls(ctx):
    import base64 as b64
    import struct
    a = ctx.client(); signin(a, 0, 1, "Alice")
    a.request("titles_update", user=0, titles=[{"id": "584111F7", "name": "Minecraft", "achievements": 0,
                                                 "earned": 0, "gamerscore": 0}])
    me = int(xuid(1), 16)
    # My Pins as PinService.lua calls it: GET with headers, xuid / wstr URL tokens and an opt'ed query field.
    req_schema = ("x-xbl-contract-version{t:uint,offset:0,size:4,loc:hdr},Accept{t:str,offset:4,size:64,loc:hdr,max:64},"
                  "xuid{t:xuid,offset:68,size:8,loc:url},listType{t:wstr,offset:76,size:200,loc:url,max:100},"
                  "listName{t:wstr,offset:276,size:200,loc:url,max:100},maxItems{t:uint,offset:488,size:4,loc:qs,opt:484}")
    resp_schema = ("body{t:item,offset:0,size:600,loc:body}[?{t:item,offset:0,size:600,loc:dom}[ListMetadata{t:item,"
                   "offset:0,size:12,loc:dom}[ListVersion{t:uint,offset:0,size:4,loc:dom},ListCount{t:uint,offset:4,"
                   "size:4,loc:dom}],ListItems{t:item,offset:16,size:580,loc:dom,max:5,itemsize:116,countoffset:12,"
                   "countsize:4}[Item{t:item,offset:0,size:116,loc:dom}[ItemId{t:wstr,offset:0,size:40,loc:dom,max:20},"
                   "ContentType{t:wstr,offset:40,size:40,loc:dom,max:20}]]]]")
    req = bytearray(492)
    req[0:4] = struct.pack(">I", 2); req[4:21] = b"application/json\0"; req[68:76] = struct.pack(">Q", me)
    req[76:86] = "PINS\0".encode("utf-16-be"); req[276:292] = "XBLPins\0".encode("utf-16-be")
    req[484:488] = struct.pack(">I", 1); req[488:492] = struct.pack(">I", 25)
    url = "{LiveHive:ListServiceUriRoot}/users/xuid({xuid})/lists/{listType}/{listName}"
    a.request("web", user=0, method="POST", url="http://live.local/lists/users/xuid(%d)/lists/PINS/XBLPins" % me,
              headers={}, body=b64.b64encode(json.dumps({"Items": [{"Item": {"ItemId": "584111F7",
                                                                           "ContentType": "Game"}}]}).encode()).decode())
    r = a.request("ws_call", user=0, url=url, method="GET", flags=2, req_schema=req_schema,
                  req=b64.b64encode(bytes(req)).decode(), resp_schema=resp_schema, resp_size=600)
    out = b64.b64decode(r["resp"]).ljust(600, b"\0")
    assert r["status"] == 200 and r["hr"] == 0, r
    assert struct.unpack(">III", out[0:12])[1] == 1 and struct.unpack(">I", out[12:16])[0] == 1
    assert out[16:32].decode("utf-16-be") == "584111F7" and out[56:64].decode("utf-16-be") == "Game"
    # Search as Dash.Search does it: BeginRequest, BeginResponse (a stream: its length), ContinueResponse reads.
    sreq_schema = "q{t:wstr,offset:0,size:40,loc:qs,max:20},locale{t:str,offset:40,size:7,loc:url,max:7}"
    sreq = bytearray(47)
    sreq[0:10] = "mine\0".encode("utf-16-be"); sreq[40:46] = b"en-US\0"
    h = a.request("ws_begin", user=-1, url="{LiveHive:SearchDataAnonymousURL}/{locale}/crossMediaGroupSearch",
                  method="GET", flags=0x24, req_schema=sreq_schema, req=b64.b64encode(bytes(sreq)).decode())["handle"]
    r = a.request("ws_response", handle=h, resp_schema="body{t:stream,offset:0,size:4,loc:body,enc:xml}", resp_size=4,
                  flags=0x24)
    length = struct.unpack(">I", b64.b64decode(r["resp"]).ljust(4, b"\0"))[0]
    assert r["status"] == 200 and length == r["length"] > 0
    body = b64.b64decode(a.request("ws_read", handle=h, offset=0, size=length)["data"])
    assert b"<Name>Minecraft</Name>" in body
    a.request("ws_close", handle=h)
    assert "no such" in a.fails("ws_read", handle=h, offset=0, size=1)


# The Xbox LIVE service schema is Microsoft's data: this test runs only where the user's analysis has it.
XAM_IMAGE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "Xbox 360 Dashboard", "analysis",
                         "xam", "xam.image.bin")


@test
def test_xon(ctx):
    import base64 as b64
    if not os.path.exists(XAM_IMAGE):
        return
    with open(XAM_IMAGE, "rb") as f:
        f.seek(0x81A888C8 - 0x815F0000)
        blob = f.read(0x7B15)
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import xon_schema
    sch = xon_schema.Schema(blob)
    a = ctx.client(); signin(a, 0, 1, "Alice")
    assert "schema" in a.fails("xon", index=12, wire="")  # no schema yet
    assert a.request("xon_schema", blob=b64.b64encode(blob).decode())["requests"] == 304
    # vetstring2: two strings, each S_OK; the reply is the user buffer XAM would have filled.
    idx = sch.index_of(0x801)
    wire = sch.encode(idx, {"f0": 0, "f1": 0, "f4": b"en-US", "f5": [{"f1": "hi"}, {"f1": "there"}]}, response=False)
    r = a.request("xon", user=0, index=idx, wire=b64.b64encode(wire).decode(), user_base=0x40000000,
                  buf_size=256, default_count=0)
    native = b64.b64decode(r["native"])
    assert r["hr"] == 0 and r["path"] == "/msgserver/vetstring2.ashx", r
    assert native == bytes.fromhex("0002" "40000006" "00000000" "00000000"), native.hex()
    # A service with no handler yet: XONLINE_E_LOGON_SERVICE_NOT_REQUESTED, as before.
    r = a.request("xon", user=0, index=sch.index_of(0x703), wire="", user_base=0, buf_size=0, default_count=0)
    assert r["hr"] == 0x80151100 and "native" not in r


class Ctx:
    def __init__(self, port):
        self.port = port
        self.clients = []

    def client(self, **kw):
        c = Client(self.port, **kw)
        self.clients.append(c)
        return c

    def close(self):
        for c in self.clients:
            c.close()


def free_port():
    s = socket.socket()
    s.bind((HOST, 0))
    port = s.getsockname()[1]
    s.close()
    return port


def start_server(data_dir, log_path):
    port = free_port()
    log = open(log_path, "w", encoding="utf-8")
    # A content folder of its own (the real tenure images are the user's, imported and gitignored): one small PNG.
    content = os.path.join(data_dir, "content")
    os.makedirs(os.path.join(content, "tenure"), exist_ok=True)
    with open(os.path.join(content, "tenure", "crown10.png"), "wb") as f:
        f.write(TEST_PNG)
    proc = subprocess.Popen([sys.executable, "-I", SERVER, "--host", HOST, "--port", str(port),
                             "--data", os.path.join(data_dir, "data.json"),
                             "--logs", os.path.join(data_dir, "logs"), "--content", content,
                             "--search-provider", "none", "--xboxunity", "off"],
                            stdout=log, stderr=subprocess.STDOUT)
    deadline = time.time() + 10
    while time.time() < deadline:
        if proc.poll() is not None:
            raise RuntimeError("server exited early; see %s" % log_path)
        try:
            socket.create_connection((HOST, port), timeout=0.5).close()
            return proc, port, log
        except OSError:
            time.sleep(0.1)
    raise RuntimeError("server did not start")


def run():
    verbose = "-v" in sys.argv
    passed = failed = 0
    with tempfile.TemporaryDirectory(prefix="live_test_") as root:
        for i, fn in enumerate(TESTS):
            data_dir = os.path.join(root, "t%02d" % i)
            os.makedirs(data_dir)
            log_path = os.path.join(data_dir, "server.log")
            proc, port, log = start_server(data_dir, log_path)
            ctx = Ctx(port)
            try:
                fn(ctx)
                passed += 1
                if verbose:
                    print("ok   ", fn.__name__)
            except Exception as e:
                failed += 1
                print("FAIL ", fn.__name__, "-", e)
                if verbose:
                    traceback.print_exc()
            finally:
                ctx.close()
                proc.terminate()
                try:
                    proc.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    proc.kill()
                log.close()
                text = open(log_path, encoding="utf-8", errors="replace").read()
                if "Traceback" in text or "server error" in text or "Task exception" in text:
                    print("     server reported errors in %s:" % fn.__name__)
                    for line in text.splitlines():
                        if any(w in line for w in ("Traceback", "Error", "error", "  File")):
                            print("       " + line)
    print("\n%d passed, %d failed" % (passed, failed))
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(run())
