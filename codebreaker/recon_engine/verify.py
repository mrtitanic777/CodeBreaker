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
import glob, re, sys, collections
from codebreaker.foundation import cores
idord = cores.get('idord')
trace = cores.get('trace')
target = cores.get('target')
from codebreaker.coverage import ledger
from codebreaker.format_loaders import pe_trace
from codebreaker.foundation import layout
from codebreaker.disasm_engine.disasm import open_ne

FNDEF = re.compile(r"^([A-Za-z_][A-Za-z0-9_ \t\*]*?\b)([A-Za-z_]\w*)\s*\(([^;{]*)\)\s*\{", re.M)
SYS = {"kernel_": "KERNEL", "gdi_": "GDI", "user_": "USER", "comm": "COMM",
       "keyboard": "KEYBOARD", "mouse": "MOUSE", "sound": "SOUND",
       "system": "SYSTEM", "display": "DISPLAY",
       # system DLLs — module names target._resolve maps to files/*.DLL
       "toolhelp_": "TOOLHELP.DLL", "ver": "VER.DLL",
       "lzexpand": "LZEXPAND.DLL", "win87em": "WIN87EM.DLL",
       "shell": "SHELL.DLL", "commdlg": "COMMDLG.DLL", "mmsystem": "MMSYSTEM.DLL",
       "ddeml": "DDEML.DLL", "olecli": "OLECLI.DLL", "olesvr": "OLESVR.DLL",
       "kernel286": "KRNL286.EXE", "winoa386": "WINOA386.MOD"}
APPS = {"calc": "CALC", "calendar": "CALENDAR", "cardfile": "CARDFILE",
        "clipbrd": "CLIPBRD", "clock": "CLOCK", "control": "CONTROL",
        "notepad": "NOTEPAD", "paint_core": "PAINT", "paint_tools": "PAINT",
        "reversi": "REVERSI", "spooler": "SPOOLER", "terminal": "TERMINAL",
        "write_wnd": "WRITE", "write_dlg": "WRITE", "write": "WRITE",
        "winmine": "WINMINE",
        "sol": "SOL", "progman": "PROGMAN", "winfile": "WINFILE"}


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
        head = src[max(0, m.start() - 2500):m.start()]
        blocks = re.findall(r"/\*.*?\*/", head, flags=re.S)
        cite = ""
        for b in reversed(blocks):
            if re.search(re.escape(name), b, re.I) and re.search(r"(?:seg\d+|rva):0x", b):
                # A block may document several functions at once ("SetDlgItemText
                # (…0x04fa) and GetDlgItemText (…0x052f)"). Take the address
                # NEAREST this function's name, not the first in the block.
                nm = re.search(re.escape(name), b, re.I)
                addrs = list(re.finditer(r"(?:seg\d+|rva):0x[0-9a-fA-F]{1,8}", b))
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
            if m and int(m.group(1), 16) != start:
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


def arg_bytes(argtext):
    """(stack bytes the args occupy, determinable?) for a far-PASCAL signature.

    16-bit stack model: a far pointer (has `*`) or a 32-bit scalar (long/DWORD)
    is 4 bytes; everything word-sized (int/short/WORD/char/BYTE/handle/BOOL) is
    2. A parameter type we don't recognise makes the whole thing indeterminate
    (return False) so the advisory stays silent rather than guessing — this is
    an advisory, and a wolf-crying advisory is worthless.
    """
    at = argtext.strip()
    if at == "" or at == "void":
        return 0, True
    total = 0
    for p in at.split(","):
        p = p.strip()
        if not p:
            continue
        if "*" in p:
            total += 4                                   # far pointer
        elif re.search(r"\b(long|LONG|DWORD|dword|LPARAM|LRESULT|COLORREF)\b", p):
            total += 4
        elif re.search(r"\b(int|short|unsigned|char|WORD|word|BYTE|byte|UINT|"
                       r"BOOL|WPARAM|ATOM|HWND|HDC|HANDLE|HMENU|HINSTANCE|HGDIOBJ|"
                       r"HBITMAP|HRGN|HPEN|HBRUSH|HFONT|HGLOBAL|HLOCAL|HTASK)\b", p):
            total += 2
        else:
            return 0, False                              # unknown -> indeterminate
    return total, True


def retf_of(sg, start):
    """(N, is_far) for the terminal return of the function at `start`, following
    a leading tail-jmp thunk. Only far `retf N` is reported as far; near `ret`
    (caller-cleans C convention) returns is_far=False so the advisory skips it."""
    s = start
    for _ in range(3):
        probe = trace.disasm_whole(sg.code[s:s + 16], base=s)[:4]
        jmps = [r for r in probe if r[1].startswith("jmp")]
        if jmps and (jmps[0][0] - s) <= 6:
            m = re.match(r"jmp\s+0x([0-9a-f]+)$", jmps[0][1])
            if m and int(m.group(1), 16) != s:
                s = int(m.group(1), 16)
                continue
        break
    f = sg.funcs.get(s)
    end = f["end"] if f else min([x for x in sorted(sg.funcs) if x > s],
                                 default=len(sg.code))
    for off, asm in trace.disasm_whole(sg.code[s:end], base=s):
        m = re.match(r"retf\s+0x([0-9a-f]+)$", asm)
        if m:
            return int(m.group(1), 16), True
        if asm == "retf":
            return 0, True
        m = re.match(r"ret\s+0x([0-9a-f]+)$", asm)
        if m:
            return int(m.group(1), 16), False
        if asm == "ret":
            return 0, False
    return None, False


