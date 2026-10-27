# CodeBreaker as an MCP server

`python -m codebreaker mcp` (codebreaker/ui/mcp_server.py) is CodeBreaker as an MCP server, so an
assistant such as Claude Code can take binaries apart with CodeBreaker's own tools: what a file
is, its headers, its code instruction by instruction, what refers to what, the reconstructed C and
its verdict, and two builds side by side. 32 tools, one resource and one prompt. Tests:
tests/test_mcp.py.

## Connecting

**Claude Code on this machine (the usual way).** Nothing to sign in to, no key, no internet: Claude
Code starts the server itself, over stdio, one copy per session.

- In this checkout, `.mcp.json` registers it; approve the `codebreaker` server once.
- Everywhere else, register it for the user (it is in `~/.claude.json` already on Logan's machine):

  ```
  claude mcp add --scope user codebreaker -- C:/Python314/python.exe -m codebreaker mcp
  ```

  This works from any folder because the `codebreaker` package is installed from this checkout;
  it finds the corpus, Converted/ and state/ from there.

A session keeps the server it started with: after changing mcp_server.py, start a new session (or
reconnect the server) to get the new code.

Claude Code's Connectors settings are not needed for this. They are for servers on the internet,
for claude.ai on the web and the phone.

**Over HTTP.** `python -m codebreaker mcp --http PORT` serves the same tools over streamable HTTP at
`http://127.0.0.1:PORT/mcp`. Every request must carry `Authorization: Bearer <token>`; the token
is CB_MCP_TOKEN, or one made once and kept in `state/mcp/http_token.txt` (delete the file for a new
one). `--host H` listens somewhere other than 127.0.0.1; `--allow-host NAME` answers to another
Host name, such as a tunnel's.

**On the internet (claude.ai).** Add `--public`: the server then offers the reading tools only (no
run_command, stop_job or name_function, so nothing from outside can start a process or change the
store), and also takes the token as the URL's first segment, `/<token>/mcp`, for clients that
cannot send a header. It still needs a public HTTPS address in front of it, from a tunnel, and the
tunnel's name passed with `--allow-host`:

```
python -m codebreaker mcp --http 8765 --public --allow-host <name>.trycloudflare.com
cloudflared tunnel --url http://127.0.0.1:8765
```

In claude.ai: Settings, Connectors, Add custom connector; the URL `https://<name>/mcp`, "No
sign-in", and a request header `Authorization: Bearer <token>`. A free quick tunnel (localhost.run,
trycloudflare) changes its name when it reconnects, and the connector then stops working; for a
connector that lasts, use a named Cloudflare tunnel run as a service. Whether to put it on the
internet at all is the owner's call. None is set up now.

## Several at once

