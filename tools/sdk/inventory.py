# SDK phase 0 (docs/SDK_DESIGN.md): what each area of src/ is actually used for by the others.
# Reads every source file's #include "..." lines, maps files to areas (src/<area>/...), and writes the raw facts as JSON for report.py.
import json, os, re, sys
from collections import defaultdict

SRC = sys.argv[1] if len(sys.argv) > 1 else 'src'
OUT = sys.argv[2] if len(sys.argv) > 2 else 'work/sdk-inventory/facts.json'
EXT = ('.cpp', '.cc', '.cxx', '.c', '.hpp', '.hh', '.h', '.inc')
INC = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)
IDENT = re.compile(r'\b[A-Za-z_][A-Za-z0-9_]*\b')
# declarations in a header: type names, functions and constants at namespace level (approximate on purpose)
DECL = re.compile(r'^(?:\s*(?:struct|class|enum(?:\s+class)?|union|using|namespace)\s+([A-Za-z_]\w*)'
                  r'|[A-Za-z_][\w:<>,\s\*&]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*(?:const\s*)?(?:noexcept\s*)?;'
                  r'|\s*(?:inline\s+)?(?:constexpr|const)\s+[\w:<>]+\s+([A-Za-z_]\w*)\s*=)', re.M)
SKIP_DIRS = {'third_party', 'testdata', 'tests'}

def area_of(rel):
    parts = rel.replace('\\', '/').split('/')
    return parts[0] if len(parts) > 1 else '(root)'

files = {}
for root, dirs, names in os.walk(SRC):
    rel_root = os.path.relpath(root, SRC).replace('\\', '/')
    if rel_root.split('/')[0] in SKIP_DIRS or any(p in SKIP_DIRS for p in rel_root.split('/')):
        dirs[:] = []
        continue
    for n in names:
        if n.endswith(EXT):
            rel = (rel_root + '/' + n) if rel_root != '.' else n
            files[rel] = os.path.join(root, n)

def read(p):
    with open(p, encoding='utf-8', errors='replace') as f:
        return f.read()

text = {rel: read(p) for rel, p in files.items()}
lines = {rel: t.count('\n') + 1 for rel, t in text.items()}

by_tail = defaultdict(list)   # (area, path tail) -> files: for subtrees that find headers through their own include folders
for rel in files:
    parts = rel.split('/')
    for i in range(1, len(parts)):
        by_tail[(parts[0], '/'.join(parts[i:]))].append(rel)

def resolve(rel, inc):
    here = os.path.dirname(rel)
    for cand in (os.path.normpath(os.path.join(here, inc)).replace('\\', '/'), os.path.normpath(inc).replace('\\', '/')):
        if cand in files:
            return cand
    same = by_tail.get((area_of(rel), os.path.normpath(inc).replace('\\', '/')), [])
    if len(same) == 1:
        return same[0]
    if same:   # several: the one sharing the longest directory prefix with the includer
        return max(same, key=lambda c: len(os.path.commonprefix([c, rel])))
    return None

edges = []          # (from_file, to_header)
unresolved = defaultdict(int)
for rel, t in text.items():
    for inc in INC.findall(t):
        to = resolve(rel, inc)
        if to:
            edges.append((rel, to))
        else:
            unresolved[inc] += 1

decls = {}
for rel, t in text.items():
    if rel.endswith(('.hpp', '.hh', '.h', '.inc')):
        names = set()
        for m in DECL.finditer(t):
            n = next(g for g in m.groups() if g)
            if len(n) > 2 and not n.isupper() and n not in ('std', 'cb', 'detail', 'operator'):
                names.add(n)
        decls[rel] = sorted(names)

# which of a header's declarations does each including file mention
used = defaultdict(lambda: defaultdict(int))   # header -> decl -> count of outside files using it
for frm, to in edges:
    if area_of(frm) == area_of(to):
        continue
    toks = set(IDENT.findall(text[frm]))
    for d in decls.get(to, ()):
        if d in toks:
            used[to][d] += 1

json.dump({
    'files': {rel: {'area': area_of(rel), 'lines': lines[rel], 'test': rel.endswith(('_test.cpp', '_test.cc')) or '/tests/' in rel}
              for rel in files},
    'edges': edges,
    'decls': {h: len(d) for h, d in decls.items()},
    'used': {h: dict(sorted(v.items(), key=lambda kv: -kv[1])) for h, v in used.items()},
    'unresolved': dict(sorted(unresolved.items(), key=lambda kv: -kv[1])[:200]),
}, open(OUT, 'w', encoding='utf-8'), indent=0)
print(f'{len(files)} files, {len(edges)} includes, {sum(unresolved.values())} unresolved -> {OUT}')
