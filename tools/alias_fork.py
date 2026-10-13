#!/usr/bin/env python3
"""alias_fork.py -- replace a copied module with a pointer to the one real copy.

    python tools/alias_fork.py <fork.py> <dotted module>      e.g.
    python tools/alias_fork.py src/targets/win98/le.py src.formats.lefile

A per-version tool directory imports its helpers by bare name (`import le`), so a copy cannot
simply be deleted. The file is kept, as a few lines that hand back the shared module under the
old name -- and still run it when executed as a script. Use only once the copy and the shared
module really do the same thing (tools/dup_functions.py and a diff say whether they do).
"""
import importlib.util
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

_XBT = '''from src.core import cores                                        # noqa: E402
cores.ensure_target([os.path.join(_ROOT, "src", "xbox", "xbt", d) for d in ("core", "win", "match")])
'''

_SHIM = '''"""One copy: this is CodeBreaker's {rel} -- {who} its flat import name."""
import importlib
import os
import runpy
import sys

_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), {ups}))
if _ROOT not in sys.path:
    sys.path.append(_ROOT)
{extra}if __name__ == "__main__":
    runpy.run_module("{mod}", run_name="__main__")
else:
    sys.modules[__name__] = importlib.import_module("{mod}")
'''


def tracked(prefix=""):
    """Repository files under `prefix` that git tracks (relative, forward slashes). Local
    folders git ignores -- samples, scratch material -- are not part of the package and are
    left out of every scan."""
    import subprocess
    r = subprocess.run(["git", "ls-files", "--", prefix or "."], cwd=ROOT, capture_output=True, text=True)
    return [x for x in r.stdout.split("\n") if x]


def is_alias(path):
    try:
        text = open(path, encoding="utf-8", errors="replace").read()
    except OSError:
        return False
    return len(text) < 1200 and "sys.modules[__name__] = importlib.import_module(" in text


def write(fork, mod):
    fork = os.path.abspath(fork)
    target = os.path.join(ROOT, *mod.split(".")) + ".py"
    if not os.path.isfile(target):
        raise SystemExit("%s: no such module (%s)" % (mod, target))
    if os.path.exists(fork) and os.path.samefile(fork, target):
        raise SystemExit("that is the shared module itself")
    rel = os.path.relpath(fork, ROOT).replace("\\", "/")
    depth = rel.count("/")
    xbt = "/xbox/xbt/" in "/" + rel
    who = "the Xbox tool keeps" if xbt else "the %s tools keep" % rel.split("/")[-2]
    text = _SHIM.format(rel=os.path.relpath(target, ROOT).replace("\\", "/"), who=who, mod=mod,
                        ups=", ".join(['".."'] * depth), extra=_XBT if xbt else "")
    with open(fork, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    return rel


def main(argv):
    if len(argv) != 2:
        print(__doc__)
        return 2
    print("%s -> %s" % (write(argv[0], argv[1]), argv[1]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
