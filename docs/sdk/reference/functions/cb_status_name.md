[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_status_name

`cb.h` · function · since 0.1

The name of a status ("CB_ERROR_NOT_FOUND"), for logs.

```c
const char* cb_status_name(cb_status status);
```

| Parameter | |
|---|---|
| `status` | any value; one that is not a cb_status gives "CB_ERROR_UNKNOWN". |

**Returns.** a string the library owns, valid while it is loaded.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/HelloDecompile/main.cpp](../../../../samples/HelloDecompile/main.cpp), [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

