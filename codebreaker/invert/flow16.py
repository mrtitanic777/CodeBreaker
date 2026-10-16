"""flow16.py -- Control-flow-graph analysis for the 16-bit inverse decompiler.

Foundation for the mc2inv structurer rewrite. Builds basic blocks from a capstone instruction
list, the CFG edges, dominators, and natural loops, then runs dominator-based structural analysis
to recover a structure tree (Seq / If / IfElse / While / DoWhile / Goto-fallback). This is a
STANDALONE, independently-tested module: it does NOT replace mc2inv.walk yet. The existing linear
walker keeps serving the 228 trustworthy-complete functions until this is proven equivalent-or-
better on them (0 structdiff regressions) and wired in behind a flag.

Scope notes:
 * `call`/`lcall` are NOT control-flow edges for structuring (the call returns); a block continues
   past them. Only jmp/jcc/ret/retf and fall-through form edges.
 * A jmp/jcc whose target is outside [lo,hi) is a tail call / return and becomes an exit edge.
 * Conditions are kept as (mnemonic, operand-text) on the branch block; the value model that turns
   them into C expressions stays in mc2inv -- this module only recovers STRUCTURE.
"""
import re, os

CJMP = {"jl","jle","jg","jge","je","jne","ja","jae","jb","jbe","jc","jnc","jz","jnz",
        "jo","jno","js","jns","jp","jnp","jcxz","loop"}
RET  = {"ret","retf","iret"}


class Block:
    __slots__ = ("id","lo","hi","insns","term","succ","cond_idx","addr", "table", "cases", "default", "selreg", "selbase")
    def __init__(s, bid, lo, hi, insns, addr):
        s.id = bid           # 0-based block index
        s.lo = lo            # first instruction index (into the function's insn list)
        s.hi = hi            # one past the last instruction index
        s.insns = insns      # the capstone instructions in this block
        s.addr = addr        # start address
        s.term = None        # 'ret' | 'jmp' | 'cond' | 'fall' | 'tailcall'
        s.succ = []          # list of successor block ids; for 'cond' = [taken, fallthrough]
        s.cond_idx = None    # instruction index of the terminating jcc (for 'cond')
        s.table = None       # 'switch' by a jump table: the case blocks in table order
        s.cases = None       # 'switch' by compares: [(value, block)]; default: the block the rest go to
        s.default = None
        s.selreg = None
        s.selbase = None     # (by a table: the instruction in the block where the selector's code ends)
    def __repr__(s):
        return "B%d[%05x..%05x %s->%s]" % (s.id, s.addr, (s.insns[-1].address if s.insns else s.addr),
                                           s.term, s.succ)


# Optional driver hook: SWITCH(addr) -> list of case-target addresses (table order) for the
# indirect `jmp cs:[bx+table]` at `addr`, or None. Set by mc2inv from its own SWITCH hook.
SWITCH = None

def _switch_targets(ins):
    if SWITCH is None or ins.mnemonic != "jmp" or "[" not in ins.op_str: return None
    try: return SWITCH(ins.address)
    except Exception: return None

def _jump_target(ins):
    """Absolute target address of a jmp/jcc, or None if indirect."""
    m = re.match(r"(0x[0-9a-f]+|\d+)$", ins.op_str)       # capstone prints targets < 10 without 0x
    return int(m.group(1), 0) if m else None


