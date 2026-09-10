#!/usr/bin/env python3
r"""common.py -- shared CLI helpers for CodeBreaker subsystem command modules.

These were copy-pasted into ~13 command files (each with its own `_opt`). Factored here so there
is one definition. Command modules use:

    from codebreaker.cli.common import opt as _opt
    n = int(_opt(argv, "--limit", "30"))
"""


def opt(argv, name, default=None):
    """The value following `name` in argv (e.g. opt(argv, '--limit')), or `default` if absent."""
    return argv[argv.index(name) + 1] if name in argv and argv.index(name) + 1 < len(argv) else default
