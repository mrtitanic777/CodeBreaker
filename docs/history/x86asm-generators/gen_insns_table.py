#!/usr/bin/env python
"""Generate native/cb/build/x86asm/insns_table.cpp from NASM's own generated tables.

Inputs (native/nasm/x86/): insnsa.c (the instruction templates), insnsb.c (the byte-code programs),
iflag.c (the instruction flag sets), insnsi.h (the opcode enum order).  Output: C++ with the same data, in
the same order, as NASM compiled it.  Usage: python gen_insns_table.py <repo-root>
"""
import re, sys, os

root = sys.argv[1]
x86 = os.path.join(root, 'native', 'nasm', 'x86')
out = os.path.join(root, 'native', 'cb', 'build', 'x86asm', 'insns_table.cpp')

def rd(n):
    with open(os.path.join(x86, n), encoding='utf-8') as f:
        return f.read()

# --- opcode enum order
enum = re.search(r'enum opcode \{(.*?)\};', rd('insnsi.h'), re.S).group(1)
opcodes = [t.strip() for t in enum.split(',') if t.strip() and '=' not in t]
index = {n: i for i, n in enumerate(opcodes)}

# --- byte codes
sb = rd('insnsb.c')
body = re.search(r'nasm_bytecodes\[(\d+)\] = \{(.*?)\n\};', sb, re.S)
nbytes = int(body.group(1))
codes = []
for line in body.group(2).split('\n'):
    line = re.sub(r'/\*.*?\*/', '', line)
    for tok in line.split(','):
        tok = tok.strip()
        if tok:
            codes.append(int(tok, 8) if tok.startswith('0') and len(tok) > 1 else int(tok))
assert len(codes) == nbytes, (len(codes), nbytes)

# --- flag sets
sf = rd('iflag.c')
m = re.search(r'insns_flags\[(\d+)\] = \{(.*?)\n\};', sf, re.S)
nflags = int(m.group(1))
flagrows = []
for line in m.group(2).split('\n'):
    mm = re.match(r'\s*\{\{(.*?)\}\},', line)
    if mm:
        words = re.findall(r'UINT32_C\((0x[0-9a-fA-F]+)\)', mm.group(1))
        assert len(words) == 7
        flagrows.append(words)
assert len(flagrows) == nflags

# --- templates, in nasm_instructions[] order
sa = rd('insnsa.c')
arrays = {}
cur = None
rowre = re.compile(r'^\s+\{(I_\w+), (\d+), \{([^}]*)\}, (NO_DECORATOR|\{[^}]*\}), nasm_bytecodes\+(\d+), (\d+), (\d+)\},\s*$')
for line in sa.split('\n'):
    h = re.match(r'static const struct itemplate (instrux_\w+)\[(\d+)\] = \{', line)
    if h:
        cur = h.group(1); arrays[cur] = []; continue
    r = rowre.match(line)
    if r and cur:
        arrays[cur].append(r.groups())
lst = re.search(r'nasm_instructions\[\d+\] = \{(.*?)\n\};', sa, re.S).group(1)
order = re.findall(r'\{\s*(\d+),\s*(instrux_\w+|NULL)\s*(?:/\*.*?\*/)?\s*\}', lst)
assert len(order) == len(opcodes), (len(order), len(opcodes))

# names <windows.h> defines as macros carry a trailing underscore in opflags.hpp
RENAMED = {'ABS': 'ABS_', 'NEAR': 'NEAR_', 'FAR': 'FAR_', 'STRICT': 'STRICT_'}

def qual(prefix, t):
    return re.sub(r'\b([A-Za-z_]\w*)\b', lambda k: prefix + RENAMED.get(k.group(1), k.group(1)), t)

def opd(s):
    toks = [t.strip() for t in s.split(',')]
    res = []
    for t in toks:
        if t == '0':
            res.append('0')
        else:
            res.append(qual('opf::', t))
    return ', '.join(res)

def deco(s):
    if s == 'NO_DECORATOR':
        return '0, 0, 0, 0, 0'
    toks = [t.strip() for t in s.strip('{}').split(',')]
    res = []
    for t in toks:
        if t == '0':
            res.append('0')
        else:
            res.append(qual('deco::', t))
    return ', '.join(res)

rows = []
firsts = []
for ntemp, name in order:
    ntemp = int(ntemp)
    arr = arrays[name] if name != 'NULL' else []
    assert len(arr) == ntemp, name
    firsts.append((len(rows), ntemp))
    pos = len(firsts) - 1
    for (op, n, o, d, code, fl, xda) in arr:
        assert index[op] == pos, (op, pos)
        rows.append('    {%s, %s, {%s}, {%s}, kBytecodes + %s, %s, %s},' % (op, n, opd(o), deco(d), code, fl, xda))

with open(out, 'w', encoding='utf-8', newline='\n') as f:
    f.write('// cb/build/x86asm/insns_table.cpp -- the instruction tables of x86asm: NASM 3.02\'s templates, byte-code programs and\n')
    f.write('// instruction flag sets, as C++ data.\n')
    f.write('//\n')
    f.write('// GENERATED -- do not edit. Made by state/work/x86asm-agents/encoder/gen_insns_table.py from NASM\'s own generated\n')
    f.write('// tables (native/nasm/x86/insnsa.c, insnsb.c, iflag.c, insnsi.h, themselves made from insns.dat by insns.pl), so the\n')
    f.write('// rows, their order and their byte-code offsets are exactly NASM\'s. %d templates, %d byte-code bytes, %d flag sets.\n' % (len(rows), nbytes, nflags))
    f.write('#include "cb/build/x86asm/insns.hpp"\n\n')
    f.write('namespace cb::build::x86asm {\n\n')
    f.write('// The byte-code programs every template points into (nasm_bytecodes).\n')
    f.write('const std::uint8_t kBytecodes[%d] = {\n' % nbytes)
    for i in range(0, nbytes, 24):
        f.write('    ' + ','.join('0x%02x' % c for c in codes[i:i+24]) + ',\n')
    f.write('};\n\n')
    f.write('// All combinations of instruction flags used by the templates (insns_flags).\n')
    f.write('const std::array<IFlags, %d> kInsnsFlags = {{\n' % nflags)
    for w in flagrows:
        f.write('    {{{%s}}},\n' % ','.join(w))
    f.write('}};\n\n')
    f.write('// Every template of every instruction, grouped by opcode in the opcode enum\'s order.\n')
    f.write('const std::array<ItemTemplate, %d> kTemplates = {{\n' % len(rows))
    f.write('\n'.join(rows) + '\n')
    f.write('}};\n\n')
    f.write('// For each opcode: where its templates start in kTemplates, and how many (nasm_instructions).\n')
    f.write('const std::array<TemplateRange, %d> kInstructions = {{\n' % len(firsts))
    for (a, n) in firsts:
        f.write('    {%d, %d},\n' % (a, n))
    f.write('}};\n\n')
    f.write('}  // namespace cb::build::x86asm\n')
print(len(rows), nbytes, nflags, len(firsts))
