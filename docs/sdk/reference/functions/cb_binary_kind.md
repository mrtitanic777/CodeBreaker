[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_kind

`binary.h` · function · since 0.1

The lane that reads the file ("pe", "ne", "le", "dos"), or the family of a file that is not an executable.

```c
const char* cb_binary_kind(const cb_binary* binary);
```

**Returns.** a string the binary owns; "" when not known.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

