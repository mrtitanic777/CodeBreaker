#!/usr/bin/env python3
"""
verify.py — check the reconstruction against the binary, automatically.

The project's whole claim is that every line traces to the 1985 code. Up to now
that was enforced by discipline and by reading. This checks it mechanically.

For each reconstructed C function that cites an address, it compares:

    the API calls the C body makes      vs.      the calls the binary makes
                                                 at that address

and reports three kinds of finding:

  GHOST    the C calls something the binary does NOT call there.
           This is an error — a name that drifted, a hand-written line that
           was never in the code, a copy-paste between functions.

  missing  the binary calls something the C does not mention.
           Expected for a "[documented partial]"; suspicious for a function
           claimed as fully transcribed, so those are reported separately.

  unresolved  the cited address is not a function start in the binary. That is
           a finding about the COMMENT, not the code — usually a citation that
           drifted — and it is kept separate so it cannot masquerade as a ghost.

Getting this trustworthy took four rounds against reality: the first run cried
"154 errors", every one of which was the tool's fault (function extents taken to
the first `ret`; moveable-entry thunks not followed; cross-segment calls left
unresolved; brace matching fooled by pseudo-code inside comments; a shared
comment block attributing one function's address to its neighbour). A verifier
that cries wolf is worse than none, so each of those is fixed and documented in
the code below.

Address citations are the reconstruction's own comments — `seg3:0x0a6f`,
`USER.151`, `@5` — which is what makes this checkable at all.

Usage:
    python verify.py                # check everything
    python verify.py user_menu      # check files matching a substring
    python verify.py --strict       # also report MISSING on partials
"""
import glob, os, re, sys, collections
import idord, trace
from src.core import layout
from src.disasm.disasm import open_ne

FNDEF = re.compile(r"^([A-Za-z_][A-Za-z0-9_ \t\*]*?\b)([A-Za-z_]\w*)\s*\(([^;{]*)\)\s*\{", re.M)
SYS = {"kernel_": "KERNEL", "gdi_": "GDI", "user_": "USER", "comm": "COMM",
       "keyboard": "KEYBOARD", "mouse": "MOUSE", "sound": "SOUND",
       "system": "SYSTEM", "display": "DISPLAY"}
APPS = {"calc": "CALC", "calendar": "CALENDAR", "cardfile": "CARDFILE",
        "clipbrd": "CLIPBRD", "clock": "CLOCK", "control": "CONTROL",
        "notepad": "NOTEPAD", "paint_core": "PAINT", "paint_tools": "PAINT",
        "reversi": "REVERSI", "spooler": "SPOOLER", "terminal": "TERMINAL",
        "write_wnd": "WRITE", "write_dlg": "WRITE"}


def mask(src):
    """A copy of the source with comments and string/char literals blanked to
    spaces, preserving every offset. Brace matching MUST run on this: several
    files carry pseudo-code in comments (`while (GetMessage(...)) {`) and a
    naive scan runs straight past the end of the function — which is what made
    one NOTEPAD function appear to call 58 different APIs."""
    out = list(src)
    i = 0
    while i < len(src) - 1:
        if src[i] == "/" and src[i + 1] == "*":
            j = src.find("*/", i + 2)
            j = len(src) if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j; continue
        if src[i] in "\"'":
            q = src[i]; j = i + 1
            while j < len(src) and src[j] != q:
                j += 2 if src[j] == "\\" else 1
            for k in range(i, min(j + 1, len(src))):
                if out[k] != "\n":
                    out[k] = " "
            i = j + 1; continue
        i += 1
    return "".join(out)


def bodies(src):
    """[(name, argtext, body, preceding_comment)] for each function in a file."""
    out = []
    msk = mask(src)
    for m in FNDEF.finditer(src):
        try:
            i = msk.index("{", m.start())
        except ValueError:
            continue        # the "definition" was inside a comment
        d = 0; j = i
        while j < len(msk):
            if msk[j] == "{": d += 1
            elif msk[j] == "}":
                d -= 1
                if d == 0: break
            j += 1
        # Find the address citation. The comment block immediately above is
        # NOT reliable — a forward declaration for a neighbouring helper often
        # sits there, and taking its address attributes the wrong function
        # (this produced 150 false "ghosts" on the first run). Require the
        # block to NAME the function, and search backwards for the nearest one
        # that does.
        name = m.group(2)
        # 8000, not 2500: a comment block is a comment block however long, and
        # a 2500-char window silently loses the citation of any function whose
        # banner is longer than that — the block's opening `/*` falls outside
        # the window, `findall` never sees it, and the function is reported as
        # having no address citation at all. CALC's DrawKeypad hit exactly that.
        head = src[max(0, m.start() - 8000):m.start()]
        blocks = re.findall(r"/\*.*?\*/", head, flags=re.S)
        cite = ""
        for b in reversed(blocks):
            if re.search(re.escape(name), b, re.I) and re.search(r"seg\d+:0x", b):
                # A block may document several functions at once ("SetDlgItemText
                # (…0x04fa) and GetDlgItemText (…0x052f)"). Take the address
                # NEAREST this function's name, not the first in the block.
                nm = re.search(re.escape(name), b, re.I)
                addrs = list(re.finditer(r"seg\d+:0x[0-9a-fA-F]{1,4}", b))
                if nm and len(addrs) > 1:
                    best = min(addrs, key=lambda a: abs(a.start() - nm.start()))
                    cite = best.group(0)
                else:
                    cite = b
                break
        out.append((name, m.group(3), src[i:j + 1], cite))
    return out


