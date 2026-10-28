"""ctext.py -- C text with its strings, character constants and comments blanked: what is code, at the
offsets it has, for the regular expressions and counts that must not look inside a literal.

    from codebreaker.base import ctext
    ctext.blank(text)               what is inside each comment, string and character constant made
                                    spaces -- the quotes and a comment's newlines kept, so every offset
                                    and every line is the same; one left open runs to the end of its
                                    line (a string) or of the text (a comment)
    ctext.blank(text, whole=True)   each closed one made spaces whole, its quotes and newlines too (a
                                    quote with no partner on its line is left as it is: an apostrophe)
    ctext.blank(text, whole=True, across_lines=True)
                                    the same, but a string or character constant runs on across lines
                                    to its partner (how exact/batch's brace count has always read them)
    ctext.sub_code(text, rx, fn)    re.sub of what is code alone (not in strings or comments)
"""
import functools
import re

_OPEN = re.compile(r"/\*[\s\S]*?(?:\*/|\Z)|\"(?:\\[\s\S]|[^\"\\\n])*\"?|'(?:\\[\s\S]|[^'\\\n])*'?")
_CLOSED = re.compile(r"/\*[\s\S]*?\*/|\"(?:\\[\s\S]|[^\"\\\n])*\"|'(?:\\[\s\S]|[^'\\\n])*'")
_ACROSS = re.compile(r"/\*[\s\S]*?\*/|\"(?:\\[\s\S]|[^\"\\])*\"|'(?:\\[\s\S]|[^'\\])*'")


def _inside(m):
    t = m.group(0)
    if t.startswith("/*"):                                  # (a comment: its newlines kept)
        return re.sub(r"[^\n]", " ", t)
    q = t[0]
    end = len(t) - 1 if len(t) > 1 and t[-1] == q else len(t)
    return q + " " * (end - 1) + t[end:]                    # (a string: inside it spaces, its quotes kept)


@functools.lru_cache(maxsize=16384)
def blank(text, whole=False, across_lines=False):
    """the text with its comments, strings and character constants blanked, every offset the same: inside
    them made spaces (the quotes and newlines kept), or all of each closed one (`whole`; with
    `across_lines` a string or character constant may close on a later line). A text seen before is
    given as it was -- the passes ask often."""
    if whole:
        return (_ACROSS if across_lines else _CLOSED).sub(lambda m: " " * len(m.group(0)), text)
    return _OPEN.sub(_inside, text)


def sub_code(text, rx, fn):
    """re.sub of what is code alone (not in strings or comments)"""
    code = blank(text)
    out, last = [], 0
    for m in rx.finditer(code):
        out += [text[last:m.start()], fn(m)]
        last = m.end()
    return "".join(out) + text[last:]
