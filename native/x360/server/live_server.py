"""Test server for the ports' own Xbox LIVE replacement (docs/x360/network_protocol.md).

    python -I live_server.py [--host 127.0.0.1] [--port 36074] [--data live_server_data.json]

Presence, friends, messages and parties for the profiles that sign in. Accounts, friendships and messages are kept
in the data file (JSON) between runs; who is online, titles and parties live in memory.
"""
import argparse
import asyncio
import base64
import hashlib
import itertools
import json
import logging
import os
import secrets
import socket
import struct
import sys
import time

# Our own modules beside this script (python -I leaves the script's folder off sys.path).
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import web_services  # noqa: E402
import xboxunity  # noqa: E402
import xon_services  # noqa: E402
import xws_engine  # noqa: E402

PROTOCOL_VERSION = 1
log = logging.getLogger("live")



# A public profile's keys and the longest text each may hold (the console's limits: motto 21 characters, name 129,
# location 40, bio 499; the pictures are base64 PNGs). reputation: XPROFILE_GAMERCARD_REP, 0-100 (the stars x 20).
PROFILE_KEYS = ("gamerscore", "motto", "zone", "name", "location", "bio", "reputation")
PROFILE_TEXT_LIMITS = {"motto": 21, "name": 129, "location": 40, "bio": 499, "picture": 200000, "picture_small": 50000}


def clean_profile(profile, keys):
    """The allowed keys of a client's profile, numbers as numbers and texts cut to their limits."""
    out = {}
    for k in keys:
        if k not in profile:
            continue
        v = profile[k]
        if k in ("gamerscore", "zone"):
            out[k] = int(v) if isinstance(v, (int, float)) else 0
        elif k == "reputation":
            out[k] = min(max(float(v), 0.0), 100.0) if isinstance(v, (int, float)) else 0.0
        else:
            out[k] = str(v)[:PROFILE_TEXT_LIMITS[k]]
    return out


class Store:
    """The persistent part: accounts {xuid: gamertag}, friendships, pending requests, messages."""

    def __init__(self, path):
        self.path = path
        self.accounts = {}       # xuid -> gamertag
        self.friends = {}        # xuid -> set of xuid (accepted, both directions)
        self.requests = {}       # xuid -> set of xuid they asked to be friends with
        self.messages = {}       # xuid -> list of message dicts (newest last)
        self.last_seen = {}      # xuid -> time
        self.profiles = {}       # xuid -> public profile {"gamerscore", "motto", "zone", "name", "location", "bio",
        #                          "picture" (base64 PNG), "picture_small"}
        self.livehive = {}       # LiveHive: Xbox LIVE's configuration strings by key (sent to every console)
        self.registered = {}     # xuid -> console key, for XUIDs this server handed out (op_register)
        # The Guide's Players tab: xuid -> [{"xuid", "title_id", "title", "met"}], the players met (same party, or
        # the same game at the same time), newest first, at most RECENT_MAX (Guide.Friends.xex's 0x32).
        self.recent = {}
        # Compare Games: xuid -> the titles their profile has played, most recent first (op_titles_update): [{"id",
        # "name", "achievements", "earned", "gamerscore", "tile" (base64 PNG)}], public like the profile.
        self.titles = {}
        # My Pins and the other web-service lists (web_services.py): xuid -> {"<type>/<name>": [item]}.
        self.lists = {}
        # Player reviews and complaints (op_feedback), newest last: [{"from", "about", "kind", "category", "reason",
        # "time"}]. Kept for whoever runs the server to read; nothing acts on them.
        self.feedback = []
        # Preferences > Online Status: xuid -> "away", "busy" or "offline" (Appear Offline); absent: Online.
        self.status = {}
        # When each account was first seen (register or first sign-in), Unix seconds: the gamercard's tenure
        # ("member since" / years on Xbox LIVE, XPROFILE_GAMERCARD_TENURE).
        self.created = {}
        # The Guide's Beacons (XamShowBeaconsUI, "players wanted"): xuid -> {"title_id", "title", "text", "at"}, the
        # game a player wants company in and their note. Friends see it; cleared by the player or when they leave.
        self.beacons = {}
        self.next_message = 1
        if os.path.exists(path):
            with open(path, encoding="utf-8") as f:
                d = json.load(f)
            self.accounts = d.get("accounts", {})
            self.friends = {k: set(v) for k, v in d.get("friends", {}).items()}
            self.requests = {k: set(v) for k, v in d.get("requests", {}).items()}
            self.messages = d.get("messages", {})
            self.last_seen = d.get("last_seen", {})
            self.profiles = d.get("profiles", {})
            self.livehive = d.get("livehive", {})
            self.registered = d.get("registered", {})
            self.recent = d.get("recent", {})
            self.titles = d.get("titles", {})
            self.lists = d.get("lists", {})
            self.feedback = d.get("feedback", [])
            self.status = d.get("status", {})
            self.created = d.get("created", {})
            self.beacons = d.get("beacons", {})
            self.next_message = d.get("next_message", 1)

    def save(self):
        d = {"accounts": self.accounts, "friends": {k: sorted(v) for k, v in self.friends.items()},
             "requests": {k: sorted(v) for k, v in self.requests.items()}, "messages": self.messages,
             "last_seen": self.last_seen, "next_message": self.next_message, "profiles": self.profiles,
             "livehive": self.livehive, "registered": self.registered, "recent": self.recent,
             "titles": self.titles, "lists": self.lists, "feedback": self.feedback, "status": self.status,
             "created": self.created, "beacons": self.beacons}
        tmp = self.path + ".tmp"
        with open(tmp, "w", encoding="utf-8") as f:
            json.dump(d, f, indent=1)
        os.replace(tmp, self.path)

    def xuid_of(self, gamertag):
        for xuid, tag in self.accounts.items():
            if tag.lower() == gamertag.lower():
                return xuid
        return None


class Session:
    """A signed-in user: one controller of one connected port."""

    def __init__(self, conn, user, xuid):
        self.conn, self.user, self.xuid = conn, user, xuid
        self.voice_token = secrets.token_bytes(8)  # identifies this session's UDP voice packets
        self.voice_addr = None                     # where its voice packets come from (and go to)
        self.title_id, self.title, self.rich = "FFFE07D1", "Xbox 360 Dashboard", ""
        self.party = None


