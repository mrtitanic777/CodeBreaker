#!/usr/bin/env python3
"""
cleanroom_stripper.py

A small, self-contained CLI that walks a directory of C / C++ / assembly source
files and, for every file it recognizes:

  1. Removes ALL comments (string- and char-literal aware, so it never touches a
     "//" or ";" that lives inside a string), and
  2. Inserts a fresh one-line comment header directly above every function it can
     identify. The header text is:

         To be rewritten as clean room

By DEFAULT the tool runs as a preview (dry run): it reports exactly what it would
change but writes nothing. Add --apply to actually rewrite the files.

Usage examples (PowerShell):

    python tools\\cleanroom_stripper.py C:\\path\\to\\code
    python tools\\cleanroom_stripper.py C:\\path\\to\\code --apply
    python tools\\cleanroom_stripper.py C:\\path\\to\\code --apply --backup

Or, from the repo root, use the wrapper:

    cleanroom C:\\path\\to\\code --apply

Notes / limitations (this is a pragmatic heuristic tool, not a full compiler):
  * C/C++ comment stripping is exact (a real character-level scanner that
    understands "..." strings, '.' char literals, raw R"(...)" strings, line
    comments and /* block */ comments).
  * C/C++ function detection is a heuristic: it finds `name(...) {` definitions
    and skips control keywords (if/for/while/...). K&R-style definitions are not
    detected. False positives are harmless (they only add a comment line).
  * ASM comment stripping removes ';' comments (MASM/NASM) plus '//' and
    '/* */' (GAS-style), string-aware.
  * ASM function detection annotates MASM `name PROC` definitions and the label
    definitions of symbols exported via `global` / `.globl` / `.global`. If a
    file has neither, no ASM functions are annotated.
"""

import argparse
import os
import re
import sys

HEADER_TEXT = "To be rewritten as clean room"

# --- extension -> language -------------------------------------------------

C_EXTS = {
    ".c", ".h", ".cc", ".cpp", ".cxx", ".c++", ".hpp", ".hh", ".hxx",
    ".h++", ".inl", ".ipp", ".tcc", ".m", ".mm",
}
# .inc is treated as assembly here (this repo is NASM/MASM oriented).
ASM_EXTS = {".asm", ".nasm", ".inc", ".s", ".asmx"}
ASM_GAS_EXTS = {".s"}  # capital .S handled case-insensitively below

DEFAULT_EXCLUDES = {".git", ".svn", ".hg"}

# C / C++ keywords that can appear as `word ( ... ) {` but are NOT functions.
C_NON_FUNCTION_WORDS = {
    "if", "for", "while", "switch", "do", "else", "return", "catch",
    "sizeof", "alignof", "decltype", "typeof", "typeof_unqual", "case",
    "goto", "new", "delete", "and", "or", "not", "defined", "asm",
    "__asm", "__asm__", "typedef", "using", "template", "operator",
    "static_assert", "_Static_assert", "__attribute__", "__declspec",
    "noexcept", "constexpr", "consteval", "requires", "co_await",
}


# ---------------------------------------------------------------------------
# C / C++ scanner
# ---------------------------------------------------------------------------

