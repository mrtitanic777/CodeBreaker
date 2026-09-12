# Third-party notices

## empty_live.bin

`xbt/xbox/empty_live.bin` is an empty Xbox 360 LIVE content-header template,
redistributed from **iso2god-rs** by Ilia Pozdnyakov, under the MIT License:

```
MIT License

Copyright (c) 2023 Ilia Pozdnyakov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

The GoD/SVOD block-and-hash layout and CON-header field offsets used by
`xbt/xbox/god.py` were learned from the same MIT project and reimplemented as
original Python (format constants are facts, not copied code).

## xbt/crypto/

Integrated from **Xbox-360-Crypto** by GoobyCorp, BSD 3-Clause (see
`xbt/crypto/LICENSE`).

## xbt/vendor/rex_dxbc, xbt/vendor/rex_kernel

Vendored **RexGlue SDK** runtime (Copyright (c) 2026, Tom Clay
`<tomc@tctechstuff.com>`; portions derived from the **Xenia** project,
Copyright (c) 2022 Ben Vanik and Xenia project contributors), under the
BSD 3-Clause License (see each dir's `LICENSE`). This is the PowerPC
static-recompile *runtime library* that `ppcrecomp`-generated C++ links
against; the recompiler's own codegen logic (`xbt/ppc/port_*.py`,
`jumptable.py`) is original Python, a faithful reimplementation of the
RexGlue builders — not vendored.

## Native XEX / LZX code (clean-room)

`xbt/xbox/xex.py`, `xbt/xbox/_lzx.py`, `xbt/xbox/lzx_encode.py`,
`xbt/xbox/xex_sign.py`, and `xbt/xbox/idc_boilerplate.py` are original code,
reimplemented clean-room from the XEX2 container format, the Microsoft LZX/LZXD
bitstream, and the XEX header-signing scheme (format facts, not copied source).
They were validated byte-for-byte against a reference `xextool` build during
development; that reference is not redistributed, and the golden digests in
`xbt/xbox/selftest.py` lock the result without it.
