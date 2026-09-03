; vmpoll_pcode_05.asm -- VMPOLL object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0EBh, 006h, 0FFh, 025h, 078h, 001h, 000h, 000h
loc_5_0008:
        db  0FFh, 035h, 078h, 001h, 000h, 000h
        jmp    SHORT loc_5_001E
        db  0EBh, 006h, 0FFh, 025h, 07Ch, 001h, 000h, 000h
loc_5_0018:
        db  0FFh, 035h, 07Ch, 001h, 000h, 000h
loc_5_001E:
        cmp    dword ptr [esi - 8], 54534e49h
        je     SHORT loc_5_0049
        cmp    dword ptr [esi - 8], 55544553h
        je     SHORT loc_5_0031
loc_5_0030:
        ret
loc_5_0031:
        cmp    word ptr [esi - 4], 50h
        jne    SHORT loc_5_0030
loc_5_0038:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        xor    byte ptr [ebx + 10h], 4
        db  02Bh, 01Dh, 080h, 001h, 000h, 000h
        ret
loc_5_0049:
        cmp    dword ptr [esi - 4], 4c4c41h
        je     SHORT loc_5_0038
        ret
        db  090h
loc_5_0054:
        db  08Bh, 00Dh, 088h, 001h, 000h, 000h
        db  08Bh, 015h, 084h, 001h, 000h, 000h
        mov    word ptr [ebp + 28h], cx
        mov    dword ptr [ebp + 24h], edx
        nop
loc_5_0068:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        db  081h, 03Bh, 046h, 000h, 000h, 000h
        je     SHORT loc_5_008E
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 0bh
        jbe    SHORT loc_5_0090
        cmp    ax, 4406h
        je     SHORT loc_5_009F
loc_5_0084:
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
loc_5_008E:
        stc
        ret
loc_5_0090:
        je     SHORT loc_5_008E
        cmp    ah, 6
        jne    SHORT loc_5_0084
        cmp    byte ptr [ebp + 14h], 0ffh
        jne    SHORT loc_5_0084
        jmp    SHORT loc_5_008E
loc_5_009F:
        cmp    word ptr [ebp + 10h], 0
        jne    SHORT loc_5_0084
        jmp    SHORT loc_5_008E
_PTEXT ends
        end