class Server:
    def __init__(self, store, logs, content_root, search_provider="none", unity=None):
        self.store = store
        self.logs = logs         # live debugging: the folder for clients' log files (op_log)
        # The content subsystem (network_protocol.md "Content"): a folder of packages the dashboard downloads from us,
        # as a console downloaded content (tenure numbers, and whatever else we add) from Xbox LIVE. <root>/<package>/
        # <file>; served by op_content_manifest / op_content_get, cached by the client.
        self.content_root = content_root
        self.web = web_services.WebServices(store, search_provider, unity)
        self.sessions = {}       # xuid -> Session
        self.parties = {}        # id -> {"id", "leader", "members": [xuid], "open"}
        self.voice = None        # the UDP transport (VoiceProtocol)
        self.voice_tokens = {}   # token -> Session
        self.party_ids = itertools.count(1)
        # Party seats kept for members whose console dropped (a port starting a game or going back to the
        # dashboard is a new process): xuid -> (party id, expiry time). Signing in again reseats them.
        self.held_seats = {}
        # The same minute for everyone who drops without signing out: friends and party members keep seeing them
        # online in their last title, so a switch between ports looks as it does on a console (the player never
        # left). xuid -> {"until", "title_id", "title", "rich"}.
        self.away = {}

    # ---- views ----
    YEAR = 365.25 * 24 * 3600

    def account(self, xuid):
        created = self.store.created.get(xuid)
        return {"xuid": xuid, "gamertag": self.store.accounts.get(xuid, ""),
                "created": created or 0,
                "tenure": int((time.time() - created) / self.YEAR) if created else 0}

    def away_view(self, xuid):
        """A player switching ports (dropped less than a minute ago): their last presence, else None."""
        a = self.away.get(xuid)
        return a if a is not None and a["until"] > time.time() else None

    def friend_view(self, owner, xuid):
        s = self.sessions.get(xuid)
        request = ""
        if xuid in self.store.requests.get(owner, set()):
            request = "outgoing"
        elif owner in self.store.requests.get(xuid, set()):
            request = "incoming"
        away = self.away_view(xuid) if s is None else None
        status = self.store.status.get(xuid, "")
        online = (s is not None or away is not None) and request == "" and status != "offline"
        p = s if s is not None else None
        title_id = p.title_id if p else away["title_id"] if away else ""
        title = p.title if p else away["title"] if away else ""
        rich = p.rich if p else away["rich"] if away else ""
        in_party = bool(p.party) if p else xuid in self.held_seats
        return {"xuid": xuid, "gamertag": self.store.accounts.get(xuid, ""), "online": online,
                "title_id": title_id if online else "", "title": title if online else "",
                "rich": rich if online else "", "party": bool(online and in_party),
                "request": request, "last_seen": self.store.last_seen.get(xuid, 0),
                "status": status if online else "",
                "beacon": self.store.beacons.get(xuid) if online and request == "" else None}

    def friend_list(self, xuid):
        ids = set(self.store.friends.get(xuid, set())) | set(self.store.requests.get(xuid, set()))
        ids |= {x for x, asked in self.store.requests.items() if xuid in asked}
        return [self.friend_view(xuid, x) for x in sorted(ids, key=lambda x: self.store.accounts.get(x, "").lower())]

    def party_view(self, pid):
        p = self.parties.get(pid)
        if p is None:
            return None
        members = []
        for x in p["members"]:
            s = self.sessions.get(x)
            away = self.away_view(x) if s is None else None  # switching ports: still in their last title
            members.append({"xuid": x, "gamertag": self.store.accounts.get(x, ""),
                            "title_id": s.title_id if s else away["title_id"] if away else "",
                            "title": s.title if s else away["title"] if away else ""})
        return {"id": pid, "leader": p["leader"], "members": members, "open": p["open"],
                "invite_only": p.get("invite_only", False)}

    def recent_view(self, owner):
        """The Guide's Players tab: the players `owner` met, newest first, with where (the title) and when, and
        whether they are online now (the row's player_* status icon, the tab's "Players (%d Online)")."""
        out = []
        for r in self.store.recent.get(owner, []):
            x = r["xuid"]
            online = (x in self.sessions or self.away_view(x) is not None) and \
                self.store.status.get(x, "") != "offline"
            out.append({"xuid": x, "gamertag": self.store.accounts.get(x, ""), "title_id": r.get("title_id", ""),
                        "title": r.get("title", ""), "met": r.get("met", 0), "online": online})
        return out

    # ---- recent players ----
    RECENT_MAX = 50  # Guide.Friends.xex keeps 0x32

    def met(self, a, b, title_id, title, when):
        """`a` met `b` (b's title then): b moves to the top of a's recent players."""
        lst = [r for r in self.store.recent.get(a, []) if r["xuid"] != b]
        lst.insert(0, {"xuid": b, "title_id": title_id, "title": title, "met": when})
        self.store.recent[a] = lst[:self.RECENT_MAX]

    def together(self, s):
        """The sessions `s` is with now: the other members of its party, and everyone signed in to the same game
        (not the dashboard or another system title, FFFE....). Users of the same console are local players, never
        recent players."""
        out = {}
        p = self.parties.get(s.party) if s.party else None
        for x in p["members"] if p else []:
            o = self.sessions.get(x)
            if o is not None:
                out[x] = o
        if s.title_id and not s.title_id.startswith("FFFE"):
            for x, o in self.sessions.items():
                if o.title_id == s.title_id:
                    out[x] = o
        return [o for x, o in out.items() if x != s.xuid and o.conn is not s.conn]

    def meet(self, s):
        """`s` is (or was, until now) with other players: both sides remember the other, newest first."""
        others = self.together(s)
        if not others:
            return
        now = int(time.time())
        for o in others:
            self.met(s.xuid, o.xuid, o.title_id, o.title, now)
            self.met(o.xuid, s.xuid, s.title_id, s.title, now)
        self.store.save()

    # ---- events ----
    def send(self, xuid, event):
        s = self.sessions.get(xuid)
        if s:
            event = dict(event, user=s.user)
            s.conn.send(event)

    def tell_friends(self, xuid):
        """`xuid` changed (online state, title, party): every signed-in friend or requester gets its new entry."""
        watchers = set(self.store.friends.get(xuid, set()))
        watchers |= {x for x, asked in self.store.requests.items() if xuid in asked}
        watchers |= set(self.store.requests.get(xuid, set()))
        for w in watchers:
            self.send(w, {"op": "friend", "friend": self.friend_view(w, xuid)})

    def tell_party(self, pid):
        p = self.parties.get(pid)
        if p:
            view = self.party_view(pid)
            for x in p["members"]:
                self.send(x, {"op": "party", "party": view})

    MESSAGES_KEPT = 200  # inbox messages kept per account (the oldest dropped)

    def deliver(self, sender, to, text, kind="text", title_id=""):
        for x in to:
            if x not in self.store.accounts:
                continue
            m = {"id": self.store.next_message, "from": self.account(sender), "sent": int(time.time()),
                 "text": text, "read": False, "kind": kind, "title_id": title_id}
            self.store.next_message += 1
            inbox = self.store.messages.setdefault(x, [])
            inbox.append(m)
            del inbox[:-self.MESSAGES_KEPT]
            self.send(x, {"op": "message", "message": m})
        self.store.save()

    # ---- requests ----
    def session(self, conn, msg):
        s = conn.users.get(msg.get("user"))
        if s is None:
            raise ValueError("user not signed in")
        return s

    def handle(self, conn, msg):
        op = msg.get("op")
        handler = getattr(self, "op_" + str(op), None)
        if handler is None:
            raise ValueError("unknown op " + str(op))
        reply = handler(conn, msg)
        return reply if asyncio.iscoroutine(reply) else (reply or {})

    def op_hello(self, conn, msg):
        conn.client = msg.get("client", "")
        conn.title_id = str(msg.get("title_id", "FFFE07D1")).upper()
        log.info("%s: hello from %s", conn.peer, conn.client)
        # The web services' LiveHive roots (web_services.LIVEHIVE), under the data file's own values.
        return {"server": "live_server.py", "version": PROTOCOL_VERSION,
                "livehive": {**web_services.LIVEHIVE, **self.store.livehive}}

    def op_register(self, conn, msg):
        """A new Xbox LIVE account for a profile that has none (made on a PC, or offline on a console): a fresh online
        XUID (0009 + 48 random bits, unused here) for the gamertag. The console's key lets the same console get the
        same XUID back for that gamertag (a profile deleted and made again); anyone else gets "gamertag taken"."""
        tag, key = str(msg.get("gamertag", "")).strip(), str(msg.get("key", ""))
        if not 1 <= len(tag) <= 15:
            raise ValueError("bad gamertag")
        owner = self.store.xuid_of(tag)
        if owner is not None:
            if key and self.store.registered.get(owner) == key:
                log.info("%s registered again (%s)", tag, owner)
                return {"account": self.account(owner)}
            raise ValueError("gamertag taken")
        while True:
            xuid = "0009%012X" % secrets.randbits(48)
            if xuid not in self.store.accounts:
                break
        self.store.accounts[xuid] = tag
        self.store.registered[xuid] = key
        self.store.created.setdefault(xuid, int(time.time()))
        self.store.save()
        log.info("%s registered (%s)", tag, xuid)
        return {"account": self.account(xuid)}

    def op_signin(self, conn, msg):
        user, xuid, tag = int(msg["user"]), str(msg["xuid"]).upper(), str(msg["gamertag"])
        if not 0 <= user < 4 or len(xuid) != 16:
            raise ValueError("bad user or xuid")
        owner = self.store.xuid_of(tag)
        if owner is not None and owner != xuid:
            raise ValueError("gamertag taken")
        if user in conn.users:
            self.sign_out(conn, user)
        old = self.sessions.get(xuid)
        if old is not None:
            # As Xbox LIVE does: the account moves to this console, the other one is told (it goes offline and does
            # not sign in again by itself). Its party seat moves along (a game started from the dashboard signs in
            # before the dashboard has gone).
            if old.conn is not conn:
                log.info("%s: %s signed in on another console (%s): signed out here", old.conn.peer, tag, conn.peer)
                old.conn.send({"op": "signed_out", "user": old.user, "reason": "signed in on another console"})
            self.sign_out(old.conn, old.user, keep_seat=True)
        self.store.accounts[xuid] = tag
        self.store.created.setdefault(xuid, int(time.time()))
        # A profile with nothing in it (a game's runtime that has no gamercard to send) keeps the stored one.
        if isinstance(msg.get("profile"), dict):
            profile = clean_profile(msg["profile"], PROFILE_KEYS + ("picture", "picture_small"))
            if any(profile.values()) or xuid not in self.store.profiles:
                self.store.profiles[xuid] = profile
        self.store.save()
        s = Session(conn, user, xuid)
        if conn.client:
            s.title_id, s.title = conn.title_id, conn.client
        conn.users[user] = s
        self.sessions[xuid] = s
        self.voice_tokens[s.voice_token] = s
        log.info("%s signed in (%s, user %d)", tag, xuid, user)
        # Back within the minute after dropping: the same seat in the same party (the leader still leads: the seat
        # and the member order never changed).
        self.away.pop(xuid, None)
        held = self.held_seats.pop(xuid, None)
        if held is not None and held[0] in self.parties and xuid in self.parties[held[0]]["members"]:
            s.party = held[0]
            log.info("%s is back in party %s", tag, held[0])
            self.tell_party(held[0])
        self.tell_friends(xuid)
        self.meet(s)
        return {"account": self.account(xuid), "voice_token": s.voice_token.hex().upper()}

    SEAT_SECONDS = 60

    def clear_beacon(self, xuid):
        if self.store.beacons.pop(xuid, None) is not None:
            self.store.save()

    def sign_out(self, conn, user, keep_seat=False):
        """`keep_seat`: the console dropped or the account moved (not signing out on purpose): its party seat is
        kept for SEAT_SECONDS, the party sees the member without a title meanwhile."""
        s = conn.users.pop(user, None)
        if s is None:
            return
        self.meet(s)  # the players they were with: last met now
        if not keep_seat:
            self.clear_beacon(s.xuid)  # a deliberate sign-out takes the beacon down (a drop keeps it for the minute)
        if keep_seat:
            self.away[s.xuid] = {"until": time.time() + self.SEAT_SECONDS, "title_id": s.title_id, "title": s.title,
                                 "rich": s.rich}
            try:
                asyncio.get_running_loop().call_later(self.SEAT_SECONDS + 1, self.expire_away, s.xuid)
            except RuntimeError:
                pass
        if s.party and keep_seat and s.party in self.parties:
            pid = s.party
            self.held_seats[s.xuid] = (pid, time.time() + self.SEAT_SECONDS)
            s.party = None
            log.info("%s: party %s seat kept for %d s", self.store.accounts.get(s.xuid), pid, self.SEAT_SECONDS)
            try:
                asyncio.get_running_loop().call_later(self.SEAT_SECONDS + 1, self.expire_seat, s.xuid, pid)
            except RuntimeError:  # (no loop: tests calling the server directly)
                pass
        elif s.party:
            self.leave_party(s)
        if self.sessions.get(s.xuid) is s:
            del self.sessions[s.xuid]
        self.voice_tokens.pop(s.voice_token, None)
        self.store.last_seen[s.xuid] = int(time.time())
        self.store.save()
        log.info("%s signed out", self.store.accounts.get(s.xuid))
        self.tell_friends(s.xuid)

    def op_signout(self, conn, msg):
        """Signing out on purpose (also a program the player closes): the party seat is given up at once."""
        self.sign_out(conn, msg.get("user"))
        return {}

    def op_presence(self, conn, msg):
        s = self.session(conn, msg)
        old_title = s.title_id
        s.title_id = str(msg.get("title_id", s.title_id)).upper()
        s.title = str(msg.get("title", s.title))
        s.rich = str(msg.get("rich", ""))
        log.info("%s: %s %s", self.store.accounts.get(s.xuid), s.title, s.rich)
        self.tell_friends(s.xuid)
        if s.party:
            self.tell_party(s.party)
        if s.title_id != old_title:
            self.meet(s)  # another game: the players in it (and the party, with its new title)

    def op_profile(self, conn, msg):
        self.session(conn, msg)
        xuid = str(msg.get("xuid", "")).upper()
        if xuid not in self.store.accounts:
            raise ValueError("no such xuid")
        return {"account": self.account(xuid), "profile": self.store.profiles.get(xuid, {})}

    def op_profile_update(self, conn, msg):
        """The signed-in user edited their public profile (motto, zone, name, location, bio): kept, and their friends
        get a friend event (their consoles drop the cached profile and read it again)."""
        s = self.session(conn, msg)
        if not isinstance(msg.get("profile"), dict):
            raise ValueError("no profile")
        self.store.profiles.setdefault(s.xuid, {}).update(clean_profile(msg["profile"], PROFILE_KEYS))
        self.store.save()
        log.info("%s updated their profile", self.store.accounts.get(s.xuid))
        self.tell_friends(s.xuid)

    TITLES_MAX = 64          # titles kept per player
    TITLE_TILE_MAX = 24576   # bytes of a title's tile (base64 text)

    STATUSES = ("online", "away", "busy", "offline")

    def op_status(self, conn, msg):
        """Preferences > Online Status: with `status`, sets it (kept for the account; friends told), and returns it."""
        s = self.session(conn, msg)
        if "status" in msg:
            status = str(msg.get("status"))
            if status not in self.STATUSES:
                raise ValueError("bad status")
            if status == "online":
                self.store.status.pop(s.xuid, None)
            else:
                self.store.status[s.xuid] = status
            self.store.save()
            log.info("%s: online status %s", self.store.accounts.get(s.xuid), status)
            self.tell_friends(s.xuid)
        return {"status": self.store.status.get(s.xuid, "online")}

    FEEDBACK_KINDS = ("prefer", "avoid", "complaint")
    FEEDBACK_MAX = 2000      # feedback entries kept (the oldest dropped)

    def op_feedback(self, conn, msg):
        """A player review (prefer / avoid, with a category and reason) or a complaint about another player."""
        s = self.session(conn, msg)
        about = str(msg.get("xuid", "")).upper()
        kind = str(msg.get("kind", ""))
        if about not in self.store.accounts:
            raise ValueError("no such xuid")
        if about == s.xuid:
            raise ValueError("feedback about yourself")
        if kind not in self.FEEDBACK_KINDS:
            raise ValueError("bad kind")
        self.store.feedback.append({"from": s.xuid, "about": about, "kind": kind,
                                    "category": str(msg.get("category", ""))[:64],
                                    "reason": str(msg.get("reason", ""))[:64], "time": int(time.time())})
        del self.store.feedback[:-self.FEEDBACK_MAX]
        self.store.save()
        log.info("%s: %s about %s (%s / %s)", s.xuid, kind, about, msg.get("category", ""), msg.get("reason", ""))

    def op_titles_update(self, conn, msg):
        """The signed-in user's played titles (sent at sign-in): what Compare Games shows of them to others."""
        s = self.session(conn, msg)
        titles = msg.get("titles")
        if not isinstance(titles, list):
            raise ValueError("no titles")
        out = []
        # A title's achievements come after (title_achievements_update): keep the ones already sent.
        lists = {t.get("id"): t["list"] for t in self.store.titles.get(s.xuid, []) if "list" in t}
        for t in titles[:self.TITLES_MAX]:
            if not isinstance(t, dict):
                continue
            tile = str(t.get("tile", ""))
            out.append({"id": str(t.get("id", ""))[:8].upper(), "name": str(t.get("name", ""))[:64],
                        "achievements": max(0, int(t.get("achievements", 0) or 0)),
                        "earned": max(0, int(t.get("earned", 0) or 0)),
                        "gamerscore": max(0, int(t.get("gamerscore", 0) or 0)),
                        "tile": tile if len(tile) <= self.TITLE_TILE_MAX else ""})
            if out[-1]["id"] in lists:
                out[-1]["list"] = lists[out[-1]["id"]]
        self.store.titles[s.xuid] = out
        self.store.save()

    ACHIEVEMENTS_MAX = 256   # achievements kept per title

    def op_title_achievements_update(self, conn, msg):
        """One played title's achievements (after titles_update): what Compare Games shows of them to others."""
        s = self.session(conn, msg)
        tid = str(msg.get("title_id", ""))[:8].upper()
        achievements = msg.get("achievements")
        if not isinstance(achievements, list):
            raise ValueError("no achievements")
        title = next((t for t in self.store.titles.get(s.xuid, []) if t["id"] == tid), None)
        if title is None:
            raise ValueError("no such title")
        out = []
        for a in achievements[:self.ACHIEVEMENTS_MAX]:
            if not isinstance(a, dict):
                continue
            out.append({"name": str(a.get("name", ""))[:64], "description": str(a.get("description", ""))[:256],
                        "unachieved": str(a.get("unachieved", ""))[:256],
                        "gamerscore": max(0, int(a.get("gamerscore", 0) or 0)), "earned": bool(a.get("earned")),
                        "shown": bool(a.get("shown")), "time": max(0, int(a.get("time", 0) or 0))})
        title["list"] = out
        self.store.save()

    def op_title_achievements_of(self, conn, msg):
        """Another player's achievements in a title (Compare Games' title)."""
        self.session(conn, msg)
        xuid = str(msg.get("xuid", "")).upper()
        tid = str(msg.get("title_id", "")).upper()
        title = next((t for t in self.store.titles.get(xuid, []) if t["id"] == tid), None)
        return {"achievements": title.get("list", []) if title else []}

    def op_titles_of(self, conn, msg):
        """Another player's played titles (the Guide's Compare Games)."""
        self.session(conn, msg)
        xuid = str(msg.get("xuid", "")).upper()
        if xuid not in self.store.accounts:
            raise ValueError("no such xuid")
        return {"titles": [{k: v for k, v in t.items() if k != "list"} for t in self.store.titles.get(xuid, [])]}

    def op_friends(self, conn, msg):
        return {"friends": self.friend_list(self.session(conn, msg).xuid)}

    def op_recent_players(self, conn, msg):
        return {"players": self.recent_view(self.session(conn, msg).xuid)}

    BEACON_TEXT_MAX = 168    # the console's beacon comment limit

    def op_beacon_set(self, conn, msg):
        """The Guide's Beacons (XamShowBeaconsUI): the signed-in user wants company in a game. Friends see the beacon
        (in their friend entry and `beacons`). `title_id` / `title`: the game; `text`: the player's note."""
        s = self.session(conn, msg)
        title_id = str(msg.get("title_id", "")).upper()
        if not title_id or title_id.startswith("FFFE"):
            raise ValueError("a beacon needs a game")
        self.store.beacons[s.xuid] = {"title_id": title_id, "title": str(msg.get("title", "")),
                                      "text": str(msg.get("text", ""))[:self.BEACON_TEXT_MAX], "at": int(time.time())}
        self.store.save()
        log.info("%s set a beacon for %s", self.store.accounts.get(s.xuid), title_id)
        self.tell_friends(s.xuid)
        return {"beacon": self.store.beacons[s.xuid]}

    def op_beacon_clear(self, conn, msg):
        s = self.session(conn, msg)
        if self.store.beacons.pop(s.xuid, None) is not None:
            self.store.save()
            self.tell_friends(s.xuid)

    def op_beacons(self, conn, msg):
        """The caller's friends' beacons (online friends only), newest first: the Beacons list and the Guide's
        "friends who want to play"."""
        s = self.session(conn, msg)
        out = []
        for x in self.store.friends.get(s.xuid, set()):
            b = self.store.beacons.get(x)
            online = x in self.sessions or self.away_view(x) is not None
            if b and online and self.store.status.get(x, "") != "offline":
                out.append(dict(b, xuid=x, gamertag=self.store.accounts.get(x, "")))
        out.sort(key=lambda b: b["at"], reverse=True)
        return {"beacons": out}

    def op_friends_of(self, conn, msg):
        """A friend's friends (the Guide's View Friends on a friend's Personal Profile): only for one of the
        caller's own friends; their accepted friends only, as everyone sees them (no requests)."""
        s = self.session(conn, msg)
        xuid = str(msg.get("xuid", "")).upper()
        if xuid not in self.store.friends.get(s.xuid, set()):
            raise ValueError("not a friend")
        ids = sorted(self.store.friends.get(xuid, set()), key=lambda x: self.store.accounts.get(x, "").lower())
        out = []
        for x in ids:
            view = self.friend_view(xuid, x)
            view["request"] = ""
            out.append(view)
        return {"friends": out}

    FRIENDS_MAX = 100  # the console's friends-list limit

    def op_friend_add(self, conn, msg):
        s = self.session(conn, msg)
        other = self.store.xuid_of(str(msg.get("gamertag", "")))
        if other is None:
            raise ValueError("no such gamertag")
        if other == s.xuid:
            raise ValueError("cannot add yourself")
        if other in self.store.friends.get(s.xuid, set()):
            raise ValueError("already friends")
        if len(self.store.friends.get(s.xuid, set())) >= self.FRIENDS_MAX:
            raise ValueError("friends list full")
        if s.xuid in self.store.requests.get(other, set()):
            return self.op_friend_accept(conn, dict(msg, xuid=other))
        self.store.requests.setdefault(s.xuid, set()).add(other)
        self.store.save()
        self.send(other, {"op": "friend", "friend": self.friend_view(other, s.xuid)})
        return {"friend": self.friend_view(s.xuid, other)}

    def op_friend_accept(self, conn, msg):
        s = self.session(conn, msg)
        other = str(msg.get("xuid", "")).upper()
        if s.xuid not in self.store.requests.get(other, set()):
            raise ValueError("no request from that xuid")
        self.store.requests[other].discard(s.xuid)
        self.store.friends.setdefault(s.xuid, set()).add(other)
        self.store.friends.setdefault(other, set()).add(s.xuid)
        self.store.save()
        self.send(other, {"op": "friend", "friend": self.friend_view(other, s.xuid)})
        return {"friend": self.friend_view(s.xuid, other)}

    def op_friend_decline(self, conn, msg):
        s = self.session(conn, msg)
        other = str(msg.get("xuid", "")).upper()
        self.store.requests.get(other, set()).discard(s.xuid)
        self.store.save()
        self.send(other, {"op": "friend_removed", "xuid": s.xuid})

    def op_friend_remove(self, conn, msg):
        s = self.session(conn, msg)
        other = str(msg.get("xuid", "")).upper()
        self.store.friends.get(s.xuid, set()).discard(other)
        self.store.friends.get(other, set()).discard(s.xuid)
        self.store.requests.get(s.xuid, set()).discard(other)
        self.store.save()
        self.send(other, {"op": "friend_removed", "xuid": s.xuid})

    MESSAGE_TEXT_MAX = 256  # the console's message-body limit

    def op_message_send(self, conn, msg):
        s = self.session(conn, msg)
        to = [str(x).upper() for x in msg.get("to", [])]
        if not 1 <= len(to) <= 16:
            raise ValueError("bad recipient list")
        text = str(msg.get("text", ""))
        if len(text) > self.MESSAGE_TEXT_MAX:
            raise ValueError("message too long")
        self.deliver(s.xuid, to, text, str(msg.get("kind", "text")), str(msg.get("title_id", "")))

    def op_messages(self, conn, msg):
        s = self.session(conn, msg)
        return {"messages": list(reversed(self.store.messages.get(s.xuid, [])))}

    def op_message_read(self, conn, msg):
        s = self.session(conn, msg)
        for m in self.store.messages.get(s.xuid, []):
            if m["id"] == msg.get("id"):
                m["read"] = True
        self.store.save()

    def op_message_delete(self, conn, msg):
        s = self.session(conn, msg)
        self.store.messages[s.xuid] = [m for m in self.store.messages.get(s.xuid, []) if m["id"] != msg.get("id")]
        self.store.save()

    def expire_away(self, xuid):
        """Not back within the minute: friends see them go offline now."""
        a = self.away.get(xuid)
        if a is None or a["until"] > time.time() or xuid in self.sessions:
            return
        del self.away[xuid]
        self.tell_friends(xuid)

    def expire_seat(self, xuid, pid):
        """A held seat nobody came back to: the member leaves the party."""
        held = self.held_seats.get(xuid)
        if held is None or held[0] != pid or held[1] > time.time():
            return
        del self.held_seats[xuid]
        p = self.parties.get(pid)
        if p is None or xuid not in p["members"]:
            return
        p["members"].remove(xuid)
        log.info("%s did not come back: left party %s", self.store.accounts.get(xuid), pid)
        if not p["members"]:
            del self.parties[pid]
            return
        if p["leader"] == xuid:
            p["leader"] = p["members"][0]
        self.tell_party(pid)
        self.tell_friends(xuid)

    def leave_party(self, s):
        p = self.parties.get(s.party)
        if p is not None:
            self.meet(s)  # the party's members: last met now
        pid, s.party = s.party, None
        if p is None:
            return
        if s.xuid in p["members"]:
            p["members"].remove(s.xuid)
        self.send(s.xuid, {"op": "party", "party": None})
        if not p["members"]:
            del self.parties[pid]
            return
        if p["leader"] == s.xuid:
            p["leader"] = p["members"][0]
        self.tell_party(pid)
        self.tell_friends(s.xuid)

    def op_party_create(self, conn, msg):
        s = self.session(conn, msg)
        if s.party:
            self.leave_party(s)
        pid = "P%d" % next(self.party_ids)
        self.parties[pid] = {"id": pid, "leader": s.xuid, "members": [s.xuid], "open": False}
        s.party = pid
        self.tell_friends(s.xuid)
        return {"party": self.party_view(pid)}

    def op_party_invite(self, conn, msg):
        s = self.session(conn, msg)
        if not s.party:
            raise ValueError("not in a party")
        self.parties[s.party].setdefault("invited", set()).add(str(msg.get("xuid", "")).upper())
        self.deliver(s.xuid, [str(msg.get("xuid", "")).upper()], s.party, "party_invite")

    def op_party_join(self, conn, msg):
        """Joins `party`, or with `friend` (a friend's XUID) the party that friend is in (the Guide's Join Party on a
        friend's profile); the same rules either way."""
        s = self.session(conn, msg)
        pid = str(msg.get("party", ""))
        if msg.get("friend"):
            friend = str(msg["friend"]).upper()
            if friend not in self.store.friends.get(s.xuid, set()):
                raise ValueError("not a friend")
            f = self.sessions.get(friend)
            if f is None or not f.party:
                raise ValueError("not in a party")
            pid = f.party
        p = self.parties.get(pid)
        if p is None:
            raise ValueError("no such party")
        invited = s.xuid in p.get("invited", set())
        if p.get("invite_only") and not invited:
            raise ValueError("not invited")
        if not p["open"] and not invited and not (set(p["members"]) & self.store.friends.get(s.xuid, set())):
            raise ValueError("not invited")
        if s.party and s.party != pid:
            self.leave_party(s)
        if s.xuid not in p["members"]:
            p["members"].append(s.xuid)
        s.party = pid
        self.meet(s)
        self.tell_party(pid)
        self.tell_friends(s.xuid)
        return {"party": self.party_view(pid)}

    def op_party_settings(self, conn, msg):
        """The leader's Party Options: Party Status "Open to Friends" (friends of members may join) or "Invite Only"."""
        s = self.session(conn, msg)
        p = self.parties.get(s.party)
        if p is None:
            raise ValueError("not in a party")
        if p["leader"] != s.xuid:
            raise ValueError("not the party leader")
        p["invite_only"] = bool(msg.get("invite_only", False))
        self.tell_party(s.party)
        return {"party": self.party_view(s.party)}

    def op_party_leave(self, conn, msg):
        self.leave_party(self.session(conn, msg))

    def op_party_kick(self, conn, msg):
        """The leader's Kick from Party (Guide.Friends row 0x13, XamPartyKickUser): the member leaves the party."""
        s = self.session(conn, msg)
        p = self.parties.get(s.party)
        if p is None:
            raise ValueError("not in a party")
        if p["leader"] != s.xuid:
            raise ValueError("not the party leader")
        target = str(msg.get("xuid", "")).upper()
        if target == s.xuid or target not in p["members"]:
            raise ValueError("not in the party")
        t = self.sessions.get(target)
        if t is not None and t.party == s.party:
            self.leave_party(t)
        else:
            p["members"].remove(target)
            self.tell_party(s.party)
        p.get("invited", set()).discard(target)
        return {"party": self.party_view(s.party)}

    def op_party(self, conn, msg):
        s = self.session(conn, msg)
        return {"party": self.party_view(s.party) if s.party else None}

    def op_log(self, conn, msg):
        """Live debugging (a client with live_log = 1): its log lines as they are written, kept per connection in
        <logs>/<date>-<time>-<address>.log; the first signed-in gamertag is noted in it."""
        lines = msg.get("lines")
        if not isinstance(lines, list):
            raise ValueError("no lines")
        if conn.log_file is None:
            os.makedirs(self.logs, exist_ok=True)
            name = "%s-%s.log" % (time.strftime("%Y%m%d-%H%M%S"), conn.peer.replace(":", "_"))
            conn.log_file = open(os.path.join(self.logs, name), "a", encoding="utf-8", errors="replace")
            conn.log_file.write("# live log of %s (%s), %s\n" % (conn.peer, conn.client, time.strftime("%Y-%m-%d %H:%M:%S")))
            log.info("%s: live log -> %s", conn.peer, os.path.join(self.logs, name))
        if msg.get("dropped"):
            conn.log_file.write("# ... %d lines not sent (queue full or connection lost) ...\n" % int(msg["dropped"]))
        for line in lines:
            conn.log_file.write(str(line) + "\n")
        conn.log_file.flush()

    # ---- content subsystem (the dashboard downloads content packages from us, like the console did from LIVE) ----
    CONTENT_FILE_MAX = 4 * 1024 * 1024  # a served file's size cap

    def content_path(self, package, file):
        """The on-disk path of a content file, refusing anything outside the content root (no path traversal)."""
        root = os.path.realpath(self.content_root)
        if not package or not file or "/" in package or "\\" in package:
            raise ValueError("bad content name")
        path = os.path.realpath(os.path.join(root, package, file))
        if (path != root and not path.startswith(root + os.sep)) or not os.path.isfile(path):
            raise ValueError("no such content")
        return path

    def op_content_manifest(self, conn, msg):
        """Every content package and its files with sizes and hashes: the dashboard caches by hash and only refetches
        what changed. `package` limits it to one package."""
        only = str(msg.get("package", ""))
        out = {}
        root = self.content_root
        for pkg in sorted(os.listdir(root)) if os.path.isdir(root) else []:
            if only and pkg != only:
                continue
            pdir = os.path.join(root, pkg)
            if not os.path.isdir(pdir):
                continue
            files = {}
            for f in sorted(os.listdir(pdir)):
                fp = os.path.join(pdir, f)
                if os.path.isfile(fp) and os.path.getsize(fp) <= self.CONTENT_FILE_MAX:
                    with open(fp, "rb") as h:
                        files[f] = {"size": os.path.getsize(fp), "hash": hashlib.sha1(h.read()).hexdigest()}
            out[pkg] = files
        return {"content": out}

    def op_xon_schema(self, conn, msg):
        """The client's Xbox LIVE service schema (the copy in its memory: Microsoft's data, never stored here),
        sent once per connection before its first `xon` task."""
        conn.xon_schema = xon_services.load_schema(base64.b64decode(msg.get("blob", "")))
        return {"requests": conn.xon_schema.n_req}

    def op_xon(self, conn, msg):
        """One Xbox LIVE service task (an .ashx service: xon_services.py): {index, wire, user_base, buf_size,
        default_count} -> {hr, path, native (base64), used}."""
        schema = getattr(conn, "xon_schema", None)
        if schema is None:
            raise ValueError("no service schema (xon_schema first)")
        s = conn.users.get(msg.get("user"))
        msg = dict(msg, wire_bytes=base64.b64decode(msg.get("wire", "") or ""))
        r = xon_services.call(self, schema, s.xuid if s else None, msg)
        if "native" in r:
            r["native"] = base64.b64encode(r["native"]).decode("ascii")
        return r

    # ---- XOnlineCallWebService (XAM's web-service engine, xws_engine.py) ----
    WS_HANDLES_MAX = 16  # open Begin/Continue calls per connection

    async def run_web_call(self, conn, msg):
        """Builds the HTTP request XAM would send for a call (template, schemas, the caller's request bytes) and
        answers it with web_services.py: a web_services.Response."""
        livehive = {**web_services.LIVEHIVE, **self.store.livehive}
        try:
            method, url, headers, body = xws_engine.build_request(
                str(msg.get("url", "")), str(msg.get("method", "GET")), str(msg.get("req_schema", "") or ""),
                base64.b64decode(msg.get("req", "") or ""), livehive)
        except (xws_engine.SchemaError, ValueError) as e:
            raise ValueError("web-service request: %s" % e)
        call = {"method": method, "url": url, "headers": dict(headers),
                "body": base64.b64encode(body).decode("ascii"), "user": msg.get("user")}
        reply = await self.op_web(conn, call)
        return reply

    @staticmethod
    def web_fill(msg, reply):
        """The caller's response buffer after the response (trailing zero bytes left out: the client zero-fills)."""
        out, hr, length = xws_engine.fill_response(
            str(msg.get("resp_schema", "") or ""), int(msg.get("resp_size", 0)), reply["status"], reply["headers"],
            base64.b64decode(reply["body"]), int(msg.get("flags", 0)))
        return {"status": reply["status"], "hr": hr, "resp": base64.b64encode(out.rstrip(b"\0")).decode("ascii"),
                "length": length}

    async def op_ws_call(self, conn, msg):
        """XOnlineCallWebService (0x5806A): {user, url, method, flags, req_schema, req, resp_schema, resp_size} ->
        {status, hr, resp, length}: the whole call."""
        return self.web_fill(msg, await self.run_web_call(conn, msg))

    async def op_ws_begin(self, conn, msg):
        """BeginRequest (0x5806B): performs the call now and keeps its response for ws_response / ws_read."""
        handles = conn.__dict__.setdefault("ws_handles", {})
        if len(handles) >= self.WS_HANDLES_MAX:
            raise ValueError("too many open web-service calls")
        reply = await self.run_web_call(conn, msg)
        handle = conn.__dict__.setdefault("ws_next", 1)
        conn.ws_next = handle + 1
        handles[handle] = {"reply": reply, "body": base64.b64decode(reply["body"])}
        return {"handle": handle}

    def op_ws_response(self, conn, msg):
        """BeginResponse (0x5806D): {handle, resp_schema, resp_size, flags} -> {status, hr, resp, length}."""
        h = conn.__dict__.get("ws_handles", {}).get(msg.get("handle"))
        if h is None:
            raise ValueError("no such web-service call")
        return self.web_fill(msg, h["reply"])

    def op_ws_read(self, conn, msg):
        """ContinueResponse (0x5806E): {handle, offset, size} -> {data}: the next bytes of a stream body."""
        h = conn.__dict__.get("ws_handles", {}).get(msg.get("handle"))
        if h is None:
            raise ValueError("no such web-service call")
        at, n = int(msg.get("offset", 0)), int(msg.get("size", 0))
        return {"data": base64.b64encode(h["body"][at:at + max(0, n)]).decode("ascii")}

    def op_ws_close(self, conn, msg):
        """XOnlineCloseWebServiceHandle (0x5806F)."""
        conn.__dict__.get("ws_handles", {}).pop(msg.get("handle"), None)

    async def op_web(self, conn, msg):
        """A web-service call the dashboard made (XOnlineCallWebService), answered by web_services.py: {method, url,
        headers, body (base64)} -> {status, headers, body (base64)}. `user` names the signed-in caller (its XUID
        is what lists/xuid(...) may name); calls without one (search) are anonymous. A call that needs the PC's
        search engine fetches it on a worker thread, so other consoles' requests go on meanwhile."""
        fetched = None
        fetch = self.web.prefetch(msg)
        if fetch:
            fetched = await asyncio.get_running_loop().run_in_executor(None, fetch)
        s = conn.users.get(msg.get("user"))
        return self.web.call(conn, s.xuid if s else None, msg, fetched).message()

    def op_content_get(self, conn, msg):
        """One content file's bytes (base64), with its hash (the client caches it)."""
        package, file = str(msg.get("package", "")), str(msg.get("file", ""))
        path = self.content_path(package, file)
        if os.path.getsize(path) > self.CONTENT_FILE_MAX:
            raise ValueError("content too large")
        with open(path, "rb") as h:
            data = h.read()
        return {"package": package, "file": file, "hash": hashlib.sha1(data).hexdigest(),
                "data": base64.b64encode(data).decode("ascii")}