def build_cfg(insns, lo_addr, hi_addr):
    """insns: capstone instructions for [lo_addr, hi_addr). Returns (blocks, addr2blk, entry_id)."""
    addr2idx = {ins.address: i for i, ins in enumerate(insns)}
    n = len(insns)
    # --- leaders: entry, every in-range branch target, every instr after a branch/ret ---
    leaders = {0}
    for i, ins in enumerate(insns):
        mn = ins.mnemonic
        if mn in CJMP or mn == "jmp":
            t = _jump_target(ins)
            for st in (_switch_targets(ins) or []):
                if st in addr2idx: leaders.add(addr2idx[st])
            if t is not None and t in addr2idx:
                leaders.add(addr2idx[t])      # in-range target on an instruction boundary
            if i + 1 < n:
                leaders.add(i + 1)            # fall-through after a branch
        elif mn in RET:
            if i + 1 < n:
                leaders.add(i + 1)
    leads = sorted(leaders)
    # --- blocks: [leader, next leader) ---
    blocks = []
    idx2blk = {}
    for bi, start in enumerate(leads):
        end = leads[bi + 1] if bi + 1 < len(leads) else n
        b = Block(bi, start, end, insns[start:end], insns[start].address)
        blocks.append(b)
        for k in range(start, end):
            idx2blk[k] = bi
    # --- edges ---
    for b in blocks:
        last = insns[b.hi - 1]
        mn = last.mnemonic
        if mn in RET:
            b.term = "ret"; b.succ = []
        elif mn == "jmp":
            t = _jump_target(last)
            sw = _switch_targets(last)
            if t is not None and t in addr2idx:
                b.term = "jmp"; b.succ = [idx2blk[addr2idx[t]]]
            elif sw and all(x in addr2idx for x in sw):
                b.term = "switch"; b.table = [idx2blk[addr2idx[x]] for x in sw]
                b.succ = list(dict.fromkeys(b.table))
            else:
                b.term = "tailcall"; b.succ = []      # out-of-range/indirect jmp = tail call / return
        elif mn in CJMP:
            t = _jump_target(last)
            fall = idx2blk.get(b.hi) if b.hi < n else None
            taken = idx2blk[addr2idx[t]] if (t is not None and t in addr2idx) else None
            b.term = "cond"; b.cond_idx = b.hi - 1
            b.succ = [x for x in (taken, fall) if x is not None]
        else:
            fall = idx2blk.get(b.hi) if b.hi < n else None
            b.term = "fall"; b.succ = [fall] if fall is not None else []
    # Unreachable blocks (alignment `nop` padding after a jmp -- MS-C 5.0 pads branch targets --
    # or dead bytes) must not contribute edges: an unreachable block "falling into" its successor
    # is vacuously dominated by everything, which fabricates a back-edge and a phantom loop.
    seen = {0}; work = [0]
    while work:
        for t in blocks[work.pop()].succ:
            if t not in seen: seen.add(t); work.append(t)
    for blk in blocks:
        if blk.id not in seen: blk.succ = []; blk.term = "dead"
    if CMPSWITCH:
        _compare_switches(blocks)
        _table_switches(blocks)
    return blocks, idx2blk, 0


def _table_switches(blocks):
    """a jump table behind its bounds test - [sub R, K] cmp R, N / ja default (or jbe on, jmp default) / the
    table's dispatch - read as one switch by values: K + each place in the table, the bounds test's other way
    out the default (the table's entries that go there are no cases)"""
    preds = {b.id: [] for b in blocks}
    for b in blocks:
        for t in b.succ:
            preds[t].append(b.id)
    jmp_to = lambda bid: blocks[bid].succ[0] if blocks[bid].term == "jmp" and len(blocks[bid].insns) == 1 else bid
    for d in blocks:
        if d.term != "switch" or d.table is None or len(preds[d.id]) != 1:
            continue
        h = blocks[preds[d.id][0]]
        if h.term != "cond" or len(h.insns) < 2 or _cmp_of(h.insns[-2]) is None or h.insns[-1].mnemonic not in ("ja", "jbe"):
            continue
        reg, n = _cmp_of(h.insns[-2])
        taken, fall = h.succ[0], h.succ[1]
        other = fall if taken == d.id else taken
        if d.id not in (taken, fall) or len(d.table) != n + 1:
            continue
        default = jmp_to(other)
        base, cut = 0, len(h.insns) - 2
        if len(h.insns) >= 3:
            m = re.fullmatch(r"%s, (0x[0-9a-f]+|\d+)" % reg, h.insns[-3].op_str)
            if h.insns[-3].mnemonic == "sub" and m:
                base, cut = int(m.group(1), 0), len(h.insns) - 3
        h.term = "switch"
        h.cases = [(base + k, t) for k, t in enumerate(d.table) if t != default]
        h.default = default
        h.selreg = reg
        h.selbase = cut                                     # (the selector's code ends there: the sub is the table's)
        h.table = None
        h.succ = list(dict.fromkeys([t for v, t in sorted(h.cases)] + [default]))
        d.succ, d.term = [], "dead"
        if other != default:
            blocks[other].succ, blocks[other].term = [], "dead"


