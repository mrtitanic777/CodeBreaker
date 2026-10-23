"""crypto -- Xbox 360 console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots.

Most of this package is Xbox-360-Crypto by GoobyCorp (BSD 3-Clause; see LICENSE and ATTRIBUTION.md),
kept as its authors wrote it. Each tool runs as

    python -m codebreaker crypto <tool> [arguments]      (python -m codebreaker crypto: the list)

and takes its own --help. What the tools read that is not part of the package comes from settings:
`xbox360_keys` (the key store folder: 1BL_pub.bin, SB_prv.bin, ...; default Keys/ in the current
folder) and `xbox360_crypto_bin` (upstream's bin/ folder: the LZX libraries, the Free60 xenon-as and
xenon-objcopy, hypervisor images). Neither is in the repository.
"""
import importlib
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))


def tools():
    """Names of the modules here that run as tools (they define main)."""
    out = []
    for f in sorted(os.listdir(_HERE)):
        if f.endswith(".py") and not f.startswith("_"):
            with open(os.path.join(_HERE, f), encoding="utf-8", errors="replace") as fh:
                if "\ndef main(" in fh.read():
                    out.append(f[:-3])
    return out


def main(argv):
    names = tools()
    if not argv or argv[0] in ("-h", "--help", "help") or argv[0] not in names:
        print(__doc__)
        print("tools: " + " ".join(names))
        return 0 if not argv or argv[0] in ("-h", "--help", "help") else 2
    module = importlib.import_module(__name__ + "." + argv[0])
    saved = sys.argv
    sys.argv = [argv[0]] + list(argv[1:])
    try:
        result = module.main()
    except SystemExit as e:
        result = e.code
    finally:
        sys.argv = saved
    return result if isinstance(result, int) else 0
