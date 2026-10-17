"""How far native/decompiler is from upstream Ghidra.

Compares the vendored decompiler core (core/cpp, core/zlib) and the language files
(native/decompiler/languages) with an upstream Ghidra clone at a given ref, line endings
ignored, and prints each file as identical, patched, ours-only or upstream-only. Used to
write docs/GHIDRA_FORK.md and to check it after a rebase onto a newer upstream.

    git clone --filter=blob:none --no-checkout https://github.com/NationalSecurityAgency/ghidra.git G
    python tools/ghidra_fork.py G f0774b72f2            # the core's base
    python tools/ghidra_fork.py G Ghidra_12.1.3_build   # the language files' base
"""
import argparse
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# ours -> upstream
PAIRS = [
    ('native/decompiler/core/cpp', ['Ghidra/Features/Decompiler/src/decompile/cpp']),
    ('native/decompiler/core/zlib', ['Ghidra/Features/Decompiler/src/decompile/zlib']),
    ('native/decompiler/languages', ['Ghidra/Processors/x86/data/languages',
                                     'Ghidra/Processors/PowerPC/data/languages']),
]


def git(repo, *args):
    return subprocess.run(['git', '-C', repo, *args], capture_output=True, check=True).stdout


def tree(repo, ref, path):
    out = {}
    for line in git(repo, 'ls-tree', ref, path + '/').decode().splitlines():
        meta, name = line.split('\t')
        out[name.rsplit('/', 1)[1]] = meta.split()[2]
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('upstream', help='a clone of github.com/NationalSecurityAgency/ghidra')
    ap.add_argument('ref', help='upstream tag or commit to compare with')
    ap.add_argument('--ours', default='HEAD', help='our ref (default HEAD)')
    ap.add_argument('--all', action='store_true', help='list identical files too')
    a = ap.parse_args()
    norm = lambda b: b.replace(b'\r\n', b'\n')
    for ours_dir, up_dirs in PAIRS:
        ours = tree(ROOT, a.ours, ours_dir)
        up = {}
        for d in up_dirs:
            up.update(tree(a.upstream, a.ref, d))
        counts = {'identical': 0, 'patched': 0, 'ours-only': 0}
        lines = []
        for name, h in sorted(ours.items()):
            if name not in up:
                state = 'ours-only'
            elif norm(git(ROOT, 'cat-file', 'blob', h)) == norm(git(a.upstream, 'cat-file', 'blob', up[name])):
                state = 'identical'
            else:
                state = 'patched'
            counts[state] += 1
            if state != 'identical' or a.all:
                lines.append('  %-10s %s' % (state, name))
        print('%s vs %s: %s' % (ours_dir, a.ref, ', '.join('%d %s' % (v, k) for k, v in counts.items())))
        print('\n'.join(lines))
        if ours_dir.endswith('cpp'):
            gone = sorted(set(up) - set(ours))
            if gone:
                print('  upstream-only (not vendored): ' + ' '.join(gone))
    return 0


if __name__ == '__main__':
    sys.exit(main())
