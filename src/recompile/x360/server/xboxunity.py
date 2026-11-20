"""XboxUnity (xboxunity.net, the Xbox 360 community's title database that Aurora and FSD use) for our Xbox LIVE: the
bing hub's search finds every Xbox 360 title by name, and box art is the title's cover. All calls block (the server
runs them off its event loop, live_server.op_web) and are cached: searches in memory for a day, covers on disk.

  search(q)  -> [{"id", "name", "type"}]      TitleList.php?search=q   (type: "360", "XBLA", "XBLIG", "Xbox", ...)
  cover(tid) -> JPEG bytes of the front cover  CoverInfo.php?titleid=   then Cover.php?size=large&cid=
               (XboxUnity's covers are the whole case wrap, back | spine | front: the front is cut out)
"""
import io
import json
import logging
import os
import threading
import time
import urllib.parse
import urllib.request

log = logging.getLogger("live")

ROOT = "https://xboxunity.net/Resources/Lib/"
TIMEOUT = 2.5          # inside the client's 3 s request wait
SEARCH_TTL = 24 * 3600
USER_AGENT = "Xbox360Recomp-LIVE/1.0 (+our own Xbox LIVE server)"


class XboxUnity:
    def __init__(self, cache_dir, enabled=True):
        self.enabled = enabled
        self.cache_dir = cache_dir
        self.searches = {}   # lowercase query -> (time, results)
        self.lock = threading.Lock()

    def get(self, path, query):
        url = ROOT + path + "?" + urllib.parse.urlencode(query)
        req = urllib.request.Request(url, headers={"User-Agent": USER_AGENT})
        with urllib.request.urlopen(req, timeout=TIMEOUT) as r:
            return r.read()

    def search(self, q, count=20):
        q = q.strip()
        if not self.enabled or not q:
            return []
        key = q.lower()
        with self.lock:
            hit = self.searches.get(key)
            if hit and time.time() - hit[0] < SEARCH_TTL:
                return hit[1]
        try:
            data = json.loads(self.get("TitleList.php", {"page": 0, "count": count, "search": q, "sort": 3,
                                                          "direction": 1, "category": 0, "filter": 0}))
            out = [{"id": str(i.get("TitleID", "")).upper(), "name": str(i.get("Name", "")),
                    "type": str(i.get("TitleType", ""))} for i in data.get("Items", []) if i.get("TitleID")]
        except Exception as e:
            log.info("xboxunity: search %r failed: %s", q, e)
            return []
        with self.lock:
            self.searches[key] = (time.time(), out)
            if len(self.searches) > 2000:
                self.searches.pop(next(iter(self.searches)))
        return out

    def cover(self, tid):
        """The title's front cover as JPEG bytes (None if XboxUnity has none); cached on disk, misses too."""
        tid = tid.upper()
        if not self.enabled or len(tid) != 8 or not all(c in "0123456789ABCDEF" for c in tid):
            return None
        path = os.path.join(self.cache_dir, tid + ".jpg")
        miss = os.path.join(self.cache_dir, tid + ".none")
        if os.path.exists(path):
            with open(path, "rb") as f:
                return f.read()
        if os.path.exists(miss) and time.time() - os.path.getmtime(miss) < 7 * 24 * 3600:
            return None
        try:
            info = json.loads(self.get("CoverInfo.php", {"titleid": tid}))
            covers = info.get("Covers") or []
            # The official cover first, then the best rated.
            covers.sort(key=lambda c: (str(c.get("Official")) != "1", -int(c.get("Rating") or 0)))
            data = self.get("Cover.php", {"size": "large", "cid": covers[0]["CoverID"]}) if covers else None
            image = front(data) if data else None
        except Exception as e:
            log.info("xboxunity: cover of %s failed: %s", tid, e)
            return None
        os.makedirs(self.cache_dir, exist_ok=True)
        if image is None:
            open(miss, "wb").close()
            return None
        with open(path, "wb") as f:
            f.write(image)
        log.info("xboxunity: cover of %s (%d bytes)", tid, len(image))
        return image


def front(data):
    """The front of a case-wrap cover (its right part, past the spine), as JPEG; a cover that is already a front
    (taller than wide) stays as it is."""
    try:
        from PIL import Image
    except ImportError:
        return data
    im = Image.open(io.BytesIO(data)).convert("RGB")
    w, h = im.size
    if w > h:
        # Xbox 360 case wraps: back and front halves around a spine of about 6 % of the width.
        im = im.crop((int(w * 0.53), 0, w, h))
    out = io.BytesIO()
    im.save(out, "JPEG", quality=90)
    return out.getvalue()