def build_byaddr(ne, mod, ordinals):
    """(seg, offset) -> API name, so a call into ANOTHER segment resolves to
    its name instead of `seg1:0x61fd`. Without this the verifier reports every
    cross-segment call as a ghost."""
    out = {}
    for e in ne.entries():
        if e.get("kind") != "UNUSED" and "offset" in e:
            nm = ordinals.get(f"{mod}.{e['ordinal']}")
            if nm and re.match(r"^[A-Za-z_]\w*$", nm):
                out[(e["seg"], e["offset"])] = nm
    return out


def calls_in_binary(sg, start, limit=4000, byaddr=None, seg=None):
    """Every call the binary makes inside the function at `start`.

    The function's extent comes from the CARVE, not from the first `ret` — a
    window procedure returns from every arm of its dispatch, so stopping at the
    first return would see almost nothing and report the rest as ghosts.
    """
    # A moveable-segment export is often a THUNK: a couple of bytes that jump
    # to the real body. The reconstruction cites the entry address, so follow
    # the jump before measuring, or the body looks empty and everything the C
    # says would be reported as a ghost.
    for _ in range(3):
        probe = trace.disasm_whole(sg.code[start:start + 16], base=start)[:4]
        jmps = [r for r in probe if r[1].startswith("jmp")]
        if jmps and (jmps[0][0] - start) <= 6:
            m = re.match(r"jmp\s+0x([0-9a-f]+)$", jmps[0][1])
            # ...but only if it jumps somewhere ELSE. A thunk jumps out of its
            # own two or three bytes into a different function; a function that
            # merely OPENS with a jump into its own body (a while loop whose
            # test is at the bottom — `jmp test` is how every 1985 compiler
            # emits one) is not a thunk, and following it skips everything
            # above the test. That misfire made CALENDAR's print-abort message
            # pump look as if it called nothing, and reported its four real
            # calls as ghosts.
            here = sg.funcs.get(start)
            if m and int(m.group(1), 16) != start and not (
                    here and here["start"] <= int(m.group(1), 16) < here["end"]):
                start = int(m.group(1), 16)
                continue
        break
    f = sg.funcs.get(start)
    if f:
        end = f["end"]
    else:
        later = [s for s in sorted(sg.funcs) if s > start]
        end = later[0] if later else len(sg.code)
    # A very short "function" that does not end in a return is an ENTRY that
    # falls through into a shared body — the one-body-two-entries idiom this
    # binary uses everywhere (GetMessage/PeekMessage, CheckMenuItem/
    # EnableMenuItem, ClientToScreen/ScreenToClient). Follow the fall-through,
    # or the entry looks like it calls nothing at all.
    while end - start < 12:
        probe = trace.disasm_whole(sg.code[start:end], base=start)
        if probe and probe[-1][1].startswith(("ret", "jmp")):
            break
        later = [s for s in sorted(sg.funcs) if s >= end]
        if not later:
            break
        nxt = sg.funcs[later[0]]
        end = nxt["end"]
    rows = trace.disasm_whole(sg.code[start:end], base=start)[:limit]

    # THE FUNCTION ENDS AT THE FIRST RETURN THAT NOTHING JUMPS PAST.
    #
    # The carve runs start-to-next-start, so a function followed by code that
    # none of trace.py's discovery rules can see absorbs it — and every call
    # that neighbour makes is then reported as missing from THIS function.
    # CONTROL's ControlDialogBox (seg1:0x157b, a `retf 0xc` at 0x15ba) was
    # being charged with FatalExit, InitApp, InitTask and WaitEvent, which
    # belong to the C startup stub sitting behind it.
    #
    # Truncating at the first `ret` is the obvious fix and it is WRONG in
    # general: a window procedure returns from every arm of its dispatch, and
    # cutting at the first one would hide almost the whole body (that mistake
    # is recorded in this file's own history). The guard is what makes it
    # safe — stop only at a return that no branch inside the function reaches
    # past. A dispatcher always has targets beyond its first return; a
    # finished function never does.
    # Only branches SEEN SO FAR count: a jump that appears after the candidate
    # return belongs to whatever code follows, and letting it vote lets the
    # absorbed neighbour keep the function alive (which is exactly what
    # happened here — the startup stub's own jumps reached "past" the retf).
    inside = set()
    for i, (off, asm) in enumerate(rows):
        if asm.startswith("ret") and not any(t > off for t in inside):
            rows = rows[:i + 1]
            break
        m = re.match(r"(?:j\w+|call)\s+0x([0-9a-f]+)$", asm)
        if m:
            t = int(m.group(1), 16)
            if start <= t < end:
                inside.add(t)

    names = []
    for off, asm in rows:
        if asm.startswith("call") or asm.startswith("jmp"):
            kind, det = sg.resolve_call(off, asm)
            if kind == "sys":
                lbl, nm = det
                names.append(nm or lbl)
            elif kind == "int" and byaddr:
                nm = byaddr.get((seg, det))
                if nm:
                    names.append(nm)
            elif kind == "far":
                d = str(det)
                m2 = re.match(r"seg(\d+):0x([0-9a-f]+)$", d)
                if m2 and byaddr:
                    nm = byaddr.get((int(m2.group(1)), int(m2.group(2), 16)))
                    names.append(nm or d)
                else:
                    names.append(d)
    return names


