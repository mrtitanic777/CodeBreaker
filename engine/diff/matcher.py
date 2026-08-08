#!/usr/bin/env python3
r"""matcher.py -- fuzzy cross-version function matching.

Scores candidate pairs with the blueprint's weighted model (CFG 30% / opcode 40%
/ calls 30%), but does NOT compare all-pairs: at 224k x 18k that is 4e9 scorings.
Instead a target-side index buckets functions by their three fingerprint hashes,
so each source only scores the handful of targets that share SOME structure with
it. Exact and near matches both surface; unrelated pairs are never scored.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from normalizer import EMPTY_SHA256  # noqa: E402

_W_CFG, _W_OPCODE, _W_CALLS = 0.30, 0.40, 0.30


def compute_similarity(s, t):
    """Weighted 0..1 similarity between two function rows (sqlite3.Row / dict-like)."""
    score = 0.0
    if s["cfg_topology_hash"] == t["cfg_topology_hash"]:
        score += _W_CFG
    elif (s["cyclomatic_complexity"] == t["cyclomatic_complexity"]
          and s["cfg_node_count"] == t["cfg_node_count"]):
        score += _W_CFG / 2
    if s["normalized_opcode_hash"] == t["normalized_opcode_hash"]:
        score += _W_OPCODE
    if (s["call_vector_hash"] == t["call_vector_hash"]
            and s["call_vector_hash"] != EMPTY_SHA256):
        score += _W_CALLS
    return round(score, 4)


def _match_type(s, t):
    cfg = s["cfg_topology_hash"] == t["cfg_topology_hash"]
    opc = s["normalized_opcode_hash"] == t["normalized_opcode_hash"]
    cal = (s["call_vector_hash"] == t["call_vector_hash"]
           and s["call_vector_hash"] != EMPTY_SHA256)
    if cfg and opc:
        return "EXACT"
    if cfg or opc:
        return "CFG_OPCODE"
    if cal:
        return "CALL_GRAPH"
    return "WEAK"


def _load(conn, version):
    return list(conn.execute("SELECT * FROM functions WHERE version_id=?", (version,)))


def _index(rows):
    idx = {"cfg": {}, "opc": {}, "call": {}}
    for r in rows:
        idx["cfg"].setdefault(r["cfg_topology_hash"], []).append(r)
        idx["opc"].setdefault(r["normalized_opcode_hash"], []).append(r)
        if r["call_vector_hash"] != EMPTY_SHA256:
            idx["call"].setdefault(r["call_vector_hash"], []).append(r)
    return idx


def match_versions(conn, source_ver, target_ver, threshold=0.75, store=True,
                   min_insns=4):
    """Best target match per source function, above `threshold`. Returns the match
    list; also writes function_matches when store=True. Tiny functions (< min_insns)
    are skipped -- a 3-instruction stub matches thousands and means nothing."""
    src_rows = _load(conn, source_ver)
    tgt_rows = _load(conn, target_ver)
    idx = _index(tgt_rows)

    matches = []
    for s in src_rows:
        if (s["insn_count"] or 0) < min_insns:
            continue
        # candidate targets = anything sharing a CFG, opcode, or call fingerprint
        cand = {}
        for key, bucket in (("cfg_topology_hash", "cfg"),
                            ("normalized_opcode_hash", "opc"),
                            ("call_vector_hash", "call")):
            if bucket == "call" and s["call_vector_hash"] == EMPTY_SHA256:
                continue
            for t in idx[bucket].get(s[key], ()):
                cand[t["function_id"]] = t
        best, best_score = None, 0.0
        for t in cand.values():
            sc = compute_similarity(s, t)
            if sc > best_score:
                best, best_score = t, sc
        if best is not None and best_score >= threshold:
            matches.append({
                "source": s["function_id"], "target": best["function_id"],
                "score": best_score, "match_type": _match_type(s, best),
                "source_name": s["symbol_name"], "target_name": best["symbol_name"],
                "source_module": s["module_name"], "target_module": best["module_name"],
            })
    if store:
        conn.execute("DELETE FROM function_matches WHERE source_version=? AND"
                     " target_version=?", (source_ver, target_ver))
        conn.executemany(
            "INSERT OR REPLACE INTO function_matches(source_func_id, target_func_id,"
            " source_version, target_version, similarity_score, match_type)"
            " VALUES (?,?,?,?,?,?)",
            [(m["source"], m["target"], source_ver, target_ver, m["score"],
              m["match_type"]) for m in matches])
        conn.commit()
    return matches


def match_one(conn, fid, target_ver, threshold=0.0, topn=5):
    """Ranked candidate matches for a single function in `target_ver` (on the fly,
    no stored function_matches needed). Returns [(row, score, type)] best-first."""
    s = conn.execute("SELECT * FROM functions WHERE function_id=?", (fid,)).fetchone()
    if s is None:
        return []
    idx = _index(_load(conn, target_ver))
    cand = {}
    for key, bucket in (("cfg_topology_hash", "cfg"),
                        ("normalized_opcode_hash", "opc"),
                        ("call_vector_hash", "call")):
        if bucket == "call" and s["call_vector_hash"] == EMPTY_SHA256:
            continue
        for t in idx[bucket].get(s[key], ()):
            if t["function_id"] != fid:
                cand[t["function_id"]] = t
    scored = [(t, compute_similarity(s, t), _match_type(s, t)) for t in cand.values()]
    scored = [x for x in scored if x[1] >= threshold]
    scored.sort(key=lambda x: -x[1])
    return scored[:topn]


def lineage(conn, fid):
    """A function's best counterpart in every OTHER version -- its cross-version
    'history'. Returns {version_id: {row fields..., score, match_type}}."""
    s = conn.execute("SELECT * FROM functions WHERE function_id=?", (fid,)).fetchone()
    if s is None:
        return {}
    others = [r[0] for r in conn.execute(
        "SELECT DISTINCT version_id FROM functions WHERE version_id<>?",
        (s["version_id"],))]
    out = {}
    for ver in others:
        best = match_one(conn, fid, ver, threshold=0.3, topn=1)
        if best:
            t, score, mtype = best[0]
            out[ver] = {"function_id": t["function_id"], "symbol_name": t["symbol_name"],
                        "module_name": t["module_name"], "segment_offset": t["segment_offset"],
                        "score": score, "match_type": mtype,
                        "cyclomatic": t["cyclomatic_complexity"], "insn": t["insn_count"]}
    return out


def census(conn, source_ver, target_ver, matches=None):
    """Summary of a match run: counts by type + a module-evolution table."""
    if matches is None:
        matches = [dict(r) for r in conn.execute(
            "SELECT * FROM function_matches WHERE source_version=? AND target_version=?",
            (source_ver, target_ver))]
    by_type = {}
    for m in matches:
        by_type[m["match_type"]] = by_type.get(m["match_type"], 0) + 1
    total_src = conn.execute("SELECT COUNT(*) FROM functions WHERE version_id=?",
                             (source_ver,)).fetchone()[0]
    return {"source": source_ver, "target": target_ver,
            "source_functions": total_src, "matched": len(matches),
            "by_type": by_type}
