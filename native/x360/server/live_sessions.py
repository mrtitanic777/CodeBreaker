"""live_sessions.py -- the LIVE server's game sessions (network_protocol.md "Sessions") and the relay for game
traffic ("Game data"). A title's XSession* calls come here so friends can find and join games: sessions live in
memory and end when their host signs out or drops. A mixin of live_server.Server.
"""
import base64
import struct
import time

# XSESSION_CREATE_* (xonline.h)
USES_PRESENCE = 0x2
USES_MATCHMAKING = 0x8
JOIN_VIA_PRESENCE_DISABLED = 0x200
JOIN_IN_PROGRESS_DISABLED = 0x400
JOIN_VIA_PRESENCE_FRIENDS_ONLY = 0x800
QOS_MAX = 1024
ATTRIBUTES_MAX = 64
GAME_MAGIC = b"XG01"


def _hex16(v):
    v = str(v or "").upper()
    if len(v) != 16 or any(c not in "0123456789ABCDEF" for c in v):
        raise ValueError("bad session id")
    return v


class SessionOps:
    # ---- views ----
    def session_view(self, sid, qos=False):
        g = self.games.get(sid)
        if g is None:
            return None
        out = {k: g[k] for k in ("id", "title_id", "host", "flags", "public_slots", "private_slots", "state",
                                 "created", "attributes")}
        out["members"] = [self.account(x) for x in g["members"]]
        if qos:
            out["qos"] = g.get("qos", "")
        return out

    def _open_slots(self, g):
        return max(0, g["public_slots"] + g["private_slots"] - len(g["members"]))

    def presence_session(self, owner, xuid):
        """The game session `xuid` shows in presence, for `owner`'s friend entry: (session id, host, joinable)."""
        for g in self.games.values():
            if xuid in g["members"] and g["flags"] & USES_PRESENCE:
                f = g["flags"]
                joinable = not f & JOIN_VIA_PRESENCE_DISABLED and \
                    not (g["state"] == "playing" and f & JOIN_IN_PROGRESS_DISABLED) and \
                    not (f & JOIN_VIA_PRESENCE_FRIENDS_ONLY and owner not in self.store.friends.get(g["host"], set())) \
                    and self._open_slots(g) > 0 and owner not in g["members"]
                return g["id"], g["host"], joinable
        return "", "", False

    def _game(self, sid, s=None, host=False):
        g = self.games.get(_hex16(sid))
        if g is None:
            raise ValueError("no such session")
        if host and s is not None and g["host"] != s.xuid:
            raise ValueError("not the session's host")
        return g

    def _changed(self, g):
        for x in g["members"]:
            self.tell_friends(x)

    # ---- requests ----
    def op_session_create(self, conn, msg):
        s = self.session(conn, msg)
        sid = _hex16(msg.get("session_id"))
        if sid in self.games:
            raise ValueError("session exists")
        g = {"id": sid, "title_id": s.title_id, "host": s.xuid, "flags": int(msg.get("flags", 0)) & 0xFFFFFFFF,
             "public_slots": max(0, int(msg.get("public_slots", 0))),
             "private_slots": max(0, int(msg.get("private_slots", 0))),
             "members": [s.xuid], "state": "lobby", "created": int(time.time()), "attributes": {}}
        self.games[sid] = g
        self._changed(g)
        return {"session": self.session_view(sid)}

    def op_session_delete(self, conn, msg):
        """The host ends the session; a member leaves it."""
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"))
        if g["host"] == s.xuid:
            self.end_game(g["id"])
        elif s.xuid in g["members"]:
            g["members"].remove(s.xuid)
            self._changed(g)
            self.tell_friends(s.xuid)

    def end_game(self, sid):
        g = self.games.pop(sid, None)
        if g is not None:
            for x in g["members"]:
                self.tell_friends(x)

    def op_session_join(self, conn, msg):
        """The host adds anyone; others only themselves."""
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"))
        xuids = [str(x).upper() for x in msg.get("xuids", [s.xuid])]
        if g["host"] != s.xuid and any(x != s.xuid for x in xuids):
            raise ValueError("only the host adds others")
        for x in xuids:
            if x not in g["members"]:
                if self._open_slots(g) == 0:
                    raise ValueError("session full")
                g["members"].append(x)
        self._changed(g)
        return {"session": self.session_view(g["id"])}

    def op_session_leave(self, conn, msg):
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"))
        xuids = [str(x).upper() for x in msg.get("xuids", [s.xuid])]
        if g["host"] != s.xuid and any(x != s.xuid for x in xuids):
            raise ValueError("only the host removes others")
        for x in xuids:
            if x in g["members"] and x != g["host"]:
                g["members"].remove(x)
                self.tell_friends(x)
        self._changed(g)

    def op_session_modify(self, conn, msg):
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"), s, host=True)
        for key in ("public_slots", "private_slots"):
            if key in msg:
                g[key] = max(0, int(msg[key]))
        if "flags" in msg:
            g["flags"] = int(msg["flags"]) & 0xFFFFFFFF
        self._changed(g)
        return {"session": self.session_view(g["id"])}

    def op_session_state(self, conn, msg):
        """XSessionStart ("playing") / XSessionEnd ("lobby"), from the host."""
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"), s, host=True)
        state = str(msg.get("state", ""))
        if state not in ("playing", "lobby"):
            raise ValueError("bad state")
        g["state"] = state
        self._changed(g)

    def op_session_qos(self, conn, msg):
        """The host's XNetQosListen data, which joiners read with `session`."""
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"), s, host=True)
        qos = str(msg.get("qos", ""))
        if len(base64.b64decode(qos, validate=True)) > QOS_MAX:
            raise ValueError("qos too large")
        g["qos"] = qos

    def op_session(self, conn, msg):
        self.session(conn, msg)
        sid = _hex16(msg.get("session_id"))
        return {"session": self.session_view(sid, qos=True)}

    def op_session_attributes(self, conn, msg):
        """The host's matchmaking contexts and properties: {"<id, 8 hex digits>": base64}."""
        s = self.session(conn, msg)
        g = self._game(msg.get("session_id"), s, host=True)
        attributes = msg.get("attributes")
        if not isinstance(attributes, dict) or len(attributes) > ATTRIBUTES_MAX:
            raise ValueError("bad attributes")
        g["attributes"].update({str(k).upper()[:8]: str(v) for k, v in attributes.items()})

    def op_session_search(self, conn, msg):
        """XSessionSearchEx: the caller's title's matchmaking sessions with `players` open public slots, joinable now,
        not the caller's own, whose attributes equal every filter; newest first."""
        s = self.session(conn, msg)
        filters = {str(k).upper(): str(v) for k, v in (msg.get("filters") or {}).items()}
        players = max(1, int(msg.get("players", 1)))
        limit = max(1, min(50, int(msg.get("max_results", 10))))
        found = []
        for g in self.games.values():
            if g["title_id"] != s.title_id or not g["flags"] & USES_MATCHMAKING or s.xuid in g["members"]:
                continue
            if g["state"] == "playing" and g["flags"] & JOIN_IN_PROGRESS_DISABLED:
                continue
            open_public = g["public_slots"] - max(0, len(g["members"]) - g["private_slots"])
            if open_public < players:
                continue
            if any(g["attributes"].get(k) != v for k, v in filters.items()):
                continue
            found.append(g)
        found.sort(key=lambda g: g["created"], reverse=True)
        return {"sessions": [self.session_view(g["id"], qos=True) for g in found[:limit]]}

    def sessions_signed_out(self, xuid):
        """`xuid` signed out or dropped: the sessions they host end, and they leave the others."""
        for sid, g in list(self.games.items()):
            if g["host"] == xuid:
                self.end_game(sid)
            elif xuid in g["members"]:
                g["members"].remove(xuid)
                self._changed(g)

    # ---- game data (UDP) ----
    def game_datagram(self, transport, data, addr):
        """client -> server: magic, token (8), destination handle (u32: 10.x.y.z from the destination XUID's low 24
        bits), destination port (u16), source port (u16), payload; destination 0 with no payload is a keep-alive.
        server -> client: magic, destination XUID (8), source XUID (8), source port, destination port, payload."""
        if len(data) < 20:
            return
        s = self.voice_tokens.get(data[4:12])
        if s is None:
            return
        s.game_addr = addr
        handle, dport, sport = struct.unpack_from(">IHH", data, 12)
        payload = data[20:]
        if handle == 0:
            return
        low = handle & 0xFFFFFF
        candidates = [o for o in self.sessions.values()
                      if int(o.xuid, 16) & 0xFFFFFF == low and o is not s and getattr(o, "game_addr", None)]
        if not candidates:
            return
        candidates.sort(key=lambda o: o.title_id != s.title_id)          # one playing the sender's title first
        dest = candidates[0]
        out = GAME_MAGIC + struct.pack(">QQHH", int(dest.xuid, 16), int(s.xuid, 16), sport, dport) + payload
        transport.sendto(out, dest.game_addr)
