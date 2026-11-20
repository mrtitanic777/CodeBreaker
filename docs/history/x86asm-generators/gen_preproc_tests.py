"""Generate preproc_test.cpp: run each case through the real nasm.exe -E and embed source + expected output."""
import subprocess, os, sys, json

HERE = os.path.dirname(os.path.abspath(__file__))
NASM = os.path.join(HERE, '..', '..', '..', '..', 'native', 'nasm', 'build', 'bin', 'nasm.exe')
NASM = os.path.abspath(NASM)
WORK = os.path.join(HERE, 'cases')
os.makedirs(WORK, exist_ok=True)

# (name, main source, {extra files}, masm?)
CASES = []


def case(name, src, files=None, masm=False):
    CASES.append((name, src, files or {}, masm))


case('define_basic', r'''
%define FOO 1+2
%define BAR(x) (x)*2
%define BAZ(x,y) x+y
%xdefine NOW FOO
%undef FOO
mov eax, FOO
mov eax, NOW
mov eax, BAR(3)
mov eax, BAZ(1, 2)
%idefine Mixed Case
mov eax, mixed
%define EMPTY
db EMPTY 1
''')

case('define_params', r'''
%define A(x,y) x %+ y
%define B(=x) x
%define C(&x) x
%define D(x+) x
%define E(x,y,z) [x] [y] [z]
%define F(x) %str(x)
mov A(ea,x), 1
dd B(1+2*3)
db C(hello world)
db D(a, b, c)
db E(1,{2,3},4)
db F(abc)
db %strlen(F(xyz))
''')

case('smacro_recursion', r'''
%define foo foo+1
%define bar(x) bar(x)+x
mov eax, foo
mov eax, bar(2)
%define a b
%define b a
mov eax, a
%define %$l 4
''')

case('assign', r'''
%assign i 1
%assign i i+1
%assign j i*8
db i, j
%assign k (j >> 1) | 1
db k
%assign big 0x7fffffff + 1
dd big
%assign neg -5
db neg
%assign s 3 > 2
db s
''')

case('mmacro_basic', r'''
%macro mov2 2
  mov %1, %2
  mov %2, %1
%endmacro
%macro nop0 0
  nop
%endmacro
%macro many 1-3
  db %0, %1
  %if %0 > 1
  db %2
  %endif
%endmacro
mov2 eax, ebx
nop0
many 1
many 1, 2
many 1, 2, 3
''')

case('mmacro_defaults_greedy', r'''
%macro d 1-3 10, 20
  dw %1, %2, %3
%endmacro
%macro g 1+
  db %1
%endmacro
%macro star 1-*
  %rep %0
    db %1
    %rotate 1
  %endrep
%endmacro
d 1
d 1, 2
d 1, 2, 3
g a, b, c
star 1, 2, 3, 4
''')

case('mmacro_labels', r'''
%macro loop 1
%%top:
  dec %1
  jnz %%top
%endmacro
%macro lbl 0
%00:
  nop
%endmacro
loop eax
loop ebx
here: lbl
there lbl
''')

case('mmacro_cc', r'''
%macro jcc 2
  j%+1 %2
  j%-1 %2
%endmacro
jcc ae, target
jcc z, other
%macro pr 0-*
  %rep %0
  db %1
  %rotate 1
  %endrep
%endmacro
pr 1, 2, 3
%macro pt 3
  db %{1:3}
  db %{3:1}
  db %{-1:-2}
%endmacro
pt a, b, c
''')

case('mmacro_misc', r'''
%macro n 0.nolist
  nop
%endmacro
%imacro Ic 0
  db 'ic'
%endmacro
n
IC
ic
%unmacro Ic 0
ic
%macro q 1
  %? %?? %1
  %exitmacro
  db 99
%endmacro
q x
Q x
''')

case('rep', r'''
%assign n 3
%rep n
  db n
  %assign n n-1
%endrep
%rep 0
  db 0
%endrep
%assign i 0
%rep 10
  %if i = 3
    %exitrep
  %endif
  db i
  %assign i i+1
%endrep
%rep 2
 %rep 2
  db 7
 %endrep
%endrep
''')

