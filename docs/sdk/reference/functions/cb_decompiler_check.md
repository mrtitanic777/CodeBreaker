[The reference](../README.md) · [decompile.h](../decompile_h.md)

# cb_decompiler_check

`decompile.h` · function · since 0.1

Whether the decompiler can run: its program and its processor descriptions are where the data root says.

```c
cb_status cb_decompiler_check(void);
```

**Returns.** CB_OK; CB_ERROR_NOT_FOUND naming what is missing; CB_ERROR_NOT_INITIALIZED before cb_init.

**Thread safety.** Any thread.

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

