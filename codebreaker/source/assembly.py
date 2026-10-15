"""assembly.py -- what the assembly-source writers share (drivers, real-mode programs).

    assembly.operands(text)                  a decoder's operand text in the assembler's spelling
    assembly.data_lines(bytes)               bytes as `db` lines, text as text
    assembly.probe(candidates, ...)          which instruction texts the assembler turns back into
                                             the original's bytes

An instruction is written as its mnemonic only when the assembler turns that text back into the
bytes the original has -- assemblers choose between equivalent encodings, and a different choice
moves everything behind it. That is checked rather than assumed: every distinct instruction text
is assembled once, at a place of its own in a probe file, and its bytes are read back from the
object file. What the probe has established is remembered (state/assembly_probes.json), so a
text is assembled once however many programs use it.
"""
import json
import threading
import os
import re
import struct

from codebreaker.base import settings
from codebreaker.build import driver as builder

CONDITIONS = ("o", "no", "b", "ae", "e", "ne", "be", "a", "s", "ns", "p", "np", "l", "ge", "le", "g")
SPELLING = {"pushal": "pushad", "popal": "popad", "int3": "int 3", "retn": "ret", "fwait": "wait",
            "pushaw": "pusha", "popaw": "popa", "retfw": "retf", "iretw": "iret"}
STRING_OPS = re.compile(r"^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)([bwd])\b")
_HEX = re.compile(r"\b0x([0-9a-f]+)\b")
_TEXT = re.compile(rb"[\x20-\x26\x28-\x7e]{5,}")              # printable, without the quote itself


def assemble(source, work):
    """Assemble one file with OpenWatcom's assembler. -> (object file or None, its messages)"""
    wasm = builder.tool("wasm")
    if not wasm:
        return None, "no assembler (settings.json: openwatcom)"
    work = os.path.abspath(work)                             # the assembler runs with this as its cwd
    obj = os.path.join(work, os.path.splitext(os.path.basename(source))[0] + ".obj")
    if os.path.exists(obj):
        os.remove(obj)
    code, log = builder._run([wasm, "-zq", "-fo=" + obj, os.path.abspath(source)], work)
    return (obj if code == 0 and os.path.isfile(obj) else None), log


def link(lines, out, work):
    """Link with OpenWatcom's linker from the directives in `lines`. -> (file or None, messages)"""
    wlink = builder.tool("wlink")
    if not wlink:
        return None, "no linker (settings.json: openwatcom)"
    work, out = os.path.abspath(work), os.path.abspath(out)
    script = os.path.splitext(out)[0] + ".lnk"
    with open(script, "w", newline="\r\n") as f:
        f.write("\n".join(list(lines) + ["name %s" % out]) + "\n")
    if os.path.exists(out):
        os.remove(out)
    code, log = builder._run([wlink, "@" + script], work)
    return (out if code == 0 and os.path.isfile(out) else None), log


def operands(text):
    """Hexadecimal numbers the way the assembler reads them (0x1f -> 01Fh)."""
    return _HEX.sub(lambda m: "0%sh" % m.group(1).upper(), text)


def mnemonic(name):
    return SPELLING.get(name, name)


def data_lines(data, per_line=16):
    """Bytes as `db` lines; runs of printable characters as quoted text."""
    out, at, data = [], 0, bytes(data)

    def plain(chunk):
        for k in range(0, len(chunk), per_line):
            out.append("        db   " + ", ".join("0%02Xh" % b for b in chunk[k:k + per_line]))
    for m in _TEXT.finditer(data):
        plain(data[at:m.start()])
        text = m.group(0).decode("latin1")
        for k in range(0, len(text), 60):
            out.append("        db   '%s'" % text[k:k + 60])
        at = m.end()
    plain(data[at:])
    return out


def _key(candidate, mode):
    return "%s|%s|%s|%s" % (mode, candidate[0], candidate[1].hex(), candidate[2].hex())


