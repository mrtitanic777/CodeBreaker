# ERROUT.EXE — recovery report

`ERROUT` runs a command with its **standard error redirected** to a file (or merged into
stdout). Usage strings: `no arguments`, `bad command line switch`, `missing file name`,
`missing command`, `cannot open file`, `cannot redirect standard error`, `command failed`
(errors `U1251`–`U125x`). It parses a small switch set, opens/creates the redirect target,
`dup2`s it onto handle 2, and `spawnvp`s the command.

- **entry** `__astart` @0x1c0 · **49** carved functions · **16** app + **33** CRT (idlib) ·
  plus `__chkstk`@0x3f6 (named by asmid; the recurring `xor ax,ax / call 0x3f6` prologue).

## Function map (app surface)

| addr | name | size | tier | notes |
|---|---|---|---|---|
| 0x0010 | `main` (sub_10) | 356 | analyzed | opens redirect, `dup2`/`close`, `spawnvp`, error exits |
| 0x0174 | `err1` (sub_174) | 37 | **recovered** | `fprintf(stderr,_NMSG_TEXT(n)); exit(1)` — 3-byte codegen delta† |
| 0x019a | `err2` (sub_19a) | 37 | recovered | twin of `err1`, different message base |
| 0x0ab6 | sub_ab6 | 622 | **CRT (missed)** | `_output`/printf core — `__flsbuf` cluster, not app |
| 0x0d78 | sub_d78 | 309 | CRT (missed) | printf formatting (`_strlen`, buffer) |
| 0x0eae | sub_eae | 238 | CRT (missed) | printf field/pad |
| 0x0f9c | sub_f9c | 162 | CRT (missed) | printf helper |
| 0x103e/1080/10de | sub_* | 57–104 | CRT (missed) | `__flsbuf` write wrappers (`_flushbuf`/`putc`) |
| 0x1146 | sub_1146 | 197 | CRT (missed) | printf output driver |
| 0x124e | sub_124e | 127 | app | switch/arg helper (no calls) — queued |
| 0x12ce | `inset` (sub_12ce) | 34 | **BYTE-EXACT** | char-in-set search; table @DGROUP:0x228 |
| 0x1392 | `parseargs` (sub_1392) | 399 | analyzed | command-line switch parser (calls `__chkstk`) |
| 0x1682 | sub_1682 | 76 | CRT (missed) | `__amallocbrk` wrapper (`malloc`/`sbrk`) |
| 0x19e4 | sub_19e4 | 10 | app | alt-entry thunk: `bl=0; jmp 0x1b1e` (shared body) |

† `err1`: recovered C is correct (`work/recover/err.c`), but the shipped object defers the
inner-call argument cleanup (one `add sp,2` omitted, legal because `exit` never returns) —
an optimization our probe flags didn't reproduce. Byte-exact pending the exact build flags.

## Verified reconstructions
- **`inset` (0x12ce)** — byte-exact. Identical to the `EXEMOD@0x1ade` idiom (only the
  `mov si,set` table pointer differs, a masked fixup). Source: `work/recover/inset.c`.

## Key lesson
Several "app" functions are **unmatched CRT** (the printf/`_output` cluster), not authored
code — do NOT reconstruct them; extend `idlib`/`asmid` to name them. The true app surface
of ERROUT is ~6 functions: `main`, `err1`/`err2`, `parseargs`, `inset`, the switch helper,
and the alt-entry thunk. This is the faithfulness-over-coverage discipline in action.
