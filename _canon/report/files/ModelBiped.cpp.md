# ModelBiped.cpp

Risk score: **2**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `rule-of-three-dtor`): Rule-of-Three: synthesized destructor frees 7 heap-owning field(s): bipedBody, bipedHead, bipedHeadwear, bipedLeftArm, bipedLeftLeg, bipedRightArm, bipedRightLeg

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ModelBiped

## RULE — an automatic fix rule fired

- **⚪ LOW** `L13` (emit, `ctor-delegation-inlined`): ctor delegation this(0.0F) inlined from the 1-arg constructor (registration preserved)
- **⚪ LOW** `L17` (emit, `ctor-delegation-inlined`): ctor delegation this(var1, 0.0F) inlined from the 2-arg constructor (registration preserved)
