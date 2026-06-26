# EntityPlayerSP.cpp

Risk score: **6**  ·  4 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L169` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L173` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L51` (marker, `PORT`): Session::username is a std::string value (converter wrongly used m_inventory).
  ```cpp
  // PORT: Session::username is a std::string value (converter wrongly used m_inventory).
  ```
