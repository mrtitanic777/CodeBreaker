[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_function

`binary.h` · function · since 0.1

One of the binary's functions, in address order.

```c
const cb_function* cb_binary_function(const cb_binary* binary, size_t index);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `index` | 0 to cb_binary_function_count() - 1. |

**Returns.** the function (owned by the binary); NULL for an index out of range or a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

