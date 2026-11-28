[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_entry

`binary.h` · function · since 0.1

Where the program starts.

```c
cb_address cb_binary_entry(const cb_binary* binary);
```

**Returns.** the entry point; style CB_ADDRESS_NONE when it has none (a resource-only DLL, a file that is not a program).

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

