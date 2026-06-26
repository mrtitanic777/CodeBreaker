# PathPoint.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L52` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool PathPoint::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L65` (marker, `PORT`): header returns const std::string& (auto-gen convention). Cache in a
  ```cpp
  // PORT: header returns const std::string& (auto-gen convention). Cache in a
  ```
