"""Our Xbox LIVE's web services: the HTTP services the 17559 dashboard calls through XOnlineCallWebService (XLiveBase
message 0x5806A), answered here instead of Microsoft's hosts. The client (the PC port's runtime) sends each call as
op `web` {method, url, headers, body (base64)} and gets {status, headers, body (base64)} back
(docs/x360/network_protocol.md "Web services"). URLs reach us through the LiveHive roots this server hands out at
hello (LIVEHIVE below: every service root names the host `live.local`, which the client routes here).

Services so far:
  search    Dash.Search.lex / Dash.Search.xex (the "bing" hub): searchsettings.xml, EDS crossMediaGroupSearch /
            details / browse / related (XML), autosuggest qsonhs.aspx (JSON). The catalog is every player's played
            titles (titles_update); web suggestions can come from a PC search engine (--search-provider).
  lists     My Pins (PinService.lua): {ListServiceUriRoot}/users/xuid(X)/lists/{type}/{name}[/metadata], kept in the
            data file.
"""
import base64
import hashlib
import json
import logging
import time
import urllib.parse
import urllib.request

import epix_hubs
from xml.sax.saxutils import escape

log = logging.getLogger("live")

HOST = "http://live.local"

# LiveHive values the dash reads for these services (merged under the data file's own "livehive").
LIVEHIVE = {
    "SearchKillSwitch": "false",
    "SearchUnavailableLocales": "",
    "SearchOnlineRecUnavailableLocales": "",
    "SearchDataURL": HOST + "/eds/media",
    "SearchDataAnonymousURL": HOST + "/eds/media",
    "SearchAutosuggestURL": HOST + "/search/qsonhs.aspx",
    "SearchSpeechURL": HOST + "/search/speech",
    "EDSUriRoot": HOST + "/eds",
    "ListServiceUriRoot": HOST + "/lists",
}

# The media types the bing hub's settings switch on (searchsettings.xml) and the prompts its bubble grid cycles.
SEARCH_SETTINGS = [
    ("GamesEnabled", "TRUE"), ("AppsEnabled", "TRUE"), ("MoviesEnabled", "TRUE"), ("TvEnabled", "TRUE"),
    ("MusicEnabled", "TRUE"), ("WebVideoEnabled", "FALSE"), ("LiveTVEnabled", "FALSE"), ("SportsEnabled", "FALSE"),
    ("WebEnabled", "TRUE"),
    ("GamesTerm", "minecraft"), ("AppsTerm", "netflix"), ("MoviesTerm", "action movies"), ("TvTerm", "comedy"),
    ("MusicTerm", "rock"),
]


class Response:
    def __init__(self, status=200, body=b"", content_type="application/xml; charset=utf-8", headers=None):
        self.status, self.body = status, body if isinstance(body, bytes) else body.encode("utf-8")
        self.headers = {"Content-Type": content_type}
        self.headers.update(headers or {})

    def message(self):
        return {"status": self.status, "headers": self.headers, "body": base64.b64encode(self.body).decode("ascii")}


def not_found(what):
    return Response(404, json.dumps({"code": 404, "description": what}), "application/json")


