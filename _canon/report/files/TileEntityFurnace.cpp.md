# TileEntityFurnace.cpp

Risk score: **4**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `L65` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for TileEntityFurnace

## RULE — an automatic fix rule fired

- **⚪ LOW** `—` (emit, `mi-forward`): MI-diamond: synthesized onInventoryChanged() forwarding to TileEntity:: (interface decl + base impl unified into one vtable override)