# A switch a compiler lays out as compares (a few cases, or values far apart): the selector in a register,
# then blocks that only compare it with a constant and branch - je to a case, the other tests (ja, jbe ..)
# splitting the values in halves, a jmp on to the next test - every way out that is not a case's going to
# one block, the default. Off unless the driver asks (CMPSWITCH): the 2.03 path keeps its own reading.
CMPSWITCH = False
_REG16 = {"ax", "bx", "cx", "dx", "si", "di"}


def _cmp_of(ins, reg=None):
    if ins.mnemonic in ("or", "test", "and") and re.fullmatch(r"(\w+), \1", ins.op_str):   # (a register tested: with 0)
        r = ins.op_str.split(",")[0]
        if r in _REG16 | {"al"} and (not reg or r == reg):
            return r, 0
        return None
    if ins.mnemonic != "cmp":
        return None
    m = re.fullmatch(r"(\w+), (0x[0-9a-f]+|\d+)", ins.op_str)
    if not m or m.group(1) not in _REG16 | {"al"} or reg and m.group(1) != reg:
        return None
    return m.group(1), int(m.group(2), 0)


def _compare_switches(blocks):
    preds = {b.id: [] for b in blocks}
    for b in blocks:
        for t in b.succ:
            preds[t].append(b.id)

    def test_block(bid, reg):
        blk = blocks[bid]
        ins = blk.insns
        if not ins or blk.term == "dead":
            return False
        if len(ins) == 1:
            return ins[0].mnemonic in CJMP or blk.term == "jmp"
        return len(ins) == 2 and _cmp_of(ins[0], reg) is not None and (ins[1].mnemonic in CJMP or blk.term == "jmp")

    for h in blocks:
        ins = h.insns
        if h.term != "cond" or len(ins) < 2 or _cmp_of(ins[-2]) is None:
            continue
        reg = _cmp_of(ins[-2])[0]
        if any(i.mnemonic in ("mov", "add", "sub", "inc", "dec", "pop") and i.op_str.startswith(reg + ",") for i in ins[-2:-1]):
            continue
        cases, leaves, region, ok = [], set(), {h.id}, [True]

        def walk(bid, v, first=False):
            blk = blocks[bid]
            seq = blk.insns[-2:] if first else blk.insns
            k = 0
            c = _cmp_of(seq[0], reg)
            if c:
                v = c[1]
                k = 1
            last = seq[k]
            if blk.term == "jmp":
                follow(blk.succ[0], v)
                return
            if v is None or len(blk.succ) != 2:
                ok[0] = False
                return
            taken, fall = blk.succ[0], blk.succ[1]
            if last.mnemonic in ("je", "jz"):
                cases.append((v, taken))
                follow(fall, v)
            elif last.mnemonic in ("jne", "jnz"):
                cases.append((v, fall))
                follow(taken, v)
            else:
                follow(taken, v)
                follow(fall, v)

        def follow(t, v):
            if t in region:
                return
            if test_block(t, reg) and all(p in region for p in preds[t]):
                region.add(t)
                walk(t, v)
            else:
                leaves.add(t)
                ins_ = blocks[t].insns                      # (a last compare with no jump after it: its case is
                c_ = _cmp_of(ins_[0], reg) if ins_ else None  #  the block it falls into - the default's too)
                if c_ and len(ins_) > 1 and ins_[1].mnemonic not in CJMP and all(p in region for p in preds[t]):
                    cases.append((c_[1], t))

        walk(h.id, None, first=True)
        vals = [v for v, t in cases]
        # (two values are a switch too when the selector was loaded into the register for the tests alone -
        #  `mov ax,[m]; cmp ax,K1; je; cmp ax,K2; je`: an if would compare the memory itself)
        loaded = reg in ("ax", "al") and len(ins) >= 3 and ins[-3].mnemonic == "mov" and ins[-3].op_str.startswith(reg + ", ") and "[" in ins[-3].op_str
        if not ok[0] or len(leaves) != 1 or len(set(vals)) < (2 if loaded else 3) or len(set(vals)) != len(vals):
            continue
        default = leaves.pop()

        def through(t):                                     # (a case reached by a jmp of the compares' own - the
            blk = blocks[t]                                 #  long form of a jump: jne over, jmp to the case)
            if len(blk.insns) == 1 and blk.term == "jmp" and all(p in region for p in preds[t]):
                region.add(t)
                return blk.succ[0]
            return t
        cases = [(v, through(t)) for v, t in cases]
        default = through(default)
        h.term = "switch"
        h.cases = cases
        h.default = default
        h.selreg = reg
        h.table = None
        h.succ = sorted(set([t for v, t in cases] + [default]))
        # (the arms in the code's order, the default's too - the source's, and one falls into the next as written)
        for r in region - {h.id}:
            blocks[r].succ = []
            blocks[r].term = "dead"


