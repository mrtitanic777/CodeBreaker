[The reference](../README.md) · [binary.h](../binary_h.md)

# cb_binary_format

`binary.h` · function · since 0.1

What the file is: "PE", "NE", "LE", "LX", "W3", "MZ" for an executable, else the format line of what it was

```c
const char* cb_binary_format(const cb_binary* binary);
```

identified as ("NES ROM", "Xbox 360 XEX" ...).

**Returns.** a string the binary owns; "" when the file is not one CodeBreaker knows, or for a NULL binary.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../../src/sdk/sdk_test.cpp)