Every session runs its own server. The function store (names and verdicts) is one SQLite database
in WAL mode, so a name given in one session is seen by all. What a server keeps in memory is its
own: its jobs (another session's `jobs` does not list them, though their output files are on disk)
and its caches (decompiles, comparisons). The machine's limits (four gate workers, three VMs) are
not enforced across sessions.

## Addresses

Wherever a tool takes `addr` it takes any of:

- an address in hex, with or without 0x (`bfea1198`, `0x142a`)
- for a PE, an RVA (the image base is added when the number is below it)
- for a 16-bit module, `seg:off`
- a name: an export, a name the analyses or debug symbols gave, or one given with name_function in
  any module holding the same body
- for xrefs, an import: `KERNEL32!CommonUnimpStub`, `CommonUnimpStub`, or `KERNEL32!#17`

Addresses come back as hex text; file offsets and sizes as numbers. Imports by ordinal carry their
name when it is known (`KERNEL32!CommonUnimpStub (#17)`), and say where the name came from. The
documented-only KERNEL32 ordinals (17 CommonUnimpStub, 20/21 the DOS and Win32 handle conversions,
22 DisposeLZ32Handle, 41 ThunkInitLSF, 89 FT_PrologPrime, 90 QT_ThunkPrime, ...) live in
`symbols/ordinals.documented()`, kept apart from `ordinals.name()` so the decompiler's output and
its cache keys do not change.

A module is a corpus name (`lz32.dll`) or a path.

## The tools

### Start here

| Tool | What it gives |
| --- | --- |
| `triage(module)` | A module at a glance: what it is, exports and imports, what made it (compiler, linker, functions a tool or runtime library made), the store's counts, each unverified function with its verdict's detail and source, where its authored source lives, and what to look at next. |
| `describe(path)` | What a file is, its platform, and the lane that reconstructs it. |
| `modules(kind?, find?, only_in_store?, offset?, limit?, refresh?)` | The corpus, with each module's ledger numbers. |
| `coverage(refresh?)` | How the corpus stands: totals and one row per kind of module. |
| `progress()` | What a running sweep is doing: rate, recent modules, workers, a rough ETA. |

The coverage ledger behind coverage, modules and progress takes minutes when cold. It is worked out
in the background and kept in `state/mcp/ledger.json`; a server started later answers from that
copy, says how old it is, and works out a fresh one.

### The file itself

| Tool | What it gives |
| --- | --- |
| `inspect(module, section?)` | Headers, sections, imports, exports, resources, version, relocations; one `section` or all of it. Addresses in hex, ordinal imports named. |
| `strings(module, find?, min_len?, offset?, limit?)` | ASCII and UTF-16 text with file offsets. |
| `hex_dump(module, offset?, length?, addr?)` | Bytes from a file offset, or from where an address or function name is stored (up to 16 KB). |
| `resource(module, offset, size, type_id?, wide?)` | One resource: icons, bitmaps and cursors as images, the rest as text. |

### The code

| Tool | What it gives |
| --- | --- |
| `disasm(module, addr, count?)` | At a function's start, the whole function as the analyses decode it; anywhere else (or with `count`), that many instructions straight on. Each line: address, file offset, bytes, instruction, and a note naming what it refers to: a function, an import (through its thunk: `sub_001424 (thunk: KERNEL32!ThunkInitLSF)`), a string, even one in a code section. |
| `xrefs(module, addr)` | What refers to an address or function: call and jump sites (tail jumps included), code that reads, writes or takes it, data that holds it, the export it is, "entered from outside" for exports; for a function, every call it makes. For an import, the code that calls it, directly or through a `jmp [slot]` thunk. |
| `names(module, find?, offset?, limit?)` | Every function and import with its best name and where the name came from. |
| `identify(module)` | Functions a runtime library or a tool made, by signature, and what built the module. |
| `demangle(decorated)` | Microsoft C++ and stdcall decorated names as declarations. |

### The reconstruction

| Tool | What it gives |
| --- | --- |
| `functions(module, cls?, find?, offset?, limit?)` | Every function in the store with its class (verified, weak, unverified, not-judged, no-compile, no-source), name and why; counts and the commonest reasons. Library and tool-made functions carry `library` and are counted under `identified`; `cls="identified"` lists them. |
| `function(module, addr)` | One function: its reconstructed source, verdict and the verdict's detail. Also the resource `codebreaker://function/{module}/{addr}`. |
| `decompile_function(module, addr, wait?)` | One function decompiled now, without a job (kept by its inputs). Not built or judged: `function` has what the store holds. |
| `source_tree(module, find?)` | Where the module's source sits in the product's source tree and why, each file marked source, notes or built, with counts. |
| `projects(find?)`, `project(name, find?)` | The project folders under Converted/ and what was done in each. |
| `reach_results(run?, platform?, judged_only?, find?, offset?, limit?)` | The saved `reach` runs and the programs they found and judged. |
| `read_file(path, offset?, length?)` | A text file from a project folder, the source tree, state/ or this checkout. |

### Two builds

| Tool | What it gives |
| --- | --- |
| `compare_versions(module_a, module_b, status?, offset?, limit?, wait?)` | Two builds function by function: identical, changed (with a likeness score), added, removed, and names the old build gives functions the new one leaves unnamed. Changed pairs first; `fields` says what each field means. |
| `diff_function(module_a, module_b, addr, wait?)` | One function in both builds, lined up by kind of instruction: ` ` the same, `~` the same instruction with other registers or constants, `-` only in the first, `+` only in the second. |

### Jobs and writing

| Tool | What it does |
| --- | --- |
| `commands()`, `command_help(command)` | Every `python -m codebreaker` command, and one in full. |
| `run_command(command, args?)` | Starts a command as a background job and returns its id: decompile, measure, reach --judge, pe-source, recompile-project, xna, java, ... At most two jobs at a time, one measure. |
| `jobs()`, `job(job_id, offset?, tail?)`, `job_file(job_id, offset?, length?)` | This server's jobs, a job's state and output, the file it wrote. |
| `stop_job(job_id)` | Stops a job this server started (that process only). |
| `name_function(module, addr, name)` | Names a function's body in the store, so every module holding the same code gets the name. |

Slow answers (decompile_function, compare_versions, diff_function, the ledger) are worked out in the
background: when one takes longer than `wait` seconds the tool says so, and calling again picks it
up. They are kept for an hour.

### The prompt

`triage` (a prompt, beside the tool of the same name) asks the assistant to take a binary apart
with these tools: the triage tool first, a measure job if nothing is stored, then disasm and xrefs
on each function to look at, and what most likely went wrong.

## A worked example: lz32.dll

The first real use (a Win95 lz32.dll triage) is what most of the reading tools came from:

1. `triage lz32.dll`: a 32-to-16-bit flat thunk (19 functions the thunk compiler made), 17 of 20
   judged functions verified, three not: DllMain (`exit ret vs wild-call`), GetExpandedNameW and
   LZOpenFileW (stack cleanup 4 vs 12 and 16 bytes).
2. `disasm lz32.dll GetExpandedNameW`: `xor eax,eax; mov cl,2; jmp` to a thunk for
   `KERNEL32!CommonUnimpStub (#17)`, which returns EAX and pops CL arguments. The decompiled C calls
   it without those registers, so the cleanup differs; the cause is in the register arguments, not the
   prototype.
3. `xrefs lz32.dll CommonUnimpStub`: both W functions, through the thunk at 0x142A.
4. `disasm lz32.dll sub_001057`: pops its own return address into EDX, pushes three more
   arguments and tail-jumps to ThunkInitLSF, the likely wild call in DllMain.
5. `diff_function lz32.dll <win98 lz32.dll> LZClose`: the same code with registers swapped.

## Not done yet

- The verdict detail does not give the address of a wild call (that is on the emulator side).
- `pe_functions` misses thunks reached only by a jump; disasm and xrefs label them anyway.
- Claude Code cannot fetch MCP prompts or list resource templates, so in it the `triage` tool is
  the way in.