def dominators(blocks, entry=0):
    """Iterative dominator sets. Returns dom: {bid: set(dominators)}."""
    ids = [b.id for b in blocks]
    preds = {i: [] for i in ids}
    for b in blocks:
        for s in b.succ:
            preds[s].append(b.id)
    allset = set(ids)
    dom = {i: (set([entry]) if i == entry else set(allset)) for i in ids}
    changed = True
    while changed:
        changed = False
        for b in blocks:
            if b.id == entry:
                continue
            new = set(allset)
            for p in preds[b.id]:
                new &= dom[p]
            new.add(b.id)
            if new != dom[b.id]:
                dom[b.id] = new; changed = True
    return dom, preds


def back_edges(blocks, dom):
    """Edges u->v where v dominates u (natural-loop back edges)."""
    be = []
    for b in blocks:
        for s in b.succ:
            if s in dom[b.id]:
                be.append((b.id, s))
    return be


def natural_loop(blocks, preds, header, tail):
    """Nodes of the natural loop for back-edge tail->header."""
    loop = {header, tail}
    stack = [tail]
    while stack:
        x = stack.pop()
        for p in preds[x]:
            if p not in loop:
                loop.add(p); stack.append(p)
    return loop


def post_dominators(blocks, preds):
    """Post-dominators: dominators on the reversed CFG from a virtual exit that all exit blocks
    (ret/tailcall, i.e. no successors) flow into. Returns pdom: {bid: set(post-dominators)} using a
    sentinel exit id = len(blocks)."""
    ids = [b.id for b in blocks]
    EXIT = len(blocks)
    exits = [b.id for b in blocks if not b.succ]
    allset = set(ids) | {EXIT}
    # On the reversed graph a forward edge b->s becomes s->b, so b's reverse-predecessors are its
    # forward successors; every exit block's reverse-predecessor is the virtual EXIT.
    rpred_map = {i: [] for i in allset}
    for b in blocks:
        for s in b.succ:
            rpred_map[b.id].append(s)
    for e in exits:
        rpred_map[e].append(EXIT)
    pdom = {i: (set([EXIT]) if i == EXIT else set(allset)) for i in allset}
    changed = True
    while changed:
        changed = False
        for i in list(ids):
            new = set(allset)
            for p in rpred_map[i]:
                new &= pdom[p]
            new.add(i)
            if new != pdom[i]:
                pdom[i] = new; changed = True
    return pdom, EXIT


