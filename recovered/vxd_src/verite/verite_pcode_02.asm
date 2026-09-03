; verite_pcode_02.asm -- VERITE object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        mov    ecx, dword ptr [ebp + 18h]
        test   ecx, ecx
        je     SHORT loc_2_0013
        cmp    ecx, 1
        je     SHORT loc_2_0025
        cmp    ecx, 2
        je     SHORT loc_2_0047
        jmp    SHORT loc_2_0054
loc_2_0013:
        db  0A1h, 03Ch, 001h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 040h, 001h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        jmp    SHORT loc_2_0054
loc_2_0025:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, eax
        and    eax, 0ff00h
        db  0A3h, 03Ch, 001h, 000h, 000h
        and    ecx, 0ff000000h
        db  089h, 00Dh, 040h, 001h, 000h, 000h
        db  0E8h, 038h, 000h, 000h, 000h
        jmp    SHORT loc_2_0054
loc_2_0047:
        db  08Bh, 01Dh, 04Ch, 001h, 000h, 000h
        db  0E8h, 05Fh, 000h, 000h, 000h
        jmp    SHORT loc_2_0054
loc_2_0054:
        ret
loc_2_0055:
        pushad
        db  089h, 01Dh, 044h, 001h, 000h, 000h
        db  0E8h, 050h, 000h, 000h, 000h
        popad
        ret
loc_2_0063:
        pushad
        db  08Bh, 01Dh, 04Ch, 001h, 000h, 000h
        db  0E8h, 05Eh, 000h, 000h, 000h
        popad
        ret
loc_2_0071:
        pushad
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  0A1h, 044h, 001h, 000h, 000h
loc_2_007D:
        cmp    eax, edi
        jne    SHORT loc_2_0088
        mov    ebx, edi
        db  0E8h, 045h, 000h, 000h, 000h
loc_2_0088:
        db  08Bh, 01Dh, 04Ch, 001h, 000h, 000h
        db  0E8h, 01Eh, 000h, 000h, 000h
        xor    eax, eax
        db  0A3h, 044h, 001h, 000h, 000h
        popad
        ret
loc_2_009C:
        ret
loc_2_009D:
        ret
loc_2_009E:
        ret
loc_2_009F:
        ret
loc_2_00A0:
        ret
loc_2_00A1:
        ret
_PTEXT ends
        end
