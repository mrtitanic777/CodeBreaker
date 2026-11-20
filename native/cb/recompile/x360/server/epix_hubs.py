"""The hubs' online tiles, as the console's online manifest (dashhome.xml) had them in 2013: Xbox LIVE gave home and
games their own layouts (the dash's epix:// templates) with icon tiles, a spotlight and game tiles, where the offline
manifest has one "Connect to Live" picture. The layouts and tiles are the online console's (the user's photos); the
pictures are the player's own games with XboxUnity's box art (Microsoft's promotions are gone).

  GET /epix/hub/<hub>   the hub's slot groups in the layout's slot order (text/xml); <slotgroup keep="name"/> is one
                        of the hub's own (the runtime puts it there), enable="yes" without its enabled=no. Header
                        X-Epix-Template names the layout. 404: the hub stays as the offline manifest has it.
  GET /epix/img/<xuid>/<kind>/<titleid>.jpg   a game tile's picture (JPEG).

The runtime asks for each hub when it builds the online manifest (runtime kernel/epix_home.cpp).
"""
import base64
import io
import logging
import os
from xml.sax.saxutils import escape

log = logging.getLogger("live")

# The dash loads a slot's picture over its own HTTP (XHttp), which the runtime answers for epix.localhost: /live/...
# is passed on to this server (runtime kernel/xam_xhttp.cpp).
HOST = "http://epix.localhost/live"
# Template1HD: the online home's 9 tiles. Slot groups in manifest order: 1 tray (left, top), 2 the spotlight,
# 3 pins (right, top), 4 recent (left, middle), then ours: 5 right middle, 6 left bottom, 7 and 8 the wide tiles
# under the spotlight, 9 right bottom. Sizes in the template's units (its FLOT table).
TEMPLATE = "epix://Template1HD.xur"
SIZES = {"big": (466, 264), "big18": (352, 264), "small": (174, 130), "wide": (231, 130)}
SCALE = 2  # pictures at twice the tile size, as the console's (the hub scales them down)

XBOX_GREEN = (16, 124, 16)


def font(size, light=False):
    try:
        from PIL import ImageFont
    except ImportError:
        return None
    names = ("segoeuil.ttf", "segoeui.ttf", "arial.ttf") if light else ("segoeui.ttf", "arial.ttf")
    for name in names:
        for path in (name, os.path.join(os.environ.get("WINDIR", r"C:\Windows"), "Fonts", name)):
            try:
                return ImageFont.truetype(path, size)
            except OSError:
                pass
    return ImageFont.load_default()


def png_bytes(b64):
    try:
        return base64.b64decode(b64) if b64 else None
    except ValueError:
        return None


