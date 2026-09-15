# ERROUT.EXE — recovery report

`ERROUT` runs a command with its **standard error redirected** to a file (or merged into
stdout). Usage strings: `no arguments`, `bad command line switch`, `missing file name`,
`missing command`, `cannot open file`, `cannot redirect standard error`, `command failed`
(errors `U1251`–`U125x`). It parses a small switch set, opens/creates the redirect target,
`dup2`s it onto handle 2, and `spawnvp`s the command.

- **entry** `__astart` @0x1c0 · **49** carved functions · **16** app + **33** CRT (idlib) ·
  plus `__chkstk`@0x3f6 (named by asmid; the recurring `xor ax,ax / call 0x3f6` prologue).

## Function map (app surface — all 16 are genuine app code)

**Correction:** an earlier pass guessed the `__flsbuf` cluster was unmatched CRT. A brute
masked-match against all 4,153 library functions found **no** hit (best ~40–54%, random) —
so these are ERROUT's own code. ERROUT ships a **self-contained mini-`printf`** (it does not
call the CRT `_output`), keyed off a fixed format-state block at DGROUP:0x6b4–0x6d6.

| addr | name | size | tier | notes |
|---|---|---|---|---|
| 0x0010 | `main` (sub_10) | 356 | analyzed | opens redirect, `dup2`/`close`, `spawnvp`, error exits |
| 0x0174 | `err1` (sub_174) | 37 | **recovered** | `fprintf(stderr,_NMSG_TEXT(n)); exit(1)` — 3-byte codegen delta† |
| 0x019a | `err2` (sub_19a) | 37 | recovered | twin of `err1`, different message base |
| 0x0ab6 | `fmt` (sub_ab6) | 622 | analyzed | **mini-printf core**: scans `%`, flags `0`/`+`/space, 352B buffer |
| 0x0d78 | `fmt_num` (sub_d78) | 309 | analyzed | number → text (calls `strlen`, itoa-style) |
| 0x0eae | `fmt_pad` (sub_eae) | 238 | analyzed | field width / padding emit |
| 0x0f9c | `fmt_str` (sub_f9c) | 162 | analyzed | string-field emit |
| 0x103e/1080/10de | `putc*`/`flush` | 57–104 | analyzed | output via CRT `__flsbuf` |
| 0x1146 | `fmt_out` (sub_1146) | 197 | analyzed | output driver |
| 0x124e | `getwidth` (sub_124e) | 127 | analyzed | width/`*`/`-`/digit parse: `w = w*10 + (c-'0')` |
| 0x12ce | `inset` (sub_12ce) | 34 | **BYTE-EXACT** | char-in-set search; table @DGROUP:0x228 |
| 0x1392 | `parseargs` (sub_1392) | 399 | analyzed | command-line switch parser |
| 0x1682 | `xmalloc` (sub_1682) | 76 | analyzed | `__amallocbrk` (sbrk) wrapper |
| 0x19e4 | sub_19e4 | 10 | app | alt-entry thunk: `bl=0; jmp 0x1b1e` (shared body) |

† `err1`: recovered C is correct (`work/recover/err.c`), but the shipped object defers the
inner-call argument cleanup (one `add sp,2` omitted, legal because `exit` never returns) —
an optimization our probe flags didn't reproduce. Byte-exact pending the exact build flags.

## Verified reconstructions
- **`inset` (0x12ce)** — byte-exact. Identical to the `EXEMOD@0x1ade` idiom (only the
  `mov si,set` table pointer differs, a masked fixup). Source: `work/recover/inset.c`.

## Key lesson
The **brute-match cross-check** (test an "app" function against every library function
before reconstructing it) is what keeps triage honest — here it *reversed* a wrong guess and
confirmed the whole `printf` cluster is authored code. idlib's original triage was accurate.
ERROUT's real shape: `main` + `parseargs` + a hand-rolled `printf` family + `err1`/`err2` +
`inset` + `xmalloc`. That mini-printf is the bulk of the reconstruction work here.
