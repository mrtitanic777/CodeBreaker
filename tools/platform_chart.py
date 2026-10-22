"""The any-platform flowchart (docs/PLATFORMS.md): docs/platforms.svg, and docs/platforms.png at
four times its size, drawn by headless Chrome.

    python tools/platform_chart.py
"""
import os, subprocess, tempfile

OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "docs")
DX, DY = 40, 112          # content offset inside the canvas
W = 680
GAP = 40

RAMP = {  # fill, stroke, title, subtitle
    "gray":   ("#F1EFE8", "#888780", "#2C2C2A", "#5F5E5A"),
    "purple": ("#EEEDFE", "#7F77DD", "#26215C", "#534AB7"),
    "teal":   ("#E1F5EE", "#1D9E75", "#04342C", "#0F6E56"),
}
ARROW = "#8A8880"
parts = []


def box(x, y, w, h, title, subs, ramp, dashed=False, small=False):
    f, s, t, st = RAMP[ramp]
    x, y = x + DX, y + DY
    dash = ' stroke-dasharray="6 4"' if dashed else ""
    parts.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="9" fill="{f}" stroke="{s}" stroke-width="1.2"{dash}/>')
    block = 20 + 18 * len(subs)                      # title line + subtitle lines
    ty = y + (h - block) / 2 + 10
    parts.append(f'<text x="{x + w / 2}" y="{ty}" class="{"t2" if small else "t"}" fill="{t}">{title}</text>')
    for i, sub in enumerate(subs):
        parts.append(f'<text x="{x + w / 2}" y="{ty + 21 + 18 * i}" class="s" fill="{st}">{sub}</text>')


def line(x1, y1, x2, y2, head=True):
    m = ' marker-end="url(#a)"' if head else ""
    parts.append(f'<line x1="{x1 + DX}" y1="{y1 + DY}" x2="{x2 + DX}" y2="{y2 + DY}" stroke="{ARROW}" stroke-width="1.4"{m}/>')


def down(xs, y0, y1):
    for x in xs:
        line(x, y0, x, y1 - 2)


def bus(src, xs, y0, y1):
    """from src at y0, down to a rail midway, then down to each x at y1"""
    mid = (y0 + y1) / 2
    line(src, y0, src, mid, head=False)
    line(min(xs + [src]), mid, max(xs + [src]), mid, head=False)
    down(xs, mid, y1)


def cols(n, gap):
    w = (600 - gap * (n - 1)) / n
    return [i * (w + gap) for i in range(n)], w


y = 0
# 1. anything that runs
ci, wi = cols(5, 8)
midi = [x + wi / 2 for x in ci]
for x, t, subs in zip(ci, ["Game consoles", "Arcade games", "8-bit computers", "PC software", "Mobile, devices"],
                      [["Atari to PS5", "Xbox, handhelds"],
                       ["Boards and", "every ROM set"],
                       ["C64, Apple II", "Spectrum, MSX"],
                       ["DOS, Windows", "Mac, Linux"],
                       ["Android, iOS", "J2ME, firmware"]]):
    box(x, y, wi, 80, t, subs, "gray", small=True)
down(midi, y + 80, y + 80 + GAP)
y += 80 + GAP

box(0, y, 600, 64, "Unpack and identify", ["Tapes, disks, cartridge and arcade ROMs, discs, ELF, PE, APK ... which CPU, OS"], "purple")
c5, w5 = cols(5, 10)
mid5 = [x + w5 / 2 for x in c5]
bus(300, mid5, y + 64, y + 64 + GAP)
y += 64 + GAP

# 2. one front end per instruction set
for x, t, subs in zip(c5, ["8/16-bit CPUs","MIPS, SuperH", "PowerPC, Cell", "x86, ARM", "Bytecode"],
                      [["6502, 65816", "Z80, 68000"], ["PS1, PS2, N64", "Saturn, Dreamcast"],
                       ["GameCube, Wii", "Xbox 360, PS3"], ["PC, Xbox, PS4/5", "phones, Switch"],
                       ["Java, Dalvik", ".NET, scripts"]]):
    box(x, y, w5, 80, t, subs, "purple", small=True)
for x in mid5:                                      # all into one shared form
    line(x, y + 80, x, y + 80 + 14, head=False)
line(mid5[0], y + 80 + 14, mid5[-1], y + 80 + 14, head=False)
line(300, y + 80 + 14, 300, y + 80 + GAP - 2)
y += 80 + GAP

box(0, y, 600, 64, "Lift to one shared form", ["Every CPU becomes the same intermediate code (Ghidra-based core)"], "purple")
down([300], y + 64, y + 64 + GAP)
y += 64 + GAP

box(0, y, 600, 64, "Analyze and name", ["Functions, types, structures, SDK and library calls, hardware registers"], "purple")
c4, w4 = cols(4, 13.33)
mid4 = [x + w4 / 2 for x in c4]
down(mid4, y + 64, y + 64 + GAP)
y += 64 + GAP

# 3. back to source
for x, t, subs in zip(c4, ["Decompile", "Compiler models", "Assembly source", "Bytecode to source"],
                      [["To C and C++"], ["Read code backwards"], ["Hand-written parts"], ["Java, C#, scripts"]]):
    box(x, y, w4, 64, t, subs, "purple", small=True)
