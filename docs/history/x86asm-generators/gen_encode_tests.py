#!/usr/bin/env python
"""Capture NASM's bytes for the encoder's test cases and write native/cb/build/x86asm/encode_cases.inc.

For each mode (bits 16/32/64) every case line is assembled by the real nasm.exe in ONE run (`-f bin`), each line followed
by a marker so the bytes of each line can be told apart; lines nasm rejects are found from its error messages (file:LINE:
error) and the run is repeated without them (they become "!" in the table: the encoder must refuse them too).

Usage: python gen_encode_tests.py <repo-root>
"""
import os
import subprocess
import sys
import tempfile

root = sys.argv[1]
NASM = os.path.join(root, 'native', 'nasm', 'build', 'bin', 'nasm.exe')
OUT = os.path.join(root, 'native', 'cb', 'build', 'x86asm', 'encode_cases.inc')
WORK = os.path.join(root, 'state', 'work', 'x86asm-agents', 'encoder', 'run')
os.makedirs(WORK, exist_ok=True)

MARK = bytes([0xA5, 0x5A, 0xC3, 0x3C])
MARKDB = 'db 0xa5,0x5a,0xc3,0x3c'

GPR16 = ['ax', 'cx', 'dx', 'bx', 'sp', 'bp', 'si', 'di']
GPR32 = ['eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi']
GPR64 = ['rax', 'rcx', 'rdx', 'rbx', 'rsp', 'rbp', 'rsi', 'rdi'] + ['r%d' % i for i in range(8, 16)]
GPR8 = ['al', 'cl', 'dl', 'bl', 'ah', 'ch', 'dh', 'bh']
ALU = ['add', 'or', 'adc', 'sbb', 'and', 'sub', 'xor', 'cmp']
CC = ['o', 'no', 'b', 'nb', 'e', 'ne', 'be', 'a', 's', 'ns', 'p', 'np', 'l', 'ge', 'le', 'g']
SEGS = ['es', 'cs', 'ss', 'ds', 'fs', 'gs']

cases = {16: [], 32: [], 64: []}


def add(bits, *lines):
    for b in ([16, 32, 64] if bits == 'all' else [bits] if isinstance(bits, int) else bits):
        for l in lines:
            if l not in cases[b]:
                cases[b].append(l)


# ---------------------------------------------------------------- addressing: 16-bit
EA16 = ['bx+si', 'bx+di', 'bp+si', 'bp+di', 'si', 'di', 'bp', 'bx']
DISPS = ['', '+0', '+5', '-5', '+0x7f', '+0x80', '-0x80', '-0x81', '+0x1234', '-0x1234', '+0x8000']
for e in EA16:
    for d in DISPS:
        add(16, 'mov ax, [%s%s]' % (e, d), 'mov [%s%s], cl' % (e, d))
    add(16, 'mov ax, [byte %s+0]' % e, 'mov ax, [word %s+5]' % e, 'mov ax, [byte %s+5]' % e, 'lea di, [%s+3]' % e,
        'mov dword [%s], 0x12345678' % e, 'add word [%s], 5' % e, 'add byte [%s], 5' % e, 'es mov ax, [%s]' % e,
        'mov ax, [es:%s]' % e, 'mov ax, [ss:%s]' % e, 'mov ax, [cs:%s+2]' % e, 'inc word [%s]' % e)
add(16, 'mov ax, [0x1234]', 'mov [0x1234], ax', 'mov al, [0x1234]', 'mov [0x1234], al', 'mov eax, [0x1234]', 'mov [0x1234], eax',
    'mov bx, [0x1234]', 'mov bx, [0]', 'mov ax, [ds:0x1234]', 'mov ax, [fs:0x10]', 'mov cx, [gs:bx]', 'mov [dword 0x12345678], ax',
    'mov ax, [dword 0x12345678]', 'mov ax, [word 0x1234]', 'mov ax, [0x12345678]', 'mov ax, [a32 0x100]',
    'mov ax, [a32 eax]', 'mov ax, [eax]', 'mov ax, [eax+ebx*2]', 'mov ax, [eax+0x1234]', 'mov ax, [esp]', 'mov ax, [ebp]',
    'mov ax, [a32 esp+4]', 'mov [eax*4+0x100], bx', 'mov ax, [eax*3]', 'mov ax, [bx+si+di]', 'mov ax, [bx+bp]', 'mov ax, [si+di]',
    'mov ax, [ax]', 'mov ax, [bx*2]', 'mov ax, [si*1]', 'mov ax, [bx+si*1]', 'mov ax, [si+bx]', 'mov ax, [di+bp]', 'mov ax, [bp+0]',
    'mov ax, [bp]', 'mov ax, [byte bp]', 'mov ax, [word bp]', 'mov ax, [byte 0x10]', 'mov ax, [word 0x10]', 'mov ax, [nosplit bx+si]')

# ---------------------------------------------------------------- addressing: 32-bit
for b in GPR32:
    for d in ['', '+0', '+5', '-5', '+0x7f', '+0x80', '-0x81', '+0x12345678', '-0x12345678']:
        add(32, 'mov eax, [%s%s]' % (b, d))
    add(32, 'mov [%s], ax' % b, 'mov ax, [byte %s]' % b, 'mov ax, [dword %s]' % b, 'mov ax, [byte %s+0x10]' % b, 'mov ax, [dword %s+0x10]' % b,
        'lea eax, [%s]' % b, 'mov al, [%s+%s]' % (b, 'ebx'), 'inc dword [%s]' % b, 'fs mov eax, [%s]' % b,
        'mov eax, [gs:%s+4]' % b, 'mov ax, [a16 %s]' % b)
for i in ['eax', 'ecx', 'edx', 'ebx', 'ebp', 'esi', 'edi']:
    for s in [1, 2, 4, 8]:
        for d in ['', '+0', '+0x10', '+0x1000']:
            add(32, 'mov eax, [ebx+%s*%d%s]' % (i, s, d), 'mov eax, [%s*%d%s]' % (i, s, d))
        add(32, 'mov eax, [ebp+%s*%d]' % (i, s), 'mov eax, [esp+%s*%d]' % (i, s), 'mov eax, [%s+ebx*%d]' % (i, s),
            'mov eax, [nosplit %s*%d]' % (i, s))
    add(32, 'mov eax, [%s*3]' % i, 'mov eax, [%s*5]' % i, 'mov eax, [%s*9]' % i, 'mov eax, [%s+%s]' % (i, i),
        'mov eax, [%s*2+4]' % i, 'mov eax, [%s*2]' % i, 'mov eax, [nosplit %s*2]' % i, 'mov eax, [%s*1]' % i,
        'mov eax, [nosplit %s*1]' % i, 'mov eax, [%s+esp]' % i, 'mov eax, [esp+%s]' % i, 'mov eax, [%s*1+esp]' % i)
add(32, 'mov eax, [esp*1]', 'mov eax, [esp*2]', 'mov eax, [esp+esp]', 'mov eax, [ebp+ebp]', 'mov eax, [esp+4]', 'mov eax, [esp+0x100]',
    'mov eax, [0x1000]', 'mov [0x1000], eax', 'mov al, [0x1000]', 'mov [0x1000], al', 'mov ax, [0x1000]', 'mov [0x1000], ax',
    'mov eax, [dword 0x1000]', 'mov eax, [word 0x1000]', 'mov eax, [a16 0x1000]', 'mov eax, [fs:0x1000]', 'mov eax, [fs:0]',
    'mov eax, [abs 0x1000]', 'mov eax, [rel 0x1000]', 'mov eax, [eax+bx]', 'mov eax, [ax]', 'mov eax, [bx+si]', 'mov eax, [a16 bx+si]',
    'mov eax, [a16 bp]', 'mov eax, [a16 bp+0x1000]', 'mov eax, [a32 eax]', 'mov eax, [a64 eax]', 'mov eax, [esp+esp*2]',
    'mov eax, [ebx+ecx+4]', 'mov eax, [ecx+ebx*1]', 'mov eax, [ebx*1+ecx]', 'mov eax, [ecx*1+ebx]',
    'lea eax, [eax+eax*2]', 'lea eax, [eax*2]', 'lea eax, [eax*4+8]', 'lea eax, [ebx+ecx*8-1]', 'lea esp, [esp+8]', 'lea ebp, [esp-4]')

# ---------------------------------------------------------------- addressing: 64-bit
for b in GPR64:
    for d in ['', '+0', '+5', '-5', '+0x80', '+0x12345678']:
        add(64, 'mov rax, [%s%s]' % (b, d))
    add(64, 'mov [%s], eax' % b, 'mov eax, [%s+rax*4]' % b, 'mov eax, [%s+r9*8+0x10]' % b, 'lea rax, [%s]' % b, 'mov rax, [byte %s]' % b,
        'mov al, [%s]' % b, 'fs mov rax, [%s]' % b)
b32 = ['eax', 'ecx', 'ebx', 'esp', 'ebp', 'r8d', 'r13d', 'r12d']
for b in b32:
    add(64, 'mov rax, [%s]' % b, 'mov rax, [%s+4]' % b, 'mov eax, [a32 %s]' % b)
for i in ['rax', 'rcx', 'rbx', 'rbp', 'r8', 'r12', 'r13', 'r15']:
    for s in [1, 2, 4, 8]:
        add(64, 'mov rax, [rbx+%s*%d]' % (i, s), 'mov rax, [%s*%d]' % (i, s), 'mov rax, [%s*%d+0x20]' % (i, s), 'mov rax, [r12+%s*%d]' % (i, s))
    add(64, 'mov rax, [%s*3]' % i, 'mov rax, [%s*5]' % i, 'mov rax, [%s*9]' % i, 'mov rax, [%s+%s]' % (i, i), 'mov rax, [nosplit %s*2]' % i)
add(64, 'mov rax, [rel $+5]', 'mov rax, [rel $+0x100]', 'lea rax, [rel $+5]', 'lea rax, [rel $-5]', 'mov [rel $+0x1000], eax',
    'mov eax, [rel $+5]', 'cmp dword [rel $+8], 5', 'cmp byte [rel $+8], 5', 'mov al, [rel $+3]', 'mov [rel $+3], al',
    'mov rax, [0x1000]', 'mov eax, [0x1000]', 'mov [0x1000], eax', 'mov al, [0x1000]', 'mov rax, [abs 0x1000]', 'mov rax, [rel 0x1000]',
    'mov rax, [abs 0x123456789a]', 'mov rax, [qword 0x123456789a]', 'mov rax, [dword 0x1000]', 'mov rax, [fs:0x28]', 'mov rax, [gs:0]',
    'mov rax, [fs:rax]', 'mov eax, [fs:0x1000]', 'mov rax, [a32 0x1000]', 'mov rax, [a32 rel $+5]', 'mov rax, [word 0x1000]',
    'mov rax, [rsp]', 'mov rax, [rsp+8]', 'mov rax, [r12]', 'mov rax, [r12+8]', 'mov rax, [rbp]', 'mov rax, [r13]', 'mov rax, [r13+0]',
    'mov rax, [rsp+rax]', 'mov rax, [rax+rsp]', 'mov rax, [rax*1+rsp]', 'mov rax, [rsp*1]', 'mov rax, [rsp+rsp]', 'mov rax, [eax+ebx*2]',
    'mov rax, [ax]', 'mov rax, [a16 bx]', 'mov rax, [eax+rbx]', 'mov rax, [byte rax]', 'mov rax, [dword rax]', 'mov rax, [qword rax]',
    'mov rax, [word rax]', 'mov rax, [rel rax]', 'mov rax, [rel rax+4]', 'mov rax, [rel rax*2]', 'lea rax, [rel $]',
    'mov dword [rel $+8], 0x12345678', 'mov qword [rel $+8], -1', 'mov qword [rax], -1', 'call [rel $+0x20]', 'jmp [rel $+0x20]',
    'call [rax]', 'jmp [rax+rbx*8]', 'call rax', 'jmp r10', 'push qword [rax]', 'pop qword [rax]', 'push rax', 'pop r15')

# ---------------------------------------------------------------- ALU and friends, every size and form
for bits in (16, 32, 64):
    pass
