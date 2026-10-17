"""Frozen cases: a function's input and output, recorded once from a real run and kept as JSON in
tests/fixtures/frozen/. A test replays the input and wants the same output -- each pass of the C
text, the description, each format reader pinned on its own, so a change to one shows as a
difference there and not only far downstream.

Recorded by tools/freeze_tests.py; replayed by tests/test_frozen.py. A difference after an
intended change is recorded again (tools/freeze_tests.py ... --refresh) with the reason in the
commit.

Values that JSON has no word for are tagged: bytes {"$b": hex}, a large buffer read only in one
window {"$w": [start, hex]} (zeros elsewhere), set {"$s": [...]}, tuple {"$t": [...]}, a dict with
keys that are not strings {"$d": [[k, v], ...]}."""
import json
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FIXTURES = os.path.join(ROOT, "tests", "fixtures", "frozen")


class Window(object):
    """A large buffer of which a function reads only [start, start+len(data))."""

    def __init__(self, start, data):
        self.start, self.data = start, bytes(data)

    def bytes(self):
        return b"\0" * self.start + self.data


def encode(v):
    if isinstance(v, Window):
        return {"$w": [v.start, v.data.hex()]}
    if isinstance(v, (bytes, bytearray)):
        return {"$b": bytes(v).hex()}
    if isinstance(v, (set, frozenset)):
        return {"$s": sorted((encode(x) for x in v), key=lambda x: json.dumps(x, sort_keys=True))}
    if isinstance(v, tuple):
        return {"$t": [encode(x) for x in v]}
    if isinstance(v, list):
        return [encode(x) for x in v]
    if isinstance(v, dict):
        if all(isinstance(k, str) for k in v) and not any(k.startswith("$") for k in v):
            return {k: encode(x) for k, x in v.items()}
        return {"$d": [[encode(k), encode(x)] for k, x in v.items()]}
    if v is None or isinstance(v, (bool, int, float, str)):
        return v
    raise TypeError("cannot freeze %s" % type(v).__name__)


def decode(v):
    if isinstance(v, list):
        return [decode(x) for x in v]
    if isinstance(v, dict):
        if len(v) == 1:
            (k, x), = v.items()
            if k == "$w":
                return Window(x[0], bytes.fromhex(x[1])).bytes()
            if k == "$b":
                return bytes.fromhex(x)
            if k == "$s":
                return {_hashable(decode(y)) for y in x}
            if k == "$t":
                return tuple(decode(y) for y in x)
            if k == "$d":
                return {_hashable(decode(a)): decode(b) for a, b in x}
        return {k: decode(x) for k, x in v.items()}
    return v


def _hashable(v):
    return tuple(_hashable(x) for x in v) if isinstance(v, list) else v


def path(group, name):
    return os.path.join(FIXTURES, group, name + ".json")


def save(group, name, data):
    p = path(group, name)
    os.makedirs(os.path.dirname(p), exist_ok=True)
    text = json.dumps(data, indent=1, sort_keys=True, ensure_ascii=False)
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(text + "\n")


def load(group, name):
    with open(path(group, name), encoding="utf-8") as f:
        return json.load(f)


def names(group):
    folder = os.path.join(FIXTURES, group)
    if not os.path.isdir(folder):
        return []
    return sorted(f[:-len(".json")] for f in os.listdir(folder) if f.endswith(".json"))


def first_difference(want, got, where=""):
    """Where two decoded values first differ, in words (None if they are the same)."""
    if type(want) is not type(got):
        return "%s: %s vs %s" % (where or "value", type(want).__name__, type(got).__name__)
    if isinstance(want, dict):
        for k in sorted(set(want) | set(got), key=repr):
            if k not in got:
                return "%s[%r]: missing" % (where, k)
            if k not in want:
                return "%s[%r]: unexpected" % (where, k)
            d = first_difference(want[k], got[k], "%s[%r]" % (where, k))
            if d:
                return d
        return None
    if isinstance(want, (list, tuple)):
        for i, (a, b) in enumerate(zip(want, got)):
            d = first_difference(a, b, "%s[%d]" % (where, i))
            if d:
                return d
        if len(want) != len(got):
            return "%s: %d items vs %d" % (where, len(want), len(got))
        return None
    if want != got:
        if isinstance(want, str) and isinstance(got, str):
            wl, gl = want.split("\n"), got.split("\n")
            for i, (a, b) in enumerate(zip(wl, gl)):
                if a != b:
                    return "%s line %d:\n         want %r\n         got  %r" % (where, i + 1, a[:160], b[:160])
            return "%s: %d lines vs %d" % (where, len(wl), len(gl))
        return "%s: %r vs %r" % (where, str(want)[:160], str(got)[:160])
    return None