case('ifs', r'''
%define X 1
%ifdef X
 db 'X def'
%endif
%ifndef Y
 db 'Y undef'
%endif
%if X = 1
 db 'x is 1'
%elif X = 2
 db 'x is 2'
%else
 db 'other'
%endif
%ifidn X, 1
 db 'idn'
%endif
%ifidn abc, ABC
 db 'no'
%else
 db 'case'
%endif
%ifidni abc, ABC
 db 'idni'
%endif
%ifnum 5
 db 'num'
%endif
%ifstr "x"
 db 'str'
%endif
%ifid foo
 db 'id'
%endif
%ifempty
 db 'empty'
%endif
%iftoken a
 db 'token'
%endif
%ifn X = 1
 db 'not'
%else
 db 'is'
%endif
%macro m 1
%endmacro
%ifmacro m 1
 db 'macro m'
%endif
%ifmacro m 2
 db 'macro m 2'
%endif
%ifdirective %define
 db 'dir'
%endif
%if 1
 %if 0
  db 'no'
 %elif 1
  db 'nested'
 %endif
%endif
''')

case('contexts', r'''
%push foo
%define %$x 1
%$x
%push bar
%define %$x 2
%$x
%$$x
%ifctx bar
 db 'bar'
%endif
%pop
%$x
%repl baz
%ifctx baz
 db 'baz'
%endif
%pop
''')

case('strings', r'''
%strlen L "hello"
db L
%substr S "hello" 2, 3
db S
%strcat C "ab", 'cd', "e"
db C
%defstr DS foo bar
db DS
%deftok DT 'a+b'
db DT
%strlen L2 `a\nb`
db L2
%substr T "hello" 2
db T
%substr U "hello" 2, -1
db U
%substr V "hello" 9
db V
''')

case('functions', r'''
db %strlen("abc"), %substr("hello", 2, 3), %strcat("a", "b")
db %chr(65, 66, 67), %ord("A"), %ord("AB", 1, 2)
db %hs2b("4142"), %b2hs("AB"), %b2hs("AB", "-")
db %num(255), %num(255, 4, 16), %num(5, 8, -2), %num(10, 0, 10)
db %abs(-5), %abs(7)
db %count(a, b, c), %sel(2, x, y, z), %cond(1, yes, no), %cond(0, yes, no)
db %eval(1+2), %str(a b), %tok("a+b")
dd %hex(255), %hex(-1)
db %find("b", "a", "b"), %find("z", "a"), %findi("B", "a", "b")
db %isnum(5), %isnum(x), %isid(x), %isidn(a, a), %isdef(x), %istoken(a b)
db %strlen(%str(a,b))
db %null(abc) 1
''')

case('map', r'''
%define sq(x) ((x)*(x))
%define add(a,b) (a+b)
db %map(sq, 1, 2, 3)
db %map(add:10, 1, 2)
db %map(add::2, 1, 2, 3, 4)
''')

case('builtin_macros', r'''
db __?LINE?__
db __?FILE?__
db __?BITS?__
db __?PASS?__
dd __?NASM_VERSION_ID?__
db __?NASM_MAJOR?__, __?NASM_MINOR?__
db __?NASM_VER?__
%line 100+2 other.asm
db __?LINE?__
db __?FILE?__
db __?LINE?__
%line 7 x.y
db __?FILE?__
db __?PTR?__
%ifdef __?utf16?__
db 1
%endif
''')

case('std_macros', r'''
%use altreg
mov r0, r1
mov r8, r9
struc mystruc
 .a: resb 1
 .b: resd 1
endstruc
istruc mystruc
 at mystruc.a, db 1
 at mystruc.b, dd 2
iend
align 16
alignb 4
sectalign off
cpu 386
''')

case('use_packages', r'''
%use fp
%use ifunc
%use smartalign
%use vtern
dd __?float64?__(1.0)
db ilog2(8)
alignmode nop
%ifusing fp
db 'fp'
%endif
%ifusable altreg
db 'usable'
%endif
%use nosuch
''')

