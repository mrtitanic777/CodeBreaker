; dosmgr_mcode_02.asm -- DOSMGR object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        and    eax, 5200h
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        db  073h, 075h
        db  070h, 070h
        outsd
        db  072h, 074h
        and    byte ptr fs:[ebp + 53h], cl
        sub    eax, 20534f44h
        db  076h, 065h
        db  072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 0a0d2eh
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        db  073h, 075h
        db  070h, 070h
        outsd
        db  072h, 074h
        and    byte ptr fs:[ebx + 6fh], dh
        db  066h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        db  02Eh, 00Dh, 00Ah, 000h
_LMSGTABLE ends
        end
