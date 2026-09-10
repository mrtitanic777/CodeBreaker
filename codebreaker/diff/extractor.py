#!/usr/bin/env python3
r"""extractor.py -- compute a function's three-level canonical fingerprint.

Reuses the engine's own `il.parse_asm` to tokenize a reconstructed .asm function
(so the db-escape, macro headers and mis-carved `ret` bytes are handled exactly as
the real lifter handles them), then derives:

  Level 1  CFG topology  -- basic blocks split on the same terminators the lifter
                           uses (jcc / jmp / loop / jcxz / ret), edges from the
                           branch targets. Yields node/edge counts, cyclomatic
                           complexity, and a hash of the block-adjacency listing.
  Level 2  Opcode stream -- every instruction pushed through Normalizer (registers
                           -> roles, imm/mem masked), hashed in order.
  Level 3  Call vector   -- the external call targets, in call order, hashed.

CFG is built independently of il's C-emission block list (which il keeps private);
the definition here only has to be deterministic and identical across versions,
which it is. It is intentionally a superset-safe read: an unresolvable branch
target simply contributes no edge rather than a wrong one.
"""
import hashlib
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # engine/
from codebreaker.ir_engine import il

from normalizer import Normalizer, EMPTY_SHA256  # noqa: E402

# terminators, matching il.lift_asm_function's block splitter
_COND = {"jz", "je", "jnz", "jne", "jg", "jge", "jl", "jle", "ja", "jae", "jb",
         "jbe", "jo", "jno", "js", "jns", "jc", "jnc", "jp", "jnp", "jpe", "jpo"}
_LOOP = {"loop", "loopw", "loopd", "loope", "loopz", "loopne", "loopnz"}
_CXZ = {"jcxz", "jecxz"}
_JMP = {"jmp", "ljmp", "jmpf"}
_RET = {"ret", "retn", "retf", "retd", "retfd", "retnd", "retfw", "retnw", "iret",
        "iretd", "iretw"}
_CALL = {"call", "lcall", "callf"}
_PREFIX = re.compile(r"^\s*((es|cs|ss|ds|fs|gs|lock|rep|repe|repz|repne|repnz|"
                     r"addr32|data32|addr16|data16)\s+)+", re.I)


def _mnem(text):
    text = _PREFIX.sub("", text.strip())
    low = re.sub(r"\s+", " ", text.lower())
    return low.split(" ")[0] if low else ""


def _target_token(text):
    """The branch/call target operand (last token; far form is seg,off -> off)."""
    t = _PREFIX.sub("", text.strip())
    return t.split(",")[-1].split()[-1].strip().lower() if t.split() else ""


class _Block:
    __slots__ = ("idx", "name", "first_off", "succ")

    def __init__(self, idx, name):
        self.idx = idx
        self.name = name
        self.first_off = None
        self.succ = []          # successor block indices (set in a second pass)


def _split_blocks(rows):
    """rows (from il.parse_asm) -> (blocks, term) where blocks is a list of _Block in
    program order and term[i] = (kind, target_text|None) records how block i ends.
    Instruction rows carried in `insns[i]`."""
    blocks, insns, term = [], [], []
    labels = {}                 # label name -> block idx
    offs = {}                   # first instruction offset -> block idx

    def _new(name):
        b = _Block(len(blocks), name)
        blocks.append(b); insns.append([]); term.append(("fall", None))
        return b

    cur = _new("entry")
    for row in rows:
        tag, val = row
        if tag == "label":
            term[cur.idx] = ("fall", None)
            nb = _new(val)
            labels[val.lower()] = nb.idx    # lower-cased: target tokens are lower-cased too
            cur = nb
            continue
        off, text = tag, val
        if cur.first_off is None and isinstance(off, int) and off >= 0:
            cur.first_off = off
            offs.setdefault(off, cur.idx)
        op = _mnem(text)
        if op in _COND or op in _LOOP or op in _CXZ:
            insns[cur.idx].append(text)
            term[cur.idx] = ("cond", _target_token(text))
            cur = _new("_after_%d" % cur.idx)
        elif op in _JMP:
            insns[cur.idx].append(text)
            term[cur.idx] = ("goto", _target_token(text))
            cur = _new("_dead_%d" % cur.idx)
        elif op in _RET:
            insns[cur.idx].append(text)
            term[cur.idx] = ("ret", None)
            cur = _new("_dead_%d" % cur.idx)
        else:
            insns[cur.idx].append(text)
    return blocks, insns, term, labels, offs