for op in ALU:
    add('all', '%s al, bl' % op, '%s bl, al' % op, '%s [bx], al' % op if True else '', '%s al, [bx]' % op if False else '%s al, 5' % op,
        '%s al, 0x7f' % op, '%s al, 0x80' % op, '%s al, -1' % op, '%s al, 0xff' % op, '%s bl, 5' % op, '%s ah, 5' % op, '%s byte [bx], 5' % op,
        '%s byte [bx], -1' % op, '%s byte [bx], bl' % op, '%s cl, [bx]' % op if False else '%s cl, byte [bx]' % op)
    add('all', '%s ax, bx' % op, '%s ax, 5' % op, '%s ax, 0x7f' % op, '%s ax, 0x80' % op, '%s ax, -128' % op, '%s ax, -129' % op,
        '%s ax, 0x1234' % op, '%s bx, 5' % op, '%s bx, 0x1234' % op, '%s word [0x100], 5' % op, '%s word [0x100], 0x1234' % op,
        '%s word [0x100], byte 5' % op, '%s ax, strict byte 5' % op, '%s ax, strict word 5' % op, '%s bx, strict word 5' % op,
        '%s cx, 0xffff' % op, '%s cx, 0xff80' % op, '%s cx, 65535' % op, '%s cx, 0x10000' % op)
    add('all', '%s eax, ebx' % op, '%s eax, 5' % op, '%s eax, 0x7f' % op, '%s eax, 0x80' % op, '%s eax, -128' % op, '%s eax, -129' % op,
        '%s eax, 0x12345678' % op, '%s ebx, 5' % op, '%s ebx, 0x12345678' % op, '%s dword [0x100], 5' % op,
        '%s dword [0x100], 0x12345678' % op, '%s dword [0x100], byte 5' % op, '%s eax, strict byte 5' % op, '%s eax, strict dword 5' % op,
        '%s ebx, strict dword 5' % op, '%s ecx, 0xffffffff' % op, '%s ecx, -1' % op, '%s ecx, 0x100000000' % op, '%s eax, dword 5' % op,
        '%s eax, byte 5' % op)
    add(64, '%s rax, rbx' % op, '%s rax, 5' % op, '%s rax, 0x7f' % op, '%s rax, 0x80' % op, '%s rax, -128' % op, '%s rax, 0x12345678' % op,
        '%s rax, -0x12345678' % op, '%s rax, 0x7fffffff' % op, '%s rax, 0x80000000' % op, '%s rax, 0xffffffff' % op, '%s rbx, 5' % op,
        '%s r8, r9' % op, '%s r8, 5' % op, '%s r8d, r9d' % op, '%s r8w, r9w' % op, '%s r8b, r9b' % op, '%s r8b, 5' % op, '%s sil, 1' % op,
        '%s dil, sil' % op, '%s spl, bpl' % op, '%s ah, 1' % op, '%s ah, sil' % op, '%s ah, r8b' % op, '%s qword [rax], 5' % op,
        '%s qword [rax], 0x12345678' % op, '%s qword [rax], rbx' % op, '%s rbx, [rax]' % op, '%s qword [r8], r9' % op,
        '%s rax, strict qword 5' % op, '%s rax, 0x123456789a' % op, '%s r15, 0x7fffffff' % op, '%s [rax], r15b' % op, '%s [rax], ah' % op,
        '%s al, sil' % op, '%s eax, r8d' % op, '%s ax, r8w' % op)
    add(32, '%s eax, [ebx]' % op, '%s [ebx], eax' % op, '%s ax, [ebx]' % op, '%s [ebx], ax' % op, '%s bl, [ebx]' % op, '%s [ebx], bl' % op)
    add(16, '%s ax, [bx]' % op, '%s [bx], ax' % op, '%s eax, [bx]' % op, '%s [bx], eax' % op, '%s bl, [bx]' % op, '%s [bx], bl' % op)
    add('all', 'lock %s [0x100], eax' % op, 'lock %s dword [0x100], 5' % op, 'xacquire lock %s dword [0x100], 5' % op,
        'xrelease %s dword [0x100], 5' % op, 'xacquire %s dword [0x100], 5' % op, 'lock %s eax, [0x100]' % op, 'lock %s eax, ebx' % op)
    add('all', 'o16 %s eax, ebx' % op, 'o32 %s ax, bx' % op, 'o16 %s ax, bx' % op, 'o32 %s eax, ebx' % op, 'a16 %s eax, [bx]' % op,
        'a32 %s eax, [ebx]' % op, 'o32 %s [bx], 5' % op, 'osp %s ax, bx' % op, 'asp %s eax, [ebx]' % op)

# ---------------------------------------------------------------- mov
for bits in (16, 32, 64):
    pass
add('all', 'mov al, bl', 'mov ax, bx', 'mov eax, ebx', 'mov al, 5', 'mov bl, 5', 'mov ah, 5', 'mov ax, 5', 'mov ax, 0x1234', 'mov ax, -1',
    'mov bx, 0xffff', 'mov eax, 5', 'mov eax, 0x12345678', 'mov eax, -1', 'mov eax, 0xffffffff', 'mov byte [0x100], 5', 'mov word [0x100], 5',
    'mov dword [0x100], 5', 'mov word [0x100], 0x1234', 'mov dword [0x100], 0x12345678', 'mov [0x100], al', 'mov [0x100], ax', 'mov [0x100], eax',
    'mov al, [0x100]', 'mov ax, [0x100]', 'mov eax, [0x100]', 'mov bl, [0x100]', 'mov bx, [0x100]', 'mov ebx, [0x100]', 'mov [0x100], bl',
    'mov [0x100], bx', 'mov [0x100], ebx', 'mov eax, strict dword 5', 'mov ax, strict word 5', 'mov ax, byte 5', 'mov eax, byte 5',
    'mov ebx, strict dword 0', 'mov ecx, 0', 'mov cx, 0', 'mov cl, 0', 'mov ebx, dword 0x1234', 'mov dword [ebx], strict dword 5',
    'mov [ebx], dword 5', 'mov [ebx], word 5', 'mov [ebx], byte 5', 'mov qword [ebx], 5', 'mov [bx], 5', 'mov [bx], eax', 'mov eax, [bx]',
    'mov dword [bx], 5', 'mov ax, dword 5', 'mov ax, qword 5')
add('all', *['mov %s, %s' % (a, b) for a in SEGS for b in ['ax', 'bx', 'eax']] + ['mov %s, [bx]' % a for a in SEGS] +
    ['mov ax, %s' % a for a in SEGS] + ['mov eax, %s' % a for a in SEGS] + ['mov [bx], %s' % a for a in SEGS] + ['mov word [bx], %s' % a for a in SEGS] +
    ['mov [0x100], %s' % a for a in SEGS] + ['mov %s, [0x100]' % a for a in SEGS] + ['mov rax, %s' % a for a in SEGS[:2]] +
    ['push %s' % a for a in SEGS] + ['pop %s' % a for a in SEGS] + ['push word %s' % a for a in SEGS] + ['push dword %s' % a for a in SEGS])
add('all', *['mov cr%d, eax' % i for i in range(8)] + ['mov eax, cr%d' % i for i in range(8)] + ['mov dr%d, eax' % i for i in range(8)] +
    ['mov eax, dr%d' % i for i in range(8)] + ['mov tr%d, eax' % i for i in (3, 6, 7)] + ['mov eax, tr%d' % i for i in (3, 6, 7)] +
    ['mov cr8, rax', 'mov rax, cr8', 'mov cr0, rax', 'mov rax, cr0', 'mov r8, cr0', 'mov cr8, r8', 'mov dr0, rax', 'mov rax, dr7'])
add(64, 'mov rax, 0x1122334455667788', 'mov rax, -1', 'mov rax, 0xffffffff', 'mov rax, 0x80000000', 'mov rax, 0x7fffffff', 'mov rax, -0x80000000',
    'mov rax, -0x80000001', 'mov r8, 0x1122334455667788', 'mov r8, 5', 'mov r8, -5', 'mov r8d, 5', 'mov rax, strict qword 5', 'mov rax, qword 5',
    'mov rax, dword 5', 'mov rax, strict dword 5', 'mov qword [rax], 5', 'mov qword [rax], -1', 'mov qword [rax], 0x80000000',
    'mov [rax], rbx', 'mov rax, [rax]', 'mov r15, [r15]', 'mov rax, rbx', 'mov r8, r9', 'mov rax, [0x1122334455667788]',
    'mov [0x1122334455667788], rax', 'mov al, [0x1122334455667788]', 'mov [0x1122334455667788], eax', 'mov rax, [qword 0x1122334455667788]',
    'mov rax, [abs 0x1122334455667788]', 'mov [abs 0x1122334455667788], al', 'mov [abs 0x11223344], al', 'mov [abs 0x11223344], rax',
    'movabs rax, 0x1122334455667788', 'movabs al, [0x1122334455667788]', 'movabs [0x1122334455667788], rax', 'movabs rax, [0x11]',
    'mov spl, 1', 'mov sil, 1', 'mov dil, 1', 'mov bpl, 1', 'mov r8b, 1', 'mov r15b, 1', 'mov ah, 1', 'mov ah, bl', 'mov ah, sil',
    'mov bl, spl', 'mov [rax], spl', 'mov [rax], ah', 'mov ah, [rax]', 'mov ah, [r8]', 'mov eax, r8d', 'mov r8d, eax', 'mov ax, r8w',
    'mov r8w, 5', 'mov r8w, [rax]', 'movsxd rax, ebx', 'movsxd rax, dword [rax]', 'movsxd r8, r9d', 'movsxd eax, ebx', 'movsxd ax, bx',
    'movzx rax, bl', 'movzx rax, bx', 'movsx rax, bl', 'movsx rax, bx', 'movsx r8, r9b', 'movzx eax, spl', 'movzx eax, ah', 'movzx r8d, ah',
    'mov rax, cs', 'mov cs, ax', 'mov ds, rax', 'mov r8, ds', 'mov es, [rax]', 'mov [rax], fs', 'mov fs, ax', 'mov gs, eax',
    'xchg rax, rax', 'xchg rax, rbx', 'xchg rbx, rax', 'xchg r8, rax', 'xchg rax, r8', 'xchg eax, eax', 'xchg eax, ebx', 'xchg ax, ax',
    'xchg r8d, eax', 'nop', 'xchg r8d, r8d', 'xchg [rax], rbx', 'xchg rbx, [rax]', 'lock xchg [rax], rbx', 'xchg al, bl', 'xchg al, [rax]',
    'xchg r8b, al', 'xchg ah, al')

# ---------------------------------------------------------------- unary and group instructions
for op in ['not', 'neg', 'mul', 'div', 'idiv', 'inc', 'dec']:
    add('all', '%s al' % op, '%s bl' % op, '%s ah' % op, '%s ax' % op, '%s bx' % op, '%s eax' % op, '%s ebx' % op, '%s byte [0x100]' % op,
        '%s word [0x100]' % op, '%s dword [0x100]' % op, '%s byte [bx]' % op, '%s word [bx]' % op, '%s dword [bx]' % op, '%s [bx]' % op,
        'lock %s dword [0x100]' % op, 'lock %s word [0x100]' % op, 'lock %s byte [0x100]' % op, 'lock %s eax' % op)
    add(64, '%s rax' % op, '%s r8' % op, '%s r8d' % op, '%s r8w' % op, '%s r8b' % op, '%s spl' % op, '%s sil' % op, '%s qword [rax]' % op,
        '%s qword [r8]' % op, '%s byte [r9+r10*2]' % op, '%s ah' % op, 'lock %s qword [rax]' % op)
add('all', 'imul bx', 'imul ebx', 'imul bl', 'imul ax, bx', 'imul eax, ebx', 'imul ax, [bx]', 'imul eax, [bx]', 'imul ax, bx, 5', 'imul ax, bx, 0x1234',
    'imul eax, ebx, 5', 'imul eax, ebx, 0x12345678', 'imul eax, [bx], 5', 'imul eax, [bx], 0x1234', 'imul ax, 5', 'imul eax, 5', 'imul eax, 0x1234',
    'imul ax, 0x1234', 'imul eax, 0x12345678', 'imul ax, bx, strict word 5', 'imul eax, ebx, strict dword 5', 'imul bx, 0x80', 'imul ebx, 0x80',
    'imul eax, ebx, -1', 'imul eax, ebx, -0x80', 'imul eax, ebx, -0x81')
add(64, 'imul rax, rbx', 'imul rax, rbx, 5', 'imul rax, [rbx], 0x12345678', 'imul r8, r9', 'imul r8, r9, 5', 'imul rax, 5', 'imul r8, 0x1234',
    'imul qword [rax]', 'imul rax, qword [r8+r9*4]')
for sh in ['rol', 'ror', 'rcl', 'rcr', 'shl', 'sal', 'shr', 'sar']:
    add('all', '%s al, 1' % sh, '%s al, cl' % sh, '%s al, 3' % sh, '%s ax, 1' % sh, '%s ax, cl' % sh, '%s ax, 3' % sh, '%s eax, 1' % sh,
        '%s eax, cl' % sh, '%s eax, 3' % sh, '%s byte [bx], 1' % sh, '%s word [bx], cl' % sh, '%s dword [bx], 3' % sh, '%s dword [bx], 1' % sh,
        '%s eax, 0' % sh, '%s eax, 255' % sh, '%s eax, 256' % sh, '%s eax, byte 1' % sh, '%s eax, strict byte 1' % sh, '%s ebx, 2' % sh)
    add(64, '%s rax, 1' % sh, '%s rax, cl' % sh, '%s rax, 3' % sh, '%s r8, 1' % sh, '%s r8d, cl' % sh, '%s qword [rax], 5' % sh,
        '%s sil, 1' % sh, '%s ah, 1' % sh, '%s r8b, 5' % sh)
add('all', 'shld eax, ebx, 5', 'shld eax, ebx, cl', 'shld ax, bx, 5', 'shld [bx], ax, cl', 'shrd eax, ebx, 5', 'shrd [bx], eax, cl', 'shrd ax, bx, cl')
add(64, 'shld rax, rbx, 5', 'shrd r8, r9, cl', 'shld [rax], r9, 3')
add('all', 'test al, bl', 'test al, 5', 'test ax, 5', 'test eax, 5', 'test eax, 0x12345678', 'test ax, 0x1234', 'test bl, 5', 'test bx, 5', 'test ebx, 5',
    'test byte [bx], 5', 'test word [bx], 5', 'test dword [bx], 5', 'test [bx], al', 'test [bx], ax', 'test [bx], eax', 'test al, [bx]', 'test eax, [bx]',
    'test eax, ebx', 'test ax, bx', 'test eax, byte 5', 'test eax, strict byte 5', 'test ebx, -1', 'test ah, 5', 'test dword [bx], byte 5')
