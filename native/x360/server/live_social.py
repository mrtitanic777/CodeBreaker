"""live_social.py -- the LIVE server's account and social requests (network_protocol.md): generated and changed
gamertags, the signup's account login (email and password), player reviews and complaints with the reputation
they give, message attachments (voice mail, photos) and a friend's party. A mixin of live_server.Server: it uses the
server's store, sessions and events.
"""
import base64
import hashlib
import re
import secrets
import time

GAMERTAG = re.compile(r"^[A-Za-z](?:[A-Za-z0-9]| (?! ))*$")     # letters, digits, single spaces; a letter first
EMAIL = re.compile(r"^[^@\s]+@[^@\s]+\.[^@\s]+$")
ADJECTIVES = ("Brave", "Clever", "Silent", "Rapid", "Lucky", "Mighty", "Golden", "Cosmic", "Frosty", "Sunny",
              "Rusty", "Noble", "Swift", "Wild", "Happy", "Shadow", "Crimson", "Electric", "Gentle", "Iron")
ANIMALS = ("Falcon", "Otter", "Panda", "Tiger", "Wolf", "Badger", "Koala", "Lynx", "Moose", "Raven", "Gecko", "Bison",
           "Heron", "Llama", "Puffin", "Shark", "Walrus", "Yak", "Fox", "Owl")
REVIEW_MAX, COMPLAINT_MAX = 9, 15              # FB_REVIEW_PREFER 0, AVOID 1-9; FB_COMPLAINT_* 0-15
VOICE_FRAME, VOICE_FRAMES_MAX = 164, 750       # the party-voice frame (Voice); 15 s of them
PHOTO_MAX = 256 * 1024


def gamertag_problem(tag):
    """Why `tag` is not a gamertag, or None."""
    if not 1 <= len(tag) <= 15:
        return "a gamertag is 1-15 characters"
    if tag.endswith(" ") or not GAMERTAG.match(tag):
        return "letters, digits and single spaces, starting with a letter"
    return None


