[The reference](../README.md) · [disasm.h](../disasm_h.md)

# cb_instructions_free

`disasm.h` · function · since 0.1

Free a list of instructions and every string in it.

```c
void cb_instructions_free(cb_instructions* instructions);
```

| Parameter | |
|---|---|
| `instructions` | NULL (nothing happens), or a list cb_disasm made. |

**Thread safety.** Any thread.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

