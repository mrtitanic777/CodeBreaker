; vmpoll_pcode_06.asm -- VMPOLL object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_6_000C
        and    byte ptr [ecx + 1], 0fbh
loc_6_000C:
        clc
        ret
        db  08Bh, 0C0h
loc_6_0010:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        db  081h, 03Bh, 046h, 000h, 000h, 000h
        je     SHORT loc_6_002E
        cmp    byte ptr [ebp + 1dh], 3
        je     SHORT loc_6_002E
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
loc_6_002E:
        stc
        ret
loc_6_0030:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        db  081h, 03Bh, 046h, 000h, 000h, 000h
        je     SHORT loc_6_004E
        cmp    byte ptr [ebp + 1dh], 2
        je     SHORT loc_6_004E
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
loc_6_004E:
        stc
        ret
_PTEXT ends
        end