case('includes', r'''
%include "inc1.inc"
db 1
%include "inc2.inc"
db __?FILE?__, __?LINE?__
%pathsearch P "inc1.inc"
db P
%ifidn __?FILE?__, "t.asm"
%endif
''', {'inc1.inc': 'db 10\n%define INC1 yes\n%include "inc3.inc"\ndb __?FILE?__, __?LINE?__\n',
      'inc2.inc': '%ifndef INC1\ndb 20\n%endif\ndb INC1\n',
      'inc3.inc': 'db 30\n'})

case('errors', r'''
%error "custom error"
%warning "custom warning"
%note "custom note"
%define
%macro
%endmacro
%endif
%else
%rep
%endrep
%pop
%include "nonexistent.inc"
%ifdef
%endif
%undef
mov %1
%0
%foo bar
db 1
''')

case('line_cont', '''
db 1, \\
   2, \\
   3
%define X \\
   4
db X ; comment \\
db 5
db "a;b" ; c
db `a\\nb`
db 'it''s'
%macro m 1
  db %1 ; hi
%endmacro
m 7 ; x
''')

case('local_labels', r'''
%macro a 0
%%x: jmp %%x
%endmacro
a
a
%macro b 0
  a
%endmacro
b
%define LBL %%x
LBL
''')

case('tokens', r'''
db 1e3, 1.5, 0x1f, $1f, 1_000, 0b101, 12h, 'str'
dd $, $$, 1.0e-3, 0x1p3
db a>>b, a<<b, a>>>b, a<=>b, a<>b, a!=b, a==b, a&&b, a||b, a^^b, a//b, a%%b
db a ? b : c
db %+1
db %%local, %$ctx
db %{foo}, %[bar]
db %?, %??
mov [%1], %2
''')

case('paste', r'''
%define a1 AA
%define n 1
%define cat(x,y) x %+ y
db cat(a, n)
db cat(a, 1)
db a %+ n
db a%+n
db a %+ %+ n
%define pre foo
%define pre2 pre %+ 2
db pre2
mov eax, cat(e, ax)
db cat(0x, 10)
db cat(1, 2)
db cat(., x)
''')

case('misc_directives', r'''
%pragma preproc sane_empty_expansion yes
%pragma foo bar baz
%pragma list options
%aliases off
%defalias AL FOO
%define FOO 1
db AL
%aliases on
%clear
db FOO
%clear all
%null junk
%stacksize flat
%arg a:dword, b:word
%local l1:byte, l2:dword
mov eax, a
mov eax, b
mov eax, l1
mov eax, l2
db %$localsize
''')

case('depend_realpath', r'''
%depend "dep.inc"
db 1
%require "dep.inc"
%require "dep.inc"
%include "dep.inc"
%include "dep.inc"
''', {'dep.inc': 'db 99\n'})

case('mmacro_params_more', r'''
%macro m 3
  %rotate -1
  db %1, %2, %3
  %rotate 1
  db %1, %2, %3
%endmacro
m a, b, c
%macro e 1-2
  db %0
%endmacro
e 1,
e 1, 2
e {1,2}
e {a b}, c
e a b
%macro z 0-1
  db %0
%endmacro
z
z 5
%macro w 1-2+
  db %1, %2
%endmacro
w 1, 2, 3
%macro c1 2
 db %1 %2
%endmacro
c1 x,
c1 ,y
''')

case('numbers_expr', r'''
%assign a 10
%assign b a / 3
%assign c a % 3
%assign d a // 3
%assign e a %% 3
%assign f -a
%assign g ~a
%assign i (a > 5) && (a < 20)
%assign j 7
%assign k 1 << 4
%assign l 0xff & 0x0f
%assign m 'a'
%assign n 'abc'
db b, c, d, e, f, g, i, j, k, l, m, n
''')

case('ifusing_nested_skip', r'''
%if 0
%macro skipped 0
%error "not seen"
%endmacro
%define SKIPPED 1
%if bogus garbage ((
%endif
%rep 3
%endrep
%else
 db 'ok'
%endif
db SKIPPED
%ifdef SKIPPED
%else
 db 'undefined'
%endif
''')

