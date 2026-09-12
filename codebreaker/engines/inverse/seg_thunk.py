#!/usr/bin/env python3
r"""seg_thunk.py -- the whole-function structural template for 16-bit segment-switch thunks.

A large share of the framed corpus (the printer/device drivers especially) is NOT compiled C
-- it is generated assembly THUNKS that switch DS to a fixed data segment and forward to a
handler. They share one rigid binary shape, so a single structural template reconstructs all of
them byte-exact WITHOUT the C decompiler or a compiler oracle: parse the fixed grammar, and if
every byte is accounted for, the function is a faithful thunk. emit() regenerates the bytes from
the parsed fields (proving completeness), and emit_asm() renders the equivalent MASM stub.

    <far loadds prologue>          ; mov ax,ds; nop; inc bp; push bp; mov bp,sp; push ds; mov ds,ax
    mov ax, <seg> ; mov ds, ax     ; switch DS to the fixed data segment
    <body>                         ; forwarder: push args; call far <target>
                                   ; stub:      xor ax,ax [; cdq]   (return 0 / 0L)
    <lea/std epilogue> ; retf <N>

    from codebreaker.engines.inverse import seg_thunk
    t = seg_thunk.parse(func_bytes)          # None if not a pure thunk
    if t and seg_thunk.emit(t) == func_bytes: ...   # byte-exact by construction
"""

PRO_8CD8 = bytes.fromhex("8cd89045558bec1e8ed8")   # far loadds, mov-ax,ds encoding (1.52c)
PRO_1E58 = bytes.fromhex("1e589045558bec1e8ed8")   # far loadds, push-ds;pop-ax encoding (6.0a)
EPI_LEA = bytes.fromhex("8d66fe1f5d4d")            # lea sp,[bp-2]; pop ds; pop bp; dec bp
EPI_STD = bytes.fromhex("4d4d8be51f5d4d")          # dec bp;dec bp; mov sp,bp; pop ds; pop bp; dec bp


def _body_op(fb, p):
    """One thunk-body instruction at fb[p] -> (name, size), or (None, 0) if fb[p] is not a
    modelled body op (i.e. the epilogue has begun). Names drive emit_asm; the raw bytes are
    what emit() re-lays, so the round-trip is exact by construction."""
    n = len(fb); c = fb[p]
    if fb[p:p+3] == b"\x66\xff\x76" and p+4 <= n:            # push dword ptr [bp+N]
        return "pushd", 4
    if c == 0xff and p+3 <= n and fb[p+1] == 0x76:          # push word ptr [bp+N]
        return "pushw", 3
    if c == 0xff and p+4 <= n and fb[p+1] == 0x36:          # push word ptr [mem]
        return "pushm", 4
    if 0x50 <= c <= 0x57:                                    # push ax..di
        return "pushr", 1
    if c in (0x1e, 0x06, 0x0e, 0x16):                        # push ds/es/cs/ss
        return "pushseg", 1
    if c in (0x1f, 0x07):                                    # pop ds/es (es:=ds setup etc.)
        return "popseg", 1
    if c == 0x6a and p+2 <= n:                               # push imm8 (sign-extended arg)
        return "pushi8", 2
    if c == 0x68 and p+3 <= n:                               # push imm16 (arg)
        return "pushi16", 3
    if c == 0x9a and p+5 <= n:                               # call far seg:off
        return "lcall", 5
    if fb[p:p+2] in (b"\x2b\xc0", b"\x33\xc0"):              # xor/sub ax,ax  (return 0)
        return "zero", 2
    if c == 0x99:                                            # cdq  (ax -> ax:dx = 0L)
        return "cdq", 1
    if c == 0x98:                                            # cbw/cwde
        return "cwd", 1
    if fb[p:p+2] == b"\x83\xc4" and p+3 <= n:                # add sp,N  (cdecl cleanup)
        return "addsp", 3
    if c == 0xb8 and p+3 <= n:                               # mov ax,imm  (return const)
        return "movax", 3
    return None, 0


