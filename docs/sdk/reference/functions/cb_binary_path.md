[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_path

`binary.h` · function · since 0.1

The path the binary was opened from.

```c
const char* cb_binary_path(const cb_binary* binary);
```

**Returns.** a string the binary owns; "" for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

