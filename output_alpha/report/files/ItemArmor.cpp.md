# ItemArmor.cpp

Risk score: **0**  ·  2 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L9` (marker, `PORT`): int array init {3, 8, 6, 3} */;
  ```cpp
  int* ItemArmor::s_damageReduceAmmountArray = NULL /* PORT: int array init {3, 8, 6, 3} */;
  ```
- **⚪ LOW** `L10` (marker, `PORT`): int array init {11, 16, 15, 13} */;
  ```cpp
  int* ItemArmor::s_maxDamageArray = NULL /* PORT: int array init {11, 16, 15, 13} */;
  ```