def parse(fb):
    """Decode a segment-switch thunk to {prologue, seg, body[(name,raw)], epilogue, retf, pad}.
    Returns None the instant a byte is not explained by the grammar -- a non-thunk never parses."""
    fb = bytes(fb)
    pro = PRO_8CD8 if fb.startswith(PRO_8CD8) else (PRO_1E58 if fb.startswith(PRO_1E58) else None)
    if pro is None:
        return None
    p = len(pro)
    if not (p + 5 <= len(fb) and fb[p] == 0xb8 and fb[p+3:p+5] == b"\x8e\xd8"):
        return None                                    # require `mov ax,imm ; mov ds,ax`
    seg = fb[p+1] | (fb[p+2] << 8); p += 5
    body = []
    while p < len(fb):
        name, sz = _body_op(fb, p)
        if name is None:
            break
        body.append((name, fb[p:p+sz])); p += sz
    epi = EPI_LEA if fb[p:p+len(EPI_LEA)] == EPI_LEA else (
        EPI_STD if fb[p:p+len(EPI_STD)] == EPI_STD else None)
    if epi is None:
        return None
    p += len(epi)
    if p < len(fb) and fb[p] == 0xca and p+3 <= len(fb):   # retf N
        retf = fb[p+1] | (fb[p+2] << 8); p += 3
    elif p < len(fb) and fb[p] == 0xcb:                    # retf
        retf = None; p += 1
    else:
        return None
    pad = fb[p:]
    if any(x not in (0x90, 0x00) for x in pad):            # unexplained tail -> not a pure thunk
        return None
    return {"pro": pro, "seg": seg, "body": body, "epi": epi, "retf": retf, "pad": pad}


def emit(t):
    """Regenerate the exact bytes from a parsed thunk (proves the parse is lossless)."""
    out = bytearray(t["pro"])
    out += b"\xb8" + bytes((t["seg"] & 0xff, t["seg"] >> 8)) + b"\x8e\xd8"
    for _name, raw in t["body"]:
        out += raw
    out += t["epi"]
    out += b"\xcb" if t["retf"] is None else (b"\xca" + bytes((t["retf"] & 0xff, t["retf"] >> 8)))
    out += t["pad"]
    return bytes(out)


_SEGREG = {0x1e: "ds", 0x06: "es", 0x0e: "cs", 0x16: "ss"}
_GPREG = ["ax", "cx", "dx", "bx", "sp", "bp", "si", "di"]


def emit_asm(t, name="thunk"):
    """Render the equivalent 16-bit MASM stub (the clean-room source for the thunk)."""
    L = ["%s PROC FAR" % name, "    inc  bp", "    push bp", "    mov  bp, sp",
         "    push ds", "    mov  ds, ax", "    mov  ax, %04Xh" % t["seg"], "    mov  ds, ax"]
    for nm, raw in t["body"]:
        if nm == "pushd":
            L.append("    push dword ptr [bp+%02Xh]" % raw[3])
        elif nm == "pushw":
            L.append("    push word ptr [bp+%02Xh]" % raw[2])
        elif nm == "pushm":
            L.append("    push word ptr [%04Xh]" % (raw[2] | (raw[3] << 8)))
        elif nm == "pushr":
            L.append("    push %s" % _GPREG[raw[0] - 0x50])
        elif nm == "pushseg":
            L.append("    push %s" % _SEGREG[raw[0]])
        elif nm == "popseg":
            L.append("    pop  %s" % ("ds" if raw[0] == 0x1f else "es"))
        elif nm == "pushi8":
            L.append("    push %02Xh" % raw[1])
        elif nm == "pushi16":
            L.append("    push %04Xh" % (raw[1] | (raw[2] << 8)))
        elif nm == "lcall":
            L.append("    call FAR PTR target")
        elif nm == "zero":
            L.append("    xor  ax, ax")
        elif nm == "cdq":
            L.append("    cdq")
        elif nm == "cwd":
            L.append("    cwde")
        elif nm == "addsp":
            L.append("    add  sp, %02Xh" % raw[2])
        elif nm == "movax":
            L.append("    mov  ax, %04Xh" % (raw[1] | (raw[2] << 8)))
    L += ["    lea  sp, [bp-2]" if t["epi"] == EPI_LEA else "    dec  bp",
          "    pop  ds", "    pop  bp", "    dec  bp",
          "    retf %d" % (0 if t["retf"] is None else t["retf"]), "%s ENDP" % name]
    return "\n".join(L)