down(mid4, y + 64, y + 64 + GAP)
y += 64 + GAP

box(0, y, 600, 64, "Rebuild", ["With the original's own compilers and SDKs, or modern ones"], "purple")
JX = 215                                             # judge box starts here
line(300 + 105, y + 64, 300 + 105, y + 64 + GAP - 2)
y += 64 + GAP

# 4. judged by behaviour, on a model of the platform
box(0, y, 190, 64, "Platform models", ["CPU, hardware, OS"], "teal", small=True)
box(JX, y, 600 - JX, 64, "Judge by behaviour", ["Run beside the original: same calls, output, memory"], "teal")
line(190, y + 32, JX - 2, y + 32)
judge_y = y
line(250, y + 64, 250, y + 64 + GAP - 2)
line(455, y + 64, 455, y + 64 + GAP - 2)
y += 64 + GAP

box(0, y, 290, 64, "Refine", ["Repair, match, readable names"], "purple")
box(310, y, 290, 64, "Record verdict", ["Function store, scoreboard, coverage"], "teal")
rebuild_y = judge_y - 64 - GAP
parts.append(f'<path d="M{DX} {DY + y + 32} L{DX - 22} {DY + y + 32} L{DX - 22} {DY + rebuild_y + 32} L{DX - 3} {DY + rebuild_y + 32}" '
             f'fill="none" stroke="#7F77DD" stroke-width="1.4" stroke-dasharray="5 4" marker-end="url(#ap)"/>')
bus(455, mid4, y + 64, y + 64 + GAP)
y += 64 + GAP

# 5. what comes out
for x, t, subs in zip(c4, ["Source tree", "Rebuilt originals", "Ports", "Workbench"],
                      [["Readable, compilable"], ["Byte-close builds"], ["Any platform to any"], ["Web, desktop, Live"]]):
    box(x, y, w4, 64, t, subs, "gray", small=True)
y += 64 + GAP

box(0, y, 600, 64, "Self-checks, for every platform", ["Golden tests, judge-the-judge, open-source and homebrew benchmarks"], "teal", dashed=True)
y += 64 + 36

# legend
ly = DY + y
for (ramp, label), lx in zip([("gray", "In and out"), ("purple", "Reconstruction"), ("teal", "Verification")], (0, 120, 265)):
    f, s, _t, _st = RAMP[ramp]
    parts.append(f'<rect x="{DX + lx}" y="{ly}" width="16" height="16" rx="4" fill="{f}" stroke="{s}" stroke-width="1.2"/>')
    parts.append(f'<text x="{DX + lx + 26}" y="{ly + 12.5}" class="l">{label}</text>')
parts.append(f'<path d="M{DX + 400} {ly + 8} L{DX + 428} {ly + 8}" stroke="#7F77DD" stroke-width="1.4" stroke-dasharray="5 4" marker-end="url(#ap)"/>')
parts.append(f'<text x="{DX + 438}" y="{ly + 12.5}" class="l">Loops back to rebuild</text>')
H = ly + 16 + 40

svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">
<defs>
<marker id="a" viewBox="0 0 10 10" refX="8" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M2 1L8 5L2 9" fill="none" stroke="{ARROW}" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round"/></marker>
<marker id="ap" viewBox="0 0 10 10" refX="8" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M2 1L8 5L2 9" fill="none" stroke="#7F77DD" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round"/></marker>
<style>
text {{ font-family: "Segoe UI Variable Text", "Segoe UI", sans-serif; }}
.t {{ font-size: 15px; font-weight: 600; text-anchor: middle; dominant-baseline: central; }}
.t2 {{ font-size: 14px; font-weight: 600; text-anchor: middle; dominant-baseline: central; }}
.s {{ font-size: 12.5px; text-anchor: middle; dominant-baseline: central; }}
.l {{ font-size: 13px; fill: #5F5E5A; }}
.h1 {{ font-size: 24px; font-weight: 600; fill: #1F1E1D; }}
.h2 {{ font-size: 14px; fill: #6B6A65; }}
</style>
</defs>
<rect width="{W}" height="{H}" fill="#FFFFFF"/>
<text x="{DX}" y="50" class="h1">CodeBreaker: any software, any platform</text>
<text x="{DX}" y="78" class="h2">From 8-bit to today, all under one umbrella: back to source that behaves the same</text>
{chr(10).join(parts)}
</svg>'''

svg_path = os.path.join(OUT, "platforms.svg")
open(svg_path, "w", encoding="utf-8").write(svg)
html = os.path.join(tempfile.gettempdir(), "platforms_chart.html")
open(html, "w", encoding="utf-8").write(
    f'<!doctype html><html><body style="margin:0;background:#fff">{svg}</body></html>')
png = os.path.join(OUT, "platforms.png")
chrome = r"C:\Program Files\Google\Chrome\Application\chrome.exe"
subprocess.run([chrome, "--headless=new", "--disable-gpu", "--hide-scrollbars", "--force-device-scale-factor=4",
                f"--window-size={W},{int(H)}", f"--screenshot={png}", "file:///" + html.replace("\\", "/")],
               check=True, capture_output=True)
print(png, os.path.getsize(png), W * 4, int(H) * 4)
