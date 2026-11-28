[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_imports

`binary.h` · function · since 0.1

Every function the binary imports, module by module, in the order the file lists them.

```c
const cb_import* cb_binary_imports(const cb_binary* binary, size_t* count);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `count` | receives how many there are. |

**Returns.** the first of count records the binary owns; NULL when there are none.

**Samples.** [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