add(64, 'test rax, rbx', 'test rax, 5', 'test rax, 0x7fffffff', 'test r8, r9', 'test qword [rax], 5', 'test qword [rax], r8', 'test sil, 1', 'test r8b, 1')
add('all', 'bt ax, bx', 'bt eax, ebx', 'bt ax, 5', 'bt eax, 5', 'bt [bx], eax', 'bt dword [bx], 5', 'bt word [bx], 5', 'bts eax, 5', 'btr eax, ebx', 'btc eax, 5',
    'bsf eax, ebx', 'bsr ax, bx', 'bsf eax, [bx]', 'bsr eax, [bx]', 'bswap eax', 'bswap ebx', 'bswap ax', 'popcnt eax, ebx', 'popcnt ax, [bx]',
    'lzcnt eax, ebx', 'tzcnt eax, ebx', 'bt eax, 256', 'bt dword [bx], 31')
add(64, 'bswap rax', 'bswap r8', 'bswap r8d', 'bt rax, 5', 'bt r8, r9', 'bts qword [rax], 5', 'bsf r8, r9', 'bsr rax, [r8]', 'popcnt rax, rbx', 'lzcnt r8, r9', 'tzcnt rax, [r8]')
add('all', 'xchg ax, bx', 'xchg bx, ax', 'xchg eax, ebx', 'xchg ebx, eax', 'xchg [bx], ax', 'xchg ax, [bx]', 'xchg [bx], eax', 'xchg al, bl', 'xchg [bx], al', 'xchg al, [bx]',
    'xchg ax, ax', 'xchg eax, eax', 'xchg ecx, edx', 'cmpxchg [bx], eax', 'cmpxchg ebx, ecx', 'lock cmpxchg [bx], eax', 'xadd [bx], eax', 'xadd eax, ebx', 'lock xadd [bx], al',
    'cmpxchg8b [bx]', 'lock cmpxchg8b [bx]', 'cmpxchg al, bl', 'cmpxchg [bx], ax')
add(64, 'cmpxchg16b [rax]', 'lock cmpxchg16b [rax]', 'cmpxchg [rax], rbx', 'xadd [rax], r8', 'lock cmpxchg [r8], r9')
add('all', 'lea ax, [bx]', 'lea eax, [bx]', 'lea ax, [ebx]', 'lea eax, [ebx]', 'lea ax, [bx+si+5]', 'lea eax, [eax*4]', 'lea bx, [0x1234]', 'lea eax, [0x1234]',
    'lea eax, bx', 'lea eax, [byte bx]')
add(64, 'lea rax, [rbx]', 'lea r8, [r9+r10*8+0x10]', 'lea eax, [rbx]', 'lea ax, [rbx]', 'lea eax, [ebx]', 'lea rax, [ebx]', 'lea rax, [0x1234]', 'lea rax, [rel 0x1234]')
add('all', 'movzx eax, bl', 'movzx eax, bx', 'movzx ax, bl', 'movzx eax, byte [bx]', 'movzx eax, word [bx]', 'movzx ax, byte [bx]', 'movzx eax, [bx]', 'movzx eax, ah',
    'movsx eax, bl', 'movsx eax, bx', 'movsx ax, bl', 'movsx eax, byte [bx]', 'movsx eax, word [bx]', 'movsx bx, byte [bx]', 'movsx ebx, ebx')
add('all', *['set%s al' % c for c in CC] + ['set%s byte [bx]' % c for c in CC[:4]] + ['cmov%s eax, ebx' % c for c in CC] +
    ['cmov%s ax, [bx]' % c for c in CC[:4]] + ['cmov%s eax, [bx]' % c for c in CC[:4]] + ['setz al', 'setnz bl', 'setc al', 'setnc al', 'setna al', 'seta al',
    'cmovz eax, ebx', 'cmovnz eax, ebx', 'cmovne eax, ebx', 'cmovnae eax, ebx', 'cmovc eax, ebx', 'cmovg eax, ebx', 'cmovnle eax, ebx', 'set al'])
add(64, 'setz sil', 'setz ah', 'setz r8b', 'cmovz r8, r9', 'cmovg rax, [r8]', 'setz byte [r8]', 'cmovz rax, rbx', 'cmovz ax, bx')

# ---------------------------------------------------------------- push / pop / stack
add('all', 'push ax', 'push bx', 'push eax', 'push ebx', 'pop ax', 'pop bx', 'pop eax', 'pop ebx', 'push word 5', 'push dword 5', 'push 5', 'push 0x80', 'push -1',
    'push 0x1234', 'push 0x12345678', 'push byte 5', 'push byte -1', 'push strict byte 5', 'push strict word 5', 'push strict dword 5', 'push word 0x1234',
    'push dword 0x12345678', 'push word [bx]', 'push dword [bx]', 'push [bx]', 'pop word [bx]', 'pop dword [bx]', 'pop [bx]', 'pusha', 'popa', 'pushad', 'popad',
    'pushf', 'popf', 'pushfd', 'popfd', 'pushfw', 'popfw', 'push word ax', 'push dword eax', 'o16 push ax', 'o32 push eax', 'enter 8, 0', 'enter 8, 1', 'enter 0x1234, 3',
    'leave', 'leavew', 'leaved', 'push byte 0x7f', 'push byte 0x80', 'push 0x7fff', 'push 0x8000', 'push 0xffff', 'push 0x10000', 'push -0x8000', 'push -0x8001', 'push cs')
add(64, 'push rax', 'push r8', 'push ax', 'push r8w', 'pop rax', 'pop r15', 'pop ax', 'push 5', 'push -1', 'push 0x12345678', 'push qword 5', 'push qword [rax]',
    'push word 5', 'push word [rax]', 'pop qword [rax]', 'pop word [rax]', 'push fs', 'push gs', 'pop fs', 'pop gs', 'pushf', 'popf', 'pushfq', 'popfq', 'leave', 'leaveq',
    'enter 8, 0', 'push eax', 'pop ebx', 'push dword 5', 'push strict dword 5', 'push 0x80000000', 'push 0xffffffff', 'push qword 0x80000000', 'push qword -0x80000000')
add(16, 'pusha', 'popa', 'pushaw', 'popaw', 'pushad', 'push es', 'pop es', 'pop cs', 'push ss', 'pop ss', 'push ds', 'pop ds')
add(32, 'pusha', 'popa', 'pushaw', 'popaw', 'pushad', 'popad', 'push es', 'pop es', 'pop cs', 'pushfw', 'push cs', 'pop ss', 'pop ds')

# ---------------------------------------------------------------- control flow
for bits in (16, 32, 64):
    pass
for c in CC + ['z', 'nz', 'c', 'nc', 'na', 'nae', 'ae', 'nbe', 'pe', 'po', 'nl', 'nge', 'ng', 'nle']:
    add('all', 'j%s $+5' % c, 'j%s $+0x7f' % c, 'j%s $+0x81' % c, 'j%s $+0x82' % c, 'j%s $-0x7e' % c, 'j%s $-0x7d' % c, 'j%s $-0x80' % c, 'j%s $-0x7c' % c, 'j%s $+0x100' % c,
        'j%s $' % c, 'j%s $+2' % c, 'j%s short $+5' % c, 'j%s near $+5' % c, 'j%s near $+0x1000' % c, 'j%s short $+0x1000' % c, 'j%s $+0x10000' % c, 'j%s $-0x1000' % c)
add('all', 'jmp $+5', 'jmp $', 'jmp $+2', 'jmp $-2', 'jmp $+0x7f', 'jmp $+0x80', 'jmp $+0x81', 'jmp $+0x82', 'jmp $-0x7e', 'jmp $-0x7d', 'jmp $-0x7c', 'jmp $-0x80', 'jmp $+0x100',
    'jmp short $+5', 'jmp near $+5', 'jmp near $+0x7f', 'jmp short $+0x100', 'jmp $+0x10000', 'jmp $-0x10000', 'jmp strict short $+5', 'jmp strict near $+5', 'jmp $+0x100000',
    'jmp ax', 'jmp bx', 'jmp eax', 'jmp ebx', 'jmp [bx]', 'jmp near [bx]', 'jmp word [bx]', 'jmp dword [bx]', 'jmp near word [bx]', 'jmp near dword [bx]', 'jmp far [bx]',
    'jmp far word [bx]', 'jmp far dword [bx]', 'jmp near ax', 'jmp near eax', 'jmp word ax', 'jmp dword eax', 'jmp 0x1234:0x5678', 'jmp 0x1234:0x56789abc', 'jmp word 0x1234:0x5678',
    'jmp dword 0x1234:0x56789abc', 'jmp 0:0', 'jmp far 0x1234:0x5678', 'jmp 0x1234:0x5678', 'jmp far [0x100]', 'jmp [0x100]', 'jmp [eax*4+0x100]', 'jmp [bx+si]',
    'call $+5', 'call $', 'call $+0x100', 'call $+0x10000', 'call near $+5', 'call short $+5', 'call ax', 'call eax', 'call bx', 'call [bx]', 'call near [bx]', 'call word [bx]', 'call dword [bx]',
    'call far [bx]', 'call far word [bx]', 'call far dword [bx]', 'call 0x1234:0x5678', 'call 0x1234:0x56789abc', 'call word 0x1234:0x5678', 'call dword 0x1234:0x56789abc',
    'call far 0x1234:0x5678', 'call near ax', 'call near eax', 'call near word [bx]', 'call near dword [bx]', 'call [0x100]', 'call [eax*4+0x100]', 'call -1', 'call $-1',
    'ret', 'ret 8', 'ret 0x1234', 'retn', 'retn 8', 'retf', 'retf 8', 'retw', 'retd', 'retw 8', 'retfw', 'retfd', 'retfd 8', 'iret', 'iretw', 'iretd', 'iretq', 'ret 0', 'retf 0',
    'int 3', 'int 0x21', 'int3', 'int1', 'icebp', 'int 0', 'int 255', 'int 256', 'into', 'int 0x80',
    'loop $+5', 'loope $+5', 'loopne $+5', 'loopz $+5', 'loopnz $+5', 'loop $', 'loop $+0x7f', 'loop $+0x81', 'loop $-0x7e', 'loop $-0x7d', 'loop $-0x80', 'loop near $+5', 'loop short $+5',
    'jcxz $+5', 'jecxz $+5', 'jcxz $', 'jecxz $', 'jcxz short $+5', 'jecxz short $+5', 'jcxz near $+5', 'jcxz $+0x100', 'jecxz $-5', 'jrcxz $+5', 'loop $+5, cx', 'loop $+5, ecx', 'loop $+5, rcx',
    'loopz $+5, cx', 'loopz $+5, ecx', 'loopne $+5, cx', 'loopne $+5, ecx', 'loope $+5, cx', 'loope $+5, ecx', 'bnd call $+5', 'bnd jmp $+5', 'bnd ret', 'bnd jmp short $+5', 'bnd je $+5', 'bnd jmp eax',
    'notrack jmp eax', 'bnd jmp near $+5', 'jmp $+5, 5')
add(64, 'jmp rax', 'jmp r8', 'jmp qword [rax]', 'jmp near qword [rax]', 'jmp far [rax]', 'jmp far qword [rax]', 'jmp far dword [rax]', 'jmp far tword [rax]', 'call rax', 'call r8', 'call qword [rax]',
    'call far [rax]', 'call far dword [rax]', 'call far tword [rax]', 'jmp ax', 'jmp eax', 'call ax', 'call eax', 'jmp word [rax]', 'jmp dword [rax]', 'call word [rax]', 'call dword [rax]',
    'jmp 0x1234:0x5678', 'call 0x1234:0x5678', 'jmp $+0x100000000', 'call $+0x100000000', 'jmp abs 0x1122334455667788', 'jmp near abs 0x1122334455667788', 'jmpabs 0x1122334455667788',
    'iretq', 'iret', 'iretd', 'iretw', 'ret', 'retq', 'retf', 'retfq', 'retf 8', 'sysret', 'sysretq', 'sysretl', 'sysenter', 'sysexit', 'sysexitq', 'syscall', 'swapgs', 'jrcxz $+5', 'jrcxz $', 'jecxz $+5', 'jcxz $+5',
    'loop $+5', 'loop $+5, ecx', 'loop $+5, rcx', 'loop $+5, cx', 'xlat', 'xlatb', 'xlat byte [rbx]', 'bnd call rax', 'bnd jmp [rax]', 'bnd ret', 'call [rel $+5]')
add(16, 'jmp 0x1234', 'jmp $+0x7fff', 'jmp $+0x8000', 'jmp $-0x8000', 'jmp short $+0x7f', 'jmp near $+0x7fff', 'jmp near $+0x8000')
add(32, 'jmp 0x1234', 'jmp $+0x7fffffff', 'jmp near $+0x7fffffff', 'jmp near $-0x7fffffff', 'call $+0x7fffffff', 'call o16 $+5', 'jmp o16 $+5', 'o16 jmp $+5', 'o16 call $+5', 'o16 jmp $+0x100',
    'o16 je $+0x100', 'o16 ret', 'o16 retf', 'o32 ret', 'o16 iret', 'o32 iret', 'o16 jmp bx', 'o32 jmp ebx', 'o16 call bx', 'o32 call ebx', 'o16 call [bx]', 'o32 call [bx]', 'o16 call far [bx]',
    'o32 call far [bx]', 'o16 jmp far [bx]', 'o32 jmp far [bx]')

