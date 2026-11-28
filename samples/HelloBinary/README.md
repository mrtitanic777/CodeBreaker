# HelloBinary

Opens a file with `cb_binary_open` and prints what it is: `cb_binary_format`, `cb_binary_arch`, the sections, imports and exports (records
that point into the binary, valid until `cb_binary_close`), and the functions the file names. Plain C.

```
HelloBinary lz32.dll
HelloBinary lzexpand.dll        a 16-bit module: segment:offset addresses
```
