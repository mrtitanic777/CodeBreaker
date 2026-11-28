[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_version_string

`cb.h` · function · since 0.1

The version of the library that is loaded, as text ("0.1.0").

```c
const char* cb_version_string(void);
```

**Returns.** a string the library owns, valid while it is loaded.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

