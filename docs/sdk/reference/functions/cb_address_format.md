[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_address_format

`binary.h` · function · since 0.1

An address as text, into a caller-owned buffer: "0x10001000", "3:01a4", "1234:0010", "1:00001000", "" for none.

```c
cb_status cb_address_format(const cb_address* address, char* buffer, size_t size, size_t* needed);
```

| Parameter | |
|---|---|
| `address` | the address. |
| `buffer` | where the text goes, NUL-terminated; may be NULL when size is 0. |
| `size` | the buffer's size in bytes. |
| `needed` | NULL, or receives the bytes the text needs with its NUL (also when the buffer is too small). |

**Returns.** CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL address; CB_ERROR_BUFFER_TOO_SMALL when the text and its NUL do not fit (24 bytes always do).

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/HelloDecompile/main.cpp](../../../../samples/HelloDecompile/main.cpp), [samples/framework/framework.c](../../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

