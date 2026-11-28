[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_function_address

`binary.h` · function · since 0.1

Where the function starts.

```c
cb_address cb_function_address(const cb_function* function);
```

**Returns.** the address; style CB_ADDRESS_NONE for a NULL function.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