# ---------------------------------------------------------------- string instructions, in/out, misc one-byte
for s in ['movsb', 'movsw', 'movsd', 'movsq', 'cmpsb', 'cmpsw', 'cmpsd', 'cmpsq', 'scasb', 'scasw', 'scasd', 'scasq', 'lodsb', 'lodsw', 'lodsd', 'lodsq', 'stosb', 'stosw', 'stosd', 'stosq',
          'insb', 'insw', 'insd', 'outsb', 'outsw', 'outsd']:
    add('all', s, 'rep %s' % s, 'repe %s' % s, 'repne %s' % s, 'repz %s' % s, 'repnz %s' % s, 'a16 %s' % s, 'a32 %s' % s, 'o16 %s' % s, 'o32 %s' % s, 'es %s' % s, 'fs %s' % s, 'rep a32 %s' % s)
    add(64, 'a64 %s' % s, 'o64 %s' % s, 'rep o64 %s' % s)
add('all', 'movsb', 'rep movsb', 'rep stosb', 'rep stosd', 'repne scasb', 'repe cmpsb', 'rep movsd', 'cs movsb', 'ds lodsb', 'ds rep lodsb', 'es rep movsb', 'xlat', 'xlatb', 'xlat byte [bx]',
    'xlat [bx]', 'xlat [ebx]', 'xlat [a32 ebx]', 'xlat byte [ebx]', 'xlat byte [es:bx]', 'wait', 'fwait', 'hlt', 'nop', 'pause', 'cli', 'sti', 'cld', 'std', 'clc', 'stc', 'cmc', 'lahf', 'sahf', 'cbw', 'cwd', 'cwde', 'cdq',
    'cwde', 'cdqe', 'cqo', 'cbw', 'daa', 'das', 'aaa', 'aas', 'aam', 'aad', 'aam 5', 'aad 5', 'aam 0x10', 'salc', 'cpuid', 'rdtsc', 'rdtscp', 'rdmsr', 'wrmsr', 'rdpmc', 'rsm', 'ud2', 'ud0 eax, ebx', 'ud1 eax, ebx',
    'emms', 'femms', 'invd', 'wbinvd', 'clts', 'lock', 'sysenter', 'sysexit', 'syscall', 'sysret', 'cpuid', 'xgetbv', 'xsetbv', 'monitor', 'mwait', 'vmcall', 'vmlaunch', 'vmresume', 'vmxoff',
    'in al, 0x10', 'in ax, 0x10', 'in eax, 0x10', 'in al, dx', 'in ax, dx', 'in eax, dx', 'out 0x10, al', 'out 0x10, ax', 'out 0x10, eax', 'out dx, al', 'out dx, ax', 'out dx, eax', 'in al, 0x100',
    'out 0x100, al', 'in al, 0', 'in eax, 255', 'out 255, eax', 'in al, byte 0x10', 'in ax, dx', 'in eax, word 5',
    'nop', 'nop eax', 'nop ax', 'nop [bx]', 'nop dword [bx]', 'nop word [bx]', 'nop dword [eax+eax+0]', 'nop word [cs:eax+eax+0]', 'nop dword [eax]', 'nop dword [eax+0]', 'nop dword [eax+0x100]', 'nop ebx', 'nop bx',
    'prefetchnta [bx]', 'prefetcht0 [bx]', 'prefetcht1 [bx]', 'prefetcht2 [bx]', 'prefetchw [bx]', 'prefetch [bx]', 'sfence', 'lfence', 'mfence', 'clflush [bx]', 'clflushopt [bx]', 'clwb [bx]',
    'fxsave [bx]', 'fxrstor [bx]', 'ldmxcsr [bx]', 'stmxcsr [bx]', 'xsave [bx]', 'xrstor [bx]', 'xsaveopt [bx]', 'xsavec [bx]', 'xsaves [bx]', 'xrstors [bx]',
    'lgdt [bx]', 'lidt [bx]', 'lldt ax', 'lldt [bx]', 'ltr ax', 'ltr [bx]', 'sgdt [bx]', 'sidt [bx]', 'sldt ax', 'sldt eax', 'sldt [bx]', 'str ax', 'str eax', 'str [bx]', 'smsw ax', 'smsw eax', 'smsw [bx]',
    'lmsw ax', 'lmsw [bx]', 'verr ax', 'verr [bx]', 'verw ax', 'lar ax, bx', 'lar eax, ebx', 'lar eax, [bx]', 'lsl ax, bx', 'lsl eax, ebx', 'arpl ax, bx', 'arpl [bx], ax', 'invlpg [bx]',
    'lds ax, [bx]', 'les ax, [bx]', 'lds eax, [bx]', 'les eax, [bx]', 'lss ax, [bx]', 'lss eax, [bx]', 'lfs ax, [bx]', 'lfs eax, [bx]', 'lgs ax, [bx]', 'lgs eax, [bx]', 'bound ax, [bx]', 'bound eax, [bx]',
    'rdrand eax', 'rdrand ax', 'rdseed eax', 'rdrand bx', 'movbe ax, [bx]', 'movbe eax, [bx]', 'movbe [bx], eax', 'crc32 eax, bl', 'crc32 eax, bx', 'crc32 eax, ebx', 'crc32 eax, byte [bx]', 'crc32 eax, dword [bx]',
    'rdfsbase eax', 'wrfsbase eax', 'rdgsbase eax', 'wrgsbase eax', 'invpcid eax, [bx]', 'xend', 'xabort 5', 'xbegin $+5', 'xtest', 'rdpid eax', 'clac', 'stac', 'tpause eax', 'umonitor eax', 'umwait eax', 'cldemote [bx]',
    'serialize', 'endbr32', 'endbr64', 'rdsspd eax', 'incsspd eax', 'wbnoinvd', 'pconfig', 'enclu', 'encls', 'enclv', 'getsec', 'vmread eax, ebx', 'vmwrite eax, ebx', 'vmclear [bx]', 'vmptrld [bx]', 'vmptrst [bx]', 'vmxon [bx]',
    'invept eax, [bx]', 'invvpid eax, [bx]', 'vmfunc', 'monitorx', 'mwaitx', 'clzero', 'rdpru', 'mcommit', 'skinit', 'vmrun', 'vmmcall', 'vmload', 'vmsave', 'stgi', 'clgi', 'invlpga', 'int 3', 'int3')
add(64, 'in al, 0x10', 'out dx, eax', 'insb', 'cwde', 'cdqe', 'cqo', 'ud2', 'cpuid', 'rdfsbase rax', 'wrfsbase rax', 'rdgsbase r8', 'wrgsbase r9', 'rdrand rax', 'rdrand r8d', 'rdseed rax', 'rdpid rax', 'movbe rax, [rbx]',
    'crc32 rax, rbx', 'crc32 rax, bl', 'crc32 r8, r9b', 'crc32 r8d, r9d', 'crc32 eax, r9b', 'invpcid rax, [rbx]', 'lgdt [rax]', 'lidt [rax]', 'sgdt [rax]', 'sidt [rax]', 'xsave64 [rax]', 'xrstor64 [rax]', 'fxsave64 [rax]', 'fxrstor64 [rax]',
    'xsaveopt64 [rax]', 'swapgs', 'syscall', 'sysret', 'sysretq', 'sysretl', 'o64 sysret', 'lar rax, bx', 'lsl rax, ebx', 'lss rax, [rbx]', 'lfs rax, [rbx]', 'lgs rax, [rbx]', 'lss eax, [rbx]', 'lds eax, [rbx]', 'les eax, [rbx]',
    'smsw rax', 'sldt rax', 'str rax', 'movnti [rax], rbx', 'movnti [rax], eax', 'rdsspq rax', 'incsspq rax', 'vmread rax, rbx', 'vmwrite rax, rbx', 'cmpxchg16b [rax]', 'prefetchw [rax]', 'popcnt rax, rbx', 'cmpxchg8b [rax]', 'lock cmpxchg8b [rax]')

# ---------------------------------------------------------------- x87
for op in ['fadd', 'fmul', 'fsub', 'fsubr', 'fdiv', 'fdivr', 'fcom', 'fcomp']:
    add('all', '%s dword [bx]' % op, '%s qword [bx]' % op, '%s st1' % op, '%s st0, st1' % op, '%s st1, st0' % op, '%s to st1' % op, '%s [bx]' % op, '%s st0' % op, '%s' % op,
        '%s dword [0x100]' % op, '%s qword [eax*4+0x100]' % op)
for op in ['faddp', 'fmulp', 'fsubp', 'fsubrp', 'fdivp', 'fdivrp']:
    add('all', op, '%s st1' % op, '%s st1, st0' % op, '%s st2, st0' % op, '%s st0, st1' % op)
for op in ['fld', 'fst', 'fstp']:
    add('all', '%s dword [bx]' % op, '%s qword [bx]' % op, '%s st1' % op, '%s tword [bx]' % op, '%s [bx]' % op, '%s st0' % op, '%s st7' % op, '%s dword [0x100]' % op, '%s tword [0x100]' % op, '%s oword [bx]' % op)
for op in ['fild', 'fist', 'fistp', 'fisttp', 'fiadd', 'fimul', 'ficom', 'ficomp', 'fisub', 'fisubr', 'fidiv', 'fidivr']:
    add('all', '%s word [bx]' % op, '%s dword [bx]' % op, '%s qword [bx]' % op, '%s [bx]' % op)
add('all', 'fbld [bx]', 'fbld tword [bx]', 'fbstp [bx]', 'fbstp tword [bx]', 'fxch', 'fxch st1', 'fxch st0, st1', 'fxch st1, st0', 'fxch st7', 'fucom', 'fucom st1', 'fucomp st1', 'fucompp', 'fcompp',
    'fcomi st1', 'fcomi st0, st1', 'fcomip st1', 'fucomi st1', 'fucomip st0, st1', 'fcmovb st0, st1', 'fcmove st0, st1', 'fcmovbe st0, st1', 'fcmovu st0, st1', 'fcmovnb st0, st1', 'fcmovne st0, st1',
    'fcmovnbe st0, st1', 'fcmovnu st0, st1', 'fchs', 'fabs', 'ftst', 'fxam', 'fld1', 'fldl2t', 'fldl2e', 'fldpi', 'fldlg2', 'fldln2', 'fldz', 'f2xm1', 'fyl2x', 'fptan', 'fpatan', 'fxtract', 'fprem1', 'fdecstp',
    'fincstp', 'fprem', 'fyl2xp1', 'fsqrt', 'fsincos', 'frndint', 'fscale', 'fsin', 'fcos', 'fnop', 'fclex', 'fnclex', 'finit', 'fninit', 'fstsw ax', 'fnstsw ax', 'fstsw [bx]', 'fnstsw [bx]', 'fstsw word [bx]', 'fstcw [bx]',
    'fnstcw [bx]', 'fldcw [bx]', 'fldcw word [bx]', 'fstenv [bx]', 'fnstenv [bx]', 'fldenv [bx]', 'fsave [bx]', 'fnsave [bx]', 'frstor [bx]', 'ffree st1', 'ffreep st1', 'fstp st1', 'fst st1', 'fld st1', 'fstsw eax',
    'fadd st0, st0', 'fadd st2, st0', 'fadd st0, st2', 'fadd to st2', 'fmul st2', 'fsub st2, st0', 'fsubr st2, st0', 'fsub to st2', 'fdiv to st2', 'fcom st2', 'fcomp st2', 'fcom st0, st2', 'fld dword es:[bx]',
    'fldenv [eax]', 'fsave [eax]', 'fld dword [eax*8+ebx+0x10]', 'o16 fldenv [bx]', 'o32 fldenv [bx]', 'o16 fnsave [bx]', 'o32 fnsave [bx]', 'o16 fnstenv [bx]', 'o32 fnstenv [bx]', 'o16 frstor [bx]', 'o32 frstor [bx]',
    'fisttp word [bx]', 'fisttp dword [bx]', 'fisttp qword [bx]', 'ffreep st0', 'fxsave [bx]', 'fadd', 'fmul', 'fucomp', 'fucomp st0, st1')

# ---------------------------------------------------------------- MMX / SSE / SSE2 / later
mmx = ['paddb', 'paddw', 'paddd', 'paddsb', 'paddsw', 'paddusb', 'paddusw', 'psubb', 'psubw', 'psubd', 'psubsb', 'psubsw', 'psubusb', 'psubusw', 'pmullw', 'pmulhw', 'pmaddwd', 'pand', 'pandn', 'por', 'pxor',
       'pcmpeqb', 'pcmpeqw', 'pcmpeqd', 'pcmpgtb', 'pcmpgtw', 'pcmpgtd', 'packsswb', 'packssdw', 'packuswb', 'punpckhbw', 'punpckhwd', 'punpckhdq', 'punpcklbw', 'punpcklwd', 'punpckldq', 'psllw', 'pslld', 'psllq',
       'psrlw', 'psrld', 'psrlq', 'psraw', 'psrad']
for op in mmx:
    add('all', '%s mm0, mm1' % op, '%s mm7, [bx]' % op, '%s mm2, qword [bx]' % op, '%s xmm0, xmm1' % op, '%s xmm1, [bx]' % op, '%s xmm1, oword [bx]' % op)
    add(64, '%s xmm8, xmm9' % op, '%s xmm8, [r8]' % op)
