[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_function_at

`binary.h` · function · since 0.1

The function that starts at an address: one the binary names, else a new one named sub_<address>, added to

```c
cb_status cb_binary_function_at(cb_binary* binary, cb_address address, const cb_function** out);
```

the binary (it is not counted by cb_binary_function_count).

| Parameter | |
|---|---|
| `binary` | the binary. |
| `address` | where the function starts. |
| `out` | receives the function (owned by the binary). |

**Returns.** CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL argument or an address of style CB_ADDRESS_NONE.

**Thread safety.** One thread at a time per binary.

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

