"""Edit helpers that keep a file's CRLF endings: ed.cut(path, start, end) removes text from `start` up to (not including) `end`;
ed.sub(path, old, new) replaces exactly one occurrence."""


def _read(p):
    raw = open(p, "rb").read().decode("utf-8")
    crlf = "\r\n" in raw
    return raw.replace("\r\n", "\n"), crlf


def _write(p, s, crlf):
    open(p, "wb").write((s.replace("\n", "\r\n") if crlf else s).encode("utf-8"))


def cut(p, start, end):
    s, c = _read(p)
    a = s.index(start)
    b = s.index(end, a)
    _write(p, s[:a] + s[b:], c)


def sub(p, old, new, count=1):
    s, c = _read(p)
    assert s.count(old) == count, (p, old[:60], s.count(old))
    _write(p, s.replace(old, new), c)