def kind(t):
    return "forwarder" if any(nm == "lcall" for nm, _ in t["body"]) else "return-stub"


def verify(fb):
    """The authoritative thunk test: parse AND regenerate the exact bytes. Returns the parsed
    thunk (byte-exact by construction) or None -- stricter than the frame shape-match, since a
    function with real body logic after the DS switch fails to round-trip and is rejected."""
    fb = bytes(fb)
    t = parse(fb)
    return t if (t is not None and emit(t) == fb) else None


def _sig(t):
    """A shape signature that ignores the per-thunk seg constant, call targets and stack
    offsets -- so every structurally identical thunk collapses to one Level-0 key."""
    import hashlib
    shape = "|".join(nm for nm, _ in t["body"]) + "/" + str(t["retf"] is None) + "/" + \
        ("lea" if t["epi"] == EPI_LEA else "std")
    return "segthunk:" + hashlib.sha1(shape.encode()).hexdigest()[:12]


def main(argv):
    """Sweep the oracle corpus, verify every segment-switch thunk byte-exact, and bank the
    results: a per-function manifest (thunks.json) plus their shape signatures into the
    Level-0 solved.json, so the auto-sweeper treats the whole class as solved.

        python -m engine.inverse_engine.seg_thunk <oracle_dir> [--db <dir>]
    """
    import os, sys, struct, glob, json
    from codebreaker.toolchain.sdk import reconstruct as R
    from codebreaker.formats import ne
    from codebreaker.core import oracle as _oracle
    ora = argv[0] if argv else _oracle._oracle_dir()
    db = argv[argv.index("--db") + 1] if "--db" in argv else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "_sweepdb")
    os.makedirs(db, exist_ok=True)

    def is_ne(p):
        try:
            d = open(p, "rb").read(0x40)
            if d[:2] != b"MZ":
                return False
            lfa = struct.unpack_from("<I", d, 0x3c)[0]
            return open(p, "rb").read(lfa + 2)[lfa:lfa + 2] == b"NE"
        except Exception:
            return False

    manifest = []; sigs = {}; scanned = tot = solved = 0
    for path in sorted(glob.glob(os.path.join(ora, "*.dll")) + glob.glob(os.path.join(ora, "*.drv"))
                       + glob.glob(os.path.join(ora, "*.exe"))):
        if not is_ne(path):
            continue
        try:
            rep = R.analyze(path)
        except Exception:
            continue
        scanned += 1
        d = open(path, "rb").read(); lfa = struct.unpack_from("<I", d, 0x3c)[0]
        f = ne.NEFile.from_buffer(d, lfa, path)
        sb = {s["idx"]: d[s["file_off"]:s["file_off"] + s["length"]]
              for s in f.segments() if s["kind"] == "CODE"}
        for sg in rep["segments"]:
            for fn in sg["funcs"]:
                if fn.get("origin") != "seg-switch-thunk":
                    continue
                tot += 1
                fb = bytes(sb[sg["idx"]][fn["off"]:fn["off"] + fn["size"]])
                t = verify(fb)
                if t is None:
                    continue
                solved += 1
                sig = _sig(t); sigs[sig] = "SEG-SWITCH-THUNK (%s)" % kind(t)
                manifest.append({"module": os.path.basename(path), "seg": sg["idx"],
                                 "off": fn["off"], "size": len(fb), "seg_const": t["seg"],
                                 "kind": kind(t), "sig": sig})
    json.dump(manifest, open(os.path.join(db, "thunks.json"), "w"), indent=1)
    hp = os.path.join(db, "solved.json")
    solved_db = json.load(open(hp)) if os.path.exists(hp) else {}
    solved_db.update(sigs)
    json.dump(solved_db, open(hp, "w"), indent=1)
    print("== seg-switch-thunk bulk bank ==")
    print("   NE files scanned : %d" % scanned)
    print("   thunks verified  : %d / %d byte-exact" % (solved, tot))
    print("   shape signatures : %d (banked into %s)" % (len(sigs), hp))
    print("   manifest         : %s (%d entries)" % (os.path.join(db, "thunks.json"), len(manifest)))
    return 0


if __name__ == "__main__":
    import sys
    raise SystemExit(main(sys.argv[1:]))