add('all', 'psllw mm0, 5', 'pslld mm0, 5', 'psllq mm0, 5', 'psrlw mm0, 5', 'psraw mm0, 5', 'psllw xmm0, 5', 'pslld xmm0, 5', 'psllq xmm0, 5', 'pslldq xmm0, 5', 'psrldq xmm0, 5', 'psrlw xmm0, 5', 'psrad xmm0, 5',
    'movq mm0, mm1', 'movq mm0, [bx]', 'movq [bx], mm0', 'movq mm0, rax', 'movq rax, mm0', 'movd mm0, eax', 'movd eax, mm0', 'movd mm0, [bx]', 'movd [bx], mm0', 'movd xmm0, eax', 'movd eax, xmm0', 'movd xmm0, [bx]',
    'movd [bx], xmm0', 'movq xmm0, xmm1', 'movq xmm0, [bx]', 'movq [bx], xmm0', 'movq xmm0, rax', 'movq rax, xmm0', 'movq mm0, qword [bx]', 'movq xmm0, qword [bx]', 'movdqa xmm0, xmm1', 'movdqa xmm0, [bx]',
    'movdqa [bx], xmm0', 'movdqu xmm0, xmm1', 'movdqu xmm0, [bx]', 'movdqu [bx], xmm0', 'movdq2q mm0, xmm0', 'movq2dq xmm0, mm0', 'movaps xmm0, xmm1', 'movaps xmm0, [bx]', 'movaps [bx], xmm0', 'movups xmm0, [bx]',
    'movups [bx], xmm1', 'movapd xmm0, [bx]', 'movupd [bx], xmm0', 'movss xmm0, xmm1', 'movss xmm0, [bx]', 'movss [bx], xmm0', 'movsd xmm0, xmm1', 'movsd xmm0, [bx]', 'movsd [bx], xmm0', 'movhps xmm0, [bx]',
    'movhps [bx], xmm0', 'movlps xmm0, [bx]', 'movlps [bx], xmm0', 'movhpd xmm0, [bx]', 'movlpd [bx], xmm0', 'movlhps xmm0, xmm1', 'movhlps xmm0, xmm1', 'movmskps eax, xmm0', 'movmskpd eax, xmm0', 'pmovmskb eax, mm0',
    'pmovmskb eax, xmm0', 'movntps [bx], xmm0', 'movntpd [bx], xmm0', 'movntdq [bx], xmm0', 'movntq [bx], mm0', 'movnti [bx], eax', 'movntdqa xmm0, [bx]', 'maskmovq mm0, mm1', 'maskmovdqu xmm0, xmm1',
    'lddqu xmm0, [bx]', 'movddup xmm0, xmm1', 'movddup xmm0, [bx]', 'movshdup xmm0, xmm1', 'movsldup xmm0, [bx]', 'addps xmm0, xmm1', 'addps xmm0, [bx]', 'addss xmm0, xmm1', 'addss xmm0, [bx]', 'addpd xmm0, xmm1', 'addsd xmm0, xmm1',
    'subps xmm0, xmm1', 'mulps xmm0, xmm1', 'divps xmm0, xmm1', 'sqrtps xmm0, xmm1', 'rsqrtps xmm0, xmm1', 'rcpps xmm0, xmm1', 'minps xmm0, xmm1', 'maxps xmm0, xmm1', 'andps xmm0, xmm1', 'andnps xmm0, xmm1',
    'orps xmm0, xmm1', 'xorps xmm0, xmm1', 'xorps xmm0, [bx]', 'andpd xmm0, xmm1', 'xorpd xmm0, xmm1', 'cmpps xmm0, xmm1, 5', 'cmpss xmm0, xmm1, 5', 'cmppd xmm0, xmm1, 5', 'cmpsd xmm0, xmm1, 5', 'cmpeqps xmm0, xmm1',
    'cmpltps xmm0, xmm1', 'cmpleps xmm0, xmm1', 'cmpunordps xmm0, xmm1', 'cmpneqps xmm0, xmm1', 'cmpnltps xmm0, xmm1', 'cmpnleps xmm0, xmm1', 'cmpordps xmm0, xmm1', 'cmpeqss xmm0, xmm1', 'cmpeqsd xmm0, xmm1',
    'comiss xmm0, xmm1', 'ucomiss xmm0, [bx]', 'comisd xmm0, xmm1', 'ucomisd xmm0, xmm1', 'cvtsi2ss xmm0, eax', 'cvtsi2ss xmm0, [bx]', 'cvtsi2ss xmm0, dword [bx]', 'cvtsi2sd xmm0, eax', 'cvtsi2sd xmm0, dword [bx]',
    'cvtss2si eax, xmm0', 'cvtsd2si eax, xmm0', 'cvttss2si eax, xmm0', 'cvttsd2si eax, xmm0', 'cvtss2sd xmm0, xmm1', 'cvtsd2ss xmm0, xmm1', 'cvtps2pd xmm0, xmm1', 'cvtpd2ps xmm0, xmm1', 'cvtdq2ps xmm0, xmm1', 'cvtps2dq xmm0, xmm1',
    'cvttps2dq xmm0, xmm1', 'cvtdq2pd xmm0, xmm1', 'cvtpd2dq xmm0, xmm1', 'cvttpd2dq xmm0, xmm1', 'cvtpi2ps xmm0, mm0', 'cvtps2pi mm0, xmm0', 'cvttps2pi mm0, xmm0', 'cvtpi2pd xmm0, mm0', 'cvtpd2pi mm0, xmm0', 'cvttpd2pi mm0, xmm0',
    'shufps xmm0, xmm1, 5', 'shufpd xmm0, xmm1, 1', 'unpcklps xmm0, xmm1', 'unpckhps xmm0, xmm1', 'unpcklpd xmm0, xmm1', 'unpckhpd xmm0, xmm1', 'pshufd xmm0, xmm1, 5', 'pshufhw xmm0, xmm1, 5', 'pshuflw xmm0, xmm1, 5',
    'pshufw mm0, mm1, 5', 'pextrw eax, mm0, 1', 'pextrw eax, xmm0, 1', 'pinsrw mm0, eax, 1', 'pinsrw xmm0, eax, 1', 'pinsrw xmm0, [bx], 1', 'pinsrw xmm0, word [bx], 1', 'pextrw [bx], xmm0, 1', 'pextrb eax, xmm0, 1', 'pextrd eax, xmm0, 1',
    'pinsrb xmm0, eax, 1', 'pinsrd xmm0, eax, 1', 'pinsrb xmm0, [bx], 1', 'pinsrd xmm0, [bx], 1', 'pextrb [bx], xmm0, 1', 'pextrd [bx], xmm0, 1', 'extractps eax, xmm0, 1', 'insertps xmm0, xmm1, 5', 'insertps xmm0, [bx], 5',
    'pmulld xmm0, xmm1', 'pmuldq xmm0, xmm1', 'pminsb xmm0, xmm1', 'pmaxsd xmm0, xmm1', 'pminud xmm0, xmm1', 'pmaxuw xmm0, xmm1', 'pblendw xmm0, xmm1, 5', 'blendps xmm0, xmm1, 5', 'blendvps xmm0, xmm1, xmm0', 'blendvps xmm0, xmm1',
    'pblendvb xmm0, xmm1, xmm0', 'pblendvb xmm0, xmm1', 'roundps xmm0, xmm1, 5', 'roundss xmm0, xmm1, 5', 'roundpd xmm0, xmm1, 5', 'roundsd xmm0, xmm1, 5', 'dpps xmm0, xmm1, 5', 'dppd xmm0, xmm1, 5', 'mpsadbw xmm0, xmm1, 5',
    'ptest xmm0, xmm1', 'pcmpeqq xmm0, xmm1', 'pcmpgtq xmm0, xmm1', 'pminsw xmm0, xmm1', 'pmaxsw xmm0, xmm1', 'pavgb xmm0, xmm1', 'pavgw xmm0, xmm1', 'psadbw xmm0, xmm1', 'pmulhuw xmm0, xmm1', 'pmuludq xmm0, xmm1',
    'psubq xmm0, xmm1', 'paddq xmm0, xmm1', 'phaddw xmm0, xmm1', 'phaddd xmm0, xmm1', 'phsubw mm0, mm1', 'pmaddubsw xmm0, xmm1', 'pshufb xmm0, xmm1', 'pshufb mm0, mm1', 'psignb xmm0, xmm1', 'pabsb xmm0, xmm1', 'pabsw mm0, mm1',
    'pabsd xmm0, [bx]', 'palignr xmm0, xmm1, 5', 'palignr mm0, mm1, 5', 'pmulhrsw xmm0, xmm1', 'pmovsxbw xmm0, xmm1', 'pmovsxbw xmm0, [bx]', 'pmovzxbd xmm0, xmm1', 'pmovzxwd xmm0, [bx]', 'pmovzxdq xmm0, xmm1', 'pmovsxbq xmm0, [bx]',
    'packusdw xmm0, xmm1', 'phminposuw xmm0, xmm1', 'pcmpestri xmm0, xmm1, 5', 'pcmpestrm xmm0, xmm1, 5', 'pcmpistri xmm0, xmm1, 5', 'pcmpistrm xmm0, [bx], 5', 'aesenc xmm0, xmm1', 'aesenclast xmm0, [bx]', 'aesdec xmm0, xmm1',
    'aesdeclast xmm0, xmm1', 'aesimc xmm0, xmm1', 'aeskeygenassist xmm0, xmm1, 5', 'pclmulqdq xmm0, xmm1, 5', 'pclmullqlqdq xmm0, xmm1', 'sha1rnds4 xmm0, xmm1, 1', 'sha1nexte xmm0, xmm1', 'sha1msg1 xmm0, xmm1', 'sha256rnds2 xmm0, xmm1, xmm0',
    'sha256rnds2 xmm0, xmm1', 'sha256msg1 xmm0, xmm1', 'haddps xmm0, xmm1', 'haddpd xmm0, xmm1', 'hsubps xmm0, xmm1', 'addsubps xmm0, xmm1', 'addsubpd xmm0, xmm1', 'movbe eax, [bx]', 'gf2p8mulb xmm0, xmm1', 'gf2p8affineqb xmm0, xmm1, 5',
    'gf2p8affineinvqb xmm0, xmm1, 5', 'pclmulhqhqdq xmm0, xmm1', 'blendvpd xmm0, xmm1', 'pslld xmm0, xmm1', 'pslld xmm0, [bx]', 'psrlq xmm0, xmm1', 'psraw mm0, mm1', 'psllq mm0, [bx]', 'psllw xmm0, xmm1', 'pextrq rax, xmm0, 1',
    'pinsrq xmm0, rax, 1', 'movq xmm0, rax', 'movd xmm0, rax', 'movq rax, xmm0', 'movd rax, xmm0', 'cvtsi2ss xmm0, rax', 'cvtsi2sd xmm0, qword [bx]', 'cvtsi2sd xmm0, rax', 'cvtss2si rax, xmm0', 'cvttsd2si rax, xmm0')
add(64, 'movaps xmm8, xmm9', 'movaps xmm0, [r8]', 'movaps [r8+r9*2], xmm15', 'movq xmm8, rax', 'movq r8, xmm9', 'movd xmm8, r9d', 'movd r8d, xmm9', 'pshufd xmm8, xmm9, 5', 'pextrq r8, xmm9, 1', 'pinsrq xmm8, r9, 1', 'cvtsi2sd xmm8, r9',
    'cvtsi2sd xmm0, r9', 'cvtsi2ss xmm8, qword [r9]', 'cvttsd2si r8, xmm9', 'movdqa xmm8, [r8]', 'pxor xmm8, xmm8', 'pxor mm0, mm1', 'psllq xmm8, 5', 'movmskpd r8d, xmm9', 'pmovmskb r8d, xmm9', 'blendvps xmm8, xmm9', 'pcmpestri xmm8, xmm9, 5',
    'pclmulqdq xmm8, [r8], 5', 'movntdqa xmm8, [r9]', 'maskmovdqu xmm8, xmm9', 'cvtps2pd xmm8, xmm9', 'movq xmm8, xmm9', 'movq xmm8, [r9]', 'movq [r9], xmm8', 'movss xmm8, [r9+4]', 'cvtsi2ss xmm8, r9d', 'cvtss2si r8d, xmm9', 'movd mm0, r8d', 'movq mm0, r8',
    'movq r8, mm0', 'movd r8d, mm0', 'pextrw r8d, xmm9, 1', 'pextrw rax, xmm0, 1', 'pextrw r8, mm0, 1', 'pextrw eax, xmm8, 1', 'pextrb r8d, xmm9, 1', 'pextrb rax, xmm0, 1', 'pextrd r8d, xmm9, 1', 'pinsrb xmm8, r9d, 1', 'pinsrb xmm8, [r9], 1',
    'extractps r8d, xmm9, 1', 'extractps rax, xmm0, 1')

# ---------------------------------------------------------------- AVX / AVX2 / FMA / BMI
vx = ['vaddps', 'vaddpd', 'vsubps', 'vmulps', 'vdivps', 'vandps', 'vxorps', 'vorps', 'vmaxps', 'vminps', 'vpaddb', 'vpaddd', 'vpxor', 'vpand', 'vpor', 'vpcmpeqb', 'vpsubd', 'vpunpcklbw', 'vpshufb', 'vpmulld', 'vunpcklps',
      'vhaddps', 'vaddsubps', 'vpacksswb']
for op in vx:
    add('all', '%s xmm0, xmm1, xmm2' % op, '%s xmm0, xmm1, [bx]' % op, '%s ymm0, ymm1, ymm2' % op, '%s ymm0, ymm1, [bx]' % op, '%s xmm0, xmm1, oword [bx]' % op, '%s ymm0, ymm1, yword [bx]' % op)
    add(64, '%s xmm8, xmm9, xmm10' % op, '%s ymm8, ymm9, ymm10' % op, '%s xmm0, xmm1, xmm12' % op, '%s xmm0, xmm9, xmm2' % op, '%s xmm8, xmm1, xmm2' % op, '%s xmm0, xmm1, [r8]' % op, '%s xmm0, xmm1, [rax+r9*2]' % op,
        '%s ymm15, ymm15, [r15]' % op)
