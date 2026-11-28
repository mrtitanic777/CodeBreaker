[The reference](../README.md) · [disasm.h](../disasm_h.md)

# cb_instructions_data

`disasm.h` · function · since 0.1

The instructions one cb_disasm call read, in address order.

```c
const cb_instruction* cb_instructions_data(const cb_instructions* instructions, size_t* count);
```

| Parameter | |
|---|---|
| `instructions` | the list. |
| `count` | receives how many there are. |

**Returns.** the first of count records the list owns, valid until cb_instructions_free; NULL when there are none.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

