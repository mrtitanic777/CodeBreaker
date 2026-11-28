[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_sections

`binary.h` · function · since 0.1

The binary's sections, in the order the file lists them.

```c
const cb_section* cb_binary_sections(const cb_binary* binary, size_t* count);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `count` | receives how many there are (0 for a NULL binary). |

**Returns.** the first of count records the binary owns; NULL when there are none.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