VOICE_MAGIC = b"XV01"


class VoiceProtocol(asyncio.DatagramProtocol):
    """Party voice (network_protocol.md "Voice"): a member's frame is relayed to the party's other members."""

    def __init__(self, server):
        self.server = server
        self.transport = None

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # client -> server: magic, token (8), sequence (u32), codec payload (may be empty: a keep-alive)
        if len(data) < 16 or data[:4] != VOICE_MAGIC:
            return
        s = self.server.voice_tokens.get(data[4:12])
        if s is None:
            return
        s.voice_addr = addr
        if len(data) == 16 or not s.party:
            return
        p = self.server.parties.get(s.party)
        if p is None:
            return
        # server -> client: magic, sender XUID (8), sequence, payload
        out = VOICE_MAGIC + struct.pack(">Q", int(s.xuid, 16)) + data[12:]
        for x in p["members"]:
            other = self.server.sessions.get(x)
            if other is not None and other is not s and other.voice_addr is not None:
                self.transport.sendto(out, other.voice_addr)


class Connection:
    def __init__(self, server, reader, writer):
        self.server, self.reader, self.writer = server, reader, writer
        self.peer = "%s:%d" % writer.get_extra_info("peername")[:2]
        self.client = ""
        self.title_id = "FFFE07D1"
        self.users = {}
        self.log_file = None  # live debugging (op_log)

    def send(self, obj):
        # A console that went away (closed, crashed, lost its network) gets nothing more; writing to a closed
        # transport would make asyncio print "socket.send() raised exception" for every event.
        if self.writer.is_closing():
            return
        if obj.get("op") != "reply":
            log.debug("%s <- event %s user=%s", self.peer, obj.get("op"), obj.get("user"))
        self.writer.write((json.dumps(obj, separators=(",", ":")) + "\n").encode("utf-8"))

    async def run(self):
        try:
            while True:
                line = await self.reader.readline()
                if not line:
                    break
                try:
                    msg = json.loads(line)
                except ValueError:
                    log.warning("%s: bad line %r", self.peer, line[:200])
                    continue
                if not isinstance(msg, dict):
                    log.warning("%s: bad message %r", self.peer, line[:200])
                    continue
                started = time.perf_counter()
                if msg.get("op") != "log":  # (live logs go to their own files)
                    log.debug("%s -> %s user=%s id=%s (%d bytes)", self.peer, msg.get("op"), msg.get("user"),
                              msg.get("id"), len(line))
                try:
                    reply = self.server.handle(self, msg)
                    if asyncio.iscoroutine(reply):  # an op that waits (op_web's search engine)
                        reply = await reply
                    if "id" in msg:
                        self.send(dict(reply, op="reply", id=msg["id"], ok=True))
                        log.debug("%s <- reply %s id=%s ok (%.1f ms)", self.peer, msg.get("op"), msg["id"],
                                  (time.perf_counter() - started) * 1000)
                except (ValueError, KeyError, TypeError) as e:
                    log.info("%s: %s failed: %s", self.peer, msg.get("op"), e)
                    if "id" in msg:
                        self.send({"op": "reply", "id": msg["id"], "ok": False, "error": str(e)})
                except Exception:  # a server bug: this request fails, the server and the connection go on
                    log.exception("%s: %s: server error", self.peer, msg.get("op"))
                    if "id" in msg:
                        self.send({"op": "reply", "id": msg["id"], "ok": False, "error": "server error"})
                await self.writer.drain()
        except (OSError, asyncio.IncompleteReadError, asyncio.LimitOverrunError, ValueError) as e:
            # ConnectionError and Windows' other socket errors (WinError 64 / 121 when a console on the Internet
            # drops), or a line over the reader's limit
            if not isinstance(e, ConnectionError):
                log.info("%s: connection lost (%s)", self.peer, e)
        finally:
            for user in list(self.users):
                self.server.sign_out(self, user, keep_seat=True)
            if self.log_file is not None:
                self.log_file.write("# connection closed %s\n" % time.strftime("%Y-%m-%d %H:%M:%S"))
                self.log_file.close()
            self.writer.close()
            log.info("%s: closed", self.peer)


