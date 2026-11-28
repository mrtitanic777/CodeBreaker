# CodeBreaker SDK samples

One small program per feature, each on the installed SDK (`find_package(CodeBreaker)`) and the shared [framework](framework/)
(command line, starting the SDK, errors in the library's own words, `--time`). The reference is [docs/sdk/reference](../docs/sdk/reference/README.md).

| Sample | Language | Shows |
|---|---|---|
| [HelloBinary](HelloBinary/) | C | open a file: format, processor, sections, imports, exports, functions (`binary.h`) |
| [HelloDecompile](HelloDecompile/) | C++ | one function, or a whole module, as C (`decompile.h` through `cb.hpp`) |
| [DisasmAnyCpu](DisasmAnyCpu/) | C | one disassembler for every described processor (`disasm.h`) |

Build them against an installed SDK:

```
cmake --install build --component sdk --prefix build\sdk        (after building the target cbsdk)
cmake -S samples -B build\samples -G Ninja -DCMAKE_PREFIX_PATH=%CD%\build\sdk
cmake --build build\samples
set PATH=%CD%\build\sdk\bin;%PATH%
build\samples\HelloBinary\HelloBinary.exe src\formats\testdata\lz32.dll
```

`tools\sdk\samples.cmd` does all of it and compares each sample's output with `tests/sdk/golden` (the gate runs it).
