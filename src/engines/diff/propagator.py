#!/usr/bin/env python3
r"""propagator.py -- carry recovered names across a matched version pair.

When a high-confidence match connects a NAMED function in version A to a raw
`sub_HEX` in version B, copy the name over. This is deliberately DB-only and
audited (every write lands in the `propagations` table with its evidence), and
dry-run by default -- writing names into the golden C headers is a separate,
reviewable step, because a propagated label is a lead, not proof, and must never
silently overwrite a hand-recovered name.

A source function is "named" when its symbol_name is not the synthetic `sub_...`
placeholder. Until an export/ordinal enrichment pass populates real names (the
NE/PE export tables via the engine's idord/ne), most sources are still `sub_`,
so this reports 0 -- that is honest, not a failure.
"""
import time


def _is_named(name):
    return bool(name) and not name.startswith("sub_")


def propagate_signatures(conn, source_ver, target_ver, min_score=0.85, apply=False):
    """Propagate names A->B for matches >= min_score where A is named and B is raw.
    Returns the count. With apply=False (default) nothing is written -- it only
    reports what would change."""
    pairs = conn.execute(
        "SELECT m.similarity_score s, m.source_func_id sid, m.target_func_id tid,"
        "       a.symbol_name aname, b.symbol_name bname, a.insn_count ains"
        "  FROM function_matches m"
        "  JOIN functions a ON a.function_id = m.source_func_id"
        "  JOIN functions b ON b.function_id = m.target_func_id"
        " WHERE m.source_version=? AND m.target_version=? AND m.similarity_score>=?",
        (source_ver, target_ver, min_score)).fetchall()

    # Group candidates by target. A name is only trustworthy if the target's named
    # sources AGREE: a generic skeleton (an About-dialog proc, a stub) structurally
    # matches several different named functions, and guessing one would be noise.
    by_target = {}
    for p in pairs:
        if _is_named(p["aname"]) and not _is_named(p["bname"]):
            by_target.setdefault(p["tid"], []).append(p)

    n = ambiguous = 0
    for tid, cands in by_target.items():
        distinct = {c["aname"] for c in cands}
        if len(distinct) > 1:
            ambiguous += 1
            print("  [skip] %s: ambiguous (%s) -- %d named sources disagree"
                  % (cands[0]["bname"], ", ".join(sorted(distinct)[:4]), len(distinct)))
            continue
        best = max(cands, key=lambda c: (c["s"], c["ains"] or 0))
        print("  [%s] %s <- %s  (%.2f)" % (
            "apply" if apply else "dry", best["bname"], best["aname"], best["s"]))
        if apply:
            conn.execute("UPDATE functions SET symbol_name=? WHERE function_id=?",
                         (best["aname"], tid))
            conn.execute(
                "INSERT OR REPLACE INTO propagations(target_func_id, old_name,"
                " new_name, source_func_id, similarity_score, applied_at)"
                " VALUES (?,?,?,?,?,?)",
                (tid, best["bname"], best["aname"], best["sid"], best["s"],
                 time.strftime("%Y-%m-%dT%H:%M:%S")))
        n += 1
    if ambiguous:
        print("  (%d target(s) skipped as ambiguous)" % ambiguous)
    if apply:
        conn.commit()
    return n
