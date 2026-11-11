#!/usr/bin/env python3
"""inspect.render on a table whose first record has an empty list where later records hold records.

    python tests/test_inspect_render.py

A column is nested (its records shown under the row) when ANY row holds records under it; deciding by the
first row alone printed later rows' records as one crowded cell (info --section imports on a 16-bit module
whose first import module has no symbols)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.formats import inspect  # noqa: E402


def test_nested_column_is_decided_by_every_row():
    table = [{"module": "A.DLL", "count": 0, "symbols": []},
             {"module": "B.DLL", "count": 1, "symbols": [{"name": "f", "ordinal": 3}]}]
    assert inspect.render(table, "imports") == [
        "module  count",
        "A.DLL   0",
        "    (none)",
        "B.DLL   1",
        "    name  ordinal",
        "    f     3"], inspect.render(table, "imports")


def test_plain_and_first_row_cases_are_unchanged():
    table = [{"module": "A.DLL", "count": 1, "symbols": [{"name": "f", "ordinal": 3}]},
             {"module": "B.DLL", "count": 0, "symbols": []}]
    assert inspect.render(table, "imports") == [
        "module  count",
        "A.DLL   1",
        "    name  ordinal",
        "    f     3",
        "B.DLL   0",
        "    (none)"]
    assert inspect.render([{"a": 1, "flags": ["X", "Y"]}], "t") == ["a  flags", "1  X Y"]


def main():
    failed = 0
    for fn in (test_nested_column_is_decided_by_every_row, test_plain_and_first_row_cases_are_unchanged):
        try:
            fn()
            print("  ok   %s" % fn.__name__)
        except Exception as e:
            failed += 1
            print("  FAIL %s  -- %s" % (fn.__name__, e))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