add('all', 'vmovaps xmm0, xmm1', 'vmovaps ymm0, ymm1', 'vmovaps xmm0, [bx]', 'vmovaps [bx], ymm0', 'vmovups ymm0, [bx]', 'vmovups [bx], xmm0', 'vmovdqa xmm0, [bx]', 'vmovdqa ymm0, ymm1', 'vmovdqu ymm0, [bx]', 'vmovdqu [bx], ymm0',
    'vmovss xmm0, xmm1, xmm2', 'vmovss xmm0, [bx]', 'vmovss [bx], xmm0', 'vmovsd xmm0, xmm1, xmm2', 'vmovsd xmm0, [bx]', 'vmovd xmm0, eax', 'vmovd eax, xmm0', 'vmovq xmm0, xmm1', 'vmovq xmm0, [bx]', 'vmovq [bx], xmm0', 'vmovq xmm0, rax',
    'vmovq rax, xmm0', 'vmovmskps eax, ymm0', 'vmovhps xmm0, xmm1, [bx]', 'vmovlps xmm0, xmm1, [bx]', 'vmovhps [bx], xmm0', 'vmovlhps xmm0, xmm1, xmm2', 'vmovhlps xmm0, xmm1, xmm2', 'vmovntdq [bx], ymm0', 'vmovntps [bx], xmm0',
    'vmovddup ymm0, ymm1', 'vmovshdup ymm0, [bx]', 'vlddqu ymm0, [bx]', 'vbroadcastss xmm0, [bx]', 'vbroadcastss ymm0, [bx]', 'vbroadcastss ymm0, xmm1', 'vbroadcastsd ymm0, [bx]', 'vbroadcastsd ymm0, xmm1', 'vbroadcastf128 ymm0, [bx]',
    'vbroadcasti128 ymm0, [bx]', 'vpbroadcastb xmm0, xmm1', 'vpbroadcastb ymm0, [bx]', 'vpbroadcastw ymm0, xmm1', 'vpbroadcastd ymm0, xmm1', 'vpbroadcastq xmm0, [bx]', 'vextractf128 xmm0, ymm1, 1', 'vextractf128 [bx], ymm1, 1',
    'vinsertf128 ymm0, ymm1, xmm2, 1', 'vinsertf128 ymm0, ymm1, [bx], 1', 'vextracti128 xmm0, ymm1, 1', 'vinserti128 ymm0, ymm1, xmm2, 1', 'vperm2f128 ymm0, ymm1, ymm2, 5', 'vperm2i128 ymm0, ymm1, [bx], 5', 'vpermq ymm0, ymm1, 5',
    'vpermpd ymm0, [bx], 5', 'vpermd ymm0, ymm1, ymm2', 'vpermps ymm0, ymm1, [bx]', 'vpermilps xmm0, xmm1, 5', 'vpermilps xmm0, xmm1, xmm2', 'vpermilpd ymm0, ymm1, ymm2', 'vpermilps ymm0, [bx], 5', 'vshufps ymm0, ymm1, ymm2, 5', 'vshufpd xmm0, xmm1, xmm2, 1',
    'vpshufd ymm0, ymm1, 5', 'vpalignr ymm0, ymm1, ymm2, 5', 'vpblendw ymm0, ymm1, ymm2, 5', 'vblendps ymm0, ymm1, ymm2, 5', 'vblendvps ymm0, ymm1, ymm2, ymm3', 'vblendvps xmm0, xmm1, [bx], xmm3', 'vpblendvb xmm0, xmm1, xmm2, xmm3',
    'vpblendd xmm0, xmm1, xmm2, 5', 'vpblendd ymm0, ymm1, ymm2, 5', 'vcmpps xmm0, xmm1, xmm2, 5', 'vcmpps ymm0, ymm1, ymm2, 5', 'vcmpeqps xmm0, xmm1, xmm2', 'vcmpltps ymm0, ymm1, [bx]', 'vcmpss xmm0, xmm1, xmm2, 5', 'vcmpeq_uqps xmm0, xmm1, xmm2',
    'vcvtps2pd ymm0, xmm1', 'vcvtpd2ps xmm0, ymm1', 'vcvtpd2ps xmm0, yword [bx]', 'vcvtpd2ps xmm0, oword [bx]', 'vcvtdq2ps ymm0, ymm1', 'vcvtps2dq ymm0, [bx]', 'vcvtsi2ss xmm0, xmm1, eax', 'vcvtsi2sd xmm0, xmm1, eax', 'vcvtsi2sd xmm0, xmm1, dword [bx]',
    'vcvtss2sd xmm0, xmm1, xmm2', 'vcvtsd2ss xmm0, xmm1, [bx]', 'vcvttss2si eax, xmm0', 'vcvtsd2si eax, xmm0', 'vcvtps2ph xmm0, ymm1, 5', 'vcvtps2ph [bx], ymm1, 5', 'vcvtph2ps ymm0, xmm1', 'vcvtph2ps xmm0, [bx]', 'vsqrtps ymm0, ymm1', 'vsqrtss xmm0, xmm1, xmm2',
    'vrcpps ymm0, ymm1', 'vroundps ymm0, ymm1, 5', 'vroundsd xmm0, xmm1, xmm2, 5', 'vdpps ymm0, ymm1, ymm2, 5', 'vtestps ymm0, ymm1', 'vptest ymm0, ymm1', 'vzeroupper', 'vzeroall', 'vldmxcsr [bx]', 'vstmxcsr [bx]',
    'vpsllw xmm0, xmm1, 5', 'vpsllw ymm0, ymm1, 5', 'vpslld xmm0, xmm1, xmm2', 'vpslld ymm0, ymm1, xmm2', 'vpsllq ymm0, ymm1, [bx]', 'vpsrad xmm0, xmm1, 5', 'vpslldq xmm0, xmm1, 5', 'vpsrldq ymm0, ymm1, 5', 'vpsllvd xmm0, xmm1, xmm2', 'vpsrlvq ymm0, ymm1, ymm2',
    'vpmovmskb eax, xmm0', 'vpmovmskb eax, ymm0', 'vpextrw eax, xmm0, 1', 'vpextrb eax, xmm0, 1', 'vpextrd eax, xmm0, 1', 'vpextrq rax, xmm0, 1', 'vpinsrw xmm0, xmm1, eax, 1', 'vpinsrb xmm0, xmm1, eax, 1', 'vpinsrd xmm0, xmm1, eax, 1', 'vpinsrq xmm0, xmm1, rax, 1',
    'vpinsrw xmm0, xmm1, [bx], 1', 'vextractps eax, xmm0, 1', 'vinsertps xmm0, xmm1, xmm2, 5', 'vmaskmovps ymm0, ymm1, [bx]', 'vmaskmovps [bx], ymm1, ymm2', 'vpmaskmovd xmm0, xmm1, [bx]', 'vpmaskmovq [bx], ymm1, ymm2', 'vgatherdps xmm0, [eax+xmm1*4], xmm2',
    'vgatherdps ymm0, [eax+ymm1*4], ymm2', 'vgatherqps xmm0, [eax+xmm1*4], xmm2', 'vgatherqpd ymm0, [eax+ymm1*8], ymm2', 'vpgatherdd xmm0, [eax+xmm1*4], xmm2', 'vpgatherdq xmm0, [eax+xmm1*8], xmm2', 'vpgatherqd xmm0, [eax+ymm1*4], xmm2',
    'vfmadd132ps xmm0, xmm1, xmm2', 'vfmadd213ps ymm0, ymm1, [bx]', 'vfmadd231pd ymm0, ymm1, ymm2', 'vfmadd132ss xmm0, xmm1, xmm2', 'vfmadd231sd xmm0, xmm1, [bx]', 'vfmsub213ps xmm0, xmm1, xmm2', 'vfnmadd132ps xmm0, xmm1, xmm2', 'vfnmsub231pd ymm0, ymm1, ymm2',
    'vfmaddsub132ps xmm0, xmm1, xmm2', 'vfmsubadd231ps ymm0, ymm1, ymm2', 'vaesenc xmm0, xmm1, xmm2', 'vaesenc ymm0, ymm1, ymm2', 'vaesimc xmm0, xmm1', 'vpclmulqdq xmm0, xmm1, xmm2, 5', 'vpclmulqdq ymm0, ymm1, ymm2, 5',
    'andn eax, ebx, ecx', 'andn eax, ebx, [bx]', 'bextr eax, ebx, ecx', 'bextr eax, [bx], ecx', 'blsi eax, ebx', 'blsmsk eax, [bx]', 'blsr eax, ebx', 'bzhi eax, ebx, ecx', 'mulx eax, ebx, ecx', 'mulx eax, ebx, [bx]', 'pdep eax, ebx, ecx', 'pext eax, ebx, [bx]',
    'rorx eax, ebx, 5', 'rorx eax, [bx], 5', 'sarx eax, ebx, ecx', 'shlx eax, [bx], ecx', 'shrx eax, ebx, ecx', 'adcx eax, ebx', 'adox eax, ebx', 'adcx eax, [bx]', 'vpermq ymm0, ymm1, 0x1b', 'vpsadbw ymm0, ymm1, ymm2', 'vmpsadbw ymm0, ymm1, ymm2, 5',
    'vphaddd ymm0, ymm1, ymm2', 'vpabsd ymm0, ymm1', 'vpsignb ymm0, ymm1, ymm2', 'vpmovsxbw ymm0, xmm1', 'vpmovzxbw ymm0, [bx]', 'vpmovsxbd ymm0, xmm1', 'vpmovzxdq ymm0, xmm1', 'vpackusdw ymm0, ymm1, ymm2', 'vpminud ymm0, ymm1, ymm2', 'vpmaxsd ymm0, ymm1, ymm2',
    'vcomiss xmm0, xmm1', 'vucomisd xmm0, [bx]', 'vandnps ymm0, ymm1, ymm2', 'vunpckhpd ymm0, ymm1, ymm2', 'vpcmpestri xmm0, xmm1, 5', 'vpcmpistri xmm0, [bx], 5', 'vmovmskpd eax, xmm0', 'vmovapd ymm0, ymm1', 'vpsrlw ymm0, ymm1, xmm2', 'vpavgb ymm0, ymm1, ymm2',
    'vcvttpd2dq xmm0, ymm1', 'vcvttps2dq ymm0, ymm1', 'vpmaddwd ymm0, ymm1, ymm2', 'vpmuludq ymm0, ymm1, ymm2', 'vpmuldq ymm0, ymm1, ymm2', 'vpmulhuw ymm0, ymm1, ymm2', 'vpmaddubsw ymm0, ymm1, ymm2', 'vphminposuw xmm0, xmm1', 'vmaskmovdqu xmm0, xmm1')
add(64, 'vmovaps xmm8, xmm9', 'vmovaps ymm8, [r8]', 'vmovaps [r9], ymm10', 'vmovq xmm8, rax', 'vmovq r8, xmm9', 'vmovd xmm8, r9d', 'vpextrq r8, xmm9, 1', 'vpinsrq xmm8, xmm9, r10, 1', 'vcvtsi2sd xmm8, xmm9, r10', 'vcvtsi2sd xmm8, xmm9, qword [r10]',
    'vcvttsd2si r8, xmm9', 'vcvtsd2si r8, xmm9', 'vgatherdps ymm8, [r8+ymm9*4], ymm10', 'vgatherqpd xmm8, [rax+xmm9*8], xmm10', 'vpgatherqq ymm0, [rax+ymm9*8], ymm1', 'andn r8, r9, r10', 'andn rax, rbx, [r8]', 'bextr r8, r9, r10', 'blsi r8, r9', 'mulx r8, r9, r10',
    'pdep rax, rbx, rcx', 'pext r8, r9, [r10]', 'rorx r8, r9, 5', 'rorx rax, [r8], 5', 'sarx r8, r9, r10', 'shlx rax, [rbx], rcx', 'adcx r8, r9', 'adox rax, rbx', 'vbroadcastss ymm8, xmm9', 'vzeroupper', 'vextractf128 xmm8, ymm9, 1', 'vinserti128 ymm8, ymm9, xmm10, 1',
    'vperm2f128 ymm8, ymm9, ymm10, 5', 'vpermq ymm8, ymm9, 5', 'vblendvps ymm8, ymm9, ymm10, ymm11', 'vblendvps xmm0, xmm1, xmm2, xmm15', 'vfmadd231ps ymm8, ymm9, ymm10', 'vfmadd231ps ymm0, ymm1, [r8]', 'vfmadd132pd xmm8, xmm9, [r10+r11*8]',
    'vpshufb ymm8, ymm9, [r10]', 'vpaddd xmm15, xmm14, [r13]', 'vpsllq ymm8, ymm9, 5', 'vpslld xmm8, xmm9, xmm10', 'vpmovmskb r8d, ymm9', 'vmovmskps r8d, ymm9', 'vextractps r8d, xmm9, 1', 'vpextrw r8d, xmm9, 1', 'vcmpps ymm8, ymm9, ymm10, 5')
add(['all'][0], '{vex} vaddps xmm0, xmm1, xmm2', '{vex3} vaddps xmm0, xmm1, xmm2', '{vex2} vaddps xmm0, xmm1, xmm2', '{vex3} vaddps xmm0, xmm1, [bx]', '{vex2} vaddps xmm8, xmm1, xmm2', '{evex} vaddps xmm0, xmm1, xmm2',
    '{vex} vmovaps xmm0, xmm1', '{vex3} vmovaps xmm0, xmm1', '{evex} vmovaps xmm0, xmm1', '{vex2} vpermq ymm0, ymm1, 5', '{vex} andn eax, ebx, ecx')
