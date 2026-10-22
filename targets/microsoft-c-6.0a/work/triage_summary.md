# Phase 3 - origin triage (first pass)

CRT library functions identified in each pass via `wt inverse libmatch` (FLIRT-style: mask
FIXUPP sites, slide-search the .LIB functions in the image). Identified = toolchain, not
reconstructed. Remaining = authored compiler code (the reconstruction target).

| Pass | CRT lib | Functions | CRT-identified | Authored (remain) | CRT % of image |
|---|---|---:|---:|---:|---:|
| CL | SLIBCR | 209 | 41 | 168 | 12% |
| C1 | MLIBCR | 1016 | 45 | 971 | 2% |
| C1L | LLIBCR | 982 | 43 | 939 | 2% |
| C2 | LLIBCR | 991 | 22 | 969 | 0% |
| C3 | LLIBCR | 951 | 24 | 927 | 0% |

The low CRT fraction is expected and is the point: the passes are almost entirely the compiler's
OWN authored code. Next-pass improvements: match additional libs (LIBH, *FA/*FP math, EM, 87),
align leaders to exact CRT entries, and fold in `@(#)` module boundaries (`work/module_manifest.txt`).

Per-pass hit lists: `work/triage/<pass>_<lib>.txt`.