class SocialOps:
    # ---- gamertags ----
    def _taken(self, tag, by=None):
        owner = self.store.xuid_of(tag)
        return owner is not None and owner != by

    def _generated(self, base=None):
        for _ in range(200):
            stem = base or (secrets.choice(ADJECTIVES) + secrets.choice(ANIMALS))
            tag = (stem[:15 - 3] + str(secrets.randbelow(900) + 100)) if base else stem[:11] + str(secrets.randbelow(9000) + 1000)
            if not gamertag_problem(tag) and not self._taken(tag):
                return tag
        raise ValueError("no free gamertag")

    def op_generate_gamertag(self, conn, msg):
        """A free generated gamertag (adjective + animal + number), not reserved: the signup asks for one."""
        return {"gamertag": self._generated()}

    def op_gamertag_check(self, conn, msg):
        s = self.session(conn, msg)
        tag = str(msg.get("gamertag", ""))
        reason = gamertag_problem(tag) or ("taken" if self._taken(tag, s.xuid) else None)
        out = {"available": reason is None, "reason": reason or ""}
        if reason is not None:
            stem = re.sub(r"[^A-Za-z0-9 ]", "", tag).strip() or "Player"
            if not stem[0].isalpha():
                stem = "X" + stem
            out["suggestions"] = [self._generated(stem.replace("  ", " ")) for _ in range(3)]
        return out

    def op_gamertag_change(self, conn, msg):
        """A new gamertag for the signed-in user: friends and the party are told, and messages they sent show it. The
        old one is kept so a console signing in with it gets the new one."""
        s = self.session(conn, msg)
        tag = str(msg.get("gamertag", ""))
        problem = gamertag_problem(tag)
        if problem:
            raise ValueError(problem)
        if self._taken(tag, s.xuid):
            raise ValueError("gamertag taken")
        old = self.store.accounts.get(s.xuid, "")
        if old and old != tag:
            self.store.old_gamertags.setdefault(s.xuid, [])
            if old not in self.store.old_gamertags[s.xuid]:
                self.store.old_gamertags[s.xuid].append(old)
        self.store.accounts[s.xuid] = tag
        for inbox in self.store.messages.values():
            for m in inbox:
                if m.get("from", {}).get("xuid") == s.xuid:
                    m["from"] = self.account(s.xuid)
        self.store.save()
        self.tell_friends(s.xuid)
        if s.party:
            self.tell_party(s.party)
        return {"account": self.account(s.xuid)}

    def current_gamertag(self, xuid, tag):
        """The gamertag a sign-in gets: after a change the server's wins over the old one a console still has."""
        stored = self.store.accounts.get(xuid)
        if stored and tag != stored and tag in self.store.old_gamertags.get(xuid, []):
            return stored
        return tag

    # ---- the signup's account login ----
    def op_email_check(self, conn, msg):
        email = str(msg.get("email", "")).strip().lower()
        if not EMAIL.match(email):
            raise ValueError("bad email")
        return {"free": all(v.get("email") != email for v in self.store.logins.values())}

    def keep_login(self, xuid, login):
        """`register`'s optional `login`: kept with the account, the password as a salted PBKDF2-SHA256 hash."""
        email = str(login.get("email", "")).strip().lower()
        password = str(login.get("password", ""))
        if not EMAIL.match(email):
            raise ValueError("bad email")
        if not 8 <= len(password) <= 16:
            raise ValueError("bad password")
        if any(v.get("email") == email for x, v in self.store.logins.items() if x != xuid):
            raise ValueError("email in use")
        salt = secrets.token_bytes(16)
        self.store.logins[xuid] = {
            "email": email, "salt": salt.hex(),
            "hash": hashlib.pbkdf2_hmac("sha256", password.encode("utf-8"), salt, 200000).hex(),
            **{k: str(login.get(k, ""))[:64] for k in ("first_name", "last_name", "birthdate", "country")}}

    # ---- reviews and complaints ----
    def reputation(self, xuid):
        """0-100 once a player has reviews: their average (Prefer 100, Avoid 0) with 5 more at the player's own value
        (their profile's reputation, else 60 = 3 stars). Without reviews: the profile's own value, if any."""
        own = self.store.profiles.get(xuid, {}).get("reputation")
        reviews = self.store.reviews.get(xuid, {})
        if not reviews:
            return own
        base = own if isinstance(own, (int, float)) else 60
        total = sum(100 if r == 0 else 0 for r in reviews.values()) + 5 * base
        return int(round(total / (len(reviews) + 5)))

    def op_player_review(self, conn, msg):
        s = self.session(conn, msg)
        about = str(msg.get("xuid", "")).upper()
        review = int(msg.get("review", -1))
        if about == s.xuid or about not in self.store.accounts:
            raise ValueError("bad xuid")
        if not 0 <= review <= REVIEW_MAX:
            raise ValueError("bad review")
        self.store.reviews.setdefault(about, {})[s.xuid] = review
        self.store.save()
        self.tell_friends(about)
        return {"reputation": self.reputation(about)}

    def op_complaint(self, conn, msg):
        s = self.session(conn, msg)
        about = str(msg.get("xuid", "")).upper()
        kind = int(msg.get("type", -1))
        if about not in self.store.accounts:
            raise ValueError("bad xuid")
        if not 0 <= kind <= COMPLAINT_MAX:
            raise ValueError("bad complaint type")
        self.store.feedback.append({"from": s.xuid, "about": about, "kind": "complaint", "type": kind,
                                    "text": str(msg.get("text", ""))[:499], "time": int(time.time())})
        del self.store.feedback[:-self.FEEDBACK_MAX]
        self.store.save()

    def review_of(self, reviewer, about):
        return self.store.reviews.get(about, {}).get(reviewer, -1)

    # ---- message attachments ----
    def attachment(self, a):
        """A message's attachment {"type","format","data"} checked and kept: its view for the message."""
        if not isinstance(a, dict):
            raise ValueError("bad attachment")
        kind, fmt = str(a.get("type", "")), str(a.get("format", ""))
        try:
            data = base64.b64decode(str(a.get("data", "")), validate=True)
        except ValueError:
            raise ValueError("bad attachment data")
        view = {"type": kind, "format": fmt, "size": len(data)}
        if kind == "voice":
            if fmt != "adpcm16k":
                raise ValueError("bad attachment format")
            if not data or len(data) % VOICE_FRAME:
                raise ValueError("bad voice data")
            if len(data) // VOICE_FRAME > VOICE_FRAMES_MAX:
                raise ValueError("attachment too large")
            view["duration_ms"] = len(data) // VOICE_FRAME * 20
        elif kind == "photo":
            if len(data) > PHOTO_MAX:
                raise ValueError("attachment too large")
            if not ((fmt == "png" and data[:8] == b"\x89PNG\r\n\x1a\n") or (fmt == "jpeg" and data[:3] == b"\xff\xd8\xff")):
                raise ValueError("bad photo data")
        else:
            raise ValueError("bad attachment type")
        key = hashlib.sha1(data).hexdigest()
        self.store.attachments[key] = {"type": kind, "format": fmt, "data": base64.b64encode(data).decode("ascii")}
        view["key"] = key
        return view

    def drop_unused_attachments(self):
        used = {m["attachment"]["key"] for inbox in self.store.messages.values() for m in inbox if m.get("attachment")}
        for key in [k for k in self.store.attachments if k not in used]:
            del self.store.attachments[key]

    def op_message_attachment(self, conn, msg):
        s = self.session(conn, msg)
        mid = msg.get("message", msg.get("id"))
        m = next((m for m in self.store.messages.get(s.xuid, []) if m["id"] == mid), None)
        if m is None or not m.get("attachment"):
            raise ValueError("no attachment")
        a = self.store.attachments.get(m["attachment"]["key"])
        if a is None:
            raise ValueError("no attachment")
        return {"type": a["type"], "format": a["format"], "data": a["data"]}

    # ---- a friend's party ----
    def op_party_of(self, conn, msg):
        """One of the user's friends' party (the Social hub's party details), or null."""
        s = self.session(conn, msg)
        xuid = str(msg.get("xuid", "")).upper()
        if xuid not in self.store.friends.get(s.xuid, set()):
            raise ValueError("not a friend")
        f = self.sessions.get(xuid)
        pid = f.party if f is not None else (self.held_seats.get(xuid) or (None,))[0]
        return {"party": self.party_view(pid) if pid else None}