add(64, '{rex} mov eax, ebx', '{rex} mov al, bl', '{rex} nop', '{rex} add eax, 5', '{rex} mov rax, rbx', '{rex2} mov eax, ebx', '{rex} movaps xmm0, xmm1', '{rex} vaddps xmm0, xmm1, xmm2', '{rex} push rax', '{rex} ret', '{rex} lock inc dword [rax]')

# ---------------------------------------------------------------- AVX-512
add('all', 'vaddps zmm0, zmm1, zmm2', 'vaddps zmm0, zmm1, [bx]', 'vaddps zmm0 {k1}, zmm1, zmm2', 'vaddps zmm0 {k1}{z}, zmm1, zmm2', 'vaddps zmm0 {k7}{z}, zmm1, [bx]', 'vaddps zmm0, zmm1, [bx]{1to16}', 'vaddps zmm0, zmm1, [bx+4]{1to16}',
    'vaddps zmm0, zmm1, zmm2, {rn-sae}', 'vaddps zmm0, zmm1, zmm2, {rz-sae}', 'vaddps zmm0, zmm1, zmm2, {rd-sae}', 'vaddps zmm0, zmm1, zmm2, {ru-sae}', 'vaddpd zmm0, zmm1, zmm2', 'vaddpd zmm0, zmm1, [bx]{1to8}', 'vaddpd zmm0, zmm1, zmm2, {rn-sae}',
    'vaddps ymm0, ymm1, [bx]{1to8}', 'vaddps xmm0, xmm1, [bx]{1to4}', 'vaddps xmm0 {k1}, xmm1, xmm2', 'vaddps ymm0 {k2}{z}, ymm1, [bx]', 'vaddss xmm0, xmm1, xmm2', 'vaddss xmm0, xmm1, xmm2, {rn-sae}', 'vaddsd xmm0 {k1}, xmm1, [bx]',
    'vmovaps zmm0, zmm1', 'vmovaps zmm0, [bx]', 'vmovaps [bx], zmm0', 'vmovaps zmm0 {k1}{z}, [bx]', 'vmovaps [bx] {k1}, zmm0', 'vmovups zmm0, [bx+0x40]', 'vmovups zmm0, [bx+0x41]', 'vmovups zmm0, [bx+0x1000]', 'vmovups zmm0, [bx-0x40]',
    'vmovdqa32 zmm0, zmm1', 'vmovdqa64 zmm0, [bx]', 'vmovdqu8 zmm0, [bx]', 'vmovdqu16 zmm0 {k1}{z}, zmm1', 'vmovdqu32 [bx], zmm0', 'vmovdqu64 zmm0, [bx+0x80]', 'vmovdqa32 xmm0, [bx+0x10]', 'vmovdqa32 ymm0, [bx+0x20]', 'vmovdqa32 ymm0, [bx+0x21]',
    'vpaddd zmm0, zmm1, zmm2', 'vpaddd zmm0, zmm1, [bx]{1to16}', 'vpaddq zmm0, zmm1, [bx]{1to8}', 'vpandd zmm0, zmm1, zmm2', 'vpandq zmm0, zmm1, zmm2', 'vpord zmm0, zmm1, [bx]', 'vpxorq zmm0, zmm1, zmm2', 'vpternlogd zmm0, zmm1, zmm2, 5',
    'vpternlogq zmm0, zmm1, [bx], 5', 'vpcmpeqd k1, zmm0, zmm1', 'vpcmpeqd k1 {k2}, zmm0, zmm1', 'vpcmpd k1, zmm0, zmm1, 5', 'vpcmpud k1, zmm0, [bx], 5', 'vcmpps k1, zmm0, zmm1, 5', 'vcmpps k1, zmm0, zmm1, 5, {sae}', 'vptestmd k1, zmm0, zmm1',
    'kmovw k1, k2', 'kmovw k1, eax', 'kmovw eax, k1', 'kmovw k1, [bx]', 'kmovw [bx], k1', 'kmovq k1, rax', 'kmovq rax, k1', 'kmovb k1, eax', 'kmovd k1, eax', 'kmovd eax, k1', 'kandw k1, k2, k3', 'korw k1, k2, k3', 'kxorw k1, k2, k3', 'knotw k1, k2',
    'kortestw k1, k2', 'ktestw k1, k2', 'kshiftlw k1, k2, 5', 'kshiftrq k1, k2, 5', 'kunpckbw k1, k2, k3', 'kaddw k1, k2, k3', 'vpbroadcastd zmm0, eax', 'vpbroadcastd zmm0, xmm1', 'vpbroadcastq zmm0, [bx]', 'vbroadcastss zmm0, xmm1', 'vbroadcastf32x4 zmm0, [bx]',
    'vbroadcasti64x4 zmm0, [bx]', 'vextractf32x4 xmm0, zmm1, 1', 'vextractf64x4 ymm0, zmm1, 1', 'vinsertf32x4 zmm0, zmm1, xmm2, 1', 'vinserti64x4 zmm0, zmm1, ymm2, 1', 'vshuff32x4 zmm0, zmm1, zmm2, 5', 'vpermt2d zmm0, zmm1, zmm2', 'vpermi2q zmm0, zmm1, [bx]',
    'vpermd zmm0, zmm1, zmm2', 'vpermq zmm0, zmm1, 5', 'vpermq zmm0, zmm1, zmm2', 'vpermps zmm0, zmm1, zmm2', 'vcompressps zmm0 {k1}, zmm1', 'vcompressps [bx] {k1}, zmm1', 'vexpandps zmm0 {k1}{z}, [bx]', 'vpcompressd zmm0 {k1}, zmm1', 'vscatterdps [eax+zmm1*4] {k1}, zmm0',
    'vgatherdps zmm0 {k1}, [eax+zmm1*4]', 'vgatherqps ymm0 {k1}, [eax+zmm1*4]', 'vpgatherdd zmm0 {k1}, [eax+zmm1*4]', 'vpscatterqq [eax+zmm1*8] {k1}, zmm0', 'vcvtdq2ps zmm0, zmm1', 'vcvtps2dq zmm0, zmm1, {rn-sae}', 'vcvttps2dq zmm0, zmm1, {sae}', 'vcvtps2pd zmm0, ymm1',
    'vcvtpd2ps ymm0, zmm1', 'vcvtusi2ss xmm0, xmm1, eax', 'vcvtusi2sd xmm0, xmm1, eax', 'vcvtss2usi eax, xmm0', 'vcvtsd2usi eax, xmm0', 'vcvtqq2pd zmm0, zmm1', 'vcvtpd2qq zmm0, zmm1', 'vfmadd231ps zmm0, zmm1, zmm2', 'vfmadd231ps zmm0, zmm1, zmm2, {rn-sae}',
    'vfmadd231ps zmm0, zmm1, [bx]{1to16}', 'vfmadd213pd zmm0 {k1}, zmm1, zmm2', 'vrcp14ps zmm0, zmm1', 'vrsqrt14pd zmm0, [bx]', 'vrndscaleps zmm0, zmm1, 5', 'vrangeps zmm0, zmm1, zmm2, 5', 'vreduceps zmm0, zmm1, 5', 'vgetexpps zmm0, zmm1', 'vgetmantps zmm0, zmm1, 5',
    'vscalefps zmm0, zmm1, zmm2', 'vfixupimmps zmm0, zmm1, zmm2, 5', 'vpmovdb xmm0, zmm1', 'vpmovdw ymm0, zmm1', 'vpmovqb xmm0, zmm1', 'vpmovsdb xmm0, zmm1', 'vpmovusdb xmm0, zmm1', 'vpmovdb [bx], zmm1', 'vpmovzxbd zmm0, xmm1', 'vpmovsxwd zmm0, ymm1',
    'vpmullq zmm0, zmm1, zmm2', 'vpmulld zmm0, zmm1, zmm2', 'vpsllvw zmm0, zmm1, zmm2', 'vpsraq zmm0, zmm1, 5', 'vpsraq xmm0, xmm1, xmm2', 'vprold zmm0, zmm1, 5', 'vprolvd zmm0, zmm1, zmm2', 'vpabsq zmm0, zmm1', 'vpmaxsq zmm0, zmm1, zmm2', 'vpconflictd zmm0, zmm1',
    'vplzcntd zmm0, zmm1', 'vpopcntd zmm0, zmm1', 'vpshufbitqmb k1, zmm0, zmm1', 'vpdpbusd zmm0, zmm1, zmm2', 'vpmadd52luq zmm0, zmm1, zmm2', 'vbroadcastsd zmm0, [bx]', 'vmovd xmm16, eax', 'vmovq xmm0, xmm1', 'vmovaps zmm16, zmm17', 'vmovaps zmm31, zmm30',
    'vmovaps xmm16, xmm17', 'vmovaps ymm16, ymm17', 'vaddps xmm16, xmm17, xmm18', 'vaddps zmm0, zmm1, zmm2 {k1}', 'vpbroadcastb zmm0, eax', 'vpbroadcastw zmm0 {k1}, eax', 'valignd zmm0, zmm1, zmm2, 5', 'vpmovm2d zmm0, k1', 'vpmovd2m k1, zmm0', 'vpcmpb k1, zmm0, zmm1, 5',
    'vpblendmd zmm0 {k1}, zmm1, zmm2', 'vblendmps zmm0 {k1}, zmm1, [bx]', 'vdbpsadbw zmm0, zmm1, zmm2, 5', 'vpmultishiftqb zmm0, zmm1, zmm2', 'vpermb zmm0, zmm1, zmm2', 'vpermw zmm0, zmm1, zmm2', 'vcvtne2ps2bf16 zmm0, zmm1, zmm2', 'vdpbf16ps zmm0, zmm1, zmm2',
    'vcvtph2ps zmm0, ymm1', 'vcvtps2ph ymm0, zmm1, 5', 'vcvtps2ph [bx], zmm1, 5', 'vpextrw eax, xmm16, 1', 'vpinsrb xmm16, xmm1, eax, 1', 'vextractf32x4 [bx], zmm1, 1', 'vmovdqu8 zmm0 {k1}, [bx+0x40]', 'vmovdqu8 zmm0 {k1}, [bx+0x41]')
add(64, 'vaddps zmm8, zmm9, zmm10', 'vaddps zmm16, zmm17, zmm18', 'vaddps zmm31, zmm30, zmm29', 'vaddps zmm0, zmm1, [r8]', 'vaddps zmm0, zmm1, [r8+r9*2+0x40]', 'vaddps zmm0, zmm1, [r16]', 'vmovaps zmm16, [rax]', 'vmovaps xmm31, xmm0', 'vpbroadcastd zmm0, r8d',
    'vpbroadcastq zmm0, r8', 'vcvtsi2sd xmm16, xmm1, r8', 'vgatherdps zmm16 {k1}, [rax+zmm17*4]', 'vpgatherqq zmm0 {k1}, [r8+zmm9*8]', 'kmovq k1, r8', 'kmovd r8d, k1', 'vpextrq r8, xmm16, 1', 'vmovq xmm16, r8', 'vmovd r9d, xmm16', 'vaddps xmm16, xmm1, xmm2',
    'vaddps xmm0, xmm16, xmm2', 'vaddps xmm0, xmm1, xmm16', 'vaddps ymm16, ymm17, ymm18', 'vpcmpeqd k1, zmm16, zmm17', 'vpermq zmm16, zmm17, 5', 'vscatterdps [rax+zmm16*4] {k1}, zmm0', 'vaddps zmm0, zmm1, [rax+zmm2*4]')

# ---------------------------------------------------------------- prefixes and operand-size forms
add('all', 'o16 nop', 'o32 nop', 'o16 pushf', 'o32 pushf', 'a16 lodsb', 'a32 stosb', 'a32 movsw', 'a16 scasb', 'o16 movsb', 'lock nop', 'lock mov eax, ebx', 'lock add eax, ebx', 'lock inc eax', 'rep nop', 'repe nop', 'repne nop', 'rep add eax, ebx',
    'repe movsb', 'repne movsb', 'wait nop', 'cs nop', 'ds nop', 'es nop', 'fs nop', 'gs nop', 'ss nop', 'cs mov eax, ebx', 'fs mov eax, [bx]', 'cs mov eax, [bx]', 'fs fs mov eax, [bx]', 'o16 o32 nop', 'a16 a32 nop', 'lock lock nop', 'rep rep nop',
    'lock rep nop', 'bnd nop', 'xacquire nop', 'xrelease nop', 'xacquire lock inc dword [bx]', 'xrelease lock dec dword [bx]', 'xacquire xchg [bx], eax', 'xrelease mov [bx], eax', 'xrelease mov byte [bx], 5', 'xacquire mov [bx], eax',
    'xrelease add [bx], eax', 'xacquire add [bx], eax', 'lock xacquire inc dword [bx]', 'xacquire lock add eax, ebx', 'xrelease lock xchg [bx], eax', 'cs jne $+0x10', '{pt} jne $+0x10', '{pn} jne $+0x10', '{pt} jmp $+5', 'ds je $+5', 'cs je $+0x100',
    '{pt} je near $+0x100', 'rep ret', 'repe ret', 'repne ret', 'rep pause', 'o16 ret', 'lock jmp $+5', 'rep call $+5', 'rep movsb', 'rep a16 movsb')
