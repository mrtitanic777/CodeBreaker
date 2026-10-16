#!/usr/bin/env python3
"""newcomp.py - is a procedure of MASM 1.10 the kit compiler's code but for the later compiler's habits?

MASM 1.10's main compiland was built by a state of the MS-Pascal compiler later than 2.05 and
earlier than 3.04 (neither gives its code; tried, with the kits of Seattle Computer Products).
For the same source it differs from the compiler of 1981 in four habits:

  a value worked     later:  mov ax,M  [xor ah,ah]  shl ax,1  add ax,k ..  then  xchg R,ax,  or
  out in AX                  push ax,  or  mov [m],al,  or  add dx,ax ..
                     1981:   mov R,M   [and R,0FFh | xor Rh,Rh]  shl R,1  add R,k ..  push R ..
                     The later compiler loads a value into AX (the encodings are shorter), or
                     moves it there (xchg R,ax), and computes there; where the result is wanted in
                     another register it is exchanged into it. The compiler of 1981 computes in the
                     register itself. So for a stretch the two keep the same values in other
                     registers: the one's AX is the other's DX, and - AX being taken - what the one
                     then puts in DX the other has in CX.

  DISPOSE(P)         later:  the address of P pushed, call far 0CA2:0250 (the library frees the block)
                     1981:   mov si,[P]  or byte ptr [si-2],1   (the block's free bit set in line)

  FOR I := c1 TO c2  later:  xor dx,dx  shr dx,1  jb past the loop   (the entry test, on constants)
                     1981:   nothing: the constants say the loop runs

  CASE               later:  sub bx,lo  ja +3  jmp case-lo  shl bx,1  jmp cs:[bx+table]
                     1981:   sub bx,lo  shl bx,1  jmp cs:[bx+table]

  a shift count      later:  the value into AX, then  mov cx,n  shl ax,cl
                     1981:   mov cx,n  first, then the value into R,  shl R,cl

  a pointer in DX    later:  mov di,dx   (the value is still there from the statement before)
                     1981:   the variable loaded again

  an outer frame     later:  mov di,[bp]  mov di,[di]  again before each use
                     1981:   SI still has it from the statement before

  VAR argument       an outer procedure's variable passed by reference right after an assignment to it:
                     the compiler of 1981 pushes its value (an error; a call put between makes it
                     push the address), the later one pushes the address

  MUL                1981:   mov [temporary],dx  before it, never read again
                     later:  nothing

  temporaries        the two do not need the same number of them: the frame's size may differ and a
                     temporary may have another place in it (always the same one)

equivalent(want, got, fixups) reads the two procedures instruction by instruction. Two
instructions agree if their bytes are equal (but for the bytes a linker fills in), or if, under
the renaming of registers in force, the original's instruction is the compiled one. A renaming
begins only where the original loads into AX what the compiled code loads into another register,
or exchanges a register with AX where the compiled code does nothing; it is changed by an exchange
only one of the two has, and by a load both do into different registers; a call ends it. Every
relative jump must go to the instruction that corresponds. Anything else fails the procedure.
"""
import re, struct
from codebreaker.exact.habits import cmpp, symx, boolx

H = bytes.fromhex
DISPOSE = H("9a5002a20c")                    # call far 0CA2:0250, the heap's routine that frees a block
NUM = re.compile(r"0x[0-9a-f]+|\b\d+\b")
FAM = {"ax": ("ax", "al", "ah"), "cx": ("cx", "cl", "ch"), "dx": ("dx", "dl", "dh"), "bx": ("bx", "bl", "bh"), "si": ("si", None, None), "di": ("di", None, None)}
PART = {p: (f, k) for f, ps in FAM.items() for k, p in enumerate(ps) if p}
REG = re.compile(r"\b(%s)\b" % "|".join(PART))
R16 = "ax|cx|dx|bx|si|di"
TABJMP = H("2effa7")                         # jmp word ptr cs:[bx + table]
DEBUG = False
GWALK = []         # ... and where the compiled code's instructions begin
AMAP = []          # the alignment of the last pair found equivalent: [(offset in the original, offset in the compiled)]

def mask(t): return NUM.sub("N", t).replace("- N", "+ N")

def rel(b):
    """a relative jump or call -> (length of opcode, displacement) or None"""
    if len(b) == 2 and (0x70 <= b[0] <= 0x7F or b[0] in (0xEB, 0xE2, 0xE3)): return 1, b[1] - 256 if b[1] > 127 else b[1]
    if len(b) == 3 and b[0] in (0xE8, 0xE9): return 1, struct.unpack_from("<h", b, 1)[0]
    return None