def _resolve_target(tok, labels, offs):
    if tok is None:
        return None
    if tok in labels:
        return labels[tok]
    m = re.match(r"^(?:0x)?([0-9a-f]+)h?$", tok)   # bare-hex offset -> block at that offset
    if m:
        try:
            return offs.get(int(m.group(1), 16))
        except ValueError:
            return None
    return None


def _wire_edges(blocks, term, labels, offs):
    n = len(blocks)
    for i, b in enumerate(blocks):
        kind = term[i][0]
        if kind == "ret":
            continue
        if kind == "fall":
            if i + 1 < n:
                b.succ.append(i + 1)
        elif kind == "goto":
            t = _resolve_target(term[i][1], labels, offs)
            if t is not None:
                b.succ.append(t)
        elif kind == "cond":
            t = _resolve_target(term[i][1], labels, offs)
            if t is not None:
                b.succ.append(t)
            if i + 1 < n:                 # fallthrough
                b.succ.append(i + 1)


def _components(kept_sorted, reindex, blocks):
    """Number of connected components over the kept blocks, edges undirected."""
    parent = list(range(len(kept_sorted)))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    for old in kept_sorted:
        a = reindex[old]
        for s in blocks[old].succ:
            if s in reindex:
                b = reindex[s]
                ra, rb = find(a), find(b)
                if ra != rb:
                    parent[ra] = rb
    return len({find(i) for i in range(len(kept_sorted))})


def fingerprint(path):
    """Compute the fingerprint dict for one reconstructed-asm function file."""
    rows = il.parse_asm(path)
    blocks, insns, term, labels, offs = _split_blocks(rows)
    _wire_edges(blocks, term, labels, offs)
    # Keep real basic blocks: any with instructions, plus the entry. This drops the
    # empty `_dead_*`/`_after_*` shells (and with them their spurious fall edges).
    keep = {i for i, b in enumerate(blocks) if insns[i] or i == 0}

    # ---- Level 1: CFG topology -------------------------------------------------
    kept_sorted = sorted(keep)
    nodes = len(kept_sorted)
    reindex = {old: new for new, old in enumerate(kept_sorted)}
    adj = []
    edge_count = 0
    for old in kept_sorted:
        succ = sorted(reindex[s] for s in blocks[old].succ if s in reindex)
        edge_count += len(succ)
        adj.append("%d->%s" % (reindex[old], ",".join(map(str, succ))))
    cfg_hash = hashlib.sha256(";".join(adj).encode()).hexdigest()
    # McCabe for a possibly-disconnected graph: E - N + 2P (P = components).
    ncomp = _components(kept_sorted, reindex, blocks) if nodes else 0
    cyclomatic = (edge_count - nodes + 2 * ncomp) if nodes else 0

    # ---- Level 2: normalized opcode stream ------------------------------------
    norm = []
    insn_count = 0
    for i in sorted(keep):
        for text in insns[i]:
            insn_count += 1
            t = Normalizer.normalize_instruction(text)
            if t:
                norm.append(t)
    opcode_hash = Normalizer.hash_opcode_stream(norm)

    # ---- Level 3: call vector --------------------------------------------------
    calls = []
    for i in sorted(keep):
        for text in insns[i]:
            if _mnem(text) in _CALL:
                calls.append(_target_token(text))
    call_hash = (hashlib.sha256(";".join(calls).encode()).hexdigest()
                 if calls else EMPTY_SHA256)

    return {
        "cfg_node_count": nodes,
        "cfg_edge_count": edge_count,
        "cyclomatic_complexity": cyclomatic,
        "cfg_topology_hash": cfg_hash,
        "insn_count": insn_count,
        "normalized_opcode_hash": opcode_hash,
        "call_count": len(calls),
        "call_vector_hash": call_hash,
    }


def instruction_view(path):
    """[(raw, normalized)] per instruction, in program order -- powers the diff view.
    Labels are shown as their own pseudo-rows so block structure stays visible."""
    rows = il.parse_asm(path)
    out = []
    for tag, val in rows:
        if tag == "label":
            out.append((val + ":", "@LABEL"))
        else:
            out.append((val, Normalizer.normalize_instruction(val) or "-"))
    return out


if __name__ == "__main__":
    import json
    for p in sys.argv[1:]:
        print(p)
        print(json.dumps(fingerprint(p), indent=2))
