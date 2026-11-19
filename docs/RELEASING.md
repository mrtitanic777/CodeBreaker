# Making a release

Versions are `MAJOR.MINOR.PATCH`: a patch fixes, a minor release adds commands or lanes, a major release changes
what existing commands do or what they write.

## 1. Bring the version up to date

- `CHANGELOG.md`: rename **Unreleased** to the new version and date, and start a new empty **Unreleased** above it.
- `native/cb/app/codebreaker.rc`: `FILEVERSION`, `PRODUCTVERSION` and the two version strings (the version of `codebreaker.exe`).
- The MCP server's version: `native/cb/ui/mcp_symbols.cpp` (`services.version`), and the goldens that record it
  (`native/cb/ui/testdata/mcp_python.golden.json`, `tests/exe_gate/golden/mcp-session.txt`) and the two tests
  that check it (`native/cb/ui/mcp_server_test.cpp`, `native/cb/ui/mcp_services_test.cpp`).
- README.md: the version note under the first screenshot.

Commit these as one change.

## 2. Prove it

```bash
tools\build_codebreaker_exe.cmd
```

```bash
tools\gate.cmd
```

The gate must end with every suite passing. For a minor or major release, also run the milestone regression
(about three hours of machine time):

```bash
codebreaker regress all15 --runs 2
```

```bash
codebreaker regress setup --runs 2
```

```bash
codebreaker regress install --runs 2
```

```bash
codebreaker regress slow
```

Push only after the gate has passed, as its own step.

## 3. Package and publish

```bash
tools\package_release.cmd X.Y.Z
```

This makes `state\release\CodeBreaker-X.Y.Z-win64.zip` from the committed source (no tests: `.gitattributes`
export-ignore) and the built program and engines, and checks that the packaged program starts. Then:

```bash
git tag -a vX.Y.Z -m "CodeBreaker X.Y.Z"
```

```bash
git push origin vX.Y.Z
```

```bash
gh release create vX.Y.Z state\release\CodeBreaker-X.Y.Z-win64.zip --title "CodeBreaker X.Y.Z" --notes-file NOTES.md --latest
```

`NOTES.md` is the version's CHANGELOG section, with a Download line. Delete `state\release` afterwards.
