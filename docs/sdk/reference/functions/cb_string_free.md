[The reference](../README.md) · [cb.h](../cb_h.md)

# cb_string_free

`cb.h` · function · since 0.1

Free a cb_string the library made, and set it to {NULL, 0}.

```c
void cb_string_free(cb_string* string);
```

| Parameter | |
|---|---|
| `string` | NULL, or a cb_string a call filled (freeing one twice is safe: the first sets it empty). |

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

