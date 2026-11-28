[The reference](../README.md) · [disasm.h](../disasm_h.md)

# cb_arch_list

`disasm.h` · function · since 0.1

Every processor CodeBreaker knows, in a fixed order.

```c
const cb_arch* cb_arch_list(size_t* count);
```

| Parameter | |
|---|---|
| `count` | receives how many there are. |

**Returns.** the first of count records the library owns, valid while it is loaded.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

