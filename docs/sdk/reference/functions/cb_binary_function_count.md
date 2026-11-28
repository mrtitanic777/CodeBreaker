[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_function_count

`binary.h` · function · since 0.1

How many functions the binary names: its entry point and the exports that are code, by address.

```c
size_t cb_binary_function_count(const cb_binary* binary);
```

(Functions found by analysis join this list in a later version.)

**Returns.** the count; 0 for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