async def main():
    p = argparse.ArgumentParser()
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=36074)
    p.add_argument("--data", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "live_server_data.json"))
    p.add_argument("--logs", help="live debugging: where clients' logs go (default: live_logs beside the data file)")
    p.add_argument("--content", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "content"),
                   help="content packages served to the dashboard (default: content/ beside this script)")
    p.add_argument("--xboxunity", choices=["on", "off"], default="on",
                   help="find every Xbox 360 title and its box art on xboxunity.net (default: on)")
    p.add_argument("--search-provider", choices=["none", "google"], default="google",
                   help="where the bing hub's web suggestions come from besides the game catalog (default: google)")
    p.add_argument("--verbose", action="store_true", help="log every request and reply (debugging)")
    p.add_argument("--log", default="", help="also write the log to this file (appended)")
    args = p.parse_args()
    handlers = [logging.StreamHandler()]
    if args.log:
        handlers.append(logging.FileHandler(args.log, encoding="utf-8"))
    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO, format="%(asctime)s %(message)s",
                        handlers=handlers)
    logging.getLogger("asyncio").setLevel(logging.INFO)
    server = Server(Store(args.data), args.logs or os.path.join(os.path.dirname(os.path.abspath(args.data)), "live_logs"),
                    args.content, args.search_provider,
                    xboxunity.XboxUnity(os.path.join(os.path.dirname(os.path.abspath(args.data)), "xboxunity_cache"),
                                        args.xboxunity == "on"))
    srv = await asyncio.start_server(lambda r, w: Connection(server, r, w).run(), args.host, args.port,
                                     limit=1 << 20)  # a line carries gamer pictures (base64)
    loop = asyncio.get_running_loop()
    # Socket errors of connections that went away are not the server's problem: one line, no traceback.
    def on_error(loop, context):
        e = context.get("exception")
        if isinstance(e, OSError):
            log.info("network: %s", e)
        else:
            loop.default_exception_handler(context)
    loop.set_exception_handler(on_error)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if hasattr(socket, "SIO_UDP_CONNRESET"):
        # Windows: a voice packet sent to a console that has gone (ICMP port unreachable) must not reset the socket.
        sock.ioctl(socket.SIO_UDP_CONNRESET, False)
    sock.bind((args.host, args.port))
    _, server.voice = await loop.create_datagram_endpoint(lambda: VoiceProtocol(server), sock=sock)
    where = "every network interface of this PC (0.0.0.0)" if args.host in ("0.0.0.0", "") else args.host
    log.info("listening on %s, port %d TCP and UDP (data %s)", where, args.port, args.data)
    async with srv:
        await srv.serve_forever()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass
