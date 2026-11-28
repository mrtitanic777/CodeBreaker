[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_shutdown

`cb.h` · function · since 0.1

Undo one cb_init. The last one frees what the library made (the decompiler and its caches).

```c
void cb_shutdown(void);
```

Close every handle before the last cb_shutdown; a handle used after it is undefined behaviour. A cb_shutdown with no cb_init does nothing.

**Thread safety.** Any thread, when no other call is running.

**Samples.** [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