def ipdom(pdom, EXIT, bid):
    """Immediate post-dominator of bid: the nearest strict post-dominator. Among the strict
    post-dominators, it is the one that every other strict post-dominator also post-dominates
    (i.e. its own post-dominator set contains all the candidates)."""
    cand = pdom[bid] - {bid}
    if not cand:
        return None
    for ip in cand:
        if all(d in pdom[ip] for d in cand):
            return None if ip == EXIT else ip
    return None


def structure(blocks, entry=0):
    """Recover a structure tree from the CFG. Node forms:
        ('block', bid)                         a straight-line basic block
        ('seq', [nodes])                       sequence
        ('if', bid, then_tree, join)           if-then (cond block bid, join = merge)
        ('ifelse', bid, then_tree, else_tree, join)
        ('while', bid, body_tree, join)        pre-tested loop (header bid tests, exits to join)
        ('dowhile', header, body_tree, latch)  post-tested loop (test at latch)
        ('goto', bid, target)                  unstructured edge -> goto fallback
    Dominator/post-dominator guided; falls back to 'goto' for flow it can't fit (never fabricates).
    This is the SKELETON for the rewrite: it proves structure recovery; the value model (conditions
    -> C expressions, phi at joins) is layered on at integration time in mc2inv."""
    dom, preds = dominators(blocks, entry)
    pdom, EXIT = post_dominators(blocks, preds)
    be = back_edges(blocks, dom)
    headers = {}
    for (t, h) in be:                                       # several back-edges to one header (each
        headers.setdefault(h, set()).update(natural_loop(blocks, preds, h, t))   # `continue`) = ONE loop
    latch = {}
    for (t, h) in be:
        latch.setdefault(h, t)

    # Duplication budget. A shared tail reached from several paths is copied onto each path only
    # while it is SMALL; a large shared region is emitted once and reached by `goto` from the
    # other paths (registers are pinned at every goto/label, so that is sound). Without the cap
    # nested shared tails multiply: one MASM 5.10 function came out at 39,000 lines.
    emitted = set()
    _big = {}
    def big(b):
        if b not in _big:
            seen = {b}; work = [b]
            while work and len(seen) <= 5:
                for t in blocks[work.pop()].succ:
                    if t not in seen: seen.add(t); work.append(t)
            _big[b] = len(seen) > 5 or sum(blocks[x].hi - blocks[x].lo for x in seen) > 24
        return _big[b]

    active = set()        # blocks currently being expanded on the recursion stack (ancestor guard)
    loopstack = []        # enclosing loops as (header, follow): edges to follow -> break, to header -> continue
    casestack = []        # the arms of the switches by compares being structured: reaching another is a goto

    def ends_terminal(nodes):
        if not nodes: return False
        n = nodes[-1]
        if n[0] in ('break', 'continue', 'goto'): return True
        if n[0] == 'block': return blocks[n[1]].term in ('ret', 'tailcall')
        if n[0] == 'ifelse': return ends_terminal(n[2]) and ends_terminal(n[3])
        if n[0] == 'switch': return n[3] is None and all(ends_terminal(a) for _, a in n[2])
        return False

    def seq_from(b, stops):
        out = []
        local = set()     # blocks emitted in THIS linear run (sibling tail-dup across calls is OK;
                          # re-entering one on the current PATH is a back-edge -> goto)
        while b is not None and b != EXIT:
            if loopstack:                                   # break/continue for the innermost loop
                h, fol, lbody, lflag = loopstack[-1]
                if casestack and casestack[-1][1] == (len(loopstack), b):
                    return out                              # (a switch's arm at its join, the loop's follow: the switch then breaks)
                if b == fol: out.append(('break',)); return out
                if b == h: out.append(('continue',)); return out
                if b not in lbody and not lflag[0]:
                    # A SECOND exit: this edge leaves the loop to somewhere other than its follow
                    # (typically `found: return 1` inside a search loop). It is NOT "reaching the
                    # join" -- treating it so made the early return fall through and keep looping.
                    # A returning block is duplicated in place; anything else is a goto.
                    # A short straight-line exit path that ends in a ret (`found: x = ..; return 1`)
                    # is duplicated in place; anything else is a goto.
                    chain = []; c = b
                    while c is not None and len(chain) < 4:
                        chain.append(c)
                        if blocks[c].term == "ret": break
                        c = blocks[c].succ[0] if (blocks[c].term in ("jmp", "fall") and blocks[c].succ) else None
                    if c is not None and blocks[c].term == "ret" and chain[-1] == c:
                        out.extend(('block', x) for x in chain)
                        return out
                    # Otherwise structure the exit path in place (it is code that runs once, on the
                    # way out). Valid only if it never falls back into the loop body: it must end in
                    # a return, a break to the loop's follow, a continue, or a goto.
                    lflag[0] = True
                    try: sub = seq_from(b, stops)
                    finally: lflag[0] = False
                    out.extend(sub) if ends_terminal(sub) else out.append(('goto', None, b))
                    return out
            if b in stops:
                if casestack and b in casestack[-1][0]:
                    out.append(('goto', None, b))       # (into another arm: the case falls into it)
                break
            if b in active or b in local:                   # ancestor/irreducible re-entry -> goto
                out.append(('goto', None, b)); break
            if b in emitted and big(b) and not os.environ.get("CFG_NOCAP"):   # large region already emitted elsewhere
                out.append(('goto', None, b)); break
            emitted.add(b)
            local.add(b)
            if b in headers:
                active.add(b)
                node, after = do_loop(b)
                active.discard(b)
                out.append(node); b = after; continue
            blk = blocks[b]
            if blk.term == "cond":
                active.add(b)
                node, after = do_if(b, stops)
                active.discard(b)
                out.append(node); b = after; continue
            if blk.term == "switch":
                active.add(b)
                node, after = do_switch(b, stops)
                active.discard(b)
                out.append(node); b = after; continue
            out.append(('block', b))
            b = blk.succ[0] if blk.succ else None
        return out

    def do_switch(b, stops):
        """('switch', bid, [(case_values, arm_tree)], join): one arm per distinct table target,
        each structured up to the switch's join (the implicit `break`)."""
        blk = blocks[b]
        join = ipdom(pdom, EXIT, b)
        if loopstack and join is not None:
            _h, _fol, _lbody, _lf = loopstack[-1]
            if join not in _lbody and join != _fol: join = None
        sub = stops | ({join} if join is not None else set())
        cases = []
        for t in blk.succ:
            if blk.cases is not None:                      # (by compares: the values, the default's arm)
                vals = sorted(v for v, x in blk.cases if x == t) + (["default"] if t == blk.default else [])
                if t == join and vals == ["default"]:
                    continue                               # (no default: the rest go to the end)
                arms = set(blk.succ) - {t, join}
                casestack.append((arms, (len(loopstack), join)))
                try:
                    cases.append((vals, seq_from(t, sub | arms) if t != join else []))
                finally:
                    casestack.pop()
                continue
            else:
                vals = [i for i, x in enumerate(blk.table) if x == t]
            cases.append((vals, seq_from(t, sub) if t != join else []))
        return ('switch', b, cases, join), join

    def do_if(b, stops):
        blk = blocks[b]
        join = ipdom(pdom, EXIT, b)
        if loopstack and join is not None:
            # Inside a loop, a join that lies OUTSIDE the loop (and is not its follow) is not a
            # merge point of this if: its arms leave the loop separately (handled as exits).
            _h, _fol, _lbody, _lf = loopstack[-1]
            # ... and the loop's FOLLOW is not a merge inside the loop either: an arm reaching it
            # is a `break`, while the other arm may run on to the latch. (Treating it as the join
            # produced `if (c) {} break;` -- the loop exited unconditionally.)
            if join not in _lbody: join = None
        taken = blk.succ[0]
        fall = blk.succ[1] if len(blk.succ) > 1 else None
        then_s, else_s = fall, taken                        # then = fall-through, else = taken (jcc)
        sub = stops | ({join} if join is not None else set())
        # 'if' node carries a `neg` flag (see emitter): False => cond_at's fall-through condition;
        # True => the structured branch is the TAKEN side, so the emitter negates the condition.
        if join is not None and else_s == join:            # if-then: taken goes straight to join
            return ('if', b, seq_from(then_s, sub), join, False), join
        if join is not None and then_s == join:            # if-then, empty fall -> guard on taken
            return ('if', b, seq_from(else_s, sub), join, True), join
        if join is not None:                                # if-then-else
            return ('ifelse', b, seq_from(then_s, sub), seq_from(else_s, sub), join), join
        # no clean join (e.g. both arms break/continue/return) -> structure each arm to its own end
        return ('ifelse', b, seq_from(then_s, stops), seq_from(else_s, stops), None), None

    def do_loop(h):
        body = headers[h]
        hblk = blocks[h]
        exits = set()                                       # successors leaving the loop body
        for n in body:
            for s in blocks[n].succ:
                if s not in body:
                    exits.add(s)
        follow = next(iter(exits)) if len(exits) == 1 else None
        lb = latch.get(h)
        # pre-tested while: header is the test and one successor exits the loop
        # (a block that branches back to ITSELF runs its body before its test: that is the
        #  do-while form below, not a pre-tested while)
        if hblk.term == "cond" and any(s not in body for s in hblk.succ) and h not in hblk.succ:
            inside = [s for s in hblk.succ if s in body]
            fol = [s for s in hblk.succ if s not in body][0]
            loopstack.append((h, fol, body, [False]))
            body_t = seq_from(inside[0], {h}) if inside else []
            loopstack.pop()
            # 5th field: True when the loop is entered on the jcc's TAKEN edge (the bottom-tested
            # `jmp test; body; test: cmp; jcc body` shape) -> the emitter must negate cond_at's
            # fall-through-sense condition. Without it every such while came out inverted.
            return ('while', h, body_t, fol, bool(inside) and inside[0] == hblk.succ[0]), fol
        # post-tested do-while: the latch is the test and one successor exits
        if lb is not None and blocks[lb].term == "cond" and any(s not in body for s in blocks[lb].succ):
            fol = [s for s in blocks[lb].succ if s not in body][0]
            loopstack.append((h, fol, body, [False]))
            # Emit the body STARTING WITH the header: seq_from(h, ..) hits its own `b == h ->
            # continue` check on the first step and would drop the entire body.
            if h == lb:
                body_t = []                                 # single-block loop: the latch IS the body
            elif hblk.term == "cond":
                active.add(h)
                node, after = do_if(h, {lb})
                active.discard(h)
                body_t = [node]
                if after is not None and after not in (h, lb, EXIT):
                    body_t += seq_from(after, {lb})
            else:
                body_t = [('block', h)]
                if hblk.succ and hblk.succ[0] != lb:
                    body_t += seq_from(hblk.succ[0], {lb})
            body_t = body_t + [('block', lb)]
            loopstack.pop()
            # 5th field: True when the latch's TAKEN edge continues the loop (the usual `jcc top`).
            return ('dowhile', h, body_t, lb, blocks[lb].succ[0] in body), fol
        # endless/multi-exit loop: emit the body STARTING WITH the header (seq_from(h,...) would hit
        # the b==h continue-check immediately and drop the whole body). Process the header block/if
        # first, then walk onward; re-reaching h is the back-edge -> continue, reaching follow -> break.
        loopstack.append((h, follow, body, [False]))
        hblk = blocks[h]
        if hblk.term == "cond":
            node, after = do_if(h, {h})
            body_t = [node]
            if after is not None and after != h and after != EXIT:
                body_t += seq_from(after, {h})
        else:
            body_t = [('block', h)]
            if hblk.succ:
                body_t += seq_from(hblk.succ[0], {h})
        loopstack.pop()
        return ('loop', h, body_t, follow), follow

    return seq_from(entry, set())


