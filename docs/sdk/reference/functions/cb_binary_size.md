[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_size

`binary.h` · function · since 0.1

The file's size in bytes.

```c
uint64_t cb_binary_size(const cb_binary* binary);
```

**Returns.** the size; 0 for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

