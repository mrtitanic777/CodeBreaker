"""Compares the STFS library's view of packages with tools/scripts/stfs_extract.py's: the same entries (path, kind,
size) and the same file contents (SHA-1). Reads the packages only.

    python -I compare.py <stfs_test.exe> <package> [<package>...]
"""
import hashlib
import os
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', 'tools', 'scripts'))
import stfs_extract  # noqa: E402


def expected(path):
    stfs = stfs_extract.Stfs(open(path, 'rb').read())
    entries = stfs.entries()

    def full(e):
        parts = [e['name']]
        while e['parent'] >= 0:
            e = entries[e['parent']]
            parts.append(e['name'])
        return '/'.join(reversed(parts))

    out = []
    for e in entries:
        if e['dir']:
            out.append('%s\tdir\t0\t-' % full(e))
        else:
            out.append('%s\tfile\t%d\t%s' % (full(e), e['length'], hashlib.sha1(stfs.read(e)).hexdigest()))
    return out


def main():
    exe = sys.argv[1]
    failed = 0
    for package in sys.argv[2:]:
        want = expected(package)
        got = subprocess.run([exe, 'dump', package], capture_output=True, text=True, encoding='cp1252').stdout.splitlines()
        name = os.path.basename(package)
        if got == want:
            files = sum(1 for line in want if '\tfile\t' in line)
            print('same: %s (%d entries, %d files)' % (name, len(want), files))
        else:
            failed += 1
            print('DIFFERENT: %s (%d entries vs %d from stfs_extract.py)' % (name, len(got), len(want)))
            for a, b in zip(got, want):
                if a != b:
                    print('  ours:            %s\n  stfs_extract.py: %s' % (a, b))
                    break
    sys.exit(1 if failed else 0)


if __name__ == '__main__':
    main()
