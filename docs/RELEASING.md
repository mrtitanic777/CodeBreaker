# Making a release

Versions are `MAJOR.MINOR.PATCH`: a patch fixes, a minor release adds commands or lanes, a major release changes
what existing commands do or what they write.

## 1. Bring the version up to date

- `CHANGELOG.md`: rename **Unreleased** to the new version and date, and start a new empty **Unreleased** above it.
- `src/app/codebreaker.rc`: `FILEVERSION`, `PRODUCTVERSION` and the two version strings (the version of `codebreaker.exe`).
- The MCP server's version: `src/ui/mcp_symbols.cpp` (`services.version`), and the goldens that record it
  (`src/ui/testdata/mcp_python.golden.json`, `tests/exe_gate/golden/mcp-session.txt`) and the two tests
  that check it (`src/ui/mcp_server_test.cpp`, `src/ui/mcp_services_test.cpp`).
- README.md: the version note under the first screenshot.

Commit these as one change.

## 2. Prove it

```bash
tools\build_codebreaker_exe.cmd
```

```bash
tools\gate.cmd
```

The gate must end with every suite passing. The milestone regression boots Windows 95 from the rebuilt modules; it
tests only the Windows 95 build path (src/build, decompile, emulate/windows16*, verify/gate16, config/build and
the OpenWatcom toolchain). Run only what covers code that changed since its last proof: nothing, if that path is
unchanged since the last release or since its last proven run; `all15 --runs 1` for small changes there that were not
proven yet; the full set below (about three hours) when a change to that path has not been proven at all:

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

This makes `build\release\CodeBreaker-X.Y.Z-win64.zip` from the committed source (no tests: `.gitattributes`
export-ignore) and the built program and engines, and checks that the packaged program starts. Then:

```bash
git tag -a vX.Y.Z -m "CodeBreaker X.Y.Z"
```

```bash
git push origin vX.Y.Z
```

```bash
gh release create vX.Y.Z build\release\CodeBreaker-X.Y.Z-win64.zip --title "CodeBreaker X.Y.Z" --notes-file NOTES.md --latest
```

`NOTES.md` is the version's CHANGELOG section, with a Download line. Delete `build\release` afterwards.