def rename(t, mp):
    """the original's instruction with the registers renamed; None if it cannot be written (SI and DI have no halves)"""
    bad = []
    def one(m):
        f, k = PART[m.group(1)]; p = FAM[mp.get(f, f)][k]
        if p is None: bad.append(1); return m.group(1)
        return p
    t = REG.sub(one, t)
    return None if bad else t

def equivalent(want, got, fx):
    """-> (True, [what was found]) or (False, [where it stopped])"""
    global AMAP
    # the jump tables of CASE statements are words, not instructions: in the compiled code every one is
    # an address the linker fills in, so their length is known; the original's lie at the same jumps
    tabs = []; k = got.find(TABJMP)
    while k >= 0:
        n = 0
        while (k + 5 + 2 * n) in fx and (k + 6 + 2 * n) in fx: n += 1
        tabs.append(2 * n); k = got.find(TABJMP, k + 5)
    def walk(b):
        out = []; p = 0; t = 0
        while True:
            k = b.find(TABJMP, p)
            if k < 0 or t >= len(tabs): out += [(p + o, n, x, m) for o, n, x, m in cmpp.walk(b[p:], 0, skip_hdr=(p == 0))]; return out
            out += [(p + o, n, x, m) for o, n, x, m in cmpp.walk(b[p:k + 5], 0, skip_hdr=(p == 0))] + [(k + 5, tabs[t], "table of %d" % (tabs[t] // 2), "table")]
            p = k + 5 + tabs[t]; t += 1
    W = walk(want); G = walk(got); wi = gi = 0; amap = []; jumps = []; notes = []; mp = {}; since = 0; fors = []; reload = -1; temps = {}; rtemps = {}; count = None
    fixed = lambda og, sg: any((og + k) in fx for k in range(sg))
    def setmap(new, ow):
        nonlocal mp, since
        new = {k: v for k, v in new.items() if v is not None and k != v}
        if not mp and new: since = ow
        if mp and not new: notes.append("registers used otherwise (%s) from +%04x" % (", ".join("%s for %s" % (k.upper(), v.upper()) for k, v in sorted(mp.items())), since))
        mp = new
    sync = None
    ENDS = (0xE8, 0x9A, 0xE9, 0xEB, 0xC2, 0xC3, 0xCA, 0xCB)
    def stretch():
        """the last resort: from the last call or jump to the next, do the two compute the same (tools/symx.py)?"""
        nonlocal wi, gi, mp
        if sync is None or count is not None: return False
        def end(L, b, i):
            while i < len(L):
                c = b[L[i][0]]
                if L[i][3] == "table": return i
                if 0x70 <= c <= 0x7F and b[L[i][0] + 1] == 1 and i and re.fullmatch(r"mov \w\w, 0", L[i - 1][2]): i += 1; continue
                if c in ENDS or 0x70 <= c <= 0x7F: return i
                i += 1
            return None
        ew = end(W, want, sync[0]); eg = end(G, got, sync[1])
        if ew is None or eg is None or ew < wi or eg < gi or want[W[ew][0]] != got[G[eg][0]] or W[ew][3] == "table": return False
        c = want[W[ew][0]]
        iw = [(x[2], want[x[0]:x[0] + x[1]]) for x in W[sync[0]:ew]]; ig = [(x[2], got[x[0]:x[0] + x[1]]) for x in G[sync[1]:eg]]
        fx_ = {k: {struct.unpack_from("<H", got, x[0] + j)[0] for j in range(x[1] - 1) if (x[0] + j) in fx and (x[0] + j + 1) in fx}
               for k, x in enumerate(G[sync[1]:eg]) if fixed(x[0], x[1])}
        ok, why = symx.same(iw, ig, fx_, regs_too=c not in (0xE8, 0x9A), flags_too=0x70 <= c <= 0x7F)
        if not ok:
            if DEBUG: print('symx +%04x..+%04x: %s' % (W[sync[0]][0], W[ew][0], why))
            return False
        amap[:] = [x for x in amap if x[0] <= W[sync[0]][0]]
        notes.append("the same values, by other registers and temporaries, from +%04x to +%04x" % (W[sync[0]][0], W[ew][0]))
        wi, gi = ew, eg; mp = {}; return True
    def cond():
        """a condition the compiled code works out in full and the original leaves early (tools/boolx.py)"""
        nonlocal wi, gi
        ws, gs = wi, gi
        while ws and gs and W[ws - 1][2] == G[gs - 1][2] and re.match(r"(cmp|test) ", G[gs - 1][2]): ws -= 1; gs -= 1
        eg = None
        for j in range(gi, min(len(G) - 1, gi + 60)):
            if G[j][3] in ("call", "lcall", "ret", "retf", "table"): return False
            if re.fullmatch(r"shr (ax|cx|dx), 1", G[j][2]) and G[j + 1][3] in ("jae", "jb"): eg = j + 2; break
        if eg is None or eg + 1 >= len(G):
            if DEBUG: print('cond +%04x: no end in the compiled code' % W[wi][0])
            return False
        nxt = (mask(G[eg][2]), mask(G[eg + 1][2]))
        ew = next((j for j in range(wi + 1, min(len(W) - 1, wi + 60)) if (mask(W[j][2]), mask(W[j + 1][2])) == nxt), None)
        if ew is None:
            if DEBUG: print('cond +%04x: the original does not come to %r' % (W[wi][0], nxt))
            return False
        def lines(L, b, i0, i1):
            out = []; glob = {}                                        # a variable's address is the linker's: named by the order they come in
            for o, n, x, m in L[i0:i1]:
                r = rel(b[o:o + n])
                if r and b[o] != 0xE8: x = "%s 0x%x" % (m, o + n + r[1])
                else: x = re.sub(r"\[(0x[0-9a-f]+|\d+)\]", lambda mm: "[g%d]" % glob.setdefault(mm.group(1), len(glob)), x)
                out.append((o, x))
            return out
        def bases(L, i):
            d = {"si": "si0", "di": "di0"}
            for r in ("si", "di"):
                for j in range(i - 1, max(-1, i - 40), -1):
                    m = re.fullmatch(r"mov %s, word ptr \[([^\]]+)\]" % r, L[j][2])
                    if m: d[r] = m.group(1); break
                    if re.match(r"(mov|xchg) %s" % r, L[j][2]) or L[j][3] in ("call", "lcall"): break
            return d
        ok, res = boolx.same(lines(W, want, ws, ew), W[ws][0], W[ew][0], lines(G, got, gs, eg), G[gs][0], G[eg][0], bases(W, ws), bases(G, gs))
        if not ok:
            if DEBUG: print("boolx +%04x: %s" % (W[ws][0], res))
            return False
        if res: jumps.append(res)
        notes.append("a condition worked out in full where the original jumps, from +%04x to +%04x" % (W[ws][0], W[ew][0]))
        amap[:] = [x for x in amap if x[0] <= W[ws][0]]
        wi, gi = ew, eg; return True
    while wi < len(W) and gi < len(G):
        ow, sw, tw, _ = W[wi]; og, sg, tg, _ = G[gi]; bw = want[ow:ow + sw]; bg = got[og:og + sg]; amap.append((ow, og))
        if wi and gi:                                                  # behind a call the registers hold nothing; behind a jump the same, if they did before it
            pw, pg = want[W[wi - 1][0]], got[G[gi - 1][0]]
            if pw == pg and (pw in (0xE8, 0x9A) or not mp and (pw in ENDS or 0x70 <= pw <= 0x7F)) and W[wi - 1][3] != "table": sync = (wi, gi)
        fxg = fixed(og, sg); same = sw == sg and all(bw[k] == bg[k] or (og + k) in fx for k in range(sw))
        eq = lambda a, b: a is not None and (a == b or fxg and mask(a) == mask(b))
        rw, rg = rel(bw), rel(bg)
        if rw and rg and bw[0] == bg[0] and not fxg:                   # a jump or a call: where it goes is checked at the end (a call's, by build.py)
            if bw[0] != 0xE8: jumps.append((ow + sw + rw[1], og + sg + rg[1]))
            else: setmap({}, ow)
            wi += 1; gi += 1; continue
        if not same and tw != tg and not mp and cond(): continue
        # FOR with constant bounds: the later compiler still tests them before the loop (xor dx,dx / shr dx,1 / jb past the loop)
        if not same and tw == "xor dx, dx" and wi + 2 < len(W) and W[wi + 1][2] == "shr dx, 1" and want[W[wi + 2][0]] == 0x72:
            j3 = W[wi + 2]; tgt = j3[0] + 2 + (want[j3[0] + 1] - 256 if want[j3[0] + 1] > 127 else want[j3[0] + 1])
            if tgt > j3[0]: fors.append(tgt); setmap({k: v for k, v in mp.items() if 'dx' not in (k, v)}, ow); notes.append("FOR entry test on constants at +%04x" % ow); wi += 3; continue
        if tw == tg and tw.startswith("table of"): wi += 1; gi += 1; continue      # (build.py compares where the entries point)
        # CASE: the later compiler takes the least value by itself (sub bx,lo / ja / jmp to its case) before the jump through the table
        if bw == H("7703") and tg == "shl bx, 1" and wi and W[wi - 1][2].startswith("sub bx, ") and wi + 2 < len(W) and want[W[wi + 1][0]] == 0xE9 and W[wi + 2][2] == "shl bx, 1":
            j3 = W[wi + 1]; fors.append(j3[0] + 3 + struct.unpack_from("<h", want, j3[0] + 1)[0]); notes.append("CASE: least value taken first at +%04x" % ow); wi += 2; continue
        if len(bw) == 2 and bw[0] == 0x76 and tg == "shl bx, 1" and wi and W[wi - 1][2].startswith("sub bx, ") and wi + 1 < len(W) and W[wi + 1][2] == "shl bx, 1":
            fors.append(ow + 2 + rw[1]); notes.append("CASE: least value taken first at +%04x" % ow); wi += 1; continue      # ... or jbe case-lo, when it is near
        # after a DISPOSE through the library DI is lost: the original loads the WITH pointer again, the in-line code still has it
        if reload == wi and not same and tw.startswith("mov di, word ptr [bp") and wi + 1 < len(W) and W[wi + 1][2] == tg:
            notes.append("WITH pointer loaded again after DISPOSE at +%04x" % ow); wi += 1; continue
        # the frame: the two compilers do not need the same number of temporaries, so the frame's size may differ,
        # and a temporary may lie elsewhere in it - but one of the original's is always the same one of the compiled
        if _ == "hdr" and G[gi][3] == "hdr" and len(bw) == 2 and bw != bg:          # (MS-Pascal 3.x: the size alone)
            notes.append("frame of %d bytes, %d as compiled here" % (bw[0] | bw[1] << 8, bg[0] | bg[1] << 8)); wi += 1; gi += 1; continue
        if _ == "hdr" and G[gi][3] == "hdr" and bw[0] == bg[0] and bw != bg:
            notes.append("frame of %d bytes, %d in the 1981 code" % (bw[1] | bw[2] << 8, bg[1] | bg[2] << 8)); wi += 1; gi += 1; continue
        def tempeq(x, y):
            mw = re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]", x or ""); mg = re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]", y)
            if mw and mg and x[:mw.start()] + x[mw.end():] == y[:mg.start()] + y[mg.end():]:
                a, b = int(mw.group(1), 0), int(mg.group(1), 0)
                if a != b and temps.get(a, b) == b and rtemps.get(b, a) == a and all((x < a) == (y < b) for x, y in temps.items() if x != a):
                    temps[a] = b; rtemps[b] = a; return True          # (in the same order: a parameter is not taken for a local)
            return False
        if not same and tempeq(rename(tw, mp) if mp else tw, tg): wi += 1; gi += 1; continue
        # a shift count: the compiler of 1981 loads it into CX before the value to be shifted, the later one after
        # it (the value is in AX by then). Between the two places neither may touch CX.
        mc = re.fullmatch(r"mov cx, (0x[0-9a-f]+|\d+)", tg)
        if count is None and mc and tw != tg and not same and not tw.endswith(", " + mc.group(1)) \
           and any(re.search(r"\b(shl|shr|sal|sar) \w+, cl", g[2]) for g in G[gi + 1:gi + 8]): count = (mc.group(1), ow); gi += 1; continue
        if count is not None:
            if tw == "mov cx, " + count[0] and tg != tw:
                notes.append("shift count loaded after the value at +%04x" % count[1]); count = None; wi += 1; continue
            if re.search(r"\b(cx|cl|ch)\b", tw): return False, ["CX used before the shift count is loaded, at +%04x: original %s, compiled %s" % (ow, tw, tg)]
        # before MUL the compiler of 1981 saves DX in a temporary it never reads again; the later one does not
        m = re.fullmatch(r"mov word ptr (\[bp - \w+\]), dx", tg)
        if m and tw != tg and gi + 1 < len(G) and G[gi + 1][2].startswith("mul ") and sum(1 for g in G if m.group(1) in g[2]) == 1:
            notes.append("DX not saved before MUL at +%04x" % ow); gi += 1; continue
        if not mp and same: wi += 1; gi += 1; continue
        xw = re.fullmatch(r"xchg (%s), (%s)" % (R16, R16), tw); xg = re.fullmatch(r"xchg (%s), (%s)" % (R16, R16), tg)
        lw = re.fullmatch(r"mov (%s), (.+)" % R16, tw); lg = re.fullmatch(r"mov (%s), (.+)" % R16, tg)
        if mp:
            old = lambda x: mp[x] if x in mp else (None if x in mp.values() else x)     # what the compiled code has for the original's register x; None: nothing
            if same and tw.startswith("lcall"): setmap({}, ow); wi += 1; gi += 1; continue
            if eq(rename(tw, mp), tg):                                 # the same instruction, the registers renamed
                wi += 1; gi += 1; continue
            m = re.fullmatch(r"xor (ah|ch|dh|bh), \1", tw)              # the high byte cleared: SI and DI have none
            if m and tg == "and %s, 0xff" % mp.get(m.group(1)[0] + "x", "?"): wi += 1; gi += 1; continue
            m = re.fullmatch(r"mov (%s), ax" % R16, tw)                 # the value put aside, AX wanted for the next term
            if m and tw != tg and wi + 1 < len(W) and W[wi + 1][2].startswith("mov ax, ") and old("ax") is not None:
                new = dict(mp); new[m.group(1)] = old("ax"); setmap(new, ow); wi += 1; continue
            ma = re.fullmatch(r"add (%s), (%s)" % (R16, R16), tw); mb = re.fullmatch(r"add (%s), (%s)" % (R16, R16), tg)
            if ma and mb and old(ma.group(1)) == mb.group(2) and old(ma.group(2)) == mb.group(1):      # a sum, the terms the other way round
                new = {k: v for k, v in mp.items() if k not in ma.groups() and v not in mb.groups()}; new[ma.group(1)] = mb.group(1); setmap(new, ow); wi += 1; gi += 1; continue
            m = re.fullmatch(r"mov (%s), (0x[0-9a-f]+|\d+)" % R16, tw)   # a constant loaded before a store the compiled code must do first,
            if m and wi + 1 < len(W) and gi + 1 < len(G) and G[gi + 1][2] == tw and (eq(rename(W[wi + 1][2], mp), tg) or tempeq(rename(W[wi + 1][2], mp), tg)) \
               and re.match(r"mov (byte|word) ptr \[bp", tg):            # because it has the value in the register the constant goes to
                setmap({k: v for k, v in mp.items() if v != m.group(1) and k != m.group(1)}, ow); wi += 2; gi += 2; continue
            if xw:                                                     # an exchange the compiled code has not: the values change places in the original only
                a, b = xw.group(1), xw.group(2); new = dict(mp); new[a], new[b] = old(b), old(a); setmap(new, ow); wi += 1; continue
            if xg:                                                     # ... or one only the compiled code has
                a, b = xg.group(1), xg.group(2); sw_ = {a: b, b: a}; setmap({k: sw_.get(old(k), old(k)) for k in FAM}, ow); gi += 1; continue
            if lw and lg and eq(rename(lw.group(2), mp), lg.group(2)):  # the same value loaded, into different registers
                new = {k: v for k, v in mp.items() if v != lg.group(1)}; new[lw.group(1)] = lg.group(1); setmap(new, ow); wi += 1; gi += 1; continue
            m = re.fullmatch(r"(?:pop|(?:xor|sub) (%s),|lea|mov) (%s)(?:, (.+))?" % (R16, R16), tw)
            if same and m and (m.group(1) in (None, m.group(2))) and (m.group(1) or not m.group(3) or eq(rename(m.group(3), mp), m.group(3))):
                r = m.group(2); setmap({k: v for k, v in mp.items() if k != r and v != r}, ow); wi += 1; gi += 1; continue      # a register set anew by both
            if stretch(): continue
            return False, ["with %s, at +%04x: original %s, compiled %s" % (", ".join("%s for %s" % (k.upper(), v.upper()) for k, v in sorted(mp.items())), ow, tw, tg)]
        # a value loaded into AX where the compiled code loads it into another register
        if lw and lg and lw.group(1) != lg.group(1) and "ptr" in tw and eq(lw.group(2), lg.group(2))            and ("ax" in (lw.group(1), lg.group(1)) or {lw.group(1), lg.group(1)} <= {"cx", "dx", "bx"}):
            setmap({lw.group(1): lg.group(1)}, ow); wi += 1; gi += 1; continue
        # a value moved out of AX, which the next computation wants (the compiled code leaves it there and computes elsewhere)
        m = re.fullmatch(r"mov (cx|dx|bx|si|di), ax", tw)
        if m and tw != tg and wi + 1 < len(W) and W[wi + 1][2].startswith("mov ax, "):
            setmap({m.group(1): "ax"}, ow); wi += 1; continue
        # ... or the compiled code moves the value out of AX to work on it (an index), which the original does in AX
        if xg and "ax" in xg.groups() and tw != tg and not same:
            r = [x for x in xg.groups() if x != "ax"][0]; setmap({"ax": r, r: "ax"}, ow); gi += 1; continue
        # a register's value moved into AX to be worked on there
        if xw and "ax" in xw.groups():
            r = [x for x in xw.groups() if x != "ax"][0]; setmap({"ax": r, r: "ax"}, ow); wi += 1; continue
        # a pointer still in DX: the later compiler takes it from there (mov di,dx), the one of 1981 loads the variable again -
        # with the very instructions that loaded DX a moment ago, and no call between
        if tw == "mov di, dx" and not mp:
            done = False
            for k in (1, 2, 3):
                if gi + k > len(G) or not G[gi + k - 1][2].startswith("mov di, "): continue
                x = G[gi + k - 1][2][8:]
                for j in range(gi - 1, max(gi - 10, k - 2), -1):
                    if G[j][2] == "mov dx, " + x and [g[2] for g in G[gi:gi + k - 1]] == [g[2] for g in G[j - (k - 1):j]] and not any("call" in g[2] for g in G[j:gi]):
                        notes.append("pointer taken from DX at +%04x" % ow); wi += 1; gi += k; done = True; break
                if done: break
            if done: continue
        # ... and the other way: the compiler of 1981 still has an outer procedure's frame in SI and uses it; the later one
        # loads it again, into DI, with the instructions that loaded SI
        if tw.startswith("mov di, word ptr [bp") and not mp and re.search(r"\bsi\b", tg):
            done = False
            for k in (1, 2):
                if wi + k >= len(W): break
                loads = [re.sub(r"\bdi\b", "si", w[2]) for w in W[wi:wi + k]]
                if re.sub(r"\bdi\b", "si", W[wi + k][2]) != tg or not all(l.startswith("mov si, ") for l in loads): continue
                for j in range(gi - k, max(gi - 14, -1), -1):
                    if [g[2] for g in G[j:j + k]] == loads and not any("call" in g[2] or re.match(r"(mov|lea|pop|xchg) si,", g[2]) for g in G[j + k:gi]):
                        notes.append("outer frame loaded again at +%04x" % ow); wi += k + 1; gi += 1; done = True; break
                if done: break
            if done: continue
        # an outer procedure's variable passed by reference right after it was assigned: the compiler of 1981 pushes the
        # variable itself there (push [di-n], its error: seen with a call put between, which makes it right); the later one
        # loads the frame again and pushes the address
        m = re.fullmatch(r"push word ptr \[di ([-+] \w+)\]", tg)
        if m and tw == "mov di, word ptr [bp]" and wi + 2 < len(W) and W[wi + 1][2] == "lea dx, [di %s]" % m.group(1) and W[wi + 2][2] == "push dx" \
           and gi and re.fullmatch(r"mov (byte|word) ptr \[di %s\], \w+" % re.escape(m.group(1)), G[gi - 1][2]):
            notes.append("address of the variable pushed, where the 1981 compiler pushes its value (an error of that compiler), at +%04x" % ow); wi += 3; gi += 1; continue
        # DISPOSE
        if gi + 2 < len(G) and G[gi + 1][2] == "mov si, word ptr [si]" and G[gi + 2][2] == "or byte ptr [si - 2], 1":
            k = None
            if tg.startswith("lea si, ") and eq(tw, "lea dx, " + tg[8:]) and wi + 2 < len(W) and W[wi + 1][2] == "push dx": k = 2
            elif tg.startswith("mov si, word ptr ") and eq(tw, "push word ptr " + tg[17:]): k = 1
            elif tg == "mov si, di" and tw == "mov dx, di" and wi + 2 < len(W) and W[wi + 1][2] == "push di": k = 2
            elif "ptr" not in tg and tg.startswith("mov si, ") and tg[8:9].isdigit() and eq(tw, "mov dx, " + tg[8:]) and wi + 2 < len(W) and W[wi + 1][2] == "push dx": k = 2    # a variable by its address
            if k is not None and wi + k < len(W) and want[W[wi + k][0]:W[wi + k][0] + 5] == DISPOSE:
                notes.append("DISPOSE through the library at +%04x" % ow); wi += k + 1; gi += 3; reload = wi; continue
        # DISPOSE of a variable of an outer procedure: mov di,[bp] / lea dx,[di-n] / push dx against mov si,[bp] / lea si,[si-n]
        if gi + 3 < len(G) and wi + 3 < len(W) and G[gi + 2][2] == "mov si, word ptr [si]" and G[gi + 3][2] == "or byte ptr [si - 2], 1":
            m1 = re.fullmatch(r"mov si, (word ptr \[.+\])", tg); m2 = re.fullmatch(r"lea si, \[si ([-+] \w+)\]", G[gi + 1][2])
            if m1 and m2 and tw == "mov di, " + m1.group(1) and W[wi + 1][2] == "lea dx, [di %s]" % m2.group(1) and W[wi + 2][2] == "push dx" and want[W[wi + 3][0]:W[wi + 3][0] + 5] == DISPOSE:
                notes.append("DISPOSE through the library at +%04x" % ow); wi += 4; gi += 4; reload = wi; continue
        # a conditional jump one of the two had to make long
        if rw and 0x70 <= bw[0] <= 0x7F and bg[0] == bw[0] ^ 1 and len(bg) == 2 and bg[1] == 3 and gi + 1 < len(G) and got[G[gi + 1][0]] == 0xE9:
            g2 = G[gi + 1]; jumps.append((ow + sw + rw[1], g2[0] + 3 + struct.unpack_from("<h", got, g2[0] + 1)[0])); notes.append("long jump at +%04x" % ow); wi += 1; gi += 2; continue
        if rg and 0x70 <= bg[0] <= 0x7F and bw[0] == bg[0] ^ 1 and len(bw) == 2 and bw[1] == 3 and wi + 1 < len(W) and want[W[wi + 1][0]] == 0xE9:
            w2 = W[wi + 1]; jumps.append((w2[0] + 3 + struct.unpack_from("<h", want, w2[0] + 1)[0], og + sg + rg[1])); notes.append("long jump at +%04x" % ow); wi += 2; gi += 1; continue
        if stretch(): continue
        return False, ["no pattern at +%04x: original %s, compiled %s" % (ow, tw, tg)]
    if count is not None: return False, ["a shift count the original never loads, from +%04x" % count[1]]
    if wi != len(W) or gi != len(G): return False, ["the two do not end together: %d of %d, %d of %d instructions" % (wi, len(W), gi, len(G))]
    setmap({}, len(want))
    at = dict(amap); at[len(want)] = len(got)
    for t in fors:
        if t not in at: return False, ["the FOR entry test's jump to +%04x goes into an instruction" % t]
    for tw_, tg_ in jumps:
        if 0 <= tw_ <= len(want) and at.get(tw_) != tg_: return False, ["a jump to +%04x goes to +%04x in the compiled code, not +%s" % (tw_, tg_, "%04x" % at[tw_] if tw_ in at else "?")]
    if temps: notes.append("temporaries elsewhere in the frame (%s)" % ", ".join("-%X for -%X" % kv for kv in sorted(temps.items())))
    AMAP = sorted(set(amap)); GWALK[:] = [g[0] for g in G]
    return (True, notes) if notes else (False, ["differs only in jump displacements?"])


def equivalent3(want, got, fx):
    """INIT86 of MASM 1.10 was built by yet another state of the compiler: the entry helper of MS-Pascal 3.0x (only the
    frame size behind the call, a frame four bytes smaller, mov sp,bp at the exit), the code still that of 1981 but for
    a few forms - none of the compilers in hand writes it (1981: other helper; 3.04: other registers). Compared
    instruction by instruction with the 1981 compiler's code; allowed, besides equal bytes:

      the entry          call helper / dw frame            against  call helper / db args / dw frame + 4
      a local            [bp - n]                          against  [bp - (n + 4)]
      the exit           mov sp,bp                         against  lea sp,[bp - 4]
      an indexed store   add di,k / mov [di],v             against  mov [di + k],v
      a byte moved       mov dx,[..] / mov [..],dl         against  mov ax,[..] / mov [..],al
      a far address      mov dx,k / push ds / push dx      against  mov bx,k / mov dx,ds / mov es,dx / push es / push bx
      OR of two tests    or al,[temporary]                 against  or ax,[temporary]
    """
    global AMAP
    if not (want[:1] == got[:1] == b"\xe8") or (got[4] | got[5] << 8) - (want[3] | want[4] << 8) not in (0, 4): return False, ["the entry is not call helper / dw frame"]
    W = [(5 + o, n, t, m) for o, n, t, m in cmpp.walk(want[5:], 0, skip_hdr=False)]; G = [(6 + o, n, t, m) for o, n, t, m in cmpp.walk(got[6:], 0, skip_hdr=False)]
    notes = ["entry helper of MS-Pascal 3.0x"]; amap = [(0, 0)]; jumps = []; wi = gi = 0
    def local(t): return re.sub(r"\[bp - (0x[0-9a-f]+|\d+)\]", lambda m: "[bp - 0x%x]" % (int(m.group(1), 0) + 4), t)
    def hexed(t): return re.sub(r"\[bp - (\d+)\]", lambda m: "[bp - 0x%x]" % int(m.group(1)), t)
    def cond():
        """a condition the compiled code works out in full and the original leaves early (tools/boolx.py)"""
        nonlocal wi, gi
        ws, gs = wi, gi
        while ws and gs and W[ws - 1][2] == G[gs - 1][2] and re.match(r"(cmp|test) ", G[gs - 1][2]): ws -= 1; gs -= 1
        eg = None
        for j in range(gi, min(len(G) - 1, gi + 60)):
            if G[j][3] in ("call", "lcall", "ret", "retf", "table"): return False
            if re.fullmatch(r"shr (ax|cx|dx), 1", G[j][2]) and G[j + 1][3] in ("jae", "jb"): eg = j + 2; break
        if eg is None or eg + 1 >= len(G): return False
        nxt = (mask(G[eg][2]), mask(G[eg + 1][2]))
        ew = next((j for j in range(wi + 1, min(len(W) - 1, wi + 60)) if (mask(W[j][2]), mask(W[j + 1][2])) == nxt), None)
        if ew is None: return False
        def lines(L, b, i0, i1):
            out = []; glob = {}                                        # a variable's address is the linker's: named by the order they come in
            for o, n, x, m in L[i0:i1]:
                r = rel(b[o:o + n])
                if r and b[o] != 0xE8: x = "%s 0x%x" % (m, o + n + r[1])
                else: x = re.sub(r"\[(0x[0-9a-f]+|\d+)\]", lambda mm: "[g%d]" % glob.setdefault(mm.group(1), len(glob)), x)
                out.append((o, x))
            return out
        def bases(L, i):
            d = {"si": "si0", "di": "di0"}
            for r in ("si", "di"):
                for j in range(i - 1, max(-1, i - 40), -1):
                    m = re.fullmatch(r"mov %s, word ptr \[([^\]]+)\]" % r, L[j][2])
                    if m: d[r] = m.group(1); break
                    if re.match(r"(mov|xchg) %s" % r, L[j][2]) or L[j][3] in ("call", "lcall"): break
            return d
        ok, res = boolx.same(lines(W, want, ws, ew), W[ws][0], W[ew][0], lines(G, got, gs, eg), G[gs][0], G[eg][0], bases(W, ws), bases(G, gs))
        if not ok:
            if DEBUG: print("boolx +%04x: %s" % (W[ws][0], res))
            return False
        if res: jumps.append(res)
        notes.append("a condition worked out in full where the original jumps, from +%04x to +%04x" % (W[ws][0], W[ew][0]))
        amap[:] = [x for x in amap if x[0] <= W[ws][0]]
        wi, gi = ew, eg; return True
    while wi < len(W) and gi < len(G):
        ow, sw, tw, _ = W[wi]; og, sg, tg, _ = G[gi]; bw = want[ow:ow + sw]; bg = got[og:og + sg]; amap.append((ow, og))
        fxg = any((og + k) in fx for k in range(sg)); same = sw == sg and all(bw[k] == bg[k] or (og + k) in fx for k in range(sw))
        eq = lambda a, b: a == b or fxg and mask(a) == mask(b)
        rw, rg = rel(bw), rel(bg)
        if rw and rg and bw[0] == bg[0] and not fxg:
            if bw[0] != 0xE8: jumps.append((ow + sw + rw[1], og + sg + rg[1]))
            wi += 1; gi += 1; continue
        if same: wi += 1; gi += 1; continue
        if local(tw) == hexed(tg): wi += 1; gi += 1; continue
        if tw == "mov sp, bp" and tg == "lea sp, [bp - 4]": wi += 1; gi += 1; continue
        m = re.fullmatch(r"add di, (\w+)", tw)
        if m and wi + 1 < len(W) and fxg and mask(W[wi + 1][2].replace("[di]", "[di + N]")) == mask(tg): notes.append("indexed store at +%04x" % ow); wi += 2; gi += 1; continue
        if re.match(r"mov dx, word ptr \[", tw) and eq(tw.replace("mov dx,", "mov ax,"), tg): notes.append("byte moved through DX at +%04x" % ow); wi += 1; gi += 1; continue
        if re.match(r"mov byte ptr \[.*\], dl$", tw) and eq(tw[:-2] + "al", tg): wi += 1; gi += 1; continue
        if tw.startswith("mov dx, ") and wi + 2 < len(W) and W[wi + 1][2] == "push ds" and W[wi + 2][2] == "push dx" and gi + 4 < len(G) \
           and mask(tg) == "mov bx, N" and [g[2] for g in G[gi + 1:gi + 5]] == ["mov dx, ds", "mov es, dx", "push es", "push bx"]:
            notes.append("far address pushed at +%04x" % ow); wi += 3; gi += 5; continue
        if tw.startswith("or al, byte ptr [bp") and local(tw).replace("or al, byte ptr", "or ax, word ptr") == hexed(tg): wi += 1; gi += 1; continue
        return False, ["no pattern at +%04x: original %s, compiled %s" % (ow, tw, tg)]
    if wi != len(W) or gi != len(G): return False, ["the two do not end together"]
    at = dict(amap); at[len(want)] = len(got)
    for a, b in jumps:
        if at.get(a) != b: return False, ["a jump to +%04x goes to +%04x in the compiled code" % (a, b)]
    AMAP = sorted(set(amap)); GWALK[:] = [g[0] for g in G]
    return True, notes