add(64, 'o64 nop', 'o64 mov eax, ebx', 'o64 add eax, ebx', 'o32 mov rax, rbx', 'o16 mov rax, rbx', 'o64 mov ax, bx', 'o16 mov ax, bx', 'o32 mov ax, bx', 'a64 mov eax, [rax]', 'a32 mov eax, [rax]', 'a16 mov eax, [rax]', 'a32 mov eax, [eax]', 'a64 mov eax, [eax]',
    'a32 lodsb', 'a64 lodsb', 'a16 lodsb', 'o64 lodsd', 'o16 lodsd', 'o64 movsq', 'o16 movsq', 'es nop', 'ds nop', 'cs nop', 'ss nop', 'fs nop', 'gs nop', 'es mov eax, [rax]', 'ds mov eax, [rax]', 'cs mov eax, [rax]', 'ss mov eax, [rax]',
    'fs mov eax, [rax]', 'gs mov eax, [rax]', 'cs jne $+0x10', 'ds jne $+0x10', 'es jne $+0x10', 'osp mov rax, rbx', 'osp mov eax, ebx', 'osp mov ax, bx', 'asp mov eax, [rax]', 'asp mov eax, [eax]', 'o64 push rax', 'o16 push rax', 'o32 push rax', 'o64 push ax',
    'o16 push ax', 'o64 pop rax', 'o16 push word 5', 'push word [rax]', 'o64 call rax', 'o16 call rax', 'o64 jmp rax', 'o16 jmp rax', 'o16 ret', 'o64 ret', 'o16 iretq', 'o64 iret', 'o32 iret', 'o16 iret', 'o64 iretd', 'o64 sysret', 'o64 syscall')
add(32, 'o64 nop', 'o64 mov eax, ebx', 'a64 mov eax, [eax]', 'mov rax, rbx', 'mov r8, rax', 'push rax', 'mov eax, r8d', 'mov al, sil', 'mov eax, [rax]', 'lea eax, [rax]', 'add rax, rbx', 'mov eax, [eax+r8]', 'mov eax, [r8d]', 'dec eax', 'inc ebx', 'mov r8b, al',
    'movsxd eax, ebx', 'movsxd rax, ebx', 'syscall', 'swapgs', 'sysretq', 'lds eax, [ebx]', 'les eax, [ebx]', 'pusha', 'aaa', 'daa', 'bound eax, [ebx]', 'arpl ax, bx', 'into', 'salc', 'jmp 0x1234:0x56789abc', 'call 0x1234:0x56789abc', 'call far 0x1234:0x56789abc',
    'xchg r8d, eax', 'pop es', 'push cs', 'push ds')
add(16, 'dec ax', 'inc bx', 'dec eax', 'inc ebx', 'o32 inc eax', 'o16 inc ax', 'mov eax, ebx', 'mov eax, [ebx]', 'mov eax, [eax*2]', 'push eax', 'o32 push eax', 'o16 push eax', 'lea bx, [eax]', 'lea eax, [ebx+eax*4]', 'a32 mov ax, [eax]', 'a32 lodsb', 'a16 lodsb', 'o32 lodsb',
    'o32 lodsw', 'o32 lodsd', 'o16 lodsd', 'o32 pushf', 'o16 pushf', 'pushfd', 'popfd', 'o32 popf', 'mov ax, [ebx]', 'mov [ebx], ax', 'mov [eax+ebx*4+0x12345678], ax', 'mov eax, [0x12345678]', 'mov eax, [a32 0x12345678]', 'mov al, [a32 0x12345678]', 'mov [a32 0x12345678], al')

# ---------------------------------------------------------------- invalid things (NASM rejects; the encoder must refuse too)
add('all', 'mov', 'mov al', 'mov al, bx', 'mov ax, bl', 'mov eax, bx', 'mov [bx], [bx]', 'mov [bx], 5', 'add [bx], 5', 'inc [bx]', 'mov cs, ax', 'pop cs', 'mov al, 300', 'mov ax, 70000', 'push al', 'push byte al', 'xchg al, ax', 'lea ax, bx', 'lea ax, 5',
    'jmp short $+0x200', 'jcc $+5', 'mov ah, [bx], 5', 'int', 'int 1, 2', 'shl eax, ebx', 'shl eax, cx', 'div', 'imul', 'out 5, bx', 'in al, bx', 'movzx eax, ecx', 'movzx eax', 'bt eax', 'cmp [bx], [bx]', 'test [bx], [bx]', 'mov ds, cs', 'mov ds, 5', 'mov [bx], ds, ax',
    'lock mov [bx], eax', 'lock inc [bx]', 'lock inc eax', 'lock nop', 'rep inc eax', 'xacquire mov eax, ebx', 'bnd mov eax, ebx', 'bnd add eax, ebx', 'popf eax', 'ret 1, 2', 'ret eax', 'call far eax', 'call far 5', 'jmp far eax', 'jmp far 5', 'jmp 5:5:5', 'call 5:5:5',
    'fadd st0, st0, st0', 'fld', 'fild', 'fld st8', 'faddp st0', 'fadd dword', 'fxch st1, st2', 'movq mm0, xmm0', 'movd mm0, ax', 'paddb mm0, xmm0', 'paddb xmm0, mm0', 'movaps xmm0, mm0', 'movaps eax, xmm0', 'cvtsi2ss xmm0, ax', 'pshufd xmm0, xmm1', 'pshufd xmm0, xmm1, 300',
    'pextrw eax, xmm0', 'vaddps xmm0, xmm1', 'vaddps xmm0, ymm1, xmm2', 'vaddps xmm0, xmm1, ymm2', 'vaddps ymm0, xmm1, ymm2', 'vaddps zmm0, ymm1, zmm2', 'vmovaps xmm0, ymm1', 'vpermq ymm0, ymm1', 'vfmadd231ps xmm0, xmm1', 'vgatherdps xmm0, [eax+ymm1*4], xmm2', 'kmovw k1, ax',
    'kmovw k1, k2, k3', 'vaddps zmm0 {k8}, zmm1, zmm2', 'vaddps zmm0 {z}, zmm1, zmm2', 'vaddps zmm0, zmm1, [bx]{1to8}', 'vaddps xmm0, xmm1, [bx]{1to16}', 'vaddps zmm0, zmm1, zmm2, {sae}', 'vaddps zmm0, zmm1, [bx], {rn-sae}', 'vaddps ymm0, ymm1, ymm2, {rn-sae}',
    'vpaddb zmm0, zmm1, [bx]{1to16}', 'vmovaps [bx], [bx]', 'vaddps zmm0, zmm1', 'mov rax, [bx]', 'mov [rax], 5', 'mov eax, [esp*2]', 'mov eax, [esp+esp]', 'mov eax, [ax+bx]', 'mov eax, [bx+bp]', 'mov eax, [si+si]', 'mov eax, [bx*2]', 'mov eax, [bx+eax]',
    'mov eax, [eax+bx]', 'mov eax, [eax*3+ebx*5]', 'mov eax, [eax*8+ebx*2]', 'mov eax, [eax+ebx+ecx]', 'mov eax, [eax*16]', 'mov eax, [word eax]', 'mov eax, [qword eax]', 'mov eax, [dword bx]', 'mov eax, [qword bx]', 'mov eax, [a64 eax]', 'rep', 'lock', 'o16', 'a32',
    'o64 mov ax, bx', 'a64 mov eax, [eax]', 'push cs, 5', 'pop', 'push', 'inc', 'dec', 'not', 'neg', 'mul', 'ret ret', 'pusha eax', 'enter 5', 'enter', 'enter 0x10000, 0', 'enter 5, 256', 'leave 5', 'xlat 5', 'in 5, al', 'out al, 5', 'cmpxchg8b eax', 'cmpxchg16b [bx]',
    'movsb [bx], [bx]', 'movsb al', 'stosb bl', 'lodsb dx', 'lodsb [bx]', 'cmpsb [bx]', 'scasb [bx]', 'insb al', 'outsb dx')

# ---------------------------------------------------------------- DB/data is not tested here (see the unit tests)

# ================================================================ run nasm
def run_nasm(bits, lines, extra=()):
    """Assemble the lines; return {index: bytes or None}"""
    live = list(range(len(lines)))
    rejected = set()
    for attempt in range(12):
        src = ['bits %d' % bits]
        index_of_line = {}
        for k, i in enumerate(live):
            src.append(lines[i])
            index_of_line[len(src)] = i
            src.append(MARKDB)
        path = os.path.join(WORK, 'b%d.asm' % bits)
        outp = os.path.join(WORK, 'b%d.bin' % bits)
        if os.path.exists(outp):
            os.remove(outp)
        with open(path, 'w', newline='\n') as f:
            f.write('\n'.join(src) + '\n')
        p = subprocess.run([NASM, *extra, '-f', 'bin', '-o', outp, path], capture_output=True, text=True)
        bad = set()
        for l in p.stderr.splitlines():
            parts = l.split(':')
            # file:LINE: error: msg   (the path may hold a drive colon)
            for idx, tok in enumerate(parts):
                if tok.strip().split(' ')[0] in ('error', 'fatal', 'panic'):
                    try:
                        ln = int(parts[idx - 1])
                        if ln in index_of_line:
                            bad.add(index_of_line[ln])
                    except ValueError:
                        pass
                    break
        if p.returncode == 0:
            data = open(outp, 'rb').read()
            chunks = data.split(MARK)
            if len(chunks) != len(live) + 1 or chunks[-1] != b'':
                raise SystemExit('marker count mismatch for bits %d: %d vs %d' % (bits, len(chunks), len(live) + 1))
            res = {}
            for i in rejected:
                res[i] = None
            for k, i in enumerate(live):
                res[i] = chunks[k]
            return res
        if not bad:
            sys.stderr.write(p.stderr[:3000])
            raise SystemExit('nasm failed without a locatable line (bits %d)' % bits)
        rejected |= bad
        live = [i for i in live if i not in bad]
    raise SystemExit('too many retries')


SKIP = {'call -1', 'call $-1x', 'jmp 0x1234', 'wait', 'wait nop', 'lock', 'rep', 'o16', 'a32', 'notrack jmp eax', 'jmp 0x1234:0x5678x'}
rows = []
for bits in (16, 32, 64):
    lines = cases[bits]
    res = run_nasm(bits, lines)
    n_ok = n_bad = 0
    for i, l in enumerate(lines):
        b = res[i]
        if b is not None and len(b) == 0:
            continue   # a label or a bare prefix, not an instruction the reader handles
        if l in SKIP:
            continue
        if b is None:
            rows.append((bits, l, '!'))
            n_bad += 1
        else:
            rows.append((bits, l, ' '.join('%02x' % c for c in b)))
            n_ok += 1
    print('bits %d: %d cases, %d encoded, %d rejected' % (bits, len(lines), n_ok, n_bad))


def cstr(s):
    return '"' + s.replace('\\', '\\\\').replace('"', '\\"') + '"'


with open(OUT, 'w', encoding='utf-8', newline='\n') as f:
    f.write('// GENERATED by state/work/x86asm-agents/encoder/gen_encode_tests.py from the output of native/nasm/build/bin/nasm.exe\n')
    f.write('// (-f bin): {bits, source line, NASM\'s bytes ("!" when NASM rejects the line)}. Do not edit.\n')
    for bits, l, h in rows:
        f.write('    {%d, %s, %s},\n' % (bits, cstr(l), cstr(h)))
print('wrote', len(rows), 'cases to', OUT)

# ---- --masm cases
MASM = {16: [], 32: [], 64: []}
for b in (16, 32):
    for op in ALU + ['mov']:
        MASM[b] += ['%s ax, bx' % op, '%s bx, ax' % op, '%s al, bl' % op, '%s eax, ebx' % op, '%s ax, 5' % op, '%s ax, 0x1234' % op,
                     '%s ax, 0x7f' % op, '%s ax, -1' % op, '%s eax, 5' % op, '%s bx, 5' % op, '%s word [bx], 5' % op, '%s cx, dx' % op]
    MASM[b] += ['test ax, bx', 'xchg ax, bx', 'xchg bx, ax', 'lea eax, [ebx*2]', 'lea eax, [ebx*2+4]', 'mov eax, [ebx*2]', 'mov eax, [ebx*4]',
                'mov eax, [ebx+ebx]', 'mov eax, [ebx*3]', 'jmp $+5', 'je $+0x7f', 'mov ax, [bx]', 'mov [bx], ax', 'imul ax, bx', 'add ax, strict word 5',
                'cmp ax, 0xffff', 'sub ax, 0x80', 'and ax, 0xff', 'or ax, 1', 'xor ax, ax', 'mov esp, ebp', 'mov ebp, esp', 'sbb ax, ax']
MASM[64] += ['and ax, 5', 'xor eax, eax']
rows = []
for bits in (16, 32, 64):
    lines = []
    for l in MASM[bits]:
        if l not in lines:
            lines.append(l)
    res = run_nasm(bits, lines, ['--masm'])
    for i, l in enumerate(lines):
        bb = res[i]
        if bb is not None and len(bb) == 0:
            continue
        rows.append((bits, l, '!' if bb is None else ' '.join('%02x' % c for c in bb)))
with open(OUT.replace('encode_cases', 'encode_masm_cases'), 'w', encoding='utf-8', newline=chr(10)) as f:
    f.write('// GENERATED by state/work/x86asm-agents/encoder/gen_encode_tests.py from nasm.exe --masm -f bin. Do not edit.' + chr(10))
    for bits, l, h in rows:
        f.write('    {%d, %s, %s},' % (bits, cstr(l), cstr(h)) + chr(10))
print('masm cases', len(rows))