def main():
    strict = "--strict" in sys.argv
    pick = [a for a in sys.argv[1:] if not a.startswith("--")]
    ordinals = trace.load_ordinals("ORDINALS.md")
    known = {v for v in ordinals.values() if v and re.match(r"^[A-Za-z_]\w*$", v)}
    # NE export names are stored UPPER-CASE in the binary's name tables, while
    # reconstruction C uses the conventional mixed case (GlobalAlloc). Match
    # case-insensitively or every real call reads as a mismatch.
    known_u = {k.upper() for k in known}

    ghosts = missing_full = checked = skipped = unresolved = argadv = 0
    cache = {}

    # 95/98-era change: a reconstruction declares its module with a `MODULE:` banner
    # (as the ledger reads it), instead of the 3.1 SYS/APPS filename-prefix maps.
    # Module names collide across formats, so the banner is the only safe key.
    # Only NE modules are verifiable here — `trace.Seg` is a 16-bit disassembler;
    # PE/LE reconstructions need a 32-bit/LE carver first (PLAN.md), so they are
    # skipped (not failed). The behaviour we care about is the NE core anyway.
    # (win98 also verifies .c carried over from its win95 base; the from95/ glob
    # is a no-op on eras without that directory.)
    for f in layout.written_c_files():
        base = f.replace("\\", "/").split("/")[-1][:-2]
        if pick and not any(p in base for p in pick):
            continue
        src = open(f, encoding="utf-8", errors="replace").read()

        owner = ledger.owner_of(f, src)
        if not owner:
            continue
        fmt = target.format_of(owner)

        # ---- PE path: 32-bit modules, carved & call-resolved by pe_trace ----
        # A PE reconstruction cites `rva:0xADDR`; the GHOST check compares the
        # C body's API calls to the calls pe_trace resolves at that function
        # (imports `DLL!func` and named exports). Same doctrine as the NE path.
        if fmt == "PE":
            key = ("pe", owner)
            if key not in cache:
                try:
                    cache[key] = pe_trace.PeCarve(owner)
                except Exception:
                    cache[key] = None
            c = cache[key]
            if c is None:
                continue
            kp = cache.setdefault(("pe_known", owner),
                                  {nm.upper() for _dll, nm in c.imports.values()}
                                  | {nm.upper() for nm in c.exports.values()})
            for name, args, body, head in bodies(src):
                m = re.search(r"rva:0x([0-9a-fA-F]{1,8})", head)
                if not m:
                    skipped += 1
                    continue
                va = c.pe.image_base + int(m.group(1), 16)
                if va not in c.funcs:
                    print(f"  unresolved  {base}:{name}()  cites rva:0x{int(m.group(1),16):x}, "
                          f"not a function start in the binary")
                    unresolved += 1
                    continue
                real = set()
                for call in c.calls_in(va):
                    if "!" in call:                       # DLL!func import
                        real.add(call.split("!", 1)[1].upper())
                    elif not call.startswith("sub_"):     # named export
                        real.add(call.upper())
                code = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
                used = {x.upper() for x in re.findall(r"\b([A-Za-z_]\w*)\s*\(", code)
                        if x.upper() in kp}
                partial = "documented partial" in body.lower()
                checked += 1
                for cc in sorted(used - real):
                    print(f"  GHOST    {base}:{name}()  calls {cc}(), but rva:0x{int(m.group(1),16):x} "
                          f"does not - binary calls: {', '.join(sorted(real)) or '(nothing)'}")
                    ghosts += 1
                if not partial or strict:
                    for cc in sorted(real - used):
                        print(f"  missing  {base}:{name}()  omits {cc}(), which "
                              f"rva:0x{int(m.group(1),16):x} does call")
                        missing_full += 1
            continue

        if fmt != "NE":
            continue
        try:
            ne = cache.setdefault(owner, target.open_module(owner))
        except Exception:
            continue
        modname = ne.module_name()

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

            byaddr = cache.setdefault(("byaddr", modname),
                                      build_byaddr(ne, modname, ordinals))
            real = calls_in_binary(sg, off, byaddr=byaddr, seg=seg)
            realset = {r.upper() for r in real}          # binary calls, upper-cased
            code = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
            used = {c.upper() for c in re.findall(r"\b([A-Za-z_]\w*)\s*\(", code)
                    if c.upper() in known_u}             # cited API calls, upper-cased
            partial = "documented partial" in body.lower()
            checked += 1

            # ADVISORY (never a gate): for a far-PASCAL export the callee cleans
            # the stack, so `retf N` must equal the argument bytes. A mismatch
            # means the reconstructed signature is wrong (extra/missing/mis-sized
            # param). Only fires when both sides are determinable AND the binary
            # really uses a far retf — near `ret` (C convention) is skipped.
            declared, ok = arg_bytes(args)
            rn, is_far = retf_of(sg, off)
            if ok and is_far and rn is not None and declared != rn and not partial:
                print(f"  argbytes {base}:{name}()  signature = {declared} arg-bytes "
                      f"but seg{seg}:0x{off:04x} does retf {rn} - check the parameter list")
                argadv += 1

            for c in sorted(used - realset):
                print(f"  GHOST    {base}:{name}()  calls {c}(), but seg{seg}:0x{off:04x} "
                      f"does not - the binary calls: {', '.join(sorted(realset)) or '(nothing)'}")
                ghosts += 1
            if not partial or strict:
                for c in sorted(realset - used):
                    if c in known_u:
                        print(f"  missing  {base}:{name}()  omits {c}(), which "
                              f"seg{seg}:0x{off:04x} does call")
                        missing_full += 1

    print(f"\nchecked {checked} functions ({skipped} had no address citation)")
    print(f"GHOST calls (errors): {ghosts}")
    print(f"missing calls in non-partial functions: {missing_full}")
    print(f"argbytes advisories (signature vs far retf N): {argadv}  [advisory only]")
    if ghosts == 0:
        print("no reconstruction claims a call the binary does not make.")


if __name__ == "__main__":
    main()
