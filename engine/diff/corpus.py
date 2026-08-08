#!/usr/bin/env python3
r"""corpus.py -- version registry + SQLite ingest for CodeBreaker Diff.

Maps each OS snapshot to the reconstructed-asm tree that actually exists in this
repo (the layout is `<ver>/asm/...`; win95 keeps its under `win95/src/asm`),
enumerates one function per .asm file, fingerprints it, and
records a row. Ingest is idempotent and resumable: an already-present function is
skipped unless --force, so a 224k-file win95 run can be stopped and continued.

The cross-version axis that genuinely exists here is win101 -> win310 -> win95:
16-bit real-mode, 16-bit protected-mode, 32-bit flat PE -- the exact architecture
jump the fingerprint is built to see through.
"""
import hashlib
import os
import re
import sqlite3
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))       # engine/diff
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))   # repo parent
sys.path.insert(0, HERE)
import extractor  # noqa: E402

DEFAULT_DB = os.path.join(HERE, "corpus.db")

# version_id -> corpus description. asm_root is repo-relative.
VERSIONS = {
    "win101":     {"asm_root": "win1.0/asm",    "architecture": "x86_16_real",
                   "release_date": "1985-11-20"},
    "win310":     {"asm_root": "win3.1/asm",     "architecture": "x86_16_prot",
                   "release_date": "1992-04-06"},
    "win95_gold": {"asm_root": "win95/src/asm",  "architecture": "x86_32_flat",
                   "release_date": "1995-08-24"},
}

_FN_RE = re.compile(r"^(seg|sec)([0-9a-fA-F]+)_([0-9a-fA-F]+)\.asm$", re.I)


def open_db(path=DEFAULT_DB):
    conn = sqlite3.connect(path)
    conn.row_factory = sqlite3.Row
    return conn


def init_db(conn):
    with open(os.path.join(HERE, "schema.sql"), encoding="utf-8") as f:
        conn.executescript(f.read())
    conn.commit()


def parse_path(asm_root_abs, path_abs):
    """(group, module, seg, offset_hex) from a corpus .asm path, or None if the
    filename is not a `<seg|sec><n>_<off>.asm` function file."""
    rel = os.path.relpath(path_abs, asm_root_abs).replace("\\", "/")
    parts = rel.split("/")
    m = _FN_RE.match(parts[-1])
    if not m or len(parts) < 2:
        return None
    seg = m.group(2).lstrip("0") or "0"
    off = m.group(3).lower()
    module = parts[-2]
    group = "/".join(parts[:-2])            # "", "apps", "system", ... (any depth)
    return group, module, seg, off


def function_id(version_id, module, seg, off):
    key = "%s|%s|%s:%s" % (version_id, module, seg, off)
    return hashlib.sha1(key.encode()).hexdigest()


def _detect_bits(path):
    try:
        head = open(path, encoding="latin1").read(2000)
    except OSError:
        return None
    return 32 if "use32" in head else 16


def iter_functions(version_id):
    """Yield (path_abs, group, module, seg, off) for every function file."""
    root = os.path.join(REPO, VERSIONS[version_id]["asm_root"])
    for dirpath, _dirs, files in os.walk(root):
        for fn in files:
            if not fn.lower().endswith(".asm"):
                continue
            p = os.path.join(dirpath, fn)
            parsed = parse_path(root, p)
            if parsed:
                yield (p, ) + parsed


def register_version(conn, version_id):
    v = VERSIONS[version_id]
    conn.execute(
        "INSERT OR REPLACE INTO os_versions(version_id, release_date, architecture,"
        " asm_root, ingested_at) VALUES (?,?,?,?,?)",
        (version_id, v["release_date"], v["architecture"], v["asm_root"],
         time.strftime("%Y-%m-%dT%H:%M:%S")))
    conn.commit()


def ingest(conn, version_id, limit=None, modules=None, force=False, progress=None):
    """Fingerprint and record every function of `version_id`. Returns (added, skipped,
    errored). Resumable: skips rows already present unless force."""
    if version_id not in VERSIONS:
        raise SystemExit("unknown version %r (known: %s)"
                         % (version_id, ", ".join(sorted(VERSIONS))))
    register_version(conn, version_id)
    root = os.path.join(REPO, VERSIONS[version_id]["asm_root"])
    if not os.path.isdir(root):
        raise SystemExit("corpus root missing: %s" % root)

    have = set()
    if not force:
        have = {r[0] for r in conn.execute(
            "SELECT function_id FROM functions WHERE version_id=?", (version_id,))}
    modset = set(modules) if modules else None

    added = skipped = errored = 0
    batch = []
    for path, group, module, seg, off in iter_functions(version_id):
        if modset and module not in modset:
            continue
        fid = function_id(version_id, module, seg, off)
        if fid in have:
            skipped += 1
            continue
        try:
            fp = extractor.fingerprint(path)
        except Exception:                    # a malformed function must not abort a 224k run
            errored += 1
            continue
        seg_off = "%s%s:0x%s" % ("sec" if VERSIONS[version_id]["architecture"]
                                 == "x86_32_flat" else "seg", seg, off)
        batch.append((
            fid, version_id, module, group, seg_off, "sub_%s_%s" % (seg, off), None,
            _detect_bits(path),
            fp["cfg_node_count"], fp["cfg_edge_count"], fp["cyclomatic_complexity"],
            fp["cfg_topology_hash"], fp["insn_count"], fp["normalized_opcode_hash"],
            fp["call_count"], fp["call_vector_hash"], None, None,
            os.path.relpath(path, REPO).replace("\\", "/")))
        added += 1
        if len(batch) >= 2000:
            _flush(conn, batch); batch = []
            if progress:
                progress(added, skipped, errored)
        if limit and added >= limit:
            break
    if batch:
        _flush(conn, batch)
    conn.commit()
    return added, skipped, errored


def _flush(conn, batch):
    conn.executemany(
        "INSERT OR REPLACE INTO functions(function_id, version_id, module_name,"
        " group_name, segment_offset, symbol_name, ordinal, bits, cfg_node_count,"
        " cfg_edge_count, cyclomatic_complexity, cfg_topology_hash, insn_count,"
        " normalized_opcode_hash, call_count, call_vector_hash, param_count,"
        " returns_value, src_path) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
        batch)
    conn.commit()