def scan_c(src):
    """Return (stripped, masked, comment_count) for C/C++ source.

    `stripped` is `src` with every comment removed but strings intact.
    `masked` is the same length as `stripped`, byte-for-byte aligned, but with
    the *contents* of string and character literals blanked to spaces so that
    braces/parens inside literals do not confuse function detection.
    """
    out = []
    mask = []
    i = 0
    n = len(src)
    ncmt = 0

    def emit(co, cm):
        out.append(co)
        mask.append(cm)

    while i < n:
        c = src[i]
        nxt = src[i + 1] if i + 1 < n else ""

        # ---- line comment: // ... (with backslash-newline continuation) ----
        if c == "/" and nxt == "/":
            ncmt += 1
            i += 2
            while i < n and src[i] != "\n":
                if src[i] == "\\" and src.startswith("\\\n", i):
                    i += 2
                    continue
                if src[i] == "\\" and src.startswith("\\\r\n", i):
                    i += 3
                    continue
                i += 1
            continue  # leave the terminating newline for the next iteration

        # ---- block comment: /* ... */ (newlines preserved) ----
        if c == "/" and nxt == "*":
            ncmt += 1
            i += 2
            while i < n and not (src[i] == "*" and src.startswith("*/", i)):
                if src[i] == "\n":
                    emit("\n", "\n")
                i += 1
            i += 2  # consume the closing */
            continue

        # ---- raw string literal: R"delim( ... )delim" ----
        if c == "R" and nxt == '"':
            j = i + 2
            delim = []
            while j < n and src[j] not in "( )\\\t\r\n":
                delim.append(src[j])
                j += 1
            if j < n and src[j] == "(":
                delim = "".join(delim)
                term = ")" + delim + '"'
                # opening R"delim(
                for ch in src[i:j + 1]:
                    emit(ch, " ")
                i = j + 1
                while i < n and not src.startswith(term, i):
                    emit(src[i], "\n" if src[i] == "\n" else " ")
                    i += 1
                for ch in term:
                    emit(ch, " ")
                i += len(term)
                continue
            # not actually a raw string; fall through as a normal char

        # ---- normal string literal: "..." ----
        if c == '"':
            emit('"', " ")
            i += 1
            while i < n:
                ch = src[i]
                if ch == "\\" and i + 1 < n:
                    emit(ch, " ")
                    emit(src[i + 1], " ")
                    i += 2
                    continue
                emit(ch, "\n" if ch == "\n" else " ")
                i += 1
                if ch == '"':
                    break
            continue

        # ---- char literal: '...' ----
        if c == "'":
            emit("'", " ")
            i += 1
            while i < n:
                ch = src[i]
                if ch == "\\" and i + 1 < n:
                    emit(ch, " ")
                    emit(src[i + 1], " ")
                    i += 2
                    continue
                emit(ch, "\n" if ch == "\n" else " ")
                i += 1
                if ch == "'":
                    break
            continue

        # ---- ordinary character ----
        emit(c, c)
        i += 1

    stripped = "".join(out)
    masked = "".join(mask)
    assert len(stripped) == len(masked), "scanner alignment invariant broken"
    return stripped, masked, ncmt


_C_FUNC_RE = re.compile(
    r"(?P<name>[A-Za-z_~][A-Za-z0-9_]*)\s*"
    r"\((?P<params>[^;{}()]*(?:\([^;{}()]*\)[^;{}()]*)*)\)"
    r"(?P<tail>[^;{}]*?)\{",
    re.DOTALL,
)


def find_c_function_offsets(masked):
    """Return a list of character offsets (into the masked/stripped text) at the
    start of each detected function's signature line-block."""
    offsets = []
    for m in _C_FUNC_RE.finditer(masked):
        name = m.group("name")
        if name in C_NON_FUNCTION_WORDS:
            continue
        offsets.append(m.start("name"))
    return offsets


# ---------------------------------------------------------------------------
# ASM scanner
# ---------------------------------------------------------------------------

def scan_asm(src, gas_style):
    """Return (stripped, masked, comment_count) for assembly source.

    Removes ';' comments always, plus '//' line and '/* */' block comments.
    String-aware for both '...' and "..." literals.
    """
    out = []
    mask = []
    i = 0
    n = len(src)
    ncmt = 0

    def emit(co, cm):
        out.append(co)
        mask.append(cm)

    while i < n:
        c = src[i]
        nxt = src[i + 1] if i + 1 < n else ""

        # ';' line comment (MASM / NASM)
        if c == ";":
            ncmt += 1
            while i < n and src[i] != "\n":
                i += 1
            continue

        # '//' line comment (GAS-style; harmless elsewhere)
        if c == "/" and nxt == "/":
            ncmt += 1
            while i < n and src[i] != "\n":
                i += 1
            continue

        # '/* ... */' block comment
        if c == "/" and nxt == "*":
            ncmt += 1
            i += 2
            while i < n and not (src[i] == "*" and src.startswith("*/", i)):
                if src[i] == "\n":
                    emit("\n", "\n")
                i += 1
            i += 2
            continue

        # string literals: "..." and '...'
        if c == '"' or c == "'":
            quote = c
            emit(c, " ")
            i += 1
            while i < n:
                ch = src[i]
                if ch == "\\" and i + 1 < n:
                    emit(ch, " ")
                    emit(src[i + 1], " ")
                    i += 2
                    continue
                emit(ch, "\n" if ch == "\n" else " ")
                i += 1
                if ch == quote:
                    break
            continue

        emit(c, c)
        i += 1

    stripped = "".join(out)
    masked = "".join(mask)
    assert len(stripped) == len(masked), "scanner alignment invariant broken"
    return stripped, masked, ncmt


