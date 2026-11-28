[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_open

`binary.h` · function · since 0.1

Open a file and read what it is.

```c
cb_status cb_binary_open(const char* path, cb_binary** out);
```

| Parameter | |
|---|---|
| `path` | the file, UTF-8. |
| `out` | receives the binary; close it with cb_binary_close. Left NULL when the call fails. |

**Returns.** CB_OK, also for a file CodeBreaker does not recognise (its format is then ""); CB_ERROR_NOT_FOUND when there is no such file; CB_ERROR_INVALID_ARGUMENT for a NULL path or out, or a folder; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the binary.

**Thread safety.** Any thread.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

