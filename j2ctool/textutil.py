# -*- coding: utf-8 -*-
"""Small shared text/IO helpers used across the transpiler.

NOTE ON "DUPLICATION": several modules define their own `_split_top_commas`, `_balanced_paren`,
`sub_outside_literals`, etc. Those look like duplicates but are deliberately *context-tuned* and
are NOT interchangeable -- e.g. `parse._split_top_commas` tracks `<>` while
`emit.statements._split_top_commas` intentionally does not (it would mis-balance `->`) and instead
tracks `{}` and backslash escapes. They are left in place on purpose. Only the genuinely-identical
utilities below are centralized here.
"""

import re

# A string-or-char literal (with backslash escapes). Used to mask literals before applying regex
# substitutions to code. Identical across the emit back-end.
STR_LITERAL = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')


def read_text(path, errors='replace', missing_ok=True):
    """Read a UTF-8 text file.

    errors    -- passed to open() ('replace' tolerates undecodable bytes; 'strict' raises).
    missing_ok -- when True, an IOError/OSError (e.g. missing file) yields '' instead of raising.
    """
    try:
        with open(path, 'r', encoding='utf-8', errors=errors) as f:
            return f.read()
    except (IOError, OSError):
        if missing_ok:
            return ''
        raise


def write_text(path, text):
    """Write a UTF-8 text file, overwriting."""
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)