_MASM_PROC_RE = re.compile(
    r"^[ \t]*(?P<name>[A-Za-z_$@?][\w$@?.]*)[ \t]+PROC\b",
    re.MULTILINE | re.IGNORECASE,
)
_GLOBAL_RE = re.compile(
    r"^[ \t]*(?:global|globl|\.globl|\.global)[ \t]+(?P<names>[^\n;/]+)",
    re.MULTILINE | re.IGNORECASE,
)


def find_asm_function_offsets(masked):
    """Return offsets for MASM PROC definitions and for exported-label
    definitions (symbols named in `global` / `.globl` directives)."""
    offsets = set()

    for m in _MASM_PROC_RE.finditer(masked):
        # anchor on the start of the name
        offsets.add(m.start("name"))

    # collect exported symbol names
    exported = set()
    for m in _GLOBAL_RE.finditer(masked):
        chunk = m.group("names")
        for tok in re.split(r"[,\s]+", chunk.strip()):
            if not tok:
                continue
            # NASM allows `global foo:function` -- keep the symbol part only
            tok = tok.split(":", 1)[0].strip()
            if re.fullmatch(r"[A-Za-z_.$@?][\w$@?.]*", tok):
                exported.add(tok)

    for name in exported:
        # label definition: `name:` at start of a line
        label_re = re.compile(
            r"^[ \t]*" + re.escape(name) + r"[ \t]*:",
            re.MULTILINE,
        )
        for m in label_re.finditer(masked):
            offsets.add(m.start())

    return sorted(offsets)


# ---------------------------------------------------------------------------
# Header insertion (shared)
# ---------------------------------------------------------------------------

def _line_start(text, off):
    nl = text.rfind("\n", 0, off)
    return nl + 1


def _prev_nonblank_line_bounds(text, line_start):
    """Return (start, end) of the line immediately preceding `line_start`,
    or None if there is none."""
    if line_start <= 0:
        return None
    prev_end = line_start - 1  # index of the '\n' that ends the previous line
    prev_start = text.rfind("\n", 0, prev_end) + 1
    return prev_start, prev_end


def _signature_block_start(text, name_off):
    """Walk upward from the function name's line to the true first line of the
    signature block (handles multi-line return types / parameter lists)."""
    ls = _line_start(text, name_off)
    while True:
        prev = _prev_nonblank_line_bounds(text, ls)
        if prev is None:
            break
        p_start, p_end = prev
        prev_line = text[p_start:p_end]
        stripped = prev_line.strip()
        # Stop conditions: previous line clearly terminates a prior construct,
        # is blank, is a preprocessor directive, or is a label / comment header.
        if stripped == "":
            break
        if stripped[0] == "#":
            break
        if stripped.endswith((";", "{", "}", ":")):
            break
        # otherwise assume the signature continues upward (e.g. return type)
        ls = p_start
    return ls


def annotate(stripped, offsets, comment_prefix, walk_up=True):
    """Insert a header comment above each offset. Returns (text, count_added).

    When `walk_up` is True (C/C++), the insertion point climbs to the true first
    line of a possibly multi-line signature. When False (ASM), the header goes
    directly above the matched line (the label / PROC line is the anchor).
    """
    header_line_core = comment_prefix + " " + HEADER_TEXT
    added = 0
    # Insert from the end so earlier offsets stay valid.
    seen_line_starts = set()
    for off in sorted(offsets, reverse=True):
        block_start = _signature_block_start(stripped, off) if walk_up \
            else _line_start(stripped, off)
        if block_start in seen_line_starts:
            continue
        seen_line_starts.add(block_start)

        # indentation of the signature's first line
        j = block_start
        while j < len(stripped) and stripped[j] in " \t":
            j += 1
        indent = stripped[block_start:j]

        # skip if the line directly above is already our header
        prev = _prev_nonblank_line_bounds(stripped, block_start)
        if prev is not None:
            p_start, p_end = prev
            if stripped[p_start:p_end].strip() == header_line_core.strip():
                continue

        header = indent + header_line_core + "\n"
        stripped = stripped[:block_start] + header + stripped[block_start:]
        added += 1
    return stripped, added


def tidy(text):
    """Trim trailing whitespace from every line (comment removal often leaves
    some behind) and collapse runs of 2+ blank lines into a single blank line."""
    lines = text.split("\n")
    result = []
    blank = False
    for ln in lines:
        ln = ln.rstrip()
        if ln == "":
            if blank:
                continue
            blank = True
        else:
            blank = False
        result.append(ln)
    # drop leading blank lines (e.g. left behind by a stripped top-of-file
    # comment or a previously-stripped header)
    while result and result[0] == "":
        result.pop(0)
    # normalize to exactly one trailing newline
    while result and result[-1] == "":
        result.pop()
    return "\n".join(result) + "\n" if result else ""