def main():
    strict = "--strict" in sys.argv
    pick = [a for a in sys.argv[1:] if not a.startswith("--")]
    ordinals = trace.load_ordinals("ORDINALS.md")
    known = {v for v in ordinals.values() if v and re.match(r"^[A-Za-z_]\w*$", v)}

    ghosts = missing_full = checked = skipped = unresolved = 0
    cache = {}

    for f in sorted(glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c"))):
        base = f.replace("\\", "/").split("/")[-1][:-2]
        if pick and not any(p in base for p in pick):
            continue
        src = open(f, encoding="utf-8", errors="replace").read()

        mod = next((m for p, m in SYS.items() if base.startswith(p)), None)
        app = APPS.get(base)
        if not mod and not app:
            continue
        try:
            if mod:
                ne = cache.setdefault(mod, idord.open_module(mod))
            else:
                ne = cache.setdefault(app, open_ne(f"files/WF/{app}.EXE"))
        except Exception:
            continue

        for name, args, body, head in bodies(src):
            m = re.search(r"seg(\d+):0x([0-9a-fA-F]{1,4})", head)
            if not m:
                skipped += 1
                continue
            seg, off = int(m.group(1)), int(m.group(2), 16)
            key = (id(ne), seg)
            if key not in cache:
                try:
                    cache[key] = trace.Seg(ne, seg, ordinals)
                except Exception:
                    cache[key] = None
            sg = cache[key]
            if sg is None or off >= len(sg.code):
                skipped += 1
                continue

            # A cited address that is not a function start means the citation
            # is wrong (or points into another segment). That is a finding about
            # the COMMENT, not about the code, so report it as its own category
            # instead of blaming every call in the body on it.
            # An exported entry is a function start BY DEFINITION, even when
            # the linear disassembly pass drifted past it (embedded data
            # desynchronises the decoder — the same reason mfn.py has --at).
            entries = cache.setdefault(("ents", id(ne)),
                                       {(e.get("seg"), e.get("offset"))
                                        for e in ne.entries() if "offset" in e})
            if (off not in sg.funcs and off not in sg.starts
                    and (seg, off) not in entries):
                print(f"  unresolved  {base}:{name}()  cites seg{seg}:0x{off:04x}, "
                      f"which is not a function start in the binary")
                unresolved += 1
                continue

            byaddr = cache.setdefault(("byaddr", mod or app),
                                      build_byaddr(ne, mod or app, ordinals))
            real = calls_in_binary(sg, off, byaddr=byaddr, seg=seg)
            realset = set(real)
            code = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
            used = {c for c in re.findall(r"\b([A-Za-z_]\w*)\s*\(", code) if c in known}
            partial = "documented partial" in body.lower()
            checked += 1

            for c in sorted(used - realset):
                print(f"  GHOST    {base}:{name}()  calls {c}(), but seg{seg}:0x{off:04x} "
                      f"does not — the binary calls: {', '.join(sorted(realset)) or '(nothing)'}")
                ghosts += 1
            if not partial or strict:
                for c in sorted(realset - used):
                    if c in known:
                        print(f"  missing  {base}:{name}()  omits {c}(), which "
                              f"seg{seg}:0x{off:04x} does call")
                        missing_full += 1

    print(f"\nchecked {checked} functions ({skipped} had no address citation)")
    print(f"GHOST calls (errors): {ghosts}")
    print(f"missing calls in non-partial functions: {missing_full}")
    if ghosts == 0:
        print("no reconstruction claims a call the binary does not make.")


if __name__ == "__main__":
    main()
