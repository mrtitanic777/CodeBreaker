"""XboxTranspiler package.

Modules are grouped into domain subpackages (core, xbox, ppc, win, match,
assets, crypto, cli, gui) for organization. To preserve the flat sibling-import
style the tools were written in (import target, import disasm_ppc, ...), every
subpackage directory is injected onto sys.path here, giving one flat import
namespace over the whole package. This lets the algorithm code stay byte-for-byte
unchanged while the tree is cleanly organized.
"""
import os, sys

_HERE = os.path.dirname(os.path.abspath(__file__))
for _sub in ("core", "xbox", "ppc", "win", "match", "assets", "crypto", "cli", "gui"):
    _d = os.path.join(_HERE, _sub)
    if os.path.isdir(_d) and _d not in sys.path:
        sys.path.insert(0, _d)
