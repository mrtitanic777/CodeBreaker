[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_image_base

`binary.h` · function · since 0.1

Where the executable asks to be loaded (a PE's image base); 0 for a format without one.

```c
uint64_t cb_binary_image_base(const cb_binary* binary);
```

**Returns.** the base address.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

