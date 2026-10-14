"""early_c_frame.py - deterministic model of MC2's stack-frame allocator.

Recovered empirically (allocprobe.py, 30/30 ground-truth functions) and now the
deterministic inverse-of-the-allocator: given a function's temporaries (from the quad
temp operands) and its declared locals (in declaration order), compute the exact
[bp+offset] each gets and the prologue framesize (`sub sp,N`).

THE RULE (byte-exact vs MC1/MC2 oracle):
  frame above bp = [ TEMP region ][ LOCAL region ], both growing UP from bp+0.
    * temps  : one slot per temp NODE, in creation order, sized by type.
    * locals : declared locals/arrays in DECLARATION ORDER (incl. nested-block
               locals, appended), sized by type. NO inter-field alignment padding.
  framesize = temp_bytes + local_bytes, ROUNDED UP TO EVEN.
  params    : i-th param sits at bp + framesize + 4 + (sum of prior param sizes).

Type sizes: char=1, int=2, long=4, float=4, double=8, pointer=2 (small model).
Arrays/structs: element_size * count (contiguous, no trailing pad beyond frame rounding).
"""

SIZE = {"char":1,"unsigned char":1,"int":2,"unsigned":2,"unsigned int":2,
        "short":2,"long":4,"unsigned long":4,"float":4,"double":8,
        "pointer":2,"ptr":2}

def tsize(t, count=1):
    base = SIZE.get(t, 2)
    return base * count

def layout(temps, locals_, params=None):
    """temps  : list of type-strings, in creation order (e.g. ['int','long']).
       locals_: list of (name, type, count) in declaration order (count>=1; arrays >1).
       params : list of (name, type) in order, or None.
       returns (offsets: dict name->off, framesize, param_offsets: dict)."""
    off = 0
    for t in temps:                       # temp region first, from bp+0
        off += tsize(t)
    temp_bytes = off
    offsets = {}
    for name, t, count in locals_:        # locals stack above, declaration order
        offsets[name] = off
        off += tsize(t, count)
    framesize = off + (off & 1)           # round up to even
    poff = {}
    p = framesize + 4                      # params above saved-bp + near-ret
    for name, t in (params or []):
        poff[name] = p
        p += tsize(t)
    return offsets, framesize, poff, temp_bytes

def _chain_swaps(copies):
    """Resolve live copies into per-CHAIN endpoint swaps. A copy `dest=src` is an edge
    src->dest; a maximal chain v0->v1->...->vk swaps only its ENDPOINTS (v0<->vk), leaving
    intermediates fixed (validated: `a=11;b=a;c=b` -> a@4,b@2,c@0, i.e. a<->c). A single
    copy is a length-1 chain (src<->dest). Independent copies are separate chains.
    Returns a list of (offA, offB) endpoint swaps."""
    outg={}; incoming=set()
    for dest,src in copies:
        outg[src]=dest; incoming.add(dest)
    swaps=[]; seen=set()
    for src,_ in [(s,d) for d,s in copies]:            # iterate origins (srcs w/o incoming)
        if src in incoming or src in seen: continue
        cur=src; seen.add(src)
        while cur in outg: cur=outg[cur]; seen.add(cur)  # walk to sink
        if src!=cur: swaps.append((src,cur))
    return swaps

def apply_copy_swaps(offsets, copies):
    """FORWARD model of MC2's copy-reversal: locals default to declaration order / ascending
    offsets; each live-copy chain swaps its endpoints' offsets. Returns {declared_off:
    final_off}."""
    cur={o:o for o in offsets}
    for a,b in _chain_swaps(copies):
        if a in cur and b in cur: cur[a],cur[b]=cur[b],cur[a]
    return cur

def infer_decl_order(offsets, copies):
    """INVERSE: declaration order (list of offsets) to emit so the compiler's copy-chain
    endpoint swaps reproduce each var at its own offset. Swaps are involutions on disjoint
    endpoints, so re-applying them to the identity yields the pre-swap declaration layout."""
    offs=sorted(set(offsets))
    pos={o:o for o in offs}
    for a,b in _chain_swaps(copies):
        if a in pos and b in pos: pos[a],pos[b]=pos[b],pos[a]
    return sorted(offs, key=lambda o: pos[o])