class Hubs:
    def __init__(self, store, unity=None):
        self.store = store
        self.unity = unity

    # ---- data ----
    def recent_titles(self, xuid):
        """The player's games, most recently played first (by their latest achievement; profile order breaks ties)."""
        titles = [t for t in self.store.titles.get(xuid, []) if not str(t.get("id", "")).upper().startswith("FFFE")]

        def last(t):
            return max([a.get("time", 0) or 0 for a in t.get("list", [])] or [0])
        order = {id(t): i for i, t in enumerate(titles)}
        return sorted(titles, key=lambda t: (-last(t), order[id(t)]))

    def profile(self, xuid):
        return self.store.profiles.get(xuid, {}) if xuid else {}

    # ---- the hub's slots ----
    @staticmethod
    def slot(name, picture, cmd=None, param=None, description=None):
        click = ""
        if cmd:
            click = ("<onclick><button>A</button><action>EpixCmd</action><cmd>%s</cmd>%s</onclick>"
                     % (cmd, "<param>%s</param>" % escape(param) if param is not None else ""))
        return ("<slotgroup><slot><name>%s</name>%s<shallowimg>%s</shallowimg>%s</slot></slotgroup>"
                % (escape(name), "<description>%s</description>" % escape(description) if description else "",
                   escape(picture), click))

    def img(self, xuid, kind, arg=None):
        return "%s/epix/img/%s/%s%s.jpg" % (HOST, xuid, kind, "/" + arg if arg else "")

    @staticmethod
    def keep(key, enable=False):
        """One of the hub's own slot groups from the dash's manifest, by its slot's name or definition path (the
        runtime puts it here); enable: without its enabled=no (the online console had it on)."""
        return '<slotgroup keep="%s"%s/>' % (escape(key), ' enable="yes"' if enable else "")

    @staticmethod
    def icon(label, icon, cmd=None, param=None):
        """A green icon tile, as the dash's own (EsLiveTile with a common:// icon, its label the description)."""
        click = ""
        if cmd:
            click = ("<onclick><button>A</button><action>EpixCmd</action><cmd>%s</cmd>%s</onclick>"
                     % (cmd, "<param>%s</param>" % escape(param) if param is not None else ""))
        return ("<slotgroup><slot><name>%s</name><description>%s</description><deepasset><format>EpixScene</format>"
                "<path>EsLiveTile</path><param>common://%s</param></deepasset>%s</slot></slotgroup>"
                % (escape(label), escape(label), icon, click))

    def game_tile(self, xuid, g, kind):
        """A picture tile of a game (its box art), labelled with its name; A opens its details."""
        return self.slot(g["name"], self.img(xuid, kind, g["id"]), "EcNavToURI", "gamedetails:%d" % int(g["id"], 16),
                         g["name"])

    def hub(self, xuid, hub):
        """(template, [slot group XML]) for a hub, in the layout's slot order, or None to leave the hub as the dash's
        offline manifest has it. The layouts and tiles are the online console's (2013, the user's photos)."""
        if not xuid:
            return None
        games = self.recent_titles(xuid)
        if hub == "home":
            # Template1HD: 1 tray, 2 spotlight, 3 / 5 right column, 4 / 6 left column, 7 / 8 wide, 9 right bottom.
            spotlight = self.game_tile(xuid, games[0], "spotlight") if games else self.keep("Connect to Live")
            return "epix://Template1HD.xur", [
                self.keep("slot://DiscInTray"),
                spotlight,
                self.icon("Search", "ico_64x_search.png"),
                self.keep("Pins", enable=True),
                self.icon("Need Help?", "ico_64x_gamedemo.png"),
                self.keep("Recent"),
                self.icon("Microsoft Movies & TV", "ico_64x_movie.png", "EcNavToURI",
                          "applibrary:Category=4;_LibraryType=video;"),
                self.icon("My Apps", "ico_64x_apps.png", "EcNavToURI", "applibrary:_LibraryType=apps;"),
                self.icon("My Games", "ico_64x_controller.png", "EcNavToURI",
                          "gameslibrary:_RootTitle=My Games;"),
            ]
        if hub == "games" and games:
            # Template18HD: a 2 x 3 grid of icon tiles (slots 1, 2, 4 ... 7), the spotlight (3), two game tiles (8, 9).
            return "epix://Template18HD.xur", [
                self.keep("Collection"),
                self.icon("Add Ons", "ico_64x_gameAddOn.png"),
                self.game_tile(xuid, games[0], "spotlight18"),
                self.keep("Browse Games", enable=True),
                self.icon("Demos", "ico_64x_gamedemo.png"),
                self.keep("Search Games", enable=True),
                self.icon("A - Z", "Ico_64x_AZ.png"),
            ] + [self.game_tile(xuid, g, "game") for g in games[1:3]]
        return None

    def popular(self):
        """Every title the players here have played, most players first: [(title, players)]."""
        seen = {}
        for titles in self.store.titles.values():
            for t in titles:
                tid = str(t.get("id", "")).upper()
                if tid and not tid.startswith("FFFE"):
                    seen.setdefault(tid, [t, 0])[1] += 1
        return sorted(seen.values(), key=lambda e: (-e[1], str(e[0].get("name", "")).lower()))

    # ---- pictures ----
    def cover(self, tid):
        return self.unity.cover(tid) if self.unity else None

    def needs_cover(self, parts):
        """The title whose XboxUnity cover a picture request needs (fetched off the server's event loop), or None."""
        if len(parts) >= 4 and parts[2] in ("spotlight", "spotlight18", "game"):
            return parts[3].split(".")[0].upper()
        return None

    def picture(self, xuid, kind, arg, cover=None):
        try:
            from PIL import Image, ImageDraw, ImageFilter
        except ImportError:
            return None
        title = None
        if arg:
            title = next((t for t in self.store.titles.get(xuid, []) if str(t.get("id", "")).upper() == arg), None)
        p = self.profile(xuid)

        def canvas(size, colour=XBOX_GREEN):
            w, h = SIZES[size]
            w, h = w * SCALE, h * SCALE
            im = Image.new("RGB", (w, h), colour)
            d = ImageDraw.Draw(im)
            r, g, b = colour
            for y in range(h):  # the tile colour darkening downwards, as the dash's own tiles
                t = y / max(1, h - 1)
                d.line([(0, y), (w, y)], fill=(int(r * (1 - .45 * t)), int(g * (1 - .45 * t)), int(b * (1 - .45 * t))))
            return im, d

        def open_png(data):
            try:
                return Image.open(io.BytesIO(data)).convert("RGBA") if data else None
            except Exception:
                return None

        def blurred(im, art):
            w, h = im.size
            bg = art.convert("RGB").resize((w, max(h, int(w * art.height / art.width))))
            top = (bg.height - h) // 2
            bg = bg.crop((0, top, w, top + h)).filter(ImageFilter.GaussianBlur(20))
            im.paste(Image.blend(bg, Image.new("RGB", bg.size, (0, 0, 0)), .45), (0, 0))

        def paste(im, pic, box):
            x, y, w, h = box
            pic = pic.resize((w, h))
            im.paste(pic, (x, y), pic)

        def lines(d, x, y, items, width):
            """Draw (text, size, light, colour) rows from y, each cut to fit `width`; returns the y after them."""
            for text, size, light, colour in items:
                f = font(size, light)
                text = str(text)
                while text and d.textlength(text, font=f) > width:
                    text = text[:-2].rstrip() + "…" if len(text) > 2 else ""
                    if text.endswith("……"):
                        text = text[:-1]
                d.text((x, y), text, font=f, fill=colour)
                y += int(size * 1.25)
            return y

        # A game's tile: its box art over a blur of itself (the dash draws the name in a band along the bottom).
        size = {"spotlight": "big", "spotlight18": "big18", "game": "small"}.get(kind)
        if not size or not title:
            return None
        im, d = canvas(size)
        w, h = im.size
        pic = open_png(cover) or open_png(png_bytes(title.get("tile")))
        if pic:
            blurred(im, pic)
            ch = int(h * .76)
            cw = int(ch * pic.width / pic.height)
            paste(im, pic, ((w - cw) // 2, int(h * .03), cw, ch))
        return im

    def image(self, parts, cover=None):
        """GET /epix/img/<xuid>/<kind>[/<arg>].jpg -> JPEG bytes or None."""
        if len(parts) < 3:
            return None
        xuid = parts[1].upper()
        kind = parts[2].split(".")[0]
        arg = parts[3].split(".")[0].upper() if len(parts) >= 4 else None
        im = self.picture(xuid, kind, arg, cover)
        if im is None:
            return None
        out = io.BytesIO()
        im.convert("RGB").save(out, "JPEG", quality=88)  # as the console's spotlight pictures (a big PNG is not shown)
        return out.getvalue()
