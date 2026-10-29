# Virtual device drivers (VxDs)

`python -m codebreaker vxd <tool>` holds the tools for Windows 95's virtual device drivers:
the LE files in `WINDOWS\SYSTEM` (and `IOSUBSYS`) and the W3 bundle `VMM32.VXD`
(`codebreaker/source/vxd/`; `python -m codebreaker vxd` lists them). The source side writes a
driver in the driver kit's macro form (`idiom`, `tree`), assembles and links it (`masm`, `link`,
`from-source`, `owbuild`); `le_writer.build_le` / `build_w3` write a loadable driver straight from
the recovered disassembly, through `formats/le.py`'s `write_image`.

## Does Windows start with it? `vxd boot`

The test a rebuilt driver has to pass in the end is the machine: Windows 95 loading it and
reaching its desktop.

```
python -m codebreaker vxd boot --drivers vtd,vpicd --vmm32       rebuilt: build_le / build_w3
python -m codebreaker vxd boot --drivers vtd,vpicd --vmm32 --retail   the corpus originals (baseline)
python -m codebreaker vxd boot ... --bisect                      on a failure, find the drivers that fail
python -m codebreaker vxd boot                                   the disk as it is (BootLog on)
    [--disk IMAGE] [--wait SECONDS (300)] [--tag NAME]
```

What a run does:

1. Builds each named driver (a corpus file name, extension optional) with `build_le`, and with
   `--vmm32` the corpus `VMM32.VXD` (VMM alone, a W3) with `build_w3`. `--retail` takes the corpus
   files as they are.
2. Copies the installed disk into a run folder of its own (`state/vxdboot/runs/<tag>_*`, base/runs.py;
   a killed run's is swept by the next run once a day old) and writes
   the files into the copy with `formats/media.put` (FAT16/FAT32 in Python: no WSL, no mtools):
   - a driver the corpus keeps in `WINDOWS\SYSTEM` goes to `WINDOWS\SYSTEM\VMM32\NAME.VXD` --
     Windows loads a driver found there in place of the copy inside the installed `VMM32.VXD`;
     one kept elsewhere (`IOSUBSYS\DRVSPACX.VXD`, `VDHCP.386`) goes where the corpus has it;
   - `VMM32.VXD` goes to `WINDOWS\SYSTEM`. The installed one is a W4 holding 43 drivers, the
     corpus one VMM alone, so every other driver of the installed bundle that the disk has no
     file of in `VMM32\` is put there too (the corpus original): Windows takes a bundled driver it
     does not find in `VMM32.VXD` from there;
   - `MSDOS.SYS` gets `BootLog=1` (so `C:\BOOTLOG.TXT` says what loaded) and `AutoScan=0` (every
     run stops the machine uncleanly; without it ScanDisk runs first).
3. Boots the copy in QEMU (setting `qemu`), no window, below-normal priority, one machine; the
   process number is printed. The screen is read every 4 seconds through QEMU's monitor:
   - **PASS**: the desktop -- mostly Windows' teal (87,168,168 on QEMU's VGA), whatever box is on
     it, or a taskbar along the bottom -- on three screens running;
   - **FAIL(blue screen)**: a blue text screen for 30 seconds (a fatal exception);
   - **FAIL(text screen)** / **FAIL(black screen)**: back in text (a "Windows protection error")
     or dark for a minute after Windows' own screens; **FAIL(timeout)**, **FAIL(qemu exited)**.
4. Reads `BOOTLOG.TXT` from the copy and prints, for each driver named, what the log says of it
   (`LoadSuccess SYSCRITINITSUCCESS DEVICEINITSUCCESS INITCOMPLETESUCCESS`, or the phase it
   stopped in, or that it was never loaded), then deletes the run folder. The last screen and
   the log are kept: `state/vxdboot/shots/<tag>.png`, `<tag>.bootlog.txt` -- one of each per tag
   (`rebuilt` or `retail` unless `--tag` names another), the run before's overwritten.

`--bisect` boots each half of a failing set, and the halves of a half that fails, down to the
single drivers that fail by themselves (or the group that fails only together).

**The disk.** Setting `win95_installed_hd` (or `--disk`): a raw image of a hard disk with
Windows 95 installed, read only. The default is the one the Phase 3 install left in WSL,
`\\wsl$\Ubuntu\home\mrtit\win95_install.img` (1 GB, FAT16 at sector 63; WSL is started when it
is asleep). That install has FIX95CPU's drivers in `WINDOWS\SYSTEM\VMM32` (IOS and the others):
without them a fast host stops at "While initializing device IOS: Windows protection error",
whatever drivers are put in -- the classic timing-loop fault -- so a run never replaces those.
Each run needs the image's size free on the state drive, plus `min_free_gb`.

**What is not loaded** on that machine is not tested by booting it: `DRVSPACX` (no DriveSpace
volume) and `VDHCP` (no network) are written and the run passes, but BOOTLOG.TXT never names
them; the report says so.

### Results, 2026-10-28

The twelve drivers whose bytes changed when `write_image` / `build_le` / `build_w3` began storing
pages at their page-map number (drvspacx dosmgr ifsmgr shell vdhcp vdmad vfat vmpoll vpicd vtd
vtdapi vxdldr) and VMM32.VXD:

| run | what was put in | verdict |
|---|---|---|
| `vxd boot` | nothing (BootLog, AutoScan only) | PASS |
| `--retail --vmm32 --drivers <the 12>` | the originals, VMM32 (VMM alone) + 31 bundle drivers as files | PASS, desktop at ~100 s |
| `--vmm32 --bisect --drivers <the 12>` | all 13 rebuilt (+ the 31 originals) | PASS, desktop at ~40-50 s; no bisect needed |
| a VPICD with its pages filled with `int3` (a check that a broken driver fails) | | FAIL: "built with an old beta version of Windows DDK ... Press a key" |

In the rebuilt run BOOTLOG.TXT shows 42 drivers loaded from `VMM32\` (so the rebuilt VMM-only
VMM32.VXD was the one running) and each rebuilt driver `LoadSuccess` through
`INITCOMPLETESUCCESS` -- except DRVSPACX and VDHCP, never loaded on that machine. The same two
lines fail in every run, untouched disk included: `LoadFailed = vshare` and
`INITCOMPLETEFAILED = SDVXD`.
