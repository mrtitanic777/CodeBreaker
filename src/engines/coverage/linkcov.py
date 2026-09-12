#!/usr/bin/env python3
r"""
linkcov.py -- whole-corpus link-coverage report.

Per-function byte-exactness is proven and recorded in the conduct ledger
(reports/conduct/<module>.json). Whole-file linking (linkfile.py) adds placement
+ fixup + relocation on top, proven byte-identical across every format and edge
case on an empirical sample. This report joins the two:

  * ledger  -> which modules are FULLY byte-exact (every carved function exact),
               so they can rebuild byte-identical; plus % live (real recovered
               disassembly vs db-fallback), the meaningful quality number.
  * sample  -> actual linkfile.link_module verdicts (reports/link_sample.jsonl),
               empirical proof the whole-file link step holds.

Grouped by tier and format.

    python linkcov.py                 # ledger report -> reports/link_coverage.md
    python linkcov.py --sample 12     # ALSO link up to N modules/tier to verify
"""
import glob
from src.core import cores
import json
import os
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")

LEDGER_DIR = os.path.join("reports", "conduct")
SAMPLE_JSONL = os.path.join("reports", "link_sample.jsonl")
REPORT_MD = os.path.join("reports", "link_coverage.md")


def _load_ledgers():
    """[{module, tier, fmt, n, exact, partial, error, live, total, db, ready, asm}].
    `asm` = count of .asm actually emitted to disk. A module can be ledger-exact
    yet have fewer .asm than exact functions (banked without --emit) -- linking such
    a module byte-matches only by RETAINING shipped bytes at the un-emitted gaps,
    not by placing reconstructed source, so `emit_gap = exact - asm` must be zero
    for a module to count as genuinely link-complete."""
    target = cores.get('target')
    from src.engines.recon import linkfile
    rows = []
    for p in sorted(glob.glob(os.path.join(LEDGER_DIR, "*.json"))):
        mod = os.path.basename(p)[:-5]                # strip .json
        try:
            d = json.load(open(p, encoding="utf-8"))
        except Exception:
            continue
        if not isinstance(d, dict) or not d:
            continue
        try:
            path = target.resolve(mod)
            tier, fmt = target.tier_of(path), target.format_of(path)
        except Exception:
            tier, fmt = "?", "?"
        n = exact = partial = error = live = total = db = 0
        for v in d.values():
            if not isinstance(v, dict):
                continue
            n += 1
            st = v.get("status")
            exact += st == "exact"
            partial += st == "partial"
            error += st == "error"
            live += v.get("live", 0) or 0
            total += v.get("total", 0) or 0
            db += v.get("db", 0) or 0
        adir = linkfile._asm_dir(mod)
        asm = len([f for f in os.listdir(adir) if f.endswith(".asm")]) if adir else 0
        rows.append(dict(module=mod, tier=tier, fmt=fmt, n=n, exact=exact,
                         partial=partial, error=error, live=live, total=total, db=db,
                         asm=asm, emit_gap=max(0, exact - asm),
                         ready=(n > 0 and exact == n and asm >= exact)))
    return rows


def _load_sample():
    """{module: verdict-bool} from prior empirical link runs."""
    out = {}
    if os.path.exists(SAMPLE_JSONL):
        for line in open(SAMPLE_JSONL, encoding="utf-8"):
            line = line.strip()
            if not line:
                continue
            try:
                r = json.loads(line)
                out[r["module"]] = r
            except Exception:
                pass
    return out


def _run_sample(rows, per_tier, maxfuncs=900):
    """Link up to `per_tier` READY modules per tier that aren't sampled yet and have
    reconstructed .asm on disk; append verdicts to SAMPLE_JSONL. Auto-sample skips
    modules over `maxfuncs` functions (giant core DLLs) to stay fast -- those link
    on demand via `linkfile.py <module>`. Prefers smaller modules (fast)."""
    from src.core import config
    config.apply_config()
    from src.engines.recon import linkfile
    have = _load_sample()
    from collections import defaultdict
    buckets = defaultdict(list)
    for r in rows:
        if (r["ready"] and r["module"] not in have and 0 < r["n"] <= maxfuncs
                and linkfile._asm_dir(r["module"]) is not None):
            buckets[r["tier"]].append(r)
    f = open(SAMPLE_JSONL, "a", encoding="utf-8")
    done = 0
    for tier, rs in buckets.items():
        rs.sort(key=lambda r: r["n"])                # small first = fast
        for r in rs[:per_tier]:
            rec = {"module": r["module"], "tier": tier, "fmt": r["fmt"]}
            try:
                lr = linkfile.link_module(r["module"])
                rec.update(match=lr["match"], n_diff=lr["n_diff"],
                           n_funcs=lr["n_funcs"], size=lr["size"])
            except Exception as e:
                rec.update(match=False, error=str(e)[:150])
            f.write(json.dumps(rec) + "\n")
            f.flush()
            done += 1
            v = "IDENTICAL" if rec.get("match") else rec.get("error", "%d diffs" % rec.get("n_diff", -1))
            print("  link %-16s %-4s %-10s -> %s" % (r["module"], r["fmt"], tier, v), flush=True)
    f.close()
    print("sampled %d new module(s)" % done, flush=True)


