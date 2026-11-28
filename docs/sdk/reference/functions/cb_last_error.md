[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_last_error

`cb.h` · function · since 0.1

What the last call on this thread that failed said: what failed, on which input, and what to do.

```c
const char* cb_last_error(void);
```

A call that succeeds does not change it.

**Returns.** a string the library owns, valid until the next call on this thread that fails; "" when no call has failed.

**Thread safety.** Each thread has its own.

**Samples.** [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

