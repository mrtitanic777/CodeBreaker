# ACHIEVEMENTS.md -- achievement conversion

The program defines 7 achievement(s). A console does NOT let code invent achievements at runtime -- each must be declared in the title configuration and unlocked through a platform call that writes to the signed-in profile over the online service. This maps the program's achievements onto that model.

- profile: **retail** (max 99, 1000 gamerscore)   assigned total: **1000**   delivery: **online**

## Achievement table

| # | Key | Name | Gamerscore | Hidden | Tile art |
|---:|---|---|---:|:---:|---|
| 1 | `openInventory` | Taking Inventory | 145 |  | TODO |
| 2 | `mineWood` | Getting Wood | 145 |  | `..\content\achievement_images\mineWood_unlocked.png` |
| 3 | `buildWorkBench` | Build Work Bench | 145 |  | `..\content\achievement_images\buildWorkBench_unlocked.png` |
| 4 | `diamonds` | Diamonds | 145 | yes | `..\content\achievement_images\diamonds_unlocked.png` |
| 5 | `onARail` | On A Rail | 140 |  | TODO |
| 6 | `cookFish` | Cook Fish | 140 |  | `..\content\achievement_images\cookFish_unlocked.png` |
| 7 | `flyPig` | When Pigs Fly = yes | 140 | yes | TODO |

### Tile art

Achievement tiles were auto-wired from the game's own item/block icons (cropped from the texture atlas, scaled to 64x64; a darkened/desaturated locked variant is generated). They land in `achievement_images/` and are referenced by the XLAST project.

- auto-wired: **4**   needs manual art: **3**

Icons not auto-resolved (add art by hand in the XLAST project):

- `openInventory` — icon (none) not resolvable to an atlas sprite
- `onARail` — icon Item::s_missing not resolvable to an atlas sprite
- `flyPig` — icon (none) not resolvable to an atlas sprite


## What the console requires (and the source program does not provide)

1. **Declared up front.** Every achievement above must be entered into the title's configuration tool (Xbox 360: an XLAST project, compiled to an SPA blob embedded in the build). A skeleton is emitted as `achievements.xlast` -- import it, add the locked/unlocked tile art, and build the SPA.
2. **Unlocked via a networked write.** Each in-code unlock becomes a call to the platform achievement write (Xbox 360: `XUserWriteAchievements`). This is the telemetry/networking path you flagged: it needs
   - a **Title ID** assigned to the build,
   - a **signed-in profile** (tie this to the *User / profile* decision),
   - the **network** for immediate sync -- **offline it queues and syncs on next connect**.
3. **Budget + limits.** Gamerscore must sum to the profile target and the count must stay under the limit (validated above).
4. **Hidden achievements.** The program's "special/secret" flag maps to the console "hidden" attribute (shown above).

## Title ID

The achievement config (SPA) and the built XEX are both keyed to a **32-bit Title ID** (high 16 bits = publisher, low 16 = title number). It must be the SAME in the XLAST project and on the XEX, or unlocks will not bind.

- Title ID: **0xFFFE00A1**

Since Xbox 360 titles are no longer submitted to Microsoft, you **self-issue** this: there is no database to request one from. Pick a value and make sure it does not collide with an existing title (community title-ID lists exist to check against). The suggested id uses a high publisher prefix (`0xFFFE`) unlikely to clash and a title number derived from the name; override it with your own if you prefer.

Set the same value in **Visual Studio -> project properties -> Xbox 360 -> Title ID** (it is written into the XEX at build), and in the XLAST project (already set in the emitted `achievements.xlast`).

## Code seam

Emitted `Achievements.h/.cpp` provides `Achievement_Unlock(userIndex, ACH_*)`. The converter routes each in-code unlock (e.g. the program's `triggerAchievement`) to it; on hardware it performs the platform write, off-hardware it is a safe no-op. The `ACH_*` id constants match the table above.

