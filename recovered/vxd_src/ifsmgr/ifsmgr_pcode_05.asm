; ifsmgr_pcode_05.asm -- IFSMGR object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        db  075h, 0D4h
        db  0E8h, 017h, 0B0h, 0FFh, 0FFh
        add    esp, 4
        push   eax
        db  0FFh
loc_5_000C:
        db  075h, 0D4h
        push   dword ptr [ebp - 28h]
        db  0E8h, 008h, 0B0h, 0FFh, 0FFh
        add    esp, 4
        push   eax
        push   dword ptr [ebp - 28h]
        cmp    esi, 1
        sbb    eax, eax
        add    eax, 3
        push   eax
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 18h]
        db  0E8h, 095h, 00Eh, 000h, 000h
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_5_005C
        mov    dword ptr [ebp - 10h], eax
        or     di, di
        je     SHORT loc_5_0063
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax], di
        mov    eax, dword ptr [ebx + 0ch]
        add    word ptr [eax], di
        mov    eax, dword ptr [ebx + 0ch]
        mov    dx, word ptr [ebp - 2]
        add    word ptr [eax + 2], dx
        db  0E9h, 0FEh, 0FEh, 0FFh, 0FFh
loc_5_005C:
        mov    dword ptr [ebp - 1ch], 8
loc_5_0063:
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax], di
        mov    ecx, dword ptr [ebx + 0ch]
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebx + 0ch]
        mov    ax, word ptr [ebp - 6]
        db  066h, 089h
_PTEXT ends
        end
