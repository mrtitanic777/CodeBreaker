R"BOOTDOC(boot.py -- does Windows 95 start with these drivers? A rebuilt driver's real test.

    python -m codebreaker vxd boot [--drivers NAME,...] [--vmm32] [--retail] [--bisect]
                                   [--disk IMAGE] [--wait SECONDS] [--tag NAME]

Each driver named (vtd, vpicd, drvspacx ...: a file of the corpus) is built from the recovered
disassembly with le_writer.build_le, and with --vmm32 Windows' own VMM32.VXD with build_w3; they
are put into a copy of an installed Windows 95 disk, and the copy is booted in QEMU with no
window. Windows reaching its desktop (or a logon box over it) is a pass; a blue screen, a screen
that stays black or in text (a "Windows protection error" is one), or no desktop in --wait
seconds (default 300) is a fail. --retail puts the corpus originals in instead of the rebuilt
files: the baseline a rebuilt run is measured against. --bisect, when the run fails, boots
halves of the set until it finds the drivers that fail by themselves.

Where each file goes: a driver kept in WINDOWS\SYSTEM of the corpus goes into
WINDOWS\SYSTEM\VMM32 -- Windows loads a driver found there in place of the copy inside VMM32.VXD
-- and one kept in a folder below it (IOSUBSYS) or with another extension (.386) where the corpus
has it. VMM32.VXD goes into WINDOWS\SYSTEM. The corpus VMM32.VXD holds VMM alone, where the
installed one (W4, compressed) holds 43 drivers; so with --vmm32 every other driver of the
installed bundle the disk has no file of in VMM32 is put there from the corpus too (Windows takes
a driver it does not find in the bundle from there).

The disk is never changed: `--disk`, or the setting "win95_installed_hd" (a raw image of a hard
disk Windows 95 is installed on; default the one the Phase 3 install left in WSL), is copied into
a run folder of its own (base/runs.py), which is deleted when the run ends -- and a killed run's,
by the next run, once it is a day old. BootLog=1 is set in the copy's MSDOS.SYS; after the run the
copy's BOOTLOG.TXT says which of the files Windows loaded, and it is kept with the last screen,
one of each per tag (state/vxdboot/shots/<tag>.png and <tag>.bootlog.txt; the tag is "rebuilt"
or "retail" unless --tag names another, and a run overwrites the last of its tag). That
image has FIX95CPU's drivers in VMM32 already: without them a fast host fails at IOS, whatever
the drivers (the classic timing-loop fault of Windows 95), and those files are left alone.

QEMU runs at below-normal priority, one machine at a time; its process number is printed.
)BOOTDOC"