# ---------------------------------------------------------------------------
# Per-file processing
# ---------------------------------------------------------------------------

def classify(path):
    ext = os.path.splitext(path)[1].lower()
    if ext in C_EXTS:
        return "c"
    if ext in ASM_EXTS:
        return "asm"
    return None


def process_text(text, lang, ext_lower):
    if lang == "c":
        stripped, masked, ncmt = scan_c(text)
        offsets = find_c_function_offsets(masked)
        prefix = "//"
    else:
        gas = ext_lower in (".s",)
        stripped, masked, ncmt = scan_asm(text, gas)
        offsets = find_asm_function_offsets(masked)
        prefix = ";"
    annotated, nfun = annotate(stripped, offsets, prefix, walk_up=(lang == "c"))
    annotated = tidy(annotated)
    return annotated, ncmt, nfun


def iter_files(root, excludes):
    if os.path.isfile(root):
        yield root
        return
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames[:] = [d for d in dirnames if d not in excludes]
        for fn in filenames:
            yield os.path.join(dirpath, fn)


def main(argv=None):
    ap = argparse.ArgumentParser(
        prog="cleanroom_stripper",
        description="Strip all comments from C/C++/ASM files and stamp a "
                    '"%s" header over every function.' % HEADER_TEXT,
    )
    ap.add_argument("path", help="File or directory to scan.")
    ap.add_argument("--apply", action="store_true",
                    help="Actually rewrite files. Without this, runs as a "
                         "preview (no files are changed).")
    ap.add_argument("--backup", action="store_true",
                    help="When applying, save a .bak copy of each changed file.")
    ap.add_argument("--exclude", default="",
                    help="Comma-separated directory names to skip "
                         "(always skips .git/.svn/.hg).")
    ap.add_argument("-q", "--quiet", action="store_true",
                    help="Only print the final summary.")
    args = ap.parse_args(argv)

    if not os.path.exists(args.path):
        print("error: path does not exist: %s" % args.path, file=sys.stderr)
        return 2

    excludes = set(DEFAULT_EXCLUDES)
    excludes.update(x.strip() for x in args.exclude.split(",") if x.strip())

    total_files = 0
    changed_files = 0
    total_comments = 0
    total_functions = 0
    errors = 0

    for path in iter_files(args.path, excludes):
        lang = classify(path)
        if lang is None:
            continue
        total_files += 1
        ext_lower = os.path.splitext(path)[1].lower()
        try:
            with open(path, "rb") as f:
                raw = f.read()
            text = raw.decode("utf-8", errors="surrogateescape")
        except OSError as e:
            print("  ! could not read %s: %s" % (path, e), file=sys.stderr)
            errors += 1
            continue

        try:
            new_text, ncmt, nfun = process_text(text, lang, ext_lower)
        except Exception as e:  # never let one weird file abort the whole run
            print("  ! failed to process %s: %s" % (path, e), file=sys.stderr)
            errors += 1
            continue

        will_change = (new_text != text)
        if will_change:
            changed_files += 1
        total_comments += ncmt
        total_functions += nfun

        if not args.quiet and (will_change or ncmt or nfun):
            tag = "would change" if not args.apply else "changed"
            if not will_change:
                tag = "no change"
            print("  [%s] %s  (comments removed: %d, functions marked: %d)"
                  % (tag, os.path.relpath(path, args.path if os.path.isdir(args.path) else os.path.dirname(args.path) or "."),
                     ncmt, nfun))

        if args.apply and will_change:
            try:
                if args.backup:
                    with open(path + ".bak", "wb") as bf:
                        bf.write(raw)
                with open(path, "wb") as f:
                    f.write(new_text.encode("utf-8", errors="surrogateescape"))
            except OSError as e:
                print("  ! could not write %s: %s" % (path, e), file=sys.stderr)
                errors += 1

    print()
    print("=" * 60)
    mode = "APPLIED" if args.apply else "PREVIEW (no files written)"
    print("Mode:              %s" % mode)
    print("Files scanned:     %d" % total_files)
    print("Files %s: %d" % ("changed   " if args.apply else "to change ",
                            changed_files))
    print("Comments removed:  %d" % total_comments)
    print("Functions marked:  %d" % total_functions)
    if errors:
        print("Errors:            %d" % errors)
    if not args.apply and changed_files:
        print()
        print("This was a preview. Re-run with --apply to write the changes")
        print("(add --backup to also keep a .bak copy of each file).")
    print("=" * 60)
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
