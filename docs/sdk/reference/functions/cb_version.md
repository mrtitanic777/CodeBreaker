[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_version

`cb.h` · function · since 0.1

The version of the library that is loaded, as CB_SDK_MAKE_VERSION makes it.

```c
uint32_t cb_version(void);
```

Compare it with CB_SDK_VERSION (the headers') to find a program running on another library than it was built with: the same major
version and a minor version at least the headers' is compatible.

**Returns.** the library's version number.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

