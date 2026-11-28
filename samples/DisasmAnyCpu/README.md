# DisasmAnyCpu

One call, `cb_disasm`, for every processor CodeBreaker has a SLEIGH description of: each instruction's text, length, how control leaves it
(`cb_flow`) and where it goes. With no arguments it tours eight processors; `--binary` reads a file's entry point in the processor
`cb_binary_arch` names; `--list` prints `cb_arch_list`. Plain C.

```
DisasmAnyCpu
DisasmAnyCpu z80 3E12CD3412C9 0x100
DisasmAnyCpu --binary lz32.dll 12
DisasmAnyCpu --list
```