class WebServices:
    PINS_MAX = 100  # items per list (the dash's own My Pins limit is not known; this keeps the data file small)

    def __init__(self, store, search_provider="none", unity=None):
        self.store = store
        self.search_provider = search_provider
        self.unity = unity      # xboxunity.XboxUnity: every Xbox 360 title and its cover (None: played titles only)
        self.hubs = epix_hubs.Hubs(store, unity)
        self.field_tokens = {}  # EDS field token -> the field names it stands for

    # ---- entry ----
    def prefetch(self, msg):
        """The network lookups a call needs first (the PC's search engine, XboxUnity), as a function the server runs
        off its event loop (live_server.op_web); None when the call needs none. Its result goes to call()."""
        url = urllib.parse.urlsplit(str(msg.get("url", "")))
        path = url.path.lower()
        q = urllib.parse.parse_qs(url.query).get("q", [""])[-1]
        if path.endswith("/qsonhs.aspx") and q:
            def fetch():
                return {"web": self.fetch_google(q) if self.search_provider == "google" else [],
                        "titles": self.unity.search(q) if self.unity else []}
            return fetch
        if path.endswith("/crossmediagroupsearch") and q and self.unity:
            return lambda: {"titles": self.unity.search(q, 25)}
        parts = [p for p in path.split("/") if p]
        if parts and parts[0] == "epix" and self.unity:
            tid = self.hubs.needs_cover(parts[1:])
            if tid:
                return lambda: {"cover": self.unity.cover(tid)}
        if len(parts) >= 3 and parts[0] == "eds" and parts[1] == "image" and self.unity:
            tid = parts[2].split(".")[0].upper()
            return lambda: {"cover": self.unity.cover(tid)}
        return None

    @staticmethod
    def fetch_google(q):
        """Google's query suggestions for q (blocking; [] on any failure: suggestions still come from the catalog)."""
        try:
            url = "https://suggestqueries.google.com/complete/search?client=firefox&q=" + urllib.parse.quote(q)
            with urllib.request.urlopen(url, timeout=1.5) as r:  # inside the client's 3 s wait
                return [str(x) for x in json.loads(r.read().decode("utf-8", "replace"))[1]]
        except Exception as e:
            log.info("search: Google suggestions failed: %s", e)
            return []

    def call(self, conn, xuid, msg, fetched=None):
        fetched = fetched or {}
        method = str(msg.get("method", "GET")).upper()
        url = urllib.parse.urlsplit(str(msg.get("url", "")))
        query = {k: v[-1] for k, v in urllib.parse.parse_qs(url.query, keep_blank_values=True).items()}
        headers = {str(k).lower(): str(v) for k, v in (msg.get("headers") or {}).items()}
        body = base64.b64decode(msg.get("body", "") or "")
        parts = [urllib.parse.unquote(p) for p in url.path.split("/") if p]
        log.info("web %s %s", method, url.path + ("?" + url.query if url.query else ""))
        if not parts:
            return not_found("no path")
        service = parts[0].lower()
        if url.path.lower().endswith("/searchsettings.xml"):
            return self.search_settings()
        if service == "eds":
            return self.eds(parts[1:], query, fetched)
        if service == "search":
            return self.search(parts[1:], query, fetched)
        if service == "lists":
            return self.lists(xuid, method, parts[1:], query, body)
        if service == "epix":
            return self.epix(xuid, parts[1:], fetched)
        return not_found("no such service: " + url.path)

    # ---- the hubs' online tiles (epix_hubs.py) ----
    def epix(self, xuid, parts, fetched):
        if len(parts) >= 2 and parts[0].lower() == "hub":
            made = self.hubs.hub(xuid, parts[1].lower())
            if not made:
                return not_found("no online tiles for hub " + parts[1])
            template, slots = made
            return Response(body="".join(slots), content_type="text/xml; charset=utf-8",
                            headers={"X-Epix-Template": template})
        if len(parts) >= 3 and parts[0].lower() == "img":
            png = self.hubs.image(parts, fetched.get("cover"))
            return Response(body=png, content_type="image/jpeg") if png else not_found("no such picture")
        return not_found("no such epix call")

    # ---- search ----
    def search_settings(self):
        props = "".join("<property><name>%s</name><value>%s</value></property>" % (escape(n), escape(v))
                        for n, v in SEARCH_SETTINGS)
        return Response(body='<?xml version="1.0" encoding="utf-8"?><manifest><channel><metadata>%s</metadata>'
                             "</channel></manifest>" % props)

    def catalog(self):
        """Every title any player has played, by title ID (name, tile), most played first."""
        seen = {}
        for titles in self.store.titles.values():
            for t in titles:
                tid = str(t.get("id", "")).upper()
                if not tid or tid.startswith("FFFE"):
                    continue
                entry = seen.setdefault(tid, {"id": tid, "name": t.get("name", tid), "players": 0,
                                              "tile": t.get("tile", "")})
                entry["players"] += 1
                if not entry["tile"] and t.get("tile"):
                    entry["tile"] = t["tile"]
        return sorted(seen.values(), key=lambda e: (-e["players"], e["name"].lower()))

    # XboxUnity's title types as the dash's media item types, with the ItemTypeId (content type) the search screen
    # needs for a game: 1 a full Xbox 360 game, 23 an Arcade game.
    UNITY_TYPES = {"360": ("Xbox360Game", 1), "XBLA": ("XboxArcadeGame", 23), "XBLIG": ("XboxXnaCommunityGame", 23),
                   "Xbox1": ("XboxOriginalGame", 21)}

    def find(self, q, unity=()):
        """The players' played titles that match q, then XboxUnity's matches for q (every Xbox 360 title)."""
        words = [w for w in q.lower().split() if w]
        hits = [e for e in self.catalog() if all(w in e["name"].lower() for w in words)] if words else []
        seen = {e["id"] for e in hits}
        for t in unity:
            if t["id"] not in seen and t.get("type") in self.UNITY_TYPES:
                seen.add(t["id"])
                hits.append({"id": t["id"], "name": t["name"], "type": t["type"], "players": 0, "tile": ""})
        return hits

    def game_item(self, e):
        # Dash.Search's EDS parser (its element names at 0x9AD0E098..0x9AD0E41C, case-sensitive): "ID" in capitals;
        # a game (a media type of group 4) counts only with a non-zero ItemTypeId (its content type, 0x9AD64630;
        # 1 = a full game); it appends "&h=..&w=..&format=jpg" to ResizeUrl, so that URL carries its own query.
        image = "%s/eds/image/%s.jpg" % (HOST, e["id"])
        media_type, item_type = self.UNITY_TYPES.get(e.get("type", "360"), self.UNITY_TYPES["360"])
        return ("<Item><MediaGroup>XboxGameItem</MediaGroup><MediaItemType>%s</MediaItemType>"
                "<ID>%s</ID><Name>%s</Name><ItemTypeId>%d</ItemTypeId><Images><Image><Purpose>BoxArt</Purpose>"
                "<Url>%s</Url><ResizeUrl>%s?r=1</ResizeUrl><Width>219</Width><Height>300</Height></Image></Images>"
                "<DeepLinkInfo><ProductId>%s</ProductId></DeepLinkInfo></Item>"
                % (media_type, e["id"], escape(e["name"]), item_type, image, image, e["id"]))

    def eds(self, parts, query, fetched):
        if len(parts) >= 2 and parts[0].lower() == "image":
            # Box art: the title's XboxUnity cover, else the tile from a player's profile.
            if fetched.get("cover"):
                return Response(body=fetched["cover"], content_type="image/jpeg")
            tid = parts[1].split(".")[0].upper()
            e = next((x for x in self.catalog() if x["id"] == tid), None)
            if not e or not e["tile"]:
                return not_found("no image")
            return Response(body=base64.b64decode(e["tile"]), content_type="image/png")
        op = parts[-1].lower() if parts else ""
        if op == "fields":
            # The dash's EDSHelper.lua (GetEDSFieldToken): the field list it wants back, as a short token that later
            # calls pass in their `fields` query (at most 51 characters); we keep the list behind the token.
            desired = query.get("desired", "")
            token = "f" + hashlib.sha1(desired.encode("utf-8")).hexdigest()[:16]
            self.field_tokens[token] = desired.split(".")
            return Response(body=json.dumps({"Fields": token}), content_type="application/json")
        if op == "contentrating":
            # GetEDSRatingToken: the Family Settings limits as a token for `combinedContentRating` (none here).
            return Response(body=json.dumps({"CrossMediaRatingToken": "r0"}), content_type="application/json")
        if op == "crossmediagroupsearch":
            items = self.find(query.get("q", ""), fetched.get("titles", ()))
            limit = int(query.get("maxItems", "25") or 25)
            body = "".join(self.game_item(e) for e in items[:limit])
            # One ResultCounter per media item type, counting exactly the items sent (no further pages).
            counts = {}
            for e in items[:limit]:
                media_type = self.UNITY_TYPES.get(e.get("type", "360"), self.UNITY_TYPES["360"])[0]
                counts[media_type] = counts.get(media_type, 0) + 1
            totals = "<Totals>%s</Totals>" % "".join(
                "<ResultCounter><MediaItemTypes>%s</MediaItemTypes><Count>%d</Count></ResultCounter>" % (t, n)
                for t, n in counts.items())
            return Response(body='<?xml version="1.0" encoding="utf-8"?><Response><Items>%s</Items>%s'
                                 "<ImpressionGuid>%s</ImpressionGuid></Response>"
                                 % (body, totals, "00000000-0000-0000-0000-%012x" % int(time.time())))
        if op == "details":
            ids = [i.upper() for i in query.get("ids", "").split(".") if i]
            items = [e for e in self.catalog() if e["id"] in ids]
            return Response(body='<?xml version="1.0" encoding="utf-8"?><Response><Items>%s</Items></Response>'
                                 % "".join(self.game_item(e) for e in items))
        if op in ("browse", "related"):
            return Response(body='<?xml version="1.0" encoding="utf-8"?><Response><Items/></Response>')
        return not_found("no such EDS call: " + "/".join(parts))

    def suggestions(self, q, fetched):
        out = [e["name"] for e in self.find(q, fetched.get("titles", ()))][:8]
        out += [s for s in fetched.get("web", []) if s not in out]
        return out[:10]

    def search(self, parts, query, fetched):
        if parts and parts[-1].lower() == "qsonhs.aspx":
            suggests = [{"Txt": s, "Type": "AS"} for s in self.suggestions(query.get("q", ""), fetched)]
            return Response(body=json.dumps({"AS": {"Query": query.get("q", ""), "FullResults": 1,
                                                    "Results": [{"Type": "AS", "Suggests": suggests}]}}),
                            content_type="application/json; charset=utf-8")
        return not_found("no such search call: " + "/".join(parts))

    # ---- My Pins (lists) ----
    def lists(self, xuid, method, parts, query, body):
        # users/xuid(<hex or decimal>)/lists/<type>/<name>[/metadata]
        if len(parts) < 4 or parts[0].lower() != "users" or parts[2].lower() != "lists":
            return not_found("no such lists call")
        who = parts[1][parts[1].find("(") + 1:parts[1].rfind(")")]
        # Xbox LIVE's REST URLs name a XUID in decimal (what the console sends); 16 hex digits with a letter are
        # taken as our hex form.
        if who.lower() in ("me", ""):
            owner = xuid
        elif who.isdigit():
            owner = "%016X" % int(who)
        elif len(who) == 16 and all(c in "0123456789abcdefABCDEF" for c in who):
            owner = who.upper()
        else:
            return not_found("bad xuid " + who)
        if owner != xuid:
            return Response(403, json.dumps({"code": 403, "description": "not your list"}), "application/json")
        key = "/".join(parts[3:5])
        lists = self.store.lists.setdefault(owner, {})
        items = lists.setdefault(key, [])
        if method == "GET":
            skip, top = int(query.get("skipItems", "0") or 0), int(query.get("maxItems", "100") or 100)
            page = items[skip:skip + top]
            return Response(body=json.dumps({"ListMetadata": {"ListVersion": len(items), "ListCount": len(items),
                                                              "MaxListSize": self.PINS_MAX},
                                             "ListItems": page}), content_type="application/json")
        if method in ("POST", "PUT"):
            try:
                data = json.loads(body.decode("utf-8") or "{}")
            except ValueError:
                return Response(400, json.dumps({"code": 400, "description": "bad JSON"}), "application/json")
            new = data.get("Items") or data.get("ListItems") or ([data] if data else [])
            for item in new:
                if len(items) < self.PINS_MAX:
                    items.append(item)
            self.store.save()
            return Response(body=json.dumps({"ListVersion": len(items)}), content_type="application/json")
        if method == "DELETE":
            lists.pop(key, None)
            self.store.save()
            return Response(body=json.dumps({"ListVersion": 0}), content_type="application/json")
        return not_found("method " + method)
