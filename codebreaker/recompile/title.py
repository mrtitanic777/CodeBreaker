"""title.py -- an Xbox 360 title as it comes, ready for recompile-project.

    from codebreaker.recompile import title
    t = title.locate(path)          # -> Title, or None when `path` holds no title
    t.read_xex()                    # default.xex's bytes
    t.unpack(folder)                # every file of the title under `folder` (once; a marker records it)

A title comes as a LIVE/CON/PIRS package (Xbox LIVE Arcade, installs), a disc image (ISO, CSO), a
Games-on-Demand folder, a folder of its files, or a folder holding any of those (a console's
content folders: <title id>/000D0000/<package>). The title is the volume whose top folder holds
default.xex; packages beside it without one (title updates, DLC) are left alone.
"""
import json
import os

from codebreaker.formats.xbox import volumes

_STFS = (b"CON ", b"LIVE", b"PIRS")
_DISCS = (".iso", ".xiso", ".cso", ".cci")
_MARKER = ".codebreaker-unpacked"
_FORMAT = 2       # what an unpack writes; a marker of another format is unpacked again (1: STFS files lost their folders)


class Title(object):
    def __init__(self, source, name, volume=None, folder=None):
        self.source, self.name = source, name
        self._volume, self._folder = volume, folder

    def _xex_entry(self):
        for e in self._volume.files():
            if not e.is_dir and e.path.replace("\\", "/").strip("/").lower() == "default.xex":
                return e
        return None

    def read_xex(self):
        if self._folder:
            with open(os.path.join(self._folder, _default_xex(self._folder)), "rb") as f:
                return f.read()
        return self._volume.read(self._xex_entry())

    def unpack(self, folder):
        """Writes the title's files under `folder` unless they are there from the same source.
        -> files written (0 when already unpacked)."""
        marker = os.path.join(folder, _MARKER)
        stamp = {"source": os.path.abspath(self.source), "size": _size(self.source), "format": _FORMAT}
        try:
            with open(marker, encoding="utf-8") as f:
                if json.load(f) == stamp:
                    return 0
        except (OSError, ValueError):
            pass
        n = 0
        if self._folder:
            import shutil
            for dp, _dn, fn in os.walk(self._folder):
                for name in fn:
                    src = os.path.join(dp, name)
                    dst = os.path.join(folder, os.path.relpath(src, self._folder))
                    os.makedirs(os.path.dirname(dst), exist_ok=True)
                    shutil.copy2(src, dst)
                    n += 1
        else:
            for e in self._volume.files():
                if e.is_dir:
                    continue
                dst = os.path.join(folder, *[p for p in e.path.replace("\\", "/").split("/") if p])
                os.makedirs(os.path.dirname(dst), exist_ok=True)
                with open(dst, "wb") as f:
                    f.write(self._volume.read(e))
                n += 1
        with open(marker, "w", encoding="utf-8") as f:
            json.dump(stamp, f)
        return n


def _size(path):
    if os.path.isfile(path):
        return os.path.getsize(path)
    return sum(os.path.getsize(os.path.join(dp, n)) for dp, _dn, fn in os.walk(path) for n in fn)


def _default_xex(folder):
    for n in os.listdir(folder):
        if n.lower() == "default.xex" and os.path.isfile(os.path.join(folder, n)):
            return n
    return None


def _from_volumes(path, found):
    for v in found or ():
        t = Title(path, None, volume=v)
        if t._xex_entry():
            label = v.boot.get("display name") if isinstance(v.boot, dict) else None
            name = label or v.label or os.path.basename(path)
            if name.lower().endswith(_DISCS):
                name = os.path.splitext(name)[0]
            t.name = "".join(c for c in name if c.isascii() and c.isprintable()).strip() or "title"
            return t
    return None


def _container(path):
    """A Title when the file `path` is a package or disc image holding one."""
    try:
        with open(path, "rb") as f:
            magic = f.read(4)
    except OSError:
        return None
    if magic not in _STFS and not path.lower().endswith(_DISCS):
        return None
    try:
        return _from_volumes(path, volumes.volumes(path))
    except Exception:  # a damaged or foreign file: not this title
        return None


def locate(path):
    """The title in `path` (a package, disc image, GoD folder, folder of files, or a folder holding
    any of those), or None."""
    path = os.path.abspath(path)
    if os.path.isfile(path):
        return _container(path)
    if not os.path.isdir(path):
        return None
    if _default_xex(path):
        return Title(path, os.path.basename(path), folder=path)
    try:
        t = _from_volumes(path, volumes.volumes(path))  # Games on Demand
        if t:
            t.name = os.path.basename(path) if t.name in (None, "GoD") else t.name
            return t
    except Exception:
        pass
    for dp, dn, fn in os.walk(path):
        dn.sort()
        for name in sorted(fn):
            t = _container(os.path.join(dp, name))
            if t:
                return t
        for d in dn:
            if _default_xex(os.path.join(dp, d)):
                sub = os.path.join(dp, d)
                return Title(sub, d, folder=sub)
    return None
