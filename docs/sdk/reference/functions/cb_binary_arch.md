[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_arch

`binary.h` · function · since 0.1

The processor the file's code is for, by the name cb_disasm takes ("x86-32", "x86-16", "ppc", "a64" ...).

```c
const char* cb_binary_arch(const cb_binary* binary);
```

A processor CodeBreaker has no description of yet (an x64 PE: "x86-64") is named all the same; cb_disasm says it is not supported.

**Returns.** a string the binary owns; "" when the file says no processor.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

