[The reference](../README.md) · [disasm.h](../disasm_h.md)

# cb_arch_find

`disasm.h` · function · since 0.1

The processor a name or one of its aliases names, compared without case ("I386" is "x86-32").

```c
cb_status cb_arch_find(const char* name, const cb_arch** out);
```

| Parameter | |
|---|---|
| `name` | the name. |
| `out` | receives the processor's record (owned by the library). |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when no processor has that name (the message lists the names); CB_ERROR_INVALID_ARGUMENT for a NULL argument.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

