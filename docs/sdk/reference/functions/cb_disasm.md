[The reference](../README.md) · [disasm.h](../disasm_h.md)

# cb_disasm

`disasm.h` · function · since 0.1

Read instructions from bytes.

```c
cb_status cb_disasm(const char* arch, const void* code, size_t size, uint64_t address, size_t count, cb_instructions** out);
```

| Parameter | |
|---|---|
| `arch` | a processor's name or alias (cb_arch_list). |
| `code` | the bytes; may be NULL when size is 0. |
| `size` | how many bytes there are. |
| `address` | the address of the first byte. |
| `count` | how many instructions to read at most; 0: until the bytes end. |
| `out` | receives the instructions; free them with cb_instructions_free. Left NULL when the call fails. |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND for a name no processor has; CB_ERROR_UNSUPPORTED for a processor with no description; CB_ERROR_IO when its description cannot be read (the data root is not complete); CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the instructions; they do not point into code, which may be freed at once.

**Thread safety.** Any thread; calls for the same processor from several threads run side by side.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

