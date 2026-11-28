[The reference](../README.md) · [decompile.h](../decompile_h.md)

# cb_function_decompile

`decompile.h` · function · since 0.1

One function as C.

```c
cb_status cb_function_decompile(const cb_function* function, const cb_decompile_options* options, cb_string* out);
```

| Parameter | |
|---|---|
| `function` | the function (cb_binary_function, cb_binary_function_find, cb_binary_function_at). |
| `options` | NULL for the defaults. |
| `out` | receives the C: the function with the comment lines above it. Free it with cb_string_free. |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when the decompiler made no function at that address, or cannot run (cb_decompiler_check); CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c"; CB_ERROR_TOOL when the native decompiler failed; CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the string.

**Thread safety.** One decompile call runs at a time in the process; others wait.

**Samples.** [samples/HelloDecompile/main.cpp](../../../../samples/HelloDecompile/main.cpp)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

