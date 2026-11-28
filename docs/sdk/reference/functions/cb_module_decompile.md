[The reference](../README.md) · [decompile.h](../decompile_h.md)

# cb_module_decompile

`decompile.h` · function · since 0.1

Every function of a binary as the text of one .c file.

```c
cb_status cb_module_decompile(cb_binary* binary, const cb_decompile_options* options, cb_string* out, cb_module_info* info);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `options` | NULL for the defaults. |
| `out` | receives the C. Free it with cb_string_free. |
| `info` | NULL, or receives how many functions were found and decompiled. |

**Returns.** CB_OK; CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c"; CB_ERROR_NOT_FOUND when the decompiler cannot run (cb_decompiler_check); CB_ERROR_TOOL when the native decompiler failed; CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the string. The binary keeps its own copy for cb_function_decompile.

**Thread safety.** One decompile call runs at a time in the process; others wait.

**Samples.** [samples/HelloDecompile/main.cpp](../../../../samples/HelloDecompile/main.cpp)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