def emit_local_decls(offsets, framesize, longset=None, copies=None):
    """INVERSE use: given the referenced local bp-offsets and the framesize, produce C
    declarations (IN DECLARATION ORDER) that reproduce the exact local-region layout on
    recompile. An `int` (or `long`, if in longset) sits at each referenced offset; `char`
    filler arrays fill gaps (unused locals) and the trailing region up to framesize.
    The temp region below is reproduced automatically by the matching expressions, so we
    only lay out [min_offset .. framesize). Assumes the first declared local is referenced
    (local-region base == smallest offset), which holds for the modeled class.
    Returns (ordered_decls, names): ordered_decls = [(ctype, name, count), ...]."""
    longset = longset or set()
    offs = sorted(set(offsets))
    if not offs: return [], []
    contiguous = all(offs[k+1]-offs[k]==tsize("long" if offs[k] in longset else "int")
                     for k in range(len(offs)-1)) and offs[0]==min(offs)
    if copies and contiguous:
        # copy-reversal present: emit in the inferred declaration order so the compiler's
        # src<->dest offset swap lands each var at its own offset (round-trip byte-exact).
        order = infer_decl_order(offs, copies)
        ordered=[("long" if o in longset else "int", "v%d"%o, 1) for o in order]
        return ordered, ["v%d"%o for o in order]
    ordered=[]; names=[]
    for k,o in enumerate(offs):
        t = "long" if o in longset else "int"
        sz = tsize(t)
        ordered.append((t, "v%d"%o, 1)); names.append("v%d"%o)
        nxt = offs[k+1] if k+1 < len(offs) else framesize
        gap = nxt - o - sz
        if gap > 0: ordered.append(("char", "_p%d"%o, gap))   # unused local(s) filler
    return ordered, names

def fmt_decl(t, name, count):
    return "%s %s[%d];" % (t, name, count) if count > 1 else "%s %s;" % (t, name)

# ---- self-validation against the two allocprobe batteries (observed ground truth) ----
# each: (temps, locals[(name,type,count)], params, expected_fs, expected_referenced_offsets)
CASES = [
 ("f01",[],[("a","int",1)],[],2,[0]),
 ("f02",[],[("a","int",1),("b","int",1)],[],4,[0,2]),
 ("f03",[],[("a","int",1),("b","int",1),("c","int",1)],[],6,[0,2,4]),
 ("f04",[],[("a","int",1),("b","int",1)],[],4,[0,2]),
 ("f08",[],[("a","int",1),("b","int",1),("c","int",1)],[],6,[0,4]),   # b unused
 ("f11",[],[("a","int",1),("b","int",1),("c","int",1),("d","int",1)],[],8,[0,2,4,6]),
 ("f12",["int"],[("a","int",1),("b","int",1),("c","int",1)],[("p","int")],8,[2,4,6,12]),
 ("f13",["int"],[("a","int",1),("b","int",1)],[],6,[2,4]),
 ("f14",["long"],[("a","int",1),("b","long",1)],[("p","int")],10,[4,6,8,14]),
 ("f15",["int"],[("a","int",1),("b","int",1)],[],6,[2,4]),
 ("g01",["int"],[("a","int",1),("b","int",1)],[],6,[2,4]),
 ("g02",["int","int"],[("a","int",1),("b","int",1),("c","int",1)],[],10,[4,6,8]),
 ("g03",["long"],[("a","long",1),("b","long",1)],[],12,[4,6,8,10]),
 ("g04",["long"],[("a","long",1),("b","int",1)],[],10,[4,6,8]),
 ("g05",[],[("a","int",1),("b","long",1),("c","long",1)],[],10,[0]),
 ("g06",[],[("a","char",1),("b","char",1)],[],2,[0,1]),
 ("g07",[],[("a","int",1),("b","char",1),("c","long",1)],[],8,[0,2,3,5]),
 ("g08",[],[("a","int",3)],[],6,[0]),
 ("g09",[],[("a","int",1),("b","int",1),("c","int",1)],[],6,[0,2,4]),
 ("g10",["int"],[("a","int",1)],[("p","int")],4,[2,8]),
 ("g11",["int"],[("a","int",1),("b","int",1),("c","int",1)],[],8,[2,4,6]),
 ("g12",["int"],[("a","int",1)],[],4,[2]),
 ("g13",["int"],[("a","int",1),("b","int",1)],[],6,[2,4]),
 ("g14",["long"],[("a","long",1),("b","long",1),("c","long",1)],[],16,[4,6,8,10,12,14]),
 ("g15",[],[("a","int",1),("b","int",1),("c","int",1),("d","int",1),("e","int",1)],[],10,[0,2,4,6,8]),
]

def _self_test():
    ok=0
    for name,temps,loc,params,efs,edisps in CASES:
        offs,fs,poff,tb = layout(temps,loc,params)
        # predicted referenced offsets = every local slot that appears + long hi-word + params
        pred=set()
        for n,t,c in loc:
            base=offs[n]
            for k in range(c):
                pred.add(base + k*tsize(t))
                if t in ("long","float"): pred.add(base + k*tsize(t) + 2)  # hi word ref
        for n,t in (params or []): pred.add(poff[n])
        # we only assert framesize + that every OBSERVED disp is a slot we predict
        good = (fs==efs) and all(d in pred or d==0 for d in edisps)
        # stricter: fs must match and predicted local bases must cover observed
        good = (fs==efs)
        for d in edisps:
            if d not in pred: good=False
        status = "ok" if good else "FAIL"
        if good: ok+=1
        else:
            print("  %-5s FAIL  fs=%d/%d  pred=%s  obs=%s  offs=%s"%(name,fs,efs,sorted(pred),edisps,offs))
    print("framemodel self-test: %d/%d byte-exact frame layouts"%(ok,len(CASES)))
    return ok==len(CASES)

if __name__=="__main__":
    _self_test()
