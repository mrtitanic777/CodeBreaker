[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_init

`cb.h` · function · since 0.1

Set the library up. Call it once before any other call that does work; each cb_init is matched by a cb_shutdown.

```c
cb_status cb_init(const cb_init_options* options);
```

The first call decides the data root and the state folder. For this process it sets CB_CHECKOUT_ROOT, CB_STATE and, when the data root
has them, CB_LANGUAGES_DIR, CB_DECOMPILER_BIN and CB_THIRD_PARTY_BIN: the settings the engines read (a value already set is kept).

| Parameter | |
|---|---|
| `options` | NULL for the defaults. |

**Returns.** CB_OK; CB_ERROR_INVALID_ARGUMENT for a struct_size this library does not know, or a later call naming another data root or state folder than the first; CB_ERROR_NOT_FOUND when the data root has no config/ folder; CB_ERROR_IO when the state folder cannot be made.

**Thread safety.** Any thread; calls are serialized.

**Samples.** [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