case('xdefine_params', r'''
%define two 2
%xdefine f(x) x + two
%undef two
%define two 3
db f(1)
%define g(x) x + two
db g(1)
%xdefine h 1+two
%define two 4
db h
''')

case('ifmacro_misc', r'''
%macro a 1-3
%endmacro
%macro b 0
%endmacro
%macro c 2+
%endmacro
%ifmacro a
db 'a'
%endif
%ifmacro a 2
db 'a2'
%endif
%ifmacro a 4
db 'a4'
%endif
%ifmacro b
db 'b'
%endif
%ifmacro b 1
db 'b1'
%endif
%ifmacro c 5
db 'c5'
%endif
%ifmacro a 1-*
db 'a1*'
%endif
%ifmacro nosuch
db 'no'
%endif
''')

case('listing_misc', r'''
%define LONG_MACRO_NAME_FOR_TEST(a, b, c, d) a b c d
db LONG_MACRO_NAME_FOR_TEST(1, 2, 3, 4)
%macro mm 1
 %define %%l %1
 db %%l
%endmacro
mm 1
mm 2
db 'end'
''')


FIX = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..', 'native', 'nasm', 'test', 'masm'))
for fn in sorted(os.listdir(os.path.join(FIX, 'fixtures'))):
    if fn.endswith('.asm'):
        txt = open(os.path.join(FIX, 'fixtures', fn), encoding='latin-1', newline='').read()
        case('masm_' + fn[:-4], txt, ({'cmacros_shim.inc': open(os.path.join(FIX, 'cmacros_shim.inc'), encoding='latin-1', newline='').read()} if 'cmacros' in txt.lower() else {}), True)

case('dbg1', """%use masm
_TEXT segment dword public use32 'CODE'
 nop
""")
case('dbg2', """%use masm
segment foo use32
 nop
""")
case('dbg3', """%use masm
.386
 nop
""")


def run_nasm(name, src, files, masm):
    d = os.path.join(WORK, name)
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, 't.asm'), 'w', newline='') as f:
        f.write(src)
    for fn, txt in files.items():
        with open(os.path.join(d, fn), 'w', newline='') as f:
            f.write(txt)
    args = [NASM] + (['--masm'] if masm else []) + ['-E', 't.asm']
    p = subprocess.run(args, cwd=d, capture_output=True)
    out = p.stdout.decode('latin-1').replace('\r\n', '\n')
    err = p.stderr.decode('latin-1').replace('\r\n', '\n')
    return out, err, p.returncode


def cs(s):
    out = ['"']
    for ch in s:
        if ch == '\\': out.append('\\\\')
        elif ch == '"': out.append('\\"')
        elif ch == '\n': out.append('\\n"\n        "')
        elif ch == '\r': out.append('\\r')
        elif ch == '\t': out.append('\\t')
        elif ord(ch) < 32 or ord(ch) > 126: out.append('\\x%02x' % ord(ch) + '""')
        else: out.append(ch)
    out.append('"')
    return ''.join(out)


def main():
    cases_cpp = []
    for name, src, files, masm in CASES:
        out, err, rc = run_nasm(name, src, files, masm)
        fl = ', '.join('{%s, %s}' % (cs(k), cs(v)) for k, v in files.items())
        cases_cpp.append('    {%s, %s,\n     {%s}, %s,\n     %s,\n     %s, %d},' % (cs(name), cs(src), fl, 'true' if masm else 'false', cs(out), cs(err), rc))
    tmpl = open(os.path.join(HERE, 'test_template.cpp'), encoding='utf8').read()
    res = tmpl.replace('/*CASES*/', '\n'.join(cases_cpp))
    open(os.path.join(HERE, '..', '..', '..', '..', 'native', 'cb', 'build', 'x86asm', 'preproc_test.cpp'), 'w', encoding='utf8', newline='\n').write(res)
    print(len(CASES), 'cases')


main()
