[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_function_name

`binary.h` · function · since 0.1

The function's name ("LZCopy", "entry", "sub_10001000").

```c
const char* cb_function_name(const cb_function* function);
```

**Returns.** a string the binary owns; "" for a NULL function.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

