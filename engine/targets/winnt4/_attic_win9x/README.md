# _attic_win9x — Win9x-only tools, inert on NT 4.0

These tools were ported with the rest of the CodeBreaker scaffold from the
win95/win98 projects, but they encode **Win9x concepts that do not exist on
Windows NT 4.0** and are parked here so they don't clutter the NT4 command
surface. Nothing in the live toolset imports any of them.

| tool | what it did on 9x | why inert on NT 4.0 |
|---|---|---|
| `cab.py` | read the Win98 spanning LZX cabinet set | NT4 installs from an `I386\` tree of `expand`-compressed files, not a spanning-CAB chain |
| `cabwrite.py` | write an MSCF (MSZIP) cabinet | — (only used by the 9x disc builder) |
| `vmmsvc.py` | extract VxD/VMM service tables from the DDK | NT4 has no VMM and no VxDs |
| `disc_build.py` | assemble a self-hosted Win98 install disc | NT4 install/boot is `NTLDR` + `TXTSETUP.SIF`, a different mechanism |
| `bootchain.py` | Win98 real-mode boot-chain readiness | NT4 does not use the IO.SYS boot chain |
| `isowrite.py` | write an ISO9660 image (part of the 9x disc build) | not needed for the NT4 flow |
| `w3.py` | reconstruct a W3 container (VMM32.VXD) | NT4 has no W3/VMM32 container |

To resurrect one, move it back up to `tools/` and re-add its `BUILTINS` entry in
`winnt4re.py`. See [`../WIN9X_INERT.md`](../WIN9X_INERT.md) for the full inert list
(including tools left in place because they are lazy-imported by survivors).