def _known():
    try:
        with open(settings.state("assembly_probes.json"), encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return {}


_PROBE_LOCK = threading.Lock()         # (one probe at a time: they share the file of what is known)


def probe(candidates, labels, outdir, mode="flat32", near=()):
    with _PROBE_LOCK:
        return _probe(candidates, labels, outdir, mode, near)


def _probe(candidates, labels, outdir, mode="flat32", near=()):
    """Which candidates assemble to their original bytes. A candidate is (text, the original
    bytes outside patched fields, a mask with 1 for each byte of a patched field); `labels` are
    the names the texts may mention (`near` those used as call or jump targets). `mode` is
    "flat32" (32-bit code, flat model) or "real16". -> the set of candidates that do."""
    known = _known()
    todo = [c for c in set(candidates) if _key(c, mode) not in known]
    refused = set()
    use = "use32" if mode == "flat32" else "use16"
    head = [".386p", ".model flat"] if mode == "flat32" else [".386p"]
    for _attempt in range(16):
        if not todo:
            break
        lines = head + ["PD segment para public %s 'PXCODE'" % use]
        lines += ["%s label %s" % (n, "near" if n in near else "byte") for n in sorted(labels)]
        lines += ["PD ends", "PC segment para public %s 'PCODE'" % use]
        if mode == "real16":
            lines.append("        assume cs:PC, ds:nothing, es:nothing, ss:nothing")
        line_of = {}
        for n, c in enumerate(todo):
            lines.append("        org  %d" % (32 * n))
            line_of[len(lines) + 1] = c
            lines.append("        " + ("nop" if c in refused else c[0]))
        lines += ["PC ends", "end"]
        src = os.path.join(outdir, "probe.asm")
        with open(src, "w", newline="\r\n", encoding="latin1") as f:
            f.write("\n".join(lines) + "\n")
        obj, messages = assemble(src, outdir)
        if obj:
            with open(obj, "rb") as f:
                got = _last_segment_data(f.read())
            for n, c in enumerate(todo):
                out = got.get(32 * n, b"")
                known[_key(c, mode)] = c not in refused and len(out) == len(c[2]) and \
                    bytes(b for b, k in zip(out, c[2]) if not k) == c[1]
            todo = []
            break
        failed = {line_of[int(m.group(1))] for m in re.finditer(r"\((\d+)\)\s*:\s*Error", messages)
                  if int(m.group(1)) in line_of}
        if not failed:                                       # the probe file itself is refused: nothing is known good
            for c in todo:
                known[_key(c, mode)] = False
            todo = []
            break
        refused |= failed
    with open(settings.state("assembly_probes.json"), "w", encoding="utf-8") as f:
        json.dump(known, f, indent=0, sort_keys=True)
    return {c for c in set(candidates) if known.get(_key(c, mode))}


def _last_segment_data(obj):
    """{offset: bytes} of the data records of the last segment an object file defines, joined
    where one record continues the one before."""
    chunks, i, n, segs = {}, 0, len(obj), 0
    while i + 3 <= n:
        kind = obj[i]
        end = i + 3 + (obj[i + 1] | (obj[i + 2] << 8))
        if end > n:
            break
        if kind in (0x98, 0x99):
            segs += 1
        elif kind in (0xA0, 0xA1):
            q = i + 4
            if kind == 0xA1:
                off = struct.unpack_from("<I", obj, q)[0]
                q += 4
            else:
                off = struct.unpack_from("<H", obj, q)[0]
                q += 2
            chunks.setdefault(obj[i + 3], {})[off] = obj[q:end - 1]
        i = end
    out, last = {}, None
    for off, data in sorted(chunks.get(segs, {}).items()):
        if last is not None and last + len(out[last]) == off:
            out[last] += data
        else:
            out[off] = bytes(data)
            last = off
    return out


def culprits(original, built, pieces):
    """Which instructions did not come back as they were. `pieces` covers the original in
    order: (offset, length, mask or None, key or None, is a jump inside the segment); a piece
    with a key is an instruction written as its mnemonic. The built bytes are walked piece by
    piece, so that an instruction that came back longer or shorter is found itself, and not
    the first place behind it where the bytes no longer line up. A jump that only changed
    because what it jumps over changed size is blamed only when nothing else is.
    -> the set of keys to write as bytes instead."""
    def same(pos, piece):
        off, n, mask, _key, _jump = piece
        if pos < 0 or pos + n > len(built):
            return False
        a, b = built[pos:pos + n], original[off:off + n]
        if mask is None:
            return a == b
        return all(x == y for x, y, k in zip(a, b, mask) if not k)

    pos, bad, jumps = 0, set(), set()
    count = len(pieces)
    k = 0
    while k < count:
        piece = pieces[k]
        if same(pos, piece):
            pos += piece[1]
            k += 1
            continue
        if piece[3] is not None:
            (jumps if piece[4] else bad).add(piece[3])
        # where does the built file continue? try the sizes this piece may have come back with
        ahead = [q for q in pieces[k + 1:k + 5] if q[1] >= 2] or pieces[k + 1:k + 2]
        for d in sorted(range(-8, 17), key=abs):
            p = pos + piece[1] + d
            if p < pos:
                continue
            q = p
            for nxt in pieces[k + 1:k + 1 + max(1, len(ahead))]:
                if not same(q, nxt):
                    break
                q += nxt[1]
            else:
                pos = p
                break
        else:
            break                                            # lost: what was found so far is the lesson
        k += 1
    return bad or jumps
