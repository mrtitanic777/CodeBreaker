-- Transpiler-Diff corpus schema (SQLite).
--
-- One row per lifted function per OS snapshot, carrying the three-level canonical
-- fingerprint (CFG topology / normalized opcode stream / call vector) that lets a
-- function be matched across the 16-real -> 16-prot -> 32-flat architecture jump.
-- Matches and propagated names live in their own tables so a re-run is idempotent.

PRAGMA journal_mode = WAL;
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS os_versions (
    version_id    TEXT PRIMARY KEY,   -- "win101", "win310", "win95_gold"
    release_date  TEXT,
    architecture  TEXT,               -- "x86_16_real" | "x86_16_prot" | "x86_32_flat"
    asm_root      TEXT,               -- repo-relative corpus root actually ingested
    ingested_at   TEXT
);

CREATE TABLE IF NOT EXISTS functions (
    function_id   TEXT PRIMARY KEY,   -- sha1(version_id | module | seg:offset)
    version_id    TEXT NOT NULL,
    module_name   TEXT,               -- "USER", "GDI", "KERNEL", "CALC", ...
    group_name    TEXT,               -- corpus sub-tree: "apps" | "system" | "" ...
    segment_offset TEXT,              -- "seg1:0x0168" (16-bit) | "sec1:0x00012f40" (32)
    symbol_name   TEXT,               -- export/ordinal name, else "sub_<seg>_<off>"
    ordinal       INTEGER,
    bits          INTEGER,            -- 16 | 32 (detected per function, not per era)

    -- Level 1: CFG features
    cfg_node_count       INTEGER,
    cfg_edge_count       INTEGER,
    cyclomatic_complexity INTEGER,
    cfg_topology_hash    TEXT,        -- sha256 of the block-adjacency listing

    -- Level 2: normalized opcode stream
    insn_count           INTEGER,
    normalized_opcode_hash TEXT,      -- sha256 of the register/imm/mem-masked stream

    -- Level 3: AST / call-graph features
    call_count           INTEGER,
    call_vector_hash     TEXT,        -- sha256 of external call targets, in order
    param_count          INTEGER,     -- best-effort (nulled until arg recovery joins in)
    returns_value        INTEGER,     -- 0/1/NULL

    src_path      TEXT,               -- repo-relative .asm the row was lifted from
    FOREIGN KEY (version_id) REFERENCES os_versions(version_id)
);

CREATE INDEX IF NOT EXISTS ix_fn_version   ON functions(version_id);
CREATE INDEX IF NOT EXISTS ix_fn_module    ON functions(version_id, module_name);
CREATE INDEX IF NOT EXISTS ix_fn_cfg       ON functions(cfg_topology_hash);
CREATE INDEX IF NOT EXISTS ix_fn_opcode    ON functions(normalized_opcode_hash);
CREATE INDEX IF NOT EXISTS ix_fn_calls     ON functions(call_vector_hash);
-- the matcher's fast-path bucket: complexity is the cheap pre-filter
CREATE INDEX IF NOT EXISTS ix_fn_cyclo     ON functions(version_id, cyclomatic_complexity);

CREATE TABLE IF NOT EXISTS function_matches (
    source_func_id  TEXT NOT NULL,
    target_func_id  TEXT NOT NULL,
    source_version  TEXT NOT NULL,
    target_version  TEXT NOT NULL,
    similarity_score REAL NOT NULL,   -- 0.00 .. 1.00
    match_type      TEXT,             -- "EXACT" | "CFG_OPCODE" | "CALL_GRAPH" | "STRUCT"
    PRIMARY KEY (source_func_id, target_func_id),
    FOREIGN KEY (source_func_id) REFERENCES functions(function_id),
    FOREIGN KEY (target_func_id) REFERENCES functions(function_id)
);

CREATE INDEX IF NOT EXISTS ix_match_pair ON function_matches(source_version, target_version);

-- Audit trail for the propagator: every name it writes, and the evidence, so a
-- propagated label is never confused with a hand-recovered one and can be undone.
CREATE TABLE IF NOT EXISTS propagations (
    target_func_id  TEXT NOT NULL,
    old_name        TEXT,
    new_name        TEXT,
    source_func_id  TEXT,
    similarity_score REAL,
    applied_at      TEXT,
    PRIMARY KEY (target_func_id, new_name)
);