def _fmt_report(rows, sample):
    target = cores.get('target')
    order = [t for t in target.TIER_ORDER] + ["?"]
    lines = ["# CodeBreaker -- whole-file link coverage", ""]
    lines.append("Per-function byte-exactness from the conduct ledger; a module is "
                 "**link-ready** when every carved function is `exact` (it then "
                 "rebuilds byte-identical via linkfile). `% live` = real recovered "
                 "disassembly vs trivially-exact db-fallback.")
    lines.append("")

    def block(title, subset):
        tot_m = len(subset)
        ready = sum(r["ready"] for r in subset)
        n = sum(r["n"] for r in subset)
        ex = sum(r["exact"] for r in subset)
        live = sum(r["live"] for r in subset)
        total = sum(r["total"] for r in subset)
        pl = (100.0 * live / total) if total else 0.0
        pf = (100.0 * ex / n) if n else 0.0
        smp = [sample[r["module"]] for r in subset
               if r["module"] in sample and sample[r["module"]].get("n_funcs", 0) > 0]
        smp_ok = sum(1 for s in smp if s.get("match"))
        lines.append("## %s" % title)
        lines.append("- modules: **%d**  |  link-ready (100%% exact): **%d** (%.1f%%)  |  "
                     "partial/error: %d" % (tot_m, ready, 100.0 * ready / tot_m if tot_m else 0, tot_m - ready))
        lines.append("- functions: %d  |  byte-exact: %d (%.2f%%)  |  %% live: %.1f%%"
                     % (n, ex, pf, pl))
        if smp:
            lines.append("- empirical link sample: **%d/%d byte-identical**" % (smp_ok, len(smp)))
        lines.append("")

    block("Corpus total", rows)
    lines.append("---\n\n# By tier\n")
    for t in order:
        sub = [r for r in rows if r["tier"] == t]
        if sub:
            block("tier: %s" % t, sub)
    lines.append("---\n\n# By format\n")
    for fmt in ("PE", "NE", "LE", "?"):
        sub = [r for r in rows if r["fmt"] == fmt]
        if sub:
            block("format: %s" % fmt, sub)

    # residual (not link-ready) modules, so the gap is explicit and actionable
    resid = [r for r in rows if not r["ready"]]
    lines.append("---\n\n# Residual (not yet link-ready): %d module(s)\n" % len(resid))
    if resid:
        lines.append("`emit_gap` = ledger-exact functions with no .asm on disk "
                     "(banked but not emitted; would link only via shipped-byte gaps).")
        lines.append("")
        lines.append("| module | tier | fmt | funcs | exact | partial | error | emit_gap |")
        lines.append("|---|---|---|---|---|---|---|---|")
        for r in sorted(resid, key=lambda r: (r["tier"], -(r["partial"] + r["error"] + r["emit_gap"]))):
            lines.append("| %s | %s | %s | %d | %d | %d | %d | %d |"
                         % (r["module"], r["tier"], r["fmt"], r["n"], r["exact"],
                            r["partial"], r["error"], r["emit_gap"]))
    return "\n".join(lines) + "\n"


def main():
    per_tier = 0
    if "--sample" in sys.argv:
        i = sys.argv.index("--sample")
        per_tier = int(sys.argv[i + 1]) if i + 1 < len(sys.argv) else 12

    print("loading ledgers...", flush=True)
    rows = _load_ledgers()
    print("%d module ledgers" % len(rows), flush=True)
    if per_tier:
        print("linking up to %d ready module(s)/tier to verify..." % per_tier, flush=True)
        _run_sample(rows, per_tier)
    sample = _load_sample()
    report = _fmt_report(rows, sample)
    os.makedirs("reports", exist_ok=True)
    open(REPORT_MD, "w", encoding="utf-8").write(report)

    # concise console tally
    ready = sum(r["ready"] for r in rows)
    n = sum(r["n"] for r in rows)
    ex = sum(r["exact"] for r in rows)
    print("=" * 64)
    print("LINK COVERAGE: %d/%d modules link-ready (100%% byte-exact)  |  "
          "%d/%d functions byte-exact" % (ready, len(rows), ex, n))
    real = [s for s in sample.values() if s.get("n_funcs", 0) > 0]
    if real:
        ok = sum(1 for s in real if s.get("match"))
        print("empirical link sample: %d/%d byte-identical (real links, n_funcs>0)"
              % (ok, len(real)))
    print("report -> %s" % REPORT_MD)
    print("=" * 64)


if __name__ == "__main__":
    main()
