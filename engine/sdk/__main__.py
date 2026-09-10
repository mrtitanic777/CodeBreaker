#!/usr/bin/env python3
r"""CLI for SDK/DDK ingestion (Phase 3):  python -m engine.sdk <cmd>

  scan   <root>                 classify an SDK/DDK tree (headers/defs/libs/bins/tools)
  detect <root>                 identify toolchain executables -> backbone adapter names
  plan   <root>                 the chosen toolchain, cross-checked against the backbone
  api    <hdr> [hdr..] [--def d] parse prototypes -> typed signatures (msc6dec form)
  recon  <binary> [--verify] [--sdk <include-dir>]   per-segment deconstruct+invert; coverage
  emit   <binary> <out-dir>      write a reconstructed source tree (byte-exact + stubs)
  refine <binary> <seg> <off> <c-file>   oracle-check a candidate C against a function
  sweep  <dir|dll ...>           auto-coverage across many NE binaries (fast, no verify)
  kits                           list discovered SDK/DDK/CRT kits (headers + lib dirs)
"""
import os, sys
from engine.sdk import ingest, detect, apidb


def _sdk_db(rest):
    """Build an SDK apidb from a `--sdk <include-dir>` flag, else None."""
    if "--sdk" not in rest:
        return None
    from engine.sdk import apidb
    return apidb.ApiDb().add_dir(rest[rest.index("--sdk") + 1])


def main(argv):
    if not argv or argv[0] in ("-h", "--help", "help"):
        print(__doc__); return 0
    cmd, rest = argv[0], argv[1:]
    if cmd == "scan":
        inv = ingest.scan(rest[0])
        print("root:", inv.root)
        for k, v in inv.summary().items():
            print("  %-9s %d" % (k, v))
        return 0
    if cmd in ("detect", "plan"):
        inv = ingest.scan(rest[0])
        if cmd == "detect":
            for t in detect.identify_tools(inv):
                print("  %-11s %-11s %s" % (t["role"], t["adapter"] or "-",
                                            os.path.basename(t["path"])))
        else:
            p = detect.plan(inv)
            for role, v in p.items():
                if role == "_unidentified":
                    print("  unidentified: %d tool(s)" % len(v))
                else:
                    print("  %-11s -> %-11s available=%s" % (role, v["adapter"], v.get("available")))
        return 0
    if cmd == "api":
        hdrs = [a for a in rest if not a.startswith("--")]
        defs = []
        if "--def" in rest:
            defs = [rest[rest.index("--def") + 1]]
            hdrs = [h for h in hdrs if h != defs[0]]
        db = apidb.ApiDb.from_paths(hdrs, defs)
        for nm, f in sorted(db.funcs.items()):
            print("  %-18s ret=%-5s @%-4s %s" % (
                f["name"], f["ret"], f["ordinal"], f["params"]))
        u = db.unknowns()
        if u:
            print("  unknowns (review):", u)
        return 0
    if cmd == "recon":
        from engine.sdk import reconstruct
        binp = [a for a in rest if not a.startswith("--")][0]
        rep = reconstruct.analyze(binp, _sdk_db(rest))
        reconstruct.print_coverage(rep, reasons=True)
        if "--verify" in rest:
            print("  verifying inverted functions (recompiling each)...")
            n = reconstruct.verify_inverted(rep)
            prod = sum(s["inverted"] for s in rep["segments"])
            print("  BYTE-EXACT: %d / %d produced" % (n, prod))
        return 0
    if cmd == "emit":
        from engine.sdk import reconstruct, emit
        pos = [a for a in rest if not a.startswith("--")]
        binp, outd = pos[0], pos[1]
        r = emit.write_tree(reconstruct.analyze(binp, _sdk_db(rest)), outd, binp,
                            verify="--verify" in rest)
        print("wrote %d files to %s: %d verified byte-exact, %d candidate, %d pending-link, %d @RECON-UNKNOWN"
              % (len(r["files"]), r["dir"], r["exact"], r["candidate"], r["link"], r["todo"]))
        return 0
    if cmd == "refine":
        import struct
        from engine.format_loaders import ne
        from engine.sdk import reconstruct as R, refine
        binp, segn, off, cfile = rest[0], int(rest[1]), int(rest[2], 0), rest[3]
        d = open(binp, "rb").read(); ne_off = struct.unpack_from("<I", d, 0x3c)[0]
        f = ne.NEFile.from_buffer(d, ne_off, binp); seg = f.segments()[segn - 1]
        segb = d[seg["file_off"]:seg["file_off"] + seg["length"]]
        e = R._func_end(segb, off, seg["length"]) or off + 64
        target = segb[off:e]
        res = refine.check(target, open(cfile).read())
        print("byte-exact:", res["exact"], "recipe:", res.get("recipe"))
        if not res["exact"]:
            print(res.get("report"))
            for dgn in res.get("diagnosis", []):
                print("  >> likely: %s -- %s\n     fix: %s" % (dgn["pattern"], dgn["cause"], dgn["fix"]))
        return 0 if res["exact"] else 1
    if cmd == "kits":
        from engine.sdk import kits
        reg = kits.Registry.discover()
        print("discovered %d SDK/DDK/CRT kits:" % len(reg.kits))
        for k in reg.kits:
            de = k.describe()
            print("  %-10s %-32s %d-bit  %4d headers  %d libdirs%s"
                  % (de["key"], de["name"], de["bits"], de["n_headers"], len(de["lib"]),
                     "  [CRT]" if de["crt"] else ""))
        return 0
    if cmd == "sweep":
        from engine.sdk import reconstruct as R
        rows = R.sweep(rest)
        print("%-14s %6s %7s %9s %11s" % ("module", "funcs", "framed", "produced*", "needs-link"))
        tf = tp = 0
        for r in sorted(rows, key=lambda x: -x.get("framed", 0)):
            if "error" in r:
                print("%-14s ERR %s" % (r["name"], r["error"])); continue
            tf += r["framed"]; tp += r["produced"]
            print("%-14s %6d %7d %9d %11d" % (r["name"], r["funcs"], r["framed"],
                                              r["produced"], r["needs_link"]))
        print("-- TOTAL framed=%d produced=%d (%.0f%%)" % (tf, tp, 100.0 * tp / tf if tf else 0))
        print("   * produced = msc6dec output (CANDIDATES); byte-exact requires oracle verify")
        print("     (use `wt sdk recon <dll> --verify` or `emit --verify` for the real count)")
        return 0
    print("unknown command %r (try: scan, detect, plan, api, recon, emit, refine, sweep)" % cmd); return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
