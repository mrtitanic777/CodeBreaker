[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_close

`binary.h` · function · since 0.1

Free a binary, its functions and every record and string it gave.

```c
void cb_binary_close(cb_binary* binary);
```

| Parameter | |
|---|---|
| `binary` | NULL (nothing happens), or an open binary. |

**Thread safety.** Any thread, when no other call is using the binary.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

