[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_function_find

`binary.h` · function · since 0.1

The function with this name (an export's name, "entry"), compared exactly.

```c
cb_status cb_binary_function_find(const cb_binary* binary, const char* name, const cb_function** out);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `name` | the name. |
| `out` | receives the function (owned by the binary). |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when the binary names no such function; CB_ERROR_INVALID_ARGUMENT for a NULL argument.

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

