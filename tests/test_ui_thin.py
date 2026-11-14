#!/usr/bin/env python3
"""The terminal interface is a thin client: with ONLY the files it runs from (codebreaker/__init__.py and, of
codebreaker/ui, __init__.py, client.py, terminal.py and terminal.tcss) in front of it -- none of the rest of the
package -- it imports, and every screen works against the native service.
Run:  python tests/test_ui_thin.py     (exit code 0 = all passed)

The tree is copied to a temporary folder and run there with `python -E` (nothing of this checkout on the path;
Textual and Rich from the site; an editable install of the package is hidden). The tour needs the native program
(CB_EXE, `exe` in settings.json or the build in state/cxx), Textual and the corpus (it seeds the function store
test_ui.py does), and is SKIPPED without them; the import check needs Textual only."""
import importlib.util
import json
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
KEEP = ("codebreaker/__init__.py", "codebreaker/ui/__init__.py", "codebreaker/ui/client.py",
        "codebreaker/ui/terminal.py", "codebreaker/ui/terminal.tcss")
EXPECT = {"codebreaker", "codebreaker.ui", "codebreaker.ui.client", "codebreaker.ui.terminal"}
HIDE = "sys.meta_path[:] = [f for f in sys.meta_path if '_EditableFinder' not in repr(f)]; "   # (an editable install of the package would find the rest)
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _only_ui(into):
    for rel in KEEP:
        os.makedirs(os.path.dirname(os.path.join(into, rel)), exist_ok=True)
        shutil.copy2(os.path.join(ROOT, rel), os.path.join(into, rel))


def _need_python():
    if not hasattr(sys, "meta_path"):
        raise Skip("run under python: the exe's translated runtime has no import system or Textual, and sys.executable is the exe")


def test_imports_only_the_ui():
    _need_python()
    if not importlib.util.find_spec("textual"):
        raise Skip("textual is not installed")
    tree = tempfile.mkdtemp(prefix="cbthin")
    try:
        _only_ui(tree)
        code = ("import sys; " + HIDE + "sys.path.insert(0, %r); import codebreaker.ui.terminal as T; "
                "import json; assert T.__file__.startswith(%r), T.__file__; "
                "print(json.dumps(sorted(m for m in sys.modules if m == 'codebreaker' or m.startswith('codebreaker.'))))"
                % (tree, tree))
        r = subprocess.run([sys.executable, "-E", "-c", code], cwd=tree, capture_output=True, text=True, timeout=120)
        assert r.returncode == 0, r.stderr
        got = set(json.loads(r.stdout.strip().splitlines()[-1]))
        assert got == EXPECT, sorted(got ^ EXPECT)
        for rel in ("codebreaker/ui/terminal.py", "codebreaker/ui/client.py"):    # (no import of the rest, lazy or not)
            with open(os.path.join(tree, rel), encoding="utf-8") as f:
                text = f.read()
            for bad in ("codebreaker.base", "codebreaker.store", "codebreaker.verify", "codebreaker.engine", "ui import service",
                        "ui import mcp_server", "ui.service", "ui.mcp_server"):
                assert bad not in text, (rel, bad)
    finally:
        shutil.rmtree(tree, ignore_errors=True)


DRIVER = r'''
import asyncio, os, sys
sys.meta_path[:] = [f for f in sys.meta_path if '_EditableFinder' not in repr(f)]
sys.path.insert(0, %(tree)r)
from codebreaker.ui import client, terminal as T
assert T.__file__.startswith(%(tree)r) and not any(m.startswith(("codebreaker.base", "codebreaker.store")) for m in sys.modules)

async def tour():
    app = T.CodeBreaker(%(module)r)
    async with app.run_test(size=(160, 44)) as pilot:
        for _ in range(60):
            await pilot.pause(0.5)
            if isinstance(app.screen, T.Module) and app.screen.rows:
                break
        assert isinstance(app.screen, T.Module), app.screen
        mod = app.screen
        assert len(mod.rows) == 6, mod.rows
        for tab in ("tab-asm", "tab-calls", "tab-verdict", "tab-source"):
            mod.query_one(T.TabbedContent).active = tab
            await pilot.pause(1.0)
        await pilot.press("i")                                   # file info (triage)
        await pilot.pause(4)
        assert isinstance(app.screen, T.Info), app.screen
        await pilot.press("escape")
        await pilot.press("d")                                   # decompile now (the service answers, or says why not)
        await pilot.pause(3)
        await pilot.press("m")                                   # measure: a job
        await pilot.pause(2)
        await pilot.press("question_mark"); await pilot.pause(0.5)
        assert isinstance(app.screen, T.Help)
        await pilot.press("escape", "j"); await pilot.pause(2)
        assert isinstance(app.screen, T.Jobs)
        assert app.screen.query_one("#jobs").row_count >= 1, "the measure job is not listed"
        await pilot.press("escape", "escape"); await pilot.pause(0.5)
        assert isinstance(app.screen, T.Home), app.screen
        for _ in range(40):
            await pilot.pause(0.5)
            if app.screen.rows:
                break
        assert app.screen.rows, "the overview is empty"
        modules = len(app.screen.rows)
    print("TOUR-OK", len(mod.rows), modules)

asyncio.run(tour())
# the call no screen makes without a person typing: a name (the seeded function has no body to carry it: the service says so, as a ValueError)
try:
    T.STORE.name_function(%(module)r, %(addr)r, "thin_client_name")
    raise AssertionError("named a function with no body")
except ValueError as e:
    assert "no body recorded" in str(e), e
print("NAME-OK")
c = client.find()
if c:
    c.stop()
'''


def test_every_screen_works_with_only_the_ui_present():
    _need_python()
    if not importlib.util.find_spec("textual"):
        raise Skip("textual is not installed")
    sys.path.insert(0, ROOT)
    from codebreaker.ui import client
    if not client.program():
        raise Skip("the native program is not built (CB_EXE)")
    sys.path.insert(0, os.path.join(ROOT, "tests"))
    import test_ui                                                # (its own function store, CB_FUNCTIONS_DB: the service started below shares it)
    try:
        module, _sha, _digest = test_ui._fill_store()
    except test_ui.Skip as e:
        raise Skip(str(e))
    image = test_ui.pe.open_pe(module)
    addr = sorted("%08x" % (image.image_base + x["rva"]) for x in image.export_table()["symbols"][:6])[0]
    tree = tempfile.mkdtemp(prefix="cbthin")
    state = tempfile.mkdtemp(prefix="cbthinstate")
    try:
        _only_ui(tree)
        env = dict(os.environ, CB_EXE=client.program(), CB_STATE=state)
        r = subprocess.run([sys.executable, "-E", "-c", DRIVER % {"tree": tree, "module": module, "addr": addr}], cwd=tree,
                           capture_output=True, text=True, timeout=600, env=env)
        assert r.returncode == 0 and "TOUR-OK" in r.stdout and "NAME-OK" in r.stdout, r.stdout[-800:] + r.stderr[-2500:]
    finally:
        shutil.rmtree(tree, ignore_errors=True)
        shutil.rmtree(state, ignore_errors=True)
        shutil.rmtree(test_ui.TEMP, ignore_errors=True)


def main():
    for fn in (test_imports_only_the_ui, test_every_screen_works_with_only_the_ui_present):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
